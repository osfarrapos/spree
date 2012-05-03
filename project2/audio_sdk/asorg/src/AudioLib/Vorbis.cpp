/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "Vorbis.h"

#ifdef USE_VORBIS
using namespace std;
using namespace Audio;

// Memory callback function declarations
size_t read_func_mem(void *ptr, size_t size, size_t nmemb, void *datasource);
int    seek_func_mem(void *datasource, ogg_int64_t offset, int whence);
int    close_func_mem(void *datasource);
long   tell_func_mem(void *datasource);

// Stream callback function declarations
size_t read_func_stream(void *ptr, size_t size, size_t nmemb, void *datasource);
int    seek_func_stream(void *datasource, ogg_int64_t offset, int whence);
int    close_func_stream(void *datasource);
long   tell_func_stream(void *datasource);


IMPLEMENT_POOL(Vorbis);


//------------------------------------------------------------------------//
Vorbis::Vorbis()
{
	FN("Vorbis::Vorbis()");
	Clear();
}

//------------------------------------------------------------------------//
Vorbis::~Vorbis()
{
	Close();
	FN("Vorbis::~Vorbis()");
}

//------------------------------------------------------------------------//
void Vorbis::Clear()
{
	FN("Vorbis::Clear()");
	m_pBufferPtr = 0;
	m_pBuffer = 0;
	m_nBufferSize = 0;
	m_nNumSamples = 0;
	m_pVorbisInfo = 0;
	memset(&m_WaveFormatEx, 0, sizeof(WAVEFORMATEX));
	memset(&m_VorbisFile, 0, sizeof(OggVorbis_File));
	m_bEOF = false;
	m_MemSrc.Clear();
	m_pStreamSrc = 0;
	m_bOpen = false;
}


//------------------------------------------------------------------------//
// Use for IAudioStream-based source
bool Vorbis::Open( std::string sFileName)
{
	FN("Vorbis::Open()");
	int iStartTime = timeGetTime();

	if(m_bOpen)
		return false;

	// Open the file
	if(!AudioMgr()->CreateAudioStream(m_pStreamSrc))
		return false;
	
	if(FAILED(m_pStreamSrc->Open(sFileName)))
		return false;

	// Set the proper callback functions
	ov_callbacks cb;
	cb.read_func = &read_func_stream;
	cb.seek_func = &seek_func_stream;
	cb.close_func = &close_func_stream;
	cb.tell_func = &tell_func_stream;

	// Open the ogg bitstream
	if(ov_open_callbacks(m_pStreamSrc, &m_VorbisFile, NULL, 0, cb) < 0) 
		return false;

	m_bOpen = true;
	if(!GetStreamInfo())
		return false;

	DebugOut(1, "Open() time: %dms", timeGetTime() - iStartTime);


	return true;
}

//------------------------------------------------------------------------//
// Used for loading direct from memory
bool Vorbis::Open( BYTE* pbData, uint32 dwDataSize)
{
	FN("Vorbis::Open()");

	if(m_bOpen)
		return false;

	m_MemSrc.m_pData = (char*)pbData;
	m_MemSrc.m_nDataSize = dwDataSize;
	m_MemSrc.m_pReadPtr = (char*)pbData;

	// Set the proper callback functions
	ov_callbacks cb;
	cb.read_func = &read_func_mem;
	cb.seek_func = &seek_func_mem;
	cb.close_func = &close_func_mem;
	cb.tell_func = &tell_func_mem;

	if(ov_open_callbacks(&m_MemSrc, &m_VorbisFile, NULL, dwDataSize, cb) < 0)
		return false;

	if(!GetStreamInfo())
		return false;

	m_bOpen = true;
	return true;
}

//------------------------------------------------------------------------//
bool Vorbis::GetStreamInfo()
{
	// Display comment information when loading the file
	char **ptr=ov_comment(&m_VorbisFile,-1)->user_comments;
	while(*ptr)
	{
	  DebugOut(4, "%s",*ptr);
	  ++ptr;
	}

	// Get vorbis file information
	m_pVorbisInfo = ov_info(&m_VorbisFile,-1);
	DebugOut(4, "Bitstream is %d channel, %ldHz", m_pVorbisInfo->channels,m_pVorbisInfo->rate);

	// Get the number of PCM samples in this file
	m_nNumSamples = (long)ov_pcm_total(&m_VorbisFile,-1);
	DebugOut(4, "Decoded length: %ld samples", m_nNumSamples);

	// Display other info
	DebugOut(4, "Encoded by: %s", ov_comment(&m_VorbisFile,-1)->vendor);

	// set up the WaveFormatEx structure
	m_WaveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
	m_WaveFormatEx.nChannels = m_pVorbisInfo->channels;
	m_WaveFormatEx.nSamplesPerSec = m_pVorbisInfo->rate;
	m_WaveFormatEx.wBitsPerSample = 16;
	m_WaveFormatEx.nBlockAlign = m_WaveFormatEx.nChannels * m_WaveFormatEx.wBitsPerSample / 8;
	m_WaveFormatEx.nAvgBytesPerSec = m_WaveFormatEx.nSamplesPerSec * m_WaveFormatEx.nBlockAlign;
	m_WaveFormatEx.cbSize = 0;

	return true;
}

//------------------------------------------------------------------------//
bool Vorbis::Close()
{
	FN("Vorbis::Close()");

	if(!m_bOpen)
		return false;

	// close out the vorbis file bitstream
	ov_clear(&m_VorbisFile);

	Clear();

	return true;
}


