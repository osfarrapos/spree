// XWindow_NPCTrade.cpp: implementation of the _XWindow_NPCTrade class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4786)

#include "stdafx.h"
#include <list>
#include "EmperorOfDragons.h"
#include "XWindow_NPCTrade.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindow_TradeConfirm.h"
#include "XWindowObjectDefine.h"
#include "XNPC.h"
#include "XNPCScript.h"
#include "XNPCScriptConditionItem.h"
#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static int _XDEF_NPCTRADEITEMPOSX = 11;
static int _XDEF_NPCTRADEITEMPOSY = 71;

_XWindow_NPCTrade::_XWindow_NPCTrade() : 
	m_nPriceMode(0)
{
	m_NPCJobIcon				= NULL;
	for( int i = 0; i < 5; i++ )
	{
		m_SelectTabButtton[i] = NULL;
	}

	m_GambleItemPatten			= NULL;
	m_GambleItemInfo			= NULL;
	m_BlackMarketMode			= FALSE;

	m_SelectedTab				= _XTRADETAB_ONE;
	
	m_OwnerNPCID				= 0;
	m_ItemPackageID				= 0;
	m_AdditionalItemPackageID	= 0;
	m_SpecialItemPackgeID		= 0;
		
	m_bClanMode					= FALSE;
	m_bHonorMode1				= FALSE;
	m_bHonorMode2				= FALSE;
	m_bCustomerMode				= FALSE;	

	m_MouseSlotItem.ResetMouseSlotItem();

	m_bDrawSelectedItem			= FALSE;

	m_bHaveMouseSlot			= FALSE;
	m_bDragState				= FALSE;
	m_LastDraggingSlot			= -1;
	m_IconDragStartPos.x = m_IconDragStartPos.y = 0;
	memset(&m_SlotPosition, 0, sizeof(POINT)*_XDEF_MAX_NPCTRADESLOT);

	m_bIsBuy = FALSE;
	m_MaxViewCount = 0;
	// insert end
	
	m_bTradeMenu = FALSE;
	m_WindowMouseState = _XW_STATE_NONE;
}

_XWindow_NPCTrade::~_XWindow_NPCTrade()
{
	m_MouseSlotItem.ResetMouseSlotItem();
	DestroyWindow();
}

BOOL _XWindow_NPCTrade::Initailize(void)
{
	SetDrawBorder( FALSE );
	// Create GUI Object =================================================================
	int npctradeimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create(0, 0, 184, 70, &g_MainInterfaceTextureArchive, npctradeimageindex );
	pTitleBar->SetClipRect( 0, 125, 183, 194 );
	InsertChildObject(pTitleBar);
	
	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create( 0, 69, 184, 269, &g_MainInterfaceTextureArchive, 
						   g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ));
	pBottomBorder->SetClipRect( 0, 10,  183, 211 );
	InsertChildObject( pBottomBorder );

	_XImageStatic* pLeftWindowLine = new _XImageStatic;
	pLeftWindowLine->Create( 0, 69, 8, 234, &g_MainInterfaceTextureArchive, npctradeimageindex );
	pLeftWindowLine->SetClipRect( 184, 64, 193, 230 );
	InsertChildObject( pLeftWindowLine );


	_XImageStatic* pRightWindowLine = new _XImageStatic;
	pRightWindowLine->Create( 175, 69, 182, 234, &g_MainInterfaceTextureArchive, npctradeimageindex );
	pRightWindowLine->SetClipRect( 194, 64, 202, 230 );
	InsertChildObject( pRightWindowLine );
	
	_XImageStatic* pBottomWindowLine = new _XImageStatic;
	pBottomWindowLine->Create( 0, 235, 182, 241, &g_MainInterfaceTextureArchive, npctradeimageindex );
	pBottomWindowLine->SetClipRect( 0, 195, 183, 201 );
	InsertChildObject( pBottomWindowLine );	


	m_NPCJobIcon = new _XImageStatic;
	// 윈도우의 좌표를 더하지 않는다.
	m_NPCJobIcon->Create( 7, 23, 48, 64, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "mi_npcname.tga" ) );
	m_NPCJobIcon->SetClipRect(  4, 4, 44, 44 );
	InsertChildObject( m_NPCJobIcon );

	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].Create( 0, 0, 34, 34, &g_MainInterfaceTextureArchive, npctradeimageindex );
	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].SetClipRect( 213, 64, 247, 98 );

	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].Create( 0, 0, 34, 34, &g_MainInterfaceTextureArchive, npctradeimageindex );
	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].SetClipRect( 213, 30, 247, 64 );

	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_DISABLESLOT].Create( 0, 0, 30, 30, &g_MainInterfaceTextureArchive, npctradeimageindex );
	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_DISABLESLOT].SetClipRect( 216, 0, 246, 30 );

	npctradeimageindex = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );	
	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_NPCTRADE_HELP,
		npctradeimageindex,npctradeimageindex,npctradeimageindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	
	_XCHECKBTN_STRUCTURE tabbtnstruct = { TRUE, { 59, 43 }, { 20, 20 }, 
		_XDEF_NPCTRADE_ONE,
		g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" ), 
		g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" ),
		&g_MainInterfaceTextureArchive };
	
	TCHAR tempbuf[2];
	for( int i = 0; i < 5; i++ )
	{
		m_SelectTabButtton[i] = new _XCheckButton;
		m_SelectTabButtton[i]->Create( tabbtnstruct );		
		m_SelectTabButtton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 431, 10,  451, 30 );
		m_SelectTabButtton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  485, 10,  505, 30 );
		m_SelectTabButtton[i]->SetButtonSwapMode(TRUE);
		m_SelectTabButtton[i]->SetCheck( i == 0 );
		sprintf( tempbuf, "%d", i);
		m_SelectTabButtton[i]->SetButtonText( tempbuf );
		InsertChildObject( m_SelectTabButtton[i] );
		
		tabbtnstruct.position.x += 24;
		tabbtnstruct.commandid++;		
	}

	_XTOOLTIP_STRUCTURE itemtooltipstruct = 
	{
		{ 0, 0 },
		{ 78, 13 },
		{ 1, 1 },
		-1,
		NULL
	};	
	
	for(i = 0 ; i < _XDEF_MAX_NPCTRADESLOT ; i++)
	{
		m_SlotPosition[i].x = _XDEF_NPCTRADEITEMPOSX + ((i%5)*33);
		m_SlotPosition[i].y = _XDEF_NPCTRADEITEMPOSY + ((i/5)*33);
	}

	// insert end

	return TRUE;
}

void _XWindow_NPCTrade::DestroyWindow(void)
{
	list <_XTradeItem>::iterator iter_item;
	for(iter_item = m_TradeItemList.begin() ; iter_item != m_TradeItemList.end() ; )
	{
		iter_item = m_TradeItemList.erase(iter_item);
	}
	m_TradeItemList.clear();	
	
	SAFE_DELETE_ARRAY( m_GambleItemPatten );
	SAFE_DELETE_ARRAY( m_GambleItemInfo );

	_XWindow::DestroyWindow();
}

// 2004.05.19->oneway48 insert
void _XWindow_NPCTrade::MoveWindow( int X, int Y )
{			
	// Clipping from screen size	
	_XWindow::MoveWindow( X, Y );
}


void _XWindow_NPCTrade::Draw(_XGUIObject*& pfocusobject)
{	
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow) return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE) return;
	
	DrawTradeItemIcon();
	DrawSelectedItem();
}

