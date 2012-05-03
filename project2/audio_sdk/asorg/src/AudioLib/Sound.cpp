/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "Sound.h"
#include "AudioCommon.h"
#include "AudioMgr.h"
#include "Wave.h"
#include "Vorbis.h"
#include "WMA.h"
#include "XAudioPackage.h"
#include "XPackageWave.h"

using namespace std;
using namespace Audio;

const float STREAMING_BUFFER_SECONDS = 1.0f;

IMPLEMENT_POOL(Sound);

//------------------------------------------------------------------------//
Sound::Sound()
{
	FN("Sound::Sound()");
	Clear();
}


//------------------------------------------------------------------------//
Sound::~Sound()
{
	FN("Sound::~Sound()");
	Term();
}


//------------------------------------------------------------------------//
void Sound::Clear()
{
	FN("Sound::Clear()");
	m_bInitialized = false;
	m_pDSBuffer = 0;
	m_PropertySet.Clear();
	m_Init.Clear();
	m_b3DSound = false;
	ZeroMemory(&m_WaveFormat, sizeof(WAVEFORMATEX));
	ZeroMemory(&m_BufferDesc, sizeof(DSBUFFERDESC));
	m_BufferDesc.dwSize = sizeof(DSBUFFERDESC);
	ZeroMemory(&m_Caps, sizeof(DSBCAPS));
	m_Caps.dwSize = sizeof(DSBCAPS);
	m_nBytesPlayed = 0;
	m_nLastReadPos = 0;
	m_nDataCursor = 0;
	m_bRemoveStream = true;
	m_pLoader = 0;
	m_bPaused = false;
	m_nSourceSize = 0;
	m_nLastTimePlayed = 0;
	m_bQueuePlayback = false;
	m_bLoading = false;
	m_bLoaded = false;
}



//------------------------------------------------------------------------//
bool Sound::Init(const SoundInit& init)
{
	FN("Sound::Init()");

	if(m_bInitialized)
		return Error::Handle("Sound object is already initialized");

	// Set the audio definition
	m_Init = init;
	
	// Extract the extension - decide on a loader based on extension name
	string sExt = m_Init.m_sFileName.substr(m_Init.m_sFileName.size() - 3);

	// Use the Wave loader
	if(stricmp(sExt.c_str(), "wav") == 0)
	{
		if( m_Init.m_pPackage )
		{
			m_pLoader = _XPackageWave::CreateObject();
			((_XPackageWave*)m_pLoader)->m_pPackage = m_Init.m_pPackage;
		}
		else
		{
			m_pLoader = Wave::CreateObject();
		}		
	}
	// Use the Vorbis loader
#ifdef USE_VORBIS
	else if(stricmp(sExt.c_str(), "ogg") == 0)
		m_pLoader = Vorbis::CreateObject();
#endif // USE_VORBIS
#ifdef USE_WMA
	else if(stricmp(sExt.c_str(), "wma") == 0)
		m_pLoader = WMA::CreateObject();
	else if(stricmp(sExt.c_str(), "mp3") == 0)
		m_pLoader = WMA::CreateObject();
#endif // USE_WMA
	else
		return Error::Handle("Unsupported file type");

	m_bInitialized = true;

	return true;
}


//------------------------------------------------------------------------//
void Sound::Term()
{
	FN("Sound::Term()");
	Unload();
	m_PropertySet.Term();
	if(m_pLoader)
	{
		m_pLoader->Destroy();
		m_pLoader = 0;
	}
	Clear();

	m_pDSBuffer = NULL;
	m_pLoader = NULL;
}

//------------------------------------------------------------------------//
void Sound::Destroy()
{
	FN("Sound::Destroy()");
	Term();
	Sound::DestroyObject(this);
}

//------------------------------------------------------------------------//
bool Sound::Load()
{
	FN("Sound::Load()");
	
	// Don't allow the function to proceed if the audio manager 
	// is not initialized
	if(!DXAudioMgr()->IsInitialized())
		return false;
	
	// Make sure we don't reload the sound
	if(IsLoaded() || IsLoading())
		return true;
	
	// If not loading asyncronously, load immediately and return
	if(!DXAudioMgr()->LoadAsync())
		return DoLoad();
	
	// Otherwise, schedule the audio manager to asynchronously load this sound
	m_bLoading = true;
	DXAudioMgr()->ScheduleLoad(this);
	
	return true;
}


