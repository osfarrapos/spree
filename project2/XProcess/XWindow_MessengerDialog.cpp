// XWindow_MessengerDialog.cpp: implementation of the _XWindow_MessengerDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_MessengerDialog.h"

#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern D3DCOLOR g_ChattingStringColor[16];
void __stdcall _XIMECallback_MessengerDialogReturn( _XIMEContainer* pIMEContainer )
{
	DWORD windowid = pIMEContainer->GetCommandID();
	PostMessage(gHWnd, WM_COMMAND, (WPARAM)((windowid <<16)|_XMESSENGERDIALOGBUTTON_CHATINPUT), (LPARAM)gHWnd);
}

void __stdcall _XIMECallback_MessengerDialogEscape( _XIMEContainer* pIMEContainer )
{				
	DWORD windowid = pIMEContainer->GetCommandID();
	PostMessage(gHWnd, WM_COMMAND, (WPARAM)((windowid <<16)|_XMESSENGERDIALOGBUTTON_CLOSE), (LPARAM)gHWnd);
}

_XWindow_MessengerDialog::_XWindow_MessengerDialog()
{	
	m_imageInstanceMessageTitleBar			 = NULL;	// 메신져 인스턴트 메세지 창 타이틀 바	
	m_imageInstanceMessageEndLine			 = NULL;	// 메신져 인스턴트 메세지 창 끝 라인

	m_imageMessengerDialogTitleBar			 = NULL;	// 메신져 대화창 타이틀 바	
	m_imageMessengerDialogLeftSideBorder	 = NULL;	// 메신져 대화창 왼쪽 사이드 이미지
	m_imageMessengerDialogRightSideBorder	 = NULL;	// 메신져 대화창 오른쪽 사이드 이미지
	m_imageMessengerDialogDownBorder		 = NULL;	// 메신져 대화창 창 아래쪽  바탕
	m_imageMessengerDialogEmoticonBox		 = NULL;	// 메신져 대화창 이모티콘 박스 

	m_imageMessengerDialogAddTitleBar		 = NULL;	// Author :양희왕 //메신져 대화창 타이틀 바 아랫 부분 확장	
	m_pImageMessengerDialogBorder_1			 = NULL;	// Author :양희왕 //스승 제자 대화시 테두리
	m_pImageMessengerDialogBorder_2			 = NULL;	// Author :양희왕 //스승 제자 대화시 테두리
	m_pImageMessengerDialogBorder_3			 = NULL;	// Author :양희왕 //스승 제자 대화시 테두리
	m_pImageMessengerDialogBorder_4			 = NULL;	// Author :양희왕 //스승 제자 대화시 테두리

	m_pBtnMessengerMnDAccept				 = NULL;	// Author :양희왕 //스승 승인 버튼
	m_pBtnMessengerMnDReject				 = NULL;	// Author :양희왕 //스승 거절 버튼
	
	m_btnMessengerDialogHelp				 = NULL;	// 메신져 대화창 도움 버튼 
	m_btnMessengerDialogMinimize			 = NULL;	// 메신져 대화창 축소 버튼 
	m_btnMessengerDialogClose				 = NULL;	// 메신져 대화창 닫기 버튼 
	
	m_btnMessengerDialogInformation			 = NULL;	// 메신져 대화창 상대 정보 버튼	 
	m_btnMessengerDialogPartyRequest		 = NULL;	// 메신져 대화창 동행신청 버튼	
	m_btnMessengerDialogFriendCut			 = NULL;	// 메신져 대화창 차단 버튼 	
	m_btnMessengerDialogChatInput			 = NULL;	// 메신져 대화창 메세지 입력 버튼 	
	m_btnMessengerDialogEmoticonInput		 = NULL;	// 메신져 대화창 이모티콘 입력 버튼 
	
	m_MessengerChatListScrollPos			 = 0;			
	m_MessengeChatListScrollBar				 = NULL;	// 메신져 대화창 표시창 스크롤바 
	m_pScrbarFocusedObject					 = NULL;

	m_MessengerDialogMode					 = _XMESSENGERDIALOGMODE_INSTANCE;

	m_bReady								 = FALSE;	// 인스턴스 메세지 창에 마우스가 올려져 있을때 
	m_bMessageReceive						 = FALSE;	// 인스턴스 메세지 모드일때 메세지가 도착하면
	m_InstanceMessagePositionNumber			 = -1;		// 인스턴스 메세지 창의 위치를 나타내는 변수
	m_bGMChatting							 = FALSE;	// GM과 채팅인지 아닌지를 구분함.
}

_XWindow_MessengerDialog::~_XWindow_MessengerDialog()
{
	m_IMEControlMessengerDialog.ClearBuffer();
	_XIMEKERNEL.SetFocus( NULL );
}

_XWindow_MessengerDialog* _XWindow_MessengerDialog::CreateMessengerDialogBox( _XWindowManager* pWindowManager )
{
	for( int windowid = 60000; windowid < 60050; windowid++ )
	{
		if( !pWindowManager->FindWindow(windowid) )
		{
			_XWindow_MessengerDialog* pNewMessengerDialogBox = new _XWindow_MessengerDialog;
			pNewMessengerDialogBox->Create( pWindowManager, (DWORD)windowid );
			return pNewMessengerDialogBox;
		}
	}

	return NULL;
}

