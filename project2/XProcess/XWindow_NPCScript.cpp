// XWindow_NPCScript.cpp: implementation of the XWindow_NPCScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_NPCScript.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_Inventory.h"
#include "XWindow_Warehouse.h"// 2004.06.01->oneway48 insert

#include "XWindow_ChattingDefault.h"
#include "XWindow_Option.h"
#include "XWindow_PCTrade.h"
#include "XWindow_Party.h"
#include "XWindow_PartyMemberMiniBar.h"
#include "XWindow_PartyMasterMiniBar.h"
#include "XWindow_Skill.h"
#include "XWindow_NPCTrade.h"
#include "XWindow_TradeConfirm.h"
#include "XWindow_AddSaveItem.h"
#include "XWindow_MaximumMap.h"
#include "XWindow_ChattingDefault.h"
#include "XWindow_MainMenu.h"
#include "XWindow_Messenger.h"
#include "XWindow_JinStatus.h"
#include "XWindow_MonsterStatus.h"
#include "XWindow_NPCSkill.h"
#include "XWindow_Rebirth.h"
#include "XWindow_RepairItem.h"
#include "XWindow_NameInput.h"
#include "XWindow_TutorialIcon.h"
#include "XWindow_QuestRank.h"
#include "XMessageWindow.h"

#include "XWindowObjectDefine.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern D3DCOLOR g_ChattingStringColor[16];

void __stdcall _XIMECallback_NPCDialogModeQuestInputReturn( _XIMEContainer* pIMEContainer )
{
	_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
	
	if( !pNPCScript_Window ) return;
	if( !pNPCScript_Window->m_QuestIMEControl.GetFocus() ) return;
	memset( pNPCScript_Window->m_QuestString, 0, sizeof(pNPCScript_Window->m_QuestString) );
	strcpy( pNPCScript_Window->m_QuestString , pNPCScript_Window->m_QuestIMEControl.GetText());
	pNPCScript_Window->m_QuestIMEControl.ClearBuffer();
	_XIMEKERNEL.SetFocus( NULL );	
	_XIMEKERNEL.ResetCombinationBuffer();	
	pNPCScript_Window->m_bQuestStringInputComplete = TRUE;

}

void __stdcall _XIMECallback_EventCodeInputReturn( _XIMEContainer* pIMEContainer )
{
	_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
	
	if( !pNPCScript_Window ) return;
	if( !pNPCScript_Window->m_MiniStopEventIMEControl.GetFocus() ) return;
	if( pNPCScript_Window->m_MessageType == _XDEF_NPCMESSAGE_TRACING ) return;
	if( pNPCScript_Window->m_MessageType == _XDEF_NPCMESSAGE_MASTERNAME  ) return; //Author : 양희왕 //breif : 사제 사호입력 일떄
	
	
	POINT pt = pNPCScript_Window->m_MiniStopEventIMEControl.GetWindowPosition();
	POINT winpos = pNPCScript_Window->GetWindowPos();
	
	
	if( pt.x == winpos.x+47 )
	{
		strncpy( pNPCScript_Window->m_EventCodeString01,pNPCScript_Window->m_MiniStopEventIMEControl.GetText(),5);
		memset(pNPCScript_Window->m_EventCodeString02, 0, sizeof(TCHAR)*5);
		pNPCScript_Window->m_MiniStopEventIMEControl.MoveWindow(winpos.x+97, winpos.y + 345);
		pNPCScript_Window->m_MiniStopEventIMEControl.ClearBuffer();
		pNPCScript_Window->m_MiniStopEventIMEControl.SetFocus();
	}
	else if( pt.x == winpos.x+97 )
	{
		strncpy( pNPCScript_Window->m_EventCodeString02,pNPCScript_Window->m_MiniStopEventIMEControl.GetText(),5);
		memset(pNPCScript_Window->m_EventCodeString03, 0, sizeof(TCHAR)*5);
		pNPCScript_Window->m_MiniStopEventIMEControl.MoveWindow(winpos.x+147, winpos.y + 345);
		pNPCScript_Window->m_MiniStopEventIMEControl.ClearBuffer();
		pNPCScript_Window->m_MiniStopEventIMEControl.SetFocus();
	}
}

void __stdcall _XIMECallback_EventCodeInputTab( _XIMEContainer* pIMEContainer )
{
	_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
	
	if( !pNPCScript_Window ) return;
	if( !pNPCScript_Window->m_MiniStopEventIMEControl.GetFocus() ) return;
	if( pNPCScript_Window->m_MessageType == _XDEF_NPCMESSAGE_TRACING ) return;
	if( pNPCScript_Window->m_MessageType == _XDEF_NPCMESSAGE_MASTERNAME  ) return; //Author : 양희왕 //breif : 사제 사호입력 일떄
	
	POINT pt = pNPCScript_Window->m_MiniStopEventIMEControl.GetWindowPosition();
	POINT winpos = pNPCScript_Window->GetWindowPos();
	
	
	if( pt.x == winpos.x+47 )
	{
		strncpy( pNPCScript_Window->m_EventCodeString01,pNPCScript_Window->m_MiniStopEventIMEControl.GetText(),5);
		memset(pNPCScript_Window->m_EventCodeString02, 0, sizeof(TCHAR)*5);
		pNPCScript_Window->m_MiniStopEventIMEControl.MoveWindow(winpos.x+97, winpos.y + 345);
		pNPCScript_Window->m_MiniStopEventIMEControl.ClearBuffer();
		pNPCScript_Window->m_MiniStopEventIMEControl.SetFocus();
	}
	else if( pt.x == winpos.x+97 )
	{
		strncpy( pNPCScript_Window->m_EventCodeString02,pNPCScript_Window->m_MiniStopEventIMEControl.GetText(),5);
		memset(pNPCScript_Window->m_EventCodeString03, 0, sizeof(TCHAR)*5);
		pNPCScript_Window->m_MiniStopEventIMEControl.MoveWindow(winpos.x+147, winpos.y + 345);
		pNPCScript_Window->m_MiniStopEventIMEControl.ClearBuffer();
		pNPCScript_Window->m_MiniStopEventIMEControl.SetFocus();
	}
}


_XWindow_NPCScript::_XWindow_NPCScript()
{
	m_NPCDialogBorder					= NULL;
	m_NPCDialogSubBorderA1				= NULL;
	m_NPCDialogSubBorderA2				= NULL;
	m_NPCDialogSubBorderB1				= NULL;
	m_NPCDialogSubBorderB2				= NULL;
	m_NPCDialogMainTitleBorder			= NULL;
	m_NPCDialogIMEBorder1				= NULL;
	m_NPCDialogIMEBorder2				= NULL;
	m_OrganizationNameIMEBorder1		= NULL;
	m_OrganizationNameIMEBorder2		= NULL;
	m_MiniStopEventIMEBorder1			= NULL;
	m_MiniStopEventIMEBorder2			= NULL;
	m_EventIMEBorder1					= NULL;					
	m_EventIMEBorder2					= NULL;
	m_imageEventContent01				= NULL;
	m_imageEventContent02				= NULL;
	m_imageEventContent03				= NULL;
	m_imageEventContent04				= NULL;
	
	m_btnNPCDialogModExit			= NULL;
	m_listboxNPCDialogMessage			= NULL;

	m_lpszNPCDialogMessage				= NULL;
	m_NPCDialogPositionTimeConter		= 0;
	m_NPCDialogPositionCounter			= 0;
	m_NPCDialogLineCounter				= 0;
	
	memset( &m_NPCDialogSplitinfo, 0, sizeof(_XStringSplitInfo) );
	memset(m_pTitleString, 0, sizeof(TCHAR)*256);
		
	m_bFirstDialog						= TRUE; // 2004.04.28->oneway48 insert
	m_NPCDialogCount					= 0;// 2004.06.20->oneway48 close3-2
	
	m_NPCMode							= _XNPC_NONE;//2004.07.01->oneway48 insert
	m_ChatDisplayTimer					= 0;//2004.07.08->oneway48 insert
	m_DrawLineCount						= 0;//2004.07.08->oneway48 insert
	m_bNewChatDisplay					= FALSE;
	m_bProgressNPCDialogMessage			= FALSE;
	m_bPageClickQuestDialog				= FALSE;

	m_QuestType							= _XQUEST_TYPE_NONE;
	m_MessageType						= _XDEF_NPCMESSAGE_NORMAL;
	memset( m_HaveQuestName, 0, sizeof(TCHAR)*256 );
	memset( m_HaveQuestType, 0, sizeof(TCHAR)*128 );
	m_btnQuestOK						= NULL;
	m_btnBackToNPCMode					= NULL;
	m_listboxQuestReward				= NULL;	// 퀘스트에 관련되어서 진행할 때 보상물을 표시하는 리스트 박스
	m_listboxSelectionItem				= NULL;	// 퀘스트에서 선택지를 표시할 리스트 박스
	m_listboxHaveQuestList				= NULL;	// 가지고 있는 퀘스트 리스트를 표시할 리스트 박스
	m_imageQuesLevelDisplay				= NULL;
	memset(m_QuestString, 0, sizeof(m_QuestString));
	m_bQuestStringInputComplete			= FALSE;
	m_pMiniMapWindowPtr					= NULL; // 여장풀기에서 지도를 그리기 위해서 미니맵 윈도우의 포인트를 얻어온다.
	m_SavePointPositionX				= 0.0f;
	m_SavePointPositionY				= 0.0f;	
	m_imageSavePoint[0]					= NULL;
	m_imageSavePoint[1]					= NULL;
	m_btnMiniStopEventInput				= NULL; //미니스톱이벤트 입력버튼 
	
	m_imageDialogNextPageLeftBorder		= NULL;
	m_imageDialogNextPageRightBorder	= NULL;
	m_imageDialogNextPagePoint[0]		= NULL;
	m_imageDialogNextPagePoint[1]		= NULL;
	
	m_btnDialogNextPage					= NULL;	// NPC대사가 많을때 다음 페이지로 넘기는 버튼 

	m_PrevDrawTime						= 0;	
	m_EffectFrame						= 0.0f;	

	m_pDialogStringTotalBufferLength	= 0;
	m_pDialogStringTotalBuffer			= NULL;
	
	m_EventType							= -1;
	m_btnMiniStopEventInput				= NULL;
	
	memset(m_EventCodeString01, 0, sizeof(TCHAR)*6);
	memset(m_EventCodeString02, 0, sizeof(TCHAR)*6);
	memset(m_EventCodeString03, 0, sizeof(TCHAR)*6);
}

_XWindow_NPCScript::~_XWindow_NPCScript()
{
	DestroyWindow();
}

