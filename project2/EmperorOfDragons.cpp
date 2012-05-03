// EmperorOfDragons.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "resource.h"
#include <winuser.h>

#ifdef _XDWDEBUG
#include <Richedit.h>
#endif

#include "XSR_STRINGHEADER.H"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EODDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_Debug.h"
#include "XMessageWindow.h"
#include "XCharacterState.h"
#include "XWindow_PartyMasterMiniBar.h"
#include "XWindow_MonsterStatus.h"
#include "XWindow_Messenger.h"
#include "XWindow_WorldMinimap.h"
#include "XWindow_QuestWindow.h"
#include "XWindow_ChattingDefault.h"
#include "XWindow_DefaultTooltip.h"
#include "XWindow_CreateCharacter.h"
#ifdef _XTS_ITEMMALLBROWSER	
#include "XWindow_CashMall.h"	
#endif
#ifdef _XDEF_WEBBOARD_20061211
#include "XWindow_WebBoard.h"	
#endif
#include "XProc_LoginServer.h"

#ifdef _XUSEFMOD
	#include "XFMODWrapper.h"
#else
	#include "XAudioSystem.h"
	using namespace Audio;
#endif

// win32 web browser api
#include "./Library/CWebPage/XWebPage.h"

#ifdef _XENGLISH
//#define _XENGLISH_SUPPORTSUBLANGUAGE
#endif

#include "FlashControlInterface.h"
extern CFlashControlInterface*						g_pTitleFlashObject;
extern LPDIRECT3DTEXTURE9							g_pTitleFlashOffScreenTexture;

extern _XFontManager_Baseclass*						g_XDefaultFont;

extern XMemoryPool<_XParticleList>					g_ParticleListPool;
extern XMemoryPool<_XParticleGroupItem>				g_ParticleGroupItemPool;
extern XMemoryPool<_XDecalObject>					g_ParticleDecalObjectPool;
extern XMemoryPool<_XParticleGroupInstanceManager>	g_ParticleGroupInstanceManagerPool;


_XMEMORYCHECKDATA	g_MemoryCheckData_0;
_XMEMORYCHECKDATA	g_MemoryCheckData_1;
_XMEMORYCHECKDATA	g_MemoryCheckData_2;
_XMEMORYCHECKDATA	g_MemoryCheckData_3;
_XMEMORYCHECKDATA	g_MemoryCheckData_4;
_XMEMORYCHECKDATA	g_MemoryCheckData_5;

LONG				g_ModuleFileSize = 0;

#ifdef _XTAIWANESE
	
#else
	#ifdef _XVIETNAMESE
		//#define _XENABLE_GAMEGUARD
		//#pragma message( "_XVIETNAMESE" )
	#else		
		#ifdef _ACCLAIM_VERSION
			#define _XENABLE_GAMEGUARD
			//#pragma message( "_ACCLAIM_VERSION" )
		#else
			#ifdef _PWC_VERSION				
				//#pragma message( "_PWC_VERSION" )
			#else
				#ifdef _XJAPANESE
					#define _XENABLE_GAMEGUARD
					#pragma message( "_JAPANESE_VERSION" )
				#else
					#ifdef _XRUSSIAN					
						#pragma message( "RUSSIAN_VERSION" )
					#else
						#define _XENABLE_GAMEGUARD
						#pragma message( "NEXON_VERSION" )
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

#ifdef _XDWDEBUG
	//#pragma message( "_XDWDEBUG" )
#endif

#ifdef _XADMINISTRATORMODE
	//#pragma message( "_XADMINISTRATORMODE" )
#endif

#ifdef _DEBUG
	//#pragma message( "_DEBUG" )
#endif

#ifdef _XUSEAPEXMODULE
	#undef _XENABLE_GAMEGUARD
#endif

#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT)
	#ifdef _XENABLE_HACKSHEILD
		#define _XUSE_HACKSHEILD		
	#else
		#ifdef _XENABLE_GAMEGUARD
			#define _XUSE_GAMEGUARD
			#pragma message( "_XUSE_GAMEGUARD" )
		#endif
	#endif
#endif

#ifdef _XUSE_HACKSHEILD
#include "HShield.h"
extern DWORD  g_dwMainThreadID;
#pragma comment(lib, "./Library/HShield.lib")
#endif

#ifdef _XUSE_GAMEGUARD

#include "NPGameLib.h"
#pragma message( "Include NPGameLib.h : EOD" )

LPTSTR g_nProtectModuleCheckMessage = _T("Could not find module : GameGuard");

#ifdef _XVIETNAMESE
	#pragma comment(lib, "./Library/NPGameLib_DE.lib")
#else
	#ifdef _XTAIWANESE
		#pragma comment(lib, "./Library/NPGameLib_SE.lib")
		//#pragma message( "Link : ./Library/NPGameLib_SE.lib" )
	#else
		#ifdef _ACCLAIM_VERSION
			#pragma comment(lib, "./Library/NPGameLib_SE.lib")
		#else
			#ifdef _PWC_VERSION
							
			#else
				#ifdef _XJAPANESE
					#pragma comment(lib, "./Library/NPGameLib_SE.lib")
				#else
					#ifdef _XRUSSIAN
					
					#else
						#pragma comment(lib, "./Library/NPGameLib_SE.lib")
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
struct MSG_GGAUTH g_GameGuardData;
#endif

#endif

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _XDEF_USESHADER		TRUE

#define _FILEDATAENCODEKEY		0xA5

extern void GetFileNameFromPathString( TCHAR *value, TCHAR *line );

#ifdef _XCHECKMEMORYUSAGE

#pragma comment(lib, "Psapi.lib")
#include "psapi.h"

long GetMemoryUsageInfo( void )
{
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;
    hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId() );
    if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
    {
        return (long)pmc.WorkingSetSize;
    }	
    CloseHandle( hProcess );
	return 0;
}

#endif

#include "XUnicodeConv.h"

HWND g_hLoadingWindow = NULL;

LRESULT	CALLBACK _XKernelLoadingWindowProc( HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam )
{
	/*
	switch (message)
	{
	case WM_ERASEBKGND :
		RedrawWindow( hDlg, NULL, NULL, RDW_UPDATENOW );
		return 0;
		break;
	}
	*/

	return(DefWindowProc(hDlg, message, wparam, lparam));
}

#include "XCrypto.h"

void XKernelMain(void)
{
/*
#ifdef _XVIETNAMESE
	extern BOOL _XCheckLoginServerIP( LPCTSTR szHostName );
	if( _XCheckLoginServerIP( _T("login.cuulong.com.vn") ) )
	{
		return;
	}
#endif
*/	
	g_hLoadingWindow = CreateDialog(NULL, MAKEINTRESOURCE(IDD_NOWLOADINGDIALOG), gHWnd, (DLGPROC)_XKernelLoadingWindowProc);
	
	RECT rect;
	GetWindowRect( g_hLoadingWindow, &rect );
	int winxpos = (GetSystemMetrics(SM_CXSCREEN)-(rect.right-rect.left))/2;
	int winypos = (GetSystemMetrics(SM_CYSCREEN)-(rect.bottom-rect.top))/3;
	
	int winwidth  = rect.right-rect.left;
	int winheight = rect.bottom-rect.top;	
	MoveWindow( g_hLoadingWindow, winxpos, winypos, winwidth, winheight, TRUE );

	ShowWindow(g_hLoadingWindow, SW_SHOW);
	//RedrawWindow( g_hLoadingWindow, NULL, NULL, RDW_UPDATENOW );
	UpdateWindow( g_hLoadingWindow );	
			
#ifdef _XVIETNAMESE
	g_LanguageType = _XLANGUAGE_TYPE_VIETNAMESE;
#else

	#ifdef _XTAIWANESE
		g_LanguageType = _XLANGUAGE_TYPE_TAIWANESE;
	#else
		#ifdef _XENGLISH
			g_LanguageType = _XLANGUAGE_TYPE_ENGLISH;
		#else
			#ifdef _XJAPANESE
				g_LanguageType = _XLANGUAGE_TYPE_JAPANESE;
			#else
				#ifdef _XRUSSIAN					
					g_LanguageType = _XLANGUAGE_TYPE_RUSSIAN;
				#else
					g_LanguageType = _XLANGUAGE_TYPE_KOREAN;
				#endif
			#endif
		#endif
	#endif

#endif

	if( (_XCmdLineOption("korean",0) != 0) )
	{
		g_LanguageType = _XLANGUAGE_TYPE_KOREAN;		
	}
	else if( (_XCmdLineOption("vietnamese",0) != 0) )
	{
		g_LanguageType = _XLANGUAGE_TYPE_VIETNAMESE;
	}
	else if( (_XCmdLineOption("japanese",0) != 0) )
	{
		g_LanguageType = _XLANGUAGE_TYPE_JAPANESE;
	}
	else if( (_XCmdLineOption("english",0) != 0) )
	{
		g_LanguageType = _XLANGUAGE_TYPE_ENGLISH;
	}
	else if( (_XCmdLineOption("taiwanese",0) != 0) )
	{
		g_LanguageType = _XLANGUAGE_TYPE_TAIWANESE;
	}
	else if( (_XCmdLineOption("chinese",0) != 0) )
	{
		g_LanguageType = _XLANGUAGE_TYPE_CHINESE;
	}
	else if( (_XCmdLineOption("russian",0) != 0) )
	{
		g_LanguageType = _XLANGUAGE_TYPE_RUSSIAN;
	}
	

	//g_LanguageType = _XLANGUAGE_TYPE_KOREAN;

	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{
		strcpy( g_GUIFontName, _T("gulim") );
		strcpy( g_XFontName, _T("gulim12.xft") );
		//strcpy( g_XFontName, _T("Dotum12.xft") );
		strcpy( g_XLargeFontName, _T("GoongSeo18.xft") );
		//strcpy( g_XLargeFontName, _T("HY예서M18.xft") );
		//strcpy( g_XFontName, _T("SimSun12.fnt") );

		_XLog( "Set korean language pack, Font name [%s]", g_GUIFontName );				
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		strcpy( g_GUIFontName, _T("MingLiU") );
		strcpy( g_XFontName, _T("MingLiU12.xft") );			
		strcpy( g_XLargeFontName, _T("PMingLiU18.xft") );

		_XLog( "Set taiwanese language pack, Font name [%s]", g_GUIFontName );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		strcpy( g_GUIFontName, _T("Arial") );
		strcpy( g_XFontName, _T("Palatino16.xft") );
		strcpy( g_XLargeFontName, _T("Speedball22.xft") );

		_XLog( "Set english language pack, Font name [%s]", g_GUIFontName );
	}	
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		memset( g_XFontName, 0, sizeof(TCHAR) * 64 );
		memset( g_XLargeFontName, 0, sizeof(TCHAR) * 64 );

		strcpy( g_GUIFontName, _T("Arial Unicode MS") );
		strcpy( g_XFontName, _T("ArialUnicodeMS16.xft") );
		strcpy( g_XLargeFontName, _T("MicrosoftSansSerif24.xft") );
		
		_XLog( "Set vietnamese language pack, Font name [%s]", g_GUIFontName );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		strcpy( g_GUIFontName, _T("Arial") );		
		strcpy( g_XFontName, _T("MS Gothic16.xft") );
		strcpy( g_XLargeFontName, _T("HGPKyokashotai22.xft") );
		
		_XLog( "Set japanese language pack, Font name [%s]", g_GUIFontName );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_CHINESE )
	{
		strcpy( g_GUIFontName, _T("SimSun") );
		strcpy( g_XFontName, _T("SimSun12.xft") );
		//strcpy( g_XLargeFontName, _T("Haeseo22.xft") );
		strcpy( g_XLargeFontName, _T("GoongSeo18.xft") );

		_XLog( "Set chinese language pack, Font name [%s]", g_GUIFontName );		
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		strcpy( g_GUIFontName, _T("Arial") );
		strcpy( g_XFontName, _T("Palatino16.xft") );
		strcpy( g_XLargeFontName, _T("Speedball22.xft") );

		_XLog( "Set english language pack, Font name [%s]", g_GUIFontName );
	}
	else 
	{
		g_LanguageType = _XLANGUAGE_TYPE_UNKNOWN;		
		strcpy( g_GUIFontName, _T("Arial") );	
		strcpy( g_XFontName, _T("gulim12.xft") );
		//strcpy( g_XLargeFontName, _T("Haeseo22.xft") );
		strcpy( g_XLargeFontName, _T("GoongSeo18.xft") );

		_XLog( "unknown language, Set default language pack, Font name [%s]", g_GUIFontName );
	}	

	memset( g_MI_CharTextureName, 0, sizeof(TCHAR) * 32 );
	memset( g_MI_KiUpTextureName, 0, sizeof(TCHAR) * 32 );
	memset( g_MI_LifeSkillTextureName, 0, sizeof(TCHAR) * 32 );

	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		strcpy( g_MI_CharTextureName, _T("us_MI_char.tga") );
		strcpy( g_MI_KiUpTextureName, _T("us_mi_kiup.tga") );
		strcpy( g_MI_LifeSkillTextureName, _T("us_MI_LifeSkill.tga") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		strcpy( g_MI_CharTextureName, _T("rs_MI_char.tga") );
		strcpy( g_MI_KiUpTextureName, _T("rs_mi_kiup.tga") );
		strcpy( g_MI_LifeSkillTextureName, _T("rs_MI_LifeSkill.tga") );
	}	
	else
	{
		strcpy( g_MI_CharTextureName, _T("MI_char.tga") );
		strcpy( g_MI_KiUpTextureName, _T("mi_kiup.tga") );
		strcpy( g_MI_LifeSkillTextureName, _T("MI_LifeSkill.tga") );
	}

	TCHAR OsLocaldata[64];
	memset(OsLocaldata, 0, 64);
	GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_ICOUNTRY , OsLocaldata, 63);
	_XLog( "Code page : %d, Local : %s", GetACP(), OsLocaldata );
	
	// Fast sqrt LUT
	_XLog( "Build sqrt LUT" );
	_XFC_BuildSqrtTable();
	
	_XLog( "Load text resource" );
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

#ifdef _XTESTSERVER		
	TCHAR textstringdbfilename[10][32] = 
	{
		_T("tsNDTextDB_KR.xsd"), _T("tsNDTextDB_VN.xsd"), _T("tsNDTextDB_TW.xsd"), _T("tsNDTextDB_US.xsd"), 
		_T("tsNDTextDB_CN.xsd"), _T("tsNDTextDB_JP.xsd"), _T("tsNDTextDB_RS.xsd"), _T("tsNDTextDB_TH.xsd")
	};

	if( !g_StringDataBase.Load( textstringdbfilename[g_LanguageType], g_LanguageType ) )
	{
		::ShowWindow( g_hLoadingWindow, SW_HIDE );
		::DestroyWindow(g_hLoadingWindow);
		g_hLoadingWindow = NULL;
		return;
	}	
#else
	
#ifdef _XDEF_NEWSTRINGDBFORMAT
	TCHAR textstringdbfilename[10][32] = 
	{
		_T("NDTextDB_KR.xsd"), _T("NDTextDB_VN.xsd"), _T("NDTextDB_TW.xsd"), _T("NDTextDB_US.xsd"), 
		_T("NDTextDB_CN.xsd"), _T("NDTextDB_JP.xsd"), _T("NDTextDB_RS.xsd"), _T("NDTextDB_TH.xsd")
	};

#ifdef _XENGLISH
	TCHAR szFilename[256];
	memset( szFilename, 0, sizeof(TCHAR) * 256 );
#ifdef _XENGLISH_SUPPORTSUBLANGUAGE

	if( !CEmperorOfDragonsApp::LoadSubLanguageList() )
	{
		return;
	}

	g_SubLanguage = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_SUBLANGUAGE, 0, TRUE );	
	_XStringItem* pLanguageText = (_XStringItem*)g_SubLanguageList.getItem( g_SubLanguage );
	if( !pLanguageText )
	{
		_XFatalError( _T("Can't access sub language item") );
		return;
	}

	_XLog( _T("Set sub language : %d:%s"), g_SubLanguage, pLanguageText->Get_String() );

	if( strcmpi( pLanguageText->Get_String(), _T("english") ) == 0 )
	{
		strcpy( szFilename, _T("NDTextDB_US.XSD") );
	}
	else
	{
		strcpy( szFilename, _T("NDTextDB_") );
		strcat( szFilename, pLanguageText->Get_String() );
		strcat( szFilename, _T(".XSD") );
	}
#else
	strcpy( szFilename, textstringdbfilename[g_LanguageType] );
#endif

	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

	if( !g_StringDataBase.Load( szFilename, g_LanguageType ) )
	{
		::ShowWindow( g_hLoadingWindow, SW_HIDE );
		::DestroyWindow(g_hLoadingWindow);
		g_hLoadingWindow = NULL;
		return;
	}

#else
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

	if( !g_StringDataBase.Load( textstringdbfilename[g_LanguageType], g_LanguageType ) )
	{
		::ShowWindow( g_hLoadingWindow, SW_HIDE );
		::DestroyWindow(g_hLoadingWindow);
		g_hLoadingWindow = NULL;
		return;
	}
#endif

#else
	if( !g_StringDataBase.Load( _T("NDTextDB.xsd"), g_LanguageType ) )
	{
		::ShowWindow( g_hLoadingWindow, SW_HIDE );
		::DestroyWindow(g_hLoadingWindow);
		g_hLoadingWindow = NULL;
		return;
	}	
#endif

#endif

	if( !CEmperorOfDragonsApp::CheckData() ) 
	{
		::ShowWindow( g_hLoadingWindow, SW_HIDE );
		::DestroyWindow(g_hLoadingWindow);
		g_hLoadingWindow = NULL;
		return;
	}

	if( !CEmperorOfDragonsApp::ReadyPackageArchive() )
	{
		::ShowWindow( g_hLoadingWindow, SW_HIDE );
		::DestroyWindow(g_hLoadingWindow);
		g_hLoadingWindow = NULL;
		return;
	}

	::ShowWindow( g_hLoadingWindow, SW_HIDE );
	::DestroyWindow(g_hLoadingWindow);
	g_hLoadingWindow = NULL;

	g_ParticleListPool.Reserve(500);
	g_ParticleGroupItemPool.Reserve(300);
	g_ParticleDecalObjectPool.Reserve(30);
	g_ParticleGroupInstanceManagerPool.Reserve(50);
	

	g_MemoryCheckData_0.ResetData();
	g_MemoryCheckData_1.ResetData();
	g_MemoryCheckData_2.ResetData();
	g_MemoryCheckData_3.ResetData();
	g_MemoryCheckData_4.ResetData();
	g_MemoryCheckData_5.ResetData();

	LPCTSTR pModuleFilename = _XArgv(0);
	LONG    lFileSize = 0;
	if( pModuleFilename )
	{
		FILE* pFile = fopen( pModuleFilename, _T("rb") );
		if( pFile )
		{
			LONG curptr = ftell( pFile );
			fseek( pFile , 0L, SEEK_END );
			LONG endptr = ftell( pFile );
			lFileSize = endptr - curptr;
			fclose( pFile );
		}
		else
		{
			_XLog( "%d,%d", 32, g_LocalSystemTime );
		}
	}
	else
	{
		_XLog( "ERROR : %d,%d", 33, g_LocalSystemTime );
	}

	g_ModuleFileSize = lFileSize;

#ifdef _XDEF_MODELTHREADLOADING
	if( _XInitializeTextureLoadBuffer( 1048620, 524310, 327680 ) )
#else
	if( _XInitializeTextureLoadBuffer( 1048620, 524310 ) )
#endif
	{
		timeBeginPeriod(1);		
		CEmperorOfDragonsApp *App = new CEmperorOfDragonsApp();
		App->Run();			
		delete App;
		timeEndPeriod(1);

		_XReleaseTextureLoadBuffer();
	}

#ifdef _XDEF_NEWENCRYPTION_PACKDATA
	// ..
#else	
	_XSetDefaultPath();
	_XDeleteFile( g_XScriptDecryptFilename );
#endif
}

BOOL CEmperorOfDragonsApp::ReadyPackageArchive( void )
{
	TCHAR packagefilename[_MAX_PATH];
	memset( packagefilename, 0, sizeof(TCHAR) * _MAX_PATH );
	
#ifdef _XDEF_NEWENCRYPTION_PACKDATA	

	// remove previous temporary file.
	_XSetDefaultPath();
	::DeleteFile( _T("ndcore.dsc") );

	strcpy( packagefilename, gModulePath );

	#ifdef _XTESTSERVER	
		strcat( packagefilename, _T("\\Data\\Script\\tsSCR_BASE.XP") );
	#else
		strcat( packagefilename, _T("\\Data\\Script\\SCR_BASE.XP") );
	#endif

	if( !g_ScriptArchive.OpenPackage(packagefilename) )
		return FALSE;

	g_ScriptArchive.SetEncryptedPackage( TRUE );

	g_ScriptArchive.ClosePackage();

#else	
	
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\") );
	strcat( packagefilename, g_XScriptDecryptFilename );
	if( !g_ScriptArchive.OpenPackage(packagefilename) )
		return FALSE;
	g_ScriptArchive.ClosePackage();

#endif
	
	// foreground loading용 model archieve.
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
#ifdef _XTESTSERVER		
	strcat( packagefilename, _T("\\Data\\Model\\tsMR_BASEMODEL.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Model\\MR_BASEMODEL.XP") );
#endif
	if( !g_ModelArchive.OpenPackage(packagefilename) )
		return FALSE;
	g_ModelArchive.ClosePackage();

	// background loading용 model archieve.
	if( !g_ThreadModelArchive.OpenPackage(packagefilename) )
		return FALSE;
	g_ThreadModelArchive.ClosePackage();

	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
#ifdef _XTESTSERVER		
	strcat( packagefilename, _T("\\Data\\Model\\tsAR_BASEANIMATION.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Model\\AR_BASEANIMATION.XP") );
#endif
	if( !g_AniArchive.OpenPackage(packagefilename) ) 
		return FALSE;
	g_AniArchive.ClosePackage();

	return TRUE;
}

void CEmperorOfDragonsApp::FinalizePackageArchive( void )
{
	g_ScriptArchive.FinalizePackage();
	g_ModelArchive.FinalizePackage();
	g_ThreadModelArchive.FinalizePackage();
	g_AniArchive.FinalizePackage();
	g_ScriptArchive.FinalizePackage();
}

