// XWindow_CashItem.cpp: implementation of the _XWindow_CashItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindowObjectDefine.h"
#include "XWindow_CashItem.h"
#include <stdlib.h>
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern _XImageStatic		g_InventoryIconImage;


_XWindow_CashItem::_XWindow_CashItem()
{
	m_pBuyItem					= NULL;
	m_pPresentItem				= NULL;
	m_pCashItemListScrollBar	= NULL;
	m_CashItemListScrollPos		= 0;

	m_CashItemMode				= 0;
	srand((unsigned)time( NULL ));// 추후 삭제
}

_XWindow_CashItem::~_XWindow_CashItem()
{
	DestroyWindow();
}

BOOL _XWindow_CashItem::Initialize(void)
{
	// 바탕 이미지 
	int resourceimageindex = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );

	_XImageStatic* pMainBorder = new _XImageStatic;
	pMainBorder->Create( 0, 0, 425, 410, &g_MainInterfaceTextureArchive, resourceimageindex );
	pMainBorder->SetClipRect(0, 0, 425, 410);
	pMainBorder->ShowWindow(TRUE);
	InsertChildObject( pMainBorder );

	// Title바 이미지
	_XImageStatic* pTitleBorder = new _XImageStatic;
	pTitleBorder->Create( 20, 38, 401, 50, &g_MainInterfaceTextureArchive, resourceimageindex );
	pTitleBorder->SetClipRect(4, 461, 379, 486);
	pTitleBorder->ShowWindow(TRUE);
	InsertChildObject( pTitleBorder );

	// 버튼들 ------------------------------------------------------------------------------┓	
	_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, {23, 360}, {91, 27}, _XDEF_CASHITEM_BUYITEM_BTN, 
		resourceimageindex, resourceimageindex, &g_MainInterfaceTextureArchive };
	
	m_pBuyItem = new _XCheckButton;
	m_pBuyItem->Create( checkbtnstruct );
	m_pBuyItem->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_CASHITEM_2464 );
	m_pBuyItem->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 400, 425, 491, 452 );
	m_pBuyItem->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  400, 457, 491, 484 );
	m_pBuyItem->SetCheck( TRUE );	
	InsertChildObject( m_pBuyItem );

	checkbtnstruct.commandid++;
	checkbtnstruct.position.x += 99;

	m_pPresentItem = new _XCheckButton;
	m_pPresentItem->Create( checkbtnstruct );
	m_pPresentItem->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_CASHITEM_2465 );
	m_pPresentItem->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 400, 425, 491, 452 );
	m_pPresentItem->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  400, 457, 491, 484 );
	m_pPresentItem->SetCheck( FALSE );	
	InsertChildObject( m_pPresentItem );

	_XBTN_STRUCTURE btnstruct = { TRUE, {320, 360}, {91, 27}, _XDEF_CASHITEM_CLOSE_BTN, 
		resourceimageindex, resourceimageindex, resourceimageindex, &g_MainInterfaceTextureArchive };
	
	_XButton* pClose = new _XButton;
	pClose->Create(btnstruct);
	pClose->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_CLOSE );	
	pClose->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
	pClose->SetButtonImageClipRect(_XBUTTONIMAGE_READY,  400, 457, 491, 484);
	pClose->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK,  400, 457, 491, 484);
	InsertChildObject(pClose);

	resourceimageindex = g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName);

	_XBTN_STRUCTURE btnstruct2 = 
	{
		TRUE, {125, 326}, {88, 19}, _XDEF_CASHITEM_REFRESH_BTN,
		resourceimageindex,	resourceimageindex,	resourceimageindex,
		&g_MainInterfaceTextureArchive
	};

	_XButton* pRefresh = new _XButton;
	pRefresh->Create(btnstruct2);
	pRefresh->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
	pRefresh->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
	pRefresh->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
	pRefresh->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LOGINSERVER_2180);
	InsertChildObject(pRefresh);

	btnstruct2.commandid++;
	btnstruct2.position.x += 95;

	_XButton* pAllMove = new _XButton;
	pAllMove->Create(btnstruct2);
	pAllMove->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
	pAllMove->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
	pAllMove->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
	pAllMove->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_CASHITEM_2466);
	InsertChildObject(pAllMove);

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 373, 43 }, { 12, 12 }, _XDEF_CASHITEM_CLOSE_BTN,
									  charinfointerface,charinfointerface,charinfointerface, 
									  &g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
		
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );

	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 360, 43 }, { 12, 12 }, _XDEF_GAMBLE_HELP,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	// -------------------------------------------------------------------------------------┛
	
	// ScrollBar ---------------------------------------------------------------------------┓	
	resourceimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );

	SAFE_DELETE( m_pCashItemListScrollBar );
	m_pCashItemListScrollBar = new _XVScrollBar;
	_XSCRBAR_STRUCTURE cashitemlistscrbarstructure = 
	{
		TRUE, {m_WindowPosition.x+m_WindowSize.cx-54,m_WindowPosition.y+69 }, 
			12, 248, // width, height
			12,		 // btnysize
			22,		 // trackbar size
			_XDEF_CASHITEM_QUESIZE, 7, 0,
			resourceimageindex,
			resourceimageindex,
			resourceimageindex,
			resourceimageindex,
			resourceimageindex,
			&g_MainInterfaceTextureArchive
	};
	
	m_pCashItemListScrollBar->Create( cashitemlistscrbarstructure );
	m_pCashItemListScrollBar->ShowWindow( TRUE );
	m_pCashItemListScrollBar->SetViewBorder( TRUE );
	m_pCashItemListScrollBar->SetViewNullBorder( FALSE );
	m_pCashItemListScrollBar->SetTotalLineCount(0);	
	m_pCashItemListScrollBar->UpdateData();
	
	m_pCashItemListScrollBar->SetViewLineCount( 7 );	// 유료물품 : 7 선물 : 5 

	_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
	{
		{ 0,  0, 12, 12 },
		{ 24, 0, 36, 12 },
		{ 0,  12, 12, 24 },
		{ 24, 12, 36, 24 },
		{ 40,157, 52, 179}
	};
	m_pCashItemListScrollBar->SetClipRect( cliprectstructure );
	// -------------------------------------------------------------------------------------┛

	// 슬롯 영역 셋팅
	for( int i = 0; i < 7; i++ )
	{
		m_BuyItemSlotPosition[i].x = 0;
		m_BuyItemSlotPosition[i].y = 0;		
	}
	for( i = 0; i < 5; i++ )
	{
		m_PresentItemSlotPosition[i].x = 0;
		m_PresentItemSlotPosition[i].y = 0;
	}
	
	return TRUE;
}

