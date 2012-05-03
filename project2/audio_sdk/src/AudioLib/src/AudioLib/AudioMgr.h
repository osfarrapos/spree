/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __AUDIOMGR_H
#define __AUDIOMGR_H

#include "Audio.h"

#include "BufferCache.h"

struct IDirectMusicLoader8;
struct IDirectMusicPerformance8;
struct IDirectSound8;
struct IDirectSoundBuffer;
struct IDirectMusicAudioPath;

namespace Audio
{

class Sound;
class Sound3D;
class Segment;
class AudioScript;
class Listener;

/*
Thanks to:
Stéphane PETIT (S.Petit@cryo-interactive.com)
Technical Direction - Programming Department Manager
Cryo Interactive Entertainment - 24, rue Marc Seguin - 75018 Paris - France
*/
static int LinearToLogVol(double fLevel)
{
	// Clamp the value
	if(fLevel <= 0.0f)
		return DSBVOLUME_MIN;
	else if(fLevel >= 1.0f)
		return 0;
    return (long) (-2000.0 * log10(1.0f / fLevel));
}

static float LogToLinearVol(int iLevel)
{
	// Clamp the value
	if(iLevel <= -9600)
		return 0.0f;
	else if(iLevel >= 0)
		return 1.0f;
    return pow(10, double(iLevel + 2000) / 2000.0f) / 10.0f;
}

// Unknown contributor (thanks):
// use a linear scale from 0.0 (silence) to 1.0 (full volume)
static int VolumeToDecibels(float vol) 
{
	if (vol>=1.0F) 
		return 0;
	if (vol<=0.0F) 
		return DSBVOLUME_MIN;
	static const float adj=3321.928094887F;  // 1000/log10(2)
	return int(float(log10(vol)) * adj);
}


typedef std::vector<Sound*> SoundVector;
typedef std::vector<Sound3D*> Sound3DVector;
typedef std::vector<Segment*> SegmentVector;

typedef std::queue<Sound*> SoundQueue;
typedef std::queue<Sound3D*> Sound3DQueue;
typedef std::queue<Segment*> SegmentQueue;

typedef std::list<Sound*> SoundList;
typedef std::list<Sound3D*> Sound3DList;
typedef std::list<Segment*> SegmentList;


class AudioManager : public IAudioManager
{
// Interface functions
public:
	bool Init(const AudioMgrInit& init);
	void Term();

	bool IsInitialized() const;
	bool GetStats(AudioMgrStats& stats) const;

	// Object create and destroy functions
	bool CreateSound(ISound*& pSound);
	bool CreateSound3D(ISound3D*& pSound3D);
	bool CreateSegment(ISegment*& pSegment);
	bool CreateDLS(IDLS*& pDLS);
	bool CreateAudioScript(IAudioScript*& pScript);

	// This interface is unique in that it will return a single object created internally
	bool GetListener(IListener*& pListener);
	
	bool SetSoundVolume(float fVolume);
	bool GetSoundVolume(float& fVolume) const;

	bool SetMusicVolume(float fVolume);
	bool GetMusicVolume(float& fVolume) const;

	bool StopAll();
	bool PauseAll();
	bool ResumeAll();

	bool GetCurrentSegment(ISegment*& pSegment) const;
	bool GetNextSegment(ISegment*& pSegment) const;

	bool CreateAudioStream(IAudioStream*& pStream);

// Concrete functions
public:
	AudioManager();
	virtual ~AudioManager();

	void Clear();

	// DX object accessors
	IDirectMusicLoader8*		Loader()		{  return m_pLoader;  }
	IDirectSound8*				DirectSound()	{  return m_pDirectSound;  }
	IDirectSoundBuffer*			PrimaryBuffer() {  return m_pPrimaryBuffer;  }
	IDirectMusicPerformance8*	Performance(bool bMusic)	
	{  return (bMusic) ? m_pMusicPerformance : m_pSoundPerformance;  }
	DSCAPS&						GetCaps()		{  return m_DSCaps;  }		

	// Hardware management functions
	bool ForceSoftware(bool b3D) const			{  return (b3D) ? m_Stats.m_bForce3DSoftware : m_Stats.m_bForce2DSoftware;  }
	uint32 GetNumBuffers(bool b3D) const		{  return (b3D) ? m_Stats.m_n3DSoundsLoaded : m_Stats.m_n2DSoundsLoaded;  }
	const AudioMgrInit* GetInit() const			{  return &m_Init;  }

	bool CanAddSound() const;
	bool CanAddSound3D() const;
	bool CanAddSegment() const;
	bool RemoveSound(Sound* pSound);
	bool RemoveSound3D(Sound3D* pSound3D);
	bool RemoveSegment(Segment* pSegment);
	void ResetSoundLimit();
	void ResetSound3DLimit();

	// Stream insertion and removal
	void InsertStream(Sound* pStream);
	void RemoveStream(Sound* pStream);

