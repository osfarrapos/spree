// XWindow_CashBox.cpp: implementation of the _XWindow_CashBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_CashBox.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL __stdcall _CashItemPackageListBoxItemMouseHoverCallback (int param1, int param2, int XPos, int YPos )
{
#ifdef _XTS_ITEMMALLBROWSER
	if( param2 == -1 )
	{
		
	}
	else
	{
		_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
		if( !pCashBox_Window ) return TRUE;

		if( pCashBox_Window->m_bShowPresentMessage ) return TRUE;
		
		_XCASHITEMBOX_INFO* cashitem = NULL;
		list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
		for(iter_cashitem = pCashBox_Window->m_listCashItem.begin() ; iter_cashitem != pCashBox_Window->m_listCashItem.end() ; ++iter_cashitem)
		{
			cashitem = *iter_cashitem;
			if( cashitem )
			{
				if( (cashitem->indexBox-1) == param2 )
				{					
					_XCashItemPackage* currentcashitempackage = NULL;
					list <_XCashItemPackage*>::iterator iter_cashitempackage;
					for(iter_cashitempackage = g_CashItemPackageManger.m_listCashItemPackage.begin() ; iter_cashitempackage != g_CashItemPackageManger.m_listCashItemPackage.end() ; ++iter_cashitempackage)
					{
						currentcashitempackage = *iter_cashitempackage;
						if( currentcashitempackage )
						{
							if( currentcashitempackage->m_indexCashItemPackage == cashitem->indexPackage )
							{
								
								ScrnPos* scrnpos	= gpInput->GetMouseVirScreenPos();
								int count = g_XBaseFont->GetRowCountNewLineVersion(currentcashitempackage->m_PackageDiscription,250);
								_XDrawSolidBar( scrnpos->x +34, scrnpos->z+4, scrnpos->x+284, scrnpos->z+4 + (count*15) , D3DCOLOR_ARGB(255,0,0,0) );
								_XDrawRectAngle( scrnpos->x+33, scrnpos->z+3, scrnpos->x+285, scrnpos->z+5 + (count*15) , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );
								
								g_XBaseFont->SetColor(_XSC_DEFAULT);		
								
								g_XBaseFont->Puts_SeparateNewLineVersion( scrnpos->x+36, scrnpos->z+6, currentcashitempackage->m_PackageDiscription, 250, 1.0, 3 );

								break;
							}
						}
					}
				}
			}
		}

	}
#endif
	return TRUE;
}

#ifdef _XDEF_COMMONITEMMALL_20070402
void __stdcall _XCallback_CashMalll_Normal( POINT pos, SIZE size )
{
	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3381), _XSC_INFORMATION );//
}
void __stdcall _XCallback_CashMalll_Present( POINT pos, SIZE size )
{
	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3382), _XSC_INFORMATION );//
}
#endif

_XWindow_CashBox::_XWindow_CashBox()
{
	m_listboxCashItem = NULL;
	m_SelectedTab = _XCASHBOXTAB_PERSONAL;
	m_btnMoveSelectedItem = NULL;;	//선택옮김
	m_btnMoveAllItem = NULL;;		//전부옮김
	m_btnRenew = NULL;;				//새로고침
	for( int i = 0; i < _XCASHBOXTAB_TOTALCOUNT; ++i )
	{
		m_TabButton[i] = NULL;
		m_Common_TabButton[i] = NULL;
	}
	m_nCashItemTotalCount = 0;
	
	m_btnFirst = NULL;				//처음으로 
	m_btnEnd = NULL;				//끝으로
	m_btnAdjacent = NULL;			//이전
	m_btnNext = NULL;				//다음
	
	for( i = 0; i < 5; i++ )
	{
		m_btnPage[i] = NULL;
	}
	
	m_btnPage1 = NULL;				//페이지1
	m_btnPage2 = NULL;				//페이지2
	m_btnPage3 = NULL;				//페이지3
	m_btnPage4 = NULL;				//페이지4
	m_btnPage5 = NULL;				//페이지5
	
	m_btnMsgClose = NULL;
	m_bShowPresentMessage = FALSE;
	m_PresentMessageBoxPosition = 0;
	m_Maxium5PageCount	= 0;
	m_Current5PageCount = 0;
	m_CurrentPageNumber = 0;
	
	for( i = 0; i < 9; ++i )
	{
		m_btnMessage[i] = NULL;
	}
	memset(m_PresentName, 0, sizeof(TCHAR)*128 );
	memset(m_PresentTime, 0, sizeof(TCHAR)*128 );
	memset(m_PresentMessage, 0, sizeof(TCHAR)*128 );
	

}

_XWindow_CashBox::~_XWindow_CashBox()
{
	
	_XCASHITEMBOX_INFO* cashitem = NULL;
	list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
	for(iter_cashitem = m_listCashItem.begin() ; iter_cashitem != m_listCashItem.end() ; ++iter_cashitem)
	{
		cashitem = *iter_cashitem;
		SAFE_DELETE( cashitem );
	}
	m_listCashItem.clear();
	DestroyWindow();
}

