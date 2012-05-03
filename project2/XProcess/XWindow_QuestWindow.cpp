// XWindow_QuestWindow.cpp: implementation of the _XWindow_QuestWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindow_QuestWindow.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XQuestScriptItemGroup.h"
#include "XQuestScriptItem.h"
#include "XWindowObjectDefine.h"

BOOL __stdcall _XCallback_QuestWindow_MouseHover(int commandid, int itemid, int XPos, int YPos)
{
	if( itemid <= 0 ) return FALSE;	

//	_XWindow_QuestWindow* pQuest_Window = (_XWindow_QuestWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUESTWINDOW);
//	if(pQuest_Window)
//	{
//		pQuest_Window->m_QuestItemTooltip->SetToolTipString(XPos+13, YPos-15, g_QuestItemProperty[itemid].cName, _XSC_INFORMATION, TRUE);
//		pQuest_Window->m_QuestItemTooltip->Draw();
//		return TRUE;
//	}

	
	return FALSE;
}

void __stdcall _XCallback_QuestTabButton( POINT pos, SIZE size )
{
	if( !g_pQuest_Window ) return;

	int quest2count = 0;
	int quest3count = 0;
	int quest4count = 0;
	int quest5count = 0;
	
	for ( int i = 0; i < 5; i++ )
	{
		if( g_QuestScriptManager.m_ProgressSubQuestID[i] != -1 )
		{
			if( g_QuestScriptManager.m_ProgressSubQuestID[i] >= 20000 && g_QuestScriptManager.m_ProgressSubQuestID[i] < 30000 )//무림행	
			{		
				quest2count++;
			}
			else if( g_QuestScriptManager.m_ProgressSubQuestID[i] >= 30000 && g_QuestScriptManager.m_ProgressSubQuestID[i] < 40000 )//협객행
			{
				quest3count++;
			}
			else if( g_QuestScriptManager.m_ProgressSubQuestID[i] >= 40000 && g_QuestScriptManager.m_ProgressSubQuestID[i] < 50000 )//기연행
			{		
				quest4count++;		
			}
		}
	}
	
	if( !g_QuestScriptManager.m_listQuestNoSaveType.empty() )
	{
		list <int>::iterator iter_questnosavetype;
		for(iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.begin() ; iter_questnosavetype != g_QuestScriptManager.m_listQuestNoSaveType.end() ; iter_questnosavetype++)
		{
			int questid = *iter_questnosavetype;
			if( questid >= 50000 && questid < 60000 )
			{
				quest5count++;
			}
		}
	}

	int tooltipxpos = g_pQuest_Window->GetWindowPos().x + g_pQuest_Window->GetWindowSize().cx + 4;
	int tooltipypos = g_pQuest_Window->GetWindowPos().y + 10;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		_XDrawSolidBar( tooltipxpos+1, tooltipypos+1, tooltipxpos + 131, tooltipypos + 69, 0x7F000000 );
		_XDrawRoundRectAngle( tooltipxpos, tooltipypos, tooltipxpos + 130, tooltipypos+68, 0.0f, D3DCOLOR_ARGB(255,221,221,174) );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		_XDrawSolidBar( tooltipxpos+1, tooltipypos+1, tooltipxpos + 171, tooltipypos + 69, 0x7F000000 );
		_XDrawRoundRectAngle( tooltipxpos, tooltipypos, tooltipxpos + 170, tooltipypos+68, 0.0f, D3DCOLOR_ARGB(255,221,221,174) );
	}

	if( g_pQuest_Window->m_SelectedTab == _XQUESTTAB_QUEST1 ) g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	else g_XBaseFont->SetColor( _XSC_DEFAULT );
	if( g_QuestScriptManager.m_bProgressMainQuest )
	{
		g_XBaseFont->Puts( tooltipxpos + 6, tooltipypos + 2, _XGETINTERFACETEXT( ID_STRING_QUEST_TYPE1COUNT1 ) );
	}
	else
	{
		g_XBaseFont->Puts( tooltipxpos + 6, tooltipypos + 2, _XGETINTERFACETEXT( ID_STRING_QUEST_TYPE1COUNT0 ) );
	}
	
	TCHAR	questtabnamebuffer[64];

	memset(questtabnamebuffer, 0, sizeof(TCHAR) * 64);
	sprintf( questtabnamebuffer, _XGETINTERFACETEXT(ID_STRING_QUEST_TYPE2COUNT),quest2count );	// "무림행(%d)"
	if( g_pQuest_Window->m_SelectedTab == _XQUESTTAB_QUEST2 ) g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	else g_XBaseFont->SetColor( _XSC_DEFAULT );
	g_XBaseFont->Puts( tooltipxpos + 6, tooltipypos + 15, questtabnamebuffer );
	
	
	memset(questtabnamebuffer, 0, sizeof(TCHAR) * 64);
	sprintf( questtabnamebuffer, _XGETINTERFACETEXT(ID_STRING_QUEST_TYPE3COUNT),quest3count );	// "협객행(%d)"	
	if( g_pQuest_Window->m_SelectedTab == _XQUESTTAB_QUEST3 ) g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	else g_XBaseFont->SetColor( _XSC_DEFAULT );
	g_XBaseFont->Puts( tooltipxpos + 6, tooltipypos + 28, questtabnamebuffer );
	
	memset(questtabnamebuffer, 0, sizeof(TCHAR) * 64);
	sprintf( questtabnamebuffer, _XGETINTERFACETEXT(ID_STRING_QUEST_TYPE4COUNT),quest4count );	// "기연행(%d)"	
	if( g_pQuest_Window->m_SelectedTab == _XQUESTTAB_QUEST4 ) g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	else g_XBaseFont->SetColor( _XSC_DEFAULT );
	g_XBaseFont->Puts( tooltipxpos + 6, tooltipypos + 41, questtabnamebuffer );
	
	memset(questtabnamebuffer, 0, sizeof(TCHAR) * 64);
	sprintf( questtabnamebuffer, _XGETINTERFACETEXT(ID_STRING_QUEST_TYPE5COUNT),quest5count );	// "무림소사(%d)"	
	if( g_pQuest_Window->m_SelectedTab == _XQUESTTAB_QUEST5 ) g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	else g_XBaseFont->SetColor( _XSC_DEFAULT );
	g_XBaseFont->Puts( tooltipxpos + 6, tooltipypos + 54, questtabnamebuffer );

	g_XBaseFont->Flush();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_QuestWindow::_XWindow_QuestWindow()
{	
	
	// 퀘스트 이름 바탕 이미지 
	m_QuestNameBorder1 = NULL;
	m_QuestNameBorder2 = NULL;
	
	// 퀘스트 정보 바탕 이미지 
	m_QuestInfomationBorder1 = NULL;
	m_QuestInfomationBorder2 = NULL;
	m_QuestInfomationBorder3 = NULL;
		
	// 퀘스트 완료 도장 이미지 
	m_QuestCompleteIconImage = NULL;
	
	m_QuestGroupNumber = 0;
	m_GetTotalQuestCount = 0;

	memset( m_SelectedQuestName, 0, sizeof(TCHAR)*512 );
	memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
	memset( m_TempSelectedQuestSubName, 0, sizeof(TCHAR)*512 );
	m_lpszQuestProcessInfomation = NULL;
	m_lpszQuestOutline = NULL;

	m_SelectedTab = _XQUESTTAB_QUEST1;
	m_SelectedItemIndex = -1;
	m_PrevSelectedItemIndex = -1;
	m_SelectedQuestID = -1;
	m_TotalCountQuestListIndex = 0;

	for(int i = 0; i < _XQUESTTAB_TOTALCOUNT; i++ )
	{			
		m_QuestTab[i] = NULL;
	}
	m_listboxQuestScriptItem = NULL;
	m_listboxQuestDesc = NULL;
	m_listboxQuestReward = NULL;	
	
	m_indexProgressQuestGroup		= -1;
	m_indexCompleteQuestGroup		= -1;
	m_bSelectedProgressQuestGroup	= FALSE;
	m_bSelectedCompleteQuestGroup	= FALSE;
	m_bOverFlowQuestSubName			= FALSE;
	
}

_XWindow_QuestWindow::~_XWindow_QuestWindow()
{
	m_QuestGroupList.disposeList();
	DestroyWindow();
}