BOOL _XWindow_NPCScript::Initialize( void )
{
	int npcresourceindex1		= g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
	int npcresourceindex2		= g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_02.tga" );
	int npcresourceindex3		= g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_03.tga" );
	int npcresourceindex4		= g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_04.tga" );
	int questresourceindex2	 	= g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
	int mainbuttonindex		 	= g_MainInterfaceTextureArchive.FindResource( "MI_main_button.tga" );
	int	evnetcontentsindex01	= g_MainInterfaceTextureArchive.FindResource( "mi_npceventcontents01.tga" );
	int	evnetcontentsindex02	= g_MainInterfaceTextureArchive.FindResource( "mi_npceventcontents02.tga" );
	int	evnetcontentsindex03	= g_MainInterfaceTextureArchive.FindResource( "mi_npceventcontents03.tga" );
	int	evnetcontentsindex04	= g_MainInterfaceTextureArchive.FindResource( "mi_npceventcontents04.tga" );
	
	
	// NPC 대화창 바탕 이미지 
	m_NPCDialogBorder = new _XImageStatic;
	m_NPCDialogBorder->Create( 0, 0, 425, 410, &g_MainInterfaceTextureArchive, npcresourceindex1 );
	m_NPCDialogBorder->SetClipRect(0, 0, 425, 410);
	m_NPCDialogBorder->ShowWindow(TRUE);
	InsertChildObject( m_NPCDialogBorder );
	
	// NPC 대화창 위쪽 Sub 바탕 이미지 
	m_NPCDialogSubBorderA1 = new _XImageStatic;
	m_NPCDialogSubBorderA1->Create( 18, 30, 274, 105, &g_MainInterfaceTextureArchive, npcresourceindex3 );
	m_NPCDialogSubBorderA1->SetClipRect(0, 0, 256, 75);
	m_NPCDialogSubBorderA1->ShowWindow(TRUE);
	InsertChildObject( m_NPCDialogSubBorderA1 );
	
	m_NPCDialogSubBorderA2 = new _XImageStatic;
	m_NPCDialogSubBorderA2->Create( 274, 30, 407, 105, &g_MainInterfaceTextureArchive, npcresourceindex3 );
	m_NPCDialogSubBorderA2->SetClipRect(0, 75, 133, 150);
	m_NPCDialogSubBorderA2->ShowWindow(TRUE);
	InsertChildObject( m_NPCDialogSubBorderA2 );
	
	// NPC 대화창 아래쪽 Sub 바탕 이미지 
	m_NPCDialogSubBorderB1 = new _XImageStatic;
	m_NPCDialogSubBorderB1->Create( 11, 286, 267, 396, &g_MainInterfaceTextureArchive, npcresourceindex2 );
	m_NPCDialogSubBorderB1->SetClipRect(0, 0, 256, 110);
	m_NPCDialogSubBorderB1->ShowWindow(TRUE);
	InsertChildObject( m_NPCDialogSubBorderB1 );
	
	m_NPCDialogSubBorderB2 = new _XImageStatic;
	m_NPCDialogSubBorderB2->Create( 267, 286, 414, 396, &g_MainInterfaceTextureArchive, npcresourceindex2 );
	m_NPCDialogSubBorderB2->SetClipRect(0, 110, 147, 220);
	m_NPCDialogSubBorderB2->ShowWindow(TRUE);
	InsertChildObject( m_NPCDialogSubBorderB2 );
	
	// NPC 대화창 Title바 이미지
	m_NPCDialogMainTitleBorder = new _XImageStatic;
	m_NPCDialogMainTitleBorder->Create( 26, 25, 401, 50, &g_MainInterfaceTextureArchive, npcresourceindex1 );
	m_NPCDialogMainTitleBorder->SetClipRect(4, 461, 379, 486);
	m_NPCDialogMainTitleBorder->ShowWindow(TRUE);
	InsertChildObject( m_NPCDialogMainTitleBorder );
	
	// NPC 대화창 IME 바탕이미지 
	m_NPCDialogIMEBorder1 = new _XImageStatic;
	m_NPCDialogIMEBorder1->Create( 41, 335, 209, 363, &g_MainInterfaceTextureArchive, npcresourceindex3 );
	m_NPCDialogIMEBorder1->SetClipRect( 88, 154, 256, 182 );
	m_NPCDialogIMEBorder1->ShowWindow(FALSE);
	InsertChildObject( m_NPCDialogIMEBorder1 );
	
	m_NPCDialogIMEBorder2 = new _XImageStatic;
	m_NPCDialogIMEBorder2->Create( 377, 335, 545, 363, &g_MainInterfaceTextureArchive, npcresourceindex3 );
	m_NPCDialogIMEBorder2->SetClipRect( 88, 154, 256, 182 );
	m_NPCDialogIMEBorder2->SetScale( -1, 1 );
	m_NPCDialogIMEBorder2->ShowWindow(FALSE);
	InsertChildObject( m_NPCDialogIMEBorder2 );
	
	// 퀘스트 난이도 표시 이미지
	m_imageQuesLevelDisplay = new _XImageStatic;
	// 해외 버전일 때 
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) // US, Vietnamese version은 퀘스트 타입이 길어 보상 글 제외.
	{
		m_imageQuesLevelDisplay->Create( 55-20, 61-10, 68-20, 77-10, &g_MainInterfaceTextureArchive, questresourceindex2 );
	}
	else//국내버전일 때 
	{
		m_imageQuesLevelDisplay->Create( 55, 61, 68, 77, &g_MainInterfaceTextureArchive, questresourceindex2 );
	}
	m_imageQuesLevelDisplay->SetClipRect(238, 81, 251, 97);//노란색 두루마기 이미지
	m_imageQuesLevelDisplay->ShowWindow(FALSE);
	InsertChildObject( m_imageQuesLevelDisplay );
	
	// 현재 위치를 나타내는 이미지
	m_imageSavePoint[0] = new _XImageStatic;
	m_imageSavePoint[0]->Create( 0, 0, 21, 21, &g_MainInterfaceTextureArchive, mainbuttonindex );
	m_imageSavePoint[0]->SetClipRect(148, 214, 169, 235);

	m_imageSavePoint[1] = new _XImageStatic;
	m_imageSavePoint[1]->Create( 0, 0, 21, 21, &g_MainInterfaceTextureArchive, mainbuttonindex );
	m_imageSavePoint[1]->SetClipRect(170, 214, 191, 235);
	
	// NPC 대화창 버튼 생성
	_XBTN_STRUCTURE scriptbtnstruct = { TRUE, {18, 365}, {91, 27}, _XDEF_NPCSCRIPT_SUBSCRIPT1, 
		npcresourceindex1, npcresourceindex1, npcresourceindex1, &g_MainInterfaceTextureArchive };
	
	for(int i = 0 ; i < 4 ; i++)
	{
		m_ScriptButton[i] = new _XButton;
		m_ScriptButton[i]->Create(scriptbtnstruct);
		m_ScriptButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
		m_ScriptButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
		m_ScriptButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
		m_ScriptButton[i]->ShowWindow(FALSE);
		InsertChildObject(m_ScriptButton[i]);
		
		scriptbtnstruct.position.x += 99;
		scriptbtnstruct.commandid++;
	}
	
	// 해외 버전일 때 
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		// 퀘스트 창에서 선택시 획득할 보상물 표시에 사용될 리스트 박스
		_XLISTBOX_STRUCTURE rewardstructure = 
		{
			TRUE,											// 윈도우 활성화
				
			//{ 172, 56+10 },									// 윈도우 좌표
			{ 37, 56+10 },									// 윈도우 좌표
			{ 297, 36 },									// 윈도우 사이즈
			_XDEF_NPCSCRIPT_LISTBOXQUESTREWARD,				// 윈도우 command 아이디
			-1,												// 윈도우 border image
			&g_MainInterfaceTextureArchive,					// Texture list
			_T(" "),										// 윈도우 타이틀
			36,												// 리스트 아이템 라인 피치
			6,												// 리스트 박스에 보이는 최대 라인수
			_XLSTYLE_NOSCROLLBAR							// 스크롤바의 위치
			
		};
		m_listboxQuestReward = new _XListBox;
		m_listboxQuestReward->SetIconReportMode( TRUE );
		m_listboxQuestReward->SetIconReportModeInfo( 1, 9, 36, 36 );
		m_listboxQuestReward->Create(rewardstructure);	
		
		POINT rewardcolumnoffset1 = { 1, 1 };
		POINT rewardcolumnoffset2 = { 0, 0 };
		m_listboxQuestReward->SetColumnOffset(0, rewardcolumnoffset1);				// column offset
		m_listboxQuestReward->SetColumnOffset(1, rewardcolumnoffset2);				// column offset	
		
		m_listboxQuestReward->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
		m_listboxQuestReward->SetColumnStyle(1, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
		
		m_listboxQuestReward->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
		m_listboxQuestReward->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);		// column align type	
		
		m_listboxQuestReward->SetColumnWidth(0, 256);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
		m_listboxQuestReward->SetColumnWidth(1, 256);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
		
		m_listboxQuestReward->EnableSelectItem(FALSE);
		m_listboxQuestReward->ShowWindow(FALSE);
		InsertChildObject(m_listboxQuestReward);
		
		// 영웅회 이름 IME 바탕이미지 
		m_OrganizationNameIMEBorder1 = new _XImageStatic;
		m_OrganizationNameIMEBorder1->Create( 180, 317, 184, 375, &g_MainInterfaceTextureArchive, npcresourceindex3 );
		m_OrganizationNameIMEBorder1->SetClipRect( 88, 154, 122, 182 );
		m_OrganizationNameIMEBorder1->ShowWindow(FALSE);
		InsertChildObject( m_OrganizationNameIMEBorder1 );
		
		m_OrganizationNameIMEBorder2 = new _XImageStatic;
		m_OrganizationNameIMEBorder2->Create( 248, 317, 282, 345, &g_MainInterfaceTextureArchive, npcresourceindex3 );
		m_OrganizationNameIMEBorder2->SetClipRect( 88, 154, 122, 182 );
		m_OrganizationNameIMEBorder2->SetScale( -1, 1 );
		m_OrganizationNameIMEBorder2->ShowWindow(FALSE);
		InsertChildObject( m_OrganizationNameIMEBorder2 );
		
		// 영웅회 이름 입력창
		m_OrganizationNameIME.ResetContainer();
		m_OrganizationNameIME.MoveWindow(m_WindowPosition.x+138, m_WindowPosition.y+325);
		m_OrganizationNameIME.ResizeWindow(150, 17);
		m_OrganizationNameIME.SetMaxLength(_XDEF_ORGANIZATIONNAMELENGTH);
		m_OrganizationNameIME.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
		m_OrganizationNameIME.SetTextColor(_XSC_DEFAULT_BLACK);
		m_OrganizationNameIME.ShowWindowRectAngle(TRUE);
		m_OrganizationNameIME.ShowWindow(FALSE);

	}
	else//국내버전일 때 
	{
		// 퀘스트 창에서 선택시 획득할 보상물 표시에 사용될 리스트 박스
		_XLISTBOX_STRUCTURE rewardstructure = 
		{
			TRUE,											// 윈도우 활성화
			{ 152, 56 },									// 윈도우 좌표
			{ 297, 36 },									// 윈도우 사이즈
			_XDEF_NPCSCRIPT_LISTBOXQUESTREWARD,				// 윈도우 command 아이디
			-1,												// 윈도우 border image
			&g_MainInterfaceTextureArchive,					// Texture list
			_T(" "),										// 윈도우 타이틀
			36,												// 리스트 아이템 라인 피치
			6,												// 리스트 박스에 보이는 최대 라인수
			_XLSTYLE_NOSCROLLBAR							// 스크롤바의 위치
			
		};
		m_listboxQuestReward = new _XListBox;
		m_listboxQuestReward->SetIconReportMode( TRUE );
		m_listboxQuestReward->SetIconReportModeInfo( 1, 9, 31, 31 );
		m_listboxQuestReward->Create(rewardstructure);	
		
		POINT rewardcolumnoffset1 = { 1, 1 };
		POINT rewardcolumnoffset2 = { 0, 0 };
		m_listboxQuestReward->SetColumnOffset(0, rewardcolumnoffset1);				// column offset
		m_listboxQuestReward->SetColumnOffset(1, rewardcolumnoffset2);				// column offset	
		
		m_listboxQuestReward->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
		m_listboxQuestReward->SetColumnStyle(1, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
		
		m_listboxQuestReward->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
		m_listboxQuestReward->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);		// column align type	
		
		m_listboxQuestReward->SetColumnWidth(0, 256);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
		m_listboxQuestReward->SetColumnWidth(1, 256);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
		
		m_listboxQuestReward->EnableSelectItem(FALSE);
		m_listboxQuestReward->ShowWindow(FALSE);
		InsertChildObject(m_listboxQuestReward);

		
		// 영웅회 이름 IME 바탕이미지 
		m_OrganizationNameIMEBorder1 = new _XImageStatic;
		m_OrganizationNameIMEBorder1->Create( 180, 317, 184, 375, &g_MainInterfaceTextureArchive, npcresourceindex3 );
		m_OrganizationNameIMEBorder1->SetClipRect( 88, 154, 122, 182 );
		m_OrganizationNameIMEBorder1->ShowWindow(FALSE);
		InsertChildObject( m_OrganizationNameIMEBorder1 );
		
		m_OrganizationNameIMEBorder2 = new _XImageStatic;
		m_OrganizationNameIMEBorder2->Create( 248, 317, 282, 345, &g_MainInterfaceTextureArchive, npcresourceindex3 );
		m_OrganizationNameIMEBorder2->SetClipRect( 88, 154, 122, 182 );
		m_OrganizationNameIMEBorder2->SetScale( -1, 1 );
		m_OrganizationNameIMEBorder2->ShowWindow(FALSE);
		InsertChildObject( m_OrganizationNameIMEBorder2 );
		
		// 영웅회 이름 입력창
		m_OrganizationNameIME.ResetContainer();
		m_OrganizationNameIME.MoveWindow(m_WindowPosition.x+190, m_WindowPosition.y+325);
		m_OrganizationNameIME.ResizeWindow(90, 20);
		m_OrganizationNameIME.SetMaxLength(8);
		m_OrganizationNameIME.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
		m_OrganizationNameIME.SetTextColor(_XSC_DEFAULT_BLACK);
		m_OrganizationNameIME.ShowWindow(FALSE);

	}
	
	int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	
	// NPC 대화를 표시할 때 사용될 리스트 박스
	_XLISTBOX_STRUCTURE npcdialogstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 55, 122 },		// 윈도우 좌표
		{ 338, 162 },	// 윈도우 사이즈
		_XDEF_NPCSCRIPT_MESSAGELIST,					// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		18,												// 리스트 아이템 라인 피치
		9,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 162 },									// 스크롤바 사이즈
		12,												// 스크롤바 상하 버튼 사이즈
		22,												// 스크롤바 트랙바 사이즈
		150,											// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
	};

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		npcdialogstructure.windowsize.cx = 358;
		npcdialogstructure.position.x = 35;
	}

	m_listboxNPCDialogMessage = new _XListBox;
	m_listboxNPCDialogMessage->Create(npcdialogstructure);
		
	_XVScrollBar* pScrollBar = m_listboxNPCDialogMessage->GetScrollBarObject();
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
		pScrollBar->SetViewNullBorder(FALSE);
	}
	
	POINT columnoffset1 = { 0, 0 };
	m_listboxNPCDialogMessage->SetColumnOffset(0, columnoffset1);			// column offset
	m_listboxNPCDialogMessage->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);	// column style (아이콘)
	m_listboxNPCDialogMessage->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxNPCDialogMessage->SetColumnWidth(0, 338);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	
	m_listboxNPCDialogMessage->EnableSelectItem(FALSE);
	m_listboxNPCDialogMessage->ShowWindow(TRUE);
	InsertChildObject(m_listboxNPCDialogMessage);
	
	//퀘스트에서 선택지를 표시할 리스트 박스
//	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
//	{
//		_XLISTBOX_STRUCTURE selectionlistboxstructure = 
//		{
//			TRUE,											// 윈도우 활성화
//			{ 0, 304 },										// 윈도우 좌표
//			{ 362, 90 },									// 윈도우 사이즈
//			_XDEF_NPCSCRIPT_LISTBOXSELECTION,				// 윈도우 command 아이디
//			-1,												// 윈도우 border image
//			&g_MainInterfaceTextureArchive,					// Texture list
//			_T(""),											// 윈도우 타이틀
//			17,												// 리스트 아이템 라인 피치
//			5,												// 리스트 박스에 보이는 최대 라인수
//			_XLSTYLE_NOSCROLLBAR							// 스크롤바의 위치
//		};
//	}
//	else
//	{
		_XLISTBOX_STRUCTURE selectionlistboxstructure = 
		{
			TRUE,											// 윈도우 활성화
			{ 22, 304 },									// 윈도우 좌표
			{ 400, 90 },									// 윈도우 사이즈
			_XDEF_NPCSCRIPT_LISTBOXSELECTION,				// 윈도우 command 아이디
			-1,												// 윈도우 border image
			&g_MainInterfaceTextureArchive,					// Texture list
			_T(""),											// 윈도우 타이틀
			17,												// 리스트 아이템 라인 피치
			5,												// 리스트 박스에 보이는 최대 라인수
			_XLSTYLE_NOSCROLLBAR							// 스크롤바의 위치
		};
//	}
	
	m_listboxSelectionItem = new _XListBox;
	m_listboxSelectionItem->Create(selectionlistboxstructure);
	
	
	POINT selectioncoloffset1 = { 0, 3 };
	POINT selectioncoloffset2 = { 18, 4 };
	POINT selectioncoloffset3 = { 27, 8 };
	POINT selectioncoloffset4 = { 24, 5 };
	
	m_listboxSelectionItem->SetColumnOffset(0, selectioncoloffset1);	// column offset	
	m_listboxSelectionItem->SetColumnOffset(1, selectioncoloffset2);	// column offset	
	m_listboxSelectionItem->SetColumnOffset(2, selectioncoloffset3);	// column offset	
	m_listboxSelectionItem->SetColumnOffset(3, selectioncoloffset4);	// column offset	
	
	m_listboxSelectionItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);	// column style 	
	m_listboxSelectionItem->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);	// column style 	
	m_listboxSelectionItem->SetColumnStyle(2, _XLCOLUMNSTYLE_ICONRECT);	// column style 	
	m_listboxSelectionItem->SetColumnStyle(3, _XLCOLUMNSTYLE_ICONRECT);	// column style 

	m_listboxSelectionItem->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type	
	m_listboxSelectionItem->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxSelectionItem->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxSelectionItem->SetAlignType(3, _XFONT_ALIGNTYPE_LEFT);		// column align type
	
	m_listboxSelectionItem->SetColumnWidth(0, 330);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxSelectionItem->SetColumnWidth(1, 400);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxSelectionItem->SetColumnWidth(2, 170);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxSelectionItem->SetColumnWidth(3, 170);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	
	m_listboxSelectionItem->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
	m_listboxSelectionItem->SetSelectedImagePosition(0, 1);
	m_listboxSelectionItem->SetSelectedImageHeight(17);
	m_listboxSelectionItem->SetTrackWidth(400);
	
	m_listboxSelectionItem->ShowWindow(FALSE);	
	InsertChildObject(m_listboxSelectionItem);
	

	// 가지고 있는 퀘스트 리스트를 표시할 리스트 박스
	_XLISTBOX_STRUCTURE queststructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 40, 120 },									// 윈도우 좌표
		{ 333, 162 },									// 윈도우 사이즈
		_XDEF_NPCSCRIPT_LISTBOXHAVEQUESTLIST,			// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		18,												// 리스트 아이템 라인 피치
		9,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 162 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		150,												// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
	};
		
	m_listboxHaveQuestList = new _XListBox;
	m_listboxHaveQuestList->Create(queststructure);
	
	pScrollBar = m_listboxHaveQuestList->GetScrollBarObject();
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
		pScrollBar->SetViewNullBorder(FALSE);
	}
	
	POINT columnoffset11 = { 3, 3 };
	POINT columnoffset2 = { 20, 6 };
	POINT columnoffset3 = { 70, 6 };

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		columnoffset3.x = 140;
	}

	m_listboxHaveQuestList->SetColumnOffset(0, columnoffset11);				// column offset
	m_listboxHaveQuestList->SetColumnOffset(1, columnoffset2);
	m_listboxHaveQuestList->SetColumnOffset(2, columnoffset3);
	
	m_listboxHaveQuestList->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_listboxHaveQuestList->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_listboxHaveQuestList->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	
	m_listboxHaveQuestList->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxHaveQuestList->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_listboxHaveQuestList->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxHaveQuestList->SetColumnWidth(0, 330);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxHaveQuestList->SetColumnWidth(1, 330);
	m_listboxHaveQuestList->SetColumnWidth(2, 330);
	
	m_listboxHaveQuestList->SetSelectedImageArchive(npcresourceindex1, &g_MainInterfaceTextureArchive);	
	_XImageStatic* selectbar = m_listboxHaveQuestList->GetSelectBarObject();
	selectbar->SetClipRect( 196, 490, 507, 506 );
	m_listboxHaveQuestList->SetSelectedImagePosition(5, 3);
	m_listboxHaveQuestList->SetSelectedImageHeight(18);
	m_listboxHaveQuestList->SetTrackWidth(322);
	
	m_listboxHaveQuestList->SetAlwaysShowScrollBarMode(FALSE);
	m_listboxHaveQuestList->ShowWindow(FALSE);
	InsertChildObject(m_listboxHaveQuestList);
	
	// 퀘스트 텍스트 확인 입력창 
	m_QuestIMEControl.ResetContainer();
	m_QuestIMEControl.MoveWindow(m_WindowPosition.x + 50, m_WindowPosition.y + 345);
	m_QuestIMEControl.ResizeWindow(340, 20);
	m_QuestIMEControl.SetMaxLength(128);
	m_QuestIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
	m_QuestIMEControl.SetTextColor(_XSC_DEFAULT_BLACK);
	m_QuestIMEControl.ShowWindow(FALSE);	
	m_QuestIMEControl.SetReturnKeyCallback( _XIMECallback_NPCDialogModeQuestInputReturn );
	
	// 퀘스트 관련 확인 버튼
	_XBTN_STRUCTURE okbtnstruct = 
	{
		TRUE, {315, 365}, {91, 27}, _XDEF_NPCSCRIPT_CLOSE,
			npcresourceindex1, npcresourceindex1, npcresourceindex1,
			&g_MainInterfaceTextureArchive
	};
	m_btnQuestOK = new _XButton;
	m_btnQuestOK->Create(okbtnstruct);
	m_btnQuestOK->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
	m_btnQuestOK->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
	m_btnQuestOK->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
	m_btnQuestOK->SetButtonTextColor( _XSC_DEFAULT );
	m_btnQuestOK->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_OK );		// _T("확인")
	m_btnQuestOK->ShowWindow(FALSE);
	InsertChildObject(m_btnQuestOK);
	
	// 퀘스트 대화 끝난 후에  NPC 기능으로 돌아가는 버튼
	_XBTN_STRUCTURE backtonpcmodebtnstruct = 
	{
		TRUE, {220, 365}, {91, 27}, _XDEF_NPCSCRIPT_BACKTONPCMODE,
			npcresourceindex1, npcresourceindex1, npcresourceindex1,
			&g_MainInterfaceTextureArchive
	};
	m_btnBackToNPCMode = new _XButton;
	m_btnBackToNPCMode->Create(backtonpcmodebtnstruct);
	m_btnBackToNPCMode->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
	m_btnBackToNPCMode->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
	m_btnBackToNPCMode->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
	m_btnBackToNPCMode->SetButtonTextColor( _XSC_DEFAULT );
	m_btnBackToNPCMode->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_CONTINUE);	// _T("계속하기")
	m_btnBackToNPCMode->ShowWindow(FALSE);
	InsertChildObject(m_btnBackToNPCMode);
	
	_XBTN_STRUCTURE npcexitbtnstruct = 
	{					
		TRUE, {331, 584}, {80, 26}, _XDEF_NPCSCRIPT_CLOSE,
			questresourceindex2, questresourceindex2, questresourceindex2,
			&g_MainInterfaceTextureArchive
	};	
