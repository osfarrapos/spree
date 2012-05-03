// XWindow_GM_StatusControl.h: interface for the _XWindow_GM_StatusControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_GM_STATUSCONTROL_H__00DF8FD5_153E_4AF1_8FD0_446554795926__INCLUDED_)
#define AFX_XWINDOW_GM_STATUSCONTROL_H__00DF8FD5_153E_4AF1_8FD0_446554795926__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XWindow_GM_StatusControl : public _XWindow  
{
public:
	_XCheckButton*				m_pHiddenButton;

public:
	_XWindow_GM_StatusControl();
	virtual ~_XWindow_GM_StatusControl();

	BOOL						Initialize( void );
	void						DestroyWindow( void );
	
	void						Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL						Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void						MoveWindow( int X, int Y );
	void						ShowWindow( BOOL show );

	void						ToggleHiddenStatus( void );
};

#endif // !defined(AFX_XWINDOW_GM_STATUSCONTROL_H__00DF8FD5_153E_4AF1_8FD0_446554795926__INCLUDED_)