void _XWindow_MessengerDialog::Create( _XWindowManager* pWindowManager, DWORD windowid )
{
	m_WindowID = windowid;
	_XWINDOW_STRUCTURE windowstruct = 
	{
		TRUE, { (gnWidth >> 1) - (_XDEF_MESSENGERDIALOGWINDOW_WIDTH >>1),
			    (gnHeight>> 1) - (_XDEF_MESSENGERDIALOGWINDOW_HEIGHT>>1) }, 
				{ _XDEF_INSTANCEMESSAGEWINDOW_WIDTH, _XDEF_INSTANCEMESSAGEWINDOW_HEIGHT }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 200, 0, 0, 0 ),
			windowid
	};

	_XWindow::Create( windowstruct );
	
	m_solidborderstatic.Create( m_WindowPosition.x+1,m_WindowPosition.y+1, 
								m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+ m_WindowSize.cy -2 ,
								_XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR_ARGB( 200, 0, 0, 0 ) );


	int mainbackresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	int messengerresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	int messengerresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" );
	int charinforesourceindex = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	
	/////////////////////////////////////////////////////////////////////
	//                  메신져 인스턴스 메세지 모드                    //
	/////////////////////////////////////////////////////////////////////

	// 메신져 인스턴트 메세지 창 타이틀 바	( 183 X 57 )
	m_imageInstanceMessageTitleBar = new _XImageStatic;
	m_imageInstanceMessageTitleBar->Create( 0, 0, 180, 43, &g_MainInterfaceTextureArchive, mainbackresourceindex1 );
	m_imageInstanceMessageTitleBar->SetClipRect(0, 125, 183, 168);
	m_imageInstanceMessageTitleBar->ShowWindow(TRUE);
	InsertChildObject( m_imageInstanceMessageTitleBar );
	
	// 메신져 인스턴트 메세지 창 끝 라인	( 183 X 3 )
	m_imageInstanceMessageEndLine = new _XImageStatic;
	m_imageInstanceMessageEndLine->Create( 0, 43, 183, 46, &g_MainInterfaceTextureArchive, mainbackresourceindex1 );
	m_imageInstanceMessageEndLine->SetClipRect(0, 227, 183, 230);
	m_imageInstanceMessageEndLine->ShowWindow(TRUE);
	InsertChildObject( m_imageInstanceMessageEndLine );
	


	/////////////////////////////////////////////////////////////////////
	//                      메신져 대화창  모드                        //
	/////////////////////////////////////////////////////////////////////

	// 메신져 타이틀 바	( 250 X 57 )
	m_imageMessengerDialogTitleBar = new _XImageStatic;
	m_imageMessengerDialogTitleBar->Create( 0, 0, 250, 57, &g_MainInterfaceTextureArchive, messengerresourceindex2 ); 
	m_imageMessengerDialogTitleBar->SetClipRect(0, 0, 250, 56); //57
	m_imageMessengerDialogTitleBar->ShowWindow(FALSE);
	InsertChildObject( m_imageMessengerDialogTitleBar );
	
	// 메신져 대화창 왼쪽 사이드 이미지 ( 7 X 158 )
	m_imageMessengerDialogLeftSideBorder = new _XImageStatic;
	m_imageMessengerDialogLeftSideBorder->Create( 0, 57, 7, 215, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageMessengerDialogLeftSideBorder->SetClipRect(193, 0, 200, 158);
	m_imageMessengerDialogLeftSideBorder->ShowWindow(FALSE);
	InsertChildObject( m_imageMessengerDialogLeftSideBorder );
	
	// 메신져 대화창 오른쪽 사이드 이미지 ( 22 X 158 )
	m_imageMessengerDialogRightSideBorder = new _XImageStatic;
	m_imageMessengerDialogRightSideBorder->Create( 228, 57, 250, 215, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageMessengerDialogRightSideBorder->SetClipRect(201, 0, 223, 158);
	m_imageMessengerDialogRightSideBorder->ShowWindow(FALSE);
	InsertChildObject( m_imageMessengerDialogRightSideBorder );

	// 메신져 대화창 창 아래쪽  바탕 ( 250 X 63 ) // 사제 242 원래 215
	m_imageMessengerDialogDownBorder = new _XImageStatic;
	m_imageMessengerDialogDownBorder->Create( 0, 215, 250, 278, &g_MainInterfaceTextureArchive, messengerresourceindex2 );
	m_imageMessengerDialogDownBorder->SetClipRect(0, 60, 250, 123); 
	m_imageMessengerDialogDownBorder->ShowWindow(FALSE);
	InsertChildObject( m_imageMessengerDialogDownBorder );
	

	// 메신져 대화창 이모티콘 박스 ( 109 X 22 )
	m_imageMessengerDialogEmoticonBox = new _XImageStatic;
	m_imageMessengerDialogEmoticonBox->Create( 6, 220, 115, 242, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageMessengerDialogEmoticonBox->SetClipRect(33, 229, 142, 251);
	m_imageMessengerDialogEmoticonBox->ShowWindow(FALSE);
	InsertChildObject( m_imageMessengerDialogEmoticonBox );
	
	//메신져 도움 버튼 ( 12 X 12 ) :대화창일때 위치 207
	_XBTN_STRUCTURE helpstruct = { TRUE, { 153, 2 }, { 12, 12 }, (((DWORD)windowid<<16)|_XMESSENGERDIALOGBUTTON_HELP) ,
		charinforesourceindex,charinforesourceindex,charinforesourceindex, 
		&g_MainInterfaceTextureArchive };
	m_btnMessengerDialogHelp = new _XButton;
	m_btnMessengerDialogHelp->Create( helpstruct );		
	m_btnMessengerDialogHelp->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	m_btnMessengerDialogHelp->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	m_btnMessengerDialogHelp->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	m_btnMessengerDialogHelp->ShowWindow(TRUE);
	InsertChildObject( m_btnMessengerDialogHelp );
	
	//메신져 대화창 축소 버튼 ( 12 X 12 ) 
	_XBTN_STRUCTURE minimizestruct = { TRUE, { 220, 2 }, { 12, 12 }, (((DWORD)windowid<<16)|_XMESSENGERDIALOGBUTTON_MINIMIZE),
		charinforesourceindex,charinforesourceindex,charinforesourceindex, 
		&g_MainInterfaceTextureArchive };
	m_btnMessengerDialogMinimize = new _XButton;
	m_btnMessengerDialogMinimize->Create( minimizestruct );		
	m_btnMessengerDialogMinimize->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 155, 212, 167, 224);
	m_btnMessengerDialogMinimize->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  167, 212, 179, 224 );
	m_btnMessengerDialogMinimize->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  179, 212, 191, 224 );
	m_btnMessengerDialogMinimize->ShowWindow(FALSE);
	InsertChildObject( m_btnMessengerDialogMinimize );
	
	// 메신져 닫기 버튼	 ( 12 X 12 ) :대화창일때 위치 233
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, (((DWORD)windowid<<16)|_XMESSENGERDIALOGBUTTON_CLOSE),
		charinforesourceindex,charinforesourceindex,charinforesourceindex, 
		&g_MainInterfaceTextureArchive };
	m_btnMessengerDialogClose = new _XButton;
	m_btnMessengerDialogClose->Create( closebtnstruct );	
	m_btnMessengerDialogClose->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	m_btnMessengerDialogClose->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	m_btnMessengerDialogClose->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	m_btnMessengerDialogClose->ShowWindow(TRUE);
	InsertChildObject( m_btnMessengerDialogClose );
	
	// 메신져 대화창 상대 정보 버튼 ( 60 X 24 )
	_XBTN_STRUCTURE messengerdialogbtnstruct = 
	{
		TRUE, {186, 30}, {60, 24}, (((DWORD)windowid<<16)|_XMESSENGERDIALOGBUTTON_INFORMATION),
			messengerresourceindex1, messengerresourceindex1, messengerresourceindex1,
			&g_MainInterfaceTextureArchive
	};

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		int imageindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
		
		messengerdialogbtnstruct.position.x = m_WindowSize.cx - 110;
		messengerdialogbtnstruct.windowsize.cx = 108;
		messengerdialogbtnstruct.imageindex_click = imageindex;
		messengerdialogbtnstruct.imageindex_normal = imageindex;
		messengerdialogbtnstruct.imageindex_ready = imageindex;
	}

	m_btnMessengerDialogInformation = new _XButton;
	m_btnMessengerDialogInformation->Create(messengerdialogbtnstruct);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_btnMessengerDialogInformation->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 176, 110, 200);
		m_btnMessengerDialogInformation->SetButtonImageClipRect(_XBUTTONIMAGE_READY,  2, 152, 110, 176);
		m_btnMessengerDialogInformation->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 200, 110, 224);
	}
	else
	{
		m_btnMessengerDialogInformation->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
		m_btnMessengerDialogInformation->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
		m_btnMessengerDialogInformation->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	}

	messengerdialogbtnstruct.imageindex_click = messengerresourceindex1;
	messengerdialogbtnstruct.imageindex_normal = messengerresourceindex1;
	messengerdialogbtnstruct.imageindex_ready = messengerresourceindex1;

	m_btnMessengerDialogInformation->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_TARGETINFO );
	m_btnMessengerDialogInformation->EnableWindow(TRUE);
	m_btnMessengerDialogInformation->ShowWindow(FALSE);
	InsertChildObject(m_btnMessengerDialogInformation);
	
	// 메신져 대화창 동행신청 버튼 ( 60 X 24 )
	messengerdialogbtnstruct.commandid = (((DWORD)windowid<<16)|_XMESSENGERDIALOGBUTTON_PARTYREQUEST);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		messengerdialogbtnstruct.position.x = m_WindowSize.cx - 110 - 62;
		messengerdialogbtnstruct.position.y = 220;
		messengerdialogbtnstruct.windowsize.cx = 108;

		int imageindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
		
		messengerdialogbtnstruct.imageindex_click = imageindex;
		messengerdialogbtnstruct.imageindex_normal = imageindex;
		messengerdialogbtnstruct.imageindex_ready = imageindex;
	}
	else
	{
		messengerdialogbtnstruct.position.x = 144;
		messengerdialogbtnstruct.position.y = 220;
		messengerdialogbtnstruct.windowsize.cx = 60;
	}

	m_btnMessengerDialogPartyRequest = new _XButton;
	m_btnMessengerDialogPartyRequest->Create(messengerdialogbtnstruct);
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_btnMessengerDialogPartyRequest->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 176, 110, 200);
		m_btnMessengerDialogPartyRequest->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 152, 110, 176);
		m_btnMessengerDialogPartyRequest->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 200, 110, 224);
	}
	else
	{
		m_btnMessengerDialogPartyRequest->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
		m_btnMessengerDialogPartyRequest->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
		m_btnMessengerDialogPartyRequest->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	}
	m_btnMessengerDialogPartyRequest->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_REQPARTY );
	m_btnMessengerDialogPartyRequest->ShowWindow(FALSE);
	InsertChildObject(m_btnMessengerDialogPartyRequest);

	messengerdialogbtnstruct.imageindex_click = messengerresourceindex1;
	messengerdialogbtnstruct.imageindex_normal = messengerresourceindex1;
	messengerdialogbtnstruct.imageindex_ready = messengerresourceindex1;
	
	// 메신져 대화창 차단 버튼 ( 40 X 24 )
	messengerdialogbtnstruct.commandid = (((DWORD)windowid<<16)|_XMESSENGERDIALOGBUTTON_FRIENDCUT);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		messengerdialogbtnstruct.windowsize.cx = 60;
		messengerdialogbtnstruct.position.x = m_WindowSize.cx - 62;
		messengerdialogbtnstruct.position.y = 220;
	}
	else
	{
		messengerdialogbtnstruct.windowsize.cx = 40;
		messengerdialogbtnstruct.position.x = 205;
		messengerdialogbtnstruct.position.y = 220;		
	}
	
	m_btnMessengerDialogFriendCut = new _XButton;
	m_btnMessengerDialogFriendCut->Create(messengerdialogbtnstruct);
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_btnMessengerDialogFriendCut->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
		m_btnMessengerDialogFriendCut->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
		m_btnMessengerDialogFriendCut->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	}
	else
	{
		m_btnMessengerDialogFriendCut->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 75, 157, 115, 181);
		m_btnMessengerDialogFriendCut->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 32, 157, 72, 181);
		m_btnMessengerDialogFriendCut->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 118, 157, 158, 181);		
	}

	m_btnMessengerDialogFriendCut->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_INTERCEPTION );
	m_btnMessengerDialogFriendCut->ShowWindow(FALSE);
	m_btnMessengerDialogFriendCut->EnableWindow(FALSE);
	InsertChildObject(m_btnMessengerDialogFriendCut);
	
	// 메신져 대화창 메세지 입력 버튼 	 ( 40 X 24 )	
	messengerdialogbtnstruct.commandid = (((DWORD)windowid<<16)|_XMESSENGERDIALOGBUTTON_CHATINPUT);
	
	messengerdialogbtnstruct.windowsize.cx = 40;
	messengerdialogbtnstruct.position.x = 205;
	messengerdialogbtnstruct.position.y = 245;
	
	m_btnMessengerDialogChatInput = new _XButton;
	m_btnMessengerDialogChatInput->Create(messengerdialogbtnstruct);
	m_btnMessengerDialogChatInput->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 75, 157, 115, 181);
	m_btnMessengerDialogChatInput->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 32, 157, 72, 181);
	m_btnMessengerDialogChatInput->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 118, 157, 158, 181);
	m_btnMessengerDialogChatInput->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_INPUT );
	m_btnMessengerDialogChatInput->ShowWindow(FALSE);
	InsertChildObject(m_btnMessengerDialogChatInput);

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	//스승 제자 상단 테투리 이미지
	m_imageMessengerDialogAddTitleBar = new _XImageStatic;
	m_imageMessengerDialogAddTitleBar->Create( 0, 56, 250, 85, &g_MainInterfaceTextureArchive, messengerresourceindex2 );
	m_imageMessengerDialogAddTitleBar->SetClipRect(0, 28, 250, 57);
	m_imageMessengerDialogAddTitleBar->ShowWindow(FALSE);
	InsertChildObject( m_imageMessengerDialogAddTitleBar );

	int mainframeinterface		= g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	m_pImageMessengerDialogBorder_1 = new _XImageStatic;
	m_pImageMessengerDialogBorder_1->Create(5, 24, 245, 24, &g_MainInterfaceTextureArchive, mainframeinterface);
	m_pImageMessengerDialogBorder_1->SetClipRect(9, 193, 175, 194);
	m_pImageMessengerDialogBorder_1->SetScale(1.44f, 1.0f);
	m_pImageMessengerDialogBorder_1->ShowWindow(FALSE);
	InsertChildObject(m_pImageMessengerDialogBorder_1);

	m_pImageMessengerDialogBorder_2 = new _XImageStatic;
	m_pImageMessengerDialogBorder_2->Create(5, 78, 273, 78, &g_MainInterfaceTextureArchive, mainframeinterface);
	m_pImageMessengerDialogBorder_2->SetClipRect(9, 193, 175, 194);
	m_pImageMessengerDialogBorder_2->SetScale(1.44f, 1.0f);
	m_pImageMessengerDialogBorder_2->ShowWindow(FALSE);
	InsertChildObject(m_pImageMessengerDialogBorder_2);

	m_pImageMessengerDialogBorder_3	= new _XImageStatic;
	m_pImageMessengerDialogBorder_3->Create(4, 25, 4, 77, &g_MainInterfaceTextureArchive, mainframeinterface);
	m_pImageMessengerDialogBorder_3->SetClipRect(192, 64, 193, 116);
	m_pImageMessengerDialogBorder_3->SetScale(1.0f, 1.0f);
	m_pImageMessengerDialogBorder_3->ShowWindow(FALSE);
	InsertChildObject(m_pImageMessengerDialogBorder_3);

	m_pImageMessengerDialogBorder_4 = new _XImageStatic;
	m_pImageMessengerDialogBorder_4->Create(245, 25, 245, 77, &g_MainInterfaceTextureArchive, mainframeinterface);
	m_pImageMessengerDialogBorder_4->SetClipRect(192, 64, 193, 116);
	m_pImageMessengerDialogBorder_4->SetScale(1.0f, 1.0f);
	m_pImageMessengerDialogBorder_4->ShowWindow(FALSE);
	InsertChildObject(m_pImageMessengerDialogBorder_4);

	//승인 버튼
	messengerdialogbtnstruct.commandid = (((DWORD)windowid<<16)|_XMESSENGERDIALOGBUTTON_CHATINPUT);
	
	messengerdialogbtnstruct.windowsize.cx = 40;
	messengerdialogbtnstruct.position.x = 205;
	messengerdialogbtnstruct.position.y = 26;
	
	m_pBtnMessengerMnDAccept = new _XButton;
	m_pBtnMessengerMnDAccept->Create(messengerdialogbtnstruct);
	m_pBtnMessengerMnDAccept->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 75, 157, 115, 181);
	m_pBtnMessengerMnDAccept->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 32, 157, 72, 181);
	m_pBtnMessengerMnDAccept->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 118, 157, 158, 181);
	m_pBtnMessengerMnDAccept->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_ACCEPT ); //수락
	m_pBtnMessengerMnDAccept->ShowWindow(FALSE);
	InsertChildObject(m_pBtnMessengerMnDAccept);

	//거절 버튼
	messengerdialogbtnstruct.commandid = (((DWORD)windowid<<16)|_XMESSENGERDIALOGBUTTON_CHATINPUT);
	
	messengerdialogbtnstruct.windowsize.cx = 40;
	messengerdialogbtnstruct.position.x = 205;
	messengerdialogbtnstruct.position.y = 51;
	
	m_pBtnMessengerMnDReject = new _XButton;
	m_pBtnMessengerMnDReject->Create(messengerdialogbtnstruct);
	m_pBtnMessengerMnDReject->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 75, 157, 115, 181);
	m_pBtnMessengerMnDReject->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 32, 157, 72, 181);
	m_pBtnMessengerMnDReject->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 118, 157, 158, 181);
	m_pBtnMessengerMnDReject->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_REFUSE ); //거절
	m_pBtnMessengerMnDReject->ShowWindow(FALSE);
	InsertChildObject(m_pBtnMessengerMnDReject);