BOOL _XWindow_QuestWindow::Initialize( void )
{
	
	int npcresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
	int npcresourceindex3 = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_03.tga" );
	int questresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_quest1.tga" );
	int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
	
	int	szButtonstring[5];	
	szButtonstring[0] = ID_STRING_QUEST_QUESTTYPE1;
	szButtonstring[1] = ID_STRING_QUEST_QUESTTYPE2;
	szButtonstring[2] = ID_STRING_QUEST_QUESTTYPE3;
	szButtonstring[3] = ID_STRING_QUEST_QUESTTYPE4;
	szButtonstring[4] = ID_STRING_QUEST_QUESTTYPE5;

	int skillwinimageindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {(m_WindowSize.cx>>1) - (165>>1) - 14, 10}, {10, 21}, _XDEF_QW_QUESTTAB_PREV,
			skillwinimageindex,
			skillwinimageindex,
			skillwinimageindex,
			&g_MainInterfaceTextureArchive
	};
	m_LeftButton = new _XButton;
	m_LeftButton->Create(btnstruct);
	m_LeftButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 195, 52, 205, 73);
	m_LeftButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 195, 52, 205, 73);
	m_LeftButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 215, 52, 225, 73);
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_LeftButton->ShowWindow(TRUE);
	}
	else
	{
		m_LeftButton->ShowWindow(FALSE);
	}
	InsertChildObject(m_LeftButton);
	
	btnstruct.position.x = (m_WindowSize.cx>>1) + (165>>1) + 4;
	btnstruct.position.y = 10;
	btnstruct.commandid = _XDEF_QW_QUESTTAB_NEXT;
	m_RightButton = new _XButton;
	m_RightButton->Create(btnstruct);
	m_RightButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 196, 95, 206, 116);
	m_RightButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 196, 95, 206, 116);
	m_RightButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 216, 95, 226, 116);
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_RightButton->ShowWindow(TRUE);
	}
	else
	{
		m_RightButton->ShowWindow(FALSE);
	}

	InsertChildObject(m_RightButton);
	
	// 퀘스트 종류 탭
	_XCHECKBTN_STRUCTURE questtabbtnstruct = 
	{   
		TRUE, { 4, 9 }, { 66, 26 }, 
			_XDEF_QW_QUESTTABBUTTON01, questresourceindex2,	questresourceindex2, 
			&g_MainInterfaceTextureArchive 
	};	
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		questtabbtnstruct.position.x = (m_WindowSize.cx>>1) - (165>>1);
		questtabbtnstruct.windowsize.cx = 165;

		int npcresourceindex3			= g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_03.tga" );
		questtabbtnstruct.imageindex_checked = npcresourceindex3;
		questtabbtnstruct.imageindex_normal = npcresourceindex3;

		for( int i = 0; i < _XQUESTTAB_TOTALCOUNT; i++ )
		{
			m_QuestTab[i] = new _XCheckButton;
			m_QuestTab[i]->Create( questtabbtnstruct );
			m_QuestTab[i]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, szButtonstring[i] );
			m_QuestTab[i]->SetGrowTextMode( TRUE, 0xFF000000 );
			m_QuestTab[i]->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
			
			m_QuestTab[i]->SetButtonImageClipRect(   _XCHECKBUTTONIMAGE_NORMAL, 91, 208, 256, 230 );
			m_QuestTab[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 91, 233, 256, 255 );
			
			m_QuestTab[i]->SetCheck( FALSE );
			m_QuestTab[i]->EnableWindow(TRUE);

			m_QuestTab[i]->ShowWindow( FALSE );

			m_QuestTab[i]->SetCallBackFunction_ToolTip( _XCallback_QuestTabButton );

			InsertChildObject( m_QuestTab[i] );
			
			questtabbtnstruct.commandid++;
		}

		m_QuestTab[0]->ShowWindow( TRUE );
	}
	else
	{
		for( int i = 0; i < _XQUESTTAB_TOTALCOUNT-1; i++ ) //무림소사 탭버튼이미지는 다른 이미지이므로 뺀다. 
		{			
			questtabbtnstruct.position.x = 4+(i*65);
			m_QuestTab[i] = new _XCheckButton;
			m_QuestTab[i]->Create( questtabbtnstruct );		
			m_QuestTab[i]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, szButtonstring[i] );
			m_QuestTab[i]->SetGrowTextMode( TRUE, 0xFF000000 );
			m_QuestTab[i]->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
		
			m_QuestTab[i]->SetButtonImageClipRect(   _XCHECKBUTTONIMAGE_NORMAL, 1, 229, 67, 255 );
			m_QuestTab[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 68, 229, 134, 255 );
			
			m_QuestTab[i]->SetCheck( FALSE );
			m_QuestTab[i]->EnableWindow(TRUE);
			InsertChildObject( m_QuestTab[i] );
			
			questtabbtnstruct.commandid++;
		}
		questtabbtnstruct.windowsize.cx = 80; 
		//questtabbtnstruct.windowsize.cy = 28; 
		questtabbtnstruct.position.x = 4+(4*65);
		//questtabbtnstruct.position.y = 11;
		
		m_QuestTab[4] = new _XCheckButton;
		m_QuestTab[4]->Create( questtabbtnstruct );
		m_QuestTab[4]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_QUEST_QUESTTYPE5 );	// _T("무림소사")
		m_QuestTab[4]->SetGrowTextMode( TRUE, 0xFF000000 );
		m_QuestTab[4]->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
		
		m_QuestTab[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 80, 190, 160, 218 );
		m_QuestTab[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 0, 188, 80, 216 );
		m_QuestTab[4]->SetCheck( FALSE );
		m_QuestTab[4]->EnableWindow(TRUE);
		InsertChildObject( m_QuestTab[4] );
	}
	
	m_QuestTab[0]->SetCheck( TRUE );
	
	
	// 퀘스트 정보 바탕 이미지 
	m_QuestInfomationBorder1 = new _XImageStatic;
	m_QuestInfomationBorder1->Create( 5, 148, 223, 382,
		&g_MainInterfaceTextureArchive, questresourceindex1 );
	m_QuestInfomationBorder1->SetClipRect(0, 0, 218, 234);
	m_QuestInfomationBorder1->ShowWindow(TRUE);
	InsertChildObject( m_QuestInfomationBorder1 );
	
	m_QuestInfomationBorder2 = new _XImageStatic;
	m_QuestInfomationBorder2->Create( 223, 148, 327, 265,
		&g_MainInterfaceTextureArchive, questresourceindex2 );
	m_QuestInfomationBorder2->SetClipRect(1, 0, 105, 117);
	m_QuestInfomationBorder2->ShowWindow(TRUE);
	InsertChildObject( m_QuestInfomationBorder2 );
	
	m_QuestInfomationBorder3 = new _XImageStatic;
	m_QuestInfomationBorder3->Create( 223, 265, 327, 382,
		&g_MainInterfaceTextureArchive, questresourceindex2 );
	m_QuestInfomationBorder3->SetClipRect(107, 0, 211, 117);
	m_QuestInfomationBorder3->ShowWindow(TRUE);
	InsertChildObject( m_QuestInfomationBorder3 );
		
	// 퀘스트 이름 바탕 이미지 
	m_QuestNameBorder1 = new _XImageStatic;
	m_QuestNameBorder1->Create( 14, 157, 233, 182,
		&g_MainInterfaceTextureArchive, questresourceindex2 );
	m_QuestNameBorder1->SetClipRect(0, 117, 219, 142);
	m_QuestNameBorder1->ShowWindow(FALSE);
	InsertChildObject( m_QuestNameBorder1 );
	
	m_QuestNameBorder2 = new _XImageStatic;
	m_QuestNameBorder2->Create( 233, 157, 316, 182,
		&g_MainInterfaceTextureArchive, questresourceindex2 );
	m_QuestNameBorder2->SetClipRect(0, 143, 83, 168);
	m_QuestNameBorder2->ShowWindow(FALSE);
	InsertChildObject( m_QuestNameBorder2 );
	
	// 퀘스트 완료 도장 이미지 ( 94 X 98 )
	m_QuestCompleteIconImage = new _XImageStatic;
	m_QuestCompleteIconImage->Create( 233, 284, 327, 382,
		&g_MainInterfaceTextureArchive, questresourceindex2 );
	m_QuestCompleteIconImage->SetClipRect(162, 158, 256, 256);
	m_QuestCompleteIconImage->ShowWindow(FALSE);
	InsertChildObject( m_QuestCompleteIconImage );
	
	// 퀘스트 취소 버튼 
	_XBTN_STRUCTURE questcancelbtnstruct = 
	{
		TRUE, {261, 159}, {52, 18}, _XDEF_QW_QUESTCANCEL_BUTTON,
		questresourceindex2, questresourceindex2, questresourceindex2,
		&g_MainInterfaceTextureArchive
	};

	m_QuestCancelButton = new _XButton;
	m_QuestCancelButton->Create(questcancelbtnstruct);
	m_QuestCancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 170, 52, 188);
	m_QuestCancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 53, 170, 105, 188);
	m_QuestCancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 106, 170, 158, 188);
	m_QuestCancelButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_QUEST_GIVEUP );	// _T("포기")
	m_QuestCancelButton->ShowWindow(FALSE);
	InsertChildObject(m_QuestCancelButton);
	
	int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
		
	// 퀘스트에 사용될 리스트 박스
	_XLISTBOX_STRUCTURE queststructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 4, 38 },		// 윈도우 좌표
		{ 335, 104 },	// 윈도우 사이즈
		_XDEF_QW_QUESTLISTBOX,							// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		17,												// 리스트 아이템 라인 피치
		6,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 104 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		150,												// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
	};
	m_listboxQuestScriptItem = new _XListBox;
	m_listboxQuestScriptItem->Create(queststructure);
		
	_XVScrollBar* pScrollBar = m_listboxQuestScriptItem->GetScrollBarObject();
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
	m_listboxQuestScriptItem->SetAlwaysShowScrollBarMode(TRUE);
	
	POINT columnoffset1 = { 5, 5 };
	POINT columnoffset2 = { 20, 6 };
	POINT columnoffset3 = { 24, 3 };
	POINT columnoffset4 = { 40, 6 };
	m_listboxQuestScriptItem->SetColumnOffset(0, columnoffset1);				// column offset
	m_listboxQuestScriptItem->SetColumnOffset(1, columnoffset2);
	m_listboxQuestScriptItem->SetColumnOffset(2, columnoffset3);
	m_listboxQuestScriptItem->SetColumnOffset(3, columnoffset4);
	
	m_listboxQuestScriptItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_listboxQuestScriptItem->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_listboxQuestScriptItem->SetColumnStyle(2, _XLCOLUMNSTYLE_ICONRECT);
	m_listboxQuestScriptItem->SetColumnStyle(3, _XLCOLUMNSTYLE_STRING);
	
	m_listboxQuestScriptItem->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxQuestScriptItem->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_listboxQuestScriptItem->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);
	m_listboxQuestScriptItem->SetAlignType(3, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxQuestScriptItem->SetColumnWidth(0, 320 -  5);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxQuestScriptItem->SetColumnWidth(1, 320 - 20);
	m_listboxQuestScriptItem->SetColumnWidth(2, 320 - 24);
	m_listboxQuestScriptItem->SetColumnWidth(3, 320 - 40);
	
	m_listboxQuestScriptItem->SetSelectedImageArchive(npcresourceindex1, &g_MainInterfaceTextureArchive);	
	_XImageStatic* selectbar = m_listboxQuestScriptItem->GetSelectBarObject();
	selectbar->SetClipRect( 196, 490, 507, 506 );
	m_listboxQuestScriptItem->SetSelectedImagePosition(5, 3);
	m_listboxQuestScriptItem->SetSelectedImageHeight(17);
	m_listboxQuestScriptItem->SetTrackWidth(322);
	
	m_listboxQuestScriptItem->ShowWindow(FALSE);
	InsertChildObject(m_listboxQuestScriptItem);
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		// 퀘스트 창에서 선택시 획득할 보상물 표시에 사용될 리스트 박스
		_XLISTBOX_STRUCTURE rewardstructure = 
		{
			TRUE,											// 윈도우 활성화
			{ 80, 200 },									// 윈도우 좌표
			{ 256, 34 },									// 윈도우 사이즈
			_XDEF_QW_QUESTREWARDLISTBOX,					// 윈도우 command 아이디
			-1,												// 윈도우 border image
			&g_MainInterfaceTextureArchive,					// Texture list
			_T(" "),										// 윈도우 타이틀
			34,												// 리스트 아이템 라인 피치
			6,												// 리스트 박스에 보이는 최대 라인수
			_XLSTYLE_NOSCROLLBAR							// 스크롤바의 위치
			
		};
		m_listboxQuestReward = new _XListBox;
		m_listboxQuestReward->Create(rewardstructure);	
		
		m_listboxQuestReward->SetIconReportMode( TRUE );
		m_listboxQuestReward->SetIconReportModeInfo( 1, 9, 34, 34 );
		
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
		
	}
	else
	{
		// 퀘스트 창에서 선택시 획득할 보상물 표시에 사용될 리스트 박스
		_XLISTBOX_STRUCTURE rewardstructure = 
		{
			TRUE,											// 윈도우 활성화
			{ 68, 200 },									// 윈도우 좌표
			{ 256, 34 },									// 윈도우 사이즈
			_XDEF_QW_QUESTREWARDLISTBOX,					// 윈도우 command 아이디
			-1,												// 윈도우 border image
			&g_MainInterfaceTextureArchive,					// Texture list
			_T(" "),										// 윈도우 타이틀
			34,												// 리스트 아이템 라인 피치
			6,												// 리스트 박스에 보이는 최대 라인수
			_XLSTYLE_NOSCROLLBAR							// 스크롤바의 위치
			
		};
		m_listboxQuestReward = new _XListBox;
		m_listboxQuestReward->Create(rewardstructure);	
		
		m_listboxQuestReward->SetIconReportMode( TRUE );
		m_listboxQuestReward->SetIconReportModeInfo( 1, 9, 31, 31 );
		
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
		
	}
	
	// 퀘스트 설명에 사용될 리스트 박스
	_XLISTBOX_STRUCTURE questdescstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 4, 231 },		// 윈도우 좌표
		{ 335, 150 },	// 윈도우 사이즈
		_XDEF_QW_QUESTDESCLISTBOX,						// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		15,												// 리스트 아이템 라인 피치
		10,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 234 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		150,												// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
	};
	m_listboxQuestDesc = new _XListBox;
	m_listboxQuestDesc->Create(questdescstructure);
	
	pScrollBar = m_listboxQuestDesc->GetScrollBarObject();
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
	m_listboxQuestDesc->SetAlwaysShowScrollBarMode(TRUE);
	m_listboxQuestDesc->SetMoveScrollBarMode(TRUE);
	m_listboxQuestDesc->MoveScrollBarPos( 324, -82 );
	
	POINT desccolumnoffset1 = { 30, 2 };
	POINT desccolumnoffset2 = { 20, 2 };

	m_listboxQuestDesc->SetColumnOffset(0, desccolumnoffset1);				// column offset
	m_listboxQuestDesc->SetColumnOffset(1, desccolumnoffset2);
	
	m_listboxQuestDesc->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style (아이콘)
	m_listboxQuestDesc->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	
	m_listboxQuestDesc->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxQuestDesc->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxQuestDesc->SetColumnWidth(0, 330);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxQuestDesc->SetColumnWidth(1, 330);

	m_listboxQuestDesc->EnableSelectItem(FALSE);
	m_listboxQuestDesc->ShowWindow(FALSE);
	InsertChildObject(m_listboxQuestDesc);
	
	
	m_SelectedTab = _XQUESTTAB_QUEST1;

	return TRUE;
}


void _XWindow_QuestWindow::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_QuestWindow::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	
	POINT listwinpos;
	SIZE listwinsize;
	
	listwinpos = m_listboxQuestScriptItem->GetWindowPos();
	listwinsize = m_listboxQuestScriptItem->GetWindowSize();
	
	_XDrawSolidBar( listwinpos.x, listwinpos.y-1,
		listwinpos.x+listwinsize.cx - 13, listwinpos.y+listwinsize.cy-1 , D3DCOLOR_ARGB(102,0,0,0));
	_XDrawRectAngle( listwinpos.x, listwinpos.y-1,
		listwinpos.x+listwinsize.cx- 13, listwinpos.y+listwinsize.cy-1, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( listwinpos.x+1, listwinpos.y,
		listwinpos.x+listwinsize.cx - 14, listwinpos.y+listwinsize.cy-2, 0.0f, D3DCOLOR_ARGB(191,0,0,0) );
	_XDrawRectAngle( listwinpos.x+2, listwinpos.y+1,
		listwinpos.x+listwinsize.cx - 15, listwinpos.y+listwinsize.cy-3, 0.0f, D3DCOLOR_ARGB(63,0,0,0) );
	
	m_listboxQuestScriptItem->Draw(pfocusobject);

	if( m_SelectedQuestID != -1)
	{
		_XDrawRectAngle( m_WindowPosition.x + 3, m_WindowPosition.y + 147,
			m_WindowPosition.x + 327, m_WindowPosition.y + 382, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

		//퀘스트 이름 
		if(m_SelectedQuestName[0] != 0)
		{
			switch(m_SelectedTab) {
			case _XQUESTTAB_QUEST1: //제룡행(노란색)
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 
				}
				break;
			case _XQUESTTAB_QUEST2: //무림행(파랑색)
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 22, 146, 252));
				}
				break;
			case _XQUESTTAB_QUEST3:	//협객행(주황색)
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 245, 116, 34)); 
				}
				break;
			case _XQUESTTAB_QUEST4:	//기연행(녹색)
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 78, 229, 48));
				}
				break;
			case _XQUESTTAB_QUEST5: //무림소사(흰색)
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
				}
				break;
			}
			g_XBaseFont->SetBoldMode( TRUE );
			g_XBaseFont->Puts(m_WindowPosition.x + 24, m_WindowPosition.y + 163, m_SelectedQuestName);
			g_XBaseFont->Flush();
			g_XBaseFont->SetBoldMode(FALSE);
		}
		
		//퀘스트 부제목 
		if(m_TempSelectedQuestSubName[0] != 0)
		{
			g_XBaseFont->SetBoldMode( TRUE );
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 0, 0, 0));
			g_XBaseFont->Puts(m_WindowPosition.x + 24, m_WindowPosition.y + 184, m_TempSelectedQuestSubName);
			g_XBaseFont->Flush();
			g_XBaseFont->SetBoldMode(FALSE);

			if( m_bOverFlowQuestSubName )
			{
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
				
				if(scrnpos->x > (m_WindowPosition.x + 24) )
				{
					if(scrnpos->x < (m_WindowPosition.x + 320) )
					{
						if( scrnpos->z > (m_WindowPosition.y + 184) )
						{
							if( scrnpos->z < (m_WindowPosition.y + 195) )
							{
								int linecount = g_XBaseFont->Puts_Separate(scrnpos->x+32, scrnpos->z+6, m_SelectedQuestSubName, 290 );

								_XDrawSolidBar( scrnpos->x+30, scrnpos->z+4, scrnpos->x+30+290+4, scrnpos->z+linecount*18 , D3DCOLOR_ARGB(255,255,255,255) );;
								_XDrawRectAngle( scrnpos->x+30, scrnpos->z+4, scrnpos->x+30+290+4, scrnpos->z+linecount*18 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
								
								g_XBaseFont->SetBoldMode( TRUE );
								g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 0, 0, 0));
								g_XBaseFont->Puts_Separate(scrnpos->x+32, scrnpos->z+6, m_SelectedQuestSubName, 290 );
								g_XBaseFont->Flush();
								g_XBaseFont->SetBoldMode(FALSE);
							}
						}
					}
				}
			}
		}
		else
		{
			if( m_SelectedQuestName[0] != 0 )
			{
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 0, 0, 0));
				g_XBaseFont->Puts(m_WindowPosition.x + 24, m_WindowPosition.y + 184, m_SelectedQuestName);
				g_XBaseFont->Flush();
				g_XBaseFont->SetBoldMode(FALSE);
			}
		}

		if( m_listboxQuestReward->GetItemCount() != 0 )
		{
			// 해외 버젼일 때 
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) // US, Vietnamese version은 퀘스트 타입이 길어 보상 글 제외.
			{	
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 0, 0, 0));
				if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 26, m_WindowPosition.y + 200, _XGETINTERFACETEXT(ID_STRING_QUEST_REWARD));	// _T("보상 :")
				}
				else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 8, m_WindowPosition.y + 200, _XGETINTERFACETEXT(ID_STRING_QUEST_REWARD));	// _T("보상 :")
				}
				g_XBaseFont->Flush();
				g_XBaseFont->SetBoldMode(FALSE);
			}
			else // 국내 버젼일때 
			{
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 0, 0, 0));
				g_XBaseFont->Puts(m_WindowPosition.x + 26, m_WindowPosition.y + 200, _XGETINTERFACETEXT(ID_STRING_QUEST_REWARD));	// _T("보상 :")
				g_XBaseFont->Flush();
				g_XBaseFont->SetBoldMode(FALSE);
			}
		}
				
	}

	g_XBaseFont->Flush();



}

void _XWindow_QuestWindow::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow( X, Y);
	
}
BOOL _XWindow_QuestWindow::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;
	
	if( m_listboxQuestReward->GetItemCount() != 0 )
	{		
		MouseState* mousestate = gpInput->GetMouseState();
		
		if(mousestate)
		{
			if(!pfocusobject)
			{
				if(mousestate->bButton[0])
				{
					int selectitem = m_listboxQuestReward->CheckSelectedItem();
					if( selectitem != -1 )
					{						
						if( g_pDefaultTooltip_Window )
						{								
							DWORD cmdid = (DWORD)m_listboxQuestReward->GetItemCommandID( selectitem );
							int type = (cmdid & 0xFFFF0000) >> 16;
							int id = (cmdid & 0x0000FFFF);
							if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_QUEST, selectitem, type, id ) )									
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
				}
			}
		}		
	}
	
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

