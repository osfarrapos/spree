// XWindow_OptionMenu.h: interface for the _XWindow_OptionMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_OPTIONMENU_H__8DC93B3C_5723_46F8_9702_32E51623C968__INCLUDED_)
#define AFX_XWINDOW_OPTIONMENU_H__8DC93B3C_5723_46F8_9702_32E51623C968__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_OptionMenu : public _XWindow  
{
public:	
	
public:
	_XWindow_OptionMenu();
	virtual ~_XWindow_OptionMenu();

	BOOL Initialize( void );
	void DestroyWindow( void );
	void MoveWindow( int X, int Y );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void SetDefaultPosition( void );
};

#endif // !defined(AFX_XWINDOW_OPTIONMENU_H__8DC93B3C_5723_46F8_9702_32E51623C968__INCLUDED_)
