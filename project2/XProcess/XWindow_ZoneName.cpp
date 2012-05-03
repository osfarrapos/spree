// XWindow_ZoneName.cpp: implementation of the _XWindow_ZoneName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_ZoneName.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_ZoneName::_XWindow_ZoneName()
{
	m_EffectStaus = _XZoneWindowEffectClass_Stop;
	m_EffectTime  = 0;	
	m_DropStatus = FALSE;
	m_DropProgress		= 0.0f;
	m_EffectProgress	= 0.0f;		
	m_WorldNameDisplayMode = FALSE;
	memset( m_ZoneName, 0, _XDEF_MAXZONENAMEWINDOWSTRINGSIZE );
#ifdef _XJAPANESE
	memset( m_ZoneHiraganaName, 0, _XDEF_MAXZONENAMEWINDOWSTRINGSIZE );
#endif
}

_XWindow_ZoneName::~_XWindow_ZoneName()
{

}

#ifdef _XJAPANESE
void _XWindow_ZoneName::SetZoneName( LPTSTR zonename, LPTSTR zonehiragananame )
#else
void _XWindow_ZoneName::SetZoneName( LPTSTR zonename )
#endif
{
	memset( m_ZoneName, 0, _XDEF_MAXZONENAMEWINDOWSTRINGSIZE );
	if( zonename )
		strcpy( m_ZoneName, zonename );

#ifdef _XJAPANESE
	memset( m_ZoneHiraganaName, 0, _XDEF_MAXZONENAMEWINDOWSTRINGSIZE );
	if( zonehiragananame )
		strcpy( m_ZoneHiraganaName, zonehiragananame );
#endif
}

BOOL _XWindow_ZoneName::Initialize( void )
{
	SetDrawBorder( FALSE );
	SetWindowMoveMode( _XWMOVE_FIXED );
	ShowWindow( FALSE );

	int imageindex = g_MainInterfaceTextureArchive.FindResource( "areaname_base.tga" );
	
#ifdef _XDEF_EXPAND_ZONENAME
	m_pBorder.Create( m_WindowPosition.x, 
		m_WindowPosition.y, 
		m_WindowPosition.x + 256, 
		m_WindowPosition.y + m_WindowSize.cy,
		&g_MainInterfaceTextureArchive, 
		imageindex );	
	m_pBorder.SetClipRect( 0, 0, 256, 45  );

	m_pBorder2.Create( m_WindowPosition.x, 
		m_WindowPosition.y, 
		m_WindowPosition.x + 108, 
		m_WindowPosition.y + m_WindowSize.cy,
		&g_MainInterfaceTextureArchive, 
		imageindex );	
	m_pBorder2.SetClipRect( 148, 45, 256, 90  );
	
#else
	m_pBorder.Create( m_WindowPosition.x, 
		m_WindowPosition.y, 
		m_WindowPosition.x + m_WindowSize.cx, 
		m_WindowPosition.y + m_WindowSize.cy,
		&g_MainInterfaceTextureArchive, 
		imageindex );	
	m_pBorder.SetClipRect( 0, 0, 162, 45  );
#endif
	m_pLeftStick.Create( m_WindowPosition.x - 16, 
						 m_WindowPosition.y - 16, 
						 m_WindowPosition.x, 
						 m_WindowPosition.y + 77 - 16,
						 &g_MainInterfaceTextureArchive, 
						 imageindex );	
	m_pLeftStick.SetClipRect( 0, 45, 16, 122  );

	m_pRightStick.Create( m_WindowPosition.x + m_WindowSize.cx, 
						  m_WindowPosition.y - 16, 
						  m_WindowPosition.x + m_WindowSize.cx + 16, 
						  m_WindowPosition.y + 77 - 16,
						  &g_MainInterfaceTextureArchive, 
						  imageindex );	
	m_pRightStick.SetClipRect( 16, 45, 32, 122  );

	
	return TRUE;
}

void _XWindow_ZoneName::DestroyWindow( void )
{	
	_XWindow::DestroyWindow();
	
	
}

