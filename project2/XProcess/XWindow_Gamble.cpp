// XWindow_Gamble.cpp: implementation of the _XWindow_Gamble class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XMessageWindow.h"
#include "XWindow_Gamble.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Gamble::_XWindow_Gamble()
{
	m_pBoxOpenButton	= NULL;	
	m_pApplyCancleButton = NULL;
	m_pClosebutton = NULL;
	m_MouseSlotItem.ResetMouseSlotItem();
	m_bHaveMouseSlot		= FALSE;	
	m_IconDragStartPos.x	= m_IconDragStartPos.y = 0;	
	m_bDragState			= FALSE;
	m_bDrawSelectedItem		= FALSE;	
	m_LastDraggingSlot		= -1;		
	m_MsgBoxOpen			= FALSE;

	m_GambleBoxItem			= NULL;	
	for( int i = 0; i < _XDEF_GAMBLEBOX_MAXCOUNT; i++ )
		m_GambleKeySlot[i] = NULL;

	ResetSocketItemList();
}

_XWindow_Gamble::~_XWindow_Gamble()
{

}

void _XWindow_Gamble::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

BOOL _XWindow_Gamble::Initialize( void )
{
	int imageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 184, 71, &g_MainInterfaceTextureArchive, imageindex );	
	pTitleBar->SetClipRect( 0, 125, 183, 195 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pBackBoard = new _XImageStatic;
	pBackBoard->Create( 0, 60, 184, 271, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ) );	
	pBackBoard->SetClipRect( 0, 1, 183, 211 );
	InsertChildObject( pBackBoard );

	_XImageStatic* pSocketItemView = new _XImageStatic;
	pSocketItemView->Create( 7, 26, 176, 141, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pSocketItemView->SetClipRect( 4, 140, 172, 254 );
	InsertChildObject( pSocketItemView );
	
	// 장식물 슬롯 ----------------------------------------------------------------------------------------------------=
	int socketslotimg = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	m_GambleKeySlot[0] = new _XImageStatic;
	m_GambleKeySlot[0]->Create( 121, 70, 153, 102, &g_MainInterfaceTextureArchive, socketslotimg );
	m_GambleKeySlot[0]->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( m_GambleKeySlot[0] );

	m_GambleKeySlot[1] = new _XImageStatic;
	m_GambleKeySlot[1]->Create( 105, 105, 137, 137, &g_MainInterfaceTextureArchive, socketslotimg );
	m_GambleKeySlot[1]->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( m_GambleKeySlot[1] );

	m_GambleKeySlot[2] = new _XImageStatic;
	m_GambleKeySlot[2]->Create( 140, 105, 172, 137, &g_MainInterfaceTextureArchive, socketslotimg );
	m_GambleKeySlot[2]->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( m_GambleKeySlot[2] );
	// ----------------------------------------------------------------------------------------------------------------=

	// 버튼들 ---------------------------------------------------------------------------------------------------------=
	int applybtnindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XBTN_STRUCTURE btnstruct = 
	{ 
		TRUE, { 117, 240  }, { 60, 24 }, 
		_XDEF_GAMBLE_CANCLE, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive
	};
	m_pApplyCancleButton = new _XButton;
	m_pApplyCancleButton->Create( btnstruct );	
	m_pApplyCancleButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pApplyCancleButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE);		// _T("취소")
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	InsertChildObject( m_pApplyCancleButton );

	applybtnindex = g_MainInterfaceTextureArchive.FindResource( "mi_skillwin.tga" );
	_XBTN_STRUCTURE boxbtnstruct = 
	{ 
		TRUE, { 9, 240  }, { 108, 24 }, 
		_XDEF_GAMBLE_OPEN, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive 
	};
	m_pBoxOpenButton = new _XButton;
	m_pBoxOpenButton->Create( boxbtnstruct );	
	m_pBoxOpenButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pBoxOpenButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKITEMTRADE_1756);
	m_pBoxOpenButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200 );
	m_pBoxOpenButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 152, 110, 176 ); 
	m_pBoxOpenButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 201, 110, 225 );	
	InsertChildObject( m_pBoxOpenButton );

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_GAMBLE_CLOSE,
									  charinfointerface,charinfointerface,charinfointerface, 
									  &g_MainInterfaceTextureArchive };
	m_pClosebutton = new _XButton;
	m_pClosebutton->Create( closebtnstruct );	
		
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( m_pClosebutton );

	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 153, 2 }, { 12, 12 }, _XDEF_GAMBLE_HELP,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	// ----------------------------------------------------------------------------------------------------------------=
	for( int i = 0; i < _XDEF_GAMBLEBOX_MAXCOUNT; i++ )
	{
		m_GambleKeyList[i].Reset();
	}

	m_SlotPosition[0].x = 121;
	m_SlotPosition[0].y = 70;
	m_SlotPosition[1].x = 105;
	m_SlotPosition[1].y = 105;
	m_SlotPosition[2].x = 140;
	m_SlotPosition[2].y = 105;	

	// Model View Camera Setting =========================================
	m_GambleItemModelViewCamera.Set( 45, 83.0f/60.0f, 0.01f, 180.0f);
	m_GambleItemModelViewCamera.Set(0.0f, 0.0f, 0.0f);	
	m_GambleItemModelViewCamera.SetDistanceLimits( 0.01, 30.0f );
	m_GambleItemModelViewCamera.SetDistance( 1.0f );	
	m_GambleItemModelViewCamera.UpdateViewMatrix();
	m_GambleItemModelViewCamera.UpdateProjMatrix();

	// ViewPortReposition - for Model View
	m_GambleItemModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_GambleItemModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
	m_GambleItemModelViewPort.Width  = 83;
	m_GambleItemModelViewPort.Height = 60;
	m_GambleItemModelViewPort.MinZ = 0.0f;
	m_GambleItemModelViewPort.MaxZ = 1.0f;

	m_GambleItemRotateAngle = 0.0f;

	return TRUE;
}

