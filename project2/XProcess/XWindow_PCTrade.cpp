// XWindow_PCTrade.cpp: implementation of the _XWindow_PCTrade class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_PCTrade.h"
#include "XWindow_TradeConfirm.h"
#include "XMessageWindow.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern unsigned long g_MoneyColor[6];

_XWindow_PCTrade::_XWindow_PCTrade()
{
	m_WaitIcon				= NULL;
	m_ConfirmIcon			= NULL;
	m_TargetWaitIcon		= NULL;
	m_TargetConfirmIcon		= NULL;

	m_OKButton				= NULL;
	m_CancelButton			= NULL;
	
	m_WindowMouseState		= _XW_STATE_NONE;
	m_bTrading				= FALSE;

	ResetTradeInfo();
}

_XWindow_PCTrade::~_XWindow_PCTrade()
{
	
}


BOOL _XWindow_PCTrade::Initialize( void )
{	
	// Create window sub border ---------------------------------------------------------------------------------------
	int pctradeimgindex = g_MainInterfaceTextureArchive.FindResource( "MI_Trade.tga" );
	// 상단 라인
	_XImageStatic* pUpLineBar = new _XImageStatic;
	pUpLineBar->Create( 0, 120, 183, 124, &g_MainInterfaceTextureArchive, pctradeimgindex );	
	pUpLineBar->SetClipRect( 0, 146, 183, 150 );
	InsertChildObject( pUpLineBar );	
	// 하단 라인
	_XImageStatic* pDownLineBar = new _XImageStatic;
	pDownLineBar->Create( 0, 267, 183, 269, &g_MainInterfaceTextureArchive, pctradeimgindex );	
	pDownLineBar->SetClipRect( 0, 146, 183, 148 );
	InsertChildObject( pDownLineBar );	
	// 상단
	_XImageStatic* pUpTitleBar = new _XImageStatic;
	pUpTitleBar->Create( 0, 0, 183, 120, &g_MainInterfaceTextureArchive, pctradeimgindex );	
	pUpTitleBar->SetClipRect( 0, 0, 183, 121 );
	InsertChildObject( pUpTitleBar );	
	// 하단 1
	_XImageStatic* pDownTitleBar = new _XImageStatic;
	pDownTitleBar->Create( 0, 123, 183, 243, &g_MainInterfaceTextureArchive, pctradeimgindex );	
	pDownTitleBar->SetClipRect( 0, 0, 183, 121 );
	InsertChildObject( pDownTitleBar );
	// 하단 2
	_XImageStatic* pDown2TitleBar = new _XImageStatic;
	pDown2TitleBar->Create( 0, 242, 183, 267, &g_MainInterfaceTextureArchive, pctradeimgindex );	
	pDown2TitleBar->SetClipRect( 0, 120, 183, 145 );
	InsertChildObject( pDown2TitleBar );
	
	//------------------------------------------------------------------------------------------------------------------------=
	// imsi
	m_TargetWaitIcon = new _XImageStatic;
	m_TargetWaitIcon->Create(9, 4, 18, 13,
						&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("icon_deathblow.tga"));
	m_TargetWaitIcon->SetClipRect(0, 0, 10, 10);
	m_TargetWaitIcon->ShowWindow(TRUE);
	InsertChildObject(m_TargetWaitIcon);

	m_TargetConfirmIcon = new _XImageStatic;
	m_TargetConfirmIcon->Create(9, 4, 18, 13,
							&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("icon_deathblow.tga"));
	m_TargetConfirmIcon->SetClipRect(10, 0, 20, 10);
	m_TargetConfirmIcon->ShowWindow(FALSE);
	InsertChildObject(m_TargetConfirmIcon);

	m_WaitIcon = new _XImageStatic;
	m_WaitIcon->Create(9, 126, 18, 135,
						&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("icon_deathblow.tga"));
	m_WaitIcon->SetClipRect(0, 0, 10, 10);
	m_WaitIcon->ShowWindow(TRUE);
	InsertChildObject(m_WaitIcon);
	
	m_ConfirmIcon = new _XImageStatic;
	m_ConfirmIcon->Create(9, 126, 18, 135,
							&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("icon_deathblow.tga"));
	m_ConfirmIcon->SetClipRect(10, 0, 20, 10);
	m_ConfirmIcon->ShowWindow(FALSE);
	InsertChildObject(m_ConfirmIcon);
	//------------------------------------------------------------------------------------------------------------------------=
	int moneyimgindex = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	// 금액창
	_XImageStatic* pTargetEditImg = new _XImageStatic;	
	pTargetEditImg->Create( 9, 97, 105, 114, &g_MainInterfaceTextureArchive, moneyimgindex );
	pTargetEditImg->SetClipRect(7, 201, 104, 219);
	InsertChildObject( pTargetEditImg );

	_XImageStatic* pSelfEditImg = new _XImageStatic;	
	pSelfEditImg->Create( 9, 220, 105, 237, &g_MainInterfaceTextureArchive, moneyimgindex );
	pSelfEditImg->SetClipRect(7, 201, 104, 219);
	InsertChildObject( pSelfEditImg );
	
	// 금
	_XImageStatic* pTargetMoneyImg = new _XImageStatic;	
	pTargetMoneyImg->Create( 16, 101, 28, 110, &g_MainInterfaceTextureArchive, moneyimgindex );
	pTargetMoneyImg->SetClipRect(184, 6, 197, 16);
	InsertChildObject( pTargetMoneyImg );

	// 소지금액 버튼
	_XBTN_STRUCTURE moneybtnstruct = 
	{
		TRUE, {16, 224}, {13, 10},
			_XDEF_PCTRADEWINDOW_MONEY,
			moneyimgindex,
			moneyimgindex,
			moneyimgindex,
			&g_MainInterfaceTextureArchive
	};
	_XButton* p_SelfMoney = new _XButton;
	p_SelfMoney->Create(moneybtnstruct);
	p_SelfMoney->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 184, 6, 197, 16);
	p_SelfMoney->SetButtonImageClipRect(_XBUTTONIMAGE_READY,  184, 6, 197, 16);
	p_SelfMoney->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK,  184, 6, 197, 16);
	p_SelfMoney->EnableWindow(TRUE);
	InsertChildObject(p_SelfMoney);

	// Create minimize button -----------------------------------------------------------------------------------------
	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_PCTRADEWINDOW_CLOSE,
									  charinfointerface,charinfointerface,charinfointerface, 
									  &g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
		
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );

	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 153, 2 }, { 12, 12 }, _XDEF_PCTRADEWINDOW_HELP,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {70, 242}, {52, 22},
		_XDEF_PCTRADEWINDOW_OK,
		pctradeimgindex, pctradeimgindex, pctradeimgindex,
		&g_MainInterfaceTextureArchive
	};
	m_OKButton = new _XButton;
	m_OKButton->Create(btnstruct);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 3, 240, 24);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 28, 240, 49);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 53, 240, 74);
	m_OKButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));	
	m_OKButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_OK);
	InsertChildObject(m_OKButton);

	btnstruct.position.x = 124;
	btnstruct.position.y = 242;
	btnstruct.commandid = _XDEF_PCTRADEWINDOW_CANCEL;

	m_CancelButton = new _XButton;
	m_CancelButton->Create(btnstruct);
	m_CancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 3, 240, 24);
	m_CancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 28, 240, 49);
	m_CancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 53, 240, 74);	
	m_CancelButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_CancelButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_CANCEL);
	InsertChildObject(m_CancelButton);

	_XTOOLTIP_STRUCTURE itemtooltipstruct = 
	{
		{ 0, 0 },
		{ 78, 13 },
		{ 1, 1 },
		-1,
		NULL
	};	

	for(int i = 0 ; i < _XDEF_PCTRADE_MAX_SLOTNUMBER ; i++)
	{
		m_TargetSlotPosition[i].x = 10 + ((i%5)*33);
		m_TargetSlotPosition[i].y = 27 + ((i/5)*33);
	}
	for(i = 0 ; i < _XDEF_PCTRADE_MAX_SLOTNUMBER ; i++)
	{
		m_SelfSlotPosition[i].x = 10 + ((i%5)*33);
		m_SelfSlotPosition[i].y = 150 + ((i/5)*33);
	}
	
	return TRUE;
}

