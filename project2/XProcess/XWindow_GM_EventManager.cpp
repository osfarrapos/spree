// XWindow_GM_UserCoordination.cpp: implementation of the _XWindow_GM_EventManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_GM_EventManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_GM_EventManager::_XWindow_GM_EventManager()
{
}

_XWindow_GM_EventManager::~_XWindow_GM_EventManager()
{

}

BOOL _XWindow_GM_EventManager::Initialize( void )
{
	// Title---------------------------------------------------------------------------------------=
	int titleimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );
	
	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create( 0, 0, 177, 23,
		&g_MainInterfaceTextureArchive, titleimageindex );
	pTitleLeftBar->SetClipRect( 0, 0, 177, 24 );
	InsertChildObject( pTitleLeftBar );
	
	_XImageStatic* pTitleCentertBar = new _XImageStatic;
	pTitleCentertBar->Create( 176, 0, 177+175, 23,
		&g_MainInterfaceTextureArchive, titleimageindex );
	pTitleCentertBar->SetClipRect( 79, 0, 177, 24 );
	InsertChildObject( pTitleCentertBar );
	
	_XImageStatic* pTitleRightBar = new _XImageStatic; 
	pTitleRightBar->Create( 451, 0, 451+177, 23,
		&g_MainInterfaceTextureArchive, titleimageindex );
	pTitleRightBar->SetClipRect( 0, 0, 177, 24 );
	pTitleRightBar->SetScale( -1, 1 );
	InsertChildObject( pTitleRightBar );


	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 430, 5 }, { 12, 12 }, _XDEF_GMEVENTCONTROLWINDOWCLOSEBTN,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );
			
	int characterwindowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );

	_XBTN_STRUCTURE menubtnstruct = { TRUE, { 50, 36 }, { 111, 26 },
		_XDEF_GMEVENTCONTROLWINDOW_BGM1,
		characterwindowimageindex,characterwindowimageindex,characterwindowimageindex, 
		&g_MainInterfaceTextureArchive };
	
	TCHAR ButtonMenuString[13][20];	
	memset( ButtonMenuString, 0, sizeof(TCHAR) * 12 * 20 );

	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{
		strcpy( ButtonMenuString[0], _T("Ω≈∂˚¿‘¿ÂBGM") );
		strcpy( ButtonMenuString[1], _T("Ω≈∫Œ¿‘¿ÂBGM") );
		strcpy( ButtonMenuString[2], _T("«‡¡¯BGM") );
		strcpy( ButtonMenuString[3], _T("Stop BGM") );
		strcpy( ButtonMenuString[4], _T("√‡∆˜") );
		strcpy( ButtonMenuString[5], _T("≤… 1") );
		strcpy( ButtonMenuString[6], _T("≤… 2") );
		strcpy( ButtonMenuString[7], _T("≤… 3") );
		strcpy( ButtonMenuString[8], _T("øπΩƒ¿Â") );
		strcpy( ButtonMenuString[9], _T("øπΩƒ¿Â ªË¡¶") );
		strcpy( ButtonMenuString[10], _T("≥∑") );
		strcpy( ButtonMenuString[11], _T("«ÿ¡˙≥Ë") );
		strcpy( ButtonMenuString[12], _T("π„") );
	}
	else
	{
		strcpy( ButtonMenuString[0], _T("groom BGM") );
		strcpy( ButtonMenuString[1], _T("bride BGM") );
		strcpy( ButtonMenuString[2], _T("Start march") );
		strcpy( ButtonMenuString[3], _T("Stop BGM") );
		strcpy( ButtonMenuString[4], _T("FireWork") );
		strcpy( ButtonMenuString[5], _T("Flower1") );
		strcpy( ButtonMenuString[6], _T("Flower2") );
		strcpy( ButtonMenuString[7], _T("Flower3") );
		strcpy( ButtonMenuString[8], _T("Create hall") );
		strcpy( ButtonMenuString[9], _T("Erase hall") );
		strcpy( ButtonMenuString[10], _T("Day") );
		strcpy( ButtonMenuString[11], _T("Night fall") );
		strcpy( ButtonMenuString[12], _T("Night") );
	}
	
	for( int i = 0; i < 13; i++ )
	{
		_XButton* pMenubutton = new _XButton;
		pMenubutton->Create( menubtnstruct );
		pMenubutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 105, 112, 131 );
		pMenubutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  1, 105, 112, 131 );
		pMenubutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 131, 112, 157 );		
		pMenubutton->SetButtonText( ButtonMenuString[i] );
		pMenubutton->SetGrowTextMode( TRUE );
		pMenubutton->SetButtonTextColor( D3DCOLOR_ARGB(255,180,180,180) );
		InsertChildObject( pMenubutton );
		
		menubtnstruct.commandid++;

		if( i == 4 )
		{
			menubtnstruct.position.x += 118;
			menubtnstruct.position.y = 36;
		}
		else if( i == 9 )
		{
			menubtnstruct.position.x += 118;
			menubtnstruct.position.y = 36;
		}
		else
		{
			menubtnstruct.position.y += 25;
		}
	}

	return TRUE;
}

void _XWindow_GM_EventManager::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_GM_EventManager::Draw( _XGUIObject*& pfocusobject )
{
	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// Draw Border, Line ---------------------------------------------------------------------=	
	_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y+26, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, D3DCOLOR_ARGB(128, 0, 0, 0) );
	
	_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y+23, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, 0.0f,0xFF000000 );
	_XDrawRectAngle( m_WindowPosition.x+1, m_WindowPosition.y+24, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f,0xFF9F9A84 );
	_XDrawRectAngle( m_WindowPosition.x+2, m_WindowPosition.y+25, m_WindowPosition.x+m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f,0xFF040000 );
	_XDrawRectAngle( m_WindowPosition.x+3, m_WindowPosition.y+26, m_WindowPosition.x+m_WindowSize.cx-3, m_WindowPosition.y+m_WindowSize.cy-3, 0.0f,0xFF1A211A );

	_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+28, m_WindowPosition.x+m_WindowSize.cx-9, m_WindowPosition.y+m_WindowSize.cy-6, 0.0f,0xFF000000 );

	_XWindow::Draw(pfocusobject);
		
	// Title Name
	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+6 , _XFONT_ALIGNTYPE_CENTER, _T("Event manager") );	// _T("¡¬«• ¿Ãµø")
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
	
	g_XBaseFont->Flush();
}

BOOL _XWindow_GM_EventManager::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}	

void _XWindow_GM_EventManager::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_GM_EventManager::ShowWindow( BOOL show )
{
	_XWindow::ShowWindow(show);
}