//	_XBTN_STRUCTURE npcexitbtnstruct = 
//	{					
//		TRUE, {gnWidth-(1024-331), gnHeight-(768-584)}, {80, 26}, _XDEF_NPCSCRIPT_CLOSE,
//			questresourceindex2, questresourceindex2, questresourceindex2,
//			&g_MainInterfaceTextureArchive
//	};
	
	
	m_btnNPCDialogModExit = new _XButton;
	m_btnNPCDialogModExit->Create(npcexitbtnstruct);
	m_btnNPCDialogModExit->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL,80, 190, 160, 216);
	m_btnNPCDialogModExit->SetButtonImageClipRect(_XBUTTONIMAGE_READY,80, 190, 160, 216);
	m_btnNPCDialogModExit->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 190, 80, 216);
	m_btnNPCDialogModExit->SetButtonTextColor( _XSC_DEFAULT_HIGHLIGHT );
	m_btnNPCDialogModExit->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_ENDTALK);	// _T("대화마침")
	m_btnNPCDialogModExit->ShowWindow(TRUE);
	InsertChildObject(m_btnNPCDialogModExit);	
	m_btnNPCDialogModExit->MoveWindow(gnWidth-(100), gnHeight-(46) );
	
	// 영웅단 이름 입력 버튼, 취소 버튼 ----------------------------------------------------------------------=	
	_XBTN_STRUCTURE nameinputbtnstruct = { TRUE, {117, 365}, {91, 27}, _XDEF_GROUP_ORGANIZATION_NAMECOMPLETE, 
										npcresourceindex1, npcresourceindex1, npcresourceindex1, &g_MainInterfaceTextureArchive };

	m_CompleteNameInput = new _XButton;
	m_CompleteNameInput->Create(nameinputbtnstruct);
	m_CompleteNameInput->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
	m_CompleteNameInput->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
	m_CompleteNameInput->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
	m_CompleteNameInput->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_INPUTCOMPLETE);	// _T("입력 완료")
	m_CompleteNameInput->SetButtonTextColor( _XSC_DEFAULT_HIGHLIGHT );
	m_CompleteNameInput->ShowWindow(FALSE);
	InsertChildObject(m_CompleteNameInput);

	nameinputbtnstruct.position.x+=99;
	nameinputbtnstruct.commandid++;

	m_NampInputCancle = new _XButton;
	m_NampInputCancle->Create(nameinputbtnstruct);
	m_NampInputCancle->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
	m_NampInputCancle->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
	m_NampInputCancle->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
	m_NampInputCancle->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_CANCLE);	// _T("취 소")
	m_NampInputCancle->SetButtonTextColor( _XSC_DEFAULT_HIGHLIGHT );
	m_NampInputCancle->ShowWindow(FALSE);
	InsertChildObject(m_NampInputCancle);	
	// -------------------------------------------------------------------------------------------------------=
		
	// 이벤트 내용 이미지 ( 256 X 256 )
	m_imageEventContent01 = new _XImageStatic;
	m_imageEventContent01->Create( 46, 51, 46+256, 51+256, &g_MainInterfaceTextureArchive, evnetcontentsindex01 );
	m_imageEventContent01->SetClipRect( 0, 0, 256, 256 );
	m_imageEventContent01->ShowWindow(FALSE);
	InsertChildObject( m_imageEventContent01 );
	
	// 이벤트 내용 이미지 ( 256 X 256 )
	m_imageEventContent02 = new _XImageStatic;
	m_imageEventContent02->Create( 46, 51, 46+256, 51+256, &g_MainInterfaceTextureArchive, evnetcontentsindex02 );
	m_imageEventContent02->SetClipRect( 0, 0, 256, 256 );
	m_imageEventContent02->ShowWindow(FALSE);
	InsertChildObject( m_imageEventContent02 );
	
	// 이벤트 내용 이미지 ( 256 X 256 )
	m_imageEventContent03 = new _XImageStatic;
	m_imageEventContent03->Create( 46, 65, 46+256, 65+256, &g_MainInterfaceTextureArchive, evnetcontentsindex03 );
	m_imageEventContent03->SetClipRect( 0, 0, 256, 256 );
	m_imageEventContent03->ShowWindow(FALSE);
	InsertChildObject( m_imageEventContent03 );
	
	// 이벤트 내용 이미지 ( 256 X 256 )
	m_imageEventContent04 = new _XImageStatic;
	m_imageEventContent04->Create( 46, 65, 46+256, 65+256, &g_MainInterfaceTextureArchive, evnetcontentsindex04 );
	m_imageEventContent04->SetClipRect( 0, 0, 256, 256 );
	m_imageEventContent04->ShowWindow(FALSE);
	InsertChildObject( m_imageEventContent04 );
	
	// 미니스톱이벤트 IME 바탕이미지 
	m_MiniStopEventIMEBorder1 = new _XImageStatic;
	m_MiniStopEventIMEBorder1->Create( 41, 335, 141, 363, &g_MainInterfaceTextureArchive, npcresourceindex3 );
	m_MiniStopEventIMEBorder1->SetClipRect( 88, 154, 180, 182 );
	m_MiniStopEventIMEBorder1->ShowWindow(FALSE);
	InsertChildObject( m_MiniStopEventIMEBorder1 );
		
	m_MiniStopEventIMEBorder2 = new _XImageStatic;
	m_MiniStopEventIMEBorder2->Create(  225, 335, 309, 363, &g_MainInterfaceTextureArchive, npcresourceindex3 );
	m_MiniStopEventIMEBorder2->SetClipRect( 88, 154, 180, 182 );
	m_MiniStopEventIMEBorder2->SetScale( -1, 1 );
	m_MiniStopEventIMEBorder2->ShowWindow(FALSE);
	InsertChildObject( m_MiniStopEventIMEBorder2 );
	
	
	// 이벤트 IME 바탕이미지 
	m_EventIMEBorder1 = new _XImageStatic;
	m_EventIMEBorder1->Create( 41, 335, 63, 363, &g_MainInterfaceTextureArchive, npcresourceindex3 );
	m_EventIMEBorder1->SetClipRect( 88, 154, 110, 182 );
	m_EventIMEBorder1->ShowWindow(FALSE);
	
	m_EventIMEBorder2 = new _XImageStatic;
	m_EventIMEBorder2->Create(  85, 335, 107, 363, &g_MainInterfaceTextureArchive, npcresourceindex3 );
	m_EventIMEBorder2->SetClipRect( 88, 154, 110, 182 );
	m_EventIMEBorder2->SetScale( -1, 1 );
	m_EventIMEBorder2->ShowWindow(FALSE);
	
	int messengerresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	_XBTN_STRUCTURE ministopeventbtnstruct = 
	{
		TRUE, {244, 337}, {60, 24}, _XDEF_NPCSCRIPT_MINISTOPOK,
		messengerresourceindex1, messengerresourceindex1, messengerresourceindex1,
		&g_MainInterfaceTextureArchive
	};
	m_btnMiniStopEventInput = new _XButton;
	m_btnMiniStopEventInput->Create(ministopeventbtnstruct);
	m_btnMiniStopEventInput->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_btnMiniStopEventInput->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_btnMiniStopEventInput->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	m_btnMiniStopEventInput->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_INPUT );
	m_btnMiniStopEventInput->SetButtonTextColor( _XSC_DEFAULT_HIGHLIGHT );
	m_btnMiniStopEventInput->ShowWindow(FALSE);
	InsertChildObject(m_btnMiniStopEventInput);	
	
	// 미니스톱이벤트 입력창	
	m_MiniStopEventIMEControl.ResetContainer();
	m_MiniStopEventIMEControl.MoveWindow(m_WindowPosition.x+50, m_WindowPosition.y + 345);
	m_MiniStopEventIMEControl.ResizeWindow(194, 20);
	m_MiniStopEventIMEControl.SetMaxLength(64);
	m_MiniStopEventIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
	m_MiniStopEventIMEControl.SetTextColor(_XSC_DEFAULT_BLACK);
	m_MiniStopEventIMEControl.SetReturnKeyCallback( _XIMECallback_EventCodeInputReturn );
	m_MiniStopEventIMEControl.SetTabKeyCallback( _XIMECallback_EventCodeInputTab );
	m_MiniStopEventIMEControl.ShowWindow(FALSE);	
	
	// -------------------------------------------------------------------------------------------------------=
	// NPC Dialog Next Page 관련 

	// NPC Dialog Next Page 버튼 왼쪽 이미지 (30 X 32)
	m_imageDialogNextPageLeftBorder = new _XImageStatic;
	m_imageDialogNextPageLeftBorder->Create( 59, 297, 89, 329, &g_MainInterfaceTextureArchive, npcresourceindex4 );
	m_imageDialogNextPageLeftBorder->SetClipRect( 0, 99, 30, 131 );
	m_imageDialogNextPageLeftBorder->ShowWindow(FALSE);
	InsertChildObject( m_imageDialogNextPageLeftBorder );

	// NPC Dialog Next Page 버튼 오른쪽 이미지 (30 X 32)
	m_imageDialogNextPageRightBorder = new _XImageStatic;
	m_imageDialogNextPageRightBorder->Create( 339, 297, 369, 329, &g_MainInterfaceTextureArchive, npcresourceindex4 );
	m_imageDialogNextPageRightBorder->SetClipRect( 31, 99, 61, 131 );
	m_imageDialogNextPageRightBorder->ShowWindow(FALSE);
	InsertChildObject( m_imageDialogNextPageRightBorder );
	
	// NPC Dialog Next Page Point (22 X 19)
	m_imageDialogNextPagePoint[0] = new _XImageStatic;
	m_imageDialogNextPagePoint[0]->Create( 0, 0, 22, 19, &g_MainInterfaceTextureArchive, npcresourceindex4 );
	m_imageDialogNextPagePoint[0]->SetClipRect( 67, 107, 89, 126);
	
	m_imageDialogNextPagePoint[1] = new _XImageStatic;
	m_imageDialogNextPagePoint[1]->Create( 0, 0, 22, 19, &g_MainInterfaceTextureArchive, npcresourceindex4 );
	m_imageDialogNextPagePoint[1]->SetClipRect( 91, 107, 113, 126);
	
	// NPC Dialog Next Page Button (250 X 32)
	_XBTN_STRUCTURE nextpagebtnstruct = 
	{
		TRUE, {89, 297}, {250, 32}, _XDEF_NPCSCRIPT_NPCDIALOGNEXTPAGE,
			npcresourceindex4, npcresourceindex4, npcresourceindex4,
			&g_MainInterfaceTextureArchive
	};
	m_btnDialogNextPage = new _XButton;
	m_btnDialogNextPage->Create(nextpagebtnstruct);
	m_btnDialogNextPage->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 0, 250, 32);
	m_btnDialogNextPage->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 33, 250, 65);
	m_btnDialogNextPage->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 66, 250, 98);
	m_btnDialogNextPage->ShowWindow(FALSE);
	InsertChildObject(m_btnDialogNextPage);	


	m_pDialogStringTotalBuffer			= new TCHAR[2048];
	if( !m_pDialogStringTotalBuffer )
	{
		m_pDialogStringTotalBufferLength	= 0;
		return FALSE;
	}
	else m_pDialogStringTotalBufferLength	= 2048;

	m_bFirstDialog = TRUE; // 2004.04.28->oneway48 insert

	return TRUE;
}

void _XWindow_NPCScript::DestroyWindow( void )
{
	_XWindow::DestroyWindow();

	list <S_SUBSCRIPTITEM>::iterator iter;
	m_SubScriptList.clear();

	m_pDialogStringTotalBufferLength	= 0;
	SAFE_DELETE_ARRAY( m_pDialogStringTotalBuffer );
	SAFE_DELETE( m_EventIMEBorder1 );
	SAFE_DELETE( m_EventIMEBorder2 );
	
	SAFE_DELETE( m_imageSavePoint[0] );
	SAFE_DELETE( m_imageSavePoint[1] );

	SAFE_DELETE( m_imageDialogNextPagePoint[0] );
	SAFE_DELETE( m_imageDialogNextPagePoint[1] );
}

#define _XDEF_NPCCHATMESSAGEBLINKTIME		10000
#define _XDEF_NPCCHATAUTOCLODETIMER			5000