void _XWindow_PCTrade::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_PCTrade::SaveWindowProperty( void )
{
	
}


void _XWindow_PCTrade::ResetTradeInfo(void)
{
	m_bRequest				= FALSE;
	m_TradeServerSlotNo		= 0;
	m_TargetID				= 0;
	
	m_TargetMoney			= 0;
	m_SelfMoney				= 0;
	
	m_bWaitTrade			= FALSE;
	
	m_MouseSlotItem.ResetMouseSlotItem();
	m_bDragState			= FALSE;
	m_bHaveMouseSlot		= FALSE;
	m_bIsTarget				= FALSE;
	
	m_IconDragStartPos.x = m_IconDragStartPos.y = 0;
	m_LastDraggingSlot		= -1;
	
	m_bDrawSelectedItem		 = FALSE;
	m_bTrading				 = FALSE;
	
	if(m_OKButton)
		m_OKButton->EnableWindow(TRUE);
	
	for( int i = 0; i < _XDEF_PCTRADE_MAX_SLOTNUMBER; i++ )
	{
		m_TargetTradeItem[i].ResetTradeItem();
		m_SelfTradeItem[i].ResetTradeItem();
	}
	
	memset(m_TargetName, 0, sizeof(TCHAR)*_XDEF_MAX_USERNAMESTRINGLENGTH);
}

