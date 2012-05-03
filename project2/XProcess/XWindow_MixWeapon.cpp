// XWindow_MixWeapon.cpp: implementation of the _XWindow_MixWeapon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"
#include "XWindow_MixWeapon.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MixWeapon::_XWindow_MixWeapon()
{

}

_XWindow_MixWeapon::~_XWindow_MixWeapon()
{

}

BOOL _XWindow_MixWeapon::Initialize()
{
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 184, 71, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" ) );	
	pTitleBar->SetClipRect( 0, 125, 183, 195 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pBackBoard = new _XImageStatic;
	pBackBoard->Create( 0, 60, 184, 271, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ) );	
	pBackBoard->SetClipRect( 0, 1, 183, 211 );
	InsertChildObject( pBackBoard );

	// 버튼들 ---------------------------------------------------------------------------------------------------------=
	int applybtnindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XBTN_STRUCTURE btnstruct = 
	{ 
		TRUE, 
		{ 49, 242  }, 
		{ 60, 24 }, 
		_XDEF_MIXWEAPON_OK, 
		applybtnindex, 
		applybtnindex, 
		applybtnindex, 
		&g_MainInterfaceTextureArchive 
	};
	m_pOKButton = new _XButton;
	m_pOKButton->Create( btnstruct );	
	m_pOKButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pOKButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_OK);		// _T("확인")
	m_pOKButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 187, 152, 247, 176 );
	m_pOKButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
	m_pOKButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	m_pOKButton->EnableWindow(FALSE);
	InsertChildObject( m_pOKButton );	


	btnstruct.position.x = 117;
	btnstruct.commandid = _XDEF_MIXWEAPON_CANCEL;
	m_pCancelButton = new _XButton;
	m_pCancelButton->Create( btnstruct );	
	m_pCancelButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pCancelButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE);		// _T("취소")
	m_pCancelButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 187, 152, 247, 176 );
	m_pCancelButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  187, 127, 247, 151 );
	m_pCancelButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	m_pCancelButton->ShowWindow(TRUE);
	InsertChildObject( m_pCancelButton );

	m_SlotPosition[0].x = 12;
	m_SlotPosition[0].y = 135;

	m_SlotPosition[1].x = 12;
	m_SlotPosition[1].y = 171;

	m_SlotPosition[2].x = 12;
	m_SlotPosition[2].y = 206;


	_XImageStatic* pSlotWeapon = new _XImageStatic;
	pSlotWeapon->Create( m_SlotPosition[0].x, m_SlotPosition[0].y, m_SlotPosition[0].x+32, m_SlotPosition[0].y+32, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" ) );
	pSlotWeapon->SetClipRect( 214, 31, 246, 63 );
	InsertChildObject( pSlotWeapon );

	_XImageStatic* pSlotFirstItem = new _XImageStatic;
	pSlotFirstItem->Create( m_SlotPosition[1].x, m_SlotPosition[1].y, m_SlotPosition[1].x+32, m_SlotPosition[1].y+32, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" ) );
	pSlotFirstItem->SetClipRect( 214, 31, 246, 63 );
	InsertChildObject( pSlotFirstItem );

	_XImageStatic* pSlotSecondItem = new _XImageStatic;
	pSlotSecondItem->Create( m_SlotPosition[2].x, m_SlotPosition[2].y, m_SlotPosition[2].x+32, m_SlotPosition[2].y+32, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" ) );
	pSlotSecondItem->SetClipRect( 214, 31, 246, 63 );
	InsertChildObject( pSlotSecondItem );
 
#ifdef _XDEF_MIXWEAPON_071009_KUKURI	
	LoadCostScript();
#endif

	return TRUE;
}

void _XWindow_MixWeapon::DestroyWindow()
{
#ifdef _XDEF_MIXWEAPON_071009_KUKURI
	_XVecMixCost::iterator iter_cost;
	for(iter_cost = m_vecMixCost.begin() ; iter_cost != m_vecMixCost.end() ; ++iter_cost)
	{
		_XMixCost* pCost = *iter_cost;
		if(pCost)
		{
			SAFE_DELETE(pCost);
		}
	}
	m_vecMixCost.clear();
#endif
	_XWindow::DestroyWindow();
}

void _XWindow_MixWeapon::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

    int nDrawY;

	// TODO
	_XDrawSolidBar( m_WindowPosition.x+11, m_WindowPosition.y+28, m_WindowPosition.x+172, m_WindowPosition.y+126, 0xFF1A1819 );
	_XDrawRectAngle( m_WindowPosition.x+11, m_WindowPosition.y+28, m_WindowPosition.x+172, m_WindowPosition.y+126, 0.0f, 0xFFC6BAA2 );
	_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+27, m_WindowPosition.x+173, m_WindowPosition.y+127, 0.0f, 0xFF000000 );

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 251, 250, 247));

