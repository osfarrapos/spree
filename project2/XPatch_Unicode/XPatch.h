// XPatch.h : main header file for the XPatch application
//

#if !defined(AFX_XPATCH_H__B1806227_FD70_448D_972D_CE991648A132__INCLUDED_)
#define AFX_XPATCH_H__B1806227_FD70_448D_972D_CE991648A132__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXPatchApp:
// See XPatch.cpp for the implementation of this class
//

class CXPatchApp : public CWinApp
{
public:
	CXPatchApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXPatchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXPatchApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XPATCH_H__B1806227_FD70_448D_972D_CE991648A132__INCLUDED_)
