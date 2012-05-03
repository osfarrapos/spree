// XWindow_Messenger.cpp: implementation of the _XWindow_Messenger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_Messenger.h"
#include "XWindow_MessengerDialog.h"
#include "XSR_STRINGHEADER.H"
#include "XMessengerGroup.h"
#include "XMessengerFriend.h"
#include "XWindow_RecallItem.h" //Author : 양희왕 //breif : 친구소환
#include "XWindow_MessengerTooltip.h"
#include "XWindow_ChattingDefault.h"
#include "XWindow_SystemMsgDefault.h"
#include "XWindow_MainMenu.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define _XDEF_MESSENGERWINDOWMINSIZE_WIDTH		183
#define _XDEF_MESSENGERWINDOWMINSIZE_HEIGHT		206

#define _XDEF_MESSENGERWINDOWMAXSIZE_WIDTH		183
#define _XDEF_MESSENGERWINDOWMAXSIZE_HEIGHT		530 


_XWindow_Messenger::_XWindow_Messenger() : m_btnMessengerRecallSystem(NULL)
{	
	
	m_imageMessengerTitleBar = NULL;// 메신져 타이틀 바	
	m_imageMessengerStateBorder = NULL;// 메신져 상태 전환, 로그인 로그아웃 바탕	
	m_imageMessengerDownBorder = NULL;// 메신져 창 아래쪽  바탕	
	m_imageMessengerDockingBorder = NULL;// 메신져가 메인 메뉴창에 도킹했을때 생성되는 바탕
	m_imageMessengerLeftSideBorder = NULL;// 메신져 왼쪽 사이드 이미지
	m_imageMessengerRightSideBorder = NULL;// 메신져 오른쪽 사이드 이미지	
	m_imageMessengerDownEndLine = NULL;// 메신져 창 아래쪽 끝 라인
	
	m_btnMessengerClose = NULL;// 메신져 닫기 버튼 
	m_btnMessengerHelp = NULL;//메신져 도움 버튼 

	m_btnMessengerLogIn = NULL;// 메신져 로그인 버튼
	m_btnMessengerLogOut = NULL;// 메신져 로그아웃 버튼 	
	m_btnMessengerAddFriend = NULL;// 메신져 친구추가 버튼 	
	m_btnMessengerAddFriendGroup = NULL;// 메신져 그룹추가 버튼 
	m_btnChangeStatus	= NULL;//메신져 상태전환 버튼 

	m_SelectedItemIndex = -1;


	m_Resizing				= FALSE;
	m_ResizeMode			= _XWRESIZE_NONE;
		
	memset( m_SelectedCharactername, 0, sizeof(TCHAR)*13 ); 
	memset( m_SelectedGroupname, 0, sizeof(TCHAR)*15 ); 

	
	memset( m_CharacterName, 0, sizeof(TCHAR)*15 ); 
	memset(m_GroupName, 0, sizeof(TCHAR)*15);	
	memset(m_GroupReName, 0, sizeof(TCHAR)*15);	
	
	memset( m_IsDragingCharacterName, 0, sizeof(TCHAR)*13 ); 
	memset( m_IsDragingGroupName, 0, sizeof(TCHAR)*15 );
	memset( m_IsDragingText, 0, sizeof(TCHAR)*256 );
	m_IsDragingCharacterNumber = -1;
	m_IsDragingGroupNumber = -1;

	m_CurrentUserStatus = 0;

	m_listboxGroupList = NULL;
	
	m_DragingDisplayBox = NULL;
	m_bDragingDisplayBox = FALSE;
	m_bDrawDragingDisplayBox = FALSE;
	m_bDockingMode = FALSE;

	m_bSelectedGroup = FALSE;
	m_bClickRightButton = FALSE;
	m_bClickLeftButton = FALSE;
	m_bDblClick			= FALSE;

	
	m_TotalCountFriends = 0;
	
	m_DragStartPosX = 0;	// 마우스 오른쪽 클릭시  다른 쪽에서 클릭해서 
	m_DragStartPosY = 0;	//움직여서 왔을 때는 툴팁이 뜨지 않게 하게 위한 변수
	
	m_ClickRightStartPosX = 0;	// 마우스 오른쪽 클릭시  다른 쪽에서 클릭해서 
	m_ClickRightStartPosY = 0;	//움직여서 왔을 때는 툴팁이 뜨지 않게 하게 위한 변수
	
	
	m_MaxViewCount = 10;		// 2004.05.12->oneway48 insert 
}

_XWindow_Messenger::~_XWindow_Messenger()
{
	m_MessengerGroupList.disposeList();
}

BOOL _XWindow_Messenger::Initialize( void )
{	
	
	int mainbackresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	int messengerresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	int messengerresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" );

	// 메신져 타이틀 바	( 183 X 18 )
	m_imageMessengerTitleBar = new _XImageStatic;
	m_imageMessengerTitleBar->Create( 0, 0, 183, 18, &g_MainInterfaceTextureArchive, mainbackresourceindex1 );
	m_imageMessengerTitleBar->SetClipRect(0, 125, 183, 143);
	m_imageMessengerTitleBar->ShowWindow(TRUE);
	InsertChildObject( m_imageMessengerTitleBar );
	
	// 메신져 상태 전환, 로그인 로그아웃 바탕	( 183 X 31 )
	m_imageMessengerStateBorder = new _XImageStatic;
	m_imageMessengerStateBorder->Create( 0, 18, 183, 49, &g_MainInterfaceTextureArchive, mainbackresourceindex1 );
	m_imageMessengerStateBorder->SetClipRect(0, 196, 183, 227);
	m_imageMessengerStateBorder->ShowWindow(TRUE);
	InsertChildObject( m_imageMessengerStateBorder );

	// 메신져 왼쪽 사이드 이미지 ( 10 X 158 )
	m_imageMessengerLeftSideBorder = new _XImageStatic;
	m_imageMessengerLeftSideBorder->Create( 0, 49, 10, 207, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageMessengerLeftSideBorder->SetClipRect(226, 0, 236, 158);
	m_imageMessengerLeftSideBorder->ShowWindow(TRUE);
	InsertChildObject( m_imageMessengerLeftSideBorder );

	// 메신져 오른쪽 사이드 이미지 ( 20 X 158 )
	m_imageMessengerRightSideBorder = new _XImageStatic;
	m_imageMessengerRightSideBorder->Create( 163, 49, 183, 207, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageMessengerRightSideBorder->SetClipRect(236, 0, 256, 158);
	m_imageMessengerRightSideBorder->ShowWindow(TRUE);
	InsertChildObject( m_imageMessengerRightSideBorder );
		
	// 메신져 창 아래쪽  바탕	 ( 183 X 52 )
	m_imageMessengerDownBorder = new _XImageStatic;
	m_imageMessengerDownBorder->Create( 0, 207, 183, 259, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageMessengerDownBorder->SetClipRect( 0, 26, 183, 78);
	m_imageMessengerDownBorder->ShowWindow(TRUE);
	InsertChildObject( m_imageMessengerDownBorder );
	
	// 메신져가 메인 메뉴창에 도킹했을때 생성되는 바탕	 ( 183 X 21 )
	m_imageMessengerDockingBorder = new _XImageStatic;
	m_imageMessengerDockingBorder->Create( 0, 259, 183, 280, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageMessengerDockingBorder->SetClipRect( 0, 79, 183, 100);
	m_imageMessengerDockingBorder->ShowWindow(TRUE);
	InsertChildObject( m_imageMessengerDockingBorder );

	// 메신져 창 아래쪽 끝 라인	 ( 183 X 3 )
	m_imageMessengerDownEndLine = new _XImageStatic;
	m_imageMessengerDownEndLine->Create( 0, 280, 183, 283, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageMessengerDownEndLine->SetClipRect( 0, 106, 183, 109);
#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER //Author : 양희왕
	m_imageMessengerDownEndLine->ShowWindow(TRUE);
#else
	m_imageMessengerDownEndLine->ShowWindow(FALSE);
#endif
	InsertChildObject( m_imageMessengerDownEndLine );

	// 메신져 닫기 버튼	 ( 12 X 12 ) 
	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_MESSENGERWINDOW_CLOSEBUTTON,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	m_btnMessengerClose = new _XButton;
	m_btnMessengerClose->Create( closebtnstruct );	
	
	m_btnMessengerClose->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	m_btnMessengerClose->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	m_btnMessengerClose->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( m_btnMessengerClose );
	
	//메신져 도움 버튼 ( 12 X 12 ) 
	_XBTN_STRUCTURE helpstruct = { TRUE, { 153, 2 }, { 12, 12 }, _XDEF_MESSENGERWINDOW_HELPBUTTON,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	m_btnMessengerHelp = new _XButton;
	m_btnMessengerHelp->Create( helpstruct );	
	
	m_btnMessengerHelp->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	m_btnMessengerHelp->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	m_btnMessengerHelp->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( m_btnMessengerHelp );


	// 메신져 로그인 버튼 ( 60 X 24 )
	_XBTN_STRUCTURE messengerbtnstruct = 
	{
		TRUE, {107, 21}, {60, 24}, _XDEF_MESSENGERWINDOW_MESSENGERLOGIN,
			messengerresourceindex1, messengerresourceindex1, messengerresourceindex1,
			&g_MainInterfaceTextureArchive
	};
	
	m_btnMessengerLogIn = new _XButton;
	m_btnMessengerLogIn->Create(messengerbtnstruct);
	m_btnMessengerLogIn->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_btnMessengerLogIn->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_btnMessengerLogIn->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	m_btnMessengerLogIn->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_LOGIN );	// _T("로그인")
	m_btnMessengerLogIn->ShowWindow(FALSE);
	InsertChildObject(m_btnMessengerLogIn);
	
	
	// 메신져 로그아웃 버튼 ( 60 X 24 )
	messengerbtnstruct.commandid = _XDEF_MESSENGERWINDOW_MESSENGERLOGOUT;

	m_btnMessengerLogOut = new _XButton;
	m_btnMessengerLogOut->Create(messengerbtnstruct);
	m_btnMessengerLogOut->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_btnMessengerLogOut->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_btnMessengerLogOut->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	m_btnMessengerLogOut->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_LOGOUT);	// _T("로그아웃")
	m_btnMessengerLogOut->ShowWindow(TRUE);
	InsertChildObject(m_btnMessengerLogOut);
	
	// 메신져 친구추가 버튼 ( 60 X 24 )
	messengerbtnstruct.commandid = _XDEF_MESSENGERWINDOW_ADDFRIENDBUTTON;
	messengerbtnstruct.position.x = 10;
	messengerbtnstruct.position.y = 211;

	#if defined(_XJAPANESE) || defined(_XRUSSIAN)
	messengerbtnstruct.windowsize.cx = 81;
	messengerbtnstruct.windowsize.cy = 17;
	int npcresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
	messengerbtnstruct.imageindex_click = messengerbtnstruct.imageindex_normal = messengerbtnstruct.imageindex_ready = npcresourceindex1;
	#endif
	
	m_btnMessengerAddFriend = new _XButton;
	m_btnMessengerAddFriend->Create(messengerbtnstruct);
	#if defined(_XJAPANESE) || defined(_XRUSSIAN)
	m_btnMessengerAddFriend->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 429, 39, 510, 57);
	m_btnMessengerAddFriend->SetButtonImageClipRect(_XBUTTONIMAGE_READY,  429, 59, 510, 77);
	m_btnMessengerAddFriend->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK,  429, 79, 510, 97);
	#else
	m_btnMessengerAddFriend->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_btnMessengerAddFriend->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_btnMessengerAddFriend->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	#endif
	m_btnMessengerAddFriend->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_FRIEND );	// _T("친구추가")
	m_btnMessengerAddFriend->ShowWindow(TRUE);
	InsertChildObject(m_btnMessengerAddFriend);
	
	
	// 메신져 그룹추가 버튼 ( 60 X 24 )
	messengerbtnstruct.commandid = _XDEF_MESSENGERWINDOW_ADDGROUPBUTTON;
	#if defined(_XJAPANESE) || defined(_XRUSSIAN)
		messengerbtnstruct.position.x = 95;
	#else
		messengerbtnstruct.position.x = 70;
	#endif
	
	m_btnMessengerAddFriendGroup = new _XButton;
	m_btnMessengerAddFriendGroup->Create(messengerbtnstruct);
	#if defined(_XJAPANESE) || defined(_XRUSSIAN)
	m_btnMessengerAddFriendGroup->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 429, 39, 510, 57 );
	m_btnMessengerAddFriendGroup->SetButtonImageClipRect(_XBUTTONIMAGE_READY,  429, 59, 510, 77 );
	m_btnMessengerAddFriendGroup->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK,  429, 79, 510, 97 );
	#else
	m_btnMessengerAddFriendGroup->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_btnMessengerAddFriendGroup->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_btnMessengerAddFriendGroup->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	#endif
	m_btnMessengerAddFriendGroup->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_GROUP );	// _T("그룹추가")
	m_btnMessengerAddFriendGroup->ShowWindow(TRUE);
	InsertChildObject(m_btnMessengerAddFriendGroup);

	#if defined(_XJAPANESE) || defined(_XRUSSIAN)
	// restore.
	messengerbtnstruct.windowsize.cx = 60;
	messengerbtnstruct.windowsize.cy = 24;
	messengerbtnstruct.imageindex_click = messengerbtnstruct.imageindex_normal = messengerbtnstruct.imageindex_ready = messengerresourceindex1;
	#endif

