// XWindow_SkillTooltip.cpp: implementation of the _XWindow_SkillTooltip class.
//
// Created Date	: 2003.03.03
// Author		: Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_SkillTooltip.h"
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
void __stdcall _XStickGaugeCallback_NeedSkillExp( POINT& winpos, SIZE& winsize, FLOAT& factor )
{
	
	_XWindow_SkillTooltip* pSkillTooltip_Window = (_XWindow_SkillTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SKILLTOOLTIP) ;
	
	int skillid = pSkillTooltip_Window->GetSkillId();
	int skillindex = g_pLocalUser->GetLearnedSkillIndex(skillid);

	int totalexp = 0;
	if(g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel >= 11)
	{
		totalexp = g_SkillProperty[skillid]->detailInfo[g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel].skillExp;
	}
	else
	{
		totalexp = g_SkillProperty[skillid]->detailInfo[g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel+1].skillExp;
	}
	
	g_XSmallFont->SetColor( _XSC_GAUGENUMBER );
	g_XSmallFont->PrintAlign( winpos.x + (winsize.cx>>1), winpos.y-2, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%d/%d",
							 _XFC_FloatToIntRet(g_pLocalUser->m_SkillList[skillindex].m_iSkillExp),	totalexp );
	g_XSmallFont->Flush();
}


_XWindow_SkillTooltip::_XWindow_SkillTooltip()
{
	m_SkillIcon_Default		= NULL;
	m_SkillIcon_NangIn		= NULL;
	m_SkillIcon_GaeBang		= NULL;
	m_SkillIcon_BeeGoong	= NULL;
	m_SkillIcon_SoRim		= NULL;
	m_SkillIcon_NockRim		= NULL;
	
	m_SkillIcon_Default_g	= NULL;
	m_SkillIcon_NangIn_g	= NULL;
	m_SkillIcon_GaeBang_g	= NULL;
	m_SkillIcon_BeeGoong_g	= NULL;
	m_SkillIcon_SoRim_g		= NULL;
	m_SkillIcon_NockRim_g	= NULL;

	m_ParentWindow = NULL;

	m_SkillID = 0;
	m_ShowSignalTime = 0;

	m_pNeedSkillExpGuage = NULL;
}

_XWindow_SkillTooltip::~_XWindow_SkillTooltip()
{

}