BOOL _XWindow_QuestWindow::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		if( gpInput->CheckMousePos( m_WindowPosition.x+4, m_WindowPosition.y+11, m_WindowPosition.x + m_WindowSize.cx-4, m_WindowPosition.y+37 ) )
		{
			if( zDelta > 0 )
			{
				if( m_SelectedTab == _XQUESTTAB_QUEST1 )
				{
					ChangeTab(_XQUESTTAB_QUEST5);
				}
				else
				{
					ChangeTab((_XQuestWindowTab)(m_SelectedTab-1));
				}
			}
			else
			{
				if( m_SelectedTab == _XQUESTTAB_QUEST5)
				{
					ChangeTab(_XQUESTTAB_QUEST1);
				}
				else
				{
					ChangeTab((_XQuestWindowTab)(m_SelectedTab+1));
				}			
			}
			return TRUE;
		}
	}

	if( CheckMousePositionQuestDescriptionBox() )
	{
		if( m_listboxQuestDesc->ProcessWheelMessage(zDelta) )
		{
			return TRUE;
		}
	}

	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}

BOOL _XWindow_QuestWindow::CheckMousePositionQuestDescriptionBox(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(scrnpos->x > (m_WindowPosition.x + 3) )
		if(scrnpos->x < (m_WindowPosition.x + 327) )
			if( scrnpos->z > (m_WindowPosition.y + 147) )
				if( scrnpos->z < (m_WindowPosition.y + 383) )
					return TRUE;
	
	return FALSE;
}

