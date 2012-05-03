// XWindow_GM_UserCoordination.cpp: implementation of the _XWindow_GM_UserCoordination class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_GM_UserCoordination.h"

void __stdcall _XIMECallback_GMCoordControlTabKey( _XIMEContainer* pIMEContainer )
{
	_XWindow_GM_UserCoordination* pWindow = (_XWindow_GM_UserCoordination*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERCOORDSETTINGWINDOW);
	if( pWindow )
	{
		if( pIMEContainer == &pWindow->m_XCoordIMEControl)
		{
			pWindow->m_ZCoordIMEControl.SetFocus();
		}
		else if( pIMEContainer == &pWindow->m_ZCoordIMEControl )
		{
			pWindow->m_CharacterNameIMEControl.SetFocus();
		}
		else if( pIMEContainer == &pWindow->m_CharacterNameIMEControl )
		{
			pWindow->m_XCoordIMEControl.SetFocus();
		}		
	}
}

void __stdcall _XIMECallback_GMCoordControlEscapeKey( _XIMEContainer* pIMEContainer )
{
	_XWindow_GM_UserCoordination* pWindow = (_XWindow_GM_UserCoordination*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERCOORDSETTINGWINDOW);
	if( pWindow )
	{
		_XIMEKERNEL.SetFocus( NULL );
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_GM_UserCoordination::_XWindow_GM_UserCoordination()
{
}

_XWindow_GM_UserCoordination::~_XWindow_GM_UserCoordination()
{

}

BOOL _XWindow_GM_UserCoordination::Initialize( void )
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
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 430, 5 }, { 12, 12 }, _XDEF_GMCHARACTERCOORDSETTINGWINDOWCLOSEBTN,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );
			
	int characterwindowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );

	_XBTN_STRUCTURE menubtnstruct = { TRUE, { 50, 51 }, { 111, 26 },
		_XDEF_GMCHARACTERCOORDSETTINGGMMOVE,
		characterwindowimageindex,characterwindowimageindex,characterwindowimageindex, 
		&g_MainInterfaceTextureArchive };
	
	int	ButtonMenuString[3];	
	ButtonMenuString[0] = ID_STRING_GMCOMMAND_GMCOORDMOVE;
	ButtonMenuString[1] = ID_STRING_GMCOMMAND_USERNEARMOVE;
	ButtonMenuString[2] = ID_STRING_GMCOMMAND_USERMOVE;
	
	for( int i = 0; i < 3; i++ )
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
		
		menubtnstruct.commandid++;
		menubtnstruct.position.x += 118;
	}


	m_XCoordIMEControl.ResetContainer();
	m_XCoordIMEControl.ResizeWindow(40, 12);
	m_XCoordIMEControl.MoveWindow( m_WindowPosition.x + 50 , m_WindowPosition.y + 34 );	
	m_XCoordIMEControl.ShowWindow(TRUE);
	m_XCoordIMEControl.ShowWindowRectAngle( TRUE );
	m_XCoordIMEControl.SetMaxLength(4);
	m_XCoordIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
	m_XCoordIMEControl.SetHorizScrollMode( FALSE );		
	m_XCoordIMEControl.SetTabKeyCallback( _XIMECallback_GMCoordControlTabKey );	
	m_XCoordIMEControl.SetEscapeKeyCallback( _XIMECallback_GMCoordControlEscapeKey );

	m_ZCoordIMEControl.ResetContainer();
	m_ZCoordIMEControl.ResizeWindow(40, 12);
	m_ZCoordIMEControl.MoveWindow( m_WindowPosition.x + 140 , m_WindowPosition.y + 34 );
	m_ZCoordIMEControl.ShowWindow(TRUE);
	m_ZCoordIMEControl.ShowWindowRectAngle( TRUE );
	m_ZCoordIMEControl.SetMaxLength(4);
	m_ZCoordIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
	m_ZCoordIMEControl.SetHorizScrollMode( FALSE );		
	m_ZCoordIMEControl.SetTabKeyCallback( _XIMECallback_GMCoordControlTabKey );	
	m_ZCoordIMEControl.SetEscapeKeyCallback( _XIMECallback_GMCoordControlEscapeKey );

	m_CharacterNameIMEControl.ResetContainer();
	m_CharacterNameIMEControl.ResizeWindow(204, 12);
	m_CharacterNameIMEControl.MoveWindow( m_WindowPosition.x + 230 , m_WindowPosition.y + 34 );
	m_CharacterNameIMEControl.ShowWindow(TRUE);
	m_CharacterNameIMEControl.ShowWindowRectAngle( TRUE );
	m_CharacterNameIMEControl.SetMaxLength(13);
	m_CharacterNameIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
	m_CharacterNameIMEControl.SetHorizScrollMode( FALSE );		
	m_CharacterNameIMEControl.SetTabKeyCallback( _XIMECallback_GMCoordControlTabKey );	
	m_CharacterNameIMEControl.SetEscapeKeyCallback( _XIMECallback_GMCoordControlEscapeKey );

	return TRUE;
}

