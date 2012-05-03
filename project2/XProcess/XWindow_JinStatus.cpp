// XWindow_JinStatus.cpp: implementation of the _XWindow_JinStatus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_JinStatus.h"
#include "XWindow_DeathBlow.h"//2004.06.23->oneway48 insert
#include "XWindow_GuideLine.h"
#include "XWindow_MainMenu.h"
#include "XSR_STRINGHEADER.H" // 2004.05.14->oneway48 insert
#include "XParticleIDDefine.h"
#include "XWindowObjectDefine.h"
#include "SoundEffectList.h"
#include "XTextureAnimation.h" //author : 양희왕 // 불꽃 애니

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

_XWindow_JinStatus* g_pJinStatus_Window = NULL;


void __stdcall _XJinStickGaugeCallback_LifePower( POINT& winpos, SIZE& winsize, FLOAT& factor )
{
	g_XSmallFont->SetColor( 0xFFE0E0E0 );

	short maxlife = 0;

	maxlife = g_pLocalUser->m_MaxLifePower.Get_result();

	if(maxlife < 0)
		maxlife = 0;

	g_XSmallFont->PrintAlign( winpos.x + (winsize.cx>>1), winpos.y+1, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%d/%d",  
		g_pLocalUser->m_CharacterInfo.Get_current_lifepower(),maxlife );
	g_XSmallFont->Flush();		
}

void __stdcall _XJinStickGaugeCallback_ForcePower( POINT& winpos, SIZE& winsize, FLOAT& factor )
{
	g_XSmallFont->SetColor( 0xFFE0E0E0 );

	short maxforce = 0;

	maxforce = g_pLocalUser->m_MaxForcePower.Get_result();

	if(maxforce < 0)
		maxforce = 0;

	g_XSmallFont->PrintAlign( winpos.x + (winsize.cx>>1), winpos.y+1, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%d/%d",  
		g_pLocalUser->m_CharacterInfo.Get_current_forcepower(),maxforce );
	g_XSmallFont->Flush();		
}

void __stdcall _XJinStickGaugeCallback_DamagedLifePower( POINT& winpos, SIZE& winsize, FLOAT& factor )
{
	g_XSmallFont->SetColor( 0xFFFFFFFF );
	g_XSmallFont->PrintAlign( winpos.x + winsize.cx - 1, winpos.y+1, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.Get_externalwoundrate() );
	g_XSmallFont->Flush();		
}

void __stdcall _XJinStickGaugeCallback_DamagedForcePower( POINT& winpos, SIZE& winsize, FLOAT& factor )
{
	g_XSmallFont->SetColor( 0xFFFFFFFF );
	g_XSmallFont->PrintAlign( winpos.x + winsize.cx - 1, winpos.y+1, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.Get_internalwoundrate() );
	g_XSmallFont->Flush();		
}

void __stdcall _XJinStickGaugeCallback_AbilityUp(  POINT pos, SIZE size  )
{	
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2154), _XSC_INFORMATION );
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

POINT g_JinRoundPosition[8] = {  { 26, 9 }, { 37,14 }, { 41,24 }, { 37,34 }, { 26,39 }, { 15,34 }, { 11,24 }, { 15,14 }	};
POINT g_AbilityPosition[8] = { {228, 0}, {228, 28}, {228, 56}, {228, 84}, {228, 112}, {228, 84}, {228, 56}, {228, 28} };

_XWindow_JinStatus::_XWindow_JinStatus() : m_pFireImage(NULL)
{
	m_SpeedCounter = 0;
	
	m_JinBorderFrame = 0.0f;
	m_JinBorderPrevFrame = 0;
	
	m_JinMaxEffectFrame = 0.0f;
	m_JinMaxEffectPrevFrame = 0;

	m_AccumulatedImageBorderFrame = 0.0f;
	m_AccumulatedImageBorderPrevFrame = 0;

	m_FireEffectFrame	= 0.0f;
	m_FireEffectPrevFrame =	0;

	m_JinGaugePercentage = 0;
	
	m_LevelUpButton = NULL;
	m_LevelUpSpeedCounter = 0;
	m_SelectedEffectFrame = 0.0f;
	m_SelectedEffectPrevFrame = 0;
	m_EffectOrderSub = FALSE;

	for(int i = 0 ; i < 24 ; i++)
		m_FuryParameterImage[i] = NULL;

	m_ConvergenceButton = NULL;

	for(i = 0 ; i < 5 ; i++)
		m_ConvergenceImage[i] = NULL;

	m_ConvPrevDrawTime = 0;
	m_ConvEffectFrame = 0.0f;
	m_ConvEffectOrderSub = false;

	m_ConvergenceStartTime = 0;
	m_bConvergenceSendPacket = false;

	for(i = 0 ; i < 6 ; i++)
		m_LevelUpImage[i] = NULL;

	m_AbilityUpButton = NULL;
	m_AbilityUpFrame	= 0.0f;
	m_CurAbilityUpCount = 0;

	m_BossRoomPoisonTimerBorder1 = NULL;
	m_BossRoomPoisonTimerBorder2 = NULL;
	m_BossRoomPoisonTimerBorder3 = NULL;
	m_BossRoomPoisonIcon		 = NULL;
	m_PehwangPoisonIcon			 = NULL;
	m_bViewBossRoomPoisonTimer	 = FALSE;
	m_bViewPehwangPoisonIcon	 = FALSE;

	m_btnSDSOnOff	= NULL;
	m_pClassIcon	= NULL;

	m_dwSDSSetTime	= 0;
	m_nSDSState		= 0;
}

_XWindow_JinStatus::~_XWindow_JinStatus()
{
	SAFE_DELETE(m_PehwangPoisonIcon);
	SAFE_DELETE(m_BossRoomPoisonIcon);
	SAFE_DELETE(m_BossRoomPoisonTimerBorder1);
	SAFE_DELETE(m_BossRoomPoisonTimerBorder2);
	SAFE_DELETE(m_BossRoomPoisonTimerBorder3);

	if(m_pFireImage)
	{
		delete m_pFireImage;
		m_pFireImage = NULL;
	}

}

