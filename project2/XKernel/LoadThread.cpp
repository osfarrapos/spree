// LoadThread.cpp: implementation of the LoadThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoadThread.h"
#include "XAnimMeshContainer.h"

#include <crtdbg.h>

#include <process.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LoadTextureThread g_LoadTextureThread;
LoadMeshThread g_LoadMeshThread;

//////////////////////////////////////////////////////////////////////////
// Thread main

//------------------------------------------------------------------------//
void LoadTextureThreadLoop(LPVOID lpv)
{
	OutputDebugString("LoadTextureThread Start !!\n");

#ifdef _XDEF_MODELTHREADLOADING	
	::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_NORMAL );
#endif

	LoadTextureThread::QUEUE_DATA queue_data;
	while(TRUE)
	{
		if (g_LoadTextureThread.m_bEnableThread == FALSE)
		{
			break;
		}
		if (g_LoadTextureThread.GetTexture(queue_data) == FALSE)
		{
			WaitForSingleObject(g_LoadTextureThread.m_hActive, INFINITE);
			continue;
		}
		if (g_LoadTextureThread.m_csTextureLoad.DebugInfo == NULL) continue;
		CRITICAL_LOCK(&g_LoadTextureThread.m_csTextureLoad);

		_ASSERT(queue_data.pManager);
		_ASSERT(queue_data.pTexture);
		_ASSERT(queue_data.pTexture->m_strFileName.length()>0);

		if( !queue_data.pManager->m_TextureArchive.ReOpenPackage_Thread() )
		{
			queue_data.pTexture->m_ThreadLoad = false;
			continue;
		}

		int resourceindex = queue_data.pManager->m_TextureArchive.FindResource( const_cast<LPSTR>(queue_data.pTexture->m_strFileName.c_str()) );
		if( resourceindex != -1 )
		{
			int   filesize = queue_data.pManager->m_TextureArchive.GetPackedFileSize( resourceindex );
					
			if( queue_data.pManager->m_TextureArchive.GetUseVirtualMemory() )
			{
				LPBYTE memarchiveptr = queue_data.pManager->m_TextureArchive.GetPackedMemory( resourceindex );
				
				if( !queue_data.pTexture->ThreadReLoad( memarchiveptr, filesize ) )
				{			
					queue_data.pTexture->m_ThreadLoad = false;
					queue_data.pManager->m_TextureArchive.ClosePackage();
					_ASSERT(!"error");
					continue;
				}
			}
			else
			{
				FILE* filearchiveptr = queue_data.pManager->m_TextureArchive.GetPackedFile( resourceindex );

				if( !queue_data.pTexture->ThreadReLoad( filearchiveptr, filesize ) )
				{			
					queue_data.pTexture->m_ThreadLoad = false;
					queue_data.pManager->m_TextureArchive.ClosePackage();
					_ASSERT(!"error");
					continue;
				}
			}
			queue_data.pTexture->m_initialized = true;		
			queue_data.pTexture->m_ThreadLoad = false;
		}
		else
		{
			queue_data.pTexture->m_ThreadLoad = false;
			OutputDebugString("Texture not found \n");
		}

		g_LoadTextureThread.PopTexture();
		queue_data.pManager->m_TextureArchive.ClosePackage();
	}

	OutputDebugString("LoadTextureThread End !!\n");
}

//////////////////////////////////////////////////////////////////////////
// LoadTextureThread

LoadTextureThread::LoadTextureThread()
{
	m_bEnableThread = FALSE;
	m_hActive = 0;

	ZeroMemory(&m_csListUpdate, sizeof(CRITICAL_SECTION));
	ZeroMemory(&m_csTextureLoad, sizeof(CRITICAL_SECTION));

	m_bEnable = FALSE;
}

LoadTextureThread::~LoadTextureThread()
{
}

BOOL LoadTextureThread::CreateThread()
{
	_ASSERT(m_bEnableThread == FALSE);

	InitializeCriticalSection(&m_csListUpdate);
	InitializeCriticalSection(&m_csTextureLoad);

	m_hActive = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_bEnableThread = TRUE;

	if(_beginthread(LoadTextureThreadLoop, 0, NULL) == -1)
	{
		m_bEnableThread = FALSE;
		return FALSE;
	}

	return TRUE;
}

