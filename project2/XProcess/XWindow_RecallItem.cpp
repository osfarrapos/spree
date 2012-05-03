// XWindow_RecallItem.cpp: implementation of the XWindow_RecallItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XProc_ProcessDefine.h"
#include "xsr_stringheader.h"

#include "XWindow_Messenger.h"
#include "XMessengerGroup.h"
#include "XMessengerFriend.h"

#include "XWindow_SystemMsgDefault.h"
#include "XWindow_BloodPoint.h"
#include "XWindow_MnDTotalPoint.h" //Author : 양희왕 //breif :사제 시스템
#include "XWindow_MnDCurrentPoint.h" //Author : 양희왕 //breif :사제 시스템

#include "XWindow_Escape.h"

#include "XWindow_RecallItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static int _XDEF_INVENTORYITEMPOSX = 11;
static int _XDEF_INVENTORYITEMPOSY = 71;

static int _XDEF_INVENTORYSLOT_WIDTH = 5;
static int _XDEF_INVENTORYSLOT_HEIGHT = 5;

_XWindow_RecallItem::_XWindow_RecallItem() : m_listboxGroupList(NULL), m_nRecallItemIndex(0), m_nSelectedItem(0), m_nCharState(0), m_bMoving(FALSE), m_pMoveButton(NULL), m_pCallButton(NULL)
{

}

_XWindow_RecallItem::~_XWindow_RecallItem()
{

}

