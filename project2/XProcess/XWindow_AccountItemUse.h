// XWindow_AccountItemUse.h: interface for the _XWindow_AccountItemUse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_ACCOUNTITEMUSE_H__0DD13013_31A5_4C6A_92EC_5E20AF3EC12D__INCLUDED_)
#define AFX_XWINDOW_ACCOUNTITEMUSE_H__0DD13013_31A5_4C6A_92EC_5E20AF3EC12D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_AccountItemUse : public _XWindow
{

public:
	_XWindow_AccountItemUse();
	virtual ~_XWindow_AccountItemUse();

	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void ShowWindow( BOOL show );

};

#endif // !defined(AFX_XWINDOW_ACCOUNTITEMUSE_H__0DD13013_31A5_4C6A_92EC_5E20AF3EC12D__INCLUDED_)
