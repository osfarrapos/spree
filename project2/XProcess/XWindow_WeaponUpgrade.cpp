// XWindow_WeaponUpgrade.cpp: implementation of the _XWindow_WeaponUpgrade class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XMessageWindow.h"
#include "XWindow_WeaponUpgrade.h"
#include "XWindow_ChargeGauge.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_WeaponUpgrade::_XWindow_WeaponUpgrade()
{
	m_InchantApplyButton	= NULL;
	m_pApplyCancleButton	= NULL;
	m_pClosebutton			= NULL;
	
	m_InchantStep			= US_NONE;
	m_InchantItem			= NULL;
	m_slotImage				= NULL;
}

_XWindow_WeaponUpgrade::~_XWindow_WeaponUpgrade()
{

}

BOOL _XWindow_WeaponUpgrade::Initialize( void )
{
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 184, 71, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" ) );	
	pTitleBar->SetClipRect( 0, 125, 183, 195 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pBackBoard = new _XImageStatic;
	pBackBoard->Create( 0, 60, 184, 271, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ) );	
	pBackBoard->SetClipRect( 0, 1, 183, 211 );
	InsertChildObject( pBackBoard );

	_XImageStatic* pSocketItemLeftView = new _XImageStatic;
	pSocketItemLeftView->Create( 7, 26, 93, 141, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pSocketItemLeftView->SetClipRect( 4, 140, 89, 254 );
	InsertChildObject( pSocketItemLeftView );

	_XImageStatic* pSocketItemRightView = new _XImageStatic;
	pSocketItemRightView->Create( 176, 26, 261, 141, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pSocketItemRightView->SetClipRect( 4, 140, 88, 254 );
	pSocketItemRightView->SetScale( -1.0f, 1.0f );
	InsertChildObject( pSocketItemRightView );

	int mainimagebackindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );
	
	
	m_slotImage = new _XImageStatic;
	m_slotImage->Create( 12, 172, 44, 204, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" ) );
	m_slotImage->SetClipRect( 214, 31, 246, 63 );
	InsertChildObject( m_slotImage );

	// 버튼들 ---------------------------------------------------------------------------------------------------------=
	int applybtnindex = g_MainInterfaceTextureArchive.FindResource( "mi_skillwin.tga" );
	_XBTN_STRUCTURE socketpluginbtnstruct = 
	{ 
		TRUE, { 9, 242  }, { 104, 20 }, 
		_XDEF_INCHANT_STARTBUTTON, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive 
	};
	m_InchantApplyButton = new _XButton;
	m_InchantApplyButton->Create( socketpluginbtnstruct );	
	m_InchantApplyButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_InchantApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 152, 110, 176 );
	m_InchantApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200 ); 
	m_InchantApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 201, 110, 225 );
	m_InchantApplyButton->ShowWindow(FALSE);
	InsertChildObject( m_InchantApplyButton );	

	applybtnindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XBTN_STRUCTURE applycanclebtnstruct = 
	{ 
		TRUE, { 117, 242  }, { 60, 24 }, 
		_XDEF_INCHANT_CANCLEBUTTON, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive
	};
	m_pApplyCancleButton = new _XButton;
	m_pApplyCancleButton->Create( applycanclebtnstruct );	
	m_pApplyCancleButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pApplyCancleButton->SetButtonText(_XGETINTERFACETEXT(ID_STRING_COMMON_CANCLE));		// _T("취소")
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 187, 152, 247, 176 );
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  187, 127, 247, 151 );
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	InsertChildObject( m_pApplyCancleButton );

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_INCHANT_CANCLEBUTTON,
									  charinfointerface,charinfointerface,charinfointerface, 
									  &g_MainInterfaceTextureArchive };
	m_pClosebutton = new _XButton;
	m_pClosebutton->Create( closebtnstruct );	
		
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( m_pClosebutton );

	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 153, 2 }, { 12, 12 }, _XDEF_INCHANT_HELPBUTTON,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	// ----------------------------------------------------------------------------------------------------------------=	

	// Model View Camera Setting =========================================
	m_UpgradeItemModelViewCamera.Set( 45, 154.0f/60.0f, 0.01f, 180.0f);
	m_UpgradeItemModelViewCamera.Set(0.0f, 0.0f, 0.0f);	
	m_UpgradeItemModelViewCamera.SetDistanceLimits( 0.01, 30.0f );
	m_UpgradeItemModelViewCamera.SetDistance( 1.0f );	
	m_UpgradeItemModelViewCamera.UpdateViewMatrix();
	m_UpgradeItemModelViewCamera.UpdateProjMatrix();

	// ViewPortReposition - for Model View
	m_UpgradeItemModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_UpgradeItemModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
	m_UpgradeItemModelViewPort.Width  = 154;
	m_UpgradeItemModelViewPort.Height = 60;
	m_UpgradeItemModelViewPort.MinZ = 0.0f;
	m_UpgradeItemModelViewPort.MaxZ = 1.0f;

	m_UpgradeItemtRotateAngle = 0.0f;
	
	return TRUE;
}

