// XWindow_AdminNotice.h: interface for the _XWindow_AdminNotice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_ADMINNOTICE_H__4D2DB1B9_225D_4B17_8ED6_797F0855E72D__INCLUDED_)
#define AFX_XWINDOW_ADMINNOTICE_H__4D2DB1B9_225D_4B17_8ED6_797F0855E72D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_AdminNotice : public _XWindow  
{
public:			
	int					m_NoticeListScrollPos;
	_XVScrollBar*		m_NoticeListScrollBar;	
	_XGUIObject*		m_pScrbarFocusedObject;	
	
	int					m_MaxViewCount;
	int					m_NoticeMessageMaxWidth;
	
	_XList				m_NoticeMessageList;
	
	LONG				m_AutoCloseTimer;
	
	_XButton*			m_CloseButton;
	
public:
	_XWindow_AdminNotice();
	virtual ~_XWindow_AdminNotice();
	
	BOOL Initialize( void );
	void DestroyWindow( void );
	void MoveWindow( int X, int Y );
	void ShowWindow( BOOL show );
	BOOL CheckMousePosition( void );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL ProcessWheelMessage( short zDelta );
	
	void SetDefaultPosition( void );
	void SaveWindowProperty( void );
	
	void SetNoticeMessage( LPSTR message );
};

#endif // !defined(AFX_XWINDOW_ADMINNOTICE_H__4D2DB1B9_225D_4B17_8ED6_797F0855E72D__INCLUDED_)
