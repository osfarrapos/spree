// XWindow_Debug.h: interface for the _XWindow_Debug class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_DEBUG_H__4D2DB1B9_225D_4B17_8ED6_797F0855E72D__INCLUDED_)
#define AFX_XWINDOW_DEBUG_H__4D2DB1B9_225D_4B17_8ED6_797F0855E72D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_Debug : public _XWindow  
{
public:
	_XImageStatic		m_LeftImage;	
		
	_XWINDOW_RESIZEMODE m_ResizeMode;
	BOOL				m_Resizing;

	int					m_DebugInfoListScrollPos;
	_XVScrollBar*		m_DebugInfoListScrollBar;	
	_XGUIObject*		m_pScrbarFocusedObject;	

	int					m_MaxViewCount;
	int					m_DebugInfoMode;	

	_XCheckButton		m_DebugInfoModeButton[4];	

	TCHAR				m_LastSendedCommand[51];
	
public:
	_XWindow_Debug();
	virtual ~_XWindow_Debug();

	BOOL Initialize( void );
	void DestroyWindow( void );
	void MoveWindow( int X, int Y );
	void ShowWindow( BOOL show );
	void ResizeWindow( RECT winrect ){ ResizeWindow(winrect.left, winrect.top, winrect.right, winrect.bottom ); }
	void ResizeWindow( int SX, int SY, int EX, int EY );
	BOOL CheckMousePosition( void );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL ProcessWheelMessage( short zDelta );
	
	void SetDefaultPosition( void );
	void SaveWindowProperty( void );

	void SetDebugInfoMode( int mode );	
};

#endif // !defined(AFX_XWINDOW_DEBUG_H__4D2DB1B9_225D_4B17_8ED6_797F0855E72D__INCLUDED_)
