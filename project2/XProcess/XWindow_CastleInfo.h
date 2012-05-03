// XWindow_CastleInfo.h: interface for the _XWindow_CastleInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CASTLEINFO_H__1DA7E997_2BAF_4D9F_826F_AAC59746FD2F__INCLUDED_)
#define AFX_XWINDOW_CASTLEINFO_H__1DA7E997_2BAF_4D9F_826F_AAC59746FD2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_CastleInfo  : public _XWindow
{
private :
	_XStatic		m_BorderTitle;

	_XImageStatic*	m_pSubTitle;
	_XImageStatic*	m_pLeftEdge;
	_XImageStatic*	m_pRightEdge;
	_XImageStatic*	m_pTopBar;
	_XImageStatic*	m_pBottomBar;

	_XButton*		m_btnReqWar;

	_XImageStatic*	m_pGroupIcon;
	_XImageStatic*	m_pScheduleIcon[2];

	TCHAR			m_strOrgName[256];
	TCHAR			m_strDate[3][256];
	TCHAR			m_strPropertyDate[256];
	
	_XCastleInfo*	m_pCurCastleInfo;

public:
	_XWindow_CastleInfo();
	virtual ~_XWindow_CastleInfo();

	BOOL			Initialize( void );
	void			DestroyWindow( void );
	
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);

	void			GetIconRect(int groupindex, RECT& rect);
	
	void			SetData(void);
	
	void			EnableButton(BOOL enable);

};

#endif // !defined(AFX_XWINDOW_CASTLEINFO_H__1DA7E997_2BAF_4D9F_826F_AAC59746FD2F__INCLUDED_)
