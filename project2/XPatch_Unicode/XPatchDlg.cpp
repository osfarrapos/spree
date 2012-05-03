// XPatchDlg.cpp : implementation file
//

#include "stdafx.h"

#pragma warning(disable:4786)

#include <direct.h>

#include "XPatch.h"
#include "XPatchDlg.h"

#include "RegStringDefine.h"
#include "XRegistry.h"

#include "Package.h"

#include "Zip\\ZipArchive.h"

#include "SeverMessageDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TCHAR g_InterfaceString[100][1024];

_XServerAddressManager	g_ServerAddressManager;
_XPatchVersionManager	g_PatchVersionManager;
TCHAR	g_Installedpath[_REGSTRING_MAXLENGTH+1];
TCHAR	g_PatchProgrampath[_MAX_PATH+1];

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
			strcpy( m_ServerInfo.patchserveraddress, "64.93.77.158" );
			strcpy( m_ServerInfo.serveraddress, "64.93.77.158" );
			strcpy( m_ServerInfo.cdnserverurl, "9dragons.acclaimdownloads.com/9dragons" );
		#else
			#ifdef _XPWC
				strcpy( m_ServerInfo.patchserveraddress, "85.17.55.207" );
				strcpy( m_ServerInfo.serveraddress, "85.17.55.207" );
				strcpy( m_ServerInfo.cdnserverurl, "85.17.55.204" );				
			#else
				strcpy( m_ServerInfo.patchserveraddress, "login.ninedragons.co.kr" );
				strcpy( m_ServerInfo.serveraddress, "login.ninedragons.co.kr" );
				strcpy( m_ServerInfo.cdnserverurl, "download.ninedragons.co.kr" );
			#endif
		#endif
	#endif
#endif

	m_ServerInfo.patchserverport = 50001;
	m_ServerInfo.serverport = 48300;
	
	XOREncrypt();
	BOOL retval = g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT), TRUE );	
	XOREncrypt();
	
	if( m_ServerInfo.testserverindicator != _XDEF_TESTSERVERSTRUCTINDICATOR )
	{
		memset( &m_ServerInfo, 0, sizeof(_XSERVERINFOSTRUCT) );
		AfxMessageBox( "Invalid server information : Modified registry data" );
		_XLog( TRUE, "Invalid server information : Modified registry data\n" );
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
	#endif
#else
	#ifdef _XVIETNAMESE
		strcpy( m_ServerInfo.patchserveraddress, "login.cuulong.com.vn" );
		strcpy( m_ServerInfo.serveraddress, "login.cuulong.com.vn" );
		strcpy( m_ServerInfo.cdnserverurl, "dl.sg.vinagame.com.vn" );
	#else
		#ifdef _XACCLAIM
			strcpy( m_ServerInfo.patchserveraddress, "64.93.77.158" );
			strcpy( m_ServerInfo.serveraddress, "64.93.77.158" );
			strcpy( m_ServerInfo.cdnserverurl, "9dragons.acclaimdownloads.com/9dragons" );
		#else
			#ifdef _XPWC
				strcpy( m_ServerInfo.patchserveraddress, "85.17.55.207" );
				strcpy( m_ServerInfo.serveraddress, "85.17.55.207" );
				strcpy( m_ServerInfo.cdnserverurl, "85.17.55.204" );				
			#else
				strcpy( m_ServerInfo.patchserveraddress, "login.ninedragons.co.kr" );
				strcpy( m_ServerInfo.serveraddress, "login.ninedragons.co.kr" );
				strcpy( m_ServerInfo.cdnserverurl, "download.ninedragons.co.kr" );
			#endif
		#endif
	#endif
#endif

	m_ServerInfo.patchserverport = 50001;
	m_ServerInfo.serverport = 48300;
	
	XOREncrypt();
	BOOL retval = g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "ndc info", (LPBYTE)&m_ServerInfo, (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT), TRUE );	
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
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "ts ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif

	XOREncrypt();
}

