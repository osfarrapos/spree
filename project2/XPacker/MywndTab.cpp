// MywndTab.cpp: implementation of the CMywndTab class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XPacker.h"
#include "XPackerDlg.h"
#include "MywndTab.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMywndTab::CMywndTab()
{

}

CMywndTab::~CMywndTab()
{

}

void CMywndTab::SelectView( UINT nIndex )
{
	CXPackerDlg* pMainFrame = (CXPackerDlg*)AfxGetMainWnd();	
	CwndTab::SelectView( nIndex );

	if( pMainFrame )
	{		
		if( nIndex == ID_TAB_FILEPACKER )
		{
			(pMainFrame->GetDlgItem( IDC_RESETBUTTON ))->EnableWindow( TRUE );
			(pMainFrame->GetDlgItem( IDC_LOADBUTTON ))->EnableWindow( TRUE );
			(pMainFrame->GetDlgItem( IDC_SAVEBUTTON ))->EnableWindow( TRUE );
		}
		else
		{
			(pMainFrame->GetDlgItem( IDC_RESETBUTTON ))->EnableWindow( FALSE );
			(pMainFrame->GetDlgItem( IDC_LOADBUTTON ))->EnableWindow( FALSE );
			(pMainFrame->GetDlgItem( IDC_SAVEBUTTON ))->EnableWindow( FALSE );
		}
	}
}