void _XWindow_QuestWindow::SetListItem(void)
{
	
	m_listboxQuestScriptItem->DeleteAllItem();	
	switch(m_SelectedTab) {
	case _XQUESTTAB_QUEST1:
		{
			int seachquestnumber = 0;
			if( g_QuestScriptManager.m_CurrentMainQuestID >= 11000 && g_QuestScriptManager.m_CurrentMainQuestID < 12000 )// 개방
			{
				seachquestnumber = 1;
			}
			else if( g_QuestScriptManager.m_CurrentMainQuestID >= 12000 && g_QuestScriptManager.m_CurrentMainQuestID < 13000)// 비궁
			{
				seachquestnumber = 2;
			}
			else if( g_QuestScriptManager.m_CurrentMainQuestID >= 13000 && g_QuestScriptManager.m_CurrentMainQuestID < 14000 )// 소림
			{
				seachquestnumber = 3;
			}
			else if( g_QuestScriptManager.m_CurrentMainQuestID >= 14000 && g_QuestScriptManager.m_CurrentMainQuestID < 15000 )// 녹림
			{
				seachquestnumber = 4;
			}
			else if( g_QuestScriptManager.m_CurrentMainQuestID >= 15000 && g_QuestScriptManager.m_CurrentMainQuestID < 16000 )// 무당
			{
				seachquestnumber = 5;
			}
			else if( g_QuestScriptManager.m_CurrentMainQuestID >= 16000 && g_QuestScriptManager.m_CurrentMainQuestID < 17000 )// 마교
			{
				seachquestnumber = 6;
			}
			
			if( g_QuestScriptManager.m_CurrentMainQuestID == 65535 || g_QuestScriptManager.m_CurrentMainQuestID == -1 )
			{
				switch(g_pLocalUser->m_CharacterInfo.groupindex) 
				{
				case _XGROUP_GAEBANG:
					{
						seachquestnumber = 1;
					}
					break;
				case _XGROUP_BEEGOONG:
					{
						seachquestnumber = 2;
					}
					break;
				case _XGROUP_SORIM:
					{
						seachquestnumber = 3;
					}
					break;
				case _XGROUP_NOCKRIM:
					{
						seachquestnumber = 4;
					}
					break;
				case _XGROUP_MOODANG :
					{
						seachquestnumber = 5;
					}
					break;
				case _XGROUP_MAKYO :
					{
						seachquestnumber = 6;
					}
					break;
				}
				
			}		
			
			int count = 0 ;	
			int groupindexnumber = 0;
			m_TotalCountQuestListIndex = 0;

			int currentgroupnumber = GetQuestGroupNumber( g_QuestScriptManager.m_CurrentMainQuestID );
			if( !m_QuestGroupList.listEmpty() )
			{
				POINT columnoffset1 = { 5, 5 };
				m_listboxQuestScriptItem->SetColumnOffset(0, columnoffset1);				// column offset

				
				m_QuestGroupList.resetList();
				_XQuestScriptItemGroup* currentgroup = NULL;
				do 
				{
					currentgroup = (_XQuestScriptItemGroup *)m_QuestGroupList.currentItem();
					if(currentgroup)
					{				
						
						TCHAR	groupnumberbuffer[256];
						TCHAR	groupnamebuffer[256];	
						
						memset(groupnumberbuffer, 0, sizeof(groupnumberbuffer));
						memset(groupnamebuffer, 0, sizeof(groupnamebuffer));
						

						int param1 = currentgroup->m_QuestGroupNumber;						
						strcpy( groupnumberbuffer, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_QUEST_2296, &param1, currentgroup->m_QuestGroupName ) );					
					
						groupindexnumber++;
						
						
						int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
						RECT rect = {12, 12, 24, 24};
						if( currentgroup->m_bSelected )	
						{
							rect.top =0;
							rect.bottom = 12;
						}

						m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
										
						m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxQuestScriptItem->SetItemAttrib(count, 0, chattingbuttonindex, rect);
						
						m_listboxQuestScriptItem->SetItemText(count, 1, groupnumberbuffer);
						m_listboxQuestScriptItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
						m_listboxQuestScriptItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));

						m_listboxQuestScriptItem->SetItemAttrib(count, 2, -1 , rect);
						count++;
						currentgroup->m_QuestGroupIndexNumber = count; // 리스트 박스의 인텍스 값과 동기화
						m_TotalCountQuestListIndex = count;
												
						if( currentgroup->m_bSelected ) //퀘스트 그룹이 선택되면 거기에 있는 퀘스트를 보여 준다.
						{	
							int minimumnumber, maximumnumber ;
							if( currentgroup->m_QuestGroupNumber == 1)
							{
								switch(seachquestnumber) {
								case 1://_XGROUP_GAEBANG
								case 2://_XGROUP_BEEGOONG
								case 4://_XGROUP_NOCKRIM
									{
										int basenumber = 10000 + seachquestnumber*1000;	
										minimumnumber = basenumber + (currentgroup->m_QuestGroupNumber-1)*10 ;
										maximumnumber = basenumber + 4 ;
									}
									break;
								case 3://_XGROUP_SORIM
									{
										int basenumber = 10000 + seachquestnumber*1000;	
										minimumnumber = basenumber + (currentgroup->m_QuestGroupNumber-1)*10 ;
										maximumnumber = basenumber + 3 ;
									}
									break;
								case 5://_XGROUP_MOODANG
								case 6://_XGROUP_MAKYO
									{
										int basenumber = 10000 + seachquestnumber*1000;	
										minimumnumber = basenumber;
										maximumnumber = basenumber+1;
									}
									break;
								}
							}
							else if( currentgroup->m_QuestGroupNumber == 2)
							{
								switch(seachquestnumber) {
								case 1://_XGROUP_GAEBANG
								case 2://_XGROUP_BEEGOONG
								case 4://_XGROUP_NOCKRIM
									{
										int basenumber = 10000 + seachquestnumber*1000;	
										minimumnumber = basenumber + 4 ;
										maximumnumber = basenumber + currentgroup->m_QuestGroupNumber*10 ;
									}
									break;
								case 3://_XGROUP_SORIM
									{
										int basenumber = 10000 + seachquestnumber*1000;	
										minimumnumber = basenumber + 3 ;
										maximumnumber = basenumber + currentgroup->m_QuestGroupNumber*10 ;
									}
									break;
								case 5://_XGROUP_MOODANG
								case 6://_XGROUP_MAKYO
									{
										int basenumber = 10000 + seachquestnumber*1000;	
										minimumnumber = basenumber + 1 ;
										maximumnumber = basenumber + currentgroup->m_QuestGroupNumber*10 ;
									}
									break;
								}

							}
							else
							{
								int basenumber = 10000 + seachquestnumber*1000;	
								minimumnumber = basenumber + (currentgroup->m_QuestGroupNumber-1)*10 ;
								maximumnumber = basenumber + currentgroup->m_QuestGroupNumber*10 ;
							}
						

							map <int, _XQuestScriptItem*>::iterator iter_quest;
							for(iter_quest = g_QuestScriptManager.m_mapQuestScriptList.begin() ; iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end() ; iter_quest++)
							{
								int questid = iter_quest->first;
								if( questid >= minimumnumber && questid < maximumnumber )
								{
									_XQuestScriptItem* currentquestscriptitem = iter_quest->second;
									if(currentquestscriptitem)
									{
										if( currentquestscriptitem->GetQuestID() != g_QuestScriptManager.m_CurrentMainQuestID )
										{
											m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
											
											int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
											RECT rect = {238, 81, 251, 97}; //노란색 두루마기 이미지 
											
											m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
											m_listboxQuestScriptItem->SetItemAttrib(count, 0, -1 , rect);
											m_listboxQuestScriptItem->SetItemAttrib(count, 2, questresourceindex2, rect);
											
											TCHAR	questnamebuffer[256];	
											memset(questnamebuffer, 0, sizeof(questnamebuffer));											
											sprintf( questnamebuffer, _XGETINTERFACETEXT(ID_STRING_QUEST_COMPLETE),currentquestscriptitem->GetQuestName() );	// "%s(완료)"
											
											m_listboxQuestScriptItem->SetItemText(count, 3, questnamebuffer );
											m_listboxQuestScriptItem->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
											m_listboxQuestScriptItem->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));									
																					
											count++;						
											currentquestscriptitem->m_QuestScriptItemIndexNumber = count;
											m_TotalCountQuestListIndex = count;
										}
										else // 현재 퀘스트 일때 
										{
											if( g_QuestScriptManager.m_bProgressMainQuest )
											{
												m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
												
												int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
												RECT rect = {238, 81, 251, 97}; //노란색 두루마기 이미지 
												
												m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
												m_listboxQuestScriptItem->SetItemAttrib(count, 0, -1 , rect);
												m_listboxQuestScriptItem->SetItemAttrib(count, 2, questresourceindex2, rect);
												
												m_listboxQuestScriptItem->SetItemText(count, 3, currentquestscriptitem->GetQuestName() );
												m_listboxQuestScriptItem->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
												m_listboxQuestScriptItem->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));									
												
												count++;						
												currentquestscriptitem->m_QuestScriptItemIndexNumber = count;
												m_TotalCountQuestListIndex = count;
											}
											break;
										}									
									}
								}
							}
						}
						else//퀘스트 그룹에 선택되지 않은 경우에는 그룹안에 있는 퀘스트의 인덱스 넘버를 초기화시켜야 한다.
						{
							
							int minimumnumber, maximumnumber ;
							if( currentgroup->m_QuestGroupNumber == 1)
							{
								if( seachquestnumber != 3 )
								{
									int basenumber = 10000 + seachquestnumber*1000;	
									minimumnumber = basenumber + (currentgroup->m_QuestGroupNumber-1)*10 ;
									maximumnumber = basenumber + 4 ;
								}
								else
								{
									int basenumber = 10000 + seachquestnumber*1000;	
									minimumnumber = basenumber + (currentgroup->m_QuestGroupNumber-1)*10 ;
									maximumnumber = basenumber + 3 ;
								}
							}
							else if( currentgroup->m_QuestGroupNumber == 2)
							{
								if( seachquestnumber != 3 )
								{
									int basenumber = 10000 + seachquestnumber*1000;	
									minimumnumber = basenumber + 4 ;
									maximumnumber = basenumber + currentgroup->m_QuestGroupNumber*10 ;
								}
								else
								{
									int basenumber = 10000 + seachquestnumber*1000;	
									minimumnumber = basenumber + 3 ;
									maximumnumber = basenumber + currentgroup->m_QuestGroupNumber*10 ;
								}
							}
							else
							{
								int basenumber = 10000 + seachquestnumber*1000;	
								minimumnumber = basenumber + (currentgroup->m_QuestGroupNumber-1)*10 ;
								maximumnumber = basenumber + currentgroup->m_QuestGroupNumber*10 ;
							}
							
							
							map <int, _XQuestScriptItem*>::iterator iter_quest;
							for(iter_quest = g_QuestScriptManager.m_mapQuestScriptList.begin() ; iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end() ; iter_quest++)
							{
								int questid = iter_quest->first;
								if( questid >= minimumnumber && questid < maximumnumber )
								{
									_XQuestScriptItem* currentquestscriptitem = iter_quest->second;
									if(currentquestscriptitem)
									{
										currentquestscriptitem->m_QuestScriptItemIndexNumber = 0;
									}
								}
							}
						}
					}
					m_QuestGroupList.nextItem();
				} while(!m_QuestGroupList.atHeadOfList());
			}
	
		}
		break;
	case _XQUESTTAB_QUEST2://무림행
		{			
			int count = 0 ;			
			m_TotalCountQuestListIndex = 0;	
			
			// "진행중인 무림행" 리스트 생성
			int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
			RECT rect = {12, 12, 24, 24};
			if( m_bSelectedProgressQuestGroup )	
			{
				rect.top =0;
				rect.bottom = 12;
			}
			
			POINT columnoffset1 = { 5, 5 };
			m_listboxQuestScriptItem->SetColumnOffset(0, columnoffset1);				// column offset
			
			m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
			
			m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
			m_listboxQuestScriptItem->SetItemAttrib(count, 0, chattingbuttonindex, rect);
			
			m_listboxQuestScriptItem->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_ADVQUESTTYPE1));	// _T("진행중인 무림행")
			m_listboxQuestScriptItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			m_listboxQuestScriptItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			
			m_listboxQuestScriptItem->SetItemAttrib(count, 2, -1 , rect);
			count++;
			m_indexProgressQuestGroup = count; // 리스트 박스의 인텍스 값과 동기화
			m_TotalCountQuestListIndex = count;
			int tempindex = count;
			// "진행중인 무림행" 리스트 생성

			if( m_bSelectedProgressQuestGroup )
			{							
				for( int i = 0; i < ID_QUEST_SYNC-1; i++)
				{
					if( g_QuestScriptManager.m_ProgressSubQuestID[i] !=  -1)
					{
						int questid = g_QuestScriptManager.m_ProgressSubQuestID[i];
						if( questid >= 20000 && questid < 30000 )
						{	
							map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
							if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
							{
								_XQuestScriptItem* currentquestscriptitem = iter_quest->second;
								if(currentquestscriptitem)
								{										
									int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
									RECT rect = {238, 3, 251, 19}; //파란색 두루마기 이미지 							
									
									m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
									
									m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
									m_listboxQuestScriptItem->SetItemAttrib(count, 0, -1, rect);
									m_listboxQuestScriptItem->SetItemAttrib(count, 2, questresourceindex2 , rect);
									
									m_listboxQuestScriptItem->SetItemText(count, 3, currentquestscriptitem->GetQuestName());
									m_listboxQuestScriptItem->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
									m_listboxQuestScriptItem->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
									
									count++;						
									currentquestscriptitem->m_QuestScriptItemIndexNumber = count;	
									m_TotalCountQuestListIndex = count;
								}
							}
										
						}				

					}
				}
				if( tempindex == count )
				{
					m_bSelectedProgressQuestGroup = FALSE;
					RECT rect = {12, 12, 24, 24};
					m_listboxQuestScriptItem->SetIconArchive(count-1, &g_MainInterfaceTextureArchive);
					m_listboxQuestScriptItem->SetItemAttrib(count-1, 0, chattingbuttonindex, rect);
				}
				
			}
			
			// "완료한 무림행" 리스트 생성			
			RECT rect1 = {12, 12, 24, 24};
			if( m_bSelectedCompleteQuestGroup )	
			{
				rect1.top =0;
				rect1.bottom = 12;
			}			
			m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
			
			m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
			m_listboxQuestScriptItem->SetItemAttrib(count, 0, chattingbuttonindex, rect1);
			
			m_listboxQuestScriptItem->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_FINQUESTTYPE1));	// _T("완료한 무림행")
			m_listboxQuestScriptItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			m_listboxQuestScriptItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			
			m_listboxQuestScriptItem->SetItemAttrib(count, 2, -1 , rect1);
			count++;
			m_indexCompleteQuestGroup = count; // 리스트 박스의 인텍스 값과 동기화
			m_TotalCountQuestListIndex = count;
			tempindex = count;
			// "완료한 무림행" 리스트 생성

			if( m_bSelectedCompleteQuestGroup )
			{
				map <int, _XQUEST_COMPLETE_RESULT>::iterator iter;
				for(iter = g_QuestScriptManager.m_mapCompleteQuestList.begin() ; iter != g_QuestScriptManager.m_mapCompleteQuestList.end() ; iter++)
				{				
					int questid = iter->first;
					if( questid >= 20000 && questid < 30000 )
					{					
						map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
						if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
						{
							_XQuestScriptItem* currentquestscriptitem = iter_quest->second;
							if(currentquestscriptitem)
							{								
								int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
								RECT rect = {238, 3, 251, 19}; //파란색 두루마기 이미지 							
								
								m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
								
								m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
								m_listboxQuestScriptItem->SetItemAttrib(count, 0, -1, rect);
								m_listboxQuestScriptItem->SetItemAttrib(count, 2, questresourceindex2 , rect);
								
								TCHAR	questnamebuffer[512];	
								memset(questnamebuffer, 0, sizeof(questnamebuffer));											
								sprintf( questnamebuffer, _XGETINTERFACETEXT(ID_STRING_QUEST_COMPLETE),currentquestscriptitem->GetQuestName() );	// "%s(완료)"
								
								m_listboxQuestScriptItem->SetItemText(count, 3, questnamebuffer);
								m_listboxQuestScriptItem->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
								m_listboxQuestScriptItem->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
								
								count++;						
								currentquestscriptitem->m_QuestScriptItemIndexNumber = count;
								m_TotalCountQuestListIndex = count;
							}		
						}
					}
				}
				if( tempindex == count )
				{
					m_bSelectedCompleteQuestGroup = FALSE;
					RECT rect = {12, 12, 24, 24};
					m_listboxQuestScriptItem->SetIconArchive(count-1, &g_MainInterfaceTextureArchive);
					m_listboxQuestScriptItem->SetItemAttrib(count-1, 0, chattingbuttonindex, rect);
				}
			}
		}
		break;
	case _XQUESTTAB_QUEST3://협객행 
		{					
			int count = 0 ;				
			m_TotalCountQuestListIndex = 0;
			
			// "진행중인 협객행" 리스트 생성
			int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
			RECT rect = {12, 12, 24, 24};
			if( m_bSelectedProgressQuestGroup )	
			{
				rect.top =0;
				rect.bottom = 12;
			}
			
			POINT columnoffset1 = { 5, 5 };
			m_listboxQuestScriptItem->SetColumnOffset(0, columnoffset1);				// column offset
			
			m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
			
			m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
			m_listboxQuestScriptItem->SetItemAttrib(count, 0, chattingbuttonindex, rect);
			
			m_listboxQuestScriptItem->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_ADVQUESTTYPE2));	// _T("진행중인 협객행")
			m_listboxQuestScriptItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			m_listboxQuestScriptItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			
			m_listboxQuestScriptItem->SetItemAttrib(count, 2, -1 , rect);
			count++;
			m_indexProgressQuestGroup = count; // 리스트 박스의 인텍스 값과 동기화
			m_TotalCountQuestListIndex = count;
			int tempindex = count;
			// "진행중인 협객행" 리스트 생성
			
			if( m_bSelectedProgressQuestGroup )
			{
				for( int i = 0; i < ID_QUEST_SYNC-1; i++)
				{
					if( g_QuestScriptManager.m_ProgressSubQuestID[i] !=  -1)
					{
						int questid = g_QuestScriptManager.m_ProgressSubQuestID[i];
						if( questid >= 30000 && questid < 40000 )
						{	
							map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
							if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
							{
								_XQuestScriptItem* currentquestscriptitem = iter_quest->second;
								if(currentquestscriptitem)
								{										
									int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
									RECT rect = {238, 21, 251, 37}; //주황색 두루마기 이미지 							
									
									m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
									
									m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
									m_listboxQuestScriptItem->SetItemAttrib(count, 0, -1, rect);
									m_listboxQuestScriptItem->SetItemAttrib(count, 2, questresourceindex2 , rect);
									
									m_listboxQuestScriptItem->SetItemText(count, 3, currentquestscriptitem->GetQuestName());
									m_listboxQuestScriptItem->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
									m_listboxQuestScriptItem->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
									
									count++;						
									currentquestscriptitem->m_QuestScriptItemIndexNumber = count;	
									m_TotalCountQuestListIndex = count;
								}
							}
						}					
					}
				}
				if( tempindex == count )
				{
					m_bSelectedProgressQuestGroup = FALSE;
					RECT rect = {12, 12, 24, 24};
					m_listboxQuestScriptItem->SetIconArchive(count-1, &g_MainInterfaceTextureArchive);
					m_listboxQuestScriptItem->SetItemAttrib(count-1, 0, chattingbuttonindex, rect);
				}
			}
			// "완료한 협객행" 리스트 생성		
			RECT rect1 = {12, 12, 24, 24};
			if( m_bSelectedCompleteQuestGroup )	
			{
				rect1.top =0;
				rect1.bottom = 12;
			}
			
			m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
			
			m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
			m_listboxQuestScriptItem->SetItemAttrib(count, 0, chattingbuttonindex, rect1);
			
			m_listboxQuestScriptItem->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_FINQUESTTYPE2));	// _T("완료한 협객행")
			m_listboxQuestScriptItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			m_listboxQuestScriptItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			
			m_listboxQuestScriptItem->SetItemAttrib(count, 2, -1 , rect1);
			count++;
			m_indexCompleteQuestGroup = count; // 리스트 박스의 인텍스 값과 동기화
			m_TotalCountQuestListIndex = count;
			tempindex = count;
			// "완료한 협객행" 리스트 생성

			if( m_bSelectedCompleteQuestGroup )
			{
				map <int, _XQUEST_COMPLETE_RESULT>::iterator iter;
				for(iter = g_QuestScriptManager.m_mapCompleteQuestList.begin() ; iter != g_QuestScriptManager.m_mapCompleteQuestList.end() ; iter++)
				{				
					int questid = iter->first;
					if( questid >= 30000 && questid < 40000 )
					{	
						map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
						if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
						{
							_XQuestScriptItem* currentquestscriptitem = iter_quest->second;
							if(currentquestscriptitem)
							{						
								int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
								RECT rect = {238, 21, 251, 37}; //주황색 두루마기 이미지 							
								
								m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
								
								m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
								m_listboxQuestScriptItem->SetItemAttrib(count, 0, -1, rect);
								m_listboxQuestScriptItem->SetItemAttrib(count, 2, questresourceindex2 , rect);
								
								TCHAR	questnamebuffer[512];	
								memset(questnamebuffer, 0, sizeof(questnamebuffer));											
								sprintf( questnamebuffer, _XGETINTERFACETEXT(ID_STRING_QUEST_COMPLETE),currentquestscriptitem->GetQuestName() );	// "%s(완료)"
								
								m_listboxQuestScriptItem->SetItemText(count, 3, questnamebuffer);
								m_listboxQuestScriptItem->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
								m_listboxQuestScriptItem->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
								
								count++;						
								currentquestscriptitem->m_QuestScriptItemIndexNumber = count;
								m_TotalCountQuestListIndex = count;
								
							}
						}				
					}
				}
				if( tempindex == count )
				{
					m_bSelectedCompleteQuestGroup = FALSE;
					RECT rect = {12, 12, 24, 24};
					m_listboxQuestScriptItem->SetIconArchive(count-1, &g_MainInterfaceTextureArchive);
					m_listboxQuestScriptItem->SetItemAttrib(count-1, 0, chattingbuttonindex, rect);
				}
			}
		}
		break;
	case _XQUESTTAB_QUEST4: // 기연행
		{
			
			int count = 0 ;				
			m_TotalCountQuestListIndex = 0;
			// "진행중인 기연행" 리스트 생성
			int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
			RECT rect = {12, 12, 24, 24};
			if( m_bSelectedProgressQuestGroup )	
			{
				rect.top =0;
				rect.bottom = 12;
			}
			
			POINT columnoffset1 = { 5, 5 };
			m_listboxQuestScriptItem->SetColumnOffset(0, columnoffset1);				// column offset
			
			m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
			
			m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
			m_listboxQuestScriptItem->SetItemAttrib(count, 0, chattingbuttonindex, rect);
			
			m_listboxQuestScriptItem->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_ADVQUESTTYPE3));	// _T("진행중인 기연행")
			m_listboxQuestScriptItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			m_listboxQuestScriptItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			
			m_listboxQuestScriptItem->SetItemAttrib(count, 2, -1 , rect);
			count++;
			m_indexProgressQuestGroup = count; // 리스트 박스의 인텍스 값과 동기화
			m_TotalCountQuestListIndex = count;
			int tempindex = count;
			// "진행중인 기연행" 리스트 생성
			
			if( m_bSelectedProgressQuestGroup )
			{
				for( int i = 0; i < ID_QUEST_SYNC-1; i++)
				{
					if( g_QuestScriptManager.m_ProgressSubQuestID[i] !=  -1)
					{
						int questid = g_QuestScriptManager.m_ProgressSubQuestID[i];
						if( questid >= 40000 && questid < 50000 )
						{	
							map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
							if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
							{
								_XQuestScriptItem* currentquestscriptitem = iter_quest->second;
								if(currentquestscriptitem)
								{										
									int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
									RECT rect = {238, 61, 251, 77}; //녹색 두루마기 이미지 							
									
									m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
									
									m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
									m_listboxQuestScriptItem->SetItemAttrib(count, 0, -1, rect);
									m_listboxQuestScriptItem->SetItemAttrib(count, 2, questresourceindex2 , rect);
									
									m_listboxQuestScriptItem->SetItemText(count, 3, currentquestscriptitem->GetQuestName());
									m_listboxQuestScriptItem->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
									m_listboxQuestScriptItem->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
									
									count++;						
									currentquestscriptitem->m_QuestScriptItemIndexNumber = count;	
									m_TotalCountQuestListIndex = count;
								}
							}											
						}				
					}
				}
				if( tempindex == count )
				{
					m_bSelectedProgressQuestGroup = FALSE;
					RECT rect = {12, 12, 24, 24};
					m_listboxQuestScriptItem->SetIconArchive(count-1, &g_MainInterfaceTextureArchive);
					m_listboxQuestScriptItem->SetItemAttrib(count-1, 0, chattingbuttonindex, rect);
				}
				
			}
			
			// "완료한 기연행" 리스트 생성	
			RECT rect1 = {12, 12, 24, 24};
			if( m_bSelectedCompleteQuestGroup )	
			{
				rect1.top =0;
				rect1.bottom = 12;
			}
			
			m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
			
			m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
			m_listboxQuestScriptItem->SetItemAttrib(count, 0, chattingbuttonindex, rect1);
			
			m_listboxQuestScriptItem->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_FINQUESTTYPE3));	// _T("완료한 기연행")
			m_listboxQuestScriptItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			m_listboxQuestScriptItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			
			m_listboxQuestScriptItem->SetItemAttrib(count, 2, -1 , rect1);
			count++;
			m_indexCompleteQuestGroup = count; // 리스트 박스의 인텍스 값과 동기화
			m_TotalCountQuestListIndex = count;
			tempindex = count;
			// "완료한 기연행" 리스트 생성

			if( m_bSelectedCompleteQuestGroup )
			{
				map <int, _XQUEST_COMPLETE_RESULT>::iterator iter;
				for(iter = g_QuestScriptManager.m_mapCompleteQuestList.begin() ; iter != g_QuestScriptManager.m_mapCompleteQuestList.end() ; iter++)
				{				
					int questid = iter->first;
					if( questid >= 40000 && questid < 50000 )
					{					
						map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
						if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
						{
							_XQuestScriptItem* currentquestscriptitem = iter_quest->second;
							if(currentquestscriptitem)
							{								
								int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
								RECT rect = {238, 61, 251, 77}; //녹색 두루마기 이미지 							
								
								m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
								
								m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
								m_listboxQuestScriptItem->SetItemAttrib(count, 0, -1, rect);
								m_listboxQuestScriptItem->SetItemAttrib(count, 2, questresourceindex2 , rect);
								
								TCHAR	questnamebuffer[512];	
								memset(questnamebuffer, 0, sizeof(questnamebuffer));											
								sprintf( questnamebuffer, _XGETINTERFACETEXT(ID_STRING_QUEST_COMPLETE),currentquestscriptitem->GetQuestName() );	// "%s(완료)"
								
								m_listboxQuestScriptItem->SetItemText(count, 3, questnamebuffer);
								m_listboxQuestScriptItem->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
								m_listboxQuestScriptItem->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
								
								count++;						
								currentquestscriptitem->m_QuestScriptItemIndexNumber = count;
								m_TotalCountQuestListIndex = count;
							}		
						}
					}
				}
				if( tempindex == count )
				{
					m_bSelectedCompleteQuestGroup = FALSE;
					RECT rect = {12, 12, 24, 24};
					m_listboxQuestScriptItem->SetIconArchive(count-1, &g_MainInterfaceTextureArchive);
					m_listboxQuestScriptItem->SetItemAttrib(count-1, 0, chattingbuttonindex, rect);
				}
			}
		}
		break;
	case _XQUESTTAB_QUEST5: //무림소사 
		{
			
			int count = 0 ;	
			m_TotalCountQuestListIndex = 0;

			// "진행중인 무림소사" 리스트 생성
			int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
			RECT rect = {12, 12, 24, 24};
			if( m_bSelectedProgressQuestGroup )	
			{
				rect.top =0;
				rect.bottom = 12;
			}
			
			POINT columnoffset1 = { 5, 5 };
			m_listboxQuestScriptItem->SetColumnOffset(0, columnoffset1);				// column offset
			
			m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
			
			m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
			m_listboxQuestScriptItem->SetItemAttrib(count, 0, chattingbuttonindex, rect);
			
			m_listboxQuestScriptItem->SetItemText(count, 1, _XGETINTERFACETEXT(ID_STRING_QUEST_ADVQUESTTYPE4));	// _T("진행중인 무림소사")
			m_listboxQuestScriptItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			m_listboxQuestScriptItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 219, 196, 136));
			
			m_listboxQuestScriptItem->SetItemAttrib(count, 2, -1 , rect);
			count++;
			m_indexProgressQuestGroup = count; // 리스트 박스의 인텍스 값과 동기화
			m_TotalCountQuestListIndex = count;
			int tempindex = count;
			// "진행중인 무림소사" 리스트 생성

			if( m_bSelectedProgressQuestGroup )
			{
				if( !g_QuestScriptManager.m_listQuestNoSaveType.empty() )
				{		
					list <int>::iterator iter_questnosavetype;		
					iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.begin();
					while( iter_questnosavetype != g_QuestScriptManager.m_listQuestNoSaveType.end() )
					{			
						int questid = *iter_questnosavetype;			
						if( questid != -1 )
						{
							if( questid >= 50000 && questid < 60000 )
							{
								_XQuestScriptItem* currentquestscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[questid];
								if( currentquestscriptitem )
								{	
									int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
									RECT rect = {238, 41, 251, 57}; //흰색 두루마기 이미지 							
									
									m_listboxQuestScriptItem->InsertListItem(_T(""), 0, count+1);		
									
									m_listboxQuestScriptItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
									m_listboxQuestScriptItem->SetItemAttrib(count, 0, -1, rect);
									m_listboxQuestScriptItem->SetItemAttrib(count, 2, questresourceindex2 , rect);
									
									m_listboxQuestScriptItem->SetItemText(count, 3, currentquestscriptitem->GetQuestName());
									m_listboxQuestScriptItem->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
									m_listboxQuestScriptItem->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
									
									count++;						
									currentquestscriptitem->m_QuestScriptItemIndexNumber = count;	
									m_TotalCountQuestListIndex = count;
								}
							}
						}
						iter_questnosavetype++;	
					}
				}
				
				if( tempindex == count )
				{
					m_bSelectedProgressQuestGroup = FALSE;
					RECT rect = {12, 12, 24, 24};
					m_listboxQuestScriptItem->SetIconArchive(count-1, &g_MainInterfaceTextureArchive);
					m_listboxQuestScriptItem->SetItemAttrib(count-1, 0, chattingbuttonindex, rect);
				}
			}
			count++;
			m_indexCompleteQuestGroup = count;
			m_TotalCountQuestListIndex = count;			
		}
		break;
	}
	
}


