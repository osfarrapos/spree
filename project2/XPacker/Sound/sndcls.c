/* ------------------------------------------------------------------
	 Sound Class Handler for all Platforms

	Pixelstorm Copyright - by Teodoro Di Bello
 
	Version: 0.6				Date: 16/2/98

	Description: 
		   
	Note:	
  ------------------------------------------------------------------ */
#include "myutil.h"
#include "sndgame.h"
#include "sndcls.h"
#include "sndfade.h"
#include "sndplay.h"
#include "soundmath.h"

typedef struct SNDCLASStag{
	BOOL power;
	DWORD volume;
	DWORD maxVolume;
	int pan;
	SNDFADE sndFade;
} SNDCLASS;

// ------------------------------------------------------------------
// Sound Class Functions Handler
static SNDCLASS sndClassDef[]={
	{1,SNDMAXVOLUME,SNDMAXVOLUME,0}, //SFX
	{1,SNDMAXVOLUME,SNDMAXVOLUME,0}, //COMMENT
	{1,SNDMAXVOLUME,SNDMAXVOLUME,0}, //CROWD
	{1,SNDMAXVOLUME,SNDMAXVOLUME,0}, //BACKGROUND
	{1,SNDMAXVOLUME,SNDMAXVOLUME,0}, //MENU
	{1,SNDMAXVOLUME,SNDMAXVOLUME,0}, //MASTER
	{1,SNDMAXVOLUME,SNDMAXVOLUME,0}  //CDA
};

static SNDCLASS sndClass[N_SNDCLS];

void SndClsResetSystem(void)
{
	int i;
	for (i=0; i<N_SNDCLS; i++){
		sndClass[i]=sndClassDef[i];
		SndFadeInitStruct(&sndClass[i].sndFade,i);
		SndClsSetVolume(i,sndClass[i].volume);
		SndClsSetPower(i,sndClass[i].power);
		SndClsMasterInitFadeVars();
	}
}

BOOL SndClsSetVolume(DWORD cls, DWORD vol)
{	
	if(cls>=N_SNDCLS)
		return FALSE;
	if(vol>=SNDMAXVOLUME) 
		vol=SNDMAXVOLUME;	
	sndClass[cls].volume=vol;
	switch(cls){	
	case SNDCLS_MASTER:		
		return HwSndMstSetVolume(vol);
		break;
	default:
		SndPlayUpdateClsVolume(cls);
		return TRUE;		
		break;
	}	
	return FALSE;
}


BOOL SndClsGetVolume(DWORD cls, DWORD *pVol)
{
	if(cls<N_SNDCLS){
		*pVol=sndClass[cls].volume;
		return TRUE;
	}	
	return FALSE;
}


BOOL SndClsStopAll(DWORD cls)
{		
	if(cls>=N_SNDCLS)
		return FALSE;	
	switch(cls){	
	case SNDCLS_MASTER:		
		HwSndMstStopAll();
		return TRUE;
		break;
	default: //SNDCLS_SFX,SNDCLS_COMMENT,SNDCLS_CROWD, SNDCLS_BACKGROUND,SNDCLS_MENU
		SndPlayClsStop(cls);
		return TRUE;
		break;		
	}	
	return FALSE;	
}


BOOL SndClsSetPower(DWORD cls, BOOL pow)
{	
	if(cls>=N_SNDCLS)
		return FALSE;
	sndClass[cls].power=pow;
	switch(cls){		
	case SNDCLS_MASTER:		
		return HwSndMstSetPower(pow);
		break;
	default: //SNDCLS_SFX,SNDCLS_COMMENT,SNDCLS_CROWD, SNDCLS_BACKGROUND,SNDCLS_MENU
		return ((pow) ? TRUE : SndClsStopAll(cls));				
		break;		
	}	
	return FALSE;
}

BOOL SndClsGetPower(DWORD cls, BOOL *pPow)
{
	if(cls<N_SNDCLS){
		*pPow=sndClass[cls].power;
		return TRUE;
	}
	return FALSE;
}

DWORD SndClsConvertVolume(DWORD cls, DWORD vol)
{
	DWORD volume;
	volume=MUSTIN(vol,0,SNDMAXVOLUME);	
	if(cls<N_SNDCLS){
		volume=SndObjComputePercent(sndClass[cls].volume,
			sndClass[cls].maxVolume, SNDMAXVOLUME);
		volume=SndObjComputePercent(vol,volume, SNDMAXVOLUME);
	}
	return volume;
}

