// XNetwork_item_trade.cpp : item과 trade 관련
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "SoundEffectList.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_Inventory.h"
#include "XWindow_Warehouse.h"
#include "XWindow_NPCScript.h"
#include "XWindow_RepairItem.h"
#include "XWindow_Warehouse.h"
#include "XWindow_AddSaveItem.h"
#include "XWindow_PCTrade.h"
#include "XWindow_NPCTrade.h"
#include "XWindow_SocketPlugIn.h"
#include "xwindow_chargegauge.h"
#include "XWindow_Contribution.h"
#include "XWindow_Pocket.h"
#include "XWindow_TutorialIcon.h"
#include "XWindow_Gamble.h"
#include "XWindow_Inchant.h"
#include "XMessageWindow.h"
#include "XGameItem.h"
#include "XParticleIDDefine.h"
#include "XWindowObjectDefine.h"
#include "XWindow_BloodPoint.h"
#include "XWindow_SmeltItem.h" //Author : 양희왕
#include "XWindow_SocketDetach.h" //Author : 양희왕
#include "Xwindow_MixWeapon.h"
#include "XWindow_Rebirth.h"

extern void ChangeModel(DWORD lparam, char firstType, char facemaskmode = 0 );

#ifdef _XTS_ITEM_OPTIMIZATION
BOOL _XNetwork::CheckMSGInvenInfo(unsigned char idx, char type, char second)
{	
	switch(idx) 
	{
	case _XINVENTORY_SLOTTYPE_AMOR :
		{
			if( ( _XGI_FC_CLOTHES == GetEqualItemType(type) ) && second == _XGI_SC_CLOTHES_UPPER )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_PANTS :
		{
			if( ( _XGI_FC_CLOTHES == GetEqualItemType(type) ) && second == _XGI_SC_CLOTHES_LOWER )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_ADAE :
		{
			if( (_XGI_FC_CLOTHES == GetEqualItemType(type) ) && second == _XGI_SC_CLOTHES_GLOVES )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_SHOES :
		{
			if( (_XGI_FC_CLOTHES == GetEqualItemType(type) ) && second == _XGI_SC_CLOTHES_SHOES )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_CAP :
		{
			if( (_XGI_FC_CLOTHES == GetEqualItemType(type) ) && second == _XGI_SC_CLOTHES_CAP )
				return TRUE;			
		}
		break;
	case _XINVENTORY_SLOTTYPE_GAP_UPPER :				// 없음	
	case _XINVENTORY_SLOTTYPE_GAP_LOWER :				// 없음
		{
			if( _XGI_FC_ACCESSORY == GetEqualItemType(type)  )
				return TRUE;			
		}
		break;
	case _XINVENTORY_SLOTTYPE_BACKPACK1 :
		{
			if( (_XGI_FC_CLOTHES == GetEqualItemType(type) ) && second == _XGI_SC_CLOTHES_BACKPACK )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_MASK :
		{
			if( (_XGI_FC_CLOTHES == GetEqualItemType(type) ) && second == _XGI_SC_CLOTHES_MASK )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_WEAPON_1 :
	case _XINVENTORY_SLOTTYPE_WEAPON_2 :
		{
			if( _XGI_FC_WEAPON == GetEqualItemType(type) )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_ACCESSORY_1 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_2 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_3 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_4 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_5 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_6 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_7 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_8 :
		{
			if( type == _XGI_FC_ACCESSORY )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_BACKPACK2 :
		{
			if( (_XGI_FC_CLOTHES == GetEqualItemType(type) ) && second == _XGI_SC_CLOTHES_BACKPACK )
				return TRUE;
		}
		break;	
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	case _XINVENTORY_SLOTTYPE_CASHBACKPACK1 :
		{
			if( (_XGI_FC_CLOTHES == GetEqualItemType(type) ) && second == _XGI_SC_CLOTHES_BACKPACK )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_CASHBACKPACK2 :
		{
			if( (_XGI_FC_CLOTHES == GetEqualItemType(type) ) && second == _XGI_SC_CLOTHES_BACKPACK )
				return TRUE;
		}
		break;
#endif
	}
	return FALSE;
}
#else
BOOL _XNetwork::CheckMSGInvenInfo(unsigned char idx, char type, char second)
{	
	switch(idx) 
	{
	case _XINVENTORY_SLOTTYPE_AMOR :
		{
			if( (type == _XGI_FC_CLOTHES || type == _XGI_FC_CLOTHES2 || type == _XGI_FC_CLOTHES3 ) && second == _XGI_SC_CLOTHES_UPPER )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_PANTS :
		{
			if( (type == _XGI_FC_CLOTHES || type == _XGI_FC_CLOTHES2 || type == _XGI_FC_CLOTHES3 ) && second == _XGI_SC_CLOTHES_LOWER )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_ADAE :
		{
			if( (type == _XGI_FC_CLOTHES || type == _XGI_FC_CLOTHES2 || type == _XGI_FC_CLOTHES3 ) && second == _XGI_SC_CLOTHES_GLOVES )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_SHOES :
		{
			if( (type == _XGI_FC_CLOTHES || type == _XGI_FC_CLOTHES2 || type == _XGI_FC_CLOTHES3 ) && second == _XGI_SC_CLOTHES_SHOES )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_CAP :
		{
			if( (type == _XGI_FC_CLOTHES || type == _XGI_FC_CLOTHES2 || type == _XGI_FC_CLOTHES3 ) && second == _XGI_SC_CLOTHES_CAP )
				return TRUE;			
		}
		break;
	case _XINVENTORY_SLOTTYPE_GAP_UPPER :				// 없음	
	case _XINVENTORY_SLOTTYPE_GAP_LOWER :				// 없음
		{
			if( _XGI_FC_ACCESSORY == type  )
				return TRUE;			
		}
		break;
	case _XINVENTORY_SLOTTYPE_BACKPACK1 :
		{
			if( (type == _XGI_FC_CLOTHES || type == _XGI_FC_CLOTHES2 || type == _XGI_FC_CLOTHES3 ) && second == _XGI_SC_CLOTHES_BACKPACK )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_MASK :
		{
			if( (type == _XGI_FC_CLOTHES || type == _XGI_FC_CLOTHES2 || type == _XGI_FC_CLOTHES3 ) && second == _XGI_SC_CLOTHES_MASK )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_WEAPON_1 :
		{
			if( type == _XGI_FC_WEAPON || type == _XGI_FC_WEAPON2 || type == _XGI_FC_WEAPON3 )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_WEAPON_2 :
		{
			if( type == _XGI_FC_WEAPON || type == _XGI_FC_WEAPON2 || type == _XGI_FC_WEAPON3 )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_ACCESSORY_1 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_2 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_3 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_4 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_5 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_6 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_7 :		
	case _XINVENTORY_SLOTTYPE_ACCESSORY_8 :
		{
			if( type == _XGI_FC_ACCESSORY )
				return TRUE;
		}
		break;
	case _XINVENTORY_SLOTTYPE_BACKPACK2 :
		{
			if( (type == _XGI_FC_CLOTHES || type == _XGI_FC_CLOTHES2 || type == _XGI_FC_CLOTHES3 ) && second == _XGI_SC_CLOTHES_BACKPACK )
				return TRUE;
		}
		break;	
	}
	return FALSE;
}
#endif

int _XNetwork::CheckEmptyFameItem(void)
{
	int count = 0;
	short id = 0;
	
	for( int i = _XINVENTORY_SLOTTYPE_AMOR; i < _XINVENTORY_SLOTTYPE_POCKETSTART; ++i )
	{
		id = g_pLocalUser->m_UserItemList[i].Get_m_sID();
		if( id > 0 )
		{
			switch(g_pLocalUser->m_UserItemList[i].Get_m_cType()) 
			{
			case _XGI_FC_WEAPON:
				{
					if( g_WeaponItemProperty[id].ucFame  > 0 )
						count++;
				}
				break;
			case _XGI_FC_WEAPON2:
				{
					if( g_WeaponItemProperty2[id].ucFame  > 0 )
						count++;
				}
				break;
			case _XGI_FC_WEAPON3:
				{
					if( g_WeaponItemProperty3[id].ucFame  > 0 )
						count++;
				}
				break;
			case _XGI_FC_CLOTHES:
				{
					if( g_ClothesItemProperty[id].ucFame > 0 )
						count++;
				}
				break;
			case _XGI_FC_CLOTHES2:
				{
					if( g_ClothesItemProperty2[id].ucFame > 0 )
						count++;
				}
				break;
			case _XGI_FC_CLOTHES3:
				{
					if( g_ClothesItemProperty3[id].ucFame > 0 )
						count++;
				}
				break;
			case _XGI_FC_ACCESSORY:
				{
					if( g_AccessoryItemProperty[id].ucFame > 0 )
						count++;
				}
				break;
			}
		}
	}

	if( count == 0 )
		g_pLocalUser->m_FameAttrib = -1;

	return count;
}

BOOL _XNetwork::CheckFameItem(char cType, short sID)
{
	if( sID <= 0 || sID > g_MaxItemCount[cType] )
		return FALSE;

	switch(cType) 
	{
	case _XGI_FC_WEAPON :
		{
			if( g_WeaponItemProperty[sID].ucFame > 0 )
			{
				if( g_pLocalUser->m_FameAttrib == -1 )
				{
					if( g_WeaponItemProperty[sID].ucFame > 100 )
						g_pLocalUser->m_FameAttrib = 1;	
					else
						g_pLocalUser->m_FameAttrib = 0;	
				}
				else
				{
					if( g_WeaponItemProperty[sID].ucFame > 100 )
					{
						if( g_pLocalUser->m_FameAttrib == 0 )
							return FALSE;
					}
					else
					{
						if( g_pLocalUser->m_FameAttrib == 1 )						
							return FALSE;
					}
				}							
			}			
		}
		break;
	case _XGI_FC_CLOTHES :
		{
			if( g_ClothesItemProperty[sID].ucFame > 0 )
			{
				if( g_pLocalUser->m_FameAttrib == -1 )
				{
					if( g_ClothesItemProperty[sID].ucFame > 100 )
						g_pLocalUser->m_FameAttrib = 1;	
					else
						g_pLocalUser->m_FameAttrib = 0;	
				}
				else
				{
					if( g_ClothesItemProperty[sID].ucFame > 100 )
					{
						if( g_pLocalUser->m_FameAttrib == 0 )
							return FALSE;
					}
					else
					{
						if( g_pLocalUser->m_FameAttrib == 1 )						
							return FALSE;
					}
				}							
			}
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			if( g_AccessoryItemProperty[sID].ucFame > 0 )
			{
				if( g_pLocalUser->m_FameAttrib == -1 )
				{
					if( g_AccessoryItemProperty[sID].ucFame > 100 )
						g_pLocalUser->m_FameAttrib = 1;	
					else
						g_pLocalUser->m_FameAttrib = 0;	
				}
				else
				{
					if( g_AccessoryItemProperty[sID].ucFame > 100 )
					{
						if( g_pLocalUser->m_FameAttrib == 0 )
							return FALSE;
					}
					else
					{
						if( g_pLocalUser->m_FameAttrib == 1 )						
							return FALSE;
					}
				}							
			}
		}
		break;
	case _XGI_FC_WEAPON2 :
		{
			if( g_WeaponItemProperty2[sID].ucFame > 0 )
			{
				if( g_pLocalUser->m_FameAttrib == -1 )
				{
					if( g_WeaponItemProperty2[sID].ucFame > 100 )
						g_pLocalUser->m_FameAttrib = 1;	
					else
						g_pLocalUser->m_FameAttrib = 0;	
				}
				else
				{
					if( g_WeaponItemProperty2[sID].ucFame > 100 )
					{
						if( g_pLocalUser->m_FameAttrib == 0 )
							return FALSE;
					}
					else
					{
						if( g_pLocalUser->m_FameAttrib == 1 )						
							return FALSE;
					}
				}							
			}
		}
		break;
	case _XGI_FC_CLOTHES2 :
		{
			if( g_ClothesItemProperty2[sID].ucFame > 0 )
			{
				if( g_pLocalUser->m_FameAttrib == -1 )
				{
					if( g_ClothesItemProperty2[sID].ucFame > 100 )
						g_pLocalUser->m_FameAttrib = 1;	
					else
						g_pLocalUser->m_FameAttrib = 0;	
				}
				else
				{
					if( g_ClothesItemProperty2[sID].ucFame > 100 )
					{
						if( g_pLocalUser->m_FameAttrib == 0 )
							return FALSE;
					}
					else
					{
						if( g_pLocalUser->m_FameAttrib == 1 )						
							return FALSE;
					}
				}							
			}
		}
		break;
	case _XGI_FC_WEAPON3 :
		{
			if( g_WeaponItemProperty3[sID].ucFame > 0 )
			{
				if( g_pLocalUser->m_FameAttrib == -1 )
				{
					if( g_WeaponItemProperty3[sID].ucFame > 100 )
						g_pLocalUser->m_FameAttrib = 1;	
					else
						g_pLocalUser->m_FameAttrib = 0;	
				}
				else
				{
					if( g_WeaponItemProperty3[sID].ucFame > 100 )
					{
						if( g_pLocalUser->m_FameAttrib == 0 )
							return FALSE;
					}
					else
					{
						if( g_pLocalUser->m_FameAttrib == 1 )						
							return FALSE;
					}
				}							
			}
		}
		break;
	case _XGI_FC_CLOTHES3 :
		{
			if( g_ClothesItemProperty3[sID].ucFame > 0 )
			{
				if( g_pLocalUser->m_FameAttrib == -1 )
				{
					if( g_ClothesItemProperty3[sID].ucFame > 100 )
						g_pLocalUser->m_FameAttrib = 1;	
					else
						g_pLocalUser->m_FameAttrib = 0;	
				}
				else
				{
					if( g_ClothesItemProperty3[sID].ucFame > 100 )
					{
						if( g_pLocalUser->m_FameAttrib == 0 )
							return FALSE;
					}
					else
					{
						if( g_pLocalUser->m_FameAttrib == 1 )						
							return FALSE;
					}
				}							
			}
		}
		break;
	}

	return TRUE;
}

void _XNetwork::MSGInvenInfo(MSG_INVEN_INFO* inventorylist)
{	
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	switch(inventorylist->ucInvenMode)
	{
	case en_inven_info_mode_elixir : //영약
		{
			ElixirInfoSetting(inventorylist);
		}
		break;
	case en_inven_info_mode_inventory : //인벤
		{
			InvenInfoSetting(inventorylist);
		}
		break;
	}
#else
	InvenInfoSetting(inventorylist);
	ElixirInfoSetting(inventorylist);
#endif
	g_pLocalUser->UpdateLocalUserAttackLevel();

	// 화타신단 체크를 위해서 위치 변경
	if(g_pLocalUser->m_CharacterInfo.Get_current_lifepower() < 0)
		g_pLocalUser->SetDieMode();
	
#ifdef _XDWDEBUG
	_XLog( "========================================================");
#endif

}

//Author : 양희왕 //breif : 캐시 행낭 때문에 밖으로 뺌
void _XNetwork::ElixirInfoSetting(MSG_INVEN_INFO* templist)
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭
	MSG_ELIXIR_DATA* inventorylist = (MSG_ELIXIR_DATA*)templist;
#else
	MSG_INVEN_INFO* inventorylist = templist;
#endif
	// 영약 데이타
	g_pLocalUser->m_ElixirInfo.Reset();
	
	for(int i = 0; i < 5; ++i )
	{
		g_pLocalUser->m_ElixirInfo.Data[i].__Grade = inventorylist->E_Data.Data[i].__Grade;
		for( int ii = 0; ii < 10; ++ii )
			g_pLocalUser->m_ElixirInfo.Data[i].__Data[ii] = inventorylist->E_Data.Data[i].__Data[ii];
	}
	g_pLocalUser->m_ElixirInfo._sIntoxication = inventorylist->E_Data._sIntoxication;
	for( i = 0; i < 3; ++i )
		g_pLocalUser->m_ElixirInfo.Temp[i] = inventorylist->E_Data.Temp[i];	

	g_pLocalUser->UpdateLocalUserAttackLevel();

	if( inventorylist->E_Data._sIntoxication > 100 )
	{
		g_pState_Window->InsertStateListFromOther(_XST_INTOXICATION, 2003);
	}
}
void _XNetwork::InvenInfoSetting(MSG_INVEN_INFO* templist)
{
    // inven basetabButton---------------------------------------------------------------------------------------------=
	g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 97,  247, 138 );
	g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  205, 97,  247, 138 );
	g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 97,  247, 138 );
	g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  205, 97,  247, 138 );

	// ----------------------------------------------------------------------------------------------------------------=
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭
    // inven basetabButton---------------------------------------------------------------------------------------------=
    for( int j = 1; j < 5; j++ )
    {
        g_pInventory_Window->m_BaseTabButton[j]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
        g_pInventory_Window->m_BaseTabButton[j]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  136, 0, 168, 32 );
    }
    g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;
    g_pInventory_Window->m_bIteminCashBackPack[0] = FALSE;
    g_pInventory_Window->m_bIteminCashBackPack[1] = FALSE;

	MSG_INVENTORY* inventorylist = (MSG_INVENTORY*)templist;
#else
	MSG_INVEN_INFO* inventorylist = templist;
#endif

#ifdef _XDWDEBUG
	_XLog( "========================================================");
	_XLog( "Total inven item count %d", inventorylist->ucItemCount );
	_XLog( "--------------------------------------------------------");
#endif

	g_pLocalUser->ResetInventoryInfo();
	g_pLocalUser->m_Money = inventorylist->uiMoney;

	g_pLocalUser->m_cAmorPocketNum = inventorylist->cAmorPocketNum;
	g_pLocalUser->m_cPantsPocketNum = inventorylist->cPantsPocketNum;
	g_pLocalUser->m_cBackPackExtend = inventorylist->cBackPackExtend;

	int  CheckFlag = FALSE;

	char* currentptr = inventorylist->cInvenPack;
	for(int i = 0 ; i < inventorylist->ucItemCount ; ++i)
	{
		_sUser_InvenItem* tempInvenItem = (_sUser_InvenItem*)currentptr;
		if( tempInvenItem )
		{
			if( tempInvenItem->cSlot >= 0 && tempInvenItem->cSlot < _XINVENTORY_SLOTTYPE_POCKETSTART )
			{
				// 제대로 장착되어 있는지 검사
				if( !CheckMSGInvenInfo( tempInvenItem->cSlot, tempInvenItem->cType, tempInvenItem->cSecond ) )
				{
					CheckFlag = TRUE;				
				}
#ifdef _XTS_FAME
				if( !CheckFameItem(tempInvenItem->cType, tempInvenItem->sID) )
				{
//					CheckFlag = TRUE;
				}
#endif
			}
			if( tempInvenItem->sID > 0 )
			{
			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭				
				if( tempInvenItem->cSlot >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART && tempInvenItem->cSlot < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
				{
                #ifdef _XDWDEBUG
					g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
					g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                #else
                    g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
					g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                    //g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
	                //g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
                #endif
					g_pInventory_Window->m_bIteminCashBackPack[0] = TRUE;
				}
				
				if( tempInvenItem->cSlot >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART && tempInvenItem->cSlot < _XINVENTORY_SLOTTYPE_TOTALCOUNT)
				{
                #ifdef _XDWDEBUG
					g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
					g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                #else
                    g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
					g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                    //g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
	                //g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
                #endif
					g_pInventory_Window->m_bIteminCashBackPack[1] = TRUE;
				}
			#endif
				int slotnumber = -1;
#ifdef _XTS_ITEM_OPTIMIZATION
				switch( GetEqualItemType(tempInvenItem->cType) ) 
#else
				switch(tempInvenItem->cType) 
#endif
				{
#ifdef _XTS_ITEM_OPTIMIZATION
				case _XGI_FC_WEAPON :
#else
				case _XGI_FC_WEAPON :
				case _XGI_FC_WEAPON2 :
				case _XGI_FC_WEAPON3 :
#endif
					{
						if( tempInvenItem->ucIncludeLimit == 1 )
						{
							_sUser_Inven_v1_all* tempInvenItem = (_sUser_Inven_v1_all*)currentptr;
							if( tempInvenItem )
							{
								currentptr += sizeof(_sUser_Inven_v1_all);
								slotnumber = tempInvenItem->cSlot;
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cType(tempInvenItem->cType);
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cSecond(tempInvenItem->cSecond);
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_sID( tempInvenItem->sID );
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cThird(_XGameItem::GetThird(tempInvenItem->cType, tempInvenItem->sID));  
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucCount(tempInvenItem->Item_Money.S_NormalItem.ucCount);
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_usCurDur( tempInvenItem->Item_Money.S_NormalItem.usCurDur );
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_usMaxDur( _XGameItem::GetMaxDurability(tempInvenItem->cType, tempInvenItem->sID) );

								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucStrength( tempInvenItem->ucStrength );
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
								g_pLocalUser->m_UserItemList[slotnumber].m_usCurProtect  = tempInvenItem->uselimit.usCurProtect; // 바른용린의수
								g_pLocalUser->m_UserItemList[slotnumber].m_usCurMaxProtect = tempInvenItem->uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
								g_pLocalUser->m_UserItemList[slotnumber].m_usUsedProtect = tempInvenItem->uselimit.usUsedProtect; // 용루 사용여부
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucWeaponStrength( _XGameItem::GetNature(tempInvenItem->cType, tempInvenItem->sID) );
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSharpness( tempInvenItem->ucSharpness );
#else
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSharpness(tempInvenItem->ucSharpness);
#endif
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSocketCount( tempInvenItem->Item_Money.S_NormalItem.ucSocketCount );
								if( tempInvenItem->Item_Money.S_NormalItem.ucSocketCount > 0 )
								{
									for( int i = 0; i < 4; ++i )
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_sSocket( i, tempInvenItem->sSocket[i] );
								}
									
								g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag1 = tempInvenItem->uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //Author : 양희왕 //breif : 디파인 되어있으면 들어오지 않는다
								g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag2	= tempInvenItem->uselimit.ucFlag2;	
								g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag3	= tempInvenItem->uselimit.ucFlag3;
#endif
								g_pLocalUser->m_UserItemList[slotnumber].m_usValue1	= tempInvenItem->uselimit.usValue1;

								if( tempInvenItem->uselimit.ucYear )
								{									
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucYear(tempInvenItem->uselimit.ucYear);	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucMonth(tempInvenItem->uselimit.ucMonth);	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucDay(tempInvenItem->uselimit.ucDay);	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucHour(tempInvenItem->uselimit.ucHour);	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucMinute(tempInvenItem->uselimit.ucMinute);
								}
							}							
						}											
						else if( tempInvenItem->ucIncludeLimit == 0 )
						{
							_sUser_Inven_v1_extension* tempInvenItem = (_sUser_Inven_v1_extension*)currentptr;
							if( tempInvenItem )
							{
								currentptr += sizeof(_sUser_Inven_v1_extension);
								slotnumber = tempInvenItem->cSlot;
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cType(tempInvenItem->cType);
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cSecond(tempInvenItem->cSecond);
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_sID( tempInvenItem->sID );
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cThird(_XGameItem::GetThird(tempInvenItem->cType, tempInvenItem->sID));  
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucCount(tempInvenItem->Item_Money.S_NormalItem.ucCount);
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_usCurDur( tempInvenItem->Item_Money.S_NormalItem.usCurDur );
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_usMaxDur( _XGameItem::GetMaxDurability(tempInvenItem->cType, tempInvenItem->sID) );
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSocketCount( tempInvenItem->Item_Money.S_NormalItem.ucSocketCount );
								if( tempInvenItem->Item_Money.S_NormalItem.ucSocketCount > 0 )
								{
									for( int i = 0; i < 4; ++i )
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_sSocket(i, tempInvenItem->sSocket[i] );
								}
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucStrength( tempInvenItem->ucStrength );
							
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕 //breif : 추가 하지 않았음
								//g_pLocalUser->m_UserItemList[slotnumber].m_ucCurProtect = tempInvenItem->ucCurProtect; // 바른용린의수
								//g_pLocalUser->m_UserItemList[slotnumber].m_ucIncProtect = tempInvenItem->ucIncProtect; // 최대강도 증가분
								//g_pLocalUser->m_UserItemList[slotnumber].m_ucUsedProtect = tempInvenItem->ucUsedProtect; // 용루 사용여부
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucWeaponStrength( _XGameItem::GetNature(tempInvenItem->cType, tempInvenItem->sID) );
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSharpness( tempInvenItem->ucSharpness );
#else
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSharpness(tempInvenItem->ucSharpness);
#endif
							}
						}
					}
					break;
#ifdef _XTS_ITEM_OPTIMIZATION
				case _XGI_FC_CLOTHES :
#else
				case _XGI_FC_CLOTHES :
				case _XGI_FC_CLOTHES2 :
				case _XGI_FC_CLOTHES3 :
#endif
					{
						if( tempInvenItem->ucIncludeLimit == 1 )
						{
							_sUser_Inven_v1_all* tempInvenItem = (_sUser_Inven_v1_all*)currentptr;
							if( tempInvenItem )
							{
								currentptr += sizeof(_sUser_Inven_v1_all);
								slotnumber = tempInvenItem->cSlot;
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cType(tempInvenItem->cType);
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cSecond(tempInvenItem->cSecond);
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_sID( tempInvenItem->sID );
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cThird(_XGameItem::GetThird(tempInvenItem->cType, tempInvenItem->sID));  
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucCount(tempInvenItem->Item_Money.S_NormalItem.ucCount);
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_usCurDur( tempInvenItem->Item_Money.S_NormalItem.usCurDur );
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_usMaxDur( _XGameItem::GetMaxDurability(tempInvenItem->cType, tempInvenItem->sID) );
								
								if( tempInvenItem->cSecond != _XGI_SC_CLOTHES_BACKPACK )
								{
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSocketCount( tempInvenItem->Item_Money.S_NormalItem.ucSocketCount );
									if( tempInvenItem->Item_Money.S_NormalItem.ucSocketCount > 0 )
									{
										for( int i = 0; i < 4; ++i )
											g_pLocalUser->m_UserItemList[slotnumber].Set_m_sSocket( i, tempInvenItem->sSocket[i] );
									}								
								}
								
								g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag1 = tempInvenItem->uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //Author : 양희왕 //breif : 추가 하지 않았음
								g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag2	= tempInvenItem->uselimit.ucFlag2;	
								g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag3	= tempInvenItem->uselimit.ucFlag3;
#endif
								g_pLocalUser->m_UserItemList[slotnumber].m_usValue1	= tempInvenItem->uselimit.usValue1;

								if( tempInvenItem->uselimit.ucYear )
								{	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucYear(tempInvenItem->uselimit.ucYear);	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucMonth( tempInvenItem->uselimit.ucMonth);	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucDay(tempInvenItem->uselimit.ucDay);	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucHour(tempInvenItem->uselimit.ucHour);	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucMinute(tempInvenItem->uselimit.ucMinute);
								}
							}
						}
						else if( tempInvenItem->ucIncludeLimit == 0 )
						{
							_sUser_Inven_v1_extension* tempInvenItem = (_sUser_Inven_v1_extension*)currentptr;
							if( tempInvenItem )
							{
								currentptr += sizeof(_sUser_Inven_v1_extension);
								slotnumber = tempInvenItem->cSlot;
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cType(tempInvenItem->cType);
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cSecond(tempInvenItem->cSecond);
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_sID( tempInvenItem->sID );
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cThird(_XGameItem::GetThird(tempInvenItem->cType, tempInvenItem->sID));  
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucCount(tempInvenItem->Item_Money.S_NormalItem.ucCount);
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_usCurDur( tempInvenItem->Item_Money.S_NormalItem.usCurDur );
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_usMaxDur( _XGameItem::GetMaxDurability(tempInvenItem->cType, tempInvenItem->sID) );
								
								if( tempInvenItem->cSecond != _XGI_SC_CLOTHES_BACKPACK )
								{
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSocketCount( tempInvenItem->Item_Money.S_NormalItem.ucSocketCount );
									if( tempInvenItem->Item_Money.S_NormalItem.ucSocketCount > 0 )
									{
										for( int i = 0; i < 4; ++i )
											g_pLocalUser->m_UserItemList[slotnumber].Set_m_sSocket( i, tempInvenItem->sSocket[i] );
									}								
								}
							}
						}
					}
					break;
				default:	// 무기 의상을 제외한 아이템
					{
						if( tempInvenItem->ucIncludeLimit == 1 )
						{
							_sUser_Inven_v1_limit* tempInvenItem = (_sUser_Inven_v1_limit*)currentptr;						
							if( tempInvenItem )
							{
								currentptr += sizeof(_sUser_Inven_v1_limit);
								slotnumber = tempInvenItem->cSlot;
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cType(tempInvenItem->cType);
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cSecond(tempInvenItem->cSecond);
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_sID( tempInvenItem->sID );
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cThird(_XGameItem::GetThird(tempInvenItem->cType, tempInvenItem->sID));  
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucCount(tempInvenItem->Item_Money.S_NormalItem.ucCount);
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_usCurDur( tempInvenItem->Item_Money.S_NormalItem.usCurDur );
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_usMaxDur( _XGameItem::GetMaxDurability(tempInvenItem->cType, tempInvenItem->sID) );

								g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag1 = tempInvenItem->uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //Author : 양희왕 //breif : 추가 하지 않았음
								g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag2	= tempInvenItem->uselimit.ucFlag2;	
								g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag3	= tempInvenItem->uselimit.ucFlag3;
#endif
								g_pLocalUser->m_UserItemList[slotnumber].m_usValue1	= tempInvenItem->uselimit.usValue1;

								if( tempInvenItem->uselimit.ucYear )
								{
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucYear(tempInvenItem->uselimit.ucYear);	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucMonth(tempInvenItem->uselimit.ucMonth);	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucDay(tempInvenItem->uselimit.ucDay);	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucHour(tempInvenItem->uselimit.ucHour);	
									g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucMinute(tempInvenItem->uselimit.ucMinute);
								}
							}
						}
						else if( tempInvenItem->ucIncludeLimit == 0 )
						{
							_sUser_InvenItem* tempInvenItem = (_sUser_InvenItem*)currentptr;
							if( tempInvenItem )
							{
								currentptr += sizeof(_sUser_InvenItem);
								slotnumber = tempInvenItem->cSlot;
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cType(tempInvenItem->cType);
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cSecond(tempInvenItem->cSecond);
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_sID( tempInvenItem->sID );
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_cThird(_XGameItem::GetThird(tempInvenItem->cType, tempInvenItem->sID));  
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucCount( tempInvenItem->Item_Money.S_NormalItem.ucCount );
								
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_usCurDur( tempInvenItem->Item_Money.S_NormalItem.usCurDur );
								g_pLocalUser->m_UserItemList[slotnumber].Set_m_usMaxDur( _XGameItem::GetMaxDurability(tempInvenItem->cType, tempInvenItem->sID) );
							}						
						}
						else // Exception error
						{
							_XLog( "ERROR : parsing inven packet" );
						}
					}
					break;
				}	
				if( slotnumber == _XINVENTORY_SLOTTYPE_BACKPACK1 )
				{
					if( g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID() > 0 )
					{
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
						g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
                    #else
						g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 138,  247, 180 );
						g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 205, 181,  247, 223 );
                    #endif
					}	
					else
					{
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
						g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  136, 0, 168, 32 );
                    #else
						g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 97,  247, 138 );
						g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  205, 97,  247, 138 );
                    #endif
					}
				}
				if( slotnumber == _XINVENTORY_SLOTTYPE_BACKPACK2)
				{
					if( g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID() > 0)
					{
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
						g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
                    #else
						g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 138,  247, 180 );
						g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 205, 181,  247, 223 );	
                    #endif
					}
					else
					{
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
						g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  136, 0, 168, 32 );
                    #else
						g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 97,  247, 138 );
						g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  205, 97,  247, 138 );
                    #endif  
					}
				}
			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
				if( slotnumber == _XINVENTORY_SLOTTYPE_CASHBACKPACK1) //포문으로 캐시백2 가 뒤에 있으니깐.. 예외는 안해도 된다.
				{
					if( g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID() > 0 )
					{
                        //일반 행낭이 없을 때 캐시 행낭 모델
                        //DWORD lparam = (slotnumber*100000) + g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID();
					    //ChangeModel(lparam,g_pLocalUser->m_UserItemList[slotnumber].m_cType);

						g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; //첫번째 행낭이 있음..
                    #ifdef _XDWDEBUG
                        g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
					    g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                    #else
                        g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
					    g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                        //g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
	                    //g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
                    #endif
					}	
					else
					{
						g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;
						g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
						g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  136, 0, 168, 32 );
					}
				}
				if( slotnumber == _XINVENTORY_SLOTTYPE_CASHBACKPACK2)
				{
					if( g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID() > 0)
					{
                        //일반 행낭이 없을 때 캐시 행낭 모델
                        //DWORD lparam = (slotnumber*100000) + g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID();
					    //ChangeModel(lparam,g_pLocalUser->m_UserItemList[slotnumber].m_cType);

						g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_TOTALCOUNT;
                    #ifdef _XDWDEBUG
						g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
					    g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                    #else
                        g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
					    g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                        //g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
	                    //g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
                    #endif
					}
					else
					{
						if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 ) //예외가 있을수 있음.
							g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; //캐시백2가 없다
						else
							g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; //캐시백1도 없다

						g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
						g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  136, 0, 168, 32 );
					}
				}
			#endif
				
#ifdef _XTS_FAME
				if(slotnumber >= _XINVENTORY_SLOTTYPE_AMOR && slotnumber <= _XINVENTORY_SLOTTYPE_ACCESSORY_8)				
#else
				if(slotnumber >= _XINVENTORY_SLOTTYPE_AMOR && slotnumber <= _XINVENTORY_SLOTTYPE_WEAPON_2)
#endif
				{
					CheckValidItem(slotnumber);
				}

				if( slotnumber == _XINVENTORY_SLOTTYPE_WEAPON_1)
				{
					if( g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID() > 0)
					{
						g_pLocalUser->m_CharacterInfo.weaponStrength = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucStrength();

						int	effectid = _XGameItem::GetInchantEffectId(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType(),
							g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID(), g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucStrength() );
						if( effectid > 0 )
						{
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
								effectid, &g_pLocalUser->m_ModelDescriptor.m_Position, NULL, TRUE );
						}
					}					
				}				
			}					
		}
	}	

