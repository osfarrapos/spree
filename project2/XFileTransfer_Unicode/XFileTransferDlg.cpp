// XFileTransferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XFileTransfer.h"
#include "XFileTransferDlg.h"
#include "Registry.h"
#include "RegistryDef.h"
#include "xzip.h"
#include <tchar.h>
#include <direct.h>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

TCHAR g_InterfaceString[100][1024];

static TCHAR * _xlstrrchr(LPCTSTR string, int ch)
{
	TCHAR *start = (TCHAR *)string;
	
	while (*string++)                       /* find end of string */
		;
	/* search towards front */
	while (--string != start && *string != (TCHAR) ch)
		;
	
	if (*string == (TCHAR) ch)                /* char found ? */
		return (TCHAR *)string;
	
	return NULL;
}

static TCHAR * GetFilePart(LPCTSTR source)
{
	TCHAR *result = _xlstrrchr(source, _T('\\'));
	if (result)
		result++;
	else
		result = (TCHAR *)source;
	return result;
}

BOOL BackupFile(LPCTSTR lpszFile)
{
	ASSERT(lpszFile);
	
	// if file doesn't exist, nothing to do
	if (_taccess(lpszFile, 00) == -1)
		return TRUE;
	
	// file exists, so make a backup copy
	
	CString strFileRoot = lpszFile;
	
	CString strPath = _T("");
	CString strExt = _T("");
	BOOL bSuccess = FALSE;
	for (int i = 1; i <= 9999; i++)
	{
		strExt.Format(_T(".%04d"), i);
		strPath = strFileRoot;
		strPath += strExt;
		if (_taccess(strPath, 00) == -1)
		{
			// file doesn't exist, so we can use it
			bSuccess = CopyFile(lpszFile, strPath, TRUE);
			break;
		}
	}
	
	return bSuccess;
}

BOOL CheckExistFile( LPCTSTR filename, BOOL resetAttrib )
{
	HANDLE      hFind;
	WIN32_FIND_DATA rFind;	
	
	hFind = FindFirstFile ( filename, &rFind );
	
	if ( INVALID_HANDLE_VALUE == hFind )
	{
		FindClose ( hFind );
		return FALSE;
	}
	
	FindClose ( hFind );
	
	if( resetAttrib )
	{
		SetFileAttributes( filename, FILE_ATTRIBUTE_ARCHIVE );
		/*CFileStatus filestatus;
		CFile::GetStatus( filename, filestatus );
		filestatus.m_attribute = CFile::Attribute::normal; // normal file attrib setting
		CFile::SetStatus( filename, filestatus );*/
	}
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXFileTransferDlg dialog

CXFileTransferDlg::CXFileTransferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXFileTransferDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXFileTransferDlg)
	m_SenderID = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXFileTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXFileTransferDlg)
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDC_IDSTATIC, m_IDStatic);
	DDX_Control(pDX, IDC_NOTICESTATIC, m_NoticeStatic);
	DDX_Control(pDX, IDC_TRANSFERSTATIC, m_TransferStatic);
	DDX_Control(pDX, IDC_PROGRESSBAR, m_Progress);
	DDX_Text(pDX, IDC_SENDERIDEDIT, m_SenderID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXFileTransferDlg, CDialog)
	//{{AFX_MSG_MAP(CXFileTransferDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_SENDERIDEDIT, OnChangeSenderidedit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXFileTransferDlg message handlers

BOOL CXFileTransferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LoadString();

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

	m_OKButton.SetWindowText( g_InterfaceString[7] );

	#ifdef _XJAPANESE
	m_OKButton.ShowWindow( SW_HIDE );
	CompressLogFile();
	#endif

	m_CancelButton.SetWindowText( g_InterfaceString[8] );

	m_NoticeStatic.SetWindowText( g_InterfaceString[0] );
	m_IDStatic.SetWindowText( g_InterfaceString[1] );

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	if(m_FTPTransfer.Initialze() == FALSE)
	{
		AfxMessageBox( g_InterfaceString[6] );//_T("서버 주소를 읽을 수 없습니다."));
		return FALSE;
	}

	TCHAR		registrybuffer[_REGSTRING_MAXLENGTH];
	memset(registrybuffer, 0, sizeof(TCHAR) * _REGSTRING_MAXLENGTH);
	if(g_RegManager._XRegReadString(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTUSERNAME, _T("UNKNOWNUSER"), registrybuffer, _REGSTRING_MAXLENGTH, FALSE))
	{
		m_SenderID = registrybuffer;
	}
	else
		m_SenderID = _T("UNKNOWNUSER");

	//OutputDebugString( m_SenderID );
		
	
	UpdateData( FALSE );

	m_Progress.SetRange(0, 100);

#if defined(_XKOREAN) || defined(_XJAPANESE)
	CWnd* pWnd = GetDlgItem( IDC_IDSTATIC );
	if( pWnd )
	{
		pWnd->ShowWindow( SW_HIDE );
	}

	pWnd = GetDlgItem( IDC_SENDERIDEDIT );
	if( pWnd )
	{
		pWnd->ShowWindow( SW_HIDE );
	}