//------------------------------------------------------------------------//
bool Sound::DoLoad()
{
	FN("Sound::DoLoad()");

	// Don't allow the function to proceed if the audio manager 
	// is not initialized
	if(!DXAudioMgr()->IsInitialized())
		return false;

	// First prepare and load the source
	uint32 nBufferSize = 0;
	if(!LoadSource(nBufferSize))
		return false;

	// Set this buffer's data size
	m_nSourceSize = m_pLoader->GetSize();

	// Ensure we're not over the buffer limit
	if(!m_b3DSound)
	{
		if(!DXAudioMgr()->CanAddSound())
			DXAudioMgr()->RemoveSound(this);
	}

	// Set the buffer creation flags depending on user preferences
	uint32 nFlags =
		DSBCAPS_GETCURRENTPOSITION2 | 
		DSBCAPS_CTRLFREQUENCY | 
		DSBCAPS_CTRLVOLUME |
		DSBCAPS_GLOBALFOCUS;

	// Check for 3d sound
	if(m_b3DSound)
		nFlags |= DSBCAPS_CTRL3D | DSBCAPS_MUTE3DATMAXDISTANCE;

	// Check for pan flag
	if((m_WaveFormat.nChannels == 1) && !m_b3DSound)
		nFlags |= DSBCAPS_CTRLPAN;

	// Check for multichannel / 3d conflict
	if((m_WaveFormat.nChannels != 1) && m_b3DSound)
	{
		Unload();
		return Error::Handle("Can't create a 3d buffer with more than one channel in sound source.");
	}
	
	// Determine if we should be in hardware or software
	if(DXAudioMgr()->ForceSoftware(m_b3DSound))
		nFlags |= DSBCAPS_LOCSOFTWARE;
	else
		nFlags |= DSBCAPS_LOCHARDWARE;

	// Create the actual sound buffer
	m_BufferDesc.dwFlags = nFlags;
	m_BufferDesc.lpwfxFormat = &m_WaveFormat;
	m_BufferDesc.dwBufferBytes = nBufferSize;

	if(!DXAudioMgr()->GetBufferCache()->Acquire(m_BufferDesc, m_pDSBuffer, !m_Init.m_bLooping))
		return false;

	// Get the caps to determine the actual size of the created buffer
	HRESULT hr = m_pDSBuffer->GetCaps(&m_Caps);
	if(FAILED(hr))
	{
		Unload();
		return Error::Handle("Could not get buffer caps.  Error = %s", DXGetErrorString(hr));
	}

	// Get the property set interface for this buffer if it's not a 3d sound buffer.
	if(!m_b3DSound)
		if(!m_PropertySet.OnLoad(m_pDSBuffer))
			Error::Handle("Could not obtain IKsPropertySet interface");
	
	// Fill the entire buffer with source data
	if(!FillBuffer())
	{
		Unload();
		return false;
	}

	// We no longer need the file or reader if it's a static buffer (non-streaming)
	if(!m_Init.m_bStreaming)
		m_pLoader->Close();

	DebugOut(3, "Loaded sound %s", m_Init.m_sFileName.c_str());

	// Insert into the appropriate load list if not a 3d sound.
	// 3D sounds are loaded into their own list as appropriate
	if(!m_b3DSound)
		DXAudioMgr()->OnLoadSound(this);

	SetProperties(m_Init.m_Prop);

	m_bLoading = false;
	m_bLoaded = true;

	if(m_bQueuePlayback)
		Play();

	return true;
}


