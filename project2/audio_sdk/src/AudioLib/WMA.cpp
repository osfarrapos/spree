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
#include "WMA.h"
#include "AudioMgr.h"

#ifdef USE_WMA
using namespace std;
using namespace Audio;


const uint32 DECODING_BUFFER_ERROR = 8192;

//------------------------------------------------------------------------//
WMACallback::WMACallback()
{
	Clear();
}

//------------------------------------------------------------------------//
WMACallback::~WMACallback()
{
	assert(!m_iRefCount);
}

//------------------------------------------------------------------------//
void WMACallback::Clear()
{
	m_iRefCount = 0;
}


//------------------------------------------------------------------------//
HRESULT WMACallback::QueryInterface(
	const struct _GUID& guid, 
	void** ppInterface)
{
	if ( guid == IID_IWMReaderCallback )
		*ppInterface = ( IWMReaderCallback* )this;
	else if( guid == IID_IWMReaderCallbackAdvanced )
		*ppInterface = ( IWMReaderCallbackAdvanced* )this;
	else
		return E_NOINTERFACE;
	return S_OK;
}

//------------------------------------------------------------------------//
ULONG WMACallback::AddRef()
{
	return 1;
}

//------------------------------------------------------------------------//
ULONG WMACallback::Release()
{
	return 1;
}

//------------------------------------------------------------------------//
HRESULT WMACallback::OnStatus(
	WMT_STATUS  Status,
	HRESULT  hr,
	WMT_ATTR_DATATYPE  dwType,
	BYTE*  pValue,
	void*  pvContext)
{
	if(!pvContext)
		return E_FAIL;
	WMA* pWMA = static_cast<WMA*>(pvContext);
	return pWMA->OnStatus(Status, hr, dwType, pValue);
}

//------------------------------------------------------------------------//
HRESULT WMACallback::OnSample(
	DWORD  dwOutputNum,
	QWORD  cnsSampleTime,
	QWORD  cnsSampleDuration,
	DWORD  dwFlags,
	INSSBuffer*  pSample,
	void*  pvContext)
{
	if(!pvContext)
		return E_FAIL;
	WMA* pWMA = static_cast<WMA*>(pvContext);
	return pWMA->OnSample(dwOutputNum, cnsSampleTime, cnsSampleDuration, dwFlags, pSample);
}

//------------------------------------------------------------------------//
HRESULT WMACallback::OnStreamSample( 
    WORD wStreamNum,
    QWORD cnsSampleTime,
    QWORD cnsSampleDuration,
    DWORD dwFlags,
    INSSBuffer __RPC_FAR *pSample,
    void __RPC_FAR *pvContext)
{
    return E_NOTIMPL;
}

//------------------------------------------------------------------------//
HRESULT WMACallback::OnTime( 
    QWORD qwCurrentTime,
    void __RPC_FAR *pvContext)
{
	if(!pvContext)
		return E_FAIL;
	WMA* pWMA = static_cast<WMA*>(pvContext);
	return pWMA->OnTime(qwCurrentTime);
}

//------------------------------------------------------------------------//
HRESULT WMACallback::OnStreamSelection( 
    WORD wStreamCount,
    WORD __RPC_FAR *pStreamNumbers,
    WMT_STREAM_SELECTION __RPC_FAR *pSelections,
    void __RPC_FAR *pvContext)
{
    return S_OK;
}

//------------------------------------------------------------------------//
HRESULT WMACallback::OnOutputPropsChanged( 
    DWORD dwOutputNum,
    WM_MEDIA_TYPE __RPC_FAR *pMediaType,
    void __RPC_FAR *pvContext )
{
    return S_OK;
}

//------------------------------------------------------------------------//
HRESULT WMACallback::AllocateForOutput( 
    DWORD dwOutputNum,
    DWORD cbBuffer,
    INSSBuffer __RPC_FAR *__RPC_FAR *ppBuffer,
    void __RPC_FAR *pvContext)
{
    return E_NOTIMPL;
}