//------------------------------------------------------------------------//
bool Vorbis::Read( BYTE* pBuffer, uint32 dwSizeToRead, uint32* pdwSizeRead )
{
	FN("Vorbis::Read()");

	int iStartTime = timeGetTime();

	if(!m_bOpen)
		return false;

	char* pCurBuffer = (char*)pBuffer;
	uint32 nBytesRead = 0;
	int iSection = 0;
	while((nBytesRead < dwSizeToRead) && !m_bEOF)
	{
		int32 iRet = ov_read(&m_VorbisFile, pCurBuffer, dwSizeToRead - nBytesRead, 0, 2, 1, &iSection);
		if (iRet == 0 || iSection != 0) 
		{
			m_bEOF = true;
		} 
		else if (iRet < 0) 
		{
			return Error::Handle("Error in vorbis data stream!");;
		}
		nBytesRead += iRet;
		pCurBuffer += iRet;
	}

	*pdwSizeRead = nBytesRead;
	DebugOut(5, "Read() time: %dms", timeGetTime() - iStartTime);
	return true;
}

//------------------------------------------------------------------------//
uint32 Vorbis::GetSize()
{
	FN("Vorbis::GetSize()");

	return m_nNumSamples * m_WaveFormatEx.nChannels * m_WaveFormatEx.wBitsPerSample / 8;
}

//------------------------------------------------------------------------//
bool Vorbis::Reset()
{
	FN("Vorbis::Reset()");

	if(!m_bOpen)
		return false;

	// Seek to beginning of file to begin reading it again
	m_bEOF = false;
	ov_pcm_seek(&m_VorbisFile, 0);

	return true;
}


//------------------------------------------------------------------------//
void Vorbis::Destroy()
{
	Close();
	Vorbis::DestroyObject(this);
}


//------------------------------------------------------------------------//
WAVEFORMATEX* Vorbis::GetFormat()
{
	FN("Vorbis::GetFormat()");
	return &m_WaveFormatEx;
}

//------------------------------------------------------------------------//
bool Vorbis::IsEOF()
{
	FN("Vorbis::IsEOF()");
	return m_bEOF;
}


//-----------------------------------------------------------------
// Vorbis file library callback functions for reading out of memory
//-----------------------------------------------------------------
size_t read_func_mem(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	MemorySource* pSrc = (MemorySource*)datasource;
	uint32 nBytesToCopy = size * nmemb;
	uint32 nBytesRemaining = (pSrc->m_pData + pSrc->m_nDataSize) - pSrc->m_pReadPtr;
	if(nBytesToCopy > nBytesRemaining)
		nBytesToCopy = nBytesRemaining;
	memcpy(ptr, pSrc->m_pReadPtr, nBytesToCopy);
	pSrc->m_pReadPtr += nBytesToCopy;
	return nBytesToCopy;
}

//------------------------------------------------------------------------//
int seek_func_mem(void *datasource, ogg_int64_t offset, int whence)
{
	MemorySource* pSrc = (MemorySource*)datasource;
	switch(whence)
	{
		case SEEK_SET:
			pSrc->m_pReadPtr = pSrc->m_pData + offset;
			break;
		case SEEK_CUR:
			pSrc->m_pReadPtr += offset;
			break;
		case SEEK_END:
			pSrc->m_pReadPtr = (pSrc->m_pData + pSrc->m_nDataSize) - offset;
			break;
	};

	// check for out of bounds seek pointer
	if(pSrc->m_pReadPtr < pSrc->m_pData)
	{
		pSrc->m_pReadPtr = pSrc->m_pData;
		return -1;
	}
	else if(pSrc->m_pReadPtr > (pSrc->m_pData + pSrc->m_nDataSize))
	{
		pSrc->m_pReadPtr = pSrc->m_pData + pSrc->m_nDataSize;
		return -1;
	}

	return 0;
}

//------------------------------------------------------------------------//
int close_func_mem(void *datasource)
{
	MemorySource* pSrc = (MemorySource*)datasource;
	return 0;
}

//------------------------------------------------------------------------//
long tell_func_mem(void *datasource)
{
	MemorySource* pSrc = (MemorySource*)datasource;
	return pSrc->m_pReadPtr - pSrc->m_pData;
}


//-----------------------------------------------------------------
// Vorbis file library callback functions for reading from a file
//-----------------------------------------------------------------
size_t read_func_stream(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	if(!size || !nmemb)
		return 0;

	IAudioStream* pSrc = (IAudioStream*)datasource;
	uint32 nBytesToRead = size * nmemb;

	uint32 nRead;
	if(FAILED(pSrc->Read(ptr, nBytesToRead, &nRead)))
		return 0;

	return nRead;
}

//------------------------------------------------------------------------//
int seek_func_stream(void *datasource, ogg_int64_t offset, int whence)
{
	IAudioStream* pSrc = (IAudioStream*)datasource;
	LARGE_INTEGER liOffset;
	ULARGE_INTEGER lnNewPos;
	liOffset.QuadPart = offset;
	if(FAILED(pSrc->Seek(liOffset, whence, &lnNewPos)))
		return -1;
	return 0;
}

//------------------------------------------------------------------------//
int close_func_stream(void *datasource)
{
	IAudioStream* pSrc = (IAudioStream*)datasource;
	_XASAFE_RELEASE(pSrc);
	return 0;
}

//------------------------------------------------------------------------//
long tell_func_stream(void *datasource)
{
	IAudioStream* pSrc = (IAudioStream*)datasource;
	LARGE_INTEGER dlibMove;
	dlibMove.QuadPart = 0;
	ULARGE_INTEGER nNewPos;
	pSrc->Seek(dlibMove, SEEK_CUR, &nNewPos);
	return nNewPos.QuadPart;
}

#endif // USE_VORBIS

