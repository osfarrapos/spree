// XWindow_AdminNotice.cpp: implementation of the _XWindow_AdminNotice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_AdminNotice.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern void _XDrawEdgedSolidBar( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color );


_XWindow_AdminNotice::_XWindow_AdminNotice()
{
	m_CloseButton			= NULL;
	m_NoticeListScrollBar	= NULL;
	m_pScrbarFocusedObject	= NULL;
	m_NoticeListScrollPos   = 0;
	m_MaxViewCount			= 10;
	m_AutoCloseTimer		= 0;
	m_NoticeMessageMaxWidth = 0;
}

_XWindow_AdminNotice::~_XWindow_AdminNotice()
{
	DestroyWindow();	
}

BOOL _XWindow_AdminNotice::Initialize( void )
{
	SetDrawBorder( FALSE );

	int titleimageindex = g_MainInterfaceTextureArchive.FindResource("MI_Death01.tga");

	_XImageStatic* pTitleLeftBorder = new _XImageStatic;
	pTitleLeftBorder->Create(0, 0, 128, 18, 
						&g_MainInterfaceTextureArchive,
						titleimageindex);
	pTitleLeftBorder->SetClipRect(0, 0, 128, 19);
	InsertChildObject(pTitleLeftBorder);

	_XImageStatic* pTitleCenterBorder = new _XImageStatic;
	pTitleCenterBorder->Create(128, 0, 183, 18,
		&g_MainInterfaceTextureArchive,
		titleimageindex);
	pTitleCenterBorder->SetClipRect(84, 0, 84+55, 19);
	InsertChildObject(pTitleCenterBorder);
	
	_XImageStatic* pTitleRightBorder = new _XImageStatic;
	pTitleRightBorder->Create(183, 0, 183+128, 18,
						&g_MainInterfaceTextureArchive,
						titleimageindex);
	pTitleRightBorder->SetClipRect(84, 0, 212, 19);
	InsertChildObject(pTitleRightBorder);

	int btnimageindex = g_MainInterfaceTextureArchive.FindResource("MI_Quest2.tga");

	_XBTN_STRUCTURE closebtnstruct = 
	{
		TRUE, {(310>>1) - (76>>1), 107}, {76, 22},
		_XDEF_NOTICEWINDOW_CLOSEBUTTON,
		btnimageindex,btnimageindex,btnimageindex,
		&g_MainInterfaceTextureArchive
	};
	m_CloseButton = new _XButton;
	m_CloseButton->Create(closebtnstruct);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 81, 192, 157, 214);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 81, 192, 157, 214);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 190, 76, 212);
	m_CloseButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_CloseButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_OK);
	m_CloseButton->EnableWindow(FALSE);
	InsertChildObject(m_CloseButton);

	m_MaxViewCount = 5;

	int systemmsgimgindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	
	SAFE_DELETE( m_NoticeListScrollBar );
	m_NoticeListScrollBar = new _XVScrollBar;
	_XSCRBAR_STRUCTURE noticelistscrbarstructure = 
	{
		TRUE, {m_WindowPosition.x+m_WindowSize.cx-15,m_WindowPosition.y+23 }, 
			12, 75, // width, height
			12,		 // btnysize
			22,		 // trackbar size
			0, 5, 0,
			systemmsgimgindex,
			systemmsgimgindex,
			systemmsgimgindex,
			systemmsgimgindex,
			systemmsgimgindex,
			&g_MainInterfaceTextureArchive
	};
	
	m_NoticeListScrollBar->Create( noticelistscrbarstructure );
	m_NoticeListScrollBar->ShowWindow( TRUE );
	m_NoticeListScrollBar->SetViewBorder( TRUE );	
	m_NoticeListScrollBar->SetViewNullBorder( FALSE );
	m_NoticeListScrollBar->SetTotalLineCount(0);	
	m_NoticeListScrollBar->UpdateData();

	_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
	{
		{ 0,  0, 12, 12 },
		{ 24, 0, 36, 12 },
		{ 0,  12, 12, 24 },
		{ 24, 12, 36, 24 },
		{ 40,157, 52, 179}
	};
	m_NoticeListScrollBar->SetClipRect( cliprectstructure );
				
	return TRUE;
}

void _XWindow_AdminNotice::SetDefaultPosition( void )
{		
	CheckOutside();
}

void _XWindow_AdminNotice::SaveWindowProperty( void )
{

}

