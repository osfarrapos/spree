/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef SOUND_H___
#define SOUND_H___

#include <dsound.h>
#include "Audio.h"
#include "IAudioLoader.h"
#include "PropertySet.h"
#include "XAudioPackage.h"


struct IDirectSoundBuffer8;

namespace Audio
{

class Sound : public ISound
{
DEFINE_POOL(Sound);

// Interface functions
public:
	void Destroy();

	bool Init(const SoundInit& create );
	
	bool IsInitialized() const	{  return m_bInitialized;  }

	bool Load();
	bool Unload();
	bool IsLoaded()	const	{  return m_bLoaded;  }

	bool Play();
	bool Stop();
	bool Pause();

	bool IsPlaying() const;
	bool IsPaused() const;

	bool IsLooping() const;

	bool SetProperties(const SoundProp& prop);
	bool GetProperties(SoundProp& prop) const;

	bool SetVolume(float fVolume);
	bool GetVolume(float &fVolume) const;

	bool SetPan(float fPan);
	bool GetPan(float& fPan) const;

	bool SetPitch(float fPitch);
	bool GetPitch(float& fPitch) const;

	bool SetReadCursor(uint32 nBytes);
	bool GetReadCursor(uint32& nBytes) const;
	bool GetSourceSize(uint32& nBytes) const;

	// Generic property support (for driver-specific extensions)
	bool QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport);
	bool Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
		uint32 nPropLength, uint32* pBytesReturned);
	bool Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
		uint32 nPropLength,
		bool bStoreProperty);

// Concrete-only functions
public:

	void Term();

	bool IsLoading() const		{  return m_bLoading;  }
	bool DoLoad();

	bool SetFrequency(uint32 nFrequency);

	inline bool IsMusic() const	{  return m_Init.m_bMusic;  }

	void ServiceBuffer();

	uint32 GetLastPlayTime() const;
	bool operator < (const Sound& snd) const;

private:
	Sound();
	virtual ~Sound();
	uint8 GetSilenceData();

	bool LoadSource(uint32& nBufferSize);
	bool FillBuffer();

	void Clear();

// Private data
private:
	// This thread function needs access to a number of sound member variables
	friend class Sound3D;

	// DirectSound-required structures
	IDirectSoundBuffer8*	m_pDSBuffer; 
	DSBUFFERDESC			m_BufferDesc;
	WAVEFORMATEX			m_WaveFormat;
	DSBCAPS					m_Caps;

	PropertySet				m_PropertySet;

	// Initial sound properties set at Init()
	SoundInit				m_Init;
	
	// status flags
	bool					m_bInitialized;
	bool					m_b3DSound;
	bool					m_bQueuePlayback;
	bool					m_bLoading;
	bool					m_bLoaded;
	bool					m_bPaused;

	// Last time the sound was played (used for prioritization)
	uint32					m_nLastTimePlayed;

	// This is the data source file
	uint32					m_nSourceSize;

	// Streaming data
	IAudioLoader*			m_pLoader;
	uint32					m_nDataCursor;
	uint32					m_nBytesPlayed;
	uint32					m_nLastReadPos;
	bool					m_bRemoveStream;

public:	
	LPBYTE				    m_pResourceBuffer;
	LONG					m_ResourceSize;
};

}; // namespace Audio


#endif // SOUND_H___