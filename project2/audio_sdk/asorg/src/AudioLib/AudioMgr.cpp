/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "AudioMgr.h"
#include "AudioCommon.h"
#include "Sound.h"
#include "Sound3D.h"
#include "Segment.h"
#include "DLS.h"
#include "AudioScript.h"
#include "Listener.h"
#include "AudioStreamFactory.h"


const int RESERVE_SOUND		= 128;
const int RESERVE_SOUND3D	= 128;
const int RESERVE_SEGMENT	= 32;
const int RESERVE_SCRIPT	= 16;
const int RESERVE_DLS		= 4;

const int TIME_EVENT = 0;
const int MUSIC_EVENT = 1;

using namespace std;
using namespace Audio;

#define CHECK_INIT() if(!IsInitialized())	return Error::Handle("The audio manager has not been properly initialized yet");


//------------------------------------------------------------------------//
AudioManager::AudioManager()
{
	FN("AudioManager::AudioManager()");
	Clear();
}

//------------------------------------------------------------------------//
AudioManager::~AudioManager()
{
	FN("AudioManager::~AudioManager()");
	Term();
}

//------------------------------------------------------------------------//
void AudioManager::Clear()
{
	FN("AudioManager::Clear()");
	m_pLoader = 0;
	m_pMusicPerformance = 0;
	m_pSoundPerformance = 0;
	m_pDirectSound = 0;
	m_pPrimaryBuffer = 0;
	memset(&m_DSCaps, 0, sizeof(DSCAPS));
	m_DSCaps.dwSize = sizeof(DSCAPS);
	m_bInitialized = false;
	m_Init.Clear();

	// Path information
	m_pszAudioSystemPath[0] = 0;
	m_pszCurrentWorkingPath[0] = 0;

	// Buffer usage members
	m_Stats.Clear();

	// Volume info
	m_fSoundVolume = VOLUME_MAX;
	m_fMusicVolume = VOLUME_MAX;

	// Currently loaded objects
	m_LoadedSound.clear();
	m_LoadedSound3D.clear();
	m_LoadedSegment.clear();

	// Clear the buffer cache
	m_BufferCache.Clear();

	// Asyncronous load pending lists
	CLEAR_STL_QUEUE(m_SoundLoadPending);
	CLEAR_STL_QUEUE(m_Sound3DLoadPending);
	CLEAR_STL_QUEUE(m_SegmentLoadPending);
	CLEAR_STL_QUEUE(m_SoundLoadTemp);
	CLEAR_STL_QUEUE(m_Sound3DLoadTemp);
	CLEAR_STL_QUEUE(m_SegmentLoadTemp);

	// handle for load notification
	m_hLoadNotify = 0;

	// Stream data
	m_SoundStreamProcess.clear();
	m_SoundStreamRemoval.clear();

	// Listener object
	m_pListener = 0;

	// Music data
	m_pCurrentSegment = 0;
	m_pNextSegment = 0;

	// Music update notification
	m_hMusicNotify = 0;

	// Shut-down synchronization handles
	m_hTerm[TIME_EVENT] = 0;
	m_hTerm[MUSIC_EVENT] = 0;

	// Update thread synchronization critical-section object
	ZeroMemory(&m_csAudioUpdate, sizeof(CRITICAL_SECTION));
	ZeroMemory(&m_csLoading, sizeof(CRITICAL_SECTION));
	ZeroMemory(&m_csLoadScheduling, sizeof(CRITICAL_SECTION));

	// Audio stream factory information
	m_pStreamFactory = 0;

}

