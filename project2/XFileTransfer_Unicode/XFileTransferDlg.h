// XFileTransferDlg.h : header file
//

#ifndef _XFILETRANSFERDLG_H
#define _XFILETRANSFERDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FTPTransfer.h"

/////////////////////////////////////////////////////////////////////////////
// CXFileTransferDlg dialog


/** File Transfer를 위한 Dialog class.
 * 
 * OnOK() 함수에서 CFileTransfer class의 멤버 함수들을 호출, 파일을 지정된 FTP 서버로 보낸다.
 * \author Sohyun, Park
 * \date 2003.08.18
 */
class CXFileTransferDlg : public CDialog
{
// Construction
public:
	CXFileTransferDlg(CWnd* pParent = NULL);	// standard constructor

	BOOL CompressLogFile( void );

	void LoadString( void );
// Dialog Data
	//{{AFX_DATA(CXFileTransferDlg)
	enum { IDD = IDD_XFILETRANSFER_DIALOG };
	CButton	m_CancelButton;
	CButton	m_OKButton;
	CStatic	m_IDStatic;
	CStatic	m_NoticeStatic;
	CStatic	m_TransferStatic;
	CProgressCtrl	m_Progress;
	CString	m_SenderID;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXFileTransferDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	CFTPTransfer		m_FTPTransfer;

	// Generated message map functions
	//{{AFX_MSG(CXFileTransferDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChangeSenderidedit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
