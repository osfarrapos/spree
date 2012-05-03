//Snd Stream


#include <stdio.h>
#include "sndlow.h" 
#include "sndsuppl.h"
#include "myerror.h"
#include "myutil.h"
#include "funcalloc.h"
// cd sound part I see it as low level sound
//#include "cdplay.h"

// Stream Sound functions
BOOL HwSndStreamPlayUpdate(LPSNDSTREAM lpss);
BOOL HwSndStreamStop(LPSNDSTREAM lpss);

#define KBYTE 1024
#define MINBLOCKSIZEKB 16 //it must be even becuase i use its half 
#define MINBLOCKSIZEKBDIV2 MINBLOCKSIZEKB/2 
#define MINBLOCKSIZE MINBLOCKSIZEKB*KBYTE

#define FREQ_DFLT 22050
#define NMAXCHN 2  //mono o stereo
#define NMAXBITS 2 //8bit o 16bit
#define NTYPES  2  //normal, 3d
#define GETICHN(nChannels) ((nChannels)-1)
#define GETIBITS(wBitsPerSample) (((wBitsPerSample)>>3)-1)
#define GETITYPES(type) ( ((type) & SND_3D) ? 1 : 0)

#define GETCHN(ind) (ind+1) 
#define GETBITS(ind) ((ind+1)<<3) 

typedef struct SNDSTREAMBLOCKtag {
	LPSNDSTREAM *vlpss;
	DWORD n;	
}SNDSTREAMBLOCK, *LPSNDSTREAMBLOCK;

static SNDSTREAMBLOCK sndStream[NMAXCHN][NMAXBITS][NTYPES]; //[nChannel][nByte]
/*
	(mono 8bit ) (mono 16bit)
	(stereo 8bit) (stereo 16bit) (normal)
									(3d)	
*/
void SndStreamDestroyPerIndex(DWORD i, DWORD j, DWORD k);

void SndStreamInitSystem(void)
{
	DWORD i,j,k;
	for(i=0;i<NMAXCHN; i++) {
		for(j=0; j<NMAXBITS; j++) {
			for(k=0; k<NTYPES; k++) {
				sndStream[i][j][k].n=0;
				sndStream[i][j][k].vlpss=NULL;
			}
		}
	}
}

void SndStreamCreate(BOOL bStereo,BOOL b16Bits, DWORD type, DWORD nStream)
{
	SNDMIXFORMAT fmt;	
	DWORD sndType;
	DWORD i;
	DWORD iChn,iBits,iTypes;

	fmt.nSamplesPerSec=FREQ_DFLT;	
	fmt.nChannels=(bStereo) ? 2: 1;
	iChn=(bStereo) ? 1 :0;
	fmt.wBitsPerSample=(b16Bits) ? 16 : 8;	
	iBits=(b16Bits) ? 1: 0;
	
	iTypes=GETITYPES(type);	
	//if it already initialized I destroy it I destroy it
	if(sndStream[iChn][iBits][iTypes].vlpss) 
		SndStreamDestroyPerIndex(iChn,iBits,iTypes);
	sndType=SND_STREAMING;
	if(type & SND_3D )
		sndType|=SND_3D;
	if(type & SND_USECPURAM);
		sndType |= SND_USECPURAM; //
	sndStream[iChn][iBits][iTypes].vlpss=MyMalloc(LPSNDSTREAM,nStream);
	ASSERTOUTMEMORY(sndStream[iChn][iBits][iTypes].vlpss);
	
	for(i=0;i<nStream;i++){
		sndStream[iChn][iBits][iTypes].vlpss[i]=HwSndStreamCreate(&fmt, sndType);
		if(!sndStream[iChn][iBits][iTypes].vlpss[i])
			break;
	}
	sndStream[iChn][iBits][iTypes].n=i;	
}

void SndStreamDestroyPerIndex(DWORD i, DWORD j, DWORD k)
{
	DWORD h;
	if(sndStream[i][j][k].vlpss){
		for(h=0; h<sndStream[i][j][k].n; h++)
			HwSndStreamDestroy(sndStream[i][j][k].vlpss[h]);
		MyFree(sndStream[i][j][k].vlpss);
		sndStream[i][j][k].vlpss=NULL;
	}
	sndStream[i][j][k].n=0;
}

void SndStreamCloseSystem(void)
{
	DWORD i,j,k;
	for(i=0;i<NMAXCHN; i++)
		for(j=0; j<NMAXBITS; j++)
			for(k=0; k<NTYPES; k++)	
				SndStreamDestroyPerIndex(i,j,k);			
}

