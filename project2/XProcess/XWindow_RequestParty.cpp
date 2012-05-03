// XWindow_RequestParty.cpp: implementation of the _XWindow_RequestParty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "emperorofdragons.h"
#include "XWindow_RequestParty.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_RequestParty::_XWindow_RequestParty()
{
	m_SendMessageButton = NULL;
	m_RequestPartyButton = NULL;
	m_RequestTradeButton = NULL;
	m_RequestFriendButton = NULL;
	m_RequestGroupButton = NULL;
	m_RequestAlienceButton = NULL;
	m_RequestPVPButton = NULL;
	m_RequestPVPGroupButton = NULL;
	m_BlackListButton	= NULL;
	m_pRequestMnDButton = NULL;

	memset(m_RequestCharacterName, 0, sizeof(TCHAR)*13);
	memset(m_PickedCharacterName, 0, sizeof(TCHAR)*13);

	m_LastPickedObjectPosition.x = 0.0f;
	m_LastPickedObjectPosition.y = 0.0f;
	m_LastPickedObjectPosition.z = 0.0f;

	m_PickedCharacterID = 0;
}

_XWindow_RequestParty::~_XWindow_RequestParty()
{
	DestroyWindow();
}

BOOL _XWindow_RequestParty::Initialize(void)
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName);

	if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		_XBTN_STRUCTURE btnstruct = 
		{
			TRUE,
			{4, 4},
			{124, 19},
			_XDEF_REQUESTPARTY_REQUESTPARTY,
			-1,
			-1,
			-1,
			NULL
		};
		m_RequestPartyButton = new _XButton;
		m_RequestPartyButton->Create(btnstruct);
		m_RequestPartyButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestPartyButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_REQUESTPARTY);	// 동행신청
		m_RequestPartyButton->EnableWindow(TRUE);
		InsertChildObject(m_RequestPartyButton);

		btnstruct.position.y = 22;
		btnstruct.commandid = _XDEF_REQUESTPARTY_REQUESTFRIEND;
		m_RequestFriendButton = new _XButton;
		m_RequestFriendButton->Create(btnstruct);
		m_RequestFriendButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestFriendButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_REQFRIEND);		// 친구신청
		InsertChildObject(m_RequestFriendButton);

		btnstruct.position.y = 40;
		btnstruct.commandid = _XDEF_REQUESTPARTY_REQUESTTRADE;
		m_RequestTradeButton = new _XButton;
		m_RequestTradeButton->Create(btnstruct);
		m_RequestTradeButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestTradeButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_REQUESTTRADE);	//교환신청
		InsertChildObject(m_RequestTradeButton);

		btnstruct.position.y = 58;
		btnstruct.commandid = _XDEF_REQUESTPARTY_REQGROUP;
		m_RequestGroupButton = new _XButton;
		m_RequestGroupButton->Create(btnstruct);
		m_RequestGroupButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestGroupButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_2297);	// 영웅단 권유
		InsertChildObject(m_RequestGroupButton);
		
#ifdef _XTS_ALIANCE_20061018
		btnstruct.position.y += 18;
		btnstruct.commandid = _XDEF_REQUESTPARTY_REQALIENCE;
		m_RequestAlienceButton = new _XButton;
		m_RequestAlienceButton->Create(btnstruct);
		m_RequestAlienceButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestAlienceButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3124);	// 동맹 권유
		m_RequestAlienceButton->EnableWindow(FALSE);
		InsertChildObject(m_RequestAlienceButton);
		
#endif
		btnstruct.position.y += 18;
		btnstruct.commandid = _XDEF_REQUESTPARTY_SENDMESSAGE;
		m_SendMessageButton = new _XButton;
		m_SendMessageButton->Create(btnstruct);
		m_SendMessageButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_SendMessageButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_WISPER);	// 귓속말
		InsertChildObject(m_SendMessageButton);
		
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		btnstruct.position.y += 18;
		btnstruct.commandid = _XDEF_CHATDEFAULT_USER_CHATBLACKLISTUBUTTON;
		m_BlackListButton = new _XButton;
		m_BlackListButton->Create(btnstruct);
		m_BlackListButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_BlackListButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3422);				// 블랙리스트
		InsertChildObject(m_BlackListButton);
