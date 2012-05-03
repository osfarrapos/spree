// XWindow_PK_ListBox.cpp: implementation of the XWindow_PK_ListBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_PK_ListBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XWindow_PK_ListBox::XWindow_PK_ListBox() : m_pListBox(NULL), m_nSelectedItem(0), m_nLapsed(0), m_dwUpdatedTime(0)
{

}

XWindow_PK_ListBox::~XWindow_PK_ListBox()
{
//	DestroyWindow();
}

BOOL XWindow_PK_ListBox::Initialize()
{
	//맨위 타이틀 이미지
	_XImageStatic* pTitleImage = new _XImageStatic;
	pTitleImage->Create( 0, 0, 154, 17, //만들어진 윈도우 에서의 좌표 클라이언트 윈도우 말고
						&g_MainInterfaceTextureArchive,
						g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName));	
	pTitleImage->SetClipRect( 0, 206, 153, 223 ); // 파일에서의 좌표
	InsertChildObject( pTitleImage );

	//제목 쓰는곳 이미지
	_XImageStatic* psubjectImage = new _XImageStatic;
	psubjectImage->Create( 3, 17, 154, 32,
							&g_MainInterfaceTextureArchive, 
							g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName));
	psubjectImage->SetClipRect( 0, 223, 150, 238 );
	InsertChildObject( psubjectImage );

	//바닥
	_XImageStatic* pBottomImage = new _XImageStatic;
	pBottomImage->Create( 0, m_WindowSize.cy-18, 154, m_WindowSize.cy,
						&g_MainInterfaceTextureArchive,
						g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName));	
	pBottomImage->SetScale( 1.6f, 1.0f);
	pBottomImage->SetClipRect( 0, 238, 96, 256 );
	InsertChildObject( pBottomImage );

	//왼쪽막대기
	_XImageStatic* pLeftEdgeImage = new _XImageStatic;
	pLeftEdgeImage->Create( 0, 18, 2, m_WindowSize.cy-36,
							 &g_MainInterfaceTextureArchive, 
							 g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName));	
	pLeftEdgeImage->SetScale( 1.0f, 1.2f);
	pLeftEdgeImage->SetClipRect( 254, 105, 256, 256 );
	InsertChildObject( pLeftEdgeImage  );

	//클로즈 버튼
	_XBTN_STRUCTURE closebtnstruct = 
	{ 
		TRUE,
		{ 137, 2 }, 
		{ 12, 12 }, 
		_XDEF_PK_TRACINGSERVICES_BTN_CLOSE,
		g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName),
		g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName),
		g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName), 
		&g_MainInterfaceTextureArchive };

	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 96,  238, 107, 249 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 119, 249 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 131, 249 );
	InsertChildObject( pClosebutton  );

	int scrollbarindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );

	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,								// 윈도우 활성화
		{ 2,   36 },						// 윈도우 좌표
		{ 152, 154 },						// 윈도우 사이즈
		_XDEF_PK_TRACINGSERVICES_LISTBOX,	// 윈도우 아이디
		-1,									// 윈도우 border 이미지 인덱스
		&g_MainInterfaceTextureArchive,		// Texture 리스트
		_T(" "),							// 윈도우 타이틀
		36,									// 리스트 아이템 라인 피치
		4,									// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,			// 스크롤바의 위치
		
		// Scroll bar property
		{12, 156},
		12, 
		22,
		150,
		scrollbarindex,
		scrollbarindex,
		scrollbarindex,
		scrollbarindex,
		scrollbarindex
	};

	m_pListBox = new _XListBox;
	m_pListBox->Create(listboxstructure);

	_XVScrollBar* pScrollBar = m_pListBox->GetScrollBarObject();
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
	m_pListBox->SetAlwaysShowScrollBarMode(TRUE);

	POINT columnoffset0 = {8, 4};
	POINT columnoffset1 = {26, 4};
	POINT columnoffset2 = {8, 18};

	m_pListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_pListBox->SetColumnOffset(0, columnoffset0);
	m_pListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);

	m_pListBox->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_pListBox->SetColumnOffset(1, columnoffset1);
	m_pListBox->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);

	m_pListBox->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	m_pListBox->SetColumnOffset(2, columnoffset2);
	m_pListBox->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);

	m_pListBox->SetColumnWidth(0, 15);
	m_pListBox->SetColumnWidth(1, 120);
	m_pListBox->SetColumnWidth(2, 140);

	InsertChildObject(m_pListBox);

	return TRUE;
}

