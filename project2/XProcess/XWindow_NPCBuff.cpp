// XWindow_NPCBuff.cpp: implementation of the _XWindow_NPCBuff class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Xwindow_State.h"
#include "XWindow_Escape.h"
#include "XWindow_MatchPR.h"
#include "XWindow_NPCBuff.h"
#include "XSR_STRINGHEADER.H"

//Author : 양희왕
#include "XWindowObjectDefine.h"

extern unsigned long		g_MoneyColor[6];

//typedef map<int, _XNPCBUFFSTR*> _map_NPCBuffStr;
_map_NPCBuffStr				g_NPCBuffStr;


void __stdcall NPCBuffListItemSelectDrawCallback( int XPos, int YPos, int Width, int height )
{
}

void __stdcall NPCBuffListItemClickedDrawCallback( int XPos, int YPos, int Width, int height )
{
	_XWindow_NPCBuff* pNPCBuff_Window = (_XWindow_NPCBuff*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCBUFF);
	if(pNPCBuff_Window)
	{
		pNPCBuff_Window->m_LeftSelectedImage.Draw(XPos-8, YPos-8);
		pNPCBuff_Window->m_RightSelectedImage.Draw(XPos+301, YPos-8);
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_NPCBuff::_XWindow_NPCBuff()
{
	m_lstboxBuff = NULL;
	m_SelectedItem = 0;
	m_nViewMode = EN_MODE_NPC_VIEW;
}

_XWindow_NPCBuff::~_XWindow_NPCBuff()
{

}

BOOL _XWindow_NPCBuff::Initialize()
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
	
	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 349, 2 }, { 12, 12 }, _XDEF_NPCBUFF_CLOSE,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );

	int skillresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {241, 235}, {108, 23}, _XDEF_NPCBUFF_APPLY,
		skillresourceindex,
		skillresourceindex,
		skillresourceindex,
		&g_MainInterfaceTextureArchive
	};
	_XButton* pApplyBtn = new _XButton;
	pApplyBtn->Create(btnstruct);
	pApplyBtn->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 176, 110, 199);
	pApplyBtn->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 152, 110, 175);
	pApplyBtn->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 201, 110, 224);
	pApplyBtn->SetGrowTextMode(TRUE);
	pApplyBtn->SetButtonTextColor(0xFFCBCBCB);
	pApplyBtn->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCBUFF_2533);
	pApplyBtn->ShowWindow(TRUE);
	InsertChildObject(pApplyBtn);

	_XImageStatic* pMoneyIcon = new _XImageStatic;
	pMoneyIcon->Create(111, 240, 126, 252, &g_MainInterfaceTextureArchive, mainframeinterface);
	pMoneyIcon->SetClipRect(185, 6, 200, 18);
	InsertChildObject(pMoneyIcon);

	int borderresourceindex = g_MainInterfaceTextureArchive.FindResource("MI_Premium.tga");

	_XImageStatic* pBorder_1 = new _XImageStatic;
	pBorder_1->Create(15, 24, 269, 70, &g_MainInterfaceTextureArchive, borderresourceindex);
	pBorder_1->SetClipRect(2, 42, 256, 88);
	InsertChildObject(pBorder_1);

	_XImageStatic* pBorder_2 = new _XImageStatic;
	pBorder_2->Create(269, 24, 325, 70, &g_MainInterfaceTextureArchive, borderresourceindex);
	pBorder_2->SetClipRect(0, 92, 56, 138);
	InsertChildObject(pBorder_2);

	_XImageStatic* pBorder_3 = new _XImageStatic;
	pBorder_3->Create(15, 77, 269, 123, &g_MainInterfaceTextureArchive, borderresourceindex);
	pBorder_3->SetClipRect(2, 42, 256, 88);
	InsertChildObject(pBorder_3);
	
	_XImageStatic* pBorder_4 = new _XImageStatic;
	pBorder_4->Create(269, 77, 325, 123, &g_MainInterfaceTextureArchive, borderresourceindex);
	pBorder_4->SetClipRect(0, 92, 56, 138);
	InsertChildObject(pBorder_4);

	_XImageStatic* pBorder_5 = new _XImageStatic;
	pBorder_5->Create(15, 130, 269, 176, &g_MainInterfaceTextureArchive, borderresourceindex);
	pBorder_5->SetClipRect(2, 42, 256, 88);
	InsertChildObject(pBorder_5);
	
	_XImageStatic* pBorder_6 = new _XImageStatic;
	pBorder_6->Create(269, 130, 325, 176, &g_MainInterfaceTextureArchive, borderresourceindex);
	pBorder_6->SetClipRect(0, 92, 56, 138);
	InsertChildObject(pBorder_6);

	_XImageStatic* pBorder_7 = new _XImageStatic;
	pBorder_7->Create(15, 183, 269, 229, &g_MainInterfaceTextureArchive, borderresourceindex);
	pBorder_7->SetClipRect(2, 42, 256, 88);
	InsertChildObject(pBorder_7);
	
	_XImageStatic* pBorder_8 = new _XImageStatic;
	pBorder_8->Create(269, 183, 325, 229, &g_MainInterfaceTextureArchive, borderresourceindex);
	pBorder_8->SetClipRect(0, 92, 56, 138);
	InsertChildObject(pBorder_8);

	_XImageStatic* pIconBorder_1 = new _XImageStatic;
	pIconBorder_1->Create(21, 30, 55, 64, &g_MainInterfaceTextureArchive, borderresourceindex);

	_XImageStatic* pIconBorder_2 = new _XImageStatic;
	pIconBorder_2->Create(21, 83, 55, 117, &g_MainInterfaceTextureArchive, borderresourceindex);

	_XImageStatic* pIconBorder_3 = new _XImageStatic;
	pIconBorder_3->Create(21, 136, 55, 170, &g_MainInterfaceTextureArchive, borderresourceindex);

	_XImageStatic* pIconBorder_4 = new _XImageStatic;
	pIconBorder_4->Create(21, 189, 55, 223, &g_MainInterfaceTextureArchive, borderresourceindex);

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI //Author :양희왕// breif : 구룡창 사이즈 변경 되면서 이미지상 아이콘 위치도 변경
	pIconBorder_1->SetClipRect(172, 0, 206, 34);
	pIconBorder_2->SetClipRect(172, 0, 206, 34);
	pIconBorder_3->SetClipRect(172, 0, 206, 34);
	pIconBorder_4->SetClipRect(172, 0, 206, 34);