BOOL CEmperorOfDragonsApp::CheckData( void )
{
#ifdef _XDEF_NEWENCRYPTION_PACKDATA
	// ..
#else
	_XSetDefaultPath();
	_XDeleteFile( g_XScriptDecryptFilename );
#endif

	_XPackageArchive scrpackagearchive;
	TCHAR srcfilename[_MAX_PATH];	
	memset( srcfilename, 0, sizeof(TCHAR) * _MAX_PATH );	
	strcpy( srcfilename, gModulePath );

#ifdef _XTESTSERVER	
	strcat( srcfilename, _T("\\Data\\Script\\tsSCR_BASE.XP") );
#else
	strcat( srcfilename, _T("\\Data\\Script\\SCR_BASE.XP") );
#endif
	
#ifdef _XDEF_NEWENCRYPTION_PACKDATA	
	// ..
#else

	TCHAR destfilename[_MAX_PATH];
	memset( destfilename, 0, sizeof(TCHAR) * _MAX_PATH );

	strcpy( destfilename, gModulePath );
	strcat( destfilename, _T("\\") );
	strcat( destfilename, g_XScriptDecryptFilename );

#endif

	if( !scrpackagearchive.OpenPackage( srcfilename ) )
	{
		return FALSE;
	}

#ifdef _XTESTSERVER	
	if( 0 > scrpackagearchive.FindResource( _T("TestPackIndicator.vid") ) )
	{
		_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
		scrpackagearchive.FinalizePackage();
		return FALSE;
	}
#else
	if( 0 <= scrpackagearchive.FindResource( _T("TestPackIndicator.vid") ) )
	{
		_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
		scrpackagearchive.FinalizePackage();		
		return FALSE;
	}
#endif

	int resourceindex = scrpackagearchive.FindResource( _T("versionindicator.vid") );
	if( resourceindex < 0 )
	{
		_XLog( "ERROR : scr_version indicator / step 1" );
		_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
		scrpackagearchive.FinalizePackage();		
		return FALSE;
	}
	else
	{
		FILE* pFile = scrpackagearchive.GetPackedFile( resourceindex );
		TCHAR readbuffer[64];
		TCHAR indicatorheader[64];
		int   packedversion;
		memset(readbuffer, 0, sizeof(TCHAR) * 64 );
		fread( readbuffer, sizeof(TCHAR) * 63, 1, pFile );
		for( int idx = 0; idx < 63; ++idx )
		{
			readbuffer[idx] ^= _FILEDATAENCODEKEY;
		}

		sscanf( readbuffer, "%s %d", indicatorheader, &packedversion );

#ifdef _PREOBVERSION // 대만의 프리오비버전은 pack version 1

		if( strcmpi( _T("scr_base.xp"), indicatorheader ) != 0 || (packedversion != 1) )
		{
			_XLog( "ERROR : scr_version indicator / step 2" );
			_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
			scrpackagearchive.FinalizePackage();		
			return FALSE;
		}

#else
	#ifdef _XTW_CSRELEASEVERSION // 대만의 정식릴리즈버전은 pack version 2

		if( strcmpi( _T("scr_base.xp"), indicatorheader ) != 0 || (packedversion != 2) )
		{
			_XLog( "ERROR : scr_version indicator / step 2" );
			_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
			scrpackagearchive.FinalizePackage();
			return FALSE;
		}

	#else

		#ifdef _XTESTSERVER	
			if( strcmpi( _T("scr_base.xp"), indicatorheader ) != 0 || (packedversion != 0) )
			{
				_XLog( "ERROR : scr_version indicator / step 2" );
				_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
				scrpackagearchive.FinalizePackage();
				return FALSE;
			}
		#else
			if( strcmpi( _T("scr_base.xp"), indicatorheader ) != 0 || (packedversion != 0) )
			{
				_XLog( "ERROR : scr_version indicator / step 2" );
				_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
				scrpackagearchive.FinalizePackage();
				return FALSE;
			}
		#endif

	#endif

#endif
	}

	scrpackagearchive.FinalizePackage();

#ifdef _XTAIWANESE

	_XPackageArchive checkpackagearchive;
	int checkresindex = -1;

	TCHAR checksrcfilename[_MAX_PATH];
	TCHAR packname[12][64] = 
	{
		"\\Data\\Environment\\VR_ENVIRONMENT.XP",
		
		"\\Data\\Model\\AR_BASEANIMATION.XP",
		"\\Data\\Model\\MR_BASEMODEL.XP",

		"\\Data\\Script\\XSCENESCRIPT.XP",
		"\\Data\\Script\\SCR_BASE.XP",

		"\\Data\\Sound\\SR_SOUND.XP",

		"\\Data\\Texture\\TR_ENVIRONMENT.XP",
		"\\Data\\Texture\\TR_MAININTERFACE.XP",
		"\\Data\\Texture\\TR_CHARACTER.XP",
		"\\Data\\Texture\\TR_MONSTER.XP",		
		"\\Data\\Texture\\TR_SSTMODEL.XP",
		"\\Data\\Texture\\TR_VISUALEFFECT.XP"
	};
	
	for( int i = 0; i < 12; i++ )
	{
		memset( checksrcfilename, 0, sizeof(TCHAR) * _MAX_PATH );		
		strcpy( checksrcfilename, gModulePath );
		strcat( checksrcfilename, packname[i] );

		if( !checkpackagearchive.OpenPackage( checksrcfilename ) )
		{
			return FALSE;
		}

#ifdef _PREOBVERSION
		int checkresindex = checkpackagearchive.FindResource( _T("preobindicator.vid") );
		if( checkresindex < 0 )
		{
			_XLog( "ERROR : Invalid pack file [%s]", checksrcfilename );
			_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
			checkpackagearchive.FinalizePackage();
			return FALSE;
		}
#else
		#ifdef _XTW_CSRELEASEVERSION // 대만의 정식릴리즈버전은 pack version 2
			int checkresindex = checkpackagearchive.FindResource( _T("csindicator.vid") );
			if( checkresindex < 0 )
			{
				_XLog( "ERROR : Invalid pack file [%s]", checksrcfilename );
				_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
				checkpackagearchive.FinalizePackage();
				return FALSE;
			}
		#else
			int checkresindex = checkpackagearchive.FindResource( _T("preobindicator.vid") );
			if( checkresindex >= 0 )
			{
				_XLog( "ERROR : Invalid pack file [%s]", checksrcfilename );
				_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
				checkpackagearchive.FinalizePackage();
				return FALSE;
			}	
		#endif

#endif
		checkpackagearchive.FinalizePackage();
	}	

#endif

#ifdef _XDEF_NEWENCRYPTION_PACKDATA
	// ..
#else
	if( !scrpackagearchive.CreateDecryptFile( srcfilename, destfilename ) )
	{
		_XLog( "Can't create ndcore data" );
		return FALSE;
	}
#endif
/*
	TCHAR deletetargetname[10][32] = 
	{
		_T("MAP_SORIM"),
		_T("MAP_GAEBANG"),
		_T("map_noklim"),
		_T("map_beegoong"),
		_T("MAP_JUNGJU"),
		_T("MAP_SEOKGAJA"),
		_T("MAP_JAENAM"),
		_T("MAP_NAMCHANG"),
		_T("MAP_HANGJOO"),
		_T("MAP_DAEDONG")
	};

	TCHAR deletetargetextaname[5][10] = 
	{
		_T(".XTV"),
		_T(".XFS"),
		_T(".XNS"),
		_T(".XSS"),
		_T(".XZI")
	};

	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_ENVIRONMENT);

		TCHAR deletetargetfilename[64];
		for( int i = 0; i < 10; ++i )
		{
			for( int j = 0; j < 5; ++j )
			{
				memset( deletetargetfilename, 0, sizeof(TCHAR) * 64 );
				strcpy( deletetargetfilename, deletetargetname[i] );
				strcat( deletetargetfilename, deletetargetextaname[j] );
				
				_XLog( "Delete file [%s]", deletetargetfilename );

				_XDeleteFile( deletetargetfilename );				
			}
		}

		//_XLog( "Complete delete gabage file [%s]", deletetargetfilename );
	}
	
	_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
	_XDeleteFile( _T("mob_table_client.xbmo") ); // gabage clear

  */	
	
	TCHAR	terrainpackname[64];
	memset( terrainpackname, 0, sizeof(TCHAR)*64 );
	
#ifdef _XTESTSERVER	
	_XSetDefaultPath();
	strcpy( terrainpackname, _T("\\Data\\Environment\\tsVR_ENVIRONMENT.XP") );
#else	
	_XSetDefaultPath();
	strcpy( terrainpackname, _T("\\Data\\Environment\\VR_ENVIRONMENT.XP") );
#endif

	memset( srcfilename, 0, sizeof(TCHAR) * _MAX_PATH );
	strcpy( srcfilename, gModulePath );
	strcat( srcfilename, terrainpackname );
	if( !scrpackagearchive.OpenPackage( srcfilename ) )
	{
		return FALSE;
	}

#ifdef _XTESTSERVER	
	if( 0 > scrpackagearchive.FindResource( _T("TestPackIndicator.vid") ) )
	{
		_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
		scrpackagearchive.FinalizePackage();
		return FALSE;
	}
#else
	if( 0 <= scrpackagearchive.FindResource( _T("TestPackIndicator.vid") ) )
	{
		_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
		scrpackagearchive.FinalizePackage();
		return FALSE;
	}
#endif
	
	resourceindex = scrpackagearchive.FindResource( _T("vr_versionindicator.vid") );
	if( resourceindex < 0 )
	{
		_XLog( "ERROR : vr_version indicator / step 1" );
		_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
		scrpackagearchive.FinalizePackage();
		return FALSE;
	}
	else
	{
		FILE* pFile = scrpackagearchive.GetPackedFile( resourceindex );
		TCHAR indicatorheader[64];
		TCHAR readbuffer[64];
		int   packedversion;

		memset(readbuffer, 0, sizeof(TCHAR) * 64 );
		fgets( readbuffer, 63, pFile );
		sscanf( readbuffer, "%s %d", indicatorheader, &packedversion );		

#ifdef _XTESTSERVER	
		if( strcmpi( _T("VR_ENVIRONMENT.XP"), indicatorheader ) != 0 || (packedversion != 0) )
		{
			_XLog( "ERROR : vr_version indicator / step 2" );
			_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
			scrpackagearchive.FinalizePackage();
			return FALSE;
		}
#else	
		if( strcmpi( _T("VR_ENVIRONMENT.XP"), indicatorheader ) != 0 || (packedversion != 0) )
		{
			_XLog( "ERROR : vr_version indicator / step 2" );
			_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
			scrpackagearchive.FinalizePackage();
			return FALSE;
		}
#endif		
	}

	scrpackagearchive.FinalizePackage();

#ifdef _XTESTSERVER	
	TCHAR checkpackname[18][128] = 
	{
		_T("\\Data\\Model\\tsMR_BASEMODEL.XP"),
		_T("\\Data\\Model\\tsAR_BASEANIMATION.XP"),
		_T("\\Data\\Model\\tsMR_VISUALEFFECT.XP"),
		_T("\\Data\\Model\\tsMR_STMODEL.XP"),		
		
		_T("\\Data\\Script\\tsSCR_BASE.XP"),
		_T("\\Data\\Script\\tsSHADER.XP"),
		_T("\\Data\\Script\\tsXSCENESCRIPT.XP"),		

		_T("\\Data\\Sound\\tsSR_SOUND.XP"),
		
		_T("\\Data\\Texture\\tsTR_ENVIRONMENT.XP"),
		_T("\\Data\\Texture\\tsTR_INTERFACE.XP"),
		_T("\\Data\\Texture\\tsTR_HMOUSE.XP"),
		_T("\\Data\\Texture\\tsTR_EFFECT.XP"),
		_T("\\Data\\Texture\\tsTR_MAININTERFACE.XP"),
		_T("\\Data\\Texture\\tsTR_CHARACTER.XP"),
		_T("\\Data\\Texture\\tsTR_MONSTER.XP"),
		_T("\\Data\\Texture\\tsTR_XFONT.XP"),
		_T("\\Data\\Texture\\tsTR_VISUALEFFECT.XP"),
		_T("\\Data\\Texture\\tsTR_SSTMODEL.XP"),
	};
#else
	TCHAR checkpackname[18][128] = 
	{
		_T("\\Data\\Model\\MR_BASEMODEL.XP"),
		_T("\\Data\\Model\\AR_BASEANIMATION.XP"),
		_T("\\Data\\Model\\MR_VISUALEFFECT.XP"),
		_T("\\Data\\Model\\MR_STMODEL.XP"),		
			
		_T("\\Data\\Script\\SCR_BASE.XP"),
		_T("\\Data\\Script\\SHADER.XP"),
		_T("\\Data\\Script\\XSCENESCRIPT.XP"),		
			
		_T("\\Data\\Sound\\SR_SOUND.XP"),
			
		_T("\\Data\\Texture\\TR_ENVIRONMENT.XP"),
		_T("\\Data\\Texture\\TR_INTERFACE.XP"),
		_T("\\Data\\Texture\\TR_HMOUSE.XP"),
		_T("\\Data\\Texture\\TR_EFFECT.XP"),
		_T("\\Data\\Texture\\TR_MAININTERFACE.XP"),
		_T("\\Data\\Texture\\TR_CHARACTER.XP"),
		_T("\\Data\\Texture\\TR_MONSTER.XP"),
		_T("\\Data\\Texture\\TR_XFONT.XP"),
		_T("\\Data\\Texture\\TR_VISUALEFFECT.XP"),
		_T("\\Data\\Texture\\TR_SSTMODEL.XP"),
	};
#endif

	TCHAR packpathname[_MAX_PATH];
		
	for( int j = 0 ; j < 18; ++j )
	{	
		memset( packpathname, 0, sizeof(TCHAR) * _MAX_PATH );
		strcpy( packpathname, gModulePath );
		strcat( packpathname, checkpackname[j] );

#ifdef _XTESTSERVER	
		_XSetDefaultPath();
		if( !scrpackagearchive.OpenPackage( packpathname ) )
		{
			return FALSE;
		}

		if( 0 > scrpackagearchive.FindResource( _T("TestPackIndicator.vid") ) )
		{
			_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
			scrpackagearchive.FinalizePackage();
			return FALSE;
		}
#else	
		_XSetDefaultPath();
		if( !scrpackagearchive.OpenPackage( packpathname ) )
		{
			scrpackagearchive.FinalizePackage();
			return FALSE;
		}
		
		if( 0 <= scrpackagearchive.FindResource( _T("TestPackIndicator.vid") ) )
		{
			_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2588) );
			scrpackagearchive.FinalizePackage();
			return FALSE;
		}
#endif
		scrpackagearchive.FinalizePackage();
	}

	return TRUE;
}

CEmperorOfDragonsApp::CEmperorOfDragonsApp()
{
	m_InitializedApp = FALSE;
	g_CurrentUser.InitializeData();
	g_pLocalUser = &g_CurrentUser; // hotblood zerolevel
}

CEmperorOfDragonsApp::~CEmperorOfDragonsApp()
{

}

bool CEmperorOfDragonsApp::Startup()
{	
#ifdef _XCHECKMEMORYUSAGE
	long usedmemsize = GetMemoryUsageInfo();
	_XLog( "Memory report : Global memory section used size : %f [%d]", usedmemsize / 1048576.0f, usedmemsize );
#endif

	if( !InitGameObjects() ) return false;

#ifdef _XDWDEBUG
	m_ProcessManager.ChangeProcess( &m_proc_firstload );	
	//m_ProcessManager.ChangeProcess( &m_proc_ninedragonstitle );
#else
	m_ProcessManager.ChangeProcess( &m_proc_ninedragonstitle );
#endif
			
	m_InitializedApp = TRUE;
	
/*	char* ppp = 0;
	char x = 0;
	*ppp = x;*/
		
	return true;
}


void CEmperorOfDragonsApp::Shutdown() 
{	
	m_ProcessManager.CloseAllProcess();	
	ReleaseGameObjects();		
	m_proc_firstload.Relase3DSound();
	m_InitializedApp = FALSE;

	_XLog( "Shutdown _XKernel" );
}


//=====================================================================================================================
// Initialize game data
//=====================================================================================================================
bool CEmperorOfDragonsApp::InitGameObjects()
{
	m_proc_firstload.Initialize3DSound();
				
	if( !g_XGUIControlCenter.Initialize() )
	{
		_XFatalError( "_XGUIControlCenter initialize" );
		return false;
	}

	InitializeStringData();

	g_XGQProperty.terrainqualitylevel		= (_XGraphicsQualityLevel)g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_QUALITYLEVEL_TERRAIN,     _XGQLEVEL_MAX, TRUE );
	g_XGQProperty.characterqualitylevel		= (_XGraphicsQualityLevel)g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_QUALITYLEVEL_CHARACTER,   _XGQLEVEL_MAX, TRUE );
	g_XGQProperty.npcqualitylevel			= (_XGraphicsQualityLevel)g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_QUALITYLEVEL_NPC,		   _XGQLEVEL_MAX, TRUE );
	g_XGQProperty.monsterqualitylevel		= (_XGraphicsQualityLevel)g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_QUALITYLEVEL_MONSTER,	   _XGQLEVEL_MAX, TRUE );
	g_XGQProperty.worldobjectqualitylevel	= (_XGraphicsQualityLevel)g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_QUALITYLEVEL_WORLDOBJECT, _XGQLEVEL_MAX, TRUE );

	g_XGQProperty.worldobjectmiplevel		= (_XGraphicsQualityLevel)g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_MIPLEVEL_WORLDOBJECT,     _XGQLEVEL_MAX, TRUE );	
	g_XGQProperty.detailttilemiplevel		= (_XGraphicsQualityLevel)g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_MIPLEVEL_DETAILTTILE,     _XGQLEVEL_MAX, TRUE );		
	g_XGQProperty.charactermiplevel			= (_XGraphicsQualityLevel)g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_MIPLEVEL_CHARACTER,	   _XGQLEVEL_MAX, TRUE );

	g_XGQProperty.sightrange				= (_XGraphicsQualityLevel)g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_SIGHTLEVEL_RANGE,	    _XGQLEVEL_MIDDLE, TRUE );
	
	g_DefaultCameraFov						= 40.0f;

	D3DXMatrixIdentity( &g_IdentMatrix );

	_XGraphicsQualityLevel charactershadowql= (_XGraphicsQualityLevel)g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_QUALITYLEVEL_CHARACTERSHADOW, _XGQLEVEL_MAX, TRUE );
	switch( charactershadowql ) 
	{	
	case _XGQLEVEL_MAX		: g_ShadowTextureSize	=	512;
		break;
	case _XGQLEVEL_HIGH		: g_ShadowTextureSize	=	256;
		break;
	default					:
	case _XGQLEVEL_MIDDLE	: g_ShadowTextureSize	=	128;
		break;
	case _XGQLEVEL_LOW		: g_ShadowTextureSize	=	64;
		break;	
	} 
	
	switch( g_XGQProperty.worldobjectmiplevel ) 
	{
	default					:
	case _XGQLEVEL_MAX		: 
	case _XGQLEVEL_HIGH		: g_TextureMipLevel_Object = 7;
		break;
	case _XGQLEVEL_MIDDLE	: g_TextureMipLevel_Object = 3;
		break;
	case _XGQLEVEL_LOW		: g_TextureMipLevel_Object = 1;
		break;	
	} 
	
	switch( g_XGQProperty.detailttilemiplevel ) 
	{
	default					:
	case _XGQLEVEL_MAX		: 
	case _XGQLEVEL_HIGH		: g_TextureMipLevel_TerrainDetail = 5;
		break;
	case _XGQLEVEL_MIDDLE	: g_TextureMipLevel_TerrainDetail = 3;
		break;
	case _XGQLEVEL_LOW		: g_TextureMipLevel_TerrainDetail = 1;
		break;	
	} 

	switch( g_XGQProperty.charactermiplevel ) 
	{
	default					:
	case _XGQLEVEL_MAX		: 
	case _XGQLEVEL_HIGH		: g_TextureMipLevel_CharacterModel = 3;
		break;
	case _XGQLEVEL_MIDDLE	: g_TextureMipLevel_CharacterModel = 2;
		break;
	case _XGQLEVEL_LOW		: g_TextureMipLevel_CharacterModel = 1;
		break;	
	} 
	
	//g_TextureMipLevel_CharacterModel = 1; // 무조건 캐릭터는 1단계

	_XMEMORYUSECHECKREADY

	_XMEMORYUSECHECKSTART
	if(!_XCharacterStateList::LoadCharacterStateTable())
	{
		_XFatalError( "Failed to load : Character state table" );
		return false;
	}
	_XMEMORYUSECHECKEND( "_XSkillItem::LoadSCharacterStateTable()" );

	_XMEMORYUSECHECKSTART
	if( !_XGameItem::LoadItemProperty() )
	{
		_XFatalError( "Failed to load : Item property" );
		return false;
	}
	_XMEMORYUSECHECKEND( "_XGameItem::LoadItemProperty" );
	
	_XMEMORYUSECHECKSTART
	if( !_XGameItem::LoadNickNameInfoTable() ) 
	{
		_XFatalError( "Failed to load : Nick name information table" );
		return false; //별호 테이블을 읽어들인다.
	}
	_XMEMORYUSECHECKEND( "_XGameItem::LoadNickNameInfo" );

	_XMEMORYUSECHECKSTART
	if( !_XGameItem::LoadItemMixTable() ) 
	{
		_XFatalError( "Failed to load : Item Mix Table" );
		return false; 
	}
	_XMEMORYUSECHECKEND( "_XGameItem::LoadItemMixTable" );

	if( !_XGameItem::LoadItemInchantTable() )
	{
		_XFatalError( "Failed to load : Item Inchant Table" );
		return false; 
	}
	_XMEMORYUSECHECKEND( "_XGameItem::LoadItemInchantTable" );	
		
	_XMEMORYUSECHECKSTART
	if( !_XSkillItem::LoadSkillProperty() ) 
	{
		_XFatalError( "Failed to load : Skill properties" );
		return false;
	}
	_XMEMORYUSECHECKEND( "_XSkillItem::LoadSkillProperty" );
	
	_XMEMORYUSECHECKSTART
	if( !_XSkillItem::LoadSkillPackage() ) 
	{
		_XFatalError( "Failed to load : Skill package" );
		return false;
	}
	_XMEMORYUSECHECKEND( "_XSkillItem::LoadSkillPackage()" );


#ifdef _XDWDEBUG

	if( _XCmdLineOption("xscriptwindow",0) != 0 )
	{
		g_ViewScriptWindow = TRUE;
	}

	if( g_ViewScriptWindow )
	{
		g_RichEditDLL = LoadLibraryA("riched32.dll"); // LoadLibraryA("Riched20.dll");
		failed_bool(!g_RichEditDLL, "Can't load Riched32.dll" );

		g_hEffectScriptWindow = CreateDialog(NULL, MAKEINTRESOURCE(IDD_EFFECTSCRIPTDIALOG), gHWnd, (DLGPROC)_XKernelEffectScriptWindowProc);
		ShowWindow(g_hEffectScriptWindow, SW_SHOW); 

		RECT rect;
		GetWindowRect( g_hEffectScriptWindow, &rect );
		int effectscriptwinxpos = (GetSystemMetrics(SM_CXSCREEN)-(rect.right-rect.left))/2;
		int effectscriptwinypos = (GetSystemMetrics(SM_CYSCREEN)-(rect.bottom-rect.top))/2;

		effectscriptwinxpos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_EFFECTSCRIPTWINDOWPOSITIONX, effectscriptwinxpos, TRUE );
		effectscriptwinypos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_EFFECTSCRIPTWINDOWPOSITIONY, effectscriptwinypos, TRUE );
		
		int effectscriptwinwidth  = rect.right-rect.left;
		int effectscriptwinheight = rect.bottom-rect.top;	
		MoveWindow( g_hEffectScriptWindow, effectscriptwinxpos, effectscriptwinypos, effectscriptwinwidth, effectscriptwinheight, TRUE );
	}

#endif

	
#ifdef _XTS_ITEMMALLBROWSER
	
	#ifdef _XTESTSERVER
		g_bTestWebPageAddress = TRUE;	// 테스트 서버일 경우 테스트 페이지 열기
	#endif

	// 아이템몰 웹 브라우져 
	// Initialize the OLE interface. We do this once-only.
	WNDCLASSEX		wc;
	// The class name of our Main Window. It can be anything of your choosing.
	static const TCHAR	ClassName[] = "Item Mall";
	
	// The class name of our Window to host the browser. It can be anything of your choosing.
	static const TCHAR	BrowserClassName[] = "Item Mall Object";
	
	// Register the class of our Main window. 'windowProc' is our message handler
	// and 'ClassName' is the class name. You can choose any class name you want.
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = gHInst;
	wc.lpfnWndProc = (WNDPROC)_XKernelItemMallWindowProc;
	wc.lpszClassName = &ClassName[0];
	wc.style = CS_CLASSDC|CS_HREDRAW|CS_VREDRAW|CS_PARENTDC|CS_BYTEALIGNCLIENT|CS_DBLCLKS;
	RegisterClassEx(&wc);
	
	// Register the class of our window to host the browser. 'browserWindowProc' is our message handler
	// and 'BrowserClassName' is the class name. You can choose any class name you want.
	wc.lpfnWndProc = (WNDPROC)_XKernelItemMallWebBrowserWindowProc;
	wc.lpszClassName = &BrowserClassName[0];
	wc.style = CS_HREDRAW|CS_VREDRAW;
	RegisterClassEx(&wc);
	
	int defaultwinxpos = 0;
	int defaultwinypos = 0;
	defaultwinxpos = (GetSystemMetrics(SM_CXSCREEN)-gnWidth)/2;
	defaultwinypos = (GetSystemMetrics(SM_CYSCREEN)-gnHeight)/2;
	defaultwinxpos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_WINDOWPOSITIONX, defaultwinxpos );	
	defaultwinypos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_WINDOWPOSITIONY, defaultwinypos );
	// Create a Main window.
	if ((g_hItemMallWindow = CreateWindowEx(0, &ClassName[0], 0, WS_CHILD|WS_POPUP    ,//WS_OVERLAPPEDWINDOW,
		//defaultwinxpos+112, defaultwinypos+76, 800, 600,
		(gnWidth>>1)-400, (gnHeight>>1)-280, 800, 600,		
		gHWnd, NULL, gHInst, 0)))//HWND_DESKTOP
	{
		//UINT style = GetWindowLong(g_hItemMallWindow,GWL_STYLE);
		//style &= ~WS_CAPTION;
		//SetWindowLong(g_hItemMallWindow, GWL_STYLE, style);

		// Create a child window to host the browser object. NOTE: We embed the browser object
		// duing our WM_CREATE handling for this child window.
		if ((g_hItemMallWebBrowserObject = CreateWindowEx(0, &BrowserClassName[0], 0, WS_CHILD|WS_VISIBLE,
			0, 0, 800, 600,
			//g_hItemMallWindow, (HMENU)1000, gHInst, 0)))
			g_hItemMallWindow, NULL, gHInst, 0)))
		{
			UINT style = GetWindowLong(g_hItemMallWebBrowserObject,GWL_STYLE);
			style &= ~WS_HSCROLL;
			style &= ~WS_VSCROLL;
			SetWindowLong(g_hItemMallWebBrowserObject, GWL_STYLE, style);
			
#ifdef _XKOREAN
			// For this window, display a URL. This could also be a HTML file on disk such as "c:\\myfile.htm".	
			if( !g_bTestWebPageAddress )
				DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ninemall.nexon.com");
			else
				DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://panda.nexon.com");	
#endif

#ifdef _XENGLISH 
			DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://9dragons.acclaim.com/store/store.jsp");
#endif
			
#ifdef _XVIETNAMESE

			//DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ktc.cuulong.com.vn");
			DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://cuulongid.zing.vn/ktc");
#endif
			
#ifdef _XTAIWANESE
			DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://member.pfamily.com.tw/9ds_shopping_mall/login.html"); //Author: 양희왕 //breif : 08/01/16 추가
#endif
			
#ifdef _XJAPANESE
			//DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ninetest.gameleon.jp/item_mall/item_login.asp"); 
			DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://nine.gameleon.jp/item_mall/item_login.asp"); 
#endif
			// Show the Main window.
			ShowWindow(g_hItemMallWindow, SW_HIDE);
			UpdateWindow(g_hItemMallWindow);
			
		}
		else
		{
			MessageBox(g_hItemMallWindow, "Can't create browser child window!", "ERROR", MB_OK);
			DestroyWindow(g_hItemMallWindow);
		}
	}
#endif

	
#ifdef _XDEF_WEBBOARD_20061211
	// 웹 게시판 브라우져 
	// Initialize the OLE interface. We do this once-only.
	WNDCLASSEX		wc01;
	// The class name of our Main Window. It can be anything of your choosing.
	static const TCHAR	ClassName01[] = "Web Board";
	
	// The class name of our Window to host the browser. It can be anything of your choosing.
	static const TCHAR	BrowserClassName01[] = "Web Board Object";
	
	// Register the class of our Main window. 'windowProc' is our message handler
	// and 'ClassName' is the class name. You can choose any class name you want.
	ZeroMemory(&wc01, sizeof(WNDCLASSEX));
	wc01.cbSize = sizeof(WNDCLASSEX);
	wc01.hInstance = gHInst;
	wc01.lpfnWndProc = (WNDPROC)_XKernelWebBoardWindowProc;
	wc01.lpszClassName = &ClassName01[0];
	wc01.style = CS_CLASSDC|CS_HREDRAW|CS_VREDRAW|CS_PARENTDC|CS_BYTEALIGNCLIENT|CS_DBLCLKS;
	RegisterClassEx(&wc01);
	
	// Register the class of our window to host the browser. 'browserWindowProc' is our message handler
	// and 'BrowserClassName' is the class name. You can choose any class name you want.
	wc01.lpfnWndProc = (WNDPROC)_XKernelWebBoardWebBrowserWindowProc;
	wc01.lpszClassName = &BrowserClassName01[0];
	wc01.style = CS_HREDRAW|CS_VREDRAW;
	RegisterClassEx(&wc01);
	
	int defaultwinxpos01 = 0;
	int defaultwinypos01 = 0;
	defaultwinxpos01 = (GetSystemMetrics(SM_CXSCREEN)-gnWidth)/2;
	defaultwinypos01 = (GetSystemMetrics(SM_CYSCREEN)-gnHeight)/2;
	defaultwinxpos01 = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_WINDOWPOSITIONX, defaultwinxpos01 );	
	defaultwinypos01 = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_WINDOWPOSITIONY, defaultwinypos01 );
	// Create a Main window.
	if ((g_hWebBoardWindow = CreateWindowEx(0, &ClassName01[0], 0, WS_CHILD|WS_POPUP    ,//WS_OVERLAPPEDWINDOW,
		//defaultwinxpos+112, defaultwinypos+76, 800, 600,
		(gnWidth>>1)-344, (gnHeight>>1)-225, 688, 450,		
		gHWnd, NULL, gHInst, 0)))//HWND_DESKTOP
	{
		//UINT style = GetWindowLong(g_hItemMallWindow,GWL_STYLE);
		//style &= ~WS_CAPTION;
		//SetWindowLong(g_hItemMallWindow, GWL_STYLE, style);
		
		// Create a child window to host the browser object. NOTE: We embed the browser object
		// duing our WM_CREATE handling for this child window.
		if ((g_hIWebBoardWebBrowserObject = CreateWindowEx(0, &BrowserClassName01[0], 0, WS_CHILD|WS_VISIBLE,
			0, 0, 688, 450,
			//g_hItemMallWindow, (HMENU)1000, gHInst, 0)))
			g_hWebBoardWindow, NULL, gHInst, 0)))
		{
			UINT style = GetWindowLong(g_hIWebBoardWebBrowserObject,GWL_STYLE);
			style &= ~WS_HSCROLL;
			style &= ~WS_VSCROLL;
			SetWindowLong(g_hIWebBoardWebBrowserObject, GWL_STYLE, style);
			// For this window, display a URL. This could also be a HTML file on disk such as "c:\\myfile.htm".
			//DisplayHTMLPage(g_hIWebBoardWebBrowserObject, "http://nine.nexon.com");
			
			// Show the Main window.
			ShowWindow(g_hWebBoardWindow, SW_HIDE);
			UpdateWindow(g_hWebBoardWindow);
			
		}
		else
		{
			MessageBox(g_hWebBoardWindow, "Can't create browser child window!", "ERROR", MB_OK);
			DestroyWindow(g_hWebBoardWindow);
		}
	}
#endif
	