#endif
	
	//메신져 대화창 이모티콘 입력 버튼 ( 12 X 12 ) 
	_XBTN_STRUCTURE emoticonstruct = { TRUE, { 99, 226 }, { 12, 12 }, (((DWORD)windowid<<16)|_XMESSENGERDIALOGBUTTON_EMOTICON),
		chattingbuttonindex,chattingbuttonindex,chattingbuttonindex, 
		&g_MainInterfaceTextureArchive };
	m_btnMessengerDialogEmoticonInput = new _XButton;
	m_btnMessengerDialogEmoticonInput->Create( emoticonstruct );		
	m_btnMessengerDialogEmoticonInput->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0,  12, 12, 24 );
	m_btnMessengerDialogEmoticonInput->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  12, 12, 24, 24 );
	m_btnMessengerDialogEmoticonInput->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  24, 12, 36, 24 );
	m_btnMessengerDialogEmoticonInput->ShowWindow(FALSE);
	InsertChildObject( m_btnMessengerDialogEmoticonInput );
	
	m_IMEControlMessengerDialog.ResetContainer();
	m_IMEControlMessengerDialog.ResizeWindow(188, 12);
	m_IMEControlMessengerDialog.MoveWindow(m_WindowPosition.x + 10 , m_WindowPosition.y + 252);	
	m_IMEControlMessengerDialog.ShowWindow(FALSE);
	m_IMEControlMessengerDialog.ShowWindowRectAngle( FALSE );
	m_IMEControlMessengerDialog.SetMaxLength(80);
	m_IMEControlMessengerDialog.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
	m_IMEControlMessengerDialog.SetCommandID( windowid );
	m_IMEControlMessengerDialog.SetHorizScrollMode( TRUE );
	
	m_IMEControlMessengerDialog.SetReturnKeyCallback( _XIMECallback_MessengerDialogReturn );
	m_IMEControlMessengerDialog.SetEscapeKeyCallback( _XIMECallback_MessengerDialogEscape );
	
	// 메신져 대화창 표시창 스크롤바
	SAFE_DELETE( m_MessengeChatListScrollBar );
	m_MessengeChatListScrollBar = new _XVScrollBar;
	_XSCRBAR_STRUCTURE chatlistscrbarstructure = 
	{
		TRUE, { m_WindowPosition.x + 231, m_WindowPosition.y + 56 }, 
			12, 160, // width, height
			12,		 // btnysize
			22,		 // trackbar size
			_XDEF_CHATLIST_QUESIZE, 10, 0,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			&g_MainInterfaceTextureArchive
	};	
	m_MessengeChatListScrollBar->Create( chatlistscrbarstructure );
	m_MessengeChatListScrollBar->ShowWindow( FALSE );
	m_MessengeChatListScrollBar->SetViewBorder( TRUE );
	m_MessengeChatListScrollBar->SetTotalLineCount(0);	
	m_MessengeChatListScrollBar->UpdateData();
	m_MessengeChatListScrollBar->SetViewLineCount( 10 );
	
	_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
	{
		{ 0,  0, 12, 12 },
		{ 24, 0, 36, 12 },
		{ 0,  12, 12, 24 },
		{ 24, 12, 36, 24 },
		{ 40,157, 52, 179}
	};
	m_MessengeChatListScrollBar->SetClipRect( cliprectstructure );

	if( pWindowManager )
		pWindowManager->InsertWindow( this );
	
}