//------------------------------------------------------------------------//
HRESULT WMACallback::AllocateForStream( 
    WORD wStreamNum,
    DWORD cbBuffer,
    INSSBuffer __RPC_FAR *__RPC_FAR *ppBuffer,
    void __RPC_FAR *pvContext)
{
    return E_NOTIMPL;
}


IMPLEMENT_POOL(WMA);


//------------------------------------------------------------------------//
WMA::WMA()
{
	FN("WMA::WMA()");
	m_pBuffer = 0;
	m_nBufferSize = 0;
	Clear();
}

//------------------------------------------------------------------------//
WMA::~WMA()
{
	FN("WMA::~WMA()");
	Close();
	_XASAFE_DELETE_ARRAY(m_pBuffer);
	m_nBufferSize = 0;
}

//------------------------------------------------------------------------//
void WMA::Clear()
{
	FN("WMA::Clear()");
	m_pReader = 0;
	m_pReaderAdvanced = 0;
	m_pReaderAdvanced2 = 0;
	m_pHeaderInfo = 0;
	ZeroMemory(&m_WaveFormatEx, sizeof(WAVEFORMATEX));
	m_hWaitEvent = 0;
    m_hRespondEvent = 0;
	m_bOpen = false;
	m_Callback.Clear();
	m_hrCallbackResult = 0;
	m_nStreamSize = 0;
	m_nBytesRead = 0;
	m_bFirstRead = true;
	m_nTargetPtr = 0;
	m_nWritePtr = 0;
	m_bEOF = false;
	m_qwTime = 0;

	ZeroMemory(&m_csTerm, sizeof(CRITICAL_SECTION));
}


//------------------------------------------------------------------------//
bool WMA::PreOpen()
{
	InitializeCriticalSection(&m_csTerm);

    m_hWaitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hRespondEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	HRESULT hr = WMCreateReader(0, 0, &m_pReader);
	if(FAILED(hr))
		return false;

	hr = m_pReader->QueryInterface(
		IID_IWMReaderAdvanced, 
		(void**)&m_pReaderAdvanced);
	if(FAILED(hr))
		return false;

	hr = m_pReaderAdvanced->QueryInterface(
		IID_IWMReaderAdvanced2, 
		(void**)&m_pReaderAdvanced2);
	if(FAILED(hr))
		return false;

	hr = m_pReader->QueryInterface(
		IID_IWMHeaderInfo, 
		(void**)&m_pHeaderInfo);
	if(FAILED(hr))
		return false;

	return true;
}


//------------------------------------------------------------------------//
// This Open function is used if the source is either streaming or a 
// one-shot load.  It also is used for custom file systems.
bool WMA::Open(string sFileName)
{
	FN("WMA::Open()");

	if(!PreOpen())
		return false;
	
	IAudioStream* pStream;
	CreateAudioStream cas(pStream);
	if(!pStream)
		return false;

	HRESULT hr = pStream->Open(sFileName);
	if(FAILED(hr))
		return false;

	hr = m_pReaderAdvanced2->OpenStream(pStream, &m_Callback, this);
	if(FAILED(hr))
		return false;

	if(!PostOpen())
		return false;

	return true;
}

//------------------------------------------------------------------------//
// This Open function is used for loading directly from memory
bool WMA::Open(uint8* pbData, uint32 dwDataSize)
{
	FN("WMA::Open()");

	if(!PreOpen())
		return false;

	HGLOBAL hMem;
	IStream* pStream;

	hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD | GMEM_SHARE, dwDataSize);

	LPVOID pMem = GlobalLock(hMem);
	CopyMemory(pMem, pbData, dwDataSize);
	GlobalUnlock(hMem);

	HRESULT hr = CreateStreamOnHGlobal(hMem, TRUE, &pStream);
	if(FAILED(hr))
		return false;
	
	hr = m_pReaderAdvanced2->OpenStream(pStream, &m_Callback, this);
	if(FAILED(hr))
		return false;

	if(!PostOpen())
		return false;

	return true;
}