BOOL _XWindow_NPCTrade::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)			return FALSE;
	if(!this->m_ShowWindow)		return FALSE;
	
	_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
	if( pTradeMessageBox ) 
	{
		if(pfocusobject == _XDEF_DUMMYID_NPCTRADE) pfocusobject = NULL;

		if(!_XWindow::Process(pfocusobject)) return FALSE;
		if( pTradeMessageBox->GetShowStatus() ) return FALSE;
	}

	_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)m_pMyManager->FindWindow(_XDEF_WTITLE_TRADECONFIRM);
	if( pTradeConfirm_Window->GetShowStatus() ) 
	{
		if(pfocusobject == _XDEF_DUMMYID_NPCTRADE) pfocusobject = NULL;
		if(!_XWindow::Process(pfocusobject)) return FALSE;
	}

	BOOL	bInTrade = FALSE;
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(mousestate)
	{
		if(!pfocusobject)		// focus 가 없을 때 
		{
			if(mousestate->bButton[1])
			{
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
				// mouse left down
				if(CheckMousePosition())
				{
					// Focus
					if(scrnpos->x > m_WindowPosition.x+7 && scrnpos->x < m_WindowPosition.x+174)
					{
						if(scrnpos->z > m_WindowPosition.y+69 && scrnpos->z < m_WindowPosition.y+236)
						{
							pfocusobject = _XDEF_DUMMYID_NPCTRADE;
							m_WindowMouseState = _XW_STATE_NONE;
						}
					}
					else
					{
						m_WindowMouseState = _XW_STATE_LEFTCLICK; // 2004.06.16->oneway48 insert
					}
					
					if(g_pInventory_Window)	// 인벤토리에서 드래그 했을 경우
					{
						if(g_pInventory_Window->GetShowStatus())
						{
							if(g_pInventory_Window->m_bHaveMouseSlot && g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)
							{
								bool flag = true;

								if( _XGameItem::CheckNPCTrade( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType(), 
															   g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sID() ) )
								{
									flag = false;									
									_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
									pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2120), TRUE);									

									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
								}
								
							#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
								if( m_nPriceMode == _MODE_MNDPOINT )
								{
									flag = false;
									_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
									pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3798), TRUE);//해당 NPC에게는 아이템을 판매할 수 없습니다.

									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
								}
							#endif
								if(m_nPriceMode == _MODE_CLANPOINT)
								{
									flag = false;
									_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
									pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3798), TRUE);//해당 NPC에게는 아이템을 판매할 수 없습니다.

									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
								}

								if( flag )
								{
									// NPC에게 팔기
									if(g_pInventory_Window->m_MouseSlotItem.IsStackable())
									{
										_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
										
										TCHAR messagestring[256];
										memset(messagestring, 0, sizeof(TCHAR)*256);
										int resultcashcheck = CheckCashItem(g_pInventory_Window->m_MouseSlotItem.Get_m_cType(),g_pInventory_Window->m_MouseSlotItem.Get_m_sID());
										if( resultcashcheck == 1 || resultcashcheck == 2 )
										{
											sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_2981) );	// 캐시아이템을 버리게 되면 영원히 삭제되며 환불/복구 되지 않습니다.판매하시겠습니까?
										}
										else
										{	
											TCHAR		namestring[128];
											memset(namestring, 0, sizeof(TCHAR)*128);
											strcpy(namestring, _XGameItem::GetItemName(g_pInventory_Window->m_MouseSlotItem.Get_m_cType(), g_pInventory_Window->m_MouseSlotItem.Get_m_sID()));
											
											sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INVENTORY_SELL), namestring);	// _T("%s(을)를 판매하시겠습니까?")									
										}
										pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_NPCTRADE_SELLOK, _XDEF_NPCTRADE_SELLNO);
										
									}
									else
									{
										_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
									
										TCHAR messagestring[256];
										memset(messagestring, 0, sizeof(TCHAR)*256);
										int resultcashcheck = CheckCashItem(g_pInventory_Window->m_MouseSlotItem.Get_m_cType(),g_pInventory_Window->m_MouseSlotItem.Get_m_sID());
										if( resultcashcheck == 1 || resultcashcheck == 2 )
										{
											sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_2981) );	// 캐시아이템을 버리게 되면 영원히 삭제되며 환불/복구 되지 않습니다.판매하시겠습니까?
										}
										else
										{	
											TCHAR		namestring[128];
											memset(namestring, 0, sizeof(TCHAR)*128);
											strcpy(namestring, _XGameItem::GetItemName(g_pInventory_Window->m_MouseSlotItem.Get_m_cType(), g_pInventory_Window->m_MouseSlotItem.Get_m_sID()));
											
											sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INVENTORY_SELL), namestring);	// _T("%s(을)를 판매하시겠습니까?")									
										}
										
										pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_NPCTRADE_SELLOK, _XDEF_NPCTRADE_SELLNO);
									}									
								}
								bInTrade = TRUE;
							}
						}
					}

					if(!bInTrade)
					{
						if(!m_bHaveMouseSlot)
						{
							// Mouse down 위치 저장
							m_IconDragStartPos.x = scrnpos->x;
							m_IconDragStartPos.y = scrnpos->z;
							m_LastDraggingSlot = GetSlotIndex();
							m_bDragState = TRUE;
						}
						else
						{
							m_MouseSlotItem.ResetMouseSlotItem();
							m_bHaveMouseSlot = FALSE;
						}
					}

					if( g_pInventory_Window )
					{
						if( g_pInventory_Window->GetShowStatus())
						{
							g_pInventory_Window->m_bDrawSelectedItem = FALSE;
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
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
					}

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
		}

		
		if(pfocusobject == _XDEF_DUMMYID_NPCTRADE)	// focus 가 있을 때 
		{
			if(mousestate->bButton[0])
			{
				// 드래그 처리
				char	cType = 0;
				short	sID = 0;
				int		itemNo = 0;

				if(m_bDragState && !m_bHaveMouseSlot)
				{
					if(scrnpos->x != m_IconDragStartPos.x || scrnpos->z != m_IconDragStartPos.y)
					{
						if(GetItemPropertyFromSlot(m_LastDraggingSlot, cType, sID, itemNo))
						{
							m_bHaveMouseSlot = TRUE;
							m_MouseSlotItem.Set_m_cType(cType);
							m_MouseSlotItem.Set_m_sID(sID);
							m_MouseSlotItem.m_InventoryNumber = itemNo;
							m_LastDraggingSlot = -1;

							// 2004.05.31->oneway48 insert : 드래그 시작시 아이콘 툴팁 제거.							
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}

						}
					}
				} // if(m_bDragState && !m_bHaveMouseSlot)
			}
			if(!mousestate->bButton[0])
			{
				m_bDrawSelectedItem = TRUE;

				char	cType = 0;
				short	sID = 0;
				int		itemNo = 0;

				if(scrnpos->x == m_IconDragStartPos.x && scrnpos->z == m_IconDragStartPos.y)
				{
					// 제자리에서 클릭
					if(GetItemPropertyFromSlot(m_LastDraggingSlot, cType, sID, itemNo))
					{
						m_MouseSlotItem.Set_m_cType(cType);
						m_MouseSlotItem.Set_m_sID(sID);
						m_MouseSlotItem.m_InventoryNumber = itemNo;						
						if( g_pDefaultTooltip_Window )
						{
							if( m_BlackMarketMode )
							{
								if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_GAMBLE, itemNo, cType, sID ) )
								{
									g_pDefaultTooltip_Window->ShowWindow( TRUE );
								}
							}
							else
							{
								if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_NPCTRADE_NPCITEM, itemNo, cType, sID, m_nPriceMode ) )
								{
									g_pDefaultTooltip_Window->ShowWindow( TRUE );
								}
							}							
						}
						m_LastDraggingSlot = -1;
					}
					else
					{
						// icon이 없는 slot click
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
						int selectedslot = GetSlotIndex();
						if(GetItemPropertyFromSlot(selectedslot, cType, sID, itemNo))
						{
							if(itemNo == m_MouseSlotItem.m_InventoryNumber)
							{
								// drag한 상태에서 그 slot을 빠져나가지 못함 - Reset
								m_MouseSlotItem.ResetMouseSlotItem();
								m_bHaveMouseSlot = FALSE;
								m_bDragState = FALSE;
								m_LastDraggingSlot = -1;
							}
							else
							{
								// drag 성공
								m_bDragState = FALSE;
							}
						}
					} // if(m_bDragState && m_bHaveMouseSlot)
				}
			}
		} // if(pfocusobject == _XDEF_DUMMYID_NPCTRADE)

		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				// 오른쪽 버튼 처리
				m_LastDraggingSlot = GetSlotIndex();
				short sID = 0;
				char cType = 0;
				if(GetItemPropertyFromSlot(m_LastDraggingSlot, cType, sID, m_MouseSlotItem.m_InventoryNumber))
				{
					m_MouseSlotItem.Set_m_cType(cType);
					m_MouseSlotItem.Set_m_sID( sID );

					if(m_MouseSlotItem.IsStackable())
					{
						_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)m_pMyManager->FindWindow(_XDEF_WTITLE_TRADECONFIRM);
						if(pTradeConfirm_Window)
						{
							pTradeConfirm_Window->ShowWindow(TRUE);
							pTradeConfirm_Window->m_IMEControl.SetFocus();
							pTradeConfirm_Window->m_SelectedCategory1 = m_MouseSlotItem.Get_m_cType();
							pTradeConfirm_Window->m_SelectedItemID = m_MouseSlotItem.Get_m_sID();
							pTradeConfirm_Window->m_TradeMode = _XNPCTRADE_BUY;
							m_LastDraggingSlot = -1;
						}
					}
					else
					{
						//2004.06.22->oneway48 insert						
						_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
						TCHAR		namestring[128];
						memset(namestring, 0, sizeof(TCHAR)*128);
						strcpy(namestring, _XGameItem::GetItemName(m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID()));
						
						TCHAR messagestring[256];
						memset(messagestring, 0, sizeof(TCHAR)*256);
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INVENTORY_BUY), namestring);	// _T("%s(을)를 구입하시겠습니까?")

						pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_NPCTRADE_BUYOK, _XDEF_NPCTRADE_BUYNO);						
					}
				}
				else
				{
					m_LastDraggingSlot = -1;
				}

				if(m_bHaveMouseSlot)
				{
					BOOL resetmouse = TRUE;
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
					}
				} // if(m_bHaveMouseSlot)

				m_WindowMouseState = _XW_STATE_NONE;
			}
		}


		if(!mousestate->bButton[0])
		{
			if(m_WindowMouseState == _XW_STATE_LEFTCLICK)// 2004.06.16->oneway48 insert : 다른 곳에 클릭하였을 때 툴팁 없애기
			{
				m_WindowMouseState = _XW_STATE_NONE;
				if( g_pDefaultTooltip_Window )
				{
					if( g_pDefaultTooltip_Window->GetShowStatus() )						
						g_pDefaultTooltip_Window->ShowWindow( FALSE );	
				}
			}

			if(pfocusobject == _XDEF_DUMMYID_NPCTRADE)
				pfocusobject = NULL;
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_NPCTrade::DrawTradeItemIcon(void)
{
	int startnumber = m_SelectedTab * _XDEF_MAX_NPCTRADESLOT;
	int count = 0;

	list <_XTradeItem>::iterator iter_item = m_TradeItemList.begin();
	if( startnumber < m_TradeItemList.size() )
	{
		advance( iter_item, startnumber );
		for( int i = 0; i < _XDEF_MAX_NPCTRADESLOT; i++ )
		{		
			if( iter_item != m_TradeItemList.end() )
			{
				_XTradeItem tradeitem = *iter_item;
				m_ItemEdgeIcon[_XNT_ITEMEDGEICONTYPE_NORMAL].Draw( m_WindowPosition.x+m_SlotPosition[i].x-2, m_WindowPosition.y+m_SlotPosition[i].y-2 );
				if( m_BlackMarketMode )
				{
					g_pInventory_Window->DrawIcon_Gamble( TRUE, m_WindowPosition.x+m_SlotPosition[i].x, m_WindowPosition.y+m_SlotPosition[i].y, (_XUserItem*)&tradeitem, FALSE, FALSE, CheckCondition(tradeitem.Get_m_cType(), tradeitem.Get_m_sID()) );
				}
				else
				{
					g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+m_SlotPosition[i].x, m_WindowPosition.y+m_SlotPosition[i].y, (_XUserItem*)&tradeitem, FALSE );
					// 조건 안 맞는 아이템 처리
					if( !CheckCondition(tradeitem.Get_m_cType(), tradeitem.Get_m_sID()) )
					{
						_XDrawSolidBar( m_WindowPosition.x+m_SlotPosition[i].x, m_WindowPosition.y+m_SlotPosition[i].y, 
										m_WindowPosition.x+m_SlotPosition[i].x-1+31, m_WindowPosition.y+m_SlotPosition[i].y-1+31, D3DCOLOR_ARGB(102,237,27,35) );
					}					
				}
				
				iter_item++;
			}
			else
			{
				m_ItemEdgeIcon[_XNT_ITEMEDGEICONTYPE_EMPTY].Draw( m_WindowPosition.x+m_SlotPosition[i].x-2, m_WindowPosition.y+m_SlotPosition[i].y-2 );
			}
		}	
	}
	else
	{
		for( int i = 0; i < _XDEF_MAX_NPCTRADESLOT; i++ )
		{
			m_ItemEdgeIcon[_XNT_ITEMEDGEICONTYPE_EMPTY].Draw( m_WindowPosition.x+m_SlotPosition[i].x-2, m_WindowPosition.y+m_SlotPosition[i].y-2 );
		}
	}
}

