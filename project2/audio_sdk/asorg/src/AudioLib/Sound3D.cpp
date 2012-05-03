/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "Sound3D.h"
#include "AudioCommon.h"
#include "AudioMgr.h"

using namespace std;
using namespace Audio;

IMPLEMENT_POOL(Sound3D);

//------------------------------------------------------------------------//
Sound3D::Sound3D()
{
	FN("Sound3D::Sound3D()");
	Clear();
}


//------------------------------------------------------------------------//
Sound3D::~Sound3D()
{
	FN("Sound3D::~Sound3D()");
	Term();
}


//------------------------------------------------------------------------//
void Sound3D::Clear()
{
	FN("Sound3D::Clear()");

	m_p3DBuffer = 0;	
	m_PropertySet.Clear();
	m_pSound = 0;
	m_bInitialized = false;
	m_Init.Clear();
	m_bQueuePlayback = false;
	m_bLoading = false;
	m_bLoaded = false;
}



//------------------------------------------------------------------------//
bool Sound3D::Init(const Sound3DInit& init)
{
	FN("Sound3D::Init()");

	// Set the audio definition
	m_Init = init;
	SoundInit sndinit;
	sndinit.m_sFileName = m_Init.m_sFileName;
	sndinit.m_bStreaming = m_Init.m_bStreaming;
	sndinit.m_bLooping = m_Init.m_bLooping;
	sndinit.m_nPriority = 0xFFFFFFFF;
	sndinit.m_Prop.m_fVolume = m_Init.m_Prop.m_fVolume;
	sndinit.m_Prop.m_fPitch = m_Init.m_Prop.m_fPitch;
	sndinit.m_pPackage = m_Init.m_pPackage;

	if(!DXAudioMgr()->CreateSound((ISound*&)m_pSound))
		return false;

	if(!m_pSound->Init(sndinit))
		return false;
	m_pSound->m_b3DSound = true;

	m_bInitialized = true;

	return true;
}


//------------------------------------------------------------------------//
void Sound3D::Term()
{
	FN("Sound3D::Term()");

	// Unload the sound
	Unload();
	
	m_PropertySet.Term();

	// Terminate the sound buffer object
	if(m_pSound)
	{
		m_pSound->Term();
		m_pSound = 0;
	}

	Clear();
}


//------------------------------------------------------------------------//
void Sound3D::Destroy()
{
	FN("Sound3D::Destroy()");
	Term();
	Sound3D::DestroyObject(this);
}