void _XWindow_MessengerDialog::DestroyWindow()
{
	_XWindow::DestroyWindow();	
}

void _XWindow_MessengerDialog::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);
	
	if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_INSTANCE)
	{
		if( !m_bReady )
		{
			// 대화 상대 박스 ( 164 X 17 )
			_XDrawSolidBar( m_WindowPosition.x + 10, m_WindowPosition.y + 23,
				m_WindowPosition.x + 174, m_WindowPosition.y + 40 , D3DCOLOR_ARGB(200,0,0,0));
			_XDrawRectAngle( m_WindowPosition.x + 10, m_WindowPosition.y + 23,
				m_WindowPosition.x + 174, m_WindowPosition.y + 40 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			
			TCHAR tempmessage[64];
			memset( tempmessage, 0, sizeof(tempmessage) );
			if( !m_bGMChatting )
			{
				sprintf(tempmessage,_XGETINTERFACETEXT(ID_STRING_MESSENGER_TOMESSAGE), m_ToCharactername );
			}
			else
			{
				sprintf(tempmessage,_XGETINTERFACETEXT(ID_STRING_MESSENGER_2532), m_ToCharactername );
			}
			int tabwidth = ( _XDEF_INSTANCEMESSAGEWINDOW_WIDTH - g_XBaseFont->GetWidth( tempmessage ) )/2;

			if( !m_bMessageReceive )
			{
				if( !m_bGMChatting )
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts( m_WindowPosition.x + tabwidth, m_WindowPosition.y + 26, tempmessage );		
					
					if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || 
						g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) // 어순 문제
					{
					}
					else
					{					
						//g_XBaseFont->Puts( m_WindowPosition.x + tabwidth+7, m_WindowPosition.y + 26, m_ToCharactername );  
						//Author : 양희왕 //breif : 무슨이유로 해놓았을까 //Date : 07/11/01
						//g_XBaseFont->Puts( m_WindowPosition.x + tabwidth, m_WindowPosition.y + 26, m_ToCharactername );  		
					}
				}
				else
				{
					g_XBaseFont->SetBoldMode(TRUE);
					
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,255,0));
					g_XBaseFont->Puts( m_WindowPosition.x + tabwidth, m_WindowPosition.y + 26, tempmessage );	
					
					g_XBaseFont->Flush();
					g_XBaseFont->SetBoldMode(FALSE);
				}	
			}
			else// 메세지가 도착하면 깜빡거림.
			{
				if( (gnFPS/3) < gnFrameCounter )
				{
					if( !m_bGMChatting )
					{
						g_XBaseFont->SetColor(_XSC_DEFAULT);
						g_XBaseFont->Puts( m_WindowPosition.x + tabwidth, m_WindowPosition.y + 26, tempmessage );		
						if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || 
							g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
						{
						}
						else
						{	
							//g_XBaseFont->Puts( m_WindowPosition.x + tabwidth+7, m_WindowPosition.y + 26, m_ToCharactername );  
							//Author : 양희왕 //breif : 무슨이유로 해놓았을까 //Date : 07/11/01
							//g_XBaseFont->Puts( m_WindowPosition.x + tabwidth, m_WindowPosition.y + 26, m_ToCharactername );  		
						}
					}
					else
					{
						g_XBaseFont->SetBoldMode(TRUE);
						
						g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,255,0));
						g_XBaseFont->Puts( m_WindowPosition.x + tabwidth, m_WindowPosition.y + 26, tempmessage );	

						g_XBaseFont->Flush();
						g_XBaseFont->SetBoldMode(FALSE);
					}
				}
			}

			g_XBaseFont->Flush();	
		}
		else
		{
			// 대화 상대 박스 ( 164 X 17 )
			_XDrawSolidBar( m_WindowPosition.x + 10, m_WindowPosition.y + 23,
				m_WindowPosition.x + 174, m_WindowPosition.y + 40 , D3DCOLOR_ARGB(200,0,0,0));
			_XDrawRectAngle( m_WindowPosition.x + 10, m_WindowPosition.y + 23,
				m_WindowPosition.x + 174, m_WindowPosition.y + 40 , 0.0f, D3DCOLOR_ARGB(255,149,149,149) );
			
			TCHAR tempmessage[64];
			memset( tempmessage, 0, sizeof(tempmessage) );
			if( !m_bGMChatting )
			{
				sprintf(tempmessage,_XGETINTERFACETEXT(ID_STRING_MESSENGER_TOMESSAGE), m_ToCharactername );
			}
			else
			{
				sprintf(tempmessage,_XGETINTERFACETEXT(ID_STRING_MESSENGER_2532), m_ToCharactername );
			}
			int tabwidth = ( _XDEF_INSTANCEMESSAGEWINDOW_WIDTH - g_XBaseFont->GetWidth( tempmessage ) )/2;
			
			if( !m_bMessageReceive )
			{
				if( !m_bGMChatting )
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
					g_XBaseFont->Puts( m_WindowPosition.x + tabwidth, m_WindowPosition.y + 26, tempmessage );		
					
					if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || 
						g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
					{
					}
					else
					{	
						//g_XBaseFont->Puts( m_WindowPosition.x + tabwidth+7, m_WindowPosition.y + 26, m_ToCharactername );  
						//Author : 양희왕 //breif : 무슨이유로 해놓았을까 //Date : 07/11/01
						g_XBaseFont->Puts( m_WindowPosition.x + tabwidth, m_WindowPosition.y + 26, m_ToCharactername );  
					}
				}
				else
				{
					g_XBaseFont->SetBoldMode(TRUE);
					
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,255,0));
					g_XBaseFont->Puts( m_WindowPosition.x + tabwidth, m_WindowPosition.y + 26, tempmessage );	

					g_XBaseFont->Flush();
					g_XBaseFont->SetBoldMode(FALSE);
				}
			}
			else// 메세지가 도착하면 깜빡거림.
			{
				if( (gnFPS/3) < gnFrameCounter )
				{
					if( !m_bGMChatting )
					{
						g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
						g_XBaseFont->Puts( m_WindowPosition.x + tabwidth, m_WindowPosition.y + 26, tempmessage );		
						if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || 
							g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
						{
						}
						else
						{	
							//g_XBaseFont->Puts( m_WindowPosition.x + tabwidth+7, m_WindowPosition.y + 26, m_ToCharactername );  
							//Author : 양희왕 //breif : 무슨이유로 해놓았을까 //Date : 07/11/01
							//g_XBaseFont->Puts( m_WindowPosition.x + tabwidth, m_WindowPosition.y + 26, m_ToCharactername );  
						}
					}
					else
					{
						g_XBaseFont->SetBoldMode(TRUE);
						
						g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,255,0));
						g_XBaseFont->Puts( m_WindowPosition.x + tabwidth, m_WindowPosition.y + 26, tempmessage );
						
						g_XBaseFont->Flush();
						g_XBaseFont->SetBoldMode(FALSE);
					}
				}
			}
			
			g_XBaseFont->Flush();
		}
		
	}
	else
	{
		// 대화 상대 박스 ( 107 X 20 )
	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
		if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_DIALOG )
	#endif
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				_XDrawSolidBar( m_WindowPosition.x + 8, m_WindowPosition.y + 32,
					m_WindowPosition.x + 120, m_WindowPosition.y + 52 , D3DCOLOR_ARGB(200,0,0,0));		
				_XDrawRectAngle( m_WindowPosition.x + 8, m_WindowPosition.y + 32,
					m_WindowPosition.x + 120, m_WindowPosition.y + 52 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			}
			else
			{
				_XDrawSolidBar( m_WindowPosition.x + 64, m_WindowPosition.y + 32,
					m_WindowPosition.x + 171, m_WindowPosition.y + 52 , D3DCOLOR_ARGB(200,0,0,0));		
				_XDrawRectAngle( m_WindowPosition.x + 64, m_WindowPosition.y + 32,
					m_WindowPosition.x + 171, m_WindowPosition.y + 52 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			}
			
			g_XBaseFont->SetBoldMode(TRUE);
			g_XBaseFont->SetColor(_XSC_DEFAULT);

			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->Puts( m_WindowPosition.x + 8, m_WindowPosition.y + 17, _XGETINTERFACETEXT(ID_STRING_MESSENGER_TARGETUSER) );
			}
			else
			{
				g_XBaseFont->Puts( m_WindowPosition.x + 8, m_WindowPosition.y + 37, _XGETINTERFACETEXT(ID_STRING_MESSENGER_TARGETUSER) );
			}

			g_XBaseFont->Flush();
			g_XBaseFont->SetBoldMode(FALSE);
		}
	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
		else if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MNDGROUPDIALOG ) //사제 대화 창일떄
		{
			//구현,,
		}

		if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MASTERDIALOG ) //스승이 보는 창
		{
			g_XBaseFont->SetColor( 0xFFFFD514 );
			g_XBaseFont->SetBoldMode(TRUE);
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 35, m_WindowPosition.y + 38, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3847));//_T("레벨"));
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 90, m_WindowPosition.y + 38, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_NAME)); //이름
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 155, m_WindowPosition.y + 38, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3848));//_T("직책"));
			g_XBaseFont->SetBoldMode(FALSE);
			g_XBaseFont->Flush();

			g_XBaseFont->SetColor( _XSC_DEFAULT );
			g_XBaseFont->SetBoldMode(TRUE);
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 35, m_WindowPosition.y + 38 + 16, _XFONT_ALIGNTYPE_CENTER, _T("레벨"));
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 90, m_WindowPosition.y + 38 + 16, _XFONT_ALIGNTYPE_CENTER, _T("asdfasdf"));
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 155, m_WindowPosition.y + 38 + 16, _XFONT_ALIGNTYPE_CENTER, _T("박투귀전투"));
			g_XBaseFont->SetBoldMode(FALSE);
			g_XBaseFont->Flush();
		}
		else if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_DISCIPLEDIALOG ) //제자가 보는 창
		{
			g_XBaseFont->SetColor( 0xFFFFD514 );
			g_XBaseFont->SetBoldMode(TRUE);
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 30, m_WindowPosition.y + 38, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3849));//_T("등급"));
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 105, m_WindowPosition.y + 38, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_NAME)); //이름
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 190, m_WindowPosition.y + 38, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3848));//_T("직책"));
			g_XBaseFont->SetBoldMode(FALSE);
			g_XBaseFont->Flush();

			g_XBaseFont->SetColor( _XSC_DEFAULT );
			g_XBaseFont->SetBoldMode(TRUE);
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 30, m_WindowPosition.y + 38 + 16, _XFONT_ALIGNTYPE_CENTER, _T("1"));
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 105, m_WindowPosition.y + 38 + 16, _XFONT_ALIGNTYPE_CENTER, _T("바보바보ㅇㅇ"));
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 190, m_WindowPosition.y + 38 + 16, _XFONT_ALIGNTYPE_CENTER, _T("녹림맹/전투"));
			g_XBaseFont->SetBoldMode(FALSE);
			g_XBaseFont->Flush();
		}
	#endif

		if( !m_bGMChatting ) //살짝 더러운데,,
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
			#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
				if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_DIALOG )
			#endif
				{
					g_XBaseFont->Puts( m_WindowPosition.x + 14, m_WindowPosition.y + 37, m_ToCharactername );
				}
			#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
				else if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MNDGROUPDIALOG ) //사제 대화 창일떄
				{
				}
			#endif
			}
			else
			{
			#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
				if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_DIALOG )
			#endif
				{
					g_XBaseFont->Puts( m_WindowPosition.x + 70, m_WindowPosition.y + 37, m_ToCharactername );
				}
			#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
				else if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MNDGROUPDIALOG ) //사제 대화 창일떄
				{
				}
			#endif
			}
		}
		else
		{			
			TCHAR tempmessage[64];
			memset( tempmessage, 0, sizeof(tempmessage) );
			sprintf(tempmessage,_T("[GM]%s"), m_ToCharactername ); //Author : 양희왕 //breif : \ 를 제거 했음
			
			g_XBaseFont->SetBoldMode(TRUE);

			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,255,0));

			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->Puts( m_WindowPosition.x + 14, m_WindowPosition.y + 37, tempmessage );
			}
			else
			{
				g_XBaseFont->Puts( m_WindowPosition.x + 70, m_WindowPosition.y + 37, tempmessage );
			}

			g_XBaseFont->Flush();
			g_XBaseFont->SetBoldMode(FALSE);
		}
		


		if( !m_MessengerChatList.listEmpty() )
		{
			m_MessengerChatList.resetList();
			for( int i = 0; i < m_MessengerChatListScrollPos; i++ ) m_MessengerChatList.nextItem();
			
			_XStringItem* curstring;
			int ypos = 0;
			DWORD colorindex;
									
			do
			{			
				curstring = (_XStringItem*)m_MessengerChatList.currentItem();			
				colorindex = (((DWORD)curstring->Get_Attribute()) % 16);
				DWORD offset  = HIWORD( (DWORD)curstring->Get_Attribute() );
				
				if(!m_bGMChatting)
				{
					g_XBaseFont->SetColor(g_ChattingStringColor[ colorindex ]);
				}
				else
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,255,255,0));			
				}

			#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
				if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MASTERDIALOG ||
					m_MessengerDialogMode == _XMESSENGERDIALOGMODE_DISCIPLEDIALOG ) //스승정보 제자정보 창일 때만 ,, 위치 이동
					g_XBaseFont->PutsAlign( this->m_WindowPosition.x + 13 + offset, (m_WindowPosition.y + 85) + (ypos * 16), _XFONT_ALIGNTYPE_LEFT, curstring->Get_String() );
				else
			#endif
				{
					g_XBaseFont->PutsAlign( this->m_WindowPosition.x + 13 + offset, (m_WindowPosition.y + 58) + (ypos * 16), _XFONT_ALIGNTYPE_LEFT, curstring->Get_String() );
				}
							
				ypos++;  
				
				if( ypos >= 10 ) break;			
				m_MessengerChatList.nextItem();			
			}while( !m_MessengerChatList.atHeadOfList() );
		}	
		
		m_MessengerChatListScrollPos = m_MessengeChatListScrollBar->GetLineScrollPos();
		m_MessengeChatListScrollBar->Draw( m_pScrbarFocusedObject );		
		g_XBaseFont->Flush();

		m_IMEControlMessengerDialog.Draw(pfocusobject);

	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
		if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MASTERDIALOG )
		{
			g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
			g_XBaseFont->SetColor(0xFFFFFFFF);
			g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 3, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3850));//_T("제자정보"));
			g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1)+1, m_WindowPosition.y + 3, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3850));//_T("제자정보"));
			g_XBaseFont->Flush();
			g_XBaseFont->DisableGrowMode();
		}
		else if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_DISCIPLEDIALOG )
		{
			g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
			g_XBaseFont->SetColor(0xFFFFFFFF);
			g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 3, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3851));//_T("스승정보"));
			g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1)+1, m_WindowPosition.y + 3, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3851));//_T("스승정보"));
			g_XBaseFont->Flush();
			g_XBaseFont->DisableGrowMode();
		}
	#endif
		
	}

	if(!this->m_ShowWindow)
		return;

}


