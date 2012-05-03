/* ------------------------------------------------------------------
	 Low Level Sound Handler for Pc/Win95 Platform

	Pixelstorm Copyright - by Teodoro Di Bello
 
	Version: 0.6				Date: 16/2/98

	Description: It's based on the old version 0.5 but totally revised
		   
	Note:	
  ------------------------------------------------------------------ */
#include <stdio.h>
#include "sndlow.h" 
#include "sndplay.h"
#include "sndsuppl.h"
#include "soundmath.h"
#include "myerror.h"
#include "myutil.h"
#include "funcalloc.h"
#include "gameset.h" //for SCALE_FACTOR definition !!!!!!
 
// in mode emulation debbuging it is the max of 3d sound handled by D3D
#define HWMAX3DSNDEMULATE 20
// cooperative level
#define SNDCOOPLEVEL  DSSCL_EXCLUSIVE

//Local Global Variables
//Direct sound Object
static LPDIRECTSOUND lpDS = NULL;
//Primary Buffer
static LPDIRECTSOUNDBUFFER lpDSBuffPrimary = NULL;
// Listener main object
static LPDIRECTSOUND3DLISTENER  gp3DListener = NULL;
// it shows the number of 3d buffer available (if 0 no 3d sound)
static DWORD n3DSndBuf =0;
// the caps of sound card
static DSCAPS dsCaps;
static SNDENV actSndEnv;

//--------------------------------------------------------------------------
// Local Functions Declaration
// DS Initialization
static BOOL CreatePrimarySoundBuffer(SNDENV *psndEnv);
static BOOL InitPrimarySoundBuffer(SNDENV *psndEnv);
static BOOL InitDSObject(SNDENV *psndEnv);
static BOOL CheckEmulLevel(SNDENV *psndEnv);
static void ReleaseBuffPrimary(void);
static void Release3DListener(void);
static void ReleaseDS(void);
static void PREALtoD3DVALUE(PREAL *lpPr, LPD3DVALUE lpD3Dv);
static void MYVtoD3DVECT(MYVECTOR myV,LPD3DVECTOR lpD3DVect);
BOOL HwSndInit3DEnv(void);

//--------------------------------------------------------------------------
// Functions to handle master object: return DS Object, Primary buffer Volume 
// cd sound part I see it as low level sound

//#include "cdplay.h"

typedef struct SNDOBJLISTITEMtag{
	LPHWSNDOBJ lpSo;
	struct SNDOBJLISTITEMtag *next;
	struct SNDOBJLISTITEMtag *prev;
}SNDOBJLISTITEM, *LPSNDOBJLISTITEM;

static struct HWSNDMASTERtag{
	DWORD vol;
	BOOL pow;
	LONG winVol;
	LPSNDOBJLISTITEM head;
	LPSNDOBJLISTITEM tail;	
	DWORD nObj;
} hwSndMaster ={SNDMAXVOLUME,1,0,NULL,NULL,0};

LPSNDOBJLISTITEM  HwSndMstFindObj(LPHWSNDOBJ lpSo);
void HwSndMstReset(void);


BOOL HwSndGetCardDetect(void)
{
	return (lpDS!=NULL);
}

void *HwSndMstGetObj(void)
{
	return (void *) lpDS;
}

BOOL HwSndPrimarySetVolume(DWORD vol)
{
	LONG hwVol;
	HRESULT hr;
	BOOL bFlag=TRUE;
	if(lpDSBuffPrimary){		
		hwVol=HwSndObjConvertVolume(vol);
		hr=lpDSBuffPrimary->lpVtbl->SetVolume(lpDSBuffPrimary, hwVol);//DSBVOLUME_MAX
		if(FAILED(hr)){
			DEBUGMYERROR(DSTAB,hr);		
			bFlag=FALSE;
		}
	}
	return bFlag;
}

BOOL HwSndPrimaryStop(void)
{
	HRESULT hr;
	if(!lpDSBuffPrimary)
		return FALSE;

	if( FAILED( hr = lpDSBuffPrimary->lpVtbl->Stop(lpDSBuffPrimary))) {
		DEBUGMYERROR(DSTAB,hr);
		return FALSE;			
	}
	return TRUE;
}

BOOL HwSndPrimaryPlay(void)
{
	HRESULT hr;
	if(!lpDSBuffPrimary)
		return FALSE;
	if( FAILED( hr = lpDSBuffPrimary->lpVtbl->Play(lpDSBuffPrimary, 0, 0, DSBPLAY_LOOPING ))) {
		DEBUGMYERROR(DSTAB,hr);
		return FALSE;			
	}
	return TRUE;
}


void HwSndMstRegObj(LPHWSNDOBJ lpSo)
{	
	LPSNDOBJLISTITEM newItem;
	if(!lpSo) //no real sound to reg
		return ;
	newItem=MyMalloc(SNDOBJLISTITEM,1);
	ASSERTOUTMEMORY(newItem);
	if(hwSndMaster.head==NULL){ //init list	
		hwSndMaster.head=newItem;					
	}
	else		
		hwSndMaster.tail->next=newItem;
	newItem->prev=hwSndMaster.tail;
	hwSndMaster.tail=newItem;		
	newItem->next=NULL;  
	newItem->lpSo=lpSo;
	hwSndMaster.nObj++;
	return ;	
}

LPSNDOBJLISTITEM  HwSndMstFindObj(LPHWSNDOBJ lpSo)
{
	LPSNDOBJLISTITEM scan;
	if(lpSo==NULL)
		return NULL;
	scan=hwSndMaster.head;
	while(scan!=NULL && scan->lpSo!=lpSo)
		scan=scan->next;
	return scan;
}

void HwSndMstDelObj(LPHWSNDOBJ lpSo)
{
	LPSNDOBJLISTITEM listItem;
	if(!lpSo) //no real buffer to destroy
		return; 
	listItem=HwSndMstFindObj(lpSo);
	if(listItem==NULL)
		return;
	if(listItem->prev==NULL && listItem->next==NULL){ //it's  the only element
		hwSndMaster.tail=NULL;
		hwSndMaster.head=NULL;
	}else if(listItem->prev==NULL){//it is the first element
		hwSndMaster.head=listItem->next;	
		hwSndMaster.head->prev=NULL; 	//lpHwSo->next->prev=NULL;
	}else if(listItem->next==NULL){ // it is the last element
		hwSndMaster.tail=listItem->prev;		
		hwSndMaster.tail->next=NULL; //lpHwSo->prev->next=NULL;			
	}else{ //it's a  normal element
		listItem->prev->next=listItem->next;	
		listItem->next->prev=listItem->prev;
	}
	DEBUGASSERT(hwSndMaster.nObj>0);
	hwSndMaster.nObj--;
	MyFree(listItem);
}

void HwSndMstReset(void)
{
	hwSndMaster.vol=SNDMAXVOLUME;
	hwSndMaster.pow=TRUE;
	hwSndMaster.winVol=0;
	hwSndMaster.head=NULL;
	hwSndMaster.tail=NULL;	
	hwSndMaster.nObj=0;
}

BOOL HwSndMstSetVolume(DWORD vol)
{
	BOOL bFlag=TRUE;	
	bFlag=HwSndPrimarySetVolume(vol);
	if(bFlag){
		hwSndMaster.vol=vol;
	}
	return bFlag;
}

DWORD HwSndMstGetVolume(void)
{
	return hwSndMaster.vol;	
}

BOOL HwSndMstGetPower(void)
{
	return hwSndMaster.pow;
}

void HwSndMstStopAll(void)
{
	LPSNDOBJLISTITEM scan;
	scan=hwSndMaster.head;

	while(scan){
		if(scan->lpSo)
			HwSndObjStop(scan->lpSo);
		scan=scan->next;
	}
}

BOOL HwSndMstSetPower(BOOL pow)
{
	hwSndMaster.pow=pow;
	if(!hwSndMaster.pow){ //stop all		
		HwSndMstStopAll();		
		HwSndPrimaryStop();
	}
	else{// restart mixer
		HwSndPrimaryPlay();
	}	
	return TRUE;
}

//--------------------------------------------------------------------------
// to help the use of HwSndInitSystem

