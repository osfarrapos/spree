// XWindow_Exp.cpp: implementation of the _XWindow_Exp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XParticleIDDefine.h"
#include "XWindowObjectDefine.h"
#include "XWindow_Exp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Exp* g_pExp_Window = NULL;

void __stdcall _XExpWindowCallback_FuryGauge( POINT& winpos, SIZE& winsize, FLOAT& factor )
{
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	g_XSmallFont->SetColor( _XSC_DEFAULT );
	g_XSmallFont->PrintAlign( winpos.x + (winsize.cx>>1), winpos.y-1, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%3.2f %%",  factor );
	g_XSmallFont->Flush();
#endif
}

void __stdcall _XExpWindowCallback_ExpGauge(POINT& winpos, SIZE& winsize, FLOAT& factor)
{
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	g_XSmallFont->SetColor( _XSC_DEFAULT );
	g_XSmallFont->PrintAlign( winpos.x + (winsize.cx>>1)-22, winpos.y+3, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%3.2f %%",  factor );
	g_XSmallFont->Flush();
#endif
}

_XWindow_Exp::_XWindow_Exp() : m_pConvergenceAnimation(NULL)
{
	m_pBtnConvergence = NULL;
	m_pFuryGauge = NULL;
	m_pExpGauge = NULL;

	m_dwConvergenceStartTime = 0;
	m_bSendConvergencePacket = FALSE;
}

_XWindow_Exp::~_XWindow_Exp()
{

}

BOOL _XWindow_Exp::Initialize()
{
	g_pExp_Window = this;

	int mi_char_resource_01 = g_MainInterfaceTextureArchive.FindResource("mi_char_01.tga");

	_XImageStatic* pLBorder = new _XImageStatic;
	pLBorder->Create(0, 0, 256, 47, &g_MainInterfaceTextureArchive, mi_char_resource_01);
	pLBorder->SetClipRect(0, 73, 256, 120);
	InsertChildObject(pLBorder);

	_XImageStatic* pRBorder = new _XImageStatic;
	pRBorder->Create(256, 0, 256+127, 47, &g_MainInterfaceTextureArchive, mi_char_resource_01);
	pRBorder->SetClipRect(24, 122, 151, 169);
	InsertChildObject(pRBorder);

	// stick gauge
	_XSTICKGAUGE_STRUCTURE	gaugestruct = 
	{
		TRUE, { 46, 12 }, { 265, 6 },	
		mi_char_resource_01, -1, &g_MainInterfaceTextureArchive, 
	};
	
	m_pFuryGauge = new _XStickGauge;
	m_pFuryGauge->Create( gaugestruct );	
	m_pFuryGauge->SetImageClipRect(_XGAUGEIMAGE_BORDER, 1,  122, 11, 128);
	m_pFuryGauge->SetGaugeImageScaleFactor(265.f/10.f);
	m_pFuryGauge->SetCallBackFunction_FactorDraw( _XExpWindowCallback_FuryGauge );
	m_pFuryGauge->SetFactorDraw(TRUE);
	m_pFuryGauge->SetGaugeImageDrawType(_XGAUGEIMAGEDRAWTYPE_SCALE);
	InsertChildObject( m_pFuryGauge );

	gaugestruct.position.x = 67;
	gaugestruct.position.y = 29;
	gaugestruct.windowsize.cx = 282;
	gaugestruct.windowsize.cy = 12;

	m_pExpGauge = new _XStickGauge;
	m_pExpGauge->Create( gaugestruct );	
	m_pExpGauge->SetImageClipRect(_XGAUGEIMAGE_BORDER, 1,  129, 11, 141);
	m_pExpGauge->SetGaugeImageScaleFactor(282.f/10.f);
	m_pExpGauge->SetCallBackFunction_FactorDraw( _XExpWindowCallback_ExpGauge );
	m_pExpGauge->SetFactorDraw(TRUE);
	m_pExpGauge->SetGaugeImageDrawType(_XGAUGEIMAGEDRAWTYPE_SCALE);
	InsertChildObject( m_pExpGauge );


	_XImageStatic* pLSBorder = new _XImageStatic;
	pLSBorder->Create(0, 12, 113, 12+29, &g_MainInterfaceTextureArchive, mi_char_resource_01);
	pLSBorder->SetClipRect(0, 170, 113, 199);
	InsertChildObject(pLSBorder);

	_XImageStatic* pRSBorder = new _XImageStatic;
	pRSBorder->Create(312, 12, 312+71, 12+29, &g_MainInterfaceTextureArchive, mi_char_resource_01);
	pRSBorder->SetClipRect(0, 201, 71, 230);
	InsertChildObject(pRSBorder);

	// button		
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, 
		{320, 4},
		{23, 23},
		_XDEF_EXPWINDOW_CONVERGENCE,
		mi_char_resource_01,
		mi_char_resource_01,
		mi_char_resource_01,
		&g_MainInterfaceTextureArchive
	};
	m_pBtnConvergence = new _XButton;
	m_pBtnConvergence->Create(btnstruct);
	m_pBtnConvergence->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 199, 159, 222, 182);
	m_pBtnConvergence->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK,  199, 159, 222, 182);
	m_pBtnConvergence->SetButtonImageClipRect(_XBUTTONIMAGE_READY,  199, 159, 222, 182);
	m_pBtnConvergence->ShowWindow(TRUE);
	InsertChildObject(m_pBtnConvergence);

	m_pConvergenceAnimation = new _XTextureAnimation( 0, 0, 32, 32, this, &g_MainInterfaceTextureArchive, 
													 g_MainInterfaceTextureArchive.FindResource("mi_char_03.tga"),
													 4, 4, 16 );
	m_pConvergenceAnimation->ShowWindow(FALSE);
	m_pConvergenceAnimation->SetDrawPostion(m_WindowPosition.x+315, m_WindowPosition.y-1);

	return TRUE;
}