BOOL _XWindow_MessengerDialog::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( m_MessengeChatListScrollBar )
	{
		if( m_MessengeChatListScrollBar->ProcessWheelMessage( zDelta ) )
		{
			return TRUE;
		}
		
		if( CheckMousePosition() )
		{
			int MoveDelta  = (FLOAT)zDelta / 40;			
			
			if( zDelta > 0 )
			{
				m_MessengeChatListScrollBar->LineUp( abs(MoveDelta) );
			}
			else
			{
				m_MessengeChatListScrollBar->LineDown( abs(MoveDelta) );
			}
			return TRUE;				
		}
	}
	
	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}

void _XWindow_MessengerDialog::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow( X, Y);

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	switch(m_MessengerDialogMode )
	{
	case _XMESSENGERDIALOGMODE_DIALOG :
	case _XMESSENGERDIALOGMODE_MNDGROUPDIALOG :
		{
			m_MessengeChatListScrollBar->MoveWindow( m_WindowPosition.x + 231 , m_WindowPosition.y + 56 );
			m_IMEControlMessengerDialog.MoveWindow( m_WindowPosition.x + 10 , m_WindowPosition.y + 252 );
		}
		break;
	case _XMESSENGERDIALOGMODE_MASTERDIALOG : //스승이 보는창
	case _XMESSENGERDIALOGMODE_DISCIPLEDIALOG : //제자가 보는창
		{
			m_MessengeChatListScrollBar->MoveWindow( m_WindowPosition.x + 231 , m_WindowPosition.y + 84);
			m_IMEControlMessengerDialog.MoveWindow( m_WindowPosition.x + 10 , m_WindowPosition.y + 252 );
		}
		break;
	}