#endif
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXFileTransferDlg::OnPaint() 
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
HCURSOR CXFileTransferDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CXFileTransferDlg::CompressLogFile( void )
{
	TCHAR		registrybuffer[_REGSTRING_MAXLENGTH];
	memset(registrybuffer, 0, _REGSTRING_MAXLENGTH);
	if(!g_RegManager._XRegReadString(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_INSTALLEDPATH, _T(""), registrybuffer, _REGSTRING_MAXLENGTH, TRUE))		
	{
		GetCurrentDirectory( _REGSTRING_MAXLENGTH, registrybuffer );
	}

	_wchdir( registrybuffer );

	BackupFile( _T("Log.xl") );

	HZIP hZip = NULL;
	TCHAR logfilename[4][32] = { _T("ND_CRASHLOG.HTM"),_T("ND_DUMP.BIN"),_T("EOD.Log"),_T("XLauncher.log") };
	int nFiles = 0;
	
	for (int i = 0; i < 4; i++)
	{
		if (!hZip)
		{
			hZip = CreateZip((LPVOID)(LPCTSTR)_T("Log.xl"), 0, ZIP_FILENAME);
			
			if (!hZip)
			{
				AfxMessageBox(_T("Failed to create log.xl"));
				return FALSE;
			}
		}

		ZRESULT zr = ZipAdd(hZip, logfilename[i], (LPVOID)(LPCTSTR)logfilename[i], 0, ZIP_FILENAME);
		if (zr == ZR_OK)
		{
			nFiles++;
		}
		else
		{
			CString message;
			message.Format(_T("ERROR - failed to add '%s' to zip\n"), logfilename[i]);
		}
	}

	if (hZip) CloseZip(hZip);
	return TRUE;
}

void CXFileTransferDlg::OnOK() 
{

#ifdef _XKOREAN

	CString urlstring;
	urlstring = g_InterfaceString[9];
	::ShellExecute(NULL, _T("open"), urlstring, NULL, NULL, SW_SHOWNORMAL);	

#else

	TCHAR szModuleName[MAX_PATH*2];
	ZeroMemory(szModuleName, sizeof(szModuleName));
	if (GetModuleFileName(0, szModuleName, _countof(szModuleName)-2) <= 0)
		lstrcpy(szModuleName, _T("Unknown"));
	
	TCHAR *pszFilePart = GetFilePart(szModuleName);
	pszFilePart[0] = NULL;

	_wchdir( szModuleName );

	m_Progress.ShowWindow(TRUE);
	m_TransferStatic.ShowWindow(TRUE);
	m_TransferStatic.SetWindowText(_T("Compressing..."));
	m_TransferStatic.RedrawWindow();
	CompressLogFile();
	m_Progress.SetPos(30);

	// TODO: Add extra validation here
	m_Progress.ShowWindow(TRUE);
	m_TransferStatic.ShowWindow(TRUE);
	m_TransferStatic.SetWindowText( g_InterfaceString[2] ); //_T("파일을 전송중입니다.")

	if(m_FTPTransfer.ConnetServer() == FALSE)
	{
		AfxMessageBox(g_InterfaceString[3]); //_T("인터넷 연결에 실패했습니다.")
		goto exit;
	}
	m_Progress.SetPos(50);
	
	m_FTPTransfer.SetServerPath( m_SenderID );
	m_Progress.SetPos(55);

	// Look for File Path
	TCHAR		registrybuffer[_REGSTRING_MAXLENGTH];
	memset(registrybuffer, 0, _REGSTRING_MAXLENGTH);
	if(g_RegManager._XRegReadString(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_INSTALLEDPATH, _T(""), registrybuffer, _REGSTRING_MAXLENGTH, TRUE))
	{
		m_FTPTransfer.SetInputSpec(registrybuffer);
	}
	else
	{
		AfxMessageBox(g_InterfaceString[4]); //_T("파일 경로를 찾지 못했습니다.")
		goto exit;
	}

	m_Progress.SetPos(60);

	// Send EOD.log
	m_FTPTransfer.ConcatInputSpec(_T("\\Log.xl"));
	m_FTPTransfer.SetOutputSpec(_T("./Log.xl"));
	m_FTPTransfer.SendFile();
	m_Progress.SetPos(100);

	m_TransferStatic.SetWindowText(g_InterfaceString[5]); //_T("파일 전송이 완료 되었습니다.")
	goto exit;

#endif
	
exit :
	CDialog::OnOK();
}

void CXFileTransferDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}



void CXFileTransferDlg::OnChangeSenderidedit() 
{
	UpdateData();
}


void CXFileTransferDlg::LoadString( void )
{
#ifdef _XVIETNAMESE
	HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_VIETNAMESEINTERFACETEXT),_T("Text"));
#else	
	#ifdef _XTAIWANESE
		HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_TAIWANESEINTERFACETEXT),_T("Text"));
	#else
		#ifdef _XENGLISH
			HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_ENGLISHINTERFACETEXT),_T("Text"));
		#else
			#ifdef _XJAPANESE
				HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_JAPANESEINTERFACETEXT),_T("Text"));
			#else
				#ifdef _XRUSSIAN
					HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_RUSSIANINTERFACETEXT),_T("Text"));
				#else				
					HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_KOREANINTERFACETEXT),_T("Text"));
				#endif
			#endif
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

			if( szStr == _T("_XM_EOS") )
			{
				break;
			}
			
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
