// XWindow_Pocket.cpp: implementation of the _XWindow_Pocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindow_Pocket.h"
#include "XWindowObjectDefine.h"
#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static int _XDEF_POCKETITEMPOSX = 18;
static int _XDEF_POCKETITEMPOSY = 29;

_XWindow_Pocket::_XWindow_Pocket()
{
	ResetPocketItem();
}

_XWindow_Pocket::~_XWindow_Pocket()
{

}

void _XWindow_Pocket::ResetPocketItem(void)
{
	for( int i = 0; i < _XDEF_POCKET_MAX_SLOTNUMBER; i++ )
		m_AccessoryItemList[i].Reset();

	m_MouseSlotItem.ResetMouseSlotItem();
	m_bHaveMouseSlot		= NULL;
	m_IconDragStartPos.x	= m_IconDragStartPos.y = 0;
	m_bDragState			= NULL;
	m_LastDraggingSlot		= -1;	
}

BOOL _XWindow_Pocket::Initialize( void )
{
	int resourceimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );

	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].Create( 0, 0, 32, 32, &g_MainInterfaceTextureArchive, resourceimageindex );
	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].SetClipRect( 214, 65, 246, 97 );

	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].Create( 0, 0, 32, 32, &g_MainInterfaceTextureArchive, resourceimageindex );
	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].SetClipRect( 214, 32, 246, 64 );	
	
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create(0, 0, 90, 18, &g_MainInterfaceTextureArchive, resourceimageindex );
	pTitleBar->SetClipRect( 0, 125, 91, 143 );
	InsertChildObject(pTitleBar);
	_XImageStatic* pTitleBarAdd = new _XImageStatic;
	pTitleBarAdd->Create(91, 0, 167, 18, &g_MainInterfaceTextureArchive, resourceimageindex );
	pTitleBarAdd->SetClipRect( 106, 125, 183, 143 );
	InsertChildObject(pTitleBarAdd);

	int resourceimageindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XImageStatic* pBorder = new _XImageStatic;
	pBorder->Create(0, 18, 167, 142, &g_MainInterfaceTextureArchive, resourceimageindex1 );
	pBorder->SetClipRect( 6, 83, 174, 208 );
	InsertChildObject(pBorder);

	// bottom Line
	_XImageStatic* pBottomLine = new _XImageStatic;
	pBottomLine->Create(0, 143, 87, 145, &g_MainInterfaceTextureArchive, resourceimageindex1 );
	pBottomLine->SetClipRect( 0, 208, 88, 211 );
	InsertChildObject(pBottomLine);
	_XImageStatic* pBottomLine2 = new _XImageStatic;
	pBottomLine2->Create(88, 143, 167, 145, &g_MainInterfaceTextureArchive, resourceimageindex1 );
	pBottomLine2->SetClipRect( 103, 208, 183, 211 );
	InsertChildObject(pBottomLine2);

	// Left Line
	_XImageStatic* pLeftLine = new _XImageStatic;
	pLeftLine->Create(0, 18, 2, 143, &g_MainInterfaceTextureArchive, resourceimageindex );
	pLeftLine->SetClipRect( 248, 0, 251, 126 );
	InsertChildObject(pLeftLine);
	
	// Right Line
	_XImageStatic* pRightLine = new _XImageStatic;
	pRightLine->Create(165, 18, 167, 143, &g_MainInterfaceTextureArchive, resourceimageindex );
	pRightLine->SetClipRect( 248, 0, 251, 126 );
	InsertChildObject(pRightLine);

	// close, help
	resourceimageindex = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 136, 2 }, { 12, 12 }, _XDEF_POCKET_CLOSE,
									  resourceimageindex,resourceimageindex,resourceimageindex, 
									  &g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
		
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );

	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 150, 2 }, { 12, 12 }, _XDEF_POCKET_HELP,
		resourceimageindex,resourceimageindex,resourceimageindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );

	for( int i = 0; i < _XDEF_POCKET_MAX_SLOTNUMBER; i++ )
	{
		m_SlotPosition[i].x = _XDEF_POCKETITEMPOSX + ((i%4)*33);
		m_SlotPosition[i].y = _XDEF_POCKETITEMPOSY + ((i/4)*33);
	}	
	return TRUE;
}

void _XWindow_Pocket::DestroyWindow( void )
{

}

