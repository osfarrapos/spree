// XWindow_FSWindow.cpp: implementation of the XWindow_FSWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_WorldMinimap.h"
#include "XWindow_QSlot.h"
#include "XWindow_FSWindow.h"
#include "XWindow_CInfo.h"
#include "XSR_STRINGHEADER.H"

extern void __stdcall _XCallback_FS_RunButton( void );
extern void __stdcall _XCallback_FS_WalkButton( void );
extern void __stdcall _XCallback_FS_CatchButton( void );
extern void __stdcall _XCallback_FS_UserInfoButton( void );
extern void __stdcall _XCallback_FS_MinimapButton( void );
extern void __stdcall _XCallback_FS_QuestButton( void );
extern void __stdcall _XCallback_FS_BattleModeButton( void );
extern void __stdcall _XCallback_FS_PeaceModeButton( void );
extern void __stdcall _XCallback_FS_InventoryButton( void );
extern void __stdcall _XCallback_FS_SkillListButton( void );
extern void __stdcall _XCallback_FS_ComminityButton( void );
extern void __stdcall _XCallback_FS_OptionButton( void );
extern void __stdcall _XCallback_FS_QuickSlotButton( void );
extern void __stdcall _XCallback_FS_IconizeButton( void );
extern void __stdcall _XCallback_FS_ShortenButton( void );
extern void __stdcall _XCallback_FS_ChangeBarButton( void );

extern void __stdcall _XStickGaugeCallback_LifePower( POINT& winpos, SIZE& winsize, FLOAT& factor );
extern void __stdcall _XStickGaugeCallback_ForcePower( POINT& winpos, SIZE& winsize, FLOAT& factor );
extern void __stdcall _XStickGaugeCallback_ConcentrationPower( POINT& winpos, SIZE& winsize, FLOAT& factor );


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_FSWindow::_XWindow_FSWindow()
{	
	m_pLifePowerBorder	= NULL;
	m_pForcePowerBorder		= NULL;	
	m_pConcentrationPowerBorder		= NULL;	
}

_XWindow_FSWindow::~_XWindow_FSWindow()
{

}


BOOL _XWindow_FSWindow::Initialize( void )
{	
	int gaugeimageindex = g_MainInterfaceTextureArchive.FindResource( "gaugebar_common.tga" );

	_XSTICKGAUGE_STRUCTURE	gaugestruct = 
	{
		TRUE, { 19, 19 }, { 174, 6 },	
		gaugeimageindex, -1, &g_MainInterfaceTextureArchive
	};

	m_pLifePowerBorder = new _XStickGauge;
	m_pLifePowerBorder->Create( gaugestruct );	
	m_pLifePowerBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  18, 174,  24 );
	m_pLifePowerBorder->SetCallBackFunction_FactorDraw( _XStickGaugeCallback_LifePower );
	m_pLifePowerBorder->SetFactorDraw(TRUE);
	InsertChildObject( m_pLifePowerBorder );
	
	gaugestruct.position.x		=	19; 
	gaugestruct.position.y		=	28;
	gaugestruct.windowsize.cx	=	174;
	gaugestruct.windowsize.cy	=	6;
	m_pForcePowerBorder = new _XStickGauge;
	m_pForcePowerBorder->Create( gaugestruct );	
	m_pForcePowerBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  45, 174, 51 );
	m_pForcePowerBorder->SetCallBackFunction_FactorDraw( _XStickGaugeCallback_ForcePower );
	m_pForcePowerBorder->SetFactorDraw(TRUE);
	InsertChildObject( m_pForcePowerBorder );

	gaugestruct.position.x		=	19;
	gaugestruct.position.y		=	37;
	gaugestruct.windowsize.cx	=	174;
	gaugestruct.windowsize.cy	=	6;
	m_pConcentrationPowerBorder = new _XStickGauge;
	m_pConcentrationPowerBorder->Create( gaugestruct );	
	m_pConcentrationPowerBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 9, 174, 15 );
	m_pConcentrationPowerBorder->SetCallBackFunction_FactorDraw( _XStickGaugeCallback_ConcentrationPower );
	m_pConcentrationPowerBorder->SetFactorDraw(TRUE);
	InsertChildObject( m_pConcentrationPowerBorder );

	int fswinxpos = _XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_FSWINDOWXPOS, (gnWidth>>1)-70-212, TRUE );
	if( fswinxpos < 0 ) fswinxpos = 0;
	int fswinypos = _XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_FSWINDOWYPOS, 0, TRUE );
	if( fswinypos < 0 ) fswinypos = 0;
	MoveWindow( fswinxpos, fswinypos );
	CheckOutside();
	
	return TRUE;
}

void _XWindow_FSWindow::MoveWindow( int X, int Y )
{	
	_XWindow::MoveWindow( X, Y );	
}

