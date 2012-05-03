/* ------------------------------------------------------------------
	 Sound Game Main Handler

	Pixelstorm Copyright - by Teodoro Di Bello
 
	Version: 0.5		Date: 19/01/98

	Description: 
		It is the main interface of all sound system. 
		It also includes Cd play System.
	Note:	
  ------------------------------------------------------------------ */

//PSS PRESS DEMO 21-2-99 DA SPOSTARE???

//#define _PSS_PRESS_DEMO
#include "sndgame.h"
#ifndef _SND_TEDDY_USE
	#include "ballmote.h"
#endif
static BOOL initFlag=FALSE;

//local functions for movie
void SndGameInitMovie( PLATFHWND hwnd);
void SndGameDestroyMovie(void);

void SndGameCreateStreams(void)
{
	/* 
	SndStreamCreate(FALSE,TRUE, SND_3D, 1);
	SndStreamCreate(FALSE,FALSE, SND_3D, 1);
	SndStreamCreate(FALSE,TRUE, 0, 1);
	SndStreamCreate(FALSE,FALSE, 0, 1);
	*/
}


BOOL SndGameInit(PLATFHWND hwnd)
{
	SNDENV sndEnv;
	if(initFlag==TRUE) //already initialized
		return TRUE; 
	initFlag=TRUE;
	SndInitGlobalClock();	
	HwSndInitDefaultEnv(&sndEnv,hwnd);

	if(initFlag && !HwSndInitSystem(&sndEnv))
		initFlag= FALSE;		
	if(initFlag){	//check if ok and then not initialize sound ??
		SndGameInitMovie(hwnd);
	}
	if(initFlag && !SndScnTblLoad())
		initFlag=FALSE;
	if(initFlag && !SndAnimTblLoad())
		initFlag= FALSE;	
	if(initFlag){
		SndClsResetSystem();
		SndStreamInitSystem();
		SndPlaySlotInitSystem();
		// Create Streams
		SndGameCreateStreams();
	}

	if(!initFlag)
		SndGameClose();	
	
	return initFlag;	
}

// close the whole game system also release if not done.
void SndGameClose(void)
{	
	SndStreamCloseSystem();
	//HSndAnimDestroyAll();
	//SndScnDestroyAll();
	SndAnimTblDestroy();
	SndScnTblDestroy();
	SndGameDestroyMovie();
	HwSndCloseSystem();
	initFlag=FALSE;	
}

BOOL SndGameIsInitialized(void)
{
	return initFlag;
}
// I load all sound that is needed for env. Not load streaming Sound (comment)
BOOL SndGameLoad(SND_GAME_ENV env)
{
	if(!initFlag)
		return FALSE;
	SndScnEnvLoad(env);
	SndAnimEnvLoad(env);
	return TRUE;//TD 13-09-00 fix warning "not all control paths return a value"
}

// it release all loaded sounds related to env. Not streaming sound (comment)
void SndGameRelease(SND_GAME_ENV env)
{
	if(!initFlag)
		return;
	SndAnimEnvDestroy(env);
	SndScnEnvDestroy(env);
}

void SndGameReleaseAll(void)
{
	if(!initFlag)
		return;
	SndScnDestroyAll();
	HSndAnimDestroyAll();
}



// It run all function to handle sound at run time
void SndGameRunTimeHandler(void)
{
	if(!initFlag)
		return;
	SndUpdateGlobalClock();
	SndAnimRealTimeHandler();
	SndPlayRTUpdate();
	SndClsAllFadeContinue();
}

void SndGameStopAll(void)
{
	if(!initFlag)
		return;
	HSndAnimStopAll();	
	SndPlayStopAll();
	HwSndPrimaryStop();
}

//-------------------------------------------------------------------------------
static LPWINDS_SNDENV lpWinDs=NULL;

void SndGameInitMovie( PLATFHWND hwnd){		
	SNDMIXFORMAT fmt;
	fmt.nChannels=2;
	fmt.nSamplesPerSec=22050; 
	fmt.wBitsPerSample=16;
	lpWinDs=HwSndCreateWinDSObj(hwnd, &fmt);
}

LPWINDS_SNDENV SndGameGetMovie(void){
	BOOL pow;
	if(SndClsGetPower(SNDCLS_MASTER, &pow) && pow)		
		return lpWinDs;
	else 
		return NULL;
}

void SndGameDestroyMovie(void){		
	if(lpWinDs){
		HwSndDestroyWinDSObj(lpWinDs);
		lpWinDs=NULL;
	}
}