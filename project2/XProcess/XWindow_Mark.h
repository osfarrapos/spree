// XWindow_Mark.h: interface for the _XWindow_Mark class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MARK_H__88B58B78_927D_4D7A_819B_C97C46D41F62__INCLUDED_)
#define AFX_XWINDOW_MARK_H__88B58B78_927D_4D7A_819B_C97C46D41F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_Mark  : public _XWindow
{
public : 
	_XImageStatic*		m_pImgAttackType[2];
	_XImageStatic*		m_pImgCaptain;

public:
	_XWindow_Mark();
	virtual ~_XWindow_Mark();

	BOOL			Initialize( void );
	void			DestroyWindow( void );
	
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

};

#endif // !defined(AFX_XWINDOW_MARK_H__88B58B78_927D_4D7A_819B_C97C46D41F62__INCLUDED_)
