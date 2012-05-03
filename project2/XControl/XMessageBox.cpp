// XMessageBox.cpp: implementation of the _XMessageBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XStringItem.h"
#include "EODEXTDEF_GlobalObject.h"
#include "SoundEffectList.h"

#include "XMessageBox.h"

#include "XProc_MainGameDef.h"

#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

#define _XDEF_MESSAGECENTERPOS				163

extern void _XPlayInterfaceSound( int index, bool checkplay );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XIMECallback_MessageBoxEditReturn( _XIMEContainer* pIMEContainer )
{
	_XIMEKERNEL.SetFocus( NULL );

	g_MessageBox.ShowWindow( FALSE );
	g_MessageBox.m_FocusedObject = NULL;

	if( g_MessageBox.m_OkCmdID )
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(g_MessageBox.m_OkCmdID, 0), (LPARAM)gHWnd);
}

void __stdcall _XIMECallback_MessageBoxEditEscape( _XIMEContainer* pIMEContainer )
{	
	_XIMEKERNEL.SetFocus( NULL );
	g_MessageBox.m_IMEControl.ClearBuffer();

	g_MessageBox.ShowWindow( FALSE );	
	g_MessageBox.m_FocusedObject = NULL;

	if( g_MessageBox.m_CancelCmdID )
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(g_MessageBox.m_CancelCmdID, 0), (LPARAM)gHWnd);
}


_XMessageBox::_XMessageBox()
{
	m_OkCmdID				= 0;
	m_CancelCmdID			= 0;
	m_CloseCmdID			= 0;
	m_ButtonMode			= 3;
	m_DragStartPos.x		= 0; 
	m_DragStartPos.y		= 0;
	m_TimerMode				= FALSE;
	m_SendCancelMessage		= FALSE;
	m_Timer					= 0;
	m_ViewTime				= 0;
	m_FocusedObject			= NULL;		
	m_ShowWindow			= FALSE;
	m_Dragging				= FALSE;
	m_ButtonMode			= _XDEF_DLGBTNTYPE_OK;
	m_WindowSize.cx			= _XDEF_MESSAGEDIALOGDLGPOS_WIDTH;
	m_WindowSize.cy			= _XDEF_MESSAGEDIALOGDLGPOS_HEIGHT;
	m_MessageColor			= _XSC_DEFAULT;	
		
	m_WindowPosition.x		= (gnWidth >> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH >>1);
	m_WindowPosition.y		= (gnHeight>> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_HEIGHT>>1);	
	
	m_TitlePos.x			= m_WindowPosition.x + _XDEF_MESSAGECENTERPOS;
	m_TitlePos.y			= m_WindowPosition.y + 14;
	
	m_MessagePos.x			= m_WindowPosition.x + (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH >> 1);
	m_MessagePos.y			= m_WindowPosition.y + 13;

	m_TextSplitWidth		= 288;

	memset( m_Message, 0, _XDEF_MAXMESSAGEBUFFERSIZE ); 
	memset( m_TitleString, 0, _XDEF_MAXTITLEBUFFERSIZE );
}

_XMessageBox::~_XMessageBox()
{
	DestroyWindow();
}

void _XMessageBox::MoveWindow( int X, int Y )
{
	POINT	movepoint = { m_WindowPosition.x - X, m_WindowPosition.y - Y };

	m_WindowPosition.x = X;
	m_WindowPosition.y = Y;

	m_LeftBorderStatic.MoveWindow( m_WindowPosition.x, m_WindowPosition.y );
	m_RightBorderStatic.MoveWindow( m_WindowPosition.x+_XDEF_MESSAGEDIALOGDLGPOS_WIDTH, m_WindowPosition.y );

	POINT wpos = m_OkBtn.GetWindowPos();
	m_OkBtn.MoveWindow( wpos.x - movepoint.x, wpos.y - movepoint.y );

	wpos = m_CancelBtn.GetWindowPos();
	m_CancelBtn.MoveWindow( wpos.x - movepoint.x, wpos.y - movepoint.y );

	wpos = m_CloseBtn.GetWindowPos();
	m_CloseBtn.MoveWindow( wpos.x - movepoint.x, wpos.y - movepoint.y );

	m_MessagePos.x = m_WindowPosition.x + (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH >> 1);
	m_MessagePos.y = m_MessagePos.y - movepoint.y;
	
	wpos = m_IMEControl.GetWindowPosition();
	m_IMEControl.MoveWindow( wpos.x - movepoint.x, wpos.y - movepoint.y );
	//m_IMEControl.MoveWindow(m_WindowPosition.x + (m_WindowSize.cx>>1) - 48, m_WindowPosition.y + 45);
}