//------------------------------------------------------------------------//
bool Sound::LoadSource(uint32& nBufferSize)
{
	FN("Sound::LoadSource()");

	// Don't allow the function to proceed if the audio manager 
	// is not initialized
	if(!DXAudioMgr()->IsInitialized())
		return false;

	if(!m_pLoader->Open(m_Init.m_sFileName))
		return false;

	memcpy(&m_WaveFormat, m_pLoader->GetFormat(), sizeof(WAVEFORMATEX));

	// Check to see if we're streaming or now.  We handle things a bit differently
	// in each case.
	if(m_Init.m_bStreaming)
	{
		// Calculate the size of the streaming buffer.  Note that currently a one
		// second buffer is being used.
		nBufferSize = m_WaveFormat.nAvgBytesPerSec * STREAMING_BUFFER_SECONDS;

		// Make sure we don't try to stream a file that's smaller than the
		// buffer size - this will not work, and is completely unnecessary anyhow.
		if(nBufferSize > m_pLoader->GetSize())
		{
			// If so, just make it a static buffer and continue with a warning
			DebugOut(2, "Warning!  Sound is too small for streaming - continuing as a static buffer.");
			m_Init.m_bStreaming = false;
			nBufferSize = m_pLoader->GetSize();
			return true;
		}
	}
	else
	{
		// calculate the size of the dsound buffer
		nBufferSize = m_pLoader->GetSize();

		// Switch to a streaming buffer if auto stream caching is on
		// and the file is large enough...
		if(DXAudioMgr()->GetInit()->m_bAutoStream &&  
			(nBufferSize >= (m_WaveFormat.nAvgBytesPerSec * STREAMING_BUFFER_SECONDS)))
		{
			m_Init.m_bStreaming = true;
			nBufferSize = (m_WaveFormat.nAvgBytesPerSec * STREAMING_BUFFER_SECONDS);
		}
		// If buffer caching is on, force small non-looping buffers to use a
		// minimum standard size.  This will allow us to efficiently
		// reuse these buffers
		else if(DXAudioMgr()->GetInit()->m_bCacheBuffers && 
			(m_Init.m_bLooping == false) &&
			(nBufferSize < (m_WaveFormat.nAvgBytesPerSec * STREAMING_BUFFER_SECONDS)))
		{
			nBufferSize = (m_WaveFormat.nAvgBytesPerSec * STREAMING_BUFFER_SECONDS);
		}
	}

	return true;
}


//------------------------------------------------------------------------//
bool Sound::FillBuffer()
{
	FN("Sound::FillBuffer()");

	CRITICAL_FUNCTION(&DXAudioMgr()->GetUpdateCS());

	// Lock the buffer
	void* pData;
	uint32  nBytes;
	HRESULT hr;
 	hr = m_pDSBuffer->Lock(
		0,
		0,
		&pData,
		&nBytes,
		NULL,
		NULL,
		DSBLOCK_ENTIREBUFFER);
	if(FAILED(hr))
		return Error::Handle("Could not lock sound buffer.  Error = %s", DXGetErrorString(hr));

	// Fill the entire buffer with audio data from source
	uint32 nBytesToRead;
	uint32 nBytesRead;

	nBytesToRead = nBytes;

	// Note that in the case of errors, we'll still want to continue so we
	// can properly unlock the buffer again.
	if(!m_pLoader->Read((unsigned char*)pData, nBytesToRead, &nBytesRead))
	{		
		Error::Handle("Could not read sound data source.");
	}

	m_nDataCursor += nBytesRead;
	m_nDataCursor %= nBytes;

	// If we read less than the entire buffer, fill the rest with silence
	if(nBytesRead < nBytes)
		memset(((unsigned char*)pData) + nBytesRead, GetSilenceData(), nBytes - nBytesRead);

	// Unlock buffer
	hr = m_pDSBuffer->Unlock(pData, nBytes, NULL, 0);
	if(FAILED(hr))
		return Error::Handle("Could not unlock sound buffer.  Error = %s", DXGetErrorString(hr));

	return true;
}


//------------------------------------------------------------------------//
bool Sound::Unload()
{
	FN("Sound::Unload()");

	// Don't allow the function to proceed if the audio manager 
	// is not initialized
	if(!DXAudioMgr()->IsInitialized())
		return false;

	// Check to make sure nothing is currently being loaded
	CRITICAL_FUNCTION(&DXAudioMgr()->GetLoadingCS());

	// If already unloaded, don't bother
	if(!IsLoaded())
		return true;

	EnterCriticalSection(&DXAudioMgr()->GetUpdateCS());

	// Stop the buffer before unloading
	Stop();

	// Remove from the appropriate load list
	if(!m_b3DSound)
		DXAudioMgr()->OnUnloadSound(this);

	// Release the source if it's a streaming buffer
	if(m_Init.m_bStreaming)		
    {		
        m_pLoader->Close();		
        
		// Remove the stream from the update list		
        DXAudioMgr()->RemoveStream(this);		
        
		// Reset streaming variables		
        m_nBytesPlayed = 0;		
        m_nLastReadPos = 0;		
        m_nDataCursor = 0;		
    }
	
	

	// Release the propertyset interface
	m_PropertySet.OnUnload();

	// Release the DirectSound buffer
	DXAudioMgr()->GetBufferCache()->Free(m_pDSBuffer);
	_XASAFE_RELEASE(m_pDSBuffer);

	// Mark the sound as unloaded
	m_bLoaded = false;

	LeaveCriticalSection(&DXAudioMgr()->GetUpdateCS());

	DebugOut(3, "Unloaded sound %s", m_Init.m_sFileName.c_str());
	return true;
}