#ifdef _XDEF_EVENTWINDOW_WEBPAGE_20070214
	// 웹 이벤트 창 브라우져 
	// Initialize the OLE interface. We do this once-only.
	WNDCLASSEX		wc02;
	// The class name of our Main Window. It can be anything of your choosing.
	static const TCHAR	ClassName02[] = "Web Event Window";
	
	// The class name of our Window to host the browser. It can be anything of your choosing.
	static const TCHAR	BrowserClassName02[] = "Web Event Object";
	
	// Register the class of our Main window. 'windowProc' is our message handler
	// and 'ClassName' is the class name. You can choose any class name you want.
	ZeroMemory(&wc02, sizeof(WNDCLASSEX));
	wc02.cbSize = sizeof(WNDCLASSEX);
	wc02.hInstance = gHInst;
	wc02.lpfnWndProc = (WNDPROC)_XKernelWebEventWindowProc;
	wc02.lpszClassName = &ClassName02[0];
	wc02.style = CS_CLASSDC|CS_HREDRAW|CS_VREDRAW|CS_PARENTDC|CS_BYTEALIGNCLIENT|CS_DBLCLKS;
	RegisterClassEx(&wc02);
	
	// Register the class of our window to host the browser. 'browserWindowProc' is our message handler
	// and 'BrowserClassName' is the class name. You can choose any class name you want.
	wc02.lpfnWndProc = (WNDPROC)_XKernelWebEventWebBrowserWindowProc;
	wc02.lpszClassName = &BrowserClassName02[0];
	wc02.style = CS_HREDRAW|CS_VREDRAW;
	RegisterClassEx(&wc02);
	
	int defaultwinxpos02 = 0;
	int defaultwinypos02 = 0;
	defaultwinxpos02 = (GetSystemMetrics(SM_CXSCREEN)-gnWidth)/2;
	defaultwinypos02 = (GetSystemMetrics(SM_CYSCREEN)-gnHeight)/2;
	defaultwinxpos02 = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_WINDOWPOSITIONX, defaultwinxpos02 );	
	defaultwinypos02 = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_WINDOWPOSITIONY, defaultwinypos02 );
	// Create a Main window.
	if ((g_hWebEventWindow = CreateWindowEx(0, &ClassName02[0], 0, WS_CHILD|WS_POPUP    ,//WS_OVERLAPPEDWINDOW,
		(gnWidth>>1)-344, (gnHeight>>1)-225, 215, 315,		
		gHWnd, NULL, gHInst, 0)))//HWND_DESKTOP
	{
		//UINT style = GetWindowLong(g_hItemMallWindow,GWL_STYLE);
		//style &= ~WS_CAPTION;
		//SetWindowLong(g_hItemMallWindow, GWL_STYLE, style);
		
		// Create a child window to host the browser object. NOTE: We embed the browser object
		// duing our WM_CREATE handling for this child window.
		if ((g_hEventWebBrowserObject = CreateWindowEx(0, &BrowserClassName02[0], 0, WS_CHILD|WS_VISIBLE,
			0, 0, 215, 315,
			g_hWebEventWindow, NULL, gHInst, 0)))
		{
			UINT style = GetWindowLong(g_hEventWebBrowserObject,GWL_STYLE);
			style &= ~WS_HSCROLL;
			style &= ~WS_VSCROLL;
			SetWindowLong(g_hEventWebBrowserObject, GWL_STYLE, style);
			// For this window, display a URL. This could also be a HTML file on disk such as "c:\\myfile.htm".
			
			if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
			{
				DisplayHTMLPage(g_hEventWebBrowserObject, "http://md.ninedragons.co.kr/popup/Gpop/Gpop.html");
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
			{
				//DisplayHTMLPage(g_hEventWebBrowserObject, "http://www.cuulong.com.vn/sukien/");
				DisplayHTMLPage(g_hEventWebBrowserObject, "http://cuulong.zing.vn/sukien/"); //Author : 양희왕 //breif : 08/01/03 변경
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
			{
				DisplayHTMLPage(g_hEventWebBrowserObject, "http://9dragons.acclaim.com/game/9d-pop.htm");			
			}
			
			// Show the Main window.
			ShowWindow(g_hWebEventWindow, SW_HIDE);
			UpdateWindow(g_hWebEventWindow);
			
		}
		else
		{
			MessageBox(g_hWebEventWindow, "Can't create browser child window!", "ERROR", MB_OK);
			DestroyWindow(g_hWebEventWindow);
		}
	}
#endif
	
#ifdef _ACCLAIM_RUBICONADSYSTEM

	// Acclaim rubicon ad system browser 	
	WNDCLASSEX		wc_AD;
	
	static const TCHAR	ClassName_AD[] = "Rubicon AD Window";		
	static const TCHAR	BrowserClassName_AD[] = "Rubicon AD Object";	
	
	ZeroMemory(&wc_AD, sizeof(WNDCLASSEX));
	wc_AD.cbSize = sizeof(WNDCLASSEX);
	wc_AD.hInstance = gHInst;
	wc_AD.lpfnWndProc = (WNDPROC)_XKernelRubiconADWindowProc;
	wc_AD.lpszClassName = &ClassName_AD[0];
	wc_AD.style = CS_CLASSDC|CS_HREDRAW|CS_VREDRAW|CS_PARENTDC|CS_BYTEALIGNCLIENT|CS_DBLCLKS;
	RegisterClassEx(&wc_AD);
		
	wc_AD.lpfnWndProc = (WNDPROC)_XKernelRubiconADWebBrowserWindowProc;
	wc_AD.lpszClassName = &BrowserClassName_AD[0];
	wc_AD.style = CS_HREDRAW|CS_VREDRAW;
	RegisterClassEx(&wc_AD);
	
	int defaultwinxpos_AD = 0;
	int defaultwinypos_AD = 0;
	defaultwinxpos_AD = (GetSystemMetrics(SM_CXSCREEN)-gnWidth)/2;
	defaultwinypos_AD = (GetSystemMetrics(SM_CYSCREEN)-gnHeight)/2;
	defaultwinxpos_AD = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_WINDOWPOSITIONX, defaultwinxpos_AD );	
	defaultwinypos_AD = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_WINDOWPOSITIONY, defaultwinypos_AD );
	// Create a Main window.
	if ((g_hRubiconADWindow = CreateWindowEx(0, &ClassName_AD[0], 0, WS_CHILD|WS_POPUP    ,//WS_OVERLAPPEDWINDOW,
		(gnWidth>>1)-344, (gnHeight>>1)-225, 215, 315,		
		gHWnd, NULL, gHInst, 0)))//HWND_DESKTOP
	{
		//UINT style = GetWindowLong(g_hItemMallWindow,GWL_STYLE);
		//style &= ~WS_CAPTION;
		//SetWindowLong(g_hItemMallWindow, GWL_STYLE, style);
		
		// Create a child window to host the browser object. NOTE: We embed the browser object
		// duing our WM_CREATE handling for this child window.
		if ((g_hRubiconADWebBrowserObject = CreateWindowEx(0, &BrowserClassName_AD[0], 0, WS_CHILD|WS_VISIBLE,
			0, 0, 800, 600,
			g_hRubiconADWindow, NULL, gHInst, 0)))
		{
			UINT style = GetWindowLong(g_hRubiconADWebBrowserObject,GWL_STYLE);
			style &= ~WS_HSCROLL;
			style &= ~WS_VSCROLL;
			SetWindowLong(g_hRubiconADWebBrowserObject, GWL_STYLE, style);
			// For this window, display a URL. This could also be a HTML file on disk such as "c:\\myfile.htm".
			
			DisplayHTMLPage(g_hRubiconADWebBrowserObject, "http://www.acclaim.com/ads/9d_rubicon_728_90.html");
						
			// Show the Main window.
			ShowWindow(g_hRubiconADWindow, SW_HIDE);
			UpdateWindow(g_hRubiconADWindow);
			
		}
		else
		{
			MessageBox(g_hRubiconADWindow, "Can't create rubicon AD browser child window!", "ERROR", MB_OK);
			DestroyWindow(g_hRubiconADWindow);
		}
	}
	
	g_RubiconADFreeLevel = 0;

	if( g_ScriptArchive.ReOpenPackage() )
	{
		FILE* pFreeAdLevelScript = NULL;
		
		int resourceindex = g_ScriptArchive.FindResource( _T("RubiconAD.ini") );
		if( resourceindex >= 0 )
		{
			pFreeAdLevelScript = g_ScriptArchive.GetPackedFile( _T("RubiconAD.ini") );
			if( pFreeAdLevelScript )
			{
				TCHAR buffer[128];
				memset( buffer, 0, sizeof(TCHAR) * 128 );
				
				if( fgets( buffer, 127, pFreeAdLevelScript ) )
				{
					sscanf( buffer, _T("%d"), &g_RubiconADFreeLevel );
				}
			}
		}

		g_ScriptArchive.ClosePackage();
	}
	//_XLog("Lev %d", g_RubiconADFreeLevel);


#endif
	

	TCHAR _resstring[64];
		
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strcpy(_resstring, _T("Cuu Long Tranh Ba"));
	}
	else
	{
		strcpy(_resstring, _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_TITLE));		

#ifdef _XTESTSERVER	
		if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
		{
			strcat(_resstring, _T("-수련서버") );
		}
#endif

	}

	SetTitle( _resstring );	

	//////////////////////////////////////////////////////////////////////////
	// Create charactor model stack
	_XLog("========================================================================" );

	if( g_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1,1) )
	{
		_XAnimMeshAbstractClass::m_UseVertexShaderAnimation = TRUE;

		_XLog( "shader version [ VS : %d.%d / PS : %d.%d ]", 
					D3DSHADER_VERSION_MAJOR(g_d3dCaps.VertexShaderVersion), D3DSHADER_VERSION_MINOR(g_d3dCaps.VertexShaderVersion),
					D3DSHADER_VERSION_MAJOR(g_d3dCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(g_d3dCaps.PixelShaderVersion) );	
		_XLog( "[ HCR ] Use vertex shader animation" );
	}
	else
	{
		_XAnimMeshAbstractClass::m_UseVertexShaderAnimation = FALSE;
		_XLog("shader version [ VS : %d.%d / PS : %d.%d ]", 
					D3DSHADER_VERSION_MAJOR(g_d3dCaps.VertexShaderVersion), D3DSHADER_VERSION_MINOR(g_d3dCaps.VertexShaderVersion),
					D3DSHADER_VERSION_MAJOR(g_d3dCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(g_d3dCaps.PixelShaderVersion) );	
		_XLog( "[ HCR ] Use cpu animation" );
	}
 
	BOOL buseshaderanimation =  g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USESHADERANIMATION,  TRUE, FALSE );
	if( _XAnimMeshAbstractClass::m_UseVertexShaderAnimation && !buseshaderanimation )
	{
		_XAnimMeshAbstractClass::m_UseVertexShaderAnimation = FALSE;
		_XLog( "[ HCR ] vertex shader animation disabled from registry" );
	}

	_XLog("========================================================================" );

	// by mahwang
	g_Userlist.CreatePool();
	g_Moblist.CreatePool();

	_XMEMORYUSECHECKSTART
	if( !CreateCharacterModelContainer() ) return FALSE;
	_XMEMORYUSECHECKEND( "CreateCharacterModelContainer" );

	_XMEMORYUSECHECKSTART
	g_XST_Wrapper.LoadTemplateData();
	_XMEMORYUSECHECKEND( "g_XST_Wrapper.LoadTemplateData" );

	_XHTMLWrapper::InitializeQHTMLibrary();

#ifdef _XDEF_HELPSYSTEMSERVERSAVE	
	SAFE_DELETE( g_TutorialInfo );
	g_TutorialInfo = new _XTUTORIALINFO;
#endif
	
	::SetForegroundWindow( gHWnd );
	::SetActiveWindow( gHWnd );
	SetFocus( gHWnd );
	
	return true;
}

void CEmperorOfDragonsApp::InitializeStringData( void )
{
	
#ifdef _XDEF_NEWSTRINGMANAGER
	g_JoinGroupName[0] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00);
	g_JoinGroupName[1] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01);
	g_JoinGroupName[2] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02);
	g_JoinGroupName[3] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03);
	g_JoinGroupName[4] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04);
	g_JoinGroupName[5] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05);
	g_JoinGroupName[6] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06);
	g_JoinGroupName[7] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_07);
	g_JoinGroupName[8] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_08);
	
	// 한글	

	g_LevelName[0] = _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR);
	g_LevelName[1] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_01);
	g_LevelName[2] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_02);
	g_LevelName[3] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_03);
	g_LevelName[4] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_04);
	g_LevelName[5] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_05);
	g_LevelName[6] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_06);
	g_LevelName[7] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_07);
	g_LevelName[8] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_08);
	g_LevelName[9] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_09);
	g_LevelName[10] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_10);
	g_LevelName[11] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_11);
	g_LevelName[12] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_12);
	g_LevelName[13] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_13);
	g_LevelName[14] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_14);
	g_LevelName[15] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_15);
	g_LevelName[16] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_16);
	g_LevelName[17] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_17);
	g_LevelName[18] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_18);
	g_LevelName[19] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_19);
	g_LevelName[20] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_20);	

	// 한자 - 18단계
	g_LevelName_CH[0] = _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR);
	g_LevelName_CH[1] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_01);
	g_LevelName_CH[2] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_02);
	g_LevelName_CH[3] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_03);
	g_LevelName_CH[4] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_04);
	g_LevelName_CH[5] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_05);
	g_LevelName_CH[6] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_06);
	g_LevelName_CH[7] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_07);
	g_LevelName_CH[8] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_08);
	g_LevelName_CH[9] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_09);
	g_LevelName_CH[10] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_10);
	g_LevelName_CH[11] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_11);
	g_LevelName_CH[12] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_12);
	g_LevelName_CH[13] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_13);
	g_LevelName_CH[14] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_14);
	g_LevelName_CH[15] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_15);
	g_LevelName_CH[16] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_16);
	g_LevelName_CH[17] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_17);
	g_LevelName_CH[18] = _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_18);
		
	g_stringExperienceSharingForm[0] = _XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONEXP);		//_T("경험치분배")
	g_stringExperienceSharingForm[1] = _XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONPRIVATE);	//_T("개인분배")
	g_stringExperienceSharingForm[2] = _XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONPUBLIC);		//_T("공동분배")

	g_stringItemSharingForm[0] = _XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONITEM);				//_T("전리품분배"),
	g_stringItemSharingForm[1] = _XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONPRIVATE);			//_T("개인분배"),
	g_stringItemSharingForm[2] = _XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONORDER);			//_T("순차분배"),
	g_stringItemSharingForm[3] = _XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONRANDOM);			//_T("임의분배")
		
	g_TargetFullName[0] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00);		//_T("낭인"),	 낭인 00		
	g_TargetFullName[1] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01);		//_T("개방"),			// 개방 01
	g_TargetFullName[2] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02);		//_T("비궁"),			// 비궁 02
	g_TargetFullName[3] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03);		//_T("소림사"),		// 소림 03
	g_TargetFullName[4] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04);		//_T("녹림맹"),		// 녹림 04
	g_TargetFullName[5] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05);		//_T("무당"),			// 무당 05
	g_TargetFullName[6] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06);		//_T("마교"),			// 마교 06
	g_TargetFullName[7] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_07);		//_T("세가연합"),		// 세가 07
	g_TargetFullName[8] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4);	//_T("없음"),			// 임시	08
	g_TargetFullName[9] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4);	//_T("없음"),			// 임시 09
	g_TargetFullName[10] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4);	//_T("없음"),			// 임시 10
	g_TargetFullName[11] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON5);	//_T("야수"),			// 늑대 / 혈랑 11
	g_TargetFullName[12] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON5);	//_T("야수"),			// 호랑이 12
	g_TargetFullName[13] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON5);	//_T("야수"),			// 곰 13
	g_TargetFullName[14] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON5);	//_T("야수"),			// 표범 14
	g_TargetFullName[15] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON5);	//_T("야수"),			// 거미/뱀/독사 15
	g_TargetFullName[16] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON5);	//_T("야수"),			// 여우 16
	g_TargetFullName[17] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON6);	//_T("외도세력"),		// 음화단 17	 
	g_TargetFullName[18] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON6);	//_T("외도세력"),		// 혈교 18
	g_TargetFullName[19] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON7);	//_T("망자"),			// 고루 19
	g_TargetFullName[20] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON8);	//_T("비영단"),		// 비영단 20
	g_TargetFullName[21] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON9);	//_T("사망곡"),		// 사망곡 21
	g_TargetFullName[22] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON10);	//_T("염왕곡"),		// 염왕곡 22
	g_TargetFullName[23] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON11);	//_T("남림맹"),		// 남림야수족 23
	g_TargetFullName[24] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON12);	//_T("금전방"),		// 금전방 24
	g_TargetFullName[25] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON13);	//_T("백성"),			// 백성  25
	g_TargetFullName[26] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON14);	//_T("가축"),			// 가축  26
	g_TargetFullName[27] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON15);	//_T("수련더미"),		// 수련더미  27
	g_TargetFullName[28] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON16);	//_T("?"),			// 수상한  28
	g_TargetFullName[29] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_1564);	// 암상인
	g_TargetFullName[30] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_1565);	//_T("무관관주")
	g_TargetFullName[31] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_1566);   //_T("관병")
	g_TargetFullName[32] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON6);	//_T("외도세력"),		// 무적 몬스터 32
	g_TargetFullName[33] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00);		//_T("낭인"),			// PK 낭인 33		
	g_TargetFullName[34] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01);		//_T("개방"),			// PK 개방 34
	g_TargetFullName[35] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02);		//_T("비궁"),			// PK 비궁 35
	g_TargetFullName[36] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03);		//_T("소림사"),			// PK 소림 36
	g_TargetFullName[37] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04);		//_T("녹림맹"),			// PK 녹림 37
	g_TargetFullName[38] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05);		//_T("무당"),			// PK 무당 38
	g_TargetFullName[39] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06);		//_T("마교"),			// PK 마교 39
	g_TargetFullName[40] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4);	//_T("없음"),			// 임시	40
	g_TargetFullName[41] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4);	//_T("없음"),			// 임시 41
	g_TargetFullName[42] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4);	//_T("없음"),			// 임시 42
	g_TargetFullName[43] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON16);	//_T("?"),			// PK 수상한  43
	g_TargetFullName[44] = _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4);	//_T("없음"),			// 임시 44
	g_TargetFullName[45] = _XGETINTERFACETEXT(ID_STRING_NEW_2616);				// 토기병 45
	g_TargetFullName[46] = _XGETINTERFACETEXT(ID_STRING_NEW_2970);				// 46				// 욱일승천류
	g_TargetFullName[47] = _XGETINTERFACETEXT(ID_STRING_NEW_3418);				// 47 북해빙궁
	g_TargetFullName[48] = _XGETINTERFACETEXT(ID_STRING_NEW_3419);				// 48 북해설인
	g_TargetFullName[49] = _XGETINTERFACETEXT(ID_STRING_NEW_3420);				// 49 북해동물
	g_TargetFullName[50] = _XGETINTERFACETEXT(ID_STRING_NEW_3484);				// 50 추적자
	
	g_UserStatus[0]	= _XGETINTERFACETEXT(ID_STRING_MESSENGER_1567); // _T("오프라인"));
	g_UserStatus[1]	= _XGETINTERFACETEXT(ID_STRING_MESSENGER_1568); // _T("온라인") );
	g_UserStatus[2]	= _XGETINTERFACETEXT(ID_STRING_MESSENGER_1569); // _T("전투 중") );	
	g_UserStatus[3]	= _XGETINTERFACETEXT(ID_STRING_MESSENGER_ABSENCE);
	g_UserStatus[4]	= _XGETINTERFACETEXT(ID_STRING_MESSENGER_MOVESERVER);	

	g_SunMoonIconText[0] = _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_DAY);
	g_SunMoonIconText[1] = _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_NIGHTFALL);
	g_SunMoonIconText[2] = _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_NIGHT);
	g_SunMoonIconText[3] = _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_DAWN);
	
	g_QuestGroupNameString[0] = _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAME1);
	g_QuestGroupNameString[1] = _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAME2);
	g_QuestGroupNameString[2] = _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAME3);
	g_QuestGroupNameString[3] = _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAM4);
	g_QuestGroupNameString[4] = _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAME5);
	g_QuestGroupNameString[5] = _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAME6);
	g_QuestGroupNameString[6] = _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAME7);

//#ifdef _XDEF_CASHITEM_SHOUT
	g_ChatFunctionButtonStringID[0] = ID_STRING_CHATTINGDEFAULT_EMOTION;	// _T("감정표현")
	g_ChatFunctionButtonStringID[1] = ID_STRING_NEW_2955;					// _T("외치기(%)")
	g_ChatFunctionButtonStringID[2] = ID_STRING_CHATTINGDEFAULT_GROUP;		// _T("문파(#)")
	g_ChatFunctionButtonStringID[3] = ID_STRING_CHATTINGDEFAULT_BANGHOI;	// _T("영웅단(@)")
	g_ChatFunctionButtonStringID[4] = ID_STRING_CHATTINGDEFAULT_PARTY;		// _T("동행(!)")
	g_ChatFunctionButtonStringID[5] = ID_STRING_CHATTINGDEFAULT_WHISPER;	// _T("귓속말(/)")
	g_ChatFunctionButtonStringID[6] = ID_STRING_NEW_3216;					// _T("지역대화(*)")
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	g_ChatFunctionButtonStringID[7] = ID_STRING_NEW_3421 ;					// _T("일반")
#else	
	g_ChatFunctionButtonStringID[7] = ID_STRING_CHATTINGDEFAULT_TOTAL;		// _T("전체")	
#endif

//#else
//	g_ChatFunctionButtonStringID[0] = ID_STRING_CHATTINGDEFAULT_EMOTION;	// _T("감정표현")
//	g_ChatFunctionButtonStringID[1] = ID_STRING_CHATTINGDEFAULT_GROUP;		// _T("문파(#)")
//	g_ChatFunctionButtonStringID[2] = ID_STRING_CHATTINGDEFAULT_BANGHOI;	// _T("영웅단(@)")
//	g_ChatFunctionButtonStringID[3] = ID_STRING_CHATTINGDEFAULT_PARTY;		// _T("동행(!)")
//	g_ChatFunctionButtonStringID[4] = ID_STRING_CHATTINGDEFAULT_WHISPER;	// _T("귓속말(/)")
//	g_ChatFunctionButtonStringID[5] = ID_STRING_CHATTINGDEFAULT_TOTAL;		// _T("전체")
//#endif

	g_ChatEmotionString[0] = _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO1);	// _T("손인사")
	g_ChatEmotionString[1] = _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO2);	// _T("포  권")
	g_ChatEmotionString[2] = _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO3);	// _T("도  발")
	g_ChatEmotionString[3] = _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO4);	// _T("전  진")
	g_ChatEmotionString[4] = _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO5);	// _T("앉  기")
	g_ChatEmotionString[5] = _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO6);	// _T("환호성")
	g_ChatEmotionString[6] = _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO7);	// _T("실  망")
	g_ChatEmotionString[7] = _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO8);	// _T("울  음")
	g_ChatEmotionString[8] = _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO9);	// _T("웃  음")
	g_ChatEmotionString[9] = _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO10);	// _T("분  노")
	g_ChatEmotionString[10] = _XGETINTERFACETEXT(ID_STRING_NEW_2617);	
	g_ChatEmotionString[11] = _XGETINTERFACETEXT(ID_STRING_NEW_2618);
	g_ChatEmotionString[12] = _XGETINTERFACETEXT(ID_STRING_NEW_2619);

	g_pClothesItemClanName[0] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00);			//_T("낭인") );
	g_pClothesItemClanName[1] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01);			//_T("개방") );
	g_pClothesItemClanName[2] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02);			//_T("비궁") );
	g_pClothesItemClanName[3] = _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1788);	//_T("소림") );
	g_pClothesItemClanName[4] = _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1790);	//_T("녹림") );
	g_pClothesItemClanName[5] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05);			//_T("무당") );
	g_pClothesItemClanName[6] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06);			//_T("마교") );
	g_pClothesItemClanName[7] = _XGETINTERFACETEXT(ID_STRING_PARTY_SEGA);				//_T("세가") );
	g_pClothesItemClanName[8] = _XGETINTERFACETEXT(ID_STRING_GROUPNAME_08);			//_T("미정") );

	g_pClothesItemSecondName[0] = _XGETINTERFACETEXT(ID_STRING_CINFO_UPPER);			//_T("상의") );
	g_pClothesItemSecondName[1] = _XGETINTERFACETEXT(ID_STRING_CINFO_LOWER);			//_T("하의") );
	g_pClothesItemSecondName[2] = _XGETINTERFACETEXT(ID_STRING_CINFO_GROVES);			//_T("아대") );
	g_pClothesItemSecondName[3] = _XGETINTERFACETEXT(ID_STRING_CINFO_SHOES);			//_T("신발") );
	g_pClothesItemSecondName[4] = _XGETINTERFACETEXT(ID_STRING_CINFO_CAP);				//_T("모자") );
	g_pClothesItemSecondName[5] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2305);	//_T("흉갑") );
	g_pClothesItemSecondName[6] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2306);	//_T("신갑") );
	g_pClothesItemSecondName[7] = _XGETINTERFACETEXT(ID_STRING_CINFO_BACKPACK);		//_T("배낭") );	
	
	g_BillingStatusString[0] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2307);			//_T("개인 사용자") );
	g_BillingStatusString[1] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2308);			//_T("피씨방 사용자") );
	g_BillingStatusString[2] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2309);			//_T("ISP 사용자") );
	g_BillingStatusString[3] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2310);			//_T("프리미엄 사용자") );	

	g_BillingPaymentTypeString[0] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2311);	//_T("후불") );	
	g_BillingPaymentTypeString[1] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2312);	//_T("직불") );	
	g_BillingPaymentTypeString[2] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2313);	//_T("선불	") );	

	g_BillingMethodString[0] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2314);			//_T("월정액") );	
	g_BillingMethodString[1] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2315);			//_T("주말 월 정액") );	
	
	g_HonorName[0] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2316);			//_T("천하삼괴") );
	g_HonorName[1] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2317);			//_T("무림괴협") );
	g_HonorName[2] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2318);			//_T("극악서생") );
	g_HonorName[3] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2319);			//_T("무명") );
	g_HonorName[4] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2320);			//_T("소협") );
	g_HonorName[5] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2321);			//_T("대협") );	
	g_HonorName[6] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2322);			//_T("협객") );
	g_HonorName[7] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2323);			//_T("중원제일협객") );
	g_HonorName[8] = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2324);			//_T("천하영웅") );

	g_ZoneExplaneString[0] = _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ABOUTSORIM);
	g_ZoneExplaneString[1] = _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ABOUTGAEBANG);
	g_ZoneExplaneString[2] = _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ABOUTNOCKRIM);
	g_ZoneExplaneString[3] = _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ABOUTBEEGOONG);
	g_ZoneExplaneString[4] = _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_2384);
	g_ZoneExplaneString[5] = _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_2385);
	
	g_szServerNameString[0] = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER1);
	g_szServerNameString[1] = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER2);
	g_szServerNameString[2] = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER3);
	g_szServerNameString[3] = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER4);
	g_szServerNameString[4] = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER7);
	g_szServerNameString[5] = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER5);
	g_szServerNameString[6] = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER6);
	g_szServerNameString[7] = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1795);
	g_szServerNameString[8] = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1796);
	g_szServerNameString[9] = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1797);
	g_szServerNameString[10] = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1798);
	g_szServerNameString[11] = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1799);
#ifdef _XTS_NEWCHARACLIST
#ifndef _XTESTSERVER
	g_szServerNameString[12] = _XGETINTERFACETEXT(ID_STRING_NEW_2949);
	g_szServerNameString[13] = _XGETINTERFACETEXT(ID_STRING_NEW_2950);
	g_szServerNameString[14] = _XGETINTERFACETEXT(ID_STRING_NEW_2951);
	g_szServerNameString[15] = _XGETINTERFACETEXT(ID_STRING_NEW_2976);
	for( int i = 16; i < _COUNT_MAX_GROUP_; ++i )
		g_szServerNameString[i] = _XGETINTERFACETEXT(-1);
	g_szServerNameString[_GR_INDEX_UNION_]	= _XGETINTERFACETEXT(ID_STRING_NEW_2952);
#endif
#else
	g_szServerNameString[12] = _XGETINTERFACETEXT(-1);
	g_szServerNameString[13] = _XGETINTERFACETEXT(-1);
	g_szServerNameString[14] = _XGETINTERFACETEXT(-1);
	g_szServerNameString[15] = _XGETINTERFACETEXT(-1);
	g_szServerNameString[16] = _XGETINTERFACETEXT(-1);
	g_szServerNameString[17] = _XGETINTERFACETEXT(-1);
#endif

	g_pSkillUsefulTypeName[0] = _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_2011);
	g_pSkillUsefulTypeName[1] = _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ACTIVESKILL);	
	g_pSkillUsefulTypeName[2] = _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CHARGESKILL);
	g_pSkillUsefulTypeName[3] = _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_2011);
	g_pSkillUsefulTypeName[4] = _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CASTINGSPELL);
	g_pSkillUsefulTypeName[5] = _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CHANNELINGSPELL);
	g_pSkillUsefulTypeName[6] = _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FINISHSKILL);
	g_pSkillUsefulTypeName[7] = _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ROUNDINGSPELL);
	g_pSkillUsefulTypeName[8] = _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA);
	g_pSkillUsefulTypeName[9] = _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SMASHINGSKILL);
	g_pSkillUsefulTypeName[10]= _XGETINTERFACETEXT(ID_STRING_NEW_2620);