#ifdef _XDEF_MIXWEAPON_PERIODDELETE_080509_MAGVIPER //Author : 양희왕  //Author : 양희왕 //breif : 국내는 기간을 없애야 함

    nDrawY = 45;

    if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE ) //일본꺼 금환결 은환결은 기간을 찍어 준다
    {
        if( (m_ItemSlot[1].cType == 10 && m_ItemSlot[1].sID == 41) ||// 금환결 
            (m_ItemSlot[1].cType == 10 && m_ItemSlot[1].sID == 40) )//은환결 
        {
            nDrawY = 37;
            g_XBaseFont->Puts(m_WindowPosition.x+19, m_WindowPosition.y+nDrawY, _XGETINTERFACETEXT(ID_STRING_NEW_3714));	//기간:
            if(m_ItemSlot[1].sID > 0)
	        {
		        if(CheckCashItem(m_ItemSlot[1].cType, m_ItemSlot[1].sID) == 1)
			        g_XBaseFont->Puts(m_WindowPosition.x+55, m_WindowPosition.y+nDrawY, _XGETINTERFACETEXT(ID_STRING_NEW_3720));	//무제한
		        else
			        g_XBaseFont->Print(m_WindowPosition.x+55, m_WindowPosition.y+nDrawY, 1.0f, _XGETINTERFACETEXT(ID_STRING_NEW_3721), 30);	//%d일
                
                nDrawY += 18;
	        }
        }
    }

	g_XBaseFont->Puts(m_WindowPosition.x+19, m_WindowPosition.y+nDrawY, _XGETINTERFACETEXT(ID_STRING_NEW_3715));	//소켓수:
    if(m_ItemSlot[1].sID > 0)
    {
        g_XBaseFont->Print(m_WindowPosition.x+85, m_WindowPosition.y+nDrawY, 1.0, _XGETINTERFACETEXT(ID_STRING_QUEST_2615), 
			_XGameItem::GetThird(m_ItemSlot[1].cType, m_ItemSlot[1].sID));//%d개
		g_XBaseFont->Puts(m_WindowPosition.x+53, m_WindowPosition.y+174, _XGameItem::GetItemName(m_ItemSlot[1].cType, m_ItemSlot[1].sID));
    }
    else
        g_XBaseFont->Puts(m_WindowPosition.x+53, m_WindowPosition.y+174, _XGETINTERFACETEXT(ID_STRING_NEW_3718));	//첫번째 조합 아이템

    nDrawY += 18;
	g_XBaseFont->Puts(m_WindowPosition.x+19, m_WindowPosition.y+nDrawY, _XGETINTERFACETEXT(ID_STRING_NEW_3716));	//착용제한감소:
    if(m_ItemSlot[2].sID > 0)
	{
		g_XBaseFont->Print(m_WindowPosition.x+112, m_WindowPosition.y+nDrawY, 1.0, "%d", 
			_XGameItem::GetThird(m_ItemSlot[2].cType, m_ItemSlot[2].sID));
		g_XBaseFont->Puts(m_WindowPosition.x+53, m_WindowPosition.y+209, _XGameItem::GetItemName(m_ItemSlot[2].cType, m_ItemSlot[2].sID));
	}
	else
	{
		g_XBaseFont->Puts(m_WindowPosition.x+53, m_WindowPosition.y+209, _XGETINTERFACETEXT(ID_STRING_NEW_3719));	//두번째 조합 아이템
	}
    
    nDrawY += 18;
    g_XBaseFont->Puts(m_WindowPosition.x+19, m_WindowPosition.y+nDrawY, _XGETINTERFACETEXT(ID_STRING_NEW_3433));
    if(m_ItemSlot[0].sID > 0)
	{
		g_XBaseFont->Puts(m_WindowPosition.x+55, m_WindowPosition.y+nDrawY, _XGameItem::GetItemName(m_ItemSlot[0].cType, m_ItemSlot[0].sID));
		g_XBaseFont->Puts(m_WindowPosition.x+53, m_WindowPosition.y+137, _XGameItem::GetItemName(m_ItemSlot[0].cType, m_ItemSlot[0].sID));
	}
	else
	{
		g_XBaseFont->Puts(m_WindowPosition.x+53, m_WindowPosition.y+137, _XGETINTERFACETEXT(ID_STRING_NEW_3717));	//조합할 무기
	}

    nDrawY += 18;
	g_XBaseFont->Print(m_WindowPosition.x+19, m_WindowPosition.y+nDrawY, 1.0f, _XGETINTERFACETEXT(ID_STRING_NEW_3446), m_SelectedWeaponCost);
