/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __IAUDIO_H__
#define __IAUDIO_H__

#include "XAudioPackage.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace Audio
{
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/*! \file IAudio.h*/ 

//! VOLUME_MIN represents the minimum sound on any audio system volume control.
static const float VOLUME_MIN			= 0.0f;
//! VOLUME_MAX represents the maximum sound on any audio system volume control.
static const float VOLUME_MAX			= 1.0f;
//! Represents the leftmost value for pan settings
static const float PAN_LEFT				= -1.0f;
//! Represents the center value for pan settings
static const float PAN_CENTER			= 0.0f;
//! Represents the rightmost value for pan settings
static const float PAN_RIGHT			= 1.0f;
//! Represents the original sound's pitch
static const float PITCH_ORIGINAL		= 1.0f;
//! Sets a 3D object to normal listening mode
static const uint32 MODE_NORMAL			= 0x00000000;
//! Sets a 3D object to position itself relative to the listener position
static const uint32 MODE_HEADRELATIVE	= 0x00000001;
//! Disables all 3D positional processing of a 3D sound object
static const uint32 MODE_DISABLE		= 0x00000002;


//! Base class defining common interface of all managed audio components 
/*!
IAudioBase defines the common interface to which all basic audio components (such as 
sounds, scripts, and DLS collections must conform.  Objects are obtained from
the AudioMgr, and are destroyed by using the Destroy() function, which internally
cleans up and deletes the object.  Once the object is initialized via its own Init()
function, the audio data may be loaded and unloaded at will using the Load() and
Unload() functions.  The audio manager may also call Load() or Unload() at its
discretion, since these are defined as no argument functions, and operate entirly
using cached information from the object's original initialization.
*/
class IAudioBase 
{
public:
	/*!
	Destroys an audio object, discarding any information stored from the original
	object initialization.  The object should be considered invalid for any use
	after this function is called.  The audio manager will automatically destroy 
	the object when IAudioManager::Term() is called.
	*/
	virtual void Destroy() = 0;

	/*!
	Determines whether an audio object is currently initialized.
	\return true indicates the object is initialized, false indicates it is not
	*/
	virtual bool IsInitialized() const = 0;

	/*!
	Loads the audio file from disk or memory and creates internal structures
	to hold this data as appropriate.  In general, objects derived from
	IPlayable will load data in a background thread, but all other
	objects will finish loading before the function returns.
	\return true indicates success, false indicates failure.
	\sa Unload(), IsLoaded()
	*/
	virtual bool Load() = 0;
	/*!
	Destroys the internal data container (buffer, segment, etc) currently holding
	audio data.
	\return true indicates success, false indicates failure.
	\sa Load(), IsLoaded()
	*/
	virtual bool Unload() = 0;
	/*!
	Determines if an audio object is currently loaded.
	\return true indicates the object is loaded, false indicates it is not loaded
	\sa Load(), Unload()
	*/
	virtual bool IsLoaded() const = 0;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
protected:
	virtual ~IAudioBase() {};
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

};


//! Base class defining common interface of all playable audio components 
/*!
IPlayable defines the common interface to which all basic audio components 
which can play sound or music must conform.  Basic control functions such
as Play(), Pause(), and Stop() are defined, as well as status functions
to ascertain the current state of the object.  Playable objects will
automatically attempt to call Load() if the object is not currently 
loaded when it first attempts to play.  If needed, the object will load
in a background thread and begin playback immediately after the loading
is completed.
*/
class IPlayable : public IAudioBase
{
public:

	/*!
	Plays the audio data as appropriate.  Note that looping is considered to be
	a property of the audio object, not a parameter of the play function.
	\return true indicates success, false indicates failure.
	*/
	virtual bool Play() = 0;
	/*!
	Pauses the object playback, leaving the play cursor where it is.  Calling Play()
	after Pause() causes playback to resume where it was stopped, if seeking is
	supported (note - Segments will start playback from the beginning when paused).
	\return true indicates success, false indicates failure.
	\sa IsPaused()
	*/
	virtual bool Pause() = 0;
	/*!
	Stops the audio object playback, resetting the play cursor to the beginning.
	\return true indicates success, false indicates failure.
	*/
	virtual bool Stop() = 0;

	/*!
	Checks to see if the audio object is currently playing.
	\return true indicates the audio object is playing, otherwise returns false.
	\sa Play(), Stop(), Pause()
	*/
	virtual bool IsPlaying() const = 0;
	/*!
	Checks to see if Pause() if this audio object has been paused.
	\return true indicates the audio object is playing, otherwise returns false.
	\sa Pause()
	*/
	virtual bool IsPaused() const = 0;
	/*!
	This function checks to see if the object is set to continuously loop.
	\return true if the audio object is set to continuously loop
	*/
	virtual bool IsLooping() const = 0;
};

//! 2D sound properties structure
/*!
Structure containing general runtime-alterable properties
of a 2D sound.
\sa ISound::SetProperties(), ISound::GetProperties()
*/
struct SoundProp
{
	/*!
	SoundProp constructor automatically calls Clear() to initialize the structure.
	*/
	SoundProp()	{ Clear(); }
	/*!
	Sets all members either to an appropriate default value if it exists, or else to zero.
	*/
	void Clear()
	{
		m_fVolume = VOLUME_MAX;
		m_fPan = PAN_CENTER;
		m_fPitch = PITCH_ORIGINAL;
		m_nReadCursor = 0;
	}
	//! Set the volume from VOLUME_MIN (0.0) to VOLUME_MAX (1.0)
	float	m_fVolume;
	//! Set the pan from PAN_LEFT (-1.0) to PAN_RIGHT (1.0)
	float	m_fPan;
	//! Sets the target pitch
	float	m_fPitch;
	//! Sets the position of the read cursor
	uint32	m_nReadCursor;

};

//! 2D sound initialization structure
/*!
Initialization structure for the 2D ISound class.  All initial parameters can be
set in this structure.  Generally speaking, parameters listed in this structure
are crucial to the initial contruction of the sound object, and so cannot be
modified at run-time.
\sa ISound::Init()
*/
struct SoundInit
{
	/*!
	SoundInit constructor automatically calls Clear() to initialize the structure.
	*/
	SoundInit()	{ Clear(); }
	/*!
	Sets all members either to an appropriate default value if it exists, or else to zero.
	*/
	void Clear()
	{
		m_sFileName.erase();
		m_bStreaming = false;
		m_bLooping = false;
		m_bMusic = false;
		m_nPriority = 0;
		m_Prop.Clear();
		m_pPackage = NULL;
	}
	//! The file from which to load the sound data
	std::string m_sFileName;
	//! Should the file be incrementally streamed from the disk?
	bool		m_bStreaming;
	//! Should the sound be continously looped?
	bool		m_bLooping;
	//! Is this music?
	bool		m_bMusic;
	//! User-defined sound priority setting.
	/*!
	This value is used (among other determinations) when deciding which sound
	should be discarded if the buffer limit is reached.
	*/
	uint32		m_nPriority;
	//! Used to set the initial properties of the sound
	/*!
	All values in in SoundProp can also be set at run-time
	\sa SoundProp
	*/
	SoundProp	m_Prop;

	_XAudioPackageArchive* m_pPackage;
};


//! ISound is the interface for 2D sound objects.
/*!
ISound-derived objects are representation of 2D sounds.  These objects should be used
for any sound that does not require true 3D positioning.  The interface provides specific
initalization routines, pan controls, and read cursor controls.
*/
class ISound : public IPlayable
{
public:
	/*!
	Initializes the 2D sound parameters.  This must be the first function
	called on the sound after the object is created.
	\param init is the initialization structure for 2D sounds, containing
	pamrameters needed to create the sound.
	\return true indicates success, false indicates failure
	\sa SoundInit, Destroy()
	*/
	virtual bool Init(const SoundInit& init) = 0;

	/*!
	Sets all properties simultaneously.
	\param prop contains all properties to set.
	\return true indicates success, false indicates failure
	\sa GetProperties()
	*/
	virtual bool SetProperties(const SoundProp& prop) = 0;
	/*!
	Gets all properties simultaneously.
	\param prop is a structure holding all modifiable properties.
	\return true indicates success, false indicates failure.
	\sa SetProperties()
	*/
	virtual bool GetProperties(SoundProp& prop) const = 0;

	/*!
	Sets the individual volume of a 2D sound.  This value will decrease
	the volume in addition to any decrease from maximum volume set in the master
	volume controls.
	\param fVolume sets the volume desired
	\return true indicates success, false indicates failure.
	\sa GetVolume()
	*/
	virtual bool SetVolume(float fVolume) = 0;
	/*!
	Gets the volume for this audio object.
	\param fVolume retrieves the current object's volume
	\return true indicates success, false indicates failure.
	\sa SetVolume()
	*/
	virtual bool GetVolume(float& fVolume) const = 0;

	/*!
	Sets the pan value on the sound.  This function could fail if the buffers is
	not allowed to make pan changes due to hardware limitations or conflicting
	parameters.
	\param fPan is automatically clamped between AUD_PAN_LEFT and AUD_PAN_RIGHT.  
	\return true indicates success, false indicates failure.
	\sa GetPan()
	*/
	virtual bool SetPan(float fPan) = 0;
	/*!
	Gets the current pan value.
	\param fPan will contain the current pan value for this buffer.  The value will
	be between AUD_PAN_LEFT and AUD_PAN_RIGHT.
	\return true indicates success, false indicates failure.
	\sa SetPan()
	*/
	virtual bool GetPan(float& fPan) const = 0;

	/*!
	Sets the desired frequency of the audio object relative to its original
	frequency.  For instance, a value of 0.5 sets it to half the original pitch,
	and a value of 2.0 doubles the pitch.  Note that the pitch range may be 
	constained by the sample's original pitch and the range allowed in hardware.
	\return true indicates success, false indicates failure.
	\sa GetPitch()
	*/
	virtual bool SetPitch(float fPitch) = 0;
	/*!
	Gets the objects frequency in hertz
	\return true indicates success, false indicates failure.
	\sa SetPitch()
	*/
	virtual bool GetPitch(float& fPitch) const = 0;

	/*!
	Sets the current read cursor position in bytes in the current source.
	\param nBytes sets the number of bytes from the beginning of the source.
	\return true indicates success, false indicates failure.
	\sa GetReadCursor()
	*/
	virtual bool SetReadCursor(uint32 nBytes) = 0;
	/*!
	Gets the current read cursor position in bytes.
	\param nBytes will be the number of bytes from the beginning of the source.
	\return true indicates success, false indicates failure.
	\sa SetReadCursor()
	*/
	virtual bool GetReadCursor(uint32& nBytes) const = 0;

	/*!
	Retrieves the total size of the current source in bytes.
	\param nBytes total size retrieved of the source in bytes.
	\return true indicates success, false indicates failure.
	*/
	virtual bool GetSourceSize(uint32& nBytes) const = 0;
};


//! Property class representing all commonly modified run-time sound properties
/*!
This class represents all 3D parameters commonly modified at run-time.  They are
presented in a common structure to allow the game (or the engine) to retrieve and/or
modify all 3D parameters of a sound with a single function call.
*/
struct Sound3DProp
{
	/*!
	Sound3DProp constructor automatically calls Clear() to initialize the structure.
	*/
	Sound3DProp()	{ Clear(); }
	/*!
	Sets all members either to an appropriate default value if it exists, or else to zero.
	*/
	void Clear()
	{
		m_vPosition.x = 0;  m_vPosition.y = 0;  m_vPosition.z = 0;
		m_vVelocity.x = 0;  m_vVelocity.y = 0;  m_vVelocity.z = 0;
		m_vConeOrientation.x = 0;  m_vConeOrientation.y = 0;  m_vConeOrientation.z = 1.0f;
		m_nInsideConeAngle = 360;
		m_nOutsideConeAngle = 360;
		m_fConeOutsideVolume = VOLUME_MAX;
		m_fMinDistance = 1.0f;
		m_fMaxDistance = 1000.0f;
		m_nMode = MODE_NORMAL;
		m_fVolume = VOLUME_MAX;
		m_fPitch = PITCH_ORIGINAL;
		m_nReadCursor = 0;
	}
	//! Represents the sound's current position
	AUDIOVECTOR	m_vPosition;
	//! Represents the sound's current velocity
	AUDIOVECTOR	m_vVelocity;
	//! Represents the sound's current orientation
	AUDIOVECTOR	m_vConeOrientation;
	//! Represents the inside angle of the sound cone in degrees.
	/*!
	This value is only valid from 0 to 360.
	*/
	uint32		m_nInsideConeAngle;
	//! Represents the outside angle of the sound cone in degrees.
	/*!
	This value is only valid from 0 to 360.
	*/
	uint32		m_nOutsideConeAngle;
	//! Volume outside the outer cone angle from AUD_VOLUME_MIN to AUD_VOLUME_MAX.
	float		m_fConeOutsideVolume;
	//! Minimum distance at which a sound begins to attenuate with distance.
	float		m_fMinDistance;
	//! Maximum range at which a sound can be heard.
	float		m_fMaxDistance;
	//! Specifies the processing mode for this 3D sound.
	/*!
	\sa ISound3D::SetMode()
	*/
	uint32		m_nMode;
	//! Set the volume from VOLUME_MIN (0.0) to VOLUME_MAX (1.0)
	float	m_fVolume;
	//! Sets the target pitch
	float	m_fPitch;
	//! Sets the position of the read cursor
	uint32	m_nReadCursor;
};


//! Initialization structure for the 3D ISound3D class.  
/*!
All initial parameters can be set in this structure.  Generally speaking, parameters 
listed in this structure are crucial to the initial contruction of the sound object, 
and so cannot be modified at run-time.
\sa ISound3D::Init()
*/
struct Sound3DInit
{
	/*!
	Sound3DInit constructor automatically calls Clear() to initialize the structure.
	*/
	Sound3DInit()	{ Clear(); }
	/*!
	Sets all members either to an appropriate default value if it exists, or else to zero.
	*/
	void Clear()
	{
		m_sFileName.erase();
		m_bStreaming = false;
		m_bLooping = false;
		m_bMusic = false;
		m_nPriority = 0;
		m_Prop.Clear();
		m_pPackage = NULL;
	}
	//! The file from which to load the sound data
	std::string m_sFileName;
	//! Should the file be incrementally streamed from the disk?
	bool		m_bStreaming;
	//! Should the sound be continously looped?
	bool		m_bLooping;
	//! Is this music?
	bool		m_bMusic;
	//! User-defined sound priority setting.
	/*!
	This value is used (among other determinations) when deciding which sound
	should be discarded if the buffer limit is reached.
	*/
	uint32		m_nPriority;
	//! Used to set the initial properties of the sound
	/*!
	All values in in Sound3DProp can also be set at run-time
	\sa Sound3SProp
	*/
	Sound3DProp	m_Prop;

	_XAudioPackageArchive* m_pPackage;
};


//! ISound3D is the interface for 3D sound objects.
/*!
ISound3D-derived objects represent sounds located in 3D world or local space.  
The ISound3D interface provides specific initalization routines and 3d functions,
as well as functions for setting the read cursor position and getting the source
size.
*/
class ISound3D : public IPlayable
{
public:
	/*!
	Initializes the 2D sound parameters.  This must be the first function
	called on the sound after the object is created.
	\param init is the initialization structure for 2D sounds, containing
	pamrameters needed to create the sound.
	\return true indicates success, false indicates failure.
	\sa Sound3DInit, Destroy()
	*/
	virtual bool Init(const Sound3DInit& init) = 0;
	
	/*!
	Sets all 3D properties simultaneously.
	\param prop contains all 3d properties to set.
	\return true indicates success, false indicates failure
	\sa GetProperties()
	*/
	virtual bool SetProperties(const Sound3DProp& prop) = 0;
	/*!
	Gets all 3D properties simultaneously.
	\param prop is a structure holding all 3d properties.
	\return true indicates success, false indicates failure.
	\sa Set3DProperties()
	*/
	virtual bool GetProperties(Sound3DProp& prop) const = 0;

	/*!
	Sets the current position of the sound source.
	\param vPosition sets the position in 3D space.
	\return true indicates success, false indicates failure.
	\sa GetPosition()
	*/
	virtual bool SetPosition(const AUDIOVECTOR& vPosition) = 0;
	/*!
	Retrieves the current position of the sound source.
	\param vPosition gets the position in 3D space.
	\return true indicates success, false indicates failure.
	\sa SetPosition()
	*/
	virtual bool GetPosition(AUDIOVECTOR& vPosition) const = 0;

	/*!
	Sets the current velocity of the sound source.
	\param vPosition sets the velocity in 3D space.
	\return true indicates success, false indicates failure.
	\sa GetVelocity()
	*/
	virtual bool SetVelocity(const AUDIOVECTOR& vVelocity) = 0;
	/*!
	Retrieves the current velocity of the sound source.
	\param vPosition gets the velocity in 3D space.
	\return true indicates success, false indicates failure.
	\sa SetVelocity()
	*/
	virtual bool GetVelocity(AUDIOVECTOR& vVelocity) const = 0;

	/*!
	Sets the maximum distance at which a sound can be heard.
	\param fMaxDist is distance to set in world units.
	\return true indicates success, false indicates failure.
	\sa GetMaxDistance()
	*/
	virtual bool SetMaxDistance(float fMaxDist) = 0;
	/*!
	Gets the maximum distance at which a sound can be heard.
	\param fMaxDist is distance to get in world units.
	\return true indicates success, false indicates failure.
	\sa SetMaxDistance()
	*/
	virtual bool GetMaxDistance(float& fMaxDist) const = 0;

	/*!
	Sets the minimum distance at which a sound begins to attenuate with distance.
	\param fMinDist is distance to set in world units.
	\return true indicates success, false indicates failure.
	\sa GetMinDistance()
	*/
	virtual bool SetMinDistance(float fMinDist) = 0;
	/*!
	Gets the minimum distance at which a sound begins to attenuate with distance.
	\param fMinDist is distance to get in world units.
	\return true indicates success, false indicates failure.
	\sa SetMinDistance()
	*/
	virtual bool GetMinDistance(float& fMinDist) const = 0;

	/*!
	Sets the inside and outside cone angles of the sound.  Paramater values are in
	degrees, and valid ranges are 0 to 360.
	\param nInside is the inside angle in degrees.
	\param nOutside is the outside angle in degrees.
	\return true indicates success, false indicates failure.
	\sa GetConeAngles()
	*/
	virtual bool SetConeAngles(uint32 nInside, uint32 nOutside) = 0;
	/*!
	Gets the inside and outside cone angles of the sound.  Paramater values are in
	degrees, and valid ranges are 0 to 360.
	\param nInside is the inside angle in degrees.
	\param nOutside is the outside angle in degrees.
	\return true indicates success, false indicates failure.
	\sa SetConeAngles()
	*/
	virtual bool GetConeAngles(uint32& nInside, uint32& nOutside) const = 0;

	/*!
	Sets the orientation of the sound cone.
	\param vOrientation is the orientation of the sound.
	\return true indicates success, false indicates failure.
	\sa GetConeOrientation()
	*/
	virtual bool SetConeOrientation(const AUDIOVECTOR& vOrientation) = 0;
	/*!
	Gets the orientation of the sound cone.
	\param vOrientation is the orientation of the sound.
	\return true indicates success, false indicates failure.
	\sa SetConeOrientation()
	*/
	virtual bool GetConeOrientation(AUDIOVECTOR& vOrientation) const = 0;

	/*!
	Sets the volume outside the sound cone.
	\param fVolume sets the outside cone volume.  Valid range is from
	AUD_VOLUME_MIN to AUD_VOLUME_MAX.
	\return true indicates success, false indicates failure.
	\sa GetConeOutsideVolume()
	*/
	virtual bool SetConeOutsideVolume(float fVolume) = 0;
	/*!
	Gets the volume outside the sound cone.
	\param fVolume gets the outside cone volume.  Valid range is from
	AUD_VOLUME_MIN to AUD_VOLUME_MAX.
	\return true indicates success, false indicates failure.
	\sa SetConeOutsideVolume()
	*/
	virtual bool GetConeOutsideVolume(float& fVolume) const = 0;

	/*!
	Sets the 3D sound processing mode.
	\param nMode is the processing mode.  Possible values are:
	- DS3DMODE_DISABLE: 3D processing is disabled.  Sound appears to originate from the
	listener's head.
	- DS3DMODE_HEADRELATIVE: All 3D parameters are relative to the listener's position and
	orientation.
	- DS3DMODE_NORMAL: This is the standard, default mode.  All 3D parameters are in
	absolute world coordinates.
	\return true indicates success, false indicates failure.
	\sa GetMode()
	*/
	virtual bool SetMode(uint32 nMode) = 0;
	/*!
	Sets the 3D sound processing mode.
	\param nMode is the processing mode.  See SetMode() for possible values.
	\return true indicates success, false indicates failure.
	\sa SetMode()
	*/
	virtual bool GetMode(uint32& nMode) const = 0;

	/*!
	Sets the individual volume of a 3D sound.  This value will decrease
	the volume in addition to any decrease from maximum volume set in the master
	volume controls.
	\param fVolume sets the volume desired
	\return true indicates success, false indicates failure.
	\sa GetVolume()
	*/
	virtual bool SetVolume(float fVolume) = 0;
	/*!
	Gets the volume for this audio object.
	\param fVolume retrieves the current object's volume
	\return true indicates success, false indicates failure.
	\sa SetVolume()
	*/
	virtual bool GetVolume(float& fVolume) const = 0;

	/*!
	Sets the desired frequency of the audio object relative to its original
	frequency.  For instance, a value of 0.5 sets it to half the original pitch,
	and a value of 2.0 doubles the pitch.  Note that the pitch range may be 
	constained by the sample's original pitch and the range allowed in hardware.
	\return true indicates success, false indicates failure.
	\sa GetPitch()
	*/
	virtual bool SetPitch(float fPitch) = 0;
	/*!
	Gets the objects frequency in hertz
	\return true indicates success, false indicates failure.
	\sa SetPitch()
	*/
	virtual bool GetPitch(float& fPitch) const = 0;

	/*!
	Sets the current read cursor position in bytes in the current source.
	\param nBytes sets the number of bytes from the beginning of the source.
	\return true indicates success, false indicates failure.
	\sa GetReadCursor()
	*/
	virtual bool SetReadCursor(uint32 nBytes) = 0;
	/*!
	Gets the current read cursor position in bytes.
	\param nBytes will be the number of bytes from the beginning of the source.
	\return true indicates success, false indicates failure.
	\sa SetReadCursor()
	*/
	virtual bool GetReadCursor(uint32& nBytes) const = 0;

	/*!
	Retrieves the total size of the current source in bytes.
	\param nBytes total size retrieved of the source in bytes.
	\return true indicates success, false indicates failure.
	*/
	virtual bool GetSourceSize(uint32& nBytes) const = 0;
	
	/*!
	Generic property support (for driver-specific extensions).  Querying an object
	allows a program to determine if driver-specific extensions are available on a
	user's system.  By exposing this at the API level, it ensures that the application
	has complete control over any property sets needed at the application level.
	See the DirectX Audio documentation and the documentation for the property
	set you wish to work with for details on how to use these parameters.
	\return true indicates success, false indicates failure.
	\sa Get(), Set()
	*/
	virtual bool QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport) = 0;
	/*!
	This function allows an application to get property set information from a
	driver.  See the DirectX Audio documentation and the documentation for the property
	set you wish to work with for details on how to use these parameters.
	\return true indicates success, false indicates failure.
	\sa QuerySupport(), Set()
	*/
	virtual bool Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, uint32 nPropLength, 
		uint32* pBytesReturned) = 0;
	/*!
	This function allows an application to set a driver-specific property.  
	See the DirectX Audio documentation and the documentation for the property
	set you wish to work with for details on how to use these parameters.
	\param bStoreProperty This specific audio extention tells the audio object
	to store this property setting and automatically restore the setting when
	an object is unloaded, and then reloaded.
	\return true indicates success, false indicates failure.
	\sa Get(), QuerySupport()
	*/
	virtual bool Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, uint32 nPropLength,
		bool bStoreProperty) = 0;
};


