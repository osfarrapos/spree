//////////////////////////////////////////////////////////////////////////
// XLauncherDlg.cpp : implementation file
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XLauncher.h"
#include "XLauncherDlg.h"
#include <Mmsystem.h>
#include <direct.h>
#include <fstream>
#include <afxsock.h> 
#include <shlwapi.h>        // for StrFormatByteSize()
#include "exdispid.h"
#include "fmod.h"
#include "fmod_errors.h" 

#include "Wininet.h"

#include "EnterSerialDlg.h"
#include "SeverMessageDialog.h"
#include "XRegistry.h"
#include "XEncryption.h"
#include "FTPTransferDlg.h"
#include "Package.h"
#include "DLGHTML.h"
#include "NotInstalledDialog.h"
#include "FTPOptionsDialog.h"
#include "Zip\\ZipArchive.h"
#include "ImageHlp.h"
#include "CheckVersionDlg.h"
#include "XFileDownload.h"
#include "SelectServer.h"
#include "ResChangeDialog.h"

#ifdef _XACCLAIM
#include "Markup.h"
#endif

//////////////////////////////////////////////////////////////////////////
// Define.
//////////////////////////////////////////////////////////////////////////

#define _XDEF_USETIMERSTART 
#define _XUSE_MULTITHREADDOWNLOAD

#define _XDEF_SERVERTRAFFICGAUGEMAXSIZE		85
#define _XDEF_SERVERTRAFFICGAUGETIMER		1
//#define _XDEF_SERVERTRAFFICGAUGETEST

#define _PATCH_STARTTIMERID					1001

//#define _XDEF_FIXDOWNLOADADRESS
//#define _XDEF_ADDRESSLOGGING

//////////////////////////////////////////////////////////////////////////
// Performace log macro
//////////////////////////////////////////////////////////////////////////
#define _XDEF_PERFORMANCE_CHECK				

#ifdef  _XDEF_PERFORMANCE_CHECK				
LARGE_INTEGER frequency, count1, count2;

#define _PERFORMANCECHECKSTART   if (!QueryPerformanceFrequency(&frequency)) ::MessageBox( NULL, "No performancefreq", "Error !", MB_OK );								\
							     QueryPerformanceCounter(&count1);

#define _PERFORMANCECHECKEND	 QueryPerformanceCounter(&count2);																										\
								 double timeinmicroseconds = (double)( (double)((count2.QuadPart - count1.QuadPart) * 1000 * 1000) / (double)frequency.QuadPart );		\
								 CString message;																														\
								 message.Format( "Rebuild time : %f second", timeinmicroseconds/1000000 );																\
								 ((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(message);								 
#else   

#define _PERFORMANCECHECKSTART
#define _PERFORMANCECHECKEND

#endif

#ifdef _XACCLAIM_NEWWEBPAGE
	#define _XHIDEWEBBROWSER
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define _DOWNLOADTEST
#endif

#ifdef _XRELEASEDEBUG
//#define _DOWNLOADTEST
#endif

//////////////////////////////////////////////////////////////////////////
// LS Packet
//////////////////////////////////////////////////////////////////////////

#pragma pack( push, enter_game )
#pragma pack(1)

struct _sHeader {
	short		sLength : 12 ;		// -2048 ~ 2047
	short		sCrypto : 2 ;		// 0 : false , 나머지는 true
	short		sCompressed : 2 ;	// 0 : false , 나머지는 true
};

struct _ForestInfo {
	int			nVersion;
	bool		bServerRun;		
	char		cForestState[_COUNT_MAX_GROUP_] ;	//	-2, Maintenance -1 : Off ,  0~100 Normal
};

#define MSG_NO_LOGIN_FOREST_LIST		5			// Packet number


struct MSG_LOGIN_FOREST_LIST
{	
	_ForestInfo	ForestInfo ;						// Server list & version number
};

#pragma pack( pop, enter_game )


//////////////////////////////////////////////////////////////////////////
// Global define
//////////////////////////////////////////////////////////////////////////

TCHAR	g_Installedpath[_REGSTRING_MAXLENGTH+1];

CSocket g_DownloadSocket; 

CString g_ExecuteParam;

BOOL	g_FirstRun = TRUE;

int		g_CurrentDownloadMethod = 0;

/*
TCHAR	g_szServerNameString[18][40] = 
{
		_T("01. 의천 서버"),
		_T("02. 청홍 서버"),
		_T("03. 용봉 서버"),
		_T("04. 도룡 서버"),
		_T("05. 칠성 서버"),
		_T("06. 청운 서버"),
		_T("07. 청룡 서버"),
		_T("08. 백호 서버"),
		_T("09. 주작 서버"),
		_T("10. 현무 서버"),
		_T("11. 천지 서버"),
		_T("12. 태극 서버"),
		_T("13. Server 13"),
		_T("14. Server 14"),
		_T("15. Server 15"),
		_T("16. Server 16"),
		_T("17. Server 17"),
		_T("18. Server 18")
};
*/
TCHAR	g_szServerNameString[18][40] = 
{
		_T("01. Server 1"),
		_T("02. Server 2"),
		_T("03. Server 3"),
		_T("04. Server 4"),
		_T("05. Server 5"),
		_T("06. Server 6"),
		_T("07. Server 7"),
		_T("08. Server 8"),
		_T("09. Server 9"),
		_T("10. Server 10"),
		_T("11. Server 11"),
		_T("12. Server 12"),
		_T("13. Server 13"),
		_T("14. Server 14"),
		_T("15. Server 15"),
		_T("16. Server 16"),
		_T("17. Server 17"),
		_T("18. Server 18")
};

TCHAR g_InterfaceString[128][1024];


#ifdef _XTESTSERVER
CString g_NoticeWebURL = _T("");// = _T("http://www.ninedragons.co.kr/launchpop/notice.asp");
CString g_UpdateWebURL = _T("");// = _T("http://www.ninedragons.co.kr/launchpop/update2.asp");
#else
CString g_NoticeWebURL = _T("");// = _T("http://www.ninedragons.co.kr/launchpop/notice.asp");
CString g_UpdateWebURL = _T("");// = _T("http://www.ninedragons.co.kr/launchpop/update.asp");
#endif

//////////////////////////////////////////////////////////////////////////
// IP Address handler
//////////////////////////////////////////////////////////////////////////
BOOL _XIsIPv4Address(char *pHost, int nLen)
{	
    int i, nIndex;	
    bool bDigitOK;	
    int nDigit;
	
    if (pHost == NULL)		
        goto _false;
	
    i = 0;	
    for (nIndex = 1; nIndex <= 4; nIndex++)		
    {		
        bDigitOK = false;		
        nDigit = 0;
		
        while (i < nLen)			
        {			
            if (pHost[i] < '0' || pHost[i] > '9')				
                break;
			
            bDigitOK = true;			
            nDigit = nDigit * 10 + (pHost[i] - '0');			
            i++;
			
            if (nDigit > 255)				
                goto _false;			
        }
		
        if (!bDigitOK)			
            goto _false;
		
        if (nDigit > 255)			
            goto _false;
		
        if (nIndex < 4)			
        {			
            if (pHost[i] != '.')				
            {				
                goto _false;				
            }			
            i++; // skip '.'			
        } 
		else 
		{			
            if (i != nLen)				
            {				
                goto _false;				
            }			
        }		
    }
	
    return TRUE;
	
_false:	
    return FALSE;
}

BOOL _XGetHostName( char* pHostName, char* pIPAdress, int _length ) 
{
	PHOSTENT pHostInfo;
	
	if((pHostInfo = gethostbyname(pHostName)) != NULL)
	{
		memset( pIPAdress, 0, _length );
		strcpy( pIPAdress, inet_ntoa(*(struct in_addr *)*pHostInfo->h_addr_list) );
		
		return TRUE;
	}
	
	return FALSE;		
}


//////////////////////////////////////////////////////////////////////////
// Log utility
//////////////////////////////////////////////////////////////////////////

#ifdef  _LOGFILE_

FILE* g_Logfile = NULL;
CString  g_LogFileName;

void	_XLog( BOOL insertlistview, LPCTSTR msg, ... )
{
		if( g_Logfile )
		{
			fclose(g_Logfile);
			g_Logfile = NULL;
		}

		g_Logfile = _wfopen( g_LogFileName, _T("at") );

		TCHAR buf[512];
		memset( buf, 0, sizeof(TCHAR) * 512 );
	
		va_list ap;

		if(msg)
		{ 
			va_start(ap, msg);
			vswprintf(buf, msg, ap);
			va_end(ap);

			if( g_Logfile )
			{		 
				fputws( buf, g_Logfile );
				//fputws( "\n", g_Logfile );			 
			}
		}
		
		if( g_Logfile )
		{
			fclose(g_Logfile);
			g_Logfile = NULL;
		}

/*		if( insertlistview )
		{	
			CXLauncherDlg* pLauncherdlg = (CXLauncherDlg*)AfxGetMainWnd();
			if( pLauncherdlg )
			{
				if( pLauncherdlg->m_Initialized )
				{
					pLauncherdlg->InsertStringToProgressMessageListCtrl( buf );
				}
			}
		}
*/
}

#endif

#define RTN_OK 0
#define RTN_USAGE 1
#define RTN_ERROR 13

#include <lmerr.h>

void DisplayErrorText( DWORD dwLastError )
{
    HMODULE hModule = NULL; // default to system source
    LPTSTR MessageBuffer;
    DWORD dwBufferLength;
	
    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM ;
	    
    // If dwLastError is in the network range, load the message source.	
    if(dwLastError >= NERR_BASE && dwLastError <= MAX_NERR) {
        hModule = LoadLibraryEx(
            _T("netmsg.dll"),
            NULL,
            LOAD_LIBRARY_AS_DATAFILE
            );
		
        if(hModule != NULL)
            dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
    }
	
    // Call FormatMessage() to allow for message 
    //  text to be acquired from the system 
    //  or from the supplied module handle.
    
	
    if(dwBufferLength = FormatMessage(
        dwFormatFlags,
        hModule, // module to get message from (NULL == system)
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
        (LPTSTR) &MessageBuffer,
        0,
        NULL
        ))
    {
        // Output message string on stderr.
		AfxMessageBox( MessageBuffer, MB_OK );     
		
        // Free the buffer allocated by the system.
        LocalFree(MessageBuffer);
    }
	    
    // If we loaded a message source, unload it.
    if(hModule != NULL)
        FreeLibrary(hModule);
}

//////////////////////////////////////////////////////////////////////////
// Server & patch version manager : reg value.
//////////////////////////////////////////////////////////////////////////

_XServerAddressManager	g_ServerAddressManager;
_XPatchVersionManager	g_PatchVersionManager;


//////////////////////////////////////////////////////////////////////////
// Server address manager
void _XServerAddressManager::XOREncrypt( void )
{
	LPBYTE pServerInfo = (LPBYTE)&m_ServerInfo;

	for( int i = 0; i < sizeof(_XSERVERINFOSTRUCT); i++ )
	{
		pServerInfo[i] ^= _XADDRESSENCODEKEY;
	}
}

BOOL _XServerAddressManager::GetServerInfo( void )
{

#ifdef _XTESTSERVER
	m_ServerInfo.testserverindicator = _XDEF_TESTSERVERSTRUCTINDICATOR;

#ifdef _XTAIWANESE
	#ifdef _XTW_CSTESTVERSION
		strcpy( m_ServerInfo.patchserveraddress, "219.84.173.38" );
		strcpy( m_ServerInfo.serveraddress, "219.84.173.38" );
		strcpy( m_ServerInfo.cdnserverurl, "patch.9ds.com.tw" );
	#else
		strcpy( m_ServerInfo.patchserveraddress, "login.9ds.com.tw" );
		strcpy( m_ServerInfo.serveraddress, "login.9ds.com.tw" );
		strcpy( m_ServerInfo.cdnserverurl, "patch.9ds.com.tw" );
	#endif
#else
	#ifdef _XVIETNAMESE
		strcpy( m_ServerInfo.patchserveraddress, "login.cuulong.com.vn" );
		strcpy( m_ServerInfo.serveraddress, "login.cuulong.com.vn" );
		strcpy( m_ServerInfo.cdnserverurl, "dl.sg.vinagame.com.vn" );
	#else
		#ifdef _XACCLAIM
			strcpy( m_ServerInfo.patchserveraddress, "69.89.80.51" );
			strcpy( m_ServerInfo.serveraddress, "69.89.80.51" );
			strcpy( m_ServerInfo.cdnserverurl, "9dragons.acclaimdownloads.com/9dragons" );
		#else
			#ifdef _XPWC
				strcpy( m_ServerInfo.patchserveraddress, "85.17.55.207" );
				strcpy( m_ServerInfo.serveraddress, "85.17.55.207" );
				strcpy( m_ServerInfo.cdnserverurl, "85.17.55.204" );				
			#else
				#ifdef _XJAPANESE
					strcpy( m_ServerInfo.patchserveraddress, "login.9dragon.jp" );
					strcpy( m_ServerInfo.serveraddress, "login.9dragon.jp" );
					strcpy( m_ServerInfo.cdnserverurl, "rocworks.nefficient.jp/rocworks/9dragons" );
				#else
					#ifdef _XRUSSIAN
						strcpy( m_ServerInfo.patchserveraddress, "77.221.141.138" );
						strcpy( m_ServerInfo.serveraddress, "77.221.141.138" );
						strcpy( m_ServerInfo.cdnserverurl, "download.ninedragons.ru" );
					#else
						strcpy( m_ServerInfo.patchserveraddress, "211.39.155.196" );
						strcpy( m_ServerInfo.serveraddress, "211.39.155.196" );
						strcpy( m_ServerInfo.cdnserverurl, "download.ninedragons.co.kr" );
					#endif
				#endif
			#endif
		#endif
	#endif
#endif
	m_ServerInfo.patchserverport = 50001;
	m_ServerInfo.serverport = 48300;
	
	XOREncrypt();	
	BOOL retval = g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "ts ndc info", (LPBYTE)&m_ServerInfo, (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT), TRUE );	
	XOREncrypt();
	
	if( m_ServerInfo.testserverindicator != _XDEF_TESTSERVERSTRUCTINDICATOR )
	{
		memset( &m_ServerInfo, 0, sizeof(_XSERVERINFOSTRUCT) );
		AfxMessageBox( _T("Invalid server information : Modified registry data") );
		_XLog( TRUE, _T("Invalid server information : Modified registry data\n") );
		return retval;
	}

#ifdef _XDEF_ADDRESSLOGGING
	_XLog( FALSE, "Get server informations : %s\n", m_ServerInfo.serveraddress );
#endif

#ifdef _XDEF_FIXDOWNLOADADRESS
	strcpy( m_ServerInfo.cdnserverurl, "download.ninedragons.co.kr" );
#endif
	
	return retval;
#else

#ifdef _XTAIWANESE
	#ifdef _XTW_CSTESTVERSION
		strcpy( m_ServerInfo.patchserveraddress, "219.84.173.38" );
		strcpy( m_ServerInfo.serveraddress, "219.84.173.38" );
		strcpy( m_ServerInfo.cdnserverurl, "patch.9ds.com.tw" );
	#else
		strcpy( m_ServerInfo.patchserveraddress, "login.9ds.com.tw" );
		strcpy( m_ServerInfo.serveraddress, "login.9ds.com.tw" );
		strcpy( m_ServerInfo.cdnserverurl, "patch.9ds.com.tw" );
		//strcpy( m_ServerInfo.patchserveraddress, "219.84.173.18" );
		//strcpy( m_ServerInfo.serveraddress, "219.84.173.18" );
		//strcpy( m_ServerInfo.cdnserverurl, "patch.9ds.com.tw" );
	#endif
#else
	#ifdef _XVIETNAMESE
		strcpy( m_ServerInfo.patchserveraddress, "login.cuulong.com.vn" );
		strcpy( m_ServerInfo.serveraddress, "login.cuulong.com.vn" );
		strcpy( m_ServerInfo.cdnserverurl, "dl.sg.vinagame.com.vn" );
	#else
		#ifdef _XACCLAIM
			strcpy( m_ServerInfo.patchserveraddress, "69.89.80.51" );
			strcpy( m_ServerInfo.serveraddress, "69.89.80.51" );
			strcpy( m_ServerInfo.cdnserverurl, "9dragons.acclaimdownloads.com/9dragons" );
		#else
			#ifdef _XPWC
				strcpy( m_ServerInfo.patchserveraddress, "85.17.55.207" );
				strcpy( m_ServerInfo.serveraddress, "85.17.55.207" );
				strcpy( m_ServerInfo.cdnserverurl, "85.17.55.204" );				
			#else
				#ifdef _XJAPANESE
					strcpy( m_ServerInfo.patchserveraddress, "login.9dragon.jp" );
					strcpy( m_ServerInfo.serveraddress, "login.9dragon.jp" );
					strcpy( m_ServerInfo.cdnserverurl, "rocworks.nefficient.jp/rocworks/9dragons" );
				#else		
					#ifdef _XRUSSIAN
						strcpy( m_ServerInfo.patchserveraddress, "77.221.141.138" );
						strcpy( m_ServerInfo.serveraddress, "77.221.141.138" );
						strcpy( m_ServerInfo.cdnserverurl, "download.ninedragons.ru" );
					#else
						strcpy( m_ServerInfo.patchserveraddress, "login.ninedragons.co.kr" );
						strcpy( m_ServerInfo.serveraddress, "login.ninedragons.co.kr" );
						strcpy( m_ServerInfo.cdnserverurl, "download.ninedragons.co.kr" );
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

	m_ServerInfo.patchserverport = 50001;
	m_ServerInfo.serverport = 48300;
	
	XOREncrypt();
	BOOL retval = g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "ndc info", (LPBYTE)&m_ServerInfo, (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT), TRUE );	
	XOREncrypt();

#ifdef _XDEF_FIXDOWNLOADADRESS
	strcpy( m_ServerInfo.cdnserverurl, "download.ninedragons.co.kr" );
#endif

	return retval;
#endif
	
}

void _XServerAddressManager::SetServerAddress( LPSTR address )
{
	GetServerInfo();
	strcpy( m_ServerInfo.serveraddress, address );

	XOREncrypt();

#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "ts ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif

	XOREncrypt();
}

void _XServerAddressManager::SetPatchServerAddress( LPSTR address )
{
	GetServerInfo();	
	strcpy( m_ServerInfo.patchserveraddress, address );
	
	XOREncrypt();

#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "ts ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif

	XOREncrypt();
}

void _XServerAddressManager::SetServerPort( DWORD port )
{
	GetServerInfo();	
	m_ServerInfo.serverport = port;
	
	XOREncrypt();

#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "ts ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif

	XOREncrypt();
}

void _XServerAddressManager::SetPatchServerPort( DWORD port )
{
	GetServerInfo();	
	m_ServerInfo.patchserverport = port;
	
	XOREncrypt();

#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "ts ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif

	XOREncrypt();
}

void _XServerAddressManager::SetCDNServerUrl( LPSTR url )
{
	GetServerInfo();	
	strcpy( m_ServerInfo.cdnserverurl , url );
	
	XOREncrypt();

#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "ts ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif

	XOREncrypt();
}

//////////////////////////////////////////////////////////////////////////
// Patch version manager
//////////////////////////////////////////////////////////////////////////

void _XPatchVersionManager::XOREncrypt( void )
{
	LPBYTE pServerInfo = (LPBYTE)&m_VersionInfo;
	
	for( int i = 0; i < sizeof(_XPATCHVERSIONINFOSTRUCT); i++ )
	{
		pServerInfo[i] ^= _XPATCHVERSIONENCODEKEY;
	}
}

DWORD _XPatchVersionManager::GetExeVersion( void )
{
	return (((BYTE)g_PatchVersionManager.m_VersionInfo.exeversion_1) << 24) |
		   (((BYTE)g_PatchVersionManager.m_VersionInfo.exeversion_2) << 16) |
		   (((USHORT)g_PatchVersionManager.m_VersionInfo.exeversion_4) );
}

BOOL _XPatchVersionManager::GetVersionInfo( void )
{
	memset( &m_VersionInfo, 0, sizeof(_XPATCHVERSIONINFOSTRUCT) );

	// Read installed path
	CString szMainProgramPath;	
	
	if( wcslen( g_Installedpath ) <= 0 )
	{
		memset( g_Installedpath, 0, sizeof(TCHAR)*(_REGSTRING_MAXLENGTH+1) );		
		g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_INSTALLEDPATH, _T(""), g_Installedpath, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, FALSE);
		
		if( wcslen( g_Installedpath ) <= 0 )
		{
			memset( g_Installedpath, 0, sizeof(TCHAR)*(_REGSTRING_MAXLENGTH+1) );
			GetCurrentDirectory( _REGSTRING_MAXLENGTH, g_Installedpath );
			g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_INSTALLEDPATH, g_Installedpath );
		}
	}
	
	szMainProgramPath = g_Installedpath;
	
#ifdef _XTESTSERVER
	szMainProgramPath += _T("\\tsNineDragons.EXE");
#else
	szMainProgramPath += _T("\\NineDragons.EXE");
#endif
	
	// Read program version
	DWORD h1,h2,l1,l2;
	CXLauncherDlg::_XGetExeFileVersion( szMainProgramPath.GetBuffer(512), h1,h2,l1,l2 );
	
#ifdef _XTESTSERVER		

	m_VersionInfo.testserverindicator = _XDEF_TESTSERVERSTRUCTINDICATOR;

	if( !g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT), FALSE ) )
	{
		XOREncrypt();
		m_VersionInfo.patchversion_1 = 205;
		m_VersionInfo.patchversion_2 = rand() % 32700;
		XOREncrypt();
	}
#else
	
	if( !g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT), FALSE ) )
	{
		#ifdef _XVIETNAMESE
			m_VersionInfo.patchversion_1 = 24;
		#else 
			#ifdef _XTAIWANESE
				#ifdef _XTW_CSTESTVERSION
					m_VersionInfo.patchversion_1 = 8;
				#else
					m_VersionInfo.patchversion_1 = 0;
				#endif
			#else
				#ifdef _XACCLAIM
					m_VersionInfo.patchversion_1 = 55;
				#else
					#ifdef _XPWC
						m_VersionInfo.patchversion_1 = 0;
					#else
						#ifdef _XJAPANESE
							m_VersionInfo.patchversion_1 = 0;
						#else
							#ifdef _XRUSSIAN
								m_VersionInfo.patchversion_1 = 0;
							#else
								m_VersionInfo.patchversion_1 = 297;
							#endif
						#endif
					#endif
				#endif
			#endif
		#endif

		m_VersionInfo.patchversion_2 = rand() % 32700;

		m_VersionInfo.exeversion_1 = h1;
		m_VersionInfo.exeversion_2 = h2;
		m_VersionInfo.exeversion_3 = l1;
		m_VersionInfo.exeversion_4 = l2;

		XOREncrypt();
#ifdef _XTESTSERVER
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#endif
		XOREncrypt();
	}
	else
	{
		XOREncrypt();
		if( m_VersionInfo.patchversion_1 > 1000 )
		{
			#ifdef _XVIETNAMESE
				m_VersionInfo.patchversion_1 = 24;
			#else 
				#ifdef _XTAIWANESE
					#ifdef _XTW_CSTESTVERSION
						m_VersionInfo.patchversion_1 = 8;
					#else
						m_VersionInfo.patchversion_1 = 0;
					#endif
				#else
					#ifdef _XACCLAIM
						m_VersionInfo.patchversion_1 = 55;
					#else
						#ifdef _XPWC
							m_VersionInfo.patchversion_1 = 0;
						#else
							#ifdef _XJAPANESE
								m_VersionInfo.patchversion_1 = 0;
							#else
								#ifdef _XRUSSIAN
									m_VersionInfo.patchversion_1 = 0;
								#else
									m_VersionInfo.patchversion_1 = 297;
								#endif
							#endif
						#endif
					#endif
				#endif
			#endif
			_XLog( FALSE, _T("Invalid version: reset patch number\n") );
		}

		m_VersionInfo.patchversion_2 = rand() % 32700;

		XOREncrypt();
#ifdef _XTESTSERVER
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#endif
		XOREncrypt();
	}
#endif
	
	m_VersionInfo.exeversion_1 = h1;
	m_VersionInfo.exeversion_2 = h2;
	m_VersionInfo.exeversion_3 = l1;
	m_VersionInfo.exeversion_4 = l2;
	
	XOREncrypt();

#ifdef _XTESTSERVER
	BOOL retval = g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT), TRUE );
#else
	BOOL retval = g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT), TRUE );
#endif

	XOREncrypt();
	
	if( m_VersionInfo.exeversion_1 != h1 ||
		m_VersionInfo.exeversion_2 != h2 ||
		m_VersionInfo.exeversion_4 != l2 )
	{
		XOREncrypt();
#ifdef _XTESTSERVER
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#endif
		XOREncrypt();
	}

	return retval;
}

void _XPatchVersionManager::SetPatchNumber( DWORD version )
{
	GetVersionInfo();	
	m_VersionInfo.patchversion_1 = version;
	m_VersionInfo.patchversion_2 = rand() % 32700;
	
	XOREncrypt();
#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#endif
	XOREncrypt();
}

void _XPatchVersionManager::SetExeVersionnumber( DWORD v1, DWORD v2, DWORD v3, DWORD v4 )
{
	GetVersionInfo();	
	
	m_VersionInfo.exeversion_1 = v1;
	m_VersionInfo.exeversion_2 = v2;
	m_VersionInfo.exeversion_3 = v3;
	m_VersionInfo.exeversion_4 = v4;
	
	XOREncrypt();
#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#endif
	XOREncrypt();
}

//////////////////////////////////////////////////////////////////////////
// Play sound util
//////////////////////////////////////////////////////////////////////////
void _XPlayPatchCompleteSound( void )
{
	DWORD flags = SND_NODEFAULT | SND_NOWAIT | SND_RESOURCE | SND_SYNC;
	::PlaySound(MAKEINTRESOURCE(IDR_PATCHCOMPLETEWAVE), ::GetModuleHandle(NULL), flags);
}

//////////////////////////////////////////////////////////////////////////
// _XResetFileAttribute
//////////////////////////////////////////////////////////////////////////
BOOL _XResetFileAttribute(LPCTSTR lpszPath)
{
	if( NULL == lpszPath ) return FALSE;
	
    CString strWildcard(lpszPath);	
    strWildcard += _T("\\*.*");
	
    CFileFind finder;
    BOOL bWorking = finder.FindFile(strWildcard);
	
    while (bWorking)
	{
        bWorking = finder.FindNextFile();
		
        if (finder.IsDots())
            continue;
		
        if (finder.IsDirectory()) 
		{
            if( _XResetFileAttribute( finder.GetFilePath() ) )
			{
				return TRUE;
			}
			
            continue;
        }
		
		SetFileAttributes( finder.GetFilePath(), FILE_ATTRIBUTE_ARCHIVE );
    }
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//////////////////////////////////////////////////////////////////////////

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXLauncherDlg dialog

CXLauncherDlg::CXLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXLauncherDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXLauncherDlg)
	m_TextureQuality_Terrain = 2;
	m_TextureQuality_Character = 2;
	m_SightRange = 1;
	m_TextureQuality_Shadow = 1;
	m_UseHardwareCursor = 0;
	m_SoundOutPutType = 0;
	m_SoundDriverType = 0;
	m_SoundMixRate = 0;
	m_DownloadMethod = -1;
	m_SubLanguageIndex = -1;
	m_AutoRunIndex = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bPasv = FALSE;
	m_bUsePreconfig = TRUE;
	m_bUseProxy = FALSE;
	m_sProxy = _T("");
	m_dbLimit = 40.F;
	
	m_progmax		= 0;
	m_percent		= -1;
	mf_downloading	= false;
	m_statusstring = _T("");

	m_CurrentTabMode = _XTABMODE_UPDATEMESSAGE;
	m_UpdateComplete = FALSE;
	m_DownloadComplete = FALSE;

	m_MaxServerCount = _COUNT_MAX_GROUP_;
	m_SelectedServerIndex = -1;

	m_ViewUpdateInformationMode = FALSE;
	
	m_ViewServerList = FALSE;

	srand( (unsigned)time( NULL ) );
	
	for( int serverindex = 0; serverindex < m_MaxServerCount; serverindex++ )
	{
		m_ServerTrafficFactor[serverindex] = 0;
		m_NextServerTrafficFactor[serverindex] = 0;

#ifdef _XDEF_SERVERTRAFFICGAUGETEST
		m_NextServerTrafficFactor[serverindex] = rand() % 121;

		if( m_NextServerTrafficFactor[serverindex] > 100 ) 
			m_NextServerTrafficFactor[serverindex] = 100;
#endif

	}

	m_ControlClickedCount = 0;
	m_ShiftClickedCount = 0;
	m_TestServerConnectMode = FALSE;
	m_OnePatchIncreaseFactor = 0;

	//_XGetServerInfo();

	m_Initialized = FALSE;
}

void CXLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXLauncherDlg)		
	DDX_Control(pDX, IDC_SOUNDSAMPLERATEOPTIONSTATIC3, m_AutoRunStatic);
	DDX_Control(pDX, IDC_AUTORUNCOMBO, m_AutoRunComboBox);
	DDX_Control(pDX, IDC_LANGUAGECOMBO, m_LanguageComboBox);
	DDX_Control(pDX, IDC_SOUNDSAMPLERATEOPTIONSTATIC2, m_LanguageStatic);
	DDX_Control(pDX, IDC_CHANGERESOLUTION, m_ChangeResolution);
	DDX_Control(pDX, IDC_DOWNLOADMETHODSTATIC, m_DownloadMethodStatic);
	DDX_Control(pDX, IDC_DOWNLOADMETHODCOMBO, m_DownloadMethodCombo);
	DDX_Control(pDX, IDC_COLOGOSTATIC, m_CologoStatic);
	DDX_Control(pDX, IDC_SOUNDTITLESTATIC, m_SoundOptionTitle);
	DDX_Control(pDX, IDC_GRAPHICSTITLESTATIC, m_GraphicOptionTitle);
	DDX_Control(pDX, IDC_OPTIONTITLESTATIC, m_OptionTitleStatic);
	DDX_Control(pDX, IDC_SOUNDSAMPLERATEOPTIONSTATIC, m_SoundSampleRateOptionStatic);
	DDX_Control(pDX, IDC_SOUNDDRIVEROPTIONSTATIC, m_SoundDeviceOptionStatic);
	DDX_Control(pDX, IDC_SOUNDOUTPUTDEVICEOPTIONSTATIC, m_SoundOutputOptionStatic);
	DDX_Control(pDX, IDC_CURSOROPTIONSTATIC, m_CursorOptionStatic);
	DDX_Control(pDX, IDC_SHADOWOPTIONSTATIC, m_ShadowOptionStatic);
	DDX_Control(pDX, IDC_SIGHTOPTIONSTATIC, m_SightOptionStatic);
	DDX_Control(pDX, IDC_MODELOPTIONSTATIC, m_ModelOptionStatic);
	DDX_Control(pDX, IDC_TERRAINOPTIONSTATIC, m_TerrainOptionStatic);
	DDX_Control(pDX, IDC_UPDATETITLESTATIC, m_UpdateTitleStatic);
	DDX_Control(pDX, IDC_UPDATESTATIC, m_UpdateStatic);
	DDX_Control(pDX, IDC_NOTICESTATIC, m_NoticeStatic);
	DDX_Control(pDX, IDC_MIXRATECOMBO, m_SoundMixRateCombo);
	DDX_Control(pDX, IDC_SOUNDDRIVERCOMBO, m_SoundDriverTypeCombo);
	DDX_Control(pDX, IDC_SOUNDOUTPUTTYPECOMBO, m_SoundOutputTypeCombo);
	DDX_Control(pDX, IDC_USEHARDWARECURSORCOMBO, m_UseHardWareCursorCombo);
	DDX_Control(pDX, IDC_SHADOWQUALITYCOMBO, m_ShadowQualityCombo);
	DDX_Control(pDX, IDC_VIEWSIGHTCOMBO, m_ViewSightRangeCombo);
	DDX_Control(pDX, IDC_TERRAINQUALITYCOMBO, m_TerrainQualityComboBox);
	DDX_Control(pDX, IDC_CHARACTERQUALITYCOMBO, m_CharacterQualityComboBox);
	DDX_Control(pDX, IDC_FLASHSTATIC, m_FlashStatic);
	DDX_Control(pDX, IDC_HIDEPATCHINFO_BUTTON, m_HidePatchInfoButton);
	DDX_Control(pDX, IDC_SHOWPATCHINFO_BUTTON, m_ShowPatchInfoButton);
	DDX_Control(pDX, IDC_SERVERREFRESH_BUTTON, m_ServerListRefreshButton);	
	DDX_Control(pDX, IDC_SERVERLISTCLOSE_BUTTON, m_ServerListCloseButton);	
	DDX_Control(pDX, IDC_OPTION_OK_BUTTON, m_OptionOKButton);
	DDX_Control(pDX, IDC_OPTION_MIDDLE_BUTTON, m_OptionMiddleButton);
	DDX_Control(pDX, IDC_OPTION_LOW_BUTTON, m_OptionLowButton);
	DDX_Control(pDX, IDC_OPTION_HIGH_BUTTON, m_OptionHighButton);
	DDX_Control(pDX, IDC_OPTION_CANCEL_BUTTON, m_OptionCancelButton);	
	DDX_Control(pDX, IDC_PROGRESSRATESTATIC, m_ProgressRateStatic);
	DDX_Control(pDX, IDC_LAUNCHERTITLESTATIC, m_LauncherTitleStatic);
	DDX_Control(pDX, IDC_UPDATEMORESTATIC, m_UpdateMoreStatic);
	DDX_Control(pDX, IDC_NOTICEMORESTATIC, m_NoticeMoreStatic);
	DDX_Control(pDX, IDC_CLOSEBUTTON, m_CloseButton);
	DDX_Control(pDX, IDC_DOWNLOADPROGRESS, m_DownloadProgress);
	DDX_Control(pDX, IDC_TOTALUPDATEPROGRESS, m_TotalUpdateProgress);
	DDX_Control(pDX, IDC_UPDATEPROGRESS, m_UpdateProgress);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, ID_SETUPBUTTON, m_SetupButton);
	DDX_Control(pDX, ID_RUNBUTTON, m_RunButton);
	DDX_Control(pDX, ID_SELECTSERVERBUTTON, m_SelectServerButton );
	DDX_Control(pDX, IDC_STATICUPDATETEXT, m_ctlUpdateText);
	DDX_Control(pDX, ID_JOINBUTTON, m_JoinButton);
	DDX_Control(pDX, IDC_SERVERMESSAGELIST, m_ServerMessageList);
	DDX_CBIndex(pDX, IDC_TERRAINQUALITYCOMBO, m_TextureQuality_Terrain);
	DDX_CBIndex(pDX, IDC_CHARACTERQUALITYCOMBO, m_TextureQuality_Character);
	DDX_CBIndex(pDX, IDC_VIEWSIGHTCOMBO, m_SightRange);
	DDX_CBIndex(pDX, IDC_SHADOWQUALITYCOMBO, m_TextureQuality_Shadow);
	DDX_CBIndex(pDX, IDC_USEHARDWARECURSORCOMBO, m_UseHardwareCursor);	
	DDX_CBIndex(pDX, IDC_SOUNDOUTPUTTYPECOMBO, m_SoundOutPutType);
	DDX_CBIndex(pDX, IDC_SOUNDDRIVERCOMBO, m_SoundDriverType);
	DDX_CBIndex(pDX, IDC_MIXRATECOMBO, m_SoundMixRate);
	DDX_CBIndex(pDX, IDC_DOWNLOADMETHODCOMBO, m_DownloadMethod);
	DDX_CBIndex(pDX, IDC_LANGUAGECOMBO, m_SubLanguageIndex);
	DDX_CBIndex(pDX, IDC_AUTORUNCOMBO, m_AutoRunIndex);
	//}}AFX_DATA_MAP
#ifndef _XHIDEWEBBROWSER	
	DDX_Control(pDX, IDC_NOTICEEXPLORER, m_NoticeListBrowser);
	#ifndef _XPWC
		DDX_Control(pDX, IDC_UPDATEEXPLORER, m_UpdateListBrowser);
	#endif
#else
	#ifdef _XACCLAIM_NEWWEBPAGE
		DDX_Control(pDX, IDC_NOTICEEXPLORER, m_NoticeListBrowser);
	#endif
#endif
}

BEGIN_MESSAGE_MAP(CXLauncherDlg, CDialog)
	//{{AFX_MSG_MAP(CXLauncherDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()	
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_SETUPBUTTON, OnSetupbutton)
	ON_BN_CLICKED(ID_RUNBUTTON, OnRunbutton)
	ON_BN_CLICKED(ID_JOINBUTTON, OnJoinbutton)
	ON_BN_CLICKED(ID_SELECTSERVERBUTTON, OnSelectServerbutton)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CLOSEBUTTON, OnClosebutton)
	ON_BN_CLICKED(IDC_NOTICEMORESTATIC, OnNoticemorestatic)
	ON_BN_CLICKED(IDC_UPDATEMORESTATIC, OnUpdatemorestatic)
	ON_BN_CLICKED(IDC_OPTION_HIGH_BUTTON, OnOptionHighButton)
	ON_BN_CLICKED(IDC_OPTION_MIDDLE_BUTTON, OnOptionMiddleButton)
	ON_BN_CLICKED(IDC_OPTION_LOW_BUTTON, OnOptionLowButton)
	ON_BN_CLICKED(IDC_OPTION_OK_BUTTON, OnOptionOkButton)
	ON_BN_CLICKED(IDC_OPTION_CANCEL_BUTTON, OnOptionCancelButton)
	ON_BN_CLICKED(IDC_SERVERREFRESH_BUTTON, OnServerrefreshButton)
	ON_BN_CLICKED(IDC_SHOWPATCHINFO_BUTTON, OnShowDetailPatchInfoButton)
	ON_BN_CLICKED(IDC_HIDEPATCHINFO_BUTTON, OnHideDetailPatchInfoButton)
	ON_CBN_SELENDOK(IDC_TERRAINQUALITYCOMBO, OnSelendokTerrainqualitycombo)
	ON_CBN_SELENDOK(IDC_CHARACTERQUALITYCOMBO, OnSelendokCharacterqualitycombo)
	ON_CBN_SELENDOK(IDC_VIEWSIGHTCOMBO, OnSelendokViewsightcombo)
	ON_CBN_SELENDOK(IDC_SHADOWQUALITYCOMBO, OnSelendokShadowqualitycombo)
	ON_CBN_SELENDOK(IDC_USEHARDWARECURSORCOMBO, OnSelendokUsehardwarecursorcombo)
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_CBN_SELENDOK(IDC_SOUNDOUTPUTTYPECOMBO, OnSelendokSoundoutputtypecombo)
	ON_CBN_SELENDOK(IDC_SOUNDDRIVERCOMBO, OnSelendokSounddrivercombo)
	ON_CBN_SELENDOK(IDC_MIXRATECOMBO, OnSelendokMixratecombo)
	ON_CBN_SELENDOK(IDC_DOWNLOADMETHODCOMBO, OnSelendokDownloadmethodcombo)
	ON_BN_CLICKED(IDC_CHANGERESOLUTION, OnChangeresolution)
	ON_CBN_SELENDOK(IDC_LANGUAGECOMBO, OnSelendokLanguagecombo)
	ON_CBN_SELENDOK(IDC_AUTORUNCOMBO, OnSelendokAutoruncombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXLauncherDlg message handlers

BOOL CXLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LoadString();

	// Initialize windows socket
	int			nReturn		= 0;
	WSADATA		wsadata		= {0};
	
	_XLog( TRUE, _T("WSAStartup...\n") );
	nReturn = WSAStartup(MAKEWORD(2,2), &wsadata);
	if (nReturn != 0)
	{
		_XLog( TRUE, _T("ERROR : Failed WSAStartup. Error No.%d\n"), WSAGetLastError());
		EndDialog(IDOK);
		return FALSE;
	}

	TCHAR currentpath[_REGSTRING_MAXLENGTH];
	memset( currentpath, 0, sizeof(TCHAR) * _REGSTRING_MAXLENGTH );
	GetCurrentDirectory( _REGSTRING_MAXLENGTH, currentpath );
	g_RegManager.SetRegistryFolder(currentpath);

	if( !CheckExistFile( _T("ndreg.xrg"), FALSE ) )
	{
		g_RegManager.ImportWindowsRegistry();
	}
	else
	{
		g_RegManager.LoadRegistry();
	}

	_XLog( FALSE, _T("Trace start...\n") );

	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	_XLog( FALSE, _T("Create menu\n") );
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	_XLog( FALSE, _T("Create windows object\n") );

	m_brushHollow = (HBRUSH) GetStockObject (HOLLOW_BRUSH);
		
	m_TotalUpdateProgress.MoveWindow(138, 397, 346, 4);	
	m_DownloadProgress.MoveWindow(138, 403, 346, 4);
	m_UpdateProgress.MoveWindow(138, 409, 346, 4);
		
	m_UpdateProgress.SetRange(0, 100);
	m_UpdateProgress.SetBitmapChannel( IDB_PATCHSLIDERBACKBITMAP, IDB_PATCHSLIDERBITMAP, !TRUE );	
	m_UpdateProgress.SetRange( 0, 100 );	
	m_UpdateProgress.SetPos( 0 );
	m_UpdateProgress.SetMarginLeft( 0 );
	m_UpdateProgress.SetMarginRight( 0 );
	m_UpdateProgress.SetPageSize( 25 );

	m_TotalUpdateProgress.SetBitmapChannel( IDB_PATCHSLIDERBACKBITMAP, IDB_PATCHSLIDERBITMAP, !TRUE );	
	m_TotalUpdateProgress.SetRange( 0, 100 );	
	m_TotalUpdateProgress.SetPos( 0 );
	m_TotalUpdateProgress.SetMarginLeft( 0 );
	m_TotalUpdateProgress.SetMarginRight( 0 );
	m_TotalUpdateProgress.SetPageSize( 25 );
	
	m_DownloadProgress.SetBitmapChannel( IDB_PATCHSLIDERBACKBITMAP, IDB_PATCHSLIDERBITMAP, !TRUE );	
	m_DownloadProgress.SetRange( 0, 100 );	
	m_DownloadProgress.SetPos( 0 );
	m_DownloadProgress.SetMarginLeft( 0 );
	m_DownloadProgress.SetMarginRight( 0 );
	m_DownloadProgress.SetPageSize( 25 );

	this->MoveWindow(0, 0, 700, 420);

	RECT rect;
	GetClientRect(&rect);	
	ModifyStyle(WS_CAPTION, NULL);
	MoveWindow(&rect); 
	
	GetWindowRect( &rect );	
	this->MoveWindow( 0, 0, 700, 420 );	
	this->CenterWindow();

	m_CologoStatic.MoveWindow(7, 384, 68, 33);

	m_OptionTitleStatic.MoveWindow(295, 35, 392, 12);
	m_OptionTitleStatic.SetTransparent(TRUE);
	m_OptionTitleStatic.SetTextColor(RGB(227, 227, 227));
	m_OptionTitleStatic.SetWindowText( g_InterfaceString[52] );

	m_SoundSampleRateOptionStatic.MoveWindow(497, 137, 79, 14);
	m_SoundSampleRateOptionStatic.SetTransparent(TRUE);
	m_SoundSampleRateOptionStatic.SetTextColor(RGB(227, 227, 227));
	m_SoundSampleRateOptionStatic.SetWindowText( g_InterfaceString[53] );

	m_SoundDeviceOptionStatic.MoveWindow(497, 114, 79, 14);
	m_SoundDeviceOptionStatic.SetTransparent(TRUE);
	m_SoundDeviceOptionStatic.SetTextColor(RGB(227, 227, 227));
	m_SoundDeviceOptionStatic.SetWindowText( g_InterfaceString[54] );

	m_SoundOutputOptionStatic.MoveWindow(497, 90, 79, 14);
	m_SoundOutputOptionStatic.SetTransparent(TRUE);
	m_SoundOutputOptionStatic.SetTextColor(RGB(227, 227, 227));
	m_SoundOutputOptionStatic.SetWindowText( g_InterfaceString[55] );

	m_CursorOptionStatic.MoveWindow(314, 183, 86, 14);
	m_CursorOptionStatic.SetTransparent(TRUE);
	m_CursorOptionStatic.SetTextColor(RGB(227, 227, 227));
	//m_CursorOptionStatic.SetWindowText( g_InterfaceString[56] );
	m_CursorOptionStatic.SetWindowText( g_InterfaceString[65] ); // Changed resolution button

#ifdef _XACCLAIM
	m_LanguageStatic.MoveWindow(497, 183, 79, 14);
	m_LanguageStatic.SetTransparent(TRUE);
	m_LanguageStatic.SetTextColor(RGB(227, 227, 227));	
	m_LanguageStatic.SetWindowText( g_InterfaceString[69] );
#endif

#ifdef _XVIETNAMESE
	m_CursorOptionStatic.ShowWindow( SW_SHOW );
#else
	#ifdef _XPWC
		m_CursorOptionStatic.ShowWindow( SW_SHOW );
	#else
		#ifdef _XACCLAIM
			m_CursorOptionStatic.ShowWindow( SW_SHOW );
		#else
			#ifdef _XKOREAN
				m_CursorOptionStatic.ShowWindow( SW_SHOW );
			#else
				#ifdef _XJAPANESE
					m_CursorOptionStatic.ShowWindow( SW_SHOW );
				#else
					#ifdef _XRUSSIAN
						m_CursorOptionStatic.ShowWindow( SW_SHOW );
					#else
						m_CursorOptionStatic.ShowWindow( SW_HIDE );
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

#ifdef _XRUSSIAN
	m_DownloadMethodStatic.MoveWindow(357, 214, 180, 14);
#else
	m_DownloadMethodStatic.MoveWindow(417, 214, 130, 14);
#endif
	m_DownloadMethodStatic.SetTransparent(TRUE);
	m_DownloadMethodStatic.SetTextColor(RGB(227, 227, 227));
	m_DownloadMethodStatic.SetWindowText( g_InterfaceString[64] );

	m_ShadowOptionStatic.MoveWindow(314, 160, 86, 14);
	m_ShadowOptionStatic.SetTransparent(TRUE);
	m_ShadowOptionStatic.SetTextColor(RGB(227, 227, 227));
	m_ShadowOptionStatic.SetWindowText( g_InterfaceString[57] );

	m_SightOptionStatic.MoveWindow(314, 137, 86, 14);
	m_SightOptionStatic.SetTransparent(TRUE);
	m_SightOptionStatic.SetTextColor(RGB(227, 227, 227));
	m_SightOptionStatic.SetWindowText( g_InterfaceString[58] );

	m_ModelOptionStatic.MoveWindow(314, 114, 86, 14);
	m_ModelOptionStatic.SetTransparent(TRUE);
	m_ModelOptionStatic.SetTextColor(RGB(227, 227, 227));
	m_ModelOptionStatic.SetWindowText( g_InterfaceString[59] );

	m_TerrainOptionStatic.MoveWindow(314, 90, 86, 14);
	m_TerrainOptionStatic.SetTransparent(TRUE);
	m_TerrainOptionStatic.SetTextColor(RGB(227, 227, 227));
	m_TerrainOptionStatic.SetWindowText( g_InterfaceString[60] );

	m_GraphicOptionTitle.MoveWindow(314, 65, 183, 14);
	m_GraphicOptionTitle.SetTransparent(TRUE);
	m_GraphicOptionTitle.SetTextColor(RGB(251, 182, 111));
	m_GraphicOptionTitle.SetWindowText( g_InterfaceString[61] );

	m_SoundOptionTitle.MoveWindow(496, 65, 183, 14);
	m_SoundOptionTitle.SetTransparent(TRUE);
	m_SoundOptionTitle.SetTextColor(RGB(251, 182, 111));
	m_SoundOptionTitle.SetWindowText( g_InterfaceString[62] );

	m_SoundOptionTitle.ShowWindow( SW_HIDE );
	m_GraphicOptionTitle.ShowWindow( SW_HIDE );
	m_OptionTitleStatic.ShowWindow( SW_HIDE );
	m_SoundSampleRateOptionStatic.ShowWindow( SW_HIDE );
	m_SoundDeviceOptionStatic.ShowWindow( SW_HIDE );
	m_SoundOutputOptionStatic.ShowWindow( SW_HIDE );
	m_CursorOptionStatic.ShowWindow( SW_HIDE );
	m_ShadowOptionStatic.ShowWindow( SW_HIDE );
	m_SightOptionStatic.ShowWindow( SW_HIDE );
	m_ModelOptionStatic.ShowWindow( SW_HIDE );
	m_TerrainOptionStatic.ShowWindow( SW_HIDE );
	m_UpdateTitleStatic.ShowWindow( SW_HIDE );
	m_DownloadMethodStatic.ShowWindow( SW_HIDE );
//#ifdef _XACCLAIM
	m_LanguageStatic.ShowWindow( SW_HIDE );
//#endif


	m_TerrainQualityComboBox.MoveWindow( 401, 86, 80, 15 );
	m_TerrainQualityComboBox.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_TerrainQualityComboBox.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_TerrainQualityComboBox.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_TerrainQualityComboBox.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));
	m_TerrainQualityComboBox.ShowWindow( SW_HIDE );
	m_TerrainQualityComboBox.AddString( g_InterfaceString[29] );
	m_TerrainQualityComboBox.AddString( g_InterfaceString[30] );
	m_TerrainQualityComboBox.AddString( g_InterfaceString[31] );

	m_CharacterQualityComboBox.MoveWindow( 401, 109, 80, 15 );
	m_CharacterQualityComboBox.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_CharacterQualityComboBox.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_CharacterQualityComboBox.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_CharacterQualityComboBox.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));
	m_CharacterQualityComboBox.ShowWindow( SW_HIDE );
	m_CharacterQualityComboBox.AddString( g_InterfaceString[29] );
	m_CharacterQualityComboBox.AddString( g_InterfaceString[30] );
	m_CharacterQualityComboBox.AddString( g_InterfaceString[31] );

	m_ViewSightRangeCombo.MoveWindow( 401, 132, 80, 15 );
	m_ViewSightRangeCombo.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_ViewSightRangeCombo.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_ViewSightRangeCombo.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_ViewSightRangeCombo.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));
	m_ViewSightRangeCombo.ShowWindow( SW_HIDE );
	m_ViewSightRangeCombo.AddString( g_InterfaceString[29] );
	m_ViewSightRangeCombo.AddString( g_InterfaceString[30] );
	m_ViewSightRangeCombo.AddString( g_InterfaceString[31] );

	m_ShadowQualityCombo.MoveWindow( 401, 155, 80, 15 );
	m_ShadowQualityCombo.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_ShadowQualityCombo.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_ShadowQualityCombo.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_ShadowQualityCombo.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));
	m_ShadowQualityCombo.ShowWindow( SW_HIDE );
	m_ShadowQualityCombo.AddString( g_InterfaceString[29] );
	m_ShadowQualityCombo.AddString( g_InterfaceString[30] );
	m_ShadowQualityCombo.AddString( g_InterfaceString[31] );

	m_UseHardWareCursorCombo.MoveWindow( 401, 178, 80, 15 );
	m_UseHardWareCursorCombo.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_UseHardWareCursorCombo.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_UseHardWareCursorCombo.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_UseHardWareCursorCombo.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));
	m_UseHardWareCursorCombo.ShowWindow( SW_HIDE );
	m_UseHardWareCursorCombo.AddString( g_InterfaceString[32] );
	m_UseHardWareCursorCombo.AddString( g_InterfaceString[33] );
	// Sound properties

	m_SoundOutputTypeCombo.MoveWindow( 581, 86, 100, 15 );
	m_SoundOutputTypeCombo.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_SoundOutputTypeCombo.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_SoundOutputTypeCombo.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_SoundOutputTypeCombo.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));
	m_SoundOutputTypeCombo.ShowWindow( SW_HIDE );
	
	m_SoundDriverTypeCombo.MoveWindow( 581, 109, 100, 15 );
	m_SoundDriverTypeCombo.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_SoundDriverTypeCombo.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_SoundDriverTypeCombo.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_SoundDriverTypeCombo.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));
	m_SoundDriverTypeCombo.ShowWindow( SW_HIDE );
	
	m_SoundMixRateCombo.MoveWindow( 581, 132, 100, 15 );  
	m_SoundMixRateCombo.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_SoundMixRateCombo.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_SoundMixRateCombo.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_SoundMixRateCombo.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));
	m_SoundMixRateCombo.ShowWindow( SW_HIDE );

	m_LanguageComboBox.MoveWindow( 581, 178, 100, 15 );
	m_LanguageComboBox.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_LanguageComboBox.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_LanguageComboBox.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_LanguageComboBox.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));
	m_LanguageComboBox.ShowWindow( SW_HIDE );

	m_DownloadMethodCombo.MoveWindow( 532, 210, 150, 15 );
	m_DownloadMethodCombo.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_DownloadMethodCombo.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_DownloadMethodCombo.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_DownloadMethodCombo.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));
	m_DownloadMethodCombo.ShowWindow( SW_HIDE );

#ifdef _XACCLAIM
	if( !LoadLanguageList() )
	{
		return FALSE;
	}
#endif

	_XLog( FALSE, _T("FMOD : Check FMOD version\n") );

	g_CurrentDownloadMethod = m_DownloadMethod = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_DOWNLOADMETHOD, 0, TRUE );
	m_DownloadMethodCombo.SetCurSel( m_DownloadMethod );

	if (FSOUND_GetVersion() < FMOD_VERSION)
	{
		TCHAR messagebuffer[512];
		wsprintf( messagebuffer, _T("You are using the wrong FMOD.DLL version!  You should be using FMOD %.02f\n"), FMOD_VERSION );
		AfxMessageBox( messagebuffer, MB_OK );
		_XLog( FALSE, messagebuffer );
		InsertStringToServerMessageListCtrl( messagebuffer );
		EndDialog(IDOK);
        return FALSE;
	}

	m_SoundOutputTypeCombo.AddString( _T("Direct sound 8.0") );
	m_SoundOutputTypeCombo.AddString( _T("Windows wave out") );
	m_SoundOutputTypeCombo.SetCurSel(m_SoundOutPutType);

	_XLog( FALSE, _T("FMOD : Set output device\n") );

	switch (m_SoundOutPutType)
    {
	case 0 :  FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
		break;
	case 1 :  FSOUND_SetOutput(FSOUND_OUTPUT_WINMM);
		break;
	case 2 :  FSOUND_SetOutput(FSOUND_OUTPUT_ASIO);
		break;
	default:
	case 3 :  FSOUND_SetOutput(FSOUND_OUTPUT_NOSOUND);
		break;
    }

	_XLog( FALSE, _T("FMOD : Enumelate sound driver\n") );

	{

		TCHAR widestring[1024];
		for (int i=0; i < FSOUND_GetNumDrivers(); i++) 
		{
			int strlength = strlen( FSOUND_GetDriverName(i) );
			memset( widestring, 0, sizeof(TCHAR) * 1024 );
			int requirelength = MultiByteToWideChar(CP_ACP, 0, FSOUND_GetDriverName(i), strlength, widestring, 1024);
						
			m_SoundDriverTypeCombo.AddString( widestring );
		}
	}
	if( FSOUND_GetNumDrivers() <= 0 )
	{
		TCHAR messagebuffer[512];
		wsprintf( messagebuffer, g_InterfaceString[0] );
		AfxMessageBox( messagebuffer, MB_OK );
		_XLog( FALSE, messagebuffer );
		InsertStringToServerMessageListCtrl( messagebuffer );
		EndDialog(IDOK);
		return FALSE;
	}
	else
	{
		if( FSOUND_GetNumDrivers() <= m_SoundDriverType )
		{
			m_SoundDriverType = 0;
			m_SoundDriverTypeCombo.SetCurSel( m_SoundDriverType );
		}
		else
		{
			m_SoundDriverTypeCombo.SetCurSel( m_SoundDriverType );
		}
	}
	
	m_SoundMixRateCombo.AddString( _T("44,100 Hz") );
	m_SoundMixRateCombo.AddString( _T("22,050 Hz") );
	m_SoundMixRateCombo.SetCurSel( m_SoundMixRate );

#ifdef _XNEXON_WEBLOGIN

	m_AutoRunComboBox.MoveWindow( 581, 178, 100, 15 );
	m_AutoRunComboBox.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_AutoRunComboBox.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_AutoRunComboBox.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_AutoRunComboBox.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));
	m_AutoRunComboBox.ShowWindow( SW_HIDE );

	m_AutoRunComboBox.AddString( _T("수동실행") );
	m_AutoRunComboBox.AddString( _T("자동실행") );

	m_AutoRunStatic.MoveWindow(497, 183, 79, 14);
	m_AutoRunStatic.SetTransparent(TRUE);
	m_AutoRunStatic.SetTextColor(RGB(227, 227, 227));	
	m_AutoRunStatic.SetWindowText( _T("패치 후") );
	
	m_AutoRunIndex			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_AUTORUN, 0, TRUE );
#else
	
#endif
	m_AutoRunComboBox.ShowWindow( SW_HIDE );

	m_UseHardwareCursor			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_USEHARDWARECURSOR, 0, TRUE );	
	m_TextureQuality_Terrain	= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_TERRAIN, 2, TRUE );
	m_TextureQuality_Character	= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_CHARACTER, 2, TRUE );
	m_TextureQuality_Shadow		= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_CHARACTERSHADOW, 2, TRUE );	
	m_SightRange				= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SIGHTLEVEL_RANGE, 1, TRUE );
	//m_ViewCharacterShadow		= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_VIEWCHARACTERSHADOW, 1, TRUE );		
	
	m_SoundOutPutType			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SOUNDOUTPUTTYPE, 0, TRUE );
	m_SoundDriverType			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SOUNDDRIVERTYPE, 0, TRUE );
	m_SoundMixRate				= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SOUNDMIXRATE, 0, TRUE );
	
	UpdateData( FALSE );
	
	_XLog( FALSE, _T("Initialize background Flash\n") );

#ifdef _XTESTSERVER
	CString titlestring;
	titlestring = g_InterfaceString[45];
	titlestring += g_InterfaceString[1];	
	m_LauncherTitleStatic.SetWindowText( titlestring );
	m_LauncherTitleStatic.MoveWindow( 10, 3, 400, 14 );
#else
	m_LauncherTitleStatic.MoveWindow( 10, 3, 400, 14 );
	m_LauncherTitleStatic.SetWindowText( g_InterfaceString[45] );
#endif
	
	m_LauncherTitleStatic.SetTextColor(RGB(96,96,96))
						 .SetBkColor( RGB(255,255,255) )
						 //.SetText3DHiliteColor( RGB(64,64,64) )
						 .SetTransparent(TRUE)
						 .SetFont3D( TRUE );

	srand( (unsigned)time( NULL ) );
	int randomselectedgroup = rand() % 4;

#ifdef _XVIETNAMESE 
	char flashfilename[9][24] = 
	{
		"vn_sorim.swf",
		"vn_nokrim.swf",
		"vn_beegoong.swf",
		"vn_gaebang.swf"
	};
#else
	#ifdef _XTAIWANESE
		char flashfilename[9][24] = 
		{
			"tw_sorim.swf",
			"tw_nokrim.swf",
			"tw_beegoong.swf",
			"tw_gaebang.swf"
		};
	#else
		#ifdef _XENGLISH
			char flashfilename[9][24] = 
			{
				"us_sorim.swf",
				"us_nokrim.swf",
				"us_beegoong.swf",
				"us_gaebang.swf"
			};
		#else
			#ifdef _XJAPANESE
				char flashfilename[9][24] = 
				{
					"jp_sorim.swf",
					"jp_nokrim.swf",
					"jp_beegoong.swf",
					"jp_gaebang.swf"
				};	
			#else
				#ifdef _XRUSSIAN
					char flashfilename[9][24] = 
					{
						"rs_sorim.swf",
						"rs_nokrim.swf",
						"rs_beegoong.swf",
						"rs_gaebang.swf"
					};	
				#else
					char flashfilename[9][24] = 
					{
						"sorim.swf",
						"nokrim.swf",
						"beegoong.swf",
						"gaebang.swf"
					};
				#endif
			#endif
		#endif
	#endif