void _XWindow_QuestWindow::ParseQuestDescString(void)
{

	int		i = 0, linecount = 0;
	TCHAR	buffer[2048];

	if(m_SelectedItemIndex == -1)
	{
		m_SelectedQuestID = -1;
		m_listboxQuestReward->ShowWindow(FALSE);
		m_listboxQuestDesc->ShowWindow(FALSE);
		
		m_QuestNameBorder1->ShowWindow(FALSE);
		m_QuestNameBorder2->ShowWindow(FALSE);
		
		m_QuestCancelButton->ShowWindow(FALSE);
		m_QuestCompleteIconImage->ShowWindow(FALSE);

		m_listboxQuestDesc->DeleteAllItem();
		return;
	}
		
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_SelectedQuestID >= 10000 && m_SelectedQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_SelectedQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
		{
			currentquest = iter_quest->second;	
		}
		else return;			
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_SelectedQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
		{
			currentquest = iter_quest->second;
		}
		else return;
	}

	if(currentquest)
	{
		m_listboxQuestReward->DeleteAllItem();
		m_listboxQuestDesc->DeleteAllItem();
		m_listboxQuestDesc->SetLineScrollPos(0);
		
		LPTSTR descstring = NULL;

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

		bool bprogressquest = false;
		if( currentquest->GetMainQuest() )
		{
			if( m_SelectedQuestID == g_QuestScriptManager.m_CurrentMainQuestID )
			{
				bprogressquest = true;
			}
		}
		else
		{
			if( currentquest->GetTempQuest() || currentquest->GetPartyQuest() )
			{
				// 무림소사가 리스트에 있으면 무조건 진행중인 퀘스트 
				bprogressquest = true;
			}
			else
			{
				for( int i = 0; i < ID_QUEST_SYNC-1; i++)
				{
					if( g_QuestScriptManager.m_ProgressSubQuestID[i] ==  m_SelectedQuestID)
					{
						bprogressquest = true;
						break;
					}
				}			
			}

		}
		
		
		if( !bprogressquest ) //전체 개요& 시작개요를 표시한다.
		{
			m_lpszQuestProcessInfomation = currentquest->m_lpszOutlineMessage2 ;//전체개요 
			if( m_lpszQuestProcessInfomation )
			{
				// 한 칸 띄운다.
				memset(buffer, 0, sizeof(buffer));
				m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
				m_listboxQuestDesc->SetItemText(linecount, 0, buffer);
				m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));	
				m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));			
				linecount++;

				_XStringSplitInfo splitinfo;

				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->SplitString(m_lpszQuestProcessInfomation, 260, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR );
				}
				else
				{
					g_XBaseFont->SplitStringByCR(m_lpszQuestProcessInfomation, splitinfo);
				}
				
				for(i = 0 ; i < splitinfo.splitcount ; i++)
				{
					memset(buffer, 0, sizeof(buffer));
					strncpy(buffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
					m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
					m_listboxQuestDesc->SetItemText(linecount, 0, buffer, TRUE);
					m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 214, 65, 9));
					m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
					
					linecount++;
				}
			}
			
			
			m_lpszQuestOutline = currentquest->m_lpszOutlineMessage;//시작 개요
			if( m_lpszQuestOutline )
			{
				// 한 칸 띄운다.
				memset(buffer, 0, sizeof(buffer));
				m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
				m_listboxQuestDesc->SetItemText(linecount, 0, buffer);
				m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));	
				m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));			
				linecount++;
				
				_XStringSplitInfo splitinfo;
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->SplitString(m_lpszQuestOutline, 300, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
				}
				else
				{
					g_XBaseFont->SplitStringByCR(m_lpszQuestOutline, splitinfo);
				}
				
				for(i = 0 ; i < splitinfo.splitcount ; i++)
				{
					memset(buffer, 0, sizeof(buffer));
					strncpy(buffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
					m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
					m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
					
					if( i == 0 )
					{
						m_listboxQuestDesc->SetItemText(linecount, 1, buffer, TRUE);
					}
					else
					{
						m_listboxQuestDesc->SetItemText(linecount, 1, buffer);
					}
					
					m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
					
					linecount++;
				}

			}
			
			m_listboxQuestReward->ShowWindow(TRUE);
			m_listboxQuestDesc->ShowWindow(TRUE);
			
			m_QuestNameBorder1->ShowWindow(TRUE);
			m_QuestNameBorder2->ShowWindow(TRUE);
			
			m_QuestCancelButton->ShowWindow(FALSE);
			m_QuestCompleteIconImage->ShowWindow(TRUE);
			m_listboxQuestDesc->SetLineScrollPos(0);
		}
		else//진행중인 퀘스트 -> 카운터 표시 & 시작 정보 & 시작개요를 표시한다.
		{
			BOOL bCountDisplay = FALSE;
			// 한 칸 띄운다.
			memset(buffer, 0, sizeof(buffer));
			m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
			m_listboxQuestDesc->SetItemText(linecount, 0, buffer);
			m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));	
			m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));			
			linecount++;
			
			// 파티퀘스트 카운터 표시 부분 
			if( currentquest->GetPartyQuest() )
			{
				for( int counterindex = 0; counterindex < 4; counterindex++ )
				{
					if( currentquest->m_PartyCounterInformation[counterindex].CountTotalPoint >= 1 )
					{
						if(currentquest->m_PartyCounterInformation[counterindex].CounterTargetName[0] != 0 )
						{
							if( currentquest->m_PartyCounterInformation[counterindex].CountTotalPoint > currentquest->m_PartyCounterInformation[counterindex].CountCurrentPoint)
							{
								sprintf(buffer, _T("%s : %d/%d"),
									currentquest->m_PartyCounterInformation[counterindex].CounterTargetName,
									currentquest->m_PartyCounterInformation[counterindex].CountCurrentPoint,
									currentquest->m_PartyCounterInformation[counterindex].CountTotalPoint );
							}
							else
							{
								// add word order

								sprintf(buffer, _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT),
									currentquest->m_PartyCounterInformation[counterindex].CounterTargetName,
									currentquest->m_PartyCounterInformation[counterindex].CountTotalPoint,
									currentquest->m_PartyCounterInformation[counterindex].CountTotalPoint );	// _T("%s : %d/%d (완료)")
							}
							
							m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
							m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
							m_listboxQuestDesc->SetItemText(linecount, 1, buffer, TRUE);
							m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
							linecount++;
							bCountDisplay = TRUE;
						}							
					}			
					else if( currentquest->m_PartyCounterInformation[counterindex].CountTotalPoint == 0)
					{
						sprintf(buffer, _T("%s : %d"),
							currentquest->m_PartyCounterInformation[counterindex].CounterTargetName,
							currentquest->m_PartyCounterInformation[counterindex].CountCurrentPoint);
						
						m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
						m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
						m_listboxQuestDesc->SetItemText(linecount, 1, buffer, TRUE);
						m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
						linecount++;
						bCountDisplay = TRUE;
					}
				}//for( int counterindex = 0; counterindex < 4; counterindex++ )
			}
			else
			{
				// 카운터 표시 부분 
				for( int counterindex = 0; counterindex < 4; counterindex++ )
				{
					if( currentquest->m_CounterInformation[counterindex].CountTotalPoint >= 1 )
					{
						if(currentquest->m_CounterInformation[counterindex].CounterTargetName[0] != 0 )
						{
							if( currentquest->m_CounterInformation[counterindex].CountTotalPoint > currentquest->m_CounterInformation[counterindex].CountCurrentPoint)
							{
								sprintf(buffer, _T("%s : %d/%d"),
									currentquest->m_CounterInformation[counterindex].CounterTargetName,
									currentquest->m_CounterInformation[counterindex].CountCurrentPoint,
									currentquest->m_CounterInformation[counterindex].CountTotalPoint );
							}
							else
							{
								// add word order

								sprintf(buffer, _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT),
									currentquest->m_CounterInformation[counterindex].CounterTargetName,
									currentquest->m_CounterInformation[counterindex].CountTotalPoint,
									currentquest->m_CounterInformation[counterindex].CountTotalPoint );	// _T("%s : %d/%d (완료)")
							}
							
							m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
							m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
							m_listboxQuestDesc->SetItemText(linecount, 1, buffer, TRUE);
							m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
							linecount++;
							bCountDisplay = TRUE;
						}							
					}			
					else if( currentquest->m_CounterInformation[counterindex].CountTotalPoint == 0)
					{
						sprintf(buffer, _T("%s : %d"),
							currentquest->m_CounterInformation[counterindex].CounterTargetName,
							currentquest->m_CounterInformation[counterindex].CountCurrentPoint);
						
						m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
						m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
						m_listboxQuestDesc->SetItemText(linecount, 1, buffer, TRUE);
						m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
						linecount++;
						bCountDisplay = TRUE;
					}
				}//for( int counterindex = 0; counterindex < 4; counterindex++ )
				
				// 아이템카운터 표시 부분 
				for( int itemcounterindex = 0; itemcounterindex < 8; itemcounterindex++ )
				{
					if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType != -1 )
					{
						
						memset(buffer, 0, sizeof(buffer));
						
						char cType = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType;
						short sID = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID;
						
						if(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName[0] != 0 )
						{													
							int itemtotalcount = 0;
							for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; i++)
							{
								if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == cType
									&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == sID)
									itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();														
							}

						#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장
							for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++)
							{
								if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == cType
									&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == sID)
									itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();														
							}
						#endif
							
							if( currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint == 0 )
							{
								sprintf(buffer, _T("%s : %d"),
									currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, itemtotalcount);
							}
							else if( itemtotalcount < currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								sprintf(buffer, _T("%s : %d/%d"),
									currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
									itemtotalcount,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint );
							}
							else
							{
								// add word order

								sprintf(buffer, _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT),	// _T("%s : %d/%d (완료)")
									currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint);
							}
							m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
							m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
							m_listboxQuestDesc->SetItemText(linecount, 1, buffer, TRUE );
							m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
							linecount++;	
							bCountDisplay = TRUE;											
						}						
					}						
				}//for( int itemcounterindex = 0; itemcounterindex < 8; itemcounterindex++ )
			}
			
			
			if(	bCountDisplay )
			{
				// 한 칸 띄운다.
				memset(buffer, 0, sizeof(buffer));
				m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
				m_listboxQuestDesc->SetItemText(linecount, 0, buffer);
				m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));	
				m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));			
				linecount++;						
			}

			m_lpszQuestProcessInfomation = currentquest->m_lpszQuestProgressinformation;
			if( m_lpszQuestProcessInfomation )
			{
				_XStringSplitInfo splitinfo;
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->SplitString(m_lpszQuestProcessInfomation, 260, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
				}
				else
				{
					g_XBaseFont->SplitStringByCR(m_lpszQuestProcessInfomation, splitinfo);
				}
				
				for(i = 0 ; i < splitinfo.splitcount ; i++)
				{
					memset(buffer, 0, sizeof(buffer));
					strncpy(buffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
					m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
					m_listboxQuestDesc->SetItemText(linecount, 0, buffer, TRUE );
					m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 214, 65, 9));
					m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
					
					linecount++;
				}
			}
			
			m_lpszQuestOutline = currentquest->m_lpszOutlineMessage;//시작 개요
			if( m_lpszQuestOutline )
			{
				// 한 칸 띄운다.
				memset(buffer, 0, sizeof(buffer));
				m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
				m_listboxQuestDesc->SetItemText(linecount, 0, buffer);
				m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));	
				m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));			
				linecount++;	

				_XStringSplitInfo splitinfo;

				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->SplitString(m_lpszQuestOutline, 300, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
				}
				else
				{
					g_XBaseFont->SplitStringByCR(m_lpszQuestOutline, splitinfo);
				}
				
				for(i = 0 ; i < splitinfo.splitcount ; i++)
				{
					memset(buffer, 0, sizeof(buffer));
					strncpy(buffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
					m_listboxQuestDesc->InsertListItem(_T(""), 0, 0);
					m_listboxQuestDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
					
					if( i == 0 )
					{
						m_listboxQuestDesc->SetItemText(linecount, 1, buffer, TRUE);
					}
					else
					{
						m_listboxQuestDesc->SetItemText(linecount, 1, buffer);
					}
					
					m_listboxQuestDesc->SetItemAttrib(linecount, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
					
					linecount++;
				}
			}
			m_listboxQuestReward->ShowWindow(TRUE);
			m_listboxQuestDesc->ShowWindow(TRUE);
			
			m_QuestNameBorder1->ShowWindow(TRUE);
			m_QuestNameBorder2->ShowWindow(TRUE);
			
			if( !currentquest->GetPartyQuest() )
			{
				m_QuestCancelButton->ShowWindow(TRUE);
			}
			else
			{
				//파티 퀘스트일 때는 행수일 때만 퀘스트 포기 버튼이 보인다.
				if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
				{
					m_QuestCancelButton->ShowWindow(TRUE);
				}
				else
				{
					m_QuestCancelButton->ShowWindow(FALSE);
				}
			}
			m_QuestCompleteIconImage->ShowWindow(FALSE);
			m_listboxQuestDesc->SetLineScrollPos(0);
		}	
		
	}//if( currentquest )
}