//! Initialization structure for the IDLS class.  
/*!
All initial parameters can be set in this structure.  Generally speaking, parameters 
listed in this structure are crucial to the initial contruction of the sound object, 
and so cannot be modified at run-time.
\sa IDLS::Init()
*/
struct DLSInit
{
	/*!
	Sound3DProp constructor automatically calls Clear() to initialize the structure.
	*/
	DLSInit()	{  Clear();  }
	/*!
	Sets all members either to an appropriate default value if it exists, or else to zero.
	*/
	void Clear()
	{
		m_sFileName.erase();
	}

	//! The file from which to load the DLS data
	std::string		m_sFileName;
};

//! Interface structure for DLS collections
/*!
The IDLS interface is designed to be used internally by segment objects.  A DLS
collection may be associated with an ISegment object through the 
SegmentInit::m_pDLS member.  Segments will internally call Lock() and Unlock()
to load or unload the DLS member explicitly as appropriate.
\sa DLSInit
*/
class IDLS : public IAudioBase
{
public:

	/*!
	Initializes the DLS object.  This must be the first function
	called on the sound after the object is created.
	\param init is the initialization structure for DLS objects, containing
	pamrameters needed to prepare the DLS object for use.
	\return true indicates success, false indicates failure.
	\sa Destroy()
	*/
	virtual bool Init(const DLSInit& init) = 0;
	/*!
	This loads all DLS data.  Unlike a typical load function, an internal 
	reference count is incremented with each call to Lock(), and will force
	the DLS data to remain loaded until either the reference count drops to
	zero or the Destoy() function is called.
	\return true if the lock was successful and/or the DLS collection was
	succesfully loaded, and false if there was an error loading the data.
	\sa Unlock()
	*/
	virtual bool Lock() = 0;
	/*!
	Decreases an internal reference count incremented by the Lock() function.
	When the reference count drops to zero, the DLS collection is unloaded.
	\return true indicates success, false indicates failure.
	\sa Lock()
	*/
	virtual bool Unlock() = 0;

};


