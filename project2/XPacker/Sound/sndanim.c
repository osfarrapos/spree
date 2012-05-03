/* ------------------------------------------------------------------
	 Sound Animation

	Pixelstorm Copyright - by Teodoro Di Bello
 
	Version: 0.5		Date: 19/01/98

	Description: 
	Functions to handle sound animations
		   
	Note:	
  ------------------------------------------------------------------ */
#include "plattype.h"
#include "sndscene.h"
#include "funcalloc.h"
#include "myerror.h"
#include "myutil.h"
#include "sndfade.h"
#include "sndsuppl.h"
#include "sndplay.h"
#include "sndanim.h"

//#define MAXLENNAMEOBJ 20
#define SNDANIMPATH  SNDDFLTPATH
#define SNDANIMTBLNAME "sndantbl"
#define SNDANIMEXT "sal"
#define SNDANIMHEADTEXT "sndanim1"

// for debbug
char *stateText[]={
	"SA_READY", "SA_PLAYING", "SA_END","SA_STOP"
};

//---------------------------------------------------------------------------
enum SASTATEtag {SA_READY, SA_PLAYING, SA_END,SA_STOP};

#define INDEX2HANDLE(ind) ((ind)+1)
#define HANDLE2INDEX(hnd) ((hnd)-1)
// I Know That SNDNOHANDLE=0
#define ISVALIDHANDLE(hnd) ((hnd)>SNDNOHANDLE && (hnd)<=nSndAnim) 

// Sound Animations Table 
static LPSNDANIMTBLITEM sndAnimTable=NULL;
// Total Sound Animations  number, not loaded animations
static DWORD nSndAnim=0;

//Local Functions
void SndAnimSetInitValue(LPSNDANIM lpsa);
BOOL SndAnimUpdateVolume(LPSNDANIM lpsa, DWORD iKey);
void SndAnimExecKey(LPSNDANIM lpsa, DWORD iKey);
void SndAnimUpdateObj(LPSNDANIM lpsa);

#define SNDANIMNONAME ""

void SndAnimGetPosForSound(char *name, PREAL **lpPos)
{
	if(strcmp(name,SNDANIMNONAME)!=0)
		GetPosForSound(name, lpPos);
	else
		*lpPos=NULL;
}

char *SndAnimTblGetName(void)
{
	static char nameAnimTbl[MAX_PATH];
	BuildFullPath(nameAnimTbl,NULL, SNDANIMTBLNAME, 0,SNDLISTEXT);
	return nameAnimTbl;
}

BOOL SndAnimTblLoad(void)
{
	FILE *pf;
	char nameFull[MAX_PATH];
	char *nameTbl=SNDANIMTBLNAME;
	DWORD i;
	if(sndAnimTable!=NULL) //already loaded
		return TRUE;
	BuildFullPath(nameFull,SNDANIMPATH, nameTbl, sndDriveContext,SNDLISTEXT);
	pf=SndListOpen(nameFull,&nSndAnim,'r');	
	ASSERTFILE(pf!=NULL, nameFull);
	sndAnimTable=MyMalloc(SNDANIMTBLITEM,nSndAnim); 
	ASSERTOUTMEMORY(sndAnimTable!=NULL);
	for(i=0; i<nSndAnim; i++){
		SndListItemRead(pf,&sndAnimTable[i].li);
		sndAnimTable[i].lpsa=NULL;
	}
	SndListClose(pf);
	return TRUE;
}

void HSndAnimDestroy(DWORD hsa) 
{
	if(nSndAnim==0)
		return;
	SndAnimDestroy(SndAnimTblGetAnim(hsa)); 
	sndAnimTable[HANDLE2INDEX(hsa)].lpsa=NULL; // I directly access to vector to nulled it
}

void HSndAnimDestroyAll(void)
{
	DWORD i;
	for(i=0; i<nSndAnim; i++)
		HSndAnimDestroy(INDEX2HANDLE(i));
}