#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER //Author : 양희왕
	//소환 버튼
	messengerbtnstruct.commandid = _XDEF_MESSENGERWINDOW_RECALLBUTTON;
	messengerbtnstruct.position.x = 10;
	messengerbtnstruct.position.y = 240;
	
	m_btnMessengerRecallSystem = new _XButton;
	m_btnMessengerRecallSystem->Create(messengerbtnstruct);
	m_btnMessengerRecallSystem->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_btnMessengerRecallSystem->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_btnMessengerRecallSystem->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	m_btnMessengerRecallSystem->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_GMCOMMAND_2479);	// _T("소환")
	m_btnMessengerRecallSystem->ShowWindow(TRUE);
	m_btnMessengerRecallSystem->EnableWindow(FALSE);
	InsertChildObject(m_btnMessengerRecallSystem);
#endif

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	messengerbtnstruct.commandid = _XDEF_MESSENGERWINDOW_RECALLBUTTON;
	
	#if defined(_XJAPANESE) || defined(_XRUSSIAN)
	messengerbtnstruct.position.x = 95;
	#else
	messengerbtnstruct.position.x = 70;
	#endif
	messengerbtnstruct.position.y = 240;
	
	m_btnMessengerMnDSystem = new _XButton;
	m_btnMessengerMnDSystem->Create(messengerbtnstruct);
	m_btnMessengerMnDSystem->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_btnMessengerMnDSystem->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_btnMessengerMnDSystem->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	m_btnMessengerMnDSystem->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3880);	// _T("사제대화")
	m_btnMessengerMnDSystem->ShowWindow(TRUE);
	m_btnMessengerMnDSystem->EnableWindow(FALSE);
	InsertChildObject(m_btnMessengerMnDSystem);
#endif
	
	// 메신져 상태전환 버튼 ( 12 X 12 )
	int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	_XBTN_STRUCTURE statusbtnstruct = 
	{
		TRUE, {90, 27}, {12, 12}, _XDEF_MESSENGERWINDOW_CHANGESTATUSBUTTON,
			chattingbuttonindex, chattingbuttonindex, chattingbuttonindex,
			&g_MainInterfaceTextureArchive
	};
	
	m_btnChangeStatus = new _XButton;
	m_btnChangeStatus->Create(statusbtnstruct);
	m_btnChangeStatus->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 12, 12, 24);
	m_btnChangeStatus->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 12, 12, 24, 24);
	m_btnChangeStatus->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 24, 12, 36, 24);
	m_btnChangeStatus->ShowWindow(TRUE);
	InsertChildObject(m_btnChangeStatus);
	

	//메신져 그룹과 친구리스트를 표시하는 리스트 박스 
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ m_WindowPosition.x + 8, m_WindowPosition.y + 48  },		// 윈도우 좌표
		{ 168, 160 },										// 윈도우 사이즈
		_XDEF_MESSENGERWINDOW_LISTBOX,					// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(""),											// 윈도우 타이틀
		16,												// 리스트 아이템 라인 피치
		m_MaxViewCount,									// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치

		// scroll bar property
		{ 12, 160 },									// 스크롤바 사이즈
		12,												// 스크롤바 상하 버튼 사이즈
		22,												// 스크롤바 트랙바 사이즈
		58,												// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex

	};
	m_listboxGroupList = new _XListBox;
	m_listboxGroupList->Create(listboxstructure);
	
	_XVScrollBar* pScrollBar = m_listboxGroupList->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}
	
	POINT coloffset1 = { 15, 3 };
	POINT coloffset2 = { 25, 3 };
	POINT coloffset3 = { 2, 3 }; 
	POINT coloffset4 = { 12, 3 };
	m_listboxGroupList->SetColumnOffset(0, coloffset1);				// column offset
	m_listboxGroupList->SetColumnOffset(1, coloffset2);
	m_listboxGroupList->SetColumnOffset(2, coloffset3);				
	m_listboxGroupList->SetColumnOffset(3, coloffset4);
	
	m_listboxGroupList->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style 
	m_listboxGroupList->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_listboxGroupList->SetColumnStyle(2, _XLCOLUMNSTYLE_ICONRECT);		// column style 
	m_listboxGroupList->SetColumnStyle(3, _XLCOLUMNSTYLE_ICONRECT);
	
	m_listboxGroupList->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxGroupList->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_listboxGroupList->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxGroupList->SetAlignType(3, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxGroupList->SetColumnWidth(0, 155);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxGroupList->SetColumnWidth(1, 155 );
	m_listboxGroupList->SetColumnWidth(2, 15);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxGroupList->SetColumnWidth(3, 25 );
	
	m_listboxGroupList->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
	m_listboxGroupList->SetSelectedImagePosition(0, 1);
	m_listboxGroupList->SetSelectedImageHeight(16);
	m_listboxGroupList->SetTrackWidth(155);
	
	m_listboxGroupList->SetScrollPos(0);
	m_listboxGroupList->SetAlwaysShowScrollBarMode(TRUE);
	InsertChildObject(m_listboxGroupList);
		
	// 메신져 리스트에서 드래깅 되는 친구 이름이나 그룹 이름을 표시함
	_XTOOLTIP_STRUCTURE draginglistboxstruct = 
	{
		{ 0, 0 },
		{ 140, 15 },
		{ 1, 1 },
		-1,
		NULL
	};
	m_DragingDisplayBox = new _XToolTip;
	m_DragingDisplayBox->Create(draginglistboxstruct);
	m_DragingDisplayBox->ShowWindow(FALSE);
		
	return TRUE;
}

void _XWindow_Messenger::SetDefaultPosition( void )
{		
}

void _XWindow_Messenger::SaveWindowProperty( void )
{
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MESSENGERWINDOWXPOS,   GetWindowPos().x );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MESSENGERWINDOWYPOS,   GetWindowPos().y  );
}

void _XWindow_Messenger::ShowWindow( BOOL show )
{	
	_XWindow_SystemMsgDefault* SystemMsgWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);	

	if( !show )
	{	
		m_listboxGroupList->DeleteAllItem();
		if( SystemMsgWindow )
			SystemMsgWindow->SetDockingMode(FALSE);
	} 
	else
	{
		if( m_bDockingMode )
		{
			if( SystemMsgWindow )
				SystemMsgWindow->SetDockingMode(TRUE);

			SetEnableTopWindow(FALSE);	
			
			_XWindow_MainMenu* pMainmenuwindow = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);	
			g_MainWindowManager.SetTopWindow(pMainmenuwindow);			
		}
		else
		{
			if( SystemMsgWindow )
				SystemMsgWindow->SetDockingMode(FALSE);

			SetEnableTopWindow(TRUE);				
		}

#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER //Author : 양희왕
		_XWindow_RecallItem* pRecallItemWindow = (_XWindow_RecallItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_RECALLITEM_WINDOW);
		m_btnMessengerRecallSystem->EnableWindow(FALSE);

		if(pRecallItemWindow->SearchSlotItem( _XGI_SC_POTION_RECALL )) //소환령 아이템 second type
			m_btnMessengerRecallSystem->EnableWindow(TRUE);
#endif

		SetListItem();
	}

	_XWindow::ShowWindow( show );	
}

void _XWindow_Messenger::DestroyWindow( void )
{	
	_XWindow::DestroyWindow();		
	SAFE_DELETE(m_DragingDisplayBox);
	
}

void _XWindow_Messenger::Draw( _XGUIObject*& pfocusobject )
{			
	
	_XWindow::Draw(pfocusobject);	
	
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
	
	m_listboxGroupList->Draw(pfocusobject);
	
	POINT listwinpos;
	SIZE listwinsize;
	
	listwinpos = m_listboxGroupList->GetWindowPos();
	listwinsize = m_listboxGroupList->GetWindowSize();
	
	_XDrawRectAngle( listwinpos.x, listwinpos.y+1,
		listwinpos.x+listwinsize.cx-13, listwinpos.y+listwinsize.cy+1, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0) );
	
	if(m_bDrawDragingDisplayBox)
	{
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
		if(!m_bSelectedGroup)
		{
			if( CheckMousePosition() )
			{
				if( m_IsDragingCharacterName[0] != 0 )
				{
					m_DragingDisplayBox->SetToolTipString(scrnpos->x, scrnpos->z, m_IsDragingText, _XSC_DEFAULT, TRUE);
					m_DragingDisplayBox->Draw();
				}
			}
		}			
	}

	if(m_CurrentUserStatus != -1)
	{
		g_XBaseFont->Flush();
		g_XBaseFont->SetColor( _XSC_DEFAULT);
		g_XBaseFont->SetClippingArea( m_WindowPosition.x + 14, m_WindowPosition.y + 27, m_WindowPosition.x + 88, m_WindowPosition.y + 39 );
		g_XBaseFont->Puts( m_WindowPosition.x + 14, m_WindowPosition.y + 27, g_UserStatus[ m_CurrentUserStatus ] );
		g_XBaseFont->Flush();
		g_XBaseFont->DisableClippingArea();
	}
}

