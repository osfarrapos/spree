#if !defined(AFX_SELECTSERVER_H__03F5ED09_96FF_43F9_A550_8BFC31B322CB__INCLUDED_)
#define AFX_SELECTSERVER_H__03F5ED09_96FF_43F9_A550_8BFC31B322CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectServer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectServer dialog

class CSelectServer : public CDialog
{
// Construction
public:
	CSelectServer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectServer)
	enum { IDD = IDD_SELECTSERVERDIALOG };
	CListBox	m_ServerList;
	int		m_SelectedServer;
	int		m_PatchNumber;
	int		m_CurrentPatchversion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectServer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectServer)
	afx_msg void OnSelchangeServerlist();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit1();
	afx_msg void OnDeltaposPatchnumberspin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTSERVER_H__03F5ED09_96FF_43F9_A550_8BFC31B322CB__INCLUDED_)
