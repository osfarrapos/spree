/* ------------------------------------------------------------------
	Low Level Sound Support Functions

	Pixelstorm Copyright - by Teodoro Di Bello

	FILE: supplsnd.c
 
	Version: 0.5		Date: 16/02/98

	Description: 
		   
	Note:	
  ------------------------------------------------------------------ */
#include "sndsuppl.h"
#include "myerror.h"
#include "myutil.h"
#include "funcalloc.h"

//--------------------------------------------------------------------------
// Conversion Routines from SNDMIXFORMAT to WAVEFORMATEX and viceversa!

// Vectors that stores the possible formats (channels,frequency, Bit resolution)
static WORD nChannelsBaseV[]={1,2}; //it must be setted in increasing value
#define N_CH (NELEMV(nChannelsBaseV))
//it must be setted in increasing value
static DWORD nSamplesPerSecBaseV[]={8000,11025,22050,33075,44100};
#define N_SPS (NELEMV(nSamplesPerSecBaseV))
static WORD  wBitsPerSampleBaseV[]={8,16}; //it must be setted in increasing value
#define N_BPS (NELEMV(wBitsPerSampleBaseV))

void SndFmt2WaveFmt(SNDMIXFORMAT *pfmt,WAVEFORMATEX *pwfx)
{
	ASSERTPOINTER(pfmt!=NULL && pwfx!=NULL);
	INITTOZERO(pwfx,sizeof(WAVEFORMATEX));
	pwfx->wFormatTag = WAVE_FORMAT_PCM;
	pwfx->nChannels= pfmt->nChannels;
	pwfx->nSamplesPerSec=pfmt->nSamplesPerSec;
	pwfx->wBitsPerSample=pfmt->wBitsPerSample;
	// The nBlockAlign calculation below only works for whole-byte samples
	ASSERTRANGE( pwfx->wBitsPerSample % 8 == 0 );		
	pwfx->nBlockAlign=pwfx->nChannels * (pwfx->wBitsPerSample / 8);
	pwfx->nAvgBytesPerSec= pwfx->nBlockAlign * pwfx->nSamplesPerSec;
}

void WaveFmt2SndFmt(WAVEFORMATEX *pwfx, SNDMIXFORMAT *pfmt)
{
	pfmt->nChannels=pwfx->nChannels;
	pfmt->nSamplesPerSec=pwfx->nSamplesPerSec;
	pfmt->wBitsPerSample=pwfx->wBitsPerSample;
}

//--------------------------------------------------------------------------
// First check about snd card capability and change the requested format to
// the available format (without change frequency)
// Return FALSE if the snd card doesn't support any format (it's not very feasible)
BOOL MakeWaveFmtCompatible(WAVEFORMATEX *pwfx, LPDSCAPS lpdsCaps)
{
	BOOL bflag=TRUE;
	
	ASSERTPOINTER(pwfx!=NULL && lpdsCaps);
	ASSERTRANGE(pwfx->nChannels==2 || pwfx->nChannels==1);
	if(pwfx->nChannels==2) //STEREO
		if((lpdsCaps->dwFlags & DSCAPS_PRIMARYSTEREO)==0)
			pwfx->nChannels=1;
	if(pwfx->nChannels==1) //MONO
		if((lpdsCaps->dwFlags & DSCAPS_PRIMARYMONO)==0)
			bflag=FALSE; // IT'S VERY STRANGE BUT POSSIBLE, THE SOUND CARD ISN'T GOOD ;-)							
	ASSERTRANGE(pwfx->wBitsPerSample==16 || pwfx->wBitsPerSample==8);
	if(pwfx->wBitsPerSample==16) //16 Bit Sample
		if((lpdsCaps->dwFlags & DSCAPS_PRIMARY16BIT)==0)
			pwfx->wBitsPerSample=8;
	if(pwfx->wBitsPerSample==8) //8 Bit Sample
		if((lpdsCaps->dwFlags & DSCAPS_PRIMARY8BIT)==0)
			bflag=FALSE; // IT'S VERY STRANGE BUT POSSIBLE, THE SOUND CARD ISN'T GOOD ;-)
	return bflag;
}