#define _XDEF_MESSENGERRESIZETRACKSIZE 4
BOOL _XWindow_Messenger::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	if( !m_bDblClick )
	{
		if( !m_ObjectList.listEmpty() )
		{
			m_ObjectList.resetList();
			_XGUIObject* currentObject = NULL;		
			do
			{
				currentObject = (_XGUIObject*)m_ObjectList.currentItem();
				
				if(currentObject) currentObject->Process( pfocusobject );
				
				m_ObjectList.nextItem();
			}while( !m_ObjectList.atHeadOfList() );
		}
	}
	
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if( !m_bDragingDisplayBox ) 
	{
		ProcessResizeAndDrag( pfocusobject );
	}
	
	if(mousestate)
	{
		if(pfocusobject)
		{	
			if(mousestate->bButton[0])//마우스 왼쪽 클릭시 
			{
				if( m_listboxGroupList->CheckMousePosition() )
				{					
					m_bClickLeftButton = TRUE; 	
				}						
			}
		}
	}

	if( m_bClickLeftButton )
	{	
		// 리스트 박스에서 마우스 왼쪽 클릭하고 버튼을 땠을 때
		if(!mousestate->bButton[0] )
		{		
			if(m_bDragingDisplayBox)
			{
				if( m_DragStartPosX != scrnpos->x || m_DragStartPosY != scrnpos->z )
				{
					ProcessMoveListbox();
				}			
				m_bDragingDisplayBox = FALSE;
				m_bDrawDragingDisplayBox = FALSE;
			}
			m_bClickLeftButton = FALSE;			
			m_bDblClick = FALSE;
		}

		// 리스트 박스에서 마우스 왼쪽 클릭하고 있을 때 : Draging
		if( mousestate->bButton[0] ) 
		{
			if( !m_bDblClick )
			{
				if(!CheckGroupListbox()) // 선택한 리스트가 그룹이 아니면  Draging한다 
				{
					if( !m_bDragingDisplayBox )
					{
						ProcessSelectedListboxLeftButton();	
						m_DragStartPosX = scrnpos->x;	
						m_DragStartPosY = scrnpos->z;	
						m_bDragingDisplayBox = TRUE;
					}				
				}

				if( m_bDragingDisplayBox )
				{
					if( m_DragStartPosX != scrnpos->x || m_DragStartPosY != scrnpos->z )
					{
						m_bDrawDragingDisplayBox = TRUE;
						ProcessSelectMovingGroup();
					}
				}
			}
		}
		
	}		


	// 리스트 박스에서 마우스 오른쪽 클릭시 : 클릭시 위치 저장 ->팝업시 비교
	if(mousestate->bButton[1])
	{
		m_ClickRightStartPosX = scrnpos->x;	// 마우스 오른쪽 클릭시  다른 쪽에서 클릭해서 
		m_ClickRightStartPosY = scrnpos->z;	//움직여서 왔을 때는 툴팁이 뜨지 않게 하게 위한 변수
		m_bClickRightButton = TRUE;
	}
	else
	{
		if( m_bClickRightButton )
		{
			if( m_ClickRightStartPosX == scrnpos->x && m_ClickRightStartPosY == scrnpos->z )
			{
				if( CheckMousePosition() )
				{
					ProcessSelectedListboxRightButton();
					m_bClickRightButton = FALSE;
				}
			}
		}
	}
	
	// 메신저 창에서 마우스 왼쪽 클릭시 
	if( mousestate->bButton[0] )  
	{
		_XWindow_MessengerTooltip* pMessengerTooltip_Window 
			= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MESSENGERTOOLTIPWINDOW );
		
		if(pMessengerTooltip_Window->GetShowStatus())
		{
			if( !pMessengerTooltip_Window->CheckMousePosition() )
			{
				if( pMessengerTooltip_Window->m_MessengerTooltipMode == _XMESSENGERTOOLTIP_CHARACTERSTATE )
				{
					if(!m_btnChangeStatus->CheckMousePosition())
					{
						pMessengerTooltip_Window->ShowWindow(FALSE);
						pMessengerTooltip_Window->SetMode( _XMESSENGERTOOLTIP_NONE );
					}					
				}	
				else
				{					
					pMessengerTooltip_Window->ShowWindow(FALSE);
					pMessengerTooltip_Window->SetMode( _XMESSENGERTOOLTIP_NONE );
				}
			}
		}
	}

	return TRUE;
}


BOOL _XWindow_Messenger::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	m_listboxGroupList->ProcessWheelMessage( zDelta );	
	if( CheckMousePosition() )
		return TRUE;

	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}

void _XWindow_Messenger::MoveWindow( int X, int Y )
{			
	// Clipping from screen size	
	_XWindow::MoveWindow( X, Y );
	_XWindow_SystemMsgDefault* systemWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);	

#ifndef _XDEF_RECALLSYSTEM_070604_MAGVIPER //Author : 양희왕 //breif : 디파인 되어있으면 들어오지 않는다
	if( m_bDockingMode )
	{
		if( systemWindow )
			systemWindow->SetDockingMode(TRUE);
	}
	else
	{
		if( systemWindow )
			systemWindow->SetDockingMode(FALSE);
	}
#endif

	m_listboxGroupList->MoveWindow( m_WindowPosition.x + 8 , m_WindowPosition.y + 48 );
}

void _XWindow_Messenger::ResizeWindow( int left, int top, int right, int bottom )
{
	if( right-left < _XDEF_MESSENGERWINDOWMINSIZE_WIDTH && bottom-top < _XDEF_MESSENGERWINDOWMINSIZE_HEIGHT ) return;
	if( right-left > _XDEF_MESSENGERWINDOWMAXSIZE_WIDTH && bottom-top > _XDEF_MESSENGERWINDOWMAXSIZE_HEIGHT ) return;
	
	if( right-left > _XDEF_MESSENGERWINDOWMAXSIZE_WIDTH )
	{
		if( left == m_WindowPosition.x )
		{
			right = m_WindowPosition.x + _XDEF_MESSENGERWINDOWMAXSIZE_WIDTH;
		}
		else
		{
			left = m_WindowPosition.x + m_WindowSize.cx - _XDEF_MESSENGERWINDOWMAXSIZE_WIDTH;
		}
	}
	else if( right-left < _XDEF_MESSENGERWINDOWMINSIZE_WIDTH )
	{
		if( left == m_WindowPosition.x )
		{
			right = m_WindowPosition.x + _XDEF_MESSENGERWINDOWMINSIZE_WIDTH;
		}
		else
		{
			left = m_WindowPosition.x + m_WindowSize.cx - _XDEF_MESSENGERWINDOWMINSIZE_WIDTH;
		}
	}
	
	if( bottom-top > _XDEF_MESSENGERWINDOWMAXSIZE_HEIGHT )
	{
		if( top == m_WindowPosition.y )
		{
			bottom = m_WindowPosition.y + _XDEF_MESSENGERWINDOWMAXSIZE_HEIGHT;
		}
		else
		{
			top = m_WindowPosition.y + m_WindowSize.cy - _XDEF_MESSENGERWINDOWMAXSIZE_HEIGHT;
		}
	}
#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER //Author : 양희왕
	else if( bottom-top < 160 )
	{
		if( top == m_WindowPosition.y )
		{
			bottom = m_WindowPosition.y + 160;
		}
		else
		{
			top = m_WindowPosition.y + m_WindowSize.cy - 160;
		}
	}
#else
	else if( bottom-top < 140 )
	{
		if( top == m_WindowPosition.y )
		{
			bottom = m_WindowPosition.y + 140;
		}
		else
		{
			top = m_WindowPosition.y + m_WindowSize.cy - 140;
		}
	}
#endif


	m_WindowPosition.x = left;
	m_WindowPosition.y = top;
	
	m_WindowSize.cx = right-left;
	m_WindowSize.cy = bottom-top;
	
	m_solidborderstatic.MoveWindow( left, top, right, bottom-1 );
	
	if( m_bDockingMode )
	{
		m_MaxViewCount = (( m_WindowSize.cy - 123) / 16);
		m_listboxGroupList->MoveWindow( m_WindowPosition.x + 8 , m_WindowPosition.y + 48);
		m_listboxGroupList->SetWindowSize( 168,  m_WindowSize.cy - 123 );	
		m_listboxGroupList->SetMaxViewLineCount( m_MaxViewCount );

		m_imageMessengerTitleBar->MoveWindow(m_WindowPosition.x, m_WindowPosition.y);
		m_imageMessengerStateBorder->MoveWindow(m_WindowPosition.x, m_WindowPosition.y+18);
		m_imageMessengerLeftSideBorder->MoveWindow(m_WindowPosition.x, m_WindowPosition.y+50);
		m_imageMessengerRightSideBorder->MoveWindow(m_WindowPosition.x+163, m_WindowPosition.y+50);

		m_btnMessengerClose->MoveWindow(m_WindowPosition.x + 166 , m_WindowPosition.y + 2);
		m_btnMessengerHelp->MoveWindow(m_WindowPosition.x + 153 , m_WindowPosition.y + 2);
		m_btnMessengerLogIn->MoveWindow(m_WindowPosition.x + 107 , m_WindowPosition.y + 21);
		m_btnMessengerLogOut->MoveWindow(m_WindowPosition.x + 107 , m_WindowPosition.y + 21);
		m_btnChangeStatus->MoveWindow(m_WindowPosition.x + 90 , m_WindowPosition.y + 27);

		float scalerate = (float)( m_WindowSize.cy - 123 )/ 158.0f;
		m_imageMessengerLeftSideBorder->SetScale( 1, scalerate);	
		m_imageMessengerRightSideBorder->SetScale( 1, scalerate);

#ifndef _XDEF_RECALLSYSTEM_070604_MAGVIPER //Author : 양희왕 //breif : 디파인되어있으면 들어오지 않는다
		m_imageMessengerDownBorder->MoveWindow(m_WindowPosition.x, m_WindowPosition.y+m_WindowSize.cy-73);		
		m_imageMessengerDockingBorder->MoveWindow(m_WindowPosition.x, m_WindowPosition.y+m_WindowSize.cy-21);
		
		m_imageMessengerDockingBorder->ShowWindow(TRUE);
		m_imageMessengerDownEndLine->ShowWindow(FALSE);

		
		m_btnMessengerAddFriend->MoveWindow(m_WindowPosition.x+10, m_WindowPosition.y+m_WindowSize.cy-69);
		m_btnMessengerAddFriendGroup->MoveWindow(m_WindowPosition.x+70, m_WindowPosition.y+m_WindowSize.cy-69);
#endif
				
		_XVScrollBar* pScrollBar = m_listboxGroupList->GetScrollBarObject();
		pScrollBar->SetWindowHeight( m_WindowSize.cy - 120 );
		SetEnableTopWindow(FALSE);
		
		_XWindow_MainMenu* pMainmenuwindow = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);	
		g_MainWindowManager.SetTopWindow(pMainmenuwindow);		
	}
	else
	{
		m_MaxViewCount = (( m_WindowSize.cy - 105) / 16);
		m_listboxGroupList->MoveWindow( m_WindowPosition.x + 8 , m_WindowPosition.y + 48);
		m_listboxGroupList->SetWindowSize( 168,  m_WindowSize.cy - 105 );	
		m_listboxGroupList->SetMaxViewLineCount( m_MaxViewCount );
		
		m_imageMessengerTitleBar->MoveWindow(m_WindowPosition.x, m_WindowPosition.y);
		m_imageMessengerStateBorder->MoveWindow(m_WindowPosition.x, m_WindowPosition.y+18);
		m_imageMessengerLeftSideBorder->MoveWindow(m_WindowPosition.x, m_WindowPosition.y+50);
		m_imageMessengerRightSideBorder->MoveWindow(m_WindowPosition.x+163, m_WindowPosition.y+50);

		m_btnMessengerClose->MoveWindow(m_WindowPosition.x + 166 , m_WindowPosition.y + 2);
		m_btnMessengerHelp->MoveWindow(m_WindowPosition.x + 153 , m_WindowPosition.y + 2);
		m_btnMessengerLogIn->MoveWindow(m_WindowPosition.x + 107 , m_WindowPosition.y + 21);
		m_btnMessengerLogOut->MoveWindow(m_WindowPosition.x + 107 , m_WindowPosition.y + 21);
		m_btnChangeStatus->MoveWindow(m_WindowPosition.x + 90 , m_WindowPosition.y + 27);

		float scalerate = (float)( m_WindowSize.cy - 105 )/ 158.0f;
		m_imageMessengerLeftSideBorder->SetScale( 1, scalerate);	
		m_imageMessengerRightSideBorder->SetScale( 1, scalerate);
		
#ifndef _XDEF_RECALLSYSTEM_070604_MAGVIPER //Author : 양희왕 //breif : 디파인되어있으면 들어오지 않는다
		m_imageMessengerDownBorder->MoveWindow(m_WindowPosition.x, m_WindowPosition.y+m_WindowSize.cy-55);
		m_imageMessengerDownEndLine->MoveWindow(m_WindowPosition.x, m_WindowPosition.y+m_WindowSize.cy-3);

		m_imageMessengerDockingBorder->ShowWindow(FALSE);
		m_imageMessengerDownEndLine->ShowWindow(TRUE);

		m_btnMessengerAddFriend->MoveWindow(m_WindowPosition.x+10, m_WindowPosition.y+m_WindowSize.cy-51);
		m_btnMessengerAddFriendGroup->MoveWindow(m_WindowPosition.x+70, m_WindowPosition.y+m_WindowSize.cy-51);
#endif
		
		_XVScrollBar* pScrollBar = m_listboxGroupList->GetScrollBarObject();
		pScrollBar->SetWindowHeight( m_WindowSize.cy - 102 );
		
		SetEnableTopWindow(TRUE);
		
		_XWindow_MainMenu* pMainmenuwindow = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);	
		g_MainWindowManager.SetTopWindow(pMainmenuwindow);		
	}
}

