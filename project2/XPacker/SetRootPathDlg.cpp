// SetRootPathDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XPacker.h"
#include "SetRootPathDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetRootPathDlg dialog


CSetRootPathDlg::CSetRootPathDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRootPathDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetRootPathDlg)
	m_RootPath = _T("");
	//}}AFX_DATA_INIT
}


void CSetRootPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRootPathDlg)
	DDX_Text(pDX, IDC_PACKAGENAMEEDIT, m_RootPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRootPathDlg, CDialog)
	//{{AFX_MSG_MAP(CSetRootPathDlg)
	ON_EN_CHANGE(IDC_PACKAGENAMEEDIT, OnChangePackagenameedit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRootPathDlg message handlers

void CSetRootPathDlg::OnChangePackagenameedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
}