void _XWindow_PCTrade::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw(pfocusobject);
	
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	//_XDrawRectAngle(m_WindowPosition.x, m_WindowPosition.y + 15, 
	//				m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));

	/*
	for(int row = 0 ; row < 2 ; row++)
		{
			for(int column = 0 ; column < 5 ; column++)
			{
				_XDrawRectAngle(m_WindowPosition.x+23+(33*column), m_WindowPosition.y+49+(33*row), 
								m_WindowPosition.x+54+(33*column), m_WindowPosition.y+80+(33*row), 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
				_XDrawRectAngle(m_WindowPosition.x+23+(33*column), m_WindowPosition.y+196+(33*row),
								m_WindowPosition.x+54+(33*column), m_WindowPosition.y+227+(33*row), 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
			}
		}*/
	

	for(int i = 0 ; i < _XDEF_PCTRADE_MAX_SLOTNUMBER ; i ++)
	{
		// Target Trade Icon draw
		if( m_TargetTradeItem[i].Get_m_sID() > 0 )
			g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+m_TargetSlotPosition[i].x, m_WindowPosition.y+m_TargetSlotPosition[i].y, &m_TargetTradeItem[i]);

		// Self Trade Icon draw
		if( m_SelfTradeItem[i].Get_m_sID() > 0 )
			g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+m_SelfSlotPosition[i].x, m_WindowPosition.y+m_SelfSlotPosition[i].y, &m_SelfTradeItem[i]);
	}

	DrawSelectedItem();


	g_XBaseFont->SetColor(_XSC_DEFAULT);

	// 이름
	g_XBaseFont->PrintAlign(m_WindowPosition.x + 22, m_WindowPosition.y + 4, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%s", m_TargetName);
	g_XBaseFont->PrintAlign(m_WindowPosition.x + 22, m_WindowPosition.y + 126, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%s", g_pLocalUser->m_CharacterInfo.charactername);

	// 소지 금액
	TCHAR moneyStr[128];
	memset( moneyStr, 0, sizeof(moneyStr));
	sprintf(moneyStr, "%u", m_TargetMoney);
//	itoa(m_TargetMoney, moneyStr, 10 );
	int colorindex = 0;

	if( _XGameItem::GetMoneyColor(moneyStr, colorindex) )
	{
		g_XBaseFont->SetColor( g_MoneyColor[colorindex] );
		g_XBaseFont->PrintAlign(m_WindowPosition.x+100, m_WindowPosition.y+101, 1.0f, _XFONT_ALIGNTYPE_RIGHT, moneyStr );
	}
	else
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->PrintAlign(m_WindowPosition.x + 100, m_WindowPosition.y + 101, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%u", m_TargetMoney);
	}
	
	memset( moneyStr, 0, sizeof(moneyStr));
	sprintf(moneyStr, "%u", m_SelfMoney);
//	itoa(m_SelfMoney, moneyStr, 10 );
	colorindex = 0;
	if( _XGameItem::GetMoneyColor(moneyStr, colorindex) )
	{
		g_XBaseFont->SetColor( g_MoneyColor[colorindex] );
		g_XBaseFont->PrintAlign(m_WindowPosition.x+100, m_WindowPosition.y+224, 1.0f, _XFONT_ALIGNTYPE_RIGHT, moneyStr );
	}
	else
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->PrintAlign(m_WindowPosition.x + 100, m_WindowPosition.y + 224, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%u", m_SelfMoney);
	}	
	
	g_XBaseFont->Flush();
}

BOOL _XWindow_PCTrade::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;

	BOOL	bInTrade = FALSE;
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();

	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[1])
			{
				// right down
				if(CheckMousePosition())
					m_WindowMouseState = _XW_STATE_RIGHTCLICK;
				else
				{
					if(m_bHaveMouseSlot)
					{
						m_bHaveMouseSlot = FALSE;
						m_MouseSlotItem.ResetMouseSlotItem();
					}
				}
			}
			if(mousestate->bButton[0])
			{
				// left down
				if(CheckMousePosition())
				{
					// Focus
					if(scrnpos->x > m_WindowPosition.x+10 && scrnpos->z < m_WindowPosition.x+173)
					{
						if(scrnpos->z > m_WindowPosition.y+26 && scrnpos->z < m_WindowPosition.y+214)
						{
							pfocusobject = _XDEF_DUMMYID_PCTRADE;
						}
					}

					if(g_pInventory_Window)
					{
						if(g_pInventory_Window->GetShowStatus())
						{
							if(g_pInventory_Window->m_bHaveMouseSlot)
							{
								m_LastDraggingSlot = SearchEmptySlotIndex();
								if(m_LastDraggingSlot >= 10)
								{
									m_LastDraggingSlot -= 10;
									if(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)	//장착하고 있는 것은 불가능
									{
										if( _XGameItem::CheckUserTrade( g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_cType(), 
											g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_sID() ) )
										{				
											_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
											pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2123), TRUE);
											g_pInventory_Window->m_bHaveMouseSlot = FALSE;
											g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
											return FALSE;
										}

										if(g_pInventory_Window->m_MouseSlotItem.IsStackable())
										{
											// stackable 처리
											_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
											if(pTradeConfirm_Window)
											{
												pTradeConfirm_Window->ShowWindow(TRUE);									
												pTradeConfirm_Window->m_IMEControl.SetFocus();
												pTradeConfirm_Window->m_SelectedCategory1 = g_pInventory_Window->m_MouseSlotItem.Get_m_cType();
												pTradeConfirm_Window->m_SelectedItemID = g_pInventory_Window->m_MouseSlotItem.Get_m_sID();
												m_SelfTradeItem[m_LastDraggingSlot].m_InventoryNumber = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
												pTradeConfirm_Window->m_PCTradeSlotIndex = m_LastDraggingSlot;
												pTradeConfirm_Window->m_TradeMode = _XPCTRADE_TOTRADE_ITEM;
											}
										}
										else
										{
											CopyItemToSlot(m_LastDraggingSlot, g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber);
											m_SelfTradeItem[m_LastDraggingSlot].m_InventoryNumber = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
											g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Reset();
											g_pInventory_Window->m_bHaveMouseSlot = FALSE;
											g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
											g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_ADD, m_LastDraggingSlot, 1);
										}
										bInTrade = TRUE;
									} // if(g_pInventory_Window->m_MouseSlotItem.inventorynumber >= 16)
								}
							} // if(g_pInventory_Window->m_bHaveMouseSlot)
						}
					} // if(g_pInventory_Window)

					if(!bInTrade)
					{
						if(!m_bHaveMouseSlot)
						{
							// Mouse down 위치 저장
							m_IconDragStartPos.x = scrnpos->x;
							m_IconDragStartPos.y = scrnpos->z;
							m_LastDraggingSlot = SearchFullSlotIndex();
							m_bDragState = TRUE;
						}
						else
						{
							m_MouseSlotItem.ResetMouseSlotItem();
							m_bHaveMouseSlot = FALSE;
						}
					}

					if(g_pInventory_Window )
					{
						if( g_pInventory_Window->GetShowStatus())
						{
							g_pInventory_Window->m_bDrawSelectedItem = FALSE;						
							if( g_pDefaultTooltip_Window && g_pDefaultTooltip_Window->GetShowStatus() )
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
				} // if(CheckMousePosition())
				else
				{
					if( g_pInventory_Window )
					{
						if( g_pInventory_Window->GetShowStatus() && !g_pInventory_Window->CheckMousePosition() )
						{
							if(m_bHaveMouseSlot)
							{
								m_MouseSlotItem.ResetMouseSlotItem();
								m_bHaveMouseSlot = FALSE;
								m_LastDraggingSlot = -1;
							}
						}	
					}
				}
			}
		} // if(!pfocusobject)

		if(pfocusobject == _XDEF_DUMMYID_PCTRADE)
		{
			if(mousestate->bButton[0])
			{
				// 드래그 처리
				if(m_bDragState && !m_bHaveMouseSlot)
				{
					if(scrnpos->x != m_IconDragStartPos.x || scrnpos->z != m_IconDragStartPos.y)
					{
						if(m_LastDraggingSlot >= 10)
						{
							m_LastDraggingSlot -= 10;
							m_MouseSlotItem.Copy(m_SelfTradeItem[m_LastDraggingSlot]);
							m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
							m_bIsTarget = FALSE;
							m_bHaveMouseSlot = TRUE;
							m_LastDraggingSlot = -1;
						}
					}
				} // if(m_bDragState && !m_bHaveMouseSlot)
			}

			if(!mousestate->bButton[0])
			{
				m_bDrawSelectedItem = TRUE;

				if(m_bDragState)
				{
					if(scrnpos->x == m_IconDragStartPos.x && scrnpos->z == m_IconDragStartPos.y)
					{
						// 제자리에서 클릭
						if(m_LastDraggingSlot >= 10)
						{
							m_LastDraggingSlot -= 10;
							m_MouseSlotItem.Copy(m_SelfTradeItem[m_LastDraggingSlot]);
							m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;

							if( g_pDefaultTooltip_Window )
							{
								g_pDefaultTooltip_Window->SetPCTradeWindowPtr( this );
								
								if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_PCTRADE_SELFITEM, m_LastDraggingSlot, m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID() ) )
								{
									g_pDefaultTooltip_Window->ShowWindow( TRUE );
								}
							}							

							m_bIsTarget = FALSE;							
							m_LastDraggingSlot = -1;
						}
						else if(m_LastDraggingSlot >= 0)
						{
							m_MouseSlotItem.Copy(m_TargetTradeItem[m_LastDraggingSlot]);
							m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
							if( g_pDefaultTooltip_Window )
							{
								g_pDefaultTooltip_Window->SetPCTradeWindowPtr( this );
								
								if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_PCTRADE_TARGETITEM, m_LastDraggingSlot, m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID() ) )
								{
									g_pDefaultTooltip_Window->ShowWindow( TRUE );
								}
							}
							m_bIsTarget = TRUE;
							m_LastDraggingSlot = -1;
						}
						else
						{
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}
						}						
					}
					else
					{
						if(m_bDragState && m_bHaveMouseSlot)
						{
							int selectedslot = SearchFullSlotIndex();
							if(selectedslot == m_MouseSlotItem.m_InventoryNumber)
							{
								// drag한 상태에서 그 slot을 빠져나오지 못함
								m_MouseSlotItem.ResetMouseSlotItem();
								m_bHaveMouseSlot = FALSE;
								m_bDragState = FALSE;
								m_LastDraggingSlot = -1;
							}
							else
							{
								// Drag 성공
								m_bDragState = FALSE;
							}
						} // if(m_bDragState && m_bHaveMouseSlot)
					}
				} // if(m_bDragState)
			}
		} // if(pfocusobject == _XDEF_DUMMYID_PCTRADE)

		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				BOOL resetmouse = TRUE;
				if(!m_bHaveMouseSlot)
				{
					m_LastDraggingSlot = SearchFullSlotIndex();
					if(m_LastDraggingSlot >= 10)
					{
						m_LastDraggingSlot -= 10;
						g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_DELETE, m_LastDraggingSlot, m_SelfTradeItem[m_LastDraggingSlot].Get_m_ucCount());
						m_LastDraggingSlot = -1;
					}
					m_WindowMouseState = _XW_STATE_NONE;
					resetmouse = FALSE;
				}
				if(g_pInventory_Window)
				{
					if(g_pInventory_Window->GetShowStatus() && g_pInventory_Window->CheckMousePosition())
					{
						resetmouse = FALSE;
					}
				}
				if(resetmouse)
				{
					m_bHaveMouseSlot = FALSE;
					m_MouseSlotItem.ResetMouseSlotItem();
					m_WindowMouseState = _XW_STATE_NONE;
					m_LastDraggingSlot = -1;
				}
			} // if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
		}

		if(!mousestate->bButton[0])
		{
			if(pfocusobject == _XDEF_DUMMYID_PCTRADE)
				pfocusobject = NULL;
		}
	}

	// 거래시 거리 체크 
	_XUser* targetUser = g_Userlist.FindUser( m_TargetID );
	if( targetUser )
	{
		FLOAT dx = g_pLocalUser->m_Position.x - targetUser->m_Position.x;
		FLOAT dz = g_pLocalUser->m_Position.z - targetUser->m_Position.z;
		FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

		if( distance > 20.0f )
		{
			// 거래 취소
			g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_CANCEL);
			ShowWindow( FALSE );
		}
	}
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}	

