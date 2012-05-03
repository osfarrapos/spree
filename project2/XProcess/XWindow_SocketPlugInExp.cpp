// XWindow_SocketPlugInExp.cpp: implementation of the _XWindow_SocketPlugInExp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XMessageWindow.h"
#include "XWindow_SocketPlugInExp.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_SocketPlugInExp::_XWindow_SocketPlugInExp()
{
	m_SocketPlugInButton	= NULL;	
	m_pApplyCancleButton	= NULL;
	m_pClosebutton			= NULL;
	
	m_MouseSlotItem.ResetMouseSlotItem();
	m_bHaveMouseSlot		= FALSE;	
	m_IconDragStartPos.x	= m_IconDragStartPos.y = 0;	
	m_bDragState			= FALSE;
	m_bDrawSelectedItem		= FALSE;	
	m_LastDraggingSlot		= -1;	
	m_bChangeButtonMode		= FALSE;
	m_MsgSocketPuluInOpen	= FALSE;

	m_IndependentItem		= NULL;
	for( int i = 0; i < 4; i++ )
	{
		m_SocketSlot[i] = NULL;
		m_SocketSlotEx[i] = NULL;
	}

	m_SocketPlugInMode = SM_NONE;
	m_SocketPlugInStep = SS_READY;

	m_CurrentItemMaxSocketCount = 0;
}

_XWindow_SocketPlugInExp::~_XWindow_SocketPlugInExp()
{

}

