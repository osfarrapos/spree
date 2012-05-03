// NotInstalledDialog.cpp : implementation file
//

#include "stdafx.h"
#include "XLauncher.h"
#include "NotInstalledDialog.h"
#include "XRegistry.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotInstalledDialog dialog


CNotInstalledDialog::CNotInstalledDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNotInstalledDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNotInstalledDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_count = 0;
}


void CNotInstalledDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNotInstalledDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNotInstalledDialog, CDialog)
	//{{AFX_MSG_MAP(CNotInstalledDialog)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotInstalledDialog message handlers


void CNotInstalledDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	m_count++;

	if( m_count == 10 ) 
	{
		SetDefaultRegistryInformations();
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CNotInstalledDialog::OnOK() 
{
	// TODO: Add extra validation here
	//SetDefaultRegistryInformations();
	CDialog::OnOK();
}


void CNotInstalledDialog::SetDefaultRegistryInformations( void )
{
	char currentPath[ 512 ];
	memset( currentPath , 0 , sizeof(char) * 512 );
	GetModuleFileNameA( NULL, currentPath, 511 );
	
	for( int i = 511; i > 0; i-- )
	{
		if( currentPath[i] == '\\' )
		{
			currentPath[i] = NULL;
			break;
		}
		else currentPath[i] = NULL;
	}				
	
	g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_EXECUTEFILE, "NineDragons.EXE" );
	g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_INSTALLEDPATH, currentPath );		
	g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PACKAGEVERSION, "OpenBetaX2" );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_PATCHNUMBER, 0 );
	
	
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_USEHARDWARECURSOR, TRUE );
	
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_TERRAIN, 3 );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_WORLDOBJECT, 3 );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_CHARACTER, 3 );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_MONSTER, 3 );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_NPC, 3 );
	
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_MIPLEVEL_DETAILTTILE, 1 );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_MIPLEVEL_WORLDOBJECT, 1 );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_MIPLEVEL_CHARACTER, 0 );
	
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_CHARACTERSHADOW, 2 );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_VIEWCHARACTERSHADOW, 1 );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_VIEWCLOUDSHADOW, 1 );	
	
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SIGHTLEVEL_RANGE, 1 );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SIGHTLEVEL_ANGLE, 1 );
	
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_ZDEPTHFORMAT, 2 );
	
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "InterfaceSoundEffect", 0 );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, "BGM", 1 );
	
	CString messageString;
	messageString.LoadString(IDS_SETREGISTRY);
	AfxMessageBox( messageString ); 

}