// XPacker.h : main header file for the XPACKER application
//

#if !defined(AFX_XPACKER_H__50B2B8FA_6BAA_11D5_A852_00022A009AE8__INCLUDED_)
#define AFX_XPACKER_H__50B2B8FA_6BAA_11D5_A852_00022A009AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXPackerApp:
// See XPacker.cpp for the implementation of this class
//

class CXPackerApp : public CWinApp
{
public:
	CXPackerApp();

	CCommandLineInfo cmdInfo;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXPackerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXPackerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XPACKER_H__50B2B8FA_6BAA_11D5_A852_00022A009AE8__INCLUDED_)