void _XServerAddressManager::SetPatchServerAddress( LPSTR address )
{
	GetServerInfo();	
	strcpy( m_ServerInfo.patchserveraddress, address );
	
	XOREncrypt();

#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "ts ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif

	XOREncrypt();
}

void _XServerAddressManager::SetServerPort( DWORD port )
{
	GetServerInfo();	
	m_ServerInfo.serverport = port;
	
	XOREncrypt();

#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "ts ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif

	XOREncrypt();
}

void _XServerAddressManager::SetPatchServerPort( DWORD port )
{
	GetServerInfo();	
	m_ServerInfo.patchserverport = port;
	
	XOREncrypt();

#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "ts ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif

	XOREncrypt();
}

void _XServerAddressManager::SetCDNServerUrl( LPSTR url )
{
	GetServerInfo();	
	strcpy( m_ServerInfo.cdnserverurl , url );
	
	XOREncrypt();

#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "ts ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "ndc info", (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif

	XOREncrypt();
}



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

	// 메인프로그램의 버전을 읽어옴..
	CString szMainProgramPath;	
	
	if( wcslen( g_Installedpath ) <= 0 )
	{
		memset( g_Installedpath, 0, sizeof(TCHAR)*(_REGSTRING_MAXLENGTH+1) );		
		g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_INSTALLEDPATH, _T(""), g_Installedpath, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, FALSE);
		
		if( wcslen( g_Installedpath ) <= 0 )
		{
			memset( g_Installedpath, 0, sizeof(TCHAR)*(_REGSTRING_MAXLENGTH+1) );
			GetCurrentDirectory( _REGSTRING_MAXLENGTH, g_Installedpath );
			g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_INSTALLEDPATH, g_Installedpath );
		}
	}
	
	szMainProgramPath = g_Installedpath;
	
#ifdef _XTESTSERVER
	szMainProgramPath += _T("\\tsNineDragons.EXE");
#else
	szMainProgramPath += _T("\\NineDragons.EXE");
#endif
	
	DWORD h1,h2,l1,l2;
	CXPatchDlg::_XGetExeFileVersion( szMainProgramPath.GetBuffer(512), h1,h2,l1,l2 );
	
#ifdef _XTESTSERVER		

	m_VersionInfo.testserverindicator = _XDEF_TESTSERVERSTRUCTINDICATOR;

	if( !g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT), FALSE ) )
	{
		XOREncrypt();
		m_VersionInfo.patchversion_1 = 205;
		m_VersionInfo.patchversion_2 = rand() % 32700;
		XOREncrypt();
	}
#else
	
	if( !g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT), FALSE ) )
	{
		#ifdef _XVIETNAMESE
			m_VersionInfo.patchversion_1 = 0;
		#else 
			#ifdef _XTAIWANESE
				#ifdef _XTW_CSTESTVERSION
					m_VersionInfo.patchversion_1 = 0;
				#else
					m_VersionInfo.patchversion_1 = 0;
				#endif
			#else
				#ifdef _XACCLAIM
					m_VersionInfo.patchversion_1 = 0;
				#else
					#ifdef _XPWC
						m_VersionInfo.patchversion_1 = 0;
					#else
						m_VersionInfo.patchversion_1 = 256;
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
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#endif
		XOREncrypt();
	}
	else
	{
		XOREncrypt();
		if( m_VersionInfo.patchversion_1 > 1000 )
		{
			#ifdef _XVIETNAMESE
				m_VersionInfo.patchversion_1 = 0;
			#else 
				#ifdef _XTAIWANESE
					#ifdef _XTW_CSTESTVERSION
						m_VersionInfo.patchversion_1 = 0;
					#else
						m_VersionInfo.patchversion_1 = 0;
					#endif
				#else
					#ifdef _XACCLAIM
						m_VersionInfo.patchversion_1 = 0;
					#else
						#ifdef _XPWC
							m_VersionInfo.patchversion_1 = 0;
						#else
							m_VersionInfo.patchversion_1 = 256;
						#endif
					#endif
				#endif
			#endif
		}

		m_VersionInfo.patchversion_2 = rand() % 32700;

		XOREncrypt();
#ifdef _XTESTSERVER
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
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
	BOOL retval = g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT), TRUE );