#else
	if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_DIALOG)
	{
		m_MessengeChatListScrollBar->MoveWindow( m_WindowPosition.x + 231 , m_WindowPosition.y + 56 );
		m_IMEControlMessengerDialog.MoveWindow( m_WindowPosition.x + 10 , m_WindowPosition.y + 252 );
	}
#endif
}
BOOL _XWindow_MessengerDialog::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;
	
	if( m_MessengerDialogMode == _XMESSENGERDIALOGMODE_INSTANCE)
	{
		MouseState* mousestate = gpInput->GetMouseState();
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
		if(mousestate)
		{
			if( CheckMousePosition() )
			{
				m_bReady = TRUE;
				
				if(!pfocusobject)
				{
					if(mousestate->bButton[0])
					{
						if(scrnpos->x > (m_WindowPosition.x + 10) )
						{
							if( scrnpos->x < (m_WindowPosition.x + 174 ) )
							{
								if( scrnpos->z > (m_WindowPosition.y + 23) )
								{
									if( scrnpos->z < (m_WindowPosition.y + 53) )
									{
										SetMode( _XMESSENGERDIALOGMODE_DIALOG );
									}
								}
							}
						}
					}
				}
			}
			else
			{
				m_bReady = FALSE;
			}
		}
		
	}
	else
	{
		MouseState* mousestate = gpInput->GetMouseState();
		if(mousestate)
		{
			if(!pfocusobject)
			{
				if(mousestate->bButton[0])
				{
					if(CheckMousePosition() )//CheckIMEBoxArea()
					{
						m_IMEControlMessengerDialog.SetFocus();
					}
					else
					{
						m_IMEControlMessengerDialog.ClearBuffer();
						_XIMEKERNEL.SetFocus( NULL );						
					}
				}
			}
		}
		if( m_pScrbarFocusedObject ) return TRUE;
	}
	

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_MessengerDialog::ProcessDblClick()
{	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(m_MessengerDialogMode == _XMESSENGERDIALOGMODE_INSTANCE )
	{
		if(scrnpos->x > m_WindowPosition.x )
		{
			if( scrnpos->x < (m_WindowPosition.x + 183 ) )
			{
				if( scrnpos->z > m_WindowPosition.y )
				{
					if( scrnpos->z < (m_WindowPosition.y + 18) )
					{
						SetMode( _XMESSENGERDIALOGMODE_DIALOG );
					}
				}
			}
		}
	}
	else
	{
		if(scrnpos->x > m_WindowPosition.x )
		{
			if( scrnpos->x < (m_WindowPosition.x + 250 ) )
			{
				if( scrnpos->z > m_WindowPosition.y )
				{
					if( scrnpos->z < (m_WindowPosition.y + 18) )
					{
						SetMode( _XMESSENGERDIALOGMODE_INSTANCE );
					}
				}
			}
		}
	}
}

BOOL _XWindow_MessengerDialog::CheckIMEBoxArea(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 8) &&
		scrnpos->x < (m_WindowPosition.x + 205) &&
		scrnpos->z > (m_WindowPosition.y + 246) &&
		scrnpos->z < (m_WindowPosition.y + 268))
		return TRUE;
	
	return FALSE;
}

void _XWindow_MessengerDialog::ShowWindow(BOOL show)
{
	if(!show)
	{		
		m_IMEControlMessengerDialog.ClearBuffer();
		_XIMEKERNEL.SetFocus( NULL );
	}
	
	_XWindow::ShowWindow(show);
}

