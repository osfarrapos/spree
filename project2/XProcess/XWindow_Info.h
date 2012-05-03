// XWindow_Info.h: interface for the _XWindow_Info class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_INFO_H__EC5D4B39_697F_4DF2_9863_F08F4A888DA0__INCLUDED_)
#define AFX_XWINDOW_INFO_H__EC5D4B39_697F_4DF2_9863_F08F4A888DA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_Info : public _XWindow 
{
private :
	BOOL		m_bSetTooltip;

public:
	_XWindow_Info();
	virtual ~_XWindow_Info();

	BOOL			Initialize( void );
	void			DestroyWindow( void );
	
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void			ShowWindow(BOOL show);

	void			ShowTooltip(BOOL show);

};

#endif // !defined(AFX_XWINDOW_INFO_H__EC5D4B39_697F_4DF2_9863_F08F4A888DA0__INCLUDED_)