#else
	pIconBorder_1->SetClipRect(146, 0, 180, 34);
	pIconBorder_2->SetClipRect(146, 0, 180, 34);
	pIconBorder_3->SetClipRect(146, 0, 180, 34);
	pIconBorder_4->SetClipRect(146, 0, 180, 34);
#endif
	InsertChildObject(pIconBorder_1);
	InsertChildObject(pIconBorder_2);
	InsertChildObject(pIconBorder_3);
	InsertChildObject(pIconBorder_4);

	int scrollresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_chat_quick.tga");
	_XLISTBOX_STRUCTURE lboxstruct = 
	{
		TRUE,
		{15, 22},
		{333, 207},
		_XDEF_NPCBUFF_LISTBOX,
		-1,
		&g_MainInterfaceTextureArchive,
		_T(""),
		46,
		4,
		_XLSTYLE_RIGHTSCROLLBAR,

		{12, 207},
		12,
		22,
		150,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex
	};
	m_lstboxBuff = new _XListBox;
	m_lstboxBuff->Create(lboxstruct);

	_XVScrollBar* pScrollBar = m_lstboxBuff->GetScrollBarObject();
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

	m_lstboxBuff->SetItemSelectDrawCallBack(NPCBuffListItemSelectDrawCallback);
	m_lstboxBuff->SetItemClickedDrawCallBack(NPCBuffListItemClickedDrawCallback);

	m_lstboxBuff->SetIconReportMode(TRUE);
	m_lstboxBuff->SetIconReportModeInfo(4, 1, 53, 310);
	m_lstboxBuff->SetTrackWidth(310);
	m_lstboxBuff->SetTrackHeight(53);

	m_lstboxBuff->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);
	m_lstboxBuff->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_lstboxBuff->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	m_lstboxBuff->SetColumnStyle(3, _XLCOLUMNSTYLE_STRING);
	m_lstboxBuff->SetColumnStyle(4, _XLCOLUMNSTYLE_ICONRECT);
	m_lstboxBuff->SetColumnStyle(5, _XLCOLUMNSTYLE_STRING);

	POINT columnoffset0 = { 8, 10  };
	POINT columnoffset1 = { 46, 10 };
	POINT columnoffset2 = { 303, 10};
	POINT columnoffset3 = { 46, 29 };
	POINT columnoffset4 = { 221, 29};
	POINT columnoffset5 = { 303, 29};

	m_lstboxBuff->SetColumnOffset(0, columnoffset0);
	m_lstboxBuff->SetColumnOffset(1, columnoffset1);
	m_lstboxBuff->SetColumnOffset(2, columnoffset2);
	m_lstboxBuff->SetColumnOffset(3, columnoffset3);
	m_lstboxBuff->SetColumnOffset(4, columnoffset4);
	m_lstboxBuff->SetColumnOffset(5, columnoffset5);

	m_lstboxBuff->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lstboxBuff->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_lstboxBuff->SetAlignType(2, _XFONT_ALIGNTYPE_RIGHT);
	m_lstboxBuff->SetAlignType(3, _XFONT_ALIGNTYPE_LEFT);
	m_lstboxBuff->SetAlignType(4, _XFONT_ALIGNTYPE_LEFT);
	m_lstboxBuff->SetAlignType(5, _XFONT_ALIGNTYPE_RIGHT);

	m_lstboxBuff->SetColumnWidth(0, 30);
	m_lstboxBuff->SetColumnWidth(1, 166);
	m_lstboxBuff->SetColumnWidth(2, 120);
	m_lstboxBuff->SetColumnWidth(3, 170);
	m_lstboxBuff->SetColumnWidth(4, 15);
	m_lstboxBuff->SetColumnWidth(5, 50);

	InsertChildObject(m_lstboxBuff);

	m_LeftSelectedImage.Create(0, 0, 156, 47, &g_MainInterfaceTextureArchive, borderresourceindex);
	m_LeftSelectedImage.SetClipRect(61, 92, 216, 138);

	m_RightSelectedImage.Create(0, 0, 156, 47, &g_MainInterfaceTextureArchive, borderresourceindex);
	m_RightSelectedImage.SetClipRect(61, 92, 216, 138);
	m_RightSelectedImage.SetScale(-1, 1);

	SetWindowMoveMode(_XWMOVE_FIXED);
	
	return TRUE;
}