//------------------------------------------------------------------------//
// For streaming audio, ServiceBuffer() is invoked five times per second for each
// streaming buffer via a multimedia timer in order to fill and update the content 
// of the circular buffer.
void Sound::ServiceBuffer()
{
	FN("Sound::ServiceBuffer()");

	// Don't allow the function to proceed if the audio manager 
	// is not initialized
	if(!DXAudioMgr()->IsInitialized())
		return;

	// If this flag has been set, it means that we should add this object to
	// the list of those to be removed as soon as the service loop is completed.
	if(m_bRemoveStream)
	{
		m_bRemoveStream = false;

		DXAudioMgr()->RemoveStream(this);

		if(IsPaused())
			return;

		// Check to make sure we actually have a valid source, which should be the case.
		// If not, we'll probably hear some garbage on the next Play() call.
		if(!m_pLoader)
			return;

		// Reset the source to play from the beginning of the file
		m_pLoader->Reset();

		// Reset streaming variables
		m_nBytesPlayed = 0;
		m_nLastReadPos = 0;
		m_nDataCursor = 0;
		if(!m_pDSBuffer)
			return;

		// Make sure the buffer plays from the beginning when it starts up again
		m_pDSBuffer->SetCurrentPosition(0);

		// The next time this buffer plays, we want it set up properly with a 
		// full buffer of valid source data.
		if(!FillBuffer())
			return;

		return;
	}

	// If we don't have a buffer or source, no use continuing
	if(!m_pDSBuffer || !m_pLoader)
		return;

	// Get the current play and write cursors for the buffer
	DWORD dwReadCursor;
	DWORD dwWriteCursor;
	HRESULT hr = m_pDSBuffer->GetCurrentPosition(&dwReadCursor, &dwWriteCursor);

	// Calculate how many bytes have played since the last update call
	if(dwReadCursor > m_nLastReadPos)
		m_nBytesPlayed += dwReadCursor - m_nLastReadPos;
	else
		m_nBytesPlayed += (m_Caps.dwBufferBytes - m_nLastReadPos) + dwReadCursor;

	// Have we played the entire sound?  If so, take appropriate action based on
	// whether we're looping the file or not.
	if(m_nBytesPlayed >= m_pLoader->GetSize())
	{
		if(m_Init.m_bLooping)
		{
			// If we're looping, just start the count back at the beginning
			m_nBytesPlayed -= m_pLoader->GetSize();
		}
		else
		{
			// Otherwise, stop playing and processing the buffer
			Stop();
			return;
		}
	}

	// Calculate how much data can be copied to the buffer this update
	DWORD dwDataToCopy;
	if(m_nDataCursor < dwReadCursor)
		dwDataToCopy = dwReadCursor - m_nDataCursor;
	else
		dwDataToCopy = (m_Caps.dwBufferBytes - m_nDataCursor) + dwReadCursor;

	// No need to allow more than 1/2 of the buffer to be read at a time.  We're
	// reading five times a second, so this should keep up without overtaxing
	// the readers.
	if(dwDataToCopy > (m_Caps.dwBufferBytes / 2))
		dwDataToCopy = m_Caps.dwBufferBytes / 2;

	// Lock the buffer into one or two buffers
	LPVOID  pPtr1; 
	DWORD   dwBytes1; 
	LPVOID  pPtr2; 
	DWORD   dwBytes2; 
	hr = m_pDSBuffer->Lock(m_nDataCursor, dwDataToCopy, &pPtr1, 
		&dwBytes1, &pPtr2, &dwBytes2, 0);
	if(FAILED(hr))
	{
		Error::Handle("Error locking DSound streaming buffer!");
		return;
	}

	// If we're at the end of the wave data...
	if(m_pLoader->IsEOF())
	{
		// Fill the buffer with silence - we're at the end of the file
		memset(pPtr1, GetSilenceData(), dwBytes1);
		if(pPtr2)
			memset(pPtr2, GetSilenceData(), dwBytes2);
		m_nDataCursor += (dwBytes1 + dwBytes2);
	}
	// Otherwise...
	else
	{
		// Fill the buffer with wave data as needed
		uint32 dwBytesRead = 0;
		if(!m_pLoader->Read((unsigned char*)pPtr1, dwBytes1, &dwBytesRead))
		{  Error::Handle("Error reading wave file!");  return;  }
		m_nDataCursor += dwBytesRead;
		if(pPtr2 && (dwBytes1 == dwBytesRead))
		{
			if(!m_pLoader->Read((unsigned char*)pPtr2, dwBytes2, &dwBytesRead))
			{  Error::Handle("Error reading wave file!");  return;  }
			m_nDataCursor += dwBytesRead;
		}
	}

	// Unlock the buffer now that we're done with it
	m_pDSBuffer->Unlock(pPtr1, dwBytes1, pPtr2, dwBytes2);

	// If we want to loop the stream, reset the file to the beginning
	if(m_Init.m_bLooping && m_pLoader->IsEOF())
		m_pLoader->Reset();

	// Loop the write position around if it goes past the end of the buffer
	m_nDataCursor %= m_Caps.dwBufferBytes;

	// Set the last play cursor position for next update calculation
	m_nLastReadPos = dwReadCursor;
}