void HwSndInitDefaultEnv(SNDENV *lpSndEnv,PLATFHWND hwnd)
{
	ASSERTPOINTER(lpSndEnv);
	lpSndEnv->hWndMain=hwnd;
	lpSndEnv->fmt.nChannels=2;
	lpSndEnv->fmt.nSamplesPerSec=22050; 
	lpSndEnv->fmt.wBitsPerSample=16;
	lpSndEnv->enable3DSnd=TRUE;
	lpSndEnv->n3DSndForced=HWMAX3DSNDEMULATE;//FALSE;
}

//--------------------------------------------------------------------------
// It creates the primary buffer, checks if it's CTRL3D compatible
// Returns FALSE if failed!

BOOL CreatePrimarySoundBuffer(SNDENV *psndEnv)
{
	HRESULT hr;
    DSBUFFERDESC dsbd;
	BOOL bflag=TRUE;
	
	ASSERTPOINTER(lpDS && psndEnv);	

	INITTOZERO(&dsbd,sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
    dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER|DSBCAPS_CTRLVOLUME;
	
	if(psndEnv->enable3DSnd) 
		dsbd.dwFlags|=DSBCAPS_CTRL3D;
	hr=lpDS->lpVtbl->CreateSoundBuffer(lpDS,&dsbd,&lpDSBuffPrimary,NULL);
	if(FAILED(hr)){ //if it fails and 3d is enabled we try without DSBCAPS_CTRL3D
		ReleaseBuffPrimary();
		bflag=FALSE;
		//DEBUGMYERROR(DSTAB,hr);
		if(psndEnv->enable3DSnd){			
			psndEnv->enable3DSnd=FALSE;
			bflag=CreatePrimarySoundBuffer(psndEnv);//recursive call only one level!!
		}
		else
			DEBUGMYERROR(DSTAB,hr);
	}
	return bflag;
}

//--------------------------------------------------------------------------
// It initialize the primary buffer, set the snd format, making it compatible
// to the sound card capabilities, and then play it in loop mode!
// If 3D sound is enable, it also creates the SOUND 3DLISTENER Object
// Returns FALSE if failed!

BOOL InitPrimarySoundBuffer(SNDENV *psndEnv)
{	
	WAVEFORMATEX wfx;
	HRESULT hr;			

	ASSERTPOINTER(psndEnv!=NULL);
	SndFmt2WaveFmt(&psndEnv->fmt,&wfx);
	//We not check the return value because try to configure it
	 //to the lowest format, anyway; also if it appear to support
	 //no format
	MakeWaveFmtCompatible(&wfx, &dsCaps); 
	WaveFmt2SndFmt(&wfx,&psndEnv->fmt);
	hr=lpDSBuffPrimary->lpVtbl->SetFormat(lpDSBuffPrimary,&wfx);		
	if(FAILED(hr)){
		DEBUGMYERROR(DSTAB,hr);
		return FALSE;
	} 
	else{		
		if(psndEnv->enable3DSnd){			
			if( FAILED( hr = lpDSBuffPrimary->lpVtbl->QueryInterface
								(lpDSBuffPrimary,&IID_IDirectSound3DListener,
													(void **) &gp3DListener))){
				gp3DListener=NULL;
				psndEnv->enable3DSnd=FALSE;
				DEBUGMYERROR(DSTAB,hr);
			}			
		}
		//it also play the primary buffer if power is on
		if(!HwSndMstSetPower(HwSndMstGetPower()))
			return FALSE;
		hr=lpDSBuffPrimary->lpVtbl->GetVolume(lpDSBuffPrimary,&hwSndMaster.winVol);
		DEBUGMYERROR(DSTAB,hr);
		HwSndMstSetVolume(HwSndMstGetVolume());//DSBVOLUME_MAX	
	}	
	return TRUE;
}

//--------------------------------------------------------------------------
// It create the ds object and set cooperative level, and set caps
// It uses only default sound card (not do any enumeration)!
// Returns FALSE if failed!

BOOL InitDSObject(SNDENV *psndEnv)
{
	HRESULT hr;
	
	hr=DirectSoundCreate(NULL,&lpDS,NULL);
	if(FAILED(hr)){	// it's not possible to create ds object, we set no sound available
		lpDS=NULL;
		if(hr!= DSERR_NODRIVER )
			DEBUGMYERROR(DSTAB,hr);
		return FALSE;
	}
	hr=lpDS->lpVtbl->SetCooperativeLevel(lpDS,psndEnv->hWndMain,SNDCOOPLEVEL);//psndEnv->coopLev PRIORITY
	if(FAILED(hr)){	// it's not possible to set coop lev, we set no sound available
		DEBUGMYERROR(DSTAB,hr);
		return FALSE;
	}	
	return TRUE;
}


//--------------------------------------------------------------------------
// Set emulation, checking if the snd card supports the format in HAL
BOOL CheckEmulLevel(SNDENV *psndEnv)
{
	WAVEFORMATEX wfx;
	HRESULT hr;
	hr=lpDSBuffPrimary->lpVtbl->GetFormat(lpDSBuffPrimary,&wfx,sizeof(wfx),NULL);
	
	if(FAILED(hr)){ //It's very difficult, but ...
		psndEnv->emulLev=2;
		DEBUGMYERROR(DSTAB,hr);
	}
	else
		if(psndEnv->fmt.nChannels==wfx.nChannels &&
						psndEnv->fmt.nSamplesPerSec==wfx.nSamplesPerSec &&
						psndEnv->fmt.wBitsPerSample==wfx.wBitsPerSample		){
			psndEnv->emulLev=0;		
		}
		else{
			if( (dsCaps.dwFlags & DSCAPS_EMULDRIVER)==0 ) // DS driver installed
				psndEnv->emulLev=1;
			else
				psndEnv->emulLev=2; //No DS driver installed, win emulation
		}
	
	return TRUE;
}
//--------------------------------------------------------------------------
// Main routine to inizialize sound card
// We first create and initialize the direct sound object
// then create the primary buffer;
// finally we try to initialize it with the format requested,
// if it fails we try with another format, the successfull format is
// saved in the input SNDENV object. If no format is available it fails!
// DWORD maxVolume: it shows the max value of volume and then 
// how many volume steps are required
// Returns FALSE if fails!

BOOL HwSndInitSystem(SNDENV *psndEnv)
{ 
	BOOL bfInit=FALSE;
	BOOL bfRetry=TRUE;
	HRESULT hr;
	
	if(lpDS!=NULL){
		actSndEnv=*psndEnv;
		return TRUE; //Already initialized
	}
	HwSndMstReset();
	CreateVolumeTable();
	CreatePanTable();
	while(bfRetry){
		if(InitDSObject(psndEnv)==TRUE){
			dsCaps.dwSize=sizeof(dsCaps);
			hr=lpDS->lpVtbl->GetCaps(lpDS,&dsCaps);
			if(FAILED(hr)) {				
				DEBUGMYERROR(DSTAB,hr);
				break;
			}
			if(CreatePrimarySoundBuffer(psndEnv)==TRUE){									
					// Check if the sound card support really 3D acc.
					// i.e. if it has or not dedicated 3D buffers
				if(psndEnv->enable3DSnd){
					n3DSndBuf=dsCaps.dwMaxHw3DAllBuffers;
					psndEnv->hwSupport3D=(n3DSndBuf>0);
					n3DSndBuf+=psndEnv->n3DSndForced;					
					psndEnv->enable3DSnd=(n3DSndBuf>0);
				}
				// set primary buffer property
				if(InitPrimarySoundBuffer(psndEnv)==TRUE){
					bfRetry=FALSE;
					bfInit=TRUE;
					CheckEmulLevel(psndEnv);
				}
				else
					bfRetry=TryOtherSndFormat(&(psndEnv->fmt));				
			}
			else
				bfRetry=FALSE;					
		} 
		else
			bfRetry=FALSE;
		if(bfInit==FALSE)
			HwSndCloseSystem();
	}
	if(bfInit){						
		if(!psndEnv->enable3DSnd){
			Release3DListener();
			HwSndInit3DEnv();
		}
		else{
			psndEnv->enable3DSnd=HwSndInit3DEnv(); 
			if(!psndEnv->enable3DSnd){				
				Release3DListener();
			}	
		}
		if(!psndEnv->hwSupport3D) // if not hw support we don't use doppler
			HwSndSetDopplerFact(PVALUE(0.0f));

		if(!psndEnv->enable3DSnd)
			n3DSndBuf=0;
		actSndEnv=*psndEnv;
	}

	return bfInit;
}

 
BOOL HwSndDebugCheckFormat(WAVEFORMATEX  *lpWfx)
{	
	if(lpDSBuffPrimary){
		HRESULT hr;
		hr=lpDSBuffPrimary->lpVtbl->GetFormat(lpDSBuffPrimary,lpWfx,sizeof(WAVEFORMATEX),NULL);
		if(FAILED(hr)){
			DEBUGMYERROR(DSTAB,hr);
			return FALSE;
		}		
	}
	return FALSE;
}
//--------------------------------------------------------------------------
// Routines to close and release sound objects

void ReleaseBuffPrimary(void)
{
	HRESULT hr;
	if( lpDSBuffPrimary!=NULL ) {
		// should we check if there are any sound buffers playing?		
		hr=lpDSBuffPrimary->lpVtbl->Stop(lpDSBuffPrimary);
		if(FAILED(hr))
			DEBUGMYERROR(DSTAB,hr);
		// we reset volume to  extern value of win95
		hr=lpDSBuffPrimary->lpVtbl->SetVolume(lpDSBuffPrimary,hwSndMaster.winVol);
		if(FAILED(hr))
			DEBUGMYERROR(DSTAB,hr);
		hr=lpDSBuffPrimary->lpVtbl->Release(lpDSBuffPrimary);
		DEBUGASSERT(hr==0);			
		lpDSBuffPrimary = NULL;
	}
}

void Release3DListener(void)
{
	HRESULT hr;
	if( gp3DListener!=NULL ) {
		hr=gp3DListener->lpVtbl->Release(gp3DListener);
		DEBUGASSERT(hr==0);
		gp3DListener = NULL;
	}
}

void ReleaseDS(void)
{
	HRESULT hr;
	if( lpDS!=NULL ){
		hr=lpDS->lpVtbl->Release(lpDS);
		DEBUGASSERT(hr==0);
		lpDS = NULL;
	}
}

void HwSndCloseSystem(void)
{
	Release3DListener();
	ReleaseBuffPrimary();
	ReleaseDS();		
}

//--------------------------------------------------------------------------
// for 3D sound system part

#define APPLY3D_FLAG DS3D_DEFERRED  //DS3D_IMMEDIATE
#define BUFF3D_MODE SND3D_NORMAL //DS3DMODE_NORMAL //DS3DMODE_HEADRELATIVE 
					//default mode of 3d buffer
	
typedef struct SND3DENVIRONMENTtag{
	SNDLISTENER listener;
	PREAL flDistanceFactor;
	PREAL flRolloffFactor;
	PREAL flDopplerFactor;
	BOOL enable;
} SND3DENVIRONMENT;


static SND3DENVIRONMENT snd3DEnvDef={
	{	//listener
		{	
			{0.0,0.0,0.0,1.0}, //vPosition
			{0.0,0.0,0.0,1.0}, //vVelocity
			{0.0,0.0,1.0,1.0}, //vOrientFront
			{0.0,1.0,0.0,1.0}, //vOrientTop	
		},
		{NULL,NULL,NULL}
	},
    PREPDIV(PUNO,SCALE_FACTOR),//DS3D_DEFAULTDISTANCEFACTOR, //flDistanceFactor	=1.0f (0.0f-10.0f)	
	DS3D_DEFAULTROLLOFFFACTOR, //flRolloffFactor	=1.0f (0.0f-10.0f)
	DS3D_DEFAULTDOPPLERFACTOR, //flDopplerFactor	=1.0f (0.0f-10.0f)
	TRUE,
};

static SND3DENVIRONMENT snd3DEnv;

BOOL HwSndSetListenerObj(LPSNDLISTENER_OBJ lplo)
{
	HRESULT hr;
	D3DVECTOR d3dvct,d3dvctTop;
	BOOL bFlag=TRUE;
		
	ASSERTPOINTER(lplo);	
	
	if(gp3DListener){
		MYVtoD3DVECT(lplo->vPosition,&d3dvct);
		hr=gp3DListener->lpVtbl->SetPosition(gp3DListener,d3dvct.x,
										d3dvct.y,d3dvct.z, APPLY3D_FLAG);
		if(FAILED(hr)){
			DEBUGMYERROR(DSTAB,hr);		
			bFlag=FALSE;
		}

		MYVtoD3DVECT(lplo->vVelocity,&d3dvct);
		hr=gp3DListener->lpVtbl->SetVelocity(gp3DListener,d3dvct.x,
										d3dvct.y,d3dvct.z, APPLY3D_FLAG);
		if(FAILED(hr)){
			DEBUGMYERROR(DSTAB,hr);		
			bFlag=FALSE;
		}
		MYVtoD3DVECT(lplo->vOrientFront,&d3dvct);
		MYVtoD3DVECT(lplo->vOrientTop,&d3dvctTop);
		hr=gp3DListener->lpVtbl->SetOrientation(gp3DListener,d3dvct.x,
				d3dvct.y,d3dvct.z, d3dvctTop.x,d3dvctTop.y,d3dvctTop.z,APPLY3D_FLAG);
		if(FAILED(hr)){
			DEBUGMYERROR(DSTAB,hr);		
			bFlag=FALSE;
		}
	}
	snd3DEnv.listener.obj=*lplo;
	return bFlag;
}

BOOL HwSndSetDistanceFact(PREAL d)
{
	HRESULT hr;
	D3DVALUE d3dv;
	BOOL bFlag=TRUE;
	
	if( PLESSEQ(d, PVALUE(0)) )
		d=PVALUE(FLT_MIN);	
	
	if(gp3DListener){
		PREALtoD3DVALUE(&d,&d3dv);
		hr=gp3DListener->lpVtbl->SetDistanceFactor(gp3DListener,d3dv, APPLY3D_FLAG);	
		if(FAILED(hr)){
			DEBUGMYERROR(DSTAB,hr);		
			bFlag=FALSE;
		}			
	}
	snd3DEnv.flDistanceFactor=d;
	return bFlag;
}

BOOL HwSndSetRolloffFact(PREAL d)
{
	HRESULT hr;
	D3DVALUE d3dv;
	BOOL bFlag=TRUE;

	if( PLESS(d, PVALUE(DS3D_MINROLLOFFFACTOR)) )
		d=PVALUE(DS3D_MINROLLOFFFACTOR);	
	else if(PGREAT(d,PVALUE(DS3D_MAXROLLOFFFACTOR)) )
		d=PVALUE(DS3D_MAXROLLOFFFACTOR);
	if(gp3DListener){
		PREALtoD3DVALUE(&d,&d3dv);
		hr=gp3DListener->lpVtbl->SetDistanceFactor(gp3DListener,d3dv, APPLY3D_FLAG);	
		if(FAILED(hr)){
			DEBUGMYERROR(DSTAB,hr);		
			bFlag=FALSE;
		}	
	}
	snd3DEnv.flRolloffFactor=d;
	return bFlag;
}

BOOL HwSndSetDopplerFact(PREAL d)
{
	HRESULT hr;
	D3DVALUE d3dv;
	BOOL bFlag=TRUE;

	if( PLESS(d, PVALUE(DS3D_MINDOPPLERFACTOR)) )
		d=PVALUE(DS3D_MINDOPPLERFACTOR);	
	else if(PGREAT(d,PVALUE(DS3D_MAXDOPPLERFACTOR)) )
		d=PVALUE(DS3D_MAXDOPPLERFACTOR);
	if(gp3DListener){
		PREALtoD3DVALUE(&d,&d3dv);
		hr=gp3DListener->lpVtbl->SetDopplerFactor(gp3DListener,d3dv, APPLY3D_FLAG);
	
		if(FAILED(hr)){
			DEBUGMYERROR(DSTAB,hr);		
			bFlag=FALSE;
		}
	}
	snd3DEnv.flDopplerFactor=d;
	return bFlag;	
}


BOOL HwSndSet3DEnable(BOOL enable)
{
	LPSNDOBJLISTITEM scan;
	snd3DEnv.enable=enable;
	scan=hwSndMaster.head;
	while(scan){
		HwSndObjRefresh3DMode(scan->lpSo);
		scan=scan->next;
	}
	return TRUE;
}

BOOL HwSndGetListenerObj(LPSNDLISTENER_OBJ lplo){
	ASSERTPOINTER(lplo);
	COPYMEMORY(lplo,&snd3DEnv.listener.obj,sizeof(SNDLISTENER_OBJ));
	return TRUE;
}

PREAL HwSndGetDistanceFact(void)
{
	return snd3DEnv.flDistanceFactor;
}

PREAL HwSndGetRolloffFact(void)
{
	return snd3DEnv.flRolloffFactor;
}

PREAL HwSndGetDopplerFact(void)
{
	return snd3DEnv.flDopplerFactor;
}


BOOL HwSndGet3DEnable(void)
{
	return snd3DEnv.enable;
}

BOOL HwSndInit3DEnv(void)
{
	BOOL bFlag;
	bFlag=HwSndSetListenerObj(&snd3DEnvDef.listener.obj);
	bFlag= bFlag && HwSndSetDistanceFact(snd3DEnvDef.flDistanceFactor);
	bFlag= bFlag && HwSndSetRolloffFact(snd3DEnvDef.flRolloffFactor);
	bFlag= bFlag && HwSndSetDopplerFact(snd3DEnvDef.flDopplerFactor);
	bFlag= bFlag && HwSndSet3DEnable(snd3DEnvDef.enable);
	return bFlag;
}

void HwSndLinkListener(PREAL *lpPosition, PREAL *lpOrientFront,PREAL *lpOrientTop)
{	
	snd3DEnv.listener.linkObj.lpPosition=lpPosition;
	//sndLs.lpVelocity=NULL; //lpVelocity;
	snd3DEnv.listener.linkObj.lpOrientFront=lpOrientFront;
	snd3DEnv.listener.linkObj.lpOrientTop=lpOrientTop;			
}

BOOL HwSndUpdateListener(PREAL deltaT)
{
	LPSNDLISTENER_LINKOBJ lpLinkObj;
	LPSNDLISTENER_OBJ lpObj;
	BOOL bChange=FALSE;
	
	
	lpLinkObj=&snd3DEnv.listener.linkObj;
	lpObj=&snd3DEnv.listener.obj;
	if(lpLinkObj->lpPosition!=NULL){		
		if(PGREAT(deltaT,0)){
			MYVECTOR newVel;
			SndObjComputeVelocity(newVel, lpObj->vPosition,lpLinkObj->lpPosition, deltaT);
			if(!HwSndCompareMYVECTOR(lpObj->vVelocity,newVel)){			
				COPYMEMORY(lpObj->vVelocity,newVel,sizeof(newVel));
				bChange=TRUE;
			}
		}
		if(!HwSndCompareMYVECTOR(lpObj->vPosition,lpLinkObj->lpPosition)){			
			COPYMEMORY(lpObj->vPosition,lpLinkObj->lpPosition,sizeof(MYVECTOR));
			bChange=TRUE;
		}				
	}	
	if(lpLinkObj->lpOrientFront!=NULL &&
			!HwSndCompareMYVECTOR(lpObj->vOrientFront,lpLinkObj->lpOrientFront)){
		COPYMEMORY(lpObj->vOrientFront,lpLinkObj->lpOrientFront,sizeof(MYVECTOR));		
		bChange=TRUE;
	}
	if(lpLinkObj->lpOrientTop!=NULL &&
			!HwSndCompareMYVECTOR(lpObj->vOrientTop,lpLinkObj->lpOrientTop)){
		COPYMEMORY(lpObj->vOrientTop,lpLinkObj->lpOrientTop,sizeof(MYVECTOR));		
		bChange=TRUE;
	}	
	if(bChange)
		bChange=HwSndSetListenerObj(lpObj);
	return bChange;
}

BOOL HwSnd3DExecuteChanges(void)
{
	HRESULT hr;	
	if(gp3DListener){		
		hr=gp3DListener->lpVtbl->CommitDeferredSettings(gp3DListener);
		if(FAILED(hr)){
			DEBUGMYERROR(DSTAB,hr);
			return FALSE;
		}
	}
	return TRUE;
}

//--------------------------------------------------------------------------
// Check if hw snd obj is stored in sound card memory

BOOL HwSndObjIsInHwMem(LPHWSNDOBJ lpHwSo)
{
	DSBCAPS dsb;
	HRESULT hr;	
	
	//DEBUGASSERT(lpHwSo);
	if(!lpHwSo || !lpHwSo->base.sndBuf) 
		return FALSE;
	dsb.dwSize=sizeof(DSBCAPS);
	hr=lpHwSo->base.sndBuf->lpVtbl->GetCaps(lpHwSo->base.sndBuf,&dsb);
	if(FAILED(hr)){
		DEBUGMYERROR(DSTAB,hr);
		return FALSE;
	
	}else 
		return (dsb.dwFlags & DSBCAPS_LOCHARDWARE)>0;
}
//--------------------------------------------------------------------------
BOOL HwSndObjDestroy(LPHWSNDOBJ lpSO)
{
	HRESULT hr;	
	if(!lpDS)	return FALSE;
	if(!lpSO) return FALSE;		
	HwSndObjStop(lpSO);
	//Free handle
	if(lpSO->hndPlay!=SNDNOHANDLE)
		SndPlayFreeHnd(lpSO->hndPlay);
	HwSndMstDelObj(lpSO);
	if(lpSO->p3D.sndBuf3D){
		hr=lpSO->p3D.sndBuf3D->lpVtbl->Release(lpSO->p3D.sndBuf3D);
		lpSO->p3D.sndBuf3D=NULL;
		n3DSndBuf++;
	}
	if(lpSO->base.sndBuf){
		hr=lpSO->base.sndBuf->lpVtbl->Release(lpSO->base.sndBuf);
		lpSO->base.sndBuf=NULL;
	}
	MyFree(lpSO);
	hr=lpDS->lpVtbl->Compact(lpDS);
	DEBUGMYERROR(DSTAB,hr);
	return TRUE;
}

//--------------------------------------------------------------------------
		
// Create a 3D part of hw sound obj
BOOL HwSndObjCreate3DBuffer(LPHWSNDOBJ lpHwSo)
{
	HRESULT hr;	
	if(!lpDS) return FALSE;
	ASSERTPOINTER(lpHwSo && lpHwSo->base.sndBuf);	
	lpHwSo->p3D.sndBuf3D=NULL;				
	if(n3DSndBuf>0 &&!lpHwSo->p3D.emul3D){// it needs to check buffer availability
		hr=lpHwSo->base.sndBuf->lpVtbl->QueryInterface
				(lpHwSo->base.sndBuf,&IID_IDirectSound3DBuffer, &(lpHwSo->p3D.sndBuf3D));				
		if (FAILED(hr)) { 
			lpHwSo->p3D.sndBuf3D=NULL;				
		}
		else
			n3DSndBuf--;	
	}
	if(lpHwSo->p3D.sndBuf3D==NULL) // It fails it's  a 3D sound emulate	
		lpHwSo->p3D.emul3D=TRUE;	
	// set params default
	HwSndObjInit3DProps(lpHwSo);
	HwSndObjInit3DCynem(lpHwSo);
	lpHwSo->p3D.linkObj.lpPosition=NULL;
	return TRUE;
}

//--------------------------------------------------------------------------
// Restore a buffer, if it's lost
BOOL HwSndObjRestore(LPHWSNDOBJ lpSO,LPVOID lpMem)
{	
	HRESULT hr;	
	if(lpDS==NULL) return FALSE;
	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf));
	if(!lpSO || !lpSO->base.sndBuf) 
		return FALSE;
	if(!lpSO->base.reload)
		return FALSE;
	if(!FAILED(hr=lpSO->base.sndBuf->lpVtbl->Restore(lpSO->base.sndBuf)) ){	
		LPWAVEFORMATEX  lpFmt;
		BYTE    *pSampleMem;
		DWORD   sampleSize;
		if(lpMem!=NULL){
			if( ParseWaveMemory(lpMem,&lpFmt, &pSampleMem,&sampleSize) &&
				DSFillSoundBuffer(lpSO->base.sndBuf, pSampleMem, sampleSize))
				lpSO->base.reload=FALSE;												
		}
		else{
			if(DSFillSoundBufferByte(lpSO->base.sndBuf,
				SNDGETSILENCE(lpSO->base.fmt.wBitsPerSample), lpSO->base.size))
				lpSO->base.reload=FALSE;										
		}
	}
	else
		DEBUGMYERROR(DSTAB,hr);	
	return (lpSO->base.reload==FALSE);
}

