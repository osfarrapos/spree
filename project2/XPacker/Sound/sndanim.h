#ifndef __SNDANIM
#define __SNDANIM

#include "sndname.h"
#include "sndscene.h"

#define MAXLENNAMEOBJ 50
#define SNDANIM_NOOBJ -1

typedef struct SNDANIMOBJtag{
	DWORD hSnd;
	char nameGObj[MAXLENNAMEOBJ];
	DWORD vol;
	DWORD fadeVol;
	int pan;
	DWORD playFlag;		
	int nextPlay; //other obj to play at end of it -1 no obj 
} SNDANIMOBJ, *LPSNDANIMOBJ;


typedef struct SNDKEYFRAMEtag{
	DWORD time;
	LONG cmnd;
	DWORD idObj;
	int arg1; // argomento cambia a secondo del comando
} SNDKEYFRAME, *LPSNDKEYFRAME;

typedef struct SNDANIMHEADtag{
	DWORD nKey;
	DWORD nObj;
	BOOL bLoop;
	BYTE type; //0 animation must update, 1 animation is updated by extern
	DWORD hSndScn;	
} SNDANIMHEAD, *LPSNDANIMHEAD;

typedef struct SNDANIMOBJPROP{
	DWORD vol;
	DWORD fadeVol;
	int pan;
	DWORD playFlag;	
	DWORD inst;
	int nextPlay; //other obj to play at end of it -1 no obj 
	PREAL *lpPos;
} SNDANIMOBJPROP, *LPSNDANIMOBJPROP;

typedef struct SNDANIMKEYRUNtag{
	DWORD state;
	DWORD stepTime;
	DWORD lastTime;
	int stepVol;	
} SNDKEYRUN, *LPSNDKEYRUN;


typedef struct SNDANIMATIONtag{	
	SNDANIMHEAD head;
	LPSNDANIMOBJ vObj;
	LPSNDKEYFRAME vKey;
// run time variables
	LPSNDANIMOBJPROP vObjProp;
	LPSNDKEYRUN vKeyRun;	
	DWORD startTime;
	DWORD runTime;
	DWORD currKey; // is the key from which execute new commands
	DWORD state;
	//DWORD
} SNDANIM, *LPSNDANIM;



typedef struct{
	SNDLISTITEM li;
	LPSNDANIM lpsa;
} SNDANIMTBLITEM, *LPSNDANIMTBLITEM;

enum SNDANIMCMNDtag { SNDCMND_SETVOL, SNDCMND_SETPAN, SNDCMND_STOP,
						SNDCMND_PLAY, SNDCMND_SETLOOP,SNDCMND_SETFADEVOL,
						SNDCMND_ATTACH,SNDCMND_DONOTHING };
enum SATYPEtag {SATYPE_REALTIME,SATYPE_TIMEFRAME};

//------------------------------------------------------------------------------

BOOL SndAnimTblLoad(void);
void SndAnimTblDestroy(void);
DWORD SndAnimTblGetHnd(SNDNAME nameFile);
LPSNDANIM SndAnimTblGetAnim(DWORD hsa);

//------------------------------------------------------------------------------

//functions for editor
DWORD SndAnimTblGetN(void);
void SndAnimTblCopy(LPSNDLISTITEM tblCopy);
void SndAnimTblSave(DWORD nAnim, LPSNDLISTITEM tblCopy);
char *SndAnimTblGetName(void);
//------------------------------------------------------------------------------
// Functions for reading from file
FILE *SndAnimFileOpen(SNDNAME nameFile,LPSNDANIMHEAD lpHead, char mode);
void SndAnimFileClose(FILE *pf);
BOOL SndAnimWriteObj(LPSNDANIMOBJ lpObj, FILE *pf);
BOOL SndAnimReadObj(LPSNDANIMOBJ lpObj, FILE *pf);
BOOL SndAnimWriteKey(LPSNDKEYFRAME lpKey, FILE *pf);	
BOOL SndAnimReadKey(LPSNDKEYFRAME lpKey, FILE *pf);

//------------------------------------------------------------------------------
void SndAnimEnvLoad(SND_GAME_ENV env);
void SndAnimEnvDestroy(SND_GAME_ENV env);

LPSNDANIM SndAnimLoad(SNDNAME nameFile);
void SndAnimDestroy(LPSNDANIM lpsa);
BOOL SndAnimStart(LPSNDANIM lpsa);
BOOL SndAnimUpdate(LPSNDANIM lpsa, DWORD time);
BOOL SndAnimStop(LPSNDANIM lpsa);
BOOL SndAnimIsPlaying(LPSNDANIM lpsa);

// handle sound animation functions
void HSndAnimStopAll(void);
void HSndAnimDestroyAll(void);
BOOL HSndAnimLoad(DWORD hsa);
void HSndAnimDestroy(DWORD hsa);

#define	HSndAnimStop(hsa)	SndAnimStop(SndAnimTblGetAnim(hsa))
#define HSndAnimStart(hsa)	SndAnimStart(SndAnimTblGetAnim(hsa))
#define HSndAnimUpdate(hsa, time) SndAnimUpdate(SndAnimTblGetAnim(hsa),time)
#define HSndAnimIsPlaying(hsa) SndAnimIsPlaying(SndAnimTblGetAnim(hsa))

void SndAnimRealTimeHandler(void);
void SndAnimResetState(void);
void SndPlayRandom(DWORD hScn);

#endif