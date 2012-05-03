#if !defined(AFX_SEVERMESSAGEDIALOG_H__D105C17A_538A_4006_A872_65D5E35A7752__INCLUDED_)
#define AFX_SEVERMESSAGEDIALOG_H__D105C17A_538A_4006_A872_65D5E35A7752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeverMessageDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSeverMessageDialog dialog

#include "DIB.h"

class CSeverMessageDialog : public CDialog
{
// Construction
public:
	CSeverMessageDialog(CWnd* pParent = NULL);   // standard constructor

	CDIB			m_BackgroundImage;

// Dialog Data
	//{{AFX_DATA(CSeverMessageDialog)
	enum { IDD = IDD_SERVERMESSAGEBOX };
	CString	m_MessageString;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeverMessageDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeverMessageDialog)		
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEVERMESSAGEDIALOG_H__D105C17A_538A_4006_A872_65D5E35A7752__INCLUDED_)
