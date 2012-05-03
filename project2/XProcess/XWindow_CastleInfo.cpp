// XWindow_CastleInfo.cpp: implementation of the _XWindow_CastleInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_CastleInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_CastleInfo::_XWindow_CastleInfo()
{
	m_pSubTitle		= NULL;
	m_pLeftEdge		= NULL;
	m_pRightEdge	= NULL;
	m_pTopBar		= NULL;
	m_pBottomBar	= NULL;
	
	m_pGroupIcon	= NULL;
	m_btnReqWar		= NULL;

	for(int i = 0 ; i < 2 ; ++i)
		m_pScheduleIcon[i] = NULL;

	memset(m_strOrgName, 0, sizeof(m_strOrgName));
	memset(m_strDate, 0, sizeof(m_strDate));
	memset(m_strPropertyDate, 0, sizeof(m_strPropertyDate));
	
	m_pCurCastleInfo = NULL;
}

_XWindow_CastleInfo::~_XWindow_CastleInfo()
{

}

BOOL _XWindow_CastleInfo::Initialize()
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
			_XDEF_CASTLEINFO_REQWAR,
			btnresourceindex,
			btnresourceindex,
			btnresourceindex,
			&g_MainInterfaceTextureArchive
	};

	m_btnReqWar = new _XButton;
	m_btnReqWar->Create(btnstruct);
	m_btnReqWar->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 3, 177, 109, 199);
	m_btnReqWar->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 3, 153, 109, 175);
	m_btnReqWar->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 3, 202, 109, 224);

	m_btnReqWar->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	m_btnReqWar->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3204);
	InsertChildObject(m_btnReqWar);

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
	
	m_pGroupIcon = new _XImageStatic;
	m_pGroupIcon->Create(0, 0, 15, 15, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_party.tga"));
	
	for(int i = 0 ; i < 2 ; ++i)
	{
		m_pScheduleIcon[i] = new _XImageStatic;
		m_pScheduleIcon[i]->Create(0, 0, 18, 18, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_manor02.tga"));
		m_pScheduleIcon[i]->SetClipRect(236, 116+(i*18), 254, 134+(i*18));
	}

	return TRUE;
}

void _XWindow_CastleInfo::DestroyWindow( void )
{
	SAFE_DELETE(m_pSubTitle);
	SAFE_DELETE(m_pLeftEdge);
	SAFE_DELETE(m_pRightEdge);
	SAFE_DELETE(m_pTopBar);
	SAFE_DELETE(m_pBottomBar);

	SAFE_DELETE(m_pScheduleIcon[0]);
	SAFE_DELETE(m_pScheduleIcon[1]);
	
	SAFE_DELETE(m_pGroupIcon);

	_XWindow::DestroyWindow();
}

void _XWindow_CastleInfo::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		m_BorderTitle.RenderAlphaBlend(D3DCOLOR_ARGB(128, 255, 255, 255));
		
//		_XDrawSolidBar(m_WindowPosition.x+10, m_WindowPosition.y+74, m_WindowPosition.x+347, m_WindowPosition.y+228, 0x7f000000);
//		_XDrawRectAngle(m_WindowPosition.x+9, m_WindowPosition.y+73, m_WindowPosition.x+346, m_WindowPosition.y+227, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
	}

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO

	// titles
	m_pSubTitle->SetScale(334.f/158.f, 1.0f);
	m_pSubTitle->Draw(m_WindowPosition.x+10, m_WindowPosition.y+23);
	m_pSubTitle->Draw(m_WindowPosition.x+10, m_WindowPosition.y+122);

	m_pLeftEdge->Draw(m_WindowPosition.x+8, m_WindowPosition.y+21);
	m_pLeftEdge->Draw(m_WindowPosition.x+8, m_WindowPosition.y+120);

	m_pRightEdge->Draw(m_WindowPosition.x+344, m_WindowPosition.y+21);
	m_pRightEdge->Draw(m_WindowPosition.x+344, m_WindowPosition.y+120);

	m_pTopBar->SetScale(334.f/158.f, 1.0f);
	m_pTopBar->Draw(m_WindowPosition.x+10, m_WindowPosition.y+21);
	m_pTopBar->Draw(m_WindowPosition.x+10, m_WindowPosition.y+120);

	m_pBottomBar->SetScale(334.f/158.f, 1.0f);
	m_pBottomBar->Draw(m_WindowPosition.x+10, m_WindowPosition.y+41);
	m_pBottomBar->Draw(m_WindowPosition.x+10, m_WindowPosition.y+140);

	// solid
	_XDrawSolidBar(m_WindowPosition.x+9, m_WindowPosition.y+48, m_WindowPosition.x+346, m_WindowPosition.y+112, 0x7f000000);
	_XDrawSolidBar(m_WindowPosition.x+9, m_WindowPosition.y+147, m_WindowPosition.x+346, m_WindowPosition.y+234, 0x7f000000);

	_XDrawRectAngle(m_WindowPosition.x+8, m_WindowPosition.y+47, m_WindowPosition.x+345, m_WindowPosition.y+111, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
	_XDrawRectAngle(m_WindowPosition.x+8, m_WindowPosition.y+146, m_WindowPosition.x+345, m_WindowPosition.y+233, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
	g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+27, _XGETINTERFACETEXT(ID_STRING_NEW_3199));					// 장원 기본 정보
	g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+126, _XGETINTERFACETEXT(ID_STRING_NEW_3203));					// 장원전 도전 일정
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	g_XBaseFont->Print(m_WindowPosition.x+19, m_WindowPosition.y+57, 1.0f, "%s   :", _XGETINTERFACETEXT(ID_STRING_NEW_3200));	// 소유자
	g_XBaseFont->Print(m_WindowPosition.x+19, m_WindowPosition.y+75, 1.0f, "%s :", _XGETINTERFACETEXT(ID_STRING_NEW_3201));		// 소유일수
	g_XBaseFont->Print(m_WindowPosition.x+19, m_WindowPosition.y+93, 1.0f, "%s :", _XGETINTERFACETEXT(ID_STRING_NEW_3202));		// 장원단계
	g_XBaseFont->Flush();

	// 내용
	if(m_pCurCastleInfo)
	{
		RECT rect;
		GetIconRect(m_pCurCastleInfo->m_tOwner.nGroupOfMaster, rect);
		m_pGroupIcon->SetClipRect(rect);
		m_pGroupIcon->Draw(m_WindowPosition.x+91, m_WindowPosition.y+55);


		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 117, 199, 98));
		g_XBaseFont->Puts(m_WindowPosition.x+109, m_WindowPosition.y+57, m_strOrgName);
		g_XBaseFont->Flush();

		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
		
		if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont->Print(m_WindowPosition.x+140, m_WindowPosition.y+75, 1.0f, m_strPropertyDate);
			g_XBaseFont->Print(m_WindowPosition.x+114, m_WindowPosition.y+93, 1.0f, _XGETINTERFACETEXT(ID_STRING_NEW_3206), m_pCurCastleInfo->m_nStep+1);		// %d 단계
		}
		else
		{
			g_XBaseFont->Print(m_WindowPosition.x+86, m_WindowPosition.y+75, 1.0f, m_strPropertyDate);
			g_XBaseFont->Print(m_WindowPosition.x+86, m_WindowPosition.y+93, 1.0f, _XGETINTERFACETEXT(ID_STRING_NEW_3206), m_pCurCastleInfo->m_nStep+1);		// %d 단계
		}
		g_XBaseFont->Flush();
		