BOOL _XWindow_JinStatus::Initialize( void )
{
	g_pJinStatus_Window = this;

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI

	// 레벨 업 버튼 
	int levelupresource = g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName);

	_XBTN_STRUCTURE levelupbtnstruct = 
	{
		TRUE, {(gnWidth>>1)-103, 75}, {206, 48},
		_XDEF_JINSTATUS_LEVELUPBUTTON,
		levelupresource,
		levelupresource,
		levelupresource,
		&g_MainInterfaceTextureArchive
	};
	m_LevelUpButton = new _XButton;
	m_LevelUpButton->Create(levelupbtnstruct);
	m_LevelUpButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 0, 206, 48);
	m_LevelUpButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 0, 206, 48);
	m_LevelUpButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 0, 206, 48);
	m_LevelUpButton->EnableWindow(FALSE);
	m_LevelUpButton->ShowWindow(FALSE);
	InsertChildObject(m_LevelUpButton);

	for(int i = 0 ; i < 6 ; i++)
	{
		m_LevelUpImage[i] = new _XImageStatic;
		m_LevelUpImage[i]->Create(0, 0, 155, 31, &g_MainInterfaceTextureArchive, levelupresource);
		m_LevelUpImage[i]->SetClipRect(0, (i%6)*32+53, 155, (i%6)*32+84);
	}

	// 능력치 업 버튼 ----------------------------------------------------------------------=	
	int abilityupupresource = g_MainInterfaceTextureArchive.FindResource("MI_tra_base.tga");

	_XBTN_STRUCTURE abilityupbtnstruct = 
	{
		TRUE, {25, 73}, {28, 28},
		_XDEF_JINSTATUS_ABILITYUPBUTTON,
		abilityupupresource,
		abilityupupresource,
		abilityupupresource,
		&g_MainInterfaceTextureArchive
	};

	m_AbilityUpButton = new _XButton;
	m_AbilityUpButton->Create(abilityupbtnstruct);
	m_AbilityUpButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 228, 112, 256, 140);
	m_AbilityUpButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 200, 118, 228, 146);
	m_AbilityUpButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 172, 118, 200, 146);	
	m_AbilityUpButton->SetCallBackFunction_ToolTip( _XJinStickGaugeCallback_AbilityUp );
	m_AbilityUpButton->ShowWindow(FALSE);
	InsertChildObject(m_AbilityUpButton);

	// Heath, Force gauge
	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );

	_XSTICKGAUGE_STRUCTURE	healthgaugestruct = 
	{
			TRUE, { 84, 41 }, { 167, 12 },	
			charinfointerface, -1, &g_MainInterfaceTextureArchive
	};

	m_pLifePowerDamageBorder = new _XStickGauge;
	m_pLifePowerDamageBorder->Create( healthgaugestruct );	
	m_pLifePowerDamageBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  145, 141,  157 );
	m_pLifePowerDamageBorder->SetGaugeImageScaleFactor(167.f/141.f);
	m_pLifePowerDamageBorder->SetCallBackFunction_FactorDraw( _XJinStickGaugeCallback_DamagedLifePower );
	m_pLifePowerDamageBorder->SetFactorDraw(TRUE);
	m_pLifePowerDamageBorder->SetGaugeProgressType( _XGAUGEPROGRESSTYPE_BACKWARD );
	InsertChildObject( m_pLifePowerDamageBorder );
	
	m_pLifePowerBorder = new _XStickGauge;
	m_pLifePowerBorder->Create( healthgaugestruct );	
	m_pLifePowerBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  133, 141,  145 );
	m_pLifePowerBorder->SetGaugeImageScaleFactor(167.f/141.f);
	m_pLifePowerBorder->SetCallBackFunction_FactorDraw( _XJinStickGaugeCallback_LifePower );
	m_pLifePowerBorder->SetFactorDraw(TRUE);
	m_pLifePowerBorder->SetGaugeColorFactor( D3DCOLOR_ARGB( 255, 255, 0, 0 ), D3DCOLOR_ARGB( 160, 255, 0, 0 ) );
	m_pLifePowerBorder->SetViewDelayGauge( TRUE, 20 );
	m_pLifePowerBorder->SetBlinking( TRUE, 10 );
	//m_pLifePowerBorder->ShowWindow( FALSE );
	InsertChildObject( m_pLifePowerBorder );
	
	healthgaugestruct.position.y		=	56;

	m_pForcePowerDamageBorder = new _XStickGauge;
	m_pForcePowerDamageBorder->Create( healthgaugestruct );	
	m_pForcePowerDamageBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  145, 141,  157 );
	m_pForcePowerDamageBorder->SetGaugeImageScaleFactor(167.f/141.f);
	m_pForcePowerDamageBorder->SetCallBackFunction_FactorDraw( _XJinStickGaugeCallback_DamagedForcePower );
	m_pForcePowerDamageBorder->SetFactorDraw(TRUE);
	m_pForcePowerDamageBorder->SetGaugeProgressType( _XGAUGEPROGRESSTYPE_BACKWARD );
	InsertChildObject( m_pForcePowerDamageBorder );
			
	m_pForcePowerBorder = new _XStickGauge;
	m_pForcePowerBorder->Create( healthgaugestruct );	
	m_pForcePowerBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  133, 141,  145 );
	m_pForcePowerBorder->SetGaugeImageScaleFactor(167.f/141.f);
	m_pForcePowerBorder->SetCallBackFunction_FactorDraw( _XJinStickGaugeCallback_ForcePower );
	m_pForcePowerBorder->SetFactorDraw(TRUE);
	m_pForcePowerBorder->SetGaugeColorFactor( D3DCOLOR_ARGB( 255, 0, 0, 255 ), D3DCOLOR_ARGB( 160, 0, 0, 255 )  );
	m_pForcePowerBorder->SetViewDelayGauge( TRUE, 20 );
	//m_pForcePowerBorder->ShowWindow( FALSE );
	InsertChildObject( m_pForcePowerBorder );

	#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI

		// sds on/off 버튼
		_XBTN_STRUCTURE sdsbtnstruct = 
		{
			TRUE,
			{53, 83},
			{28, 28},
			_XDEF_JINSTATUS_SHUTDOWNONOFFBUTTON,
			g_MainInterfaceTextureArchive.FindResource("mi_main_unki_icon.tga"),
			g_MainInterfaceTextureArchive.FindResource("mi_main_unki_icon.tga"),
			g_MainInterfaceTextureArchive.FindResource("mi_main_unki_icon.tga"),
			&g_MainInterfaceTextureArchive
		};
		
		m_btnSDSOnOff = new _XButton;
		m_btnSDSOnOff->Create(sdsbtnstruct);
		m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 227, 65, 255, 93);
		m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 227, 65, 255, 93);
		m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 227, 65, 255, 93);
		m_btnSDSOnOff->ShowWindow(FALSE);
		InsertChildObject(m_btnSDSOnOff);

	#endif

	m_pClassIcon = new _XImageStatic;
	m_pClassIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("skill_icon_08_02.tga"));
	m_pClassIcon->SetClipRect(2, 2, 32, 32);

	//Author : 양희왕 //breif : 불꽃 애니
	m_pFireImage = new _XTextureAnimation( 0, 0, 30, 47, this, &g_MainInterfaceTextureArchive,
										   g_MainInterfaceTextureArchive.FindResource("MI_total_s.tga"),
										   4, 4, 16 );
	m_pFireImage->SetDrawPostion( m_WindowPosition.x + 54, m_WindowPosition.y + 22 );
	m_pFireImage->ShowWindow(TRUE);
	