void _XWindow_Messenger::SetListItem()
{
	TCHAR	groupnamebuffer[256];
	TCHAR	friendnamebuffer[256];
	TCHAR	friendnumberbuffer[64];
	TCHAR	friendstatusbuffer[128];
	
	
	memset(groupnamebuffer, 0, sizeof(groupnamebuffer));
	memset(friendnamebuffer, 0, sizeof(friendnamebuffer));
	memset(friendnumberbuffer, 0, sizeof(friendnumberbuffer));
	memset(friendstatusbuffer, 0, sizeof(friendstatusbuffer));
	
	m_listboxGroupList->DeleteAllItem();
	
	m_listboxGroupList->MoveWindow( m_WindowPosition.x + 8 , m_WindowPosition.y + 48);
	
	
	if( m_WindowPosition.x+m_WindowSize.cx > ( gnWidth - 100) )
	{
		if( m_WindowPosition.y+m_WindowSize.cy > ( gnHeight - 90 ) )
		{
			m_listboxGroupList->SetWindowSize( 168,  m_WindowSize.cy - 123 );
			SetDockingMode(TRUE);
		}
		else
		{
			m_listboxGroupList->SetWindowSize( 168,  m_WindowSize.cy - 105 );
			SetDockingMode(FALSE);
		}
	}
	else
	{
		m_listboxGroupList->SetWindowSize( 168,  m_WindowSize.cy - 105 );
		SetDockingMode(FALSE);
	}
	
	int count = 0 ;	
	if( !m_MessengerGroupList.listEmpty() )
	{
		m_MessengerGroupList.resetList();
		_XMessengerGroup* currentgroup = NULL;
		do 
		{
			currentgroup = (_XMessengerGroup *)m_MessengerGroupList.currentItem();
			if(currentgroup)
			{
				int friendnumber = currentgroup->m_FriendList.getitemcount();
				sprintf( friendnumberbuffer, " (%d)",friendnumber);				

				sprintf( groupnamebuffer, currentgroup->m_GroupName ); 
				strcat( groupnamebuffer, friendnumberbuffer );
				
				int messengerresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
				RECT rect = { 45, 186, 56, 197};
				if( !currentgroup->m_bSelected )
				{
					rect.left = 67;
					rect.right = 78;
				}
												
				m_listboxGroupList->InsertListItem(_T(""), 0, count+1);		
				
				m_listboxGroupList->SetIconArchive(count, &g_MainInterfaceTextureArchive);
				
				m_listboxGroupList->SetItemText(count , 0, groupnamebuffer);
				m_listboxGroupList->SetItemAttrib(count, 0, D3DCOLOR_ARGB( 255, 219, 196, 136 ));
				m_listboxGroupList->SetItemAttribHighlight(count, 0, D3DCOLOR_ARGB(255, 255, 255, 0));
				
				m_listboxGroupList->SetItemText(count , 1, _T(""));
				m_listboxGroupList->SetItemAttrib(count, 1, D3DCOLOR_ARGB( 255, 219, 196, 136 ));
				m_listboxGroupList->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 0));
				
				m_listboxGroupList->SetItemAttrib(count, 2, messengerresourceindex1, rect);
				m_listboxGroupList->SetItemAttrib(count, 3, -1 , rect);
				
				currentgroup->m_GroupNumber = count; // 리스트 박스의 인텍스 값과 동기화
				count++;

				if( currentgroup->m_bSelected )
				{
					if(!currentgroup->m_FriendList.listEmpty())
					{
						currentgroup->m_FriendList.resetList();
						_XMessengerFriend* currentfriend = NULL;
						do 
						{
							currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
							if(currentfriend)
							{
								if(currentfriend->m_FriendName[0] != 0 )
								{
									sprintf( friendnamebuffer, currentfriend->m_FriendName ); 
									sprintf( friendstatusbuffer, "(%s)", g_UserStatus[currentfriend->m_FriendState]);				
									
									strcat( friendnamebuffer, friendstatusbuffer );
									
									RECT friendrect = { 0, 0, 0, 0 };
									switch( currentfriend->m_FriendState ) {
									case 0:	//로그 오프
										{
											friendrect.left = 35;
											friendrect.top	= 215;
											friendrect.right = 46;
											friendrect.bottom = 226;
										}
										break;
									case 1: // 온라인
										{
											friendrect.left = 46;
											friendrect.top	= 204;
											friendrect.right = 57;
											friendrect.bottom = 215;
										}
										break;
									case 2: // 전투 중 
										{
											friendrect.left = 35;
											friendrect.top	= 204;
											friendrect.right = 46;
											friendrect.bottom = 215;
										}
										break;
									case 3: // 부재 중 
										{
											friendrect.left = 46;
											friendrect.top	= 215;
											friendrect.right = 57;
											friendrect.bottom = 226;
										}
										break;
									case 4: // 서버이동 중
										{
											friendrect.left = 46;
											friendrect.top	= 215;
											friendrect.right = 57;
											friendrect.bottom = 226;
										}
										break;
									}

									m_listboxGroupList->InsertListItem(_T(""), 0, count+1);
									
									m_listboxGroupList->SetIconArchive(count, &g_MainInterfaceTextureArchive);

									m_listboxGroupList->SetItemText(count , 0,  _T(" "));
									m_listboxGroupList->SetItemAttrib(count, 0, D3DCOLOR_ARGB(255, 189, 188, 175));
									m_listboxGroupList->SetItemAttribHighlight(count, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
									
									m_listboxGroupList->SetItemText(count , 1, friendnamebuffer);
									m_listboxGroupList->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 189, 188, 175));
									m_listboxGroupList->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
									
									m_listboxGroupList->SetItemAttrib(count, 2, -1 , friendrect);
									m_listboxGroupList->SetItemAttrib(count, 3, messengerresourceindex1, friendrect);

									currentfriend->m_FriendNumber = count;	
									count++;						
								}								
							}
							currentgroup->m_FriendList.nextItem();							
						} while(!currentgroup->m_FriendList.atHeadOfList());						
					}					
				}
			}
			m_MessengerGroupList.nextItem();
		} while(!m_MessengerGroupList.atHeadOfList());
	}
}

int _XWindow_Messenger::SearchListNumber()
{
	if(!m_listboxGroupList->CheckMousePosition()) return -1;
	
	int selectedlistnumber	= -1;
	ScrnPos* scrnpos	= gpInput->GetMouseVirScreenPos();
	int count			= 0;
	int rowpoint		= m_WindowPosition.y + 48;	
	
	_XWindow_MessengerTooltip* pMessengerTooltip_Window 
		= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MESSENGERTOOLTIPWINDOW );
	
	// 툴팁이 떠 있을 때는 Skip
	if( pMessengerTooltip_Window->GetShowStatus() )
	{
		if(pMessengerTooltip_Window->CheckMousePosition())
		{
			return -1;			
		}
	}

	if( selectedlistnumber == -1 )
	{
		
		for( int row = 0; row < m_listboxGroupList->GetItemCount() ; row++ )
		{			
			if( (scrnpos->z > rowpoint ) && (scrnpos->z < rowpoint+16) ) 
			{
				if( (scrnpos->x > m_WindowPosition.x + 8  ) && (scrnpos->x < m_WindowPosition.x + 156) ) 
				{
					selectedlistnumber = count;
					break;
				}
			}
			rowpoint += 16;
			++count;
		}
	}
	
	return selectedlistnumber;

}