#ifdef _XDWDEBUG
	_sUser_InvenItem* tempInvenItem = (_sUser_InvenItem*)currentptr;
	if( tempInvenItem )
	{
		int slotnumber = tempInvenItem->cSlot;
		_XLog( "Idx : %d : Type %d, Second %d, ID %d, Third %d, Count %d, Dur %d, MaxDur %d", i, tempInvenItem->cType, tempInvenItem->cSecond, tempInvenItem->sID,
				g_pLocalUser->m_UserItemList[slotnumber].Get_m_cThird(), tempInvenItem->Item_Money.S_NormalItem.ucCount, tempInvenItem->Item_Money.S_NormalItem.usCurDur,
				g_pLocalUser->m_UserItemList[slotnumber].Get_m_usMaxDur() );
	}
#endif

	if( CheckFlag )
	{
		_sUser_InvenItem* tempInvenItem = (_sUser_InvenItem*)currentptr;
		if( tempInvenItem )
		{
			int slotnumber = tempInvenItem->cSlot;
			
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1691), TRUE);
#ifdef _XDWDEBUG
			_XLog( "Idx : %d : Type %d, Second %d, ID %d, Third %d, Count %d, Dur %d, MaxDur %d", i, tempInvenItem->cType, tempInvenItem->cSecond, tempInvenItem->sID,
				g_pLocalUser->m_UserItemList[slotnumber].Get_m_cThird(), tempInvenItem->Item_Money.S_NormalItem.ucCount, tempInvenItem->Item_Money.S_NormalItem.usCurDur,
				g_pLocalUser->m_UserItemList[slotnumber].Get_m_usMaxDur() );
#endif
		}
	}	

	for(i = _XINVENTORY_SLOTTYPE_AMOR ; i <= _XINVENTORY_SLOTTYPE_WEAPON_2 ; ++i)
	{
		DWORD lparam = (i * 100000) + g_pLocalUser->m_UserItemList[i].Get_m_sID();
		ChangeModel(lparam, g_pLocalUser->m_UserItemList[i].Get_m_cType() );
	}
}

void _XNetwork::MSGStorageInfo(MSG_STR_ITEM_INFO* storagelist)
{
	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
	
	pWarehouse_Window->m_bWarehouseFirstLoad = TRUE;
	pWarehouse_Window->m_bWaitForServerResult = FALSE;
	
	pWarehouse_Window->m_TerminativeYear = storagelist->wYear;
	pWarehouse_Window->m_TerminativeMonth = storagelist->ucMonth;
	pWarehouse_Window->m_TerminativeDay = storagelist->ucDay;
	pWarehouse_Window->m_TerminativeHour = storagelist->ucHour;
	pWarehouse_Window->m_TerminateLastItemIndex = storagelist->ucLastItemIdx;
	
	pWarehouse_Window->m_UseSlotCount = storagelist->ucTotalSlot;
	//Author : 양희왕 //breif : 창고 확장..
	if( pWarehouse_Window->m_UseSlotCount > 20 && 
		pWarehouse_Window->m_UseSlotCount <= 40 )
	{
		pWarehouse_Window->m_SelectTabButtton[0]->ShowWindow( TRUE );
		pWarehouse_Window->m_SelectTabButtton[1]->ShowWindow( TRUE );
		pWarehouse_Window->m_SelectTabButtton[2]->ShowWindow( FALSE );
	}
	else if( pWarehouse_Window->m_UseSlotCount > 40 )
	{
		pWarehouse_Window->m_SelectTabButtton[0]->ShowWindow( TRUE );
		pWarehouse_Window->m_SelectTabButtton[1]->ShowWindow( TRUE );
		pWarehouse_Window->m_SelectTabButtton[2]->ShowWindow( TRUE );
	}
	else
	{
		pWarehouse_Window->m_SelectTabButtton[0]->ShowWindow( TRUE );
		pWarehouse_Window->m_SelectTabButtton[1]->ShowWindow( FALSE );
		pWarehouse_Window->m_SelectTabButtton[2]->ShowWindow( FALSE );
	}
	pWarehouse_Window->ChangeTab(0);

	g_pLocalUser->m_WarehouseMoney = storagelist->uiMoney;
	
	
	g_pLocalUser->ResetWarehouseInfo();
	
	if( pWarehouse_Window->m_UseSlotCount == 0 )
	{
		if(storagelist->ucSavedItem == 0)
		{
			if( g_pLocalUser->m_WarehouseMoney > 0 )
			{
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1692), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				
				pWarehouse_Window->m_bTerminateWarehouse = TRUE;
				pNPCScript_Window->GenerateSubScriptList();
			}
			// 창고 추가를 해야한다.

		//	pNPCScript_Window->InsertNPCChatString("창고 추가에서 창고 사용 계약을 하십시오.", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);				
			
		}
		else if( pWarehouse_Window->m_UseSlotCount < storagelist->ucSavedItem )
		{
			// 만료기간이 지나서 창고를 사용할 수 없다.
			InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1692), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );			
			
			pWarehouse_Window->m_bTerminateWarehouse = TRUE;
			pNPCScript_Window->GenerateSubScriptList();
		}
		pWarehouse_Window->m_bUseableWarehouse = FALSE;
	}
	else
	{
		// 창고 사용 만료일은 %d년 %d월 %d일 입니다.
		TCHAR messagestring[256];

		int param1 = pWarehouse_Window->m_TerminativeYear;
		int param2 = pWarehouse_Window->m_TerminativeMonth;
		int param3 = pWarehouse_Window->m_TerminativeDay;

		strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAMEMESSAGEHANDLER_WAREHOUSETERMINATIONDAY,
																&param1, &param2, &param3 ) );
		
		pNPCScript_Window->InsertNPCChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
		
		pWarehouse_Window->m_bUseableWarehouse = TRUE;				
	}
	if( storagelist->ucSavedItem )
	{
		char* currentptr = storagelist->cStorage;

		for(int i = 0 ; i < _XDEF_WAREHOUSE_TOTALCOUNT; ++i)
		{
			_sUser_Inven_v1_all* tempInvenItem = (_sUser_Inven_v1_all*)currentptr;
			if( tempInvenItem )
			{
				currentptr += sizeof(_sUser_Inven_v1_all);
				if( tempInvenItem->sID > 0 )
				{					
					int slotnumber = tempInvenItem->cSlot;
					
					g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_cType(tempInvenItem->cType);
					g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_cSecond(tempInvenItem->cSecond);
					g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_sID(tempInvenItem->sID);
					
					g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_cThird(_XGameItem::GetThird(tempInvenItem->cType, tempInvenItem->sID));  
					g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_ucCount( tempInvenItem->Item_Money.S_NormalItem.ucCount );
					
					g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_usCurDur( tempInvenItem->Item_Money.S_NormalItem.usCurDur );
					g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_usMaxDur( _XGameItem::GetMaxDurability(tempInvenItem->cType, tempInvenItem->sID) );
					
					g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_ucSocketCount( tempInvenItem->Item_Money.S_NormalItem.ucSocketCount );
					if( tempInvenItem->Item_Money.S_NormalItem.ucSocketCount > 0 )
					{
						for( int i = 0; i < 4; ++i )
							g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_sSocket( i, tempInvenItem->sSocket[i] );
					}									
					g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_ucStrength( tempInvenItem->ucStrength );
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
					g_pLocalUser->m_WarehouseItemList[slotnumber].m_usCurProtect  = tempInvenItem->uselimit.usCurProtect; // 바른용린의수
					g_pLocalUser->m_WarehouseItemList[slotnumber].m_usCurMaxProtect = tempInvenItem->uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
					g_pLocalUser->m_WarehouseItemList[slotnumber].m_usUsedProtect = tempInvenItem->uselimit.usUsedProtect; // 용루 사용여부
					
					g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_ucWeaponStrength( _XGameItem::GetNature(tempInvenItem->cType, tempInvenItem->sID) );
					g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_ucSharpness( tempInvenItem->ucSharpness );
#else
					g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_ucSharpness(tempInvenItem->ucSharpness);
#endif
					
					g_pLocalUser->m_WarehouseItemList[slotnumber].m_ucFlag1 = tempInvenItem->uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //Author : 양희왕 //breif 디파인되어있으면 들어오지 않는다
					g_pLocalUser->m_WarehouseItemList[slotnumber].m_ucFlag2		= tempInvenItem->uselimit.ucFlag2;	
					g_pLocalUser->m_WarehouseItemList[slotnumber].m_ucFlag3		= tempInvenItem->uselimit.ucFlag3;
#endif
					g_pLocalUser->m_WarehouseItemList[slotnumber].m_usValue1	= tempInvenItem->uselimit.usValue1;

					if( tempInvenItem->uselimit.ucYear > 0 )
					{						
						g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_ucYear(tempInvenItem->uselimit.ucYear);	
						g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_ucMonth(tempInvenItem->uselimit.ucMonth);	
						g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_ucDay(tempInvenItem->uselimit.ucDay);	
						g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_ucHour(tempInvenItem->uselimit.ucHour);	
						g_pLocalUser->m_WarehouseItemList[slotnumber].Set_m_ucMinute(tempInvenItem->uselimit.ucMinute);
					}
				}
				else
				{
#ifdef _XDWDEBUG
					_XLog( "Idx : %d : Type %d, Second %d, ID %d, Third %d, Count %d, Dur %d, MaxDur %d", i, tempInvenItem->cType, tempInvenItem->cSecond, tempInvenItem->sID,
						_XGameItem::GetThird(tempInvenItem->cType, tempInvenItem->sID), tempInvenItem->Item_Money.S_NormalItem.ucCount,
						tempInvenItem->Item_Money.S_NormalItem.usCurDur, _XGameItem::GetMaxDurability(tempInvenItem->cType, tempInvenItem->sID) );
#endif				
				}
			} // -------------------------------------┛
		}
	}
}

void _XNetwork::MSGInvenMove(MSG_INVEN_MOVE* invenmove)
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	if(invenmove->cTo == 255)
	{
		TCHAR messagestring[256];
		memset(messagestring, 0, sizeof(messagestring));

		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_CANNOTCHANGE));
		InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
	}
	else if(invenmove->cTo == 254)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_NOTEQUIP), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
	}
	else if(invenmove->cTo == 253)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1694), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
	}
	else if(invenmove->cTo == 252)
	{
		// 해당 물품은 중복해서 소지할 수 없습니다.
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2985), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
	}
#else
    if(invenmove->cTo == -1)
	{
		TCHAR messagestring[256];
		memset(messagestring, 0, sizeof(messagestring));
		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_CANNOTCHANGE));
		InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
	}
	else if(invenmove->cTo == -2)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_NOTEQUIP), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
	}
	else if(invenmove->cTo == -3)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1694), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
	}
	else if(invenmove->cTo == -4)
	{
		// 해당 물품은 중복해서 소지할 수 없습니다.
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2985), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
	}
#endif
	else
	{
		if(invenmove->cFlag == 1)
		{
			// merge
			g_pLocalUser->m_UserItemList[invenmove->cTo].Set_m_ucCount(g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_ucCount() + invenmove->ucSendCount);
			g_pLocalUser->m_UserItemList[invenmove->cFrom].Set_m_ucCount(g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_ucCount() - invenmove->ucSendCount);
			if(g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_ucCount() <= 0)
			{
				
				// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
				g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_cType(), g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_sID()  );
				
				g_pLocalUser->m_UserItemList[invenmove->cFrom].Reset();
			}
			
			// Quick slot에 등록되어있던 아이템 체크
			if(g_pQuickSlot_Window)
			{
				g_pQuickSlot_Window->CheckQuickSlotItem();
			}

			PlayItemEquipSound( g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType(), 
								g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_sID() );
		}		
		else if(invenmove->cFlag == 0)
		{
			// 교환	
			//_XPlayInterfaceSound(ID_SR_INTERFACE_INTERFACE_PICKITEM01_WAV);
			PlayItemEquipSound( g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_cType(), 
								g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_sID() );
			
			_XUserItem temp = g_pLocalUser->m_UserItemList[invenmove->cFrom];
			g_pLocalUser->m_UserItemList[invenmove->cFrom] = g_pLocalUser->m_UserItemList[invenmove->cTo];			
			g_pLocalUser->m_UserItemList[invenmove->cTo] = temp;

			// 장착된것을 내린다.
			if(invenmove->cFrom >= _XINVENTORY_SLOTTYPE_AMOR && invenmove->cFrom < _XINVENTORY_SLOTTYPE_POCKETSTART )
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType()) ||
					_XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType()) )
#else
				if( g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType() == _XGI_FC_WEAPON || 
					g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType() == _XGI_FC_WEAPON2 ||
					g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType() == _XGI_FC_WEAPON3 ||
					g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType() == _XGI_FC_CLOTHES ||
					g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType() == _XGI_FC_CLOTHES2 ||
					g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType() == _XGI_FC_CLOTHES3 )
#endif
				{										
					if( invenmove->cFrom == _XINVENTORY_SLOTTYPE_WEAPON_1 )
					{						
						int effectid = _XGameItem::GetInchantEffectId(g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType(),
										g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_sID(), g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_ucStrength() );
						if( effectid > 0 )
						{
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance( effectid );
						}
						g_pLocalUser->m_CharacterInfo.weaponStrength = 0;
					}
					DWORD lparam = (invenmove->cFrom*100000) + g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_sID();
					ChangeModel(lparam, g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_cType() );
					
					if( invenmove->cFrom == _XINVENTORY_SLOTTYPE_BACKPACK1 )
					{
						g_pLocalUser->m_cBackPackExtend = invenmove->cBackPackExtend;

                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        //행낭으로 아이템 이동할 때 그 창으로 바꿔준다 //창이 켜 있지 않아도 바꿔 준다
                        if( invenmove->cTo >= _XINVENTORY_SLOTTYPE_POCKETSTART && invenmove->cTo < _XINVENTORY_SLOTTYPE_TOTALCOUNT)
                        {
                            int nbaseTab = (int)(invenmove->cTo/_XINVENTORY_SLOTTYPE_POCKETSTART) - 1;
                            g_pInventory_Window->ChangeBaseTab( nbaseTab );
                        }
                        
                        g_pInventory_Window->SetTabButtonPosition();
                        g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
						g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  136, 0, 168, 32 );
                    #else
						g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 97,  247, 138 );
						g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 205, 97,  247, 138 );
                    #endif
					}
					else if( invenmove->cFrom == _XINVENTORY_SLOTTYPE_BACKPACK2 )
					{
						g_pLocalUser->m_cBackPackExtend = invenmove->cBackPackExtend;

                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        //행낭으로 아이템 이동할 때 그 창으로 바꿔준다 //창이 켜 있지 않아도 바꿔 준다
                        if( invenmove->cTo >= _XINVENTORY_SLOTTYPE_POCKETSTART && invenmove->cTo < _XINVENTORY_SLOTTYPE_TOTALCOUNT)
                        {
                            int nbaseTab = (int)(invenmove->cTo/_XINVENTORY_SLOTTYPE_POCKETSTART) - 1;
                            g_pInventory_Window->ChangeBaseTab( nbaseTab );
                        }

                        g_pInventory_Window->SetTabButtonPosition();
                        g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
						g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  136, 0, 168, 32 );
                    #else						
						g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 97,  247, 138 );
						g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 205, 97,  247, 138 );
                    #endif
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장
					else if( invenmove->cFrom == _XINVENTORY_SLOTTYPE_CASHBACKPACK1 )
					{
                        //행낭으로 아이템 이동할 때 그 창으로 바꿔준다 //창이 켜 있지 않아도 바꿔 준다
                        if( invenmove->cTo >= _XINVENTORY_SLOTTYPE_POCKETSTART && invenmove->cTo < _XINVENTORY_SLOTTYPE_TOTALCOUNT)
                        {
                            int nbaseTab = (int)(invenmove->cTo/_XINVENTORY_SLOTTYPE_POCKETSTART) - 1;
                            g_pInventory_Window->ChangeBaseTab( nbaseTab );
                        }

                        //행낭이 빠졋따는건 아이템이 없다는 뜻
                        g_pInventory_Window->m_bIteminCashBackPack[0] = FALSE;
                        g_pInventory_Window->SetTabButtonPosition();
						g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;
						g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
						g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  136, 0, 168, 32 );
					}
					else if( invenmove->cFrom == _XINVENTORY_SLOTTYPE_CASHBACKPACK2 )
					{
                        //행낭으로 아이템 이동할 때 그 창으로 바꿔준다 //창이 켜 있지 않아도 바꿔 준다
                        if( invenmove->cTo >= _XINVENTORY_SLOTTYPE_POCKETSTART && invenmove->cTo < _XINVENTORY_SLOTTYPE_TOTALCOUNT)
                        {
                            int nbaseTab = (int)(invenmove->cTo/_XINVENTORY_SLOTTYPE_POCKETSTART) - 1;
                            g_pInventory_Window->ChangeBaseTab( nbaseTab );
                        }

                        if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 )
                            g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; //장착한걸 내리니깐.. 캐시 1만 남은상태
                        else
                            g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; //캐시 1이 없다.. 1이없는데 2가 있는건 1이 기간제로 삭제 됬을 경우

                        //행낭이 빠졋따는건 아이템이 없다는 뜻
                        g_pInventory_Window->m_bIteminCashBackPack[1] = FALSE;
                        g_pInventory_Window->SetTabButtonPosition();
						g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
						g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  136, 0, 168, 32 );
					}
				#endif
					// 포켓 갯수 처리
					else if( invenmove->cFrom == _XINVENTORY_SLOTTYPE_AMOR )
					{
						char tempPocket = _XGameItem::GetPocketNum(g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_cType(), g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_sID());
						if( invenmove->cAmorPocket == tempPocket )
							g_pLocalUser->m_cAmorPocketNum = invenmove->cAmorPocket;
					}
					else if( invenmove->cFrom == _XINVENTORY_SLOTTYPE_PANTS )
					{
						char tempPocket = _XGameItem::GetPocketNum(g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_cType(), g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_sID());
						if( invenmove->cPantsPocket == tempPocket )
							g_pLocalUser->m_cPantsPocketNum = invenmove->cPantsPocket;
					}
				}
				
				// 교환한 후 원래 있던 것은 삭제
				if(g_pState_Window)
				{
					g_pState_Window->DeleteStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_sID(), g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType(), invenmove->cFrom);
				}
				// 퀘스트 시작조건에 아이템 장착 조건이 있으면 시작조건의 아이템인지 체크를 한다.
				g_QuestScriptManager.RecheckWearItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType(), g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_sID() );
			}
			
			// 빼는 것이 악세사리일때  퀘스트 시작조건에 아이템 장착 조건이 있으면 시작조건의 아이템인지 체크를 한다.
			if( invenmove->cFrom >= _XINVENTORY_SLOTTYPE_ACCESSORY_1 &&  invenmove->cFrom <= _XINVENTORY_SLOTTYPE_ACCESSORY_8 )
			{				
				// 퀘스트 시작조건에 아이템 장착 조건이 있으면 시작조건의 아이템인지 체크를 한다.
				g_QuestScriptManager.RecheckWearItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType(), g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_sID() );
				
			}
			
			// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
			if( g_TutorialInfo )
			{
				if( !g_TutorialInfo->_XTutorial_009 )
				{
					if( invenmove->cTo >= _XINVENTORY_SLOTTYPE_AMOR && invenmove->cTo < _XINVENTORY_SLOTTYPE_WEAPON_1 )
					{
						_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
						pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_009);
						g_TutorialInfo->_XTutorial_009 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
						g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif						
					}
				}
				
				if( invenmove->cTo >= _XINVENTORY_SLOTTYPE_ACCESSORY_1 &&  invenmove->cTo <= _XINVENTORY_SLOTTYPE_ACCESSORY_8 && !g_TutorialInfo->_XTutorial_011 )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_011);
					g_TutorialInfo->_XTutorial_011 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
					
					// 퀘스트 시작조건에 아이템 장착 조건이 들어간 퀘스트 시작조건을 검사 
					//g_QuestScriptManager.QuestStartConditionWearItemMode();
				}
				
				if( (invenmove->cAmorPocket > 0 || invenmove->cPantsPocket > 0) && !g_TutorialInfo->_XTutorial_012 )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_012);
					g_TutorialInfo->_XTutorial_012 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
				}
				
				
				if( !g_TutorialInfo->_XTutorial_010 )
				{
					if( invenmove->cTo == _XINVENTORY_SLOTTYPE_WEAPON_1 || invenmove->cTo == _XINVENTORY_SLOTTYPE_WEAPON_2 )
					{
						_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
						pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_010);
						g_TutorialInfo->_XTutorial_010 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
						g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
					}
				}
			}			
			// ----------------------------------------------------------------------------------------------------------------------------=

			// 장착한다.
			if(invenmove->cTo >= _XINVENTORY_SLOTTYPE_AMOR && invenmove->cTo < _XINVENTORY_SLOTTYPE_POCKETSTART)
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType()) ||
					_XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType()) )
#else
				if( g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType() == _XGI_FC_WEAPON || 
					g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType() == _XGI_FC_WEAPON2 ||
					g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType() == _XGI_FC_WEAPON3 ||
					g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType() == _XGI_FC_CLOTHES ||
					g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType() == _XGI_FC_CLOTHES2 ||
					g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType() == _XGI_FC_CLOTHES3 )
#endif
				{
					if( invenmove->cTo == _XINVENTORY_SLOTTYPE_WEAPON_1 )
					{
						int effectid = _XGameItem::GetInchantEffectId(g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_cType(),
										g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_sID(), g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_ucStrength() );
						
						if( effectid > 0 )
						{
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(effectid);
						}
						
						g_pLocalUser->m_CharacterInfo.weaponStrength = g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_ucStrength();
					}
					DWORD lparam = (invenmove->cTo*100000) + g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_sID();
					ChangeModel(lparam,g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType());
					
					if( invenmove->cTo == _XINVENTORY_SLOTTYPE_BACKPACK1 )
					{
						g_pLocalUser->m_cBackPackExtend = invenmove->cBackPackExtend;
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        g_pInventory_Window->SetTabButtonPosition();
                        g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
						g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
                    #else
						g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 138,  247, 180 );
						g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 205, 181,  247, 223 );
                    #endif
					}
					else if( invenmove->cTo == _XINVENTORY_SLOTTYPE_BACKPACK2 )
					{
						g_pLocalUser->m_cBackPackExtend = invenmove->cBackPackExtend;
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
                        g_pInventory_Window->SetTabButtonPosition();
                        g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
						g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
                    #else
						g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 138,  247, 180 );
						g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 205, 181,  247, 223 );
                    #endif
					}					
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장
					else if( invenmove->cTo == _XINVENTORY_SLOTTYPE_CASHBACKPACK1 )
					{
						g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART;
                        g_pInventory_Window->SetTabButtonPosition();
                    #ifdef _XDWDEBUG
						g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
						g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                    #else
                        g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
						g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                        //g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
	                    //g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
                    #endif
					}
					else if( invenmove->cTo == _XINVENTORY_SLOTTYPE_CASHBACKPACK2 )
					{
						g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_TOTALCOUNT;
                        g_pInventory_Window->SetTabButtonPosition();
                    #ifdef _XDWDEBUG
						g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
						g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                    #else
                        g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
						g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                        //g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
	                    //g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
                    #endif
					}
				#endif
					// 포켓 갯수 처리				
					else if( invenmove->cTo == _XINVENTORY_SLOTTYPE_AMOR )
					{
						char tempPocket = _XGameItem::GetPocketNum(g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType(), g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_sID());
						if( invenmove->cAmorPocket == tempPocket )
							g_pLocalUser->m_cAmorPocketNum = invenmove->cAmorPocket;
					}
					else if( invenmove->cTo == _XINVENTORY_SLOTTYPE_PANTS )
					{
						char tempPocket = _XGameItem::GetPocketNum(g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType(), g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_sID());
						if( invenmove->cPantsPocket == tempPocket )
							g_pLocalUser->m_cPantsPocketNum = invenmove->cPantsPocket;
					}
				}
				// 원래 있던 것 삭제
				if(g_pState_Window)
				{
					g_pState_Window->DeleteStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_sID(), g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_cType(), invenmove->cTo);
				}
								
				// 새로들어가는 것 삽입
				CheckValidItem(invenmove->cTo);
				
				// 퀘스트 시작조건에 아이템 장착 조건이 들어간 퀘스트 시작조건을 검사 
				g_QuestScriptManager.QuestStartConditionWearItemMode();
			}

			// Quick slot에 등록되어있던 아이템 체크
			if(g_pQuickSlot_Window)
			{
				g_pQuickSlot_Window->CheckQuickSlotItem();
			}
			
			g_pLocalUser->UpdateLocalUserAttackLevel();
			

			// 인터넷 회선이 느려서 박스가 열리고 난 후에 인벤토리 이동 패킷이 오면
			// 박스형 아이템이 아닌 경우에도 박스 인터페이스가 뜰 경우
			// 이동된 박스 아이템의 정보를 다시 세팅해 준다.
			if( g_pInventory_Window )
			{
				if( g_pInventory_Window->m_pGamble_Window )
				{
					if( g_pInventory_Window->m_pGamble_Window->GetShowStatus() )
					{
						if( invenmove->cFrom >= 0 )
						{
							if( g_pLocalUser->m_UserItemList[invenmove->cFrom].Get_m_cType() == _XGI_FC_BOX )
							{
								g_pInventory_Window->m_UsedLifeItemIndex = invenmove->cFrom;
								g_pInventory_Window->m_pGamble_Window->m_GambleBoxItem = &g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex];
								g_pInventory_Window->m_iSocketIndexList[0] = g_pInventory_Window->m_UsedLifeItemIndex;
							}
						}
						
					}
				}
			}



		}
		else if(invenmove->cFlag == 2)
		{
			g_pLocalUser->SetChangingWeaponPacket( TRUE, invenmove->cTo ); //Author : 양희왕 //무기 변경 요청을 받았다
			
		#ifdef _XDEF_BLOCK_MOVINGATTACK //Author : 양희왕 //breif : 한대치고 에니메이션이 끊기는 경우 //date 07/12/03
			if(g_pLocalUser->GetMotionClass() == _XACTION_ATTACK )
			{
				return;
			}
			if(g_pLocalUser->m_CurrentBattleState >= _XUSER_STATE_2 && g_pLocalUser->m_CurrentBattleState <= _XUSER_STATE_LAST)
			{
				return;
			}
		#endif
			// weapon 변경
#ifdef _XTESTSERVER
			if( (invenmove->cTo   >= _XINVENTORY_SLOTTYPE_WEAPON_1 || invenmove->cTo == _XINVENTORY_SLOTTYPE_WEAPON_2  ) &&
			    (invenmove->cFrom >= _XINVENTORY_SLOTTYPE_WEAPON_1 || invenmove->cFrom == _XINVENTORY_SLOTTYPE_WEAPON_2) )
#else
			if( (invenmove->cTo   >= _XINVENTORY_SLOTTYPE_WEAPON_1 || invenmove->cTo == _XINVENTORY_SLOTTYPE_WEAPON_2  ) &&
			    (invenmove->cFrom >= _XINVENTORY_SLOTTYPE_WEAPON_1 || invenmove->cFrom == _XINVENTORY_SLOTTYPE_WEAPON_2) )
#endif
			{
				// 경고 icon이 떠있을경우 inven number update
				if(g_pState_Window)
				{
					g_pState_Window->UpdatePlayerStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID(), g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType(),
						_XINVENTORY_SLOTTYPE_WEAPON_1, _XINVENTORY_SLOTTYPE_WEAPON_2);

					g_pState_Window->UpdatePlayerStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2].Get_m_sID(), g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2].Get_m_cType(),
						_XINVENTORY_SLOTTYPE_WEAPON_2, _XINVENTORY_SLOTTYPE_WEAPON_1);
				}
				
				_XUserItem temp = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1];
				g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1] = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2];
				g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2] = temp;

				//Author : 양희왕  breif : 무기 교체가 완료 되었다
				g_pLocalUser->SetChangingWeaponPacket( FALSE, 0 ); 
				g_pLocalUser->SetChangingWeapon( FALSE );

				int effectid = _XGameItem::GetInchantEffectId(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2].Get_m_cType(),
									g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2].Get_m_sID(), g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_2].Get_m_ucStrength() );

				if( effectid > 0 )
				{
					g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(effectid);
				}
				g_pLocalUser->m_CharacterInfo.weaponStrength = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucStrength();

				DWORD lparam = (invenmove->cTo*100000) + g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_sID();
				ChangeModel(lparam, g_pLocalUser->m_UserItemList[invenmove->cTo].Get_m_cType());

				g_pLocalUser->UpdateLocalUserAttackLevel();

				_XPlayInterfaceSound(ID_SR_INTERFACE_PICK_ITEM_001_WAV);
			}
		}
#ifdef _XTS_FAME		
		// 명성, 악명 아이템 착용 여부 검사
		CheckEmptyFameItem();
#endif
	}

	//Author : 양희왕 //breif : 아이템 교체 시 슬롯 막기 해제
	g_pQuickSlot_Window->ReplacementOfItems(FALSE);

	if(g_pInventory_Window)
	{
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
		if( g_pDefaultTooltip_Window )
		{
			if( g_pDefaultTooltip_Window->GetShowStatus() )
				g_pDefaultTooltip_Window->ShowWindow( FALSE );
		}
	}
}

void _XNetwork::MSGItemGrabSuccess(MSG_ITEM_GRAB_SUCCESS* successgrabitem)
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
    if(successgrabitem->cInvenSlot == 255)
	{				
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_WARNGETITEM), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}
	else if(successgrabitem->cInvenSlot == 254)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAIGAME_NOTEMPTY), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		_XGameItem* pItem = g_GameItemlist.FindWorldItem(successgrabitem->ItemWorldID);
		return;
	}
	else if(successgrabitem->cInvenSlot == 253)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1695), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}
#else
	if(successgrabitem->cInvenSlot == -1)
	{				
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_WARNGETITEM), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}
	else if(successgrabitem->cInvenSlot == -2)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAIGAME_NOTEMPTY), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		_XGameItem* pItem = g_GameItemlist.FindWorldItem(successgrabitem->ItemWorldID);
		return;
	}
	else if(successgrabitem->cInvenSlot == -3)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1695), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}
