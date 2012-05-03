/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef ISOUND_H__
#define ISOUND_H__

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace Audio
{
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/*! \file ISound.h*/ 

//! struct used for initializing a background looping element
struct BackgroundInit
{
	//! Constructor clears all values to defaults
	BackgroundInit()	{  Clear();  }
	//! Clears all structure members to default values
	void Clear()
	{
		m_pSound = 0;
		m_fMinVolume = 1.0f;
		m_fMaxVolume = 1.0f;
		m_fMinVolumeTime = 0.0f;
		m_fMaxVolumeTime = 0.0f;
		m_fMinPitch = 1.0f;
		m_fMaxPitch = 1.0f;
		m_fMinPitchTime = 0.0f;
		m_fMaxPitchTime = 0.0f;
	};

	//! Sound to play as background loop
	ISound*				m_pSound;
	//! Minimum sound volume
	float				m_fMinVolume;
	//! Maximum sound volume
	float				m_fMaxVolume;
	//! Minimum time to take when transitioning to a new volume
	float				m_fMinVolumeTime;
	//! Maximum time to take when transitioning to a new volume
	float				m_fMaxVolumeTime;
	//! Minimum sound pitch
	float				m_fMinPitch;
	//! Maximum sound pitch
	float				m_fMaxPitch;
	//! Minimum time to take when transitioning to a new pitch
	float				m_fMinPitchTime;
	//! Maximum time to take when transitioning to a new pitch
	float				m_fMaxPitchTime;
};

//! struct used for initializing a peroidic element
struct PeriodicInit
{
	//! Constructor clears all values to defaults
	PeriodicInit()	{  Clear();  }
	//! Clears all structure members to default values
	void Clear()
	{
		m_pSound3D = 0;
		m_fMinPitch = 1.0f;
		m_fMaxPitch = 1.0f;
		m_fMinDelay = 0.0f;
		m_fMaxDelay = 0.0f;
		m_fXRange = 25.0f;
		m_fYRange = 25.0f;
		m_fZRange = 25.0f;
		m_fMinDistance = 0.0f;
	};
	//! 3D Sound to play as periodic element
	ISound3D*			m_pSound3D;
	//! Minimum random pitch
	float				m_fMinPitch;
	//! Maximum random pitch
	float				m_fMaxPitch;
	//! Minimum delay between random plays
	float				m_fMinDelay;
	//! Maximum delay between random plays
	float				m_fMaxDelay;
	//! Maximum distance from listener on the x axis (both pos and neg)
	float				m_fXRange;
	//! Maximum distance from listener on the y axis (both pos and neg)
	float				m_fYRange;
	//! Maximum distance from listener on the z axis (both pos and neg)
	float				m_fZRange;
	//! Minimum distance from listener on all axes
	float				m_fMinDistance;
};


//! Class defining a collection of sounds which make up a soundscape
/*!
ISoundScape represents a collection of looping background sounds and
randomly firing 3D positional elements which can be collectively
adjusted and controlled via the IPlayable interface.
*/
class ISoundScape : public IPlayable
{
public:
	virtual bool Init() = 0;

	/*!
	Adds a background element to the soundscape.
	\param init defines the characteristics of the background element
	\return true indicates success, false indicates failure.
	*/
	virtual bool AddElement(const BackgroundInit& init) = 0;
	/*!
	Adds a periodic element to the soundscape.
	\param init defines the characteristics of the periodic element
	\return true indicates success, false indicates failure.
	*/
	virtual bool AddElement(const PeriodicInit& init) = 0;

	/*!
	Sets the collective volume of a soundscape.  This value will decrease
	the volume in addition to any decrease from maximum volume set in the master
	volume controls.  
	\param fVolume sets the volume desired
	\return true indicates success, false indicates failure.
	\sa GetVolume()
	*/
	virtual bool SetVolume(float fVolume) = 0;
	/*!
	Gets the volume for this soundscape.
	\param fVolume retrieves the current object's volume
	\return true indicates success, false indicates failure.
	\sa SetVolume()
	*/
	virtual bool GetVolume(float& fVolume) const = 0;

};

//! Controls and manages high-level sound objects
/*!
ISoundManager is responsible for managing high-level sound objects, including
multiple-instance ISound and ISound3D objects, and ISoundScape objects.  Definitions
are loaded using a text-based script, and individual objects are instantiated using
the string-based identifiers found in the scripts.
*/
class ISoundManager
{
public:

	/*!
	Initializes the sound manager.  This must be called after the low-level
	audio manager has been initalized, and before any other sound manager
	calls are made, except for IsInitialized().
	\return true indicates success, false indicates failure.
	\sa Term(), IsInitialized()
	*/
	virtual bool Init() = 0;
	/*!
	Shuts down the sound manager.  This should be called before the low-level
	audio manager is shut down.  Any calls made to this interface after Term() is
	called will fail except for Init() and IsInitialized(), which simply reports
	status and cannot fail.
	\sa Init(), IsInitialized()
	*/
	virtual void Term() = 0;
	/*!
	Determines if the sound manager is initialized.
	\return true indicates the manager is initialized, false indicates the 
	manager is not initialized.
	\sa Init(), Term()
	*/
	virtual bool IsInitialized() = 0;

	/*!
	Loads a sound script containing ISound, ISound3D, or ISoundScape
	definitions.
	\param sFileName name of the sound script to load
	\return true indicates success, false indicates failure.
	\sa RemoveAll()
	*/
	virtual bool LoadScript(std::string sFileName) = 0;
	/*!
	Removes all ISound, ISound3D, and ISoundscape definitions.  In addition,
	all sound objects currently associated with ISound or ISound3D
	definitions will be removed.
	\param sFileName name of the sound script to load
	\return true indicates success, false indicates failure.
	\sa RemoveAll()
	*/
	virtual bool RemoveAll() = 0;

	/*!
	Gets a cached sound if one is available.  If not, the function
	creates and returns a new sound object.
	\param sSoundName name of the sound definition as defined 
	in a loaded script file.
	\param pSound ISound interface to retrieve.
	\return true indicates success, false indicates failure.
	\sa ReleaseSound()
	*/
	virtual bool GetSound(std::string sSoundName, ISound*& pSound) = 0;
	/*!
	Retrieves a sound initialization structure for a given sound script
	definition.  This allows you to create sounds indepenent of the 
	sound caching mechanism while still using the script system.
	\return true indicates success, false indicates failure.
	*/
	virtual bool GetSoundInit(std::string sSoundName, SoundInit& init) = 0;
	virtual bool ReleaseSound(std::string sSoundName, ISound* pSound) = 0;
	virtual bool IsSoundRegistered(std::string sSoundName) = 0;
	virtual void ResetSoundItr() = 0;
	virtual bool GetNextSound(std::string& sSoundName) = 0;
	virtual bool RemoveSound(std::string sSoundName) = 0;
	virtual bool RemoveAllSounds() = 0;

	/*!
	Gets a cached 3D sound if one is available.  If not, the function
	creates and returns a new 3D sound object.
	\param sSound3DName name of the 3D sound definition as defined 
	in a loaded script file.
	\param pSound3D ISound3D interface to retrieve.
	\return true indicates success, false indicates failure.
	\sa ReleaseSound3D()
	*/
	virtual bool GetSound3D(std::string sSound3DName, ISound3D*& pSound3D) = 0;
	/*!
	Retrieves a 3D sound initialization structure for a given sound script
	definition.  This allows you to create 3D sounds indepenent of the 
	sound caching mechanism while still using the script system.
	\return true indicates success, false indicates failure.
	*/
	virtual bool GetSound3DInit(std::string sSound3DName, Sound3DInit& init) = 0;
	virtual bool ReleaseSound3D(std::string sSound3DName, ISound3D* pSound3D) = 0;
	virtual bool IsSound3DRegistered(std::string sSound3DName) = 0;
	virtual void ResetSound3DItr() = 0;
	virtual bool GetNextSound3D(std::string& sSound3DName) = 0;
	virtual bool RemoveSound3D(std::string sSound3DName) = 0;
	virtual bool RemoveAllSounds3D() = 0;

	virtual bool CreateSoundScape(ISoundScape*& pSoundScape) = 0;
	virtual bool InitSoundScape(std::string sSoundScapeName, ISoundScape*& pSoundScape) = 0;
	virtual bool IsSoundScapeRegistered(std::string sSoundScapeName) = 0;
	virtual void ResetSoundScapeItr() = 0;
	virtual bool GetNextSoundScape(std::string& sSoundScapeName) = 0;
	virtual bool RemoveSoundScape(std::string sSoundScapeName) = 0;
	virtual bool RemoveAllSoundScapes() = 0;
};

/*!
SoundMgr() is used to access a single static sound manager object from anywhere 
in the code.
\sa ISoundManager
*/
inline static ISoundManager* SoundMgr()
{  return AudioScriptFactory::GetSoundMgr();  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}; // namespace Audio
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif // ISOUND_H__