// XWindow_WebBoard.h: interface for the _XWindow_WebBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_WEBBOARD_H__8B9228BA_6AC9_436B_82BF_D3ABFD87CA88__INCLUDED_)
#define AFX_XWINDOW_WEBBOARD_H__8B9228BA_6AC9_436B_82BF_D3ABFD87CA88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_WebBoard : public _XWindow  
{
public:
	_XWindow_WebBoard();
	virtual ~_XWindow_WebBoard();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
};

#endif // !defined(AFX_XWINDOW_WEBBOARD_H__8B9228BA_6AC9_436B_82BF_D3ABFD87CA88__INCLUDED_)