void _XMessageBox::Create( void )
{	
	
	m_WindowPosition.x	=	(gnWidth >> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH >>1);
	m_WindowPosition.y	=	(gnHeight>> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_HEIGHT>>1);
		
	m_ButtonMode = _XDEF_DLGBTNTYPE_OK;
	
	m_WindowSize.cx = _XDEF_MESSAGEDIALOGDLGPOS_WIDTH;
	m_WindowSize.cy = _XDEF_MESSAGEDIALOGDLGPOS_HEIGHT;
	
	int imageindex = g_MainInterfaceTextureArchive.FindResource( "MI_party.tga" );

	m_LeftBorderStatic.Create( m_WindowPosition.x, m_WindowPosition.y,
							   m_WindowPosition.x+_XDEF_MESSAGECENTERPOS, m_WindowPosition.y+68,
						       &g_MainInterfaceTextureArchive, 
						       imageindex );

	m_LeftBorderStatic.SetClipRect( 0, 188, 163, 256 );

	m_RightBorderStatic.Create( m_WindowPosition.x+_XDEF_MESSAGEDIALOGDLGPOS_WIDTH, m_WindowPosition.y,
							    m_WindowPosition.x+_XDEF_MESSAGEDIALOGDLGPOS_WIDTH+_XDEF_MESSAGECENTERPOS,  m_WindowPosition.y+68,
							    &g_MainInterfaceTextureArchive, 
							    imageindex );
	
	m_RightBorderStatic.SetClipRect( 0, 188, 163, 256 );
	m_RightBorderStatic.SetScale( -1, 1 );
		
	int buttonimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	
	_XBTN_STRUCTURE subbutton_attr[3] = 
	{
		{ TRUE, 
			{m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 93,m_WindowPosition.y+41},
			{ 60, 23 }, _XDEF_MESSAGEDLGBUTTON_OK,
			  buttonimageindex,buttonimageindex,buttonimageindex,			  
			  &g_MainInterfaceTextureArchive },
		{ TRUE, 
			{m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 30,m_WindowPosition.y+41}, 
			{ 60, 23 }, _XDEF_MESSAGEDLGBUTTON_CANCEL, 
			  buttonimageindex,buttonimageindex,buttonimageindex,
			  &g_MainInterfaceTextureArchive },
		{ TRUE, 
			{m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1)+ 33,m_WindowPosition.y+41},
			{ 60, 23 }, _XDEF_MESSAGEDLGBUTTON_CLOSE,
			  buttonimageindex,buttonimageindex,buttonimageindex,
			  &g_MainInterfaceTextureArchive }
	};

	if( m_ButtonMode == _XDEF_DLGBTNTYPE_OK )
	{
		subbutton_attr[0].position.x = m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 30;
	}
	else if( m_ButtonMode == _XDEF_DLGBTNTYPE_YESNO )
	{
		subbutton_attr[0].position.x = m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 65;
		subbutton_attr[1].position.x = m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) + 5;
	}

	m_OkBtn.Create(	subbutton_attr[0]  );
	m_OkBtn.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 188, 128, 246, 151 );
	m_OkBtn.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  188, 153, 246, 177 );
	m_OkBtn.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  188, 178, 246, 202 );

	m_CancelBtn.Create( subbutton_attr[1]  );
	m_CancelBtn.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 188, 128, 246, 151 );
	m_CancelBtn.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  188, 153, 246, 177 );
	m_CancelBtn.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  188, 178, 246, 202 );

	m_CloseBtn.Create(  subbutton_attr[2]  );
	m_CloseBtn.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 188, 128, 246, 151 );
	m_CloseBtn.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  188, 153, 246, 177 );
	m_CloseBtn.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  188, 178, 246, 202 );

	m_OkBtn.ShowWindow( TRUE );
	m_CancelBtn.ShowWindow( TRUE );
	m_CloseBtn.ShowWindow( TRUE );		

	if( m_ButtonMode == _XDEF_DLGBTNTYPE_OK )
	{
		m_OkBtn.ShowWindow( TRUE );
		m_CancelBtn.ShowWindow( FALSE );
		m_CloseBtn.ShowWindow( FALSE );
	}
	else if( m_ButtonMode == _XDEF_DLGBTNTYPE_YESNO )
	{
		m_OkBtn.ShowWindow( TRUE );
		m_CancelBtn.ShowWindow( TRUE );
		m_CloseBtn.ShowWindow( FALSE );
	}	
	
	m_IMEControl.ResetContainer();
	m_IMEControl.ResizeWindow(80, 13);
	m_IMEControl.MoveWindow(m_WindowPosition.x + (m_WindowSize.cx>>1) - 48, m_WindowPosition.y + 42);	
	m_IMEControl.ShowWindow(FALSE);
	m_IMEControl.ShowWindowRectAngle( TRUE );
	m_IMEControl.SetMaxLength(12);
	m_IMEControl.SetReturnKeyCallback(_XIMECallback_MessageBoxEditReturn);
	m_IMEControl.SetEscapeKeyCallback(_XIMECallback_MessageBoxEditEscape);
	m_IMEControl.SetAlignType( _XFONT_ALIGNTYPE_CENTER );
	
	m_FocusedObject = NULL;	
}

