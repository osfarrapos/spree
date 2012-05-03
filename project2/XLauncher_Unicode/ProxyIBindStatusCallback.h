#pragma once

class _XBindStatusCallback {
public:
	virtual HRESULT XOnStartBinding(DWORD dwReserved, IBinding *pib)
	{
		return S_OK;
	}
	virtual HRESULT XGetPriority(LONG *pnPriority)
	{
		return S_OK;
	}
	virtual HRESULT XOnLowResource(DWORD reserved)
	{
		return S_OK;
	}
	virtual HRESULT XOnProgress(ULONG ulProgress, ULONG ulProgressMax,
		ULONG ulStatusCode, LPCWSTR szStatusText)
	{
		return S_OK;
	}
	virtual HRESULT XOnStopBinding(HRESULT hresult, LPCWSTR szError)
	{
		return S_OK;
	}
	virtual HRESULT XGetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo)
	{
		return S_OK;
	}
	virtual HRESULT XOnDataAvailable(DWORD grfBSCF, DWORD dwSize,
		FORMATETC *pformatetc, STGMEDIUM *pstgmed)
	{
		return S_OK;
	}
	virtual HRESULT XOnObjectAvailable(REFIID riid, IUnknown *punk)
	{
		return S_OK;
	}
};

class _XProxyIBindStatusCallback : public IBindStatusCallback {
public:
	_XProxyIBindStatusCallback(_XBindStatusCallback* handler)
		: m_ref(1), m_handler(handler)
	{}
	virtual ~_XProxyIBindStatusCallback()
	{
		ASSERT(m_ref == 1);
	}

private:
	long m_ref;
	_XBindStatusCallback* m_handler;

public:
	// IUnknown override
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void ** ppv)
	{
		if (riid == __uuidof(IUnknown)) {
			*ppv = static_cast<IUnknown*>( static_cast<IBindStatusCallback*>(this) );
		} else if (riid == __uuidof(IBindStatusCallback)) {
			*ppv = static_cast<IBindStatusCallback*>(this);
		} else {
			*ppv;
			return E_NOINTERFACE;
		}
		AddRef();
		return S_OK;
	}
	virtual ULONG __stdcall AddRef()
	{
		ASSERT(m_ref < 0x7fffffff);
		return ++m_ref;
	}
	virtual ULONG __stdcall Release()
	{
		ASSERT(m_ref > 0);
		if (--m_ref == 0) {
			delete this;
			return 0;
		}
		return m_ref;
	}
	// IBindStatusCallback interface
	virtual HRESULT __stdcall OnStartBinding(
		DWORD dwReserved,
		IBinding *pib)
	{
		return m_handler->XOnStartBinding(dwReserved, pib);
	}
	virtual HRESULT __stdcall GetPriority(
		LONG *pnPriority)
	{
		return m_handler->XGetPriority(pnPriority);
	}
	virtual HRESULT __stdcall OnLowResource(
		DWORD reserved)
	{
		return m_handler->XOnLowResource(reserved);
	}
	virtual HRESULT __stdcall OnProgress(
		ULONG ulProgress,
		ULONG ulProgressMax,
		ULONG ulStatusCode,
		LPCWSTR szStatusText)
	{
		return m_handler->XOnProgress(ulProgress, ulProgressMax,
			ulStatusCode, szStatusText);
	}
	virtual HRESULT __stdcall OnStopBinding(
		HRESULT hresult,
		LPCWSTR szError)
	{
		return m_handler->XOnStopBinding(hresult, szError);
	}
	virtual HRESULT __stdcall GetBindInfo(
		DWORD *grfBINDF,
		BINDINFO *pbindinfo)
	{
		return m_handler->XGetBindInfo(grfBINDF, pbindinfo);
	}
	virtual HRESULT __stdcall OnDataAvailable(
		DWORD grfBSCF,
		DWORD dwSize,
		FORMATETC *pformatetc,
		STGMEDIUM *pstgmed)
	{
		return m_handler->XOnDataAvailable(grfBSCF, dwSize,
			pformatetc, pstgmed);
	}
	virtual HRESULT __stdcall OnObjectAvailable(
		REFIID riid,
		IUnknown *punk)
	{
		return m_handler->XOnObjectAvailable(riid, punk);
	}
};