BOOL _XWindow_CashBox::Initialize(void)
{
	// Cash Box Size ( 327 X 442 )
	int cashboxresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Cash_Box1.tga" );
	int cashboxresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_Cash_Box2.tga" );
	int cashboxresourceindex3 = g_MainInterfaceTextureArchive.FindResource( "MI_Cash_Box3.tga" );
	int cashmallresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_Cash_Shop2.tga" );

	// 타이틀 바1	( 71 X 50 )
	_XImageStatic* pTitleBar1 = new _XImageStatic;
	pTitleBar1->Create( 0, 0, 71, 50, &g_MainInterfaceTextureArchive, cashboxresourceindex1 );
	pTitleBar1->SetClipRect( 0, 52, 71, 102);
	pTitleBar1->ShowWindow(TRUE);
	InsertChildObject( pTitleBar1 );
	
	// 타이틀 바2	( 256 X 50 )
	_XImageStatic* pTitleBar2 = new _XImageStatic;
	pTitleBar2->Create( 71, 0, 327, 50, &g_MainInterfaceTextureArchive, cashboxresourceindex1 );
	pTitleBar2->SetClipRect( 0, 0, 256, 50);
	pTitleBar2->ShowWindow(TRUE);
	InsertChildObject( pTitleBar2 );
	
	// 왼쪽 사이드 바1	( 10 X 163 )
	_XImageStatic* pLeftSideBar1 = new _XImageStatic;
	pLeftSideBar1->Create( 0, 50, 10, 213, &g_MainInterfaceTextureArchive, cashboxresourceindex1 );
	pLeftSideBar1->SetClipRect( 180, 54, 190, 217 );
	pLeftSideBar1->ShowWindow(TRUE);
	InsertChildObject( pLeftSideBar1 );
	
	// 왼쪽 사이드 바2	( 10 X 121 )
	_XImageStatic* pLeftSideBar2 = new _XImageStatic;
	pLeftSideBar2->Create( 0, 213, 10, 334, &g_MainInterfaceTextureArchive, cashboxresourceindex1 );
	pLeftSideBar2->SetClipRect( 193, 54, 203, 175 );
	pLeftSideBar2->ShowWindow(TRUE);
	InsertChildObject( pLeftSideBar2 );
	
	// 왼쪽 사이드 바3	( 10 X 78 )
	_XImageStatic* pLeftSideBar3 = new _XImageStatic;
	pLeftSideBar3->Create( 0, 334, 10, 412, &g_MainInterfaceTextureArchive, cashboxresourceindex1 );
	pLeftSideBar3->SetClipRect( 206, 54, 216, 132 );
	pLeftSideBar3->ShowWindow(TRUE);
	InsertChildObject( pLeftSideBar3 );
	
	// 오른쪽 사이드 바1	( 10 X 169 )
	_XImageStatic* pRightSideBar1 = new _XImageStatic;
	pRightSideBar1->Create( 317, 50, 327, 219, &g_MainInterfaceTextureArchive, cashboxresourceindex1 );
	pRightSideBar1->SetClipRect( 220, 54, 230, 223 );
	pRightSideBar1->ShowWindow(TRUE);
	InsertChildObject( pRightSideBar1 );
	
	// 오른쪽 사이드 바2	( 10 X 112 )
	_XImageStatic* pRightSideBar2 = new _XImageStatic;
	pRightSideBar2->Create( 317, 219, 327, 331, &g_MainInterfaceTextureArchive, cashboxresourceindex1 );
	pRightSideBar2->SetClipRect( 233, 54, 243, 166 );
	pRightSideBar2->ShowWindow(TRUE);
	InsertChildObject( pRightSideBar2 );
	
	// 오른쪽 사이드 바3	( 10 X 81 )
	_XImageStatic* pRightSideBar3 = new _XImageStatic;
	pRightSideBar3->Create( 317, 331, 327, 412, &g_MainInterfaceTextureArchive, cashboxresourceindex1 );
	pRightSideBar3->SetClipRect( 246, 54, 256, 135 );
	pRightSideBar3->ShowWindow(TRUE);
	InsertChildObject( pRightSideBar3 );
	
	// 바닥 1 ( 71 X 30 )
	_XImageStatic* pBottomBar1 = new _XImageStatic;
	pBottomBar1->Create( 0, 412, 71, 442, &g_MainInterfaceTextureArchive, cashboxresourceindex1 );
	pBottomBar1->SetClipRect(0, 195, 71, 225);
	pBottomBar1->ShowWindow(TRUE);
	InsertChildObject( pBottomBar1 );
	
	// 바닥 2 ( 256 X 30 )
	_XImageStatic* pBottomBar2 = new _XImageStatic;
	pBottomBar2->Create( 71, 412, 327, 442, &g_MainInterfaceTextureArchive, cashboxresourceindex1 );
	pBottomBar2->SetClipRect(0, 226, 256, 256);
	pBottomBar2->ShowWindow(TRUE);
	InsertChildObject( pBottomBar2 );
	
	//선택옮김 버튼 ( 76 X 20 ) 
#ifdef _XDEF_COMMONITEMMALL_20070402
	_XBTN_STRUCTURE movebtnstruct = { TRUE, { 240, 388 }, { 76, 20 }, _XDEF_CASHBOX_BTN_MOVE ,
		cashboxresourceindex1,cashboxresourceindex1,cashboxresourceindex1, 
		&g_MainInterfaceTextureArchive };
	
	m_btnMoveSelectedItem = new _XButton;
	m_btnMoveSelectedItem->Create( movebtnstruct );		
	m_btnMoveSelectedItem->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 107, 77, 127 );
	m_btnMoveSelectedItem->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  1, 134, 77, 154 );
	m_btnMoveSelectedItem->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 160, 77, 180 );
	m_btnMoveSelectedItem->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	m_btnMoveSelectedItem->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3383 );//행낭으로
	m_btnMoveSelectedItem->ShowWindow(TRUE);
	InsertChildObject( m_btnMoveSelectedItem );
#else
	_XBTN_STRUCTURE movebtnstruct = { TRUE, { 127, 388 }, { 76, 20 }, _XDEF_CASHBOX_BTN_MOVE ,
		cashboxresourceindex1,cashboxresourceindex1,cashboxresourceindex1, 
		&g_MainInterfaceTextureArchive };
	
	m_btnMoveSelectedItem = new _XButton;
	m_btnMoveSelectedItem->Create( movebtnstruct );		
	m_btnMoveSelectedItem->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 107, 77, 127 );
	m_btnMoveSelectedItem->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  1, 134, 77, 154 );
	m_btnMoveSelectedItem->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 160, 77, 180 );
	m_btnMoveSelectedItem->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	m_btnMoveSelectedItem->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3167 );//선택옮김
	m_btnMoveSelectedItem->ShowWindow(TRUE);
	InsertChildObject( m_btnMoveSelectedItem );
#endif
	//모두옮김 버튼 ( 76 X 20 ) 
//	_XBTN_STRUCTURE allmovebtnstruct = { TRUE, { 188, 388 }, { 76, 20 }, _XDEF_CASHBOX_BTN_ALLMOVE ,
//		cashboxresourceindex1,cashboxresourceindex1,cashboxresourceindex1, 
//		&g_MainInterfaceTextureArchive };
//	
//	m_btnMoveAllItem = new _XButton;
//	m_btnMoveAllItem->Create( allmovebtnstruct );		
//	m_btnMoveAllItem->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 107, 77, 127 );
//	m_btnMoveAllItem->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  1, 134, 77, 154 );
//	m_btnMoveAllItem->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 160, 77, 180 );
//	m_btnMoveAllItem->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
//	m_btnMoveAllItem->SetButtonText(_T("모두 옮김"));//
//	m_btnMoveAllItem->ShowWindow(TRUE);
//	InsertChildObject( m_btnMoveAllItem );
	
	
	//새로고침 버튼 ( 91 X 21 ) 
	_XBTN_STRUCTURE renewbtnstruct = { TRUE, { 41, 416 }, { 91, 21 }, _XDEF_CASHBOX_BTN_RENEW ,
		cashmallresourceindex2,cashmallresourceindex2,cashmallresourceindex2, 
		&g_MainInterfaceTextureArchive };
	
	m_btnRenew = new _XButton;
	m_btnRenew->Create( renewbtnstruct );		
	m_btnRenew->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 164, 188, 255, 209 );
	m_btnRenew->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  164, 211, 255, 232 );
	m_btnRenew->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  164, 234, 255, 255 );
	m_btnRenew->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	m_btnRenew->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LOGINSERVER_2180 );//새로고침
	m_btnRenew->ShowWindow(TRUE);
	InsertChildObject( m_btnRenew );
	
	//닫기 버튼 ( 91 X 21 ) 
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 194, 416 }, { 91, 21 }, _XDEF_CASHBOX_BTN_CLOSE ,
		cashmallresourceindex2,cashmallresourceindex2,cashmallresourceindex2, 
		&g_MainInterfaceTextureArchive };
	
	_XButton* pCloseButton = new _XButton;
	pCloseButton->Create( closebtnstruct );		
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 164, 188, 255, 209 );
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  164, 211, 255, 232 );
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  164, 234, 255, 255 );
	pCloseButton->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	pCloseButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_OPTION_CLOSE );//닫기
	pCloseButton->ShowWindow(TRUE);
	InsertChildObject( pCloseButton );
	
	//아이템 상점 버튼 ( 179 X 32 ) 
	_XBTN_STRUCTURE gocashmallbtnstruct = { TRUE, { 73, 442 }, { 179, 26 }, _XDEF_CASHBOX_BTN_MOVECASHMALL ,
		cashboxresourceindex2,cashboxresourceindex2,cashboxresourceindex2, 
		&g_MainInterfaceTextureArchive };
	
	_XButton* pGoCashMallButton = new _XButton;
	pGoCashMallButton->Create( gocashmallbtnstruct );		
	pGoCashMallButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 77, 143, 256, 175 );
	pGoCashMallButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  77, 183, 256, 216 );
	pGoCashMallButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  77, 223, 256, 255 );
	pGoCashMallButton->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	pGoCashMallButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3193 );//아이템 상점으로
	pGoCashMallButton->ShowWindow(TRUE);
	InsertChildObject( pGoCashMallButton );
	
	// 캐쉬 보관함 탭
	_XCHECKBTN_STRUCTURE cashboxtabbtnstruct = 
	{   
		TRUE, { -31, 62 }, { 41, 88 }, 
			_XDEF_CASHBOX_TABBUTTON_PERSONAL, cashboxresourceindex1,	cashboxresourceindex1, 
			&g_MainInterfaceTextureArchive 
	};	
	
	for( int i = 0; i < _XCASHBOXTAB_TOTALCOUNT; i++ )
	{
		m_TabButton[i] = new _XCheckButton;
		m_TabButton[i]->Create( cashboxtabbtnstruct );
		
		m_TabButton[i]->SetButtonImageClipRect(   _XCHECKBUTTONIMAGE_NORMAL, 134, 56, 175, 144 );
		m_TabButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 89, 56, 130, 144 );
		
		m_TabButton[i]->SetCheck( FALSE );
		m_TabButton[i]->EnableWindow(TRUE);
		
		m_TabButton[i]->ShowWindow( TRUE );
		
		InsertChildObject( m_TabButton[i] );
		
		
		
		cashboxtabbtnstruct.position.y += 90;
		cashboxtabbtnstruct.commandid++;
	}
	m_TabButton[0]->SetCheck( TRUE );
	
