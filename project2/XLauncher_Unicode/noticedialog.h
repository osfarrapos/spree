#if !defined(AFX_NOTICEDIALOG_H__9A0CD9DE_2FF8_4D5B_9DC0_C10223F7D004__INCLUDED_)
#define AFX_NOTICEDIALOG_H__9A0CD9DE_2FF8_4D5B_9DC0_C10223F7D004__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// noticedialog.h : header file
//

#include "Dib.h"
#include "BitmapDialog.h"
#include "BtnST.h"
#include "Package.h"
#include "XBrowser.h"
#include "webbro.h"
#include "Label.h"

/////////////////////////////////////////////////////////////////////////////
// CNoticeDialog dialog

typedef enum _XWEBBROWSER_TITLE
{
	_XWEBBROWSER_TITLE_NOTICE = 0,
	_XWEBBROWSER_TITLE_UPDATE = 1,
};

class CNoticeDialog : public CDialog
{
// Construction
public:
	CNoticeDialog(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CNoticeDialog)
	enum { IDD = IDD_NOTICEDIALOG };
	CLabel	m_TitleStatic;
	CButtonST		m_CloseButton;
	CWebBrowser2	m_WebBoardBrowser;
	//}}AFX_DATA

	CDIB			m_BorderImage;

	CString			m_csIPAddress;
	void			ShowNoticePage( LPTSTR IPAddress, _XWEBBROWSER_TITLE titletype );
	void			SetTitle( _XWEBBROWSER_TITLE titletype );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoticeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNoticeDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClosebutton();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTICEDIALOG_H__9A0CD9DE_2FF8_4D5B_9DC0_C10223F7D004__INCLUDED_)
