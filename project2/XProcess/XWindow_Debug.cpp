// XWindow_Debug.cpp: implementation of the _XWindow_Debug class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _XDWDEBUG	

#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_Debug.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define _XDEF_DEBUGWINDOWMINSIZE_WIDTH		314
#define _XDEF_DEBUGWINDOWMINSIZE_HEIGHT		100

#define _XDEF_DEBUGWINDOWMAXSIZE_WIDTH		1024
#define _XDEF_DEBUGWINDOWMAXSIZE_HEIGHT		768

extern D3DCOLOR g_ChattingStringColor[16];
extern void _XKernelCommandProcedure( LPSTR commandstring );

void __stdcall _XIMECallback_DebugReturn( _XIMEContainer* pIMEContainer )
{	
	_XWindow_Debug* debugwindow = (_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );	
	if( !debugwindow ) return;

	TCHAR tbuf[100];
	memset( tbuf, 0, sizeof(TCHAR) * 100 );
    strcpy(tbuf, g_DebugCommandEditContainer.GetText());

	if( strlen( tbuf ) == 0 ) return;

	memset( debugwindow->m_LastSendedCommand, 0, sizeof(TCHAR) * 51 );
	strncpy( debugwindow->m_LastSendedCommand, tbuf, 50 );
    
    _XKernelCommandProcedure( tbuf );
	_XIMEKERNEL.ResetCombinationBuffer();
	g_DebugCommandEditContainer.ClearBuffer();	
}


void __stdcall _XIMECallback_DebugEscape( _XIMEContainer* pIMEContainer )
{	
	_XWindow_Debug* debugwindow = (_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );		
	_XIMEKERNEL.ResetCombinationBuffer();
	g_DebugCommandEditContainer.ClearBuffer();
	_XIMEKERNEL.SetFocus( NULL );
// 2004.05.08->oneway delete
//	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;		
//	eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();	
}

void __stdcall _XIMECallback_DebugTab( _XIMEContainer* pIMEContainer )
{	
	_XWindow_Debug* debugwindow = (_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );	
	
	_XIMEKERNEL.ResetCombinationBuffer();
	g_DebugCommandEditContainer.ClearBuffer();
	g_DebugCommandEditContainer.AddString( debugwindow->m_LastSendedCommand, 
										   strlen(debugwindow->m_LastSendedCommand) );
}

_XWindow_Debug::_XWindow_Debug()
{
	m_Resizing				= FALSE;
	m_ResizeMode			= _XWRESIZE_NONE;

	m_DebugInfoListScrollBar= NULL;
	m_pScrbarFocusedObject	= NULL;

	m_DebugInfoListScrollPos= 0;

	m_MaxViewCount			= 10;
	m_DebugInfoMode			= 0;
}

_XWindow_Debug::~_XWindow_Debug()
{
	DestroyWindow();	
}