void HSndAnimStopAll(void)
{
	DWORD i;
	for(i=0; i<nSndAnim; i++)
		HSndAnimStop(INDEX2HANDLE(i));

}

void SndAnimTblDestroy(void)
{
	HSndAnimDestroyAll();
	if(sndAnimTable!=NULL)
		MyFree(sndAnimTable);
	sndAnimTable=NULL;
	nSndAnim=0;	
}

DWORD SndAnimTblGetHnd(SNDNAME nameFile)
{
	DWORD i;
	for(i=0; i<nSndAnim; i++)
		if(!strcmp(nameFile,sndAnimTable[i].li.name))
			break;
	if(i==nSndAnim)
		return SNDNOHANDLE;
	return INDEX2HANDLE(i);
}	


LPSNDANIM SndAnimTblGetAnim(DWORD hsa)
{
	if(nSndAnim==0 || !ISVALIDHANDLE(hsa))
		return NULL;	
	return sndAnimTable[HANDLE2INDEX(hsa)].lpsa;
}

BOOL HSndAnimLoad(DWORD hsa)
{
	DWORD i;
	if(nSndAnim==0 || !ISVALIDHANDLE(hsa))
		return FALSE;
	i=HANDLE2INDEX(hsa);
	if(sndAnimTable[i].lpsa!=NULL) //already loaded
		return TRUE;
	sndAnimTable[i].lpsa=SndAnimLoad(sndAnimTable[i].li.name);
	return sndAnimTable[i].lpsa!=NULL;
}

//---------------------------------------------------------------------
// special for editor
DWORD SndAnimTblGetN(void)
{
	return nSndAnim;
}

void SndAnimTblCopy(LPSNDLISTITEM tblCopy)
{
	DWORD i;
	for(i=0; i<nSndAnim; i++)
		COPYMEMORY(tblCopy+i, &sndAnimTable[i].li, sizeof(SNDLISTITEM));
}

void SndAnimTblSave(DWORD nAnim, LPSNDLISTITEM tblCopy)
{
	FILE *pf;
	DWORD nSndAnim=nAnim;
	char nameFull[MAX_PATH];
	char *nameTbl=SNDANIMTBLNAME;
	DWORD i;
	BuildFullPath(nameFull,SNDANIMPATH, nameTbl, sndDriveContext,SNDLISTEXT);
	pf=SndListOpen(nameFull,&nSndAnim,'w');
	ASSERTFILE(pf!=NULL,nameFull);
	for(i=0; i<nSndAnim; i++)
		SndListItemWrite(pf,&tblCopy[i]);
	SndListClose(pf);	
}

FILE *SndAnimFileOpen(SNDNAME nameFile,LPSNDANIMHEAD lpHead, char mode)
{
	FILE *pf=NULL;
	size_t nf1,nf2;
	char *opMode;
	char headText[sizeof(SNDANIMHEADTEXT)+1];
	char nameFull[MAX_PATH];

	if(mode=='w'){
		opMode=FMTWRITE;		
	}
	else if(mode=='r')
		opMode=FMTREAD;
	else
		return NULL;				
	BuildFullPath(nameFull,SNDANIMPATH, nameFile, sndDriveContext,SNDANIMEXT);
	pf=fopen(nameFull,opMode);	
	ASSERTFILE(pf!=NULL,nameFull);
	if(mode=='w'){
		nf1=fwrite(SNDANIMHEADTEXT, sizeof(char), strlen(SNDANIMHEADTEXT)+1, pf);
		nf2=fwrite(lpHead, sizeof(SNDANIMHEAD), 1, pf);
	}
	else{
		nf1=fread(headText, sizeof(char), strlen(SNDANIMHEADTEXT)+1, pf);
		if(!strcmp(headText,SNDANIMHEADTEXT))
			nf2=fread(lpHead, sizeof(SNDANIMHEAD), 1, pf);
		else{
			fclose(pf);
			pf=NULL;
		}
	}
	return pf;	
}