void _XWindow_ZoneName::ShowWindow( BOOL show )
{
	if( m_ShowWindow == show ) return;

	if( g_CurrentFocusedObject == (_XGUIObject*)this )
		g_CurrentFocusedObject = NULL;		
	m_DockedParentWindow = NULL;
	m_DockedChildWindow  = NULL;

	m_ShowWindow = show;
	
	if(m_pMyManager)
		m_pMyManager->SetTopWindow(this);
}

void _XWindow_ZoneName::StartAnimation( _XZoneWindowEffectClass effectclass )
{	
	m_EffectTime		= g_LocalSystemTime;
	m_EffectStaus		= effectclass;

	switch( effectclass ) 
	{
	case _XZoneWindowEffectClass_Stop  :
		m_EffectProgress	= 0;
		break;
	case _XZoneWindowEffectClass_Open  :
#ifdef _XDEF_EXPAND_ZONENAME
		m_DropStatus = TRUE;
		m_DropProgress		= 0.0f;
		m_EffectProgress	= 0.0f;
		
		m_WindowSize.cx = 364;
		m_WindowSize.cy = 45;
				
		m_WindowPosition.y = (-m_WindowSize.cy - 16);

		#ifdef _ACCLAIM_RUBICONADSYSTEM
		if( g_ViewRubiconAD )
			m_WindowPosition.y += g_RubiconADWindowSize.cy;
		#endif

		m_pBorder.SetWindowSize( 0, m_pBorder.GetWindowSize().cy );
		m_pBorder.MoveWindow( m_WindowPosition.x+182, m_WindowPosition.y );
		m_pBorder.SetClipRect( 182, 0, 182, 45  );		
				
		m_pBorder2.SetWindowSize( 0, m_pBorder2.GetWindowSize().cy );
		m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
		m_pBorder2.SetClipRect( 148, 45, 148, 90  );	
		
		m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
		m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx+m_pBorder2.GetWindowSize().cx, m_WindowPosition.y - 16 );
		ShowWindow( TRUE );
#else
		m_DropStatus = TRUE;
		m_DropProgress		= 0.0f;
		m_EffectProgress	= 0.0f;
		
		m_WindowSize.cx = 161;
		m_WindowSize.cy = 45;
		
		m_WindowPosition.y = (-m_WindowSize.cy - 16);

#ifdef _ACCLAIM_RUBICONADSYSTEM
		if( g_ViewRubiconAD )
			m_WindowPosition.y += g_RubiconADWindowSize.cy;
#endif

		m_pBorder.SetWindowSize( 0, m_pBorder.GetWindowSize().cy );
		m_pBorder.MoveWindow( m_WindowPosition.x+81, m_WindowPosition.y );
		m_pBorder.SetClipRect( 81, 0, 81, 45  );		
		
		m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
		m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx, m_WindowPosition.y - 16 );
		ShowWindow( TRUE );
#endif

		break;
	case _XZoneWindowEffectClass_Close :
#ifdef _XDEF_EXPAND_ZONENAME
		m_DropStatus = FALSE;
		m_DropProgress		= 0.0f;
		m_EffectProgress	= 0.0f;		
		m_WindowPosition.y = 96;

#ifdef _ACCLAIM_RUBICONADSYSTEM
		if( g_ViewRubiconAD )
			m_WindowPosition.y += g_RubiconADWindowSize.cy;
#endif

		m_pBorder.SetWindowSize( 256, m_pBorder.GetWindowSize().cy );
		m_pBorder.MoveWindow( m_WindowPosition.x, m_WindowPosition.y );
		m_pBorder.SetClipRect( 0, 0, 256, 45  );
		
		m_pBorder2.SetWindowSize( 108, m_pBorder.GetWindowSize().cy );
		m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
		m_pBorder2.SetClipRect( 148, 45, 256, 90  );

		m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
		m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx+m_pBorder2.GetWindowSize().cx, m_WindowPosition.y - 16 );
		
		ShowWindow( TRUE );
#else
		m_DropStatus = FALSE;
		m_DropProgress		= 0.0f;
		m_EffectProgress	= 0.0f;		
		m_WindowPosition.y = 96;

