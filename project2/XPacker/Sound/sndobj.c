#include "sndobj.h"
#include "sndplay.h"
#include "sndsuppl.h"


//-----------------------------------------------------------------------------
//local 
DWORD SndObjGetFreeInstance(LPSNDOBJ lpSndObj);

//-----------------------------------------------------------------------------
BOOL SndObjDestroy(LPSNDOBJ lpSndObj)
{
	if(!lpSndObj) 
		return FALSE;
	
	if(lpSndObj->instVector!=NULL){
		int i;
		for(i=(int)(lpSndObj->nInst)-1; i>=0; i--){						
			HwSndObjDestroy(lpSndObj->instVector[i]);
		}
		MyFree(lpSndObj->instVector);
	}		
	if(lpSndObj->lpMem)
		MyFree(lpSndObj->lpMem);
	
	MyFree(lpSndObj);	
	return TRUE;
}


void  SndObjInitProps3D(LPSNDOBJ lpSndObj,LPHWSNDOBJ lpHwSo) 
{
	SND3DPROPS props;
	if(lpSndObj->info.sndType==SND_3D) {		
		HwSndObjGet3DProps(lpHwSo, &props);
		if(lpSndObj->info.param3D.setFlag & SND_INCONESET)				
			props.dwInsideConeAngle=lpSndObj->info.param3D.props.dwInsideConeAngle;
		if(lpSndObj->info.param3D.setFlag & SND_OUTCONESET)	
			props.dwOutsideConeAngle=lpSndObj->info.param3D.props.dwOutsideConeAngle;
		if(lpSndObj->info.param3D.setFlag & SND_OUTVOLSET)	
			props.OutsideVolumePerCent=lpSndObj->info.param3D.props.OutsideVolumePerCent;
		if(lpSndObj->info.param3D.setFlag & SND_MINDSET)
			props.flMinDistance=lpSndObj->info.param3D.props.flMinDistance;
		if(lpSndObj->info.param3D.setFlag & SND_MAXDSET)
			props.flMaxDistance=lpSndObj->info.param3D.props.flMaxDistance;
		HwSndObjSet3DProps(lpHwSo,&props);	
	}
}

void SndObjInitInstVector(LPSNDOBJ lpSndObj, LPDWORD lpSize)
{
	DWORD i;

	lpSndObj->instVector=MyMalloc(LPHWSNDOBJ,lpSndObj->info.nInstRequired);
	ASSERTOUTMEMORY(lpSndObj->instVector);		
	for(i=0; i<lpSndObj->info.nInstRequired; i++){
		lpSndObj->instVector[i]=NULL;
		if(i==0)//I create the first master instance
			lpSndObj->instVector[i]=HwSndObjCreate(lpSndObj->lpMem, lpSize,
												&lpSndObj->fmt, lpSndObj->info.sndType);
		else{
			lpSndObj->instVector[i]=
				HwSndObjDuplicate(lpSndObj->instVector[0]);
			if(lpSndObj->instVector[i]==NULL){
			// I must create another master obj
				lpSndObj->instVector[i]=
					  HwSndObjCreate(lpSndObj->lpMem, lpSize,&lpSndObj->fmt,lpSndObj->info.sndType);				
			}
		}
		if(lpSndObj->instVector[i]==NULL)
			break;	
		else{
			HwSndObjSetCls(lpSndObj->instVector[i],lpSndObj->info.sndClass);
			SndObjInitProps3D(lpSndObj,lpSndObj->instVector[i]);
		}
	}
	lpSndObj->nInst=i;
}