#endif

		btnstruct.position.y += 18;
		btnstruct.commandid = _XDEF_REQUESTPARTY_REQPVP;
		m_RequestPVPButton = new _XButton;
		m_RequestPVPButton->Create(btnstruct);
		m_RequestPVPButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestPVPButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_2572);		// 개인비무 신청
		m_RequestPVPButton->EnableWindow(FALSE);
		InsertChildObject(m_RequestPVPButton);
		
		btnstruct.position.y += 18;
		btnstruct.commandid = _XDEF_REQUESTPARTY_REQPVPGROUP;
		m_RequestPVPGroupButton = new _XButton;
		m_RequestPVPGroupButton->Create(btnstruct);
		m_RequestPVPGroupButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestPVPGroupButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_2573);	// 집단비무 신청
		m_RequestPVPGroupButton->EnableWindow(FALSE);
		InsertChildObject(m_RequestPVPGroupButton);

	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
		btnstruct.position.y += 18;
		btnstruct.commandid = _XDEF_REQUESTPARTY_REQMASTERNDISCIPLE;

		m_pRequestMnDButton = new _XButton;
		m_pRequestMnDButton->Create(btnstruct);
		m_pRequestMnDButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_pRequestMnDButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3865); //사제신청
		m_pRequestMnDButton->EnableWindow(FALSE);
		InsertChildObject(m_pRequestMnDButton);
	#endif
	}
	else
	{
		_XBTN_STRUCTURE messagebtnstruct = 
		{
			TRUE, 
			{4, 4},
			{88, 19},
			_XDEF_REQUESTPARTY_REQUESTPARTY,
			resourceindex,
			resourceindex,
			resourceindex,
			&g_MainInterfaceTextureArchive
		};
		m_RequestPartyButton = new _XButton;
		m_RequestPartyButton->Create(messagebtnstruct);
		m_RequestPartyButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_RequestPartyButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_RequestPartyButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_RequestPartyButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestPartyButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_REQUESTPARTY);	// "동행신청"
		m_RequestPartyButton->EnableWindow(TRUE);
		
		InsertChildObject(m_RequestPartyButton);

		_XBTN_STRUCTURE requestbtnstruct = 
		{
			TRUE, 
			{4, 22},
			{88, 19},
			_XDEF_REQUESTPARTY_REQUESTFRIEND,
			resourceindex,
			resourceindex,
			resourceindex,
			&g_MainInterfaceTextureArchive
		};
		m_RequestFriendButton = new _XButton;
		m_RequestFriendButton->Create(requestbtnstruct);
		m_RequestFriendButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_RequestFriendButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_RequestFriendButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_RequestFriendButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestFriendButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_REQFRIEND);	// "친구신청"
		InsertChildObject(m_RequestFriendButton);

		_XBTN_STRUCTURE tradebtnstruct = 
		{
			TRUE,
			{4, 40},
			{88, 19},
			_XDEF_REQUESTPARTY_REQUESTTRADE,
			resourceindex,
			resourceindex,
			resourceindex,
			&g_MainInterfaceTextureArchive
		};
		m_RequestTradeButton = new _XButton;
		m_RequestTradeButton->Create(tradebtnstruct);
		m_RequestTradeButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_RequestTradeButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_RequestTradeButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_RequestTradeButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestTradeButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_REQUESTTRADE);	// "교환신청"
		InsertChildObject(m_RequestTradeButton);

		tradebtnstruct.position.x = 4;
		tradebtnstruct.position.y = 58;
		tradebtnstruct.commandid = _XDEF_REQUESTPARTY_REQGROUP;
		
		m_RequestGroupButton = new _XButton;
		m_RequestGroupButton->Create(tradebtnstruct);
		m_RequestGroupButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_RequestGroupButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_RequestGroupButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_RequestGroupButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestGroupButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_2297);	//"영웅단 권유"
		//m_RequestGroupButton->EnableWindow(FALSE);
		InsertChildObject(m_RequestGroupButton);

#ifdef _XTS_ALIANCE_20061018
		tradebtnstruct.position.x = 4;
		tradebtnstruct.position.y += 18;
		tradebtnstruct.commandid = _XDEF_REQUESTPARTY_REQALIENCE;
		
		m_RequestAlienceButton = new _XButton;
		m_RequestAlienceButton->Create(tradebtnstruct);
		m_RequestAlienceButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_RequestAlienceButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_RequestAlienceButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_RequestAlienceButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestAlienceButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3124);	// "동맹 권유"
		InsertChildObject(m_RequestAlienceButton);
