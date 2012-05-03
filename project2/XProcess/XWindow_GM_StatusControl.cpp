// XWindow_GM_StatusControl.cpp: implementation of the _XWindow_GM_StatusControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_GM_StatusControl.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_GM_StatusControl::_XWindow_GM_StatusControl()
{

}

_XWindow_GM_StatusControl::~_XWindow_GM_StatusControl()
{

}


void _XWindow_GM_StatusControl::ToggleHiddenStatus( void )
{
	if( !m_pHiddenButton ) return;

	if( m_pHiddenButton->GetCheck() )
	{
		g_NetworkKernel.SendPacket(MSG_NO_CTRL_CHARAC_SETTING, 9, 1);
	}
	else
	{
		g_NetworkKernel.SendPacket(MSG_NO_CTRL_CHARAC_SETTING, 9, 0);
	}
}


BOOL _XWindow_GM_StatusControl::Initialize( void )
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
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 430, 5 }, { 12, 12 }, _XDEF_GMCHARACTERSETTINGWINDOWCLOSEBTN,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );
			
	int optionmenuinterface = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );

	_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, { 80, 34 }, 
	{ 14, 14 }, 
	_XDEF_GMSTATUSCONTROLHIDDENBUTTON,
	optionmenuinterface,optionmenuinterface,
	&g_MainInterfaceTextureArchive };
	
	m_pHiddenButton = new _XCheckButton;
	m_pHiddenButton->Create( checkbtnstruct );	
	m_pHiddenButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pHiddenButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	m_pHiddenButton->SetButtonSwapMode( FALSE );
	m_pHiddenButton->SetCheck( TRUE );
	InsertChildObject( m_pHiddenButton );

	checkbtnstruct.commandid = _XDEF_GMSTATUSCONTROLFLYCAMMODE;
	checkbtnstruct.position.x = 80;
	checkbtnstruct.position.y = 54;
	_XCheckButton* pFlyCamButton = new _XCheckButton;
	pFlyCamButton->Create( checkbtnstruct );	
	pFlyCamButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	pFlyCamButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	pFlyCamButton->SetButtonSwapMode( FALSE );
	pFlyCamButton->SetCheck( FALSE );
	InsertChildObject( pFlyCamButton );

	checkbtnstruct.commandid = _XDEF_GMSTATUSCONTROLTIMEFLOWCHECKBUTTON;
	checkbtnstruct.position.x = 390;
	checkbtnstruct.position.y = 34;
	_XCheckButton* pTimeFlowButton = new _XCheckButton;
	pTimeFlowButton->Create( checkbtnstruct );	
	pTimeFlowButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	pTimeFlowButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	pTimeFlowButton->SetButtonSwapMode( FALSE );
	pTimeFlowButton->SetCheck( TRUE );
	InsertChildObject( pTimeFlowButton );

	checkbtnstruct.commandid = _XDEF_GMSTATUSCONTROLCURSORVIEW;
	checkbtnstruct.position.x = 390;
	checkbtnstruct.position.y = 54;
	_XCheckButton* pCursorViewButton = new _XCheckButton;
	pCursorViewButton->Create( checkbtnstruct );	
	pCursorViewButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	pCursorViewButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	pCursorViewButton->SetButtonSwapMode( FALSE );
	pCursorViewButton->SetCheck( TRUE );
	InsertChildObject( pCursorViewButton );

	int characterwindowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	int optionmenuinterface2 = g_MainInterfaceTextureArchive.FindResource( g_MI_KiUpTextureName );

	_XSLIDERCONTROL_STRUCTURE	slidercreatestruct =
	{
		TRUE, { 200, 34 }, 
		{ 88, 12 }, _XDEF_GMSTATUSCONTROLCAMERASPEEDSLIDER,
		optionmenuinterface,
		optionmenuinterface,
		optionmenuinterface,
		optionmenuinterface,
		optionmenuinterface,
		{ -5, -4 }, { 12, 12 },
		optionmenuinterface,
		&g_MainInterfaceTextureArchive,
		"HSlider"
	};

	SIZE spinbtnsize = { 12, 12 };
	RECT leftspinbtncliprect[3] =
	{
		{ 200, 17, 212, 29 },
		{ 200, 29, 212, 41 },
		{ 200, 41, 212, 53 },
	};

	RECT rightspinbtncliprect[3] =
	{
		{ 178,  0,  190, 12 },
		{ 178,  12, 190, 24 },
		{ 178,  24, 190, 36 },
	};

	_XHSlider* pCameraSpeedSlider = new _XHSlider;
	pCameraSpeedSlider->Create( slidercreatestruct );
	pCameraSpeedSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	pCameraSpeedSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	pCameraSpeedSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	pCameraSpeedSlider->m_IndicatorImage.SetClipRect( 161,244 , 249, 256 );
	pCameraSpeedSlider->SetClipRect( 2,244, 90, 256 );
	pCameraSpeedSlider->m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	pCameraSpeedSlider->SetGabSize( 1 );
	pCameraSpeedSlider->ShowWindow( TRUE );
	pCameraSpeedSlider->EnableWindow( TRUE );
	pCameraSpeedSlider->SetGaugeValue( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_GAMMA, 35, TRUE ) );
	pCameraSpeedSlider->SetUseSpinButton( TRUE, 1 );
	pCameraSpeedSlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );
		
	InsertChildObject( pCameraSpeedSlider );

	slidercreatestruct.commandid = _XDEF_GMSTATUSCONTROLFOGDISTANCESLIDER;
	slidercreatestruct.position.x = 200;
	slidercreatestruct.position.y = 54;

	_XHSlider* pFogDistanceSlider = new _XHSlider;
	pFogDistanceSlider->Create( slidercreatestruct );
	pFogDistanceSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	pFogDistanceSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	pFogDistanceSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	pFogDistanceSlider->m_IndicatorImage.SetClipRect( 161,244 , 249, 256 );
	pFogDistanceSlider->SetClipRect( 2,244, 90, 256 );
	pFogDistanceSlider->m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	pFogDistanceSlider->SetGabSize( 1 );
	pFogDistanceSlider->ShowWindow( TRUE );
	pFogDistanceSlider->EnableWindow( TRUE );
	pFogDistanceSlider->SetGaugeValue( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_GAMMA, 35, TRUE ) );
	pFogDistanceSlider->SetUseSpinButton( TRUE, 1 );
	pFogDistanceSlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );
		
	InsertChildObject( pFogDistanceSlider );

	slidercreatestruct.commandid = _XDEF_GMSTATUSCONTROLENVIRONMENTSLIDER;
	slidercreatestruct.position.x = 93;
	slidercreatestruct.position.y = 74;

	_XHSlider* pEnvSlider = new _XHSlider;
	pEnvSlider->Create( slidercreatestruct );
	pEnvSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	pEnvSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	pEnvSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	pEnvSlider->m_IndicatorImage.SetClipRect( 161,244 , 249, 256 );
	pEnvSlider->SetClipRect( 2,244, 90, 256 );
	pEnvSlider->m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	pEnvSlider->SetGabSize( 1 );
	pEnvSlider->ShowWindow( TRUE );
	pEnvSlider->EnableWindow( TRUE );
	pEnvSlider->SetGaugeValue( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_GAMMA, 35, TRUE ) );
	pEnvSlider->SetUseSpinButton( TRUE, 1 );
	pEnvSlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );
		
	InsertChildObject( pEnvSlider );
	
	return TRUE;
}

void _XWindow_GM_StatusControl::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_GM_StatusControl::Draw( _XGUIObject*& pfocusobject )
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
	g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+6 , _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_2078) );
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y+34 , _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_2079) );

	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y+54 , _XFONT_ALIGNTYPE_LEFT, "FlyCam mode" );

	g_XBaseFont->PutsAlign(m_WindowPosition.x+110, m_WindowPosition.y+34 , _XFONT_ALIGNTYPE_LEFT, "Camera speed" );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+110, m_WindowPosition.y+54 , _XFONT_ALIGNTYPE_LEFT, "Fog distance" );

	g_XBaseFont->PutsAlign(m_WindowPosition.x+320, m_WindowPosition.y+34 , _XFONT_ALIGNTYPE_LEFT, "Time flow" );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+320, m_WindowPosition.y+54 , _XFONT_ALIGNTYPE_LEFT, "View cursor" );

	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y+74 , _XFONT_ALIGNTYPE_LEFT, "Environment" );

	g_XBaseFont->Flush();
}

BOOL _XWindow_GM_StatusControl::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}	

void _XWindow_GM_StatusControl::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_GM_StatusControl::ShowWindow( BOOL show )
{
	_XWindow::ShowWindow(show);
	
}


