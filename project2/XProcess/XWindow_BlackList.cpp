// XWindow_BlackList.cpp: implementation of the _XWindow_BlackList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_BlackList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_BlackList::_XWindow_BlackList()
{
	m_listboxBlackList 		=	NULL;
}

_XWindow_BlackList::~_XWindow_BlackList()
{
	m_BlackList.disposeList();
}


BOOL _XWindow_BlackList::Initialize( void )
{
	int mainbackinterface = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );
	int optionmenuinterface = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 183, 59, &g_MainInterfaceTextureArchive, mainbackinterface );
	pTitleBar->SetClipRect( 0, 125, 183, 184 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pBottomBar = new _XImageStatic;
	pBottomBar->Create( 0, 59, 182, 269, &g_MainInterfaceTextureArchive, optionmenuinterface );
	pBottomBar->SetClipRect( 0, 0, 183, 211 );
	InsertChildObject( pBottomBar );
	
		
	// Blacklist 닫기 버튼	 ( 12 X 12 ) 
	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_BLACKLISTWINDOW_CLOSEBUTTON,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* btnClose = new _XButton;
	btnClose->Create( closebtnstruct );	
	
	btnClose->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	btnClose->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	btnClose->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( btnClose );
	
	//Blacklist 도움 버튼 ( 12 X 12 ) 
	_XBTN_STRUCTURE helpstruct = { TRUE, { 153, 2 }, { 12, 12 }, _XDEF_BLACKLISTWINDOW_HELPBUTTON,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* btnHelp = new _XButton;
	btnHelp->Create( helpstruct );	
	
	btnHelp->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	btnHelp->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	btnHelp->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( btnHelp );

	int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );

	//메신져 그룹과 친구리스트를 표시하는 리스트 박스 
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 8, 48  },		// 윈도우 좌표
		{ 168, 168 },										// 윈도우 사이즈
		_XDEF_MESSENGERWINDOW_BLACKLISTBOX,					// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(""),											// 윈도우 타이틀
		16,												// 리스트 아이템 라인 피치
		10,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치

		// scroll bar property
		{ 12, 168 },									// 스크롤바 사이즈
		12,												// 스크롤바 상하 버튼 사이즈
		22,												// 스크롤바 트랙바 사이즈
		0,												// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex

	};
	m_listboxBlackList = new _XListBox;
	m_listboxBlackList->Create(listboxstructure);
	
	_XVScrollBar* pScrollBar = m_listboxBlackList->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}
	
	POINT coloffset1 = { 0, 4 };
	POINT coloffset2 = { 26, 4 };
	m_listboxBlackList->SetColumnOffset(0, coloffset1);				// column offset
	m_listboxBlackList->SetColumnOffset(1, coloffset2);
	
	m_listboxBlackList->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style 
	m_listboxBlackList->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	
	m_listboxBlackList->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxBlackList->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxBlackList->SetColumnWidth(0, 26);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxBlackList->SetColumnWidth(1, 130 );
	
	m_listboxBlackList->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
	m_listboxBlackList->SetSelectedImagePosition(0, 1);
	m_listboxBlackList->SetSelectedImageHeight(16);
	m_listboxBlackList->SetTrackWidth(155);

	m_listboxBlackList->SetScrollPos(0);
	InsertChildObject(m_listboxBlackList);

	m_listboxBlackList->SetDrawBorder( TRUE );
	m_listboxBlackList->SetBorderColorFactor( D3DCOLOR_ARGB( 160, 0, 0, 0 ) );
		
	int resourceimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );

	_XBTN_STRUCTURE btnstruct = { TRUE, { (m_WindowSize.cx>>1)-66, 218 }, { 64, 24 }, 
									_XDEF_MESSENGERWINDOW_ADDBLACKLISTBUTTON,
									resourceimageindex,
									resourceimageindex,
									resourceimageindex,
									&g_MainInterfaceTextureArchive };
			
	_XButton* pAddButton = new _XButton;
	pAddButton->Create( btnstruct );
	pAddButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 230,  66, 254 );
	pAddButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  69, 230,  133, 254 );
	pAddButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 230,  66, 254 );
	pAddButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3267 );//"추 가"
	InsertChildObject(pAddButton);

	btnstruct.commandid = _XDEF_MESSENGERWINDOW_DELETEBLACKLISTBUTTON;
	btnstruct.position.x = (m_WindowSize.cx>>1)+2;
	_XButton* pDeleteButton = new _XButton;
	pDeleteButton->Create( btnstruct );
	pDeleteButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 230,  66, 254 );
	pDeleteButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  69, 230,  133, 254 );
	pDeleteButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 230,  66, 254 );
	pDeleteButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_MAILDEL );//"삭 제" );
	InsertChildObject(pDeleteButton);

	btnstruct.commandid = _XDEF_BLACKLISTWINDOW_CLOSEBUTTON;
	btnstruct.position.x = (m_WindowSize.cx>>1)-32;
	btnstruct.position.y = 243;

	_XButton* pCloseButton = new _XButton;
	pCloseButton->Create( btnstruct );
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 230,  66, 254 );
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  69, 230,  133, 254 );
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 230,  66, 254 );
	pCloseButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_CLOSE );//"닫 기"	
	InsertChildObject(pCloseButton);
	

	return TRUE;
}