void _XWindow_Gamble::Draw( _XGUIObject*& pfocusobject )
{	
	_XWindow::Draw(pfocusobject);
	if(!this->m_ShowWindow)	return;

	int tempx = m_WindowPosition.x + 9;
	int tempy = m_WindowPosition.y + 68;

	if( m_GambleBoxItem )
	{
		for( int i = 0; i < 3; i++ )
		{
			tempx = m_WindowPosition.x+m_SlotPosition[i].x;
			tempy = m_WindowPosition.y+m_SlotPosition[i].y;
			_XDrawRectAngle( tempx, tempy, tempx+32, tempy+32, 0.0f,D3DCOLOR_ARGB(255,114,98,87) );
		}
		
		for( i = 0 ; i < _XDEF_GAMBLEBOX_MAXCOUNT ; i++)
		{
			if( m_GambleKeyList[i].Get_m_sID() )
				g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+m_SlotPosition[i].x+1, m_WindowPosition.y+m_SlotPosition[i].y+1, &m_GambleKeyList[i]);
		}
	}
	
	_XDrawSolidBar( m_WindowPosition.x+9, m_WindowPosition.y+145, m_WindowPosition.x+174, m_WindowPosition.y+236, 0x80000000 );
	_XDrawRectAngle( m_WindowPosition.x+9, m_WindowPosition.y+145, m_WindowPosition.x+174, m_WindowPosition.y+236, 0.0f, 0xAA000000 );	
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	g_XBaseFont->Puts_Separate( m_WindowPosition.x+15, m_WindowPosition.y+150 , _XGETINTERFACETEXT(ID_STRING_GAMBLE_2080), 160, 1.0, 3 );

	DrawSelectedItem();
	if( m_GambleBoxItem )
	{
		DrawSocketPlugInionItem();

		g_XBaseFont->SetColor(_XSC_DEFAULT);
		int tempLength = strlen( _XGameItem::GetItemName(m_GambleBoxItem->Get_m_cType(), m_GambleBoxItem->Get_m_sID() ));
		if( tempLength > 16 )
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+40, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_GambleBoxItem->Get_m_cType(), m_GambleBoxItem->Get_m_sID() ) );
		}
		else
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+40, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_GambleBoxItem->Get_m_cType(), m_GambleBoxItem->Get_m_sID() ) );
		}
	}
	g_XBaseFont->Flush();
}