void _XWindow_CashItem::DestroyWindow(void)
{
	if( !m_BuyItemList.empty() )
	{
		list<_XCASHITEM_BUY*>::iterator iter = m_BuyItemList.begin();
		while( iter != m_BuyItemList.end() )
		{
			_XCASHITEM_BUY* tempItem = *iter;
			if( tempItem )
			{
				SAFE_DELETE(tempItem);
				iter = m_BuyItemList.erase( iter );
			}
			else
			{
				iter++;
			}
		}
	}

	if( !m_PresentItemList.empty() )
	{
		list<_XCASHITEM_PRESENT*>::iterator iter = m_PresentItemList.begin();
		while( iter != m_PresentItemList.end() )
		{
			_XCASHITEM_PRESENT* tempItem = *iter;
			if( tempItem )
			{
				SAFE_DELETE(tempItem);
				iter = m_PresentItemList.erase( iter );
			}
			else
			{
				iter++;
			}
		}
	}

	SAFE_DELETE( m_pCashItemListScrollBar );
}

void _XWindow_CashItem::Draw(_XGUIObject*& pfocusobject )
{
	if(!this->m_ShowWindow)		return;

	_XWindow::Draw(pfocusobject);

	m_CashItemListScrollPos = m_pCashItemListScrollBar->GetLineScrollPos();
	m_pCashItemListScrollBar->Draw( pfocusobject );

	g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 44, _XGETINTERFACETEXT(ID_STRING_CASHITEM_2467) );	
	g_XBaseFont->Puts( m_WindowPosition.x + 31, m_WindowPosition.y + 44, _XGETINTERFACETEXT(ID_STRING_CASHITEM_2467) );	

	g_XBaseFont->SetColor( 0xff000000 );
	TCHAR tempStr[256];
	sprintf( tempStr, _XGETINTERFACETEXT(ID_STRING_CASHITEM_2468), m_BuyItemList.size() );
	g_XBaseFont->PrintAlign(m_WindowPosition.x+m_WindowSize.cx-30, m_WindowPosition.y+326, 1.0f, _XFONT_ALIGNTYPE_RIGHT, tempStr );

	sprintf( tempStr, _XGETINTERFACETEXT(ID_STRING_CASHITEM_2469), m_PresentItemList.size() );
	g_XBaseFont->PrintAlign(m_WindowPosition.x+m_WindowSize.cx-30, m_WindowPosition.y+341, 1.0f, _XFONT_ALIGNTYPE_RIGHT, tempStr );

	if( m_CashItemMode == _XDEF_CASHITEM_BUYITEM_BTN )
	{
		DrawBuyItemList();
	}
	else if( m_CashItemMode == _XDEF_CASHITEM_PRESENTITEM_BTN )
	{
		DrawPresentItemList();
	}

	// 설명 보여주기 ----------------------┓	
	int curindex = SearchSlotRegion();
	if( curindex != -1 )
	{
		ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();

		g_InterfaceTooltip.SetToolTipString( scrnpos->x+16, scrnpos->z-16, 
			_XGETINTERFACETEXT(ID_STRING_CASHITEM_2470), _XSC_INFORMATION, TRUE);
	}
	// ------------------------------------┛

	g_XBaseFont->Flush();
}