//! Initialization structure for the ISegment class.  
/*!
All initial parameters can be set in this structure.  Generally speaking, 
parameters listed in this structure are crucial to the initial contruction 
of the sound object, and so cannot be modified at run-time.
\sa ISegment::Init()
*/
struct SegmentInit
{
	/*!
	SegmentInit constructor automatically calls Clear() to initialize the structure.
	*/
	SegmentInit()	{ Clear(); }
	/*!
	Sets all members either to an appropriate default value if it exists, or else to zero.
	*/
	void Clear()
	{
		m_sFileName.erase();
		m_bMusic = true;
		m_bLooping = false;
		m_pDLS = 0;
	}
	//! Source file used when creating segment object.
	/*!
	For Win32/DirectX platform, valid file types include DirectMusic segments,
	MIDI files, and wave files (uncompressed or ACM compressed).
	*/
	std::string			m_sFileName;
	//! Does the segment loop?
	bool				m_bLooping;
	//! Is this segment part of the music playback system? (defaults to true)
	bool				m_bMusic;
	//! This DLS object is used by the segment.
	/*!
	The segment will automatically call Lock() on the DLS interface, which
	increments an internal reference count and loads the DLS file whenever
	the segment is loaded, and calls Unlock() when it is unloaded.
	*/
	IDLS*				m_pDLS;

};


