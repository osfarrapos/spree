#ifndef _SND_STRUCTURE_
#define _SND_STRUCTURE_

#include <mmsystem.h>
#include "dsound.h"

#define SNDSCNEXT "ssl"
#define SNDSCNHEADER "SNDSCN1"
#define SNDLISTHEADER "SNDLIST1"
#define SNDSCNPATH SNDDFLTPATH
#define SNDSCNTBLNAME "sndscntb"

// FILE format operation
#define FMTWRITE "wb" //wb
#define FMTREAD "rb"  //rb

#define SNDLISTEXT "slt"

#define MAXSNDNAME 8+1
#define MAXSNDENVNAME 30+1

#define MAXLOADNAME 8+1

#define MAXSIZELOADPATH MAX_PATH

//Sound property
typedef enum {
SNDPROP_NO=0x00000000,
SNDPROP_LOOP=0x00000001
} SNDPROP;   //every bit, when it is on, enables an effect (i.e. loop, on PSX reverb, )

typedef float PREAL;
#define PREAL_MAX FLT_MAX
typedef PREAL ANGLE;

typedef PREAL MYMATRIX[16];
typedef PREAL MYVECTOR[4];

typedef DWORD SNDHANDLE;

typedef char SNDNAME[MAXSNDNAME];
typedef char LOADHANDLE[MAXLOADNAME];
typedef char LOADPATH[MAXSIZELOADPATH];
//to substitute  the kind of type??
typedef char SND_GAME_ENV[MAXSNDENVNAME];//, *LPSND_GAME_ENV; 
#define NOSNDENV "" //0 no env and no link

/////////////////////////////////////////////////////////////////////
typedef struct SNDLISTITEMtag {
	SNDNAME name;
	SND_GAME_ENV env; //if the game is in this environment I load relative sounds
	SND_GAME_ENV envLink; // the environment requires also other sound of
						// other environment: example port requires also common
} SNDLISTITEM, *LPSNDLISTITEM;

/////////////////////////////////////////////////////////////////////
typedef struct SNDLOADINFOtag{
	LOADHANDLE loadBlock; //Blocco di appartenenza per il loading
	LOADPATH loadPath;  //Path in cui si trovano i relativi file
}SNDLOADINFO;

/////////////////////////////////////////////////////////////////////
typedef struct SND3DPROPStag{
	DWORD dwInsideConeAngle;  //Range=(0-360)
    DWORD dwOutsideConeAngle; //Range=(dwInsideConeAngle-360)
	DWORD OutsideVolumePerCent; //il volume esterno al cono espresso come 
								//percentuale del volume interno
	PREAL flMinDistance; //D3DVALUE //Constraint >=0
    PREAL flMaxDistance; //D3DVALUE ////Constraint >=flMinDistance	
} SND3DPROPS, *LPSND3DPROPS;

/////////////////////////////////////////////////////////////////////
typedef struct SND3DINFOtag {
	SND3DPROPS props;
	DWORD setFlag;
} SND3DINFO, *LPSND3DINFO;

/////////////////////////////////////////////////////////////////////
typedef struct SNDINFOtag{
	LOADHANDLE loadID; // Id del suono nel blocco per il loading	
	DWORD sndClass;	// classe del suono: è legato al change di volume: SFX, COMMENT, ...
	DWORD sndType; // tipo del suono: 3D, STREAMING, ...
	DWORD nInstRequired; // istanze del suono da creare 	
	SND3DINFO param3D; //????
} SNDINFO, *LPSNDINFO;


/////////////////////////////////////////////////////////////////////
// Sound Sample format
typedef struct SNDMIXFORMATtag{
	WORD  nChannels;
	DWORD nSamplesPerSec; 
	WORD  wBitsPerSample; 
} SNDMIXFORMAT, *LPSNDMIXFORMAT;

/////////////////////////////////////////////////////////////////////
typedef struct HWSNDBUFtag{		
	LPDIRECTSOUNDBUFFER sndBuf;	
	SNDMIXFORMAT fmt;
	DWORD size;
	DWORD bufPos;
	BOOL reload;
	BOOL loadInHw; // it shows if the instance is loaded or not in hw
	SNDPROP propPlay; // play parameters: LOOP, ...
	DWORD volume; // percentuale corrente del volume del tipo di suono def=100
	int pan; 	// pan ?		
}  HWSNDBASE, *LPHWSNDBASE;

/////////////////////////////////////////////////////////////////////
typedef struct SND3DCYNEMATICtag{
	MYVECTOR  vPosition; //D3DVECTOR
    MYVECTOR  vVelocity; //D3DVECTOR
	MYVECTOR  vConeOrientation; ////D3DVECTOR	
}SND3DCYNEMATIC, *LPSND3DCYNEMATIC;

/////////////////////////////////////////////////////////////////////
typedef struct SND3DLINKOBJtag{
	PREAL *lpPosition; // linked obj position
}SND3DLINKOBJ, *LPSND3DLINKOBJ;

//----------------------------------------------------
typedef struct HWSND3DPARTtag{
	SND3DPROPS props;
	SND3DCYNEMATIC cynemObj;
	SND3DLINKOBJ linkObj;
	DWORD dwMode; //disable or enable sound 
	BOOL emul3D;
	LPDIRECTSOUND3DBUFFER sndBuf3D;
} HWSND3DPART, *LPHWSND3DPART;

/////////////////////////////////////////////////////////////////////
typedef struct HWSNDOBJtag{
	HWSNDBASE base;
	HWSND3DPART p3D;
	//DWORD sndType;
	DWORD cls;
	//struct SNDOBJtag *lpFather;
	//BOOL duplicate;	
	SNDHANDLE hndPlay;
} HWSNDOBJ, *LPHWSNDOBJ;

/////////////////////////////////////////////////////////////////////
typedef struct SNDOBJtag{
	SNDINFO info;	
	SNDLOADINFO loadInfo;
	SNDMIXFORMAT fmt;
	PREAL timeLength; //time length in  secs
	BYTE *lpMem;//if !=NULL it contains the whole buffer sound (included header and tail)
	DWORD nInst; //not important if streaming
	LPHWSNDOBJ *instVector;	//NULL if streaming
} SNDOBJ, *LPSNDOBJ;

/////////////////////////////////////////////////////////////////////
typedef struct{
	SNDLISTITEM li;
	LPSNDOBJ *table;
	DWORD nSnd;
	LOADHANDLE loadBlock; //Block of belongings for the loading	
	LOADPATH loadPath;    //Path in which the relative rows are found	
} SNDSCENE, *LPSNDSCENE;


#endif