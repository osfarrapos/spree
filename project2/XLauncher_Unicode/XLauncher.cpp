// XLauncher.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "XLauncher.h"
#include "XLauncherDlg.h"
#include "Package.h"

#include <..\src\occimpl.h>
#include "custsite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXLauncherApp

BEGIN_MESSAGE_MAP(CXLauncherApp, CWinApp)
	//{{AFX_MSG_MAP(CXLauncherApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXLauncherApp construction

CXLauncherApp::CXLauncherApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CXLauncherApp::~CXLauncherApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

void CXLauncherApp::ParseCommandLine2(CCommandLineInfo& rCmdInfo)
{
	for (int i = 1; i < __argc; i++)
	{
		LPCTSTR pszParam = __targv[i];
		BOOL bFlag = FALSE;
		BOOL bLast = ((i + 1) == __argc);
		if ( pszParam[0] == '/' )
		{
			// remove flag specifier
			bFlag = TRUE;
			++pszParam;
		}
		rCmdInfo.ParseParam(pszParam, bFlag, bLast);
	}
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXLauncherApp object

CXLauncherApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CXLauncherApp initialization
extern CString g_LogFileName;
extern FILE*   g_Logfile;
extern CString g_ExecuteParam;
BOOL		   g_MutexExitStatus = FALSE;

HMODULE g_hUnicowsModule = NULL;

HMODULE LoadUnicowsProc(void) 
{
	g_hUnicowsModule = LoadLibraryA("unicows.dll"); 
	
	return g_hUnicowsModule;
} 

#ifdef _cplusplus 
extern "C" { 
#endif 
	extern FARPROC _PfnLoadUnicows = (FARPROC) &LoadUnicowsProc; 
#ifdef _cplusplus 
} 
#endif 

BOOL CXLauncherApp::InitInstance()
{
	_PfnLoadUnicows();
	
	if( !g_hUnicowsModule )
	{
		::MessageBox( NULL, _T("Could not load unicouws.dll"), _T("Error!"), MB_OK | MB_ICONWARNING );
		return FALSE;
	}

	// Create a custom control manager class so we can overide the site
	CCustomOccManager *pMgr = new CCustomOccManager;
	
	// Create an IDispatch class for extending the Dynamic HTML Object Model 
	m_pDispOM = new CImpIDispatch;

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer(pMgr);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

#ifdef _USE_MUTEX
	
	HANDLE hMutex = NULL;

	//_XLog( FALSE, "Create launcher mutex\n" );	
	hMutex = ::CreateMutex(NULL, TRUE, _T("XLauncher ver 0.9b") );
	// Already launched.
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{		
		//_XLog( FALSE, "FAILED : Create mutex[Already exists]\n" );
		g_MutexExitStatus = TRUE;
		CloseHandle( hMutex );
		return FALSE;
	}

	if( hMutex )
	{
		::ReleaseMutex( hMutex );
	}

#endif

#ifdef  _LOGFILE_
	TCHAR curdir[ 512 ];
	GetCurrentDirectory( 511, curdir );
	g_LogFileName = curdir;
	g_LogFileName += _T("\\XLauncher.log");

	g_Logfile = _wfopen( _T("XLauncher.log"), _T("wt") );
	if(g_Logfile)
	{
		fclose( g_Logfile );
	}

	g_Logfile = NULL;	

	COleDateTime currentTime;
	currentTime = COleDateTime::GetCurrentTime();
	
	_XLog( FALSE, _T("=============================================================================\n") );
	_XLog( FALSE, _T("Startup log...\n") );
	_XLog( FALSE, _T("%d-%d-%d / %d:%d:%d \n"), currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), 
			               currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond() );
	_XLog( FALSE, _T("=============================================================================\n") );		
#endif

#ifdef _SELFPATCH_

	CCommandLineInfo cmdInfo;
	ParseCommandLine2(cmdInfo);

	_XLog( FALSE, _T("Launcher section 2...\n") );

	// Run another process

	if( wcslen( cmdInfo.m_strFileName ) == 0 ||
		cmdInfo.m_strFileName.Find( _T("kerygma@netian.com") ) < 0 )
	{	
		TCHAR	ModuleFilePath[ 512 ];
		CString ModuleFileName;
		
		memset( ModuleFilePath, 0, sizeof(TCHAR) * 512 );
		GetModuleFileName( NULL, ModuleFilePath, 511 );

		ModuleFileName = ModuleFilePath;
		
		for( int i = 511; i > 0; i-- )
		{
			if( ModuleFilePath[i] == _T('\\') ) break;
			else ModuleFilePath[i] = NULL;
		}			

		_XLog( FALSE, _T("Module path : %s\n"), ModuleFilePath );
				
		ModuleFileName.Replace( ModuleFilePath, _T("") );

#ifdef _XTESTSERVER		
		CString runfilename = _T("tsNDLauncher2.exe");
#else
		CString runfilename = _T("NDLauncher2.exe");
#endif

		// 실행한 파일과 실행할 파일 이름이 같으면 종료
		if( ModuleFileName.CompareNoCase( runfilename ) != 0 )
		{
			HANDLE      hFind;
			WIN32_FIND_DATA rFind;
			BOOL        bCheckFile = TRUE;

			hFind = FindFirstFile ( runfilename, &rFind );

			if ( INVALID_HANDLE_VALUE != hFind )
			{		
				FindClose ( hFind );
				CFileStatus filestatus;
				CFile::GetStatus( runfilename, filestatus );
				filestatus.m_attribute = 0x00; // set normal attrib
				CFile::SetStatus( runfilename, filestatus );
				
				TRY
				{
					if( CheckExistFile(runfilename,   TRUE) )
						::DeleteFile( runfilename );
				}
				CATCH( CFileException, e )
				{
					AfxMessageBox( _T("FAILED : Remove temporary launcher file") );
					return FALSE;
				}
				END_CATCH

			}
			
			BOOL bCheck = FALSE;

			TRY
			{
				_XLog( FALSE, _T("Create launcher module\n") );
				bCheck = CopyFile( ModuleFileName, runfilename, FALSE);
				_XLog( FALSE, _T("Created launcher module\n") );
			}
			CATCH( CFileException, e )
			{
				_XLog( FALSE, _T("FAILED : Create launcher file\n") );
				AfxMessageBox( _T("FAILED : Create launcher file") );
				return FALSE;
			}
			END_CATCH

			if(  bCheck )
			{			
				CFileStatus filestatus;
				CFile::GetStatus( runfilename, filestatus );
				filestatus.m_attribute = 0x02; // set hidden attrib
				CFile::SetStatus( runfilename, filestatus );

				_XLog( FALSE, _T("Run sub module\n") );

#ifdef _XACCLAIM // acclaim web launching
				CString szParam;

				szParam.Format( _T("kerygma@netian.com%s"), cmdInfo.m_strFileName );
				
				OutputDebugString( szParam );

				::ShellExecute(NULL, NULL, runfilename , szParam, NULL, SW_HIDE);
#else

	#ifdef _XNEXON_WEBLOGIN
				CString szParam;

				szParam.Format( _T("kerygma@netian.com%s"), cmdInfo.m_strFileName );

				OutputDebugString( szParam );

				::ShellExecute(NULL, NULL, runfilename , szParam, NULL, SW_HIDE);

	#else
				::ShellExecute(NULL, NULL, runfilename , _T("kerygma@netian.com") , NULL, SW_HIDE);
	#endif
#endif
			}			
		}
		
		return FALSE;
	}

#ifdef _XNEXON_WEBLOGIN
	else
	{
		int idparampos = cmdInfo.m_strFileName.Find( _T("-passport=") );

		if( idparampos >= 0 )
		{
			TCHAR szTempBuffer[2048];
			memset( szTempBuffer, 0, sizeof(TCHAR) * 2048 );

			wcscpy( szTempBuffer, cmdInfo.m_strFileName.operator LPCTSTR() + idparampos );
			g_ExecuteParam = szTempBuffer;

			if( g_ExecuteParam.GetLength() <= wcslen( _T("-passport=") ) )
			{
				AfxMessageBox( _T("웹런쳐를 이용해 접속할 경우 홈페이지 로그인 후, 게임시작 버튼을 눌러주시기 바랍니다. 자세한 사항은 공지를 참조하여 주시기 바랍니다.") );
				return FALSE;
			}
		}
		else
		{
			g_ExecuteParam = _T("");
		}
	}
#endif

#ifdef _XACCLAIM // acclaim web launching
	else
	{
		int idparampos = cmdInfo.m_strFileName.Find( _T("-id") );

		if( idparampos >= 0 )
		{
			TCHAR szTempBuffer[256];
			memset( szTempBuffer, 0, sizeof(TCHAR) * 256 );

			wcscpy( szTempBuffer, cmdInfo.m_strFileName.operator LPCTSTR() + idparampos );			
			g_ExecuteParam = szTempBuffer;

#ifdef _DEBUG
			OutputDebugString( g_ExecuteParam );
#endif
		}
		else
		{
			g_ExecuteParam = _T("");
		}
	}
#endif
	
	cmdInfo.m_strFileName = _T("");
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;

#endif

	_XLog( FALSE, _T("Start launcher sub module\n") );

	CXLauncherDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	_XLog( FALSE, _T("End of launcher sub module\n") );

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


int CXLauncherApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

#ifdef  _LOGFILE_	
	if( !g_MutexExitStatus )
	{
		COleDateTime currentTime;
		currentTime = COleDateTime::GetCurrentTime();		

		_XLog( FALSE, _T("=============================================================================\n") );
		_XLog( FALSE, _T("End of log.\n") );
		_XLog( FALSE, _T("%d-%d-%d / %d:%d:%d \n"), currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), 
							   currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond() );		
		_XLog( FALSE, _T("==================================================================Code by _X=\n") );		
	}
#endif
	
	delete m_pDispOM;
	
	return CWinApp::ExitInstance();
}