#endif
		
		tradebtnstruct.position.x = 4;
		tradebtnstruct.position.y += 18;
		tradebtnstruct.commandid = _XDEF_REQUESTPARTY_SENDMESSAGE;
		
		m_SendMessageButton = new _XButton;
		m_SendMessageButton->Create(tradebtnstruct);
		m_SendMessageButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_SendMessageButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_SendMessageButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_SendMessageButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_SendMessageButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_WISPER);	// "귓속말"
		InsertChildObject(m_SendMessageButton);

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		tradebtnstruct.position.x = 4;
		tradebtnstruct.position.y += 18;
		tradebtnstruct.commandid = _XDEF_CHATDEFAULT_USER_CHATBLACKLISTUBUTTON;

		m_BlackListButton = new _XButton;
		m_BlackListButton->Create(tradebtnstruct);
		m_BlackListButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_BlackListButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_BlackListButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_BlackListButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_BlackListButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3422);	// 블랙리스트
		InsertChildObject(m_BlackListButton);		
#endif
		
		tradebtnstruct.position.x = 4;
		tradebtnstruct.position.y += 18;
		tradebtnstruct.commandid = _XDEF_REQUESTPARTY_REQPVP;
		
		m_RequestPVPButton = new _XButton;
		m_RequestPVPButton->Create(tradebtnstruct);
		m_RequestPVPButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_RequestPVPButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_RequestPVPButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_RequestPVPButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestPVPButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_2572);
		m_RequestPVPButton->EnableWindow(FALSE);
		InsertChildObject(m_RequestPVPButton);
		
		tradebtnstruct.position.x = 4;
		tradebtnstruct.position.y += 18;
		tradebtnstruct.commandid = _XDEF_REQUESTPARTY_REQPVPGROUP;
		
		m_RequestPVPGroupButton = new _XButton;
		m_RequestPVPGroupButton->Create(tradebtnstruct);
		m_RequestPVPGroupButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_RequestPVPGroupButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_RequestPVPGroupButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_RequestPVPGroupButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_RequestPVPGroupButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_2573);
		m_RequestPVPGroupButton->EnableWindow(FALSE);
		InsertChildObject(m_RequestPVPGroupButton);
	
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
		tradebtnstruct.position.x = 4;
		tradebtnstruct.position.y += 18;
		tradebtnstruct.commandid = _XDEF_REQUESTPARTY_REQMASTERNDISCIPLE;

		m_pRequestMnDButton = new _XButton;
		m_pRequestMnDButton->Create(tradebtnstruct);
		m_pRequestMnDButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_pRequestMnDButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_pRequestMnDButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_pRequestMnDButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		m_pRequestMnDButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3865); //사제 신청
		m_pRequestMnDButton->EnableWindow(TRUE);
		m_pRequestMnDButton->ShowWindow(TRUE);
		InsertChildObject(m_pRequestMnDButton);
	#endif
	}

	for(int i = 0 ; i < 4 ; i++)
		m_CornerImage[i].Create(0, 0, 2, 2, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_chat_quick.tga"));

	m_CornerImage[0].SetClipRect(185, 216, 187, 218);
	m_CornerImage[1].SetClipRect(188, 216, 190, 218);
	m_CornerImage[2].SetClipRect(191, 216, 193, 218);
	m_CornerImage[3].SetClipRect(194, 216, 196, 218);

	return TRUE;
}

void _XWindow_RequestParty::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_RequestParty::ShowWindow( BOOL show )
{
	_XWindow::ShowWindow( show );

	if( !show )
	{
		if( g_CurrentFocusedObject == (_XGUIObject*)m_SendMessageButton )
			g_CurrentFocusedObject = NULL;

		if( g_CurrentFocusedObject == (_XGUIObject*)m_RequestPartyButton )
			g_CurrentFocusedObject = NULL;
	}	
//#ifdef _XDEF_SHUTDOWN_20061027
//	// 셧다운시에 제약 사항
//	// 영웅단 가입 금지
//	else
//	{		
//		if( g_PlayLeftTime == 0 )
//			m_RequestGroupButton->EnableWindow(FALSE);
//		else
//			m_RequestGroupButton->EnableWindow(TRUE);			
//	}
//	
//#endif
}

void _XWindow_RequestParty::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
//		_XDrawSolidBar((FLOAT)(m_WindowPosition.x+3), (FLOAT)(m_WindowPosition.y+3), (FLOAT)(m_WindowPosition.x+93), (FLOAT)(m_WindowPosition.x+113),
//			D3DCOLOR_ARGB(127, 0, 0, 0));

		int additionalheight = 0;
