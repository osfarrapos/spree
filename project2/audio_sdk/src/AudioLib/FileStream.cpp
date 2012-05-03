/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "FileStream.h"
#include "AudioMgr.h"

using namespace std;
using namespace Audio;



IMPLEMENT_POOL(FileStream);

//------------------------------------------------------------------------//
FileStream::FileStream()
{
	Clear();
}

//------------------------------------------------------------------------//
FileStream::~FileStream()
{
	assert(m_iRefCount == 0);
}

//------------------------------------------------------------------------//
void FileStream::Clear()
{
	m_iRefCount = 0;
	m_pFile = 0;
	m_nSize = 0;
	m_sFileName.erase();

	m_pBuffer = 0;
	m_nReadPtr = 0;
	m_bBuffered = false;
}


//------------------------------------------------------------------------//
HRESULT FileStream::Open(std::string sFileName)
{
	if(m_pFile)
		return E_FAIL;

	m_sFileName = sFileName;
	m_pFile = fopen(sFileName.c_str(), "rb");
	if(!m_pFile)
	{

		_getcwd(DXAudioMgr()->GetCurrentWorkingPath(), MAX_PATH);
		chdir(DXAudioMgr()->GetAudioSystemPath());
		m_pFile = fopen(sFileName.c_str(), "rb");
		chdir(DXAudioMgr()->GetCurrentWorkingPath());
		if(!m_pFile)
			return E_FAIL;
	}


	fseek(m_pFile, 0, SEEK_END);
	m_nSize = ftell(m_pFile);
	fseek(m_pFile, 0, SEEK_SET);

	// Cache the entire file in memory if it is relatively small
	// 44100 * 2 * 2 (2 second 16-bit buffer at 44.1KHz)
	if(m_nSize <= 176400 && DXAudioMgr()->GetInit()->m_bAutoStream)
		m_pBuffer = new uint8[m_nSize];

	return S_OK;
}


//------------------------------------------------------------------------//
void FileStream::Close()
{
	_XASAFE_DELETE_ARRAY(m_pBuffer);
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = 0;
	}
	Clear();
}



//------------------------------------------------------------------------//
HRESULT FileStream::QueryInterface(
	const struct _GUID& guid, 
	void** ppInterface)
{
	if (guid == IID_IStream)
		*ppInterface = (IStream*)this;
	else if (guid == IID_ISequentialStream)
		*ppInterface = (ISequentialStream*)this;
	else if (guid == IID_IUnknown)
		*ppInterface = (IUnknown*)this;
	else
		return E_NOTIMPL;
	((IUnknown*)*ppInterface)->AddRef();
	return S_OK;
}

//------------------------------------------------------------------------//
ULONG FileStream::AddRef()
{
	InterlockedIncrement(&m_iRefCount);
	return m_iRefCount;
}

//------------------------------------------------------------------------//
ULONG FileStream::Release()
{
	int32 iPrev = InterlockedDecrement(&m_iRefCount);
	if(iPrev == 0)
	{
		Close();
		FileStream::DestroyObject(this);
	}
	assert(m_iRefCount >= 0);
	return m_iRefCount;
}



// ISequentialStream Interface
//------------------------------------------------------------------------//
HRESULT FileStream::Read( 
	/* [length_is][size_is][out] */ void *pv,
	/* [in] */ ULONG cb,
	/* [out] */ ULONG *pcbRead)
{
	if(!m_pFile)
		return E_FAIL;

	uint32 nRead;
	if(m_pBuffer)
	{
		if(!m_bBuffered)
		{
			uint32 nFullRead = fread((uint8*)m_pBuffer, 1, m_nSize, m_pFile);
			if(nFullRead != m_nSize)
				return E_FAIL;
			m_bBuffered = true;
		}

		nRead = ClampMax(cb, m_nSize - m_nReadPtr);
		memcpy(pv, m_pBuffer + m_nReadPtr, nRead);
		m_nReadPtr += cb;
	}
	else
	{
		nRead = fread((uint8*)pv, 1, cb, m_pFile);
	}

	if(pcbRead)
		*pcbRead = nRead;

	return S_OK;
}

//------------------------------------------------------------------------//
HRESULT FileStream::Write( 
	/* [size_is][in] */ const void *pv,
	/* [in] */ ULONG cb,
	/* [out] */ ULONG *pcbWritten)
{
	return E_NOTIMPL;
}


