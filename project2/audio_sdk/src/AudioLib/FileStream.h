#ifndef FILESTREAM_H__
#define FILESTREAM_H__

#include "Audio.h"

#pragma pack(push,4)

namespace Audio
{

class FileStream : public IAudioStream
{

DEFINE_POOL(FileStream);

public:
	FileStream();
	virtual ~FileStream();
	void Clear();

	HRESULT __stdcall Open(std::string sFileName);
	void Close();

	// IUnknown Interface
	HRESULT __stdcall QueryInterface(
		const struct _GUID& guid, 
		void** ppInterface);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	// ISequentialStream Interface
	HRESULT __stdcall Read( 
		/* [length_is][size_is][out] */ void *pv,
		/* [in] */ ULONG cb,
		/* [out] */ ULONG *pcbRead);
    
	HRESULT __stdcall Write( 
		/* [size_is][in] */ const void *pv,
		/* [in] */ ULONG cb,
		/* [out] */ ULONG *pcbWritten);

	// IStream Interface
	HRESULT __stdcall Seek( 
		/* [in] */ LARGE_INTEGER dlibMove,
		/* [in] */ DWORD dwOrigin,
		/* [out] */ ULARGE_INTEGER *plibNewPosition);
        
	HRESULT __stdcall SetSize( 
		/* [in] */ ULARGE_INTEGER libNewSize);
        
	HRESULT __stdcall CopyTo( 
		/* [unique][in] */ IStream *pstm,
		/* [in] */ ULARGE_INTEGER cb,
		/* [out] */ ULARGE_INTEGER *pcbRead,
		/* [out] */ ULARGE_INTEGER *pcbWritten);
        
	HRESULT __stdcall Commit( 
		/* [in] */ DWORD grfCommitFlags);
        
	HRESULT __stdcall Revert(void);
        
	HRESULT __stdcall LockRegion( 
		/* [in] */ ULARGE_INTEGER libOffset,
		/* [in] */ ULARGE_INTEGER cb,
		/* [in] */ DWORD dwLockType);
        
	HRESULT __stdcall UnlockRegion( 
		/* [in] */ ULARGE_INTEGER libOffset,
		/* [in] */ ULARGE_INTEGER cb,
		/* [in] */ DWORD dwLockType);
        
	HRESULT __stdcall Stat( 
		/* [out] */ STATSTG *pstatstg,
		/* [in] */ DWORD grfStatFlag);
        
	HRESULT __stdcall Clone( 
		/* [out] */ IStream **ppstm);
        
private:

	int32			m_iRefCount;
	FILE*			m_pFile;
	uint32			m_nSize;
	std::string		m_sFileName;

	uint8*			m_pBuffer;
	uint32			m_nReadPtr;
	bool			m_bBuffered;
};

}; // namespace Audio

#pragma pack(pop,4)

#endif // FILESTREAM_H__