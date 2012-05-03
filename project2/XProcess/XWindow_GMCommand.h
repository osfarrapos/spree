// XWindow_GMCommand.h: interface for the _XWindow_GMCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_GMCOMMAND_H__0751EFEE_7F28_4EFF_830A_3EF94B956B86__INCLUDED_)
#define AFX_XWINDOW_GMCOMMAND_H__0751EFEE_7F28_4EFF_830A_3EF94B956B86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum _XGMWINDOW_ANIMATIONMODE
{
	_XGMWINDOW_ANIMATIONMODE_OPEN = 0,
	_XGMWINDOW_ANIMATIONMODE_CLOSE
};

#define _XDEF_GMCOMMAND_MAXCOUNT	10

class _XWindow_GMCommand : public _XWindow  
{
public:
	_XGMWINDOW_ANIMATIONMODE	m_WindowAnimationMode;

	_XButton					m_CommandButton[_XDEF_GMCOMMAND_MAXCOUNT];


public:
	_XWindow_GMCommand();
	virtual ~_XWindow_GMCommand();

	BOOL						Initialize( void );
	void						DestroyWindow( void );
	
	void						CheckOutside( void );
	void						MoveWindow( int X, int Y );		
	void						WindowPosChanging( int& X, int& Y );

	void						Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL						Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void						SetWindowAnimationMode( _XGMWINDOW_ANIMATIONMODE windowanimationmode );
	_XGMWINDOW_ANIMATIONMODE	GetWindowAnimationMode( void )
	{
								return m_WindowAnimationMode;
	}
	
};

#endif // !defined(AFX_XWINDOW_GMCOMMAND_H__0751EFEE_7F28_4EFF_830A_3EF94B956B86__INCLUDED_)
