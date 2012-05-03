// _XWindow_TestServerNotice.h: interface for the _XWindow_TestServerNotice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TESTSERVERNOTICE_H__D975DC5C_6339_489B_883E_F7C668BA11D6__INCLUDED_)
#define AFX_XWINDOW_TESTSERVERNOTICE_H__D975DC5C_6339_489B_883E_F7C668BA11D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

class _XWindow_TestServerNotice : public _XWindow  
{
public:
	_XListBox* m_pListBox;

public:
	_XWindow_TestServerNotice();
	virtual ~_XWindow_TestServerNotice();

public:

	BOOL					Initialize( _XTextureManager& texturearchive );
	void					DestroyWindow( void );
	
	void					Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL					Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	BOOL					ProcessWheelMessage( short zDelta );
};

#endif // !defined(AFX_XWINDOW_TESTSERVERNOTICE_H__D975DC5C_6339_489B_883E_F7C668BA11D6__INCLUDED_)