#else
	m_borderstatic.SetClipRect( 0, 0, 229, 82 );
	
	int characterresourceindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );

	int chargeboximageindex = g_MainInterfaceTextureArchive.FindResource( "chargebox_gauge.tga" );

	m_JinBorderImage.Create( m_WindowPosition.x + 22, 
							 m_WindowPosition.y + 22, 
							 m_WindowPosition.x + 54, 
							 m_WindowPosition.y + 54,
							&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_jinborder.tga" ) );	
	m_JinBorderImage.SetClipRect( 0, 0, 32, 32  );

	m_JinGaugeImage.Create( m_WindowPosition.x + 22, 
							 m_WindowPosition.y + 22, 
							 m_WindowPosition.x + 54, 
							 m_WindowPosition.y + 54,
							 &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_jingauge.tga" ) );	
	m_JinGaugeImage.SetClipRect( 0, 0, 32, 32  );

	m_JinMaxImage.Create( m_WindowPosition.x + 13, 
							 m_WindowPosition.y + 12, 
							 m_WindowPosition.x + 13 + 52, 
							 m_WindowPosition.y + 12 + 52,
							 &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_JinMax.tga" ) );	
	m_JinMaxImage.SetClipRect( 0, 0, 52, 52  );

	m_LeftFireImage.Create( m_WindowPosition.x - 6, 
							 m_WindowPosition.y + 23, 
							 m_WindowPosition.x + 24, 
							 m_WindowPosition.y + 70,
							 &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_total_s.tga" ) );	
	m_LeftFireImage.SetClipRect( 0, 0, 30, 47  );

	m_RightFireImage.Create( m_WindowPosition.x + 54, 
							 m_WindowPosition.y + 23, 
							 m_WindowPosition.x + 84, 
							 m_WindowPosition.y + 70,
							 &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_total_s.tga" ) );	
	m_RightFireImage.SetClipRect( 0, 0, 30, 47  );


	// Create accumulated jin round indicator...
	
	for( int i = 0; i < 8; i++ )
	{
		m_AccumulatedJinImage[i].Create( m_WindowPosition.x + g_JinRoundPosition[i].x, 
										 m_WindowPosition.y + g_JinRoundPosition[i].y, 
										 m_WindowPosition.x + g_JinRoundPosition[i].x+4, 
										 m_WindowPosition.y + g_JinRoundPosition[i].y+4,
										&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_JinMax.tga" ) );	
		m_AccumulatedJinImage[i].SetClipRect( 204, 204, 209, 209 );
	}


	int levelupresource = g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName);

	_XBTN_STRUCTURE levelupbtnstruct = 
	{
		TRUE, {(gnWidth>>1)-103, 75}, {206, 48},
		_XDEF_JINSTATUS_LEVELUPBUTTON,
		levelupresource,
		levelupresource,
		levelupresource,
		&g_MainInterfaceTextureArchive
	};
	m_LevelUpButton = new _XButton;
	m_LevelUpButton->Create(levelupbtnstruct);
	m_LevelUpButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 0, 206, 48);
	m_LevelUpButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 0, 206, 48);
	m_LevelUpButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 0, 206, 48);
	m_LevelUpButton->EnableWindow(FALSE);
	m_LevelUpButton->ShowWindow(FALSE);
	InsertChildObject(m_LevelUpButton);

	for(i = 0 ; i < 6 ; i++)
	{
		m_LevelUpImage[i] = new _XImageStatic;
		m_LevelUpImage[i]->Create(0, 0, 155, 31, &g_MainInterfaceTextureArchive, levelupresource);
		m_LevelUpImage[i]->SetClipRect(0, (i%6)*32+53, 155, (i%6)*32+84);
	}

	// 능력치 업 버튼 ----------------------------------------------------------------------=	
	int abilityupupresource = g_MainInterfaceTextureArchive.FindResource("MI_tra_base.tga");

	_XBTN_STRUCTURE abilityupbtnstruct = 
	{
		TRUE, {25, 83}, {28, 28},
		_XDEF_JINSTATUS_ABILITYUPBUTTON,
		abilityupupresource,
		abilityupupresource,
		abilityupupresource,
		&g_MainInterfaceTextureArchive
	};

	m_AbilityUpButton = new _XButton;
	m_AbilityUpButton->Create(abilityupbtnstruct);
	m_AbilityUpButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 228, 112, 256, 140);
	m_AbilityUpButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 200, 118, 228, 146);
	m_AbilityUpButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 172, 118, 200, 146);	
	m_AbilityUpButton->SetCallBackFunction_ToolTip( _XJinStickGaugeCallback_AbilityUp );
	m_AbilityUpButton->ShowWindow(FALSE);
	InsertChildObject(m_AbilityUpButton);
	// -------------------------------------------------------------------------------------=

	for(i = 0 ; i < 24 ; i++)
	{
		m_FuryParameterImage[i] = new _XImageStatic;
		m_FuryParameterImage[i]->Create(0, 0, 8, 8, &g_MainInterfaceTextureArchive, charinfointerface);
		m_FuryParameterImage[i]->SetClipRect(193+(i%5)*8, 123+(i/5)*8, 193+(i%5)*8+8, 123+(i/5)*8+8);
	}

	// position
	m_FuryParameterImage[0]->MoveWindow(m_WindowPosition.x+18+20, m_WindowPosition.y+17);
	m_FuryParameterImage[1]->MoveWindow(m_WindowPosition.x+18+26, m_WindowPosition.y+17);
	m_FuryParameterImage[2]->MoveWindow(m_WindowPosition.x+18+29, m_WindowPosition.y+17+3);
	m_FuryParameterImage[3]->MoveWindow(m_WindowPosition.x+18+32, m_WindowPosition.y+17+6);
	m_FuryParameterImage[4]->MoveWindow(m_WindowPosition.x+18+35, m_WindowPosition.y+17+9);
	m_FuryParameterImage[5]->MoveWindow(m_WindowPosition.x+18+37, m_WindowPosition.y+17+14);
	m_FuryParameterImage[6]->MoveWindow(m_WindowPosition.x+18+37, m_WindowPosition.y+17+22);
	m_FuryParameterImage[7]->MoveWindow(m_WindowPosition.x+18+35, m_WindowPosition.y+17+27);
	m_FuryParameterImage[8]->MoveWindow(m_WindowPosition.x+18+32, m_WindowPosition.y+17+29);
	m_FuryParameterImage[9]->MoveWindow(m_WindowPosition.x+18+30, m_WindowPosition.y+17+34);
	m_FuryParameterImage[10]->MoveWindow(m_WindowPosition.x+18+25, m_WindowPosition.y+17+36);
	m_FuryParameterImage[11]->MoveWindow(m_WindowPosition.x+18+21, m_WindowPosition.y+17+38);
	m_FuryParameterImage[12]->MoveWindow(m_WindowPosition.x+18+15, m_WindowPosition.y+17+38);
	m_FuryParameterImage[13]->MoveWindow(m_WindowPosition.x+18+10, m_WindowPosition.y+17+36);
	m_FuryParameterImage[14]->MoveWindow(m_WindowPosition.x+18+5, m_WindowPosition.y+17+34);
	m_FuryParameterImage[15]->MoveWindow(m_WindowPosition.x+18+2, m_WindowPosition.y+17+31);
	m_FuryParameterImage[16]->MoveWindow(m_WindowPosition.x+18, m_WindowPosition.y+17+27);
	m_FuryParameterImage[17]->MoveWindow(m_WindowPosition.x+18, m_WindowPosition.y+17+22);
	m_FuryParameterImage[18]->MoveWindow(m_WindowPosition.x+18, m_WindowPosition.y+17+15);
	m_FuryParameterImage[19]->MoveWindow(m_WindowPosition.x+18, m_WindowPosition.y+17+9);
	m_FuryParameterImage[20]->MoveWindow(m_WindowPosition.x+18+2, m_WindowPosition.y+17+5);
	m_FuryParameterImage[21]->MoveWindow(m_WindowPosition.x+18+5, m_WindowPosition.y+17+2);
	m_FuryParameterImage[22]->MoveWindow(m_WindowPosition.x+18+9, m_WindowPosition.y+17);
	m_FuryParameterImage[23]->MoveWindow(m_WindowPosition.x+18+12, m_WindowPosition.y+17);

 
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, 
		{20, 20},
		{37, 37},
		_XDEF_JINSTATUS_CONVERGENCEBUTTON,
		charinfointerface,
		charinfointerface,
		charinfointerface,
		&g_MainInterfaceTextureArchive
	};
	m_ConvergenceButton = new _XButton;
	m_ConvergenceButton->Create(btnstruct);
	m_ConvergenceButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 46, 82, 83, 119);
	m_ConvergenceButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 46, 82, 83, 119);
	m_ConvergenceButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 46, 82, 83, 119);
	m_ConvergenceButton->ShowWindow(FALSE);
	InsertChildObject(m_ConvergenceButton);

	for(i = 0 ; i < 5 ; i++)
	{
		m_ConvergenceImage[i] = new _XImageStatic;
		m_ConvergenceImage[i]->Create(0, 0, 37, 37, &g_MainInterfaceTextureArchive, charinfointerface);
	}
	m_ConvergenceImage[0]->SetClipRect(46, 82, 83, 119);
	m_ConvergenceImage[1]->SetClipRect(83, 82, 119, 119);
	m_ConvergenceImage[2]->SetClipRect(120, 82, 156, 119);
	m_ConvergenceImage[3]->SetClipRect(157, 82, 193, 119);
	m_ConvergenceImage[4]->SetClipRect(194, 82, 230, 119);

	int gaugeimageindex = g_MainInterfaceTextureArchive.FindResource( "gaugebar_common.tga" );
	
	_XSTICKGAUGE_STRUCTURE	healthgaugestruct = 
	{
			TRUE, { 83, 41 }, { 141, 12 },	
			charinfointerface, -1, &g_MainInterfaceTextureArchive
	};

	m_pLifePowerDamageBorder = new _XStickGauge;
	m_pLifePowerDamageBorder->Create( healthgaugestruct );	
	m_pLifePowerDamageBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  145, 141,  157 );
	m_pLifePowerDamageBorder->SetCallBackFunction_FactorDraw( _XJinStickGaugeCallback_DamagedLifePower );
	m_pLifePowerDamageBorder->SetFactorDraw(TRUE);
	m_pLifePowerDamageBorder->SetGaugeProgressType( _XGAUGEPROGRESSTYPE_BACKWARD );
	InsertChildObject( m_pLifePowerDamageBorder );
	
	m_pLifePowerBorder = new _XStickGauge;
	m_pLifePowerBorder->Create( healthgaugestruct );	
	m_pLifePowerBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  133, 141,  145 );
	m_pLifePowerBorder->SetCallBackFunction_FactorDraw( _XJinStickGaugeCallback_LifePower );
	m_pLifePowerBorder->SetFactorDraw(TRUE);
	m_pLifePowerBorder->SetGaugeColorFactor( D3DCOLOR_ARGB( 255, 255, 0, 0 ), D3DCOLOR_ARGB( 160, 255, 0, 0 ) );
	m_pLifePowerBorder->SetViewDelayGauge( TRUE, 20 );
	m_pLifePowerBorder->SetBlinking( TRUE, 10 );
	//m_pLifePowerBorder->ShowWindow( FALSE );
	InsertChildObject( m_pLifePowerBorder );
	
	healthgaugestruct.position.y		=	56;

	m_pForcePowerDamageBorder = new _XStickGauge;
	m_pForcePowerDamageBorder->Create( healthgaugestruct );	
	m_pForcePowerDamageBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  145, 141,  157 );
	m_pForcePowerDamageBorder->SetCallBackFunction_FactorDraw( _XJinStickGaugeCallback_DamagedForcePower );
	m_pForcePowerDamageBorder->SetFactorDraw(TRUE);
	m_pForcePowerDamageBorder->SetGaugeProgressType( _XGAUGEPROGRESSTYPE_BACKWARD );
	InsertChildObject( m_pForcePowerDamageBorder );
			
	m_pForcePowerBorder = new _XStickGauge;
	m_pForcePowerBorder->Create( healthgaugestruct );	
	m_pForcePowerBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  133, 141,  145 );
	m_pForcePowerBorder->SetCallBackFunction_FactorDraw( _XJinStickGaugeCallback_ForcePower );
	m_pForcePowerBorder->SetFactorDraw(TRUE);
	m_pForcePowerBorder->SetGaugeColorFactor( D3DCOLOR_ARGB( 255, 0, 0, 255 ), D3DCOLOR_ARGB( 160, 0, 0, 255 )  );
	m_pForcePowerBorder->SetViewDelayGauge( TRUE, 20 );
	//m_pForcePowerBorder->ShowWindow( FALSE );
	InsertChildObject( m_pForcePowerBorder );
		
	// 생사결 중독시간 바탕 1 (11X40)
	m_BossRoomPoisonTimerBorder1 = new _XImageStatic;
	m_BossRoomPoisonTimerBorder1->Create(0, 0, 11, 40, &g_MainInterfaceTextureArchive, characterresourceindex);
	m_BossRoomPoisonTimerBorder1->SetClipRect(114, 94, 125, 134);
	
	// 생사결 중독시간 바탕 2 (8X40) -> (139X40)
	m_BossRoomPoisonTimerBorder2 = new _XImageStatic;
	m_BossRoomPoisonTimerBorder2->Create(0, 0, 10, 40, &g_MainInterfaceTextureArchive, characterresourceindex);
	m_BossRoomPoisonTimerBorder2->SetClipRect(128, 94, 136, 134);
	m_BossRoomPoisonTimerBorder2->SetScale( 17.375f, 1.0f);
	
	// 생사결 중독시간 바탕 3 (11X40)
	m_BossRoomPoisonTimerBorder3 = new _XImageStatic;
	m_BossRoomPoisonTimerBorder3->Create(0, 0, 11, 40, &g_MainInterfaceTextureArchive, characterresourceindex);
	m_BossRoomPoisonTimerBorder3->SetClipRect(139, 94, 150, 134);
	
	int imageindex = g_MainInterfaceTextureArchive.FindResource( "Condition_icon_01.tga" );
	RECT rect = { 33, 33, 63, 63 };
			
	m_BossRoomPoisonIcon = new _XImageStatic;
	m_BossRoomPoisonIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
	m_BossRoomPoisonIcon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
	m_BossRoomPoisonIcon->SetScale(0.94f, 0.94f);
	
	// 패왕릉 중독아이콘
	RECT rect1 = { 0, 33, 33, 63 };	
	m_PehwangPoisonIcon = new _XImageStatic;
	m_PehwangPoisonIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
	m_PehwangPoisonIcon->SetClipRect(rect1.left, rect1.top, rect1.right, rect1.bottom);
	m_PehwangPoisonIcon->SetScale(0.94f, 0.94f);
	
	#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI

		// sds on/off 버튼
		_XBTN_STRUCTURE sdsbtnstruct = 
		{
			TRUE,
			{53, 83},
			{28, 28},
			_XDEF_JINSTATUS_SHUTDOWNONOFFBUTTON,
			g_MainInterfaceTextureArchive.FindResource("mi_main_unki_icon.tga"),
			g_MainInterfaceTextureArchive.FindResource("mi_main_unki_icon.tga"),
			g_MainInterfaceTextureArchive.FindResource("mi_main_unki_icon.tga"),
			&g_MainInterfaceTextureArchive
		};
		
		m_btnSDSOnOff = new _XButton;
		m_btnSDSOnOff->Create(sdsbtnstruct);
		m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 227, 65, 255, 93);
		m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 227, 65, 255, 93);
		m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 227, 65, 255, 93);
		m_btnSDSOnOff->ShowWindow(FALSE);
		InsertChildObject(m_btnSDSOnOff);

	#endif

	m_pClassIcon = new _XImageStatic;
	m_pClassIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("skill_icon_08_02.tga"));
	m_pClassIcon->SetClipRect(2, 2, 32, 32);