// IStream Interface
//------------------------------------------------------------------------//
HRESULT FileStream::Seek( 
	/* [in] */ LARGE_INTEGER dlibMove,
	/* [in] */ DWORD dwOrigin,
	/* [out] */ ULARGE_INTEGER *plibNewPosition)
{

	if(!m_pFile)
		return E_FAIL;

	uint32 nSeek = (uint32)dlibMove.QuadPart;
	if(m_pBuffer)
	{
		switch(dwOrigin)
		{
			case SEEK_SET:
				m_nReadPtr = dlibMove.QuadPart;
				break;
			case SEEK_CUR:
				m_nReadPtr += dlibMove.QuadPart;
				break;
			case SEEK_END:
				m_nReadPtr = m_nSize - dlibMove.QuadPart;
				break;
		};
		if(plibNewPosition)
			plibNewPosition->QuadPart = m_nReadPtr;
	}
	else
	{
		if(fseek(m_pFile, nSeek, dwOrigin))
			return E_FAIL;
		if(plibNewPosition)
			plibNewPosition->QuadPart = ftell(m_pFile);
	}

	return S_OK;
}

    
//------------------------------------------------------------------------//
HRESULT FileStream::SetSize( 
	/* [in] */ ULARGE_INTEGER libNewSize)
{
	return E_NOTIMPL;
}

    
//------------------------------------------------------------------------//
HRESULT FileStream::CopyTo( 
	/* [unique][in] */ IStream *pstm,
	/* [in] */ ULARGE_INTEGER cb,
	/* [out] */ ULARGE_INTEGER *pcbRead,
	/* [out] */ ULARGE_INTEGER *pcbWritten)
{
	return E_NOTIMPL;
}

    
//------------------------------------------------------------------------//
HRESULT FileStream::Commit( 
	/* [in] */ DWORD grfCommitFlags)
{
	return E_NOTIMPL;
}

    
//------------------------------------------------------------------------//
HRESULT FileStream::Revert(void)
{
	return E_NOTIMPL;
}

    
//------------------------------------------------------------------------//
HRESULT FileStream::LockRegion( 
	/* [in] */ ULARGE_INTEGER libOffset,
	/* [in] */ ULARGE_INTEGER cb,
	/* [in] */ DWORD dwLockType)
{
	return E_NOTIMPL;
}

    
//------------------------------------------------------------------------//
HRESULT FileStream::UnlockRegion( 
	/* [in] */ ULARGE_INTEGER libOffset,
	/* [in] */ ULARGE_INTEGER cb,
	/* [in] */ DWORD dwLockType)
{
	return E_NOTIMPL;
}

    
//------------------------------------------------------------------------//
HRESULT FileStream::Stat( 
	/* [out] */ STATSTG *pstatstg,
	/* [in] */ DWORD grfStatFlag)
{
	if(!pstatstg)
		return E_INVALIDARG;

	pstatstg->pwcsName = 0;
	if(grfStatFlag == 1)
	{
		pstatstg->pwcsName = (WCHAR*)CoTaskMemAlloc(MAX_PATH * sizeof(WCHAR));
		ConvertAnsiStringToWide(pstatstg->pwcsName, m_sFileName.c_str());
	}
	else
		pstatstg->pwcsName = 0;
	pstatstg->type = STGTY_STREAM;
	pstatstg->cbSize.QuadPart = m_nSize;
	pstatstg->atime.dwHighDateTime = 0;
	pstatstg->atime.dwLowDateTime = 0;
	pstatstg->ctime.dwHighDateTime = 0;
	pstatstg->ctime.dwLowDateTime = 0;
	pstatstg->mtime.dwHighDateTime = 0;
	pstatstg->mtime.dwLowDateTime = 0;
	pstatstg->grfLocksSupported = 0;

	return S_OK;
}

    
//------------------------------------------------------------------------//
HRESULT FileStream::Clone( 
	/* [out] */ IStream **ppstm)
{
	IAudioStream* pStream;
	if(!AudioMgr()->CreateAudioStream(pStream))
		return E_FAIL;
	if(FAILED(pStream->Open(m_sFileName)))
	{
		pStream->Release();
		return E_FAIL;
	}
	LARGE_INTEGER liPos;
	if(m_pBuffer)
		liPos.QuadPart = m_nReadPtr;
	else
		liPos.QuadPart = ftell(m_pFile);
	pStream->Seek(liPos, SEEK_SET, 0);
	*ppstm = (IStream*)pStream;
	return S_OK;
}

