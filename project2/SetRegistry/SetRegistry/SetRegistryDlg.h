// SetRegistryDlg.h : header file
//

#if !defined(AFX_SETREGISTRYDLG_H__C5CAF787_DA52_42DB_A892_6FBB9E06B361__INCLUDED_)
#define AFX_SETREGISTRYDLG_H__C5CAF787_DA52_42DB_A892_6FBB9E06B361__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSetRegistryDlg dialog

#include "XList.h"

#define _XDEF_MAXSERVERINFO	100

typedef struct
{
	TCHAR servername[50];
	TCHAR serveraddress[50];
	TCHAR patchserveraddress[50];
	TCHAR downloadserveraddress[100];
} _XDEF_SERVERINFOSTRUCT;

class _XServerInfoItem : public _XItem
{
public:
	_XDEF_SERVERINFOSTRUCT	si;
};

class CSetRegistryDlg : public CDialog
{
// Construction
public:
	CSetRegistryDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSetRegistryDlg)
	enum { IDD = IDD_SETREGISTRY_DIALOG };
	CButton	m_ManagerWindowModeButton;
	CListCtrl	m_ServerListCtrl;
	BOOL	m_TestServerSetting;
	//}}AFX_DATA
		
	_XList		m_ServerInfoList;
	void		RebuildServerList( void );

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetRegistryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSetRegistryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAddserveraddress();
	afx_msg void OnItemchangedServerlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnDeleteserveraddress();
	afx_msg void OnModifyserveraddress();
	afx_msg void OnDblclkServerlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnManagerwindow();
	afx_msg void OnTestservercheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETREGISTRYDLG_H__C5CAF787_DA52_42DB_A892_6FBB9E06B361__INCLUDED_)
