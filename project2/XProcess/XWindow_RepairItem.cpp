// XWindow_RepairItem.cpp: implementation of the _XWindow_RepairItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "XWindow_RepairItem.h"
#include "XWindow_TradeConfirm.h"
#include "xwindow_chargegauge.h"
#include "XMessageWindow.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XMessageWindow.h"
#include "XWindowObjectDefine.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static int _XDEF_REPAIRITEMPOSX = 17;
static int _XDEF_REPAIRITEMPOSY = 36;

_XWindow_RepairItem::_XWindow_RepairItem()
{
	m_pRepairListBox = NULL;
	m_SelectedAllButton = NULL;
	m_RepairButton = NULL;
	m_SelectedCancleButton = NULL;		
	m_NeedMoney = 0;
	
	// drag
	m_MouseSlotItem.ResetMouseSlotItem();
	m_bDragState		= FALSE;
	m_bHaveMouseSlot	= FALSE;
	m_bDrawSelectedItem = FALSE;
	m_IconDragStartPos.x = m_IconDragStartPos.y = 0;
	m_LastDraggingSlot	= -1;	

	m_RepairMode = RM_NONE;
	m_EndRepairDelayTime = 0.0f;

	ResetRepairListData();
}

_XWindow_RepairItem::~_XWindow_RepairItem()
{

}

