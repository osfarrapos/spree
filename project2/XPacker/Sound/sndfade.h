#ifndef __SNDFADE
#define __SNDFADE


#include "sndlow.h"

// to generalize fading for all volumes
typedef struct SNDFADEtag {
	int fadeState; 
	DWORD initVol; //silence
	DWORD finalVol;
	DWORD stepTime;
	int stepVol;
	DWORD prevTime;
	DWORD cls;
} SNDFADE, *LPSNDFADE; 

void SndInitFadeSteps(DWORD *lpStepTime, int *lpStepVol, DWORD timeFade,
													DWORD initVol,DWORD tgtVol);
void SndFadeStart(LPSNDFADE lpsf, DWORD time, DWORD tgtVol);
BOOL SndFadeContinue(LPSNDFADE lpsf);
void SndFadeStop(LPSNDFADE lpsf, BOOL bRestore);
DWORD SndFadeComputeVol(DWORD vol, int stepVol, DWORD tgtVol);
void SndFadeInitStruct(LPSNDFADE lpsf, DWORD cls);
BOOL SndFadeIsActive(LPSNDFADE lpsf);



#endif