int _XWindow_PCTrade::SearchEmptySlotIndex(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	for(int i = 0 ; i < _XDEF_PCTRADE_MAX_SLOTNUMBER ; i++)
	{
		if(scrnpos->x > m_WindowPosition.x+m_TargetSlotPosition[i].x && scrnpos->x < m_WindowPosition.x+m_TargetSlotPosition[i].x+30)
		{
			if(scrnpos->z > m_WindowPosition.y+m_TargetSlotPosition[i].y && scrnpos->z < m_WindowPosition.y+m_TargetSlotPosition[i].y+30)
			{
				if(m_TargetTradeItem[i].Get_m_sID() == 0)
				{
					return i;
				}
			}
		}

		if(scrnpos->x > m_WindowPosition.x+m_SelfSlotPosition[i].x && scrnpos->x < m_WindowPosition.x+m_SelfSlotPosition[i].x+30)
		{
			if(scrnpos->z > m_WindowPosition.y+m_SelfSlotPosition[i].y && scrnpos->z < m_WindowPosition.y+m_SelfSlotPosition[i].y+30)
			{
				if(m_SelfTradeItem[i].Get_m_sID() == 0)
				{
					return i + 10;
				}
			}
		}
	}
	return -1;
}

int _XWindow_PCTrade::SearchFullSlotIndex(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	for(int i = 0 ; i < _XDEF_PCTRADE_MAX_SLOTNUMBER ; i++)
	{
		if(scrnpos->x > m_WindowPosition.x+m_TargetSlotPosition[i].x && scrnpos->x < m_WindowPosition.x+m_TargetSlotPosition[i].x+30)
		{
			if(scrnpos->z > m_WindowPosition.y+m_TargetSlotPosition[i].y && scrnpos->z < m_WindowPosition.y+m_TargetSlotPosition[i].y+30)
			{
				if(m_TargetTradeItem[i].Get_m_sID() != 0)
				{
					return i;
				}
			}
		}
		
		if(scrnpos->x > m_WindowPosition.x+m_SelfSlotPosition[i].x && scrnpos->x < m_WindowPosition.x+m_SelfSlotPosition[i].x+30)
		{
			if(scrnpos->z > m_WindowPosition.y+m_SelfSlotPosition[i].y && scrnpos->z < m_WindowPosition.y+m_SelfSlotPosition[i].y+30)
			{
				if(m_SelfTradeItem[i].Get_m_sID() != 0)
				{
					return i + 10;
				}
			}
		}
	}
	return -1;
}


