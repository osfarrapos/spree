// SeverMessageDialog.cpp : implementation file
//

#include "stdafx.h"
#include "XLauncher.h"
#include "SeverMessageDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeverMessageDialog dialog


CSeverMessageDialog::CSeverMessageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSeverMessageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeverMessageDialog)
	m_MessageString = _T("");
	//}}AFX_DATA_INIT
}


void CSeverMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeverMessageDialog)
	DDX_Text(pDX, IDC_MESSAGEEDIT, m_MessageString);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeverMessageDialog, CDialog)
	//{{AFX_MSG_MAP(CSeverMessageDialog)	
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(ID_CONNECTIONSETTINGSBUTTON, OnConnectionsettingsbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeverMessageDialog message handlers




void CSeverMessageDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CSeverMessageDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CSeverMessageDialog::OnInitDialog()  
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSeverMessageDialog::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default	
	return CDialog::OnEraseBkgnd(pDC);
}

void CSeverMessageDialog::OnConnectionsettingsbutton() 
{
	// TODO: Add your control notification handler code here
	optionsdlg.DoModal();	
}