void _XMessageBox::DestroyWindow( void )
{
	m_LeftBorderStatic.DestroyWindow();
	m_RightBorderStatic.DestroyWindow();
	
	m_OkBtn.DestroyWindow();
	m_CancelBtn.DestroyWindow();
	m_CloseBtn.DestroyWindow();

	m_MessageList.disposeList();

	m_OkCmdID		= 0;
	m_CancelCmdID	= 0;
	m_CloseCmdID	= 0;
	
	m_FocusedObject = NULL;
}

void _XMessageBox::Draw( void )
{
	if( !m_ShowWindow ) return;
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );	
		
	//_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-12, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

	m_LeftBorderStatic.DrawWithRegion();
	m_RightBorderStatic.DrawWithRegion();

	g_LastUsedRenderStyle = NULL;
	
	m_OkBtn.Draw(m_FocusedObject);
	m_CancelBtn.Draw(m_FocusedObject);
	m_CloseBtn.Draw(m_FocusedObject);

	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
	if( !m_MessageList.listEmpty() )
	{
		m_MessageList.resetList();

		_XStringItem* cip = NULL;
		int linecount = 0;

		do
		{			
			cip = (_XStringItem*)m_MessageList.currentItem();

			if( cip )
			{	
				g_XBaseFont->SetColor( m_MessageColor );
				//g_XBaseFont->Puts_SeparateAlign( m_MessagePos.x, m_MessagePos.y, cip->Get_String(), 288, _XFONT_ALIGNTYPE_CENTER, 1.0f, 2 );

				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
					g_XBaseFont->PutsAlign( m_MessagePos.x, m_MessagePos.y + (12)*linecount, _XFONT_ALIGNTYPE_CENTER, cip->Get_String() );
				else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
					g_XBaseFont->PutsAlign( m_MessagePos.x, m_MessagePos.y + (10)*linecount, _XFONT_ALIGNTYPE_CENTER, cip->Get_String() );
				else
					g_XBaseFont->PutsAlign( m_MessagePos.x, m_MessagePos.y + (g_XBaseFont->GetHeight()+2)*linecount, _XFONT_ALIGNTYPE_CENTER, cip->Get_String() );

			}

			linecount++;
			m_MessageList.nextItem();
		}while( !m_MessageList.atHeadOfList() );		
	}	

	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	if( m_TimerMode )
	{
		g_XSmallFont->SetColor( D3DCOLOR_ARGB(160,255,255,255) );
		g_XSmallFont->PrintAlign( m_WindowPosition.x + m_WindowSize.cx - 6, m_WindowPosition.y + m_WindowSize.cy - g_XSmallFont->GetHeight() - 4,
							     1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", (m_ViewTime - (g_LocalSystemTime - m_Timer)) / 1000 );
		g_XSmallFont->Flush();
	}

	if( m_ButtonMode == _XDEF_DLGBTNTYPE_INPUTSTRING )
	{		
		m_IMEControl.Draw();
	}

	if(m_ButtonMode == _XDEF_DLGBTNTYPE_SOCIALSENUMBER)
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x + (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 66, m_WindowPosition.y + 43, _XFONT_ALIGNTYPE_RIGHT, _T("***"));
		m_IMEControl.Draw();
	}
}

