#if !defined(AFX_CHECKVERSIONDLG_H__92349906_F72F_4E01_A146_F6096A1F7E25__INCLUDED_)
#define AFX_CHECKVERSIONDLG_H__92349906_F72F_4E01_A146_F6096A1F7E25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckVersionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckVersionDlg dialog

class CCheckVersionDlg : public CDialog
{
// Construction
public:
	CCheckVersionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCheckVersionDlg)
	enum { IDD = IDD_PATCHNUMBERSERVERCONNECTDIALOG };
	CString	m_CheckVersionString;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckVersionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCheckVersionDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKVERSIONDLG_H__92349906_F72F_4E01_A146_F6096A1F7E25__INCLUDED_)
