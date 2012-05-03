// XWindow_EventNotice.cpp: implementation of the _XWindow_EventNotice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindowObjectDefine.h"
#include "XWindow_EventNotice.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define _EVENTNOTICESIZE_WIDTH		219
#define _EVENTNOTICESIZE_HEIGHT		339

_XWindow_EventNotice::_XWindow_EventNotice()
{
	m_pClosebutton = NULL;
	m_pCheckbutton = NULL;
}

_XWindow_EventNotice::~_XWindow_EventNotice()
{

}

BOOL _XWindow_EventNotice::Initialize( void )
{
	// 닫기 버튼 -----------------------------------------------------------------------------=
	int btnimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Trade.tga" );
	
#ifdef _XRUSSIAN	
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { m_WindowPosition.x+m_WindowSize.cx-58, m_WindowPosition.y+m_WindowSize.cy-20 }, 
										{ 55, 17 }, _XDEF_EVENTNOTICE_CLOSE,
#else
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { m_WindowPosition.x+m_WindowSize.cx-50, m_WindowPosition.y+m_WindowSize.cy-20 }, 
										{ 41, 17 }, _XDEF_EVENTNOTICE_CLOSE,
#endif
										btnimageindex,btnimageindex,btnimageindex, 
										&g_MainInterfaceTextureArchive };
	m_pClosebutton = new _XButton;
	m_pClosebutton->Create( closebtnstruct );

	#ifdef _XRUSSIAN	
		m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 190, 79, 245, 93 );
		m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  190, 99, 245, 116 );
		m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  190, 119, 245, 136 );
	#else	
		m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 214, 220, 255, 237 );
		m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  214, 202, 255, 219 );
		m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  214, 238, 255, 255 );
	#endif

	m_pClosebutton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_CLOSE );
	InsertChildObject( m_pClosebutton );

	// 체크버튼 ------------------------------------------------------------------------------=
	int checkbtnimage = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	
	_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, { m_WindowPosition.x+m_WindowSize.cx-74, m_WindowPosition.y+m_WindowSize.cy-20 },
											{ 14, 14 }, 
											_XDEF_EVENTNOTICE_CHECK,
											checkbtnimage,checkbtnimage,
											&g_MainInterfaceTextureArchive };

	m_pCheckbutton = new _XCheckButton;
	m_pCheckbutton->Create( checkbtnstruct );	
	m_pCheckbutton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pCheckbutton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	m_pCheckbutton->SetCheck( FALSE );
	m_pCheckbutton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pCheckbutton );

	m_HTMLViewStatic.Create( m_WindowPosition.x+2, m_WindowPosition.y+2, m_WindowPosition.x+2+_EVENTNOTICESIZE_WIDTH, 
							 m_WindowPosition.y+2+_EVENTNOTICESIZE_HEIGHT, &g_MainInterfaceTextureArchive, 0 );

	m_HTMLViewStatic.SetClipRect( 0, 0, _EVENTNOTICESIZE_WIDTH-4, _EVENTNOTICESIZE_HEIGHT-24 );

	return TRUE;
}

void _XWindow_EventNotice::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return;
	
	_XDrawSolidBar(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, D3DCOLOR_ARGB(178,0,0,0) );
	g_pDefaultTooltip_Window->DrawTooltipLine(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1);

	_XWindow::Draw(pfocusobject);
	
	g_XBaseFont->SetColor( 0xFFE9DC96 );	
	
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )		
	{
		g_XBaseFont->PutsAlign( m_WindowPosition.x+m_WindowSize.cx-78, m_WindowPosition.y+m_WindowSize.cy-18 , _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2780) );	// _T("오늘 하루 안봄")
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont->PutsAlign( m_WindowPosition.x+m_WindowSize.cx-82, m_WindowPosition.y+m_WindowSize.cy-18 , _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2780) );	// _T("오늘 하루 안봄")
	}
	else
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+m_WindowSize.cx-120, m_WindowPosition.y+m_WindowSize.cy-18 , _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2780) );	// _T("오늘 하루 안봄")
	}
	g_XBaseFont->Flush();
	

#ifdef _XDEF_EVENTWINDOW_WEBPAGE_20070214	
#else
	g_EventHTMLWrapper.UpdateTextureBuffer( 0 );
	m_HTMLViewStatic.DrawWithRegion( g_EventHTMLWrapper.m_pHTMLTexture );