BOOL _XWindow_RepairItem::Initialize( void )
{
	// Border---------------------------------------------------------------------------------------------=
	int repairimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create(0, 0, 184, 70, &g_MainInterfaceTextureArchive, repairimageindex );
	pTitleBar->SetClipRect( 0, 125, 183, 194 );
	InsertChildObject(pTitleBar);

	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create( 0, 59, 184, 269, &g_MainInterfaceTextureArchive, 
						   g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ));
	pBottomBorder->SetClipRect( 0, 0,  183, 211 );
	InsertChildObject( pBottomBorder );	

	m_ItemEdgeIcon.Create( 0, 0, 34, 34, &g_MainInterfaceTextureArchive, repairimageindex );
	m_ItemEdgeIcon.SetClipRect( 213, 64, 247, 98 );	
	// ---------------------------------------------------------------------------------------------------=

	// Line ----------------------------------------------------------------------------------------------=
	repairimageindex = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );

	_XImageStatic* pTopWindowLine = new _XImageStatic;
	pTopWindowLine->Create( 5, 31, 178, 33, &g_MainInterfaceTextureArchive, repairimageindex );
	pTopWindowLine->SetClipRect( 21, 490, 195, 493 );
	InsertChildObject( pTopWindowLine );

	_XImageStatic* pLeftWindowLine = new _XImageStatic;
	pLeftWindowLine->Create( 5, 34, 7, 171, &g_MainInterfaceTextureArchive, repairimageindex );
	pLeftWindowLine->SetClipRect( 495, 283, 498, 421 );
	InsertChildObject( pLeftWindowLine );


	_XImageStatic* pRightWindowLine = new _XImageStatic;
	pRightWindowLine->Create( 176, 34, 178, 171, &g_MainInterfaceTextureArchive, repairimageindex );
	pRightWindowLine->SetClipRect( 501, 283, 504, 421 );
	InsertChildObject( pRightWindowLine );
	
	_XImageStatic* pBottomWindowLine = new _XImageStatic;
	pBottomWindowLine->Create( 5, 172, 178, 174, &g_MainInterfaceTextureArchive, repairimageindex );
	pBottomWindowLine->SetClipRect( 21, 500, 195, 503 );
	InsertChildObject( pBottomWindowLine );	
	// ---------------------------------------------------------------------------------------------------=

	// 전체선택, 전체 해제 Button-------------------------------------------------------------------------=
	 _XBTN_STRUCTURE allbtnstruct = { TRUE, { 9, 221 }, { 81, 18 }, _XDEF_REPAIRITEMWINDOW_SELECTEDALL,
		repairimageindex,repairimageindex,repairimageindex, 
		&g_MainInterfaceTextureArchive };
	m_SelectedAllButton = new _XButton;
	m_SelectedAllButton->Create( allbtnstruct );	
	
	m_SelectedAllButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 429,  39, 510,  57 );
	m_SelectedAllButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  429,  59, 510,  77 );	
	m_SelectedAllButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  429,  79, 510,  97 );
	m_SelectedAllButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REPAIRITEM_SELECTALL );
	InsertChildObject( m_SelectedAllButton );

	allbtnstruct.position.x = 95;
	allbtnstruct.commandid++;		
	m_SelectedCancleButton = new _XButton;
	m_SelectedCancleButton->Create( allbtnstruct );	
	
	m_SelectedCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 429,  39, 510,  57 );
	m_SelectedCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  429,  59, 510,  77 );	
	m_SelectedCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  429,  79, 510,  97 );
	m_SelectedCancleButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REPAIRITEM_SELECTREMOVAL );	// 선택해제
	InsertChildObject( m_SelectedCancleButton );
	// ---------------------------------------------------------------------------------------------------=

	// 수리 Button ---------------------------------------------------------------------------------------=
	repairimageindex = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_03.tga" );

	_XBTN_STRUCTURE repairbtnstruct = { TRUE, { 9, 241 }, { 165, 22 }, _XDEF_REPAIRITEMWINDOW_REPAIRBUTTON,
		repairimageindex,repairimageindex,repairimageindex, 
		&g_MainInterfaceTextureArchive };
	m_RepairButton = new _XButton;
	m_RepairButton->Create( repairbtnstruct );	
	
	m_RepairButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  91, 183, 256, 205 );
	m_RepairButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,   91, 208, 256, 230 );	
	m_RepairButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,   91, 233, 256, 255 );
	m_RepairButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REPAIRITEM_REPAIR );	// 수리
	InsertChildObject( m_RepairButton );
	// ---------------------------------------------------------------------------------------------------=

	// close, help Button ---------------------------------------------------------------------=	
	repairimageindex = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	/*_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_REPAIRITEMWINDOW_CLOSE,
		repairimageindex,repairimageindex,repairimageindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );*/
	
	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_REPAIRITEMWINDOW_HELP,
		repairimageindex,repairimageindex,repairimageindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	// ----------------------------------------------------------------------------------------=
	
	// Scroll Bar -----------------------------------------------------------------------------=
	repairimageindex = g_MainInterfaceTextureArchive.FindResource("MI_chat_quick.tga");

	_XLISTBOX_STRUCTURE repairstructure = 
	{
		TRUE,
		{m_WindowPosition.x+_XDEF_REPAIRITEMPOSX, m_WindowPosition.y+_XDEF_REPAIRITEMPOSY},
		{149, 133},
		_XDEF_REPAIRITEMWINDOW_LISTBOX,
		-1,
		&g_MainInterfaceTextureArchive,
		_T(""),
		34,
		4,
		_XLSTYLE_RIGHTSCROLLBAR,
		{12, 134},
		12,
		22,
		19,
		repairimageindex,
		repairimageindex,
		repairimageindex,
		repairimageindex,
		repairimageindex
	};
	m_pRepairListBox = new _XListBox;
	m_pRepairListBox->Create(repairstructure);
		

	_XVScrollBar* pScrollBar = m_pRepairListBox->GetScrollBarObject();
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
		pScrollBar->SetViewNullBorder(FALSE);
	}
	
	m_pRepairListBox->SetIconReportMode(TRUE);
	m_pRepairListBox->SetIconReportModeInfo(4, 4, 33, 33);
	
	m_pRepairListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);
	m_pRepairListBox->SetColumnStyle(1, _XLCOLUMNSTYLE_ICONRECT);

	m_pRepairListBox->SetColumnOffset(0, 0, 0);
	m_pRepairListBox->SetColumnOffset(1, 2, 2);

	m_pRepairListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_pRepairListBox->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);

	m_pRepairListBox->SetColumnWidth(0, 33);
	m_pRepairListBox->SetColumnWidth(1, 33);	

	for( int i = 0; i < _XDEF_REPAIR_MAX_SENDCOUNT; i++ )
	{
		m_RepairSlotPosition[i].x = _XDEF_REPAIRITEMPOSX + ((i%4)*34);
		m_RepairSlotPosition[i].y = _XDEF_REPAIRITEMPOSY + ((i/4)*34);
	}
	// ----------------------------------------------------------------------------------------=

	return TRUE;
}
void _XWindow_RepairItem::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
	m_MouseSlotItem.ResetMouseSlotItem();
	m_SelectedSlotNoList.clear();
	SAFE_DELETE( m_pRepairListBox );
}
void _XWindow_RepairItem::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;

	DrawDefaultSlot();
	m_pRepairListBox->Draw();	

	_XDrawSolidBar( m_WindowPosition.x+9, m_WindowPosition.y+177, m_WindowPosition.x+173, m_WindowPosition.y+217, 0xFF1A1819 );
	_XDrawRectAngle( m_WindowPosition.x+9, m_WindowPosition.y+177, m_WindowPosition.x+173, m_WindowPosition.y+217, 0.0f, 0xFF000000 );
	DrawRepairText();
	DrawSelectedItem();
}
BOOL _XWindow_RepairItem::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_ShowWindow)
		return FALSE;
	
	if(!this->m_Enable)
		return FALSE;
	
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
					if(scrnpos->x > m_WindowPosition.x+10 && scrnpos->x < m_WindowPosition.x+172)
					{
						if(scrnpos->z > m_WindowPosition.y+33 && scrnpos->z < m_WindowPosition.y+172)
						{
							pfocusobject = _XDEF_DUMMYID_REPAIR;
						}
					}

					if(g_pInventory_Window)
					{
						if(g_pInventory_Window->GetShowStatus())
						{
							if(g_pInventory_Window->m_bHaveMouseSlot && g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)
							{
								if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sID() > 0 )
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType()) ||
										_XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType()) )