#ifdef _XDEF_COMMONITEMMALL_20070402
	_XCHECKBTN_STRUCTURE commoncashboxtabbtnstruct = 
	{   
		TRUE, { -31, 62 }, { 41, 52 }, 
			_XDEF_CASHBOX_TABBUTTON_PERSONAL, cashboxresourceindex3, cashboxresourceindex3, 
			&g_MainInterfaceTextureArchive 
	};	
	
	m_Common_TabButton[0] = new _XCheckButton;
	m_Common_TabButton[0]->Create( commoncashboxtabbtnstruct );
	m_Common_TabButton[0]->SetButtonImageClipRect(   _XCHECKBUTTONIMAGE_CLICK, 132, 2, 173, 54 );
	m_Common_TabButton[0]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 177, 2, 218, 54 );
	m_Common_TabButton[0]->SetCheck( FALSE );
	m_Common_TabButton[0]->EnableWindow(TRUE);
	m_Common_TabButton[0]->ShowWindow( TRUE );
	m_Common_TabButton[0]->SetCallBackFunction_ToolTip( _XCallback_CashMalll_Normal );
	InsertChildObject( m_Common_TabButton[0] );	
	
	commoncashboxtabbtnstruct.position.y += 54;
	commoncashboxtabbtnstruct.commandid++;
	m_Common_TabButton[1] = new _XCheckButton;
	m_Common_TabButton[1]->Create( commoncashboxtabbtnstruct );
	m_Common_TabButton[1]->SetButtonImageClipRect(   _XCHECKBUTTONIMAGE_CLICK, 177, 58, 218, 110 );
	m_Common_TabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 132, 58, 173, 110 );
	m_Common_TabButton[1]->SetCheck( FALSE );
	m_Common_TabButton[1]->EnableWindow(TRUE);
	m_Common_TabButton[1]->ShowWindow( TRUE );
	m_Common_TabButton[1]->SetCallBackFunction_ToolTip( _XCallback_CashMalll_Present );
	InsertChildObject( m_Common_TabButton[1] );		

	m_Common_TabButton[0]->SetCheck( TRUE );
		
	//처음으로 버튼 ( 15 X 11 ) 
	_XBTN_STRUCTURE firstbtnstruct = { TRUE, { 98, 395 }, { 15, 11 }, _XDEF_CASHBOX_BTN_FIRST ,
		cashboxresourceindex1,cashboxresourceindex1,cashboxresourceindex1, 
		&g_MainInterfaceTextureArchive };
	
	m_btnFirst = new _XButton;
	m_btnFirst->Create( firstbtnstruct );		
	m_btnFirst->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 111, 199, 126, 210 );
	m_btnFirst->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  128, 199, 143, 210 );
	m_btnFirst->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  145, 198, 160, 209 );
	m_btnFirst->ShowWindow(FALSE);
	InsertChildObject( m_btnFirst );
	
	//끝으로 버튼 ( 15 X 11 ) 
	_XBTN_STRUCTURE endbtnstruct = { TRUE, { 98+103+10, 395 }, { 15, 11 }, _XDEF_CASHBOX_BTN_END ,
		cashboxresourceindex1,cashboxresourceindex1,cashboxresourceindex1, 
		&g_MainInterfaceTextureArchive };
	
	m_btnEnd = new _XButton;
	m_btnEnd->Create( endbtnstruct );		
	m_btnEnd->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 113, 185, 128, 196 );
	m_btnEnd->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  130, 185, 145, 196 );
	m_btnEnd->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  147, 185, 162, 196 );
	m_btnEnd->ShowWindow(FALSE);
	InsertChildObject( m_btnEnd );
	
	//이전 버튼 ( 9 X 11 ) 
	_XBTN_STRUCTURE abjacentbtnstruct = { TRUE, { 98+16, 395 }, { 9, 11 }, _XDEF_CASHBOX_BTN_ADJACENT ,
		cashboxresourceindex1,cashboxresourceindex1,cashboxresourceindex1, 
		&g_MainInterfaceTextureArchive };
	
	m_btnAdjacent = new _XButton;
	m_btnAdjacent->Create( abjacentbtnstruct );		
	m_btnAdjacent->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  121, 212, 130, 223 );
	m_btnAdjacent->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 111, 212, 120, 223 );
	m_btnAdjacent->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  131, 212, 140, 223 );
	m_btnAdjacent->ShowWindow(FALSE);
	InsertChildObject( m_btnAdjacent );
	
	//다음 버튼 ( 9 X 11 ) 
	_XBTN_STRUCTURE nextbtnstruct = { TRUE, { 98+103, 395 }, { 9, 11 }, _XDEF_CASHBOX_BTN_NEXT ,
		cashboxresourceindex1,cashboxresourceindex1,cashboxresourceindex1, 
		&g_MainInterfaceTextureArchive };
	
	m_btnNext = new _XButton;
	m_btnNext->Create( nextbtnstruct );		
	m_btnNext->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  154, 212, 163, 223 );
	m_btnNext->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 144, 212, 153, 223 );
	m_btnNext->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  164, 212, 173, 223 );
	m_btnNext->ShowWindow(FALSE);
	InsertChildObject( m_btnNext );
	
	//페이지 버튼 ( 12 X 12 ) 
	_XBTN_STRUCTURE pagebtnstruct = { TRUE, { 98+16+12, 394 }, { 12, 12 }, _XDEF_CASHBOX_BTN_PAGE1 ,
		cashboxresourceindex1,cashboxresourceindex1,cashboxresourceindex1, 
		&g_MainInterfaceTextureArchive };

	for( i = 0; i < 5; i++ ) //Author : 양희왕 //breif : 페이지 버튼 수정
	{
		m_btnPage[i] = new _XButton;
		m_btnPage[i]->Create( pagebtnstruct );		
		m_btnPage[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,	113, 167, 125, 179 );
		m_btnPage[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,	127, 167, 139, 179 );
		m_btnPage[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,	141, 167, 153, 179 );
		m_btnPage[i]->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
		m_btnPage[i]->ShowWindow(FALSE);
		m_btnPage[i]->EnableWindow(TRUE);
		InsertChildObject( m_btnPage[i] );

		++pagebtnstruct.commandid;
		pagebtnstruct.position.x += 15;	
	}
	
	/*m_btnPage1 = new _XButton;
	m_btnPage1->Create( pagebtnstruct );		
	m_btnPage1->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,	113, 167, 125, 179 );
	m_btnPage1->SetButtonImageClipRect( _XBUTTONIMAGE_READY,	127, 167, 139, 179 );
	m_btnPage1->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,	141, 167, 153, 179 );
	m_btnPage1->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	m_btnPage1->SetButtonText(_T("1"));
	m_btnPage1->ShowWindow(TRUE);
	InsertChildObject( m_btnPage1 );
	
	++pagebtnstruct.commandid;
	pagebtnstruct.position.x += 15;	
	m_btnPage2 = new _XButton;
	m_btnPage2->Create( pagebtnstruct );		
	m_btnPage2->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,	113, 167, 125, 179 );
	m_btnPage2->SetButtonImageClipRect( _XBUTTONIMAGE_READY,	127, 167, 139, 179 );
	m_btnPage2->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,	141, 167, 153, 179 );
	m_btnPage2->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	m_btnPage2->SetButtonText(_T("2"));
	m_btnPage2->ShowWindow(TRUE);
	InsertChildObject( m_btnPage2 );
	
	++pagebtnstruct.commandid;
	pagebtnstruct.position.x += 15;	
	m_btnPage3 = new _XButton;
	m_btnPage3->Create( pagebtnstruct );		
	m_btnPage3->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,	113, 167, 125, 179 );
	m_btnPage3->SetButtonImageClipRect( _XBUTTONIMAGE_READY,	127, 167, 139, 179 );
	m_btnPage3->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,	141, 167, 153, 179 );
	m_btnPage3->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	m_btnPage3->SetButtonText(_T("3"));
	m_btnPage3->ShowWindow(TRUE);
	InsertChildObject( m_btnPage3 );
	
	++pagebtnstruct.commandid;
	pagebtnstruct.position.x += 15;	
	m_btnPage4 = new _XButton;
	m_btnPage4->Create( pagebtnstruct );		
	m_btnPage4->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,	113, 167, 125, 179 );
	m_btnPage4->SetButtonImageClipRect( _XBUTTONIMAGE_READY,	127, 167, 139, 179 );
	m_btnPage4->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,	141, 167, 153, 179 );
	m_btnPage4->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	m_btnPage4->SetButtonText(_T("4"));
	m_btnPage4->ShowWindow(TRUE);
	InsertChildObject( m_btnPage4 );
	
	++pagebtnstruct.commandid;
	pagebtnstruct.position.x += 15;	
	m_btnPage5 = new _XButton;
	m_btnPage5->Create( pagebtnstruct );		
	m_btnPage5->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,	113, 167, 125, 179 );
	m_btnPage5->SetButtonImageClipRect( _XBUTTONIMAGE_READY,	127, 167, 139, 179 );
	m_btnPage5->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,	141, 167, 153, 179 );
	m_btnPage5->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	m_btnPage5->SetButtonText(_T("5"));
	m_btnPage5->ShowWindow(TRUE);
	InsertChildObject( m_btnPage5 );*/
	
	//메세지 보기 버튼 ( 14 X 14 ) 
	_XBTN_STRUCTURE msgbtnstruct = { TRUE, { 290, 80 }, { 14, 14 }, _XDEF_CASHBOX_BTN_MSG1 ,
		cashboxresourceindex1,cashboxresourceindex1,cashboxresourceindex1, 
		&g_MainInterfaceTextureArchive };
	
	for( i = 0; i < 9; ++i )
	{
		m_btnMessage[i] = new _XButton;
		m_btnMessage[i]->Create( msgbtnstruct );		
		m_btnMessage[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,	83, 194, 97, 208 );
		m_btnMessage[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,	83, 194, 97, 208 );
		m_btnMessage[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,	83, 209, 97, 223 );
		m_btnMessage[i]->ShowWindow(FALSE);
		InsertChildObject( m_btnMessage[i] );
		
		++msgbtnstruct.commandid;
		msgbtnstruct.position.y += 34;	
	}
	
	//메세지 버튼 닫기 버튼 ( 10 X 10 ) 
	_XBTN_STRUCTURE msgclosebtnstruct = { TRUE, { 0, 0 }, { 10, 10 }, _XDEF_CASHBOX_BTN_MSGCLOSE ,
		cashboxresourceindex1,cashboxresourceindex1,cashboxresourceindex1, 
		&g_MainInterfaceTextureArchive };
	
	m_btnMsgClose = new _XButton;
	m_btnMsgClose->Create( msgclosebtnstruct );		
	m_btnMsgClose->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,	99, 199, 109, 209 );
	m_btnMsgClose->SetButtonImageClipRect( _XBUTTONIMAGE_READY,		99, 199, 109, 209 );
	m_btnMsgClose->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,		99, 212, 109, 222 );
	m_btnMsgClose->ShowWindow(FALSE);
	InsertChildObject( m_btnMsgClose );
		
