// XWindow_GMCommand.cpp: implementation of the _XWindow_GMCommand class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "XWindow_GMCommand.h"

#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"
#include "XProc_ProcessDefine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_GMCommand::_XWindow_GMCommand()
{
	m_WindowAnimationMode = _XGMWINDOW_ANIMATIONMODE_CLOSE;
}

_XWindow_GMCommand::~_XWindow_GMCommand()
{

}


BOOL _XWindow_GMCommand::Initialize( void )
{	
	SetDrawBorder( FALSE );

	int gmwindowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_GMWindow.tga" );
	_XBTN_STRUCTURE openbtnstruct = 
	{ 
		TRUE, { 0, 0 }, { 154, 32 }, _XDEF_GMWINDOW_OPENBUTTON,
		gmwindowimageindex,gmwindowimageindex,gmwindowimageindex, 
		&g_MainInterfaceTextureArchive 
	};

	_XButton* pOpenbutton = new _XButton;
	pOpenbutton->Create( openbtnstruct );		
	pOpenbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 0, 154, 32 );
	pOpenbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0, 0, 154, 32 );
	pOpenbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0, 0, 154, 32 );
	InsertChildObject( pOpenbutton );

	_XBTN_STRUCTURE commandbtnstruct = 
	{ 
		TRUE, { m_WindowPosition.x+45, m_WindowPosition.y+28 }, 
		{ 156, 18 }, _XDEF_GMWINDOW_COMMANDBUTTON0,
		gmwindowimageindex,gmwindowimageindex,gmwindowimageindex, 
		&g_MainInterfaceTextureArchive 
	};

	int	btnstring[_XDEF_GMCOMMAND_MAXCOUNT];
		
	btnstring[0] = ID_STRING_GMCOMMAND_GMOPTION;
	btnstring[1] = ID_STRING_GMCOMMAND_COORD;
	btnstring[2] = ID_STRING_GMCOMMAND_INFO;
	btnstring[3] = ID_STRING_GMCOMMAND_CONTROL;
	btnstring[4] = ID_STRING_GMCOMMAND_BUSINESSTALK;
	btnstring[5] = ID_STRING_GMCOMMAND_ADMINTALK;
	btnstring[6] = ID_STRING_GMCOMMAND_MONSTER;
	btnstring[7] = ID_STRING_GMCOMMAND_EVENT;
	btnstring[8] = ID_STRING_NEW_2667;
	btnstring[9] = ID_STRING_NEW_2668;
	
	for( int i = 0; i < _XDEF_GMCOMMAND_MAXCOUNT; i++ )
	{
		m_CommandButton[i].Create( commandbtnstruct );		
		m_CommandButton[i].SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 33, 156, 51 );
		m_CommandButton[i].SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0, 58, 156, 76 );
		m_CommandButton[i].SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0, 33, 156, 51 );
		m_CommandButton[i].SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, btnstring[i], _XFONT_ALIGNTYPE_LEFT, 16, -1 );
		m_CommandButton[i].ShowWindow( FALSE );
		commandbtnstruct.commandid++;
		commandbtnstruct.position.y += 18;
	}

	return TRUE;
}

void _XWindow_GMCommand::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_GMCommand::CheckOutside( void )
{

}

void _XWindow_GMCommand::Draw( _XGUIObject*& pfocusobject )
{
	
	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	_XWindow::Draw(pfocusobject);

	for( int i = 0; i < _XDEF_GMCOMMAND_MAXCOUNT; i++ )
	{
		m_CommandButton[i].Draw( pfocusobject );
	}

	g_XBaseFont->SetColor(0xFFFEE502);
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->Puts( m_WindowPosition.x + 50, m_WindowPosition.y + 12 , _T("G.M. Command list") );
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
}

#define _XDEF_GMWINDOWSCROLLSPEED	22

