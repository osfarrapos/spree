// XWindow_ChallengerList.h: interface for the _XWindow_ChallengerList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CHALLENGERLIST_H__ADE1201C_76D8_439E_A559_3DDEDED4DDE1__INCLUDED_)
#define AFX_XWINDOW_CHALLENGERLIST_H__ADE1201C_76D8_439E_A559_3DDEDED4DDE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_ChallengerList  : public _XWindow
{
private :
	_XStatic		m_BorderTitle;

	_XImageStatic*	m_pSubTitle;
	_XImageStatic*	m_pLeftEdge;
	_XImageStatic*	m_pRightEdge;
	_XImageStatic*	m_pTopBar;
	_XImageStatic*	m_pBottomBar;
	_XImageStatic*	m_pScheduleIcon;

	_XButton*		m_btnReqList;
	
	_XListBox*		m_lstboxChallengers;

	TCHAR			m_strDate[256];
	
	_XCastleInfo*	m_pCurCastleInfo;


public:
	_XWindow_ChallengerList();
	virtual ~_XWindow_ChallengerList();

	BOOL			Initialize( void );
	void			DestroyWindow( void );
	
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);

	void			SetListItem(void);
	void			SetData(void);
	
	void			EnableButton(BOOL enable);
};

#endif // !defined(AFX_XWINDOW_CHALLENGERLIST_H__ADE1201C_76D8_439E_A559_3DDEDED4DDE1__INCLUDED_)
