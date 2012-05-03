/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "Audio.h"
#include "Wave.h"

using namespace std;
using namespace Audio;

IMPLEMENT_POOL(Wave);



LRESULT CALLBACK CustomMMIOProc(
  LPSTR lpmmioinfo,  
  UINT uMsg,         
  LONG lParam1,      
  LONG lParam2       
)
{

	char cBuffer[MAX_PATH];

	LRESULT ret = 0;

	MMIOINFO* pMMIOINFO = (MMIOINFO*)lpmmioinfo;
	IAudioStream* pStream = (IAudioStream*)pMMIOINFO->adwInfo[0];

	switch(uMsg)
	{
	case MMIOM_OPEN:
		{
			strcpy(cBuffer, (char*)lParam1);
			cBuffer[strlen(cBuffer) - 1] = 0;
			if(!AudioMgr()->CreateAudioStream(pStream))
				return MMIOERR_CANNOTOPEN;
			if(FAILED(pStream->Open(cBuffer)))
				return MMIOERR_CANNOTOPEN;
			pMMIOINFO->adwInfo[0] = (DWORD)pStream;
		}
		break;
	case MMIOM_SEEK:
		{
			if(!pStream)
				return 0;
			ULARGE_INTEGER lnNewPos;
			LARGE_INTEGER liSeek;
			liSeek.QuadPart = lParam1;
			pStream->Seek(liSeek, lParam2, &lnNewPos);
			pMMIOINFO->lDiskOffset = (DWORD)lnNewPos.QuadPart;
		}
		break;
	case MMIOM_READ:
		{
			if(!pStream)
				return 0;
			uint32 nRead;
			pStream->Read((void*)lParam1, lParam2, &nRead);
			pMMIOINFO->lDiskOffset += nRead;
			return nRead;
		}
	case MMIOM_CLOSE:
		{
			pMMIOINFO->lDiskOffset = 0;
			pMMIOINFO->adwInfo[0] = 0;
			_XASAFE_RELEASE(pStream);
		}
		break;
	};

	return ret;
}



uint32 Wave::s_nWaveCount = 0;

// Constant value to overestimate decompression buffer size
const uint32 ACM_BUFFER_OVERESTIMATE = 2048;

//------------------------------------------------------------------------//
// Constructs the class.  Call Open() to open a wave file for reading.  
// Then call Read() as needed.  Calling the destructor or Close() 
// will close the file.  
Wave::Wave()
{
	memset(&m_ck, 0, sizeof(MMCKINFO));
	memset(&m_ckRiff, 0, sizeof(MMCKINFO));
	m_pwfx    = NULL;
	m_hmmio   = NULL;
	m_dwSize  = 0;
	m_bEOF = false;
	memset(&m_DestFormat, 0, sizeof(WAVEFORMATEX));
	m_hACMStream = 0;
	m_pCompressBuffer = 0;
	m_nCompressBufferSize = 0;
	m_pDecompressBuffer = 0;
	m_nDecompressStart = 0;
	m_nDecompressEnd = 0;
	m_nDecompressBufferSize = 0;
}

//------------------------------------------------------------------------//
// Destructs the class
Wave::~Wave()
{
	if(!s_nWaveCount)
		mmioInstallIOProc(mmioFOURCC('W', 'A', 'V', ' '), 0, MMIO_REMOVEPROC);
	Close();
	_XASAFE_DELETE_ARRAY(m_pCompressBuffer);
	m_nCompressBufferSize = 0;
	_XASAFE_DELETE_ARRAY(m_pDecompressBuffer);
	m_nDecompressBufferSize = 0;
}


//------------------------------------------------------------------------//
// Opens a wave file for reading from an IAudioStream system
bool Wave::Open(string sFileName)
{
	if(!s_nWaveCount)
	{
		if(!mmioInstallIOProc(mmioFOURCC('W', 'A', 'V', ' '), &CustomMMIOProc, MMIO_INSTALLPROC))
			return false;
	}
	sFileName += "+";
	// Open the wave file using the installed io procedure function
    m_hmmio = mmioOpen((char*)sFileName.c_str(), NULL, MMIO_ALLOCBUF | MMIO_READ);

	s_nWaveCount++;

    return OpenMMIO();
}

