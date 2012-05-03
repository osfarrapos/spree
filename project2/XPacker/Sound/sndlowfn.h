#ifndef __LOWFUNC
#define __LOWFUNC

#include "sndlow.h"
// Master

void HwSndMstRegObj(LPHWSNDOBJ lpSo);
void HwSndMstDelObj(LPHWSNDOBJ lpSo);
BOOL HwSndGetCardDetect(void);
void *HwSndMstGetObj(void);
BOOL HwSndMstSetVolume(DWORD vol);
DWORD HwSndMstGetVolume(void);
BOOL HwSndMstGetPower(void);
void HwSndMstStopAll(void);
BOOL HwSndMstSetPower(BOOL pow);


// Init System
// Init & Close System
void HwSndInitDefaultEnv(SNDENV *lpSndEnv,PLATFHWND hwnd);
BOOL HwSndInitSystem(SNDENV *psndEnv);
void HwSndCloseSystem(void);

// Listener

BOOL HwSndSetListenerObj(LPSNDLISTENER_OBJ lplo);
BOOL HwSndGetListenerObj(LPSNDLISTENER_OBJ lplo);
void HwSndLinkListener(PREAL *lpPosition, PREAL *lpOrientFront,PREAL *lpOrientTop);
BOOL HwSndUpdateListener(PREAL deltaT);

// 3D environment
BOOL HwSndInit3DEnv(void);
BOOL HwSndSetDistanceFact(PREAL d);
BOOL HwSndSetRolloffFact(PREAL d);
BOOL HwSndSetDopplerFact(PREAL d);
BOOL HwSndSet3DMode(DWORD mode);
BOOL HwSndSet3DEnable(BOOL enable);
PREAL HwSndGetDistanceFact(void);
PREAL HwSndGetRolloffFact(void);
PREAL HwSndGetDopplerFact(void);
DWORD HwSndGet3DMode(void);
BOOL HwSndGet3DEnable(void);

// runtime execute
BOOL HwSnd3DExecuteChanges(void);

// base Hw Snd obj
BOOL HwSndObjIsInHwMem(LPHWSNDOBJ lpSO);
BOOL HwSndObjDestroy(LPHWSNDOBJ lpSO);


BOOL HwSndObjCreate3DBuffer(LPHWSNDOBJ lpHwSo);
LPHWSNDOBJ HwSndObjCreate(LPVOID lpMem, DWORD *lpSize, SNDMIXFORMAT *lpFmt, DWORD sndType);
LPHWSNDOBJ HwSndObjDuplicate(LPHWSNDOBJ lpHwSo);

//Restore obj
BOOL HwSndObjIsToReload(LPHWSNDOBJ lphwso);
BOOL HwSndObjRestore(LPHWSNDOBJ lpSO,LPVOID lpMem);

//change obj props
BOOL HwSndObjPlay(LPHWSNDOBJ lpSO, SNDPROP sndProp);
BOOL HwSndObjStop(LPHWSNDOBJ lpSO);
BOOL HwSndObjPause(LPHWSNDOBJ lpSO);
BOOL HwSndObjResume(LPHWSNDOBJ lpSO);
BOOL HwSndObjIsPlaying(LPHWSNDOBJ lpSO);
BOOL HwSndObjSetVolume(LPHWSNDOBJ lpSO, DWORD volume);
BOOL HwSndObjGetVolume(LPHWSNDOBJ lpSO, DWORD *pVolume);
BOOL HwSndObjSetPan(LPHWSNDOBJ lpSO, int pan);
BOOL HwSndObjGetPan(LPHWSNDOBJ lpSO,int *pPan);
BOOL HwSndObjRefreshVolume(LPHWSNDOBJ lpSO);
BOOL HwSndObjSetFrequency(LPHWSNDOBJ lpSO, DWORD freq);
BOOL HwSndObjGetFrequency(LPHWSNDOBJ lpSO, LPDWORD lpFreq);


BOOL HwSndObjSetHnd(LPHWSNDOBJ lpHwSo,SNDHANDLE hnd);
SNDHANDLE HwSndObjGetHnd(LPHWSNDOBJ lpHwSo);
void HwSndObjSetCls(LPHWSNDOBJ lpHwSo, DWORD sndCls);
DWORD HwSndObjGetCls(LPHWSNDOBJ lpHwSo);
// 3D
BOOL HwSndObjIs3DSound(LPHWSNDOBJ lphwso);

void HwSndObjRefresh3DMode(LPHWSNDOBJ lpHwSo);
void HwSndObjInit3DProps(LPHWSNDOBJ lpHwSo);
void HwSndObjInit3DCynem(LPHWSNDOBJ lpHwSo);
BOOL HwSndObjApply3DProps(LPHWSNDOBJ lpHwSo);
BOOL HwSndObjApply3DCynem(LPHWSNDOBJ lpHwSo);
BOOL HwSndObjGet3DProps(LPHWSNDOBJ lpHwSo, LPSND3DPROPS lpProps);
BOOL HwSndObjGet3DCynem(LPHWSNDOBJ lpHwSo, LPSND3DCYNEMATIC lpCynem);
BOOL HwSndObjSet3DProps(LPHWSNDOBJ lpHwSo, LPSND3DPROPS lpProps);
BOOL HwSndObjSet3DCynem(LPHWSNDOBJ lpHwSo, LPSND3DCYNEMATIC lpNewCynem);
BOOL HwSndObjSetLinkObj(LPHWSNDOBJ lpHwSo, PREAL *lpPosition);
BOOL HwSndObjUpdate3D(LPHWSNDOBJ lpHwSo,PREAL deltaT, BOOL bForce);


BOOL HwSndEmulate3DEffects(LPHWSNDOBJ lpHwSo);

// Stream
void SndStreamInitSystem(void);
void SndStreamCloseSystem(void);
void SndStreamCreate(BOOL bStereo,BOOL b16Bits, DWORD type, DWORD nStream);
void SndStreamRTUpdate(void);
BOOL SndStreamPlay(LPSNDSTREAM lpss, void *lpMem,LOADPATH lPath, LOADHANDLE lBlock, 
														LOADHANDLE lID,SNDPROP sndProp);
LPSNDSTREAM SndStreamGetFreeBuffer(DWORD sndType, LPSNDMIXFORMAT lpfmt);

LPSNDSTREAM HwSndStreamCreate(SNDMIXFORMAT *lpFmt, DWORD sndType);
BOOL HwSndStreamDestroy(LPSNDSTREAM lpss);


LPWINDS_SNDENV HwSndCreateWinDSObj(PLATFHWND hWndMain, LPSNDMIXFORMAT lpFmt);
void HwSndDestroyWinDSObj(LPWINDS_SNDENV lpWinDsEnv);

BOOL HwSndPrimaryStop(void);

#endif