#endif

	m_FlashStatic.MoveWindow( -1, 20, 268, 368 );
	m_FlashStatic.ShowWindow( SW_SHOW );
	m_FlashStatic.OpenFile( flashfilename[ randomselectedgroup ] );
	m_FlashStatic.m_bHighQuality = FALSE;
	if( m_FlashStatic.flashWin )
		m_FlashStatic.flashWin->SetLowQuality();
	m_FlashStatic.RedrawWindow();

	_XLog( FALSE, _T("Initialize background Image\n") );

	CDC* pDC = GetDC();
	if( pDC )
	{	
		CPackage package;
		if( !package.Open( _T("XLauncher.xp") ) )
		{
			EndDialog(IDOK);
			return FALSE;
		}
#ifdef _XTAIWANESE
		TCHAR borderimagename[9][24] = 
		{
			_T("TW_LBorder_sorim.ppm"),
			_T("TW_LBorder_nokrim.ppm"),
			_T("TW_LBorder_beegoong.ppm"),
			_T("TW_LBorder_gaebang.ppm")
		};
#else
		TCHAR borderimagename[9][24] = 
		{
			_T("LBorder_sorim.ppm"),
			_T("LBorder_nokrim.ppm"),
			_T("LBorder_beegoong.ppm"),
			_T("LBorder_gaebang.ppm")
		};
#endif

		m_BorderImage.LoadPPM( package.GetPackedFile( borderimagename[ randomselectedgroup ] ) );
		
		m_BackBufferImage.Create( m_BorderImage.m_Size.cx, m_BorderImage.m_Size.cy );
				
		m_NoticeSmallBorderImage.LoadXI( package.GetPackedFile(_T("Notice.XI") ) );
		m_UpdateSmallBorderImage.LoadXI( package.GetPackedFile(_T("Update.XI") ) );		
		m_OptionBorderImage.LoadXI( package.GetPackedFile(_T("Option.XI") ) );		
		m_ServerSelectBorderImage.LoadXI( package.GetPackedFile(_T("SelectServer.XI") ) );
		m_UpdateTitleImage.LoadXI( package.GetPackedFile(_T("UpdateTitle.XI") ) );
		
		/*
		m_UpdateTitleImage.Create( pDC, IDB_UPDATETITLEBARBITMAP );
		m_UpdateTitleImage.SaveXI( "UpdateTitle.XI" );
		m_NoticeSmallBorderImage.Create( pDC, IDB_NOTICESMALLBITMAP );
		m_NoticeSmallBorderImage.SaveXI( "Notice.XI" );
		m_UpdateSmallBorderImage.Create( pDC, IDB_UPDATESMALLBITMAP );
		m_UpdateSmallBorderImage.SaveXI( "Update.XI" );
		m_OptionBorderImage.Create( pDC, IDB_OPTIONBORDERBITMAP );
		m_OptionBorderImage.SaveXI( "Option.XI" );
		*/
				
		//m_ServerSelectBorderImage.Create( pDC, IDB_SERVERSELECTBORDERBITMAP );
		//m_ServerSelectBorderImage.SaveXI( "SelectServer.XI" );

		m_PopupBorderImage.Create( pDC, IDB_POPUPBORDERBITMAP );
		m_ProgressBarBackBorderImage.Create( pDC, IDB_PROGRESSBARBACKBORDERBITMAP );
		m_ProgressbarBackBorderTitleImage.Create( pDC, IDB_PROGRESSBARBACKBORDERTITLE );
		
		m_NoticeDialog.Create(IDD_NOTICEDIALOG, this);
		
		package.Close();

		m_ServerFullImage.Create( pDC, IDB_SERVERFULLBITMAP );

		CString messagestring;

		//int PatchNumber = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHNUMBER, 0, TRUE);
		g_PatchVersionManager.GetVersionInfo();		
		int PatchNumber = g_PatchVersionManager.m_VersionInfo.patchversion_1;

		m_SelectedServerIndex = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "Last selected server", 0, FALSE );	
		messagestring.Format( g_InterfaceString[2], PatchNumber );
		m_UpdatedMessageImage.CreateFromText( pDC, messagestring.GetBuffer(256), RGB( 255, 193, 0 ), 300 );
		
		ReleaseDC( pDC );
	}

	_XLog( FALSE, _T("Initialize web browser control\n") );

	g_NoticeWebURL = g_InterfaceString[49];
	
	_XLog( FALSE, _T("read UI string 1\n") );
#ifdef _XTESTSERVER	
	g_UpdateWebURL = g_InterfaceString[51];
#else
	g_UpdateWebURL = g_InterfaceString[50];	
#endif

	_XLog( FALSE, _T("Move notice browser window\n") );
#ifdef _XHIDEWEBBROWSER
	
#ifdef _XACCLAIM_NEWWEBPAGE
	m_NoticeListBrowser.MoveWindow( 293, 51, 394, 315 );
	
	_XLog( FALSE, _T("connect to web : phase 1\n") );
	m_NoticeListBrowser.Navigate( g_NoticeWebURL );

#else
	m_NoticeListBrowser.MoveWindow( 1000, 1000, 184, 74 );
	//m_NoticeListBrowser.Navigate( _T("about:blank") );
#endif

#else

#ifdef _XPWC // PWC: expand notice window
	m_NoticeListBrowser.MoveWindow( 294, 289-21, 393, 74+21 );
#else
	m_NoticeListBrowser.MoveWindow( 294, 289, 184, 74 );
#endif
	_XLog( FALSE, _T("connect to web : phase 1\n") );
	m_NoticeListBrowser.Navigate( g_NoticeWebURL );
#endif
	
	_XLog( FALSE, _T("Move update notice browser window\n") );
#ifdef _XHIDEWEBBROWSER
	m_UpdateListBrowser.MoveWindow( 1000, 1000, 184, 74 );
	//m_UpdateListBrowser.Navigate( _T("about:blank") );
#else	
	m_UpdateListBrowser.MoveWindow( 503, 289, 184, 74 );
	
	_XLog( FALSE, _T("connect to web : phase 2\n") );
	m_UpdateListBrowser.Navigate( g_UpdateWebURL );
#endif

#ifdef _XPWC // PWC : hide update list window
	m_UpdateListBrowser.MoveWindow( 1000, 1000, 184, 74 );
	m_UpdateListBrowser.ShowWindow( SW_HIDE );
#endif
	
	_XLog( FALSE, _T("Set browser static\n") );
#ifdef _XVIETNAMESE
	m_NoticeMoreStatic.MoveWindow( 425, 272, 52, 12 );
#else
	#ifdef _XENGLISH
		m_NoticeMoreStatic.MoveWindow( 445, 270, 52, 12 );
	#else
		#ifdef _XRUSSIAN
			m_NoticeMoreStatic.MoveWindow( 435, 270, 52, 12 );
		#else
			m_NoticeMoreStatic.MoveWindow( 445, 275, 32, 12 );
		#endif
	#endif
#endif
	_XLog( FALSE, _T("Link web page : Phase 1\n") );
	m_NoticeMoreStatic.SetLink(TRUE,FALSE)
					  .EnableUseLink(FALSE)
					  .SetTextColor(RGB( 255, 193, 0 ))
					  .SetBkColor( RGB(0,0,0) )
					  .SetFontUnderline(TRUE)
					  .SetTransparent(TRUE)
					  .SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HANDCURSOR));
	m_NoticeMoreStatic.SetWindowText( g_InterfaceString[63] );

#ifdef _XTAIWANESE
	m_NoticeMoreStatic.ShowWindow( SW_HIDE );
#endif
	
#ifdef _XVIETNAMESE
	m_UpdateMoreStatic.MoveWindow( 633, 272, 52, 12 );
#else
	#ifdef _XENGLISH
		m_UpdateMoreStatic.MoveWindow( 653, 270, 52, 12 );
	#else
		#ifdef _XRUSSIAN
			m_UpdateMoreStatic.MoveWindow( 643, 270, 52, 12 );
		#else
			m_UpdateMoreStatic.MoveWindow( 653, 275, 32, 12 );
		#endif
	#endif
#endif

#ifdef _XPWC // PWC : hide more button
	m_NoticeMoreStatic.ShowWindow( SW_HIDE );
	m_UpdateMoreStatic.ShowWindow( SW_HIDE );
#endif

	_XLog( FALSE, _T("Link web page : Phase 2\n") );
	m_UpdateMoreStatic.SetLink(TRUE,FALSE)
					  .EnableUseLink(FALSE)
					  .SetTextColor(RGB( 255, 193, 0 ))
					  .SetBkColor( RGB(0,0,0) )
					  .SetFontUnderline(TRUE)
					  .SetTransparent(TRUE)
					  .SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HANDCURSOR));
	m_UpdateMoreStatic.SetWindowText( g_InterfaceString[63] );

#ifdef _XTAIWANESE
	m_UpdateMoreStatic.ShowWindow( SW_HIDE );
#endif
	
#ifdef _XHIDEWEBBROWSER

	#ifdef _XACCLAIM_NEWWEBPAGE
		m_NoticeListBrowser.ShowWindow( SW_SHOW );
		
		m_UpdateListBrowser.ShowWindow( SW_HIDE );	
		m_NoticeMoreStatic.ShowWindow( SW_HIDE );	
		m_UpdateMoreStatic.ShowWindow( SW_HIDE );	
	#else
		m_NoticeListBrowser.ShowWindow( SW_HIDE );	
		m_UpdateListBrowser.ShowWindow( SW_HIDE );	
		m_NoticeMoreStatic.ShowWindow( SW_HIDE );	
		m_UpdateMoreStatic.ShowWindow( SW_HIDE );	
	#endif

#endif

	m_CloseButton.SetBitmaps( IDB_CLOSEBITMAP, RGB(255,0,0) ,IDB_CLOSEBITMAP, RGB(255, 0, 0) );
	m_CloseButton.DrawBorder( FALSE );
	m_CloseButton.SetFlat( TRUE );
	m_CloseButton.MoveWindow( 682, 3, 14, 14 );
	m_CloseButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	
	_XLog( FALSE, _T("Resize  : log list control\n") );
	m_ServerMessageList.MoveWindow( 293, 51, 383, 200 );
	
	_XLog( FALSE, _T("Initialize  : log list control\n") );
	m_ServerMessageList.Init();
	_XLog( FALSE, _T("Change color :  log list control\n") );
	m_ServerMessageList.SetBkColor(RGB(0,0,0));
	m_ServerMessageList.SetTextColor(RGB(222,222,222));

	_XLog( FALSE, _T("Change style : log list control\n") );
	ListView_SetExtendedListViewStyle(m_ServerMessageList.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);

	_XLog( FALSE, _T("Hide window : log list control\n") );
	m_ServerMessageList.ShowWindow( SW_HIDE );

	_XLog( FALSE, _T("Initialize status text\n") );
	#ifdef _XRUSSIAN
	m_ctlUpdateText.MoveWindow(358, 120, 246, 58);
	#else
	m_ctlUpdateText.MoveWindow(392, 120, 186, 58);
	#endif
	m_ctlUpdateText.SetTransparent(TRUE);
	m_ctlUpdateText.SetTextColor(RGB(227, 227, 227));

	m_ProgressRateStatic.MoveWindow(454, 401, 30, 12);
	m_ProgressRateStatic.SetTransparent(TRUE);
	m_ProgressRateStatic.SetTextColor(RGB(227, 227, 227));
		
	_XLog( FALSE, _T("Initialize title control\n") );
	
	CString messageString;
	messageString = g_InterfaceString[13];
	m_ctlUpdateText.SetWindowText(messageString);

	m_UpdateTitleStatic.MoveWindow(293, 31, 394, 19);
	m_UpdateTitleStatic.SetTransparent(TRUE);
	m_UpdateTitleStatic.SetTextColor(RGB(227, 227, 227));
	m_UpdateTitleStatic.SetWindowText(g_InterfaceString[13]);
	m_UpdateTitleStatic.ShowWindow( SW_HIDE );

	#ifdef _XRUSSIAN
		m_NoticeStatic.MoveWindow(298, 270, 134, 14);
	#else
		m_NoticeStatic.MoveWindow(305, 270, 134, 14);
	#endif
	m_NoticeStatic.SetTransparent(TRUE);
	m_NoticeStatic.SetTextColor(RGB(227, 227, 227));
	m_NoticeStatic.SetWindowText(g_InterfaceString[42]);

	#ifdef _XRUSSIAN
		m_UpdateStatic.MoveWindow(506, 270, 134, 14);
	#else
		m_UpdateStatic.MoveWindow(513, 270, 134, 14);
	#endif
	m_UpdateStatic.SetTransparent(TRUE);
	m_UpdateStatic.SetTextColor(RGB(227, 227, 227));
	m_UpdateStatic.SetWindowText(g_InterfaceString[43]);	

#ifdef _XHIDEWEBBROWSER
	#ifdef _XACCLAIM_NEWWEBPAGE
		m_NoticeStatic.ShowWindow( SW_HIDE );
		m_UpdateStatic.ShowWindow( SW_HIDE );

		m_NoticeStatic.MoveWindow(473, 34, 50, 12);
	#else
		m_NoticeStatic.ShowWindow( SW_HIDE );
		m_UpdateStatic.ShowWindow( SW_HIDE );
	#endif
#endif

#ifdef _XPWC
	m_NoticeStatic.ShowWindow( SW_HIDE );
	m_UpdateStatic.ShowWindow( SW_HIDE );
#endif
	
	_XLog( FALSE, _T("Initialize gui buttons\n") );

	m_JoinButton.MoveWindow( 294, 372, 95, 20 );
	m_JoinButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_JoinButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_JoinButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_JoinButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_JoinButton.SetWindowText( g_InterfaceString[34] );
	
	m_RunButton.SetUseGrayMode( FALSE );
	m_RunButton.MoveWindow( 493, 372, 138, 44 );
	m_RunButton.SetSound(MAKEINTRESOURCE(IDR_STARTGAMEWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_RunButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_RunButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_RunButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_RunButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_RunButton.SetWindowText( g_InterfaceString[35] );

	m_SetupButton.MoveWindow( 392, 372, 95, 20 );
	m_SetupButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_SetupButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_SetupButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_SetupButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_SetupButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_SetupButton.SetWindowText( g_InterfaceString[36] );

	m_SelectServerButton.MoveWindow( 432, 372, 58, 20 );
	m_SelectServerButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_SelectServerButton.ShowWindow( SW_HIDE );
	m_SelectServerButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_SelectServerButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_SelectServerButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_SelectServerButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_SelectServerButton.SetWindowText( g_InterfaceString[37] );

	m_CancelButton.MoveWindow( 634, 372, 62, 44 );	
	m_CancelButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_CancelButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_CancelButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_CancelButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_CancelButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_CancelButton.SetWindowText( g_InterfaceString[38] );

	m_ServerListRefreshButton.MoveWindow( 620, 233, 58, 20 );	
	m_ServerListRefreshButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_ServerListRefreshButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_ServerListRefreshButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_ServerListRefreshButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_ServerListRefreshButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_ServerListRefreshButton.SetWindowText( g_InterfaceString[39] );
	m_ServerListRefreshButton.ShowWindow( SW_HIDE );

	m_ServerListCloseButton.MoveWindow( 690, 233, 58, 20 );	
	m_ServerListCloseButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_ServerListCloseButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_ServerListCloseButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_ServerListCloseButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_ServerListCloseButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_ServerListCloseButton.SetWindowText( g_InterfaceString[39] );
	m_ServerListCloseButton.ShowWindow( SW_HIDE );

	m_ShowPatchInfoButton.MoveWindow( 441, 161, 76, 20 );	
	m_ShowPatchInfoButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_ShowPatchInfoButton.ShowWindow(SW_SHOW);
	m_ShowPatchInfoButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_ShowPatchInfoButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_ShowPatchInfoButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_ShowPatchInfoButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_ShowPatchInfoButton.SetWindowText( g_InterfaceString[40] );

	m_HidePatchInfoButton.SetBitmaps( IDB_MINICLOSEBUTTONHIGHLIGHTBITMAP, RGB(255,0,0), IDB_MINICLOSEBUTTONNORMALBITMAP, RGB(255, 0, 0) );
	m_HidePatchInfoButton.DrawBorder( FALSE );
	m_HidePatchInfoButton.DrawTransparent();
	m_HidePatchInfoButton.SetFlat( TRUE );	
	m_HidePatchInfoButton.MoveWindow( 293+381, 34, 12, 12 );	
	m_HidePatchInfoButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_HidePatchInfoButton.ShowWindow(SW_HIDE);
	
	m_OptionHighButton.MoveWindow( 305, 235, 78, 20 );	
	m_OptionHighButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_OptionHighButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_OptionHighButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_OptionHighButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_OptionHighButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_OptionHighButton.SetWindowText( g_InterfaceString[31] );

	#ifdef _XRUSSIAN
	m_OptionMiddleButton.MoveWindow( 384, 235, 78, 20 );	
	#else
	m_OptionMiddleButton.MoveWindow( 387, 235, 78, 20 );	
	#endif
	m_OptionMiddleButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_OptionMiddleButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_OptionMiddleButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_OptionMiddleButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_OptionMiddleButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_OptionMiddleButton.SetWindowText( g_InterfaceString[30] );

	#ifdef _XRUSSIAN
	m_OptionLowButton.MoveWindow( 463, 235, 78, 20 );	
	#else
	m_OptionLowButton.MoveWindow( 468, 235, 78, 20 );	
	#endif
	m_OptionLowButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_OptionLowButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_OptionLowButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_OptionLowButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_OptionLowButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_OptionLowButton.SetWindowText( g_InterfaceString[29] );
	
	#ifdef _XRUSSIAN
		m_OptionOKButton.MoveWindow( 544, 235, 78, 20 );
	#else
		m_OptionOKButton.MoveWindow( 562, 235, 58, 20 );
	#endif

	m_OptionOKButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_OptionOKButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_OptionOKButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_OptionOKButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_OptionOKButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_OptionOKButton.SetWindowText( g_InterfaceString[41] );

	m_OptionCancelButton.MoveWindow( 623, 235, 58, 20 );
	m_OptionCancelButton.SetSound(MAKEINTRESOURCE(IDR_BUTTONWAVE), ::GetModuleHandle(NULL), TRUE, FALSE);
	m_OptionCancelButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_OptionCancelButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_OptionCancelButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_OptionCancelButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_OptionCancelButton.SetWindowText( g_InterfaceString[38] );

	m_ChangeResolution.MoveWindow( 403, 181, 77, 20 );
	m_ChangeResolution.SetShade(CShadeButtonST::SHS_HSHADE);
	m_ChangeResolution.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_ChangeResolution.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_ChangeResolution.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_ChangeResolution.SetWindowText( g_InterfaceString[66] );

#ifdef _XVIETNAMESE
	m_ChangeResolution.ShowWindow( SW_SHOW );
#else
	#ifdef _XPWC
		m_ChangeResolution.ShowWindow( SW_SHOW );
	#else
		#ifdef _XACCLAIM
			m_ChangeResolution.ShowWindow( SW_SHOW );
		#else
			#ifdef _XKOREAN
				m_ChangeResolution.ShowWindow( SW_SHOW );
			#else
				#ifdef _XJAPANESE
					m_ChangeResolution.ShowWindow( SW_SHOW );
				#else
					#ifdef _XRUSSIAN
						m_ChangeResolution.ShowWindow( SW_SHOW );
					#else
						m_ChangeResolution.ShowWindow( SW_HIDE );
					#endif
				#endif
			#endif
		#endif
	#endif
#endif
	
	ChangeTab( _XTABMODE_UPDATEMESSAGE );
	
	m_Initialized = TRUE;

	_XLog( FALSE, _T("Initialize complete\n") );

#ifdef _XNEXON_WEBLOGIN
	CreateDeskTopShortCut();
#endif

/*
#ifdef _XDEF_USETIMERSTART 
	SetTimer( _PATCH_STARTTIMERID, 300, NULL );	
#else
	StartPatch();
#endif	
*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#ifdef _XACCLAIM

BOOL CXLauncherDlg::LoadLanguageList( void )
{
	BOOL rc = FALSE;

	DWORD dwFileSize = 0;
	HANDLE hFile = NULL;

	CString szFilepath;
	szFilepath = g_Installedpath;
	szFilepath += _T("Data\\Script\\Language.XML");
	
	hFile = CreateFile( szFilepath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_XLog(FALSE,_T("ERROR - CreateFile failed"));
		hFile = 0;
	}
	else
	{
		dwFileSize = GetFileSize(hFile, NULL);

		if ((dwFileSize != INVALID_FILE_SIZE))
		{
			BYTE * pBuf = new BYTE [dwFileSize+16];
			ASSERT(pBuf);

			if (pBuf)
			{
				DWORD dwBytesRead = 0;
				BOOL bRet = ReadFile(hFile, (LPVOID) pBuf, dwFileSize, &dwBytesRead, NULL);

				if (bRet)
				{
					pBuf[dwFileSize] = 0;
					pBuf[dwFileSize+1] = 0;
					pBuf[dwFileSize+2] = 0;
					
					CString csText;
					// Windows Unicode file is detected if starts with FEFF
					if ( pBuf[0] == 0xff && pBuf[1] == 0xfe )
					{
						// Contains byte order mark, so assume wide char content
						// non _UNICODE builds should perform UCS-2 (wide char) to UTF-8 conversion here
						csText = (LPCWSTR)(&pBuf[2]);
						//_ISM( _T("File starts with hex FFFE, assumed to be wide char format. ") );
					}
					else
					{
						// _UNICODE builds should perform UTF-8 to UCS-2 (wide char) conversion here
						csText = (LPCSTR)pBuf;
					}

					delete [] pBuf;
					
					CMarkup xmldocument;

					rc = xmldocument.SetDoc( csText );
					if( rc )
					{
						xmldocument.ResetPos();						
						while ( xmldocument.FindElem( _T("LanguageList") ) )
						{
							xmldocument.IntoElem();

							while ( xmldocument.FindElem( _T("language") ) )
							{
								m_LanguageComboBox.AddString( xmldocument.GetData() );
							}
						}	
						
						int sublanguage = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SUBLANGUAGE, 0, TRUE );
						
						if( m_LanguageComboBox.GetCount() > sublanguage )
						{
							m_SubLanguageIndex = sublanguage;
							m_LanguageComboBox.UpdateData(FALSE);
						}
						else
						{
							m_SubLanguageIndex = 0;
							m_LanguageComboBox.UpdateData(FALSE);
						}
					}
				}
				else
				{
					_XLog(FALSE, _T("ERROR - ReadFile failed") );
					dwFileSize = 0;
				}					
			}
			else
			{
				_XLog(FALSE, _T("ERROR - failed to allocate memory") );
				dwFileSize = 0;
			}
		}
		else
		{
			_XLog(FALSE, _T("ERROR - GetFileSize failed") );
			dwFileSize = 0;
		}
	}
	
	if (hFile)
		CloseHandle(hFile);

	return rc;
}

#endif



void CXLauncherDlg::StartPatch( void )
{
	this->m_ServerLauncherVersion = 0;
		
	DWORD h1,h2,l1,l2;
	TCHAR ModuleFile[ 512 ];

	memset( ModuleFile, 0, sizeof(TCHAR)*512 );
	GetModuleFileName( NULL, ModuleFile, 511 );
	
	_XLog( FALSE, _T("Check exe version.\n") );

	if( _XGetExeFileVersion( ModuleFile, h1,h2,l1,l2 ) == S_OK ) this->m_LauncherVersion = l1*10000 + l2;
	else this->m_LauncherVersion = 0;

	CString tTitleString;
	tTitleString.Format( _T("XLauncher 0.9d [%d.%d]"), l1, l2 );
	SetWindowText( tTitleString ); 
	
	RECT rect;
	m_ServerMessageList.GetClientRect( &rect );
	m_ServerMessageList.InsertColumn( 0, _T("Patch logs"), LVCFMT_LEFT, (rect.right-rect.left)+((rect.right-rect.left)>>1) );
	m_ServerMessageList.SetExtendedStyle( LVS_EX_FULLROWSELECT );

	CheckInnerTester();
	
	m_Version      = 0x0000;
	m_VersionCheck = FALSE;
	m_PatchDownloadComplete = FALSE;
	m_PatchFilename = _T("");
	m_PatchInfoFilename = _T("");
	
	m_BannerIndex  = 0x0000;
	m_BannerCheck  = FALSE;
	m_BannerDownloadComplete = FALSE;
	m_BannerFilename = _T(""); 
		
	m_PatchMySelf = _T("");
	m_SelfPatchReady = FALSE;

	// Serial number check 	
	if( CheckSerialNumber() == -1 )
	{		

#ifdef  _LOGFILE_
	_XLog( TRUE, _T("MESSAGE : ERROR [ End user license ]. Exit.\n") );
#endif
		PostQuitMessage(1);
		return;
	}

	_XLog( FALSE, _T("Get server info.\n") );

	g_ServerAddressManager.GetServerInfo();

	TCHAR widestring[1024];
	memset( widestring, 0, sizeof(TCHAR) * 1024 );
	int requirelength = MultiByteToWideChar(CP_ACP, 0, g_ServerAddressManager.m_ServerInfo.cdnserverurl, 
													   strlen(g_ServerAddressManager.m_ServerInfo.cdnserverurl), widestring, 1024);
	m_PatchServerIPAddress = widestring;
			
	m_PatchServerPortNumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHSERVERPORTNUMBER, 21, TRUE );

	// Package version ( Beta, Release.. )
	TCHAR registrybuffer[_REGSTRING_MAXLENGTH];
	memset( registrybuffer, 0, sizeof(TCHAR)*_REGSTRING_MAXLENGTH );

#ifdef _XTESTSERVER		
	#ifdef _XTAIWANESE
	g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, _T("OpenBetaX1-TS"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
	#else
		#ifdef _XVIETNAMESE
			g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, _T("CSX1-TS"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
		#else
			#ifdef _XACCLAIM			
					g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, _T("OpenBetaX1-TS"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
			#else
				#ifdef _XPWC
					g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, _T("ClosedBetaX1-TS"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
				#else
					#ifdef _XJAPANESE
						g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_TESTSERVERPACKAGEVERSION, _T("OpenBetaX1-TS"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
					#else
						#ifdef _XRUSSIAN
							g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_TESTSERVERPACKAGEVERSION, _T("ClosedBetaX1-TS"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
						#else
							g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_TESTSERVERPACKAGEVERSION, _T("OpenBetaX1-TS"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
						#endif
					#endif
				#endif
			#endif
		#endif
	#endif
#else
	#ifdef _XTAIWANESE
	g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, _T("CSX5"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
	#else
		#ifdef _XVIETNAMESE
			g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, _T("CSX1"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);			
		#else
			#ifdef _XACCLAIM
				g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, _T("OpenBetaX1"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
			#else
				#ifdef _XPWC
					g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, _T("ClosedBetaX1"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
				#else
					#ifdef _XJAPANESE
						g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, _T("OpenBetaX1"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
					#else
						#ifdef _XRUSSIAN
							g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, _T("ClosedBetaX1"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
						#else
							g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, _T("OpenBetaX1"), registrybuffer, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
						#endif
					#endif
				#endif
			#endif
		#endif
	#endif
#endif


	m_PackageVersionName = registrybuffer;
	
	this->m_Version = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHVERSION, 0, TRUE);
	
	g_PatchVersionManager.GetVersionInfo();		
	this->m_PatchNumber = g_PatchVersionManager.m_VersionInfo.patchversion_1;
	
	// Read banner index
	this->m_BannerIndex = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_BANNERVERSION, 0, TRUE);
	
	_XLog( FALSE, _T("Start version check.\n") );

	int versioncheckresult = VersionCheck();

	_XLog( FALSE, _T("End version check.\n") );
}

void CXLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CXLauncherDlg::CheckInnerTester( void )
{
	TCHAR	LicenseCode[_REGSTRING_MAXLENGTH];
	memset( LicenseCode, 0, sizeof(TCHAR)*_REGSTRING_MAXLENGTH );
	g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_LICENSECODE, _T(""), LicenseCode, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, FALSE);
	
	if( wcslen( LicenseCode ) <= 0 )
	{
		return;
	}

	char mbcsstring[1024];		
	memset( mbcsstring, 0, 1024 );
	WideCharToMultiByte(CP_ACP, 0, LicenseCode, -1, mbcsstring, 1024, NULL, NULL);

	_XEncryption encription;
	encription.setKey( 10, 31, "9E2GH4FWDWKC2MBL4EBNKUV63T" );

	if( encription.checksumSerial( mbcsstring ) == TRUE )
	{
		CSelectServer	selectserver;
		if( selectserver.DoModal() == IDOK )
		{
			if( selectserver.m_SelectedServer == 1 )
			{
				m_TestServerConnectMode = TRUE;
			}					
		}
	}
	else
	{
		//AfxMessageBox( "Failed authentication." );
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXLauncherDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		/*
		CDC* pDC = GetDC();
		if( pDC )
		{	
			m_BorderImage.PasteToDC( pDC, 0, 0 );
			ReleaseDC( pDC );
		}
		*/

		CDialog::OnPaint();			
	}
}

void CXLauncherDlg::RedrawAllControls( void )
{
	CRect rect;

	// bottom progress
	#ifdef _XACCLAIM_NEWWEBPAGE	
		if( m_CurrentTabMode == _XTABMODE_OPTIONVIEW ) 
		{
			rect.left = 283;
			rect.top = 30;	
			rect.right = 696;
			rect.bottom = 260;
			RedrawWindow( &rect );
		}
		else if( m_CurrentTabMode == _XTABMODE_SELECTSERVER ) 
		{
			rect.left = 283;
			rect.top = 30;	
			rect.right = 696;
			rect.bottom = 51;
			RedrawWindow( &rect );			
		}
		else if( m_CurrentTabMode == _XTABMODE_UPDATEMESSAGE ) 
		{
			rect.left = 283;
			rect.top = 30;	
			rect.right = 696;
			rect.bottom = 51;
			RedrawWindow( &rect );			
		}
	#else
		rect.left = 283;
		rect.top = 30;	
		rect.right = 696;
		rect.bottom = 260;
		RedrawWindow( &rect );
	#endif

	// tab view
	rect.left = 81;
	rect.top = 395;	
	rect.right = 489;
	rect.bottom = 415;
	RedrawWindow( &rect );

#ifdef _XPWC
	
#else

	#ifdef _XACCLAIM_NEWWEBPAGE	
		rect.left = 283;
		rect.top = 30;	
		rect.right = rect.left + 414;
		rect.bottom = rect.top + 21;
		RedrawWindow( &rect, NULL, RDW_ERASENOW );		
		//m_NoticeListBrowser.RedrawWindow( NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW );		
	#else
	
		// notice
		rect.left = 283;
		rect.top = 268;	
		rect.right = 488;
		rect.bottom = 288;
		RedrawWindow( &rect );

		// update
		rect.left = 491;
		rect.top = 268;	
		rect.right = 696;
		rect.bottom = 288;
		RedrawWindow( &rect );
	#endif
	
#endif

}

