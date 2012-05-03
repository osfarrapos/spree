// XWindow_MessengerTooltip.cpp: implementation of the _XWindow_MessengerTooltip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "emperorofdragons.h"
#include "XWindow_MessengerTooltip.h"
#include "XWindow_Messenger.h"
#include "XWindow_MainMenu.h"
#include "XWindow_ChattingDefault.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MessengerTooltip::_XWindow_MessengerTooltip()
{
	m_AddGroupButton		= NULL;	
	m_DeleteGroupButton		= NULL;	
	m_ChangeGroupNameButton = NULL;
	m_DeleteFriendButton	= NULL;	
	m_CloseButton			= NULL;
	
//	m_bIsButtonProcess		= FALSE;

	m_MessengerTooltipMode = _XMESSENGERTOOLTIP_GROUPLIST;
	
	m_listboxUserStatus = NULL;
}

_XWindow_MessengerTooltip::~_XWindow_MessengerTooltip()
{
	DestroyWindow();
}



BOOL _XWindow_MessengerTooltip::Initialize(void)
{
	
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 0, 0 },		// 윈도우 좌표
		{ 94, 75 },	// 윈도우 사이즈
		_XDEF_MESSENGERTOOLTIPWINDOW_STATUSLISTBOX,			// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(""),											// 윈도우 타이틀
		15,												// 리스트 아이템 라인 피치
		5,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_NOSCROLLBAR						// 스크롤바의 위치
	};
	
	m_listboxUserStatus = new _XListBox;
	m_listboxUserStatus->Create(listboxstructure);
	
	
	#ifdef _XRUSSIAN
	POINT coloffset1 = { 2, 2 };
	#else
	POINT coloffset1 = { 5, 2 };
	#endif
	m_listboxUserStatus->SetColumnOffset(0, coloffset1);				// column offset	
	m_listboxUserStatus->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style 	
	m_listboxUserStatus->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type	
	m_listboxUserStatus->SetColumnWidth(0, 94);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	
	m_listboxUserStatus->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
	m_listboxUserStatus->SetSelectedImagePosition(0, 1);
	m_listboxUserStatus->SetSelectedImageHeight(15);
	m_listboxUserStatus->SetTrackWidth(94);
	
	m_listboxUserStatus->SetScrollPos(0);
	
	InsertChildObject(m_listboxUserStatus);
	SetListStatus();

	_XBTN_STRUCTURE messengertooltipbtnstruct = 
	{
		TRUE, 
		{5, 5},
		{112, 15},
		_XDEF_MESSENGERTOOLTIPWINDOW_ADDGROUPBUTTON,
		g_MainInterfaceTextureArchive.FindResource("maximummap_button.tga"),
		g_MainInterfaceTextureArchive.FindResource("maximummap_button.tga"),
		g_MainInterfaceTextureArchive.FindResource("maximummap_button.tga"),
		&g_MainInterfaceTextureArchive
	};
	m_AddGroupButton = new _XButton;
	m_AddGroupButton->Create(messengertooltipbtnstruct);
	m_AddGroupButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 0, 112, 15);
	m_AddGroupButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 15, 112, 30);
	m_AddGroupButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 30, 112, 45);
	m_AddGroupButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_AddGroupButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_CREATEGROUP);
	InsertChildObject(m_AddGroupButton);

	messengertooltipbtnstruct.position.y = 23;
	messengertooltipbtnstruct.commandid = _XDEF_MESSENGERTOOLTIPWINDOW_DELETEGROUPBUTTON;
	m_DeleteGroupButton = new _XButton;
	m_DeleteGroupButton->Create(messengertooltipbtnstruct);
	m_DeleteGroupButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 0, 112, 15);
	m_DeleteGroupButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 15, 112, 30);
	m_DeleteGroupButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 30, 112, 45);
	m_DeleteGroupButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_DeleteGroupButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_DELETEGROUP);
	InsertChildObject(m_DeleteGroupButton);
	
	messengertooltipbtnstruct.position.y = 41;
	messengertooltipbtnstruct.commandid = _XDEF_MESSENGERTOOLTIPWINDOW_CHANGERGROUPNAMEBUTTON;
	m_ChangeGroupNameButton = new _XButton;
	m_ChangeGroupNameButton->Create(messengertooltipbtnstruct);
	m_ChangeGroupNameButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 0, 112, 15);
	m_ChangeGroupNameButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 15, 112, 30);
	m_ChangeGroupNameButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 30, 112, 45);
	m_ChangeGroupNameButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_ChangeGroupNameButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_RENAMEGROUP);
	InsertChildObject(m_ChangeGroupNameButton);

	messengertooltipbtnstruct.position.y = 59;
	messengertooltipbtnstruct.commandid = _XDEF_MESSENGERTOOLTIPWINDOW_DELETEFRIENDBUTTON;
	m_DeleteFriendButton = new _XButton;
	m_DeleteFriendButton->Create(messengertooltipbtnstruct);
	m_DeleteFriendButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 0, 112, 15);
	m_DeleteFriendButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 15, 112, 30);
	m_DeleteFriendButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 30, 112, 45);
	m_DeleteFriendButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_DeleteFriendButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_DELETEFRIEND);
	InsertChildObject(m_DeleteFriendButton);
	

	_XBTN_STRUCTURE closebtnstruct = 
	{
		TRUE, 
		{5, 77},
		{112, 15},
		_XDEF_MESSENGERTOOLTIPWINDOW_CLOSEBUTTON,
		g_MainInterfaceTextureArchive.FindResource("maximummap_button.tga"),
		g_MainInterfaceTextureArchive.FindResource("maximummap_button.tga"),
		g_MainInterfaceTextureArchive.FindResource("maximummap_button.tga"),
		&g_MainInterfaceTextureArchive
	};
	m_CloseButton = new _XButton;
	m_CloseButton->Create(closebtnstruct);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 0, 112, 15);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 15, 112, 30);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 30, 112, 45);
	m_CloseButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_CloseButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_CANCEL);
	InsertChildObject(m_CloseButton);

	return TRUE;
}

