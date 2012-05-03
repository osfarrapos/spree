// XDSound.h: interface for the _XDSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDSOUND_H__3CAA5F41_4F9B_11D3_9DC0_BB01789C672E__INCLUDED_)
#define AFX_XDSOUND_H__3CAA5F41_4F9B_11D3_9DC0_BB01789C672E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

#include "XItem.h"
#include "XList.h"

BOOL	InitDirectsound( void );
BOOL	DSoundRelease( void );
extern  LPDIRECTSOUND         g_pDSound;

typedef struct
{
	WORD         Indicator;
	WAVEFORMATEX wfe;	
	DWORD        soundSize;
	char         Wavename[21];

}_XWAVE_FORMAT;

typedef struct
{
	WORD Indicator;
	int  Wavecount;
	char Listname[11];
}_XWAVELIST_FORMAT;



class _XWave : public item 
{
	  private :
		  _XWAVE_FORMAT WF;
		  		  
		  LPDIRECTSOUNDBUFFER lpDSBuffer;
	

      public :
		  _XWave( LPSTR Filename );
		  _XWave();
		  ~_XWave();
			
		  BOOL loadWave(  LPSTR Filename );
		  BOOL loadXWave( LPSTR Filename );
		  BOOL loadXWave( FILE* Fileptr  );

		  BOOL saveXWave( LPSTR Filename );
		  BOOL saveXWave( FILE* Fileptr  );

		  void  setPanning( long Value ){ lpDSBuffer->SetPan( Value );    }
		  void  setVolume ( long Value ){ lpDSBuffer->SetVolume( Value ); }

		  void  setName( LPSTR Name )
		  { 
			  memset( WF.Wavename, 0, 21 );
			  strncpy( WF.Wavename, Name, 20 );
		  }
		  LPSTR getName( void ){ return WF.Wavename; }

		  void Play( void );
		  void Stop( void );
};

class _XDSoundlist  : public list
{
	private :
		int Wavecount;
	public:
		_XDSoundlist();
	    ~_XDSoundlist();

		BOOL loadXWavelist( LPSTR Filename );
		BOOL saveXWavelist( LPSTR Filename );		

		void disposeWavelist( void );
		BOOL deleteWave( int Position );

		BOOL insertWave( LPSTR Filename );
		BOOL insertWave( _XWave* wave );

		_XWave* getWaveobject( int Position );
		int     getWavecount( void ){ return Wavecount; }
		
		const   _XWave *operator[](int Position);

		void Play( int Position );
		void Play( void );
		void Stop( int Position );
		void Stop( void );
};

#endif // !defined(AFX_XDSOUND_H__3CAA5F41_4F9B_11D3_9DC0_BB01789C672E__INCLUDED_)