void _XWindow_AdminNotice::DestroyWindow( void )
{	
	_XWindow::DestroyWindow();
	SAFE_DELETE( m_NoticeListScrollBar );	
}

void _XWindow_AdminNotice::ShowWindow( BOOL show )
{		
	_XWindow::ShowWindow( show );	
	CheckOutside();

	if( show )
	{
		m_AutoCloseTimer = 0;
	}
}

void _XWindow_AdminNotice::MoveWindow( int X, int Y )
{			
	// Clipping from screen size	
	_XWindow::MoveWindow( X, Y );
	m_NoticeListScrollBar->MoveWindow( m_WindowPosition.x+m_WindowSize.cx-15,m_WindowPosition.y+23 );
}

#define _XDEF_CHATMESSAGEBLINKTIME		10000
#define _XDEF_ADMINNOTICEAUTOCLODETIMER	5000

void _XWindow_AdminNotice::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		_XDrawEdgedSolidBar( m_WindowPosition.x, m_WindowPosition.y+15,  m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy, 0x7f000000 );
	}

	_XWindow::Draw(pfocusobject);

	if( !this->m_ShowWindow ) return;	
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// draw chat list...
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;

	TCHAR noticestring[256];
	memset(noticestring, 0, sizeof(TCHAR)*256);
	strcpy(noticestring, _XGETINTERFACETEXT(ID_STRING_ADMINNOTICE_ADMINNOTICE));

	g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
	g_XBaseFont->PrintAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 3, 1.0f, _XFONT_ALIGNTYPE_CENTER, noticestring );
	g_XBaseFont->Flush();

	g_XBaseFont->DisableGrowMode();
	
	g_XBaseFont->SetColor( _XSC_INFORMATION );

	if( !m_NoticeMessageList.listEmpty() )
	{
		m_NoticeMessageList.resetList();
		for( int i = 0; i < m_NoticeListScrollPos; i++ ) m_NoticeMessageList.nextItem();		
		
		_XStringItem* curstring;
		int xcenterpos = this->m_WindowPosition.x + 5;
		int ypos = 0;
		DWORD colorindex;		
										
		do
		{			
			curstring = (_XStringItem*)m_NoticeMessageList.currentItem();			
			colorindex = (((DWORD)curstring->Get_Attribute()) % 16);
			DWORD attrib1 = LOWORD( (DWORD)curstring->Get_Attribute() );			
			
			int CheckAdminAttrib  = (attrib1 / _XDEF_CHATMESSAGEMODE_ADMINNOTICE);
			int CheckGlobalAttrib = (attrib1 / _XDEF_CHATMESSAGEMODE_GLOBALNOTICE);
			
			if( CheckAdminAttrib == 1 )
			{
				if( (gnFPS/3) < gnFrameCounter )
				{
					g_XBaseFont->SetColor( _XSC_INFORMATION );
					g_XBaseFont->Puts( xcenterpos, (m_WindowPosition.y+31) + (ypos * 14), 
						              curstring->Get_String() );
										
					// ½Ã°£ÀÌ Áö³ª¸é ±ôºýÀÓÀ» ¾ø¾Ú.
					if( (g_LocalSystemTime - curstring->Get_Attribute(1)) > _XDEF_CHATMESSAGEBLINKTIME )
					{
						curstring->Set_Attribute( colorindex, 0 );
					}					
				}
			}
			else if( CheckGlobalAttrib == 1 )
			{
				if( (gnFPS/3) < gnFrameCounter )
				{
					g_XBaseFont->SetColor(_XSC_INFORMATION);
					g_XBaseFont->Puts( xcenterpos, (m_WindowPosition.y+31) + (ypos * 14), 
									  curstring->Get_String() );
					
					// ½Ã°£ÀÌ Áö³ª¸é ±ôºýÀÓÀ» ¾ø¾Ú.
					if( (g_LocalSystemTime - curstring->Get_Attribute(1)) > _XDEF_CHATMESSAGEBLINKTIME )
					{
						curstring->Set_Attribute( colorindex, 0 );
					}
				}
			}
			else
			{			
				g_XBaseFont->SetColor(_XSC_INFORMATION);
				g_XBaseFont->Puts( xcenterpos, (m_WindowPosition.y+24) + (ypos * 16), 
								  curstring->Get_String() );
			}
			
			ypos++;
			
			if( ypos >= m_MaxViewCount ) break;
			
			m_NoticeMessageList.nextItem();
			
		}while( !m_NoticeMessageList.atHeadOfList() );
	}	

	g_XBaseFont->Flush();
		
	m_NoticeListScrollPos = m_NoticeListScrollBar->GetLineScrollPos();
	m_NoticeListScrollBar->Draw( pfocusobject );	
}

