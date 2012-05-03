// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__50B2B8FE_6BAA_11D5_A852_00022A009AE8__INCLUDED_)
#define AFX_DLGPROXY_H__50B2B8FE_6BAA_11D5_A852_00022A009AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXPackerDlg;

/////////////////////////////////////////////////////////////////////////////
// CXPackerDlgAutoProxy command target

class CXPackerDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CXPackerDlgAutoProxy)

	CXPackerDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CXPackerDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXPackerDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CXPackerDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CXPackerDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CXPackerDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CXPackerDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__50B2B8FE_6BAA_11D5_A852_00022A009AE8__INCLUDED_)
