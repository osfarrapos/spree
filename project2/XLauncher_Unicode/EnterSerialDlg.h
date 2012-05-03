#if !defined(AFX_ENTERSERIALDLG_H__991E704B_0267_4CBF_8860_384E4AEC1AA0__INCLUDED_)
#define AFX_ENTERSERIALDLG_H__991E704B_0267_4CBF_8860_384E4AEC1AA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnterSerialDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnterSerialDlg dialog

class CEnterSerialDlg : public CDialog
{
// Construction
public:
	CEnterSerialDlg(CWnd* pParent = NULL);   // standard constructor

	int  easterneggcdkeyinsertLevel;
	BOOL easterneggcdkeyinsertKey;
	
	static void makeSerial( LPSTR field1, LPSTR field2, LPSTR field3, LPSTR field4, LPSTR field5 );	
	void generateSerial();
// Dialog Data
	//{{AFX_DATA(CEnterSerialDlg)
	enum { IDD = IDD_SERIALDIALOG };
	CString	m_SerialField1;
	CString	m_SerialField2;
	CString	m_SerialField3;
	CString	m_SerialField4;
	CString	m_SerialField5;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnterSerialDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEnterSerialDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChangeSerialfieldedit1();
	afx_msg void OnChangeSerialfieldedit2();
	afx_msg void OnChangeSerialfieldedit3();
	afx_msg void OnChangeSerialfieldedit4();
	afx_msg void OnChangeSerialfieldedit5();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTERSERIALDLG_H__991E704B_0267_4CBF_8860_384E4AEC1AA0__INCLUDED_)
