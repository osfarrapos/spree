// CheckVersionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xlauncher.h"
#include "CheckVersionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckVersionDlg dialog


extern TCHAR g_InterfaceString[100][1024];

CCheckVersionDlg::CCheckVersionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckVersionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckVersionDlg)
	m_CheckVersionString = g_InterfaceString[28];//.LoadString( IDS_CHECKVERSIONSTRING );
	//}}AFX_DATA_INIT
}


void CCheckVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckVersionDlg)
	DDX_Text(pDX, IDC_CHECKVERSIONSTATIC, m_CheckVersionString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckVersionDlg, CDialog)
	//{{AFX_MSG_MAP(CCheckVersionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckVersionDlg message handlers

BOOL CCheckVersionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	m_CheckVersionString = g_InterfaceString[28];//.LoadString( IDS_CHECKVERSIONSTRING );
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