	// Segment access functions
	ISegment*	GetCurrentSegment()	const			{  return m_pCurrentSegment;  }
	void		SetCurrentSegment(ISegment* pSeg)	{  m_pCurrentSegment = pSeg;  }
	ISegment*	GetNextSegment() const				{  return m_pNextSegment;  }
	void		SetNextSegment(ISegment* pSeg)		{  m_pNextSegment = pSeg;  }

	// Loaded list functions
	void OnLoadSound(Sound* pSound);
	void OnUnloadSound(Sound* pSound);
	void OnLoadSound3D(Sound3D* pSound3D);
	void OnUnloadSound3D(Sound3D* pSound3D);
	void OnLoadSegment(Segment* pSegment);
	void OnUnloadSegment(Segment* pSegment);

	// Insert the audio element into the pending load lists
	void ScheduleLoad(Sound* pSound);
	void ScheduleLoad(Sound3D* pSound3D);
	void ScheduleLoad(Segment* pSegment);

	// Are we loading asynchronously?
	bool LoadAsync()								{  return m_Init.m_bLoadAsyncronously;  }

	void AdjustVolume(Sound* pSound);
	void AdjustVolume(Segment* pSound);

	uint32 GetOptimalSampleBits()					{  return m_Init.m_nOptimalSampleBits;  }
	uint32 GetOptimalSampleRate()					{  return m_Init.m_nOptimalSampleRate;  }

	char*	GetAudioSystemPath()					{  return m_pszAudioSystemPath;  }
	char*	GetCurrentWorkingPath()					{  return m_pszCurrentWorkingPath;  }

	CRITICAL_SECTION& GetUpdateCS()					{  return m_csAudioUpdate;  }
	CRITICAL_SECTION& GetLoadingCS()				{  return m_csLoading;  }

	BufferCache* GetBufferCache()					{  return &m_BufferCache;  }

private:


	// Separate thread for timed events
	static void TimeEvent(LPVOID lpv);
	// Service all currently playing streaming buffers
	void ServiceStreamingBuffers();

	// Separate thread used to update the music system
	static void MusicEventThread(LPVOID lpv);
	void UpdateMusic();

	// Thread used for loading audio elements asyncronously
	static void LoadingThread(LPVOID lpv);
	void ServiceLoading();


	// DirectMusic interfaces
	IDirectMusicLoader8*		m_pLoader;
	IDirectMusicPerformance8*	m_pSoundPerformance;
	IDirectMusicPerformance8*	m_pMusicPerformance;

	// DirectSound interfaces and structures
	IDirectSound8*				m_pDirectSound;
	IDirectSoundBuffer*			m_pPrimaryBuffer;
	DSCAPS						m_DSCaps;

	// Verification that the system has been initialized;
	bool						m_bInitialized;

	// Stored path information for the audio system
	char						m_pszAudioSystemPath[MAX_PATH];
	char						m_pszCurrentWorkingPath[MAX_PATH];

	// Initialization structure
	AudioMgrInit				m_Init;

	// Buffer and segment usage members
	AudioMgrStats				m_Stats;

	// Ordered sets of all currently loaded objects by type
	SoundVector					m_LoadedSound;
	Sound3DVector				m_LoadedSound3D;
	SegmentVector				m_LoadedSegment;

	// Caching system for equal sized buffers
	BufferCache					m_BufferCache;

	// Asynchronous load pending lists for sounds and segments
	SoundQueue					m_SoundLoadPending;
	SoundQueue					m_SoundLoadTemp;
	Sound3DQueue				m_Sound3DLoadPending;
	Sound3DQueue				m_Sound3DLoadTemp;
	SegmentQueue				m_SegmentLoadPending;
	SegmentQueue				m_SegmentLoadTemp;
	HANDLE						m_hLoadNotify;

	// Sound and music volume level variable and structures
	float						m_fSoundVolume;
	float						m_fMusicVolume;

	// Streaming members
	SoundList					m_SoundStreamProcess;
	SoundList					m_SoundStreamRemoval;

	// The listener object - used for 3d audio positioning
	Listener*					m_pListener;

	// Current and next music segments
	ISegment*					m_pCurrentSegment;
	ISegment*					m_pNextSegment;

	// Music update thread handle
	HANDLE						m_hMusicNotify;

	// Used to properly synchronize and shut down the manager's multiple threads
	HANDLE						m_hTerm[2];

	// Critical sections ensuring threads are properly syncronized with
	// other thread's functions
	CRITICAL_SECTION			m_csAudioUpdate;
	CRITICAL_SECTION			m_csLoading;
	CRITICAL_SECTION			m_csLoadScheduling;

	// Audio file factory information
	IAudioStreamFactory*		m_pStreamFactory;
};


static AudioManager* DXAudioMgr()
{  return static_cast<AudioManager*>(AudioMgr());  }


}; // namespace Audio

#endif // __AUDIOMGR_H