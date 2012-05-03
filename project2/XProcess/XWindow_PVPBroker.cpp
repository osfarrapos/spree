// XWindow_PVPBroker.cpp: implementation of the _XWindow_PVPBroker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_PVPBroker.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_PVPBroker::_XWindow_PVPBroker()
{
	m_MatchTargetListBox = NULL;
	m_ListBoxCount = 0;
}

_XWindow_PVPBroker::~_XWindow_PVPBroker()
{

}

BOOL _XWindow_PVPBroker::Initialize(void)
{
	int mainframeinterface = g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	
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

	_XImageStatic* pBorder_1 = new _XImageStatic;
	pBorder_1->Create(5, 23, 359, 23, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_1->SetClipRect(9, 193, 175, 194);
	pBorder_1->SetScale(2.138f, 1.0f);
	InsertChildObject(pBorder_1);

	_XImageStatic* pBorder_2 = new _XImageStatic;
	pBorder_2->Create(5, 44, 359, 44, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_2->SetClipRect(9, 193, 175, 194);
	pBorder_2->SetScale(2.138f, 1.0f);
	InsertChildObject(pBorder_2);

	_XImageStatic* pBorder_3 = new _XImageStatic;
	pBorder_3->Create(5, 227, 359, 227, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_3->SetClipRect(9, 193, 175, 194);
	pBorder_3->SetScale(2.138f, 1.0f);
	InsertChildObject(pBorder_3);

	_XImageStatic* pBorder_4 = new _XImageStatic;
	pBorder_4->Create(4, 24, 4, 226, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_4->SetClipRect(192, 64, 193, 230);
	pBorder_4->SetScale(1.0f, 1.223f);
	InsertChildObject(pBorder_4);

	_XImageStatic* pBorder_5 = new _XImageStatic;
	pBorder_5->Create(360, 24, 360, 226, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBorder_5->SetClipRect(192, 64, 193, 230);
	pBorder_5->SetScale(1.0f, 1.223f);
	InsertChildObject(pBorder_5);

	/*int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 349, 2 }, { 12, 12 }, _XDEF_PVPBROKER_CLOSE,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );*/

	// 제목 바닥
	m_Static[0].Create(m_WindowPosition.x+7, m_WindowPosition.y+27, m_WindowPosition.x+20, m_WindowPosition.y+40, 1.0f, D3DCOLOR_ARGB(255, 83, 39, 20));
	m_Static[1].Create(m_WindowPosition.x+24, m_WindowPosition.y+27, m_WindowPosition.x+98, m_WindowPosition.y+40, 1.0f, D3DCOLOR_ARGB(255, 83, 39, 20));
	m_Static[2].Create(m_WindowPosition.x+102, m_WindowPosition.y+27, m_WindowPosition.x+140, m_WindowPosition.y+40, 1.0f, D3DCOLOR_ARGB(255, 83, 39, 20));
	m_Static[3].Create(m_WindowPosition.x+144, m_WindowPosition.y+27, m_WindowPosition.x+206, m_WindowPosition.y+40, 1.0f, D3DCOLOR_ARGB(255, 83, 39, 20));
	m_Static[4].Create(m_WindowPosition.x+210, m_WindowPosition.y+27, m_WindowPosition.x+224, m_WindowPosition.y+40, 1.0f, D3DCOLOR_ARGB(255, 83, 39, 20));
	m_Static[5].Create(m_WindowPosition.x+228, m_WindowPosition.y+27, m_WindowPosition.x+302, m_WindowPosition.y+40, 1.0f, D3DCOLOR_ARGB(255, 83, 39, 20));
	m_Static[6].Create(m_WindowPosition.x+306, m_WindowPosition.y+27, m_WindowPosition.x+343, m_WindowPosition.y+40, 1.0f, D3DCOLOR_ARGB(255, 83, 39, 20));

	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {50, 237}, {108, 23}, _XDEF_PVPBROKER_REQUESTPVP,
			resourceindex,
			resourceindex,
			resourceindex,
			&g_MainInterfaceTextureArchive
	};

	btnstruct.position.x = 12;

	_XButton* pReqMatchButton = new _XButton;
	pReqMatchButton->Create(btnstruct);
	pReqMatchButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 176, 110, 199);
	pReqMatchButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 152, 110, 175);
	pReqMatchButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 201, 110, 224);
	pReqMatchButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	pReqMatchButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2556);
	InsertChildObject(pReqMatchButton);
	
	int optionresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_system.tga");

	btnstruct.position.x = 137;
	btnstruct.position.y = 237;
	btnstruct.windowsize.cx = 60;
	btnstruct.windowsize.cy = 24;
	btnstruct.commandid = _XDEF_MATCH_REQUESTLIST;
	btnstruct.imageindex_click = optionresourceindex;
	btnstruct.imageindex_normal = optionresourceindex;
	btnstruct.imageindex_ready = optionresourceindex;

	_XButton* pReqListButton = new _XButton;
	pReqListButton->Create(btnstruct);
	pReqListButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 187, 127, 247, 151);
	pReqListButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 187, 152, 247, 176);
	pReqListButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 187, 177, 247, 201);
	pReqListButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	pReqListButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2557);
	InsertChildObject(pReqListButton);

	btnstruct.position.x = 215;
	btnstruct.position.y = 237;
	btnstruct.commandid = _XDEF_MATCH_REGISTRATION;
	
	_XButton* pReqRegistButton = new _XButton;
	pReqRegistButton->Create(btnstruct);
	pReqRegistButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 187, 127, 247, 151);
	pReqRegistButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 187, 152, 247, 176);
	pReqRegistButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 187, 177, 247, 201);
	pReqRegistButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	pReqRegistButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2558);
	InsertChildObject(pReqRegistButton);

	btnstruct.position.x = 293;
	btnstruct.position.y = 237;
	btnstruct.commandid = _XDEF_MATCH_REGISTCANCEL;

	_XButton* pReqCancelButton = new _XButton;
	pReqCancelButton->Create(btnstruct);
	pReqCancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 187, 127, 247, 151);
	pReqCancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 187, 152, 247, 176);
	pReqCancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 187, 177, 247, 201);
	pReqCancelButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	pReqCancelButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2559);
	InsertChildObject(pReqCancelButton);

	int scrollbarresourceindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		// window property
		TRUE,										// 윈도우 활성화
		{6, 48},									// 윈도우 좌표 
		{353, 177},									// 윈도우 사이즈
		_XDEF_PVPBROKER_LISTBOX,					// 윈도우 아이디
		-1,											// 윈도우 볼더 이미지 인덱스
		&g_MainInterfaceTextureArchive,				// 텍스쳐리스트
		_T(""),										// 윈도우 타이틀 
		18,										// 리스트 아이템 라인 피치
		10,											// 리스트 박스에 보이는 최대 라인수 
		_XLSTYLE_RIGHTSCROLLBAR,					// 스크롤바의 위치 		
		
		// Scroll bar property
		{12, 177},									// 스크롤바 사이즈
		12,											// 스크롤바 상하버튼 사이즈
		22,											// 스크롤바 트랙바 사이즈
		150,										// 전체 리스트 갯수 <설정 필요없슴>			
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex		
	};

	m_MatchTargetListBox = new _XListBox;
	m_MatchTargetListBox->Create(listboxstructure);

	_XVScrollBar* pScrollBar = m_MatchTargetListBox->GetScrollBarObject();
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

	/*POINT columnoffset1 = { 0, 0  };
	POINT columnoffset2 = { 17, 2 };
	POINT columnoffset3 = { 95, 2 };
	POINT columnoffset4 = { 137, 2};
	POINT columnoffset5 = { 203, 2};
	POINT columnoffset6 = { 221, 2};
	POINT columnoffset7 = { 299, 2};*/

	POINT columnoffset1 = { 7, 0  };
	POINT columnoffset2 = { 17+38, 2 };
	POINT columnoffset3 = { 95+20, 2 };
	POINT columnoffset4 = { 137+32, 2};
	POINT columnoffset5 = { 203+8, 2};
	POINT columnoffset6 = { 221+38, 2};
	POINT columnoffset7 = { 299+19, 2};

	m_MatchTargetListBox->SetColumnOffset(0, columnoffset1);
	m_MatchTargetListBox->SetColumnOffset(1, columnoffset2);
	m_MatchTargetListBox->SetColumnOffset(2, columnoffset3);
	m_MatchTargetListBox->SetColumnOffset(3, columnoffset4);
	m_MatchTargetListBox->SetColumnOffset(4, columnoffset5);
	m_MatchTargetListBox->SetColumnOffset(5, columnoffset6);
	m_MatchTargetListBox->SetColumnOffset(6, columnoffset7);

	m_MatchTargetListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);
	m_MatchTargetListBox->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_MatchTargetListBox->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	m_MatchTargetListBox->SetColumnStyle(3, _XLCOLUMNSTYLE_STRING);
	m_MatchTargetListBox->SetColumnStyle(4, _XLCOLUMNSTYLE_STRING);
	m_MatchTargetListBox->SetColumnStyle(5, _XLCOLUMNSTYLE_STRING);
	m_MatchTargetListBox->SetColumnStyle(6, _XLCOLUMNSTYLE_STRING);

	/*
	m_MatchTargetListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_MatchTargetListBox->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_MatchTargetListBox->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);
	m_MatchTargetListBox->SetAlignType(3, _XFONT_ALIGNTYPE_LEFT);
	m_MatchTargetListBox->SetAlignType(4, _XFONT_ALIGNTYPE_LEFT);
	m_MatchTargetListBox->SetAlignType(5, _XFONT_ALIGNTYPE_LEFT);
	m_MatchTargetListBox->SetAlignType(6, _XFONT_ALIGNTYPE_LEFT);
	*/
	m_MatchTargetListBox->SetAlignType(0, _XFONT_ALIGNTYPE_CENTER);
	m_MatchTargetListBox->SetAlignType(1, _XFONT_ALIGNTYPE_CENTER);
	m_MatchTargetListBox->SetAlignType(2, _XFONT_ALIGNTYPE_CENTER);
	m_MatchTargetListBox->SetAlignType(3, _XFONT_ALIGNTYPE_CENTER);
	m_MatchTargetListBox->SetAlignType(4, _XFONT_ALIGNTYPE_CENTER);
	m_MatchTargetListBox->SetAlignType(5, _XFONT_ALIGNTYPE_CENTER);
	m_MatchTargetListBox->SetAlignType(6, _XFONT_ALIGNTYPE_CENTER);

	m_MatchTargetListBox->SetColumnWidth(0, 15);
	m_MatchTargetListBox->SetColumnWidth(1, 76);
	m_MatchTargetListBox->SetColumnWidth(2, 40);
	m_MatchTargetListBox->SetColumnWidth(3, 64);
	m_MatchTargetListBox->SetColumnWidth(4, 16);
	m_MatchTargetListBox->SetColumnWidth(5, 76);
	m_MatchTargetListBox->SetColumnWidth(6, 39);

	m_MatchTargetListBox->SetTrackWidth(338);
	m_MatchTargetListBox->SetTrackHeight(18);
	InsertChildObject(m_MatchTargetListBox);

	SetWindowMoveMode(_XWMOVE_FIXED);

	return TRUE;
}