void _XWindow_QuestWindow::ProcessSelectedTabButton()
{
	if( m_SelectedItemIndex != -1 )
	{
		ProcessSelectedListBox();
	}
	else
	{
		if( m_SelectedTab == _XQUESTTAB_QUEST1 )
		{
			if( m_TotalCountQuestListIndex != 0 )
			{
				m_SelectedItemIndex = m_TotalCountQuestListIndex;// 진행중인 퀘스트를 선택하게끔 한다. 
				ProcessSelectedListBox();
			}
			else
			{			
				m_SelectedQuestID = -1;
				m_listboxQuestReward->ShowWindow(FALSE);
				m_listboxQuestDesc->ShowWindow(FALSE);
				
				m_QuestNameBorder1->ShowWindow(FALSE);
				m_QuestNameBorder2->ShowWindow(FALSE);
				
				m_QuestCancelButton->ShowWindow(FALSE);
				m_QuestCompleteIconImage->ShowWindow(FALSE);
				
				m_listboxQuestDesc->DeleteAllItem();
				
				memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
				strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR1) );	// _T("진행중인 제룡행이 없습니다.")
				if( m_SelectedQuestSubName[0] != 0 )
					SetTempQuestSubName(m_TempSelectedQuestSubName, m_SelectedQuestSubName );
			}
		}
		else// 제룡행이 아닐때 
		{
			if( m_TotalCountQuestListIndex != 2 ) 
			{
				m_SelectedItemIndex = 2;// 진행중인 퀘스트를 선택하게끔 한다. 
				ProcessSelectedListBox();
			}
			else// 총개수가 2이면 진행중인 퀘스트가 없는 것.
			{
				m_SelectedQuestID = -1;
				m_listboxQuestReward->ShowWindow(FALSE);
				m_listboxQuestDesc->ShowWindow(FALSE);
				
				m_QuestNameBorder1->ShowWindow(FALSE);
				m_QuestNameBorder2->ShowWindow(FALSE);
				
				m_QuestCancelButton->ShowWindow(FALSE);
				m_QuestCompleteIconImage->ShowWindow(FALSE);
				
				m_listboxQuestDesc->DeleteAllItem();
				switch(m_SelectedTab) {
				case _XQUESTTAB_QUEST1:
					{						
						memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
						strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR1) );	// _T("진행중인 제룡행이 없습니다.")
					}
					break;
				case _XQUESTTAB_QUEST2:
					{					
						memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
						strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR2) );	// _T("진행중인 무림행이 없습니다.")
					}
					break;
				case _XQUESTTAB_QUEST3:
					{
						memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
						strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERRPR3) );	//_T("진행중인 협객행이 없습니다.") ); 
					}
					break;
				case _XQUESTTAB_QUEST4:
					{
						memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
						strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR4) );	//_T("진행중인 기연행이 없습니다.") );
					}
					break;
				case _XQUESTTAB_QUEST5:
					{
						memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
						strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR5) );	//_T("진행중인 무림소사가 없습니다.") );
					}
					break;
				}
				
				if( m_SelectedQuestSubName[0] != 0 )
					SetTempQuestSubName(m_TempSelectedQuestSubName, m_SelectedQuestSubName );
			}
		}
	}

}
void _XWindow_QuestWindow::ChangeTab(_XQuestWindowTab selectedtab)
{
	m_ChangeTab = TRUE;
	m_listboxQuestScriptItem->ShowWindow(TRUE);

	if( m_SelectedTab != selectedtab )
	{
		m_SelectedTab = selectedtab;
		m_SelectedItemIndex = -1;
	}
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		for( int i = 0; i < _XQUESTTAB_TOTALCOUNT; i++ )
		{			
			if( m_SelectedTab == (_XQuestWindowTab)i )
			{
				m_QuestTab[i]->ShowWindow( TRUE );
			}
			else
			{
				m_QuestTab[i]->ShowWindow( FALSE );
			}
		}
	}
	
	for( int i = 0; i < _XQUESTTAB_TOTALCOUNT; i++ )
	{			
		m_QuestTab[i]->SetCheck( FALSE );
		m_QuestCancelButton->ShowWindow(FALSE);			
	}	

	if( m_SelectedTab == _XQUESTTAB_QUEST1)
	{
		m_QuestTab[0]->SetCheck( TRUE );

		_XQuestScriptItemGroup* currentgroup = NULL;
		if( !m_QuestGroupList.listEmpty() )
		{
			m_QuestGroupList.resetList();
			currentgroup = (_XQuestScriptItemGroup *)m_QuestGroupList.prevItem();
			if(currentgroup) currentgroup->m_bSelected = TRUE;			
		}
		SetListItem();
		ProcessSelectedTabButton();
	}
	else if( m_SelectedTab == _XQUESTTAB_QUEST2)
	{
		m_bSelectedProgressQuestGroup = TRUE;
		m_bSelectedCompleteQuestGroup = FALSE;
		m_QuestTab[1]->SetCheck( TRUE );
		SetListItem();
		ProcessSelectedTabButton();
	}
	else if( m_SelectedTab == _XQUESTTAB_QUEST3)
	{
		m_bSelectedProgressQuestGroup = TRUE;
		m_bSelectedCompleteQuestGroup = FALSE;
		m_QuestTab[2]->SetCheck( TRUE );
		SetListItem();
		ProcessSelectedTabButton();
	}
	else if( m_SelectedTab == _XQUESTTAB_QUEST4)
	{
		m_bSelectedProgressQuestGroup = TRUE;
		m_bSelectedCompleteQuestGroup = FALSE;
		m_QuestTab[3]->SetCheck( TRUE );
		SetListItem();
		ProcessSelectedTabButton();
	}
	else if( m_SelectedTab == _XQUESTTAB_QUEST5)
	{
		m_bSelectedProgressQuestGroup = TRUE;
		m_bSelectedCompleteQuestGroup = FALSE;
		m_QuestTab[4]->SetCheck( TRUE );
		SetListItem();
		ProcessSelectedTabButton();
	}		
}

void _XWindow_QuestWindow::ShowWindow(BOOL show)
{
	if(show)
	{
		SetQuestTabButton();
		ChangeTab( m_SelectedTab);
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_QuestWindow::SetDefaultPosition( void )
{		
}

void _XWindow_QuestWindow::SetParameter(void)
{
}

void _XWindow_QuestWindow::ProcessSelectedListBox()
{
	if( m_SelectedItemIndex != -1 )
	{
		if( m_SelectedTab == _XQUESTTAB_QUEST1 )
		{
			// 퀘스트 그룹이 선택되어 있는지를 체크한다.
			if( !m_QuestGroupList.listEmpty() )
			{
				m_QuestGroupList.resetList();
				_XQuestScriptItemGroup* currentgroup = NULL;
				do 
				{
					currentgroup = (_XQuestScriptItemGroup *)m_QuestGroupList.currentItem();
					if(currentgroup)
					{
						if( currentgroup->m_QuestGroupIndexNumber == m_SelectedItemIndex )
						{
							if( currentgroup->m_bSelected )
							{
								currentgroup->m_bSelected = FALSE;
							}
							else
							{
								currentgroup->m_bSelected = TRUE;
							}
							SetListItem();
							return;
						}
					}
					m_QuestGroupList.nextItem();
				} while(!m_QuestGroupList.atHeadOfList());
			}

			// 퀘스트 그룹이 선택되지 않았으면 퀘스트를 검사한다.
			map <int, _XQuestScriptItem*>::iterator iter_quest;
			for(iter_quest = g_QuestScriptManager.m_mapQuestScriptList.begin() ; iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end() ; iter_quest++)
			{
				int questid = iter_quest->first;
				if( questid >= 10000 && questid < 20000)
				{
					_XQuestScriptItem* currentquestscriptitem = iter_quest->second;
					if(currentquestscriptitem)
					{				
						if( currentquestscriptitem->m_QuestScriptItemIndexNumber == m_SelectedItemIndex )
						{
							
							if( currentquestscriptitem->m_bSelectedQuestScriptItem )
							{
								currentquestscriptitem->m_bSelectedQuestScriptItem = FALSE;
							}
							else
							{
								currentquestscriptitem->m_bSelectedQuestScriptItem = TRUE;
							}
							m_SelectedQuestID = currentquestscriptitem->GetQuestID();
							memset( m_SelectedQuestName, 0, sizeof(TCHAR)*512 );
							memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );

							strcpy( m_SelectedQuestName, currentquestscriptitem->GetQuestName() );
							strcpy( m_SelectedQuestSubName, currentquestscriptitem->GetQuestSubName() );

							if( m_SelectedQuestSubName[0] != 0 )
								SetTempQuestSubName(m_TempSelectedQuestSubName, m_SelectedQuestSubName );

							ParseQuestDescString();
							return;					
						}		
					}
				}
			}	
		}
		else
		{			
			if( m_indexProgressQuestGroup == m_SelectedItemIndex )
			{
				bool tempselect = false;
				if( m_bSelectedProgressQuestGroup )
				{
					m_bSelectedProgressQuestGroup = FALSE;
				}
				else
				{
					m_bSelectedProgressQuestGroup = TRUE;
					tempselect = true;
				}
				SetListItem();
				if( tempselect )
				{
					if( !m_bSelectedProgressQuestGroup )
					{
						// 진행중인 퀘스트가 없습니다.
						m_SelectedQuestID = -1;
						m_listboxQuestReward->ShowWindow(FALSE);
						m_listboxQuestDesc->ShowWindow(FALSE);
						
						m_QuestNameBorder1->ShowWindow(FALSE);
						m_QuestNameBorder2->ShowWindow(FALSE);
						
						m_QuestCancelButton->ShowWindow(FALSE);
						m_QuestCompleteIconImage->ShowWindow(FALSE);
						
						m_listboxQuestDesc->DeleteAllItem();

						switch(m_SelectedTab) {
						case _XQUESTTAB_QUEST1:
							{						
								memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
								strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR1) );	// _T("진행중인 제룡행이 없습니다.")
							}
							break;
						case _XQUESTTAB_QUEST2:
							{					
								memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
								strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR2) );	// _T("진행중인 무림행이 없습니다.") );
							}
							break;
						case _XQUESTTAB_QUEST3:
							{
								memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
								strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERRPR3) );	// _T("진행중인 협객행이 없습니다.") );	
							}
							break;
						case _XQUESTTAB_QUEST4:
							{
								memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
								strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR4) );	// _T("진행중인 기연행이 없습니다.") );
							}
							break;
						case _XQUESTTAB_QUEST5:
							{
								memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
								strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR5) );	// _T("진행중인 무림소사가 없습니다.") );
							}
							break;
						}
						if( m_SelectedQuestSubName[0] != 0 )
							SetTempQuestSubName(m_TempSelectedQuestSubName, m_SelectedQuestSubName );
						
					}
				}
				return;
			}
			else if( m_indexCompleteQuestGroup == m_SelectedItemIndex )
			{
				bool tempselect = false;
				if( m_bSelectedCompleteQuestGroup )
				{
					m_bSelectedCompleteQuestGroup = FALSE;
				}
				else
				{
					m_bSelectedCompleteQuestGroup = TRUE;
					tempselect = true;
				}
				SetListItem();
				if( tempselect )
				{
					if( !m_bSelectedCompleteQuestGroup )
					{
						// 완료한 퀘스트가 없습니다.
						m_SelectedQuestID = -1;
						m_listboxQuestReward->ShowWindow(FALSE);
						m_listboxQuestDesc->ShowWindow(FALSE);
						
						m_QuestNameBorder1->ShowWindow(FALSE);
						m_QuestNameBorder2->ShowWindow(FALSE);
						
						m_QuestCancelButton->ShowWindow(FALSE);
						m_QuestCompleteIconImage->ShowWindow(FALSE);
						
						m_listboxQuestDesc->DeleteAllItem();
						switch(m_SelectedTab) {
						case _XQUESTTAB_QUEST1:
							{						
								memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
								strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR1) );	// _T("완료한 제룡행이 없습니다.") );
							}
							break;
						case _XQUESTTAB_QUEST2:
							{					
								memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
								strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR2) );	// _T("완료한 무림행이 없습니다.") );
							}
							break;
						case _XQUESTTAB_QUEST3:
							{
								memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
								strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERRPR3) );	// _T("완료한 협객행이 없습니다.") );
							}
							break;
						case _XQUESTTAB_QUEST4:
							{
								memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
								strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR4) );	// _T("완료한 기연행이 없습니다.") );
							}
							break;
						case _XQUESTTAB_QUEST5:
							{
								memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
								strcpy( m_SelectedQuestSubName, _XGETINTERFACETEXT(ID_STRING_QUEST_ERROR5) );	// _T("완료한 무림소사가 없습니다.") );
							}
							break;
						}
						if( m_SelectedQuestSubName[0] != 0 )
							SetTempQuestSubName(m_TempSelectedQuestSubName, m_SelectedQuestSubName );
					}
				}
				return;
			}
			
			if( m_indexProgressQuestGroup < m_SelectedItemIndex &&  m_indexCompleteQuestGroup > m_SelectedItemIndex )// 진행중인 퀘스트를 선택함 
			{
				// 무림 소사가 아닐 때 
				if( m_SelectedTab != _XQUESTTAB_QUEST5)
				{
					for( int i = 0; i < ID_QUEST_SYNC-1; i++)
					{
						if( g_QuestScriptManager.m_ProgressSubQuestID[i] !=  -1)
						{
							int questid = g_QuestScriptManager.m_ProgressSubQuestID[i];
							
							bool bvalidquest = false;
							switch(m_SelectedTab) {
							case _XQUESTTAB_QUEST2:
								{
									if( questid >= 20000 && questid < 30000) 
										bvalidquest = true;						
								}
								break;
							case _XQUESTTAB_QUEST3:
								{
									if( questid >= 30000 && questid < 40000) 
										bvalidquest = true;					
								}
								break;
							case _XQUESTTAB_QUEST4:
								{
									if( questid >= 40000 && questid < 50000) 
										bvalidquest = true;					
								}
								break;
							}
							
							if( bvalidquest )
							{
								map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
								if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
								{
									_XQuestScriptItem* currentquestscriptitem = iter_quest->second;
									if(currentquestscriptitem)
									{
										if( currentquestscriptitem->m_QuestScriptItemIndexNumber == m_SelectedItemIndex )
										{
											
											if( currentquestscriptitem->m_bSelectedQuestScriptItem )
											{
												currentquestscriptitem->m_bSelectedQuestScriptItem = FALSE;
											}
											else
											{
												currentquestscriptitem->m_bSelectedQuestScriptItem = TRUE;
											}
											m_SelectedQuestID = currentquestscriptitem->GetQuestID();
											
											memset( m_SelectedQuestName, 0, sizeof(TCHAR)*512 );
											memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
											
											if( currentquestscriptitem->GetQuestName() != NULL)
												strcpy( m_SelectedQuestName, currentquestscriptitem->GetQuestName() );
											if( currentquestscriptitem->GetQuestSubName() != NULL )
												strcpy( m_SelectedQuestSubName, currentquestscriptitem->GetQuestSubName() );
											
											if( m_SelectedQuestSubName[0] != 0 )
												SetTempQuestSubName(m_TempSelectedQuestSubName, m_SelectedQuestSubName );

											ParseQuestDescString();
											return;					
										}		
										
									}
								}
							}						
						}
					}
				}
				else // 무림소사일때 
				{
					if( !g_QuestScriptManager.m_listQuestNoSaveType.empty() )
					{		
						list <int>::iterator iter_questnosavetype;		
						iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.begin();
						while( iter_questnosavetype != g_QuestScriptManager.m_listQuestNoSaveType.end() )
						{			
							int questid = *iter_questnosavetype;			
							if( questid != -1 )
							{
								map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
								if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
								{
									_XQuestScriptItem* currentquestscriptitem = iter_quest->second;
									if(currentquestscriptitem)
									{
										if( currentquestscriptitem->m_QuestScriptItemIndexNumber == m_SelectedItemIndex )
										{
											
											if( currentquestscriptitem->m_bSelectedQuestScriptItem )
											{
												currentquestscriptitem->m_bSelectedQuestScriptItem = FALSE;
											}
											else
											{
												currentquestscriptitem->m_bSelectedQuestScriptItem = TRUE;
											}
											m_SelectedQuestID = currentquestscriptitem->GetQuestID();
											
											memset( m_SelectedQuestName, 0, sizeof(TCHAR)*512 );
											memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
											
											if( currentquestscriptitem->GetQuestName() != NULL)
												strcpy( m_SelectedQuestName, currentquestscriptitem->GetQuestName() );
											if( currentquestscriptitem->GetQuestSubName() != NULL )
												strcpy( m_SelectedQuestSubName, currentquestscriptitem->GetQuestSubName() );
											
											if( m_SelectedQuestSubName[0] != 0 )
												SetTempQuestSubName(m_TempSelectedQuestSubName, m_SelectedQuestSubName );

											ParseQuestDescString();
											return;					
										}		
										
									}
								}
							}
							iter_questnosavetype++;	
						}
					}
				}

			}
			else // 완료한 퀘스트를 선택함 
			{
				
				map <int, _XQUEST_COMPLETE_RESULT>::iterator iter;
				for(iter = g_QuestScriptManager.m_mapCompleteQuestList.begin() ; iter != g_QuestScriptManager.m_mapCompleteQuestList.end() ; iter++)
				{				
					int questid = iter->first;
					bool bvalidquest = false;
					switch(m_SelectedTab) {
					case _XQUESTTAB_QUEST2:
						{
							if( questid >= 20000 && questid < 30000) 
								bvalidquest = true;						
						}
						break;
					case _XQUESTTAB_QUEST3:
						{
							if( questid >= 30000 && questid < 40000) 
								bvalidquest = true;					
						}
						break;
					case _XQUESTTAB_QUEST4:
						{
							if( questid >= 40000 && questid < 50000) 
								bvalidquest = true;					
						}
						break;
					case _XQUESTTAB_QUEST5:
						{
							if( questid >= 0 && questid < 10000) 
								bvalidquest = true;					
						}
						break;
					}
					
					if( bvalidquest )
					{
						map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
						if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
						{
							_XQuestScriptItem* currentquestscriptitem = iter_quest->second;
							if(currentquestscriptitem)
							{		
								if( currentquestscriptitem->m_QuestScriptItemIndexNumber == m_SelectedItemIndex )
								{
									
									if( currentquestscriptitem->m_bSelectedQuestScriptItem )
									{
										currentquestscriptitem->m_bSelectedQuestScriptItem = FALSE;
									}
									else
									{
										currentquestscriptitem->m_bSelectedQuestScriptItem = TRUE;
									}
									m_SelectedQuestID = currentquestscriptitem->GetQuestID();
									
									memset( m_SelectedQuestName, 0, sizeof(TCHAR)*512 );
									memset( m_SelectedQuestSubName, 0, sizeof(TCHAR)*512 );
									
									if( currentquestscriptitem->GetQuestName() != NULL)
										strcpy( m_SelectedQuestName, currentquestscriptitem->GetQuestName() );
									if( currentquestscriptitem->GetQuestSubName() != NULL )
										strcpy( m_SelectedQuestSubName, currentquestscriptitem->GetQuestSubName() );
																		
									if( m_SelectedQuestSubName[0] != 0 )
										SetTempQuestSubName(m_TempSelectedQuestSubName, m_SelectedQuestSubName );

									ParseQuestDescString();
									return;					
								}					
							}		
						}
					}
				}
			}	
		}
	}

}

