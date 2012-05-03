// XWindow_CastleBattlePersonalHistory.cpp: implementation of the _XWindow_CastleBattlePersonalHistory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_CastleBattlePersonalHistory.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_CastleBattlePersonalHistory::_XWindow_CastleBattlePersonalHistory()
{
	m_pImageGreenPoint		= NULL;		// 녹색불
	m_pImageRedPoint		= NULL;		// 빨간불
	m_pImageGrayPoint		= NULL;		// 회색불
	
	m_GaugeThreate			= NULL;		// 협박 게이지
	m_GaugeSettle			= NULL;		// 진정 게이지

	for( int i = 0; i < 10; ++i )
		m_ThreatePoint[i] = 0;
	m_ThreatePoint[0] = 1;
	
	m_GaugeFactor = 0.0f;
	
	m_nOwnerType		= 0;
	m_nOwnerIndex		= 0;
	memset(m_szOwnerName, 0, sizeof(m_szOwnerName));
	memset(m_szThreatenString, 0, sizeof(m_szThreatenString));

	m_nPoint		= 0;
	m_GaugeMode		= _XDEF_CB_GAUGEMODE_NONE;
	
}

_XWindow_CastleBattlePersonalHistory::~_XWindow_CastleBattlePersonalHistory()
{

}

BOOL _XWindow_CastleBattlePersonalHistory::Initialize(void)
{	
	int manorresourceindex02	= g_MainInterfaceTextureArchive.FindResource("MI_manor02.tga");
	int manorresourceindex03	= g_MainInterfaceTextureArchive.FindResource("MI_manor03.tga");
	int pvpresourceindex01		= g_MainInterfaceTextureArchive.FindResource("MI_pvp_01.tga");
	
	// 타이틀 바 ( 187 X 19 )
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 187, 19, &g_MainInterfaceTextureArchive, manorresourceindex03 );
	pTitleBar->SetClipRect( 2, 214, 189, 233 );
	InsertChildObject( pTitleBar );
	
	// 바닥 바 ( 187 X 19 )
	_XImageStatic* pBottomBar = new _XImageStatic;
#ifdef _XDEF_CASTLEBATTLE_RECORD
	pBottomBar->Create( 0, 152, 187, 171, &g_MainInterfaceTextureArchive, manorresourceindex03 );
#else
	pBottomBar->Create( 0, 125, 187, 144, &g_MainInterfaceTextureArchive, manorresourceindex03 );