BOOL _XWindow_AdminNotice::CheckMousePosition( void )
{
	if(  _XWindow::CheckMousePosition() ) return TRUE;	
	return FALSE;
}

BOOL _XWindow_AdminNotice::Process( _XGUIObject*& pfocusobject )
{	
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if( m_AutoCloseTimer == 0 ) m_AutoCloseTimer = g_LocalSystemTime;
	if( (g_LocalSystemTime-m_AutoCloseTimer) > _XDEF_ADMINNOTICEAUTOCLODETIMER )
	{		
		m_CloseButton->EnableWindow(TRUE);
	}
	else
	{
		m_CloseButton->EnableWindow(FALSE);
	}

	m_NoticeListScrollBar->Process( pfocusobject );
			
	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

BOOL _XWindow_AdminNotice::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if( m_NoticeListScrollBar )
	{
		if( m_NoticeListScrollBar->ProcessWheelMessage( zDelta ) )
		{
			return TRUE;
		}

		if( CheckMousePosition() )
		{
			int MoveDelta  = (FLOAT)zDelta / 40;			
			
			if( zDelta > 0 )
			{
				m_NoticeListScrollBar->LineUp( abs(MoveDelta) );
			}
			else
			{
				m_NoticeListScrollBar->LineDown( abs(MoveDelta) );
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


void _XWindow_AdminNotice::SetNoticeMessage( LPSTR message )
{
	if( !message ) return;
	int strlength = strlen(message);
	if( strlength <= 0 ) return;

	// String splitting....
	_XStringSplitInfo splitinfo;		
	
#ifdef US_RB_NEWSPLITTINGNOTICETEXT_070529_KERYGMA
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		int curpos = 0;
		LPTSTR pBuffer = message;
		while( *pBuffer != NULL )
		{
			if( *pBuffer == _T('\\') && *(pBuffer+1) == _T('n') )
			{
				*pBuffer = _T(' ');
				*(pBuffer+1) = _T('\n');
				pBuffer++;
			}			

			pBuffer++;
		}
	}
#endif


	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont->SplitString( message, m_WindowSize.cx - 26, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR  );
	}
	else
	{
		g_XBaseFont->SplitString( message, m_WindowSize.cx - 26, splitinfo );
	}
	
	m_NoticeMessageList.resetList();

	TCHAR splitstring[128];
	for( int i = 0; i < splitinfo.splitcount; i++ )
	{			
		memset( splitstring, 0, 128 );
		strncpy( splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i] );		
				
		_XStringItem* noticemessage = new _XStringItem( splitstring, g_LocalSystemTime );
		if( noticemessage )
		{				
			m_NoticeMessageList.insertItem( noticemessage );
		}
	}
		
	m_NoticeMessageList.resetList();
	m_NoticeMessageMaxWidth = 0;
	_XStringItem* pStringItem = NULL;
	do 
	{
		pStringItem = (_XStringItem*)m_NoticeMessageList.currentItem();
		
		if( pStringItem )
		{
			int width = g_XBaseFont->GetWidth( pStringItem->Get_String() );
			if( width > m_NoticeMessageMaxWidth )
			{
				m_NoticeMessageMaxWidth = width;
			}
		}
		
		m_NoticeMessageList.nextItem();
	} while( !m_NoticeMessageList.atHeadOfList() );	
	
	if( m_NoticeListScrollBar )
	{
		m_NoticeListScrollBar->SetTotalLineCount( m_NoticeMessageList.getitemcount() );						
		m_NoticeListScrollBar->SetTotalLineCount( m_NoticeMessageList.getitemcount() );
		m_NoticeListScrollBar->UpdateData();
		
		// Æ®·¢¹Ù µå·¡±ëÁß¿£ ¸Ç ¾Æ·¡¾ÆÀÌÅÛÀ¸·Î  ÀÚµ¿½ºÅ©·Ñ ¾ÈÇÔ.
		if( !m_NoticeListScrollBar->Dragging() )
		{
			m_NoticeListScrollBar->SetLineScrollPos( 999999 ); 
		}
	}								
	
	ShowWindow( TRUE );
}