BOOL _XWindow_RecallItem::Initialize()
{
	int mainbackresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	int messengerresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	int messengerresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" );

	// 메신져 타이틀 바	( 183 X 18 )
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 183, 67, &g_MainInterfaceTextureArchive, mainbackresourceindex1 );
	pTitleBar->SetClipRect(0, 125, 183, 192);
	pTitleBar->ShowWindow(TRUE);
	InsertChildObject( pTitleBar );

	// 메신져 왼쪽 사이드 이미지 ( 10 X 158 )
	_XImageStatic* pLeftSideBorder = new _XImageStatic;
	pLeftSideBorder->Create( 0, 55, 10, 213, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	pLeftSideBorder->SetClipRect(226, 0, 236, 158);
	pLeftSideBorder->ShowWindow(TRUE);
	InsertChildObject( pLeftSideBorder );

	// 메신져 오른쪽 사이드 이미지 ( 20 X 158 )
	_XImageStatic* pRightSideBorder = new _XImageStatic;
	pRightSideBorder->Create( 163, 55, 183, 213, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	pRightSideBorder->SetClipRect(236, 0, 256, 158);
	pRightSideBorder->ShowWindow(TRUE);
	InsertChildObject( pRightSideBorder );
		
	// 메신져 창 아래쪽  바탕	 ( 183 X 52 )
	_XImageStatic* pDownBorder = new _XImageStatic;
	pDownBorder->Create( 0, 213, 183, 265, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	pDownBorder->SetClipRect( 0, 26, 183, 78);
	pDownBorder->ShowWindow(TRUE);
	InsertChildObject( pDownBorder );
		
	// 메신져 창 아래쪽 끝 라인	 ( 183 X 3 )
	_XImageStatic* pDownEndLine = new _XImageStatic;
	pDownEndLine->Create( 0, 266, 183, 269, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	pDownEndLine->SetClipRect( 0, 106, 183, 109);
	pDownEndLine->ShowWindow(TRUE);
	InsertChildObject( pDownEndLine );

	//클로즈 버튼
	_XBTN_STRUCTURE closeminibtnstruct = 
	{ 
		TRUE,
		{ m_WindowSize.cx - 14, 2 }, 
		{ 12, 12 }, 
		_XDEF_RECALLITEM_MINICLOSEBUTTON,
		g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName),
		g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName),
		g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName), 
		&g_MainInterfaceTextureArchive };

	_XButton* pCloseMinibutton = new _XButton;
	pCloseMinibutton->Create( closeminibtnstruct );
	pCloseMinibutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 96,  238, 107, 249 );
	pCloseMinibutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 119, 249 );
	pCloseMinibutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 131, 249 );
	InsertChildObject( pCloseMinibutton );


	// 메신져 버튼 ( 60 X 24 )
	_XBTN_STRUCTURE callbtnstruct = 
	{
		TRUE, {21, 215}, {60, 24}, _XDEF_RECALLITEM_CALLBUTTON,
			messengerresourceindex1, messengerresourceindex1, messengerresourceindex1,
			&g_MainInterfaceTextureArchive
	};
	
	m_pCallButton = new _XButton;
	m_pCallButton->Create(callbtnstruct );
	m_pCallButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_pCallButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_pCallButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	m_pCallButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3489 );	// _T("호출")
	m_pCallButton->ShowWindow(TRUE);
	InsertChildObject(m_pCallButton);

	// 메신져 버튼 ( 60 X 24 )
	_XBTN_STRUCTURE movebtnstruct = 
	{
		TRUE, {100, 215}, {60, 24}, _XDEF_RECALLITEM_MOVEBUTTON,
			messengerresourceindex1, messengerresourceindex1, messengerresourceindex1,
			&g_MainInterfaceTextureArchive
	};
	
	m_pMoveButton = new _XButton;
	m_pMoveButton->Create(movebtnstruct);
	m_pMoveButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_pMoveButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_pMoveButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	m_pMoveButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3490 );	// _T("이동")
	m_pMoveButton->ShowWindow(TRUE);
	InsertChildObject(m_pMoveButton);

	// 메신져 버튼 ( 60 X 24 )
	_XBTN_STRUCTURE closebtnstruct = 
	{
		TRUE, {62, 242}, {60, 24}, _XDEF_RECALLITEM_CLOSEBUTTON,
			messengerresourceindex1, messengerresourceindex1, messengerresourceindex1,
			&g_MainInterfaceTextureArchive
	};
	
	_XButton* pCloseButton = new _XButton;
	pCloseButton->Create(closebtnstruct);
	pCloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	pCloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	pCloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	pCloseButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CLOSE );	// _T("닫기")
	pCloseButton->ShowWindow(TRUE);
	InsertChildObject(pCloseButton);

	int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );

	//메신져 그룹과 친구리스트를 표시하는 리스트 박스 
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 8, 48  },										// 윈도우 좌표
		{ 168, 160 },									// 윈도우 사이즈
		_XDEF_RECALLITEM_LISTBOX,						// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(""),											// 윈도우 타이틀
		16,												// 리스트 아이템 라인 피치
		10,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치

		// scroll bar property
		{ 12, 160 },									// 스크롤바 사이즈
		12,												// 스크롤바 상하 버튼 사이즈
		22,												// 스크롤바 트랙바 사이즈
		0,												// 전체 리스트 갯수
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
	
	InsertChildObject(m_listboxGroupList);

	m_listboxGroupList->SetAlwaysShowScrollBarMode(TRUE);
	//m_listboxGroupList->SetDrawBorder( TRUE );
	//m_listboxGroupList->SetBorderColorFactor( D3DCOLOR_ARGB( 160, 0, 0, 0 ) );

	// 메신져 리스트에서 드래깅 되는 친구 이름이나 그룹 이름을 표시함
	_XTOOLTIP_STRUCTURE draginglistboxstruct = 
	{
		{ 0, 0 },
		{ 140, 15 },
		{ 1, 1 },
		-1,
		NULL
	};
	/*m_DragingDisplayBox = new _XToolTip;
	m_DragingDisplayBox->Create(draginglistboxstruct);
	m_DragingDisplayBox->ShowWindow(FALSE);*/

	return TRUE;
}