#endif
	if( g_TutorialInfo )
	{
		if( !g_TutorialInfo->_XTutorial_050 )	// 생사결 물품 획득시
		{
			if( successgrabitem->ItemInfo.cType == _XGI_FC_RESOURCE )
			{
				if( successgrabitem->ItemInfo.sID > 169 && successgrabitem->ItemInfo.sID < 176 )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_050);
					g_TutorialInfo->_XTutorial_050 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
				}
			}			
		}
	}

	bool success = false;
	
	// 퀘스트 아이템 카운트 표시할때 2개이상 들어와도 표시하기 위해서 더하기 이전 값을 저장해야 한다.
	int itemcountbeforetrade = g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Get_m_ucCount();

   	if(successgrabitem->cCount == 0)
	{ 
		success = true;
		// 인벤토리 한 슬롯만 채운다.
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_cType(successgrabitem->ItemInfo.cType);
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_cSecond(successgrabitem->ItemInfo.cSecond);
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_sID( successgrabitem->ItemInfo.sID );
		
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_cIdentify = successgrabitem->ItemInfo.Item_Money.S_NormalItem.cIdentify;
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_cThird(_XGameItem::GetThird(successgrabitem->ItemInfo.cType, successgrabitem->ItemInfo.sID));
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_ucCount(successgrabitem->ItemInfo.Item_Money.S_NormalItem.ucCount);
		
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_usCurDur( successgrabitem->ItemInfo.Item_Money.S_NormalItem.usCurDur );
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_usMaxDur( _XGameItem::GetMaxDurability(successgrabitem->ItemInfo.cType, successgrabitem->ItemInfo.sID) );
		
// 기간제 아이템 : 행낭에 바로 들어가기 처리하기.
#ifdef _XDEF_PERIODITEM_20061027		
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_ucSocketCount( successgrabitem->ItemInfo.Item_Money.S_NormalItem.ucSocketCount );
		if( successgrabitem->ItemInfo.Item_Money.S_NormalItem.ucSocketCount > 0 )
		{
			for( int i = 0; i < 4; ++i )
				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_sSocket( i, successgrabitem->ItemInfo.sSocket[i] );
		}
		
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_ucStrength( successgrabitem->ItemInfo.ucStrength );
	#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_usCurProtect  = successgrabitem->ItemInfo.uselimit.usCurProtect; // 바른용린의수
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_usCurMaxProtect = successgrabitem->ItemInfo.uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_usUsedProtect = successgrabitem->ItemInfo.uselimit.usUsedProtect; // 용루 사용여부
		
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_ucWeaponStrength( _XGameItem::GetNature(successgrabitem->ItemInfo.cType, successgrabitem->ItemInfo.sID) );
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_ucSharpness( successgrabitem->ItemInfo.ucSharpness );
	#else
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_ucSharpness(successgrabitem->ItemInfo.ucSharpness);
	#endif
		
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucFlag1 = successgrabitem->ItemInfo.uselimit.ucFlag1;

	#ifndef _XDEF_SMELT_ITEM //Author : 양희왕 //breif 디파인되어있으면 들어오지 않는다
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucFlag2		= successgrabitem->ItemInfo.uselimit.ucFlag2;	
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucFlag3		= successgrabitem->ItemInfo.uselimit.ucFlag3;
	#endif
		g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_usValue1	= successgrabitem->ItemInfo.uselimit.usValue1;

		if( successgrabitem->ItemInfo.uselimit.ucYear > 0 )
		{
			g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_ucYear(successgrabitem->ItemInfo.uselimit.ucYear);	
			g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_ucMonth(successgrabitem->ItemInfo.uselimit.ucMonth);	
			g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_ucDay(successgrabitem->ItemInfo.uselimit.ucDay);	
			g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_ucHour(successgrabitem->ItemInfo.uselimit.ucHour);	
			g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Set_m_ucMinute(successgrabitem->ItemInfo.uselimit.ucMinute);
		}
#else//_XDEF_PERIODITEM_20061027
		// 무기, 의상일때만 소켓값을 읽어들인다.. 
	#ifdef _XTS_ITEM_OPTIMIZATION
		switch( GetEqualItemType(g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Get_m_cType()) ) 
	#else
		switch(g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Get_m_cType()) 
	#endif
		{
	#ifdef _XTS_ITEM_OPTIMIZATION
		case _XGI_FC_WEAPON :
	#else
		case _XGI_FC_WEAPON :
		case _XGI_FC_WEAPON2 :
		case _XGI_FC_WEAPON3 :
	#endif
			{
				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucSocketCount = successgrabitem->ItemInfo.Item_Money.S_NormalItem.ucSocketCount;
				if( successgrabitem->ItemInfo.Item_Money.S_NormalItem.ucSocketCount > 0 )
				{
					for( int i = 0; i < 4; ++i )
						g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_sSocket[i]	= successgrabitem->ItemInfo.sSocket[i];
				}

				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucStrength = successgrabitem->ItemInfo.ucStrength;
	#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucCurProtect  = successgrabitem->ItemInfo.uselimit.ucCurProtect; // 바른용린의수
				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_usCurMaxProtect = successgrabitem->ItemInfo.uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucUsedProtect = successgrabitem->ItemInfo.uselimit.usUsedProtect; // 용루 사용여부
				
				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucWeaponStrength	= _XGameItem::GetNature(successgrabitem->ItemInfo.cType, successgrabitem->ItemInfo.sID);
				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucSharpness	= successgrabitem->ItemInfo.ucSharpness;
	#else
				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucSharpness	= successgrabitem->ItemInfo.ucSharpness;
	#endif

				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucFlag1 = successgrabitem->ItemInfo.uselimit.ucFlag1;

	#ifndef _XDEF_SMELT_ITEM //author : 양희왕 //breif : 디파인 되어있으면 들어오지 않는다
				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucFlag2		= successgrabitem->ItemInfo.uselimit.ucFlag2;	
				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucFlag3		= successgrabitem->ItemInfo.uselimit.ucFlag3;
	#endif
				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_usValue1	= successgrabitem->ItemInfo.uselimit.usValue1;

				if( successgrabitem->ItemInfo.uselimit.ucYear > 0 )
				{
					g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucYear		= successgrabitem->ItemInfo.uselimit.ucYear;	
					g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucMonth		= successgrabitem->ItemInfo.uselimit.ucMonth;	
					g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucDay		= successgrabitem->ItemInfo.uselimit.ucDay;	
					g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucHour		= successgrabitem->ItemInfo.uselimit.ucHour;	
					g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucMinute	= successgrabitem->ItemInfo.uselimit.ucMinute;
				}
			}
			break;
	#ifdef _XTS_ITEM_OPTIMIZATION
		case _XGI_FC_CLOTHES :
	#else
		case _XGI_FC_CLOTHES :
		case _XGI_FC_CLOTHES2 :
		case _XGI_FC_CLOTHES3 :
	#endif
			{
				if( g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_cSecond != _XGI_SC_CLOTHES_BACKPACK )
				{
					g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucSocketCount = successgrabitem->ItemInfo.Item_Money.S_NormalItem.ucSocketCount;
					if( successgrabitem->ItemInfo.Item_Money.S_NormalItem.ucSocketCount > 0 )
					{
						for( int i = 0; i < 4; ++i )
							g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_sSocket[i]	= successgrabitem->ItemInfo.sSocket[i];
					}
					g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucFlag1		= successgrabitem->ItemInfo.uselimit.ucFlag1;

	#ifndef _XDEF_SMELT_ITEM //author : 양희왕 //breif : 디파인 되어있으면 들어오지 않는다
					g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucFlag2		= successgrabitem->ItemInfo.uselimit.ucFlag2;	
					g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucFlag3		= successgrabitem->ItemInfo.uselimit.ucFlag3;
	#endif
					g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_usValue1	= successgrabitem->ItemInfo.uselimit.usValue1;

					if( successgrabitem->ItemInfo.uselimit.ucYear > 0 )
					{
						g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucYear		= successgrabitem->ItemInfo.uselimit.ucYear;	
						g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucMonth		= successgrabitem->ItemInfo.uselimit.ucMonth;	
						g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucDay		= successgrabitem->ItemInfo.uselimit.ucDay;	
						g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucHour		= successgrabitem->ItemInfo.uselimit.ucHour;	
						g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_ucMinute	= successgrabitem->ItemInfo.uselimit.ucMinute;
					}
				}
			}
			break;
		}
#endif
		
		if( g_pInventory_Window )
		{
			if( g_pInventory_Window->GetShowStatus() )
			{
				g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].m_bNewItem = TRUE;
				g_pInventory_Window->m_bDrawGrabItem = TRUE;
			}
		}

		PlayItemEquipSound( g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Get_m_cType(), 
							g_pLocalUser->m_UserItemList[successgrabitem->cInvenSlot].Get_m_sID() );
	}
	else if(successgrabitem->cCount > 0)
	{
		success = true;
		for(int i = 0 ; i < successgrabitem->cCount ; ++i)
		{
			g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_cType( successgrabitem->ItemInfo.cType );
			g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_cSecond( successgrabitem->ItemInfo.cSecond );
			g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_sID( successgrabitem->ItemInfo.sID );
			
			g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].m_cIdentify = successgrabitem->ItemInfo.Item_Money.S_NormalItem.cIdentify;
			g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_cThird( _XGameItem::GetThird(successgrabitem->ItemInfo.cType, successgrabitem->ItemInfo.sID) );
			g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_ucCount( successgrabitem->result[i].number );
			
			g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_usCurDur( successgrabitem->ItemInfo.Item_Money.S_NormalItem.usCurDur );
			g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_usMaxDur( _XGameItem::GetMaxDurability(successgrabitem->ItemInfo.cType, successgrabitem->ItemInfo.sID) );

			g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_ucSocketCount( successgrabitem->ItemInfo.Item_Money.S_NormalItem.ucSocketCount );
			if( successgrabitem->ItemInfo.Item_Money.S_NormalItem.ucSocketCount > 0 )
			{
				for( int i = 0; i < 4; ++i )
					g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_sSocket( i, successgrabitem->ItemInfo.sSocket[i] );
			}
			g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].m_ucFlag1 = successgrabitem->ItemInfo.uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //Author : 양희왕 //breif : 추가 하지 않았음
				g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].m_ucFlag2		= successgrabitem->ItemInfo.uselimit.ucFlag2;	
				g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].m_ucFlag3		= successgrabitem->ItemInfo.uselimit.ucFlag3;
#endif
				g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].m_usValue1	= successgrabitem->ItemInfo.uselimit.usValue1;

			if( successgrabitem->ItemInfo.uselimit.ucYear > 0 )
			{				
				g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_ucYear( successgrabitem->ItemInfo.uselimit.ucYear );	
				g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_ucMonth( successgrabitem->ItemInfo.uselimit.ucMonth );	
				g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_ucDay( successgrabitem->ItemInfo.uselimit.ucDay );	
				g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_ucHour( successgrabitem->ItemInfo.uselimit.ucHour );	
				g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].Set_m_ucMinute( successgrabitem->ItemInfo.uselimit.ucMinute );
			}

			if( g_pInventory_Window )
			{
				if( g_pInventory_Window->GetShowStatus() )
				{
					g_pLocalUser->m_UserItemList[successgrabitem->result[i].slot].m_bNewItem = TRUE;
					g_pInventory_Window->m_bDrawGrabItem = TRUE;
				}
			}
		}

		PlayItemEquipSound( successgrabitem->ItemInfo.cType, successgrabitem->ItemInfo.sID );
	}
	
	if( success )
	{
#ifdef _XDEF_CHRISTMASEVENT
		// 크리스 마스 이벤트 아이템을 획득할 경우 Effect와 sound 출력
		if(successgrabitem->ItemInfo.cType == _XGI_FC_RESOURCE && successgrabitem->ItemInfo.sID == 463)
		{
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager,_XDEF_CONDITON031, &g_pLocalUser->m_ModelDescriptor.m_Position, NULL, TRUE);
			_XPlayInterfaceSound(ID_SR_INTERFACE_CHRISTMAS_WAV);
		}
#endif
		
#ifdef _XTS_NEWPARTYCOUNTER
		g_QuestScriptManager.CheckAllItemCounter(successgrabitem->ItemInfo.cType,successgrabitem->ItemInfo.sID);
#else
		bool checkitem = false;
		// 아이템 카운터에 관련된 아이템이 들어오는 경우 갱신시킨다.		
		for( int i = 0; i < ID_QUEST_SYNC; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
		{
			if( checkitem ) break;
			
			_XQuestScriptItem* currentquest = NULL;
			if( i != 5 )
			{
				if( g_QuestScriptManager.m_ProgressSubQuestID[i] != -1)
				{
					int questid = g_QuestScriptManager.m_ProgressSubQuestID[i];
					
					if( questid >= 10000 && questid < 20000  )
					{
						map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
						if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
							currentquest = iter_quest->second;
					}
					else
					{

						map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
						if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
							currentquest = iter_quest->second;
					}
					
				}//if( g_QuestScriptManager.m_ProgressSubQuestID[i] != -1)
			}
			else
			{
				if( g_QuestScriptManager.m_bProgressMainQuest )
				{
					int questid = g_QuestScriptManager.m_CurrentMainQuestID;
					map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
					if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
						currentquest = iter_quest->second;
				}				
			}

			if( currentquest )
			{
				for( int itemcounterindex = 0; itemcounterindex < 8; ++itemcounterindex )
				{
					if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType != -1 )
					{
						if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType == successgrabitem->ItemInfo.cType 
							&& currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID == successgrabitem->ItemInfo.sID )
						{
							
							TCHAR	buffer[256];
							memset(buffer, 0, sizeof(TCHAR)*256);
							
							int itemtotalcount = 0;
							for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
							{
								if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == successgrabitem->ItemInfo.cType
									&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == successgrabitem->ItemInfo.sID)
									itemtotalcount += g_pLocalUser->m_UserItemList[i].m_ucCount;														
							}

						#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
							for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; ++i)
							{
								if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == successgrabitem->ItemInfo.cType
									&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == successgrabitem->ItemInfo.sID)
									itemtotalcount += g_pLocalUser->m_UserItemList[i].m_ucCount;														
							}
						#endif
							
							if( itemtotalcount != 0 )
							{
								if( currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint == 0 )
								{
									_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
										currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
										itemtotalcount );
								}
								else if( itemtotalcount < currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
								{
									_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
										currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
										itemtotalcount,
										currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint );
								}
								else if( itemtotalcount >= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
								{
									if( itemcountbeforetrade < currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
									{
										// add word order
										_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), 
											currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
											currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint,
											currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint);
									}
								}
								
								//if( itemtotalcount <= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
								{
									if( buffer[0] != 0 )
									{
										int strlength = strlen(buffer);
										
										if(g_QuestScriptManager.m_lpszQuestSystemMessage )
										{
											delete g_QuestScriptManager.m_lpszQuestSystemMessage;
											g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
										}
										
										if(strlength > 0)
										{
											g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
											g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
											strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
										}
										
										g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
										g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
									}		
								}	
								
								
								// 현재 퀘스트 창에 열려있는 퀘스트 관련 아이템이 들어오는 경우 퀘스트 창을 갱신시킨다.								
								_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
								if( pMainFrame_Window && pMainFrame_Window->GetShowStatus() )
								{
									if( g_pQuest_Window )
									{
										if( g_pQuest_Window->GetShowStatus() )
										{
											g_pQuest_Window->ParseQuestDescString();
										}
									}
								}								
							}
							
							checkitem = true;
							break;
						}
					}
				}
			}//	if( currentquest )
		}//for( int i = 0; i < ID_QUEST_SYNC-1; i++ )
		
		// 재료수집 창에 켜져 있을 때는 실시간으로 재료 현황을 갱신한다.
		if( g_pLifeSkill_Window && g_pLifeSkill_Window->GetShowStatus() )
		{
			if(g_pLifeSkill_Window->m_SelectedTab == _XLIFETAB_LIFECLASS2)
				g_pLifeSkill_Window->SetListResourceNeedItem(FALSE);
		}
#endif
	}
	
	_XGameItem* pItem = g_GameItemlist.FindWorldItem(successgrabitem->ItemWorldID);
	if(pItem)
	{
		LPTSTR param1 = _XGameItem::GetItemName(pItem->m_cType, pItem->m_sID);
		int    param2 = pItem->m_ucCount;

		InsertChatString( g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_GETSTATUS, param1, &param2 ),
 						 _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
		_XPlayInterfaceSound(ID_SR_INTERFACE_PICK_ITEM_001_WAV);
	}
	
	g_GameItemlist.RemoveWorldItem(successgrabitem->ItemWorldID);
	g_pLocalUser->m_PickItem =  NULL;
}

void _XNetwork::MSGMoneyGrabSuccess(MSG_MONEY_GRAB_SUCCESS* successgrabmoney)
{
	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(messagestring));
	_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_GETMONEY), successgrabmoney->uiMoney - g_pLocalUser->m_Money);
	InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
	_XPlayInterfaceSound(ID_SR_INTERFACE_PICK_ITEM_001_WAV);
	
	g_pLocalUser->m_Money = successgrabmoney->uiMoney;	
	g_GameItemlist.RemoveWorldItem(successgrabmoney->iItemWorldID);
}

void _XNetwork::MSGInvenDrop(MSG_INVEN_DROP* inventoryitemdrop)
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
    if(inventoryitemdrop->cInvenIndex == 255)
	{
		TCHAR messagestring[256];
		memset(messagestring, 0, sizeof(messagestring));
		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_WARNDROPITEM));
		InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	}
	else if(inventoryitemdrop->cInvenIndex == 254)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1696), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	}
#else
	if(inventoryitemdrop->cInvenIndex == -1)
	{
		TCHAR messagestring[256];
		memset(messagestring, 0, sizeof(messagestring));
		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_WARNDROPITEM));
		InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	}
	else if(inventoryitemdrop->cInvenIndex == -2)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1696), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	}
#endif
	else
	{	
		PlayItemEquipSound( g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_cType(), 
							g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_sID() );

		int invenitemtype	= g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_cType();
		int invenitemid		= g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_sID();
		
		// 셀수 있는 아이템 이면
		if(g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].IsStackable())
		{
			g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Set_m_ucCount(g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_ucCount() - inventoryitemdrop->uiCount);
			if(g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_ucCount() <= 0)
			{
				// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
				g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_cType(), g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_sID() );

				g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Reset();
				g_pQuickSlot_Window->DeleteQuickSlot((u_char)inventoryitemdrop->cInvenIndex);
				
			}
		}
		else
		{
			// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
			g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_cType(), g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_sID() );

			// 경고 Icon이 있었던 경우 Icon 삭제
			if(inventoryitemdrop->cInvenIndex >= _XINVENTORY_SLOTTYPE_AMOR && inventoryitemdrop->cInvenIndex <= _XINVENTORY_SLOTTYPE_WEAPON_2)
			{
				if(g_pState_Window)
				{
					if(g_pState_Window->FindPlayerStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_sID(), 
						g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_cType(), inventoryitemdrop->cInvenIndex))
					{
						g_pState_Window->DeletePlayerStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_sID(), 
							g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Get_m_cType(), inventoryitemdrop->cInvenIndex);
					}
				}
			}
			if( inventoryitemdrop->cInvenIndex == _XINVENTORY_SLOTTYPE_WEAPON_1 )
			{			
				int effectid = _XGameItem::GetInchantEffectId(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType(),
					g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID(), g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucStrength() );
				
				if( effectid > 0 )
				{
					g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(effectid);
				}
				g_pLocalUser->m_CharacterInfo.weaponStrength = 0;
			}		
			g_pLocalUser->m_UserItemList[inventoryitemdrop->cInvenIndex].Reset();
			g_pQuickSlot_Window->DeleteQuickSlot((u_char)inventoryitemdrop->cInvenIndex);
		}

        if( inventoryitemdrop->cInvenIndex == _XINVENTORY_SLOTTYPE_BACKPACK1 )
		{
			g_pLocalUser->m_cBackPackExtend = inventoryitemdrop->cBackPackExtend;
        #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
            g_pInventory_Window->SetTabButtonPosition();
            g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
			g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 136, 0, 168, 32 );
        #else
			g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 97,  247, 138 );
			g_pInventory_Window->m_BaseTabButton[1]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 205, 97,  247, 138 );
        #endif
		}

		if( inventoryitemdrop->cInvenIndex == _XINVENTORY_SLOTTYPE_BACKPACK2 )
		{
			g_pLocalUser->m_cBackPackExtend = inventoryitemdrop->cBackPackExtend;			
        #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
            g_pInventory_Window->SetTabButtonPosition();
            g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
			g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 136, 0, 168, 32 );
        #else
			g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 97,  247, 138 );
			g_pInventory_Window->m_BaseTabButton[2]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 205, 97,  247, 138 );
        #endif
		}

	#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
		if( inventoryitemdrop->cInvenIndex == _XINVENTORY_SLOTTYPE_CASHBACKPACK1 )
		{
            //아이템을 내릴 땐 두번째 행낭이 없어야 함
			g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; //장착한걸 내리니깐 캐시없음.
            g_pInventory_Window->SetTabButtonPosition();
			g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
			g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 136, 0, 168, 32 );
		}
		if( inventoryitemdrop->cInvenIndex == _XINVENTORY_SLOTTYPE_CASHBACKPACK2 )
		{
            //기간제라 1번째 행낭이 사라질 수 있다
            if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 )
                g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; //장착한걸 내리니깐.. 캐시 1만 남은상태
            else
                g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; //캐시 1 행낭도 없다..

            g_pInventory_Window->SetTabButtonPosition();
			g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
			g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 136, 0, 168, 32 );
		}
	#endif

		if(inventoryitemdrop->cInvenIndex >= _XINVENTORY_SLOTTYPE_AMOR && inventoryitemdrop->cInvenIndex <= _XINVENTORY_SLOTTYPE_WEAPON_2)
		{
			DWORD lparam = inventoryitemdrop->cInvenIndex * 100000;
			ChangeModel(lparam, 0 );	// id가 0으로 계산되므로 아이템 벗기기

			// 아이템 상태효과 update
			g_pLocalUser->UpdateLocalUserAttackLevel();

			// 경고 Icon이 있었던 경우 Icon 삭제
			if(g_pState_Window)
			{
				if(g_pState_Window->FindPlayerStateList(_XST_ITEM, invenitemid, invenitemtype, inventoryitemdrop->cInvenIndex))
				{
					g_pState_Window->DeletePlayerStateList(_XST_ITEM, invenitemid, invenitemtype, inventoryitemdrop->cInvenIndex);
				}
			}
			
			if(inventoryitemdrop->cInvenIndex == _XINVENTORY_SLOTTYPE_AMOR)
			{
				g_pLocalUser->m_cAmorPocketNum = 0;
			}
			if(inventoryitemdrop->cInvenIndex == _XINVENTORY_SLOTTYPE_PANTS)
			{
				g_pLocalUser->m_cPantsPocketNum = 0;
			}
		}
		
#ifdef _XTS_NEWPARTYCOUNTER
		g_QuestScriptManager.CheckAllItemCounter(invenitemtype,invenitemid);
#else
		// 현재 퀘스트 창에 열려있는 퀘스트 관련 아이템이 들어오는 경우 퀘스트 창을 갱신시킨다.								
		_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
		if( pMainFrame_Window && pMainFrame_Window->GetShowStatus() )
		{
			if( g_pQuest_Window )
			{
				if( g_pQuest_Window->GetShowStatus() )
				{
					g_pQuest_Window->ParseQuestDescString();
				}
			}
		}
#endif
		
		// 재료수집 창에 켜져 있을 때는 실시간으로 재료 현황을 갱신한다.
		if( g_pLifeSkill_Window && g_pLifeSkill_Window->GetShowStatus() )
		{
			if(g_pLifeSkill_Window->m_SelectedTab == _XLIFETAB_LIFECLASS2)
				g_pLifeSkill_Window->SetListResourceNeedItem(FALSE);
		}

		if( inventoryitemdrop->cBackPackExtend == 10 )
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3337), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//아이템이 삭제되었습니다
		}
		else
		{
			TCHAR messagestring[256];
			memset(messagestring, 0, sizeof(messagestring));
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_DROPITEM));
			InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		}
	}
}

