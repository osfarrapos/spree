// XWindow_MatchNoticeSmall.cpp: implementation of the _XWindow_MatchNoticeSmall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_MatchNoticeSmall.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MatchNoticeSmall::_XWindow_MatchNoticeSmall()
{
	m_MyVitalGauge = NULL;
	m_TargetVitalGauge = NULL;
}

_XWindow_MatchNoticeSmall::~_XWindow_MatchNoticeSmall()
{

}

BOOL _XWindow_MatchNoticeSmall::Initialize()
{
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

	_XImageStatic* pLeftBorder = new _XImageStatic;
	pLeftBorder->Create(0, 0, 60, 59, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pLeftBorder->SetClipRect(0, 121, 60, 180);
	InsertChildObject(pLeftBorder);

	_XImageStatic* pRightBorder = new _XImageStatic;
	pRightBorder->Create(212, 0, 222, 59, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pRightBorder->SetClipRect(72, 121, 82, 180);
	InsertChildObject(pRightBorder);

	_XImageStatic* pCenterBorder = new _XImageStatic;
	pCenterBorder->Create(60, 0, 70, 59, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pCenterBorder->SetClipRect(61, 121, 71, 180);
	pCenterBorder->SetScale(15.2f, 1.0f);
	InsertChildObject(pCenterBorder);

	_XImageStatic* pVSBorder = new _XImageStatic;
	pVSBorder->Create(99, 26, 123, 50, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pVSBorder->SetClipRect(86, 155, 110, 179);
	InsertChildObject(pVSBorder);

	_XImageStatic* pVSImage = new _XImageStatic;
	pVSImage->Create(102, 28, 1, 48, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pVSImage->SetClipRect(146, 1, 165, 21);
	InsertChildObject(pVSImage);

	m_DefaultRecordImage.Create(0, 0, 10, 10, &g_MainInterfaceTextureArchive, pvpresourceindex);
	m_DefaultRecordImage.SetClipRect(83, 104, 93, 114);

	_XImageStatic* pMyHPBorder_1 = new _XImageStatic;
	pMyHPBorder_1->Create(9, 32, 97, 44, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pMyHPBorder_1->SetClipRect(82, 137, 126, 149);
	InsertChildObject(pMyHPBorder_1);

	_XImageStatic* pMyHPBorder_2 = new _XImageStatic;
	pMyHPBorder_2->Create(97, 32, 97, 44, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pMyHPBorder_2->SetClipRect(82, 137, 126, 149);
	pMyHPBorder_2->SetScale(-1, 1);
	InsertChildObject(pMyHPBorder_2);

	_XImageStatic* pTargetHPBorder_1 = new _XImageStatic;
	pTargetHPBorder_1->Create(125, 32, 213, 44, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pTargetHPBorder_1->SetClipRect(82, 137, 126, 149);
	InsertChildObject(pTargetHPBorder_1);
	
	_XImageStatic* pTargetHPBorder_2 = new _XImageStatic;
	pTargetHPBorder_2->Create(213, 32, 213, 44, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pTargetHPBorder_2->SetClipRect(82, 137, 126, 149);
	pTargetHPBorder_2->SetScale(-1, 1);
	InsertChildObject(pTargetHPBorder_2);

	_XSTICKGAUGE_STRUCTURE gaugestruct = 
	{
		TRUE, {12, 35}, {82, 6},
			pvpresourceindex,
			-1,
			&g_MainInterfaceTextureArchive
	};
	
	m_MyVitalGauge = new _XStickGauge;
	m_MyVitalGauge->Create(gaugestruct);
	m_MyVitalGauge->SetImageClipRect(_XGAUGEIMAGE_BORDER, 84, 123, 166, 129);
	m_MyVitalGauge->SetFactorDraw(TRUE);
	m_MyVitalGauge->SetGaugeFactor(100.0f);
	m_MyVitalGauge->SetGaugeImageDrawType( _XGAUGEIMAGEDRAWTYPE_RESIZE );
	m_MyVitalGauge->SetViewDelayGauge( TRUE, 20 );
	m_MyVitalGauge->SetBlinking( TRUE, 10 );
	InsertChildObject(m_MyVitalGauge);
	
	gaugestruct.position.x = 128;
	gaugestruct.position.y = 35;
	
	m_TargetVitalGauge = new _XStickGauge;
	m_TargetVitalGauge->Create(gaugestruct);
	m_TargetVitalGauge->SetImageClipRect(_XGAUGEIMAGE_BORDER, 84, 130, 166, 136);
	m_TargetVitalGauge->SetFactorDraw(TRUE);
	m_TargetVitalGauge->SetGaugeFactor(100.0f);
	m_TargetVitalGauge->SetGaugeImageDrawType( _XGAUGEIMAGEDRAWTYPE_RESIZE );
	m_TargetVitalGauge->SetViewDelayGauge( TRUE, 20 );
	m_TargetVitalGauge->SetBlinking( TRUE, 10 );
	InsertChildObject(m_TargetVitalGauge);

	return TRUE;
}

void _XWindow_MatchNoticeSmall::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	for(int i = 0 ; i < 9 ; i++)
	{
		m_DefaultRecordImage.Draw(m_WindowPosition.x+12+(i*9), m_WindowPosition.y+44);
		m_DefaultRecordImage.Draw(m_WindowPosition.x+129+(i*9), m_WindowPosition.y+44);
	}
	
	g_XBaseFont->SetColor(0xFFdecb80);
	g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_FULLEDGE, 0xFF000000);
	g_XBaseFont->PrintAlign(m_WindowPosition.x+52, m_WindowPosition.y+18, 1.0f, _XFONT_ALIGNTYPE_CENTER, g_pLocalUser->m_CharacterInfo.charactername);
	g_XBaseFont->PrintAlign(m_WindowPosition.x+168, m_WindowPosition.y+18, 1.0f, _XFONT_ALIGNTYPE_CENTER, g_pLocalUser->m_myMatchMemberInfo.cName);
	g_XBaseFont->DisableGrowMode();
	g_XBaseFont->Flush();
}

BOOL _XWindow_MatchNoticeSmall::Process(_XGUIObject*& pfocusobject)
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

void _XWindow_MatchNoticeSmall::SetParameter()
{
	short maxlife = 0;
	maxlife = g_pLocalUser->m_MaxLifePower.Get_result();
	
	if( maxlife )
	{
		g_pLocalUser->LifePowerPercentage	=	(int)(((FLOAT)g_pLocalUser->m_CharacterInfo.Get_current_lifepower() / (FLOAT)maxlife) * 100.0f);
	}
	else
		g_pLocalUser->LifePowerPercentage	=	0;
	
	m_MyVitalGauge->SetGaugeFactor(g_pLocalUser->LifePowerPercentage);
	m_TargetVitalGauge->SetGaugeFactor(g_pLocalUser->m_myMatchMemberInfo.ucVitalRate);
}