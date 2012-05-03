// XWindow_ChallengerList.cpp: implementation of the _XWindow_ChallengerList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_CastleInfo.h"
#include "XWindow_ChallengerList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_ChallengerList::_XWindow_ChallengerList()
{
	m_pSubTitle		= NULL;
	m_pLeftEdge		= NULL;
	m_pRightEdge	= NULL;
	m_pTopBar		= NULL;
	m_pBottomBar	= NULL;
	m_pScheduleIcon	= NULL;
	m_btnReqList	= NULL;

	memset(m_strDate, 0, sizeof(m_strDate));

	m_pCurCastleInfo = NULL;
}

_XWindow_ChallengerList::~_XWindow_ChallengerList()
{

}

BOOL _XWindow_ChallengerList::Initialize()
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
	pTitleRightBar->Create(283, 0, 355, 18, &g_MainInterfaceTextureArchive, mainframeinterface);
	pTitleRightBar->SetClipRect(110, 125, 183, 143);
	InsertChildObject(pTitleRightBar);
	
	_XImageStatic* pLeftBorder_1 = new _XImageStatic;
	pLeftBorder_1->Create(0, 17, 2, 48, &g_MainInterfaceTextureArchive, mainframeinterface);
	pLeftBorder_1->SetClipRect(0, 195, 3, 227);
	pLeftBorder_1->SetScale(1.0f, 7.0f);
	InsertChildObject(pLeftBorder_1);
	
	_XImageStatic* pRightBorder_1 = new _XImageStatic;
	pRightBorder_1->Create(353, 17, 355, 48, &g_MainInterfaceTextureArchive, mainframeinterface);
	pRightBorder_1->SetClipRect(0, 195, 3, 227);
	pRightBorder_1->SetScale(1.0f, 7.0f);
	InsertChildObject(pRightBorder_1);
	
	_XImageStatic* pLeftCorner = new _XImageStatic;
	pLeftCorner->Create(0, 241, 2, 265, &g_MainInterfaceTextureArchive, mainframeinterface);
	pLeftCorner->SetClipRect(0, 205, 3, 230);
	InsertChildObject(pLeftCorner);
	
	_XImageStatic* pRightCorner = new _XImageStatic;
	pRightCorner->Create(353, 241, 355, 265, &g_MainInterfaceTextureArchive, mainframeinterface);
	pRightCorner->SetClipRect(180, 205, 183, 230);
	InsertChildObject(pRightCorner);
	
	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create(3, 263, 179, 265, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBottomBorder->SetClipRect(3, 227, 180, 230);
	pBottomBorder->SetScale(1.9774f, 1.0f);
	InsertChildObject(pBottomBorder);
	
	m_BorderTitle.Create(m_WindowPosition.x+3, m_WindowPosition.y+18, m_WindowPosition.x+353, m_WindowPosition.y+263);
	m_BorderTitle.SetTexture(&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_BorderTile.tga"));
	m_BorderTitle.SetFaceColor(D3DCOLOR_ARGB(128, 255, 255, 255));
	
	FLOAT ufactor = 350.0f / 128.0f;
	FLOAT vfactor = 245.0f / 128.0f;

	m_BorderTitle.ChangeUV(0.0f, ufactor, ufactor, 0.0f, 0.0f, 0.0f, vfactor, vfactor);

	int btnresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {124, 237}, {106, 22},
			_XDEF_CHALLENGERLIST_REQLIST,
			btnresourceindex,
			btnresourceindex,
			btnresourceindex,
			&g_MainInterfaceTextureArchive
	};

	m_btnReqList = new _XButton;
	m_btnReqList->Create(btnstruct);
	m_btnReqList->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 3, 177, 109, 199);
	m_btnReqList->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 3, 153, 109, 175);
	m_btnReqList->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 3, 202, 109, 224);

	m_btnReqList->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	m_btnReqList->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3208);
	InsertChildObject(m_btnReqList);

	// List box
	int scrollresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_chat_quick.tga");

	_XLISTBOX_STRUCTURE tchallengerlistbox = 
	{
		TRUE,
		{9, 79},
		{339, 156},
		_XDEF_CHALLENGERLIST_LISTBOX,
		-1,
		&g_MainInterfaceTextureArchive,
		_T(""),
		21,
		7,
		_XLSTYLE_RIGHTSCROLLBAR,

		// scroll bar property
		{ 12, 156 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		255,												// 전체 리스트 갯수
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
	};

	m_lstboxChallengers = new _XListBox;
	m_lstboxChallengers->Create(tchallengerlistbox);

	_XVScrollBar* pScrollBar = m_lstboxChallengers->GetScrollBarObject();
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
	m_lstboxChallengers->SetAlwaysShowScrollBarMode(FALSE);

	POINT columnoffset1 = {10, 6};
	POINT columnoffset2 = {34, 8};
	m_lstboxChallengers->SetColumnOffset(0, columnoffset1);
	m_lstboxChallengers->SetColumnOffset(1, columnoffset2);

	m_lstboxChallengers->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);
	m_lstboxChallengers->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);

	m_lstboxChallengers->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lstboxChallengers->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);

	m_lstboxChallengers->SetColumnWidth(0, 15);
	m_lstboxChallengers->SetColumnWidth(1, 296);

	m_lstboxChallengers->SetTrackWidth(320);
	m_lstboxChallengers->SetTrackHeight(21);
	m_lstboxChallengers->EnableSelectItem(FALSE);
	InsertChildObject(m_lstboxChallengers);

	m_pScheduleIcon = new _XImageStatic;
	m_pScheduleIcon->Create(0, 0, 18, 18, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_manor02.tga"));
	m_pScheduleIcon->SetClipRect(236, 116, 254, 134);

	int charresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_characterwin.tga");

	m_pSubTitle = new _XImageStatic;
	m_pSubTitle->Create(0, 0, 158, 18, &g_MainInterfaceTextureArchive, charresourceindex);
	m_pSubTitle->SetClipRect(0, 23, 158, 41);

	m_pLeftEdge = new _XImageStatic;
	m_pLeftEdge->Create(0, 0, 2, 22, &g_MainInterfaceTextureArchive, charresourceindex);
	m_pLeftEdge->SetClipRect(0, 0, 2, 22);

	m_pRightEdge = new _XImageStatic;
	m_pRightEdge->Create(0, 0, 2, 22, &g_MainInterfaceTextureArchive, charresourceindex);
	m_pRightEdge->SetClipRect(160, 0, 162, 22);

	m_pTopBar = new _XImageStatic;
	m_pTopBar->Create(0, 0, 158, 2, &g_MainInterfaceTextureArchive, charresourceindex);
	m_pTopBar->SetClipRect(2, 0, 160, 2);

	m_pBottomBar = new _XImageStatic;
	m_pBottomBar->Create(0, 0, 158, 2, &g_MainInterfaceTextureArchive, charresourceindex);
	m_pBottomBar->SetClipRect(2, 20, 160, 22);

	return TRUE;
}

