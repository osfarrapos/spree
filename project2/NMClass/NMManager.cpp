#include "stdafx.h"
#include "XKernel.h"

#include "NMCONew.h"
#include "NMManager.h"

#include "NMGSGeneralSession.h"

#define DEFAULT_BUFFER_SIZE		( 1024 * 1024 )		// 1 Mega Bytes

#ifdef _NM_DYNAMIC_LOAD
	wapp::CDynamicFunctionT< BOOL ( __cdecl* )( _UInt32_, BYTE*, BYTE**, _UInt32_& ) >	s_pNMCO_CallNMFunc;
	wapp::CDynamicFunctionT< void ( __cdecl* )( LPVOID ) >								s_pNMCO_MemoryFree;
	wapp::CDynamicFunctionT< BOOL ( __cdecl* )( BOOL ) >								s_pNMCO_SetPatchOption;
	wapp::CDynamicFunctionT< BOOL ( __cdecl* )( BOOL ) >								s_pNMCO_SetUseFriendModuleOption;
	wapp::CDynamicFunctionT< BOOL ( __cdecl* )( LPCTSTR ) >								s_pNMCO_SetVersionFileUrl;
	wapp::CDynamicFunctionT< BOOL ( __cdecl* )( BOOL ) >								s_pNMCO_SetUseNGMOption;
	wapp::CDynamicFunctionT< BOOL ( __cdecl* )( UINT32, UINT32 ) >						s_pNMCO_SetLocale;
#endif

CNMManager::CNMManager(void)
{
	::ZeroMemory( &this->m_cs, sizeof( this->m_cs ) );
	::InitializeCriticalSection( &this->m_cs );

	this->m_pRetData	= new BYTE[ DEFAULT_BUFFER_SIZE ];
}

CNMManager::~CNMManager(void)
{
	::DeleteCriticalSection( &this->m_cs );

	if ( this->m_pRetData != NULL )
		delete []	this->m_pRetData;
}

void CNMManager::Init( void )
{
}

void CNMManager::Finalize( void )
{
}

BOOL CNMManager::CallNMFunc( CNMFunc* pFunc )
{
	BOOL	bRet = FALSE;

	if ( pFunc == NULL )
		return FALSE;

	NMCSLOCK( this->m_cs )
	{
		CNMSimpleStream		ssStream;
		_UInt32_				uRetDataLen		= DEFAULT_BUFFER_SIZE;
		BYTE*				pBuffer			= this->m_pRetData;

		pFunc->SetCalling();
		pFunc->Serialize( ssStream );

#ifdef	_NM_DYNAMIC_LOAD
		if (!s_pNMCO_CallNMFunc.IsValid())
		{
			s_pNMCO_CallNMFunc.Load( NMCONEW_DLL_NAME, "NMCO_CallNMFunc" );
			s_pNMCO_MemoryFree.Load( NMCONEW_DLL_NAME, "NMCO_MemoryFree" );
		}

		if (!s_pNMCO_CallNMFunc.IsValid() || !s_pNMCO_MemoryFree.IsValid())
		{
			return FALSE;
		}

		if ((*s_pNMCO_CallNMFunc)(pFunc->GetCode(), ssStream.GetBufferPtr(), &pBuffer, uRetDataLen) == TRUE)
#else
		if ( TRUE == ::NMCO_CallNMFunc( pFunc->GetCode(), ssStream.GetBufferPtr(), &pBuffer, uRetDataLen ) )
#endif
		{
			ssStream.SetBuffer( pBuffer, TRUE );

			pFunc->SetReturn();
			pFunc->DeSerialize( ssStream );
			bRet	= pFunc->bSuccess;

			if ( pBuffer != this->m_pRetData )
			{
#ifdef	_NM_DYNAMIC_LOAD
				(*s_pNMCO_MemoryFree)(pBuffer);
#else
				::NMCO_MemoryFree( pBuffer );
#endif
			}
		}
	}

	return bRet;
}


