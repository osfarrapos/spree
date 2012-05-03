// XWindow_Elixir.h: interface for the _XWindow_Elixir class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_ELIXIR_H__3C07A0FB_887D_4865_B8A5_020F21757A39__INCLUDED_)
#define AFX_XWINDOW_ELIXIR_H__3C07A0FB_887D_4865_B8A5_020F21757A39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_Elixir : public _XWindow  
{
public:
	_XWindow_Elixir();
	virtual ~_XWindow_Elixir();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
};

#endif // !defined(AFX_XWINDOW_ELIXIR_H__3C07A0FB_887D_4865_B8A5_020F21757A39__INCLUDED_)
