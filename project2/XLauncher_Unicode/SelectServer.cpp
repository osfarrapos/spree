// SelectServer.cpp : implementation file
//

#include "stdafx.h"
#include "xlauncher.h"
#include "SelectServer.h"
#include "XRegistry.h"
#include "XLauncherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectServer dialog


CSelectServer::CSelectServer(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectServer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectServer)
	m_SelectedServer = -1;
	m_PatchNumber = 0;
	m_CurrentPatchversion = 0;
	//}}AFX_DATA_INIT
}


void CSelectServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectServer)
	DDX_Control(pDX, IDC_SERVERLIST, m_ServerList);
	DDX_LBIndex(pDX, IDC_SERVERLIST, m_SelectedServer);
	DDX_Text(pDX, IDC_EDIT1, m_PatchNumber);
	DDX_Text(pDX, IDC_EDIT2, m_CurrentPatchversion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectServer, CDialog)
	//{{AFX_MSG_MAP(CSelectServer)
	ON_LBN_SELCHANGE(IDC_SERVERLIST, OnSelchangeServerlist)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PATCHNUMBERSPIN, OnDeltaposPatchnumberspin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectServer message handlers

void CSelectServer::OnSelchangeServerlist() 
{
	UpdateData( TRUE );
}

BOOL CSelectServer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ServerList.AddString( _T("9Dragons main server") );
	m_ServerList.AddString( _T("9Dragons test server") );
	m_SelectedServer = 1;
	UpdateData( FALSE );

	g_PatchVersionManager.GetVersionInfo();	
	m_CurrentPatchversion = m_PatchNumber = g_PatchVersionManager.m_VersionInfo.patchversion_1;
	UpdateData( FALSE );
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectServer::OnChangeEdit1() 
{
	UpdateData();	
}

void CSelectServer::OnDeltaposPatchnumberspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if( pNMUpDown->iDelta == -1 )
	{
		m_PatchNumber++;
	}		
	else if( pNMUpDown->iDelta == 1 )
	{
		m_PatchNumber--;
		if( m_PatchNumber < 0 ) m_PatchNumber = 0;
	}

	g_PatchVersionManager.GetVersionInfo();
	g_PatchVersionManager.SetPatchNumber( m_PatchNumber );
	//g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHNUMBER, m_PatchNumber );

	UpdateData( FALSE );
	
	*pResult = 0;
}
