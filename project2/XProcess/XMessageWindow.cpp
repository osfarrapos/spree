// XMessageWindow.cpp: implementation of the _XMessageWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XStringItem.h"
#include "EODEXTDEF_GlobalObject.h"
#include "SoundEffectList.h"
#include "XMessageWindow.h"
#include "XProc_MainGameDef.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"

#define _XDEF_MESSAGECENTERPOS				163

extern void _XPlayInterfaceSound( int index, bool checkplay );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XMessageWindow::_XMessageWindow()
{
	m_OkCmdID				= 0;
	m_CancelCmdID			= 0;
	m_CloseCmdID			= 0;
	m_ButtonMode			= 3;	
	m_TimerMode				= FALSE;
	m_SendCancelMessage		= FALSE;
	m_Timer					= 0;
	m_ViewTime				= 0;
	m_ButtonMode			= _XDEF_DLGBTNTYPE_OK;
	m_MessageColor			= _XSC_DEFAULT;
	memset( m_Message, 0, _XDEF_MAXMESSAGEBUFFERSIZE ); 
	memset( m_TitleString, 0, _XDEF_MAXTITLEBUFFERSIZE );
}

_XMessageWindow::~_XMessageWindow()
{
	DestroyWindow();
}

void _XMessageWindow::MoveWindow( int X, int Y )
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
	
	m_IMEControl.MoveWindow(m_WindowPosition.x + (m_WindowSize.cx>>1) - 48, m_WindowPosition.y + 45);
}

_XMessageWindow* _XMessageWindow::CreateMessageBox( _XWindowManager* pWindowManager, DWORD windowid, BOOL bCash )
{
	_XMessageWindow* pNewMessageBox = new _XMessageWindow;
	pNewMessageBox->Create( pWindowManager, windowid, bCash );	
	return pNewMessageBox;
}

void _XMessageWindow::Create( _XWindowManager* pWindowManager, DWORD windowid, BOOL bCash )
{
	SetDrawBorder( FALSE );

	_XWINDOW_STRUCTURE windowstruct = 
	{
		TRUE, { (gnWidth >> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH >>1),
			    (gnHeight>> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_HEIGHT>>1) }, 
				{ _XDEF_MESSAGEDIALOGDLGPOS_WIDTH, _XDEF_MESSAGEDIALOGDLGPOS_HEIGHT }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 127, 0, 0, 0 ),
			windowid
	};

	_XWindow::Create( windowstruct );

	m_ButtonMode = _XDEF_DLGBTNTYPE_OK;

	int imageindex = -1;

	if(bCash)
		imageindex = g_MainInterfaceTextureArchive.FindResource("MI_message.tga");
	else
		imageindex = g_MainInterfaceTextureArchive.FindResource( "MI_party.tga" );

	m_LeftBorderStatic.Create( m_WindowPosition.x, m_WindowPosition.y,
		m_WindowPosition.x+_XDEF_MESSAGECENTERPOS, m_WindowPosition.y+68,
		&g_MainInterfaceTextureArchive, 
		imageindex );
	
	if(bCash)
		m_LeftBorderStatic.SetClipRect(3, 2, 166, 70);
	else
		m_LeftBorderStatic.SetClipRect( 0, 188, 163, 256 );
	
	m_RightBorderStatic.Create( m_WindowPosition.x+_XDEF_MESSAGEDIALOGDLGPOS_WIDTH, m_WindowPosition.y,
		m_WindowPosition.x+_XDEF_MESSAGEDIALOGDLGPOS_WIDTH+_XDEF_MESSAGECENTERPOS,  m_WindowPosition.y+68,
		&g_MainInterfaceTextureArchive, 
		imageindex );
	
	if(bCash)
		m_RightBorderStatic.SetClipRect(3, 2, 166, 70);
	else
		m_RightBorderStatic.SetClipRect( 0, 188, 163, 256 );

	m_RightBorderStatic.SetScale( -1, 1 );
		
	int buttonimageindex = -1;

	if(bCash)
		buttonimageindex = g_MainInterfaceTextureArchive.FindResource("MI_Message.tga");
	else
		buttonimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );

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
	if(bCash)
	{
		m_OkBtn.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 87, 61, 110 );
		m_OkBtn.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 111, 61, 134 );
		m_OkBtn.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 136, 61, 159 );
	}
	else
	{
		m_OkBtn.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 188, 128, 246, 151 );
		m_OkBtn.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  188, 153, 246, 177 );
		m_OkBtn.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  188, 178, 246, 202 );
	}

	m_CancelBtn.Create( subbutton_attr[1]  );
	if(bCash)
	{
		m_CancelBtn.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 87, 61, 110 );
		m_CancelBtn.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 111, 61, 134 );
		m_CancelBtn.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 136, 61, 159 );
	}
	else
	{
		m_CancelBtn.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 188, 128, 246, 151);
		m_CancelBtn.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  188, 153, 246, 177);
		m_CancelBtn.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  188, 178, 246, 202);
	}
	
	m_CloseBtn.Create(  subbutton_attr[2]  );
	if(bCash)
	{
		m_CloseBtn.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 87, 61, 110 );
		m_CloseBtn.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 111, 61, 134 );
		m_CloseBtn.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 136, 61, 159 );
	}
	else
	{
		m_CloseBtn.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 188, 128, 246, 151 );
		m_CloseBtn.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  188, 153, 246, 177 );
		m_CloseBtn.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  188, 178, 246, 202 );
	}

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
	//m_IMEControl.SetReturnKeyCallback(_XIMECallback_MessageBoxEditReturn);
	//m_IMEControl.SetEscapeKeyCallback(_XIMECallback_MessageBoxEditEscape);
	m_IMEControl.SetAlignType( _XFONT_ALIGNTYPE_CENTER );

	m_TitlePos.x			= m_WindowPosition.x + _XDEF_MESSAGECENTERPOS;
	m_TitlePos.y			= m_WindowPosition.y + 14;
	
	m_MessagePos.x			= m_WindowPosition.x + (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH >> 1);
	m_MessagePos.y			= m_WindowPosition.y + 13;

	if( pWindowManager )
	{
		pWindowManager->InsertWindow( this );		
	}

}