BOOL LoadTextureThread::DeleteThread()
{
	if (m_bEnableThread == FALSE) return TRUE;
	m_bEnableThread = FALSE;

	if (m_csListUpdate.DebugInfo != NULL)
	{
		EnterCriticalSection(&m_csListUpdate);

		while(!m_sqXTexture.empty())
		{
			m_sqXTexture.pop();			
		}

		LeaveCriticalSection(&m_csListUpdate);
		DeleteCriticalSection(&m_csListUpdate);
		ZeroMemory(&m_csListUpdate, sizeof(CRITICAL_SECTION));
	}
	else
	{
		while(!m_sqXTexture.empty())
		{
			m_sqXTexture.pop();
		}
	}

	if(m_hActive)
	{
		SetEvent(m_hActive);
		CloseHandle(m_hActive);
		m_hActive = 0;
	}

	if (m_csTextureLoad.DebugInfo != NULL)
	{
		EnterCriticalSection(&m_csTextureLoad);
		LeaveCriticalSection(&m_csTextureLoad);
		DeleteCriticalSection(&m_csTextureLoad);
		ZeroMemory(&m_csTextureLoad, sizeof(CRITICAL_SECTION));
	}
	
	_ASSERT(m_sqXTexture.size()<=0);

	if(m_sqXTexture.size()>0)
	{
		TCHAR messagestr[64];
		sprintf( messagestr, _T("Invalid texture queue size %d\n"), m_sqXTexture.size() );
		OutputDebugString(messagestr);
	}

	return TRUE;
}

BOOL LoadTextureThread::GetTexture(QUEUE_DATA& queue_data)
{
	if (m_bEnableThread == FALSE) return FALSE;

	if (m_csListUpdate.DebugInfo == NULL)
	{
		return FALSE;
	}
	CRITICAL_LOCK(&m_csListUpdate);

	queue_data.pManager = NULL;
	queue_data.pTexture = NULL;
	if (m_sqXTexture.empty())
	{
		return FALSE;
	}

	queue_data = m_sqXTexture.front();
	return TRUE;
}

BOOL LoadTextureThread::PopTexture()
{
	if (m_bEnableThread == FALSE) return FALSE;

	if (m_csListUpdate.DebugInfo == NULL)
	{
		return FALSE;
	}

	if (m_sqXTexture.empty())
	{
		return FALSE;
	}

	CRITICAL_LOCK(&m_csListUpdate);
	m_sqXTexture.pop();
	return TRUE;
}

void LoadTextureThread::LoadTexture(_XTextureManager* pTextureManager, _XTexture* pTexture)
{
	_ASSERT(m_bEnableThread == TRUE);
	CRITICAL_LOCK(&m_csListUpdate);

	if (pTexture->m_ThreadLoad == true)
	{
		return;
	}
	
	if( !pTexture )
	{
		OutputDebugString("Invalid texture object\n");
		return;
	}

	_ASSERT(pTexture->m_strFileName.length() > 0);

	if( pTexture->m_strFileName.length() <= 0 )
	{
		OutputDebugString("Invalid texture file name\n");
		return;
	}

	pTexture->m_ThreadLoad = true;

	QUEUE_DATA queue_data;
	queue_data.pManager = pTextureManager;
	queue_data.pTexture = pTexture;

	m_sqXTexture.push(queue_data);

	SetEvent(m_hActive);
}


//////////////////////////////////////////////////////////////////////////
// Load Mesh thread