void _XWindow_PCTrade::ShowWindow(BOOL show)
{
	if(show)
	{
		m_TargetWaitIcon->ShowWindow(TRUE);
		m_TargetConfirmIcon->ShowWindow(FALSE);

		m_WaitIcon->ShowWindow(TRUE);
		m_ConfirmIcon->ShowWindow(FALSE);

		// 거래중 Inventory lock
		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;
		}
	}
	else
	{
		// 거래 끝나면 lock 해제
		if(g_pInventory_Window)
		{
			if(g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = FALSE;
		}
	}
	_XWindow::ShowWindow(show);
}

void _XWindow_PCTrade::DrawDragIcon(void)
{
//	if(g_MessageBox.m_ShowWindow) return;
	_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
	if(pMessageBox)
	{
		if(pMessageBox->GetShowStatus())
			return;
	}
	
	_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
	if(pTradeConfirm_Window)
	{
		if(pTradeConfirm_Window->GetShowStatus())
			return;
	}
	
	if(m_bHaveMouseSlot)
	{
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		g_pInventory_Window->DrawIcon(TRUE, scrnpos->x - 16 , scrnpos->z - 16, (_XUserItem*)&m_MouseSlotItem, FALSE);
		
		((CEmperorOfDragonsApp*)gApp)->ReadyCursor(_XMOUSETYPE_SELECT);
	}	
}

void _XWindow_PCTrade::RestoreTrade(void)
{
	for(int i = 0 ; i < _XDEF_PCTRADE_MAX_SLOTNUMBER ; i++)
	{
		if(m_SelfTradeItem[i].Get_m_sID() > 0)
		{
			if(m_SelfTradeItem[i].IsStackable())
			{
				if(g_pLocalUser->m_UserItemList[m_SelfTradeItem[i].m_InventoryNumber].Get_m_sID() == m_SelfTradeItem[i].Get_m_sID())
				{
					g_pLocalUser->m_UserItemList[m_SelfTradeItem[i].m_InventoryNumber].Set_m_ucCount(
						g_pLocalUser->m_UserItemList[m_SelfTradeItem[i].m_InventoryNumber].Get_m_ucCount() + m_SelfTradeItem[i].Get_m_ucCount() );
				}
				else
				{
					g_pLocalUser->m_UserItemList[m_SelfTradeItem[i].m_InventoryNumber].Copy(m_SelfTradeItem[i]);
				}
			}
			else
			{
				g_pLocalUser->m_UserItemList[m_SelfTradeItem[i].m_InventoryNumber].Copy(m_SelfTradeItem[i]);
			}
		}
	}
}

