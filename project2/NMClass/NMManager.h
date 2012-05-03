#ifdef _XDEF_NEXONLOGIN

#ifndef __NMMANAGER_H_DE71CC54_1039_4148_9CFE_29651ACA8374__
#define __NMMANAGER_H_DE71CC54_1039_4148_9CFE_29651ACA8374__

//
//	nmcogame.dll 링크 방식
//
#if defined(_NM_DYNAMIC_LOAD)
	//
	//	nmcogame.dll 런타임 로드
	//
	#define NMCONEW_DLL_NAME "nmcogame.dll"
#else
	//
	//	nmcogame.dll 정적 링크
	//
	#pragma comment( lib, "NMClass/nmcogame" )
#endif

class CNMManager
{
	DECLARE_NMSINGLETON( CNMManager )

protected:
	BYTE*				m_pRetData;
	CRITICAL_SECTION	m_cs;

	CNMManager( void );
public:
	~CNMManager( void );

public:
	void				Init( void );
	void				Finalize( void );

	BOOL				CallNMFunc( CNMFunc* pFunc );
	void				MemFree( LPVOID pBuffer );
	BOOL				SetPatchOption( BOOL bPatch );
	BOOL				SetUseFriendModuleOption( BOOL bUseFriendModule );
	BOOL				SetVersionFileUrl( LPCTSTR bPatch );
	BOOL				SetUseNGMOption( BOOL bUseNGM );
	BOOL				SetLocale( NMLOCALEID uLocaleID, UINT32 uRegionCode );
};

namespace wapp
{
	class CDynamicFunctionBase
	{
	protected:
		HMODULE	m_hDll;
		void*	m_pProc;

		CDynamicFunctionBase( void ) : m_hDll( NULL ), m_pProc( NULL )	{}
		virtual ~CDynamicFunctionBase( void ) { this->Unload(); }

	public:
		BOOL	IsValid( void ) const	{ return ( this->m_pProc != NULL ); }
		void	Unload( void )			{ if ( this->m_hDll ) { ::FreeLibrary( this->m_hDll ); this->m_hDll = NULL; } this->m_pProc = NULL; }
		void	Load( LPCSTR szDllName, LPCSTR aszFnName )
		{
			this->Unload();
			this->m_hDll	= ::LoadLibraryA( szDllName );
			if ( this->m_hDll ) { this->m_pProc	= ( void* )::GetProcAddress( this->m_hDll, aszFnName ); if ( !this->m_pProc ) this->Unload(); }
		}

	};

	template< class T >
	class CDynamicFunctionT : public CDynamicFunctionBase
	{
	public:
		CDynamicFunctionT( void )									{}
		CDynamicFunctionT( LPCTSTR szDllName, LPCSTR aszFnName )	{ this->Load( szDllName, aszFnName ); }
		T operator *() const										{ return ( T )this->m_pProc; }
	};
};

#endif	//	#ifndef __NMMANAGER_H_DE71CC54_1039_4148_9CFE_29651ACA8374__

#endif