BOOL _XWindow_Gamble::Process( _XGUIObject*& pfocusobject )
{	
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;	

	// 장착버튼 미리 처리
	if( !m_ObjectList.listEmpty() )
	{
		m_ObjectList.resetList();
		m_ObjectList.prevItem();
		_XGUIObject* currentObject = NULL;

		do
		{
			currentObject = (_XGUIObject*)m_ObjectList.currentItem();

			if(currentObject) 
			{
				currentObject->Process( pfocusobject );				
			}

			m_ObjectList.prevItem();
		}while( !m_ObjectList.atEndOfList() );
	}

	_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
	if( pMessageBox ) 
	{
		if(pfocusobject == _XDEF_DUMMYID_GAMBLEBOX ) pfocusobject = NULL;
			return FALSE;	
	}

	BOOL bUseSocketPlagIn = FALSE;
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();

	if(mousestate)
	{
		if(!pfocusobject || pfocusobject == this )
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
				if(CheckMousePosition())
				{
					if(scrnpos->x > m_WindowPosition.x+9 && scrnpos->z < m_WindowPosition.x+173)
					{
						if(scrnpos->z > m_WindowPosition.y+68 && scrnpos->z < m_WindowPosition.y+138)
						{
							pfocusobject = _XDEF_DUMMYID_GAMBLEBOX;
						}
					}

					if(g_pInventory_Window)	// 인벤토리에서 드래그 했을 경우
					{
						if(g_pInventory_Window->GetShowStatus() && !g_pLocalUser->m_bGambleBoxOpenLock)
						{
							int ivindex = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
							if(g_pInventory_Window->m_bHaveMouseSlot && ivindex >= _XINVENTORY_SLOTTYPE_POCKETSTART)
							{
								if( g_pLocalUser->m_UserItemList[ivindex].Get_m_cType() == _XGI_FC_BOXKEY )
								{
									if( m_GambleBoxItem )
									{
										int insertindex = -1;
										for( int i = 0; i < 3; i++ )
										{
											if( m_GambleKeyList[i].Get_m_sID() == 0 )
											{
												insertindex = i;									
												break;
											}
										}
										
										if( insertindex == -1 )
										{
											MessagePrint(5);	// 빈 공간이 없습니다.
											return FALSE;
										}
										else
										{
											if(CheckAllSlot(ivindex))
											{
												if( g_pLocalUser->m_UserItemList[ivindex].Get_m_ucCount() > 0 )
												{
													m_GambleKeyList[insertindex].Copy(g_pLocalUser->m_UserItemList[ivindex]);
													m_GambleKeyList[insertindex].Set_m_ucCount(1);
													m_GambleKeyFromInvenNumber[insertindex] = ivindex;
#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
													// inven 건드리지 않는다
#else
													g_pLocalUser->m_UserItemList[ivindex].Set_m_ucCount(g_pLocalUser->m_UserItemList[ivindex].Get_m_ucCount()-1);									
#endif
												}
																			
												PlayItemEquipSound( m_GambleKeyList[insertindex].Get_m_cType(), m_GambleKeyList[insertindex].Get_m_sID() );
												g_pInventory_Window->m_iSocketIndexList[insertindex+1] = ivindex;											
												m_pBoxOpenButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_INVENTORY_2121 );
												
												g_pInventory_Window->m_bHaveMouseSlot = FALSE;
												g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
											}
										}
									}
								}
								else
								{
									MessagePrint(3);									
									return FALSE;
								}								
								bUseSocketPlagIn = TRUE;
							}							
						}
					}
					
					if( !bUseSocketPlagIn )
					{
						if( !m_bHaveMouseSlot )
						{
							// mouse 위치 저장
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
						if( g_pInventory_Window->GetShowStatus() )
						{
							g_pInventory_Window->m_bDrawSelectedItem = FALSE;
						}
					}
				} // if( CheckMousePosition() )
				else
				{
				}
			}			
		}
		if( pfocusobject == _XDEF_DUMMYID_GAMBLEBOX )
		{
			if( mousestate->bButton[0] )
			{
				//drag
				if( m_bDragState && !m_bHaveMouseSlot )
				{
					if(scrnpos->x != m_IconDragStartPos.x || scrnpos->z != m_IconDragStartPos.y)
					{
						if(m_LastDraggingSlot != -1)
						{
							m_bHaveMouseSlot = TRUE;
							m_MouseSlotItem.Copy(m_GambleKeyList[m_LastDraggingSlot]);
							m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
							m_LastDraggingSlot = -1;
						}
					}
				}
			}

			if( !mousestate->bButton[0] )
			{
				m_bDrawSelectedItem = TRUE;
				if( m_bDragState )
				{
					if(scrnpos->x != m_IconDragStartPos.x && scrnpos->z != m_IconDragStartPos.y)
					{
						if(m_bDragState && m_bHaveMouseSlot)
						{
							int selectedslot = GetSlotIndex();
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
				}
			} //if( !mousestate->bButton[0] )
		}

		if( !mousestate->bButton[1] )
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				BOOL resetmouse = TRUE;
				if( !m_bHaveMouseSlot && !g_pLocalUser->m_bGambleBoxOpenLock )
				{
					m_LastDraggingSlot = GetSlotIndex();
					if( m_LastDraggingSlot != -1)
					{
						m_GambleKeyList[m_LastDraggingSlot].Reset();
						m_GambleKeyFromInvenNumber[m_LastDraggingSlot] = 0;
#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
						// inven 건드리지 않음
#else
						g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[m_LastDraggingSlot+1]].Set_m_ucCount(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[m_LastDraggingSlot+1]].Get_m_ucCount() + 1);
#endif
						g_pInventory_Window->m_iSocketIndexList[m_LastDraggingSlot+1] = 0;

						// 소켓 중간이 빠졌을 경우
						for( int i = m_LastDraggingSlot+1; i < _XDEF_GAMBLEBOX_MAXCOUNT; i++ )
						{
							if( m_GambleKeyList[i].Get_m_sID() > 0 )
							{
								m_GambleKeyList[i-1].Copy(m_GambleKeyList[i]);
								m_GambleKeyFromInvenNumber[i-1] = g_pInventory_Window->m_iSocketIndexList[i+1];
								m_GambleKeyList[i].Reset();
								m_GambleKeyFromInvenNumber[i] = 0;
								g_pInventory_Window->m_iSocketIndexList[i] = g_pInventory_Window->m_iSocketIndexList[i+1];
								g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
							}
						}

						// 장식물을 다 내렸는지 검사
						BOOL tempflag = TRUE;
						for( i = 1; i < 3; i++ )
						{
							if( g_pInventory_Window->m_iSocketIndexList[i] > 0 )
								tempflag = FALSE;
						}
						if( tempflag )
						{
							m_pBoxOpenButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKITEMTRADE_1756); 
						}
						
						m_WindowMouseState = _XW_STATE_NONE;
						m_LastDraggingSlot = -1;
					}					
				}
				if( g_pInventory_Window )
				{
					if( g_pInventory_Window->GetShowStatus() && g_pInventory_Window->CheckMousePosition() )
						resetmouse = FALSE;
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
			if(pfocusobject == _XDEF_DUMMYID_GAMBLEBOX)
				pfocusobject = NULL;
		}
	}
	
	if(!_XWindow::Process(pfocusobject)) return FALSE;

	return TRUE;
}

