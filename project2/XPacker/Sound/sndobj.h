#ifndef __SNDOBJ
#define __SNDOBJ

#include "sndlow.h"

BOOL SndObjDestroy(LPSNDOBJ lpSndObj);
LPSNDOBJ SndObjCreate(SNDINFO *pInfo, LOADHANDLE lBlock, LOADPATH lPath);
void SndObjCheckRestore(LPSNDOBJ lpSndObj);
SNDHANDLE SndObjPlay(LPSNDOBJ lpSndObj,SNDPROP prop,PREAL *lpPosition, DWORD vol,int pan);
BOOL SndObjGetInfo(LPSNDOBJ lpSndObj, LPSNDMIXFORMAT lpFmt, PREAL *lpTimeLength);
SNDHANDLE SndObjPlayFreq(LPSNDOBJ lpSndObj,SNDPROP prop,PREAL *lpPosition, DWORD vol,int pan,DWORD freq);

#endif