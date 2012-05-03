// XWindow_MainMenu.cpp: implementation of the _XWindow_MainMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "xwindow_messenger.h"
#include "XWindow_Email.h"
#include "XWindow_GuideLine.h"
#include "XWindow_MainMenu.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

/*
void __stdcall _XCallback_Battle( POINT pos, SIZE size )
{
	TCHAR messagestring[32];
	memset(messagestring, 0, sizeof(TCHAR)*32);
	
	if( g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE )
	{
		strncpy(messagestring, _T("평화모드 Tab"), 31);
	}
	else
	{
		strncpy(messagestring, _T("전투모드 Tab"), 31);
	}
	g_InterfaceTooltip.SetToolTipString( pos.x+16, pos.y, messagestring, _XSC_INFORMATION );	
}

// 2004.06.07->oneway48 modify : 메인메뉴 툴팁 단축기 수정
void __stdcall _XCallback_Life( POINT pos, SIZE size )
{
	TCHAR messagestring[32];
	memset(messagestring, 0, sizeof(TCHAR)*32);
	strncpy(messagestring, _T("생활"), 31);
	g_InterfaceTooltip.SetToolTipString( pos.x+16, pos.y, messagestring, _XSC_INFORMATION );	
}


void __stdcall _XCallback_CharacterInfo( POINT pos, SIZE size )
{
	TCHAR messagestring[32];
	memset(messagestring, 0, sizeof(TCHAR)*32);
	//strncpy(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINMENU_CINFO), 31);
	strncpy(messagestring, _T("인물(C)"), 31);
	g_InterfaceTooltip.SetToolTipString( pos.x+16, pos.y, messagestring, _XSC_INFORMATION );	
}

void __stdcall _XCallback_Skill( POINT pos, SIZE size )
{
	TCHAR messagestring[32];
	memset(messagestring, 0, sizeof(TCHAR)*32);
	//strncpy(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINMENU_SKILL), 31);
	strncpy(messagestring, _T("무공(S)"), 31);
	g_InterfaceTooltip.SetToolTipString( pos.x+16, pos.y, messagestring, _XSC_INFORMATION );	
}

void __stdcall _XCallback_Minimap( POINT pos, SIZE size )
{
	TCHAR messagestring[32];
	memset(messagestring, 0, sizeof(TCHAR)*32);
	//strncpy(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINMENU_MINIMAP), 31);
	strncpy(messagestring, _T("지도(M)"), 31);
	g_InterfaceTooltip.SetToolTipString( pos.x+16, pos.y, messagestring, _XSC_INFORMATION );	
}

void __stdcall _XCallback_Community( POINT pos, SIZE size )
{
	TCHAR messagestring[32];
	memset(messagestring, 0, sizeof(TCHAR)*32);
	//strncpy(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINMENU_PARTY), 31);
	strncpy(messagestring, _T("군웅(N)"), 31);
	g_InterfaceTooltip.SetToolTipString( pos.x+16, pos.y, messagestring, _XSC_INFORMATION );	
}

void __stdcall _XCallback_Messenger( POINT pos, SIZE size )
{
	TCHAR messagestring[32];
	memset(messagestring, 0, sizeof(TCHAR)*32);
	strncpy(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINMENU_MESSENGER), 31);
	g_InterfaceTooltip.SetToolTipString( pos.x+16, pos.y, messagestring, _XSC_INFORMATION );	
}

void __stdcall _XCallback_Quest( POINT pos, SIZE size )
{
	TCHAR messagestring[32];
	memset(messagestring, 0, sizeof(TCHAR)*32);
	//strncpy(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINMENU_QUEST), 31);
	strncpy(messagestring, _T("강호풍운록(Q)"), 31);
	g_InterfaceTooltip.SetToolTipString( pos.x+16, pos.y, messagestring, _XSC_INFORMATION );	
}

void __stdcall _XCallback_SystemMenu( POINT pos, SIZE size )
{
	TCHAR messagestring[32];
	memset(messagestring, 0, sizeof(TCHAR)*32);
	//strncpy(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINMENU_OPTION), 31);
	strncpy(messagestring, _T("설정(O)"), 31);
	g_InterfaceTooltip.SetToolTipString( pos.x+16, pos.y, messagestring, _XSC_INFORMATION );	
}
*/
// New CallBack Func-----------------------------------------------------------------------------=
void __stdcall _XCallback_MainMenu( POINT pos, SIZE size )
{
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_MAINMENU_ALLINFO), _XSC_INFORMATION );		// _T("전체정보")
}

