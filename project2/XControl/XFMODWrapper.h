// XFMODWrapper.h: interface for the _XFMODWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFMODWRAPPER_H__24ECAEE6_62A9_4E8C_92CD_3736ADDEB168__INCLUDED_)
#define AFX_XFMODWRAPPER_H__24ECAEE6_62A9_4E8C_92CD_3736ADDEB168__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#include "fmod.h"
#include "fmod_errors.h"    // optional

#include <map>
#include <string>
#include <vector>
#include <functional>

typedef enum _XSOUNDFADEMODE
{
	_XSOUNDFADEMODE_NONE = 0,
	_XSOUNDFADEMODE_FADEIN,
	_XSOUNDFADEMODE_FADEOUT
};

typedef struct _XFMODSetup
{
	int					SoundOutputMethod;
	int					SoundDriver;
	int					SoundMixrate;
	int					MaxSoftWareChannels;
	DWORD				Flags;
	DWORD				SoundBufferSize;
	DWORD				StreamingBufferSize;
	FSOUND_MIXERTYPES	MixerTypes;
} _XFMODSETUP;

typedef struct _XPackedFileHandle
{
	int   fileid;
	DWORD fileoffset;
	DWORD filesize;
	LONG  currentposition;
} _XPACKEDFILEHANDLE, *_LPXPACKEDFILEHANDLE;

typedef	enum _XSOUNDTYPE
{
	_XSOUNDTYPE_2DSOUND = 0,
	_XSOUNDTYPE_3DSOUND
};

#define _XDEF_FLSOUNDFADEMODE_IDLE		0
#define _XDEF_FLSOUNDFADEMODE_FADEIN	1
#define _XDEF_FLSOUNDFADEMODE_FADEOUT	2

typedef struct _XSoundModule
{
	FSOUND_SAMPLE  *psoundobject;
	TCHAR			filename[32];
	DWORD			modeflag;
	FLOAT			mindistance;
	FLOAT			maxdistance;
	int				playreferencecounter;
	int				channel;	
	DWORD			lastusedtime;
	BYTE			fademode;
} _XSOUNDMODULE, *_LPXSOUNDMODULE;

typedef std::vector<_LPXSOUNDMODULE>	svdef_XSOUNDMODULE;


typedef struct _XStreamModule
{
	FSOUND_STREAM  *pstreamobject;
	TCHAR			filename[32];
	DWORD			modeflag;	
	int				channel;
	BOOL			play;
	_XSOUNDFADEMODE fademode;
	LONG			fadestarttimer;	
	FLOAT			currentvolume;
	FLOAT			speed;
	DWORD			lastusedtime;
} _XSTREAMMODULE, *_LPXSTREAMMODULE;

typedef std::vector<_LPXSTREAMMODULE>	svdef_XSTREAMMODULE;

class _XFMODWrapper  
{
private:
	// Configurator
	_XFMODSETUP			m_SetupParameter;	
	
	FLOAT				m_3DMinDistance;
	FLOAT				m_3DMaxDistance;

	D3DXVECTOR3			m_LastListenerPosition;
	D3DXVECTOR3			m_CurentListenerPosition;

	D3DXVECTOR3			m_ListnerForwardOrientVector;
	D3DXVECTOR3			m_ListnerTopOrientVector;


	BYTE				m_SoundVolume;
	svdef_XSOUNDMODULE	m_svSoundModuleList;
	
	BYTE				m_FLSoundVolume;
	svdef_XSOUNDMODULE	m_svSoundModuleFlexibleList;
	int					m_FLSoundProcessTimer;

	BYTE				m_StreamVolume;
	svdef_XSTREAMMODULE m_svStreamModuleList;		

	int					m_2DSoundMaxInstanceCount;
	int					m_SoundMaxInstanceCount;
	int					m_SoundFlxMaxInstanceCount;
	int					m_StreanMaxInstanceCount;