//! ISegment is the interface for segment audio objects
/*!
ISegment is the interface for sound and music segments.  Although segments are
primarily used as discrete components in interactive musical playback, they can
also be used as specialized sound effects.  Segments can contain either
MIDI data or digital audio data, and can be associated with a specific DLS 
collection using the SegmentInit::m_pDLS member.
*/
class ISegment : public IPlayable
{
public:
	/*!
	Initializes the segment object with the data needed to load and play the 
	segment.  
	\param init SegmentInit structure contains all initialization parameters.
	\return true indicates success, false indicates failure.
	\sa SegmentInit, Destroy()
	*/
	virtual bool Init(const SegmentInit& init) = 0;

};

//! Initialization structure for the IAudioScript class.  
/*!
All initial parameters can be set in this structure.  Generally speaking, parameters 
listed in this structure are crucial to the initial contruction of the sound object, 
and so cannot be modified at run-time.
\sa IAudioScript::Init()
*/
struct AudioScriptInit
{
	/*!
	AudioScriptInit constructor automatically calls Clear() to initialize the structure.
	*/
	AudioScriptInit()	{ Clear(); }
	/*!
	Sets all members either to an appropriate default value if it exists, or else to zero.
	*/
	void Clear()
	{
		m_sFileName.erase();
		m_bMusic = true;
	}
	//! Name of the script file
	std::string			m_sFileName;
	//! Is this script part of the music playback system? (defaults to true)
	bool				m_bMusic;

};

