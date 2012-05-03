#ifndef __SNDNAME
#define __SNDNAME

#include <stdio.h>
#include "plattype.h"

// FILE format operation
#define FMTWRITE "wb" //wb
#define FMTREAD "rb"  //rb

#define SNDLISTEXT "slt"

#ifdef _SND_TEDDY_USE
	#define SNDDFLTPATH ""
#else
	#define SNDDFLTPATH "data\\sounds"
#endif

#define MAXSNDNAME 8+1
#define MAXSNDENVNAME 30+1

//to substitute  the kind of type??
typedef char SND_GAME_ENV[MAXSNDENVNAME];//, *LPSND_GAME_ENV; 
#define NOSNDENV "" //0 no env and no link

typedef char SNDNAME[MAXSNDNAME];

typedef struct SNDLISTITEMtag {
	SNDNAME name;
	SND_GAME_ENV env; //if the game is in this environment I load relative sounds
	SND_GAME_ENV envLink; // the environment requires also other sound of
						// other environment: example port requires also common
} SNDLISTITEM, *LPSNDLISTITEM;

BOOL SndNameItemWrite(FILE *pf,SNDNAME name);
BOOL SndNameItemRead(FILE *pf,SNDNAME name);
BOOL SndListItemWrite(FILE *pf,LPSNDLISTITEM lpLI);
BOOL SndListItemRead(FILE *pf,LPSNDLISTITEM lpLI);
FILE *SndListOpen(char *nameFile,DWORD *pnItem,char mode);
void SndListClose(FILE *pf);
BOOL SndEnvCmpEq(SND_GAME_ENV env1, SND_GAME_ENV env2);
BOOL SndEnvCopy(SND_GAME_ENV envtgt,SND_GAME_ENV envsrc);


#endif