#endif
	
	/*
	int jinwinxpos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_JINWINDOWXPOS, 0, TRUE );
	if( jinwinxpos < 0 ) jinwinxpos = 0;
	int jinwinypos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_JINWINDOWYPOS, 0, TRUE );
	if( jinwinypos < 0 ) jinwinypos = 0;
	MoveWindow( jinwinxpos, jinwinypos );
	*/
	CheckOutside();
			
	return TRUE;
}

void _XWindow_JinStatus::SetDefaultPosition( void )
{		
#ifdef _ACCLAIM_RUBICONADSYSTEM			
	if( g_ViewRubiconAD )
		MoveWindow( 0, g_RubiconADWindowSize.cy );
	else
		MoveWindow( 0, 0 );
#else
	MoveWindow( 0, 0 );
#endif
	CheckOutside();
}

void _XWindow_JinStatus::SaveWindowProperty( void )
{
	/*
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_JINWINDOWXPOS,   GetWindowPos().x );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_JINWINDOWYPOS,   GetWindowPos().y );	
	*/
}

void _XWindow_JinStatus::DestroyWindow( void )
{

	for(int i = 0 ; i < 24 ; i++)
		SAFE_DELETE(m_FuryParameterImage[i]);

	for(i = 0 ; i < 5 ; i++)
		SAFE_DELETE(m_ConvergenceImage[i]);

	for(int jj = 0 ; jj < 6 ; jj++)
		SAFE_DELETE(m_LevelUpImage[jj]);

	SAFE_DELETE(m_pClassIcon);

	_XWindow::DestroyWindow();
}

#define _XDEF_JINGAUGEFRAMECOUNTERTICK		200	

void _XWindow_JinStatus::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		if( m_pClassIcon )
			m_pClassIcon->Draw(m_WindowPosition.x+23, m_WindowPosition.y+22);
	}

	_XWindow::Draw(pfocusobject);	
	
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI

	TCHAR szText[256];
	int textwidth = 0;
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		sprintf( szText, "%s : %d[Lv.%d]", g_LevelName[ g_pLocalUser->m_CharacterInfo.Get_level() ], 
			g_pLocalUser->m_CharacterInfo.Get_sublevel(), g_pLocalUser->m_CharacterInfo.Get_innerlevel() );

		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->PutsAlign( m_WindowPosition.x + 179, m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, szText );

		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->PutsAlign( m_WindowPosition.x + 162, m_WindowPosition.y + 24, _XFONT_ALIGNTYPE_CENTER, g_pLocalUser->m_CharacterInfo.charactername );
	}
	else
	{
		sprintf( szText, "%s %d%s[Lv.%d]", g_LevelName[ g_pLocalUser->m_CharacterInfo.Get_level() ], 
			g_pLocalUser->m_CharacterInfo.Get_sublevel(),  
			_XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL), g_pLocalUser->m_CharacterInfo.Get_innerlevel() );

		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->PutsAlign( m_WindowPosition.x + 179, m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, szText );

		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->PutsAlign( m_WindowPosition.x + 162, m_WindowPosition.y + 24, _XFONT_ALIGNTYPE_CENTER, g_pLocalUser->m_CharacterInfo.charactername );
	}
	
	g_XBaseFont->Flush();

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();	
	
	if( ((scrnpos->x > m_WindowPosition.x+23) ) && (scrnpos->x < m_WindowPosition.x+23+32) &&
		(scrnpos->z > m_WindowPosition.y+22 ) && (scrnpos->z < m_WindowPosition.y+22+32) )
	{
		if(g_pLocalUser->m_CharacterInfo.Get_cClass() > 0)
		{
			int classid = g_pLocalUser->m_CharacterInfo.Get_cClass();
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG)
			{
				if(classid == 7)	// 섭혼녀
					classid = 4;
			}
			else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM)
			{
				if(classid == 8)	// 철갑귀
					classid = 4;
			}

			TCHAR classname[256];
			sprintf(classname, "%s(%s)", 
				g_CharacterRollNameTable[g_pLocalUser->m_CharacterInfo.groupindex][g_pLocalUser->m_CharacterInfo.Get_cClass()][g_pLocalUser->m_CharacterInfo.Get_cClassGrade()],
				_XGETINTERFACETEXT(ID_STRING_NEW_2660+classid-1));
			
			g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+102, m_WindowPosition.y+56, classname, _XSC_INFORMATION, TRUE);
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 0)
		{
			g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+102, m_WindowPosition.y+56, _XGETINTERFACETEXT(ID_STRING_NEW_3713), _XSC_INFORMATION, TRUE);
		}
	}

	BOOL flag = false;
	if( scrnpos->x > m_WindowPosition.x+84 && scrnpos->x < m_WindowPosition.x+253 && scrnpos->z > 22 && scrnpos->z < 70 )
	{
		flag = true;
		if( g_pMiniTooltip_Window )
		{
			if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_CHARINFO, 0, 0, 0 ) )
				g_pMiniTooltip_Window->ShowWindow( TRUE );
		}		
	}	

	if( flag == false && g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_CHARINFO )
		g_pMiniTooltip_Window->ShowWindow( FALSE );

	if(m_LevelUpButton->GetShowStatus())
	{
		DrawLevelUp();	
	}

	if( m_AbilityUpButton->GetShowStatus() )
	{
		if( !m_AbilityUpButton->CheckMousePosition() )		
		{
			FLOAT currentTime = g_fLocalSystemTime;
			if( g_fLocalSystemTime - m_AbilityUpFrame > 200.0f )
			{
				m_AbilityUpFrame = g_fLocalSystemTime;
				m_CurAbilityUpCount++;
				if( m_CurAbilityUpCount > 4 )
					m_CurAbilityUpCount = 0;				
			}						
			
			m_AbilityUpButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, g_AbilityPosition[m_CurAbilityUpCount].x, g_AbilityPosition[m_CurAbilityUpCount].y, 
													   g_AbilityPosition[m_CurAbilityUpCount].x+28, g_AbilityPosition[m_CurAbilityUpCount].y+28);			
		}
	}

	DrawMatchWaitTimer();		// 비무 대기 - 남은시간 표시

	m_pFireImage->Update(0.02); //Author : 양희왕