//--------------------------------------------------------------------------
// It returns the next possible format, changes only frequency, if there is!
// Return FALSE if no formats are available!
BOOL TryOtherSndFormat(SNDMIXFORMAT *pfmt)
{
	int i;

	ASSERTPOINTER(pfmt!=NULL);
	// find the first descending frequency		
	for(i=N_SPS-1; i>=0; i--){		
		if(nSamplesPerSecBaseV[i]>pfmt->nSamplesPerSec)
			continue;						
		pfmt->nSamplesPerSec=nSamplesPerSecBaseV[i];
		break;
	}
	return (i>=0);
}
//--------------------------------------------------------------------------

#include <math.h>
static double mylog2=0.0;

// volume conversion table
static LONG sndVolumeTable[SNDMAXVOLUME+1];
static LONG sndPanTable[SNDMAXPAN+1]; //2*

// Set and Get volume and pan
// Note:
// In DS there is no amplification only attenuation!
// (DSBVOLUME_MAX=0 DSBVOLUME_MIN=-10000)
// In DS Sound System the volume unit is equal to 1/100 of decibel
// and decibel is a logarithmic measure, i.e. an attenuation of 10 db 
// means to reduce the sound volume to the half one, of 20 db to the quarter one
// I suppose that the law is
// volume<->2^(1/10 *(n decibel))=2^(1/10* (n*(100 ds unit))=
//		 = 2^(10*n ds unit)=2^(N ds unit)
// Now volume must range between 0 (-10000 ds unit) to SNDMAXVOLUME (0 ds unit)
// and it is linear 
// DO I PRECOMPUTE THESE VALUES AND STORE THEM IN A TABLE?

// Create table 
void CreateVolumeTable(void)
{
	DWORD i;

	for(i=0; i<=SNDMAXVOLUME; i++){
		sndVolumeTable[i]= (LONG) LinearScale2DSScale(i,SNDMAXVOLUME);
	}
}

void CreatePanTable(void)
{
	int i,pan;

	for(pan=0,i=0; pan<=SNDMAXPAN; pan++,i++){//-SNDMAXPAN
		sndPanTable[i]=(LONG) LinearScale2DSScale(SNDMAXPAN-SIGN(pan)*pan, SNDMAXPAN);
		sndPanTable[i]*=(-1); 
	}
}

double LinearScale2DSScale(DWORD linearValue, DWORD linearMax)
{
	double dVol;
	if(linearValue==0)
		return DSBVOLUME_MIN;
	if(mylog2==0.0) mylog2=log((double)2.0);
	dVol=(double)linearValue/ (double) linearMax;	
	dVol=(log(dVol)/mylog2)*1000;
	return dVol;
}

LONG HwSndObjConvertVolume(DWORD volume)
{
	if (volume>=SNDMAXVOLUME)
		return DSBVOLUME_MAX;
	else if(volume==0)
		return DSBVOLUME_MIN;
	else		
		return (sndVolumeTable[volume]);
}

LONG HwSndObjConvertPan(int pan)
{
	LONG hwPan;
	if(!pan)
		hwPan=DSBPAN_CENTER; //i.e. =0;
	else if(pan<=-SNDMAXPAN)
		hwPan=DSBPAN_LEFT; 
	else if(pan>=SNDMAXPAN)
		hwPan=DSBPAN_RIGHT;
	else {
		if(pan>=0)
			hwPan=sndPanTable[pan];
		else //pan<0
			hwPan=-sndPanTable[-pan];
	}
	return hwPan;
}

DWORD DSVolume2LinVol(LONG volDS)
{
	if (mylog2==0.0) 
		mylog2=log((double)2.0);
	if (volDS>=DSBVOLUME_MAX)
		return SNDMAXVOLUME;
	else if(volDS<=DSBVOLUME_MIN)
		return 0;
	else
		return (DWORD) (exp( volDS*mylog2/1000 )*SNDMAXVOLUME);
}

DWORD HwSndConvertMode(DWORD mode)
{
	DWORD dsmode;
	switch(mode){
	case SND3D_NORMAL:
		dsmode=DS3DMODE_NORMAL;
		break;
	case SND3D_DISABLE:
		dsmode=DS3DMODE_DISABLE;
		break;
	case SND3D_HEADRELATIVE:
		dsmode=DS3DMODE_HEADRELATIVE ;
		break;
	default:
		dsmode=DS3DMODE_NORMAL;
		break;
	}
	return dsmode;
}

