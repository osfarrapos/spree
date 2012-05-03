#if !defined(AFX_ADDSERVERDLG_H__8AD6D699_16CB_46B7_99E4_E3951D850C7B__INCLUDED_)
#define AFX_ADDSERVERDLG_H__8AD6D699_16CB_46B7_99E4_E3951D850C7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddServerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddServerDlg dialog

class CAddServerDlg : public CDialog
{
// Construction
public:
	CAddServerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddServerDlg)
	enum { IDD = IDD_ADDSERVERITEM };
	CString	m_ServerName;
	CString	m_AdressString;
	CString	m_PatchServerIPString;
	CString	m_DownloadAdress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddServerDlg)
	virtual void OnOK();
	afx_msg void OnChangeServernameedit();
	afx_msg void OnChangeServeraddressedit();
	afx_msg void OnChangePatchserveraddressedit();
	afx_msg void OnChangeDownloadadressedit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDSERVERDLG_H__8AD6D699_16CB_46B7_99E4_E3951D850C7B__INCLUDED_)