BOOL CXLauncherDlg::OnEraseBkgnd(CDC* pDC) 
{
	m_BackBufferImage.CopyFrom( &m_BorderImage );

	m_BackBufferImage.PasteRect( &m_ProgressBarBackBorderImage, 81, 395 );
	
	if( !m_DownloadComplete )
		m_BackBufferImage.PasteRect( &m_ProgressbarBackBorderTitleImage, 81, 395 );
		
#ifdef _XHIDEWEBBROWSER
	#ifdef _XACCLAIM_NEWWEBPAGE
		//m_BackBufferImage.BlendRect( &m_OptionBorderImage, 283, 30 );		
	
		if( m_CurrentTabMode == _XTABMODE_SELECTSERVER || m_CurrentTabMode == _XTABMODE_UPDATEMESSAGE ) 
		{
			m_BackBufferImage.BlendRect_ClipHeight( &m_OptionBorderImage, 283, 30, 21 );
		}
	#endif
#else
	#ifdef _XPWC
	
	#else
		m_BackBufferImage.BlendRect( &m_NoticeSmallBorderImage, 283, 268 );
		m_BackBufferImage.BlendRect( &m_UpdateSmallBorderImage, 492, 268 );
	#endif
#endif
	
	switch( m_CurrentTabMode ) 
	{
	case _XTABMODE_UPDATEMESSAGE	:
		#ifdef _XACCLAIM_NEWWEBPAGE
		
		#else
		if( m_ViewUpdateInformationMode )
		{
			m_BackBufferImage.BlendRect( &m_UpdateTitleImage, 283, 30 );
		}
		else
		{
			m_BackBufferImage.BlendRect( &m_PopupBorderImage, 385, 127, 160 );
		}
		#endif
		break;
	case _XTABMODE_OPTIONVIEW		:
		m_BackBufferImage.BlendRect( &m_OptionBorderImage, 283, 30 );
		break;	
	case _XTABMODE_SELECTSERVER		:
		if( m_ViewServerList )
		{
			m_BackBufferImage.BlendRect( &m_ServerSelectBorderImage, 283, 30 );
			
			int xcoordfactor = 0;
			int ycoordfactor = 0;
			for( int serverindex = 0; serverindex < m_MaxServerCount; serverindex++ )
			{
				if( m_NextServerTrafficFactor[serverindex] < 0 )
				{
					m_BackBufferImage.BlendRectTransparently( &m_ServerNameImage[serverindex], 283+20+xcoordfactor, 30+38+ycoordfactor, 160 );
				}
				else if( m_SelectedServerIndex == serverindex  )
				{
					m_BackBufferImage.BlendRectTransparently( &m_ServerNameImage[serverindex], 283+20+xcoordfactor, 30+38+ycoordfactor, 255 );
				}
				else
				{
					m_BackBufferImage.BlendRectTransparently( &m_ServerNameImage[serverindex], 283+20+xcoordfactor, 30+38+ycoordfactor, 180 );
				}
				ycoordfactor += 18;

				if( serverindex == 9 )
				{
					xcoordfactor = 193;
					ycoordfactor = 0;
				}
			}

			RECT rect;		
			rect.left   = 496; 
			rect.top    = 212;
			rect.right  = 684;
			rect.bottom = 258;
			m_BackBufferImage.FillGlassRect( rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, 16,16,16, 128 );

			m_BackBufferImage.BlendRectTransparently( &m_SelectedServerNameImage, 498, 212, 255 );		

			m_BackBufferImage.ClippedLine( rect.left+1, rect.top, rect.right, rect.top, 0,0,0);
			m_BackBufferImage.ClippedLine( rect.left+1, rect.bottom, rect.right, rect.bottom, 0,0,0);
			m_BackBufferImage.ClippedLine( rect.left, rect.top+1, rect.left, rect.bottom, 0,0,0);
			m_BackBufferImage.ClippedLine( rect.right, rect.top+1, rect.right, rect.bottom, 0,0,0);
		}

		break;
	}

	if( m_UpdateComplete )
		m_BackBufferImage.BlendRectTransparently( &m_UpdatedMessageImage, 86, 396, 255 );
	
	m_BackBufferImage.PasteToDC( pDC, 0, 0 );
	
	//m_NoticeListBrowser.UpdateWindow();
	//m_NoticeListBrowser.RedrawWindow( NULL, NULL, RDW_UPDATENOW );

	if( g_FirstRun )
	{
		g_FirstRun = FALSE;

		_XLog( FALSE, _T("Begin patch works.\n") );

#ifdef _XDEF_USETIMERSTART 
		SetTimer( _PATCH_STARTTIMERID, 300, NULL );	
#else
		StartPatch();
#endif
	}
	//CDialog::OnEraseBkgnd(pDC);
	return TRUE;//CDialog::OnEraseBkgnd(pDC);
}

void CXLauncherDlg::OnDestroy() 
{
	CDialog::OnDestroy();		
	
#ifdef _XDEF_USETIMERSTART 
	KillTimer( _PATCH_STARTTIMERID );
#endif

	_XLog( TRUE, _T("WSA Cleanup...\n") );
	WSACleanup();
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXLauncherDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CXLauncherDlg::InsertStringToServerMessageListCtrl( LPCTSTR message, BOOL insert )
{
	if( !message ) return;	
	CString szMessage = message;

	szMessage.Replace( _T("\n"), _T("\0") );
	szMessage.Replace( _T("\r"), _T("\0") );

	int listitemcount = -1;
	if( insert )
	{		
		listitemcount =  m_ServerMessageList.GetItemCount();		
		m_ServerMessageList.InsertItem( listitemcount, szMessage );
		m_ServerMessageList.EnsureVisible( listitemcount, FALSE );
	}
	else
	{
		listitemcount = m_ServerMessageList.GetItemCount()-1;
		m_ServerMessageList.SetItemText( listitemcount, 0, szMessage );
		m_ServerMessageList.EnsureVisible( listitemcount, FALSE );
	}

	m_ServerMessageList.UpdateWindow();

	Sleep(0);
}

void CXLauncherDlg::InsertStringToProgressMessageListCtrl( LPCTSTR message, int insert )
{
	InsertStringToServerMessageListCtrl( message, insert );
}

BOOL CXLauncherDlg::SplitSerialnumber( LPCTSTR serialstring )
{
	
	return TRUE;	
}

void CXLauncherDlg::ResetRegistry( void )
{		
	g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_LICENSECODE );
	g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION );
	g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SERVERIPADDRESS );
	g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SERVERPORTNUMBER );	
	g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHFTPSERVERIPADDRESS );
	g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHSERVERPORTNUMBER );
	g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHNUMBER );
	g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHVERSION );
	g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_BANNERVERSION );
	g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_INSTALLEDPATH );
	g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_EXECUTEFILE );

	//g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHSERVERIPADDRESS );
	//g_RegManager._XRegDeleteNode(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHFTPSERVERPORTNUMBER );

	CString LicenseFilename = g_Installedpath;
	LicenseFilename += _T("\\Data\\License.inf");

	if( CheckExistFile( LicenseFilename, TRUE ) )
	{
		::DeleteFile( LicenseFilename );
	}

#ifdef  _LOGFILE_
	_XLog( TRUE, _T("MESSAGE : Reset registry\n") );
#endif
}

int CXLauncherDlg::CheckSerialNumber()
{
	// Serial number check
	//InsertStringToProgressMessageListCtrl( "Begin authentication" );
	
	memset( g_Installedpath, 0, sizeof(TCHAR)*(_REGSTRING_MAXLENGTH+1) );
	
	g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_INSTALLEDPATH, _T(""), g_Installedpath, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, FALSE);

	if( wcslen( g_Installedpath ) <= 0 )
	{
		memset( g_Installedpath, 0, sizeof(TCHAR)*(_REGSTRING_MAXLENGTH+1) );
		GetCurrentDirectory( _REGSTRING_MAXLENGTH, g_Installedpath );
		g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_INSTALLEDPATH, g_Installedpath );
	}

	if( _wchdir( g_Installedpath ) == -1 )
	{
		memset( g_Installedpath, 0, sizeof(TCHAR)*(_REGSTRING_MAXLENGTH+1) );
		GetCurrentDirectory( _REGSTRING_MAXLENGTH, g_Installedpath );
		g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_INSTALLEDPATH, g_Installedpath );
	}

	_XResetFileAttribute( g_Installedpath );

	return 1;

/*
	_XEncryption encription;
	encription.setKey( 10, 31, "9E2GH4FWDWKC2MBL4EBNKUV63T" );
			
	char serialstring[_REGSTRING_MAXLENGTH+1];
	memset( serialstring, 0, _REGSTRING_MAXLENGTH+1 );

	char xorencyperserial[_REGSTRING_MAXLENGTH+1];
	memset( xorencyperserial, 0, _REGSTRING_MAXLENGTH+1 );

	char licensefilecode[_REGSTRING_MAXLENGTH+1];    
	memset( licensefilecode, 0, _REGSTRING_MAXLENGTH+1 );

	char mixeddiskserialcode[_REGSTRING_MAXLENGTH+1];
	memset( mixeddiskserialcode, 0, _REGSTRING_MAXLENGTH+1 );

	char finalcode[_REGSTRING_MAXLENGTH+1];
	memset( finalcode, 0, _REGSTRING_MAXLENGTH+1 );

	CString LicenseFilename = g_Installedpath;
	LicenseFilename += _T("\\Data\\License.inf");

	CString DirectoryPath = g_Installedpath;
	DirectoryPath += _T("\\Data\\");
	MakeSureDirectoryPathExists( DirectoryPath );

	CEnterSerialDlg enterserialdlg;

	BOOL regload = g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_LICENSECODE, _T("000000000000000000000"), serialstring, _REGSTRING_MAXLENGTH);

	if( regload )
	{
		if( !CheckExistFile( LicenseFilename, TRUE ) ) regload = FALSE;
		else
		{
			FILE* licensefile = NULL;

			licensefile = _wfopen( LicenseFilename, _T("rb") );

			if( !licensefile )
			{
				regload = FALSE;
			}
			else
			{
				if( fread( licensefilecode, (_ENCRYPTION_FIELD_LENGTH>>1)+10, 1, licensefile ) < 1 )
				{
					regload = FALSE;
				}				

				fclose( licensefile );
			}
		}
	}
	

	if( !regload )
	{		
		InsertStringToProgressMessageListCtrl( _T("제품번호 인증 작업을 시작합니다.") );

		while( 1 )
		{
			this->ShowWindow( SW_RESTORE );

			if( enterserialdlg.DoModal() == IDOK )
			{
				CString sumSerialField;
				sumSerialField = enterserialdlg.m_SerialField1 + enterserialdlg.m_SerialField2 + 
								 enterserialdlg.m_SerialField3 + enterserialdlg.m_SerialField4 + 
								 enterserialdlg.m_SerialField5;

				if( encription.checksumSerial( sumSerialField.GetBuffer(256) ) == TRUE )
				{
					InsertStringToProgressMessageListCtrl( _T("제품번호 인증 완료") );
										
					encription.xorEncyper( sumSerialField.GetBuffer(256), xorencyperserial );
					
					memset( serialstring, 0, _REGSTRING_MAXLENGTH+1 );

					encription.SplitSerial( xorencyperserial, serialstring, licensefilecode );

					encription.mixDiskSerial( serialstring, mixeddiskserialcode );

					g_RegManager._XRegWriteString(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_LICENSECODE, mixeddiskserialcode );

					
					encription.mixDiskSerial( licensefilecode, mixeddiskserialcode );

					if( CheckExistFile( LicenseFilename, TRUE ) ) ::DeleteFile( LicenseFilename );
					
					FILE* licensefile = NULL;

					licensefile = _wfopen( LicenseFilename, _T("wb") );

					if( !licensefile )
					{
						AfxMessageBox( _T("ERROR : License 등록에 실패했습니다.[0x0120]") );
						InsertStringToProgressMessageListCtrl( _T("ERROR : License 등록에 실패했습니다.[0x0120]") );
						return -1;
					}
					else
					{
						if( fwrite( mixeddiskserialcode, (_ENCRYPTION_FIELD_LENGTH>>1)+10, 1, licensefile ) < 1 )
						{
							AfxMessageBox( _T("ERROR : License 등록에 실패했습니다.[0x0121]") );
							InsertStringToProgressMessageListCtrl( _T("ERROR : License 등록에 실패했습니다.[0x0121]") );
							fclose( licensefile );
							return -1;
						}				

						fclose( licensefile );
					}
					
					AfxMessageBox( _T("구룡쟁패를 이용해 주셔서 감사합니다.\r\n제품번호 인증작업이 종료되었습니다.") );

					break;
				}			
				else
				{
					CString tMessageString;
					tMessageString.Format( _T("[ %s - %s - %s - %s - %s ] 는 인증되지 않은 제품번호입니다"),
								 enterserialdlg.m_SerialField1, enterserialdlg.m_SerialField2, 
								 enterserialdlg.m_SerialField3, enterserialdlg.m_SerialField4, 
								 enterserialdlg.m_SerialField5 );

					InsertStringToProgressMessageListCtrl( tMessageString );
				}
			}
			else
			{				
				InsertStringToProgressMessageListCtrl( _T("Cancel end user license.") );

				return -1;
			}		
		}
	}
	else
	{
		BOOL serialpass = TRUE;

		encription.JointSerial(  serialstring, licensefilecode, finalcode );

		DWORD filediskserialnumber = encription.GetDiskserialFromSerialString( licensefilecode );

		DWORD regdiskserialnumber = encription.GetDiskserialFromSerialString( serialstring );

		if( filediskserialnumber != regdiskserialnumber ) serialpass = FALSE;

		if( filediskserialnumber != encription.getDiskSerial( 2 ) ) serialpass = FALSE;


		encription.xorEncyper( finalcode, xorencyperserial );
		
		if( !encription.checksumSerial( xorencyperserial ) )  serialpass = FALSE;

		if( serialpass )
		{			
			//InsertStringToProgressMessageListCtrl(  "제품번호 인증 완료" ); 		
		}			
		else
		{
			while( 1 )
			{				
				//wwiistarterdlg->InsertStringToProgressMessageListCtrl( "인증되지 않은 제품번호입니다" );
				this->ShowWindow( SW_RESTORE );

				if( enterserialdlg.DoModal() == IDOK )
				{
					CString sumSerialField;
					sumSerialField = enterserialdlg.m_SerialField1 + enterserialdlg.m_SerialField2 + 
									 enterserialdlg.m_SerialField3 + enterserialdlg.m_SerialField4 + 
									 enterserialdlg.m_SerialField5;

					if( encription.checksumSerial( sumSerialField.GetBuffer(256) ) == TRUE )
					{
						InsertStringToProgressMessageListCtrl( _T("제품번호 인증 완료") );
										
						encription.xorEncyper( sumSerialField.GetBuffer(256), xorencyperserial );
						
						memset( serialstring, 0, _REGSTRING_MAXLENGTH+1 );

						encription.SplitSerial( xorencyperserial, serialstring, licensefilecode );

						encription.mixDiskSerial( serialstring, mixeddiskserialcode );

						g_RegManager._XRegWriteString(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_LICENSECODE, mixeddiskserialcode );

						
						encription.mixDiskSerial( licensefilecode, mixeddiskserialcode );

						if( CheckExistFile( LicenseFilename, TRUE ) ) ::DeleteFile( LicenseFilename );
						
						FILE* licensefile = NULL;

						licensefile = _wfopen( LicenseFilename, _T("wb") );

						if( !licensefile )
						{
							AfxMessageBox( _T("ERROR : License 등록에 실패했습니다.[0x0122]") );
							InsertStringToProgressMessageListCtrl( _T("ERROR : License 등록에 실패했습니다.[0x0122]") );
							return -1;
						}
						else
						{
							if( fwrite( mixeddiskserialcode, (_ENCRYPTION_FIELD_LENGTH>>1)+10, 1, licensefile ) < 1 )
							{
								AfxMessageBox( _T("ERROR : License 등록에 실패했습니다.[0x0123]") );
								InsertStringToProgressMessageListCtrl( _T("ERROR : License 등록에 실패했습니다.[0x0123]") );
								fclose( licensefile );
								return -1;
							}				

							fclose( licensefile );
						}
						
						AfxMessageBox( _T("구룡쟁패를 이용하여 주셔서 감사합니다.\r\n제품번호 인증작업이 종료되었습니다.") );

						break;
					}			
					else
					{
						CString tMessageString;
						tMessageString.Format( _T("[ %s - %s - %s - %s - %s ] 는 인증되지 않은 제품번호입니다"),
									 enterserialdlg.m_SerialField1, enterserialdlg.m_SerialField2, 
									 enterserialdlg.m_SerialField3, enterserialdlg.m_SerialField4, 
									 enterserialdlg.m_SerialField5 );

						InsertStringToProgressMessageListCtrl( tMessageString );						
					}
				}
				else
				{
					InsertStringToProgressMessageListCtrl( _T("Cancel end user license.") );
					return -1;
				}		
			}
		}

	}
	*/


	return 1;
}

int  CXLauncherDlg::PatchProcess( int patchindex )
{
	 m_UpdateProgress.SetPos(0);

	 CString tMessageString;
	 CString messageString;
	 messageString = g_InterfaceString[14];
	 InsertStringToProgressMessageListCtrl( messageString.operator LPCTSTR() );

	 // Decompress patch package...	 	 
	 _wchdir( g_Installedpath );
		 
	 CZipArchive zip;

	 zip.Open( this->m_PatchFilename, CZipArchive::openReadOnly );

	 m_UpdateProgress.SetPos(10);

	 for (int i = 0; i < zip.GetNoEntries(); i++)
	 {
	      zip.ExtractFile(i, g_Installedpath);
	 }

	 m_UpdateProgress.SetPos(30);

	 zip.Close();

	 messageString = g_InterfaceString[15];
	 InsertStringToProgressMessageListCtrl( messageString.operator LPCTSTR() );
	 
	 messageString = g_InterfaceString[16];
	 tMessageString.Format( messageString.operator LPCTSTR(), patchindex );
	 InsertStringToProgressMessageListCtrl( tMessageString );
	 	 
	 CPackage package;
	 CString patchfilename;
	 patchfilename.Format( _T("PATCH%d.XP"), patchindex );

	 CString patchinfofilename;
	 patchinfofilename.Format( _T("PATCH%d.XPI"), patchindex );

	 if( package.Patch( patchfilename, patchinfofilename, this ) )
	 {
	
	 //////////////////////////////////////////////////////////////////////////
	 // Modify patch number
	 //////////////////////////////////////////////////////////////////////////
#ifndef _DOWNLOADTEST
		 g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHVERSION, this->m_ServerVersion );		 
		 
#ifdef _XTESTSERVER
		 g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_TESTSERVERPACKAGEVERSION, m_ServerPackageVersionName );
#else		 
		 g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, m_ServerPackageVersionName );
#endif
		 
		 g_PatchVersionManager.SetPatchNumber( patchindex );
#endif 
		 messageString = g_InterfaceString[17];
		 tMessageString.Format( messageString.operator LPCTSTR() );
		 InsertStringToProgressMessageListCtrl( tMessageString );		 
	 }
	 else 
	 {
		 messageString = g_InterfaceString[18];
		 tMessageString.Format( messageString.operator LPCTSTR() );
		 InsertStringToProgressMessageListCtrl( tMessageString );

		 return 0;
	 }

	 m_UpdateProgress.SetPos(95);

	 _wchdir( g_Installedpath );
	 if( CheckExistFile( m_PatchFilename, TRUE ) ) ::DeleteFile( m_PatchFilename );
	 if( CheckExistFile( patchfilename, TRUE ) ) ::DeleteFile( patchfilename );
	 if( CheckExistFile( patchinfofilename, TRUE ) ) ::DeleteFile( patchinfofilename );
	 
	 m_UpdateProgress.SetPos(100);

	 return 1;
}

#define _XDEF_LAUNCHER_SIGNAL   	"-9E2GH4FWDWKC2MBL4EBNKUV63T"

void CXLauncherDlg::Run()
{	
	/*
	if( m_SelectedServerIndex < 0 || m_NextServerTrafficFactor[m_SelectedServerIndex] < 0 ) 
	{
		AfxMessageBox( "정상 운영중인 서버를 선택하여 주시기 바랍니다." );
		return;
	}
	*/

	this->ShowWindow( SW_MINIMIZE );
	
	TCHAR ExecuteFile[_REGSTRING_MAXLENGTH];
	memset( ExecuteFile, 0, sizeof(TCHAR) * _REGSTRING_MAXLENGTH );
	
#ifdef _XTESTSERVER
	wcscpy( ExecuteFile, _T("tsNineDragons.exe") );
#else
	g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_EXECUTEFILE, _T("NineDragons.exe"), ExecuteFile, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, TRUE);
#endif
	
	char tCommandLineString[256] = _XDEF_LAUNCHER_SIGNAL;
	CString ExecutePathandFile = g_Installedpath;
	ExecutePathandFile += _T("\\");
	ExecutePathandFile += ExecuteFile;

	char serialfield[5][7];
	memset( serialfield, 0, sizeof(char) * 5 * 7 );

	CEnterSerialDlg::makeSerial( serialfield[0], serialfield[1], serialfield[2], serialfield[3], serialfield[4] );
	strcpy( tCommandLineString, "-" );
	strcat( tCommandLineString, serialfield[0] );
	strcat( tCommandLineString, serialfield[1] );
	strcat( tCommandLineString, serialfield[2] );
	strcat( tCommandLineString, serialfield[3] );
	strcat( tCommandLineString, serialfield[4] );

	TCHAR widestring[2048];
	memset( widestring, 0, sizeof(TCHAR) * 2048 );
	int requirelength = MultiByteToWideChar(CP_ACP, 0, tCommandLineString, strlen(tCommandLineString), widestring, 2048 );

#ifdef _XNEXON_WEBLOGIN
	if( g_ExecuteParam.GetLength() > 0 )
	{
		wcscat( widestring, _T(" ") );
		wcscat( widestring, g_ExecuteParam );
	}
#endif

#ifdef _XACCLAIM
	if( g_ExecuteParam.GetLength() > 0 )
	{
		wcscat( widestring, _T(" ") );
		wcscat( widestring, g_ExecuteParam );
	}
#endif

	if( ExecuteFile[0] ) ::ShellExecute(NULL, NULL, ExecutePathandFile, widestring,NULL, SW_SHOWNORMAL);	

#ifdef  _LOGFILE_
	CString messageString;
	messageString = g_InterfaceString[19];
	_XLog( TRUE, messageString.GetBuffer(256) );
#endif

	PostQuitMessage( 1 );	
}

int	CXLauncherDlg::ConnectVersionServer( BOOL viewmessage )
{
	_XLog( FALSE, _T("Start : cvs.\n") );

	CCheckVersionDlg*	pCheckVersionDialog;

	if( viewmessage )
	{
		pCheckVersionDialog = new CCheckVersionDlg;
		pCheckVersionDialog->Create( IDD_PATCHNUMBERSERVERCONNECTDIALOG, this );
		pCheckVersionDialog->ShowWindow( SW_SHOWNORMAL );
		pCheckVersionDialog->RedrawWindow();
		pCheckVersionDialog->CenterWindow();
	}
	
	sockaddr_in	addr		= {0};
	int			addrlen		= sizeof(addr);
	SOCKET		clientsock	= INVALID_SOCKET;

	_XLog( TRUE, _T("Create socket object...\n") );
	clientsock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientsock == INVALID_SOCKET)
	{
		_XLog( TRUE, _T("ERROR : socket intialize failed. Error No.%d\n"), WSAGetLastError());
		closesocket(clientsock) ;		
		if( viewmessage )
		{
			pCheckVersionDialog->ShowWindow( SW_HIDE );
			pCheckVersionDialog->DestroyWindow();
			delete pCheckVersionDialog;
		}
		return -1;
	}
	_XLog( TRUE, _T("Created socket object...\n") );
	
		
	char registrybuffer[_REGSTRING_MAXLENGTH];
	memset( registrybuffer, 0, _REGSTRING_MAXLENGTH );

	g_ServerAddressManager.GetServerInfo();

	strcpy( registrybuffer, g_ServerAddressManager.m_ServerInfo.patchserveraddress );
	
	char loginipadress[256];
	memset( loginipadress, 0, sizeof(char) * 256 );
	
	if( !_XIsIPv4Address( registrybuffer, strlen(registrybuffer) ) )
	{		
		if( !_XGetHostName( registrybuffer, loginipadress, 256 ) )
		{
			_XLog( TRUE, _T("Failed to get host name") );
			return FALSE;
		}
	}
	else
	{
		strcpy( loginipadress, registrybuffer);
	}
	
	addr.sin_family = AF_INET;	
	addr.sin_addr.s_addr = inet_addr(loginipadress);	
	addr.sin_port = htons(g_ServerAddressManager.m_ServerInfo.patchserverport);
	
	_XLog( TRUE, _T("Connect to version server...\n") );
	InsertStringToProgressMessageListCtrl( _T("Connect to version server...\n") );
	BOOL nReturn = connect(clientsock, (sockaddr*)&addr, addrlen);
	if (nReturn == SOCKET_ERROR)
	{
		int nError = WSAGetLastError();	
		if( nError != WSAEWOULDBLOCK )
		{
			_XLog( TRUE, _T("ERROR : connect failed. Error No.%d\n"), WSAGetLastError());
			closesocket(clientsock) ;		
			if( viewmessage )
			{
				pCheckVersionDialog->ShowWindow( SW_HIDE );
				pCheckVersionDialog->DestroyWindow();
				delete pCheckVersionDialog;
			}
			return -1;
		}
	}
	_XLog( TRUE, _T("Connected to version server...\n") );
	InsertStringToProgressMessageListCtrl( _T("Connected version server...\n") );

	_XLog( TRUE, _T("Recieve version...\n") );
	InsertStringToProgressMessageListCtrl( _T("Recieve version...\n") );

	char    pRecvBuf[1024] = {0};
	memset( pRecvBuf, 0, sizeof(char) * 1024 );

	char* pCurrentRecieveBuffer = pRecvBuf;	
	DWORD connecttime = timeGetTime();

	DWORD totalrecievesize = 0;

	while ( 1 ) 
	{	
		nReturn = recv( clientsock, pCurrentRecieveBuffer, sizeof(char) * 1024, NULL );
		if ( nReturn < 1 ) 
		{
			_XLog( TRUE, _T("ERROR : Recieve failed patch number : %d \n"), GetLastError() ) ;		
			if( viewmessage )
			{
				pCheckVersionDialog->ShowWindow( SW_HIDE );
				pCheckVersionDialog->DestroyWindow();
				delete pCheckVersionDialog;
			}
			
			_XLog( TRUE, _T("Close socket...\n") );
			closesocket(clientsock);
			return -1;
		}
		
		totalrecievesize += nReturn;
		pCurrentRecieveBuffer += nReturn;
		
		if( totalrecievesize >= sizeof(MSG_LOGIN_FOREST_LIST) )
		{
			break;
		}
		
		DWORD curtime = timeGetTime();

		if( ( curtime - connecttime) > 15000 )
		{
			_XLog( TRUE, _T("ERROR : Connection time out") );
									
			_XLog( TRUE, _T("Close socket...\n") );
			closesocket(clientsock);
			return -1;
		}
	}

	_XLog( TRUE, _T("Recieved version...\n") );
	InsertStringToProgressMessageListCtrl( _T("Recieved version...\n") );

	MSG_LOGIN_FOREST_LIST* pLoginForestList = (MSG_LOGIN_FOREST_LIST*)pRecvBuf;
	
	if( pLoginForestList->ForestInfo.bServerRun == 0 )
	{
		AfxMessageBox( g_InterfaceString[3] );
		_XLog( TRUE, g_InterfaceString[3] );
		
		InsertStringToProgressMessageListCtrl( g_InterfaceString[3] );				
		return -2;
	}
	
	int serverversion = pLoginForestList->ForestInfo.nVersion;

	/*
	m_MaxServerCount = _COUNT_MAX_GROUP_; // 서버에서 받아옴.
	
	for( int i = 0; i < m_MaxServerCount; i++ )
	{
		m_NextServerTrafficFactor[i] = pLoginForestList->ForestInfo.cForestState[i];
		
#ifdef _XDEF_SERVERTRAFFICGAUGETEST
		if( i < 9 )
			m_NextServerTrafficFactor[i] = (i%2) ? -1 : -2;
		else
		{
			m_NextServerTrafficFactor[i] = rand() % 121;
			
			if( m_NextServerTrafficFactor[i] > 100 ) 
				m_NextServerTrafficFactor[i] = 100;
		}
#endif
	}
	*/

	_XLog( TRUE, _T("Close socket...\n") );
	closesocket(clientsock);
	
	if( viewmessage )
	{
		pCheckVersionDialog->ShowWindow( SW_HIDE );
		pCheckVersionDialog->DestroyWindow();
		delete pCheckVersionDialog;
	}
	
//#define _XDEF_CHECKSERVERRUN

#ifdef _XDEF_CHECKSERVERRUN // Nexon 측 요구사항 처리.

	BOOL serverservice = FALSE;
	for( int i = 0; i < _COUNT_MAX_GROUP_; i++ )
	{
		 if( pLoginForestList->ForestInfo.cForestState[i] > -1 )
		 {
			serverservice = TRUE;
			break;
		 }
	}
	
	if( !serverservice )
	{
		AfxMessageBox( _T("지금은 서비스 시간이 아닙니다. 구룡쟁패 홈페이지의 공지사항을 참고하시기 바랍니다.") );
		_XLog( TRUE, _T("지금은 서비스 시간이 아닙니다. 구룡쟁패 홈페이지의 공지사항을 참고하시기 바랍니다." ) );	
		
		InsertStringToProgressMessageListCtrl( _T("지금은 서비스 시간이 아닙니다.") );
		InsertStringToProgressMessageListCtrl( _T("구룡쟁패 홈페이지의 공지사항을 참고하시기 바랍니다." ) );

		return -2;
	}

