/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __MUSICMGR_H
#define __MUSICMGR_H

#include "Audio.h"
#include "Script.h"

namespace Audio
{

class Theme;

typedef std::map<std::string, IDLS*> IDLSMap;
typedef std::map<std::string, ISegment*> ISegmentMap;
typedef std::map<std::string, Theme*> ThemeMap;

class MusicManager : public IMusicManager
{
// Interface functions
public:
	
	bool Init();
	void Term();
	bool IsInitialized()		{  return m_bInitialized;  }

	bool LoadScript(std::string sFileName);

	// Theme functions
	bool PlayTheme(std::string sThemeName);
	void ResetThemeItr();
	bool GetNextTheme(std::string& sThemeName);
	bool RemoveTheme(std::string& sThemeName);
	bool RemoveAll();

	void OnSegmentStart();

// Concrete functions	
public:
	MusicManager();
	virtual ~MusicManager();

	void Clear();



// Internal functions and types
private:
	

	bool LoadDLS(ScriptNode* pNode);
	bool LoadSegment(ScriptNode* pNode);
	bool LoadTheme(ScriptNode* pNode);
	bool LoadThemeNode(ScriptNode* pNode, Theme* pTheme);


// Internal data
private:
	// Bool indicating initialization status
	bool					m_bInitialized;
	// Script loader object
	ScriptLoader			m_Loader;

	// DLS data
	IDLSMap					m_DLSMap;
	// Segment data
	ISegmentMap				m_SegmentMap;
	// Theme collection
	ThemeMap				m_ThemeMap;
	ThemeMap::iterator		m_ThemeItr;

	IDLS*					m_pCurrentDLS;

	// Currently playing theme
	Theme*					m_pPlayingTheme;
	Theme*					m_pPlayingInterlude;
	Theme*					m_pPreviousTheme;

	bool					m_bIsPlaying;
};

inline static MusicManager* CMusicMgr()
{  return static_cast<MusicManager*>(MusicMgr());  }

}; // namespace Audio

#endif // __MUSICMGR_H