BOOL _XMessageBox::CheckMousePosition( void )
{
	if( !this->m_ShowWindow ) return FALSE;

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if( scrnpos )
	{	
		if( ((scrnpos->x > m_WindowPosition.x) && (scrnpos->x < m_WindowPosition.x+m_WindowSize.cx) &&
			 (scrnpos->z > m_WindowPosition.y) && (scrnpos->z < m_WindowPosition.y+m_WindowSize.cy) ) ) return TRUE;
	}

	return FALSE;
}

int  _XMessageBox::Process( void )
{
	if( !m_ShowWindow ) return 0;

	int retvalue = 0;

	g_CurrentFocusedObject	= _XDEF_DUMMYID_MESSAGEBOX;
	
	if( m_OkBtn.Process(m_FocusedObject) )
	{
		if( m_OkCmdID )
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_OkCmdID, 0), (LPARAM)gHWnd);
		
		ShowWindow( FALSE );	
		retvalue = IDOK;		
		m_FocusedObject = NULL;
	}

	if( m_CancelBtn.Process(m_FocusedObject)  )
	{
		if( m_CancelCmdID )
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CancelCmdID, 0), (LPARAM)gHWnd);
		
		ShowWindow( FALSE );	
		retvalue = IDCANCEL;		
		m_FocusedObject = NULL;
	}

	if( m_CloseBtn.Process(m_FocusedObject) )
	{
		if( m_CloseCmdID )
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CloseCmdID, 0), (LPARAM)gHWnd);

		ShowWindow( FALSE );	
		retvalue = IDCLOSE;		
		m_FocusedObject = NULL;
	}		

	MouseState* mousestate = gpInput->GetMouseState();
	// Process window dragging...
	if( mousestate )
	{
		if( !m_FocusedObject )
		{
			if( CheckMousePosition() && mousestate->bButton[0] )
			{
				m_FocusedObject = (_XGUIObject*)this;

				if( !m_Dragging )
				{
					m_Dragging = TRUE;
					ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
					m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
					m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
				}
				if( m_IMEControl.CheckMousePosition() )
				{
					m_IMEControl.SetFocus();
				}
			}
		}

		if( !mousestate->bButton[0] )
		{			
			if( m_FocusedObject == (_XGUIObject*)this )
			{				
				m_Dragging = FALSE;	
				m_DragStartPos.x = m_DragStartPos.y = 0;
				m_FocusedObject = NULL;				
			}			
		}

		else if( m_FocusedObject == (_XGUIObject*)this )
		{
			if( m_Dragging )
			{
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
				int newxpos = scrnpos->x + m_DragStartPos.x;
				int newypos = scrnpos->z + m_DragStartPos.y;

				if( newxpos < 0	) newxpos = 0;
				if( newypos < 0	) newypos = 0;				
				if( newxpos+m_WindowSize.cx >= gnWidth ) newxpos = gnWidth - m_WindowSize.cx;
				if( newypos+m_WindowSize.cy >= gnHeight) newypos = gnHeight- m_WindowSize.cy;

				MoveWindow( newxpos, newypos );			
			}
		}
	}

	if( m_TimerMode )
	{
		if( m_Timer == 0 )	m_Timer = g_LocalSystemTime;
			
		if( g_LocalSystemTime - m_Timer > m_ViewTime )
		{
			ShowWindow( FALSE );
			m_TimerMode		= FALSE;
			m_Timer			= 0;
			m_ViewTime		= 0;

			if( m_SendCancelMessage )
			{
				switch( m_ButtonMode ) 
				{
				case _XDEF_DLGBTNTYPE_NOTICE :
					break;
				case _XDEF_DLGBTNTYPE_OK	 :
					if( m_OkCmdID )
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_OkCmdID, 0), (LPARAM)gHWnd);
					break;
				case _XDEF_DLGBTNTYPE_YESNO  :
					if( m_CancelCmdID )
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CancelCmdID, 0), (LPARAM)gHWnd);
					break;
				case _XDEF_DLGBTNTYPE_YESNOCANCEL :
					if( m_CloseCmdID )
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CloseCmdID, 0), (LPARAM)gHWnd);
					break;
				case _XDEF_DLGBTNTYPE_CANCEL :
					if( m_OkCmdID )
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_OkCmdID, 0), (LPARAM)gHWnd);
					break;
				case _XDEF_DLGBTNTYPE_SOCIALSENUMBER :
				case _XDEF_DLGBTNTYPE_INPUTSTRING :
					break;
				}
			}
		}
	}	
	
	return retvalue;
}

