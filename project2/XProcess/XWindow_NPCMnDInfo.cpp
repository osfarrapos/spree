// XWindow_NPCMnDInfo.cpp: implementation of the _XWindow_NPCMnDInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_QuestRank.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_NPCMnDInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_NPCMnDInfo::_XWindow_NPCMnDInfo() : m_pMasterInfoListBox(NULL),
											 m_nListBoxCount(0)
{

}

_XWindow_NPCMnDInfo::~_XWindow_NPCMnDInfo()
{

}


BOOL _XWindow_NPCMnDInfo::Initialize(void)
{
	int mainframeinterface		= g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	int npcresourceindex4		= g_MainInterfaceTextureArchive.FindResource("mi_npcquestwin_04.tga" );

	int pvpresourceindex = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("tw_mi_pvp_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("us_mi_pvp_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("rs_mi_pvp_01.tga");
	}
	else
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_pvp_01.tga");
	}
	
	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create(0, 0, 180, 18, &g_MainInterfaceTextureArchive, mainframeinterface);
	pTitleLeftBar->SetClipRect(0, 125, 180, 143);
	InsertChildObject(pTitleLeftBar);
	
	_XImageStatic* pTitleCenterBar = new _XImageStatic;
	pTitleCenterBar->Create(180, 0, 282, 18, &g_MainInterfaceTextureArchive, mainframeinterface);
	pTitleCenterBar->SetClipRect(77, 125, 180, 143);
	InsertChildObject(pTitleCenterBar);
	
	_XImageStatic* pTitleRightBar = new _XImageStatic;
	pTitleRightBar->Create(282, 0, 365, 18, &g_MainInterfaceTextureArchive, mainframeinterface);
	pTitleRightBar->SetClipRect(99, 125, 183, 143);
	InsertChildObject(pTitleRightBar);

	m_BorderTitle.Create(m_WindowPosition.x+1, m_WindowPosition.y+18, m_WindowPosition.x+365, m_WindowPosition.y+268);
	m_BorderTitle.SetTexture(&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_BorderTile.tga"));
	m_BorderTitle.SetFaceColor(D3DCOLOR_ARGB(128, 255, 255, 255));

	FLOAT ufactor = 364.0f/128.0f;
	FLOAT vfactor = 250.0f/128.0f;

	m_BorderTitle.ChangeUV(0.0f, ufactor, ufactor, 0.0f, 0.0f, 0.0f, vfactor, vfactor);
	
	_XImageStatic* pLeftBorder_1 = new _XImageStatic;
	pLeftBorder_1->Create(0, 17, 2, 48, &g_MainInterfaceTextureArchive, mainframeinterface);
	pLeftBorder_1->SetClipRect(0, 195, 3, 227);
	pLeftBorder_1->SetScale(1.0f, 7.2f);
	InsertChildObject(pLeftBorder_1);
	
	_XImageStatic* pRightBorder_1 = new _XImageStatic;
	pRightBorder_1->Create(363, 17, 365, 48, &g_MainInterfaceTextureArchive, mainframeinterface);
	pRightBorder_1->SetClipRect(0, 195, 3, 227);
	pRightBorder_1->SetScale(1.0f, 7.2f);
	InsertChildObject(pRightBorder_1);
	
	_XImageStatic* pLeftCorner = new _XImageStatic;
	pLeftCorner->Create(0, 245, 2, 269, &g_MainInterfaceTextureArchive, mainframeinterface);
	pLeftCorner->SetClipRect(0, 205, 3, 230);
	InsertChildObject(pLeftCorner);
	
	_XImageStatic* pRightCorner = new _XImageStatic;
	pRightCorner->Create(363, 245, 365, 269, &g_MainInterfaceTextureArchive, mainframeinterface);
	pRightCorner->SetClipRect(180, 205, 183, 230);
	InsertChildObject(pRightCorner);
	
	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create(3, 267, 179, 269, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBottomBorder->SetClipRect(3, 227, 180, 230);
	pBottomBorder->SetScale(2.033f, 1.0f);
	InsertChildObject(pBottomBorder);

	_XImageStatic* pDragonBorder_1 = new _XImageStatic;
	pDragonBorder_1->Create(5, 18, 181, 73, &g_MainInterfaceTextureArchive, npcresourceindex4);
	pDragonBorder_1->SetClipRect(4, 133, 180, 182);
	InsertChildObject(pDragonBorder_1);

	_XImageStatic* pDragonBorder_2 = new _XImageStatic;
	pDragonBorder_2->Create(357, 18, 180, 73, &g_MainInterfaceTextureArchive, npcresourceindex4);
	pDragonBorder_2->SetClipRect(4, 133, 180, 182);
	pDragonBorder_2->SetScale(-1.0f, 1.0f);
	InsertChildObject(pDragonBorder_2);

	//안에 테투리
	_XImageStatic* pBorder_1 = new _XImageStatic;
	pBorder_1->Create(5, 75, 359, 75, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_1->SetClipRect(9, 193, 175, 194);
	pBorder_1->SetScale(2.138f, 1.0f);
	InsertChildObject(pBorder_1);

	_XImageStatic* pBorder_2 = new _XImageStatic;
	pBorder_2->Create(5, 227, 359, 227, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_2->SetClipRect(9, 193, 175, 194);
	pBorder_2->SetScale(2.138f, 1.0f);
	InsertChildObject(pBorder_2);

	_XImageStatic* pBorder_3 = new _XImageStatic;
	pBorder_3->Create(4, 76, 4, 226, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_3->SetClipRect(192, 64, 193, 214);
	pBorder_3->SetScale(1.0f, 1.0f);
	InsertChildObject(pBorder_3);

	_XImageStatic* pBorder_4 = new _XImageStatic;
	pBorder_4->Create(360, 76, 360, 226, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_4->SetClipRect(192, 64, 193, 214);
	pBorder_4->SetScale(1.0f, 1.0f);
	InsertChildObject(pBorder_4);

	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
	int optionresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_system.tga");

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {50, 237}, {60, 24}, _XDEF_WTITLE_NPCMNDAPPLYBUTTON,
			optionresourceindex,
			optionresourceindex,
			optionresourceindex,
			&g_MainInterfaceTextureArchive
	};

	btnstruct.position.x = 215;

	_XButton* pReqButton = new _XButton;
	pReqButton->Create(btnstruct);
	//pReqButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 176, 110, 199);
	//pReqButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 152, 110, 175);
	//pReqButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 201, 110, 224);
	pReqButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 187, 127, 247, 151);
	pReqButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 187, 152, 247, 176);
	pReqButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 187, 177, 247, 201);
	pReqButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	pReqButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3865); //사제 신청
	InsertChildObject(pReqButton);

	btnstruct.position.x = 293;
	btnstruct.position.y = 237;
	btnstruct.windowsize.cx = 60;
	btnstruct.windowsize.cy = 24;
	btnstruct.commandid = _XDEF_WTITLE_NPCMNDREQLISTBTN;
	btnstruct.imageindex_click = optionresourceindex;
	btnstruct.imageindex_normal = optionresourceindex;
	btnstruct.imageindex_ready = optionresourceindex;

	_XButton* pReqListButton = new _XButton;
	pReqListButton->Create(btnstruct);
	pReqListButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 187, 127, 247, 151);
	pReqListButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 187, 152, 247, 176);
	pReqListButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 187, 177, 247, 201);
	pReqListButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	pReqListButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2557); //목록갱신
	InsertChildObject(pReqListButton);

	int scrollbarresourceindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		// window property
		TRUE,										// 윈도우 활성화
		{6, 75},									// 윈도우 좌표 
		{353, 150},									// 윈도우 사이즈
		_XDEF_PVPBROKER_LISTBOX,					// 윈도우 아이디
		-1,											// 윈도우 볼더 이미지 인덱스
		&g_MainInterfaceTextureArchive,				// 텍스쳐리스트
		_T(""),										// 윈도우 타이틀 
		18,										// 리스트 아이템 라인 피치
		10,											// 리스트 박스에 보이는 최대 라인수 
		_XLSTYLE_RIGHTSCROLLBAR,					// 스크롤바의 위치 		
		
		// Scroll bar property
		{12, 150},									// 스크롤바 사이즈
		12,											// 스크롤바 상하버튼 사이즈
		22,											// 스크롤바 트랙바 사이즈
		150,										// 전체 리스트 갯수 <설정 필요없슴>			
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex		
	};

	m_pMasterInfoListBox = new _XListBox;
	m_pMasterInfoListBox->Create(listboxstructure);

	_XVScrollBar* pScrollBar = m_pMasterInfoListBox->GetScrollBarObject();
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

	POINT columnoffset1 = { 7, 0  };
	POINT columnoffset2 = { 17+38, 2 };
	POINT columnoffset3 = { 95+20, 2 };
	POINT columnoffset4 = { 137+32, 2};
	POINT columnoffset5 = { 203+8, 2};
	POINT columnoffset6 = { 221+38, 2};

	m_pMasterInfoListBox->SetColumnOffset(0, columnoffset1);
	m_pMasterInfoListBox->SetColumnOffset(1, columnoffset2);
	m_pMasterInfoListBox->SetColumnOffset(2, columnoffset3);
	m_pMasterInfoListBox->SetColumnOffset(3, columnoffset4);
	m_pMasterInfoListBox->SetColumnOffset(4, columnoffset5);
	m_pMasterInfoListBox->SetColumnOffset(5, columnoffset6);

	m_pMasterInfoListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);
	m_pMasterInfoListBox->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_pMasterInfoListBox->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	m_pMasterInfoListBox->SetColumnStyle(3, _XLCOLUMNSTYLE_STRING);
	m_pMasterInfoListBox->SetColumnStyle(4, _XLCOLUMNSTYLE_STRING);
	m_pMasterInfoListBox->SetColumnStyle(5, _XLCOLUMNSTYLE_STRING);

	m_pMasterInfoListBox->SetAlignType(0, _XFONT_ALIGNTYPE_CENTER);
	m_pMasterInfoListBox->SetAlignType(1, _XFONT_ALIGNTYPE_CENTER);
	m_pMasterInfoListBox->SetAlignType(2, _XFONT_ALIGNTYPE_CENTER);
	m_pMasterInfoListBox->SetAlignType(3, _XFONT_ALIGNTYPE_CENTER);
	m_pMasterInfoListBox->SetAlignType(4, _XFONT_ALIGNTYPE_CENTER);
	m_pMasterInfoListBox->SetAlignType(5, _XFONT_ALIGNTYPE_CENTER);

	m_pMasterInfoListBox->SetColumnWidth(0, 15);
	m_pMasterInfoListBox->SetColumnWidth(1, 76);
	m_pMasterInfoListBox->SetColumnWidth(2, 40);
	m_pMasterInfoListBox->SetColumnWidth(3, 64);
	m_pMasterInfoListBox->SetColumnWidth(4, 16);
	m_pMasterInfoListBox->SetColumnWidth(5, 76);

	m_pMasterInfoListBox->SetTrackWidth(338);
	m_pMasterInfoListBox->SetTrackHeight(18);
	InsertChildObject(m_pMasterInfoListBox);

	SetWindowMoveMode(_XWMOVE_FIXED);

	return TRUE;
}