#else

	BOOL viewMaxEffect = ( g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= 
		                   g_UserLevelTable[ g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1 ].needjinexperience ) ? TRUE : FALSE;

	FLOAT accfactor = 9.0f * ((FLOAT)g_pLocalUser->m_CharacterInfo.Get_jinlevel() / 
		                      (FLOAT)g_UserLevelTable[ g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1 ].needjinexperience);
	
	FLOAT gaugefactor = (FLOAT)g_UserLevelTable[ g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1 ].needjinexperience / 9.0f;
	if( (int)gaugefactor > 0 )
		gaugefactor = (g_pLocalUser->m_CharacterInfo.Get_jinlevel() % (int)gaugefactor) / gaugefactor;


	if( m_SpeedCounter == 0 ) m_SpeedCounter = g_LocalSystemTime;
	int elapsedtimetick = g_LocalSystemTime - m_SpeedCounter;
	m_SpeedCounter = g_LocalSystemTime;
	
	m_JinBorderFrame += ((FLOAT)elapsedtimetick / 100.0f);
	int jiborderframe = (int)m_JinBorderFrame; //_XFC_FloatToIntRet( m_JinBorderFrame );
	if( jiborderframe > 14 )
	{		
		jiborderframe = 14;
	}

	if( m_JinBorderPrevFrame != jiborderframe )
	{	
		RECT	cliprect;
		int		column, row;
		column = jiborderframe % 4;
		row	   = (jiborderframe >> 2);
		cliprect.left  = column * 32;
		cliprect.right = cliprect.left+32;
		cliprect.top  = row * 32;
		cliprect.bottom = cliprect.top+32;
		m_JinBorderImage.SetClipRect( cliprect );	
		
		// Gauge rect 
		
		m_JinGaugePercentage = (FLOAT)( 23.0f * ( gaugefactor ) );
		cliprect.top += (23 - m_JinGaugePercentage) + 4;
		m_JinGaugeImage.SetClipRect( cliprect );
		m_JinGaugeImage.MoveWindow( m_JinGaugeImage.GetWindowPos().x, m_WindowPosition.y + 26 + (23 - m_JinGaugePercentage) );		
	}

	m_JinMaxEffectFrame += ((FLOAT)elapsedtimetick / 50.0f);
	int jinmaxframe = (int)m_JinMaxEffectFrame; //_XFC_FloatToIntRet( m_JinBorderFrame );
	if( jinmaxframe > 22 )
	{		
		jinmaxframe = 22;
	}

	if( m_JinMaxEffectPrevFrame != jinmaxframe )
	{	
		RECT	cliprect;
		int		column, row;
		column = jinmaxframe % 5;
		row	   = jinmaxframe / 5;
		cliprect.left  = column * 51;
		cliprect.right = cliprect.left+51;
		cliprect.top  = row * 51;
		cliprect.bottom = cliprect.top+51;
		m_JinMaxImage.SetClipRect( cliprect );
	}

	m_AccumulatedImageBorderFrame += ((FLOAT)elapsedtimetick / 300.0f);
	int accframe = (int)m_AccumulatedImageBorderFrame; //_XFC_FloatToIntRet( m_AccumulatedImageBorderFrame );
	if( accframe > 3 )
	{		
		accframe = 3;
	}
	
	if( m_AccumulatedImageBorderPrevFrame != accframe )
	{	
		RECT	cliprect;
		int		column;
		column = accframe % 4;
		cliprect.left  = 204 + column * 5;
		cliprect.right = cliprect.left+5;
		cliprect.top  = 204;
		cliprect.bottom = 209;
		for( int i = 0; i < 8; i++ )
		{
			m_AccumulatedJinImage[i].SetClipRect( cliprect );
		}
	}

	if( m_JinBorderFrame > 14.0f )
	{
		m_JinBorderFrame = 0.0f;
	}
	if( m_JinMaxEffectFrame > 22.0f )
	{
		m_JinMaxEffectFrame = 0.0f;
	}
	if( m_AccumulatedImageBorderFrame > 3.0f )
	{
		m_AccumulatedImageBorderFrame = 0.0f;
	}
	if( m_FireEffectFrame > 15.0f )
	{
		m_FireEffectFrame = 0.0f;
	}
	

	m_FireEffectFrame += ((FLOAT)elapsedtimetick / 100.0f);
	int fireborderframe = (int)m_FireEffectFrame; //_XFC_FloatToIntRet( m_JinBorderFrame );
	if( fireborderframe > 15 )
	{		
		fireborderframe = 15;
	}
	
	if( m_FireEffectPrevFrame != fireborderframe )
	{	
		RECT	cliprect;
		int		column, row;
		column = fireborderframe % 4;
		row	   = (fireborderframe >> 2);
		cliprect.left  = column * 30;
		cliprect.right = cliprect.left+30;
		cliprect.top  = row * 47;
		cliprect.bottom = cliprect.top+47;
		m_LeftFireImage.SetClipRect( cliprect );
		m_RightFireImage.SetClipRect( cliprect );
	}

	m_JinBorderPrevFrame = jiborderframe;
	m_JinMaxEffectPrevFrame = jinmaxframe;
	m_AccumulatedImageBorderPrevFrame = accframe;
	m_FireEffectPrevFrame = fireborderframe;


	if(g_pLocalUser->m_UserTroubleState != _XDEF_CONVSTATE_FURY)
	{
		m_JinBorderImage.DrawWithRegion();
		m_JinGaugeImage.DrawWithRegion();
	}
	
	m_LeftFireImage.DrawWithRegion();
	m_RightFireImage.DrawWithRegion();

	if(g_pLocalUser->m_UserTroubleState != _XDEF_CONVSTATE_FURY)
	{
		if( viewMaxEffect ) 
		{
			m_JinMaxImage.DrawWithRegion();
		}
		else
		{
			for( int i = 0; i < (int)accfactor; i++ )
			{
				//if( i == 8 ) break;
				m_AccumulatedJinImage[i].DrawWithRegion();
			}
		}
	}


	TCHAR szText[256];
	int textwidth = 0;
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		sprintf( szText, "%s : %d", g_LevelName[ g_pLocalUser->m_CharacterInfo.Get_level() ], 
			g_pLocalUser->m_CharacterInfo.Get_sublevel() );	// 2004.05.17->oneway48 modify

		int textwidth = g_XBaseFont->GetWidth( szText );
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->PutsAlign( m_WindowPosition.x + m_WindowSize.cx - 4, m_WindowPosition.y + 7, _XFONT_ALIGNTYPE_RIGHT, szText );

		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->PutsAlign( m_WindowPosition.x + m_WindowSize.cx - 4, m_WindowPosition.y + 24, _XFONT_ALIGNTYPE_RIGHT, g_pLocalUser->m_CharacterInfo.charactername );
	}
	else
	{
#ifdef _XDEF_MODIFY_UI_070710_KUKURI
		sprintf( szText, "%s %d%s[%d]", g_LevelName[ g_pLocalUser->m_CharacterInfo.Get_level() ], 
			g_pLocalUser->m_CharacterInfo.Get_sublevel(),  
			_XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL), g_pLocalUser->m_CharacterInfo.Get_innerlevel() );
#else
		sprintf( szText, "%s %d%s", g_LevelName[ g_pLocalUser->m_CharacterInfo.Get_level() ], 
			g_pLocalUser->m_CharacterInfo.Get_sublevel(), 
			_XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL) );	// 2004.05.17->oneway48 modify
#endif

		textwidth = g_XBaseFont->GetWidth( szText );
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->Puts( m_WindowPosition.x + 165 - (textwidth >> 1), m_WindowPosition.y + 7, szText );

		g_XBaseFont->SetColor( 0xFF000000 );
		g_XBaseFont->Puts( m_WindowPosition.x + 100, m_WindowPosition.y + 25, 
			g_CharacterRollNameTable[g_pLocalUser->m_CharacterInfo.groupindex][g_pLocalUser->m_CharacterInfo.Get_cClass()][g_pLocalUser->m_CharacterInfo.Get_cClassGrade()] );
		g_XBaseFont->Puts( m_WindowPosition.x + 154, m_WindowPosition.y + 25, g_pLocalUser->m_CharacterInfo.charactername );
		
		g_XBaseFont->SetColor( 0xFFBBB96E );
		g_XBaseFont->Puts( m_WindowPosition.x + 99, m_WindowPosition.y + 24, 
			g_CharacterRollNameTable[g_pLocalUser->m_CharacterInfo.groupindex][g_pLocalUser->m_CharacterInfo.Get_cClass()][g_pLocalUser->m_CharacterInfo.Get_cClassGrade()] );
		
		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->Puts( m_WindowPosition.x + 153, m_WindowPosition.y + 24, g_pLocalUser->m_CharacterInfo.charactername );
	}
	
	g_XBaseFont->Flush();

#ifdef _XTS_LEVELTOOLTIP
	// level tooltip
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		// 베트남은 적용 안함.꽝이 요청했음.
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		MouseState* mousestate = gpInput->GetMouseState();
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();	
		
		if( ((scrnpos->x > m_WindowPosition.x+145) && (scrnpos->x < m_WindowPosition.x+145+78) &&
			(scrnpos->z > m_WindowPosition.y ) && (scrnpos->z < m_WindowPosition.y+20) ) )
		{
			TCHAR messagebuffer[256];
			sprintf(messagebuffer, _XGETINTERFACETEXT(ID_STRING_NEW_2753), g_pLocalUser->m_CharacterInfo.Get_innerlevel());		// "레벨 %d"
			g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+223, m_WindowPosition.y+22, messagebuffer, _XSC_INFORMATION, TRUE);
		}
	}
	else
	{
		 
 		if( !g_bShowItemMallWindow )//아이템몰이 있는 상태에서는 툴팁을 그리지 않는다.
		{
			MouseState* mousestate = gpInput->GetMouseState();
			ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();	
			
			if( ((scrnpos->x > m_WindowPosition.x+165-(textwidth>>1) ) && (scrnpos->x < m_WindowPosition.x+165-(textwidth>>1)+textwidth) &&
				(scrnpos->z > m_WindowPosition.y+7 ) && (scrnpos->z < m_WindowPosition.y+7+11) ) )
			{
				TCHAR messagebuffer[256];
				sprintf(messagebuffer, _XGETINTERFACETEXT(ID_STRING_NEW_2753), g_pLocalUser->m_CharacterInfo.Get_innerlevel());		// "레벨 %d"
				g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+165-(textwidth>>1)+textwidth, m_WindowPosition.y+7+11, messagebuffer, _XSC_INFORMATION, TRUE);
			}
		}
	}
#endif
	