void _XWindow_NPCBuff::DestroyWindow()
{
//	SAFE_DELETE_ARRAY(g_NPCBuffStr);

	map<int, _XNPCBUFFSTR*>::iterator iter_buff;
	for(iter_buff = g_NPCBuffStr.begin() ; iter_buff != g_NPCBuffStr.end() ; ++iter_buff)
	{
		_XNPCBUFFSTR* pNPCBuff = iter_buff->second;
		SAFE_DELETE(pNPCBuff);
	}
	g_NPCBuffStr.clear();

	_XWindow::DestroyWindow();
}

void _XWindow_NPCBuff::Draw(_XGUIObject*& pfocusobject)
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
	_XDrawRectAngle(m_WindowPosition.x+106, m_WindowPosition.y+235, m_WindowPosition.x+193, m_WindowPosition.y+256, 0.0f, 0xFFDFDAD6);

	g_XBaseFont->SetColor(0xFFCBCBCB);
	g_XBaseFont->Puts(m_WindowPosition.x+18, m_WindowPosition.y+240, _XGETINTERFACETEXT(ID_STRING_NPCBUFF_2534));
	g_XBaseFont->Flush();

	TCHAR moneyStr[128];
	memset( moneyStr, 0, sizeof(moneyStr));
//	sprintf(moneyStr, "%u", g_pLocalUser->m_Money);
	_snprintf(moneyStr, sizeof(moneyStr), "%u", g_pLocalUser->m_Money);

	int colorindex = 0;

	if( _XGameItem::GetMoneyColor(moneyStr, colorindex) )
	{
		g_XBaseFont->SetColor( g_MoneyColor[colorindex] );
		g_XBaseFont->PrintAlign(m_WindowPosition.x+188, m_WindowPosition.y+240, 1.0f, _XFONT_ALIGNTYPE_RIGHT, moneyStr );
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFEFFFFF );
		g_XBaseFont->PrintAlign(m_WindowPosition.x+188, m_WindowPosition.y+240, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%u", g_pLocalUser->m_Money);
	}

	g_XBaseFont->Flush();

	//Author : 양희왕 //breif : 미국 설명이 길어서 툴팁으로 표시
	//breif : 베트남도 추가 Date : 08/03/11

	#if defined(_XVIETNAMESE) || defined(_XENGLISH)	|| defined(_XRUSSIAN)
	if( m_lstboxBuff->GetMouseOverItem() > -1 )
	{
		POINT pos = m_lstboxBuff->GetColumnOffsetWindowPos( m_lstboxBuff->GetMouseOverItem(), 3 );
		g_InterfaceTooltip.SetToolTipString(pos.x, pos.y, m_lstboxBuff->GetItemText(m_lstboxBuff->GetMouseOverItem(), 3), _XSC_INFORMATION);
	}
	#endif
	
}