void __stdcall _XCallback_Inventory( POINT pos, SIZE size )
{
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_MAINMENU_INVENTORY), _XSC_INFORMATION );	// _T("행낭(I)")
}

void __stdcall _XCallback_Option( POINT pos, SIZE size )
{
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_MAINMENU_OPTION), _XSC_INFORMATION );		// _T("설정(O)")
}

void __stdcall _XCallback_HelpDesk( POINT pos, SIZE size )
{
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_MAINMENU_HELP), _XSC_INFORMATION );		// _T("도움말")
}

void __stdcall _XCallback_Messenger( POINT pos, SIZE size )
{
 	if( g_bShowItemMallWindow ) return;
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);	

	if( pMessenger_Window->m_CurrentUserStatus >= 0 && pMessenger_Window->m_CurrentUserStatus < 5 )
	{
		TCHAR messagestring[128];
		sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAINMENU_MESSANGER), g_UserStatus[pMessenger_Window->m_CurrentUserStatus] );		// "메신져 : %s"
		g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, messagestring, _XSC_INFORMATION );	
	}
}

void __stdcall _XCallback_EMail( POINT pos, SIZE size )
{	
 	if( g_bShowItemMallWindow ) return;
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);

	bool flag = false;
	for( int i = 0; i < 20; i++ )
	{
		if( pEmail_Window->m_EmailList[i].uiIndex > 0 && pEmail_Window->m_EmailList[i].check )
		{
			flag = true;
			break;
		}
	}

	TCHAR messagestring[128];	
	if( flag )	// 새편지 있음
	{
		strcpy( messagestring, _XGETINTERFACETEXT(ID_STRING_MAINMENU_NEWMAIL));	// _T("전서구 : 새 편지 있음")
	}
	else
	{
		strcpy( messagestring, _XGETINTERFACETEXT(ID_STRING_MAINMENU_EMAIL));	// _T("전서구")
	}		
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, messagestring, _XSC_INFORMATION );
}

#ifdef _XTS_ITEMMALLBROWSER
void __stdcall _XCallback_CashMall( POINT pos, SIZE size )
{
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15,_XGETINTERFACETEXT(ID_STRING_NEW_2977), _XSC_INFORMATION );		// _T("캐쉬아이템상점") 
}

void __stdcall _XCallback_CashBox( POINT pos, SIZE size )
{
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_2978), _XSC_INFORMATION );		// _T("캐쉬아이템보관함") 
}
#endif
// ----------------------------------------------------------------------------------------------=

// modify end

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MainMenu::_XWindow_MainMenu()
{
	m_pMessengerStatusStatic = NULL;
	m_MainButtonRoundEdgeAlphaIncrease = TRUE;
	m_MainButtonRoundEdgeAlphaLevel = 0;

	m_MeditationButton = NULL;
	m_MeditationIndex = -1;

	m_PrevDrawTime = 0;
	m_EffectFrame = 0.f;
	m_bDrawMedEffect = FALSE;
	m_bDrawMedEffectInfinite = FALSE;
}

_XWindow_MainMenu::~_XWindow_MainMenu()
{
	
}