//--------------------------------------------------------------------------
// Create the hw obj. Returns NULL if fails
// Arguments:
// lpMem: sample buffer  (wav file)
// size : buffer size
// lpFmt: sample format
// sndType: sound type
// if lpMem!=NULL, it creates a buffer of type sndType from lpMem and fill lpFmt and size
// else it creates a buffer of type sndType and of size lpSize and format lpFmt
// and fill it of silence
LPHWSNDOBJ HwSndObjCreate(LPVOID lpMem, DWORD *lpSize, SNDMIXFORMAT *lpFmt, DWORD sndType)
{ 
	DSBUFFERDESC dsBD;
	BYTE *pSampleMem=NULL;
	LPDIRECTSOUNDBUFFER lpDsb=NULL;
	LPHWSNDOBJ lpHwSo=NULL;
	HRESULT hr;
	LPWAVEFORMATEX lpwfxFormat=NULL;
	
	if(!lpDS) 
		return NULL;
	//check ds object  & check parameter 
	ASSERTPOINTER(lpSize && lpFmt);
	
	lpHwSo=MyMalloc(HWSNDOBJ,1);
	ASSERTOUTMEMORY(lpHwSo!=NULL);	

	memset(&dsBD,0, sizeof(dsBD));
	dsBD.dwSize=sizeof(DSBUFFERDESC);
		
	// I check if 3d is supported by system
	if((sndType & SND_3D) && n3DSndBuf==0){		
		lpHwSo->p3D.emul3D=TRUE;
		dsBD.dwFlags=HwSndType2Flag(sndType&(~(SND_3D)));
	}
	else {
		lpHwSo->p3D.emul3D=FALSE;	
		dsBD.dwFlags=HwSndType2Flag(sndType);
	}

	if(lpMem){
		if(!ParseWaveMemory(lpMem,&dsBD.lpwfxFormat, &pSampleMem,&dsBD.dwBufferBytes)){
			HwSndObjDestroy(lpHwSo);
			return NULL;
		}	
		WaveFmt2SndFmt(dsBD.lpwfxFormat,lpFmt);
		*lpSize=dsBD.dwBufferBytes;
	}
	else{
		lpwfxFormat=MyMalloc(WAVEFORMATEX,1);
		ASSERTOUTMEMORY(lpwfxFormat);
		dsBD.lpwfxFormat=lpwfxFormat;
		SndFmt2WaveFmt(lpFmt,dsBD.lpwfxFormat);
		dsBD.dwBufferBytes=*lpSize;
	}
	lpHwSo->base.fmt=*lpFmt;
	lpHwSo->base.size=dsBD.dwBufferBytes;
	hr=lpDS->lpVtbl->CreateSoundBuffer(lpDS, &dsBD,&lpDsb,NULL);
	if(	FAILED(hr))	{
		lpDsb=NULL;
		if((sndType & SND_3D) && !lpHwSo->p3D.emul3D){			
			lpHwSo->p3D.emul3D=TRUE;
			dsBD.dwFlags=HwSndType2Flag(sndType&(~(SND_3D)));			
			hr=lpDS->lpVtbl->CreateSoundBuffer(lpDS, &dsBD,&lpDsb,NULL);
			if(FAILED(hr)){
				HwSndObjDestroy(lpHwSo);							
				DEBUGMYERROR(DSTAB,hr);
				return NULL;
			}			
		}	
		if(lpwfxFormat)
			MyFree(lpwfxFormat);
	}
	if(lpDsb!=NULL){ //Ok continue to create
		if(lpMem){
			if(!DSFillSoundBuffer(lpDsb, pSampleMem, dsBD.dwBufferBytes) ){
				HwSndObjDestroy(lpHwSo);
				return NULL;
			}			
		}
		else{
			if(!DSFillSoundBufferByte(lpDsb, SNDGETSILENCE(lpFmt->wBitsPerSample), 
																dsBD.dwBufferBytes) ){
				HwSndObjDestroy(lpHwSo);
				return NULL;
			}			
		}
		lpHwSo->base.sndBuf=lpDsb;
		lpHwSo->base.bufPos=0;	
		lpHwSo->base.reload=FALSE;
		lpHwSo->base.propPlay=SNDPROP_NO;
		lpHwSo->base.volume=SNDMAXVOLUME;
		lpHwSo->base.pan=0;
		lpHwSo->base.loadInHw=HwSndObjIsInHwMem(lpHwSo);
		lpHwSo->hndPlay=SNDNOHANDLE;
		HwSndMstRegObj(lpHwSo);
		
		if(sndType & SND_3D){ // now query 3D interface	also if emulate
			// because it must init some 3d properties
			HwSndObjCreate3DBuffer(lpHwSo);			
		}
	}
	return lpHwSo;	
}

