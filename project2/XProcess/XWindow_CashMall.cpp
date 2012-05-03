// XWindow_CashMall.cpp: implementation of the _XWindow_CashMall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_CashMall.h"
#include "XSR_STRINGHEADER.H"

// win32 web browser api
#include "./Library/CWebPage/XWebPage.h"

#ifdef _XTS_ITEMMALLBROWSER
_XWindow_CashMall*		g_pCashMall_Window = NULL;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_CashMall::_XWindow_CashMall()
{

}

_XWindow_CashMall::~_XWindow_CashMall()
{
	DestroyWindow();

}


BOOL _XWindow_CashMall::Initialize(void)
{
	// CashMall Size ( 820 X 700 )
	int cashmallresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Cash_Shop1.tga" );
	int cashmallresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_Cash_Shop2.tga" );
	
	int startpositionx = (gnWidth>>1)-410;
	int startpositiony = (gnHeight>>1)-350;

	// 타이틀 바1	( 202 X 50 )
	_XImageStatic* pTitleBar1 = new _XImageStatic;
	pTitleBar1->Create( startpositionx, startpositiony, startpositionx+202, startpositiony+50, &g_MainInterfaceTextureArchive, cashmallresourceindex1 );
	pTitleBar1->SetClipRect(1, 1, 203, 51);
	pTitleBar1->ShowWindow(TRUE);
	InsertChildObject( pTitleBar1 );

	// 타이틀 바2	( 208 X 70 )
	_XImageStatic* pTitleBar2 = new _XImageStatic;
	pTitleBar2->Create( startpositionx+202, startpositiony-20, startpositionx+410, startpositiony+50, &g_MainInterfaceTextureArchive, cashmallresourceindex1 );
	pTitleBar2->SetClipRect(1, 53, 209, 123);
	pTitleBar2->ShowWindow(TRUE);
	InsertChildObject( pTitleBar2 );
	
	// 타이틀 바3	( 201 X 70 )
	_XImageStatic* pTitleBar3 = new _XImageStatic;
	pTitleBar3->Create( startpositionx+410, startpositiony-20, startpositionx+611, startpositiony+50, &g_MainInterfaceTextureArchive, cashmallresourceindex1 );
	pTitleBar3->SetClipRect(1, 129, 202, 199);
	pTitleBar3->ShowWindow(TRUE);
	InsertChildObject( pTitleBar3 );
	
	// 타이틀 바4	( 209 X 50 )
	_XImageStatic* pTitleBar4 = new _XImageStatic;
	pTitleBar4->Create( startpositionx+611, startpositiony, startpositionx+820, startpositiony+50, &g_MainInterfaceTextureArchive, cashmallresourceindex1 );
	pTitleBar4->SetClipRect(1, 205, 210, 255);
	pTitleBar4->ShowWindow(TRUE);
	InsertChildObject( pTitleBar4 );
	
	// 왼쪽 사이드 바1	( 10 X 244 )
	_XImageStatic* pLeftSideBar1 = new _XImageStatic;
	pLeftSideBar1->Create( startpositionx, startpositiony+50, startpositionx+10, startpositiony+50+244, &g_MainInterfaceTextureArchive, cashmallresourceindex2 );
	pLeftSideBar1->SetClipRect(1, 6, 11, 250);
	pLeftSideBar1->ShowWindow(TRUE);
	InsertChildObject( pLeftSideBar1 );
	
	// 왼쪽 사이드 바2	( 10 X 244 )
	_XImageStatic* pLeftSideBar2 = new _XImageStatic;
	pLeftSideBar2->Create( startpositionx, startpositiony+50+244, startpositionx+10, startpositiony+50+244+244, &g_MainInterfaceTextureArchive, cashmallresourceindex2 );
	pLeftSideBar2->SetClipRect(16, 6, 26, 250);
	pLeftSideBar2->ShowWindow(TRUE);
	InsertChildObject( pLeftSideBar2 );
	
	// 왼쪽 사이드 바3	( 10 X 112 )
	_XImageStatic* pLeftSideBar3 = new _XImageStatic;
	pLeftSideBar3->Create( startpositionx, startpositiony+50+244+244, startpositionx+10, startpositiony+50+244+244+112, &g_MainInterfaceTextureArchive, cashmallresourceindex2 );
	pLeftSideBar3->SetClipRect(31, 6, 41, 250);
	pLeftSideBar3->ShowWindow(TRUE);
	InsertChildObject( pLeftSideBar3 );
	
	// 오른쪽 사이드 바1	( 10 X 245 )
	_XImageStatic* pRightSideBar1 = new _XImageStatic;
	pRightSideBar1->Create( startpositionx+810, startpositiony+50, startpositionx+820, startpositiony+50+245, &g_MainInterfaceTextureArchive, cashmallresourceindex1 );
	pRightSideBar1->SetClipRect(216, 6, 226, 251);
	pRightSideBar1->ShowWindow(TRUE);
	InsertChildObject( pRightSideBar1 );
	
	// 오른쪽 사이드 바2	( 10 X 244 )
	_XImageStatic* pRightSideBar2 = new _XImageStatic;
	pRightSideBar2->Create( startpositionx+810, startpositiony+50+245, startpositionx+820, startpositiony+50+245+244, &g_MainInterfaceTextureArchive, cashmallresourceindex1 );
	pRightSideBar2->SetClipRect(232, 7, 242, 251);
	pRightSideBar2->ShowWindow(TRUE);
	InsertChildObject( pRightSideBar2 );
	
	// 오른쪽 사이드 바3	( 10 X 111 )
	_XImageStatic* pRightSideBar3 = new _XImageStatic;
	pRightSideBar3->Create( startpositionx+810, startpositiony+50+245+244, startpositionx+820, startpositiony+50+245+244+111, &g_MainInterfaceTextureArchive, cashmallresourceindex1 );
	pRightSideBar3->SetClipRect(246, 12, 256, 123);
	pRightSideBar3->ShowWindow(TRUE);
	InsertChildObject( pRightSideBar3 );
	
	// 바닥 1 ( 203 X 30 )
	_XImageStatic* pBottomBar1 = new _XImageStatic;
	pBottomBar1->Create( startpositionx, startpositiony+650, startpositionx+203, startpositiony+680, &g_MainInterfaceTextureArchive, cashmallresourceindex2 );
	pBottomBar1->SetClipRect(51, 4, 254, 34);
	pBottomBar1->ShowWindow(TRUE);
	InsertChildObject( pBottomBar1 );
	
	// 바닥 2 ( 207 X 30 )
	_XImageStatic* pBottomBar2 = new _XImageStatic;
	pBottomBar2->Create( startpositionx+203, startpositiony+650, startpositionx+410, startpositiony+680, &g_MainInterfaceTextureArchive, cashmallresourceindex2 );
	pBottomBar2->SetClipRect(47, 39, 254, 69);
	pBottomBar2->ShowWindow(TRUE);
	InsertChildObject( pBottomBar2 );
	
	// 바닥 3 ( 202 X 30 )
	_XImageStatic* pBottomBar3 = new _XImageStatic;
	pBottomBar3->Create( startpositionx+410, startpositiony+650, startpositionx+612, startpositiony+680, &g_MainInterfaceTextureArchive, cashmallresourceindex2 );
	pBottomBar3->SetClipRect(52, 74, 254, 104);
	pBottomBar3->ShowWindow(TRUE);
	InsertChildObject( pBottomBar3 );

	// 바닥 4 ( 208 X 30 )
	_XImageStatic* pBottomBar4 = new _XImageStatic;
	pBottomBar4->Create( startpositionx+612, startpositiony+650, startpositionx+820, startpositiony+680, &g_MainInterfaceTextureArchive, cashmallresourceindex2 );
	pBottomBar4->SetClipRect(46, 110, 254, 140);
	pBottomBar4->ShowWindow(TRUE);
	InsertChildObject( pBottomBar4 );
	
	//닫기 버튼 ( 91 X 21 ) 
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { startpositionx+365, startpositiony+655 }, { 91, 21 }, _XDEF_CASHMALL_BTN_CLOSE ,
		cashmallresourceindex2,cashmallresourceindex2,cashmallresourceindex2, 
		&g_MainInterfaceTextureArchive };
	
	_XButton* pCloseButton = new _XButton;
	pCloseButton->Create( closebtnstruct );		
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 164, 188, 255, 209 );
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  164, 211, 255, 232 );
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  164, 234, 255, 255 );
	pCloseButton->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	pCloseButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_OPTION_CLOSE );
	pCloseButton->ShowWindow(TRUE);
	InsertChildObject( pCloseButton );


	return TRUE;
}

