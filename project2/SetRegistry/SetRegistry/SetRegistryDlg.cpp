// SetRegistryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetRegistry.h"
#include "SetRegistryDlg.h"

#include "AddServerDlg.h"

#include "RegStringDefine.h"
#include "NangReg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = __FILE__;
#endif

#define _XDEF_TESTSERVERSTRUCTINDICATOR		0xFD4E2A4C

typedef struct _XTESTSERVERINFOSTRUCT
{
	DWORD	testserverindicator;
	TCHAR	cdnserverurl[64];
	DWORD	serverport;
	TCHAR	serveraddress[32];
	DWORD	patchserverport;
	TCHAR	patchserveraddress[32];	
} _XTESTSERVERINFOSTRUCT;


typedef struct _XSERVERINFOSTRUCT
{
	TCHAR	patchserveraddress[32];
	DWORD	patchserverport;
	TCHAR	cdnserverurl[64];
	TCHAR	serveraddress[32];
	DWORD	serverport;
} _XSERVERINFOSTRUCT;

#define _XADDRESSENCODEKEY	0xDC

class _XServerAddressManager
{

public:
	_XTESTSERVERINFOSTRUCT	m_ServerInfo;

	BOOL				m_TestServerSettingMode;

public:
	_XServerAddressManager()
	{
		m_TestServerSettingMode = TRUE;
	}

	BOOL GetServerInfo( void );
	
	void SetServerAddress( LPTSTR address );
	void SetPatchServerAddress( LPTSTR address );
	void SetDowndloadServerAddress( LPTSTR address );

	void SetServerPort( DWORD port );
	void SetPatchServerPort( DWORD port );

	void SetCDNServerUrl( LPTSTR url );

	void XOREncrypt( void );
};

void _XServerAddressManager::XOREncrypt( void )
{
	LPBYTE pServerInfo = (LPBYTE)&m_ServerInfo;

	for( int i = 0; i < sizeof(_XTESTSERVERINFOSTRUCT); i++ )
	{
		pServerInfo[i] ^= _XADDRESSENCODEKEY;
	}
}

BOOL _XServerAddressManager::GetServerInfo( void )
{
	memset( &m_ServerInfo, 0, sizeof(_XTESTSERVERINFOSTRUCT) );

	m_ServerInfo.testserverindicator = _XDEF_TESTSERVERSTRUCTINDICATOR;
	strcpy( m_ServerInfo.patchserveraddress, _T("211.115.88.248") );
	strcpy( m_ServerInfo.serveraddress, _T("211.115.88.248") );
	strcpy( m_ServerInfo.cdnserverurl, _T("download.indy21.lgcdn.com") );
	m_ServerInfo.patchserverport = 50001;
	m_ServerInfo.serverport = 48300;

	XOREncrypt();
	BOOL retval;
	
	if( m_TestServerSettingMode )
		retval = _XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, (LPBYTE)&m_ServerInfo, sizeof(_XTESTSERVERINFOSTRUCT), TRUE );	
	else
	{
		_XSERVERINFOSTRUCT serverinfo;
		retval = _XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)&serverinfo, (LPBYTE)&serverinfo, sizeof(_XSERVERINFOSTRUCT), TRUE );

		memcpy( m_ServerInfo.patchserveraddress, serverinfo.patchserveraddress, sizeof(TCHAR) * 32 );
		memcpy( m_ServerInfo.serveraddress, serverinfo.serveraddress, sizeof(TCHAR) * 32 );
		memcpy( m_ServerInfo.cdnserverurl, serverinfo.cdnserverurl, sizeof(TCHAR) * 64 );
		m_ServerInfo.patchserverport = serverinfo.patchserverport;
		m_ServerInfo.serverport = serverinfo.serverport;
	}

	XOREncrypt();
	return retval;
}

void _XServerAddressManager::SetServerAddress( LPTSTR address )
{
	GetServerInfo();
	strcpy( m_ServerInfo.serveraddress, address );

	XOREncrypt();
	
	if( m_TestServerSettingMode )
		_XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XTESTSERVERINFOSTRUCT) );
	else
	{
		_XSERVERINFOSTRUCT serverinfo;
		
		memcpy( serverinfo.patchserveraddress, m_ServerInfo.patchserveraddress, sizeof(TCHAR) * 32 );
		memcpy( serverinfo.serveraddress, m_ServerInfo.serveraddress, sizeof(TCHAR) * 32 );
		memcpy( serverinfo.cdnserverurl, m_ServerInfo.cdnserverurl, sizeof(TCHAR) * 64 );
		serverinfo.patchserverport = m_ServerInfo.patchserverport;
		serverinfo.serverport = m_ServerInfo.serverport;

		_XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)(&serverinfo), sizeof(_XSERVERINFOSTRUCT) );
	}

	XOREncrypt();
}