void SndClsFadeStart(DWORD cls, DWORD time, DWORD tgtVol)
{
	if(cls<N_SNDCLS){
		SndFadeStart(&sndClass[cls].sndFade,time,tgtVol);
	}
}

BOOL SndClsFadeContinue(DWORD cls)
{
	if(cls<N_SNDCLS){
		return SndFadeContinue(&sndClass[cls].sndFade);
	}
	return FALSE;
}

void SndClsAllFadeContinue(void)
{
	DWORD cls;
	for(cls=0; cls<N_SNDCLS; cls++)
		 SndClsFadeContinue(cls);	
}

void SndClsFadeStop(DWORD cls, BOOL bRestore)
{
	if(cls<N_SNDCLS){
		SndFadeStop(&sndClass[cls].sndFade,bRestore);
	}
}

BOOL SndClsFadeIsActive(DWORD cls)
{
	if(cls<N_SNDCLS)
		return SndFadeIsActive(&sndClass[cls].sndFade);
	return FALSE;
}
//-------------------------------------------------------------------------------------------

static DWORD masterVolume;
static BOOL bMasterFadeIn=FALSE;
static BOOL bMasterFadeOut=FALSE;
static BOOL bMasterVolumeSetted=FALSE;

void SndClsMasterInitFadeVars(void)
{
	bMasterFadeIn=FALSE;
	bMasterFadeOut=FALSE;		
	bMasterVolumeSetted=FALSE;
	//SndClsGetVolume(SNDCLS_MASTER, &masterVolume);
}

BOOL SndClsSetMasterVolumeForFade(int vol)
{
	if(bMasterFadeIn || bMasterFadeOut){
		if(SndClsFadeIsActive(SNDCLS_MASTER))
			return FALSE;		
		bMasterFadeIn=bMasterFadeOut=FALSE;
	}
	if(vol>=SNDMAXVOLUME) 
		vol=SNDMAXVOLUME;		
	masterVolume=vol;
	bMasterVolumeSetted=TRUE;
	return TRUE;
}

void SndClsMasterRecordVolume(void)
{
	if(bMasterFadeIn && SndClsFadeIsActive(SNDCLS_MASTER)){
		;
	}
	else if(bMasterFadeOut && SndClsFadeIsActive(SNDCLS_MASTER)){
		;	
	}
	else if(bMasterVolumeSetted)
		bMasterVolumeSetted=FALSE;
	else
		SndClsGetVolume(SNDCLS_MASTER, &masterVolume);
}

void SndClsMasterFadeIn(DWORD time)
{
	if(!SndGameIsInitialized() || !sndClass[SNDCLS_MASTER].power){
		bMasterFadeOut=FALSE;
		bMasterFadeIn=TRUE;
		return;	
	}
	SndClsMasterRecordVolume();	
	bMasterFadeIn=TRUE;
	bMasterFadeOut=FALSE;
	SndClsSetVolume(SNDCLS_MASTER,0);
	SndClsFadeStart(SNDCLS_MASTER, time, masterVolume);
}

void SndClsMasterFadeOut(DWORD time)
{
	if(!SndGameIsInitialized() || !sndClass[SNDCLS_MASTER].power){
		bMasterFadeIn=FALSE;
		bMasterFadeOut=TRUE;
		return;	
	}
	SndClsMasterRecordVolume();
	bMasterFadeIn=FALSE;
	bMasterFadeOut=TRUE;
	SndClsFadeStart(SNDCLS_MASTER, time, 0);
}

BOOL SndClsMasterFadeIsActive(void)
{
	if(!SndClsFadeIsActive(SNDCLS_MASTER)){		
		return FALSE;
	}
	return (bMasterFadeIn || bMasterFadeOut);	
}

BOOL SndClsMasterQuery(void)
{
	if(!SndGameIsInitialized() || !sndClass[SNDCLS_MASTER].power){
		if(bMasterFadeOut){
			bMasterFadeOut=FALSE;
			return TRUE;
		}
		return FALSE;
	}	
	if(bMasterFadeOut){
		if(!SndClsFadeIsActive(SNDCLS_MASTER)){		
			bMasterFadeOut=FALSE;
			SndClsSetMasterVolumeForFade(masterVolume);
			return TRUE;
		}
	}	
	return FALSE;
}

void SndClsMasterFadeOutStop(void)
{
	if(bMasterFadeOut){	
		bMasterFadeOut=FALSE;
		SndFadeStop(&sndClass[SNDCLS_MASTER].sndFade,FALSE);
		SndClsSetVolume(SNDCLS_MASTER,masterVolume);
	}
}
