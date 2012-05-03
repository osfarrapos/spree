// XWindow_OptionMenu.cpp: implementation of the _XWindow_OptionMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_OptionMenu.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_OptionMenu::_XWindow_OptionMenu()
{

}

_XWindow_OptionMenu::~_XWindow_OptionMenu()
{
	
}

BOOL _XWindow_OptionMenu::Initialize( void )
{
	//this->SetDrawBorder( FALSE );

	int mainbackinterface = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );
	int optionmenuinterface = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 183, 59, &g_MainInterfaceTextureArchive, mainbackinterface );
	pTitleBar->SetClipRect( 0, 125, 183, 184 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pBottomBar = new _XImageStatic;
	pBottomBar->Create( 0, 59, 182, 269, &g_MainInterfaceTextureArchive, optionmenuinterface );
	pBottomBar->SetClipRect( 0, 0, 183, 211 );
	InsertChildObject( pBottomBar );

	_XImageStatic* pTopEdgeLeftLine = new _XImageStatic;
	pTopEdgeLeftLine->Create( 10, 25, 13, 146, &g_MainInterfaceTextureArchive, optionmenuinterface );
	pTopEdgeLeftLine->SetClipRect( 186, 0, 190, 122 );
	InsertChildObject( pTopEdgeLeftLine );

	_XImageStatic* pTopEdgeRightLine = new _XImageStatic;
	pTopEdgeRightLine->Create( 171, 25, 175, 146, &g_MainInterfaceTextureArchive, optionmenuinterface );
	pTopEdgeRightLine->SetClipRect( 186, 0, 190, 122 );
	pTopEdgeRightLine->SetScale( -1, 1 );
	InsertChildObject( pTopEdgeRightLine );

	_XImageStatic* pTopEdgeTopLine = new _XImageStatic;
	pTopEdgeTopLine->Create( 14, 24, 167, 28, &g_MainInterfaceTextureArchive, optionmenuinterface );
	pTopEdgeTopLine->SetClipRect( 2, 216, 156, 222 );	
	InsertChildObject( pTopEdgeTopLine );

	_XImageStatic* pTopEdgeBottomLine = new _XImageStatic;
	pTopEdgeBottomLine->Create( 14, 142, 167, 146, &g_MainInterfaceTextureArchive, optionmenuinterface );
	pTopEdgeBottomLine->SetClipRect( 2, 223, 156, 228 );
	InsertChildObject( pTopEdgeBottomLine );


	_XImageStatic* pBottomEdgeLeftLine = new _XImageStatic;
	pBottomEdgeLeftLine->Create( 10, 146, 14, 242, &g_MainInterfaceTextureArchive, optionmenuinterface );
	pBottomEdgeLeftLine->SetClipRect( 192, 0, 196, 96 );
	InsertChildObject( pBottomEdgeLeftLine );
	
	_XImageStatic* pBottomEdgeRightLine = new _XImageStatic;
	pBottomEdgeRightLine->Create( 171, 146, 175, 242, &g_MainInterfaceTextureArchive, optionmenuinterface );
	pBottomEdgeRightLine->SetClipRect( 192, 0, 196, 96 );
	pBottomEdgeRightLine->SetScale( -1, 1 );
	InsertChildObject( pBottomEdgeRightLine );
	
	_XImageStatic* pBottomEdgeTopLine = new _XImageStatic;
	pBottomEdgeTopLine->Create( 14, 145, 167, 150, &g_MainInterfaceTextureArchive, optionmenuinterface );
	pBottomEdgeTopLine->SetClipRect( 2, 231, 156, 237 );
	InsertChildObject( pBottomEdgeTopLine );
	
	_XImageStatic* pBottomEdgeBottomLine = new _XImageStatic;
	pBottomEdgeBottomLine->Create( 14, 237, 167, 241, &g_MainInterfaceTextureArchive, optionmenuinterface );
	pBottomEdgeBottomLine->SetClipRect( 2, 237, 156, 242 );
	InsertChildObject( pBottomEdgeBottomLine );


	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_OPTIONMENU_CLOSEBUTTON,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );
	
	_XBTN_STRUCTURE closebtn2struct = { TRUE, { 59, 241 }, { 62, 26 }, _XDEF_OPTIONMENU_CLOSEBUTTON,
		optionmenuinterface,optionmenuinterface,optionmenuinterface, 
		&g_MainInterfaceTextureArchive };
	pClosebutton = new _XButton;
	pClosebutton->Create( closebtn2struct );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 186, 126, 248, 152 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  186, 151, 248, 177 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  186, 176, 248, 202 );
	pClosebutton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_CANCEL );
	InsertChildObject( pClosebutton );	
	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 153, 2 }, { 12, 12 }, _XDEF_OPTIONMENU_HELPBUTTON,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );


	int characterwindowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );

	int ButtonMenuString[7];
	ButtonMenuString[0] = ID_STRING_OPTION_2285;
	ButtonMenuString[1] = ID_STRING_OPTION_2286;
	ButtonMenuString[2] = ID_STRING_OPTION_2287;
	ButtonMenuString[3] = ID_STRING_OPTION_2288;
	ButtonMenuString[4] = ID_STRING_OPTION_OTHERCHARACTER;

#ifdef _XNEXON_WEBLOGIN
	BOOL readyautologin = TRUE;

	int parampos = _XCmdLineOption("passport=",0,8);
	if( parampos != 0 )
	{
		ButtonMenuString[5] = ID_STRING_LOGINSERVER_2179; // 서버 선택.
	}		
	else
	{
		ButtonMenuString[5] = ID_STRING_OPTION_GOTITLE;		
	}
		
#else	
	ButtonMenuString[5] = ID_STRING_OPTION_GOTITLE;
#endif

	ButtonMenuString[6] = ID_STRING_OPTION_EXITGAME;
	
	_XBTN_STRUCTURE menubtnstruct = { TRUE, { 34, 31 }, { 111, 26 }, _XDEF_OPTIONMENU_GRAPHICTABBUTTON,
		characterwindowimageindex,characterwindowimageindex,characterwindowimageindex, 
		&g_MainInterfaceTextureArchive };

	for( int i = 0; i < 7; i++ )
	{
		_XButton* pMenubutton = new _XButton;
		pMenubutton->Create( menubtnstruct );
		pMenubutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 105, 112, 131 );
		pMenubutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  1, 105, 112, 131 );
		pMenubutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 131, 112, 157 );		
		pMenubutton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ButtonMenuString[i] );
		pMenubutton->SetGrowTextMode( TRUE );
		pMenubutton->SetButtonTextColor( D3DCOLOR_ARGB(255,180,180,180) );
		InsertChildObject( pMenubutton );

		if( i == 4 )
		{
#ifdef _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI
#else
			pMenubutton->EnableWindow( FALSE );
#endif
		}

		menubtnstruct.commandid++;

		if( i == 3 )
		{
			menubtnstruct.position.y += 38;
		}
		else
		{
			menubtnstruct.position.y += 28;
		}		
	}
	
	return TRUE;
}

void _XWindow_OptionMenu::DestroyWindow( void )
{

	_XWindow::DestroyWindow();
}

void _XWindow_OptionMenu::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_OptionMenu::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw( pfocusobject );

	if(!this->m_ShowWindow) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
}

BOOL _XWindow_OptionMenu::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}

void _XWindow_OptionMenu::SetDefaultPosition( void )
{

}