#ifdef _ACCLAIM_RUBICONADSYSTEM
		if( g_ViewRubiconAD )
			m_WindowPosition.y += g_RubiconADWindowSize.cy;
#endif

		m_pBorder.SetWindowSize( 161, m_pBorder.GetWindowSize().cy );
		m_pBorder.MoveWindow( m_WindowPosition.x, m_WindowPosition.y );
		m_pBorder.SetClipRect( 0, 0, 162, 45  );
		
		m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
		m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx, m_WindowPosition.y - 16 );
		
		ShowWindow( TRUE );
#endif

		break;	
	}	
}

void _XWindow_ZoneName::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );
	
	m_pBorder.MoveWindow( m_WindowPosition.x, m_WindowPosition.y );
#ifdef _XDEF_EXPAND_ZONENAME
	m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
#endif
	m_pLeftStick.MoveWindow( m_WindowPosition.x-16, m_WindowPosition.y );
	m_pRightStick.MoveWindow( m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y );


}

void _XWindow_ZoneName::Draw( _XGUIObject*& pfocusobject )
{			
	_XWindow::Draw(pfocusobject);	
	
	if( !this->m_ShowWindow ) return;
	
	m_pBorder.DrawWithRegion();
#ifdef _XDEF_EXPAND_ZONENAME
	m_pBorder2.DrawWithRegion();
#endif
	m_pLeftStick.DrawWithRegion();
	m_pRightStick.DrawWithRegion();		

	if( m_EffectStaus == _XZoneWindowEffectClass_Stop )
	{
		g_XLargeFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
		g_XLargeFont->SetColor( D3DCOLOR_ARGB( 255, 250, 230, 170 ) );
#ifdef _XJAPANESE
		if( m_ZoneHiraganaName[0] )
		{
			g_XLargeFont->PrintAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 5, 1.0f, _XFONT_ALIGNTYPE_CENTER, m_ZoneName );
			g_XLargeFont->Flush();
			g_XLargeFont->DisableGrowMode();

			g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 250, 230, 170 ) );
			g_XBaseFont->PrintAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 26, 1.0f, _XFONT_ALIGNTYPE_CENTER, m_ZoneHiraganaName );
			g_XBaseFont->Flush();
		}
		else
		{
			g_XLargeFont->PrintAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 14, 1.0f, _XFONT_ALIGNTYPE_CENTER, m_ZoneName );
			g_XLargeFont->Flush();
			g_XLargeFont->DisableGrowMode();
		}
#else
		g_XLargeFont->PrintAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 14, 1.0f, _XFONT_ALIGNTYPE_CENTER, m_ZoneName );
		g_XLargeFont->Flush();
		g_XLargeFont->DisableGrowMode();
#endif		
	}
}