void _XMessageBox::ShowWindow( BOOL show )
{ 
	m_ShowWindow = show; 
	g_CurrentFocusedObject = NULL;
	
	if( _XIMEKERNEL.GetIMEContainer() == &m_IMEControl )
	{
		_XIMEKERNEL.SetFocus(NULL);
	}
}

void _XMessageBox::StartTimer( int counttime, BOOL sendcancelmessage )
{
	m_Timer			=	g_LocalSystemTime;
	m_ViewTime		=	counttime;
	m_TimerMode		=	TRUE;
	m_SendCancelMessage = sendcancelmessage;
}

void _XMessageBox::SetMessage( TCHAR* argv, ... )
{
	int retValue;
	va_list argptr;

	memset( m_Message, 0, _XDEF_MAXMESSAGEBUFFERSIZE );
	m_MessageList.disposeList();

	if( !argv ) return;
		
	va_start( argptr, argv );
	retValue = vsprintf( m_Message, argv, argptr );
	va_end( argptr );
		
	if( strlen( m_Message ) )
		m_MessageList.insertItem( new _XStringItem(m_Message,0) );
	
	::SetFocus( gHWnd );

	m_TimerMode		=	FALSE;
}

void _XMessageBox::SetMessage( int resourceid )
{
	TCHAR messagestring[256];
	LoadString( gHInst, resourceid, messagestring, 255 );
	SetMessage( messagestring );
}

void _XMessageBox::SetMessage( int btntype, int resourceid, BOOL showwindow, 
							   DWORD okcmdid, DWORD cancelcmdid, DWORD closecmdid )
{
	TCHAR messagestring[256];
	strncpy(messagestring, _XGETINTERFACETEXT(resourceid), 255);

	SetMessage( btntype, _T(""), messagestring, showwindow, okcmdid, cancelcmdid, closecmdid );
}

