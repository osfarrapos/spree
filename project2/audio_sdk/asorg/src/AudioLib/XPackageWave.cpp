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
#include "XPackageWave.h"
#include "XPackage.h"

using namespace std;
using namespace Audio;

IMPLEMENT_POOL(_XPackageWave);

extern void _XSetDefaultPath ( LPSTR localpath );

//------------------------------------------------------------------------//
// Constructs the class.  Call Open() to open a wave file for reading.  
// Then call Read() as needed.  Calling the destructor or Close() 
// will close the file.  
_XPackageWave::_XPackageWave()
{	
	memset(&m_ck, 0, sizeof(MMCKINFO));
	memset(&m_ckRiff, 0, sizeof(MMCKINFO));
	m_pwfx    = NULL;
	m_hmmio   = NULL;
	m_dwSize  = 0;
	m_bEOF = false;
	memset(&m_DestFormat, 0, sizeof(WAVEFORMATEX));
		
	m_pResourceBuffer = NULL;
	m_pPackage = NULL;
	m_pbDataCur = m_pbData = NULL;
	m_ulDataSize = 0;
}

//------------------------------------------------------------------------//
// Destructs the class
_XPackageWave::~_XPackageWave()
{	
	Close();
	_XASAFE_DELETE_ARRAY(m_pResourceBuffer);
}


//------------------------------------------------------------------------//
// Opens a wave file for reading from an IAudioStream system
bool _XPackageWave::Open(string sFileName)
{	
	if( !m_pPackage ) return false;

	m_sFileName = sFileName;

	LONG   dwDataSize;
	
	int dataindex = m_pPackage->FindResource( (LPSTR)m_sFileName.c_str() );
	if( dataindex > -1 )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH);
		
		m_pPackage->ReOpen();
		m_pResourceBuffer = m_pPackage->LoadPackedFileToMemory( (DWORD)dataindex, &dwDataSize  );
		m_pPackage->Close( FALSE );
	}
	else
	{
		return Error::Handle("Wave Error: ReadMMIO");
	}
	
	MMIOINFO mmioInfo;
	ZeroMemory( &mmioInfo, sizeof(mmioInfo) );
	mmioInfo.fccIOProc = FOURCC_MEM;
	mmioInfo.cchBuffer = dwDataSize;
	mmioInfo.pchBuffer = (CHAR*) m_pResourceBuffer;
				
	// Open the memory buffer
	m_hmmio = mmioOpen(NULL, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ );

	return OpenMMIO();
}

//------------------------------------------------------------------------//
// Open a wave file format from a memory buffer
bool _XPackageWave::Open( BYTE* pbData, uint32 dwDataSize)
{
	// Indicate to read from a memory buffer
	MMIOINFO mmioInfo;
	ZeroMemory( &mmioInfo, sizeof(mmioInfo) );
	mmioInfo.fccIOProc = FOURCC_MEM;
	mmioInfo.cchBuffer = dwDataSize;
	mmioInfo.pchBuffer = (CHAR*) pbData;
	
	// Open the memory buffer
	m_hmmio = mmioOpen(NULL, &mmioInfo, MMIO_READ);
			
	return OpenMMIO();
}

//------------------------------------------------------------------------//
// Resets the internal m_ck pointer so reading starts from the 
// beginning of the file again 
bool _XPackageWave::Reset()
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
	
	return true;
}


//------------------------------------------------------------------------//
bool _XPackageWave::OpenMMIO()
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

	m_dwSize = m_ck.cksize;
	
	return true;
}


//------------------------------------------------------------------------//
// Verifies that this is a wave file, and allocates and 
// fills out the wave format header structure.
bool _XPackageWave::ReadMMIO()
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
		return Error::Handle("Could not support this format");
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
// Reads section of data from a wave file into pBuffer and returns 
// how much read in pdwSizeRead, reading not more than dwSizeToRead.
// This uses m_ck to determine where to start reading from.  So 
// subsequent calls will be continue where the last left off unless 
// Reset() is called.
bool _XPackageWave::Read( BYTE* pDestBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )
{	
	// Search the input file for the 'data' chunk.
	m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if( 0 != mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
		return Error::Handle("Wave Error: mmioDescend");

	if( dwSizeToRead < m_ck.cksize )
	{
		m_ulDataSize = m_ck.cksize;
	}
	else
	{
		m_ulDataSize = dwSizeToRead;
	}

	if( pdwSizeRead != NULL ) *pdwSizeRead = 0;
	
	dwSizeToRead = mmioRead(m_hmmio, (HPSTR)pDestBuffer, m_ulDataSize );
		    
    if( pdwSizeRead != NULL )
        *pdwSizeRead = dwSizeToRead;	

	return true;
}


//------------------------------------------------------------------------//
// Closes the wave file 
bool _XPackageWave::Close()
{		
	mmioClose( m_hmmio, 0 );
	m_hmmio = NULL;
	_XASAFE_DELETE_ARRAY(m_pwfx);
	m_dwSize  = 0;
	m_bEOF = false;
	
	_XASAFE_DELETE_ARRAY(m_pResourceBuffer);
	
	return true;
}


//------------------------------------------------------------------------//
void _XPackageWave::Destroy()
{
	Close();
	_XPackageWave::DestroyObject(this);
}