#endif
	pBottomBar->SetClipRect( 2, 235, 189, 254 );
	InsertChildObject( pBottomBar );
	
	// 게이지 왼쪽 바 ( 13 X 13 )
	_XImageStatic* pGaugeLeft = new _XImageStatic;
	pGaugeLeft->Create( 25, 87, 38, 100, &g_MainInterfaceTextureArchive, manorresourceindex02 );
	pGaugeLeft->SetClipRect( 108, 159, 121, 172 );
	InsertChildObject( pGaugeLeft );
	
	// 게이지 오른쪽 바 ( 14 X 13 )
	_XImageStatic* pGaugeRight = new _XImageStatic;
	pGaugeRight->Create( 150, 87, 164, 100, &g_MainInterfaceTextureArchive, manorresourceindex02 );
	pGaugeRight->SetClipRect( 233, 159, 247, 172 );
	InsertChildObject( pGaugeRight );
	
	// 회색 포인트 ( 15 X 15 )
	m_pImageGrayPoint = new _XImageStatic;
	m_pImageGrayPoint->Create( 0, 0, 15, 15, &g_MainInterfaceTextureArchive, pvpresourceindex01 );
	m_pImageGrayPoint->SetClipRect( 113, 104, 128, 119 );
	
	// 녹색 포인트 ( 15 X 15 )
	m_pImageGreenPoint = new _XImageStatic;
	m_pImageGreenPoint->Create( 0, 0, 15, 15, &g_MainInterfaceTextureArchive, pvpresourceindex01 );
	m_pImageGreenPoint->SetClipRect( 113, 88, 128, 103 );
	
	// 빨간 포인트 ( 15 X 15 )
	m_pImageRedPoint = new _XImageStatic;
	m_pImageRedPoint->Create( 0, 0, 15, 15, &g_MainInterfaceTextureArchive, manorresourceindex02 );
	m_pImageRedPoint->SetClipRect( 175, 129, 190, 144 );
	
	_XSTICKGAUGE_STRUCTURE thretegaugestruct =
	{
		TRUE, {38, 87}, {112, 13},
			manorresourceindex02,manorresourceindex02,
			&g_MainInterfaceTextureArchive
	};
	
	m_GaugeThreate = new _XStickGauge;
	m_GaugeThreate->Create(thretegaugestruct);	
	m_GaugeThreate->SetImageClipRect(_XGAUGEIMAGE_OVERLAY, 121, 159, 233, 172);
	m_GaugeThreate->SetImageClipRect(_XGAUGEIMAGE_BORDER, 121, 172, 233, 185);
	m_GaugeThreate->SetFactorDraw(TRUE);
	m_GaugeThreate->SetBackGroundDraw(TRUE);
	m_GaugeThreate->SetGaugeFactor( 0.0f );
	m_GaugeThreate->SetGaugeColorFactor( 0xFFFFFFFF, 0x00000000 );
	m_GaugeThreate->SetViewDelayGauge( FALSE );
	m_GaugeThreate->SetGaugeImageDrawType(_XGAUGEIMAGEDRAWTYPE_RESIZE);
	m_GaugeThreate->ShowWindow(FALSE);
	InsertChildObject(m_GaugeThreate);
	
	
	_XSTICKGAUGE_STRUCTURE settlegaugestruct =
	{
		TRUE, {38, 87}, {112, 13},
			manorresourceindex02,manorresourceindex02,
			&g_MainInterfaceTextureArchive
	};
	
	m_GaugeSettle = new _XStickGauge;
	m_GaugeSettle->Create(thretegaugestruct);	
	m_GaugeSettle->SetImageClipRect(_XGAUGEIMAGE_OVERLAY, 121, 159, 233, 172);
	m_GaugeSettle->SetImageClipRect(_XGAUGEIMAGE_BORDER, 121, 185, 233, 198);
	m_GaugeSettle->SetFactorDraw(FALSE);
	m_GaugeSettle->SetBackGroundDraw(TRUE);
	m_GaugeSettle->SetGaugeFactor( 0.0f );
	m_GaugeSettle->SetGaugeColorFactor( 0xFFFFFFFF, 0x00000000 );
	m_GaugeSettle->SetViewDelayGauge( FALSE );
	m_GaugeSettle->SetGaugeImageDrawType(_XGAUGEIMAGEDRAWTYPE_RESIZE);
//	m_GaugeSettle->SetGaugeProgressType(_XGAUGEPROGRESSTYPE_BACKWARD);
	m_GaugeSettle->ShowWindow(FALSE);
	InsertChildObject(m_GaugeSettle);

#ifdef _XDEF_CASTLEBATTLE_RECORD
	int btnresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {40, 127}, {106, 22},
			_XDEF_CBPERSONAL_VIEWRECORD,
			btnresourceindex,
			btnresourceindex,
			btnresourceindex,
			&g_MainInterfaceTextureArchive
	};

	_XButton* pRecordButton = new _XButton;
	pRecordButton->Create(btnstruct);
	pRecordButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 3, 177, 109, 199);
	pRecordButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 3, 153, 109, 175);
	pRecordButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 3, 202, 109, 224);

	pRecordButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	pRecordButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3400);	//"장원 전적 보기"
	InsertChildObject(pRecordButton);
#endif
	
	return TRUE;
}

void _XWindow_CastleBattlePersonalHistory::DestroyWindow()
{
	SAFE_DELETE(m_pImageGrayPoint);
	SAFE_DELETE(m_pImageGreenPoint);
	SAFE_DELETE(m_pImageRedPoint);
	_XWindow::DestroyWindow();
}