//------------------------------------------------------------------------//
bool AudioManager::Init(const AudioMgrInit& init)
{
	FN("AudioManager::Init()");

	Error::Log("Initializing audio system...");
	HRESULT hr;

	// Make sure we don't initialize more than once
	if(IsInitialized())
		return Error::Handle("Audio system has already been initialized.");
	m_Init = init;

	hr = CoInitialize(0);
	if(FAILED(hr))
		return Error::Handle("Could not initialize COM");

	// Initialize the buffer cache
	m_BufferCache.Init();

	if(m_Init.m_bCacheBuffers && !m_Init.m_bAutoStream)
	{
		DebugOut(1, "WARNING: Cannot enable buffer caching without auto streaming");
		DebugOut(1, "Enabling autostreaming now...");
		m_Init.m_bAutoStream = true;
	}

	// Pre-allocate audio objects
	Sound::ReservePool(RESERVE_SOUND);
	Sound3D::ReservePool(RESERVE_SOUND3D);
	Segment::ReservePool(RESERVE_SEGMENT);
	AudioScript::ReservePool(RESERVE_SCRIPT);
	DLS::ReservePool(RESERVE_DLS);

	// First create the DirectSound object and set the cooperative level
	hr = DirectSoundCreate8(NULL, &m_pDirectSound, NULL);
	if(FAILED(hr))
		return Error::Handle("Could not create DirectSound8 object.  Error = %s.", DXGetErrorString(hr));
	hr = m_pDirectSound->SetCooperativeLevel(init.m_hWnd, DSSCL_PRIORITY);
	if(FAILED(hr))
		return Error::Handle("Failed to set DirectSound cooperative level.  Error = %s.", DXGetErrorString(hr));

	// Get the capabilities of this sound system
	hr = m_pDirectSound->GetCaps(&m_DSCaps);
	if(FAILED(hr))
		return Error::Handle("Failed to get DirectSound caps.  Error = %s.", DXGetErrorString(hr));

	Error::Log("Reporting sound driver caps...");
	Error::Log("\tSound Driver %s certified or is a WDM driver.", 
		(m_DSCaps.dwFlags & DSCAPS_CERTIFIED) ? "is" : "is not");
	Error::Log("\tSound Driver %s support variable sample rate playback.", 
		(m_DSCaps.dwFlags & DSCAPS_CONTINUOUSRATE ) ? "does" : "does not");
	Error::Log("\tSound Driver %s being emulated with waveform audio functions.", 
		(m_DSCaps.dwFlags & DSCAPS_EMULDRIVER) ? "is" : "is not");
	Error::Log("\tSound Driver %s support 16-bit primary buffers.", 
		(m_DSCaps.dwFlags & DSCAPS_PRIMARY16BIT) ? "does" : "does not");
	Error::Log("\tSound Driver %s support 8-bit primary buffers.", 
		(m_DSCaps.dwFlags & DSCAPS_PRIMARY8BIT) ? "does" : "does not");
	Error::Log("\tSound Driver %s support mono primary buffers.", 
		(m_DSCaps.dwFlags & DSCAPS_PRIMARYMONO) ? "does" : "does not");
	Error::Log("\tSound Driver %s support stereo primary buffers.", 
		(m_DSCaps.dwFlags & DSCAPS_PRIMARYSTEREO) ? "does" : "does not");
	Error::Log("\tSound Driver %s support 16-bit secondary buffers.", 
		(m_DSCaps.dwFlags & DSCAPS_SECONDARY16BIT ) ? "does" : "does not");
	Error::Log("\tSound Driver %s support 8-bit secondary buffers.", 
		(m_DSCaps.dwFlags & DSCAPS_SECONDARY8BIT ) ? "does" : "does not");
	Error::Log("\tSound Driver %s support mono secondary buffers.", 
		(m_DSCaps.dwFlags & DSCAPS_SECONDARYMONO) ? "does" : "does not");
	Error::Log("\tSound Driver %s support stereo secondary buffers.", 
		(m_DSCaps.dwFlags & DSCAPS_SECONDARYSTEREO) ? "does" : "does not");

	Error::Log("\tMinimum secondary buffer sample rate = %d", m_DSCaps.dwMinSecondarySampleRate);
	Error::Log("\tMaximum secondary buffer sample rate = %d", m_DSCaps.dwMaxSecondarySampleRate);
	Error::Log("\tNumber of primary buffers = %d", m_DSCaps.dwPrimaryBuffers);
	Error::Log("\tMaximum hardware buffers = %d", m_DSCaps.dwMaxHwMixingAllBuffers);
	Error::Log("\tMaximum hardware static buffers = %d", m_DSCaps.dwMaxHwMixingStaticBuffers );
	Error::Log("\tMaximum hardware streaming buffers = %d", m_DSCaps.dwMaxHwMixingStreamingBuffers );
	Error::Log("\tFree hardware buffers = %d", m_DSCaps.dwFreeHwMixingAllBuffers );
	Error::Log("\tFree hardware static buffers = %d", m_DSCaps.dwFreeHwMixingStaticBuffers );
	Error::Log("\tFree hardware streaming buffers = %d", m_DSCaps.dwFreeHwMixingStreamingBuffers );
	Error::Log("\tMaximum hardware 3D buffers = %d", m_DSCaps.dwMaxHw3DAllBuffers );
	Error::Log("\tMaximum hardware 3D static buffers = %d", m_DSCaps.dwMaxHw3DStaticBuffers );
	Error::Log("\tMaximum hardware 3D streaming buffers = %d", m_DSCaps.dwMaxHw3DStreamingBuffers );
	Error::Log("\tFree hardware 3D buffers = %d", m_DSCaps.dwFreeHw3DAllBuffers );
	Error::Log("\tFree hardware 3D static buffers = %d", m_DSCaps.dwFreeHw3DStaticBuffers );
	Error::Log("\tFree hardware 3D streaming buffers = %d", m_DSCaps.dwFreeHw3DStreamingBuffers );
	Error::Log("\tTotal hardware memory = %d", m_DSCaps.dwTotalHwMemBytes );
	Error::Log("\tFree hardware memory = %d", m_DSCaps.dwFreeHwMemBytes );
	Error::Log("\tMax contiguous free memory = %d", m_DSCaps.dwMaxContigFreeHwMemBytes );
	Error::Log("\tHardware buffer data transfer rate = %d", m_DSCaps.dwUnlockTransferRateHwBuffers );
	Error::Log("\tCPU overhead for software buffers = %d", m_DSCaps.dwPlayCpuOverheadSwBuffers );

	// Determine how to configure the system based on stats and initalization requirements
	if(init.m_bForceSoftware || 
		(init.m_n2DHardwareBufferMin > m_DSCaps.dwMaxHwMixingAllBuffers))
		m_Stats.m_bForce2DSoftware = true;
	if(init.m_bForceSoftware || 
		(init.m_n3DHardwareBufferMin > m_DSCaps.dwFreeHw3DAllBuffers))
		m_Stats.m_bForce3DSoftware = true;

	Error::Log("Force 2D software buffers = %s",	(m_Stats.m_bForce2DSoftware) ? "true" : "false");
	Error::Log("Force 3D software buffers = %s",	(m_Stats.m_bForce3DSoftware) ? "true" : "false");

	// Create the primary sound buffer
	DSBUFFERDESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.dwSize = sizeof(desc);
	desc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;
	hr = m_pDirectSound->CreateSoundBuffer(&desc, &m_pPrimaryBuffer, NULL);
	if(FAILED(hr))
		Error::Handle("Could not create primary buffer.  Error = %s.", DXGetErrorString(hr));

	// Setting the format and playing the primary buffer does nothing 
	// with modern WDM drivers, but it's good to try in case older 
	// VxD model drivers are used
	WAVEFORMATEX wf;
	memset(&wf, 0, sizeof(WAVEFORMATEX)); 
	wf.wFormatTag = WAVE_FORMAT_PCM; 
	wf.nChannels = 2; 
	wf.nSamplesPerSec = m_Init.m_nOptimalSampleRate; 
	wf.nBlockAlign = 4; 
	wf.nAvgBytesPerSec = 
		wf.nSamplesPerSec * wf.nBlockAlign; 
	wf.wBitsPerSample = m_Init.m_nOptimalSampleBits; 
	m_pPrimaryBuffer->SetFormat(&wf);
	if(FAILED(hr))
		Error::Handle("Could not set primary buffer format.  Error = %s.", DXGetErrorString(hr));
	m_pPrimaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
	if(FAILED(hr))
		Error::Handle("Could not play primary buffer.  Error = %s.", DXGetErrorString(hr));

	// Note that we're creating two performance objects, one for sound effects played
	// through DirectMusic, and the other is for music segments to play on.  This is
	// done so that playing segments as sound effects will not interfere with the
	// notification system used by the music manager.  This also makes it convenient
	// to set up different default audiopaths for the two types of segments.
    hr = CoCreateInstance(CLSID_DirectMusicPerformance, NULL,
                     CLSCTX_INPROC, IID_IDirectMusicPerformance8,
                     (void**)&m_pMusicPerformance );
	if(FAILED(hr))
		return Error::Handle("Could not create music DirectMusic Performance object.  Error = %s.", DXGetErrorString(hr));

	uint32 nDefaultAudiopath;
	if(m_Init.m_bUseMusicReverb)
		nDefaultAudiopath = DMUS_APATH_SHARED_STEREOPLUSREVERB;
	else
		nDefaultAudiopath = DMUS_APATH_DYNAMIC_STEREO;

	// Initialize the music performance
	hr = m_pMusicPerformance->InitAudio( 
			NULL,								// IDirectMusic
			(IDirectSound**)&m_pDirectSound,	// IDirectSound
			init.m_hWnd,						// Window handle.
			nDefaultAudiopath,					// Specify the default audiopath
			64,									// Number of performance channels.
			DMUS_AUDIOF_ALL,					// Features on synthesizer.
			NULL								// Audio parameters; use defaults.
		);
	if(FAILED(hr))
		return Error::Handle("Could not init music performance.  Error = %s", DXGetErrorString(hr));

	IDirectMusicAudioPath* pAudioPath = 0;
	hr = m_pMusicPerformance->GetDefaultAudioPath(&pAudioPath);
	if(FAILED(hr))
		return Error::Handle("Could not get music performance buffer.  Error = %s", DXGetErrorString(hr));

	// Create the sound performance object
    hr = CoCreateInstance(CLSID_DirectMusicPerformance, NULL,
                     CLSCTX_INPROC, IID_IDirectMusicPerformance8,
                     (void**)&m_pSoundPerformance );
	if(FAILED(hr))
		return Error::Handle("Could not create sound DirectMusic Performance object.  Error = %s.", DXGetErrorString(hr));

	// Initialize the sound performance
	hr = m_pSoundPerformance->InitAudio( 
			NULL,								// IDirectMusic
			(IDirectSound**)&m_pDirectSound,	// IDirectSound
			init.m_hWnd,						// Window handle.
			DMUS_APATH_DYNAMIC_STEREO,			// Standard stereo audiopath is default
			64,									// Number of performance channels.
			DMUS_AUDIOF_ALL,					// Features on synthesizer.
			NULL								// Audio parameters; use defaults.
		);
	if(FAILED(hr))
		return Error::Handle("Could not init sound performance.  Error = %s", DXGetErrorString(hr));

	hr = m_pSoundPerformance->GetDefaultAudioPath(&pAudioPath);
	if(FAILED(hr))
		return Error::Handle("Could not get music performance buffer.  Error = %s", DXGetErrorString(hr));

	// Now create the DirectMusic loader.  We don't really
	// need to bother with a DirectMusic object itself.
	hr = CoCreateInstance(CLSID_DirectMusicLoader, NULL, 
						 CLSCTX_INPROC, IID_IDirectMusicLoader8,
						 (void**)&m_pLoader);
	if(FAILED(hr))
		return Error::Handle("Could not create DirectMusic Loader object.  Error = %s.", DXGetErrorString(hr));

	// See if the user has provided a custom file factory object.  If not, create
	// our own disk-based system by default
	if(m_Init.m_pAudioStreamFactory)
		m_pStreamFactory = m_Init.m_pAudioStreamFactory;
	else
		m_pStreamFactory = new AudioStreamFactory;

	// Create an event for music notification
	m_hMusicNotify = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_pMusicPerformance->SetNotificationHandle(m_hMusicNotify, 0);

	// Set notifications for the music performance
	hr = m_pMusicPerformance->AddNotificationType(GUID_NOTIFICATION_MEASUREANDBEAT);
	if(FAILED(hr))
		return Error::Handle("Failed to set DirectMusic notification.  Error = %s", DXGetErrorString(hr));
	hr = m_pMusicPerformance->AddNotificationType(GUID_NOTIFICATION_PERFORMANCE);
	if(FAILED(hr))
		return Error::Handle("Failed to set DirectMusic notification.  Error = %s", DXGetErrorString(hr));
	hr = m_pMusicPerformance->AddNotificationType(GUID_NOTIFICATION_SEGMENT);
	if(FAILED(hr))
		return Error::Handle("Failed to set DirectMusic notification.  Error = %s", DXGetErrorString(hr));

	// Set the working directory
	if(m_Init.m_sWorkingPath.empty())
		getcwd(m_pszAudioSystemPath, MAX_PATH);
	else
		strcpy(m_pszAudioSystemPath, m_Init.m_sWorkingPath.c_str());

	// Create an event for loading notifications
	m_hLoadNotify = CreateEvent(NULL, FALSE, FALSE, NULL);

	// Initialize the audio update's critical section object
	InitializeCriticalSection(&m_csAudioUpdate);
	InitializeCriticalSection(&m_csLoading);
	InitializeCriticalSection(&m_csLoadScheduling);

	// Audio system has been successfully initialized
	m_bInitialized = true;

	// Note that we're using a very modest stack size of 4K in these threads.  
	// If you add anything substantial to these functions, you may need to
	// increase the stack size.

	// Set the callback for the timer function used for general events that 
	// need to happen ten times per second.
	if(_beginthread(&AudioManager::TimeEvent, 4096, NULL) == -1)
		return false;

	// Begin the DirectMusic event thread
	if(_beginthread(AudioManager::MusicEventThread, 4096, NULL) == -1)
		return false;

	// Start the loading thread
	if(_beginthread(AudioManager::LoadingThread, 4096, NULL) == -1)
		return false;

	Error::Log("Successfully initialized audio system.");

	return true;
}