#ifdef _XDEF_MODIFY_UI_070710_KUKURI
	if( !g_bShowItemMallWindow )//아이템몰이 있는 상태에서는 툴팁을 그리지 않는다.
	{
		MouseState* mousestate = gpInput->GetMouseState();
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();	
		
		if( ((scrnpos->x > m_WindowPosition.x+99) ) && (scrnpos->x < m_WindowPosition.x+99+36) &&
			(scrnpos->z > m_WindowPosition.y+23 ) && (scrnpos->z < m_WindowPosition.y+23+13) )
		{
			if(g_pLocalUser->m_CharacterInfo.Get_cClass() > 0)
			{
				int classid = g_pLocalUser->m_CharacterInfo.Get_cClass();
				if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG)
				{
					if(classid == 7)	// 섭혼녀
						classid = 4;
				}
				else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM)
				{
					if(classid == 8)	// 철갑귀
						classid = 4;
				}

				g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+104, m_WindowPosition.y+38, 
					_XGETINTERFACETEXT(ID_STRING_NEW_2660+classid-1), _XSC_INFORMATION, TRUE);
			}
		}
	}
#endif

//	if( m_LevelUpButton->IsButtonEnabled() )
	if(m_LevelUpButton->GetShowStatus())
	{
		DrawLevelUp();	
	}

	if( m_AbilityUpButton->GetShowStatus() )
	{
		if( !m_AbilityUpButton->CheckMousePosition() )		
		{
			FLOAT currentTime = g_fLocalSystemTime;
			if( g_fLocalSystemTime - m_AbilityUpFrame > 200.0f )
			{
				m_AbilityUpFrame = g_fLocalSystemTime;
				m_CurAbilityUpCount++;
				if( m_CurAbilityUpCount > 4 )
					m_CurAbilityUpCount = 0;				
			}						
			
			m_AbilityUpButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, g_AbilityPosition[m_CurAbilityUpCount].x, g_AbilityPosition[m_CurAbilityUpCount].y, 
													   g_AbilityPosition[m_CurAbilityUpCount].x+28, g_AbilityPosition[m_CurAbilityUpCount].y+28);			
		}
	}

	FLOAT levelpercetage = ((FLOAT)g_pLocalUser->m_CharacterInfo.Get_jinlevel() / 
		(FLOAT)g_UserLevelTable[ g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1 ].needjinexperience) * 100;
	
	sprintf( szText,"%.2f%%",levelpercetage);
	textwidth = g_XBaseFont->GetWidth( szText );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 - (textwidth >> 1), m_WindowPosition.y + 66, szText ); 
	g_XBaseFont->Flush();

	
//	if( g_pLocalUser->m_bIsBossBattle )	DrawBossRoomPoisonTimer();	// 생사결 치사독 시간 표시 
//	if( m_bViewPehwangPoisonIcon ) DrawPehwangPoisonIcon();			// 패왕릉 독무 아이콘 표시 
	DrawMatchWaitTimer();		// 비무 대기 - 남은시간 표시
	DrawFuryParameter();
	DrawTooltip();

#endif
}

BOOL _XWindow_JinStatus::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	ProcessConvergence();

	MouseState* mousestate = gpInput->GetMouseState();
	
	if(!pfocusobject)  
	{
		if(mousestate->bButton[0])
		{
			if(CheckMousePosition())
			{
				if(g_pLocalUser->m_AttackTargetObjectType != _XPICKINGTARGET_SELF)
				{
					g_pLocalUser->SetViewTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
					g_pLocalUser->SetAttackTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
				}
			}
		}
	}

/*	if(pfocusobject == this)
	{
		_XDWINPRINT("focus == jinstatus");

		if(!mousestate->bButton[0])
		{
			_XDWINPRINT("focus == jinstatus , mouse up");

			if(g_pLocalUser->m_AttackTargetObjectType != _XPICKINGTARGET_SELF)
			{
				g_pLocalUser->SetViewTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
				g_pLocalUser->SetAttackTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
			}
		}
	}*/
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}

void _XWindow_JinStatus::MoveWindow( int X, int Y )
{		
	_XWindow::MoveWindow( X, Y );

#ifndef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	// define되어 있지 않을 때 실행
	m_JinBorderImage.MoveWindow( m_WindowPosition.x + 22, m_WindowPosition.y + 22 );
	
	RECT cliprect = m_JinGaugeImage.m_ClipRect;
	cliprect.top += (23 - m_JinGaugePercentage) + 4;
	m_JinGaugeImage.SetClipRect( cliprect );
	m_JinGaugeImage.MoveWindow( m_WindowPosition.x + 22, m_WindowPosition.y + 26 + (23 - m_JinGaugePercentage) );
	m_JinMaxImage.MoveWindow( m_WindowPosition.x + 13, m_WindowPosition.y + 12 ); 
	
	m_LeftFireImage.MoveWindow( m_WindowPosition.x - 6, m_WindowPosition.y + 23 );
	m_RightFireImage.MoveWindow( m_WindowPosition.x + 54, m_WindowPosition.y + 23 );
	
	for( int i = 0; i < 8; i++ )
	{
		m_AccumulatedJinImage[i].MoveWindow( m_WindowPosition.x + g_JinRoundPosition[i].x + 10, 
											 m_WindowPosition.y + g_JinRoundPosition[i].y + 12);
	}
	
	if(m_FuryParameterImage[0])  m_FuryParameterImage[0]->MoveWindow(m_WindowPosition.x+18+20, m_WindowPosition.y+17);
	if(m_FuryParameterImage[1])  m_FuryParameterImage[1]->MoveWindow(m_WindowPosition.x+18+26, m_WindowPosition.y+17);
	if(m_FuryParameterImage[2])  m_FuryParameterImage[2]->MoveWindow(m_WindowPosition.x+18+29, m_WindowPosition.y+17+3);
	if(m_FuryParameterImage[3])  m_FuryParameterImage[3]->MoveWindow(m_WindowPosition.x+18+32, m_WindowPosition.y+17+6);
	if(m_FuryParameterImage[4])  m_FuryParameterImage[4]->MoveWindow(m_WindowPosition.x+18+35, m_WindowPosition.y+17+9);
	if(m_FuryParameterImage[5])  m_FuryParameterImage[5]->MoveWindow(m_WindowPosition.x+18+37, m_WindowPosition.y+17+14);
	if(m_FuryParameterImage[6])  m_FuryParameterImage[6]->MoveWindow(m_WindowPosition.x+18+37, m_WindowPosition.y+17+22);
	if(m_FuryParameterImage[7])  m_FuryParameterImage[7]->MoveWindow(m_WindowPosition.x+18+35, m_WindowPosition.y+17+27);
	if(m_FuryParameterImage[8])  m_FuryParameterImage[8]->MoveWindow(m_WindowPosition.x+18+32, m_WindowPosition.y+17+29);
	if(m_FuryParameterImage[9])  m_FuryParameterImage[9]->MoveWindow(m_WindowPosition.x+18+30, m_WindowPosition.y+17+34);
	if(m_FuryParameterImage[10]) m_FuryParameterImage[10]->MoveWindow(m_WindowPosition.x+18+25, m_WindowPosition.y+17+36);
	if(m_FuryParameterImage[11]) m_FuryParameterImage[11]->MoveWindow(m_WindowPosition.x+18+21, m_WindowPosition.y+17+38);
	if(m_FuryParameterImage[12]) m_FuryParameterImage[12]->MoveWindow(m_WindowPosition.x+18+15, m_WindowPosition.y+17+38);
	if(m_FuryParameterImage[13]) m_FuryParameterImage[13]->MoveWindow(m_WindowPosition.x+18+10, m_WindowPosition.y+17+36);
	if(m_FuryParameterImage[14]) m_FuryParameterImage[14]->MoveWindow(m_WindowPosition.x+18+5, m_WindowPosition.y+17+34);
	if(m_FuryParameterImage[15]) m_FuryParameterImage[15]->MoveWindow(m_WindowPosition.x+18+2, m_WindowPosition.y+17+31);
	if(m_FuryParameterImage[16]) m_FuryParameterImage[16]->MoveWindow(m_WindowPosition.x+18, m_WindowPosition.y+17+27);
	if(m_FuryParameterImage[17]) m_FuryParameterImage[17]->MoveWindow(m_WindowPosition.x+18, m_WindowPosition.y+17+22);
	if(m_FuryParameterImage[18]) m_FuryParameterImage[18]->MoveWindow(m_WindowPosition.x+18, m_WindowPosition.y+17+15);
	if(m_FuryParameterImage[19]) m_FuryParameterImage[19]->MoveWindow(m_WindowPosition.x+18, m_WindowPosition.y+17+9);
	if(m_FuryParameterImage[20]) m_FuryParameterImage[20]->MoveWindow(m_WindowPosition.x+18+2, m_WindowPosition.y+17+5);
	if(m_FuryParameterImage[21]) m_FuryParameterImage[21]->MoveWindow(m_WindowPosition.x+18+5, m_WindowPosition.y+17+2);
	if(m_FuryParameterImage[22]) m_FuryParameterImage[22]->MoveWindow(m_WindowPosition.x+18+9, m_WindowPosition.y+17);
	if(m_FuryParameterImage[23]) m_FuryParameterImage[23]->MoveWindow(m_WindowPosition.x+18+12, m_WindowPosition.y+17);

	for(i = 0 ; i < 5 ; i++)
	{
		if( m_ConvergenceImage[i] )
			m_ConvergenceImage[i]->MoveWindow(m_WindowPosition.x,m_WindowPosition.y);
	}

	for(i = 0 ; i < 6 ; i++)
	{
		if(m_LevelUpImage[i])
			m_LevelUpImage[i]->MoveWindow(m_WindowPosition.x,m_WindowPosition.y);
	}

#endif
}