BOOL _XWindow_Debug::Initialize( void )
{		
	SAFE_DELETE( m_DebugInfoListScrollBar );
	m_DebugInfoListScrollBar = new _XVScrollBar;
	_XSCRBAR_STRUCTURE DebugInfolistscrbarstructure = 
	{
		TRUE, {m_WindowPosition.x+2,m_WindowPosition.y + 2 }, 
			10, m_WindowSize.cy - 48, // width, height
			10,		 // btnysize
			20,		 // trackbar size
			50, m_MaxViewCount, 0,
			g_MainInterfaceTextureArchive.FindResource( "scrollbar.tga" ),
			g_MainInterfaceTextureArchive.FindResource( "scrollbar.tga" ),
			g_MainInterfaceTextureArchive.FindResource( "scrollbar.tga" ),
			g_MainInterfaceTextureArchive.FindResource( "scrollbar.tga" ),
			g_MainInterfaceTextureArchive.FindResource( "scrollbar.tga" ),
			&g_MainInterfaceTextureArchive
	};
	
	m_DebugInfoListScrollBar->Create( DebugInfolistscrbarstructure );
	m_DebugInfoListScrollBar->ShowWindow( TRUE );
	m_DebugInfoListScrollBar->SetViewBorder( TRUE );
	m_DebugInfoListScrollBar->SetTotalLineCount(0);	
	m_DebugInfoListScrollBar->UpdateData();

	_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
	{
		{ 0,  0, 10, 10 },
		{ 20, 0, 30, 10 },
		{ 0, 10, 10, 20 },
		{ 20,10, 30, 20 },
		{ 20,20, 30, 40 }
	};
	m_DebugInfoListScrollBar->SetClipRect( cliprectstructure );

	m_LeftImage.Create( m_WindowPosition.x,
						m_WindowPosition.y + m_WindowSize.cy - 45, 
						m_WindowPosition.x + 18,
		                m_WindowPosition.y + m_WindowSize.cy - 1,
		                &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "chatting.tga" ) );
	RECT cliprect = { 72, 0, 90, 44  };
	m_LeftImage.SetClipRect( cliprect );

	_XCHECKBTN_STRUCTURE debuginfomodebtnstruct = { TRUE, { m_WindowPosition.x+18, m_WindowPosition.x + m_WindowSize.cy - 22 }, { 72, 20 }, 
										_XDEF_XDEBUGINFOMODE_BASE,
										g_MainInterfaceTextureArchive.FindResource( "chatting.tga" ),										
										g_MainInterfaceTextureArchive.FindResource( "chatting.tga" ), 
										&g_MainInterfaceTextureArchive };
			
	for( int i = 0; i < 4; i++ )
	{			
		m_DebugInfoModeButton[i].Create( debuginfomodebtnstruct );
		m_DebugInfoModeButton[i].MoveWindow( m_WindowPosition.x+18+(i*73), m_WindowPosition.y + m_WindowSize.cy - 23 );
		m_DebugInfoModeButton[i].SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 0, 0,  72, 20 );
		m_DebugInfoModeButton[i].SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 20, 72, 40 );		
		m_DebugInfoModeButton[i].SetCheck( FALSE );
		m_DebugInfoModeButton[i].SetButtonSwapMode( TRUE );
		debuginfomodebtnstruct.commandid++;
	}

	m_DebugInfoModeButton[0].SetCheck( TRUE );
	m_DebugInfoModeButton[0].SetButtonText( _T("Game log") );	
	m_DebugInfoModeButton[1].SetButtonText( _T("Network") );
	m_DebugInfoModeButton[2].SetButtonText( _T("System") );
	m_DebugInfoModeButton[3].SetButtonText( _T("Results") );
		
	// Set window position...
	int xdebugwinxpos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_XDEBUGWINDOWXPOS, 0, TRUE );
	if( xdebugwinxpos < 0 ) xdebugwinxpos = 0;
	int xdebugwinypos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_XDEBUGWINDOWYPOS, gnHeight-_XDEF_DEBUGWINDOWMINSIZE_HEIGHT, TRUE );
	if( xdebugwinypos < 0 ) xdebugwinxpos = 0;
	int xdebugwinwidth  = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_XDEBUGWINDOWWIDTH , _XDEF_DEBUGWINDOWMAXSIZE_WIDTH  ,TRUE );
	int xdebugwinheight = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_XDEBUGWINDOWHEIGHT, _XDEF_DEBUGWINDOWMINSIZE_HEIGHT ,TRUE );
	ResizeWindow( xdebugwinxpos, xdebugwinypos, xdebugwinxpos+xdebugwinwidth, xdebugwinypos+xdebugwinheight );
	
	CheckOutside();

	POINT pos = GetWindowPos();
	SIZE size = GetWindowSize();
	POINT imepos;
	imepos.x =  pos.x + 20;
	imepos.y =  pos.y + size.cy - 38;		

	g_DebugCommandEditContainer.ResetContainer();
	g_DebugCommandEditContainer.MoveWindow( imepos.x, imepos.y );
	g_DebugCommandEditContainer.ResizeWindow( _XDEF_DEBUGWINDOWMINSIZE_WIDTH - 40, 12 );
	g_DebugCommandEditContainer.ShowWindow(TRUE);
	g_DebugCommandEditContainer.SetMaxLength(50);
	g_DebugCommandEditContainer.SetReturnKeyCallback( _XIMECallback_DebugReturn );	
	g_DebugCommandEditContainer.SetEscapeKeyCallback( _XIMECallback_DebugEscape );
	g_DebugCommandEditContainer.SetTabKeyCallback( _XIMECallback_DebugTab );
	memset( m_LastSendedCommand, 0, sizeof(TCHAR) * 51 );
		
	return TRUE;
}

void _XWindow_Debug::SetDefaultPosition( void )
{		
	ResizeWindow( 0, 0,_XDEF_DEBUGWINDOWMINSIZE_WIDTH, _XDEF_DEBUGWINDOWMINSIZE_HEIGHT );	
	CheckOutside();
}