#else
	BOOL retval = g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT), TRUE );
#endif

	XOREncrypt();
	
	if( m_VersionInfo.exeversion_1 != h1 ||
		m_VersionInfo.exeversion_2 != h2 ||
		m_VersionInfo.exeversion_4 != l2 )
	{
		XOREncrypt();
#ifdef _XTESTSERVER
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
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
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
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
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_tsndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "_xkern_ndvinfo", (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#endif
	XOREncrypt();
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
// CXPatchDlg dialog

CXPatchDlg::CXPatchDlg(CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CXPatchDlg::IDD, pParent, _T("IDR_BORDERRGN"))
{
	//{{AFX_DATA_INIT(CXPatchDlg)
	m_PatchPackageFilename = _T("");
	m_ProgressString = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXPatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CBitmapDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXPatchDlg)	
	DDX_Text(pDX, IDC_SELECTEDFILENAMESTATIC, m_PatchPackageFilename);
	DDX_Text(pDX, IDC_PROGRESSSTATIC, m_ProgressString);
	DDX_Control(pDX, IDC_MINIMIZEBUTTON, m_MinimizeButton);
	DDX_Control(pDX, IDC_CLOSEBUTTON, m_CloseButton);
	DDX_Control(pDX, IDC_SELECTPACKAGEBUTTON, m_PackageOpenButton );
	DDX_Control(pDX, IDC_UPDATEBUTTON, m_PatchButton );
	DDX_Control(pDX, IDC_SELECTEDFILENAMESTATIC, m_SelectedPackageStatic );
	DDX_Control(pDX, IDC_PROGRESSSTATIC, m_ProgressStatic );	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXPatchDlg, CBitmapDialog)
	//{{AFX_MSG_MAP(CXPatchDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECTPACKAGEBUTTON, OnSelectpackagebutton)
	ON_BN_CLICKED(IDC_UPDATEBUTTON, OnUpdatebutton)	
	ON_BN_CLICKED(IDC_MINIMIZEBUTTON, OnMinimizebutton)
	ON_BN_CLICKED(IDC_CLOSEBUTTON, OnClosebutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXPatchDlg message handlers

STDMETHODIMP CXPatchDlg::_XGetExeFileVersion(LPTSTR sFilename, DWORD& h_1,DWORD& h_2,DWORD& l_1,DWORD& l_2 )
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

BOOL CXPatchDlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog();

	LoadString();

	memset( g_PatchProgrampath, 0, sizeof(TCHAR) * (_MAX_PATH+1) );
	GetCurrentDirectory( _MAX_PATH, g_PatchProgrampath );
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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
	
	// TODO: Add extra initialization here

	CenterWindow();

	RECT rect;
	GetClientRect(&rect);
	ModifyStyle(WS_CAPTION, NULL);
	MoveWindow(&rect);
	
	MoveWindow( 0, 0, 578, 123 );
		
	LoadBitmap (IDB_BACKGROUNDBITMAP);
	SetTransparent (TRUE);
	SetTransColor (RGB(255,0,0));
	SetStaticTransparent (TRUE);
	SetClickAnywhereMove (TRUE);

	GetWindowRect( &rect );	

	m_MinimizeButton.SetIcon( IDI_MINIMIZEICON );
	m_MinimizeButton.MoveWindow( rect.right - 48, 4, 16, 16 );

	m_CloseButton.SetIcon( IDI_CLOSEICON );
	m_CloseButton.MoveWindow( rect.right - 30, 4, 16, 16 );

	m_PatchButton.SetBitmaps( IDB_UPGRADEBITMAP, RGB(255,0,0) );
	m_PatchButton.MoveWindow( 464, 92, 68, 26 );

	m_PackageOpenButton.SetBitmaps( IDB_OPENBITMAP, RGB(255,0,0) );
	m_PackageOpenButton.MoveWindow( 317, 92, 138, 26 );

	m_SelectedPackageStatic.MoveWindow( 54, 44, 472, 15 );
	m_SelectedPackageStatic.SetTextColor(RGB(255,255,255))
						   .SetBkColor( RGB(0,0,0) );
						   //.SetTransparent(TRUE);	
	m_ProgressStatic.MoveWindow( 54, 69, 472, 15 );
	m_ProgressStatic.SetTextColor(RGB(255,255,255))
				    .SetBkColor( RGB(0,0,0) );
	
	m_Message = _T("");

	m_PatchNumber = 0;
	m_ServerPatchNumber = 0;
	
	m_Version = 0;
	m_ServerVersion = 0;
	
	memset( g_Installedpath, 0, sizeof(TCHAR)*(_REGSTRING_MAXLENGTH+1) );
	_XRegReadString( HKEY_CURRENT_USER, _T(ID_REGSTR_INDY21KEY), _T(ID_REGSTR_INSTALLEDPATH), _T(""), g_Installedpath, sizeof(TCHAR)*_REGSTRING_MAXLENGTH, FALSE);
	
	if( wcslen(g_Installedpath) <= 0 )
	{
		wcscpy( g_Installedpath, g_PatchProgrampath );
	}
		
	//OutputDebugString( g_Installedpath );

	g_RegManager.SetRegistryFolder(g_Installedpath);
	
	_wchdir( g_Installedpath );
	if( !CheckExistFile( _T("ndreg.xrg"), FALSE ) )
	{
		//g_RegManager.ImportWindowsRegistry();
		AfxMessageBox( _T("Error : Could not find game installed folder.") );
		return FALSE;
	}
	else
	{
		g_RegManager.LoadRegistry();
	}

	_wchdir( g_PatchProgrampath );

	CString intializemessage;
	m_PatchPackageFilename.Format( _T("[ %s ]"), g_InterfaceString[1] );//_T("구룡쟁패를 이용해 주셔서 감사합니다.")

	g_PatchVersionManager.GetVersionInfo();

	intializemessage.Format( g_InterfaceString[2], g_PatchVersionManager.m_VersionInfo.patchversion_1 ); //_T("현재 %d 버젼으로 업데이트 되어 있습니다.")
	SetProgressString( intializemessage );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXPatchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CBitmapDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXPatchDlg::OnPaint() 
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
		CBitmapDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXPatchDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CXPatchDlg::OnSelectpackagebutton() 
{
	// TODO: Add your control notification handler code here
	CFileDialog	FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL );

	FileDlg.m_ofn.hwndOwner		= NULL;
	FileDlg.m_ofn.lStructSize		= sizeof(OPENFILENAME);
	FileDlg.m_ofn.lpstrFilter		= _T("XPZ File(*.XPZ)\0*.XPZ\0");
	FileDlg.m_ofn.lpstrInitialDir	= _T(".\\");
	FileDlg.m_ofn.lpstrTitle		= _T("Load NineDragons Upgrade package file");
	FileDlg.m_ofn.lpstrDefExt		= _T("*.XPZ");
	
	if( FileDlg.DoModal() == IDOK )
	{			
		this->m_PatchPackageFilename = FileDlg.GetPathName();
		m_SelectedPackageStatic.SetTextColor(RGB(255,255,255))
							   .SetBkColor( RGB(0,0,0) )			   
						       .SetTransparent(TRUE);
		
		UpdateData( FALSE );
		RedrawWindow();
	}
	
}

void CXPatchDlg::OnMinimizebutton() 
{
	// TODO: Add your control notification handler code here
	ShowWindow( SW_MINIMIZE );
}

void CXPatchDlg::OnClosebutton() 
{
	// TODO: Add your control notification handler code here
	PostQuitMessage( IDOK );
}


void CXPatchDlg::OnUpdatebutton() 
{
	// TODO: Add your control notification handler code here
	Patch();

}

void CXPatchDlg::Patch()
{
	 // Check registry
	 TCHAR registrybuffer[_REGSTRING_MAXLENGTH];    	
	 memset( registrybuffer, 0, _REGSTRING_MAXLENGTH );	 
	 //_XRegReadString( ID_REGSTR_MASTERKEY, _T(ID_REGSTR_INDY21KEY), _T(ID_REGSTR_INSTALLEDPATH), _T(""), registrybuffer, _REGSTRING_MAXLENGTH, FALSE);
	 wcscpy( registrybuffer, g_Installedpath );
	 
	 BOOL showedservermessage = !TRUE;

	 if( wcslen( registrybuffer ) <= 0 )
	 {
		 CString tMessageString;
		 tMessageString = g_InterfaceString[0];//_T("프로그램의 버전이 비정상입니다. 프로그램을 재 설치 해주시기 바랍니다.");

		 AfxMessageBox( tMessageString );
		 return;
	 }

	 /*if( this->m_PatchPackageFilename.GetLength() == 0 ) 
	 {
		 OnSelectpackagebutton();

		 if( this->m_PatchPackageFilename.GetLength() == 0 ) return;
	 }*/

	 
	 // Decompress patch package...	 

	 CZipArchive zip;

	 m_PatchNumber = g_PatchVersionManager.m_VersionInfo.patchversion_1;

	 int patchprocessindex = 0;
	 
	 CString teststring;
	 teststring.Format( _T("PATCH%d.XPZ"), m_PatchNumber+1 );

	 _wchdir( g_PatchProgrampath );	 
	 if( !CheckExistFile( teststring, TRUE ) )
	 {
		patchprocessindex = m_PatchNumber;
	 }
	 else 
	 {
		patchprocessindex = m_PatchNumber + 1;
	 }
	 
	 while(1)
	 {		
		 this->m_PatchPackageFilename.Format( _T("PATCH%d.XPZ"), patchprocessindex );
	
		 _wchdir( g_PatchProgrampath );
		 
		 if( !CheckExistFile( this->m_PatchPackageFilename, TRUE ) )
		 {
			 CString messagestring;
			 messagestring = g_InterfaceString[3];//_T("업데이트 완료");
			 
			 m_PatchPackageFilename.Format( _T("[ %s ]"), g_InterfaceString[1] );//_T("구룡쟁패를 이용해 주셔서 감사합니다.") );
			 SetProgressString( messagestring );
			 break;
		 }

		 zip.Open( this->m_PatchPackageFilename, CZipArchive::openReadOnly );

		 _wchdir( registrybuffer );

		 SetProgressString( g_InterfaceString[6] );

		 for ( int i = 0; i < zip.GetNoEntries(); i++)
		 {
			  zip.ExtractFile(i, registrybuffer);
		 }

		 zip.Close();

		 SetProgressString( g_InterfaceString[7] );

		 if( ParsingPatchScript() == -1 )
		 {			 
			 _wchdir( g_PatchProgrampath );	 
			 return;
		 }
		 
		 // Package 안의 파일 이름 설정 
		 CPackage package;
		 CString patchfilename;
		 patchfilename.Format( _T("PATCH%d.XP"), m_ServerPatchNumber );

		 CString patchinfofilename;
		 patchinfofilename.Format( _T("PATCH%d.XPI"), m_ServerPatchNumber );

		 
		 if( m_PatchNumber >= m_ServerPatchNumber )
		 {
			CString tmessagestring;
			tmessagestring.Format( g_InterfaceString[4], m_ServerPatchNumber ); //_T("업데이트를 시작합니다. Version  : %d")
			if( AfxMessageBox( tmessagestring, MB_YESNO ) != IDYES )
			{			
				if( CheckExistFile( patchfilename, TRUE ) ) CFile::Remove( patchfilename );
				if( CheckExistFile( patchinfofilename, TRUE ) ) CFile::Remove( patchinfofilename );			
				_wchdir( g_PatchProgrampath );
				return;
			}
		 } 


		 // 메세지 출력
		 int result = -1;

		 result = IDYES;

		 if( showedservermessage )
		 {
			 showedservermessage = FALSE;
		
			 if( m_Message.GetLength() > 0 )
			 {
				 CSeverMessageDialog messagedialog;	
				 messagedialog.m_MessageString = this->m_Message;
				 result = messagedialog.DoModal();
				 
				 if( result == IDOK ) result = IDYES;
				 else result = -1;
				
				 this->m_Message = _T("");
			 }
			 else
			 {		 
				 CString versionstring;
				 versionstring.Format( _T("%d"), this->m_ServerPatchNumber );
				 AfxFormatString1(this->m_Message, IDS_UPDATENOW, versionstring );		 
				 result = AfxMessageBox( this->m_Message );
			 }
		 }

		 if( result == IDYES ) 
		 {
			 if( package.Patch( patchfilename, patchinfofilename ) )
			 {	
				 CString messagestring;
				 messagestring.Format( _T("%s : [version %d]"), g_InterfaceString[3], patchprocessindex ); //_T("업데이트 완료")
				 SetProgressString( messagestring );
				 // 패치된 버전 번호 및 패치 넘버를 레지스트리에 등록
				 /*
				 _XRegWriteInt( ID_REGSTR_MASTERKEY, _T(ID_REGSTR_INDY21KEY), _T(ID_REGSTR_PATCHVERSION), this->m_ServerVersion );
				 _XRegWriteInt( ID_REGSTR_MASTERKEY, _T(ID_REGSTR_INDY21KEY), _T(ID_REGSTR_PATCHNUMBER), m_ServerPatchNumber );
				 _XRegWriteString( ID_REGSTR_MASTERKEY, _T(ID_REGSTR_INDY21KEY), _T(ID_REGSTR_PACKAGEVERSION), m_ServerPackageVersionName );
				 */
				
				 g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_PATCHVERSION, this->m_ServerVersion );				 
				 g_PatchVersionManager.SetPatchNumber( m_ServerPatchNumber );
				 g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_PACKAGEVERSION, m_ServerPackageVersionName );
			 }
			 else
			 {
				 CString messagestring;
				 messagestring.Format( _T("%s : [%d]"), g_InterfaceString[3], patchprocessindex );//_T("업데이트 완료")
				 SetProgressString( messagestring );
			 }
		 }
		 
		 if( CheckExistFile( patchfilename, TRUE ) ) CFile::Remove( patchfilename );
		 if( CheckExistFile( patchinfofilename, TRUE ) ) CFile::Remove( patchinfofilename );	 

		 patchprocessindex++;
	 }

	 _wchdir( g_PatchProgrampath );
}


