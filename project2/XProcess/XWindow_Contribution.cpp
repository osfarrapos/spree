// XWindow_Contribution.cpp: implementation of the _XWindow_Contribution class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_TradeConfirm.h"
#include "XWindow_Contribution.h"
#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static int _XDEF_CONTRISLOT_WIDTH = 5;
static int _XDEF_CONTRISLOT_HEIGHT = 4;


_XWindow_Contribution::_XWindow_Contribution()
{
	ResetContributionData();
}

_XWindow_Contribution::~_XWindow_Contribution()
{

}

void _XWindow_Contribution::ResetContributionData()
{
	m_CurrentItemContribution	= 0;
	m_SendDelayTime				= 0;
	m_ContributionType			= -1;
	m_ContributionSecondType	= -1;
	m_ContributionThirdType		= -1;
	m_ContributionId			= -1;
	m_ContributionModelIndex	= -1;
	m_ContributionGender		= -1;
	m_ContributionIvenIndex		= -1;
	m_ContributionItemCount		= 0;
}

BOOL _XWindow_Contribution::Initailize(void)
{
	// Create GUI Object =================================================================
	// Border---------------------------------------------------------------------------------------------=
	int contribution_imageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create(0, 0, 184, 70, &g_MainInterfaceTextureArchive, contribution_imageindex );
	pTitleBar->SetClipRect( 0, 125, 183, 194 );
	InsertChildObject(pTitleBar);

	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create( 0, 69, 184, 269, &g_MainInterfaceTextureArchive, 
						   g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ));
	pBottomBorder->SetClipRect( 0, 10,  183, 211 );
	InsertChildObject( pBottomBorder );

	_XImageStatic* pModelBorder = new _XImageStatic;
	pModelBorder->Create( 7, 25, 174, 171, &g_MainInterfaceTextureArchive, 
						   g_MainInterfaceTextureArchive.FindResource( "MI_npc_present.tga" ));
	pModelBorder->SetClipRect( 0, 0,  168, 147 );
	InsertChildObject( pModelBorder );
	// ---------------------------------------------------------------------------------------------------=

	// close, help Button --------------------------------------------------------------------------=
	contribution_imageindex = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	/*_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_CONTRIWINDOW_CLOSEBUTTON,
		contribution_imageindex,contribution_imageindex,contribution_imageindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );	*/
	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_CONTRIWINDOW_HELPBUTTON,
		contribution_imageindex,contribution_imageindex,contribution_imageindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	// ---------------------------------------------------------------------------------------------=

	// 기부, 취소 Button ---------------------------------------------------------------------------------=
	contribution_imageindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );

	_XBTN_STRUCTURE Applybtnstruct = { TRUE, { 35, 238 }, { 61, 25 }, _XDEF_CONTRIWINDOW_APPLYBUTTON,
		contribution_imageindex,contribution_imageindex,contribution_imageindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* pContriApplyButton = new _XButton;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		int imageindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");

		Applybtnstruct.position.x = 6;
		Applybtnstruct.windowsize.cx = 108;
		Applybtnstruct.windowsize.cy = 24;
		Applybtnstruct.imageindex_click = imageindex;
		Applybtnstruct.imageindex_normal = imageindex;
		Applybtnstruct.imageindex_ready = imageindex;
	}
	
	pContriApplyButton->Create( Applybtnstruct );	
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pContriApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200 );
		pContriApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 152, 110, 176 );	
		pContriApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 200, 110, 224 );
	}
	else
	{
		pContriApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
		pContriApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );	
		pContriApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );		
	}
	
	pContriApplyButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_CONTRIBUTION_ITEMCONTRIBUTION );	// _T("물품기부")
	InsertChildObject( pContriApplyButton );

	_XBTN_STRUCTURE Canclebtnstruct = { TRUE, { 95, 238 }, { 61,  25 }, _XDEF_CONTRIWINDOW_CANCLE,
		contribution_imageindex,contribution_imageindex,contribution_imageindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* pContriCancleButton = new _XButton;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		Canclebtnstruct.position.x = 118;
	}

	pContriCancleButton->Create( Canclebtnstruct );	
	
	pContriCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
	pContriCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );	
	pContriCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	pContriCancleButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE );	// _T("취소")
	InsertChildObject( pContriCancleButton );
	// ---------------------------------------------------------------------------------------------------=

	// 기여 Box ------------------------------------------------------------------------------------------=
	m_ContributionBox.Create( m_WindowPosition.x+10, m_WindowPosition.y+206, m_WindowPosition.x+171, m_WindowPosition.y+227, 
							  &g_MainInterfaceTextureArchive, 
							   g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" ) );
	m_ContributionBox.SetClipRect( 0, 0, 161, 21 );

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_ContributionBox.ShowWindow( FALSE );
	}
	// ---------------------------------------------------------------------------------------------------=

	// Model View Camera Setting =========================================
	m_ModelViewCamera.Set( 45, 110.0f/90.0f, 0.01f, 180.0f);
	m_ModelViewCamera.Set(0.0f, 0.0f, 0.0f);	
	m_ModelViewCamera.SetDistanceLimits( 0.01, 30.0f );
	m_ModelViewCamera.SetDistance( 1.0f );	
	m_ModelViewCamera.UpdateViewMatrix();
	m_ModelViewCamera.UpdateProjMatrix();

	// ViewPortReposition - for Model View
	m_ModelViewPort.X = (DWORD)(m_WindowPosition.x + 36);
	m_ModelViewPort.Y = (DWORD)(m_WindowPosition.y + 54);
	m_ModelViewPort.Width  = 110;
	m_ModelViewPort.Height = 90;
	m_ModelViewPort.MinZ = 0.0f;
	m_ModelViewPort.MaxZ = 1.0f;

	m_RotateAngle = 0.0f;

	return TRUE;
}