void _XMessageWindow::DestroyWindow( void )
{
	if( g_CurrentFocusedObject == &m_LeftBorderStatic ) g_CurrentFocusedObject = NULL;
	m_LeftBorderStatic.DestroyWindow();

	if( g_CurrentFocusedObject == &m_RightBorderStatic ) g_CurrentFocusedObject = NULL;
	m_RightBorderStatic.DestroyWindow();

	if( g_CurrentFocusedObject == &m_OkBtn ) g_CurrentFocusedObject = NULL;
	m_OkBtn.DestroyWindow();

	if( g_CurrentFocusedObject == &m_CancelBtn ) g_CurrentFocusedObject = NULL;
	m_CancelBtn.DestroyWindow();

	if( g_CurrentFocusedObject == &m_CloseBtn ) g_CurrentFocusedObject = NULL;
	m_CloseBtn.DestroyWindow();

	m_MessageList.disposeList();

	m_OkCmdID		= 0;
	m_CancelCmdID	= 0;
	m_CloseCmdID	= 0;
	
	_XWindow::DestroyWindow();
}

void _XMessageWindow::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw( pfocusobject );

	if( !m_ShowWindow ) return;
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );	
	
	m_LeftBorderStatic.DrawWithRegion();
	m_RightBorderStatic.DrawWithRegion();
	
	m_OkBtn.Draw(pfocusobject);
	m_CancelBtn.Draw(pfocusobject);
	m_CloseBtn.Draw(pfocusobject);

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
				g_XBaseFont->Puts_SeparateAlign( m_MessagePos.x, m_MessagePos.y, cip->Get_String(), 256, _XFONT_ALIGNTYPE_CENTER, 1.0f, 2 );
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
}