int _XWindow_QuestWindow::GetQuestGroupNumber(int mainquestid)
{
	int currentgroupnumber = -1;
	if( mainquestid >= 11000 && mainquestid < 12000 )// 개방
	{		
		currentgroupnumber = GetQuestGroupNumber1(mainquestid);		
	}
	else if( mainquestid >= 12000 && mainquestid < 13000)// 비궁
	{
		currentgroupnumber = GetQuestGroupNumber2(mainquestid);	
	}
	else if( mainquestid >= 13000 && mainquestid < 14000 )// 소림
	{
		currentgroupnumber = GetQuestGroupNumber3(mainquestid);	
	}
	else if( mainquestid >= 14000 && mainquestid < 15000 )// 녹림
	{
		currentgroupnumber = GetQuestGroupNumber4(mainquestid);	
	}
	else if( mainquestid >= 15000 && mainquestid < 16000 )// 무당
	{
		currentgroupnumber = GetQuestGroupNumber5(mainquestid);	
	}
	else if( mainquestid >= 16000 && mainquestid < 17000 )// 마교
	{
		currentgroupnumber = GetQuestGroupNumber6(mainquestid);	
	}

	return currentgroupnumber;
}
void _XWindow_QuestWindow::SetQuestGroupListItem(int mainquestid )
{	
	int questgroupnumber = 0;
	
	if( mainquestid != 65535 )
	{
		questgroupnumber = GetQuestGroupNumber( mainquestid );
	}
	else
	{
		int seachquestnumber = 0;
		int lastmainquestnumber = 0;
		switch(g_pLocalUser->m_CharacterInfo.groupindex) 
		{
		case _XGROUP_GAEBANG:
			{
				seachquestnumber = 1;
			}
			break;
		case _XGROUP_BEEGOONG:
			{
				seachquestnumber = 2;
			}
			break;
		case _XGROUP_SORIM:
			{
				seachquestnumber = 3;
			}
			break;
		case _XGROUP_NOCKRIM:
			{
				seachquestnumber = 4;
			}
			break;
		case _XGROUP_MOODANG :
			{
				seachquestnumber = 5;
			}
			break;
		case _XGROUP_MAKYO :
			{
				seachquestnumber = 6;
			}
			break;
		}
		_XQuestScriptItem* currentquest = NULL;
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.end();
		currentquest = iter_quest->second;
		if( currentquest ) 
		{
			lastmainquestnumber = currentquest->GetQuestID();
			questgroupnumber = GetQuestGroupNumber( lastmainquestnumber );
		}
	}
	
	if( questgroupnumber != -1)
	{
		if( m_QuestGroupNumber != questgroupnumber )
		{
			m_QuestGroupNumber = questgroupnumber;
			
			// 제룡행 퀘스트 장을 재설정한다.
			if( !m_QuestGroupList.listEmpty() )	m_QuestGroupList.disposeList();
			
			_XQuestScriptItemGroup* currentgroup = NULL;
			for( int i = 0; i < questgroupnumber; i++ )
			{ 
				currentgroup = new _XQuestScriptItemGroup;
				currentgroup->m_QuestGroupNumber = i+1;
				strcpy( currentgroup->m_QuestGroupName, g_QuestGroupNameString[i]);//퀘스트 장 이름 넣기 
				m_QuestGroupList.insertItem( currentgroup );
			}
		}
	}	

}

int _XWindow_QuestWindow::GetQuestGroupNumber1(int mainquestid)
{
	int questgroupnumber = 0;
	if(mainquestid >= 11000 && mainquestid < 11004)
	{
		questgroupnumber = 1;
	}
	else if( mainquestid >= 11004 && mainquestid < 11020 )
	{
		questgroupnumber = 2;
	}
	else if( mainquestid >= 11020 && mainquestid < 11030 )
	{
		questgroupnumber = 3;
	}
	else if( mainquestid >= 11030 && mainquestid < 11040 )
	{
		questgroupnumber = 4;
	}
	else if( mainquestid >= 11040 && mainquestid < 11050 )
	{
		questgroupnumber = 5;
	}
	else if( mainquestid >= 11050 && mainquestid < 11060 )
	{
		questgroupnumber = 6;
	}
	else if( mainquestid >= 11060 && mainquestid < 11070 )
	{
		questgroupnumber = 7;
	}
	else if( mainquestid >= 11070 && mainquestid < 11080 )
	{
		questgroupnumber = 8;
	}
	else if( mainquestid >= 11080 && mainquestid < 11090 )
	{
		questgroupnumber = 9;
	}
	else if( mainquestid >= 11090 && mainquestid < 11100 )
	{
		questgroupnumber = 10;
	}
	else if( mainquestid >= 11100 && mainquestid < 11110 )
	{
		questgroupnumber = 11;
	}
	else if( mainquestid >= 11110 && mainquestid < 11120 )
	{
		questgroupnumber = 12;
	}
	else if( mainquestid >= 11120 && mainquestid < 11130 )
	{
		questgroupnumber = 13;
	}
	else if( mainquestid >= 11130 && mainquestid < 11140 )
	{
		questgroupnumber = 14;
	}
	else if( mainquestid >= 11140 && mainquestid < 11150 )
	{
		questgroupnumber = 15;
	}
	else if( mainquestid >= 11150 && mainquestid < 11160 )
	{
		questgroupnumber = 16;
	}
	else if( mainquestid >= 11160 && mainquestid < 11170 )
	{
		questgroupnumber = 17;
	}
	else if( mainquestid >= 11170 && mainquestid < 11180 )
	{
		questgroupnumber = 18;
	}
	else if( mainquestid >= 11180 && mainquestid < 11190 )
	{
		questgroupnumber = 19;
	}
	else if( mainquestid >= 11190 && mainquestid < 11200 )
	{
		questgroupnumber = 20;
	}
	else if( mainquestid >= 11200 && mainquestid < 11210 )
	{
		questgroupnumber = 21;
	}
	else if( mainquestid >= 11210 && mainquestid < 11220 )
	{
		questgroupnumber = 22;
	}
	else if( mainquestid >= 11220 && mainquestid < 11230 )
	{
		questgroupnumber = 23;
	}
	else if( mainquestid >= 11230 && mainquestid < 11240 )
	{
		questgroupnumber = 24;
	}
	else if( mainquestid >= 11240 && mainquestid < 11250 )
	{
		questgroupnumber = 25;
	}

	return questgroupnumber;
}
int _XWindow_QuestWindow::GetQuestGroupNumber2(int mainquestid)
{
	int questgroupnumber = 0;
	if(mainquestid >= 12000 && mainquestid < 12004)
	{
		questgroupnumber = 1;
	}
	else if( mainquestid >= 12004 && mainquestid < 12020 )
	{
		questgroupnumber = 2;
	}
	else if( mainquestid >= 12020 && mainquestid < 12030 )
	{
		questgroupnumber = 3;
	}
	else if( mainquestid >= 12030 && mainquestid < 12040 )
	{
		questgroupnumber = 4;
	}
	else if( mainquestid >= 12040 && mainquestid < 12050 )
	{
		questgroupnumber = 5;
	}
	else if( mainquestid >= 12050 && mainquestid < 12060 )
	{
		questgroupnumber = 6;
	}
	else if( mainquestid >= 12060 && mainquestid < 12070 )
	{
		questgroupnumber = 7;
	}
	else if( mainquestid >= 12070 && mainquestid < 12080 )
	{
		questgroupnumber = 8;
	}
	else if( mainquestid >= 12080 && mainquestid < 12090 )
	{
		questgroupnumber = 9;
	}
	else if( mainquestid >= 12090 && mainquestid < 12100 )
	{
		questgroupnumber = 10;
	}
	else if( mainquestid >= 12100 && mainquestid < 12110 )
	{
		questgroupnumber = 11;
	}
	else if( mainquestid >= 12110 && mainquestid < 12120 )
	{
		questgroupnumber = 12;
	}
	else if( mainquestid >= 12120 && mainquestid < 12130 )
	{
		questgroupnumber = 13;
	}
	else if( mainquestid >= 12130 && mainquestid < 12140 )
	{
		questgroupnumber = 14;
	}
	else if( mainquestid >= 12140 && mainquestid < 12150 )
	{
		questgroupnumber = 15;
	}
	else if( mainquestid >= 12150 && mainquestid < 12160 )
	{
		questgroupnumber = 16;
	}
	else if( mainquestid >= 12160 && mainquestid < 12170 )
	{
		questgroupnumber = 17;
	}
	else if( mainquestid >= 12170 && mainquestid < 12180 )
	{
		questgroupnumber = 18;
	}
	else if( mainquestid >= 12180 && mainquestid < 12190 )
	{
		questgroupnumber = 19;
	}
	else if( mainquestid >= 12190 && mainquestid < 12200 )
	{
		questgroupnumber = 20;
	}
	else if( mainquestid >= 12200 && mainquestid < 12210 )
	{
		questgroupnumber = 21;
	}
	else if( mainquestid >= 12210 && mainquestid < 12220 )
	{
		questgroupnumber = 22;
	}
	else if( mainquestid >= 12220 && mainquestid < 12230 )
	{
		questgroupnumber = 23;
	}
	else if( mainquestid >= 12230 && mainquestid < 12240 )
	{
		questgroupnumber = 24;
	}
	else if( mainquestid >= 12240 && mainquestid < 12250 )
	{
		questgroupnumber = 25;
	}
	return questgroupnumber;
}
int _XWindow_QuestWindow::GetQuestGroupNumber3(int mainquestid)
{
	int questgroupnumber = 0;
	if(mainquestid >= 13000 && mainquestid < 13003)
	{
		questgroupnumber = 1;
	}
	else if( mainquestid >= 13003 && mainquestid < 13020 )
	{
		questgroupnumber = 2;
	}
	else if( mainquestid >= 13020 && mainquestid < 13030 )
	{
		questgroupnumber = 3;
	}
	else if( mainquestid >= 13030 && mainquestid < 13040 )
	{
		questgroupnumber = 4;
	}
	else if( mainquestid >= 13040 && mainquestid < 13050 )
	{
		questgroupnumber = 5;
	}
	else if( mainquestid >= 13050 && mainquestid < 13060 )
	{
		questgroupnumber = 6;
	}
	else if( mainquestid >= 13060 && mainquestid < 13070 )
	{
		questgroupnumber = 7;
	}
	else if( mainquestid >= 13070 && mainquestid < 13080 )
	{
		questgroupnumber = 8;
	}
	else if( mainquestid >= 13080 && mainquestid < 13090 )
	{
		questgroupnumber = 9;
	}
	else if( mainquestid >= 13090 && mainquestid < 13100 )
	{
		questgroupnumber = 10;
	}
	else if( mainquestid >= 13100 && mainquestid < 13110 )
	{
		questgroupnumber = 11;
	}
	else if( mainquestid >= 13110 && mainquestid < 13120 )
	{
		questgroupnumber = 12;
	}
	else if( mainquestid >= 13120 && mainquestid < 13130 )
	{
		questgroupnumber = 13;
	}
	else if( mainquestid >= 13130 && mainquestid < 13140 )
	{
		questgroupnumber = 14;
	}
	else if( mainquestid >= 13140 && mainquestid < 13150 )
	{
		questgroupnumber = 15;
	}
	else if( mainquestid >= 13150 && mainquestid < 13160 )
	{
		questgroupnumber = 16;
	}
	else if( mainquestid >= 13160 && mainquestid < 13170 )
	{
		questgroupnumber = 17;
	}
	else if( mainquestid >= 13170 && mainquestid < 13180 )
	{
		questgroupnumber = 18;
	}
	else if( mainquestid >= 13180 && mainquestid < 13190 )
	{
		questgroupnumber = 19;
	}
	else if( mainquestid >= 13190 && mainquestid < 13200 )
	{
		questgroupnumber = 20;
	}
	else if( mainquestid >= 13200 && mainquestid < 13210 )
	{
		questgroupnumber = 21;
	}
	else if( mainquestid >= 13210 && mainquestid < 13220 )
	{
		questgroupnumber = 22;
	}
	else if( mainquestid >= 13220 && mainquestid < 13230 )
	{
		questgroupnumber = 23;
	}
	else if( mainquestid >= 13230 && mainquestid < 13240 )
	{
		questgroupnumber = 24;
	}
	else if( mainquestid >= 13240 && mainquestid < 13250 )
	{
		questgroupnumber = 25;
	}
	return questgroupnumber;
}
int _XWindow_QuestWindow::GetQuestGroupNumber4(int mainquestid)
{
	int questgroupnumber = 0;
	if(mainquestid >= 14000 && mainquestid < 14004)
	{
		questgroupnumber = 1;
	}
	else if( mainquestid >= 14004 && mainquestid < 14020 )
	{
		questgroupnumber = 2;
	}
	else if( mainquestid >= 14020 && mainquestid < 14030 )
	{
		questgroupnumber = 3;
	}
	else if( mainquestid >= 14030 && mainquestid < 14040 )
	{
		questgroupnumber = 4;
	}
	else if( mainquestid >= 14040 && mainquestid < 14050 )
	{
		questgroupnumber = 5;
	}
	else if( mainquestid >= 14050 && mainquestid < 14060 )
	{
		questgroupnumber = 6;
	}
	else if( mainquestid >= 14060 && mainquestid < 14070 )
	{
		questgroupnumber = 7;
	}
	else if( mainquestid >= 14070 && mainquestid < 14080 )
	{
		questgroupnumber = 8;
	}
	else if( mainquestid >= 14080 && mainquestid < 14090 )
	{
		questgroupnumber = 9;
	}
	else if( mainquestid >= 14090 && mainquestid < 14100 )
	{
		questgroupnumber = 10;
	}
	else if( mainquestid >= 14100 && mainquestid < 14110 )
	{
		questgroupnumber = 11;
	}
	else if( mainquestid >= 14110 && mainquestid < 14120 )
	{
		questgroupnumber = 12;
	}
	else if( mainquestid >= 14120 && mainquestid < 14130 )
	{
		questgroupnumber = 13;
	}
	else if( mainquestid >= 14130 && mainquestid < 14140 )
	{
		questgroupnumber = 14;
	}
	else if( mainquestid >= 14140 && mainquestid < 14150 )
	{
		questgroupnumber = 15;
	}
	else if( mainquestid >= 14150 && mainquestid < 14160 )
	{
		questgroupnumber = 16;
	}
	else if( mainquestid >= 14160 && mainquestid < 14170 )
	{
		questgroupnumber = 17;
	}
	else if( mainquestid >= 14170 && mainquestid < 14180 )
	{
		questgroupnumber = 18;
	}
	else if( mainquestid >= 14180 && mainquestid < 14190 )
	{
		questgroupnumber = 19;
	}
	else if( mainquestid >= 14190 && mainquestid < 14200 )
	{
		questgroupnumber = 20;
	}
	else if( mainquestid >= 14200 && mainquestid < 14210 )
	{
		questgroupnumber = 21;
	}
	else if( mainquestid >= 14210 && mainquestid < 14220 )
	{
		questgroupnumber = 22;
	}
	else if( mainquestid >= 14220 && mainquestid < 14230 )
	{
		questgroupnumber = 23;
	}
	else if( mainquestid >= 14230 && mainquestid < 14240 )
	{
		questgroupnumber = 24;
	}
	else if( mainquestid >= 14240 && mainquestid < 14250 )
	{
		questgroupnumber = 25;
	}
	return questgroupnumber;
}