//------------------------------------------------------------------------//
// This function gets a single byte of "silence" data, which differs depending on
// the number of bits per sample.
uint8 Sound::GetSilenceData()
{
	FN("Sound::GetSilenceData()");
	if(m_WaveFormat.wBitsPerSample == 8)
		return 0x80;
	else if(m_WaveFormat.wBitsPerSample == 16)
		return 0x00;
	return 0;
}


//------------------------------------------------------------------------//
bool Sound::Play()
{
	FN("Sound::Play()");

	// Mark the play time for prioritization
	m_nLastTimePlayed = timeGetTime();

	// Determine if we need to load this buffer before playing.  After the
	// buffer is finished loading, playback will begin automatically.
	if(IsLoading())
	{
		m_bQueuePlayback = true;
		return true;
	}
	else if(!IsLoaded())
	{
		m_bQueuePlayback = true;
		return Load();
	}

	// Verify we actually have a DS buffer at this point
	if(!m_pDSBuffer)
		return false;
	
	// Check to see if we're already playing
	if(IsPlaying() && !m_bQueuePlayback)
		return true;

	// If the sound is streaming, insert this buffer into a list managed
	// by the audio mgr which periodically calls ServiceBuffer()
	if(m_Init.m_bStreaming)
	{
		EnterCriticalSection(&DXAudioMgr()->GetUpdateCS());
		m_bRemoveStream = false;
		DXAudioMgr()->InsertStream(this);
		LeaveCriticalSection(&DXAudioMgr()->GetUpdateCS());
	}

	// Begin playing the buffer.  If the looping flag is set or if the
	// buffer is streaming, begin looping playback
	HRESULT hr;
	if(m_Init.m_bStreaming || m_Init.m_bLooping)
		hr = m_pDSBuffer->Play(0, 0, DSBPLAY_LOOPING);
	else
		hr = m_pDSBuffer->Play(0, 0, 0);
	if(FAILED(hr))
		return false;

	// Set the paused flag to off
	m_bPaused = false;

	// Clear queue play flag
	m_bQueuePlayback = false;

	return true;
}


//------------------------------------------------------------------------//
// Stops playback 
bool Sound::Stop()
{
	FN("Sound::Stop()");

	m_bQueuePlayback = false;

	// If the sound is already stopped (and it's not paused), then just return
	if(!IsPlaying() && !IsPaused())
		return true;

	// Check to make sure we actually have a buffer
	if(!m_pDSBuffer)
		return false;

	// Stop the DirectSound buffer
	m_pDSBuffer->Stop();

	// If the buffer is streaming, we have a bit of extra work to do
	if(!m_Init.m_bStreaming)
	{
		// For an in-memory sound, simply reset the current position to the beginning
		SetReadCursor(0);
	}
	// Flag the sound for removal from the managed stream list.  This flag will
	// tell the stream to remove itself on the next call to ServiceBuffer().
	m_bRemoveStream = true;

	return true;
}