BOOL _XWindow_SocketPlugInExp::Initialize( void )
{
	int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 184, 71, &g_MainInterfaceTextureArchive, inventoryimageindex );	
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
	m_SocketSlot[0] = new _XImageStatic;
	m_SocketSlot[0]->Create( 105, 70, 137, 102, &g_MainInterfaceTextureArchive, socketslotimg );
	m_SocketSlot[0]->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( m_SocketSlot[0] );
	m_SocketSlot[1] = new _XImageStatic;
	m_SocketSlot[1]->Create( 140, 70, 172, 102, &g_MainInterfaceTextureArchive, socketslotimg );
	m_SocketSlot[1]->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( m_SocketSlot[1] );

	m_SocketSlot[2] = new _XImageStatic;
	m_SocketSlot[2]->Create( 105, 105, 137, 137, &g_MainInterfaceTextureArchive, socketslotimg );
	m_SocketSlot[2]->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( m_SocketSlot[2] );
	m_SocketSlot[3] = new _XImageStatic;
	m_SocketSlot[3]->Create( 140, 105, 172, 137, &g_MainInterfaceTextureArchive, socketslotimg );
	m_SocketSlot[3]->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( m_SocketSlot[3] );

	// 분리/확장 슬롯 ----------------------------------------------------------------------------------------------------=
	socketslotimg = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	m_SocketSlotEx[0] = new _XImageStatic;
	m_SocketSlotEx[0]->Create( 9, 184, 43, 219, &g_MainInterfaceTextureArchive, socketslotimg );
	m_SocketSlotEx[0]->SetClipRect( 213, 64, 247, 98 );	
	InsertChildObject( m_SocketSlotEx[0] );
	m_SocketSlotEx[1] = new _XImageStatic;
	m_SocketSlotEx[1]->Create( 52, 184, 86, 219, &g_MainInterfaceTextureArchive, socketslotimg );
	m_SocketSlotEx[1]->SetClipRect( 213, 64, 247, 98 );	
	InsertChildObject( m_SocketSlotEx[1] );

	m_SocketSlotEx[2] = new _XImageStatic;
	m_SocketSlotEx[2]->Create( 96, 184, 130, 219, &g_MainInterfaceTextureArchive, socketslotimg );
	m_SocketSlotEx[2]->SetClipRect( 213, 64, 247, 98 );	
	InsertChildObject( m_SocketSlotEx[2] );
	m_SocketSlotEx[3] = new _XImageStatic;
	m_SocketSlotEx[3]->Create( 140, 184, 174, 219, &g_MainInterfaceTextureArchive, socketslotimg );
	m_SocketSlotEx[3]->SetClipRect( 213, 64, 247, 98 );	
	InsertChildObject( m_SocketSlotEx[3] );
	
	m_DisableSlotImage.Create( 0, 0, 30, 30, &g_MainInterfaceTextureArchive, socketslotimg );	
	m_DisableSlotImage.SetClipRect( 216, 0, 246, 30 );
	// ----------------------------------------------------------------------------------------------------------------=
	
	// 버튼들 ---------------------------------------------------------------------------------------------------------=
	int applybtnindex = g_MainInterfaceTextureArchive.FindResource( "mi_skillwin.tga" );
	_XBTN_STRUCTURE socketpluginbtnstruct = 
	{ 
		TRUE, { 9, 242  }, { 104, 20 }, 
		_XDEF_SOCKETPLUGINEXP_START, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive 
	};
	m_SocketPlugInButton = new _XButton;
	m_SocketPlugInButton->Create( socketpluginbtnstruct );	
	m_SocketPlugInButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_SocketPlugInButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 152, 110, 176 );
	m_SocketPlugInButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200 ); 
	m_SocketPlugInButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 201, 110, 225 );
	m_SocketPlugInButton->ShowWindow(FALSE);
	InsertChildObject( m_SocketPlugInButton );	

	applybtnindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XBTN_STRUCTURE applycanclebtnstruct = 
	{ 
		TRUE, { 117, 242  }, { 60, 24 }, 
		_XDEF_SOCKETPLUGINEXP_CANCLE, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive
	};
	m_pApplyCancleButton = new _XButton;
	m_pApplyCancleButton->Create( applycanclebtnstruct );	
	m_pApplyCancleButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pApplyCancleButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE);		// _T("취소")
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 187, 152, 247, 176 );
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  187, 127, 247, 151 );
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	InsertChildObject( m_pApplyCancleButton );

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_SOCKETPLUGINEXP_CANCLE,
									  charinfointerface,charinfointerface,charinfointerface, 
									  &g_MainInterfaceTextureArchive };
	m_pClosebutton = new _XButton;
	m_pClosebutton->Create( closebtnstruct );	
		
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( m_pClosebutton );

	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 153, 2 }, { 12, 12 }, _XDEF_SOCKETPLUGINEXP_HELP,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	// ----------------------------------------------------------------------------------------------------------------=

	// 가격
	int resourceimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	_XImageStatic* pLineupLeftBorder = new _XImageStatic;
	pLineupLeftBorder->Create( 20, 219, 91, 240, &g_MainInterfaceTextureArchive, resourceimageindex );
	pLineupLeftBorder->SetClipRect( 33, 229, 105, 251 );
	InsertChildObject( pLineupLeftBorder );

	_XImageStatic* pLineupRightBorder = new _XImageStatic;
	pLineupRightBorder->Create( 161, 219, 232, 240, &g_MainInterfaceTextureArchive, resourceimageindex );
	pLineupRightBorder->SetClipRect( 33, 229, 105, 251 ); 
	pLineupRightBorder->SetScale( -1.0f, 1.0f );
	InsertChildObject( pLineupRightBorder );

	_XImageStatic* pMoneyImage = new _XImageStatic;	
	pMoneyImage->Create( 86, 225, 101, 237, &g_MainInterfaceTextureArchive, inventoryimageindex );
	pMoneyImage->SetClipRect(185, 6, 200, 18);
	InsertChildObject( pMoneyImage );
	
	for( int i = 0; i < 4; i++ )
	{
		m_SocketItemList[i].Reset();
		m_SocketExpItemList[i].Reset();
	}

	m_SlotPosition[0].x = 105;
	m_SlotPosition[0].y = 70;
	m_SlotPosition[1].x = 140;
	m_SlotPosition[1].y = 70;
	m_SlotPosition[2].x = 105;
	m_SlotPosition[2].y = 105;
	m_SlotPosition[3].x = 140;
	m_SlotPosition[3].y = 105;	

	m_SlotPositionEx[0].x = 9;
	m_SlotPositionEx[0].y = 184;
	m_SlotPositionEx[1].x = 52;
	m_SlotPositionEx[1].y = 184;
	m_SlotPositionEx[2].x = 96;
	m_SlotPositionEx[2].y = 184;
	m_SlotPositionEx[3].x = 140;
	m_SlotPositionEx[3].y = 184;	


	// Model View Camera Setting =========================================
	m_SocketModelViewCamera.Set( 45, 83.0f/60.0f, 0.01f, 180.0f);
	m_SocketModelViewCamera.Set(0.0f, 0.0f, 0.0f);	
	m_SocketModelViewCamera.SetDistanceLimits( 0.01, 30.0f );
	m_SocketModelViewCamera.SetDistance( 1.0f );	
	m_SocketModelViewCamera.UpdateViewMatrix();
	m_SocketModelViewCamera.UpdateProjMatrix();

	// ViewPortReposition - for Model View
	m_SocketModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_SocketModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
	m_SocketModelViewPort.Width  = 83;
	m_SocketModelViewPort.Height = 60;
	m_SocketModelViewPort.MinZ = 0.0f;
	m_SocketModelViewPort.MaxZ = 1.0f;

	m_SocketRotateAngle = 0.0f;
	
	return TRUE;
}