void SndAnimFileClose(FILE *pf)
{
	fclose(pf);
}

BOOL SndAnimWriteObj(LPSNDANIMOBJ lpObj, FILE *pf)
{
	return (fwrite(lpObj, sizeof(SNDANIMOBJ), 1, pf)>0);
}

BOOL SndAnimReadObj(LPSNDANIMOBJ lpObj, FILE *pf)
{
	return (fread(lpObj, sizeof(SNDANIMOBJ), 1, pf)>0);
}

BOOL SndAnimWriteKey(LPSNDKEYFRAME lpKey, FILE *pf)
{
	return (fwrite(lpKey, sizeof(SNDKEYFRAME), 1, pf)>0);
}

BOOL SndAnimReadKey(LPSNDKEYFRAME lpKey, FILE *pf)
{
	return (fread(lpKey, sizeof(SNDKEYFRAME), 1, pf)>0);
}

void SndAnimEnvLoadGeneral(SND_GAME_ENV env, BOOL linkFlag)
{
	DWORD i;
	if(SndEnvCmpEq(env,NOSNDENV))
		return;
	
	for(i=0; i<nSndAnim; i++) {
		if(SndEnvCmpEq(sndAnimTable[i].li.env,env)){	
			HSndAnimLoad(INDEX2HANDLE(i));
			if(linkFlag && !SndEnvCmpEq(sndAnimTable[i].li.envLink,NOSNDENV) )
				SndAnimEnvLoadGeneral(sndAnimTable[i].li.envLink,FALSE);
		}
	}
}

void SndAnimEnvLoad(SND_GAME_ENV env)
{
	SndAnimEnvLoadGeneral(env,TRUE);
}

void SndAnimEnvDestroyGeneral(SND_GAME_ENV env, BOOL linkFlag)
{
	DWORD i;
	if(SndEnvCmpEq(env,NOSNDENV))
		return;	
	for(i=0; i<nSndAnim; i++) {
		if(SndEnvCmpEq(sndAnimTable[i].li.env,env)){	
			HSndAnimDestroy(INDEX2HANDLE(i));
			if(linkFlag && !SndEnvCmpEq(sndAnimTable[i].li.envLink,NOSNDENV))
				SndAnimEnvDestroyGeneral(sndAnimTable[i].li.envLink,FALSE);
		}
	}
}

void SndAnimEnvDestroy(SND_GAME_ENV env)
{
	SndAnimEnvDestroyGeneral(env,TRUE);
}

//------------------------------------------------------------------------------
// SNDANIM object functions

LPSNDANIM SndAnimLoad(SNDNAME nameFile)
{ //handle? or name?
	FILE *pf;
	LPSNDANIM lpsa;
	DWORD i;
	BOOL bf;
	lpsa=MyMalloc(SNDANIM,1);
	ASSERTOUTMEMORY(lpsa!=NULL);
	pf=SndAnimFileOpen(nameFile, &lpsa->head, 'r');
	ASSERTFILE(pf!=NULL,nameFile);
	
	lpsa->vObj=MyMalloc(SNDANIMOBJ,lpsa->head.nObj);
	ASSERTOUTMEMORY(lpsa->vObj!=NULL);

	lpsa->vKey=MyMalloc(SNDKEYFRAME,lpsa->head.nKey);
	ASSERTOUTMEMORY(lpsa->vKey!=NULL);
	
	lpsa->vObjProp=MyMalloc(SNDANIMOBJPROP,lpsa->head.nObj);
	ASSERTOUTMEMORY(lpsa->vObjProp!=NULL);
	
	lpsa->vKeyRun=MyMalloc(SNDKEYRUN,lpsa->head.nKey);
	ASSERTOUTMEMORY(lpsa->vKeyRun!=NULL);

	for(i=0; i<lpsa->head.nObj; i++){
		SndAnimReadObj(lpsa->vObj+i, pf);
		// I read  now the position vector for object and never change it
		SndAnimGetPosForSound(lpsa->vObj[i].nameGObj,&lpsa->vObjProp[i].lpPos);
	}
	for(i=0; i<lpsa->head.nKey; i++){
		SndAnimReadKey(lpsa->vKey+i, pf);		
	}
	// RUNTIME init
	lpsa->state=SA_READY;	
	//LOAD SCENE!
	bf=HSndScnLoad(lpsa->head.hSndScn);
	ASSERTFILE(bf==TRUE,SndScnGetName(lpsa->head.hSndScn));
	return lpsa;
}

