/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "SoundScape.h"
#include "SoundMgr.h"

using namespace Audio;


IMPLEMENT_POOL(SoundScape);


//------------------------------------------------------------------------//
SoundScape::SoundScape()
{
	FN("SoundScape::SoundScape()");
	Clear();
}

//------------------------------------------------------------------------//
SoundScape::~SoundScape()
{
	FN("SoundScape::~SoundScape()");
	Term();
}

//------------------------------------------------------------------------//
void SoundScape::Term()
{
	FN("SoundScape::Term()");
	Stop();
	Unload();

	CRITICAL_FUNCTION(&CSoundMgr()->GetUpdateCS());
	int i;
	for(i = 0; i < m_aBackground.size(); i++)
		m_aBackground[i].m_Init.m_pSound->Destroy();
	for(i = 0; i < m_aPeriodic.size(); i++)
		m_aPeriodic[i].m_Init.m_pSound3D->Destroy();
	m_aBackground.clear();
	m_aPeriodic.clear();

	Clear();
}

//------------------------------------------------------------------------//
void SoundScape::Destroy()
{
	FN("SoundScape::Destroy()");
	Term();
	SoundScape::DestroyObject(this);
}




//------------------------------------------------------------------------//
void SoundScape::Clear()
{
	FN("SoundScape::Clear()");
	m_bInitialized = false;
	m_bLoaded = false;
	m_bPlaying = false;
	m_bPaused = false;
	m_aBackground.clear();
	m_aPeriodic.clear();
	m_bFirstUpdate = false;
	m_fCurrentTime = 0.0f;
	m_fVolume = 1.0f;
	m_bVolumeChanged = false;
}


//------------------------------------------------------------------------//
bool SoundScape::Init()
{
	FN("SoundScape::Init()");
	m_bInitialized = true;
	m_bFirstUpdate = true;
	// You may want to trim this down if you have a LOT of soundscapes to create,
	// but it's probably best initially to try to avoid additional allocations.
	// You'll need an awful lot of them before it really makes a dent in the
	// memory of any modern gaming system.
	m_aBackground.reserve(2);
	m_aPeriodic.reserve(15);
	return true;
}


//------------------------------------------------------------------------//
bool SoundScape::InternalInit(SoundScapeInternalInit& init)
{
	FN("SoundScape::InternalInit()");
	if(!Init())
		return false;

	ISound* pSound;
	ISound3D* pSound3D;
	SoundInit sndinit;
	Sound3DInit snd3Dinit;

	int i;
	for(i = 0; i < init.m_aBackground.size(); i++)
	{
		if(!AudioMgr()->CreateSound(pSound))
			return false;
		if(!SoundMgr()->GetSoundInit(init.m_aBackground[i].m_sSoundID, sndinit))
			return false;
		if(!pSound->Init(sndinit))
			return false;
		init.m_aBackground[i].m_Init.m_pSound = pSound;
		if(!AddElement(init.m_aBackground[i].m_Init))
			return false;
	}
	for(i = 0; i < init.m_aPeriodic.size(); i++)
	{
		if(!AudioMgr()->CreateSound3D(pSound3D))
			return false;
		if(!SoundMgr()->GetSound3DInit(init.m_aPeriodic[i].m_sSound3DID, snd3Dinit))
			return false;
		if(!pSound3D->Init(snd3Dinit))
			return false;
		init.m_aPeriodic[i].m_Init.m_pSound3D = pSound3D;
		if(!AddElement(init.m_aPeriodic[i].m_Init))
			return false;
	}
	
	return true;
}


//------------------------------------------------------------------------//
bool SoundScape::AddElement(const BackgroundInit& init)
{
	FN("SoundScape::AddElement()");
	BackgroundElement element;
	element.m_Init = init;
	m_aBackground.push_back(element);
	return true;
}


//------------------------------------------------------------------------//
bool SoundScape::AddElement(const PeriodicInit& init)
{
	FN("SoundScape::AddElement()");
	PeriodicElement element;
	element.m_Init = init;
	m_aPeriodic.push_back(element);
	return true;
}


//------------------------------------------------------------------------//
bool SoundScape::Load()
{
	FN("SoundScape::Load()");
	int i;
	for(i = 0; i < m_aBackground.size(); i++)
		m_aBackground[i].m_Init.m_pSound->Load();

	for(i = 0; i < m_aPeriodic.size(); i++)
		m_aPeriodic[i].m_Init.m_pSound3D->Load();

	m_bLoaded = true;

	return true;
}

