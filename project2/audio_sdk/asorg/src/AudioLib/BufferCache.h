/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef BUFFER_CACHE_H__
#define BUFFER_CACHE_H__

#include "Audio.h"
#include "AudioCommon.h"

namespace Audio
{

struct BufferInfo
{
	IDirectSoundBuffer8*	m_pBuffer;
	DSBUFFERDESC			m_Desc;
	WAVEFORMATEX			m_Format;
};

typedef std::vector<BufferInfo*> BufferInfoVector;

class BufferCache
{
public:
	BufferCache();
	~BufferCache();
	void Clear();

	void Init();
	void Term();

	bool Acquire(const DSBUFFERDESC& desc, IDirectSoundBuffer8*& pBuffer, bool bUseCache = true);
	void Free(IDirectSoundBuffer8* pBuffer);

private:

	BufferInfoVector	m_Master;
	BufferInfoVector	m_Free;
	BufferInfoVector	m_Used;
};


}; // namespace Audio

#endif // BUFFER_CACHE_H__