void _XWindow_RecallItem::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_RecallItem::MoveWindow( int X, int Y )
{			
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_RecallItem::ShowWindow( BOOL show )
{
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
	_XWindow_SystemMsgDefault* systemWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);

	if( show )
	{
		SetListItem();
		
		systemWindow->MoveWindow(m_WindowPosition.x - 374, gnHeight-195);
		systemWindow->m_SystemMsgListScrollBar->MoveWindow(m_WindowPosition.x - 13,gnHeight - 182);

	#ifdef _XTS_BLOODPOINT
		// system msg 창 떠있음
		_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
		if(pBloodPoint_Window)
		{
			if(systemWindow->GetShowStatus())
				pBloodPoint_Window->MoveWindow(m_WindowPosition.x - 60, gnHeight - 120 - 96 );
			else
				pBloodPoint_Window->MoveWindow(m_WindowPosition.x - 60, gnHeight - 99 - 96 );
		}
	#endif
	
	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
		_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
		if(pMnDCurrentPoint_Window)
		{
			pMnDCurrentPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 19 );
		}
		
		_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
		if(pMnDTotalPoint_Window)
		{
			pMnDTotalPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDCurrentPoint_Window->GetWindowPos().y - 19 );
		}
	#endif
	}
	else
	{
		m_listboxGroupList->DeleteAllItem();
		
		if( g_pInventory_Window->GetShowStatus() ||
			(pMessenger_Window->GetShowStatus() &&pMessenger_Window->m_bDockingMode) )
			 systemWindow->SetDockingMode(TRUE);	
		 else
			 systemWindow->SetDockingMode(FALSE);
	}

	m_nSelectedItem = 0;

	_XWindow::ShowWindow( show );
}

void _XWindow_RecallItem::Draw( _XGUIObject*& pfocusobject )
{
	//그리는 순서
	if( !this->m_ShowWindow ) return;

	_XWindow::Draw(pfocusobject);

	m_listboxGroupList->Draw(pfocusobject);

	POINT listwinpos;
	SIZE listwinsize;
	
	listwinpos = m_listboxGroupList->GetWindowPos();
	listwinsize = m_listboxGroupList->GetWindowSize();
	
	//끝에 영역선 그리기
	_XDrawRectAngle( listwinpos.x, listwinpos.y+1,
		listwinpos.x+listwinsize.cx-13, listwinpos.y+listwinsize.cy+1, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0) );
}

BOOL _XWindow_RecallItem::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	//시스템 메시지 창과 도킹체크를 하자
	_XWindow_SystemMsgDefault* systemWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);

	RECT curwindowrect = { m_WindowPosition.x, m_WindowPosition.y, 
						   m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy };

	//가장 왼쪽 하단 모서리 윈도우 좌표만 체크한다
	 if( curwindowrect.left >= gnWidth - 557 && curwindowrect.bottom >= gnHeight-195 ) 
	 {
		 systemWindow->MoveWindow(m_WindowPosition.x - 374, gnHeight-195);
		 systemWindow->m_SystemMsgListScrollBar->MoveWindow(m_WindowPosition.x - 13,gnHeight - 182);

#ifdef _XTS_BLOODPOINT
		// system msg 창 떠있음
		_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
		if(pBloodPoint_Window)
		{
			if(systemWindow->GetShowStatus())
				pBloodPoint_Window->MoveWindow(m_WindowPosition.x - 60, gnHeight - 120 - 96 );
			else
				pBloodPoint_Window->MoveWindow(m_WindowPosition.x - 60, gnHeight - 99 - 96 );
		}
#endif

	 #ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
		_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
		if(pMnDCurrentPoint_Window)
		{
			pMnDCurrentPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 19 );
		}
		
		_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
		if(pMnDTotalPoint_Window)
		{
			pMnDTotalPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDCurrentPoint_Window->GetWindowPos().y - 19 );
		}
	 #endif
	 }
	 else
	 {
		 // 인벤토리나 메신져 윈도우가 그려지고 있을땐 도킹
		 if( g_pInventory_Window->GetShowStatus() ||
			pMessenger_Window->GetShowStatus() && pMessenger_Window->m_bDockingMode )
			 systemWindow->SetDockingMode(TRUE);	
		 else
			 systemWindow->SetDockingMode(FALSE);	
	 }

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