#ifdef _XTS_ITEM_OPTIMIZATION
void _XNetwork::MSGItemDrop(MSG_ITEM_DROP* itemdrop)
{
	if(itemdrop->cNoItems > 0)
	{
		_XGameItem* newitem;
		D3DXVECTOR3 position;
		FLOAT		additionalheight;

		for(int i = 0 ; i < itemdrop->cNoItems ; ++i)
		{
			newitem = new _XGameItem;
			position.x = itemdrop->items[i].fX;
			position.z = itemdrop->items[i].fZ;

			_XOBBData* pOBB = NULL;
			FLOAT obbheight = g_LodTerrain.GetTerrainAndRideOBBHeight(position, pOBB);
			position.y = obbheight;
			additionalheight = 0.0f;

			_XAnimMeshAbstractClass* pModel = NULL;

			switch( GetEqualItemType(itemdrop->items[i].cType) )
			{
			case _XGI_FC_WEAPON :
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(itemdrop->items[i].cType);
					if( WeaponItem )
						pModel = g_ItemModel_Weapon[0][WeaponItem[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_CLOTHES :
				{
					_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(itemdrop->items[i].cType);
					if( ClothesItem )
					{
						int extravalue = 0;														
						if( ClothesItem[itemdrop->items[i].S_detail.sID].cSexType == _XGENDER_WOMAN )
							extravalue = g_Model_GenderSeparateIndex;
						
						switch(ClothesItem[itemdrop->items[i].S_detail.sID].cSecondType)
						{
						case _XGI_SC_CLOTHES_UPPER :
							{
								pModel = g_UserModel_UBodyType[ClothesItem[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
								additionalheight -= 0.65f;
							}
							break;
						case _XGI_SC_CLOTHES_LOWER :
							{
								pModel = g_UserModel_LBodyType[ClothesItem[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
								additionalheight -= 0.3f;
							}
							break;
						case _XGI_SC_CLOTHES_GLOVES :
							{
								pModel = g_UserModel_LHandType[ClothesItem[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
								additionalheight = 0.15f;
							}
							break;
						case _XGI_SC_CLOTHES_SHOES :
							{
								pModel = g_UserModel_LFootType[ClothesItem[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
								additionalheight = 0.0f;
							}
							break;
						case _XGI_SC_CLOTHES_CAP   :
							{
								pModel = g_UserModel_Cap[ClothesItem[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
								additionalheight = 0.0f;
							}
							break;
						case _XGI_SC_CLOTHES_BACKPACK   :
							{
								pModel = g_UserModel_BackPack[ClothesItem[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
								additionalheight = 0.0f;
							}
							break;
						case _XGI_SC_CLOTHES_MASK :
							{
								
							}
							break;
						default :
							break;
						}
					}					
				}
				break;			
			case _XGI_FC_BOOK :
				{
					pModel = g_ItemModel_MartialBook[g_BookItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_POTION :
				{
					pModel = g_ItemModel_Potion[g_PotionItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_CONSUMING : // 2005.04.09 oneway48 insert : 술 종류
				{
					pModel = g_ItemModel_JewelBox[0];// 임시로 보석 상자 모델을 넣어둔다.
				}
				break;
			case _XGI_FC_MONEY :
				{
					pModel = g_ItemModel_JewelBox[2];
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					pModel = g_ItemModel_ETC[g_ResourceItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];					
				}
				break;
			case _XGI_FC_ACCESSORY :
				{
					pModel = g_ItemModel_JewelBox[0];//g_ItemModel_JewelBox[itemdrop->items[i].S_detail.sID];
				}
				break;
			case _XGI_FC_LIFE :
				{
					pModel = g_ItemModel_JewelBox[2];
				}
				break;
			case _XGI_FC_SOCKET :
				{
					pModel = g_ItemModel_JewelBox[2];
				}
				break;
			case _XGI_FC_ELIXIR :
				{
					pModel = g_ItemModel_Elixir[g_ElixirItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_BOX :
				{
					pModel = g_ItemModel_JewelBox[g_BoxItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_BOXKEY :
				{
					pModel = g_ItemModel_JewelBox[g_BoxKeyItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			default :
				break;
			}

			if(!pModel)
			{
				_XDWPACKETLOG("WARNING : invalid item model");
				delete newitem;
			}
			else
			{
				newitem->Create(itemdrop->items[i].iItemWorldID, position, additionalheight, pModel);

				newitem->m_cType		= itemdrop->items[i].cType;
				newitem->m_cSecond		= itemdrop->items[i].S_detail.cSecond;
				newitem->m_sID			= itemdrop->items[i].S_detail.sID;
				newitem->m_cIdentity	= itemdrop->items[i].S_detail.cIdentify;
				newitem->m_usOwnerID	= itemdrop->usUniqueID;
				newitem->m_InsideFrustum = TRUE;
				newitem->m_LastReceiveMessageTime = g_LocalSystemTime;

				switch(newitem->m_cType)
				{
				case _XGI_FC_MONEY :
					{
						newitem->m_uiCount = itemdrop->items[i].uiCount;
					}
					break;
				default :
					{
						newitem->m_ucCount = itemdrop->items[i].ucCount;
					}
					break;
				}

				g_GameItemlist.insertItem(newitem);
			}
		}
	}
}
#else
void _XNetwork::MSGItemDrop(MSG_ITEM_DROP* itemdrop)
{
	if(itemdrop->cNoItems > 0)
	{
		_XGameItem* newitem;
		D3DXVECTOR3 position;
		FLOAT		additionalheight;

		for(int i = 0 ; i < itemdrop->cNoItems ; ++i)
		{
			newitem = new _XGameItem;
			position.x = itemdrop->items[i].fX;
			position.z = itemdrop->items[i].fZ;

			_XOBBData* pOBB = NULL;
			FLOAT obbheight = g_LodTerrain.GetTerrainAndRideOBBHeight(position, pOBB);
			position.y = obbheight;
			additionalheight = 0.0f;

			_XAnimMeshAbstractClass* pModel = NULL;

			switch(itemdrop->items[i].cType)
			{
			case _XGI_FC_WEAPON :
				{
					pModel = g_ItemModel_Weapon[0][g_WeaponItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_CLOTHES :
				{
					int extravalue = 0;														
					if( g_ClothesItemProperty[itemdrop->items[i].S_detail.sID].cSexType == _XGENDER_WOMAN )
						extravalue = g_Model_GenderSeparateIndex;

					switch(g_ClothesItemProperty[itemdrop->items[i].S_detail.sID].cSecondType)
					{
					case _XGI_SC_CLOTHES_UPPER :
						{
							pModel = g_UserModel_UBodyType[g_ClothesItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight -= 0.65f;
						}
						break;
					case _XGI_SC_CLOTHES_LOWER :
						{
							pModel = g_UserModel_LBodyType[g_ClothesItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight -= 0.3f;
						}
						break;
					case _XGI_SC_CLOTHES_GLOVES :
						{
							pModel = g_UserModel_LHandType[g_ClothesItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight = 0.15f;
						}
						break;
					case _XGI_SC_CLOTHES_SHOES :
						{
							pModel = g_UserModel_LFootType[g_ClothesItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_CAP   :
						{
							pModel = g_UserModel_Cap[g_ClothesItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_BACKPACK   :
						{
							pModel = g_UserModel_BackPack[g_ClothesItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_MASK :
						{

						}
						break;
					default :
						break;
					}
				}
				break;
			case _XGI_FC_WEAPON2 :
				{
					pModel = g_ItemModel_Weapon[0][g_WeaponItemProperty2[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_CLOTHES2 :
				{
					int extravalue = 0;														
					if( g_ClothesItemProperty2[itemdrop->items[i].S_detail.sID].cSexType == _XGENDER_WOMAN )
						extravalue = g_Model_GenderSeparateIndex;

					switch(g_ClothesItemProperty2[itemdrop->items[i].S_detail.sID].cSecondType)
					{
					case _XGI_SC_CLOTHES_UPPER :
						{
							pModel = g_UserModel_UBodyType[g_ClothesItemProperty2[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight -= 0.65f;
						}
						break;
					case _XGI_SC_CLOTHES_LOWER :
						{
							pModel = g_UserModel_LBodyType[g_ClothesItemProperty2[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight -= 0.3f;
						}
						break;
					case _XGI_SC_CLOTHES_GLOVES :
						{
							pModel = g_UserModel_LHandType[g_ClothesItemProperty2[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight = 0.15f;
						}
						break;
					case _XGI_SC_CLOTHES_SHOES :
						{
							pModel = g_UserModel_LFootType[g_ClothesItemProperty2[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_CAP   :
						{
							pModel = g_UserModel_Cap[g_ClothesItemProperty2[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_BACKPACK   :
						{
							pModel = g_UserModel_BackPack[g_ClothesItemProperty2[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_MASK :
						{

						}
						break;
					default :
						break;
					}
				}
				break;
			case _XGI_FC_BOOK :
				{
					pModel = g_ItemModel_MartialBook[g_BookItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_POTION :
				{
					pModel = g_ItemModel_Potion[g_PotionItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_CONSUMING : // 2005.04.09 oneway48 insert : 술 종류
				{
					pModel = g_ItemModel_JewelBox[0];// 임시로 보석 상자 모델을 넣어둔다.
				}
				break;
			case _XGI_FC_MONEY :
				{
					pModel = g_ItemModel_JewelBox[2];
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					pModel = g_ItemModel_ETC[g_ResourceItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];					
				}
				break;
			case _XGI_FC_ACCESSORY :
				{
					pModel = g_ItemModel_JewelBox[0];//g_ItemModel_JewelBox[itemdrop->items[i].S_detail.sID];
				}
				break;
			case _XGI_FC_LIFE :
				{
					pModel = g_ItemModel_JewelBox[2];
				}
				break;
			case _XGI_FC_ELIXIR :
				{
					pModel = g_ItemModel_Elixir[g_ElixirItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_BOX :
				{
					pModel = g_ItemModel_JewelBox[g_BoxItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_BOXKEY :
				{
					pModel = g_ItemModel_JewelBox[g_BoxKeyItemProperty[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_WEAPON3 :
				{
					pModel = g_ItemModel_Weapon[0][g_WeaponItemProperty3[itemdrop->items[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_CLOTHES3 :
				{
					int extravalue = 0;														
					if( g_ClothesItemProperty3[itemdrop->items[i].S_detail.sID].cSexType == _XGENDER_WOMAN )
						extravalue = g_Model_GenderSeparateIndex;

					switch(g_ClothesItemProperty3[itemdrop->items[i].S_detail.sID].cSecondType)
					{
					case _XGI_SC_CLOTHES_UPPER :
						{
							pModel = g_UserModel_UBodyType[g_ClothesItemProperty3[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight -= 0.65f;
						}
						break;
					case _XGI_SC_CLOTHES_LOWER :
						{
							pModel = g_UserModel_LBodyType[g_ClothesItemProperty3[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight -= 0.3f;
						}
						break;
					case _XGI_SC_CLOTHES_GLOVES :
						{
							pModel = g_UserModel_LHandType[g_ClothesItemProperty3[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight = 0.15f;
						}
						break;
					case _XGI_SC_CLOTHES_SHOES :
						{
							pModel = g_UserModel_LFootType[g_ClothesItemProperty3[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_CAP   :
						{
							pModel = g_UserModel_Cap[g_ClothesItemProperty3[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_BACKPACK   :
						{
							pModel = g_UserModel_BackPack[g_ClothesItemProperty3[itemdrop->items[i].S_detail.sID].sModelIndex + extravalue];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_MASK :
						{

						}
						break;
					default :
						break;
					}
				}
				break;
			default :
				break;
			}

			if(!pModel)
			{
				_XDWPACKETLOG("WARNING : invalid item model");
				delete newitem;
			}
			else
			{
				newitem->Create(itemdrop->items[i].iItemWorldID, position, additionalheight, pModel);

				newitem->m_cType		= itemdrop->items[i].cType;
				newitem->m_cSecond		= itemdrop->items[i].S_detail.cSecond;
				newitem->m_sID			= itemdrop->items[i].S_detail.sID;
				newitem->m_cIdentity	= itemdrop->items[i].S_detail.cIdentify;
				newitem->m_usOwnerID	= itemdrop->usUniqueID;
				newitem->m_InsideFrustum = TRUE;
				newitem->m_LastReceiveMessageTime = g_LocalSystemTime;

				switch(newitem->m_cType)
				{
				case _XGI_FC_MONEY :
					{
						newitem->m_uiCount = itemdrop->items[i].uiCount;
					}
					break;
				default :
					{
						newitem->m_ucCount = itemdrop->items[i].ucCount;
					}
					break;
				}

				g_GameItemlist.insertItem(newitem);
			}
		}
	}
}
#endif

// 2005.02.23->hotblood ------------------------------------------------------------------------------------------------=
void _XNetwork::MSGTempInven(MSG_TEMP_INVEN* tempinven)
{
#ifndef _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// temp inven mod가 아닐때만

	_XWindow_Pocket* pPocket_Window = (_XWindow_Pocket*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POCKET);

	if( tempinven->cTo )	// 템프 인벤
	{
		if( !pPocket_Window->GetShowStatus() )
		{
			pPocket_Window->ResetPocketItem();
			pPocket_Window->ShowWindow(TRUE);
			g_pInventory_Window->SetPocketWindowPtr(pPocket_Window);			
		}

		for( int i = 0; i < _TEMP_INVEN_MAX_COUNT_; ++i )
		{
			if( pPocket_Window )
			{
				pPocket_Window->InsertAccessoryItem(i, tempinven->TInven[i].sID, tempinven->TInven[i].cSecond );
			}
		}		
	}
	else					// 인벤
	{
		for( int i = 0; i < _TEMP_INVEN_MAX_COUNT_; ++i )
		{
			if( tempinven->IndexPair[i].cInvenIdx >= _XINVENTORY_SLOTTYPE_POCKETSTART )
			{
				if(tempinven->IndexPair[i].cInvenIdx >= _XINVENTORY_SLOTTYPE_TOTALCOUNT)
				{
					// temp inven 에 넣는다
					if(pPocket_Window)
					{
						pPocket_Window->InsertAccessoryItem(i, g_pLocalUser->m_UserItemList[tempinven->IndexPair[i].cAccessoryIdx].Get_m_sID(), g_pLocalUser->m_UserItemList[tempinven->IndexPair[i].cAccessoryIdx].m_cSecond);
					}
				}
				else
				{
					g_pLocalUser->m_UserItemList[tempinven->IndexPair[i].cInvenIdx].Copy( g_pLocalUser->m_UserItemList[tempinven->IndexPair[i].cAccessoryIdx] );
					g_pLocalUser->m_UserItemList[tempinven->IndexPair[i].cAccessoryIdx].Reset();
				}
			}
		}
	}

#endif
}
	
void _XNetwork::MSGTempInvenCtrlResult(MSG_TEMP_INVEN_CTRL_RESULT* tempinvenctrlresult)
{
	if( tempinvenctrlresult->cInvenIdx == 0 && tempinvenctrlresult->cTIdx == 0 )	// 템프인벤창 제거
	{		
		_XWindow_Pocket* pPocket_Window = (_XWindow_Pocket*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POCKET);
		if( pPocket_Window )			
		{
			if( pPocket_Window->GetShowStatus() )
				pPocket_Window->ShowWindow(FALSE);
		}
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1697),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	}
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	else if( tempinvenctrlresult->cInvenIdx != 255 )
#else
	else if( tempinvenctrlresult->cInvenIdx != -1 )
#endif
	{
		if( g_pLocalUser->m_UserItemList[tempinvenctrlresult->cInvenIdx].Get_m_sID() <= 0 )
		{
			_XWindow_Pocket* pPocket_Window = (_XWindow_Pocket*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POCKET);
			if( pPocket_Window )
			{
				pPocket_Window->CopyPocketItem( tempinvenctrlresult->cInvenIdx, tempinvenctrlresult->cTIdx );
			}
		}
	}
	else
	{
		// 실패 처리
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1698),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	}
}

void _XNetwork::MSGTempInvenMove(MSG_TEMP_INVEN_MOVE* invenmove)
{
	for( int i = 0; i < _TEMP_INVEN_MAX_COUNT_; ++i )
	{
		if(invenmove->IndexPair[i].cAccessoryIdx >= _XINVENTORY_SLOTTYPE_ACCESSORY_1 && invenmove->IndexPair[i].cAccessoryIdx <= _XINVENTORY_SLOTTYPE_ACCESSORY_8)
		{
			if(invenmove->IndexPair[i].cInvenIdx >= _XINVENTORY_SLOTTYPE_TOTALCOUNT)
			{
				// temp inven에 넣는다
				_XWindow_Pocket* pPocket_Window = (_XWindow_Pocket*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POCKET);
				if(pPocket_Window)
				{
					if( !pPocket_Window->GetShowStatus() )
					{
						pPocket_Window->ResetPocketItem();
						pPocket_Window->ShowWindow(TRUE);
						g_pInventory_Window->SetPocketWindowPtr(pPocket_Window);			
					}

					pPocket_Window->m_AccessoryItemList[invenmove->IndexPair[i].cInvenIdx - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Copy(g_pLocalUser->m_UserItemList[invenmove->IndexPair[i].cAccessoryIdx]);
					g_pLocalUser->m_UserItemList[invenmove->IndexPair[i].cAccessoryIdx].Reset();
				}
			}
			else if(invenmove->IndexPair[i].cInvenIdx >= _XINVENTORY_SLOTTYPE_POCKETSTART)
			{
				// inven에 넣는다
				g_pLocalUser->m_UserItemList[invenmove->IndexPair[i].cInvenIdx].Copy( g_pLocalUser->m_UserItemList[invenmove->IndexPair[i].cAccessoryIdx] );
				g_pLocalUser->m_UserItemList[invenmove->IndexPair[i].cAccessoryIdx].Reset();
			}
		}
	}
}

void _XNetwork::MSGTempInvenInfo(MSG_TEMP_INVEN_INFO* inveninfo)
{
	for(int i = 0 ; i < _TEMP_INVEN_MAX_COUNT_ ; ++i)
	{
		if(inveninfo->TInven[i].sID > 0)
		{
			// temp inven에 넣어준다.
			_XWindow_Pocket* pPocket_Window = (_XWindow_Pocket*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POCKET);
			if(pPocket_Window)
			{
				if( !pPocket_Window->GetShowStatus() )
				{
					pPocket_Window->ResetPocketItem();
					pPocket_Window->ShowWindow(TRUE);
					g_pInventory_Window->SetPocketWindowPtr(pPocket_Window);			
				}
                //Author : 양희왕 //breif : 템프인벤에 빠져있는 정보를 채워넣는다

				pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_cType(inveninfo->TInven[i].cType);
                pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_cSecond(inveninfo->TInven[i].cSecond);                
                pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_cThird(_XGameItem::GetThird(inveninfo->TInven[i].cType, inveninfo->TInven[i].sID));  
				pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_sID( inveninfo->TInven[i].sID );
				pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_ucCount(inveninfo->TInven[i].Item_Money.S_NormalItem.ucCount);

				pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_usMaxDur( _XGameItem::GetMaxDurability(inveninfo->TInven[i].cType, inveninfo->TInven[i].sID) );
				pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_usCurDur( inveninfo->TInven[i].Item_Money.S_NormalItem.usCurDur );

				pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_ucSocketCount( inveninfo->TInven[i].Item_Money.S_NormalItem.ucSocketCount );
				if( inveninfo->TInven[i].Item_Money.S_NormalItem.ucSocketCount > 0 )
				{
					for( int i = 0; i < 4; ++i )
						pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_sSocket( i, inveninfo->TInven[i].sSocket[i] );
				}
				pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].m_ucFlag1 = inveninfo->TInven[i].uselimit.ucFlag1;
#ifndef _XDEF_SMELT_ITEM
				pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].m_ucFlag2	= inveninfo->TInven[i].uselimit.ucFlag2;	
				pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].m_ucFlag3	= inveninfo->TInven[i].uselimit.ucFlag3;
#endif
				pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].m_usValue1	= inveninfo->TInven[i].uselimit.usValue1;

				if( inveninfo->TInven[i].uselimit.ucYear > 0 )
				{						
					pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_ucYear(inveninfo->TInven[i].uselimit.ucYear);	
					pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_ucMonth(inveninfo->TInven[i].uselimit.ucMonth);	
					pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_ucDay(inveninfo->TInven[i].uselimit.ucDay);	
					pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_ucHour(inveninfo->TInven[i].uselimit.ucHour);	
					pPocket_Window->m_AccessoryItemList[inveninfo->TInven[i].cSlot - _XINVENTORY_SLOTTYPE_TOTALCOUNT].Set_m_ucMinute(inveninfo->TInven[i].uselimit.ucMinute);
				}
			}
		}
	}
}


// ---------------------------------------------------------------------------------------------------------------------=

void _XNetwork::MSGItemEquipChange(MSG_EQUIP_CHANGE* itemequip)
{
	if(itemequip->usUniqueID != g_pLocalUser->m_Uniqueid)
	{				
		_XUser* pUser = g_Userlist.FindUser(itemequip->usUniqueID);
		
		if(pUser)
		{
			// 다른 유저의 아이템 장착 처리					
			switch(itemequip->cIndex)
			{
			case _XINVENTORY_SLOTTYPE_AMOR :
				{
					pUser->m_CharacterInfo.modeldescript.upperbodyCategory = (_XGI_FirstCategory)itemequip->cType;
					pUser->m_CharacterInfo.modeldescript.upperbody = itemequip->sID;
				}
				
				break;
			case _XINVENTORY_SLOTTYPE_PANTS :
				{
					pUser->m_CharacterInfo.modeldescript.lowerbodyCategory = (_XGI_FirstCategory)itemequip->cType;
					pUser->m_CharacterInfo.modeldescript.lowerbody = itemequip->sID;
				}
				
				break;
			case _XINVENTORY_SLOTTYPE_CAP :
				{
					pUser->m_CharacterInfo.modeldescript.capCategory = (_XGI_FirstCategory)itemequip->cType;
					pUser->m_CharacterInfo.modeldescript.cap = itemequip->sID;
				}				
				break;
			case _XINVENTORY_SLOTTYPE_ADAE :
				{
					pUser->m_CharacterInfo.modeldescript.adaeCategory = (_XGI_FirstCategory)itemequip->cType;
					pUser->m_CharacterInfo.modeldescript.adae = itemequip->sID;
				}				
				break;					
			case _XINVENTORY_SLOTTYPE_SHOES :
				{
					pUser->m_CharacterInfo.modeldescript.lfootCategory = (_XGI_FirstCategory)itemequip->cType;
					pUser->m_CharacterInfo.modeldescript.rfootCategory = (_XGI_FirstCategory)itemequip->cType;
					pUser->m_CharacterInfo.modeldescript.lfoot = itemequip->sID;
					pUser->m_CharacterInfo.modeldescript.rfoot = itemequip->sID;
				}				
				break;					
			case _XINVENTORY_SLOTTYPE_GAP_UPPER :
				break;
			case _XINVENTORY_SLOTTYPE_GAP_LOWER :		
				break;
			case _XINVENTORY_SLOTTYPE_BACKPACK1 :
				{
					pUser->m_CharacterInfo.modeldescript.backpackCategory = (_XGI_FirstCategory)itemequip->cType;
					pUser->m_CharacterInfo.modeldescript.backpack = itemequip->sID;
				}				
				break;
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장
			case _XINVENTORY_SLOTTYPE_CASHBACKPACK1 :
				break;
			case _XINVENTORY_SLOTTYPE_CASHBACKPACK2 :
				break;
		#endif
			case _XINVENTORY_SLOTTYPE_WEAPON_1 :
				{
					int effectid = _XGameItem::GetInchantEffectId( pUser->m_CharacterInfo.modeldescript.item1Category,
									pUser->m_CharacterInfo.modeldescript.item1, pUser->m_CharacterInfo.weaponStrength );
		
					if( effectid > 0 )
					{
						pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(effectid);
					}

					pUser->m_CharacterInfo.modeldescript.item1Category = (_XGI_FirstCategory)itemequip->cType;
					pUser->m_CharacterInfo.modeldescript.item1 = itemequip->sID;
					pUser->m_CharacterInfo.weaponStrength = itemequip->ucStrength;

					effectid = _XGameItem::GetInchantEffectId( pUser->m_CharacterInfo.modeldescript.item1Category,
						pUser->m_CharacterInfo.modeldescript.item1, pUser->m_CharacterInfo.weaponStrength );
					if( effectid > 0 )
					{
						pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
							effectid, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE );
					}

					if( itemequip->sID == 0 )
					{							
						pUser->m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
					}						
				}			
				break;
			case _XINVENTORY_SLOTTYPE_WEAPON_2 :
				
				pUser->m_CharacterInfo.modeldescript.item2 = itemequip->sID;
				if( itemequip->sID == 0 )
				{							
					//pUser->m_ModelDescriptor.m_ViewRightSwordEffect = FALSE;
				}						

				break;
			case _XINVENTORY_SLOTTYPE_MASK :
				{
					pUser->m_CharacterInfo.modeldescript.maskCategory = (_XGI_FirstCategory)itemequip->cType;
					pUser->m_CharacterInfo.modeldescript.mask = itemequip->sID;
				}
				break;
			}
			
			pUser->SetCharacterModel();
		}
	}
}

void _XNetwork::MSGMoneyDrop(MSG_MONEY_DROP* moneydrop)
{
	if(moneydrop->bSuccess)
	{
		g_pLocalUser->m_Money -= moneydrop->uiCount;
		
		TCHAR messagestring[256];
		memset(messagestring, 0, sizeof(messagestring));
		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1699), moneydrop->uiCount);
		InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);

		PlayItemEquipSound( _XGI_FC_MONEY, 0 );
	}
	else
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_CANNOTDROP), TRUE);
		_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
	}
}

void _XNetwork::MSGUseItem(MSG_USE_ITEM_RESULT* useitemresult)
{	
	if( g_pInventory_Window )
	{
		// gauge 바 제거
		_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
		if( pChargeGauge_Window )
		{
			if( pChargeGauge_Window->GetShowStatus() )
				pChargeGauge_Window->StartFadeOutMode();
		}
		
		// 툴팁 제거
		if( g_pDefaultTooltip_Window )
		{
			if( g_pDefaultTooltip_Window->GetShowStatus() )
				g_pDefaultTooltip_Window->ShowWindow( FALSE );
		}

		// 영약 복용 중이면 자세 복구
		if( g_pLocalUser->m_bInternalUse )
		{
			g_pLocalUser->m_CurrentInternalUseState = _XUSER_STATE_5;
		}
		else if( g_pLocalUser->m_bReadingBook )
		{
			g_pLocalUser->m_CurrentReadBookState = _XUSER_STATE_5;
		}

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
        VIEWPACKETLOG
			_XDWINPRINT("=== Item Use Result [Invennum:%d  Reason:%d]", useitemresult->ucInvenIndex, useitemresult->ucResult);

        switch(useitemresult->ucResult) 
		{
		case 216 : // -40
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );								
			}
			break;
		case 215 : // -41 
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1700), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );					
			}
			break;
		case 214 : //-42
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1701), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );					
			}
			break;
		case 213 : //-43 
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1702), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );					
			}
			break;
		case 212 : //-44 
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1703), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );					
			}
			break;
		case 211 : //-45 
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1704), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 228 : //-28 
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1705), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 232  :	//-24// -20: 성이 맞지않다
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1706), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 231 :	//-25 // -21: 자신이 먹은 영약보다 낮은 스텝의 영약을 먹었다.
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1707), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 230 : //-26	// -22: 자신이 먹은 영약보다 2레벨 높은 영약을 먹으려고 했다.
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1708), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 229 : //-27 	// -23: 중독 수치가 맥스치를 넘었다
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1709), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 234 : //-22//없는 KEY LPS를 쳤다.
		case 233 : //-23 //KEY LPS가 이미 내놓을 것을 내 놓았다. 
			break;
		case 235 : //-21 // 무공서 읽었는데 등급이 맞지않음
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3795), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//_T("직책 등급이 모자랍니다.")
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 236 : //-20
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1710), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 237 ://-19 //  쿨 다운 시간이 안맞는 경우.
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTUSE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 254 ://-2//  쿨 다운 시간이 안맞는 경우.
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTUSE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );

#if defined(ALL_RB_REBIRTH_USERITEM_071108_KUKURI) || defined(ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI)
				if(g_pLocalUser->GetMotionClass() == _XACTION_DIE)
				{
					// 죽어있을 경우 - 화타신단 먹었을 경우일듯..
					_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
					if(pRebirth_Window)
					{
						if(pRebirth_Window->GetShowStatus())
							pRebirth_Window->EnableWindow(TRUE);
					}
				}
#endif
			}
			break;
            
		case 238 : //-18 
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1711), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}				
			break;
		case 239 : //-17
		case 244  : //-12
		case 255 ://-1 넘어온 아이템이 이상한 경우.
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1712), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 253 : //-3
		case 252 : //-4
		case 245 : //-11
			{
				// 읽지 못하는 책
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1713), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 251 : //-5
		case 250 : //-6
		case 249 : //-7
		case 248 : //-8
		case 247 : //-9
		case 246 : //-10
		case 243 : //-13
		case 242 : //-14
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTABILITY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 241 : //-15
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1714), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case 240 : //-16
			{
				if( g_pLocalUser->m_CharacterInfo.gender == 0 )
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1715), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				else
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1716), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
#else
        VIEWPACKETLOG
			_XDWINPRINT("=== Item Use Result [Invennum:%d  Reason:%d]", useitemresult->ucInvenIndex, useitemresult->cResult);

		switch(useitemresult->cResult) 
		{
		case -40 :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );								
			}
			break;
		case -41 :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1700), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );					
			}
			break;
		case -42 :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1701), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );					
			}
			break;
		case -43 :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1702), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );					
			}
			break;
		case -44 :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1703), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );					
			}
			break;
		case -45 :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1704), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -28 :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1705), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -24 :	// -20: 성이 맞지않다
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1706), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -25 :	// -21: 자신이 먹은 영약보다 낮은 스텝의 영약을 먹었다.
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1707), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -26 :	// -22: 자신이 먹은 영약보다 2레벨 높은 영약을 먹으려고 했다.
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1708), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -27 :	// -23: 중독 수치가 맥스치를 넘었다
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1709), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -22 : //없는 KEY LPS를 쳤다.
		case -23 : //KEY LPS가 이미 내놓을 것을 내 놓았다. 
			break;
		case -21 : // 무공서 읽었는데 등급이 맞지않음
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3795), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//_T("직책 등급이 모자랍니다.")
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -20 :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1710), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -19 ://  쿨 다운 시간이 안맞는 경우.
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTUSE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -2 ://  쿨 다운 시간이 안맞는 경우.
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTUSE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );

#if defined(ALL_RB_REBIRTH_USERITEM_071108_KUKURI) || defined(ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI)
				if(g_pLocalUser->GetMotionClass() == _XACTION_DIE)
				{
					// 죽어있을 경우 - 화타신단 먹었을 경우일듯..
					_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
					if(pRebirth_Window)
					{
						if(pRebirth_Window->GetShowStatus())
							pRebirth_Window->EnableWindow(TRUE);
					}
				}
#endif
			}
			break;
            
		case -18 :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1711), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}				
			break;
		case -17:
		case -12 :
		case -1 :// 넘어온 아이템이 이상한 경우.
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1712), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -3 :
		case -4 :
		case -11 :
			{
				// 읽지 못하는 책
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1713), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -5 :
		case -6 :
		case -7 :
		case -8 :
		case -9 :
		case -10 :
		case -13 :
		case -14 :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTABILITY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -15 :
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1714), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
		case -16 :
			{
				if( g_pLocalUser->m_CharacterInfo.gender == 0 )
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1715), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				else
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1716), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				_XPlayInterfaceSound( ID_SR_INTERFACE_ERROR01_WAV );
			}
			break;
#endif
		case 1 :
			{
				if( g_pLocalUser->m_bKeyLPSProcess ) 
				{
					g_pLocalUser->m_bKeyLPSProcess = FALSE;
					//key LPS에 대한 처리 					
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1717), 
						_XGameItem::GetItemName(g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType(), 
						g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID()));
					InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					
				}
				else
				{
					g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
						_XDEF_PLAYERID_POTIONUSE,
						&g_pLocalUser->m_ModelDescriptor.m_Position );
					
					if( !(g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType() == _XGI_FC_POTION &&
						g_PotionItemProperty[g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID()].cSecondType == _XGI_SC_POTION_MASK) )
					{
						PlayItemUseSound( g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType(), 
											g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID() );
					}					
				}

				if( useitemresult->ucCount == 0 )
				{
					// Item 사용
					if(g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType() == _XGI_FC_POTION)	
					{
						if(g_PotionItemProperty[g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID()].cSecondType == _XGI_SC_POTION_HERT_IN ||
							g_PotionItemProperty[g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID()].cSecondType == _XGI_SC_POTION_HERT_OUT)
						{
							if(g_pState_Window)
								g_pState_Window->InsertStateListFromOther(_XST_ITEM, g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID(),
																 g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType(), useitemresult->ucInvenIndex);
						}
					}
					else if(g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType() == _XGI_FC_QUEST)
					{
						if( g_QuestItemProperty[g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID()].cSecondType == 1 )
						{
							if(g_pState_Window)
								g_pState_Window->InsertStateListFromOther(_XST_ITEM, g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID(),
								g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType(), useitemresult->ucInvenIndex);
						}
					}	
					else if(g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType() == _XGI_FC_BOOK)
					{
						if( g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cSecond() == 1 )	// 무공서
						{
							TCHAR messagestring[256];
							memset(messagestring, 0, sizeof(messagestring));
							_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1718), 
								_XGameItem::GetItemName(g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType(), 
								g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID()));
							InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
						}
					}

					// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
					g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType(), g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID()  );
					
					g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Reset();
					g_pQuickSlot_Window->DeleteQuickSlot(useitemresult->ucInvenIndex);					
				}
				else
				{
					// Item 사용
					if(g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType() == _XGI_FC_POTION)
					{
						if(g_PotionItemProperty[g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID()].cSecondType == _XGI_SC_POTION_HERT_IN ||
							g_PotionItemProperty[g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID()].cSecondType == _XGI_SC_POTION_HERT_OUT)
						{
							if(g_pState_Window)
								g_pState_Window->InsertStateListFromOther(_XST_ITEM, g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID(),
								g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType(), useitemresult->ucInvenIndex);
						}
					}
					else if(g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType() == _XGI_FC_QUEST)
					{
						if( g_QuestItemProperty[g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID()].cSecondType == 1 )
						{
							if(g_pState_Window)
								g_pState_Window->InsertStateListFromOther(_XST_ITEM, g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID(),
								g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType(), useitemresult->ucInvenIndex);
						}
					}					
					
					g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Set_m_ucCount( useitemresult->ucCount );
				}
				
				// 퀘스트 시작조건에 아이템 사용 조건이 들어간 퀘스트 시작조건을 검사 
				g_QuestScriptManager.QuestStartConditionUsingItemMode();
				
			}
			break;
		case 2 :	// 복용 후 성공
			{				
				_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_YES_WAV);				

				// Item 사용
				if(g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType() == _XGI_FC_ELIXIR)
				{
					if( g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cSecond() == _XGI_SC_ELIXIR_SUPPORT )
					{
						if(g_pState_Window)
							g_pState_Window->InsertStateListFromOther(_XST_ITEM, g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID(),
							g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType(), useitemresult->ucInvenIndex);
					}
					else
					{
						// 보조영약을 복용 중이면 지운다.
						g_pState_Window->DeleteElixirState(_XST_ITEM, _XGI_FC_ELIXIR );

						int addid = g_ElixirItemProperty[g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID()].ucStep;

						// 일단 제어
						if( addid > 4 )
							addid = 4;				
						g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
							_XDEF_MONSTERID_YAK1+addid, &g_pLocalUser->m_ModelDescriptor.m_Position );
					}
				}
				
				g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Reset();				

				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1719), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
			}
			break;
		case 3 :	// 복용중 공격 당해서 실패
			{
				g_pLocalUser->m_CurrentInternalUseState = _XUSER_STATE_7;
				
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
					_XDEF_MONSTERID_YAK1, &g_pLocalUser->m_ModelDescriptor.m_Position );

				if(g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType() == _XGI_FC_ELIXIR)
				{
					if( g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cSecond() == _XGI_SC_ELIXIR_SUPPORT )
					{						
					}
					else
					{
						// 보조영약을 복용 중이면 지운다.
						g_pState_Window->DeleteElixirState(_XST_ITEM, _XGI_FC_ELIXIR );
					}
				}
				
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1721), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Reset();
				g_pQuickSlot_Window->DeleteQuickSlot(useitemresult->ucInvenIndex);

				_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_NO_WAV);
			}
			break;
		case 4 :	// 복용 후 실패
			{				
				g_pLocalUser->m_CurrentInternalUseState = _XUSER_STATE_7;

				_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_NO_WAV);

				// Item 사용
				if(g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType() == _XGI_FC_ELIXIR)
				{
					if( g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cSecond() == _XGI_SC_ELIXIR_SUPPORT )
					{
						if(g_pState_Window)
							g_pState_Window->InsertStateListFromOther(_XST_ITEM, g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_sID(),
							g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Get_m_cType(), useitemresult->ucInvenIndex);
					}
					else
					{
						// 보조영약을 복용 중이면 지운다.
						g_pState_Window->DeleteElixirState(_XST_ITEM, _XGI_FC_ELIXIR );

						g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
							_XDEF_MONSTERID_YAK1, &g_pLocalUser->m_ModelDescriptor.m_Position );			
					}
				}
				
				g_pLocalUser->m_UserItemList[useitemresult->ucInvenIndex].Reset();

				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1720), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);	
			}
			break;
		}
	}
}

void _XNetwork::MSGElixirUseItem(MSG_ELIXIR_USE_RESULT* useitemresult)
{
	g_pLocalUser->m_ElixirInfo.Reset();	
	
	for( int i = 0; i < 5; ++i )
	{
		g_pLocalUser->m_ElixirInfo.Data[i].__Grade = useitemresult->E_Data.Data[i].__Grade;
		for( int ii = 0; ii < 10; ++ii )
			g_pLocalUser->m_ElixirInfo.Data[i].__Data[ii] = useitemresult->E_Data.Data[i].__Data[ii];
	}
	g_pLocalUser->m_ElixirInfo._sIntoxication = useitemresult->E_Data._sIntoxication;
	g_pLocalUser->m_IntoxicationTimer = g_LocalSystemTime;
	for( i = 0; i < 3; ++i )
		g_pLocalUser->m_ElixirInfo.Temp[i] = useitemresult->E_Data.Temp[i];
	
	g_pLocalUser->UpdateLocalUserAttackLevel();
	
	if( g_pLocalUser->m_ElixirInfo._sIntoxication )
	{
		TCHAR message[256];
		memset( message, 0, sizeof(message) );
		_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1722), g_pLocalUser->m_ElixirInfo._sIntoxication ); 
		InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, FALSE, 0xFFFF0000);
	}	
	
	if( useitemresult->E_Data._sIntoxication > 200 )
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1723), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);		
	}
	else if( useitemresult->E_Data._sIntoxication > 100 )
	{
		g_pState_Window->InsertStateListFromOther(_XST_INTOXICATION, 2003);
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1724), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);		
	}
}

void _XNetwork::MSGElixirIntox(MSG_ELIXIR_INTOX_TIME* intoxresult)
{
	g_pLocalUser->m_ElixirInfo._sIntoxication = intoxresult->sElixirIntox;
}

void _XNetwork::MSGItemRemove(MSG_ITEM_REMOVE* itemremove)
{
	g_GameItemlist.RemoveWorldItem(itemremove->iItemWorldID); // 내부적으로 서치하므로 검색 필요없슴.
}

void _XNetwork::MSGRepairItemResult(MSG_NPC_ITEM_REPAIR_RESULT* repairitemresult)
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
    if( repairitemresult->ucRepairCount == 0 && repairitemresult->cMode == 0 )	// 수리비 요청 결과
	{
		_XWindow_RepairItem* pRepairItem_Window = (_XWindow_RepairItem*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REPAIRITEM );	
		if( pRepairItem_Window )
		{
			pRepairItem_Window->m_NeedMoney = repairitemresult->uiLeftMoney;
			return;
		}		
	}

	switch(repairitemresult->ucRepairCount) 
	{
	case 255 :	// 수리 비용 불일치
		{
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if(pNPCScript_Window)
			{
				pNPCScript_Window->InsertNPCChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1725), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			}
		}
		break;
	case 254:	// 수리할 인벤토리 인덱스 불일치
		break;
	case 253:	// 무기나 의상이 아님, 타입 불일치
		{
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if(pNPCScript_Window)
			{
				pNPCScript_Window->InsertNPCChatString( _XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTREPAIRITEM), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			}
		}
		break;
	case 252:	// 사이즈 에러
		{
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if(pNPCScript_Window)
			{
				pNPCScript_Window->InsertNPCChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1726), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			}
		}
		break;
	case 251:	// 수리 NPC가 아님
		{
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if(pNPCScript_Window)
			{
				pNPCScript_Window->InsertNPCChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1727), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			}
		}
		break;
	default:	// 수리 OK
		{
			g_pLocalUser->m_Money = repairitemresult->uiLeftMoney;
			
			for( int i = 0; i < repairitemresult->ucRepairCount; ++i )
			{
				int slotnumber = repairitemresult->_S_Repair_Result[i].ucInvenIndex;
				g_pLocalUser->m_UserItemList[slotnumber].Set_m_usCurDur( repairitemresult->_S_Repair_Result[i].usCurDurability );
				g_pLocalUser->m_UserItemList[slotnumber].Set_m_usMaxDur( repairitemresult->_S_Repair_Result[i].usMaxDurability );
				
				TCHAR		namestring[256];
				memset(namestring, 0, sizeof(namestring));
				strncpy(namestring, _XGameItem::GetItemName(g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType(), g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID()), 
					sizeof(namestring));
				
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
				if(pNPCScript_Window)
				{
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1728), namestring);
					pNPCScript_Window->InsertNPCChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
				}
			}			
		}
		break;
	}

    // gauge 바 제거
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
	if( pChargeGauge_Window ) 
	{
		if( pChargeGauge_Window->GetShowStatus() )
		{
			if( repairitemresult->ucRepairCount > 0 )
				pChargeGauge_Window->StartFadeOutMode();
			else
				pChargeGauge_Window->ShowWindow(FALSE);
		}		
	}
#else
    if( repairitemresult->cRepairCount == 0 && repairitemresult->cMode == 0 )	// 수리비 요청 결과
	{
		_XWindow_RepairItem* pRepairItem_Window = (_XWindow_RepairItem*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REPAIRITEM );	
		if( pRepairItem_Window )
		{
			pRepairItem_Window->m_NeedMoney = repairitemresult->uiLeftMoney;
			return;
		}		
	}

	switch(repairitemresult->cRepairCount) 
	{
	case -1 :	// 수리 비용 불일치
		{
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if(pNPCScript_Window)
			{
				pNPCScript_Window->InsertNPCChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1725), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			}
		}
		break;
	case -2:	// 수리할 인벤토리 인덱스 불일치
		break;
	case -3:	// 무기나 의상이 아님, 타입 불일치
		{
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if(pNPCScript_Window)
			{
				pNPCScript_Window->InsertNPCChatString( _XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTREPAIRITEM), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			}
		}
		break;
	case -4:	// 사이즈 에러
		{
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if(pNPCScript_Window)
			{
				pNPCScript_Window->InsertNPCChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1726), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			}
		}
		break;
	case -5:	// 수리 NPC가 아님
		{
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if(pNPCScript_Window)
			{
				pNPCScript_Window->InsertNPCChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1727), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			}
		}
		break;
	default:	// 수리 OK
		{
			g_pLocalUser->m_Money = repairitemresult->uiLeftMoney;
			
			for( int i = 0; i < repairitemresult->cRepairCount; ++i )
			{
				int slotnumber = repairitemresult->_S_Repair_Result[i].ucInvenIndex;
				g_pLocalUser->m_UserItemList[slotnumber].Set_m_usCurDur( repairitemresult->_S_Repair_Result[i].usCurDurability );
				g_pLocalUser->m_UserItemList[slotnumber].Set_m_usMaxDur( repairitemresult->_S_Repair_Result[i].usMaxDurability );
				
				TCHAR		namestring[256];
				memset(namestring, 0, sizeof(namestring));
				strncpy(namestring, _XGameItem::GetItemName(g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType(), g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID()),
					sizeof(namestring));
				
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
				if(pNPCScript_Window)
				{
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1728), namestring);
					pNPCScript_Window->InsertNPCChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
				}
			}			
		}
		break;
	}

    // gauge 바 제거
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
	if( pChargeGauge_Window ) 
	{
		if( pChargeGauge_Window->GetShowStatus() )
		{
			if( repairitemresult->cRepairCount > 0 )
				pChargeGauge_Window->StartFadeOutMode();
			else
				pChargeGauge_Window->ShowWindow(FALSE);
		}		
	}
#endif

	_XWindow_RepairItem* pRepairItem_Window = (_XWindow_RepairItem*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REPAIRITEM );	
	
	if(pRepairItem_Window && g_pInventory_Window)
	{
		g_pInventory_Window->m_MouseSlotItem.Reset();
		pRepairItem_Window->ResetRepairListData();
		pRepairItem_Window->m_RepairButton->EnableWindow(TRUE);
		pRepairItem_Window->m_SelectedAllButton->EnableWindow(TRUE);
		pRepairItem_Window->m_SelectedCancleButton->EnableWindow(TRUE);
		pRepairItem_Window->SetEndDelayTime();
	}
}

