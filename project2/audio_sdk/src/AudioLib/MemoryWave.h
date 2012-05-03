/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#pragma once

#ifndef _MEMORYWAVE_H_
#define _MEMORYWAVE_H_

#include "Audio.h"
#include "IAudioLoader.h"

namespace Audio
{

class MemoryWave : public IAudioLoader
{
DEFINE_POOL(MemoryWave);

public:
	bool			Open(std::string sFileName);
	bool			Open(BYTE* pbData, uint32 dwDataSize);
	bool			Close();

	bool			Read( BYTE* pDestBuffer, uint32 dwSizeToRead, uint32* pdwSizeRead );

	bool			Reset();
	WAVEFORMATEX*	GetFormat()			{  return &m_DestFormat;  }
	uint32			GetSize()			{  return m_dwSize;  }
	bool			IsEOF()				{  return m_bEOF;  }

	void			Destroy();
private:
		
protected:
	virtual ~MemoryWave();
private:
	MemoryWave();

	WAVEFORMATEX	m_DestFormat;// Pointer to WAVEFORMATEX structure
	HMMIO			m_hmmio;	 // MM I/O handle for the WAVE
	MMCKINFO		m_ck;		 // Multimedia RIFF chunk
	MMCKINFO		m_ckRiff;	 // Use in opening a WAVE file
	uint32			m_dwSize;	 // The size of the wave file
	bool			m_bEOF;		 // Has reached end of file?z

	BYTE*			m_pbData;
    BYTE*			m_pbDataCur;
    ULONG			m_ulDataSize;
};

}; // namespace Audio


#endif // _MEMORYWAVE_H_
