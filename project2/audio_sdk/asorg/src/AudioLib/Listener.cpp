/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "Listener.h"
#include "AudioCommon.h"
#include "AudioMgr.h"

using namespace std;
using namespace Audio;

//------------------------------------------------------------------------//
Listener::Listener()
{
	FN("Listener::Listener()");
	Clear();
}

//------------------------------------------------------------------------//
Listener::~Listener()
{
	FN("Listener::~Listener()");
	Term();
}


//------------------------------------------------------------------------//
void Listener::Clear()
{
	FN("Listener::Clear()");
	m_pListener = 0;
	m_PropertySet.Clear();
	m_pDSBuffer = 0;
	m_Init.Clear();
}


//------------------------------------------------------------------------//
bool Listener::Init(const ListenerInit& init)
{
	FN("Listener::Init()");

	if(!DXAudioMgr()->IsInitialized())
		return Error::Handle("Audio system is not yet initialized"); 

	// Set the initialization and property structure
	m_Init = init;

	// Get the listener interface from the primary buffer interface.
	IDirectSoundBuffer* pBuffer = DXAudioMgr()->PrimaryBuffer();
	HRESULT hr = pBuffer->QueryInterface(IID_IDirectSound3DListener8, (void**)&m_pListener);
	if(FAILED(hr))
		return Error::Handle("Error = %s", DXGetErrorString(hr)); 

	// Now that we have the listener interface, we're going to create a 
	// dedicated hardware buffer so that we can query and set hardware 
	// properties (such as EAX) on it.  Note that we don't return false
	// on failure after this point, because technically the listener 
	// object didn't fail to init.  The property set simply failed to
	// be created, which is not a fatal error.

	// Set the buffer format
	WAVEFORMATEX wf;
    memset(&wf, 0, sizeof(WAVEFORMATEX)); 
    wf.wFormatTag = WAVE_FORMAT_PCM; 
    wf.nChannels = 1; 
    wf.nSamplesPerSec = 22050; 
    wf.nBlockAlign = 2; 
    wf.nAvgBytesPerSec = 
        wf.nSamplesPerSec * wf.nBlockAlign; 
    wf.wBitsPerSample = 16; 

	// Create the actual sound buffer
	DSBUFFERDESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.dwSize = sizeof(desc);
	desc.dwFlags = DSBCAPS_LOCHARDWARE | DSBCAPS_CTRL3D;
	desc.lpwfxFormat = &wf;
	desc.dwBufferBytes = 1024;
	IDirectSoundBuffer* pDSBuffer;
	hr = DXAudioMgr()->DirectSound()->CreateSoundBuffer(&desc, &pDSBuffer, NULL);
	if(FAILED(hr))
	{
		Error::Log("No hardware buffers available for listener object");
		return true;
	}

	// Get the IDirectSoundBuffer8 interface
	hr = pDSBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_pDSBuffer);
	// Release the temporary DirectSoundBuffer interface
	_XASAFE_RELEASE(pDSBuffer);
	if(FAILED(hr))
	{
		Error::Log("Could not obtain DirectSoundBuffer8 interface for listener property set");
		return true;
	}

	// A null zoomfx setting is applied to the listener's sound buffer because
	// of a bug which cancels EAX effects the first time one is set.	
	m_PropertySet.OnLoad(m_pDSBuffer);
	
	return true;
}

//------------------------------------------------------------------------//
void Listener::Term()
{
	FN("Listener::Term()");		
	m_PropertySet.Term();
	_XASAFE_RELEASE(m_pDSBuffer);
	_XASAFE_RELEASE(m_pListener);
	Clear();
}


