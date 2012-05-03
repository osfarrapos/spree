/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __WMA_H__
#define __WMA_H__

#ifdef USE_WMA


#include "Audio.h"
#include "IAudioLoader.h"
#include "WMSDK.h"


namespace Audio
{

class WMACallback : public IWMReaderCallback, public IWMReaderCallbackAdvanced
{
public:
	WMACallback();
	virtual ~WMACallback();
	void Clear();

	HRESULT __stdcall QueryInterface(
		const struct _GUID& guid, 
		void** ppInterface);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	HRESULT __stdcall OnStatus(
		WMT_STATUS  Status,
		HRESULT  hr,
		WMT_ATTR_DATATYPE  dwType,
		BYTE*  pValue,
		void*  pvContext);

	HRESULT __stdcall OnSample(
		DWORD  dwOutputNum,
		QWORD  cnsSampleTime,
		QWORD  cnsSampleDuration,
		DWORD  dwFlags,
		INSSBuffer*  pSample,
		void*  pvContext);

     HRESULT __stdcall OnStreamSample( 
        WORD wStreamNum,
        QWORD cnsSampleTime,
        QWORD cnsSampleDuration,
        DWORD dwFlags,
        INSSBuffer __RPC_FAR *pSample,
        void __RPC_FAR *pvContext);
    
    HRESULT __stdcall OnTime( 
        QWORD qwCurrentTime,
        void __RPC_FAR *pvContext);

   HRESULT __stdcall OnStreamSelection( 
        WORD wStreamCount,
        WORD __RPC_FAR *pStreamNumbers,
        WMT_STREAM_SELECTION __RPC_FAR *pSelections,
        void __RPC_FAR *pvContext);

    HRESULT __stdcall OnOutputPropsChanged( 
        DWORD dwOutputNum,
        WM_MEDIA_TYPE __RPC_FAR *pMediaType,
        void __RPC_FAR *pvContext );

    HRESULT __stdcall AllocateForOutput( 
        DWORD dwOutputNum,
        DWORD cbBuffer,
        INSSBuffer __RPC_FAR *__RPC_FAR *ppBuffer,
        void __RPC_FAR *pvContext);

    HRESULT __stdcall AllocateForStream( 
        WORD wStreamNum,
        DWORD cbBuffer,
        INSSBuffer __RPC_FAR *__RPC_FAR *ppBuffer,
        void __RPC_FAR *pvContext);


private:
	LONG	m_iRefCount;
};



class WMA : public IAudioLoader
{
DEFINE_POOL(WMA);

public:
	void Clear();

	// This Open function is used if the source is to be streamed from the disk
    bool Open(std::string sFileName);
	bool Open(BYTE* pbData, uint32 dwDataSize);
    bool Close();

    bool Read(uint8* pBuffer, uint32 dwSizeToRead, uint32* pdwSizeRead );

    uint32			GetSize();
    bool			Reset();
    WAVEFORMATEX*	GetFormat();
	bool			IsEOF();

	void			Destroy();

	friend class WMACallback;

protected:
	virtual ~WMA();
private:
	WMA();

	bool PreOpen();
	bool PostOpen();

	bool OpenReader();

	HRESULT OnStatus(
		WMT_STATUS  Status,
		HRESULT  hr,
		WMT_ATTR_DATATYPE  dwType,
		BYTE*  pValue);

	HRESULT OnSample(
		DWORD  dwOutputNum,
		QWORD  cnsSampleTime,
		QWORD  cnsSampleDuration,
		DWORD  dwFlags,
		INSSBuffer*  pSample);

    HRESULT STDMETHODCALLTYPE OnTime( 
        QWORD qwCurrentTime);
private:
	
	IWMReader*			m_pReader;
	IWMReaderAdvanced*	m_pReaderAdvanced;
	IWMReaderAdvanced2*	m_pReaderAdvanced2;
	IWMHeaderInfo*		m_pHeaderInfo;
	WAVEFORMATEX		m_WaveFormatEx;
	WMACallback			m_Callback;

	HANDLE				m_hWaitEvent;
    HANDLE				m_hRespondEvent;

	bool				m_bOpen;

	HRESULT				m_hrCallbackResult;

	uint32				m_nStreamSize;
	uint32				m_nBytesRead;

	bool				m_bFirstRead;
	uint8*				m_pBuffer;
	uint32				m_nBufferSize;
	uint32				m_nWritePtr;
	uint32				m_nTargetPtr;
	bool				m_bEOF;
	uint64				m_qwTime;

	CRITICAL_SECTION	m_csTerm;

};

}; // namespace Audio

#endif // USE_WMA


#endif // __WMA_H__