//------------------------------------------------------------------------//
bool WMA::PostOpen()
{
	WaitForSingleObject(m_hRespondEvent, INFINITE);
	if(FAILED(m_hrCallbackResult))
		return false;

	uint32 nOutputCount;
	HRESULT hr = m_pReader->GetOutputCount(&nOutputCount);
	if(FAILED(hr))
		return false;
	if(nOutputCount != 1)
		return false;

	uint32 nOutputFormatCount;
	hr = m_pReader->GetOutputFormatCount(0, &nOutputFormatCount);
	if(FAILED(hr))
		return false;

	uint32 nFormatSize = 0;	
	BYTE* pBuf = 0;
	IWMOutputMediaProps* pProps;
	for(uint32 j = 0; j < nOutputFormatCount; j++)
	{
		hr = m_pReader->GetOutputFormat(0, j, &pProps);
		if(FAILED(hr))
			continue;

		// Get the required size of the media type structure
		uint32 nNewSize = 0;
		hr = pProps->GetMediaType(NULL, &nNewSize);
		if(FAILED(hr))
			continue;
		if(nNewSize > nFormatSize)
		{
			_XASAFE_DELETE_ARRAY(pBuf);
			nFormatSize = nNewSize;
			pBuf = new BYTE[nFormatSize];
		}

		WM_MEDIA_TYPE* pType = (WM_MEDIA_TYPE*) pBuf;
		hr = pProps->GetMediaType(pType, &nFormatSize);
		if(FAILED(hr))
			continue;

		if(pType->formattype == WMFORMAT_WaveFormatEx)
		{
			memcpy(&m_WaveFormatEx, pType->pbFormat, pType->cbFormat);
			if((m_WaveFormatEx.nChannels == 2) &&
				(m_WaveFormatEx.wBitsPerSample == DXAudioMgr()->GetOptimalSampleBits()) &&
				(m_WaveFormatEx.nSamplesPerSec == DXAudioMgr()->GetOptimalSampleRate()))
			{
				break;
			}
		}
		_XASAFE_RELEASE(pProps);
	}
	_XASAFE_DELETE_ARRAY(pBuf);

	// Now set the format we want
	hr = m_pReader->SetOutputProps(0, pProps);
	if(FAILED(hr))
		return false;

	_XASAFE_RELEASE(pProps);

	// Tell the reader to read as fast as possible
	hr = m_pReaderAdvanced->SetUserProvidedClock(true);
	if(FAILED(hr))
		return false;

	// Determine the size of the opened file
	WORD wStreamNum = 0;
	WMT_ATTR_DATATYPE Type;
	DWORD dwDuration = 0;
	WORD wLength = 8;
	hr = m_pHeaderInfo->GetAttributeByName(
		&wStreamNum,
		g_wszWMDuration,
		&Type,
		(unsigned char*)&dwDuration,
		&wLength);
	if(FAILED(hr))
		return false;

	// divide by 10 million to get seconds
	double fTime = double(dwDuration) / 10000000.0f;
		
	// Calculate the stream size
	m_nStreamSize = fTime * m_WaveFormatEx.nAvgBytesPerSec;

	// Create a default one-point-five second scratch buffer for decoding streams
	if(!m_pBuffer)
	{
		m_nBufferSize = m_WaveFormatEx.nAvgBytesPerSec * 1.5;
		m_pBuffer = new uint8[m_nBufferSize];
	}

	m_nTargetPtr = 0;
	m_nWritePtr = 0;

	m_bOpen = true;

	return true;
}

//------------------------------------------------------------------------//
bool WMA::Close()
{
	FN("WMA::Close()");

	m_bOpen = false;

	if(m_hWaitEvent)
	{
		if(m_pReader)
			m_pReader->Stop();

		SetEvent(m_hWaitEvent);
		EnterCriticalSection(&m_csTerm);
		LeaveCriticalSection(&m_csTerm);
		DeleteCriticalSection(&m_csTerm);
	}

	if(m_hWaitEvent)
	{
		CloseHandle(m_hWaitEvent);
		m_hWaitEvent = 0;
	}
	if(m_hRespondEvent)
    {
		CloseHandle(m_hRespondEvent);
		m_hRespondEvent = 0;
	}

	_XASAFE_RELEASE(m_pHeaderInfo);
	_XASAFE_RELEASE(m_pReaderAdvanced2);
	_XASAFE_RELEASE(m_pReaderAdvanced);
	_XASAFE_RELEASE(m_pReader);

	Clear();

	return true;
}