int _XWindow_Gamble::GetSlotIndex(void)
{
	if( m_MsgBoxOpen )
		return -1;
	
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	
	for(int i = 0 ; i < _XDEF_GAMBLEBOX_MAXCOUNT ; i++)
	{
		if(scrnpos->x > m_WindowPosition.x + m_SlotPosition[i].x && scrnpos->x < m_WindowPosition.x + m_SlotPosition[i].x + 30)
		{
			if(scrnpos->z > m_WindowPosition.y + m_SlotPosition[i].y && scrnpos->z < m_WindowPosition.y + m_SlotPosition[i].y + 30)
			{
				if( m_GambleKeyList[i].Get_m_sID() )
					return i;
			}
		}
	}
	return -1;
}

void _XWindow_Gamble::DrawDragIcon(void)
{
	_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
	if(pMessageBox)
	{
		if(pMessageBox->GetShowStatus())
			return;
	}	
	
	if(m_bHaveMouseSlot)
	{
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		g_pInventory_Window->DrawIcon(TRUE, scrnpos->x - 16 , scrnpos->z - 16, (_XUserItem*)&m_MouseSlotItem, FALSE);
		
		((CEmperorOfDragonsApp*)gApp)->ReadyCursor(_XMOUSETYPE_SELECT);
	}	
}