void _XWindow_Debug::SaveWindowProperty( void )
{
	// save xdebug window properties...	
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_XDEBUGWINDOWXPOS,   GetWindowPos().x );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_XDEBUGWINDOWYPOS,   GetWindowPos().y );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_XDEBUGWINDOWWIDTH,	 GetWindowSize().cx );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_XDEBUGWINDOWHEIGHT, GetWindowSize().cy );	
}

void _XWindow_Debug::DestroyWindow( void )
{	
	_XWindow::DestroyWindow();
	SAFE_DELETE( m_DebugInfoListScrollBar );	
}

void _XWindow_Debug::ShowWindow( BOOL show )
{		
	_XWindow::ShowWindow( show );	

	if( !show )
	{
		_XIMEKERNEL.ResetCombinationBuffer();
		g_DebugCommandEditContainer.ClearBuffer();

		_XIMEKERNEL.SetFocus( NULL );// 2004.05.07->oneway48 insert
		//CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;		
		//eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();	
	}
}

void _XWindow_Debug::MoveWindow( int X, int Y )
{			
	// Clipping from screen size	
	_XWindow::MoveWindow( X, Y );

	g_DebugCommandEditContainer.MoveWindow( X+20, this->m_WindowPosition.y + this->m_WindowSize.cy - 38 );		
		
	m_LeftImage.MoveWindow( m_WindowPosition.x , m_WindowPosition.y + m_WindowSize.cy - 45 );	
	m_DebugInfoListScrollBar->MoveWindow( m_WindowPosition.x + 2, m_WindowPosition.y + 2 );

	for( int i = 0; i < 4; i++ )
	{
		m_DebugInfoModeButton[i].MoveWindow( m_WindowPosition.x+18+(i*73), m_WindowPosition.y + m_WindowSize.cy - 22 );
	}
}

void _XWindow_Debug::ResizeWindow( int SX, int SY, int EX, int EY )
{
	if( EX-SX < _XDEF_DEBUGWINDOWMINSIZE_WIDTH && EY-SY < _XDEF_DEBUGWINDOWMINSIZE_HEIGHT ) return;
	if( EX-SX > _XDEF_DEBUGWINDOWMAXSIZE_WIDTH && EY-SY > _XDEF_DEBUGWINDOWMAXSIZE_HEIGHT ) return;

	if( EX-SX > _XDEF_DEBUGWINDOWMAXSIZE_WIDTH )
	{
		if( SX == m_WindowPosition.x )
		{
			EX = m_WindowPosition.x + _XDEF_DEBUGWINDOWMAXSIZE_WIDTH;
		}
		else
		{
			SX = m_WindowPosition.x + m_WindowSize.cx - _XDEF_DEBUGWINDOWMAXSIZE_WIDTH;
		}
	}
	else if( EX-SX < _XDEF_DEBUGWINDOWMINSIZE_WIDTH )
	{
		if( SX == m_WindowPosition.x )
		{
			EX = m_WindowPosition.x + _XDEF_DEBUGWINDOWMINSIZE_WIDTH;
		}
		else
		{
			SX = m_WindowPosition.x + m_WindowSize.cx - _XDEF_DEBUGWINDOWMINSIZE_WIDTH;
		}
	}

	if( EY-SY > _XDEF_DEBUGWINDOWMAXSIZE_HEIGHT )
	{
		if( SY == m_WindowPosition.y )
		{
			EY = m_WindowPosition.y + _XDEF_DEBUGWINDOWMAXSIZE_HEIGHT;
		}
		else
		{
			SY = m_WindowPosition.y + m_WindowSize.cy - _XDEF_DEBUGWINDOWMAXSIZE_HEIGHT;
		}
	}
	else if( EY-SY < _XDEF_DEBUGWINDOWMINSIZE_HEIGHT )
	{
		if( SY == m_WindowPosition.y )
		{
			EY = m_WindowPosition.y + _XDEF_DEBUGWINDOWMINSIZE_HEIGHT;
		}
		else
		{
			SY = m_WindowPosition.y + m_WindowSize.cy - _XDEF_DEBUGWINDOWMINSIZE_HEIGHT;
		}
	}

	m_WindowPosition.x = SX;
	m_WindowPosition.y = SY;

	m_WindowSize.cx = EX-SX;
	m_WindowSize.cy = EY-SY;

	g_DebugCommandEditContainer.MoveWindow( this->m_WindowPosition.x + 20, this->m_WindowPosition.y + this->m_WindowSize.cy - 38 );	
	
	m_solidborderstatic.MoveWindow( SX, SY+1, EX, EY );

	m_LeftImage.MoveWindow( m_WindowPosition.x , m_WindowPosition.y + m_WindowSize.cy - 45 );
	
	for( int i = 0; i < 4; i++ )
	{
		m_DebugInfoModeButton[i].MoveWindow( m_WindowPosition.x+18+(i*73), m_WindowPosition.y + m_WindowSize.cy - 22 );
	}

	m_DebugInfoListScrollBar->SetWindowHeight( m_WindowSize.cy - 49 );
	m_DebugInfoListScrollBar->MoveWindow( m_WindowPosition.x+2,m_WindowPosition.y + 2 );

	m_MaxViewCount = ((m_WindowSize.cy-50) / 12);
	m_DebugInfoListScrollBar->SetViewLineCount( m_MaxViewCount );
	m_DebugInfoListScrollBar->RebuildTrackBar();
}

