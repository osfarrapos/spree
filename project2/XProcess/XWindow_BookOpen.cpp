// XWindow_BookOpen.cpp: implementation of the _XWindow_BookOpen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindowObjectDefine.h"
#include "XWindow_BookOpen.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_BookOpen::_XWindow_BookOpen()
{
	m_pMainBorder = NULL;
	m_pTitleBorder = NULL;
}

_XWindow_BookOpen::~_XWindow_BookOpen()
{

}

BOOL _XWindow_BookOpen::Initialize(void)
{
	// 바탕 이미지 
	int resourceimageindex = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );

	m_pMainBorder = new _XImageStatic;
	m_pMainBorder->Create( 0, 0, 425, 410, &g_MainInterfaceTextureArchive, resourceimageindex );
	m_pMainBorder->SetClipRect(0, 0, 425, 410);
	m_pMainBorder->ShowWindow(TRUE);
	InsertChildObject( m_pMainBorder );

	// Title바 이미지
	m_pTitleBorder = new _XImageStatic;
	m_pTitleBorder->Create( 20, 40, 401, 52, &g_MainInterfaceTextureArchive, resourceimageindex );
	m_pTitleBorder->SetClipRect(4, 461, 379, 486);
	m_pTitleBorder->ShowWindow(TRUE);
	InsertChildObject( m_pTitleBorder );
	
	// 버튼들 ------------------------------------------------------------------------------┓	
	_XBTN_STRUCTURE btnstruct = { TRUE, {173, 352}, {91, 27}, _XDEF_BOOKOPEN_CLOSE, 
		resourceimageindex, resourceimageindex, resourceimageindex, &g_MainInterfaceTextureArchive };
	
	_XButton* pClose = new _XButton;
	pClose->Create(btnstruct);
	pClose->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_CLOSE );
	pClose->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
	pClose->SetButtonImageClipRect(_XBUTTONIMAGE_READY,  400, 457, 491, 484);
	pClose->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK,  400, 457, 491, 484);
	InsertChildObject(pClose);
	// -------------------------------------------------------------------------------------┛

	int hedekimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );

	_XSCRBAR_STRUCTURE htmlscrbarstructure = 
	{
		TRUE, { m_WindowPosition.x+375, m_WindowPosition.y + 64 }, 
			12, 272, // width, height
			12,		 // btnysize
			22,		 // trackbar size
			270, 270, 0,
			hedekimageindex,
			hedekimageindex,
			hedekimageindex,
			hedekimageindex,
			hedekimageindex,
			&g_MainInterfaceTextureArchive
	};

	m_HTMLListScrollBar.Create( htmlscrbarstructure );
	m_HTMLListScrollBar.ShowWindow( TRUE );
	m_HTMLListScrollBar.SetViewBorder( TRUE );
	m_HTMLListScrollBar.SetViewNullBorder( FALSE );
	m_HTMLListScrollBar.SetViewLineCount(270);
	m_HTMLListScrollBar.UpdateData();
	m_HTMLListScrollBar.SetPagecount( 64 );
	
	_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
	{ 
		{ 0,  0, 12, 12 },
		{ 24, 0, 36, 12 },
		{ 0,  12, 12, 24 },
		{ 24, 12, 36, 24 },
		{ 40,157, 52, 179}
	};
	m_HTMLListScrollBar.SetClipRect( cliprectstructure );

	m_HTMLViewStatic.Create( m_WindowPosition.x + 40, m_WindowPosition.y + 75, 
		                     m_WindowPosition.x + 374, m_WindowPosition.y + 344, 
							 &g_MainInterfaceTextureArchive, 0 );

	m_HTMLViewStatic.SetClipRect( 0, 0, 335, 270 );

	m_pScrbarFocusedObject = NULL;

	return FALSE;
}

void _XWindow_BookOpen::DestroyWindow(void)
{

}

void _XWindow_BookOpen::Draw(_XGUIObject*& pfocusedobject )
{
	if(!this->m_ShowWindow)		return;

	_XWindow::Draw(pfocusedobject);

	g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	g_XBaseFont->PutsAlign( m_WindowPosition.x+m_WindowSize.cx/2, m_WindowPosition.y + 46, _XFONT_ALIGNTYPE_CENTER, m_TitleName );	
	g_XBaseFont->PutsAlign( m_WindowPosition.x+m_WindowSize.cx/2, m_WindowPosition.y + 46, _XFONT_ALIGNTYPE_CENTER, m_TitleName );	
	g_XBaseFont->Flush();
	
	g_BookOpenHTMLWrapper.UpdateTextureBuffer( m_HTMLListScrollBar.GetLineScrollPos() );
	m_HTMLListScrollBar.Draw( m_pScrbarFocusedObject );

	m_HTMLViewStatic.DrawWithRegion( g_BookOpenHTMLWrapper.m_pHTMLTexture );
}