BOOL _XWindow_MainMenu::Initialize( void )
{
	m_borderstatic.SetClipRect( 0, 0, 182, 93 );
	
	int mainmenuimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_button.tga" );

	_XBTN_STRUCTURE mainmenubtnstruct = { TRUE, { 116, 24 }, { 58, 58 }, 
												_XDEF_MAINMENU_MAINWINDOW,
												mainmenuimageindex,
												mainmenuimageindex, 
												mainmenuimageindex,
												&g_MainInterfaceTextureArchive };
		
	m_pMainButton = new _XButton;
	m_pMainButton->Create( mainmenubtnstruct );	
	m_pMainButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  198,   0, 256,   58 );
	m_pMainButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,   198,  58, 256,  116 );
	m_pMainButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,   198,  
		174, 256, 232 );
	m_pMainButton->SetCallBackFunction_ToolTip( _XCallback_MainMenu );
	InsertChildObject(m_pMainButton);
	
	m_MainButtonRoundEdge.Create( m_WindowPosition.x+116, m_WindowPosition.y+25, 
								  m_WindowPosition.x+116+58, m_WindowPosition.y+25+58, 
								  &g_MainInterfaceTextureArchive, mainmenuimageindex );	
	m_MainButtonRoundEdge.SetClipRect( 198,  116, 256, 174 );	

	_XBTN_STRUCTURE inventorybtnstruct = { TRUE, { 77, 43 }, { 32, 32 }, 
										   _XDEF_MAINMENU_INVENTORY,
										   mainmenuimageindex,
										   mainmenuimageindex, 
										   mainmenuimageindex,
										  &g_MainInterfaceTextureArchive };
	
	_XButton* pInventoryButton = new _XButton;
	pInventoryButton->Create( inventorybtnstruct );	
	pInventoryButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  0,  167, 32, 199 );
	pInventoryButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,   35, 167, 66, 199 );
	pInventoryButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,   71, 167, 103,199 );
	pInventoryButton->SetCallBackFunction_ToolTip( _XCallback_Inventory );
	InsertChildObject(pInventoryButton);


	_XBTN_STRUCTURE optionbtnstruct = { TRUE, { 100, 71 }, { 21, 21 }, 
											_XDEF_MAINMENU_OPTION,
											mainmenuimageindex,
											mainmenuimageindex, 
											mainmenuimageindex,
											&g_MainInterfaceTextureArchive };
	
	_XButton* pOptionButton = new _XButton;
	pOptionButton->Create( optionbtnstruct );	
	pOptionButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  108, 177, 129, 198 );
	pOptionButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,   132, 177, 153, 198 );
	pOptionButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,   156, 177, 177, 198 );
	pOptionButton->SetCallBackFunction_ToolTip( _XCallback_Option );
	InsertChildObject(pOptionButton);

	_XBTN_STRUCTURE hedekbtnstruct = { TRUE, { 165, 76 }, { 17, 17 }, 
										_XDEF_MAINMENU_HEDEK,
										mainmenuimageindex,
										mainmenuimageindex, 
										mainmenuimageindex,
										&g_MainInterfaceTextureArchive };
	
	_XButton* pHedekButton = new _XButton;
	pHedekButton->Create( hedekbtnstruct );	
	pHedekButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  2,  147, 20, 165 );
	pHedekButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,   22, 147, 40, 165 );
	pHedekButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,   42, 147, 60, 165 );
	pHedekButton->SetCallBackFunction_ToolTip( _XCallback_HelpDesk );
	InsertChildObject(pHedekButton);


	_XBTN_STRUCTURE messengerbtnstruct = { TRUE, { 50, 53 }, { 20, 17 }, 
											_XDEF_MAINMENU_MESSENGER,
											mainmenuimageindex,
											mainmenuimageindex, 
											mainmenuimageindex,
											&g_MainInterfaceTextureArchive };
	
	_XButton* pMessengerButton = new _XButton;
	pMessengerButton->Create( messengerbtnstruct );	
	pMessengerButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  0,  94, 20, 111 );
	pMessengerButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,   0,  111, 20, 128 );
	pMessengerButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,   0,  128, 20, 145 );
	pMessengerButton->SetCallBackFunction_ToolTip( _XCallback_Messenger );
	InsertChildObject(pMessengerButton);

	_XBTN_STRUCTURE emailbtnstruct = { TRUE, { 50, 72 }, { 20, 17 }, 
									_XDEF_MAINMENU_EMAIL,
									mainmenuimageindex,
									mainmenuimageindex, 
									mainmenuimageindex,
									&g_MainInterfaceTextureArchive };
	
	_XButton* pEmailButton = new _XButton;
	pEmailButton->Create( emailbtnstruct );
	pEmailButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  0,  94, 20, 111 );
	pEmailButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,   0,  111, 20, 128 );
	pEmailButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,   0,  128, 20, 145 );
	pEmailButton->SetCallBackFunction_ToolTip( _XCallback_EMail );
	InsertChildObject(pEmailButton);


