// SetRegistry.h : main header file for the SETREGISTRY application
//

#if !defined(AFX_SETREGISTRY_H__A5BB01A3_DF62_4D27_AFA2_8800693B632E__INCLUDED_)
#define AFX_SETREGISTRY_H__A5BB01A3_DF62_4D27_AFA2_8800693B632E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSetRegistryApp:
// See SetRegistry.cpp for the implementation of this class
//

class CSetRegistryApp : public CWinApp
{
public:
	CSetRegistryApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetRegistryApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSetRegistryApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETREGISTRY_H__A5BB01A3_DF62_4D27_AFA2_8800693B632E__INCLUDED_)