void _XWindow_PCTrade::ProcessDblClick(void)
{
	// Delete
	m_LastDraggingSlot = SearchFullSlotIndex();
	if(m_LastDraggingSlot >= 10)
	{
		m_LastDraggingSlot -= 10;

		m_MouseSlotItem.Copy(m_SelfTradeItem[m_LastDraggingSlot]);
		m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
		
		g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_DELETE, m_LastDraggingSlot, m_SelfTradeItem[m_LastDraggingSlot].Get_m_ucCount());

		/*
		if(m_MouseSlotItem.IsStackable())
		{
			_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
			if(pTradeConfirm_Window)
			{
				int nextslot =  g_pInventory_Window->GetUserItemListEmptyCount();
				if(nextslot != -1)
				{
					pTradeConfirm_Window->ShowWindow(TRUE);									
					pTradeConfirm_Window->m_IMEControl.SetFocus();
					pTradeConfirm_Window->m_SelectedCategory1 = m_MouseSlotItem.m_cType;
					pTradeConfirm_Window->m_SelectedItemID = m_MouseSlotItem.m_sID;					
					pTradeConfirm_Window->m_PCTradeSlotIndex = m_LastDraggingSlot;
					pTradeConfirm_Window->m_TradeMode = _XPCTRADE_FROMTRADE_ITEM;
				}
			}
		}
		else
		{
			g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_DELETE, m_LastDraggingSlot, m_SelfTradeItem[m_LastDraggingSlot].m_ucCount);
			m_bHaveMouseSlot = FALSE;
			m_MouseSlotItem.ResetMouseSlotItem();
		}*/
				
		m_LastDraggingSlot = -1;
	}
}

void _XWindow_PCTrade::DrawSelectedItem(void)
{
	if(!m_bDrawSelectedItem)
	{
		int selectedslot = SearchFullSlotIndex();
		if(selectedslot != -1)
		{
			if(selectedslot >= 10)
			{
				RECT iconrect;
				iconrect.left = m_WindowPosition.x + m_SelfSlotPosition[selectedslot-10].x;
				iconrect.top = m_WindowPosition.y + m_SelfSlotPosition[selectedslot-10].y;
				iconrect.right = m_WindowPosition.x + m_SelfSlotPosition[selectedslot-10].x + 31;
				iconrect.bottom = m_WindowPosition.y + m_SelfSlotPosition[selectedslot-10].y + 31;
				
				_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
				DrawItemTooltip(selectedslot, iconrect);
			}
			else
			{
				RECT iconrect;
				iconrect.left = m_WindowPosition.x + m_TargetSlotPosition[selectedslot].x;
				iconrect.top =  m_WindowPosition.y + m_TargetSlotPosition[selectedslot].y;
				iconrect.right = m_WindowPosition.x + m_TargetSlotPosition[selectedslot].x + 31;
				iconrect.bottom =  m_WindowPosition.y + m_TargetSlotPosition[selectedslot].y + 31;
				
				_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
				DrawItemTooltip(selectedslot, iconrect);
			}
		}
		return;
	}

	BOOL	selectedslotarray[_XDEF_PCTRADE_MAX_SLOTNUMBER*2];
	memset(selectedslotarray, 0, sizeof(BOOL)*_XDEF_PCTRADE_MAX_SLOTNUMBER*2);

	if(m_MouseSlotItem.Get_m_sID() > 0)
	{
		if(!m_bIsTarget)
		{
			_XDrawRectAngle(m_WindowPosition.x+m_SelfSlotPosition[m_MouseSlotItem.m_InventoryNumber].x,
						m_WindowPosition.y+m_SelfSlotPosition[m_MouseSlotItem.m_InventoryNumber].y,
						m_WindowPosition.x+m_SelfSlotPosition[m_MouseSlotItem.m_InventoryNumber].x+31,
						m_WindowPosition.y+m_SelfSlotPosition[m_MouseSlotItem.m_InventoryNumber].y+31, 1.0f, _XSC_ITEM_SELECT);
			selectedslotarray[m_MouseSlotItem.m_InventoryNumber+10] = TRUE;
		}
		else
		{
			_XDrawRectAngle(m_WindowPosition.x+m_TargetSlotPosition[m_MouseSlotItem.m_InventoryNumber].x,
						m_WindowPosition.y+m_TargetSlotPosition[m_MouseSlotItem.m_InventoryNumber].y,
						m_WindowPosition.x+m_TargetSlotPosition[m_MouseSlotItem.m_InventoryNumber].x+31,
						m_WindowPosition.y+m_TargetSlotPosition[m_MouseSlotItem.m_InventoryNumber].y+31, 1.0f, _XSC_ITEM_SELECT);
			selectedslotarray[m_MouseSlotItem.m_InventoryNumber] = TRUE;
		}

		if(m_bHaveMouseSlot)
		{
			_XDrawSolidBar(m_WindowPosition.x+m_SelfSlotPosition[m_MouseSlotItem.m_InventoryNumber].x+1,
						m_WindowPosition.y+m_SelfSlotPosition[m_MouseSlotItem.m_InventoryNumber].y+1,
						m_WindowPosition.x+m_SelfSlotPosition[m_MouseSlotItem.m_InventoryNumber].x+30, 
						m_WindowPosition.y+m_SelfSlotPosition[m_MouseSlotItem.m_InventoryNumber].y+30, D3DCOLOR_ARGB(160, 0, 0, 0));
		}
	}

	int selectedslot = SearchFullSlotIndex();
	if(selectedslot != -1 && !selectedslotarray[selectedslot])
	{
		// 2004.05.27->oneway48 modify
		if(selectedslot >= 10)
		{
			RECT iconrect;
			iconrect.left =m_WindowPosition.x+m_SelfSlotPosition[selectedslot-10].x;
			iconrect.top = m_WindowPosition.y+m_SelfSlotPosition[selectedslot-10].y;
			iconrect.right =m_WindowPosition.x+m_SelfSlotPosition[selectedslot-10].x + 31;
			iconrect.bottom = m_WindowPosition.y+m_SelfSlotPosition[selectedslot-10].y + 31;

			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
			DrawItemTooltip(selectedslot, iconrect);
		}
		else
		{
			RECT iconrect;
			iconrect.left = m_WindowPosition.x+m_TargetSlotPosition[selectedslot].x;
			iconrect.top = m_WindowPosition.y+m_TargetSlotPosition[selectedslot].y;
			iconrect.right = m_WindowPosition.x+m_TargetSlotPosition[selectedslot].x + 31;
			iconrect.bottom = m_WindowPosition.y+m_TargetSlotPosition[selectedslot].y + 31;
			
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
			DrawItemTooltip(selectedslot, iconrect);
		}
		// modify end
	}
}