LPHWSNDOBJ HwSndObjDuplicate(LPHWSNDOBJ lpHwSo)
{
	LPHWSNDOBJ lpNewHwSndObj=NULL;
	HRESULT hr;
	if(!lpDS) return NULL;
	DEBUGASSERT(!(!lpHwSo || !lpHwSo->base.sndBuf));
	if(!lpHwSo || !lpHwSo->base.sndBuf) 
		return FALSE;
	lpNewHwSndObj=MyMalloc(HWSNDOBJ,1);
	ASSERTOUTMEMORY(lpNewHwSndObj!=NULL);	
	COPYMEMORY(lpNewHwSndObj,lpHwSo,sizeof(HWSNDOBJ));
	//sndType=lpHwSo->lpFather->info.sndType;
	lpNewHwSndObj->base.sndBuf=NULL;
	//lpNewHwSndObj->duplicate=TRUE;
	hr=lpDS->lpVtbl->DuplicateSoundBuffer(lpDS,lpHwSo->base.sndBuf,
		&lpNewHwSndObj->base.sndBuf);	
	if(FAILED(hr)){
		HwSndObjDestroy(lpNewHwSndObj);			
		lpNewHwSndObj=NULL;
	}
	else{
		if(lpHwSo->p3D.emul3D || lpHwSo->p3D.sndBuf3D){
			if(n3DSndBuf==0)
				lpNewHwSndObj->p3D.emul3D=TRUE;		
			if(!HwSndObjCreate3DBuffer(lpNewHwSndObj)){
				HwSndObjDestroy(lpNewHwSndObj);			
				lpNewHwSndObj=NULL;
			}			
		}
	}
	return lpNewHwSndObj;
}