void SndStreamRTUpdate(void)
{
	DWORD i,j,k,h;
	for(i=0;i<NMAXCHN; i++) {
		for(j=0; j<NMAXBITS; j++) {
			for(k=0; k<NTYPES; k++) {				
				if(sndStream[i][j][k].vlpss) {
					for(h=0; h<sndStream[i][j][k].n; h++) {
						if(sndStream[i][j][k].vlpss[h])
							HwSndStreamPlayUpdate(sndStream[i][j][k].vlpss[h]);
					}
				}
			}				
		}
	}
}


BOOL SndStreamIsFree(LPSNDSTREAM lpss)
{
	if(lpss)
		if(!HwSndObjIsPlaying(lpss->lpHwSo))
			return HwSndStreamStop(lpss);			 		
	return FALSE;	
}

LPSNDSTREAM SndStreamGetFreeBuffer(DWORD sndType, LPSNDMIXFORMAT lpfmt)
{
	DWORD iChn,iBits,iTypes,i;
	LPSNDSTREAM lpss=NULL;
	iChn=GETICHN(lpfmt->nChannels);
	iBits=GETIBITS(lpfmt->wBitsPerSample);
	iTypes=GETITYPES(sndType);
	FATALASSERT(sndStream[iChn][iBits][iTypes].n>0 && 
		sndStream[iChn][iBits][iTypes].vlpss);
	if(sndStream[iChn][iBits][iTypes].n>0 && sndStream[iChn][iBits][iTypes].vlpss) {
		for(i=0;i<sndStream[iChn][iBits][iTypes].n;i++){
			if(SndStreamIsFree(sndStream[iChn][iBits][iTypes].vlpss[i])){
				lpss=sndStream[iChn][iBits][iTypes].vlpss[i];
				break;
			}
		}
	}
	return lpss;
}

BOOL HwSndStreamDestroy(LPSNDSTREAM lpss)
{	
	if(!lpss)
		return FALSE;
	HwSndStreamStop(lpss);
	HwSndObjDestroy(lpss->lpHwSo);
	MyFree(lpss);
	return TRUE;
}


LPSNDSTREAM HwSndStreamCreate(SNDMIXFORMAT *lpFmt, DWORD sndType)
{
	BYTE *pSampleMem=NULL;		
	LPSNDSTREAM lpSndStream=NULL;

	if(!HwSndGetCardDetect()) 
		return NULL;
	FATALASSERT(lpFmt!=NULL);
	//check ds object  & check parameter 
	lpSndStream=MyMalloc(SNDSTREAM,1);	
	ASSERTOUTMEMORY(lpSndStream!=NULL);	
	sndType|=SND_STREAMING; //it must be streaming!!!!
	lpSndStream->strData.nBlock=2;
	//lpHwSndStream->blockSize=dsBD.lpwfxFormat->nAvgBytesPerSec;
	lpSndStream->strData.blockSize=0;
	if(lpSndStream->strData.blockSize<MINBLOCKSIZE)
		lpSndStream->strData.blockSize=MINBLOCKSIZE;			

	lpSndStream->size=lpSndStream->strData.nBlock*lpSndStream->strData.blockSize; //32*1024;	
	lpSndStream->strData.state=HWSTR_READY;	
	lpSndStream->lpHwSo=HwSndObjCreate(NULL, &lpSndStream->size, lpFmt, sndType);
	ASSERTOUTMEMORY(lpSndStream->lpHwSo);
	return lpSndStream;
}

void HwSndStreamReset(LPHWSNDSTREAM lpHwSs)
{
	INITTOZERO(lpHwSs,sizeof(HWSNDSTREAM));
}