void _XServerAddressManager::SetPatchServerAddress( LPTSTR address )
{
	GetServerInfo();	
	strcpy( m_ServerInfo.patchserveraddress, address );
	
	XOREncrypt();
	
	if( m_TestServerSettingMode )
		_XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XTESTSERVERINFOSTRUCT) );
	else
	{
		_XSERVERINFOSTRUCT serverinfo;
		
		memcpy( serverinfo.patchserveraddress, m_ServerInfo.patchserveraddress, sizeof(TCHAR) * 32 );
		memcpy( serverinfo.serveraddress, m_ServerInfo.serveraddress, sizeof(TCHAR) * 32 );
		memcpy( serverinfo.cdnserverurl, m_ServerInfo.cdnserverurl, sizeof(TCHAR) * 64 );
		serverinfo.patchserverport = m_ServerInfo.patchserverport;
		serverinfo.serverport = m_ServerInfo.serverport;
		
		_XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)(&serverinfo), sizeof(_XSERVERINFOSTRUCT) );
	}

	XOREncrypt();
}

void _XServerAddressManager::SetDowndloadServerAddress( LPTSTR address )
{
	GetServerInfo();	
	strcpy( m_ServerInfo.cdnserverurl, address );
	
	XOREncrypt();
	
	if( m_TestServerSettingMode )
		_XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XTESTSERVERINFOSTRUCT) );
	else
	{
		_XSERVERINFOSTRUCT serverinfo;
		
		memcpy( serverinfo.patchserveraddress, m_ServerInfo.patchserveraddress, sizeof(TCHAR) * 32 );
		memcpy( serverinfo.serveraddress, m_ServerInfo.serveraddress, sizeof(TCHAR) * 32 );
		memcpy( serverinfo.cdnserverurl, m_ServerInfo.cdnserverurl, sizeof(TCHAR) * 64 );
		serverinfo.patchserverport = m_ServerInfo.patchserverport;
		serverinfo.serverport = m_ServerInfo.serverport;
		
		_XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)(&serverinfo), sizeof(_XSERVERINFOSTRUCT) );
	}
	
	XOREncrypt();
}


void _XServerAddressManager::SetServerPort( DWORD port )
{
	GetServerInfo();	
	m_ServerInfo.serverport = port;
	
	XOREncrypt();
	
	if( m_TestServerSettingMode )
		_XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XTESTSERVERINFOSTRUCT) );
	else
	{
		_XSERVERINFOSTRUCT serverinfo;
		
		memcpy( serverinfo.patchserveraddress, m_ServerInfo.patchserveraddress, sizeof(TCHAR) * 32 );
		memcpy( serverinfo.serveraddress, m_ServerInfo.serveraddress, sizeof(TCHAR) * 32 );
		memcpy( serverinfo.cdnserverurl, m_ServerInfo.cdnserverurl, sizeof(TCHAR) * 64 );
		serverinfo.patchserverport = m_ServerInfo.patchserverport;
		serverinfo.serverport = m_ServerInfo.serverport;
		
		_XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)(&serverinfo), sizeof(_XSERVERINFOSTRUCT) );
	}

	XOREncrypt();
}

void _XServerAddressManager::SetPatchServerPort( DWORD port )
{
	GetServerInfo();	
	m_ServerInfo.patchserverport = port;
	
	XOREncrypt();
	
	if( m_TestServerSettingMode )
		_XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XTESTSERVERINFOSTRUCT) );
	else
	{
		_XSERVERINFOSTRUCT serverinfo;
		
		memcpy( serverinfo.patchserveraddress, m_ServerInfo.patchserveraddress, sizeof(TCHAR) * 32 );
		memcpy( serverinfo.serveraddress, m_ServerInfo.serveraddress, sizeof(TCHAR) * 32 );
		memcpy( serverinfo.cdnserverurl, m_ServerInfo.cdnserverurl, sizeof(TCHAR) * 64 );
		serverinfo.patchserverport = m_ServerInfo.patchserverport;
		serverinfo.serverport = m_ServerInfo.serverport;
		
		_XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)(&serverinfo), sizeof(_XSERVERINFOSTRUCT) );
	}

	XOREncrypt();
}

