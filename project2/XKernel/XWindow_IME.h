// XWindow_IME.h: interface for the _XWindow_IME class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_IME_H_
#define _XWINDOW_IME_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

class _XListBox;

class _XWindow_IME : public _XWindow  
{

public:
	_XListBox*	m_pListBox;

public:
	_XWindow_IME();
	virtual ~_XWindow_IME();

	BOOL	Initialize(void);
	void	DestroyWindow(void);
	
	void	Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL	Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
			
	BOOL	CheckArea(void);
	
	void	ShowWindow(BOOL show);

};

#endif // !defined(_XWINDOW_IME_H_)