#define _XDEF_DEBUGMESSAGEBLINKTIME		10000


void _XWindow_Debug::Draw( _XGUIObject*& pfocusobject )
{	
	_XWindow::Draw(pfocusobject);

	//if( !this->m_Enable     ) return;
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// Window rect
	_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, _XSC_DEFAULT_BLACK );
	// Scroll bar rect
	_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+13, m_WindowPosition.y+m_WindowSize.cy-46, 0.0f, _XSC_DEFAULT_BLACK );
	// Control window 
	_XDrawRectAngle( m_WindowPosition.x+16, m_WindowPosition.y+m_WindowSize.cy-45, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, _XSC_DEFAULT_BLACK );
	
	m_LeftImage.DrawWithRegion();
	
	// draw DebugInfo list...
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
	
	if( !g_DebugOutputlist[m_DebugInfoMode].listEmpty() )
	{
		//g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );

		g_DebugOutputlist[m_DebugInfoMode].resetList();
		for( int i = 0; i < m_DebugInfoListScrollPos; i++ ) g_DebugOutputlist[m_DebugInfoMode].nextItem();		
		
		_XStringItem*	curstring;
		int				ypos = 0;		
		int				textattrib = 0;
		
		int maxviewline = m_MaxViewCount - 1;
				
		do
		{			
			curstring = (_XStringItem*)g_DebugOutputlist[m_DebugInfoMode].currentItem();			
			textattrib = curstring->Get_Attribute();
			int separatedline = 0;
			
			if( textattrib == 1 )
			{
				g_XBaseFont->SetColor(0xFFFFFFFF);//g_ChattingStringColor[ textattrib ]);
				separatedline = g_XBaseFont->Puts_Separate( this->m_WindowPosition.x + 20, (m_WindowPosition.y+4) + (ypos * 12),
						                                   curstring->Get_String(), m_WindowSize.cx - 28 );
			}
			else if( textattrib == 4 )
			{
				g_XBaseFont->SetColor(_XSYSCOLOR_WARNING);
				separatedline = g_XBaseFont->Puts_Separate( this->m_WindowPosition.x + 20, (m_WindowPosition.y+4) + (ypos * 12),
														   curstring->Get_String(), m_WindowSize.cx - 28 );
			}
			else if( textattrib == 5 )
			{			
				g_XBaseFont->SetColor(_XSYSCOLOR_WARNING_HIGHLIGHT);
				separatedline = g_XBaseFont->Puts_Separate( this->m_WindowPosition.x + 20, (m_WindowPosition.y+4) + (ypos * 12),
									                       curstring->Get_String(), m_WindowSize.cx - 28 );
			}
			else
			{			
				g_XBaseFont->SetColor(g_ChattingStringColor[ textattrib ]);
				separatedline = g_XBaseFont->Puts_Separate( this->m_WindowPosition.x + 20, (m_WindowPosition.y+4) + (ypos * 12),
									                       curstring->Get_String(), m_WindowSize.cx - 28 );
			}
			
			ypos+=separatedline;
			
			if( ypos >= maxviewline ) break;
			
			g_DebugOutputlist[m_DebugInfoMode].nextItem();
			
		}while( !g_DebugOutputlist[m_DebugInfoMode].atHeadOfList() );

		g_XBaseFont->Flush();

		//g_XBaseFont->DisableGrowMode();
	}		

	{
		for( int i = 0; i < 4; i++ )
		{
			if( i != m_DebugInfoMode )
				m_DebugInfoModeButton[i].Draw( pfocusobject );
		}

		// Input window 
		_XDrawRectAngle( m_WindowPosition.x+18, m_WindowPosition.y+m_WindowSize.cy-44  , m_WindowPosition.x+m_WindowSize.cx - 3, m_WindowPosition.y+m_WindowSize.cy-22, 0.0f, D3DCOLOR_ARGB(255,141,141,141) );

		m_DebugInfoModeButton[m_DebugInfoMode].Draw( pfocusobject );
	}
	
	m_DebugInfoListScrollPos = m_DebugInfoListScrollBar->GetLineScrollPos();
	m_DebugInfoListScrollBar->Draw( m_pScrbarFocusedObject );

	g_DebugCommandEditContainer.Draw();		
}

