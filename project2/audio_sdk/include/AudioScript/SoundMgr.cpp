/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "SoundMgr.h"
#include "Script.h"
#include "SoundScape.h"

using namespace std;
using namespace Audio;

#define CHECK_INIT() if(!IsInitialized())	return Error::Handle("Sound manager has not been properly initialized yet");

// Comment out this line if you don't want the sound manager to
// require keywords to be case sensitive
#define SNDMGR_CASE_SENSITIVE

#ifdef SNDMGR_CASE_SENSITIVE
#define SNDMGR_STRCMP strcmp
#else
#define SNDMGR_STRCMP strcmpi
#endif

//------------------------------------------------------------------------//
SoundManager::SoundManager()
{
	FN("SoundManager::SoundManager()");
	Clear();
}

//------------------------------------------------------------------------//
SoundManager::~SoundManager()
{
	FN("SoundManager::~SoundManager()");
	Term();
}


//------------------------------------------------------------------------//
void SoundManager::Clear()
{
	FN("SoundManager::Clear()");
	m_bInitialized = false;
	m_SoundScapeUpdateList.clear();
	m_Loader.Clear();
	m_SndMap.clear();
	m_SndItr = m_SndMap.end();
	m_Snd3DMap.clear();
	m_Snd3DItr = m_Snd3DMap.end();
	m_SSMap.clear();
	m_SSItr = m_SSMap.end();
	m_hTerm = 0;
	//ZeroMemory(&m_csSoundMgrUpdate, sizeof(CRITICAL_SECTION));
	m_fCurrentTime = 0.0f;
	m_fFrameTime = 0.0f;

}


//------------------------------------------------------------------------//
bool SoundManager::Init()
{
	FN("SoundManager::Init()");
	if(!m_Loader.Init())
		return Error::Handle("Could not initialize script loader in SoundManager.");
	m_bInitialized = true;

	m_fCurrentTime = float(timeGetTime()) / 1000.0f;

	SoundScape::ReservePool(8);

	InitializeCriticalSection(&m_csSoundMgrUpdate);

	// Set the callback for the timer function used for general events that 
	// need to happen ten times per second.
	if(_beginthread(&SoundManager::TimeEvent, 4096, NULL) == -1)
		return false;

	return true;
}

//------------------------------------------------------------------------//
void SoundManager::Term()
{
	FN("SoundManager::Term()");
	if(m_bInitialized)
	{
		// Create event objects to be used by the thread to signal
		// a successful shutdown.
		m_hTerm = CreateEvent(NULL, FALSE, FALSE, NULL);
		// Indicates the audio manager is now terminating
		m_bInitialized = false;
		// Wait for the thread to shut down before continuing
		WaitForSingleObject(m_hTerm, INFINITE);
		// Close the event object now that we're done with it
		CloseHandle(m_hTerm);

		// Delete all soundscape objects
		//for(int i = 0; i < m_MasterSoundScape.size(); i++)
		//	delete m_MasterSoundScape[i];
		//m_MasterSoundScape.clear();
		SoundScape::TermPool();

		// Enter and leave the critical section to ensure that the
		// soundscape update loop is done
		EnterCriticalSection(&m_csSoundMgrUpdate);
		LeaveCriticalSection(&m_csSoundMgrUpdate);

		// Remove the soundscape critical section
		DeleteCriticalSection(&m_csSoundMgrUpdate);

		// Clear all registration entries
		RemoveAll();

	}
	m_Loader.Term();
	Clear();
}


