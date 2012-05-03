// XWindow_PaymentWindow.cpp: implementation of the _XWindow_PaymentWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"
#include "SoundEffectList.h"
#include "XWindow_PaymentWindow.h"

void __stdcall _XPaymethodCallback_FreeUser( POINT winpos, SIZE winsize )
{

}

void __stdcall _XPaymethodCallback_PCRoom( POINT winpos, SIZE winsize )
{
	
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_PaymentWindow::_XWindow_PaymentWindow()
{
}

_XWindow_PaymentWindow::~_XWindow_PaymentWindow()
{

}

BOOL _XWindow_PaymentWindow::Initialize( void )
{
	SetDrawBorder( FALSE );

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( "MI_Premium.tga" );

	_XImageStatic* pLeftBorderImage = new _XImageStatic;

#ifdef _XDEF_SHUTDOWN_20061027
	pLeftBorderImage->Create( 0, 0, 146, 20, &g_MainInterfaceTextureArchive, charinfointerface );
	pLeftBorderImage->SetClipRect( 0, 0, 146, 20 );
#else
	pLeftBorderImage->Create( 0, 0, 146, 20, &g_MainInterfaceTextureArchive, charinfointerface );
	pLeftBorderImage->SetClipRect( 0, 20, 146, 40 );
#endif

	pLeftBorderImage->SetCallBackFunction_ToolTip( _XPaymethodCallback_FreeUser );

	InsertChildObject( pLeftBorderImage );

	return TRUE;
}

void _XWindow_PaymentWindow::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_PaymentWindow::ResetTitleBar( void )
{
}

//Author : 양희왕
//breif : 수정
void _XWindow_PaymentWindow::Draw( _XGUIObject*& pfocusobject )
{
	// 시작 시간 기억
	static DWORD iCurrentCheckTime = 0;
	if( iCurrentCheckTime == 0 )
		iCurrentCheckTime = g_LocalSystemTime;

	if( g_pLocalUser->m_bIsBossBattle ) return;

	_XWindow::Draw(pfocusobject);
	if( !this->m_ShowWindow ) return;

	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );

	// 베트남 셧다운 
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y+5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3149) );

	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	bool flag = false;
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if( scrnpos->x > m_WindowPosition.x && scrnpos->x < m_WindowPosition.x+146 )
	{
		if(scrnpos->z > m_WindowPosition.y && scrnpos->z < m_WindowPosition.y+20 )
		{
			flag = true;
			if( g_pMiniTooltip_Window )
			{
				DWORD dwTime = 0;
				if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_PAYMENTINFO, 0, 0, 0, dwTime ) )
					g_pMiniTooltip_Window->ShowWindow( TRUE );
			}		
		}
	}

	if( flag == false && g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_PAYMENTINFO )
		g_pMiniTooltip_Window->ShowWindow( FALSE );
}

BOOL _XWindow_PaymentWindow::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if( g_pLocalUser->m_bIsBossBattle ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}

void _XWindow_PaymentWindow::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_PaymentWindow::ShowWindow( BOOL show )
{
	_XWindow::ShowWindow( show );
}