int CXPatchDlg::ParsingPatchScript( void )
{
	FILE* PatchScriptFile = NULL;

	PatchScriptFile = fopen( "Patch.PSC", "rt" );

	if( !PatchScriptFile )
	{
		CString tMessageString;
		tMessageString.Format( _T("[ %s ] file not found"), _T("Patch.PSC") );
		AfxMessageBox( tMessageString );

		m_PatchPackageFilename = g_InterfaceString[5];//_T("[ 업데이트에 실패하였습니다. ]");
		SetProgressString( tMessageString );
		// File openning error...
		return -1;
	}

	char argIDString[256];
	char argValueString[256];
	char parsingString[256];
	BOOL messagestart = FALSE;

	m_Message = _T("");
	//int  argValue;

	while( !feof( PatchScriptFile ) )
	{	
		if( feof( PatchScriptFile ) ) break;
				
		memset( parsingString, 0, 256 );
		fgets( parsingString, 255, PatchScriptFile );

		if( parsingString[0] != ';' ) 
		{
			if( strcmp( argIDString, "EOF" ) == 0 )
			{
				break;
			}

			memset( argIDString, 0, 256 );
			memset( argValueString, 0, 256 );
			sscanf( parsingString, "%s %s", argIDString, argValueString );			
			
			if( messagestart )
			{
				/*
				if( strcmp( argIDString, "ID_ENDMESSAGE" ) == 0 )
				{
					messagestart = FALSE;
				}
				else
				{
					CString tstr = parsingString;
					tstr.Replace( "\n", "\r\n" );
					m_Message += tstr;					
				}
				*/
			}
			else
			{
				if( strcmp( argIDString, "ID_PATCHINDEX" ) == 0 )
				{
					m_ServerPatchNumber = atoi( argValueString );
				}
				if( strcmp( argIDString, "ID_PACKAGEVERSION" ) == 0 )
				{
					m_ServerPackageVersionName = argValueString;
				}
				else if( strcmp( argIDString, "ID_PATCHVERSION" ) == 0 )
				{
					m_ServerVersion = atoi( argValueString );
				}			
				else if( strcmpi( argIDString, "ID_VERSIONSERVERIP" ) == 0 )
				{
					g_ServerAddressManager.SetPatchServerAddress(argValueString);
				}
				else if( strcmpi( argIDString, "ID_PATCHSERVERIP" ) == 0 )
				{
					g_ServerAddressManager.SetCDNServerUrl(argValueString);
				}
				else if( strcmpi( argIDString, "ID_SERVERIP" ) == 0 )
				{
					g_ServerAddressManager.SetServerAddress(argValueString);
				}
				else if( strcmpi( argIDString, "ID_SERVERPORT" ) == 0 )
				{
					DWORD portnumber = atoi( argValueString );
					g_ServerAddressManager.SetServerPort(portnumber);
				}
				else if( strcmpi( argIDString, "ID_MESSAGERSERVERIP" ) == 0 )
				{
					g_RegManager._XRegWriteString(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "Messenger Server address", argValueString );
				}
				else if( strcmpi( argIDString, "ID_LOGSERVERIP" ) == 0 )
				{
					g_RegManager._XRegWriteString(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "Log Ftp Server address", argValueString );
				}
				else if( strcmpi( argIDString, "ID_MESSAGERSERVERPORT" ) == 0 )
				{
					long portnumber = atol( argValueString );
					g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, "Messenger Server port", portnumber );
				}
				else if( strcmp( argIDString, "ID_MESSAGE" ) == 0 )
				{
					//messagestart = TRUE;			
				}
			}
		}		
	}

	fclose( PatchScriptFile );
	
	if( CheckExistFile( _T("Patch.PSC"), TRUE ) ) CFile::Remove( _T("Patch.PSC") );

	return 1;
}