#ifdef _XTS_ITEMMALLBROWSER
	_XBTN_STRUCTURE cashmallbtnstruct = { TRUE, { 32, 22 }, { 21, 21 }, _XDEF_MAINMENU_BTN_CASHMALL,
		mainmenuimageindex, mainmenuimageindex, mainmenuimageindex,&g_MainInterfaceTextureArchive };
	_XButton* pCashMallButton = new _XButton;
	pCashMallButton->Create( cashmallbtnstruct );
	pCashMallButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  24,  98, 45, 119 );
	pCashMallButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,   48,  99, 69, 120 );
	pCashMallButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,   72,  98, 93, 120 );
	pCashMallButton->SetCallBackFunction_ToolTip( _XCallback_CashMall );
	InsertChildObject(pCashMallButton);

	_XBTN_STRUCTURE cashboxbtnstruct = { TRUE, { 61, 18 }, { 21, 21 }, _XDEF_MAINMENU_BTN_CASHBOX,
		mainmenuimageindex, mainmenuimageindex, mainmenuimageindex,&g_MainInterfaceTextureArchive };
	_XButton* pCashBoxButton = new _XButton;
	pCashBoxButton->Create( cashboxbtnstruct );
	pCashBoxButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  24,  123, 45, 144 );
	pCashBoxButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,   48,  123, 69, 144 );
	pCashBoxButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,   72,  123, 93, 144 );
	pCashBoxButton->SetCallBackFunction_ToolTip( _XCallback_CashBox );
	InsertChildObject(pCashBoxButton);
	
	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{	
		pCashMallButton->EnableWindow(TRUE);
		pCashBoxButton->EnableWindow(TRUE);
	}
	// 일본 아이템몰 활성화 2008/3/10 kerygma : 현재 테스트 중이므로 내부 인증된 유저인 경우만 표시
	/*
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		extern BOOL CheckInnerTester( void );
		BOOL _xinnertester = CheckInnerTester();

		if(_xinnertester)
		{
		}
		else
		{
			pCashMallButton->EnableWindow(FALSE);
			pCashBoxButton->EnableWindow(FALSE);
		}
	}*/
	else
	{		
		#ifdef _XTESTSERVER
		#else
		pCashMallButton->EnableWindow(TRUE);
		pCashBoxButton->EnableWindow(TRUE);
		#endif
	}

#endif

	m_pMessengerIcon = new _XImageStatic;
//	m_pMessengerIcon->Create( 15, 53, 29, 68, &g_MainInterfaceTextureArchive, mainmenuimageindex );	
	m_pMessengerIcon->Create( 53, 54, 67, 68, &g_MainInterfaceTextureArchive, mainmenuimageindex );	
	m_pMessengerIcon->SetClipRect( 99, 95, 114, 111 );
	InsertChildObject(m_pMessengerIcon);

	m_pEmailIcon = new _XImageStatic;
	m_pEmailIcon->Create( 52, 75, 66, 85, &g_MainInterfaceTextureArchive, mainmenuimageindex );	
//	m_pEmailIcon->Create( 14, 74, 30, 85, &g_MainInterfaceTextureArchive, mainmenuimageindex );	
	m_pEmailIcon->SetClipRect( 97, 114, 114, 126 );
	InsertChildObject(m_pEmailIcon);

	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_main_unki_icon.tga");
	_XBTN_STRUCTURE medbtnstruct = 
	{	TRUE, { 15, 56 }, 
		{ 30, 30 }, 
		_XDEF_MAINMENU_MEDBUTTON,
		resourceindex,
		resourceindex, 
		resourceindex,
		&g_MainInterfaceTextureArchive 
	};
	m_MeditationButton = new _XButton;
	m_MeditationButton->Create(medbtnstruct);
	m_MeditationButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 1, 1, 31, 31);
	m_MeditationButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 1, 1, 31, 31);
	m_MeditationButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 1, 1, 31, 31);
	m_MeditationButton->EnableWindow(FALSE);
	InsertChildObject(m_MeditationButton);
	
	m_MedEffectImage.Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, resourceindex);
	m_MedEffectImage.SetClipRect(225, 33, 255, 63);