BOOL HwSndObjIsToReload(LPHWSNDOBJ lphwso)
{
	if(!lphwso)
		return FALSE;
	return lphwso->base.reload;
}

BOOL HwSndObjIs3DSound(LPHWSNDOBJ lphwso)
{
	if(!lphwso)
		return FALSE;
	return (lphwso->p3D.emul3D || lphwso->p3D.sndBuf3D);	
}
//-----------------------------------------------------------------------------

//No control because only called by other functions
BOOL HwSndObjPlayBase(LPHWSNDOBJ lpSO)
{
	HRESULT hr;
	DWORD dwFlags=0;

	if(!HwSndMstGetPower())
		return FALSE;
	
	if(lpSO->base.propPlay & SNDPROP_LOOP) 
		dwFlags|=DSBPLAY_LOOPING;
	if(lpSO->base.sndBuf){	
		hr=lpSO->base.sndBuf->lpVtbl->Play(lpSO->base.sndBuf, 0, 0, dwFlags); 
		if(hr==DSERR_BUFFERLOST)
			lpSO->base.reload=TRUE;	    
		else if(hr!=DS_OK)
			DEBUGMYERROR(DSTAB,hr);
		return hr==DS_OK;
	}
	else
		return FALSE;
}	

BOOL HwSndObjPlay(LPHWSNDOBJ lpSO, SNDPROP sndProp)
{		
	HRESULT hr;
	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf) );
	if(!lpSO || !lpSO->base.sndBuf) 
		return FALSE;		
	lpSO->base.propPlay=sndProp;	
	hr=lpSO->base.sndBuf->lpVtbl->SetCurrentPosition(lpSO->base.sndBuf,0);
	if(FAILED(hr))
		DEBUGMYERROR(DSTAB,hr);
	return HwSndObjPlayBase(lpSO);
}