//------------------------------------------------------------------------//
bool SoundManager::LoadScript(string sFileName)
{
	FN("SoundManager::LoadScript()");
	CHECK_INIT();
	Script script;
	if(!m_Loader.Load(sFileName, script))
		return Error::Handle("Could not load script %s", sFileName.c_str());

	ScriptNode* pNode = script.GetRoot();
	while(pNode)
	{
		// Look for named "Sound" nodes in the script
		if(SNDMGR_STRCMP(pNode->GetName(), "Sound") == 0)
		{
			if(!LoadSound(pNode))
				return false;
		}
		// Look for named "Sound3D" nodes in the script
		else if(SNDMGR_STRCMP(pNode->GetName(), "Sound3D") == 0)
		{
			if(!LoadSound3D(pNode))
				return false;
		}
		// Look for named "soundscape" nodes in the script
		else if(SNDMGR_STRCMP(pNode->GetName(), "SoundScape") == 0)
		{
			if(!LoadSoundScape(pNode))
				return false;
		}
		
		// advance to the next sibling node if one exists
		pNode = pNode->GetSibling();
	};

	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::RemoveAll()
{
	FN("SoundManager::RemoveAll()");
	CHECK_INIT();
	bool bRet = true;
	if(!RemoveAllSounds())
		bRet = false;
	if(!RemoveAllSounds3D())
		bRet = false;
	if(!RemoveAllSoundScapes())
		bRet = false;
	return bRet;
}


//------------------------------------------------------------------------//
bool SoundManager::IsSoundRegistered(std::string sSoundName)
{
	FN("SoundManager::IsSoundRegistered()");
	CHECK_INIT();
	StrSndPoolMap::iterator itr = m_SndMap.find(sSoundName);
	if(itr == m_SndMap.end())
		return false;
	else
		return true;
}


//------------------------------------------------------------------------//
bool SoundManager::GetSoundInit(std::string sSoundName, SoundInit& init)
{
	FN("SoundManager::GetSoundInit()");
	CHECK_INIT();
	StrSndPoolMap::iterator itr = m_SndMap.find(sSoundName);
	if(itr == m_SndMap.end())
		return Error::Handle("Sound definition %s does not exist in map.", sSoundName.c_str());
	init = itr->second.m_Init;
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::GetSound(std::string sSoundName, ISound*& pSound)
{
	FN("SoundManager::GetSound()");
	CHECK_INIT();
	StrSndPoolMap::iterator itr = m_SndMap.find(sSoundName);
	if(itr == m_SndMap.end())
		return Error::Handle("Sound definition %s does not exist in map.", sSoundName.c_str());
	// If there are existing free sounds, use these first
	if(itr->second.m_Free.size() && !itr->second.m_Free.front()->IsPlaying())
	{
		pSound = itr->second.m_Free.front();
		itr->second.m_Free.pop();
		itr->second.m_Used.push_back(pSound);
	}
	// Otherwise, create a new sound object
	else
	{
		if(!AudioMgr()->CreateSound(pSound))
			return Error::Handle("Error creating sound from definition %s.", sSoundName.c_str());
		itr->second.m_Used.push_back(pSound);
		if(!pSound->Init(itr->second.m_Init))
			return Error::Handle("Sound definition %s could not be initialized.", sSoundName.c_str());
	}
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::ReleaseSound(std::string sSoundName, ISound* pSound)
{
	FN("SoundManager::ReleaseSound()");
	CHECK_INIT();
	StrSndPoolMap::iterator itr = m_SndMap.find(sSoundName);
	if(itr == m_SndMap.end())
		return Error::Handle("Sound definition %s does not exist in map.", sSoundName.c_str());
	ISoundVector::iterator snditr;
	snditr = find(itr->second.m_Used.begin(), itr->second.m_Used.end(), pSound); 
	if(snditr == itr->second.m_Used.end())
		return Error::Handle("No sound instances of %s to release", sSoundName.c_str());
	itr->second.m_Used.erase(snditr);
	itr->second.m_Free.push(pSound);
	return true;
}


//------------------------------------------------------------------------//
void SoundManager::ResetSoundItr()
{
	FN("SoundManager::ResetSoundItr()");
	m_SndItr = m_SndMap.begin();
}


//------------------------------------------------------------------------//
bool SoundManager::GetNextSound(std::string& sSoundName)
{
	FN("SoundManager::GetNextSound()");
	CHECK_INIT();
	if(m_SndItr == m_SndMap.end())
		return false;
	sSoundName = (m_SndItr->first);
	++m_SndItr;
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::RemoveSound(std::string sSoundName)
{
	FN("SoundManager::RemoveSound()");
	CHECK_INIT();
	StrSndPoolMap::iterator itr = m_SndMap.find(sSoundName);
	if(itr == m_SndMap.end())
		return false;
	while(!itr->second.m_Free.empty())
	{
		itr->second.m_Free.front()->Destroy();
		itr->second.m_Free.pop();
	}
	for(int i = 0; i < itr->second.m_Used.size(); i++)
		itr->second.m_Used[i]->Destroy();
	m_SndMap.erase(itr);
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::RemoveAllSounds()
{
	FN("SoundManager::RemoveAllSounds()");
	CHECK_INIT();
	StrSndPoolMap::iterator itr;
	for(itr = m_SndMap.begin(); itr != m_SndMap.end(); ++itr)
	{
		while(!itr->second.m_Free.empty())
		{
			itr->second.m_Free.front()->Destroy();
			itr->second.m_Free.pop();
		}
		for(int i = 0; i < itr->second.m_Used.size(); i++)
			itr->second.m_Used[i]->Destroy();
	}
	m_SndMap.clear();
	return true;
}



//------------------------------------------------------------------------//
bool SoundManager::IsSound3DRegistered(std::string sSound3DName)
{
	FN("SoundManager::IsSound3DRegistered()");
	CHECK_INIT();
	StrSnd3DPoolMap::iterator itr = m_Snd3DMap.find(sSound3DName);
	if(itr == m_Snd3DMap.end())
		return false;
	else
		return true;
}


//------------------------------------------------------------------------//
bool SoundManager::GetSound3DInit(std::string sSound3DName, Sound3DInit& init)
{
	FN("SoundManager::GetSound3DInit()");
	CHECK_INIT();
	StrSnd3DPoolMap::iterator itr = m_Snd3DMap.find(sSound3DName);
	if(itr == m_Snd3DMap.end())
		return Error::Handle("Sound3D definition %s does not exist in map.", sSound3DName.c_str());
	init = itr->second.m_Init;
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::GetSound3D(std::string sSound3DName, ISound3D*& pSound3D)
{
	FN("SoundManager::GetSound3D()");
	CHECK_INIT();
	StrSnd3DPoolMap::iterator itr = m_Snd3DMap.find(sSound3DName);
	if(itr == m_Snd3DMap.end())
		return Error::Handle("Sound3D definition %s does not exist in map.", sSound3DName.c_str());
	// If there are existing free sounds and they are not playing, use these first
	if(itr->second.m_Free.size() && !itr->second.m_Free.back()->IsPlaying())
	{
		pSound3D = itr->second.m_Free.front();
		itr->second.m_Free.pop();
		if(pSound3D->IsInitialized())
			pSound3D->Destroy();
	}
	// Otherwise, create a new sound object
	else
	{
		if(!AudioMgr()->CreateSound3D(pSound3D))
			return Error::Handle("Error creating sound from definition %s.", sSound3DName.c_str());
	}
	itr->second.m_Used.push_back(pSound3D);
	if(!pSound3D->Init(itr->second.m_Init))
		return Error::Handle("Sound3D definition %s could not be initialized.", sSound3DName.c_str());
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::ReleaseSound3D(std::string sSound3DName, ISound3D* pSound3D)
{
	FN("SoundManager::ReleaseSound3D()");
	CHECK_INIT();
	StrSnd3DPoolMap::iterator itr = m_Snd3DMap.find(sSound3DName);
	if(itr == m_Snd3DMap.end())
		return Error::Handle("Sound3D definition %s does not exist in map.", sSound3DName.c_str());
	ISound3DVector::iterator snd3Ditr;
	snd3Ditr = find(itr->second.m_Used.begin(), itr->second.m_Used.end(), pSound3D); 
	if(snd3Ditr == itr->second.m_Used.end())
		return Error::Handle("No sound instances of %s to release", sSound3DName.c_str());
	itr->second.m_Used.erase(snd3Ditr);
	itr->second.m_Free.push(pSound3D);
	return true;
}


//------------------------------------------------------------------------//
void SoundManager::ResetSound3DItr()
{
	FN("SoundManager::ResetSound3DItr()");
	m_Snd3DItr = m_Snd3DMap.begin();
}


//------------------------------------------------------------------------//
bool SoundManager::GetNextSound3D(std::string& sSound3DName)
{
	FN("SoundManager::GetNextSound3D()");
	CHECK_INIT();
	if(m_Snd3DItr == m_Snd3DMap.end())
		return false;
	sSound3DName = (m_Snd3DItr->first);
	++m_Snd3DItr;
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::RemoveSound3D(std::string sSound3DName)
{
	FN("SoundManager::RemoveSound3D()");
	CHECK_INIT();
	StrSnd3DPoolMap::iterator itr = m_Snd3DMap.find(sSound3DName);
	if(itr == m_Snd3DMap.end())
		return false;
	while(!itr->second.m_Free.empty())
	{
		itr->second.m_Free.front()->Destroy();
		itr->second.m_Free.pop();
	}
	for(int i = 0; i < itr->second.m_Used.size(); i++)
		itr->second.m_Used[i]->Destroy();
	m_Snd3DMap.erase(itr);
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::RemoveAllSounds3D()
{
	FN("SoundManager::RemoveAllSounds3D()");
	CHECK_INIT();
	StrSnd3DPoolMap::iterator itr;
	for(itr = m_Snd3DMap.begin(); itr != m_Snd3DMap.end(); ++itr)
	{
		while(!itr->second.m_Free.empty())
		{
			itr->second.m_Free.front()->Destroy();
			itr->second.m_Free.pop();
		}
		for(int i = 0; i < itr->second.m_Used.size(); i++)
			itr->second.m_Used[i]->Destroy();
	}
	m_Snd3DMap.clear();
	return true;
}




//------------------------------------------------------------------------//
bool SoundManager::IsSoundScapeRegistered(std::string sSoundScapeName)
{
	FN("SoundManager::IsSoundScapeRegistered()");
	CHECK_INIT();
	SSInitMap::iterator itr = m_SSMap.find(sSoundScapeName);
	if(itr == m_SSMap.end())
		return false;
	else
		return true;
}


//------------------------------------------------------------------------//
bool SoundManager::CreateSoundScape(ISoundScape*& pSoundScape)
{
	FN("SoundManager::CreateSoundScape()");
	CHECK_INIT();
	pSoundScape = SoundScape::CreateObject();
	if(!pSoundScape)
		return false;
//	m_MasterSoundScape.push_back(pSoundScape);
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::InitSoundScape(std::string sSoundScapeName, ISoundScape*& pSoundScape)
{
	FN("SoundManager::InitSoundScape()");
	CHECK_INIT();
	SSInitMap::iterator itr = m_SSMap.find(sSoundScapeName);
	if(itr == m_SSMap.end())
		return false;

	SoundScape* pSS = static_cast<SoundScape*>(pSoundScape);
	if(!pSS->InternalInit(itr->second))
		return false;

	return true;
}


//------------------------------------------------------------------------//
void SoundManager::ResetSoundScapeItr()
{
	FN("SoundManager::ResetSoundScapeItr()");
	m_SSItr = m_SSMap.begin();
}


//------------------------------------------------------------------------//
bool SoundManager::GetNextSoundScape(std::string& sSoundScapeName)
{
	FN("SoundManager::GetNextSoundScape()");
	CHECK_INIT();
	if(m_SSItr == m_SSMap.end())
		return false;
	sSoundScapeName = (m_SSItr->first);
	++m_SSItr;
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::RemoveSoundScape(std::string sSoundScapeName)
{
	FN("SoundManager::RemoveSoundScape()");
	CHECK_INIT();
	SSInitMap::iterator itr = m_SSMap.find(sSoundScapeName);
	if(itr == m_SSMap.end())
		return false;
	m_SSMap.erase(itr);
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::RemoveAllSoundScapes()
{
	FN("SoundManager::RemoveAllSoundScapes()");
	CHECK_INIT();
	m_SSMap.clear();
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::LoadSound(ScriptNode* pNode)
{
	FN("SoundManager::LoadSound()");
	// We use string ids in this scripting system
	if(pNode->GetDataType() != Script::STRING)
		return Error::Handle("No id found for sound identifier");

	// Just return true if the sound is already registered
	if(IsSoundRegistered(pNode->GetString()))
		return true;

	SoundPool pool;
	ScriptNode* pChildNode = pNode->GetChild();
	while(pChildNode)
	{
		if(SNDMGR_STRCMP(pChildNode->GetName(), "FileName") == 0)
			pool.m_Init.m_sFileName = pChildNode->GetString();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Looping") == 0)
			pool.m_Init.m_bLooping = pChildNode->GetBool();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Streaming") == 0)
			pool.m_Init.m_bStreaming = pChildNode->GetBool();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Music") == 0)
			pool.m_Init.m_bMusic = pChildNode->GetBool();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Volume") == 0)
			pool.m_Init.m_Prop.m_fVolume = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Pan") == 0)
			pool.m_Init.m_Prop.m_fPan = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Pitch") == 0)
			pool.m_Init.m_Prop.m_fPitch = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "ReadCursor") == 0)
			pool.m_Init.m_Prop.m_nReadCursor = pChildNode->GetInteger();
		else
			return Error::Handle("Syntax error in Sound definition");
		pChildNode = pChildNode->GetSibling();
	}

	// Insert the sound definition into the map
	m_SndMap.insert(make_pair(pNode->GetString(), pool));
	DebugOut(4, "Registering Sound \"%s\" in sound manager", pNode->GetString());
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::LoadSound3D(ScriptNode* pNode)
{
	FN("SoundManager::LoadSound3D()");
	// We use string ids in this scripting system
	if(pNode->GetDataType() != Script::STRING)
		return Error::Handle("No id found for sound3d identifier");

	// Just return true if the sound3d is already registered
	if(IsSound3DRegistered(pNode->GetString()))
		return true;

	Sound3DPool pool;
	ScriptNode* pChildNode = pNode->GetChild();
	while(pChildNode)
	{
		if(SNDMGR_STRCMP(pChildNode->GetName(), "FileName") == 0)
			pool.m_Init.m_sFileName = pChildNode->GetString();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Looping") == 0)
			pool.m_Init.m_bLooping = pChildNode->GetBool();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Streaming") == 0)
			pool.m_Init.m_bStreaming = pChildNode->GetBool();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Music") == 0)
			pool.m_Init.m_bMusic = pChildNode->GetBool();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Position") == 0)
			pool.m_Init.m_Prop.m_vPosition = pChildNode->GetVector();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Velocity") == 0)
			pool.m_Init.m_Prop.m_vVelocity = pChildNode->GetVector();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "ConeOrientation") == 0)
			pool.m_Init.m_Prop.m_vConeOrientation = pChildNode->GetVector();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "InsideConeAngle") == 0)
			pool.m_Init.m_Prop.m_nInsideConeAngle = pChildNode->GetInteger();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "OutsideConeAngle") == 0)
			pool.m_Init.m_Prop.m_nOutsideConeAngle = pChildNode->GetInteger();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "ConeOutsideVolume") == 0)
			pool.m_Init.m_Prop.m_fConeOutsideVolume = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MinDistance") == 0)
			pool.m_Init.m_Prop.m_fMinDistance = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MaxDistance") == 0)
			pool.m_Init.m_Prop.m_fMaxDistance = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Mode") == 0)
		{
			if(pChildNode->GetDataType() == Script::INTEGER)
			{
				pool.m_Init.m_Prop.m_nMode = pChildNode->GetInteger();
			}
			else
			{
				if(SNDMGR_STRCMP(pChildNode->GetString(), "Normal") == 0)
					pool.m_Init.m_Prop.m_nMode = MODE_NORMAL;
				else if(SNDMGR_STRCMP(pChildNode->GetString(), "HeadRelative") == 0)
					pool.m_Init.m_Prop.m_nMode = MODE_HEADRELATIVE;
				else if(SNDMGR_STRCMP(pChildNode->GetString(), "Disable") == 0)
					pool.m_Init.m_Prop.m_nMode = MODE_DISABLE;
			}
		}
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Volume") == 0)
			pool.m_Init.m_Prop.m_fVolume = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Pitch") == 0)
			pool.m_Init.m_Prop.m_fPitch = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "ReadCursor") == 0)
			pool.m_Init.m_Prop.m_nReadCursor = pChildNode->GetInteger();
		else
		{
			return Error::Handle("Syntax error in Sound3D definition");
		}
		pChildNode = pChildNode->GetSibling();
	}

	// Insert the sound3d definition into the map
	m_Snd3DMap.insert(make_pair(pNode->GetString(), pool));
	DebugOut(4, "Registering Sound3D \"%s\" in sound manager", pNode->GetString());
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::LoadSoundScape(ScriptNode* pNode)
{
	FN("SoundManager::LoadSoundScape()");
	// We use string ids in this scripting system
	if(pNode->GetDataType() != Script::STRING)
		return Error::Handle("No id found for soundscape identifier");

	// Just return true if the soundscape is already registered
	if(IsSoundScapeRegistered(pNode->GetString()))
		return true;

	SoundScapeInternalInit init;
	BackgroundElement bge;
	PeriodicElement pe;

	ScriptNode* pChildNode = pNode->GetChild();
	while(pChildNode)
	{
		if(SNDMGR_STRCMP(pChildNode->GetName(), "Periodic") == 0)
		{
			if(!LoadPeriodic(pChildNode, pe))
				return false;
			init.m_aPeriodic.push_back(pe);
		}
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "Background") == 0)
		{
			if(!LoadBackground(pChildNode, bge))
				return false;
			init.m_aBackground.push_back(bge);
		}
		else
			return false;

		pChildNode = pChildNode->GetSibling();
	}

	// Insert the sound definition into the map
	m_SSMap.insert(make_pair(pNode->GetString(), init));
	DebugOut(4, "Registering Soundscape \"%s\" in sound manager", pNode->GetString());
	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::LoadBackground(ScriptNode* pNode, BackgroundElement& bge)
{
	FN("SoundManager::LoadBackground()");
	// Check to see if the node has the "Default" keyword, meaning it should
	// clear all values to their original defaults.
	if((pNode->GetDataType() == Script::VARIABLE) &&
		(strcmp(pNode->GetVariable(), "Default") == 0))
		bge.Clear();

	ScriptNode* pChildNode = pNode->GetChild();
	while(pChildNode)
	{
		if(SNDMGR_STRCMP(pChildNode->GetName(), "Sound") == 0)
			bge.m_sSoundID = pChildNode->GetString();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MinVolume") == 0)
			bge.m_Init.m_fMinVolume = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MaxVolume") == 0)
			bge.m_Init.m_fMaxVolume = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MinVolumeTime") == 0)
			bge.m_Init.m_fMinVolumeTime = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MaxVolumeTime") == 0)
			bge.m_Init.m_fMaxVolumeTime = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MinPitch") == 0)
			bge.m_Init.m_fMinPitch = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MaxPitch") == 0)
			bge.m_Init.m_fMaxPitch = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MinPitchTime") == 0)
			bge.m_Init.m_fMinPitchTime = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MaxPitchTime") == 0)
			bge.m_Init.m_fMaxPitchTime = pChildNode->GetReal();
		else
			return false;
		pChildNode = pChildNode->GetSibling();
	}

	return true;
}


//------------------------------------------------------------------------//
bool SoundManager::LoadPeriodic(ScriptNode* pNode, PeriodicElement& pe)
{
	FN("SoundManager::LoadPeriodic()");
	// Check to see if the node has the "default" keyword, meaning it should
	// clear all values to their original defaults.
	if((pNode->GetDataType() == Script::VARIABLE) &&
		(strcmp(pNode->GetVariable(), "Default") == 0))
		pe.Clear();

	ScriptNode* pChildNode = pNode->GetChild();
	while(pChildNode)
	{
		if(SNDMGR_STRCMP(pChildNode->GetName(), "Sound3D") == 0)
			pe.m_sSound3DID = pChildNode->GetString();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MinPitch") == 0)
			pe.m_Init.m_fMinPitch = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MaxPitch") == 0)
			pe.m_Init.m_fMaxPitch = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MinDelay") == 0)
			pe.m_Init.m_fMinDelay = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MaxDelay") == 0)
			pe.m_Init.m_fMaxDelay = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "XRange") == 0)
			pe.m_Init.m_fXRange = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "YRange") == 0)
			pe.m_Init.m_fYRange = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "ZRange") == 0)
			pe.m_Init.m_fZRange = pChildNode->GetReal();
		else if(SNDMGR_STRCMP(pChildNode->GetName(), "MinDistance") == 0)
			pe.m_Init.m_fMinDistance = pChildNode->GetReal();
		else
			return false;
		pChildNode = pChildNode->GetSibling();
	}

	return true;
}