BOOL _XWindow_NPCBuff::Process(_XGUIObject*& pfocusobject)
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

void _XWindow_NPCBuff::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	
	m_BorderTitle.MoveWindow(m_WindowPosition.x+1, m_WindowPosition.y+18);
}

void _XWindow_NPCBuff::ShowWindow(BOOL show)
{
	if(show)
	{
		MoveWindow( (gnWidth>>1)+98, (gnHeight>>1)-164 );
		m_SelectedItem = 0;
	}

	_XWindow::ShowWindow(show);
}


void _XWindow_NPCBuff::SetListItem()
{
	_XWindow_State* pState_Window = (_XWindow_State*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_STATEWINDOW);

	m_lstboxBuff->DeleteAllItem();
	m_lstboxBuff->SetScrollPos(0);

	int iconresourceindex = -1;
	RECT iconrect;

	int moneyresourceindex = g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	RECT moneyiconrect = {185, 6, 200, 18};

	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(messagestring));

	BOOL bInsert = FALSE, bSkillInsert = TRUE;
	int count = 0;

	for(int i = 0 ; i < g_NPCBuffCount ; i++)
	{
		bInsert = FALSE;
		for(int jj = 0 ; jj < 15 ; ++jj)
		{
			if(g_NPCBuffList.nList[jj] == i+1)
			{
				bInsert = TRUE;
				break;
			}
		}

		if(bInsert)
		{
//			if(g_NPCBuffStr[i].bufindex <= 0)
//				continue;

			_XNPCBUFFSTR* pNPCBuff = NULL;
			_map_NPCBuffStr::iterator iter_buff = g_NPCBuffStr.find(i+1);
			if(iter_buff != g_NPCBuffStr.end())
			{
				pNPCBuff = iter_buff->second;
			}
			else
			{
				continue;
			}

			m_lstboxBuff->InsertListItem(_T(""), 0, i+1);

			// condition icon
			m_lstboxBuff->SetIconArchive(count, &g_MainInterfaceTextureArchive);

			if(!_XSkillItem::FindSkillProperty(pNPCBuff->nSkillindex))
			{
				bSkillInsert = FALSE;
			}

			if(bSkillInsert)
			{
				if(g_pQuickSlot_Window)
					g_pQuickSlot_Window->GetIconRect(pNPCBuff->nSkillindex, iconresourceindex, iconrect, true, true);
				m_lstboxBuff->SetItemAttrib(count, 0, iconresourceindex, iconrect);

				// 이름
				m_lstboxBuff->SetItemText(count, 1, g_SkillProperty[pNPCBuff->nSkillindex]->skillName);
				m_lstboxBuff->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 180, 180, 180));
				m_lstboxBuff->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			else
			{
				// 이름
				m_lstboxBuff->SetItemText(count, 1, "NONAME");	// 에러 체크 용
				m_lstboxBuff->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 180, 180, 180));
				m_lstboxBuff->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			// type
			memset(messagestring, 0, sizeof(messagestring));
			switch(pNPCBuff->type)
			{
			case 0 :
				{
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NPCBUFF_2535));
				}
				break;
			case 1 :
			case 2 :
			case 3 :
				{
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3350));
				}
				break;
			case 4 :
				{
				#if defined(_XVIETNAMESE)
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3414));
				#else
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NPCBUFF_2539));
				#endif
				}
				break;
			case 5 :
				{
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3414));
				}
				break;
			case 6 : //Author : 양희왕 //breif : 베트남 추가
				{
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NPCBUFF_2539));
				}
				break;
			}
			m_lstboxBuff->SetItemText(count, 2, messagestring);
			m_lstboxBuff->SetItemAttrib(count, 2, D3DCOLOR_ARGB(255, 180, 180, 180));
			m_lstboxBuff->SetItemAttribHighlight(count, 2, D3DCOLOR_ARGB(255, 80, 240, 42));

			// info
			memset(messagestring, 0, sizeof(messagestring));
			
			int		param1	= pNPCBuff->time;
			LPTSTR	param2	= pNPCBuff->info;
			
			strcpy(messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NPCBUFF_2540, &param1, param2 ) );

			m_lstboxBuff->SetItemText(count, 3, messagestring);
			m_lstboxBuff->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 180, 180, 180));
			m_lstboxBuff->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 180, 180, 180));

			// money icon
			if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				if(pNPCBuff->type == 4 || pNPCBuff->type == 5)
				{
					// money icon / free 표시하지 않음
				}
				else
				{
					m_lstboxBuff->SetItemAttrib(count, 4, moneyresourceindex, moneyiconrect);
			
					// 필요한 돈
					memset(messagestring, 0, sizeof(messagestring));
					if(pNPCBuff->price == 0)
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NPCBUFF_2541));
					else
						sprintf(messagestring, "%d", pNPCBuff->price);
					m_lstboxBuff->SetItemText(count, 5, messagestring);
					m_lstboxBuff->SetItemAttrib(count, 5, D3DCOLOR_ARGB(255, 180, 180, 180));
					m_lstboxBuff->SetItemAttribHighlight(count, 5, D3DCOLOR_ARGB(255, 180, 180, 180));
				}
			}
			else
			{
				m_lstboxBuff->SetItemAttrib(count, 4, moneyresourceindex, moneyiconrect);
				
				// 필요한 돈
				memset(messagestring, 0, sizeof(messagestring));
				if(pNPCBuff->price == 0)
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NPCBUFF_2541));
				else
					sprintf(messagestring, "%d", pNPCBuff->price);
				m_lstboxBuff->SetItemText(count, 5, messagestring);
				m_lstboxBuff->SetItemAttrib(count, 5, D3DCOLOR_ARGB(255, 180, 180, 180));
				m_lstboxBuff->SetItemAttribHighlight(count, 5, D3DCOLOR_ARGB(255, 180, 180, 180));
			}

			count++;
		}
	}
}



