// XWindow_WebBoard.cpp: implementation of the _XWindow_WebBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XSR_STRINGHEADER.H"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_WebBoard.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_WebBoard::_XWindow_WebBoard()
{

}

_XWindow_WebBoard::~_XWindow_WebBoard()
{
	DestroyWindow();
}

BOOL _XWindow_WebBoard::Initialize(void)
{
	// CashMall Size ( 692 X 504 )
	int webboardresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Cash_01.tga" );

	int startpositionx = (gnWidth>>1)-346;
	int startpositiony = (gnHeight>>1)-311;
	// 타이틀 바1	( 90 X 31 )
	_XImageStatic* pTitleBar1 = new _XImageStatic;
	pTitleBar1->Create( startpositionx+0, startpositiony+0, startpositionx+90, startpositiony+31, &g_MainInterfaceTextureArchive, webboardresourceindex1  );
	pTitleBar1->SetClipRect(3, 84, 93, 115);
	pTitleBar1->ShowWindow(TRUE);
	InsertChildObject( pTitleBar1 );

	// 타이틀 바2	( 512 X 31 )
	_XImageStatic* pTitleBar2 = new _XImageStatic;
	pTitleBar2->Create( startpositionx+90, startpositiony+0, startpositionx+602, startpositiony+31, &g_MainInterfaceTextureArchive, webboardresourceindex1  );
	pTitleBar2->SetClipRect(93, 84, 103, 115);
	pTitleBar2->SetScale( 51.2f, 1.0f);
	pTitleBar2->ShowWindow(TRUE);
	InsertChildObject( pTitleBar2 );
	
	// 타이틀 바3	( 92 X 31 )
	_XImageStatic* pTitleBar3 = new _XImageStatic;
	pTitleBar3->Create( startpositionx+602, startpositiony+0, startpositionx+694, startpositiony+31, &g_MainInterfaceTextureArchive, webboardresourceindex1  );
	pTitleBar3->SetClipRect(103, 84, 195, 115);
	pTitleBar3->ShowWindow(TRUE);
	InsertChildObject( pTitleBar3 );
	
	// 왼쪽 사이드 바1	( 3 X 450 )
	_XImageStatic* pLeftSideBar1 = new _XImageStatic;
	pLeftSideBar1->Create( startpositionx+0, startpositiony+31, startpositionx+3, startpositiony+481, &g_MainInterfaceTextureArchive, webboardresourceindex1  );
	pLeftSideBar1->SetClipRect(2, 186, 5, 196);
	pLeftSideBar1->SetScale( 1.0f, 45.0f );
	pLeftSideBar1->ShowWindow(TRUE);
	InsertChildObject( pLeftSideBar1 );
		
	// 오른쪽 사이드 바1	( 3 X 450 )
	_XImageStatic* pRightSideBar1 = new _XImageStatic;
	pRightSideBar1->Create( startpositionx+694-3, startpositiony+31, startpositionx+694, startpositiony+481, &g_MainInterfaceTextureArchive, webboardresourceindex1  );
	pRightSideBar1->SetClipRect( 13, 186, 16, 196);
	pRightSideBar1->SetScale( 1.0f, 45.0f );
	pRightSideBar1->ShowWindow(TRUE);
	InsertChildObject( pRightSideBar1 );
		
	// 바닥 1 ( 3 X 21 )
	_XImageStatic* pBottomBar1 = new _XImageStatic;
	pBottomBar1->Create( startpositionx+0, startpositiony+481, startpositionx+3, startpositiony+502, &g_MainInterfaceTextureArchive, webboardresourceindex1  );
	pBottomBar1->SetClipRect(81, 123, 84, 144);
	pBottomBar1->ShowWindow(TRUE);
	InsertChildObject( pBottomBar1 );
	
	// 바닥 2 ( 688 X 21 )
	_XImageStatic* pBottomBar2 = new _XImageStatic;
	pBottomBar2->Create( startpositionx+3, startpositiony+481, startpositionx+691, startpositiony+502, &g_MainInterfaceTextureArchive, webboardresourceindex1 );
	pBottomBar2->SetClipRect(84, 123, 174, 144 );
	pBottomBar2->SetScale( 7.6444f, 1.0f );
	pBottomBar2->ShowWindow(TRUE);
	InsertChildObject( pBottomBar2 );
	
	// 바닥 3 ( 3 X 21 )
	_XImageStatic* pBottomBar3 = new _XImageStatic;
	pBottomBar3->Create( startpositionx+694-3, startpositiony+481, startpositionx+694, startpositiony+502, &g_MainInterfaceTextureArchive, webboardresourceindex1  );
	pBottomBar3->SetClipRect(174, 123, 177, 144);
	pBottomBar3->ShowWindow(TRUE);
	InsertChildObject( pBottomBar3 );

	
	//닫기 버튼 ( 72 X 16 ) 
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { startpositionx+306, startpositiony+484 }, { 72, 16 }, _XDEF_WEBBOARD_BTN_CLOSE ,
		webboardresourceindex1 ,webboardresourceindex1 ,webboardresourceindex1 , 
		&g_MainInterfaceTextureArchive };
	
	_XButton* pCloseButton = new _XButton;
	pCloseButton->Create( closebtnstruct );		
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 121, 74, 137 );
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 141, 74, 157 );
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 161, 74, 177 );
	pCloseButton->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	pCloseButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_OPTION_CLOSE );
	pCloseButton->ShowWindow(TRUE);
	InsertChildObject( pCloseButton );


	return TRUE;
}

void _XWindow_WebBoard::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_WebBoard::Draw(_XGUIObject*& pfocusobject)
{
	
	_XWindow::Draw(pfocusobject);	
	
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
	
	g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	g_XBaseFont->SetBoldMode(TRUE);
	g_XBaseFont->PrintAlign( m_WindowPosition.x + (m_WindowSize.cx>>1) - 8, (gnHeight>>1)-311 + 10, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3168));
	g_XBaseFont->Flush();
	g_XBaseFont->SetBoldMode(FALSE);

}

BOOL _XWindow_WebBoard::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_WebBoard::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
}

void _XWindow_WebBoard::ShowWindow(BOOL show)
{	
	_XWindow::ShowWindow(show);
}
