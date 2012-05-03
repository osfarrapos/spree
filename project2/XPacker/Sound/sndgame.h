#ifndef __SNDGAME
#define __SNDGAME


#include "sndlow.h"
#include "sndobj.h"
#include "sndname.h"
#include "sndscene.h"
#include "sndanim.h"
#include "sndfade.h"
#include "sndplay.h"
#include "sndcls.h"
//#include "cdplay.h"
#include "idsuoni.h"

//#include "sndmovie.h"
BOOL SndGameIsInitialized(void);
BOOL SndGameInit(PLATFHWND hwnd);
void SndGameClose(void);
BOOL SndGameLoad(SND_GAME_ENV env);
void SndGameRelease(SND_GAME_ENV env);
void SndGameRunTimeHandler(void);
void SndGameStopAll(void);
LPWINDS_SNDENV SndGameGetMovie(void);

#endif