void LoadMeshThreadLoop(LPVOID lpv)
{
	OutputDebugString("LoadMeshThread Start !!\n");

	::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_NORMAL );

	LoadMeshThread::MODELQUEUE_DATA queue_data;
	while(TRUE)
	{
		if (g_LoadMeshThread.m_bEnableThread == FALSE)
		{
			break;
		}
		if (g_LoadMeshThread.GetMeshContainer(queue_data) == FALSE)
		{
			WaitForSingleObject(g_LoadMeshThread.m_hActive, INFINITE);
			continue;
		}
		if (g_LoadMeshThread.m_csMeshLoad.DebugInfo == NULL) continue;
		CRITICAL_LOCK(&g_LoadMeshThread.m_csMeshLoad);

		_ASSERT(queue_data.pModelContainer);
		_ASSERT(queue_data.pTextureManager);
		
		if( !g_LoadMeshThread.m_pPackageArchive )
		{
			OutputDebugString("Not initialized model archieve!!\n");
			break;
		}

		if( !g_LoadMeshThread.m_pPackageArchive->ReOpenPackage_Thread() )
		{
			queue_data.pModelContainer->m_ThreadLoad = false;
			continue;
		}

		FILE* pModelFile = g_LoadMeshThread.m_pPackageArchive->GetPackedFile( (LPSTR)queue_data.pModelContainer->m_strFileName.c_str() );

		if( pModelFile )
		{
			if( queue_data.pModelContainer->m_Initialized )
			{
				continue;
			}

			if( !queue_data.pModelContainer->Load_Runtime( pModelFile, queue_data.pTextureManager,
													       queue_data.usesecondbuffer, 
													      (LPSTR)queue_data.pModelContainer->m_strFileName.c_str() ) )
			{
				queue_data.pModelContainer->m_ThreadLoad = false;
				g_LoadMeshThread.m_pPackageArchive->ClosePackage();
				_ASSERT(!"error");
				continue;
			}

			int lodcount = 0;

			if( queue_data.pModelContainer->m_strLodFileName[0].length() > 0 ) lodcount++;
			if( queue_data.pModelContainer->m_strLodFileName[1].length() > 0 ) lodcount++;

			if( lodcount > 0 )
			{
				queue_data.pModelContainer->CreateLODObject( lodcount );
							
				for( int ilod = 0; ilod < lodcount; ++ilod )
				{
					queue_data.pModelContainer->m_pLODObjectList[ilod]->m_ThreadLoad = true;

					queue_data.pModelContainer->m_pLODObjectList[ilod]->m_strFileName = queue_data.pModelContainer->m_strLodFileName[ilod].c_str();

					pModelFile = g_LoadMeshThread.m_pPackageArchive->GetPackedFile( (LPSTR)queue_data.pModelContainer->m_strLodFileName[ilod].c_str() );

					if(pModelFile)
					{
						if( !queue_data.pModelContainer->m_pLODObjectList[ilod]->Load_Runtime( pModelFile, queue_data.pTextureManager,
																							   queue_data.usesecondbuffer, 
																							   (LPSTR)queue_data.pModelContainer->m_strLodFileName[ilod].c_str() ) )
						{
							queue_data.pModelContainer->m_ThreadLoad = false;
							g_LoadMeshThread.m_pPackageArchive->ClosePackage();
							_ASSERT(!"error");
							continue;
						}

						queue_data.pModelContainer->m_pLODObjectList[ilod]->m_ThreadLoad = false;
						queue_data.pModelContainer->m_pLODObjectList[ilod]->m_Initialized = true;
					}
					else
					{
						queue_data.pModelContainer->m_ThreadLoad = false;
						g_LoadMeshThread.m_pPackageArchive->ClosePackage();
						_ASSERT(!"error");
						continue;
					}
				}
			}

			queue_data.pModelContainer->m_ThreadLoad = false;
			queue_data.pModelContainer->m_Initialized = true;
		}
		else
		{
			queue_data.pModelContainer->m_ThreadLoad = false;
			OutputDebugString("Mesh not found \n");
		}

		g_LoadMeshThread.PopMeshContainer();
		
		g_LoadMeshThread.m_pPackageArchive->ClosePackage();
	}

	OutputDebugString("Load mesh thread : end\n");
}


//////////////////////////////////////////////////////////////////////////
// LoadMeshThread

LoadMeshThread::LoadMeshThread()
{
	m_bEnableThread = FALSE;
	m_hActive = 0;

	ZeroMemory(&m_csListUpdate, sizeof(CRITICAL_SECTION));
	ZeroMemory(&m_csMeshLoad, sizeof(CRITICAL_SECTION));

	m_bEnable = FALSE;
}

LoadMeshThread::~LoadMeshThread()
{
}

BOOL LoadMeshThread::CreateThread()
{
	_ASSERT(m_bEnableThread == FALSE);

	InitializeCriticalSection(&m_csListUpdate);
	InitializeCriticalSection(&m_csMeshLoad);

	m_hActive = CreateEvent(NULL, FALSE, FALSE, NULL);

	if( !m_pPackageArchive ) return FALSE;

	m_bEnableThread = TRUE;

	if(_beginthread(LoadMeshThreadLoop, 0, NULL) == -1)
	{
		m_bEnableThread = FALSE;
		return FALSE;
	}

	return TRUE;
}

