// XWindow_DeathBlow.cpp: implementation of the _XWindow_DeathBlow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_DeathBlow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_DeathBlow::_XWindow_DeathBlow()
{
	m_LeftDeathBlowButton  = NULL;
	m_RightDeathBlowButton = NULL;
	
	m_SpeedCounter = 0;
	m_SelectedEffectFrame = 0.0f;
	m_SelectedEffectPrevFrame = 0;
}

_XWindow_DeathBlow::~_XWindow_DeathBlow()
{
	m_LeftDeathBlowButton  = NULL;
	m_RightDeathBlowButton = NULL;
}

BOOL _XWindow_DeathBlow::Initialize( void )
{

	SetWindowMoveMode( _XWMOVE_FIXED );
	SetDocking( FALSE );
	SetDrawBorder( FALSE );

	int btnimageindex = g_MainInterfaceTextureArchive.FindResource( "chargebox_gauge.tga" ); //2004.06.22->oneway48 insert
	
	_XBTN_STRUCTURE leftblowbtnstruct = { TRUE, { 0, 0 }, { 52, 25 }, _XDEF_DEATHBLOW_LEFTBLOWBUTTON,
		btnimageindex,btnimageindex,btnimageindex,										  
		&g_MainInterfaceTextureArchive };
	m_LeftDeathBlowButton = new _XButton;
	m_LeftDeathBlowButton->Create( leftblowbtnstruct );
	m_LeftDeathBlowButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0,49,52,66 );
	m_LeftDeathBlowButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0,66,52,83 );
	m_LeftDeathBlowButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0,83,52,100 );
	InsertChildObject( m_LeftDeathBlowButton );
	
	
	_XBTN_STRUCTURE rightblowbtnstruct = { TRUE, { (m_WindowSize.cx>>1), 0 }, { 61, 35 }, _XDEF_DEATHBLOW_RIGHTBLOWBUTTON,
		btnimageindex,btnimageindex,btnimageindex,										  
		&g_MainInterfaceTextureArchive };
	m_RightDeathBlowButton = new _XButton;
	m_RightDeathBlowButton->Create( rightblowbtnstruct );
	m_RightDeathBlowButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 61, 0,122,35 );
	m_RightDeathBlowButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  61,35,122,70 );
	m_RightDeathBlowButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  61, 0,122,35 );
	InsertChildObject( m_RightDeathBlowButton );
		
	m_LevelUpBorderImage.Create( 0, 0, 58, 25, &g_MainInterfaceTextureArchive, btnimageindex );	
	m_LevelUpBorderImage.SetClipRect( 0, 24, 58, 49  );
	
	return TRUE;
}

void _XWindow_DeathBlow::SetDefaultPosition( void )
{	
	MoveWindow( (gnWidth>>1)-26,56);
	CheckOutside();
}

void _XWindow_DeathBlow::DestroyWindow( void )
{	
	_XWindow::DestroyWindow();
	
	
}

void _XWindow_DeathBlow::Draw( _XGUIObject*& pfocusobject )
{				
	
	_XWindow::Draw(pfocusobject);
	if( !this->m_ShowWindow ) return;	

	
	//2004.06.22->oneway48 insert
	if( m_LeftDeathBlowButton->IsButtonEnabled() )	
	{
		m_LevelUpBorderImage.MoveWindow( (gnWidth>>1)-29, 52);
		m_LevelUpBorderImage.DrawWithRegion();
				
		if( m_SpeedCounter == 0 ) m_SpeedCounter = g_LocalSystemTime;
		int elapsedtimetick = g_LocalSystemTime - m_SpeedCounter;
		m_SpeedCounter = g_LocalSystemTime;
		
		m_SelectedEffectFrame += ((FLOAT)elapsedtimetick / 100.0f);
		int selectedframe = (int)m_SelectedEffectFrame; 
		if( selectedframe > 3 )
		{		
			selectedframe = 0;
			m_SelectedEffectFrame = 0 ;
			m_SpeedCounter = 0;
		}
		
		if( m_SelectedEffectPrevFrame != selectedframe )
		{	
			if( selectedframe < 3 )
			{
				int y = 49+selectedframe*17;
				m_LeftDeathBlowButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0,y,52,y+17 );
			}
		}
		
		m_SelectedEffectPrevFrame = selectedframe;
		m_LeftDeathBlowButton->Draw(pfocusobject);
				
	}
}

BOOL _XWindow_DeathBlow::Process( _XGUIObject*& pfocusobject )
{		
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}


// 진이 꽉 차면 레벨업 버튼 활성화
void _XWindow_DeathBlow::SetButtonStatus( BOOL Show )
{
	if( Show )
	{		
		if( m_LeftDeathBlowButton )
		{
			m_LeftDeathBlowButton->EnableWindow( TRUE );
			m_LeftDeathBlowButton->ShowWindow( TRUE );			
		}
		if( m_RightDeathBlowButton )
		{			
			m_RightDeathBlowButton->ShowWindow( FALSE );
		}
		
		ShowWindow( TRUE );
	}
	else
	{		
		if( m_LeftDeathBlowButton )
		{
			m_LeftDeathBlowButton->EnableWindow( FALSE );
			m_LeftDeathBlowButton->ShowWindow( FALSE );			
		}
		if( m_RightDeathBlowButton )
		{			
			m_RightDeathBlowButton->ShowWindow( FALSE );
		}
		
		if( g_CurrentFocusedObject == m_LeftDeathBlowButton )
		{
			g_CurrentFocusedObject = NULL;
		}
		
		if(  g_CurrentFocusedObject == m_RightDeathBlowButton )
		{
			g_CurrentFocusedObject = NULL;
		}
		
		ShowWindow( FALSE );		
	}
}