void _XWindow_BlackList::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_BlackList::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow(X,Y);
}
	
void _XWindow_BlackList::ShowWindow( BOOL show )
{
	_XWindow::ShowWindow( show );
}

void _XWindow_BlackList::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw(pfocusobject);	
	
	if( !this->m_ShowWindow ) return;

	g_XBaseFont->SetColor( D3DCOLOR_ARGB(255,255,128,64) );
	g_XBaseFont->SetBoldMode( TRUE );
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 29, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3422) );
	g_XBaseFont->Flush();
	g_XBaseFont->SetBoldMode( FALSE );
}	

BOOL _XWindow_BlackList::Process( _XGUIObject*& pfocusobject )
{
	_XWindow::Process(pfocusobject);

	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;


	return TRUE;
}

void _XWindow_BlackList::RebuildListBox(void)
{
	_ASSERT( m_listboxBlackList );

	m_listboxBlackList->DeleteAllItem();

	TCHAR str[64];
	int   index = 0;
	if( !m_BlackList.listEmpty() )
	{
		m_BlackList.resetList();

		do 
		{
			_XStringItem* pCharacterName = (_XStringItem*)m_BlackList.currentItem();

			sprintf( str, "%d", index+1 );
			m_listboxBlackList->InsertListItem( str, D3DCOLOR_ARGB(255, 200,200,200), index+1 );
			m_listboxBlackList->SetItemAttribHighlight( index, 0, D3DCOLOR_ARGB(255, 255,255,255) );
			
			m_listboxBlackList->SetItemText( index, 1, pCharacterName->Get_String() );
			m_listboxBlackList->SetItemAttrib( index, 1, D3DCOLOR_ARGB(255, 200,200,200) );
			m_listboxBlackList->SetItemAttribHighlight( index, 1, D3DCOLOR_ARGB(255, 255,255,255) );
				
			++index;
			m_BlackList.nextItem();
			
		} while( !m_BlackList.atHeadOfList() );
	}
}

BOOL _XWindow_BlackList::InsertCharacter( LPCTSTR charactername )
{
	if( !charactername )
		return FALSE;

	_XStringItem* pCharacterName = new _XStringItem( (LPTSTR)charactername, 0, 0 );

	if(!pCharacterName)
		return FALSE;

	m_BlackList.resetList();
	m_BlackList.insertItem( pCharacterName );

	return TRUE;
}

