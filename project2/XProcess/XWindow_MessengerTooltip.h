// XWindow_MessengerTooltip.h: interface for the _XWindow_MessengerTooltip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MESSENGERTOOLTIP_H__6204F860_99FE_4E59_8FC8_104A343111A5__INCLUDED_)
#define AFX_XWINDOW_MESSENGERTOOLTIP_H__6204F860_99FE_4E59_8FC8_104A343111A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum _XMessengerTooltip_Mode
{
	_XMESSENGERTOOLTIP_NONE = 0,
	_XMESSENGERTOOLTIP_GROUPLIST,
	_XMESSENGERTOOLTIP_CHARACTERLIST,
	_XMESSENGERTOOLTIP_CHARACTERSTATE
}_XMESSENGERTOOLTIP_MODE;

class _XWindow_MessengerTooltip : public _XWindow  
{
public:
	_XButton*		m_AddGroupButton;	
	_XButton*		m_DeleteGroupButton;	
	_XButton*		m_ChangeGroupNameButton;	
	_XButton*		m_DeleteFriendButton;	
	_XButton*		m_CloseButton;
	
//	BOOL			m_bIsButtonProcess;	// 버튼을 클릭하였을 때 응답이 오기전에는 버튼작용을 막는다.
	
	_XListBox*		m_listboxUserStatus;
	
	_XMESSENGERTOOLTIP_MODE	m_MessengerTooltipMode;
	
public:
	_XWindow_MessengerTooltip();
	virtual ~_XWindow_MessengerTooltip();
	
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			ShowWindow( BOOL show );
	
	void			SetMode(_XMESSENGERTOOLTIP_MODE tooltipmode );
	void			SetListStatus();
	
};

#endif // !defined(AFX_XWINDOW_MESSENGERTOOLTIP_H__6204F860_99FE_4E59_8FC8_104A343111A5__INCLUDED_)
