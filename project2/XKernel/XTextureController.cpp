// XTextureManager.cpp: implementation of the _XTextureController class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)

#include "XTextureController.h"

#include "XKernel.h"
#include "LoadThread.h"

#include "crtdbg.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern BOOL	g_SetTextureFailed;

_XTextureController::_XTextureController()
{
	m_bTextureCache	= TRUE;
	m_bUseThread	= FALSE;

	m_nNumTexture = 0;
	m_svTexture.reserve(50);
	m_svRenderStyleItem.reserve(50);

	SetFormat(D3DFMT_UNKNOWN);
}

_XTextureController::~_XTextureController()
{

}

void _XTextureController::DisposeTexture()
{
// #ifdef _XDWDEBUG
// 	_XDWINPRINT( "Cleanup texture list [PN:%s][count : %d]",m_strArchiveName.c_str(),m_svTexture.size() );
// #endif

	m_nNumTexture = 0;	
	std::for_each(m_svTexture.begin(), m_svTexture.end(), DELETEOBJECT());
	m_svTexture.clear();

	// m_svRenderStyleItem는 m_RenderStyleManager 에서 메모리를 가져 왔기 때문에 
	// m_RenderStyleManager 에서 지워야 하지 여기서 지우면 에러난다.
	// 절때 하지 마라ㅏ
//	std::for_each(m_svRenderStyleItem.begin(), m_svRenderStyleItem.end(), DELETEOBJECT());
	m_svRenderStyleItem.clear();


	m_RenderStyleManager.Dispose();

	m_TextureArchive.FinalizePackage();
}

BOOL _XTextureController::SetTexturePackage(LPCTSTR lpName)
{
	_ASSERT(lpName);
	m_strArchiveName = lpName;
	return TRUE;
}

int	_XTextureController::CreateTexture(LPCTSTR lpName, int miplevel, BOOL noresize)
{
	_ASSERT(lpName);

	if( strlen( lpName ) <= 0 ) return -1;

	int index = FindTextureName(lpName);
	if (index >= 0) return index;

	_XTexture* pTextire = new _XTexture;
	pTextire->SetFileName(const_cast<char*>(lpName));
	pTextire->m_noresize = noresize ? true : false;
	pTextire->m_miplevel = static_cast<byte>(miplevel);

	m_svTexture.push_back(pTextire);
	m_svRenderStyleItem.push_back(NULL);

	m_nNumTexture = m_svTexture.size();

	return m_nNumTexture - 1;
}

int _XTextureController::FindTextureName(LPCTSTR lpName)
{
	if (lpName == NULL) return -1;
	int index = 0;
	for (svdef_XTexture::iterator it = m_svTexture.begin(); it != m_svTexture.end(); ++ index, ++ it)
	{
		if(stricmp((*it)->GetFileName(), lpName) == 0)
		{
			return index;
		}
	}
	return -1;
}

BOOL _XTextureController::LoadRenderStyle(LPCTSTR lpName)
{
	_ASSERT(lpName);
	_ASSERT(m_svRenderStyleItem.empty());
	_ASSERT(!m_RenderStyleManager.m_RenderStyleCount);

	_XSetDefaultPath( _XDEF_DEFAULTPATH_TEXTURE );
	m_RenderStyleManager.Load(const_cast<LPTSTR>(lpName));

	if( !m_TextureArchive.OpenPackage( m_strArchiveName.c_str() ) )
	{
		return FALSE;
	}

	for(int i = 0; i < m_RenderStyleManager.m_RenderStyleCount; ++i)
	{
		m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[0] = CreateTexture( m_RenderStyleManager.m_RenderStyleList[i].m_rs.texturename[0] );
		m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[1] = CreateTexture( m_RenderStyleManager.m_RenderStyleList[i].m_rs.texturename[1] );
		m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[2] = CreateTexture( m_RenderStyleManager.m_RenderStyleList[i].m_rs.texturename[2] );
		m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[3] = CreateTexture( m_RenderStyleManager.m_RenderStyleList[i].m_rs.texturename[3] );
		if( m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[0] != -1 )
		{
			m_svRenderStyleItem[m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[0]] = &m_RenderStyleManager.m_RenderStyleList[i];
		}
		if( m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[1] != -1 )
		{
			LoadTexture( m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[1] );
		}
		if( m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[2] != -1 )
		{
			LoadTexture( m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[2] );
		}
		if( m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[3] != -1 )
		{
			LoadTexture( m_RenderStyleManager.m_RenderStyleList[i].m_textureindex[3] );
		}
	}	

	m_TextureArchive.ClosePackage();

	return TRUE;
}