void XWindow_PK_ListBox::DestroyWindow()
{
	_XWindow::DestroyWindow();

	m_pListBox = NULL;
	m_nLapsed = 0;
	m_nSelectedItem = 0;
}

void XWindow_PK_ListBox::ShowWindow( BOOL show )
{	
	if( !show )
	{
		g_pMiniTooltip_Window->ShowWindow(show);
		m_dwUpdatedTime = 0;
	}
	else
	{
		UpdateLeftTime();
	}

	_XWindow::ShowWindow( show );
}

void XWindow_PK_ListBox::MoveWindow( int X, int Y )
{			
	// Clipping from screen size	
	_XWindow::MoveWindow( X, Y );
}

void XWindow_PK_ListBox::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return;

	_XWindow::Draw(pfocusobject);

	g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	g_XBaseFont->SetBoldMode(TRUE);
	g_XBaseFont->PutsAlign( m_WindowPosition.x + 76, m_WindowPosition.y + 18, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3456));	// 위치 추적 목록
	g_XBaseFont->SetBoldMode(FALSE);
	g_XBaseFont->Flush();
}

BOOL XWindow_PK_ListBox::Process( _XGUIObject*& pfocusobject )
{	
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	// TODO
	if((g_LocalSystemTime - m_dwUpdatedTime > 5000) && m_dwUpdatedTime > 0)
	{
		// 5초에 한번씩 시간 업데이트
		UpdateLeftTime();
	}

	MouseState* mousestate = gpInput->GetMouseState();
	
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if(!CheckMousePosition())
				{
					g_pMiniTooltip_Window->ShowWindow(FALSE);
				}
			}
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void XWindow_PK_ListBox::SetListItem()
{
	m_pListBox->DeleteAllItem();
	m_pListBox->SetScrollPos(0);

	int nCount = 0;
	int nTempHourTime = 0;
	int nTempMinTime = 0;
	int nTempSecTime = 0;

	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(messagestring));

	list<_XPKTRACING_INFO*>::iterator iter = g_pLocalUser->m_listPKTracing.begin();

	for(; iter != g_pLocalUser->m_listPKTracing.end(); ++iter )
	{
		_XPKTRACING_INFO* pPKTracingItem = (*iter);
		if( pPKTracingItem->ucPKType == 2 || pPKTracingItem->ucPKType == 3)
		{
			m_pListBox->InsertListItem(_T(""), 0, nCount+1);

			m_pListBox->SetItemText(nCount, 0, _XGETINTERFACETEXT(ID_STRING_NEW_3457));		// _T("追")

			if(pPKTracingItem->ucPKType == 2)
			{
				m_pListBox->SetItemAttrib(nCount, 0, D3DCOLOR_ARGB(255, 255, 0, 0));
				m_pListBox->SetItemAttribHighlight(nCount, 0, D3DCOLOR_ARGB(255, 255, 0, 0));
			}
			else
			{
				m_pListBox->SetItemAttrib(nCount, 0, D3DCOLOR_ARGB(255, 255, 255, 0));
				m_pListBox->SetItemAttribHighlight(nCount, 0, D3DCOLOR_ARGB(255, 255, 255, 0));
			}

			m_pListBox->SetItemText(nCount, 1, pPKTracingItem->szCharacName);
			m_pListBox->SetItemAttrib(nCount, 1, D3DCOLOR_ARGB(255, 180, 180, 180));
			m_pListBox->SetItemAttribHighlight(nCount, 1, D3DCOLOR_ARGB(255, 255, 255, 255));

			// 시간 검사
			m_nLapsed = g_ServerTimeCode - pPKTracingItem->nStartTrackingTime;

			if(m_nLapsed < 0)
				m_nLapsed = 0;
			
			switch( pPKTracingItem->ucTimeMode )
			{
			case 1: // 1시간
				{
					m_nLapsed = 3600 -	m_nLapsed;
				}
				break;
			case 2: // 24시간
				{
					m_nLapsed = (3600*24) - m_nLapsed;
				}
				break;
			case 3: // 48시간
				{
					m_nLapsed = (3600*48) - m_nLapsed;
				}
				break;
			}

			nTempHourTime = m_nLapsed/3600;
			nTempMinTime = (m_nLapsed%3600) / 60;
			nTempSecTime = (m_nLapsed%3600) % 60;

			if( nTempHourTime > 0)
				sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2224), nTempHourTime, nTempMinTime );//남은시간 : %d시간 %d분
			else if( nTempMinTime > 0)
				sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), nTempMinTime );//남은시간 : %d분
			else	
				sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), nTempSecTime );//남은시간 : %d초

			m_pListBox->SetItemText(nCount, 2, messagestring);
			m_pListBox->SetItemAttrib(nCount, 2, D3DCOLOR_ARGB(255, 255, 204, 102));
			m_pListBox->SetItemAttribHighlight(nCount, 2, D3DCOLOR_ARGB(255, 255, 204, 102));

			nCount++;
		}
	}

	m_dwUpdatedTime = g_LocalSystemTime;
}

