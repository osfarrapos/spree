// XWindow_NPCSearch.cpp: implementation of the _XWindow_NPCSearch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_NPCSearch.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_NPCSearch::_XWindow_NPCSearch()
{
	m_listboxNPCJob = NULL;

}

_XWindow_NPCSearch::~_XWindow_NPCSearch()
{

}

BOOL _XWindow_NPCSearch::Initialize( void )
{	
	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	int mainframeinterface = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	
	// 선택창 타이틀 바
	_XImageStatic* pTitleBar1 = new _XImageStatic;
	pTitleBar1->Create(0, 0, 109, 17, &g_MainInterfaceTextureArchive, charinfointerface);
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		pTitleBar1->SetClipRect(0, 206, 109+42, 223);
	}
	else
	{
		pTitleBar1->SetClipRect(0, 206, 109, 223);
	}
	InsertChildObject( pTitleBar1 );

	_XImageStatic* pTitleBar2 = new _XImageStatic;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		pTitleBar2->Create(109+42, 0, 112, 17, &g_MainInterfaceTextureArchive, charinfointerface);
	}
	else
	{
		pTitleBar2->Create(109, 0, 112, 17, &g_MainInterfaceTextureArchive, charinfointerface);
	}
	pTitleBar2->SetClipRect(151, 206, 154, 223);
	InsertChildObject( pTitleBar2 );
	
	// 왼쪽 끝 ( 2 X 181 )
	_XImageStatic* pLeftBar = new _XImageStatic;
	pLeftBar->Create( 0, 17, 3, 198, &g_MainInterfaceTextureArchive, mainframeinterface );
	pLeftBar->SetClipRect( 184, 64, 187, 250 );	// 길이 166	
	pLeftBar->SetScale( 1.0f, 181.0f/166.0f); 			
	InsertChildObject( pLeftBar );

	// 오른쪽 끝 ( 2 X 181 )
	_XImageStatic* pRightBar = new _XImageStatic;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		pRightBar->Create( 109+42, 17, 112, 198, &g_MainInterfaceTextureArchive, mainframeinterface );
	}
	else
	{
		pRightBar->Create( 109, 17, 112, 198, &g_MainInterfaceTextureArchive, mainframeinterface );
	}
	pRightBar->SetClipRect( 199, 64, 202, 250 );	// 길이 166	
	pRightBar->SetScale( 1.0f, 181.0f/166.0f); 			
	InsertChildObject( pRightBar );
	
	// 바닥 
	_XImageStatic* pBottomBar1 = new _XImageStatic;
	pBottomBar1->Create( 0, 198, 109, 201, &g_MainInterfaceTextureArchive, mainframeinterface );
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		pBottomBar1->SetClipRect( 0, 227, 109+42, 230 );
	}
	else
	{
		pBottomBar1->SetClipRect( 0, 227, 109, 230 );
	}

	InsertChildObject( pBottomBar1 );
	
	_XImageStatic* pBottomBar2 = new _XImageStatic;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		pBottomBar2->Create( 109+42, 198, 112, 201, &g_MainInterfaceTextureArchive, mainframeinterface );
	}
	else
	{
		pBottomBar2->Create( 109, 198, 112, 201, &g_MainInterfaceTextureArchive, mainframeinterface );
	}
	pBottomBar2->SetClipRect( 180, 227, 183, 230 );
	InsertChildObject( pBottomBar2 );
	
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { m_WindowSize.cx - 16, 2 }, { 12, 12 }, _XDEF_NPCSEARCH_CLOSE,
		charinfointerface,charinfointerface,charinfointerface, &g_MainInterfaceTextureArchive };
	_XButton* closebutton = new _XButton;
	closebutton->Create( closebtnstruct );		
	closebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	closebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	closebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( closebutton );
	
	
	int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	// 퀘스트에 사용될 리스트 박스
	_XLISTBOX_STRUCTURE npcjobstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 7, 31 },		// 윈도우 좌표
		{ 100, 155 },	// 윈도우 사이즈
		_XDEF_NPCSEARCH_LISTBOX,						// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		17,												// 리스트 아이템 라인 피치
		9,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 154 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		50,												// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
	};
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		npcjobstructure.windowsize.cx += 42;
	}

	m_listboxNPCJob = new _XListBox;
	m_listboxNPCJob->Create(npcjobstructure);
	
	_XVScrollBar* pScrollBar = m_listboxNPCJob->GetScrollBarObject();
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
	m_listboxNPCJob->SetAlwaysShowScrollBarMode(TRUE);
	
	POINT columnoffset1 = { 5, 3 };
	m_listboxNPCJob->SetColumnOffset(0, columnoffset1);				// column offset	
	m_listboxNPCJob->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style (문자)	
	m_listboxNPCJob->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		m_listboxNPCJob->SetColumnWidth(0, 120);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
		m_listboxNPCJob->SetTrackWidth(126);
	}
	else
	{
		m_listboxNPCJob->SetColumnWidth(0, 80);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
		m_listboxNPCJob->SetTrackWidth(80);
	}
	
	m_listboxNPCJob->ShowWindow(TRUE);

	InsertChildObject(m_listboxNPCJob);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		m_BorderGradationTitle.Create( m_WindowPosition.x + 3, m_WindowPosition.y + 17, m_WindowPosition.x + 112 + 42, m_WindowPosition.y + 27, 
			&g_MainInterfaceTextureArchive, mainframeinterface );
	}
	else
	{
		m_BorderGradationTitle.Create( m_WindowPosition.x + 3, m_WindowPosition.y + 17, m_WindowPosition.x + 112, m_WindowPosition.y + 27, 
			&g_MainInterfaceTextureArchive, mainframeinterface );		
	}

	m_BorderGradationTitle.SetClipRect( 206, 236, 244, 246 );
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		m_BorderGradationTitle.SetScale( (110.0f+42.0f)/39.0f, 1.0f );
	}
	else
	{
		m_BorderGradationTitle.SetScale( 110.0f/39.0f, 1.0f );		
	}
	
	return TRUE;
}