//------------------------------------------------------------------------//
bool SoundScape::Unload()
{
	FN("SoundScape::Unload()");

	Stop();

	int i;
	for(i = 0; i < m_aBackground.size(); i++)
		m_aBackground[i].m_Init.m_pSound->Unload();

	for(i = 0; i < m_aPeriodic.size(); i++)
		m_aPeriodic[i].m_Init.m_pSound3D->Unload();

	m_bLoaded = false;

	return true;
}

//------------------------------------------------------------------------//
bool SoundScape::IsLoaded() const
{
	FN("SoundScape::IsLoaded()");
	return m_bLoaded;
}


//------------------------------------------------------------------------//
bool SoundScape::Play()
{
	FN("SoundScape::Play()");
	if(m_bPlaying)
		return true;

	int i;
	for(i = 0; i < m_aBackground.size(); i++)
	{
		float fVolume = GetRandom(
			m_aBackground[i].m_Init.m_fMinVolume,
			m_aBackground[i].m_Init.m_fMaxVolume);
		m_aBackground[i].m_fCurrentVolume = fVolume * m_fVolume;
		m_aBackground[i].m_Init.m_pSound->SetVolume(m_aBackground[i].m_fCurrentVolume);
		float fPitch = GetRandom(
			m_aBackground[i].m_Init.m_fMinPitch,
			m_aBackground[i].m_Init.m_fMaxPitch);
		m_aBackground[i].m_fCurrentPitch = fPitch;
		m_aBackground[i].m_Init.m_pSound->SetPitch(fPitch);
		m_aBackground[i].m_Init.m_pSound->Play();
	}

	for(i = 0; i < m_aPeriodic.size(); i++)
		if(m_aPeriodic[i].m_Init.m_pSound3D->IsPaused())
			m_aPeriodic[i].m_Init.m_pSound3D->Play();

	CSoundMgr()->InsertSoundScape(this);

	m_bPlaying = true;
	m_bPaused = false;

	m_bFirstUpdate = true;

	return true;
}

//------------------------------------------------------------------------//
bool SoundScape::Stop()
{
	FN("SoundScape::Stop()");
	if(!m_bPlaying && !m_bPaused)
		return true;

	CSoundMgr()->RemoveSoundScape(this);

	int i;
	for(i = 0; i < m_aBackground.size(); i++)
		m_aBackground[i].m_Init.m_pSound->Stop();

	for(i = 0; i < m_aPeriodic.size(); i++)
		m_aPeriodic[i].m_Init.m_pSound3D->Stop();

	m_bPlaying = false;
	m_bPaused = false;


	return true;
}

//------------------------------------------------------------------------//
bool SoundScape::Pause()
{
	FN("SoundScape::Pause()");

	CSoundMgr()->RemoveSoundScape(this);

	int i;
	for(i = 0; i < m_aBackground.size(); i++)
		m_aBackground[i].m_Init.m_pSound->Pause();

	for(i = 0; i < m_aPeriodic.size(); i++)
		m_aPeriodic[i].m_Init.m_pSound3D->Pause();

	m_bPlaying = false;
	m_bPaused = true;

	return true;
}


//------------------------------------------------------------------------//
bool SoundScape::IsPlaying() const
{
	FN("SoundScape::IsPlaying()");
	return m_bPlaying;
}

//------------------------------------------------------------------------//
bool SoundScape::IsPaused() const
{
	FN("SoundScape::IsPaused()");
	return m_bPaused;
}


//------------------------------------------------------------------------//
bool SoundScape::IsLooping() const
{
	FN("SoundScape::IsLooping()");
	return true;
}


//------------------------------------------------------------------------//
bool SoundScape::SetVolume(float fVolume)
{
	FN("SoundScape::SetVolume()");
	m_fVolume = Clamp<float>(fVolume, VOLUME_MIN, VOLUME_MAX);
	m_bVolumeChanged = true;
	return true;
}

//------------------------------------------------------------------------//
bool SoundScape::GetVolume(float &fVolume) const
{
	FN("SoundScape::GetVolume()");
	fVolume = m_fVolume;
	return true;
}


//------------------------------------------------------------------------//
void SoundScape::Update()
{
	FN("SoundScape::Update()");

	// We track local time for each soundscape since we don't want
	// time to elapse when the object is paused or stopped.
	m_fCurrentTime += CSoundMgr()->GetFrameTime();

	// Update all background elements
	UpdateBackground();
	// Update all periodic elements
	UpdatePeriodic();

	m_bFirstUpdate = false;
	m_bVolumeChanged = false;
}