void _XWindow_WeaponUpgrade::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_WeaponUpgrade::Draw( _XGUIObject*& pfocusobject )
{
#ifdef _XTS_ITEMUPGRADE
	if(!this->m_ShowWindow)	return;
	_XWindow::Draw(pfocusobject);

	_XDrawSolidBar( m_WindowPosition.x+8, m_WindowPosition.y+144, m_WindowPosition.x+172, m_WindowPosition.y+235, 0xFF1A1819 );
	_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+144, m_WindowPosition.x+172, m_WindowPosition.y+235, 0.0f, 0xFF000000 );

	m_slotImage->Draw();
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	g_XBaseFont->Puts_Separate( m_WindowPosition.x+54, m_WindowPosition.y+174, m_InchantInfoText, 114, 1.0, 3 );

	if( m_InchantItem )
	{
		DrawInchantItem();
		
		g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_InchantItem->m_cType, m_InchantItem->m_sID));
		int tempLength = strlen( _XGameItem::GetItemNick(m_InchantItem->m_cType, m_InchantItem->m_sID) );
		if( tempLength > 1 )
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+32, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemNick(m_InchantItem->m_cType, m_InchantItem->m_sID ) );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+47, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_InchantItem->m_cType, m_InchantItem->m_sID ) );
		}
		else
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+40, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_InchantItem->m_cType, m_InchantItem->m_sID ) );
		}

		TCHAR tempSTr[128];
		sprintf( tempSTr, _XGETINTERFACETEXT(ID_STRING_INCHANT_2396), m_InchantItem->m_ucStrength+1 ); //"%d차 제련"
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+151, _XFONT_ALIGNTYPE_CENTER, tempSTr );

		g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+13, m_WindowPosition.y+173, &m_InchantStuff[0], FALSE);
		_XDrawSolidBar( m_WindowPosition.x+13, m_WindowPosition.y+173, m_WindowPosition.x+42, m_WindowPosition.y+202, D3DCOLOR_ARGB(160, 0, 0, 0) );
		g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+13, m_WindowPosition.y+173, &m_InchantStuff[1]);
	}
	g_XBaseFont->Flush();	
#endif
}

BOOL _XWindow_WeaponUpgrade::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	if( m_InchantStuff[0].m_sID > 0 )
	{	
		MouseState* mousestate = gpInput->GetMouseState();
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

		if(mousestate)
		{
			if(CheckMousePosition())
			{
				if(scrnpos->x > m_WindowPosition.x+12 && scrnpos->x < m_WindowPosition.x+44)
				{
					if(scrnpos->z > m_WindowPosition.y+172 && scrnpos->z < m_WindowPosition.y+203)
					{
						if(!mousestate->bButton[0])
						{
							if( g_pDefaultTooltip_Window )
							{							
								if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_DEFAULTITEM, 0, m_InchantStuff[0].m_cType, m_InchantStuff[0].m_sID ) )
								{
									g_pDefaultTooltip_Window->ShowWindow( TRUE );
								}
							}
						}
					}
				}
			}
		}
	}

	if(!_XWindow::Process(pfocusobject)) return FALSE;	
	
	return TRUE;
}

