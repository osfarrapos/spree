/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __SOUNDSCAPE_H
#define __SOUNDSCAPE_H

#include "Audio.h"

namespace Audio
{

// Internally used to contain all data needed for playback
// of a background element
struct BackgroundElement
{
	BackgroundElement()	{  Clear();  }
	void Clear()
	{
		m_Init.Clear();
		m_sSoundID.erase();
		m_fCurrentVolume = 1.0f;
		m_fTargetVolumeTime = 0.0f;
		m_fCurrentVolumeChangeRate = 0.0f;
		m_fCurrentPitch = 1.0f;
		m_fTargetPitchTime = 0.0f;
		m_fCurrentPitchChangeRate = 0.0f;
	}

	BackgroundInit	m_Init;
	std::string		m_sSoundID;
	float			m_fCurrentVolume;
	float			m_fTargetVolumeTime;
	float			m_fCurrentVolumeChangeRate;
	float			m_fCurrentPitch;
	float			m_fTargetPitchTime;
	float			m_fCurrentPitchChangeRate;
};


// Internally used to contain all data needed for playback
// of a periodic element
struct PeriodicElement
{
	PeriodicElement()	{  Clear();  }
	void Clear()
	{
		m_Init.Clear();
		m_sSound3DID.erase();
		m_fNextPlay = 0.0f;
		m_vPosition.x = 0.0f;
		m_vPosition.y = 0.0f;
		m_vPosition.z = 0.0f;
	}

	PeriodicInit	m_Init;
	std::string		m_sSound3DID;
	float			m_fNextPlay;
	AUDIOVECTOR		m_vPosition;
};


typedef std::vector<BackgroundElement> SSBEVector;
typedef std::vector<PeriodicElement> SSPEVector;

// Used to internally initialize a soundscape using scripts
struct SoundScapeInternalInit
{
	SSBEVector	m_aBackground;
	SSPEVector	m_aPeriodic;
};


class SoundScape : public ISoundScape
{

DEFINE_POOL(SoundScape);

// Interface functions
public:
	bool Init();
	void Destroy();

	bool IsInitialized() const	{  return m_bInitialized;  }

	bool AddElement(const BackgroundInit& init);
	bool AddElement(const PeriodicInit& init);

	bool Load();
	bool Unload();
	bool IsLoaded() const;

	bool Play();
	bool Stop();
	bool Pause();

	bool IsPlaying() const;
	bool IsPaused() const;

	bool IsLooping() const;

	bool SetVolume(float fVolume);
	bool GetVolume(float &fVolume) const;

	// Generic property support (for driver-specific extensions)
	bool QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport);
	bool Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
		uint32 nPropLength, uint32* pBytesReturned);
	bool Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
		uint32 nPropLength, bool bStoreProperty);

// Concrete-only functions
public:

	void Term();
	bool InternalInit(SoundScapeInternalInit& init);
	void Update();

private:
	SoundScape();
	virtual ~SoundScape();

	void Clear();

	void UpdateBackground();
	void UpdatePeriodic();

// Private data
private:

	bool		m_bInitialized;
	bool		m_bFirstUpdate;
	bool		m_bLoaded;
	bool		m_bPlaying;
	bool		m_bPaused;

	SSBEVector	m_aBackground;
	SSPEVector	m_aPeriodic;

	float		m_fCurrentTime;

	float		m_fVolume;
	bool		m_bVolumeChanged;
};

}; // namespace Audio


#endif // __SOUNDSCAPE_H