BOOL _XWindow_SkillTooltip::Initialize(void)
{
	SAFE_DELETE( m_SkillIcon_Default );
	SAFE_DELETE( m_SkillIcon_NangIn );
	SAFE_DELETE( m_SkillIcon_GaeBang );
	SAFE_DELETE( m_SkillIcon_BeeGoong );
	SAFE_DELETE( m_SkillIcon_SoRim );
	SAFE_DELETE( m_SkillIcon_NockRim );

	SAFE_DELETE( m_SkillIcon_Default_g );
	SAFE_DELETE( m_SkillIcon_NangIn_g );
	SAFE_DELETE( m_SkillIcon_GaeBang_g );
	SAFE_DELETE( m_SkillIcon_BeeGoong_g );
	SAFE_DELETE( m_SkillIcon_SoRim_g );
	SAFE_DELETE( m_SkillIcon_NockRim_g );
	
	m_SkillIcon_Default = new _XImageStatic;
	m_SkillIcon_Default->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_10.tga"));
	m_SkillIcon_NangIn = new _XImageStatic;
	m_SkillIcon_NangIn->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_00.tga"));
	m_SkillIcon_GaeBang = new _XImageStatic;
	m_SkillIcon_GaeBang->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_01.tga"));
	m_SkillIcon_BeeGoong = new _XImageStatic;
	m_SkillIcon_BeeGoong->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_02.tga"));
	m_SkillIcon_SoRim = new _XImageStatic;
	m_SkillIcon_SoRim->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_03.tga"));
	m_SkillIcon_NockRim = new _XImageStatic;
	m_SkillIcon_NockRim->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_04.tga"));

	m_SkillIcon_Default_g = new _XImageStatic;
	m_SkillIcon_Default_g->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_10.tga"));
	m_SkillIcon_NangIn_g = new _XImageStatic;
	m_SkillIcon_NangIn_g->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_00.tga"));
	m_SkillIcon_GaeBang_g = new _XImageStatic;
	m_SkillIcon_GaeBang_g->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_01.tga"));
	m_SkillIcon_BeeGoong_g = new _XImageStatic;
	m_SkillIcon_BeeGoong_g->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_02.tga"));
	m_SkillIcon_SoRim_g = new _XImageStatic;
	m_SkillIcon_SoRim_g->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_03.tga"));
	m_SkillIcon_NockRim_g = new _XImageStatic;
	m_SkillIcon_NockRim_g->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_04.tga"));

	m_SkillID		= 0;
	m_ParentWindow	= NULL;
	SetWindowMoveMode(_XWMOVE_FIXED);

	memset(m_ClanTypeString, 0, sizeof(TCHAR)*16);
	strncpy(m_ClanTypeString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CLANTYPE), 15);
	memset(m_SkillSTypeString, 0, sizeof(TCHAR)*16);
	strncpy(m_SkillSTypeString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SKILLSTYPE), 15);
	memset(m_WeaponCTypeString, 0, sizeof(TCHAR)*16);
	strncpy(m_WeaponCTypeString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_USEWEAPON), 15);
	memset(m_TargetTypeString, 0, sizeof(TCHAR)*16);
	strncpy(m_TargetTypeString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_TARGETTYPE), 15);
	memset(m_UsefulTypeString, 0, sizeof(TCHAR)*16);
	strncpy(m_UsefulTypeString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_USEFULTYPE), 15);
	memset(m_FERAString, 0, sizeof(TCHAR)*16);
	strncpy(m_FERAString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FERA), 15);
	memset(m_CombinationString, 0, sizeof(TCHAR)*16);
	strncpy(m_CombinationString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_COMBINATION), 15);
	memset(m_InsideTypeString, 0, sizeof(TCHAR)*16);
	strncpy(m_InsideTypeString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_INSIDETYPE), 15);
	memset(m_AttackDamageString, 0, sizeof(TCHAR)*16);
	strncpy(m_AttackDamageString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ATTACKDAMAGE), 15);
	memset(m_AttackRatingString, 0, sizeof(TCHAR)*16);
	strncpy(m_AttackRatingString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ATTACKRATING), 15);
	memset(m_AvoidRatingString, 0, sizeof(TCHAR)*16);
	strncpy(m_AvoidRatingString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_AVOIDRATING), 15);
	memset(m_CostForceString, 0, sizeof(TCHAR)*16);
	strncpy(m_CostForceString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_COSTFORCE), 15);
	memset(m_CostMindString, 0, sizeof(TCHAR)*16);
	strncpy(m_CostMindString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_COSTMIND), 15);
	memset(m_CostManaString, 0, sizeof(TCHAR)*16);
	strncpy(m_CostManaString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_COSTMANA), 15);
	memset(m_CostEnergyString, 0, sizeof(TCHAR)*16);
	strncpy(m_CostEnergyString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_COSTENERGY), 15);
	memset(m_TechniqueExpString, 0, sizeof(TCHAR)*16);
	strncpy(m_TechniqueExpString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_TECHNIQUEEXP), 15);
	memset(m_ReqMissionString, 0, sizeof(TCHAR)*16);
	strncpy(m_ReqMissionString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_REQMISSION), 15);
	memset(m_ObjectIDString, 0, sizeof(TCHAR)*16);
	strncpy(m_ObjectIDString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_OBJECTID), 15);
	memset(m_AddBonusString, 0, sizeof(TCHAR)*16);
	strncpy(m_AddBonusString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ADDBONUS), 15);
	memset(m_NumberString, 0, sizeof(TCHAR)*16);
	strncpy(m_NumberString, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_NUMBER), 15);
	memset(m_Effect1String, 0, sizeof(TCHAR)*16);
	strncpy(m_Effect1String, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_EFFECT1), 15);
	memset(m_Effect2String, 0, sizeof(TCHAR)*16);
	strncpy(m_Effect2String, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_EFFECT2), 15);
	memset(m_Effect3String, 0, sizeof(TCHAR)*16);
	strncpy(m_Effect3String, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_EFFECT3), 15);
	memset(m_Effect4String, 0, sizeof(TCHAR)*16);
	strncpy(m_Effect4String, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_EFFECT4), 15);

	int gaugeimageindex = g_MainInterfaceTextureArchive.FindResource( "gaugebar_common.tga" );
	_XSTICKGAUGE_STRUCTURE	gaugestruct = 
	{
		TRUE, { 42, 26 }, { 150, 7 },	// 2004.05.22->oneway48 modify
			gaugeimageindex, -1, &g_MainInterfaceTextureArchive
	};
	
	m_pNeedSkillExpGuage = new _XStickGauge;
	m_pNeedSkillExpGuage->Create( gaugestruct );	
	m_pNeedSkillExpGuage->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 18, 150, 25 );
	m_pNeedSkillExpGuage->SetCallBackFunction_FactorDraw( _XStickGaugeCallback_NeedSkillExp );
	m_pNeedSkillExpGuage->SetFactorDraw(TRUE);
	//m_pStaminaBorder->SetImageClipRect( _XGAUGEIMAGE_OVERLAY, 0, 53, 65, 60 );
	InsertChildObject( m_pNeedSkillExpGuage );
		
	return TRUE;
}