void _XTextureController::ReleaseTextureData()
{
	for(svdef_XTexture::iterator it = m_svTexture.begin(); it != m_svTexture.end(); ++ it)
	{
		(*it)->ResetRef();
		(*it)->ReleaseTexture();
//		_ASSERT((*it)->GetRef() == 0);
	}	
}

void _XTextureController::CheckTextureUseHitRate()
{
	#define _XDEF_TEXTURECACHECHECKTIME		120000

	for(svdef_XTexture::iterator it = m_svTexture.begin(); it != m_svTexture.end(); ++ it)
	{
		if( (*it)->m_initialized )
		{
			if( (g_LocalSystemTime - (*it)->m_LastUsedTime) > _XDEF_TEXTURECACHECHECKTIME )
			{
				(*it)->ReleaseTexture();
			}
		}
	}
}

void _XTextureController::ResetTextureCacheTime()
{
	for(svdef_XTexture::iterator it = m_svTexture.begin(); it != m_svTexture.end(); ++ it)
	{
		(*it)->m_LastUsedTime = g_LocalSystemTime;
	}
}

BOOL _XTextureController::IsAlphaBlending( DWORD index )
{
	if(index < 0 || index >= m_nNumTexture)
	{
		return FALSE;
	}	
	
	if( m_svRenderStyleItem[index] )
	{
		if( m_svRenderStyleItem[index]->m_rs.blendmode == _XRS_NOBLEND ) return FALSE;
		else return TRUE;
	}
	else 
	{
		return FALSE;
	}
}

BOOL _XTextureController::LoadTexture(DWORD index)
{
	_XSetDefaultPath(_XDEF_DEFAULTPATH_TEXTURE);
	if( !m_TextureArchive.ReOpenPackage() )
	{
		return FALSE;
	}

	int resourceindex = m_TextureArchive.FindResource( m_svTexture[index]->m_strFileName.c_str() );
	if( resourceindex == -1 ) return FALSE;
	
	int   filesize = m_TextureArchive.GetPackedFileSize( resourceindex );

	if( m_TextureArchive.GetUseVirtualMemory() )
	{
		LPBYTE memarchiveptr = m_TextureArchive.GetPackedMemory( resourceindex );
		
		if( !m_svTexture[index]->ReLoad( memarchiveptr, filesize ) )
		{
			m_TextureArchive.ClosePackage();
			return FALSE;
		}
	}
	else
	{
		FILE* filearchiveptr = m_TextureArchive.GetPackedFile( resourceindex );

		if( !m_svTexture[index]->ReLoad( filearchiveptr, filesize ) )
		{			
			m_TextureArchive.ClosePackage();
			return FALSE;
		}
	}

	m_TextureArchive.ClosePackage();
	
	m_svRenderStyleItem[index] = m_RenderStyleManager.GetRenderStyle( m_RenderStyleManager.FindRenderStyle( m_svTexture[index]->m_strFileName.c_str() ) );

	_XDWINPRINT( "Texture Reloaded [%20s]", m_svTexture[index]->m_strFileName.c_str() );

	return TRUE;
}

BOOL _XTextureController::ReleaseTexture(DWORD index)
{
	m_svTexture[index]->ReleaseDeviceObjects();
	_XDWINPRINT( "Texture Deleted [%20s]", m_svTexture[index]->m_strFileName.c_str() );

	return TRUE;
}

int _XTextureController::GetRef(DWORD index)
{
	if(index < 0 || index >= m_nNumTexture)
	{
		return -1;
	}
	return m_svTexture[index]->GetRef();
}


