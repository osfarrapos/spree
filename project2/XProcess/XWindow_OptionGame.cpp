// XWindow_OptionGame.cpp: implementation of the _XWindow_OptionGame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_OptionGame.h"
#include "XSR_STRINGHEADER.H"

extern void _XDrawEdgedSolidBar( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color );


void __stdcall _XCallback_OP_RANKHIDING(  POINT pos, SIZE size  )
{		
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_OPTION_2555), _XSC_INFORMATION );
}
void __stdcall _XCallback_OG_KEYTAPEA(  POINT pos, SIZE size  )
{
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3236), _XSC_INFORMATION );//단축창 숫자키 변경
}
void __stdcall _XCallback_OG_KEYTAPEB(  POINT pos, SIZE size  )
{		
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3237), _XSC_INFORMATION );//단축창 기능키 변경
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_OptionGame::_XWindow_OptionGame()
{
	m_pGameGuideButton					= NULL;
	m_pUserNameTooltipButton			= NULL;
	m_pMyNameTooltipButton				= NULL;
	m_pItemNameTooltipButton			= NULL;
	m_RankHidingButton					= NULL;
	
	m_pCameraQuaterViewButton			= NULL;
	m_pCameraUseFirstPersonViewButton	= NULL;

	m_pViewBattleGaugeButton			= NULL;
	m_pViewMonsterGaugeButton			= NULL;
	
	m_pDenyMatchButton					= NULL;
	m_pChattingAutoClose				= NULL;
	
#ifdef _XDEF_KEYCHANGE_20070115
	m_pCheckBtnKeyTypeA					= NULL;	// 단축키가 숫자키
	m_pCheckBtnKeyTypeB					= NULL;	// 단축키가 Function 키
#endif
	
#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI		// 의상 교체 시스템
	m_pSelectClothesButton				= NULL;
#endif
}

