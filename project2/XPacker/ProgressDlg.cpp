// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog


CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDlg)
	m_ProcessSectionString = _T("...");
	m_PercentageString = _T("0%");
	//}}AFX_DATA_INIT
}


void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_Progressctrl);
	DDX_Text(pDX, IDC_PROCESSINGSECTIONSTATIC, m_ProcessSectionString);
	DDX_Text(pDX, IDC_PROGRESSSTATIC, m_PercentageString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CProgressDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg message handlers

BOOL CProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CenterWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgressDlg::setData( int totalcount, LPSTR Processstring )
{
	 TotalCount = totalcount;
	 m_ProcessSectionString = _T( Processstring );
	 m_PercentageString.Format( "0/%d", TotalCount );

	 this->m_Progressctrl.SetRange32( 0, TotalCount );

}

void CProgressDlg::setProgressRate( int nowprogressrate, LPSTR Processstring )
{
	 m_ProcessSectionString = _T( Processstring );

	 m_PercentageString.Format( "%d/%d", nowprogressrate, TotalCount );

	 this->m_Progressctrl.SetPos( nowprogressrate ); 

	 UpdateData( FALSE );
}

BOOL CProgressDlg::Create( CWnd* pParentWnd ) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd); 
}