void CXPatchDlg::SplitMessage( CString& sourceString, CString& destString )
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


void CXPatchDlg::SetProgressString( CString progressmessage )
{	
	m_SelectedPackageStatic.SetTextColor(RGB(255,255,255))
						   .SetBkColor( RGB(0,0,0) )
						   .SetTransparent(FALSE);

	this->m_ProgressString = progressmessage;
	m_ProgressStatic.SetTextColor(RGB(255,255,255))
					.SetBkColor( RGB(0,0,0) )
					.SetTransparent(FALSE);
	
	RECT windowrect;
	this->GetWindowRect( &windowrect );

	windowrect.right	 = windowrect.left + 527;
	windowrect.bottom	 = windowrect.top + 82;
	windowrect.left		 += 55;
	windowrect.top		 += 41;							
		
	UpdateData( FALSE );
	RedrawWindow( &windowrect );
	m_SelectedPackageStatic.RedrawWindow();
	m_ProgressStatic.RedrawWindow();
}


void CXPatchDlg::LoadString( void )
{
#ifdef _XVIETNAMESE
	HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_VIETNAMESEINTERFACETEXT2),_T("Text"));
#else
	
#ifdef _XTAIWANESE
	HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_TAIWANESEINTERFACETEXT),_T("Text"));
#else
	#ifdef _XACCLAIM
		HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_ENGLISHINTERFACETEXT),_T("Text"));
	#else
		HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_KOREANINTERFACETEXT),_T("Text"));
	#endif
#endif
	
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