BOOL _XWindow_CashItem::Process(_XGUIObject*& pfocusobject )
{
	if(!this->m_ShowWindow)		return FALSE;	
	if(!this->m_Enable)		return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[1])
			{
				// mouse right down
				if(CheckMousePosition())
					m_WindowMouseState = _XW_STATE_RIGHTCLICK;
			}
			if(mousestate->bButton[0])
			{
				// mouse left down
				if(CheckMousePosition())
				{
					// Focus
					if(scrnpos->x > m_WindowPosition.x+28 && scrnpos->x < m_WindowPosition.x+59)
					{
						if(scrnpos->z > m_WindowPosition.y+68 && scrnpos->z < m_WindowPosition.y+315)
						{
							pfocusobject = _XDEF_DUMMYID_CASHITEM;
						}
					}
					
					if(g_pInventory_Window )
					{
						if( g_pInventory_Window->GetShowStatus())
						{
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}
						}
					}
				} // if(CheckMousePosition())
				else
				{
					// Tooltip show false
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
						{
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
				}
			}
		}
		if(pfocusobject == _XDEF_DUMMYID_CASHITEM)
		{
			if(!mousestate->bButton[0])
			{
				int curslot = SearchSlotRegion() + m_CashItemListScrollPos;
				if( curslot != -1 )
				{
					// 제자리에서 클릭 - 툴팁 보여주기
					if( m_CashItemMode == _XDEF_CASHITEM_BUYITEM_BTN )
					{
						list<_XCASHITEM_BUY*>::iterator k = m_BuyItemList.begin();
						if( curslot < m_BuyItemList.size() )
							advance( k, curslot );

						_XCASHITEM_BUY* tempBuyItem = *k;
						if( tempBuyItem )
						{
							if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_CASHITEM, 0, tempBuyItem->Item.Get_m_cType(), tempBuyItem->Item.Get_m_sID()) )
							{
								m_SelectedItem = &tempBuyItem->Item;
								g_pDefaultTooltip_Window->ShowWindow( TRUE );
							}							
						}
					}
					else if( m_CashItemMode == _XDEF_CASHITEM_PRESENTITEM_BTN )
					{
						list<_XCASHITEM_PRESENT*>::iterator k = m_PresentItemList.begin();
						if( curslot < m_PresentItemList.size() )
							advance( k, curslot );

						_XCASHITEM_PRESENT* tempPresentItem = *k;
						if( tempPresentItem )
						{
							if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_CASHITEM, 0, tempPresentItem->Item.Get_m_cType(), tempPresentItem->Item.Get_m_sID()) )
							{
								m_SelectedItem = &tempPresentItem->Item;
								g_pDefaultTooltip_Window->ShowWindow( TRUE );
							}							
						}						
					}
				}							
				else
				{
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
					}
				}
			} //if(!mousestate->bButton[0])
		}

		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				
			}
		}
		
		if(!mousestate->bButton[0])
		{
			if(pfocusobject == _XDEF_DUMMYID_CASHITEM)
				pfocusobject = NULL;
		}
	}

	

	m_pCashItemListScrollBar->Process( pfocusobject );
	if(!_XWindow::Process(pfocusobject)) return FALSE;	

	return TRUE;
}