//--------------------------------------------------------------------------
//  Fill a DS buffer with the sample
BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD cbWaveSize)
{	
	if (pDSB && pbWaveData && cbWaveSize) {
		LPVOID pMem1, pMem2;
		DWORD dwSize1, dwSize2;
		HRESULT hr;

		if (SUCCEEDED(hr=IDirectSoundBuffer_Lock(pDSB, 0, cbWaveSize,
							&pMem1, &dwSize1, &pMem2, &dwSize2, 0))) {
			CopyMemory(pMem1, pbWaveData, dwSize1);
			if ( 0 != dwSize2 )
				CopyMemory(pMem2, pbWaveData+dwSize1, dwSize2);
			hr=IDirectSoundBuffer_Unlock(pDSB, pMem1, dwSize1, pMem2, dwSize2);
			return TRUE;
		}
		else
			DEBUGMYERROR(DSTAB,hr);
	}
	return FALSE;
}

BOOL DSFillSoundBufferByte(IDirectSoundBuffer *pDSB, BYTE value, DWORD cbWaveSize)
{	
	if (pDSB && cbWaveSize) {
		LPVOID pMem1, pMem2;
		DWORD dwSize1, dwSize2;
		HRESULT hr;

		if (SUCCEEDED(hr=IDirectSoundBuffer_Lock(pDSB, 0, cbWaveSize,
							&pMem1, &dwSize1, &pMem2, &dwSize2, 0))) {
			FillMemory(pMem1, dwSize1,value);
			if ( 0 != dwSize2 )
				FillMemory(pMem2, dwSize2,value);
			hr=IDirectSoundBuffer_Unlock(pDSB, pMem1, dwSize1, pMem2, dwSize2);
			return TRUE;
		}
		else
			DEBUGMYERROR(DSTAB,hr);
	}
	return FALSE;
}

//--------------------------------------------------------------------------
// Returns the ds flags corrispondent to the sound type 
// to create the ds buffer
#ifndef DSBCAPS_CTRLDEFAULT   //for compatibility with DirectX7
#define DSBCAPS_CTRLDEFAULT   0x000000E0
#endif

DWORD HwSndType2Flag(DWORD sndType)
{
	DWORD sndFlag=0;

	sndFlag=DSBCAPS_CTRLDEFAULT;
	if(sndType & SND_3D)
		sndFlag |=DSBCAPS_CTRL3D;
	if(sndType & SND_USECPURAM)
		sndFlag |=DSBCAPS_LOCSOFTWARE;
	else
		sndFlag |=DSBCAPS_STATIC;
	if(sndType & SND_STREAMING)
		sndFlag |=DSBCAPS_GETCURRENTPOSITION2;// | DSBCAPS_CTRLPOSITIONNOTIFY	
	return sndFlag;
}