void _XWindow_NPCBuff::SetListFreeItem()
{
	_XWindow_State* pState_Window = (_XWindow_State*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_STATEWINDOW);

	m_lstboxBuff->DeleteAllItem();
	m_lstboxBuff->SetScrollPos(0);

	int iconresourceindex = -1;
	RECT iconrect;

	int moneyresourceindex = g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	RECT moneyiconrect = {185, 6, 200, 18};

	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(messagestring));

	BOOL bInsert = FALSE, bSkillInsert = TRUE;
	int count = 0;
	int addindex = 0;

	for(int i = 0 ; i < g_NPCBuffCount ; i++)
	{
		bInsert = FALSE;
		for(int jj = 0 ; jj < 15 ; ++jj)
		{
			if(g_NPCBuffList.nList[jj] == i+1)
			{
				bInsert = TRUE;
				addindex = i+100;
				break;
			}
		}

		if(bInsert)
		{
//			if(g_NPCBuffStr[i].bufindex <= 0)
//				continue;

			_XNPCBUFFSTR* pNPCBuff = NULL;
			_map_NPCBuffStr::iterator iter_buff = g_NPCBuffStr.find(addindex+1);
			if(iter_buff != g_NPCBuffStr.end())
			{
				pNPCBuff = iter_buff->second;
			}
			else
			{
				continue;
			}

			m_lstboxBuff->InsertListItem(_T(""), 0, addindex+1);

			// condition icon
			m_lstboxBuff->SetIconArchive(count, &g_MainInterfaceTextureArchive);

			if(!_XSkillItem::FindSkillProperty(pNPCBuff->nSkillindex))
			{
				bSkillInsert = FALSE;
			}

			if(bSkillInsert)
			{
				if(g_pQuickSlot_Window)
					g_pQuickSlot_Window->GetIconRect(pNPCBuff->nSkillindex, iconresourceindex, iconrect, true, true);
				m_lstboxBuff->SetItemAttrib(count, 0, iconresourceindex, iconrect);

				// 이름
				m_lstboxBuff->SetItemText(count, 1, g_SkillProperty[pNPCBuff->nSkillindex]->skillName);
				m_lstboxBuff->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 180, 180, 180));
				m_lstboxBuff->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			else
			{
				// 이름
				m_lstboxBuff->SetItemText(count, 1, "NONAME");	// 에러 체크 용
				m_lstboxBuff->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 180, 180, 180));
				m_lstboxBuff->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			// type
			memset(messagestring, 0, sizeof(messagestring));
			switch(pNPCBuff->type)
			{
			case 0 :
				{
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NPCBUFF_2535));
				}
				break;
			case 1 :
			case 2 :
			case 3 :
				{
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3350));
				}
				break;
			case 4 :
				{
				#if defined(_XVIETNAMESE)
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3414));
				#else
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NPCBUFF_2539));
				#endif
				}
				break;
			case 5 :
				{
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3414));
				}
				break;
			case 6 : //Author : 양희왕 //breif : 베트남 추가
				{
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NPCBUFF_2539));
				}
				break;
			}
			m_lstboxBuff->SetItemText(count, 2, messagestring);
			m_lstboxBuff->SetItemAttrib(count, 2, D3DCOLOR_ARGB(255, 180, 180, 180));
			m_lstboxBuff->SetItemAttribHighlight(count, 2, D3DCOLOR_ARGB(255, 80, 240, 42));

			// info
			memset(messagestring, 0, sizeof(messagestring));
			
			int		param1	= pNPCBuff->time;
			LPTSTR	param2	= pNPCBuff->info;
			
			strcpy(messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NPCBUFF_2540, &param1, param2 ) );

			m_lstboxBuff->SetItemText(count, 3, messagestring);
			m_lstboxBuff->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 180, 180, 180));
			m_lstboxBuff->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 180, 180, 180));

			// money icon
			if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				if(pNPCBuff->type == 4 || pNPCBuff->type == 5)
				{
					// money icon / free 표시하지 않음
				}
				else
				{
					m_lstboxBuff->SetItemAttrib(count, 4, moneyresourceindex, moneyiconrect);
			
					// 필요한 돈
					memset(messagestring, 0, sizeof(messagestring));
					if(pNPCBuff->price == 0)
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NPCBUFF_2541));
					else
						sprintf(messagestring, "%d", pNPCBuff->price);
					m_lstboxBuff->SetItemText(count, 5, messagestring);
					m_lstboxBuff->SetItemAttrib(count, 5, D3DCOLOR_ARGB(255, 180, 180, 180));
					m_lstboxBuff->SetItemAttribHighlight(count, 5, D3DCOLOR_ARGB(255, 180, 180, 180));
				}
			}
			else
			{
				m_lstboxBuff->SetItemAttrib(count, 4, moneyresourceindex, moneyiconrect);
				
				// 필요한 돈
				memset(messagestring, 0, sizeof(messagestring));
				if(pNPCBuff->price == 0)
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NPCBUFF_2541));
				else
					sprintf(messagestring, "%d", pNPCBuff->price);
				m_lstboxBuff->SetItemText(count, 5, messagestring);
				m_lstboxBuff->SetItemAttrib(count, 5, D3DCOLOR_ARGB(255, 180, 180, 180));
				m_lstboxBuff->SetItemAttribHighlight(count, 5, D3DCOLOR_ARGB(255, 180, 180, 180));
			}

			count++;
		}
	}
}