void _XWindow_NPCScript::Draw( _XGUIObject*& pfocusobject )
{					
	_XWindow::Draw(pfocusobject);

	if( !this->m_ShowWindow || ( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) ) return;

	switch(m_MessageType) {
	case _XDEF_NPCMESSAGE_MINISTOPEVENT:
	case _XDEF_NPCMESSAGE_TRACING:
	case _XDEF_NPCMESSAGE_NORMAL:
	case _XDEF_NPCMESSAGE_MASTERNAME  : //Author : 양희왕 //breif : 사제 
		{
			if( m_pTitleString[0] != 0 )
			{
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
				g_XBaseFont->Puts( m_WindowPosition.x + 37, m_WindowPosition.y + 31, m_pTitleString );	
				g_XBaseFont->Flush();
				g_XBaseFont->SetBoldMode( FALSE );	
			}

			if( m_EventType == 2 || m_EventType == 3) // 예약 가입 이벤트 , 상용화 이벤트 
			{
				if( m_EventIMEBorder1->GetShowStatus() )
				{
					m_EventIMEBorder1->Draw(m_WindowPosition.x +41, m_WindowPosition.y +335);
					m_EventIMEBorder2->Draw(m_WindowPosition.x +41+44, m_WindowPosition.y +335);
					
					m_EventIMEBorder1->Draw(m_WindowPosition.x +91, m_WindowPosition.y +335);
					m_EventIMEBorder2->Draw(m_WindowPosition.x +91+44, m_WindowPosition.y +335);
					
					m_EventIMEBorder1->Draw(m_WindowPosition.x +141, m_WindowPosition.y +335);
					m_EventIMEBorder2->Draw(m_WindowPosition.x +141+44, m_WindowPosition.y +335);
					
					g_XBaseFont->SetColor( _XSC_DEFAULT_BLACK );
					if( m_EventCodeString01[0] != 0 )
					{
						g_XBaseFont->Puts( m_WindowPosition.x+47, m_WindowPosition.y + 345, m_EventCodeString01 );	
					}
					
					if( m_EventCodeString02[0] != 0  )
					{
						g_XBaseFont->Puts( m_WindowPosition.x+97, m_WindowPosition.y + 345, m_EventCodeString02 );	
					}
					
					if( m_EventCodeString03[0] != 0  )
					{
						g_XBaseFont->Puts( m_WindowPosition.x+147, m_WindowPosition.y + 345, m_EventCodeString03 );	
					}
				}
			}

			if( m_imageDialogNextPageLeftBorder->GetShowStatus() )
			{
				if(m_PrevDrawTime == 0)
					m_PrevDrawTime = g_LocalSystemTime;
				
				DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
				
				m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
				
				if(m_EffectFrame >= 2.0f)
				{
					m_EffectFrame = 0.0f;
				}				
				
				int selectedframe = (int)m_EffectFrame;
				
				if( selectedframe == 0 || selectedframe == 1 )
					m_imageDialogNextPagePoint[selectedframe]->Draw( m_WindowPosition.x+200, m_WindowPosition.y + 306 );					
					//m_imageDialogNextPagePoint[selectedframe]->Draw( 784, 451 );
				m_PrevDrawTime = g_LocalSystemTime;
			}
		}
		break;
	case _XDEF_NPCMESSAGE_SAVEPOINT:
		{
			if( m_pTitleString[0] != 0 )
			{
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
				g_XBaseFont->Puts( m_WindowPosition.x + 37, m_WindowPosition.y + 31, m_pTitleString );		
				g_XBaseFont->Flush();
				g_XBaseFont->SetBoldMode( FALSE );	
			}
			if( m_pMiniMapWindowPtr )
			{
				m_pMiniMapWindowPtr->m_LargeMinimapImage.Draw( 645, 200, m_pMiniMapWindowPtr->m_pMinimapTexture );
				
				if(m_PrevDrawTime == 0)
					m_PrevDrawTime = g_LocalSystemTime;
				
				DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
				
				m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
				
				if(m_EffectFrame >= 2.0f)
				{
					m_EffectFrame = 0.0f;
				}				
				
				int selectedframe = (int)m_EffectFrame;
				
				if( selectedframe == 0 || selectedframe == 1 )
					m_imageSavePoint[selectedframe]->Draw( m_SavePointPositionX-10, m_SavePointPositionY-10 );
				m_PrevDrawTime = g_LocalSystemTime;
			}
		}
		break;
	case _XDEF_NPCMESSAGE_QUEST:
		{
			if( m_HaveQuestName[0] != 0 )
			{
				switch( m_QuestType ) {
				case _XQUEST_TYPE_1://제룡행 
					{
						g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 
					}
					break;
				case _XQUEST_TYPE_2://무림행 
					{
						g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 22, 146, 252));
					}
					break;
				case _XQUEST_TYPE_3://협객행
					{
						g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 245, 116, 34)); 
					}
					break;
				case _XQUEST_TYPE_4://기연행 
					{
						g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 78, 229, 48));
					}
					break;
				case _XQUEST_TYPE_5://무림소사
					{
						g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
					}
					break;
				}		
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->Puts( m_WindowPosition.x + 37, m_WindowPosition.y + 31, m_HaveQuestName );		
				g_XBaseFont->Flush();
				g_XBaseFont->SetBoldMode( FALSE );	
			}

			// 해외 버젼일 때 
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )  // US, Vietnamese version은 퀘스트 타입이 길어 보상 글 제외.
			{
				if( m_HaveQuestType[0] != 0 )
				{
					g_XBaseFont->SetBoldMode( TRUE );
					g_XBaseFont->SetColor( _XSC_DEFAULT_BLACK );
					g_XBaseFont->Puts( m_WindowPosition.x + 74-20, m_WindowPosition.y + 64-10, m_HaveQuestType );		
					g_XBaseFont->Flush();
					g_XBaseFont->SetBoldMode( FALSE );	
				}

			}
			else // 국내 버젼일때 
			{
				if( m_HaveQuestType[0] != 0 )
				{
					g_XBaseFont->SetBoldMode( TRUE );
					g_XBaseFont->SetColor( _XSC_DEFAULT_BLACK );
					g_XBaseFont->Puts( m_WindowPosition.x + 74, m_WindowPosition.y + 64, m_HaveQuestType );		
					g_XBaseFont->Flush();
					g_XBaseFont->SetBoldMode( FALSE );	
				}

				if( m_listboxQuestReward->GetItemCount() != 0 )
				{
					g_XBaseFont->SetBoldMode( TRUE );
					g_XBaseFont->SetColor( _XSC_DEFAULT_BLACK );
					g_XBaseFont->Puts( m_WindowPosition.x + 122, m_WindowPosition.y + 64, _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ITEM) );	// _T("보상")
					g_XBaseFont->Flush();
					g_XBaseFont->SetBoldMode( FALSE );	
				}
			}

			if( m_imageDialogNextPageLeftBorder->GetShowStatus() )
			{
				if(m_PrevDrawTime == 0)
					m_PrevDrawTime = g_LocalSystemTime;
				
				DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
				
				m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
				
				if(m_EffectFrame >= 2.0f)
				{
					m_EffectFrame = 0.0f;
				}				
				
				int selectedframe = (int)m_EffectFrame;
				
				if( selectedframe == 0 || selectedframe == 1 )
					m_imageDialogNextPagePoint[selectedframe]->Draw( m_WindowPosition.x+200, m_WindowPosition.y + 306 );	
					//m_imageDialogNextPagePoint[selectedframe]->Draw( 784, 451 );
				m_PrevDrawTime = g_LocalSystemTime;
			}
		}
		break;	
	case _XDEF_NPCMESSAGE_ORGANIZATION : 
		{
			if( m_pTitleString[0] != 0 )
			{
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
				g_XBaseFont->Puts( m_WindowPosition.x + 37, m_WindowPosition.y + 31, m_pTitleString );	
				g_XBaseFont->Flush();
				g_XBaseFont->SetBoldMode( FALSE );	
			}

			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )  // US, Vietnamese version은 퀘스트 타입이 길어 보상 글 제외.
			{
				if( m_OrganizationNameIME.GetShowStatus() )
				{
					TCHAR tempname[512];
					memset( tempname, 0, sizeof(TCHAR)*512 );

					g_XBaseFont->SetColor( _XSC_DEFAULT_BLACK );
					switch(g_pLocalUser->m_CharacterInfo.groupindex)		// 문파
					{
					case _XGROUP_NANGIN :
						strcpy( tempname, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00 ) );	
						break;
					case _XGROUP_GAEBANG :
						strcpy( tempname, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01 ) );		
						break;
					case _XGROUP_BEEGOONG :
						strcpy( tempname, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02 ) );		
						break;
					case _XGROUP_SORIM :
						strcpy( tempname, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03 ) );
						break;
					case _XGROUP_NOCKRIM :
						strcpy( tempname, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04 ) );
						break;
					case _XGROUP_MOODANG :
						strcpy( tempname, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05 ) );
						break;
					case _XGROUP_MAKYO :
						strcpy( tempname, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06 ) );
						break;
					case _XGROUP_SEGA :
						strcpy( tempname, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_07 ) );
						break;
					case _XGROUP_NOFIXED :
						strcpy( tempname, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_08 ) );
						break;
					}
					strcat( tempname, _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ORGANIZATION) );
					g_XBaseFont->PutsAlign( m_WindowPosition.x + 210, m_WindowPosition.y + 344, _XFONT_ALIGNTYPE_CENTER, tempname );
				}
				
			}
			else // 국내 버젼일때 
			{
				if( m_OrganizationNameIME.GetShowStatus() )
				{
					g_XBaseFont->SetColor( _XSC_DEFAULT_BLACK );
					switch(g_pLocalUser->m_CharacterInfo.groupindex)		// 문파
					{
					case _XGROUP_NANGIN :
						g_XBaseFont->Puts( m_WindowPosition.x + 151, m_WindowPosition.y + 324, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00 ) );			
						break;
					case _XGROUP_GAEBANG :
						g_XBaseFont->Puts( m_WindowPosition.x + 151, m_WindowPosition.y + 324, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01 ) );			
						break;
					case _XGROUP_BEEGOONG :
						g_XBaseFont->Puts( m_WindowPosition.x + 151, m_WindowPosition.y + 324, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02 ) );			
						break;
					case _XGROUP_SORIM :
						g_XBaseFont->Puts( m_WindowPosition.x + 151, m_WindowPosition.y + 324, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03 ) ); 
						break;
					case _XGROUP_NOCKRIM :
						g_XBaseFont->Puts( m_WindowPosition.x + 151, m_WindowPosition.y + 324, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04 ) );
						break;
					case _XGROUP_MOODANG :
						g_XBaseFont->Puts( m_WindowPosition.x + 151, m_WindowPosition.y + 324, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05 ) ); 
						break;
					case _XGROUP_MAKYO :
						g_XBaseFont->Puts( m_WindowPosition.x + 151, m_WindowPosition.y + 324, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06 ) );
						break;
					case _XGROUP_SEGA :
						g_XBaseFont->Puts( m_WindowPosition.x + 151, m_WindowPosition.y + 324, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_07 ) );
						break;
					case _XGROUP_NOFIXED :
						g_XBaseFont->Puts( m_WindowPosition.x + 151, m_WindowPosition.y + 324, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_08 ) );
						break;
					}
					g_XBaseFont->Puts( m_WindowPosition.x + 252, m_WindowPosition.y + 324, _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ORGANIZATION) );	// _T("단")		
				}
			}


			if( m_imageDialogNextPageLeftBorder->GetShowStatus() )
			{
				if(m_PrevDrawTime == 0)
					m_PrevDrawTime = g_LocalSystemTime;
				
				DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
				
				m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
				
				if(m_EffectFrame >= 2.0f)
				{
					m_EffectFrame = 0.0f;
				}				
				
				int selectedframe = (int)m_EffectFrame;
				
				if( selectedframe == 0 || selectedframe == 1 )
					m_imageDialogNextPagePoint[selectedframe]->Draw( m_WindowPosition.x+200, m_WindowPosition.y + 306 );	
					//m_imageDialogNextPagePoint[selectedframe]->Draw( 784, 451 );
				m_PrevDrawTime = g_LocalSystemTime;
			}	
		}
		break;
	case _XDEF_NPCMESSAGE_HAVEQUESTLIST :
		{
			if( m_pTitleString[0] != 0 )
			{
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
				g_XBaseFont->Puts( m_WindowPosition.x + 37, m_WindowPosition.y + 31, m_pTitleString );	
				g_XBaseFont->Flush();
				g_XBaseFont->SetBoldMode( FALSE );	
			}
			
			g_XBaseFont->SetBoldMode( TRUE );
			g_XBaseFont->SetColor( _XSC_DEFAULT_BLACK );
			g_XBaseFont->Puts( m_WindowPosition.x + 54, m_WindowPosition.y + 64, _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_2613) );
			g_XBaseFont->Flush();
			g_XBaseFont->SetBoldMode( FALSE );	
		}
		break;
	}
	
	g_XBaseFont->Flush();
	if( m_QuestIMEControl.GetShowStatus() ) m_QuestIMEControl.Draw();
	if( m_OrganizationNameIME.GetShowStatus() )	m_OrganizationNameIME.Draw();
	if( m_MiniStopEventIMEControl.GetShowStatus() )	m_MiniStopEventIMEControl.Draw();
}

BOOL _XWindow_NPCScript::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;
		
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	
	if(mousestate)
	{
		if(!pfocusobject)
		{			
			if(mousestate->bButton[0])//마우스 왼쪽 클릭시 
			{
				m_WindowMouseState = _XW_STATE_LEFTCLICK;
			}//마우스 왼쪽 클릭하고 있을 때 처리 끝 
		}
	}
	
	
	if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
	{			
		if(!mousestate->bButton[0])
		{	
			
			int selectitem = m_listboxQuestReward->CheckSelectedItem();
			if( selectitem != -1 )
			{						
				if( g_pDefaultTooltip_Window )
				{								
					g_pDefaultTooltip_Window->SetNPCScriptWindowPtr( this );
					DWORD cmdid = (DWORD)m_listboxQuestReward->GetItemCommandID( selectitem );
					int type = (cmdid & 0xFFFF0000) >> 16;
					int id = (cmdid & 0x0000FFFF);
					if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_NPCQUEST, selectitem, type, id ) )									
					{
						g_pDefaultTooltip_Window->ShowWindow( TRUE );
					}														
				}
			}
			else
			{
				if( g_pDefaultTooltip_Window )
				{
					if( g_pDefaultTooltip_Window->GetShowStatus() )						
						g_pDefaultTooltip_Window->ShowWindow( FALSE );
				}
			}
			
			if( m_QuestIMEControl.GetShowStatus() )
			{
				if(CheckQuestStringEditBoxArea())
					m_QuestIMEControl.SetFocus();
			}
			
			if( m_OrganizationNameIME.GetShowStatus() )
			{
				if(CheckOrganizationEditBoxArea())
					m_OrganizationNameIME.SetFocus();
			}

			if( m_MiniStopEventIMEControl.GetShowStatus() )
			{
				if(CheckEventEditBoxArea())
					m_MiniStopEventIMEControl.SetFocus();
			}
			
			if( m_MiniStopEventIMEControl.GetShowStatus() )
			{
				if(CheckEvent01EditBoxArea())
				{
					POINT pt = m_MiniStopEventIMEControl.GetWindowPosition();
					if( pt.x == m_WindowPosition.x+97 )
					{
						strncpy( m_EventCodeString02,m_MiniStopEventIMEControl.GetText(),5);
					}
					else if( pt.x == m_WindowPosition.x+147 )
					{
						strncpy( m_EventCodeString03,m_MiniStopEventIMEControl.GetText(),5);
					}
					memset(m_EventCodeString01, 0, sizeof(TCHAR)*6);
					m_MiniStopEventIMEControl.MoveWindow(m_WindowPosition.x+47, m_WindowPosition.y + 345);
					m_MiniStopEventIMEControl.ClearBuffer();
					m_MiniStopEventIMEControl.SetFocus();
				}
				
				if(CheckEvent02EditBoxArea())
				{
					POINT pt = m_MiniStopEventIMEControl.GetWindowPosition();
					if( pt.x == m_WindowPosition.x+47 )
					{
						strncpy( m_EventCodeString01,m_MiniStopEventIMEControl.GetText(),5);
					}
					else if( pt.x == m_WindowPosition.x+147 )
					{
						strncpy( m_EventCodeString03,m_MiniStopEventIMEControl.GetText(),5);
					}
					memset(m_EventCodeString02, 0, sizeof(TCHAR)*5);
					m_MiniStopEventIMEControl.MoveWindow(m_WindowPosition.x+97, m_WindowPosition.y + 345);
					m_MiniStopEventIMEControl.ClearBuffer();
					m_MiniStopEventIMEControl.SetFocus();
				}

				if(CheckEvent03EditBoxArea())
				{
					POINT pt = m_MiniStopEventIMEControl.GetWindowPosition();
					if( pt.x == m_WindowPosition.x+47 )
					{
						strncpy( m_EventCodeString01,m_MiniStopEventIMEControl.GetText(),5);
					}
					else if( pt.x == m_WindowPosition.x+97 )
					{
						strncpy( m_EventCodeString02,m_MiniStopEventIMEControl.GetText(),5);
					}
					memset(m_EventCodeString03, 0, sizeof(TCHAR)*5);
					m_MiniStopEventIMEControl.MoveWindow(m_WindowPosition.x+147, m_WindowPosition.y + 345);
					m_MiniStopEventIMEControl.ClearBuffer();
					m_MiniStopEventIMEControl.SetFocus();
				}
			}
			

//			if( m_bProgressNPCDialogMessage )
//			{
//				if( m_listboxNPCDialogMessage->CheckMousePosition() )
//					ProcessNPCDialogAtOnce();
//			}

			
			if( m_bProgressNPCDialogMessage )
			{
				if( m_listboxNPCDialogMessage->CheckMousePosition() )
				{					
					if( m_NPCDialogLineCounter%9 != 0) //한페이지가 되기전 글씨가 나오고 있는 상태 
					{
						ProcessNPCDialogAtOnce();	
					}
//					else
//					{
//						m_bPageClickQuestDialog = TRUE;	
//					}					
				}
			}

		}
	}
	
	if(!mousestate->bButton[0])
	{
		if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
			m_WindowMouseState = _XW_STATE_NONE;
	}
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;	

	ProcessNPCDialog();
	
	return TRUE;
}

BOOL _XWindow_NPCScript::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
		
	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}

void _XWindow_NPCScript::SetTitle( LPSTR titlestring )
{
	if( !titlestring ) return;
	if( strlen(titlestring) <= 0 ) return;
	
	memset(m_pTitleString, 0, sizeof(TCHAR)*256);
	strcpy(m_pTitleString, titlestring);
}

 void _XWindow_NPCScript::SetMessage( LPSTR messagestring )
{
	if( !messagestring ) return;
	if( strlen(messagestring) <= 0 ) return;

	m_lpszNPCDialogMessage = messagestring;
	m_NPCDialogLineCounter = 0;
	m_NPCDialogPositionCounter = 0;
	m_NPCDialogPositionTimeConter = 0;
	m_bProgressNPCDialogMessage = TRUE;
		
	memset( &m_NPCDialogSplitinfo, 0, sizeof(_XStringSplitInfo) );

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		g_XBaseFont->SplitString(m_lpszNPCDialogMessage, 335, m_NPCDialogSplitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
	}
	else
	{
		g_XBaseFont->SplitStringByCR(m_lpszNPCDialogMessage, m_NPCDialogSplitinfo);
	}
	m_listboxNPCDialogMessage->DeleteAllItem();
	
	int stringlength = strlen(m_lpszNPCDialogMessage);	
	if( m_pDialogStringTotalBufferLength < stringlength )
	{
		SAFE_DELETE_ARRAY( m_pDialogStringTotalBuffer );
		
		m_pDialogStringTotalBuffer = new TCHAR[stringlength];
		m_pDialogStringTotalBufferLength = stringlength;
		memset( m_pDialogStringTotalBuffer, 0, sizeof(TCHAR) * m_pDialogStringTotalBufferLength );
	}
}

void _XWindow_NPCScript::SetSubScriptList(int index, BYTE scriptclass)
{
	S_SUBSCRIPTITEM newsubscript;
	newsubscript.index = index;

	newsubscript.scriptclass = scriptclass;

	newsubscript.x = 25; 
	newsubscript.y = 181 + 16*index;
	newsubscript.cx = 181;
	newsubscript.cy = 11;

	m_SubScriptList.push_back(newsubscript);
}

void _XWindow_NPCScript::ClearSubScriptList(void)
{
	for(int i = 0 ; i < 4 ; i++)
	{
		m_ScriptButton[i]->SetButtonText(_T(""));
		m_ScriptButton[i]->ShowWindow(FALSE);
	}
	m_ScriptButton[0]->SetCommandID(_XDEF_NPCSCRIPT_SUBSCRIPT1);
	m_ScriptButton[1]->SetCommandID(_XDEF_NPCSCRIPT_SUBSCRIPT2);
	m_ScriptButton[2]->SetCommandID(_XDEF_NPCSCRIPT_SUBSCRIPT3);
	m_ScriptButton[3]->SetCommandID(_XDEF_NPCSCRIPT_SUBSCRIPT4);

	m_SubScriptList.clear();
}