BOOL LoadMeshThread::DeleteThread()
{
	if (m_bEnableThread == FALSE) return TRUE;
	m_bEnableThread = FALSE;

	if (m_csListUpdate.DebugInfo != NULL)
	{
		EnterCriticalSection(&m_csListUpdate);
		while(!m_sqMeshContainer.empty())
		{
			m_sqMeshContainer.pop();
		}
		LeaveCriticalSection(&m_csListUpdate);
		DeleteCriticalSection(&m_csListUpdate);
		ZeroMemory(&m_csListUpdate, sizeof(CRITICAL_SECTION));
	}
	else
	{
		while(!m_sqMeshContainer.empty())
		{
			m_sqMeshContainer.pop();
		}
	}

	if(m_hActive)
	{
		SetEvent(m_hActive);
		CloseHandle(m_hActive);
		m_hActive = 0;
	}

	if (m_csMeshLoad.DebugInfo != NULL)
	{
		EnterCriticalSection(&m_csMeshLoad);
		LeaveCriticalSection(&m_csMeshLoad);
		DeleteCriticalSection(&m_csMeshLoad);
		ZeroMemory(&m_csMeshLoad, sizeof(CRITICAL_SECTION));
	}

	_ASSERT(m_sqMeshContainer.size()<=0);

	if(m_sqMeshContainer.size()>0)
	{
		TCHAR messagestr[64];
		sprintf( messagestr, _T("Invalid mesh queue size %d\n"), m_sqMeshContainer.size() );
		OutputDebugString(messagestr);
	}

	return TRUE;
}

BOOL LoadMeshThread::GetMeshContainer(MODELQUEUE_DATA& queue_data)
{
	if (m_bEnableThread == FALSE) return TRUE;

	if (m_csListUpdate.DebugInfo == NULL)
	{
		return FALSE;
	}

	CRITICAL_LOCK(&m_csListUpdate);
	
	queue_data.pModelContainer = NULL;
	queue_data.pTextureManager = NULL;
	queue_data.usesecondbuffer = FALSE;
	
	if (m_sqMeshContainer.empty())
	{
		return FALSE;
	}

	queue_data = m_sqMeshContainer.front();
	return TRUE;
}

BOOL LoadMeshThread::PopMeshContainer()
{
	if (m_bEnableThread == FALSE) return TRUE;

	if (m_csListUpdate.DebugInfo == NULL)
	{
		return FALSE;
	}

	if (m_sqMeshContainer.empty())
	{
		return FALSE;
	}

	CRITICAL_LOCK(&m_csListUpdate);
	m_sqMeshContainer.pop();
	return TRUE;
}

BOOL LoadMeshThread::LoadMeshContainer_BlockMode(_XAnimMeshAbstractClass* pModelContainer, _XTextureManager* pTextureManager, BOOL usesecondbuffer )
{
	if( pModelContainer->m_strFileName.length() <= 0 ) // skip empty model stack
		return FALSE;

	_XPackageArchive ModelPackageArchive;
	
	if( !ModelPackageArchive.OpenPackage( m_pPackageArchive->GetArchieveName() ) )
	{
		return FALSE;
	}

	FILE* pModelFile = ModelPackageArchive.GetPackedFile( (LPSTR)pModelContainer->m_strFileName.c_str() );

	if( pModelFile )
	{
		if( !pModelContainer->Load_Runtime( pModelFile, pTextureManager,
													   usesecondbuffer, 
													  (LPSTR)pModelContainer->m_strFileName.c_str() ) )
		{
			ModelPackageArchive.FinalizePackage();
			return FALSE;
		}

		int lodcount = 0;

		if( pModelContainer->m_strLodFileName[0].length() > 0 ) lodcount++;
		if( pModelContainer->m_strLodFileName[1].length() > 0 ) lodcount++;

		if( lodcount > 0 )
		{
			pModelContainer->CreateLODObject( lodcount );
		
			for( int ilod = 0; ilod < lodcount; ++ilod )
			{
				pModelContainer->m_pLODObjectList[ilod]->m_strFileName = pModelContainer->m_strLodFileName[ilod].c_str();

				pModelFile = ModelPackageArchive.GetPackedFile( (LPSTR)pModelContainer->m_strLodFileName[ilod].c_str() );

				if(pModelFile)
				{
					if( !pModelContainer->m_pLODObjectList[ilod]->Load_Runtime( pModelFile, pTextureManager,
																							usesecondbuffer, 
																						   (LPSTR)pModelContainer->m_strLodFileName[ilod].c_str() ) )
					{
						ModelPackageArchive.FinalizePackage();
						return FALSE;
					}
					
					pModelContainer->m_pLODObjectList[ilod]->m_Initialized = true;
				}
				else
				{
					ModelPackageArchive.FinalizePackage();
					return FALSE;
				}
			}
		}
	}
	else
	{
		ModelPackageArchive.FinalizePackage();
		return FALSE;
	}

	pModelContainer->m_Initialized = true;

	ModelPackageArchive.FinalizePackage();
	return TRUE;
}

