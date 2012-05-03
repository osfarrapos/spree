// XLauncher.h : main header file for the XLAUNCHER application
//

#if !defined(AFX_XLAUNCHER_H__C5560F13_D949_45C3_93B0_EB6AC00D4C2B__INCLUDED_)
#define AFX_XLAUNCHER_H__C5560F13_D949_45C3_93B0_EB6AC00D4C2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "idispimp.h"

/////////////////////////////////////////////////////////////////////////////
// CXLauncherApp:
// See XLauncher.cpp for the implementation of this class
//
#ifndef _DEBUG
#define _SELFPATCH_
#endif
#define _USE_MUTEX

class CXLauncherApp : public CWinApp
{
public:
	CXLauncherApp();
	~CXLauncherApp();
	
	class CImpIDispatch* m_pDispOM;

	void ParseCommandLine2(CCommandLineInfo& rCmdInfo);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXLauncherApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXLauncherApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CXLauncherApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XLAUNCHER_H__C5560F13_D949_45C3_93B0_EB6AC00D4C2B__INCLUDED_)