void _XWindow_NPCScript::ShowWindow(BOOL show)
{	

	if( !show )
	{		
		ClearSubScriptList();

		m_listboxNPCDialogMessage->DeleteAllItem();
		m_listboxQuestReward->DeleteAllItem();
		m_listboxSelectionItem->DeleteAllItem();
		
		m_btnQuestOK->ShowWindow(FALSE);
		m_btnBackToNPCMode->ShowWindow(FALSE);
		m_listboxQuestReward->ShowWindow(FALSE);
		m_listboxSelectionItem->ShowWindow(FALSE);
		m_NPCDialogSubBorderA1->ShowWindow(FALSE);
		m_NPCDialogSubBorderA2->ShowWindow(FALSE);
		m_NPCDialogSubBorderB1->ShowWindow(FALSE);
		m_NPCDialogSubBorderB2->ShowWindow(FALSE);
		m_NPCDialogMainTitleBorder->ShowWindow(FALSE);
		m_NPCDialogIMEBorder1->ShowWindow(FALSE);
		m_NPCDialogIMEBorder2->ShowWindow(FALSE);
		m_OrganizationNameIMEBorder1->ShowWindow(FALSE);
		m_OrganizationNameIMEBorder2->ShowWindow(FALSE);
	}


	_XWindow::ShowWindow(show);
}

void _XWindow_NPCScript::SetDefaultPosition( void )
{		
	//MoveWindow(  200, gnHeight-150  );	
	m_btnNPCDialogModExit->MoveWindow(gnWidth-(100), gnHeight-(46) );
}

BOOL _XWindow_NPCScript::GetShowWindow(void)
{
	return m_ShowWindow;
}

void _XWindow_NPCScript::GenerateSubScriptList(void)
{
	if(!m_SubScriptList.empty())
	{
		int position = 0;
		list <S_SUBSCRIPTITEM>::iterator iter_script;
		for(iter_script = m_SubScriptList.begin() ; iter_script != m_SubScriptList.end() ; iter_script++)
		{
			S_SUBSCRIPTITEM subscript = *iter_script;
			if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_ENDSCRIPT ) 
			{	
				//스크립트 종료
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_STARTTALK);		// _T("말 걸기")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_DIALOG )
			{	
				//대화
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_TALK);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}	
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_QA )
			{
				//정보
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_INFORMATION);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_TRADEITEM )
			{	
				//아이템 매매
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_ITEMCONTRACT);	// _T("물품 매매")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_REPAIRITEM )
			{	
				//아이템 수리 
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_REPAIRWEAPON);	// _T("무기 수리") 2004.06.19->oneway48 insert
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_REPAIRCLOTHES )// 2004.06.19->oneway48 insert
			{	
				//아이템 수리 
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_REPAIRCLOTHES);	// _T("의상 수선")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_INFORMATION )
			{	
				//힌트 제공
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_FURNISHINFO);	// _T("단서 제공")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_BLACKMARKET )
			{	
				//암거래
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_BACKDOORBUSINESS);	// _T("암 거래")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_SELLHERB )
			{	
				//약초 구입
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);//D3DCOLOR_ARGB(255, 189, 188, 175)
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_HERB);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_SELLCHARM )
			{	
				//부적 구입
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_CHARM);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_JUDGEVALUE )
			{	
				//아이템 감정
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_JUDGMENTITEM);	// _T("물품 감정")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_GAMBLE )
			{	
				//미니 게임 시작
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_GAMBLE);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_MOVE )
			{	
				//순간 이동
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_WARP);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_WARP )
			{	
				//지역 이동
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_WARPZONE);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_SAVEITEM )
			{	
				//창고 : 아이템 저장
				// 2004.06.02->oneway48 insert
				_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
				
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				
				// 2004.06.02->oneway48 modify
				if( !pWarehouse_Window->m_bTerminateWarehouse )
					m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_USESTORAGE);
				else
					m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_TIMEOUTWAREHOUSE);	// _T("창고 만기")

				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_ADDSAVEITEM )
			{	
				//창고 추가
				
				// 2004.06.01->oneway48 insert
				_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
				
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				
				// 2004.06.01->oneway48 modify
				//if( !pWarehouse_Window->m_bUseableWarehouse )
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_ADDSTORAGE);
				//else
				//	m_ScriptButton[position]->SetButtonText(_T("창고 해제"));
				
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_EXITGAME )
			{	
				//게임 종료 
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_EXITGAME);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_STARTQUEST )
			{	
				//퀘스트 
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_STARTQUEST);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if( subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_LEARNSKILL )
			{
				//무공 전수
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_RECEIVINGSKILL);	// _T("무공 전수")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_SAVEPOINT)
			{
				// 여장 풀기
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_INN);	// _T("여장 풀기")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_CLANSERVICEPOINT)
			{
				// 문파 기여
				// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
				if( g_TutorialInfo )
				{
					if( !g_TutorialInfo->_XTutorial_027 )
					{
						_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
						pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_027);
						g_TutorialInfo->_XTutorial_027 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
						g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif						
					}
				}				
				// ----------------------------------------------------------------------------------------------------------------------------=
				
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_CONTRIBUTION);	// _T("문파 기여")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_HEALPLAYER)
			{
				// 치료하기
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_HEALING);	// _T("치료 하기")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_EXPLAINCLASS)
			{
				// 역할설명 
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_JOBEMPLANATION);	// _T("직책 설명")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_MAKEORGANIZATION)
			{
				// 영웅단창설
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_CREATEORGANIZATION);	// _T("영웅단창설")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_MINISTOPEVENT)
			{
				// 이벤트
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				
				switch(m_EventType) {
				case 0:
					m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_2267);
					break;
				case 1:
					m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_2268);
					break;
				case 2:
					m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_2269);
					break;
				case 3:
					m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_2270);
					break;
				}
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_SHOWRANKQUEST)
			{
				// 퀘스트 순위 보여주기
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_2271);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_PVPLIST)
			{
				// 비무 목록 보여주기
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_2272);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_SOCKETEXPANSION)
			{
				// 영웅단창설
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAMEMESSAGEHANDLER_1872);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_SOCKETDESTROY)
			{
				// 영웅단창설
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_2273);	
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_VIEWFREEMATCHINFO)
			{
				// 비무대회 정보 보기
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3776);//"대회 정보"
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_COLLECTION)
			{
				// 명품수집
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAMEMESSAGEHANDLER_1877);	
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_RESOURCE)
			{
				// 재료수집
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAMEMESSAGEHANDLER_1878);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_INCHANT )
			{
				// 무기 제련
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAMEMESSAGEHANDLER_1879);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_NPCBUFF)
			{
				// 점혈술
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAMEMESSAGEHANDLER_1880);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
#ifdef _XTS_MOVEVILLAGE
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_MOVEVILLAGE)
			{
				// 표국 이동
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3043);
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
#endif
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_CASTLEINFO)
			{
				// 장원정보
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3234); //장원 정보
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_CASTLEATTACKLIST)
			{
				// 장원도전자목록
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3235);//도전자 목록
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_TRACING)
			{
				// 특정인 위치 추적
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3402);//"추적의뢰"
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
#ifdef _XDEF_MIXWEAPON_071009_KUKURI
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_MIXWEAPON)
			{
				// 무기 조합
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3725);//"무기조합"
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
#endif
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_ENTERCONTEST)
			{
				// 비무참가
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_2274);//대회참가
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_VIEWCONTEST)
			{
				// 비무관람
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3728);//대회관람
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
#endif
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_CLANPOINTITEM)
			{
				// 문파기여도로 아이템 사기
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_ITEMCONTRACT);	// _T("물품 매매")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_MASTERNAME) 
			{
				// 사호 입력
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3875); //사호 입력
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_MNDINFO) 
			{
				// 사제 정보
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3851);	//스승 정보
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
			else if(subscript.scriptclass == _XDEF_NPCSUBSCRIPTCLASS_MNDPOINTPURCHASE) 
			{
				// 사제기여도로 아이템 사기
				m_ScriptButton[position]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
				m_ScriptButton[position]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_ITEMCONTRACT);	// _T("물품 매매")
				m_ScriptButton[position]->SetCommandID(_XDEF_NPCSCRIPT_NEXTSCRIPT);
				m_ScriptButton[position]->SetLParam(subscript.index);
				m_ScriptButton[position]->ShowWindow(TRUE);
				position++;
			}
		#endif			
		}
	}
}

void _XWindow_NPCScript::ChangeButton(void)
{
	if(!m_SubScriptList.empty())
	{
		list <S_SUBSCRIPTITEM>::iterator iter_script;
		for(iter_script = m_SubScriptList.begin() ; iter_script != m_SubScriptList.end() ; iter_script++)
		{
			S_SUBSCRIPTITEM subscript = *iter_script;
			if( subscript.scriptclass = _XDEF_NPCSUBSCRIPTCLASS_ENDSCRIPT )
			{
				m_ScriptButton[3]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_OK);
			}
		}
	}
}

void _XWindow_NPCScript::InsertNPCChatString(LPSTR message, int messagemode,  BOOL mainchatwrite)
{
	if( strlen( message ) <= 0 ) return;
	
	if( mainchatwrite )// 2004.06.02->oneway48 insert
		g_NetworkKernel.InsertChatString(message,messagemode);//2004.05.22->oneawy48 insert

}

void _XWindow_NPCScript::ProcessNPCDialogAtOnce(void)
{
	if( !m_bProgressNPCDialogMessage ) return;
	if( !m_lpszNPCDialogMessage ) return;
	
	m_listboxNPCDialogMessage->DeleteAllItem();
	
	int		linecount = 0;
		
	int totalline = ((m_NPCDialogLineCounter/9)+1)*9;
	if( totalline >= m_NPCDialogSplitinfo.splitcount )
		totalline = m_NPCDialogSplitinfo.splitcount;
	
	//for(int i = 0 ; i < m_NPCDialogSplitinfo.splitcount ; i++)
	for(int i = 0 ; i < totalline ; i++)
	{
		memset( m_pDialogStringTotalBuffer, 0, sizeof(TCHAR) * m_pDialogStringTotalBufferLength );
		strncpy(m_pDialogStringTotalBuffer, m_NPCDialogSplitinfo.splitpoint[i], m_NPCDialogSplitinfo.splitlength[i]);
		m_listboxNPCDialogMessage->InsertListItem(_T(""), 0, 0);
		m_listboxNPCDialogMessage->SetItemText(linecount, 0, m_pDialogStringTotalBuffer);
		m_listboxNPCDialogMessage->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
		
		linecount++;
	}	
	m_listboxNPCDialogMessage->SetLineScrollPos( 999999 );
	
	if( totalline != m_NPCDialogSplitinfo.splitcount )
	{
		m_NPCDialogLineCounter = totalline;
		m_NPCDialogPositionCounter = m_NPCDialogSplitinfo.splitlength[m_NPCDialogLineCounter-1];
	}
	else
	{
		m_NPCDialogLineCounter = m_NPCDialogSplitinfo.splitcount;
		m_NPCDialogPositionCounter = m_NPCDialogSplitinfo.splitlength[m_NPCDialogLineCounter-1];
		memset( &m_NPCDialogSplitinfo, 0, sizeof(_XStringSplitInfo) );
		m_bProgressNPCDialogMessage = FALSE;
		g_QuestScriptManager.m_bStartMessage = FALSE;// 대사가 끝나면 선택지를 보여준다.
		
		if( m_MessageType == _XDEF_NPCMESSAGE_QUEST)
		{
			//if( g_LastNPCScriptOwner->m_NPCRollType == _XNPCROLL_TRADE || g_LastNPCScriptOwner->m_NPCRollType == _XNPCROLL_WAREHOUSE)
			//{
			//	m_btnBackToNPCMode->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_TRADE);	// _T("거래하기")
			//}
			//else //Author : 양희왕 breif : 주석처리, 퀘스트가 끝나면 나오는 메시지 처리 Date : 07/11/01
			{
				m_btnBackToNPCMode->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_TALK);	// _T("대화하기")
			}
			m_btnQuestOK->ShowWindow(TRUE);
			m_btnBackToNPCMode->ShowWindow(TRUE);
			m_btnNPCDialogModExit->ShowWindow(TRUE);
		}
		else if( m_MessageType == _XDEF_NPCMESSAGE_ORGANIZATION )
		{
			m_NPCDialogSubBorderB1->ShowWindow(TRUE);
			m_NPCDialogSubBorderB2->ShowWindow(TRUE);
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
			{
				m_OrganizationNameIMEBorder1->ShowWindow(FALSE);
				m_OrganizationNameIMEBorder2->ShowWindow(FALSE);
			}
			else
			{
				m_OrganizationNameIMEBorder1->ShowWindow(TRUE);
				m_OrganizationNameIMEBorder2->ShowWindow(TRUE);
			}
			m_CompleteNameInput->ShowWindow(TRUE);
			m_NampInputCancle->ShowWindow(TRUE);		
			m_OrganizationNameIME.ShowWindow(TRUE);
			m_OrganizationNameIME.SetFocus();
		}
	}
}

