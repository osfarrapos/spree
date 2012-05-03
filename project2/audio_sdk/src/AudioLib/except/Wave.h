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

#ifndef _WAVE_H_
#define _WAVE_H_

#include "Audio.h"
#include "IAudioLoader.h"
#include "XAudioPackage.h"

using namespace std;

namespace Audio
{

class Wave : public IAudioLoader
{
DEFINE_POOL(Wave);

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
	bool OpenMMIO();
    bool ReadMMIO();
	bool PrepareACMBuffers(uint32& nDataIn, uint32& nStreamRead, uint32 dwSizeToRead);
	bool DecompressData(uint32 nDataIn, BYTE* pDestBuffer, 
		uint32 dwSizeToRead, DWORD* pdwSizeRead);

protected:
	virtual ~Wave();
private:
	Wave();

	WAVEFORMATEX	m_DestFormat;// Pointer to WAVEFORMATEX structure
	HMMIO			m_hmmio;	 // MM I/O handle for the WAVE
	MMCKINFO		m_ck;		 // Multimedia RIFF chunk
	MMCKINFO		m_ckRiff;	 // Use in opening a WAVE file
	uint32			m_dwSize;	 // The size of the wave file
	bool			m_bEOF;		 // Has reached end of file?

	// ACM specific structures
	WAVEFORMATEX*	m_pwfx;		
	HACMSTREAM		m_hACMStream;
	uint8*			m_pCompressBuffer;
	uint32			m_nCompressBufferSize;
	uint8*			m_pDecompressBuffer;
	uint32			m_nDecompressStart;
	uint32			m_nDecompressEnd;
	uint32			m_nDecompressBufferSize;

	static uint32	s_nWaveCount;

public:				
	_XAudioPackageArchive* m_pPackage;
	string				   m_sFileName;

	BYTE*				   m_pResourceBuffer;
	BYTE*				   m_pbData;
    BYTE*				   m_pbDataCur;
	ULONG				   m_ulDataSize;
};

}; // namespace Audio


#endif // _WAVE_H_