void _XWindow_NPCTrade::SetListItem(void)
{
	list <_XTradeItem>::iterator iter_item;
	for(iter_item = m_TradeItemList.begin() ; iter_item != m_TradeItemList.end() ; )
	{
		iter_item = m_TradeItemList.erase(iter_item);
	}
	m_TradeItemList.clear();

	char		cType = 0;
	char		cSecond = 0;
	short		sID = 0;
	int			count = 0;

	for(int i = 0 ; i < g_ItemTradePackage[m_ItemPackageID].itemCount ; i++)
	{
		_XTradeItem tradeitem;
		tradeitem.Set_m_cType(g_ItemTradePackage[m_ItemPackageID].itemUnit[i].category);
		tradeitem.Set_m_sID( g_ItemTradePackage[m_ItemPackageID].itemUnit[i].uniqueID );
		tradeitem.m_InventoryNumber = count;
		tradeitem.m_ItemNo = i;
		m_TradeItemList.push_back(tradeitem);
		count++;
	}

	if( count > 0 )
	{
		int tempcount = count % _XDEF_MAX_NPCTRADESLOT;
		if( tempcount == 0 )
		{
			count /= _XDEF_MAX_NPCTRADESLOT;
			count--;
		}
		else
		{
			count /= _XDEF_MAX_NPCTRADESLOT;
		}	
		
		for( i = 0; i < 5; i++ )
		{
			m_SelectTabButtton[i]->ShowWindow( i <= count );
		}
	}	
}

void _XWindow_NPCTrade::ChangeTab(_XNPCTRADETAB tab)
{
	m_SelectedTab = tab;

	for( int i = 0; i < 5; i++ )
		m_SelectTabButtton[i]->SetCheck( i == tab );

	m_MouseSlotItem.ResetMouseSlotItem();
	m_bHaveMouseSlot = FALSE;
}

void _XWindow_NPCTrade::SetItemPackageID(int packageid)
{
	m_ItemPackageID = packageid;
	ChangeTab( _XTRADETAB_ONE );
}

void _XWindow_NPCTrade::SetAdditionalItemPackageID(int packageid)
{
	m_AdditionalItemPackageID = packageid;
	ChangeTab( _XTRADETAB_ONE );
}

void _XWindow_NPCTrade::SetSpecialItemPackageID(int packageid)
{
	m_SpecialItemPackgeID = packageid;
	ChangeTab( _XTRADETAB_ONE );
}

void _XWindow_NPCTrade::ProcessBuy(void)
{
	if( m_MouseSlotItem.Get_m_cType() != _XGI_FC_POTION && m_MouseSlotItem.Get_m_cType() != _XGI_FC_CONSUMING &&
		m_MouseSlotItem.Get_m_cType() != _XGI_FC_BOXKEY && m_MouseSlotItem.Get_m_cType() != _XGI_FC_RESOURCE )
		m_MouseSlotItem.Set_m_ucCount(1);

	int emptyslot = 0;
	int end;
	
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장
	end = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; //시작 25 끝 50 기본 행낭 // 이소스는 무엇인가..
	if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 )
	{
		end = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART;
		if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0 )
			end = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;
	}

	for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < end ; i++)
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0)
			emptyslot++;
	}

	for( i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++)
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0)
			emptyslot++;
	}
#else
	if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 )
		end = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART + g_pLocalUser->m_cBackPackExtend;
	else
		end = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART;

	for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < end ; i++)
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0)
			emptyslot++;
	}
