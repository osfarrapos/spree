// EditSoundListItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XPacker.h"
#include "EditSoundListItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditSoundListItemDlg dialog


CEditSoundListItemDlg::CEditSoundListItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditSoundListItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditSoundListItemDlg)	
	m_EnvironmentString = _T("");
	m_EnvironmentlinkString = _T("");
	m_SoundNameString = _T("");
	//}}AFX_DATA_INIT
}


void CEditSoundListItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditSoundListItemDlg)	
	DDX_Text(pDX, IDC_ENVIRONMENTEDIT, m_EnvironmentString);
	DDV_MaxChars(pDX, m_EnvironmentString, 30);
	DDX_Text(pDX, IDC_ENVIRONMENTLINKEDIT, m_EnvironmentlinkString);
	DDV_MaxChars(pDX, m_EnvironmentlinkString, 30);
	DDX_CBString(pDX, IDC_SOUNDNAMEEDIT, m_SoundNameString);
	DDV_MaxChars(pDX, m_SoundNameString, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditSoundListItemDlg, CDialog)
	//{{AFX_MSG_MAP(CEditSoundListItemDlg)	
	ON_EN_CHANGE(IDC_ENVIRONMENTEDIT, OnChangeEnvironmentedit)
	ON_EN_CHANGE(IDC_ENVIRONMENTLINKEDIT, OnChangeEnvironmentlinkedit)
	ON_CBN_SELENDOK(IDC_SOUNDNAMEEDIT, OnSelendokSoundnameedit)
	ON_CBN_EDITCHANGE(IDC_SOUNDNAMEEDIT, OnEditchangeSoundnameedit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditSoundListItemDlg message handlers


void CEditSoundListItemDlg::OnChangeEnvironmentedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
}

void CEditSoundListItemDlg::OnChangeEnvironmentlinkedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
}

void CEditSoundListItemDlg::OnSelendokSoundnameedit() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
}

void CEditSoundListItemDlg::OnEditchangeSoundnameedit() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );	
}