int _XWindow_QuestWindow::GetQuestGroupNumber5(int mainquestid)
{
	int questgroupnumber = 0;
	if(mainquestid == 15000 )
	{
		questgroupnumber = 1;
	}
	else if( mainquestid >= 15001 && mainquestid < 15020 )
	{
		questgroupnumber = 2;
	}
	else if( mainquestid >= 15020 && mainquestid < 15030 )
	{
		questgroupnumber = 3;
	}
	else if( mainquestid >= 15030 && mainquestid < 15040 )
	{
		questgroupnumber = 4;
	}
	else if( mainquestid >= 15040 && mainquestid < 15050 )
	{
		questgroupnumber = 5;
	}
	else if( mainquestid >= 15050 && mainquestid < 15060 )
	{
		questgroupnumber = 6;
	}
	else if( mainquestid >= 15060 && mainquestid < 15070 )
	{
		questgroupnumber = 7;
	}
	else if( mainquestid >= 15070 && mainquestid < 15080 )
	{
		questgroupnumber = 8;
	}
	else if( mainquestid >= 15080 && mainquestid < 15090 )
	{
		questgroupnumber = 9;
	}
	else if( mainquestid >= 15090 && mainquestid < 15100 )
	{
		questgroupnumber = 10;
	}
	else if( mainquestid >= 15100 && mainquestid < 15110 )
	{
		questgroupnumber = 11;
	}
	else if( mainquestid >= 15110 && mainquestid < 15120 )
	{
		questgroupnumber = 12;
	}
	else if( mainquestid >= 15120 && mainquestid < 15130 )
	{
		questgroupnumber = 13;
	}
	else if( mainquestid >= 15130 && mainquestid < 15140 )
	{
		questgroupnumber = 14;
	}
	else if( mainquestid >= 15140 && mainquestid < 15150 )
	{
		questgroupnumber = 15;
	}
	else if( mainquestid >= 15150 && mainquestid < 15160 )
	{
		questgroupnumber = 16;
	}
	else if( mainquestid >= 15160 && mainquestid < 15170 )
	{
		questgroupnumber = 17;
	}
	else if( mainquestid >= 15170 && mainquestid < 15180 )
	{
		questgroupnumber = 18;
	}
	else if( mainquestid >= 15180 && mainquestid < 15190 )
	{
		questgroupnumber = 19;
	}
	else if( mainquestid >= 15190 && mainquestid < 15200 )
	{
		questgroupnumber = 20;
	}
	else if( mainquestid >= 15200 && mainquestid < 15210 )
	{
		questgroupnumber = 21;
	}
	else if( mainquestid >= 15210 && mainquestid < 15220 )
	{
		questgroupnumber = 22;
	}
	else if( mainquestid >= 15220 && mainquestid < 15230 )
	{
		questgroupnumber = 23;
	}
	else if( mainquestid >= 15230 && mainquestid < 15240 )
	{
		questgroupnumber = 24;
	}
	else if( mainquestid >= 15240 && mainquestid < 15250 )
	{
		questgroupnumber = 25;
	}
	return questgroupnumber;
}

int _XWindow_QuestWindow::GetQuestGroupNumber6(int mainquestid)
{
	int questgroupnumber = 0;
	if(mainquestid == 16000 )
	{
		questgroupnumber = 1;
	}
	else if( mainquestid >= 16001 && mainquestid < 16020 )
	{
		questgroupnumber = 2;
	}
	else if( mainquestid >= 16020 && mainquestid < 16030 )
	{
		questgroupnumber = 3;
	}
	else if( mainquestid >= 16030 && mainquestid < 16040 )
	{
		questgroupnumber = 4;
	}
	else if( mainquestid >= 16040 && mainquestid < 16050 )
	{
		questgroupnumber = 5;
	}
	else if( mainquestid >= 16050 && mainquestid < 16060 )
	{
		questgroupnumber = 6;
	}
	else if( mainquestid >= 16060 && mainquestid < 16070 )
	{
		questgroupnumber = 7;
	}
	else if( mainquestid >= 16070 && mainquestid < 16080 )
	{
		questgroupnumber = 8;
	}
	else if( mainquestid >= 16080 && mainquestid < 16090 )
	{
		questgroupnumber = 9;
	}
	else if( mainquestid >= 16090 && mainquestid < 16100 )
	{
		questgroupnumber = 10;
	}
	else if( mainquestid >= 16100 && mainquestid < 16110 )
	{
		questgroupnumber = 11;
	}
	else if( mainquestid >= 16110 && mainquestid < 16120 )
	{
		questgroupnumber = 12;
	}
	else if( mainquestid >= 16120 && mainquestid < 16130 )
	{
		questgroupnumber = 13;
	}
	else if( mainquestid >= 16130 && mainquestid < 16140 )
	{
		questgroupnumber = 14;
	}
	else if( mainquestid >= 16140 && mainquestid < 16150 )
	{
		questgroupnumber = 15;
	}
	else if( mainquestid >= 16150 && mainquestid < 16160 )
	{
		questgroupnumber = 16;
	}
	else if( mainquestid >= 16160 && mainquestid < 16170 )
	{
		questgroupnumber = 17;
	}
	else if( mainquestid >= 16170 && mainquestid < 16180 )
	{
		questgroupnumber = 18;
	}
	else if( mainquestid >= 16180 && mainquestid < 16190 )
	{
		questgroupnumber = 19;
	}
	else if( mainquestid >= 16190 && mainquestid < 16200 )
	{
		questgroupnumber = 20;
	}
	else if( mainquestid >= 16200 && mainquestid < 16210 )
	{
		questgroupnumber = 21;
	}
	else if( mainquestid >= 16210 && mainquestid < 16220 )
	{
		questgroupnumber = 22;
	}
	else if( mainquestid >= 16220 && mainquestid < 16230 )
	{
		questgroupnumber = 23;
	}
	else if( mainquestid >= 16230 && mainquestid < 16240 )
	{
		questgroupnumber = 24;
	}
	else if( mainquestid >= 16240 && mainquestid < 16250 )
	{
		questgroupnumber = 25;
	}
	return questgroupnumber;
}



void _XWindow_QuestWindow::SetQuestTabButton(void)
{
	int quest2count = 0;
	int quest3count = 0;
	int quest4count = 0;
	int quest5count = 0;
	m_GetTotalQuestCount = 0;
	
	for ( int i = 0; i < 5; i++ )
	{
		if( g_QuestScriptManager.m_ProgressSubQuestID[i] != -1 )
		{
			if( g_QuestScriptManager.m_ProgressSubQuestID[i] >= 20000 && g_QuestScriptManager.m_ProgressSubQuestID[i] < 30000 )//무림행	
			{		
				quest2count++;
			}
			else if( g_QuestScriptManager.m_ProgressSubQuestID[i] >= 30000 && g_QuestScriptManager.m_ProgressSubQuestID[i] < 40000 )//협객행
			{
				quest3count++;
			}
			else if( g_QuestScriptManager.m_ProgressSubQuestID[i] >= 40000 && g_QuestScriptManager.m_ProgressSubQuestID[i] < 50000 )//기연행
			{		
				quest4count++;		
			}
		}
	}
	
	if( !g_QuestScriptManager.m_listQuestNoSaveType.empty() )
	{
		list <int>::iterator iter_questnosavetype;
		for(iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.begin() ; iter_questnosavetype != g_QuestScriptManager.m_listQuestNoSaveType.end() ; iter_questnosavetype++)
		{
			int questid = *iter_questnosavetype;
			if( questid >= 50000 && questid < 60000 )
			{
				quest5count++;
			}
		}
	}
	
	if( g_QuestScriptManager.m_bProgressMainQuest )
	{
		m_QuestTab[0]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_QUEST_TYPE1COUNT1 );	// _T("제룡행(1)")
	}
	else
	{
		m_QuestTab[0]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_QUEST_TYPE1COUNT0 );	// _T("제룡행(0)")
	}
	
	TCHAR	questtabnamebuffer[64];
	memset(questtabnamebuffer, 0, sizeof(TCHAR) * 64);
	sprintf( questtabnamebuffer, _XGETINTERFACETEXT(ID_STRING_QUEST_TYPE2COUNT),quest2count );	// "무림행(%d)"
	m_QuestTab[1]->SetButtonText( questtabnamebuffer );
	
	memset(questtabnamebuffer, 0, sizeof(TCHAR) * 64);
	sprintf( questtabnamebuffer, _XGETINTERFACETEXT(ID_STRING_QUEST_TYPE3COUNT),quest3count );	// "협객행(%d)"
	m_QuestTab[2]->SetButtonText( questtabnamebuffer );
	
	memset(questtabnamebuffer, 0, sizeof(TCHAR) * 64);
	sprintf( questtabnamebuffer, _XGETINTERFACETEXT(ID_STRING_QUEST_TYPE4COUNT),quest4count );	// "기연행(%d)"
	m_QuestTab[3]->SetButtonText( questtabnamebuffer );
	
	memset(questtabnamebuffer, 0, sizeof(TCHAR) * 64);
	sprintf( questtabnamebuffer, _XGETINTERFACETEXT(ID_STRING_QUEST_TYPE5COUNT),quest5count );	// "무림소사(%d)"
	m_QuestTab[4]->SetButtonText( questtabnamebuffer );
 
	m_GetTotalQuestCount = quest2count + quest3count + quest4count + quest5count;

}

void _XWindow_QuestWindow::SetTempQuestSubName( TCHAR* comsubname, TCHAR* srcsubname )
{
	
	memset( comsubname, 0, sizeof(TCHAR)*512 );
	
	int length = strlen( srcsubname );	
	
	if( length > 54 )
	{
		if( srcsubname[54] & 0x80 ) //한글일때 2바이트 
		{
			int compairindex = 0;
			while (compairindex < 54) 
			{
				if( srcsubname[compairindex] & 0x80 )
					compairindex+= 2;
				else
					compairindex++;
			}
			
			strncpy( comsubname, srcsubname, sizeof(TCHAR)*compairindex );	
			strcat( comsubname, _T(" ...") );
		}
		else
		{
			strncpy( comsubname, srcsubname, sizeof(TCHAR)*54 );	
			strcat( comsubname, _T(" ...") );
		}	
		m_bOverFlowQuestSubName = TRUE;
	}
	else
	{
		strcpy( comsubname, srcsubname );
		m_bOverFlowQuestSubName = FALSE;
	}
}