void _XMessageBox::SetMessage( int btntype, LPSTR title, LPSTR message, BOOL showwindow, 
							   DWORD okcmdid, DWORD cancelcmdid, DWORD closecmdid )
{		
	m_OkCmdID			= okcmdid; 
	m_CancelCmdID		= cancelcmdid;
	m_CloseCmdID		= closecmdid;
	m_ShowWindow		= showwindow;

	m_TimerMode			= FALSE;

	memset( m_Message, 0, _XDEF_MAXMESSAGEBUFFERSIZE );
	if( message )
	{
		if( strlen(message) )
			strcpy( m_Message, message );
	}
	
	//_XLog( "Message Title : %s", message );

	_XStringSplitInfo splitinfo;
	g_XBaseFont->SplitString( m_Message, m_TextSplitWidth, splitinfo );

	if( splitinfo.splitcount > 1 )
	{
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
			m_MessagePos.y = m_WindowPosition.y + 19 - ( splitinfo.splitcount*(12) / 2 );
		else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			m_MessagePos.y = m_WindowPosition.y + 19 - ( splitinfo.splitcount*(10) / 2 );
		else
			m_MessagePos.y = m_WindowPosition.y + 19 - ( splitinfo.splitcount*(g_XBaseFont->GetHeight()+2) / 2 );
	}
	else
	{
		m_MessagePos.y = m_WindowPosition.y + 19 - (g_XBaseFont->GetHeight()>>1);
	}

	memset( m_TitleString, 0, _XDEF_MAXTITLEBUFFERSIZE );
	if(title)
	{
		if( strlen(title) )
			strcpy( m_TitleString, title );
	}
		
	m_MessageList.disposeList();
	
	TCHAR tempbuffer[512];
	for( int i = 0; i < splitinfo.splitcount; i++ )
	{
		memset( tempbuffer, 0, sizeof(TCHAR) * 512 );
		strncpy( tempbuffer, splitinfo.splitpoint[i], splitinfo.splitlength[i] );
		m_MessageList.insertItem( new _XStringItem(tempbuffer,0) );
	}

	m_ButtonMode = btntype;

	//if( m_ButtonMode == btntype )
	{
		if( m_ButtonMode == _XDEF_DLGBTNTYPE_OK )
		{
			m_OkBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_OK );
			m_OkBtn.SetButtonTextColor( _XSC_DEFAULT );
			m_OkBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 30, m_WindowPosition.y+41 );
			m_OkBtn.ShowWindow( TRUE );
			m_CancelBtn.ShowWindow( FALSE );
			m_CloseBtn.ShowWindow( FALSE );
			_XIMEKERNEL.SetFocus( NULL );
		}
		else if( m_ButtonMode == _XDEF_DLGBTNTYPE_YESNO )
		{
			m_OkBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_YES );
			m_OkBtn.SetButtonTextColor( _XSC_DEFAULT );
			m_CancelBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_NO );
			m_CancelBtn.SetButtonTextColor( _XSC_WARNING );
			
			m_OkBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 62, m_WindowPosition.y+41 );
			m_CancelBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) + 2, m_WindowPosition.y+41 );
			m_OkBtn.ShowWindow( TRUE );
			m_CancelBtn.ShowWindow( TRUE );
			m_CloseBtn.ShowWindow( FALSE );
			_XIMEKERNEL.SetFocus( NULL );
		}	
		else if( m_ButtonMode == _XDEF_DLGBTNTYPE_YESNOCANCEL )
		{
			m_OkBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_YES );			
			m_OkBtn.SetButtonTextColor( _XSC_DEFAULT );			
			m_CancelBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_NO );
			m_CancelBtn.SetButtonTextColor( _XSC_WARNING );			
			m_CloseBtn.SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_CANCEL);
			m_CloseBtn.SetButtonTextColor( _XSC_WARNING_CLICK );

			m_OkBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 93, m_WindowPosition.y+41 );
			m_CancelBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 30, m_WindowPosition.y+41 );
			m_CloseBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1)+ 33, m_WindowPosition.y+41 );
			m_OkBtn.ShowWindow( TRUE );
			m_CancelBtn.ShowWindow( TRUE );
			m_CloseBtn.ShowWindow( TRUE );
			_XIMEKERNEL.SetFocus( NULL );
		}
		else if( m_ButtonMode == _XDEF_DLGBTNTYPE_CANCEL )
		{
			m_OkBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_CANCEL);			
			m_OkBtn.SetButtonTextColor( _XSC_WARNING );
			m_OkBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 30, m_WindowPosition.y+41 );
			m_OkBtn.ShowWindow( TRUE );
			m_CancelBtn.ShowWindow( FALSE );
			m_CloseBtn.ShowWindow( FALSE );
			_XIMEKERNEL.SetFocus( NULL );
		}
		else if( m_ButtonMode == _XDEF_DLGBTNTYPE_INPUTSTRING )
		{				
			m_OkBtn.ShowWindow( FALSE );
			m_CancelBtn.ShowWindow( FALSE );
			m_CloseBtn.ShowWindow( FALSE );
			int count = 0;
			if( okcmdid != 0 )
			{
				m_OkBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_OK );
				m_OkBtn.SetButtonTextColor( _XSC_DEFAULT );
				m_OkBtn.ShowWindow( TRUE );
				++count;
			}
			if( cancelcmdid != 0 )
			{
				m_CancelBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_CANCEL );
				m_CancelBtn.SetButtonTextColor( _XSC_WARNING );
				m_CancelBtn.ShowWindow( TRUE );
				++count;
			}
			if( closecmdid != 0 )
			{
				m_CloseBtn.SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_CANCEL);
				m_CloseBtn.SetButtonTextColor( _XSC_WARNING_CLICK );
				m_CloseBtn.ShowWindow( TRUE );
				++count;
			}
			if(count == 1 )
			{
				m_OkBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) + 80, m_WindowPosition.y+41 );
				m_IMEControl.MoveWindow(m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 100, m_WindowPosition.y+43);
			}
			else if(count == 2)
			{
				m_OkBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) +2, m_WindowPosition.y+41 );
				m_CancelBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) + 62, m_WindowPosition.y+41 );
				m_IMEControl.MoveWindow(m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 130, m_WindowPosition.y+45);
			}
			else if(count == 3)
			{
				m_OkBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) -18, m_WindowPosition.y+41 );
				m_CancelBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) +42, m_WindowPosition.y+41 );
				m_CloseBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1)+ 102, m_WindowPosition.y+41 );
				m_IMEControl.MoveWindow(m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 130, m_WindowPosition.y+45);
			}
			m_IMEControl.SetPasswordMode( m_IMEPasswordMode );
			m_IMEControl.ClearBuffer();
			m_IMEControl.SetFocus();
		}
		else if(m_ButtonMode == _XDEF_DLGBTNTYPE_SOCIALSENUMBER)
		{
			m_OkBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_OK );
			m_OkBtn.SetButtonTextColor( _XSC_DEFAULT );
			m_CancelBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_CANCEL );
			m_CancelBtn.SetButtonTextColor( _XSC_WARNING );
			
			m_OkBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) + 36, m_WindowPosition.y+40 );
			m_CancelBtn.MoveWindow( m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) + 95, m_WindowPosition.y+40 );
			m_OkBtn.ShowWindow( TRUE );
			m_CancelBtn.ShowWindow( TRUE );
			m_CloseBtn.ShowWindow( FALSE );

			m_IMEControl.MoveWindow(m_WindowPosition.x+(_XDEF_MESSAGEDIALOGDLGPOS_WIDTH>>1) - 62, m_WindowPosition.y+43);
			m_IMEControl.SetPasswordMode( m_IMEPasswordMode );
			m_IMEControl.ClearBuffer();
			m_IMEControl.SetFocus();
			m_IMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
		}
		else 
		{
			m_OkBtn.ShowWindow( FALSE );
			m_CancelBtn.ShowWindow( FALSE );
			m_CloseBtn.ShowWindow( FALSE );			
			_XIMEKERNEL.SetFocus( NULL );
		}
	}

	m_ButtonMode = btntype;

	if( m_ShowWindow )
		g_CurrentFocusedObject	= _XDEF_DUMMYID_MESSAGEBOX;

	//m_FocusedObject = NULL;

	m_MessageColor = _XSC_DEFAULT;	
}