//------------------------------------------------------------------------//
// Open a wave file format from a memory buffer
bool Wave::Open( BYTE* pbData, uint32 dwDataSize)
{
	if(!s_nWaveCount)
	{
		if(!mmioInstallIOProc(mmioFOURCC('W', 'A', 'V', ' '), &CustomMMIOProc, MMIO_INSTALLPROC))
			return false;
	}
	// Indicate to read from a memory buffer
	MMIOINFO mmioInfo;
	ZeroMemory( &mmioInfo, sizeof(mmioInfo) );
	mmioInfo.fccIOProc = FOURCC_MEM;
	mmioInfo.cchBuffer = dwDataSize;
	mmioInfo.pchBuffer = (CHAR*) pbData;

	// Open the memory buffer
	m_hmmio = mmioOpen(NULL, &mmioInfo, MMIO_READ);

	s_nWaveCount++;
	return OpenMMIO();
}

//------------------------------------------------------------------------//
bool Wave::OpenMMIO()
{
	// Read the wave header file
	if(!ReadMMIO())
	{
		// ReadMMIO will fail if its an not a wave file
		mmioClose(m_hmmio, 0);
		return Error::Handle("Wave Error: ReadMMIO");
	}

	// Reset the file to prepare for reading
	if(!Reset())
	{
		mmioClose(m_hmmio, 0);
		return Error::Handle("Wave Error: Reset");
	}

	// Set (or estimate) the size of the decompressed wave data
	if(m_hACMStream)
	{
		MMRESULT error = acmStreamSize(m_hACMStream, m_ck.cksize, &m_dwSize, 
			ACM_STREAMSIZEF_SOURCE);
		if(error)
			return Error::Handle("Could not estimate size for non-PCM data conversion");
	}
	else
	{
		m_dwSize = m_ck.cksize;
	}
	return true;
}


//------------------------------------------------------------------------//
// Verifies that this is a wave file, and allocates and 
// fills out the wave format header structure.
bool Wave::ReadMMIO()
{
	// chunk info. for general use.
	MMCKINFO        ckIn;   
	// Temp PCM structure to load in.
	PCMWAVEFORMAT   pcmWaveFormat;         

	// Make sure this structure has been deallocated
    _XASAFE_DELETE_ARRAY(m_pwfx);

	if(mmioSeek( m_hmmio, 0, SEEK_SET ) == -1)
		return false;

	if( ( 0 != mmioDescend( m_hmmio, &m_ckRiff, NULL, 0 ) ) )
		return Error::Handle("Wave Error: mmioDescend");

	// Check to make sure this is a valid wave file
	if( (m_ckRiff.ckid != FOURCC_RIFF) ||
		(m_ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E') ) )
		return Error::Handle("Wave Error: mmioFOURCC"); 

	// Search the input file for for the 'fmt ' chunk.
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if( 0 != mmioDescend( m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK ) )
		return Error::Handle("Wave Error: mmioDescend");

	// Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
	// if there are extra parameters at the end, we'll ignore them
	if( ckIn.cksize < (LONG) sizeof(PCMWAVEFORMAT) )
		return Error::Handle("Wave Error: sizeof(PCMWAVEFORMAT)");

	// Read the 'fmt ' chunk into <pcmWaveFormat>.
	if( mmioRead( m_hmmio, (HPSTR) &pcmWaveFormat, 
		sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat) )
		return Error::Handle("Wave Error: mmioRead");

	// Allocate the waveformatex, but if its not pcm format, read the next
	// word, and thats how many extra bytes to allocate.
	if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
	{
		memcpy(&m_DestFormat, &pcmWaveFormat, sizeof(pcmWaveFormat) );
		m_DestFormat.cbSize = 0;
	}
	else
	{
		// Read in length of extra bytes.
		WORD cbExtraBytes = 0L;
		if( mmioRead( m_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD) )
			return Error::Handle("Wave Error: mmioRead");

		m_pwfx = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) + cbExtraBytes ];
		if( NULL == m_pwfx )
			return Error::Handle("Wave Error: new");

		// Copy the bytes from the pcm structure to the waveformatex structure
		memcpy( m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat) );
		m_pwfx->cbSize = cbExtraBytes;

		// Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
		if( mmioRead( m_hmmio, (CHAR*)(((BYTE*)&(m_pwfx->cbSize))+sizeof(WORD)),
			cbExtraBytes ) != cbExtraBytes )
		{
			_XASAFE_DELETE( m_pwfx );
			return Error::Handle("Wave Error: mmioRead");
		}

		// Since this is a non PCM format, we must retrieve an appropriate
		// PCM format to convert to.
		ZeroMemory(&m_DestFormat, sizeof(WAVEFORMATEX));
		m_DestFormat.wFormatTag = WAVE_FORMAT_PCM;   
		
		MMRESULT error = acmFormatSuggest(NULL, m_pwfx, &m_DestFormat, 16, 
			ACM_FORMATSUGGESTF_WFORMATTAG);
		if(error)
			return Error::Handle("Could not suggest PCM format to convert to");

		error = acmStreamOpen(&m_hACMStream, NULL, m_pwfx, &m_DestFormat,
			NULL, 0, 0, 0);
		if(error)
			return Error::Handle("Could not open format conversion stream");

	}

	// Ascend the input file out of the 'fmt ' chunk.
	if( 0 != mmioAscend( m_hmmio, &ckIn, 0 ) )
	{
		_XASAFE_DELETE( m_pwfx );
		return Error::Handle("Wave Error: mmioAscend");
	}

	return true;
}


