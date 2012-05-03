// XWindow_FreeMatchInfo.h: interface for the _XWindow_FreeMatchInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_FREEMATCHINFO_H__A098852B_840D_420A_88CF_1D0D6A96FFEE__INCLUDED_)
#define AFX_XWINDOW_FREEMATCHINFO_H__A098852B_840D_420A_88CF_1D0D6A96FFEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_FreeMatchInfo  : public _XWindow
{
private :
	_XStatic		m_BorderTitle;

	_XImageStatic*	m_pSubTitle;
	_XImageStatic*	m_pLeftEdge;
	_XImageStatic*	m_pRightEdge;
	_XImageStatic*	m_pTopBar;
	_XImageStatic*	m_pBottomBar;

public :
	TCHAR			m_szDate[512];
	TCHAR			m_szLevel[512];

public:

	_XWindow_FreeMatchInfo();
	virtual ~_XWindow_FreeMatchInfo();

	BOOL			Initialize( void );
	void			DestroyWindow( void );
	
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);

};

#endif // !defined(AFX_XWINDOW_FREEMATCHINFO_H__A098852B_840D_420A_88CF_1D0D6A96FFEE__INCLUDED_)