BOOL _XWindow_NPCBuff::CheckUse()
{	 
	BOOL bUse = TRUE;

	_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);	
	CEmperorOfDragonsApp* pApp = ((CEmperorOfDragonsApp*)gApp);

	if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3679), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//대전 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->m_bIsBossBattle )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3680), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//결 진행중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
		(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// 내공심법 시전중
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1580), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//운기요상 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}		
	else if( g_pLocalUser->m_bReadingBook )	// 비급 읽는 중이면 리턴
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1581), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//비급을 읽는 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->m_bSocketPlugIn )	// 소켓 장착 중이면 리턴
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1582), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//장식물을 다는 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->m_bGambleBoxOpen )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1583), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//패왕궤를 여는 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->m_bInternalUse )	// 영약 복용 중이면 리턴
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1584), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//영약복용 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//노점상태에서는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if(g_pLocalUser->GetMotionClass() == _XACTION_DIE || g_pLocalUser->GetMotionClass() == _XACTION_DYING)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3681), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//빈사나 혼수 상태 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->m_CharacterLog.isTrade )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3682), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//교환 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if(g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3683), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//수련 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( (g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT) )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3684), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//비무 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( pEscape_Window->GetShowStatus() )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3313), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//탈출 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( pApp->m_proc_maingame.m_bNPCDialogMode || pApp->m_proc_maingame.m_bNPCDialogWideView || pApp->m_proc_maingame.m_bFunctionalObjectWideView)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//지금은 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->GetEnterMatchEvent() ) //Author : 양희왕 //breif : 영웅대회 에서 막기..
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3739), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//지금은 사용할 수 없습니다.
		bUse = FALSE;
	}
	
	_XWindow_MatchPR* pMatchPR_Window = (_XWindow_MatchPR*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MATCHPR);
	if( pMatchPR_Window)
	{
		if( pMatchPR_Window->GetShowStatus() )
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3686), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//비무 홍보 중에는 사용할 수 없습니다.
			bUse = FALSE;
		}
	}

	return bUse;
}

void _XWindow_NPCBuff::SetViewMode(int mode)
{
	m_nViewMode = mode;

	switch(mode)
	{
	case EN_MODE_ITEM_VIEW :
		{
			SetWindowMoveMode(_XWMOVE_FREE);
		}
		break;
	default :
	case EN_MODE_NPC_VIEW :
		{
			SetWindowMoveMode(_XWMOVE_FIXED);
		}
		break;
	}
}