void _XWindow_SkillTooltip::DestroyWindow(void)
{   
	SAFE_DELETE( m_SkillIcon_Default );
	SAFE_DELETE( m_SkillIcon_NangIn );
	SAFE_DELETE( m_SkillIcon_GaeBang );
	SAFE_DELETE( m_SkillIcon_BeeGoong );
	SAFE_DELETE( m_SkillIcon_SoRim );
	SAFE_DELETE( m_SkillIcon_NockRim );	

	SAFE_DELETE( m_SkillIcon_Default_g );
	SAFE_DELETE( m_SkillIcon_NangIn_g );
	SAFE_DELETE( m_SkillIcon_GaeBang_g );
	SAFE_DELETE( m_SkillIcon_BeeGoong_g );
	SAFE_DELETE( m_SkillIcon_SoRim_g );
	SAFE_DELETE( m_SkillIcon_NockRim_g );
	
	_XWindow::DestroyWindow();
}

void _XWindow_SkillTooltip::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;

	// Border edge...
	_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, _XSC_DEFAULT_BLACK );
	_XDrawRectAngle( m_WindowPosition.x+1, m_WindowPosition.y+1, m_WindowPosition.x+m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f, D3DCOLOR_ARGB(255,96,96,96) );
	_XDrawRectAngle( m_WindowPosition.x+2, m_WindowPosition.y+2, m_WindowPosition.x+m_WindowSize.cx-3, m_WindowPosition.y+m_WindowSize.cy-3, 0.0f, _XSC_DEFAULT_BLACK );

	// Icon...	
	_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+5, m_WindowPosition.x+36, m_WindowPosition.y+36, 0.0f, _XSC_DEFAULT_BLACK );
	
	
	// Skill title
	_XDrawSolidBar( m_WindowPosition.x+41, m_WindowPosition.y+6, m_WindowPosition.x+194, m_WindowPosition.y+36 ,D3DCOLOR_ARGB(64,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+40, m_WindowPosition.y+5, m_WindowPosition.x+194, m_WindowPosition.y+36, 0.0f, _XSC_DEFAULT_BLACK );
	// 2004.05.22->oneway48 modify : Skill gauge border 26->25
	_XDrawRectAngle( m_WindowPosition.x+42, m_WindowPosition.y+25, m_WindowPosition.x+192, m_WindowPosition.y+34, 0.0f, D3DCOLOR_ARGB(200,16,16,16) );

	// Skill description
	_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+40, m_WindowPosition.x+194, m_WindowPosition.y+71 , D3DCOLOR_ARGB(64,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+39, m_WindowPosition.x+194, m_WindowPosition.y+71 , 0.0f, _XSC_DEFAULT_BLACK );

	// description group1
	_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+80, m_WindowPosition.x+194, m_WindowPosition.y+154 , D3DCOLOR_ARGB(64,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+79, m_WindowPosition.x+194, m_WindowPosition.y+154 , 0.0f, _XSC_DEFAULT_BLACK );

	// description group2
	_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+156, m_WindowPosition.x+194, m_WindowPosition.y+206 , D3DCOLOR_ARGB(64,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+157, m_WindowPosition.x+194, m_WindowPosition.y+206 , 0.0f, _XSC_DEFAULT_BLACK );

	// description group3
	_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+209, m_WindowPosition.x+194, m_WindowPosition.y+255 , D3DCOLOR_ARGB(64,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+208, m_WindowPosition.x+194, m_WindowPosition.y+255 , 0.0f, _XSC_DEFAULT_BLACK );

	// description group4
	_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+259, m_WindowPosition.x+194, m_WindowPosition.y+320 , D3DCOLOR_ARGB(64,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+258, m_WindowPosition.x+194, m_WindowPosition.y+320 , 0.0f, _XSC_DEFAULT_BLACK );

	// 선택된 무공 Description
	int learnedindex = g_pLocalUser->GetLearnedSkillIndex(m_SkillID);
	if(m_SkillID > 0)
	{
		switch(g_SkillProperty[m_SkillID]->clanType)
		{			
		case _XGROUP_NANGIN :
			if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
				m_SkillIcon_NangIn_g->DrawWithRegion(m_WindowPosition.x+5, m_WindowPosition.y+5, m_IconRect);
			else
				m_SkillIcon_NangIn->DrawWithRegion(m_WindowPosition.x+5, m_WindowPosition.y+5, m_IconRect);
			break;		
		case _XGROUP_GAEBANG :
			if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
				m_SkillIcon_GaeBang_g->DrawWithRegion(m_WindowPosition.x+5, m_WindowPosition.y+5, m_IconRect);
			else
				m_SkillIcon_GaeBang->DrawWithRegion(m_WindowPosition.x+5, m_WindowPosition.y+5, m_IconRect);
			break;
		case _XGROUP_BEEGOONG :
			if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
				m_SkillIcon_BeeGoong_g->DrawWithRegion(m_WindowPosition.x+5, m_WindowPosition.y+5, m_IconRect);
			else
				m_SkillIcon_BeeGoong->DrawWithRegion(m_WindowPosition.x+5, m_WindowPosition.y+5, m_IconRect);
			break;
		case _XGROUP_SORIM :
			if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
				m_SkillIcon_SoRim_g->DrawWithRegion(m_WindowPosition.x+5, m_WindowPosition.y+5, m_IconRect);
			else
				m_SkillIcon_SoRim->DrawWithRegion(m_WindowPosition.x+5, m_WindowPosition.y+5, m_IconRect);
			break;
		case _XGROUP_NOCKRIM :
			if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
				m_SkillIcon_NockRim_g->DrawWithRegion(m_WindowPosition.x+5, m_WindowPosition.y+5, m_IconRect);
			else
				m_SkillIcon_NockRim->DrawWithRegion(m_WindowPosition.x+5, m_WindowPosition.y+5, m_IconRect);
			break;
		case _XGROUP_MOODANG :
		case _XGROUP_MAKYO :
		case _XGROUP_SEGA :
		case _XGROUP_NOFIXED :
		default :
			break;
		}
	}
		
	DrawSkillInfo();
}

