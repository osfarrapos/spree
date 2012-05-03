#include "sndgame.h"
#include "sndfade.h"

// 0 no fade, 1 Fade playing, 2 fade to end, 3 fade end but not close
static enum SNDFADESTATEtag { NOFADE, FADEACTIVE, FADETOEND, FADEEND };

// define relative to  handle sound fade
#define MINTIMEVOL 100 // minimo intervallo di tempo tra due cambi di vol (in milliseconds)
#define NSTEPSVOL  SNDMAXVOLUME/2 // massimo numero di passi per il fading

void SndInitFadeSteps(DWORD *lpStepTime, int *lpStepVol, DWORD timeFade,
													DWORD initVol,DWORD tgtVol){
	DWORD nSteps=NSTEPSVOL;
	DWORD stepTime;
	int stepVol;
	
	if(tgtVol==initVol){
		*lpStepTime=0;
		*lpStepVol=0;
		return; 
	}
	if(timeFade==0){
		*lpStepTime=0;
		*lpStepVol=(int)tgtVol-(int)initVol;
		return; 
	}

	stepTime=timeFade/nSteps;
	if(stepTime<MINTIMEVOL){
		stepTime=MINTIMEVOL;
		nSteps=timeFade/stepTime;
	}
	if(nSteps>0)
		stepVol=((int)tgtVol-(int)initVol)/(int)nSteps;
	else
		stepVol=((int)tgtVol-(int)initVol);
	if(stepVol==0) 
		stepVol=(tgtVol>initVol) ? 1: -1;	
	//return values
	*lpStepTime=stepTime;
	*lpStepVol=stepVol;
}

DWORD SndFadeComputeVol(DWORD vol, int stepVol, DWORD tgtVol){
	if(stepVol>0){
		if(((DWORD)stepVol)< (tgtVol-vol)){
			vol+=(stepVol);
			//if(vol>tgtVol)
			//	vol=tgtVol;		
		}else
			vol=tgtVol;

	}else if(stepVol<0){
		if(((DWORD)(-stepVol))< (vol-tgtVol)){
			vol+=(stepVol);
			//if(vol<tgtVol)
			//	vol=tgtVol;		
		}else
			vol=tgtVol;
	}
	return vol;
}


void SndFadeInitStruct(LPSNDFADE lpsf, DWORD cls){
	lpsf->fadeState=NOFADE; 
	lpsf->initVol=0; //silence
	lpsf->finalVol=0;
	lpsf->stepTime=0;
	lpsf->stepVol=0;
	lpsf->prevTime=0;
	lpsf->cls=cls;
}

void SndFadeStart(LPSNDFADE lpsf,DWORD time, DWORD tgtVol){
	BOOL pow;
	lpsf->fadeState=NOFADE;
	if(!SndGameIsInitialized() )
		return;
	if(!SndClsGetPower(lpsf->cls, &pow) || !pow)
		return;
	//if(lpsf->fadeState==FADEACTIVE) 
	//	;// we change fading step also if it's already active
		//obviously we lost initVol and so it's not possible to restore volume 
	SndClsGetVolume(lpsf->cls, &lpsf->initVol); //HwSndMstGetVolume();
	
	if(lpsf->initVol==tgtVol) return; //no fade needed
	SndInitFadeSteps(&lpsf->stepTime, &lpsf->stepVol, time, lpsf->initVol,tgtVol);
	lpsf->finalVol=tgtVol;
	lpsf->fadeState=FADEACTIVE;
	lpsf->prevTime=SndGetTime();
}


BOOL SndFadeContinue(LPSNDFADE lpsf){	
	if(lpsf->fadeState==FADEACTIVE){
		DWORD time=SndGetTime();
		DWORD vol;
		if(time-lpsf->prevTime>lpsf->stepTime){
			lpsf->prevTime=time;
			SndClsGetVolume(lpsf->cls, &vol);//vol= HwSndMstGetVolume();												
			if(vol==lpsf->finalVol) {				
				lpsf->fadeState=FADEEND;
			}else{
				vol=SndFadeComputeVol(vol, lpsf->stepVol, lpsf->finalVol);
				SndClsSetVolume(lpsf->cls, vol); 	//HwSndMstSetVolume(vol);			
			}
		}		
	}
	return lpsf->fadeState==FADEACTIVE;
}
 
BOOL SndFadeIsActive(LPSNDFADE lpsf){
	return lpsf->fadeState==FADEACTIVE;
}

//Restore volume ?
void SndFadeStop(LPSNDFADE lpsf, BOOL bRestore){
	if(lpsf->fadeState==NOFADE) //fading never start
		return; 
	//if(fadeState==FADEACTIVE) ;//???
	if(bRestore)
		SndClsSetVolume(lpsf->cls, lpsf->initVol);//HwSndMstSetVolume();
	lpsf->fadeState=NOFADE;
}