#endif

	CString message;
	message.Format( _T("Server version : %d\n"), serverversion );
	_XLog( TRUE, message.GetBuffer(256) );	
	InsertStringToProgressMessageListCtrl( message );
	
	/*
	m_SelectedServerIndex = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "Last selected server", 0, TRUE );	

	if( m_SelectedServerIndex > m_MaxServerCount )
	{
		m_SelectedServerIndex = m_MaxServerCount;
	}

	if( m_NextServerTrafficFactor[m_SelectedServerIndex] < 0 ) // 최종 선택된 서버가 서비스 불가인 경우
	{
		for( i = 0; i < m_MaxServerCount; i++ )
		{
			if( m_NextServerTrafficFactor[i] >= 0 )
			{
				m_SelectedServerIndex = i;
				g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "Last selected server", m_SelectedServerIndex );
				break;
			}
		}		
	}

	RebuildServerTrafficGraph();
	SelectServer( m_SelectedServerIndex );	
	*/

	return serverversion;
}

int CXLauncherDlg::VersionCheck()
{
	int checkversionstatus = ConnectVersionServer();
	if( checkversionstatus == -1 )
	{
		AfxMessageBox( IDS_ERRORCONNECTVERSIONCHECKSERVERSTRING );		
#ifdef  _LOGFILE_
		_XLog( TRUE, _T("ERROR : Failed to connect : patch information server\n") );
#endif					
		CString messageString;
		messageString = g_InterfaceString[18];
		m_ctlUpdateText.SetWindowText(messageString);
		m_ProgressRateStatic.ShowWindow( SW_HIDE );

		InsertStringToProgressMessageListCtrl( messageString );

		m_DownloadComplete = TRUE;

		m_TotalUpdateProgress.ShowWindow( SW_HIDE );
		m_DownloadProgress.ShowWindow( SW_HIDE );
		m_UpdateProgress.ShowWindow( SW_HIDE );
		m_ProgressRateStatic.ShowWindow( SW_HIDE );

		RedrawAllControls();

		return -1;
	}
	else if( checkversionstatus == -2 )
	{
		//RedrawAllControls();
		return -1;
	}
	else
	{
		if( this->m_PatchNumber == (DWORD)checkversionstatus )
		{
			CString messageString;
			messageString = g_InterfaceString[17];
			m_ctlUpdateText.SetWindowText(messageString);
			m_RunButton.EnableWindow( TRUE );
			m_ProgressRateStatic.ShowWindow( SW_HIDE );
			m_DownloadComplete = TRUE;
			m_UpdateComplete = TRUE;
			RedrawAllControls();
			ChangeTab( _XTABMODE_SELECTSERVER );

#ifdef _XNEXON_WEBLOGIN
			if( g_ExecuteParam.GetLength() > 0 && m_AutoRunIndex == 1 ) // passport가 parameter로 전달되었고 자동실행일시 게임 자동 실행.
			{
				Run();
			}
#endif

			return 0;
		}
		else if( this->m_PatchNumber > (DWORD)checkversionstatus )
		{
			AfxMessageBox( g_InterfaceString[4] );
			CString messageString;
			messageString.Format( _T("%s : %d/%d"), g_InterfaceString[4], m_PatchNumber, checkversionstatus );
			InsertStringToProgressMessageListCtrl( messageString );
			return -1;
		}
	}

	StartDownload_ScriptFile();
	
	return 1;
}

BOOL CXLauncherDlg::ProcessParsingPatchScript( void )
{
	CString messageString;
	// 스크립트 해석이 끝났으면 패치해야하는지 체크 
	if( ParsingPatchScript() == 1 )
	{
		// 서버쪽 버전이 높으면 패치파일 다운로드 후 패치 시작 
		if( this->m_PatchNumber < this->m_ServerPatchNumber )
		{
			CString tMessageString;
			
			DWORD Version_BugSection = (m_Version / 10000);
			DWORD Version_UpdateSection = m_Version - (Version_BugSection * 10000);
			
			DWORD ServerVersion_BugSection = (m_ServerVersion / 10000);
			DWORD ServerVersion_UpdateSection = m_ServerVersion - (ServerVersion_BugSection * 10000);
			
			messageString = g_InterfaceString[20];
			tMessageString.Format( messageString.operator LPCTSTR(),
				m_ServerPackageVersionName, ServerVersion_BugSection, ServerVersion_UpdateSection, 
				m_PackageVersionName, Version_BugSection, Version_UpdateSection );
			
			InsertStringToProgressMessageListCtrl( tMessageString );
			
			messageString = g_InterfaceString[21];
			tMessageString.Format( messageString.operator LPCTSTR(),
				m_ServerPatchNumber, m_PatchNumber );
			
			InsertStringToProgressMessageListCtrl( tMessageString );
			
			if( this->m_LauncherVersion < this->m_ServerLauncherVersion )
			{
				Version_BugSection = (m_LauncherVersion / 10000);
				Version_UpdateSection = m_LauncherVersion - (Version_BugSection * 10000);
				
				ServerVersion_BugSection = (m_ServerLauncherVersion / 10000);
				ServerVersion_UpdateSection = m_ServerLauncherVersion - (ServerVersion_BugSection * 10000);
				
				messageString = _T("XLauncher version   Server : %s %d.%d / Client : %s %d.%d");//.LoadString(IDS_XLAUNCHERVERSION);
				tMessageString.Format( messageString,
					m_ServerPackageVersionName, ServerVersion_BugSection, ServerVersion_UpdateSection, 
					m_PackageVersionName, Version_BugSection, Version_UpdateSection );
				
				InsertStringToProgressMessageListCtrl( tMessageString );
			}
			
			ShowWindow( SW_RESTORE );

			Update();
		}
		else if( this->m_PatchNumber > this->m_ServerPatchNumber )
		{
			CString messageString;
			messageString = g_InterfaceString[18];
			m_ctlUpdateText.SetWindowText(messageString);

			m_TotalUpdateProgress.ShowWindow( SW_HIDE );
			m_DownloadProgress.ShowWindow( SW_HIDE );
			m_UpdateProgress.ShowWindow( SW_HIDE );
			m_ProgressRateStatic.ShowWindow( SW_HIDE );
			m_DownloadComplete = TRUE;

			RedrawAllControls();

			AfxMessageBox( g_InterfaceString[4] );
			return FALSE;
		}
	}
	else 
	{
		CString messageString;
		messageString = g_InterfaceString[18];
		m_ctlUpdateText.SetWindowText(messageString);

		m_TotalUpdateProgress.ShowWindow( SW_HIDE );
		m_DownloadProgress.ShowWindow( SW_HIDE );
		m_UpdateProgress.ShowWindow( SW_HIDE );
		m_ProgressRateStatic.ShowWindow( SW_HIDE );
		m_DownloadComplete = TRUE;
		m_UpdateComplete = TRUE;

		RedrawAllControls();
		return FALSE;
	}

	m_TotalUpdateProgress.ShowWindow( SW_HIDE );
	m_DownloadProgress.ShowWindow( SW_HIDE );
	m_UpdateProgress.ShowWindow( SW_HIDE );
	m_ProgressRateStatic.ShowWindow( SW_HIDE );
	m_UpdateComplete = TRUE;
	m_DownloadComplete = TRUE;

	return TRUE;
}

int CXLauncherDlg::Update( void )
{
	// 서버쪽 배너 버전이 높으면 배너 다운로드 시작 
	if( this->m_BannerIndex < this->m_ServerBannerIndex )
	{
			// 패치된 배너인덱스를 레지스트리에 등록
#ifndef _DOWNLOADTEST
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_BANNERVERSION, this->m_ServerBannerIndex );
#endif
	}
		
	CString messageString;

#ifdef  _LOGFILE_
		_XLog( TRUE, _T("Package name : server : %s, client : %s\n"), m_ServerPackageVersionName, m_PackageVersionName );
#endif

	if( m_ServerPackageVersionName.CompareNoCase( m_PackageVersionName ) )
	{
		messageString = g_InterfaceString[22];
		AfxMessageBox( messageString );
#ifdef  _LOGFILE_
		_XLog( TRUE, _T("MESSAGE : Failed [ Different package ]. Exit.\n") );
#endif
		messageString = g_InterfaceString[18];
		m_ctlUpdateText.SetWindowText(messageString);
		RedrawAllControls();

		return -1;
	}

	m_patchprocess = _XPATCHDOWNPROCESS_PATCHFILE;

	m_TotalUpdateProgress.SetRange( 0, 100 );
	m_TotalUpdateProgress.SetPos( 0 );

	CString downloadstring, downloadedstring;
	downloadstring = g_InterfaceString[23];
	downloadedstring = g_InterfaceString[24];

	if( m_ServerPatchNumber - m_PatchNumber > 0 )
		m_OnePatchIncreaseFactor = 100.0f / (FLOAT)(m_ServerPatchNumber - (m_PatchNumber+1) + 1 );
	else
		m_OnePatchIncreaseFactor = 0.0f;

	for( DWORD patchindex = m_PatchNumber+1; patchindex <= m_ServerPatchNumber; patchindex++ )
	{
		int leftpatchcount = m_ServerPatchNumber - (m_PatchNumber+1);		
		int percentage = (int)( ((FLOAT)(patchindex-(m_PatchNumber+1)) / (FLOAT)leftpatchcount)  * 100 );

		m_PatchFilename.Format( _T("PATCH%d.XPZ"), patchindex );
		m_downloadfilename = m_PatchFilename;
			
		CString messageString;
		messageString.Format( downloadstring.GetBuffer(256), m_PatchFilename );
		InsertStringToProgressMessageListCtrl( messageString.operator LPCTSTR() );
		_XLog( TRUE, downloadstring.GetBuffer(256), m_PatchFilename );
		
		m_downloadtargetfilename.Format( _T("%s\\%s"), g_Installedpath, m_PatchFilename );

		m_UpdateProgress.SetPos(0);
		m_UpdateProgress.RedrawWindow();
		
		if( DownloadThread((void*)this) == 0 )
		{	
			messageString.Format( downloadedstring.GetBuffer(256), m_PatchFilename );
			InsertStringToProgressMessageListCtrl( messageString.operator LPCTSTR() );			
			_XLog( TRUE, downloadedstring.GetBuffer(256), m_PatchFilename );

			if( PatchProcess(patchindex) == 0 ) // failed.
			{
#ifdef  _LOGFILE_
				_XLog( TRUE, _T("\n") );
				messageString = g_InterfaceString[18];
				_XLog( TRUE, messageString.GetBuffer(256) );
				_XLog(TRUE, _T("\n") );
				messageString = g_InterfaceString[25];
				_XLog( TRUE, messageString.GetBuffer(256) );
				_XLog(TRUE, _T("\n") );
#endif
				messageString = g_InterfaceString[18];
				m_ctlUpdateText.SetWindowText(messageString);			
				m_ProgressRateStatic.ShowWindow( SW_HIDE );
				m_DownloadComplete = TRUE;
				ChangeTab( _XTABMODE_UPDATEMESSAGE );
				RedrawAllControls();
				return -1;							
			}

			m_TotalUpdateProgress.SetPos( m_OnePatchIncreaseFactor * ( patchindex - (m_PatchNumber+1) + 1 ) );

			CString progressmessageString;
			progressmessageString.Format( _T("%d %%"), m_TotalUpdateProgress.GetPos() );
			m_ProgressRateStatic.SetWindowText( progressmessageString );

			RedrawAllControls();
		}
		else
		{							
#ifdef  _LOGFILE_
			_XLog( TRUE, _T("\n") );
			messageString = g_InterfaceString[18];
			_XLog( TRUE, messageString.GetBuffer(256) );
			_XLog(TRUE, _T("\n") );
			messageString = g_InterfaceString[25];
			_XLog( TRUE, messageString.GetBuffer(256) );
			_XLog(TRUE, _T("\n") );
#endif
			messageString = g_InterfaceString[18];
			m_ctlUpdateText.SetWindowText(messageString);			
			m_ProgressRateStatic.ShowWindow( SW_HIDE );
			m_DownloadComplete = TRUE;
			ChangeTab( _XTABMODE_UPDATEMESSAGE );
			RedrawAllControls();
			return -1;							
		}						
	}

	CString progressmessageString;
	progressmessageString.Format( _T("100 %%") );
	m_ProgressRateStatic.SetWindowText( progressmessageString );	
	
	m_RunButton.EnableWindow( TRUE );				
	messageString = g_InterfaceString[17];
	m_ctlUpdateText.SetWindowText(messageString);
	m_ProgressRateStatic.ShowWindow( SW_HIDE );
	m_UpdateComplete = TRUE;
	m_DownloadComplete = TRUE;
	ChangeTab( _XTABMODE_SELECTSERVER );
	RedrawAllControls();
	
	//InsertStringToProgressMessageListCtrl(_T("Update complete."));

	_XPlayPatchCompleteSound();
	
#ifdef _XNEXON_WEBLOGIN
	if( g_ExecuteParam.GetLength() > 0 && m_AutoRunIndex == 1 ) // passport가 parameter로 전달되었고 자동실행일시 게임 자동 실행.
	{
		PostMessage (WM_COMMAND, ID_RUNBUTTON );
		//Run();
	}
#endif

	return 1;
}


int CXLauncherDlg::ParsingPatchScript( void )
{
	_XLog( FALSE, _T("Parse patch script\n") );

	FILE* PatchScriptFile = NULL;

	PatchScriptFile = fopen( "Patch.PSC", "rt" );

	if( !PatchScriptFile )
	{
		// File openning error...
#ifdef  _LOGFILE_
		_XLog( TRUE, _T("MESSAGE : ERROR [ Load patch information file ]\n") );
#endif
		return -1;
	}

	char argIDString[256];
	char argValueString[256];
	char parsingString[256];

	BOOL messagestart = FALSE;
	
	//m_ServerMessageList.DeleteAllItems();

	char tabcode[2];
	tabcode[0] = 9;
	tabcode[1] = 0;

	BOOL headercheck = TRUE;
	
	while( !feof( PatchScriptFile ) )
	{	
		if( feof( PatchScriptFile ) ) break;
				
		memset( parsingString, 0, sizeof(char)*256 );
		fgets( parsingString, 255, PatchScriptFile ); 

		if( headercheck && strcmpi( parsingString, "<h1>Bad Request (Invalid Hostname)</h1>" ) == 0 )
		{			
			fclose( PatchScriptFile );

			AfxMessageBox( g_InterfaceString[5] );
			InsertStringToProgressMessageListCtrl( _T("================================================================\n") );
			InsertStringToProgressMessageListCtrl( g_InterfaceString[5] );
			InsertStringToProgressMessageListCtrl( _T("================================================================\n") );
			_XLog( TRUE, _T("ERROR : Invalid patch information file\n") );
			return -1;
		}

		if( headercheck && strncmp( parsingString, "; DATE", 6 ) != 0 )
		{
			fclose( PatchScriptFile );
			
			AfxMessageBox( g_InterfaceString[5] ); 
			InsertStringToProgressMessageListCtrl( _T("================================================================\n") );
			InsertStringToProgressMessageListCtrl( g_InterfaceString[5] );
			InsertStringToProgressMessageListCtrl( _T("================================================================\n") );
			_XLog( TRUE, _T("ERROR : Invalid patch information file\n") );
			_XLog( TRUE, _T("Download file data : %s\n"), parsingString );
			return -1;
		}

		headercheck = FALSE;

		if( parsingString[0] != ';' ) 
		{
			memset( argIDString, 0, 256 );
			memset( argValueString, 0, 256 );
			sscanf( parsingString, "%s %s", argIDString, argValueString );

			if( messagestart )
			{
				/*
				if( strcmpi( argIDString, "ID_ENDMESSAGE" ) == 0 )
				{
					messagestart = FALSE;
				}
				else
				{					
					CString tstr = parsingString;					
					tstr.Replace( tabcode, "    " );
					tstr.Replace( '\n', '\0' );					
					InsertStringToServerMessageListCtrl( tstr );
				}*/
			}
			else
			{
				if( strcmpi( argIDString, "ID_PATCHINDEX" ) == 0 )
				{
					m_ServerPatchNumber = atol( argValueString );
				}
				if( strcmpi( argIDString, "ID_PACKAGEVERSION" ) == 0 )
				{
					TCHAR widestring[1024];
					memset( widestring, 0, sizeof(TCHAR) * 1024 );
					int requirelength = MultiByteToWideChar(CP_ACP, 0, argValueString, strlen(argValueString), widestring, 1024);

					m_ServerPackageVersionName = widestring;
				}
				else if( strcmpi( argIDString, "ID_VERSIONSERVERIP" ) == 0 )
				{
					g_ServerAddressManager.SetPatchServerAddress(argValueString);
#ifdef _XDEF_ADDRESSLOGGING
					TCHAR widestring[1024];
					memset( widestring, 0, sizeof(TCHAR) * 1024 );
					int requirelength = MultiByteToWideChar(CP_ACP, 0, argValueString, strlen(argValueString), widestring, 1024);

					_XLog( FALSE, _T("Change version server %s\n"), widestring );
#endif
				}
				else if( strcmpi( argIDString, "ID_PATCHSERVERIP" ) == 0 )
				{
					g_ServerAddressManager.SetCDNServerUrl(argValueString);

					TCHAR widestring[1024];
					memset( widestring, 0, sizeof(TCHAR) * 1024 );
					int requirelength = MultiByteToWideChar(CP_ACP, 0, argValueString, strlen(argValueString), widestring, 1024);

					m_PatchServerIPAddress = widestring;
#ifdef _XDEF_ADDRESSLOGGING
					_XLog( FALSE, _T("Change patch server %s\n"), widestring );
#endif
				}
				else if( strcmpi( argIDString, "ID_SERVERIP" ) == 0 )
				{
					g_ServerAddressManager.SetServerAddress(argValueString);
#ifdef _XDEF_ADDRESSLOGGING
					TCHAR widestring[1024];
					memset( widestring, 0, sizeof(TCHAR) * 1024 );
					int requirelength = MultiByteToWideChar(CP_ACP, 0, argValueString, strlen(argValueString), widestring, 1024);
					_XLog( FALSE, _T("Change login server %s\n"), widestring );
#endif
				}
				else if( strcmpi( argIDString, "ID_SERVERPORT" ) == 0 )
				{
					DWORD portnumber = atoi( argValueString );
					g_ServerAddressManager.SetServerPort(portnumber);
#ifdef _XDEF_ADDRESSLOGGING
					_XLog( FALSE, _T("Change server port %d\n"), portnumber);
#endif
				}
				else if( strcmpi( argIDString, "ID_MESSAGERSERVERIP" ) == 0 )
				{
					/*
					TCHAR widestring[1024];
					memset( widestring, 0, sizeof(TCHAR) * 1024 );
					int requirelength = MultiByteToWideChar(CP_ACP, 0, argValueString, strlen(argValueString), widestring, 1024);
					*/
					g_RegManager._XRegWriteString(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_MESSENGERSERVERADDRESS, argValueString );
				}
				else if( strcmpi( argIDString, "ID_LOGSERVERIP" ) == 0 )
				{
					/*
					TCHAR widestring[1024];
					memset( widestring, 0, sizeof(TCHAR) * 1024 );
					int requirelength = MultiByteToWideChar(CP_ACP, 0, argValueString, strlen(argValueString), widestring, 1024);
					*/
					g_RegManager._XRegWriteString(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "Log Ftp Server address", argValueString );
				}
				else if( strcmpi( argIDString, "ID_MESSAGERSERVERPORT" ) == 0 )
				{
					long portnumber = atol( argValueString );
					g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_MESSENGERSERVERPORT, portnumber );
				}
				else if( strcmpi( argIDString, "ID_PATCHVERSION" ) == 0 )
				{
					m_ServerVersion = atoi( argValueString );
				}
				else if( strcmpi( argIDString, "ID_LAUNCHERVERSION" ) == 0 )
				{
					m_ServerLauncherVersion = atol( argValueString );
				}
				else if( strcmpi( argIDString, "ID_BANNERVERSION" ) == 0 )
				{
					m_ServerBannerIndex = atoi( argValueString );
				}			
				else if( strcmpi( argIDString, "ID_BANNERFILE" ) == 0 )
				{
					TCHAR widestring[1024];
					memset( widestring, 0, sizeof(TCHAR) * 1024 );
					int requirelength = MultiByteToWideChar(CP_ACP, 0, argValueString, strlen(argValueString), widestring, 1024);
					m_BannerFilename = widestring;
				}
				else if( strcmpi( argIDString, "ID_MESSAGE" ) == 0 )
				{
					//messagestart = TRUE;			
				}
				else if( strcmpi( argIDString, "ID_HTMLVIEW" ) == 0 )
				{						
					TCHAR widestring[1024];
					memset( widestring, 0, sizeof(TCHAR) * 1024 );
					int requirelength = MultiByteToWideChar(CP_ACP, 0, argValueString, strlen(argValueString), widestring, 1024);

					::ShellExecute(NULL, _T("open"), widestring, NULL, NULL, SW_SHOWNORMAL);
				}
				else if( strcmpi( argIDString, "ID_NOTICEVIEW" ) == 0 )
				{					
					//m_NoticeDialog.ShowNoticePage( argValueString );
				}
				else if( strcmpi( argIDString, "ID_PATCHMYSELF" ) == 0 )
				{
					TCHAR widestring[1024];
					memset( widestring, 0, sizeof(TCHAR) * 1024 );
					int requirelength = MultiByteToWideChar(CP_ACP, 0, argValueString, strlen(argValueString), widestring, 1024);
					m_PatchMySelf = widestring;
				}
				else if( strcmpi( argIDString, "ID_RESETPATCHNUMBER" ) == 0 )
				{
					 DWORD resetpatchindex = atoi( argValueString );
					 //g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHNUMBER, resetpatchindex );
					 g_PatchVersionManager.SetPatchNumber( resetpatchindex );
				}				
			}
		}		
	}

	fclose( PatchScriptFile );

	
	if( CheckExistFile( _T("Patch.PSC"), TRUE ) ) ::DeleteFile( _T("Patch.PSC") );

	if( m_ServerMessageList.GetItemCount() == 0 )
	{
		CString messageString;
		messageString = g_InterfaceString[26];//.LoadString(IDS_NONOTICE);
		InsertStringToServerMessageListCtrl( messageString.operator LPCTSTR() );
	}

	return 1;
}


void CXLauncherDlg::SplitMessage( CString& sourceString, CString& destString )
{
	TCHAR tbuffer[512];

	memset( tbuffer, 0, sizeof(TCHAR)*512 );

	wcsncpy( tbuffer, sourceString, 511 );

	for( int i = 0; i < 511; i++ )
	{
		if( tbuffer[i] != _T(' ') ) break;
	}

	destString = tbuffer + i;
	
}



STDMETHODIMP CXLauncherDlg::_XGetExeFileVersion(LPTSTR sFilename, DWORD& h_1,DWORD& h_2,DWORD& l_1,DWORD& l_2 )
{
    // TODO: Add your implementation code here
    
  DWORD nInfoSize = GetFileVersionInfoSize(sFilename, 0);

  if( nInfoSize <= 0 ) 
  {
	  h_1 = h_2 = l_1 = l_2 = 0;
	  return E_FAIL;
  }

  BYTE* pBlock = new BYTE[nInfoSize+1];
  ZeroMemory(pBlock, nInfoSize+1);

  GetFileVersionInfo(sFilename, 0, nInfoSize, pBlock);

  UINT cbTranslate;

  struct VS_FIXEDFILEINFO { 
    DWORD dwSignature; 
    DWORD dwStrucVersion; 
    DWORD dwFileVersionMS; 
    DWORD dwFileVersionLS; 
    DWORD dwProductVersionMS; 
    DWORD dwProductVersionLS; 
    DWORD dwFileFlagsMask; 
    DWORD dwFileFlags; 
    DWORD dwFileOS; 
    DWORD dwFileType; 
    DWORD dwFileSubtype; 
    DWORD dwFileDateMS; 
    DWORD dwFileDateLS; 
  } *lpTranslate;

  // Read the list of languages and code pages.

  VerQueryValue(pBlock, 
                TEXT("\\"),
                (LPVOID*)&lpTranslate,
                &cbTranslate);

  h_1 = HIWORD(lpTranslate->dwFileVersionMS);
  h_2 = LOWORD(lpTranslate->dwFileVersionMS);
  l_1 = HIWORD(lpTranslate->dwFileVersionLS);
  l_2 = LOWORD(lpTranslate->dwFileVersionLS);
  
  delete pBlock;

  return S_OK;
}


void CXLauncherDlg::OnCancel() 
{
	if (mf_downloading) 
	{
		mf_continue = false;
		_XLog( TRUE, _T("Cancel...\n") );
		m_ctlUpdateText.SetWindowText( g_InterfaceString[6] );//_T("업데이트를 종료 중입니다.") );
		RedrawAllControls();
	} 
	else 
	{
		CDialog::OnCancel();
	}	
}

void CXLauncherDlg::OnSetupbutton() 
{
	if( m_CurrentTabMode == _XTABMODE_OPTIONVIEW )
	{
		if( m_UpdateComplete )
		{
			ChangeTab( _XTABMODE_SELECTSERVER );			
		}
		else
		{
			ChangeTab( _XTABMODE_UPDATEMESSAGE );
		}
	}
	else
	{
		ChangeTab( _XTABMODE_OPTIONVIEW );
	}

	RedrawAllControls();
}

void CXLauncherDlg::OnRunbutton()
{
	Run();
}

void CXLauncherDlg::OnJoinbutton() 
{
	CString urlstring;
	urlstring = g_InterfaceString[27];

	::ShellExecute(NULL, _T("open"), urlstring, NULL, NULL, SW_SHOWNORMAL);	
}

void CXLauncherDlg::OnSelectServerbutton()
{
	if( m_UpdateComplete )
	{
		m_ViewServerList = !m_ViewServerList;
		
		if( m_ViewServerList )
		{
			ChangeTab( _XTABMODE_SELECTSERVER );
			OnServerrefreshButton();
			m_ServerListRefreshButton.ShowWindow( SW_SHOW );
		}
		else
		{
			m_ServerListRefreshButton.ShowWindow( SW_HIDE );
		}

		RedrawAllControls();
	}
}

void CXLauncherDlg::StartDownload_ScriptFile( void )
{
	mf_continue = true;
	m_patchprocess = _XPATCHDOWNPROCESS_PATCHSCRIPT;	
	
	m_downloadfilename = _T( "PATCH.PSC" );
	m_downloadtargetfilename.Format( _T("%s\\PATCH.PSC"), g_Installedpath );

#ifdef _XUSE_MULTITHREADDOWNLOAD
	AfxBeginThread( DownloadThread, (void*)this );
#else
	DownloadThread( (void*)this );
#endif
	
}

UINT CXLauncherDlg::DownloadThread(void* param)
{
	CXLauncherDlg* self = (CXLauncherDlg*) param;
	self->mf_downloading = true;
	self->m_progmax = (UINT)-1;
	
#ifdef _XUSE_MULTITHREADDOWNLOAD
	if (!AfxSocketInit())
	{
		AfxMessageBox(g_InterfaceString[11]);
		return FALSE;
	}
#endif

	CString downloadfilename;

#ifdef _XTESTSERVER
	downloadfilename.Format( _T("/NDTestPatchData/%s"), self->m_downloadfilename );
#else
	#ifdef _XTW_CSTESTVERSION
		downloadfilename.Format( _T("/NDTestPatchData/%s"), self->m_downloadfilename );
	#else
		//#ifdef _XRELEASEDEBUG
		//	downloadfilename.Format( _T("/CTOTestData/%s"), self->m_downloadfilename );
		//#else
		//	downloadfilename.Format( _T("/NDPatchData/%s"), self->m_downloadfilename );

		#ifdef _XNEXON_NEWTESTVERSION
			downloadfilename.Format( _T("/NDNewTestPatchData/%s"), self->m_downloadfilename );
		#else
			downloadfilename.Format( _T("/NDPatchData/%s"), self->m_downloadfilename );
		#endif
		//#endif
	#endif
#endif

	BOOL retstatus = FALSE;

	switch( g_CurrentDownloadMethod ) 
	{
	default:
	case 0 :
		retstatus = self->XFileDownload_Method1( self->m_PatchServerIPAddress, downloadfilename, self->m_downloadtargetfilename );
		break;
	case 1 :
		retstatus = self->XFileDownload_Method2( self->m_PatchServerIPAddress, downloadfilename, self->m_downloadtargetfilename );
		break;
	case 2 :
		retstatus = self->XFileDownload_Method3( self->m_PatchServerIPAddress, downloadfilename, self->m_downloadtargetfilename );
		break;	
	case 3 :
		retstatus = self->XFileDownload_Method4( self->m_PatchServerIPAddress, downloadfilename, self->m_downloadtargetfilename );
		break;	
	case 4 :
		retstatus = self->XFileDownload_Method5( self->m_PatchServerIPAddress, downloadfilename, self->m_downloadtargetfilename );
		break;	
	}	
		
	self->mf_downloading = false;

	if ( retstatus && self->mf_continue)
	{
		if( self->m_patchprocess == _XPATCHDOWNPROCESS_PATCHSCRIPT )
		{
			self->InsertStringToServerMessageListCtrl(_T("Patch script file download complete"));
			self->ProcessParsingPatchScript();
		}
		else if( self->m_patchprocess == _XPATCHDOWNPROCESS_PATCHFILE )
		{
			self->InsertStringToServerMessageListCtrl(_T("Patch file download complete"));
		}
	}
	else 
	{		
		self->InsertStringToServerMessageListCtrl(_T("Failed to download"));
		
		CString messageString;
		messageString = g_InterfaceString[18];
		self->m_ctlUpdateText.SetWindowText(messageString);
		self->RedrawAllControls();

		return 1;
	}

	return 0;
}