BOOL _XWindow_GMCommand::Process( _XGUIObject*& pfocusobject )
{	
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	if( m_WindowAnimationMode == _XGMWINDOW_ANIMATIONMODE_OPEN )
	{
		if( m_WindowPosition.x != (gnWidth - (1024-869)) )
		{
			if( m_WindowPosition.x > (gnWidth - (1024-869)) )
			{
				int nextposition = m_WindowPosition.x - _XDEF_GMWINDOWSCROLLSPEED;
				if( nextposition < (gnWidth - (1024-869)) )
					nextposition = (gnWidth - (1024-869)) ;
				
				MoveWindow( nextposition, m_WindowPosition.y );
			}
			else
			{
				int nextposition = m_WindowPosition.x + _XDEF_GMWINDOWSCROLLSPEED;
				if( nextposition > (gnWidth - (1024-869)) )
					nextposition = (gnWidth - (1024-869));
				
				MoveWindow( nextposition, m_WindowPosition.y );
			}

			for( int i = 0; i < _XDEF_GMCOMMAND_MAXCOUNT; i++ )
			{
				if( !m_CommandButton[i].GetShowStatus() )
				{
					m_CommandButton[i].ShowWindow( TRUE );
					break;
				}
			}

			return TRUE;
		}
		else
		{
			for( int i = 0; i < _XDEF_GMCOMMAND_MAXCOUNT; i++ )
			{
				if( !m_CommandButton[i].GetShowStatus() )
				{
					m_CommandButton[i].ShowWindow( TRUE );
					break;
				}
			}
		}
	}
	else if( m_WindowAnimationMode == _XGMWINDOW_ANIMATIONMODE_CLOSE )
	{
		if( m_WindowPosition.x != (gnWidth - (1024-980)) )
		{
			if( m_WindowPosition.x > (gnWidth - (1024-980)) )
			{
				int nextposition = m_WindowPosition.x - _XDEF_GMWINDOWSCROLLSPEED;
				if( nextposition < (gnWidth - (1024-980)) )
					nextposition = (gnWidth - (1024-980));
				
				MoveWindow( nextposition, m_WindowPosition.y );
			}
			else
			{
				int nextposition = m_WindowPosition.x + _XDEF_GMWINDOWSCROLLSPEED;
				if( nextposition > (gnWidth - (1024-980)) )
					nextposition = (gnWidth - (1024-980));

				MoveWindow( nextposition, m_WindowPosition.y );
			}

			for( int i = _XDEF_GMCOMMAND_MAXCOUNT-1; i > -1; i-- )
			{
				if( m_CommandButton[i].GetShowStatus() )
				{
					m_CommandButton[i].ShowWindow( FALSE );
					break;
				}
			}

			return TRUE;
		}
		else
		{
			for( int i = 0; i < _XDEF_GMCOMMAND_MAXCOUNT; i++ )
			{
				if( m_CommandButton[i].GetShowStatus() )
				{
					m_CommandButton[i].ShowWindow( FALSE );
					break;
				}
			}
		}
	}

	for( int i = 0; i < _XDEF_GMCOMMAND_MAXCOUNT; i++ )
	{
		m_CommandButton[i].Process( pfocusobject );
	}

	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}	


void _XWindow_GMCommand::SetWindowAnimationMode( _XGMWINDOW_ANIMATIONMODE windowanimationmode )
{
	m_WindowAnimationMode = windowanimationmode;
	
	if( m_WindowAnimationMode == _XGMWINDOW_ANIMATIONMODE_OPEN )
	{
		if( _XWindow::m_GlobalPlaySoundEffect )
			_XPlayInterfaceSound( g_WindowOpenSoundIndex, true );

		if( m_WindowPosition.x == (gnWidth - (1024-980)) )
		{
			for( int i = 0; i < _XDEF_GMCOMMAND_MAXCOUNT; i++ )
			{
				m_CommandButton[i].ShowWindow( FALSE );
			}			
		}
	}
	else if( m_WindowAnimationMode == _XGMWINDOW_ANIMATIONMODE_CLOSE )
	{
		if( _XWindow::m_GlobalPlaySoundEffect )
			_XPlayInterfaceSound( g_WindowCloseSoundIndex, true );

		if( m_WindowPosition.x == (gnWidth - (1024-869) ) )
		{
			for( int i = 0; i < _XDEF_GMCOMMAND_MAXCOUNT; i++ )
			{
				m_CommandButton[i].ShowWindow( TRUE );
			}			
		}		
	}
}

void _XWindow_GMCommand::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );

	for( int i = 0; i < _XDEF_GMCOMMAND_MAXCOUNT; i++ )
	{
		m_CommandButton[i].MoveWindow( m_WindowPosition.x+45, m_WindowPosition.y+28+(i*18) );
	}
}

void _XWindow_GMCommand::WindowPosChanging( int& X, int& Y )
{
	
}










