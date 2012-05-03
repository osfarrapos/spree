// XWindow_MainFrame.h: interface for the _XWindow_MainFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MAINFRAME_H__5E323C5A_B21C_40AC_A2BC_D49A45935FFD__INCLUDED_)
#define AFX_XWINDOW_MAINFRAME_H__5E323C5A_B21C_40AC_A2BC_D49A45935FFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#define _XDEF_MAXWINDOWTABCOUNT		7

class _XWindow_MainFrame : public _XWindow  
{
public:
	_XStatic		m_BorderTile;
	_XImageStatic	m_BorderGradationTitle;

	int				m_SelectedTab;
	_XCheckButton*  m_pWindowTabbutton[_XDEF_MAXWINDOWTABCOUNT];
	_XWindow*		m_pWindowObject[_XDEF_MAXWINDOWTABCOUNT];


public:
	_XWindow_MainFrame();
	virtual ~_XWindow_MainFrame();

	BOOL					Initialize( void );
	void					DestroyWindow( void );
	
	void					Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL					Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void					MoveWindow(int X, int Y);
	void					ShowWindow(BOOL show);
	
	void					SetDefaultPosition( void );
	void					SaveWindowProperty( void );

	void					ChangeWindowTab( DWORD windowid );

	DWORD					GetCurrentSelectedTab(void);
};

#endif // !defined(AFX_XWINDOW_MAINFRAME_H__5E323C5A_B21C_40AC_A2BC_D49A45935FFD__INCLUDED_)