void HwSndStreamOpen(LPHWSNDSTREAM lpHwSndStream, void *lpMem, 
					 LOADPATH lPath, LOADHANDLE lBlock, LOADHANDLE lID)
{
	char nameBuf[MAX_PATH];
	if(lpMem!=NULL){		
		lpHwSndStream->whereType=HWSTR_MEM;
		lpHwSndStream->fread=Memfread;
		lpHwSndStream->ftell=Memftell;
		lpHwSndStream->fseek=Memfseek;
		lpHwSndStream->pf=Memfopen(lpMem,"rb");
	}
	else{
		if(lBlock[0]=='\0'){
			BuildFullPath(nameBuf,lPath, lID,sndDriveContext,SNDIDEXT);
			lpHwSndStream->pf=fopen(nameBuf,"rb");
			ASSERTFILE(lpHwSndStream->pf,nameBuf);
			lpHwSndStream->whereType=HWSTR_FILE;

			lpHwSndStream->fread=fread;
			lpHwSndStream->ftell=ftell;
			lpHwSndStream->fseek=fseek;
		}
		else{
			BuildFullPath(nameBuf,lPath, lBlock, sndDriveContext,SNDBLOCKEXT); // 'c' ??
			ASSERTFILE(lpHwSndStream->lpPf=PkPackInit(nameBuf, 1, NULL),nameBuf);
			BuildFullPath(nameBuf,NULL, lID, sndDriveContext,SNDIDEXT);
			lpHwSndStream->pf=Pkfopen(lpHwSndStream->lpPf,nameBuf,"rb");
			ASSERTFILE(lpHwSndStream->pf,nameBuf);
			lpHwSndStream->whereType=HWSTR_BLOCK;

			lpHwSndStream->fread=Pkfread;
			lpHwSndStream->ftell=Pkftell;
			lpHwSndStream->fseek=Pkfseek;
		}
	}
}

BOOL SndStreamPlay(LPSNDSTREAM lpss, void *lpMem,LOADPATH lPath, LOADHANDLE lBlock, 
				   LOADHANDLE lID,SNDPROP sndProp)
{
	DWORD wReadSize;
	HRESULT hr;
	_WAVHEADER wavHeader;
	LPHWSNDSTREAM lpHwSndStream;
	LPDIRECTSOUNDBUFFER sndBuf;
	
	lpHwSndStream=&lpss->strData;	
	sndBuf=lpss->lpHwSo->base.sndBuf;
	// open file 
	HwSndStreamOpen(lpHwSndStream,lpMem,lPath,lBlock, lID);
	if(lpHwSndStream->pf==NULL)
		return FALSE;
	// read header	
	wReadSize = lpHwSndStream->fread(&wavHeader, 1, sizeof( _WAVHEADER ), lpHwSndStream->pf);
	// check if format is ok, and set frequency
	hr=sndBuf->lpVtbl->SetFrequency(sndBuf,wavHeader.dwSamplesPerSec);
	if(FAILED(hr)) 
		DEBUGMYERROR(DSTAB,hr);
	ASSERTRANGE(lpss->lpHwSo->base.fmt.wBitsPerSample==wavHeader.wBitsPerSample);
	ASSERTRANGE(lpss->lpHwSo->base.fmt.nChannels==wavHeader.wChannels);

	lpHwSndStream->dwDataLength= wavHeader.dwDataLength;	
	
	// save sndprop //for loop
	lpHwSndStream->propPlay=sndProp;	
	// read bytes to fill stream buffer, if file ends set it	
	lpHwSndStream->state=HWSTR_START;
	lpHwSndStream->dataWritten=0;	

	if(HwSndStreamPlayUpdate(lpss))
		return HwSndObjPlay(lpss->lpHwSo, SNDPROP_LOOP); 	
	return FALSE;
}