_XWindow_OptionGame::~_XWindow_OptionGame()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Graphic option
BOOL _XWindow_OptionGame::Initialize( void )
{
	SetDrawBorder( FALSE );
	
	int optionmenuinterface = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	
	_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, { 243, 20 }, 
											{ 14, 14 }, 
											_XDEF_OPTION_GAME_GAMEGUIDE,
											optionmenuinterface,optionmenuinterface,
											&g_MainInterfaceTextureArchive };
	
	m_pGameGuideButton= new _XCheckButton;
	m_pGameGuideButton->Create( checkbtnstruct );	
	m_pGameGuideButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pGameGuideButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_ViewGuide = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWGUIDE, TRUE, TRUE );
	m_pGameGuideButton->SetCheck( g_ViewGuide );
	m_pGameGuideButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pGameGuideButton );

	checkbtnstruct.position.y = 40;
	checkbtnstruct.commandid = _XDEF_OPTION_GAME_VIEWUSERTOOLTIP;
	m_pUserNameTooltipButton= new _XCheckButton;
	m_pUserNameTooltipButton->Create( checkbtnstruct );		
	m_pUserNameTooltipButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pUserNameTooltipButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_ViewUserNameTooltip = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_ALWAYSVIEWTOOLTIP, TRUE, TRUE );
	m_pUserNameTooltipButton->SetCheck( g_ViewUserNameTooltip );
	m_pUserNameTooltipButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pUserNameTooltipButton );

	checkbtnstruct.position.y = 60;
	checkbtnstruct.commandid = _XDEF_OPTION_GAME_VIEWMYTOOLTIP;
	m_pMyNameTooltipButton= new _XCheckButton;
	m_pMyNameTooltipButton->Create( checkbtnstruct );		
	m_pMyNameTooltipButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pMyNameTooltipButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_ViewMyNameTooltip = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWMYTOOLTIP, TRUE, TRUE );
	m_pMyNameTooltipButton->SetCheck( g_ViewMyNameTooltip );
	m_pMyNameTooltipButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pMyNameTooltipButton );

	checkbtnstruct.position.y = 80;
	checkbtnstruct.commandid = _XDEF_OPTION_GAME_VIEWITEMTOOLTIP;
	m_pItemNameTooltipButton= new _XCheckButton;
	m_pItemNameTooltipButton->Create( checkbtnstruct );		
	m_pItemNameTooltipButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pItemNameTooltipButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_ViewItemNameTooltip = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWITEMTOOLTIP, TRUE, TRUE );
	m_pItemNameTooltipButton->SetCheck( g_ViewItemNameTooltip );
	m_pItemNameTooltipButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pItemNameTooltipButton );

	checkbtnstruct.position.y = 100;
	checkbtnstruct.commandid = _XDEF_OPTION_GAME_RANKHIDING;
	m_RankHidingButton= new _XCheckButton;
	m_RankHidingButton->Create( checkbtnstruct );	
	m_RankHidingButton->SetCallBackFunction_ToolTip( _XCallback_OP_RANKHIDING );
	m_RankHidingButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_RankHidingButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	m_RankHidingButton->SetCheck( FALSE );
	m_RankHidingButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_RankHidingButton );

	checkbtnstruct.position.y = 120;
	checkbtnstruct.commandid = _XDEF_OPTION_GAME_DENYMATCH;
	m_pDenyMatchButton = new _XCheckButton;
	m_pDenyMatchButton->Create(checkbtnstruct);
	m_pDenyMatchButton->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45);
	m_pDenyMatchButton->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 216, 0, 230, 14);
	g_DenyMatch = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_DENYMATCH, FALSE, TRUE );
	m_pDenyMatchButton->SetButtonSwapMode(FALSE);
	m_pDenyMatchButton->SetCheck(g_DenyMatch);
	m_pDenyMatchButton->ShowWindow( TRUE );
	InsertChildObject(m_pDenyMatchButton);
	
	BOOL usequaterview = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEQUATERVIEW, FALSE, TRUE );
	g_LodTerrain.m_3PCamera.SetQuaterViewMode( usequaterview );

	checkbtnstruct.position.y = 162;
	checkbtnstruct.commandid = _XDEF_OPTION_GAME_CAMERAQUATERVIEW;
	m_pCameraQuaterViewButton= new _XCheckButton;
	m_pCameraQuaterViewButton->Create( checkbtnstruct );		
	m_pCameraQuaterViewButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pCameraQuaterViewButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );	
	m_pCameraQuaterViewButton->SetCheck( usequaterview );	
	m_pCameraQuaterViewButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pCameraQuaterViewButton );

	BOOL enablefpv = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEFIRSTPERSONVIEW, TRUE, TRUE );
	g_LodTerrain.m_3PCamera.SetEnableFirstPersonView( enablefpv );

	checkbtnstruct.position.y = 182;
	checkbtnstruct.commandid = _XDEF_OPTION_GAME_USEFIRSTPERSONVIEW;
	m_pCameraUseFirstPersonViewButton= new _XCheckButton;
	m_pCameraUseFirstPersonViewButton->Create( checkbtnstruct );		
	m_pCameraUseFirstPersonViewButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pCameraUseFirstPersonViewButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );	
	m_pCameraUseFirstPersonViewButton->SetButtonSwapMode( FALSE );
	m_pCameraUseFirstPersonViewButton->SetCheck( enablefpv );
	InsertChildObject( m_pCameraUseFirstPersonViewButton );

	checkbtnstruct.position.y = 202;
	checkbtnstruct.commandid = _XDEF_OPTION_GAME_VIEWBATTLEGAUGE;
	m_pViewBattleGaugeButton = new _XCheckButton;
	m_pViewBattleGaugeButton->Create(checkbtnstruct);
	m_pViewBattleGaugeButton->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45);
	m_pViewBattleGaugeButton->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 216, 0, 230, 14);
	g_ViewMyBattleGauge = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MINIHPBAR, TRUE, TRUE );
	m_pViewBattleGaugeButton->SetButtonSwapMode(FALSE);
	m_pViewBattleGaugeButton->SetCheck(g_ViewMyBattleGauge);
	InsertChildObject(m_pViewBattleGaugeButton);

	checkbtnstruct.position.y = 222;
	checkbtnstruct.commandid = _XDEF_OPTION_GAME_VIEWMONSTERGAUGE;
	m_pViewMonsterGaugeButton = new _XCheckButton;
	m_pViewMonsterGaugeButton->Create(checkbtnstruct);
	m_pViewMonsterGaugeButton->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45);
	m_pViewMonsterGaugeButton->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 216, 0, 230, 14);
	g_ViewMonsterBattleGauge = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWMONSTERHP, FALSE, TRUE );
	m_pViewMonsterGaugeButton->SetButtonSwapMode(FALSE);
	m_pViewMonsterGaugeButton->SetCheck(g_ViewMonsterBattleGauge);
	m_pViewMonsterGaugeButton->ShowWindow( TRUE );
	InsertChildObject(m_pViewMonsterGaugeButton);

	checkbtnstruct.position.y = 242;
	checkbtnstruct.commandid = _XDEF_OPTION_GAME_CHATTINGAUTOCLOSE;
	m_pChattingAutoClose = new _XCheckButton;
	m_pChattingAutoClose->Create(checkbtnstruct);
	m_pChattingAutoClose->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45);
	m_pChattingAutoClose->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 216, 0, 230, 14);
	m_pChattingAutoClose->SetButtonSwapMode(FALSE);
	m_pChattingAutoClose->SetCheck(g_ChattingAutoClose);
	m_pChattingAutoClose->ShowWindow( TRUE );
	InsertChildObject(m_pChattingAutoClose);

#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI		// 의상 교체 시스템
	checkbtnstruct.position.y += 20;
	checkbtnstruct.commandid = _XDEF_OPTION_GAME_SELECTCLOTHES;
	m_pSelectClothesButton = new _XCheckButton;
	m_pSelectClothesButton->Create(checkbtnstruct);
	m_pSelectClothesButton->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45);
	m_pSelectClothesButton->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 216, 0, 230, 14);
	m_pSelectClothesButton->SetButtonSwapMode(FALSE);
	m_pSelectClothesButton->SetCheck(g_bSelectClothesMask);
	m_pSelectClothesButton->ShowWindow( TRUE );
	InsertChildObject(m_pSelectClothesButton);