#if (defined(_XDEF_CASTLEBATTLE_SCHEDULE_RENEWAL_US_KUKURI) || defined (_XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI))

		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 200, 198, 45));
		m_pScheduleIcon[0]->Draw(m_WindowPosition.x+16, m_WindowPosition.y+157);
		TCHAR tempString[512];
		memset( tempString, 0, sizeof(tempString));
		sprintf(tempString, _T("%s  :  %s"), _XGETINTERFACETEXT(ID_STRING_NEW_3775), m_strDate[0]);//장원전
		g_XBaseFont->Puts(m_WindowPosition.x+40, m_WindowPosition.y+161, tempString); 

#else
		for(int i = 0 ; i < 3 ; ++i)
		{
			if(i >= (m_pCurCastleInfo->m_nCurMatchCount))
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 200, 198, 45));
				m_pScheduleIcon[0]->Draw(m_WindowPosition.x+16, m_WindowPosition.y+157+(i*21));
			}
			else
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 86, 86, 86));
				m_pScheduleIcon[1]->Draw(m_WindowPosition.x+16, m_WindowPosition.y+157+(i*21));
			}

			TCHAR tempString[512];
			memset( tempString, 0, sizeof(tempString));
			sprintf(tempString, _T("%s  :  %s"), _XGETINTERFACETEXT(ID_STRING_NEW_3219), m_strDate[i]);
			g_XBaseFont->Print(m_WindowPosition.x+40, m_WindowPosition.y+161+(i*21), 1.0f, tempString, i+1); //%d 차 장원전
			//g_XBaseFont->Puts(m_WindowPosition.x+40+63, m_WindowPosition.y+161+(i*21), _T(":"));
		}