void _XWindow_ChallengerList::DestroyWindow( void )
{
	SAFE_DELETE(m_pSubTitle);
	SAFE_DELETE(m_pLeftEdge);
	SAFE_DELETE(m_pRightEdge);
	SAFE_DELETE(m_pTopBar);
	SAFE_DELETE(m_pBottomBar);
	SAFE_DELETE(m_pScheduleIcon);

	_XWindow::DestroyWindow();
}

void _XWindow_ChallengerList::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		m_BorderTitle.RenderAlphaBlend(D3DCOLOR_ARGB(128, 255, 255, 255));
		
//		_XDrawSolidBar(m_WindowPosition.x+10, m_WindowPosition.y+74, m_WindowPosition.x+347, m_WindowPosition.y+228, 0x7f000000);
//		_XDrawRectAngle(m_WindowPosition.x+9, m_WindowPosition.y+73, m_WindowPosition.x+346, m_WindowPosition.y+227, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));

		// solid
		_XDrawSolidBar(m_WindowPosition.x+9, m_WindowPosition.y+48, m_WindowPosition.x+346, m_WindowPosition.y+74, 0x7f000000);
		_XDrawSolidBar(m_WindowPosition.x+9, m_WindowPosition.y+80, m_WindowPosition.x+336, m_WindowPosition.y+235, 0x7f000000);

		_XDrawRectAngle(m_WindowPosition.x+8, m_WindowPosition.y+47, m_WindowPosition.x+345, m_WindowPosition.y+73, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawRectAngle(m_WindowPosition.x+8, m_WindowPosition.y+79, m_WindowPosition.x+335, m_WindowPosition.y+234, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
	}

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO

	// titles
	m_pSubTitle->SetScale(334.f/158.f, 1.0f);
	m_pSubTitle->Draw(m_WindowPosition.x+10, m_WindowPosition.y+23);

	m_pLeftEdge->Draw(m_WindowPosition.x+8, m_WindowPosition.y+21);
	m_pRightEdge->Draw(m_WindowPosition.x+344, m_WindowPosition.y+21);

	m_pTopBar->SetScale(334.f/158.f, 1.0f);
	m_pTopBar->Draw(m_WindowPosition.x+10, m_WindowPosition.y+21);

	m_pBottomBar->SetScale(334.f/158.f, 1.0f);
	m_pBottomBar->Draw(m_WindowPosition.x+10, m_WindowPosition.y+41);

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
	g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+27, _XGETINTERFACETEXT(ID_STRING_NEW_3207));					// 도전자 목록
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
	
	if(m_pCurCastleInfo)
	{
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 200, 198, 45));

		TCHAR tempString[512];
		memset( tempString, 0, sizeof(tempString));
		sprintf(tempString, _T("%s  :  %s"), _XGETINTERFACETEXT(ID_STRING_NEW_3219), m_strDate);
		g_XBaseFont->Print(m_WindowPosition.x+41, m_WindowPosition.y+57, 1.0f, tempString, m_pCurCastleInfo->m_nCurMatchCount+1);
		//g_XBaseFont->Puts(m_WindowPosition.x+41+63, m_WindowPosition.y+57, _T(":"));
		g_XBaseFont->Flush();

		m_pScheduleIcon->Draw(m_WindowPosition.x+18, m_WindowPosition.y+54);
	}
	
}