//! IAudioScript is the interface for DirectMusic script objects
/*!
Audio script objects allow an audio designer to perform custom actions through
the use of variables and routines that can be set, retrieved, and executed
from code.
*/
class IAudioScript : public IAudioBase
{
public:
	/*!
	Initializes the script object with the data needed to load and execute
	script functions.  
	\param init SegmentInit structure contains all initialization parameters.
	\return true indicates success, false indicates failure.
	\sa SegmentInit, Destroy()
	*/
	virtual bool Init(const AudioScriptInit& init) = 0;

	/*!
	Sets a named variable in a script to a numerical value.
	\param sVarName specifies the variable name to set
	\param iVal is the value to which the variable will be set
	\return true indicates success, false indicates failure.
	\sa GetVariable(), CallRoutine()
	*/
	virtual bool SetVariable(std::string sVarName, int32 iVal) = 0;
	/*!
	Retrieves the value of a named variable in a script.
	\param sVarName specifies the variable name to retrieve
	\param iVal which will hold the value of the script variable
	\return true indicates success, false indicates failure.
	\sa SetVariable(), CallRoutine()
	*/
	virtual bool GetVariable(std::string sVarName, int32& iVal) = 0;

	/*!
	Calls a specific routine in a script.
	\param sRoutineName indicates the name of the routine to call
	\return true indicates success, false indicates failure.
	\sa GetVariable(), SetVariable()
	*/
	virtual bool CallRoutine(std::string sRoutineName) = 0;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
protected:
	virtual ~IAudioScript() {};
#endif /* DOXYGEN_SHOULD_SKIP_THIS */
};