#endif
	
#ifdef _XDEF_COMMONITEMMALL_20070402		
	// 자원 수집에 필요한 수집 아이템 리스트 박스
	_XLISTBOX_STRUCTURE cashitemliststructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 10, 76 },										// 윈도우 좌표
		{ 277, 306 },									// 윈도우 사이즈
		_XDEF_CASHBOX_LISTBOX_CASHITEM,					// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		34,												// 리스트 아이템 라인 피치
		9,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_NOSCROLLBAR,							// 스크롤바의 위치
		
	};
	
	m_listboxCashItem = new _XListBox;
	m_listboxCashItem->Create(cashitemliststructure);	
	m_listboxCashItem->SetSelectItemMouseHoverCallBack( _CashItemPackageListBoxItemMouseHoverCallback );
	
	POINT columnoffset1 = { 12, 10 };
	POINT columnoffset2 = { 12, 10 };
	POINT columnoffset3 = { 46, 2 };
	POINT columnoffset4 = { 90, 10 };
	
	m_listboxCashItem->SetColumnOffset(0, columnoffset1);				// column offset
	m_listboxCashItem->SetColumnOffset(1, columnoffset2);
	m_listboxCashItem->SetColumnOffset(2, columnoffset3);
	m_listboxCashItem->SetColumnOffset(3, columnoffset4);
	
	m_listboxCashItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_listboxCashItem->SetColumnStyle(1, _XLCOLUMNSTYLE_ICONRECT);
	m_listboxCashItem->SetColumnStyle(2, _XLCOLUMNSTYLE_ICONRECT);
	m_listboxCashItem->SetColumnStyle(3, _XLCOLUMNSTYLE_STRING);
	
	m_listboxCashItem->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxCashItem->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_listboxCashItem->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);
	m_listboxCashItem->SetAlignType(3, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxCashItem->SetColumnWidth(0, 170);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxCashItem->SetColumnWidth(1, 170);
	m_listboxCashItem->SetColumnWidth(2, 136);
	m_listboxCashItem->SetColumnWidth(3, 170);
	
	m_listboxCashItem->SetTrackWidth(273);	
	m_listboxCashItem->ShowWindow(TRUE);
	InsertChildObject(m_listboxCashItem);	
#else
	// 자원 수집에 필요한 수집 아이템 리스트 박스
	_XLISTBOX_STRUCTURE cashitemliststructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 10, 76 },								// 윈도우 좌표
		{ 307, 306 },									// 윈도우 사이즈
		_XDEF_CASHBOX_LISTBOX_CASHITEM,		// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		34,												// 리스트 아이템 라인 피치
		9,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 306 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		150,												// 전체 리스트 갯수
		cashboxresourceindex2,
		cashboxresourceindex2,
		cashboxresourceindex2,
		cashboxresourceindex2,
		cashboxresourceindex2,
	};
	
	m_listboxCashItem = new _XListBox;
	m_listboxCashItem->Create(cashitemliststructure);
		
	_XVScrollBar* pScrollBar = m_listboxCashItem->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 222,  2, 236, 14 },
			{ 222, 36, 236, 48 },
			{ 243,  2, 255, 14 },
			{ 243, 36, 255, 48 },
			{ 222, 54, 236, 76 }
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}
	m_listboxCashItem->SetAlwaysShowScrollBarMode(TRUE);
	m_listboxCashItem->SetSelectItemMouseHoverCallBack( _CashItemPackageListBoxItemMouseHoverCallback );
	
	POINT columnoffset1 = { 12, 10 };
	POINT columnoffset2 = { 12, 10 };
	POINT columnoffset3 = { 46, 2 };
	POINT columnoffset4 = { 90, 10 };
	
	m_listboxCashItem->SetColumnOffset(0, columnoffset1);				// column offset
	m_listboxCashItem->SetColumnOffset(1, columnoffset2);
	m_listboxCashItem->SetColumnOffset(2, columnoffset3);
	m_listboxCashItem->SetColumnOffset(3, columnoffset4);
	
	m_listboxCashItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_listboxCashItem->SetColumnStyle(1, _XLCOLUMNSTYLE_ICONRECT);
	m_listboxCashItem->SetColumnStyle(2, _XLCOLUMNSTYLE_ICONRECT);
	m_listboxCashItem->SetColumnStyle(3, _XLCOLUMNSTYLE_STRING);
	
	m_listboxCashItem->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxCashItem->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_listboxCashItem->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);
	m_listboxCashItem->SetAlignType(3, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxCashItem->SetColumnWidth(0, 170);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxCashItem->SetColumnWidth(1, 170);
	m_listboxCashItem->SetColumnWidth(2, 136);
	m_listboxCashItem->SetColumnWidth(3, 170);
	
	m_listboxCashItem->SetTrackWidth(293);	
	m_listboxCashItem->ShowWindow(TRUE);
	InsertChildObject(m_listboxCashItem);
#endif
	
	return TRUE;
}

