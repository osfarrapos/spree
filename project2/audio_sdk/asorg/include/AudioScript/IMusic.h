/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef IMUSIC_H__
#define IMUSIC_H__

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace Audio
{
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/*! \file IMusic.h*/ 

//! High-level music management interface class
/*!
This class allows loading and playback of music scripts.
Each script contains a number of themes, each containing a
complete transition mapping of segments.  Different themes
may be transitioned to at run-time, and specific transition
mappings based on segments or themes may be defined.
*/
class IMusicManager : public IMusicCallback
{
public:

	/*!
	Initializes the music manager.  This must be called
	after IAudioManager::Init() is called.
	\return true indicates success, false indicates failure
	\sa Term(), IAudioManager::Init()
	*/
	virtual bool Init() = 0;
	/*!
	Shuts down the music manager.  This must be called
	before IAudioManager::Term() is called.
	\sa Init(), IAudioManager::Term()
	*/
	virtual void Term() = 0;
	/*!
	Determines if the music manager is initialized.
	\return true indicates the manager is initialized, false indicates the 
	manager is not initialized.
	\sa Init(), Term()
	*/
	virtual bool IsInitialized() = 0;

	/*!
	Loads a music script and all associated segments, DLS
	collections, and themes.
	\param sFileName is the name of the script file to load
	\return true indicates success, false indicates failure
	\sa RemoveTheme(), RemoveAll()
	*/
	virtual bool LoadScript(std::string sFileName) = 0;

	/*!
	Plays a new theme based on its given script string ID
	\param sThemeName is the name of the theme ID as defined
	in the script file.  There is no method to stop the
	theme.  Instead, simply retrieve the current theme
	from the audio manager and stop it in the normal
	fashion.
	\return true indicates success, false indicates failure
	\sa IAudioManager::GetCurrentSegment(), IPlayable::Stop()
	*/
	virtual bool PlayTheme(std::string sThemeName) = 0;
	/*!
	Resets an internal iterator used to cycle though and
	retrieve all currently registered themes.
	\sa GetNextTheme()
	*/
	virtual void ResetThemeItr() = 0;
	/*!
	Retrieves the next theme name.  This function can be called
	repeatedly until it returns false.
	\param sThemeName contains the name of the retrieved
	theme unless the function returns false, in which case
	the string will remain unmodified.
	\return true indicates another theme was found, false
	indicates that no more themes were found.
	\sa ResetThemeItr()
	*/
	virtual bool GetNextTheme(std::string& sThemeName) = 0;
	/*!
	Removes a specific theme from the active set.  Keep in 
	mind that only the theme definition, not any associated
	segments and DLS files are removed, since these are
	defined independently of the themes.
	*/
	virtual bool RemoveTheme(std::string& sThemeName) = 0;

	/*!
	Removes all segments, DLS collections, and themes
	currently mapped in the music manager.  This is provided
	as a convenient method of clearing out all current
	musical content loaded from any number of scripts.
	\return true indicates success, false indicates failure
	*/
	virtual bool RemoveAll() = 0;
};

/*!
MusicMgr() is used to access a single static sound manager object from anywhere 
in the code.
\sa IMusicManager
*/
inline static IMusicManager* MusicMgr()
{  return AudioScriptFactory::GetMusicMgr();  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}; // namespace Audio
#endif /* DOXYGEN_SHOULD_SKIP_THIS */


#endif // IMUSIC_H__