BOOL _XWindow_Debug::CheckMousePosition( void )
{
	if(  _XWindow::CheckMousePosition() ) return TRUE;	
	return FALSE;
}

#define _XDEF_RESIZETRACKSIZE	4

BOOL _XWindow_Debug::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( m_pScrbarFocusedObject ) return TRUE;
	
	if( !m_ObjectList.listEmpty() )
	{
		m_ObjectList.resetList();
		_XGUIObject* currentObject = NULL;
		
		do
		{
			currentObject = (_XGUIObject*)m_ObjectList.currentItem();
			
			if(currentObject) currentObject->Process( pfocusobject );
			
			m_ObjectList.nextItem();
		}while( !m_ObjectList.atHeadOfList() );
	}

	for( int i = 0; i < 4; i++ )
	{
		m_DebugInfoModeButton[i].Process( pfocusobject );
	}
		
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
	BOOL changedmousecursor = FALSE;
	
	if( mousestate && scrnpos && !m_pScrbarFocusedObject )
	{			
		if( !pfocusobject )
		{
			if( CheckMousePosition() )
			{	
				if( gpInput->GetMouseState()->bButton[0] )
				{
					if( ((scrnpos->x > this->m_WindowPosition.x + 20) && 
						 (scrnpos->x < this->m_WindowPosition.x + m_WindowSize.cx - 3) &&
						 (scrnpos->z > this->m_WindowPosition.y + this->m_WindowSize.cy - 44) && 
						 (scrnpos->z < this->m_WindowPosition.y + this->m_WindowSize.cy - 22) ) ) 
					{													
						if( !g_DebugCommandEditContainer.GetFocus() )
						{	
							g_DebugCommandEditContainer.SetFocus();
						}
					}
				}

#define _XDEF_FULLRESIZEMODE
#ifdef	_XDEF_FULLRESIZEMODE
				if( scrnpos->x < m_WindowPosition.x+_XDEF_RESIZETRACKSIZE )
				{	
					if( scrnpos->z < m_WindowPosition.y+_XDEF_RESIZETRACKSIZE )
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;												
							m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
							m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
						}											
						
						m_ResizeMode = _XWRESIZE_LEFTBOTTOM;
						changedmousecursor = TRUE;
					}
					else if( scrnpos->z >= m_WindowPosition.y+m_WindowSize.cy-_XDEF_RESIZETRACKSIZE )
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;												
							m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
							m_DragStartPos.y = m_WindowPosition.y+m_WindowSize.cy - scrnpos->z;
						}
												
						m_ResizeMode = _XWRESIZE_LEFTBOTTOM;
						changedmousecursor = TRUE;
					}
					else
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;												
							m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
						}
												
						m_ResizeMode = _XWRESIZE_LEFT;
						changedmousecursor = TRUE;
					}

				}
				else if( scrnpos->x >= m_WindowPosition.x+m_WindowSize.cx-_XDEF_RESIZETRACKSIZE )
				{	
					if( scrnpos->z < m_WindowPosition.y+_XDEF_RESIZETRACKSIZE )
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;												
							m_DragStartPos.x = m_WindowPosition.x+m_WindowSize.cx - scrnpos->x;
							m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
						}
												
						m_ResizeMode = _XWRESIZE_RIGHTTOP;
						changedmousecursor = TRUE;
					}
					else if( scrnpos->z >= m_WindowPosition.y+m_WindowSize.cy-_XDEF_RESIZETRACKSIZE )
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;												
							m_DragStartPos.x = m_WindowPosition.x+m_WindowSize.cx - scrnpos->x;
							m_DragStartPos.y = m_WindowPosition.y+m_WindowSize.cy - scrnpos->z;
						}
						
						m_ResizeMode = _XWRESIZE_RIGHTBOTTOM;
						changedmousecursor = TRUE;
					}
					else
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;												
							m_DragStartPos.x = m_WindowPosition.x+m_WindowSize.cx - scrnpos->x;							
						}
												
						m_ResizeMode = _XWRESIZE_RIGHT;
						changedmousecursor = TRUE;
					}
				}
				else 
