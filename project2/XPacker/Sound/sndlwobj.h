#ifndef __SNDLWOBJ
#define __SNDLWOBJ

//WARNING
//This is a part of sndlow.h

#define MAXSIZELOADPATH MAX_PATH
typedef char LOADPATH[MAXSIZELOADPATH];
#define MAXLOADNAME 8+1
typedef char LOADHANDLE[MAXLOADNAME];

#define SND_INCONESET   0x00000001
#define SND_OUTCONESET	0x00000002
#define SND_MINDSET		0x00000004
#define SND_MAXDSET		0x00000008
#define SND_OUTVOLSET	0x00000010
#define SND_ALLSET		0x0000001F

#define SND3D_DISABLE		DS3DMODE_DISABLE
#define SND3D_HEADRELATIVE	DS3DMODE_HEADRELATIVE 
#define SND3D_NORMAL		DS3DMODE_NORMAL 

typedef DWORD SNDHANDLE;




typedef struct SND3DPROPStag{
	DWORD dwInsideConeAngle;  //Range=(0-360)
    DWORD dwOutsideConeAngle; //Range=(dwInsideConeAngle-360)
	DWORD OutsideVolumePerCent; //il volume esterno al cono espresso come 
								//percentuale del volume interno
	PREAL flMinDistance; //D3DVALUE //Constraint >=0
    PREAL flMaxDistance; //D3DVALUE ////Constraint >=flMinDistance	
} SND3DPROPS, *LPSND3DPROPS;

typedef struct SND3DINFOtag {
	SND3DPROPS props;
	DWORD setFlag;
} SND3DINFO, *LPSND3DINFO;


typedef struct SNDLOADINFOtag{
	LOADHANDLE loadBlock; //Blocco di appartenenza per il loading
	LOADPATH loadPath;  //Path in cui si trovano i relativi file
}SNDLOADINFO;

typedef struct SNDINFOtag{
	LOADHANDLE loadID; // Id del suono nel blocco per il loading	
	DWORD sndClass;	// classe del suono: è legato al change di volume: SFX, COMMENT, ...
	DWORD sndType; // tipo del suono: 3D, STREAMING, ...
	DWORD nInstRequired; // istanze del suono da creare 	
	SND3DINFO param3D; //????
} SNDINFO, *LPSNDINFO;


typedef struct SND3DCYNEMATICtag{
	MYVECTOR  vPosition; //D3DVECTOR
    MYVECTOR  vVelocity; //D3DVECTOR
	MYVECTOR  vConeOrientation; ////D3DVECTOR	
}SND3DCYNEMATIC, *LPSND3DCYNEMATIC;

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


typedef struct HWSNDOBJtag{
	HWSNDBASE base;
	HWSND3DPART p3D;
	//DWORD sndType;
	DWORD cls;
	//struct SNDOBJtag *lpFather;
	//BOOL duplicate;	
	SNDHANDLE hndPlay;
} HWSNDOBJ, *LPHWSNDOBJ;

typedef struct SNDOBJtag{
	SNDINFO info;	
	SNDLOADINFO loadInfo;
	SNDMIXFORMAT fmt;
	PREAL timeLength; //time length in  secs
	BYTE *lpMem;//if !=NULL it contains the whole buffer sound (included header and tail)
	DWORD nInst; //not important if streaming
	LPHWSNDOBJ *instVector;	//NULL if streaming
} SNDOBJ, *LPSNDOBJ;


enum HWSTREAMSTATEtag {HWSTR_NULL,HWSTR_READY, HWSTR_START, HWSTR_PLAYING, HWSTR_STOP};
enum whereTypetag{HWSTR_MEM,HWSTR_BLOCK,HWSTR_FILE};
#include "unpack.h"
typedef struct HWSNDSTREAMtag{	
	void *pf;	// runtime file pointer
//	DWORD offsetStartFile; // begin of real sample in the file 
	DWORD state; // state of stream
	DWORD blockSize; // block size to write 
	DWORD nextBlock; //next block to write
	//DWORD nextWriteOffset; //offset after which we write
	DWORD nBlock; // è dato da size del buffer / blocco size
	DWORD dataWritten;
	DWORD propPlay;		
	int whereType; // it's false if direct file, true if file is in a block	
	DWORD dwDataLength;	
	LPPACKEDFILE lpPf;
	size_t (*fread)( void *buffer, size_t size, size_t count, void *stream );
	long (*ftell)(void *stream);
	int (*fseek)(void *stream, long offset, int origin );	
} HWSNDSTREAM, *LPHWSNDSTREAM;
 
typedef struct SNDSTREAMtag{
	DWORD size; //size of stream buffer	
	HWSNDSTREAM strData;
	LPHWSNDOBJ lpHwSo;	
} SNDSTREAM, *LPSNDSTREAM;

typedef struct SNDPLAYSLOTtag {
	LPHWSNDOBJ lpso;
	DWORD currHnd;
	DWORD ind;
	BOOL pauseOn;
	struct SNDPLAYSLOTtag *next;
	struct SNDPLAYSLOTtag *prev;
} SNDPLAYSLOT, *LPSNDPLAYSLOT;


typedef struct SNDLSTN_OBJtag{
	MYVECTOR vPosition; //D3DVECTOR 
    MYVECTOR vVelocity; //D3DVECTOR
    MYVECTOR vOrientFront; //D3DVECTOR  
    MYVECTOR vOrientTop;  //D3DVECTOR  
} SNDLISTENER_OBJ, *LPSNDLISTENER_OBJ;

typedef struct SNDLSTN_LINKOBJtag{
	PREAL *lpPosition;
	//PREAL *lpVelocity; // It must be computed
	PREAL *lpOrientFront;
	PREAL *lpOrientTop;//Parameters from Program
}SNDLISTENER_LINKOBJ, *LPSNDLISTENER_LINKOBJ;

typedef struct SNDLISTENERtag{
	SNDLISTENER_OBJ obj;
	SNDLISTENER_LINKOBJ linkObj;
} SNDLISTENER, *LPSNDLISTENER;

typedef struct DSOUNDENV{
	LPDIRECTSOUND lpDS;
	LPDIRECTSOUNDBUFFER lpDSBuffPrimary;
	BOOL linkToMainDS; 
} WINDS_SNDENV, *LPWINDS_SNDENV;

#endif