#else
    nDrawY = 37;

    g_XBaseFont->Puts(m_WindowPosition.x+19, m_WindowPosition.y+nDrawY, _XGETINTERFACETEXT(ID_STRING_NEW_3714));	//기간:
    if(m_ItemSlot[1].sID > 0)
	{
		if(CheckCashItem(m_ItemSlot[1].cType, m_ItemSlot[1].sID) == 1)
			g_XBaseFont->Puts(m_WindowPosition.x+55, m_WindowPosition.y+nDrawY, _XGETINTERFACETEXT(ID_STRING_NEW_3720));	//무제한
		else
			g_XBaseFont->Print(m_WindowPosition.x+55, m_WindowPosition.y+nDrawY, 1.0f, _XGETINTERFACETEXT(ID_STRING_NEW_3721), 30);	//%d일
	}
    
    nDrawY += 18;
	g_XBaseFont->Puts(m_WindowPosition.x+19, m_WindowPosition.y+nDrawY, _XGETINTERFACETEXT(ID_STRING_NEW_3715));	//소켓수:
    if(m_ItemSlot[1].sID > 0)
    {
        g_XBaseFont->Print(m_WindowPosition.x+85, m_WindowPosition.y+nDrawY, 1.0, _XGETINTERFACETEXT(ID_STRING_QUEST_2615), 
			_XGameItem::GetThird(m_ItemSlot[1].cType, m_ItemSlot[1].sID));//%d개
		g_XBaseFont->Puts(m_WindowPosition.x+53, m_WindowPosition.y+174, _XGameItem::GetItemName(m_ItemSlot[1].cType, m_ItemSlot[1].sID));
    }
    else
        g_XBaseFont->Puts(m_WindowPosition.x+53, m_WindowPosition.y+174, _XGETINTERFACETEXT(ID_STRING_NEW_3718));	//첫번째 조합 아이템

    nDrawY += 18;
	g_XBaseFont->Puts(m_WindowPosition.x+19, m_WindowPosition.y+nDrawY, _XGETINTERFACETEXT(ID_STRING_NEW_3716));	//착용제한감소:
    if(m_ItemSlot[2].sID > 0)
	{
		g_XBaseFont->Print(m_WindowPosition.x+112, m_WindowPosition.y+nDrawY, 1.0, "%d", 
			_XGameItem::GetThird(m_ItemSlot[2].cType, m_ItemSlot[2].sID));
		g_XBaseFont->Puts(m_WindowPosition.x+53, m_WindowPosition.y+209, _XGameItem::GetItemName(m_ItemSlot[2].cType, m_ItemSlot[2].sID));
	}
	else
	{
		g_XBaseFont->Puts(m_WindowPosition.x+53, m_WindowPosition.y+209, _XGETINTERFACETEXT(ID_STRING_NEW_3719));	//두번째 조합 아이템
	}
    
    nDrawY += 18;
    g_XBaseFont->Puts(m_WindowPosition.x+19, m_WindowPosition.y+nDrawY, _XGETINTERFACETEXT(ID_STRING_NEW_3433));
    if(m_ItemSlot[0].sID > 0)
	{
		g_XBaseFont->Puts(m_WindowPosition.x+55, m_WindowPosition.y+nDrawY, _XGameItem::GetItemName(m_ItemSlot[0].cType, m_ItemSlot[0].sID));
		g_XBaseFont->Puts(m_WindowPosition.x+53, m_WindowPosition.y+137, _XGameItem::GetItemName(m_ItemSlot[0].cType, m_ItemSlot[0].sID));
	}
	else
	{
		g_XBaseFont->Puts(m_WindowPosition.x+53, m_WindowPosition.y+137, _XGETINTERFACETEXT(ID_STRING_NEW_3717));	//조합할 무기
	}

    nDrawY += 18;
	g_XBaseFont->Print(m_WindowPosition.x+19, m_WindowPosition.y+nDrawY, 1.0f, _XGETINTERFACETEXT(ID_STRING_NEW_3446), m_SelectedWeaponCost);