//! This structure contains all the 3D properties of the listener.
struct ListenerProp
{
	//! The structure clears all variables when the object is created
	ListenerProp()	{  Clear();  }
	//! Clear sets all variables to zero or to appropriate default values
	void Clear()
	{
		m_vPosition.x = 0.0f; m_vPosition.y = 0.0f; m_vPosition.z = 0.0f;
		m_vVelocity.x = 0.0f; m_vVelocity.y = 0.0f; m_vVelocity.z = 0.0f;
		m_vOrientFront.x = 0.0f; m_vOrientFront.y = 0.0f; m_vOrientFront.z = 1.0f;
		m_vOrientTop.x = 0.0f; m_vOrientTop.y = 1.0f; m_vOrientTop.z = 0.0f;
		m_fDistanceFactor = 1.0f;
		m_fRolloffFactor = 1.0f;
		m_fDopplerFactor = 1.0f;
	}
	//! Current position of the listener.
	AUDIOVECTOR	m_vPosition;
	//! Current velocity of the listener.
	AUDIOVECTOR	m_vVelocity;
	//! Vector defining the forward orientation of the listener.
	AUDIOVECTOR	m_vOrientFront;
	//! Vector defining the upward orientation of the listener.
	AUDIOVECTOR	m_vOrientTop;
	//! Number of meters per vector unit.
	float		m_fDistanceFactor;
	//! Attenuation of sound over distance.
	/*!
	\sa IListener::SetRolloffFactor() for parameter info
	*/
	float		m_fRolloffFactor;
	//! Adjusts the global doppler factor.
	/*!
	\sa IListener::SetDopplerFactor() for parameter info
	*/
	float		m_fDopplerFactor;
};

//! This structure contains all parameters needed for the IListener initialization.
struct ListenerInit
{
	//! The structure clears all variables when the object is created
	ListenerInit()	{  Clear();  }
	//! Clear sets all variables to zero or to appropriate default values
	void Clear()
	{
		m_Prop.Clear();
	}
	ListenerProp	m_Prop;
};


//! IListener represents the listener around which 3D sounds are oriented and positioned
/*!
This class places a virtual "microphone" at a designated position and orientation, and 
controls how 3D sounds are heard.  Properties can also be set and retrieved on the
listener object.  One example of this is environmental reverberation, such as EAX.
*/
class IListener
{
public:
	
	/*!
	Initializes the listener object with required initialization data.  
	\param init ListenerInit structure contains all initialization parameters.
	\return true indicates success, false indicates failure.
	\sa ListenerInit, Term()
	*/
	virtual bool Init(const ListenerInit& init) = 0;
	/*!
	Shuts down the listener object.  An application normally does not have to call
	this function since the audio manager will automatically terminate the listener
	when it exits.
	\sa Init()
	*/
	virtual void Term() = 0;

	/*!
	Sets all 3D listener properties in a single function call.
	\param prop ListenerProp sets all listener paramaters simultaneously.
	\return true indicates success, false indicates failure.
	\sa GetProperties()
	*/
	virtual bool SetProperties(const ListenerProp& prop) = 0;
	/*!
	Gets all 3D listener properties in a single function call.
	\param prop ListenerProp gets all listener paramaters simultaneously.
	\sa SetProperties()
	*/
	virtual void GetProperties(ListenerProp& prop) const = 0;

	/*!
	Sets the current listener position.
	\param vPos is the current position.
	\return true indicates success, false indicates failure.
	\sa GetPosition()
	*/
	virtual bool SetPosition(const AUDIOVECTOR& vPos) = 0;
	/*!
	Gets the current listener position.
	\param vPos is the current position.
	\sa SetPosition()
	*/
	virtual void GetPosition(AUDIOVECTOR& vPos) const = 0;

	/*!
	Sets the current listener velocity.
	\param vPos is the current velocity.
	\return true indicates success, false indicates failure.
	\sa GetVelocity()
	*/
	virtual bool SetVelocity(const AUDIOVECTOR& vVel) = 0;
	/*!
	Gets the current listener velocity.
	\param vPos is the current velocity.
	\sa GetVelocity()
	*/
	virtual void GetVelocity(AUDIOVECTOR& vVel) const = 0;

	/*!
	Sets the current listener orientation.
	\param vFront is the listener's forward orientation.
	\param vTop is the listener's topward orientation.
	\return true indicates success, false indicates failure.
	\sa GetOrientation()
	*/
	virtual bool SetOrientation(const AUDIOVECTOR& vFront, const AUDIOVECTOR& vTop) = 0;
	/*!
	Gets the current listener orientation.
	\param vFront is the listener's forward orientation.
	\param vTop is the listener's topward orientation.
	\sa SetOrientation()
	*/
	virtual void GetOrientation(AUDIOVECTOR& vFront, AUDIOVECTOR& vTop) const = 0;

	/*!
	Sets the listener's distance factor.
	\param fDistanceFactor is the number of meters per vector unit
	\return true indicates success, false indicates failure.
	\sa GetDistanceFactor()
	*/
	virtual bool SetDistanceFactor(float fDistanceFactor) = 0;
	/*!
	Gets the listener's distance factor.  
	\param fDistanceFactor is the number of meters per vector unit
	\sa SetDistanceFactor()
	*/
	virtual void GetDistanceFactor(float& fDistanceFactor) const = 0;

	/*!
	Sets the listener's doppler factor, which is the rate at which pitch will shift 
	when a sound is moving rapidly toward or away from the listener.  1.0,
	or DS3D_DEFAULTDOPPLERFACTOR is the default.  The range is from 0.0 to 10.0, 
	or DS3D_MINDOPPLERFACTOR to DS3D_MAXDOPPLERFACTOR.
	\param fDistanceFactor is the doppler factor value
	\return true indicates success, false indicates failure.
	\sa GetDopplerFactor()
	*/
	virtual bool SetDopplerFactor(float fDopplerFactor) = 0;
	/*!
	Gets the listener's doppler factor.  See SetDopplerFactor() for addition parameter
	information.
	\param fDistanceFactor is the doppler factor value
	\sa SetDopplerFactor()
	*/
	virtual void GetDopplerFactor(float& fDopplerFactor) const = 0;

