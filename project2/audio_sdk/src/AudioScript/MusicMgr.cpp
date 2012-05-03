/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "MusicMgr.h"
#include "Theme.h"

// Comment out this line if you don't want the sound manager to
// require keywords to be case sensitive
#define MUSMGR_CASE_SENSITIVE

#ifdef MUSMGR_CASE_SENSITIVE
#define MUSMGR_STRCMP strcmp
#else
#define MUSMGR_STRCMP strcmpi
#endif

using namespace std;
using namespace Audio;

//------------------------------------------------------------------------//
MusicManager::MusicManager()
{
	Clear();
}


//------------------------------------------------------------------------//
MusicManager::~MusicManager()
{
	Term();
}


//------------------------------------------------------------------------//
void MusicManager::Clear()
{
	// Script loader and parser
	m_Loader.Clear();

	// music transition data
	m_DLSMap.clear();
	m_SegmentMap.clear();
	m_ThemeMap.clear();
	m_ThemeItr = m_ThemeMap.begin();

	// Current playback information
	m_pPlayingTheme = 0;
	m_pPlayingInterlude = 0;
	m_pPreviousTheme = 0;

	m_bIsPlaying = false;

	m_pCurrentDLS = 0;

	m_bInitialized = false;
}


//------------------------------------------------------------------------//
bool MusicManager::Init()
{
	if(!AudioMgr()->IsInitialized())
		return Error::Handle("The audio manager has not been initialized");

	if(!m_Loader.Init())
		return Error::Handle("Could not initialize script loader in SoundManager.");

	m_bInitialized = true;

	return true;
}


//------------------------------------------------------------------------//
void MusicManager::Term()
{
	m_Loader.Term();
	RemoveAll();
	Clear();
}


//------------------------------------------------------------------------//
bool MusicManager::LoadScript(string sFileName)
{
	if(!IsInitialized())
		return Error::Handle("Music manager is not initialized");

	Script script;
	if(!m_Loader.Load(sFileName, script))
		return Error::Handle("Could not load script %s", sFileName.c_str());

	ScriptNode* pNode = script.GetRoot();
	while(pNode)
	{
		// Look for named "DLS" nodes in the script
		if(MUSMGR_STRCMP(pNode->GetName(), "DLS") == 0)
		{
			if(!LoadDLS(pNode))
				return false;
		}
		// Look for named "Segment" nodes in the script
		else if(MUSMGR_STRCMP(pNode->GetName(), "Segment") == 0)
		{
			if(!LoadSegment(pNode))
				return false;
		}
		// Look for named "Theme" nodes in the script
		else if(MUSMGR_STRCMP(pNode->GetName(), "Theme") == 0)
		{
			if(!LoadTheme(pNode))
				return false;
		}
		else
		{
			return Error::Handle("Unrecognized keyword in music script");
		}

		// advance to the next sibling node if one exists
		pNode = pNode->GetSibling();
	};

	return true;
}


//------------------------------------------------------------------------//
void MusicManager::ResetThemeItr()
{
	if(!IsInitialized())
		Error::Handle("Music manager is not initialized");
	m_ThemeItr = m_ThemeMap.begin();
}

//------------------------------------------------------------------------//
bool MusicManager::GetNextTheme(std::string& sThemeName)
{
	if(!IsInitialized())
		return Error::Handle("Music manager is not initialized");
	if(m_ThemeItr == m_ThemeMap.end())
		return false;
	sThemeName = (m_ThemeItr->first);
	++m_ThemeItr;
	return true;
}

//------------------------------------------------------------------------//
bool MusicManager::RemoveTheme(std::string& sThemeName)
{
	if(!IsInitialized())
		return Error::Handle("Music manager is not initialized");
	ThemeMap::iterator itr = m_ThemeMap.find(sThemeName);
	if(itr == m_ThemeMap.end())
		return false;
	m_ThemeMap.erase(itr);
	return true;
}