void _XWindow_WeaponUpgrade::ShowWindow(BOOL show)
{
	if(show)
	{
		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;

			g_pInventory_Window->SetWeaponUpgradeWindowPtr(this);
		}
	}
	else
	{
		// 사용 끝나면 Inventory lock 해제
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

void _XWindow_WeaponUpgrade::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );
	m_UpgradeItemModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_UpgradeItemModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
}

void _XWindow_WeaponUpgrade::DrawInchantItem( void )
{
#ifdef _XTS_ITEM_V16

	if( !m_InchantItem )	return;

	D3DXMATRIX			rotmat;
	D3DXMatrixIdentity(&rotmat);
	
	m_UpgradeItemtRotateAngle++;
	D3DXMatrixRotationY(&rotmat, _X_RAD(m_UpgradeItemtRotateAngle));

	if( m_InchantItem->m_cType == _XGI_FC_WEAPON || m_InchantItem->m_cType == _XGI_FC_WEAPON2 || m_InchantItem->m_cType == _XGI_FC_WEAPON3 )
	{
		D3DXMATRIX			zrotmat;
		D3DXMatrixRotationZ(&zrotmat, _X_RAD(90.0f));
		
		D3DXMatrixMultiply( &rotmat, &rotmat, &zrotmat );
	}	
	
	// Initialize Matrix
	int modelindex = GetInchantItemModelIndex();
	D3DXMatrixIdentity(&m_UpgradeItemRHWMatrix);
	D3DXMatrixTranslation( &m_UpgradeItemRHWMatrix, 
		-g_ItemModel_Weapon[0][modelindex]->OBB_Center.x,
		-g_ItemModel_Weapon[0][modelindex]->OBB_Center.y,
		-g_ItemModel_Weapon[0][modelindex]->OBB_Center.z	);
	
	// Camera/ViewPort Setting
	m_UpgradeItemModelViewCamera.UpdateViewMatrix( NULL, FALSE );
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&m_UpgradeItemModelViewCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&m_UpgradeItemModelViewCamera.GetProjMatrix());
	gpDev->SetViewport(&m_UpgradeItemModelViewPort);
	
	// Z Buffer를 clear 한다. - 현재 viewport영역만 clear된다.
	gpDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	
	// Rendering			
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );	
	
	
	
	FLOAT scalefactor;
	switch(m_InchantItem->m_cType) 
	{
	case _XGI_FC_WEAPON:
	case _XGI_FC_WEAPON2:
	case _XGI_FC_WEAPON3:
		{
			scalefactor = 1.0f / g_ItemModel_Weapon[0][modelindex]->m_ObjectRadius;
			D3DXMATRIX matRot;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixScaling(&matRot, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply( &matRot, &matRot, &rotmat );							
			D3DXMatrixMultiply(&m_UpgradeItemRHWMatrix, &m_UpgradeItemRHWMatrix, &matRot);
			
			g_ItemModel_Weapon[0][modelindex]->RenderInterfaceItem(&m_UpgradeItemRHWMatrix);
		}		
		break;
	}	
	
	// View Matrix, Projection Matrix, Viewport 다시 되돌린다. ====
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetProjMatrix());
	gpDev->SetViewport(&g_MainViewPort);
#endif
}

int _XWindow_WeaponUpgrade::GetInchantItemModelIndex(void)
{
	if( !m_InchantItem ) return 0;

	int index;
	switch(m_InchantItem->m_cType) 
	{
	case _XGI_FC_WEAPON:
		{
			index = g_WeaponItemProperty[m_InchantItem->m_sID].sModelIndex;															
		}		
		break;
	case _XGI_FC_WEAPON2:
		{
			index = g_WeaponItemProperty2[m_InchantItem->m_sID].sModelIndex;															
		}		
		break;
#ifdef _XTS_ITEM_V16
	case _XGI_FC_WEAPON3:
		{
			index = g_WeaponItemProperty3[m_InchantItem->m_sID].sModelIndex;															
		}		
		break;
#endif
	default:
		index = 0;
		break;
	}
	return index;
}