void _XWindow_GM_UserCoordination::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_GM_UserCoordination::Draw( _XGUIObject*& pfocusobject )
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
	g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+6 , _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_COORDMOVE) );	// _T("좌표 이동")
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y+34 , _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_POSX) );	// _T("X 좌표")
	g_XBaseFont->PutsAlign(m_WindowPosition.x+100, m_WindowPosition.y+34 , _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_POSZ) );	// _T("Z 좌표")
	g_XBaseFont->PutsAlign(m_WindowPosition.x+188, m_WindowPosition.y+34 , _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_CHARACTERNAME) );	// _T("인물명")
	g_XBaseFont->Flush();

	m_XCoordIMEControl.Draw();
	m_ZCoordIMEControl.Draw();
	m_CharacterNameIMEControl.Draw();
}

BOOL _XWindow_GM_UserCoordination::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if(CheckMousePosition() )
				{
					if( m_XCoordIMEControl.CheckMousePosition() )
						m_XCoordIMEControl.SetFocus();
					else if( m_ZCoordIMEControl.CheckMousePosition() )
						m_ZCoordIMEControl.SetFocus();
					else if( m_CharacterNameIMEControl.CheckMousePosition() )
						m_CharacterNameIMEControl.SetFocus();
				}
			}
		}
	}
		
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}	

void _XWindow_GM_UserCoordination::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );

	m_XCoordIMEControl.MoveWindow(m_WindowPosition.x + 50 , m_WindowPosition.y + 34);
	m_ZCoordIMEControl.MoveWindow(m_WindowPosition.x + 140 , m_WindowPosition.y + 34);
	m_CharacterNameIMEControl.MoveWindow(m_WindowPosition.x + 230 , m_WindowPosition.y + 34);
}

void _XWindow_GM_UserCoordination::ShowWindow( BOOL show )
{
	_XWindow::ShowWindow(show);

	if( show )
	{
		m_XCoordIMEControl.SetFocus();
		m_pMyManager->SetTopWindow( this );
	}
	else
	{
		_XIMEKERNEL.SetFocus( NULL );
	}
}


void _XWindow_GM_UserCoordination::Run( _XGMCoordControlType mode )
{

#ifdef _XGMCLIENT

	if( mode != _XGMCOORDCONTROLTYPE_GMMOVE )
	{
		if( strlen( m_CharacterNameIMEControl.GetText() ) <= 0 )
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_INPUTNAME), TRUE );	// _T("대상 인물 이름을 넣어 주세요.")
			return;
		}	
	}

	FLOAT x	= atof(m_XCoordIMEControl.GetText());
	FLOAT z	= atof(m_ZCoordIMEControl.GetText());

	if( mode == _XGMCOORDCONTROLTYPE_GMMOVE )
	{
		g_pLocalUser->m_Position.x = x - g_LodTerrain.m_HalfWorldSize;
		g_pLocalUser->m_Position.z = z - g_LodTerrain.m_HalfWorldSize;
		
		_XOBBData* pOBBData = NULL;
		
		g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pOBBData );
		
		g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
		g_pLocalUser->m_pGoundCollideOBB = pOBBData;
		
		g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3(g_pLocalUser->m_Position.x,
			g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
			g_pLocalUser->m_Position.z ) );
		g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
		g_LodTerrain.RebuildLevel(FALSE);
		
		_XWindow_WorldMinimap* pminimapwindow = 
			(_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
		if( pminimapwindow )
		{
			// Set direction to minimap arrow 
			pminimapwindow->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );					
			// Set minimap status
			pminimapwindow->SetLocalUserPosition();
		}
		
		g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );
	}
	else if( mode == _XGMCOORDCONTROLTYPE_GMMOVETOUSER )
	{
		g_NetworkKernel.SendPacket( MSG_NO_GM_REQUEST, en_gm_req_nextto_charac );		
	}
	else if( mode == _XGMCOORDCONTROLTYPE_MOVEUSER )
	{
		if( x == 0.0f && z == 0.0f )
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_ERRORPOSZERO), TRUE );	// _T("X,Z 좌표는 동시에 0을 넣을수 없습니다. 이동 좌표를 정확히 입력하세요.")
		}
		else
		{
			g_NetworkKernel.SendPacket( MSG_NO_GM_REQUEST, en_gm_req_charac_move );
		}
	}

#endif

}