void _XWindow_SocketPlugInExp::DestroyWindow( void )
{

}

void _XWindow_SocketPlugInExp::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw(pfocusobject);
	if(!this->m_ShowWindow)	return;

	int tempx = m_WindowPosition.x + 9;
	int tempy = m_WindowPosition.y + 68;

	if( m_IndependentItem )
	{
		// 해당 아이템의 소켓 그리기
		for( int i = 0; i < m_IndependentItem->Get_m_ucSocketCount(); i++ )
		{
			tempx = m_WindowPosition.x+m_SlotPosition[i].x;
			tempy = m_WindowPosition.y+m_SlotPosition[i].y;
			_XDrawRectAngle( tempx, tempy, tempx+32, tempy+32, 0.0f,D3DCOLOR_ARGB(255,114,98,87) );

			if( m_SocketItemList[i].Get_m_sID() )
				g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+m_SlotPosition[i].x+1, m_WindowPosition.y+m_SlotPosition[i].y+1, &m_SocketItemList[i]);
		}
		
		// 확장 / 분리 삭제 소켓 그리기
		for( i = 0 ; i < m_CurrentItemMaxSocketCount ; i++ )
		{
			if( i < m_IndependentItem->Get_m_ucSocketCount() )
			{
				m_DisableSlotImage.Draw( m_WindowPosition.x+m_SlotPositionEx[i].x+2, m_WindowPosition.y+m_SlotPositionEx[i].y+2 );
			}
			else
			{
				if( m_SocketExpItemList[i].Get_m_sID() )
					g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+m_SlotPositionEx[i].x+2, m_WindowPosition.y+m_SlotPositionEx[i].y+2, &m_SocketExpItemList[i]);
			}
		}

		DrawSocketPlugInionItem();
	}
	
	_XDrawSolidBar( m_WindowPosition.x+9, m_WindowPosition.y+142, m_WindowPosition.x+173, m_WindowPosition.y+182, 0xFF1A1819 );
	_XDrawRectAngle( m_WindowPosition.x+9, m_WindowPosition.y+142, m_WindowPosition.x+173, m_WindowPosition.y+182, 0.0f, 0xFF000000 );
	
}

