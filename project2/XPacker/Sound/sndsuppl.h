#ifndef __SUPPLSND
#define __SUPPLSND

#include "sndlow.h"

// Error tab code

#define SNDIDEXT ".wav"
#define SNDBLOCKEXT ".dat"

// pack the structure on a byte alignment
#pragma pack( 1 )

// .WAV file header structure
typedef struct _tag_wavheader  
{
   BYTE szRIFF[ 4 ];
   DWORD dwFormatLength;
   BYTE szWAVE[ 4 ];
   BYTE szFMT[ 4 ];
   DWORD dwWaveFormatLength;
   short wFormatTag;
   short wChannels;
   DWORD dwSamplesPerSec;
   DWORD dwAvgBytesPerSec;
   short wBlockAlign;
   short wBitsPerSample;
   BYTE szDATA[ 4 ];
   DWORD dwDataLength;

} _WAVHEADER ;

#pragma pack()

void SndFmt2WaveFmt(SNDMIXFORMAT *pfmt,WAVEFORMATEX *pwfx);
void WaveFmt2SndFmt(WAVEFORMATEX *pwfx, SNDMIXFORMAT *pfmt);
BOOL MakeWaveFmtCompatible(WAVEFORMATEX *pwfx, LPDSCAPS lpdsCaps);
BOOL TryOtherSndFormat(SNDMIXFORMAT *pfmt);

//Volume & pan local
double LinearScale2DSScale(DWORD linearValue, DWORD linearMax);
DWORD DSVolume2LinVol(LONG volDS);
void CreateVolumeTable(void);
void CreatePanTable(void);
LONG HwSndObjConvertPan(int pan);
LONG HwSndObjConvertVolume(DWORD volume);

BOOL ParseWaveMemory(LPVOID lpMemory, LPWAVEFORMATEX  *lplpWaveHeader, 
								LPBYTE *lplpWaveSamples, LPDWORD lpWaveSize);

void *HwSndObjLoad(LOADPATH lPath, LOADHANDLE lBlock, LOADHANDLE lID);
DWORD HwSndConvertMode(DWORD mode);
void SndObjGetFmt(BYTE *lpMem,LOADPATH lPath, LOADHANDLE lBlock, 
							LOADHANDLE lID,	LPSNDMIXFORMAT lpFmt, LPDWORD lpLengthData);


BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD cbWaveSize);
BOOL DSFillSoundBufferByte(IDirectSoundBuffer *pDSB, BYTE value, DWORD cbWaveSize);

DWORD HwSndType2Flag(DWORD sndType);

#define SNDGETSILENCE(bitsPerSample) (BYTE) ( ((bitsPerSample)==8) ? 128 : 0)



typedef struct MEMFILEtag{
	BYTE *lpMem;
	DWORD poscur; // cur position in the file
	DWORD size;	
	int eof;
}MEMFILE, *LPMEMFILE;


LPMEMFILE Memfopen(void *lpMem,char *mode);
size_t Memfread( void *ptr, size_t size, size_t n, LPMEMFILE lpMemFile );
long Memftell(LPMEMFILE lpMemFile);
int Memfseek(LPMEMFILE lpMemFile, long offset, int whence );
int Memfclose(LPMEMFILE lpMemFile);

#endif