#else
	strncpy(g_JoinGroupName[0], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00), 15);
	strncpy(g_JoinGroupName[1], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01), 15);
	strncpy(g_JoinGroupName[2], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02), 15);
	strncpy(g_JoinGroupName[3], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03), 15);
	strncpy(g_JoinGroupName[4], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04), 15);
	strncpy(g_JoinGroupName[5], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05), 15);
	strncpy(g_JoinGroupName[6], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06), 15);
	strncpy(g_JoinGroupName[7], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_07), 15);
	strncpy(g_JoinGroupName[8], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_08), 15);
	
	// 한글
	for( int i = 0; i < _XDEF_TOTALLEVELNAME; ++i ) memset( g_LevelName[i], 0, sizeof(TCHAR) * 16 );

	strncpy(g_LevelName[0], _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR), 15);
	strncpy(g_LevelName[1], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_01), 15);
	strncpy(g_LevelName[2], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_02), 15);
	strncpy(g_LevelName[3], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_03), 15);
	strncpy(g_LevelName[4], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_04), 15);
	strncpy(g_LevelName[5], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_05), 15);
	strncpy(g_LevelName[6], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_06), 15);
	strncpy(g_LevelName[7], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_07), 15);
	strncpy(g_LevelName[8], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_08), 15);
	strncpy(g_LevelName[9], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_09), 15);
	strncpy(g_LevelName[10], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_10), 15);
	strncpy(g_LevelName[11], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_11), 15);
	strncpy(g_LevelName[12], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_12), 15);
	strncpy(g_LevelName[13], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_13), 15);
	strncpy(g_LevelName[14], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_14), 15);
	strncpy(g_LevelName[15], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_15), 15);
	strncpy(g_LevelName[16], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_16), 15);
	strncpy(g_LevelName[17], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_17), 15);
	strncpy(g_LevelName[18], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_18), 15);
	strncpy(g_LevelName[19], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_19), 15);
	strncpy(g_LevelName[20], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_20), 15);	

	// 한자 - 18단계
	for( i = 0; i < _XDEF_TOTALLEVELNAME; ++i ) memset( g_LevelName_CH[i], 0, sizeof(TCHAR) * 16 );
	
	strncpy(g_LevelName_CH[0], _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR), 15);
	strncpy(g_LevelName_CH[1], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_01), 15);
	strncpy(g_LevelName_CH[2], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_02), 15);
	strncpy(g_LevelName_CH[3], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_03), 15);
	strncpy(g_LevelName_CH[4], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_04), 15);
	strncpy(g_LevelName_CH[5], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_05), 15);
	strncpy(g_LevelName_CH[6], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_06), 15);
	strncpy(g_LevelName_CH[7], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_07), 15);
	strncpy(g_LevelName_CH[8], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_08), 15);
	strncpy(g_LevelName_CH[9], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_09), 15);
	strncpy(g_LevelName_CH[10], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_10), 15);
	strncpy(g_LevelName_CH[11], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_11), 15);
	strncpy(g_LevelName_CH[12], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_12), 15);
	strncpy(g_LevelName_CH[13], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_13), 15);
	strncpy(g_LevelName_CH[14], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_14), 15);
	strncpy(g_LevelName_CH[15], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_15), 15);
	strncpy(g_LevelName_CH[16], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_16), 15);
	strncpy(g_LevelName_CH[17], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_17), 15);
	strncpy(g_LevelName_CH[18], _XGETINTERFACETEXT(ID_STRING_LEVELNAME_CH_18), 15);
	
	memset( g_stringExperienceSharingForm, 0, sizeof(TCHAR) * 5 * 32 );
	strcpy(g_stringExperienceSharingForm[0],_XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONEXP) );		//_T("경험치분배"),
	strcpy(g_stringExperienceSharingForm[1],_XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONPRIVATE) );	//_T("개인분배"),
	strcpy(g_stringExperienceSharingForm[2],_XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONPUBLIC) );		//_T("공동분배")

	memset( g_stringItemSharingForm, 0, sizeof(TCHAR) * 5 * 32 );

	strcpy(g_stringItemSharingForm[0],_XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONITEM) );		//_T("전리품분배"),
	strcpy(g_stringItemSharingForm[1],_XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONPRIVATE) );	//_T("개인분배"),
	strcpy(g_stringItemSharingForm[2],_XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONORDER) );		//_T("순차분배"),
	strcpy(g_stringItemSharingForm[3],_XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONRANDOM) );		//_T("임의분배")

	memset( g_TargetFullName, 0, sizeof(TCHAR) * 47 * 16 );
	
	strcpy( g_TargetFullName[0], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00) );		//_T("낭인"),	 낭인 00		
	strcpy( g_TargetFullName[1], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01) );		//_T("개방"),			// 개방 01
	strcpy( g_TargetFullName[2], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02) );		//_T("비궁"),			// 비궁 02
	strcpy( g_TargetFullName[3], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03) );		//_T("소림사"),		// 소림 03
	strcpy( g_TargetFullName[4], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04) );		//_T("녹림맹"),		// 녹림 04
	strcpy( g_TargetFullName[5], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05) );		//_T("무당"),			// 무당 05
	strcpy( g_TargetFullName[6], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06) );		//_T("마교"),			// 마교 06
	strcpy( g_TargetFullName[7], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_07) );		//_T("세가연합"),		// 세가 07
	strcpy( g_TargetFullName[8], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4) );	//_T("없음"),			// 임시	08
	strcpy( g_TargetFullName[9], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4) );	//_T("없음"),			// 임시 09
	strcpy( g_TargetFullName[10], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4) );	//_T("없음"),			// 임시 10
	strcpy( g_TargetFullName[11], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON5) );	//_T("야수"),			// 늑대 / 혈랑 11
	strcpy( g_TargetFullName[12], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON5) );	//_T("야수"),			// 호랑이 12
	strcpy( g_TargetFullName[13], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON5) );	//_T("야수"),			// 곰 13
	strcpy( g_TargetFullName[14], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON5) );	//_T("야수"),			// 표범 14
	strcpy( g_TargetFullName[15], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON5) );	//_T("야수"),			// 거미/뱀/독사 15
	strcpy( g_TargetFullName[16], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON5) );	//_T("야수"),			// 여우 16
	strcpy( g_TargetFullName[17], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON6) );	//_T("외도세력"),		// 음화단 17	 
	strcpy( g_TargetFullName[18], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON6) );	//_T("외도세력"),		// 혈교 18
	strcpy( g_TargetFullName[19], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON7) );	//_T("망자"),			// 고루 19
	strcpy( g_TargetFullName[20], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON8) );	//_T("비영단"),		// 비영단 20
	strcpy( g_TargetFullName[21], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON9) );	//_T("사망곡"),		// 사망곡 21
	strcpy( g_TargetFullName[22], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON10) );	//_T("염왕곡"),		// 염왕곡 22
	strcpy( g_TargetFullName[23], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON11) );	//_T("남림맹"),		// 남림야수족 23
	strcpy( g_TargetFullName[24], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON12) );	//_T("금전방"),		// 금전방 24
	strcpy( g_TargetFullName[25], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON13) );	//_T("백성"),			// 백성  25
	strcpy( g_TargetFullName[26], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON14) );	//_T("가축"),			// 가축  26
	strcpy( g_TargetFullName[27], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON15) );	//_T("수련더미"),		// 수련더미  27
	strcpy( g_TargetFullName[28], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON16) );	//_T("?"),			// 수상한  28
	strcpy( g_TargetFullName[29], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_1564) );	// 암상인
	strcpy( g_TargetFullName[30], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_1565) );	//_T("무관관주")
	strcpy( g_TargetFullName[31], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_1566) );   //_T("관병")
	strcpy( g_TargetFullName[32], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON6) );	//_T("외도세력"),		// 무적 몬스터 32
	strcpy( g_TargetFullName[33], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00) );		//_T("낭인"),			// PK 낭인 33		
	strcpy( g_TargetFullName[34], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01) );		//_T("개방"),			// PK 개방 34
	strcpy( g_TargetFullName[35], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02) );		//_T("비궁"),			// PK 비궁 35
	strcpy( g_TargetFullName[36], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03) );		//_T("소림사"),			// PK 소림 36
	strcpy( g_TargetFullName[37], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04) );		//_T("녹림맹"),			// PK 녹림 37
	strcpy( g_TargetFullName[38], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05) );		//_T("무당"),			// PK 무당 38
	strcpy( g_TargetFullName[39], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06) );		//_T("마교"),			// PK 마교 39
	strcpy( g_TargetFullName[40], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4) );	//_T("없음"),			// 임시	40
	strcpy( g_TargetFullName[41], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4) );	//_T("없음"),			// 임시 41
	strcpy( g_TargetFullName[42], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4) );	//_T("없음"),			// 임시 42
	strcpy( g_TargetFullName[43], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON16) );	//_T("?"),			// PK 수상한  43
	strcpy( g_TargetFullName[44], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_MON4) );	//_T("없음"),			// 임시 44
	strcpy( g_TargetFullName[45], _XGETINTERFACETEXT(ID_STRING_NEW_2616) );	//_T("토기병"),				// 토기병 45
	strcpy( g_TargetFullName[46], _XGETINTERFACETEXT(ID_STRING_NEW_2970) );	//46	욱일승천류
	strcpy( g_TargetFullName[47], _XGETINTERFACETEXT(ID_STRING_NEW_3418) );	//47	북해빙궁
	strcpy( g_TargetFullName[48], _XGETINTERFACETEXT(ID_STRING_NEW_3419) );	//48	북해설인
	strcpy( g_TargetFullName[49], _XGETINTERFACETEXT(ID_STRING_NEW_3420) );	//49	북해동물
	strcpy( g_TargetFullName[50], _XGETINTERFACETEXT(ID_STRING_NEW_3484) );	//50	추적자

	
	memset( g_UserStatus, 0, sizeof(TCHAR) * 5 * 20 );
	strcpy( g_UserStatus[0], _XGETINTERFACETEXT(ID_STRING_MESSENGER_1567) ); // _T("오프라인"));
	strcpy( g_UserStatus[1], _XGETINTERFACETEXT(ID_STRING_MESSENGER_1568) ); // _T("온라인") );
	strcpy( g_UserStatus[2], _XGETINTERFACETEXT(ID_STRING_MESSENGER_1569) ); // _T("전투 중") );	
	strcpy( g_UserStatus[3], _XGETINTERFACETEXT(ID_STRING_MESSENGER_ABSENCE) );
	strcpy( g_UserStatus[4], _XGETINTERFACETEXT(ID_STRING_MESSENGER_MOVESERVER) );	

	memset( g_SunMoonIconText, 0, sizeof(TCHAR) * 4 * 16 );
	strcpy( g_SunMoonIconText[0], _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_DAY) );
	strcpy( g_SunMoonIconText[1], _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_NIGHTFALL) );
	strcpy( g_SunMoonIconText[2], _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_NIGHT) );
	strcpy( g_SunMoonIconText[3], _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_DAWN) );	

	memset( g_QuestGroupNameString, 0, sizeof(TCHAR) * 30 * 16 );
	strcpy( g_QuestGroupNameString[0], _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAME1) );
	strcpy( g_QuestGroupNameString[1], _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAME2) );
	strcpy( g_QuestGroupNameString[2], _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAME3) );
	strcpy( g_QuestGroupNameString[3], _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAM4) );	
	strcpy( g_QuestGroupNameString[4], _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAME5) );
	strcpy( g_QuestGroupNameString[5], _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAME6) );
	strcpy( g_QuestGroupNameString[6], _XGETINTERFACETEXT(ID_STRING_QUEST_GROUPNAME7) );

	memset( g_ChatFunctionButtonString, 0, sizeof(TCHAR) * 6 * 16 );
	strcpy( g_ChatFunctionButtonString[0], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMOTION) );	// _T("감정표현")
	strcpy( g_ChatFunctionButtonString[1], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_GROUP) );	// _T("문파(#)")
	strcpy( g_ChatFunctionButtonString[2], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_BANGHOI) );	// _T("영웅단(@)")
	strcpy( g_ChatFunctionButtonString[3], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_PARTY) );	// _T("동행(!)")
	strcpy( g_ChatFunctionButtonString[4], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_WHISPER) );	// _T("귓속말(/)")

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	strcpy( g_ChatFunctionButtonString[5], _XGETINTERFACETEXT(ID_STRING_NEW_3421) );				// _T("일반")
#else
	strcpy( g_ChatFunctionButtonString[5], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_TOTAL) );	// _T("전체")
#endif

	memset( g_ChatEmotionString, 0, sizeof(TCHAR) * 13 * 32 );
	strcpy( g_ChatEmotionString[0], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO1) );	// _T("손인사")
	strcpy( g_ChatEmotionString[1], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO2) );	// _T("포  권")
	strcpy( g_ChatEmotionString[2], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO3) );	// _T("도  발")
	strcpy( g_ChatEmotionString[3], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO4) );	// _T("전  진")
	strcpy( g_ChatEmotionString[4], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO5) );	// _T("앉  기")
	strcpy( g_ChatEmotionString[5], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO6) );	// _T("환호성")
	strcpy( g_ChatEmotionString[6], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO7) );	// _T("실  망")
	strcpy( g_ChatEmotionString[7], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO8) );	// _T("울  음")
	strcpy( g_ChatEmotionString[8], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO9) );	// _T("웃  음")
	strcpy( g_ChatEmotionString[9], _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_EMO10) );	// _T("분  노")
	strcpy( g_ChatEmotionString[10], _XGETINTERFACETEXT(ID_STRING_NEW_2617) );	// _T("박  수")
	strcpy( g_ChatEmotionString[11], _XGETINTERFACETEXT(ID_STRING_NEW_2618) ); //_T("인  사")
	strcpy( g_ChatEmotionString[12], _XGETINTERFACETEXT(ID_STRING_NEW_2619) ); //_T("큰  절")

	memset( g_pClothesItemClanName, 0, sizeof(TCHAR) * 9 * 16 );

	strcpy( g_pClothesItemClanName[0], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00) );			//_T("낭인") );
	strcpy( g_pClothesItemClanName[1], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01) );			//_T("개방") );
	strcpy( g_pClothesItemClanName[2], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02) );			//_T("비궁") );
	strcpy( g_pClothesItemClanName[3], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1788) );	//_T("소림") );
	strcpy( g_pClothesItemClanName[4], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1790) );	//_T("녹림") );
	strcpy( g_pClothesItemClanName[5], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05) );			//_T("무당") );
	strcpy( g_pClothesItemClanName[6], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06) );			//_T("마교") );
	strcpy( g_pClothesItemClanName[7], _XGETINTERFACETEXT(ID_STRING_PARTY_SEGA) );				//_T("세가") );
	strcpy( g_pClothesItemClanName[8], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_08) );			//_T("미정") );

	memset( g_pClothesItemSecondName, 0, sizeof(TCHAR) * 8 * 16 );

	strcpy( g_pClothesItemSecondName[0], _XGETINTERFACETEXT(ID_STRING_CINFO_UPPER) );			//_T("상의") );
	strcpy( g_pClothesItemSecondName[1], _XGETINTERFACETEXT(ID_STRING_CINFO_LOWER) );			//_T("하의") );
	strcpy( g_pClothesItemSecondName[2], _XGETINTERFACETEXT(ID_STRING_CINFO_GROVES) );			//_T("아대") );
	strcpy( g_pClothesItemSecondName[3], _XGETINTERFACETEXT(ID_STRING_CINFO_SHOES) );			//_T("신발") );
	strcpy( g_pClothesItemSecondName[4], _XGETINTERFACETEXT(ID_STRING_CINFO_CAP) );				//_T("모자") );
	strcpy( g_pClothesItemSecondName[5], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2305) );	//_T("흉갑") );
	strcpy( g_pClothesItemSecondName[6], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2306) );	//_T("신갑") );
	strcpy( g_pClothesItemSecondName[7], _XGETINTERFACETEXT(ID_STRING_CINFO_BACKPACK) );		//_T("배낭") );	
	
	strcpy( g_BillingStatusString[0], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2307) );			//_T("개인 사용자") );
	strcpy( g_BillingStatusString[1], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2308) );			//_T("피씨방 사용자") );
	strcpy( g_BillingStatusString[2], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2309) );			//_T("ISP 사용자") );
	strcpy( g_BillingStatusString[3], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2310) );			//_T("프리미엄 사용자") );	

	strcpy( g_BillingPaymentTypeString[0], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2311) );			//_T("후불") );	
	strcpy( g_BillingPaymentTypeString[1], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2312) );			//_T("직불") );	
	strcpy( g_BillingPaymentTypeString[2], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2313) );			//_T("선불	") );	

	strcpy( g_BillingMethodString[0], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2314) );			//_T("월정액") );	
	strcpy( g_BillingMethodString[1], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2315) );			//_T("주말 월 정액") );	
	
	memset( g_HonorName, 0, sizeof(TCHAR) * 9 * 16 );

	strcpy( g_HonorName[0], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2316) );			//_T("천하삼괴") );
	strcpy( g_HonorName[1], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2317) );			//_T("무림괴협") );
	strcpy( g_HonorName[2], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2318) );			//_T("극악서생") );
	strcpy( g_HonorName[3], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2319) );			//_T("무명") );
	strcpy( g_HonorName[4], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2320) );			//_T("소협") );
	strcpy( g_HonorName[5], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2321) );			//_T("대협") );	
	strcpy( g_HonorName[6], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2322) );			//_T("협객") );
	strcpy( g_HonorName[7], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2323) );			//_T("중원제일협객") );
	strcpy( g_HonorName[8], _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2324) );			//_T("천하영웅") );

	memset( g_ZoneExplaneString, 0, sizeof(TCHAR) * 9 * 256 );

	strcpy( g_ZoneExplaneString[0], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ABOUTSORIM) );	
	strcpy( g_ZoneExplaneString[1], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ABOUTGAEBANG) );
	strcpy( g_ZoneExplaneString[2], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ABOUTNOCKRIM) );
	strcpy( g_ZoneExplaneString[3], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ABOUTBEEGOONG) );
	strcpy( g_ZoneExplaneString[4], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_2384) );
	strcpy( g_ZoneExplaneString[5], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_2385) );

	memset( g_szServerNameString, 0, sizeof(TCHAR)*18*32);

	strcpy( g_szServerNameString[0], _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER1) );
	strcpy( g_szServerNameString[1], _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER2) );
	strcpy( g_szServerNameString[2], _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER3) );
	strcpy( g_szServerNameString[3], _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER4) );
	strcpy( g_szServerNameString[4], _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER7) );
	strcpy( g_szServerNameString[5], _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER5) );
	strcpy( g_szServerNameString[6], _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER6) );
	strcpy( g_szServerNameString[7], _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1795) );
	strcpy( g_szServerNameString[8], _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1796) );
	strcpy( g_szServerNameString[9], _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1797) );

	strcpy( g_szServerNameString[10], _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1798) );
	strcpy( g_szServerNameString[11], _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1799) );
	strcpy( g_szServerNameString[12], _T("Server 13") );
	strcpy( g_szServerNameString[13], _T("Server 14") );
	strcpy( g_szServerNameString[14], _T("Server 15") );
	strcpy( g_szServerNameString[15], _T("Server 16") );
	strcpy( g_szServerNameString[16], _T("Server 17") );
	strcpy( g_szServerNameString[17], _T("Server 18") );

	memset( g_pSkillUsefulTypeName, 0, sizeof(TCHAR)*11*16);
	
	strcpy( g_pSkillUsefulTypeName[0], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_2011) );
	strcpy( g_pSkillUsefulTypeName[1], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ACTIVESKILL) );
	strcpy( g_pSkillUsefulTypeName[2], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CHARGESKILL) );
	strcpy( g_pSkillUsefulTypeName[3], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_2011) );
	strcpy( g_pSkillUsefulTypeName[4], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CASTINGSPELL) );
	strcpy( g_pSkillUsefulTypeName[5], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CHANNELINGSPELL) );
	strcpy( g_pSkillUsefulTypeName[6], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FINISHSKILL) );
	strcpy( g_pSkillUsefulTypeName[7], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ROUNDINGSPELL) );
	strcpy( g_pSkillUsefulTypeName[8], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDENMA) );
	strcpy( g_pSkillUsefulTypeName[9], _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SMASHINGSKILL) );
	strcpy( g_pSkillUsefulTypeName[10], _XGETINTERFACETEXT(ID_STRING_NEW_2620) );	// _T("무림잡기")

#endif
		
}

HRESULT	CEmperorOfDragonsApp::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior, D3DFORMAT backBufferFormat )
{
    if( dwBehavior & D3DCREATE_PUREDEVICE )
        return E_FAIL; // GetTransform doesn't work on PUREDEVICE
	
	return S_OK;
}

bool CEmperorOfDragonsApp::CreateCharacterModelContainer( void )
{
	int i;
	
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_FACE; ++i )  g_UserModel_FaceType[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_UBODY; ++i ) g_UserModel_UBodyType[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_LBODY; ++i ) g_UserModel_LBodyType[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_LHAND; ++i ) g_UserModel_LHandType[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_RHAND; ++i ) g_UserModel_RHandType[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_LFOOT; ++i ) g_UserModel_LFootType[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_RFOOT; ++i ) g_UserModel_RFootType[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_CAP; ++i ) g_UserModel_Cap[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_ADAE; ++i ) g_UserModel_Adae[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_EARRING; ++i ) g_UserModel_Earring[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_BACKPACK; ++i ) g_UserModel_BackPack[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();	
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_HAIR; ++i ) g_UserModel_HairType[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();

	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_MASK; ++i ) g_UserModel_Mask[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_FACEMASK; ++i ) g_UserModel_FaceMask[i] = _XAnimMeshAbstractClass::CreateAnimationMesh();	
	
	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_WEAPON; ++i ) 
	{
		g_ItemModel_Weapon[0][i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
		g_ItemModel_Weapon[1][i] = _XAnimMeshAbstractClass::CreateAnimationMesh();
	}

	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_MARTIALBOOK; ++i ) g_ItemModel_MartialBook[i] = new _XNSAnimMeshContainer;
	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_POTION; ++i ) g_ItemModel_Potion[i] = new _XNSAnimMeshContainer;
	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_THROW; ++i ) g_ItemModel_Throw[i] = new _XNSAnimMeshContainer;
	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_SOCERY; ++i ) g_ItemModel_Socery[i] = new _XNSAnimMeshContainer;
	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_ETC; ++i ) g_ItemModel_ETC[i] = new _XNSAnimMeshContainer;
	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_JEWELBOX; ++i ) g_ItemModel_JewelBox[i] = new _XNSAnimMeshContainer;
	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_ELIXIR; ++i) g_ItemModel_Elixir[i] = new _XNSAnimMeshContainer;
		
	g_Moblist.CreateData();
	g_Npclist.CreateData();
	
	return true;
}