void _XWindow_NPCScript::ProcessNPCDialog(void)
{
	if( !m_bProgressNPCDialogMessage ) return;
	if( !m_lpszNPCDialogMessage ) return;
	if( m_NPCDialogPositionTimeConter == 0 )
	{
		m_NPCDialogPositionTimeConter = g_LocalSystemTime;
	}
		
	DWORD elpasedtime = g_LocalSystemTime - m_NPCDialogPositionTimeConter;

		
	if( elpasedtime > 10 )
	{
		if( m_NPCDialogPositionCounter >= m_NPCDialogSplitinfo.splitlength[m_NPCDialogLineCounter-1] )
		{
			
			if( m_NPCDialogLineCounter > m_NPCDialogSplitinfo.splitcount )
			{
				m_NPCDialogPositionCounter = 0;
				m_NPCDialogLineCounter = m_NPCDialogSplitinfo.splitcount;
				m_NPCDialogPositionCounter = m_NPCDialogSplitinfo.splitlength[m_NPCDialogLineCounter-1];
				memset( &m_NPCDialogSplitinfo, 0, sizeof(_XStringSplitInfo) );
				m_bProgressNPCDialogMessage = FALSE;
				g_QuestScriptManager.m_bStartMessage = FALSE;// 대사가 끝나면 선택지를 보여준다.
				if( m_MessageType == _XDEF_NPCMESSAGE_QUEST)
				{
					if( g_LastNPCScriptOwner->m_NPCRollType == _XNPCROLL_TRADE || g_LastNPCScriptOwner->m_NPCRollType == _XNPCROLL_WAREHOUSE)
					{
						m_btnBackToNPCMode->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_TRADE);	// _T("거래하기")
					}
					else //Author : 양희왕 breif : 주석처리, 퀘스트가 끝나면 나오는 메시지 처리 Date : 07/11/01
					{
						m_btnBackToNPCMode->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_TALK);	// _T("대화하기")
					}
					m_btnQuestOK->ShowWindow(TRUE);				
					m_btnBackToNPCMode->ShowWindow(TRUE);
					m_btnNPCDialogModExit->ShowWindow(TRUE);
				}
				else if( m_MessageType == _XDEF_NPCMESSAGE_ORGANIZATION )
				{
					m_NPCDialogSubBorderB1->ShowWindow(TRUE);
					m_NPCDialogSubBorderB2->ShowWindow(TRUE);
					if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
					{
						m_OrganizationNameIMEBorder1->ShowWindow(FALSE);
						m_OrganizationNameIMEBorder2->ShowWindow(FALSE);
					}
					else
					{
						m_OrganizationNameIMEBorder1->ShowWindow(TRUE);
						m_OrganizationNameIMEBorder2->ShowWindow(TRUE);
					}
					m_CompleteNameInput->ShowWindow(TRUE);
					m_NampInputCancle->ShowWindow(TRUE);		
					m_OrganizationNameIME.ShowWindow(TRUE);
					m_OrganizationNameIME.SetFocus();
				}
			}
			else
			{
				
				if( m_NPCDialogLineCounter%9 == 0) //한페이지분량이 되면 클릭할 때까지 대기한다.
				{
					if( m_bPageClickQuestDialog || m_NPCDialogLineCounter == 0 )
					{
						m_bPageClickQuestDialog = FALSE;
						
						m_NPCDialogLineCounter++;				
						m_NPCDialogPositionCounter = 0;
						
						
						memset( m_pDialogStringTotalBuffer, 0, sizeof(TCHAR) * m_pDialogStringTotalBufferLength );
						strncpy(m_pDialogStringTotalBuffer, m_NPCDialogSplitinfo.splitpoint[m_NPCDialogLineCounter-1], m_NPCDialogSplitinfo.splitlength[m_NPCDialogLineCounter-1]);
						if( m_pDialogStringTotalBuffer[m_NPCDialogPositionCounter] & 0x80 ) //한글일때 2바이트 
						{
							m_NPCDialogPositionCounter+=2;		
						}
						else
						{
							m_NPCDialogPositionCounter++;
						}
						
						TCHAR	dialogbuffer[2048];
						memset(dialogbuffer, 0, sizeof(dialogbuffer));
						strncpy(dialogbuffer, m_pDialogStringTotalBuffer, m_NPCDialogPositionCounter);
						
						m_listboxNPCDialogMessage->InsertListItem(_T(""), 0, 0);
						m_listboxNPCDialogMessage->SetItemText(m_NPCDialogLineCounter-1, 0, dialogbuffer);
						m_listboxNPCDialogMessage->SetItemAttrib(m_NPCDialogLineCounter-1, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
						m_listboxNPCDialogMessage->SetLineScrollPos( 999999 );
					}
					else
					{
						m_btnDialogNextPage->ShowWindow(TRUE);
						m_imageDialogNextPageLeftBorder->ShowWindow(TRUE);
						m_imageDialogNextPageRightBorder->ShowWindow(TRUE);						
					}
				}
				else
				{
					m_NPCDialogLineCounter++;				
					m_NPCDialogPositionCounter = 0;
					
					memset( m_pDialogStringTotalBuffer, 0, sizeof(TCHAR) * m_pDialogStringTotalBufferLength );
					strncpy(m_pDialogStringTotalBuffer, m_NPCDialogSplitinfo.splitpoint[m_NPCDialogLineCounter-1], m_NPCDialogSplitinfo.splitlength[m_NPCDialogLineCounter-1]);
					if( m_pDialogStringTotalBuffer[m_NPCDialogPositionCounter] & 0x80 ) //한글일때 2바이트 
					{
						m_NPCDialogPositionCounter+=2;		
					}
					else
					{
						m_NPCDialogPositionCounter++;
					}
					
					TCHAR	dialogbuffer[2048];
					memset(dialogbuffer, 0, sizeof(dialogbuffer));
					strncpy(dialogbuffer, m_pDialogStringTotalBuffer, m_NPCDialogPositionCounter);
					
					m_listboxNPCDialogMessage->InsertListItem(_T(""), 0, 0);
					m_listboxNPCDialogMessage->SetItemText(m_NPCDialogLineCounter-1, 0, dialogbuffer);
					m_listboxNPCDialogMessage->SetItemAttrib(m_NPCDialogLineCounter-1, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
					m_listboxNPCDialogMessage->SetLineScrollPos( 999999 );
				}
				
			}
		}
		else
		{
			memset( m_pDialogStringTotalBuffer, 0, sizeof(TCHAR) * m_pDialogStringTotalBufferLength );
			strncpy(m_pDialogStringTotalBuffer, m_NPCDialogSplitinfo.splitpoint[m_NPCDialogLineCounter-1], m_NPCDialogSplitinfo.splitlength[m_NPCDialogLineCounter-1]);
			if( m_pDialogStringTotalBuffer[m_NPCDialogPositionCounter] & 0x80 ) //한글일때 2바이트 
			{
				m_NPCDialogPositionCounter+=2;		
			}
			else // 한글이 아닐때 1바이트
			{
				m_NPCDialogPositionCounter++;
			}
			
			TCHAR	dialogbuffer[2048];
			memset(dialogbuffer, 0, sizeof(dialogbuffer));
			strncpy(dialogbuffer, m_pDialogStringTotalBuffer, m_NPCDialogPositionCounter);
			
			m_listboxNPCDialogMessage->SetItemText(m_NPCDialogLineCounter-1, 0, dialogbuffer);
			m_listboxNPCDialogMessage->SetItemAttrib(m_NPCDialogLineCounter-1, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
		}				
	}
}


void _XWindow_NPCScript::SetMessageType(_XNPCMESSAGETYPE messagetype)
{
	m_MessageType = messagetype;
	switch(m_MessageType) {
	case _XDEF_NPCMESSAGE_NORMAL:
		{
			m_btnQuestOK->ShowWindow(FALSE);
			m_btnBackToNPCMode->ShowWindow(FALSE);
			m_listboxHaveQuestList->ShowWindow(FALSE);
			m_listboxQuestReward->ShowWindow(FALSE);
			m_listboxSelectionItem->ShowWindow(FALSE);
			m_NPCDialogSubBorderB1->ShowWindow(FALSE);
			m_NPCDialogSubBorderB2->ShowWindow(FALSE);
			m_NPCDialogMainTitleBorder->ShowWindow(TRUE);
			m_NPCDialogIMEBorder1->ShowWindow(FALSE);
			m_NPCDialogIMEBorder2->ShowWindow(FALSE);
			m_OrganizationNameIMEBorder1->ShowWindow(FALSE);
			m_OrganizationNameIMEBorder2->ShowWindow(FALSE);
			m_imageQuesLevelDisplay->ShowWindow(FALSE);
			m_CompleteNameInput->ShowWindow(FALSE);
			m_NampInputCancle->ShowWindow(FALSE);
			m_imageEventContent01->ShowWindow(FALSE);
			m_imageEventContent02->ShowWindow(FALSE);
			m_imageEventContent03->ShowWindow(FALSE);
			m_imageEventContent04->ShowWindow(FALSE);
			m_MiniStopEventIMEBorder1->ShowWindow(FALSE);
			m_MiniStopEventIMEBorder2->ShowWindow(FALSE);
			m_EventIMEBorder1->ShowWindow(FALSE);
			m_EventIMEBorder2->ShowWindow(FALSE);
			m_btnMiniStopEventInput->ShowWindow(FALSE);
			m_MiniStopEventIMEControl.ClearBuffer();
			_XIMEKERNEL.SetFocus( NULL );	
			_XIMEKERNEL.ResetCombinationBuffer();			
			if( m_pMiniMapWindowPtr )
			{
				m_pMiniMapWindowPtr->m_LargeMinimapImage.SetScale( 0.87890625f, 0.87890625f);
			}
		}
		break;
	case _XDEF_NPCMESSAGE_SAVEPOINT:
		{
			//Author : 양희왕
			m_listboxHaveQuestList->ShowWindow(FALSE); 
			m_NPCDialogSubBorderA1->ShowWindow(FALSE);
			m_NPCDialogSubBorderA2->ShowWindow(FALSE);

			m_imageEventContent01->ShowWindow(FALSE);
			m_imageEventContent02->ShowWindow(FALSE);
			m_imageEventContent03->ShowWindow(FALSE);
			m_imageEventContent04->ShowWindow(FALSE);
			m_MiniStopEventIMEBorder1->ShowWindow(FALSE);
			m_MiniStopEventIMEBorder2->ShowWindow(FALSE);
			m_EventIMEBorder1->ShowWindow(FALSE);
			m_EventIMEBorder2->ShowWindow(FALSE);
			m_btnMiniStopEventInput->ShowWindow(FALSE);
			m_MiniStopEventIMEControl.ClearBuffer();
			_XIMEKERNEL.SetFocus( NULL );	
			_XIMEKERNEL.ResetCombinationBuffer();				
			m_listboxNPCDialogMessage->DeleteAllItem();
			if( m_pMiniMapWindowPtr )
			{
				m_pMiniMapWindowPtr->m_LargeMinimapImage.ShowWindow(TRUE);
				m_pMiniMapWindowPtr->m_LargeMinimapImage.SetScale( 0.5859375f, 0.5859375f );
#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕
				m_pMiniMapWindowPtr->SetMinimapTransparency( 255 );
#endif
			}
		}
		break;
	case _XDEF_NPCMESSAGE_QUEST:
		{
			ClearSubScriptList();
			m_listboxHaveQuestList->ShowWindow(FALSE);
			m_listboxQuestReward->ShowWindow(FALSE);
			m_listboxSelectionItem->ShowWindow(FALSE);
			m_NPCDialogSubBorderA1->ShowWindow(TRUE);
			m_NPCDialogSubBorderA2->ShowWindow(TRUE);
			m_NPCDialogSubBorderB1->ShowWindow(FALSE);
			m_NPCDialogSubBorderB2->ShowWindow(FALSE);
			m_NPCDialogMainTitleBorder->ShowWindow(TRUE);
			m_NPCDialogIMEBorder1->ShowWindow(FALSE);
			m_NPCDialogIMEBorder2->ShowWindow(FALSE);
			m_OrganizationNameIMEBorder1->ShowWindow(FALSE);
			m_OrganizationNameIMEBorder2->ShowWindow(FALSE);
			m_imageQuesLevelDisplay->ShowWindow(TRUE);
			m_CompleteNameInput->ShowWindow(FALSE);
			m_NampInputCancle->ShowWindow(FALSE);
			m_imageEventContent01->ShowWindow(FALSE);
			m_imageEventContent02->ShowWindow(FALSE);
			m_imageEventContent03->ShowWindow(FALSE);
			m_imageEventContent04->ShowWindow(FALSE);
			m_MiniStopEventIMEBorder1->ShowWindow(FALSE);
			m_MiniStopEventIMEBorder2->ShowWindow(FALSE);
			m_EventIMEBorder1->ShowWindow(FALSE);
			m_EventIMEBorder2->ShowWindow(FALSE);
			m_btnMiniStopEventInput->ShowWindow(FALSE);
			m_MiniStopEventIMEControl.ClearBuffer();
			_XIMEKERNEL.SetFocus( NULL );	
			_XIMEKERNEL.ResetCombinationBuffer();			
			if( m_pMiniMapWindowPtr )
			{
				m_pMiniMapWindowPtr->m_LargeMinimapImage.SetScale( 0.87890625f, 0.87890625f);
			}
			
			_XWindow_QuestRank* pQuestRank_Window = (_XWindow_QuestRank*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_QUESTRANKWINDOW );
			if(pQuestRank_Window) 
			{
				pQuestRank_Window->ShowWindow(FALSE);
			}
		}
		break;
	case _XDEF_NPCMESSAGE_ORGANIZATION :
		{
			m_btnQuestOK->ShowWindow(FALSE);
			m_btnBackToNPCMode->ShowWindow(FALSE);
			m_listboxHaveQuestList->ShowWindow(FALSE);
			m_listboxQuestReward->ShowWindow(FALSE);
			m_listboxSelectionItem->ShowWindow(FALSE);
			m_NPCDialogSubBorderA1->ShowWindow(TRUE);
			m_NPCDialogSubBorderA2->ShowWindow(TRUE);
			m_NPCDialogSubBorderB1->ShowWindow(FALSE);
			m_NPCDialogSubBorderB2->ShowWindow(FALSE);
			m_NPCDialogMainTitleBorder->ShowWindow(TRUE);
			m_NPCDialogIMEBorder1->ShowWindow(FALSE);
			m_NPCDialogIMEBorder2->ShowWindow(FALSE);
			m_imageQuesLevelDisplay->ShowWindow(FALSE);
			m_CompleteNameInput->ShowWindow(FALSE);
			m_NampInputCancle->ShowWindow(FALSE);	
			for( int i = 0; i < 4; ++i )
			{
				m_ScriptButton[i]->ShowWindow(FALSE);
			}
			m_imageEventContent01->ShowWindow(FALSE);
			m_imageEventContent02->ShowWindow(FALSE);
			m_imageEventContent03->ShowWindow(FALSE);
			m_imageEventContent04->ShowWindow(FALSE);
			m_MiniStopEventIMEBorder1->ShowWindow(FALSE);
			m_MiniStopEventIMEBorder2->ShowWindow(FALSE);
			m_EventIMEBorder1->ShowWindow(FALSE);
			m_EventIMEBorder2->ShowWindow(FALSE);
			m_btnMiniStopEventInput->ShowWindow(FALSE);
			m_OrganizationNameIME.ShowWindow(FALSE);
			m_MiniStopEventIMEControl.ClearBuffer();
			_XIMEKERNEL.SetFocus( NULL );	
			_XIMEKERNEL.ResetCombinationBuffer();			
			
			switch(g_pLocalUser->m_CharacterInfo.groupindex)
			{
			case _XGROUP_GAEBANG :
				SetMessage(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_SPEECHGAEBANG));
				break;
			case _XGROUP_BEEGOONG :
				SetMessage(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_SPEECHBEEGOONG));	
				break;
			case _XGROUP_SORIM :
				SetMessage(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_SPEECHSORIM));	
				break;
			case _XGROUP_NOCKRIM :
				SetMessage(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_SPEECHNOCKRIM));		
				break;
			case _XGROUP_MOODANG :			
				SetMessage(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_2599));						
				break;
			case _XGROUP_MAKYO :
				SetMessage(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_2600));		
				break;
			}
			