BOOL _XWindow_SocketPlugInExp::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;	

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
						if(scrnpos->z > m_WindowPosition.y+186 && scrnpos->z < m_WindowPosition.y+218)
						{
							pfocusobject = _XDEF_DUMMYID_SOCKETPLUGINEX;
						}
					}

					if(g_pInventory_Window)	// 인벤토리에서 드래그 했을 경우
					{
						if(g_pInventory_Window->GetShowStatus() && !g_pLocalUser->m_bSocketPlugInEXLock )
						{
							if( g_pInventory_Window->m_bHaveMouseSlot )
							{
								SetSocketPlugInExpandStart(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber);
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
		if( pfocusobject == _XDEF_DUMMYID_SOCKETPLUGINEX )
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
							m_MouseSlotItem.Copy(m_SocketItemList[m_LastDraggingSlot]);
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
				if( !m_bHaveMouseSlot && !g_pLocalUser->m_bSocketPlugInLock )
				{
					m_LastDraggingSlot = GetSlotIndex();
					if( m_LastDraggingSlot != -1)
					{
						m_SocketItemList[m_LastDraggingSlot].Reset();
						g_pInventory_Window->m_iSocketIndexList[m_LastDraggingSlot+1] = 0;
						
						// 소켓 중간이 빠졌을 경우
						for( int i = m_LastDraggingSlot+1; i < m_IndependentItem->Get_m_ucSocketCount(); i++ )
						{
							if( m_SocketItemList[i].Get_m_sID() > 0 )
							{
								m_SocketItemList[i-1].Copy(m_SocketItemList[i]);
								m_SocketItemList[i].Reset();
								g_pInventory_Window->m_iSocketIndexList[i] = g_pInventory_Window->m_iSocketIndexList[i+1];
								g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
							}
						}
						
						// 장식물을 다 내렸는지 검사
						BOOL tempflag = TRUE;
						for( i = 1; i < m_IndependentItem->Get_m_ucSocketCount()+1; i++ )
						{
							if( g_pInventory_Window->m_iSocketIndexList[i] > 0 )
								tempflag = FALSE;									
						}
						if( tempflag )
						{
							m_bChangeButtonMode = FALSE;
							m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM); // 물품 내리기
							BOOL tempflag2 = TRUE;
							for( int i = 0; i < m_IndependentItem->Get_m_ucSocketCount()+1; i++ )
							{
								if( m_SocketItemList[i].Get_m_sID() > 0 )
									tempflag2 = FALSE;
							}							
						}
						else
						{
							
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
			if(pfocusobject == _XDEF_DUMMYID_SOCKETPLUGINEX)
				pfocusobject = NULL;
		}
	}
	if(!_XWindow::Process(pfocusobject)) return FALSE;

	return TRUE;
}

void _XWindow_SocketPlugInExp::ShowWindow(BOOL show)
{
	if(show)
	{
		for( int i = 0; i < 4; i ++ )
			m_SocketSlotEx[i]->ShowWindow(FALSE);

		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;
			g_pInventory_Window->SetSocketPlugInExpWindowPtr(this);
		}
		
		m_pApplyCancleButton->EnableWindow(TRUE);
		m_pClosebutton->EnableWindow(TRUE);		
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

void _XWindow_SocketPlugInExp::MoveWindow( int X, int Y )
{	
	_XWindow::MoveWindow( X, Y );
	m_SocketModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_SocketModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
}

void _XWindow_SocketPlugInExp::DrawSocketPlugInionItem( void )
{
	D3DXMATRIX			rotmat;
	D3DXMatrixIdentity(&rotmat);
	
	m_SocketRotateAngle++;
	D3DXMatrixRotationY(&rotmat, _X_RAD(m_SocketRotateAngle));

	char cType = GetEqualItemType(m_IndependentItem->Get_m_cType());
	if( cType == _XGI_FC_WEAPON )
	{
		D3DXMATRIX			zrotmat;
		D3DXMatrixRotationZ(&zrotmat, _X_RAD(45.0f));
		
		D3DXMatrixMultiply( &rotmat, &rotmat, &zrotmat );
	}	
	
	// Initialize Matrix
	int modelindex = GetSocketModelIndex();
	D3DXMatrixIdentity(&m_SocketRHWMatrix);	
	
	// Camera/ViewPort Setting
	m_SocketModelViewCamera.UpdateViewMatrix( NULL, FALSE );
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&m_SocketModelViewCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&m_SocketModelViewCamera.GetProjMatrix());
	gpDev->SetViewport(&m_SocketModelViewPort);
	
	// Z Buffer를 clear 한다. - 현재 viewport영역만 clear된다.
	gpDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	
	// Rendering			
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );	
	
	
	
	FLOAT scalefactor;

	switch(cType) 
	{
	case _XGI_FC_WEAPON:
		{
			D3DXMatrixTranslation( &m_SocketRHWMatrix, 
				-g_ItemModel_Weapon[0][modelindex]->OBB_Center.x,
				-g_ItemModel_Weapon[0][modelindex]->OBB_Center.y,
				-g_ItemModel_Weapon[0][modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_Weapon[0][modelindex]->m_ObjectRadius;
			D3DXMATRIX matRot;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixScaling(&matRot, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply( &matRot, &matRot, &rotmat );							
			D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &matRot);
			
			g_ItemModel_Weapon[0][modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			switch( m_IndependentItem->Get_m_cSecond() ) 
			{
			case _XGI_SC_CLOTHES_UPPER:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_UBodyType[modelindex]->OBB_Center.x,
						-g_UserModel_UBodyType[modelindex]->OBB_Center.y,
						-g_UserModel_UBodyType[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_UBodyType[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_UBodyType[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_UBodyType[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_LOWER:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_LBodyType[modelindex]->OBB_Center.x,
						-g_UserModel_LBodyType[modelindex]->OBB_Center.y,
						-g_UserModel_LBodyType[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_LBodyType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_LBodyType[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_LBodyType[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_GLOVES:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_LHandType[modelindex]->OBB_Center.x,
						-g_UserModel_LHandType[modelindex]->OBB_Center.y,
						-g_UserModel_LHandType[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_LHandType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = 0.0f;
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_LHandType[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_SHOES:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_LFootType[modelindex]->OBB_Center.x,
						-g_UserModel_LFootType[modelindex]->OBB_Center.y,
						-g_UserModel_LFootType[modelindex]->OBB_Center.z	);
					scalefactor = 0.28f / g_UserModel_LFootType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = scalefactor*g_UserModel_LFootType[modelindex]->OBB_Center.x;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_LFootType[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_LFootType[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);					
				}
				break;
			case _XGI_SC_CLOTHES_CAP:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_Cap[modelindex]->OBB_Center.x,
						-g_UserModel_Cap[modelindex]->OBB_Center.y,
						-g_UserModel_Cap[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_Cap[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_Cap[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_Cap[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;	
			case _XGI_SC_CLOTHES_MASK : //Author : 양희왕 //breif : 08/01/24 변경 g_UserModel_UBodyType->g_UserModel_Mask
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_Mask[modelindex]->OBB_Center.x,
						-g_UserModel_Mask[modelindex]->OBB_Center.y,
						-g_UserModel_Mask[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_Mask[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_Mask[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_Mask[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;	
			}			
		}		
		break;
	}	
	
	// View Matrix, Projection Matrix, Viewport 다시 되돌린다. ====
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetProjMatrix());
	gpDev->SetViewport(&g_MainViewPort);
}

int _XWindow_SocketPlugInExp::GetSocketModelIndex()
{
	int index = 0;

	char itemtype = GetEqualItemType(m_IndependentItem->Get_m_cType());
	if( itemtype != -1 )
	{
		switch(itemtype)
		{
		case _XGI_FC_WEAPON :
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());				
				if( WeaponItem )
				{
					index = WeaponItem[m_IndependentItem->Get_m_sID()].sModelIndex;	
				}
			}
			break;
		case _XGI_FC_CLOTHES :
			{
				if( m_IndependentItem->Get_m_cSecond() == _XGI_SC_CLOTHES_MASK )
				{
					_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());
					if( ClothesItem )
					{
						index = ClothesItem[m_IndependentItem->Get_m_sID()].sModelIndex;
						if( ClothesItem[m_IndependentItem->Get_m_sID()].cSexType ) // 여자면 인덱스 추가.
						{
							// 기존에 배열된 모델 컨테이너 영역을 모두 사용하여 새로 추가된 모델의 인덱스 계산.
							if( index >= 140 )
							{
								// 새로 추가된 여자 모델 컨테이너 는 220번 부터임. 남자는 140번부터 이므로 두번째 모델 컨테이너 구분은 80칸씩
								index+= g_MaskModel_GenderSeparateIndex2;
							}
							else
							{
								index+= g_MaskModel_GenderSeparateIndex;
							}
						}
					}
				}
				else
				{
					_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());
					if( ClothesItem )
					{
						index = ClothesItem[m_IndependentItem->Get_m_sID()].sModelIndex;
						if( ClothesItem[m_IndependentItem->Get_m_sID()].cSexType ) // 여자면 인덱스 추가.
							index+= g_Model_GenderSeparateIndex;
					}			
				}
			}
			break;
		}
	}
	return index;
}

void _XWindow_SocketPlugInExp::MessagePrint(int msgindex)
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
		
		switch(msgindex) {
		case 0:
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_2808), TRUE, 0xFFFF0000 );	// _T("더 이상 장식물 공간을 확장할 수 없습니다.")
			}			
			break;
		case 1:
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR2), TRUE, 0xFFFF0000 ); // "장식물을 장착할 수 없는 물품입니다."
			}
			break;
		case 2:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR3));	// "이 물품은 장식물이 아닙니다."
			break;
		case 3:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR4)); // 물품을 교체하여, 새로운 장식물 장착을 시작 하시겠습니까?
			break;
		case 4:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_APPLYEQUIP));		// "장식물을 장착하시겠습니까?"
			break;
		case 5:
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR5), TRUE, 0xFFFF0000); // "더 이상 장식물을 장착할 수 없습니다."
			}			
			break;
		case 6:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_STOPITEM)); // 장식물 달기를 중단하시겠습니까?
			break;
		case 7 :
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_2574));
			break;	
		case 8 :
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR7));	// "이미 장착되어 있는 장식물은 되돌릴 수 없습니다."
			break;	
		case 9 :
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR8));	// "문장 장식물만 공통으로 장착할 수 있습니다."
			break;
		case 10:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR9));	// "물품을 교체할 수 없습니다. "
			break;
		}
		
		
		if( msgindex == 3 )
		{	
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_SOCKETPLUGIN_CHANGEOK, _XDEF_SOCKETPLUGIN_CHANGECANCLE);				
		}
		else if( msgindex == 4 )
		{
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_SOCKETPLUGIN_APPLYOK, _XDEF_SOCKETPLUGIN_APPLYCANCLE);				
		}
		else if( msgindex == 6 )
		{
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_SOCKETPLUGIN_CLOSEOK, _XDEF_SOCKETPLUGIN_CLOSECANCLE);
		}
		else
		{
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, messagestring, TRUE);		
		}
		g_pInventory_Window->ResetMouseSlot();
	}	
}