LPSNDOBJ SndObjCreate(SNDINFO *pInfo, LOADHANDLE lBlock, LOADPATH lPath)
{
	LPSNDOBJ lpSndObj=NULL;		
	DWORD lengthData;
	DWORD bytesPerSec;

	if(!HwSndGetCardDetect())
		return NULL;
	ASSERTPOINTER(pInfo);
	lpSndObj=MyMalloc(SNDOBJ,1);
	ASSERTOUTMEMORY(lpSndObj);
	lpSndObj->info=*pInfo;
	if(lpSndObj->info.nInstRequired<1){
		DEBUGASSERT(lpSndObj->info.nInstRequired>=1);
		lpSndObj->info.nInstRequired=1;
	}
	lpSndObj->nInst=0;
	lpSndObj->lpMem=NULL;
	strcpy(lpSndObj->loadInfo.loadBlock,lBlock);
	strcpy(lpSndObj->loadInfo.loadPath,lPath);
	if(!(lpSndObj->info.sndType & SND_STREAMING) || 
										(lpSndObj->info.sndType & SND_LOADRAM)){
		lpSndObj->lpMem=HwSndObjLoad(lpSndObj->loadInfo.loadPath,
							lpSndObj->loadInfo.loadBlock,lpSndObj->info.loadID);
		ASSERTFILE(lpSndObj->lpMem!=NULL,lpSndObj->info.loadID);
	}
	if(!(lpSndObj->info.sndType & SND_STREAMING)){
		SndObjInitInstVector(lpSndObj, &lengthData);
	}
	else {
		 SndObjGetFmt(lpSndObj->lpMem,lpSndObj->loadInfo.loadPath, 
						lpSndObj->loadInfo.loadBlock,lpSndObj->info.loadID,&lpSndObj->fmt,&lengthData);	
		lpSndObj->instVector=NULL;
	}
	if(!(lpSndObj->info.sndType & SND_LOADRAM) && lpSndObj->lpMem){
		MyFree(lpSndObj->lpMem);
		lpSndObj->lpMem=NULL;
	}
	bytesPerSec=lpSndObj->fmt.nChannels*lpSndObj->fmt.nSamplesPerSec*
												(lpSndObj->fmt.wBitsPerSample/8);
	lpSndObj->timeLength=PDIV(INTTOP(lengthData),INTTOP(bytesPerSec));
	
	return lpSndObj;
}

void SndObjCheckRestore(LPSNDOBJ lpSndObj)
{	
	DWORD i;
	if(!lpSndObj || lpSndObj->info.sndType & SND_STREAMING)
		return;
	for(i=0; i<lpSndObj->nInst; i++){
		if(HwSndObjIsToReload(lpSndObj->instVector[i])){			
			if(!lpSndObj->lpMem){
				lpSndObj->lpMem=HwSndObjLoad(lpSndObj->loadInfo.loadPath,
										lpSndObj->loadInfo.loadBlock,lpSndObj->info.loadID);
				ASSERTFILE(lpSndObj->lpMem!=NULL,lpSndObj->info.loadID);
			}
			HwSndObjRestore(lpSndObj->instVector[i],lpSndObj->lpMem);
		}
	}
	if(!(lpSndObj->info.sndType & SND_LOADRAM) && lpSndObj->lpMem){
		MyFree(lpSndObj->lpMem);
		lpSndObj->lpMem=NULL;
	}
}

DWORD SndObjGetFreeInstance(LPSNDOBJ lpSndObj)
{
	DWORD i;	
	DWORD oldestI=0;
	BOOL bPlayFlag;
	
	if(!lpSndObj || !lpSndObj->instVector)
		return lpSndObj->nInst;	
	for(i=0; i<lpSndObj->nInst; i++){
		bPlayFlag=HwSndObjIsPlaying(lpSndObj->instVector[i]);
		if(lpSndObj->instVector[i]->base.reload){
			SndObjCheckRestore(lpSndObj);
			return lpSndObj->nInst; // I don't play the sound
		}
		if(!bPlayFlag)
			break;
	}
	return i; 
}

static DWORD freqToPlay=DSBFREQUENCY_ORIGINAL;
static BOOL bFreqSetted=FALSE;

void SndObjPatchSetFrequency(LPSNDOBJ lpSndObj,DWORD freq)
{
	if(freq==DSBFREQUENCY_ORIGINAL)
		return;
	freqToPlay=freq;
	bFreqSetted=TRUE;
}

void SndObjReset_SetFrequency(void)
{
	bFreqSetted=FALSE;
	freqToPlay=DSBFREQUENCY_ORIGINAL;
}