BOOL _XWindow_SkillTooltip::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	
	MouseState* mousestate = gpInput->GetMouseState();
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
				if(CheckMousePosition())
					this->ShowWindow(FALSE);
		}
	}
	
	if(!this->m_ShowWindow)
		return FALSE;
	
	int skillindex = g_pLocalUser->GetLearnedSkillIndex(m_SkillID);
	// 2004.05.29->oneway48 insert
	FLOAT exp = 0.0f;
	if(g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel >= 11)
	{
		exp = (FLOAT)g_SkillProperty[m_SkillID]->detailInfo[g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel].skillExp;
	}
	else
	{
		exp = (FLOAT)g_SkillProperty[m_SkillID]->detailInfo[g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel+1].skillExp;
	}
	FLOAT gaugefactor = ((FLOAT)g_pLocalUser->m_SkillList[skillindex].m_iSkillExp / (FLOAT)exp) * 100;
	m_pNeedSkillExpGuage->SetGaugeFactor( gaugefactor );
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_SkillTooltip::SetWindowPosition(void)
{
	POINT			position;
	POINT			parentposition = m_ParentWindow->GetWindowPos();
	SIZE			parentsize = m_ParentWindow->GetWindowSize();
	
	position.x = parentposition.x + parentsize.cx + 6;
	position.y = parentposition.y + 3;
	MoveWindow(position.x , position.y);
	
	CheckOutside();
}


