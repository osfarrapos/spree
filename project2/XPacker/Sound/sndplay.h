#ifndef __SNDPLAY
#define __SNDPLAY

#include "sndlow.h"



void SndPlaySlotInitSystem(void);
void SndPlayFreeHnd(SNDHANDLE hnd);
void SndPlayCheckHnd(SNDHANDLE hnd);
void SndPlayUpdateClsVolume(DWORD cls);
//void SndPlayUpdateClsPan(DWORD cls, int pan);
void SndPlayRefresh(void);
void SndPlayClsStop(DWORD cls);

SNDHANDLE SndPlayAllocateHnd(LPHWSNDOBJ lpso);
SNDHANDLE SndPlayLockHnd(LPHWSNDOBJ lpso);
void SndPlayUnLockHnd(SNDHANDLE hnd);

BOOL SndPlayIsPlayingHnd(DWORD hnd);

void SndPlayStopAll(void);

void SndPlaySetVolumeHnd(DWORD hnd, DWORD vol);
void SndPlayGetVolumeHnd(DWORD hnd, DWORD *lpVol);
void SndPlaySetPanHnd(DWORD hnd, int pan);
void SndPlayGetPanHnd(DWORD hnd, int *lpPan);
void SndPlayStopHnd(DWORD hnd);
void SndPlayRTUpdate(void);

void SndPlayPauseHnd(DWORD hnd);
void SndPlayResumeHnd(DWORD hnd);


#endif