//------------------------------------------------------------------------//
void SoundManager::InsertSoundScape(ISoundScape* pSoundScape)
{
	FN("SoundManager::InsertSoundScape()");
	CRITICAL_FUNCTION(&m_csSoundMgrUpdate);
	m_SoundScapeUpdateList.push_back(pSoundScape);
}

//------------------------------------------------------------------------//
void SoundManager::RemoveSoundScape(ISoundScape* pSoundScape)
{
	FN("SoundManager::RemoveSoundScape()");
	CRITICAL_FUNCTION(&m_csSoundMgrUpdate);
	SoundScapeList::iterator itr = find(
		m_SoundScapeUpdateList.begin(),
		m_SoundScapeUpdateList.end(),
		pSoundScape);
	if(itr != m_SoundScapeUpdateList.end())
		m_SoundScapeUpdateList.erase(itr);
}

//------------------------------------------------------------------------//
void SoundManager::TimeEvent(LPVOID lpv)
{
	FN("SoundManager::TimeEvent()");
	while(true)
	{
		// Wake up every 50ms (20fps) to perform some timed actions
		Sleep(50);

		// If the manager has been shut down then terminate this thread
		if(!CSoundMgr()->m_bInitialized)
		{
			SetEvent(CSoundMgr()->m_hTerm);
			return;
		}

		CRITICAL_FUNCTION(&CSoundMgr()->m_csSoundMgrUpdate);
		
		// Calculate the current time and elapsed frame time 
		// (in case we want to tweak timing).  Note that the frame value
		// is always clamped between .01 and .2 seconds.
		CSoundMgr()->m_fFrameTime = Clamp<float>(((float(timeGetTime()) / 1000.0f) - 
			CSoundMgr()->m_fCurrentTime),
			0.01f,
			0.2f);
		CSoundMgr()->m_fCurrentTime += CSoundMgr()->m_fFrameTime;

		// Update all currently loaded soundscapes
		CSoundMgr()->ServiceSoundScapes();
	}
}


//------------------------------------------------------------------------//
void SoundManager::ServiceSoundScapes()
{
	FN("SoundManager::ServiceSoundScapes()");
	SoundScapeList::iterator itr;
	SoundScape* pSoundScape;
	for(itr = m_SoundScapeUpdateList.begin(); itr != m_SoundScapeUpdateList.end(); ++itr)
	{
		pSoundScape = static_cast<SoundScape*>(*itr);
		pSoundScape->Update();
	}
}