void _XWindow_WeaponUpgrade::SetItemInchantStep(InchantStep step)
{
#ifdef _XTS_ITEMUPGRADE
	m_InchantStep = step;
	RebuildStuffInfo();

	switch(m_InchantStep) 
	{
	case US_NONE :
		{
			m_InchantItem = NULL;

			m_InchantStuff[0].Reset();
			m_InchantStuff[1].Reset();

			m_InchantApplyButton->ShowWindow(FALSE);
			m_InchantApplyButton->EnableWindow(TRUE);

			m_pApplyCancleButton->EnableWindow(TRUE);
			m_pClosebutton->EnableWindow(TRUE);

			if( g_pInventory_Window )
			{
				g_pInventory_Window->ResetSocketIndexList();
				g_pInventory_Window->m_UsedLifeItemIndex = 0;
			}
		}
		break;
	case US_READY :
		{
			m_InchantApplyButton->ShowWindow(TRUE);
			m_InchantApplyButton->SetButtonText(_T("물품 내리기"));

			m_pApplyCancleButton->EnableWindow(TRUE);
			m_pClosebutton->EnableWindow(TRUE);
		}
		break;
	case US_STEP1 :	// 재료를 올린 상태
		{
			m_InchantApplyButton->EnableWindow(TRUE);
			m_InchantApplyButton->SetButtonText(_T("제련 시작"));

			m_pApplyCancleButton->EnableWindow(TRUE);
			m_pClosebutton->EnableWindow(TRUE);			
		}
		break;
	case US_STEP2 :	// 제련버튼 클릭시
		{
			m_InchantApplyButton->EnableWindow(FALSE);

			m_pApplyCancleButton->EnableWindow(FALSE);
			m_pClosebutton->EnableWindow(FALSE);

			TCHAR messagestring[256];
			memset(messagestring, 0, sizeof(TCHAR)*256);
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2104), //"%d차 제련에 %d금을 지불합니다. %s를 제련하시겠습니까?"
				m_InchantItem->m_ucStrength+1, g_ItemInchantTable[m_InchantItem->m_ucStrength].uiPrice, 
				_XGameItem::GetItemName(m_InchantItem->m_cType, m_InchantItem->m_sID) );	
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_INCHANT_STARTYESBUTTON, _XDEF_INCHANT_STARTNOBUTTON);
		}
		break;
	case US_STEP3 :	// 제련 시작 
		{
			g_NetworkKernel.SendPacket( MSG_NO_INCHANT_CTRL, inchant_mode_str_req );
		}
		break;
	case US_STEP4 :
		{
			_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
			if( pChargeGauge_Window )
			{
				if(!pChargeGauge_Window->GetShowStatus())
				{
					pChargeGauge_Window->SetDelayTime( 12000.0f );
					POINT pos = this->GetWindowPos();
					pos.x += 12;
					pos.y += 216;
					pChargeGauge_Window->SetDrawPosition(pos);
					pChargeGauge_Window->SetGaugeMode( _XGAUGE_INCHANT );
					pChargeGauge_Window->ShowWindow(TRUE);
					pChargeGauge_Window->SetSelectedNumber(m_FirstResult);	// 결과값으로 이용
					g_MainWindowManager.SetTopWindow( pChargeGauge_Window );
				}
			}
		}
		break;
	case US_STEP5 :
		{
			g_NetworkKernel.SendPacket( MSG_NO_INCHANT_CTRL, inchant_mode_str_complete );
		}
		break;
	default:
		{
			m_InchantApplyButton->ShowWindow(FALSE);
			m_InchantApplyButton->EnableWindow(TRUE);

			m_pApplyCancleButton->EnableWindow(TRUE);
			m_pClosebutton->EnableWindow(TRUE);
		}
		break;
	}
#endif
}