#ifdef _XTS_ALIANCE_20061018
		
		additionalheight = 18;

	#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		additionalheight += 18;
	#endif

	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
		additionalheight += 18;
	#endif

		// 위 가로
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+2), (FLOAT)(m_WindowPosition.y+0), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+0), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+3), (FLOAT)(m_WindowPosition.y+1), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+1), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+3), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+2), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		
		// 왼쪽 세로
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+0), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+0), (FLOAT)(m_WindowPosition.y+132+additionalheight), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0,0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+1), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+1), (FLOAT)(m_WindowPosition.y+131+additionalheight), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+2), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+2), (FLOAT)(m_WindowPosition.y+131+additionalheight), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		
		// 아래 가로
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+2), (FLOAT)(m_WindowPosition.y+133+additionalheight), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+133+additionalheight), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+3), (FLOAT)(m_WindowPosition.y+132+additionalheight), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+132+additionalheight), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+3), (FLOAT)(m_WindowPosition.y+131+additionalheight), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+131+additionalheight), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		
		// 오른쪽 세로
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+m_WindowSize.cx+1), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx+1), (FLOAT)(m_WindowPosition.y+132+additionalheight), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+m_WindowSize.cx), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx), (FLOAT)(m_WindowPosition.y+131+additionalheight), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+131+additionalheight), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		
		m_CornerImage[0].Draw(m_WindowPosition.x+1, m_WindowPosition.y+1);
		m_CornerImage[1].Draw(m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+1);
		m_CornerImage[2].Draw(m_WindowPosition.x+1, m_WindowPosition.y+132+additionalheight);
		m_CornerImage[3].Draw(m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+132+additionalheight);
#else
	
	#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		additionalheight += 18;
	#endif

	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
		additionalheight += 18;
	#endif
		// 위 가로
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+2), (FLOAT)(m_WindowPosition.y+0), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+0), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+3), (FLOAT)(m_WindowPosition.y+1), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+1), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+3), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+2), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		
		// 왼쪽 세로
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+0), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+0), (FLOAT)(m_WindowPosition.y+132+additionalheight), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0,0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+1), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+1), (FLOAT)(m_WindowPosition.y+131+additionalheight), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+2), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+2), (FLOAT)(m_WindowPosition.y+131+additionalheight), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		
		// 아래 가로
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+2), (FLOAT)(m_WindowPosition.y+133+additionalheight), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+133+additionalheight), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+3), (FLOAT)(m_WindowPosition.y+132+additionalheight), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+132+additionalheight), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+3), (FLOAT)(m_WindowPosition.y+131+additionalheight), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+131+additionalheight), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		
		// 오른쪽 세로
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+m_WindowSize.cx+1), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx+1), (FLOAT)(m_WindowPosition.y+132+additionalheight), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+m_WindowSize.cx), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx), (FLOAT)(m_WindowPosition.y+131+additionalheight), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+2), (FLOAT)(m_WindowPosition.x+m_WindowSize.cx-1), (FLOAT)(m_WindowPosition.y+131+additionalheight), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		
		m_CornerImage[0].Draw(m_WindowPosition.x+1, m_WindowPosition.y+1);
		m_CornerImage[1].Draw(m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+1);
		m_CornerImage[2].Draw(m_WindowPosition.x+1, m_WindowPosition.y+132+additionalheight);
		m_CornerImage[3].Draw(m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+132+additionalheight);

