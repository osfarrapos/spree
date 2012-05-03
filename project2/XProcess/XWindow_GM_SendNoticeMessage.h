// XWindow_GM_SendNoticeMessage.h: interface for the _XWindow_GM_SendNoticeMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_GM_SENDNOTICEMESSAGE_H__0A9073F6_974B_4AF3_82BD_083308C06144__INCLUDED_)
#define AFX_XWINDOW_GM_SENDNOTICEMESSAGE_H__0A9073F6_974B_4AF3_82BD_083308C06144__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_GM_SendNoticeMessage : public _XWindow  
{
public:
	_XList						m_MessageList;
	_XIMEContainer				m_HourIMEControl;
	_XIMEContainer				m_MinuteControl;
	_XIMEContainer				m_SecondControl;
	_XIMEContainer				m_NoticeIMEControl;	

	_XListBox*					m_pListBox;

public:
	_XWindow_GM_SendNoticeMessage();
	virtual ~_XWindow_GM_SendNoticeMessage();

	BOOL						Initialize( void );
	void						DestroyWindow( void );
	
	void						Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL						Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void						MoveWindow( int X, int Y );
	void						ShowWindow( BOOL show );

	void						InsertMessage( void );
	void						DeleteMessage( void );
	void						DeleteAllMessage( void );
};

#endif // !defined(AFX_XWINDOW_GM_SENDNOTICEMESSAGE_H__0A9073F6_974B_4AF3_82BD_083308C06144__INCLUDED_)
