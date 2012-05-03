#include "Audio.h"
#include "MemoryWave.h"

using namespace std;
using namespace Audio;

IMPLEMENT_POOL(MemoryWave);


#define _XDEF_RIFF_SIZE	12
#define _XDEF_FMT_SIZE	24
#define _XDEF_DATA_SIZE	8
#define _XDEF_FACT_SIZE	8

typedef struct _WAVE_RIFF
{
	char	riffID[4];			//4
	DWORD	riffSIZE;			//4
	char	riffFORMAT[4];		//4	
} WAVE_RIFF;

typedef struct _WAVE_FMT
{
	char	fmtID[4];			//4
	DWORD	fmtSIZE;			//4
	
	WORD	wFormatTag;			//2
	WORD	nChannels;			//2
	DWORD	nSamplesPerSec;		//4
	DWORD	nAvgBytesPerSec;	//4
	WORD	nBlockAlign;		//2
	WORD	wBitsPerSample;		//2
	
} WAVE_FMT;

typedef struct _WAVE_DATA
{
	char	dataID[4];			//4 bytes
	DWORD	dataSIZE;			// 4 bytes
} WAVE_DATA;

typedef struct _WAVE_FACT
{
	DWORD samplesNumber;
	DWORD t;
} WAVE_FACT;

MemoryWave::MemoryWave()
{
	memset(&m_ck, 0, sizeof(MMCKINFO));
	memset(&m_ckRiff, 0, sizeof(MMCKINFO));	
	m_hmmio   = NULL;
	m_dwSize  = 0;
	m_bEOF = false;
	memset(&m_DestFormat, 0, sizeof(WAVEFORMATEX));
}

//------------------------------------------------------------------------//
// Destructs the class
MemoryWave::~MemoryWave()
{
	Close();
}


//------------------------------------------------------------------------//
// Opens a wave file for reading from an IAudioStream system
bool MemoryWave::Open(string sFileName)
{
	return false;
}

//------------------------------------------------------------------------//
// Open a wave file format from a memory buffer
bool MemoryWave::Open( BYTE* pbData, uint32 dwDataSize)
{
	LPBYTE localpbData= pbData;

	_WAVE_RIFF* pRiffHeader = (_WAVE_RIFF*)localpbData;
	localpbData += _XDEF_RIFF_SIZE;

	_WAVE_FMT*  pFormatHeader = (_WAVE_FMT*)localpbData;
	localpbData += 8 + pFormatHeader->fmtSIZE;

	if( pFormatHeader->wFormatTag != WAVE_FORMAT_PCM )
	{
		memcpy( &m_DestFormat.cbSize, localpbData, sizeof(WORD) );
		localpbData += sizeof(WORD);

		if( m_DestFormat.cbSize > 0 )
		{	
			localpbData += m_DestFormat.cbSize;
		}
	}

	m_DestFormat.wFormatTag			= pFormatHeader->wFormatTag;
	m_DestFormat.nChannels			= pFormatHeader->nChannels;
	m_DestFormat.nSamplesPerSec		= pFormatHeader->nSamplesPerSec;
	m_DestFormat.nAvgBytesPerSec	= pFormatHeader->nAvgBytesPerSec;
	m_DestFormat.nBlockAlign		= pFormatHeader->nBlockAlign;
	m_DestFormat.wBitsPerSample		= pFormatHeader->wBitsPerSample;	

	_WAVE_DATA* pWaveData = (_WAVE_DATA*)localpbData;
	_WAVE_FACT* pFactData;
	localpbData += 4;
	if (pWaveData->dataID[0]=='d' && pWaveData->dataID[1]=='a' && pWaveData->dataID[2]=='t' && pWaveData->dataID[3]=='a')
	{
		localpbData += 4;
	}
	else
	{
		if (pWaveData->dataID[0]=='J' && pWaveData->dataID[1]=='U' && pWaveData->dataID[2]=='N' && pWaveData->dataID[3]=='K')
		{
			localpbData += (pWaveData->dataSIZE + 4);
			pWaveData = (_WAVE_DATA*)localpbData;
		}
		else if (pWaveData->dataID[0]=='f' && pWaveData->dataID[1]=='a' && pWaveData->dataID[2]=='c' && pWaveData->dataID[3]=='t')
		{
			pFactData = (_WAVE_FACT*)localpbData;
			localpbData += _XDEF_FACT_SIZE;
			pWaveData = (_WAVE_DATA*)localpbData;
		}
	}
	
	m_dwSize		=   m_ulDataSize	=	pWaveData->dataSIZE;
	m_pbData		=	localpbData;
	m_pbDataCur		=	m_pbData;

	return true;
}

//------------------------------------------------------------------------//
// Resets the internal m_ck pointer so reading starts from the 
// beginning of the file again 
bool MemoryWave::Reset()
{
	m_pbDataCur = m_pbData;
	return true;
}

bool MemoryWave::Read( BYTE* pDestBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )
{
    if( m_pbDataCur == NULL )
        return false;
    if( pdwSizeRead != NULL )
        *pdwSizeRead = 0;
	
    if( (BYTE*)(m_pbDataCur + dwSizeToRead) > 
        (BYTE*)(m_pbData + m_ulDataSize) )
    {
        dwSizeToRead = m_ulDataSize - (DWORD)(m_pbDataCur - m_pbData);
    }
    
    CopyMemory( pDestBuffer, m_pbDataCur, dwSizeToRead );
    
    if( pdwSizeRead != NULL )
        *pdwSizeRead = dwSizeToRead;

	return true;
}

//------------------------------------------------------------------------//
// Closes the wave file 
bool MemoryWave::Close()
{
	return true;
}


//------------------------------------------------------------------------//
void MemoryWave::Destroy()
{
	Close();
	MemoryWave::DestroyObject(this);
}