#endif				
				if( scrnpos->z < m_WindowPosition.y+_XDEF_RESIZETRACKSIZE )
				{
					if( mousestate->bButton[0] )
					{
						pfocusobject = (_XGUIObject*)this;
						m_Resizing = TRUE;											
						m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
					}
										
					m_ResizeMode = _XWRESIZE_TOP;
					changedmousecursor = TRUE;
				}
				else if( scrnpos->z >= m_WindowPosition.y+m_WindowSize.cy-_XDEF_RESIZETRACKSIZE )
				{
					if( mousestate->bButton[0] )
					{
						pfocusobject = (_XGUIObject*)this;
						m_Resizing = TRUE;											
						m_DragStartPos.y = m_WindowPosition.y + m_WindowSize.cy - scrnpos->z;
					}
										
					m_ResizeMode = _XWRESIZE_BOTTOM;
					changedmousecursor = TRUE;
				}					
				else
				{		
					if( mousestate->bButton[0] )
					{
						pfocusobject = (_XGUIObject*)this;
						if( (m_WindowMoveMode != _XWMOVE_FIXED) && !m_Dragging )
						{					
							m_Dragging = TRUE;
							m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
							m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
						}
					}					
				}									
			}			
		}

		// 리사이즈 모드가 아니면 이전 마우스 포인터 복구
		if( ( !pfocusobject || pfocusobject == this ) && (changedmousecursor || m_Resizing) )
		{
			switch( m_ResizeMode ) {
			case _XWRESIZE_LEFTTOP :
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_DIAGONAL_LEFT );
				break;
			case _XWRESIZE_RIGHTTOP :				
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_DIAGONAL_RIGHT );
				break;
			case _XWRESIZE_LEFTBOTTOM :
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_DIAGONAL_RIGHT );				
				break;
			case _XWRESIZE_RIGHTBOTTOM :
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_DIAGONAL_LEFT );
				break;
			case _XWRESIZE_LEFT :
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_HORIZ );
				break;
			case _XWRESIZE_TOP :
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_VERT );
				break;
			case _XWRESIZE_RIGHT :
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_HORIZ );
				break;
			case _XWRESIZE_BOTTOM :	
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_VERT );
				break;			
			default:
				break;
			}			
		}
		else 
		{
			//if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE ) eodapp->ReadyCursor( _XMOUSETYPE_PEACE );
			//else eodapp->ReadyCursor( _XMOUSETYPE_BATTLE );
		}
		
		if( !mousestate->bButton[0] )
		{
			m_Dragging = FALSE;	
			m_Resizing = FALSE;
			m_DragStartPos.x = m_DragStartPos.y = 0;			

			if( CheckMousePosition() )
			{	
				if( pfocusobject == (_XGUIObject*)this )
				{
					if( m_CommandID )
					{						
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CommandID, 0), (LPARAM)gHWnd);
						mousestate->bButton[0] = 0;
					}						
					pfocusobject = NULL;
					return TRUE;
				}		
			}
			if( pfocusobject == (_XGUIObject*)this )
			{
				pfocusobject = NULL;
				return FALSE;
			}			
		}
		else if( pfocusobject == (_XGUIObject*)this )
		{
			if( m_Resizing )
			{					
				RECT winrect;
				switch(m_ResizeMode) 
				{
				case _XWRESIZE_LEFTTOP:
					winrect.left   = scrnpos->x+m_DragStartPos.x; 
					winrect.top    = scrnpos->z+m_DragStartPos.y;
					winrect.right  = m_WindowPosition.x+m_WindowSize.cx;
					winrect.bottom = m_WindowPosition.y+m_WindowSize.cy; 
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_RIGHTTOP:
					winrect.left   = m_WindowPosition.x;
					winrect.top    = scrnpos->z+m_DragStartPos.y;
					winrect.right  = scrnpos->x+m_DragStartPos.x;
					winrect.bottom = m_WindowPosition.y+m_WindowSize.cy;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_LEFTBOTTOM:
					winrect.left   = scrnpos->x+m_DragStartPos.x;
					winrect.top    = m_WindowPosition.y;
					winrect.right  = m_WindowPosition.x+m_WindowSize.cx;
					winrect.bottom = scrnpos->z+m_DragStartPos.y;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_RIGHTBOTTOM:
					winrect.left   = m_WindowPosition.x;
					winrect.top    = m_WindowPosition.y;
					winrect.right  = scrnpos->x+m_DragStartPos.x;
					winrect.bottom = scrnpos->z+m_DragStartPos.y;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_TOP:
					winrect.left   = m_WindowPosition.x;
					winrect.top    = scrnpos->z+m_DragStartPos.y;
					winrect.right  = m_WindowPosition.x+m_WindowSize.cx;
					winrect.bottom = m_WindowPosition.y+m_WindowSize.cy;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_BOTTOM:
					winrect.left   = m_WindowPosition.x;
					winrect.top    = m_WindowPosition.y;
					winrect.right  = m_WindowPosition.x+m_WindowSize.cx;
					winrect.bottom = scrnpos->z+m_DragStartPos.y;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_LEFT:
					winrect.left   = scrnpos->x+m_DragStartPos.x;
					winrect.top    = m_WindowPosition.y;
					winrect.right  = m_WindowPosition.x+m_WindowSize.cx;
					winrect.bottom = m_WindowPosition.y+m_WindowSize.cy;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_RIGHT:
					winrect.left   = m_WindowPosition.x;
					winrect.top    = m_WindowPosition.y;
					winrect.right  = scrnpos->x+m_DragStartPos.x;
					winrect.bottom = m_WindowPosition.y+m_WindowSize.cy;
					ResizeWindow( winrect );
					break;
				default:
					break;
				}
			}
			else if( m_Dragging )
			{				
				int newxpos = scrnpos->x + m_DragStartPos.x;
				int newypos = scrnpos->z + m_DragStartPos.y;
				
				if( newxpos < 0	) newxpos = 0;
				if( newypos < 0	) newypos = 0;				
				if( newxpos+m_WindowSize.cx >= gnWidth ) newxpos = gnWidth - m_WindowSize.cx;
				if( newypos+m_WindowSize.cy >= gnHeight) newypos = gnHeight- m_WindowSize.cy;
				
				switch( m_WindowMoveMode )
				{
				case _XWMOVE_FREE :
					WindowPosChanging( newxpos, newypos );
					MoveWindow( newxpos, newypos );
					break;
				case _XWMOVE_HORIZ :
					WindowPosChanging( newxpos, (int&)m_WindowPosition.y );
					MoveWindow( newxpos, this->m_WindowPosition.y );
					break;
				case _XWMOVE_VERT :								
					WindowPosChanging( (int&)this->m_WindowPosition.x, newypos );
					MoveWindow( this->m_WindowPosition.x, newypos );
					break;
				}		
				
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_PICKUP_CLICK );
			}
		}
	}
	
	return FALSE;	
}

BOOL _XWindow_Debug::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if( m_DebugInfoListScrollBar )
	{
		if( m_DebugInfoListScrollBar->ProcessWheelMessage( zDelta ) )
		{
			return TRUE;
		}

		if( CheckMousePosition() )
		{
			int MoveDelta  = (FLOAT)zDelta / 40;			
			
			if( zDelta > 0 )
			{
				m_DebugInfoListScrollBar->LineUp( abs(MoveDelta) );
			}
			else
			{
				m_DebugInfoListScrollBar->LineDown( abs(MoveDelta) );
			}
			return TRUE;				
		}
	}

	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}

	return FALSE;	
}

void _XWindow_Debug::SetDebugInfoMode( int mode )
{
	m_DebugInfoMode = mode;
	
	for( int i = 0; i < 4; i++ )
	{			
		m_DebugInfoModeButton[i].SetCheck( i == m_DebugInfoMode );
	}	
		
	m_DebugInfoListScrollBar->SetTotalLineCount( g_DebugOutputlist[m_DebugInfoMode].getitemcount() );
	m_DebugInfoListScrollBar->UpdateData();	
	m_DebugInfoListScrollBar->SetLineScrollPos( 999999 ); 
}

#endif