//------------------------------------------------------------------------//
// Resets the internal m_ck pointer so reading starts from the 
// beginning of the file again 
bool Wave::Reset()
{
	m_bEOF = false;

	if( m_hmmio == NULL )
		return false;

	// Seek to the data
	if( -1 == mmioSeek( m_hmmio, m_ckRiff.dwDataOffset + sizeof(FOURCC),
		SEEK_SET ) )
		return Error::Handle("Wave Error: mmioSeek");

	// Search the input file for the 'data' chunk.
	m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if( 0 != mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
		return Error::Handle("Wave Error: mmioDescend");
    
	// Reset the pointers in the decompression buffer
	m_nDecompressStart = 0;
	m_nDecompressEnd = 0;

	return true;
}


//------------------------------------------------------------------------//
// Reads section of data from a wave file into pBuffer and returns 
// how much read in pdwSizeRead, reading not more than dwSizeToRead.
// This uses m_ck to determine where to start reading from.  So 
// subsequent calls will be continue where the last left off unless 
// Reset() is called.
bool Wave::Read( BYTE* pDestBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )
{
	MMIOINFO mmioinfoIn; // current status of m_hmmio

	if( m_hmmio == NULL )
		return false;
	if( pDestBuffer == NULL || pdwSizeRead == NULL )
		return false;

	if( pdwSizeRead != NULL )
		*pdwSizeRead = 0;

	if( 0 != mmioGetInfo( m_hmmio, &mmioinfoIn, 0 ) )
		return Error::Handle("Wave Error: mmioGetInfo");
     
	// Data member specifying how many bytes to actually
	// read out of the wave file.
	uint32 nDataIn = 0;
	BYTE* pWaveBuffer = 0;
	uint32 nStreamRead = 0;

	// If we're decompressing this data... (ignore if using PCM)
	if(m_hACMStream)
	{
		// Prepare the ACM buffers for decompression
		if(!PrepareACMBuffers(nDataIn, nStreamRead, dwSizeToRead))
			return false;

		// Use the oversized compression buffer to read the source
		// before sending it to the decompression routines.
		pWaveBuffer = m_pCompressBuffer;
	}
	else
	{
		// If we're not decompressing data, we can directly 
		// use the buffer passed in to this function.
		pWaveBuffer = pDestBuffer;
		nDataIn = dwSizeToRead;
	}


	// Begin reading from the actual wave data
	if( nDataIn > m_ck.cksize ) 
		nDataIn = m_ck.cksize;  
	m_ck.cksize -= nDataIn;


	// Should do a comparison test to see if the new
	// reader is faster than a byte-by-byte copy.
	// For now, I'm assuming it probably is by a
	// small degree.
	for( DWORD cT = 0; cT < nDataIn; cT++ )
	{
		// Copy the bytes from the io to the buffer.
		if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
		{
			if( 0 != mmioAdvance( m_hmmio, &mmioinfoIn, MMIO_READ ) )
				return Error::Handle("Wave Error: mmioAdvance");

			if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
				return Error::Handle("Wave Error: mmioinfoIn.pchNext");
		}

		// Actual copy.
		*((BYTE*)pWaveBuffer+cT) = *((BYTE*)mmioinfoIn.pchNext);
		mmioinfoIn.pchNext++;
	}

/*
	uint32 nRead = 0;
	uint32 nCopySize;
	while(nRead < nDataIn)
	{
        // Copy the bytes from the io to the buffer.
		if (0 != mmioAdvance(m_hmmio, &mmioinfoIn, MMIO_READ))
			return Error::Handle("Wave Error: mmioAdvance");
			
			  if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
			return Error::Handle("Wave Error: mmioinfoIn.pchNext");

		// Actual copy.
		nCopySize = mmioinfoIn.pchEndRead -  mmioinfoIn.pchNext;
		nCopySize = ClampMax<uint32>(nCopySize, nDataIn - nRead);
		memcpy(pWaveBuffer + nRead, mmioinfoIn.pchNext, nCopySize);
		nRead += nCopySize;
		mmioinfoIn.pchNext += nCopySize;
	}
*/
	if( 0 != mmioSetInfo( m_hmmio, &mmioinfoIn, 0 ) )
		return Error::Handle("Wave Error: mmioSetInfo");

	// Report the number of bytes read
	if( pdwSizeRead != NULL)
		*pdwSizeRead = nDataIn;

	// Check to see if we hit the end of the file
	if(m_ck.cksize == 0)
		m_bEOF = true;

	// Convert to a PCM format if required
	if(m_hACMStream)
	{
		// Decompress the wave data stored in the compress buffer
		if(!DecompressData(nDataIn, pDestBuffer, dwSizeToRead, pdwSizeRead))
			return false;
	}

	return true;
}


//------------------------------------------------------------------------//
bool Wave::PrepareACMBuffers(uint32& nDataIn, uint32& nStreamRead, uint32 dwSizeToRead)
{
	// This is where we overestimate by a specified amount.  Note
	// that we reduce the overestimation by the amount of valid
	// data left over in the decompression buffer from the last
	// call to this function.
	nStreamRead = dwSizeToRead + ACM_BUFFER_OVERESTIMATE - 
		(m_nDecompressEnd - m_nDecompressStart);
	nStreamRead = ClampMin<int>(nStreamRead, ACM_BUFFER_OVERESTIMATE);

	// Use acmStreamSize to get a rough estimate of how much data we
	// should read from the wave file to decompress to fill at
	// least dwSizeToRead bytes.  cbDataIn should end up slightly
	// more than dwSizeToRead.
	MMRESULT error = acmStreamSize(m_hACMStream, nStreamRead, &nDataIn, 
		ACM_STREAMSIZEF_DESTINATION);
	int iErrorCount = 0;
	while(error)
	{
		// acmStreamSize cannot estimate below certain minimum sizes,
		// so we attempt to increase the minimum estimation until
		// we either succeed or fail 10 times.
		nStreamRead += ACM_BUFFER_OVERESTIMATE;
		DebugOut(5, "acmStreamSize() failed.  Resizing estimated buffer to %u", nStreamRead);
		error = acmStreamSize(m_hACMStream, nStreamRead, &nDataIn, 
			ACM_STREAMSIZEF_DESTINATION);
		iErrorCount++;
		if(iErrorCount > 10)
			return Error::Handle("Could not estimate compressed stream data");
	}

	// Check to see if we need to grow the conversion buffer
	if(nStreamRead > m_nDecompressBufferSize)
	{
		uint8* pTempBuffer = new BYTE[nStreamRead];
		DebugOut(1, "Allocating new buffer size of size %d", nStreamRead);
		// Copy any valid data into the beginning of the new buffer if
		// there is any valid data there
		if(m_nDecompressStart != m_nDecompressEnd)
			memcpy(pTempBuffer, m_pDecompressBuffer + m_nDecompressStart, 
				m_nDecompressEnd - m_nDecompressStart);
		m_nDecompressBufferSize = nStreamRead;
		_XASAFE_DELETE_ARRAY(m_pDecompressBuffer);
		m_pDecompressBuffer = pTempBuffer;
	}
	else
	{
		// Otherwise, just move any "leftover" data back 
		// to the beginning of the buffer.
		memmove(m_pDecompressBuffer, m_pDecompressBuffer + 
			m_nDecompressStart, m_nDecompressEnd - m_nDecompressStart);
	}
	// Reset the data markers
	m_nDecompressEnd = m_nDecompressEnd - m_nDecompressStart;
	m_nDecompressStart = 0;
	
	// Now, if needed, create (or recreate) our compressed buffer, 
	// which is larger than the actual destination size.
	if(m_nCompressBufferSize < nDataIn)
	{
		_XASAFE_DELETE_ARRAY(m_pCompressBuffer);
		m_pCompressBuffer = new uint8[nStreamRead];
		m_nCompressBufferSize = nDataIn;
	}

	return true;
}


//------------------------------------------------------------------------//
bool Wave::DecompressData(uint32 nDataIn, BYTE* pDestBuffer, 
	uint32 dwSizeToRead, DWORD* pdwSizeRead)
{
	// Prepare the converstion data
	ACMSTREAMHEADER StreamHeader;
	memset(&StreamHeader, 0, sizeof(ACMSTREAMHEADER));
	StreamHeader.cbStruct = sizeof(ACMSTREAMHEADER);
	StreamHeader.pbSrc = m_pCompressBuffer;
	StreamHeader.cbSrcLength = nDataIn;
	StreamHeader.pbDst = m_pDecompressBuffer + m_nDecompressEnd;
	StreamHeader.cbDstLength = m_nDecompressBufferSize - m_nDecompressEnd;
	MMRESULT error = acmStreamPrepareHeader(m_hACMStream, &StreamHeader, 0);
	if(error)
		// Signal an error, but go on anyway
		Error::Handle("Could not prepare wave format conversion stream header");

	// Do the actual non-PCM to PCM data conversion
	if(!error)
	{
		error = acmStreamConvert(m_hACMStream, &StreamHeader, 
			ACM_STREAMCONVERTF_START | ACM_STREAMCONVERTF_END |
			ACM_STREAMCONVERTF_BLOCKALIGN);
		if(error)
			return Error::Handle("Error converting non-PCM data to PCM format");
	}

	// Add the number of bytes read to the "valid content" 
	// markers in the uncompressed buffer
	m_nDecompressEnd += StreamHeader.cbDstLengthUsed;

	// Determine how much data we can actually copy
	uint32 nDataToCopy;
	if((m_nDecompressEnd - m_nDecompressStart) < dwSizeToRead)
		nDataToCopy = m_nDecompressEnd - m_nDecompressStart;
	else
		nDataToCopy = dwSizeToRead;

	// Copy the decompressed data from our oversized buffer to the
	memcpy(pDestBuffer, m_pDecompressBuffer, nDataToCopy);
	
	// Record how many bytes were actually read
	if( pdwSizeRead != NULL)
		*pdwSizeRead = nDataToCopy;

	// Advance the start of the valid data pointer by the 
	// amount of data copied
	m_nDecompressStart += nDataToCopy;
	
	// Clean up any internal conversion data
	if(!error)
	{
		error = acmStreamUnprepareHeader(m_hACMStream, &StreamHeader, 0);
		if(error)
			return Error::Handle("Error in cleaning up non-PCM conversion data");
	}

	return true;
}


//------------------------------------------------------------------------//
// Closes the wave file 
bool Wave::Close()
{
	mmioClose( m_hmmio, 0 );
	m_hmmio = NULL;
	_XASAFE_DELETE_ARRAY(m_pwfx);
	m_dwSize  = 0;
	m_bEOF = false;

	m_nDecompressStart = 0;
	m_nDecompressEnd = 0;

	s_nWaveCount--;

	return true;
}


//------------------------------------------------------------------------//
void Wave::Destroy()
{
	Close();
	Wave::DestroyObject(this);
}














