// TcpService.cpp: implementation of the TcpService class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WWIIStarter.h"
#include "WWIIStarterDlg.h"

#include "TcpService.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TcpService::TcpService()
{
	
}

TcpService::~TcpService()
{

}


HWND TcpService::GetServiceHandle()
{
	
	CWWIIStarterApp * papp = (CWWIIStarterApp*)AfxGetApp();
	return papp->m_pMainWnd->GetSafeHwnd();
	
	//return ((CMainFrame*)AfxGetMainWnd ())->GetSafeHwnd ();
}

