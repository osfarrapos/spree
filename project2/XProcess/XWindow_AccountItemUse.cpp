// XWindow_AccountItemUse.cpp: implementation of the _XWindow_AccountItemUse class.
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
#include "XWindow_AccountItemUse.h"
#include "Xwindow_PCRPremium.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_AccountItemUse::_XWindow_AccountItemUse()
{

}

_XWindow_AccountItemUse::~_XWindow_AccountItemUse()
{

}

BOOL _XWindow_AccountItemUse::Initialize( void )
{
	SetDrawBorder( FALSE );
	
	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( "MI_Premium.tga" );
	
	_XImageStatic* pLeftBorderImage = new _XImageStatic;
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI //Author :양희왕// breif : 구룡창 사이즈 변경 
	pLeftBorderImage->Create( 0, 0, 173, 20,
						  &g_MainInterfaceTextureArchive, charinfointerface );
	pLeftBorderImage->SetClipRect( 0, 0, 173, 20 );
#else
	pLeftBorderImage->Create( 0, 0, 146, 20,
						  &g_MainInterfaceTextureArchive, charinfointerface );
	pLeftBorderImage->SetClipRect( 0, 0, 146, 20 );
#endif
	
	InsertChildObject( pLeftBorderImage );
	
	return TRUE;
}

void _XWindow_AccountItemUse::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_AccountItemUse::ShowWindow( BOOL show )
{
	if(show)
	{
		if( g_pLocalUser->m_AccountItemsID > 0 )
		{
#ifdef _XDEF_SHUTDOWN_20061027
			_XWindow_PaymentWindow* pPaymentMethod_Window = (_XWindow_PaymentWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PAYMENTMETHODWINDOW);
			if( pPaymentMethod_Window)
				pPaymentMethod_Window->MoveWindow( 82, 93 );

			_XWindow_PCRPremium* pPCRPremium_Window = (_XWindow_PCRPremium*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCRPREMIUN_WINDOW);
			if(pPCRPremium_Window)
				pPCRPremium_Window->MoveWindow(82, 113);
#else
			_XWindow_PCRPremium* pPCRPremium_Window = (_XWindow_PCRPremium*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCRPREMIUN_WINDOW);
			if(pPCRPremium_Window)
				pPCRPremium_Window->MoveWindow(82, 93);
#endif
		}
		else
		{
#ifdef _XDEF_SHUTDOWN_20061027
			_XWindow_PaymentWindow* pPaymentMethod_Window = (_XWindow_PaymentWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PAYMENTMETHODWINDOW);

			if( pPaymentMethod_Window)
				pPaymentMethod_Window->MoveWindow( 82, 73 );

			_XWindow_PCRPremium* pPCRPremium_Window = (_XWindow_PCRPremium*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCRPREMIUN_WINDOW);
			if(pPCRPremium_Window)
				pPCRPremium_Window->MoveWindow(82, 93);
#else
			_XWindow_PCRPremium* pPCRPremium_Window = (_XWindow_PCRPremium*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCRPREMIUN_WINDOW);
			if(pPCRPremium_Window)
				pPCRPremium_Window->MoveWindow(82, 73);
#endif

			
		}
	}

	_XWindow::ShowWindow( show );
}

void _XWindow_AccountItemUse::Draw( _XGUIObject*& pfocusobject )
{
	//if( g_pLocalUser->m_bIsBossBattle ) return;

	_XWindow::Draw(pfocusobject);
	if( !this->m_ShowWindow ) return;

	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );

	// 경험치 증가 아이템 
	if(g_pLocalUser->m_AccountItemsID > 0)
	{
		switch(g_PotionItemProperty[g_pLocalUser->m_AccountItemsID].sSimgi)
		{
		case 1 :
			{
				g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y+5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2972) );
			}
			break;
		case 2 :
			{
				g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y+5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2973) );
			}
			break;
		case 3 :
			{
				g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y+5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2974) );
			}
			break;
		case 4 :
			{
				g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y+5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2969) );
			}
			break;
		case 5 :
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y+5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3405));
			}
			break;
		case 6 :
			{
				g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y+5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2969) );
			}
			break;
		}
	}
	
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	bool flag = false;
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if( scrnpos->x > m_WindowPosition.x && scrnpos->x < m_WindowPosition.x+146 )
	{
		if(  scrnpos->z > m_WindowPosition.y && scrnpos->z < m_WindowPosition.y+20 )
		{
			flag = true;
			if( g_pMiniTooltip_Window )
			{
				if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_ITEM_ACCOUNT, 0, g_pLocalUser->m_AccountItemcType, g_pLocalUser->m_AccountItemsID ) )
					g_pMiniTooltip_Window->ShowWindow( TRUE );
			}		
		}
	}

	if( flag == false && g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_ITEM_ACCOUNT )
		g_pMiniTooltip_Window->ShowWindow( FALSE );
}

BOOL _XWindow_AccountItemUse::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( g_pLocalUser->m_bIsBossBattle ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}