	/*!
	The rolloff factor adjusts the amount a sound will attenuate over distance.  1.0,
	or DS3D_DEFAULTROLLOFFFACTOR is the default.  The range is from 0.0 to 10.0, 
	or DS3D_MINROLLOFFFACTOR to DS3D_MAXROLLOFFFACTOR.
	\param fRolloffFactor is the rolloff factor value
	\sa GetRolloffFactor()
	*/
	virtual bool SetRolloffFactor(float fRolloffFactor) = 0;
	/*!
	Gets the listener's rolloff factor.  See SetRolloffFactor() for addition parameter
	information.
	\param fRolloffFactor is the rolloff factor value
	\sa SetRolloffFactor()
	*/
	virtual void GetRolloffFactor(float& fRolloffFactor) const = 0;

	/*!
	Generic property support (for driver-specific extensions).  Querying an object
	allows a program to determine if driver-specific extensions are available on a
	user's system.  By exposing this at the API level, it ensures that the application
	has complete control over any property sets needed at the application level.
	See the DirectX Audio documentation and the documentation for the property
	set you wish to work with for details on how to use these parameters.
	\return true indicates success, false indicates failure.
	\sa Get(), Set()
	*/
	virtual bool QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport) = 0;
	/*!
	This function allows an application to get property set information from a
	driver.  See the DirectX Audio documentation and the documentation for the property
	set you wish to work with for details on how to use these parameters.
	\return true indicates success, false indicates failure.
	\sa QuerySupport(), Set()
	*/
	virtual bool Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, uint32 nPropLength, 
		uint32* pBytesReturned) = 0;
	/*!
	This function allows an application to set a driver-specific property.  
	See the DirectX Audio documentation and the documentation for the property
	set you wish to work with for details on how to use these parameters.
	\return true indicates success, false indicates failure.
	\sa Get(), QuerySupport()
	*/
	virtual bool Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, uint32 nPropLength,
		bool bStoreProperty) = 0;
};

//! Music callback interface
/*!
This class allows an object derived from this interface to receive specific
notification messages from the audio manager's low-level segment playback
system.  Current the only message received is when a new segment starts
playback.
*/
class IMusicCallback
{
public:
	/*!
	This function is called when a music segment begins playback.
	*/
	virtual void OnSegmentStart()	{};
};


//! Initialization structure for the IAudioManager class.  
/*!
All initial audio manager options are set in this structure
\sa IAudioManager::Init()
*/
struct AudioMgrInit
{
	/*!
	AudioMgrInit constructor automatically calls Clear() to initialize the structure.
	*/
	AudioMgrInit()	{ Clear(); }
	/*!
	Sets all members either to an appropriate default value if it exists, or else to zero.
	*/
	void Clear()
	{
		m_hWnd = (HWND)0;
		m_bLoadAsyncronously = true;
		m_bForceSoftware = false;
		m_bUseMusicReverb = true;
		m_bUseEAX = true;
		m_bUseZoomFX = true;
		m_bAutoStream = false;
		m_bCacheBuffers = false;
		m_n2DHardwareBufferMin = 8;
		m_n3DHardwareBufferMin = 8;
		m_n2DHardwareBufferMax = 256;
		m_n3DHardwareBufferMax = 256;
		m_n2DSoftwareBufferMax = 256;
		m_n3DSoftwareBufferMax = 256;
		m_nSegmentMax = 256;
		m_nOptimalSampleBits = 16;
		m_nOptimalSampleRate = 44100;
		m_sWorkingPath.erase();
		m_pAudioStreamFactory = 0;
		m_pMusicCallback = 0;
	}
	//! HWND used for sound system initialization
	HWND					m_hWnd;
	//! Load all audio files asyncronously
	bool					m_bLoadAsyncronously;
	//! Force the system to use all software buffers
	bool					m_bForceSoftware;
	//! Use the Waves Reverb filter on the DirectMusic music performance object
	bool					m_bUseMusicReverb;
	//! Use EAX hardware extensions if available
	bool					m_bUseEAX;
	//! Use ZoomFX hardware extensions if available
	bool					m_bUseZoomFX;
	//! Automatically stream all large buffers
	bool					m_bAutoStream;
	//! Automatically cache buffers for reuse
	bool					m_bCacheBuffers;
	//! Threshold at which software buffers are used instead of hardware
	uint32					m_n2DHardwareBufferMin;
	//! Threshold at which software buffers are used instead of hardware
	uint32					m_n3DHardwareBufferMin;
	//! Maximum amount of 2D hardware buffers to allow
	uint32					m_n2DHardwareBufferMax;
	//! Maximum amount of 3D hardware buffers to allow
	uint32					m_n3DHardwareBufferMax;
	//! Maximum amount of 2D software buffers to allow
	uint32					m_n2DSoftwareBufferMax;
	//! Maximum amount of 3D software buffers to allow
	uint32					m_n3DSoftwareBufferMax;
	//! Maximum amount of segments to allow
	uint32					m_nSegmentMax;
	//! Defines the optimal bit depth is for audio data
	uint32					m_nOptimalSampleBits;
	//! Defines the optimal sample rate is for audio data
	uint32					m_nOptimalSampleRate;
	//! Audio system default working path
	/*! 
	Specifies the working directory for the audio system.  
	If left blank, the audio manager will use the current working
	directory at the time of initialization.
	*/
	std::string				m_sWorkingPath;
	//! Allows overriding of the default disk-based file system
	IAudioStreamFactory*	m_pAudioStreamFactory;
	//! Sets a music callback object for notification on specific music segment events
	IMusicCallback*			m_pMusicCallback;
	
};

//! Statistics structure for the IAudioManager class.  
/*!
All initial audio manager statistics are set in this structure
\sa IAudioManager::GetStats()
*/
struct AudioMgrStats
{
	/*!
	AudioMgrStats constructor automatically calls Clear() to initialize the structure.
	*/
	AudioMgrStats()	{ Clear(); }
	/*!
	Sets all members either to an appropriate default value if it exists, or else to zero.
	*/
	void Clear()
	{
		m_bForce2DSoftware = false;
		m_bForce3DSoftware = false;
		m_n2DSoundsLoaded = 0;
		m_n3DSoundsLoaded = 0;
		m_nSegmentsLoaded = 0;
	}
	//! Are 2D buffers restricted to software?
	bool	m_bForce2DSoftware;
	//! Are 3D buffers restricted to software?
	bool	m_bForce3DSoftware;
	//! Reports the number of 2D sounds currently loaded
	uint32	m_n2DSoundsLoaded;
	//! Reports the number of 3D sounds currently loaded
	uint32	m_n3DSoundsLoaded;
	//! Reports the number of segments currently loaded
	uint32	m_nSegmentsLoaded;
};