bool CEmperorOfDragonsApp::InitDeviceObjects()
{	
	// Be sure that all is free	
	_XLog( "Initialize device object" );

	if( !gpDev ) return false;

	// Load device indefendent mouse cursor....
	g_hGameCursor[_XMOUSETYPE_PEACE					] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_PEACE ) );
	g_hGameCursor[_XMOUSETYPE_PEACE_CLICK			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_PEACE_CLICK ) );
	g_hGameCursor[_XMOUSETYPE_BATTLE				] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_BATTLE ) );
	g_hGameCursor[_XMOUSETYPE_BATTLE_CLICK			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_BATTLE_CLICK ) );
	g_hGameCursor[_XMOUSETYPE_SELECT				] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SELECT ) );
	g_hGameCursor[_XMOUSETYPE_SELECT_CLICK			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SELECT_CLICK ) );
	g_hGameCursor[_XMOUSETYPE_PICKUP				] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_PICKUP ) );
	g_hGameCursor[_XMOUSETYPE_PICKUP_CLICK			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_PICKUP_SHUT ) );
	g_hGameCursor[_XMOUSETYPE_RESIZE_DIAGONAL_LEFT	] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_RESIZE_DIAGONAL_LEFT ) );
	g_hGameCursor[_XMOUSETYPE_RESIZE_DIAGONAL_RIGHT	] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_RESIZE_DIAGONAL_RIGHT ) );
	g_hGameCursor[_XMOUSETYPE_RESIZE_HORIZ			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_RESIZE_HORIZ ) );
	g_hGameCursor[_XMOUSETYPE_RESIZE_VERT			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_RESIZE_VERT ) );
	g_hGameCursor[_XMOUSETYPE_SANDGLASS1			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SANDGLASS1 ) );
	g_hGameCursor[_XMOUSETYPE_SANDGLASS2			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SANDGLASS1 ) );
	g_hGameCursor[_XMOUSETYPE_SANDGLASS3			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SANDGLASS1 ) );
	g_hGameCursor[_XMOUSETYPE_SANDGLASS4			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SANDGLASS1 ) );
	g_hGameCursor[_XMOUSETYPE_SANDGLASS5			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SANDGLASS1 ) );
	g_hGameCursor[_XMOUSETYPE_SANDGLASS6			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SANDGLASS1 ) );
	
	g_hGameCursor[_XMOUSETYPE_SCRSCROLL_UPLEFT		] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SCRSCROLL_UPLEFT ) );
	g_hGameCursor[_XMOUSETYPE_SCRSCROLL_UP			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SCRSCROLL_UP ) );
	g_hGameCursor[_XMOUSETYPE_SCRSCROLL_UPRIGHT		] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SCRSCROLL_UPRIGHT ) );
	g_hGameCursor[_XMOUSETYPE_SCRSCROLL_RIGHT		] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SCRSCROLL_RIGHT ) );
	g_hGameCursor[_XMOUSETYPE_SCRSCROLL_DOWNRIGHT	] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SCRSCROLL_DOWNRIGHT ) );
	g_hGameCursor[_XMOUSETYPE_SCRSCROLL_DOWN		] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SCRSCROLL_DOWN ) );
	g_hGameCursor[_XMOUSETYPE_SCRSCROLL_DOWNLEFT	] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SCRSCROLL_DOWNLEFT ) );
	g_hGameCursor[_XMOUSETYPE_SCRSCROLL_LEFT		] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_SCRSCROLL_LEFT ) );
	
	g_hGameCursor[_XMOUSETYPE_CROSSPOINTER			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_CROSSPOINTER ) );
	
	g_hGameCursor[_XMOUSETYPE_CHARACTORSELECT		] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_CHARACTERSELECT ) );
	g_hGameCursor[_XMOUSETYPE_GOTOGATE				] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_GATECURSOR ) );
	g_hGameCursor[_XMOUSETYPE_NOWAY					] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_NOWAYCURSOR ) );
	g_hGameCursor[_XMOUSETYPE_CONVERSATION			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_CONVERSATION ) );

	g_hGameCursor[_XMOUSETYPE_MOBOVER				] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_MOBOVER ) );
	g_hGameCursor[_XMOUSETYPE_MOBOVER_CLICK			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_MOBOVERCLICK ) );
		
	g_hGameCursor[_XMOUSETYPE_LPSOVER				] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_LPSOVER ) );
	g_hGameCursor[_XMOUSETYPE_LPSOVER_CLICK			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_LPSCLICK ) );
		
	g_hGameCursor[_XMOUSETYPE_CHATRESIZE			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_CHATRESIZE ) );
	g_hGameCursor[_XMOUSETYPE_CHATRESIZE_CLICK		] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_CHATRESIZECLICK ) );
		
	g_hGameCursor[_XMOUSETYPE_STATUS				] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_STATUS ) );
	g_hGameCursor[_XMOUSETYPE_STATUS_CLICK			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_STATUSCLICK ) );
	
	g_hGameCursor[_XMOUSETYPE_PERSONALSTORE			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_PSHOPOVER ) );
	g_hGameCursor[_XMOUSETYPE_PERSONALSTORE_CLICK	] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_PSHOPCLICK ) );

	g_hGameCursor[_XMOUSETYPE_REBIRTH				] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_REBORNOVER ) );
	g_hGameCursor[_XMOUSETYPE_REBIRTH_CLICK			] = LoadCursor( gHInst, MAKEINTRESOURCE( IDC_REBORNCLICK ) );


	g_SelectCursorIndex			  =		_XMOUSETYPE_SELECT;
	g_SelectClickCursorIndex      =		_XMOUSETYPE_SELECT_CLICK;
	
	
	if(!g_D3DNormalFont)
	{
		g_D3DNormalFont	=	new CD3DFont( _T("MS Sans Serif"), 8, D3DFONT_ZENABLE );
		failed_bool(!g_D3DNormalFont, "Unable to create main font !");
	}
	
	g_D3DNormalFont->InitDeviceObjects( gpDev );
		
	//g_XBaseFont.Load( g_XFontName );
	//g_XLargeFont.Load( g_XLargeFontName );
	//g_XSmallFont.Load( _T("SmallFonts10.xft") );	
	
	SAFE_DELETE( g_XBaseFont	);
	SAFE_DELETE( g_XLargeFont	);
	SAFE_DELETE( g_XSmallFont	);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont = new _XUnicodeFontManager;
		g_XBaseFont->Load(g_XFontName);
		
		g_XLargeFont = new _XUnicodeFontManager;
		g_XLargeFont->Load(g_XLargeFontName);
		
		g_XSmallFont = new _XFontManager;
		g_XSmallFont->Load(_T("SmallFonts10.xft"));
	}
	else
	{	
		g_XBaseFont = new _XFontManager;
		g_XBaseFont->Load(g_XFontName);

		g_XLargeFont = new _XFontManager;
		g_XLargeFont->Load(g_XLargeFontName);

		g_XSmallFont = new _XFontManager;
		g_XSmallFont->Load(_T("SmallFonts10.xft"));		
	}
	
	_XLog( "Initialize cursor..." );
	g_UseHardwareCursor = FALSE;//g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,ID_REGSTR_USEHARDWARECURSOR, 0,TRUE );
	
	_XLog( "Check hardware cursor..." );
	if( g_UseHardwareCursor )
	{
		D3DCAPS9 d3dCaps;
		if( gpDev )
		{
			gpDev->GetDeviceCaps( &d3dCaps );	

			if( d3dCaps.CursorCaps & D3DCURSORCAPS_COLOR )
			{
				_XLog( "Set hardware cursor." );
			}
			else
			{
				_XLog( "WARNING : Not present hardware cursor." );
				g_UseHardwareCursor = FALSE;
				g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEHARDWARECURSOR, 0 );				
				_XLog( "Set software cursor" );
			}
		}		
	}	
	else
	{
		_XLog( "Set device cursor." );
	}

	g_CurrentMouseMode = g_PrevMouseMode = _XMOUSETYPE_PEACE;	

	if( g_UseHardwareCursor )	
	{
		_XPackageArchive mouseimagearchive;

		TCHAR packagefilename[_MAX_PATH];
		packagefilename[0] = NULL;
		strcpy( packagefilename, gModulePath );
		strcat( packagefilename, _T("\\Data\\Texture\\") );

#ifdef _XTESTSERVER	
		strcat( packagefilename, _T("tsTR_HMOUSE.XP") );		
#else
		strcat( packagefilename, _T("TR_HMOUSE.XP") );		
#endif
		if( !mouseimagearchive.OpenPackage( packagefilename ) )
			return false;
		
		for( int i = 0; i < mouseimagearchive.GetResourceCount(); ++i )
		{
			if( i >= _XMOUSETYPE_TOTALCOUNT )
			{
				_XLog( "WARNING : Overflowed stack / mouse image");
				break;
			}

			_XVFile* tfile = _XVFOpenFP( mouseimagearchive.GetPackedFile(i) , 0 );			
			if( !tfile )
			{
				mouseimagearchive.FinalizePackage();	
				return false;
			}

			_XPalette pal;
			g_MouseImage[i] = _XVFLoadBitmap(tfile,pal);

			if(tfile) _XDestroyVFile( tfile );
		}

		mouseimagearchive.FinalizePackage();	

		::SetCursor( NULL );
		if(gpDev) gpDev->ShowCursor( TRUE );

		if( g_MouseImage[g_CurrentMouseMode] )
			_XSetDeviceCursor( g_MousePointerHotspotCoord[g_CurrentMouseMode], 
							   g_MouseImage[g_CurrentMouseMode] );		
	}
	else
	{
		::SetCursor( g_hGameCursor[_XMOUSETYPE_PEACE] );
	}				
	
		
	D3DUtil_InitMaterial( g_SelectedObjectMaterial, 0.7f, 0.7f, 0.78f, 1.0f );
	D3DUtil_InitMaterial( g_DamagedObjectMaterial,  1.0f, 0.1f, 0.0f, 1.0f );
	
	g_SelectedObjectMaterial.Specular.r = 0.2f;
	g_SelectedObjectMaterial.Specular.g = 0.2f;
	g_SelectedObjectMaterial.Specular.b = 0.2f;
	g_SelectedObjectMaterial.Specular.a = 0.2f;
	g_SelectedObjectMaterial.Power = 3.1f;
	
	g_DamagedObjectMaterial.Emissive.r	= 1.0f;
	g_DamagedObjectMaterial.Emissive.g	= 0.0f;
	g_DamagedObjectMaterial.Emissive.b	= 0.0f;
	g_DamagedObjectMaterial.Emissive.a	= 1.0f;

	g_DamagedObjectMaterial.Specular.r = 0.59f;
	g_DamagedObjectMaterial.Specular.g = 0.59f;
	g_DamagedObjectMaterial.Specular.b = 0.59f;
	g_DamagedObjectMaterial.Specular.a = 0.59f;
	g_DamagedObjectMaterial.Power = 1.0f;

	// Set default material
	g_pCurrentSelectMaterial = NULL;

	_XSetDefaultPath( _XDEF_DEFAULTPATH_TEXTURE );
#ifdef _XTESTSERVER
	g_DefaultRenderStyleManager.Load( "tsDefault.xrs" );
#else
	g_DefaultRenderStyleManager.Load( "Default.xrs" );
#endif
					
	///////////////////////////////////////////////////////////////////////////
	// filtering option from texture confirm
	
	// max simultaneous texturing hardware confirm	
	D3DDEVICE_CREATION_PARAMETERS DevCP;	
	gpDev->GetCreationParameters(&DevCP);	
	DWORD MaxSimultaneousTextures = g_d3dCaps.MaxSimultaneousTextures;

	CWater::m_WaterRenderingMode = _XWATER_RM_NORMAL;
	
	if( g_d3dCaps.MaxUserClipPlanes > 0 )
	{
		CWater::m_bUseClipPlanes = TRUE;
	}
	else
	{
		CWater::m_bUseClipPlanes = FALSE;
	}

	if( g_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1,1) )
	{
		g_UseSpeedTreeShader = _XDEF_USESHADER;
		
		if( g_d3dCaps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAPLUMINANCE )
		{			
			CWater::m_WaterRenderingMode = _XDEF_USESHADER ? _XWATER_RM_EMBM : _XWATER_RM_NORMAL;
			//CWater::m_WaterRenderingMode = _XWATER_RM_VERTEXSHADER;
		}

		_XAnimMeshAbstractClass::m_UseVertexShaderAnimation = TRUE;

		BOOL buseshaderanimation =  g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USESHADERANIMATION,  TRUE, FALSE );
		if( _XAnimMeshAbstractClass::m_UseVertexShaderAnimation && !buseshaderanimation )
		{
			_XAnimMeshAbstractClass::m_UseVertexShaderAnimation = FALSE;
		}

		_XLog( "InitDeviceObject : shader version [ VS : %d.%d / PS : %d.%d ]", 
			D3DSHADER_VERSION_MAJOR(g_d3dCaps.VertexShaderVersion), D3DSHADER_VERSION_MINOR(g_d3dCaps.VertexShaderVersion),
			D3DSHADER_VERSION_MAJOR(g_d3dCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(g_d3dCaps.PixelShaderVersion) );	
		_XLog( "InitDeviceObject : [ HCR ] Use vertex shader animation" );
	}
	else
	{
		g_UseSpeedTreeShader = FALSE;

		// Check ETC water bump mode...
		if( g_d3dCaps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAPLUMINANCE )
		{
			CWater::m_WaterRenderingMode = _XDEF_USESHADER ? _XWATER_RM_EMBM : _XWATER_RM_NORMAL;
		}

		_XAnimMeshAbstractClass::m_UseVertexShaderAnimation = FALSE;
		_XLog("InitDeviceObject : shader version [ VS : %d.%d / PS : %d.%d ]", 
			D3DSHADER_VERSION_MAJOR(g_d3dCaps.VertexShaderVersion), D3DSHADER_VERSION_MINOR(g_d3dCaps.VertexShaderVersion),
			D3DSHADER_VERSION_MAJOR(g_d3dCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(g_d3dCaps.PixelShaderVersion) );	
		_XLog( "InitDeviceObject : [ HCR ] Use cpu animation" );
	}		
		
	//g_UseSpeedTreeShader = FALSE;
	
	g_XST_Wrapper.SetUseVertexShader( g_UseSpeedTreeShader );
	g_XST_Wrapper.InitDeviceObjects();

	if( !_XVSAnimMeshContainer::InitializeVertexAnimationShader() ) return false;
		
	if( !CWater::InitDeviceObject() ) return false;
		
	g_LimitTextureWidth		=	g_MaxTextureWidth		=	g_d3dCaps.MaxTextureWidth;
	g_LimitTextureHeight	=	g_MaxTextureHeight		=	g_d3dCaps.MaxTextureHeight;
	
	if( g_ShadowTextureSize > g_MaxTextureWidth ) g_ShadowTextureSize = g_MaxTextureWidth;
	
	if( MaxSimultaneousTextures == 1  )
	{
		g_CloudShadowTextureStage	  = 1; 
		g_bCloudShadow  = FALSE;
		g_bBuildingCharacterShadow = FALSE;		
	}
	else if( MaxSimultaneousTextures == 2  )
	{
		g_CloudShadowTextureStage	  = 1;
		g_bCloudShadow  = FALSE;
		g_bBuildingCharacterShadow = FALSE;		
	}
	else if( MaxSimultaneousTextures == 3  )
	{
		g_CloudShadowTextureStage	  = 2;
		g_bCloudShadow  = !TRUE;
		g_bBuildingCharacterShadow = TRUE;
	}
	else if( MaxSimultaneousTextures >= 4  )
	{
		g_CloudShadowTextureStage	  = 2;		
		g_bCloudShadow  = !TRUE;
		g_bBuildingCharacterShadow = TRUE;
	}

	g_ShadowTextureClearColor  = 0x00000000;
	g_ShadowTextureRenderColor = D3DCOLOR_ARGB(96,0,0,0);
	
	D3DXMatrixIdentity( &g_ShadowDefaultoffset );
	g_ShadowDefaultoffset._11 =  0.5f;		// Scale x
	g_ShadowDefaultoffset._22 = -0.5f;		// Scale z
	g_ShadowDefaultoffset._31 =  0.512f;	// Trans x
	g_ShadowDefaultoffset._32 =  0.515f;	// Trans z
	
	// Use character & cloud shadow
	if( g_bBuildingCharacterShadow )
		g_bBuildingCharacterShadow = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWBUILDINGCHARACTERSHADOW,  FALSE, FALSE );
		
	BOOL bcharactershadow = 
		          g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWCHARACTERSHADOW,  TRUE, TRUE );
	if( !bcharactershadow )
	{
		g_bCharacterShadow = FALSE;
		g_bBuildingCharacterShadow = FALSE;
	}
		
	BOOL cloudshadow =g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWCLOUDSHADOW,  g_bCloudShadow, TRUE );
	if( !cloudshadow )
	{
		g_bCloudShadow = FALSE;
	}	
	
	// Temporary...
	g_bCloudShadow = FALSE;

	g_RS_UseZBuffer	= g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_ZDEPTHFORMAT,  D3DZB_TRUE, TRUE );	
	g_RS_UseZBuffer = D3DZB_TRUE;
	
						  
	_XDWINPRINT( "Hardware confirm result ================================================" ); 
	
	_XDWINPRINT( "[ HCR ] ZDepth format : %s", (g_RS_UseZBuffer	== D3DZB_USEW) ? _T("Use w-buffering") : _T("Use z-buffering") );
	_XDWINPRINT( "[ HCR ] Max simultaneous textures stage : %d", MaxSimultaneousTextures );
			
	_XDWINPRINT( "[ HCR ] %s Character shadow", g_bCharacterShadow ? _T("Enable") : _T("Disable") );	
	_XDWINPRINT( "[ HCR ] %s Building character shadow", g_bBuildingCharacterShadow ? _T("Enable") : _T("Disable") );

	_XDWINPRINT( "[ HCR ] %s Cloud shadow", g_bCloudShadow ? _T("Enable") : _T("Disable") );
	_XDWINPRINT( "[ HCR ] Cloud shadow texture stage : %d ", g_CloudShadowTextureStage );
	
	_XDWINPRINT( "[ RO  ] %s real time generate object", g_bUseRealtimeGenerateObject ? _T("Enable") : _T("Disable") );
				  
	_XDWINPRINT( "========================================================================" ); 
	
	// Create screen burn out billboard================================================================================
	D3DCOLOR fadecolor = D3DCOLOR_ARGB(255,0,0,0);
	g_ScreenBurnOut[0]=_XTLVERTEX( 0.0f,0.0f,0.0f			  ,_XDEF_DEFAULTDEPTH_FORINTERFACE,fadecolor,0,0.0f,0.0f);
	g_ScreenBurnOut[1]=_XTLVERTEX( (FLOAT)gnWidth,0.0f,0.0f	  ,_XDEF_DEFAULTDEPTH_FORINTERFACE,fadecolor,1,1.0f,0.0f);
	g_ScreenBurnOut[2]=_XTLVERTEX( 0.0f,(FLOAT)gnHeight,0.0f  ,_XDEF_DEFAULTDEPTH_FORINTERFACE,fadecolor,1,0.0f,1.0f);
	g_ScreenBurnOut[3]=_XTLVERTEX( (FLOAT)gnWidth,(FLOAT)gnHeight,0.0f,_XDEF_DEFAULTDEPTH_FORINTERFACE,fadecolor,0,1.0f,1.0f);

	// 그림자를 렌더링할 써피스와 텍스쳐를 생성한다
#ifndef _XDWDEBUG
	//if( g_bCharacterShadow )
#endif
	{
		D3DDISPLAYMODE dm;
		gpD3D->GetAdapterDisplayMode(m_d3dSettings.AdapterOrdinal(), &dm);
				
		HRESULT hr = D3DXCreateTexture( gpDev, g_ShadowTextureSize, g_ShadowTextureSize, 0, D3DUSAGE_RENDERTARGET, 
								        D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_lpShadowTexture );
		
		if( FAILED(hr) )
		{
			_XFatalError( "Can't create shadow texture" );
			return false;
		}
		
		if( g_lpShadowTexture )
		{
			hr = g_lpShadowTexture->GetSurfaceLevel(0, &g_lpShadowSurface );
		}
	}	
				
	// 인터페이스 출력용 스프라이트 객체
	D3DXCreateSprite(gpDev, &g_pd3dxSprite);
	
#ifdef _XDEF_USESCREEENMOTIONBLUR 
	g_ScreenMotionBlurEffectManager.InitDeviceObjects();	
#endif
	
	BOOL busebloomfilter = FALSE;
	if( g_d3dCaps.PixelShaderVersion >= D3DPS_VERSION(2,0) )
	{
		busebloomfilter = TRUE;
	}
	
	if( busebloomfilter )
		busebloomfilter = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEBLOOMFILTER,  busebloomfilter, TRUE );
	
	g_PostProcessManager.EnableBloomFilter( busebloomfilter );

	g_PostProcessManager.InitDeviceObjects();
	g_ObjectBlurProcessManager.InitDeviceObjects();
	
	CWater::InitDeviceObject();

	if( !_XParticleList::CreateParticleBillboardObject() ) return FALSE;

	g_HTMLWrapper.Initialize( 256 );
	g_EventHTMLWrapper.Initialize( 512 );
	g_BookOpenHTMLWrapper.Initialize( 512 );

	// 명성 등급에 따른 칼라값 설정
	g_HonorColor[0] = 0xFFFF0000;
	g_HonorColor[1] = 0xFFFF3200;
	g_HonorColor[2] = 0xFFFF6400;
	g_HonorColor[3] = 0xFFE6E6E6;
	g_HonorColor[4] = 0xFFB4E6FF;
	g_HonorColor[5] = 0xFF96C8FF;
	g_HonorColor[6] = 0xFF78B4FF;
	g_HonorColor[7] = 0xFF4696FF;
	g_HonorColor[8] = 0xFF3264FF;

#ifdef _XDEF_MADSYSTEM
	if( !g_MadWrapper.InitDeviceObject() )
		return FALSE;
#endif	

	return m_ProcessManager.InitProcessDeviceObject();
}

HRESULT CEmperorOfDragonsApp::RestoreDeviceObjects( void )
{ 
	if( !gpDev ) return S_OK;
	
	// Restore the font
	if( g_D3DNormalFont )
		g_D3DNormalFont->RestoreDeviceObjects();
	
	g_XST_Wrapper.InitDeviceObjects();

	// 인터페이스용 스프라이트 객체 릴리즈 
	SAFE_RELEASE (g_pd3dxSprite);
	// 인터페이스 출력용 스프라이트 객체
	D3DXCreateSprite(gpDev, &g_pd3dxSprite);

	CWater::m_WaterRenderingMode = _XWATER_RM_NORMAL;

	if( g_d3dCaps.MaxUserClipPlanes > 0 )
	{
		CWater::m_bUseClipPlanes = TRUE;
	}
	else
	{
		CWater::m_bUseClipPlanes = FALSE;
	}
	
	if( g_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1,1) )
	{
		g_UseSpeedTreeShader = _XDEF_USESHADER;
		
		if( g_d3dCaps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAPLUMINANCE )
		{			
			CWater::m_WaterRenderingMode = _XDEF_USESHADER ? _XWATER_RM_EMBM : _XWATER_RM_NORMAL;
			//CWater::m_WaterRenderingMode = _XWATER_RM_VERTEXSHADER;
		}
		
		_XAnimMeshAbstractClass::m_UseVertexShaderAnimation = TRUE;

		BOOL buseshaderanimation =  g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USESHADERANIMATION,  TRUE, FALSE );
		if( _XAnimMeshAbstractClass::m_UseVertexShaderAnimation && !buseshaderanimation )
		{
			_XAnimMeshAbstractClass::m_UseVertexShaderAnimation = FALSE;
		}

		_XLog( "RestoreDeviceObjects : shader version [ VS : %d.%d / PS : %d.%d ]", 
			D3DSHADER_VERSION_MAJOR(g_d3dCaps.VertexShaderVersion), D3DSHADER_VERSION_MINOR(g_d3dCaps.VertexShaderVersion),
			D3DSHADER_VERSION_MAJOR(g_d3dCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(g_d3dCaps.PixelShaderVersion) );	
 
		_XLog( "RestoreDeviceObjects : [ HCR ] Use vertex shader animation" );
	}
	else
	{
		g_UseSpeedTreeShader = FALSE;
		
		// Check ETC water bump mode...
		if( g_d3dCaps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAPLUMINANCE )
		{
			CWater::m_WaterRenderingMode = _XDEF_USESHADER ? _XWATER_RM_EMBM : _XWATER_RM_NORMAL;
		}
		
		_XAnimMeshAbstractClass::m_UseVertexShaderAnimation = FALSE;
		_XLog("RestoreDeviceObjects : shader version [ VS : %d.%d / PS : %d.%d ]", 
			D3DSHADER_VERSION_MAJOR(g_d3dCaps.VertexShaderVersion), D3DSHADER_VERSION_MINOR(g_d3dCaps.VertexShaderVersion),
			D3DSHADER_VERSION_MAJOR(g_d3dCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(g_d3dCaps.PixelShaderVersion) );	
		_XLog( "RestoreDeviceObjects : [ HCR ] Use cpu animation" );
	}

	m_ProcessManager.InitProcessDeviceObject();

	if( !_XVSAnimMeshContainer::InitializeVertexAnimationShader() ) return E_FAIL;

	g_XST_Wrapper.SetUseVertexShader( g_UseSpeedTreeShader );

	if( !CWater::InitDeviceObject() ) return E_FAIL;

	// 그림자를 렌더링할 써피스와 텍스쳐를 생성한다

	// 그림자용 프라퍼티 릴리즈 	
	SAFE_RELEASE( g_lpShadowSurface   );
	SAFE_RELEASE( g_lpShadowTexture   );

#ifndef _XDWDEBUG
	//if( g_bCharacterShadow )
#endif
	{
		D3DDISPLAYMODE dm;
		gpD3D->GetAdapterDisplayMode(m_d3dSettings.AdapterOrdinal(), &dm);
				
		HRESULT hr = D3DXCreateTexture( gpDev, g_ShadowTextureSize, g_ShadowTextureSize, 0, D3DUSAGE_RENDERTARGET, 
										D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_lpShadowTexture );
		
		if( FAILED(hr) )
		{
			_XFatalError( "Can't create shadow texture" );
			return E_FAIL;
		}
		
		if( g_lpShadowTexture )
		{
			hr = g_lpShadowTexture->GetSurfaceLevel(0, &g_lpShadowSurface );
		}
	}	


	// Create screen burn out billboard================================================================================
	D3DCOLOR fadecolor = D3DCOLOR_ARGB(255,0,0,0);
	g_ScreenBurnOut[0]=_XTLVERTEX( 0.0f,0.0f,0.0f			  ,_XDEF_DEFAULTDEPTH_FORINTERFACE,fadecolor,0,0.0f,0.0f);
	g_ScreenBurnOut[1]=_XTLVERTEX( (FLOAT)gnWidth,0.0f,0.0f	  ,_XDEF_DEFAULTDEPTH_FORINTERFACE,fadecolor,1,1.0f,0.0f);
	g_ScreenBurnOut[2]=_XTLVERTEX( 0.0f,(FLOAT)gnHeight,0.0f  ,_XDEF_DEFAULTDEPTH_FORINTERFACE,fadecolor,1,0.0f,1.0f);
	g_ScreenBurnOut[3]=_XTLVERTEX( (FLOAT)gnWidth,(FLOAT)gnHeight,0.0f,_XDEF_DEFAULTDEPTH_FORINTERFACE,fadecolor,0,1.0f,1.0f);

#ifdef _XDEF_USESCREEENMOTIONBLUR 
	g_ScreenMotionBlurEffectManager.RestoreDeviceObjects();
#endif

	BOOL busebloomfilter = FALSE;
	if( g_d3dCaps.PixelShaderVersion >= D3DPS_VERSION(2,0) )
	{
		busebloomfilter = TRUE;
	}

	if( busebloomfilter )
		busebloomfilter = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEBLOOMFILTER,  busebloomfilter, TRUE );

	g_PostProcessManager.EnableBloomFilter( busebloomfilter );

	g_PostProcessManager.RestoreDeviceObjects();
	g_ObjectBlurProcessManager.RestoreDeviceObjects();


	g_HTMLWrapper.RestoreDeviceObject();
	g_EventHTMLWrapper.RestoreDeviceObject();
	g_BookOpenHTMLWrapper.RestoreDeviceObject();

#ifdef _XDEF_MADSYSTEM
	if( !g_MadWrapper.RestoreDeviceObject() )
		return E_FAIL;
#endif

//	if( !_XParticleList::CreateParticleBillboardObject() ) return FALSE;

	return S_OK;
}

HRESULT CEmperorOfDragonsApp::InvalidateDeviceObjects( void )
{
	// TODO: Cleanup any objects created in RestoreDeviceObjects()
    if( g_D3DNormalFont )
		g_D3DNormalFont->InvalidateDeviceObjects();

	// 그림자용 프라퍼티 릴리즈 	
	SAFE_RELEASE( g_lpShadowSurface   );
	SAFE_RELEASE( g_lpShadowTexture   );
	
	// 인터페이스용 스프라이트 객체 릴리즈 
	SAFE_RELEASE (g_pd3dxSprite);
		
	g_XST_Wrapper.DeleteDeviceObjects();

#ifdef _XDEF_USESCREEENMOTIONBLUR 
	g_ScreenMotionBlurEffectManager.InvalidateDeviceObjects();
#endif

	g_PostProcessManager.InvalidateDeviceObjects();
	g_ObjectBlurProcessManager.InvalidateDeviceObjects();

	CWater::ReleaseDeviceObject();
	
	_XVSAnimMeshContainer::ReleaseVertexAnimationShader();

	//_XParticleList::ReleaseParticleBillboardObject();

	m_ProcessManager.ReleaseProcessDeviceObject();

	g_HTMLWrapper.Release();
	g_EventHTMLWrapper.Release();
	g_BookOpenHTMLWrapper.Release();

#ifdef _XDEF_MADSYSTEM
	g_MadWrapper.ReleaseDeviceObject();
#endif
	
	return S_OK;
}

HRESULT CEmperorOfDragonsApp::DeleteDeviceObjects( void )
{
	_XLog( "Release device object" );

    // TODO: Cleanup any objects created in InitDeviceObjects()
    if( g_D3DNormalFont )
	{
		g_D3DNormalFont->DeleteDeviceObjects();    
		delete g_D3DNormalFont;
	}

	// 그림자용 프라퍼티 릴리즈 	
	SAFE_RELEASE( g_lpShadowSurface   );
	SAFE_RELEASE( g_lpShadowTexture   );
	
	// 인터페이스용 스프라이트 객체 릴리즈 
	SAFE_RELEASE (g_pd3dxSprite);

	g_XST_Wrapper.DeleteDeviceObjects();

#ifdef _XDEF_USESCREEENMOTIONBLUR 	
	g_ScreenMotionBlurEffectManager.DeleteDeviceObjects();
#endif

	g_PostProcessManager.DeleteDeviceObjects();
	g_ObjectBlurProcessManager.DeleteDeviceObjects();

	CWater::ReleaseDeviceObject();

	_XVSAnimMeshContainer::ReleaseVertexAnimationShader();

	_XParticleList::ReleaseParticleBillboardObject();
	
	m_ProcessManager.ReleaseProcessDeviceObject();

	g_HTMLWrapper.Release();
	g_EventHTMLWrapper.Release();
	g_BookOpenHTMLWrapper.Release();

#ifdef _XDEF_MADSYSTEM
	g_MadWrapper.ReleaseDeviceObject();
#endif

    return S_OK;
}

void CEmperorOfDragonsApp::SetClickCursor( void )
{	
	if( !g_FreeMoveMode_CursorView ) return;

	if( !g_UseHardwareCursor )
	{
		switch( g_CurrentMouseMode )
		{
		default:
		case _XMOUSETYPE_PEACE : 			
			::SetCursor( g_hGameCursor[_XMOUSETYPE_PEACE_CLICK] );
			break;
		case _XMOUSETYPE_BATTLE : 			
			::SetCursor( g_hGameCursor[_XMOUSETYPE_BATTLE_CLICK] );
			break;
		case _XMOUSETYPE_SELECT : 			
			::SetCursor( g_hGameCursor[_XMOUSETYPE_SELECT_CLICK] );
			break;
		case _XMOUSETYPE_PICKUP :
			::SetCursor( g_hGameCursor[_XMOUSETYPE_PICKUP_CLICK] );
			break;
		case _XMOUSETYPE_MOBOVER :
			::SetCursor( g_hGameCursor[_XMOUSETYPE_MOBOVER_CLICK] );
			break;
		case _XMOUSETYPE_LPSOVER :
			::SetCursor( g_hGameCursor[_XMOUSETYPE_LPSOVER_CLICK] );
			break;
		case _XMOUSETYPE_CHATRESIZE :
			::SetCursor( g_hGameCursor[_XMOUSETYPE_CHATRESIZE_CLICK] );
			break;
		case _XMOUSETYPE_STATUS :
			::SetCursor( g_hGameCursor[_XMOUSETYPE_STATUS_CLICK] );
			break;
		case _XMOUSETYPE_PERSONALSTORE :
			::SetCursor( g_hGameCursor[_XMOUSETYPE_PERSONALSTORE_CLICK] );
			break;
		case _XMOUSETYPE_REBIRTH	:
			::SetCursor( g_hGameCursor[_XMOUSETYPE_REBIRTH_CLICK] );
			break;
		}		
	}
	else
	{
		switch( g_CurrentMouseMode )
		{
		default:
		case _XMOUSETYPE_PEACE : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_PEACE_CLICK], 
				g_MouseImage[_XMOUSETYPE_PEACE_CLICK] );
			break;
		case _XMOUSETYPE_BATTLE : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_BATTLE_CLICK], 
				g_MouseImage[_XMOUSETYPE_BATTLE_CLICK] );
			break;
		case _XMOUSETYPE_SELECT : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SELECT_CLICK], 
				g_MouseImage[_XMOUSETYPE_SELECT_CLICK] );
			break;
		case _XMOUSETYPE_PICKUP : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_PICKUP_CLICK], 
				g_MouseImage[_XMOUSETYPE_PICKUP_CLICK] );
			break;
		}		
	}
}

