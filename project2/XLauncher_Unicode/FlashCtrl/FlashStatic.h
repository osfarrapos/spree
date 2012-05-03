#if !defined(AFX_FLASHSTATIC_H__626D19DE_658F_429D_81AE_06FB87FB97DA__INCLUDED_)
#define AFX_FLASHSTATIC_H__626D19DE_658F_429D_81AE_06FB87FB97DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlashStatic.h : header file
//

#include "FlashControl.h"

class CFlashStatic : public CStatic
{
// Construction
public:
	CFlashStatic();

// º¯¼öµé
public:
	CFlashControlInterface *flashWin;

	int				m_nZoomIn;
	BOOL			m_bHighQuality;
	HBRUSH			m_brushHollow;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlashStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void OpenFile(char * szFileName);
	virtual ~CFlashStatic();

	HPALETTE CreateIdentityPalette( HWND hwnd );
	void	ResetFlashControl( void );

	// Generated message map functions
protected:
	//{{AFX_MSG(CFlashStatic)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnAboutFlash();
	afx_msg void OnBack();
	afx_msg void OnForward();
	afx_msg void OnHighQuality();
	afx_msg void OnLoop();
	afx_msg void OnPlay();
	afx_msg void OnRewind();
	afx_msg void OnShowAll();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLASHSTATIC_H__626D19DE_658F_429D_81AE_06FB87FB97DA__INCLUDED_)
