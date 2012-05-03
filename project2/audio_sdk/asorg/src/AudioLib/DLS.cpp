/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "DLS.h"
#include "AudioCommon.h"
#include "AudioMgr.h"

using namespace Audio;

IMPLEMENT_POOL(DLS);

//------------------------------------------------------------------------//
DLS::DLS()
{
	FN("DLS::DLS()");
	Clear();
}


//------------------------------------------------------------------------//
DLS::~DLS()
{
	FN("DLS::~DLS()");
	Term();
}


//------------------------------------------------------------------------//
void DLS::Clear()
{
	FN("DLS::Clear()");
	m_bInitialized = false;
	m_pCollection = NULL;
	m_Init.Clear();
	m_iRefCount = 0;
}



//------------------------------------------------------------------------//
bool DLS::Init(const DLSInit& init)
{
	FN("DLS::Init()");
	// Make sure the objects have been created first
	assert(DXAudioMgr()->Performance(true));

	// Set the audio definition
	m_Init = init;
	m_bInitialized = true;
	return true;
}


//------------------------------------------------------------------------//
void DLS::Term()
{
	FN("DLS::Term()");
	Unload();
	Clear();
}


//------------------------------------------------------------------------//
bool DLS::Load()
{
	FN("DLS::Load()");
	if(IsLoaded())
		return true;


	IAudioStream* pStream;
	CreateAudioStream cas(pStream);
	if(!pStream)
		return false;
	
	HRESULT hr = pStream->Open(m_Init.m_sFileName);
	if(FAILED(hr))
		return Error::Handle("Failed to open segment file %s.", m_Init.m_sFileName.c_str());
	
	DMUS_OBJECTDESC      ObjDesc;
	ObjDesc.dwSize = sizeof(DMUS_OBJECTDESC);
	ObjDesc.guidClass = CLSID_DirectMusicCollection;
	ObjDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_STREAM;
	ObjDesc.pStream = pStream;

    hr = DXAudioMgr()->Loader()->GetObject(
            &ObjDesc, 
			IID_IDirectMusicCollection8, 
            (void**) &m_pCollection );
	if(FAILED(hr))
	{
		Unload();
		return Error::Handle("Could not load DLS file %s", m_Init.m_sFileName.c_str());
	}

	return true;
}


//------------------------------------------------------------------------//
bool DLS::Unload()
{
	FN("DLS::Unload()");
	if(!IsLoaded())
		return true;

	if(m_pCollection)
	{
		DXAudioMgr()->Loader()->ReleaseObjectByUnknown(m_pCollection);
	}
	_XASAFE_RELEASE(m_pCollection);
	return true;
}



//------------------------------------------------------------------------//
bool DLS::Lock()
{
	FN("DLS::Lock()");
	if(!IsLoaded() && !Load())
		return false;
	m_iRefCount++;
	return true;
}


//------------------------------------------------------------------------//
bool DLS::Unlock()
{
	FN("DLS::Unlock()");
	if(IsLoaded() && !Unload())
		return false;
	m_iRefCount--;
	return true;
}


//------------------------------------------------------------------------//
void DLS::Destroy()
{
	FN("DLS::Destroy()");
	Term();
	DestroyObject(this);
}