//	m_pMessengerStatusStatic = new _XImageStatic;
//	m_pMessengerStatusStatic->Create( 32, 54, 65, 67, &g_MainInterfaceTextureArchive, mainmenuimageindex );	
//	m_pMessengerStatusStatic->SetClipRect( 60, 113, 93, 127 );
//	InsertChildObject(m_pMessengerStatusStatic);
	
	/*
	int mainmenuwinxpos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MAINMENUWINDOWXPOS, gnWidth-183, TRUE );
	if( mainmenuwinxpos < 0 ) mainmenuwinxpos = 0;
	int mainmenuwinypos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MAINMENUWINDOWYPOS, gnHeight-94, TRUE );
	if( mainmenuwinypos < 0 ) mainmenuwinypos = 0;
	MoveWindow( mainmenuwinxpos, mainmenuwinypos );
	CheckOutside();
	*/

	g_WindowCloseRect.left   = m_WindowPosition.x;
	g_WindowCloseRect.top    = m_WindowPosition.y;
	g_WindowCloseRect.right  = m_WindowPosition.x+m_WindowSize.cx;
	g_WindowCloseRect.bottom = m_WindowPosition.y+m_WindowSize.cy;

	int resourceindex1 = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		resourceindex1 = g_MainInterfaceTextureArchive.FindResource( "us_tutorial_01.tga" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		resourceindex1 = g_MainInterfaceTextureArchive.FindResource( "rs_tutorial_01.tga" );
	}
	else
	{
		resourceindex1 = g_MainInterfaceTextureArchive.FindResource( "tutorial_01.tga" );
	}
	
	m_GuideLineImage.Create(0, 0, 45, 46, &g_MainInterfaceTextureArchive, resourceindex1);
	m_GuideLineImage.SetClipRect( 210, 84, 256, 131 );
	m_GuideLineImage.ShowWindow(FALSE);

	return TRUE;
}

void _XWindow_MainMenu::SetDefaultPosition( void )
{		
	MoveWindow( gnWidth-182, gnHeight-93 ); 
	CheckOutside();
}

void _XWindow_MainMenu::SaveWindowProperty( void )
{
	/*
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MAINMENUWINDOWXPOS,   GetWindowPos().x );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MAINMENUWINDOWYPOS,   GetWindowPos().y );		
	*/
}

void _XWindow_MainMenu::MoveWindow( int X, int Y )
{
	g_WindowCloseRect.left   = m_WindowPosition.x;
	g_WindowCloseRect.top    = m_WindowPosition.y;
	g_WindowCloseRect.right  = m_WindowPosition.x+m_WindowSize.cx;
	g_WindowCloseRect.bottom = m_WindowPosition.y+m_WindowSize.cy;

	_XWindow::MoveWindow( X, Y );

	m_MainButtonRoundEdge.MoveWindow( m_WindowPosition.x+116, m_WindowPosition.y+24 );
}

void _XWindow_MainMenu::DestroyWindow( void )
{	
	_XWindow::DestroyWindow();
}

void _XWindow_MainMenu::Draw( _XGUIObject*& pfocusobject )
{			
	if( !this->m_ShowWindow ) return;	
	_XWindow::Draw(pfocusobject);

	if( m_pMainButton != pfocusobject &&
		m_pMainButton->CheckMousePosition() )
	{
		if( m_MainButtonRoundEdgeAlphaIncrease )
		{
			m_MainButtonRoundEdgeAlphaLevel+=10;
			
			if( m_MainButtonRoundEdgeAlphaLevel > 255 )
			{
				m_MainButtonRoundEdgeAlphaLevel = 255;
				m_MainButtonRoundEdgeAlphaIncrease = FALSE;
			}
		}
		else
		{
			m_MainButtonRoundEdgeAlphaLevel-=10;
			
			if( m_MainButtonRoundEdgeAlphaLevel < 0 )
			{
				m_MainButtonRoundEdgeAlphaLevel = 0;
				m_MainButtonRoundEdgeAlphaIncrease = TRUE;
			}
		}
		
		m_MainButtonRoundEdge.m_FColor = D3DCOLOR_ARGB( m_MainButtonRoundEdgeAlphaLevel, 0xFF,0xFF,0xFF );		
		m_MainButtonRoundEdge.Draw( pfocusobject );
	}

	if(m_bDrawMedEffect)
	{
		DrawMedEffect(10);
	}
	else if(m_bDrawMedEffectInfinite)
	{
		DrawMedEffect(0);
	}

	if( m_GuideLineImage.GetShowStatus() )
	{
		m_GuideLineImage.Draw(m_WindowPosition.x+6, m_WindowPosition.y+46 );
	}

	DrawTooltip();
}

BOOL _XWindow_MainMenu::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if( !_XWindow::Process(pfocusobject) ) return FALSE;
		
	return TRUE;
}

void _XWindow_MainMenu::SetMessengerStatus( int currentuserstatus )
{			
	switch( currentuserstatus ) {
	case 0://로그오프 
		{
//			m_pMessengerStatusStatic->SetClipRect( 60, 113, 93, 127 );
		}
		break;
	case 1://로그인
		{
//			m_pMessengerStatusStatic->SetClipRect( 60, 96, 93, 110 );
		}
		break;
	default://기타
		{
//			m_pMessengerStatusStatic->SetClipRect( 60, 130, 93, 144 );
		}
		break;
	}
}