#endif
}

BOOL _XWindow_EventNotice::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow )		return FALSE;
	if( !this->m_Enable  )			return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	return TRUE;
}

void _XWindow_EventNotice::ShowWindow(BOOL show)
{
	if(show)
	{
		g_MainWindowManager.SetTopWindow(this);
#ifdef _XDEF_EVENTWINDOW_WEBPAGE_20070214
		POINT pt = { 0, 0 };
		ClientToScreen( gHWnd, &pt );
//		if( g_reg_windowedmode )
//		{			
//			::SetWindowPos(g_hWebEventWindow, HWND_TOPMOST, pt.x + m_WindowPosition.x+2+3, pt.y + m_WindowPosition.y+2+23, 215, 315, 0);	
//		}
//		else
		{
			::SetWindowPos(g_hWebEventWindow, HWND_TOPMOST, pt.x + m_WindowPosition.x+2, pt.y + m_WindowPosition.y+2, 215, 315, 0);	
		}
		//::SetWindowPos(g_hWebEventWindow, HWND_TOPMOST, m_WindowPosition.x+2, m_WindowPosition.y+2, 215, 315, 0);		//m_WindowPosition.x+61, m_WindowPosition.y+33
		::ShowWindow(g_hWebEventWindow, SW_SHOW);
#endif
	}
	else
	{
#ifdef _XDEF_EVENTWINDOW_WEBPAGE_20070214
		::ShowWindow(g_hWebEventWindow, SW_HIDE);
#endif
	}

	_XWindow::ShowWindow(show);	
}

void _XWindow_EventNotice::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );
	
#ifdef _XDEF_EVENTWINDOW_WEBPAGE_20070214	
	POINT pt = { 0, 0 };
	ClientToScreen( gHWnd, &pt );
//	if( g_reg_windowedmode )
//	{			
//		::SetWindowPos(g_hWebEventWindow, HWND_TOPMOST, pt.x + m_WindowPosition.x+2+3, pt.y + m_WindowPosition.y+2+23, 215, 315, 0);	
//	}
//	else
	{
		::SetWindowPos(g_hWebEventWindow, HWND_TOPMOST, pt.x + m_WindowPosition.x+2, pt.y + m_WindowPosition.y+2, 215, 315, 0);	
	}
#else
	m_HTMLViewStatic.MoveWindow( m_WindowPosition.x+2, m_WindowPosition.y+2 );
#endif
	
	#ifdef _XRUSSIAN	
		m_pClosebutton->MoveWindow( m_WindowPosition.x+m_WindowSize.cx-58, m_WindowPosition.y+m_WindowSize.cy-20 );
	#else
		m_pClosebutton->MoveWindow( m_WindowPosition.x+m_WindowSize.cx-50, m_WindowPosition.y+m_WindowSize.cy-20 );
	#endif
	m_pCheckbutton->MoveWindow( m_WindowPosition.x+m_WindowSize.cx-74, m_WindowPosition.y+m_WindowSize.cy-19 );
}

void _XWindow_EventNotice::SetHelpHtml()
{
	TCHAR urlstring[_XDEF_MAX_URLSTRINGLENGTH];
	strcpy( urlstring, gModulePath );
	strcat( urlstring, _T("\\HelpSystem\\") );

	//if( !g_bNewClanQuest )
		strcat( urlstring, "event.html" );
	//else
	//	strcat( urlstring, "eventfin.html" );

	g_EventHTMLWrapper.SetViewHeight( 512 );
	g_EventHTMLWrapper.LoadHTML( urlstring, _EVENTNOTICESIZE_WIDTH-4 );	
	
	g_EventHTMLWrapper.UpdateTextureBuffer( 0 );
}

void _XWindow_EventNotice::SetEventNotice(void)
{
	SIZE tempsize;	
	tempsize.cx = _EVENTNOTICESIZE_WIDTH;
	tempsize.cy = _EVENTNOTICESIZE_HEIGHT;
	SetWindowSize(tempsize);
	m_pCheckbutton->SetCheck( FALSE );
	MoveWindow(m_WindowPosition.x, m_WindowPosition.y);
	
	//SetHelpHtml(); //웹페이지 링크로 바뀜 
	ShowWindow(TRUE);
}