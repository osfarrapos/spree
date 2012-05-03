// _XWindow_ChatBalloon : implementation of the _XToolTip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindow_ChatBalloon.h"


#define _XDEF_CHATBALLOONWINDOWWIDTH	180
#define _XDEF_CHATBALLOONVIEWTIME		7000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_ChatBalloon::_XWindow_ChatBalloon()
{
	m_Counter		= 0;
	m_Show			= FALSE;	
	m_MaxWidth		= 0;
	memset( m_SplitMessage, 0, sizeof(LPTSTR) * _XDEF_CHATBALLOONSEPARATECOUNT );
}

_XWindow_ChatBalloon::~_XWindow_ChatBalloon()
{
	for( int i = 0; i < _XDEF_CHATBALLOONSEPARATECOUNT; i++ )
	{
		SAFE_DELETE_ARRAY( m_SplitMessage[i] );
	}
}

void _XWindow_ChatBalloon::SetMessage( LPTSTR message )
{	
	m_Counter = g_LocalSystemTime;

	if( !message ) return;

	m_Show = TRUE;
	m_MaxWidth = 0;

	_XStringSplitInfo splitinfo;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont->SplitString( message, _XDEF_CHATBALLOONWINDOWWIDTH, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR );
	}
	else
	{
		g_XBaseFont->SplitString( message, _XDEF_CHATBALLOONWINDOWWIDTH, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR );
	}
	memset( m_SplitMessage, 0, sizeof(LPTSTR) * _XDEF_CHATBALLOONSEPARATECOUNT );

	m_SplitCount = splitinfo.splitcount;
	// exception process...
	if( m_SplitCount > _XDEF_CHATBALLOONSEPARATECOUNT ) m_SplitCount = _XDEF_CHATBALLOONSEPARATECOUNT;


	for( int i = 0; i < m_SplitCount; i++ )
	{
		if( splitinfo.splitlength > 0 )
		{
			SAFE_DELETE_ARRAY( m_SplitMessage[i] );
			m_SplitMessage[i] = new TCHAR[splitinfo.splitlength[i] + 1];

			if( !m_SplitMessage[i] )
			{
				_XFatalError( "Can't allocate chatting balloon message string" );
				return;
			}

			memset( m_SplitMessage[i], 0, sizeof(TCHAR) * splitinfo.splitlength[i] + 1 );
			strncpy( m_SplitMessage[i], splitinfo.splitpoint[i], splitinfo.splitlength[i] );

			int strringwidth = g_XBaseFont->GetWidth( m_SplitMessage[i] );
			if( m_MaxWidth < strringwidth )
			{
				m_MaxWidth = strringwidth;
			}
		}
	}
}

void _XWindow_ChatBalloon::Draw(  int X, int Y  )
{		
	if( m_Counter == 0 ) m_Counter = g_LocalSystemTime;

	if( g_LocalSystemTime - m_Counter > _XDEF_CHATBALLOONVIEWTIME )
	{
		m_Show = FALSE;
		m_Counter = 0;
		m_SplitCount = 0;
		m_MaxWidth = 0;

		for( int i = 0; i < _XDEF_CHATBALLOONSEPARATECOUNT; i++ )
		{
			SAFE_DELETE_ARRAY( m_SplitMessage[i] );
		}		
	}

	int halfwidth = (m_MaxWidth >> 1) + 6;
	int height = g_XBaseFont->GetHeight();
	int windowheight = m_SplitCount * height;

//	_XDrawSolidBar(  X - halfwidth, Y - 2, X + halfwidth, Y + windowheight +  1, D3DCOLOR_ARGB(96, 0,0,0) );
	_XDrawSolidBar(  X - halfwidth - 3, Y - 3, X + halfwidth + 3, Y + windowheight +  2, D3DCOLOR_ARGB(76, 0,0,0) );
	//_XDrawRectAngle( X - halfwidth, Y - 2, X + halfwidth, Y + windowheight +  1, 1.0f, D3DCOLOR_ARGB(255, 0,0,0) );

	g_XBaseFont->SetColor( m_MessageColor );
	for( int i = 0; i < m_SplitCount; i++ )
	{
		g_XBaseFont->PutsAlign( X, Y + (i*height), _XFONT_ALIGNTYPE_CENTER, m_SplitMessage[i] );
	}
	g_XBaseFont->Flush();
}