BOOL HwSndObjResume(LPHWSNDOBJ lpSO)
{		
	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf) );
	if(!lpSO || !lpSO->base.sndBuf) 
		return FALSE;		
	return HwSndObjPlayBase(lpSO);
}

//--------------------------------------------------------------------------
// Stop & Pause & is playing sound

BOOL HwSndObjStop(LPHWSNDOBJ lpSO)
{	
	HRESULT hr;
	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf) );
	_try {
		if(!lpSO || !lpSO->base.sndBuf) return FALSE;
		hr=lpSO->base.sndBuf->lpVtbl->Stop(lpSO->base.sndBuf);
		if(FAILED(hr)){
			DEBUGMYERROR(DSTAB,hr);
			return FALSE;
		}
		hr=lpSO->base.sndBuf->lpVtbl->SetCurrentPosition(lpSO->base.sndBuf, 0);
		if(FAILED(hr)){
			DEBUGMYERROR(DSTAB,hr);
			return FALSE;
		}
	}
	_except(1) {
		return TRUE;
	}
	return TRUE;
}

BOOL HwSndObjPause(LPHWSNDOBJ lpSO)
{	
	HRESULT hr;
	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf) );
	if(!lpSO || !lpSO->base.sndBuf) return FALSE;
	hr=lpSO->base.sndBuf->lpVtbl->Stop(lpSO->base.sndBuf);	
	if(FAILED(hr)){
		DEBUGMYERROR(DSTAB,hr);
		return FALSE;
	}
	return TRUE;
}


BOOL HwSndObjIsPlaying(LPHWSNDOBJ lpSO)
{
	DWORD status;
	HRESULT hr;

	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf) );
	if(!lpSO||!lpSO->base.sndBuf) return FALSE;	
	hr=lpSO->base.sndBuf->lpVtbl->GetStatus(lpSO->base.sndBuf,&status);
	if(FAILED(hr)){
		DEBUGMYERROR(DSTAB,hr);		
		return FALSE;
	}
	else{
		lpSO->base.reload=(status &DSBSTATUS_BUFFERLOST);
		return ((status & DSBSTATUS_PLAYING)>0) ;//|| status &DSBSTATUS_LOOPING);
	}
}


BOOL HwSndObjRefreshVolume(LPHWSNDOBJ lpSO)
{
	HRESULT hr;
	LONG hwVol;
	DWORD volume;
	
	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf) );
	if(!lpSO || !lpSO->base.sndBuf) return FALSE;	

	volume=SndClsConvertVolume(lpSO->cls, lpSO->base.volume);
	hwVol=HwSndObjConvertVolume(volume);
		//sndVolumeTable[volume];
	hr=lpSO->base.sndBuf->lpVtbl->SetVolume(lpSO->base.sndBuf,hwVol);
	if(FAILED(hr)){
		DEBUGMYERROR(DSTAB,hr);
		return FALSE;
	}
	else
		return TRUE;
}

BOOL HwSndObjSetVolume(LPHWSNDOBJ lpSO, DWORD volume)
{	
	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf) );
	if(!lpSO || !lpSO->base.sndBuf) return FALSE;	
	lpSO->base.volume=volume;	
	return HwSndObjRefreshVolume(lpSO);
}

BOOL HwSndObjGetVolume(LPHWSNDOBJ lpSO, DWORD *pVolume)
{	
	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf) );
	if(!lpSO || !lpSO->base.sndBuf) return FALSE;	
	ASSERTPOINTER(pVolume);
	*pVolume=lpSO->base.volume;
	return TRUE;
}

//pan is the weight, pan=-100 all sound on the left, pan=0 NOPAN, 
// pan=100 all sound on the  right; It's expressed in percent
// In DS it's implemented by attenuating the other channel

BOOL HwSndObjSetPan(LPHWSNDOBJ lpSO, int pan)
{
	HRESULT hr;
	LONG hwPan;
	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf) );
	if(!lpSO || !lpSO->base.sndBuf ) 
		return FALSE;	
	hwPan=HwSndObjConvertPan(pan);
	hr=lpSO->base.sndBuf->lpVtbl->SetPan(lpSO->base.sndBuf,hwPan);
	if(FAILED(hr)){
		DEBUGMYERROR(DSTAB,hr);
		return FALSE;
	}
	else{
		lpSO->base.pan=pan;
		return TRUE;
	}
}

BOOL HwSndObjGetPan(LPHWSNDOBJ lpSO,int *pPan)
{
	ASSERTPOINTER(pPan);
	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf) );	
	if(!lpSO || !lpSO->base.sndBuf)  
		return FALSE;	
	*pPan=lpSO->base.pan;

	return TRUE;
}


BOOL HwSndObjSetHnd(LPHWSNDOBJ lpHwSo,SNDHANDLE hnd)
{
	if(!lpHwSo)
		return FALSE;
	lpHwSo->hndPlay=hnd;
	return TRUE;
}

SNDHANDLE HwSndObjGetHnd(LPHWSNDOBJ lpHwSo)
{
	if(!lpHwSo)
		return SNDNOHANDLE;
	else
		return lpHwSo->hndPlay;		
}

void HwSndObjSetCls(LPHWSNDOBJ lpHwSo,DWORD cls)
{
	if(lpHwSo)
		lpHwSo->cls=cls;
}

DWORD HwSndObjGetCls(LPHWSNDOBJ lpHwSo)
{	
	if(lpHwSo)
		return lpHwSo->cls;
	else
		return SNDCLS_NONE;
}

BOOL HwSndObjSetFrequency(LPHWSNDOBJ lpSO, DWORD freq)
{
	HRESULT hr;	
	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf) );
	if(!lpSO || !lpSO->base.sndBuf ) 
		return FALSE;		
	hr=lpSO->base.sndBuf->lpVtbl->SetFrequency(lpSO->base.sndBuf,freq);
	if(FAILED(hr)){
		DEBUGMYERROR(DSTAB,hr);
		return FALSE;
	}
	else		
		return TRUE;
}

BOOL HwSndObjGetFrequency(LPHWSNDOBJ lpSO, LPDWORD lpFreq)
{
	HRESULT hr;			
	DEBUGASSERT( !(!lpSO || !lpSO->base.sndBuf) );
	if(!lpSO || !lpSO->base.sndBuf ) 
		return FALSE;		
	hr=lpSO->base.sndBuf->lpVtbl->GetFrequency(lpSO->base.sndBuf,lpFreq);
	if(FAILED(hr)){
		DEBUGMYERROR(DSTAB,hr);
		return FALSE;
	}else		
		return TRUE;
}

//--------------------------------------------------------------------------

BOOL HwSndSet3DEmulation(LPHWSNDOBJ lpHwSo, BOOL bEmul)
{
	if(!lpHwSo->p3D.sndBuf3D)
		lpHwSo->p3D.emul3D=bEmul;
	return TRUE;
}