bool CXLauncherDlg::XFileDownload_Method4( const TCHAR* IP, LPCTSTR URL, LPCTSTR FileName )
{
	CString szHTTPFilePath;
	szHTTPFilePath = _T("http://");
	szHTTPFilePath +=IP;
	szHTTPFilePath +=URL;

	char mbcsstring[1024];		
	memset( mbcsstring, 0, 1024 );
	WideCharToMultiByte(CP_ACP, 0, szHTTPFilePath, -1, mbcsstring, 1024, NULL, NULL);

	
	BOOL retval = DeleteUrlCacheEntry(mbcsstring);

	if( !retval )
	{
		CString sMsg;
		sMsg.Format( _T("ERROR : URL Cache : Failed to delete, %d\n"), GetLastError() );
		_XLog( TRUE, sMsg );
	}
	
	CCallback callback;
	HRESULT   hr;

	callback.m_pDlg = this;

	callback.m_bUseTimeout = TRUE;	
	callback.m_timeToStop = CTime::GetCurrentTime() + CTimeSpan( 0, 0, 0, 30 );

	m_DownloadProgress.SetRange( 0, 0 );

	hr = URLDownloadToFile ( NULL,      // ptr to ActiveX container
							 szHTTPFilePath,      // URL to get
							 FileName,     // file to store data in
							 0,         // reserved
							&callback  // ptr to IBindStatusCallback 
						   );
	
    if ( FAILED(hr) )
	{
        LPTSTR lpszErrorMessage;
        CString sMsg;
		
        if ( FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, 
							 hr, MAKELANGID ( LANG_NEUTRAL, SUBLANG_DEFAULT ),
							 (LPTSTR) &lpszErrorMessage, 0, NULL ))
		{
            sMsg.Format ( _T("Download failed.  Error = 0x%08lX\n\n%s"), (DWORD) hr, lpszErrorMessage );
            LocalFree ( lpszErrorMessage );
		}
        else
		{
            sMsg.Format ( _T("Download failed.  Error = 0x%08lX\n\nNo message available."), (DWORD) hr );
		}

		_XLog(TRUE, sMsg );
		
		memset( mbcsstring, 0, 1024 );
		WideCharToMultiByte(CP_ACP, 0, callback.m_DownloadData, -1, mbcsstring, 1024, NULL, NULL);
		retval = DeleteUrlCacheEntry(mbcsstring);

		if( !retval )
		{
			CString sMsg;
			sMsg.Format( _T("ERROR : URL Cache : Failed to delete, %d\n"), GetLastError() );
			_XLog( TRUE, sMsg );
		}

		return false;
	}

	memset( mbcsstring, 0, 1024 );
	WideCharToMultiByte(CP_ACP, 0, callback.m_DownloadData, -1, mbcsstring, 1024, NULL, NULL);
	retval = DeleteUrlCacheEntry(mbcsstring);

	if( !retval )
	{
		CString sMsg;
		sMsg.Format( _T("ERROR : URL Cache : Failed to delete, %d\n"), GetLastError() );
		_XLog( TRUE, sMsg );
	}

	return true;
}

CCallback::CCallback() : m_bUseTimeout(FALSE), m_pDlg(NULL)
{
	m_DownloadData = _T("");
}

CCallback::~CCallback()
{
}

HRESULT CCallback::OnProgress ( ULONG ulProgress,   ULONG ulProgressMax,
							   ULONG ulStatusCode, LPCWSTR wszStatusText )
{
	// Local variables are declared static so they don't have to be reallocated on
	// the stack every time.  This is safe in this app since I know I'll only have
	// one thread downloading.
	static CString sIEStatusMsg;
	static TCHAR   szCustomStatusMsg [256];
	static TCHAR   szAmtDownloaded [256], szTotalSize [256];
	
    UNREFERENCED_PARAMETER(ulStatusCode);

	if( ulStatusCode == BINDSTATUS_BEGINDOWNLOADDATA )
	{
		if( wcslen(wszStatusText) )
		{
			char mbcsstring[1024];		
			memset( mbcsstring, 0, 1024 );
			WideCharToMultiByte(CP_ACP, 0, wszStatusText, -1, mbcsstring, 1024, NULL, NULL); 
			BOOL retval = DeleteUrlCacheEntry(mbcsstring);
		}

		m_DownloadData = wszStatusText;
	}
	if( ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA )
	{
		if( wcslen(wszStatusText) )
		{
			char mbcsstring[1024];		
			memset( mbcsstring, 0, 1024 );
			WideCharToMultiByte(CP_ACP, 0, wszStatusText, -1, mbcsstring, 1024, NULL, NULL); 
			BOOL retval = DeleteUrlCacheEntry(mbcsstring);
		}
	}
	
    // Did the user hit the Stop button?
    if ( !m_pDlg->mf_continue )
        return E_ABORT;
	
    // Has the timeout period elapsed?
    if ( m_bUseTimeout  &&  CTime::GetCurrentTime() > m_timeToStop )
	{
		AfxMessageBox( _T("ERROR : Connection time out!") );
        return E_ABORT;
	}
	
	int maxrange = m_pDlg->m_DownloadProgress.GetRangeMax();
	if( maxrange == 0 && ulProgressMax > 0 )
	{
		m_pDlg->m_DownloadProgress.SetRangeMax( ulProgressMax, TRUE );
	}

	/*
    // Use CString to convert IE's status message to a TCHAR string.
    if ( NULL != wszStatusText )
	{
        sIEStatusMsg = wszStatusText;
	}
    else
	{
        sIEStatusMsg.Empty();
	}
	*/	

	m_pDlg->m_DownloadProgress.SetPos( ulProgress );

    // Make our own progress message - we'll show the amount downloaded and
    // the total file size (if known).
	
	/*
    StrFormatByteSize ( ulProgress, szAmtDownloaded, 256 );
    StrFormatByteSize ( ulProgressMax, szTotalSize, 256 );
    
    if ( 0 != ulProgressMax )
	{
        wsprintf ( szCustomStatusMsg, _T("Downloaded %s of %s"),
			szAmtDownloaded, szTotalSize );
	}
    else
	{
        wsprintf ( szCustomStatusMsg, _T("Downloaded %s (total size unknown)"),
			szAmtDownloaded );
	}
	*/
	
    // Report the progress back to the main window.
	
	/*
    if ( 0 != ulProgressMax )
	{
        //m_pDlg->ProgressUpdate ( sIEStatusMsg, szCustomStatusMsg, int( 100.0 * ulProgress / ulProgressMax) );
	}
    else
	{
        //m_pDlg->ProgressUpdate ( sIEStatusMsg, szCustomStatusMsg, 0 );
	}
	*/
    return S_OK;
}


bool CXLauncherDlg::XFileDownload_Method5( const TCHAR* IP, LPCTSTR URL, LPCTSTR FileName )
{
	HOSTENT * pEnt = NULL ;	
	
	CString messagestr;
	messagestr.Format(_T("Find patch host...\n"));
	InsertStringToProgressMessageListCtrl(messagestr);
	_XLog( TRUE, messagestr.GetBuffer(256) );

	TCHAR widestring [1024];
	char mbcsstring[1024];		
	memset( mbcsstring, 0, 1024 );
	WideCharToMultiByte(GetACP(), 0, IP, -1, mbcsstring, 1024, NULL, NULL);
	
	char ipaddress[256];
	memset( ipaddress, 0, sizeof(char) * 256 );

	if( !_XIsIPv4Address( mbcsstring, strlen(mbcsstring) ) )
	{		
		if( !_XGetHostName( mbcsstring, ipaddress, 256 ) )
		{
			CString messagestr;
			messagestr.Format(_T("Could not found patch host\n"));
			InsertStringToProgressMessageListCtrl(messagestr);
			_XLog( TRUE, messagestr.GetBuffer(256) );
			return FALSE;
		}
	}
	else
	{
		strcpy( ipaddress, mbcsstring);
	}

    CString Request;
    int nRead, nContentRead=0, nContent=1;
    char szTmp[4097], *psztmp;
    bool bContent=false;
	std::ofstream ofs;
	
	int strlength = strlen( ipaddress );	
	memset( widestring, 0, sizeof(TCHAR) * 1024 );
	int requirelength = MultiByteToWideChar(CP_ACP, 0, ipaddress, strlength, widestring, 1024);

    Request = _T("GET ");
    Request += URL;
    Request += _T(" HTTP/1.1\r\n");
    Request += _T("Host: ");
    Request += widestring;
    Request += _T("\r\n\r\n"); // 2개의 CRLF 로 message-header의 끝을 구분한다.
	
	if( CheckExistFile( FileName, TRUE ) ) ::DeleteFile( FileName );
    
	messagestr.Format(_T("Connect patch server\n"));
	InsertStringToProgressMessageListCtrl(messagestr);
	_XLog( TRUE, messagestr.GetBuffer(256) );

	m_DownloadProgress.SetWindowText( messagestr );

	if ( g_DownloadSocket.Create( /*80, SOCK_STREAM, IP*/ ) == 0 ) 
	{
		messagestr.Format(_T("ERROR : Could not create socket\n"));	
		InsertStringToProgressMessageListCtrl(messagestr);
		_XLog( TRUE, messagestr.GetBuffer(256) );
		m_DownloadProgress.SetWindowText(_T("Download error"));

		mf_continue = false;
		g_DownloadSocket.Close();
		return false;
	}

	messagestr.Format(_T("Socket created"), 0);
	m_DownloadProgress.SetWindowText( messagestr );

	strlength = strlen( ipaddress );
	memset( widestring, 0, sizeof(TCHAR) * 1024 );
	requirelength = MultiByteToWideChar(CP_ACP, 0, ipaddress, strlength, widestring, 1024);

	if ( g_DownloadSocket.Connect( widestring, 80 ) == 0 )
	{
		messagestr.Format(_T("ERROR : Could not connect patch download server\n"));
		InsertStringToProgressMessageListCtrl(messagestr);
		_XLog( TRUE, messagestr.GetBuffer(256) );
		m_DownloadProgress.SetWindowText(_T("Download error"));
		
		mf_continue = false;
		g_DownloadSocket.Close();
		return false;
	}

	messagestr.Format(_T("Socket Connected"), 0);
	m_DownloadProgress.SetWindowText( messagestr );
	
    OutputDebugString( Request );

	memset( mbcsstring, 0, 1024 );
	WideCharToMultiByte(GetACP(), 0, Request, -1, mbcsstring, 1024, NULL, NULL);

	if ( g_DownloadSocket.Send( mbcsstring, strlen(mbcsstring) ) == SOCKET_ERROR )
	{
		messagestr.Format(_T("ERROR : Could not send Request packet\n"));	
		InsertStringToProgressMessageListCtrl(messagestr);
		_XLog( TRUE, messagestr.GetBuffer(256) );
		m_DownloadProgress.SetWindowText(_T("Download error"));

		mf_continue = false;				
		g_DownloadSocket.Close();
		return false;	
	}
	
	memset( mbcsstring, 0, 1024 );
	WideCharToMultiByte(GetACP(), 0, FileName, -1, mbcsstring, 1024, NULL, NULL);

    ofs.open( mbcsstring, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary );
	
	CString progressstr;

    do
    {
		if (!mf_continue) 
		{
			messagestr.Format(_T("ERROR : Failed to download\n"));	
			InsertStringToProgressMessageListCtrl(messagestr);
			_XLog( TRUE, messagestr.GetBuffer(256) );
			
			ofs.close();
			g_DownloadSocket.Close();			

			if( CheckExistFile( FileName, TRUE ) ) ::DeleteFile( FileName );

			return false;
		} 

        nRead = g_DownloadSocket.Receive( szTmp, 4096 );
        if ( nRead == 0 ) // 메시지를 다 받았으면
            break;

		if( strstr( szTmp, "400 Bad Request" ) != 0 )
		{
			AfxMessageBox (_T("ERROR : Recieved invalid data"));
			szTmp[64] = NULL;
			CString messagestr;
			messagestr.Format(_T("ERROR : Recieved invalid data\n") );	
			_XLog( TRUE, messagestr.GetBuffer(256) );
			m_DownloadProgress.SetWindowText(_T("Download error"));
			
			mf_continue = false;
			g_DownloadSocket.Close();
			ofs.close();
			return false;
		}

		if( nRead == SOCKET_ERROR )
		{
			int errornumber = GetLastError();
			if ( errornumber != WSAEWOULDBLOCK) 
			{
				CString messagestr;
				messagestr.Format(_T("ERROR : Recieved error [ %d ]\n"), errornumber );	
				_XLog( TRUE, messagestr.GetBuffer(256) );
				m_DownloadProgress.SetWindowText(_T("Download error"));
				
				mf_continue = false;				
				g_DownloadSocket.Close();
				ofs.close();
				return false;
			}
		}
		
        if ( nRead == WSAEWOULDBLOCK ) // sub-network fail
            break;
		
        szTmp[nRead] = 0;
		
        if ( bContent ) // message-body 부분을 받고있을 때
        {
            nContentRead += nRead;
            ofs.write( szTmp, nRead );
			
			m_DownloadProgress.SetPos(nContentRead);
			m_DownloadProgress.RedrawWindow();
			//progressstr.Format("Download complete %.2fKb/%.2fKb", nContentRead/1000.0f, nContent/1000.0f );
			//m_DownloadProgress.SetWindowText( progressstr );
        }
        else // message-header 부분을 받고 있을 때
        {
            // Content-Length == message-body의 길이
            if ( (psztmp = strstr( szTmp, "Content-Length:" )) != NULL )
			{
                nContent = atoi( psztmp+16 ); 
				m_DownloadProgress.SetRange(0, nContent);
				m_DownloadProgress.SetPos(0);
				m_DownloadProgress.RedrawWindow();
			}
			
            if ( ( psztmp = strstr( szTmp, "\r\n\r\n") ) != NULL) // message-header과 message-body의 경계(4BYTE)
            {
                bContent = true;
                nContentRead = nRead - (psztmp+4 - szTmp); // 받은 부분 중 message-header 부분과 경계를 제외
                ofs.write( psztmp+4, nContentRead );
				
				m_DownloadProgress.SetPos(nContentRead);
				m_DownloadProgress.RedrawWindow();
				//progressstr.Format("Download complete %.2fKb/%.2fKb", nContentRead/1000.0f, nContent/1000.0f );
				//m_DownloadProgress.SetWindowText( progressstr );
            }
        }
    } while ( nContentRead < nContent ); // 다운 받은 message-body 부분이 Content-Length 와 같아질때 까지    
	
	m_DownloadProgress.RedrawWindow();

    ofs.close();
    g_DownloadSocket.Close();
	
    return true;
}

bool CXLauncherDlg::XFileDownload_Method1( const TCHAR* IP, LPCTSTR URL, LPCTSTR FileName )
{
	HINTERNET		hInternetSession = NULL;
	HINTERNET		hHttpConnection = NULL;
	HINTERNET		hHttpFile = NULL;
	DWORD			dwServiceType;
	INTERNET_PORT	nPort;	
	CString			sObject;
	CString			sServer;
	CString			sURLToDownload = IP;
	sURLToDownload  += URL;

	if (!AfxParseURL(sURLToDownload, dwServiceType, sServer, sObject, nPort))
	{
		//Try sticking "http://" before it
		sURLToDownload = _T("http://") + sURLToDownload;
		if (!AfxParseURL(sURLToDownload, dwServiceType, sServer, sObject, nPort))
		{
			_XLog(TRUE,_T("Failed to parse the URL: %s\n"), sURLToDownload);
			return false;
		}
	}

	_XLog(TRUE,_T("Download thread : %s -> %s\n"),  sURLToDownload, FileName );

	hInternetSession = ::InternetOpen(AfxGetAppName(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternetSession == NULL)
	{
		_XLog(TRUE,_T("Failed in call to InternetOpen, Error:%d\n"), ::GetLastError());
		return false;
	}

	//Should we exit the thread
	if (!mf_continue)
	{
		VERIFY(::InternetCloseHandle(hInternetSession));
		return false;
	}  
	
	/*
	//Setup the status callback function
	if (::InternetSetStatusCallback(hHttpFile, _OnStatusCallBack) == INTERNET_INVALID_STATUS_CALLBACK)
	{
		_XLog(TRUE,_T("Failed in call to InternetSetStatusCallback, Error:%d\n"), ::GetLastError());
		HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_GENERIC_ERROR);
		return;
	}
	
	//Should we exit the thread
	if (m_bAbort)
	{
		PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED);
		return;
	}  
	*/

	//Make the connection to the HTTP server          
	ASSERT(hHttpFile== NULL);

	hHttpConnection = ::InternetConnect(hInternetSession, sServer, nPort, NULL, 
											  NULL, dwServiceType, 0, (DWORD) this);
	if (hHttpConnection== NULL)
	{
		_XLog(TRUE,_T("Failed in call to InternetConnect, Error:%d\n"), ::GetLastError());
		VERIFY(::InternetCloseHandle(hInternetSession));
		return false;
	}

	//Should we exit the thread
	if (!mf_continue)
	{
		VERIFY(::InternetCloseHandle(hHttpConnection));
		VERIFY(::InternetCloseHandle(hInternetSession));		
		return false;
	}

	//Issue the request to read the file
	LPCTSTR ppszAcceptTypes[2];
	ppszAcceptTypes[0] = _T("*/*");  //We support accepting any mime file type since this is a simple download of a file
	ppszAcceptTypes[1] = NULL;
	ASSERT(hHttpFile == NULL);
	hHttpFile = HttpOpenRequest(hHttpConnection, NULL, sObject, NULL, NULL, ppszAcceptTypes, INTERNET_FLAG_RELOAD | 
									INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION, (DWORD) this);
	if (hHttpFile == NULL)
	{
		_XLog(TRUE,_T("Failed in call to HttpOpenRequest, Error:%d\n"), ::GetLastError());
		VERIFY(::InternetCloseHandle(hHttpConnection));
		VERIFY(::InternetCloseHandle(hInternetSession));		
		return false;
	}

	//Should we exit the thread	
	if (!mf_continue)
	{
		VERIFY(::InternetCloseHandle(hHttpConnection));
		VERIFY(::InternetCloseHandle(hInternetSession));		
		return false;
	}	

//label used to jump to if we need to resend the request
resend:

	//Issue the request
	BOOL bSend = ::HttpSendRequest(hHttpFile, NULL, 0, NULL, 0);
	if (!bSend)
	{
		_XLog(TRUE,_T("Failed in call to HttpSendRequest, Error:%d\n"), ::GetLastError());
		VERIFY(::InternetCloseHandle(hHttpFile));
		VERIFY(::InternetCloseHandle(hHttpConnection));
		VERIFY(::InternetCloseHandle(hInternetSession));
		return false;
	}

	//ERROR_FILE_NOT_FOUND 

	//Check the HTTP status code
	TCHAR szStatusCode[32];
	DWORD dwInfoSize = 32;
	if (!HttpQueryInfo(hHttpFile, HTTP_QUERY_STATUS_CODE, szStatusCode, &dwInfoSize, NULL))
	{
		_XLog(TRUE,_T("Failed in call to HttpQueryInfo for HTTP query status code, Error:%d\n"), ::GetLastError());
		VERIFY(::InternetCloseHandle(hHttpFile));
		VERIFY(::InternetCloseHandle(hHttpConnection));
		VERIFY(::InternetCloseHandle(hInternetSession));
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
		  if (::InternetErrorDlg(GetSafeHwnd(), hHttpFile, ERROR_INTERNET_INCORRECT_PASSWORD, FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
								 FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL) == ERROR_INTERNET_FORCE_RETRY)
			goto resend;
		}
  		else if (nStatusCode != HTTP_STATUS_OK)
		{
		  _XLog(TRUE,_T("Failed to retrieve a HTTP 200 status, Status Code:%d\n"), nStatusCode);
		  VERIFY(::InternetCloseHandle(hHttpFile));
		  VERIFY(::InternetCloseHandle(hHttpConnection));
		  VERIFY(::InternetCloseHandle(hInternetSession));		  
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
		//SetProgressRange(dwFileSize);
	}

	m_DownloadProgress.SetRange( 0, dwFileSize );

	CFile fDestFile;	
	if( CheckExistFile( FileName, TRUE ) ) ::DeleteFile( FileName );	
	if( !fDestFile.Open( FileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ) )
	{
		_XLog(TRUE,_T("Failed to create file, Error:%d\n"), ::GetLastError());
		VERIFY(::InternetCloseHandle(hHttpFile));
		VERIFY(::InternetCloseHandle(hHttpConnection));
		VERIFY(::InternetCloseHandle(hInternetSession));
		fDestFile.Close();
		return false;
	}

	//Now do the actual read of the file
	int	  orgtotalprogress = m_TotalUpdateProgress.GetPos();
	DWORD dwBytesRead = 0;
	char  szReadBuf[1024];
	DWORD dwBytesToRead = 1024;
	DWORD dwTotalBytesRead = 0;
	DWORD dwLastTotalBytes = 0;
	DWORD dwLastPercentage = 0;
	FLOAT totalprogress = 0.0f;
	do
	{
		//InternetQueryDataAvailable(,,) 

		if (!::InternetReadFile(hHttpFile, szReadBuf, dwBytesToRead, &dwBytesRead))
		{
		  _XLog(TRUE,_T("Failed in call to InternetReadFile, Error:%d\n"), ::GetLastError());
		  VERIFY(::InternetCloseHandle(hHttpFile));
		  VERIFY(::InternetCloseHandle(hHttpConnection));
		  VERIFY(::InternetCloseHandle(hInternetSession));
		  fDestFile.Close();
		  return false;
		}
		else if (dwBytesRead && mf_continue )
		{
		  //Write the data to file
		  TRY
		  {
			fDestFile.Write(szReadBuf, dwBytesRead);
		  }
		  CATCH(CFileException, e);                                          
		  {
			_XLog(TRUE,_T("An exception occured while writing to the download file\n"));
			e->Delete();
			VERIFY(::InternetCloseHandle(hHttpFile));
			VERIFY(::InternetCloseHandle(hHttpConnection));
			VERIFY(::InternetCloseHandle(hInternetSession));
			fDestFile.Close();
			return false;
		  }
		  END_CATCH

		  //Increment the total number of bytes read
		  dwTotalBytesRead += dwBytesRead;

		  m_DownloadProgress.SetPos(dwTotalBytesRead);
		  m_DownloadProgress.RedrawWindow();
		  
		  if( m_OnePatchIncreaseFactor > 0.0f )
		  {
			  totalprogress = (m_OnePatchIncreaseFactor/2.0f) * ( (FLOAT)dwTotalBytesRead / (FLOAT)dwFileSize );
			  m_TotalUpdateProgress.SetPos( orgtotalprogress + (int)totalprogress );
			  m_TotalUpdateProgress.RedrawWindow();
		  }
		}
	} 
	while (dwBytesRead && mf_continue );

	//Delete the file being downloaded to if it is present and the download was aborted
	fDestFile.Close();
	
	if(!mf_continue)
	{
		if( CheckExistFile( FileName, TRUE ) ) ::DeleteFile( FileName );	
		return false;
	}	

	// Close all open Internet handles
	VERIFY(::InternetCloseHandle(hHttpFile));
	VERIFY(::InternetCloseHandle(hHttpConnection));
	VERIFY(::InternetCloseHandle(hInternetSession));

	return true;
}

bool CXLauncherDlg::XFileDownload_Method2( const TCHAR* IP, LPCTSTR URL, LPCTSTR FileName )
{
   // Initialize the Internet DLL

   HINTERNET hSession = ::InternetOpen(_T("Raw HTML Reader"), PRE_CONFIG_INTERNET_ACCESS, _T(""), INTERNET_INVALID_PORT_NUMBER, 0);

   // See if the session handle is valid
   if (hSession == NULL)
   {
      AfxMessageBox(_T("Internet session initialization failed!"), MB_OK | MB_ICONEXCLAMATION);
      return false;
   }

   // Initialize HTTP session
   HINTERNET hConnect = ::InternetConnect(hSession, IP, INTERNET_INVALID_PORT_NUMBER, _T(""), _T(""), INTERNET_SERVICE_HTTP, 0, 0);

   // See if connection handle is valid
   if (hConnect == NULL)
   {
      AfxMessageBox(_T("Internet connection failed!"), MB_OK | MB_ICONEXCLAMATION);
      // Close session handle
      VERIFY(::InternetCloseHandle(hSession));
      return false;
   }

   // Open an HTTP request handle
   HINTERNET hHttpFile = ::HttpOpenRequest(hConnect, _T("GET"), URL, HTTP_VERSION, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);

   // See if HTTP request handle is valid
   if (hHttpFile == NULL)
   {
      AfxMessageBox(_T("HTTP request failed!"), MB_OK | MB_ICONEXCLAMATION);

      // Close session handles
      VERIFY(::InternetCloseHandle(hConnect));
      VERIFY(::InternetCloseHandle(hSession));
      return false;
   }
   
   // Send the request
   BOOL bSendRequest = ::HttpSendRequest(hHttpFile, NULL, 0, 0, 0);
   if (bSendRequest)
   {
      // Get the size of the requested file
      TCHAR  achQueryBuf[256];
	  memset( achQueryBuf, 0, sizeof(TCHAR) * 256 );
      DWORD dwFileSize;
      DWORD dwQueryBufLen = sizeof(achQueryBuf);
      BOOL  bQuery = ::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, achQueryBuf, &dwQueryBufLen, NULL);

      if (bQuery)
      {
         // The query succeeded, specify memory needed for file
         dwFileSize = (DWORD)_ttol(achQueryBuf);
      }
      else
      {
         // The query failed, so guess at a max file size
         dwFileSize = 1024 * 1024 * 100; // max 100mb
      }

	  #define	FILE_SIZE	65536	
	  char szBuff[FILE_SIZE];

	  CFile fDestFile;

	  if( CheckExistFile( FileName, TRUE ) ) ::DeleteFile( FileName );	
	  
	  fDestFile.Open( FileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary );
	  
	  int		orgtotalprogress = m_TotalUpdateProgress.GetPos();
	  FLOAT		totalprogress = 0.0f;
	  DWORD		dwBytesRead = 0;
	  int		pos = 0;

	  m_DownloadProgress.SetRange( 0, dwFileSize );
	  
	  while( 1 )
	  {
			BOOL bRead = ::InternetReadFile(hHttpFile, szBuff, FILE_SIZE, &dwBytesRead);

			if( dwBytesRead == 0 ) break;

			if( !mf_continue )
			{
				break;
			}

			pos += dwBytesRead;
			
			fDestFile.Write(szBuff,dwBytesRead);
		  
			m_DownloadProgress.SetPos(pos);
			m_DownloadProgress.RedrawWindow();
			  
			if( m_OnePatchIncreaseFactor > 0.0f )
			{
				totalprogress = (m_OnePatchIncreaseFactor/2.0f) * ( (FLOAT)pos / (FLOAT)dwFileSize );
				m_TotalUpdateProgress.SetPos( orgtotalprogress + (int)totalprogress );
				m_TotalUpdateProgress.RedrawWindow();
			}
			  
			//Sleep(5);
	  }
      
      // Close all open Internet handles
      VERIFY(::InternetCloseHandle(hHttpFile));
      VERIFY(::InternetCloseHandle(hConnect));
      VERIFY(::InternetCloseHandle(hSession));
   }

   if( !mf_continue )
   {
		if( CheckExistFile( FileName, TRUE ) ) ::DeleteFile( FileName );
		return false;
   }

   return true;
}

bool CXLauncherDlg::XFileDownload_Method3( const TCHAR* IP, LPCTSTR URL, LPCTSTR FileName )
{
	CInternetSession netSession;
	//CHttpConnection* pServer = NULL;

	CString szHTTPFilePath;
	szHTTPFilePath = _T("http://");
	szHTTPFilePath +=IP;
	szHTTPFilePath +=URL;
	
	CStdioFile* fTargetFile = NULL;
	
	CString messagestr;
	messagestr.Format(_T("Server connect, requesting..."));
	InsertStringToProgressMessageListCtrl(messagestr);
	_XLog( TRUE, messagestr.GetBuffer(256) );

	//OutputDebugString( szHTTPFilePath );
	
	CFile fDestFile;

	try
	{
	fTargetFile = netSession.OpenURL(szHTTPFilePath,1, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD );

	if( !fTargetFile )
	{
		//catch errors from WinInet
		CString messagestr;
		messagestr.Format( _T("ERROR : XFileDownload_Method3 : Failed open url") );
		InsertStringToProgressMessageListCtrl(messagestr);
		_XLog( TRUE, messagestr.GetBuffer(256) );		
	}

	DWORD filesize = 0;
	filesize = fTargetFile->GetLength();
	
	//filesize = fTargetFile->SeekToEnd();		
	//fTargetFile->SeekToBegin();

	m_DownloadProgress.SetRange(0, filesize);
	m_DownloadProgress.SetPos(0);
	m_DownloadProgress.RedrawWindow();
	m_UpdateProgress.SetPos( 0 );

	int bytesread=0;
	int pos=0;

	#define _XDOWNLOAD_CHUNKSIZE	512	//	65536

	char strbuf[_XDOWNLOAD_CHUNKSIZE];

	if( CheckExistFile( FileName, TRUE ) ) ::DeleteFile( FileName );	

	fDestFile.Open( FileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary );

	int		orgtotalprogress = m_TotalUpdateProgress.GetPos();
	FLOAT	totalprogress = 0.0f;

	m_DownloadProgress.SetRange( 0, filesize );
		
	while( (bytesread = fTargetFile->Read( strbuf, _XDOWNLOAD_CHUNKSIZE)) && mf_continue )
	{
		pos+=bytesread;
		fDestFile.Write(strbuf,bytesread);
		
		m_DownloadProgress.SetPos(pos);
		m_DownloadProgress.RedrawWindow();

		if( m_OnePatchIncreaseFactor > 0.0f )
		{
			totalprogress = (m_OnePatchIncreaseFactor/2.0f) * ( (FLOAT)pos / (FLOAT)filesize );
			m_TotalUpdateProgress.SetPos( orgtotalprogress + (int)totalprogress );
			m_TotalUpdateProgress.RedrawWindow();
		}
		
		//Sleep(5);
	}

	} // end of try

	catch (CInternetException* pEx)
	{
		//catch errors from WinInet
		CString messagestr;
		messagestr.Format(_T("ERROR : XFileDownload_Method3 : Failed to download. [%s,%d:%d]"), URL, pEx->m_dwError,pEx->m_dwContext );
		InsertStringToProgressMessageListCtrl(messagestr);
		_XLog( TRUE, messagestr.GetBuffer(256) );

		AfxMessageBox( g_InterfaceString[25] );

		if( CheckExistFile( FileName, TRUE ) ) ::DeleteFile( FileName );
		
		pEx->Delete();
		return false;
	}

	fDestFile.Close();
	fTargetFile->Close();
	delete fTargetFile;
	netSession.Close();

	if( !mf_continue )
	{
		if( CheckExistFile( FileName, TRUE ) ) ::DeleteFile( FileName );
		return false;
	}
	
	return true;
}



void CXLauncherDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
	
	if( m_CurrentTabMode == _XTABMODE_SELECTSERVER )
	{
		if( m_ViewServerList )
		{
			if( !((point.x >= 304 && point.y >= 69 && point.x <= 486 && point.y <= 247) ||
				  (point.x >= 496 && point.y >= 69 && point.x <= 679 && point.y <= 207) ) )
				  {
						PostMessage (WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
				  }	
			else
			{
				int xcoordfactor = 0;
				int ycoordfactor = 0;
				for( int serverindex = 0; serverindex < m_MaxServerCount; serverindex++ )
				{
					if( point.x >= 303+xcoordfactor && point.y >= 68+ycoordfactor && 
						point.x < 303+xcoordfactor+184 && point.y < 68+ycoordfactor+18 )
					{
						SelectServer( serverindex );
						g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "Last selected server", m_SelectedServerIndex );
						break;
					}
					
					ycoordfactor += 18;
					
					if( serverindex == 9 )
					{
						xcoordfactor = 193;
						ycoordfactor = 0;
					}
				}
			}
		}
		else
		{
			PostMessage (WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
		}
	}
	else
	{
		PostMessage (WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
	}
}

void CXLauncherDlg::SetTestServerMode( void )
{
	_XEncryption encription;
	encription.setKey( 10, 31, "9E2GH4FWDWKC2MBL4EBNKUV63T" );
	
	char serialstring[_REGSTRING_MAXLENGTH+1];
	memset( serialstring, 0, _REGSTRING_MAXLENGTH+1 );
	
	char xorencyperserial[_REGSTRING_MAXLENGTH+1];
	memset( xorencyperserial, 0, _REGSTRING_MAXLENGTH+1 );
	
	char licensefilecode[_REGSTRING_MAXLENGTH+1];    
	memset( licensefilecode, 0, _REGSTRING_MAXLENGTH+1 );
	
	char mixeddiskserialcode[_REGSTRING_MAXLENGTH+1];
	memset( mixeddiskserialcode, 0, _REGSTRING_MAXLENGTH+1 );
	
	char finalcode[_REGSTRING_MAXLENGTH+1];
	memset( finalcode, 0, _REGSTRING_MAXLENGTH+1 );
	
	CEnterSerialDlg enterserialdlg;

	if( enterserialdlg.DoModal() == IDOK )
	{
		CString sumSerialField;
		sumSerialField = enterserialdlg.m_SerialField1 + enterserialdlg.m_SerialField2 + 
			enterserialdlg.m_SerialField3 + enterserialdlg.m_SerialField4 + 
			enterserialdlg.m_SerialField5;

		char mbcsstring[1024];		
		memset( mbcsstring, 0, 1024 );
		WideCharToMultiByte(GetACP(), 0, sumSerialField, -1, mbcsstring, 1024, NULL, NULL);
		
		if( encription.checksumSerial( mbcsstring ) == TRUE )
		{
			g_RegManager._XRegWriteString(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_LICENSECODE, sumSerialField );
//			AfxMessageBox( sumSerialField );
			AfxMessageBox( _T("Inner test user certification complete") );
		}			
		else
		{
			CString tMessageString;
			tMessageString.Format( _T("[ %s - %s - %s - %s - %s ] : invalid serial number"),
				enterserialdlg.m_SerialField1, enterserialdlg.m_SerialField2, 
				enterserialdlg.m_SerialField3, enterserialdlg.m_SerialField4, 
				enterserialdlg.m_SerialField5 );
			
			AfxMessageBox( tMessageString );
		}
	}
	else
	{				
		AfxMessageBox( _T("Cancel certification") );
	}		

	m_ControlClickedCount = 0;
	m_ShiftClickedCount = 0;
}


void CXLauncherDlg::OnClosebutton() 
{
	OnCancel();
}

void CXLauncherDlg::ChangeTab( _XTABMODE tabmode )
{
	m_CurrentTabMode = tabmode;
		
	switch( m_CurrentTabMode ) 
	{
	case _XTABMODE_UPDATEMESSAGE	:
		//m_ctlUpdateText.ShowWindow( SW_SHOW );
		
#ifdef _XACCLAIM_NEWWEBPAGE			
		m_ServerMessageList.ShowWindow( SW_HIDE );
		m_HidePatchInfoButton.ShowWindow( SW_HIDE );
		m_ShowPatchInfoButton.ShowWindow( SW_HIDE );
		m_ctlUpdateText.ShowWindow( SW_HIDE );	
		m_UpdateTitleStatic.ShowWindow( SW_HIDE );
		m_UpdateTitleStatic.ShowWindow( SW_HIDE );
#else
		if(m_ViewUpdateInformationMode)
		{
			m_ServerMessageList.ShowWindow( SW_SHOW );
			m_HidePatchInfoButton.ShowWindow( SW_SHOW );
			m_ShowPatchInfoButton.ShowWindow( SW_HIDE );
			m_ctlUpdateText.ShowWindow( SW_HIDE );	
			m_UpdateTitleStatic.ShowWindow( SW_SHOW );
			m_UpdateTitleStatic.ShowWindow( SW_SHOW );
		}
		else
		{
			m_ServerMessageList.ShowWindow( SW_HIDE );
			m_HidePatchInfoButton.ShowWindow( SW_HIDE );
			m_ShowPatchInfoButton.ShowWindow( SW_SHOW );
			m_ctlUpdateText.ShowWindow( SW_SHOW );	
			m_UpdateTitleStatic.ShowWindow( SW_HIDE );
			m_UpdateTitleStatic.ShowWindow( SW_HIDE );
		}
#endif

		m_SoundOptionTitle.ShowWindow( SW_HIDE );
		m_GraphicOptionTitle.ShowWindow( SW_HIDE );
		m_OptionTitleStatic.ShowWindow( SW_HIDE );
		m_SoundSampleRateOptionStatic.ShowWindow( SW_HIDE );
		m_SoundDeviceOptionStatic.ShowWindow( SW_HIDE );
		m_SoundOutputOptionStatic.ShowWindow( SW_HIDE );
		m_CursorOptionStatic.ShowWindow( SW_HIDE );
		m_ShadowOptionStatic.ShowWindow( SW_HIDE );
		m_SightOptionStatic.ShowWindow( SW_HIDE );
		m_ModelOptionStatic.ShowWindow( SW_HIDE );
		m_TerrainOptionStatic.ShowWindow( SW_HIDE );
		m_DownloadMethodStatic.ShowWindow( SW_HIDE );
		
		m_TotalUpdateProgress.ShowWindow( SW_SHOW );
		m_DownloadProgress.ShowWindow( SW_SHOW );
		m_UpdateProgress.ShowWindow( SW_SHOW );
		m_ProgressRateStatic.ShowWindow( SW_SHOW );
		
		m_ServerListRefreshButton.ShowWindow( SW_HIDE );
						
		m_OptionOKButton.ShowWindow( SW_HIDE );
		m_OptionMiddleButton.ShowWindow( SW_HIDE );
		m_OptionLowButton.ShowWindow( SW_HIDE );
		m_OptionHighButton.ShowWindow( SW_HIDE );
		m_OptionCancelButton.ShowWindow( SW_HIDE );

		m_ChangeResolution.ShowWindow( SW_HIDE );
		m_UseHardWareCursorCombo.ShowWindow( SW_HIDE );
		m_ShadowQualityCombo.ShowWindow( SW_HIDE );
		m_ViewSightRangeCombo.ShowWindow( SW_HIDE );
		m_CharacterQualityComboBox.ShowWindow( SW_HIDE );
		m_TerrainQualityComboBox.ShowWindow( SW_HIDE );

		m_SoundMixRateCombo.ShowWindow( SW_HIDE );
		m_SoundDriverTypeCombo.ShowWindow( SW_HIDE );
		m_SoundOutputTypeCombo.ShowWindow( SW_HIDE );

		m_LanguageStatic.ShowWindow( SW_HIDE );
		m_LanguageComboBox.ShowWindow( SW_HIDE );

		m_AutoRunComboBox.ShowWindow( SW_HIDE );
		m_AutoRunStatic.ShowWindow( SW_HIDE );

		m_DownloadMethodCombo.ShowWindow( SW_HIDE );

#ifdef _XACCLAIM_NEWWEBPAGE			
		m_NoticeListBrowser.MoveWindow( 293, 51, 394, 315 );
		m_NoticeStatic.ShowWindow( SW_SHOW );
#endif

		KillTimer( _XDEF_SERVERTRAFFICGAUGETIMER );

		m_ViewServerList = FALSE;

		break;
	case _XTABMODE_OPTIONVIEW		:
		m_UseHardwareCursor			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_USEHARDWARECURSOR, 0, TRUE );	
		m_TextureQuality_Terrain	= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_TERRAIN, 2, TRUE );
		m_TextureQuality_Character	= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_CHARACTER, 2, TRUE );
		m_TextureQuality_Shadow		= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_CHARACTERSHADOW, 2, TRUE );	
		m_SightRange				= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SIGHTLEVEL_RANGE, 1, TRUE );
		
		m_SoundOutPutType			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SOUNDOUTPUTTYPE, 0, TRUE );
		m_SoundDriverType			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SOUNDDRIVERTYPE, 0, TRUE );
		m_SoundMixRate				= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SOUNDMIXRATE, 0, TRUE );

		//m_ViewCharacterShadow		= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_VIEWCHARACTERSHADOW, 1, TRUE );		
		
#ifdef _XACCLAIM
		m_SubLanguageIndex			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SUBLANGUAGE, 0, TRUE );
#endif

#ifdef _XNEXON_WEBLOGIN
		m_AutoRunIndex			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_AUTORUN, 0, TRUE );
#endif

		UpdateData( FALSE );

		m_SoundOptionTitle.ShowWindow( SW_SHOW	);
		m_GraphicOptionTitle.ShowWindow( SW_SHOW );
		m_OptionTitleStatic.ShowWindow( SW_SHOW );
		m_SoundSampleRateOptionStatic.ShowWindow( SW_SHOW );
		m_SoundDeviceOptionStatic.ShowWindow( SW_SHOW );
		m_SoundOutputOptionStatic.ShowWindow( SW_SHOW );

#ifdef _XVIETNAMESE
		m_CursorOptionStatic.ShowWindow( SW_SHOW );		
#else
		#ifdef _XPWC
			m_CursorOptionStatic.ShowWindow( SW_SHOW );		
		#else
			#ifdef _XACCLAIM
				m_CursorOptionStatic.ShowWindow( SW_SHOW );
			#else
				#ifdef _XKOREAN
					m_CursorOptionStatic.ShowWindow( SW_SHOW );
				#else
					#ifdef _XJAPANESE
						m_CursorOptionStatic.ShowWindow( SW_SHOW );						
					#else
						#ifdef _XRUSSIAN
							m_CursorOptionStatic.ShowWindow( SW_SHOW );						
						#else
							m_CursorOptionStatic.ShowWindow( SW_HIDE );
						#endif
					#endif
				#endif
			#endif
		#endif		
#endif
		m_ShadowOptionStatic.ShowWindow( SW_SHOW );
		m_SightOptionStatic.ShowWindow( SW_SHOW );
		m_ModelOptionStatic.ShowWindow( SW_SHOW );
		m_TerrainOptionStatic.ShowWindow( SW_SHOW );
		m_UpdateTitleStatic.ShowWindow( SW_SHOW );
		m_DownloadMethodStatic.ShowWindow( SW_SHOW );

		m_ServerListRefreshButton.ShowWindow( SW_HIDE );

		m_ServerMessageList.ShowWindow( SW_HIDE );
		m_HidePatchInfoButton.ShowWindow( SW_HIDE );
		m_ShowPatchInfoButton.ShowWindow( SW_HIDE );		
		m_ctlUpdateText.ShowWindow( SW_HIDE );	
		m_UpdateTitleStatic.ShowWindow( SW_HIDE );
		
		if( m_UpdateComplete )
		{
			m_TotalUpdateProgress.ShowWindow( SW_HIDE );
			m_DownloadProgress.ShowWindow( SW_HIDE );
			m_UpdateProgress.ShowWindow( SW_HIDE );
			m_ProgressRateStatic.ShowWindow( SW_HIDE );	
		}
		else
		{
			m_TotalUpdateProgress.ShowWindow( SW_SHOW );
			m_DownloadProgress.ShowWindow( SW_SHOW );
			m_UpdateProgress.ShowWindow( SW_SHOW );
			m_ProgressRateStatic.ShowWindow( SW_SHOW );
		}

		m_OptionOKButton.ShowWindow( SW_SHOW );
		m_OptionMiddleButton.ShowWindow( SW_SHOW );
		m_OptionLowButton.ShowWindow( SW_SHOW );
		m_OptionHighButton.ShowWindow( SW_SHOW );
		m_OptionCancelButton.ShowWindow( SW_SHOW );

#ifdef _XVIETNAMESE
	m_ChangeResolution.ShowWindow( SW_SHOW );
#else
	#ifdef _XPWC
		m_ChangeResolution.ShowWindow( SW_SHOW );
	#else
		#ifdef _XACCLAIM
			m_ChangeResolution.ShowWindow( SW_SHOW );
		#else
			#ifdef _XKOREAN
				m_ChangeResolution.ShowWindow( SW_SHOW );
			#else
				#ifdef _XJAPANESE
					m_ChangeResolution.ShowWindow( SW_SHOW );
				#else
					#ifdef _XRUSSIAN
						m_ChangeResolution.ShowWindow( SW_SHOW );
					#else
						m_ChangeResolution.ShowWindow( SW_HIDE );
					#endif
				#endif
			#endif
		#endif
	#endif
#endif
		//m_UseHardWareCursorCombo.ShowWindow( SW_SHOW );
		m_ShadowQualityCombo.ShowWindow( SW_SHOW );
		m_ViewSightRangeCombo.ShowWindow( SW_SHOW );
		m_CharacterQualityComboBox.ShowWindow( SW_SHOW );
		m_TerrainQualityComboBox.ShowWindow( SW_SHOW );

		m_SoundMixRateCombo.ShowWindow( SW_SHOW );
		m_SoundDriverTypeCombo.ShowWindow( SW_SHOW );
		m_SoundOutputTypeCombo.ShowWindow( SW_SHOW );

#ifdef _XACCLAIM
		m_LanguageStatic.ShowWindow( SW_SHOW );		
		m_LanguageComboBox.ShowWindow( SW_SHOW );
#endif

#ifdef _XNEXON_WEBLOGIN
		m_AutoRunComboBox.ShowWindow( SW_SHOW );
		m_AutoRunStatic.ShowWindow( SW_SHOW );
#endif

		m_DownloadMethodCombo.ShowWindow( SW_SHOW );

#ifdef _XACCLAIM_NEWWEBPAGE			
		m_NoticeListBrowser.MoveWindow( 1000, 1000, 184, 74 );
		m_NoticeStatic.ShowWindow( SW_HIDE );
#endif

		KillTimer( _XDEF_SERVERTRAFFICGAUGETIMER );

		m_ViewServerList = FALSE;

		break;	
	case _XTABMODE_SELECTSERVER		:
		if( m_ViewServerList )
		{
			m_ServerListRefreshButton.ShowWindow( SW_SHOW );
		}
		else
		{
			m_ServerListRefreshButton.ShowWindow( SW_HIDE );
		}

		m_SoundOptionTitle.ShowWindow( SW_HIDE );
		m_GraphicOptionTitle.ShowWindow( SW_HIDE );
		m_OptionTitleStatic.ShowWindow( SW_HIDE );
		m_SoundSampleRateOptionStatic.ShowWindow( SW_HIDE );
		m_SoundDeviceOptionStatic.ShowWindow( SW_HIDE );
		m_SoundOutputOptionStatic.ShowWindow( SW_HIDE );
		m_CursorOptionStatic.ShowWindow( SW_HIDE );
		m_ShadowOptionStatic.ShowWindow( SW_HIDE );
		m_SightOptionStatic.ShowWindow( SW_HIDE );
		m_ModelOptionStatic.ShowWindow( SW_HIDE );
		m_TerrainOptionStatic.ShowWindow( SW_HIDE );
		m_UpdateTitleStatic.ShowWindow( SW_HIDE );
		m_DownloadMethodStatic.ShowWindow( SW_HIDE );
		
		m_ServerMessageList.ShowWindow( SW_HIDE );
		m_HidePatchInfoButton.ShowWindow( SW_HIDE );
		m_ShowPatchInfoButton.ShowWindow( SW_HIDE );		
		m_ctlUpdateText.ShowWindow( SW_HIDE );		
		m_UpdateTitleStatic.ShowWindow( SW_HIDE );

		m_TotalUpdateProgress.ShowWindow( SW_HIDE );
		m_DownloadProgress.ShowWindow( SW_HIDE );
		m_UpdateProgress.ShowWindow( SW_HIDE );
		m_ProgressRateStatic.ShowWindow( SW_HIDE );

		m_OptionOKButton.ShowWindow( SW_HIDE );
		m_OptionMiddleButton.ShowWindow( SW_HIDE );
		m_OptionLowButton.ShowWindow( SW_HIDE );
		m_OptionHighButton.ShowWindow( SW_HIDE );
		m_OptionCancelButton.ShowWindow( SW_HIDE );

		m_ChangeResolution.ShowWindow( SW_HIDE );
		m_UseHardWareCursorCombo.ShowWindow( SW_HIDE );
		m_ShadowQualityCombo.ShowWindow( SW_HIDE );
		m_ViewSightRangeCombo.ShowWindow( SW_HIDE );
		m_CharacterQualityComboBox.ShowWindow( SW_HIDE );
		m_TerrainQualityComboBox.ShowWindow( SW_HIDE );

		m_SoundMixRateCombo.ShowWindow( SW_HIDE );
		m_SoundDriverTypeCombo.ShowWindow( SW_HIDE );
		m_SoundOutputTypeCombo.ShowWindow( SW_HIDE );

		m_LanguageStatic.ShowWindow( SW_HIDE );
		m_LanguageComboBox.ShowWindow( SW_HIDE );
		
		m_AutoRunComboBox.ShowWindow( SW_HIDE );
		m_AutoRunStatic.ShowWindow( SW_HIDE );

		m_DownloadMethodCombo.ShowWindow( SW_HIDE );

#ifdef _XACCLAIM_NEWWEBPAGE	
		m_NoticeListBrowser.MoveWindow( 293, 51, 394, 315 );
		m_NoticeStatic.ShowWindow( SW_SHOW );
#endif

		OnServerrefreshButton();
		
		break;
	}

	RedrawAllControls();
}

void CXLauncherDlg::OnNoticemorestatic() 
{
	m_NoticeDialog.ShowNoticePage( g_InterfaceString[46], _XWEBBROWSER_TITLE_NOTICE );	
}

void CXLauncherDlg::OnUpdatemorestatic() 
{
#ifndef _XTESTSERVER	
	m_NoticeDialog.ShowNoticePage( g_InterfaceString[48], _XWEBBROWSER_TITLE_UPDATE );
#else
	m_NoticeDialog.ShowNoticePage( g_InterfaceString[47], _XWEBBROWSER_TITLE_UPDATE );
#endif
}

void CXLauncherDlg::OnOptionHighButton() 
{
	m_ShadowQualityCombo.SetCurSel( 2 );
	m_ViewSightRangeCombo.SetCurSel( 2 );
	m_TerrainQualityComboBox.SetCurSel( 2 );
	m_CharacterQualityComboBox.SetCurSel( 2 );		

	UpdateData( TRUE );

	RedrawAllControls();
}

void CXLauncherDlg::OnOptionMiddleButton() 
{
	m_ShadowQualityCombo.SetCurSel( 1 );
	m_ViewSightRangeCombo.SetCurSel( 1 );
	m_TerrainQualityComboBox.SetCurSel( 1 );
	m_CharacterQualityComboBox.SetCurSel( 1 );

	UpdateData( TRUE );

	RedrawAllControls();
}

void CXLauncherDlg::OnOptionLowButton() 
{
	m_ShadowQualityCombo.SetCurSel( 0 );
	m_ViewSightRangeCombo.SetCurSel( 0 );
	m_TerrainQualityComboBox.SetCurSel( 0 );
	m_CharacterQualityComboBox.SetCurSel( 0 );

	UpdateData( TRUE );

	RedrawAllControls();
}

void CXLauncherDlg::OnOptionOkButton() 
{
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_USEHARDWARECURSOR, m_UseHardwareCursor );	
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_TERRAIN, m_TextureQuality_Terrain );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_WORLDOBJECT, m_TextureQuality_Terrain );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_CHARACTER, m_TextureQuality_Character );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_MONSTER, m_TextureQuality_Character );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_NPC, m_TextureQuality_Character );	

	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SOUNDOUTPUTTYPE, m_SoundOutPutType );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SOUNDDRIVERTYPE, m_SoundDriverType );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SOUNDMIXRATE, m_SoundMixRate );
	
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_CHARACTERSHADOW, m_TextureQuality_Shadow );	
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SIGHTLEVEL_RANGE, m_SightRange );

#ifdef _XACCLAIM
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SUBLANGUAGE, m_SubLanguageIndex );
#endif

#ifdef _XNEXON_WEBLOGIN
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_AUTORUN, m_AutoRunIndex );
#endif

	//g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_VIEWCHARACTERSHADOW, m_ViewCharacterShadow );		

	int prevDownloadMethod = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_DOWNLOADMETHOD, 0, TRUE );

	if( prevDownloadMethod != m_DownloadMethod )
	{
		AfxMessageBox( _T("Please restart this program") );
	}

	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_DOWNLOADMETHOD, m_DownloadMethod );
	
	if( m_UpdateComplete )
	{
		ChangeTab( _XTABMODE_SELECTSERVER );
	}
	else
	{
		ChangeTab( _XTABMODE_UPDATEMESSAGE );
	}
}

void CXLauncherDlg::OnOptionCancelButton() 
{
	if( m_UpdateComplete )
	{
		ChangeTab( _XTABMODE_SELECTSERVER );
	}
	else
	{
		ChangeTab( _XTABMODE_UPDATEMESSAGE );
	}
}

void CXLauncherDlg::OnShowDetailPatchInfoButton()
{
	m_ViewUpdateInformationMode = TRUE;
	m_ServerMessageList.ShowWindow( SW_SHOW );
	m_HidePatchInfoButton.ShowWindow( SW_SHOW );
	m_ShowPatchInfoButton.ShowWindow( SW_HIDE );
	m_ctlUpdateText.ShowWindow( SW_HIDE );	
	m_UpdateTitleStatic.ShowWindow( SW_SHOW );
	RedrawAllControls();
}

void CXLauncherDlg::OnHideDetailPatchInfoButton()
{
	m_ViewUpdateInformationMode = FALSE;
	m_ServerMessageList.ShowWindow( SW_HIDE );
	m_HidePatchInfoButton.ShowWindow( SW_HIDE );
	m_ShowPatchInfoButton.ShowWindow( SW_SHOW );
	m_ctlUpdateText.ShowWindow( SW_SHOW );
	m_UpdateTitleStatic.ShowWindow( SW_HIDE );
	RedrawAllControls();
}


void CXLauncherDlg::OnServerrefreshButton()
{
	KillTimer( _XDEF_SERVERTRAFFICGAUGETIMER );

	ConnectVersionServer( FALSE );

#ifdef _XDEF_SERVERTRAFFICGAUGETEST
	for( int serverindex = 0; serverindex < m_MaxServerCount; serverindex++ )
	{
		// test...
		m_NextServerTrafficFactor[serverindex] = rand() % 121;
		
		if( m_NextServerTrafficFactor[serverindex] > 100 ) 
			m_NextServerTrafficFactor[serverindex] = 100;
	}
#endif

	//SetTimer( _XDEF_SERVERTRAFFICGAUGETIMER, 1, NULL );

	RebuildServerTrafficGraph();
}

HBRUSH CXLauncherDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// Make static controls transparent
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		// Make sure that it's not a slider control
		TCHAR lpszClassName[256];
		GetClassName (pWnd->m_hWnd, lpszClassName, 255);
		if (wcscmp (lpszClassName, TRACKBAR_CLASS) == 0)
			return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		
		pDC->SetBkMode (TRANSPARENT);
		return m_brushHollow;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CXLauncherDlg::OnSelendokTerrainqualitycombo() 
{
	UpdateData();
}

void CXLauncherDlg::OnSelendokCharacterqualitycombo() 
{
	UpdateData();
}

void CXLauncherDlg::OnSelendokViewsightcombo() 
{
	UpdateData();
}

void CXLauncherDlg::OnSelendokShadowqualitycombo() 
{
	UpdateData();
}

void CXLauncherDlg::OnSelendokUsehardwarecursorcombo() 
{
	UpdateData();
}

void CXLauncherDlg::RebuildServerTrafficGraph( void )
{
	CDC* pDC = GetDC();
	if( pDC )
	{	
		for( int serverindex = 0; serverindex < m_MaxServerCount; serverindex++ )
		{
			if( m_NextServerTrafficFactor[serverindex] < 0 )
			{
				CString messagestring;
				if( m_NextServerTrafficFactor[serverindex] == _FOREST_STATE_STOP )
				{
					m_ServerNameImage[serverindex].CreateFromText( pDC, g_szServerNameString[serverindex], RGB(255,255,255), 184, g_InterfaceString[7], 92 );//_T("Maintenance")
					m_ServerNameImage[serverindex].HLine( 0, 6, 80, RGB(64,64,255) );
				}
				else //if( m_NextServerTrafficFactor[serverindex] == _FOREST_STATE_OFF )
				{
					m_ServerNameImage[serverindex].CreateFromText( pDC, g_szServerNameString[serverindex], RGB(255,255,255), 184, g_InterfaceString[8], 92 );//Server stopped.
					m_ServerNameImage[serverindex].HLine( 0, 6, 80, RGB(64,64,255) );
				}
			}
			else
			{
				m_ServerNameImage[serverindex].CreateFromText( pDC, g_szServerNameString[serverindex], RGB(255,255,255), 184 );
				
				int trafficfactor = (int)( (FLOAT)_XDEF_SERVERTRAFFICGAUGEMAXSIZE * ((FLOAT)m_ServerTrafficFactor[serverindex] / 100.0f) );
				
				if( trafficfactor > 0 )
				{
					int r, g, b;
					
					if( m_ServerTrafficFactor[serverindex] >= 100 || m_NextServerTrafficFactor[serverindex] >= 100 )
					{
						//trafficfactor = _XDEF_SERVERTRAFFICGAUGEMAXSIZE;
						r = 255;
						g = 0;
						b = 0;
					}
					else
					{
						//int factor = (int)(255.0f * ((FLOAT)trafficfactor / 80.0f));
						int factor = 255;
						r = factor;
						g = factor;
						b = factor;
					}				
					
					m_ServerNameImage[serverindex].FillRect( 92, 6, trafficfactor, 7, 16, 16, 16 );
					m_ServerNameImage[serverindex].FillRect( 90, 4, trafficfactor, 7, r, g, b );
					
					if( m_ServerTrafficFactor[serverindex] >= 100 )
					{
						m_ServerNameImage[serverindex].PasteCKRect( &m_ServerFullImage, 90 + 32, 4, 0x00000000 );
					}
				}
			}
		}
		
		ReleaseDC( pDC );
	}

	SelectServer( m_SelectedServerIndex );

	RedrawAllControls();
}

void CXLauncherDlg::SelectServer( int index )
{
	if( index < 0 ) return;

	if( index >= m_MaxServerCount || index > _COUNT_MAX_GROUP_ ) 
		index = 0;

	if( m_NextServerTrafficFactor[index] < 0 )
	{
		return;
	}


	int trafficfactor = (int)( (FLOAT)_XDEF_SERVERTRAFFICGAUGEMAXSIZE * ((FLOAT)m_ServerTrafficFactor[m_SelectedServerIndex] / 100.0f) );
		
	if( trafficfactor < _XDEF_SERVERTRAFFICGAUGEMAXSIZE && m_NextServerTrafficFactor[m_SelectedServerIndex] < 100 )
	{
		m_ServerNameImage[m_SelectedServerIndex].FillRect( 92, 6, trafficfactor, 7, 16, 16, 16 );
		m_ServerNameImage[m_SelectedServerIndex].FillRect( 90, 4, trafficfactor, 7, 255, 255, 255 );				
	}
	else
	{
		m_ServerNameImage[m_SelectedServerIndex].FillRect( 92, 6, trafficfactor, 7, 16, 16, 16 );
		m_ServerNameImage[m_SelectedServerIndex].FillRect( 90, 4, trafficfactor, 7, 255, 0, 0 );	
		
		if( m_ServerTrafficFactor[m_SelectedServerIndex] >= 100 )
			m_ServerNameImage[m_SelectedServerIndex].PasteCKRect( &m_ServerFullImage, 90 + 32, 4, 0x00000000 );		
	}
		

	m_ServerNameImage[ m_SelectedServerIndex ].ClippedLine( 1, 0, m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cx-1, 0, 0,0,0);
	m_ServerNameImage[ m_SelectedServerIndex ].ClippedLine( 1, m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cy-1, 
															m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cx-1, 
															m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cy-1,
															0,0,0);
	
	m_ServerNameImage[ m_SelectedServerIndex ].ClippedLine( 0, 1, 0, m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cy-1, 0,0,0);
	
	m_ServerNameImage[ m_SelectedServerIndex ].ClippedLine( m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cx-1, 
		1,
		m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cx-1,
		m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cy-1, 0,0,0);


	//////////////////////////////////////////////////////////////////////////
	// Select new server
	m_SelectedServerIndex = index;
	
	m_ServerNameImage[ m_SelectedServerIndex ].ClippedLine( 1, 0, m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cx-1, 0, 220,220,220 );
	m_ServerNameImage[ m_SelectedServerIndex ].ClippedLine( 1, m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cy-1, 
		                                                    m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cx-1, 
															m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cy-1,
															220,220,220);

	m_ServerNameImage[ m_SelectedServerIndex ].ClippedLine( 0, 1, 0, m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cy-1, 220,220,220 );

	m_ServerNameImage[ m_SelectedServerIndex ].ClippedLine( m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cx-1, 
															1,
															m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cx-1,
															m_ServerNameImage[ m_SelectedServerIndex ].m_Size.cy-1, 220,220,220 );

	trafficfactor = (int)( (FLOAT)_XDEF_SERVERTRAFFICGAUGEMAXSIZE * ((FLOAT)m_ServerTrafficFactor[m_SelectedServerIndex] / 100.0f) );
	if( trafficfactor < _XDEF_SERVERTRAFFICGAUGEMAXSIZE && m_NextServerTrafficFactor[m_SelectedServerIndex] < 100 )
	{
		m_ServerNameImage[m_SelectedServerIndex].FillRect( 92, 6, trafficfactor, 7, 16, 16, 16 );
		m_ServerNameImage[m_SelectedServerIndex].FillRect( 90, 4, trafficfactor, 7, 255, 193, 0 );
	}
	else
	{
		m_ServerNameImage[m_SelectedServerIndex].FillRect( 92, 6, trafficfactor, 7, 16, 16, 16 );
		m_ServerNameImage[m_SelectedServerIndex].FillRect( 90, 4, trafficfactor, 7, 255, 0, 0 );
		
		if( m_ServerTrafficFactor[m_SelectedServerIndex] >= 100 )
			m_ServerNameImage[m_SelectedServerIndex].PasteCKRect( &m_ServerFullImage, 90 + 32, 4, 0x00000000 );
	}
	
	CDC* pDC = GetDC();
	if( pDC )
	{
		CString messagetext;

		messagetext.Format( _T("Selected %s server."), g_szServerNameString[m_SelectedServerIndex]+4 );			
		
		m_SelectedServerNameImage.CreateFromText( pDC, messagetext, RGB(255, 193, 0) );
		
		if( m_UpdateComplete )
		{
			CString messagestring;
			
			g_PatchVersionManager.GetVersionInfo();
			//int PatchNumber = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHNUMBER, 0, TRUE);
			int PatchNumber = g_PatchVersionManager.m_VersionInfo.patchversion_1;

			/*
			if( m_TestServerConnectMode )
			{
				messagestring.Format( _T("최신버젼(%d)으로 업데이트 되어 있습니다. (테스트 : %s)"), PatchNumber,g_szServerNameString[m_SelectedServerIndex]+4 );
			}
			else
			{
				messagestring.Format( _T("최신버젼(%d)으로 업데이트 되어 있습니다. (%s)"), PatchNumber,g_szServerNameString[m_SelectedServerIndex]+4 );
			}
			*/
#ifdef _XTESTSERVER
			messagestring.Format( g_InterfaceString[9], PatchNumber );
			messagestring += g_InterfaceString[1];
#else
			messagestring.Format( g_InterfaceString[9], PatchNumber );
#endif

			m_UpdatedMessageImage.CreateFromText( pDC, messagestring.GetBuffer(256), RGB( 255, 193, 0 ), 300 );		
			ReleaseDC( pDC );
		}
	}

	RedrawAllControls();
}