void _XWindow_Messenger::ProcessSelectedListboxRightButton()
{
     //Author : 양희왕 //breif : 스크롤 바 위에 선 작동하지말자
    _XVScrollBar* pScrollBar = m_listboxGroupList->GetScrollBarObject();
    if(pScrollBar->CheckMousePosition())
        return;
	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	int selectedlistnumber = SearchListNumber() + m_listboxGroupList->GetScrollPos();
	//int selectedlistnumber = m_SelectedItemIndex;
	if( selectedlistnumber != -1 )
	{
		m_listboxGroupList->SelectItem( selectedlistnumber );
		
		_XWindow_MessengerTooltip* pMessengerTooltip_Window 
			= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MESSENGERTOOLTIPWINDOW );
		
		
		// 선택된 그룹리스트 체크하기 
		if( !m_MessengerGroupList.listEmpty() )
		{
			m_MessengerGroupList.resetList();
			_XMessengerGroup* currentgroup = NULL;
			do 
			{
				currentgroup = (_XMessengerGroup *)m_MessengerGroupList.currentItem();
				if(currentgroup)
				{
					if(currentgroup->m_GroupNumber == selectedlistnumber )
					{
						pMessengerTooltip_Window->SetMode(_XMESSENGERTOOLTIP_GROUPLIST);

						POINT messengerwinpos, tooltipwinpos;
						SIZE messengerwinsize, tooltipwinsize;
						messengerwinpos = GetWindowPos();
						messengerwinsize = GetWindowSize();
						tooltipwinpos = pMessengerTooltip_Window->GetWindowPos();
						tooltipwinsize = pMessengerTooltip_Window->GetWindowSize();

						// 메신저 창의 중심을 원점으로 1사분면
						if( scrnpos->x > messengerwinpos.x + messengerwinsize.cx/2 
							&& scrnpos->z < messengerwinpos.y + messengerwinsize.cy/2  )
						{
							if( scrnpos->x + tooltipwinsize.cx < gnWidth ) // 1, 4
							{
								if( scrnpos->z - tooltipwinsize.cy > 0 ) // 1
									pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z - tooltipwinsize.cy ); // 1
								else // 4
									pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z ); // 4								
							}
							else	// 2, 3
							{
								if( scrnpos->z - tooltipwinsize.cy > 0 ) //2
									pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z - tooltipwinsize.cy ); // 2
								else //3
									pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z ); // 3
							}
						}// 메신저 창의 중심을 원점으로 2사분면
						else if( scrnpos->x < messengerwinpos.x + messengerwinsize.cx/2 
							&& scrnpos->z < messengerwinpos.y + messengerwinsize.cy/2  )
						{							
							if( scrnpos->x - tooltipwinsize.cx > 0 ) // 2, 3
							{
								if( scrnpos->z - tooltipwinsize.cy > 0 ) // 2
									pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z - tooltipwinsize.cy ); // 2
								else // 3
									pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z ); // 3
								
							}
							else	// 1, 4
							{
								if( scrnpos->z - tooltipwinsize.cy > 0 ) //1
									pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z - tooltipwinsize.cy ); // 1
								else //4
									pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z ); // 4
							}
						}// 메신저 창의 중심을 원점으로 3사분면
						else if( scrnpos->x < messengerwinpos.x + messengerwinsize.cx/2 
							&& scrnpos->z > messengerwinpos.y + messengerwinsize.cy/2  )
						{							
							if( scrnpos->x - tooltipwinsize.cx > 0 ) // 2, 3
							{
								if( scrnpos->z + tooltipwinsize.cy < gnHeight ) // 3
									pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z ); // 3
								else // 2
									pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z - tooltipwinsize.cy ); // 2
							}
							else	// 1, 4
							{
								if( scrnpos->z + tooltipwinsize.cy < gnHeight ) //4
									pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z ); // 4
								else //1
									pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z - tooltipwinsize.cy ); // 1								
							}							
						}// 메신저 창의 중심을 원점으로 4사분면
						else
						{						
							if( scrnpos->x + tooltipwinsize.cx < gnWidth ) // 1, 4
							{
								if( scrnpos->z + tooltipwinsize.cy < gnHeight ) // 4
									pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z ); // 4
								else // 1
									pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z - tooltipwinsize.cy ); // 1
							}
							else	// 2, 3
							{
								if( scrnpos->z + tooltipwinsize.cy < gnHeight ) //3
									pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z ); // 3
								else //2
								pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z - tooltipwinsize.cy ); // 2
							}			
						}

						pMessengerTooltip_Window->ShowWindow(TRUE);			 
						g_MainWindowManager.SetTopWindow(pMessengerTooltip_Window);		
						
						memset( m_SelectedGroupname, 0, sizeof(TCHAR)*15 ); 
						memset( m_SelectedCharactername, 0, sizeof(TCHAR)*13 );  // 저장된 캐릭터 이름은 지운다.
						strcpy( m_SelectedGroupname, currentgroup->m_GroupName ); // 선택된 그룹이름 저장 
						
						break;
					}
					else
					{
						if(!currentgroup->m_FriendList.listEmpty())
						{
							currentgroup->m_FriendList.resetList();
							_XMessengerFriend* currentfriend = NULL;
							do 
							{
								currentfriend = (_XMessengerFriend *)currentgroup->m_FriendList.currentItem();
								if(currentfriend)
								{
									if(currentfriend->m_FriendNumber == selectedlistnumber )
									{
										pMessengerTooltip_Window->SetMode(_XMESSENGERTOOLTIP_CHARACTERLIST);

										POINT messengerwinpos, tooltipwinpos;
										SIZE messengerwinsize, tooltipwinsize;
										messengerwinpos = GetWindowPos();
										messengerwinsize = GetWindowSize();
										tooltipwinpos = pMessengerTooltip_Window->GetWindowPos();
										tooltipwinsize = pMessengerTooltip_Window->GetWindowSize();
										
										// 메신저 창의 중심을 원점으로 1사분면
										if( scrnpos->x > messengerwinpos.x + messengerwinsize.cx/2 
											&& scrnpos->z < messengerwinpos.y + messengerwinsize.cy/2  )
										{
											if( scrnpos->x + tooltipwinsize.cx < gnWidth ) // 1, 4
											{
												if( scrnpos->z - tooltipwinsize.cy > 0 ) // 1
													pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z - tooltipwinsize.cy ); // 1
												else // 4
													pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z ); // 4								
											}
											else	// 2, 3
											{
												if( scrnpos->z - tooltipwinsize.cy > 0 ) //2
													pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z - tooltipwinsize.cy ); // 2
												else //3
													pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z ); // 3
											}
										}// 메신저 창의 중심을 원점으로 2사분면
										else if( scrnpos->x < messengerwinpos.x + messengerwinsize.cx/2 
											&& scrnpos->z < messengerwinpos.y + messengerwinsize.cy/2  )
										{							
											if( scrnpos->x - tooltipwinsize.cx > 0 ) // 2, 3
											{
												if( scrnpos->z - tooltipwinsize.cy > 0 ) // 2
													pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z - tooltipwinsize.cy ); // 2
												else // 3
													pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z ); // 3
												
											}
											else	// 1, 4
											{
												if( scrnpos->z - tooltipwinsize.cy > 0 ) //1
													pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z - tooltipwinsize.cy ); // 1
												else //4
													pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z ); // 4
											}
										}// 메신저 창의 중심을 원점으로 3사분면
										else if( scrnpos->x < messengerwinpos.x + messengerwinsize.cx/2 
											&& scrnpos->z > messengerwinpos.y + messengerwinsize.cy/2  )
										{							
											if( scrnpos->x - tooltipwinsize.cx > 0 ) // 2, 3
											{
												if( scrnpos->z + tooltipwinsize.cy < gnHeight ) // 3
													pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z ); // 3
												else // 2
													pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z - tooltipwinsize.cy ); // 2
											}
											else	// 1, 4
											{
												if( scrnpos->z + tooltipwinsize.cy < gnHeight ) //4
													pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z ); // 4
												else //1
													pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z - tooltipwinsize.cy ); // 1								
											}							
										}// 메신저 창의 중심을 원점으로 4사분면
										else
										{						
											if( scrnpos->x + tooltipwinsize.cx < gnWidth ) // 1, 4
											{
												if( scrnpos->z + tooltipwinsize.cy < gnHeight ) // 4
													pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z ); // 4
												else // 1
													pMessengerTooltip_Window->MoveWindow( scrnpos->x, scrnpos->z - tooltipwinsize.cy ); // 1
											}
											else	// 2, 3
											{
												if( scrnpos->z + tooltipwinsize.cy < gnHeight ) //3
													pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z ); // 3
												else //2
													pMessengerTooltip_Window->MoveWindow( scrnpos->x - tooltipwinsize.cx, scrnpos->z - tooltipwinsize.cy ); // 2
											}			
										}

										pMessengerTooltip_Window->ShowWindow(TRUE);			 
										g_MainWindowManager.SetTopWindow(pMessengerTooltip_Window);		
										
										memset( m_SelectedCharactername, 0, sizeof(TCHAR)*13 );
										memset( m_SelectedGroupname, 0, sizeof(TCHAR)*15 ); // 캐릭터가 선택되면 그룹이름은 지운다.
										strcpy( m_SelectedCharactername, currentfriend->m_FriendName );
										
										break;
									}
								}
								currentgroup->m_FriendList.nextItem();
							}while (!currentgroup->m_FriendList.atHeadOfList());
						}						
					}
				}
				m_MessengerGroupList.nextItem();
			}while (!m_MessengerGroupList.atHeadOfList());	
		}
		
		//m_bClickRightButton = FALSE;
	}
}