#ifdef _XTS_ITEM_OPTIMIZATION
void _XNetwork::MSGWorldItemInfo(MSG_WORLD_ITEM* worlditeminfo)
{
	if(worlditeminfo->ucPackNum == 0)
	{
		g_GameItemlist.SetDefaultLiveWorldID();
		if(worlditeminfo->sNearItemNo == 0)
			g_GameItemlist.RemoveWorldItem();

		g_GameItemlist.resetList();
	}
	
	if(worlditeminfo->sNearItemNo > 0)
	{
		_XGameItem* newitem;
		D3DXVECTOR3 position;
		FLOAT		additionalheight;

		for(int i = 0 ; i < worlditeminfo->sNearItemNo ; ++i)
		{
			if(worlditeminfo->DropItem[i].S_detail.sID < 0)
				continue;
			
			newitem = new _XGameItem;

			position.x = worlditeminfo->DropItem[i].fX;
			position.z = worlditeminfo->DropItem[i].fZ;

			_XOBBData* pOBB = NULL;
			FLOAT obbheight = g_LodTerrain.GetTerrainAndRideOBBHeight(position, pOBB);
			position.y = obbheight;
			additionalheight = 0.0f;

			_XAnimMeshAbstractClass* pModel = NULL;

			switch( GetEqualItemType(worlditeminfo->DropItem[i].cType) )
			{
			case _XGI_FC_WEAPON :
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(worlditeminfo->DropItem[i].cType);
					if( WeaponItem )
						pModel = g_ItemModel_Weapon[0][WeaponItem[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_CLOTHES :
				{
					_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(worlditeminfo->DropItem[i].cType);
					if( ClothesItem )
					{
						switch(ClothesItem[worlditeminfo->DropItem[i].S_detail.sID].cSecondType)
						{
						case _XGI_SC_CLOTHES_UPPER :
							{
								pModel = g_UserModel_UBodyType[ClothesItem[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
								additionalheight -= 0.65f;
							}
							break;
						case _XGI_SC_CLOTHES_LOWER :
							{
								pModel = g_UserModel_LBodyType[ClothesItem[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
								additionalheight -= 0.3f;
							}
							break;
						case _XGI_SC_CLOTHES_GLOVES :
							{
								pModel = g_UserModel_LHandType[ClothesItem[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
								additionalheight = 0.15f;
							}
							break;
						case _XGI_SC_CLOTHES_SHOES :
							{
								pModel = g_UserModel_LFootType[ClothesItem[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
								additionalheight = 0.0f;
							}
							break;
						case _XGI_SC_CLOTHES_CAP :
							{
								pModel = g_UserModel_Cap[ClothesItem[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
								additionalheight = 0.0f;
							}
							break;
						case _XGI_SC_CLOTHES_BACKPACK :
							{
								pModel = g_UserModel_BackPack[ClothesItem[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
								additionalheight = 0.0f;
							}
							break;
						case _XGI_SC_CLOTHES_MASK :
							{
								
							}
							break;
						default :
							break;
						}
					}					
				}
				break;			
			case _XGI_FC_BOOK :
				{
					pModel = g_ItemModel_MartialBook[g_BookItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_POTION :
				{
					pModel = g_ItemModel_Potion[g_PotionItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_CONSUMING : // 2005.04.09 oneway48 insert : 술 종류
				{
					pModel = g_ItemModel_JewelBox[0];// 임시로 보석 상자 모델을 넣어둔다.
				}
				break;
			case _XGI_FC_MONEY :
				{
					pModel = g_ItemModel_JewelBox[2];
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					pModel = g_ItemModel_ETC[g_ResourceItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_ACCESSORY :
				{
					pModel = g_ItemModel_JewelBox[0];
				}
				break;
			case _XGI_FC_LIFE :
				{
					pModel = g_ItemModel_JewelBox[2];
				}
				break;
			case _XGI_FC_SOCKET :
				{
					pModel = g_ItemModel_JewelBox[2];
				}
				break;
			case _XGI_FC_ELIXIR :
				{
					pModel = g_ItemModel_Elixir[g_ResourceItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_BOX :
				{
					pModel = g_ItemModel_JewelBox[g_BoxItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_BOXKEY :
				{
					pModel = g_ItemModel_JewelBox[g_BoxKeyItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			default :
				break;
			}

			if(!pModel)
			{
				_XDWPACKETLOG("WARNING : invalid item model");
				delete newitem;
			}
			else
			{ 
				_XGameItem* pItem = g_GameItemlist.FindWorldItem(worlditeminfo->DropItem[i].iItemWorldID);

				if(!pItem) //존재하지 않는 아이템이면 새로 생성한다.
				{		
					newitem->Create(worlditeminfo->DropItem[i].iItemWorldID, position, additionalheight, pModel);
					newitem->m_usOwnerID	= worlditeminfo->DropItem[i].usOwner;
					newitem->m_cType		= worlditeminfo->DropItem[i].cType;
					newitem->m_cSecond		= worlditeminfo->DropItem[i].S_detail.cSecond;
					newitem->m_sID			= worlditeminfo->DropItem[i].S_detail.sID;
					newitem->m_cIdentity	= worlditeminfo->DropItem[i].S_detail.cIdentify;
					newitem->m_InsideFrustum = TRUE;
					newitem->m_LastReceiveMessageTime = g_LocalSystemTime;
					
					switch(newitem->m_cType)
					{
					case _XGI_FC_MONEY :
						{
							newitem->m_uiCount = worlditeminfo->DropItem[i].uiCount;
						}
						break;
					default :
						{
							newitem->m_ucCount = worlditeminfo->DropItem[i].ucCount;
						}
						break;
					}
					
					newitem->m_bLiveWorldID = TRUE;
		
					g_GameItemlist.insertItem(newitem);
				}
				else // 이미 존제하는 아이템이면 m_bLiveWorldID를 TRUE로 체크만 한다.
				{
					pItem->m_bLiveWorldID = TRUE;
					pItem->m_usOwnerID = worlditeminfo->DropItem[i].usOwner;
				}
			}
		}
		//2004.06.03->oneway48 insert : m_bLiveWorldID를 FALSE인 아이템을 삭제
		g_GameItemlist.RemoveWorldItem();
	}
}
#else
void _XNetwork::MSGWorldItemInfo(MSG_WORLD_ITEM* worlditeminfo)
{
	if(worlditeminfo->ucPackNum == 0)
	{
		g_GameItemlist.SetDefaultLiveWorldID();
		if(worlditeminfo->sNearItemNo == 0)
			g_GameItemlist.RemoveWorldItem();

		g_GameItemlist.resetList();
	}
	
	if(worlditeminfo->sNearItemNo > 0)
	{
		_XGameItem* newitem;
		D3DXVECTOR3 position;
		FLOAT		additionalheight;

		for(int i = 0 ; i < worlditeminfo->sNearItemNo ; ++i)
		{
			if(worlditeminfo->DropItem[i].S_detail.sID < 0)
				continue;
			
			newitem = new _XGameItem;

			position.x = worlditeminfo->DropItem[i].fX;
			position.z = worlditeminfo->DropItem[i].fZ;

			_XOBBData* pOBB = NULL;
			FLOAT obbheight = g_LodTerrain.GetTerrainAndRideOBBHeight(position, pOBB);
			position.y = obbheight;
			additionalheight = 0.0f;

			_XAnimMeshAbstractClass* pModel = NULL;

			switch(worlditeminfo->DropItem[i].cType)
			{
			case _XGI_FC_WEAPON :
				{
					pModel = g_ItemModel_Weapon[0][g_WeaponItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_CLOTHES :
				{
					switch(g_ClothesItemProperty[worlditeminfo->DropItem[i].S_detail.sID].cSecondType)
					{
					case _XGI_SC_CLOTHES_UPPER :
						{
							pModel = g_UserModel_UBodyType[g_ClothesItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight -= 0.65f;
						}
						break;
					case _XGI_SC_CLOTHES_LOWER :
						{
							pModel = g_UserModel_LBodyType[g_ClothesItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight -= 0.3f;
						}
						break;
					case _XGI_SC_CLOTHES_GLOVES :
						{
							pModel = g_UserModel_LHandType[g_ClothesItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight = 0.15f;
						}
						break;
					case _XGI_SC_CLOTHES_SHOES :
						{
							pModel = g_UserModel_LFootType[g_ClothesItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_CAP :
						{
							pModel = g_UserModel_Cap[g_ClothesItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_BACKPACK :
						{
							pModel = g_UserModel_BackPack[g_ClothesItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_MASK :
						{

						}
						break;
					default :
						break;
					}
				}
				break;
			case _XGI_FC_WEAPON2 :
				{
					pModel = g_ItemModel_Weapon[0][g_WeaponItemProperty2[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_CLOTHES2 :
				{
					short smodelindex = g_ClothesItemProperty2[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex;
					switch(g_ClothesItemProperty[worlditeminfo->DropItem[i].S_detail.sID].cSecondType)
					{						
					case _XGI_SC_CLOTHES_UPPER :
						{
							pModel = g_UserModel_UBodyType[g_ClothesItemProperty2[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight -= 0.65f;
						}
						break;
					case _XGI_SC_CLOTHES_LOWER :
						{							
							pModel = g_UserModel_LBodyType[g_ClothesItemProperty2[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight -= 0.3f;
						}
						break;
					case _XGI_SC_CLOTHES_GLOVES :
						{
							pModel = g_UserModel_LHandType[g_ClothesItemProperty2[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight = 0.15f;
						}
						break;
					case _XGI_SC_CLOTHES_SHOES :
						{
							pModel = g_UserModel_LFootType[g_ClothesItemProperty2[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_CAP :
						{
							pModel = g_UserModel_Cap[g_ClothesItemProperty2[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_BACKPACK :
						{
							pModel = g_UserModel_BackPack[g_ClothesItemProperty2[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_MASK :
						{

						}
						break;
					default :
						break;
					}
				}
				break;
			case _XGI_FC_BOOK :
				{
					pModel = g_ItemModel_MartialBook[g_BookItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_POTION :
				{
					pModel = g_ItemModel_Potion[g_PotionItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_CONSUMING : // 2005.04.09 oneway48 insert : 술 종류
				{
					pModel = g_ItemModel_JewelBox[0];// 임시로 보석 상자 모델을 넣어둔다.
				}
				break;
			case _XGI_FC_MONEY :
				{
					pModel = g_ItemModel_JewelBox[2];
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					pModel = g_ItemModel_ETC[g_ResourceItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_ACCESSORY :
				{
					pModel = g_ItemModel_JewelBox[0];
				}
				break;
			case _XGI_FC_LIFE :
				{
					pModel = g_ItemModel_JewelBox[2];
				}
				break;
			case _XGI_FC_SOCKET :
				{
					pModel = g_ItemModel_JewelBox[2];
				}
				break;
			case _XGI_FC_ELIXIR :
				{
					pModel = g_ItemModel_Elixir[g_ResourceItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_BOX :
				{
					pModel = g_ItemModel_JewelBox[g_BoxItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_BOXKEY :
				{
					pModel = g_ItemModel_JewelBox[g_BoxKeyItemProperty[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_WEAPON3 :
				{
					pModel = g_ItemModel_Weapon[0][g_WeaponItemProperty3[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
				}
				break;
			case _XGI_FC_CLOTHES3 :
				{
					short smodelindex = g_ClothesItemProperty3[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex;
					switch(g_ClothesItemProperty[worlditeminfo->DropItem[i].S_detail.sID].cSecondType)
					{						
					case _XGI_SC_CLOTHES_UPPER :
						{
							pModel = g_UserModel_UBodyType[g_ClothesItemProperty3[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight -= 0.65f;
						}
						break;
					case _XGI_SC_CLOTHES_LOWER :
						{							
							pModel = g_UserModel_LBodyType[g_ClothesItemProperty3[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight -= 0.3f;
						}
						break;
					case _XGI_SC_CLOTHES_GLOVES :
						{
							pModel = g_UserModel_LHandType[g_ClothesItemProperty3[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight = 0.15f;
						}
						break;
					case _XGI_SC_CLOTHES_SHOES :
						{
							pModel = g_UserModel_LFootType[g_ClothesItemProperty3[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_CAP :
						{
							pModel = g_UserModel_Cap[g_ClothesItemProperty3[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_BACKPACK :
						{
							pModel = g_UserModel_BackPack[g_ClothesItemProperty3[worlditeminfo->DropItem[i].S_detail.sID].sModelIndex];
							additionalheight = 0.0f;
						}
						break;
					case _XGI_SC_CLOTHES_MASK :
						{

						}
						break;
					default :
						break;
					}
				}
				break;
			default :
				break;
			}

			if(!pModel)
			{
				_XDWPACKETLOG("WARNING : invalid item model");
				delete newitem;
			}
			else
			{ 
				_XGameItem* pItem = g_GameItemlist.FindWorldItem(worlditeminfo->DropItem[i].iItemWorldID);

				if(!pItem) //존재하지 않는 아이템이면 새로 생성한다.
				{		
					newitem->Create(worlditeminfo->DropItem[i].iItemWorldID, position, additionalheight, pModel);
					newitem->m_usOwnerID	= worlditeminfo->DropItem[i].usOwner;
					newitem->m_cType		= worlditeminfo->DropItem[i].cType;
					newitem->m_cSecond		= worlditeminfo->DropItem[i].S_detail.cSecond;
					newitem->m_sID			= worlditeminfo->DropItem[i].S_detail.sID;
					newitem->m_cIdentity	= worlditeminfo->DropItem[i].S_detail.cIdentify;
					newitem->m_InsideFrustum = TRUE;
					newitem->m_LastReceiveMessageTime = g_LocalSystemTime;
					
					switch(newitem->m_cType)
					{
					case _XGI_FC_MONEY :
						{
							newitem->m_uiCount = worlditeminfo->DropItem[i].uiCount;
						}
						break;
					default :
						{
							newitem->m_ucCount = worlditeminfo->DropItem[i].ucCount;
						}
						break;
					}
					
					newitem->m_bLiveWorldID = TRUE;
		
					g_GameItemlist.insertItem(newitem);
				}
				else // 이미 존제하는 아이템이면 m_bLiveWorldID를 TRUE로 체크만 한다.
				{
					pItem->m_bLiveWorldID = TRUE;
					pItem->m_usOwnerID = worlditeminfo->DropItem[i].usOwner;
				}
			}
		}
		//2004.06.03->oneway48 insert : m_bLiveWorldID를 FALSE인 아이템을 삭제
		g_GameItemlist.RemoveWorldItem();
	}
}
#endif

void _XNetwork::MSGInvenCtrlResult(MSG_INVEN_CTRL_RESULT* invenctrlresult)
{
	switch(invenctrlresult->mode)
	{
	case inven_ctrl_item_count :
		{
			// 아이템 개수 변경
		}
		break;
		
	case inven_ctrl_money_set :
		{
			// 돈의 양 변경
		}
		break;
		
	case inven_ctrl_item_dur_set :
		{
			// 아이템 내구력 : 2004.06.14->oneway48 insert
 			g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Set_m_usCurDur( invenctrlresult->_S_Dur_Set.usDurability );
			
			FLOAT percent = 0;
			FLOAT maxdur = g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_usMaxDur();
			
			if( maxdur != 0 )
				percent = (FLOAT)g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_usCurDur() / maxdur * 100.0f; 
			
			if( percent <= 0.0f)// 2004.06.19->oneway48 insert
			{
				// 아이템 파괴됨
				if(g_pState_Window)
				{
					g_pState_Window->DeleteStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_sID(),
					g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_cType(), invenctrlresult->_S_Dur_Set.cIdx);
				}
				
				// 아이템 파괴에 대한 로그 처리 
				g_pLocalUser->m_CharacterLog.isDestroyItem		= TRUE;
				g_pLocalUser->m_CharacterLog.DestroyItemType	= g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_cType();
				g_pLocalUser->m_CharacterLog.DestroyItemID		= g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_sID();
				g_pLocalUser->m_CharacterLog.DestroyItemTime	= g_ServerTimeCode;
				
				// 시작조건이 아이템 파괴 검사인 강호풍운록에 대한 처리 
				g_QuestScriptManager.QuestStartConditionDestroyItemMode();

				TCHAR noticestring[256];
				memset(noticestring, 0, sizeof(noticestring));
				// 수정 hotblood
				_snprintf(noticestring, sizeof(noticestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1729), _XGameItem::GetItemName(g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_cType(), g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_sID() ) );

				InsertChatString(noticestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
				
				// 상.하의 인 경우 장착된 악세서리까지 같이 날려준다. hotblood ------------------------------------=				
				if( invenctrlresult->_S_Dur_Set.cIdx == _XINVENTORY_SLOTTYPE_AMOR )
				{
					for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_4; ++i )
					{
						// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
						g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[i].Get_m_cType(), g_pLocalUser->m_UserItemList[i].Get_m_sID()  );
						
						// 퀘스트 시작조건에 아이템 장착 조건이 있으면 시작조건의 아이템인지 체크를 한다.
						g_QuestScriptManager.RecheckWearItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[i].Get_m_cType(), g_pLocalUser->m_UserItemList[i].Get_m_sID()  );
						
						g_pLocalUser->m_UserItemList[i].Reset();
					}

					g_pLocalUser->m_cAmorPocketNum = 0;
				}
				else if( invenctrlresult->_S_Dur_Set.cIdx == _XINVENTORY_SLOTTYPE_PANTS )
				{
					for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_5; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_8; ++i )
					{
						// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
						g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[i].Get_m_cType(), g_pLocalUser->m_UserItemList[i].Get_m_sID()  );
						
						// 퀘스트 시작조건에 아이템 장착 조건이 있으면 시작조건의 아이템인지 체크를 한다.
						g_QuestScriptManager.RecheckWearItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[i].Get_m_cType(), g_pLocalUser->m_UserItemList[i].Get_m_sID()  );
						
						g_pLocalUser->m_UserItemList[i].Reset();
					}

					g_pLocalUser->m_cPantsPocketNum = 0;
				}
				// ------------------------------------------------------------------------------------------------=
				
				// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
				g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_cType(), g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_sID()  );
				
				// 퀘스트 시작조건에 아이템 장착 조건이 있으면 시작조건의 아이템인지 체크를 한다.
				g_QuestScriptManager.RecheckWearItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_cType(), g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_sID()  );
				
				g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Reset();
				
				DWORD lparam = (invenctrlresult->_S_Dur_Set.cIdx * 100000) + g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_sID();
				ChangeModel(lparam, g_pLocalUser->m_UserItemList[invenctrlresult->_S_Dur_Set.cIdx].Get_m_cType());

				// Item 상태효과 계산
				g_pLocalUser->UpdateLocalUserAttackLevel();
				
			}
			else
			{
				CheckValidItem(invenctrlresult->_S_Dur_Set.cIdx);
			}
		}
		break;
	}
}

void _XNetwork::MSGAddStrSlotResult(MSG_STR_BUY_SLOT_RESULT* addslotresult)
{	
	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	_XWindow_AddSaveItem* pAddSaveItem_Window = (_XWindow_AddSaveItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ADDSAVEITEM);
	_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
				
	if(pWarehouse_Window)
	{
		if( addslotresult->sCheckYear == 0 && addslotresult->ucCheckMonth == 0 &&
			addslotresult->ucCheckDay == 0 && addslotresult->ucCheckHour == 0)
		{
			// 거래에 문제가 생김
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_NOTICE, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1730), TRUE);
			return;
		}

		//Author : 양희왕 //breif : 창고 확장.. 굳이 검사할 필요가 없다.. 밑에 내용 수정
		// 작업자 확인할것
		// 창고 사용 칸수와 금액을 검사...틀리면 서버와 동기화가 맞지 않음...거래가 적용되지 않음.
	#ifndef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장
		if( pAddSaveItem_Window->m_AddStorageSize == addslotresult->ucTotalUsableSlot // 2004.05.28->oneway48 modify
			&& (g_pLocalUser->m_Money - pAddSaveItem_Window->m_AddStorageCost) == addslotresult->uiMoney )
	#endif
		{
			pWarehouse_Window->m_UseSlotCount = addslotresult->ucTotalUsableSlot;

			if( pWarehouse_Window->m_UseSlotCount > 20 && 
				pWarehouse_Window->m_UseSlotCount <= 40 )
			{
				pWarehouse_Window->m_SelectTabButtton[0]->ShowWindow( TRUE );
				pWarehouse_Window->m_SelectTabButtton[1]->ShowWindow( TRUE );
				pWarehouse_Window->m_SelectTabButtton[2]->ShowWindow( FALSE );
			}
			else if( pWarehouse_Window->m_UseSlotCount > 40 )
			{
				pWarehouse_Window->m_SelectTabButtton[0]->ShowWindow( TRUE );
				pWarehouse_Window->m_SelectTabButtton[1]->ShowWindow( TRUE );
				pWarehouse_Window->m_SelectTabButtton[2]->ShowWindow( TRUE );
			}
			else
			{
				pWarehouse_Window->m_SelectTabButtton[0]->ShowWindow( TRUE );
				pWarehouse_Window->m_SelectTabButtton[1]->ShowWindow( FALSE );
				pWarehouse_Window->m_SelectTabButtton[2]->ShowWindow( FALSE );
			}
			pWarehouse_Window->ChangeTab(0);
			g_pLocalUser->m_Money = addslotresult->uiMoney;
			
			pWarehouse_Window->m_TerminativeYear = addslotresult->sCheckYear;
			pWarehouse_Window->m_TerminativeMonth = addslotresult->ucCheckMonth;
			pWarehouse_Window->m_TerminativeDay = addslotresult->ucCheckDay;
			pWarehouse_Window->m_TerminativeHour = addslotresult->ucCheckHour;
			
			// 창고 사용 만료일은 %d년 %d월 %d일 입니다.
			TCHAR messagestring[256];
			memset(messagestring, 0, sizeof(messagestring));
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1731),	pWarehouse_Window->m_UseSlotCount);
			pNPCScript_Window->InsertNPCChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);

			int param1 = pWarehouse_Window->m_TerminativeYear;
			int param2 = pWarehouse_Window->m_TerminativeMonth;
			int param3 = pWarehouse_Window->m_TerminativeDay;
			
			strncpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAMEMESSAGEHANDLER_WAREHOUSETERMINATIONDAY,
																&param1, &param2, &param3 ), sizeof(messagestring) );
			
			pNPCScript_Window->InsertNPCChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
			pWarehouse_Window->m_bUseableWarehouse = TRUE;
			pWarehouse_Window->m_bTerminateWarehouse = FALSE; // 만료 해제 
			pNPCScript_Window->GenerateSubScriptList();

			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.SetNPCMode(_XNPC_NONE);
		}
	}
}

void _XNetwork::MSGStrItemEjectTerminateResult(MSG_STR_ITEM_EJECTEX* stritemeject)
{
	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	
	if(pWarehouse_Window)
	{
		if(stritemeject->cItemNum == 0)
		{
			// 돈
			g_pLocalUser->m_Money = stritemeject->uiMoney;
			g_pLocalUser->m_WarehouseMoney -= pWarehouse_Window->m_TempMoney;
			
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
			if(pNPCScript_Window)
			{
				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(messagestring));
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1732), pWarehouse_Window->m_TempMoney);
				pNPCScript_Window->InsertNPCChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			}
			
			pWarehouse_Window->ReleaseTempItemList();
		}
		else
		{
			// 아이템
			if( stritemeject->cItemNum > 0 )	// 서버에서 보내준 갯수 만큼만 꺼낸다.
			{
				int i = 0;
				list <_XWarehouseItem*>::iterator iter_item;
				for(iter_item = pWarehouse_Window->m_TempItemList.begin() ; iter_item != pWarehouse_Window->m_TempItemList.end() ; ++iter_item)
				{
					if( i >= stritemeject->cItemNum )
						break;

					_XWarehouseItem* pWarehouseItem = *iter_item;
					pWarehouseItem->m_InventoryNumber = stritemeject->Storage[i].cIdx;
					pWarehouseItem->m_ReceiveCount = stritemeject->Storage[i].usNumber;
					i++;
				}
				
				pWarehouse_Window->ItemSaveResultToInventory(stritemeject->cItemNum);
				pWarehouse_Window->SetTerminateLastItemIndex();
				
				if( g_pLocalUser->m_Money != stritemeject->uiMoney )		// 인벤돈과 서버에서 보내준 돈이 틀릴 경우만 처리
				{
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1733), g_pLocalUser->m_Money-stritemeject->uiMoney);
					InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );
					g_pLocalUser->m_Money = stritemeject->uiMoney;
				}				
			}
			else
			{
				_XDWINPRINT("STR_ITEM_SAVE - size ERROR =======================================");
			}
		}
		
		pWarehouse_Window->m_bWaitForServerResult = FALSE;
		pWarehouse_Window->m_bHaveMouseSlot = FALSE;
		pWarehouse_Window->m_bDragState = FALSE;
		pWarehouse_Window->m_MouseSlotItem.ResetMouseSlotItem();
	}
}

void _XNetwork::MSGPvPTradeCancel(MSG_PVP_TRADE_CANCEL* pvptradecancel)
{
	_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
	if(pPCTrade_Window && g_pInventory_Window)
	{
		for(int slotnumber = 0 ; slotnumber < _XDEF_PCTRADE_MAX_SLOTNUMBER ; ++slotnumber)
		{	
			if(pvptradecancel->result[slotnumber].slot < _XINVENTORY_SLOTTYPE_POCKETSTART)
				continue;
			
			if(g_pLocalUser->m_UserItemList[pvptradecancel->result[slotnumber].slot].Get_m_sID() != 0)
			{
				// 인벤토리가 비어있지 않음
				g_pLocalUser->m_UserItemList[pvptradecancel->result[slotnumber].slot].Set_m_ucCount(g_pLocalUser->m_UserItemList[pvptradecancel->result[slotnumber].slot].Get_m_ucCount() + pvptradecancel->result[slotnumber].number);
			}
			else
			{
				// 인벤토리가 비어있음
				g_pLocalUser->m_UserItemList[pvptradecancel->result[slotnumber].slot].Copy(pPCTrade_Window->m_SelfTradeItem[slotnumber]);
			}
			pPCTrade_Window->m_SelfTradeItem[slotnumber].ResetTradeItem();
		}
		g_pLocalUser->m_Money = pvptradecancel->uiMoney;
		
		pPCTrade_Window->m_bRequest = FALSE;
		g_pLocalUser->m_PvpNPCLock = FALSE;
		pPCTrade_Window->m_bTrading = FALSE;
		pPCTrade_Window->ResetTradeInfo();
		pPCTrade_Window->ResetTradeSetting();
		pPCTrade_Window->ShowWindow(FALSE);
		g_pInventory_Window->ShowWindow(FALSE);
		
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_CANCELTRADE), TRUE);
	}
	
	g_pLocalUser->m_CharacterLog.isTrade = FALSE;
}

void _XNetwork::MSGPvPTradeReq(MSG_PVP_TRADE_REQ* pvptradereq)
{
	TCHAR	messagestring[256];
	memset(messagestring, 0, sizeof(TCHAR)*256);

	switch(pvptradereq->cSub)
	{
	case SUB_MSG_TRADE_REQEST :
		{
			if( !g_bCommunityOptionRejectPvPTrade )
			{
				// 상대방이 교환 신청
				_XUser* pUser = g_Userlist.FindUser(pvptradereq->iTargetID);
				if(pUser)
				{
					BOOL npcdialogmode  = ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_bNPCDialogMode;  
					BOOL npcwideviewmode  = ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_bNPCDialogWideView;
					BOOL functionalwideviewmode = ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_bFunctionalObjectWideView;
					
					if( !npcdialogmode && !npcwideviewmode && !functionalwideviewmode)
					{
						if( g_pLocalUser->m_Lock.tradelock == 1 || g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )//노점 관련 행동시 P2P거래 거부 
						{
							SendPacket(MSG_NO_PVP_TRADE_REQ, SUB_MSG_TRADE_REJECT);
							_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
							if(pPCTrade_Window)
							{
								pPCTrade_Window->m_TargetID = pvptradereq->iTargetID;

								pPCTrade_Window->ResetTradeInfo();
								pPCTrade_Window->ResetTradeSetting();
							}
						}
						else if( g_pLocalUser->m_bInternalUse || g_pLocalUser->m_bSocketPlugIn || g_pLocalUser->m_bGambleBoxOpen ) // 영약을 복용중인지 검사
						{
							_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
							if(pPCTrade_Window)
							{
								pPCTrade_Window->m_TargetID = pvptradereq->iTargetID;
							}							
							SendPacket(MSG_NO_PVP_TRADE_REQ, SUB_MSG_TRADE_REJECT);
							_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1734), pUser->m_CharacterInfo.charactername );
							
							InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						}
						else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
						{
							// 비무 중에 거절
							_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
							if(pPCTrade_Window)
							{
								pPCTrade_Window->m_TargetID = pvptradereq->iTargetID;
							}							
							SendPacket(MSG_NO_PVP_TRADE_REQ, SUB_MSG_TRADE_REJECT);
							_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1735), pUser->m_CharacterInfo.charactername );
							
							InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						}
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
						else if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_trade))
						{
							SendPacket(MSG_NO_PVP_TRADE_REQ, SUB_MSG_TRADE_REJECT);
							_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1735), pUser->m_CharacterInfo.charactername );
							InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						}
#endif
#ifdef _XTS_PK
						else if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
						{
							// Pk중 거절
							_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2745), pUser->m_CharacterInfo.charactername);	// "대전 중이므로 %s님의 교환 요청을 거절하였습니다."
							InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						}
#endif
						else
						{
							_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
							if(pPCTrade_Window)
							{
								if( !pPCTrade_Window->m_bTrading )
								{
									pPCTrade_Window->m_TargetID = pvptradereq->iTargetID;
									pPCTrade_Window->m_bTrading = TRUE;

									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_PCTRADEMESSAGEWINDOW);
									_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1736), pUser->m_CharacterInfo.charactername );
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_PCTRADEWINDOW_YES, _XDEF_PCTRADEWINDOW_NO);
									pMessageBox->StartTimer(10000, TRUE);
									g_pLocalUser->m_PvpNPCLock = TRUE;							
								}								
							}							
						}					
					}
					else
					{
						_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1734), pUser->m_CharacterInfo.charactername );
						InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);

						_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
						if(pPCTrade_Window)
						{
							pPCTrade_Window->m_TargetID = pvptradereq->iTargetID;
						}
						SendPacket(MSG_NO_PVP_TRADE_REQ, SUB_MSG_TRADE_REJECT);						
					}
				}
				else
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T("ERROR"), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_TRADEUNKNOWN), TRUE);
					_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
					if(pPCTrade_Window)
					{
						pPCTrade_Window->ResetTradeInfo();
						pPCTrade_Window->ResetTradeSetting();
					}
				}
			}				
			else// 거래신청 거부 옵션에 켜 있을 때 
			{
				SendPacket(MSG_NO_PVP_TRADE_REQ, SUB_MSG_TRADE_REJECT);
				_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
				if(pPCTrade_Window)
				{
					pPCTrade_Window->ResetTradeInfo();
					pPCTrade_Window->ResetTradeSetting();
				}
			}
		}
		break;

	case SUB_MSG_TRADE_REJECT :
		{
			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			if( !strlen(pPCTrade_Window->m_TargetName) )
			{
				pPCTrade_Window->m_bRequest = FALSE;
				
				// 상대방이 거절
				_XUser* pUser = g_Userlist.FindUser(pvptradereq->iTargetID);
				if(pUser)
				{
					g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_PCTRADEMESSAGEWINDOW);

					_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1737), pUser->m_CharacterInfo.charactername );
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
					pMessageBox->StartTimer( 10000, TRUE );
					g_pLocalUser->m_PvpNPCLock = FALSE;
					pPCTrade_Window->m_bTrading = FALSE;

					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				}
				else
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T("ERROR"), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_REJECTUNKNOWN), TRUE);
					if(pPCTrade_Window)
					{
						pPCTrade_Window->ResetTradeInfo();
						pPCTrade_Window->ResetTradeSetting();
					}
				}
			}	
			
			g_pLocalUser->m_CharacterLog.isTrade = FALSE;
		}
		break;	

	case SUB_MSG_TRADE_CONFIRM :
		{
			_XUser* pUser = g_Userlist.FindUser(pvptradereq->iTargetID);
			if(pUser)
			{
				_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
				if(pPCTrade_Window)
				{
					pPCTrade_Window->m_TargetWaitIcon->ShowWindow( FALSE );
					pPCTrade_Window->m_TargetConfirmIcon->ShowWindow( TRUE );					
				}
			}
		}
		break;

	case SUB_MSG_TRADE_CANCEL  :
		{
			_XUser* pUser = g_Userlist.FindUser(pvptradereq->iTargetID);
			if(pUser)
			{
				_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
				if(pPCTrade_Window)
				{
					pPCTrade_Window->m_bTrading = FALSE;
					pPCTrade_Window->ResetTradeSetting();						
				}
			}

			g_pLocalUser->m_CharacterLog.isTrade = FALSE;
		}
		break;	

	case SUB_MSG_TRADE_BUSY :
		{
			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			if( pPCTrade_Window )
			{
				pPCTrade_Window->m_bRequest = FALSE;
				pPCTrade_Window->m_bTrading = FALSE;
			}			
			g_pLocalUser->m_PvpNPCLock = FALSE;			
			
			// 상대방이 거래중
			_XUser* pUser = g_Userlist.FindUser(pvptradereq->iTargetID);			

			if(pUser)
			{		
				_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1738), pUser->m_CharacterInfo.charactername );
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
				pMessageBox->StartTimer(10000, TRUE);				
			}
			else
			{
				if( g_pLocalUser->m_Uniqueid == pvptradereq->iTargetID )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1739), TRUE);
					pMessageBox->StartTimer(10000, TRUE);				
				}
				else
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T("ERROR"), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_TRADINGUNKNOWN), TRUE);
					if(pPCTrade_Window)
					{
						pPCTrade_Window->ResetTradeInfo();
						pPCTrade_Window->ResetTradeSetting();
					}
				}
			}
		}
		break;

	case SUB_MSG_TRADE_UNKNOWN :
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1740), TRUE);
			g_pLocalUser->m_PvpNPCLock = FALSE;
			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			if( pPCTrade_Window )
			{
				pPCTrade_Window->m_bTrading = FALSE;
			}

			g_pLocalUser->m_CharacterLog.isTrade = FALSE;
		}
		break;
	case SUB_MSG_TRADE_SELFREJECT :
		{
			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			if( !strlen(pPCTrade_Window->m_TargetName) )
			{
				pPCTrade_Window->m_bRequest = FALSE;
				
				// 상대방이 거절
				_XUser* pUser = g_Userlist.FindUser(pvptradereq->iTargetID);
				if(pUser)
				{
					g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_PCTRADEMESSAGEWINDOW);

					_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1741), pUser->m_CharacterInfo.charactername );	
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
					pMessageBox->StartTimer( 100000, TRUE );
					g_pLocalUser->m_PvpNPCLock = FALSE;
					pPCTrade_Window->m_bTrading = FALSE;

					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				}
				else
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T("ERROR"), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_REJECTUNKNOWN), TRUE);
					if(pPCTrade_Window)
					{
						pPCTrade_Window->ResetTradeInfo();
						pPCTrade_Window->ResetTradeSetting();
					}
				}
			}	
			
			g_pLocalUser->m_CharacterLog.isTrade = FALSE;
		}
		break;
	}
}

void _XNetwork::MSGStrItemSaveResult(MSG_STR_ITEM_SAVE* stritemsave)
{
	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	if(pWarehouse_Window)
	{
		pWarehouse_Window->m_bWaitForServerResult = FALSE;
		if(stritemsave->cItemNum == 0)
		{
			// 돈
			if( g_pLocalUser->m_WarehouseMoney != stritemsave->uiMoney )	// 창고돈과 서버에서 보내준 돈이 틀릴 경우만 처리
			{
				g_pLocalUser->m_WarehouseMoney = stritemsave->uiMoney;
				g_pLocalUser->m_Money -= pWarehouse_Window->m_TempMoney;
				
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
				if(pNPCScript_Window)
				{
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1742), pWarehouse_Window->m_TempMoney);
					pNPCScript_Window->InsertNPCChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );
				}
			}			
			
			pWarehouse_Window->ReleaseTempItemList();
		}
		else
		{
			//_XLog( "StoC_count %d index %d size %d", stritemsave->cItemNum, stritemsave->Storage[0].cIdx, pWarehouse_Window->m_TempItemList.size() );
			// 아이템
			if(stritemsave->cItemNum > 0)	// 서버에서 보내준 갯수만큼만 저장한다.
			{
				int i = 0;
				list <_XWarehouseItem*>::iterator iter_item;
				for(iter_item = pWarehouse_Window->m_TempItemList.begin() ; iter_item != pWarehouse_Window->m_TempItemList.end() ; ++iter_item)
				{
					if( i >= stritemsave->cItemNum )
						break;
					_XWarehouseItem* pWarehouseItem = *iter_item;
					pWarehouseItem->m_WarehouseNumber = stritemsave->Storage[i].cIdx;
					pWarehouseItem->m_ReceiveCount = stritemsave->Storage[i].usNumber;
					//_XLog( "StoC_Info index %d", stritemsave->Storage[i].cIdx );
					i++;
				}
				
				pWarehouse_Window->ItemSaveResultToWarehouse(stritemsave->cItemNum);
			}
			else
			{
				_XDWINPRINT("STR_ITEM_SAVE - size ERROR =======================================");
			}
		}
		
		if(g_pInventory_Window)
		{
			g_pInventory_Window->m_bHaveMouseSlot = FALSE;
			g_pInventory_Window->m_bDragState = FALSE;
			g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
		}
	}
}

void _XNetwork::MSGStrItemEjectResult(MSG_STR_ITEM_EJECT* stritemsave)
{
	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	
	if(pWarehouse_Window)
	{
		if(stritemsave->cItemNum == 0)
		{
			// 돈
			if( g_pLocalUser->m_Money != stritemsave->uiMoney )		// 인벤돈과 서버에서 보내준 돈이 틀릴 경우만 처리
			{
				g_pLocalUser->m_Money = stritemsave->uiMoney;
				g_pLocalUser->m_WarehouseMoney -= pWarehouse_Window->m_TempMoney;
				
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
				if(pNPCScript_Window)
				{
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1732), pWarehouse_Window->m_TempMoney);
					pNPCScript_Window->InsertNPCChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
				}
			}		
			
			pWarehouse_Window->ReleaseTempItemList();
		}
		else
		{
			// 아이템
			if( stritemsave->cItemNum > 0 )	// 서버에서 보내준 갯수만큼만 꺼낸다.
			{
				//_XLog( "StoC_count %d index %d size %d", stritemsave->cItemNum, stritemsave->Storage[0].cIdx, pWarehouse_Window->m_TempItemList.size() );

				int i = 0;
				list <_XWarehouseItem*>::iterator iter_item;
				for(iter_item = pWarehouse_Window->m_TempItemList.begin() ; iter_item != pWarehouse_Window->m_TempItemList.end() ; ++iter_item)
				{
					if( i >= stritemsave->cItemNum )
						break;

					_XWarehouseItem* pWarehouseItem = *iter_item;
					pWarehouseItem->m_InventoryNumber = stritemsave->Storage[i].cIdx;
					pWarehouseItem->m_ReceiveCount = stritemsave->Storage[i].usNumber;
					//_XLog( "StoC_Info index %d", stritemsave->Storage[i].cIdx );
					i++;
				}
				
				pWarehouse_Window->ItemSaveResultToInventory(stritemsave->cItemNum);
			}
			else
			{
				_XDWINPRINT("STR_ITEM_SAVE - size ERROR =======================================");
			}
		}
		
		pWarehouse_Window->m_bWaitForServerResult = FALSE;
		pWarehouse_Window->m_bHaveMouseSlot = FALSE;
		pWarehouse_Window->m_bDragState = FALSE;
		pWarehouse_Window->m_MouseSlotItem.ResetMouseSlotItem();
	}
}

void _XNetwork::MSGPvPTradeStart(MSG_PVP_TRADE_START* pvptradestart)
{
	_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);	
	if(pPCTrade_Window && g_pInventory_Window )
	{
		unsigned short	userid = 0;
		if(pPCTrade_Window->m_bRequest)
			userid = pvptradestart->iTargetID;
		else
			userid = pvptradestart->iReqID;
		
		_XUser* pUser = g_Userlist.FindUser(userid);
		if(pUser)
		{
			g_pLocalUser->m_PvpNPCLock = TRUE;

			pPCTrade_Window->ShowWindow(TRUE);
			pPCTrade_Window->SetDefaultPosition();

			g_pInventory_Window->ShowWindow(TRUE);
			POINT pos = pPCTrade_Window->GetWindowPos();
			SIZE size = pPCTrade_Window->GetWindowSize();
			g_pInventory_Window->MoveWindow( pos.x + size.cx, pos.y );
			g_pInventory_Window->SetPCTradeWindowPtr(pPCTrade_Window);
			
			strcpy(pPCTrade_Window->m_TargetName, pUser->m_CharacterInfo.charactername);
			pPCTrade_Window->m_TradeServerSlotNo = pvptradestart->TradeSlotNO;
			g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_PCTRADEMESSAGEWINDOW);
		}
		else
		{
			pPCTrade_Window->ResetTradeInfo();
			pPCTrade_Window->ResetTradeSetting();
		}
	}
}

void _XNetwork::MSGPvPTradeDelete(MSG_PVP_TRADE_DELETE* pvptradedelete)
{	
	_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
	if(pPCTrade_Window && g_pInventory_Window)
	{
		if( pvptradedelete->Inven_Index == 250 )	// 지우기 실패 
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1743), TRUE);
			pPCTrade_Window->ResetTradeSetting();
			g_pLocalUser->m_PvpNPCLock = FALSE;
			pPCTrade_Window->m_bTrading = FALSE;
			return;
		}
		
		if(pvptradedelete->ucSubjectID == g_pLocalUser->m_Uniqueid)
		{
			if(g_pLocalUser->m_UserItemList[pvptradedelete->Inven_Index].Get_m_sID() != 0)
			{
				// 빈 인벤토리가 아님, 더해준다.
				g_pLocalUser->m_UserItemList[pvptradedelete->Inven_Index].Set_m_ucCount(g_pLocalUser->m_UserItemList[pvptradedelete->Inven_Index].Get_m_ucCount() + pvptradedelete->number);
			}
			else
			{
				// 인벤토리 슬롯이 비었음
				g_pLocalUser->m_UserItemList[pvptradedelete->Inven_Index].Copy(pPCTrade_Window->m_SelfTradeItem[pvptradedelete->TWindowIndex]);
				g_pLocalUser->m_UserItemList[pvptradedelete->Inven_Index].Set_m_ucCount(pvptradedelete->number);
			}
			if(pPCTrade_Window->m_SelfTradeItem[pvptradedelete->TWindowIndex].IsStackable())
			{
				pPCTrade_Window->m_SelfTradeItem[pvptradedelete->TWindowIndex].Set_m_ucCount(pPCTrade_Window->m_SelfTradeItem[pvptradedelete->TWindowIndex].Get_m_ucCount() - pvptradedelete->number);
				if(pPCTrade_Window->m_SelfTradeItem[pvptradedelete->TWindowIndex].Get_m_ucCount() <= 0)
					pPCTrade_Window->m_SelfTradeItem[pvptradedelete->TWindowIndex].ResetTradeItem();
			}
			else
			{
				pPCTrade_Window->m_SelfTradeItem[pvptradedelete->TWindowIndex].ResetTradeItem();
			}
			
			pPCTrade_Window->m_MouseSlotItem.ResetMouseSlotItem();
			pPCTrade_Window->m_bHaveMouseSlot = FALSE;
			pPCTrade_Window->m_bDragState = FALSE;
			pPCTrade_Window->m_bIsTarget = FALSE;			
			
			pPCTrade_Window->ResetTradeSetting();		
		}
		else
		{
			if(pPCTrade_Window->m_TargetTradeItem[pvptradedelete->TWindowIndex].IsStackable())
			{
				pPCTrade_Window->m_TargetTradeItem[pvptradedelete->TWindowIndex].Set_m_ucCount(pPCTrade_Window->m_TargetTradeItem[pvptradedelete->TWindowIndex].Get_m_ucCount() - pvptradedelete->number);
				if(pPCTrade_Window->m_TargetTradeItem[pvptradedelete->TWindowIndex].Get_m_ucCount() <= 0)
					pPCTrade_Window->m_TargetTradeItem[pvptradedelete->TWindowIndex].ResetTradeItem();

				pPCTrade_Window->ResetTradeSetting();
			}
			else
			{
				pPCTrade_Window->m_TargetTradeItem[pvptradedelete->TWindowIndex].ResetTradeItem();
				pPCTrade_Window->ResetTradeSetting();
			}
		}
	}
}

void _XNetwork::MSGPvPTradeUpdateInfo(MSG_PVP_TRADE_UPDATE_INFO* pvptradeupdate)
{
	_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
	if(pPCTrade_Window)
	{
		if( pvptradeupdate->item.cSecond == -1 && pvptradeupdate->item.cType == -1 && 
			pvptradeupdate->item.sID == -1 && pvptradeupdate->TradeWindowSlotNo == 250 )
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1744), TRUE);
		}
		else if( pvptradeupdate->item.cSecond == -2 && pvptradeupdate->item.cType == -2 && 
			pvptradeupdate->item.sID == -2 && pvptradeupdate->TradeWindowSlotNo == 250 )
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1744), TRUE);
		}
		else if( pvptradeupdate->item.cSecond == -3 && pvptradeupdate->item.cType == -3 && 
			pvptradeupdate->item.sID == -3 && pvptradeupdate->TradeWindowSlotNo == 250 )
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1740), TRUE);
		}
		else if( pvptradeupdate->item.cSecond == -4 && pvptradeupdate->item.cType == -4 && 
			pvptradeupdate->item.sID == -4 && pvptradeupdate->TradeWindowSlotNo == 250 )
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), "", TRUE);
		}
		else
		{
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_cType( pvptradeupdate->item.cType );
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_cSecond( pvptradeupdate->item.cSecond );
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_sID (pvptradeupdate->item.sID);
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].m_cIdentify = pvptradeupdate->item.Item_Money.S_NormalItem.cIdentify;
			
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_cThird(_XGameItem::GetThird(pvptradeupdate->item.cType, pvptradeupdate->item.sID));
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_ucCount( pvptradeupdate->item.Item_Money.S_NormalItem.ucCount );	
			
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_usCurDur(pvptradeupdate->item.Item_Money.S_NormalItem.usCurDur);
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_usMaxDur(_XGameItem::GetMaxDurability(pvptradeupdate->item.cType, pvptradeupdate->item.sID));
			
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_ucSocketCount(pvptradeupdate->item.Item_Money.S_NormalItem.ucSocketCount);
			if( pvptradeupdate->item.Item_Money.S_NormalItem.ucSocketCount > 0 )
			{
				for( int i = 0; i < 4; ++i )
					pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_sSocket( i, pvptradeupdate->item.sSocket[i] );
			}

			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_ucStrength( pvptradeupdate->item.ucStrength );
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].m_usCurProtect  = pvptradeupdate->item.uselimit.usCurProtect; // 바른용린의수
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].m_usCurMaxProtect = pvptradeupdate->item.uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].m_usUsedProtect = pvptradeupdate->item.uselimit.usUsedProtect; // 용루 사용여부
			
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_ucWeaponStrength( _XGameItem::GetNature(pvptradeupdate->item.cType, pvptradeupdate->item.sID) );
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_ucSharpness( pvptradeupdate->item.ucSharpness );
#else
			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_ucSharpness(pvptradeupdate->item.ucSharpness );