BOOL _XWindow_ZoneName::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
			
	FLOAT progresstime = (FLOAT)( g_LocalSystemTime - m_EffectTime ) / 4.0f;
		
	if( m_EffectStaus == _XZoneWindowEffectClass_Open )
	{	
		m_EffectTime = g_LocalSystemTime;
		
#ifdef _XDEF_EXPAND_ZONENAME
		if( m_DropStatus )
		{
			if( m_DropProgress > 86 )
			{
				m_DropProgress+=1;
			}
			else
			{
				m_DropProgress+= progresstime;
			}
			
			if( m_DropProgress > 96 )
			{
				m_DropStatus = FALSE;
				m_pBorder.MoveWindow( m_WindowPosition.x+182, m_WindowPosition.y );
				m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
				m_pBorder2.SetWindowSize(0,0);
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx+m_pBorder2.GetWindowSize().cx, m_WindowPosition.y - 16 );
				
				m_EffectTime = g_LocalSystemTime;
				m_EffectProgress = 0;
			}
			else
			{			
				m_WindowPosition.y =  (int)m_DropProgress;

#ifdef _ACCLAIM_RUBICONADSYSTEM
				if( g_ViewRubiconAD )
					m_WindowPosition.y += g_RubiconADWindowSize.cy;
#endif
				
				int iprogress = _XFC_FloatToIntRet( m_EffectProgress );				
				m_pBorder.MoveWindow( m_WindowPosition.x+182, m_WindowPosition.y );
				m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
				m_pBorder2.SetWindowSize(0,0);
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx+m_pBorder2.GetWindowSize().cx, m_WindowPosition.y - 16 );
			}
		}
		else
		{
			if( m_EffectProgress < 10 || m_EffectProgress > 170 )
			{
				m_EffectProgress+=1;
			}
			else
			{
				m_EffectProgress+=progresstime;
			}
			
			if( m_EffectProgress > 182 )
			{
				m_EffectProgress = 0.0f;
				m_EffectStaus = _XZoneWindowEffectClass_Stop;			
								
				m_pBorder.SetWindowSize( 256, m_WindowSize.cy );
				m_pBorder.MoveWindow( m_WindowPosition.x, m_WindowPosition.y );
				m_pBorder.SetClipRect( 0, 0, 256, 45  );
				
				m_pBorder2.SetWindowSize( 108, m_WindowSize.cy );
				m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
				m_pBorder2.SetClipRect( 148, 45, 256, 90  );

				m_pLeftStick.MoveWindow( m_WindowPosition.x-16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y - 16 );			
			}
			else
			{			
				int iprogress = _XFC_FloatToIntRet( m_EffectProgress );
				if( iprogress <= 74 )
				{
					m_pBorder.SetWindowSize( (iprogress<<1), m_pBorder.GetWindowSize().cy );
					m_pBorder.MoveWindow( m_WindowPosition.x+182-iprogress, m_WindowPosition.y );
					m_pBorder.SetClipRect( 182-iprogress, 0, 182+iprogress, 45 );		
					
					m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
					m_pBorder2.SetWindowSize(0,0);
				}
				else
				{
					m_pBorder.SetWindowSize( iprogress+74, m_pBorder.GetWindowSize().cy );
					m_pBorder.MoveWindow( m_WindowPosition.x+182-iprogress, m_WindowPosition.y );
					m_pBorder.SetClipRect( 182-iprogress, 0, 256, 45  );		
					
					m_pBorder2.SetWindowSize( iprogress-74, m_pBorder2.GetWindowSize().cy );
					m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
					m_pBorder2.SetClipRect( 148, 45, 148+(iprogress-74), 90  );		
				}
				
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx+m_pBorder2.GetWindowSize().cx, m_WindowPosition.y - 16 );
			}
		}
#else
		if( m_DropStatus )
		{
			if( m_DropProgress > 86 )
			{
				m_DropProgress+=1;
			}
			else
			{
				m_DropProgress+= progresstime;
			}
			
			if( m_DropProgress > 96 )
			{
				m_DropStatus = FALSE;
				m_pBorder.MoveWindow( m_WindowPosition.x+81, m_WindowPosition.y );
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx, m_WindowPosition.y - 16 );
				
				m_EffectTime = g_LocalSystemTime;
				m_EffectProgress = 0;
			}
			else
			{			
				m_WindowPosition.y =  (int)m_DropProgress;

#ifdef _ACCLAIM_RUBICONADSYSTEM
				if( g_ViewRubiconAD )
					m_WindowPosition.y += g_RubiconADWindowSize.cy;
#endif
				
				int iprogress = _XFC_FloatToIntRet( m_EffectProgress );				
				m_pBorder.MoveWindow( m_WindowPosition.x+81, m_WindowPosition.y );
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx, m_WindowPosition.y - 16 );
			}
		}
		else
		{
			if( m_EffectProgress < 10 || m_EffectProgress > 70 )
			{
				m_EffectProgress+=1;
			}
			else
			{
				m_EffectProgress+=progresstime;
			}
			
			if( m_EffectProgress > 81 )
			{
				m_EffectProgress = 0.0f;
				m_EffectStaus = _XZoneWindowEffectClass_Stop;			
				
				m_pBorder.SetWindowSize( m_WindowSize.cx, m_WindowSize.cy );
				m_pBorder.MoveWindow( m_WindowPosition.x, m_WindowPosition.y );
				m_pBorder.SetClipRect( 0, 0, 162, 45  );
				m_pLeftStick.MoveWindow( m_WindowPosition.x-16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y - 16 );			
			}
			else
			{			
				int iprogress = _XFC_FloatToIntRet( m_EffectProgress );
				m_pBorder.SetWindowSize( (iprogress<<1), m_pBorder.GetWindowSize().cy );
				m_pBorder.MoveWindow( m_WindowPosition.x+81-iprogress, m_WindowPosition.y );
				m_pBorder.SetClipRect( 81-iprogress, 0, 81+iprogress, 45  );		
				
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx, m_WindowPosition.y - 16 );
			}
		}