void _XWindow_Messenger::ProcessSelectedListboxLeftButton()
{
	//Author : 양희왕 //breif : 다른사람 삭제 되는 문제, 툴팁창이 켜있으면 선택된 상태를 바꾸면 안된다. Date : 08/03/13
	_XWindow_MessengerTooltip* pMessengerTooltip_Window 
		= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MESSENGERTOOLTIPWINDOW );

	if( pMessengerTooltip_Window->GetShowStatus() )
	{
		if(pMessengerTooltip_Window->CheckMousePosition())
		{
			return;			
		}
	}

    //Author : 양희왕 //breif : 스크롤 바 위에 선 작동하지말자
    //_XVScrollBar* pScrollBar = m_listboxGroupList->GetScrollBarObject();
    //if(pScrollBar->CheckMousePosition())
    //    return;

    int nScrollPos = m_listboxGroupList->GetScrollPos();
	int selectedlistnumber = SearchListNumber();// + m_listboxGroupList->GetScrollPos();
	bool usesetlist = FALSE;
	
	if( selectedlistnumber != -1 )
	{
        selectedlistnumber += nScrollPos;

		// 선택된 그룹리스트 체크하기 
		if( !m_MessengerGroupList.listEmpty() )
		{
			m_MessengerGroupList.resetList();
			_XMessengerGroup* currentgroup = NULL;
			do 
			{
				currentgroup = (_XMessengerGroup *)m_MessengerGroupList.currentItem();
				if(currentgroup)
				{
					if(currentgroup->m_GroupNumber == selectedlistnumber )
					{
						m_bSelectedGroup = TRUE; // 그룹이 선택되었다는 표시 
						
						if(currentgroup->m_FriendList.listEmpty())
						{
							currentgroup->m_bSelected = FALSE;
							
							memset( m_SelectedGroupname, 0, sizeof(TCHAR)*15 ); 
							memset( m_SelectedCharactername, 0, sizeof(TCHAR)*13 );  // 저장된 캐릭터 이름은 지운다.
							strcpy( m_SelectedGroupname, currentgroup->m_GroupName ); // 선택된 그룹이름 저장 
							
						}
						else
						{
							if(currentgroup->m_bSelected)
								currentgroup->m_bSelected = FALSE;
							else
								currentgroup->m_bSelected = TRUE;
							
							memset( m_SelectedGroupname, 0, sizeof(TCHAR)*15 ); 
							memset( m_SelectedCharactername, 0, sizeof(TCHAR)*13 );  // 저장된 캐릭터 이름은 지운다.
							strcpy( m_SelectedGroupname, currentgroup->m_GroupName ); // 선택된 그룹이름 저장 
							
							usesetlist = TRUE;
						}	
						
						break;
					}
					else if(currentgroup->m_bSelected)
					{
						if(!currentgroup->m_FriendList.listEmpty())
						{
							currentgroup->m_FriendList.resetList();
							_XMessengerFriend* currentfriend = NULL;
							do 
							{
								currentfriend = (_XMessengerFriend *)currentgroup->m_FriendList.currentItem();
								if(currentfriend)
								{
									if(currentfriend->m_FriendNumber == selectedlistnumber )
									{
										m_bSelectedGroup = FALSE;	// 친구가 선택되었다는 표시
										
										memset( m_SelectedGroupname, 0, sizeof(TCHAR)*15 ); // 캐릭터가 선택되면 그룹이름은 지운다.
										strcpy( m_SelectedCharactername, currentfriend->m_FriendName );
										
										memset( m_IsDragingCharacterName, 0, sizeof(TCHAR)*13 );
										strcpy( m_IsDragingCharacterName, currentfriend->m_FriendName ); // 드래그 할때 드래그 된 대상 
										
										memset( m_IsDragingGroupName, 0, sizeof(TCHAR)*15 );
										strcpy( m_IsDragingGroupName, currentgroup->m_GroupName ); // 드래그 할때 드래그 된 대상 
										
										memset( m_IsDragingText, 0, sizeof(TCHAR)*256 );
										strcpy( m_IsDragingText, m_listboxGroupList->GetItemText( selectedlistnumber, 1) );
										
										m_IsDragingCharacterNumber = selectedlistnumber;
										m_IsDragingGroupNumber = currentgroup->m_GroupNumber;											
										break;
									}
								}
								currentgroup->m_FriendList.nextItem();
							}while (!currentgroup->m_FriendList.atHeadOfList());
						}						
					}
				}
				m_MessengerGroupList.nextItem();
			}while (!m_MessengerGroupList.atHeadOfList());	
		}

		if(usesetlist) // 친구 명단을 가지고 있는 그룹이 선택되었을 때는 리스트를 갱신한다.
		//	SetListItem();
		
		m_listboxGroupList->SelectItem( selectedlistnumber );
	}
}

void _XWindow_Messenger::ProcessDblClick()
{	
	//Author : 양희왕 //breif : 다른사람 삭제 되는 문제, 툴팁창이 켜있으면 선택된 상태를 바꾸면 안된다. Date : 08/03/13
	_XWindow_MessengerTooltip* pMessengerTooltip_Window 
		= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MESSENGERTOOLTIPWINDOW );

	if( pMessengerTooltip_Window->GetShowStatus() )
	{
		if(pMessengerTooltip_Window->CheckMousePosition())
		{
			return;			
		}
	}

    //Author : 양희왕 //breif : 스크롤 바 위에 선 작동하지말자
    //_XVScrollBar* pScrollBar = m_listboxGroupList->GetScrollBarObject();
    //if(pScrollBar->CheckMousePosition())
    //    return;

	m_bDblClick = TRUE;
	// 더블클릭할때 그룹을 열고 닫는다.
    int nScrollPos = m_listboxGroupList->GetScrollPos();
	int selectedlistnumber = SearchListNumber();// + m_listboxGroupList->GetScrollPos();
	bool usesetlist = FALSE;

	if( selectedlistnumber != -1 )
	{
        selectedlistnumber += nScrollPos;
		// 선택된 그룹리스트 체크하기 
		if( !m_MessengerGroupList.listEmpty() )
		{
			m_MessengerGroupList.resetList();
			_XMessengerGroup* currentgroup = NULL;
			do 
			{
				currentgroup = (_XMessengerGroup *)m_MessengerGroupList.currentItem();
				if(currentgroup)
				{
					if(currentgroup->m_GroupNumber == selectedlistnumber )
					{
						m_bSelectedGroup = TRUE; // 그룹이 선택되었다는 표시 

						if(currentgroup->m_FriendList.listEmpty())
						{
							currentgroup->m_bSelected = FALSE;

							memset( m_SelectedGroupname, 0, sizeof(TCHAR)*15 ); 
							memset( m_SelectedCharactername, 0, sizeof(TCHAR)*13 );  // 저장된 캐릭터 이름은 지운다.
							strcpy( m_SelectedGroupname, currentgroup->m_GroupName ); // 선택된 그룹이름 저장 
																					
						}
						else
						{
							if(currentgroup->m_bSelected)
								currentgroup->m_bSelected = FALSE;
							else
								currentgroup->m_bSelected = TRUE;
							
							memset( m_SelectedGroupname, 0, sizeof(TCHAR)*15 ); 
							memset( m_SelectedCharactername, 0, sizeof(TCHAR)*13 );  // 저장된 캐릭터 이름은 지운다.
							strcpy( m_SelectedGroupname, currentgroup->m_GroupName ); // 선택된 그룹이름 저장 
							
							usesetlist = TRUE;
						}	
						
						break;
					}
					else if(currentgroup->m_bSelected)
					{
						if(!currentgroup->m_FriendList.listEmpty())
						{
							currentgroup->m_FriendList.resetList();
							_XMessengerFriend* currentfriend = NULL;
							do 
							{
								currentfriend = (_XMessengerFriend *)currentgroup->m_FriendList.currentItem();
								if(currentfriend)
								{
									if(currentfriend->m_FriendNumber == selectedlistnumber )
									{
										m_bSelectedGroup = FALSE;	// 친구가 선택되었다는 표시

										memset( m_SelectedGroupname, 0, sizeof(TCHAR)*15 ); // 캐릭터가 선택되면 그룹이름은 지운다.
										strcpy( m_SelectedCharactername, currentfriend->m_FriendName );

										memset( m_IsDragingCharacterName, 0, sizeof(TCHAR)*13 );
										strcpy( m_IsDragingCharacterName, currentfriend->m_FriendName ); // 드래그 할때 드래그 된 대상 
										
										memset( m_IsDragingGroupName, 0, sizeof(TCHAR)*15 );
										strcpy( m_IsDragingGroupName, currentgroup->m_GroupName ); // 드래그 할때 드래그 된 대상 

										memset( m_IsDragingText, 0, sizeof(TCHAR)*256 );
										strcpy( m_IsDragingText, m_listboxGroupList->GetItemText( selectedlistnumber, 1) );

										m_IsDragingCharacterNumber = selectedlistnumber;
										m_IsDragingGroupNumber = currentgroup->m_GroupNumber;											

										if( currentfriend->m_FriendState != 0 )//로그오프 상태가 아니면 
										{											
											bool bfindmessengerdialog = FALSE;
											for( int windowid = 60000; windowid < 60050; windowid++ )
											{
												_XWindow_MessengerDialog* pMessengerDialog_Window = (_XWindow_MessengerDialog*)g_MainWindowManager.FindWindow(windowid);
												if( pMessengerDialog_Window )
												{
													// 보낸 사람으로 윈도우를 찾는다 
													if(strcmpi(m_SelectedCharactername, pMessengerDialog_Window->m_ToCharactername) == 0)
													{
														bfindmessengerdialog = TRUE;
														
														if( pMessengerDialog_Window->m_MessengerDialogMode == _XMESSENGERDIALOGMODE_INSTANCE )
														{
															pMessengerDialog_Window->SetMode( _XMESSENGERDIALOGMODE_DIALOG );
															pMessengerDialog_Window->MoveWindow((gnWidth >> 1) - (_XDEF_MESSENGERDIALOGWINDOW_WIDTH >>1), 
																(gnHeight>> 1) - (_XDEF_MESSENGERDIALOGWINDOW_HEIGHT>>1) );
														}
														
														
														pMessengerDialog_Window->ShowWindow(TRUE);
														g_MainWindowManager.IsTopWindow( pMessengerDialog_Window );

														_XIMEKERNEL.ResetCombinationBuffer();
														pMessengerDialog_Window->m_IMEControlMessengerDialog.ClearBuffer();
														pMessengerDialog_Window->m_IMEControlMessengerDialog.SetFocus();													
														break;													
													}
												}
											}
											
											if( !bfindmessengerdialog )
											{
												_XWindow_MessengerDialog* pMessengerDialog_Window = _XWindow_MessengerDialog::CreateMessengerDialogBox( &g_MainWindowManager );
												if( pMessengerDialog_Window )
												{													
													strcpy( pMessengerDialog_Window->m_ToCharactername, m_SelectedCharactername );
													
													pMessengerDialog_Window->SetMode( _XMESSENGERDIALOGMODE_DIALOG );
													pMessengerDialog_Window->MoveWindow((gnWidth >> 1) - (_XDEF_MESSENGERDIALOGWINDOW_WIDTH >>1), 
														(gnHeight>> 1) - (_XDEF_MESSENGERDIALOGWINDOW_HEIGHT>>1) );
												
													pMessengerDialog_Window->ShowWindow(TRUE);
													g_MainWindowManager.IsTopWindow( pMessengerDialog_Window );
													
													_XIMEKERNEL.ResetCombinationBuffer();
													pMessengerDialog_Window->m_IMEControlMessengerDialog.ClearBuffer();
													pMessengerDialog_Window->m_IMEControlMessengerDialog.SetFocus();	
												}
											}
										}
										break;
									}
								}
								currentgroup->m_FriendList.nextItem();
							}while (!currentgroup->m_FriendList.atHeadOfList());
						}						
					}
				}
				m_MessengerGroupList.nextItem();
			}while (!m_MessengerGroupList.atHeadOfList());	
		}

		if(usesetlist) // 친구 명단을 가지고 있는 그룹이 선택되었을 때는 리스트를 갱신한다.
			SetListItem();
	
		if( m_SelectedGroupname[0] != 0 )
		{
			if( !m_MessengerGroupList.listEmpty() )
			{
				m_MessengerGroupList.resetList();
				_XMessengerGroup* currentgroup = NULL;
				do 
				{
					currentgroup = (_XMessengerGroup *)m_MessengerGroupList.currentItem();
					if(currentgroup)
					{
						if( strcmp( currentgroup->m_GroupName, m_SelectedGroupname) == 0 )
						{
							m_listboxGroupList->SelectItem( currentgroup->m_GroupNumber );
							
							break;
						}
					}
					m_MessengerGroupList.nextItem();
				}while (!m_MessengerGroupList.atHeadOfList());					
			}
		}
		else
		{
			m_listboxGroupList->SelectItem( selectedlistnumber );
		}
	}
}