void _XWindow_MessengerTooltip::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_MessengerTooltip::ShowWindow( BOOL show )
{
	_XWindow::ShowWindow( show );
}

void _XWindow_MessengerTooltip::Draw(_XGUIObject*& pfocusobject)
{	
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
	
	_XWindow::Draw(pfocusobject);

	if( m_MessengerTooltipMode == _XMESSENGERTOOLTIP_CHARACTERSTATE )
	{
		POINT listwinpos;
		SIZE listwinsize;
		
		listwinpos = m_listboxUserStatus->GetWindowPos();
		listwinsize = m_listboxUserStatus->GetWindowSize();
		
		_XDrawRectAngle( listwinpos.x, listwinpos.y,
			listwinpos.x+listwinsize.cx, listwinpos.y+listwinsize.cy, 0.0f, D3DCOLOR_ARGB(255,141,141,141) );
		
	}
}

BOOL _XWindow_MessengerTooltip::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;
	
	// 2004.05.03->oneway48 modify
	if( m_MessengerTooltipMode == _XMESSENGERTOOLTIP_CHARACTERSTATE )
	{
		int selectedpos = m_listboxUserStatus->GetSelectedItem();
		if( selectedpos != -1 )
		{
			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			pMessenger_Window->m_CurrentUserStatus = selectedpos;
			
			_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
			if(pMainMenu_Window)
			{
				pMainMenu_Window->SetMessengerStatus( pMessenger_Window->m_CurrentUserStatus );
			}

			// 로그인이라는 상태를 알려준다.
			g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, selectedpos );
			SetMode( _XMESSENGERTOOLTIP_NONE );
			this->ShowWindow(FALSE);
		}
	}
	
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
		
	return TRUE;
}