#endif
	}
	else if( m_EffectStaus == _XZoneWindowEffectClass_Close )
	{
		m_EffectTime = g_LocalSystemTime;
		
#ifdef _XDEF_EXPAND_ZONENAME
		if( m_DropStatus )
		{
			if( m_DropProgress < 5 )
			{
				m_DropProgress+=0.5f;
			}
			else
			{
				m_DropProgress+=progresstime;
			}
			
			if( m_DropProgress > 96 )
			{				
				m_pBorder.MoveWindow( m_WindowPosition.x+182, m_WindowPosition.y );				
				m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
				m_pBorder2.SetWindowSize(0,0);
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx+m_pBorder2.GetWindowSize().cx, m_WindowPosition.y - 16 );
				
				m_EffectStaus = _XZoneWindowEffectClass_Stop;
				ShowWindow( FALSE );

				if(pfocusobject == this)
				{
					pfocusobject = NULL;
				}
				
				if( m_WorldNameDisplayMode )
				{
					m_WorldNameDisplayMode = FALSE;
					memset( m_ZoneName, 0, _XDEF_MAXZONENAMEWINDOWSTRINGSIZE );
				}

#ifdef _ACCLAIM_IGAADSYSTEM
				{
					XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;		

					if( pMainGameProcess->m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_NONE )
					{
						pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_AREACHANGE;
						g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_AREACHANGE );
						g_IGAADWrapper.ShowWindow( TRUE );
						g_IGAADWrapper.StartDisplay();
					}
				}
#endif
			}
			else
			{			
				m_WindowPosition.y =  96 - (int)m_DropProgress;

#ifdef _ACCLAIM_RUBICONADSYSTEM
				if( g_ViewRubiconAD )
					m_WindowPosition.y += g_RubiconADWindowSize.cy;
#endif
				m_pBorder.MoveWindow( m_WindowPosition.x+182, m_WindowPosition.y );
				m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
				m_pBorder2.SetWindowSize(0,0);
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx+m_pBorder2.GetWindowSize().cx, m_WindowPosition.y - 16 );
			}
		}
		else
		{
			if( m_EffectProgress < 10 || m_EffectProgress > 170 )
			{
				m_EffectProgress+=1;
			}
			else
			{
				m_EffectProgress+=progresstime;
			}
			
			if( m_EffectProgress > 182 )
			{
				m_DropStatus = TRUE;
				m_EffectProgress = 0.0f;
				
				m_pBorder.SetWindowSize( 0, m_WindowSize.cy );
				m_pBorder.MoveWindow( m_WindowPosition.x+182, m_WindowPosition.y );
				m_pBorder.SetClipRect( 0, 0, 1, 45  );
				
				m_pBorder2.SetWindowSize( 0, m_WindowSize.cy );
				m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
				m_pBorder2.SetClipRect( 148, 45, 148, 90  );
				m_pBorder2.SetWindowSize(0,0);				

				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx+m_pBorder2.GetWindowSize().cx, m_WindowPosition.y - 16 );				
			}
			else
			{			
				int iprogress = _XFC_FloatToIntRet( m_EffectProgress );
				if( iprogress > 108 )
				{
					m_pBorder.SetWindowSize( ((182-iprogress)<<1), m_pBorder.GetWindowSize().cy );
					m_pBorder.MoveWindow( m_WindowPosition.x+iprogress, m_WindowPosition.y );
					m_pBorder.SetClipRect( iprogress, 0, m_WindowSize.cx-iprogress, 45  );
				
					m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
					m_pBorder2.SetClipRect( 148, 45, 148, 90  );
					m_pBorder2.SetWindowSize(0,0);				
				}
				else
				{
					m_pBorder.SetWindowSize( (256-iprogress), m_pBorder.GetWindowSize().cy );
					m_pBorder.MoveWindow( m_WindowPosition.x+iprogress, m_WindowPosition.y );
					m_pBorder.SetClipRect( iprogress, 0, m_WindowSize.cx-iprogress, 45  );
					
					m_pBorder2.SetWindowSize( (108-iprogress), m_pBorder2.GetWindowSize().cy );
					m_pBorder2.MoveWindow( m_WindowPosition.x+256, m_WindowPosition.y );
					m_pBorder2.SetClipRect( 148, 45, 256-iprogress, 90  );
				}
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx+m_pBorder2.GetWindowSize().cx, m_WindowPosition.y - 16 );
			}
		}
