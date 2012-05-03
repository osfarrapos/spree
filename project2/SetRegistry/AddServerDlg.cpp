// AddServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetRegistry.h"
#include "AddServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddServerDlg dialog


CAddServerDlg::CAddServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddServerDlg)
	m_ServerName = _T("");
	m_AdressString = _T("");
	m_PatchServerIPString = _T("");
	m_DownloadAdress = _T("");
	//}}AFX_DATA_INIT
}


void CAddServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddServerDlg)
	DDX_Text(pDX, IDC_SERVERNAMEEDIT, m_ServerName);
	DDV_MaxChars(pDX, m_ServerName, 49);
	DDX_Text(pDX, IDC_SERVERADDRESSEDIT, m_AdressString);
	DDV_MaxChars(pDX, m_AdressString, 49);
	DDX_Text(pDX, IDC_PATCHSERVERADDRESSEDIT, m_PatchServerIPString);
	DDX_Text(pDX, IDC_DOWNLOADADRESSEDIT, m_DownloadAdress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddServerDlg, CDialog)
	//{{AFX_MSG_MAP(CAddServerDlg)
	ON_EN_CHANGE(IDC_SERVERNAMEEDIT, OnChangeServernameedit)
	ON_EN_CHANGE(IDC_SERVERADDRESSEDIT, OnChangeServeraddressedit)
	ON_EN_CHANGE(IDC_PATCHSERVERADDRESSEDIT, OnChangePatchserveraddressedit)
	ON_EN_CHANGE(IDC_DOWNLOADADRESSEDIT, OnChangeDownloadadressedit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddServerDlg message handlers

void CAddServerDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CAddServerDlg::OnChangeServernameedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CAddServerDlg::OnChangeServeraddressedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CAddServerDlg::OnChangePatchserveraddressedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CAddServerDlg::OnChangeDownloadadressedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
}