#endif

	g_XBaseFont->Flush();
	
	for(int i = 0 ; i < 3 ; ++i)
	{
		if( m_ItemSlot[i].nInvenNumber >= 0)
		{
			g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+m_SlotPosition[i].x+1, m_WindowPosition.y+m_SlotPosition[i].y+1, 
			&g_pLocalUser->m_UserItemList[m_ItemSlot[i].nInvenNumber], FALSE);
		}
	}
}

BOOL _XWindow_MixWeapon::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	// TODO
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[1])
			{
				if(CheckMousePosition())
					m_WindowMouseState = _XW_STATE_RIGHTCLICK;
			}
			if(mousestate->bButton[0])
			{
				// left click
				if(CheckMousePosition())
				{
					if(g_pInventory_Window)	// 인벤토리에서 드래그 했을 경우
					{
						if(g_pInventory_Window->GetShowStatus())
						{
							if(g_pInventory_Window->m_bHaveMouseSlot && g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)
							{
								if(CheckItem(g_pInventory_Window->m_MouseSlotItem.Get_m_cType(),g_pInventory_Window->m_MouseSlotItem.Get_m_sID(), g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber))
								{
									SetItemSlot(g_pInventory_Window->m_MouseSlotItem.Get_m_cType(), g_pInventory_Window->m_MouseSlotItem.Get_m_sID(), g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber);

									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_bDragState = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
								}
							}
						}
					}
				}
			}
		}

		
		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				for(int i = 0 ; i < 3 ; ++i)
				{
					if(scrnpos->x >= m_WindowPosition.x+m_SlotPosition[i].x && scrnpos->z >= m_WindowPosition.y+m_SlotPosition[i].y &&
						scrnpos->x <= m_WindowPosition.x+m_SlotPosition[i].x+32 && scrnpos->z <= m_WindowPosition.y+m_SlotPosition[i].y+32)
					{
						ResetItemSlot(i);
						m_WindowMouseState = _XW_STATE_NONE;
					}
				}
			}
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_MixWeapon::ShowWindow(BOOL show)
{
#ifdef _XDEF_MIXWEAPON_071009_KUKURI

	if(show)
	{
		// npc 쪽으로 옮길 것
		if(!g_pInventory_Window->m_pMixWeapon_Window)
			g_pInventory_Window->SetMixWeaponWindowPtr(this);

		if(g_pInventory_Window)
		{
			// inven lock 걸기
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;
		}
	}
	else
	{
		if(g_pInventory_Window)
		{
			// inven lock 해제
			if(g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = FALSE;
		}
	}

	ResetItemSlot();
#endif

	_XWindow::ShowWindow(show);
}

BOOL _XWindow_MixWeapon::CheckItem(int cType, int sID, int nInvenNumber)
{
	// 인벤 체크
	BOOL bValid = FALSE;
	if(g_pLocalUser->m_UserItemList[nInvenNumber].Get_m_cType() == cType &&
		g_pLocalUser->m_UserItemList[nInvenNumber].Get_m_sID() == sID)
	{
		if(g_pLocalUser->m_UserItemList[nInvenNumber].Get_m_ucCount() > 0)
		{
			bValid = TRUE;
		}
	}

	if(!bValid)
		return FALSE;

	switch(cType)
	{
	case _XGI_FC_WEAPON :
	case _XGI_FC_WEAPON2 :
	case _XGI_FC_WEAPON3 :
		{
			//Author : 양희왕 //breif : 제련된거 없앤다 Date : 08/07/31 패치
		#ifdef _XDEF_ALLOW_INCHANTITEM_MIXWEAPON_MAGVIPER_080731
		#else
			if(g_pLocalUser->m_UserItemList[nInvenNumber].Get_m_ucStrength() > 0)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3724), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//제련된 아이템은 조합할 수 없습니다.
				return FALSE;
			}
		#endif

			if(g_WeaponItemProperty[sID].sTempField5 == 1 || g_WeaponItemProperty[sID].sTempField5 == 3)
				return TRUE;
		}
		break;
	case _XGI_FC_LIFE :
		{
			if(g_LifeItemProperty[sID].cSecondType == _XGI_SC_LIFE_MIX_SOCKET_NORMAL ||
				g_LifeItemProperty[sID].cSecondType == _XGI_SC_LIFE_MIX_SOCKET_PERIOD ||
				g_LifeItemProperty[sID].cSecondType == _XGI_SC_LIFE_MIX_PENALTY)
			{
				return TRUE;
			}
		}
		break;
	}

	g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_ENTERBOSSROOM_2071), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	return FALSE;
}