BOOL HwSndEmulate3DEffects(LPHWSNDOBJ lpHwSo)
{
	LONG volume,vRange;
	PREAL d,dMin2,dMax2,dRange,dFact2,dStep;
	HRESULT hr;	
	LPSNDLISTENER_OBJ snd3dListener=&snd3DEnv.listener.obj;
	LPHWSND3DPART lpP3D;
	LPDIRECTSOUNDBUFFER sndBuf;
	MYVECTOR vOrigin={0,0,0,1};
	
	if(!lpHwSo || !lpHwSo->base.sndBuf) return FALSE;
	
	lpP3D=&lpHwSo->p3D;
	sndBuf=lpHwSo->base.sndBuf;
	if(!HwSndGet3DEnable() || lpP3D->dwMode==SND3D_DISABLE)
		return FALSE;
	//Compute Distance from listener
	if(lpP3D->dwMode==SND3D_NORMAL)
		d=HwSndSquareDistance(lpP3D->cynemObj.vPosition,snd3dListener->vPosition,
		snd3DEnv.flDistanceFactor);
	else
		d=HwSndSquareDistance(lpP3D->cynemObj.vPosition,vOrigin,
		snd3DEnv.flDistanceFactor);
	dFact2=PSQUARE(snd3DEnv.flDistanceFactor);
	dMin2=PMULT(PSQUARE(lpP3D->props.flMinDistance),dFact2);
	dMax2=PMULT(PSQUARE(lpP3D->props.flMaxDistance),dFact2);
	//Compute Volume from distance
	if(PLESSEQ(d,dMin2))
		volume=DSBVOLUME_MAX;
	else if(PLESSEQ(dMax2,d))
		volume=DSBVOLUME_MIN;
	else{
		hr=sndBuf->lpVtbl->GetVolume(sndBuf, &volume);				
		if(FAILED(hr)){
			DEBUGMYERROR(DSTAB,hr);
			return FALSE;
		}
		dRange=PSUB(dMax2,dMin2);
		vRange=volume-DSBVOLUME_MIN;
		dStep=PDIV(dRange,INTTOP(vRange));	
		// I Know that PREAL is Float	
		volume-=((int)(PDIV(PSUB(d,dMin2),dStep)));
	}
	hr=sndBuf->lpVtbl->SetVolume(sndBuf,volume);
	if(FAILED(hr)){
		DEBUGMYERROR(DSTAB,hr);
		return FALSE;
	}	
	// Compute pan from relative position to listener
	// Not implemented
	
	return TRUE;
}

void HwSndObjRefresh3DMode(LPHWSNDOBJ lpHwSo)
{
	if(lpHwSo && lpHwSo->p3D.sndBuf3D){
		DWORD dsBuffMode;
		HRESULT hr;
		if(!HwSndGet3DEnable())
			dsBuffMode=DS3DMODE_DISABLE;
		else
			dsBuffMode=HwSndConvertMode(lpHwSo->p3D.dwMode);
		hr=lpHwSo->p3D.sndBuf3D->lpVtbl->
				SetMode(lpHwSo->p3D.sndBuf3D,dsBuffMode,APPLY3D_FLAG);
		DEBUGMYERROR(DSTAB,hr);
	}
}

BOOL HwSndObjApply3DProps(LPHWSNDOBJ lpHwSo)
{	
	LPSND3DPROPS lpProps;
	DWORD insideVol;
	LPDIRECTSOUND3DBUFFER lpSb3D;	
	BOOL bFlag=TRUE;
	if(!lpHwSo) return FALSE;

	lpProps=&lpHwSo->p3D.props;
	insideVol=lpHwSo->base.volume;
	lpSb3D=lpHwSo->p3D.sndBuf3D;
	
	lpProps->dwInsideConeAngle=
		MUSTIN(lpProps->dwInsideConeAngle, DS3D_MINCONEANGLE, DS3D_MAXCONEANGLE);
	lpProps->dwOutsideConeAngle=
		MUSTIN(lpProps->dwOutsideConeAngle, lpProps->dwInsideConeAngle, DS3D_MAXCONEANGLE);	
	if(lpProps->OutsideVolumePerCent>100)
		lpProps->OutsideVolumePerCent=100;		
	if(lpProps->flMinDistance<0.0)
		lpProps->flMinDistance=0.0;		
	if(lpProps->flMaxDistance<lpProps->flMinDistance)
		lpProps->flMaxDistance=lpProps->flMinDistance;		
	if(lpHwSo->p3D.dwMode!=SND3D_DISABLE && lpHwSo->p3D.dwMode!=SND3D_HEADRELATIVE &&
		lpHwSo->p3D.dwMode!=SND3D_NORMAL)
		lpHwSo->p3D.dwMode=SND3D_NORMAL;
	if(lpSb3D){
		HRESULT hr;
		D3DVALUE d3dvalue;
		LONG lConeOutsideVolume;
		DWORD outsideVol;
		
		hr=lpSb3D->lpVtbl->SetConeAngles(lpSb3D,lpProps->dwInsideConeAngle,
			lpProps->dwOutsideConeAngle,APPLY3D_FLAG);
		DEBUGMYERROR(DSTAB,hr);
		
		PREALtoD3DVALUE(&lpProps->flMinDistance,&d3dvalue);
		hr=lpSb3D->lpVtbl->SetMinDistance(lpSb3D,d3dvalue,APPLY3D_FLAG);		
		DEBUGMYERROR(DSTAB,hr);
		
		PREALtoD3DVALUE(&lpProps->flMaxDistance,&d3dvalue);
		hr=lpSb3D->lpVtbl->SetMaxDistance(lpSb3D,d3dvalue,APPLY3D_FLAG);
		DEBUGMYERROR(DSTAB,hr);		
		
		HwSndObjRefresh3DMode(lpHwSo);
		
		outsideVol=SndObjComputePercent(lpProps->OutsideVolumePerCent,insideVol,100);
		lConeOutsideVolume=HwSndObjConvertVolume(outsideVol)-
			HwSndObjConvertVolume(insideVol);
		
		hr=lpSb3D->lpVtbl->SetConeOutsideVolume(lpSb3D,lConeOutsideVolume,APPLY3D_FLAG);
		DEBUGMYERROR(DSTAB,hr);		
	}
	return bFlag;
}

BOOL HwSndObjApply3DCynem(LPHWSNDOBJ lpHwSo)
{
	LPSND3DCYNEMATIC lpCynem;	
	LPDIRECTSOUND3DBUFFER lpSb3D;	
	BOOL bFlag=TRUE;
	if(!lpHwSo)
		return FALSE;

	lpCynem=&lpHwSo->p3D.cynemObj;
	lpSb3D=lpHwSo->p3D.sndBuf3D;
	if(lpSb3D!=NULL){
		D3DVECTOR d3dvector;
		HRESULT hr;
		MYVtoD3DVECT(lpCynem->vPosition,&d3dvector);
		hr=lpSb3D->lpVtbl->SetPosition(lpSb3D,d3dvector.x, d3dvector.y,
			d3dvector.z, APPLY3D_FLAG);
		DEBUGMYERROR(DSTAB,hr);
		
		MYVtoD3DVECT(lpCynem->vVelocity,&d3dvector);	
		hr=lpSb3D->lpVtbl->SetVelocity(lpSb3D,d3dvector.x, d3dvector.y,
			d3dvector.z, APPLY3D_FLAG);
		DEBUGMYERROR(DSTAB,hr);
		
		MYVtoD3DVECT(lpCynem->vConeOrientation,&d3dvector);    
		hr=lpSb3D->lpVtbl->SetConeOrientation(lpSb3D,d3dvector.x, d3dvector.y,
			d3dvector.z, APPLY3D_FLAG);
		DEBUGMYERROR(DSTAB,hr);
	}
	else 
		bFlag=HwSndEmulate3DEffects(lpHwSo); //emulate
	return bFlag;
}


void HwSndObjInit3DProps(LPHWSNDOBJ lpHwSo)
{
	if(!lpHwSo) 
		return;
	lpHwSo->p3D.props.dwInsideConeAngle=DS3D_DEFAULTCONEANGLE;
	lpHwSo->p3D.props.dwOutsideConeAngle=DS3D_DEFAULTCONEANGLE;
	lpHwSo->p3D.props.OutsideVolumePerCent=100; 
	lpHwSo->p3D.props.flMinDistance=PVALUE(DS3D_DEFAULTMINDISTANCE);
	lpHwSo->p3D.props.flMaxDistance=PVALUE(DS3D_DEFAULTMAXDISTANCE);
	lpHwSo->p3D.dwMode=BUFF3D_MODE;
	HwSndObjApply3DProps(lpHwSo);
}