void _XWindow_FSWindow::SetDefaultPosition( void )
{		
	MoveWindow( (gnWidth>>1)-70-212, 0 );
	CheckOutside();
}

void _XWindow_FSWindow::SaveWindowProperty( void )
{	
	_XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_FSWINDOWXPOS,   GetWindowPos().x );
	_XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_FSWINDOWYPOS,   GetWindowPos().y );	
}

void _XWindow_FSWindow::DestroyWindow( void )
{	
	_XWindow::DestroyWindow();
}

#define _XDEF_GAUGEINCREASEFACTOR	5

void _XWindow_FSWindow::Draw( _XGUIObject*& pfocusobject )
{			
	_XWindow::Draw(pfocusobject);	

	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	TCHAR levelstring[16];
	memset(levelstring, 0, sizeof(TCHAR)*16);
	strncpy(levelstring, g_StrManager.GetString(ID_STRING_SELECTCHARACTER_SUBLEVEL), 15);
	
	TCHAR szText[64];
	// 문파 / 별호 표시 -> 지금은 별호 정보가 없으므로 이름으로 출력함. 후에 수정 요망
	if( strlen( g_pLocalUser->m_CharacterInfo.m_NickName ) == 0 )
	{
		sprintf( szText, "%s %s", g_JoinGroupName[ g_pLocalUser->m_CharacterInfo.groupindex ] , g_pLocalUser->m_CharacterInfo.charactername );
	}
	else
	{
		sprintf( szText, "%s %s %s", g_JoinGroupName[ g_pLocalUser->m_CharacterInfo.groupindex ] , g_pLocalUser->m_CharacterInfo.m_NickName, g_pLocalUser->m_CharacterInfo.charactername );
	}
	int textwidth = g_XBaseFont.GetWidth( szText );
	g_XBaseFont.SetColor( _XSC_DEFAULT );
	g_XBaseFont.Puts( m_WindowPosition.x + ( (m_WindowSize.cx >> 1) - (textwidth >> 1) ), m_WindowPosition.y + 3, szText );
	g_XBaseFont.Flush();
}

void _XWindow_FSWindow::SetParameter( void )
{			
	if( g_pLocalUser->m_CharacterInfo.max_lifepower )
	{
		short maxlife = 0;
		if(g_pLocalUser->m_MaxLifePower.applyCount > 0 || g_pLocalUser->m_MaxLifePower.applyCountAdditional > 0)
		{
			maxlife = g_pLocalUser->m_MaxLifePower.result;
		}
		else
		{
			maxlife = g_pLocalUser->m_CharacterInfo.max_lifepower;
		}
		g_pLocalUser->LifePowerPercentage	=	(int)(((FLOAT)g_pLocalUser->m_CharacterInfo.current_lifepower / (FLOAT)maxlife) * 100.0f);
	}
	else
		g_pLocalUser->LifePowerPercentage	=	0;
	
	if( g_pLocalUser->m_CharacterInfo.max_forcepower )		
		g_pLocalUser->ForcePowerPercentage	=	(int)((FLOAT)(g_pLocalUser->m_CharacterInfo.current_forcepower / (FLOAT)g_pLocalUser->m_CharacterInfo.max_forcepower) * 100.0f);
	else
		g_pLocalUser->ForcePowerPercentage	=	0;
		
	if(g_pLocalUser->m_CharacterInfo.max_concentrationpower)
	   g_pLocalUser->ConcentrationPowerPercentage	=	(int)(((FLOAT)g_pLocalUser->m_CharacterInfo.current_concentrationpower / (FLOAT)g_pLocalUser->m_CharacterInfo.max_concentrationpower) * 100.0f);
	else
	   g_pLocalUser->ConcentrationPowerPercentage	=	0;
	
	m_pLifePowerBorder->SetGaugeFactor( g_pLocalUser->LifePowerPercentage );
	m_pForcePowerBorder->SetGaugeFactor( g_pLocalUser->ForcePowerPercentage );
	m_pConcentrationPowerBorder->SetGaugeFactor( g_pLocalUser->ConcentrationPowerPercentage );

	_XWindow_CInfo* pCIwindow = (_XWindow_CInfo*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHARACTERINFO );
	if( pCIwindow )
	{
		pCIwindow->SetParameter();
	}
	
	//2004.07.05->oneway48 insert
	if(g_pLocalUser->m_ViewBattleGauge)
	{
		if(g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
		{		
			g_pLocalUser->m_MiniHpBorder.SetGaugeFactor(g_pLocalUser->LifePowerPercentage);
		}
	}
}


BOOL _XWindow_FSWindow::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	if(pfocusobject == this)
	{
		if(!mousestate->bButton[0])
		{
			if(g_pLocalUser->m_AttackTargetObjectType != _XPICKINGTARGET_SELF)
				g_pLocalUser->SetAttackTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
		}
	}

	if( !_XWindow::Process(pfocusobject) ) return FALSE;

	return TRUE;
}