void _XWindow_CashBox::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_CashBox::Draw(_XGUIObject*& pfocusobject)
{
	if( this->m_ShowWindow )
	{
		// 307 X 25
		_XDrawSolidBar( m_WindowPosition.x+10, m_WindowPosition.y+50,
			m_WindowPosition.x+317, m_WindowPosition.y+75 , D3DCOLOR_ARGB(255,64,50,37));
		
		// 307 X 308
		_XDrawSolidBar( m_WindowPosition.x+10, m_WindowPosition.y+75,
			m_WindowPosition.x+317, m_WindowPosition.y+383 , D3DCOLOR_ARGB(255,35,23,7));
		
		// 307 X 29
		_XDrawSolidBar( m_WindowPosition.x+10, m_WindowPosition.y+383,
			m_WindowPosition.x+317, m_WindowPosition.y+412 , D3DCOLOR_ARGB(255,64,50,37));
		
		_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+73, m_WindowPosition.x+317, m_WindowPosition.y+384 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+74, m_WindowPosition.x+317, m_WindowPosition.y+383 , 0.0f, D3DCOLOR_ARGB(150,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+75, m_WindowPosition.x+317, m_WindowPosition.y+382 , 0.0f, D3DCOLOR_ARGB(63,0,0,0) );

	}

	_XWindow::Draw(pfocusobject);	
	
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
	
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->Puts(m_WindowPosition.x + 112, m_WindowPosition.y + 18,_XGETINTERFACETEXT(ID_STRING_NEW_2978));
	
#ifdef _XDEF_COMMONITEMMALL_20070402
#else
	g_XBaseFont->Puts(m_WindowPosition.x - 15, m_WindowPosition.y + 78, _T("내"));
	g_XBaseFont->Puts(m_WindowPosition.x - 15, m_WindowPosition.y + 93, _T("보"));
	g_XBaseFont->Puts(m_WindowPosition.x - 15, m_WindowPosition.y + 108, _T("관"));
	g_XBaseFont->Puts(m_WindowPosition.x - 15, m_WindowPosition.y + 123, _T("함"));

	g_XBaseFont->Puts(m_WindowPosition.x - 15, m_WindowPosition.y + 174, _T("선"));
	g_XBaseFont->Puts(m_WindowPosition.x - 15, m_WindowPosition.y + 189, _T("물"));
	g_XBaseFont->Puts(m_WindowPosition.x - 15, m_WindowPosition.y + 204, _T("함"));
	g_XBaseFont->Flush();
#endif
	g_XBaseFont->DisableGrowMode();
	g_XBaseFont->Puts(m_WindowPosition.x + 16, m_WindowPosition.y + 56, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_SELECT));//_T("선택")
	g_XBaseFont->Puts(m_WindowPosition.x + 55, m_WindowPosition.y + 56, _XGETINTERFACETEXT(ID_STRING_NEW_3194));//_T("아이콘")
	g_XBaseFont->Puts(m_WindowPosition.x + 177, m_WindowPosition.y + 56,_XGETINTERFACETEXT(ID_STRING_NEW_3197));// _T("물품명")
	g_XBaseFont->Flush();
	