//------------------------------------------------------------------------//
bool WMA::Read(uint8* pBuffer, uint32 dwSizeToRead, uint32* pdwSizeRead )
{
	FN("WMA::Read()");

	if(!m_bOpen)
		return false;
	DebugOut(5, "Reading %d bytes from WMA file", dwSizeToRead);

	m_nTargetPtr = dwSizeToRead - m_nWritePtr;

	// First, we have to check to see if we have enough extra data
	// stored in the buffer to satisfy the read without doing
	// any decoding.
	if(dwSizeToRead > m_nWritePtr)
	{
		if(m_bFirstRead)
		{
			// Grow the decoding buffer if necessary
			if(dwSizeToRead > m_nBufferSize - DECODING_BUFFER_ERROR)
			{
				_XASAFE_DELETE(m_pBuffer);
				m_nBufferSize = dwSizeToRead;
				m_pBuffer = new uint8[m_nBufferSize + DECODING_BUFFER_ERROR];
			}

			// Start decoding now
			HRESULT hr;
			hr = m_pReader->Start(0, 0, 1.0f, this);
			m_bFirstRead = false;
			if(FAILED(hr))
				return false;
		}
		else
		{
			// Release the decoding thread so decoding continues
			SetEvent(m_hWaitEvent);
		}

		WaitForSingleObject(m_hRespondEvent, INFINITE);
		if(FAILED(m_hrCallbackResult))
			return false;
	}

	// Normally we should have more than enough data in the buffer...
	if(m_nWritePtr >= dwSizeToRead)
	{
		memcpy(pBuffer, m_pBuffer, dwSizeToRead);
		*pdwSizeRead = dwSizeToRead;
		memmove(m_pBuffer, m_pBuffer + dwSizeToRead, m_nWritePtr - dwSizeToRead);
		m_nWritePtr -= dwSizeToRead;
	}
	// but if we're at the end of the file, we may have less than requested
	else
	{
		memcpy(pBuffer, m_pBuffer, m_nWritePtr);
		*pdwSizeRead = m_nWritePtr;
		m_nWritePtr = 0;
	}

	m_nBytesRead += *pdwSizeRead;

	if(m_bEOF)
		m_pReader->Stop();

	return true;
}

//------------------------------------------------------------------------//
uint32 WMA::GetSize()
{
	FN("WMA::GetSize()");
	return m_nStreamSize;
}

//------------------------------------------------------------------------//
bool WMA::Reset()
{
	FN("WMA::Reset()");

	if(!m_bOpen)
		return false;

	m_nBytesRead = 0;
	m_pReader->Stop();
	m_bFirstRead = true;
	m_bEOF = false;
	m_nTargetPtr = 0;
	m_nWritePtr = 0;
	m_qwTime = 0;
	ZeroMemory(m_pBuffer, m_nBufferSize);
	SetEvent(m_hWaitEvent);

	return true;
}

//------------------------------------------------------------------------//
WAVEFORMATEX* WMA::GetFormat()
{
	FN("WMA::GetFormat()");
	return &m_WaveFormatEx;
}

//------------------------------------------------------------------------//
bool WMA::IsEOF()
{
	FN("WMA::IsEOF()");
	return m_bEOF;
}

//------------------------------------------------------------------------//
void WMA::Destroy()
{
	Close();
	WMA::DestroyObject(this);
}