void CEmperorOfDragonsApp::ReleaseClickCursor( void )
{
	if( !g_FreeMoveMode_CursorView ) return;

	if( !g_UseHardwareCursor )
	{
		switch( g_CurrentMouseMode )
		{
		default:
			::SetCursor( g_hGameCursor[g_CurrentMouseMode] );
			break;
		case _XMOUSETYPE_PEACE : 			
			::SetCursor( g_hGameCursor[_XMOUSETYPE_PEACE] );
			break;
		case _XMOUSETYPE_BATTLE : 			
			::SetCursor( g_hGameCursor[_XMOUSETYPE_BATTLE] );
			break;
		case _XMOUSETYPE_SELECT : 			
			::SetCursor( g_hGameCursor[_XMOUSETYPE_SELECT] );
			break;
		case _XMOUSETYPE_PICKUP : 			
			::SetCursor( g_hGameCursor[_XMOUSETYPE_PICKUP] );
			break;
		case _XMOUSETYPE_MOBOVER :
			::SetCursor( g_hGameCursor[_XMOUSETYPE_MOBOVER] );
			break;
		case _XMOUSETYPE_LPSOVER :
			::SetCursor( g_hGameCursor[_XMOUSETYPE_LPSOVER] );
			break;
		case _XMOUSETYPE_CHATRESIZE :
			::SetCursor( g_hGameCursor[_XMOUSETYPE_CHATRESIZE] );
			break;
		case _XMOUSETYPE_STATUS :
			::SetCursor( g_hGameCursor[_XMOUSETYPE_STATUS] );
			break;
		case _XMOUSETYPE_PERSONALSTORE :
			::SetCursor( g_hGameCursor[_XMOUSETYPE_PERSONALSTORE] );
			break;
		case _XMOUSETYPE_REBIRTH	:
			::SetCursor( g_hGameCursor[_XMOUSETYPE_REBIRTH] );
			break;
		}	
	}
	else
	{
		switch( g_CurrentMouseMode )
		{
		default:
			_XSetDeviceCursor( g_MousePointerHotspotCoord[g_CurrentMouseMode], 
				g_MouseImage[g_CurrentMouseMode] );
			break;
		case _XMOUSETYPE_PEACE : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_PEACE], 
				g_MouseImage[_XMOUSETYPE_PEACE] );
			break;
		case _XMOUSETYPE_BATTLE : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_BATTLE], 
				g_MouseImage[_XMOUSETYPE_BATTLE] );
			break;
		case _XMOUSETYPE_SELECT : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SELECT], 
				g_MouseImage[_XMOUSETYPE_SELECT] );
			break;
		case _XMOUSETYPE_PICKUP : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_PICKUP],
				g_MouseImage[_XMOUSETYPE_PICKUP] );
			break;
		}		
	}
}

void CEmperorOfDragonsApp::ResetReadyCursor(  DWORD type  )
{
	g_ReadyMouseCursorMode = (_XMOUSETYPE)type;
}

void CEmperorOfDragonsApp::ReadyCursor(  DWORD type  )
{	
	if( g_MousePointerPriorityTable[ g_ReadyMouseCursorMode ] <= g_MousePointerPriorityTable[ type ] )
	{
		g_ReadyMouseCursorMode = (_XMOUSETYPE)type;
	}
}

void CEmperorOfDragonsApp::SetCursor( DWORD type, BOOL forcesetting )
{		
	if( !gpInput ) return;
	if( !g_FreeMoveMode_CursorView ) return;

	if( !forcesetting )
		if( g_CurrentMouseMode == type && !gpInput->GetMouseState()->bButton[0] ) return;

	g_PrevMouseMode = g_CurrentMouseMode;
	g_CurrentMouseMode = (_XMOUSETYPE)type;

	if( !g_UseHardwareCursor )
	{
		switch( g_CurrentMouseMode )
		{		
		case _XMOUSETYPE_PEACE : 			
			if( gpInput->GetMouseState()->bButton[0] )
			{
				::SetCursor( g_hGameCursor[_XMOUSETYPE_PEACE_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_PEACE_CLICK;
			}
			else 
				::SetCursor( g_hGameCursor[_XMOUSETYPE_PEACE] );
			break;
		case _XMOUSETYPE_BATTLE : 			
			if( gpInput->GetMouseState()->bButton[0] )
			{
				::SetCursor( g_hGameCursor[_XMOUSETYPE_BATTLE_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_BATTLE_CLICK;
			}
			else
				::SetCursor( g_hGameCursor[_XMOUSETYPE_BATTLE] );
			break;
		case _XMOUSETYPE_SELECT : 			
			if( gpInput->GetMouseState()->bButton[0] )
			{
				::SetCursor( g_hGameCursor[_XMOUSETYPE_SELECT_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_SELECT_CLICK;
			}
			else
				::SetCursor( g_hGameCursor[_XMOUSETYPE_SELECT] );
			break;
		case _XMOUSETYPE_PICKUP :
			if( gpInput->GetMouseState()->bButton[0] )
			{
				::SetCursor( g_hGameCursor[_XMOUSETYPE_PICKUP_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_PICKUP_CLICK;
			}
			else
				::SetCursor( g_hGameCursor[_XMOUSETYPE_PICKUP] );
			break;
		case _XMOUSETYPE_MOBOVER : 
			if( gpInput->GetMouseState()->bButton[0] )
			{
				::SetCursor( g_hGameCursor[_XMOUSETYPE_MOBOVER_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_MOBOVER_CLICK;
			}
			else
				::SetCursor( g_hGameCursor[_XMOUSETYPE_MOBOVER] );
			break;
		case _XMOUSETYPE_LPSOVER :
			if( gpInput->GetMouseState()->bButton[0] )
			{
				::SetCursor( g_hGameCursor[_XMOUSETYPE_LPSOVER_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_LPSOVER_CLICK;
			}
			else
				::SetCursor( g_hGameCursor[_XMOUSETYPE_LPSOVER] );
			break;				
		case _XMOUSETYPE_CHATRESIZE :
			if( gpInput->GetMouseState()->bButton[0] )
			{
				::SetCursor( g_hGameCursor[_XMOUSETYPE_CHATRESIZE_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_CHATRESIZE_CLICK;
			}
			else
				::SetCursor( g_hGameCursor[_XMOUSETYPE_CHATRESIZE] );
			break;
		case _XMOUSETYPE_STATUS :
			if( gpInput->GetMouseState()->bButton[0] )
			{
				::SetCursor( g_hGameCursor[_XMOUSETYPE_STATUS_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_STATUS_CLICK;
			}
			else
				::SetCursor( g_hGameCursor[_XMOUSETYPE_STATUS] );
			break;
		case _XMOUSETYPE_PERSONALSTORE :
			if( gpInput->GetMouseState()->bButton[0] )
			{
				::SetCursor( g_hGameCursor[_XMOUSETYPE_PERSONALSTORE_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_PERSONALSTORE_CLICK;
			}
			else
				::SetCursor( g_hGameCursor[_XMOUSETYPE_PERSONALSTORE] );
			break;
		case _XMOUSETYPE_REBIRTH :
			if( gpInput->GetMouseState()->bButton[0] )
			{
				::SetCursor( g_hGameCursor[_XMOUSETYPE_REBIRTH_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_REBIRTH_CLICK;
			}
			else
				::SetCursor( g_hGameCursor[_XMOUSETYPE_REBIRTH] );
			break;
		default:
			::SetCursor( g_hGameCursor[g_CurrentMouseMode] );
			break;
		}		
	}
	else
	{
		switch( type )
		{		
		case _XMOUSETYPE_PEACE : 
			if( gpInput->GetMouseState()->bButton[0] )
			{
				_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_PEACE_CLICK],
								   g_MouseImage[_XMOUSETYPE_PEACE_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_PEACE_CLICK;
			}
			else
				_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_PEACE], 
								   g_MouseImage[_XMOUSETYPE_PEACE] );				
			break;
		case _XMOUSETYPE_BATTLE : 
			if( gpInput->GetMouseState()->bButton[0] )
			{
				_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_BATTLE_CLICK], 
								   g_MouseImage[_XMOUSETYPE_BATTLE_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_BATTLE_CLICK;
			}
			else
				_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_BATTLE], 
				g_MouseImage[_XMOUSETYPE_BATTLE] );
				
			break;
		case _XMOUSETYPE_SELECT : 
			if( gpInput->GetMouseState()->bButton[0] )
			{
				_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SELECT_CLICK], 
								   g_MouseImage[_XMOUSETYPE_SELECT_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_SELECT_CLICK;
			}
			else
				_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SELECT], 
								   g_MouseImage[_XMOUSETYPE_SELECT] );				
			break;
		case _XMOUSETYPE_PICKUP : 			
			if( gpInput->GetMouseState()->bButton[0] )
			{
				_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_PICKUP_CLICK], 
								   g_MouseImage[_XMOUSETYPE_PICKUP_CLICK] );
				g_CurrentMouseMode = _XMOUSETYPE_PICKUP_CLICK;
			}
			else
				_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_PICKUP], 
								   g_MouseImage[_XMOUSETYPE_PICKUP] );				
			break;		
		case _XMOUSETYPE_RESIZE_DIAGONAL_LEFT :
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_RESIZE_DIAGONAL_LEFT], 
				               g_MouseImage[_XMOUSETYPE_RESIZE_DIAGONAL_LEFT] );
			break;
		case _XMOUSETYPE_RESIZE_DIAGONAL_RIGHT : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_RESIZE_DIAGONAL_RIGHT], 
				               g_MouseImage[_XMOUSETYPE_RESIZE_DIAGONAL_RIGHT] );
			break;
		case _XMOUSETYPE_RESIZE_HORIZ : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_RESIZE_HORIZ], 
				               g_MouseImage[_XMOUSETYPE_RESIZE_HORIZ] );
			break;
		case _XMOUSETYPE_RESIZE_VERT : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_RESIZE_VERT], 
				               g_MouseImage[_XMOUSETYPE_RESIZE_VERT] );
			break;
		case _XMOUSETYPE_SANDGLASS1 : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SANDGLASS1], 
				               g_MouseImage[_XMOUSETYPE_SANDGLASS1] );
			break;
		case _XMOUSETYPE_SCRSCROLL_UPLEFT : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SCRSCROLL_UPLEFT], 
				g_MouseImage[_XMOUSETYPE_SCRSCROLL_UPLEFT] );
			break;
		case _XMOUSETYPE_SCRSCROLL_UP : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SCRSCROLL_UP], 
				g_MouseImage[_XMOUSETYPE_SCRSCROLL_UP] );
			break;
		case _XMOUSETYPE_SCRSCROLL_UPRIGHT :
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SCRSCROLL_UPRIGHT], 
				g_MouseImage[_XMOUSETYPE_SCRSCROLL_UPRIGHT] );
			break;
		case _XMOUSETYPE_SCRSCROLL_RIGHT :
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SCRSCROLL_RIGHT], 
				g_MouseImage[_XMOUSETYPE_SCRSCROLL_RIGHT] );
			break;
		case _XMOUSETYPE_SCRSCROLL_DOWNRIGHT :
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SCRSCROLL_DOWNRIGHT], 
				g_MouseImage[_XMOUSETYPE_SCRSCROLL_DOWNRIGHT] );
			break;
		case _XMOUSETYPE_SCRSCROLL_DOWN :
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SCRSCROLL_DOWN], 
				g_MouseImage[_XMOUSETYPE_SCRSCROLL_DOWN] );
			break;
		case _XMOUSETYPE_SCRSCROLL_DOWNLEFT : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SCRSCROLL_DOWNLEFT], 
				g_MouseImage[_XMOUSETYPE_SCRSCROLL_DOWNLEFT] );
			break;
		case _XMOUSETYPE_SCRSCROLL_LEFT : 
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_SCRSCROLL_LEFT], 
				g_MouseImage[_XMOUSETYPE_SCRSCROLL_LEFT] );
			break;
		case _XMOUSETYPE_CROSSPOINTER :
			_XSetDeviceCursor( g_MousePointerHotspotCoord[_XMOUSETYPE_CROSSPOINTER], 
				g_MouseImage[_XMOUSETYPE_CROSSPOINTER] );
			break;
		default:
			_XSetDeviceCursor( g_MousePointerHotspotCoord[type], 
				g_MouseImage[type] );
			break;
		}		
	}
}

void CEmperorOfDragonsApp::ReleaseGameObjects()
{		
	_XLog( "Release game object" );
	
	int i;

#ifdef _XUSEFMOD
	g_FMODWrapper.StopAllSound();
	g_FMODWrapper.StopBGM();
#else
	for( i = 0; i < 3; ++i )
	{
		if( g_pBGMSoundObject[i] )
		{
			if( g_pBGMSoundObject[i]->IsPlaying() )
			{
				g_pBGMSoundObject[i]->Stop();			
			}
			
			g_pBGMSoundObject[i]->Destroy();
			g_pBGMSoundObject[i] = NULL;
		}
	}
#endif
	
	SAFE_DELETE_ARRAY( g_ItemUseSoundIndexTable );
	SAFE_DELETE_ARRAY( g_WeaponSoundIndexList   );
	SAFE_DELETE_ARRAY( g_MobWeaponSoundIndexList);

	m_proc_firstload.DestroyProcess();
	m_proc_loginserver.DestroyProcess();
	m_proc_selectcharacter.DestroyProcess();
	m_proc_createcharacter.DestroyProcess();
	m_proc_loading.DestroyProcess();
	m_proc_maingame.DestroyProcess();
	// hotblood zerolevel 
	m_proc_selecthero.DestroyProcess();
	m_proc_zerolevel.DestroyProcess();

#ifdef _XTS_SERVERUNIFICATION
	m_proc_serverunification.DestroyProcess();
#endif
	
	g_NetworkKernel.SendMessengerPacket(MMSG_NO_LOGOUT_C_S);
	g_NetworkKernel.ReleaseNetwork();

	if(g_XBaseFont)g_XBaseFont->DeleteDeviceObject();
	if(g_XLargeFont)g_XLargeFont->DeleteDeviceObject();
	if(g_XSmallFont)g_XSmallFont->DeleteDeviceObject();

	SAFE_DELETE( g_XBaseFont	);
	SAFE_DELETE( g_XLargeFont	);
	SAFE_DELETE( g_XSmallFont	);
	
	if( g_D3DNormalFont )
	{
		g_D3DNormalFont->DeleteDeviceObjects();    
		delete g_D3DNormalFont;
		g_D3DNormalFont = NULL;
	}
	
	// 그림자용 프라퍼티 릴리즈 	
	SAFE_RELEASE( g_lpShadowSurface   );
	SAFE_RELEASE( g_lpShadowTexture   );
	
	// 인터페이스용 스프라이트 객체 릴리즈 
	SAFE_RELEASE (g_pd3dxSprite);
	
	g_WordFilteringManager.RemoveAll();

	g_InterfaceTooltip.DestroyWindow();
	g_MessageBox.DestroyWindow();
	
	g_MaleBipedObject.DestroyModel();
	g_FemaleBipedObject.DestroyModel();
	
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_FACE; ++i ) 
	{
		SAFE_DELETE( g_UserModel_FaceType[i] );		
	}
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_UBODY; ++i ) 
	{
		SAFE_DELETE( g_UserModel_UBodyType[i] );
	}

	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_LBODY; ++i ) 
	{
		SAFE_DELETE( g_UserModel_LBodyType[i] );
	}

	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_LHAND; ++i )
	{
		SAFE_DELETE( g_UserModel_LHandType[i] );
	}

	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_RHAND; ++i )
	{
		SAFE_DELETE( g_UserModel_RHandType[i] );
	}

	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_LFOOT; ++i )
	{
		SAFE_DELETE( g_UserModel_LFootType[i] );
	}

	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_RFOOT; ++i )
	{
		SAFE_DELETE( g_UserModel_RFootType[i] );
	}

	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_CAP; ++i )
	{
		SAFE_DELETE( g_UserModel_Cap[i] );
	}

	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_ADAE; ++i )
	{
		SAFE_DELETE( g_UserModel_Adae[i] );
	}

	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_EARRING; ++i )
	{
		SAFE_DELETE( g_UserModel_Earring[i] );
	}
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_BACKPACK; ++i )
	{
		SAFE_DELETE( g_UserModel_BackPack[i] );
	}

	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_HAIR; ++i )
	{
		SAFE_DELETE( g_UserModel_HairType[i] );
	}

	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_MASK; ++i )
	{
		SAFE_DELETE( g_UserModel_Mask[i] );
	}
	for( i = 0; i < _XDEF_USERMODELTYPECOUNT_FACEMASK; ++i )
	{
		SAFE_DELETE( g_UserModel_FaceMask[i] );
	}
		
	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_WEAPON; ++i )
	{
		SAFE_DELETE(g_ItemModel_Weapon[0][i]);		
		SAFE_DELETE(g_ItemModel_Weapon[1][i]);
	}

	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_MARTIALBOOK; ++i ) 
	{
		SAFE_DELETE( g_ItemModel_MartialBook[i] );
	}
	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_POTION; ++i )
	{
		SAFE_DELETE( g_ItemModel_Potion[i] );		
	}
	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_THROW; ++i )
	{
		SAFE_DELETE( g_ItemModel_Throw[i] );
	}

	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_SOCERY; ++i )
	{
		SAFE_DELETE( g_ItemModel_Socery[i] );
	}

	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_ETC; ++i )
	{
		SAFE_DELETE( g_ItemModel_ETC[i] );
	}
	
	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_JEWELBOX; ++i )
	{
		SAFE_DELETE( g_ItemModel_JewelBox[i] );
	}
	for( i = 0; i < _XDEF_ITEMMODELTYPECOUNT_ELIXIR; ++i )
	{
		SAFE_DELETE( g_ItemModel_Elixir[i] );
	}
	
	/*
	for( i = 0; i < _XDEF_MODELCLASSCOUNT_MOB; i++ )
	{
		g_MOBBIPEDMODEL[i].DestroyModel();	
		SAFE_DELETE( g_MOBMODEL[i] );
	}
	
	for( i = 0; i < _XDEF_MODELCLASSCOUNT_NPC; i++ )
	{
		g_NPCBIPEDMODEL[i].DestroyModel();
		SAFE_DELETE( g_NPCMODEL[i] );
	}
	*/

	for(i = 0 ; i < _XDEF_MODELCLASSCOUNT_DUMMY ; ++i)
	{
		g_DUMMYBIPEDMODEL[i].DestroyModel();
		SAFE_DELETE(g_DUMMYMODEL[i]);
		SAFE_DELETE(g_SEATMODEL[i]);
	}

	for(i = 0 ; i < _XDEF_MODELCLASSCOUNT_FLAG ; ++i)
	{
		SAFE_DELETE( g_FLAGMODEL[i] );
	}
	
	// destroy mouse image or texture archive...
	
	for( i = 0; i < _XMOUSETYPE_TOTALCOUNT; ++i )
	{
		_XDestroyBitmap( g_MouseImage[i] );		
		g_MouseImage[i] = NULL;
	}
	
	g_XST_Wrapper.Release();

	SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_00_00 );

#ifdef _XTS_MASTERYOPTIONAL
	SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_01_00 );
	SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_02_00 );
	SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_03_00 );
	SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_04_00 );
	SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_05_00 );
	SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_06_00 );