SNDHANDLE SndObjPlayFreq(LPSNDOBJ lpSndObj,SNDPROP prop,PREAL *lpPosition, DWORD vol,int pan,DWORD freq)
{
	SNDHANDLE hnd;

	SndObjPatchSetFrequency(lpSndObj,freq);
	hnd=SndObjPlay(lpSndObj,prop,lpPosition, vol,pan);
	SndObjReset_SetFrequency();
	return hnd;
}


SNDHANDLE SndObjPlay(LPSNDOBJ lpSndObj,SNDPROP prop,PREAL *lpPosition, DWORD vol,int pan)
{
	DWORD inst;
	BOOL pow;
	SNDHANDLE hnd=SNDNOHANDLE;
	LPHWSNDOBJ lpCurrI=NULL;

	if(!lpSndObj)	
		return hnd;
	if(!HwSndMstGetPower()) // Master Power is OFF
		return hnd;
	if(SndClsGetPower(lpSndObj->info.sndClass,&pow) && pow){
		LPSNDSTREAM lpss = 0;
		if(lpSndObj->info.sndType & SND_STREAMING){
			lpss=SndStreamGetFreeBuffer(lpSndObj->info.sndType,&lpSndObj->fmt);
			if(lpss){
				lpCurrI=lpss->lpHwSo;
				HwSndObjSetCls(lpCurrI,lpSndObj->info.sndClass);
			}
		}
		else{
			inst=SndObjGetFreeInstance(lpSndObj);
			if(inst<lpSndObj->nInst) {						
				lpCurrI=lpSndObj->instVector[inst];
			}
		}
		if(lpCurrI){
			HwSndObjSetVolume(lpCurrI,vol);		
			HwSndObjSetPan(lpCurrI,pan);					
			if(lpSndObj->info.sndType & SND_3D){
				HwSndObjSetLinkObj(lpCurrI,lpPosition);
				SndObjInitProps3D(lpSndObj,lpCurrI);
			}				
			if(bFreqSetted){
				HwSndObjSetFrequency(lpCurrI, freqToPlay);
				//SndObjReset_SetFrequency();
			}
			else
				HwSndObjSetFrequency(lpCurrI, DSBFREQUENCY_ORIGINAL);			
			hnd=SndPlayAllocateHnd(lpCurrI);
			// delay play to compute velocity in 3D how handle? && fill stream buffer
			if(hnd!=SNDNOHANDLE){
				BOOL bFlag;
				if(lpSndObj->info.sndType & SND_STREAMING)
					bFlag=SndStreamPlay(lpss, lpSndObj->lpMem,lpSndObj->loadInfo.loadPath,
									 lpSndObj->loadInfo.loadBlock, lpSndObj->info.loadID,prop);
				else
					bFlag=HwSndObjPlay(lpCurrI,prop);
				//PATCH FOR MY GREAT ERROR: Instance is Free so if playing system
				//think that it's playing I notify it
				if(lpCurrI->hndPlay!=SNDNOHANDLE)
					SndPlayFreeHnd(lpCurrI->hndPlay);
				if(!bFlag){				
					SndPlayFreeHnd(hnd);
					hnd=SNDNOHANDLE;
				}
				else
					HwSndObjSetHnd(lpCurrI,hnd);
			}
		}
	}	
	return hnd;
}


BOOL SndObjIsPlaying(LPSNDOBJ lpSndObj, SNDHANDLE hnd)
{
	BOOL bFlag=FALSE;
	DWORD i;
	if(!lpSndObj || hnd==SNDNOHANDLE)
		return bFlag;		
	for(i=0; i<lpSndObj->nInst; i++) {
		if(HwSndObjGetHnd(lpSndObj->instVector[i])==hnd){
			bFlag=TRUE;
			break;
		}
	}
	return bFlag;	
}

BOOL SndObjGetInfo(LPSNDOBJ lpSndObj, LPSNDMIXFORMAT lpFmt, PREAL *lpTimeLength)
{
	if(lpSndObj){
		if(lpTimeLength)
			*lpTimeLength=lpSndObj->timeLength;
		if(lpFmt)
			*lpFmt=lpSndObj->fmt;
		return TRUE;
	}
	return FALSE;
}
