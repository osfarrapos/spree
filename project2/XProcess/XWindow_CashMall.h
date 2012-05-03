// XWindow_CashMall.h: interface for the _XWindow_CashMall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CASHMALL_H__956854E4_0567_41B6_9B49_0068F631B337__INCLUDED_)
#define AFX_XWINDOW_CASHMALL_H__956854E4_0567_41B6_9B49_0068F631B337__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_CashMall : public _XWindow  
{
public:
	_XWindow_CashMall();
	virtual ~_XWindow_CashMall();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
};

#endif // !defined(AFX_XWINDOW_CASHMALL_H__956854E4_0567_41B6_9B49_0068F631B337__INCLUDED_)
