// NewPackageFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XPacker.h"
#include "NewPackageFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewPackageFileDlg dialog


CNewPackageFileDlg::CNewPackageFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewPackageFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewPackageFileDlg)
	m_PackageName = _T("");	
	//}}AFX_DATA_INIT
}


void CNewPackageFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPackageFileDlg)
	DDX_Text(pDX, IDC_PACKAGENAMEEDIT, m_PackageName);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewPackageFileDlg, CDialog)
	//{{AFX_MSG_MAP(CNewPackageFileDlg)
	ON_EN_CHANGE(IDC_PACKAGENAMEEDIT, OnChangePackagenameedit)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPackageFileDlg message handlers

void CNewPackageFileDlg::OnChangePackagenameedit()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
}

void CNewPackageFileDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( m_PackageName.GetLength() < 4 )
	{
		AfxMessageBox( "Package와 파일 이름을 4자이상 입력하세요" );
	}
	else CDialog::OnOK();
}

