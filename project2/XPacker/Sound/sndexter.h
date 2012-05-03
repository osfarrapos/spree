#ifndef __SNDEXTERN
#define __SNDEXTERN

#include "plattype.h"

#include "gint.h"	

extern char sndDriveContext;
DWORD SndGetTime(void);
void SndInitGlobalClock(void);	
DWORD SndUpdateGlobalClock(void);

#endif