BOOL _XWindow_ChallengerList::Process( _XGUIObject*& pfocusobject )
{	
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	// TODO

	if( !_XWindow::Process(pfocusobject) ) 
		return FALSE;

	return TRUE;
}

void _XWindow_ChallengerList::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	m_BorderTitle.MoveWindow(X+3, Y+18);
}

void _XWindow_ChallengerList::ShowWindow(BOOL show)
{
	if(show)
	{
		EnableButton(TRUE);

		m_pCurCastleInfo = g_CBManager.GetCastleInfo(g_CurrentZoneInfoIndex);
		if(m_pCurCastleInfo)
		{
			int month = m_pCurCastleInfo->m_tWarSchedule[m_pCurCastleInfo->m_nCurMatchCount].tDate.dwMonth;
			int day =  m_pCurCastleInfo->m_tWarSchedule[m_pCurCastleInfo->m_nCurMatchCount].tDate.dwDay;
			int hour =  m_pCurCastleInfo->m_tWarSchedule[m_pCurCastleInfo->m_nCurMatchCount].tDate.dwHour;

			sprintf(m_strDate, g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_3220, 
				&month, &day, &hour));

//#ifdef _XDEF_CASTLEBATTLE_SENDPACKET
			g_NetworkKernel.SendPacket(MSG_NO_CB_SCHEDULE, en_cb_req_challenge_list);
//#endif
		}
		else
		{
//#ifdef _XDEF_CASTLEBATTLE_SENDPACKET
			g_NetworkKernel.SendPacket(MSG_NO_CB_SCHEDULE, en_cb_req_schedule_list);
			g_NetworkKernel.SendPacket(MSG_NO_CB_SCHEDULE, en_cb_req_challenge_list);
//#endif
		}
	}
	else
	{
		memset(m_strDate, 0, sizeof(m_strDate));
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_ChallengerList::SetListItem(void)
{
	if(!m_pCurCastleInfo)
		return;

	_XWindow_CastleInfo* pCastleInfo_Window = (_XWindow_CastleInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEINFO);

	m_lstboxChallengers->DeleteAllItem();
	m_lstboxChallengers->SetScrollPos(0);

	int iconresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_party.tga");

	int count = 0;
	TCHAR messagestr[256];
	TCHAR membercntstr[32];

	vector<_XCastle_OrgInfo*>::iterator iter_org;
	for(iter_org = m_pCurCastleInfo->m_vecChallengers.begin() ; iter_org != m_pCurCastleInfo->m_vecChallengers.end() ; ++iter_org)
	{
		_XCastle_OrgInfo* currentorg = *iter_org;
		if(currentorg)
		{
			RECT rect = {0, 0, 0, 0};
			if(pCastleInfo_Window)
			{
				pCastleInfo_Window->GetIconRect(currentorg->nGroupOfMaster, rect);
			}

			m_lstboxChallengers->InsertListItem(_T(""), 0, count+1);

			// group icon
			m_lstboxChallengers->SetIconArchive(count, &g_MainInterfaceTextureArchive);
			m_lstboxChallengers->SetItemAttrib(count, 0, iconresourceindex, rect);

			// info
			memset(messagestr, 0, sizeof(messagestr));
			memset(membercntstr, 0, sizeof(membercntstr));

//			strncpy(messagestr, currentorg->szOrgName, strlen(currentorg->szOrgName));
//			strcat(messagestr, _T(" ("));

			if(currentorg->nType == 0)
			{
				// 단주
				sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO2), currentorg->szOrgName);	//%s단
				strcat(messagestr, _T("("));
				strcat(messagestr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_CAPTAINUNIT));
			}
			else
			{
				// 회주
				sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3125), currentorg->szOrgName);	//%s회
				strcat(messagestr, _T("("));
				strcat(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3126));
			}

			strcat(messagestr, _T(":"));

			strcat(messagestr, currentorg->szMasterName);