void _XTextureController::AddRef(DWORD index)
{
	if(index < 0 || index >= m_nNumTexture)
	{
		return;
	}
	m_svTexture[index]->AddRef();

	if (m_svTexture[index]->GetRef() == 1)
	{
		LoadTexture(index);
		//OutputDebugString(m_svTexture[index]->m_strFileName.c_str());
		//OutputDebugString(" : Texture Loaded \n");
	}
}

void _XTextureController::ReleaseRef(DWORD index)
{
	if(index < 0 || index >= m_nNumTexture)
	{
		return;
	}
	m_svTexture[index]->ReleaseRef();

	if (m_svTexture[index]->GetRef() == 0)
	{
		ReleaseTexture(index);

		//OutputDebugString(m_svTexture[index]->m_strFileName.c_str());
		//OutputDebugString(" : Texture Deleted \n");
	}
}

void _XTextureController::SetTexture( DWORD index, DWORD stage )
{
	if(index < 0 || index >= m_nNumTexture)
	{
		gpDev->SetTexture(stage, NULL);
		g_SetTextureFailed = TRUE;
		return;
	}	

	if(m_bTextureCache && !m_svTexture[index]->m_initialized)
	{
//		if (m_bUseThread == TRUE && g_LoadThread.m_bEnable == TRUE)
//		{
//			g_LoadThread.LoadTexture(this, m_svTexture[index]);
//		}
//		else
		{
			if (LoadTexture(index) == FALSE)
			{
				gpDev->SetTexture( stage, NULL );
				g_SetTextureFailed = TRUE;
				return;
			}
		}
	}	
		
#ifdef _XDWDEBUG
	if( !g_UseTexture )
	{
		gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		gpDev->SetTexture( stage, NULL );		
	}
	else
	{
#endif

	if( !m_svRenderStyleItem.empty() )
	{
		if( m_svRenderStyleItem[index] )
		{
			m_svRenderStyleItem[index]->SetRenderStyle( this );
		}
		else
		{
			m_RenderStyleManager.SetRenderStyle( NULL, 0 ); // 렌더스타일이 없는경우 0번째 렌더스타일 적용.
			gpDev->SetTexture( stage, m_svTexture[index]->m_tex );
		}
	}
	else 
	{
		gpDev->SetTexture( stage, m_svTexture[index]->m_tex );
		g_SetTextureFailed = !m_svTexture[index]->m_initialized;
	}
	
#ifdef _XDWDEBUG
	}
#endif

	m_svTexture[index]->m_LastUsedTime = g_LocalSystemTime;
}

void _XTextureController::SetTexture_NoRenderStyle( DWORD index, DWORD stage )
{	
	if( index < 0 || index >= m_nNumTexture )
	{
		gpDev->SetTexture( stage, NULL );
		g_SetTextureFailed = TRUE;
		return;
	}	
	
	if(m_bTextureCache && !m_svTexture[index]->m_initialized )
	{		
//		if (m_bUseThread == TRUE && g_LoadThread.m_bEnable == TRUE)
//		{
//			g_LoadThread.LoadTexture(this, m_svTexture[index]);
//		}
//		else
		{
			if (LoadTexture(index) == FALSE)
			{
				gpDev->SetTexture( stage, NULL );
				g_SetTextureFailed = TRUE;
				return;
			}
		}
	}	
	
#ifdef _XDWDEBUG
	if( !g_UseTexture )
	{
		gpDev->SetTexture( stage, NULL );
	}
	else
#endif
	{
		gpDev->SetTexture( stage, m_svTexture[index]->m_tex );
		g_SetTextureFailed = !m_svTexture[index]->m_initialized;
	}
		
	m_svTexture[index]->m_LastUsedTime = g_LocalSystemTime;
}

_XTexture* _XTextureController::GetTextureObject( DWORD index )
{
	if( index >= m_nNumTexture ) return NULL;
	return m_svTexture[index];
}

IDirect3DTexture9* _XTextureController::GetTexture( DWORD index )
{
	if( index >= m_nNumTexture ) return NULL;	
	return m_svTexture[index]->m_tex;
}