void _XWindow_NPCMnDInfo::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_NPCMnDInfo::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		m_BorderTitle.RenderAlphaBlend(D3DCOLOR_ARGB(128, 255, 255, 255));
	}

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow) 
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	g_XBaseFont->SetBoldMode(TRUE);
	g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT); 
	g_XBaseFont->Puts(m_WindowPosition.x+40, m_WindowPosition.y+45, _XGETINTERFACETEXT(ID_STRING_NEW_3849), 1.0f);//_T("등급")
	g_XBaseFont->Puts(m_WindowPosition.x+100, m_WindowPosition.y+45, _XGETINTERFACETEXT(ID_STRING_NEW_3866), 1.0f); //_T("사호")
	g_XBaseFont->Puts(m_WindowPosition.x+160, m_WindowPosition.y+45, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_NAME), 1.0f); //("이름")
	g_XBaseFont->Puts(m_WindowPosition.x+220, m_WindowPosition.y+45, _XGETINTERFACETEXT(ID_STRING_NEW_3867), 1.0f); //_T("문파")
	g_XBaseFont->Puts(m_WindowPosition.x+280, m_WindowPosition.y+45, _XGETINTERFACETEXT(ID_STRING_NEW_3848), 1.0f); //_T("직책")
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	g_XBaseFont->Flush();
	g_XBaseFont->SetBoldMode(FALSE);

}

BOOL _XWindow_NPCMnDInfo::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_NPCMnDInfo::ShowWindow(BOOL show)
{
	if(show)
	{
		_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
		if(pNPCScript_Window) 
		{
			if( pNPCScript_Window->m_MessageType == _XDEF_NPCMESSAGE_QUEST )
			{
				ShowWindow(FALSE);
				return;
			}
			else
			{
				MoveWindow( (gnWidth>>1)+101, (gnHeight>>1)-154 );
				//g_NetworkKernel.SendPacket(MSG_NO_QUESTRANK_REQ, m_RankQuestID );
			}
		}
		
	}
	else
	{
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_NPCMnDInfo::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);

	m_BorderTitle.MoveWindow(m_WindowPosition.x+1, m_WindowPosition.y+18);

}
