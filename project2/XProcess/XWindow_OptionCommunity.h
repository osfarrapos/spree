// XWindow_OptionCommunity.h: interface for the _XWindow_OptionCommunity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_OPTIONCOMMUNITY_H__50765C4E_0A51_4A7F_B773_3307140D4783__INCLUDED_)
#define AFX_XWINDOW_OPTIONCOMMUNITY_H__50765C4E_0A51_4A7F_B773_3307140D4783__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_OptionCommunity : public _XWindow  
{

public:	
	_XCheckButton*		m_pRejectionWhisperButton;	
	_XCheckButton*		m_pRejectionFriendButton;	
	_XCheckButton*		m_pRejectionSubPartyButton;			// 동행
	_XCheckButton*		m_pRejectionPartyButton;			// 방회/분파
	_XCheckButton*		m_pRejectionExchangeButton;			// 거래
	_XCheckButton*		m_pRejectionTargetInfoButton;		// 인물정보
	
	_XCheckButton*		m_pConnectMessengerButton;			// 메신져 자동 접속
#ifdef _XDEF_ZONECHATTING_20070108
	_XCheckButton*		m_pBtnDisableZoneChatting;			// 지역대화 끄기
#endif
	

public:
	_XWindow_OptionCommunity();
	virtual ~_XWindow_OptionCommunity();

	BOOL Initialize( void );
	void DestroyWindow( void );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
};

#endif // !defined(AFX_XWINDOW_OPTIONCOMMUNITY_H__50765C4E_0A51_4A7F_B773_3307140D4783__INCLUDED_)