//------------------------------------------------------------------------//
bool Sound::Pause()
{
	FN("Sound::Pause()");

	// Don't allow the function to proceed if the audio manager 
	// is not initialized
	if(!DXAudioMgr()->IsInitialized())
		return false;

	// Ignore this call if the sound is already stopped or paused
	if(!IsPlaying() || IsPaused())
		return false;

	// If the sound is streaming, flag it for removal from the streaming list
	m_bRemoveStream = true;

	// Set the paused flag
	m_bPaused = true;

	// Stop the buffer if it's available
	if(m_pDSBuffer)
		m_pDSBuffer->Stop();

	return true;
}


//------------------------------------------------------------------------//
bool Sound::IsPlaying() const 
{
	FN("Sound::IsPlaying()");
	
    if(m_bQueuePlayback)
		return true;	
	else if(IsLoading())
        return false;	
    else if(m_pDSBuffer)		
    {		
        DWORD dwStatus;		
        m_pDSBuffer->GetStatus(&dwStatus);		

        if(dwStatus & DSBSTATUS_PLAYING)			
            return true;		
    }		

	return false;
}


//------------------------------------------------------------------------//
bool Sound::IsPaused() const
{
	FN("Sound::IsPaused()");
	return m_bPaused;
}


//------------------------------------------------------------------------//
bool Sound::IsLooping() const
{
	FN("Sound::IsLooping()");
	return m_Init.m_bLooping;
}

//------------------------------------------------------------------------//
bool Sound::SetProperties(const SoundProp& prop)
{
	m_Init.m_Prop = prop;
	SetVolume(prop.m_fVolume);
	SetPan(prop.m_fPan);
	SetPitch(prop.m_fPitch);
	return true;
}

//------------------------------------------------------------------------//
bool Sound::GetProperties(SoundProp& prop) const
{
	prop = m_Init.m_Prop;
	return true;
}


//------------------------------------------------------------------------//
bool Sound::SetVolume(float fVolume)
{
	FN("Sound::SetVolume(%f)", fVolume);

	m_Init.m_Prop.m_fVolume = Clamp<float>(fVolume, VOLUME_MIN, VOLUME_MAX);

	if(!m_pDSBuffer)
		return true;

	float fMasterVol;
	if(m_Init.m_bMusic)
		AudioMgr()->GetMusicVolume(fMasterVol);
	else
		AudioMgr()->GetSoundVolume(fMasterVol);
	int32 iVolume = LinearToLogVol(fVolume * fMasterVol);
	HRESULT hr = m_pDSBuffer->SetVolume(iVolume);
	if(FAILED(hr))
		return Error::Handle("Could not set volume.  Error = %s.", DXGetErrorString(hr));

	return true;
}



//------------------------------------------------------------------------//
bool Sound::GetVolume(float& fVolume) const
{
	FN("Sound::GetVolume()");
	fVolume = m_Init.m_Prop.m_fVolume;
	return true;
}


//------------------------------------------------------------------------//
bool Sound::SetPan(float fPan)
{
	FN("Sound::SetPan(%f)", fPan);
	m_Init.m_Prop.m_fPan = Clamp<float>(fPan, PAN_LEFT, PAN_RIGHT);

	// Panning is not allowed for sounds with particular characteristics
	if(m_b3DSound || (m_WaveFormat.nChannels != 1))
		return false;
	
	if(m_pDSBuffer)
	{
		HRESULT hr = m_pDSBuffer->SetPan(static_cast<int32>(m_Init.m_Prop.m_fPan * 10000.0f));
		if(FAILED(hr))
			return Error::Handle("Could not set pan.  Error = %s.", DXGetErrorString(hr));
	}
	return true;
}


//------------------------------------------------------------------------//
bool Sound::GetPan(float& fPan) const
{
	FN("Sound::GetPan()");
	fPan = m_Init.m_Prop.m_fPan;
	return true;
}


//------------------------------------------------------------------------//
bool Sound::SetPitch(float fPitch)
{
	FN("Sound::SetPitch(%f)", fPitch);
	m_Init.m_Prop.m_fPitch = fPitch;
	if(!SetFrequency(uint32(float(m_WaveFormat.nSamplesPerSec) * m_Init.m_Prop.m_fPitch)))
		return false;
	return true;
}