void _XWindow_MainMenu::SetMedButton(void)
{
	m_MeditationIndex = -1;

	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
	{
		if(g_pLocalUser->m_SkillList[i].m_sSkillID <= 0)
			continue;

		if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
		{
			if(g_pLocalUser->m_SkillList[i].m_cOnOff == 1)
			{
				if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->insideType > 0)
				{
					m_MeditationIndex = g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->insideType;					
					break;
				}
				else
				{
					// 운기조식 스킬이 아님
					m_MeditationIndex = 0;
				}
			}
			else
			{
				// Off 되어 있음
				m_MeditationIndex = 0;
			}
		}
	}

	int resourceindex = -1;
	RECT rect;

	if(g_pQuickSlot_Window)
	{
		g_pQuickSlot_Window->GetIconRect(m_MeditationIndex, resourceindex, rect, true, true);
	}

	if(resourceindex < 0)
	{
		resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_main_unki_icon.tga");
		rect.left = 1;
		rect.top = 1;
		rect.right = 31;
		rect.bottom = 31;
	}

	m_MeditationButton->SetButtonImage(resourceindex, resourceindex, resourceindex, &g_MainInterfaceTextureArchive);
	m_MeditationButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, rect.left, rect.top, rect.right, rect.bottom);
	m_MeditationButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, rect.left, rect.top, rect.right, rect.bottom);
	m_MeditationButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, rect.left, rect.top, rect.right, rect.bottom);

	if(m_MeditationIndex <= 0)
	{
#ifdef _XTS_AUTOMEDITATION
		m_MeditationButton->EnableWindow(TRUE);
#else
		m_MeditationButton->EnableWindow(FALSE);
#endif
	}
	else
	{
		m_MeditationButton->EnableWindow(TRUE);

		if( g_TutorialInfo )
		{
			if( !g_TutorialInfo->_XTutorial_052 )
			{
				_XWindow_GuideLine* pGuideLine_Window = (_XWindow_GuideLine*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GUIDELINEWINDOW);
				if( pGuideLine_Window )
				{
					if( pGuideLine_Window->GetShowStatus() )
						pGuideLine_Window->SetGuideLIneStep(GS_THIRD);
				}
			}
		}
	}

	if(m_MeditationIndex > 0)
	{
		m_bDrawMedEffect = TRUE;
	}
	else
	{
		m_bDrawMedEffect = FALSE;
	}
}

void _XWindow_MainMenu::DrawTooltip(void)
{
 	if( g_bShowItemMallWindow ) return;

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	if(scrnpos)
	{
		if(scrnpos->x > m_WindowPosition.x+14 && scrnpos->x < m_WindowPosition.x+14+30 &&
			scrnpos->z > m_WindowPosition.y+55 && scrnpos->z < m_WindowPosition.y+55+30)
		{
			if(m_MeditationIndex > 0)
			{
				if(_XSkillItem::FindSkillProperty(m_MeditationIndex))
				{
					TCHAR messagestring[256];
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINMENU_2188), g_SkillProperty[m_MeditationIndex]->skillName);
					g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+14, m_WindowPosition.y+35, messagestring, _XSC_INFORMATION);
				}
			}
		}
	}
}

void _XWindow_MainMenu::DrawMedEffect(int count)
{
	if(m_PrevDrawTime == 0)
		m_PrevDrawTime = g_LocalSystemTime;

	DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
	m_EffectFrame += ((FLOAT)elapsedtime / 500.0f);

	int selectedframe = (int)m_EffectFrame;

	if(selectedframe%2 == 0)
		m_MedEffectImage.Draw(m_WindowPosition.x+14, m_WindowPosition.y+55);

	m_PrevDrawTime = g_LocalSystemTime;

	if(count > 0)
	{
		if(m_EffectFrame > (FLOAT)count)
		{
			m_bDrawMedEffect = FALSE;
			m_PrevDrawTime = 0;
			m_EffectFrame = 0;
		}
	}
}

void _XWindow_MainMenu::SetDrawGuideLine(BOOL flag)
{
	if( flag )
	{
		m_GuideLineImage.ShowWindow(TRUE);
	}
	else
	{
		m_GuideLineImage.ShowWindow(FALSE);
	}
}

void _XWindow_MainMenu::ShowWindow(BOOL show)
{
	_XWindow::ShowWindow(show);
}