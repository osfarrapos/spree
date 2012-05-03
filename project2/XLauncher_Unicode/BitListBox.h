#if !defined(AFX_BITLISTBOX_H__949CB2FE_4840_4A0F_9002_1C89DA872724__INCLUDED_)
#define AFX_BITLISTBOX_H__949CB2FE_4840_4A0F_9002_1C89DA872724__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BitListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitListBox window

class CBitListBox : public CListBox
{
// Construction
public:
	CBitListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBitListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBitListBox)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITLISTBOX_H__949CB2FE_4840_4A0F_9002_1C89DA872724__INCLUDED_)