void _XWindow_Contribution::DestroyWindow(void)
{

}

void _XWindow_Contribution::Draw(_XGUIObject*& pfocusedobject)
{
	if( !this->m_ShowWindow ) return;
	
	_XWindow::Draw( pfocusedobject );

	_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+176, m_WindowPosition.x+174, m_WindowPosition.y+233, 0xFF1A1819 );
	_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+176, m_WindowPosition.x+174, m_WindowPosition.y+233, 0.0f, 0xFF000000 );

	_XDrawSolidBar( m_WindowPosition.x+65, m_WindowPosition.y+182, m_WindowPosition.x+169, m_WindowPosition.y+199, 0xFF0D0B0C );
	_XDrawRectAngle( m_WindowPosition.x+65, m_WindowPosition.y+182, m_WindowPosition.x+169, m_WindowPosition.y+199, 0.0f, 0xFF000000 );
	 
	 m_ContributionBox.Draw( m_WindowPosition.x+10, m_WindowPosition.y+206 );	 

	 g_XBaseFont->SetColor(_XSC_DEFAULT);
	 if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	 {
		g_XBaseFont->PrintAlign(m_WindowPosition.x+16, m_WindowPosition.y+183, 1.0f, _XFONT_ALIGNTYPE_LEFT, _T("%s %d"), _XGETINTERFACETEXT(ID_STRING_CONTRIBUTION_CONTRIBUTIONRATE), m_CurrentItemContribution );	// _T("기여도 :")
	 }
	 else
	 {
		 g_XBaseFont->PrintAlign(m_WindowPosition.x+16, m_WindowPosition.y+186, 1.0f, _XFONT_ALIGNTYPE_LEFT, _T("%s %d"), _XGETINTERFACETEXT(ID_STRING_CONTRIBUTION_CONTRIBUTIONRATE), m_CurrentItemContribution );	// _T("기여도 :")
	 }
	 g_XBaseFont->Flush();

	 TCHAR tempstr[128];	 
	 g_XBaseFont->SetColor( 0xFF01FE06 );

	 if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	 {
		strcpy( tempstr, _XGETINTERFACETEXT(ID_STRING_CONTRIBUTION_CURRENTCONTRIBUTION) );	// "보유기여도 : %d"
		LPTSTR pStr = strstr( tempstr, _T("%d") );
		if(pStr)
		{
			*pStr = 0;
			g_XBaseFont->SetBoldMode( TRUE );
			g_XBaseFont->Puts(m_WindowPosition.x+16, m_WindowPosition.y+204, tempstr );
			g_XBaseFont->PrintAlign(m_WindowPosition.x+m_WindowSize.cx - 10, m_WindowPosition.y+216, 1.0f, _XFONT_ALIGNTYPE_RIGHT, _T("%d"), g_pLocalUser->m_CharacterInfo.Get_iContribution() );
			g_XBaseFont->Flush();
			g_XBaseFont->SetBoldMode( FALSE );		 
		}			
	 }
	 else
	 {
		 sprintf( tempstr, _XGETINTERFACETEXT(ID_STRING_CONTRIBUTION_CURRENTCONTRIBUTION), g_pLocalUser->m_CharacterInfo.Get_iContribution() );	// "보유기여도 : %d"
		 g_XBaseFont->SetBoldMode( TRUE );
		 g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+212, _XFONT_ALIGNTYPE_CENTER, tempstr);
		 g_XBaseFont->Flush();
		 g_XBaseFont->SetBoldMode( FALSE );		 
	 }
	 
	 DrawContributionItem();
	 if( m_ContributionType > -1 && m_ContributionId > 0 )
	 {
		g_XBaseFont->SetColor(0xFFD5BE92);
		g_XBaseFont->SetBoldMode( TRUE );
		g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+140, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_ContributionType, m_ContributionId ) );
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode( FALSE );
	 }	 

	 g_XBaseFont->Flush();
}
BOOL _XWindow_Contribution::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)		return FALSE;
	if(!this->m_ShowWindow) return FALSE;

	_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
	if( pTradeMessageBox )
	{
		if(pfocusobject == _XDEF_DUMMYID_CONTRIBUTION) pfocusobject = NULL;
		
		if(!_XWindow::Process(pfocusobject)) return FALSE;
		if( pTradeMessageBox->GetShowStatus() ) return FALSE;
	}

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	// Drag and Drop 처리
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if(CheckMousePosition())
				{
					// Focus
					if( scrnpos->x > m_WindowPosition.x+8 && scrnpos->x < m_WindowPosition.x+173 &&		 
						scrnpos->z > m_WindowPosition.y+26 && scrnpos->z < m_WindowPosition.y+170 )
					{
						if( !pfocusobject || pfocusobject == this )
							pfocusobject = _XDEF_DUMMYID_CONTRIBUTION;
					}

					if(g_pInventory_Window)
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

									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2120), TRUE);									

									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();	
								}
								
								if( flag )
								{
									// 올리기
									if(g_pInventory_Window->m_MouseSlotItem.IsStackable())
									{
										_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)m_pMyManager->FindWindow(_XDEF_WTITLE_TRADECONFIRM);
										if(pTradeConfirm_Window)
										{
											pTradeConfirm_Window->ShowWindow(TRUE);
											pTradeConfirm_Window->m_IMEControl.SetFocus();
											pTradeConfirm_Window->m_SelectedCategory1 = g_pInventory_Window->m_MouseSlotItem.Get_m_cType();
											pTradeConfirm_Window->m_SelectedItemID = g_pInventory_Window->m_MouseSlotItem.Get_m_sID();
											pTradeConfirm_Window->m_TradeMode = _XCONTRIBUTION_TOCONTRIBUTION_ITEM;
										}
									}
									else
									{
										m_ContributionType			= g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType();
										m_ContributionSecondType	= g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cSecond();
										m_ContributionId			= g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sID();
										m_ContributionIvenIndex		= g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;										
										m_ContributionItemCount		= g_pInventory_Window->m_MouseSlotItem.Get_m_ucCount();

										g_pInventory_Window->m_ContributionItemInvenNumber = m_ContributionIvenIndex;
										g_pInventory_Window->m_bHaveMouseSlot = FALSE;
										g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();										

										SetAddData();																				
									}
								}								
							}
						}
					}					
				}
			}			
		}		

		if(!mousestate->bButton[0])
		{
			if(pfocusobject == _XDEF_DUMMYID_CONTRIBUTION)
			{
			   pfocusobject = NULL;
			}
		}
	}

	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}