//! The audio manager controls all low-level audio components.
/*!
It is responsible for initializing the sound and music subsystems, and for creating and
managing all 2D and 3D sounds (both static and streaming), as well as for queueing
and playing music segments.  The manager is designed to allow nearly direct access to
low level buffers and segments while still providing an easy-to-use and high-level
management system.
*/
class IAudioManager
{
public:

	/*!
	Initializes the audio manager.  This function is automatically called if using
	the standard engine framework.  Otherwise, you must call this manually before 
	attempting to play any sounds.
	\param init is the AudioMgrInit structure that contains initialization parameters 
	\return true indicates success, false indicates failure.
	\sa AudioMgrInit, Term()
	*/
	virtual bool Init(const AudioMgrInit& init) = 0;
	/*!
	Terminates the audio manager.  This should be called before the application exits to
	properly shut down the sound system.  This function is called automatically if using
	the standard engine framework.
	\sa Init()
	*/
	virtual void Term() = 0;

	/*!
	Determines whether an audio manager is currently initialized.
	\return true indicates the manager is initialized, false indicates it is not
	*/
	virtual bool IsInitialized() const = 0;

	/*!
	Gets current usage statistics from the audio manager.
	\param init is the AudioMgrInit structure that contains statistics fields 
	\return true indicates success, false indicates failure.
	\sa AudioMgrStats
	*/
	virtual bool GetStats(AudioMgrStats& stats) const = 0;

	/*!
	Creates a new sound object to be managed by the audio system.
	\param pSound holds the newly created ISound interface.
	\return true indicates success, false indicates failure.
	*/
	virtual bool CreateSound(ISound*& pSound) = 0;
	/*!
	Creates a new 3D sound object to be managed by the audio system.
	\param pSound3D holds the newly created ISound3D interface.
	\return true indicates success, false indicates failure.
	*/
	virtual bool CreateSound3D(ISound3D*& pSound3D) = 0;
	/*!
	Creates a new segment object to be managed by the audio system.
	\param pSegment holds the newly created ISegment interface.
	\return true indicates success, false indicates failure.
	*/
	virtual bool CreateSegment(ISegment*& pSegment) = 0;
	/*!
	Creates a new DLS object to be managed by the audio system.
	\param pDLS holds the newly created IDLS interface.
	\return true indicates success, false indicates failure.
	*/
	virtual bool CreateDLS(IDLS*& pDLS) = 0;

	/*!
	Creates a new audio script object to be managed by the audio system.
	\param pScript holds the newly created IAudioScript interface.
	\return true indicates success, false indicates failure.
	*/
	virtual bool CreateAudioScript(IAudioScript*& pScript) = 0;

	/*!
	Retrives the listener object.  This function will return the same object on every call.
	\param pListener holds the acquired IListener interface.
	\return true indicates success, false indicates failure.
	\sa IListener
	*/
	virtual bool GetListener(IListener*& pListener) = 0;
	
	/*!
	Sets the master volume for the audio system.
	\param fVolume sets the master volume.  Acceptable values range from AUD_VOLUME_MIN to
	AUD_VOLUME_MAX (0 to 1).  The volume control is linear.
	\return true indicates success, false indicates failure.
	\sa GetSoundVolume()
	*/
	virtual bool SetSoundVolume(float fVolume) = 0;
	/*!
	Gets the master volume from the audio system.
	\param fVolume gets the master volume.
	\return true indicates success, false indicates failure.
	\sa SetSoundVolume()
	*/
	virtual bool GetSoundVolume(float& fVolume) const = 0;

	/*!
	Sets the music volume.
	\param fVolume sets the music volume.  Acceptable values range from AUD_VOLUME_MIN to
	AUD_VOLUME_MAX (0 to 1).  The volume control is linear.
	\return true indicates success, false indicates failure.
	\sa GetMusicVolume()
	*/
	virtual bool SetMusicVolume(float fVolume) = 0;
	/*!
	Gets the music volume.
	\param fVolume gets the music volume.
	\return true indicates success, false indicates failure.
	\sa SetMusicVolume()
	*/
	virtual bool GetMusicVolume(float& fVolume) const = 0;

	/*!
	Stops all currently playing sounds and music.
	\return true indicates success, false indicates failure.
	\sa PauseAll(), ResumeAll()
	*/
	virtual bool StopAll() = 0;
	/*!
	Pauses all currently playing sounds and music.
	\return true indicates success, false indicates failure.
	\sa StopAll(), ResumeAll()
	*/
	virtual bool PauseAll() = 0;
	/*!
	Resumes all currently paused sounds and music.  Note that this will include 
	any sounds paused other than a call to PauseAll().
	\return true indicates success, false indicates failure.
	\sa StopAll(), PauseAll()
	*/
	virtual bool ResumeAll() = 0;

	/*!
	Gets the currently playing music segment from the audio system.  
	\param pSegment is the currently playing primary segment.  pSegment 
	will be NULL if no segment is currently playing.
	\return true indicates success, false indicates failure.
	\sa GetNextSegment()
	*/
	virtual bool GetCurrentSegment(ISegment*& pSegment) const = 0;
	/*!
	Gets the next queued music segment from the audio system.  
	\param pSegment is the next queued segment.  pSegment will be NULL if no segment
	is currently queued.
	\return true indicates success, false indicates failure.
	\sa GetCurrentSegment()
	*/
	virtual bool GetNextSegment(ISegment*& pSegment) const = 0;

	/*!
	Creates a file object based either on the user-defined file creation
	factory if one was supplied, or returns a default disk-based file
	object.
	\param pAudioFile holds a valid IAudioFile object on success
	\return true indicates success, false indicates failure.
	\sa DestroyAudioFile(), IAudioFileFactory, IAudioFile, AudioMgrInit
	*/
	virtual bool CreateAudioStream(IAudioStream*& pStream) = 0;
};

/*!
AudioMgr() is used to access a single static audio manager object from anywhere 
in the code.
\sa IAudioManager
*/
inline IAudioManager* AudioMgr()
{  return AudioLibFactory::GetAudioMgr();  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}; // namespace Audio
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif // __IAUDIO_H__