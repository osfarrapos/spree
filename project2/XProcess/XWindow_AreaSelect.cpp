// XWindow_AreaSelect.cpp: implementation of the _XWindow_AreaSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"

#include "XWindow_AreaSelect.h"
#include "XSR_STRINGHEADER.H"

extern void _XDrawEdgedSolidBar( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_AreaSelect::_XWindow_AreaSelect()
{
	m_pListBox = NULL;
	m_pFocusedArea = NULL;
}

_XWindow_AreaSelect::~_XWindow_AreaSelect()
{

}

BOOL _XWindow_AreaSelect::Initialize( void )
{
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

	int scrollbarresourceindex = g_MainInterfaceTextureArchive.FindResource( "scrollbar.tga" );
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,								// 윈도우 활성화
		{ 6,   26 },						// 윈도우 좌표
		{ 300, 84-36 },						// 윈도우 사이즈
		_XDEF_AREASELECTWINDOW_LISTBOX,		// 윈도우 아이디
		-1,									// 윈도우 border 이미지 인덱스
		&g_MainInterfaceTextureArchive,		// Texture 리스트
		_T(" "),							// 윈도우 타이틀
		14,									// 리스트 아이템 라인 피치
		7,									// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,			// 스크롤바의 위치
		
		// Scroll bar property
		{10, 84-36},
		10, 
		20,
		100,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex
	};
	
	m_pListBox = new _XListBox;
	m_pListBox->Create(listboxstructure);
	
	POINT coloffset = { 2, 2 };
	m_pListBox->SetColumnOffset(0, coloffset);	
	m_pListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_pListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);	
	m_pListBox->SetColumnWidth(0, 288 );
	
	coloffset.x = 144;
	m_pListBox->SetColumnOffset(1, coloffset);
	m_pListBox->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_pListBox->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_pListBox->SetColumnWidth(1, 65 );
	
	m_pListBox->SetTrackWidth(288);
	m_pListBox->SetTrackHeight(14);
	m_pListBox->SetTrackBarColor( D3DCOLOR_ARGB(64,128,128,128) );
	m_pListBox->SetSelectedTrackBarColor( D3DCOLOR_ARGB(128,148,128,148) );
	m_pListBox->EnableSelectItem(TRUE);
	
	_XVScrollBar* pScrollBar = m_pListBox->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 10, 10 },
			{ 20, 0, 30, 10 },
			{ 0, 10, 10, 20 },
			{ 20,10, 30, 20 },
			{ 20,20, 30, 40 }			
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}
	
	InsertChildObject(m_pListBox);	

	int btnimageindex = g_MainInterfaceTextureArchive.FindResource("MI_Quest2.tga");
	
	_XBTN_STRUCTURE movebtnstruct = 
	{
		TRUE, {(310>>1) - 76 - 3, 112-36}, {76, 22},
			_XDEF_AREASELECTWINDOW_MOVEBUTTON,
			btnimageindex,btnimageindex,btnimageindex,
			&g_MainInterfaceTextureArchive
	};
	_XButton* pMoveButton = new _XButton;
	pMoveButton->Create(movebtnstruct);
	pMoveButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 81, 192, 157, 214);
	pMoveButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 81, 192, 157, 214);
	pMoveButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 190, 76, 212);
	pMoveButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	pMoveButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_OK );
	InsertChildObject(pMoveButton);

	_XBTN_STRUCTURE closebtnstruct = 
	{
		TRUE, {(310>>1) + 3, 112-36}, {76, 22},
			_XDEF_AREASELECTWINDOW_CLOSEBUTTON,
			btnimageindex,btnimageindex,btnimageindex,
			&g_MainInterfaceTextureArchive
	};
	_XButton* pCloseButton = new _XButton;
	pCloseButton->Create(closebtnstruct);
	pCloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 81, 192, 157, 214);
	pCloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 81, 192, 157, 214);
	pCloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 190, 76, 212);
	pCloseButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	pCloseButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_CANCEL);
	InsertChildObject(pCloseButton);

	return TRUE;
}