void SndAnimSetInitValue(LPSNDANIM lpsa)
{
	DWORD i;
	ASSERTPOINTER(lpsa);	
	lpsa->runTime=SndGetTime();
	if(lpsa->head.type==SATYPE_REALTIME)
		lpsa->startTime=lpsa->runTime;
	else
		lpsa->startTime=0;

	for(i=0; i<lpsa->head.nObj; i++){		
		lpsa->vObjProp[i].inst=SNDNOHANDLE; //no instance yet
			lpsa->vObjProp[i].vol=lpsa->vObj[i].vol; //SNDMAXVOLUME
		lpsa->vObjProp[i].fadeVol=lpsa->vObj[i].fadeVol;//0
		lpsa->vObjProp[i].pan=lpsa->vObj[i].pan;//0
		lpsa->vObjProp[i].playFlag=lpsa->vObj[i].playFlag;// SNDPROP_NO
		lpsa->vObjProp[i].nextPlay=lpsa->vObj[i].nextPlay;//SNDANIM_NOOBJ
	}
	for(i=0; i<lpsa->head.nKey; i++){	
		lpsa->vKeyRun[i].state=SA_READY;
		lpsa->vKeyRun[i].stepTime=0;
		lpsa->vKeyRun[i].lastTime=0;
		lpsa->vKeyRun[i].stepVol=0;	
	}
}

void SndAnimDestroy(LPSNDANIM lpsa)
{	
	if(lpsa!=NULL){
		SndAnimStop(lpsa);
		if(lpsa->vKeyRun)
			MyFree(lpsa->vKeyRun);
		if(lpsa->vObjProp)
			MyFree(lpsa->vObjProp);
		if(lpsa->vKey)
			MyFree(lpsa->vKey);
		if(lpsa->vObj)
			MyFree(lpsa->vObj);		
		MyFree(lpsa);
	}
}

BOOL SndAnimStop(LPSNDANIM lpsa)
{
	DWORD i;
	if(lpsa==NULL)
		return FALSE;
	if(lpsa->state==SA_PLAYING){
		lpsa->state=SA_STOP;
		//Stop All sounds in animation
		for(i=0; i<lpsa->head.nObj; i++){
			if(lpsa->vObjProp[i].inst!=SNDNOHANDLE &&
				SndPlayIsPlayingHnd(lpsa->vObjProp[i].inst))	
				SndPlayStopHnd(lpsa->vObjProp[i].inst);
		}
	}
	return TRUE;
}

BOOL SndAnimStart(LPSNDANIM lpsa)
{
	if(lpsa==NULL)
		return FALSE;	
	if(lpsa->state!=SA_PLAYING){
		lpsa->currKey=0;
		lpsa->state=SA_PLAYING;		
		SndAnimSetInitValue(lpsa);		
	}	
	return TRUE;
}

BOOL SndAnimIsPlaying(LPSNDANIM lpsa)
{
	DWORD i;
	BOOL bFlag;

	if(lpsa==NULL)
		return FALSE;	
	bFlag=(lpsa->state==SA_PLAYING);
	for(i=0;i<lpsa->head.nObj && !bFlag; i++)
		if(lpsa->vObjProp[i].inst!=SNDNOHANDLE &&
			SndPlayIsPlayingHnd(lpsa->vObjProp[i].inst))
			bFlag=TRUE;	
	return bFlag;	
}