BOOL _XWindow_RecallItem::SearchSlotItem( int nIndex )
{
	int count			= _XINVENTORY_SLOTTYPE_POCKETSTART; //처음엔 포켓
	int columnpoint		= g_pInventory_Window->GetWindowPos().x + _XDEF_INVENTORYITEMPOSX;
	int rowpoint		= g_pInventory_Window->GetWindowPos().y + _XDEF_INVENTORYITEMPOSY;	

	// 주머니 리스트 검사
	count = _XINVENTORY_SLOTTYPE_POCKETSTART; //인벤의 가장 처음 시작위치 (맨위칸 이 25 )

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
    for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == _XGI_FC_POTION)
		{
			if(g_PotionItemProperty[g_pLocalUser->m_UserItemList[i].Get_m_sID()].cSecondType == nIndex)
			{
				m_nRecallItemIndex = i;
				return TRUE;
			}
		}				
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == _XGI_FC_POTION)
		{
			if(g_PotionItemProperty[g_pLocalUser->m_UserItemList[i].Get_m_sID()].cSecondType == nIndex)
			{
				m_nRecallItemIndex = i;
				return TRUE;
			}
		}		
	}
#else
	//인벤토리 전체 검색
	for( int row = 0; row < _XDEF_INVENTORYSLOT_HEIGHT; row++ )
	{
		for( int column = 0; column < _XDEF_INVENTORYSLOT_WIDTH; column++ )
		{
			if( count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT ) break;

			// second type == 14로 찾도록 고친다
//			if( g_pLocalUser->m_UserItemList[count].m_sID == nIndex )
			if(g_pLocalUser->m_UserItemList[count].Get_m_cType() == _XGI_FC_POTION)
			{
				if(g_PotionItemProperty[g_pLocalUser->m_UserItemList[count].Get_m_sID()].cSecondType == nIndex)
				{
					m_nRecallItemIndex = count;
					return TRUE;
				}
			}
			columnpoint+=33;
			count++;			
		}

		if( count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT ) break;
		
		columnpoint = g_pInventory_Window->GetWindowPos().x + _XDEF_INVENTORYITEMPOSX;
		rowpoint += 33;

		//끝까지 다 돌았다... 주머니를 바꿔주고 다시 검사를 한다
		if( count == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART || count == _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART)
		{
			columnpoint = g_pInventory_Window->GetWindowPos().x + _XDEF_INVENTORYITEMPOSX;
			rowpoint = g_pInventory_Window->GetWindowPos().y + _XDEF_INVENTORYITEMPOSY;

			row = -1;	//초기화 row++ 해주기때문
			column = 0; 
		}
	}
#endif

	return FALSE;
}

BOOL _XWindow_RecallItem::CharRecallCondition() //내가 호출할때 나의 상태 체크
{
	_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);	
	TCHAR tempString[256];
	memset(tempString, 0, sizeof(tempString));
    int areanumber = 0;

	CEmperorOfDragonsApp* pApp = ((CEmperorOfDragonsApp*)gApp);

	/*if( g_CurrentZoneInfoIndex == 21 || //패왕동
		g_pLocalUser->m_bIsBossBattle || //결 진행중
		g_pLocalUser->m_bCastleBattle || //장원전참여중
		g_pLocalUser->m_bOut || //자리비움
		g_pLocalUser->m_bMatchPRMode || // 비무홍보
		g_pLocalUser->m_CharacterLog.isTrade || //교환중
#ifdef _XTS_PK
		g_pLocalUser->m_BattleMode != _XBATTLE_PEACE || //전투상태
#else
		g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE || 
#endif
#ifdef _XTS_PK
		g_pLocalUser->m_BattleMode == _XBATTLE_PK || // 대전중
#endif
		( g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
		(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST) ) || // 내공심법 시전중
		g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING || //수련중
		(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT) || //비무중
		g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE || //개인상점중
		( g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL && 
		g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 ) ||//경공모드
		pEscape_Window->GetShowStatus() ) //탈출 
	{
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}*/

	if( g_CurrentZoneInfoIndex == 21 )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3652) );//_T("패왕동에서는 호출/이동할 수 없습니다.") );
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
	else if( g_pLocalUser->m_bIsBossBattle )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3653 ) );//_T("결 진행 중에는 호출/이동할 수 없습니다.") );
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
	else if( g_pLocalUser->m_bCastleBattle )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3654) );//_T("장원전 참여 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
    else if( g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_CASTLE, areanumber) )
    {
        sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3654) );//_T("장원전 참여 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
    }
	else if( g_pLocalUser->m_bOut )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3655) );//_T("자리비움 상태에서는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
	else if( g_pLocalUser->m_bMatchPRMode )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3656) );//_T("비무홍보 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
	else if( g_pLocalUser->m_CharacterLog.isTrade )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3657) );//_T("교환 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
