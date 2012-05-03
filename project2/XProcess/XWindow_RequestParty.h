// XWindow_RequestParty.h: interface for the _XWindow_RequestParty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_REQUESTPARTY_H__213655F1_AA22_4FFD_8C6A_F2F77667E06C__INCLUDED_)
#define AFX_XWINDOW_REQUESTPARTY_H__213655F1_AA22_4FFD_8C6A_F2F77667E06C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XWindow.h"

class _XWindow_RequestParty : public _XWindow  
{
public :
	_XButton*		m_SendMessageButton;
	_XButton*		m_RequestPartyButton;
	_XButton*		m_RequestTradeButton;
	_XButton*		m_RequestFriendButton;
	_XButton*		m_RequestGroupButton;	
	_XButton*		m_RequestAlienceButton;	
	_XButton*		m_RequestPVPButton;
	_XButton*		m_RequestPVPGroupButton;
	_XButton*		m_BlackListButton;
	_XButton*		m_pRequestMnDButton; //Author : ¾çÈñ¿Õ //breif : »çÁ¦
	
	TCHAR			m_RequestCharacterName[13];
	TCHAR			m_PickedCharacterName[13];
	
	unsigned short	m_PickedCharacterID;
	
	D3DXVECTOR3		m_LastPickedObjectPosition;

	_XImageStatic	m_CornerImage[4];
		
public:
	_XWindow_RequestParty();
	virtual ~_XWindow_RequestParty();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			ShowWindow( BOOL show );
};

#endif // !defined(AFX_XWINDOW_REQUESTPARTY_H__213655F1_AA22_4FFD_8C6A_F2F77667E06C__INCLUDED_)
