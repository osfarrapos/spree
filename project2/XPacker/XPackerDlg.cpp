// XPackerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XPacker.h"
#include "XPackerDlg.h"
#include "DlgProxy.h"

#include "XDSound.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CXPackerDlg dialog

IMPLEMENT_DYNAMIC(CXPackerDlg, CDialog);

CXPackerDlg::CXPackerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXPackerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXPackerDlg)
	m_LoadedFilename = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
//	m_pAutoProxy = NULL;

	this->m_Initialized = FALSE;
}

CXPackerDlg::~CXPackerDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
//	if (m_pAutoProxy != NULL)
//		m_pAutoProxy->m_pDialog = NULL;
}

void CXPackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXPackerDlg)
	DDX_Text(pDX, IDC_FILENAMESTATIC, m_LoadedFilename);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXPackerDlg, CDialog)
	//{{AFX_MSG_MAP(CXPackerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_LOADBUTTON, OnLoadbutton)
	ON_BN_CLICKED(IDC_RESETBUTTON, OnResetbutton)
	ON_BN_CLICKED(IDC_SAVEBUTTON, OnSavebutton)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_ABOUTBUTTON, OnAboutbutton)
	ON_BN_CLICKED(ID_EXITBUTTON, OnExitbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXPackerDlg message handlers

BOOL CXPackerDlg::OnInitDialog()
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

	// TODO: Add extra initialization here	
	if( !this->m_Initialized )
	{
		RECT wndrect;
		GetClientRect( &wndrect );
		RECT rect = { 2, wndrect.bottom - 64, wndrect.right-2, wndrect.bottom-46 };

		m_wndtabctrl.Create(  this, IDD_TABCTRLVIEW, WS_CHILD | WS_VISIBLE, rect );		

		m_PackerView.Create( IDD_PACKINGFORMVIEW, this );
		m_PackerView.ShowWindow( SW_SHOW );	

		//m_WavepackageView.Create( IDD_WAVEPACKERVIEW, this );
		//m_WavepackageView.ShowWindow( SW_HIDE );	
		
		m_wndtabctrl.AddView( (CWnd*)&m_PackerView, "File package" );
		//m_wndtabctrl.AddView( (CWnd*)&m_WavepackageView, "Wave package" );

		this->m_Initialized = TRUE;
	}	
	
	DragAcceptFiles();

	InitDirectsound();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXPackerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CXPackerDlg::OnPaint() 
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
HCURSOR CXPackerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CXPackerDlg::OnClose() 
{
	if (CanExit())
		CDialog::OnClose();
}

void CXPackerDlg::OnOK() 
{
	if (CanExit())
		CDialog::OnOK();
}

void CXPackerDlg::OnCancel() 
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CXPackerDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
//	if (m_pAutoProxy != NULL)
//	{
//		ShowWindow(SW_HIDE);
//		return FALSE;
//	}

	return TRUE;
}

BOOL CXPackerDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) 
	{	
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;	
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CXPackerDlg::OnLoadbutton() 
{
	// TODO: Add your control notification handler code here

	if( m_wndtabctrl.m_uSelectedViewIndex != ID_TAB_FILEPACKER ) return;

	CFileDialog	FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL );

	FileDlg.m_ofn.hwndOwner		    = NULL;
	FileDlg.m_ofn.lStructSize		= sizeof(OPENFILENAME);
	FileDlg.m_ofn.lpstrFilter		= "XPS File(*.XPS)\0*.XPS\0";
	FileDlg.m_ofn.lpstrInitialDir	= ".\\";
	FileDlg.m_ofn.lpstrTitle		= "Load XPackage structure file";
	FileDlg.m_ofn.lpstrDefExt		= "*.XPS";
		
	if( FileDlg.DoModal() == IDOK )
	{			
		m_PackerView.loadXPP( FileDlg.GetPathName().operator LPCTSTR() );		
	}	

	
}

void CXPackerDlg::OnSavebutton() 
{
	if( m_wndtabctrl.m_uSelectedViewIndex != ID_TAB_FILEPACKER ) return;

	// TODO: Add your control notification handler code here
	CFileDialog	FileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL );

	FileDlg.m_ofn.hwndOwner		= NULL;
	FileDlg.m_ofn.lStructSize		= sizeof(OPENFILENAME);
	FileDlg.m_ofn.lpstrFilter		= "XPS File(*.XPS)\0*.XPS\0";
	FileDlg.m_ofn.lpstrInitialDir	= ".\\";
	FileDlg.m_ofn.lpstrTitle		= "Save XPackage structure file";
	FileDlg.m_ofn.lpstrDefExt		= "*.XPS";			
		
	if( FileDlg.DoModal() == IDOK )
	{		
		m_PackerView.savePackStructFile( FileDlg.GetPathName().operator LPCTSTR() );
	}		
	
}


void CXPackerDlg::OnResetbutton() 
{
	if( m_wndtabctrl.m_uSelectedViewIndex != ID_TAB_FILEPACKER ) return;

	// TODO: Add your control notification handler code here
	if( AfxMessageBox( "Reset package structure ?", MB_YESNO ) == IDYES )
	{
		m_PackerView.resetTree();
	}	
}

#include "Shlwapi.h"


BOOL CXPackerDlg::InsertSubForderFile(LPCTSTR lpszPath )
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
            if( !InsertSubForderFile(finder.GetFilePath() ) )
			{
				return FALSE;
			}
			
            continue;
        }

		if( !m_PackerView.insertFileItem( finder.GetFilePath() ) )
		{
			return FALSE;			
		}        
    }
	
	return TRUE;
}

void CXPackerDlg::OnDropFiles(HDROP hDropInfo) 
{
	if( m_wndtabctrl.m_uSelectedViewIndex != ID_TAB_FILEPACKER ) return;
	// TODO: Add your message handler code here and/or call default
	UINT        uNumFiles;
	TCHAR       szNextFile [MAX_PATH];		
	int         nIndex = m_PackerView.m_FileListCtrl.GetItemCount();
	SHFILEINFO  rFileInfo;

    // Get the # of files being dropped.

    uNumFiles = DragQueryFile ( hDropInfo, -1, NULL, 0 );

    for ( UINT uFile = 0; uFile < uNumFiles; uFile++ )
    {
		// Get the next filename from the HDROP info.
		if ( DragQueryFile ( hDropInfo, uFile, szNextFile, MAX_PATH ) > 0 )
		{
			SHGetFileInfo ( szNextFile, 0, &rFileInfo, sizeof(rFileInfo),
                            SHGFI_SYSICONINDEX | SHGFI_ATTRIBUTES | SHGFI_TYPENAME );
			
			if( !( rFileInfo.dwAttributes & SFGAO_FOLDER  ) )
			{
				if( !m_PackerView.insertFileItem( szNextFile ) )
				{
					break;
				}
			}			
			else	// sub folder....
			{
				InsertSubForderFile( szNextFile );
			}
        }
    }   // end for

    // Resize columns so all text is visible.
    
    m_PackerView.m_FileListCtrl.EnsureVisible ( nIndex-1, FALSE );

	m_PackerView.setCurrentFileItem();

    //DragFinish ( hDropInfo );
	
	CDialog::OnDropFiles(hDropInfo);
}

void CXPackerDlg::OnAboutbutton() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg aboutdlg;

	aboutdlg.DoModal();
}

void CXPackerDlg::OnExitbutton() 
{
	// TODO: Add your control notification handler code here

	DSoundRelease();

	DestroyWindow();
}