/*#ifdef _XTS_PK
	else if( g_pLocalUser->m_BattleMode != _XBATTLE_PEACE ) //전투상태
#else
	else if( g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE )
#endif
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3831)); //"평화 상태에서만 호출/이동이 가능 합니다.
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}*/
#ifdef _XTS_PK
	else if( g_pLocalUser->m_BattleMode == _XBATTLE_PK ) // 대전중
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3658) );//_T("대전 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
#endif
	else if( ( g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
		(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST) ) ) //내공심법
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3659) );//_T("내공심법 시전 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
	else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3660) );//_T("수련 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
	else if( (g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT) )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3661) );//_T("비무 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
	else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3662) );//_T("개인상점 진행 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
	/*else if( ( g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL && 
		g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 ) )
	{
		sprintf(tempString, _T("경공 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}*/
	else if( pEscape_Window->GetShowStatus() )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3663) );//_T("탈출 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
	else if( pApp->m_proc_maingame.m_bNPCDialogMode || pApp->m_proc_maingame.m_bNPCDialogWideView )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3664) );//_T("NPC 대화 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
    else if(g_pLocalUser->GetEnterMatchEvent())
    {
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3661) );//_T("비무 중에는 호출/이동할 수 없습니다."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		m_nCharState = 1; //거절할때만 쓰인다
		return FALSE;
	}           
#endif

	m_nCharState = 0; //거절할때만 쓰인다

	return TRUE;
}

//친구목록에 케릭터의 이름이 존재 하는지..
BOOL _XWindow_RecallItem::CheckCharacterName( LPCTSTR pString )
{
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);

	//그룹 검색 하면서.. 친구 이름만 리턴
	if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
	{
		pMessenger_Window->m_MessengerGroupList.resetList();
		_XMessengerGroup* currentgroup = NULL;
		do 
		{
			currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
			if(currentgroup)
			{
				if(!currentgroup->m_FriendList.listEmpty())
				{
					currentgroup->m_FriendList.resetList();
					_XMessengerFriend* currentfriend = NULL;
					do 
					{
						currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
						if(!stricmp( currentfriend->m_FriendName, pString ))
							return TRUE;

						currentgroup->m_FriendList.nextItem();							
					} while(!currentgroup->m_FriendList.atHeadOfList());
				}
			}
			pMessenger_Window->m_MessengerGroupList.nextItem();
		} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());
	}

	return FALSE;
}

LPCTSTR	_XWindow_RecallItem::GetCharacterName( int nIndex )
{
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);

	//그룹 검색 하면서.. 친구 이름만 리턴
	if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
	{
		pMessenger_Window->m_MessengerGroupList.resetList();
		_XMessengerGroup* currentgroup = NULL;
		do 
		{
			currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
			if(currentgroup)
			{
				if(!currentgroup->m_FriendList.listEmpty())
				{
					currentgroup->m_FriendList.resetList();
					_XMessengerFriend* currentfriend = NULL;
					do 
					{
						currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
						if( currentfriend->m_FriendNumber == nIndex )
							return currentfriend->m_FriendName;

						currentgroup->m_FriendList.nextItem();							
					} while(!currentgroup->m_FriendList.atHeadOfList());
				}
			}
			pMessenger_Window->m_MessengerGroupList.nextItem();
		} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());
	}

	return NULL;
}

