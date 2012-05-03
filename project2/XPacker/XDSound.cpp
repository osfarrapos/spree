// XDSound.cpp: implementation of the _XDSoundlist class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
/*
#include "XDefine.h"
#include "XExtDef.h"
#include "XDX.h"
#include "XUtils.h"
*/
#include "XDSound.h"

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define _HEADER_XWAVE         0xFEE8
#define _HEADER_XLWAVE        0xFEE9

LPDIRECTSOUND         g_pDSound = NULL;

BOOL InitDirectsound( void )
{

    HRESULT result = DirectSoundCreate(NULL, &g_pDSound, NULL);
    if (result != DS_OK) 
	{
		AfxMessageBox( "_XDX::InitDirectsound : Directsound creating failed !");
		return FALSE;
	}

    result = g_pDSound->SetCooperativeLevel( AfxGetMainWnd()->m_hWnd, DSSCL_NORMAL);
    if (result != DS_OK)
	{
		AfxMessageBox( "_XDX::InitDirectsound : Cooperative Level setting failed !");
		return FALSE;
	}

	return TRUE;
}

BOOL DSoundRelease( void )
{	
	if( g_pDSound )
	{
		g_pDSound->Release();	
		//delete g_pDSound;
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XDSoundlist::_XDSoundlist()
{
	Wavecount = 0;	
}

_XDSoundlist::~_XDSoundlist()
{
	
}

void _XDSoundlist::disposeWavelist( void )
{
	 Stop(); 
	 disposeList();
	 Wavecount = 0;		 	 
}

BOOL _XDSoundlist::deleteWave( int Position )
{
	if( (Wavecount < 1) || Position < 0 || Position > Wavecount-1 ) return FALSE;
    
	Stop();
	
	resetList();
	while( Position-- ) nextItem();	
    
	if( deleteItem( currentItem() ) ) Wavecount--;

	return TRUE;
}

BOOL _XDSoundlist::insertWave( LPSTR Filename )
{
	 _XWave* wave = new _XWave;
	 if( wave->loadWave( Filename ) )
	 {
		 resetList();
		 insertItem( (item*) wave );
		 Wavecount++;		 
		 return TRUE;
	 }
	 else
	 {
		 delete wave;
		 return FALSE;
	 }
}

BOOL _XDSoundlist::insertWave( _XWave* wave )
{	 
	 if( wave )
	 {
		 resetList();
		 insertItem( (item*) wave );
		 Wavecount++;
		 return TRUE;
	 }
	 else return FALSE; 
}

_XWave* _XDSoundlist::getWaveobject( int Position )
{
	 if( (Wavecount < 1) || Position < 0 || Position > Wavecount-1 ) return NULL;
	 resetList();
	 
	 while( Position-- ) nextItem();
	 
	 return (_XWave*)( currentItem() );
}

const _XWave *_XDSoundlist::operator[]( int Position )
{
	 if( (Wavecount < 1) || Position < 0 || Position > Wavecount-1 ) return NULL;
	 resetList();
	 if( Position == 0 ) return (_XWave*)( currentItem() );
	 while( Position-- ) nextItem();
	 
	 return (_XWave*)currentItem();
}


void _XDSoundlist::Play( int Position )
{
	 if( (Wavecount < 1) || Position < 0 || Position > Wavecount-1 ) return;
	 	  
	 resetList();	 
	 while( Position-- ) nextItem();
	 if( (_XWave*)currentItem() )
	 {		 
		 ( (_XWave*)(currentItem()) )->Play();
	 }
}

void _XDSoundlist::Play( void )
{
	 if( !Wavecount ) return;
	 resetList();
	 do
	 {
		 ( (_XWave*)currentItem() )->Play();
		 nextItem();  	 
	 }while( !atHeadOfList() );
}

void _XDSoundlist::Stop( int Position )
{
	 if( (Wavecount < 1) || Position < 0 || Position > Wavecount-1 ) return;
	 resetList();
	 while( Position-- ) nextItem();  
	 ( (_XWave*)currentItem() )->Stop();
}

void _XDSoundlist::Stop( void )
{
	 if( !Wavecount ) return;
	 resetList();
	 do
	 {		 
		 ( (_XWave*)currentItem() )->Stop();
		 nextItem();  
	 }while( !atHeadOfList() );
}


BOOL _XDSoundlist::loadXWavelist( LPSTR Filename )
{	 
	 Stop();

	 FILE* Fileptr = NULL;
	 Fileptr = fopen( Filename, "rb" );
	 if( !Fileptr ) return FALSE;
	 
	 _XWAVELIST_FORMAT WLHeader;
	 
	 fread( &WLHeader, sizeof( _XWAVELIST_FORMAT ), 1, Fileptr );
	 if( WLHeader.Indicator != _HEADER_XLWAVE )
	 {
		 fclose( Fileptr );
		 return FALSE;
	 }

	 disposeWavelist();
	 
	 _XWave* twave = NULL;
	 BOOL Check = TRUE;
	 
	 while( WLHeader.Wavecount-- )
	 {	
		 twave = new _XWave;
		 if( twave->loadXWave( Fileptr ) ) 
		 {
			 insertWave( twave );
		 }
		 else 
		 {				 
			 disposeWavelist();
			 Check = FALSE;
			 break;
		 }
	 }

	 fclose( Fileptr );	 

	 return Check;
}

BOOL _XDSoundlist::saveXWavelist( LPSTR Filename )
{
	if( !Filename[0] || !Wavecount )
	{		
		return FALSE;
	}
	
	Stop();

/*	_XString* String = new _XString( Filename, 0 );
	LPSTR tStr = getExtention( String->Get_String() );
	 
	if( stricmp( tStr, ".XWL" ) != 0 )
	{		
		String->deleteExtention(); 
		String->Add(".XWL");
	}
*/
	_XWAVELIST_FORMAT WLHeader;
	WLHeader.Indicator = _HEADER_XLWAVE;
	WLHeader.Wavecount = Wavecount;

	FILE* Fileptr = NULL;
	Fileptr = fopen( Filename, "wb" );	
	/*delete String;*/

	if( !Fileptr )
	{		
		return FALSE;
	}
	
	fwrite( &WLHeader, sizeof( _XWAVELIST_FORMAT ), 1, Fileptr );
	
	resetList();

	int i = Wavecount;
	BOOL Result = FALSE;

	while( i-- )
	{
		Result = (((_XWave*)currentItem())->saveXWave( Fileptr ) );
		if( !Result )
		{ 
			break; 
		}
		nextItem();
	}
	
	fclose( Fileptr );
	return Result;
}

//---------------------------------------------------------------------------------------------

_XWave::_XWave( LPSTR Filename )
{
	  lpDSBuffer = NULL;
	  WF.Indicator = NULL;
	  WF.soundSize = 0;
	  memset( &(WF.wfe), 0, sizeof( WAVEFORMATEX ) );
	  memset( WF.Wavename, 0, 11 ); 

	  loadWave( Filename );		
}

_XWave::_XWave()
{
	  lpDSBuffer = NULL;	
	  memset( &WF, 0, sizeof( WF ) );
}


_XWave::~_XWave()
{
	if (  lpDSBuffer )
	{
		Stop();
		lpDSBuffer->Release();
		lpDSBuffer = NULL;
	}
}

void _XWave::Play( void )
{		 
	 if( g_pDSound && lpDSBuffer )
	 {
		 this->lpDSBuffer->Stop();
		 this->lpDSBuffer->SetCurrentPosition(0);
		 this->lpDSBuffer->Play(0, 0, 0);
	 }
}

void _XWave::Stop( void )
{
	 if( g_pDSound && lpDSBuffer )
	 {		 
		 lpDSBuffer->Stop();
	 }
}

BOOL _XWave::loadXWave( LPSTR Filename )
{
	if( !Filename[0] || !g_pDSound ) return FALSE;
	
	FILE* Fileptr = fopen( Filename, "rb");
		
	fread( &WF, sizeof( _XWAVE_FORMAT ), 1, Fileptr );
    	
	if( WF.Indicator != _HEADER_XWAVE ) 
	{
		fclose( Fileptr );
		return FALSE;
	}
	
	if( lpDSBuffer )
	{
		lpDSBuffer->Release();
		lpDSBuffer = NULL;
	}

	DSBUFFERDESC dsbd;
    
	LPVOID pBlock1;    
    LPVOID pBlock2;
	
	DWORD bytesBlock1;
    DWORD bytesBlock2;

    memset(&dsbd, 0, sizeof(DSBUFFERDESC));
    dsbd.dwSize = sizeof(DSBUFFERDESC);
    dsbd.dwBufferBytes = WF.soundSize;
    dsbd.lpwfxFormat = (LPWAVEFORMATEX) &(WF.wfe);
    dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;

	HRESULT result = g_pDSound->CreateSoundBuffer(&dsbd,	&lpDSBuffer, NULL);
    if (result != DS_OK)
	{		
		fclose( Fileptr );
		return FALSE;
	}
	
    result = lpDSBuffer->Lock(0, WF.soundSize, &pBlock1, &bytesBlock1, &pBlock2, &bytesBlock2, 0);
    if (result != DS_OK)
	{
		fclose( Fileptr );
		return FALSE;
	}

    fread( (void*)pBlock1, WF.soundSize, 1, Fileptr );

    lpDSBuffer->Unlock(pBlock1, bytesBlock1, pBlock2, bytesBlock2);
		
	fclose( Fileptr );

	return TRUE;
}

BOOL _XWave::loadXWave( FILE* Fileptr )
{	
	if( !Fileptr || !g_pDSound ) return FALSE;
				
	fread( &WF, sizeof( _XWAVE_FORMAT ), 1, Fileptr );
    	
	if( WF.Indicator != _HEADER_XWAVE ) 
	{		
		return FALSE;
	}

	SAFE_RELEASE( lpDSBuffer );
	
	DSBUFFERDESC dsbd;
    
	LPVOID pBlock1;    
    LPVOID pBlock2;
	
	DWORD bytesBlock1;
    DWORD bytesBlock2;

    memset(&dsbd, 0, sizeof(DSBUFFERDESC));
    dsbd.dwSize = sizeof(DSBUFFERDESC);
    dsbd.dwBufferBytes = WF.soundSize;
    dsbd.lpwfxFormat = (LPWAVEFORMATEX) &(WF.wfe);
    dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;

	HRESULT result = g_pDSound->CreateSoundBuffer(&dsbd,	&lpDSBuffer, NULL);
    if (result != DS_OK)
	{		
		return FALSE;
	}
	
    result = lpDSBuffer->Lock(0, WF.soundSize, &pBlock1, &bytesBlock1, &pBlock2, &bytesBlock2, 0);
    if (result != DS_OK)
	{	
		return FALSE;
	}

    fread( (void*)pBlock1, WF.soundSize, 1, Fileptr );

    lpDSBuffer->Unlock(pBlock1, bytesBlock1, pBlock2, bytesBlock2);
		
	return TRUE;
}


BOOL _XWave::loadWave( LPSTR Filename )
{
	
	if( !Filename[0] || !g_pDSound ) return FALSE;

	MMCKINFO mmCkiRiff;
    MMCKINFO mmCkiChunk;

	ZeroMemory( &mmCkiRiff, sizeof( MMCKINFO ) );
	ZeroMemory( &mmCkiChunk, sizeof( MMCKINFO ) );

    HMMIO hMMIO = mmioOpen((char*)Filename, NULL, MMIO_READ | MMIO_ALLOCBUF);
    if (hMMIO == NULL)
	{
		CString tMessage;
		tMessage.Format( "[ %s ] file openning error !", Filename );
		AfxMessageBox( tMessage );
		return FALSE;
	}

    mmCkiRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    MMRESULT mmResult = mmioDescend(hMMIO, &mmCkiRiff, NULL, MMIO_FINDRIFF);
    if (mmResult != MMSYSERR_NOERROR)
	{
		
		AfxMessageBox( "[ wave ] mmioDescend Error !" );
		return FALSE;
	}

	mmCkiChunk.fccType = mmioFOURCC('f', 'm', 't', ' ');
    mmResult = mmioDescend(hMMIO, &mmCkiChunk, &mmCkiRiff, NULL );
	
    if (mmResult != MMSYSERR_NOERROR)
	{
		AfxMessageBox( "[ fmt ] mmioDescend Error !" );
		return FALSE;
	}

    LONG numBytes = mmioRead(hMMIO, (char*)&(WF.wfe), sizeof(WAVEFORMATEX));
    if (numBytes == -1)
	{
		AfxMessageBox( "[ Header ] mmioRead Error !" );
		return FALSE;
	}
	if (WF.wfe.wFormatTag != WAVE_FORMAT_PCM )
	{
		AfxMessageBox( "Not PCM format !" );
		return FALSE;
	}

    mmResult = mmioAscend(hMMIO, &mmCkiChunk, 0);
    if (mmResult != MMSYSERR_NOERROR)
	{
		AfxMessageBox( "mmioAscend Error !" );
		return FALSE;
	}

    mmCkiChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
    mmResult = mmioDescend(hMMIO, &mmCkiChunk, &mmCkiRiff, MMIO_FINDCHUNK);
    if (mmResult != MMSYSERR_NOERROR)
	{
		AfxMessageBox( "[ data ] mmioDecend Error !" );
		return FALSE;
	}

    WF.soundSize = mmCkiChunk.cksize;
    		
	SAFE_RELEASE( lpDSBuffer );
		
	DSBUFFERDESC dsbd;    
	LPVOID pBlock1;    
    LPVOID pBlock2;	
	DWORD bytesBlock1;
    DWORD bytesBlock2;

    memset(&dsbd, 0, sizeof(DSBUFFERDESC));
    dsbd.dwSize = sizeof(DSBUFFERDESC);
    dsbd.dwBufferBytes = WF.soundSize;
    dsbd.lpwfxFormat = (LPWAVEFORMATEX) &(WF.wfe);
    dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;	               

    HRESULT result = g_pDSound->CreateSoundBuffer(&dsbd,	&lpDSBuffer, NULL);
    if (result != DS_OK)
	{		
		AfxMessageBox( "Create sound buffer Error !" );
		return FALSE;
	}
	
    result = lpDSBuffer->Lock(0, WF.soundSize, &pBlock1, &bytesBlock1, &pBlock2, &bytesBlock2, 0);
    if (result != DS_OK)
	{		
		AfxMessageBox( "Sound buffer lock failed !" );
		return FALSE;
	}
	
	if( pBlock1 )
	{
		numBytes = mmioRead(hMMIO, (char*)pBlock1, bytesBlock1 );
		if (numBytes == -1)
		{				
			lpDSBuffer->Unlock(pBlock1, bytesBlock1,	pBlock2, bytesBlock2);
			AfxMessageBox( "First block loading error !" );
			return FALSE;
		}
	}
    if( pBlock2 )
	{
		numBytes = mmioRead(hMMIO, (char*)pBlock2, bytesBlock2 );
		if (numBytes == -1)
		{		
			lpDSBuffer->Unlock(pBlock1, bytesBlock1,	pBlock2, bytesBlock2);
			AfxMessageBox( "Second block loading error !" );
			return FALSE;
		}
	}

    lpDSBuffer->Unlock(pBlock1, bytesBlock1,	pBlock2, bytesBlock2);
	mmioClose(hMMIO, 0);	

	return TRUE;
}


BOOL _XWave::saveXWave( LPSTR Filename )
{
	 if( !Filename[0] || !lpDSBuffer ) return FALSE;
	 
	 LPVOID pBlock1;    
     LPVOID pBlock2;	
	 DWORD bytesBlock1;
     DWORD bytesBlock2;

	 HRESULT result = lpDSBuffer->Lock(0, WF.soundSize, &pBlock1, &bytesBlock1, &pBlock2, &bytesBlock2, 0);
     if (result != DS_OK)
	 {		
		return FALSE;
	 }

/*   	 _XString* String = new _XString( Filename, 0 );
	 LPSTR tStr = getExtention( String->Get_String() );
	 
	 if( stricmp( tStr, ".XWV" ) != 0 )
	 {		
		String->deleteExtention(); 
		String->Add(".XWV");
	 }
*/
	 FILE* Fileptr = fopen( Filename, "wb");
	 //delete String;
	 
	 WF.Indicator = _HEADER_XWAVE;

	 fwrite( &WF, sizeof( _XWAVE_FORMAT ), 1, Fileptr );
     fwrite( pBlock1, WF.soundSize, 1, Fileptr ); 
	 fclose( Fileptr );

	 lpDSBuffer->Unlock(pBlock1, bytesBlock1,	pBlock2, bytesBlock2);

	 return TRUE;
}

BOOL _XWave::saveXWave( FILE* Fileptr  )
{
	 if( !Fileptr || !lpDSBuffer )
	 {		 
		 return FALSE;
	 }
	 
	 LPVOID pBlock1;
     LPVOID pBlock2;
	 DWORD  bytesBlock1;
     DWORD  bytesBlock2;

	 HRESULT result = lpDSBuffer->Lock(0, WF.soundSize, &pBlock1, &bytesBlock1, &pBlock2, &bytesBlock2, 0);
     if (result != DS_OK)
	 {			 
		 return FALSE;
	 }
     	 	 
	 WF.Indicator = _HEADER_XWAVE;

	 fwrite( &WF, sizeof( _XWAVE_FORMAT ), 1, Fileptr );
     fwrite( pBlock1, WF.soundSize, 1, Fileptr );
	 
	 lpDSBuffer->Unlock(pBlock1, bytesBlock1,	pBlock2, bytesBlock2);

	 return TRUE;
}