//			strcat(messagestr, _T("/"));

//			sprintf(membercntstr, _XGETINTERFACETEXT(ID_STRING_NEW_2889), currentorg->nMemberCnt);	//%d명
			sprintf(membercntstr, ")");

			strcat(messagestr, membercntstr);

			m_lstboxChallengers->SetItemText(count, 1, messagestr);

			if(currentorg->nType == 0)
			{
				// 단주
				m_lstboxChallengers->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
				m_lstboxChallengers->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			else
			{
				// 회주
				m_lstboxChallengers->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 117, 199, 98));
				m_lstboxChallengers->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 117, 199, 98));
			}
			count++;
		}
	}
}

void _XWindow_ChallengerList::SetData()
{
	m_pCurCastleInfo = g_CBManager.GetCastleInfo(g_CurrentZoneInfoIndex);
	if(m_pCurCastleInfo)
	{
		int month = m_pCurCastleInfo->m_tWarSchedule[m_pCurCastleInfo->m_nCurMatchCount].tDate.dwMonth;
		int day =  m_pCurCastleInfo->m_tWarSchedule[m_pCurCastleInfo->m_nCurMatchCount].tDate.dwDay;
		int hour =  m_pCurCastleInfo->m_tWarSchedule[m_pCurCastleInfo->m_nCurMatchCount].tDate.dwHour;

		sprintf(m_strDate, g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_3220, 
			&month, &day, &hour));
	}
}

void _XWindow_ChallengerList::EnableButton(BOOL enable)
{
	m_btnReqList->EnableWindow(enable);
}