void _XWindow_CashMall::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_CashMall::Draw(_XGUIObject*& pfocusobject)
{
	
	_XWindow::Draw(pfocusobject);	
	
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
	
	g_XLargeFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	g_XLargeFont->PrintAlign( m_WindowPosition.x + (m_WindowSize.cx>>1) - 8, (gnHeight>>1)-340, 1.0f, _XFONT_ALIGNTYPE_CENTER,_XGETINTERFACETEXT(ID_STRING_NEW_2977));// _T("캐시 아이템 상점")
	g_XLargeFont->Flush();

}

BOOL _XWindow_CashMall::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_CashMall::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
}

void _XWindow_CashMall::ShowWindow(BOOL show)
{		
	if( show )
	{
#ifdef _XDEF_ITEMMALL_AUTOLOGIN_20061120
//		int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
//		
//		TCHAR tempStr[256];
//		memset( tempStr, 0, sizeof(TCHAR)*256 );
//		sprintf( tempStr, "http://panda.nexon.com/?user_id=%s&select_server=%d", g_pLocalUser->m_ItemMallWebLoginID, servernumber );
//							
//		DisplayHTMLPage(g_hItemMallWebBrowserObject, tempStr);
#else
		
#ifdef _XKOREAN

	#ifdef _XDEF_ITEMMALL_AUTOLOGIN_SE_20070514		
		int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
		TCHAR postdata[128];		
		sprintf( postdata, "AL_ID=%s&AL_PWD=%s&AL_Server=%d", g_pLocalUser->m_ItemMallWebLoginID, g_pLocalUser->m_UserPW, servernumber );
		_XDisplayHTMLPageAndPostData(g_hItemMallWebBrowserObject, _T("http://global1.indy21.co.kr:8080"), postdata );		
	#else
		// For this window, display a URL. This could also be a HTML file on disk such as "c:\\myfile.htm".
		if( !g_bTestWebPageAddress )
			DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ninemall.nexon.com/");
		else
			DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://panda.nexon.com");
	#endif

#endif
		
#ifdef _XENGLISH 
	#ifdef _XDEF_ITEMMALL_AUTOLOGIN_SE_20070514		
		int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
		TCHAR postdata[128];		
		sprintf( postdata, "AL_ID=%s&AL_PWD=%s&AL_Server=%d", g_pLocalUser->m_ItemMallWebLoginID, g_pLocalUser->m_UserPW, servernumber );
		_XDisplayHTMLPageAndPostData(g_hItemMallWebBrowserObject, _T("http://global1.indy21.co.kr:8080"), postdata );		
	#else
		DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://9dragons.acclaim.com/store/store.jsp");
	#endif
#endif
		
#ifdef _XVIETNAMESE
	#ifdef _XDEF_ITEMMALL_AUTOLOGIN_SE_20070514		
		int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
		TCHAR postdata[128];		
		sprintf( postdata, "AL_ID=%s&AL_PWD=%s&AL_Server=%d", g_pLocalUser->m_ItemMallWebLoginID, g_pLocalUser->m_UserPW, servernumber );
		_XDisplayHTMLPageAndPostData(g_hItemMallWebBrowserObject, _T("http://global1.indy21.co.kr:8080"), postdata );		
	#else
		//DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ktc.cuulong.com.vn");
		//DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://www.cuulong.com.vn/ktc");
		DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://cuulong.id.zing.vn/ktc/");
	#endif
#endif
			
#ifdef _XTAIWANESE
#endif
#endif
	}
	else
	{
		
		// 베트남만 아이템몰 창을 닫을 때 로그오프 한다.
		// 창을 다시 열면 다시 로그인 해야 한다.
#ifdef _XVIETNAMESE
/*	if( g_hItemMallWebBrowserObject )
	{
		//DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://www.cuulong.com.vn/ktc/logout.aspx");	
		DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://cuulongid.zing.vn/ktc/logout.aspx");
	}
*/
#endif
		
	}
	_XWindow::ShowWindow(show);
}