void _XWindow_Messenger::ProcessResizeAndDrag( _XGUIObject*& pfocusobject )
{
	
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
	BOOL changedmousecursor = FALSE;

	
	if( mousestate && scrnpos )
	{			
		if( !pfocusobject )
		{
			if( CheckMousePosition() )
			{	
				if(!m_listboxGroupList->CheckMousePosition())
				{
					if( scrnpos->x < m_WindowPosition.x+_XDEF_MESSENGERRESIZETRACKSIZE )
					{	
						if( scrnpos->z < m_WindowPosition.y+_XDEF_MESSENGERRESIZETRACKSIZE )
						{
							if( mousestate->bButton[0] )
							{
								pfocusobject = (_XGUIObject*)this;
								//m_Resizing = TRUE;												
								m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
								m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
							}											
							
							//m_ResizeMode = _XWRESIZE_LEFTBOTTOM;
							//changedmousecursor = TRUE;
						}
						else if( scrnpos->z >= m_WindowPosition.y+m_WindowSize.cy-_XDEF_MESSENGERRESIZETRACKSIZE )
						{
							if( mousestate->bButton[0] )
							{
								pfocusobject = (_XGUIObject*)this;
								//m_Resizing = TRUE;												
								m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
								m_DragStartPos.y = m_WindowPosition.y+m_WindowSize.cy - scrnpos->z;
							}
													
							//m_ResizeMode = _XWRESIZE_LEFTBOTTOM;
							//changedmousecursor = TRUE;
						}
						else
						{
							if( mousestate->bButton[0] )
							{
								pfocusobject = (_XGUIObject*)this;
								//m_Resizing = TRUE;												
								m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
							}
													
							//m_ResizeMode = _XWRESIZE_LEFT;
							//changedmousecursor = TRUE;
						}

					}
					else if( scrnpos->x >= m_WindowPosition.x+m_WindowSize.cx-_XDEF_MESSENGERRESIZETRACKSIZE )
					{	
						if( scrnpos->z < m_WindowPosition.y+_XDEF_MESSENGERRESIZETRACKSIZE )
						{
							if( mousestate->bButton[0] )
							{
								pfocusobject = (_XGUIObject*)this;
								//m_Resizing = TRUE;												
								m_DragStartPos.x = m_WindowPosition.x+m_WindowSize.cx - scrnpos->x;
								m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
							}
													
							//m_ResizeMode = _XWRESIZE_RIGHTTOP;
							//changedmousecursor = TRUE;
						}
						else if( scrnpos->z >= m_WindowPosition.y+m_WindowSize.cy-_XDEF_MESSENGERRESIZETRACKSIZE )
						{
							if( mousestate->bButton[0] )
							{
								pfocusobject = (_XGUIObject*)this;
								//m_Resizing = TRUE;												
								m_DragStartPos.x = m_WindowPosition.x+m_WindowSize.cx - scrnpos->x;
								m_DragStartPos.y = m_WindowPosition.y+m_WindowSize.cy - scrnpos->z;
							}
							
							//m_ResizeMode = _XWRESIZE_RIGHTBOTTOM;
							//changedmousecursor = TRUE;
						}
						else
						{
							if( mousestate->bButton[0] )
							{
								pfocusobject = (_XGUIObject*)this;
								//m_Resizing = TRUE;												
								m_DragStartPos.x = m_WindowPosition.x+m_WindowSize.cx - scrnpos->x;							
							}
													
							//m_ResizeMode = _XWRESIZE_RIGHT;
							//changedmousecursor = TRUE;
						}
					}
					
					if( scrnpos->z < m_WindowPosition.y+_XDEF_MESSENGERRESIZETRACKSIZE
						&& scrnpos->z < m_WindowPosition.y + m_WindowSize.cy - 50 )
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;											
							m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
						}
						
						m_ResizeMode = _XWRESIZE_TOP;
						changedmousecursor = TRUE;
					}
					else if( scrnpos->z >= m_WindowPosition.y+m_WindowSize.cy-_XDEF_MESSENGERRESIZETRACKSIZE )
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							//m_Resizing = TRUE;											
							m_DragStartPos.y = m_WindowPosition.y + m_WindowSize.cy - scrnpos->z;
						}
											
						//m_ResizeMode = _XWRESIZE_BOTTOM;
						//changedmousecursor = TRUE;
					}					
					else
					{		
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							if( (m_WindowMoveMode != _XWMOVE_FIXED) && !m_Dragging )
							{					
								m_Dragging = TRUE;
								m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
								m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
							}
						}					
					}
				}
			} // if( CheckMousePosition() ) end
		}


		// 리사이즈 모드가 아니면 이전 마우스 포인터 복구
		if( ( !pfocusobject || pfocusobject == this ) && (changedmousecursor || m_Resizing) )
		{			
			switch( m_ResizeMode ) {
			case _XWRESIZE_LEFTTOP :
				break;
			case _XWRESIZE_RIGHTTOP :				
				break;
			case _XWRESIZE_LEFTBOTTOM :
				break;
			case _XWRESIZE_RIGHTBOTTOM :
				break;
			case _XWRESIZE_LEFT :
				break;
			case _XWRESIZE_TOP :
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_VERT );
				break;
			case _XWRESIZE_RIGHT :
				break;
			case _XWRESIZE_BOTTOM :	
				break;			
			default:
				break;
			}	
		}

		if( !mousestate->bButton[0] )
		{
			m_Dragging = FALSE;	
			m_Resizing = FALSE;
			m_DragStartPos.x = m_DragStartPos.y = 0;			
			
			if( m_bDockingMode )
			{
				MoveWindow( gnWidth - (1024 - 841), gnHeight - 355  );//gnHeight - 48 - m_WindowSize.cy  );
				
			}

			if( CheckMousePosition() )
			{	
				if( pfocusobject == (_XGUIObject*)this )
				{
					if( m_CommandID )
					{						
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CommandID, 0), (LPARAM)gHWnd);
						mousestate->bButton[0] = 0;
					}						
					pfocusobject = NULL;
				}		
			}
			if( pfocusobject == (_XGUIObject*)this )
			{
				pfocusobject = NULL;
			}					
		}
		else if( pfocusobject == (_XGUIObject*)this )
		{
			if( m_Resizing )
			{					
				RECT winrect;
				switch(m_ResizeMode) 
				{
				case _XWRESIZE_LEFTTOP:
					break;
				case _XWRESIZE_RIGHTTOP:
					break;
				case _XWRESIZE_LEFTBOTTOM:
					break;
				case _XWRESIZE_RIGHTBOTTOM:
					break;
				case _XWRESIZE_TOP:
					winrect.left   = m_WindowPosition.x;
					winrect.top    = scrnpos->z+m_DragStartPos.y;
					winrect.right  = m_WindowPosition.x+m_WindowSize.cx;
					winrect.bottom = m_WindowPosition.y+m_WindowSize.cy;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_BOTTOM:
					break;
				case _XWRESIZE_LEFT:
					break;
				case _XWRESIZE_RIGHT:
					break;
				default:
					break;
				}
				
			}
			else if( m_Dragging )
			{				
				int newxpos = scrnpos->x + m_DragStartPos.x;
				int newypos = scrnpos->z + m_DragStartPos.y;
				
				if( newxpos < 0	) newxpos = 0;
				if( newypos < 0	) newypos = 0;				
				if( newxpos+m_WindowSize.cx >= gnWidth ) newxpos = gnWidth - m_WindowSize.cx;
				if( newypos+m_WindowSize.cy >= gnHeight) newypos = gnHeight- m_WindowSize.cy;
				
				
				if( newxpos+m_WindowSize.cx > ( gnWidth - 100) )
				{
					if( newypos+m_WindowSize.cy > ( gnHeight - 90 ) )
					{
						if( !m_bDockingMode )
						{
							// 도킹모드일때 
							SetDockingMode(TRUE);
#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER //Author : 양희왕 //breif : 친구소환
							//ResizeWindow( gnWidth - (1024 - 841), gnHeight - 355, gnWidth, gnHeight - 355 + 283 );	
							MoveWindow( gnWidth - (1024 - 841), gnHeight - 355 ); 
#else
							ResizeWindow( gnWidth - (1024 - 841), gnHeight - 48 - m_WindowSize.cy - 18 ,gnWidth, gnHeight- 48 );	
							MoveWindow( gnWidth - (1024 - 841), gnHeight - 48 - m_WindowSize.cy  );
#endif												
							m_Dragging = FALSE;	
							m_Resizing = FALSE;
							m_DragStartPos.x = m_DragStartPos.y = 0;							
							
							if( CheckMousePosition() )
							{	
								if( pfocusobject == (_XGUIObject*)this )
								{
									if( m_CommandID )
									{						
										PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CommandID, 0), (LPARAM)gHWnd);
										mousestate->bButton[0] = 0;
									}						
									pfocusobject = NULL;
								}		
							}
							if( pfocusobject == (_XGUIObject*)this )
							{
								pfocusobject = NULL;
							}				

						}	
						else
						{
							switch( m_WindowMoveMode )
							{
							case _XWMOVE_FREE :
								WindowPosChanging( newxpos, newypos );
								MoveWindow( newxpos, newypos );
								break;
							case _XWMOVE_HORIZ :
								WindowPosChanging( newxpos, (int&)m_WindowPosition.y );
								MoveWindow( newxpos, this->m_WindowPosition.y );
								break;
							case _XWMOVE_VERT :								
								WindowPosChanging( (int&)this->m_WindowPosition.x, newypos );
								MoveWindow( this->m_WindowPosition.x, newypos );
								break;
							}				
							
						}
						
					}	
					else
					{
						if( m_bDockingMode )
						{
							SetDockingMode(FALSE);

#ifndef _XDEF_RECALLSYSTEM_070604_MAGVIPER //Author : 양희왕 //breif : 디파인되어있으면 안들어온다
							ResizeWindow( m_WindowPosition.x, m_WindowPosition.y 
								,m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy - 18  );
#endif
						}
						else
						{
							SetDockingMode(FALSE);
						}
					}		
				}
				else
				{ 
					if( m_bDockingMode )
					{
						SetDockingMode(FALSE);

#ifndef _XDEF_RECALLSYSTEM_070604_MAGVIPER //Author : 양희왕 //breif : 디파인되어있으면 안들어온다
							ResizeWindow( m_WindowPosition.x, m_WindowPosition.y 
								,m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy - 18  );
#endif
					}
					else
					{
						SetDockingMode(FALSE);
					}
				}

				
				if( !m_bDockingMode )
				{
					switch( m_WindowMoveMode )
					{
					case _XWMOVE_FREE :
						WindowPosChanging( newxpos, newypos );
						MoveWindow( newxpos, newypos );
						break;
					case _XWMOVE_HORIZ :
						WindowPosChanging( newxpos, (int&)m_WindowPosition.y );
						MoveWindow( newxpos, this->m_WindowPosition.y );
						break;
					case _XWMOVE_VERT :								
						WindowPosChanging( (int&)this->m_WindowPosition.x, newypos );
						MoveWindow( this->m_WindowPosition.x, newypos );
						break;
					}				
				}
				
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_PICKUP_CLICK );
						
			}
		}
	}

}