BOOL  _XMessageWindow::Process(  _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable) return FALSE;
	
	if(!this->m_ShowWindow) return FALSE;
	
	if( m_OkBtn.Process(pfocusobject) )
	{
		if( m_OkCmdID )
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_OkCmdID, 0), (LPARAM)gHWnd);
		
		m_SelfDestroy = TRUE;
		if( m_pMyManager ) m_pMyManager->EnableCheckSelfDestroyWindow();
	}

	if( m_CancelBtn.Process(pfocusobject)  )
	{
		if( m_CancelCmdID )
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CancelCmdID, 0), (LPARAM)gHWnd);		
		
		m_SelfDestroy = TRUE;
		if( m_pMyManager ) m_pMyManager->EnableCheckSelfDestroyWindow();
	}

	if( m_CloseBtn.Process(pfocusobject) )
	{
		if( m_CloseCmdID )
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CloseCmdID, 0), (LPARAM)gHWnd);
		
		m_SelfDestroy = TRUE;
		if( m_pMyManager ) m_pMyManager->EnableCheckSelfDestroyWindow();
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
				case _XDEF_DLGBTNTYPE_INPUTSTRING :
					break;
				}
			}

			m_SelfDestroy = TRUE;
			if( m_pMyManager ) m_pMyManager->EnableCheckSelfDestroyWindow();
		}
	}	
	
	if(!_XWindow::Process(pfocusobject)) return FALSE;
	
	return TRUE;
}

void _XMessageWindow::ShowWindow( BOOL show )
{ 
	_XWindow::ShowWindow( show );
	
	if( _XIMEKERNEL.GetIMEContainer() == &m_IMEControl )
	{
		_XIMEKERNEL.SetFocus(NULL);
	}
}

void _XMessageWindow::StartTimer( int counttime, BOOL sendcancelmessage )
{
	m_Timer			=	g_LocalSystemTime;
	m_ViewTime		=	counttime;
	m_TimerMode		=	TRUE;
	m_SendCancelMessage = sendcancelmessage;
}

void _XMessageWindow::SetMessage( TCHAR* argv, ... )
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

void _XMessageWindow::SetMessage( int resourceid )
{
	TCHAR messagestring[256];
	LoadString( gHInst, resourceid, messagestring, 255 );
	SetMessage( messagestring );
}

void _XMessageWindow::SetMessage( int btntype, int resourceid, BOOL showwindow, 
							   DWORD okcmdid, DWORD cancelcmdid, DWORD closecmdid )
{
	TCHAR messagestring[256];
	strncpy(messagestring, _XGETINTERFACETEXT(resourceid), 255);

	SetMessage( btntype, _T(""), messagestring, showwindow, okcmdid, cancelcmdid, closecmdid );
}

void _XMessageWindow::SetMessage( int btntype, LPSTR title, LPSTR message, BOOL showwindow, 
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

	_XStringSplitInfo splitinfo;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont->SplitString( m_Message, 288, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR );
	}
	else
	{
		g_XBaseFont->SplitString( m_Message, 288, splitinfo );
	}
	
	if( splitinfo.splitcount > 1 )
	{
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
	m_MessageList.insertItem( new _XStringItem(message,0) );

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
			m_OkBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_YES);
			m_OkBtn.SetButtonTextColor( _XSC_DEFAULT );
			m_CancelBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_NO);
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
			m_OkBtn.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_CANCEL );			
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

			m_IMEControl.SetPasswordMode( m_IMEPasswordMode );
			m_IMEControl.ClearBuffer();
			m_IMEControl.SetFocus();
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

	m_MessageColor = _XSC_DEFAULT;	
}

BOOL _XMessageWindow::OnKeyDown(WPARAM wparam, LPARAM lparam)
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
				_XPlayInterfaceSound( ID_SR_INTERFACE_SMALLBUTTONCLICK_WAV );			

				m_SelfDestroy = TRUE;
				if( m_pMyManager ) m_pMyManager->EnableCheckSelfDestroyWindow();
				return TRUE;
			}
			break;
		case VK_ESCAPE :
		case VK_N :
			{
				if( m_CancelCmdID ) 
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CancelCmdID, 0), (LPARAM)gHWnd);				

				m_OkCmdID = m_CancelCmdID = m_CloseCmdID = 0;

				ShowWindow( FALSE );
				_XPlayInterfaceSound( ID_SR_INTERFACE_SMALLBUTTONCLICK_WAV );

				m_SelfDestroy = TRUE;
				if( m_pMyManager ) m_pMyManager->EnableCheckSelfDestroyWindow();
				return TRUE;
			}
			break;
		}
	}
		
	return FALSE;
}