int _XWindow_PCTrade::GetNextEmptySlot(void)
{
	for(int i = 0 ; i < _XDEF_PCTRADE_MAX_SLOTNUMBER ; i++)
	{
		if(m_SelfTradeItem[i].Get_m_sID() <= 0)
			return i;
	}
	return -1;
}

void _XWindow_PCTrade::CopyItemToSlot(int slotnumber, int invennumber)
{
	m_SelfTradeItem[slotnumber].Set_m_cType(g_pLocalUser->m_UserItemList[invennumber].Get_m_cType());
	m_SelfTradeItem[slotnumber].Set_m_cSecond(g_pLocalUser->m_UserItemList[invennumber].Get_m_cSecond());
	m_SelfTradeItem[slotnumber].Set_m_sID( g_pLocalUser->m_UserItemList[invennumber].Get_m_sID() );
	m_SelfTradeItem[slotnumber].m_cIdentify = g_pLocalUser->m_UserItemList[invennumber].m_cIdentify;

	m_SelfTradeItem[slotnumber].Set_m_cThird(g_pLocalUser->m_UserItemList[invennumber].Get_m_cThird());
	m_SelfTradeItem[slotnumber].Set_m_ucCount( g_pLocalUser->m_UserItemList[invennumber].Get_m_ucCount());
	
	m_SelfTradeItem[slotnumber].m_cReserve = g_pLocalUser->m_UserItemList[invennumber].m_cReserve;

	m_SelfTradeItem[slotnumber].Set_m_usCurDur(g_pLocalUser->m_UserItemList[invennumber].Get_m_usCurDur());
	m_SelfTradeItem[slotnumber].Set_m_usMaxDur(g_pLocalUser->m_UserItemList[invennumber].Get_m_usMaxDur());

	for( int i = 0; i < 4; i++ )
		m_SelfTradeItem[slotnumber].Set_m_sSocket(i,g_pLocalUser->m_UserItemList[invennumber].Get_m_sSocket(i));

	m_SelfTradeItem[slotnumber].Set_m_ucSocketCount( g_pLocalUser->m_UserItemList[invennumber].Get_m_ucSocketCount() );

	m_SelfTradeItem[slotnumber].Set_m_ucStrength( g_pLocalUser->m_UserItemList[invennumber].Get_m_ucStrength());
	m_SelfTradeItem[slotnumber].m_usCurProtect = g_pLocalUser->m_UserItemList[invennumber].m_usCurProtect;
	m_SelfTradeItem[slotnumber].m_usCurMaxProtect = g_pLocalUser->m_UserItemList[invennumber].m_usCurMaxProtect;
	m_SelfTradeItem[slotnumber].m_usUsedProtect = g_pLocalUser->m_UserItemList[invennumber].m_usUsedProtect;

	m_SelfTradeItem[slotnumber].Set_m_ucWeaponStrength( g_pLocalUser->m_UserItemList[invennumber].Get_m_ucWeaponStrength() );
	m_SelfTradeItem[slotnumber].Set_m_ucSharpness( g_pLocalUser->m_UserItemList[invennumber].Get_m_ucSharpness() );
	
	m_SelfTradeItem[slotnumber].m_bDurabilityDecrease = g_pLocalUser->m_UserItemList[invennumber].m_bDurabilityDecrease;	
	m_SelfTradeItem[slotnumber].m_bQuestReward		= g_pLocalUser->m_UserItemList[invennumber].m_bQuestReward;

	m_SelfTradeItem[slotnumber].m_ucFlag1		= g_pLocalUser->m_UserItemList[invennumber].m_ucFlag1;
	m_SelfTradeItem[slotnumber].m_ucFlag2		= g_pLocalUser->m_UserItemList[invennumber].m_ucFlag2;
	m_SelfTradeItem[slotnumber].m_ucFlag3		= g_pLocalUser->m_UserItemList[invennumber].m_ucFlag3;
	m_SelfTradeItem[slotnumber].m_usValue1		= g_pLocalUser->m_UserItemList[invennumber].m_usValue1;

	m_SelfTradeItem[slotnumber].Set_m_ucYear(g_pLocalUser->m_UserItemList[invennumber].Get_m_ucYear());
	m_SelfTradeItem[slotnumber].Set_m_ucMonth(g_pLocalUser->m_UserItemList[invennumber].Get_m_ucMonth());
	m_SelfTradeItem[slotnumber].Set_m_ucDay(g_pLocalUser->m_UserItemList[invennumber].Get_m_ucDay());
	m_SelfTradeItem[slotnumber].Set_m_ucHour(g_pLocalUser->m_UserItemList[invennumber].Get_m_ucHour());
	m_SelfTradeItem[slotnumber].Set_m_ucMinute(g_pLocalUser->m_UserItemList[invennumber].Get_m_ucMinute());
}

