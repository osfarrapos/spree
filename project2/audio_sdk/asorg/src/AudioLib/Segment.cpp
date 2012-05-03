/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "Segment.h"
#include "AudioCommon.h"
#include "AudioMgr.h"

using namespace std;
using namespace Audio;

IMPLEMENT_POOL(Segment);

//------------------------------------------------------------------------//
Segment::Segment()
{
	FN("Segment::Segment()");
	Clear();
}


//------------------------------------------------------------------------//
Segment::~Segment()
{
	FN("Segment::~Segment()");
	Term();
}


//------------------------------------------------------------------------//
void Segment::Clear()
{
	FN("Segment::Clear()");
	m_pSegment = NULL;
	m_pSegState = NULL;
	m_Init.Clear();
	m_bInitialized = false;
	m_bPaused = false;
	m_b3DSegment = false;
	m_iPlayStartTime = 0;
	m_iPauseTime = 0;

	m_nLastTimePlayed = 0;

	m_bQueuePlayback = false;
	m_bLoading = false;
	m_bLoaded = false;
}



//------------------------------------------------------------------------//
bool Segment::Init(const SegmentInit& init)
{
	FN("Segment::Init()");

	// Set the audio definition
	m_Init = init;

	m_bInitialized = true;

	return true;
}


//------------------------------------------------------------------------//
void Segment::Term()
{
	FN("Segment::Term()");
	Unload();
}


//------------------------------------------------------------------------//
void Segment::Destroy()
{
	FN("Segment::Destroy()");
	Term();
	Segment::DestroyObject(this);
}

