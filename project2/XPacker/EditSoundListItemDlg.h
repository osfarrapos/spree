#if !defined(AFX_EDITSOUNDLISTITEMDLG_H__84D485B0_B38C_492A_BC2C_2A6B7FFC0C98__INCLUDED_)
#define AFX_EDITSOUNDLISTITEMDLG_H__84D485B0_B38C_492A_BC2C_2A6B7FFC0C98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditSoundListItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditSoundListItemDlg dialog

class CEditSoundListItemDlg : public CDialog
{
// Construction
public:
	CEditSoundListItemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditSoundListItemDlg)
	enum { IDD = IDD_EDITSOUNDLISTDIALOG };
	CString	m_EnvironmentString;
	CString	m_EnvironmentlinkString;
	CString	m_SoundNameString;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditSoundListItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditSoundListItemDlg)
	afx_msg void OnChangeSoundnameedit();
	afx_msg void OnChangeEnvironmentedit();
	afx_msg void OnChangeEnvironmentlinkedit();
	afx_msg void OnSelendokSoundnameedit();
	afx_msg void OnEditchangeSoundnameedit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSOUNDLISTITEMDLG_H__84D485B0_B38C_492A_BC2C_2A6B7FFC0C98__INCLUDED_)