void _XWindow_Gamble::DrawSelectedItem(void)
{
	if(m_MouseSlotItem.Get_m_sID() > 0)
	{
		int selectedslot = m_MouseSlotItem.m_InventoryNumber;
		
		if( selectedslot > -1 )
		{
			_XDrawRectAngle(m_WindowPosition.x+m_SlotPosition[selectedslot].x,
				m_WindowPosition.y+m_SlotPosition[selectedslot].y,
				m_WindowPosition.x+m_SlotPosition[selectedslot].x+31,
				m_WindowPosition.y+m_SlotPosition[selectedslot].y+31, 1.0f, _XSC_ITEM_SELECT);		
			
			if(m_bHaveMouseSlot)
			{
				// disable icon image
				_XDrawSolidBar( m_WindowPosition.x+m_SlotPosition[selectedslot].x+1,
					m_WindowPosition.y+m_SlotPosition[selectedslot].y+1,
					m_WindowPosition.x+m_SlotPosition[selectedslot].x+30, 
					m_WindowPosition.y+m_SlotPosition[selectedslot].y+30, D3DCOLOR_ARGB(160, 0, 0, 0));
			}					
		}		
	}
}

void _XWindow_Gamble::MessagePrint(int msgindex)
{
	_XMessageWindow* pFindMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
	if(pFindMessageBox)
	{
		if(pFindMessageBox->GetShowStatus())
			return;
	}
	
	_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
	if( pMessageBox )
	{
		TCHAR messagestring[256];
		memset( messagestring, 0, sizeof(TCHAR)*256 );
		
		switch(msgindex) 
		{
			case 0:			
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_GAMBLE_2081), TRUE, 0xFFFF0000 ); 			
				break;
			case 1:
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_GAMBLE_2082), TRUE, 0xFFFF0000 );			
				break;
			case 2:
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_GAMBLE_2083), TRUE, 0xFFFF0000 );
				break;
			case 3:
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_GAMBLE_2084), TRUE, 0xFFFF0000 ); 
				break;
			case 4:
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_GAMBLE_2085), TRUE, 0xFFFF0000 );		
				break;				
			case 5:
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_GAMBLE_2086), TRUE, 0xFFFF0000 );
				break;
		}		
		
		if( msgindex == 1 || msgindex == 2 )
		{	
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_GAMBLE_OPENOK, _XDEF_GAMBLE_OPENNO);
		}
		else if( msgindex == 4 )
		{
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_GAMBLE_STOPOK, _XDEF_GAMBLE_STOPNO);
		}
		else
		{
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, messagestring, TRUE);
		}
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
	}	
}