//------------------------------------------------------------------------//
bool MusicManager::RemoveAll()
{
	if(!IsInitialized())
		return Error::Handle("Music manager is not initialized");
	ThemeMap::iterator itr1;
	for(itr1 = m_ThemeMap.begin(); itr1 != m_ThemeMap.end(); ++itr1)
		_XASAFE_DELETE(itr1->second);
	m_ThemeMap.clear();

	ISegmentMap::iterator itr2;
	for(itr2 = m_SegmentMap.begin(); itr2 != m_SegmentMap.end(); ++itr2)
		itr2->second->Destroy();
	m_SegmentMap.clear();

	IDLSMap::iterator itr3;
	for(itr3 = m_DLSMap.begin(); itr3 != m_DLSMap.end(); ++itr3)
		itr3->second->Destroy();
	m_DLSMap.clear();
	

	return true;
}


//------------------------------------------------------------------------//
bool MusicManager::LoadDLS(ScriptNode* pNode)
{
	FN("MusicManager::LoadDLS()");
	// We use string ids in this scripting system
	if(pNode->GetDataType() != Script::STRING)
		return Error::Handle("No id found for DLS identifier");

	// Make sure the segment ID doesn't already exist
	IDLSMap::iterator itr = m_DLSMap.find(pNode->GetString());
	if(itr != m_DLSMap.end())
		return true;

	DLSInit init;
	ScriptNode* pChildNode = pNode->GetChild();
	while(pChildNode)
	{
		if(MUSMGR_STRCMP(pChildNode->GetName(), "FileName") == 0)
			init.m_sFileName = pChildNode->GetString();
		pChildNode = pChildNode->GetSibling();
	}
	
	IDLS* pDLS;
	if(!AudioMgr()->CreateDLS(pDLS))
		return false;
	if(!pDLS->Init(init))
		return false;

	m_pCurrentDLS = pDLS;

	// Insert the segment into the map
	m_DLSMap.insert(make_pair(pNode->GetString(), pDLS));

	return true;
}


//------------------------------------------------------------------------//
bool MusicManager::LoadSegment(ScriptNode* pNode)
{
	FN("MusicManager::LoadSegment()");
	// We use string ids in this scripting system
	if(pNode->GetDataType() != Script::STRING)
		return Error::Handle("No id found for sound identifier");

	// Make sure the segment ID doesn't already exist
	ISegmentMap::iterator itr = m_SegmentMap.find(pNode->GetString());
	if(itr != m_SegmentMap.end())
		return true;

	SegmentInit init;
	ScriptNode* pChildNode = pNode->GetChild();
	init.m_pDLS = m_pCurrentDLS;
	while(pChildNode)
	{
		if(MUSMGR_STRCMP(pChildNode->GetName(), "FileName") == 0)
			init.m_sFileName = pChildNode->GetString();
		else if(MUSMGR_STRCMP(pChildNode->GetName(), "Looping") == 0)
			init.m_bLooping = pChildNode->GetBool();
		else if(MUSMGR_STRCMP(pChildNode->GetName(), "Music") == 0)
			init.m_bMusic = pChildNode->GetBool();
		else if(MUSMGR_STRCMP(pChildNode->GetName(), "DLS") == 0)
		{
			IDLSMap::iterator itr = m_DLSMap.find(pChildNode->GetString());
			if(itr != m_DLSMap.end())
				init.m_pDLS = itr->second;
		}
		pChildNode = pChildNode->GetSibling();
	}


	ISegment* pSegment;
	if(!AudioMgr()->CreateSegment(pSegment))
		return false;
	if(!pSegment->Init(init))
		return false;

	// Insert the segment into the map
	m_SegmentMap.insert(make_pair(pNode->GetString(), pSegment));

	return true;
}


//------------------------------------------------------------------------//
bool MusicManager::LoadTheme(ScriptNode* pNode)
{
	FN("MusicManager::LoadTheme()");
	// We use string ids in this scripting system
	if(pNode->GetDataType() != Script::STRING)
		return Error::Handle("No id found for theme identifier");

	Theme* pTheme = 0;

	// Make sure the theme ID doesn't already exist
	ThemeMap::iterator itr = m_ThemeMap.find(pNode->GetString());
	if(itr != m_ThemeMap.end())
		pTheme = itr->second;
	else
		pTheme = new Theme;

	if(!pTheme)
		return false;

	ScriptNode* pChildNode = pNode->GetChild();
	while(pChildNode)
	{
		if(MUSMGR_STRCMP(pChildNode->GetName(), "Src") == 0)
		{
			if(!LoadThemeNode(pChildNode, pTheme))
				return false;
		}
		else if(MUSMGR_STRCMP(pChildNode->GetName(), "Interlude") == 0)
		{
			pTheme->SetInterlude(pChildNode->GetBool());
		}
		else
			return false;
		pChildNode = pChildNode->GetSibling();
	}
	
	// Insert the theme into the map
	m_ThemeMap.insert(make_pair(pNode->GetString(), pTheme));

	return true;
}