void _XWindow_SkillTooltip::SetSkillID(short skillid)
{
	if(m_SkillID != skillid)
	{
		m_SkillID = skillid;
		GenerateIconRect();
	}
}

void _XWindow_SkillTooltip::CheckOutside(void)
{
	BOOL		reposition = FALSE;
	POINT		position;
	POINT		parentposition = m_ParentWindow->GetWindowPos();
	SIZE		parentsize = m_ParentWindow->GetWindowSize();
	
	if(m_WindowPosition.x < 0)
	{
		// 왼쪽에서 짤림
		position.x = parentposition.x + parentsize.cx + 6;
		position.y = parentposition.y + 3;
		reposition = TRUE;
	}
	if(m_WindowPosition.x + m_WindowSize.cx >= gnWidth)
	{
		// 오른쪽에서 짤림
		position.x = parentposition.x - m_WindowSize.cx - 6;
		position.y = parentposition.y + 3;
		reposition = TRUE;
	}
	
	if(reposition)
	{
		MoveWindow(position.x, position.y);
	}
}

void _XWindow_SkillTooltip::GenerateIconRect(void)
{
	short skillid = m_SkillID - (g_SkillProperty[m_SkillID]->clanType*100);
		
	if(m_SkillID > 0)
	{
		m_IconRect.left		= ((skillid - 1)%8) * 32;
		m_IconRect.right	= ((skillid - 1)%8 + 1) * 32;
		m_IconRect.top		= ((skillid - 1)/8) * 32;
		m_IconRect.bottom	= ((skillid - 1)/8 + 1) * 32;
	}
}

void _XWindow_SkillTooltip::ShowWindow(BOOL show)
{
	if(show)
		m_pMyManager->SetTopWindow(this);
	
	_XWindow::ShowWindow(show);
}