void _XWindow_MessengerTooltip::SetMode(_XMESSENGERTOOLTIP_MODE tooltipmode)
{
	switch(tooltipmode) 
	{	
	case _XMESSENGERTOOLTIP_NONE:
		{
			m_MessengerTooltipMode = _XMESSENGERTOOLTIP_NONE;
			m_listboxUserStatus->ShowWindow(FALSE);
			m_AddGroupButton->ShowWindow(FALSE);
			m_DeleteGroupButton->ShowWindow(FALSE);
			m_ChangeGroupNameButton->ShowWindow(FALSE);
			m_DeleteFriendButton->ShowWindow(FALSE);
			m_CloseButton->ShowWindow(FALSE);
			
			
		}
		break;
	case _XMESSENGERTOOLTIP_GROUPLIST:
		{
			m_MessengerTooltipMode = _XMESSENGERTOOLTIP_GROUPLIST;
			m_listboxUserStatus->ShowWindow(FALSE);
			
			m_AddGroupButton->ShowWindow(TRUE);
			m_DeleteGroupButton->ShowWindow(TRUE);
			m_ChangeGroupNameButton->ShowWindow(TRUE);
			m_DeleteFriendButton->ShowWindow(FALSE);
			m_CloseButton->ShowWindow(TRUE);
			
			m_WindowSize.cx = 122;
			m_WindowSize.cy = 77;	
			
			m_AddGroupButton->SetWindowSize(112,15);
			m_DeleteGroupButton->SetWindowSize(112,15);
			m_ChangeGroupNameButton->SetWindowSize(112,15);
			m_DeleteFriendButton->SetWindowSize(0,0);
			m_CloseButton->SetWindowSize(112,15);
			
			m_AddGroupButton->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + 5 );
			m_DeleteGroupButton->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + 23 );
			m_ChangeGroupNameButton->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + 41);
			m_CloseButton->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + 59 ); 
			
			
			m_solidborderstatic.MoveWindow(m_WindowPosition.x, m_WindowPosition.y
				, m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy);
			
		}
		break;
	case _XMESSENGERTOOLTIP_CHARACTERLIST:
		{
			m_MessengerTooltipMode = _XMESSENGERTOOLTIP_CHARACTERLIST;
			m_listboxUserStatus->ShowWindow(FALSE);
			m_AddGroupButton->ShowWindow(FALSE);
			m_DeleteGroupButton->ShowWindow(FALSE);
			m_ChangeGroupNameButton->ShowWindow(FALSE);
			m_DeleteFriendButton->ShowWindow(TRUE);
			m_CloseButton->ShowWindow(TRUE);
			
			m_WindowSize.cx = 122;
			m_WindowSize.cy = 41;			
			
			m_AddGroupButton->SetWindowSize(0,0);
			m_DeleteGroupButton->SetWindowSize(0,0);
			m_ChangeGroupNameButton->SetWindowSize(0,0);
			m_DeleteFriendButton->SetWindowSize(112,15);
			m_CloseButton->SetWindowSize(112,15);
			
			m_DeleteFriendButton->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + 5 );
			m_CloseButton->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + 23 );
			
			m_solidborderstatic.MoveWindow(m_WindowPosition.x, m_WindowPosition.y
				, m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy);
			
		}
		break;
	case _XMESSENGERTOOLTIP_CHARACTERSTATE:
		{
			m_MessengerTooltipMode = _XMESSENGERTOOLTIP_CHARACTERSTATE;
			
			m_AddGroupButton->SetWindowSize(0,0);
			m_DeleteGroupButton->SetWindowSize(0,0);
			m_ChangeGroupNameButton->SetWindowSize(0,0);
			m_DeleteFriendButton->SetWindowSize(0,0);
			m_CloseButton->SetWindowSize(0,0);

			m_AddGroupButton->ShowWindow(FALSE);
			m_DeleteGroupButton->ShowWindow(FALSE);
			m_ChangeGroupNameButton->ShowWindow(FALSE);
			m_DeleteFriendButton->ShowWindow(FALSE);
			m_CloseButton->ShowWindow(FALSE);
			
			m_listboxUserStatus->ShowWindow(TRUE);
			m_listboxUserStatus->SetWindowSize(95,75);

			m_WindowSize.cx = 95;
			m_WindowSize.cy = 75;
			
			m_solidborderstatic.MoveWindow(m_WindowPosition.x, m_WindowPosition.y
				, m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy);

			SetListStatus();
		}
		break;	
	}

}

void _XWindow_MessengerTooltip::SetListStatus()
{
	TCHAR	statusbuffer[128];
	
	m_listboxUserStatus->DeleteAllItem();
	
	for(int i = 0 ; i < 4 ; i++)
	{	
		memset(statusbuffer, 0, sizeof(TCHAR)*128);
		strcpy( statusbuffer, g_UserStatus[i]);

				
		m_listboxUserStatus->InsertListItem(_T(""), 0, 1);		
		m_listboxUserStatus->SetItemText(i, 0, statusbuffer);
		m_listboxUserStatus->SetItemAttrib(i, 0, D3DCOLOR_ARGB(255, 189, 188, 175));
		m_listboxUserStatus->SetItemAttribHighlight(i, 0, D3DCOLOR_ARGB( 255, 219, 196, 136 ));			
		
	}	

}