void _XWindow_AreaSelect::SetBlackClan( void )
{
	int clancenterindex[ 3 ] = 
	{
		3, 4, 6
	};
	
	for( int i = 0; i < 3; i++ )
	{
		m_pListBox->InsertListItem( g_ZoneInfoTable[clancenterindex[i]-1].zonename , 0xFFDFDFDF, clancenterindex[i] );
		m_pListBox->SetItemAttribHighlight( i, 0, 0xFFFFFFFF);
	}
}

void _XWindow_AreaSelect::SetWhiteClan( void )
{
	int clancenterindex[ 3 ] = 
	{
		1, 2, 5	
	};
	
	for( int i = 0; i < 3; i++ )
	{
		m_pListBox->InsertListItem( g_ZoneInfoTable[clancenterindex[i]-1].zonename , 0xFFDFDFDF, clancenterindex[i] );
		m_pListBox->SetItemAttribHighlight( i, 0, 0xFFFFFFFF);
	}
}

void _XWindow_AreaSelect::InsertAreaList(  int areaindex, LPTSTR zonename )
{

	/*
	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || 
	    g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) //타국가 임시 처리
	{
		m_pListBox->InsertListItem( g_ZoneInfoTable[areaindex].zonename , 0xFFDFDFDF, areaindex+1);
	}
	else*/
	{
		if( zonename && (strlen(zonename) > 0) )
		{
			m_pListBox->InsertListItem( zonename , 0xFFDFDFDF, areaindex+1);
		}
		else
		{
			m_pListBox->InsertListItem( g_ZoneInfoTable[areaindex].zonename , 0xFFDFDFDF, areaindex+1);
		}
	}
	
	m_pListBox->SetItemAttribHighlight( m_pListBox->GetItemCount()-1, 0, 0xFFFFFFFF);
	
}

void _XWindow_AreaSelect::DestroyWindow( void )
{	
	_XWindow::DestroyWindow();
	m_pListBox = NULL;
	m_pFocusedArea = NULL;
}

void _XWindow_AreaSelect::ShowWindow( BOOL show )
{	
	if( !show )
	{
		// 다중 맵 이동일때는 리스트 아이템들을 삭제한다.
		if( m_pFocusedArea)
		{
			if( m_pFocusedArea->m_SpecialFunction == 6 )
			{
				m_pListBox->DeleteAllItem();				
			}
		}

	}
	_XWindow::ShowWindow( show );
}

void _XWindow_AreaSelect::MoveWindow( int X, int Y )
{			
	// Clipping from screen size	
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_AreaSelect::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		_XDrawEdgedSolidBar( m_WindowPosition.x, m_WindowPosition.y+15,  m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy, 0x7f000000 );		
	}

	_XWindow::Draw(pfocusobject);

	if( !this->m_ShowWindow ) return;

	g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
	g_XBaseFont->PrintAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 3, 1.0f, 
								_XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_AREASELECT_2463) );
	g_XBaseFont->Flush();	
}

BOOL _XWindow_AreaSelect::Process( _XGUIObject*& pfocusobject )
{	
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	if( !m_pFocusedArea )
	{
		//ShowWindow( FALSE );
	}
	else
	{
		FLOAT dx = ( g_pLocalUser->m_Position.x - m_pFocusedArea->m_CenterPoint.x);
		FLOAT dz = ( g_pLocalUser->m_Position.z - m_pFocusedArea->m_CenterPoint.z);
		FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );

		if( _distance > 15.0f )
		{
			ShowWindow( FALSE );
		}
	}
	

	return TRUE;
}

void _XWindow_AreaSelect::SelectClan( int clanindex )
{
	int clanzoneconverttable[9] = 
	{
		// NANGIN, GAEBANG, BEEGOONG, SORIM, NOCKRIM, MOODANG, MAKYO
		0, 2, 4, 1, 3, 5, 6
	};

	if( clanindex == 0 ) // 낭인일때...
	{
		m_pListBox->SelectItem( 0 );
		return;
	}

	int cnt = m_pListBox->GetItemCount();

	for( int i = 0; i < cnt; i++ )
	{
		if( m_pListBox->GetItemCommandID( i ) == clanzoneconverttable[clanindex] )
		{
			m_pListBox->SelectItem( i );
			return;
		}
	}
}