/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#include "AudioScript.h"
#include "AudioCommon.h"
#include "AudioMgr.h"

using namespace std;
using namespace Audio;

IMPLEMENT_POOL(AudioScript);


//------------------------------------------------------------------------//
AudioScript::AudioScript()
{
	Clear();
}

//------------------------------------------------------------------------//
AudioScript::~AudioScript()
{
	Term();
}

//------------------------------------------------------------------------//
void AudioScript::Clear()
{
	m_bInitialized = false;
	m_Init.Clear();
	m_pScript = 0;
	m_bQueuePlayback = false;
	m_bLoading = false;
	m_bLoaded = false;
}


//------------------------------------------------------------------------//
void AudioScript::Term()
{
	Unload();
	Clear();
}


//------------------------------------------------------------------------//
bool AudioScript::Init(const AudioScriptInit& init)
{
	m_Init = init;
	m_bInitialized = true;
	return true;
}


//------------------------------------------------------------------------//
void AudioScript::Destroy()
{
	Term();
	AudioScript::DestroyObject(this);
}


//------------------------------------------------------------------------//
bool AudioScript::Load()
{
	IAudioStream* pStream;
	CreateAudioStream cas(pStream);
	if(!pStream)
		return false;
	
	HRESULT hr = pStream->Open(m_Init.m_sFileName);
	if(FAILED(hr))
		return Error::Handle("Failed to open segment file %s.", m_Init.m_sFileName.c_str());
	
	DMUS_OBJECTDESC      ObjDesc;
	ObjDesc.dwSize = sizeof(DMUS_OBJECTDESC);
	ObjDesc.guidClass = CLSID_DirectMusicScript;
	ObjDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_STREAM;
	ObjDesc.pStream = pStream;

	// Load the data and retrieve the segment interface
	hr = DXAudioMgr()->Loader()->GetObject(
			&ObjDesc, 
			IID_IDirectMusicScript8, 
			(void**) &m_pScript );
	if(FAILED(hr))
	{	
		Unload();
		return Error::Handle("Could not load audio script %s. Error = %s.", 
			m_Init.m_sFileName.c_str(), DXGetErrorString(hr));
	}

	hr = m_pScript->Init(DXAudioMgr()->Performance(m_Init.m_bMusic), NULL);
	if(FAILED(hr))
	{
		Unload();
		return Error::Handle("Could not initialize audio script %s.  Error = %s",
			m_Init.m_sFileName.c_str(), DXGetErrorString(hr));
	}

	return true;
}

//------------------------------------------------------------------------//
bool AudioScript::Unload()
{
	if(m_pScript)
	{
		// Since we're loading and unloading segments dynamically, it is 
		// important to release the loader's internal reference to it.
		HRESULT hr = DXAudioMgr()->Loader()->ReleaseObjectByUnknown(m_pScript);
		if(FAILED(hr))
		{  Error::Handle("Error unloading sound segment from loader.");  }

		// Instruct the loader to clear out unused memory.
		DXAudioMgr()->Loader()->CollectGarbage();

		// Now release the actual segment
		_XASAFE_RELEASE(m_pScript);
	}
	return true;
}



//------------------------------------------------------------------------//
bool AudioScript::SetVariable(string sVarName, int32 iVal)
{
	if(!m_pScript)
		return false;

	WCHAR wsBuffer[_MAX_PATH];
	ConvertAnsiStringToWide(wsBuffer, sVarName.c_str());
	HRESULT hr = m_pScript->SetVariableNumber(wsBuffer, iVal, NULL);
	if(FAILED(hr))
		return false;

	return true;
}


//------------------------------------------------------------------------//
bool AudioScript::GetVariable(string sVarName, int32& iVal)
{
	if(!m_pScript)
		return false;

	WCHAR wsBuffer[_MAX_PATH];
	ConvertAnsiStringToWide(wsBuffer, sVarName.c_str());
	HRESULT hr = m_pScript->GetVariableNumber(wsBuffer, &iVal, NULL);
	if(FAILED(hr))
		return false;

	return true;
}



//------------------------------------------------------------------------//
bool AudioScript::CallRoutine(string sRoutineName)
{
	if(!m_pScript)
		return false;

	WCHAR wsBuffer[_MAX_PATH];
	ConvertAnsiStringToWide(wsBuffer, sRoutineName.c_str());
	HRESULT hr = m_pScript->CallRoutine(wsBuffer, NULL);
	if(FAILED(hr))
		return false;

	return true;
}