void CNMManager::MemFree( LPVOID pBuffer )
{
#ifdef	_NM_DYNAMIC_LOAD
	if (!s_pNMCO_MemoryFree.IsValid())
	{
		s_pNMCO_MemoryFree.Load(NMCONEW_DLL_NAME, "NMCO_MemoryFree");
	}
	if (s_pNMCO_MemoryFree.IsValid())
	{
		(*s_pNMCO_MemoryFree)(pBuffer);
	}
#else
	::NMCO_MemoryFree( pBuffer );
#endif
}

BOOL CNMManager::SetPatchOption( BOOL bPatch )
{
#if defined(_NM_DYNAMIC_LOAD)
	if (!s_pNMCO_SetPatchOption.IsValid())
	{
		s_pNMCO_SetPatchOption.Load(NMCONEW_DLL_NAME, "NMCO_SetPatchOption");
	}
	if (s_pNMCO_SetPatchOption.IsValid())
	{
		return (*s_pNMCO_SetPatchOption)(bPatch);
	}
	return FALSE;
#else
	return ::NMCO_SetPatchOption(bPatch);
#endif
}

BOOL CNMManager::SetUseFriendModuleOption( BOOL bUseFriendModule )
{
#if defined(_NM_DYNAMIC_LOAD)
	if (!s_pNMCO_SetUseFriendModuleOption.IsValid())
	{
		s_pNMCO_SetUseFriendModuleOption.Load(NMCONEW_DLL_NAME, "NMCO_SetUseFriendModuleOption");
	}
	if (s_pNMCO_SetUseFriendModuleOption.IsValid())
	{
		return (*s_pNMCO_SetUseFriendModuleOption)(bUseFriendModule);
	}
	return FALSE;
#else
	return ::NMCO_SetUseFriendModuleOption(bUseFriendModule);
#endif
}

BOOL CNMManager::SetVersionFileUrl( LPCTSTR pszVersionFileUrl )
{
#if defined(_NM_DYNAMIC_LOAD)
	if (!s_pNMCO_SetVersionFileUrl.IsValid())
	{
		s_pNMCO_SetVersionFileUrl.Load(NMCONEW_DLL_NAME, "NMCO_SetVersionFileUrl");
	}
	if (s_pNMCO_SetVersionFileUrl.IsValid())
	{
		return (*s_pNMCO_SetVersionFileUrl)(pszVersionFileUrl);
	}
	return FALSE;
#else
	return ::NMCO_SetVersionFileUrl(pszVersionFileUrl);
#endif
}

BOOL CNMManager::SetUseNGMOption( BOOL bUseNGM )
{
#if defined(_NM_DYNAMIC_LOAD)
	if (!s_pNMCO_SetUseNGMOption.IsValid())
	{
		s_pNMCO_SetUseNGMOption.Load(NMCONEW_DLL_NAME, "NMCO_SetUseNGMOption");
	}
	if (s_pNMCO_SetUseNGMOption.IsValid())
	{
		return (*s_pNMCO_SetUseNGMOption)(bUseNGM);
	}
	return FALSE;
#else
	return ::NMCO_SetUseNGMOption(bUseNGM);
#endif
}

BOOL CNMManager::SetLocale( NMLOCALEID uLocaleID, UINT32 uRegionCode )
{
#if defined(_NM_DYNAMIC_LOAD)
	if (!s_pNMCO_SetLocale.IsValid())
	{
		s_pNMCO_SetLocale.Load(NMCONEW_DLL_NAME, "NMCO_SetLocale");
	}
	if (s_pNMCO_SetLocale.IsValid())
	{
		return (*s_pNMCO_SetLocale)(uLocaleID, uRegionCode);
	}
	return FALSE;
#else
	return ::NMCO_SetLocale(uLocaleID, uRegionCode);
#endif
}
