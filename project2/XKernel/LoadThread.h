// LoadThread.h: interface for the LoadThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADTHREAD_H__E8653D54_B072_498C_88B3_7388CCF418DC__INCLUDED_)
#define AFX_LOADTHREAD_H__E8653D54_B072_498C_88B3_7388CCF418DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#include <queue>


class ListLock
{
public:
	ListLock(LPCRITICAL_SECTION lpCriticalSection)
	{
		m_pCriticalSection = lpCriticalSection;
		EnterCriticalSection(m_pCriticalSection);  
	}
	~ListLock()
	{
		LeaveCriticalSection(m_pCriticalSection);
	}
private:
	LPCRITICAL_SECTION m_pCriticalSection;
};

#define CRITICAL_LOCK(lpCriticalSection) ListLock _ListLock(lpCriticalSection)


class LoadTextureThread  
{
public:
	struct QUEUE_DATA
	{
		_XTextureManager*	pManager;
		_XTexture*			pTexture;		
	};

public:
	HANDLE				m_hActive;
	CRITICAL_SECTION	m_csListUpdate;
	CRITICAL_SECTION	m_csTextureLoad;

	BOOL				m_bEnableThread;

	BOOL				m_bEnable;

	typedef std::queue<QUEUE_DATA>		sqdef_XTexture;

	sqdef_XTexture			m_sqXTexture;
public:
	LoadTextureThread();
	virtual ~LoadTextureThread();

	BOOL	CreateThread();
	BOOL	DeleteThread();

	BOOL	GetTexture(QUEUE_DATA& queue_data);
	BOOL	PopTexture();

	void	LoadTexture(_XTextureManager* pTextureManager, _XTexture* pTexture);
};

extern LoadTextureThread g_LoadTextureThread;

class _XAnimMeshAbstractClass;

class LoadMeshThread  
{
public:
	struct MODELQUEUE_DATA
	{
		_XAnimMeshAbstractClass* pModelContainer;
		_XTextureManager*		 pTextureManager;
		BOOL					 usesecondbuffer;
	};

public:
	HANDLE				m_hActive;
	CRITICAL_SECTION	m_csListUpdate;
	CRITICAL_SECTION	m_csMeshLoad;

	BOOL				m_bEnableThread;

	BOOL				m_bEnable;

	_XPackageArchive*	m_pPackageArchive;

	typedef std::queue<MODELQUEUE_DATA>		sqdef_MeshContainer;

	sqdef_MeshContainer			m_sqMeshContainer;
public:
	LoadMeshThread();
	virtual ~LoadMeshThread();

	BOOL	CreateThread();
	BOOL	DeleteThread();

	BOOL	GetMeshContainer(MODELQUEUE_DATA& queue_data);
	BOOL	PopMeshContainer();

	BOOL	LoadMeshContainer(_XAnimMeshAbstractClass* pModelContainer, _XTextureManager* pTextureManager, BOOL usesecondbuffer );
	BOOL	LoadMeshContainer_BlockMode(_XAnimMeshAbstractClass* pModelContainer, _XTextureManager* pTextureManager, BOOL usesecondbuffer );

	void	SetModelArchieve( _XPackageArchive* pPackageArchive )
	{
			m_pPackageArchive = pPackageArchive;
	}
};

extern LoadMeshThread g_LoadMeshThread;

#endif // !defined(AFX_LOADTHREAD_H__E8653D54_B072_498C_88B3_7388CCF418DC__INCLUDED_)
