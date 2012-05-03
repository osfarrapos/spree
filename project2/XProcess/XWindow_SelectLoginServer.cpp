// XWindow_SelectLoginServer.cpp: implementation of the _XWindow_SelectLoginServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_SelectLoginServer.h"
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

BOOL __stdcall _XStartServerSelectCheckCallBack( int param1, int param2 )
{
	if( !g_ZoneInfoTable[param2-1].servicezone )
	{
		g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "", g_StrManager.GetString(ID_STRING_SELECTLOGINSERVER_NOTSERVICE) , TRUE );	// 현재는 서비스 되지 않는 지역입니다
			return FALSE;
	}

	return TRUE;	
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_SelectLoginServer::_XWindow_SelectLoginServer()
{

}

_XWindow_SelectLoginServer::~_XWindow_SelectLoginServer()
{
	
}

BOOL _XWindow_SelectLoginServer::Initialize( _XTextureManager& texturearchive )
{	
	_XImageStatic* pLeftBorder = new _XImageStatic;
	pLeftBorder->Create( 0, 0, 162, 215,
					   &texturearchive, 
					   texturearchive.FindResource( "server_base_01.tga" ) );	
	pLeftBorder->SetClipRect( 0, 0, 163, 216 );
	InsertChildObject( pLeftBorder );
	
	_XImageStatic* pRightBorder = new _XImageStatic;
	pRightBorder->Create( 162, 0, 162+164, 213,
					   &texturearchive, 
					   texturearchive.FindResource( "server_base_02.tga" ) );	
	pRightBorder->SetClipRect( 0, 0, 165, 214 );
	InsertChildObject( pRightBorder );	

	int scrollbarresourceindex = g_MainInterfaceTextureArchive.FindResource( "scrollbar.tga" );
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,								// 윈도우 활성화
		{ 60,   42 },						// 윈도우 좌표
		{ 209, 100 },						// 윈도우 사이즈
		_XDEF_SELECTSERVERLISTCTRL,			// 윈도우 아이디
		-1,									// 윈도우 border 이미지 인덱스
		&g_MainInterfaceTextureArchive,		// Texture 리스트
		_T(" "),							// 윈도우 타이틀
		14,									// 리스트 아이템 라인 피치
		7,									// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,			// 스크롤바의 위치
		
		// Scroll bar property
		{10, 101},
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
	m_pListBox->SetColumnWidth(0, 198 );
	m_pListBox->SetTrackWidth(195);
	m_pListBox->SetTrackHeight(14);
	m_pListBox->SetTrackBarColor( D3DCOLOR_ARGB(128,0,0,0) );
	m_pListBox->SetSelectItemCallBack( _XStartServerSelectCheckCallBack );
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

	if( m_pListBox )
	{
		m_pListBox->DeleteAllItem();

		int count = 0;
		
		for( int i = 0; i < _XDEF_MAXZONECOUNT; i++ )
		{		
			if( g_ZoneInfoTable[i].servicezone )
			{
				m_pListBox->InsertListItem( g_ZoneInfoTable[i].zonename, _XSC_DEFAULT );
				m_pListBox->SetItemCommandID( count, g_ZoneInfoTable[i].zoneid );

				if( g_ZoneInfoTable[i].servicezone )
				{
					m_pListBox->SetItemAttrib( count, 0, _XSC_DEFAULT );
					m_pListBox->SetItemAttribHighlight( count, 0, _XSC_DEFAULT_HIGHLIGHT );
				}
				else
				{
					m_pListBox->SetItemAttrib( count, 0, _XSC_DEFAULT_HIGHLIGHT );
					m_pListBox->SetItemAttribHighlight( count, 1, _XSC_DEFAULT_HIGHLIGHT );
				}

				count++;
			}
		}
	}

	return TRUE;
}

void _XWindow_SelectLoginServer::ShowWindow( BOOL show )
{
	m_pListBox->SelectItem( -1 );
	_XWindow::ShowWindow( show );
}

void _XWindow_SelectLoginServer::MoveWindow( int X, int Y )
{		
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_SelectLoginServer::DestroyWindow( void )
{	
	_XWindow::DestroyWindow();
}

void _XWindow_SelectLoginServer::Draw( _XGUIObject*& pfocusobject )
{			
	_XWindow::Draw(pfocusobject);	
	
	if( !this->m_ShowWindow ) return;	
}

BOOL _XWindow_SelectLoginServer::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}




