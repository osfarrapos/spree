/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __LISTENER_H
#define __LISTENER_H

#include "Audio.h"
#include "PropertySet.h"

namespace Audio
{

class Listener : public IListener
{
public:
	bool Init(const ListenerInit& init);
	void Term();

	bool SetProperties(const ListenerProp& prop);
	void GetProperties(ListenerProp& prop) const;

	bool SetPosition(const AUDIOVECTOR& vPos);
	void GetPosition(AUDIOVECTOR& vPos) const;

	bool SetVelocity(const AUDIOVECTOR& vVel);
	void GetVelocity(AUDIOVECTOR& vVel) const;

	bool SetOrientation(const AUDIOVECTOR& vFront, const AUDIOVECTOR& vTop);
	void GetOrientation(AUDIOVECTOR& vFront, AUDIOVECTOR& vTop) const;

	bool SetDistanceFactor(float fDistanceFactor);
	void GetDistanceFactor(float& fDistanceFactor) const;

	bool SetDopplerFactor(float fDopplerFactor);
	void GetDopplerFactor(float& fDopplerFactor) const;

	bool SetRolloffFactor(float fRolloffFactor);
	void GetRolloffFactor(float& fRolloffFactor) const;

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


	void CommitDeferredSettings();
	static Listener* CreateObject();
	static void DestroyObject(Listener* pListener);


private:
	Listener();
	virtual ~Listener();

	void Clear();



private:

	IDirectSound3DListener8*	m_pListener;
	IDirectSoundBuffer*			m_pDSBuffer;
	PropertySet					m_PropertySet;
	
	ListenerInit				m_Init;
};

}; // namespace Audio

#endif // __LISTENER_H