	DWORD				m_LastHitrateCheckTime;


public:
	BOOL				m_ChangeBGM;
	BOOL				m_StopedBGM;
	TCHAR				m_CurrentBGMName[64];
	TCHAR				m_NextBGMName[64];
	FSOUND_STREAM*		m_pBGMStreamObject;
	int					m_BGMChannel;
	FLOAT				m_BGMVolume;
	FLOAT				m_BGMMaxVolume;

	int					m_LoadedSoundSampleCount;
	int					m_LoadedFLSoundSampleCount;

public:
	_XFMODWrapper();
	virtual ~_XFMODWrapper();

	void				LoadSettings( void );

	BOOL				Initialize( void );
	void				Release( void );

	void				SetListenerAttribute( D3DXVECTOR3& position, D3DXVECTOR3& forwardvector, D3DXVECTOR3& topvector );

	int					FindSound( LPCTSTR szfilename );
	int					InsertSound( LPCTSTR szfilename, DWORD mode );
	int					InsertSound( LPCTSTR szfilename, DWORD mode, FLOAT mindist, FLOAT maxdist );

	void				UpdateReferenceCount( _LPXSOUNDMODULE pSoundModule );
	void				PlaySound2D( int index );
	void				PlaySound3D( int index, D3DXVECTOR3& position, bool loop );
	void				StopSound( int index );
	void				StopAllSound( void );
	_LPXSOUNDMODULE		GetSoundObject( int index );
	void				SetVolume( BYTE volume );
	BYTE				GetVolume( void );

	int					FL_FindSound( LPCTSTR szfilename );
	int					FL_InsertSound( LPCTSTR szfilename, DWORD mode );
	int					FL_InsertSound( LPCTSTR szfilename, DWORD mode, FLOAT mindist, FLOAT maxdist );
	void				FL_PlaySound2D( int index );
	void				FL_PlaySound3D( int index, D3DXVECTOR3& position, bool loop );
	void				FL_FadeinSound( int index );
	void				FL_StopSound( int index );
	void				FL_StopAllSound( void );
	void				FL_ReleaseFlexibleList( void );
	_LPXSOUNDMODULE		FL_GetSoundObject( int index );
	void				FL_SetVolume( BYTE volume );
	BYTE				FL_GetVolume( void );
	void				FL_ProcessFLSound( void );

	int					FindStream( LPCTSTR szfilename );
	int					InsertStream( LPCTSTR szfilename, DWORD mode, FLOAT fadeoutspeed = 10.0f );	
	void				PlayStream( int index, bool loop );
	void				DeleteStream( LPCTSTR szfilename );
	void				DeleteStream( int index );
	void				StopStream( int index );
	void				StopAllStream( void );
	void				ProcessStream( void );
	_LPXSTREAMMODULE	FL_GetStreamObject( int index );	
	void				SetStreamVolume( BYTE volume );
	BYTE				GetStreamVolume( void );
	
	void				SetFadeMode( LPCTSTR szfilename, _XSOUNDFADEMODE fademode, FLOAT speed = 30.0f );
	void				SetFadeMode( int index, _XSOUNDFADEMODE fademode, FLOAT speed = 30.0f );

	_XSOUNDFADEMODE		GetFadeMode( LPCTSTR szfilename );
	_XSOUNDFADEMODE		GetFadeMode( int index );	

	void				PlayBGM( LPCTSTR szfilename, BOOL loop = FALSE );
	void				StopBGM( void );
	BOOL				IsBGMPlaying( void );
	void				ProcessBGM( void );
	void				SetBGMMaxVolume( BYTE volume );
	BYTE				GetBGMMaxVolume( void ){ (BYTE)m_BGMMaxVolume; }

	void				CheckHitrate( void );

	void				FMODUpdate( void )
	{
						FL_ProcessFLSound();
						FSOUND_Update();
						CheckHitrate();
	}	
	
	void				SetBGMLoopPoints( unsigned int loopstart, unsigned int loopend );
};

extern _XFMODWrapper	g_FMODWrapper;

#endif // !defined(AFX_XFMODWRAPPER_H__24ECAEE6_62A9_4E8C_92CD_3736ADDEB168__INCLUDED_)