void _XWindow_RecallItem::SetListItem()
{
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);

	TCHAR	groupnamebuffer[256];
	TCHAR	friendnamebuffer[256];
	TCHAR	friendnumberbuffer[64];
	TCHAR	friendstatusbuffer[128];
	
	
	memset(groupnamebuffer, 0, sizeof(groupnamebuffer));
	memset(friendnamebuffer, 0, sizeof(friendnamebuffer));
	memset(friendnumberbuffer, 0, sizeof(friendnumberbuffer));
	memset(friendstatusbuffer, 0, sizeof(friendstatusbuffer));
	
	m_listboxGroupList->DeleteAllItem();
	m_listboxGroupList->SetScrollPos(0);

	int count = 0 ;

	if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
	{
		pMessenger_Window->m_MessengerGroupList.resetList();
		_XMessengerGroup* currentgroup = NULL;
		do 
		{
			currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
			if(currentgroup)
			{
				int messengerresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );

				/*int friendnumber = currentgroup->m_FriendList.getitemcount();
				sprintf( friendnumberbuffer, " (%d)",friendnumber);				

				sprintf( groupnamebuffer, currentgroup->m_GroupName ); 
				strcat( groupnamebuffer, friendnumberbuffer );
				
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
				m_listboxGroupList->SetItemAttrib(count, 3, -1 , rect);*/
				
				//currentgroup->m_GroupNumber = count; // 리스트 박스의 인텍스 값과 동기화
				//count++;

				//if( currentgroup->m_bSelected )
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

									//친구넘버에 하나를 더하여 저장하도록 한다..
									count++;
									currentfriend->m_FriendNumber = count;	
								}								
							}
							currentgroup->m_FriendList.nextItem();							
						} while(!currentgroup->m_FriendList.atHeadOfList());						
					}					
				}
			}
			pMessenger_Window->m_MessengerGroupList.nextItem();
		} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());
	}
}

void _XWindow_RecallItem::Update()
{
	//Process 와 별개로 처리

	//이동을 하려고 하는가 ??
	if( m_bMoving )
	{
		//이펙트가 종료 되었다면..
		if( NULL == g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(m_nEffectID) )
		{
			m_bMoving = FALSE;

			TCHAR tempString[128];
			memset(tempString, 0, sizeof(tempString));

			switch( m_Recallresult.ucFriendMode )
			{
			case en_rf_recv_answer_yes : //요청 수락 패킷을 받음 //만들어진 마법진이 종료되었으면..
				{
					MSG_RF_RECV_ANSWER_YES* tempresult = (MSG_RF_RECV_ANSWER_YES*)&m_Recallresult;

					sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3541), tempresult->cTargetName ); //%s님께서 소환되었습니다.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );

					g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Set_m_ucCount(tempresult->ucLeftCount); //지정된 슬롯에.. 남은개수를 저장

					memset(tempString, 0, sizeof(tempString));
					sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3542) );//소환령 1개가 소모 되었습니다
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );

					//0개
					if( g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Get_m_ucCount() < 1 )
						g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Reset();
				}
				break;
			case en_rf_recv_goto_answer_yes : //이동하여 가자 //이동하여 가는 이펙트가 종료되었다면..
				{
					MSG_RF_RECV_GOTO_ANSWER_YES* tempresult = (MSG_RF_RECV_GOTO_ANSWER_YES*)&m_Recallresult;

					g_pLocalUser->m_TargetPosition.x = tempresult->fTargetX + 1;
					g_pLocalUser->m_TargetPosition.z = tempresult->fTargetZ + 1;

					g_pLocalUser->m_Position.x = tempresult->fTargetX;
					g_pLocalUser->m_Position.z = tempresult->fTargetZ;
					g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight(g_pLocalUser->m_Position, g_pLocalUser->m_pGoundCollideOBB);

					sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3546), tempresult->cTargetName ); //"%s님에게 이동되었습니다."
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );

					g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Set_m_ucCount(tempresult->ucLeftCount); //지정된 슬롯에.. 남은개수를 저장

					memset(tempString, 0, sizeof(tempString));
					sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3542) );//소환령 1개가 소모 되었습니다
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );

					//0개
					if( g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Get_m_ucCount() < 1 )
						g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Reset();
				}
				break;
			}
		}
	}

}