//------------------------------------------------------------------------//
HRESULT WMA::OnStatus(
	WMT_STATUS  Status,
	HRESULT  hr,
	WMT_ATTR_DATATYPE  dwType,
	BYTE*  pValue)
{

	m_hrCallbackResult = hr;
	switch(Status)
	{
	case WMT_ERROR:
		SetEvent(m_hWaitEvent);
		SetEvent(m_hRespondEvent);
		DebugOut(1, "Status: WMT_ERROR");
		return E_FAIL;
	case WMT_OPENED:
		DebugOut(5, "Status: WMT_OPENED");
		SetEvent(m_hRespondEvent);
		break;
	case WMT_BUFFERING_START:
		DebugOut(5, "Status: WMT_BUFFERING_START");
		break;
	case WMT_BUFFERING_STOP:
		DebugOut(5, "Status: WMT_BUFFERING_STOP");
		break;
	case WMT_END_OF_FILE:
		DebugOut(5, "Status: WMT_END_OF_FILE");
		m_bEOF = true;
		// Make sure no threads are kept waiting, since no more reads will come
		SetEvent(m_hWaitEvent);
		SetEvent(m_hRespondEvent);
		break;
	case WMT_END_OF_SEGMENT:
		DebugOut(5, "Status: WMT_END_OF_SEGMENT");
		break;
	case WMT_END_OF_STREAMING:
		DebugOut(5, "Status: WMT_END_OF_STREAMING");
		break;
	case WMT_STARTED:
		DebugOut(5, "Status: WMT_STARTED");
		DebugOut(5, "Advancing deliver time at start");
        m_qwTime += 1000000;
		if(m_pReaderAdvanced)
			m_pReaderAdvanced->DeliverTime(m_qwTime);
		break;
	case WMT_STOPPED:
		DebugOut(5, "Status: WMT_STOPPED");
		SetEvent(m_hWaitEvent);
		SetEvent(m_hRespondEvent);
		break;
	case WMT_CLOSED:
		DebugOut(5, "Status: WMT_CLOSED");
		break;
	case WMT_TIMER:
		DebugOut(5, "Status: WMT_TIMER");
		break;
	case WMT_SOURCE_SWITCH:
		DebugOut(5, "Status: WMT_SOURCE_SWITCH");
		break;

	};

	return S_OK;
}


//------------------------------------------------------------------------//
HRESULT WMA::OnTime( 
    QWORD qwCurrentTime)
{

	// Keep asking for the specific duration of the stream till EOF
	if ( !m_bEOF )
	{
		DebugOut(5, "Advancing deliver time in OnTime() function");
		m_qwTime += 1000000;

		if(m_pReaderAdvanced)
			m_pReaderAdvanced->DeliverTime( m_qwTime );
    }

	return S_OK;
}


//------------------------------------------------------------------------//
HRESULT WMA::OnSample(
	DWORD  dwOutputNum,
	QWORD  cnsSampleTime,
	QWORD  cnsSampleDuration,
	DWORD  dwFlags,
	INSSBuffer*  pSample)
{

	if(!m_bOpen)
	{
		if(m_hRespondEvent)
			SetEvent(m_hRespondEvent);
		return S_OK;
	}

	if(m_bEOF)
	{
		SetEvent(m_hRespondEvent);
		return S_OK;
	}

	BYTE* pBuf;
	DWORD dwLen;
	if(!pSample)
		return E_FAIL;

	HRESULT hr = pSample->GetBuffer(&pBuf);
	if(FAILED(hr))
		return E_FAIL;
	hr = pSample->GetLength(&dwLen);
	if(FAILED(hr))
		return E_FAIL;

	// Expand the decoding buffer if needed
	if((m_nWritePtr + dwLen) > m_nBufferSize)
	{
		uint32 nNewBufferSize = m_nWritePtr + dwLen + DECODING_BUFFER_ERROR;
		DebugOut(1, "Expanding buffer from length %d to %d", m_nBufferSize, nNewBufferSize);
		uint8* pNewBuffer = new uint8[nNewBufferSize];
		memcpy(pNewBuffer, m_pBuffer, m_nWritePtr);
		_XASAFE_DELETE_ARRAY(m_pBuffer);
		m_pBuffer = pNewBuffer;
		m_nBufferSize = nNewBufferSize;
	}

	memcpy(m_pBuffer + m_nWritePtr, pBuf, dwLen);
	m_nWritePtr += dwLen;

	if(m_nWritePtr >= m_nTargetPtr)
	{
		SetEvent(m_hRespondEvent);
		CRITICAL_FUNCTION(&m_csTerm);
		WaitForSingleObject(m_hWaitEvent, INFINITE);
	}

	return S_OK;
}


#endif // USE_WMA

