// XWindow_SystemMsgDefault.h: interface for the _XWindow_SystemMsgDefault class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_SYSTEMMSGDEFAULT_H__C0073829_12F9_4478_9307_1431D7321CBD__INCLUDED_)
#define AFX_XWINDOW_SYSTEMMSGDEFAULT_H__C0073829_12F9_4478_9307_1431D7321CBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 시스템 메세지 창
 * 시스템 메세지 출력, 채팅창에서 on/off 할 수 있음.
 * 인벤 메신저 e메일 창 상태에 따라 자동 이동
*/

class _XWindow_Email;
class _XWindow_Messenger;

class _XWindow_SystemMsgDefault : public _XWindow  
{
private:
	_XWindow_Email*		m_pEmailWindowPtr;	
	_XWindow_Messenger*	m_pMessengerWindowPtr;
public:
	int					m_SystemMsgListScrollPos;
	_XVScrollBar*		m_SystemMsgListScrollBar;	
	_XGUIObject*		m_pScrbarFocusedObject;	

	int					m_MaxViewCount;
	
	_XButton*			m_btnFunctionalObjectViewMode;

public:
	_XWindow_SystemMsgDefault();
	virtual ~_XWindow_SystemMsgDefault();

	BOOL				Initialize( void );
	void				DestroyWindow( void );		
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				ProcessWheelMessage( short zDelta );
	BOOL				CheckMousePosition(void);
	void				MoveWindow(int X, int Y);
	void				ShowWindow(BOOL show);
	void				SetDockingMode(BOOL flag);

	void				SetEmailWindowPtr(_XWindow_Email* windowptr) { m_pEmailWindowPtr = windowptr; 	};
	void				SetMessengerWindowPtr(_XWindow_Messenger* windowptr ) { m_pMessengerWindowPtr = windowptr;	};
};

#endif // !defined(AFX_XWINDOW_SYSTEMMSGDEFAULT_H__C0073829_12F9_4478_9307_1431D7321CBD__INCLUDED_)
