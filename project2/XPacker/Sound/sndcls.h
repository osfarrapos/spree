#ifndef __SNDCLS
#define __SNDCLS

#include "plattype.h"
#include "sndlow.h"

enum CLASS_TYPE {SNDCLS_SFX,SNDCLS_COMMENT,SNDCLS_CROWD, SNDCLS_BACKGROUND,SNDCLS_MENU,
					SNDCLS_MASTER,SNDCLS_CDA, SNDCLS_NONE=0xFFFFFFFF};
#define N_SNDCLS 7
void SndClsResetSystem(void);
BOOL SndClsSetVolume(DWORD cls, DWORD vol);
BOOL SndClsSetPan(DWORD cls, int pan);
BOOL SndClsGetVolume(DWORD cls, DWORD *pVol);
BOOL SndClsGetPan(DWORD cls, int *pPan);
BOOL SndClsStopAll(DWORD cls);
BOOL SndClsSetPower(DWORD cls, BOOL pow);
BOOL SndClsGetPower(DWORD cls, BOOL *pPow);
DWORD SndClsConvertVolume(DWORD cls, DWORD vol);
void SndClsFadeStart(DWORD cls, DWORD time, DWORD tgtVol);
BOOL SndClsFadeContinue(DWORD cls);
void SndClsFadeStop(DWORD cls, BOOL bRestore);
BOOL SndClsFadeIsActive(DWORD cls);
void SndClsAllFadeContinue(void);


void SndClsMasterFadeIn(DWORD time);
void SndClsMasterFadeOut(DWORD time);
BOOL SndClsMasterQuery(void);
void SndClsMasterFadeOutStop(void);
void SndClsMasterInitFadeVars(void);
BOOL SndClsMasterFadeIsActive(void);
BOOL SndClsSetMasterVolumeForFade(int vol);



#endif