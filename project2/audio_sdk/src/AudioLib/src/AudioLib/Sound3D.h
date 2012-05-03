/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __SOUND3D_H
#define __SOUND3D_H

#include <dsound.h>
#include "Audio.h"
#include "Sound.h"

namespace Audio
{

class Sound3D : public ISound3D
{
DEFINE_POOL(Sound3D);

public:
	bool Init(const Sound3DInit& create);
	void Destroy();

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

	bool SetVolume(float fVolume);
	bool GetVolume(float &fVolume) const;

	bool SetPitch(float fPitch);
	bool GetPitch(float& fPitch) const;

	bool SetReadCursor(uint32 nBytes);
	bool GetReadCursor(uint32& nBytes) const;
	bool GetSourceSize(uint32& nBytes) const;

	// All 3d properties
	bool SetProperties(const Sound3DProp& prop);
	bool GetProperties(Sound3DProp& prop) const;

	// 3D Properties
	bool SetPosition(const AUDIOVECTOR& vPosition);
	bool GetPosition(AUDIOVECTOR& vPosition) const;

	bool SetVelocity(const AUDIOVECTOR& vVelocity);
	bool GetVelocity(AUDIOVECTOR& vVelocity) const;

	bool SetMaxDistance(float fMaxDist);
	bool GetMaxDistance(float& fMaxDist) const;

	bool SetMinDistance(float fMinDist);
	bool GetMinDistance(float& fMinDist) const;

	bool SetConeAngles(uint32 nInside, uint32 nOutside);
	bool GetConeAngles(uint32& nInside, uint32& nOutside) const;

	bool SetConeOrientation(const AUDIOVECTOR& vOrientation);
	bool GetConeOrientation(AUDIOVECTOR& vOrientation) const;

	bool SetConeOutsideVolume(float fVolume);
	bool GetConeOutsideVolume(float& fVolume) const;

	bool SetMode(uint32 nMode);
	bool GetMode(uint32& nMode) const;

	// Generic property support (for driver-specific extensions)
	bool QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport);
	bool Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
		uint32 nPropLength, uint32* pBytesReturned);
	bool Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
		uint32 nPropLength,
		bool bStoreProperty);

public:
	void Term();

	void Clear();

	bool operator < (const Sound3D& snd) const;

	bool IsLoading() const			{  return m_bLoading;  }
	bool DoLoad();

	inline bool IsMusic() const		{  return m_Init.m_bMusic;  }

// Private data
private:

	Sound3D();
	virtual ~Sound3D();

	LPDIRECTSOUND3DBUFFER8	m_p3DBuffer;
	PropertySet				m_PropertySet;
	Sound*					m_pSound;
	bool					m_bInitialized;
	

	// Initial sound properties set at Init()
	Sound3DInit				m_Init;

	// Load and Play status flags
	bool					m_bQueuePlayback;
	bool					m_bLoading;
	bool					m_bLoaded;

};

}; // namespace Audio


#endif // __SOUND3D_H