BOOL _XWindow_BlackList::DeleteCharacter( int listindex )
{
	int curindex = 0;
	if( !m_BlackList.listEmpty() )
	{
		m_BlackList.resetList();

		do 
		{
			if( curindex == listindex )
			{
				m_BlackList.deleteItem( m_BlackList.currentItem() );			
				return TRUE;
			}
			m_BlackList.nextItem();
			++curindex;
		} while( !m_BlackList.atHeadOfList() );
	}

	return FALSE;
}

BOOL _XWindow_BlackList::DeleteCharacter( LPCTSTR charactername )
{
	if( !charactername )
		return FALSE;

	if( !m_BlackList.listEmpty() )
	{
		m_BlackList.resetList();

		do 
		{
			_XStringItem* pCharacterName = (_XStringItem*)m_BlackList.currentItem();
			if( pCharacterName )
			{
				if( strcmp( pCharacterName->Get_String(), charactername ) == 0 )
				{
					m_BlackList.deleteItem( pCharacterName );
					return TRUE;
				}
			}
			m_BlackList.nextItem();
		} while( !m_BlackList.atHeadOfList() );
	}

	return FALSE;
}

BOOL _XWindow_BlackList::FindCharacter( LPCTSTR charactername )
{
	if( !charactername )
		return FALSE;

	if( !m_BlackList.listEmpty() )
	{
		m_BlackList.resetList();

		do 
		{
			_XStringItem* pCharacterName = (_XStringItem*)m_BlackList.currentItem();
			if( pCharacterName )
			{
				if( strcmp( pCharacterName->Get_String(), charactername ) == 0 )
				{
					return TRUE;
				}
			}
			m_BlackList.nextItem();
		} while( !m_BlackList.atHeadOfList() );
	}

	return FALSE;
}

BOOL _XWindow_BlackList::LoadList( LPCTSTR charactername )
{
	m_BlackList.disposeList();

	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );	

	TCHAR filename[64];
	sprintf( filename, "%s.xbl", charactername );
	FILE* pFile = fopen( filename, "rb" );

	if( !pFile )
	{
		//_XLog( "ERROR : Could not open %d file", filename );
		RebuildListBox();
		return FALSE;
	}

	int itemcount = 0;
	fread( &itemcount, sizeof(int), 1, pFile );

	for( int i = 0; i < itemcount; i++ )
	{
		int strlength = 0;
		TCHAR blacklistname[64];

		fread( &strlength, sizeof(int), 1, pFile );

		if( strlength > 0 )
		{
			memset( blacklistname, 0, sizeof(TCHAR) * 64 );
			fread( blacklistname, sizeof(TCHAR) * strlength, 1, pFile );
			InsertCharacter( blacklistname );
		}
	}		
	
	fclose( pFile );

	RebuildListBox();
	
	return TRUE;
}


BOOL _XWindow_BlackList::SaveList( LPCTSTR charactername )
{
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );	
	TCHAR filename[64];
	sprintf( filename, "%s.xbl", charactername );
	FILE* pFile = fopen( filename, "wb" );

	if( !pFile )
	{
		_XLog( "ERROR : Could not open %d file", filename );
		return FALSE;
	}
	
	int itemcount = m_BlackList.getitemcount();
	fwrite( &itemcount, sizeof(int), 1, pFile );

	if( !m_BlackList.listEmpty() )
	{
		m_BlackList.resetList();

		do 
		{
			_XStringItem* pCharacterName = (_XStringItem*)m_BlackList.currentItem();
			if( pCharacterName )
			{
				int strlength = pCharacterName->Get_Strlength();
				fwrite( &strlength, sizeof(int), 1, pFile );
				fwrite( pCharacterName->Get_String(), sizeof(TCHAR) * strlength, 1, pFile );
			}
			m_BlackList.nextItem();
		} while( !m_BlackList.atHeadOfList() );
	}
	
	fclose( pFile );
	return TRUE;
}