#ifdef _XTS_ITEM_OPTIMIZATION
void _XWindow_PCTrade::DrawItemTooltip(int slotnumber, RECT iconrect)
{
	char cType = 0;
	short sID = 0;
	unsigned char cStrength = 0;
	if(slotnumber >= 10)
	{
		slotnumber -= 10;
		cType = m_SelfTradeItem[slotnumber].Get_m_cType();
		sID = m_SelfTradeItem[slotnumber].Get_m_sID();

		if( _XGI_FC_WEAPON == GetEqualItemType(cType) )
			cStrength = m_SelfTradeItem[slotnumber].Get_m_ucStrength();
	}
	else
	{
		cType = m_TargetTradeItem[slotnumber].Get_m_cType();
		sID = m_TargetTradeItem[slotnumber].Get_m_sID();

		if( _XGI_FC_WEAPON == GetEqualItemType(cType) )
		{
			cStrength = m_TargetTradeItem[slotnumber].Get_m_ucStrength();
		}
	}
	if( sID > 0 )
	{	
		TCHAR tempbuf[256];
		switch(GetEqualItemType(cType)) 
		{
		case _XGI_FC_WEAPON:
			{			
#ifdef _XTS_ITEM_V20
				if( cStrength > 0 )
					sprintf( tempbuf, "%s +%d", _XGameItem::GetItemFullName(cType, sID), cStrength );
				else
					strcpy( tempbuf, _XGameItem::GetItemFullName(cType, sID) );
#else
				if( cStrength > 0 )
					sprintf( tempbuf, "%s %s +%d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), cStrength );
				else
					sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
#endif
			}
			break;
		case _XGI_FC_CLOTHES:
			{
#ifdef _XTS_ITEM_V20
				strcpy( tempbuf, _XGameItem::GetItemFullName(cType, sID) );
#else
				sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
#endif
			}
			break;
		default:
			{
				strcpy( tempbuf, _XGameItem::GetItemName(cType, sID) );
			}
			break;
		}
		g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, _XGameItem::GetGradeColor(cType, sID), TRUE);
	}
}
#else
void _XWindow_PCTrade::DrawItemTooltip(int slotnumber, RECT iconrect)
{
	char cType = 0;
	short sID = 0;
	unsigned char cStrength = 0;
	if(slotnumber >= 10)
	{
		slotnumber -= 10;
		cType = m_SelfTradeItem[slotnumber].m_cType;
		sID = m_SelfTradeItem[slotnumber].m_sID;
		if( cType == _XGI_FC_WEAPON || cType == _XGI_FC_WEAPON2 || cType == _XGI_FC_WEAPON3 )
		{
			cStrength = m_SelfTradeItem[slotnumber].m_ucStrength;
		}
	}
	else
	{
		cType = m_TargetTradeItem[slotnumber].m_cType;
		sID = m_TargetTradeItem[slotnumber].m_sID;
		if( cType == _XGI_FC_WEAPON || cType == _XGI_FC_WEAPON2 || cType == _XGI_FC_WEAPON3 )
		{
			cStrength = m_TargetTradeItem[slotnumber].m_ucStrength;
		}
	}
	if( sID > 0 )
	{	
		TCHAR tempbuf[256];
		switch(cType) 
		{
		case _XGI_FC_WEAPON:
		case _XGI_FC_WEAPON2:	
		case _XGI_FC_WEAPON3:
			{				
				if( cStrength > 0 )
					sprintf( tempbuf, "%s %s +%d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), cStrength );
				else
					sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
			}
			break;
		case _XGI_FC_CLOTHES:		
		case _XGI_FC_CLOTHES2:
		case _XGI_FC_CLOTHES3:
			{
				sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
			}
			break;
		default:
			{
				strcpy( tempbuf, _XGameItem::GetItemName(cType, sID) );
			}
			break;
		}
		g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, _XGameItem::GetGradeColor(cType, sID), TRUE);
	}
}
#endif

void _XWindow_PCTrade::SetDefaultPosition( void )
{		
#ifdef _ACCLAIM_RUBICONADSYSTEM	
	if( g_ViewRubiconAD )
		MoveWindow( gnWidth-376 , 220 + g_RubiconADWindowSize.cy );
	else
		MoveWindow( gnWidth-376 , 220 );
#else	
	MoveWindow( gnWidth-376 , 220 );
#endif
}

void _XWindow_PCTrade::ResetTradeSetting( void )
{
	m_TargetWaitIcon->ShowWindow(TRUE);
	m_WaitIcon->ShowWindow(TRUE);

#ifdef _XDEF_TREADE_MESSAGE_ADDITION_070809_MAGVIPER //Author : 양희왕
	if( m_ConfirmIcon->GetShowStatus() || m_TargetConfirmIcon->GetShowStatus() )
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_2425), TRUE);
		pMessageBox->StartTimer(10000, TRUE);
	}
#endif

	m_TargetConfirmIcon->ShowWindow(FALSE);
	m_ConfirmIcon->ShowWindow(FALSE);

	m_OKButton->EnableWindow(TRUE);
}