#ifdef _XDEF_COMMONITEMMALL_20070402
	if( m_bShowPresentMessage )
	{		
		_XDrawSolidBar( m_WindowPosition.x+ m_WindowSize.cx+2, m_WindowPosition.y+80+(34*m_PresentMessageBoxPosition), 
			m_WindowPosition.x+ m_WindowSize.cx+202, m_WindowPosition.y+170+(34*m_PresentMessageBoxPosition) , D3DCOLOR_ARGB(255,64,50,37));
		_XDrawRectAngle( m_WindowPosition.x+ m_WindowSize.cx+2, m_WindowPosition.y+80+(34*m_PresentMessageBoxPosition), 
			m_WindowPosition.x+ m_WindowSize.cx+202, m_WindowPosition.y+170+(34*m_PresentMessageBoxPosition) , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+ m_WindowSize.cx+3, m_WindowPosition.y+81+(34*m_PresentMessageBoxPosition), 
			m_WindowPosition.x+ m_WindowSize.cx+201, m_WindowPosition.y+169+(34*m_PresentMessageBoxPosition) , 0.0f, D3DCOLOR_ARGB(150,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+ m_WindowSize.cx+4, m_WindowPosition.y+82+(34*m_PresentMessageBoxPosition), 
			m_WindowPosition.x+ m_WindowSize.cx+200, m_WindowPosition.y+168+(34*m_PresentMessageBoxPosition) , 0.0f, D3DCOLOR_ARGB(63,0,0,0) );

		m_btnMsgClose->Draw();
		
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));

		if( m_PresentName[0] != 0 )
			g_XBaseFont->Puts(m_WindowPosition.x+ m_WindowSize.cx+5, m_WindowPosition.y+85+(34*m_PresentMessageBoxPosition), m_PresentName );
		
		if( m_PresentTime[0] != 0 )
			g_XBaseFont->Puts(m_WindowPosition.x+ m_WindowSize.cx+5, m_WindowPosition.y+85+15+(34*m_PresentMessageBoxPosition), m_PresentTime );

		if( m_PresentMessage[0] != 0 )
			g_XBaseFont->Puts_Separate( m_WindowPosition.x+ m_WindowSize.cx+5, m_WindowPosition.y+85+30+(34*m_PresentMessageBoxPosition),m_PresentMessage, 180 );

		g_XBaseFont->Flush();

	}
#endif
}

BOOL _XWindow_CashBox::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_CashBox::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
#ifdef _XDEF_COMMONITEMMALL_20070402
	m_btnMsgClose->MoveWindow(m_WindowPosition.x+ m_WindowSize.cx+188, m_WindowPosition.y+84+(34*m_PresentMessageBoxPosition));
#endif
}

void _XWindow_CashBox::ShowWindow(BOOL show)
{	
	if( show )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3881), _XDEF_CHATMESSAGECOLOR_ITEM, FALSE ); //선물받은 캐시아이템은 캐시아이템 상점의 "선물상자"에서 수령하신 후 사용하실 수 있습니다.
		//m_btnMoveAllItem->EnableWindow(TRUE);
        ChangeTab(_XCASHBOXTAB_PERSONAL);

		m_btnMoveSelectedItem->EnableWindow(TRUE);
		m_btnRenew->EnableWindow(TRUE);
		m_TabButton[0]->EnableWindow(TRUE);
		m_TabButton[1]->EnableWindow(TRUE);
#ifdef _XDEF_COMMONITEMMALL_20070402
        m_bShowPresentMessage = FALSE;
	    m_btnMsgClose->ShowWindow(FALSE);

		m_TabButton[0]->ShowWindow(FALSE);
		m_TabButton[1]->ShowWindow(FALSE);

		m_Common_TabButton[0]->EnableWindow(TRUE);
		m_Common_TabButton[1]->EnableWindow(TRUE);
		
		//Author : 양희왕 //breif : 현재 페이지를 저장한다 Date : 08/03/07
		m_CurrentPageNumber = 0;
		g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,1,9);
#else
		g_NetworkKernel.SendPacket(MSG_NO_ITEM_SHOP,PKTMODE_PERSONAL_CASH_INVEN_REQ);
#endif
		

	}
	_XWindow::ShowWindow(show);
}

void _XWindow_CashBox::SetListItem(void)
{	
	m_listboxCashItem->DeleteAllItem();	
	
	int optionmenuinterface = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	int imageindex = g_MainInterfaceTextureArchive.FindResource("item_mon.tga");
	
	RECT rect1;
	rect1.left = 216;
	rect1.right = 230;
	rect1.top = 31;
	rect1.bottom = 45;

	RECT rect2;
	rect2.left = 216;
	rect2.right = 230;
	rect2.top = 0;
	rect2.bottom = 14;

	RECT rect;
	rect.left = 1;
	rect.right = 31;
	rect.top = 1;
	rect.bottom = 31;
	
	int listcount = 0;
	_XCASHITEMBOX_INFO* cashitem = NULL;
	list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
	for(iter_cashitem = m_listCashItem.begin() ; iter_cashitem != m_listCashItem.end() ; ++iter_cashitem)
	{
		cashitem = *iter_cashitem;
		if( cashitem )
		{
			cashitem->indexBox = listcount+1;
			// 아이템 삽입
			m_listboxCashItem->InsertListItem(_T(""), 0, listcount+1);		
			
			// 리스트 아이템 이미지 삽입
			m_listboxCashItem->SetIconArchive(listcount, &g_MainInterfaceTextureArchive);
			m_listboxCashItem->SetItemAttrib(listcount, 0, optionmenuinterface, rect1);
			
			if( cashitem->bCheck )
			{
				m_listboxCashItem->SetIconArchive(listcount, &g_MainInterfaceTextureArchive);
				m_listboxCashItem->SetItemAttrib(listcount, 1, optionmenuinterface, rect2);
			}
			else
			{
				m_listboxCashItem->SetIconArchive(listcount, &g_MainInterfaceTextureArchive);
				m_listboxCashItem->SetItemAttrib(listcount, 1, -1, rect2);
			}
			
			//GetIconRect(category, itemid, imageindex, rect, TRUE);
			//m_listboxCashItem->SetIconArchive(listcount, &g_MainInterfaceTextureArchive);
			//m_listboxCashItem->SetItemAttrib(listcount, 2, imageindex, rect);
			
#ifdef _XTS_ITEMMALLBROWSER
			// 리스트 아이템 텍스트 삽입				
			_XCashItemPackage* currentcashitempackage = NULL;
			list <_XCashItemPackage*>::iterator iter_cashitempackage;
			for(iter_cashitempackage = g_CashItemPackageManger.m_listCashItemPackage.begin() ; iter_cashitempackage != g_CashItemPackageManger.m_listCashItemPackage.end() ; ++iter_cashitempackage)
			{
				currentcashitempackage = *iter_cashitempackage;
				if( currentcashitempackage )
				{
					if( currentcashitempackage->m_indexCashItemPackage == cashitem->indexPackage )
					{			
						// 리스트 아이템 이미지 삽입
						GetIconRect(currentcashitempackage->m_CashItemType , currentcashitempackage->m_indexPackageIcon-1 , imageindex, rect, TRUE);
						m_listboxCashItem->SetIconArchive(listcount, &g_MainInterfaceTextureArchive);
						m_listboxCashItem->SetItemAttrib(listcount, 2, imageindex, rect);
			
						m_listboxCashItem->SetItemText(listcount, 3, currentcashitempackage->m_PackageName);
						m_listboxCashItem->SetItemAttrib(listcount, 3, D3DCOLOR_ARGB(255, 219, 196, 136));
						m_listboxCashItem->SetItemAttribHighlight(listcount, 3, D3DCOLOR_ARGB(255, 219, 196, 136));
						break;
					}
				}
			}
#endif
			++listcount;
		}
	}
	
#ifdef _XDEF_COMMONITEMMALL_20070402
	if( m_SelectedTab == _XCASHBOXTAB_PRESENT )
	{
		for( int i = 0; i < 9; ++i )
			m_btnMessage[i]->ShowWindow(FALSE);

		int ItemCount = m_listCashItem.size();		

		for( i = 0; i < ItemCount; ++i )
			m_btnMessage[i]->ShowWindow(TRUE);

	}
#endif
	
	_XVScrollBar* pScrollBar = m_listboxCashItem->GetScrollBarObject();
	if( pScrollBar )
	{
		pScrollBar->SetLineScrollPos(0);
	}
	
}

