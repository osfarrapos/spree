// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "XPacker.h"
#include "DlgProxy.h"
#include "XPackerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXPackerDlgAutoProxy

IMPLEMENT_DYNCREATE(CXPackerDlgAutoProxy, CCmdTarget)

CXPackerDlgAutoProxy::CXPackerDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CXPackerDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CXPackerDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CXPackerDlgAutoProxy::~CXPackerDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CXPackerDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CXPackerDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CXPackerDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CXPackerDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CXPackerDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IXPacker to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {50B2B8F7-6BAA-11D5-A852-00022A009AE8}
static const IID IID_IXPacker =
{ 0x50b2b8f7, 0x6baa, 0x11d5, { 0xa8, 0x52, 0x0, 0x2, 0x2a, 0x0, 0x9a, 0xe8 } };

BEGIN_INTERFACE_MAP(CXPackerDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CXPackerDlgAutoProxy, IID_IXPacker, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {50B2B8F5-6BAA-11D5-A852-00022A009AE8}
IMPLEMENT_OLECREATE2(CXPackerDlgAutoProxy, "XPacker.Application", 0x50b2b8f5, 0x6baa, 0x11d5, 0xa8, 0x52, 0x0, 0x2, 0x2a, 0x0, 0x9a, 0xe8)

/////////////////////////////////////////////////////////////////////////////
// CXPackerDlgAutoProxy message handlers