void _XWindow_Gamble::ResetSocketItemList(void)
{
	m_GambleBoxItem		= NULL;

	for( int i = 0; i < _XDEF_GAMBLEBOX_MAXCOUNT; i++ )
	{
		m_GambleKeyList[i].Reset();	
		m_GambleKeyFromInvenNumber[i] = 0;
	}
	m_bHaveMouseSlot = FALSE;
	m_MouseSlotItem.ResetMouseSlotItem();
}

void _XWindow_Gamble::SetDefaultPosition( void )
{		
	MoveWindow( gnWidth-376 , 220 );
}

void _XWindow_Gamble::DrawSocketPlugInionItem( void )
{
	D3DXMATRIX			rotmat;
	D3DXMatrixIdentity(&rotmat);
	
	m_GambleItemRotateAngle++;
	D3DXMatrixRotationY(&rotmat, _X_RAD(m_GambleItemRotateAngle));

#ifdef _XTS_ITEM_OPTIMIZATION
	if( _XGI_FC_WEAPON == GetEqualItemType(m_GambleBoxItem->Get_m_cType()) )
#else
	if( m_GambleBoxItem->Get_m_cType() == _XGI_FC_WEAPON || m_GambleBoxItem->Get_m_cType() == _XGI_FC_WEAPON2 || m_GambleBoxItem->Get_m_cType() == _XGI_FC_WEAPON3 )
#endif
	{
		D3DXMATRIX			zrotmat;
		D3DXMatrixRotationZ(&zrotmat, _X_RAD(45.0f));
		
		D3DXMatrixMultiply( &rotmat, &rotmat, &zrotmat );
	}	
	
	// Initialize Matrix
	int modelindex = GetSocketModelIndex();
	D3DXMatrixIdentity(&m_GambleItemRHWMatrix);
	D3DXMatrixTranslation( &m_GambleItemRHWMatrix, 
		-g_ItemModel_Weapon[0][modelindex]->OBB_Center.x,
		-g_ItemModel_Weapon[0][modelindex]->OBB_Center.y,
		-g_ItemModel_Weapon[0][modelindex]->OBB_Center.z	);
	
	// Camera/ViewPort Setting
	m_GambleItemModelViewCamera.UpdateViewMatrix( NULL, FALSE );
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&m_GambleItemModelViewCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&m_GambleItemModelViewCamera.GetProjMatrix());
	gpDev->SetViewport(&m_GambleItemModelViewPort);
	
	// Z Buffer를 clear 한다. - 현재 viewport영역만 clear된다.
	gpDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	
	// Rendering			
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );	
	
	
	
	FLOAT scalefactor;
	switch(m_GambleBoxItem->Get_m_cType()) 
	{
	case _XGI_FC_BOX :
		{
			scalefactor = 0.5f / g_ItemModel_JewelBox[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_GambleItemRHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_GambleItemRHWMatrix, &m_GambleItemRHWMatrix, &rotmat);
			m_GambleItemRHWMatrix._41 = 0.0;
			m_GambleItemRHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[modelindex]->OBB_Center.y);;
			m_GambleItemRHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[modelindex]->RenderInterfaceItem(&m_GambleItemRHWMatrix);
		}
		break;
	}	
	
	// View Matrix, Projection Matrix, Viewport 다시 되돌린다. ====
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetProjMatrix());
	gpDev->SetViewport(&g_MainViewPort);

}

int _XWindow_Gamble::GetSocketModelIndex()
{
	int index;
	switch(m_GambleBoxItem->Get_m_cType()) 
	{
	case _XGI_FC_BOX :
		{
			index = g_BoxItemProperty[m_GambleBoxItem->Get_m_sID()].sModelIndex;
		}		
		break;
	default:
		index = 0;
		break;
	}
	return index;
}

void _XWindow_Gamble::MoveWindow( int X, int Y )
{	
	_XWindow::MoveWindow( X, Y );
	m_GambleItemModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_GambleItemModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
}