void SoundScape::UpdateBackground()
{
	// Update all background sounds
	for(int i = 0; i < m_aBackground.size(); i++)
	{
		// Determine if this sound has modulating volume
		if(m_aBackground[i].m_Init.m_fMinVolume != m_aBackground[i].m_Init.m_fMaxVolume)
		{
			// Adjust volume target if necessary
			if((m_fCurrentTime >= m_aBackground[i].m_fTargetVolumeTime) || m_bFirstUpdate)
			{
				float fTime = GetRandom(
					m_aBackground[i].m_Init.m_fMinVolumeTime,
					m_aBackground[i].m_Init.m_fMaxVolumeTime);
				float fVolume = GetRandom(
					m_aBackground[i].m_Init.m_fMinVolume,
					m_aBackground[i].m_Init.m_fMaxVolume);
				float fRate = (fVolume - m_aBackground[i].m_fCurrentVolume) / fTime;
				m_aBackground[i].m_fTargetVolumeTime = fTime + m_fCurrentTime;
				m_aBackground[i].m_fCurrentVolumeChangeRate = fRate;
			}

			// Adjust volume
			m_aBackground[i].m_fCurrentVolume += 
				(m_aBackground[i].m_fCurrentVolumeChangeRate * CSoundMgr()->GetFrameTime());
			m_aBackground[i].m_Init.m_pSound->SetVolume(m_aBackground[i].m_fCurrentVolume * m_fVolume);
		}
		else if(m_bVolumeChanged)
		{
			m_aBackground[i].m_Init.m_pSound->SetVolume(m_aBackground[i].m_Init.m_fMaxVolume * m_fVolume);
		}

		// Determine if this sound has modulating pitch
		if(m_aBackground[i].m_Init.m_fMinPitch != m_aBackground[i].m_Init.m_fMaxPitch)
		{
			// Adjust pitch target if necessary
			if(m_fCurrentTime >= m_aBackground[i].m_fTargetPitchTime)
			{
				float fTime = GetRandom(
					m_aBackground[i].m_Init.m_fMinPitchTime,
					m_aBackground[i].m_Init.m_fMaxPitchTime);
				float fPitch = GetRandom(
					m_aBackground[i].m_Init.m_fMinPitch,
					m_aBackground[i].m_Init.m_fMaxPitch);
				float fRate = (fPitch - m_aBackground[i].m_fCurrentPitch) / fTime;
				m_aBackground[i].m_fTargetPitchTime = fTime + m_fCurrentTime;
				m_aBackground[i].m_fCurrentPitchChangeRate = fRate;
			}

			// Adjust pitch
			m_aBackground[i].m_fCurrentPitch += 
				(m_aBackground[i].m_fCurrentPitchChangeRate * CSoundMgr()->GetFrameTime());
			m_aBackground[i].m_Init.m_pSound->SetPitch(m_aBackground[i].m_fCurrentPitch);
		}
	}
}