BOOL _XWindow_BookOpen::Process(_XGUIObject*& pfocusobject )
{
	if(!this->m_ShowWindow)		return FALSE;	
	if(!this->m_Enable)		return FALSE;
	
	if( m_pScrbarFocusedObject ) return TRUE;
	
	m_HTMLListScrollBar.Process( pfocusobject );
	
	if(!_XWindow::Process(pfocusobject)) return FALSE;	
	return TRUE;
}

BOOL _XWindow_BookOpen::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( m_HTMLListScrollBar.ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	if( gpInput->CheckMousePos( m_WindowPosition.x+40, m_WindowPosition.y+75, m_WindowPosition.x+374, m_WindowPosition.y+344 ) )
	{
		int MoveDelta  = (FLOAT)zDelta;
		
		if( zDelta > 0 )
		{
			m_HTMLListScrollBar.LineUp( abs(MoveDelta) );
		}
		else
		{
			m_HTMLListScrollBar.LineDown( abs(MoveDelta) );
		}

		return TRUE;
	}
		
	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;
}

void _XWindow_BookOpen::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow(X, Y);

	m_HTMLViewStatic.MoveWindow( m_WindowPosition.x + 40, m_WindowPosition.y +75 );
	m_HTMLListScrollBar.MoveWindow( m_WindowPosition.x + 375, m_WindowPosition.y + 74 );
}

BOOL _XWindow_BookOpen::SetBookOpenMode(int fileid, char mode )
{
	if( fileid < 0  ) return FALSE;	
	
	TCHAR urlstring[_XDEF_MAX_URLSTRINGLENGTH];
	strcpy( urlstring, gModulePath );
	strcat( urlstring, _T("\\Book\\") );

	TCHAR tempfilename[64];
	switch(mode) 
	{
	case _XGI_SC_BOOK_SCROLL :
		{
			m_pTitleBorder->ShowWindow(FALSE);
			
			sprintf( tempfilename, "SCROLL%d.HTML", fileid );
			strcat( urlstring, tempfilename );
			
			int resourceimageindex = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_05.tga" );
			m_pMainBorder->SetTexture( &g_MainInterfaceTextureArchive, resourceimageindex );
		}
		break;
	case _XGI_SC_BOOK_STUFF :
		{
			m_pTitleBorder->ShowWindow(FALSE);

			sprintf( tempfilename, "STUFF%d.HTML", fileid );
			strcat( urlstring, tempfilename );

			int resourceimageindex = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_06.tga" );
			m_pMainBorder->SetTexture( &g_MainInterfaceTextureArchive, resourceimageindex );
		}
		break;
	case _XGI_SC_BOOK_PAPER :
		{
			m_pTitleBorder->ShowWindow(TRUE);

			sprintf( tempfilename, "PAPER%d.HTML", fileid );
			strcat( urlstring, tempfilename );
			
			int resourceimageindex = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
			m_pMainBorder->SetTexture( &g_MainInterfaceTextureArchive, resourceimageindex );
		}
		break;
	}
	
	g_BookOpenHTMLWrapper.SetViewHeight( 270 );
	g_BookOpenHTMLWrapper.LoadHTML( urlstring, 335 );
	g_BookOpenHTMLWrapper.UpdateTextureBuffer( 0 );
	
	m_HTMLListScrollBar.SetTotalLineCount( g_BookOpenHTMLWrapper.m_uMaxHeight );
	m_HTMLListScrollBar.SetScrollPos( 0 );
	m_HTMLListScrollBar.SetLineScrollPos( 0 );
	m_HTMLListScrollBar.SetPagecount( 64 );

	return TRUE;
}

void _XWindow_BookOpen::SetTitleName(short sID)
{
	memset( m_TitleName, 0, sizeof(TCHAR)*64 );

	strcpy( m_TitleName, _XGameItem::GetItemName(_XGI_FC_BOOK, sID) );
}