void _XWindow_Pocket::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw(pfocusobject);

	if( !this->m_ShowWindow ) return;

	_XDrawRectAngle( m_WindowPosition.x+16, m_WindowPosition.y+27, m_WindowPosition.x+150, m_WindowPosition.y+95, 0.0f,D3DCOLOR_ARGB(255,147,138,116) );

	_XDrawSolidBar( m_WindowPosition.x+8, m_WindowPosition.y+102, m_WindowPosition.x+158, m_WindowPosition.y+137,D3DCOLOR_ARGB(178,8,7,8) );
	_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+102, m_WindowPosition.x+158, m_WindowPosition.y+137, 0.0f,D3DCOLOR_ARGB(255,8,7,8) );

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,166,169,128));
	g_XBaseFont->PutsAlign(m_WindowPosition.x+13, m_WindowPosition.y+107, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_POCKET_BROKEN) );	// _T("의상파손으로 임시 저장된")
	g_XBaseFont->PutsAlign(m_WindowPosition.x+55, m_WindowPosition.y+122, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_POCKET_BROKEN1) );	// _T("물품입니다")
	g_XBaseFont->Flush();

	for( int i = 0; i < _XDEF_POCKET_MAX_SLOTNUMBER; i++ )
	{
		if( m_AccessoryItemList[i].Get_m_sID() > 0)
		{
			m_ItemEdgeIcon[_XNT_ITEMEDGEICONTYPE_NORMAL].Draw( m_WindowPosition.x+m_SlotPosition[i].x, 
				m_WindowPosition.y+m_SlotPosition[i].y );
			g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+m_SlotPosition[i].x+1, m_WindowPosition.y+m_SlotPosition[i].y+1, &m_AccessoryItemList[i] );				
		}
		else
		{
			m_ItemEdgeIcon[_XNT_ITEMEDGEICONTYPE_EMPTY].Draw( m_WindowPosition.x+m_SlotPosition[i].x, m_WindowPosition.y+m_SlotPosition[i].y );
		}
	}

	DrawSelectedItem();
}

