#if !defined(AFX_SETROOTPATHDLG_H__B6D54784_EF37_47E6_A59E_0A5674D7591C__INCLUDED_)
#define AFX_SETROOTPATHDLG_H__B6D54784_EF37_47E6_A59E_0A5674D7591C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetRootPathDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetRootPathDlg dialog

class CSetRootPathDlg : public CDialog
{
// Construction
public:
	CSetRootPathDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetRootPathDlg)
	enum { IDD = IDD_ROOTPATHDIALOG };
	CString	m_RootPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetRootPathDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetRootPathDlg)
	afx_msg void OnChangePackagenameedit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETROOTPATHDLG_H__B6D54784_EF37_47E6_A59E_0A5674D7591C__INCLUDED_)