#endif
		if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			D3DCOLOR color;

			if(m_RequestPartyButton->IsButtonEnabled())
				color = D3DCOLOR_ARGB(255, 240, 240, 240);
			else
				color = D3DCOLOR_ARGB(255, 121, 120, 112);

			_XDrawRectAngle((FLOAT)(m_RequestPartyButton->GetWindowPos().x), 
							(FLOAT)(m_RequestPartyButton->GetWindowPos().y), 
							(FLOAT)(m_RequestPartyButton->GetWindowPos().x+m_RequestPartyButton->GetWindowSize().cx), 
							(FLOAT)(m_RequestPartyButton->GetWindowPos().y+m_RequestPartyButton->GetWindowSize().cy),
							0.0f,color);

			if(m_RequestFriendButton->IsButtonEnabled())
				color = D3DCOLOR_ARGB(255, 240, 240, 240);
			else
				color = D3DCOLOR_ARGB(255, 121, 120, 112);
			
			_XDrawRectAngle((FLOAT)(m_RequestFriendButton->GetWindowPos().x), 
							(FLOAT)(m_RequestFriendButton->GetWindowPos().y), 
							(FLOAT)(m_RequestFriendButton->GetWindowPos().x+m_RequestFriendButton->GetWindowSize().cx), 
							(FLOAT)(m_RequestFriendButton->GetWindowPos().y+m_RequestFriendButton->GetWindowSize().cy),
							0.0f,color);

			if(m_RequestTradeButton->IsButtonEnabled())
				color = D3DCOLOR_ARGB(255, 240, 240, 240);
			else
				color = D3DCOLOR_ARGB(255, 121, 120, 112);
			
			_XDrawRectAngle((FLOAT)(m_RequestTradeButton->GetWindowPos().x), 
							(FLOAT)(m_RequestTradeButton->GetWindowPos().y), 
							(FLOAT)(m_RequestTradeButton->GetWindowPos().x+m_RequestTradeButton->GetWindowSize().cx), 
							(FLOAT)(m_RequestTradeButton->GetWindowPos().y+m_RequestTradeButton->GetWindowSize().cy),
							0.0f,color);

			if(m_RequestGroupButton->IsButtonEnabled())
				color = D3DCOLOR_ARGB(255, 240, 240, 240);
			else
				color = D3DCOLOR_ARGB(255, 121, 120, 112);
			
			_XDrawRectAngle((FLOAT)(m_RequestGroupButton->GetWindowPos().x), 
							(FLOAT)(m_RequestGroupButton->GetWindowPos().y), 
							(FLOAT)(m_RequestGroupButton->GetWindowPos().x+m_RequestGroupButton->GetWindowSize().cx), 
							(FLOAT)(m_RequestGroupButton->GetWindowPos().y+m_RequestGroupButton->GetWindowSize().cy),
							0.0f,color);

			if(m_SendMessageButton->IsButtonEnabled())
				color = D3DCOLOR_ARGB(255, 240, 240, 240);
			else
				color = D3DCOLOR_ARGB(255, 121, 120, 112);
			
			_XDrawRectAngle((FLOAT)(m_SendMessageButton->GetWindowPos().x), 
							(FLOAT)(m_SendMessageButton->GetWindowPos().y), 
							(FLOAT)(m_SendMessageButton->GetWindowPos().x+m_SendMessageButton->GetWindowSize().cx), 
							(FLOAT)(m_SendMessageButton->GetWindowPos().y+m_SendMessageButton->GetWindowSize().cy),
							0.0f,color);


			if(m_RequestPVPButton->IsButtonEnabled())
				color = D3DCOLOR_ARGB(255, 240, 240, 240);
			else
				color = D3DCOLOR_ARGB(255, 121, 120, 112);
			
			_XDrawRectAngle((FLOAT)(m_RequestPVPButton->GetWindowPos().x), 
							(FLOAT)(m_RequestPVPButton->GetWindowPos().y), 
							(FLOAT)(m_RequestPVPButton->GetWindowPos().x+m_RequestPVPButton->GetWindowSize().cx), 
							(FLOAT)(m_RequestPVPButton->GetWindowPos().y+m_RequestPVPButton->GetWindowSize().cy),
							0.0f,color);

			if(m_RequestPVPGroupButton->IsButtonEnabled())
				color = D3DCOLOR_ARGB(255, 240, 240, 240);
			else
				color = D3DCOLOR_ARGB(255, 121, 120, 112);
			
			_XDrawRectAngle((FLOAT)(m_RequestPVPGroupButton->GetWindowPos().x), 
							(FLOAT)(m_RequestPVPGroupButton->GetWindowPos().y), 
							(FLOAT)(m_RequestPVPGroupButton->GetWindowPos().x+m_RequestPVPGroupButton->GetWindowSize().cx), 
							(FLOAT)(m_RequestPVPGroupButton->GetWindowPos().y+m_RequestPVPGroupButton->GetWindowSize().cy),
							0.0f,color);

		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
			if(m_pRequestMnDButton->IsButtonEnabled())
				color = D3DCOLOR_ARGB(255, 240, 240, 240);
			else
				color = D3DCOLOR_ARGB(255, 121, 120, 112);
			
			_XDrawRectAngle((FLOAT)(m_pRequestMnDButton->GetWindowPos().x), 
							(FLOAT)(m_pRequestMnDButton->GetWindowPos().y), 
							(FLOAT)(m_pRequestMnDButton->GetWindowPos().x+m_pRequestMnDButton->GetWindowSize().cx), 
							(FLOAT)(m_pRequestMnDButton->GetWindowPos().y+m_pRequestMnDButton->GetWindowSize().cy),
							0.0f,color);
		#endif
		}
	}

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
}

BOOL _XWindow_RequestParty::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if(!CheckMousePosition())
					this->ShowWindow(FALSE);
			}
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}