void XWindow_PK_ListBox::UpdateLeftTime()
{
	int nCount = 0;

	int nTempHourTime = 0;
	int nTempMinTime = 0;
	int nTempSecTime = 0;

	m_dwUpdatedTime = g_LocalSystemTime;

	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(messagestring));

	list<_XPKTRACING_INFO*>::iterator iter = g_pLocalUser->m_listPKTracing.begin();

	for(; iter != g_pLocalUser->m_listPKTracing.end(); ++iter )
	{
		if( (*iter)->ucPKType == 2 || (*iter)->ucPKType == 3)
		{
			// 시간 검사
			m_nLapsed = g_ServerTimeCode - (*iter)->nStartTrackingTime;

			if(m_nLapsed < 0)
				m_nLapsed = 0;
			
			switch( (*iter)->ucTimeMode )
			{
			case 1: // 1시간
				{
					m_nLapsed = 3600 -	m_nLapsed;
				}
				break;
			case 2: // 24시간
				{
					m_nLapsed = (3600*24) - m_nLapsed;
				}
				break;
			case 3: // 48시간
				{
					m_nLapsed = (3600*48) - m_nLapsed;
				}
				break;
			}

			nTempHourTime = m_nLapsed/3600;
			nTempMinTime = (m_nLapsed%3600) / 60;
			nTempSecTime = (m_nLapsed%3600) % 60;

			if( nTempHourTime > 0)
				sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2224), nTempHourTime, nTempMinTime );//남은시간 : %d시간 %d분
			else if( nTempMinTime > 0)
				sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2225), nTempMinTime );//남은시간 : %d분
			else	
				sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2226), nTempSecTime );//남은시간 : %d초

			m_pListBox->SetItemText(nCount, 2, messagestring);
			m_pListBox->SetItemAttrib(nCount, 2, D3DCOLOR_ARGB(255, 255, 204, 102));
			m_pListBox->SetItemAttribHighlight(nCount, 2, D3DCOLOR_ARGB(255, 255, 204, 102));
			nCount++;
		}
	}
}

void XWindow_PK_ListBox::DeleteList()
{
	m_pListBox->DeleteAllItem();
	m_pListBox->SetScrollPos(0);
}