int g_PatchStart = FALSE;

void CXLauncherDlg::OnTimer(UINT nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);

	if( _PATCH_STARTTIMERID == nIDEvent )
	{
		if( !g_PatchStart )
		{	
			g_PatchStart = TRUE;
			StartPatch();
			KillTimer( _PATCH_STARTTIMERID );
		}
	}
	else if( _XDEF_SERVERTRAFFICGAUGETIMER == nIDEvent )
	{
		BOOL timerend = FALSE;
		for( int serverindex = 0; serverindex < m_MaxServerCount; serverindex++ )
		{
			if( m_ServerTrafficFactor[serverindex] != m_NextServerTrafficFactor[serverindex] )
			{
				if( m_ServerTrafficFactor[serverindex] < m_NextServerTrafficFactor[serverindex] )
				{
					m_ServerTrafficFactor[serverindex]+=10;	
				
					if( m_ServerTrafficFactor[serverindex] > m_NextServerTrafficFactor[serverindex] )
					{
						m_ServerTrafficFactor[serverindex] = m_NextServerTrafficFactor[serverindex];
					}
				}
				else if( m_ServerTrafficFactor[serverindex] > m_NextServerTrafficFactor[serverindex] )
				{
					m_ServerTrafficFactor[serverindex]-=10;
					
					if( m_ServerTrafficFactor[serverindex] < m_NextServerTrafficFactor[serverindex] )
					{
						m_ServerTrafficFactor[serverindex] = m_NextServerTrafficFactor[serverindex];
					}

					if( m_ServerTrafficFactor[serverindex] < 0 )
					{
						m_ServerTrafficFactor[serverindex] = 0;
					}
				}

				timerend = TRUE;
			}
		}

		RebuildServerTrafficGraph();

		// tab view
		CRect rect;
		rect.left = 304;
		rect.top = 69;	
		rect.right = 486;
		rect.bottom = 247;
		RedrawWindow( &rect );

		rect.left = 496;
		rect.top = 69;	
		rect.right = 679;
		rect.bottom = 177;
		RedrawWindow( &rect );

		RedrawWindow( &rect );

		if( !timerend )
		{
			KillTimer( _XDEF_SERVERTRAFFICGAUGETIMER );
		}		
	}
}

BEGIN_EVENTSINK_MAP(CXLauncherDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CXLauncherDlg)
	ON_EVENT(CXLauncherDlg, IDC_NOTICEEXPLORER, DISPID_BEFORENAVIGATE2/* BeforeNavigate2 */, OnBeforeNavigate2Noticeexplorer, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CXLauncherDlg, IDC_UPDATEEXPLORER, DISPID_BEFORENAVIGATE2/* BeforeNavigate2 */, OnBeforeNavigate2Updateexplorer, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	//ON_EVENT(CXLauncherDlg, IDC_NOTICEEXPLORER, DISPID_DOCUMENTCOMPLETE,OnDocumentCompleteNoticeexplorer, VTS_DISPATCH VTS_PVARIANT)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/*
void CXLauncherDlg::OnDocumentCompleteNoticeexplorer( LPDISPATCH pDisp, VARIANT FAR* URL )
{
	IDispatch *pDisp2 = m_NoticeListBrowser.GetApplication();

    CString strOption = _T("no");   // yes no auto    

    if (pDisp2 != NULL)
    {
        IHTMLDocument2* pHTMLDocument2;
        HRESULT hr;
        hr = pDisp2->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDocument2);

        if (hr == S_OK)
        {
            IHTMLElement *pIElement;
            hr = pHTMLDocument2->get_body(&pIElement);
            IHTMLBodyElement *pIBodyElement;
            hr = pIElement->QueryInterface(IID_IHTMLBodyElement, (void**)&pIBodyElement);           

            BSTR bstr;
            bstr = strOption.AllocSysString();
            pIBodyElement->put_scroll(bstr);
        }
        pDisp2->Release ();
    } 
}
*/

#define Unused(parameter) parameter					// avoid compile warnings

void CXLauncherDlg::OnBeforeNavigate2Noticeexplorer(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel) 
{
	_XLog( FALSE, _T("Before bavigate notice page\n") );

	Unused(Flags);	// Note: flags value is reserved
	
    if (pDisp == m_NoticeListBrowser.GetApplication() ) {
		
		CString				URL_string;
		CString				frame;
		unsigned char		*post_data		= NULL;
		int					post_data_size	= 0;
		CString				headers;
		
        if ((URL       != NULL) && 
			(V_VT(URL) == VT_BSTR)) {
            URL_string = V_BSTR(URL);
        }
		
		if ((TargetFrameName       != NULL) &&
            (V_VT(TargetFrameName) == VT_BSTR)) {
			frame = V_BSTR(TargetFrameName);
        }
		
		if ((PostData       != NULL)                    &&
			(V_VT(PostData) == (VT_VARIANT | VT_BYREF))) {
			
			VARIANT *PostData_variant = V_VARIANTREF(PostData);
			
			if ((PostData_variant       != NULL) &&
				(V_VT(PostData_variant) != VT_EMPTY)) {
				
				SAFEARRAY *PostData_safearray = V_ARRAY(PostData_variant);
				
				if (PostData_safearray != NULL) {
					
					char *post_data_array = NULL;
					
					SafeArrayAccessData(PostData_safearray,(void HUGEP **)&post_data_array);
					
					long		lower_bound = 1;
					long		upper_bound = 1;
					
					SafeArrayGetLBound(PostData_safearray,1,&lower_bound);
					SafeArrayGetUBound(PostData_safearray,1,&upper_bound);
					
					post_data_size = (int)(upper_bound - lower_bound + 1);
					
					post_data = new unsigned char[post_data_size];
					
					memcpy(post_data,post_data_array,post_data_size);
					
					SafeArrayUnaccessData(PostData_safearray);
					
				}
				
			}
			
		}
		
		bool cancel = _OnBeforeNavigate2Noticeexplorer(URL_string, frame, post_data,post_data_size, headers);
		
		if (Cancel != NULL) {
			if (cancel) *Cancel = VARIANT_TRUE;
			else        *Cancel = VARIANT_FALSE;
		}
		
		delete []post_data;
		
    }		
}

bool CXLauncherDlg::_OnBeforeNavigate2Noticeexplorer(CString URL,CString	frame,void *post_data,int post_data_size, CString headers)
{
#ifdef _XACCLAIM_NEWWEBPAGE	
	
#else
	if( g_NoticeWebURL.CompareNoCase( URL ) != 0 && URL.CompareNoCase( _T("about:blank") ) != 0 )
	{
		if( ((CXLauncherDlg*)AfxGetMainWnd()) )
			((CXLauncherDlg*)AfxGetMainWnd())->m_NoticeDialog.ShowNoticePage( URL.GetBuffer(512), _XWEBBROWSER_TITLE_NOTICE );
		return true;
	}	
#endif
	return false;
}

void CXLauncherDlg::OnBeforeNavigate2Updateexplorer(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel) 
{
	_XLog( FALSE, _T("Before bavigate update list page\n") );

	Unused(Flags);	// Note: flags value is reserved
	
    if (pDisp == m_UpdateListBrowser.GetApplication() ) {
		
		CString				URL_string;
		CString				frame;
		unsigned char		*post_data		= NULL;
		int					post_data_size	= 0;
		CString				headers;
		
        if ((URL       != NULL) && 
			(V_VT(URL) == VT_BSTR)) {
            URL_string = V_BSTR(URL);
        }
		
		if ((TargetFrameName       != NULL) &&
            (V_VT(TargetFrameName) == VT_BSTR)) {
			frame = V_BSTR(TargetFrameName);
        }
		
		if ((PostData       != NULL)                    &&
			(V_VT(PostData) == (VT_VARIANT | VT_BYREF))) {
			
			VARIANT *PostData_variant = V_VARIANTREF(PostData);
			
			if ((PostData_variant       != NULL) &&
				(V_VT(PostData_variant) != VT_EMPTY)) {
				
				SAFEARRAY *PostData_safearray = V_ARRAY(PostData_variant);
				
				if (PostData_safearray != NULL) {
					
					char *post_data_array = NULL;
					
					SafeArrayAccessData(PostData_safearray,(void HUGEP **)&post_data_array);
					
					long		lower_bound = 1;
					long		upper_bound = 1;
					
					SafeArrayGetLBound(PostData_safearray,1,&lower_bound);
					SafeArrayGetUBound(PostData_safearray,1,&upper_bound);
					
					post_data_size = (int)(upper_bound - lower_bound + 1);
					
					post_data = new unsigned char[post_data_size];
					
					memcpy(post_data,post_data_array,post_data_size);
					
					SafeArrayUnaccessData(PostData_safearray);
					
				}
				
			}
			
		}
		
		bool cancel = _OnBeforeNavigate2Updateexplorer(URL_string, frame, post_data,post_data_size, headers);
		
		if (Cancel != NULL) {
			if (cancel) *Cancel = VARIANT_TRUE;
			else        *Cancel = VARIANT_FALSE;
		}
		
		delete []post_data;
		
    }
}

bool CXLauncherDlg::_OnBeforeNavigate2Updateexplorer(CString URL,CString	frame,void *post_data,int post_data_size, CString headers)
{
	if( g_UpdateWebURL.CompareNoCase( URL ) != 0 && URL.CompareNoCase( _T("about:blank") ) != 0 )
	{
		if( ((CXLauncherDlg*)AfxGetMainWnd()) )
			((CXLauncherDlg*)AfxGetMainWnd())->m_NoticeDialog.ShowNoticePage( URL.GetBuffer(512), _XWEBBROWSER_TITLE_UPDATE );
		return true;
	}	
	return false;
}


/*
int CXLauncherDlg::ExecShellProc(CString strParam)
{
    CWaitCursor wait;
	
    if(strParam.IsEmpty())
        return FALSE;
    char str[512];
    wsprintf(str, "Ninedragons.exe %s", strParam);
	
    int ret;
    FILE *fp;
    DWORD pstatus;
    STARTUPINFO si;
    CString szMsg, szTmp;
    PROCESS_INFORMATION pi;
    char cmdstr[300];
	
    memset(cmdstr, 0, 300);
    wsprintf(cmdstr,"%s", str);
	
    
    memset(&si, 0, sizeof(STARTUPINFO));
    si.lpTitle = NULL;
    si.cb = sizeof(STARTUPINFO);
    si.lpReserved = NULL;
    si.lpDesktop = NULL;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.cbReserved2 = 0;
    si.lpReserved2 = NULL;
    si.wShowWindow = SW_SHOW;
	
	
    ret = CreateProcess(NULL,cmdstr,NULL,NULL,0,0,NULL,".\\",&si,&pi);
    if(ret == 0)
        return FALSE;
    while(TRUE)
    {
        Sleep(1500);
        GetExitCodeProcess(pi.hProcess,&pstatus);
        if(pstatus != STILL_ACTIVE)
            break;
    }
    return TRUE;
}
*/

void CXLauncherDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(HIBYTE(GetAsyncKeyState(VK_LCONTROL)))
	{
		m_ControlClickedCount++;
	}
	
	if(HIBYTE(GetAsyncKeyState(VK_LSHIFT)))
	{
		if( m_ControlClickedCount >= 5 )
		{
			m_ShiftClickedCount++;
			
			if( m_ShiftClickedCount == 7 )
			{
				SetTestServerMode();
			}
		}
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CXLauncherDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

#ifdef _DEBUG
/*
#include "XEncryption.h"
#include "XEncryptiondata.h"

void CreateSerial( void )
{
	// Randomize...
	srand( (unsigned)time( NULL ) );

	FILE* pFile = fopen( "serial.txt", "wt" );

	for( int i = 0; i < 100; i++ )
	{
		char strSerialData[64] = "";
		char strSerialMergedtData[64] = "A2FCFX3DQ4ISC1J9R3X2E";
		char strEncryptKeyData[64] = "9E2GH4FWDWKC2MBL4EBNKUV63T";
		char strSerialIndexString[7];
		
		_XEncryption encrypt_factory;
		char encryptserial[_ENCRYPTION_FIELD_LENGTH+1];
		
		// Encrypt key setting ///////////////////////////////////////////////////////////////////////////////////////////
		encrypt_factory.setKey( 10, 31, strEncryptKeyData );
		
		memset( strSerialIndexString, 0, 7 );
		memcpy( strSerialIndexString, "9R3X2E", 6 );
		
		// Random index 를 만들기 위한 테이블 준비..
		int  randomStartRange[16];
		char randomindexstring[17];
		memset( randomindexstring, 0, 17 );
		memcpy( randomindexstring, strSerialMergedtData, 16 );
		
		for( int i = 0; i < 16; i++ )
		{
			randomStartRange[i] = _SERIALFIELD_ASCIILUT[ randomindexstring[ i ] ];
		}
		
		// generate serial number loop ///////////////////////////////////////////////////////////////////////////////////
		char strEncryptString[128];
		char strDecryptString[128];	
		
		// create random serial number -------------------------------------------------------------------------------		
		memset( randomindexstring, 0, 17 );
		
		int randomvalue;
		for( i = 0; i < 16; i++ )
		{
			randomvalue = (35 - randomStartRange[i]);
			if( randomvalue ) randomvalue = rand() % randomvalue;
			else randomvalue = 35;
			
			strcpy( randomindexstring[i], _SERIALFIELD_CHARACTERDATA[randomvalue] );
		}
		
		// joint serial field ----------------------------------------------------------------------------------------
		//strSerialData = strSerialMergedtData;		
		strSerialData = randomindexstring;
		strcat( strSerialData, strSerialIndexString );
		
		
		// serial number encryption ----------------------------------------------------------------------------------
		encrypt_factory.enciperSerial( strSerialData, encryptserial );
		char serialfield[5][7];
		memset( serialfield, 0, sizeof(char) * 5 * 7 );
		
		for( i = 0; i < 5; i++ )
		{
			memcpy( serialfield[i], encryptserial+ (i*5), (i == 4) ? 6 : 5 );
		}			
		
		char newSerialNumber[64];
		sprintf( newSerialNumber, "%s - %s - %s - %s - %s", serialfield[0], serialfield[1], serialfield[2], serialfield[3], serialfield[4] );

		fprintf( pFile, "\n%s\t\t\n", newSerialNumber );
	}	

	fclose( pFile );
}
*/
#endif

void CXLauncherDlg::OnSelendokSoundoutputtypecombo() 
{
	UpdateData();	

	switch (m_SoundOutPutType)
    {
	case 0 :  FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
		break;
	case 1 :  FSOUND_SetOutput(FSOUND_OUTPUT_WINMM);
		break;
	case 2 :  FSOUND_SetOutput(FSOUND_OUTPUT_ASIO);
		break;
	default:
	case 3 :  FSOUND_SetOutput(FSOUND_OUTPUT_NOSOUND);
		break;
    }

	m_SoundDriverTypeCombo.ResetContent();
	
	TCHAR widestring[1024];
	{
		for (int i=0; i < FSOUND_GetNumDrivers(); i++) 
		{
			int strlength = strlen( FSOUND_GetDriverName(i) );
			memset( widestring, 0, sizeof(TCHAR) * 1024 );
			int requirelength = MultiByteToWideChar(CP_ACP, 0, FSOUND_GetDriverName(i), strlength, widestring, 1024);

			m_SoundDriverTypeCombo.AddString( widestring );
		}
	}

	if( FSOUND_GetNumDrivers() <= 0 )
	{
		TCHAR messagebuffer[512];
		wsprintf( messagebuffer, g_InterfaceString[10] );
		AfxMessageBox( messagebuffer, MB_OK );
		_XLog( FALSE, messagebuffer );
		InsertStringToServerMessageListCtrl( messagebuffer );
	}
	else
	{
		m_SoundDriverType = 0;
		m_SoundDriverTypeCombo.SetCurSel( m_SoundDriverType );
	}
}

void CXLauncherDlg::OnSelendokSounddrivercombo() 
{
	UpdateData();	
}

void CXLauncherDlg::OnSelendokMixratecombo() 
{
	UpdateData();	
}


void CXLauncherDlg::LoadString( void )
{
#ifdef _XVIETNAMESE
	HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_VIETNAMESEINTERFACETEXT2),_T("Text"));
#endif

#ifdef _XTAIWANESE
	HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_TAIWANESEINTERFACETEXT),_T("Text"));
#endif
	
#ifdef _XACCLAIM
	HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_ENGLISHINTERFACETEXT),_T("Text"));
#endif

#ifdef _XPWC
	HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_PWCINTERFACETEXT),_T("Text"));
#endif
	
#ifdef _XKOREAN
	HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_NEXONINTERFACETEXT),_T("Text"));
#endif

#ifdef _XJAPANESE
	HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_JAPANESEINTERFACETEXT),_T("Text"));
#endif

#ifdef _XRUSSIAN
	HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_RUSSIANINTERFACETEXT),_T("Text"));
#endif
	
	HGLOBAL hResData = LoadResource(AfxGetResourceHandle(),hResLoad);
	LPCTSTR pdata = (LPCTSTR)LockResource(hResData);
	ASSERT(pdata != NULL && hResData != NULL);	
	DWORD sizeofresource = SizeofResource(AfxGetResourceHandle(),hResLoad);	
	
	pdata++; // skip unicode indicator
	
	int copystpos = 0;
	TCHAR tempbuffer[1024];
	int resourcelength = wcslen( pdata );
	
	for( DWORD ichar = 0; ichar < (DWORD)resourcelength-1; ichar++ )
	{
		if( pdata[ichar] == _T('\0') )
		{
			break;
		}

		if( pdata[ichar] == _T('X') && pdata[ichar+1] == _T('E') && pdata[ichar+2] == _T('O') && pdata[ichar+3] == _T('L') )
		{
			break;
		}
		
		if( (pdata[ichar] == _T('\r') && pdata[ichar+1] == _T('\n') ) )
		{
			memset( tempbuffer, 0, sizeof(TCHAR) * 1024 );
			wcsncpy( tempbuffer, pdata+copystpos, (ichar - copystpos) );
			
			CString szStr = tempbuffer;
			szStr.Replace( _T('\r'), _T('') );
			szStr.Replace( _T('\n'), _T('') );			
			
			int resid = -1;
			TCHAR stringbuffer[1024];
			memset( stringbuffer, 0, sizeof(TCHAR) * 1024 );
			swscanf( szStr.GetBuffer(1024), _T("%d %s"), &resid, stringbuffer );
			
			if( resid >= 0 && resid < 100 )
			{
				memset( g_InterfaceString[resid], 0, sizeof(TCHAR) * 1024 );
				
				int cppos = szStr.Find( stringbuffer, 0 );
				
				LPTSTR pStr = szStr.GetBuffer(1024) + cppos;
				
				//wcscpy( g_InterfaceString[resid], pStr );
				
				int _len = wcslen( pStr );
				int insertpos = 0;
				for( int i = 0; i < _len; i++ )
				{
					if( *pStr == _T('$') )
					{
						if( *(pStr+1) == _T(';') )
						{
							g_InterfaceString[resid][insertpos] = _T('\n');
							insertpos++;
							pStr+=2;
						}
						else
						{
							
						}
					}
					else
					{
						g_InterfaceString[resid][insertpos] = *pStr;
						insertpos++;
						pStr++;
					}
				}
				
				//OutputDebugString( g_InterfaceString[resid] );
				//OutputDebugString( _T("\n") );
			}		
			
			copystpos = ichar+2;			
			ichar++; 
		}
	}
	
	FreeResource(hResData);	
}

void CXLauncherDlg::OnSelendokDownloadmethodcombo() 
{
	UpdateData( TRUE );
}

void CXLauncherDlg::OnChangeresolution() 
{
	CResChangeDialog	reschangedialog;
	
	reschangedialog.LoadBitmap (IDB_RESDIALOGBITMAP);
	reschangedialog.SetStaticTransparent (TRUE);
	reschangedialog.SetClickAnywhereMove (TRUE);

	reschangedialog.m_regfullscreenmode	= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SCREENMODE, 0, TRUE );
	reschangedialog.m_screenwidth		= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SCREENWIDTH, 1024, TRUE );
	reschangedialog.m_screenheight		= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SCREENHEIGHT, 768, TRUE );
	reschangedialog.m_screenrefreshrate	= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SCREENREFRESHRATE, 60, TRUE );

	if( reschangedialog.DoModal() == IDOK )
	{
		
	}
}

void CXLauncherDlg::OnSelendokLanguagecombo() 
{
	UpdateData();
}

void CXLauncherDlg::CreateDeskTopShortCut( void )
{
	LPITEMIDLIST pidl;	
	TCHAR szPath[_MAX_PATH];

	HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl);   		 

	if( SUCCEEDED(hr) )
	{
		if( SHGetPathFromIDList(pidl, szPath) )
		{
			LPMALLOC pMalloc;   	
			hr = SHGetMalloc(&pMalloc);   
			pMalloc->Free(pidl);
			pMalloc->Release();
			
			CString szfilename = _T("");
			szfilename = szPath;
			szfilename += _T("\\구룡쟁패.url");

			CString szoldfilename = _T("");
			szoldfilename = szPath;
			szoldfilename += _T("\\구룡쟁패.lnk");

			CString szoldurlfilename = _T("");
			szoldurlfilename = szPath;
			szoldurlfilename += _T("\\구룡쟁패 홈페이지.lnk");

			if( CheckExistFile(szoldfilename,TRUE) )
			{
				::DeleteFile( szoldfilename );		
			}

			if( CheckExistFile(szoldurlfilename,TRUE) )
			{
				::DeleteFile( szoldurlfilename );		
			}

			if( !CheckExistFile(szfilename,FALSE) )
			{
				FILE* pFile = NULL;

				pFile = _wfopen( szfilename, _T("wt") );

				if( pFile )
				{
					char textbuffer[1024];
					memset( textbuffer, 0, sizeof(char) * 1024 );

					char mbcspathname[MAX_PATH];
					memset( textbuffer, 0, sizeof(char) * MAX_PATH );
					WideCharToMultiByte( CP_ACP, 0, g_Installedpath, -1, mbcspathname,MAX_PATH,0,0 );
					
					strcat( textbuffer, "[DEFAULT]\r\n" );
					//strcat( textbuffer, "BASEURL=http://work.platform.nexon.com/NxTest/NineDragons.htm\r\n" );
					strcat( textbuffer, "BASEURL=http://nine.nexon.com\r\n" );
					strcat( textbuffer, "[InternetShortcut]\r\n" );
					//strcat( textbuffer, "URL=http://work.platform.nexon.com/NxTest/NineDragons.htm\r\n" );
					strcat( textbuffer, "URL=http://nine.nexon.com\r\n" );
					strcat( textbuffer, "IDList=\r\n" );
					strcat( textbuffer, "IconFile=" );
					strcat( textbuffer,  mbcspathname );
					strcat( textbuffer, "\\NineDragons.ico" );
					strcat( textbuffer, "\r\n" );
					strcat( textbuffer, "HotKey=0\r\n" );
					strcat( textbuffer, "IconIndex=0\r\n" );
					int size = strlen(textbuffer);

					fwrite( textbuffer, sizeof(char) * size, 1, pFile );

					fclose( pFile );
					_XLog( FALSE, _T("Created url link file.") );
				}
				else
				{
					_XLog( FALSE, _T("Can't create url link file.") );
				}
			}
		}
	}
/*
	hr = SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &pidl);
	if( SUCCEEDED(hr) )
	{
		if( SHGetPathFromIDList(pidl, szPath) )
		{	
			LPMALLOC pMalloc;   	
			hr = SHGetMalloc(&pMalloc);   
			pMalloc->Free(pidl);
			pMalloc->Release();
			
			CString programfolder = szPath;			
			programfolder += _T("\\Nexon\\구룡쟁패");

			CString linkfilename = programfolder;
			linkfilename += _T("구룡쟁패 실행.lnk");
			CString linknewfilename = programfolder;
			linknewfilename += _T("구룡쟁패 실행.lnk");
			CFile::Rename( linkfilename, linknewfilename );

			CString weblaunchinglinkfilename = programfolder;
			weblaunchinglinkfilename += _T("구룡쟁패 홈페이지.lnk");
			CString weblaunchinglinknewfilename = programfolder;
			weblaunchinglinknewfilename += _T("구룡쟁패 웹 실행.lnk");
			CFile::Rename( weblaunchinglinkfilename, weblaunchinglinknewfilename );

		}
	}
*/
}

void CXLauncherDlg::CreateLink(LPCTSTR lpszItem, LPCTSTR lpszFile, LPCTSTR lpszWorkingDir, int nFolder, BOOL seticonlocation )
{
	//File system directory that contains the directories for the
	//common program groups that appear on the Start menu for all users. 
	LPITEMIDLIST pidl;
	   
	// Get a pointer to an item ID list that
	// represents the path of a special folder
	HRESULT hr = SHGetSpecialFolderLocation(NULL, nFolder, &pidl);
   
	// Convert the item ID list's binary
	// representation into a file system path
	TCHAR szPath[_MAX_PATH];
	BOOL f = SHGetPathFromIDList(pidl, szPath);
   
	// Allocate a pointer to an IMalloc interface
	LPMALLOC pMalloc;
   
	// Get the address of our task allocator's IMalloc interface
	hr = SHGetMalloc(&pMalloc);
   
	// Free the item ID list allocated by SHGetSpecialFolderLocation
	pMalloc->Free(pidl);
   
	// Free our task allocator
	pMalloc->Release();
   
	CString	szLinkName = lpszItem ;
	szLinkName+= _T(".lnk") ;
   
	CString szCurrentDirectory = szPath ;// "D:\\Documents and Settings\\Administrator\\Start Menu" ;
   
	if( nFolder == CSIDL_PROGRAMS )
	{
		CString szTemp = szLinkName;
		
		// Create program folder...
		CString newpath;
		newpath.Format( g_InterfaceString[55], szCurrentDirectory );//_T("%s\\인디21\\구룡쟁패\\")

		char mbcspathname[MAX_PATH];
		WideCharToMultiByte( CP_ACP, 0, newpath.GetBuffer(MAX_PATH), -1, mbcspathname,MAX_PATH,0,0 );

		MakeSureDirectoryPathExists( mbcspathname );

		CString formatstring = g_InterfaceString[55];
		formatstring += _T("%s");
		szLinkName.Format( formatstring, szCurrentDirectory, szTemp ); //_T("%s\\인디21\\구룡쟁패\\%s")
	}
	else
	{
		CString szTemp = szLinkName;
		szLinkName.Format( _T("%s\\%s"), szCurrentDirectory, szTemp );
	}
   
   
	HRESULT hres = NULL;  
	IShellLinkA* psl = NULL; 

	// Get a pointer to the IShellLink interface. 
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkA, reinterpret_cast<void**>(&psl)); 

	if (SUCCEEDED(hres)) 
	{ 
		   IPersistFile* ppf = NULL; 
		   
		   char mbcspathname[MAX_PATH];
		   WideCharToMultiByte( GetACP(), 0, lpszFile, -1, mbcspathname,MAX_PATH,0,0 );

		   // Set the path to the shortcut target
		   psl->SetPath(mbcspathname); 

		   WideCharToMultiByte( GetACP(), 0, lpszWorkingDir, -1, mbcspathname,MAX_PATH,0,0 );
		   psl->SetWorkingDirectory( mbcspathname );
		   //psl->SetPath(lpszFile); 
		   //psl->SetWorkingDirectory( lpszWorkingDir );
		   
		   if( seticonlocation )
		   {
			   CString iconpath = lpszWorkingDir;
			   iconpath += _T("NineDragons.ico");

			   WideCharToMultiByte( GetACP(), 0, iconpath, -1, mbcspathname,MAX_PATH,0,0 );

			   psl->SetIconLocation( mbcspathname, 0 );
		   }		   
		   
		   // Query IShellLink for the IPersistFile interface for saving 
		   //the shortcut in persistent storage. 
		   hres = psl->QueryInterface(IID_IPersistFile, 
			   reinterpret_cast<void**>(&ppf)); 
		   
		   if (SUCCEEDED(hres)) 
		   { 
			   //WCHAR wsz[MAX_PATH]; 			   
			   // Ensure that the string is ANSI. 
			   //MultiByteToWideChar(CP_ACP, 0, szLinkName, -1, wsz, MAX_PATH); 
			   
			   // Save the link by calling IPersistFile::Save. 
			   hres = ppf->Save(szLinkName.GetBuffer(MAX_PATH), TRUE); 
			   ppf->Release(); 
		   } 
		   psl->Release(); 
	}
}

void CXLauncherDlg::OnSelendokAutoruncombo() 
{
	UpdateData();
}
