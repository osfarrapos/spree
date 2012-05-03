// XWindow_NPCChatBalloon.cpp: implementation of the _XWindow_NPCChatBalloon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_NPCChatBalloon.h"

#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_NPCChatBalloon::_XWindow_NPCChatBalloon()
{
	memset( m_PrevGreetingMessage, 0, sizeof(LPTSTR) * _XDEF_NPCCHATBALLOONSEPARATECOUNT );

	memset( m_NPCRoll, 0, sizeof(TCHAR) * 256 );
	memset( m_NPCName, 0, sizeof(TCHAR) * 256 );	

	m_SplitCount = 0;
}

_XWindow_NPCChatBalloon::~_XWindow_NPCChatBalloon()
{
	for( int i = 0; i < _XDEF_NPCCHATBALLOONSEPARATECOUNT; i++ )
	{
		SAFE_DELETE_ARRAY( m_PrevGreetingMessage[i] );
	}
}

void _XWindow_NPCChatBalloon::SetMessage( LPTSTR message )
{	
	if( !message ) return;

	for( int i = 0; i < _XDEF_NPCCHATBALLOONSEPARATECOUNT; i++)
	{
		SAFE_DELETE_ARRAY( m_PrevGreetingMessage[i] );
	}
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		int stringlength = strlen(message)+1;
		m_PrevGreetingMessage[0] = new TCHAR[stringlength];
		strcpy( m_PrevGreetingMessage[0], message );
		
		LPTSTR pStr = m_PrevGreetingMessage[0];
		do 
		{
			if( *pStr == NULL )
			{
				break;
			}
			if( *pStr == _T('\n') )
				*pStr = _T(' ');
			else if( *pStr == _T('\r') )
				*pStr = _T(' ');
			pStr++;
			
		} while( *pStr );

		m_SplitCount = g_XBaseFont->GetRowCount( m_PrevGreetingMessage[0], _XDEF_MAXNPCCHATBALLOONLINEWIDTH );
	}
	else
	{	
		_XStringSplitInfo splitinfo;
		//g_XBaseFont->SplitString( message, 180, splitinfo, 0 );
		g_XBaseFont->SplitStringByCR( message, splitinfo );

		for( i = 0; i < splitinfo.splitcount; i++ )
		{
			if( splitinfo.splitlength > 0 )
			{
				m_PrevGreetingMessage[i] = new TCHAR[splitinfo.splitlength[i] + 1];
				
				if( !m_PrevGreetingMessage[i] )
				{
					_XFatalError( "Can't allocate chatting balloon message string" );
					return;
				}
				
				memset( m_PrevGreetingMessage[i], 0, sizeof(TCHAR) * splitinfo.splitlength[i] + 1 );
				strncpy( m_PrevGreetingMessage[i], splitinfo.splitpoint[i], splitinfo.splitlength[i] );
			}
		}
	}
}

void _XWindow_NPCChatBalloon::Draw(  int X, int Y )
{	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		_XDrawSolidBar( X-3-(_XDEF_MAXNPCCHATBALLOONLINEWIDTH>>1), Y- 2, 
					    X-3-(_XDEF_MAXNPCCHATBALLOONLINEWIDTH>>1) + _XDEF_MAXNPCCHATBALLOONLINEWIDTH + 3, 
						Y + g_XBaseFont->GetHeight() * m_SplitCount + 2, D3DCOLOR_ARGB(96, 0,0,0) );
		_XDrawRoundRectAngle( X-4-(_XDEF_MAXNPCCHATBALLOONLINEWIDTH>>1), Y- 3, 
							  X-4-(_XDEF_MAXNPCCHATBALLOONLINEWIDTH>>1) + _XDEF_MAXNPCCHATBALLOONLINEWIDTH + 4, Y + g_XBaseFont->GetHeight() * m_SplitCount + 3, 0.0f, 0xFF7F7F7F );

		g_XBaseFont->SetColor( _XSC_DEFAULT ); // Author : 양희왕 //breif : 글씨색 지정이 빠져 있음.. 08/01/16
		g_XBaseFont->Puts_SeparateAlign( X, Y, m_PrevGreetingMessage[0], _XDEF_MAXNPCCHATBALLOONLINEWIDTH, _XFONT_ALIGNTYPE_CENTER );		
	}
	else
	{
		// 한 줄 최대 16자 
		// 최대 2줄 	
		int halfwidth = (_XDEF_MAXNPCCHATBALLOONLINEWIDTH >> 1) + 6;
		int height = g_XBaseFont->GetHeight() + 2;
		int windowheight = _XDEF_NPCCHATBALLOONSEPARATECOUNT * height;

		_XDrawSolidBar(  X - halfwidth - 5, Y - 2, X + halfwidth, Y + windowheight + 2, D3DCOLOR_ARGB(96, 0,0,0) );
			
		g_XBaseFont->SetColor( _XSC_DEFAULT ); 

		for( int i = 0; i < 2; i++ )
		{
			g_XBaseFont->PutsAlign( X - halfwidth, Y + (i*height), _XFONT_ALIGNTYPE_LEFT, m_PrevGreetingMessage[i] );
		}
		
	// 2004.06.20->oneway48 insert	
	//	int strringwidth = g_XBaseFont->GetWidth( m_NPCRoll );
	//	g_XBaseFont->PutsAlign( X - halfwidth , Y , _XFONT_ALIGNTYPE_LEFT, m_NPCRoll );
	//	g_XBaseFont->PutsAlign( X - halfwidth + strringwidth + 10, Y , _XFONT_ALIGNTYPE_LEFT, m_NPCName );
	}

	g_XBaseFont->Flush();
}

void _XWindow_NPCChatBalloon::SetNPCNameString(LPTSTR pNPCName, LPTSTR pNPCRoll)
{

	memset( m_NPCName, 0, sizeof(TCHAR) * 256 );	
	memset( m_NPCRoll, 0, sizeof(TCHAR) * 256 );

	if( pNPCName )
	{
		strcpy( m_NPCName, pNPCName );
	}	
	if( pNPCRoll )
	{
		strcpy( m_NPCRoll, pNPCRoll );		
	}
	

}