void HwSndObjInit3DCynem(LPHWSNDOBJ lpHwSo)
{
	if(!lpHwSo) 
		return;
	lpHwSo->p3D.cynemObj.vPosition[0]=lpHwSo->p3D.cynemObj.vVelocity[0]=0; 
	lpHwSo->p3D.cynemObj.vPosition[1]=lpHwSo->p3D.cynemObj.vVelocity[1]=0; 
	lpHwSo->p3D.cynemObj.vPosition[2]=lpHwSo->p3D.cynemObj.vVelocity[2]=0; 
	
	lpHwSo->p3D.cynemObj.vConeOrientation[0]=0; 
	lpHwSo->p3D.cynemObj.vConeOrientation[1]=0; 
	lpHwSo->p3D.cynemObj.vConeOrientation[2]=1; 
	
	lpHwSo->p3D.cynemObj.vConeOrientation[3]=lpHwSo->p3D.cynemObj.vPosition[3]=
											 lpHwSo->p3D.cynemObj.vVelocity[3]=1;     
	HwSndObjApply3DCynem(lpHwSo);
}


BOOL HwSndObjGet3DProps(LPHWSNDOBJ lpHwSo, LPSND3DPROPS lpProps)
{	
	if(!lpHwSo)
		return FALSE;
	ASSERTPOINTER(lpProps);
	*lpProps=lpHwSo->p3D.props;
	return TRUE;
}

BOOL HwSndObjGet3DCynem(LPHWSNDOBJ lpHwSo, LPSND3DCYNEMATIC lpCynem)
{
	if(!lpHwSo)
		return FALSE;
	ASSERTPOINTER(lpCynem);
	*lpCynem=lpHwSo->p3D.cynemObj;
	return TRUE;
}

BOOL HwSndObjSet3DProps(LPHWSNDOBJ lpHwSo, LPSND3DPROPS lpProps)
{
	if(!lpHwSo)
		return FALSE;
	ASSERTPOINTER(lpProps);	
	lpHwSo->p3D.props=*lpProps;
	HwSndObjApply3DProps(lpHwSo);
	return TRUE;
}

BOOL HwSndObjSet3DCynem(LPHWSNDOBJ lpHwSo, LPSND3DCYNEMATIC lpNewCynem)
{	
	if(!lpHwSo)
		return FALSE;
	ASSERTPOINTER(lpNewCynem);		
	lpHwSo->p3D.cynemObj=*lpNewCynem;
	HwSndObjApply3DCynem(lpHwSo);
	return TRUE;
}

BOOL HwSndObjSetLinkObj(LPHWSNDOBJ lpHwSo, PREAL *lpPosition)
{	
	if(!lpHwSo)
		return FALSE;
	lpHwSo->p3D.linkObj.lpPosition=lpPosition;	
	return TRUE;
}

BOOL HwSndObjUpdate3D(LPHWSNDOBJ lpHwSo,PREAL deltaT, BOOL bForce)
{
	BOOL bChange=bForce;	
	PREAL *lpPosition;
	LPSND3DCYNEMATIC lpCynem;
	if(lpHwSo==NULL) 
		return FALSE;
	lpPosition=lpHwSo->p3D.linkObj.lpPosition;
	lpCynem=&lpHwSo->p3D.cynemObj;
	if(lpPosition){
		MYVECTOR currVel;		
		//compute currVel
		if(deltaT>0){
			SndObjComputeVelocity(currVel, lpCynem->vPosition, lpPosition, deltaT);
			if(	!HwSndCompareMYVECTOR(currVel, lpCynem->vVelocity)){
				COPYMEMORY(lpCynem->vVelocity,currVel,sizeof(MYVECTOR));
				bChange=TRUE;
			}
		}
		if(!HwSndCompareMYVECTOR(lpPosition, lpCynem->vPosition)){
			COPYMEMORY(lpCynem->vPosition,lpPosition,sizeof(MYVECTOR));
			bChange=TRUE;
		}		
		//COPYMEMORY(lpCynem->vConeOrientation,lpCynem->vVelocity,sizeof(MYVECTOR));		
	}
	if(bChange)
		bChange=HwSndObjApply3DCynem(lpHwSo);		
	return bChange;
}


LPWINDS_SNDENV HwSndCreateWinDSObj(PLATFHWND hWndMain, LPSNDMIXFORMAT lpFmt)
{
	HRESULT hr;	
	LPWINDS_SNDENV lpWinDsEnv=NULL;
	DSBUFFERDESC dsbd;
	WAVEFORMATEX wfx;
	
	ASSERTPOINTER(lpFmt);
	
	if(HwSndGetCardDetect()){
		lpWinDsEnv=MyMalloc(WINDS_SNDENV,1);	
		ASSERTOUTMEMORY(lpWinDsEnv);
		hr=DirectSoundCreate(NULL,&(lpWinDsEnv->lpDS),NULL);
		if(FAILED(hr)){
			goto NoCreate;
		}
		hr=lpWinDsEnv->lpDS->lpVtbl->SetCooperativeLevel(lpWinDsEnv->lpDS,hWndMain,SNDCOOPLEVEL);//psndEnv->coopLev PRIORITY
		if(FAILED(hr)){
			goto NoCreate;			
		}
		INITTOZERO(&dsbd,sizeof(DSBUFFERDESC));
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER|DSBCAPS_CTRLVOLUME;
		hr=lpWinDsEnv->lpDS->lpVtbl->CreateSoundBuffer(lpWinDsEnv->lpDS,&dsbd,&
														lpWinDsEnv->lpDSBuffPrimary,NULL);
		if(FAILED(hr)){
			goto NoCreate;
		}
		SndFmt2WaveFmt(lpFmt,&wfx);
		//MakeWaveFmtCompatible(&wfx, &dsCaps); 
		WaveFmt2SndFmt(&wfx,lpFmt);
		hr=lpWinDsEnv->lpDSBuffPrimary->lpVtbl->SetFormat(lpWinDsEnv->lpDSBuffPrimary,&wfx);
		if(FAILED(hr)){
			goto NoCreate;
		}
		lpWinDsEnv->linkToMainDS=FALSE;	
	}
	return lpWinDsEnv;
NoCreate:
	DEBUGMYERROR(DSTAB, hr);
	lpWinDsEnv->lpDS=lpDS;
	lpWinDsEnv->lpDSBuffPrimary=lpDSBuffPrimary;
	lpWinDsEnv->linkToMainDS=TRUE;
	return lpWinDsEnv;

}

void HwSndDestroyWinDSObj(LPWINDS_SNDENV lpWinDsEnv)
{
	if(lpWinDsEnv){	
		if(!lpWinDsEnv->linkToMainDS){
			if(lpWinDsEnv->lpDSBuffPrimary){
				lpWinDsEnv->lpDSBuffPrimary->lpVtbl->Stop(lpWinDsEnv->lpDSBuffPrimary);
				lpWinDsEnv->lpDSBuffPrimary->lpVtbl->Release(lpWinDsEnv->lpDSBuffPrimary);		
				lpWinDsEnv->lpDSBuffPrimary=NULL;
			}
			if(lpWinDsEnv->lpDS){
				lpWinDsEnv->lpDS->lpVtbl->Release(lpWinDsEnv->lpDS);
				lpWinDsEnv->lpDS=NULL;
			}
		}
		MyFree(lpWinDsEnv);
	}
}

static void PREALtoD3DVALUE(PREAL *lpPr, LPD3DVALUE lpD3Dv)
{
	*lpD3Dv=*lpPr;
}

static void MYVtoD3DVECT(MYVECTOR myV,LPD3DVECTOR lpD3DVect)
{
	if (myV[3]!=PVALUE(1.0)){
		lpD3DVect->x=PDIV(myV[0],myV[3]);
		lpD3DVect->y=PDIV(myV[1],myV[3]);	
		lpD3DVect->z=PDIV(myV[2],myV[3]);	
	}
	else{
		lpD3DVect->x=myV[0];
		lpD3DVect->y=myV[1];	
		lpD3DVect->z=myV[2];	
	}
}