BOOL SndAnimUpdateVolume(LPSNDANIM lpsa, DWORD iKey)
{ //DWORD
	LPSNDKEYFRAME lpK;
	LPSNDKEYRUN lpKR;
	LPSNDANIMOBJPROP lpOP;
	LPSNDANIMOBJ lpO;
	DWORD hSndScn;
	DWORD newTime;
	
	if(lpsa==NULL)
		return FALSE;
	ASSERTRANGE(iKey<lpsa->head.nKey);
	//Abbreviation variable I can use also a define
	lpK=lpsa->vKey+iKey;
	lpKR=lpsa->vKeyRun+iKey;
	lpO=lpsa->vObj+lpK->idObj;
	lpOP=lpsa->vObjProp+lpK->idObj;
	hSndScn=lpsa->head.hSndScn;
	
	newTime=lpsa->runTime;
	if(newTime-lpKR->lastTime<lpKR->stepTime)
		return TRUE;
	// ho messo in una funzione il calcolo del volume 
	lpOP->vol=SndFadeComputeVol(lpOP->vol,lpKR->stepVol, lpOP->fadeVol);
	
	if(lpOP->inst!=SNDNOHANDLE && SndPlayIsPlayingHnd(lpOP->inst) )
		SndPlaySetVolumeHnd(lpOP->inst,lpOP->vol);	
	else
		lpOP->vol=lpOP->fadeVol;
	lpKR->lastTime=newTime;
	return lpOP->vol!=lpOP->fadeVol;		
}

