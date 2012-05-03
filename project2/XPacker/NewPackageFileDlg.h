#if !defined(AFX_NEWPACKAGEFILEDLG_H__50B2B911_6BAA_11D5_A852_00022A009AE8__INCLUDED_)
#define AFX_NEWPACKAGEFILEDLG_H__50B2B911_6BAA_11D5_A852_00022A009AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewPackageFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewPackageFileDlg dialog

class CNewPackageFileDlg : public CDialog
{
	BOOL autostringsetflag;
// Construction
public:
	CNewPackageFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewPackageFileDlg)
	enum { IDD = IDD_NEWPACKAGEFILEDIALOG };
	CString	m_PackageName;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewPackageFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewPackageFileDlg)
	afx_msg void OnChangePackagenameedit();	
	virtual void OnOK();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPACKAGEFILEDLG_H__50B2B911_6BAA_11D5_A852_00022A009AE8__INCLUDED_)