void _XWindow_Gamble::ShowWindow(BOOL show)
{
	if(show)
	{
		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;
		}
		
		m_pBoxOpenButton->EnableWindow(TRUE);
		m_pApplyCancleButton->EnableWindow(TRUE);
		m_pClosebutton->EnableWindow(TRUE);

		if( g_pDefaultTooltip_Window )
		{
			if( g_pDefaultTooltip_Window->GetShowStatus() )						
				g_pDefaultTooltip_Window->ShowWindow( FALSE );
		}
	}
	else
	{
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
		
		m_pBoxOpenButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKITEMTRADE_1756);
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_Gamble::ProcessDblClick(void)
{
	if(g_pInventory_Window)
	{
		if(g_pInventory_Window->GetShowStatus() && !g_pLocalUser->m_bGambleBoxOpenLock )
		{
			if(CheckMousePosition())
			{
				m_LastDraggingSlot = GetSlotIndex();

				if( m_LastDraggingSlot != -1)
				{
					{
						m_GambleKeyList[m_LastDraggingSlot].Reset();
						m_GambleKeyFromInvenNumber[m_LastDraggingSlot] = 0;
#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
						// inven 건드리지 않음
#else
						g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[m_LastDraggingSlot+1]].Set_m_ucCount( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[m_LastDraggingSlot+1]].Get_m_ucCount() + 1);
#endif
						g_pInventory_Window->m_iSocketIndexList[m_LastDraggingSlot+1] = 0;
						
						// 소켓 중간이 빠졌을 경우
						for( int i = m_LastDraggingSlot+1; i < _XDEF_GAMBLEBOX_MAXCOUNT; i++ )
						{
							if( m_GambleKeyList[i].Get_m_sID() > 0 )
							{
								m_GambleKeyList[i-1].Copy(m_GambleKeyList[i]);
								m_GambleKeyFromInvenNumber[i-1] = g_pInventory_Window->m_iSocketIndexList[i+1];
								m_GambleKeyList[i].Reset();
								m_GambleKeyFromInvenNumber[i] = 0;
								g_pInventory_Window->m_iSocketIndexList[i] = g_pInventory_Window->m_iSocketIndexList[i+1];
								g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
							}
						}
						
						// 장식물을 다 내렸는지 검사
						BOOL tempflag = TRUE;
						for( i = 1; i < _XDEF_GAMBLEBOX_MAXCOUNT+1; i++ )
						{
							if( g_pInventory_Window->m_iSocketIndexList[i] > 0 )
								tempflag = FALSE;									
						}
						if( tempflag )
						{
							m_pBoxOpenButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKITEMTRADE_1756); 
							BOOL tempflag2 = TRUE;
							for( int i = 0; i < 3; i++ )
							{
								if( m_GambleKeyList[i].Get_m_sID() > 0 )
									tempflag2 = FALSE;
							}							
						}
						
						m_WindowMouseState = _XW_STATE_NONE;
						m_LastDraggingSlot = -1;

					}
				}
			}
		}
	}		
}

void _XWindow_Gamble::ResetData(void)
{
	m_MsgBoxOpen = FALSE;

	ResetSocketItemList();	
	if( g_pInventory_Window )
		g_pInventory_Window->ResetSocketIndexList();
}

BOOL _XWindow_Gamble::CheckAllSlot(int invennumber)
{
	int usecnt = 0;

	for(int i = 0 ; i < _XDEF_GAMBLEBOX_MAXCOUNT ; ++i)
	{
		if(m_GambleKeyList[i].Get_m_sID() > 0)
		{
			if(m_GambleKeyFromInvenNumber[i] == invennumber)
			{
				usecnt++;
			}
		}
	}

	if(g_pLocalUser->m_UserItemList[invennumber].Get_m_ucCount() > usecnt)
		return TRUE;
	else
		return FALSE;
}