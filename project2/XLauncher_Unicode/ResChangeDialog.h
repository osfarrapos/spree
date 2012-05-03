#if !defined(AFX_RESCHANGEDIALOG_H__724CD0EB_1FD5_4B30_BEE7_CDFA6F66B7A9__INCLUDED_)
#define AFX_RESCHANGEDIALOG_H__724CD0EB_1FD5_4B30_BEE7_CDFA6F66B7A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResChangeDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResChangeDialog dialog

#include "BtnST.h"
#include "ShadeButtonST.h"
#include "Label.h"
#include "BitmapDialog.h"
#include "BitComboBox.h"
#include "MacButtons.h"

class CResChangeDialog : public CBitmapDialog
{
// Construction
public:
	CResChangeDialog(CWnd* pParent = NULL);   // standard constructor

	bool FindBestFullscreenMode( bool bRequireHAL, bool bRequireREF );

	BOOL m_regfullscreenmode;
	int	 m_screenwidth;
	int  m_screenheight;
	int  m_screenrefreshrate;
	
// Dialog Data
	//{{AFX_DATA(CResChangeDialog)
	enum { IDD = IDD_RESOPTIONDIALOG };
	CLabel	m_Title;
	CBitComboBox	m_WindowModeComboBox;
	CLabel	m_WindowModeStatic;	
	CLabel	m_RefreshRate;
	CLabel	m_ResolutionStatic;
	CBitComboBox	m_RefreshRateCombo;
	CBitComboBox	m_ResolutionCombo;
	CShadeButtonST	m_CancelButton;
	CShadeButtonST	m_OKButton;
	int		m_FullScreenMode;
	int		m_ResolutionIndex;
	int		m_RefreshRateIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResChangeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResChangeDialog)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokWindowmodecombo();
	afx_msg void OnSelendokResolutionCombo();
	afx_msg void OnSelendokRefreshratecombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESCHANGEDIALOG_H__724CD0EB_1FD5_4B30_BEE7_CDFA6F66B7A9__INCLUDED_)