#endif

	if(emptyslot == 0)
	{
		_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
		pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_MESSAGEHANDLER_NOTENOUGHINVEN, TRUE);
		return;
	}

	u_int needmoney = 0;

	float discount = 0.0f;	//할인율
	
	if(m_bClanMode)  
	{
		// 문파 판별 모드일 때
		if( g_LastNPCScriptOwner->m_NPCClanClass == g_pLocalUser->m_CharacterInfo.groupindex )
		{
			// 자신의 문파일 경우
			discount = discount + 0.0f; 
		}
		else if(!g_LastNPCScriptOwner->m_bCheckHostility)
		{
			// 우호적인 문파일 경우
			discount = discount + 0.1f;
		}
	}
	
	if(m_bHonorMode1)
	{
		// 명성 판별 모드일 때
		if(g_pLocalUser->m_CharacterInfo.Get_honor() >= 0 && g_pLocalUser->m_CharacterInfo.Get_honor() < 50 )
		{
			//범인
			discount = discount + 0.0f;
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_honor() >= 50 && g_pLocalUser->m_CharacterInfo.Get_honor() < 100 )
		{
			//강호
			discount = discount + 0.03f;
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_honor() >= 100 && g_pLocalUser->m_CharacterInfo.Get_honor() < 150 )
		{
			//무림
			discount = discount + 0.06f;
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_honor() >= 150 && g_pLocalUser->m_CharacterInfo.Get_honor() < 200 )
		{
			//천하
			discount = discount + 0.09f;
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_honor() >= 200 && g_pLocalUser->m_CharacterInfo.Get_honor() < 250 )
		{
			//절세
			discount = discount + 0.12f;
		}
		else 
		{
			//일대
			discount = discount + 0.15f;
		}
	}
		  
	if(m_bCustomerMode)
	{
		// 단골일 대
		int selecteddiscount;
		selecteddiscount = rand() % 4;
		
		switch(selecteddiscount) {
		case 0 : 
			discount = discount + 0.0f;
			break;
			
		case 1 :
			discount = discount + 0.05f;
			break;
			
		case 2 :
			discount = discount + 0.1f;
			break;
			
		case 3 :
			discount = discount + 0.15f;
			break;
		}
	}

	if( m_BlackMarketMode )
	{
		needmoney = m_GambleItemInfo[m_MouseSlotItem.m_InventoryNumber].Price;
	}
	else
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		switch(GetEqualItemType(m_MouseSlotItem.Get_m_cType()))
		{
		case _XGI_FC_WEAPON :
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
				if( WeaponItem )
					needmoney = WeaponItem[m_MouseSlotItem.Get_m_sID()].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_CLOTHES :
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
				if( ClothesItem )
					needmoney = ClothesItem[m_MouseSlotItem.Get_m_sID()].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_BOOK :
			{
				needmoney = g_BookItemProperty[m_MouseSlotItem.Get_m_sID()].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_POTION :
			{
				needmoney = g_PotionItemProperty[m_MouseSlotItem.Get_m_sID()].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_CONSUMING :
			{
				needmoney = g_ConsumingItemProperty[m_MouseSlotItem.Get_m_sID()].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_ELIXIR :
			{
				needmoney = g_ElixirItemProperty[m_MouseSlotItem.Get_m_sID()].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_ACCESSORY :
			{
				needmoney = g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_LIFE :
			{
				needmoney = g_LifeItemProperty[m_MouseSlotItem.Get_m_sID()].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_SOCKET :
			{
				needmoney = g_SocketItemProperty[m_MouseSlotItem.Get_m_sID()].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_RESOURCE :
			{
				needmoney = g_ResourceItemProperty[m_MouseSlotItem.Get_m_sID()].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_BOX :
			{
				needmoney = g_BoxItemProperty[m_MouseSlotItem.Get_m_sID()].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_BOXKEY :
			{
				needmoney = g_BoxKeyItemProperty[m_MouseSlotItem.Get_m_sID()].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		}
#else
		switch(m_MouseSlotItem.Get_m_cType())
		{
		case _XGI_FC_WEAPON :
			{
				needmoney = g_WeaponItemProperty[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_CLOTHES :
			{
				needmoney = g_ClothesItemProperty[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_WEAPON2 :
			{
				needmoney = g_WeaponItemProperty2[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_CLOTHES2 :
			{
				needmoney = g_ClothesItemProperty2[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_BOOK :
			{
				needmoney = g_BookItemProperty[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_POTION :
			{
				needmoney = g_PotionItemProperty[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_CONSUMING :
			{
				needmoney = g_ConsumingItemProperty[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_ELIXIR :
			{
				needmoney = g_ElixirItemProperty[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_ACCESSORY :
			{
				needmoney = g_AccessoryItemProperty[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_LIFE :
			{
				needmoney = g_LifeItemProperty[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_SOCKET :
			{
				needmoney = g_SocketItemProperty[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_RESOURCE :
			{
				needmoney = g_ResourceItemProperty[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_BOX :
			{
				needmoney = g_BoxItemProperty[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_BOXKEY :
			{
				needmoney = g_BoxKeyItemProperty[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_WEAPON3 :
			{
				needmoney = g_WeaponItemProperty3[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		case _XGI_FC_CLOTHES3 :
			{
				needmoney = g_ClothesItemProperty3[m_MouseSlotItem.m_sID].uiPrice * m_MouseSlotItem.Get_m_ucCount();
			}
			break;
		default :
			break;
		}
#endif
	}	
	
	// 지역 세율 계산 - 살 때
#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE
	if(!m_BlackMarketMode)
	{
		// gamble 제외
		u_int orgmoney = needmoney;

		// 임시 코드
		FLOAT correction = 0.4f;
		int tvalue = orgmoney % 10;
		if(tvalue == 6)
		{
			correction = 0.5f;
		}
		
		u_int tax_money = 0;

		if(m_nPriceMode == _MODE_NORMAL)
		{
			tax_money = static_cast<u_int>( ((float)needmoney*(float)(g_nMapTaxRate)/100.0f) + correction );	//<== 세금으로 걷는 돈.
		}

		needmoney += tax_money;
	}
#endif

	if(m_nPriceMode == _MODE_CLANPOINT)
	{
		if(needmoney > g_pLocalUser->m_CharacterInfo.Get_iContribution())
		{
			_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
			pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCSCRIPT_ERROR5, TRUE);
			return;
		}
	}
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	else if( m_nPriceMode == _MODE_MNDPOINT )
	{
		//여기 수정해야 한다
		if(needmoney > g_pLocalUser->m_CharacterInfo.Get_iContribution())
		{
			_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
			pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCSCRIPT_ERROR5, TRUE);
			return;
		}
	}
#endif
	else
	{
		if(needmoney > g_pLocalUser->m_Money)
		{
			_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
			pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
			return;
		}
	}
	
	if( m_BlackMarketMode )	// 암상인한테 사기
	{
		g_NetworkKernel.SendPacket(MSG_NO_NPC_TRADE_REQ, en_npc_trade_mode_gamble, needmoney);
	}
	else
	{
		m_bIsBuy = TRUE;
		if(m_nPriceMode == _MODE_CLANPOINT)
		{
			// 문파기여도로 사기
			g_NetworkKernel.SendPacket(MSG_NO_NPC_TRADE_REQ, en_npc_trade_mode_buy_contribute, needmoney);
		}
	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
		else if( m_nPriceMode == _MODE_MNDPOINT )
		{
			//사제 기여도로 사기
		}
	#endif
		else
		{
			// npc에게 사기
			g_NetworkKernel.SendPacket(MSG_NO_NPC_TRADE_REQ, en_npc_trade_mode_buy, needmoney);
		}
	}	

	if( g_pDefaultTooltip_Window )
	{
		if( g_pDefaultTooltip_Window->GetShowStatus() )						
			g_pDefaultTooltip_Window->ShowWindow( FALSE );
	}
}

void _XWindow_NPCTrade::ProcessSell(void)
{
	if(!g_pInventory_Window)
		return;

	if(	!g_pInventory_Window->m_MouseSlotItem.IsStackable() )
	{
		g_pInventory_Window->m_MouseSlotItem.Set_m_ucCount(1);
	}

	int needmoney = 0;
	float margin = 0.0f;	// 마진
	
	if(m_bClanMode)  
	{
		// 문파 판별 모드일 때
		if( g_LastNPCScriptOwner->m_NPCClanClass == g_pLocalUser->m_CharacterInfo.groupindex )
		{
			// 자신의 문파일 경우
			margin = margin + 0.0f; 
		}
		else if(g_LastNPCScriptOwner->m_bCheckFriendship)
		{
			// 우호적인 문파일 경우
			margin = margin + 0.1f;
		}
	}
	
	if(m_bHonorMode1)
	{
		// 명성 판별 모드일 때
		margin = margin + 0.0f; 
	}
	
	if(m_bCustomerMode)
	{
		// 단골일 때
		margin = margin + 0.0f;
	}

	needmoney = PriceCalc(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber, g_pInventory_Window->m_MouseSlotItem.Get_m_ucCount() );

	// 지역 세율 계산 - 팔때
#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE
	int orgmoney = needmoney;

	FLOAT correction = 0.4f;
	int tvalue = orgmoney % 10;
	if(tvalue == 6)
	{
		correction = 0.5f;
	}

	u_int tax_money = static_cast<u_int>( ((float)needmoney*(float)(g_nMapTaxRate)/100.0f) + correction );	//<== 세금으로 걷는 돈.
	needmoney -= tax_money;

	/*
	//////////////////////////////////////////////////////////////////////////
	// 정수이용 고정소수점 연산 처리 
	LONGLONG sell_tax = ((LONGLONG)needmoney*(LONGLONG)g_nMapTaxRate) * 100L; // 소수점 이하 2자리까지 계산하고 버리기 위해 고정소수점 2자리 상승 
	sell_tax = sell_tax / 100L;	// 100분율 계산.
	sell_tax += 40L;			// 0.05 이상 반올림.
	sell_tax = sell_tax / 100L;  // 소수점 이하 2자리 버리기 위해 고정소수점 2자리 내림(복귀)

	unsigned int tax_money = (unsigned int)sell_tax;
	needmoney -= tax_money;		
	
	_XLog("tax:%d  need:%d  org:%d", tax_money, needmoney, orgmoney);
	//////////////////////////////////////////////////////////////////////////
	*/

#endif

	m_bIsBuy = FALSE;
	g_NetworkKernel.SendPacket(MSG_NO_NPC_TRADE_REQ, (int)m_bIsBuy, needmoney);

	if(g_pInventory_Window)
	{
		if( g_pDefaultTooltip_Window )
		{
			if( g_pDefaultTooltip_Window->GetShowStatus() )						
				g_pDefaultTooltip_Window->ShowWindow( FALSE );
		}
	}
}


void _XWindow_NPCTrade::SetDefaultPosition( void )
{
	if( g_pInventory_Window->m_bLockInventory ) //게임윈도우를 움직일 경우..
		return;

	MoveWindow( 409, (gnHeight>>1)-205 );
}

void _XWindow_NPCTrade::ShowWindow( BOOL show )
{
	if(show)
	{	
		// Window 위치 재조정 =============================
		MoveWindow( 409, (gnHeight>>1)-205 );
		
		// 거래중 Inventory move lock
		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;
		}

		if( g_pDefaultTooltip_Window )
			g_pDefaultTooltip_Window->SetNPCTradeWindowPtr( this );
	}
	else
	{
		// 거래 끝나면 lock 해제
		if(g_pInventory_Window)
		{
			if(g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = FALSE;
		}

		// mode 해제
		m_nPriceMode = _MODE_NORMAL;
	}
	
	_XWindow::ShowWindow( show );
}

void _XWindow_NPCTrade::SetTradeMode(_XNPCScriptItem* npcsubscript)
{
	if(!npcsubscript->m_SubScriptConditionList.listEmpty())
	{
		npcsubscript->m_SubScriptConditionList.resetList();
		_XNPCScriptConditionItem* conditionitem = NULL;
		do 
		{
			conditionitem = (_XNPCScriptConditionItem *)npcsubscript->m_SubScriptConditionList.currentItem();
			if(conditionitem)
			{
				if(conditionitem->m_NPCSubscriptCondition.m_conditionclass == ID_NPCSUBSCRIPT_CONDITION_CHECKCLANNPC)
					if(conditionitem->ProcessCheckClanNPC() == NPC_CONDITION_RESULT_SUCCESS)
						m_bClanMode = TRUE;
					
					if(conditionitem->m_NPCSubscriptCondition.m_conditionclass == ID_NPCSUBSCRIPT_CONDITION_CHECKCLANFRIENDSHIP)
						if(conditionitem->ProcessCheckClanFriendship() == NPC_CONDITION_RESULT_SUCCESS)
							m_bClanMode = TRUE;
						
						if(conditionitem->m_NPCSubscriptCondition.m_conditionclass == ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR)
						{
							m_bHonorMode1 = TRUE;
							if(conditionitem->ProcessCheckHonor() == NPC_CONDITION_RESULT_SUCCESS)
								m_bHonorMode2 = TRUE;							
						}
			}
			npcsubscript->m_SubScriptConditionList.nextItem();
		} while( !npcsubscript->m_SubScriptConditionList.atHeadOfList());
	}	
}

int _XWindow_NPCTrade::GetSlotIndex(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	int PosX, PosY;
	PosX = scrnpos->x;
	PosY = scrnpos->z;

	PosX -= (m_WindowPosition.x + _XDEF_NPCTRADEITEMPOSX);
	if( PosX < 0 )
		return -1;
	PosX /= 33;	
	PosY -= (m_WindowPosition.y + _XDEF_NPCTRADEITEMPOSY);
	if( PosY < 0 )
		return -1;
	PosY /= 33;	

	if( PosX < 0 || PosX > 4 || PosY < 0 || PosY > 4 )
		return -1;

	int jumpcount = PosY * 5 + PosX;
	if( jumpcount > _XDEF_MAX_NPCTRADESLOT ) return -1;

	int startnumber = m_SelectedTab * _XDEF_MAX_NPCTRADESLOT;
	jumpcount += startnumber;

	return jumpcount;
	
	/*for(int i = 0 ; i < _XDEF_MAX_NPCTRADESLOT ; i++)		// 예전 소스
	{
		if(scrnpos->x > m_WindowPosition.x+m_SlotPosition[i].x && scrnpos->x < m_WindowPosition.x+m_SlotPosition[i].x+30)
		{
			if(scrnpos->z > m_WindowPosition.y+m_SlotPosition[i].y && scrnpos->z < m_WindowPosition.y+m_SlotPosition[i].y+30)
			{
				int startnumber = m_SelectedTab * _XDEF_MAX_NPCTRADESLOT;				
				list <_XTradeItem>::iterator iter_item;
				for(iter_item = m_TradeItemList.begin() ; iter_item != m_TradeItemList.end() ; iter_item++)
				{
					_XTradeItem tradeitem = *iter_item;
					int checkslotnumber = tradeitem.m_InventoryNumber - startnumber;
					if(checkslotnumber == i)
						return i;
				}
			}
		} 
	}*/	
}

BOOL _XWindow_NPCTrade::GetItemPropertyFromSlot(int selectedslot, char& cType, short& sID, int& itemNo)
{
	list <_XTradeItem>::iterator iter_item;
	for(iter_item = m_TradeItemList.begin() ; iter_item != m_TradeItemList.end() ; iter_item++)
	{
		_XTradeItem tradeitem = *iter_item;
		if(tradeitem.m_InventoryNumber == selectedslot)
		{
			cType = tradeitem.Get_m_cType();
			sID = tradeitem.Get_m_sID();
			itemNo = tradeitem.m_ItemNo;

			if( sID <= 0 )
				return FALSE;
			else
				return TRUE;
		}
	}
	return FALSE;
}

BOOL _XWindow_NPCTrade::GetItemPropertyFromNo(int itemNo, char& cType, short& sID, int& slotnumber)
{
	list <_XTradeItem>::iterator iter_item;
	for(iter_item = m_TradeItemList.begin() ; iter_item != m_TradeItemList.end() ; iter_item++)
	{
		_XTradeItem tradeitem = *iter_item;
		if(tradeitem.m_ItemNo == itemNo)
		{
			cType = tradeitem.Get_m_cType();
			sID = tradeitem.Get_m_sID();
			slotnumber = tradeitem.m_InventoryNumber;
			return TRUE;
		}
	}
	return FALSE;
}

void _XWindow_NPCTrade::DrawDragIcon(void)
{
//	if(g_MessageBox.m_ShowWindow) return;
	_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
	if(pTradeMessageBox)		return;
	
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

void _XWindow_NPCTrade::ProcessDblClick(void)
{
	_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
	if( pTradeMessageBox ) return;	
	_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)m_pMyManager->FindWindow(_XDEF_WTITLE_TRADECONFIRM);
	if( pTradeConfirm_Window->GetShowStatus() ) return;

	// 물어보고 사기
	char	cType = 0;
	short	sID = 0;
	int		itemNo = -1;

	if(g_pInventory_Window)
	{
		if(g_pInventory_Window->GetShowStatus())
		{
			m_LastDraggingSlot = GetSlotIndex();
			if(GetItemPropertyFromSlot(m_LastDraggingSlot, cType, sID, itemNo))
			{
				m_MouseSlotItem.Set_m_cType(cType);
				m_MouseSlotItem.Set_m_sID(sID);
				m_MouseSlotItem.m_InventoryNumber = itemNo;

				if(m_MouseSlotItem.IsStackable())
				{
					_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)m_pMyManager->FindWindow(_XDEF_WTITLE_TRADECONFIRM);
					if(pTradeConfirm_Window)
					{
						pTradeConfirm_Window->ShowWindow(TRUE);
						pTradeConfirm_Window->m_IMEControl.SetFocus();
						pTradeConfirm_Window->m_SelectedCategory1 = m_MouseSlotItem.Get_m_cType();
						pTradeConfirm_Window->m_SelectedItemID = m_MouseSlotItem.Get_m_sID();
						pTradeConfirm_Window->m_TradeMode = _XNPCTRADE_BUY;
						m_LastDraggingSlot = -1;
					}
				}
				else
				{
					_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
					TCHAR		namestring[128];
					memset(namestring, 0, sizeof(TCHAR)*128);
					strcpy(namestring, _XGameItem::GetItemName(m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID()));
					
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INVENTORY_BUY), namestring);	// _T("%s(을)를 구입하시겠습니까?")
					
					pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_NPCTRADE_BUYOK, _XDEF_NPCTRADE_BUYNO);
				}
			}
		}
	}
}

void _XWindow_NPCTrade::DrawSelectedItem(void)
{
	if(!m_bDrawSelectedItem)
	{
		int selectedslot = GetSlotIndex();
		char cType = 0;
		short sID = 0;
		int itemNo = 0;
		
		if(GetItemPropertyFromSlot(selectedslot, cType, sID, itemNo))
		{
			int selectedslot2 = selectedslot % _XDEF_MAX_NPCTRADESLOT;
			if(selectedslot2 != -1)
			{
				RECT iconrect;
				iconrect.left = m_WindowPosition.x + m_SlotPosition[selectedslot2].x;
				iconrect.top = m_WindowPosition.y + m_SlotPosition[selectedslot2].y;
				iconrect.right = m_WindowPosition.x + m_SlotPosition[selectedslot2].x+31;
				iconrect.bottom = m_WindowPosition.y + m_SlotPosition[selectedslot2].y+31;
				
				_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
				DrawItemTooltip(selectedslot, iconrect);
			}
		}		
		return;
	}

	BOOL	selectedslotarray[_XDEF_MAX_NPCTRADESLOT];
	memset(selectedslotarray, 0, sizeof(BOOL)*_XDEF_MAX_NPCTRADESLOT);

	if(m_MouseSlotItem.Get_m_sID() > 0)
	{
		char cType = 0;
		short sID = 0;
		int selectedslot = 0;		
		if(GetItemPropertyFromNo(m_MouseSlotItem.m_InventoryNumber, cType, sID, selectedslot))
		{
			int selectedslot2 = selectedslot % _XDEF_MAX_NPCTRADESLOT;
			_XDrawRectAngle(m_WindowPosition.x+m_SlotPosition[selectedslot2].x,
							m_WindowPosition.y+m_SlotPosition[selectedslot2].y,
							m_WindowPosition.x+m_SlotPosition[selectedslot2].x+31,
							m_WindowPosition.y+m_SlotPosition[selectedslot2].y+31, 1.0f, _XSC_ITEM_SELECT);
			selectedslotarray[selectedslot2] = TRUE;

			if(m_bHaveMouseSlot)
			{
				// disable icon image
				_XDrawSolidBar(m_WindowPosition.x+m_SlotPosition[selectedslot2].x+1,
								m_WindowPosition.y+m_SlotPosition[selectedslot2].y+1,
								m_WindowPosition.x+m_SlotPosition[selectedslot2].x+30, 
								m_WindowPosition.y+m_SlotPosition[selectedslot2].y+30, D3DCOLOR_ARGB(160, 0, 0, 0));
			}
		}
	}

	int selectedslot = GetSlotIndex();
	char cType = 0;
	short sID = 0;
	int itemNo = 0;

	if(GetItemPropertyFromSlot(selectedslot, cType, sID, itemNo))
	{
		int selectedslot2 = selectedslot % _XDEF_MAX_NPCTRADESLOT;
		if(selectedslot != -1 && !selectedslotarray[selectedslot2])
		{
			RECT iconrect;
			iconrect.left = m_WindowPosition.x + m_SlotPosition[selectedslot2].x;
			iconrect.top = m_WindowPosition.y + m_SlotPosition[selectedslot2].y;
			iconrect.right = m_WindowPosition.x + m_SlotPosition[selectedslot2].x+31;
			iconrect.bottom = m_WindowPosition.y + m_SlotPosition[selectedslot2].y+31;
			
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
			DrawItemTooltip(selectedslot, iconrect);
		}
	}	
}

#ifdef _XTS_ITEM_OPTIMIZATION
void _XWindow_NPCTrade::DrawItemTooltip(int slotnumber, RECT iconrect)
{
	char cType = 0;
	short sID = 0;
	int itemNo = 0;

	if(GetItemPropertyFromSlot(slotnumber, cType, sID, itemNo))
	{
		if ( sID == 0 ) return;		

		int tempMoney = 0;
		TCHAR tempbuf[256];
		memset(tempbuf, 0, sizeof(TCHAR)*256 );

		if( m_BlackMarketMode )
		{
			tempMoney = m_GambleItemInfo[slotnumber].Price;

			switch( GetEqualItemType(cType) ) 
			{
			case _XGI_FC_WEAPON :
				{
					sprintf( tempbuf, "%s : %d", _XGameItem::GetItemName(cType, sID), tempMoney );
				}
				break;
			case _XGI_FC_CLOTHES :
				{
					if( g_ClothesItemProperty[sID].cSexType )
					{
						sprintf( tempbuf, _XGETINTERFACETEXT(ID_STRING_NPCTRADE_2276),
							g_pClothesItemClanName[g_ClothesItemProperty[sID].cClan], 
							g_pClothesItemSecondName[g_ClothesItemProperty[sID].cSecondType], tempMoney );
					}
					else
					{
						sprintf( tempbuf, _XGETINTERFACETEXT(ID_STRING_NPCTRADE_2277), 
							g_pClothesItemClanName[g_ClothesItemProperty[sID].cClan], 
							g_pClothesItemSecondName[g_ClothesItemProperty[sID].cSecondType], tempMoney );
					}
				}
				break;
			}
		}
		else
		{
			tempMoney = _XGameItem::GetItemPrice(cType, sID);

#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE
			// 지역 세율 계산 - 살 때
			u_int orgmoney = tempMoney;

			FLOAT correction = 0.4f;
			int tvalue = orgmoney % 10;
			if(tvalue == 6)
			{
				correction = 0.5f;
			}

			u_int tax_money = static_cast<u_int>( ((float)tempMoney*(float)(g_nMapTaxRate)/100.0f) + correction );	//<== 세금으로 걷는 돈.
			tempMoney += tax_money;
#endif
			bool checkmatch = false;
			sprintf( tempbuf, "%s : %d", _XGameItem::GetItemName(cType, sID), tempMoney );
		}		
		
		if( CheckCondition(cType, sID) )
			g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, _XGameItem::GetGradeColor(cType, sID), TRUE);
		else
			g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, D3DCOLOR_ARGB(255, 237, 28, 36), TRUE);
	}
}
#else
void _XWindow_NPCTrade::DrawItemTooltip(int slotnumber, RECT iconrect)
{
	char cType = 0;
	short sID = 0;
	int itemNo = 0;

	if(GetItemPropertyFromSlot(slotnumber, cType, sID, itemNo))
	{
		if ( sID == 0 ) return;

		int tempMoney = 0;
		TCHAR tempbuf[256];
		memset(tempbuf, 0, sizeof(TCHAR)*256 );

		if( m_BlackMarketMode )
		{
			tempMoney = m_GambleItemInfo[slotnumber].Price;

			switch(cType) 
			{
			case _XGI_FC_WEAPON :	
			case _XGI_FC_WEAPON2 :	
			case _XGI_FC_WEAPON3 :	
				{
					sprintf( tempbuf, "%s : %d", _XGameItem::GetItemName(cType, sID), tempMoney );
				}
				break;
			case _XGI_FC_CLOTHES :
				{
					if( g_ClothesItemProperty[sID].cSexType )
					{
						sprintf( tempbuf, _XGETINTERFACETEXT(ID_STRING_NPCTRADE_2276),
							g_pClothesItemClanName[g_ClothesItemProperty[sID].cClan], 
							g_pClothesItemSecondName[g_ClothesItemProperty[sID].cSecondType], tempMoney );
					}
					else
					{
						sprintf( tempbuf, _XGETINTERFACETEXT(ID_STRING_NPCTRADE_2277), 
							g_pClothesItemClanName[g_ClothesItemProperty[sID].cClan], 
							g_pClothesItemSecondName[g_ClothesItemProperty[sID].cSecondType], tempMoney );
					}
				}
				break;
			}
		}
		else
		{			
			bool checkmatch = false;
			switch(cType) 
			{				
				case _XGI_FC_WEAPON:
					{
						tempMoney = g_WeaponItemProperty[sID].uiPrice;
					}					
					break;
				case _XGI_FC_CLOTHES:
					{
						tempMoney = g_ClothesItemProperty[sID].uiPrice;				
					}					
					break;				
				case _XGI_FC_BOOK :
					{
						tempMoney = g_BookItemProperty[sID].uiPrice;
					}					
					break;
				case _XGI_FC_POTION:
					{
						tempMoney = g_PotionItemProperty[sID].uiPrice;				
					}
					break;
				case _XGI_FC_CONSUMING :
					{
						tempMoney = g_ConsumingItemProperty[sID].uiPrice;
					}
					break;
				case _XGI_FC_ELIXIR :
					{
						tempMoney = g_ElixirItemProperty[sID].uiPrice;
					}
					break;
				case _XGI_FC_ACCESSORY :
					{
						tempMoney = g_AccessoryItemProperty[sID].uiPrice;
					}
					break;
				case _XGI_FC_LIFE :
					{
						tempMoney = g_LifeItemProperty[sID].uiPrice;
					}
					break;
				case _XGI_FC_SOCKET :
					{
						tempMoney = g_SocketItemProperty[sID].uiPrice;
					}
					break;
				case _XGI_FC_RESOURCE :
					{
						tempMoney = g_ResourceItemProperty[sID].uiPrice;
					}
					break;
				case _XGI_FC_WEAPON2:
					{
						tempMoney = g_WeaponItemProperty2[sID].uiPrice;
					}
					break;
				case _XGI_FC_CLOTHES2:
					{
						tempMoney = g_ClothesItemProperty2[sID].uiPrice;				
					}
					break;
				case _XGI_FC_BOX :
					{
						tempMoney = g_BoxItemProperty[sID].uiPrice;
					}
					break;
				case _XGI_FC_BOXKEY :
					{
						tempMoney = g_BoxKeyItemProperty[sID].uiPrice;
					}
					break;
				case _XGI_FC_WEAPON3:
					{
						tempMoney = g_WeaponItemProperty3[sID].uiPrice;
					}
					break;
				case _XGI_FC_CLOTHES3:
					{
						tempMoney = g_ClothesItemProperty3[sID].uiPrice;				
					}
					break;
				default:
					tempMoney = 0;
					break;
			}
			
			sprintf( tempbuf, "%s : %d", _XGameItem::GetItemName(cType, sID), tempMoney );
		}		
		
		if( CheckCondition(cType, sID) )
			g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, _XGameItem::GetGradeColor(cType, sID), TRUE);
		else
			g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, D3DCOLOR_ARGB(255, 237, 28, 36), TRUE);
	}
}
#endif

void _XWindow_NPCTrade::SetNPCJobIcon(int jobindex)
{
	// 임시 
//	if( jobindex == 15 ) jobindex = 13;
//	else if( jobindex == 16 ) jobindex = 14;
//
//	int x, y;
//	y = jobindex / 5;
//	x = jobindex % 5;
//
//	x = 4 + (x*48);
//	y = 4 + (y*48);
//	m_NPCJobIcon->SetClipRect( x, y, x+40, y+40);
	
	switch(jobindex) {
	case _XNPC_JOBCLASS_FOOD:
		{
			m_NPCJobIcon->SetClipRect(  9, 9, 39, 39 );
		}
		break;
	case _XNPC_JOBCLASS_CLOTHES:
		{
			m_NPCJobIcon->SetClipRect(  57, 9, 87, 39 );
		}
		break;
	case _XNPC_JOBCLASS_REFINING:
		{
			m_NPCJobIcon->SetClipRect(  105, 9, 135, 39 );
		}
		break;
	case _XNPC_JOBCLASS_HOTEL:
		{
			m_NPCJobIcon->SetClipRect(  153, 9, 183, 39 );
		}
		break;
	case _XNPC_JOBCLASS_WAREHOUSE:
		{
			m_NPCJobIcon->SetClipRect(  202, 9, 232, 39 );
		}
		break;
	case _XNPC_JOBCLASS_DRUGSTORE:
		{
			m_NPCJobIcon->SetClipRect(  9, 57, 39, 87 );
		}
		break;
	case _XNPC_JOBCLASS_SHOP:
		{
			m_NPCJobIcon->SetClipRect(  57, 57, 87, 87 );
		}
		break;
	case _XNPC_JOBCLASS_WEAPON:
		{
			m_NPCJobIcon->SetClipRect(  105, 57, 135, 87 );
		}
		break;
	case _XNPC_JOBCLASS_COMMONPEOPLE:
		{
			m_NPCJobIcon->SetClipRect(  153, 57, 183, 87 );
		}
		break;
	case _XNPC_JOBCLASS_GAEBANGPEOPLE:
		{
			m_NPCJobIcon->SetClipRect(  202, 57, 232, 87 );
		}
		break;
	case _XNPC_JOBCLASS_SORIMPEOPLE:
		{
			m_NPCJobIcon->SetClipRect(  9, 105, 39, 135 );
		}
		break;
	case _XNPC_JOBCLASS_NOCKRIMPEOPLE:
		{
			m_NPCJobIcon->SetClipRect(  57, 105, 87, 135 );
		}
		break;
	case _XNPC_JOBCLASS_BEEGOONGPEOPLE:
		{
			m_NPCJobIcon->SetClipRect(  105, 105, 135, 135 );
		}
		break;
	case _XNPC_JOBCLASS_MOODANGPEOPLE:
		{
			m_NPCJobIcon->SetClipRect(  9, 153, 39, 183 );
		}
		break;
	case _XNPC_JOBCLASS_MAKYOPEOPLE:
		{
			m_NPCJobIcon->SetClipRect(  57, 153, 87, 183 );
		}
		break;
	case _XNPC_JOBCLASS_BLACKMARKET:
		{
			m_NPCJobIcon->SetClipRect(  153, 105, 183, 135 );
		}
		break;
	case _XNPC_JOBCLASS_MILITARYOFFICER:
		{
			m_NPCJobIcon->SetClipRect(  202, 105, 232, 135 );
		}
		break;		
	case _XNPC_JOBCLASS_HEANGSANGIN:
		{
			m_NPCJobIcon->SetClipRect(  153, 153, 183, 183 );
		}
		break;		
	case _XNPC_JOBCLASS_BUFF:
		{
			m_NPCJobIcon->SetClipRect(  202, 153, 232, 183 );
		}
		break;		
	case _XNPC_JOBCLASS_RESOURCE:
		{
			m_NPCJobIcon->SetClipRect(  9, 202, 39, 232 );
		}
		break;		
	case _XNPC_JOBCLASS_COLLECTION:
		{
			m_NPCJobIcon->SetClipRect(  57, 202, 87, 232 );
		}
		break;		
	case _XNPC_JOBCLASS_GUARD:
		{
			m_NPCJobIcon->SetClipRect(  105, 202, 135, 232 );
		}
		break;		
	case _XNPC_JOBCLASS_REPAIR:
		{
			m_NPCJobIcon->SetClipRect(  153, 202, 183, 232 );
		}
		break;		
	case _XNPC_JOBCLASS_CASTLECAPTAIN:
		{
			//일단 백성 아이콘을 사용 아이콘이 준비되면 따로 처리
			m_NPCJobIcon->SetClipRect(  153, 57, 183, 87 );
		}
		break;		
	}
}

BOOL _XWindow_NPCTrade::CheckCondition( char cType, int sId )
{
	if( sId <= 0 ) return TRUE;
		
	switch(cType) 
	{	
	case _XGI_FC_CLOTHES:
		{
			if( g_pLocalUser->m_CharacterInfo.gender != g_ClothesItemProperty[sId].cSexType )
				return FALSE;
			if( g_pLocalUser->m_CharacterInfo.groupindex != g_ClothesItemProperty[sId].cClan &&
				g_ClothesItemProperty[sId].cClan != _XGROUP_NANGIN )
				return FALSE;
		}		
		break;
	case _XGI_FC_CLOTHES2:
		{
			if( g_pLocalUser->m_CharacterInfo.gender != g_ClothesItemProperty2[sId].cSexType )
				return FALSE;
			if( g_pLocalUser->m_CharacterInfo.groupindex != g_ClothesItemProperty2[sId].cClan &&
				g_ClothesItemProperty[sId].cClan != _XGROUP_NANGIN )
				return FALSE;
		}		
		break;
	case _XGI_FC_CLOTHES3:
		{
			if( g_pLocalUser->m_CharacterInfo.gender != g_ClothesItemProperty3[sId].cSexType )
				return FALSE;
			if( g_pLocalUser->m_CharacterInfo.groupindex != g_ClothesItemProperty3[sId].cClan &&
				g_ClothesItemProperty[sId].cClan != _XGROUP_NANGIN )
				return FALSE;
		}		
		break;
	case _XGI_FC_BOOK :
		{
			
		}
		break;
	case _XGI_FC_POTION:
		{
			
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			
		}
		break;
	case _XGI_FC_LIFE :
		{
			
		}
		break;
	case _XGI_FC_SOCKET :
		{
			
		}
		break;	
	}	
	return TRUE;
}

int	_XWindow_NPCTrade::PriceCalc(int slotnumber, int count )
{
	int reslutPrice = 0;	
	int TempType = g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType();
	int TempId = g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID();
	int TempCount = count;		
	float tempmoney = 0;
	
	if( TempId <= 0 )		return 0;	
	
	if( g_pLocalUser->m_UserItemList[slotnumber].Get_m_usMaxDur() > 0 )
	{
		float TempRet = (float)g_pLocalUser->m_UserItemList[slotnumber].Get_m_usCurDur() / (float)g_pLocalUser->m_UserItemList[slotnumber].Get_m_usMaxDur();
		
		if(_XGameItem::GetItemPrice(TempType, TempId) == 0)
		{
			tempmoney = 0;
		}
		else
		{
			tempmoney = TempRet * static_cast<float>((_XGameItem::GetItemPrice( TempType, TempId))/5.0f * TempCount );
		}
	}
	else
	{
		if(_XGameItem::GetItemPrice(TempType, TempId) == 0)
		{
			tempmoney = 0;
		}
		else
		{
			tempmoney = static_cast<float>(( _XGameItem::GetItemPrice( TempType, TempId)/5.0f ) * TempCount );
		}
	}
	
	if( tempmoney > 0.0f && tempmoney < 1.0f )
	{
		reslutPrice = 1;
	}
	else
	{
		reslutPrice = static_cast<int>( tempmoney );
	}	

	return reslutPrice;
}

#define _XDEF_SCRIPT_GAMBLEITEM_FROMPACKAGE

void _XWindow_NPCTrade::LoadGambleItemTradePackage(int pattenindex)
{
	list <_XTradeItem>::iterator iter_item;
	for(iter_item = m_TradeItemList.begin() ; iter_item != m_TradeItemList.end() ; )
	{
		iter_item = m_TradeItemList.erase(iter_item);
	}
	m_TradeItemList.clear();	

	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

	FILE*	fp;

#ifdef _XDEF_SCRIPT_GAMBLEITEM_FROMPACKAGE	
	if( !g_ScriptArchive.ReOpenPackage() )
			return;

	if( (fp = g_ScriptArchive.GetPackedFile( _T("gamblelist_01.txt") ) ) == NULL )
	{
		_XFatalError("Can't load GambleItemList");
		g_ScriptArchive.ClosePackage();
	}
#else
	_XSetDefaultPath( _T("./Data/Script/TextFormat") );
	
	if( (fp = fopen( "gamblelist_01.txt", "rt" )) == NULL )	
	{
		_XFatalError("Can't load GambleItemList");
		fclose(fp);		
	}
#endif	
	
	TCHAR buffer[128];
	TCHAR findnpc[64];
	memset( findnpc, 0, 64 );
	sprintf( findnpc, "_NPCID_%d", m_OwnerNPCID );

	BOOL flag = FALSE;
	while( !feof(fp) )
	{
		if( flag )	break;

		memset( buffer, 0, 128 );
		fscanf( fp, "%s", buffer );

		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		if( buffer[0] == ';' || buffer[0] == '\n' || !strlen(buffer) ) continue;

		// NPC 를 찾고 
		if( _stricmp( buffer, findnpc ) == 0 )
		{
			flag = TRUE;
			TCHAR lastfindindex[128];			
			memset( lastfindindex, 0, 128 );			

			sprintf(lastfindindex, "%s_P%d", findnpc, pattenindex );

			while( !feof(fp) )
			{
				memset( buffer, 0, 128 );
				fscanf( fp, "%s", buffer );

				if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
				if( buffer[0] == ';' || buffer[0] == '\n' || !strlen(buffer) ) continue;

				// NPC 의 패턴을 찾고 
				if( _stricmp( buffer, lastfindindex ) == 0 )
				{
					int pattencount = 0;
					fscanf( fp, "%s", buffer );
					pattencount = atoi(buffer);

					SAFE_DELETE_ARRAY( m_GambleItemPatten );
					SAFE_DELETE_ARRAY( m_GambleItemInfo );

					m_GambleItemPatten = new int[pattencount];
					m_GambleItemInfo = new _XGambleItem[pattencount];
					memset( m_GambleItemPatten, 0, sizeof(int) * pattencount );
					memset( m_GambleItemInfo, 0, sizeof(_XGambleItem) * pattencount );

					int			count = 0;
					TCHAR		findItemNo[128];

					// 패턴 리스트
					for( int i = 0; i < pattencount; i++ )
					{
						fscanf(fp, "%d", &m_GambleItemPatten[i]);	
					}
					
					long length = ftell( fp );	// 여기까지 길이를 구한다.

					for( i = 0; i < pattencount; i++ )
					{						
						if( fseek( fp, length, SEEK_SET ) )	// 실패하면 다시 처음으로 되돌린다.
							rewind(fp);

						memset(findItemNo, 0, sizeof(TCHAR) * 128 );
						sprintf(findItemNo, "_ITEMNO_%d", m_GambleItemPatten[i] );

						while( !feof(fp) )
						{
							memset( buffer, 0, 128 );
							fscanf( fp, "%s", buffer );

							if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
							if( buffer[0] == ';' || buffer[0] == '\n' || !strlen(buffer) ) continue;
							
							// index를 찾고
							if( _stricmp( buffer, findItemNo ) == 0 )
							{
								fscanf(fp, "%d", &m_GambleItemInfo[i].ItemNo);
								fscanf(fp, "%d", &m_GambleItemInfo[i].cluindex);
								fscanf(fp, "%d", &m_GambleItemInfo[i].Type);
								fscanf(fp, "%d", &m_GambleItemInfo[i].Id);
								fscanf(fp, "%d", &m_GambleItemInfo[i].Price);

								_XTradeItem tradeitem;
								tradeitem.Set_m_cType(m_GambleItemInfo[i].Type);
								tradeitem.Set_m_sID(m_GambleItemInfo[i].Id);
								tradeitem.m_InventoryNumber = count;
								tradeitem.m_ItemNo = i;
								m_TradeItemList.push_back(tradeitem);
								count++;
								break;
							}
						}						
					}

					if( count > 0 )
					{
						int tempcount = count % _XDEF_MAX_NPCTRADESLOT;
						if( tempcount == 0 )
						{
							count /= _XDEF_MAX_NPCTRADESLOT;
							count--;
						}
						else
						{
							count /= _XDEF_MAX_NPCTRADESLOT;
						}	
						
						for( i = 0; i < 5; i++ )
						{
							m_SelectTabButtton[i]->ShowWindow( i <= count );
						}
					}
					break;
				}
			}
		}
	}

#ifdef _XDEF_SCRIPT_GAMBLEITEM_FROMPACKAGE						
	g_ScriptArchive.ClosePackage();					
#else
	fclose( fp );
#endif
}