//------------------------------------------------------------------------//
bool Listener::SetProperties(const ListenerProp& prop)
{
	FN("Listener::SetProperties()");
	m_Init.m_Prop = prop;
	DS3DLISTENER d3dprop;
	memset(&d3dprop, 0, sizeof(DS3DLISTENER));
	d3dprop.dwSize = sizeof(DS3DLISTENER);
	d3dprop.vPosition = m_Init.m_Prop.m_vPosition;
	d3dprop.vVelocity = m_Init.m_Prop.m_vVelocity;
	d3dprop.vOrientFront = m_Init.m_Prop.m_vOrientFront;
	d3dprop.vOrientTop = m_Init.m_Prop.m_vOrientTop;
	d3dprop.flDistanceFactor = m_Init.m_Prop.m_fDistanceFactor;
	d3dprop.flDopplerFactor = m_Init.m_Prop.m_fDopplerFactor;
	d3dprop.flRolloffFactor = m_Init.m_Prop.m_fRolloffFactor;
	HRESULT hr = m_pListener->SetAllParameters(&d3dprop, DS3D_DEFERRED);
	if(FAILED(hr))
		return Error::Handle("Could not set all listener parameters.  Error = %s", DXGetErrorString(hr));
	return true;	
}


//------------------------------------------------------------------------//
void Listener::GetProperties(ListenerProp& prop) const
{
	FN("Listener::GetProperties()");
	prop = m_Init.m_Prop;
}


//------------------------------------------------------------------------//
bool Listener::SetPosition(const AUDIOVECTOR& vPos)
{
	FN("Listener::SetPosition()");
	m_Init.m_Prop.m_vPosition = vPos;
	HRESULT hr = m_pListener->SetPosition(
		m_Init.m_Prop.m_vPosition.x, 
		m_Init.m_Prop.m_vPosition.y, 
		m_Init.m_Prop.m_vPosition.z, 
		DS3D_DEFERRED);
	if(FAILED(hr))
		return Error::Handle("Could not set listener position.  Error = %s", DXGetErrorString(hr));
	return true;	
}


//------------------------------------------------------------------------//
void Listener::GetPosition(AUDIOVECTOR& vPos) const
{
	FN("Listener::GetPosition()");
	vPos = m_Init.m_Prop.m_vPosition;
}


//------------------------------------------------------------------------//
bool Listener::SetVelocity(const AUDIOVECTOR& vVel)
{
	FN("Listener::SetVelocity()");
	m_Init.m_Prop.m_vVelocity = vVel;
	HRESULT hr = m_pListener->SetVelocity(
		m_Init.m_Prop.m_vVelocity.x, 
		m_Init.m_Prop.m_vVelocity.y, 
		m_Init.m_Prop.m_vVelocity.z, 
		DS3D_DEFERRED);
	if(FAILED(hr))
		return Error::Handle("Could not set listener velocity.  Error = %s", DXGetErrorString(hr));
	return true;	
}


//------------------------------------------------------------------------//
void Listener::GetVelocity(AUDIOVECTOR& vVel) const
{
	FN("Listener::GetVelocity()");
	vVel = m_Init.m_Prop.m_vVelocity;
}


//------------------------------------------------------------------------//
bool Listener::SetOrientation(const AUDIOVECTOR& vFront, const AUDIOVECTOR& vTop)
{
	FN("Listener::SetOrientation()");
	m_Init.m_Prop.m_vOrientFront = vFront;
	m_Init.m_Prop.m_vOrientTop = vTop;
	HRESULT hr = m_pListener->SetOrientation(
		m_Init.m_Prop.m_vOrientFront.x, 
		m_Init.m_Prop.m_vOrientFront.y, 
		m_Init.m_Prop.m_vOrientFront.z, 
		m_Init.m_Prop.m_vOrientTop.x, 
		m_Init.m_Prop.m_vOrientTop.y, 
		m_Init.m_Prop.m_vOrientTop.z, 
		DS3D_DEFERRED);
	if(FAILED(hr))
		return Error::Handle("Could not set listener orientation.  Error = %s", DXGetErrorString(hr));
	return true;	
}


//------------------------------------------------------------------------//
void Listener::GetOrientation(AUDIOVECTOR& vFront, AUDIOVECTOR& vTop) const
{
	FN("Listener::GetOrientation()");
	vFront = m_Init.m_Prop.m_vOrientFront;
	vTop = m_Init.m_Prop.m_vOrientTop;
}


