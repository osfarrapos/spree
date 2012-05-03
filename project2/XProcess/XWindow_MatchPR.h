// XWindow_MatchPR.h: interface for the _XWindow_MatchPR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MATCHPR_H__F08F7687_DEA6_499E_B010_3E617A0D6688__INCLUDED_)
#define AFX_XWINDOW_MATCHPR_H__F08F7687_DEA6_499E_B010_3E617A0D6688__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_MatchPR  : public _XWindow
{
public:
	_XButton*			m_btnOpen;						

	_XStatic			m_BorderTile;	
	_XIMEContainer		m_PVPIMEControl;				// 비무 홍보내용 입력창
	TCHAR				m_stringPVPPR[81];				// 비무 홍보 내용 40자
	
public:
	_XWindow_MatchPR();
	virtual ~_XWindow_MatchPR();
	
	BOOL			Initialize();
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	
	BOOL			CheckPVPIMEBoxArea(void);
};

#endif // !defined(AFX_XWINDOW_MATCHPR_H__F08F7687_DEA6_499E_B010_3E617A0D6688__INCLUDED_)