void _XWindow_SocketPlugInExp::SetSocketPlugInExMode(SocketPlugInExpMode mode)
{
	ResetSocketItemList();
	m_SocketPlugInMode = mode;
}

void _XWindow_SocketPlugInExp::SetSocketPlugInExpStep(SocketPlugInExpStep step)
{
	m_SocketPlugInStep = step;

	switch(m_SocketPlugInMode) 
	{
	case SM_EXPAND :
		{
			switch(m_SocketPlugInStep) 
			{
			case SS_READY :
				{
					for( int i = 0; i < m_CurrentItemMaxSocketCount; i++ )
						m_SocketSlotEx[i]->ShowWindow(TRUE);
					
					m_SocketPlugInButton->EnableWindow(TRUE);
					m_SocketPlugInButton->ShowWindow(TRUE);
					m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM);
				}
				break;
			case SS_STEP1 :
				{
					m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_2575 );
				}
				break;
			case SS_STEP2 : 
				{
					m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_2575);
				}
				break;			
			case SS_STEP3 :
				{
					m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_2575 );
				}
				break;	
			case SS_STEP4 :
				{
					m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_2575 );
				}
				break;
			}			
		}
		break;
	case SM_SEPARATION :
		{
			switch(m_SocketPlugInStep) 
			{
			case SS_READY :
				{
					m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM );
				}
				break;
			case SS_STEP1 :
				{
					m_SocketPlugInButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_2576 );
				}
				break;			
			}
		}
		break;
	case SM_DESTROY :
		{
			switch(m_SocketPlugInStep) 
			{
			case SS_READY :
				{
					m_SocketPlugInButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM );
				}
				break;
			case SS_STEP1 :
				{
					m_SocketPlugInButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_2577 );
				}
				break;			
			}
		}
		break;
	}
}