#endif
	
#ifdef _XDEF_KEYCHANGE_20070115
	checkbtnstruct.position.y += 20;
	checkbtnstruct.position.x -= 22;
	checkbtnstruct.commandid = _XDEF_OPTION_GAME_KEYTYPE_A;
	m_pCheckBtnKeyTypeA = new _XCheckButton;
	m_pCheckBtnKeyTypeA->Create(checkbtnstruct);
	m_pCheckBtnKeyTypeA->SetCallBackFunction_ToolTip( _XCallback_OG_KEYTAPEA );
	m_pCheckBtnKeyTypeA->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45);
	m_pCheckBtnKeyTypeA->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 216, 0, 230, 14);
	#ifdef _XTAIWANESE
		g_bGameOptionKeyTypeASelected= g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_KEYTYPEASELECTED, TRUE, TRUE );
	#else
		g_bGameOptionKeyTypeASelected= g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_KEYTYPEASELECTED, TRUE, TRUE );
	#endif
	m_pCheckBtnKeyTypeA->SetButtonSwapMode(FALSE);
	m_pCheckBtnKeyTypeA->SetCheck(g_bGameOptionKeyTypeASelected);
	m_pCheckBtnKeyTypeA->ShowWindow( TRUE );
	InsertChildObject(m_pCheckBtnKeyTypeA);

	checkbtnstruct.commandid = _XDEF_OPTION_GAME_KEYTYPE_B;
	checkbtnstruct.position.x = 243;
	m_pCheckBtnKeyTypeB = new _XCheckButton;
	m_pCheckBtnKeyTypeB->Create(checkbtnstruct);
	m_pCheckBtnKeyTypeB->SetCallBackFunction_ToolTip( _XCallback_OG_KEYTAPEB );
	m_pCheckBtnKeyTypeB->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45);
	m_pCheckBtnKeyTypeB->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 216, 0, 230, 14);
	#ifdef _XTAIWANESE
		g_bGameOptionKeyTypeASelected= g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_KEYTYPEASELECTED, TRUE, TRUE );
	#else
		g_bGameOptionKeyTypeASelected= g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_KEYTYPEASELECTED, FALSE, TRUE );
	#endif
	m_pCheckBtnKeyTypeB->SetButtonSwapMode(FALSE);
	m_pCheckBtnKeyTypeB->SetCheck(!g_bGameOptionKeyTypeASelected);
	m_pCheckBtnKeyTypeB->ShowWindow( TRUE );
	InsertChildObject(m_pCheckBtnKeyTypeB);
	
#endif

	return TRUE;
}


void _XWindow_OptionGame::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
	
}


BOOL _XWindow_OptionGame::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}	


void _XWindow_OptionGame::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		_XDrawEdgedSolidBar( m_WindowPosition.x + 6, m_WindowPosition.y + 9,  m_WindowPosition.x + 269, m_WindowPosition.y + 149, 0x80000000 );
		_XDrawEdgedSolidBar( m_WindowPosition.x + 6, m_WindowPosition.y + 153,  m_WindowPosition.x + 269, m_WindowPosition.y +  297, 0x80000000 );
	}

	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
		
	g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 180, 180, 180 ) );

	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 21, _XGETINTERFACETEXT(ID_STRING_OPTION_2278) );	
	
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 40, _XGETINTERFACETEXT(ID_STRING_OPTION_2279) );
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 60, _XGETINTERFACETEXT(ID_STRING_OPTION_2280) );
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 80, _XGETINTERFACETEXT(ID_STRING_OPTION_2281) );
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 100, _XGETINTERFACETEXT(ID_STRING_OPTION_2282) );
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 120, _XGETINTERFACETEXT(ID_STRING_OPTION_2283) );
	
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 164, _XGETINTERFACETEXT(ID_STRING_OPTION_QUATERVIEW) );		// _T("카메라 쿼터뷰 고정")
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 183, _XGETINTERFACETEXT(ID_STRING_OPTION_FPSVIEW) );		// _T("카메라 1인칭 확대")

	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 203, _XGETINTERFACETEXT(ID_STRING_OPTION_LIFESHOW));
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 223, _XGETINTERFACETEXT(ID_STRING_OPTION_2284));
	
	int height = 243;
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + height, _XGETINTERFACETEXT(ID_STRING_OPTION_3218));
	height += 20;

#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
	g_XBaseFont->Puts(m_WindowPosition.x + 21, m_WindowPosition.y + height, _XGETINTERFACETEXT(ID_STRING_NEW_3810)); //치장의상 보기
	height += 20;
#endif
	
#ifdef _XDEF_KEYCHANGE_20070115
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + height, _XGETINTERFACETEXT(ID_STRING_NEW_3238) ); // 키 배치 방식 변경
	height += 20;
#endif
	g_XBaseFont->Flush();
}

void _XWindow_OptionGame::ShowWindow(BOOL show)
{

#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI		// 의상 교체 시스템
	if(show)
		m_pSelectClothesButton->SetCheck(g_bSelectClothesMask);
#endif
	
	_XWindow::ShowWindow(show);
}