void SndAnimExecKey(LPSNDANIM lpsa, DWORD iKey)
{
	LPSNDKEYFRAME lpK;
	LPSNDKEYRUN lpKR;
	LPSNDANIMOBJPROP lpOP;
	LPSNDANIMOBJ lpO;
	DWORD hSndScn;
	
	ASSERTPOINTER(lpsa);
	ASSERTRANGE(iKey<lpsa->head.nKey);
	//Abbreviation variable I can use also a define
	lpK=lpsa->vKey+iKey;
	lpKR=lpsa->vKeyRun+iKey;
	lpO=lpsa->vObj+lpK->idObj;
	lpOP=lpsa->vObjProp+lpK->idObj;
	hSndScn=lpsa->head.hSndScn;
		
	if(lpKR->state==SA_END) // already executed and terminated
		return;	
	switch(lpK->cmnd){		 				
	case SNDCMND_SETVOL:
		ASSERTRANGE(lpK->arg1>=0);		
		lpOP->vol=(DWORD)lpK->arg1;
		if(lpOP->inst!=SNDNOHANDLE && SndPlayIsPlayingHnd(lpOP->inst) )
			SndPlaySetVolumeHnd(lpOP->inst,lpOP->vol);			
		lpKR->state=SA_END;
		break;
	case SNDCMND_SETPAN:
		//ASSERTRANGE(lpK->arg1>=-MAXPAN && lpK->arg1<=MAXPAN);
		lpOP->pan=(DWORD)lpK->arg1;
		if(lpOP->inst!=SNDNOHANDLE && SndPlayIsPlayingHnd(lpOP->inst) )
			SndPlaySetPanHnd(lpOP->inst,lpOP->pan);			
		lpKR->state=SA_END;
		break;
	case SNDCMND_STOP:
		ASSERTRANGE(lpK->arg1>=0);
		if(lpK->arg1==0){
			if(lpOP->inst!=SNDNOHANDLE && SndPlayIsPlayingHnd(lpOP->inst) ){
				SndPlayStopHnd(lpOP->inst);
				lpOP->inst=SNDNOHANDLE;
			}
			lpKR->state=SA_END;
		}
		else { // gestire fading
			if(lpKR->state==SA_READY){
				SndInitFadeSteps(&(lpKR->stepTime), &(lpKR->stepVol), 
					lpK->arg1,lpOP->vol,lpOP->fadeVol);				 
				if(SndAnimUpdateVolume(lpsa, iKey))
					lpKR->state=SA_PLAYING;
				else{
					lpKR->state=SA_END;					 
				}
			}
			else if (lpKR->state==SA_PLAYING){				
				if(!SndAnimUpdateVolume(lpsa, iKey))
					lpKR->state=SA_END;
			}
			if (lpKR->state==SA_END){
				if(lpOP->fadeVol==0){
					SndPlayStopHnd(lpOP->inst);
					lpOP->inst=SNDNOHANDLE;
				}
			}
		}
		break;
	case SNDCMND_PLAY:
		ASSERTRANGE(lpK->arg1>=0);
		
		if(lpK->arg1>0){	// gestire fading vai
			if(lpKR->state==SA_READY) {				
				lpOP->inst=HSndObjPlay(hSndScn,lpO->hSnd,lpOP->playFlag,lpOP->lpPos,
					lpOP->vol, lpOP->pan);
				if(lpOP->inst!=SNDNOHANDLE) {
					SndInitFadeSteps(&(lpKR->stepTime), &(lpKR->stepVol), 
												lpK->arg1,lpOP->vol,lpOP->fadeVol);	
					if(SndAnimUpdateVolume(lpsa, iKey))
						lpKR->state=SA_PLAYING;
					else
						lpKR->state=SA_END;
				}
				else
					lpKR->state=SA_END;
			}
			else if (lpKR->state==SA_PLAYING){				
				if(!SndAnimUpdateVolume(lpsa, iKey))
					lpKR->state=SA_END;
			}
		}
		else{
			lpOP->inst=HSndObjPlay(hSndScn,lpO->hSnd,lpOP->playFlag,lpOP->lpPos,
				lpOP->vol, lpOP->pan);
			lpKR->state=SA_END;
		}
		break;
	case SNDCMND_SETLOOP:
		ASSERTRANGE(lpK->arg1>=0);
		if(lpK->arg1==0) 
			lpOP->playFlag=SNDPROP_NO;
		else
			lpOP->playFlag|=SNDPROP_LOOP;
		lpKR->state=SA_END;
		break;
	case SNDCMND_SETFADEVOL:
		ASSERTRANGE(lpK->arg1>=0);
		lpOP->fadeVol=lpK->arg1;
		lpKR->state=SA_END;
		break;
	case SNDCMND_ATTACH:		
		if(lpKR->state==SA_READY){
			if(lpK->arg1!=SNDANIM_NOOBJ)
				ASSERTRANGE(lpK->arg1>=0 && ((DWORD) lpK->arg1)<lpsa->head.nObj);
			lpOP->nextPlay=lpK->arg1;
			lpKR->state=SA_PLAYING;
		}
		else if(lpKR->state==SA_PLAYING && !SndPlayIsPlayingHnd(lpOP->inst)){
			LPSNDANIMOBJPROP lpOPNx;
			LPSNDANIMOBJ lpONx;
			lpONx=lpsa->vObj+lpOP->nextPlay;
			lpOPNx=lpsa->vObjProp+lpOP->nextPlay;
			lpOPNx->inst=HSndObjPlay(hSndScn,lpONx->hSnd,lpOPNx->playFlag,lpOPNx->lpPos,
				lpOPNx->vol, lpOPNx->pan);
			lpKR->state=SA_END;
		}
		break;
	case SNDCMND_DONOTHING:
		lpKR->state=SA_END;
		break;
	default:
		FATALASSERT(FALSE);
		break;
	}	
}


void SndAnimUpdateObj(LPSNDANIM lpsa)
{
	DWORD i;
	ASSERTPOINTER(lpsa);	
	for(i=0; i<lpsa->head.nObj; i++){
		if(lpsa->vObjProp[i].inst!=SNDNOHANDLE && !SndPlayIsPlayingHnd(lpsa->vObjProp[i].inst))
			lpsa->vObjProp[i].inst=SNDNOHANDLE;
	}
}