int _XWindow_SocketPlugInExp::GetSlotIndex(void)
{
	if( m_MsgSocketPuluInOpen ) 
		return -1;
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	
	for(int i = 0 ; i < 4 ; i++)
	{
		if(scrnpos->x > m_WindowPosition.x + m_SlotPosition[i].x && scrnpos->x < m_WindowPosition.x + m_SlotPosition[i].x + 30)
		{
			if(scrnpos->z > m_WindowPosition.y + m_SlotPosition[i].y && scrnpos->z < m_WindowPosition.y + m_SlotPosition[i].y + 30)
			{
				if( m_SocketItemList[i].Get_m_sID() )
					return i;
			}
		}
	}
	return -1;
}

void _XWindow_SocketPlugInExp::DrawDragIcon(void)
{

}

void _XWindow_SocketPlugInExp::ProcessDbClick(void)
{

}

void _XWindow_SocketPlugInExp::ClickStartButton(void)
{
	if( m_bChangeButtonMode )
	{
		if( !m_IndependentItem )
		{
			MessagePrint(0);
		}
		else
		{
			g_pLocalUser->m_bSocketPlugInLock = TRUE;
			m_MsgSocketPuluInOpen = TRUE;
			
			if( g_pInventory_Window->GetSocketListCount() == 0 )
			{
				MessagePrint(0);
			}
			else
			{
				MessagePrint(4);
			}
		}
	}
	else
	{
		g_pInventory_Window->ResetSocketIndexList();
		ResetSocketItemList();
		m_SocketPlugInButton->EnableWindow(FALSE);
		m_SocketPlugInButton->ShowWindow(FALSE);					
		//RebuildListBoxText(_BOXTEXTMODE_READY);
	}
}