void _XWindow_PVPBroker::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_PVPBroker::Draw(_XGUIObject*& pfocusobject)
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

	// TODO
	for(int i = 0 ; i < 7 ; i++)
		m_Static[i].RenderSolid();

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_XBaseFont->Puts(m_WindowPosition.x+8, m_WindowPosition.y+28, _XGETINTERFACETEXT(ID_STRING_NEW_2664), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+38, m_WindowPosition.y+28, _XGETINTERFACETEXT(ID_STRING_MATCH_2560), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+104, m_WindowPosition.y+28, _XGETINTERFACETEXT(ID_STRING_MATCH_2561), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+152, m_WindowPosition.y+28, _XGETINTERFACETEXT(ID_STRING_MATCH_2562), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+212, m_WindowPosition.y+28, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+233, m_WindowPosition.y+28, _XGETINTERFACETEXT(ID_STRING_MATCH_2563), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+310, m_WindowPosition.y+28, _XGETINTERFACETEXT(ID_STRING_MATCH_2564), 1.0f);
	g_XBaseFont->Flush();
}

BOOL _XWindow_PVPBroker::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	// TODO

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_PVPBroker::ShowWindow(BOOL show)
{
	if(show)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3842), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE); //근접한 성취단계의 사용자가 목록에 나타납니다.

		MoveWindow( (gnWidth>>1)+101, (gnHeight>>1)-154 );
		g_NetworkKernel.SendPacket(MSG_NO_MATCH_LIST, en_match_list_mode_req_list);
		g_pLocalUser->m_MatchTargetID = 0;
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_PVPBroker::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);

	m_Static[0].MoveWindow(m_WindowPosition.x+7, m_WindowPosition.y+27); 
	m_Static[1].MoveWindow(m_WindowPosition.x+24, m_WindowPosition.y+27);
	m_Static[2].MoveWindow(m_WindowPosition.x+102, m_WindowPosition.y+27);
	m_Static[3].MoveWindow(m_WindowPosition.x+144, m_WindowPosition.y+27);
	m_Static[4].MoveWindow(m_WindowPosition.x+210, m_WindowPosition.y+27);
	m_Static[5].MoveWindow(m_WindowPosition.x+228, m_WindowPosition.y+27);
	m_Static[6].MoveWindow(m_WindowPosition.x+306, m_WindowPosition.y+27);

	m_BorderTitle.MoveWindow(m_WindowPosition.x+1, m_WindowPosition.y+18);

}