void _XWindow_CastleBattlePersonalHistory::Draw(_XGUIObject*& pfocusobject)
{
	
	if(this->m_ShowWindow)
	{
#ifdef _XDEF_CASTLEBATTLE_RECORD
		_XDrawSolidBar( m_WindowPosition.x+2, m_WindowPosition.y+19, m_WindowPosition.x+2+182, m_WindowPosition.y+19+133 , 0xFF000000 );
		_XDrawLine2D( m_WindowPosition.x+3, m_WindowPosition.y+19, m_WindowPosition.x+3, m_WindowPosition.y+19+133, D3DCOLOR_ARGB(255,115,93,69), D3DCOLOR_ARGB(255,115,93,69));
		_XDrawLine2D( m_WindowPosition.x+182, m_WindowPosition.y+19, m_WindowPosition.x+182, m_WindowPosition.y+19+133, D3DCOLOR_ARGB(255,115,93,69), D3DCOLOR_ARGB(255,115,93,69));
		_XDrawRectAngle( m_WindowPosition.x+3, m_WindowPosition.y+36, m_WindowPosition.x+3+179, m_WindowPosition.y+36+72 , 0.0f, D3DCOLOR_ARGB(255,115,93,69) );
		_XDrawLine2D( m_WindowPosition.x+3, m_WindowPosition.y+125, m_WindowPosition.x+182, m_WindowPosition.y+125, D3DCOLOR_ARGB(255,115,93,69), D3DCOLOR_ARGB(255,115,93,69));
#else
		_XDrawSolidBar( m_WindowPosition.x+2, m_WindowPosition.y+19, m_WindowPosition.x+2+182, m_WindowPosition.y+19+106 , 0xFF000000 );
		_XDrawLine2D( m_WindowPosition.x+3, m_WindowPosition.y+19, m_WindowPosition.x+3, m_WindowPosition.y+19+106, D3DCOLOR_ARGB(255,115,93,69), D3DCOLOR_ARGB(255,115,93,69));
		_XDrawLine2D( m_WindowPosition.x+182, m_WindowPosition.y+19, m_WindowPosition.x+182, m_WindowPosition.y+19+106, D3DCOLOR_ARGB(255,115,93,69), D3DCOLOR_ARGB(255,115,93,69));
		_XDrawRectAngle( m_WindowPosition.x+3, m_WindowPosition.y+36, m_WindowPosition.x+3+179, m_WindowPosition.y+36+72 , 0.0f, D3DCOLOR_ARGB(255,115,93,69) );
#endif
	}
	
	_XWindow::Draw(pfocusobject);
	
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
	
	DWORD elapsedtime = (g_LocalSystemTime - g_CBManager.m_dwTimeFromServer)/1000;
	
	int minute = elapsedtime / 60;
	int second = elapsedtime % 60;
	
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_XBaseFont->SetBoldMode(TRUE);
	g_XBaseFont->PrintAlign( m_WindowPosition.x+94, m_WindowPosition.y+3, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(g_CBManager.GetCastleNameTextId(g_pLocalUser->m_nCBMapIdInWar)));	//장원이름
	
	g_XBaseFont->Print( m_WindowPosition.x+9, m_WindowPosition.y+21, 1.0f, _XGETINTERFACETEXT(ID_STRING_NEW_3370), m_szOwnerName );//_T("현 소유주 : %s")
	g_XBaseFont->Flush();
	g_XBaseFont->SetBoldMode(FALSE);
	
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 226, 175, 48));

	if(m_GaugeMode == _XDEF_CB_GAUGEMODE_NONE)
	{
		g_XBaseFont->Puts_Separate( m_WindowPosition.x+9, m_WindowPosition.y+42, _XGETINTERFACETEXT(ID_STRING_NEW_3340), 170, 1.0f, 3 );	//단원이 대전 중 승리하거나 일정 시간이 흐르면 장원혈이 증가합니다.
	}
	else if(m_GaugeMode == _XDEF_CB_GAUGEMODE_DELAY)
	{
		g_XBaseFont->Puts_Separate( m_WindowPosition.x+9, m_WindowPosition.y+42, _XGETINTERFACETEXT(ID_STRING_NEW_3342), 170, 1.0f, 3 );	//장주가 심기를 안정시키고 있습니다.
	}
	else if(m_GaugeMode == _XDEF_CB_GAUGEMODE_THREAT)
	{
		g_XBaseFont->Puts_Separate( m_WindowPosition.x+9, m_WindowPosition.y+42, m_szThreatenString, 170, 1.0f, 3 );
	}
	g_XBaseFont->Flush();

	g_XBaseFont->SetBoldMode(TRUE);
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
#ifdef _XDEF_CASTLEBATTLE_RECORD
	g_XBaseFont->PrintAlign( m_WindowPosition.x+94, m_WindowPosition.y+156, 1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d:%02d"),minute, second );
#else
	g_XBaseFont->PrintAlign( m_WindowPosition.x+94, m_WindowPosition.y+130, 1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%d:%02d"),minute, second );
#endif

	g_XBaseFont->Flush();
	g_XBaseFont->SetBoldMode(FALSE);

	
	for( int i = 0; i < 10; ++i )
	{
		if( m_ThreatePoint[i] == 0 )//회색
		{
			m_pImageGrayPoint->Draw(m_WindowPosition.x+14+(16*i), m_WindowPosition.y+109);
		}
		else if( m_ThreatePoint[i] == 2)//녹색
		{
			m_pImageGreenPoint->Draw(m_WindowPosition.x+14+(16*i), m_WindowPosition.y+109);
		}
		else if( m_ThreatePoint[i] == 1 )//빨강
		{
			m_pImageRedPoint->Draw(m_WindowPosition.x+14+(16*i), m_WindowPosition.y+109);
		}
	}
	

}

BOOL _XWindow_CastleBattlePersonalHistory::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
		
	// 협박 dealy gauge
	if(m_GaugeMode == _XDEF_CB_GAUGEMODE_DELAY)
	{
		if(g_CBManager.m_dwThreatenRequestTime > 0)
		{
			m_fDelayGaugeFactor = ((g_LocalSystemTime - g_CBManager.m_dwThreatenRequestTime)/((FLOAT)g_CBManager.GetThreatenDelayTime()*1000.f))*100.f;

			// 100% 부터 거꾸로
			m_fDelayGaugeFactor = 100.0f - m_fDelayGaugeFactor;

			if(m_fDelayGaugeFactor <= 0.f)
			{
				m_fDelayGaugeFactor = 0.0f;
				SetGaugeMode(_XDEF_CB_GAUGEMODE_NONE);
			}

			m_GaugeSettle->SetGaugeFactor(m_fDelayGaugeFactor);
		}
	}

	return TRUE;
}