void _XWindow_NPCSearch::MoveWindow( int X, int Y )
{			
	_XWindow::MoveWindow( X, Y );
	m_BorderGradationTitle.MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + 17 );		
}


void _XWindow_NPCSearch::Draw( _XGUIObject*& pfocusobject )
{	
	
	if(this->m_ShowWindow)
	{
		_XDrawSolidBar( m_WindowPosition.x+2, m_WindowPosition.y+12,
						m_WindowPosition.x+m_WindowSize.cx-2, m_WindowPosition.y+ m_WindowSize.cy - 10, D3DCOLOR_ARGB(164,0,0,0));	
		m_BorderGradationTitle.Draw( pfocusobject );
	}
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
	
	POINT listwinpos;
	SIZE listwinsize;
	
	listwinpos = m_listboxNPCJob->GetWindowPos();
	listwinsize = m_listboxNPCJob->GetWindowSize();
	
	_XDrawSolidBar( listwinpos.x, listwinpos.y-1,
		listwinpos.x+listwinsize.cx - 13, listwinpos.y+listwinsize.cy-1 , D3DCOLOR_ARGB(102,0,0,0));
	_XDrawRectAngle( listwinpos.x, listwinpos.y-1,
		listwinpos.x+listwinsize.cx- 13, listwinpos.y+listwinsize.cy-1, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( listwinpos.x+1, listwinpos.y,
		listwinpos.x+listwinsize.cx - 14, listwinpos.y+listwinsize.cy-2, 0.0f, D3DCOLOR_ARGB(191,0,0,0) );
	_XDrawRectAngle( listwinpos.x+2, listwinpos.y+1,
		listwinpos.x+listwinsize.cx - 15, listwinpos.y+listwinsize.cy-3, 0.0f, D3DCOLOR_ARGB(63,0,0,0) );
	
	m_listboxNPCJob->Draw(pfocusobject);		
}

void _XWindow_NPCSearch::ShowWindow(BOOL show)
{
	if(show)
	{
		m_listboxNPCJob->InsertListItem(_T(""), 0, 1);	
		m_listboxNPCJob->SetItemText(0, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2692));
		m_listboxNPCJob->SetItemAttrib(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_listboxNPCJob->InsertListItem(_T(""), 0, 2);	
		m_listboxNPCJob->SetItemText(1, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2693));
		m_listboxNPCJob->SetItemAttrib(1, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(1, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_listboxNPCJob->InsertListItem(_T(""), 0, 3);	
		m_listboxNPCJob->SetItemText(2, 0,_XGETINTERFACETEXT(ID_STRING_NEW_2694));
		m_listboxNPCJob->SetItemAttrib(2, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(2, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_listboxNPCJob->InsertListItem(_T(""), 0, 4);
		m_listboxNPCJob->SetItemText(3, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2695));	
		m_listboxNPCJob->SetItemAttrib(3, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(3, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_listboxNPCJob->InsertListItem(_T(""), 0, 5);	
		m_listboxNPCJob->SetItemText(4, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2696));
		m_listboxNPCJob->SetItemAttrib(4, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(4, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_listboxNPCJob->InsertListItem(_T(""), 0, 6);	
		m_listboxNPCJob->SetItemText(5, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2697));
		m_listboxNPCJob->SetItemAttrib(5, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(5, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_listboxNPCJob->InsertListItem(_T(""), 0, 7);	
		m_listboxNPCJob->SetItemText(6, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2698));
		m_listboxNPCJob->SetItemAttrib(6, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(6, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		m_listboxNPCJob->InsertListItem(_T(""), 0, 8);	
		m_listboxNPCJob->SetItemText(7, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2699));
		m_listboxNPCJob->SetItemAttrib(7, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(7, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		// 암상인은 찾기 리스트에서 제외한다.
//		m_listboxNPCJob->InsertListItem(_T(""), 0, 9);	
//		m_listboxNPCJob->SetItemText(8, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2700));
//		m_listboxNPCJob->SetItemAttrib(8, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
//		m_listboxNPCJob->SetItemAttribHighlight(8, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->InsertListItem(_T(""), 0, 9);	
		m_listboxNPCJob->SetItemText(8, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2701));
		m_listboxNPCJob->SetItemAttrib(8, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(8, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		m_listboxNPCJob->InsertListItem(_T(""), 0, 10);	
		m_listboxNPCJob->SetItemText(9, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2702));
		m_listboxNPCJob->SetItemAttrib(9, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(9, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		m_listboxNPCJob->InsertListItem(_T(""), 0, 11);	
		m_listboxNPCJob->SetItemText(10, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2703));
		m_listboxNPCJob->SetItemAttrib(10, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(10, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		m_listboxNPCJob->InsertListItem(_T(""), 0, 12);	
		m_listboxNPCJob->SetItemText(11, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2704));
		m_listboxNPCJob->SetItemAttrib(11, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(11, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		m_listboxNPCJob->InsertListItem(_T(""), 0, 13);	
		m_listboxNPCJob->SetItemText(12, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2705));
		m_listboxNPCJob->SetItemAttrib(12, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(12, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		m_listboxNPCJob->InsertListItem(_T(""), 0, 14);	
		m_listboxNPCJob->SetItemText(13, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2706));
		m_listboxNPCJob->SetItemAttrib(13, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(13, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		m_listboxNPCJob->InsertListItem(_T(""), 0, 15);	
		m_listboxNPCJob->SetItemText(14, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2707));
		m_listboxNPCJob->SetItemAttrib(14, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_listboxNPCJob->SetItemAttribHighlight(14, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		
//		m_listboxNPCJob->InsertListItem(_T(""), 0, 16);	
//		m_listboxNPCJob->SetItemText(15, 0, _XGETINTERFACETEXT(ID_STRING_NEW_2707));
//		m_listboxNPCJob->SetItemAttrib(15, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
//		m_listboxNPCJob->SetItemAttribHighlight(15, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		
	}
	else
	{
		m_listboxNPCJob->DeleteAllItem();
	}
	
	_XWindow::ShowWindow(show);
}