void _XWindow_JinStatus::SetButtonStatus(BOOL Show)
{
	if( Show )
	{		
		if( m_LevelUpButton )
		{
			m_LevelUpButton->EnableWindow( TRUE );
			m_LevelUpButton->ShowWindow( TRUE );
			_XPlayInterfaceSound(ID_SR_INTERFACE_LEVELUP_INTRO_WAV);
		}
	}
	else
	{		
		if( m_LevelUpButton )
		{
			m_LevelUpButton->EnableWindow( FALSE );
			m_LevelUpButton->ShowWindow( FALSE );			
		}
		
		if( g_CurrentFocusedObject == m_LevelUpButton )
		{
			g_CurrentFocusedObject = NULL;
		}			
	}

}

void _XWindow_JinStatus::DrawLevelUp()
{
	if( m_LevelUpSpeedCounter == 0 ) 
		m_LevelUpSpeedCounter = g_LocalSystemTime;
	
	int elapsedtimetick = g_LocalSystemTime - m_LevelUpSpeedCounter;

	m_LevelUpSpeedCounter = g_LocalSystemTime;
	
	m_SelectedEffectFrame += ((FLOAT)elapsedtimetick / 120.0f);

	if( m_SelectedEffectFrame > 6.0f )
	{		
		m_SelectedEffectFrame = 0.f;
		m_LevelUpSpeedCounter = 0.f;
	}

	int selectedframe = (int)m_SelectedEffectFrame; 
	
//	if( m_SelectedEffectPrevFrame != selectedframe )
	{	
		if( selectedframe < 6 )
		{
			m_LevelUpImage[selectedframe]->Draw(m_WindowPosition.x + (gnWidth>>1)-103 + 26, m_WindowPosition.y + 75 + 8);
//			int y = 82+selectedframe*17;
			
//			m_LevelUpButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0,y,43,y+17 );
		}
	}
	
	m_SelectedEffectPrevFrame = selectedframe;
}

void _XWindow_JinStatus::DrawFuryParameter(void)
{
	int count = g_pLocalUser->m_CharacterInfo.Get_furyparameter() / 417;
	for(int i = 0 ; i <= count ; i++)
	{
		if(i < 24)
		{
			if(i == count)
			{
				if(g_pLocalUser->m_CharacterInfo.Get_furyparameter() > (417*i) + 200)
					m_FuryParameterImage[i]->Draw();
			}
			else
				m_FuryParameterImage[i]->Draw();
		}
	}

	if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_FURY)
	{
		// button animation draw
		if(m_ConvPrevDrawTime == 0)
			m_ConvPrevDrawTime = g_LocalSystemTime;

		DWORD elapsedtime = g_LocalSystemTime - m_ConvPrevDrawTime;

		if(!m_ConvEffectOrderSub)
			m_ConvEffectFrame += ((FLOAT)elapsedtime / 100.0f);
		else
			m_ConvEffectFrame -= ((FLOAT)elapsedtime / 100.0f);

		if(m_ConvEffectFrame > 5.0f)
		{
			m_ConvEffectOrderSub = true;
			m_ConvEffectFrame = 4.0f;
		}
		else if(m_ConvEffectFrame <= 0.0f)
		{
			m_ConvEffectOrderSub = false;
			m_ConvEffectFrame = 1.0f;
		}

		int selectedframe = (int)m_ConvEffectFrame;

		if(selectedframe > 4)
			selectedframe = 4;
		else if(selectedframe < 0)
			selectedframe = 0;

		m_ConvergenceImage[selectedframe]->Draw(m_WindowPosition.x+20, m_WindowPosition.y+20);
		m_ConvPrevDrawTime = g_LocalSystemTime;
	}
}

void _XWindow_JinStatus::ProcessConvergence(void)
{

#ifndef _XDEF_INTERFACE_RENEWAL_071002_KUKURI

	if(g_pLocalUser->m_UserTroubleState != _XDEF_CONVSTATE_FURY && g_pLocalUser->m_UserTroubleState != _XDEF_CONVSTATE_CONVERGENCE)
		return;

	DWORD elapsedtime = g_LocalSystemTime - m_ConvergenceStartTime;
	FLOAT furyparameter = (FLOAT)g_pLocalUser->m_CharacterInfo.Get_furyparameter();


	if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_FURY)
	{
		furyparameter -= elapsedtime * 0.05f;
		g_pLocalUser->m_CharacterInfo.Set_furyparameter( (short)furyparameter );

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
			if(!m_bConvergenceSendPacket)
			{
				g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_normal_fury);
				m_bConvergenceSendPacket = true;
			}
		}
	}
	else
		m_ConvergenceStartTime = g_LocalSystemTime;

#endif

}

void _XWindow_JinStatus::ClearConvergence(void)
{
#ifndef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	g_pLocalUser->m_UserTroubleState = _XDEF_CONVSTATE_NORMAL;
	m_ConvergenceButton->ShowWindow(FALSE);
	m_ConvergenceStartTime = 0;
	m_bConvergenceSendPacket = false;

	if(g_pLocalUser->m_ModelDescriptor.m_EffectManager)
	{
		if(g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(_XDEF_PLAYERID_BERSERK))
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_PLAYERID_BERSERK);
	}
#endif
}


void _XWindow_JinStatus::SetParameter( void )
{
	short maxlife = 0, maxforce = 0;


	maxlife = g_pLocalUser->m_MaxLifePower.Get_result();
	maxforce = g_pLocalUser->m_MaxForcePower.Get_result();
	
	if( maxlife )
	{
		g_pLocalUser->LifePowerPercentage	=	(int)(((FLOAT)g_pLocalUser->m_CharacterInfo.Get_current_lifepower() / (FLOAT)maxlife) * 100.0f);
	}
	else
		g_pLocalUser->LifePowerPercentage	=	0;
	
	if( maxforce )		
		g_pLocalUser->ForcePowerPercentage	=	(int)((FLOAT)(g_pLocalUser->m_CharacterInfo.Get_current_forcepower() / (FLOAT)maxforce) * 100.0f);
	else
		g_pLocalUser->ForcePowerPercentage	=	0;
	
	m_pLifePowerBorder->SetGaugeFactor( g_pLocalUser->LifePowerPercentage );
	m_pForcePowerBorder->SetGaugeFactor( g_pLocalUser->ForcePowerPercentage );

	FLOAT externalwoundrate = (FLOAT)g_pLocalUser->m_CharacterInfo.Get_externalwoundrate()/(FLOAT)maxlife * 100.0f;
	m_pLifePowerDamageBorder->SetGaugeFactor( externalwoundrate );

	
	FLOAT internalwoundrate = (FLOAT)g_pLocalUser->m_CharacterInfo.Get_internalwoundrate()/(FLOAT)maxforce * 100.0f;
	m_pForcePowerDamageBorder->SetGaugeFactor( internalwoundrate );
		
	if(g_pCInfo_Window)
	{
		g_pCInfo_Window->SetParameter();
	}
	
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	if(g_pExp_Window)
	{
		g_pExp_Window->SetParameter();
	}
#endif

#ifdef _XTS_MEDI_TUTORIAL
#ifdef _XTS_SHOWTUTORIAL
	if( g_TutorialInfo )
	{
		if( !g_TutorialInfo->_XTutorial_052 )
		{
			if( g_pLocalUser->m_CharacterInfo.Get_innerlevel() < 6 )	// 레벨이 5이하일때만
			{
				if( g_pLocalUser->LifePowerPercentage < 30 || g_pLocalUser->ForcePowerPercentage < 30 )	// 30%이하면
				{
					int mediskill = g_pLocalUser->GetLearnedSkillIndex(1);	// 양생도인술을 배웠으면
					if( mediskill >= 0 )					
					{
						if( !g_pLocalUser->m_bVirtualMode ) //튜토리얼 모드일 때  양생도인술 가이드는 보여주지 않는다.
						{
							_XWindow_GuideLine* pGuideLine_Window = (_XWindow_GuideLine*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GUIDELINEWINDOW);
							if( pGuideLine_Window )
							{
								pGuideLine_Window->ShowWindow(TRUE);
								if( g_pQuickSlot_Window->FindQuickSlotItem(_XQS_SLOTTYPE_SKILL, 1) )	// 양생도인술이 등록되어 있다
								{
									_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
									if( pMainMenu_Window )
									{
										if( pMainMenu_Window->m_MeditationIndex > 0 )	// 운기요상 활성화 On
											pGuideLine_Window->SetGuideLIneStep(GS_THIRD);
										else
											pGuideLine_Window->SetGuideLIneStep(GS_SECOND);
									}								
								}
								else
								{
									pGuideLine_Window->SetGuideLIneStep(GS_FIRST);
								}							
							}
						}
					}
				}
			}
		}
	}
#endif
#endif
	
	//2004.07.05->oneway48 insert
	if(g_ViewMyBattleGauge)
	{
		if(g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
		{		
			g_pLocalUser->m_MiniHpBorder.SetGaugeFactor(g_pLocalUser->LifePowerPercentage);
		}
	}
}

void _XWindow_JinStatus::DrawTooltip(void)
{
 	if( g_bShowItemMallWindow ) return;
	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	bool flag = false;

	// 분노
	if( scrnpos->x > m_WindowPosition.x+15 && scrnpos->x < m_WindowPosition.x+64 && scrnpos->z > m_WindowPosition.y+13 && scrnpos->z < m_WindowPosition.y+61 )
	{
		TCHAR tempstr[128];		
		sprintf( tempstr, _XGETINTERFACETEXT(ID_STRING_JINSTATUS_FURY), g_pLocalUser->m_CharacterInfo.Get_furyparameter()*100.0f/10000.0f );	// "분노수치 : %3.2f%%"
		g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+17, m_WindowPosition.y+13, tempstr, _XSC_INFORMATION );		
	}
	else if( scrnpos->x > m_WindowPosition.x+81 && scrnpos->x < m_WindowPosition.x+226 && scrnpos->z > m_WindowPosition.y+1 && scrnpos->z < m_WindowPosition.y+69 )
	{
		flag = true;
		if( g_pMiniTooltip_Window )
		{
			if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_CHARINFO, 0, 0, 0 ) )
				g_pMiniTooltip_Window->ShowWindow( TRUE );
		}		
	}	

	if( flag == false && g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_CHARINFO )
		g_pMiniTooltip_Window->ShowWindow( FALSE );
}