#endif

			pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].m_ucFlag1 = pvptradeupdate->item.uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //author : 양희왕 //breif : 디파인 되어있으면 들어오지않음
				pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].m_ucFlag2		= pvptradeupdate->item.uselimit.ucFlag2;	
				pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].m_ucFlag3		= pvptradeupdate->item.uselimit.ucFlag3;
#endif
				pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].m_usValue1	= pvptradeupdate->item.uselimit.usValue1;

			if( pvptradeupdate->item.uselimit.ucYear > 0 )
			{				
				pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_ucYear( pvptradeupdate->item.uselimit.ucYear );	
				pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_ucMonth(pvptradeupdate->item.uselimit.ucMonth );	
				pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_ucDay( pvptradeupdate->item.uselimit.ucDay );	
				pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_ucHour( pvptradeupdate->item.uselimit.ucHour );	
				pPCTrade_Window->m_TargetTradeItem[pvptradeupdate->TradeWindowSlotNo].Set_m_ucMinute( pvptradeupdate->item.uselimit.ucMinute );
			}

			if( pPCTrade_Window->m_TargetConfirmIcon->GetShowStatus() || pPCTrade_Window->m_ConfirmIcon->GetShowStatus())
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_2425), TRUE);
				pMessageBox->StartTimer(10000, TRUE);
			}			
		}

		pPCTrade_Window->ResetTradeSetting();
	}
}

void _XNetwork::MSGPvPTradeMoney(MSG_PVP_TRADE_MONEY* pvptrademoney)
{
	_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
	if(pPCTrade_Window)
	{
		if( pvptrademoney->cIsOppe == 1 )		// 남의돈
		{
			if(pvptrademoney->AddOrSub == 1)
				pPCTrade_Window->m_TargetMoney += pvptrademoney->uiMoney;
			else
				pPCTrade_Window->m_TargetMoney -= pvptrademoney->uiMoney;

			if( pPCTrade_Window->m_TargetConfirmIcon->GetShowStatus() || pPCTrade_Window->m_ConfirmIcon->GetShowStatus())
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_2425), TRUE);
				pMessageBox->StartTimer(10000, TRUE);
			}
		}
		else			// 내돈
		{
			if(pvptrademoney->AddOrSub == 1)
			{
				pPCTrade_Window->m_SelfMoney += pvptrademoney->uiMoney;
				if( g_pLocalUser->m_Money >= pvptrademoney->uiMoney )
					g_pLocalUser->m_Money -= pvptrademoney->uiMoney;				
			}
			else if(pvptrademoney->AddOrSub == -1 )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1745), TRUE);
			}
			else if(pvptrademoney->AddOrSub == -2 )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1746), TRUE);
			}
			else
			{
				g_pLocalUser->m_Money += pvptrademoney->uiMoney;
				pPCTrade_Window->m_SelfMoney -= pvptrademoney->uiMoney;
			}
		}
	}
	pPCTrade_Window->ResetTradeSetting();
}

void _XNetwork::MSGPvPTradeEnd(MSG_PVP_TRADE_END* pvptradeend)
{
	_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
	
	switch(pvptradeend->Reason)
	{
	case -3 :	// 둘다 슬롯이 모자람
	case -1 :
		{
			// 자신의 슬롯이 모자람
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_NOTENOUGHINV, TRUE);
			pPCTrade_Window->RestoreTrade();
			g_pLocalUser->m_Money = pvptradeend->uiTotalMoney; // 2004.05.28->oneway48 insert
		}
		break;
	case -2 :
		{
			// 상대방의 슬롯이 모자람
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(pvptradeend->iTargetID);
			if(pUser)
			{
				TCHAR	messagestring[256];
				memset(messagestring, 0, sizeof(messagestring));
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_NOTENOUGHOPP), pUser->m_CharacterInfo.charactername);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
			}
			else
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_NOTENOUGHUNKNOWN), TRUE);
			}
			pPCTrade_Window->RestoreTrade();
			g_pLocalUser->m_Money = pvptradeend->uiTotalMoney; // 2004.05.28->oneway48 insert
		}
		break;	
	case -4 :
		{
			// 상대방의 현금 공간이 모자람
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NEW_3789, TRUE);
			pPCTrade_Window->RestoreTrade();
			g_pLocalUser->m_Money = pvptradeend->uiTotalMoney; // 2004.05.28->oneway48 insert
		}
		break;
	case -6 : // 둘다 현금 공간이 모자람.
	case -5 :
		{
			// 자신의 현금 공간이 모자람
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NEW_3789, TRUE);
			pPCTrade_Window->RestoreTrade();
			g_pLocalUser->m_Money = pvptradeend->uiTotalMoney; // 2004.05.28->oneway48 insert
		}
		break;
	case -7 :
		{
			// 상대방이 비정상 종료
			_XUser* pUser = (_XUser*)g_Userlist.FindUser(pvptradeend->iTargetID);
			if(pUser)
			{
				TCHAR	messagestring[256];
				memset(messagestring, 0, sizeof(messagestring));
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_CANCELTRADELOGOUT), pUser->m_CharacterInfo.charactername);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
			}
			else
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_CANCELTRADEUNKNOWN), TRUE);
			}
			pPCTrade_Window->RestoreTrade();
			g_pLocalUser->m_Money = pvptradeend->uiTotalMoney; // 2004.05.28->oneway48 insert			
		}
		break;
	case 1 :
		{
			// 정상 완료
			for(int slotnumber = 0 ; slotnumber < _XDEF_PCTRADE_MAX_SLOTNUMBER ; ++slotnumber)
			{
				if(pvptradeend->User[slotnumber].slot < _XINVENTORY_SLOTTYPE_POCKETSTART)
					continue;
				
				g_pLocalUser->m_UserItemList[pvptradeend->User[slotnumber].slot].Copy(pPCTrade_Window->m_TargetTradeItem[slotnumber]);
				pPCTrade_Window->m_TargetTradeItem[slotnumber].ResetTradeItem();
			}
			g_pLocalUser->m_Money = pvptradeend->uiTotalMoney;

			_XUser* pUser = g_Userlist.FindUser(pvptradeend->iTargetID);
			if(pUser)
			{
				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(messagestring));
				_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1747), pUser->m_CharacterInfo.charactername );
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
				pMessageBox->StartTimer( 100000, TRUE );
			}
			else
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1748), TRUE);
				pMessageBox->StartTimer( 100000, TRUE );
			}
			
			_XPlayInterfaceSound(ID_SR_INTERFACE_MONEY_01_WAV);
		}
		break;
	}
	
	g_pLocalUser->m_PvpNPCLock = FALSE;	
	pPCTrade_Window->m_bTrading = FALSE;
	g_pLocalUser->m_CharacterLog.isTrade = FALSE;
	pPCTrade_Window->ResetTradeInfo();
	pPCTrade_Window->ShowWindow(FALSE);
	g_pInventory_Window->ShowWindow(FALSE);
}

void _XNetwork::NPCTradeResult(MSG_NPC_TRADE_RESULT* traderesult)
{
	switch(traderesult->cResult)
	{
	case 2 :	// 문파기여도로 사기 성공
	case 0 :
		{
			// 아이템 사기 팔기 결과
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
			if(traderesult->cInvenIndex == 255)	// 거래 취소
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_CANCELTRADE), TRUE );
				break;
			}
			if(traderesult->cInvenIndex == 254)	// 돈부족
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_2604), TRUE );
				break;
			}
			if(traderesult->cInvenIndex == 245)	// 문파기여도부족
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ERROR5), TRUE );
				break;
			}
			if(traderesult->cInvenIndex == 244)	// 문파 기여도가 다름
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_2604), TRUE );
				break;
			}
		#else
			if(traderesult->cInvenIndex == -1)	// 거래 취소
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_CANCELTRADE), TRUE );
				break;
			}
			if(traderesult->cInvenIndex == -2)	// 돈부족
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_2604), TRUE );
				break;
			}
			if(traderesult->cInvenIndex == -11)	// 문파기여도부족
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ERROR5), TRUE );
				break;
			}
			if(traderesult->cInvenIndex == -12)	// 문파 기여도가 다름
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_2604), TRUE );
				break;
			}
		#endif

			_XWindow_NPCTrade* pNPCTrade_Window = (_XWindow_NPCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADE);
			if( pNPCTrade_Window )
			{
				if( pNPCTrade_Window->m_bIsBuy ) //true = NPC에게서 사기의 결과
				{
					// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
					if( g_TutorialInfo )
					{
						if( traderesult->Item.cType == _XGI_FC_POTION && !g_TutorialInfo->_XTutorial_025 )
						{
							_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
							pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_025);
							g_TutorialInfo->_XTutorial_025 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
							g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
						}
					}					
					// ----------------------------------------------------------------------------------------------------------------------------=

					// 퀘스트 아이템 카운트 표시할때 2개이상 들어와도 표시하기 위해서 더하기 이전 값을 저장해야 한다.
					int itemcountbeforetrade = g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Get_m_ucCount();

					int buynumber = traderesult->Item.Item_Money.S_NormalItem.ucCount - g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Get_m_ucCount();
					
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_cType(traderesult->Item.cType );
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_cSecond(traderesult->Item.cSecond );
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_sID( traderesult->Item.sID );
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_cIdentify = traderesult->Item.Item_Money.S_NormalItem.cIdentify;
					
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_cThird(_XGameItem::GetThird(traderesult->Item.cType, traderesult->Item.sID));
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucCount( traderesult->Item.Item_Money.S_NormalItem.ucCount );
					
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_usCurDur(traderesult->Item.Item_Money.S_NormalItem.usCurDur);
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_usMaxDur(_XGameItem::GetMaxDurability(traderesult->Item.cType, traderesult->Item.sID));
				
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucSocketCount( traderesult->Item.Item_Money.S_NormalItem.ucSocketCount );
					if( traderesult->Item.Item_Money.S_NormalItem.ucSocketCount > 0 )
					{
						for( int i = 0; i < 4; ++i )
							g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_sSocket( i, traderesult->Item.sSocket[i] );
					}

					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucStrength( traderesult->Item.ucStrength );
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_usCurProtect  = traderesult->Item.uselimit.usCurProtect; // 바른용린의수
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_usCurMaxProtect = traderesult->Item.uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_usUsedProtect = traderesult->Item.uselimit.usUsedProtect; // 용루 사용여부
					
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucWeaponStrength( _XGameItem::GetNature(traderesult->Item.cType, traderesult->Item.sID) );
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucSharpness( traderesult->Item.ucSharpness );
#else
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucSharpness(traderesult->Item.ucSharpness);
#endif

					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_ucFlag1 = traderesult->Item.uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //author : 양희왕 //breif : 디파인 되어있으면 들어오지 않는다 
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_ucFlag2	= traderesult->Item.uselimit.ucFlag2;	
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_ucFlag3	= traderesult->Item.uselimit.ucFlag3;
#endif
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_usValue1	= traderesult->Item.uselimit.usValue1;

					if( traderesult->Item.uselimit.ucYear > 0 )
					{						
						g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucYear(traderesult->Item.uselimit.ucYear);	
						g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucMonth(traderesult->Item.uselimit.ucMonth);	
						g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucDay(traderesult->Item.uselimit.ucDay);	
						g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucHour(traderesult->Item.uselimit.ucHour);	
						g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucMinute(traderesult->Item.uselimit.ucMinute);
					}
					
					if(traderesult->cResult == 0)
					{
						g_pLocalUser->m_Money = traderesult->uiLeftMoney;
					}
					else if(traderesult->cResult == 2)
					{
						// 기여도로 물품 사기 성공
						g_pLocalUser->m_CharacterInfo.Set_iContribution(traderesult->uiLeftMoney);
					}
					
					TCHAR		namestring[256];
					memset(namestring, 0, sizeof(TCHAR)*256);
					strcpy(namestring, _XGameItem::GetItemName(traderesult->Item.cType, traderesult->Item.sID));
					
					_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
					if(pNPCScript_Window)
					{
						TCHAR messagestring[256];
						memset(messagestring, 0, sizeof(TCHAR)*256);

						strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORKITEMTRADE_1749, namestring, &buynumber ) );

						pNPCScript_Window->InsertNPCChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
						_XPlayInterfaceSound(ID_SR_INTERFACE_MONEY_01_WAV);
					}

					
#ifdef _XTS_NEWPARTYCOUNTER
					g_QuestScriptManager.CheckAllItemCounter(traderesult->Item.cType,traderesult->Item.sID);
#else
					bool checkitem = false;
					// 아이템 카운터에 관련된 아이템이 들어오는 경우 갱신시킨다.		
					for( int i = 0; i < ID_QUEST_SYNC; ++i ) // 제룡행 퀘스트는 진행중인 퀘스트에서 제외 -> 5개 
					{
						if( checkitem ) break;
						_XQuestScriptItem* currentquest = NULL;
						if( i != 5 )
						{
							if( g_QuestScriptManager.m_ProgressSubQuestID[i] != -1)
							{
								int questid = g_QuestScriptManager.m_ProgressSubQuestID[i];
								
								if( questid >= 10000 && questid < 20000  )
								{
									map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
									if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
										currentquest = iter_quest->second;
								}
								else
								{
									map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
									if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
										currentquest = iter_quest->second;
								}
								
							}//if( g_QuestScriptManager.m_ProgressSubQuestID[i] != -1)
						}
						else
						{
							if( g_QuestScriptManager.m_bProgressMainQuest )
							{
								int questid = g_QuestScriptManager.m_CurrentMainQuestID;
								map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
								if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
									currentquest = iter_quest->second;
							}				
						}
						
						if( currentquest )
						{
							for( int itemcounterindex = 0; itemcounterindex < 8; ++itemcounterindex )
							{
								if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType != -1 )
								{
									if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType == traderesult->Item.cType 
										&& currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID == traderesult->Item.sID )
									{
										
										TCHAR	buffer[256];
										memset(buffer, 0, sizeof(TCHAR)*256);
										
										int itemtotalcount = 0;
										for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
										{
											if(g_pLocalUser->m_UserItemList[i].m_cType == traderesult->Item.cType
												&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == traderesult->Item.sID)
												itemtotalcount += g_pLocalUser->m_UserItemList[i].m_ucCount;														
										}
									#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
										for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
										{
											if(g_pLocalUser->m_UserItemList[i].m_cType == traderesult->Item.cType
												&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == traderesult->Item.sID)
												itemtotalcount += g_pLocalUser->m_UserItemList[i].m_ucCount;	
										}
									#endif
										
										if( itemtotalcount != 0 )
										{
											if( currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint == 0 )
											{
												_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
													currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
													itemtotalcount );
											}
											else if( itemtotalcount < currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
											{
												_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
													currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
													itemtotalcount,
													currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint );
											}
											else if( itemtotalcount >= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
											{
												if( itemcountbeforetrade < currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
												{
													// add word order
													_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), 
														currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
														currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint,
														currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint);
												}
											}
											
											if( buffer[0] != 0 )
											{
												int strlength = strlen(buffer);
												
												if(g_QuestScriptManager.m_lpszQuestSystemMessage )
												{
													delete g_QuestScriptManager.m_lpszQuestSystemMessage;
													g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
												}
												
												if(strlength > 0)
												{
													g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
													g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
													strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
												}
												
												g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
												g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
											}	
											
											
											// 현재 퀘스트 창에 열려있는 퀘스트 관련 아이템이 들어오는 경우 퀘스트 창을 갱신시킨다.								
											_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
											if( pMainFrame_Window && pMainFrame_Window->GetShowStatus() )
											{
												if( g_pQuest_Window )
												{
													if( g_pQuest_Window->GetShowStatus() )
													{
														g_pQuest_Window->ParseQuestDescString();
													}
												}
											}
										}
										
										
										checkitem = true;
										
										break;
									}
								}
							}
						}//	if( currentquest )
					}//for( int i = 0; i < ID_QUEST_SYNC-1; i++ )