BOOL HwSndStreamPlayUpdate(LPSNDSTREAM lpss)
{	
	BOOL writeFlag=FALSE;
	LPHWSNDOBJ lpso; // hw obj of sound stream obj
	DWORD currWriteOffset=0,writeOffset=0,currPlayOffset=0; 
	DWORD wReadSize;
	HRESULT hr;
	LPVOID pMem1, pMem2;
	DWORD dwSize1, dwSize2;
	LPHWSNDSTREAM lpHwSndStream;
	LPDIRECTSOUNDBUFFER sndBuf;
	lpHwSndStream=&lpss->strData;

	lpso=lpss->lpHwSo;
	sndBuf=lpso->base.sndBuf;
	if(lpHwSndStream->state==HWSTR_READY)
		return FALSE; //nothing to do
	if(lpso->base.reload){ // something go bad so we try to adjust it
		// we restore it
		HwSndObjRestore(lpso,NULL);		
		//if(lpHwSndStream->state==HWSTR_STOP)
		HwSndStreamStop(lpss);				
	}	
	if(lpHwSndStream->state==HWSTR_START){
		writeFlag=TRUE;
		writeOffset=0;
		lpHwSndStream->state=HWSTR_PLAYING;
		lpHwSndStream->nextBlock=0;	
	}
	else if(!HwSndObjIsPlaying(lpso)){
		lpHwSndStream->state=HWSTR_STOP;
		writeFlag=TRUE;
	}
	else{ // get position
		sndBuf->lpVtbl->GetCurrentPosition(sndBuf,&currPlayOffset,&currWriteOffset);		
		writeOffset=lpHwSndStream->nBlock*lpHwSndStream->blockSize;
		// I handle the last part of block in particular mode
		if(currPlayOffset>lpHwSndStream->blockSize){
			if(lpHwSndStream->nextBlock==0){
				writeFlag=TRUE;
				writeOffset=0; 
			}
		}
		else if(currPlayOffset>0){
			if(lpHwSndStream->nextBlock==1){
				writeOffset=lpHwSndStream->nextBlock*lpHwSndStream->blockSize;
				writeFlag=TRUE;	
			}
		}
	}

	if(writeFlag){	
		if(lpHwSndStream->state==HWSTR_STOP){
			HwSndStreamStop(lpss);
		}
		else {
			hr=sndBuf->lpVtbl->Lock(sndBuf, writeOffset, lpHwSndStream->blockSize,
									&pMem1, &dwSize1, &pMem2, &dwSize2, 0);
			if (!FAILED(hr)) {
				// i am sure that i don't go around the end of ds buffer
				DWORD sizeToRead;
				// we reach the end of wav sample in the file
				if (lpHwSndStream->dataWritten+lpHwSndStream->blockSize >
					lpHwSndStream->dwDataLength)
					sizeToRead=lpHwSndStream->dwDataLength-lpHwSndStream->dataWritten;
				else
					sizeToRead=lpHwSndStream->blockSize;
				wReadSize=lpHwSndStream->fread(pMem1,1, sizeToRead, lpHwSndStream->pf );
				lpHwSndStream->dataWritten+=wReadSize;
				// we check again if reach the end of file
				if (wReadSize<lpHwSndStream->blockSize){ 
					if (lpHwSndStream->propPlay & SNDPROP_LOOP) {
						lpHwSndStream->fseek(lpHwSndStream->pf, sizeof(_WAVHEADER),SEEK_SET);
						wReadSize=lpHwSndStream->fread((BYTE *)pMem1+wReadSize, 1,
							lpHwSndStream->blockSize-wReadSize,lpHwSndStream->pf );
						lpHwSndStream->dataWritten=wReadSize;
					}
					else{
						FillMemory((BYTE *)pMem1+wReadSize, lpHwSndStream->blockSize-wReadSize, 
							SNDGETSILENCE(lpso->base.fmt.wBitsPerSample)); //128
						// prepare to stop it
						lpHwSndStream->state=HWSTR_STOP;
					}
				}
				hr=sndBuf->lpVtbl->Unlock(sndBuf, pMem1, dwSize1, pMem2, dwSize2);		
				if (FAILED(hr))
					DEBUGMYERROR(DSTAB,hr);
			}
			else {
				if(hr==DSERR_BUFFERLOST)
					lpso->base.reload=TRUE;					
				else
					DEBUGMYERROR(DSTAB,hr);
			}
		}

		lpHwSndStream->nextBlock++;
		if (lpHwSndStream->nextBlock>=lpHwSndStream->nBlock)
			lpHwSndStream->nextBlock=0;
	}
	return TRUE;
}

BOOL HwSndStreamClose(LPSNDSTREAM lpss)
{
	BOOL bFlag=FALSE;
	LPHWSNDSTREAM lpHwSndStream;
	
	lpHwSndStream=&lpss->strData;
	switch(lpHwSndStream->whereType){
	case HWSTR_MEM:
		bFlag=(Memfclose(lpHwSndStream->pf)==0);
		break;
	case HWSTR_FILE:
		bFlag=(fclose((FILE*)lpHwSndStream->pf)==0);
		break;
	case HWSTR_BLOCK:
		bFlag=(Pkfclose(lpHwSndStream->pf)==0);
		bFlag=(bFlag &&PkPackClose(lpHwSndStream->lpPf));
		break;
		
	}		
	lpHwSndStream->pf=NULL;
	return bFlag;
}

BOOL HwSndStreamStop(LPSNDSTREAM lpss)
{
	LPHWSNDSTREAM lpHwSndStream;
	
	lpHwSndStream=&lpss->strData;
	if(lpHwSndStream->state!=HWSTR_READY) {
		HwSndStreamClose(lpss);
		lpHwSndStream->state=HWSTR_READY;// I stop the stream
	}
	return HwSndObjStop(lpss->lpHwSo); //I stop the current buffer
}