#else
									if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() == _XGI_FC_WEAPON ||
										g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() ==  _XGI_FC_CLOTHES ||
										g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() == _XGI_FC_WEAPON2 ||
										g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() ==  _XGI_FC_CLOTHES2 ||
										g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() == _XGI_FC_WEAPON3 ||
										g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() ==  _XGI_FC_CLOTHES3 )
#endif
									{
										int shortdur = g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_usMaxDur() - 
														g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_usCurDur();
										if( shortdur > 0)
										{
											m_SelectedSlotNoList.push_back( g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber );
											SetRepairListItem();
										}
										else
										{
											_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
											pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTNECESSERYREPAIR), TRUE);	// 수리할 필요가 없는 물품입니다.
											pMessageBox->StartTimer(5000, TRUE);
										}
										
										g_pInventory_Window->m_bHaveMouseSlot = FALSE;
										g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();									
									}
									else
									{
										_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
										pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTREPAIRITEM), TRUE);		// 수리 할 수 없는 물품입니다.
										pMessageBox->StartTimer(5000, TRUE);
										
										g_pInventory_Window->m_bHaveMouseSlot = FALSE;
										g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
									}

								}								
							}
						}
					}
					
					if(!m_bHaveMouseSlot)
					{
						// mouse down 위치 저장
						m_IconDragStartPos.x = scrnpos->x;
						m_IconDragStartPos.y = scrnpos->z;
						m_LastDraggingSlot = GetRepairSlotIndex();
						m_bDragState = TRUE;
					}
					else
					{
						m_MouseSlotItem.ResetMouseSlotItem();
						m_bHaveMouseSlot = FALSE;
					}					
					
					if(g_pInventory_Window )
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
					// Tooltip show false
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
						{
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
							m_LastDraggingSlot = -1;
						}
					}

					if( g_pInventory_Window && g_pInventory_Window->GetShowStatus() )
					{

					}
					else
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
		if(pfocusobject == _XDEF_DUMMYID_REPAIR)
		{
			if(mousestate->bButton[0])
			{
				// 드래그 처리
				if(m_bDragState && !m_bHaveMouseSlot)
				{
					if(scrnpos->x != m_IconDragStartPos.x || scrnpos->z != m_IconDragStartPos.y)
					{
						if( m_LastDraggingSlot < m_SelectedSlotNoList.size() )
						{
							list<int>::iterator iter = m_SelectedSlotNoList.begin();
							advance(iter, m_LastDraggingSlot );
							if(m_LastDraggingSlot != -1 )
							{
								m_bHaveMouseSlot = TRUE;
								m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[*iter]);
								m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
								m_LastDraggingSlot = -1;
								
								if( g_pDefaultTooltip_Window )
								{
									if( g_pDefaultTooltip_Window->GetShowStatus() )						
										g_pDefaultTooltip_Window->ShowWindow( FALSE );
								}
							}
						}						
					}
				} // if(m_bDragState && !m_bHaveMouseSlot)
			}

			if(!mousestate->bButton[0])
			{
				m_bDrawSelectedItem = TRUE;
				{
					char	cType = 0;
					short	sID = 0;

					if(m_bDragState)
					{
						if(scrnpos->x == m_IconDragStartPos.x && scrnpos->z == m_IconDragStartPos.y)
						{
							// 제자리에서 클릭 - 툴팁 보여주기
							if( m_LastDraggingSlot < m_SelectedSlotNoList.size() )
							{
								list<int>::iterator iter = m_SelectedSlotNoList.begin();
								advance( iter, m_LastDraggingSlot );
								
								m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[*iter]);
								m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;								
								if( g_pDefaultTooltip_Window )
								{
									g_pDefaultTooltip_Window->SetRepairItemWindowPtr(this);
									
									list<int>::iterator k = m_SelectedSlotNoList.begin();
									advance(k, m_LastDraggingSlot);
									int invennumber = *k;

									if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_REPAIRITEM, invennumber,
										g_pLocalUser->m_UserItemList[invennumber].Get_m_cType(), g_pLocalUser->m_UserItemList[invennumber].Get_m_sID() ) )
									{
										g_pDefaultTooltip_Window->ShowWindow( TRUE );
									}
								}
								m_LastDraggingSlot = -1;
								//m_bProcessOneClickItem = TRUE;
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
								int selectedslot = GetRepairSlotIndex();
								if(selectedslot == m_MouseSlotItem.m_InventoryNumber)
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
						}
					} // if(m_bDragState)
				} // ctrl
			} //if(!mousestate->bButton[0])
		}

		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				BOOL resetmouse = TRUE;
				if(!m_bHaveMouseSlot)
				{
					m_LastDraggingSlot = GetRepairSlotIndex();
					if( m_LastDraggingSlot < m_SelectedSlotNoList.size() )
					{
						list<int>::iterator iter = m_SelectedSlotNoList.begin();
						advance( iter, m_LastDraggingSlot );

						m_SelectedSlotNoList.erase( iter );
						SetRepairListItem();
						m_WindowMouseState = _XW_STATE_NONE;
						m_LastDraggingSlot = -1;
						resetmouse = FALSE;
					}
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
			}
		}
		
		if(!mousestate->bButton[0])
		{
			if(pfocusobject == _XDEF_DUMMYID_REPAIR)
				pfocusobject = NULL;
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_RepairItem::SetRepairListItem(void)
{
	m_NeedMoney = 0;
	g_NetworkKernel.SendPacket(MSG_NO_NPC_ITEM_REPAIR_REQUEST, 0);
	m_pRepairListBox->DeleteAllItem();	

	int resourceindex = g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	RECT backgroundrect = {213, 30, 247, 64};
	int iconresourceindex = -1;
	RECT iconrect;
	int	count = 0;

	list<int>::iterator iter = m_SelectedSlotNoList.begin();
	for( ; iter != m_SelectedSlotNoList.end(); iter++ )
	{
		m_pRepairListBox->InsertListItem(_T(""), 0, (*iter)+1 );

		m_pRepairListBox->SetIconArchive(count, &g_MainInterfaceTextureArchive);
		m_pRepairListBox->SetItemAttrib(count, 0, resourceindex, backgroundrect);
		
		g_pInventory_Window->GetIconRect( g_pLocalUser->m_UserItemList[*iter].Get_m_cType(), 
										  g_pLocalUser->m_UserItemList[*iter].Get_m_sID(),
										  iconresourceindex, iconrect, TRUE );

		if( iconresourceindex != -1 )
		{			
			m_pRepairListBox->SetItemAttrib(count, 1, iconresourceindex, iconrect);				
		}

		FLOAT temp = ( 1.0f - ( (FLOAT)g_pLocalUser->m_UserItemList[*iter].Get_m_usCurDur() / (FLOAT)g_pLocalUser->m_UserItemList[*iter].Get_m_usMaxDur() ) );
		FLOAT temp3 = (FLOAT)(_XGameItem::GetItemPrice( g_pLocalUser->m_UserItemList[*iter].Get_m_cType(), g_pLocalUser->m_UserItemList[*iter].Get_m_sID()) ) / 5.0f;
		FLOAT temp4 = temp * temp3;
				
		/*if( temp4 > 0.0f && temp4 < 1.0f )
		{
			m_NeedMoney += 1;
		}
		else
		{
			m_NeedMoney += static_cast<DWORD>(temp4);
		}*/
		
		count++;		
	}
	m_pRepairListBox->SetLineScrollPos(9999);
	if( m_SelectedSlotNoList.empty() )
	{
		m_RepairMode = RM_NONE;
	}
	else
		m_RepairMode = RM_READYREPAIR;
}

void _XWindow_RepairItem::ResetRepairListData(void)
{
	m_SelectedSlotNoList.clear();
	
	if( m_pRepairListBox )
		m_pRepairListBox->DeleteAllItem();
}

void _XWindow_RepairItem::DrawDefaultSlot(void)
{
	for( int i = 0; i < 16; i++ )
	{
		int tempX = i / 4;
		int tempY = i % 4;

		tempX = _XDEF_REPAIRITEMPOSX + (tempX*33);
		tempY = _XDEF_REPAIRITEMPOSY + (tempY*33);
		
		m_ItemEdgeIcon.Draw( m_WindowPosition.x+tempX, m_WindowPosition.y+tempY );
	}	
}

void _XWindow_RepairItem::DrawRepairText(void)
{
	g_XBaseFont->SetColor(0xFFC4A974);
	switch(m_RepairMode) 
	{
	case RM_NONE :
		{
			if( m_SelectedSlotNoList.empty() )
				g_XBaseFont->PutsAlign(	m_WindowPosition.x+91, m_WindowPosition.y+190, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_REPAIRITEM_SELECTITEM) );	// 수리할 물품을 선택하십시오.		
		}
		break;
	case RM_READYREPAIR:
		{
			if( !m_SelectedSlotNoList.empty() )
			{		
				TCHAR tempstr[64];
				sprintf( tempstr, _XGETINTERFACETEXT(ID_STRING_REPAIRITEM_ALLMONEY), m_NeedMoney );	// 전체 수리비용 : %d
				g_XBaseFont->PutsAlign(	m_WindowPosition.x+18, m_WindowPosition.y+190, _XFONT_ALIGNTYPE_LEFT, tempstr );
			}
		}
		break;	
	case RM_ENDREPAIR:
		{
			g_XBaseFont->PutsAlign(	m_WindowPosition.x+18, m_WindowPosition.y+190, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_REPAIRITEM_END) );	// _T("물품의 수리가 끝났습니다.")
			if( g_fLocalSystemTime - m_EndRepairDelayTime > 1000 )
				m_RepairMode = RM_NONE;
		}
		break;
	}

	g_XBaseFont->Flush();
}