void _XWindow_JinStatus::DrawBossRoomPoisonTimer(void)
{
	if( m_bViewBossRoomPoisonTimer )
	{
		if( g_pLocalUser->m_BossStartTime != 0 )
		{
			m_BossRoomPoisonTimerBorder1->Draw( 67, 73);
			m_BossRoomPoisonTimerBorder2->Draw( 78, 73);
			m_BossRoomPoisonTimerBorder3->Draw( 217, 73);
			
			m_BossRoomPoisonIcon->Draw( 73, 78);				
			_XDrawRectAngle( 73, 78, 101, 106, 0.0f, D3DCOLOR_ARGB(255, 158, 155, 148));
			
			g_XBaseFont->SetColor( _XSC_DEFAULT );
			g_XBaseFont->Puts( 118, 80, _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2155) );
			
			g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 254, 151, 33 ));	
			
			g_XBaseFont->Puts( 118, 95, _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2156) );
						
			if( (g_LocalSystemTime - g_pLocalUser->m_BossStartTime) < g_pLocalUser->m_BossTimeLimit*1000	 )
			{
				int temptime = g_pLocalUser->m_BossTimeLimit - (g_LocalSystemTime - g_pLocalUser->m_BossStartTime)/1000;
				int	minute	 =  temptime / 60;
				int	second	 =  temptime % 60;
				
				if( minute < 10)
				{
					if( second < 10 )
					{	
						g_XBaseFont->Print( 118, 95, 1.0f, "%s 0%d:0%d", _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2156), minute, second );						
					}
					else
					{		
						g_XBaseFont->Print( 118, 95, 1.0f, "%s 0%d:%d", _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2156), minute, second );						
					}	
				}
				else
				{
					if( second < 10 )
					{		
						g_XBaseFont->Print( 118, 95, 1.0f, "%s %d:0%d", _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2156), minute, second );
					}
					else
					{	
						g_XBaseFont->Print( 118, 95, 1.0f, "%s %d:%d", _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2156), minute, second );
					}	
				}	
				
			}
			else if( (g_LocalSystemTime - g_pLocalUser->m_BossStartTime) < (g_pLocalUser->m_BossTimeLimit+5)*1000 )
			{
				g_XBaseFont->Print( 118, 95, 1.0f, _T("%s 00:00"), _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2156) );
			}
			else
			{
				g_pLocalUser->m_BossStartTime = 0;
				g_pLocalUser->m_BossTimeLimit = 0;
				m_bViewBossRoomPoisonTimer = FALSE;
				
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2157), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
			}

			/*
			g_XBaseFont->Puts( 118, 95, _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2156) );
			g_XBaseFont->Puts( 119, 95, _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2156) );
			
			if( (g_LocalSystemTime - g_pLocalUser->m_BossStartTime) < g_pLocalUser->m_BossTimeLimit*1000	 )
			{
				int temptime = g_pLocalUser->m_BossTimeLimit - (g_LocalSystemTime - g_pLocalUser->m_BossStartTime)/1000;
				int	minute	 =  temptime / 60;
				int	second	 =  temptime % 60;
				
				if( minute < 10)
				{
					if( second < 10 )
					{	
						g_XBaseFont->Print( 175, 95, 1.0f, "0%d:0%d", minute, second );
						g_XBaseFont->Print( 176, 95, 1.0f, "0%d:0%d", minute, second );
					}
					else
					{		
						g_XBaseFont->Print( 175, 95, 1.0f, "0%d:%d", minute, second );
						g_XBaseFont->Print( 176, 95, 1.0f, "0%d:%d", minute, second );
					}	
				}
				else
				{
					if( second < 10 )
					{		
						g_XBaseFont->Print( 175, 95, 1.0f, "%d:0%d", minute, second );
						g_XBaseFont->Print( 176, 95, 1.0f, "%d:0%d", minute, second );
					}
					else
					{	
						g_XBaseFont->Print( 175, 95, 1.0f, "%d:%d", minute, second );
						g_XBaseFont->Print( 176, 95, 1.0f, "%d:%d", minute, second );
					}	
				}	
				
			}
			else if( (g_LocalSystemTime - g_pLocalUser->m_BossStartTime) < (g_pLocalUser->m_BossTimeLimit+5)*1000 )
			{
				g_XBaseFont->Puts( 175, 95, _T("00:00") );
				g_XBaseFont->Puts( 176, 95, _T("00:00") );
			}
			else
			{
				g_pLocalUser->m_BossStartTime = 0;
				g_pLocalUser->m_BossTimeLimit = 0;
				m_bViewBossRoomPoisonTimer = FALSE;
				
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2157), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
			}
			*/
			
			g_XBaseFont->Flush();
		}	
	}
	
}

void _XWindow_JinStatus::DrawPehwangPoisonIcon(void)
{
		m_BossRoomPoisonTimerBorder1->Draw( 67, 73);
		m_BossRoomPoisonTimerBorder2->Draw( 78, 73);
		m_BossRoomPoisonTimerBorder3->Draw( 217, 73);
		
		m_PehwangPoisonIcon->Draw( 73, 78);				
		_XDrawRectAngle( 73, 78, 101, 106, 0.0f, D3DCOLOR_ARGB(255, 158, 155, 148));
		
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->SetBoldMode( TRUE );
		g_XBaseFont->Puts( 118, 86, _XGETINTERFACETEXT(ID_STRING_NEW_2754) );// _T("패왕독무 중독상태")
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode( FALSE );

		g_XBaseFont->Flush();
}

void _XWindow_JinStatus::DrawMatchWaitTimer(void)
{
	if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_WAITING)
	{
		g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 254, 151, 33 ));	
		g_XBaseFont->Puts( 118, 95, _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2156) );

		DWORD waittime = g_pLocalUser->m_myMatchTableInfo.usWaitTime*1000 - (g_LocalSystemTime - g_pLocalUser->m_myMatchTableInfo.dwWaitStartTime);
		waittime /= 1000;

		int minute = waittime / 60;
		int second = waittime - minute*60;

		if(second < 0)
			second = 0;

		if(minute < 0)
		{
			g_XBaseFont->Print( 175, 95, 1.0f, _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2158), second );
		}
		else
		{
			g_XBaseFont->Puts( 175, 95, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_JINSTATUS_2159, &minute, &second ), 1.0f );
		}
		
		g_XBaseFont->Flush();
	}
}

void _XWindow_JinStatus::SetSDSState(int state)
{
	m_nSDSState = state;

	switch(state)
	{
	case 0 :
		{
			// on
			m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 167, 65, 195, 93);
			m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 167, 65, 195, 93);
			m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 167, 65, 195, 93);
			m_btnSDSOnOff->ShowWindow(TRUE);
			m_btnSDSOnOff->EnableWindow(TRUE);
		}
		break;
	case 1 :
		{
			// off
			m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 197, 65, 225, 93);
			m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 197, 65, 225, 93);
			m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 197, 65, 225, 93);
			m_btnSDSOnOff->ShowWindow(TRUE);
			m_btnSDSOnOff->EnableWindow(TRUE);
		}
		break;
	case 2 :
		{
			// disable
			m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 227, 65, 255, 93);
			m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 227, 65, 255, 93);
			m_btnSDSOnOff->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 227, 65, 255, 93);
			m_btnSDSOnOff->ShowWindow(TRUE);
			m_btnSDSOnOff->EnableWindow(FALSE);
		}
		break;
	}
}

void _XWindow_JinStatus::SetClassIcon()
{
	int classid = g_pLocalUser->m_CharacterInfo.Get_cClass();

	if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG)
	{
		if(classid == 7)	// 섭혼녀
			classid = 4;
	}
	if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM)
	{
		if(classid == 8)	// 철갑귀
			classid = 4;
	}
	
	if( m_pClassIcon )
		m_pClassIcon->SetClipRect(2+32*classid, 2, 32+32*classid, 32);
}

void _XWindow_JinStatus::ShowWindow( BOOL show )
{
	//author : 양희왕
	if(m_pFireImage)
		m_pFireImage->ShowWindow(show);

	_XWindow::ShowWindow( show );
}