//------------------------------------------------------------------------//
bool MusicManager::LoadThemeNode(ScriptNode* pNode, Theme* pTheme)
{
	FN("MusicManager::LoadThemeNode()");

	if(pNode->GetDataType() != Script::STRING)
		return Error::Handle("No id found for theme node");

	ISegment* pSegmentSource = 0;
	Theme* pThemeSource = 0;
	// First look for matching segments based on the id value
	ISegmentMap::iterator itr = m_SegmentMap.find(pNode->GetString());
	if(itr == m_SegmentMap.end())
	{
		// Next, look for matching themes
		ThemeMap::iterator itor = m_ThemeMap.find(pNode->GetString());
		if(itor == m_ThemeMap.end())
			return Error::Handle("Unknown id %s in theme node", pNode->GetString());
		pThemeSource = itor->second;
		if(!pTheme->CreateNode(pThemeSource))
			return Error::Handle("Could not create theme node");
	}
	else
	{
		pSegmentSource = itr->second;
		if(!pTheme->CreateNode(pSegmentSource))
			return Error::Handle("Could not create theme node");
	}

	ScriptNode* pChildNode = pNode->GetChild();
	while(pChildNode)
	{
		if(MUSMGR_STRCMP(pChildNode->GetName(), "Dest") == 0)
		{
			itr = m_SegmentMap.find(pChildNode->GetString());
			if(itr == m_SegmentMap.end())
				return Error::Handle("Unknown id %s in theme node", pChildNode->GetString());
			ISegment* pDest = itr->second;
			if(pThemeSource)
			{
				if(!pTheme->CreateTransition(pThemeSource, pDest))
					return Error::Handle("Error creating theme node transition");
			}
			else
			{
				if(!pTheme->CreateTransition(pSegmentSource, pDest))
					return Error::Handle("Error creating theme node transition");
			}
		}
		else
		{
			return Error::Handle("Unknown syntax or keyword in theme node");
		}
		pChildNode = pChildNode->GetSibling();
	}

	return true;
}


//------------------------------------------------------------------------//
bool MusicManager::PlayTheme(std::string sThemeName)
{
	if(!IsInitialized())
		return Error::Handle("Music manager is not initialized");
	ThemeMap::iterator itr = m_ThemeMap.find(sThemeName);
	if(itr == m_ThemeMap.end())
		return Error::Handle("Could not find theme %s", sThemeName.c_str());

	if(m_pPlayingInterlude)
		m_pPreviousTheme = m_pPlayingInterlude;
	else if(m_pPlayingTheme)
		m_pPreviousTheme = m_pPlayingTheme;

	itr->second->StartTheme();

	if(itr->second->IsInterlude())
		m_pPlayingInterlude = itr->second;
	else
		m_pPlayingTheme = itr->second;

	OnSegmentStart();
	return true;
}


//------------------------------------------------------------------------//
void MusicManager::OnSegmentStart()
{
	FN("MusicManager::OnSegmentStart()");

	ISegment* pCurrent;
	ISegment* pNext;
	AudioMgr()->GetCurrentSegment(pCurrent);
	if(m_pPlayingInterlude)
	{
		if(!m_pPlayingInterlude->GetTransition(m_pPreviousTheme, pCurrent, pNext))
		{
			m_pPlayingInterlude = 0;
			if(!m_pPlayingTheme)
				return;
			m_pPlayingTheme->StartTheme();
			if(!m_pPlayingTheme->GetTransition(m_pPreviousTheme, pCurrent, pNext))
				return;
		}
	}
	else
	{
		if(!m_pPlayingTheme)
			return;
		if(!m_pPlayingTheme->GetTransition(m_pPreviousTheme, pCurrent, pNext))
			return;
	}
	pNext->Play();

}