//--------------------------------------------------------------------------
//  ParseWaveMemory
//   Parses the loaded wave file in memory into its 
//	 header and samples.
//   To do this, search for the "fmt " and "data"
//   fields.
BOOL ParseWaveMemory(LPVOID lpMemory,  // Pointer to ram   (Input)
					 LPWAVEFORMATEX  *lplpWaveHeader, // Pointer to a pointer to the header (Output)
					 LPBYTE          *lplpWaveSamples,// Pointer to a pointer to the samples (Output)
					 LPDWORD         lpWaveSize)      // Pointer to the size (Output)
{
    LPDWORD pointMem;
    LPDWORD pdwEnd;
    DWORD   dwRiff;
    DWORD   dwType;
    DWORD   dwLength;

	if(lpMemory==NULL) 
		return FALSE; 
    // Initialize the default values of the parameters passed in
    if (lplpWaveHeader)
        *lplpWaveHeader = NULL;

    if (lplpWaveSamples)
        *lplpWaveSamples = NULL;

    if (lpWaveSize)
        *lpWaveSize = 0;

    // Set up a pointer to indicate the start of the wave
    // memory.
    pointMem = (DWORD *)lpMemory;

    // Get the type and length of the wave memory
    dwRiff = *pointMem++;
    dwLength = *pointMem++;
    dwType = *pointMem++;

    // Use the mmioFOURCC macro from the Windows SDK to verify
    // that this is a RIFF WAVE memory chunk
    if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F'))
      return FALSE;      // not even RIFF

    if (dwType != mmioFOURCC('W', 'A', 'V', 'E'))
      return FALSE;      // not a WAV

    // Get the pointer to the end of our wave memory
    pdwEnd = (DWORD *)((BYTE *)pointMem + dwLength-4);

    // Walk through the bytes and locate the tags
    while (pointMem < pdwEnd) {
		dwType   = *pointMem++;
		dwLength = *pointMem++;
		
		switch (dwType) {
			// Located the format portion
		case mmioFOURCC('f', 'm', 't', ' '):
			if (lplpWaveHeader && !*lplpWaveHeader) {
				if (dwLength < sizeof(WAVEFORMAT))
					return FALSE; // Drop out because this is not a WAV
				
				// Set the lplpWaveHeader parameter to point to this piece of
				// the memory
				*lplpWaveHeader = (LPWAVEFORMATEX)pointMem;
				
				// See if we have the bits and the size of the
				// bits filled out yet. If they are, then we have fully parsed
				// the memory and can return happy
				if ((!lplpWaveSamples || *lplpWaveSamples) &&
					(!lpWaveSize || *lpWaveSize)) {
					return TRUE;
				}
			}
			break;
			
			// We are at the the samples - fill the values to return 
			// for the sample and size
		case mmioFOURCC('d', 'a', 't', 'a'):
			if ((lplpWaveSamples && !*lplpWaveSamples) ||
				(lpWaveSize && !*lpWaveSize)) {
				// Set the samples pointer to point to this location
				// in the memory.
				if (lplpWaveSamples) 
					*lplpWaveSamples = (LPBYTE)pointMem;
				
				// Set the size of the wave
				if (lpWaveSize)
					*lpWaveSize = dwLength;
				
				// If the header pointer is filled, we can return happy..
				if (!lplpWaveHeader || *lplpWaveHeader)
					return TRUE;
			}
			break;
			
		}
		
		// Set the pointer to the next portion of memory
		pointMem = (DWORD *)((BYTE *)pointMem + ((dwLength+1)&~1));
	}
	
	// If we get to this point, we failed and did not aquire all of the
	// wave data - Return Failure.
	return FALSE;
}

					 
//		SOUND LOAD PART
#include "unpack.h"
static char nameBuf[MAX_PATH];
static char nameSnd[MAX_PATH];	

// does it handle the drive problem? if from cd or from hdisk
void *HwSndObjLoadFile(LOADPATH lPath, LOADHANDLE lID)
{
	DWORD sizeBuff;
	FILE *pf;
	BYTE *lpMem=NULL,*lpScan;
	int c;
	char nameBuf[MAX_PATH];
	if(!lPath || !lID)
		return NULL;
	BuildFullPath(nameBuf,lPath, lID,sndDriveContext,SNDIDEXT); // 'c' ?? 
	pf=fopen(nameBuf, "rb");
	ASSERTFILE(pf!=NULL,nameBuf);
	if(pf!=NULL){
		fseek(pf,0,SEEK_END);	
		sizeBuff=ftell(pf);
		lpScan=lpMem=MyMalloc(BYTE, sizeBuff);
		ASSERTOUTMEMORY(lpMem!=NULL);
		fseek(pf,0,SEEK_SET);
		while( (c=getc(pf))!=EOF)
			*lpScan++=c;
		fclose(pf);
	}
	return (void *)lpMem;
}


void *HwSndObjLoad(LOADPATH lPath, LOADHANDLE lBlock, LOADHANDLE lID)
{
	DWORD sizeBuff;
	BYTE *lpMem=NULL;
	//static 
	LPPACKEDFILE lpPf=NULL;
	if(!lPath || !lID ||!lBlock)
		return NULL;	
	if(lBlock[0]!='\0'){				
		BuildFullPath(nameBuf,lPath, lBlock, sndDriveContext,SNDBLOCKEXT); // 'c' ?? 
		lpPf=PkPackInit(nameBuf, 1, NULL);
		ASSERTFILE(lpPf,nameBuf);
		BuildFullPath(nameSnd,NULL, lID,'0',SNDIDEXT); 
		lpMem=PkUnpack2Memory(lpPf,nameSnd, &sizeBuff);	
		ASSERTFILE(lpMem,nameSnd);
		PkPackClose(lpPf);			
	}
	else
		lpMem=HwSndObjLoadFile(lPath, lID);
	return (void *)lpMem;
}


