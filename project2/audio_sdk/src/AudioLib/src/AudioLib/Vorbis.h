/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __IN_VORBIS_H__
#define __IN_VORBIS_H__

#ifdef USE_VORBIS

#include "Audio.h"
#include "IAudioLoader.h"

#pragma pack(push,8)
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

namespace Audio
{

struct MemorySource
{
	MemorySource()
	{
		Clear();
	};
	void Clear()
	{
		m_pData = 0;
		m_nDataSize = 0;
		m_pReadPtr = 0;
	};
	char*	m_pData;
	uint32	m_nDataSize;
	char*	m_pReadPtr;
};


class Vorbis : public IAudioLoader
{
DEFINE_POOL(Vorbis);

public:
	void Clear();

	// This Open function is used if the source is to be streamed from the disk
    bool Open(std::string  sFileName);
	bool Open(BYTE* pbData, uint32 dwDataSize);
    bool Close();

    bool Read( BYTE* pBuffer, uint32 dwSizeToRead, uint32* pdwSizeRead );

    uint32			GetSize();
    bool			Reset();
    WAVEFORMATEX*	GetFormat();
	bool			IsEOF();
	
	void			Destroy();

protected:
	virtual ~Vorbis();
private:
	Vorbis();

	bool GetStreamInfo();

private:
	WAVEFORMATEX	m_WaveFormatEx;

	bool			m_bOpen;
	OggVorbis_File	m_VorbisFile;
	vorbis_info*	m_pVorbisInfo;

	uint8*			m_pBufferPtr;
	uint8*			m_pBuffer;
	uint32			m_nBufferSize;
	uint32			m_nNumSamples;

	bool			m_bEOF;

public:
	// This has to be public so the callback functions can access it
	MemorySource	m_MemSrc;
	IAudioStream*	m_pStreamSrc;
};

}; // namespace Audio

#pragma pack(pop,8)

#endif // USE_VORBIS


#endif // __IN_VORBIS_H__