BOOL SndAnimUpdate(LPSNDANIM lpsa, DWORD time)
{
	DWORD iKey;
	DWORD i;
	if(lpsa==NULL)
		return FALSE; 
	if(lpsa->state!=SA_PLAYING)
		return FALSE;
	lpsa->runTime=SndGetTime();
	SndAnimUpdateObj(lpsa);
	//find the current max key to execute
	for(iKey=0; iKey<lpsa->head.nKey; iKey++){		
		if(lpsa->vKey[iKey].time > time)
			break;			
	}
	
	// check if the time go down (local loop)
	if(iKey<=lpsa->currKey) {
		for(i=0; i<lpsa->currKey; i++){ // reset state of key frame
			if(lpsa->vKey[i].time<time)
				continue;
			if(lpsa->vKeyRun[i].state!=SA_PLAYING)
				lpsa->vKeyRun[i].state=SA_READY;
		}
	}
	lpsa->currKey=iKey;
	if(lpsa->currKey==lpsa->head.nKey) // we have executed all commands
		lpsa->state=SA_END;
	for(i=0; i<lpsa->currKey; i++){ // but can be some commands playing
		SndAnimExecKey(lpsa, i);
		if(lpsa->state==SA_END && lpsa->vKeyRun[i].state!=SA_END)
			lpsa->state=SA_PLAYING;
	}

	return TRUE;
}

void SndAnimResetState(void)
{
	DWORD i;
	LPSNDANIM lpsa;
	for(i=0;i<nSndAnim; i++){
		lpsa=sndAnimTable[i].lpsa;
		if(lpsa!=NULL && lpsa->state!=SA_PLAYING)
			lpsa->state=SA_READY;
	}
}

void SndAnimRealTimeHandler(void)
{
	DWORD i;
	LPSNDANIM lpsa;		
	for(i=0;i<nSndAnim; i++){
		lpsa=sndAnimTable[i].lpsa;
		if(lpsa!=NULL && lpsa->head.type==SATYPE_REALTIME){
			if(lpsa->head.bLoop &&  lpsa->state!=SA_PLAYING && lpsa->state!=SA_STOP){
				SndAnimStart(lpsa);	
				SndAnimUpdate(lpsa,0);
			}
			else if(lpsa->state==SA_PLAYING){
				DWORD newTime; //to avoid that update is called more times with the same time
				newTime=SndGetTime();
				if(lpsa->runTime!=newTime)
					SndAnimUpdate(lpsa,newTime-lpsa->startTime);
			}
		}
	}
}

void SndPlayRandom(DWORD hScn)
{
	DWORD nSnd;
	DWORD nRandom;
	DWORD hSnd;	
	static DWORD currHnd=SNDNOHANDLE;
	
	if (hScn==SNDNOHANDLE)
		return;
	nSnd=SndScnGetNSnd(hScn);
	if (currHnd!=SNDNOHANDLE)
		if(SndPlayIsPlayingHnd(currHnd))
			return;
	if (nSnd>0){
		nRandom=600*nSnd;
		hSnd=PRAND(nRandom);
		if(hSnd!=0 && hSnd<=nSnd)
			currHnd=HSndObjPlay(hScn, hSnd,0,NULL, 75, 0);
	}
}

void SndPlayRandom2(DWORD hScn)
{
	DWORD nSnd;
	DWORD nRandom;
	DWORD hSnd;
	
	if(hScn==SNDNOHANDLE)
		return;
	nSnd=SndScnGetNSnd(hScn);
	if (nSnd>0){
		nRandom=100*nSnd;
		hSnd=PRAND(nRandom);
		if(hSnd!=0 && hSnd<=nSnd)
			HSndObjPlay(hScn, hSnd,0,NULL, SNDMAXVOLUME, 0);
	}
}