extern LPSTR getExtention( LPSTR String );

int	_XTextureController::FindResource( LPSTR resourcename )
{
	_ASSERT( resourcename );

	int i = 0;
	for(svdef_XTexture::iterator it = m_svTexture.begin(); it != m_svTexture.end(); ++ i, ++ it)
	{
		if( strcmpi((*it)->m_strFileName.c_str(), resourcename ) != 0 )
		{
			LPCTSTR pString = (*it)->m_strFileName.c_str();
			//if( pString[0] == resourcename[0] )
			{
				LPSTR pExtString = getExtention( const_cast<char*>(pString) );
				if( pExtString && ( 0 == strnicmp( pExtString, _T(".dds"), 4 ) ) )
				{
					if( 0 == strnicmp( resourcename, pString, (pExtString-pString) ) )
					{
						return i;
					}			
				}				
			}
		}
		else
		{
#ifdef _XDWDEBUG
//			_XDWINPRINT( "Found texture in package [%s]", resourcename );
#endif
			return i;
		}
	}
#ifdef _XDWDEBUG
			_XDWINPRINT( "Texture not found [%s]", resourcename );
#endif
	
	return -1;
}

BOOL _XTextureController::InitDeviceObject( void )
{	
	return TRUE;

	_XDWINPRINT("Initialize texture object===============================================" );
	
#ifdef _XDWDEBUG
	LARGE_INTEGER frequency, count1, count2;
	if (!QueryPerformanceFrequency(&frequency))	_XDWINPRINT( "QueryPerformanceFrequency : No performancefreq" );	
#endif

	int i = 0;
	for(svdef_XTexture::iterator it = m_svTexture.begin(); it != m_svTexture.end(); ++ i, ++ it)
	{

#ifdef _XDWDEBUG
		QueryPerformanceCounter(&count1);
#endif

		if(!(*it)->InitDeviceObjects(m_TextureFormat))
		{
			_XFatalError("_XTextureController::InitDeviceObjects() : Could not create texture.[%s][%d]",
				        (*it)->m_strFileName.c_str(), i);
			return FALSE;
		}

#ifdef _XDWDEBUG
		QueryPerformanceCounter(&count2);	 
		double timeinmicroseconds = (double)( (double)((count2.QuadPart - count1.QuadPart) * 1000 * 1000) / (double)frequency.QuadPart );
		_XDWINPRINT( "Texture initialized [%20s]/ %f second [%p]", (*it)->m_strFileName.c_str(),timeinmicroseconds/1000000, (*it)->m_tex );
#else
		_XDWINPRINT( "Texture initialized [%20s]", (*it)->m_strFileName.c_str() );
#endif 	

	}

	_XDWINPRINT("End of Initialize texture object========================================" );
	
	return TRUE;
}

void _XTextureController::ReleaseDeviceObject( void )
{
	for(svdef_XTexture::iterator it = m_svTexture.begin(); it != m_svTexture.end(); ++ it)
	{
		(*it)->ReleaseDeviceObjects();
	}
}

void _XTextureController::ReportTextureInfo( void )
{
	_XLog( "= [%s] Texture archive informations========", m_strArchiveName.c_str() );
	_XLog( "= Texture count : %d", m_svTexture.size() );

	for(svdef_XTexture::iterator it = m_svTexture.begin(); it != m_svTexture.end(); ++ it)
	{
		(*it)->ReportTextureInfo();
	}
}

