// XAudioSystem.cpp: implementation of the _XAudioSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
/*
#include "XAudioSystem.h"
#include "EmperorOfDragons.h"

using namespace Audio;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XAudioSystem::_XAudioSystem()
{
	m_pListener = NULL;
}

_XAudioSystem::~_XAudioSystem()
{

}


BOOL _XAudioSystem::Initialize( void )
{
	AudioMgrInit audiomgrinit;
	audiomgrinit.m_hWnd = gHWnd;
	audiomgrinit.m_bLoadAsyncronously = false;
	audiomgrinit.m_bForceSoftware = true;
	audiomgrinit.m_bUseMusicReverb = false;
	audiomgrinit.m_bUseEAX = false;
	audiomgrinit.m_bUseZoomFX = false;
	audiomgrinit.m_bAutoStream = false;
	audiomgrinit.m_bCacheBuffers = false;
	audiomgrinit.m_nSegmentMax = 256;
	audiomgrinit.m_n3DSoftwareBufferMax = 256;
	audiomgrinit.m_n3DHardwareBufferMin = 8;
	audiomgrinit.m_n3DHardwareBufferMax = 256;
	audiomgrinit.m_n2DSoftwareBufferMax = 256;
	audiomgrinit.m_n2DHardwareBufferMin = 8;
	audiomgrinit.m_n2DHardwareBufferMax = 256;
		
	audiomgrinit.m_nOptimalSampleBits = 16;
	audiomgrinit.m_nOptimalSampleRate = 44100;
		
	audiomgrinit.m_pMusicCallback = MusicMgr();
	
	if(!AudioMgr()->Init(audiomgrinit))
		_XFatalError("Initializing audio system");
		
	if(!AudioMgr()->GetListener(m_pListener))
		_XFatalError("Getting listener object");
	
	ListenerInit listenerinit;

	//listenerinit.m_Prop.m_fDistanceFactor = 0.01f;

	if( m_pListener )
	{
		if(!m_pListener->Init(listenerinit))
			_XFatalError("Initializing listener object");
	}
	else
	{
		_XFatalError("Initializing listener object::Getting listener object");
	}
	
	if(!SoundMgr()->Init())
		_XFatalError("Initializing sound manager");
	
	if(!MusicMgr()->Init())
		_XFatalError("initializing music manager");
		
	return TRUE;
}

void _XAudioSystem::Release( void )
{		
	AudioMgr()->Term();

	Sleep( 1500 );
}

void _XAudioSystem::OnResumeall() 
{
	AudioMgr()->ResumeAll();
}

void _XAudioSystem::OnPauseall() 
{
	AudioMgr()->PauseAll();
}

void _XAudioSystem::OnStopall() 
{
	AudioMgr()->StopAll();
}

*/