#endif
	}

	g_XBaseFont->Flush();
}

BOOL _XWindow_CastleInfo::Process( _XGUIObject*& pfocusobject )
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

void _XWindow_CastleInfo::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	m_BorderTitle.MoveWindow(X+3, Y+18);
}

void _XWindow_CastleInfo::ShowWindow(BOOL show)
{
	if(show)
	{
		// 정보 요청
//#ifdef _XDEF_CASTLEBATTLE_SENDPACKET
		g_NetworkKernel.SendPacket(MSG_NO_CB_SCHEDULE, en_cb_req_schedule_list);
//#endif
		EnableButton(TRUE);
	}
	else
	{
		memset(m_strOrgName, 0, sizeof(m_strOrgName));
		memset(m_strDate, 0, sizeof(m_strDate));
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_CastleInfo::GetIconRect(int groupindex, RECT& rect)
{
	int x = 166, y = 226;

	rect.left = x + ((groupindex%6)*15);
	rect.right = rect.left + 15;
	rect.top = y + ((groupindex/6)*15);
	rect.bottom = rect.top + 15;
}

void _XWindow_CastleInfo::SetData(void)
{
	m_pCurCastleInfo = g_CBManager.GetCastleInfo(g_CurrentZoneInfoIndex);

	if(m_pCurCastleInfo)
	{
		if(m_pCurCastleInfo->m_tOwner.nOrIndex == 0)
		{
			sprintf(m_strOrgName, _XGETINTERFACETEXT(g_CBManager.GetMobMasterNameIndex(g_CurrentZoneInfoIndex)));
		}
		else
		{
			TCHAR orgname[128];
			memset(orgname, 0, sizeof(orgname));

			if(m_pCurCastleInfo->m_tOwner.nType == 0)
			{
				sprintf(orgname, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO2), m_pCurCastleInfo->m_tOwner.szOrgName);	//%s단
			}
			else if(m_pCurCastleInfo->m_tOwner.nType == 1)
			{
				sprintf(orgname, _XGETINTERFACETEXT(ID_STRING_NEW_3125), m_pCurCastleInfo->m_tOwner.szOrgName);	//%s회
			}

			strncpy(m_strOrgName, orgname, strlen(orgname));
//			sprintf(m_strOrgName, g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_3221, 
//				orgname, &m_pCurCastleInfo->m_tOwner.nMemberCnt));
		}

		for(int i = 0 ; i < 3 ; ++i)
		{
			int year = m_pCurCastleInfo->m_tWarSchedule[i].tDate.dwYear + 2000;
			int month = m_pCurCastleInfo->m_tWarSchedule[i].tDate.dwMonth;
			int day = m_pCurCastleInfo->m_tWarSchedule[i].tDate.dwDay;
			int hour = m_pCurCastleInfo->m_tWarSchedule[i].tDate.dwHour;
			int minute = m_pCurCastleInfo->m_tWarSchedule[i].tDate.dwMinute;

			sprintf(m_strDate[i], g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_2654, 
				&year, &month, &day, &hour, &minute));
		}
		
		int year = m_pCurCastleInfo->m_tPropertyDate.dwYear + 2000;
		int month = m_pCurCastleInfo->m_tPropertyDate.dwMonth;
		int day = m_pCurCastleInfo->m_tPropertyDate.dwDay;

		sprintf(m_strPropertyDate, g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_2102,
			&year, &month, &day));
	}
}

void _XWindow_CastleInfo::EnableButton(BOOL enable)
{
	m_btnReqWar->EnableWindow(enable);
}