//			m_OrganizationNameIMEBorder1->ShowWindow(TRUE);
//			m_OrganizationNameIMEBorder2->ShowWindow(TRUE);
//			m_OrganizationNameIME.ShowWindow(TRUE);
//			m_OrganizationNameIME.SetFocus();

		}
		break;
	case _XDEF_NPCMESSAGE_MINISTOPEVENT:
		{
			m_btnQuestOK->ShowWindow(FALSE);
			m_btnBackToNPCMode->ShowWindow(FALSE);
			m_listboxHaveQuestList->ShowWindow(FALSE);
			m_listboxQuestReward->ShowWindow(FALSE);
			m_listboxSelectionItem->ShowWindow(FALSE);
			m_NPCDialogSubBorderA1->ShowWindow(TRUE);
			m_NPCDialogSubBorderA2->ShowWindow(TRUE);
			m_NPCDialogSubBorderB1->ShowWindow(FALSE);
			m_NPCDialogSubBorderB2->ShowWindow(FALSE);
			m_NPCDialogMainTitleBorder->ShowWindow(TRUE);
			m_NPCDialogIMEBorder1->ShowWindow(FALSE);
			m_NPCDialogIMEBorder2->ShowWindow(FALSE);
			m_OrganizationNameIMEBorder1->ShowWindow(FALSE);
			m_OrganizationNameIMEBorder2->ShowWindow(FALSE);
			m_imageQuesLevelDisplay->ShowWindow(FALSE);
			m_CompleteNameInput->ShowWindow(FALSE);
			m_NampInputCancle->ShowWindow(FALSE);
			m_btnMiniStopEventInput->ShowWindow(TRUE);
			m_EventIMEBorder1->ShowWindow(FALSE);
			m_EventIMEBorder2->ShowWindow(FALSE);
			
			switch(m_EventType) {
			case 0://행운의 문자 이벤트
				{
					m_imageEventContent01->ShowWindow(TRUE);
					m_MiniStopEventIMEBorder1->ShowWindow(TRUE);
					m_MiniStopEventIMEBorder2->ShowWindow(TRUE);
					m_MiniStopEventIMEControl.MoveWindow(m_WindowPosition.x+50, m_WindowPosition.y + 345);
					m_MiniStopEventIMEControl.ResizeWindow(162, 20);
					m_MiniStopEventIMEControl.SetMaxLength(64);
				}
				break;
			case 1://훼밀리마트 이벤트
				{
					m_imageEventContent02->ShowWindow(TRUE);
					m_MiniStopEventIMEBorder1->ShowWindow(TRUE);
					m_MiniStopEventIMEBorder2->ShowWindow(TRUE);
					m_MiniStopEventIMEControl.MoveWindow(m_WindowPosition.x+50, m_WindowPosition.y + 345);
					m_MiniStopEventIMEControl.ResizeWindow(162, 20);
					m_MiniStopEventIMEControl.SetMaxLength(64);
				}
				break;
			case 2://예약 가입 이벤트 
				{
					memset(m_EventCodeString01, 0, sizeof(TCHAR)*6);
					memset(m_EventCodeString02, 0, sizeof(TCHAR)*6);
					memset(m_EventCodeString03, 0, sizeof(TCHAR)*6);
					
					m_imageEventContent03->ShowWindow(TRUE);
					m_EventIMEBorder1->ShowWindow(TRUE);
					m_EventIMEBorder2->ShowWindow(TRUE);
					
					m_MiniStopEventIMEControl.MoveWindow(m_WindowPosition.x+47, m_WindowPosition.y + 345);
					m_MiniStopEventIMEControl.ResizeWindow(34, 20);
					m_MiniStopEventIMEControl.SetMaxLength(5);
				}
				break;
			case 3://상용화 이벤트 
				{
					memset(m_EventCodeString01, 0, sizeof(TCHAR)*6);
					memset(m_EventCodeString02, 0, sizeof(TCHAR)*6);
					memset(m_EventCodeString03, 0, sizeof(TCHAR)*6);
					
					m_imageEventContent04->ShowWindow(TRUE);
					m_EventIMEBorder1->ShowWindow(TRUE);
					m_EventIMEBorder2->ShowWindow(TRUE);
					
					m_MiniStopEventIMEControl.MoveWindow(m_WindowPosition.x+47, m_WindowPosition.y + 345);
					m_MiniStopEventIMEControl.ResizeWindow(34, 20);
					m_MiniStopEventIMEControl.SetMaxLength(5);
				}
				break;				
			}

			if( m_pMiniMapWindowPtr )
			{
				m_pMiniMapWindowPtr->m_LargeMinimapImage.SetScale( 0.87890625f, 0.87890625f);
			}
			m_MiniStopEventIMEControl.ShowWindow(TRUE);
			m_MiniStopEventIMEControl.SetFocus();
		}
		break;
	case _XDEF_NPCMESSAGE_HAVEQUESTLIST:
		{
			SetQuestListItem();
			m_listboxHaveQuestList->ShowWindow(TRUE);

			m_listboxQuestReward->ShowWindow(FALSE);
			m_listboxSelectionItem->ShowWindow(FALSE);
			m_NPCDialogSubBorderA1->ShowWindow(TRUE);
			m_NPCDialogSubBorderA2->ShowWindow(TRUE);
			m_NPCDialogSubBorderB1->ShowWindow(FALSE);
			m_NPCDialogSubBorderB2->ShowWindow(FALSE);
			m_NPCDialogMainTitleBorder->ShowWindow(TRUE);
			m_NPCDialogIMEBorder1->ShowWindow(FALSE);
			m_NPCDialogIMEBorder2->ShowWindow(FALSE);
			m_OrganizationNameIMEBorder1->ShowWindow(FALSE);
			m_OrganizationNameIMEBorder2->ShowWindow(FALSE);
			m_imageQuesLevelDisplay->ShowWindow(FALSE);
			m_CompleteNameInput->ShowWindow(FALSE);
			m_NampInputCancle->ShowWindow(FALSE);
			m_imageEventContent01->ShowWindow(FALSE);
			m_imageEventContent02->ShowWindow(FALSE);
			m_imageEventContent03->ShowWindow(FALSE);
			m_imageEventContent04->ShowWindow(FALSE);
			m_MiniStopEventIMEBorder1->ShowWindow(FALSE);
			m_MiniStopEventIMEBorder2->ShowWindow(FALSE);
			m_EventIMEBorder1->ShowWindow(FALSE);
			m_EventIMEBorder2->ShowWindow(FALSE);
			m_btnMiniStopEventInput->ShowWindow(FALSE);
			m_MiniStopEventIMEControl.ClearBuffer();
			_XIMEKERNEL.SetFocus( NULL );	
			_XIMEKERNEL.ResetCombinationBuffer();			
			if( m_pMiniMapWindowPtr )
			{
				m_pMiniMapWindowPtr->m_LargeMinimapImage.SetScale( 0.87890625f, 0.87890625f);
			}
			
		}
		break;
	case _XDEF_NPCMESSAGE_TRACING:
	case _XDEF_NPCMESSAGE_MASTERNAME  : //Author : 양희왕 //breif : 사제 
		{
			m_btnQuestOK->ShowWindow(FALSE);
			m_btnBackToNPCMode->ShowWindow(FALSE);
			m_listboxHaveQuestList->ShowWindow(FALSE);
			m_listboxQuestReward->ShowWindow(FALSE);
			m_listboxSelectionItem->ShowWindow(FALSE);
			m_NPCDialogSubBorderA1->ShowWindow(TRUE);
			m_NPCDialogSubBorderA2->ShowWindow(TRUE);
			m_NPCDialogSubBorderB1->ShowWindow(FALSE);
			m_NPCDialogSubBorderB2->ShowWindow(FALSE);
			m_NPCDialogMainTitleBorder->ShowWindow(TRUE);
			m_NPCDialogIMEBorder1->ShowWindow(FALSE);
			m_NPCDialogIMEBorder2->ShowWindow(FALSE);
			m_OrganizationNameIMEBorder1->ShowWindow(FALSE);
			m_OrganizationNameIMEBorder2->ShowWindow(FALSE);
			m_imageQuesLevelDisplay->ShowWindow(FALSE);
			m_CompleteNameInput->ShowWindow(FALSE);
			m_NampInputCancle->ShowWindow(FALSE);
			m_btnMiniStopEventInput->ShowWindow(TRUE);
			m_EventIMEBorder1->ShowWindow(FALSE);
			m_EventIMEBorder2->ShowWindow(FALSE);
			
			m_MiniStopEventIMEBorder1->ShowWindow(TRUE);
			m_MiniStopEventIMEBorder2->ShowWindow(TRUE);
			m_MiniStopEventIMEControl.MoveWindow(m_WindowPosition.x+50, m_WindowPosition.y + 345);
			m_MiniStopEventIMEControl.ResizeWindow(162, 20);
			m_MiniStopEventIMEControl.SetMaxLength(64);
		
			m_MiniStopEventIMEControl.ShowWindow(TRUE);
			m_MiniStopEventIMEControl.SetFocus();
		}
		break;
	}
}
void _XWindow_NPCScript::SetHaveQuestID(int questid)
{
	m_HaveQuestID = questid;
	if( questid == -1 ) return;

	int i = 0;
	m_listboxQuestReward->DeleteAllItem();
	_XQuestScriptItem* currentquest = NULL;
	
	if( questid >= 10000 && questid < 20000  ) //제룡행일때 
	{
		m_imageQuesLevelDisplay->SetClipRect(238, 81, 251, 97);
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
		{
			currentquest = iter_quest->second;			
			if(currentquest)
			{
				m_QuestType = currentquest->GetQuestType();
				memset( m_HaveQuestType, 0, sizeof(TCHAR)*128 );
				strcpy( m_HaveQuestType, _XGETINTERFACETEXT(ID_STRING_QUEST_QUESTTYPE1) );	// _T("제룡행")

				memset( m_HaveQuestName, 0, sizeof(TCHAR)*256 );
				strcpy( m_HaveQuestName, currentquest->GetQuestName() );


				// 퀘스트 보상물을 표시하는 부분( 옵션 조건식에서 정보를 읽어온다.)
				if( !currentquest->m_mapOptionConditionList.empty() )
				{
					int	imageiconindex = -1;
					RECT rect = {0, 0, 0, 0};
					int rewarditemcount = 0;

					int rewardcompleteitemcount = 0;//완료시 획득하는 아이템의 갯수
					int completeitemcategory[4];
					int	completeitemid[4];
					for( i = 0; i < 4; i++)
					{
						completeitemcategory[i] = -1;
						completeitemid[i] = -1;
					}

					map <int, _XTriggerConditionItem*>::iterator iter_optioncondition;
					for(iter_optioncondition = currentquest->m_mapOptionConditionList.begin() ; iter_optioncondition != currentquest->m_mapOptionConditionList.end() ; iter_optioncondition++)
					{
						_XTriggerConditionItem* currentcondition = NULL;
						currentcondition = iter_optioncondition->second;
						if(currentcondition)
						{
							if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_DISPLAYREWARD )
							{
								if( currentcondition->m_TDC.m_compareclass2 == 1 ) //선택후 획득
								{
									int category = currentcondition->m_TDC.m_compareclass1;
									int itemid = currentcondition->m_TDC.m_numericvalue1;									
									
									g_pInventory_Window->GetIconRect(category, itemid, imageiconindex, rect, TRUE);
									
									DWORD commandid = (DWORD)category<<16 | itemid;
									
									int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );				
									RECT rect1 = {214, 32, 246, 64};	
									m_listboxQuestReward->InsertListItem(_T(""), 0, commandid);
									m_listboxQuestReward->SetIconArchive(rewarditemcount, &g_MainInterfaceTextureArchive);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 0, imageiconindex, rect);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 1, inventoryimageindex, rect1);
									rewarditemcount++;
								}
								else if(currentcondition->m_TDC.m_compareclass2 == 0) //완료시 획득 
								{						
									if( rewardcompleteitemcount < 4)
									{
										completeitemcategory[rewardcompleteitemcount] = currentcondition->m_TDC.m_compareclass1;
										completeitemid[rewardcompleteitemcount] = currentcondition->m_TDC.m_numericvalue1;	
										rewardcompleteitemcount++;
									}									
								}
							}							
						}	
					}
					
					// 선택시 획득하는 보상물 표시가 끝나면 완료시 획득하는 보상물을 표시한다.
					if( rewardcompleteitemcount != 0 )
					{
						int npcresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
						if( rewarditemcount == 0 )
						{
							for( i = 0; i < rewardcompleteitemcount; i++)
							{
								if( i == 0)
								{
									int category = completeitemcategory[i];
									int itemid = completeitemid[i];
									
									g_pInventory_Window->GetIconRect(category, itemid, imageiconindex, rect, TRUE);
									
									DWORD commandid = (DWORD)category<<16 | itemid;
									
									int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );				
									RECT rect1 = {214, 32, 246, 64};	
									m_listboxQuestReward->InsertListItem(_T(""), 0, commandid);
									m_listboxQuestReward->SetIconArchive(rewarditemcount, &g_MainInterfaceTextureArchive);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 0, imageiconindex, rect);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 1, inventoryimageindex, rect1);
									rewarditemcount++;
								}
								else
								{
									// +표시를 하고 
									RECT plusrect = { 453, 398, 483, 428};
									m_listboxQuestReward->InsertListItem(_T(""), 0, 0);
									m_listboxQuestReward->SetIconArchive(rewarditemcount, &g_MainInterfaceTextureArchive);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 0, npcresourceindex1, plusrect);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 1, -1, plusrect);
									rewarditemcount++;

									// 아이템을 표시한다.
									int category = completeitemcategory[i];
									int itemid = completeitemid[i];
									
									g_pInventory_Window->GetIconRect(category, itemid, imageiconindex, rect, TRUE);
									
									DWORD commandid = (DWORD)category<<16 | itemid;
									
									int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );				
									RECT rect1 = {214, 32, 246, 64};	
									m_listboxQuestReward->InsertListItem(_T(""), 0, commandid);
									m_listboxQuestReward->SetIconArchive(rewarditemcount, &g_MainInterfaceTextureArchive);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 0, imageiconindex, rect);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 1, inventoryimageindex, rect1);
									rewarditemcount++;
								}
							}
						}
						else
						{							
							for( i = 0; i < rewardcompleteitemcount; i++)
							{
								// +표시를 하고 
								RECT plusrect = { 453, 398, 483, 428};
								m_listboxQuestReward->InsertListItem(_T(""), 0, 0);
								m_listboxQuestReward->SetIconArchive(rewarditemcount, &g_MainInterfaceTextureArchive);
								m_listboxQuestReward->SetItemAttrib(rewarditemcount, 0, npcresourceindex1, plusrect);
								m_listboxQuestReward->SetItemAttrib(rewarditemcount, 1, -1, plusrect);
								rewarditemcount++;
								
								// 아이템을 표시한다.
								int category = completeitemcategory[i];
								int itemid = completeitemid[i];
								
								g_pInventory_Window->GetIconRect(category, itemid, imageiconindex, rect, TRUE);
								
								DWORD commandid = (DWORD)category<<16 | itemid;
								
								int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );				
								RECT rect1 = {214, 32, 246, 64};	
								m_listboxQuestReward->InsertListItem(_T(""), 0, commandid);
								m_listboxQuestReward->SetIconArchive(rewarditemcount, &g_MainInterfaceTextureArchive);
								m_listboxQuestReward->SetItemAttrib(rewarditemcount, 0, imageiconindex, rect);
								m_listboxQuestReward->SetItemAttrib(rewarditemcount, 1, inventoryimageindex, rect1);
								rewarditemcount++;
							}
						}
					}
				}
				if( m_listboxQuestReward->GetItemCount() != 0 )
					m_listboxQuestReward->ShowWindow(TRUE);

			}
		}
	}
	else//제룡행이 아닐때 
	{					
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
		{
			currentquest = iter_quest->second;
			if(currentquest)
			{					
				
				if( currentquest->GetQuestID() >= 9023 && currentquest->GetQuestID() <= 9028 )
				{
					m_QuestType = _XQUEST_TYPE_2;
				}
				else
				{	
					m_QuestType = currentquest->GetQuestType();
				}
				
				switch( m_QuestType ) {
				case _XQUEST_TYPE_2://무림행 
					{
						m_imageQuesLevelDisplay->SetClipRect(238, 3, 251, 19);
						memset( m_HaveQuestType, 0, sizeof(TCHAR)*128 );
						strcpy( m_HaveQuestType, _XGETINTERFACETEXT(ID_STRING_QUEST_QUESTTYPE2) );	// _T("무림행")
					}
					break;
				case _XQUEST_TYPE_3://협객행
					{
						m_imageQuesLevelDisplay->SetClipRect(238, 21, 251, 37);
						memset( m_HaveQuestType, 0, sizeof(TCHAR)*128 );
						strcpy( m_HaveQuestType, _XGETINTERFACETEXT(ID_STRING_QUEST_QUESTTYPE3) );	// _T("협객행")
					}
					break;
				case _XQUEST_TYPE_4://기연행 
					{
						m_imageQuesLevelDisplay->SetClipRect(238, 61, 251, 77);
						memset( m_HaveQuestType, 0, sizeof(TCHAR)*128 );
						strcpy( m_HaveQuestType, _XGETINTERFACETEXT(ID_STRING_QUEST_QUESTTYPE4) );	// _T("기연행")
					}
					break;
				case _XQUEST_TYPE_5://무림소사
					{
						m_imageQuesLevelDisplay->SetClipRect(238, 41, 251, 57);
						memset( m_HaveQuestType, 0, sizeof(TCHAR)*128 );
						strcpy( m_HaveQuestType, _XGETINTERFACETEXT(ID_STRING_QUEST_QUESTTYPE5) );	// _T("무림소사")
					}
					break;
				}		

				
				memset( m_HaveQuestName, 0, sizeof(TCHAR)*256 );
				strcpy( m_HaveQuestName, currentquest->GetQuestName() );
				

				// 퀘스트 보상물을 표시하는 부분( 옵션 조건식에서 정보를 읽어온다.)
				if( !currentquest->m_mapOptionConditionList.empty() )
				{					
					int	imageiconindex = -1;
					RECT rect = {0, 0, 0, 0};
					int rewarditemcount = 0;

					int rewardcompleteitemcount = 0;//완료시 획득하는 아이템의 갯수
					int completeitemcategory[4];
					int	completeitemid[4];
					for( i = 0; i < 4; i++)
					{
						completeitemcategory[i] = -1;
						completeitemid[i] = -1;
					}

					map <int, _XTriggerConditionItem*>::iterator iter_optioncondition;
					for(iter_optioncondition = currentquest->m_mapOptionConditionList.begin() ; iter_optioncondition != currentquest->m_mapOptionConditionList.end() ; iter_optioncondition++)
					{
						_XTriggerConditionItem* currentcondition = NULL;
						currentcondition = iter_optioncondition->second;
						if(currentcondition)
						{
							if( currentcondition->m_TDC.m_conditionclass == ID_CONDITION_DISPLAYREWARD )
							{
								if( currentcondition->m_TDC.m_compareclass2 == 1 ) //선택후 획득
								{
									int category = currentcondition->m_TDC.m_compareclass1;
									int itemid = currentcondition->m_TDC.m_numericvalue1;									
									
									g_pInventory_Window->GetIconRect(category, itemid, imageiconindex, rect, TRUE);
									
									DWORD commandid = (DWORD)category<<16 | itemid;
									
									int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );				
									RECT rect1 = {214, 32, 246, 64};	
									m_listboxQuestReward->InsertListItem(_T(""), 0, commandid);
									m_listboxQuestReward->SetIconArchive(rewarditemcount, &g_MainInterfaceTextureArchive);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 0, imageiconindex, rect);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 1, inventoryimageindex, rect1);
									rewarditemcount++;
								}
								else if(currentcondition->m_TDC.m_compareclass2 == 0) //완료시 획득 
								{						
									if( rewardcompleteitemcount < 4)
									{
										completeitemcategory[rewardcompleteitemcount] = currentcondition->m_TDC.m_compareclass1;
										completeitemid[rewardcompleteitemcount] = currentcondition->m_TDC.m_numericvalue1;	
										rewardcompleteitemcount++;
									}									
								}
							}							
						}	
					}
					
					// 선택시 획득하는 보상물 표시가 끝나면 완료시 획득하는 보상물을 표시한다.
					if( rewardcompleteitemcount != 0 )
					{
						int npcresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
						if( rewarditemcount == 0 )
						{
							for( i = 0; i < rewardcompleteitemcount; i++)
							{
								if( i == 0)
								{
									int category = completeitemcategory[i];
									int itemid = completeitemid[i];
									
									g_pInventory_Window->GetIconRect(category, itemid, imageiconindex, rect, TRUE);
									
									DWORD commandid = (DWORD)category<<16 | itemid;
									
									int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );				
									RECT rect1 = {214, 32, 246, 64};	
									m_listboxQuestReward->InsertListItem(_T(""), 0, commandid);
									m_listboxQuestReward->SetIconArchive(rewarditemcount, &g_MainInterfaceTextureArchive);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 0, imageiconindex, rect);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 1, inventoryimageindex, rect1);
									rewarditemcount++;
								}
								else
								{
									// +표시를 하고 
									RECT plusrect = { 453, 398, 483, 428};
									m_listboxQuestReward->InsertListItem(_T(""), 0, 0);
									m_listboxQuestReward->SetIconArchive(rewarditemcount, &g_MainInterfaceTextureArchive);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 0, npcresourceindex1, plusrect);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 1, -1, plusrect);
									rewarditemcount++;

									// 아이템을 표시한다.
									int category = completeitemcategory[i];
									int itemid = completeitemid[i];
									
									g_pInventory_Window->GetIconRect(category, itemid, imageiconindex, rect, TRUE);
									
									DWORD commandid = (DWORD)category<<16 | itemid;
									
									int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );				
									RECT rect1 = {214, 32, 246, 64};	
									m_listboxQuestReward->InsertListItem(_T(""), 0, commandid);
									m_listboxQuestReward->SetIconArchive(rewarditemcount, &g_MainInterfaceTextureArchive);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 0, imageiconindex, rect);
									m_listboxQuestReward->SetItemAttrib(rewarditemcount, 1, inventoryimageindex, rect1);
									rewarditemcount++;
								}
							}
						}
						else
						{							
							for( i = 0; i < rewardcompleteitemcount; i++)
							{
								// +표시를 하고 
								RECT plusrect = { 453, 398, 483, 428};
								m_listboxQuestReward->InsertListItem(_T(""), 0, 0);
								m_listboxQuestReward->SetIconArchive(rewarditemcount, &g_MainInterfaceTextureArchive);
								m_listboxQuestReward->SetItemAttrib(rewarditemcount, 0, npcresourceindex1, plusrect);
								m_listboxQuestReward->SetItemAttrib(rewarditemcount, 1, -1, plusrect);
								rewarditemcount++;
								
								// 아이템을 표시한다.
								int category = completeitemcategory[i];
								int itemid = completeitemid[i];
								
								g_pInventory_Window->GetIconRect(category, itemid, imageiconindex, rect, TRUE);
								
								DWORD commandid = (DWORD)category<<16 | itemid;
								
								int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );				
								RECT rect1 = {214, 32, 246, 64};	
								m_listboxQuestReward->InsertListItem(_T(""), 0, commandid);
								m_listboxQuestReward->SetIconArchive(rewarditemcount, &g_MainInterfaceTextureArchive);
								m_listboxQuestReward->SetItemAttrib(rewarditemcount, 0, imageiconindex, rect);
								m_listboxQuestReward->SetItemAttrib(rewarditemcount, 1, inventoryimageindex, rect1);
								rewarditemcount++;
							}
						}
					}

					if( m_listboxQuestReward->GetItemCount() != 0 )
						m_listboxQuestReward->ShowWindow(TRUE);
				}
			}
		}
	}
}