void _XWindow_MixWeapon::SetItemSlot(int cType, int sID, int nInvenNumber)
{
	// 인벤 체크
	BOOL bValid = FALSE;
	if(g_pLocalUser->m_UserItemList[nInvenNumber].Get_m_cType() == cType &&
		g_pLocalUser->m_UserItemList[nInvenNumber].Get_m_sID() == sID)
	{
		if(g_pLocalUser->m_UserItemList[nInvenNumber].Get_m_ucCount() > 0)
		{
			bValid = TRUE;
		}
	}

	if(!bValid)
		return;

	switch(cType)
	{
	case _XGI_FC_WEAPON :
	case _XGI_FC_WEAPON2 :
	case _XGI_FC_WEAPON3 :
		{
			m_ItemSlot[0].cType = cType;
			m_ItemSlot[0].sID = sID;
			m_ItemSlot[0].nInvenNumber = nInvenNumber;

			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);
			if(WeaponItem)
			{
				int rank = WeaponItem[sID].cRank;

				_XVecMixCost::iterator iter_cost;
				for(iter_cost = m_vecMixCost.begin() ; iter_cost != m_vecMixCost.end() ; ++iter_cost)
				{
					_XMixCost* pCost = *iter_cost;
					if(pCost)
					{
						if(pCost->rank == rank)
						{
							m_SelectedWeaponCost = pCost->cost;
							break;
						}
					}
				}
			}
		}
		break;
	case _XGI_FC_LIFE :
		{
			if(g_LifeItemProperty[sID].cSecondType == _XGI_SC_LIFE_MIX_SOCKET_NORMAL ||
				g_LifeItemProperty[sID].cSecondType == _XGI_SC_LIFE_MIX_SOCKET_PERIOD)
			{
				m_ItemSlot[1].cType = cType;
				m_ItemSlot[1].sID = sID;
				m_ItemSlot[1].nInvenNumber = nInvenNumber;
			}
			else if(g_LifeItemProperty[sID].cSecondType == _XGI_SC_LIFE_MIX_PENALTY)
			{
				m_ItemSlot[2].cType = cType;
				m_ItemSlot[2].sID = sID;
				m_ItemSlot[2].nInvenNumber = nInvenNumber;
			}
		}
		break;
	}

	BOOL bComplete = FALSE;
	if(m_ItemSlot[0].sID > 0 && m_ItemSlot[1].sID > 0 && m_ItemSlot[2].sID > 0) 
	{
		bComplete = TRUE;
	}

	if(bComplete)
	{
		m_pOKButton->EnableWindow(TRUE);
	}
}

void _XWindow_MixWeapon::ResetItemSlot()
{
	for(int i = 0 ; i < 3 ; ++i)
	{
		m_ItemSlot[i].cType = 0;
		m_ItemSlot[i].sID = 0;
		m_ItemSlot[i].nInvenNumber = -1;
	}

	m_SelectedWeaponCost = 0;
	m_pOKButton->EnableWindow(FALSE);
}

void _XWindow_MixWeapon::ResetItemSlot(int index)
{
	if(index < 0 || index > 2)
		return;

	m_ItemSlot[index].cType = 0;
	m_ItemSlot[index].sID = 0;
	m_ItemSlot[index].nInvenNumber = -1;

	if(index == 0)
	{
		m_SelectedWeaponCost = 0;
	}
	
	m_pOKButton->EnableWindow(FALSE);
}

BOOL _XWindow_MixWeapon::LoadCostScript()
{
	// load cost script
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	
#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		_XDeleteFile(_T("weapon_combine_cost.txt"));
	}		
#endif
	
	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );
	
	FILE* fileptr = NULL;


	TCHAR scriptfilename[32];

	memset( scriptfilename, 0, sizeof(TCHAR) * 32 );
	strcpy( scriptfilename, _T("weapon_combine_cost.txt") );
	
#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( scriptfilename, "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;			
			fileptr = g_ScriptArchive.GetPackedFile( scriptfilename );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( scriptfilename );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( scriptfilename, "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( scriptfilename );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( scriptfilename );
	}
#endif
	
	if( !fileptr )
	{
		_XFatalError("Can't load Weapon_combine_cost");
		if( !bLoadFromFile )
			g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	TCHAR buffer[512];

	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(buffer));
		if(!fgets(buffer, 512, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;

		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			_XMixCost* pCost = new _XMixCost;
			sscanf(buffer, "%d %d", &pCost->rank, &pCost->cost);

			m_vecMixCost.push_back(pCost);
		}
	}while(!feof(fileptr));

	if( bLoadFromFile ) fclose( fileptr );
	else g_ScriptArchive.ClosePackage();
	
	return TRUE;
}