//------------------------------------------------------------------------//
bool Sound3D::Load()
{
	FN("Sound3D::Load()");

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
bool Sound3D::DoLoad()
{
	FN("Sound3D::DoLoad()");

	// Don't allow the function to proceed if the audio manager 
	// is not initialized
	if(!DXAudioMgr()->IsInitialized())
		return false;

	// Make sure we don't reload the sound if it's already loaded
	if(IsLoaded())
		return true;

	// Make sure we have a valid sound object to work with.
	if(!m_pSound)
		return false;

	// Ensure we're not over the buffer limit
	if(!DXAudioMgr()->CanAddSound3D())
		DXAudioMgr()->RemoveSound3D(this);
	
	if(!m_pSound->DoLoad())
		return false;

	// Get the 3d sound buffer interface
	HRESULT hr = m_pSound->m_pDSBuffer->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&m_p3DBuffer);
	if(FAILED(hr))
	{
		m_pSound->Unload();
		return Error::Handle("Could not get 3d sound interface.  Error = %s.", DXGetErrorString(hr));
	}

	// Retrieve and (re)load all property sets for this buffer
	m_PropertySet.OnLoad(m_p3DBuffer);
	
	// Insert the 3d sound into the loaded list
	DXAudioMgr()->OnLoadSound3D(this);

	// Set all 3d properties for this buffer
	if(!SetProperties(m_Init.m_Prop))
		return false;

	m_bLoading = false;
	m_bLoaded = true;

	if(m_bQueuePlayback)
		Play();

	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::Unload()
{
	FN("Sound3D::Unload()");

	// Don't allow the function to proceed if the audio manager 
	// is not initialized
	if(!DXAudioMgr()->IsInitialized())
		return false;

	// Make sure the sound isn't already unloaded
	if(!IsLoaded())
		return true;

	// Wait until a sound is completely loaded before unloading it
	while(IsLoading());

	// Stop the buffer before unloading
	Stop();

	// Safely release the 3d buffer and property set interfaces	
	m_PropertySet.OnUnload();
	_XASAFE_RELEASE(m_p3DBuffer);

	// Unlock and unload the sound buffer
	if(m_pSound)
		m_pSound->Unload();

	// Remove the sound from the loaded list
	DXAudioMgr()->OnUnloadSound3D(this);

	// Mark the sound as unloaded
	m_bLoaded = false;

	DebugOut(1, "Unloaded sound %s", m_Init.m_sFileName.c_str());
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::Play()
{
	FN("Sound3D::Play()");

		// Mark the play time for prioritization
	if(m_pSound)
		m_pSound->m_nLastTimePlayed = timeGetTime();

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

	// Make sure we have a valid sound buffers
	if(!m_pSound)
		return false;

	return m_pSound->Play();
}


//------------------------------------------------------------------------//
bool Sound3D::Stop()
{
	FN("Sound3D::Stop()");

	m_bQueuePlayback = false;

	m_Init.m_Prop.m_nReadCursor = 0;
	if(!m_pSound)
		return false;
	return m_pSound->Stop();
}


//------------------------------------------------------------------------//
bool Sound3D::Pause()
{
	FN("Sound3D::Pause()");
	if(!m_pSound)
		return false;
	return m_pSound->Pause();
}


//------------------------------------------------------------------------//
bool Sound3D::IsPlaying() const
{
	FN("Sound3D::IsPlaying()");
	if(m_bQueuePlayback)
		return true;
	else if(IsLoading())
		return false;
	else if(m_pSound && m_pSound->IsPlaying())
		return true;
	return false;
}


//------------------------------------------------------------------------//
bool Sound3D::IsPaused() const
{
	FN("Sound3D::IsPaused()");
	if(!m_pSound)
		return false;
	return m_pSound->IsPaused();
}


//------------------------------------------------------------------------//
bool Sound3D::IsLooping() const
{
	FN("Sound3D::IsLooping()");
	return m_Init.m_bLooping;
}


//------------------------------------------------------------------------//
bool Sound3D::SetVolume(float fVolume)
{
	FN("Sound3D::SetVolume(%f)", fVolume);
	m_Init.m_Prop.m_fVolume = fVolume;
	if(m_pSound)
		return m_pSound->SetVolume(fVolume);
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::GetVolume(float& fVolume) const
{
	FN("Sound3D::GetVolume()");
	fVolume = m_Init.m_Prop.m_fVolume;
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::SetPitch(float fPitch)
{
	FN("Sound3D::SetPitch(%f)", fPitch);
	m_Init.m_Prop.m_fPitch = fPitch;
	if(m_pSound)
		return m_pSound->SetPitch(fPitch);
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::GetPitch(float& fPitch) const
{
	FN("Sound3D::GetPitch()");
	fPitch = m_Init.m_Prop.m_fPitch;
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::SetReadCursor(uint32 nBytes)
{
	FN("Sound3D::SetReadCursor(%d)", nBytes);
	m_Init.m_Prop.m_nReadCursor = nBytes;
	if(m_pSound)
		return m_pSound->SetReadCursor(nBytes);
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::GetReadCursor(uint32& nBytes) const
{
	FN("Sound3D::GetCurrentPos()");
	if(!m_pSound)
		return false;
	return m_pSound->GetReadCursor(nBytes);
}


//------------------------------------------------------------------------//
bool Sound3D::GetSourceSize(uint32& nBytes) const
{
	FN("Sound3D::GetSourceSize()");
	if(!m_pSound)
		return false;
	return m_pSound->GetSourceSize(nBytes);
}


//------------------------------------------------------------------------//
bool Sound3D::SetProperties(const Sound3DProp& prop)
{
	FN("Sound3D::Set3DProperties()");
	m_Init.m_Prop = prop;

	SetVolume(m_Init.m_Prop.m_fVolume);
	SetPitch(m_Init.m_Prop.m_fPitch);
	SetReadCursor(m_Init.m_Prop.m_nReadCursor);

	if(m_p3DBuffer)
	{
		DS3DBUFFER dsprop;
		memset(&dsprop, 0, sizeof(DS3DBUFFER));
		dsprop.dwSize = sizeof(DS3DBUFFER);
		dsprop.vPosition = m_Init.m_Prop.m_vPosition;
		dsprop.vVelocity = m_Init.m_Prop.m_vVelocity;
		dsprop.vConeOrientation = m_Init.m_Prop.m_vConeOrientation;
		dsprop.dwInsideConeAngle = m_Init.m_Prop.m_nInsideConeAngle;
		dsprop.dwOutsideConeAngle = m_Init.m_Prop.m_nOutsideConeAngle;
		dsprop.flMinDistance = m_Init.m_Prop.m_fMinDistance;
		dsprop.flMaxDistance = m_Init.m_Prop.m_fMaxDistance;
		dsprop.lConeOutsideVolume = LinearToLogVol(m_Init.m_Prop.m_fConeOutsideVolume);
		dsprop.dwMode = m_Init.m_Prop.m_nMode;
		HRESULT hr = m_p3DBuffer->SetAllParameters(&dsprop, DS3D_DEFERRED);
		if(FAILED(hr))
			return Error::Handle("Could not set 3d parameters.  Error = %s.", DXGetErrorString(hr));
	}
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::GetProperties(Sound3DProp& prop) const
{
	FN("Sound3D::Get3DProperties()");
	prop = m_Init.m_Prop;
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::SetPosition(const AUDIOVECTOR& vPosition)
{
	FN("Sound3D::SetPosition()");
	m_Init.m_Prop.m_vPosition = vPosition;
	if(m_p3DBuffer)
	{
		HRESULT hr = m_p3DBuffer->SetPosition(
			m_Init.m_Prop.m_vPosition.x,
			m_Init.m_Prop.m_vPosition.y,
			m_Init.m_Prop.m_vPosition.z,
			DS3D_DEFERRED);
		if(FAILED(hr))
			return Error::Handle("Could not set position.  Error = %s.", DXGetErrorString(hr));
	}
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::GetPosition(AUDIOVECTOR& vPosition) const
{
	FN("Sound3D::GetPosition()");
	vPosition = m_Init.m_Prop.m_vPosition;
	return true;
}



//------------------------------------------------------------------------//
bool Sound3D::SetVelocity(const AUDIOVECTOR& vVelocity)
{
	FN("Sound3D::SetVelocity()");
	m_Init.m_Prop.m_vVelocity = vVelocity;
	if(m_p3DBuffer)
	{
		HRESULT hr = m_p3DBuffer->SetVelocity(
			m_Init.m_Prop.m_vVelocity.x,
			m_Init.m_Prop.m_vVelocity.y,
			m_Init.m_Prop.m_vVelocity.z,
			DS3D_DEFERRED);
		if(FAILED(hr))
			return Error::Handle("Could not set 3d velocity.  Error = %s.", DXGetErrorString(hr));
	}
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::GetVelocity(AUDIOVECTOR& vVelocity) const
{
	FN("Sound3D::GetVelocity()");
	vVelocity = m_Init.m_Prop.m_vVelocity;
	return true;
}



//------------------------------------------------------------------------//
bool Sound3D::SetMaxDistance(float fMaxDist)
{
	FN("Sound3D::SetMaxDistance()");
	m_Init.m_Prop.m_fMaxDistance = fMaxDist;
	if(m_p3DBuffer)
	{
		HRESULT hr = m_p3DBuffer->SetMaxDistance(m_Init.m_Prop.m_fMaxDistance, DS3D_DEFERRED);
		if(FAILED(hr))
			return Error::Handle("Could not set 3d max distance.  Error = %s.", DXGetErrorString(hr));
	}
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::GetMaxDistance(float& fMaxDist) const
{
	FN("Sound3D::GetMaxDistance()");
	fMaxDist = m_Init.m_Prop.m_fMaxDistance;
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::SetMinDistance(float fMinDist)
{
	FN("Sound3D::SetMinDistance()");
	m_Init.m_Prop.m_fMinDistance = fMinDist;
	if(m_p3DBuffer)
	{
		
		HRESULT hr = m_p3DBuffer->SetMinDistance(m_Init.m_Prop.m_fMinDistance, DS3D_DEFERRED);
		if(FAILED(hr))
			return Error::Handle("Could not set 3d min distance.  Error = %s.", DXGetErrorString(hr));
	}
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::GetMinDistance(float& fMinDist) const
{
	FN("Sound3D::GetMinDistance()");
	fMinDist = m_Init.m_Prop.m_fMinDistance;
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::SetConeAngles(uint32 nInside, uint32 nOutside)
{
	FN("Sound3D::SetConeAngles()");
	if(m_p3DBuffer)
	{
		HRESULT hr = m_p3DBuffer->SetConeAngles(
			m_Init.m_Prop.m_nInsideConeAngle,
			m_Init.m_Prop.m_nOutsideConeAngle,
			DS3D_DEFERRED);
		if(FAILED(hr))
			return Error::Handle("Could not set 3d cone orientation.  Error = %s.", DXGetErrorString(hr));
	}
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::GetConeAngles(uint32& nInside, uint32& nOutside) const
{
	FN("Sound3D::GetConeAngles()");
	nInside = m_Init.m_Prop.m_nInsideConeAngle;
	nOutside = m_Init.m_Prop.m_nOutsideConeAngle;
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::SetConeOrientation(const AUDIOVECTOR& vOrientation)
{
	FN("Sound3D::SetConeOrientation()");
	if(m_p3DBuffer)
	{
		HRESULT hr = m_p3DBuffer->SetConeOrientation(
			m_Init.m_Prop.m_vConeOrientation.x,
			m_Init.m_Prop.m_vConeOrientation.y,
			m_Init.m_Prop.m_vConeOrientation.z,
			DS3D_DEFERRED);
		if(FAILED(hr))
			return Error::Handle("Could not set 3d cone orientation.  Error = %s.", DXGetErrorString(hr));
	}		
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::GetConeOrientation(AUDIOVECTOR& vOrientation) const
{
	FN("Sound3D::GetConeOrientation()");
	vOrientation = m_Init.m_Prop.m_vConeOrientation;
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::SetConeOutsideVolume(float fVolume)
{
	FN("Sound3D::SetConeOutsideVolume()");
	m_Init.m_Prop.m_fConeOutsideVolume = fVolume;
	if(m_p3DBuffer)
	{
		HRESULT hr = m_p3DBuffer->SetConeOutsideVolume(LinearToLogVol(fVolume), DS3D_DEFERRED);
		if(FAILED(hr))
			return Error::Handle("Could not set 3d cone outside volume.  Error = %s.", DXGetErrorString(hr));
	}
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::GetConeOutsideVolume(float& fVolume) const
{
	FN("Sound3D::GetConeOutsideVolume()");
	fVolume = m_Init.m_Prop.m_fConeOutsideVolume;
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::SetMode(uint32 nMode)
{
	FN("Sound3D::SetMode()");
	if(m_p3DBuffer)
	{
		HRESULT hr = m_p3DBuffer->SetMode(nMode, DS3D_DEFERRED);
		if(FAILED(hr))
			return Error::Handle("Could not set 3d mode.  Error = %s.", DXGetErrorString(hr));
	}		
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::GetMode(uint32& nMode) const
{
	FN("Sound3D::GetMode()");
	nMode = m_Init.m_Prop.m_nMode;
	return true;
}


//------------------------------------------------------------------------//
bool Sound3D::operator < (const Sound3D& snd) const
{
	int iScore = 0;

	// We compare five criteria in this priority test:
	// user-defined priority, current play status, 
	// last time played, distance past max, and
	// distance to listener
	if(m_Init.m_nPriority < snd.m_Init.m_nPriority)
		iScore--;
	else if(m_Init.m_nPriority > snd.m_Init.m_nPriority)
		iScore++;

	if(IsPlaying())
		iScore++;
	if(snd.IsPlaying())
		iScore--;

	if(m_pSound->GetLastPlayTime() < snd.m_pSound->GetLastPlayTime())
		iScore--;
	else if(m_pSound->GetLastPlayTime() > snd.m_pSound->GetLastPlayTime())
		iScore++;

	AUDIOVECTOR vLP, vP1, vP2;
	float fDist1, fDist2;
	float fMaxDist;
	IListener* pListener;
	AudioMgr()->GetListener(pListener);
	pListener->GetPosition(vLP);
	GetPosition(vP1);
	snd.GetPosition(vP2);
	fDist1 = sqrt((vLP.x - vP1.x) * (vLP.x - vP1.x) + 
		(vLP.y - vP1.y) * (vLP.y - vP1.y) + 
		(vLP.z - vP1.z) * (vLP.z - vP1.z));
	fDist2 = sqrt((vLP.x - vP2.x) * (vLP.x - vP2.x) + 
		(vLP.y - vP2.y) * (vLP.y - vP2.y) + 
		(vLP.z - vP2.z) * (vLP.z - vP2.z));

	GetMaxDistance(fMaxDist);
	if(fDist1 >= fMaxDist)
		iScore -= 2;
	snd.GetMaxDistance(fMaxDist);
	if(fDist2 >= fMaxDist)
		iScore += 2;

	if(fDist1 > fDist2)
		iScore--;
	else
		iScore++;

	return (iScore < 0) ? true : false;
}

//------------------------------------------------------------------------//
// Generic property support (for driver-specific extensions)
bool Sound3D::QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport)
{
	FN("Sound3D::QuerySupport()");
	return m_PropertySet.QuerySupport(guid, nID, pTypeSupport);
}


//------------------------------------------------------------------------//
bool Sound3D::Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
	uint32 nPropLength, uint32* pBytesReturned)
{
	FN("Sound3D::Get()");
	return m_PropertySet.Get(guidProperty, nID, pInstanceData, nInstanceLength, pPropData, 
		nPropLength, pBytesReturned);
}


//------------------------------------------------------------------------//
bool Sound3D::Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
	uint32 nInstanceLength, void* pPropData, uint32 nPropLength, bool bStoreProperty)
{
	FN("Sound3D::Set()");
	return m_PropertySet.Set(
		guidProperty, 
		nID, 
		pInstanceData, 
		nInstanceLength, 
		pPropData, 
		nPropLength,
		bStoreProperty);
}