BOOL _XWindow_CashItem::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( m_pCashItemListScrollBar )
	{
		if( m_pCashItemListScrollBar->ProcessWheelMessage( zDelta ) )
		{
			return TRUE;
		}
		
		if( CheckMousePosition() )
		{
			int MoveDelta  = (FLOAT)zDelta / 40;			
			
			if( zDelta > 0 )
			{
				m_pCashItemListScrollBar->LineUp( abs(MoveDelta) );
			}
			else
			{
				m_pCashItemListScrollBar->LineDown( abs(MoveDelta) );
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

void _XWindow_CashItem::DrawBuyItemList(void)
{
#ifdef _XTS_CASHITEM
	// Draw Line
	for( int i = 0; i < 6; i++ )
	{
		FLOAT px1 = m_WindowPosition.x+66;
		FLOAT py1 = m_WindowPosition.y+102+(i*37);
		FLOAT px2 = m_WindowPosition.x+365;
		FLOAT py2 = m_WindowPosition.y+102+(i*37);

		_XDrawLine2D( px1, py1, px2, py2, 0xffACA485, 0xffACA485 );
	}

	list<_XCASHITEM_BUY*>::iterator k = m_BuyItemList.begin();
	if( m_CashItemListScrollPos < m_BuyItemList.size() )
		advance( k , m_CashItemListScrollPos );

	for( i = 0; i < 7; i++ )
	{
		if( k == m_BuyItemList.end() ) break;
		
		_XCASHITEM_BUY* tempItem = *k;
		if( tempItem )
		{
			// 아이콘 그리기 --------------------------------------------------------------------------┓			
			g_pInventory_Window->DrawIcon(FALSE, m_WindowPosition.x+28, m_WindowPosition.y+68+(i*37), &tempItem->Item);
			// ----------------------------------------------------------------------------------------┛

			// Item Info ------------------------------------------------------------------------------┓			
			g_XBaseFont->SetColor( 0xff000000 );

			TCHAR tempStr[64];
			sprintf( tempStr, "%s %s", _XGameItem::GetItemNick(tempItem->Item.m_cType, tempItem->Item.m_sID),
				_XGameItem::GetItemName(tempItem->Item.m_cType, tempItem->Item.m_sID) );
			g_XBaseFont->PrintAlign(m_WindowPosition.x+67, m_WindowPosition.y+78+(i*37), 1.0f, _XFONT_ALIGNTYPE_LEFT, tempStr );

			g_XSmallFont->SetColor( 0xff726A68 );
			sprintf( tempStr, "0%d.%d/%d", tempItem->Item.m_ucYear, tempItem->Item.m_ucMonth, tempItem->Item.m_ucDay );
			g_XSmallFont->PrintAlign(m_WindowPosition.x+325, m_WindowPosition.y+71+(i*37), 1.0f, _XFONT_ALIGNTYPE_LEFT, tempStr );
			sprintf( tempStr, "%d", tempItem->usCash );
			g_XSmallFont->PrintAlign(m_WindowPosition.x+325, m_WindowPosition.y+85+(i*37), 1.0f, _XFONT_ALIGNTYPE_LEFT, tempStr );
			g_XSmallFont->Flush();
			// ----------------------------------------------------------------------------------------┛
			k++;
		}
	}
#endif
}

void _XWindow_CashItem::DrawPresentItemList(void)
{
#ifdef _XTS_CASHITEM
	// Draw Line
	for( int i = 0; i < 4; i++ )
	{
		FLOAT px1 = m_WindowPosition.x+66;
		FLOAT py1 = m_WindowPosition.y+113+(i*53);
		FLOAT px2 = m_WindowPosition.x+365;
		FLOAT py2 = m_WindowPosition.y+113+(i*53);

		_XDrawLine2D( px1, py1, px2, py2, 0xffACA485, 0xffACA485 );
	}	

	list<_XCASHITEM_PRESENT*>::iterator k = m_PresentItemList.begin();
	if( m_CashItemListScrollPos < m_PresentItemList.size() )
		advance( k , m_CashItemListScrollPos );

	for( i = 0; i < 5; i++ )
	{
		if( k == m_PresentItemList.end() )	break;
		
		_XCASHITEM_PRESENT* tempItem = *k;
		if( tempItem )
		{
			// 아이콘 그리기 --------------------------------------------------------------------------┓			
			g_pInventory_Window->DrawIcon(FALSE, m_WindowPosition.x+28, m_WindowPosition.y+68+(i*53), &tempItem->Item);
			// ----------------------------------------------------------------------------------------┛
			// Item Info ------------------------------------------------------------------------------┓			
			g_XBaseFont->SetColor( 0xff000000 );

			TCHAR tempStr[64];
			sprintf( tempStr, "%s %s", _XGameItem::GetItemNick(tempItem->Item.m_cType, tempItem->Item.m_sID),
				_XGameItem::GetItemName(tempItem->Item.m_cType, tempItem->Item.m_sID) );
			g_XBaseFont->PrintAlign(m_WindowPosition.x+67, m_WindowPosition.y+68+(i*53), 1.0f, _XFONT_ALIGNTYPE_LEFT, tempStr );

			g_XSmallFont->SetColor( 0xff726A68 );
			sprintf( tempStr, "0%d.%d/%d", tempItem->Item.m_ucYear, tempItem->Item.m_ucMonth, tempItem->Item.m_ucDay );
			g_XSmallFont->PrintAlign(m_WindowPosition.x+325, m_WindowPosition.y+71+(i*53), 1.0f, _XFONT_ALIGNTYPE_LEFT, tempStr );

			g_XBaseFont->SetColor( 0xff249414 );			
			sprintf( tempStr, _XGETINTERFACETEXT(ID_STRING_CASHITEM_2471), tempItem->FromUserName );
			g_XBaseFont->PrintAlign(m_WindowPosition.x+67, m_WindowPosition.y+83+(i*53), 1.0f, _XFONT_ALIGNTYPE_LEFT, tempStr );

			g_XBaseFont->SetColor( 0xff000000 );
			g_XBaseFont->PrintAlign(m_WindowPosition.x+67, m_WindowPosition.y+98+(i*53), 1.0f, _XFONT_ALIGNTYPE_LEFT, tempItem->FromMessage );
			g_XSmallFont->Flush();
			// ----------------------------------------------------------------------------------------┛
			k++;
		}
	}
#endif
}

void _XWindow_CashItem::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow(X, Y);
	m_pCashItemListScrollBar->MoveWindow( m_WindowPosition.x+m_WindowSize.cx-54,m_WindowPosition.y+69 );
}

void _XWindow_CashItem::ShowWindow(BOOL show)
{
	if( show )
	{
		if( g_pDefaultTooltip_Window)
			g_pDefaultTooltip_Window->SetCashItemWindowPtr( this );
	}
	else
	{
		if( g_pDefaultTooltip_Window )
		{
			if( g_pDefaultTooltip_Window->GetShowStatus() )						
				g_pDefaultTooltip_Window->ShowWindow( FALSE );
		}
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_CashItem::SetMode(int mode)
{
	m_CashItemMode = mode;
	switch(m_CashItemMode) 
	{
	case _XDEF_CASHITEM_BUYITEM_BTN :
		{
			m_pBuyItem->SetCheck(TRUE);
			m_pPresentItem->SetCheck(FALSE);
			BuyItemReload();
		}
		break;
	case _XDEF_CASHITEM_PRESENTITEM_BTN :
		{
			m_pBuyItem->SetCheck(FALSE);
			m_pPresentItem->SetCheck(TRUE);
			PresentItemReload();
		}
		break;
	}
}

void _XWindow_CashItem::BuyItemReload(void)
{
#ifdef _XTS_CASHITEM
	// reset list
	if( !m_BuyItemList.empty() )
	{
		list<_XCASHITEM_BUY*>::iterator iter = m_BuyItemList.begin();
		while( iter != m_BuyItemList.end() )
		{
			_XCASHITEM_BUY* tempItem = *iter;
			if( tempItem )
			{
				SAFE_DELETE(tempItem);
				iter = m_BuyItemList.erase( iter );
			}
			else
			{
				iter++;
			}
		}
	}

	// Temp Data		
	for( int i = 0; i < 20; i++ )
	{		
		_XCASHITEM_BUY* tempItem = new _XCASHITEM_BUY;
		tempItem->Reset();
		
		tempItem->Item.m_cType		= (rand() % 2) == 1 ? 1 : 3;		
		if( tempItem->Item.m_cType == 1 )
		{
			tempItem->Item.m_sID		= _XRand(25L)+1;
			tempItem->Item.m_ucCount	= 1;
		}
		else 
		{
			tempItem->Item.m_sID		= rand() % 6+1;
			tempItem->Item.m_ucCount	= rand() % 255+1;
		}
		tempItem->Item.m_ucYear		= 5;
		tempItem->Item.m_ucMonth	= rand() % 12+1;
		tempItem->Item.m_ucDay		= rand() % 30+1;
		tempItem->usCash			= rand() % 30000;

		m_BuyItemList.push_back(tempItem);		
	}
	m_pCashItemListScrollBar->SetViewLineCount(7);
	m_pCashItemListScrollBar->SetTotalLineCount(i);
	m_pCashItemListScrollBar->UpdateData();
	
	m_pCashItemListScrollBar->SetLineScrollPos(0);

	// slot setting
	if( i > 7 )
		i = 7;
	for( int j = 0; j < i; j++ )
	{
		m_BuyItemSlotPosition[j].x = 28;
		m_BuyItemSlotPosition[j].y = 68+(j*37);
	}
#endif
}

void _XWindow_CashItem::PresentItemReload(void)
{
#ifdef _XTS_CASHITEM
	// reset list
	if( !m_PresentItemList.empty() )
	{
		list<_XCASHITEM_PRESENT*>::iterator iter = m_PresentItemList.begin();
		while( iter != m_PresentItemList.end() )
		{
			_XCASHITEM_PRESENT* tempItem = *iter;
			if( tempItem )
			{
				SAFE_DELETE(tempItem);
				iter = m_PresentItemList.erase( iter );
			}
			else
			{
				iter++;
			}
		}
	}

	// Temp Data
	for( int i = 0; i < 4; i++ )
	{		
		_XCASHITEM_PRESENT* tempItem = new _XCASHITEM_PRESENT;
		tempItem->Reset();
		
		tempItem->Item.m_cType		= (rand() % 2) == 1 ? 1 : 3;		
		if( tempItem->Item.m_cType == 1 )
		{
			tempItem->Item.m_sID		= _XRand(25L)+1;
			tempItem->Item.m_ucCount	= 1;			
		}
		else 
		{
			tempItem->Item.m_sID		= rand() % 6+1;
			tempItem->Item.m_ucCount	= rand() % 255+1;
		}

		tempItem->Item.m_ucYear		= 5;
		tempItem->Item.m_ucMonth	= _XRand(12L)+1;
		tempItem->Item.m_ucDay		= _XRand(30L)+1;

		strcpy( tempItem->FromUserName, "^^" );
		strcpy( tempItem->FromMessage, "--;" );

		m_PresentItemList.push_back(tempItem);
	}

	m_pCashItemListScrollBar->SetViewLineCount(5);
	m_pCashItemListScrollBar->SetTotalLineCount(i);
	m_pCashItemListScrollBar->UpdateData();
	
	m_pCashItemListScrollBar->SetLineScrollPos(0);

	// slot setting
	if( i > 5 )
		i = 5;
	for( int j = 0; j < i; j++ )
	{
		m_PresentItemSlotPosition[j].x = 28;
		m_PresentItemSlotPosition[j].y = 68+(j*53);
	}
#endif
}

void _XWindow_CashItem::AllMoveItem(void)
{

}

int _XWindow_CashItem::SearchSlotRegion(RECT* pRect)
{
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	int count = 0;
	switch(m_CashItemMode) 
	{
	case _XDEF_CASHITEM_BUYITEM_BTN :
		{
			count = m_BuyItemList.size();
			if( count > 7 )
				count = 7;
			for(int i = 0 ; i < count ; i++)
			{
				if( scrnpos->x > m_WindowPosition.x+m_BuyItemSlotPosition[i].x && 
					scrnpos->x < m_WindowPosition.x+m_BuyItemSlotPosition[i].x+32 &&
					scrnpos->z > m_WindowPosition.y+m_BuyItemSlotPosition[i].y &&
					scrnpos->z < m_WindowPosition.y+m_BuyItemSlotPosition[i].y+32 )
				{
					if( pRect )
					{
						pRect->left   = m_WindowPosition.x+m_BuyItemSlotPosition[i].x-1;
						pRect->top    = m_WindowPosition.y+m_BuyItemSlotPosition[i].y-1;
						pRect->right  = m_WindowPosition.x+m_BuyItemSlotPosition[i].x + 29;
						pRect->bottom = m_WindowPosition.y+m_BuyItemSlotPosition[i].y + 29;
					}

					return i;					
				}
			}
		}
		break;
	case _XDEF_CASHITEM_PRESENTITEM_BTN :
		{
			count = m_PresentItemList.size();
			if( count > 5 )
				count = 5;
			for(int i = 0 ; i < count ; i++)
			{
				if( scrnpos->x > m_WindowPosition.x+m_PresentItemSlotPosition[i].x && 
					scrnpos->x < m_WindowPosition.x+m_PresentItemSlotPosition[i].x+32 &&
					scrnpos->z > m_WindowPosition.y+m_PresentItemSlotPosition[i].y && 
					scrnpos->z < m_WindowPosition.y+m_PresentItemSlotPosition[i].y+32 )
				{
					if( pRect )
					{
						pRect->left   = m_WindowPosition.x+m_PresentItemSlotPosition[i].x-1;
						pRect->top    = m_WindowPosition.y+m_PresentItemSlotPosition[i].y-1;
						pRect->right  = m_WindowPosition.x+m_PresentItemSlotPosition[i].x + 29;
						pRect->bottom = m_WindowPosition.y+m_PresentItemSlotPosition[i].y + 29;
					}

					return i;
				}
			}
		}
		break;
	}
	
	return -1;
}