void _XWindow_CashBox::GetIconRect(char cType, int iconnumber, int& imageindex, RECT& rect, BOOL smallicon)
{
	int		iconclipcolumn = 0, iconcliprow = 0;
	_XGENDERTYPE gender = _XGENDER_MAN;
	
	imageindex = g_MainInterfaceTextureArchive.FindResource("item_mon.tga");	// 이미지 없는 아이템들은 다 이걸로...
	
	TCHAR tempStr [128];
	memset( tempStr, 0, sizeof(TCHAR)*128);
	
	char itemtype = GetEqualItemType(cType);
	
	if( itemtype != -1 )
	{
		switch(itemtype) 
		{
		//case _XGI_FC_WEAPON :
		case _XGI_FC_CLOTHES :
			{
				iconclipcolumn = iconnumber % 8;
				iconcliprow    = iconnumber / 8;
				imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Pack_Clo_01.tga");
			}
			break;
		case _XGI_FC_BOOK :
			{
				iconclipcolumn = iconnumber % 8;
				iconcliprow    = iconnumber / 8;
				imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Pack_Boo_01.tga");
			}
			break;
		case _XGI_FC_POTION :
			{
				iconclipcolumn = iconnumber % 8;
				iconcliprow    = iconnumber / 8;
				imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Pack_Pot_01.tga");
			}
			break;
		//case _XGI_FC_CONSUMING :
		case _XGI_FC_ELIXIR :
			{
				iconclipcolumn = iconnumber % 8;
				iconcliprow    = iconnumber / 8;
				imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Pack_Eli_01.tga");
			}
			break;
		case _XGI_FC_RESOURCE :
			{
				iconclipcolumn = iconnumber % 8;
				iconcliprow    = iconnumber / 8;
				imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Pack_Res_01.tga");
			}
			break;
		//case _XGI_FC_QUEST :
		case _XGI_FC_ACCESSORY :
			{
				iconclipcolumn = iconnumber % 8;
				iconcliprow    = iconnumber / 8;
				imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Pack_Acc_01.tga");
			}
			break;
		case _XGI_FC_LIFE : //Author : 양희왕
			{
#ifdef _XDEF_SMELT_ITEM
				iconclipcolumn = iconnumber % 8;
				iconcliprow    = iconnumber / 8;
				imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Pack_Life_01.tga");
#endif
			}
			break;
		//case _XGI_FC_SOCKET :
		//case _XGI_FC_BOX :
		//case _XGI_FC_BOXKEY :
		//case _XGI_FC_MONEY :
		}
	}


	if(iconnumber != -1 && imageindex > -1)
	{
		rect.left    = iconclipcolumn * 32;
		rect.right   = rect.left + 32;
		
		rect.top     = iconcliprow	   * 32;
		rect.bottom  = rect.top  + 32;
		
		if(gender == _XGENDER_WOMAN)
		{
			rect.top     += 128;
			rect.bottom  += 128;
		}
		
		if(smallicon)
		{
			rect.left  += 1;
			rect.top   += 1;
			rect.right -= 1;
			rect.bottom-= 1;
		}
	}
	else
	{
		// icon 없는 아이템 처리
		rect.left    = 33;
		rect.top	 = 1;
		
		rect.right   = 63;
		rect.bottom  = 31;
		
		imageindex = g_MainInterfaceTextureArchive.FindResource("item_pot_w3_003.tga");
	}
}

void _XWindow_CashBox::ChangeTab(_XCashBoxTab selectedtab)
{
	if( m_SelectedTab != selectedtab )
	{
		m_SelectedTab = selectedtab;

		for( int i = 0; i < _XCASHBOXTAB_TOTALCOUNT; i++ )
		{			
			m_TabButton[i]->SetCheck( FALSE );	
#ifdef _XDEF_COMMONITEMMALL_20070402
			m_Common_TabButton[i]->SetCheck( FALSE );
#endif	

		}	
		
	}
	if( m_SelectedTab == _XCASHBOXTAB_PERSONAL)
	{
		m_TabButton[0]->SetCheck( TRUE );
#ifdef _XDEF_COMMONITEMMALL_20070402
		m_Common_TabButton[0]->SetCheck( TRUE );
		for( int i = 0; i < 9; ++i )
			m_btnMessage[i]->ShowWindow(FALSE);
		
		//패킷 보내기 	
		g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,1,9);
#else
		//패킷 보내기 
		g_NetworkKernel.SendPacket(MSG_NO_ITEM_SHOP,PKTMODE_PERSONAL_CASH_INVEN_REQ);
		
#endif
	}
	else if( m_SelectedTab == _XCASHBOXTAB_PRESENT)
	{
		m_TabButton[1]->SetCheck( TRUE );
#ifdef _XDEF_COMMONITEMMALL_20070402
		m_Common_TabButton[1]->SetCheck( TRUE );
		
		//패킷 보내기 	
		g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,1,9);
#else
		//패킷 보내기 	
		g_NetworkKernel.SendPacket(MSG_NO_ITEM_SHOP,PKTMODE_PRESENT_CASH_INVEN_REQ);
#endif
		
	}		
		
}


#ifdef _XDEF_COMMONITEMMALL_20070402
void _XWindow_CashBox::SetPresentMessage( int OrderNumber )
{
	int index = 0;
	_XCASHITEMBOX_INFO* cashitem = NULL;
	list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
	for(iter_cashitem = m_listCashItem.begin() ; iter_cashitem != m_listCashItem.end() ; ++iter_cashitem)
	{
		cashitem = *iter_cashitem;
		if( cashitem )
		{
			if( cashitem->OrderNumber == OrderNumber )
			{
				strcpy( m_PresentName, cashitem->PresentName );
				
				TCHAR messagestring[128];
				sprintf(messagestring,_T("%d-%d-%d %d:%d:%d"),(int)cashitem->PresentTime.wYear, (int)cashitem->PresentTime.wMonth,(int)cashitem->PresentTime.wDay,
					(int)cashitem->PresentTime.wHour,(int)cashitem->PresentTime.wMinute,(int)cashitem->PresentTime.wSecond );
										
				strcpy( m_PresentTime, messagestring );
				strcpy( m_PresentMessage, cashitem->PresentMsg);

				break;
			}

		}
	}
}