//--------------------------------------------------------------------------
void SndObjGetFmt(BYTE *lpMem,LOADPATH lPath, LOADHANDLE lBlock, LOADHANDLE lID,
				  LPSNDMIXFORMAT lpFmt, LPDWORD lpLengthData)
{
	void *pf = 0;
	char nameBuf[MAX_PATH];
	_WAVHEADER wavHeader;
	
	LPPACKEDFILE lpPf=NULL;
	if(lpMem){
		COPYMEMORY(&wavHeader,lpMem,sizeof(wavHeader));
	}
	else{	
		if(lBlock[0]=='\0'){
			BuildFullPath(nameBuf,lPath, lID,sndDriveContext,SNDIDEXT);
			pf=fopen(nameBuf,"rb");
			ASSERTFILE(pf,nameBuf);
			fread(&wavHeader, 1, sizeof( _WAVHEADER ), pf);		
		}
		else{
			BuildFullPath(nameBuf,lPath, lBlock, sndDriveContext,SNDBLOCKEXT); // 'c' ??
			lpPf=PkPackInit(nameBuf, 1, NULL);
			ASSERTFILE(lpPf,nameBuf);
			BuildFullPath(nameBuf,NULL, lID, sndDriveContext,SNDIDEXT);
			pf=Pkfopen(lpPf,nameBuf,"rb");
			ASSERTFILE(pf,nameBuf);
			Pkfread(&wavHeader, 1, sizeof( _WAVHEADER ), pf);
		}	
	}
	lpFmt->nSamplesPerSec=wavHeader.dwSamplesPerSec;	
	lpFmt->wBitsPerSample=wavHeader.wBitsPerSample;
	lpFmt->nChannels=wavHeader.wChannels;
	*lpLengthData=wavHeader.dwDataLength;
	if (!lpMem) {
		if (lBlock[0]=='\0' && pf)
			fclose(pf);
		else {
			if (pf) Pkfclose(pf);
			PkPackClose(lpPf);
		}
	}
	return;
}


//--------------------------------------------------------------------------
// Virtual read bytes from a file in the dat file 
// Synopse equal to standard c fread
LPMEMFILE Memfopen(void *lpMem,char *mode)
{
	static int ind=-1;
	LPMEMFILE lpMemFile=NULL;
	LPBYTE lpWaveSamples;
	LPWAVEFORMATEX lpfmtex;
	DWORD size;

	if(ParseWaveMemory(lpMem,&lpfmtex, &lpWaveSamples,&size)){
		lpMemFile=MyMalloc(MEMFILE,1);
		ASSERTOUTMEMORY(lpMemFile!=NULL);
		lpMemFile->lpMem=lpMem;
		lpMemFile->poscur=0; // cur position in the file
		lpMemFile->size=size+sizeof(_WAVHEADER);	
		lpMemFile->eof=0;
	}
	return lpMemFile;
}

size_t Memfread(void *ptr, size_t size, size_t n, LPMEMFILE lpMemFile)
{
	size_t rval=0;
	size_t memRead=0;
	void *readPtr;

	if(lpMemFile!=NULL && !lpMemFile->eof) {
		readPtr=lpMemFile->lpMem+lpMemFile->poscur;
		memRead=n*size;
		rval=n;
		
		if(lpMemFile->poscur+ memRead>= lpMemFile->size){
			memRead=lpMemFile->size-lpMemFile->poscur;
			rval=memRead/size;
			lpMemFile->eof=1;
			lpMemFile->poscur=lpMemFile->size;
		}
		else
			lpMemFile->poscur+=memRead;
		COPYMEMORY(ptr,readPtr,memRead);
	}
	return rval;
}
	
long Memftell(LPMEMFILE lpMemFile)
{
	return lpMemFile->poscur;
}

int Memfseek(LPMEMFILE lpMemFile, long offset, int whence)
{
	long newpos;
	int rval=1;
	
	switch(whence){
	case SEEK_SET:
		newpos=offset;
		break;
	case SEEK_CUR:
		newpos=lpMemFile->poscur+offset;
		break;
	case SEEK_END:
		newpos=lpMemFile->size-offset;
		break;
	default: 
		return rval; // break;
	}

	if( newpos>=0 && ((DWORD) newpos)<lpMemFile->size)
	{
		lpMemFile->poscur=newpos;		
		rval=0;
		lpMemFile->eof=0;
	}
	return rval;
}

int Memfclose(LPMEMFILE lpMemFile)
{
	if(lpMemFile== NULL)
		return 1;
	MyFree(lpMemFile);	
	return 0;
}
