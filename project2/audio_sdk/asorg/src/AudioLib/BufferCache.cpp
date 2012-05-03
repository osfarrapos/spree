/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "BufferCache.h"
#include "AudioMgr.h"

using namespace Audio;


bool operator == (const WAVEFORMATEX& left, const WAVEFORMATEX& right)
{
	if((left.nAvgBytesPerSec == right.nAvgBytesPerSec) &&
		(left.nBlockAlign == right.nBlockAlign) &&
		(left.nChannels == right.nChannels) &&
		(left.nSamplesPerSec == right.nSamplesPerSec) &&
		(left.wBitsPerSample == right.wBitsPerSample) &&
		(left.wFormatTag == right.wFormatTag) &&
		(left.cbSize == right.cbSize))
		return true;
	return false;
}

bool operator == (const DSBUFFERDESC& left, const DSBUFFERDESC& right)
{
	if((left.dwBufferBytes == right.dwBufferBytes) &&
		(left.dwFlags == right.dwFlags) &&
		(*left.lpwfxFormat == *right.lpwfxFormat))
		return true;
	return false;
}

BufferCache::BufferCache()
{
}

BufferCache::~BufferCache()
{
}

void BufferCache::Clear()
{
	m_Free.clear();
	m_Used.clear();
}


void BufferCache::Init()
{
	m_Free.reserve(128);
	m_Used.reserve(128);
}

void BufferCache::Term()
{
	BufferInfoVector::iterator itr;
	for(itr = m_Free.begin(); itr != m_Free.end(); ++itr)
	{
		(*itr)->m_pBuffer->Release();
		delete (*itr);
	}
	for(itr = m_Used.begin(); itr != m_Used.end(); ++itr)
	{
		(*itr)->m_pBuffer->Release();
		delete (*itr);
	}
}


bool BufferCache::Acquire(const DSBUFFERDESC& desc, IDirectSoundBuffer8*& pBuffer, bool bUseCache)
{

	// If buffer caching is enabled, try to find a 
	// buffer with a matching description structure
	if(DXAudioMgr()->GetInit()->m_bCacheBuffers && bUseCache)
	{
		BufferInfoVector::iterator itr;
		for(itr = m_Free.begin(); itr != m_Free.end(); ++itr)
		{
			if((*itr)->m_Desc == desc)
			{
				pBuffer = (*itr)->m_pBuffer;
				pBuffer->SetCurrentPosition(0);
				pBuffer->AddRef();
				m_Used.push_back(*itr);
				m_Free.erase(itr);
				return true;
			}
		}
	}

	// Create a buffer normally
	IDirectSoundBuffer* pDSBuffer;
	HRESULT hr = DXAudioMgr()->DirectSound()->CreateSoundBuffer(&desc, &pDSBuffer, NULL);
	if(FAILED(hr))
	{
		DebugOut(3, "First attempt at sound buffer creation failed.  Error = %s.  Attempting again...", DXGetErrorString(hr));
		if(desc.dwFlags & DSBCAPS_CTRL3D)
		{
			DXAudioMgr()->ResetSound3DLimit();
			if(!DXAudioMgr()->RemoveSound3D(0))
				return Error::Handle("Could not create sound buffer.  Error = %s", DXGetErrorString(hr));
		}
		else
		{
			DXAudioMgr()->ResetSoundLimit();
			if(!DXAudioMgr()->RemoveSound(0))
				return Error::Handle("Could not create sound buffer.  Error = %s", DXGetErrorString(hr));
		}

		hr = DXAudioMgr()->DirectSound()->CreateSoundBuffer(&desc, &pDSBuffer, NULL);
		if(FAILED(hr))
			return Error::Handle("Could not create sound buffer.  Error = %s", DXGetErrorString(hr));
	}

	// Get the IDirectSoundBuffer8 interface
	hr = pDSBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&pBuffer);
	if(FAILED(hr))
		return Error::Handle("Could not obtain DirectSoundBuffer8 interface.  Error = %s", DXGetErrorString(hr));

	// Release the temporary DirectSoundBuffer interface
	pDSBuffer->Release();

	// If buffer caching is enabled,
	if(DXAudioMgr()->GetInit()->m_bCacheBuffers && bUseCache)
	{
		BufferInfo* pInfo = new BufferInfo;
		pInfo->m_pBuffer = pBuffer;
		pInfo->m_pBuffer->AddRef();
		memcpy(&pInfo->m_Format, desc.lpwfxFormat, sizeof(WAVEFORMATEX));
		memcpy(&pInfo->m_Desc, &desc, sizeof(DSBUFFERDESC));
		pInfo->m_Desc.lpwfxFormat = &pInfo->m_Format;
		m_Used.push_back(pInfo);
	}

	return true;
}

void BufferCache::Free(IDirectSoundBuffer8* pBuffer)
{
	if(pBuffer && DXAudioMgr()->GetInit()->m_bCacheBuffers)
	{
		BufferInfoVector::iterator itr;
		for(itr = m_Used.begin(); itr != m_Used.end();)
		{
			BufferInfo* pInfo = (*itr);
			if(pInfo->m_pBuffer == pBuffer)
			{
				m_Free.push_back(*itr);
				itr = m_Used.erase(itr);
				return;
			}
			else
				++itr;
		}
	}
}