#endif

	for( i = 0; i < 3; i++ )
	{
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_01_01[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_01_02[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_01_03[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_01_04[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_01_05[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_01_06[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_01_07[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_01_08[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_01_09[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_01_10[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_01_11[i] );

		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_02_01[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_02_02[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_02_03[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_02_04[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_02_05[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_02_06[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_02_07[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_02_08[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_02_09[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_02_10[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_02_11[i] );

		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_03_01[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_03_02[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_03_03[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_03_04[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_03_05[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_03_06[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_03_07[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_03_08[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_03_09[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_03_10[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_03_11[i] );

		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_04_01[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_04_02[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_04_03[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_04_04[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_04_05[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_04_06[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_04_07[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_04_08[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_04_09[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_04_10[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_04_11[i] );

		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_05_01[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_05_02[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_05_03[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_05_04[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_05_05[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_05_06[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_05_07[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_05_08[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_05_09[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_05_10[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_05_11[i] );

		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_06_01[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_06_02[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_06_03[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_06_04[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_06_05[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_06_06[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_06_07[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_06_08[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_06_09[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_06_10[i] );
		SAFE_DELETE_ARRAY( g_SkillMasteryNameProperty_06_11[i] );
	}

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
	SAFE_DELETE_ARRAY( g_LoadQuestScriptText_IndexTable );
#endif

	for( i = 0; i < _XDEF_MAXFLASHCOUNT; i++)
	{		
		SAFE_DELETE_ARRAY( g_SceneList[i] );
	}
	
	g_TerrainTextureArchive.DisposeTexture();
	g_WorldObjectTextureArchive.DisposeTexture();
	g_CharacterTextureArchive.DisposeTexture();	
	g_MobTextureArchive.DisposeTexture();
	g_EffectTextureArchive.DisposeTexture();	
	g_MainInterfaceTextureArchive.DisposeTexture();
	g_RTimeObjectTextureArchive.DisposeTexture();
	g_DummyObjectTextureArchive.DisposeTexture();
	
	g_CharacterEffectGroupManager.Dispose();
	g_WorldEffectGroupManager.Dispose();	
	_XParticleList::ReleaseParticleBillboardObject();
	
	g_LodTerrain.Release();
	g_LodTerrain.ReleaseTerrainTileInfo();	
	g_LodTerrain.ReleaseTileRenderingCache();
	g_LodTerrain.ReleaseTerrainIndexTable();

	g_EnvironmentManager.ReleaseSkyBoxModel();
	
	// by mahwang
	g_Userlist.disposeList();
	g_Userlist.DeletePool();

	g_LodTerrain.DisposeCachedMonsterModel();
	g_Moblist.disposeList();
	g_Moblist.DeletePool();
	g_Moblist.DestroyData();

	g_Npclist.disposeList();
	g_Npclist.DestroyData();

	g_GameItemlist.disposeList();
	g_pLocalUser->m_ModelDescriptor.Reset();
	
	g_LodTerrain.m_FunctionalObjectManager.disposeList();

	g_XGUIControlCenter.Release();
	g_MainWindowManager.DestroyManager();
	m_proc_maingame.m_pNPCTradeWindowPtr = NULL;
	m_proc_maingame.m_pWarehouseWindowPtr = NULL;
	m_proc_maingame.m_pPCTradeWindowPtr = NULL;
	m_proc_maingame.m_pNPCSkillWindowPtr = NULL;

	g_StrManager.Release();

	g_QuestScriptManager.Release();

	_XGameItem::ReleaseItemProperty();
	_XSkillItem::ReleaseSkillProperty();
	_XSkillItem::ReleaseSkillPackage();
	_XCharacterStateList::ReleaseCharacterStateTable();

#ifdef _XTS_FAME
	SAFE_DELETE_ARRAY(g_FameStr);
#endif

	_XVSAnimMeshContainer::ReleaseVertexAnimationShader();

#ifndef _XUSEFMOD
	/*for( int sindex = 0; sindex < g_MaxInterfaceSoundEffectCount; sindex++ )
	{
		if( g_pInterfaceSound[sindex] )
		{
			g_pInterfaceSound[sindex]->Destroy();
		}
	}*/
	SAFE_DELETE_ARRAY( g_pInterfaceSound );
#endif

#ifdef _XDWDEBUG	
	if( g_hEffectScriptWindow )
	{
		DestroyWindow(g_hEffectScriptWindow);
		g_hEffectScriptWindow = NULL;
	}

	if( g_RichEditDLL )
		FreeLibrary( g_RichEditDLL );	

	g_DebugOutputlist[0].disposeList();
	g_DebugOutputlist[1].disposeList();
	g_DebugOutputlist[2].disposeList();
	g_DebugOutputlist[3].disposeList();
#endif

	if(g_pTitleFlashObject)
	{
		g_pTitleFlashObject->FreeBuffer();
		g_pTitleFlashObject->ControlClose();
		delete g_pTitleFlashObject;
		g_pTitleFlashObject = NULL;
	}
	
	SAFE_RELEASE( g_pTitleFlashOffScreenTexture );	

#ifdef _XDEF_HELPSYSTEMSERVERSAVE	
	SAFE_DELETE( g_TutorialInfo );
#endif

#ifdef _ACCLAIM_IGAADSYSTEM	
	g_IGAADWrapper.Finalize();
#endif

	g_ParticleListPool.DeleteData();
	g_ParticleGroupItemPool.DeleteData();
	g_ParticleDecalObjectPool.DeleteData();
	g_ParticleGroupInstanceManagerPool.DeleteData();

	CEmperorOfDragonsApp::FinalizePackageArchive();
}

//=====================================================================================================================
// Window message handler
//=====================================================================================================================
bool CEmperorOfDragonsApp::KeyboardPoll(_XInput *pInput)
{
	if( !gpInput ) return true;	
	if( !_XIMEKERNEL.GetIMEContainer() || gpInput->CheckKeyPress(DIK_LCONTROL) || gpInput->CheckKeyPress(DIK_LMENU) ) 
	{
		return m_ProcessManager.OnKeyboardPoll( pInput );
	}
	return true;
}

bool CEmperorOfDragonsApp::KeyDown(WPARAM wparam, LPARAM lparam)
{	
	if( g_MessageBox.GetShowStatus() )
	{
		return g_MessageBox.OnKeyDown( wparam, lparam );
	}
	else return m_ProcessManager.OnKeyDown( wparam, lparam );
}

bool CEmperorOfDragonsApp::KeyUp(WPARAM wparam, LPARAM lparam)
{
	return m_ProcessManager.OnKeyUp( wparam, lparam );	
}

bool CEmperorOfDragonsApp::MousePoll(MouseState *pState, ScrnPos *pPos)
{	
	return m_ProcessManager.OnMousePoll( pState, pPos );
}

bool CEmperorOfDragonsApp::MouseButton(_XMouseButton p_MB, bool bPushed)
{
	return m_ProcessManager.OnMouseButton( p_MB, bPushed );
}

void CEmperorOfDragonsApp::OnMouseMove( WPARAM wparam, LPARAM lparam )
{
	m_ProcessManager.OnMouseMove( wparam, lparam );
}

bool CEmperorOfDragonsApp::WndMessageHandler( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{	
	if( !m_InitializedApp ) return false;
	return false;
}

void CEmperorOfDragonsApp::MessageHandler(  UINT message, WPARAM wparam, LPARAM lparam )
{	
	m_ProcessManager.OnMessageHandler( message, wparam, lparam );
}

void CEmperorOfDragonsApp::OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	m_ProcessManager.OnUserMessage_Socket( message, wparam, lparam );
}

void CEmperorOfDragonsApp::OnUserMessage_GetHost( UINT message, WPARAM wparam, LPARAM lparam )
{
	m_ProcessManager.OnUserMessage_GetHost( message, wparam, lparam );
}

void CEmperorOfDragonsApp::OnUserMessage_UDPSocket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	m_ProcessManager.OnUserMessage_UDPSocket( message, wparam, lparam );
}

void CEmperorOfDragonsApp::OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	m_ProcessManager.OnUserMessage_MessengerSocket( message, wparam, lparam );
}

void CEmperorOfDragonsApp::PaintHandler( void )
{
	m_ProcessManager.OnPaintHandler();
}

bool CEmperorOfDragonsApp::MouseWheel( WPARAM wparam )
{	
	m_ProcessManager.OnMouseWheel( (short)(HIWORD(wparam)) );
	return true;
}

void CEmperorOfDragonsApp::MouseLBtnDblClick( WPARAM wparam )
{
	m_ProcessManager.OnMouseLBtnDblClick( wparam );	
}

void CEmperorOfDragonsApp::MouseRBtnDblClick( WPARAM wparam )
{
	m_ProcessManager.OnMouseRBtnDblClick( wparam );	
}

void CEmperorOfDragonsApp::MouseMBtnDblClick( WPARAM wparam )
{
	m_ProcessManager.OnMouseMBtnDblClick( wparam );
}

void CEmperorOfDragonsApp::MCINotify( UINT message, WPARAM wparam, LPARAM lparam )
{
	m_ProcessManager.OnMCINotify( message, wparam, lparam );	
}

void CEmperorOfDragonsApp::OnWindowSizeChangeNotify( void )
{
	m_ProcessManager.OnWindowSizeChangeNotify();
}
void CEmperorOfDragonsApp::OnTimer(WPARAM wparam, LPARAM lparam)
{
	m_ProcessManager.OnTimer( wparam, lparam );	
}
void CEmperorOfDragonsApp::OnPaint(WPARAM wparam, LPARAM lparam)
{
	m_ProcessManager.OnPaint( wparam, lparam );	
}
void CEmperorOfDragonsApp::OnMove(WPARAM wparam, LPARAM lparam)
{
	POINT pt = { 0, 0 };
	ClientToScreen( gHWnd, &pt );			
	
#ifdef _XDEF_EVENTWINDOW_WEBPAGE_20070214
	if( g_LanguageType != _XLANGUAGE_TYPE_JAPANESE )
	{
		SetWindowPos(g_hWebEventWindow, HWND_TOPMOST, pt.x + 14, pt.y + 84, 215, 315, 0);	
	}
#endif

#ifdef _XDEF_WEBBOARD_20061211
	if( g_LanguageType != _XLANGUAGE_TYPE_JAPANESE )
	{
		SetWindowPos(g_hWebBoardWindow, HWND_TOPMOST, pt.x + (gnWidth>>1)-343, pt.y + (gnHeight>>1)-280, 688, 450, 0);
	}
#endif

#ifdef _XTS_ITEMMALLBROWSER							
	if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		SetWindowPos(g_hItemMallWindow, NULL, pt.x + (gnWidth>>1)-400, pt.y + (gnHeight>>1)-300, 800, 600, 0);
	}
	else
	{
		SetWindowPos(g_hItemMallWindow, HWND_TOPMOST, pt.x + (gnWidth>>1)-400, pt.y + (gnHeight>>1)-300, 800, 600, 0);
	}
#endif

#ifdef _ACCLAIM_RUBICONADSYSTEM	
	::SetWindowPos(g_hRubiconADWindow, NULL, pt.x + g_RubiconADWindowPosition.x, pt.y + g_RubiconADWindowPosition.y, g_RubiconADWindowSize.cx, g_RubiconADWindowSize.cy, 0);
	::SetWindowPos(g_hRubiconADWebBrowserObject, NULL, 0, 0, g_RubiconADWindowSize.cx, g_RubiconADWindowSize.cy, 0);
#endif
}

void CEmperorOfDragonsApp::OnNexonMessangerEvent( WPARAM wparam, LPARAM lparam ) // Nexon messanger event
{
	m_ProcessManager.OnNexonMessangerEvent( wparam, lparam );	
}

void CEmperorOfDragonsApp::VolumeControl( int value )
{
	
}

void CEmperorOfDragonsApp::VolumeControlUp( void )
{
	
}

void CEmperorOfDragonsApp::VolumeControlDown( void )
{
	
}

bool CEmperorOfDragonsApp::Process()
{
	return m_ProcessManager.ProcessHandler();	
}

bool CEmperorOfDragonsApp::Render()
{
	m_ProcessManager.OnPaintHandler();	
	return true;
}

void CEmperorOfDragonsApp::ChangeLanguage( _XLANGUAGE_TYPE language )
{
	BOOL nochange = (g_LanguageType == language) ? TRUE : FALSE;

	if( !nochange )
	{
		g_LanguageType = language;

		if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
		{
			strcpy( g_GUIFontName, _T("gulim") );
			strcpy( g_XFontName, _T("gulim12.xft") );
			//strcpy( g_XFontName, _T("Dotum12.xft") );
			strcpy( g_XLargeFontName, _T("GoongSeo18.xft") );
			//strcpy( g_XLargeFontName, _T("HY예서M18.xft") );
			//strcpy( g_XFontName, _T("SimSun12.fnt") );
			_XLog( "Set korean language pack, Font name [%s]", g_GUIFontName );				
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
		{
			strcpy( g_GUIFontName, _T("MingLiU") );
			strcpy( g_XFontName, _T("MingLiU12.xft") );			
			strcpy( g_XLargeFontName, _T("PMingLiU18.xft") );
			_XLog( "Set taiwanese language pack, Font name [%s]", g_GUIFontName );
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
		{
			strcpy( g_GUIFontName, _T("gulim") );
			strcpy( g_XFontName, _T("Palatino16.xft") );
			strcpy( g_XLargeFontName, _T("Speedball22.xft") );
			_XLog( "Set english language pack, Font name [%s]", g_GUIFontName );
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			memset( g_XFontName, 0, sizeof(TCHAR) * 64 );
			memset( g_XLargeFontName, 0, sizeof(TCHAR) * 64 );
			
			strcpy( g_GUIFontName, _T("Arial Unicode MS") );
			strcpy( g_XFontName, _T("ArialUnicodeMS16.xft") );
			strcpy( g_XLargeFontName, _T("MicrosoftSansSerif24.xft") );
			_XLog( "Set vietnamese language pack, Font name [%s]", g_GUIFontName );
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
		{
			strcpy( g_GUIFontName, _T("Arial") );		
			strcpy( g_XFontName, _T("MS Gothic16.xft") );
			strcpy( g_XLargeFontName, _T("HGPKyokashotai22.xft") );
			_XLog( "Set japanese language pack, Font name [%s]", g_GUIFontName );
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_CHINESE )
		{
			strcpy( g_GUIFontName, _T("SimSun") );
			strcpy( g_XFontName, _T("SimSun12.xft") );
			//strcpy( g_XLargeFontName, _T("Haeseo22.xft") );
			strcpy( g_XLargeFontName, _T("GoongSeo18.xft") );
			_XLog( "Set chinese language pack, Font name [%s]", g_GUIFontName );		
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			strcpy( g_GUIFontName, _T("gulim") );
			strcpy( g_XFontName, _T("Palatino16.xft") );
			strcpy( g_XLargeFontName, _T("Speedball22.xft") );
			_XLog( "Set english language pack, Font name [%s]", g_GUIFontName );
		}
		else 
		{
			g_LanguageType = _XLANGUAGE_TYPE_UNKNOWN;
			_XLog( "unknown language, Set default language pack, Font name [%s]", g_GUIFontName );
			strcpy( g_GUIFontName, _T("Arial") );	
			strcpy( g_XFontName, _T("gulim12.xft") );
			//strcpy( g_XLargeFontName, _T("Haeseo22.xft") );
			strcpy( g_XLargeFontName, _T("GoongSeo18.xft") );
		}			
		
		SAFE_DELETE( g_XBaseFont	);
		SAFE_DELETE( g_XLargeFont	);
		SAFE_DELETE( g_XSmallFont	);
		
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
			g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont = new _XUnicodeFontManager;
			g_XBaseFont->Load(g_XFontName);
			
			g_XLargeFont = new _XUnicodeFontManager;
			g_XLargeFont->Load(g_XLargeFontName);
			
			g_XSmallFont = new _XFontManager;
			g_XSmallFont->Load(_T("SmallFonts10.xft"));
		}
		else
		{	
			g_XBaseFont = new _XFontManager;
			g_XBaseFont->Load(g_XFontName);
			
			g_XLargeFont = new _XFontManager;
			g_XLargeFont->Load(g_XLargeFontName);
			
			g_XSmallFont = new _XFontManager;
			g_XSmallFont->Load(_T("SmallFonts10.xft"));		
		}
	}

	_XLog( "Reload string database" );
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );	
#ifdef _XTESTSERVER		
	if( !g_StringDataBase.Load( _T("tsNDTextDB_KR.xsd"), g_LanguageType ) )
	{
		::ShowWindow( g_hLoadingWindow, SW_HIDE );
		::DestroyWindow(g_hLoadingWindow);
		g_hLoadingWindow = NULL;
		return;
	}
#else

#ifdef _XDEF_NEWSTRINGDBFORMAT
	TCHAR textstringdbfilename[10][32] = 
	{
		_T("NDTextDB_KR.xsd"), _T("NDTextDB_VN.xsd"), _T("NDTextDB_TW.xsd"), _T("NDTextDB_US.xsd"), 
		_T("NDTextDB_CN.xsd"), _T("NDTextDB_JP.xsd"), _T("NDTextDB_RS.xsd"), _T("NDTextDB_TH.xsd")
	};
	
#ifdef _XENGLISH
	TCHAR szFilename[256];
	memset( szFilename, 0, sizeof(TCHAR) * 256 );
#ifdef _XENGLISH_SUPPORTSUBLANGUAGE



#else
	strcpy( szFilename, textstringdbfilename[g_LanguageType] );
#endif

	if( !g_StringDataBase.Load( szFilename, g_LanguageType ) )
	{
		::ShowWindow( g_hLoadingWindow, SW_HIDE );
		::DestroyWindow(g_hLoadingWindow);
		g_hLoadingWindow = NULL;
		return;
	}

#else
	if( !g_StringDataBase.Load( textstringdbfilename[g_LanguageType], g_LanguageType ) )
	{
		::ShowWindow( g_hLoadingWindow, SW_HIDE );
		::DestroyWindow(g_hLoadingWindow);
		g_hLoadingWindow = NULL;
		return;
	}
#endif

#else
	if( !g_StringDataBase.Load( _T("NDTextDB.xsd"), g_LanguageType ) )
	{
		return;
	}
#endif

#endif

	InitializeStringData();
	
	_XCharacterStateList::ReloadCharacterStateTable();
	
	_XGameItem::ReLoadNickNameInfoTable();

	if( !_XSkillItem::ReLoadSkillProperty() )
	{
		_XFatalError( _T("Failed reload skill properties") );
	}

	for( int i = 0; i < _XDEF_MODELCLASSCOUNT_MOB; ++i )
	{
		g_MonsterName[i] = g_StringDataBase.GetTextString( _XSTRINGDB_SN_MOBMODEL_XMS, i );
#ifdef _XJAPANESE
		g_MonsterHiraganaName[i] = g_StringDataBase.GetSubTextString( _XSTRINGDB_SN_MOBMODEL_XMS, i );
#endif
	}



	m_proc_firstload.InitialzeZoneTable();

	g_MainWindowManager.NotifyUIChange();
}

//=====================================================================================================================
// Utility func....
//=====================================================================================================================

void  _XUpdateWindow( void )
{
	gApp->PaintHandler();
}

void _XPlaySoundEffect( int index, D3DXVECTOR3 position, bool checkplay, bool loop )
{
	if( !g_SoundEffectFlag ) return;
		
#ifdef _XUSEFMOD
	
	g_FMODWrapper.PlaySound3D( index, position, loop );

#else
	if( index >= g_MaxSoundEffectCount ) return;
	if( !g_pSoundEffect[index] ) return;	
	FLOAT dx = (position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
	FLOAT dz = (position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
	if( _XFastDistance( dx , dz ) > 11 ) return;
	
	AUDIOVECTOR newposition;
	newposition.x = position.x;
	newposition.y = position.y;
	newposition.z = position.z;
	
	g_pSoundEffect[index]->SetPosition( newposition );
	
	if( checkplay )
	{
		if( g_pSoundEffect[index]->IsPlaying() )
		{
			g_pSoundEffect[index]->Stop();
			g_pSoundEffect[index]->Play();
		}
		else
		{
			g_pSoundEffect[index]->Play();
		}		
	}
	else
	{		
		g_pSoundEffect[index]->Play();
	}
#endif	
}

void _XPlayEnvSoundEffect( int index, D3DXVECTOR3 position, bool checkplay, bool loop )
{			
	if( !g_3DSoundEffectFlag ) return;
		
#ifdef _XUSEFMOD

	g_FMODWrapper.FL_PlaySound3D( index, position, loop );

#else
	if( index >= g_MaxSoundEffectCount ) return;
	if( !g_pSoundEffect[index] ) return;	

	FLOAT dx = (position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
	FLOAT dz = (position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
	if( _XFastDistance( dx , dz ) > 32 ) return;
	
	AUDIOVECTOR newposition;
	newposition.x = position.x;
	newposition.y = position.y;
	newposition.z = position.z;
	
	g_pSoundEffect[index]->SetPosition( newposition );
	
	if( checkplay )
	{
		if( g_pSoundEffect[index]->IsPlaying() )
		{
			g_pSoundEffect[index]->Stop();
			g_pSoundEffect[index]->Play();
		}
		else
		{
			g_pSoundEffect[index]->Play();
		}		
	}
	else
	{		
		g_pSoundEffect[index]->Play();
	}
#endif	
}

void _XPlayInterfaceSound( int index, bool checkplay )
{		
	if( !g_SoundEffectFlag ) return;
	
#ifdef _XUSEFMOD
	g_FMODWrapper.PlaySound2D( index );
#else
	if( index < 0 || index >= g_MaxInterfaceSoundEffectCount ) return;
	if( !g_pInterfaceSound[index] ) return;	

	if( checkplay )
	{
		if( g_pInterfaceSound[index]->IsPlaying() )
		{
			g_pInterfaceSound[index]->Stop();			
		}
	}
	
	g_pInterfaceSound[index]->Play();
#endif
}

void _XKernelCommandProcedure( LPSTR commandstring )
{
#ifdef _XADMINISTRATORMODE
	if(gApp) 
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.ProcRenderStateCommand( commandstring );
#endif	
}

#ifdef _XDWDEBUG
void _XDebugOutputString( LPSTR message, int attrib, int slot )
{
	if( !message ) return;
	if( strlen(message) <= 0 ) return;
	if( slot < 0 || slot > 3 ) 
	{
		slot = 0;
	}

	TCHAR* teststring = NULL;
	teststring = _strlwr( _strdup( message ) );
	if( teststring )
	{
		if( strstr(teststring,_T("WARNING")) )
		{
			attrib = 4;
		}
		else if( strstr(teststring,_T("ERROR")) )
		{
			attrib = 5;
		}
		free( teststring );
	}

	if( g_DebugOutputlist[slot].getitemcount() > 500 )
	{
		g_DebugOutputlist[slot].resetList();
		g_DebugOutputlist[slot].deleteItem(0);
	}

	_XStringItem* newmessage = new _XStringItem( message, attrib );
	g_DebugOutputlist[slot].resetList();
	g_DebugOutputlist[slot].insertItem( newmessage );

	_XWindow_Debug* pDebugWindow = 
		(_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );
	
	if( pDebugWindow && pDebugWindow->m_DebugInfoListScrollBar )
	{	
		if( pDebugWindow->m_DebugInfoMode == slot )
		{
			pDebugWindow->m_DebugInfoListScrollBar->SetTotalLineCount( g_DebugOutputlist[pDebugWindow->m_DebugInfoMode].getitemcount() );
			pDebugWindow->m_DebugInfoListScrollBar->UpdateData();
			pDebugWindow->m_DebugInfoListScrollBar->SetLineScrollPos( 999999 ); 
		}
	}	
}

HICON g_hPlayIcon;
HICON g_hPauseIcon;
HICON g_hStopIcon;

LRESULT	CALLBACK _XKernelEffectScriptWindowProc( HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam )
{
	switch (message) 
	{    
	case WM_INITDIALOG:
		{
			g_hPlayIcon = LoadIcon(gHInst, MAKEINTRESOURCE(IDI_PLAY));
			g_hPauseIcon = LoadIcon(gHInst, MAKEINTRESOURCE(IDI_PAUSE));
			g_hStopIcon = LoadIcon(gHInst, MAKEINTRESOURCE(IDI_STOP));

			HWND hwndCtl = ::GetDlgItem( hDlg, IDC_PLAYBUTTON );
			SendMessage( hwndCtl, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)g_hPlayIcon  );
			hwndCtl = ::GetDlgItem( hDlg, IDC_STOPBUTTON );
			SendMessage( hwndCtl, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)g_hStopIcon  );
			hwndCtl = ::GetDlgItem( hDlg, IDC_PAUSEBUTTON );
			SendMessage( hwndCtl, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)g_hPauseIcon  );
		}
		break;
	case WM_SIZE:
		{
			/*RECT rect;
			GetClientRect( hDlg, &rect );
			
			if( g_DebugWindowListHandle )
			{				
				MoveWindow( g_DebugWindowListHandle, 1, 1, rect.right-rect.left-2, rect.bottom-rect.top-22, TRUE );				
			}
			HWND clearbtnhandle = GetDlgItem( hDlg, IDC_CLEARBUTTON );
			if( clearbtnhandle )
			{
				RECT btnrect;
				GetClientRect( clearbtnhandle, &btnrect );
				MoveWindow( clearbtnhandle, rect.right-btnrect.right, rect.bottom-btnrect.bottom, btnrect.right, btnrect.bottom, TRUE );
			}*/
		}
		break;
    case WM_COMMAND:
		{        
			UINT wID = LOWORD(wparam);         // item, control, or accelerator identifier 
			HWND hwndCtl = (HWND) lparam;      // handle of control 
			
			switch(wID)
			{
			case IDC_LOADTEXTFORMATBUTTON :
				{
					XString filename;
					if( OpenFileDialog( filename, "Open", "TripleX Effect script files [text format]\0*.txt\0", "*.txt" ) )
					{
						int handle = open(filename, O_RDONLY | O_BINARY);
						if (handle == -1) 
						{ 
							_XMessage("Cannot open file for input!"); 
						}
						else
						{				
							int len = filelength(handle);
							
							LPTSTR ptext = new TCHAR[len+1]; // +1 so 0 length files will save
							memset(ptext, 0, len+1);
							read(handle, ptext, len);
							close(handle);

							HWND hEditControl = GetDlgItem( hDlg, IDC_SCRIPTRICHEDIT );						
							::SendMessage( hEditControl, WM_SETTEXT, 0, (LPARAM) ptext );

							delete[] ptext;
						}
					}
				}
				break;

			case IDC_SAVETEXTFORMATBUTTON :
				{
					XString filename;
					if( SaveFileDialog( filename, "Save", "TripleX Effect script files [text format]\0*.txt\0", "*.txt" ) )
					{
						HWND hEditControl = GetDlgItem( hDlg, IDC_SCRIPTRICHEDIT );
						int textsize = ::SendMessage( hEditControl, WM_GETTEXTLENGTH, 0, 0 );

						if( textsize > 0 )
						{
							int handle = open(filename, O_RDWR | O_BINARY | O_TRUNC | O_CREAT, S_IWRITE | S_IREAD);
							if (handle == -1) 
							{ 
								_XMessage("Cannot open file for output!"); 
							}
							else
							{	
								LPTSTR ptext = new TCHAR[textsize+1]; 
								memset(ptext, 0, textsize+1);

								::SendMessage( hEditControl, WM_GETTEXT, (WPARAM)textsize, (LPARAM)ptext );

								if (write(handle, ptext, textsize) != textsize) 
								{ 
									_XMessage("error writing file, probably disk full."); 
								}

								delete[] ptext;
								close(handle);
							}
						}
					}
				}
				break;
			default:
				break;
			} // switch wID
		} // WM_COMMAND block
		break;	
	case WM_DESTROY:
		{
			RECT rect;
			GetWindowRect( g_hEffectScriptWindow, &rect );
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_EFFECTSCRIPTWINDOWPOSITIONX, rect.left );
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_EFFECTSCRIPTWINDOWPOSITIONY, rect.top );		
			g_hEffectScriptWindow = NULL;			
		}
		break;		
	} // switch uMsg 
	
	return FALSE;
}

#endif

#ifdef _XUSE_HACKSHEILD

int __stdcall AhnHS_Callback(long lCode, long lParamSize, void* pParam)
{
	switch(lCode)
	{
		//Engine Callback
	case AHNHS_ENGINE_DETECT_GAME_HACK:
	case AHNHS_ENGINE_DETECT_GENERAL_HACK:
		{
			TCHAR szMsg[_MAX_PATH];
			TCHAR szFilename[_MAX_PATH];
			GetFileNameFromPathString( szFilename, (char*)pParam );

			_snprintf(szMsg, sizeof(szMsg), _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2597), szFilename);

			//MessageBox(NULL, szMsg, _T("The Nine Dragons"), MB_OK);

			CEmperorOfDragonsApp* pEODApp = ((CEmperorOfDragonsApp*)gApp);
			_XProcess* pCurrentProcess = pEODApp->m_ProcessManager.GetCurrentProcess();

			if( !pCurrentProcess )
			{
				MessageBox(NULL, szMsg, _T("The Nine Dragons"), MB_OK);
			}
			else if( pCurrentProcess == &pEODApp->m_proc_maingame )
			{
				_XMessageWindow* pNewMessageBox = _XMessageWindow::CreateMessageBox( &g_MainWindowManager, 0xFF4343 );
				pNewMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, "XKernel message", szMsg, TRUE, 0 );				
			}
			else if( pCurrentProcess == &pEODApp->m_proc_loginserver )
			{
				_XMessageWindow* pNewMessageBox = _XMessageWindow::CreateMessageBox( &pEODApp->m_proc_loginserver.m_WindowManager , 0xFF4343 );
				pNewMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, "XKernel message", szMsg, TRUE, 0 );				
			}
			else if( pCurrentProcess == &pEODApp->m_proc_selectcharacter )
			{
				_XMessageWindow* pNewMessageBox = _XMessageWindow::CreateMessageBox( &pEODApp->m_proc_selectcharacter.m_WindowManager , 0xFF4343 );
				pNewMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, "XKernel message", szMsg, TRUE, 0 );				
			}
			else if( pCurrentProcess == &pEODApp->m_proc_createcharacter )
			{
				_XMessageWindow* pNewMessageBox = _XMessageWindow::CreateMessageBox( &pEODApp->m_proc_createcharacter.m_WindowManager , 0xFF4343 );
				pNewMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, "XKernel message", szMsg, TRUE, 0 );				
			}
#ifdef _XTS_SERVERUNIFICATION
			else if( pCurrentProcess == &pEODApp->m_proc_serverunification )
			{
				_XMessageWindow* pNewMessageBox = _XMessageWindow::CreateMessageBox( &pEODApp->m_proc_serverunification.m_WindowManager , 0xFF4343 );
				pNewMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, "XKernel message", szMsg, TRUE, 0 );				
			}
#endif
			else if( pCurrentProcess == &pEODApp->m_proc_loading   ||
				     pCurrentProcess == &pEODApp->m_proc_firstload ||
					 pCurrentProcess == &pEODApp->m_proc_flash )
			{
				MessageBox(NULL, szMsg, _T("The Nine Dragons"), MB_OK);
			}
			break;
		}
		
		//이미 후킹이 이루어진 상태 
		//일부 API가 이미 후킹되어 있는 상태
		//그러나 실제로는 이를 차단하고 있기 때문에 다른 후킹시도 프로그램으 동작하지 않습니다.
		//이 Callback은 단지 경보내지는 정보제공 차원에서 사용할 수 있습니다.
	case AHNHS_ACTAPC_DETECT_ALREADYHOOKED:
		{
			PACTAPCPARAM_DETECT_HOOKFUNCTION pHookFunction = (PACTAPCPARAM_DETECT_HOOKFUNCTION)pParam;
			TCHAR szMsg[255];
			_snwprintf(szMsg, sizeof(szMsg),_T("[HACKSHIELD] Already Hooked\n- szFunctionName : %s\n- szModuleName : %s\n"), 
					 pHookFunction->szFunctionName, pHookFunction->szModuleName);
			_XLog(szMsg);
			break;
		}
		
		//Speed 관련
	case AHNHS_ACTAPC_DETECT_SPEEDHACK:
	case AHNHS_ACTAPC_DETECT_SPEEDHACK_APP:
		{
			MessageBox(NULL, _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2589), _T("The Nine Dragons"), MB_OK);
			_XLog( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2589) );
			break;
		}
		
		//디버깅 방지 
	case AHNHS_ACTAPC_DETECT_KDTRACE:	
	case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
		{
			_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2590), lCode );
			break;
		}
		
		//해킹 방지 기능 이상 
	case AHNHS_ACTAPC_DETECT_AUTOMOUSE:
	case AHNHS_ACTAPC_DETECT_DRIVERFAILED:
	case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
	case AHNHS_ACTAPC_DETECT_MESSAGEHOOK:
		{
			_XFatalError( _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2591), lCode );
			break;
		}
	}
	return 1;
}

#endif


#ifdef _XUSE_GAMEGUARD

//#pragma message( "NP Game callback" )	

/*
DWORD PreInitNPGameMon(LPCSTR szGameName)
{
	return NPGAMEMON_SUCCESS;
}

BOOL CloseNPGameMon()
{
	return true;
}
*/

// 게임 종료시에는 false를 return 해주고, 종료하지 않는 경우는 true를 return 합니다.
//BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)

//#define _XDEF_NPROTECT_GUARD_TIME_LOG
//#define _USE_NPROTECTLOG

int CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
	bool	bAppExit = false;
	TCHAR	msg[256];
	LPTSTR  lpszMsg = NULL;

#ifdef _USE_NPROTECTLOG
	_XLog( "NPGameMonCallback Insert & dwMsg : %08X ", dwMsg);
#endif

	switch (dwMsg)
    {
	case NPGAMEMON_COMM_ERROR:
		_snprintf(msg, sizeof(msg), _T("Communication error"), dwArg);
		lpszMsg = msg;
		bAppExit = true;
		break;
	case NPGAMEMON_COMM_CLOSE:
		_snprintf(msg, sizeof(msg),_T("Communication closing"), dwArg);
		lpszMsg = msg;
		bAppExit = true;
		break;
	case NPGAMEMON_INIT_ERROR:
		bAppExit = true; // 종료 코드
		_snprintf(msg, sizeof(msg), _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2592), dwArg);
		lpszMsg = msg;
		break;
	case NPGAMEMON_SPEEDHACK:
		bAppExit = true; // 종료 코드
		lpszMsg = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2593);
		break;
	case NPGAMEMON_GAMEHACK_KILLED:
		// bAppExit = true; // 종료 코드, 종료 또는 계속 진행
		lpszMsg = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2594);
		break;
	case NPGAMEMON_GAMEHACK_DETECT:
		bAppExit = true; // 종료 코드
		lpszMsg = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2595);
		break;
	case NPGAMEMON_GAMEHACK_DOUBT:
		bAppExit = true; // 종료 코드
		lpszMsg = _XGETINTERFACETEXT(ID_STRING_EMPEROROFDRAGONS_2596);
		break;

#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD) // mod by magviper.
	case NPGAMEMON_CHECK_CSAUTH2:

#ifdef _USE_NPROTECTLOG
		_XLog( "NPROTECT_GAME_GUARD : NPGAMEMON_CHECK_CSAUTH2" );
#endif

		memcpy(&g_GameGuardData.data, (PVOID)dwArg, sizeof(g_GameGuardData.data));

#ifdef _USE_NPROTECTLOG
		_XLog( "NPGAMEMON_CHECK_CSAUTH2 : %08X %08X %08X %08X ", g_GameGuardData.data.dwIndex, g_GameGuardData.data.dwValue1, g_GameGuardData.data.dwValue2, g_GameGuardData.data.dwValue3);
#endif

		g_NetworkKernel.SendPacket(MSG_NO_GGAUTH_REQ);
		break;
#endif
	default :
		_XLog( _T("GAME GUARD CALLBACK : Unknown message, E_Code:%d"), dwMsg );
		break;
	}

	if (lpszMsg)
	{
		CEmperorOfDragonsApp* pEODApp = ((CEmperorOfDragonsApp*)gApp);
		_XProcess* pCurrentProcess = pEODApp->m_ProcessManager.GetCurrentProcess();
		
		if( !pCurrentProcess )
		{
			//_XFatalError( "%s", lpszMsg );
			MessageBox(NULL, lpszMsg, _T("XKernel"), MB_OK);
		}
		else if( pCurrentProcess == &pEODApp->m_proc_maingame )
		{
			//_XFatalError( "%s", lpszMsg );
			_XMessageWindow* pNewMessageBox = _XMessageWindow::CreateMessageBox( &g_MainWindowManager, 0xFF4343 );
			pNewMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, "XKernel", lpszMsg, TRUE, 0 );				
		}
		else if( pCurrentProcess == &pEODApp->m_proc_loginserver )
		{	
			//_XFatalError( "%s", lpszMsg );
			_XMessageWindow* pNewMessageBox = _XMessageWindow::CreateMessageBox( &pEODApp->m_proc_loginserver.m_WindowManager , 0xFF4343 );
			pNewMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, "XKernel", lpszMsg, TRUE, 0 );				
		}
		else if( pCurrentProcess == &pEODApp->m_proc_selectcharacter )
		{
			//_XFatalError( "%s", lpszMsg );
			_XMessageWindow* pNewMessageBox = _XMessageWindow::CreateMessageBox( &pEODApp->m_proc_selectcharacter.m_WindowManager , 0xFF4343 );
			pNewMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, "XKernel", lpszMsg, TRUE, 0 );				
		}
		else if( pCurrentProcess == &pEODApp->m_proc_createcharacter )
		{
			//_XFatalError( "%s", lpszMsg );
			_XMessageWindow* pNewMessageBox = _XMessageWindow::CreateMessageBox( &pEODApp->m_proc_createcharacter.m_WindowManager , 0xFF4343 );
			pNewMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, "XKernel", lpszMsg, TRUE, 0 );				
		}
#ifdef _XTS_SERVERUNIFICATION
		else if( pCurrentProcess == &pEODApp->m_proc_serverunification )
		{
			//_XFatalError( "%s", lpszMsg );
			_XMessageWindow* pNewMessageBox = _XMessageWindow::CreateMessageBox( pEODApp->m_proc_serverunification.GetWindowManagerPtr() , 0xFF4343 );
			pNewMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, "XKernel", lpszMsg, TRUE, 0 );				
		}
#endif
		else if( pCurrentProcess == &pEODApp->m_proc_loading   ||
			pCurrentProcess == &pEODApp->m_proc_firstload ||
			pCurrentProcess == &pEODApp->m_proc_flash )
		{
			//_XFatalError( "%s", lpszMsg );
			MessageBox(NULL, lpszMsg, _T("XKernel"), MB_OK);
		}
	
		_XLog( "GAME GUARD ERROR : %s", lpszMsg );
	}
		
	// 게임 종료시에만 false를 리턴
	if (bAppExit)
	{
		_XMsgClose = TRUE;
		_XLog( "Send shutdown signal from gameguard callback function" );
		return false;
	}
	
    return true;
}
#endif



/*
LANGID DetectLanguage() {
	
#define MAX_KEY_BUFFER	80
	
	OSVERSIONINFO		VersionInfo;
	LANGID				uiLangID = 0;
	HKEY				hKey;
	DWORD				Type, BuffLen = MAX_KEY_BUFFER;
	TCHAR				LangKeyValue[MAX_KEY_BUFFER];
	
	
	VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if( !GetVersionEx(&VersionInfo) )
		return(0);
	
	switch( VersionInfo.dwPlatformId ) {
		// On Windows NT, Windows 2000 or higher
	case VER_PLATFORM_WIN32_NT:
		if( VersionInfo.dwMajorVersion >= 5)   // Windows 2000 or higher
			uiLangID = GetUserDefaultUILanguage();
		else {   // for NT4 check the language of ntdll.dll
			uiLangID = GetNTDLLNativeLangID();   
			if (uiLangID == 1033) {		// special processing for Honkong SAR version of NT4
				if (IsHongKongVersion()) {
					uiLangID = 3076;
				}
			}
		}
		break;
		// On Windows 95, Windows 98 or Windows ME
	case VER_PLATFORM_WIN32_WINDOWS:
		// Open the registry key for the UI language
		if( RegOpenKeyEx(HKEY_CURRENT_USER,_T("Default\\Control Panel\\Desktop\\ResourceLocale"), 0,
			KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS ) {
			// Get the type of the default key
			if( RegQueryValueEx(hKey, NULL, NULL, &Type, NULL, NULL) == ERROR_SUCCESS 
				&& Type == REG_SZ ) {
				// Read the key value
				if( RegQueryValueEx(hKey, NULL, NULL, &Type, (LPBYTE)LangKeyValue, &BuffLen) 
					== ERROR_SUCCESS ) {
					uiLangID = _ttoi(LangKeyValue);
				}
			}
			RegCloseKey(hKey);
		}				
		break;
	}
	
    if (uiLangID == 0) {
        uiLangID = GetUserDefaultLangID();
    }
    // Return the found language ID.
    return (uiLangID);
}
*/



LRESULT CALLBACK _XKernelItemMallWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		{
			// Resize the browser child window, leaving room for our buttons.
			MoveWindow(GetDlgItem(hwnd, 1000), 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
			return(0);
		}
		
	case WM_COMMAND:
		{
			return(0);
		}
		
	case WM_KEYDOWN:
		{
#ifdef _XTS_ITEMMALLBROWSER							
			DWORD scancode = (HIWORD( lParam ) & 0xFF);
			
			if( scancode == DIK_ESCAPE )
			{
				_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
				if( pCashMall_Window )
				{
					pCashMall_Window->ShowWindow( FALSE );
					
					ShowWindow(g_hItemMallWindow, SW_HIDE);
					g_bShowItemMallWindow = FALSE;
				}		
			}
			return(0);
#endif
		}

	case WM_CREATE:
		{
			// Success
			return(0);
		}
		
	case WM_CLOSE:
		{
			// Close this window. This will also cause the child window hosting the browser
			// control to receive its WM_DESTROY
			//DestroyWindow(hwnd);			
			//return(1);
			return 0;
		}
		
	case WM_DESTROY:
		{
			// Post the WM_QUIT message to quit the message loop in WinMain()
			PostQuitMessage(0);
			
			return(1);
		}
	}
	
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}





/************************** browserWindowProc() *************************
* Our message handler for our window to host the browser.
*/

LRESULT CALLBACK _XKernelItemMallWebBrowserWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		{
			// Resize the browser object to fit the window
			ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			//ResizeBrowser( hwnd, 801, 601 );
			return(0);
		}
		
	case WM_CREATE:
		{
			// Embed the browser object into our host window. We need do this only
			// once. Note that the browser object will start calling some of our
			// IOleInPlaceFrame and IOleClientSite functions as soon as we start
			// calling browser object functions in EmbedBrowserObject().
			if (EmbedBrowserObject(hwnd)) return(-1);
			
			// Success
			return(0);
		}
	case WM_KEYDOWN:
		{
#ifdef _XTS_ITEMMALLBROWSER				
			DWORD scancode = (HIWORD( lParam ) & 0xFF);
			
			if( scancode == DIK_ESCAPE )
			{
				_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
				if( pCashMall_Window )
				{
					pCashMall_Window->ShowWindow( FALSE );
					
					ShowWindow(g_hItemMallWindow, SW_HIDE);
					g_bShowItemMallWindow = FALSE;
				}		
			}
			return(0);
#endif
		}
		
	case WM_DESTROY:
		{
			// Detach the browser object from this window, and free resources.
			UnEmbedBrowserObject(hwnd);
			
			return(TRUE);
		}
	}
	
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}



#ifdef _XDEF_WEBBOARD_20061211

LRESULT CALLBACK _XKernelWebBoardWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		{
			// Resize the browser child window, leaving room for our buttons.
			MoveWindow(GetDlgItem(hwnd, 1000), 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
			return(0);
		}
		
	case WM_COMMAND:
		{
			return(0);
		}
		
	case WM_KEYDOWN:
		{					
			DWORD scancode = (HIWORD( lParam ) & 0xFF);
			
			if( scancode == DIK_ESCAPE )
			{
				_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
				if( pWebBoard_Window  )
				{
					pWebBoard_Window ->ShowWindow( FALSE );					
					ShowWindow(g_hWebBoardWindow, SW_HIDE);
				}		
			}
			else if(  scancode == DIK_X  )
			{			
				_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
				pWebBoard_Window->ShowWindow(!pWebBoard_Window->GetShowStatus());
				
				if( pWebBoard_Window->GetShowStatus() )
				{
					g_MainWindowManager.SetTopWindow( pWebBoard_Window );					
					POINT pt = { 0, 0 };
					ClientToScreen( gHWnd, &pt );
					SetWindowPos(g_hWebBoardWindow, HWND_TOPMOST, pt.x + (gnWidth>>1)-343, pt.y + (gnHeight>>1)-280, 688, 450, 0);
					
					g_bShowItemMallWindow = TRUE;
					ShowWindow(g_hWebBoardWindow, SW_SHOW);
				}
				else
				{
					g_bShowItemMallWindow = FALSE;
					ShowWindow(g_hWebBoardWindow, SW_HIDE );
				}
			}
			return(0);
		}

	case WM_CREATE:
		{
			// Success
			return(0);
		}
		
	case WM_CLOSE:
		{
			// Close this window. This will also cause the child window hosting the browser
			// control to receive its WM_DESTROY
			//DestroyWindow(hwnd);			
			//return(1);
			return 0;
		}
		
	case WM_DESTROY:
		{
			// Post the WM_QUIT message to quit the message loop in WinMain()
			PostQuitMessage(0);
			
			return(1);
		}
	}
	
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}





/************************** browserWindowProc() *************************
* Our message handler for our window to host the browser.
*/

LRESULT CALLBACK _XKernelWebBoardWebBrowserWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		{
			// Resize the browser object to fit the window
			ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			//ResizeBrowser( hwnd, 801, 601 );
			return(0);
		}
		
	case WM_CREATE:
		{
			// Embed the browser object into our host window. We need do this only
			// once. Note that the browser object will start calling some of our
			// IOleInPlaceFrame and IOleClientSite functions as soon as we start
			// calling browser object functions in EmbedBrowserObject().
			if (EmbedBrowserObject(hwnd)) return(-1);
			
			// Success
			return(0);
		}
	case WM_KEYDOWN:
		{		
			DWORD scancode = (HIWORD( lParam ) & 0xFF);
			
			if( scancode == DIK_ESCAPE )
			{
				_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
				if( pWebBoard_Window  )
				{
					pWebBoard_Window ->ShowWindow( FALSE );					
					ShowWindow(g_hWebBoardWindow, SW_HIDE);
				}		
			}
			else if(  scancode == DIK_X  )
			{
				
				_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
				pWebBoard_Window->ShowWindow(!pWebBoard_Window->GetShowStatus());
				
				if( pWebBoard_Window->GetShowStatus() )
				{
					g_MainWindowManager.SetTopWindow( pWebBoard_Window );					
					POINT pt = { 0, 0 };
					ClientToScreen( gHWnd, &pt );
					SetWindowPos(g_hWebBoardWindow, HWND_TOPMOST, pt.x + (gnWidth>>1)-343, pt.y + (gnHeight>>1)-280, 688, 450, 0);
					
					ShowWindow(g_hWebBoardWindow, SW_SHOW);
				}
				else
				{
					ShowWindow(g_hWebBoardWindow, SW_HIDE );
				}
			}
			return(0);
		}
		
	case WM_DESTROY:
		{
			// Detach the browser object from this window, and free resources.
			UnEmbedBrowserObject(hwnd);
			
			return(TRUE);
		}
	}
	
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

#endif


#ifdef _XDEF_EVENTWINDOW_WEBPAGE_20070214

LRESULT CALLBACK _XKernelWebEventWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		{
			// Resize the browser child window, leaving room for our buttons.
			MoveWindow(GetDlgItem(hwnd, 1000), 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
			return(0);
		}
		
	case WM_COMMAND:
		{
			return(0);
		}
		
	case WM_KEYDOWN:
		{					
			DWORD scancode = (HIWORD( lParam ) & 0xFF);
			
			if( scancode == DIK_ESCAPE )
			{
			}
			else if(  scancode == DIK_X  )
			{			
			}
			return(0);
		}

	case WM_CREATE:
		{
			// Success
			return(0);
		}
		
	case WM_CLOSE:
		{
			// Close this window. This will also cause the child window hosting the browser
			// control to receive its WM_DESTROY
			//DestroyWindow(hwnd);			
			//return(1);
			return 0;
		}
		
	case WM_DESTROY:
		{
			// Post the WM_QUIT message to quit the message loop in WinMain()
			PostQuitMessage(0);
			
			return(1);
		}
	}
	
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}





/************************** browserWindowProc() *************************
* Our message handler for our window to host the browser.
*/

LRESULT CALLBACK _XKernelWebEventWebBrowserWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		{
			// Resize the browser object to fit the window
			ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			//ResizeBrowser( hwnd, 801, 601 );
			return(0);
		}
		
	case WM_CREATE:
		{
			// Embed the browser object into our host window. We need do this only
			// once. Note that the browser object will start calling some of our
			// IOleInPlaceFrame and IOleClientSite functions as soon as we start
			// calling browser object functions in EmbedBrowserObject().
			if (EmbedBrowserObject(hwnd)) return(-1);
			
			// Success
			return(0);
		}
	case WM_KEYDOWN:
		{		
			DWORD scancode = (HIWORD( lParam ) & 0xFF);
			
			if( scancode == DIK_ESCAPE )
			{
			
			}
			else if(  scancode == DIK_X  )
			{				
			}
			return(0);
		}
		
	case WM_DESTROY:
		{
			// Detach the browser object from this window, and free resources.
			UnEmbedBrowserObject(hwnd);
			
			return(TRUE);
		}
	}
	
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

#endif

#ifdef _XDEF_ITEMMALL_AUTOLOGIN_SE_20070514

#include <exdisp.h>
#include <oleauto.h>

long WINAPI _XDisplayHTMLPageAndPostData(HWND hwnd, LPCTSTR webPageName, LPCTSTR postData )
{
	IWebBrowser2	*webBrowser2;
	VARIANT			myURL;
	IOleObject		*browserObject;

	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));

	if (!browserObject->QueryInterface(IID_IWebBrowser2, (void**)&webBrowser2))
	{
		VariantInit(&myURL);
		myURL.vt = VT_BSTR;
		
		wchar_t		*buffer;
		DWORD		size;
		size = MultiByteToWideChar(CP_ACP, 0, webPageName, -1, 0, 0);
		if (!(buffer = (wchar_t *)GlobalAlloc(GMEM_FIXED, sizeof(wchar_t) * size))) goto badalloc;
		MultiByteToWideChar(CP_ACP, 0, webPageName, -1, buffer, size);
		myURL.bstrVal = SysAllocString(buffer);
		GlobalFree(buffer);
		buffer = NULL;
		size = 0;

		if (!myURL.bstrVal)
		{
badalloc:	webBrowser2->Release();
			return -6;
		}

		TCHAR strHeader[] = "Content-Type: application/x-www-form-urlencoded\r\n";

		VARIANT vtHeader = {0}, vtTarget= {0}, vtEmpty= {0}, vtPostData = {0}; 
		vtHeader.vt = VT_BSTR; 
		
		size = MultiByteToWideChar(CP_ACP, 0, strHeader, -1, 0, 0);
		if (!(buffer = (wchar_t *)GlobalAlloc(GMEM_FIXED, sizeof(wchar_t) * size))) goto badalloc2;
		MultiByteToWideChar(CP_ACP, 0, strHeader, -1, buffer, size);
		vtHeader.bstrVal = SysAllocString(buffer);
		GlobalFree(buffer);

if (!myURL.bstrVal)
		{
badalloc2:	webBrowser2->Release();
			return -7;
		}
		
		::VariantInit(&vtEmpty);
		

		LPSAFEARRAY psa;		
		LPCTSTR cszPostData = (LPCTSTR)postData;
		UINT cElems = lstrlen(cszPostData);
		LPSTR pPostData;

		VariantInit(&vtPostData);
		psa = SafeArrayCreateVector(VT_UI1, 0, cElems);
		if (!psa)
			return -8;
		SafeArrayAccessData(psa, (LPVOID*)&pPostData);

		memcpy(pPostData, cszPostData, cElems);
		SafeArrayUnaccessData(psa);

		V_VT(&vtPostData) = VT_ARRAY | VT_UI1;
		V_ARRAY(&vtPostData) = psa;

		webBrowser2->put_Visible(VARIANT_TRUE);
		webBrowser2->Navigate(myURL.bstrVal, &vtEmpty, &vtEmpty, &vtPostData, &vtHeader); 
		
		SysFreeString(vtHeader.bstrVal); 		
		VariantClear(&vtPostData);
		VariantClear(&myURL);
		
		webBrowser2->Release();
	
		return 0;
	}

	return -5;
}

#endif

#ifdef _ACCLAIM_RUBICONADSYSTEM

LRESULT CALLBACK _XKernelRubiconADWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		{
			// Resize the browser child window, leaving room for our buttons.
			MoveWindow(GetDlgItem(hwnd, 1000), 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
			return(0);
		}
		
	case WM_COMMAND:
		{
			return(0);
		}
		
	case WM_KEYDOWN:
		{					
			DWORD scancode = (HIWORD( lParam ) & 0xFF);
			
			if( scancode == DIK_ESCAPE )
			{
			}
			else if(  scancode == DIK_X  )
			{			
			}
			return(0);
		}

	case WM_CREATE:
		{
			// Success
			return(0);
		}
		
	case WM_CLOSE:
		{
			// Close this window. This will also cause the child window hosting the browser
			// control to receive its WM_DESTROY
			//DestroyWindow(hwnd);			
			//return(1);
			return 0;
		}
		
	case WM_DESTROY:
		{
			// Post the WM_QUIT message to quit the message loop in WinMain()
			PostQuitMessage(0);
			
			return(1);
		}
	}
	
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

LRESULT CALLBACK _XKernelRubiconADWebBrowserWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		{
			// Resize the browser object to fit the window
			ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			//ResizeBrowser( hwnd, 801, 601 );
			return(0);
		}
		
	case WM_CREATE:
		{
			// Embed the browser object into our host window. We need do this only
			// once. Note that the browser object will start calling some of our
			// IOleInPlaceFrame and IOleClientSite functions as soon as we start
			// calling browser object functions in EmbedBrowserObject().
			if (EmbedBrowserObject(hwnd)) return(-1);
			
			// Success
			return(0);
		}
	case WM_KEYDOWN:
		{		
			DWORD scancode = (HIWORD( lParam ) & 0xFF);
			
			if( scancode == DIK_ESCAPE )
			{
			
			}
			else if(  scancode == DIK_X  )
			{				
			}
			return(0);
		}
		
	case WM_DESTROY:
		{
			// Detach the browser object from this window, and free resources.
			UnEmbedBrowserObject(hwnd);
			
			return(TRUE);
		}
	}
	
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

#endif

#ifdef _XDEF_NOTICEPOPUPCONTROL__070910_POPUP

#include <exdisp.h>
#include <oleauto.h>
#include <mshtml.h>
#include "Wininet.h"

#pragma comment(lib, "wininet.lib")

BOOL _XGetHTMLFile( LPCTSTR serveraddress, LPCTSTR FileName, LPTSTR buffer, int bufferlength )
{
	HINTERNET		hInternetSession = NULL;
	HINTERNET		hHttpConnection = NULL;
	HINTERNET		hHttpFile = NULL;
	DWORD			dwServiceType = INTERNET_SERVICE_HTTP;
	INTERNET_PORT	nPort = 80;	
	
	hInternetSession = ::InternetOpen( gApp->m_strWindowTitle, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternetSession == NULL)
	{
		_XLog(_T("Failed in call to InternetOpen, Error:%d\n"), ::GetLastError());
		return false;
	}
	
	//Make the connection to the HTTP server          
	assert(hHttpFile== NULL);

	hHttpConnection = ::InternetConnect(hInternetSession, serveraddress, nPort, NULL, 
											  NULL, dwServiceType, 0, NULL);
	if (hHttpConnection== NULL)
	{
		_XLog(_T("Failed in call to InternetConnect, Error:%d\n"), ::GetLastError());
		::InternetCloseHandle(hInternetSession);
		return false;
	}

	//Issue the request to read the file
	LPCTSTR ppszAcceptTypes[2];
	ppszAcceptTypes[0] = _T("*/*");  //We support accepting any mime file type since this is a simple download of a file
	ppszAcceptTypes[1] = NULL;
	assert(hHttpFile == NULL);
	hHttpFile = HttpOpenRequest(hHttpConnection, NULL, FileName, NULL, NULL, ppszAcceptTypes, INTERNET_FLAG_RELOAD | 
									INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION, NULL);
	if (hHttpFile == NULL)
	{
		_XLog(_T("Failed in call to HttpOpenRequest, Error:%d\n"), ::GetLastError());
		::InternetCloseHandle(hHttpConnection);
		::InternetCloseHandle(hInternetSession);		
		return false;
	}

//label used to jump to if we need to resend the request
resend:

	//Issue the request
	BOOL bSend = ::HttpSendRequest(hHttpFile, NULL, 0, NULL, 0);
	if (!bSend)
	{
		_XLog(_T("Failed in call to HttpSendRequest, Error:%d\n"), ::GetLastError());
		::InternetCloseHandle(hHttpFile);
		::InternetCloseHandle(hHttpConnection);
		::InternetCloseHandle(hInternetSession);
		return false;
	}

	//ERROR_FILE_NOT_FOUND 

	//Check the HTTP status code
	TCHAR szStatusCode[32];
	DWORD dwInfoSize = 32;
	if (!HttpQueryInfo(hHttpFile, HTTP_QUERY_STATUS_CODE, szStatusCode, &dwInfoSize, NULL))
	{
		_XLog(_T("Failed in call to HttpQueryInfo for HTTP query status code, Error:%d\n"), ::GetLastError());
		::InternetCloseHandle(hHttpFile);
		::InternetCloseHandle(hHttpConnection);
		::InternetCloseHandle(hInternetSession);
		return false;
	}
	else
	{
		long nStatusCode = _ttol(szStatusCode);

		//Handle any authentication errors
		if (nStatusCode == HTTP_STATUS_PROXY_AUTH_REQ || nStatusCode == HTTP_STATUS_DENIED)
		{
		  // We have to read all outstanding data on the Internet handle
		  // before we can resubmit request. Just discard the data.
		  char szData[51];
		  DWORD dwSize;
		  do
		  {
				::InternetReadFile(hHttpFile, (LPVOID)szData, 50, &dwSize);
		  }
		  while (dwSize != 0);

		  //Bring up the standard authentication dialog
		  if (::InternetErrorDlg(gHWnd, hHttpFile, ERROR_INTERNET_INCORRECT_PASSWORD, FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
								 FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL) == ERROR_INTERNET_FORCE_RETRY)
			goto resend;
		}
  		else if (nStatusCode != HTTP_STATUS_OK)
		{
		  _XLog(_T("Failed to retrieve a HTTP 200 status, Status Code:%d\n"), nStatusCode);
		  ::InternetCloseHandle(hHttpFile);
		  ::InternetCloseHandle(hHttpConnection);
		  ::InternetCloseHandle(hInternetSession);
		  return false;
		}
	}

	// Get the length of the file.            
	TCHAR szContentLength[256];
	memset( szContentLength, 0, sizeof(TCHAR) * 256 );
	dwInfoSize = 32;
	DWORD dwFileSize = 0;
	BOOL bGotFileSize = FALSE;
	if (::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, szContentLength, &dwInfoSize, NULL))
	{
		//Set the progress control range
		bGotFileSize = TRUE;
		dwFileSize = (DWORD) _ttol(szContentLength);
	}

	//Now do the actual read of the file
	DWORD dwBytesRead = 0;	
	DWORD dwBytesToRead = 1024;
	DWORD dwTotalBytesRead = 0;

	char  szReadBuf[1024];
	char* pszReadPoint = szReadBuf;
	memset( szReadBuf, 0, sizeof(char) * 1024 );
	
	do
	{
		if (!::InternetReadFile(hHttpFile, pszReadPoint, dwBytesToRead, &dwBytesRead))
		{
		  _XLog(_T("Failed in call to InternetReadFile, Error:%d\n"), ::GetLastError());
		  ::InternetCloseHandle(hHttpFile);
		  ::InternetCloseHandle(hHttpConnection);
		  ::InternetCloseHandle(hInternetSession);
		  return false;
		}
		else if (dwBytesRead)
		{		  
		  //Increment the total number of bytes read
		  pszReadPoint += dwBytesRead;
		  dwTotalBytesRead += dwBytesRead;
		}
	}while(dwBytesRead);

	int cpybufferlength = dwTotalBytesRead;
	if( cpybufferlength > bufferlength )
		cpybufferlength = bufferlength;

	memcpy( buffer, szReadBuf, cpybufferlength );

	// Close all open Internet handles
	::InternetCloseHandle(hHttpFile);
	::InternetCloseHandle(hHttpConnection);
	::InternetCloseHandle(hInternetSession);

	return true;
}

BOOL _XCheckValidPopupPage( void )
{
	TCHAR htmlbuffer[1024];
	memset( htmlbuffer, 0, sizeof(TCHAR) * 1024 );

	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{
		BOOL retval = _XGetHTMLFile( _T("md.ninedragons.co.kr"), _T("/popup/Gpop/Gpop.html"), htmlbuffer, 1024 );
		if( !retval )
		{
			return FALSE;
		}
		else
		{
			if( strncmp(htmlbuffer, _T("NO Image"), strlen(_T("NO Image"))) == 0 )
			{
				return FALSE;
			}
		}
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		BOOL retval = _XGetHTMLFile( _T("9dragons.acclaim.com"), _T("/game/9d-pop.htm"), htmlbuffer, 1024 );
		if( !retval )
		{
			return FALSE;
		}
		else
		{
			if( strncmp(htmlbuffer, _T("NO Image"), strlen(_T("NO Image"))) == 0 )
			{
				return FALSE;
			}
		}
	}	
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		BOOL retval = _XGetHTMLFile( _T("cuulong.zing.vn"), _T("/sukien/index.html"), htmlbuffer, 1024 );
		if( !retval )
		{
			return FALSE;
		}
		else
		{
			if( strncmp(htmlbuffer, _T("NO Image"), strlen(_T("NO Image"))) == 0 )
			{
				return FALSE;
			}
		}
	}	

	return TRUE;
}

#endif


BOOL CEmperorOfDragonsApp::LoadSubLanguageList( void )
{
	FILE* pFile = NULL;

	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

	_XLog( _T("Parse script : language list") );

	pFile = fopen( _T("Language.xml"), _T("rt") );

	if( !pFile )
	{
		_XFatalError( _T("Can't open language.xml file") );
		return FALSE;
	}

	TCHAR readbuffer[256];

	while(1)
	{
		if( feof(pFile) )
			break;

		fgets( readbuffer, 255, pFile );

		LPTSTR pFindStartPT = NULL;
		LPTSTR pFindEndPT = NULL;

		if( (pFindStartPT = strstr( readbuffer, _T("<language>") ) ) )
		{
			pFindStartPT += strlen( _T("<language>") );

			if( (pFindEndPT= strstr( readbuffer, _T("</language>") ) ) )
			{
				if( pFindEndPT-pFindStartPT > 0 )
				{
					TCHAR tempbuffer[128];
					memset( tempbuffer, 0, sizeof(TCHAR) * 128 );

					strncpy( tempbuffer, pFindStartPT, pFindEndPT-pFindStartPT );

					_XStringItem* pNewLanguage = new _XStringItem( tempbuffer, 0 );

					if( pNewLanguage )
					{
						g_SubLanguageList.resetList();
						g_SubLanguageList.insertItem( pNewLanguage );
						_XLog( _T("Language : %s"), pNewLanguage->Get_String() );
					}
					else
					{
						_XFatalError( _T("Can't create language list item") );
						break;
					}
				}
			}
		}
	}

	fclose( pFile );

	return TRUE;
}