BOOL _XTextureController::BuildTextureList( BOOL createalphachannel ) 
{
	_XDWINPRINT("Build texture list======================================================" );

	if( m_svTexture.empty() ) return TRUE;

	/*
	_XSetDefaultPath( _XDEF_DEFAULTPATH_TEXTURE );
	if( !m_TextureArchive.ReOpenPackage() )
	{
		return FALSE;
	}
	*/
/*
#ifdef _XDWDEBUG
	LARGE_INTEGER frequency, count1, count2;
	if (!QueryPerformanceFrequency(&frequency))	_XDWINPRINT( "QueryPerformanceFrequency : No performancefreq" );	
#endif

	bool b_createalphachannel = ( createalphachannel ? true : false );
	for(svdef_XTexture::iterator it = m_svTexture.begin(); it != m_svTexture.end(); ++ it)
	{
#ifdef _XDWDEBUG
		QueryPerformanceCounter(&count1);
#endif
		int resourceindex = m_TextureArchive.FindResource( const_cast<char*>((*it)->GetFileName()));
		if( resourceindex != -1 )
		{
			int   miplevel	= (*it)->m_miplevel;
			bool  noresize  = (*it)->m_noresize;
			if (CreateLoadTexture((*it), resourceindex, miplevel, noresize, (*it)->GetFileName(), b_createalphachannel) == FALSE)
			{
				m_TextureArchive.Close();
				return FALSE;
			}
		}
		else
		{
			_XFatalError( "텍스쳐 리소스를 찾을 수 없습니다.[%s : %s]", m_strArchiveName.c_str(), (*it)->GetFileName() );
			m_TextureArchive.Close();
			return FALSE;
		}						
		
#ifdef _XDWDEBUG
		QueryPerformanceCounter(&count2);	 
		double timeinmicroseconds = (double)( (double)((count2.QuadPart - count1.QuadPart) * 1000 * 1000) / (double)frequency.QuadPart );
		_XDWINPRINT( "Texture ready [%20s]/ %f second", (*it)->GetFileName(),timeinmicroseconds/1000000 );
#else
		_XDWINPRINT( "Texture ready [%20s]", (*it)->GetFileName());
#endif 	

	}
*/
	/*
	m_TextureArchive.Close( FALSE );
	*/
	
	_XDWINPRINT("End of build texture list===============================================" );

	return TRUE;
}

BOOL _XTextureController::CreateLoadTexture(_XTexture* pTexture, int resourceindex, int miplevel, bool noresize, LPCTSTR lpName, bool b_createalphachannel) 
{
	int filesize = m_TextureArchive.GetPackedFileSize( resourceindex );

	if( m_TextureArchive.GetUseVirtualMemory() )
	{
		LPBYTE memarchiveptr = m_TextureArchive.GetPackedMemory( resourceindex );

		if( !memarchiveptr )
		{
			_XFatalError( "TextureController::Could not find file pointer.[%s : %s]", m_strArchiveName.c_str(), lpName );			
			return FALSE;
		}
		
		if( !pTexture->Load(memarchiveptr, filesize, miplevel, noresize, b_createalphachannel ) )
		{
			_XFatalError( "TextureController::Could not read texture.[%s : %s]", m_strArchiveName.c_str(), lpName );					
			return FALSE;					
		}
	}
	else
	{
		FILE* filearchiveptr = m_TextureArchive.GetPackedFile( resourceindex );

		if( !filearchiveptr )
		{
			_XFatalError( "TextureController::Could not find file pointer.[%s : %s]", m_strArchiveName.c_str(), lpName );			
			return FALSE;
		}
									
		if( !pTexture->Load(filearchiveptr, filesize, miplevel, noresize, b_createalphachannel ) )
		{
			_XFatalError( "TextureController::Could not read texture.[%s : %s]", m_strArchiveName.c_str(), lpName );					
			return FALSE;					
		}
	}
	return TRUE;
}

extern BYTE g_XByteFilter[8];

BOOL _XTextureController::TestAlphaChannel( DWORD index, int x, int y )
{
	if( index >= m_svTexture.size() ) return FALSE;	

	_XTexture* pTexture = m_svTexture[index];	
	
	if( !pTexture->m_alphachannel ) return TRUE;	
	
	int alphaarraywidth = (pTexture->m_width>>3);
	if( pTexture->m_width - (alphaarraywidth<<3) )
	{
		alphaarraywidth++;
	}
	
	int		bitoffset = x - ((x>>3)<<3);
	BYTE	alphachannel = pTexture->m_alphachannel[ (x>>3) + y * alphaarraywidth ];
	return	(alphachannel & g_XByteFilter[bitoffset]);
}