#endif
										
				} // if( pNPCTrade_Window->m_bIsBuy )
				else//false = NPC에게 팔기의 결과
				{	
					TCHAR		namestring[256];
					memset(namestring, 0, sizeof(TCHAR)*256);
					strcpy(namestring, _XGameItem::GetItemName(g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Get_m_cType(), g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Get_m_sID()));
					
					_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
					if(pNPCScript_Window)
					{
						TCHAR messagestring[256];
						memset(messagestring, 0, sizeof(TCHAR)*256);
						int sellnumber = g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Get_m_ucCount() - traderesult->Item.Item_Money.S_NormalItem.ucCount;
						DWORD price = traderesult->uiLeftMoney - g_pLocalUser->m_Money;
												
						strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORKITEMTRADE_1750, namestring, &sellnumber, &price ) );
						
						pNPCScript_Window->InsertNPCChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );
						_XPlayInterfaceSound(ID_SR_INTERFACE_MONEY_01_WAV);
						
						g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_cType( traderesult->Item.cType );
						g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_sID( traderesult->Item.sID );
						g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucCount(traderesult->Item.Item_Money.S_NormalItem.ucCount );
						g_pLocalUser->m_Money = traderesult->uiLeftMoney;

						if(traderesult->Item.sID > 0 && traderesult->Item.Item_Money.S_NormalItem.ucCount <= 0)
						{
							g_pQuickSlot_Window->DeleteQuickSlot((u_char)traderesult->cInvenIndex);
						}
					}
				}

				pNPCTrade_Window->m_bHaveMouseSlot = FALSE;
				pNPCTrade_Window->m_bDragState = FALSE;
				pNPCTrade_Window->m_MouseSlotItem.ResetMouseSlotItem();

			} // if( pNPCTrade_Window )

			if(g_pInventory_Window)
			{
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_bDragState = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
			}
			
			// 재료수집 창에 켜져 있을 때는 실시간으로 재료 현황을 갱신한다.
			if( g_pLifeSkill_Window && g_pLifeSkill_Window->GetShowStatus() )
			{
				if(g_pLifeSkill_Window->m_SelectedTab == _XLIFETAB_LIFECLASS2)
					g_pLifeSkill_Window->SetListResourceNeedItem(FALSE);
			}
		}
		break;
	case 1 :
		{
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
			// 문파 기여도
			if(traderesult->cInvenIndex == 224)	// 퀘스트로 문파 기여도 상승할 시
			{
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
				if(pNPCScript_Window)
				{
					pNPCScript_Window->InsertNPCChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1751), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );
					g_pLocalUser->m_CharacterInfo.Set_iContribution( traderesult->uiLeftMoney );
				}
			}
			else if(traderesult->cInvenIndex == 223)
			{
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
				_XWindow_Contribution* pContribution_Window = (_XWindow_Contribution*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CONTRIBUTIONWINDOW);			
				if(pNPCScript_Window && pContribution_Window )
				{
					int clanpoint = traderesult->uiLeftMoney - g_pLocalUser->m_CharacterInfo.Get_iContribution();
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);

					int param1 = pContribution_Window->m_ContributionItemCount;					
					strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORKITEMTRADE_1752, &param1, &clanpoint ) );
					
					pNPCScript_Window->InsertNPCChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );
					
					g_pLocalUser->m_Money -= pContribution_Window->m_ContributionItemCount;
					g_pLocalUser->m_CharacterInfo.Set_iContribution( traderesult->uiLeftMoney );
					pContribution_Window->ResetContributionData();
					
					_XPlayInterfaceSound(ID_SR_INTERFACE_MONEY_01_WAV);
				}
			}
		#else
			if(traderesult->cInvenIndex == 127)	// 퀘스트로 문파 기여도 상승할 시
			{
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
				if(pNPCScript_Window)
				{
					pNPCScript_Window->InsertNPCChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1751), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );
					g_pLocalUser->m_CharacterInfo.Set_iContribution( traderesult->uiLeftMoney );
				}
			}
			else if(traderesult->cInvenIndex == 126)
			{
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
				_XWindow_Contribution* pContribution_Window = (_XWindow_Contribution*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CONTRIBUTIONWINDOW);			
				if(pNPCScript_Window && pContribution_Window )
				{
					int clanpoint = traderesult->uiLeftMoney - g_pLocalUser->m_CharacterInfo.Get_iContribution();
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);

					int param1 = pContribution_Window->m_ContributionItemCount;					
					strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORKITEMTRADE_1752, &param1, &clanpoint ) );
					
					pNPCScript_Window->InsertNPCChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );
					
					g_pLocalUser->m_Money -= pContribution_Window->m_ContributionItemCount;
					g_pLocalUser->m_CharacterInfo.Set_iContribution( traderesult->uiLeftMoney );
					pContribution_Window->ResetContributionData();
					
					_XPlayInterfaceSound(ID_SR_INTERFACE_MONEY_01_WAV);
				}
			}
		#endif
			else
			{
				TCHAR		namestring[256];
				memset(namestring, 0, sizeof(TCHAR)*256);
				strcpy(namestring, _XGameItem::GetItemName(g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Get_m_cType(), g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Get_m_sID()));
				
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
				_XWindow_Contribution* pContribution_Window = (_XWindow_Contribution*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CONTRIBUTIONWINDOW);			
				if(pNPCScript_Window)
				{
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);
					//int sellnumber = g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_ucCount - traderesult->Item.ucCount;
					int clanpoint = traderesult->uiLeftMoney - g_pLocalUser->m_CharacterInfo.Get_iContribution();

					int param2 = pContribution_Window->m_ContributionItemCount;
					strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORKITEMTRADE_1753, 
																namestring, &param2, &clanpoint ) );
					
					pNPCScript_Window->InsertNPCChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );
					
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_cType( traderesult->Item.cType );
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_sID( traderesult->Item.sID );
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucCount( traderesult->Item.Item_Money.S_NormalItem.ucCount );
					g_pLocalUser->m_CharacterInfo.Set_iContribution( traderesult->uiLeftMoney );
					
					if(traderesult->Item.sID > 0 && traderesult->Item.Item_Money.S_NormalItem.ucCount <= 0)
					{
						g_pQuickSlot_Window->DeleteQuickSlot((u_char)traderesult->cInvenIndex);
					}

					_XPlayInterfaceSound(ID_SR_INTERFACE_MONEY_01_WAV);
				}			
				if( pContribution_Window )
				{
					pContribution_Window->ResetContributionData();
					g_pInventory_Window->m_ContributionItemInvenNumber = -1;
				}
			}

			if(g_pInventory_Window)
			{
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_bDragState = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
			}
			
			_XWindow_NPCTrade* pNPCTrade_Window = (_XWindow_NPCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADE);
			if(pNPCTrade_Window)
			{
				pNPCTrade_Window->m_bHaveMouseSlot = FALSE;
				pNPCTrade_Window->m_bDragState = FALSE;
				pNPCTrade_Window->m_MouseSlotItem.ResetMouseSlotItem();
			}
		}
		break;
	case 5 :
		{
			// 치료 성공
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1754), TRUE);
			g_pLocalUser->m_Money = traderesult->uiLeftMoney;
			_XPlayInterfaceSound(ID_SR_INTERFACE_USE_EAT_004_WAV);
			
			if(g_pState_Window)
			{
				g_pState_Window->DeleteStateList(_XST_EXTERNAL, 0);
				g_pState_Window->DeleteStateList(_XST_INTERNAL, 0);
			}
		}
		break;
	case 6 :
		{
			// 아이템 사기 사기 결과
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
			if(traderesult->cInvenIndex == 255)	// 거래 취소
		#else
			if(traderesult->cInvenIndex == -1)	// 거래 취소
		#endif
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_CANCELTRADE), TRUE );
				break;
			}		
			
			_XWindow_NPCTrade* pNPCTrade_Window = (_XWindow_NPCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADE);
			if( pNPCTrade_Window )
			{			
				// 퀘스트 아이템 카운트 표시할때 2개이상 들어와도 표시하기 위해서 더하기 이전 값을 저장해야 한다.
				int itemcountbeforetrade = g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Get_m_ucCount();
			
				int buynumber = traderesult->Item.Item_Money.S_NormalItem.ucCount - g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Get_m_ucCount();
				
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_cType(traderesult->Item.cType);
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_cSecond(traderesult->Item.cSecond);
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_sID( traderesult->Item.sID );
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_cIdentify = traderesult->Item.Item_Money.S_NormalItem.cIdentify;
				
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_cThird(_XGameItem::GetThird(traderesult->Item.cType, traderesult->Item.sID));
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucCount(traderesult->Item.Item_Money.S_NormalItem.ucCount);			
				
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_usCurDur( traderesult->Item.Item_Money.S_NormalItem.usCurDur );
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_usMaxDur( _XGameItem::GetMaxDurability(traderesult->Item.cType, traderesult->Item.sID) );
			
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucSocketCount( traderesult->Item.Item_Money.S_NormalItem.ucSocketCount );
				if( traderesult->Item.Item_Money.S_NormalItem.ucSocketCount > 0 )
				{
					for( int i = 0; i < 4; ++i )
						g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_sSocket( i, traderesult->Item.sSocket[i] );

				}
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_ucFlag1 = traderesult->Item.uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //Author : 양희왕 //breif : 추가 하지 않았음
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_ucFlag2	= traderesult->Item.uselimit.ucFlag2;	
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_ucFlag3	= traderesult->Item.uselimit.ucFlag3;
#endif
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].m_usValue1	= traderesult->Item.uselimit.usValue1;

				if( traderesult->Item.uselimit.ucYear > 0 )
				{					
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucYear(traderesult->Item.uselimit.ucYear);	
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucMonth(traderesult->Item.uselimit.ucMonth);	
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucDay(traderesult->Item.uselimit.ucDay);	
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucHour(traderesult->Item.uselimit.ucHour);	
					g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucMinute(traderesult->Item.uselimit.ucMinute);
				}
				
				g_pLocalUser->m_Money = traderesult->uiLeftMoney;
				
				TCHAR		namestring[256];
				memset(namestring, 0, sizeof(TCHAR)*256);
				strcpy(namestring, _XGameItem::GetItemName(traderesult->Item.cType, traderesult->Item.sID));
				
				_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
				if(pNPCScript_Window)
				{
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);
					
					strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORKITEMTRADE_1749, namestring, &buynumber ) );

					InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
					_XPlayInterfaceSound(ID_SR_INTERFACE_MONEY_01_WAV);
				}
				
				pNPCTrade_Window->m_bHaveMouseSlot = FALSE;
				pNPCTrade_Window->m_bDragState = FALSE;
				pNPCTrade_Window->m_MouseSlotItem.ResetMouseSlotItem();
				
			} // if( pNPCTrade_Window )
			
			if(g_pInventory_Window)
			{
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_bDragState = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
			}
			
			// 재료수집 창에 켜져 있을 때는 실시간으로 재료 현황을 갱신한다.
			if( g_pLifeSkill_Window && g_pLifeSkill_Window->GetShowStatus() )
			{
				if(g_pLifeSkill_Window->m_SelectedTab == _XLIFETAB_LIFECLASS2)
					g_pLifeSkill_Window->SetListResourceNeedItem(FALSE);
			}
		}		
		break;
	case -1 :
		{
			// 치료 실패
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1755), TRUE);
			g_pLocalUser->m_Money = traderesult->uiLeftMoney;
		}
		break;
	case 7 :
		{
			// 표국 이동
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
			if(traderesult->cInvenIndex >= 0 &&
				traderesult->cInvenIndex <= _XINVENTORY_SLOTTYPE_TOTALCOUNT)
		#else
			if(traderesult->cInvenIndex >= 0)
		#endif
			{
				// 성공
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_cType(traderesult->Item.cType);
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_sID( traderesult->Item.sID );
				g_pLocalUser->m_UserItemList[traderesult->cInvenIndex].Set_m_ucCount( traderesult->Item.Item_Money.S_NormalItem.ucCount );
//				g_pLocalUser->m_Money = traderesult->uiLeftMoney;
			}
			else
			{
			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
				switch(traderesult->cInvenIndex)
				{
				case 255 :
					{
						// npc 기능 없음
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3482), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// 3482
					#ifdef _XDWDEBUG
						InsertChatString("-1 :npc 기능 없음", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				case 254 :
					{
						// 아이템 정보가 없거나 표국 아이템이 아님
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3482), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// 3482
					#ifdef _XDWDEBUG
						InsertChatString("-2 :item 정보가 없거나 표국아이템이 아님", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				case 253 :
					{
						// 표국 인덱스 없음
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3482), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// 3482
					#ifdef _XDWDEBUG
						InsertChatString("-3 :표국 인덱스 없음", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				case 252 : 
					{
						// 갈수 없는 지역
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3483), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// 3483
					#ifdef _XDWDEBUG
						InsertChatString("-4 :갈수 없는 지역", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				case 251 :
					{
						// 영역 정보 없음
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3482), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// 3482
					#ifdef _XDWDEBUG
						InsertChatString("-5 :영역 정보 없음", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				case 250 :
					{
						// 레벨 제한
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2333), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// 2333
					#ifdef _XDWDEBUG
						InsertChatString("-6 : 레벨 맞지않음", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				case 245 :
					{
						// 알수 없는 오류 
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3482), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// 3482
					#ifdef _XDWDEBUG
						InsertChatString("-10 :알수 없는 오류", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				}
			#else
				switch(traderesult->cInvenIndex)
				{
				case -1 :
					{
						// npc 기능 없음
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3482), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// 3482
					#ifdef _XDWDEBUG
						InsertChatString("-1 :npc 기능 없음", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				case -2 :
					{
						// 아이템 정보가 없거나 표국 아이템이 아님
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3482), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// 3482
					#ifdef _XDWDEBUG
						InsertChatString("-2 :item 정보가 없거나 표국아이템이 아님", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				case -3 :
					{
						// 표국 인덱스 없음
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3482), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// 3482
					#ifdef _XDWDEBUG
						InsertChatString("-3 :표국 인덱스 없음", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				case -4 : 
					{
						// 갈수 없는 지역
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3483), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// 3483
					#ifdef _XDWDEBUG
						InsertChatString("-4 :갈수 없는 지역", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				case -5 :
					{
						// 영역 정보 없음
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3482), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// 3482
					#ifdef _XDWDEBUG
						InsertChatString("-5 :영역 정보 없음", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				case -6 :
					{
						// 레벨 제한
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2333), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// 2333
					#ifdef _XDWDEBUG
						InsertChatString("-6 : 레벨 맞지않음", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				case -10 :
					{
						// 알수 없는 오류 
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3482), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// 3482
					#ifdef _XDWDEBUG
						InsertChatString("-10 :알수 없는 오류", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					#endif
					}
					break;
				}
			#endif
			}
		}
		break;
	}

}
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : 양희왕 breif : 소켓제거

void _XNetwork::MSGItemEquipSocketResult( MSG_SOCKET_GAMBLE* pSocketorGambleResult )
{
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
	_XWindow_SocketPlugIn* pSocket_Window = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
	_XWindow_Gamble* pGamble_Window = (_XWindow_Gamble*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GAMBLE);
	_XWindow_SocketDetach* pSocketDetachWindow = (_XWindow_SocketDetach*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETDETACH_WINDOW);

	TCHAR tempString[128];
	memset(tempString, 0, sizeof(tempString));

	if( pChargeGauge_Window )
	{
		if( pChargeGauge_Window->GetShowStatus() )
			pChargeGauge_Window->StartFadeOutMode();
	}

	switch( pSocketorGambleResult->ucMode )
	{
	case en_socket_gamble_mode_equip_result :
		{
			MSG_SM_EQUIP_RESULT* socketitemresult = (MSG_SM_EQUIP_RESULT*)pSocketorGambleResult;
    
    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
            if( socketitemresult->CompleteItem.cSlot > 150 )
    #else
			if( socketitemresult->CompleteItem.cSlot < 0 )
    #endif
			{
				// 실패처리
				g_pInventory_Window->m_UsedLifeItemIndex = 0;
				pSocket_Window->m_IndependentItem = NULL;
				g_pInventory_Window->m_iSocketIndexList[0] = 0;
				for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT; ++i )
				{
					g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
					pSocket_Window->m_SocketItemList[i].Reset();
				}
				
				if( pSocket_Window->GetShowStatus() )
					pSocket_Window->ShowWindow(FALSE);
			
				pSocket_Window->m_SocketPlugInButton->EnableWindow(FALSE);
				pSocket_Window->m_SocketPlugInButton->ShowWindow(FALSE);
				pSocket_Window->m_bChangeButtonMode = FALSE;
				pSocket_Window->m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM); // 물품 내리기

				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1758), TRUE);
				return;
			}

			// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
			g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_cType(), g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_sID()  );
			
			g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Reset();
			g_pInventory_Window->m_UsedLifeItemIndex = 0;
			pSocket_Window->m_IndependentItem = NULL;
			
			// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
			g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_cType(), g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_sID()  );
			g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Reset();
			g_pInventory_Window->m_iSocketIndexList[0] = 0;
			
			for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT; ++i )
			{
				if( g_pInventory_Window->m_iSocketIndexList[i+1] > 0 )
				{
					// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
					g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Get_m_cType(), g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Get_m_sID()  );
					
					g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Reset();
					g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
					pSocket_Window->m_SocketItemList[i].Reset();
				}	
			}

#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
			for(i = 0 ; i < 4 ; ++i)
			{
				// 남은 아이템 처리
				if(socketitemresult->leftItemInfo[i].slot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
				{
					if(socketitemresult->leftItemInfo[i].number == 0)
					{
						g_pLocalUser->m_UserItemList[socketitemresult->leftItemInfo[i].slot].Reset();
					}
					else
					{
						g_pLocalUser->m_UserItemList[socketitemresult->leftItemInfo[i].slot].Set_m_ucCount(socketitemresult->leftItemInfo[i].number);
					}
				}
			}
#endif
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_cType( socketitemresult->CompleteItem.cType );
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_cSecond(socketitemresult->CompleteItem.cSecond);
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_sID( socketitemresult->CompleteItem.sID );
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_cIdentify = socketitemresult->CompleteItem.Item_Money.S_NormalItem.cIdentify;
			
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_cThird(_XGameItem::GetThird(socketitemresult->CompleteItem.cType, socketitemresult->CompleteItem.sID));
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucCount( socketitemresult->CompleteItem.Item_Money.S_NormalItem.ucCount );
			
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_usCurDur( socketitemresult->CompleteItem.Item_Money.S_NormalItem.usCurDur );
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_usMaxDur( _XGameItem::GetMaxDurability(socketitemresult->CompleteItem.cType, socketitemresult->CompleteItem.sID) );

			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucSocketCount( socketitemresult->CompleteItem.Item_Money.S_NormalItem.ucSocketCount );
			if( socketitemresult->CompleteItem.Item_Money.S_NormalItem.ucSocketCount > 0 )
			{
				for( int i = 0; i < 4; ++i )
					g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_sSocket( i, socketitemresult->CompleteItem.sSocket[i] );
			}
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucStrength( socketitemresult->CompleteItem.ucStrength );
	#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_usCurProtect  = socketitemresult->CompleteItem.uselimit.usCurProtect; // 바른용린의수
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_usCurMaxProtect = socketitemresult->CompleteItem.uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_usUsedProtect = socketitemresult->CompleteItem.uselimit.usUsedProtect; // 용루 사용여부
			
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucWeaponStrength( _XGameItem::GetNature(socketitemresult->CompleteItem.cType, socketitemresult->CompleteItem.sID) );
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucSharpness( socketitemresult->CompleteItem.ucSharpness );
	#else
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucSharpness( socketitemresult->CompleteItem.ucSharpness );
	#endif


			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucFlag1 = socketitemresult->CompleteItem.uselimit.ucFlag1;

	#ifndef _XDEF_SMELT_ITEM //author : 양희왕 //breif : 디파인 되어있으면 들어오지 않는다
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucFlag2	= socketitemresult->CompleteItem.uselimit.ucFlag2;	
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucFlag3	= socketitemresult->CompleteItem.uselimit.ucFlag3;
	#endif
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_usValue1	= socketitemresult->CompleteItem.uselimit.usValue1;

			if( socketitemresult->CompleteItem.uselimit.ucYear > 0 )
			{				
				g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucYear(socketitemresult->CompleteItem.uselimit.ucYear);	
				g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucMonth(socketitemresult->CompleteItem.uselimit.ucMonth);	
				g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucDay(socketitemresult->CompleteItem.uselimit.ucDay);	
				g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucHour(socketitemresult->CompleteItem.uselimit.ucHour);	
				g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucMinute(socketitemresult->CompleteItem.uselimit.ucMinute);
			}

			LPTSTR param1 = _XGameItem::GetItemName( g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Get_m_cType(), 
													 g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Get_m_sID());
			int	   param2 = g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Get_m_ucCount();
			
			InsertChatString(g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_GETSTATUS, param1, &param2 ),
							 _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET);

			g_pLocalUser->UpdateLocalUserAttackLevel();

			if( g_pInventory_Window )
			{
				if( g_pInventory_Window->GetShowStatus() )
				{
					g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_bNewItem = TRUE;
					g_pInventory_Window->m_bDrawGrabItem = TRUE;
				}
			}

			if( pSocket_Window->GetShowStatus() )
				pSocket_Window->ShowWindow(FALSE);
			
			pSocket_Window->m_SocketPlugInButton->EnableWindow(FALSE);
			pSocket_Window->m_SocketPlugInButton->ShowWindow(FALSE);
			pSocket_Window->m_bChangeButtonMode = FALSE;
			pSocket_Window->m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM); // 물품 내리기
			g_pInventory_Window->SetDefaultPosition();
		}
		break;
	case en_socket_gamble_mode_boxgamble_result :
		{
			MSG_SM_EQUIP_RESULT* socketitemresult = (MSG_SM_EQUIP_RESULT*)pSocketorGambleResult;

    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
            if( socketitemresult->CompleteItem.cSlot > 150 )	// 상자 겜블시	실패
    #else
			if( socketitemresult->CompleteItem.cSlot < 0 )	// 상자 겜블시	실패
    #endif
			{		
				if( pGamble_Window )
				{
					// 실패 처리
					g_pInventory_Window->m_UsedLifeItemIndex = 0;
					pGamble_Window->m_GambleBoxItem = NULL;
					g_pInventory_Window->m_iSocketIndexList[0] = 0;
					for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT-1; ++i )
					{
						if( g_pInventory_Window->m_iSocketIndexList[i+1] > 0 )
						{
		#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
		#else
							if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].IsStackable() )
							{
								g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Set_m_ucCount( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Get_m_ucCount()+1);
							}
		#endif
							
							g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
							pGamble_Window->m_GambleKeyList[i].Reset();
						}			
					}

		#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
					// 실패 시에는 아이템 처리 하지 않음
		/*			for(i = 0 ; i < 4 ; ++i)
					{
						// 남은 아이템 처리
						if(socketitemresult->leftItemInfo[i].slot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
						{
							if(socketitemresult->leftItemInfo[i].number == 0)
							{
								g_pLocalUser->m_UserItemList[socketitemresult->leftItemInfo[i].slot].Reset();
							}
							else
							{
								g_pLocalUser->m_UserItemList[socketitemresult->leftItemInfo[i].slot].m_ucCount = socketitemresult->leftItemInfo[i].number;
							}
						}
					}*/
		#endif
					
					if( pGamble_Window->GetShowStatus() )
						pGamble_Window->ShowWindow(FALSE);

					pGamble_Window->m_pBoxOpenButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKITEMTRADE_1756);

					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1757), TRUE);
					pMessageBox->StartTimer( 10000, TRUE );
					_XPlayInterfaceSound(ID_SR_INTERFACE_LEVELFAIL_WAV);			
					return;										
				}
			}
			
			_XPlayInterfaceSound(ID_SR_INTERFACE_OPEN_BOX_WAV);

			// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
			g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_cType(), g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_sID()  );
			
			g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Reset();
			g_pInventory_Window->m_UsedLifeItemIndex = 0;
			pGamble_Window->m_GambleBoxItem = NULL;

			// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
			g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_cType(), g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_sID()  );
			g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Reset();
			g_pInventory_Window->m_iSocketIndexList[0] = 0;
			
			for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT-1; ++i )
			{
				if( g_pInventory_Window->m_iSocketIndexList[i+1] > 0 )
				{
					// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
					g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Get_m_cType(), g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Get_m_sID()  );
					
					if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].IsStackable() )
					{
						if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Get_m_ucCount() == 0 )
							g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Reset();
					}
					
					g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
					pGamble_Window->m_GambleKeyList[i].Reset();
				}			
			}

#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
			for(i = 0 ; i < 4 ; ++i)
			{
				// 남은 아이템 처리
				if(socketitemresult->leftItemInfo[i].slot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
				{
					if(socketitemresult->leftItemInfo[i].number == 0)
					{
						g_pLocalUser->m_UserItemList[socketitemresult->leftItemInfo[i].slot].Reset();
					}
					else
					{
						g_pLocalUser->m_UserItemList[socketitemresult->leftItemInfo[i].slot].Set_m_ucCount(socketitemresult->leftItemInfo[i].number);
					}
				}
			}
#endif
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_cType( socketitemresult->CompleteItem.cType );
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_cSecond( socketitemresult->CompleteItem.cSecond);
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_sID( socketitemresult->CompleteItem.sID );
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_cIdentify = socketitemresult->CompleteItem.Item_Money.S_NormalItem.cIdentify;
			
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_cThird(_XGameItem::GetThird(socketitemresult->CompleteItem.cType, socketitemresult->CompleteItem.sID));
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucCount(socketitemresult->CompleteItem.Item_Money.S_NormalItem.ucCount);
			
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_usCurDur( socketitemresult->CompleteItem.Item_Money.S_NormalItem.usCurDur );
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_usMaxDur( _XGameItem::GetMaxDurability(socketitemresult->CompleteItem.cType, socketitemresult->CompleteItem.sID) );
			
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucSocketCount( socketitemresult->CompleteItem.Item_Money.S_NormalItem.ucSocketCount );
			if( socketitemresult->CompleteItem.Item_Money.S_NormalItem.ucSocketCount > 0 )
			{
				for( int i = 0; i < 4; ++i )
					g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_sSocket( i, socketitemresult->CompleteItem.sSocket[i] );
			}
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucStrength( socketitemresult->CompleteItem.ucStrength );
	#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_usCurProtect  = socketitemresult->CompleteItem.uselimit.usCurProtect; // 바른용린의수
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_usCurMaxProtect = socketitemresult->CompleteItem.uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_usUsedProtect = socketitemresult->CompleteItem.uselimit.usUsedProtect; // 용루 사용여부
			
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucWeaponStrength( _XGameItem::GetNature(socketitemresult->CompleteItem.cType, socketitemresult->CompleteItem.sID) );
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucSharpness( socketitemresult->CompleteItem.ucSharpness );
	#else
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucSharpness( socketitemresult->CompleteItem.ucSharpness );
	#endif


			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucFlag1 = socketitemresult->CompleteItem.uselimit.ucFlag1;

	#ifndef _XDEF_SMELT_ITEM //author : 양희왕 //breif : 디파인 되어있으면 들어오지 않는다
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucFlag2	= socketitemresult->CompleteItem.uselimit.ucFlag2;	
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucFlag3	= socketitemresult->CompleteItem.uselimit.ucFlag3;
	#endif
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_usValue1	= socketitemresult->CompleteItem.uselimit.usValue1;

			if( socketitemresult->CompleteItem.uselimit.ucYear > 0 )
			{				
				g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucYear(socketitemresult->CompleteItem.uselimit.ucYear);	
				g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucMonth(socketitemresult->CompleteItem.uselimit.ucMonth);	
				g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucDay(socketitemresult->CompleteItem.uselimit.ucDay);	
				g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucHour(socketitemresult->CompleteItem.uselimit.ucHour);	
				g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucMinute(socketitemresult->CompleteItem.uselimit.ucMinute);
			}

			LPTSTR param1 = _XGameItem::GetItemName( g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Get_m_cType(),
													 g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Get_m_sID());
			int	   param2 = g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Get_m_ucCount();
			
			InsertChatString(g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_GETSTATUS, param1, &param2 ),
							 _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET);

			g_pLocalUser->UpdateLocalUserAttackLevel();

			if( g_pInventory_Window )
			{
				if( g_pInventory_Window->GetShowStatus() )
				{
					g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_bNewItem = TRUE;
					g_pInventory_Window->m_bDrawGrabItem = TRUE;
				}
			}
			
			if( pGamble_Window->GetShowStatus() )
				pGamble_Window->ShowWindow(FALSE);

			pGamble_Window->m_pBoxOpenButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKITEMTRADE_1756);
			g_pInventory_Window->SetDefaultPosition();
		}
		break;
		case en_socket_gamble_mode_separate_success : //소켓 분리 성공
			{
				MSG_SM_SEPARATE_SUCCESS* pSocketDeatchResult = (MSG_SM_SEPARATE_SUCCESS*)pSocketorGambleResult;

				//타겟아이템, 무기 또는 의상의 처리
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_cType(pSocketDeatchResult->TargetItem.cType);
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_cSecond(pSocketDeatchResult->TargetItem.cSecond);
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_sID(pSocketDeatchResult->TargetItem.sID);
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].m_cIdentify = pSocketDeatchResult->TargetItem.Item_Money.S_NormalItem.cIdentify;
				
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_cThird(_XGameItem::GetThird(pSocketDeatchResult->TargetItem.cType, pSocketDeatchResult->TargetItem.sID));
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_ucCount(pSocketDeatchResult->TargetItem.Item_Money.S_NormalItem.ucCount);
				
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_usCurDur(pSocketDeatchResult->TargetItem.Item_Money.S_NormalItem.usCurDur);
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_usMaxDur(_XGameItem::GetMaxDurability(pSocketDeatchResult->TargetItem.cType, pSocketDeatchResult->TargetItem.sID));
				
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_ucSocketCount( pSocketDeatchResult->TargetItem.Item_Money.S_NormalItem.ucSocketCount );
				if( pSocketDeatchResult->TargetItem.Item_Money.S_NormalItem.ucSocketCount > 0 )
				{
					for( int i = 0; i < 4; ++i )
						g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_sSocket( i, pSocketDeatchResult->TargetItem.sSocket[i] );
				}
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_ucStrength( pSocketDeatchResult->TargetItem.ucStrength );
		#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].m_usCurProtect  = pSocketDeatchResult->TargetItem.uselimit.usCurProtect; // 바른용린의수
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].m_usCurMaxProtect = pSocketDeatchResult->TargetItem.uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].m_usUsedProtect = pSocketDeatchResult->TargetItem.uselimit.usUsedProtect; // 용루 사용여부
				
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_ucWeaponStrength( _XGameItem::GetNature(pSocketDeatchResult->TargetItem.cType, pSocketDeatchResult->TargetItem.sID) );
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_ucSharpness( pSocketDeatchResult->TargetItem.ucSharpness );
		#else
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_ucSharpness( pSocketDeatchResult->TargetItem.ucSharpness );
		#endif


				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].m_ucFlag1 = pSocketDeatchResult->TargetItem.uselimit.ucFlag1;

		#ifndef _XDEF_SMELT_ITEM //author : 양희왕 //breif : 디파인 되어있으면 들어오지 않는다
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].m_ucFlag2	= pSocketDeatchResult->TargetItem.uselimit.ucFlag2;	
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].m_ucFlag3	= pSocketDeatchResult->TargetItem.uselimit.ucFlag3;
		#endif
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].m_usValue1	= pSocketDeatchResult->TargetItem.uselimit.usValue1;

				if( pSocketDeatchResult->TargetItem.uselimit.ucYear > 0 )
				{					
					g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_ucYear(pSocketDeatchResult->TargetItem.uselimit.ucYear);	
					g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_ucMonth(pSocketDeatchResult->TargetItem.uselimit.ucMonth);	
					g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_ucDay(pSocketDeatchResult->TargetItem.uselimit.ucDay);	
					g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_ucHour(pSocketDeatchResult->TargetItem.uselimit.ucHour);	
					g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucTargetSlot].Set_m_ucMinute(pSocketDeatchResult->TargetItem.uselimit.ucMinute);
				}
				//타겟아이템, 무기 또는 의상의 처리 끝

				//분리된 소켓 아이템을 빈슬롯에 만들기
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_cType(pSocketDeatchResult->ResultSocket.cType);
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_cSecond(pSocketDeatchResult->ResultSocket.cSecond);
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_sID( pSocketDeatchResult->ResultSocket.sID );
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].m_cIdentify = pSocketDeatchResult->ResultSocket.Item_Money.S_NormalItem.cIdentify;
				
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_cThird(_XGameItem::GetThird(pSocketDeatchResult->ResultSocket.cType, pSocketDeatchResult->ResultSocket.sID));
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_ucCount(pSocketDeatchResult->ResultSocket.Item_Money.S_NormalItem.ucCount);
				
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_usCurDur( pSocketDeatchResult->ResultSocket.Item_Money.S_NormalItem.usCurDur );
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_usMaxDur( _XGameItem::GetMaxDurability(pSocketDeatchResult->ResultSocket.cType, pSocketDeatchResult->ResultSocket.sID) );
				
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_ucSocketCount( pSocketDeatchResult->ResultSocket.Item_Money.S_NormalItem.ucSocketCount );
				if( pSocketDeatchResult->ResultSocket.Item_Money.S_NormalItem.ucSocketCount > 0 )
				{
					for( int i = 0; i < 4; ++i )
						g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_sSocket( i, pSocketDeatchResult->ResultSocket.sSocket[i] );
				}
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_ucStrength( pSocketDeatchResult->ResultSocket.ucStrength );
		#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].m_usCurProtect  = pSocketDeatchResult->ResultSocket.uselimit.usCurProtect; // 바른용린의수
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].m_usCurMaxProtect = pSocketDeatchResult->ResultSocket.uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].m_usUsedProtect = pSocketDeatchResult->ResultSocket.uselimit.usUsedProtect; // 용루 사용여부
				
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_ucWeaponStrength( _XGameItem::GetNature(pSocketDeatchResult->ResultSocket.cType, pSocketDeatchResult->ResultSocket.sID) );
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_ucSharpness( pSocketDeatchResult->ResultSocket.ucSharpness );
		#else
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_ucSharpness( pSocketDeatchResult->ResultSocket.ucSharpness );
		#endif


				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].m_ucFlag1 = pSocketDeatchResult->ResultSocket.uselimit.ucFlag1;

		#ifndef _XDEF_SMELT_ITEM //author : 양희왕 //breif : 디파인 되어있으면 들어오지 않는다
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].m_ucFlag2	= pSocketDeatchResult->ResultSocket.uselimit.ucFlag2;	
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].m_ucFlag3	= pSocketDeatchResult->ResultSocket.uselimit.ucFlag3;
		#endif
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].m_usValue1	= pSocketDeatchResult->ResultSocket.uselimit.usValue1;

				if( pSocketDeatchResult->ResultSocket.uselimit.ucYear > 0 )
				{					
					g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_ucYear(pSocketDeatchResult->ResultSocket.uselimit.ucYear);	
					g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_ucMonth(pSocketDeatchResult->ResultSocket.uselimit.ucMonth);	
					g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_ucDay(pSocketDeatchResult->ResultSocket.uselimit.ucDay);	
					g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_ucHour(pSocketDeatchResult->ResultSocket.uselimit.ucHour);	
					g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucResultSocketSlot].Set_m_ucMinute(pSocketDeatchResult->ResultSocket.uselimit.ucMinute);
				}
				//분리된 소켓 아이템을 빈슬롯에 만들기 끝

				//장식도구는 삭제
				g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucToolSlot].Set_m_ucCount(pSocketDeatchResult->ucLeftToolCount); // 남은 갯수
			
				// 0이면 삭제
				if( g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucToolSlot].Get_m_ucCount() < 1 )
					g_pLocalUser->m_UserItemList[pSocketDeatchResult->ucToolSlot].Reset();

				g_pLocalUser->UpdateLocalUserAttackLevel();

				if( g_pInventory_Window )
				{
					g_pInventory_Window->m_UsedLifeItemIndex = 0;
					g_pInventory_Window->ResetSocketIndexList();
				}

				g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);

				pSocketDetachWindow->ShowWindow(FALSE); //윈도우 닫기
			
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
				_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3751) ); //장식물이 성공적으로 분리되었습니다.
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, tempString, TRUE);
			}
			break;
		case en_socket_gamble_mode_separate_fail : //소켓 분리 실패
			{
				MSG_SM_SEPARATE_FAIL* pSocketDeatchResult = (MSG_SM_SEPARATE_FAIL*)pSocketorGambleResult;

				pSocketDetachWindow->ShowWindow(FALSE); //윈도우 닫기

				if( g_pInventory_Window )
				{
					g_pInventory_Window->m_UsedLifeItemIndex = 0;
					g_pInventory_Window->ResetSocketIndexList();
				}

				switch( pSocketDeatchResult->ucFailReason )
				{
				case 1 :
					{
						g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
			
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
						_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3752) ); //장식물이 없거나 장착 공간이 없는 물품입니다.
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, tempString, TRUE);
					}
					break;
				case 2 :
					{
						_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3753) ); //평화상태에서만 장식물을 분리할 수 있습니다.
						g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
					}
					break;
				case 3 :
					{
						g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
			
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
						_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3754));//행낭에 여유 공간이 없어서 장식물 분리에 실패하였습니다.
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, tempString, TRUE);
					}
					break;
				}
			}
			break;
	}
}

#else //#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : 양희왕 breif : 소켓제거

void _XNetwork::MSGItemEquipSocketResult( MSG_ITEM_EQUIP_SOCKET_RESULT* socketitemresult )
{
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
	if( pChargeGauge_Window )
	{
		if( pChargeGauge_Window->GetShowStatus() )
			pChargeGauge_Window->StartFadeOutMode();
	}
	
	_XWindow_SocketPlugIn* pSocket_Window = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
	_XWindow_Gamble* pGamble_Window = (_XWindow_Gamble*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GAMBLE);

	if( socketitemresult->cMode == -1 )	// 상자 겜블시	실패
	{		
		if( pGamble_Window )
		{
			// 실패 처리
			g_pInventory_Window->m_UsedLifeItemIndex = 0;
			pGamble_Window->m_GambleBoxItem = NULL;
			g_pInventory_Window->m_iSocketIndexList[0] = 0;
			for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT-1; ++i )
			{
				if( g_pInventory_Window->m_iSocketIndexList[i+1] > 0 )
				{
#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
#else
					if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].IsStackable() )
					{
						g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].m_ucCount++;
					}
#endif
					
					g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
					pGamble_Window->m_GambleKeyList[i].Reset();
				}			
			}

#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
			// 실패 시에는 아이템 처리 하지 않음
/*			for(i = 0 ; i < 4 ; ++i)
			{
				// 남은 아이템 처리
				if(socketitemresult->leftItemInfo[i].slot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
				{
					if(socketitemresult->leftItemInfo[i].number == 0)
					{
						g_pLocalUser->m_UserItemList[socketitemresult->leftItemInfo[i].slot].Reset();
					}
					else
					{
						g_pLocalUser->m_UserItemList[socketitemresult->leftItemInfo[i].slot].m_ucCount = socketitemresult->leftItemInfo[i].number;
					}
				}
			}*/
#endif
			
			if( pGamble_Window->GetShowStatus() )
				pGamble_Window->ShowWindow(FALSE);

			pGamble_Window->m_pBoxOpenButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKITEMTRADE_1756);

			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1757), TRUE);
			pMessageBox->StartTimer( 10000, TRUE );
			_XPlayInterfaceSound(ID_SR_INTERFACE_LEVELFAIL_WAV);			
			return;										
		}
	}
	
	if( pSocket_Window )
	{
		if( socketitemresult->CompleteItem.cSlot == 254 )
		{
			// 실패처리
			g_pInventory_Window->m_UsedLifeItemIndex = 0;
			pSocket_Window->m_IndependentItem = NULL;
			g_pInventory_Window->m_iSocketIndexList[0] = 0;
			for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT; ++i )
			{
				g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
				pSocket_Window->m_SocketItemList[i].Reset();
			}
			
			if( pSocket_Window->GetShowStatus() )
				pSocket_Window->ShowWindow(FALSE);
		
			pSocket_Window->m_SocketPlugInButton->EnableWindow(FALSE);
			pSocket_Window->m_SocketPlugInButton->ShowWindow(FALSE);
			pSocket_Window->m_bChangeButtonMode = FALSE;
			pSocket_Window->m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM); // 물품 내리기

			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1758), TRUE);
			return;
		}
	}		

	if( socketitemresult )
	{
		if( socketitemresult->cMode == 0 )	// 소켓 장착시
		{
			if( pSocket_Window )
			{
				// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
				g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].m_cType, g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_sID()  );
				
				g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Reset();
				g_pInventory_Window->m_UsedLifeItemIndex = 0;
				pSocket_Window->m_IndependentItem = NULL;
				
				// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
				g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].m_cType, g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_sID()  );
				g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Reset();
				g_pInventory_Window->m_iSocketIndexList[0] = 0;
				
				for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT; ++i )
				{
					if( g_pInventory_Window->m_iSocketIndexList[i+1] > 0 )
					{
						// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
						g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].m_cType, g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Get_m_sID()  );
						
						g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Reset();
						g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
						pSocket_Window->m_SocketItemList[i].Reset();
					}			
				}
			}				
		}
		else	// 상자 겜블시
		{
			if( pGamble_Window )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_OPEN_BOX_WAV);

				// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
				g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].m_cType, g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_sID()  );
				
				g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Reset();
				g_pInventory_Window->m_UsedLifeItemIndex = 0;
				pGamble_Window->m_GambleBoxItem = NULL;

				// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
				g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].m_cType, g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_sID()  );
				g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Reset();
				g_pInventory_Window->m_iSocketIndexList[0] = 0;
				
				for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT-1; ++i )
				{
					if( g_pInventory_Window->m_iSocketIndexList[i+1] > 0 )
					{
						// 퀘스트 시작 조건에 아이템 체크가 있으면 시작조건의 아이템인지 체크를 한다.
						g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].m_cType, g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Get_m_sID()  );
						
						if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].IsStackable() )
						{
							if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].m_ucCount == 0 )
								g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[i+1]].Reset();
						}
						
						g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
						pGamble_Window->m_GambleKeyList[i].Reset();
					}			
				}

#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
				for(i = 0 ; i < 4 ; ++i)
				{
					// 남은 아이템 처리
					if(socketitemresult->leftItemInfo[i].slot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
					{
						if(socketitemresult->leftItemInfo[i].number == 0)
						{
							g_pLocalUser->m_UserItemList[socketitemresult->leftItemInfo[i].slot].Reset();
						}
						else
						{
							g_pLocalUser->m_UserItemList[socketitemresult->leftItemInfo[i].slot].m_ucCount = socketitemresult->leftItemInfo[i].number;
						}
					}
				}
#endif
			}			
		}

		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_cType = socketitemresult->CompleteItem.cType;
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_cSecond = socketitemresult->CompleteItem.cSecond;
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_sID( socketitemresult->CompleteItem.sID );
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_cIdentify = socketitemresult->CompleteItem.Item_Money.S_NormalItem.cIdentify;
		
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_cThird = _XGameItem::GetThird(socketitemresult->CompleteItem.cType, socketitemresult->CompleteItem.sID);
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucCount = socketitemresult->CompleteItem.Item_Money.S_NormalItem.ucCount;
		
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_usCurDur(socketitemresult->CompleteItem.Item_Money.S_NormalItem.usCurDur);
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_usMaxDur(_XGameItem::GetMaxDurability(socketitemresult->CompleteItem.cType, socketitemresult->CompleteItem.sID));
		
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucSocketCount(socketitemresult->CompleteItem.Item_Money.S_NormalItem.ucSocketCount);
		if( socketitemresult->CompleteItem.Item_Money.S_NormalItem.ucSocketCount > 0 )
		{
			for( int i = 0; i < 4; ++i )
				g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_sSocket(i, socketitemresult->CompleteItem.sSocket[i]);
		}
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucStrength( socketitemresult->CompleteItem.ucStrength );
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_usCurProtect  = socketitemresult->CompleteItem.uselimit.usCurProtect; // 바른용린의수
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_usCurMaxProtect = socketitemresult->CompleteItem.uselimit.usCurMaxProtect; // 최대강도 증가분  마지막 수치임
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_usUsedProtect = socketitemresult->CompleteItem.uselimit.usUsedProtect; // 용루 사용여부
		
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucWeaponStrength = _XGameItem::GetNature(socketitemresult->CompleteItem.cType, socketitemresult->CompleteItem.sID);
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucSharpness	= socketitemresult->CompleteItem.ucSharpness;
#else
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Set_m_ucSharpness(socketitemresult->CompleteItem.ucSharpness);
#endif


		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucFlag1 = socketitemresult->CompleteItem.uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //author : 양희왕 //breif : 디파인 되어있으면 들어오지 않는다
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucFlag2	= socketitemresult->CompleteItem.uselimit.ucFlag2;	
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucFlag3	= socketitemresult->CompleteItem.uselimit.ucFlag3;
#endif
		g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_usValue1	= socketitemresult->CompleteItem.uselimit.usValue1;

		if( socketitemresult->CompleteItem.uselimit.ucYear > 0 )
		{			
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucYear		= socketitemresult->CompleteItem.uselimit.ucYear;	
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucMonth	= socketitemresult->CompleteItem.uselimit.ucMonth;	
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucDay		= socketitemresult->CompleteItem.uselimit.ucDay;	
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucHour		= socketitemresult->CompleteItem.uselimit.ucHour;	
			g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucMinute	= socketitemresult->CompleteItem.uselimit.ucMinute;
		}

		LPTSTR param1 = _XGameItem::GetItemName( g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_cType, 
												 g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].Get_m_sID());
		int	   param2 = g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_ucCount;
		
		InsertChatString(g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_GETSTATUS, param1, &param2 ),
						 _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET);

		g_pLocalUser->UpdateLocalUserAttackLevel();

		if( g_pInventory_Window )
		{
			if( g_pInventory_Window->GetShowStatus() )
			{
				g_pLocalUser->m_UserItemList[socketitemresult->CompleteItem.cSlot].m_bNewItem = TRUE;
				g_pInventory_Window->m_bDrawGrabItem = TRUE;
			}
		}
	}
	
	if( socketitemresult->cMode == 0 )	// 소켓 장착시
	{
		if( pSocket_Window )
		{
			if( pSocket_Window->GetShowStatus() )
				pSocket_Window->ShowWindow(FALSE);
			
			pSocket_Window->m_SocketPlugInButton->EnableWindow(FALSE);
			pSocket_Window->m_SocketPlugInButton->ShowWindow(FALSE);
			pSocket_Window->m_bChangeButtonMode = FALSE;
			pSocket_Window->m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM); // 물품 내리기
			g_pInventory_Window->SetDefaultPosition();
		}		
	}
	else	// 상자 겜블시
	{
		if( pGamble_Window )
		{
			if( pGamble_Window->GetShowStatus() )
				pGamble_Window->ShowWindow(FALSE);

			pGamble_Window->m_pBoxOpenButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKITEMTRADE_1756);
			g_pInventory_Window->SetDefaultPosition();
		}
	}
}