void _XWindow_MessengerDialog::SetMode( _XMESSENGERDIALOG_MODE messengerdialogmode )
{
	m_MessengerDialogMode = messengerdialogmode;
	
	switch(messengerdialogmode) {
	case _XMESSENGERDIALOGMODE_INSTANCE:
		{			
			SetWindowSize(_XDEF_INSTANCEMESSAGEWINDOW_WIDTH,_XDEF_INSTANCEMESSAGEWINDOW_HEIGHT);

			m_imageInstanceMessageTitleBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y);
			m_imageInstanceMessageEndLine->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + 43);

			m_btnMessengerDialogHelp->MoveWindow( m_WindowPosition.x + 153, m_WindowPosition.y + 2);
			m_btnMessengerDialogClose->MoveWindow( m_WindowPosition.x + 166, m_WindowPosition.y + 2);

			m_solidborderstatic.MoveWindow( m_WindowPosition.x+1,m_WindowPosition.y+1, 
				m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+ m_WindowSize.cy -2 );
			
			m_IMEControlMessengerDialog.ClearBuffer();
			_XIMEKERNEL.SetFocus( NULL );

			m_imageInstanceMessageTitleBar->ShowWindow(TRUE);			// 메신져 인스턴트 메세지 창 타이틀 바	
			m_imageInstanceMessageEndLine->ShowWindow(TRUE);			// 메신져 인스턴트 메세지 창 끝 라인
			
			m_imageMessengerDialogTitleBar->ShowWindow(FALSE);			// 메신져 대화창 타이틀 바	
			m_imageMessengerDialogLeftSideBorder->ShowWindow(FALSE);	// 메신져 대화창 왼쪽 사이드 이미지
			m_imageMessengerDialogRightSideBorder->ShowWindow(FALSE);	// 메신져 대화창 오른쪽 사이드 이미지
			m_imageMessengerDialogDownBorder->ShowWindow(FALSE);		// 메신져 대화창 창 아래쪽  바탕
			m_imageMessengerDialogEmoticonBox->ShowWindow(FALSE);		// 메신져 대화창 이모티콘 박스 
				
			m_btnMessengerDialogHelp->ShowWindow(TRUE);					// 메신져 대화창 도움 버튼 
			m_btnMessengerDialogMinimize->ShowWindow(FALSE);			// 메신져 대화창 축소 버튼 
			m_btnMessengerDialogClose->ShowWindow(TRUE);				// 메신져 대화창 닫기 버튼 
			
			m_btnMessengerDialogInformation->ShowWindow(FALSE);			// 메신져 대화창 상대 정보 버튼	 
			m_btnMessengerDialogPartyRequest->ShowWindow(FALSE);		// 메신져 대화창 동행신청 버튼	
			m_btnMessengerDialogFriendCut->ShowWindow(FALSE);			// 메신져 대화창 차단 버튼 	
			m_btnMessengerDialogChatInput->ShowWindow(FALSE);			// 메신져 대화창 메세지 입력 버튼 	
			m_btnMessengerDialogEmoticonInput->ShowWindow(FALSE);		// 메신져 대화창 이모티콘 입력 버튼 
			m_MessengeChatListScrollBar->ShowWindow(FALSE);				// 메신져 대화창 표시창 스크롤바 
			m_IMEControlMessengerDialog.ShowWindow(FALSE);				// 메신져 대화창 메세지 입력창 

		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
			m_imageMessengerDialogAddTitleBar->ShowWindow(FALSE);

			m_pImageMessengerDialogBorder_1->ShowWindow(FALSE); //안에 정보들 테두리
			m_pImageMessengerDialogBorder_2->ShowWindow(FALSE); //안에 정보들 테두리
			m_pImageMessengerDialogBorder_3->ShowWindow(FALSE); //안에 정보들 테두리
			m_pImageMessengerDialogBorder_4->ShowWindow(FALSE); //안에 정보들 테두리

			m_pBtnMessengerMnDAccept->ShowWindow(FALSE); //승인
			m_pBtnMessengerMnDReject->ShowWindow(FALSE); //거절
		#endif
		}
		break;

	case _XMESSENGERDIALOGMODE_DIALOG:
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	case _XMESSENGERDIALOGMODE_MNDGROUPDIALOG  : //사제 그룹대화
#endif
		{
			SetWindowSize(_XDEF_MESSENGERDIALOGWINDOW_WIDTH,_XDEF_MESSENGERDIALOGWINDOW_HEIGHT);
			
			m_btnMessengerDialogHelp->MoveWindow( m_WindowPosition.x + 207, m_WindowPosition.y + 2);
			m_btnMessengerDialogMinimize->MoveWindow( m_WindowPosition.x + 220, m_WindowPosition.y + 2);
			m_btnMessengerDialogClose->MoveWindow( m_WindowPosition.x + 233, m_WindowPosition.y + 2);

			m_imageMessengerDialogTitleBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y);
			m_imageMessengerDialogLeftSideBorder->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + 57);
			m_imageMessengerDialogRightSideBorder->MoveWindow( m_WindowPosition.x + 228, m_WindowPosition.y + 57);
			m_imageMessengerDialogDownBorder->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + 215 );
			m_imageMessengerDialogEmoticonBox->MoveWindow( m_WindowPosition.x + 6, m_WindowPosition.y + 220);

			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				m_btnMessengerDialogInformation->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 110, m_WindowPosition.y + 30);
				m_btnMessengerDialogPartyRequest->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 110 - 62, m_WindowPosition.y + 220);
				m_btnMessengerDialogFriendCut->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 62, m_WindowPosition.y + 220);
			}
			else
			{
				m_btnMessengerDialogInformation->MoveWindow( m_WindowPosition.x + 186, m_WindowPosition.y + 30);
				m_btnMessengerDialogPartyRequest->MoveWindow( m_WindowPosition.x +144, m_WindowPosition.y + 220);
				m_btnMessengerDialogFriendCut->MoveWindow( m_WindowPosition.x + 205, m_WindowPosition.y + 220);
			}		
			
			m_btnMessengerDialogChatInput->MoveWindow( m_WindowPosition.x + 205, m_WindowPosition.y + 245);
			m_btnMessengerDialogEmoticonInput->MoveWindow( m_WindowPosition.x + 99, m_WindowPosition.y + 226);
			
			m_MessengeChatListScrollBar->MoveWindow( m_WindowPosition.x + 231 , m_WindowPosition.y + 56);
			m_IMEControlMessengerDialog.MoveWindow( m_WindowPosition.x + 10 , m_WindowPosition.y + 252 );
			
			m_solidborderstatic.MoveWindow( m_WindowPosition.x+1,m_WindowPosition.y+1, 
				m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+ m_WindowSize.cy -2 );

			//사제가 추가 되면서 바뀌는 부분들을 다시 원래대로 돌림
		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
			m_imageMessengerDialogTitleBar->SetClipRect(0, 0, 250, 57);
			m_imageMessengerDialogDownBorder->SetClipRect(0, 60, 250, 123); 
			m_imageMessengerDialogAddTitleBar->ShowWindow(FALSE);

			m_pImageMessengerDialogBorder_1->ShowWindow(FALSE); //안에 정보들 테두리
			m_pImageMessengerDialogBorder_2->ShowWindow(FALSE); //안에 정보들 테두리
			m_pImageMessengerDialogBorder_3->ShowWindow(FALSE); //안에 정보들 테두리
			m_pImageMessengerDialogBorder_4->ShowWindow(FALSE); //안에 정보들 테두리

			m_pBtnMessengerMnDAccept->ShowWindow(FALSE); //승인
			m_pBtnMessengerMnDReject->ShowWindow(FALSE); //거절
		#endif
			
			m_imageInstanceMessageTitleBar->ShowWindow(FALSE);			// 메신져 인스턴트 메세지 창 타이틀 바	
			m_imageInstanceMessageEndLine->ShowWindow(FALSE);			// 메신져 인스턴트 메세지 창 끝 라인
			
			m_imageMessengerDialogTitleBar->ShowWindow(TRUE);			// 메신져 대화창 타이틀 바	
			m_imageMessengerDialogLeftSideBorder->ShowWindow(TRUE);		// 메신져 대화창 왼쪽 사이드 이미지
			m_imageMessengerDialogRightSideBorder->ShowWindow(TRUE);	// 메신져 대화창 오른쪽 사이드 이미지
			m_imageMessengerDialogDownBorder->ShowWindow(TRUE);			// 메신져 대화창 창 아래쪽  바탕
			m_imageMessengerDialogEmoticonBox->ShowWindow(FALSE);		// 메신져 대화창 이모티콘 박스 
		
			m_btnMessengerDialogHelp->ShowWindow(TRUE);					// 메신져 대화창 도움 버튼 
			m_btnMessengerDialogMinimize->ShowWindow(TRUE);				// 메신져 대화창 축소 버튼 
			m_btnMessengerDialogClose->ShowWindow(TRUE);				// 메신져 대화창 닫기 버튼 

			
		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
			if(m_MessengerDialogMode ==  _XMESSENGERDIALOGMODE_MNDGROUPDIALOG )
			{
				m_btnMessengerDialogInformation->ShowWindow(FALSE);			// 메신져 대화창 상대 정보 버튼	 
				m_btnMessengerDialogPartyRequest->ShowWindow(FALSE);		// 메신져 대화창 동행신청 버튼	
				m_btnMessengerDialogFriendCut->ShowWindow(FALSE);			// 메신져 대화창 차단 버튼 	
			}
			else
		#endif
			{
				m_btnMessengerDialogInformation->ShowWindow(TRUE);			// 메신져 대화창 상대 정보 버튼	 
				m_btnMessengerDialogPartyRequest->ShowWindow(TRUE);		// 메신져 대화창 동행신청 버튼	
				m_btnMessengerDialogFriendCut->ShowWindow(TRUE);			// 메신져 대화창 차단 버튼 	
			}
			
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				m_btnMessengerDialogChatInput->ShowWindow(FALSE);			// 메신져 대화창 메세지 입력 버튼 	
			}
			else
			{
				m_btnMessengerDialogChatInput->ShowWindow(TRUE);			// 메신져 대화창 메세지 입력 버튼 	
			}

			m_btnMessengerDialogEmoticonInput->ShowWindow(FALSE);		// 메신져 대화창 이모티콘 입력 버튼 
			m_MessengeChatListScrollBar->ShowWindow(TRUE);				// 메신져 대화창 표시창 스크롤바 
			
			m_IMEControlMessengerDialog.ShowWindow(TRUE);				// 메신져 대화창 메세지 입력창 
			m_IMEControlMessengerDialog.SetFocus();

			
			m_bReady = FALSE;
			m_bMessageReceive = FALSE;
		}
		break;
	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제

	case _XMESSENGERDIALOGMODE_MASTERDIALOG : //스승이 보는창
	case _XMESSENGERDIALOGMODE_DISCIPLEDIALOG : //제자가 보는창
		{
			SetWindowSize(_XDEF_MESSENGERDIALOGWINDOW_WIDTH,_XDEF_MESSENGERDIALOGWINDOW_HEIGHT);
			
			m_btnMessengerDialogHelp->MoveWindow( m_WindowPosition.x + 207, m_WindowPosition.y + 2);
			m_btnMessengerDialogMinimize->MoveWindow( m_WindowPosition.x + 220, m_WindowPosition.y + 2);
			m_btnMessengerDialogClose->MoveWindow( m_WindowPosition.x + 233, m_WindowPosition.y + 2);

			m_imageMessengerDialogEmoticonBox->MoveWindow( m_WindowPosition.x + 6, m_WindowPosition.y + 220);

			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				m_btnMessengerDialogInformation->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 110, m_WindowPosition.y + 30);
				m_btnMessengerDialogPartyRequest->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 110 - 62, m_WindowPosition.y + 220);
				m_btnMessengerDialogFriendCut->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 62, m_WindowPosition.y + 220);
			}
			else
			{
				m_btnMessengerDialogInformation->MoveWindow( m_WindowPosition.x + 186, m_WindowPosition.y + 30);
				m_btnMessengerDialogPartyRequest->MoveWindow( m_WindowPosition.x +144, m_WindowPosition.y + 220);
				m_btnMessengerDialogFriendCut->MoveWindow( m_WindowPosition.x + 205, m_WindowPosition.y + 220);
			}		
			
			m_btnMessengerDialogChatInput->MoveWindow( m_WindowPosition.x + 205, m_WindowPosition.y + 245);
			m_btnMessengerDialogEmoticonInput->MoveWindow( m_WindowPosition.x + 99, m_WindowPosition.y + 226);
			
			m_IMEControlMessengerDialog.MoveWindow( m_WindowPosition.x + 10 , m_WindowPosition.y + 252 );
			
			m_solidborderstatic.MoveWindow( m_WindowPosition.x+1,m_WindowPosition.y+1, 
				m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+ m_WindowSize.cy -2 );

			m_imageMessengerDialogTitleBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y);
			
			//--------사제가 추가 되면서 바뀌는 부분
			m_imageMessengerDialogTitleBar->SetClipRect(0, 0, 250, 56); //맨위 타이틀 부분 1픽셀 축소 금 가는 현상 때문
			m_imageMessengerDialogDownBorder->SetClipRect(0, 87, 250, 123); //바닥 이미지 사이즈를 변경
			m_imageMessengerDialogLeftSideBorder->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + 85); //왼쪽 사이드바 
			m_imageMessengerDialogRightSideBorder->MoveWindow( m_WindowPosition.x + 228, m_WindowPosition.y + 85); //오른쪽 사이드바
			m_imageMessengerDialogDownBorder->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + 242); // 바닥 텍스쳐 이동
			m_MessengeChatListScrollBar->MoveWindow( m_WindowPosition.x + 231 , m_WindowPosition.y + 84); //스크롤바 이동

			m_imageMessengerDialogAddTitleBar->ShowWindow(TRUE); //추가 되는 맨위 그림
			m_pImageMessengerDialogBorder_1->ShowWindow(TRUE); //안에 정보들 테두리
			m_pImageMessengerDialogBorder_2->ShowWindow(TRUE); //안에 정보들 테두리
			m_pImageMessengerDialogBorder_3->ShowWindow(TRUE); //안에 정보들 테두리
			m_pImageMessengerDialogBorder_4->ShowWindow(TRUE); //안에 정보들 테두리

			m_pBtnMessengerMnDAccept->ShowWindow(FALSE); //승인
			m_pBtnMessengerMnDReject->ShowWindow(FALSE); //거절

			if(m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MASTERDIALOG ) //스승이 보는 창일 때
			{
				m_pBtnMessengerMnDAccept->ShowWindow(TRUE); //승인
				m_pBtnMessengerMnDReject->ShowWindow(TRUE); //거절
			}
			//-------------------------------------------------------------------
			
			m_imageInstanceMessageTitleBar->ShowWindow(FALSE);			// 메신져 인스턴트 메세지 창 타이틀 바	
			m_imageInstanceMessageEndLine->ShowWindow(FALSE);			// 메신져 인스턴트 메세지 창 끝 라인
			
			m_imageMessengerDialogTitleBar->ShowWindow(TRUE);			// 메신져 대화창 타이틀 바	
			m_imageMessengerDialogLeftSideBorder->ShowWindow(TRUE);		// 메신져 대화창 왼쪽 사이드 이미지
			m_imageMessengerDialogRightSideBorder->ShowWindow(TRUE);	// 메신져 대화창 오른쪽 사이드 이미지
			m_imageMessengerDialogDownBorder->ShowWindow(TRUE);			// 메신져 대화창 창 아래쪽  바탕
			m_imageMessengerDialogEmoticonBox->ShowWindow(FALSE);		// 메신져 대화창 이모티콘 박스 
			
			m_btnMessengerDialogHelp->ShowWindow(TRUE);					// 메신져 대화창 도움 버튼 
			m_btnMessengerDialogMinimize->ShowWindow(TRUE);				// 메신져 대화창 축소 버튼 
			m_btnMessengerDialogClose->ShowWindow(TRUE);				// 메신져 대화창 닫기 버튼 
			
			m_btnMessengerDialogInformation->ShowWindow(FALSE);			// 메신져 대화창 상대 정보 버튼	 
			m_btnMessengerDialogPartyRequest->ShowWindow(FALSE);			// 메신져 대화창 동행신청 버튼	
			m_btnMessengerDialogFriendCut->ShowWindow(FALSE);			// 메신져 대화창 차단 버튼 	
			
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				m_btnMessengerDialogChatInput->ShowWindow(FALSE);			// 메신져 대화창 메세지 입력 버튼 	
			}
			else
			{
				m_btnMessengerDialogChatInput->ShowWindow(TRUE);			// 메신져 대화창 메세지 입력 버튼 	
			}

			m_btnMessengerDialogEmoticonInput->ShowWindow(FALSE);		// 메신져 대화창 이모티콘 입력 버튼 
			m_MessengeChatListScrollBar->ShowWindow(TRUE);				// 메신져 대화창 표시창 스크롤바 
			
			m_IMEControlMessengerDialog.ShowWindow(TRUE);				// 메신져 대화창 메세지 입력창 
			m_IMEControlMessengerDialog.SetFocus();

			
			m_bReady = FALSE;
			m_bMessageReceive = FALSE;
		}
		break;
	#endif
	}
}