void _XWindow_Exp::DestroyWindow()
{
	_XWindow::DestroyWindow();

	if(m_pConvergenceAnimation)
	{
		delete m_pConvergenceAnimation;
		m_pConvergenceAnimation = NULL;
	}
}

void _XWindow_Exp::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_FURY)
	{
		// 분노 버튼 animation
		m_pConvergenceAnimation->Update(0.1f);
	}

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();	
	
	if( ((scrnpos->x > m_WindowPosition.x+36) ) && (scrnpos->x < m_WindowPosition.x+36+272) &&
		(scrnpos->z > m_WindowPosition.y+9 ) && (scrnpos->z < m_WindowPosition.y+9+12) )
	{
		// 분노
		TCHAR messagestr[256];
		memset(messagestr, 0, sizeof(messagestr));

		sprintf(messagestr, "%s:%3.2f%%", _XGETINTERFACETEXT(ID_STRING_MAINGAMECALLBACKFUNCTIONS_1089), GetFuryGaugeFactor());
		g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+269, m_WindowPosition.y+18, messagestr, _XSC_INFORMATION, TRUE);
	}

	if( ((scrnpos->x > m_WindowPosition.x+45) ) && (scrnpos->x < m_WindowPosition.x+45+297) &&
		(scrnpos->z > m_WindowPosition.y+26 ) && (scrnpos->z < m_WindowPosition.y+26+17) )
	{
		// 경험치
		TCHAR messagestr[256];
		memset(messagestr, 0, sizeof(messagestr));

		sprintf(messagestr, "%s:%3.2f%%", _XGETINTERFACETEXT(ID_STRING_NEW_3632), GetExpGaugeFactor());
		g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+269, m_WindowPosition.y+18, messagestr, _XSC_INFORMATION, TRUE);
	}
}