BOOL LoadMeshThread::LoadMeshContainer(_XAnimMeshAbstractClass* pModelContainer, _XTextureManager* pTextureManager, BOOL usesecondbuffer )
{
	if( !m_bEnableThread )
	{
		if( pModelContainer->m_strFileName.length() <= 0 ) // skip empty model stack
			return FALSE;

		if( !g_LoadMeshThread.m_pPackageArchive )
		{
			OutputDebugString("Not initialized model archieve!!\n");
			return FALSE;
		}

		if( !m_pPackageArchive->ReOpenPackage_Thread() )
		{
			return FALSE;
		}

		FILE* pModelFile = m_pPackageArchive->GetPackedFile( (LPSTR)pModelContainer->m_strFileName.c_str() );

		if( pModelFile )
		{
			if( !pModelContainer->Load_Runtime( pModelFile, pTextureManager,
													       usesecondbuffer, 
													      (LPSTR)pModelContainer->m_strFileName.c_str() ) )
			{
				m_pPackageArchive->ClosePackage();
				return FALSE;
			}

			int lodcount = 0;

			if( pModelContainer->m_strLodFileName[0].length() > 0 ) lodcount++;
			if( pModelContainer->m_strLodFileName[1].length() > 0 ) lodcount++;

			if( lodcount > 0 )
			{
				pModelContainer->CreateLODObject( lodcount );
							
				for( int ilod = 0; ilod < lodcount; ++ilod )
				{
					pModelContainer->m_pLODObjectList[ilod]->m_strFileName = pModelContainer->m_strLodFileName[ilod].c_str();

					pModelFile = m_pPackageArchive->GetPackedFile( (LPSTR)pModelContainer->m_strLodFileName[ilod].c_str() );

					if(pModelFile)
					{
						if( !pModelContainer->m_pLODObjectList[ilod]->Load_Runtime( pModelFile, pTextureManager,
																							    usesecondbuffer, 
																							   (LPSTR)pModelContainer->m_strLodFileName[ilod].c_str() ) )
						{
							m_pPackageArchive->ClosePackage();
							return FALSE;
						}

						pModelContainer->m_pLODObjectList[ilod]->m_Initialized = true;
					}
					else
					{
						m_pPackageArchive->ClosePackage();
						return FALSE;
					}
				}
			}

		}
		else
		{
			m_pPackageArchive->ClosePackage();
			return FALSE;
		}

		pModelContainer->m_Initialized = true;

		m_pPackageArchive->ClosePackage();
		return TRUE;
	}

	if( pModelContainer->m_strFileName.length() <= 0 ) // skip empty model stack
		return TRUE;

	CRITICAL_LOCK(&m_csListUpdate);

	if (pModelContainer->m_ThreadLoad == true)
	{
		return TRUE;
	}

	pModelContainer->m_ThreadLoad = true;

	MODELQUEUE_DATA queue_data;	
	queue_data.pModelContainer = pModelContainer;
	queue_data.pTextureManager = pTextureManager;
	queue_data.usesecondbuffer = usesecondbuffer;

	m_sqMeshContainer.push(queue_data);

	SetEvent(m_hActive);

	return TRUE;
}