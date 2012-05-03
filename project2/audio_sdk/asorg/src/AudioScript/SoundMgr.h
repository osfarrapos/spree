/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __SOUNDMGR_H
#define __SOUNDMGR_H

#include "Audio.h"
#include "Script.h"
#include "SoundScape.h"
#include <map>

namespace Audio
{

typedef std::vector<ISound*> ISoundVector;
typedef std::queue<ISound*> ISoundQueue;
typedef std::vector<ISound3D*> ISound3DVector;
typedef std::queue<ISound3D*> ISound3DQueue;

struct SoundPool
{
	ISoundVector	m_Used;
	ISoundQueue		m_Free;
	SoundInit		m_Init;
};

struct Sound3DPool
{
	ISound3DVector	m_Used;
	ISound3DQueue	m_Free;
	Sound3DInit		m_Init;
};

typedef std::map<std::string, SoundPool> StrSndPoolMap;
typedef std::map<std::string, Sound3DPool> StrSnd3DPoolMap;

typedef std::list<ISoundScape*> SoundScapeList;
typedef std::map<std::string, SoundScapeInternalInit> SSInitMap;
typedef std::vector<ISoundScape*> SoundScapeVector;



class SoundManager : public ISoundManager
{
// Interface functions
public:
	
	bool Init();
	void Term();

	bool IsInitialized()	{  return m_bInitialized;  }


	// Load a sound script from a file
	bool LoadScript(std::string sFileName);

	// Removes all loaded sounds, 3d sounds, and soundscape registrations
	// from memory.
	bool RemoveAll();

	bool GetSound(std::string sSoundName, ISound*& pSound);
	bool ReleaseSound(std::string sSoundName, ISound* pSound);
	bool GetSoundInit(std::string sSoundName, SoundInit& init);
	bool IsSoundRegistered(std::string sSoundName);
	void ResetSoundItr();
	bool GetNextSound(std::string& sSoundName);
	bool RemoveSound(std::string sSoundName);
	bool RemoveAllSounds();

	bool GetSound3D(std::string sSound3DName, ISound3D*& pSound3D);
	bool ReleaseSound3D(std::string sSound3DName, ISound3D* pSound3D);
	bool GetSound3DInit(std::string sSound3DName, Sound3DInit& init);
	bool IsSound3DRegistered(std::string sSound3DName);
	void ResetSound3DItr();
	bool GetNextSound3D(std::string& sSound3DName);
	bool RemoveSound3D(std::string sSound3DName);
	bool RemoveAllSounds3D();

	bool CreateSoundScape(ISoundScape*& pSoundScape);
	bool InitSoundScape(std::string sSoundScapeName, ISoundScape*& pSoundScape);
	bool IsSoundScapeRegistered(std::string sSoundScapeName);
	void ResetSoundScapeItr();
	bool GetNextSoundScape(std::string& sSoundScapeName);
	bool RemoveSoundScape(std::string sSoundScapeName);
	bool RemoveAllSoundScapes();


// Concrete functions	
public:
	SoundManager();
	virtual ~SoundManager();

	void Clear();

	void InsertSoundScape(ISoundScape* pSoundScape);
	void RemoveSoundScape(ISoundScape* pSoundScape);

	float GetCurrentTime()	{  return m_fCurrentTime;  }
	float GetFrameTime()	{  return m_fFrameTime;  }

	CRITICAL_SECTION& GetUpdateCS()	{  return m_csSoundMgrUpdate;  }

// Internal functions and types
private:
	// Separate thread for timed events
	static void TimeEvent(LPVOID lpv);
	// Service all currently playing streaming buffers
	void ServiceSoundScapes();

	bool LoadSound(ScriptNode* pNode);
	bool LoadSound3D(ScriptNode* pNode);
	bool LoadSoundScape(ScriptNode* pNode);
	bool LoadBackground(ScriptNode* pNode, BackgroundElement& bge);
	bool LoadPeriodic(ScriptNode* pNode, PeriodicElement& pe);

private:

	// Bool indicating initialization status
	bool						m_bInitialized;
	// Script loader object
	ScriptLoader				m_Loader;

	// Map associating strings with sound pools
	StrSndPoolMap				m_SndMap;
	// Iterator for traversing through all sound init structure names
	StrSndPoolMap::iterator		m_SndItr;

	// Map associating strings with sound3d initialization structures
	StrSnd3DPoolMap				m_Snd3DMap;
	// Iterator for traversing through all sound3d init structure names
	StrSnd3DPoolMap::iterator	m_Snd3DItr;

	// SoundScape string-name to init structure map
	SSInitMap					m_SSMap;
	// Iterator for traversing through all soundscape init structure names
	SSInitMap::iterator			m_SSItr;

	// List of all updating soundscapes
	SoundScapeList				m_SoundScapeUpdateList;

	// Shut-down synchronization handle
	HANDLE						m_hTerm;
	// Critical section ensuring update thread is properly
	// synchonized with other functions
	CRITICAL_SECTION			m_csSoundMgrUpdate;

	// Track the current time for all time-based calculations
	float						m_fCurrentTime;
	float						m_fFrameTime;
};

inline static SoundManager* CSoundMgr()
{  return static_cast<SoundManager*>(SoundMgr());  }

}; // namespace Audio

#endif // __SOUNDMGR_H