// Manipulable objects from the outside
#include "sndexter.h"
// context drive
char sndDriveContext=0;

// Snd clock
static DWORD sndTime;
void SndInitGlobalClock(void)
{
	sndTime=0;
}

DWORD SndUpdateGlobalClock(void)
{
	sndTime+=(DWORD) (PTOINT(IncTime));
	return sndTime;	
}

DWORD SndGetTime(void)
{
	return sndTime;
}
