// XPackerDlg.h : header file
//

#if !defined(AFX_XPACKERDLG_H__50B2B8FC_6BAA_11D5_A852_00022A009AE8__INCLUDED_)
#define AFX_XPACKERDLG_H__50B2B8FC_6BAA_11D5_A852_00022A009AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "mywndTab.h"

#include "PackingView.h"
#include "WavePackageView.h"


class CXPackerDlgAutoProxy;

#define ID_TAB_FILEPACKER         0
#define ID_TAB_WAVEPACKER         1

/////////////////////////////////////////////////////////////////////////////
// CXPackerDlg dialog

class CXPackerDlg : public CDialog
{
	DECLARE_DYNAMIC(CXPackerDlg);
	//friend class CXPackerDlgAutoProxy;

// Construction
public:
	CXPackerDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CXPackerDlg();

	CMywndTab     m_wndtabctrl;
	CPackingView  m_PackerView;

	CWavePackageView m_WavepackageView;

	BOOL m_Initialized;	

	BOOL InsertSubForderFile(LPCTSTR lpszPath );

// Dialog Data
	//{{AFX_DATA(CXPackerDlg)
	enum { IDD = IDD_XPACKER_DIALOG };
	CString	m_LoadedFilename;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXPackerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//CXPackerDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	//{{AFX_MSG(CXPackerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLoadbutton();
	afx_msg void OnResetbutton();
	afx_msg void OnSavebutton();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnAboutbutton();
	afx_msg void OnExitbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XPACKERDLG_H__50B2B8FC_6BAA_11D5_A852_00022A009AE8__INCLUDED_)