//------------------------------------------------------------------------//
void AudioManager::Term()
{
	FN("AudioManager::Term()");

	bool bInit = m_bInitialized;

	if(m_bInitialized)
	{
		// Create event objects to be used by the threads to signal
		// a successful shutdown.
		m_hTerm[TIME_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_hTerm[MUSIC_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);

		// Indicates the audio manager is now terminating
		m_bInitialized = false;

		// Tell the music notification thread to proceed.
		SetEvent(m_hMusicNotify);

		// Wait for both threads to shut down before continuing
		WaitForMultipleObjects(2, m_hTerm, TRUE, INFINITE);

		// Close the event objects now that we're done with them
		CloseHandle(m_hTerm[TIME_EVENT]);
		CloseHandle(m_hTerm[MUSIC_EVENT]);

		// Enter and leave the update critical section to ensure that the
		// update loop is finished
		DeleteCriticalSection(&m_csAudioUpdate);

		// Set a load notify event so the loading thread can exit
		SetEvent(m_hLoadNotify);

		// Enter and leave the loading critical section to ensure that the
		// loading thread is finished
		EnterCriticalSection(&m_csLoading);
		LeaveCriticalSection(&m_csLoading);
		DeleteCriticalSection(&m_csLoading);

		EnterCriticalSection(&m_csLoadScheduling);
		LeaveCriticalSection(&m_csLoadScheduling);
		DeleteCriticalSection(&m_csLoadScheduling);

		// Close the load notification handle now that we're done with it
		CloseHandle(m_hLoadNotify);
	
		// Clear the stream lists
		m_SoundStreamProcess.clear();
		m_SoundStreamRemoval.clear();

		// Destroy all cached buffers
		m_BufferCache.Term();

		// Make sure the loaded lists are clear.  
		m_LoadedSound.clear();
		m_LoadedSound3D.clear();
		m_LoadedSegment.clear();

		// Destroy all allocated objects in the reserved pools
		Sound3D::TermPool();
		Sound::TermPool();		
		Segment::TermPool();
		AudioScript::TermPool();
		DLS::TermPool();
		
		if(m_hMusicNotify)
		{
			CloseHandle(m_hMusicNotify);
			m_hMusicNotify = 0;
		}

		// See if the user has provided a custom file factory object.  If not,
		// destroy the one we've created
		if(!m_Init.m_pAudioStreamFactory)
			_XASAFE_DELETE(m_pStreamFactory);


		// Delete the listener object
		Listener::DestroyObject(m_pListener);
		m_pListener = 0;

		// Release all DirectMusic components 
		_XASAFE_RELEASE(m_pLoader);
		if(m_pMusicPerformance)
			m_pMusicPerformance->CloseDown();
		_XASAFE_RELEASE(m_pMusicPerformance);
		if(m_pSoundPerformance)
			m_pSoundPerformance->CloseDown();
		_XASAFE_RELEASE(m_pSoundPerformance);

		// Release the primary buffer
		_XASAFE_RELEASE(m_pPrimaryBuffer);
		// Release the DirectSound interface
		_XASAFE_RELEASE(m_pDirectSound);

		if(bInit)
			CoUninitialize();

		// Make sure all errors are deleted from the error log
		Error::ClearLog();
	}

	// Clear all variables
	Clear();
}


//------------------------------------------------------------------------//
bool AudioManager::IsInitialized() const
{
	FN("AudioManager::IsInitialized()");
	return m_bInitialized;
}


//------------------------------------------------------------------------//
bool AudioManager::GetStats(AudioMgrStats& stats) const
{
	FN("AudioManager::GetStats()");
	CHECK_INIT();
	stats = m_Stats; 
	return true;
}


//------------------------------------------------------------------------//
void AudioManager::TimeEvent(LPVOID lpv)
{
	FN("AudioManager::TimeEvent()");
	while(true)
	{
		// Wake up every 50ms to perform some timed actions
		Sleep(50);

		// If the manager has been shut down then terminate this thread
		if(!DXAudioMgr()->m_bInitialized)
		{
			SetEvent(DXAudioMgr()->m_hTerm[TIME_EVENT]);
			return;
		}

		// Enter the critical section to ensure that functions that alter the
		// contents of the data through which will be looping through cannot
		// continue until we are finished with this function.
		EnterCriticalSection(&DXAudioMgr()->GetUpdateCS());

		// Use a static counter to make sure streams only get 
		// updated five times a second.  More often is just wasteful.
		static int iServiceStreams = 0;
		if((iServiceStreams++) % 4 == 1)
			DXAudioMgr()->ServiceStreamingBuffers();

		// Update the listener object - calculate all deferred 3D settings
		if(DXAudioMgr()->m_pListener)
			DXAudioMgr()->m_pListener->CommitDeferredSettings();

		// We're done with the critical section now
		LeaveCriticalSection(&DXAudioMgr()->GetUpdateCS());
	}
}

//------------------------------------------------------------------------//
void AudioManager::ServiceStreamingBuffers()
{		
	FN("AudioManager::ServiceStreamingBuffers()");
	// Remove any buffers marked for removal after we're done iterating through the list
	SoundList::iterator itr;
	for(itr = m_SoundStreamRemoval.begin(); itr != m_SoundStreamRemoval.end(); ++itr)
	{
		SoundList::iterator itor = find(m_SoundStreamProcess.begin(), m_SoundStreamProcess.end(), *itr);
		if(itor != m_SoundStreamProcess.end())
			m_SoundStreamProcess.erase(itor);
	}
	// Clear the removal list
	m_SoundStreamRemoval.clear();
	// Iterate through all streaming buffers and update their buffer data
	for(itr = m_SoundStreamProcess.begin(); itr != m_SoundStreamProcess.end(); ++itr)
		(*itr)->ServiceBuffer();	
}

//------------------------------------------------------------------------//
void AudioManager::InsertStream(Sound* pStream)
{
	FN("AudioManager::InsertStream(Sound* pStream = 0x%X)", pStream);
	m_SoundStreamProcess.push_back(pStream);
}

//------------------------------------------------------------------------//
void AudioManager::RemoveStream(Sound* pStream)
{
	FN("AudioManager::RemoveStream(Sound* pStream = 0x%X)", pStream);
	m_SoundStreamRemoval.push_back(pStream);
}


//------------------------------------------------------------------------//
void AudioManager::OnLoadSound(Sound* pSound)
{
	FN("AudioManager::OnLoadSound(Sound* pSound = 0x%X)", pSound);
	CRITICAL_FUNCTION(&m_csAudioUpdate);
	m_Stats.m_n2DSoundsLoaded++;
	m_LoadedSound.push_back(pSound);
	DebugOut(3, "Sound added: %d sound(s) currently loaded.", m_Stats.m_n2DSoundsLoaded);
}

//------------------------------------------------------------------------//
void AudioManager::OnUnloadSound(Sound* pSound)
{
	FN("AudioManager::OnUnloadSound(Sound* pSound = 0x%X)", pSound);
	if(!IsInitialized())
		return;
	CRITICAL_FUNCTION(&m_csAudioUpdate);
	SoundVector::iterator itr;
	for(itr = m_LoadedSound.begin(); itr != m_LoadedSound.end(); ++itr)
	{
		if((*itr) == pSound)
		{
			m_LoadedSound.erase(itr);
			m_Stats.m_n2DSoundsLoaded--;
			DebugOut(3, "Sound removed: %d sound(s) currently loaded.", m_Stats.m_n2DSoundsLoaded);
			return;
		}
	}
}

//------------------------------------------------------------------------//
void AudioManager::OnLoadSound3D(Sound3D* pSound3D)
{
	FN("AudioManager::OnLoadSound3D(Sound3D* pSound3D = 0x%X)", pSound3D);
	CRITICAL_FUNCTION(&m_csAudioUpdate);
	m_Stats.m_n3DSoundsLoaded++;
	m_LoadedSound3D.push_back(pSound3D);
	DebugOut(3, "3D Sound added: %d sound(s) currently loaded.", m_Stats.m_n3DSoundsLoaded);
}

//------------------------------------------------------------------------//
void AudioManager::OnUnloadSound3D(Sound3D* pSound3D)
{
	FN("AudioManager::OnUnloadSound3D(Sound3D* pSound3D = 0x%X)", pSound3D);
	if(!IsInitialized())
		return;
	CRITICAL_FUNCTION(&m_csAudioUpdate);
	Sound3DVector::iterator itr;
	for(itr = m_LoadedSound3D.begin(); itr != m_LoadedSound3D.end(); ++itr)
	{
		if((*itr) == pSound3D)
		{
			m_LoadedSound3D.erase(itr);
			m_Stats.m_n3DSoundsLoaded--;
			DebugOut(3, "3D Sound removed: %d sound(s) currently loaded.", m_Stats.m_n3DSoundsLoaded);
			return;
		}
	}
}

//------------------------------------------------------------------------//
void AudioManager::OnLoadSegment(Segment* pSegment)
{
	FN("AudioManager::OnLoadSegment(Segment* pSegment = 0x%X)", pSegment);
	CRITICAL_FUNCTION(&m_csAudioUpdate);
	m_Stats.m_nSegmentsLoaded++;
	m_LoadedSegment.push_back(pSegment);
	DebugOut(3, "Segment added: %d segment(s) currently loaded.", m_Stats.m_nSegmentsLoaded);
}

//------------------------------------------------------------------------//
void AudioManager::OnUnloadSegment(Segment* pSegment)
{
	FN("AudioManager::OnUnloadSegment(Segment* pSegment = 0x%X)", pSegment);
	if(!IsInitialized())
		return;
	CRITICAL_FUNCTION(&m_csAudioUpdate);
	SegmentVector::iterator itr;
	for(itr = m_LoadedSegment.begin(); itr != m_LoadedSegment.end(); ++itr)
	{
		if((*itr) == pSegment)
		{
			m_LoadedSegment.erase(itr);
			m_Stats.m_nSegmentsLoaded--;
			DebugOut(3, "Segment removed: %d segment(s) currently loaded.", m_Stats.m_nSegmentsLoaded);
			return;
		}
	}
}



//------------------------------------------------------------------------//
void AudioManager::LoadingThread(LPVOID lpv)
{
	CoInitialize(NULL);
	DXAudioMgr()->ServiceLoading();
	CoUninitialize();
}


//------------------------------------------------------------------------//
void AudioManager::ServiceLoading()
{
	while(true)
	{
		// Wait until an loading notification event is received
		WaitForSingleObject(m_hLoadNotify, INFINITE);

		// If the audio manager is terminating, then exit the thread
		if(!m_bInitialized)
			return;

		// Copy the temporary queues to the main loading queues
		EnterCriticalSection(&m_csLoadScheduling);
		while(!m_SoundLoadTemp.empty())
		{
			m_SoundLoadPending.push(m_SoundLoadTemp.front());
			m_SoundLoadTemp.pop();
		}
		while(!m_Sound3DLoadTemp.empty())
		{
			m_Sound3DLoadPending.push(m_Sound3DLoadTemp.front());
			m_Sound3DLoadTemp.pop();
		}
		while(!m_SegmentLoadTemp.empty())
		{
			m_SegmentLoadPending.push(m_SegmentLoadTemp.front());
			m_SegmentLoadTemp.pop();
		}
		LeaveCriticalSection(&m_csLoadScheduling);

		// Do loading work
		bool bFinishedLoading = false;
		EnterCriticalSection(&m_csLoading);
		while(!bFinishedLoading)
		{

			if(!m_SoundLoadPending.empty())
			{
				if(!m_SoundLoadPending.front()->DoLoad())
					Error::Handle("WARNING! Unable to load sound!");
				m_SoundLoadPending.pop();
			}
			if(!m_Sound3DLoadPending.empty())
			{
				if(!m_Sound3DLoadPending.front()->DoLoad())
					Error::Handle("WARNING! Unable to load 3D sound!");
				m_Sound3DLoadPending.pop();
			}
			if(!m_SegmentLoadPending.empty())
			{
				if(!m_SegmentLoadPending.front()->DoLoad())
					Error::Handle("WARNING! Unable to load segment!");
				m_SegmentLoadPending.pop();
			}

			if(m_SoundLoadPending.empty() && m_Sound3DLoadPending.empty() &&
				m_SegmentLoadPending.empty())
				bFinishedLoading = true;
		}
		LeaveCriticalSection(&m_csLoading);
	}
}



//------------------------------------------------------------------------//
void AudioManager::ScheduleLoad(Sound* pSound)
{
	EnterCriticalSection(&m_csLoadScheduling);
	m_SoundLoadTemp.push(pSound);
	LeaveCriticalSection(&m_csLoadScheduling);
	SetEvent(m_hLoadNotify);
}


//------------------------------------------------------------------------//
void AudioManager::ScheduleLoad(Sound3D* pSound3D)
{
	EnterCriticalSection(&m_csLoadScheduling);
	m_Sound3DLoadTemp.push(pSound3D);
	LeaveCriticalSection(&m_csLoadScheduling);
	SetEvent(m_hLoadNotify);
}


//------------------------------------------------------------------------//
void AudioManager::ScheduleLoad(Segment* pSegment)
{
	EnterCriticalSection(&m_csLoadScheduling);
	m_SegmentLoadTemp.push(pSegment);
	LeaveCriticalSection(&m_csLoadScheduling);
	SetEvent(m_hLoadNotify);
}


//------------------------------------------------------------------------//
void AudioManager::MusicEventThread(LPVOID lpv)
{
	DXAudioMgr()->UpdateMusic();
}


//------------------------------------------------------------------------//
void AudioManager::UpdateMusic()
{
	FN("AudioManager::UpdateMusic()");

	DMUS_NOTIFICATION_PMSG* pPmsg;
	while(true)
	{
		WaitForSingleObject(m_hMusicNotify, INFINITE);
		if(!m_bInitialized)
		{
			SetEvent(m_hTerm[MUSIC_EVENT]);
			return;
		}
		while (S_OK == m_pMusicPerformance->GetNotificationPMsg(&pPmsg))
		{
			if(pPmsg->guidNotificationType == GUID_NOTIFICATION_MEASUREANDBEAT)
			{
				DebugOut(5, "Music notification: Beat = %d", pPmsg->dwField1);
			}
			else if(pPmsg->guidNotificationType == GUID_NOTIFICATION_PERFORMANCE)
			{
				if(pPmsg->dwNotificationOption == DMUS_NOTIFICATION_MUSICALMOSTEND)
				{
					DebugOut(5, "Music notification: Music almost at end");

					if(!m_pCurrentSegment)
						break;

					if(m_pCurrentSegment->IsLooping() && !m_pNextSegment)
					{
						ISegment* pSeg = m_pCurrentSegment;
						m_pCurrentSegment = 0;
						m_pNextSegment = 0;
						pSeg->Play();
					}
					else if(m_pNextSegment)
					{
						ISegment* pSeg = m_pNextSegment;
						m_pCurrentSegment = 0;
						m_pNextSegment = 0;
						pSeg->Play();
					}
				}
				else if(pPmsg->dwNotificationOption == DMUS_NOTIFICATION_MUSICSTARTED)
				{
					DebugOut(5, "Music notification: Music started");
				}
				else if(pPmsg->dwNotificationOption == DMUS_NOTIFICATION_MUSICSTOPPED)
				{
					DebugOut(5, "Music notification: Music stopped");
				}
			}
			else if(pPmsg->guidNotificationType == GUID_NOTIFICATION_SEGMENT)
			{
				if(pPmsg->dwNotificationOption == DMUS_NOTIFICATION_SEGABORT)
				{
					DebugOut(5, "Music notification: Segment aborted");
				}
				else if(pPmsg->dwNotificationOption == DMUS_NOTIFICATION_SEGALMOSTEND)
				{
					DebugOut(5, "Music notification: Segment almost at end");
				}
				else if(pPmsg->dwNotificationOption == DMUS_NOTIFICATION_SEGEND)
				{
					DebugOut(5, "Music notification: Segment at end");
				}
				else if(pPmsg->dwNotificationOption == DMUS_NOTIFICATION_SEGLOOP)
				{
					DebugOut(5, "Music notification: Segment has looped");
				}
				else if(pPmsg->dwNotificationOption == DMUS_NOTIFICATION_SEGSTART)
				{
					DebugOut(5, "Music notification: Segment has started");

					if(m_Init.m_pMusicCallback)
						m_Init.m_pMusicCallback->OnSegmentStart();
				}
			}
			m_pMusicPerformance->FreePMsg((DMUS_PMSG*)pPmsg); 
		}
	}
}


//------------------------------------------------------------------------//
bool AudioManager::CreateSound(ISound*& pSound)
{
	FN("AudioManager::CreateSound()");
	CHECK_INIT();
	pSound = Sound::CreateObject();
	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::CreateSound3D(ISound3D*& pSound3D)
{
	FN("AudioManager::CreateSound3D()");
	CHECK_INIT();
	pSound3D = Sound3D::CreateObject();
	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::CreateSegment(ISegment*& pSegment)
{
	FN("AudioManager::CreateSegment()");
	CHECK_INIT();
	pSegment = Segment::CreateObject();
	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::CreateDLS(IDLS*& pDLS)
{
	FN("AudioManager::CreateDLS()");
	CHECK_INIT();
	pDLS = DLS::CreateObject();
	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::CreateAudioScript(IAudioScript*& pScript)
{
	FN("AudioManager::CreateAudioScript()");
	CHECK_INIT();
	pScript = AudioScript::CreateObject();
	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::GetListener(IListener*& pListener)
{
	FN("AudioManager::GetListener()");
	CHECK_INIT();
	if(!m_pListener)
		m_pListener = Listener::CreateObject();
	pListener = m_pListener;
	return true;
}

//------------------------------------------------------------------------//
bool AudioManager::SetSoundVolume(float fVolume)
{
	FN("AudioManager::SetSoundVolume(float fVolume = %f)", fVolume);
	CHECK_INIT();

	// Ensure the volume stays within the allowed range
	m_fSoundVolume = Clamp<float>(fVolume, VOLUME_MIN, VOLUME_MAX);

	// Iterate through all the sounds designated as sound fx, getting
	// and setting the volume to ensure it applies the new global
	// sound setting.
	SoundVector::iterator itr;
	float fVol;
	for(itr = m_LoadedSound.begin(); itr != m_LoadedSound.end(); ++itr)
	{
		if(!(*itr)->IsMusic())
		{
			(*itr)->GetVolume(fVol);
			(*itr)->SetVolume(fVol);
		}
	}
	Sound3DVector::iterator itr3d;
	for(itr3d = m_LoadedSound3D.begin(); itr3d != m_LoadedSound3D.end(); ++itr3d)
	{
		if(!(*itr3d)->IsMusic())
		{
			(*itr3d)->GetVolume(fVol);
			(*itr3d)->SetVolume(fVol);
		}
	}

	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::GetSoundVolume(float& fVolume) const
{
	FN("AudioManager::GetSoundVolume()");
	fVolume = 0.0f;
	CHECK_INIT();
	fVolume = m_fSoundVolume;
	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::SetMusicVolume(float fVolume)
{
	FN("AudioManager::SetMusicVolume(float fVolume = %f)", fVolume);
	CHECK_INIT();


	// Ensure the volume stays within the allowed range
	m_fMusicVolume = Clamp<float>(fVolume, VOLUME_MIN, VOLUME_MAX);

	// Iterate through all the sounds designated as sound fx, getting
	// and setting the volume to ensure it applies the new global
	// sound setting.
	SoundVector::iterator itr;
	float fVol;
	for(itr = m_LoadedSound.begin(); itr != m_LoadedSound.end(); ++itr)
	{
		if((*itr)->IsMusic())
		{
			(*itr)->GetVolume(fVol);
			(*itr)->SetVolume(fVol);
		}
	}
	Sound3DVector::iterator itr3d;
	for(itr3d = m_LoadedSound3D.begin(); itr3d != m_LoadedSound3D.end(); ++itr3d)
	{
		if((*itr3d)->IsMusic())
		{
			(*itr3d)->GetVolume(fVol);
			(*itr3d)->SetVolume(fVol);
		}
	}


	// Set the global volume on the
	int32 nVol = LinearToLogVol(m_fMusicVolume);
	Performance(true)->SetGlobalParam(GUID_PerfMasterVolume, &nVol, sizeof(int32));

	return true;

}


//------------------------------------------------------------------------//
bool AudioManager::GetMusicVolume(float& fVolume) const
{
	FN("AudioManager::GetMusicVolume()");
	fVolume = 0.0f;
	CHECK_INIT();
	fVolume = m_fMusicVolume;
	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::StopAll()
{
	FN("AudioManager::StopAll()");
	CHECK_INIT();
	CRITICAL_FUNCTION(&m_csAudioUpdate);

	// Stop Music
	if(m_pCurrentSegment)
		m_pCurrentSegment->Stop();

	// Stop 2d sounds
	SoundVector::iterator snditr;
	for(snditr = m_LoadedSound.begin(); snditr != m_LoadedSound.end(); ++snditr)
		(*snditr)->Stop();

	// Stop 3d sounds
	Sound3DVector::iterator snd3ditr;
	for(snd3ditr = m_LoadedSound3D.begin(); snd3ditr != m_LoadedSound3D.end(); ++snd3ditr)
		(*snd3ditr)->Stop();

	// Stop Segments
	SegmentVector::iterator segitr;
	for(segitr = m_LoadedSegment.begin(); segitr != m_LoadedSegment.end(); ++segitr)
		(*segitr)->Stop();

	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::PauseAll()
{
	FN("AudioManager::PauseAll()");
	CHECK_INIT();
	CRITICAL_FUNCTION(&m_csAudioUpdate);

	// Pause Music
	if(m_pCurrentSegment)
		m_pCurrentSegment->Pause();

	// Pause 2d sounds
	SoundVector::iterator snditr;
	for(snditr = m_LoadedSound.begin(); snditr != m_LoadedSound.end(); ++snditr)
		(*snditr)->Pause();

	// Pause 3d sounds
	Sound3DVector::iterator snd3ditr;
	for(snd3ditr = m_LoadedSound3D.begin(); snd3ditr != m_LoadedSound3D.end(); ++snd3ditr)
		(*snd3ditr)->Pause();

	// Pause Segments
	SegmentVector::iterator segitr;
	for(segitr = m_LoadedSegment.begin(); segitr != m_LoadedSegment.end(); ++segitr)
		(*segitr)->Pause();

	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::ResumeAll()
{
	FN("AudioManager::ResumeAll()");
	CHECK_INIT();
	CRITICAL_FUNCTION(&m_csAudioUpdate);

	// Resume Music
	if(m_pCurrentSegment)
		if(m_pCurrentSegment->IsPaused())
			m_pCurrentSegment->Play();

	// Stop 2d sounds
	SoundVector::iterator snditr;
	for(snditr = m_LoadedSound.begin(); snditr != m_LoadedSound.end(); ++snditr)
		if((*snditr)->IsPaused())
			(*snditr)->Play();

	// Stop 3d sounds
	Sound3DVector::iterator snd3ditr;
	for(snd3ditr = m_LoadedSound3D.begin(); snd3ditr != m_LoadedSound3D.end(); ++snd3ditr)
		if((*snd3ditr)->IsPaused())
			(*snd3ditr)->Play();

	// Stop Segments
	SegmentVector::iterator segitr;
	for(segitr = m_LoadedSegment.begin(); segitr != m_LoadedSegment.end(); ++segitr)
		if((*segitr)->IsPaused())
		(*segitr)->Play();

	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::GetCurrentSegment(ISegment*& pSegment) const
{
	FN("AudioManager::GetCurrentSegment()");
	CHECK_INIT();
	pSegment = m_pCurrentSegment;
	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::GetNextSegment(ISegment*& pSegment) const
{
	FN("AudioManager::GetNextSegment()");
	CHECK_INIT();
	pSegment = m_pNextSegment;
	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::CanAddSound() const
{
	FN("AudioManager::CanAddSound()");
	if(m_Stats.m_bForce2DSoftware)
	{
		if((m_Stats.m_n2DSoundsLoaded) >= m_Init.m_n2DSoftwareBufferMax)
			return false;
	}
	else
	{
		if((m_Stats.m_n2DSoundsLoaded) >= m_Init.m_n2DHardwareBufferMax)
			return false;
	}
	return true;
}

//------------------------------------------------------------------------//
bool AudioManager::CanAddSound3D() const
{
	FN("AudioManager::CanAddSound3D()");
	if(m_Stats.m_bForce3DSoftware)
	{
		if((m_Stats.m_n3DSoundsLoaded) >= m_Init.m_n3DSoftwareBufferMax)
			return false;
	}
	else
	{
		if((m_Stats.m_n3DSoundsLoaded) >= m_Init.m_n3DHardwareBufferMax)
			return false;
	}
	return true;
}

//------------------------------------------------------------------------//
bool AudioManager::CanAddSegment() const
{
	FN("AudioManager::CanAddSegment()");
	if((m_Stats.m_nSegmentsLoaded) >= m_Init.m_nSegmentMax)
		return false;
	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::RemoveSound(Sound* pSound)
{
	FN("AudioManager::RemoveSound()");
	CRITICAL_FUNCTION(&m_csAudioUpdate);
	// Check for the lowest priority item in the segment set
	sort(m_LoadedSound.begin(), m_LoadedSound.end(), ptr_less<Sound*>() );
	if(m_LoadedSound.empty())
		return true;
	if(!pSound)
		return true;
	Sound* pFront = m_LoadedSound.front();
	if(*pSound < *pFront)
		return false;
	pFront->Unload();
	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::RemoveSound3D(Sound3D* pSound3D)
{
	FN("AudioManager::RemoveSound3D()");
	CRITICAL_FUNCTION(&m_csAudioUpdate);
	// Check for the lowest priority item in the segment set
	sort(m_LoadedSound3D.begin(), m_LoadedSound3D.end(), ptr_less<Sound3D*>() );
	if(m_LoadedSound3D.empty())
		return true;
	if(!pSound3D)
		return true;
	Sound3D* pFront = m_LoadedSound3D.front();
	if(*pSound3D < *pFront)
		return false;
	pFront->Unload();
	return true;
}


//------------------------------------------------------------------------//
void AudioManager::ResetSoundLimit()
{
	m_Init.m_n2DHardwareBufferMax = m_LoadedSound.size();
}


//------------------------------------------------------------------------//
void AudioManager::ResetSound3DLimit()
{
	m_Init.m_n3DHardwareBufferMax = m_LoadedSound3D.size();
}


//------------------------------------------------------------------------//
bool AudioManager::RemoveSegment(Segment* pSegment)
{
	FN("AudioManager::RemoveSegment()");
	CRITICAL_FUNCTION(&m_csAudioUpdate);
	// Check for the lowest priority item in the segment set
	sort(m_LoadedSegment.begin(), m_LoadedSegment.end(), ptr_less<Segment*>() );
	if(m_LoadedSegment.empty())
		return true;
	if(!pSegment)
		return true;
	Segment* pFront = m_LoadedSegment.front();
	if(*pSegment < *pFront)
		return false;
	pFront->Unload();
	return true;
}


//------------------------------------------------------------------------//
bool AudioManager::CreateAudioStream(IAudioStream*& pStream)
{
	FN("AudioManager::CreateAudioStream()");
	CHECK_INIT();
	if(!m_pStreamFactory)
		return false;
	return m_pStreamFactory->CreateAudioStream(pStream);
}