void _XWindow_PVPBroker::SetListItem(__fighterInfo* fightinfo, int count, BOOL bContinue)
{
	if(!bContinue)
	{
		m_MatchTargetListBox->DeleteAllItem();
		m_MatchTargetListBox->SetScrollPos(0);
		m_ListBoxCount = 0;
	}

	for(int i = 0 ; i < count ; i++)
	{
		m_MatchTargetListBox->InsertListItem(_T(""), 0, fightinfo[i].uid);

		// 계급
		int resourceindex = -1;

		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			resourceindex = g_MainInterfaceTextureArchive.FindResource("vn_mi_pvp_02.tga");
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
		{
			resourceindex = g_MainInterfaceTextureArchive.FindResource("us_mi_pvp_02.tga");
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
		{
			resourceindex = g_MainInterfaceTextureArchive.FindResource("tw_mi_pvp_02.tga");
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			resourceindex = g_MainInterfaceTextureArchive.FindResource("rs_mi_pvp_02.tga");
		}
		else
		{
			resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_pvp_02.tga");
		}

		RECT rect;

		if(fightinfo->ucMatchGrade == 0)
		{
			rect.left = 1;
			rect.top = 65;
			rect.right = rect.left + 15;
			rect.bottom = rect.top + 15;
		}
		else
		{
			rect.left = 137 - (fightinfo->ucMatchGrade-1)*17;
			rect.top = 65;
			rect.right = rect.left + 15;
			rect.bottom = rect.top + 15;
		}

		m_MatchTargetListBox->SetIconArchive(i+m_ListBoxCount, &g_MainInterfaceTextureArchive);
		m_MatchTargetListBox->SetItemAttrib(i+m_ListBoxCount, 0, resourceindex, rect);

		// 이름
		m_MatchTargetListBox->SetItemText(i+m_ListBoxCount, 1, fightinfo[i].cCharacName);
		m_MatchTargetListBox->SetItemAttrib(i+m_ListBoxCount, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_MatchTargetListBox->SetItemAttribHighlight(i+m_ListBoxCount, 1, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 문파
		m_MatchTargetListBox->SetItemText(i+m_ListBoxCount, 2, g_JoinGroupName[fightinfo[i].cMoonpa]);
		m_MatchTargetListBox->SetItemAttrib(i+m_ListBoxCount, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_MatchTargetListBox->SetItemAttribHighlight(i+m_ListBoxCount, 2, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 직책
		m_MatchTargetListBox->SetItemText(i+m_ListBoxCount, 3, g_CharacterRollNameTable[fightinfo[i].cMoonpa][fightinfo[i].cClass][1]);
		m_MatchTargetListBox->SetItemAttrib(i+m_ListBoxCount, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_MatchTargetListBox->SetItemAttribHighlight(i+m_ListBoxCount, 3, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 성별
		if(fightinfo[i].ucSex == _XGENDER_MAN)
			m_MatchTargetListBox->SetItemText(i+m_ListBoxCount, 4, _XGETINTERFACETEXT(ID_STRING_MATCH_2565));
		else
			m_MatchTargetListBox->SetItemText(i+m_ListBoxCount, 4, _XGETINTERFACETEXT(ID_STRING_MATCH_2566));
		m_MatchTargetListBox->SetItemAttrib(i+m_ListBoxCount, 4, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_MatchTargetListBox->SetItemAttribHighlight(i+m_ListBoxCount, 4, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 성취단계
		TCHAR messagestring[256];
		sprintf(messagestring, "%s %d%s", 
			g_LevelName[g_UserLevelTable[fightinfo[i].ucInnerLevel].level], g_UserLevelTable[fightinfo[i].ucInnerLevel].sublevel,
			_XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));
		m_MatchTargetListBox->SetItemText(i+m_ListBoxCount, 5, messagestring);
		m_MatchTargetListBox->SetItemAttrib(i+m_ListBoxCount, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_MatchTargetListBox->SetItemAttribHighlight(i+m_ListBoxCount, 5, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 승률
		sprintf(messagestring, "%.1f%%",fightinfo[i].usWinRate/10.f);
		m_MatchTargetListBox->SetItemText(i+m_ListBoxCount, 6, messagestring);
		m_MatchTargetListBox->SetItemAttrib(i+m_ListBoxCount, 6, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_MatchTargetListBox->SetItemAttribHighlight(i+m_ListBoxCount, 6, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	m_ListBoxCount = i;
}