BOOL _XWindow_Pocket::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

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
					if(scrnpos->x > m_WindowPosition.x+16 && scrnpos->z < m_WindowPosition.x+150)
					{
						if(scrnpos->z > m_WindowPosition.y+27 && scrnpos->z < m_WindowPosition.y+95)
						{
							pfocusobject = _XDEF_DUMMYID_POCKET;
						}
					}				
					
					if(!m_bHaveMouseSlot)
					{
						// Mouse down 위치 저장
						m_IconDragStartPos.x = scrnpos->x;
						m_IconDragStartPos.y = scrnpos->z;
						m_LastDraggingSlot = SearchSlotIndex();
						m_bDragState = TRUE;
					}
					else
					{
						m_MouseSlotItem.ResetMouseSlotItem();
						m_bHaveMouseSlot = FALSE;
					}
				} // if(CheckMousePosition())
			}
		} // if(!pfocusobject)
		
		if(pfocusobject == _XDEF_DUMMYID_POCKET)
		{
			if(mousestate->bButton[0])
			{
				// 드래그 처리
				if(m_bDragState && !m_bHaveMouseSlot)
				{
					if(scrnpos->x != m_IconDragStartPos.x || scrnpos->z != m_IconDragStartPos.y)
					{
						m_MouseSlotItem.Copy(m_AccessoryItemList[m_LastDraggingSlot]);
						m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;						
						m_bHaveMouseSlot = TRUE;
						m_LastDraggingSlot = -1;						
					}
				} // if(m_bDragState && !m_bHaveMouseSlot)
			}
			
			if(!mousestate->bButton[0])
			{
				if(m_bDragState)
				{
					if(scrnpos->x == m_IconDragStartPos.x && scrnpos->z == m_IconDragStartPos.y)
					{
						if( m_AccessoryItemList[m_LastDraggingSlot].Get_m_sID() > 0 )
						{
							// 제자리에서 클릭						
							m_MouseSlotItem.Copy(m_AccessoryItemList[m_LastDraggingSlot]);
							m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
							
							if( g_pDefaultTooltip_Window )
							{
								// 툴팁 보이기
								
							}
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
							int selectedslot = SearchSlotIndex();
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
		} // if(pfocusobject == _XDEF_DUMMYID_POCKET)
		
		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				BOOL resetmouse = TRUE;
				if(!m_bHaveMouseSlot)
				{
					m_LastDraggingSlot = SearchSlotIndex();
					if( m_AccessoryItemList[m_LastDraggingSlot].Get_m_sID() > 0 )					
					{
						// SendPacket
						g_NetworkKernel.SendPacket( MSG_NO_TEMP_INVEN_CTRL, m_LastDraggingSlot );
						m_LastDraggingSlot = -1;
					}
					m_WindowMouseState = _XW_STATE_NONE;
					resetmouse = FALSE;
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
			if(pfocusobject == _XDEF_DUMMYID_POCKET)
				pfocusobject = NULL;
		}
	}
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	return TRUE;
}

void _XWindow_Pocket::DrawSelectedItem(void)
{
	RECT iconrect;
	int	selectedslot = SearchSlotIndex();
	iconrect.left = m_WindowPosition.x + m_SlotPosition[selectedslot].x;
	iconrect.top =  m_WindowPosition.y + m_SlotPosition[selectedslot].y;
	iconrect.right = m_WindowPosition.x + m_SlotPosition[selectedslot].x + 31;
	iconrect.bottom =  m_WindowPosition.y + m_SlotPosition[selectedslot].y + 31;

	if(selectedslot != -1)
	{
		if(g_pInventory_Window->m_bHaveMouseSlot)
		{
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_WARNING );
		}
		
		// mouse hover
		if( m_AccessoryItemList[selectedslot].Get_m_sID() > 0 )
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
	}

	if( m_MouseSlotItem.m_InventoryNumber >= 0 )
	{
		iconrect.left   = m_WindowPosition.x+m_SlotPosition[m_MouseSlotItem.m_InventoryNumber].x;
		iconrect.top    = m_WindowPosition.y+m_SlotPosition[m_MouseSlotItem.m_InventoryNumber].y;
		iconrect.right  = m_WindowPosition.x+m_SlotPosition[m_MouseSlotItem.m_InventoryNumber].x + 31;
		iconrect.bottom = m_WindowPosition.y+m_SlotPosition[m_MouseSlotItem.m_InventoryNumber].y + 31;
		
		if(m_bHaveMouseSlot)
		{
			// disable icon image
			_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
		}
		
		_XDrawRectAngle( iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT );
	}
}

int _XWindow_Pocket::SearchSlotIndex(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	int PosX, PosY;
	PosX = scrnpos->x;
	PosY = scrnpos->z;

	PosX -= (m_WindowPosition.x + _XDEF_POCKETITEMPOSX);
	if( PosX < 0 )
		return -1;
	PosX /= 33;	
	PosY -= (m_WindowPosition.y + _XDEF_POCKETITEMPOSY);
	if( PosY < 0 )
		return -1;
	PosY /= 33;	

	if( PosX > 3 || PosY > 3 )
		return -1;

	return ((PosY * 4 + PosX) > _XDEF_POCKET_MAX_SLOTNUMBER) ? -1 : (PosY * 4 + PosX);
}

void _XWindow_Pocket::InsertAccessoryItem(int index, int sId, int second)
{
	m_AccessoryItemList[index].Set_m_cType(_XGI_FC_ACCESSORY);
	m_AccessoryItemList[index].Set_m_cSecond(second);
	m_AccessoryItemList[index].Set_m_sID(sId);
	m_AccessoryItemList[index].Set_m_ucCount(1);
	m_AccessoryItemList[index].Set_m_usCurDur( _XGameItem::GetMaxDurability(_XGI_FC_ACCESSORY, sId) );
	m_AccessoryItemList[index].Set_m_usMaxDur( m_AccessoryItemList[index].Get_m_usCurDur() );
}

void _XWindow_Pocket::CopyPocketItem(int invenindex, int pocketindex)
{
	g_pLocalUser->m_UserItemList[invenindex].Copy( m_AccessoryItemList[pocketindex] );
	m_AccessoryItemList[pocketindex].Reset();
}

void _XWindow_Pocket::DrawDragIcon(void)
{
//	if(g_MessageBox.m_ShowWindow) return;
	_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
	if(pTradeMessageBox)		return;	

	if(m_bHaveMouseSlot)
	{
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		g_pInventory_Window->DrawIcon(TRUE, scrnpos->x - 16 , scrnpos->z - 16, (_XUserItem*)&m_MouseSlotItem, FALSE);

		((CEmperorOfDragonsApp*)gApp)->ReadyCursor(_XMOUSETYPE_SELECT);
	}	
}