void _XWindow_CastleBattlePersonalHistory::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
}

void _XWindow_CastleBattlePersonalHistory::ShowWindow(BOOL show)
{	
	if( show )
	{
		m_GaugeThreate->ShowWindow(TRUE);
	}
	else
	{
		m_nOwnerType		= 0;
		m_nOwnerIndex		= 0;
		memset(m_szOwnerName, 0, sizeof(m_szOwnerName));
		memset(m_szThreatenString, 0, sizeof(m_szThreatenString));
		m_nPoint			= 0;
		SetGaugeMode(_XDEF_CB_GAUGEMODE_NONE);
	}

	_XWindow::ShowWindow(show);
}


void _XWindow_CastleBattlePersonalHistory::SetOwner(int type, int index)
{
	m_nOwnerType = type;
	m_nOwnerIndex = index;

	int myType = 0, myIndex = 0;

	if(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0)
	{
		myType = 1;
		myIndex = g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex;
	}
	else
	{
		if(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0)
		{
			myType = 0;
			myIndex = g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex;
		}
	}

	if(myType == type && myIndex == index)
	{
		// 내가 속해있는 곳이 owner
		if(myType == 0)
		{
			sprintf(m_szOwnerName, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO2), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName);	//%s단
		}
		else if(myType == 1)
		{
			sprintf(m_szOwnerName, _XGETINTERFACETEXT(ID_STRING_NEW_3125), g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName);	//%s회
		}

		return;
	}

	if(type == 0)
	{
		// Org
		_XUSER_ORGANIZATION_GLOBAL_INFO* orginfo = g_Userlist.SearchOrganizationInfo(index);
		if(orginfo)
		{
			sprintf(m_szOwnerName, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO2), orginfo->m_cName);	//%s단
		}
	}
	else if(type == 1)
	{
		// Ally
		_XUSER_ALIENCE_GLOBAL_INFO* allyinfo = g_Userlist.SearchAlienceInfo(index);
		if(allyinfo)
		{
			sprintf(m_szOwnerName, _XGETINTERFACETEXT(ID_STRING_NEW_3125), allyinfo->m_cName);	//%s회
		}
	}
	else
	{
		// monster
		sprintf(m_szOwnerName, _XGETINTERFACETEXT(g_CBManager.GetMobMasterNameIndex(g_CurrentZoneInfoIndex)));
	}
}

