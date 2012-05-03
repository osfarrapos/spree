// NewSectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XPacker.h"
#include "NewSectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewSectionDlg dialog


CNewSectionDlg::CNewSectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewSectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewSectionDlg)
	m_SectionNameString = _T("");
	//}}AFX_DATA_INIT
}


void CNewSectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewSectionDlg)
	DDX_Text(pDX, IDC_SECTIONEDIT, m_SectionNameString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewSectionDlg, CDialog)
	//{{AFX_MSG_MAP(CNewSectionDlg)
	ON_EN_CHANGE(IDC_SECTIONEDIT, OnChangeSectionedit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewSectionDlg message handlers

void CNewSectionDlg::OnChangeSectionedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );	
}

void CNewSectionDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( this->m_SectionNameString.GetLength() < 4 )
	{
		AfxMessageBox( "Section 이름을 4자이상 입력하세요" );	
	}
	else CDialog::OnOK();
}