void _XServerAddressManager::SetCDNServerUrl( LPTSTR url )
{
	GetServerInfo();	
	strcpy( m_ServerInfo.cdnserverurl , url );
	
	XOREncrypt();
	
	if( m_TestServerSettingMode )
		_XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XTESTSERVERINFOSTRUCT) );
	else
	{
		_XSERVERINFOSTRUCT serverinfo;
		
		memcpy( serverinfo.patchserveraddress, m_ServerInfo.patchserveraddress, sizeof(TCHAR) * 32 );
		memcpy( serverinfo.serveraddress, m_ServerInfo.serveraddress, sizeof(TCHAR) * 32 );
		memcpy( serverinfo.cdnserverurl, m_ServerInfo.cdnserverurl, sizeof(TCHAR) * 64 );
		serverinfo.patchserverport = m_ServerInfo.patchserverport;
		serverinfo.serverport = m_ServerInfo.serverport;
		
		_XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)(&serverinfo), sizeof(_XSERVERINFOSTRUCT) );
	}

	XOREncrypt();
}

_XServerAddressManager	g_ServerAddressManager;


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
// CSetRegistryDlg dialog

CSetRegistryDlg::CSetRegistryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRegistryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetRegistryDlg)
	m_TestServerSetting = TRUE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSetRegistryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRegistryDlg)
	DDX_Control(pDX, IDMANAGERWINDOW, m_ManagerWindowModeButton);
	DDX_Control(pDX, IDC_SERVERLIST, m_ServerListCtrl);
	DDX_Check(pDX, IDC_TESTSERVERCHECK, m_TestServerSetting);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetRegistryDlg, CDialog)
	//{{AFX_MSG_MAP(CSetRegistryDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDADDSERVERADDRESS, OnAddserveraddress)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SERVERLIST, OnItemchangedServerlist)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDDELETESERVERADDRESS, OnDeleteserveraddress)
	ON_BN_CLICKED(IDMODIFYSERVERADDRESS, OnModifyserveraddress)
	ON_NOTIFY(NM_DBLCLK, IDC_SERVERLIST, OnDblclkServerlist)
	ON_BN_CLICKED(IDMANAGERWINDOW, OnManagerwindow)
	ON_BN_CLICKED(IDC_TESTSERVERCHECK, OnTestservercheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRegistryDlg message handlers


BOOL CSetRegistryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	TCHAR currentPath[ _MAX_PATH ];
	memset( currentPath , 0 , _MAX_PATH );
	GetModuleFileName( NULL, currentPath, _MAX_PATH );

	for( int i = _MAX_PATH-1; i > 0; i-- )
	{
		if( currentPath[i] == '\\' )
		{
			currentPath[i] = NULL;
			break;
		}
		else currentPath[i] = NULL;
	}				

	RECT rect;
	m_ServerListCtrl.GetClientRect( &rect );
	m_ServerListCtrl.InsertColumn( 0, "Num", LVCFMT_LEFT, 40 );
	m_ServerListCtrl.InsertColumn( 1, "Server name", LVCFMT_LEFT, (rect.right-rect.left-56) / 4 );
	m_ServerListCtrl.InsertColumn( 2, "Server IP", LVCFMT_LEFT, (rect.right-rect.left-56) / 4 );
	m_ServerListCtrl.InsertColumn( 3, "Patch server IP", LVCFMT_LEFT, (rect.right-rect.left-56) / 4 );
	m_ServerListCtrl.InsertColumn( 4, "download address", LVCFMT_LEFT, (rect.right-rect.left-56) / 4 );
	
	m_ServerListCtrl.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP );

	int servercount = 0;
	FILE* serverinifile = fopen( "Server.ini", "rb" );

	if( !serverinifile )
	{
		m_ServerInfoList.disposeList();

		serverinifile = fopen( "Server.ini", "wb" );
		servercount = 0;
		fwrite( &servercount, sizeof(int), 1, serverinifile );
		fclose( serverinifile );		
	}
	else
	{
		if( fread( &servercount, sizeof(int), 1, serverinifile ) < 1 )
		{
			AfxMessageBox( "ERROR : Read server information file" );
		}
		else 
		{				
			m_ServerInfoList.disposeList();
			
			_XServerInfoItem* psi;
			
			for( int i = 0; i < servercount; i++ )
			{
				if( i >= _XDEF_MAXSERVERINFO )
				{						
					break;
				}

				psi = new _XServerInfoItem;

				if( fread( &psi->si, sizeof(_XDEF_SERVERINFOSTRUCT), 1, serverinifile ) < 1 )
				{
					delete psi;
					AfxMessageBox( "ERROR : Read server information file" );					
					break;
				}
				else
				{
					m_ServerInfoList.insertItem( psi );					
				}
			}
			RebuildServerList();			
		}

		fclose( serverinifile );
	}

	BOOL windowmode = _XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MANAGERWINDOW, 1 );

	if( windowmode )
		m_ManagerWindowModeButton.SetWindowText( "Window" );
	else
		m_ManagerWindowModeButton.SetWindowText( "Fullscreen" );
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSetRegistryDlg::RebuildServerList( void )
{
	CString str;
	int i = 0;

	m_ServerInfoList.resetList();
	if( m_ServerInfoList.listEmpty() ) return;

	m_ServerListCtrl.DeleteAllItems();

	_XServerInfoItem* psi;

	do
	{
		psi = (_XServerInfoItem*)m_ServerInfoList.currentItem();

		if( psi )
		{
			str.Format( "%d", i+1 );
			m_ServerListCtrl.InsertItem( i, str, 0 );
			m_ServerListCtrl.SetItemText( i, 1, psi->si.servername );
			m_ServerListCtrl.SetItemText( i, 2, psi->si.serveraddress );
			m_ServerListCtrl.SetItemText( i, 3, psi->si.patchserveraddress );
			m_ServerListCtrl.SetItemText( i, 4, psi->si.downloadserveraddress );
		}

		i++;

		m_ServerInfoList.nextItem();
	}while( !m_ServerInfoList.atHeadOfList() );	

	m_ServerListCtrl.SetItem( 0, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED , NULL );
}

void CSetRegistryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSetRegistryDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSetRegistryDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSetRegistryDlg::OnAddserveraddress() 
{
	// TODO: Add your control notification handler code here
	if( m_ServerInfoList.getitemcount() >= _XDEF_MAXSERVERINFO )	return;

	CAddServerDlg addserverdlg;

	if( addserverdlg.DoModal() == IDOK )
	{
		_XServerInfoItem* psi = new _XServerInfoItem;

		strcpy( psi->si.servername, addserverdlg.m_ServerName );
		strcpy( psi->si.serveraddress, addserverdlg.m_AdressString );
		strcpy( psi->si.patchserveraddress, addserverdlg.m_PatchServerIPString );
		strcpy( psi->si.downloadserveraddress, addserverdlg.m_DownloadAdress );

		m_ServerInfoList.resetList();
		m_ServerInfoList.insertItem( psi );

		RebuildServerList();
	}	
}

void CSetRegistryDlg::OnItemchangedServerlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CSetRegistryDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
		
	CDialog::OnClose();	
}

void CSetRegistryDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	FILE* serverinifile = fopen( "Server.ini", "wb" );
	
	if( serverinifile )
	{
		int ServerCount = m_ServerInfoList.getitemcount();

		fwrite( &ServerCount, sizeof(int), 1, serverinifile );

		m_ServerInfoList.resetList();
		if( m_ServerInfoList.listEmpty() )
		{
			fclose( serverinifile );	
			return;
		}

		_XServerInfoItem* psi;

		do
		{
			psi = (_XServerInfoItem*)m_ServerInfoList.currentItem();

			if( fwrite( &psi->si, sizeof(_XDEF_SERVERINFOSTRUCT), 1, serverinifile ) < 1 )
			{
				AfxMessageBox( "ERROR : Write server information file" );
				break;
			}

			m_ServerInfoList.nextItem();
		}while( !m_ServerInfoList.atHeadOfList() );	
		
		fclose( serverinifile );
	}
}