//------------------------------------------------------------------------//
bool Listener::SetDistanceFactor(float fDistanceFactor)
{
	FN("Listener::SetDistanceFactor()");
	m_Init.m_Prop.m_fDistanceFactor = fDistanceFactor;
	HRESULT hr = m_pListener->SetDistanceFactor(m_Init.m_Prop.m_fDistanceFactor, DS3D_DEFERRED);
	if(FAILED(hr))
		return Error::Handle("Could not set listener distance factor.  Error = %s", DXGetErrorString(hr));
	return true;	
}


//------------------------------------------------------------------------//
void Listener::GetDistanceFactor(float& fDistanceFactor) const
{
	FN("Listener::GetDistanceFactor()");
	fDistanceFactor = m_Init.m_Prop.m_fDistanceFactor;
}


//------------------------------------------------------------------------//
bool Listener::SetDopplerFactor(float fDopplerFactor)
{
	FN("Listener::SetDopplerFactor()");
	m_Init.m_Prop.m_fDopplerFactor = fDopplerFactor;
	HRESULT hr = m_pListener->SetDopplerFactor(m_Init.m_Prop.m_fDopplerFactor, DS3D_DEFERRED);
	if(FAILED(hr))
		return Error::Handle("Could not set listener doppler factor.  Error = %s", DXGetErrorString(hr));
	return true;	
}


//------------------------------------------------------------------------//
void Listener::GetDopplerFactor(float& fDopplerFactor) const
{
	FN("Listener::GetDopplerFactor()");
	fDopplerFactor = m_Init.m_Prop.m_fDopplerFactor;
}


//------------------------------------------------------------------------//
bool Listener::SetRolloffFactor(float fRolloffFactor)
{
	FN("Listener::SetRolloffFactor()");
	m_Init.m_Prop.m_fRolloffFactor = fRolloffFactor;
	HRESULT hr = m_pListener->SetRolloffFactor(m_Init.m_Prop.m_fRolloffFactor, DS3D_DEFERRED);
	if(FAILED(hr))
		return Error::Handle("Could not set listener rolloff factor.  Error = %s", DXGetErrorString(hr));
	return true;	
}


//------------------------------------------------------------------------//
void Listener::GetRolloffFactor(float& fRolloffFactor) const
{
	FN("Listener::GetRolloffFactor()");
	fRolloffFactor = m_Init.m_Prop.m_fRolloffFactor;
}


//------------------------------------------------------------------------//
void Listener::CommitDeferredSettings()
{
	FN("Listener::CommitDeferredSettings()");
	if(m_pListener)
		m_pListener->CommitDeferredSettings();
}


//------------------------------------------------------------------------//
Listener* Listener::CreateObject()
{
	FN("Listener::CreateObject()");
	return new Listener;
}

//------------------------------------------------------------------------//
void Listener::DestroyObject(Listener* pListener)
{
	FN("Listener::DestroyObject()");
	if(pListener)
		delete pListener;
}


//------------------------------------------------------------------------//
// Generic property support (for driver-specific extensions)
bool Listener::QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport)
{
	FN("Listener::QuerySupport()");
	return m_PropertySet.QuerySupport(guid, nID, pTypeSupport);
}


//------------------------------------------------------------------------//
bool Listener::Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
	uint32 nPropLength, uint32* pBytesReturned)
{
	FN("Listener::Get()");
	return m_PropertySet.Get(guidProperty, nID, pInstanceData, nInstanceLength, pPropData, 
		nPropLength, pBytesReturned);
}


//------------------------------------------------------------------------//
bool Listener::Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
	uint32 nPropLength, bool bStoreProperty)
{
	FN("Listener::Set()");
	return m_PropertySet.Set(
		guidProperty, 
		nID, 
		pInstanceData, 
		nInstanceLength, 
		pPropData, 
		nPropLength,
		bStoreProperty);
}