void _XWindow_CastleBattlePersonalHistory::SetThreatenString(int type, int index, BOOL bMyOrg)
{
	int myType = 0, myIndex = 0;

	TCHAR orname[128];
	memset(orname, 0, sizeof(orname));

	if(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0)
	{
		myType = 1;
		myIndex = g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex;
	}
	else
	{
		if(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0)
		{
			myType = 0;
			myIndex = g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex;
		}
	}

	if(bMyOrg)
	{
		// 내가 속해있는 곳이 owner
		if(myType == 0)
		{
			sprintf(orname, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO2), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName);	//%s단
		}
		else if(myType == 1)
		{
			sprintf(orname, _XGETINTERFACETEXT(ID_STRING_NEW_3125), g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName);	//%s회
		}

		sprintf(m_szThreatenString, _XGETINTERFACETEXT(ID_STRING_NEW_3341), orname);	//%s(이)가 장주를 협박하는중 입니다.

		return;
	}

	if(type == 0)
	{
		// Org
		_XUSER_ORGANIZATION_GLOBAL_INFO* orginfo = g_Userlist.SearchOrganizationInfo(index);
		if(orginfo)
		{
			sprintf(orname, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO2), orginfo->m_cName);	//%s단
		}
	}
	else if(type == 1)
	{
		// Ally
		_XUSER_ALIENCE_GLOBAL_INFO* allyinfo = g_Userlist.SearchAlienceInfo(index);
		if(allyinfo)
		{
			sprintf(orname, _XGETINTERFACETEXT(ID_STRING_NEW_3125), allyinfo->m_cName);	//%s회
		}
	}

	sprintf(m_szThreatenString, _XGETINTERFACETEXT(ID_STRING_NEW_3341), orname);	//%s(이)가 장주를 협박하는중 입니다.
}

void _XWindow_CastleBattlePersonalHistory::SetPoint(int point)
{
	m_nPoint = point;
	
	for(int i = 0 ; i < 10 ; i++)
	{
		m_ThreatePoint[i] = 0;		// 회색
	}

	for(i = 0 ; i < point ; i++)
	{
		m_ThreatePoint[i] = 2;		// 녹색
	}
	m_ThreatePoint[point] = 1;
}

void _XWindow_CastleBattlePersonalHistory::SetGaugeMode(EN_THREAT_GAUGEMODE mode)
{
	m_GaugeMode = mode;

	switch(mode)
	{
	case _XDEF_CB_GAUGEMODE_NONE :
		{
			m_GaugeThreate->ShowWindow(TRUE);
			m_GaugeSettle->ShowWindow(FALSE);
			m_fThreatenGaugeFactor = 0.0f;
			m_GaugeThreate->SetGaugeFactor(0.0f);
		}
		break;
	case _XDEF_CB_GAUGEMODE_THREAT :
		{
			m_GaugeThreate->ShowWindow(TRUE);
			m_GaugeSettle->ShowWindow(FALSE);
		}
		break;
	case _XDEF_CB_GAUGEMODE_DELAY :
		{
			m_GaugeThreate->ShowWindow(FALSE);
			m_GaugeSettle->ShowWindow(TRUE);
		}
		break;
	}
}

void _XWindow_CastleBattlePersonalHistory::SetThreatenParameter(FLOAT parameter)
{
	m_fThreatenGaugeFactor = parameter;
	m_GaugeThreate->SetGaugeFactor(m_fThreatenGaugeFactor);
}

void _XWindow_CastleBattlePersonalHistory::SetDelayParameter(FLOAT parameter)
{
	m_fDelayGaugeFactor = parameter;
	m_GaugeSettle->SetGaugeFactor(m_fDelayGaugeFactor);
}