void CSetRegistryDlg::OnOK() 
{
	// TODO: Add extra validation here
/*	if( m_ServerInfoList.getitemcount() > 0 )
	{
		POSITION pos = this->m_ServerListCtrl.GetFirstSelectedItemPosition();

		if ( pos )
		{
			int SelectedIndex = this->m_ServerListCtrl.GetNextSelectedItem(pos);

			m_ServerInfoList.resetList();
			if( !m_ServerInfoList.listEmpty() )
			{
				_XServerInfoItem* psi = (_XServerInfoItem*)m_ServerInfoList.getItem( SelectedIndex );

				if( psi )
				{
					NANGGEK_RegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SERVERADDRESS, psi->si.serveraddress );
					NANGGEK_RegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VERSIONSERVERADDRESS, psi->si.patchserveraddress );
				}
			}
		}
	}*/
	
	CDialog::OnOK();
}

void CSetRegistryDlg::OnDeleteserveraddress() 
{
	// TODO: Add your control notification handler code here
	if( m_ServerInfoList.getitemcount() > 0 )
	{
		POSITION pos = this->m_ServerListCtrl.GetFirstSelectedItemPosition();

		if ( pos )
		{
			int SelectedIndex = this->m_ServerListCtrl.GetNextSelectedItem(pos);
			m_ServerInfoList.deleteItem( SelectedIndex );

			RebuildServerList();
		}
	}
}

void CSetRegistryDlg::OnModifyserveraddress() 
{
	// TODO: Add your control notification handler code here	
	POSITION pos = this->m_ServerListCtrl.GetFirstSelectedItemPosition();

	if ( pos )
	{
		int SelectedIndex = this->m_ServerListCtrl.GetNextSelectedItem(pos);

		_XServerInfoItem* psi = (_XServerInfoItem*)m_ServerInfoList.getItem( SelectedIndex );

		if( psi )
		{
			CAddServerDlg addserverdlg;

			addserverdlg.m_ServerName = psi->si.servername;
			addserverdlg.m_AdressString = psi->si.serveraddress;
			addserverdlg.m_PatchServerIPString = psi->si.patchserveraddress;
			addserverdlg.m_DownloadAdress = psi->si.downloadserveraddress;

			if( addserverdlg.DoModal() == IDOK )
			{
				strcpy( psi->si.servername, addserverdlg.m_ServerName );
				strcpy( psi->si.serveraddress, addserverdlg.m_AdressString );
				strcpy( psi->si.patchserveraddress, addserverdlg.m_PatchServerIPString );			
				strcpy( psi->si.downloadserveraddress, addserverdlg.m_DownloadAdress );

				RebuildServerList();
			}	
		}
	}
}

void CSetRegistryDlg::OnDblclkServerlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if( m_ServerInfoList.getitemcount() > 0 )
	{
		POSITION pos = this->m_ServerListCtrl.GetFirstSelectedItemPosition();

		if ( pos )
		{
			int SelectedIndex = this->m_ServerListCtrl.GetNextSelectedItem(pos);

			m_ServerInfoList.resetList();
			if( !m_ServerInfoList.listEmpty() )
			{
				_XServerInfoItem* psi = (_XServerInfoItem*)m_ServerInfoList.getItem( SelectedIndex );

				if( psi )
				{
					g_ServerAddressManager.SetServerAddress( psi->si.serveraddress );
					g_ServerAddressManager.SetPatchServerAddress( psi->si.patchserveraddress );
					g_ServerAddressManager.SetDowndloadServerAddress( psi->si.downloadserveraddress );

					//_XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SERVERADDRESS, psi->si.serveraddress );
					//_XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VERSIONSERVERADDRESS, psi->si.patchserveraddress );

					CString message;
					message.Format( "Change login server : %s", psi->si.servername );
					AfxMessageBox( message );
				}
			}
		}
	}
	
	*pResult = 0;
}

void CSetRegistryDlg::OnManagerwindow() 
{
	// TODO: Add your control notification handler code here
	BOOL windowmode = _XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MANAGERWINDOW, 1 );
	_XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MANAGERWINDOW, !windowmode );

	if( windowmode )
		m_ManagerWindowModeButton.SetWindowText( "Fullscreen" );
	else
		m_ManagerWindowModeButton.SetWindowText( "Window" );
}

void CSetRegistryDlg::OnTestservercheck() 
{
	UpdateData();

	g_ServerAddressManager.m_TestServerSettingMode = m_TestServerSetting;
}