BOOL _XWindow_NPCScript::CheckQuestStringEditBoxArea(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 45) &&
		scrnpos->x < (m_WindowPosition.x + 45 + 340) &&
		scrnpos->z > (m_WindowPosition.y + 340) &&
		scrnpos->z < (m_WindowPosition.y + 340 + 20))
		return TRUE;
	
	return FALSE;
}

BOOL _XWindow_NPCScript::CheckOrganizationEditBoxArea(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	// 해외 버전일 때 
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		// 영역 추후 수정
		if(scrnpos->x > (m_WindowPosition.x + 138) &&
			scrnpos->x < (m_WindowPosition.x + 138 + 150) &&
			scrnpos->z > (m_WindowPosition.y + 325) &&
			scrnpos->z < (m_WindowPosition.y + 325 + 17))
			return TRUE;
	
	}
	else
	{
		// 영역 추후 수정
		if(scrnpos->x > (m_WindowPosition.x + 190) &&
			scrnpos->x < (m_WindowPosition.x + 190 + 90) &&
			scrnpos->z > (m_WindowPosition.y + 325) &&
			scrnpos->z < (m_WindowPosition.y + 325 + 20))
			return TRUE;
	
	}
	return FALSE;
}
BOOL _XWindow_NPCScript::CheckEventEditBoxArea(void)
{	

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 44) &&
		scrnpos->x < (m_WindowPosition.x + 44 + 165) &&
		scrnpos->z > (m_WindowPosition.y + 340) &&
		scrnpos->z < (m_WindowPosition.y + 340 + 20))
		return TRUE;
	
	return FALSE;
}
BOOL _XWindow_NPCScript::CheckEvent01EditBoxArea(void)
{
	if( m_MessageType == _XDEF_NPCMESSAGE_TRACING ) return FALSE;
	if( m_MessageType == _XDEF_NPCMESSAGE_MASTERNAME ) return FALSE; //Author : 양희왕 //breif : 사제 

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 44) &&
		scrnpos->x < (m_WindowPosition.x + 44 + 44) &&
		scrnpos->z > (m_WindowPosition.y + 340) &&
		scrnpos->z < (m_WindowPosition.y + 340 + 20))
		return TRUE;
	
	return FALSE;
}
BOOL _XWindow_NPCScript::CheckEvent02EditBoxArea(void)
{
	if( m_MessageType == _XDEF_NPCMESSAGE_TRACING ) return FALSE;
	if( m_MessageType == _XDEF_NPCMESSAGE_MASTERNAME ) return FALSE; //Author : 양희왕 //breif : 사제 

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 94) &&
		scrnpos->x < (m_WindowPosition.x + 94 + 44) &&
		scrnpos->z > (m_WindowPosition.y + 340) &&
		scrnpos->z < (m_WindowPosition.y + 340 + 20))
		return TRUE;
	
	return FALSE;
}
BOOL _XWindow_NPCScript::CheckEvent03EditBoxArea(void)
{
	if( m_MessageType == _XDEF_NPCMESSAGE_TRACING ) return FALSE;
	if( m_MessageType == _XDEF_NPCMESSAGE_MASTERNAME ) return FALSE; //Author : 양희왕 //breif : 사제 

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 144) &&
		scrnpos->x < (m_WindowPosition.x + 144 + 44) &&
		scrnpos->z > (m_WindowPosition.y + 340) &&
		scrnpos->z < (m_WindowPosition.y + 340 + 20))
		return TRUE;
	
	return FALSE;
}

void _XWindow_NPCScript::SetMiniMapWindow(_XWindow_WorldMinimap* minimapwindow)
{
	m_pMiniMapWindowPtr = minimapwindow;	
}
void _XWindow_NPCScript::SetSavePointPosition( int npcid)
{
	float RateOfLargeMapSizeToTerrain	= 300.0f / g_LodTerrain.m_Width;
	for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; i++)
	{		
		if( g_Npclist.m_pNPCObjectArray[i].m_UniqueID == npcid )
		{
			float x = g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._41 * RateOfLargeMapSizeToTerrain + 150.0f;
			float z = -g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._43 * RateOfLargeMapSizeToTerrain - 150.0f;
			
			if( g_Npclist.m_pNPCObjectArray[i].m_bShowNPCModel )
			{
				m_SavePointPositionX = 645 + x;
				m_SavePointPositionY = 500 + z;				
			}
			break; 
		}
	}
}

void _XWindow_NPCScript::ClickOrganizationBtn(void)
{
	// 영웅단창설
	if( g_pLocalUser->m_CharacterInfo.Get_iContribution() >= 5000 )
	{
		BOOL check = FALSE;
		int questid = 0;

		switch(g_pLocalUser->m_CharacterInfo.groupindex) 
		{									
		case _XGROUP_NANGIN :
			break;
		case _XGROUP_GAEBANG:
			questid = 21067;
			break;
		case _XGROUP_BEEGOONG :
			questid = 22067;
			break;
		case _XGROUP_SORIM :
			questid = 23067;
			break;
		case _XGROUP_NOCKRIM :
			questid = 24067;
			break;
		case _XGROUP_MOODANG :
			questid = 25027;
			break;
		case _XGROUP_MAKYO :
			questid = 26027;
			break;
		case _XGROUP_SEGA :
			break;
		}

		if( !g_QuestScriptManager.CheckCompleteQuest(questid) )
		{
			// 퀘스트 미완료
			TCHAR message[256];
			memset(message, 0, 256);
			sprintf( message, _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ERROR4), g_QuestScriptManager.GetQuestName(questid));
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE);
			return;
		}

		if( g_pLocalUser->m_CharacterInfo.Get_level() >= 5 )	// 주천 이상이면
		{
			if( g_pLocalUser->m_PartyMemberCount >= 5 )
			{										
				bool checkleader = FALSE;
				for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++)
				{
					if( g_pLocalUser->m_PartyInfo[i].isLeader )
					{
						if( strcmp(g_pLocalUser->m_PartyInfo[i].playername, g_pLocalUser->m_CharacterInfo.charactername) == 0 )
						{
							checkleader = TRUE;
						}
						break;
					}
				}
				if( checkleader )
				{
					bool checksamezone = TRUE;
					for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++)
					{
						if( g_pLocalUser->m_PartyInfo[i].serverno > 0 )
						{
							if( g_CurrentZoneInfoIndex != g_pLocalUser->m_PartyInfo[i].serverno )
							{
								checksamezone = FALSE;
								break;
							}
						}						
					}
					if( checksamezone )
					{
						// 마지막 단계 서버에게 요청해서 검색
						g_pGroup_Window->SetSendTargetName(g_pLocalUser->m_CharacterInfo.charactername);
						g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_CLIENT, MSG_ORGANIZATION_CLIENT_::_OR_REQ_MOONPA_CHECK_);
					}
					else
					{
						// 같은 존 안에 없음
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ERROR1), TRUE);	// _T("이 곳에 없는 동행원이 있습니다.")
					}
				}
				else
				{
					// 행수가 아님 
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ERROR2), TRUE);	// _T("행수만 영웅단을 창설할 수 있습니다.")
				}
			}
			else
			{
				// 동행원 부족
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ERROR3), TRUE);	// _T("동행원이 5명 이상이 되어야 합니다.")
			}
		}
		else
		{
			// 임시			
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1660), TRUE);
		}
	}
	else
	{
		// 문파기여도 부족
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_2275), TRUE);
	}
}

void _XWindow_NPCScript::SetQuestListItem(void)
{
    //Author : 양희왕 //breif : 무림소사는 네글자라서 한칸 더 띄어줘야한다. //좋은 방법이 없을까 -_-
	POINT columnoffset2 = { 20, 6 };
	POINT columnoffset3 = { 70, 6 };

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		columnoffset3.x = 140;
	}
	
#ifdef _XDEF_SHUTDOWN_20061027
	// 플레이 가능 시간이 지나면 퀘스트를 진행 할 수 없다.
	if( g_PlayLeftTime <= 0 ) return;// g_PlayLeftTime가 0이 아니면 진행
#endif
	if( !g_LastNPCScriptOwner ) return;
	m_listboxHaveQuestList->DeleteAllItem();	
	
	// NPC가 가지고 있는 퀘스트들을 리스트에 넣는다.
	if( !g_QuestScriptManager.m_listNPCTargettingQuest.empty() )
	{
		int count = 0;
		bool balreadyexist = false;
		list <_XQUEST_QUESTLIST>::iterator iter_npctargetting;
		for(iter_npctargetting = g_QuestScriptManager.m_listNPCTargettingQuest.begin() ; iter_npctargetting != g_QuestScriptManager.m_listNPCTargettingQuest.end() ;iter_npctargetting++ )
		{
			_XQUEST_QUESTLIST npctargetting = *iter_npctargetting;
			// 퀘스트 리스트에서 npcid가 동일 할 때만 리스트를 보여준다.
			if( g_LastNPCScriptOwner->m_UniqueID  == npctargetting.npcid )
			{
				
				_XQuestScriptItem* currentquest = NULL;
				if( npctargetting.questid >= 10000 && npctargetting.questid < 20000  )
				{
					map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(npctargetting.questid);
					if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
						currentquest = iter_quest->second;
				}
				else
				{
					map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(npctargetting.questid);
					if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
						currentquest = iter_quest->second;
				}
				
				if( currentquest )
				{
					m_listboxHaveQuestList->InsertListItem(_T(""), 0, npctargetting.questid+1);		
					
					
					if( npctargetting.questid >= 10000 && npctargetting.questid < 20000 )
					{
                        //Author : 양희왕 //breif : 무림소사는 네글자라서 한칸 더 띄어줘야한다.
                        m_listboxHaveQuestList->SetColumnOffset(1, columnoffset2);
	                    m_listboxHaveQuestList->SetColumnOffset(2, columnoffset3);

						int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
						RECT rect = {238, 81, 251, 97}; //노란색 두루마기 이미지 
						
						m_listboxHaveQuestList->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxHaveQuestList->SetItemAttrib(count, 0, questresourceindex2, rect);
						
						m_listboxHaveQuestList->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_QUESTTYPE1) ); 
						m_listboxHaveQuestList->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
						m_listboxHaveQuestList->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
						
					}
					else if(npctargetting.questid >= 20000 && npctargetting.questid < 30000)
					{
                        //Author : 양희왕 //breif : 무림소사는 네글자라서 한칸 더 띄어줘야한다.
                        m_listboxHaveQuestList->SetColumnOffset(1, columnoffset2);
	                    m_listboxHaveQuestList->SetColumnOffset(2, columnoffset3);

						int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
						RECT rect = {238, 3, 251, 19}; //파란색 두루마기 이미지 							
						
						m_listboxHaveQuestList->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxHaveQuestList->SetItemAttrib(count, 0, questresourceindex2, rect);
						
						m_listboxHaveQuestList->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_QUESTTYPE2) ); 
						m_listboxHaveQuestList->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
						m_listboxHaveQuestList->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
					}
					else if(npctargetting.questid >= 30000 && npctargetting.questid < 40000)
					{
                        //Author : 양희왕 //breif : 무림소사는 네글자라서 한칸 더 띄어줘야한다.
                        m_listboxHaveQuestList->SetColumnOffset(1, columnoffset2);
	                    m_listboxHaveQuestList->SetColumnOffset(2, columnoffset3);

						int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
						RECT rect = {238, 21, 251, 37}; //주황색 두루마기 이미지 							
						
						m_listboxHaveQuestList->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxHaveQuestList->SetItemAttrib(count, 0, questresourceindex2, rect);
						
						m_listboxHaveQuestList->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_QUESTTYPE3) ); 
						m_listboxHaveQuestList->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
						m_listboxHaveQuestList->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
					}
					else if(npctargetting.questid >= 40000 && npctargetting.questid < 50000)
					{
                        //Author : 양희왕 //breif : 무림소사는 네글자라서 한칸 더 띄어줘야한다.
                        m_listboxHaveQuestList->SetColumnOffset(1, columnoffset2);
	                    m_listboxHaveQuestList->SetColumnOffset(2, columnoffset3);

						int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
						RECT rect = {238, 61, 251, 77}; //녹색 두루마기 이미지 							
						
						m_listboxHaveQuestList->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxHaveQuestList->SetItemAttrib(count, 0, questresourceindex2, rect);
						
						m_listboxHaveQuestList->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_QUESTTYPE4) ); 
						m_listboxHaveQuestList->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
						m_listboxHaveQuestList->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
					}
					else if(npctargetting.questid >= 50000 && npctargetting.questid < 60000)
					{
                        //Author : 양희왕 //breif : 무림소사는 네글자라서 한칸 더 띄어줘야한다.
                        if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	                    {
		                    columnoffset3.x = 140;
	                    }
                        else
                            columnoffset3.x = 83;

                        m_listboxHaveQuestList->SetColumnOffset(1, columnoffset2);
	                    m_listboxHaveQuestList->SetColumnOffset(2, columnoffset3);

						int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
						RECT rect = {238, 41, 251, 57}; //흰색 두루마기 이미지 							
						
						m_listboxHaveQuestList->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxHaveQuestList->SetItemAttrib(count, 0, questresourceindex2, rect);
						
						m_listboxHaveQuestList->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_QUESTTYPE5) ); 
						m_listboxHaveQuestList->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
						m_listboxHaveQuestList->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
					}
					else if( npctargetting.questid >= 9023 && npctargetting.questid <= 9028 )
					{
                        //Author : 양희왕 //breif : 무림소사는 네글자라서 한칸 더 띄어줘야한다.
                        m_listboxHaveQuestList->SetColumnOffset(1, columnoffset2);
	                    m_listboxHaveQuestList->SetColumnOffset(2, columnoffset3);

						// 튜토리얼 퀘스트는 특별히 무림행으로 표시한다.
						int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
						RECT rect = {238, 3, 251, 19}; //파란색 두루마기 이미지 							
						
						m_listboxHaveQuestList->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxHaveQuestList->SetItemAttrib(count, 0, questresourceindex2, rect);
						
						m_listboxHaveQuestList->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_QUESTTYPE2) ); 
						m_listboxHaveQuestList->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
						m_listboxHaveQuestList->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
					}
					
					m_listboxHaveQuestList->SetItemText(count, 2, currentquest->GetQuestName() );
					m_listboxHaveQuestList->SetItemAttrib(count, 2, D3DCOLOR_ARGB(255, 0, 0, 0));
					m_listboxHaveQuestList->SetItemAttribHighlight(count, 2, D3DCOLOR_ARGB(255, 255, 255, 255));									
					
					count++;						
				}
			}
			
		}
	}

}