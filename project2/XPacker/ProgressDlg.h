#if !defined(AFX_PROGRESSDLG_H__66F338A1_40E2_11D5_BE89_0000E8ED33D3__INCLUDED_)
#define AFX_PROGRESSDLG_H__66F338A1_40E2_11D5_BE89_0000E8ED33D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

#include "resource.h"

class CProgressDlg : public CDialog
{
// Construction
public:
	CProgressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressDlg)
	enum { IDD = IDD_PROGRESSDIALOG };
	CProgressCtrl	m_Progressctrl;
	CString	m_ProcessSectionString;
	CString	m_PercentageString;
	//}}AFX_DATA

	int TotalCount;
	void setData( int totalcount, LPSTR Processstring );

	void setProgressRate( int nowprogressrate, LPSTR Processstring );


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDlg)
	public:
	virtual BOOL Create( CWnd* pParentWnd );
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLG_H__66F338A1_40E2_11D5_BE89_0000E8ED33D3__INCLUDED_)