void _XWindow_WeaponUpgrade::MessagePrint(int msgindex)
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
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2105)); //_T("무기가 아닙니다.")
			break;
		case 1:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2106)); //_T("이 무기는 제련이 불가능한 무기입니다.")); 
			break;
		case 2:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2107)); //_T("이 무기는 더 이상 제련할 수 없습니다."));
			break;
		case 3:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2108)); //_T("제련에 필요한 재료가 아닙니다."));
			break;
		case 4:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2109)); //_T("제련할 물품을 먼저 올린 후 재료를 등록해 주세요."));
			break;
		case 5:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR9));
			break;
		case 6:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2110)); //_T("이미 재료가 등록되어 있습니다."));
			break;
		case 7:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1760)); //_T("재료가 부족합니다."));
			break;
		}
		
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, messagestring, TRUE);		
		
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
	}	
}

void _XWindow_WeaponUpgrade::ClickStartButton(void)
{
	if( m_InchantStep == US_READY )
	{
		SetItemInchantStep(US_NONE);
	}
	else if( m_InchantStep == US_STEP1 )
	{
		SetItemInchantStep(US_STEP2);
	}
}

void _XWindow_WeaponUpgrade::RebuildStuffInfo(void)
{
#ifdef _XTS_ITEMUPGRADE
	memset(m_InchantInfoText, 0, sizeof(TCHAR)*128);
	
	switch(m_InchantStep) 
	{
	case US_NONE :
		{
			strcpy( m_InchantInfoText, _XGETINTERFACETEXT(ID_STRING_INCHANT_2111));//"제련할 물품을 등록하여 주십시요.");
		}
		break;
	case US_READY :
	case US_STEP1 :	// 재료를 올린 상태
		{
			if( m_InchantItem )
			{
				//"%s %d개를 재료로 사용합니다. "
				sprintf( m_InchantInfoText, _XGETINTERFACETEXT(ID_STRING_INCHANT_2112), _XGameItem::GetItemName( g_ItemInchantTable[m_InchantItem->m_ucStrength].cType,
						 g_ItemInchantTable[m_InchantItem->m_ucStrength].sID), g_ItemInchantTable[m_InchantItem->m_ucStrength].ucCount );

				m_InchantStuff[0].Reset();
				m_InchantStuff[0].m_cType	= g_ItemInchantTable[m_InchantItem->m_ucStrength].cType;
				m_InchantStuff[0].m_sID		= g_ItemInchantTable[m_InchantItem->m_ucStrength].sID;
				m_InchantStuff[0].m_ucCount = g_ItemInchantTable[m_InchantItem->m_ucStrength].ucCount;
				m_InchantStuff[0].m_cSecond = _XGI_SC_LIFE_CONSUMING;	// 임시
			}			
		}
		break;
	case US_STEP4 :	// 제련버튼 클릭시
		{
			strcpy( m_InchantInfoText, _XGETINTERFACETEXT(ID_STRING_INCHANT_2113)); //"물품을 제련중입니다."
		}
		break;
	}
#endif
}

int _XWindow_WeaponUpgrade::CheckStuffCount(int selectslot)
{
	if( g_pLocalUser->m_UserItemList[selectslot].m_sID == m_InchantStuff[0].m_sID &&
		g_pLocalUser->m_UserItemList[selectslot].m_cType == m_InchantStuff[0].m_cType )
	{
		if( g_pLocalUser->m_UserItemList[selectslot].m_ucCount >= m_InchantStuff[0].m_ucCount )
			return 1;
		else
			return -1;
	}
	else
	{
		return -2;
	}
	return 0;
}

int _XWindow_WeaponUpgrade::CheckWeaponGrade(int searchslot)
{
#ifdef _XTS_ITEMUPGRADE
	int grade = _XGameItem::GetItemGrade(g_pLocalUser->m_UserItemList[searchslot].m_cType, g_pLocalUser->m_UserItemList[searchslot].m_sID);
	switch(grade) 
	{
	case 0:
	case 10:
	case 20:
	case 1 :
	case 2 :
	case 5 :
		{
			if( g_pLocalUser->m_UserItemList[searchslot].m_ucStrength > 8 )
			{
				MessagePrint(2);
				return -2;
			}
			else
				return 1;
		}
		break;
	case 3 :
	case 4 :
	case 6 :
		{
			MessagePrint(1);
			return -1;
		}
		break;
	}
#endif
	return -1;
}