void _XWindow_Contribution::DrawContributionItem( void )
{
	D3DXMATRIX			rotmat;
	D3DXMatrixIdentity(&rotmat);

	m_RotateAngle++;
	D3DXMatrixRotationY(&rotmat, _X_RAD(m_RotateAngle));

#ifdef _XTS_ITEM_OPTIMIZATION
	if( _XGI_FC_WEAPON == GetEqualItemType(m_ContributionType) )
#else
	if( m_ContributionType == _XGI_FC_WEAPON || m_ContributionType == _XGI_FC_WEAPON2 || m_ContributionType == _XGI_FC_WEAPON3 )
#endif
	{
		D3DXMATRIX			zrotmat;
		D3DXMatrixRotationZ(&zrotmat, _X_RAD(45.0f));
		
		D3DXMatrixMultiply( &rotmat, &rotmat, &zrotmat );
	}	

	// Initialize Matrix
	D3DXMatrixIdentity(&m_RHWMatrix);	

	// Camera/ViewPort Setting
	m_ModelViewCamera.UpdateViewMatrix( NULL, FALSE );
	//m_ModelViewCamera.UpdateViewMatrix();
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&m_ModelViewCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&m_ModelViewCamera.GetProjMatrix());
	gpDev->SetViewport(&m_ModelViewPort);

	// Z Buffer를 clear 한다. - 현재 viewport영역만 clear된다.
	gpDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);

	// Rendering			
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );
	
	FLOAT scalefactor;
	switch(m_ContributionType) 
	{
	case _XGI_FC_WEAPON:
	case _XGI_FC_WEAPON2:
	case _XGI_FC_WEAPON3:
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_ItemModel_Weapon[0][m_ContributionModelIndex]->OBB_Center.x,
						   -g_ItemModel_Weapon[0][m_ContributionModelIndex]->OBB_Center.y,
						   -g_ItemModel_Weapon[0][m_ContributionModelIndex]->OBB_Center.z	);

			scalefactor = 0.5f / g_ItemModel_Weapon[0][m_ContributionModelIndex]->m_ObjectRadius;
			D3DXMATRIX matRot;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixScaling(&matRot, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply( &matRot, &matRot, &rotmat );							
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &matRot);					
			
			g_ItemModel_Weapon[0][m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}		
		break;
	case _XGI_FC_CLOTHES:
	case _XGI_FC_CLOTHES2:
	case _XGI_FC_CLOTHES3:
		{
			switch( m_ContributionSecondType ) 
			{
			case _XGI_SC_CLOTHES_UPPER:
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_UserModel_UBodyType[m_ContributionModelIndex]->OBB_Center.x,
						   -g_UserModel_UBodyType[m_ContributionModelIndex]->OBB_Center.y,
						   -g_UserModel_UBodyType[m_ContributionModelIndex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_UBodyType[m_ContributionModelIndex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_UBodyType[m_ContributionModelIndex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_UBodyType[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_LOWER:
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_UserModel_LBodyType[m_ContributionModelIndex]->OBB_Center.x,
						   -g_UserModel_LBodyType[m_ContributionModelIndex]->OBB_Center.y,
						   -g_UserModel_LBodyType[m_ContributionModelIndex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_LBodyType[m_ContributionModelIndex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_LBodyType[m_ContributionModelIndex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_LBodyType[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_GLOVES:
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						   -g_UserModel_LHandType[m_ContributionModelIndex]->OBB_Center.x,
						   -g_UserModel_LHandType[m_ContributionModelIndex]->OBB_Center.y,
						   -g_UserModel_LHandType[m_ContributionModelIndex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_LHandType[m_ContributionModelIndex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = 0.0f;
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_LHandType[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_SHOES:
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						-g_UserModel_LFootType[m_ContributionModelIndex]->OBB_Center.x,
						-g_UserModel_LFootType[m_ContributionModelIndex]->OBB_Center.y,
						-g_UserModel_LFootType[m_ContributionModelIndex]->OBB_Center.z	);
					scalefactor = 0.28f / g_UserModel_LFootType[m_ContributionModelIndex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = scalefactor*g_UserModel_LFootType[m_ContributionModelIndex]->OBB_Center.x;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_LFootType[m_ContributionModelIndex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_LFootType[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);					
				}
				break;
			case _XGI_SC_CLOTHES_CAP:
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						-g_UserModel_Cap[m_ContributionModelIndex]->OBB_Center.x,
						-g_UserModel_Cap[m_ContributionModelIndex]->OBB_Center.y,
						-g_UserModel_Cap[m_ContributionModelIndex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_Cap[m_ContributionModelIndex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_Cap[m_ContributionModelIndex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_Cap[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;			
			case _XGI_SC_CLOTHES_BACKPACK:
				{
					D3DXMatrixTranslation( &m_RHWMatrix, 
						-g_UserModel_BackPack[m_ContributionModelIndex]->OBB_Center.x,
						-g_UserModel_BackPack[m_ContributionModelIndex]->OBB_Center.y,
						-g_UserModel_BackPack[m_ContributionModelIndex]->OBB_Center.z	);
					scalefactor = 0.4f / g_UserModel_BackPack[m_ContributionModelIndex]->m_ObjectRadius; 
										
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_BackPack[m_ContributionModelIndex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_BackPack[m_ContributionModelIndex]->RenderInterfaceItem( &m_RHWMatrix );
				}
				break;
			case _XGI_SC_CLOTHES_MASK :
				{					
					D3DXMatrixTranslation( &m_RHWMatrix, 
						-g_UserModel_Mask[m_ContributionModelIndex]->OBB_Center.x,
						-g_UserModel_Mask[m_ContributionModelIndex]->OBB_Center.y,
						-g_UserModel_Mask[m_ContributionModelIndex]->OBB_Center.z	);
					scalefactor = 0.4f / g_UserModel_Mask[m_ContributionModelIndex]->m_ObjectRadius;
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -(scalefactor*g_UserModel_Mask[m_ContributionModelIndex]->OBB_Center.y);
					m_RHWMatrix._43 = 0.0f;
					
					g_UserModel_Mask[m_ContributionModelIndex]->RenderInterfaceItem( &m_RHWMatrix );
				}
				break;
			}			
		}		
		break;
	case _XGI_FC_BOOK :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
				-g_ItemModel_MartialBook[m_ContributionModelIndex]->OBB_Center.x,
				-g_ItemModel_MartialBook[m_ContributionModelIndex]->OBB_Center.y,
				-g_ItemModel_MartialBook[m_ContributionModelIndex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_MartialBook[m_ContributionModelIndex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_MartialBook[m_ContributionModelIndex]->OBB_Center.y);;
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_MartialBook[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_POTION:
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
				-g_ItemModel_Potion[m_ContributionModelIndex]->OBB_Center.x,
				-g_ItemModel_Potion[m_ContributionModelIndex]->OBB_Center.y,
				-g_ItemModel_Potion[m_ContributionModelIndex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_Potion[m_ContributionModelIndex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_Potion[m_ContributionModelIndex]->OBB_Center.y);;
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_Potion[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
				-g_ItemModel_ETC[m_ContributionModelIndex]->OBB_Center.x,
				-g_ItemModel_ETC[m_ContributionModelIndex]->OBB_Center.y,
				-g_ItemModel_ETC[m_ContributionModelIndex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_ETC[m_ContributionModelIndex]->m_ObjectRadius;
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_ETC[m_ContributionModelIndex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_ETC[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.x,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_JewelBox[m_ContributionModelIndex]->m_ObjectRadius;

			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y);;
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_MONEY :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.x,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_JewelBox[m_ContributionModelIndex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y);;
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.x,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_JewelBox[m_ContributionModelIndex]->m_ObjectRadius;
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;			
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_LIFE :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.x,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.z	);
			
			scalefactor = 0.5f / g_ItemModel_JewelBox[m_ContributionModelIndex]->m_ObjectRadius;
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_SOCKET :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.x,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.z	);
			
			scalefactor = 0.5f / g_ItemModel_JewelBox[m_ContributionModelIndex]->m_ObjectRadius;
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
				-g_ItemModel_Elixir[m_ContributionModelIndex]->OBB_Center.x,
				-g_ItemModel_Elixir[m_ContributionModelIndex]->OBB_Center.y,
				-g_ItemModel_Elixir[m_ContributionModelIndex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_Elixir[m_ContributionModelIndex]->m_ObjectRadius;
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;			
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_Elixir[m_ContributionModelIndex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_Elixir[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_BOX :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.x,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_JewelBox[m_ContributionModelIndex]->m_ObjectRadius;
		
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;			
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			D3DXMatrixTranslation( &m_RHWMatrix, 
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.x,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y,
				-g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_JewelBox[m_ContributionModelIndex]->m_ObjectRadius;
		
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = 0.0;			
			m_RHWMatrix._42 = -(scalefactor*g_ItemModel_JewelBox[m_ContributionModelIndex]->OBB_Center.y);
			m_RHWMatrix._43 = 0;
			
			g_ItemModel_JewelBox[m_ContributionModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	}	

	// View Matrix, Projection Matrix, Viewport 다시 되돌린다. ====
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetProjMatrix());
	gpDev->SetViewport(&g_MainViewPort);
}

void _XWindow_Contribution::MoveWindow( int X, int Y )
{	
	_XWindow::MoveWindow( X, Y );
	m_ModelViewPort.X = (DWORD)(m_WindowPosition.x + 36);
	m_ModelViewPort.Y = (DWORD)(m_WindowPosition.y + 54);
}

void _XWindow_Contribution::ShowWindow(BOOL show)
{
	if(show)
	{
		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;

			if( g_pDefaultTooltip_Window)
				g_pDefaultTooltip_Window->SetContributionWinddowPtr( this );
		}
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

#ifdef _XTS_ITEM_OPTIMIZATION
void _XWindow_Contribution::SetAddData()
{
	switch(GetEqualItemType(m_ContributionType) ) 
	{
	case _XGI_FC_WEAPON:
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_ContributionType);
			if( WeaponItem )
			{
				float TempRet = (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].Get_m_usCurDur() / (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].Get_m_usMaxDur();
				m_CurrentItemContribution = static_cast<int>( TempRet * static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
				m_ContributionModelIndex = WeaponItem[m_ContributionId].sModelIndex;												
				m_ContributionThirdType = WeaponItem[m_ContributionId].cThirdType;
			}			
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_ContributionType);
			if( ClothesItem )
			{
				float TempRet = (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].Get_m_usCurDur() / (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].Get_m_usMaxDur();
				m_CurrentItemContribution = static_cast<int>( TempRet * static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
				m_ContributionModelIndex = ClothesItem[m_ContributionId].sModelIndex;
				if( ClothesItem[m_ContributionId].cSexType ) 
					m_ContributionModelIndex+= g_Model_GenderSeparateIndex;
			}			
		}		
		break;
	case _XGI_FC_BOOK :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_BookItemProperty[m_ContributionId].sModelIndex;						
		}
		break;
	case _XGI_FC_POTION:
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_PotionItemProperty[m_ContributionId].sModelIndex;
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );			
			//m_ContributionModelIndex = g_ConsumingItemProperty[m_ContributionId].sModelIndex;
			m_ContributionModelIndex = 2;
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );			
			m_ContributionModelIndex = g_ElixirItemProperty[m_ContributionId].sModelIndex;
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_ResourceItemProperty[m_ContributionId].sModelIndex;
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = 0;
		}
		break;
	case _XGI_FC_LIFE :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = 2; //g_LifeItemProperty[m_ContributionId].sModelIndex; <-- 차후에 모델 인덱스로 바꿀 것.
		}
		break;
	case _XGI_FC_MONEY :
		{
			m_CurrentItemContribution = m_ContributionItemCount / 5;
			m_ContributionModelIndex = 2;
		}
		break;
	case _XGI_FC_SOCKET :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );			
			m_ContributionModelIndex = 2;
		}
		break;
	case _XGI_FC_BOX :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );			
			m_ContributionModelIndex = g_BoxItemProperty[m_ContributionId].sModelIndex;
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_BoxKeyItemProperty[m_ContributionId].sModelIndex;
		}
		break;
	default:
		m_CurrentItemContribution = 0;
		break;
	}
}
#else
void _XWindow_Contribution::SetAddData()
{
	switch(m_ContributionType) 
	{
	case _XGI_FC_WEAPON:
		{
			float TempRet = (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].m_usCurDur / (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].m_usMaxDur;			
			m_CurrentItemContribution = static_cast<int>( TempRet * static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_WeaponItemProperty[m_ContributionId].sModelIndex;												
			m_ContributionThirdType = g_WeaponItemProperty[m_ContributionId].cThirdType;
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			float TempRet = (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].m_usCurDur / (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].m_usMaxDur;
			m_CurrentItemContribution = static_cast<int>( TempRet * static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_ClothesItemProperty[m_ContributionId].sModelIndex;
			if( g_ClothesItemProperty[m_ContributionId].cSexType ) 
				m_ContributionModelIndex+= g_Model_GenderSeparateIndex;
		}		
		break;
	case _XGI_FC_WEAPON2:
		{
			float TempRet = (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].m_usCurDur / (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].m_usMaxDur;
			m_CurrentItemContribution = static_cast<int>( TempRet * static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_WeaponItemProperty2[m_ContributionId].sModelIndex;												
			m_ContributionThirdType = g_WeaponItemProperty2[m_ContributionId].cThirdType;
		}		
		break;
	case _XGI_FC_CLOTHES2:
		{
			float TempRet = (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].m_usCurDur / (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].m_usMaxDur;
			m_CurrentItemContribution = static_cast<int>( TempRet * static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_ClothesItemProperty2[m_ContributionId].sModelIndex;
			if( g_ClothesItemProperty2[m_ContributionId].cSexType ) 
				m_ContributionModelIndex+= g_Model_GenderSeparateIndex;
		}		
		break;
	case _XGI_FC_BOOK :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_BookItemProperty[m_ContributionId].sModelIndex;						
		}
		break;
	case _XGI_FC_POTION:
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_PotionItemProperty[m_ContributionId].sModelIndex;
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );			
			//m_ContributionModelIndex = g_ConsumingItemProperty[m_ContributionId].sModelIndex;
			m_ContributionModelIndex = 2;
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );			
			m_ContributionModelIndex = g_ElixirItemProperty[m_ContributionId].sModelIndex;
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_ResourceItemProperty[m_ContributionId].sModelIndex;
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = 0;
		}
		break;
	case _XGI_FC_LIFE :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = 2; //g_LifeItemProperty[m_ContributionId].sModelIndex; <-- 차후에 모델 인덱스로 바꿀 것.
		}
		break;
	case _XGI_FC_MONEY :
		{
			m_CurrentItemContribution = m_ContributionItemCount / 5;
			m_ContributionModelIndex = 2;
		}
		break;
	case _XGI_FC_SOCKET :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );			
			m_ContributionModelIndex = 2;
		}
		break;
	case _XGI_FC_BOX :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );			
			m_ContributionModelIndex = g_BoxItemProperty[m_ContributionId].sModelIndex;
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			m_CurrentItemContribution = static_cast<int>( static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_BoxKeyItemProperty[m_ContributionId].sModelIndex;
		}
		break;
	case _XGI_FC_WEAPON3:
		{
			float TempRet = (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].m_usCurDur / (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].m_usMaxDur;
			m_CurrentItemContribution = static_cast<int>( TempRet * static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_WeaponItemProperty3[m_ContributionId].sModelIndex;												
			m_ContributionThirdType = g_WeaponItemProperty3[m_ContributionId].cThirdType;
		}		
		break;
	case _XGI_FC_CLOTHES3:
		{
			float TempRet = (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].m_usCurDur / (float)g_pLocalUser->m_UserItemList[m_ContributionIvenIndex].m_usMaxDur;
			m_CurrentItemContribution = static_cast<int>( TempRet * static_cast<float>(CheckClanPoint(m_ContributionType, m_ContributionId) * m_ContributionItemCount) );
			m_ContributionModelIndex = g_ClothesItemProperty3[m_ContributionId].sModelIndex;
			if( g_ClothesItemProperty3[m_ContributionId].cSexType ) 
				m_ContributionModelIndex+= g_Model_GenderSeparateIndex;
		}		
		break;
	default:
		m_CurrentItemContribution = 0;
		break;
	}
}
#endif

int _XWindow_Contribution::CheckClanPoint(int type, int id)
{
	if( id <= 0 )	return 0;

#ifdef _XTS_ITEM_OPTIMIZATION
	char itemtype = GetEqualItemType(type);
	if( itemtype != -1 )
	{
		switch(itemtype) 
		{	
		case _XGI_FC_WEAPON:
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(type);				
				if( WeaponItem )
				{
					if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
					{
						return static_cast<float>(WeaponItem[id].sClanPoint1) ;
					}			
					else					// cp2 적용
					{
						return static_cast<float>(WeaponItem[id].sClanPoint2) ;
					}
				}				
			}		
			break;
		case _XGI_FC_CLOTHES:
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(type);
				if( ClothesItem )
				{
					if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
					{
						return static_cast<float>(ClothesItem[id].sClanPoint1) ;
					}			
					else					// cp2 적용
					{
						return static_cast<float>(ClothesItem[id].sClanPoint2) ;
					}					
				}				
			}		
			break;		
		case _XGI_FC_BOOK :
			{
				_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(type);
				if( BookItem )
				{
					if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
					{
						return static_cast<float>(BookItem[id].sClanPoint1) ;
					}			
					else					// cp2 적용
					{
						return static_cast<float>(BookItem[id].sClanPoint2) ;
					}
				}				
			}
			break;
		case _XGI_FC_POTION:
			{
				_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(type);
				if( PotionItem )
				{
					if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
					{
						return static_cast<float>(PotionItem[id].sClanPoint1) ;
					}			
					else					// cp2 적용
					{
						return static_cast<float>(PotionItem[id].sClanPoint2) ;
					}
				}				
			}
			break;
		case _XGI_FC_CONSUMING :
			{
				_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(type);
				if( ConsumingItem )
				{
					if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
					{
						return static_cast<float>(ConsumingItem[id].sClanPoint1) ;
					}			
					else					// cp2 적용
					{
						return static_cast<float>(ConsumingItem[id].sClanPoint2) ;
					}
				}				
			}
			break;
		case _XGI_FC_ELIXIR :
			{
				_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(type);
				if( ElixirItem )
				{
					if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
					{
						return static_cast<float>(ElixirItem[id].sClanPoint1) ;
					}			
					else					// cp2 적용
					{
						return static_cast<float>(ElixirItem[id].sClanPoint2) ;
					}
				}				
			}
			break;
		case _XGI_FC_RESOURCE :
			{
				_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(type);				
				if( ResourceItem )
				{
					if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
					{
						return static_cast<float>(ResourceItem[id].sClanPoint1) ;
					}			
					else					// cp2 적용
					{
						return static_cast<float>(ResourceItem[id].sClanPoint2) ;
					}					
				}				
			}
			break;
		case _XGI_FC_ACCESSORY :
			{
				_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(type);
				if( AccessoryItem )
				{
					if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
					{
						return static_cast<float>(AccessoryItem[id].sClanPoint1) ;
					}			
					else					// cp2 적용
					{
						return static_cast<float>(AccessoryItem[id].sClanPoint2) ;
					}
				}
			}
			break;
		case _XGI_FC_LIFE :
			{
				_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(type);
				if( LifeItem )
				{
					if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
					{
						return static_cast<float>(LifeItem[id].sClanPoint1) ;
					}			
					else					// cp2 적용
					{
						return static_cast<float>(LifeItem[id].sClanPoint2) ;
					}
				}				
			}
			break;	
		case _XGI_FC_SOCKET :
			{
				_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(type);
				if( SocketItem )
				{
					if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
					{
						return static_cast<float>(SocketItem[id].sClanPoint1) ;
					}			
					else					// cp2 적용
					{
						return static_cast<float>(SocketItem[id].sClanPoint2) ;
					}
				}				
			}
			break;
		case _XGI_FC_BOX :
			{
				_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(type);
				if( BoxItem )
				{
					if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
					{
						return static_cast<float>(BoxItem[id].sClanPoint1) ;
					}			
					else					// cp2 적용
					{
						return static_cast<float>(BoxItem[id].sClanPoint2) ;
					}
				}				
			}
			break;
		case _XGI_FC_BOXKEY :
			{
				_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(type);
				if( BoxkeyItem )
				{
					if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
					{
						return static_cast<float>(BoxkeyItem[id].sClanPoint1) ;
					}			
					else					// cp2 적용
					{
						return static_cast<float>(BoxkeyItem[id].sClanPoint2) ;
					}
				}				
			}
			break;
		}
	}
#else
	switch(type) 
	{	
	case _XGI_FC_WEAPON:
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_WeaponItemProperty[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_WeaponItemProperty[id].sClanPoint2) ;
			}
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_ClothesItemProperty[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_ClothesItemProperty[id].sClanPoint2) ;
			}
		}		
		break;
	case _XGI_FC_WEAPON2:
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_WeaponItemProperty2[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_WeaponItemProperty2[id].sClanPoint2) ;
			}
		}		
		break;
	case _XGI_FC_CLOTHES2:
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_ClothesItemProperty2[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_ClothesItemProperty2[id].sClanPoint2) ;
			}
		}		
		break;
	case _XGI_FC_WEAPON3:
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_WeaponItemProperty3[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_WeaponItemProperty3[id].sClanPoint2) ;
			}
		}		
		break;
	case _XGI_FC_CLOTHES3:
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_ClothesItemProperty3[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_ClothesItemProperty3[id].sClanPoint2) ;
			}
		}		
		break;
	case _XGI_FC_BOOK :
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_BookItemProperty[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_BookItemProperty[id].sClanPoint2) ;
			}
		}
		break;
	case _XGI_FC_POTION:
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_PotionItemProperty[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_PotionItemProperty[id].sClanPoint2) ;
			}
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_ConsumingItemProperty[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_ConsumingItemProperty[id].sClanPoint2) ;
			}
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_ElixirItemProperty[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_ElixirItemProperty[id].sClanPoint2) ;
			}
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_ResourceItemProperty[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_ResourceItemProperty[id].sClanPoint2) ;
			}
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_AccessoryItemProperty[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_AccessoryItemProperty[id].sClanPoint2) ;
			}
		}
		break;
	case _XGI_FC_LIFE :
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_LifeItemProperty[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_LifeItemProperty[id].sClanPoint2) ;
			}
		}
		break;	
	case _XGI_FC_SOCKET :
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_SocketItemProperty[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_SocketItemProperty[id].sClanPoint2) ;
			}
		}
		break;
	case _XGI_FC_BOX :
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_BoxItemProperty[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_BoxItemProperty[id].sClanPoint2) ;
			}
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			if( _XGameItem::CheckClan( type, id ) )	// cp1 적용
			{
				return static_cast<float>(g_BoxKeyItemProperty[id].sClanPoint1) ;
			}			
			else					// cp2 적용
			{
				return static_cast<float>(g_BoxKeyItemProperty[id].sClanPoint2) ;
			}
		}
		break;
	}
#endif
	return 0;
}

int	_XWindow_Contribution::CalcContribution(int invennumber)
{
	float TempRet = 1.0f;
	if( g_pLocalUser->m_UserItemList[invennumber].Get_m_usMaxDur() > 0 )
	{
		TempRet = (float)g_pLocalUser->m_UserItemList[invennumber].Get_m_usCurDur() / (float)g_pLocalUser->m_UserItemList[invennumber].Get_m_usMaxDur();
	}	
	return static_cast<int>( TempRet * static_cast<float>(CheckClanPoint(g_pLocalUser->m_UserItemList[invennumber].Get_m_cType(), g_pLocalUser->m_UserItemList[invennumber].Get_m_sID()) * g_pLocalUser->m_UserItemList[invennumber].Get_m_ucCount()) );
}



/*
TempRet = ItemCurDur / ItemMaxDur;

pCGResult->bIsClanPoint = 1; 

if( cApplyClan == 20 )      // 20 공통 문파. 
{
	contribution_add( static_cast<int>(  TempRet * cp[0] ) * pCGReq->ucCount );
	pCGResult->uiLeftMoney = static_cast<u_int>( get_contribution() );
}*/