void SoundScape::UpdatePeriodic()
{
	IListener* pListener;
	if(!AudioMgr()->GetListener(pListener))
		return;

	AUDIOVECTOR vListenerPos;
	pListener->GetPosition(vListenerPos);

	AUDIOVECTOR vSoundPos;

	// Update all periodic sound effects
	for(int i = 0; i < m_aPeriodic.size(); i++)
	{
		// If enough time has elapsed, change all the settings
		if(m_aPeriodic[i].m_fNextPlay <= m_fCurrentTime)
		{
			// Set a new position relative to the listener
			m_aPeriodic[i].m_vPosition.x = GetRandom(
				-1 * (m_aPeriodic[i].m_Init.m_fXRange), 
				m_aPeriodic[i].m_Init.m_fXRange); 
			m_aPeriodic[i].m_vPosition.y = GetRandom(
				-1 * (m_aPeriodic[i].m_Init.m_fYRange), 
				m_aPeriodic[i].m_Init.m_fYRange); 
			m_aPeriodic[i].m_vPosition.z = GetRandom(
				-1 * (m_aPeriodic[i].m_Init.m_fZRange), 
				m_aPeriodic[i].m_Init.m_fZRange); 

			// Ensure the new position axis values are outside the min range 
			if(m_aPeriodic[i].m_vPosition.x < 0.0f)
				m_aPeriodic[i].m_vPosition.x = ClampMax<float>(
					m_aPeriodic[i].m_vPosition.x, 
					-(m_aPeriodic[i].m_Init.m_fMinDistance));
			else
				m_aPeriodic[i].m_vPosition.x = ClampMin<float>(
					m_aPeriodic[i].m_vPosition.x, 
					m_aPeriodic[i].m_Init.m_fMinDistance);
			if(m_aPeriodic[i].m_vPosition.y < 0.0f)
				m_aPeriodic[i].m_vPosition.y = ClampMax<float>(
					m_aPeriodic[i].m_vPosition.y, 
					-(m_aPeriodic[i].m_Init.m_fMinDistance));
			else
				m_aPeriodic[i].m_vPosition.y = ClampMin<float>(
					m_aPeriodic[i].m_vPosition.y, 
					m_aPeriodic[i].m_Init.m_fMinDistance);
			if(m_aPeriodic[i].m_vPosition.z < 0.0f)
				m_aPeriodic[i].m_vPosition.z = ClampMax<float>(
					m_aPeriodic[i].m_vPosition.z, 
					-(m_aPeriodic[i].m_Init.m_fMinDistance));
			else
				m_aPeriodic[i].m_vPosition.z = ClampMin<float>(
					m_aPeriodic[i].m_vPosition.z, 
					m_aPeriodic[i].m_Init.m_fMinDistance);

			// Hold the sound's position relative to the listener
			vSoundPos.x = vListenerPos.x + m_aPeriodic[i].m_vPosition.x;
			vSoundPos.y = vListenerPos.y + m_aPeriodic[i].m_vPosition.y;
			vSoundPos.z = vListenerPos.z + m_aPeriodic[i].m_vPosition.z;
			m_aPeriodic[i].m_Init.m_pSound3D->SetPosition(vSoundPos);
			DebugOut(1, "New position is x: %f, y: %f, z: %f", vSoundPos.x, vSoundPos.y, vSoundPos.z);

			// Determine the pitch of the new sound
			float fPitch = GetRandom(m_aPeriodic[i].m_Init.m_fMinPitch, 
				m_aPeriodic[i].m_Init.m_fMaxPitch);
			m_aPeriodic[i].m_Init.m_pSound3D->SetPitch(fPitch);
			DebugOut(5, "New sound pitch is: %f rate of original sound.", fPitch);

			// Calculate the next time for the sound to play
			float fRand = GetRandom(m_aPeriodic[i].m_Init.m_fMinDelay, 
				m_aPeriodic[i].m_Init.m_fMaxDelay);
			DebugOut(5, "3D Sound should play again in %f seconds.", fRand);
			m_aPeriodic[i].m_fNextPlay = m_fCurrentTime + fRand;

			// If this isn't the first update, play the sound
			if(!m_bFirstUpdate)
			{
				DebugOut(1, "Setting volume to %f", m_fVolume);
				m_aPeriodic[i].m_Init.m_pSound3D->SetVolume(m_fVolume);
				m_aPeriodic[i].m_Init.m_pSound3D->Play();
			}
		}
		else
		{
			// Hold the sound's position relative to the listener
			vSoundPos.x = vListenerPos.x + m_aPeriodic[i].m_vPosition.x;
			vSoundPos.y = vListenerPos.y + m_aPeriodic[i].m_vPosition.y;
			vSoundPos.z = vListenerPos.z + m_aPeriodic[i].m_vPosition.z;
			m_aPeriodic[i].m_Init.m_pSound3D->SetPosition(vSoundPos);
		}
		if(m_bVolumeChanged)
		{
			DebugOut(1, "Setting volume to %f", m_fVolume);
			m_aPeriodic[i].m_Init.m_pSound3D->SetVolume(m_fVolume);
		}
	}
}



//------------------------------------------------------------------------//
// Generic property support (for driver-specific extensions)
// Technically, these could be set for individual sounds, but
// they are not implemented for this version of the library.
// Generic property support (for driver-specific extensions)
bool SoundScape::QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport)
{
	FN("SoundScape::QuerySupport()");
	return false;
}

//------------------------------------------------------------------------//
bool SoundScape::Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
	uint32 nPropLength, uint32* pBytesReturned)
{
	FN("SoundScape::Get()");
	return false;
}

//------------------------------------------------------------------------//
bool SoundScape::Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
	uint32 nPropLength, bool bStoreProperty)
{
	FN("SoundScape::Set()");
	return false;
}