//------------------------------------------------------------------------//
bool Segment::Load()
{
	FN("Segment::Load()");
	
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
bool Segment::DoLoad()
{

	// Don't allow the function to proceed if the audio manager 
	// is not initialized
	if(!DXAudioMgr()->IsInitialized())
		return false;
	
	// If already loaded, just return success
	if(IsLoaded())
		return true;
	
	// Check to see if we're allowed to create another segment
	if(!DXAudioMgr()->CanAddSegment())
	{
		// If not, attempt to remove one segment to make room for it
		if(!DXAudioMgr()->RemoveSegment(this))
			return false;
	}
	
	IAudioStream* pStream;
	CreateAudioStream cas(pStream);
	if(!pStream)
		return false;
	
	HRESULT hr = pStream->Open(m_Init.m_sFileName);
	if(FAILED(hr))
		return Error::Handle("Failed to open segment file %s.", m_Init.m_sFileName.c_str());
	
	DMUS_OBJECTDESC      ObjDesc;
	ZeroMemory(&ObjDesc, sizeof(DMUS_OBJECTDESC));
	ObjDesc.dwSize = sizeof(DMUS_OBJECTDESC);
	ObjDesc.guidClass = CLSID_DirectMusicSegment;
	ObjDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_STREAM;
	ObjDesc.pStream = pStream;
	
	// Load the data and retrieve the segment interface
	hr = DXAudioMgr()->Loader()->GetObject(
		&ObjDesc, 
		IID_IDirectMusicSegment8, 
		(void**) &m_pSegment );
	if(FAILED(hr))
	{	
		Unload();
		return Error::Handle("Could not load audio segment %s. Error = %s.", 
			m_Init.m_sFileName.c_str(), DXGetErrorString(hr));
	}
	
	// We must release the object from the cache since we are reusing
	// the same IStream object to load different data.  This will otherwise
	// confuse the cache system, resulting in errors.
	hr = DXAudioMgr()->Loader()->ReleaseObjectByUnknown(m_pSegment);
	
	// Instruct the loader to clear out unused memory.
	DXAudioMgr()->Loader()->CollectGarbage();
	
	// Download it
	hr = m_pSegment->Download(DXAudioMgr()->Performance(m_Init.m_bMusic));
	if(FAILED(hr))
	{
		Unload();
		return Error::Handle("Could not download audio segment %s. Error = %s.", 
			m_Init.m_sFileName.c_str(), DXGetErrorString(hr));
	}
	
	// Insert the loaded segment into the audio list for management
	DXAudioMgr()->OnLoadSegment(this);
	
	// Lock the segment's associate DLS file, if it exists
	if(m_Init.m_pDLS)
		m_Init.m_pDLS->Lock();
	
	// Set load status and play if needed
	m_bLoading = false;
	m_bLoaded = true;
	
	if(m_bQueuePlayback)
		Play();
	
	return true;
}

//------------------------------------------------------------------------//
bool Segment::Unload()
{
	FN("Segment::Unload()");
	
	// Don't allow the function to proceed if the audio manager 
	// is not initialized
	if(!DXAudioMgr()->IsInitialized())
		return false;
	
	if(!IsLoaded())
		return true;
	
	// Wait until a sound is completely loaded before unloading it
	while(IsLoading());
	
	if(!Stop())
	{  DebugOut(1, "Error stopping sound segment before unloading.");  }
	
	// Release the current segment state
	_XASAFE_RELEASE(m_pSegState);
	
	if(m_pSegment)
	{
		HRESULT hr;
		hr = m_pSegment->Unload(DXAudioMgr()->Performance(m_Init.m_bMusic));
		if(FAILED(hr))
		{  Error::Handle("Error unloading sound segment from performance.");  }
		
		// Unload the associated DLS file, or at least reduce the ref count
		if(m_Init.m_pDLS)
			m_Init.m_pDLS->Unlock();
		
		// Since we're loading and unloading segments dynamically, it is 
		// important to release the loader's internal reference to it.
		//hr = DXAudioMgr()->Loader()->ReleaseObjectByUnknown(m_pSegment);
		if(FAILED(hr))
		{  Error::Handle("Error unloading sound segment from loader.");  }
		
		// Remove the segment from the master list
		DXAudioMgr()->OnUnloadSegment(this);
		
		// Instruct the loader to clear out unused memory.
		//DXAudioMgr()->Loader()->CollectGarbage();
		
		// Now release the actual segment
		_XASAFE_RELEASE(m_pSegment);
	}
	
	// Mark the segment as unloaded
	m_bLoaded = false;
	
	return true;
}

//------------------------------------------------------------------------//
/*
Segment::Play() actually performs two actions.  If the segment is a non
musical segment, it simply returns the result of Segment::PlaySegment(),
which is a straightforward segment playing function.  If it is a musical
segment, however, Play() attempts to properly place the segment as the
next segment to play if there is one playing immediately, or will start
playing immediately if no segments are currently playing.
*/
bool Segment::Play()
{
	FN("Segment::Play()");

	// Get the time played for prioritization purposes
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
	// Clear queue play flag
	m_bQueuePlayback = false;

	// If this is a non-musical segment, play it immediately as a secondary segment
	if(!m_Init.m_bMusic)
		return DoPlay();

	// If there is a current segment, but it is stopped, clear it out in
	// preparation for the next test.
	if(DXAudioMgr()->GetCurrentSegment() && !DXAudioMgr()->GetCurrentSegment()->IsPlaying())
		DXAudioMgr()->SetCurrentSegment(0);
	
	// If there is no current segment playing, set this segment
	// as current and begin playing immediately.
	if(!DXAudioMgr()->GetCurrentSegment())
	{
		DXAudioMgr()->SetCurrentSegment(this);
		DXAudioMgr()->SetNextSegment(0);
		return DoPlay();
	}
	// otherwise, queue this segment for playing after the current
	// segment is finished.
	else
	{
		DXAudioMgr()->SetNextSegment(this);
	}
	

	return true;
}

//------------------------------------------------------------------------//
// DoPlay() is used to actually start the segment playing.  The Play() 
// function, in contrast, may simply queue a segment for playing next.
bool Segment::DoPlay()
{
	FN("Segment::DoPlay()");

	if(!m_pSegment)
		return false;

	// Set the flags based on the type of content (music or sound fx)
	DWORD dwFlags = 0;
	if(m_Init.m_bMusic)
		dwFlags = DMUS_SEGF_QUEUE | DMUS_SEGF_DEFAULT;
	else
		dwFlags = DMUS_SEGF_SECONDARY;

	// If paused, start from the paused time
	if(m_bPaused)
		m_pSegment->SetStartPoint(m_iPauseTime);

	// Now play the segment
	IDirectMusicSegmentState* pSegState = 0;
	HRESULT hr;
    hr = DXAudioMgr()->Performance(m_Init.m_bMusic)->PlaySegmentEx(
        m_pSegment,						// Segment to play.
        NULL,							// Used for songs; not implemented.
        NULL,							// For transitions. 
        dwFlags,						// Flags.
        0,								// Start time; 0 is immediate.
        &pSegState,						// Pointer that receives segment state.
        NULL,							// Object to stop.
        NULL							// Use default audiopath
    );  
	if(FAILED(hr))
		return Error::Handle("Could not play audio segment %s. Error = %s.", 
			m_Init.m_sFileName.c_str(), DXGetErrorString(hr));

	// Reset the new start time to the beginning of the segment
	if(m_bPaused)
	{
		m_iPauseTime = 0;
		m_pSegment->SetStartPoint(m_iPauseTime);
	}

	// Not paused any more...
	m_bPaused = false;

	// Get the segment state object
	if(!pSegState)
		return Error::Handle("Could not get segment state.  Some functions may not work");
	hr = pSegState->QueryInterface(IID_IDirectMusicSegmentState8, (void**)&m_pSegState);
	if(FAILED(hr))
		return Error::Handle("Could not get segment state.  Some functions may not work");

	return true;
}


//------------------------------------------------------------------------//
bool Segment::Stop()
{
	FN("Segment::Stop()");

	m_bQueuePlayback = false;

	// If the sound is already stopped (and it's not paused), then just return
	if(!IsPlaying() && !IsPaused())
		return true;

	// Stop the segment if it is available
	if(m_pSegment)
		DXAudioMgr()->Performance(m_Init.m_bMusic)->StopEx(m_pSegment, 0, 0);

	if(m_Init.m_bMusic)
	{
		DXAudioMgr()->SetCurrentSegment(0);
		DXAudioMgr()->SetNextSegment(0);
	}

	// Release the current segment state
	_XASAFE_RELEASE(m_pSegState);

	return true;
}


//------------------------------------------------------------------------//
bool Segment::Pause()
{
	FN("Segment::Pause()");
	if(IsPlaying())
	{
		m_bPaused = true;
		if(m_pSegState)
			m_pSegState->GetSeek(&m_iPauseTime);

		if(m_pSegment)
			DXAudioMgr()->Performance(m_Init.m_bMusic)->StopEx(m_pSegment, 0, 0);

		if(m_Init.m_bMusic)
		{
			DXAudioMgr()->SetCurrentSegment(0);
			DXAudioMgr()->SetNextSegment(0);
		}

	}

	// Release the current segment state
	_XASAFE_RELEASE(m_pSegState);

	return true;
}


//------------------------------------------------------------------------//
bool Segment::IsPlaying() const
{
	FN("Segment::IsPlaying()");
	if(m_bQueuePlayback)
		return true;
	else if(IsLoading())
		return false;
	else if(m_pSegment)
	{
		HRESULT hr = DXAudioMgr()->Performance(m_Init.m_bMusic)->IsPlaying(m_pSegment, NULL);
		if(FAILED(hr))
			return Error::Handle("Could not check playing status for segment.  Error = %s", DXGetErrorString(hr));
		if(hr == S_OK)
			return true;
	}
	return false;
}

//------------------------------------------------------------------------//
bool Segment::IsPaused() const
{
	FN("Segment::IsPaused()");
	return m_bPaused;
}


//------------------------------------------------------------------------//
bool Segment::IsLooping() const
{
	FN("Segment::IsLooping()");
	return m_Init.m_bLooping;
}

//------------------------------------------------------------------------//
uint32 Segment::GetLastPlayTime() const
{
	if(IsPlaying())
		return 0xFFFFFFFF;
	return m_nLastTimePlayed;
}

//------------------------------------------------------------------------//
bool Segment::operator < (const Segment& seg) const
{
	// The only criteria we compare for segments is
	// the last time played
	if(GetLastPlayTime() < seg.GetLastPlayTime())
		return true;
	return false;
}


// Property sets are not supported in this version of the library for segments

//------------------------------------------------------------------------//
// Generic property support (for driver-specific extensions)
bool Segment::QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport)
{
	FN("Segment::QuerySupport()");
	return false;
}

//------------------------------------------------------------------------//
bool Segment::Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
	uint32 nPropLength, uint32* pBytesReturned)
{
	FN("Segment::Get()");
	return false;
}

//------------------------------------------------------------------------//
bool Segment::Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
	uint32 nPropLength, bool bStoreProperty)
{
	FN("Segment::Set()");
	return false;
}