BOOL _XWindow_Exp::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	// TODO
	ProcessConvergence();

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_Exp::SetParameter()
{
	FLOAT gaugefactor = 0;

	if(g_UserLevelTable[ g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1 ].needjinexperience > 0)
	{
		gaugefactor = ((FLOAT)g_pLocalUser->m_CharacterInfo.Get_jinlevel() / 
							 (FLOAT)g_UserLevelTable[ g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1 ].needjinexperience) * 100;
	}
	else
	{
		gaugefactor = 0;
	}
	m_pExpGauge->SetGaugeFactor( gaugefactor );

	FLOAT furyparameter = ((FLOAT)g_pLocalUser->m_CharacterInfo.Get_furyparameter() / 10000.0f)*100.0f;
	m_pFuryGauge->SetGaugeFactor( furyparameter );
}

void _XWindow_Exp::ClearConvergence()
{
	g_pLocalUser->m_UserTroubleState = _XDEF_CONVSTATE_NORMAL;
	m_dwConvergenceStartTime = 0;
	m_bSendConvergencePacket = FALSE;
	ShowConvergenceAnimation(FALSE);

	if(g_pLocalUser->m_ModelDescriptor.m_EffectManager)
	{
		if(g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(_XDEF_PLAYERID_BERSERK))
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_PLAYERID_BERSERK);
	}
}

void _XWindow_Exp::ShowConvergenceAnimation(BOOL show)
{
	m_pConvergenceAnimation->ShowWindow(show);
}

void _XWindow_Exp::ProcessConvergence()
{
	if(g_pLocalUser->m_UserTroubleState != _XDEF_CONVSTATE_FURY && g_pLocalUser->m_UserTroubleState != _XDEF_CONVSTATE_CONVERGENCE)
		return;

	DWORD elapsedtime = g_LocalSystemTime - m_dwConvergenceStartTime;
	FLOAT furyparameter = (FLOAT)g_pLocalUser->m_CharacterInfo.Get_furyparameter();


	if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_FURY)
	{
		furyparameter -= elapsedtime * 0.05f;
		g_pLocalUser->m_CharacterInfo.Set_furyparameter( (short)furyparameter );
		
		FLOAT furyparameter = ((FLOAT)g_pLocalUser->m_CharacterInfo.Get_furyparameter() / 10000.0f)*100.0f;
		m_pFuryGauge->SetGaugeFactor( furyparameter );

		if(g_pCInfo_Window)
		{
			if(g_pCInfo_Window->GetShowStatus())
				g_pCInfo_Window->SetParameter();
		}
	}
	else if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_CONVERGENCE)
	{
		furyparameter -= elapsedtime * 0.2f;
		g_pLocalUser->m_CharacterInfo.Set_furyparameter( (short)furyparameter );
		
		FLOAT furyparameter = ((FLOAT)g_pLocalUser->m_CharacterInfo.Get_furyparameter() / 10000.0f)*100.0f;
		m_pFuryGauge->SetGaugeFactor( furyparameter );

		if(g_pCInfo_Window)
		{
			if(g_pCInfo_Window->GetShowStatus())
			   g_pCInfo_Window->SetParameter();
		}
	}

	if(g_pLocalUser->m_CharacterInfo.Get_furyparameter() <= 0)
	{	
		if(g_SinglePlayMode || g_ZeroLevelMode )	// hotblood zerolevel
		{
			ClearConvergence();
			g_pLocalUser->m_CharacterInfo.Set_furyparameter( 0 );
		}
		else if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_CONVERGENCE || g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_FURY)
		{
			if(!m_bSendConvergencePacket)
			{
				g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_normal_fury);
				m_bSendConvergencePacket = TRUE;
			}
		}
	}
	else
	{
		m_dwConvergenceStartTime = g_LocalSystemTime;
	}
}

FLOAT _XWindow_Exp::GetExpGaugeFactor()
{
	if(m_pExpGauge)
		return m_pExpGauge->GetGaugeFactor();
	else
		return 0;
}

FLOAT _XWindow_Exp::GetFuryGaugeFactor()
{
	if(m_pFuryGauge)
		return m_pFuryGauge->GetGaugeFactor();
	else
		return 0;
}

void _XWindow_Exp::ShowWindow(BOOL show)
{
	_XWindow::ShowWindow(show);
}