#else
		if( m_DropStatus )
		{
			if( m_DropProgress < 5 )
			{
				m_DropProgress+=0.5f;
			}
			else
			{
				m_DropProgress+=progresstime;
			}
			
			if( m_DropProgress > 96 )
			{				
				m_pBorder.MoveWindow( m_WindowPosition.x+81, m_WindowPosition.y );
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx, m_WindowPosition.y - 16 );
				
				m_EffectStaus = _XZoneWindowEffectClass_Stop;
				ShowWindow( FALSE );
				
				if( m_WorldNameDisplayMode )
				{
					m_WorldNameDisplayMode = FALSE;
					memset( m_ZoneName, 0, _XDEF_MAXZONENAMEWINDOWSTRINGSIZE );
				}
			}
			else
			{			
				m_WindowPosition.y =  96 - (int)m_DropProgress;

#ifdef _ACCLAIM_RUBICONADSYSTEM
				if( g_ViewRubiconAD )
					m_WindowPosition.y += g_RubiconADWindowSize.cy;
#endif

				m_pBorder.MoveWindow( m_WindowPosition.x+81, m_WindowPosition.y );
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx, m_WindowPosition.y - 16 );
			}
		}
		else
		{
			if( m_EffectProgress < 10 || m_EffectProgress > 70 )
			{
				m_EffectProgress+=1;
			}
			else
			{
				m_EffectProgress+=progresstime;
			}
			
			if( m_EffectProgress > 81 )
			{
				m_DropStatus = TRUE;
				m_EffectProgress = 0.0f;				
				m_pBorder.SetWindowSize( 0, m_WindowSize.cy );
				m_pBorder.MoveWindow( m_WindowPosition.x+81, m_WindowPosition.y );
				m_pBorder.SetClipRect( 0, 0, 1, 45  );
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx, m_WindowPosition.y - 16 );				
			}
			else
			{			
				int iprogress = _XFC_FloatToIntRet( m_EffectProgress );
				m_pBorder.SetWindowSize( ((81-iprogress)<<1), m_pBorder.GetWindowSize().cy );
				m_pBorder.MoveWindow( m_WindowPosition.x+iprogress, m_WindowPosition.y );
				m_pBorder.SetClipRect( iprogress, 0, m_WindowSize.cx-iprogress, 45  );
				
				m_pLeftStick.MoveWindow( m_pBorder.GetWindowPos().x - 16, m_WindowPosition.y - 16 );
				m_pRightStick.MoveWindow( m_pBorder.GetWindowPos().x+m_pBorder.GetWindowSize().cx, m_WindowPosition.y - 16 );
			}
		}
#endif
	}
	else if( m_EffectStaus == _XZoneWindowEffectClass_Stop )
	{
		if( GetShowStatus() )
		{
			if( progresstime > (5000 / 4) )
			{
				StartAnimation( _XZoneWindowEffectClass_Close );
			}
		}		
	}

	return _XWindow::Process(pfocusobject);
}

BOOL _XWindow_ZoneName::CompareLastLandMark( LPTSTR landmarkname )
{
	if( !landmarkname ) return FALSE;

	if( strcmp( m_ZoneName, landmarkname ) == 0 ) return TRUE;

	return FALSE;
}