void _XWindow_SocketPlugInExp::ResetSocketItemList(void)
{
	m_bChangeButtonMode		= FALSE;
	m_IndependentItem		= NULL;

	for( int i = 0; i < 4; i++ )
	{
		m_SocketItemList[i].Reset();	
		m_SocketExpItemList[i].Reset();
		m_SocketSlotEx[i]->ShowWindow(FALSE);
	}
	m_bHaveMouseSlot = FALSE;
	m_MouseSlotItem.ResetMouseSlotItem();
}

BOOL _XWindow_SocketPlugInExp::SetSocketPlugInExpandStart(int invenslotnumber)
{
	if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[invenslotnumber].Get_m_cType()) ||
		_XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[invenslotnumber].Get_m_cType()) )
	{
		// error 처리 ------------------------------------------------------------------------------------=
		if( g_pLocalUser->m_UserItemList[invenslotnumber].Get_m_cSecond() == _XGI_SC_CLOTHES_BACKPACK )
		{
			MessagePrint(7);
			return FALSE;
		}
		// -----------------------------------------------------------------------------------------------=
		if( m_IndependentItem )
		{
			MessagePrint(10);
			return FALSE;
		}
		else
		{
			char maxSocketCount = g_pInventory_Window->GetMaxSocketCount(g_pLocalUser->m_UserItemList[invenslotnumber].Get_m_cType(), g_pLocalUser->m_UserItemList[invenslotnumber].Get_m_sID());
			if( g_pLocalUser->m_UserItemList[invenslotnumber].Get_m_ucSocketCount() >= maxSocketCount )
			{
				// 확장할 소켓이 없을 경우
				MessagePrint(7);
				return FALSE;
			}
			
			m_CurrentItemMaxSocketCount = maxSocketCount;			
			m_IndependentItem = &g_pLocalUser->m_UserItemList[invenslotnumber];
			g_pInventory_Window->m_iSocketIndexList[0] = invenslotnumber;
			SetSocketPlugInExpStep(SS_READY);
			
			for( int i = 0; i < g_pLocalUser->m_UserItemList[invenslotnumber].Get_m_ucSocketCount(); i++ )
			{
				if( g_pLocalUser->m_UserItemList[invenslotnumber].Get_m_sSocket(i) )
				{
					m_SocketItemList[i].Reset();
					m_SocketItemList[i].Set_m_cType(_XGI_FC_SOCKET);					
					m_SocketItemList[i].Set_m_sID( g_pLocalUser->m_UserItemList[invenslotnumber].Get_m_sSocket(i) );
					
					m_SocketExpItemList[i].Reset();
					m_SocketExpItemList[i].Set_m_cType(_XGI_FC_SOCKET);					
					m_SocketExpItemList[i].Set_m_sID( g_pLocalUser->m_UserItemList[invenslotnumber].Get_m_sSocket(i) );
				}
			}										
			g_pInventory_Window->ResetMouseSlot();
			m_bChangeButtonMode = FALSE;
		}																											
	}
	else if( g_pLocalUser->m_UserItemList[invenslotnumber].Get_m_cType() == _XGI_FC_SOCKET )
	{
		if( m_IndependentItem )
		{
			
		}
		else
		{
			MessagePrint(0);										
			return FALSE;
		}
	}
	else
	{
		MessagePrint(1);									
		return FALSE;
	}	
	return TRUE;
}