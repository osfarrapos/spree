#if !defined(AFX_NOTINSTALLEDDIALOG_H__9CADA5E8_8B03_45C6_8A9C_DD82FA10DB6E__INCLUDED_)
#define AFX_NOTINSTALLEDDIALOG_H__9CADA5E8_8B03_45C6_8A9C_DD82FA10DB6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotInstalledDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNotInstalledDialog dialog

class CNotInstalledDialog : public CDialog
{
// Construction
public:
	CNotInstalledDialog(CWnd* pParent = NULL);   // standard constructor

	static void SetDefaultRegistryInformations( void );

// Dialog Data
	//{{AFX_DATA(CNotInstalledDialog)
	enum { IDD = IDD_NOTINSTALLEDBOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	int m_count;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotInstalledDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNotInstalledDialog)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTINSTALLEDDIALOG_H__9CADA5E8_8B03_45C6_8A9C_DD82FA10DB6E__INCLUDED_)