bool _XMessageBox::OnKeyDown(WPARAM wparam, LPARAM lparam)
{
	if( !m_ShowWindow ) return false;

	if( m_ButtonMode != _XDEF_DLGBTNTYPE_NOTICE && m_ButtonMode != _XDEF_DLGBTNTYPE_INPUTSTRING )
	{
		switch( (U32)wparam )
		{
		//case VK_SPACE  :
		case VK_RETURN :
		case VK_Y :
			{
				if( m_OkCmdID )
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_OkCmdID, 0), (LPARAM)gHWnd);

				m_OkCmdID = m_CancelCmdID = m_CloseCmdID = 0;
				
				ShowWindow( FALSE );
				m_FocusedObject = NULL;

				_XPlayInterfaceSound( ID_SR_INTERFACE_SMALLBUTTONCLICK_WAV );
				return true;
			}
			break;
		case VK_ESCAPE :
		case VK_N :
			{
				if( m_CancelCmdID )
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CancelCmdID, 0), (LPARAM)gHWnd);
				
				m_OkCmdID = m_CancelCmdID = m_CloseCmdID = 0;

				ShowWindow( FALSE );
				m_FocusedObject = NULL;

				_XPlayInterfaceSound( ID_SR_INTERFACE_SMALLBUTTONCLICK_WAV );
				return true;
			}
			break;
		}
	}
	else return true;
	
	return false;
}