BOOL _XWindow_Messenger::CheckGroupListbox()
{
	int selectedlistnumber = SearchListNumber() + m_listboxGroupList->GetScrollPos();
	//int selectedlistnumber = m_SelectedItemIndex;
	if( selectedlistnumber != -1 )
	{
		// 선택된 그룹리스트 체크하기 
		if( !m_MessengerGroupList.listEmpty() )
		{
			m_MessengerGroupList.resetList();
			_XMessengerGroup* currentgroup = NULL;
			do 
			{
				currentgroup = (_XMessengerGroup *)m_MessengerGroupList.currentItem();
				if(currentgroup)
				{
					if(currentgroup->m_GroupNumber == selectedlistnumber )
					{
						return TRUE; // 그룹이 선택되었다는 표시 												
					}
				}
				m_MessengerGroupList.nextItem();
			}while (!m_MessengerGroupList.atHeadOfList());	
			
			
		}
		
	}
	return FALSE;
}

void _XWindow_Messenger::ProcessMoveListbox()
{
	_XWindow_MessengerTooltip* pMessengerTooltip_Window 
		= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
				
	if( m_IsDragingCharacterName[0] != 0 )
	{					
		
		_XVScrollBar* pScrollBar = m_listboxGroupList->GetScrollBarObject();
 		int scrollpos = pScrollBar->GetLineScrollPos();
		int selectedlistnumber = SearchListNumber() + scrollpos;
		//int selectedlistnumber = SearchListNumber() + m_listboxGroupList->GetScrollPos();
		//int selectedlistnumber = m_SelectedItemIndex;
		
		if( selectedlistnumber != -1 )
		{
			if( !m_MessengerGroupList.listEmpty() )
			{
				m_MessengerGroupList.resetList();
				_XMessengerGroup* currentgroup = NULL;
				
				bool checkinsertfriendlist = FALSE;

				do 
				{
					if( checkinsertfriendlist ) break; 
					
					currentgroup = (_XMessengerGroup *)m_MessengerGroupList.currentItem();
					
					// 옮겨진 그룹에 친구리스트를 추가하는 부분 : 그룹을 선택했을 경우
					if( currentgroup->m_GroupNumber == selectedlistnumber )
					{
						currentgroup->m_bSelected = TRUE;
						
						if(selectedlistnumber != m_IsDragingGroupNumber)
						{
							
							memset( m_SelectedGroupname, 0, sizeof(TCHAR)*15 ); 
							memset( m_SelectedCharactername, 0, sizeof(TCHAR)*13 ); 
							strcpy( m_SelectedGroupname, currentgroup->m_GroupName );
							strcpy( m_SelectedCharactername, m_IsDragingCharacterName);
							
							g_NetworkKernel.SendMessengerPacket( MMSG_NO_JOIN_GROUP_REQ_C_S );
							
						}
						else//자기가 자기 자신의 그룹을 선택한 경우는 추가하지 않는다.
						{					
//							pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;							
							break;
						}
					}
					else if(!checkinsertfriendlist)
					{
						// 옮겨진 그룹에 친구리스트를 추가하는 부분 : 친구를 선택했을 경우
						if(!currentgroup->m_FriendList.listEmpty())
						{
							currentgroup->m_FriendList.resetList();
							_XMessengerFriend* currentfriend = NULL;
							do 
							{
								currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
								if(currentfriend)
								{
									if( currentfriend->m_FriendNumber == selectedlistnumber )
									{
										currentgroup->m_bSelected = TRUE;

										if(currentfriend->m_FriendNumber == m_IsDragingCharacterNumber ) break;									
										
										if(currentgroup->m_GroupNumber != m_IsDragingGroupNumber)
										{
											memset( m_SelectedGroupname, 0, sizeof(TCHAR)*15 ); 
											memset( m_SelectedCharactername, 0, sizeof(TCHAR)*13 ); 
											strcpy( m_SelectedGroupname, currentgroup->m_GroupName );
											strcpy( m_SelectedCharactername, m_IsDragingCharacterName);
											
											g_NetworkKernel.SendMessengerPacket( MMSG_NO_JOIN_GROUP_REQ_C_S );													
										}
										else//자기가 자기 자신의 그룹을 선택한 경우는 추가하지 않는다.
										{
//											pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;											
											break;
										}
										
									}
								}
								else
								{
//									pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
									break;
								}
								currentgroup->m_FriendList.nextItem();							
							} while(!currentgroup->m_FriendList.atHeadOfList());						
						}												
					}
					m_MessengerGroupList.nextItem();
				} while(!m_MessengerGroupList.atHeadOfList());					
			}			
		}
	}

//	pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
	
	memset( m_IsDragingCharacterName, 0, sizeof(TCHAR)*13 );
	memset( m_IsDragingGroupName, 0, sizeof(TCHAR)*15 );
	m_IsDragingCharacterNumber = -1;
	m_IsDragingGroupNumber = -1;
}

void _XWindow_Messenger::SortGroupName()
{
	//그룹 이름을 정렬하는 부분 
	int itemcount = m_MessengerGroupList.getitemcount();
	
	for( int i = 0; i < itemcount; i++ )
	{
		m_MessengerGroupList.resetList();
		
		for( int j = 0; j < itemcount-1; j++ )
		{
			_XMessengerGroup* currentgroup = NULL;
			currentgroup = (_XMessengerGroup *)m_MessengerGroupList.currentItem();
			
			if( !currentgroup || !m_MessengerGroupList.currentItem()->getright() )
			{				
				break;
			}
			
			if( strcmp(currentgroup->m_GroupName, ((_XMessengerGroup*)(m_MessengerGroupList.currentItem()->getright()))->m_GroupName) > 0 )
			{				
				m_MessengerGroupList.currentItemSwap();
			}
			else
			{
				m_MessengerGroupList.nextItem();
			}
		}    
	}

}

void _XWindow_Messenger::SortFriendName()
{
	//친구 이름을 정렬하는 부분 
	if( !m_MessengerGroupList.listEmpty() )
	{
		m_MessengerGroupList.resetList();
		_XMessengerGroup* currentgroup = NULL;
		do 
		{
			currentgroup = (_XMessengerGroup *)m_MessengerGroupList.currentItem();
			if(currentgroup)
			{
				if(!currentgroup->m_FriendList.listEmpty())
				{
					
					int itemcount = currentgroup->m_FriendList.getitemcount();
					
					for( int i = 0; i < itemcount; i++ )
					{
						currentgroup->m_FriendList.resetList();
						
						// 이름 순서대로 정렬
						for( int j = 0; j < itemcount-1; j++ )
						{
							_XMessengerFriend* currentfriend = NULL;
							currentfriend = (_XMessengerFriend *)currentgroup->m_FriendList.currentItem();
							
							if( !currentfriend || !currentgroup->m_FriendList.currentItem()->getright() )
							{				
								break;
							}
							
							if( strcmp(currentfriend->m_FriendName, ((_XMessengerFriend*)(currentgroup->m_FriendList.currentItem()->getright()))->m_FriendName) > 0 )
							{				
								currentgroup->m_FriendList.currentItemSwap();
							}
							else
							{
								currentgroup->m_FriendList.nextItem();
							}
						}   
					}					
				}					
			}			
			m_MessengerGroupList.nextItem();
		} while(!m_MessengerGroupList.atHeadOfList());
	}
	
}


void _XWindow_Messenger::ReleaseMessengerList()
{
	
	if( !m_MessengerGroupList.listEmpty() )
	{
		m_MessengerGroupList.resetList();
		_XMessengerGroup* currentgroup = NULL;
		do 
		{
			currentgroup = (_XMessengerGroup *)m_MessengerGroupList.currentItem();
			if(currentgroup)
			{				
				currentgroup->m_FriendList.disposeList();
			}
			m_MessengerGroupList.nextItem();
		} while(!m_MessengerGroupList.atHeadOfList());
	}
	m_MessengerGroupList.disposeList();
}

void _XWindow_Messenger::ProcessSelectMovingGroup()
{
	int selectedlistnumber = SearchListNumber() + m_listboxGroupList->GetScrollPos();
	//int selectedlistnumber = m_SelectedItemIndex;
				
	if( selectedlistnumber != -1 )
	{
		// 선택된 그룹리스트 체크하기 
		if( !m_MessengerGroupList.listEmpty() )
		{
			m_MessengerGroupList.resetList();
			_XMessengerGroup* currentgroup = NULL;
			do 
			{
				currentgroup = (_XMessengerGroup *)m_MessengerGroupList.currentItem();
				if(currentgroup)
				{
					if(currentgroup->m_GroupNumber == selectedlistnumber )
					{									
						m_listboxGroupList->SelectItem( selectedlistnumber );
						break;
					}
					else if(currentgroup->m_bSelected)
					{
						if(!currentgroup->m_FriendList.listEmpty())
						{
							currentgroup->m_FriendList.resetList();
							_XMessengerFriend* currentfriend = NULL;
							do 
							{
								currentfriend = (_XMessengerFriend *)currentgroup->m_FriendList.currentItem();
								if(currentfriend)
								{
									if(currentfriend->m_FriendNumber == selectedlistnumber )
									{
										m_listboxGroupList->SelectItem( currentgroup->m_GroupNumber );
										break;
									}
								}
								currentgroup->m_FriendList.nextItem();
							}while (!currentgroup->m_FriendList.atHeadOfList());
						}						
					}
				}
				m_MessengerGroupList.nextItem();
			}while (!m_MessengerGroupList.atHeadOfList());	
		}
	}
}

void _XWindow_Messenger::SetDockingMode(BOOL flag)
{
	m_bDockingMode = flag;

	_XWindow_SystemMsgDefault* SystemMsgWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);	
	if( SystemMsgWindow )
		SystemMsgWindow->SetDockingMode(flag);
}