int _XWindow_RepairItem::GetRepairSlotIndex(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	int PosX, PosY;
	PosX = scrnpos->x;
	PosY = scrnpos->z;

	PosX -= (m_WindowPosition.x + _XDEF_REPAIRITEMPOSX);
	if( PosX < 0 )
		return -1;
	PosX /= 33;	
	PosY -= (m_WindowPosition.y + _XDEF_REPAIRITEMPOSY);
	if( PosY < 0 )
		return -1;
	PosY /= 33;	

	if( PosX > 3 || PosY > 3 )
		return -1;

	int jumpcount = PosY * 4 + PosX;
	if( jumpcount > 16 ) return -1;

	int scrollpos = m_pRepairListBox->GetScrollPos();				
	jumpcount += scrollpos*m_pRepairListBox->GetColumnCount();	

	return jumpcount;
}

void _XWindow_RepairItem::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );
	m_pRepairListBox->MoveWindow( X+_XDEF_REPAIRITEMPOSX, Y+_XDEF_REPAIRITEMPOSY );
}

void _XWindow_RepairItem::ShowWindow(BOOL show)
{
	if(show)
	{
		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;
		}
		m_RepairButton->EnableWindow(TRUE);
		m_SelectedAllButton->EnableWindow(TRUE);		
		m_SelectedCancleButton->EnableWindow(TRUE);	
	}
	else
	{
		// 창고 사용 끝나면 Inventory lock 해제
		if(g_pInventory_Window)
		{
			if(g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = FALSE;
		}

		if( g_pDefaultTooltip_Window )
		{
			if( g_pDefaultTooltip_Window->GetShowStatus() )						
				g_pDefaultTooltip_Window->ShowWindow( FALSE );
		}
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_RepairItem::DrawSelectedItem(void)
{
	if(!m_bDrawSelectedItem)
	{
		int selectedslot = GetRepairSlotIndex();
		int selectedslot2 = selectedslot - (m_pRepairListBox->GetScrollPos() * m_pRepairListBox->GetColumnCount() );
		if(selectedslot != -1)
		{
			RECT iconrect;
			iconrect.left = m_WindowPosition.x + m_RepairSlotPosition[selectedslot2].x;
			iconrect.top = m_WindowPosition.y + m_RepairSlotPosition[selectedslot2].y;
			iconrect.right = m_WindowPosition.x + m_RepairSlotPosition[selectedslot2].x + 31;
			iconrect.bottom = m_WindowPosition.y + m_RepairSlotPosition[selectedslot2].y + 31;
			
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
		}
		return;
	}

	BOOL		selectedslotarray[_XDEF_REPAIR_MAX_SENDCOUNT];
	memset(selectedslotarray, 0, sizeof(BOOL)*_XDEF_REPAIR_MAX_SENDCOUNT);

	if(m_bHaveMouseSlot)
	{
		int selectedslot = m_MouseSlotItem.m_InventoryNumber;
		int selectedslot2 = selectedslot - (m_pRepairListBox->GetScrollPos() * m_pRepairListBox->GetColumnCount() );

		_XDrawRectAngle(m_WindowPosition.x+m_RepairSlotPosition[selectedslot2].x, 
						m_WindowPosition.y+m_RepairSlotPosition[selectedslot2].y, 
						m_WindowPosition.x+m_RepairSlotPosition[selectedslot2].x+31, 
						m_WindowPosition.y+m_RepairSlotPosition[selectedslot2].y+31, 
						1.0f, _XSC_ITEM_SELECT);
		selectedslotarray[selectedslot2] = TRUE;

		_XDrawSolidBar(m_WindowPosition.x+m_RepairSlotPosition[selectedslot2].x+1,
						m_WindowPosition.y+m_RepairSlotPosition[selectedslot2].y+1,
						m_WindowPosition.x+m_RepairSlotPosition[selectedslot2].x+30, 
						m_WindowPosition.y+m_RepairSlotPosition[selectedslot2].y+30, D3DCOLOR_ARGB(160, 0, 0, 0));
	}	
	
	int selectedslot = GetRepairSlotIndex();
	int selectedslot2 = selectedslot - (m_pRepairListBox->GetScrollPos() * m_pRepairListBox->GetColumnCount() );
	if(selectedslot != -1 && !selectedslotarray[selectedslot2])
	{
		RECT iconrect;
		iconrect.left = m_WindowPosition.x + m_RepairSlotPosition[selectedslot2].x;
		iconrect.top = m_WindowPosition.y + m_RepairSlotPosition[selectedslot2].y;
		iconrect.right = m_WindowPosition.x + m_RepairSlotPosition[selectedslot2].x+31;
		iconrect.bottom = m_WindowPosition.y + m_RepairSlotPosition[selectedslot2].y+31;

		_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
	}
}


void _XWindow_RepairItem::DrawDragIcon(void)
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

BOOL _XWindow_RepairItem::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	m_pRepairListBox->ProcessWheelMessage(zDelta);

	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}