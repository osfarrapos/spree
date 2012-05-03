#ifndef __SNDLOAD
#define __SNDLOAD

#include <stdio.h>
#include "sndobj.h"
#include "sndname.h"


//-------------------------------------------------------------------- 
// Sound Scene Table File Functions

//FILE *SndScnTblInitWrite(char *nameFile,DWORD *pnScenes);
//FILE *SndScnTblInitRead(char *nameFile,DWORD *pnScenes);
//void SndScnTblClose(FILE *pf);
BOOL SndScnTblLoad(void);
void SndScnTblDestroy(void);
DWORD SndScnTblGetN(void);
void SndScnTblCopy(LPSNDLISTITEM tblCopy);
void SndScnTblSave(DWORD nScn, LPSNDLISTITEM tblCopy);
char *SndScnTblGetName(void);
//-------------------------------------------------------------------- 
// Sound Scene  Functions

DWORD SndScnGetHandle(SNDNAME nameFile);
char *SndScnGetName(DWORD hndl);
DWORD SndScnGetNSnd(DWORD hndl);

BOOL SndInfoWrite(SNDINFO *pSndInfo, FILE *pf);
BOOL SndInfoRead(SNDINFO *pSndInfo, FILE *pf);
FILE *SndScnInitLoad(SNDNAME name, DWORD *pnSnd,LOADHANDLE lpBlock, LOADPATH lpPath);
FILE *SndScnInitSave(SNDNAME name, DWORD *pnSnd,LOADHANDLE lpBlock, LOADPATH lpPath);
void SndScnClose(FILE *pf);

void SndScnEnvLoad(SND_GAME_ENV env);
void SndScnEnvDestroy(SND_GAME_ENV env);
BOOL HSndScnLoad(DWORD hFile);
void HSndScnDestroy(DWORD hFile);
void SndScnDestroyAll(void);

//--------------------------------------------------------------------------------
// functions to handle sounds by the couple scene handle, sound handle

LPSNDOBJ HSndScnGetObj(DWORD hFile,DWORD hSnd);

// define functions to handle handles of sound

#define HSndObjIsPlaying(hFile, hSnd, hnd) \
	SndObjIsPlaying(HSndScnGetObj(hFile,hSnd),hnd)

#define  HSndObjPlay(hFile, hSnd,prop,lpPos, volume, pan) \
	SndObjPlay(HSndScnGetObj(hFile,hSnd), prop, lpPos, volume, pan)

#define  HSndObjPlay2D(hFile, hSnd,prop) \
	SndObjPlay(HSndScnGetObj(hFile,hSnd), prop, NULL, SNDMAXVOLUME, 0)

#define  HSndObjPlay3D(hFile, hSnd,prop,lpPos) \
	SndObjPlay(HSndScnGetObj(hFile,hSnd), prop, lpPos, SNDMAXVOLUME, 0)

#define  HSndObjGetInfo(hFile, hSnd,lpFmt,lpLength) \
	SndObjGetInfo(HSndScnGetObj(hFile,hSnd), lpFmt, lpLength)

#define  HSndObjPlayFreq(hFile, hSnd,prop,lpPos, volume, pan,freq) \
	SndObjPlayFreq(HSndScnGetObj(hFile,hSnd), prop, lpPos, volume, pan,freq)
/*
#define HSndObjGetVolume(hFile,hSnd, inst, pVol) \
	SndObjGetVolume(HSndScnGetObj(hFile,hSnd), inst, pVol)

#define HSndObjSetVolume(hFile,hSnd, inst, vol) \
	SndObjSetVolume(HSndScnGetObj(hFile,hSnd), inst, vol)

#define HSndObjSetPan(hFile,hSnd,  inst,  pan) \
	SndObjSetPan(HSndScnGetObj(hFile,hSnd),  inst,  pan)

#define  HSndObjGetPan(hFile,hSnd, inst, pPan) \
	SndObjGetPan(HSndScnGetObj(hFile,hSnd), inst, pPan)
*/
/*
#define HSndObjResume(hFile,hSnd,instance) \
	SndObjResume(HSndScnGetObj(hFile,hSnd), instance)
*/

#endif