void _XWindow_SkillTooltip::DrawSkillInfo(void)
{			
	// 현재 연공 수위
	//	TCHAR			tempbuffer[256];
	//	sprintf(tempbuffer, "%d %s", g_pLocalUser->m_CharacterInfo.무공레벨, m_LevelString);
	//g_X3DNormalFont.Puts(m_WindowPosition.x + 198, m_WindowPosition.y + 21, m_LevelString, COLOR_GRAY, 
	//_XDEF_TEXTDEFAULT_RIGHTALIGN, 22);
	
	// 무공 설명
	//g_XBaseFont->SetColor( _XSC_DEFAULT );
	//g_XBaseFont->Puts_Separate( m_WindowPosition.x + 12, m_WindowPosition.y + 44, g_SkillProperty[m_SkillID].concept, 180 );
	
	// 자기 자신의 내공 단계 2004.05.22->oneway48 delete
//	g_XBaseFont->SetColor( _XSC_INFORMATION );
//	g_XBaseFont->Puts(m_WindowPosition.x + 12, m_WindowPosition.y + 44, _T("현재 공") );
//	g_XBaseFont->Puts(m_WindowPosition.x + 12, m_WindowPosition.y + 58, _T("필요한 공") );
	g_XBaseFont->SetColor( _XSC_DEFAULT );
	
	int skillindex = g_pLocalUser->GetLearnedSkillIndex(m_SkillID);
	
//	g_XBaseFont->Print(m_WindowPosition.x + 120, m_WindowPosition.y + 10, 1.0f, "%d%s" ,  // 2004.05.22->oneway48 modify
//		g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel,_T("성") );

	// 문파 분류
	TCHAR		groupstring[16];
	memset(groupstring, 0, sizeof(TCHAR) * 16);
	switch(g_SkillProperty[m_SkillID]->clanType)
	{
	case _XGROUP_NANGIN :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00), 15);
		}
		break;
	case _XGROUP_GAEBANG :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01), 15);
		}
		break;
	case _XGROUP_BEEGOONG :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02), 15);
		}
		break;
	case _XGROUP_SORIM :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03), 15);
		}
		break;
	case _XGROUP_NOCKRIM :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04), 15);
		}
		break;
	case _XGROUP_MOODANG :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05), 15);
		}
		break;
	case _XGROUP_MAKYO :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06), 15);
		}
		break;
	case _XGROUP_SEGA :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_07), 15);
		}
		break;
	case _XGROUP_NOFIXED :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_08), 15);
		}
		break;
	}
	g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 84, groupstring);

	// 공격 형태
	TCHAR	stypestring[16];
	memset(stypestring, 0, sizeof(TCHAR) * 16);
	switch(g_SkillProperty[m_SkillID]->skillSType)
	{
	case _XSI_STYPE_FIST :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FIST), 15);
		}
		break;
	case _XSI_STYPE_WEAPON :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_WEAPON), 15);
		}
		break;
	case _XSI_STYPE_HIDDEN :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIDDEN), 15);
		}
		break;
	case _XSI_STYPE_GROUP :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_GROUP), 15);
		}
		break;
	case _XSI_STYPE_FORCE :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FORCE), 15);
		}
		break;
	case _XSI_STYPE_MAGIC :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_MAGIC), 15);
		}
		break;
	case _XSI_STYPE_MEDITATION :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_MEDITATION), 15);
		}
		break;
	case _XSI_STYPE_SPEED :
		{
			strncpy(groupstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SPEED), 15);
		}
		break;
	}
	g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 98, stypestring);

	// 사용 무기
	TCHAR		useweaponstring[16];
	memset(useweaponstring, 0, sizeof(TCHAR) * 16);
	switch(g_SkillProperty[m_SkillID]->weaponCType)
	{
	case _XGI_SC_WEAPON_SHORT :
		{
			if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_SWORD)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SWORD), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_KNIFE)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_KNIFE), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_CLUB)
				strncpy(useweaponstring, "타구봉", 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_HIT)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HIT), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_FIST)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FIST), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_SECRET)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SECRETWEAPON), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_AXE)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_AXE), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_RING)
				strncpy(useweaponstring, "륜", 15);
		}
		break;
	case _XGI_SC_WEAPON_LONG :
		{
			if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_GLAIVE)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_GLAIVE), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_SPEAR)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SPEAR), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_STAFF)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_STAFF), 15);
		}
		break;
	case _XGI_SC_WEAPON_SOFT :
		{
			if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_STEELWHIP)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FLAIL), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_HAMMER)
				strncpy(useweaponstring, "추술", 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_CHAIN)
				strncpy(useweaponstring, "사술", 15);
		}
		break;
	case _XGI_SC_WEAPON_HIDDEN :
		{
			if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_DART)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_DART), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_DAGGER)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_DAGGER), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_JAVELIN)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_PROJECTILE), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_NEEDLE)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_NEEDLE), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_ARROWLAUNCHER)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ALLOWLAUCHER), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_MINE)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_BOMB), 15);
		}
		break;
	case _XGI_SC_WEAPON_MUSICAL :
		{
			if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_RYUNG)
				strncpy(useweaponstring, "령", 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_GO)
				strncpy(useweaponstring, "고", 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_SO)
				strncpy(useweaponstring, "소", 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_KUEM)
				strncpy(useweaponstring, "금", 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_JUCK)
				strncpy(useweaponstring, "적", 15);
		}
	case _XGI_SC_WEAPON_SPECIAL :
		{
			if(g_SkillProperty[m_SkillID]->weaponSType == _XGI_TC_WEAPON_SERING)
				strncpy(useweaponstring, "륜", 15);
		}
	case -1 :
		{
			if(g_SkillProperty[m_SkillID]->weaponSType == 0)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ALLWEAPON), 15);
			else if(g_SkillProperty[m_SkillID]->weaponSType == -1)
				strncpy(useweaponstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_NOWEAPON), 15);
		}
		break;
	}
	g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 112, useweaponstring);

	// 적용 대상
	TCHAR		targetstring[16];
	memset(targetstring, 0, sizeof(TCHAR) * 16);
	switch(g_SkillProperty[m_SkillID]->targetType)
	{
	case _XSI_TARGETTYPE_SELF :
		{
			strncpy(targetstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SELF), 15);
		}
		break;
	case _XSI_TARGETTYPE_OTHER :
		{
			strncpy(targetstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_OTHER), 15);
		}
		break;
	case _XSI_TARGETTYPE_ALL :
		{
			strncpy(targetstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ALL), 15);
		}
		break;
	}
	g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 126, targetstring);

	// 기술 종류
	switch(g_SkillProperty[m_SkillID]->usefulType)
	{
	case _XSI_USEFULTYPE_PASSIVESKILL :
		{
			g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 140, "Passive skill");
		}
		break;
	case _XSI_USEFULTYPE_ACTIVESKILL :
		{
			g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 140, "Active skill");
		}
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		{
			g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 140, "Charge skill");
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		{
			g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 140, "Finish skill");
		}
		break;
	case _XSI_USEFULTYPE_PASSIVESPELL :
		{
			g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 140, "Passive spell");
		}
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		{
			g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 140, "Rounding spell");
		}
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 140, "Casting spell");
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{
			g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 140, "Channeling spell");
		}
		break;
	}

	// 오행 속성
	TCHAR		ferastring[16];
	memset(ferastring, 0, sizeof(TCHAR) * 16);
	switch(g_SkillProperty[m_SkillID]->FERA)
	{
	case _XSI_FERA_MU :
		{
			strncpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_MU), 15);
		}
		break;
	case _XSI_FERA_MOK :
		{
			strncpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_MOK), 15);
		}
		break;
	case _XSI_FERA_HWA :
		{
			strncpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HWA), 15);
		}
		break;
	case _XSI_FERA_TO :
		{
			strncpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_TO), 15);
		}
		break;
	case _XSI_FERA_KUM :
		{
			strncpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_KUM), 15);
		}
		break;
	case _XSI_FERA_SU :
		{
			strncpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SU), 15);
		}
		break;
	case _XSI_FERA_JI :
		{
			strncpy(ferastring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_JI), 15);
		}
		break;
	default:
		{
			strncpy(ferastring,  _T("알 수 없음"), 15);
		}
		break;
	}
	g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 162, ferastring);

	// 연속 계열
	TCHAR		combinationstring[16];
	memset(combinationstring, 0, sizeof(TCHAR) * 16);
	switch(g_SkillProperty[m_SkillID]->combination)
	{
	case _XSI_COMBINATION_MU :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_MU), 15);
		}
		break;
	case _XSI_COMBINATION_KUA :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_KUA), 15);
		}
		break;
	case _XSI_COMBINATION_JUNG :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_JUNG), 15);
		}
		break;
	case _XSI_COMBINATION_GANG :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_GANG), 15);
		}
		break;
	case _XSI_COMBINATION_YU :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_YU), 15);
		}
		break;
	case _XSI_COMBINATION_JOONG :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_JOONG), 15);
		}
		break;
	case _XSI_COMBINATION_GYUNG :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_GYUNG), 15);
		}
		break;
	case _XSI_COMBINATION_HU :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_HU), 15);
		}
		break;
	case _XSI_COMBINATION_SIL :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_SIL), 15);
		}
		break;
	case _XSI_COMBINATION_PAE :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_PAE), 15);
		}
		break;
	case _XSI_COMBINATION_GAE :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_GAE), 15);
		}
		break;
	case _XSI_COMBINATION_JI :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_JI), 15);
		}
		break;
	case _XSI_COMBINATION_CHON :
		{
			strncpy(combinationstring, _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_CHON), 15);
		}
		break;
	default:
		{
			strncpy(combinationstring,  _T("알 수 없음"), 15);
		}
		break;
	}
	g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 176, combinationstring);

	// 내력 속성
	g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 190, _T("알 수 없음"));
	
	// 공격력
	int skilllevel = g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel;
	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 213,  1.0f, "%d - %d",
		g_SkillProperty[m_SkillID]->detailInfo[skilllevel].minAttackDamage, g_SkillProperty[m_SkillID]->detailInfo[skilllevel].maxAttackDamage);
	
	// 공격 성공치
	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 227,  1.0f, "%d", g_SkillProperty[m_SkillID]->detailInfo[skilllevel].attackRating);
	
	// 회피 성공치
	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 241,  1.0f, "%d", g_SkillProperty[m_SkillID]->detailInfo[skilllevel].avoidRating);
	
	// 내력 소모량
	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 263,  1.0f, "%d",g_SkillProperty[m_SkillID]->requirementEnergy);
	
	// 심력 소모량
	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 277,  1.0f, "%d",g_SkillProperty[m_SkillID]->requirementDivine);
	// 영력 소모량

	// 정력 소모량

	// 필요 공치
	// 2004.06.19->oneway48 insert
	int exp = 0 ;
	if(g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel >= 11)
	{
		exp = g_SkillProperty[m_SkillID]->detailInfo[g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel].techniqueExp;
	}
	else
	{
		exp = g_SkillProperty[m_SkillID]->detailInfo[g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel+1].techniqueExp;
	}
	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 305, 1.0f, "%d" , exp );

	// 수련 요구

	// 수련

	// 추가 경험치

	// 회수

	// 부가 효과 1

	// 부가 효과 2

	// 부가 효과 3

	// 부가 효과 4

	g_XBaseFont->SetColor(_XSC_INFORMATION);

	// 무공 이름
 // 2004.05.22->oneway48 modify
	g_XBaseFont->Print(m_WindowPosition.x + 45, m_WindowPosition.y + 10,  1.0f, "%s %d%s",
		g_SkillProperty[m_SkillID]->skillName , g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel+1, _T("성") );

	// 문파 분류
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 84, m_ClanTypeString);
	// 공격 형태
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 98, m_SkillSTypeString);
	// 사용 무기
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 112, m_WeaponCTypeString);
	// 적용 대상
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 126, m_TargetTypeString);
	// 기술 종류
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 140, m_UsefulTypeString);
	// 오행 속성
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 162, m_FERAString);
	// 연속 계열
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 176, m_CombinationString);
	// 내력 속성
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 190, m_InsideTypeString);
	// 공격력
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 213, m_AttackDamageString);
	// 공격 성공치
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 227, m_AttackRatingString);
	// 회피 성공치
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 241, m_AvoidRatingString);
	// 내력 소모량
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 263, _T("내공 소모량"));
	// 심력 소모량
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 277, _T("의지 소모량"));
	// 영력 소모량
//	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 291, m_CostManaString);
	// 정력 소모량
//	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 305, m_CostEnergyString);
	// 다음단계에 필요한 공
	g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 305, m_TechniqueExpString); //323

	g_XBaseFont->Flush();
}
*/