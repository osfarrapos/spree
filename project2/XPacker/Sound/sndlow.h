#ifndef __LOWSND
#define __LOWSND

/* 
for the following symbols:
	type: BYTE, WORD, DWORD, BOOL, LONG, LPVOID
	constant: MAX_PATH 
	// For PREAL & MYVECTOR type
*/
#include "plattype.h"

// for sound structures hw dependent
//#include "platsnd.h"


#include "sndexter.h"

#define SND_NORMAL			0x00000000 // Normal Sound
#define SND_3D				0x00000001 // 3D Sound
#define SND_USECPURAM		0x00000002 // Not use snd card memory
#define SND_STREAMING		0x00000004 // Stream the sample from file 
#define SND_LOADRAM			0x00000008 // the sample is loaded also in RAM //??

//max absolute pan it's in percent, i.e. range= -100 (all left), 100 (all right)
#define SNDMAXPAN 100

// max volume 
#define SNDMAXVOLUME 100

//Sound property
typedef enum {
SNDPROP_NO=0x00000000,
SNDPROP_LOOP=0x00000001
} SNDPROP;   //every bit, when it is on, enables an effect (i.e. loop, on PSX reverb, )

// Sound Sample format
typedef struct SNDMIXFORMATtag{
	WORD  nChannels;
	DWORD nSamplesPerSec; 
	WORD  wBitsPerSample; 
} SNDMIXFORMAT, *LPSNDMIXFORMAT;

typedef struct SNDENVtag{
	PLATFHWND hWndMain; // handle of mainwindow
	//DWORD coopLev; // cooperative Level of application
	SNDMIXFORMAT fmt; // format requested and available
	BOOL enable3DSnd; //use 3D sounds, If I can?
	DWORD n3DSndForced; // use ds to 3d also if card not supports hw 3d buffer
	int emulLev; // emualtion level. 0: No emul (HAL); 1: DS emul; 2: WIN emul;	
	BOOL hwSupport3D;
} SNDENV, *LPSNDENV;


#include "sndlwobj.h"
#include "sndcls.h"
#include "myerror.h"
#include "funcalloc.h"

#include "sndlowfn.h"

#define SNDNOHANDLE 0 //0xFFFFFFFF

#ifndef DSBFREQUENCY_ORIGINAL
#define DSBFREQUENCY_ORIGINAL 0
#endif

#endif