void _XWindow_CashBox::SetPageNumberButton( void )
{//Author : 양희왕 //breif : 다음 페이지도 고려해서..만들어야함 //Date : 08/03/27
	
	TCHAR number[8];
	
	int nTotalpage = m_nCashItemTotalCount/9;
	if( m_nCashItemTotalCount > 0 )
	{
		if( m_nCashItemTotalCount%9 != 0 )
			nTotalpage += 1;
	}

	
	int nPageNumber = m_CurrentPageNumber; // 0 이 1 페이지

	if(nTotalpage >= 2) //2페이지 넘어가면 버튼 나오게..
	{
		m_btnFirst->ShowWindow(TRUE);
		m_btnEnd->ShowWindow(TRUE);
		m_btnAdjacent->ShowWindow(TRUE);
		m_btnNext->ShowWindow(TRUE);
	}
	else //1페이지 뿐이다..
	{
		m_btnFirst->ShowWindow(FALSE);
		m_btnEnd->ShowWindow(FALSE);
		m_btnAdjacent->ShowWindow(FALSE);
		m_btnNext->ShowWindow(FALSE);
	}
	
	//5페이지 씩
	m_Current5PageCount = nPageNumber/5; // 현재 페이지를 5페이지로 끊어본다. (0 ~ 4) = 0, (5 ~ 9) = 1
	int nStartPage = 5*m_Current5PageCount; //보여지는 페이지의 시작페이지를 설정한다 (0~4 는 0부터 5~9 는 5부터 10~14는 10부터..
	
	nStartPage += 1; //보여지는 페이지는 0부터 시작하지 않는다.
	int nEndPage = nStartPage + 5; //보여지는 페이지 5페이지

	for(int nCount = 0; nStartPage < nEndPage; nCount++, nStartPage++ )
	{
		if( nStartPage <= nTotalpage )
		{
			memset(number, 0, sizeof(number));
			sprintf(number,_T("%d"), nStartPage);
			m_btnPage[nCount]->SetButtonText( number ); //페이지 번호 저장
			m_btnPage[nCount]->ShowWindow( TRUE );
		}
		else
		{
			if( nCount > 4 ) //예외.
				break;

			m_btnPage[nCount]->ShowWindow( FALSE );
		}
	}


/*	int pagenumber = ((m_nCashItemTotalCount/9) ) - (5*m_Current5PageCount);
	if( (m_nCashItemTotalCount%9) != 0 )
	{
		++pagenumber; 
	}

	
	int buttonnumber = (5*m_Current5PageCount)+1;
	TCHAR number[8];
	sprintf(number,_T("%d"),buttonnumber);
	m_btnPage1->SetButtonText(number);
	++buttonnumber;
	memset( number, 0, sizeof(TCHAR)*8 );
	sprintf(number,_T("%d"),buttonnumber);
	m_btnPage2->SetButtonText(number);
	++buttonnumber;
	memset( number, 0, sizeof(TCHAR)*8 );
	sprintf(number,_T("%d"),buttonnumber);
	m_btnPage3->SetButtonText(number);
	++buttonnumber;
	memset( number, 0, sizeof(TCHAR)*8 );
	sprintf(number,_T("%d"),buttonnumber);
	m_btnPage4->SetButtonText(number);
	++buttonnumber;
	memset( number, 0, sizeof(TCHAR)*8 );
	sprintf(number,_T("%d"),buttonnumber);
	m_btnPage5->SetButtonText(number);

	if( pagenumber == 1 )
	{
		m_btnPage1->EnableWindow(TRUE);
		m_btnPage2->EnableWindow(FALSE);
		m_btnPage3->EnableWindow(FALSE);
		m_btnPage4->EnableWindow(FALSE);
		m_btnPage5->EnableWindow(FALSE);
	}
	else if( pagenumber == 2 )
	{
		m_btnPage1->EnableWindow(TRUE);
		m_btnPage2->EnableWindow(TRUE);
		m_btnPage3->EnableWindow(FALSE);
		m_btnPage4->EnableWindow(FALSE);
		m_btnPage5->EnableWindow(FALSE);
	}
	else if( pagenumber == 3 )
	{
		m_btnPage1->EnableWindow(TRUE);
		m_btnPage2->EnableWindow(TRUE);
		m_btnPage3->EnableWindow(TRUE);
		m_btnPage4->EnableWindow(FALSE);
		m_btnPage5->EnableWindow(FALSE);
	}
	else if( pagenumber == 4 )
	{
		m_btnPage1->EnableWindow(TRUE);
		m_btnPage2->EnableWindow(TRUE);
		m_btnPage3->EnableWindow(TRUE);
		m_btnPage4->EnableWindow(TRUE);
		m_btnPage5->EnableWindow(FALSE);
	}
	else if( pagenumber == 5 )
	{
		m_btnPage1->EnableWindow(TRUE);
		m_btnPage2->EnableWindow(TRUE);
		m_btnPage3->EnableWindow(TRUE);
		m_btnPage4->EnableWindow(TRUE);
		m_btnPage5->EnableWindow(TRUE);
	}
	else if( pagenumber > 5 )
	{
		m_btnPage1->EnableWindow(TRUE);
		m_btnPage2->EnableWindow(TRUE);
		m_btnPage3->EnableWindow(TRUE);
		m_btnPage4->EnableWindow(TRUE);
		m_btnPage5->EnableWindow(TRUE);
	}*/
}

void _XWindow_CashBox::SendPresentMessageReq( int MsgNumber)
{
	if( m_PresentMessageBoxPosition == (MsgNumber-1) )
	{
		if( m_bShowPresentMessage )
			m_bShowPresentMessage = FALSE;
		else
			m_bShowPresentMessage = TRUE;
	}
	else
	{
		m_bShowPresentMessage = TRUE;
		m_PresentMessageBoxPosition = MsgNumber-1;
	}
	if( m_bShowPresentMessage )
	{
		_XCASHITEMBOX_INFO* cashitem = NULL;
		list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
		for(iter_cashitem = m_listCashItem.begin() ; iter_cashitem != m_listCashItem.end() ; ++iter_cashitem)
		{
			cashitem = *iter_cashitem;
			if( cashitem )
			{
				if( cashitem->indexBox == MsgNumber )
				{
					m_SendOrderNumberPresentMessage = cashitem->OrderNumber;
					break;
				}
			}
		}
		m_btnMsgClose->MoveWindow(m_WindowPosition.x+ m_WindowSize.cx+188, m_WindowPosition.y+84+(34*m_PresentMessageBoxPosition));
		m_btnMsgClose->ShowWindow(TRUE);
		//패킷 보내기 	
		g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,20);//_CIS_PRESENT_MSG_REQ
	}
	else
	{
		m_btnMsgClose->ShowWindow(FALSE);
	}
}

#endif