#endif //#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : 양희왕 breif : 소켓제거

void _XNetwork::CheckValidItem(int slotnumber)
{
	if(g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID() <= 0)
		return;
	
	FLOAT percent = 0;
	if( g_pLocalUser->m_UserItemList[slotnumber].Get_m_usMaxDur() != 0 )
		percent = (FLOAT)g_pLocalUser->m_UserItemList[slotnumber].Get_m_usCurDur() / (FLOAT)g_pLocalUser->m_UserItemList[slotnumber].Get_m_usMaxDur() * 100.0f; // 2004.06.14->oneway48 insert

	if(percent > 0.0f)
	{
		// 내구력 검사
		if(percent <= 15.0f)
		{
			if(g_pState_Window)
			{
				g_pState_Window->InsertPlayerStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID(), g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType(), slotnumber, 1);
				g_pState_Window->SetRedLayer(_XST_ITEM, g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID(), g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType(), slotnumber);
				g_pState_Window->SetIconWarning(_XST_ITEM, g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID(), g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType());
				return;
			}
		}
		else if(percent <= 20.0f)
		{
			// 내구도 하락
			if(g_pState_Window)
			{
				g_pState_Window->InsertPlayerStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID(), g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType(), slotnumber, 1);
				g_pState_Window->SetRedLayer(_XST_ITEM, g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID(), g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType(), slotnumber);
				return;
			}
		}
		else
		{
			// 내구도 정상
			if(g_pState_Window)
			{
				if(g_pState_Window->FindPlayerStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID(), g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType(), slotnumber))
				{
					g_pState_Window->DeletePlayerStateList(_XST_ITEM, g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID(), g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType(), slotnumber);
				}
			}
		}
	} // if(percent > 0.0f)	

	// 능력치 검사
	char cType = g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType();
	short sID = g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID();
	switch( GetEqualItemType(cType) )
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);
			if( WeaponItem )
			{
#ifdef _XTS_FAME
				if( !_XGameItem::CheckFame(WeaponItem[sID].ucFame) )
				{
					if(g_pState_Window)
					{
						g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
						g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
						return;
					}
				}
#endif
				if( WeaponItem[sID].cSecondType == _XGI_SC_WEAPON_SPECIAL )
				{
					if(WeaponItem[sID].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel())
					{
						// 성취 단계
						if(g_pState_Window)
						{
							g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
							g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
							return;
						}
					}
#ifdef _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI
					else if(WeaponItem[sID].sReqJung > g_pLocalUser->m_CharacterInfo.Get_intelligence())
					{
						// 지혜
						if(g_pState_Window)
						{
							g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
							g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
							return;
						}
					}
					else if(WeaponItem[sID].sReqSim > g_pLocalUser->m_CharacterInfo.Get_dexterity())
					{
						// 민첩
						if(g_pState_Window)
						{
							g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
							g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
							return;
						}
					}
#else
					else if(WeaponItem[sID].sReqJung > g_pLocalUser->m_CharacterInfo.Get_zen())
					{
						// 진기
						if(g_pState_Window)
						{
							g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
							g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
							return;
						}
					}
					else if(WeaponItem[sID].sReqSim > g_pLocalUser->m_CharacterInfo.Get_intelligence())
					{
						// 지혜
						if(g_pState_Window)
						{
							g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
							g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
							return;
						}
					}
#endif
				}
				else
				{					
					if(WeaponItem[sID].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel())
					{
						// 성취 단계
						if(g_pState_Window)
						{
							g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
							g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
							return;
						}
					}
					else if(WeaponItem[sID].sReqJung > g_pLocalUser->m_CharacterInfo.Get_strength())
					{
						// 근력
						if(g_pState_Window)
						{
							g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
							g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
							return;
						}
					}
					else if(WeaponItem[sID].sReqSim > g_pLocalUser->m_CharacterInfo.Get_dexterity())
					{
						// 민첩
						if(g_pState_Window)
						{
							g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
							g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
							return;
						}
					}
				}
			}
		}
		break;
	case _XGI_FC_CLOTHES :
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
			{					
#ifdef _XTS_FAME
				if( !_XGameItem::CheckFame(ClothesItem[sID].ucFame) )
				{
					if(g_pState_Window)
					{
						g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
						g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
						return;
					}
				}
#endif

				if(ClothesItem[sID].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel())
				{
					// 성취단계
					if(g_pState_Window)
					{
						g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
						g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
						return;
					}
				}
				else if(ClothesItem[sID].sReqJung > g_pLocalUser->m_CharacterInfo.Get_strength())
				{
					// 근력
					if(g_pState_Window)
					{
						g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
						g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
						return;
					}
				}
				else if(ClothesItem[sID].sReqSim > g_pLocalUser->m_CharacterInfo.Get_dexterity())
				{
					// 민첩
					if(g_pState_Window)
					{
						g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 2);
						g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
						return;
					}
				}
			}
		}
		break;		
	case _XGI_FC_ACCESSORY :
		{
#ifdef _XTS_FAME
			if( !_XGameItem::CheckFame(g_AccessoryItemProperty[sID].ucFame) )
			{
				if(g_pState_Window)
				{
					g_pState_Window->InsertPlayerStateList(_XST_ITEM, sID, cType, slotnumber, 3);
					g_pState_Window->SetRedLayer(_XST_ITEM, sID, cType, slotnumber);
					return;
				}
			}
#endif
		}
		break;
	}	
}

void _XNetwork::MSGItemInchantResult(MSG_INCHANT_PACKET* inchantresult)
{
	switch(inchantresult->ucMode) 
	{
	case inchant_mode_str_req_fail :
		{
			_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
			if( pChargeGauge_Window )
			{
				if( pChargeGauge_Window->GetShowStatus() )
					pChargeGauge_Window->StartFadeOutMode();
			}

			MSG_INCHANT_STR_REQ_FAIL* tempresult = (MSG_INCHANT_STR_REQ_FAIL*)inchantresult;
			if( tempresult )
			{
				// 1 not_item_type,		// 제련할 수 없는 아이템이다.
				// 2 more_need_item,		// 재료가 더 필요하다.
				// 3 more_need_money,		// 돈이 더 필요하다.
				// 4 not_match_needs,		// 필요 정보가 다르다.
				// 5 cannot_more_inchant,	// 더이상 제련할 수 없음
				// 6 wait,					// 잠시후에 다시.
				switch(tempresult->ucFailReason) 
				{
				case 1 :
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1759), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					}
					break;
				case 2 :
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1760), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);						
					}
					break;
				case 3 :
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1761), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);						
					}
					break;
				case 4 :
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1762), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					}
					break;
				case 5 :
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1763), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					}
					break;
				case 6 :
					{
						InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_REQUESTLATER), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					}
					break;
				}

				if( g_pInventory_Window )
				{
					g_pInventory_Window->m_UsedLifeItemIndex = 0;
					g_pInventory_Window->ResetSocketIndexList();
				}
				_XWindow_Inchant* pInchant_Window = (_XWindow_Inchant*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INCHANT);
				if( pInchant_Window )
				{
					pInchant_Window->SetItemInchantStep(US_NONE);
				}
			}
		}
		break;
	case inchant_mode_str_start_inchant :
		{
			MSG_INCHANT_STR_START_INCHANT* tempresult = (MSG_INCHANT_STR_START_INCHANT*)inchantresult;
			if( tempresult )
			{
				_XWindow_Inchant* pInchant_Window = (_XWindow_Inchant*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INCHANT);
				if( pInchant_Window )
				{
					pInchant_Window->SetFirstInchantResult(tempresult->ucResult);
					pInchant_Window->SetItemInchantStep(US_STEP4);
				}
			}
		}
		break;
	case inchant_mode_str_result :
		{
			_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
			if( pChargeGauge_Window )
			{
				if( pChargeGauge_Window->GetShowStatus() )
					pChargeGauge_Window->StartFadeOutMode();
			}
			
			MSG_INCHANT_STR_RESULT* tempresult = (MSG_INCHANT_STR_RESULT*)inchantresult;
			if( tempresult )
			{
				_XWindow_Inchant* pInchant_Window = (_XWindow_Inchant*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INCHANT);
				if( pInchant_Window )
				{
					switch(tempresult->ucResult) 
					{
					case 0 :	// 성공
						{
							TCHAR tempStr[256];
							memset(tempStr, 0, sizeof(tempStr));
							_snprintf( tempStr, sizeof(tempStr), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1764), tempresult->ucInchantGrade );
							InsertChatString( tempStr, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
							_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_YES_WAV);

							g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Set_m_ucCount(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_ucCount() - g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].ucCount);
							if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_ucCount() < 1 )
								g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Reset();

							g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].m_bNewItem = TRUE;
							g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Set_m_ucStrength(tempresult->ucInchantGrade);

							g_pLocalUser->m_Money = tempresult->uiLeftMoney;							

							if( g_pInventory_Window )
							{
								g_pInventory_Window->m_bDrawGrabItem = TRUE;
								g_pInventory_Window->m_UsedLifeItemIndex = 0;
								g_pInventory_Window->ResetSocketIndexList();
							}
							pInchant_Window->SetItemInchantStep(US_NONE);							
						}
						break;
					case 1 :	// 실패 1
						{
							TCHAR tempStr[256];
							char type = g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_cType();
							short sid = g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_sID();

							int		param1 = tempresult->ucInchantGrade+1;
							LPTSTR	param2 = _XGameItem::GetItemName(type, sid);

							strncpy( tempStr, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORKITEMTRADE_1765, &param1, param2 ), sizeof(tempStr) );
							
							InsertChatString( tempStr, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );

							if( tempresult->ucLeftItemCount > 0 )
							{
								_snprintf( tempStr, sizeof(tempStr), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1766), tempresult->ucLeftItemCount );
								InsertChatString( tempStr, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
							}

							_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_NO_WAV);

							g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Reset();
							g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Set_m_ucCount(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_ucCount() - g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].ucCount);

							if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_ucCount() < 1 )
								g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Reset();
							
							if( tempresult->ucLeftItemCount == 0 )	// 이 경우는 없음
								g_pLocalUser->m_UserItemList[tempresult->cSlot].Reset();
							else
							{
								g_pLocalUser->m_UserItemList[tempresult->cSlot].Set_m_cType(_XGI_FC_LIFE);
								g_pLocalUser->m_UserItemList[tempresult->cSlot].Set_m_sID(19);	// 혈정
								g_pLocalUser->m_UserItemList[tempresult->cSlot].Set_m_cSecond(_XGameItem::GetItemSecond(_XGI_FC_LIFE, 19 ));
								g_pLocalUser->m_UserItemList[tempresult->cSlot].Set_m_ucCount(tempresult->ucLeftItemCount );
							}
							g_pLocalUser->m_Money = tempresult->uiLeftMoney;

							if( g_pInventory_Window )
							{
								g_pInventory_Window->m_UsedLifeItemIndex = 0;
								g_pInventory_Window->ResetSocketIndexList();
							}
							pInchant_Window->SetItemInchantStep(US_NONE);						
						}
						break;
					case 2 :	// 실패 2 (완전 실패)
						{
							TCHAR tempStr[256];
							char type = g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_cType();
							short sid = g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_sID();
							_snprintf( tempStr, sizeof(tempStr), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1765), tempresult->ucInchantGrade+1, _XGameItem::GetItemName(type, sid) );
							InsertChatString( tempStr, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
							_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_NO_WAV);
							
							g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Reset();
							g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Set_m_ucCount(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_ucCount() - g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].ucCount);

							if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_ucCount() < 1 )
								g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Reset();

							g_pLocalUser->m_Money = tempresult->uiLeftMoney;

							if( g_pInventory_Window )
							{
								g_pInventory_Window->m_UsedLifeItemIndex = 0;
								g_pInventory_Window->ResetSocketIndexList();
							}
							pInchant_Window->SetItemInchantStep(US_NONE);
						}
						break;
					case 3 : // 실패 3 ( 부분 실패 ) 용린이 사용되어 무기가 파괴되지않음
#ifdef _XDEF_SMELT_ITEM // Author : 양희왕 
						{
							TCHAR tempStr[256];
							char type = g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_cType();
							short sid = g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_sID();

							int		param1 = tempresult->ucInchantGrade+1;
							LPTSTR	param2 = _XGameItem::GetItemName(type, sid);

							//재료 제거인듯
							g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Set_m_ucCount(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_ucCount() - g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].ucCount);
							if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_ucCount() < 1 )
								g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Reset();

							//용루 제거 용루는 0 이하로 떨어지지 않음
							g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].m_usCurMaxProtect = tempresult->ucIncMaxStr;

							//용린이 있는지
							if(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].m_usCurProtect > 0) 
							{
								_snprintf( tempStr, sizeof(tempStr), _XGETINTERFACETEXT(ID_STRING_NEW_3453), param1 ); //%d차 제련에 실패하였습니다.
								InsertChatString( tempStr, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
								_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_NO_WAV);

								g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].m_usCurProtect = tempresult->ucLeftProtectCount; //용린의 남은갯수
							}
							else // 용린이 없으면 파괴한다
							{
								_snprintf( tempStr, sizeof(tempStr), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1765), tempresult->ucInchantGrade+1, _XGameItem::GetItemName(type, sid) );
								InsertChatString( tempStr, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
								_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_NO_WAV);

								g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Reset(); 

								//남은 혈정개수
								if( tempresult->ucLeftItemCount > 0 )
								{
									_snprintf( tempStr, sizeof(tempStr), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1766), tempresult->ucLeftItemCount );
									InsertChatString( tempStr, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
								}
								
								if( tempresult->ucLeftItemCount == 0 )	// 이 경우는 없음
									g_pLocalUser->m_UserItemList[tempresult->cSlot].Reset();
								else
								{
									//혈정 습득
									g_pLocalUser->m_UserItemList[tempresult->cSlot].Set_m_cType(_XGI_FC_LIFE);
									g_pLocalUser->m_UserItemList[tempresult->cSlot].Set_m_sID(19);	// 혈정
									g_pLocalUser->m_UserItemList[tempresult->cSlot].Set_m_cSecond(_XGameItem::GetItemSecond(_XGI_FC_LIFE, 19 ));
									g_pLocalUser->m_UserItemList[tempresult->cSlot].Set_m_ucCount(tempresult->ucLeftItemCount);
								}
							}
							
							g_pLocalUser->m_Money = tempresult->uiLeftMoney;

							if( g_pInventory_Window )
							{
								g_pInventory_Window->m_UsedLifeItemIndex = 0;
								g_pInventory_Window->ResetSocketIndexList();
							}
							pInchant_Window->SetItemInchantStep(US_NONE);
						}
#endif
						break;
					case 4 : // 실패 3 ( 완전 실패 ) 용린이 사용되어 무기가 파괴되지않음
#ifdef _XDEF_SMELT_ITEM // Author : 양희왕 
						{
							TCHAR tempStr[256];
							char type = g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_cType();
							short sid = g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_sID();
							
							//용루 제거 용루는 0 이하로 떨어지지 않음
							g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].m_usCurMaxProtect = tempresult->ucIncMaxStr;

							//용린이 있는지
							if(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].m_usCurProtect > 0) 
							{
								_snprintf( tempStr, sizeof(tempStr), _XGETINTERFACETEXT(ID_STRING_NEW_3453), tempresult->ucInchantGrade+1 );
								InsertChatString( tempStr, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
								_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_NO_WAV);

								g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].m_usCurProtect = tempresult->ucLeftProtectCount; //용린의 남은갯수
							}
							else // 용린이 없으면 파괴한다
							{
								_snprintf( tempStr, sizeof(tempStr), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1765), tempresult->ucInchantGrade+1, _XGameItem::GetItemName(type, sid) );
								InsertChatString( tempStr, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
								_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_NO_WAV);

								g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Reset(); 
							}

							//재료 제거
							g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Set_m_ucCount(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_ucCount() - g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].ucCount);

							if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Get_m_ucCount() < 1 )
								g_pLocalUser->m_UserItemList[g_pInventory_Window->m_iSocketIndexList[0]].Reset();

							g_pLocalUser->m_Money = tempresult->uiLeftMoney;

							if( g_pInventory_Window )
							{
								g_pInventory_Window->m_UsedLifeItemIndex = 0;
								g_pInventory_Window->ResetSocketIndexList();
							}
							pInchant_Window->SetItemInchantStep(US_NONE);
						}
#endif
						break;
					}
				}			
			}
		}
		break;
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕  breif : 용린/용루 시스템	
	case inchant_mode_protect_add_count_success :	// 용린 바르기 성공 s->c
		{
			XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
			MSG_INCHANT_PROTECT_ADD_COUNT_SUCCESS* tempresult = (MSG_INCHANT_PROTECT_ADD_COUNT_SUCCESS*)inchantresult;

			g_pLocalUser->m_UserItemList[tempresult->ucWeaponSlot].m_usCurProtect = tempresult->ucProtectCount; // 최종 무기에 바른 용린수
			g_pLocalUser->m_UserItemList[tempresult->ucProtectItemSlot].Set_m_ucCount( tempresult->ucLeftProtectItem ); // 남은 용린수
			
			// 0이면 삭제
			if( g_pLocalUser->m_UserItemList[tempresult->ucProtectItemSlot].Get_m_ucCount() < 1 )
				g_pLocalUser->m_UserItemList[tempresult->ucProtectItemSlot].Reset();

			if( pSmletItemWindow )
			{
				pSmletItemWindow->SetItemFlag( pSmletItemWindow->GetItemFlag(), ITEMSTEP_NONE );
				pSmletItemWindow->ShowWindow(FALSE);
			}

			if( g_pInventory_Window )
			{
				g_pInventory_Window->m_UsedLifeItemIndex = 0;
				g_pInventory_Window->ResetSocketIndexList();
			}
		
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring));

			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3454) ); // 용린을 발랐습니다.
			InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			
		}
		break;
	case inchant_mode_protect_add_count_fail :	// 용린 바르기 실패 s->c
		{
			XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
			MSG_INCHANT_PROTECT_ADD_COUNT_FAIL* tempresult = (MSG_INCHANT_PROTECT_ADD_COUNT_FAIL*)inchantresult;

			//그냥 하자
			g_pInventory_Window->m_bHaveMouseSlot = FALSE;
			g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();

			if( pSmletItemWindow )
			{
				pSmletItemWindow->SetItemFlag( pSmletItemWindow->GetItemFlag(), ITEMSTEP_NONE );
				pSmletItemWindow->ShowWindow(FALSE);
			}

			if( g_pInventory_Window )
			{
				g_pInventory_Window->m_UsedLifeItemIndex = 0;
				g_pInventory_Window->ResetSocketIndexList();
			}

			/*if(pFindMessageBox)
			{
				if(pFindMessageBox->GetShowStatus())
					break;
			}

			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
			
			if( pMessageBox )
			{
				TCHAR messagestring[256];
				memset( messagestring, 0, sizeof(TCHAR)*256 );

				switch( tempresult->ucFailReason )
				{
				case 0 : //오지않음
					sprintf(messagestring, _T("오지않음.") );  // Add String Table
					break;
				case 1 : //용린이 부족함
					sprintf(messagestring, _T("용린이 부족합니다.") ); // Add String Table
					break;
				case 2 : //용린을 바를 수 없습니다
					sprintf(messagestring, _T("용린을 바를 수 없습니다.") ); // Add String Table
					break;
				case 3 : //무기가 아닙니다 
					sprintf(messagestring, _T("무기가 아닙니다.") ); // Add String Table
					break;
				}
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, messagestring, TRUE);		
			}*/
		}
		break;
	case inchant_mode_protect_inc_str_success :	// 용루 바르기 성공 s->c
		{
			_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
			XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
			
			if( pChargeGauge_Window )
			{
			//	if( pChargeGauge_Window->GetShowStatus() )
				//	pChargeGauge_Window->StartFadeOutMode();
			}
			MSG_INCHANT_PROTECT_INC_STR_SUCCESS* tempresult = (MSG_INCHANT_PROTECT_INC_STR_SUCCESS*)inchantresult;

			//성공 했을 경우 설정 저장
			g_pLocalUser->m_UserItemList[tempresult->ucWeaponSlot].m_usCurMaxProtect = tempresult->ucWeaponIncStr;
			g_pLocalUser->m_UserItemList[tempresult->ucWeaponSlot].m_usUsedProtect = 1;

			//남은 갯수 저장
			g_pLocalUser->m_UserItemList[tempresult->ucIncStrItemSlot].Set_m_ucCount( tempresult->ucLeftIncStrItemCount ); 

			// 0개 일때 없애자
			if(g_pLocalUser->m_UserItemList[tempresult->ucIncStrItemSlot].Get_m_ucCount() <1 )
				g_pLocalUser->m_UserItemList[tempresult->ucIncStrItemSlot].Reset();

			if( pSmletItemWindow )
			{
				pSmletItemWindow->SetItemFlag( pSmletItemWindow->GetItemFlag(), ITEMSTEP_NONE );
				pSmletItemWindow->ShowWindow(FALSE);
			}

			if( g_pInventory_Window )
			{
				g_pInventory_Window->m_UsedLifeItemIndex = 0;
				g_pInventory_Window->ResetSocketIndexList();
			}

			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );

			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3455) ); //용루를 발랐습니다.
			InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
		}
		break;
	case inchant_mode_protect_inc_str_fail :	// 용루 바르기 실패 s->c
		{
			_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
			XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);

			if( pChargeGauge_Window )
			{
			//	if( pChargeGauge_Window->GetShowStatus() )
				//	pChargeGauge_Window->StartFadeOutMode();
			}
			MSG_INCHANT_PROTECT_INC_STR_FAIL* tempresult = (MSG_INCHANT_PROTECT_INC_STR_FAIL*)inchantresult;

			//그냥 하자
			g_pInventory_Window->m_bHaveMouseSlot = FALSE;
			g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();

			if( pSmletItemWindow )
			{
				pSmletItemWindow->SetItemFlag( pSmletItemWindow->GetItemFlag(), ITEMSTEP_NONE );
				pSmletItemWindow->ShowWindow(FALSE);
			}	

			if( g_pInventory_Window )
			{
				g_pInventory_Window->m_UsedLifeItemIndex = 0;
				g_pInventory_Window->ResetSocketIndexList();
			}

			/*if(pFindMessageBox)
			{
				if(pFindMessageBox->GetShowStatus())
					break;
			}

			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
			
			if( pMessageBox )
			{
				TCHAR messagestring[256];
				memset( messagestring, 0, sizeof(TCHAR)*256 );

				switch( tempresult->ucFailReason )
				{
				case 1 : //용루가 없다. 또는 용루가 아니다.
					sprintf(messagestring, _T("용루가 없습니다.") ); 
					break;
				case 2 : //이미 용루를 바른적이 있다.
					sprintf(messagestring, _T("이미 용루를 사용하였습니다.") ); 
					break;
				case 3 : //무기 정보 이상.
					sprintf(messagestring, _T("올 바른 무기가 아닙니다") ); 
					break;
				case 4 : //기타... 
					sprintf(messagestring, _T("예외") ); 
					break;
				}
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, messagestring, TRUE);		
			}*/
		}
		break;
#endif
	case inchant_mode_combine_success :
		{
			MSG_COMBINE_SUCCESS* combinesuccess = (MSG_COMBINE_SUCCESS*)inchantresult;

			int inventoryindex = combinesuccess->WeaponItem.cSlot;

			g_pLocalUser->m_UserItemList[inventoryindex].Set_m_cType( combinesuccess->WeaponItem.cType );
			g_pLocalUser->m_UserItemList[inventoryindex].Set_m_sID( combinesuccess->WeaponItem.sID );
			g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount( combinesuccess->WeaponItem.Item_Money.S_NormalItem.ucCount );
			
			// Second Type setting 
			g_pLocalUser->m_UserItemList[inventoryindex].Set_m_cSecond(_XGameItem::GetItemSecond(combinesuccess->WeaponItem.cType, combinesuccess->WeaponItem.sID));
			
			// 내구력 setting
			g_pLocalUser->m_UserItemList[inventoryindex].Set_m_usMaxDur(
				_XGameItem::GetMaxDurability(combinesuccess->WeaponItem.cType, combinesuccess->WeaponItem.sID) );

			g_pLocalUser->m_UserItemList[inventoryindex].Set_m_usCurDur( combinesuccess->WeaponItem.Item_Money.S_NormalItem.usCurDur );
			
			g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucSocketCount( combinesuccess->WeaponItem.Item_Money.S_NormalItem.ucSocketCount );
			if( combinesuccess->WeaponItem.Item_Money.S_NormalItem.ucSocketCount > 0 )
			{
				for( int i = 0; i < 4; ++i )
					g_pLocalUser->m_UserItemList[inventoryindex].Set_m_sSocket(i, combinesuccess->WeaponItem.sSocket[i] );
			}
			//Author : 양희왕 //breif : 무기 암상인 제련.. 용루 용린에 관련된 부분 추가 //Date : 07/11/12
#ifdef _XDEF_SMELT_ITEM 
			g_pLocalUser->m_UserItemList[inventoryindex].m_usCurProtect = combinesuccess->WeaponItem.uselimit.usCurProtect;
			g_pLocalUser->m_UserItemList[inventoryindex].m_usCurMaxProtect = combinesuccess->WeaponItem.uselimit.usCurMaxProtect;
			g_pLocalUser->m_UserItemList[inventoryindex].m_usUsedProtect = combinesuccess->WeaponItem.uselimit.usUsedProtect;
#endif
			g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag1 = combinesuccess->WeaponItem.uselimit.ucFlag1;
#ifndef _XDEF_SMELT_ITEM //Auhtor : 양희왕 //breif 디파인되어 들어오지 않는다 
			g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag2	= combinesuccess->WeaponItem.uselimit.ucFlag2;	
			g_pLocalUser->m_UserItemList[inventoryindex].m_ucFlag3	= combinesuccess->WeaponItem.uselimit.ucFlag3;
#endif
			g_pLocalUser->m_UserItemList[inventoryindex].m_usValue1	= combinesuccess->WeaponItem.uselimit.usValue1;

			if( combinesuccess->WeaponItem.uselimit.ucYear > 0 )
			{					
				g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucYear( combinesuccess->WeaponItem.uselimit.ucYear);	
				g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucMonth( combinesuccess->WeaponItem.uselimit.ucMonth);	
				g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucDay( combinesuccess->WeaponItem.uselimit.ucDay);	
				g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucHour( combinesuccess->WeaponItem.uselimit.ucHour);	
				g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucMinute( combinesuccess->WeaponItem.uselimit.ucMinute);
			}

			if(combinesuccess->ucLeftAddItem1 > 0)
				g_pLocalUser->m_UserItemList[combinesuccess->ucAddItem1Slot].Set_m_ucCount(combinesuccess->ucLeftAddItem1 );
			else
				g_pLocalUser->m_UserItemList[combinesuccess->ucAddItem1Slot].Reset();

			if(combinesuccess->ucLeftAddItem2 > 0)
				g_pLocalUser->m_UserItemList[combinesuccess->ucAddItem2Slot].Set_m_ucCount( combinesuccess->ucLeftAddItem2 );
			else
				g_pLocalUser->m_UserItemList[combinesuccess->ucAddItem2Slot].Reset();

			g_pLocalUser->m_Money = combinesuccess->uiLeftMoney;

			TCHAR messagestr[256];
			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NETWORK_GETSOMETHING), _XGameItem::GetItemName(combinesuccess->WeaponItem.cType, combinesuccess->WeaponItem.sID));
			InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);

			_XWindow_MixWeapon* pMixWeapon_Window = (_XWindow_MixWeapon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MIXWEAPON_WINDOW);
			if(pMixWeapon_Window)
			{
				pMixWeapon_Window->ResetItemSlot();
			}
		}
		break;
	case inchant_mode_combine_fail :
		{
			MSG_COMBINE_FAIL* combinefail = (MSG_COMBINE_FAIL*)inchantresult;

			switch(combinefail->ucFailReason)
			{
			case 0 :
				{
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3722), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//필요한 금액이 맞지 않습니다.
				}
				break;
			case 2 :
				{
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3723), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//조합할 수 없는 무기 입니다.
				}
				break;
			case 3 :
				{
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3724), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//제련된 아이템은 조합할 수 없습니다.
				}
				break;
			case 1 :
			case 4 :
			case 5 :
			case 6 :
				{
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1760), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//재료가 부족합니다.
				}
				break;
			case 10 :
				{
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1761), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//금액이 부족합니다
				}
				break;
			default :
				{
#ifdef _XDWDEBUG
					TCHAR messagestr[256];
					_snprintf(messagestr, sizeof(messagestr), "Combine Fail reason [%d]", combinefail->ucFailReason);
					InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
#endif
				}
				break;
			}

			_XLog("*** Combine fail [%d]", combinefail->ucFailReason);
		}
		break;
	}
}

void _XNetwork::MSGBloodPointSettingPoint(MSG_BLOODPOINT_SETTING_POINT* setpoint)
{
#ifdef _XTS_BLOODPOINT
	g_pLocalUser->m_BloodPoint = setpoint->sPoint;

	if(g_pLocalUser->m_BloodPoint > 0)
	{
		D3DXMATRIX offsetmat;
		D3DXMatrixIdentity(&offsetmat);
		g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
			_XDEF_MONSTERID_TM254, &offsetmat, NULL, TRUE);
		
		_XPlayInterfaceSound(g_BloodInSoundIndex);
	}
	
	_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
	if(pBloodPoint_Window)
	{
		if(g_pLocalUser->m_BloodPoint >= 999)
		{
			g_pLocalUser->m_BloodPoint = 999;
			pBloodPoint_Window->SetShowReqItem(TRUE);
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2746), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);	// "혈이 완성 되었습니다."
		}
		else
		{
			pBloodPoint_Window->SetDrawAnimation(TRUE);
		}
	}
#endif
}

void _XNetwork::MSGBloodPointGiveItem(MSG_BLOODPOINT_GIVE_ITEM* giveitem)
{
#ifdef _XTS_BLOODPOINT
	int getcount = 0;

	if(g_pLocalUser->m_UserItemList[giveitem->cInsertedSlot].Get_m_cType() == _XGI_FC_LIFE &&
		g_pLocalUser->m_UserItemList[giveitem->cInsertedSlot].Get_m_sID() == 19)
	{
		// 개수 업데이트
		getcount = giveitem->ucCount - g_pLocalUser->m_UserItemList[giveitem->cInsertedSlot].Get_m_ucCount();
		g_pLocalUser->m_UserItemList[giveitem->cInsertedSlot].Set_m_ucCount( giveitem->ucCount );
	}
	else
	{
		g_pLocalUser->m_UserItemList[giveitem->cInsertedSlot].Reset();

		// 혈정 (FType 10 / SType 2 / Id 19)
		g_pLocalUser->m_UserItemList[giveitem->cInsertedSlot].Set_m_cType(_XGI_FC_LIFE );
		g_pLocalUser->m_UserItemList[giveitem->cInsertedSlot].Set_m_cSecond( _XGI_SC_LIFE_INCHANTSTUFF );
		g_pLocalUser->m_UserItemList[giveitem->cInsertedSlot].Set_m_cThird( _XGameItem::GetThird(_XGI_FC_LIFE, 19) );
		g_pLocalUser->m_UserItemList[giveitem->cInsertedSlot].Set_m_sID( 19 );

		g_pLocalUser->m_UserItemList[giveitem->cInsertedSlot].Set_m_ucCount( giveitem->ucCount );

		getcount = giveitem->ucCount;
	}

	if( g_pInventory_Window )
	{
		if( g_pInventory_Window->GetShowStatus() )
		{
			g_pLocalUser->m_UserItemList[giveitem->cInsertedSlot].m_bNewItem = TRUE;
			g_pInventory_Window->m_bDrawGrabItem = TRUE;
		}
	}

	g_pLocalUser->m_BloodPoint = 0;

	_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
	if(pBloodPoint_Window)
	{
		pBloodPoint_Window->SetShowReqItem(FALSE);
		pBloodPoint_Window->SetDrawAnimation(TRUE);
	}
	
	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(messagestring));
	_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2747), getcount);	// "혈정을 %d개 획득했습니다. 행낭을 확인 해 주세요."
	InsertChatString(messagestring,  _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
	_XPlayInterfaceSound(ID_SR_INTERFACE_REBORN_WAV);
#endif
}

void _XNetwork::MSGBloodPointError(MSG_BLOODPOINT_ERROR* blperror)
{
#ifdef _XTS_BLOODPOINT
	if(blperror->ucError == 0)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2748), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "혈이 부족합니다."
	}
	else if(blperror->ucError == 1)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2749), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "행낭에 빈 공간이 없습니다."
	}
#endif
}