//------------------------------------------------------------------------//
bool Sound::GetPitch(float& fPitch) const
{
	FN("Sound::GetPitch()");
	fPitch = m_Init.m_Prop.m_fPitch;
	return true;
}


//------------------------------------------------------------------------//
bool Sound::SetFrequency(uint32 nFrequency)
{
	FN("Sound::SetFrequency(%d)", nFrequency);

	if(m_pDSBuffer)
	{
		nFrequency = Clamp<uint32>(nFrequency, DXAudioMgr()->GetCaps().dwMinSecondarySampleRate,
			DXAudioMgr()->GetCaps().dwMaxSecondarySampleRate);
		HRESULT hr = m_pDSBuffer->SetFrequency(nFrequency);
		if(FAILED(hr))
			return Error::Handle("Could not set frequency.  Error = %s.", DXGetErrorString(hr));
	}
	return true;
}


//------------------------------------------------------------------------//
bool Sound::SetReadCursor(uint32 nBytes)
{
	FN("Sound::SetReadCursor(%d)", nBytes);
	// Note : Jumping to an arbitrary point in a stream is currently not supported,
	// but could be added later if desired.
	if(m_Init.m_bStreaming)
		return Error::Handle("Can not perform this operation on an audio stream");
	m_Init.m_Prop.m_nReadCursor = nBytes;
	if(m_pDSBuffer)
	{
		HRESULT hr = m_pDSBuffer->SetCurrentPosition(m_Init.m_Prop.m_nReadCursor);
		if(FAILED(hr))
			return Error::Handle("Count not set position.  Error = %s.", DXGetErrorString(hr));
	}
	return true;
}


//------------------------------------------------------------------------//
bool Sound::GetReadCursor(uint32& nBytes) const
{
	FN("Sound::GetReadCursor()");
	if(m_Init.m_bStreaming)
	{
		nBytes = m_nBytesPlayed;
	}
	else
	{
		nBytes = m_Init.m_Prop.m_nReadCursor;
		if(m_pDSBuffer)
		{
			HRESULT hr = m_pDSBuffer->GetCurrentPosition(&nBytes, NULL);
			if(FAILED(hr))
				return Error::Handle("Could not get position.  Error = %s.", DXGetErrorString(hr));
		}
	}
	return true;
}


//------------------------------------------------------------------------//
bool Sound::GetSourceSize(uint32& nBytes) const
{
	FN("Sound::GetSourceSize()");
	nBytes = m_nSourceSize;
	return true;
}


//------------------------------------------------------------------------//
uint32 Sound::GetLastPlayTime() const
{
	if(IsPlaying())
		return 0xFFFFFFFF;
	return m_nLastTimePlayed;
}


//------------------------------------------------------------------------//
bool Sound::operator < (const Sound& snd) const
{
	int iScore = 0;

	// We compare three criteria in this priority test:
	// user-defined priority , current play status, and 
	// the last time played.
	if(m_Init.m_nPriority < snd.m_Init.m_nPriority)
		iScore--;
	else if(m_Init.m_nPriority > snd.m_Init.m_nPriority)
		iScore++;

	if(IsPlaying())
		iScore++;
	if(snd.IsPlaying())
		iScore--;

	if(GetLastPlayTime() < snd.GetLastPlayTime())
		iScore--;
	else if(GetLastPlayTime() > snd.GetLastPlayTime())
		iScore++;

	return (iScore < 0) ? true : false;
}


//------------------------------------------------------------------------//
// Generic property support (for driver-specific extensions)
bool Sound::QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport)
{
	FN("Sound::QuerySupport()");
	return m_PropertySet.QuerySupport(guid, nID, pTypeSupport);
}


//------------------------------------------------------------------------//
bool Sound::Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
	uint32 nPropLength, uint32* pBytesReturned)
{
	FN("Sound::Get()");
	return m_PropertySet.Get(guidProperty, nID, pInstanceData, nInstanceLength,
		pPropData, nPropLength, pBytesReturned);
}


//------------------------------------------------------------------------//
bool Sound::Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
	uint32 nInstanceLength, void* pPropData, uint32 nPropLength,
	bool bStoreProperty)
{
	FN("Sound::Set()");
	return m_PropertySet.Set(guidProperty, nID, pInstanceData, nInstanceLength, 
		pPropData, nPropLength, bStoreProperty);
}





