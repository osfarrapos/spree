 // XGameItem.cpp: implementation of the _XGameItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XMeshContainer.h"
#include "XGameItem.h"
#include "XSR_STRINGHEADER.H"
#include "XParticleIDDefine.h"
#include "SoundEffectList.h"
#include "XWindowObjectDefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// _XGameItem
//////////////////////////////////////////////////////////////////////
_XGI_WeaponItem_Property*			g_WeaponItemProperty;
_XGI_ClothesItem_Property*			g_ClothesItemProperty;
_XGI_BookItem_Property*				g_BookItemProperty;
_XGI_PotionItem_Property*			g_PotionItemProperty;
_XGI_ConsumingItem_Property*		g_ConsumingItemProperty;
_XGI_PoisonItem_Property*			g_PoisonItemProperty;
_XGI_CounteractItem_Property*		g_CounteractItemProperty;
_XGI_ResourceItem_Property*			g_ResourceItemProperty;
_XGI_QuestItem_Property*			g_QuestItemProperty;
_XGI_AccessoryItem_Property*		g_AccessoryItemProperty;
_XGI_LifeItem_Property*				g_LifeItemProperty;
_XGI_MusicItem_Property*			g_MusicItemProperty;
_XGI_PassItem_Property*				g_PassItemProperty;
_XGI_SocketItem_Property*			g_SocketItemProperty;
_XGI_WeaponItem_Property*			g_WeaponItemProperty2;
_XGI_ClothesItem_Property*			g_ClothesItemProperty2;
_XGI_ElixirItem_Property*			g_ElixirItemProperty;
_XGI_BoxItem_Property*				g_BoxItemProperty;
_XGI_BoxKeyItem_Property*			g_BoxKeyItemProperty;
_XGI_WeaponItem_Property*			g_WeaponItemProperty3;
_XGI_ClothesItem_Property*			g_ClothesItemProperty3;

_XGI_ITEMTRADE_ITEMPACKAGE			g_ItemTradePackage[150];

_XNicNameInfo*						g_NickNameInfoTable;
short								g_MaxNickNameCount;

short								g_MaxItemCount[_XDEF_ITEMTYPE_COUNT];

_XItemMixTable*						g_ItemMixTable;
_XItemInchantTable*					g_ItemInchantTable;
short								g_maxItemMixTableCount;

#ifdef _XTS_ITEM_V20
TCHAR								g_ItemnameString[256];

#define WEAPON_STARTINDEX			0		// 무기
#define CLOTH_STARTINDEX			10000	// 의상
#define BOOK_STARTINDEX				20000	// 비급
#define POTION_STARTINDEX			25000	// 회복류
#define CONSUMING_STARTINDEX		30000	// 보조제
#define ELIXIR_STARTINDEX			35000	// 영약
#define QUEST_STARTINDEX			40000	// 퀘스트
#define ACCESSORY_STARTINDEX		45000	// 장신구
#define RESOURCE_STARTINDEX			50000	// 자원
#define LIFE_STARTINDEX				55000	// 생활
#define SOCKET_STARTINDEX			60000	// 소켓
#define BOX_STARTINDEX				65000	// 패왕궤
#define BOXKEY_STARTINDEX			70000	// 패왕시

#endif

#define _XDEF_MAXITEMGRADECOLORCOUNT	21

D3DCOLOR g_ItemGradeColorTable[_XDEF_MAXITEMGRADECOLORCOUNT] = 
{
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 135, 201, 249 ),
	D3DCOLOR_ARGB( 255, 59, 146, 235 ),
	D3DCOLOR_ARGB( 255, 255, 204, 0 ),
	D3DCOLOR_ARGB( 255, 255, 120, 0 ),
	D3DCOLOR_ARGB( 255, 117, 185, 58 ),
	
	// 차후 수정 예정
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ), 
	D3DCOLOR_ARGB( 255, 180, 180, 180 ),
	D3DCOLOR_ARGB( 255, 180, 180, 180 )
};

_XGameItem::_XGameItem()
{
	m_ItemWorldID		= -1;
	m_InsideFrustum		= FALSE;
	m_pMeshContainer	= NULL;	

	m_SelfDestroyTimer  =  0;
	m_AdditionalHeight  =  0.0f;

	m_cIdentity			= 0;
	m_cType				= -1;
	m_cSecond			= 0;
	m_sID				= 0;
	m_usOwnerID			= 0;

	m_ucCount			= 0;
	m_uiCount			= 0;
	
	m_LastReceiveMessageTime = 0;
	m_bLiveWorldID		= FALSE; // 2004.06.03->oneway48 insert
}

_XGameItem::~_XGameItem()
{
	g_WorldEffectInstanceManager.DeleteUniqueInstance( m_ItemWorldID );
}

void _XGameItem::Create( int itemworldid, D3DXVECTOR3 position, FLOAT additionalheight, _XAnimMeshAbstractClass* pMeshContainer )
{
	m_ItemWorldID		= itemworldid;
	m_pMeshContainer	= pMeshContainer;
	m_Position			= position;
	m_AdditionalHeight =  additionalheight;
	m_SelfDestroyTimer  = g_LocalSystemTime;

	D3DXMATRIX	PositionMatrix;
	D3DXMatrixTranslation( &PositionMatrix, m_Position.x, m_Position.y, m_Position.z );
	_XParticleGroupItem* pNewInstance = g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_ITEMID_DROP, &PositionMatrix);
	if( pNewInstance )
	{
		pNewInstance->m_UniqueID = m_ItemWorldID;
	}
}

BOOL _XGameItem::LoadItemProperty(void)
{
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		_XDeleteFile(_T("item_table.bit"));
	}		
#endif

	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );

	FILE* pScriptFile = NULL;

#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		
		pScriptFile = fopen( _T("item_table.bit"), "rb" );
			
		if( !pScriptFile )
		{
			bLoadFromFile = FALSE;
			pScriptFile = g_ScriptArchive.GetPackedFile( _T("item_table.bit") );
		}
	}
	else
	{
		pScriptFile = g_ScriptArchive.GetPackedFile( _T("item_table.bit") );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		pScriptFile = fopen( _T("item_table.bit"), "rb" );
		
		if( !pScriptFile )
		{
			bLoadFromFile = FALSE;
			pScriptFile = g_ScriptArchive.GetPackedFile( _T("item_table.bit") );
		}
	}
	else
	{
		pScriptFile = g_ScriptArchive.GetPackedFile( _T("item_table.bit") );
	}
#endif
	
	if( !pScriptFile )
	{
		_XFatalError("Can't Load Item properties table");
		if( bLoadFromFile ) fclose( pScriptFile );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	// header
	char		ToolName[48];
	short		version_1;
	short		version_2;

	SYSTEMTIME	updatetime;
	char		user_name[16];
	char		com_name[20];

	if(fread(ToolName, sizeof(char)*48, 1, pScriptFile) < 1)
	{
		if( bLoadFromFile ) fclose( pScriptFile );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(&version_1, sizeof(short), 1, pScriptFile) < 1)
	{
		if( bLoadFromFile ) fclose( pScriptFile );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(&version_2, sizeof(short), 1, pScriptFile) < 1)
	{
		if( bLoadFromFile ) fclose( pScriptFile );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(&updatetime, sizeof(SYSTEMTIME), 1, pScriptFile) < 1)
	{
		if( bLoadFromFile ) fclose( pScriptFile );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(user_name, sizeof(char)*16, 1, pScriptFile) < 1)
	{
		if( bLoadFromFile ) fclose( pScriptFile );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(com_name, sizeof(char)*20, 1, pScriptFile) < 1)
	{
		if( bLoadFromFile ) fclose( pScriptFile );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	BOOL bContinue = TRUE;
	char cType;
	char size;
	short count;
	short maxindex;
	do 
	{
		if(fread(&cType, sizeof(char), 1, pScriptFile) < 1)
		{
			if( bLoadFromFile ) fclose( pScriptFile );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		if(fread(&size, sizeof(unsigned int), 1, pScriptFile) < 1)
		{
			if( bLoadFromFile ) fclose( pScriptFile );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		if(fread(&count, sizeof(short), 1, pScriptFile) < 1)
		{
			if( bLoadFromFile ) fclose( pScriptFile );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		if(fread(&maxindex, sizeof(short), 1, pScriptFile) < 1)
		{
			if( bLoadFromFile ) fclose( pScriptFile );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		g_MaxItemCount[cType] = maxindex;
		switch(cType)
		{
		case _XGI_FC_WEAPON :
			{
				if(!LoadWeaponItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_CLOTHES :
			{
				if(!LoadClothesItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_BOOK :
			{
				if(!LoadBookItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_POTION :
			{
				if(!LoadPotionItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_CONSUMING :
			{
				if(!LoadConsumingItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_ELIXIR :
			{
				if( !LoadElixirItemProperty(pScriptFile) )
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_COUNTERACT :
			{
				if(!LoadCounteractItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_RESOURCE :
			{
				if(!LoadResourceItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_QUEST :
			{
				if(!LoadQuestItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_ACCESSORY :
			{
				if(!LoadAccessoryItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_LIFE :
			{
				if(!LoadLifeItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_MUSIC :
			{
				if(!LoadMusicItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_PASS :
			{
				if(!LoadPassItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;

		case _XGI_FC_SOCKET :
			{
				if(!LoadSocketItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}				
			}
			break;
		case _XGI_FC_WEAPON2 :
			{
				if(!LoadWeaponItemProperty2(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}				
			}
			break;
		case _XGI_FC_CLOTHES2 :
			{
				if(!LoadClothesItemProperty2(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_BOX :
			{
				if(!LoadBoxItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_BOXKEY :
			{
				if(!LoadBoxKeyItemProperty(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_WEAPON3 :
			{
				if(!LoadWeaponItemProperty3(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
			}
			break;
		case _XGI_FC_CLOTHES3 :
			{
				if(!LoadClothesItemProperty3(pScriptFile))
				{
					if( bLoadFromFile ) fclose( pScriptFile );
					else g_ScriptArchive.ClosePackage();
					return FALSE;
				}
				bContinue = FALSE;
			}
			break;
		}
	} while(bContinue);

	if( bLoadFromFile ) fclose( pScriptFile );
	else g_ScriptArchive.ClosePackage();

	if(!LoadItemTradePackage())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL _XGameItem::LoadWeaponItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	unsigned char ucLength=0;	

	if(g_MaxItemCount[_XGI_FC_WEAPON] == 0)
		return TRUE;

	g_WeaponItemProperty = new _XGI_WeaponItem_Property[g_MaxItemCount[_XGI_FC_WEAPON] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(cType != _XGI_FC_WEAPON || id <= 0 || id > g_MaxItemCount[_XGI_FC_WEAPON])
			return FALSE;

		g_WeaponItemProperty[id].cType = cType;
		g_WeaponItemProperty[id].cSecondType = cSecondType;
		g_WeaponItemProperty[id].sID = id;

#ifdef _XTS_ITEM_V20
		unsigned short usnick;
		if(fread(&usnick, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( usnick > 0 && usnick != 57 )	// 기획팀에서 57번의 닉은 보여주면 안된다고 함. 
		{
			g_WeaponItemProperty[id].cNick = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_NICKNAME, usnick );
		}

		if(fread(&g_WeaponItemProperty[id].usAddto, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= WEAPON_STARTINDEX;
		g_WeaponItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_WEAPON, uiname );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if( ucLength > 0 )
		{
			if(fread(&g_WeaponItemProperty[id].cNick, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
		}		

		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_WeaponItemProperty[id].cThirdType, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		
#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= WEAPON_STARTINDEX;	
		g_WeaponItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_WEAPON, uiiteminfo );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;		
#endif

		if(fread(&g_WeaponItemProperty[id].sMinPower, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].sMaxPower, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usDurability, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( g_WeaponItemProperty[id].usDurability <= 0 )
			g_WeaponItemProperty[id].usDurability = 1;
		if(fread(&g_WeaponItemProperty[id].cBalance, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty[id].cAttackRate, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_WeaponItemProperty[id].sCriticalRate, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_WeaponItemProperty[id].cCriticalRate, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_WeaponItemProperty[id].cSpeed, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].cStrength, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].cRange, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].cNature, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty[id].sReqLevel, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].sReqSim, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].sReqJung, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_WeaponItemProperty[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty[id].cRank, sizeof(char), 1, fileptr) < 1)
			return FALSE;		

		if(fread(&g_WeaponItemProperty[id].sHidden_ID, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].cHidden_Value, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty[id].ucMaxSocket, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty[id].cRepairProb, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].sG_Group, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].sG_Case, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_WeaponItemProperty[id].vecItemPosition[0].x, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].vecItemPosition[0].y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].vecItemPosition[0].z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty[id].vecItemPosition[1].x, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].vecItemPosition[1].y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].vecItemPosition[1].z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty[id].vecItemPosition[2].x, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].vecItemPosition[2].y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].vecItemPosition[2].z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_WeaponItemProperty[id].vecItemPosition[3].x, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].vecItemPosition[3].y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].vecItemPosition[3].z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;	
		
		if(fread(&g_WeaponItemProperty[id].usAdd_SEID[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usAdd_Value[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucAdd_Prob[0], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucAP_Type[0], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usAP_Per[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_WeaponItemProperty[id].usAdd_SEID[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usAdd_Value[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucAdd_Prob[1], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucAP_Type[1], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usAP_Per[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty[id].usAdd_SEID[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usAdd_Value[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucAdd_Prob[2], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucAP_Type[2], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usAP_Per[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty[id].usAdd_SEID[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usAdd_Value[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucAdd_Prob[3], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucAP_Type[3], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usAP_Per[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty[id].usAdd_SEID[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usAdd_Value[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucAdd_Prob[4], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucAP_Type[4], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usAP_Per[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		
		if(fread(&g_WeaponItemProperty[id].usSpecial_ID_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usSpecial_Time_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucSpecial_Value_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucSApply_Type_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usSpecial_Prob_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty[id].usSpecial_ID_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usSpecial_Time_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucSpecial_Value_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucSApply_Type_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usSpecial_Prob_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty[id].usSpecial_ID_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usSpecial_Time_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucSpecial_Value_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucSApply_Type_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usSpecial_Prob_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty[id].usSpecial_ID_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usSpecial_Time_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucSpecial_Value_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucSApply_Type_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].usSpecial_Prob_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;	
		
		if(fread(&g_WeaponItemProperty[id].sGroup, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_WeaponItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_WeaponItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_WeaponItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_WeaponItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_WeaponItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
#ifdef _XDEF_ITEM_V24
		if(fread(&g_WeaponItemProperty[id].ucSocketCheck, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#endif

		if(id == g_MaxItemCount[_XGI_FC_WEAPON])
			break;
	}

	return TRUE;
}

BOOL _XGameItem::LoadWeaponItemProperty2(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	unsigned char ucLength;

	if(g_MaxItemCount[_XGI_FC_WEAPON2] == 0)
		return TRUE;

	g_WeaponItemProperty2 = new _XGI_WeaponItem_Property[g_MaxItemCount[_XGI_FC_WEAPON2] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(cType != _XGI_FC_WEAPON2 || id <= 0 || id > g_MaxItemCount[_XGI_FC_WEAPON2])
			return FALSE;

		g_WeaponItemProperty2[id].cType = cType;
		g_WeaponItemProperty2[id].cSecondType = cSecondType;
		g_WeaponItemProperty2[id].sID = id;

		ucLength = 0;

#ifdef _XTS_ITEM_V20
		unsigned short usnick;
		if(fread(&usnick, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( usnick > 0 && usnick != 57 )	// 기획팀에서 57번의 닉은 보여주면 안된다고 함. 
		{
			g_WeaponItemProperty2[id].cNick = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_NICKNAME, usnick );
		}

		if(fread(&g_WeaponItemProperty2[id].usAddto, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= WEAPON_STARTINDEX;
		g_WeaponItemProperty2[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_WEAPON, uiname );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if( ucLength > 0 )
		{
			if(fread(&g_WeaponItemProperty2[id].cNick, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
		}		

		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_WeaponItemProperty2[id].cThirdType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		
#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= WEAPON_STARTINDEX;		
		g_WeaponItemProperty2[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_WEAPON, uiiteminfo );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;		
#endif

		if(fread(&g_WeaponItemProperty2[id].sMinPower, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].sMaxPower, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usDurability, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( g_WeaponItemProperty2[id].usDurability <= 0 )
			g_WeaponItemProperty2[id].usDurability = 1;
		if(fread(&g_WeaponItemProperty2[id].cBalance, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].cAttackRate, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_WeaponItemProperty2[id].sCriticalRate, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_WeaponItemProperty2[id].cCriticalRate, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_WeaponItemProperty2[id].cSpeed, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].cStrength, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].cRange, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].cNature, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].sReqLevel, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].sReqSim, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].sReqJung, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_WeaponItemProperty2[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty2[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty2[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty2[id].cRank, sizeof(char), 1, fileptr) < 1)
			return FALSE;		

		if(fread(&g_WeaponItemProperty2[id].sHidden_ID, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].cHidden_Value, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty2[id].ucMaxSocket, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty2[id].cRepairProb, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty2[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].sG_Group, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].sG_Case, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_WeaponItemProperty2[id].vecItemPosition[0].x, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].vecItemPosition[0].y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].vecItemPosition[0].z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].vecItemPosition[1].x, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].vecItemPosition[1].y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].vecItemPosition[1].z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].vecItemPosition[2].x, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].vecItemPosition[2].y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].vecItemPosition[2].z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_WeaponItemProperty2[id].vecItemPosition[3].x, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].vecItemPosition[3].y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].vecItemPosition[3].z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;	
		
		if(fread(&g_WeaponItemProperty2[id].usAdd_SEID[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usAdd_Value[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucAdd_Prob[0], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucAP_Type[0], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usAP_Per[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_WeaponItemProperty2[id].usAdd_SEID[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usAdd_Value[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucAdd_Prob[1], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucAP_Type[1], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usAP_Per[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].usAdd_SEID[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usAdd_Value[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucAdd_Prob[2], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucAP_Type[2], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usAP_Per[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].usAdd_SEID[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usAdd_Value[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucAdd_Prob[3], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucAP_Type[3], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usAP_Per[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].usAdd_SEID[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usAdd_Value[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucAdd_Prob[4], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucAP_Type[4], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usAP_Per[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		
		if(fread(&g_WeaponItemProperty2[id].usSpecial_ID_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usSpecial_Time_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucSpecial_Value_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucSApply_Type_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usSpecial_Prob_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].usSpecial_ID_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usSpecial_Time_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucSpecial_Value_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucSApply_Type_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usSpecial_Prob_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].usSpecial_ID_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usSpecial_Time_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucSpecial_Value_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucSApply_Type_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usSpecial_Prob_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].usSpecial_ID_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usSpecial_Time_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucSpecial_Value_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucSApply_Type_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].usSpecial_Prob_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		

		if(fread(&g_WeaponItemProperty2[id].sGroup, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty2[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_WeaponItemProperty2[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_WeaponItemProperty2[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_WeaponItemProperty2[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_WeaponItemProperty2[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_WeaponItemProperty2[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty2[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

#ifdef _XDEF_ITEM_V24
		if(fread(&g_WeaponItemProperty2[id].ucSocketCheck, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#endif

		if(id == g_MaxItemCount[_XGI_FC_WEAPON2])
			break;
	}

	return TRUE;
}

BOOL _XGameItem::LoadWeaponItemProperty3(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	unsigned char ucLength;

	if(g_MaxItemCount[_XGI_FC_WEAPON3] == 0)
		return TRUE;

	g_WeaponItemProperty3 = new _XGI_WeaponItem_Property[g_MaxItemCount[_XGI_FC_WEAPON3] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(cType != _XGI_FC_WEAPON3 || id <= 0 || id > g_MaxItemCount[_XGI_FC_WEAPON3])
			return FALSE;

		g_WeaponItemProperty3[id].cType = cType;
		g_WeaponItemProperty3[id].cSecondType = cSecondType;
		g_WeaponItemProperty3[id].sID = id;

		ucLength = 0;

#ifdef _XTS_ITEM_V20
		unsigned short usnick;
		if(fread(&usnick, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( usnick > 0 && usnick != 57 )	// 기획팀에서 57번의 닉은 보여주면 안된다고 함. 
		{
			g_WeaponItemProperty3[id].cNick = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_NICKNAME, usnick );
		}

		if(fread(&g_WeaponItemProperty3[id].usAddto, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= WEAPON_STARTINDEX;
		g_WeaponItemProperty3[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_WEAPON, uiname );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if( ucLength > 0 )
		{
			if(fread(&g_WeaponItemProperty3[id].cNick, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
		}		

		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_WeaponItemProperty3[id].cThirdType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		
#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= WEAPON_STARTINDEX;		
		g_WeaponItemProperty3[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_WEAPON, uiiteminfo );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;		
#endif

		if(fread(&g_WeaponItemProperty3[id].sMinPower, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].sMaxPower, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usDurability, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( g_WeaponItemProperty3[id].usDurability <= 0 )
			g_WeaponItemProperty3[id].usDurability = 1;
		if(fread(&g_WeaponItemProperty3[id].cBalance, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].cAttackRate, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_WeaponItemProperty3[id].sCriticalRate, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_WeaponItemProperty3[id].cCriticalRate, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_WeaponItemProperty3[id].cSpeed, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].cStrength, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].cRange, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].cNature, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].sReqLevel, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].sReqSim, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].sReqJung, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_WeaponItemProperty3[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty3[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty3[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty3[id].cRank, sizeof(char), 1, fileptr) < 1)
			return FALSE;		

		if(fread(&g_WeaponItemProperty3[id].sHidden_ID, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].cHidden_Value, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty3[id].ucMaxSocket, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty3[id].cRepairProb, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_WeaponItemProperty3[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].sG_Group, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].sG_Case, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_WeaponItemProperty3[id].vecItemPosition[0].x, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].vecItemPosition[0].y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].vecItemPosition[0].z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].vecItemPosition[1].x, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].vecItemPosition[1].y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].vecItemPosition[1].z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].vecItemPosition[2].x, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].vecItemPosition[2].y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].vecItemPosition[2].z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_WeaponItemProperty3[id].vecItemPosition[3].x, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].vecItemPosition[3].y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].vecItemPosition[3].z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;	
		
		if(fread(&g_WeaponItemProperty3[id].usAdd_SEID[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usAdd_Value[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucAdd_Prob[0], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucAP_Type[0], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usAP_Per[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_WeaponItemProperty3[id].usAdd_SEID[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usAdd_Value[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucAdd_Prob[1], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucAP_Type[1], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usAP_Per[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].usAdd_SEID[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usAdd_Value[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucAdd_Prob[2], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucAP_Type[2], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usAP_Per[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].usAdd_SEID[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usAdd_Value[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucAdd_Prob[3], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucAP_Type[3], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usAP_Per[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].usAdd_SEID[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usAdd_Value[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucAdd_Prob[4], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucAP_Type[4], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usAP_Per[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		
		if(fread(&g_WeaponItemProperty3[id].usSpecial_ID_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usSpecial_Time_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucSpecial_Value_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucSApply_Type_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usSpecial_Prob_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].usSpecial_ID_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usSpecial_Time_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucSpecial_Value_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucSApply_Type_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usSpecial_Prob_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].usSpecial_ID_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usSpecial_Time_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucSpecial_Value_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucSApply_Type_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usSpecial_Prob_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].usSpecial_ID_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usSpecial_Time_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucSpecial_Value_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].ucSApply_Type_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].usSpecial_Prob_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		

		if(fread(&g_WeaponItemProperty3[id].sGroup, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_WeaponItemProperty3[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_WeaponItemProperty3[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_WeaponItemProperty3[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_WeaponItemProperty3[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_WeaponItemProperty3[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_WeaponItemProperty3[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_WeaponItemProperty3[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

#ifdef _XDEF_ITEM_V24
		if(fread(&g_WeaponItemProperty3[id].ucSocketCheck, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#endif

		if(id == g_MaxItemCount[_XGI_FC_WEAPON3])
			break;
	}

	return TRUE;
}

BOOL _XGameItem::LoadClothesItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	unsigned char ucLength=0;

	if(g_MaxItemCount[_XGI_FC_CLOTHES] == 0)
		return TRUE;

	g_ClothesItemProperty = new _XGI_ClothesItem_Property[g_MaxItemCount[_XGI_FC_CLOTHES] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(cType != _XGI_FC_CLOTHES || id <= 0 || id > g_MaxItemCount[_XGI_FC_CLOTHES])
			return FALSE;

		g_ClothesItemProperty[id].cType = cType;
		g_ClothesItemProperty[id].cSecondType = cSecondType;
		g_ClothesItemProperty[id].sID = id;

		if(fread(&g_ClothesItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_ClothesItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].sG_Group, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].sG_Case, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= CLOTH_STARTINDEX;		
		g_ClothesItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_CLOTHES, uiiteminfo );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_ClothesItemProperty[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20
		unsigned short usnick;
		if(fread(&usnick, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( usnick > 0 && usnick != 57 )	// 기획팀에서 57번의 닉은 보여주면 안된다고 함.
			g_ClothesItemProperty[id].cNick = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_NICKNAME, usnick );

		if(fread(&g_ClothesItemProperty[id].usAddto, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= CLOTH_STARTINDEX;
		g_ClothesItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_CLOTHES, uiname );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if( ucLength > 0 )
		{
			if(fread(&g_ClothesItemProperty[id].cNick, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
		}

		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_ClothesItemProperty[id].cPrecedence, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;

#ifdef _XDEF_ITEM_V25
		if(fread(&g_ClothesItemProperty[id].ucCharacGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_ClothesItemProperty[id].cSexType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		//if(fread(&g_ClothesItemProperty[id].cAccord, sizeof(char), 1, fileptr) < 1)
		//	return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucGroup, sizeof(unsigned char), 1, fileptr) < 1) //Author : 양희왕 //breif : 08/01/29 cAccord 를 변경
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].cClass, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cRepairProb, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cRank, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].fCenter_X, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].fCenter_Y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].fCenter_Z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_ClothesItemProperty[id].sHidden_ID, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cHidden_Value, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_ClothesItemProperty[id].sDefence, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].sStamina, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cWarm, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].cCool_Resistance, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cHot_Resistance, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cSpirit_Resistance, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cBody_Resistance, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].usDurability, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( g_ClothesItemProperty[id].usDurability <= 0 )
			g_ClothesItemProperty[id].usDurability = 1;
		if(fread(&g_ClothesItemProperty[id].cStrength, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].usAdd_SEID[0] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usAdd_Value[0] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucAdd_Prob[0], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucAP_Type[0] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usAP_Per[0] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].usAdd_SEID[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usAdd_Value[1] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucAdd_Prob[1] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucAP_Type[1] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usAP_Per[1] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].usAdd_SEID[2] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usAdd_Value[2] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucAdd_Prob[2] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucAP_Type[2] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usAP_Per[2] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].usAdd_SEID[3] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usAdd_Value[3] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucAdd_Prob[3] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucAP_Type[3] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usAP_Per[3] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].usAdd_SEID[4] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usAdd_Value[4] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucAdd_Prob[4] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucAP_Type[4] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usAP_Per[4] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].sReqLevel, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].sReqSim, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].sReqJung, sizeof(short), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_ClothesItemProperty[id].cPocket, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		//SPECIAL
		if(fread(&g_ClothesItemProperty[id].usSpecial_ID_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usSpecial_Time_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucSpecial_Value_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucSApply_Type_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usSpecial_Prob_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].usSpecial_ID_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usSpecial_Time_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucSpecial_Value_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucSApply_Type_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usSpecial_Prob_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].usSpecial_ID_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usSpecial_Time_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucSpecial_Value_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucSApply_Type_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usSpecial_Prob_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].usSpecial_ID_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usSpecial_Time_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucSpecial_Value_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].ucSApply_Type_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].usSpecial_Prob_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].ucMaxSocket, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cAttackRate, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cEvasionRate, sizeof(char), 1, fileptr) < 1)
			return FALSE;				

		if(fread(&g_ClothesItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_ClothesItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_ClothesItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_ClothesItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_ClothesItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_ClothesItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

#ifdef _XDEF_ITEM_V24
		if(fread(&g_ClothesItemProperty[id].ucSocketCheck, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#endif

		if(id == g_MaxItemCount[_XGI_FC_CLOTHES])
			break;
	}

	return TRUE;
}

BOOL _XGameItem::LoadClothesItemProperty2(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	unsigned char ucLength=0;

	if(g_MaxItemCount[_XGI_FC_CLOTHES2] == 0)
		return TRUE;

	g_ClothesItemProperty2 = new _XGI_ClothesItem_Property[g_MaxItemCount[_XGI_FC_CLOTHES2] + 1];	
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(cType != _XGI_FC_CLOTHES2 || id <= 0 || id > g_MaxItemCount[_XGI_FC_CLOTHES2])
			return FALSE;
		
		g_ClothesItemProperty2[id].cType = cType;
		g_ClothesItemProperty2[id].cSecondType = cSecondType;
		g_ClothesItemProperty2[id].sID = id;		
		
		if(fread(&g_ClothesItemProperty2[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_ClothesItemProperty2[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].sG_Group, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].sG_Case, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= CLOTH_STARTINDEX;		
		g_ClothesItemProperty2[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_CLOTHES, uiiteminfo );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_ClothesItemProperty2[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20
		unsigned short usnick;
		if(fread(&usnick, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( usnick > 0 && usnick != 57 )	// 기획팀에서 57번의 닉은 보여주면 안된다고 함.
			g_ClothesItemProperty2[id].cNick = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_NICKNAME, usnick );			

		if(fread(&g_ClothesItemProperty2[id].usAddto, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= CLOTH_STARTINDEX;
		g_ClothesItemProperty2[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_CLOTHES, uiname );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if( ucLength > 0 )
		{
			if(fread(&g_ClothesItemProperty2[id].cNick, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
		}

		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_ClothesItemProperty2[id].cPrecedence, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;

#ifdef _XDEF_ITEM_V25
		if(fread(&g_ClothesItemProperty2[id].ucCharacGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_ClothesItemProperty2[id].cSexType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		//if(fread(&g_ClothesItemProperty2[id].cAccord, sizeof(char), 1, fileptr) < 1)
		//	return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucGroup, sizeof(unsigned char), 1, fileptr) < 1) //Author : 양희왕 //breif : 08/01/29 cAccord 를 변경
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].cClass, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cRepairProb, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cRank, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].fCenter_X, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].fCenter_Y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].fCenter_Z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_ClothesItemProperty2[id].sHidden_ID, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cHidden_Value, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_ClothesItemProperty2[id].sDefence, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].sStamina, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cWarm, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].cCool_Resistance, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cHot_Resistance, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cSpirit_Resistance, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cBody_Resistance, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].usDurability, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( g_ClothesItemProperty2[id].usDurability <= 0 )
			g_ClothesItemProperty2[id].usDurability = 1;
		if(fread(&g_ClothesItemProperty2[id].cStrength, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].usAdd_SEID[0] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usAdd_Value[0] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucAdd_Prob[0], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucAP_Type[0] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usAP_Per[0] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].usAdd_SEID[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usAdd_Value[1] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucAdd_Prob[1] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucAP_Type[1] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usAP_Per[1] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].usAdd_SEID[2] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usAdd_Value[2] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucAdd_Prob[2] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucAP_Type[2] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usAP_Per[2] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].usAdd_SEID[3] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usAdd_Value[3] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucAdd_Prob[3] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucAP_Type[3] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usAP_Per[3] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].usAdd_SEID[4] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usAdd_Value[4] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucAdd_Prob[4] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucAP_Type[4] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usAP_Per[4] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].sReqLevel, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].sReqSim, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].sReqJung, sizeof(short), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_ClothesItemProperty2[id].cPocket, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		//SPECIAL
		if(fread(&g_ClothesItemProperty2[id].usSpecial_ID_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usSpecial_Time_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucSpecial_Value_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucSApply_Type_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usSpecial_Prob_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].usSpecial_ID_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usSpecial_Time_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucSpecial_Value_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucSApply_Type_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usSpecial_Prob_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].usSpecial_ID_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usSpecial_Time_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucSpecial_Value_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucSApply_Type_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usSpecial_Prob_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].usSpecial_ID_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usSpecial_Time_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucSpecial_Value_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].ucSApply_Type_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].usSpecial_Prob_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].ucMaxSocket, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cAttackRate, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cEvasionRate, sizeof(char), 1, fileptr) < 1)
			return FALSE;				

		if(fread(&g_ClothesItemProperty2[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty2[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_ClothesItemProperty2[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_ClothesItemProperty2[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_ClothesItemProperty2[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_ClothesItemProperty2[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_ClothesItemProperty2[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty2[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

#ifdef _XDEF_ITEM_V24
		if(fread(&g_ClothesItemProperty2[id].ucSocketCheck, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#endif

		if(id == g_MaxItemCount[_XGI_FC_CLOTHES2])
			break;
	}

	return TRUE;
}

BOOL _XGameItem::LoadClothesItemProperty3(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	unsigned char ucLength=0;

	if(g_MaxItemCount[_XGI_FC_CLOTHES3] == 0)
		return TRUE;

	g_ClothesItemProperty3 = new _XGI_ClothesItem_Property[g_MaxItemCount[_XGI_FC_CLOTHES3] + 1];	
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(cType != _XGI_FC_CLOTHES3 || id <= 0 || id > g_MaxItemCount[_XGI_FC_CLOTHES3])
			return FALSE;
		
		g_ClothesItemProperty3[id].cType = cType;
		g_ClothesItemProperty3[id].cSecondType = cSecondType;
		g_ClothesItemProperty3[id].sID = id;		
		
		if(fread(&g_ClothesItemProperty3[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_ClothesItemProperty3[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].sG_Group, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].sG_Case, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= CLOTH_STARTINDEX;		
		g_ClothesItemProperty3[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_CLOTHES, uiiteminfo );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_ClothesItemProperty3[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;


#ifdef _XTS_ITEM_V20
		unsigned short usnick;
		if(fread(&usnick, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( usnick > 0 && usnick != 57 )	// 기획팀에서 57번의 닉은 보여주면 안된다고 함.
			g_ClothesItemProperty3[id].cNick = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_NICKNAME, usnick );
			
		if(fread(&g_ClothesItemProperty3[id].usAddto, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= CLOTH_STARTINDEX;
		g_ClothesItemProperty3[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_CLOTHES, uiname );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if( ucLength > 0 )
		{
			if(fread(&g_ClothesItemProperty3[id].cNick, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
		}

		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_ClothesItemProperty3[id].cPrecedence, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;

#ifdef _XDEF_ITEM_V25
		if(fread(&g_ClothesItemProperty3[id].ucCharacGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_ClothesItemProperty3[id].cSexType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		//if(fread(&g_ClothesItemProperty3[id].cAccord, sizeof(char), 1, fileptr) < 1)
		//	return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucGroup, sizeof(unsigned char), 1, fileptr) < 1) //Author : 양희왕 //breif : 08/01/29 cAccord 를 변경
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].cClass, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cRepairProb, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cRank, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].fCenter_X, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].fCenter_Y, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].fCenter_Z, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_ClothesItemProperty3[id].sHidden_ID, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cHidden_Value, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_ClothesItemProperty3[id].sDefence, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].sStamina, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cWarm, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].cCool_Resistance, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cHot_Resistance, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cSpirit_Resistance, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cBody_Resistance, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].usDurability, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( g_ClothesItemProperty3[id].usDurability <= 0 )
			g_ClothesItemProperty3[id].usDurability = 1;
		if(fread(&g_ClothesItemProperty3[id].cStrength, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].usAdd_SEID[0] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usAdd_Value[0] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucAdd_Prob[0], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucAP_Type[0] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usAP_Per[0] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].usAdd_SEID[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usAdd_Value[1] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucAdd_Prob[1] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucAP_Type[1] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usAP_Per[1] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].usAdd_SEID[2] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usAdd_Value[2] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucAdd_Prob[2] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucAP_Type[2] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usAP_Per[2] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].usAdd_SEID[3] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usAdd_Value[3] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucAdd_Prob[3] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucAP_Type[3] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usAP_Per[3] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].usAdd_SEID[4] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usAdd_Value[4] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucAdd_Prob[4] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucAP_Type[4] , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usAP_Per[4] , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].sReqLevel, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].sReqSim, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].sReqJung, sizeof(short), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_ClothesItemProperty3[id].cPocket, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		//SPECIAL
		if(fread(&g_ClothesItemProperty3[id].usSpecial_ID_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usSpecial_Time_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucSpecial_Value_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucSApply_Type_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usSpecial_Prob_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].usSpecial_ID_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usSpecial_Time_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucSpecial_Value_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucSApply_Type_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usSpecial_Prob_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].usSpecial_ID_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usSpecial_Time_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucSpecial_Value_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucSApply_Type_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usSpecial_Prob_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].usSpecial_ID_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usSpecial_Time_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucSpecial_Value_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].ucSApply_Type_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].usSpecial_Prob_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].ucMaxSocket, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cAttackRate, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cEvasionRate, sizeof(char), 1, fileptr) < 1)
			return FALSE;				

		if(fread(&g_ClothesItemProperty3[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ClothesItemProperty3[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_ClothesItemProperty3[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_ClothesItemProperty3[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_ClothesItemProperty3[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_ClothesItemProperty3[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_ClothesItemProperty3[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ClothesItemProperty3[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

#ifdef _XDEF_ITEM_V24
		if(fread(&g_ClothesItemProperty3[id].ucSocketCheck, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#endif

		if(id == g_MaxItemCount[_XGI_FC_CLOTHES3])
			break;
	}

	return TRUE;
}


BOOL _XGameItem::LoadBookItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	unsigned char ucLength = 0;
	
	if(g_MaxItemCount[_XGI_FC_BOOK] == 0)
		return TRUE;

	g_BookItemProperty = new _XGI_BookItem_Property[g_MaxItemCount[_XGI_FC_BOOK] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(cType != _XGI_FC_BOOK || id <= 0 || id > g_MaxItemCount[_XGI_FC_BOOK])
			return FALSE;

		g_BookItemProperty[id].cType = cType;
		g_BookItemProperty[id].cSecondType = cSecondType;
		g_BookItemProperty[id].sID = id;

		//IIBK_NAME
#ifdef _XTS_ITEM_V20		
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= BOOK_STARTINDEX;
		g_BookItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_BOOK, uiname );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BookItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		//IIBK_THIRD_TYPE
		if(fread(&g_BookItemProperty[id].cThirdType, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		//IIBK_ITEM_INFO
#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= BOOK_STARTINDEX;		
		g_BookItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_BOOK, uiiteminfo );
#else	
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BookItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif
		
		//IIBK_SKILL_ID
		if(fread(&g_BookItemProperty[id].usSkillID, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		//IIBK_VALUE
		if(fread(&g_BookItemProperty[id].ucValue, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		//IIBK_SKILL_GROUP
		if(fread(&g_BookItemProperty[id].ucSkillGroup, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		//IIBK_SKILL_CLASS
		if(fread(&g_BookItemProperty[id].ucSkillClass, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		//IIBK_SKILL_STEP
		if(fread(&g_BookItemProperty[id].cSkillStep, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		//IIBK_SKILL_LEVEL
		if(fread(&g_BookItemProperty[id].ucSkillLevel, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		
		//IIBK_ABILITY_ID
		if(fread(&g_BookItemProperty[id].usAbilityID, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;				
		//IIBK_QUEST_ID
		if(fread(&g_BookItemProperty[id].usQuestID, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;						
		//IIBK_CLAN
		if(fread(&g_BookItemProperty[id].ucClan, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		
#ifdef _XDEF_ITEM_V25
		if(fread(&g_BookItemProperty[id].ucCharacGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#endif
		
		//IIBK_CLASS
		if(fread(&g_BookItemProperty[id].ucClass, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		
		//IIBK_REQ_SKILL1
		if(fread(&g_BookItemProperty[id].usReqSkill1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		//IIBK_REQ_SKILL1_STEP
		if(fread(&g_BookItemProperty[id].cReqSkill1Step, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		//IIBK_REQ_SKILL2
		if(fread(&g_BookItemProperty[id].usReqSkill2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		//IIBK_REQ_SKILL2_STEP
		if(fread(&g_BookItemProperty[id].cReqSkill2Step, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		//IIBK_REQ_SKILL3
		if(fread(&g_BookItemProperty[id].usReqSkill3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		//IIBK_REQ_SKILL3_STEP
		if(fread(&g_BookItemProperty[id].cReqSkill3Step, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		//IIBK_REQ_ABILITY
		if(fread(&g_BookItemProperty[id].usReqAbility, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		//IIBK_REQ_ABILITY_STEP
		if(fread(&g_BookItemProperty[id].cReqAbilityStep, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		//IIBK_HIDDEN_ID,
		if(fread(&g_BookItemProperty[id].usHiddenID, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;				
		//IIBK_PROB
		if(fread(&g_BookItemProperty[id].cProb, sizeof(char), 1, fileptr) < 1)
			return FALSE;						
		//IIBK_DELAY
		if(fread(&g_BookItemProperty[id].usDelay, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		//IIBK_DELETE
		if(fread(&g_BookItemProperty[id].ucDelete, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BookItemProperty[id].uiApplyTime, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		//IIBK_COOLDOWN
		if(fread(&g_BookItemProperty[id].usCoolDown, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		//IIBK_APPLY_CLAN
		if(fread(&g_BookItemProperty[id].ucApplyClan, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		
		//IIBK_CP1
		if(fread(&g_BookItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;				
		//IIBK_CP2
		if(fread(&g_BookItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		//IIBK_PRICE,
		if(fread(&g_BookItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;				
		//IIBK_MODEL_TYPE,
		if(fread(&g_BookItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;				
		//IIBK_ICON_NO
		if(fread(&g_BookItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;						
		//IIBK_GRADE
		if(fread(&g_BookItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;				
		//IIBK_ITEM_RANK
		if(fread(&g_BookItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;				
		//IIBK_QUALITY, 
		if(fread(&g_BookItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BookItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		//IIBK_REQ_LEVEL
		if(fread(&g_BookItemProperty[id].usReqLevel, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		//IIBK_REQ_STR
		if(fread(&g_BookItemProperty[id].usReq_STR, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;				
		//IIBK_REQ_FORCE
		if(fread(&g_BookItemProperty[id].usReq_Force, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		//IIBK_REQ_INTELL
		if(fread(&g_BookItemProperty[id].usReq_Intell, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;	
		//IIBK_REQ_CONST
		if(fread(&g_BookItemProperty[id].usReq_Const, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;			
		//IIBK_REQ_DEX,
		if(fread(&g_BookItemProperty[id].usReq_Dex, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		//IIBK_LIFE
		if(fread(&g_BookItemProperty[id].usLife, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;				
		//IIBK_FORCE
		if(fread(&g_BookItemProperty[id].usForce, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		//IIBK_CONCENT
		if(fread(&g_BookItemProperty[id].usConcent, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_BookItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BookItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BookItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_BookItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_BookItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_BookItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_BookItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_BookItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_BookItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BookItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(id == g_MaxItemCount[_XGI_FC_BOOK])
			break;
	}

	return TRUE;
}


BOOL _XGameItem::LoadPotionItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	unsigned char ucLength=0;
	
	if(g_MaxItemCount[_XGI_FC_POTION] == 0)
		return TRUE;
	
	g_PotionItemProperty = new _XGI_PotionItem_Property[g_MaxItemCount[_XGI_FC_POTION] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_POTION || id <= 0 || id > g_MaxItemCount[_XGI_FC_POTION])
			return FALSE;
		
		g_PotionItemProperty[id].cType = cType;
		g_PotionItemProperty[id].cSecondType = cSecondType;
		g_PotionItemProperty[id].sID = id;

		if(fread(&g_PotionItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_PotionItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_PotionItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		

#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= POTION_STARTINDEX;		
		g_PotionItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_POTION, uiiteminfo );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_PotionItemProperty[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20		
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= POTION_STARTINDEX;
		g_PotionItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_POTION, uiname );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif
		
		if(fread(&g_PotionItemProperty[id].cClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].sApplyValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].cApplyType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].sEffectId, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].sInCEffectId, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].sVisualEfId, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_PotionItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		

		if(fread(&g_PotionItemProperty[id].sCooldown, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].sReqLevel, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].sSimgi, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].sJungGi, sizeof(short), 1, fileptr) < 1)
			return FALSE;		

		if(fread(&g_PotionItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_PotionItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_PotionItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_PotionItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_PotionItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_PotionItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_PotionItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_PotionItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(id == g_MaxItemCount[_XGI_FC_POTION])
			break;
	}
	
	return TRUE;
}


BOOL _XGameItem::LoadConsumingItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	unsigned char ucLength = 0;
	
	if(g_MaxItemCount[_XGI_FC_CONSUMING] == 0)
		return TRUE;
	
	g_ConsumingItemProperty = new _XGI_ConsumingItem_Property[g_MaxItemCount[_XGI_FC_CONSUMING] + 1];	
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_CONSUMING || id <= 0 || id > g_MaxItemCount[_XGI_FC_CONSUMING])
			return FALSE;
		
		g_ConsumingItemProperty[id].cType = cType;
		g_ConsumingItemProperty[id].cSecondType = cSecondType;
		g_ConsumingItemProperty[id].sID = id;

		//IIC_NICK
#ifdef _XTS_ITEM_V20
		unsigned short usnick;
		if(fread(&usnick, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		g_ConsumingItemProperty[id].cNick = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_NICKNAME, usnick );		
		
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= CONSUMING_STARTINDEX;
		g_ConsumingItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_CONSUMING, uiname );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if( ucLength > 0 )
		{
			if(fread(&g_ConsumingItemProperty[id].cNick, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
		}		
		//IIC_NAME
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif
		//IIC_THIRD_TYPE
		if(fread(&g_ConsumingItemProperty[id].cThird_Type, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		//IIC_MODEL_TYPE
		if(fread(&g_ConsumingItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;				
		//IIC_ICON_NO
		if(fread(&g_ConsumingItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;						
		//IIC_ITEM_RANK
		if(fread(&g_ConsumingItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		//IIC_GRADE
		if(fread(&g_ConsumingItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		//IIC_ITEM_INFO
#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= CONSUMING_STARTINDEX;
		g_ConsumingItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_COMSUMING, uiiteminfo );		
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif
		//IIC_CLAN
		if(fread(&g_ConsumingItemProperty[id].cClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		//IIC_SEX_TYPE
		if(fread(&g_ConsumingItemProperty[id].cSex_Type, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		//IIC_CLASS
		if(fread(&g_ConsumingItemProperty[id].cClass, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		//IIC_SEID
		if(fread(&g_ConsumingItemProperty[id].usSEID, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		//IIC_TIME
		if(fread(&g_ConsumingItemProperty[id].usTime, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		//IIC_QUALITY
		if(fread(&g_ConsumingItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		
		//IIC_QUALITY2
		if(fread(&g_ConsumingItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		
		//IIC_SPEED
		if(fread(&g_ConsumingItemProperty[id].cSpeed, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		//IIC_STRENGTH
		if(fread(&g_ConsumingItemProperty[id].cStrength, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		//IIC_RANGE
		if(fread(&g_ConsumingItemProperty[id].cRange, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_ConsumingItemProperty[id].usAdd_SEID_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].usAdd_Value_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].ucAdd_Prob_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].ucAP_Type_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].usAP_Per_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ConsumingItemProperty[id].usAdd_SEID_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].usAdd_Value_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].ucAdd_Prob_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].ucAP_Type_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].usAP_Per_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ConsumingItemProperty[id].usAdd_SEID_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].usAdd_Value_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].ucAdd_Prob_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].ucAP_Type_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].usAP_Per_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ConsumingItemProperty[id].usAdd_SEID_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].usAdd_Value_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].ucAdd_Prob_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].ucAP_Type_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].usAP_Per_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ConsumingItemProperty[id].usAdd_SEID_5, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].usAdd_Value_5, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].ucAdd_Prob_5, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].ucAP_Type_5, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].usAP_Per_5, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;	
		
		//IIC_REQ_LEVEL
		if(fread(&g_ConsumingItemProperty[id].sReq_Level, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		//IIC_REQ_SIMKI
		if(fread(&g_ConsumingItemProperty[id].sReq_Simki, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		//IIC_REQ_JUNGKI
		if(fread(&g_ConsumingItemProperty[id].sReq_Jungki, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		//IIC_APPLY_CLAN
		if(fread(&g_ConsumingItemProperty[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		//IIC_CP1
		if(fread(&g_ConsumingItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;		
		//IIC_CP2
		if(fread(&g_ConsumingItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		//IIC_PRICE
		if(fread(&g_ConsumingItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;		

		if(fread(&g_ConsumingItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ConsumingItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_ConsumingItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_ConsumingItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_ConsumingItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_ConsumingItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_ConsumingItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ConsumingItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(id == g_MaxItemCount[_XGI_FC_CONSUMING])
			break;
	}
	
	return TRUE;
}

BOOL _XGameItem::LoadCounteractItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	
	if(g_MaxItemCount[_XGI_FC_COUNTERACT] == 0)
		return TRUE;
	
	g_CounteractItemProperty = new _XGI_CounteractItem_Property[g_MaxItemCount[_XGI_FC_COUNTERACT] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_COUNTERACT || id <= 0 || id > g_MaxItemCount[_XGI_FC_COUNTERACT])
			return FALSE;
		
		g_CounteractItemProperty[id].cType = cType;
		g_CounteractItemProperty[id].cSecondType = cSecondType;
		g_CounteractItemProperty[id].sID = id;
		
		if(id == g_MaxItemCount[_XGI_FC_COUNTERACT])
			break;
	}
	
	return TRUE;
}

BOOL _XGameItem::LoadResourceItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	unsigned char ucLength=0;// 2004.06.16->oneway48 insert
	
	if(g_MaxItemCount[_XGI_FC_RESOURCE] == 0)
		return TRUE;
	
	g_ResourceItemProperty = new _XGI_ResourceItem_Property[g_MaxItemCount[_XGI_FC_RESOURCE] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_RESOURCE || id <= 0 || id > g_MaxItemCount[_XGI_FC_RESOURCE])
			return FALSE;
		
		g_ResourceItemProperty[id].cType = cType;
		g_ResourceItemProperty[id].cSecondType = cSecondType;
		g_ResourceItemProperty[id].sID = id;

		if(fread(&g_ResourceItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ResourceItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ResourceItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ResourceItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		
#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= RESOURCE_STARTINDEX;		
		g_ResourceItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_RESOURCE, uiiteminfo );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ResourceItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif
		
		if(fread(&g_ResourceItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ResourceItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_ResourceItemProperty[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ResourceItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ResourceItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ResourceItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20		
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= RESOURCE_STARTINDEX;
		g_ResourceItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_RESOURCE, uiname );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ResourceItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;		
#endif

		if(fread(&g_ResourceItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ResourceItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ResourceItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ResourceItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_ResourceItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_ResourceItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_ResourceItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_ResourceItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_ResourceItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ResourceItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(id == g_MaxItemCount[_XGI_FC_RESOURCE])
			break;
	}
	
	return TRUE;
}


BOOL _XGameItem::LoadQuestItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	unsigned char ucLength=0;// 2004.06.12->oneway48 insert
	
	if(g_MaxItemCount[_XGI_FC_QUEST] == 0)
		return TRUE;
	
	g_QuestItemProperty = new _XGI_QuestItem_Property[g_MaxItemCount[_XGI_FC_QUEST] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_QUEST || id <= 0 || id > g_MaxItemCount[_XGI_FC_QUEST])
			return FALSE;
		
		g_QuestItemProperty[id].cType = cType;
		g_QuestItemProperty[id].cSecondType = cSecondType;
		g_QuestItemProperty[id].sID = id;

		if(fread(&g_QuestItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;		
		
		if(fread(&g_QuestItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_QuestItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_QuestItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_QuestItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_QuestItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= QUEST_STARTINDEX;		
		g_QuestItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_QUEST, uiiteminfo );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_QuestItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_QuestItemProperty[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_QuestItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_QuestItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_QuestItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_QuestItemProperty[id].sQuest_Id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		//IIQ_SE_ID,
		if(fread(&g_QuestItemProperty[id].usSEID, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		//IIQ_VALUE,
		if(fread(&g_QuestItemProperty[id].usValue, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		//IIQ_DELAY,
		if(fread(&g_QuestItemProperty[id].usDelay, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;				
		//IIQ_EFFECT_ID,
		if(fread(&g_QuestItemProperty[id].sEffectID, sizeof(short), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20		
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= QUEST_STARTINDEX;
		g_QuestItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_QUEST, uiname );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if( ucLength != 0 )
		{
			if(fread(g_QuestItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
				return FALSE;
		}
#endif

		if(fread(&g_QuestItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_QuestItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_QuestItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_QuestItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_QuestItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_QuestItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_QuestItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_QuestItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_QuestItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_QuestItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		// insert end
		
		if(id == g_MaxItemCount[_XGI_FC_QUEST])
			break;
	}
	
	return TRUE;
}

BOOL _XGameItem::LoadAccessoryItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	unsigned char ucLength=0;
	
	if(g_MaxItemCount[_XGI_FC_ACCESSORY] == 0)
		return TRUE;
	
	g_AccessoryItemProperty = new _XGI_AccessoryItem_Property[g_MaxItemCount[_XGI_FC_ACCESSORY] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_ACCESSORY || id <= 0 || id > g_MaxItemCount[_XGI_FC_ACCESSORY])
			return FALSE;
		
		g_AccessoryItemProperty[id].cType = cType;
		g_AccessoryItemProperty[id].cSecondType = cSecondType;
		g_AccessoryItemProperty[id].sID = id;		

		if(fread(&g_AccessoryItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_AccessoryItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].sG_Group, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].sG_Case, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= ACCESSORY_STARTINDEX;		
		g_AccessoryItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_ACCESSORY, uiiteminfo );		

		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= ACCESSORY_STARTINDEX;
		g_AccessoryItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_ACCESSORY, uiname );
#else
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
		if(fread(&ucLength, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_AccessoryItemProperty[id].cPack, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].cClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].cRank, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_AccessoryItemProperty[id].usDurability, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_AccessoryItemProperty[id].usAdd_SEID[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usAdd_Value[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucAdd_Prob[0], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucAP_Type[0], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usAP_Per[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;	

		if(fread(&g_AccessoryItemProperty[id].usAdd_SEID[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usAdd_Value[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucAdd_Prob[1], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucAP_Type[1], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usAP_Per[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_AccessoryItemProperty[id].usAdd_SEID[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usAdd_Value[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucAdd_Prob[2], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucAP_Type[2], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usAP_Per[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_AccessoryItemProperty[id].usAdd_SEID[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usAdd_Value[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucAdd_Prob[3], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucAP_Type[3], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usAP_Per[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_AccessoryItemProperty[id].usAdd_SEID[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usAdd_Value[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucAdd_Prob[4], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucAP_Type[4], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usAP_Per[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_AccessoryItemProperty[id].sReqLevel, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V22
		if(fread(&g_AccessoryItemProperty[id].sReqDex, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].sReqStr, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].sReqHealth, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].sReqJingi, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].sReqInt, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_AccessoryItemProperty[id].sReqSim, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].sReqJung, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_AccessoryItemProperty[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;			
		if(fread(&g_AccessoryItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;			
		
		//SPECIAL	
		if(fread(&g_AccessoryItemProperty[id].usSpecial_ID_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usSpecial_Time_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucSpecial_Value_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucSApply_Type_1, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usSpecial_Prob_1, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_AccessoryItemProperty[id].usSpecial_ID_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usSpecial_Time_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucSpecial_Value_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucSApply_Type_2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usSpecial_Prob_2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_AccessoryItemProperty[id].usSpecial_ID_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usSpecial_Time_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucSpecial_Value_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucSApply_Type_3, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usSpecial_Prob_3, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_AccessoryItemProperty[id].usSpecial_ID_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usSpecial_Time_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucSpecial_Value_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].ucSApply_Type_4, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].usSpecial_Prob_4, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_AccessoryItemProperty[id].ucMaxSocket, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		

		if(fread(&g_AccessoryItemProperty[id].sGroup, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_AccessoryItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_AccessoryItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_AccessoryItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_AccessoryItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_AccessoryItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_AccessoryItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_AccessoryItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(id == g_MaxItemCount[_XGI_FC_ACCESSORY])
			break;
	}
	
	return TRUE;
}


BOOL _XGameItem::LoadLifeItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;	
	unsigned char ucLength=0;
	
	if(g_MaxItemCount[_XGI_FC_LIFE] == 0)
		return TRUE;
	
	g_LifeItemProperty = new _XGI_LifeItem_Property[g_MaxItemCount[_XGI_FC_LIFE] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_LIFE || id <= 0 || id > g_MaxItemCount[_XGI_FC_LIFE])
			return FALSE;
		
		g_LifeItemProperty[id].cType = cType;
		g_LifeItemProperty[id].cSecondType = cSecondType;
		g_LifeItemProperty[id].sID = id;

#ifdef _XTS_ITEM_V20		
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= LIFE_STARTINDEX;
		g_LifeItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_LIFE, uiname );
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_LifeItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_LifeItemProperty[id].cThirdType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_LifeItemProperty[id].cClanType, sizeof(char), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= LIFE_STARTINDEX;
		g_LifeItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_LIFE, uiiteminfo );		
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_LifeItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_LifeItemProperty[id].usDurability, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_LifeItemProperty[id].ucCount, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_LifeItemProperty[id].cBalance, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_LifeItemProperty[id].cProb, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_LifeItemProperty[id].cLuck, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_LifeItemProperty[id].cStrength, sizeof(char), 1, fileptr) < 1)
			return FALSE;		

		if(fread(&g_LifeItemProperty[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_LifeItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_LifeItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_LifeItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_LifeItemProperty[id].cRank, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_LifeItemProperty[id].usAbility_ID, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_LifeItemProperty[id].ucMax_Merge, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_LifeItemProperty[id].cRepair_Prob, sizeof(char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_LifeItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;		

		if(fread(&g_LifeItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_LifeItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_LifeItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_LifeItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_LifeItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_LifeItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_LifeItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_LifeItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_LifeItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_LifeItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_LifeItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_LifeItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_LifeItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_LifeItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_LifeItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(id == g_MaxItemCount[_XGI_FC_LIFE])
			break;
	}
	
	return TRUE;
}


BOOL _XGameItem::LoadMusicItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	
	if(g_MaxItemCount[_XGI_FC_MUSIC] == 0)
		return TRUE;
	
	g_MusicItemProperty = new _XGI_MusicItem_Property[g_MaxItemCount[_XGI_FC_MUSIC] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_MUSIC || id <= 0 || id > g_MaxItemCount[_XGI_FC_MUSIC])
			return FALSE;
		
		g_MusicItemProperty[id].cType = cType;
		g_MusicItemProperty[id].cSecondType = cSecondType;
		g_MusicItemProperty[id].sID = id;
		
		if(id == g_MaxItemCount[_XGI_FC_MUSIC])
			break;
	}
	
	return TRUE;
}


BOOL _XGameItem::LoadPassItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;
	
	if(g_MaxItemCount[_XGI_FC_PASS] == 0)
		return TRUE;
	
	g_PassItemProperty = new _XGI_PassItem_Property[g_MaxItemCount[_XGI_FC_PASS] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_PASS || id <= 0 || id > g_MaxItemCount[_XGI_FC_PASS])
			return FALSE;
		
		g_PassItemProperty[id].cType = cType;
		g_PassItemProperty[id].cSecondType = cSecondType;
		g_PassItemProperty[id].sID = id;
		
		if(id == g_MaxItemCount[_XGI_FC_PASS])
			break;
	}
	
	return TRUE;
}

// Socket Item list load
BOOL _XGameItem::LoadSocketItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;	
	unsigned char ucLength=0;
	
	if(g_MaxItemCount[_XGI_FC_SOCKET] == 0)
		return TRUE;
	
	g_SocketItemProperty = new _XGI_SocketItem_Property[g_MaxItemCount[_XGI_FC_SOCKET] + 1];
	
	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_SOCKET || id <= 0 || id > g_MaxItemCount[_XGI_FC_SOCKET])
			return FALSE;
		
		g_SocketItemProperty[id].cType = cType;
		g_SocketItemProperty[id].cSecondType = cSecondType;
		g_SocketItemProperty[id].sID = id;

		if(fread(&g_SocketItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= SOCKET_STARTINDEX;
		g_SocketItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_SOCKET, uiiteminfo );
		
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= SOCKET_STARTINDEX;
		g_SocketItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_SOCKET, uiname );
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

#ifdef _XDEF_ITEM_V23  /*!< @author 양희왕 @breif 아이템 테이블 Thirdtype 추가  */
		if(fread(&g_SocketItemProperty[id].ucThirdType, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_SocketItemProperty[id].sSkill_Effect_ID1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V20
		unsigned short usskillinfo1;
		if(fread(&usskillinfo1 , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( usskillinfo1 > 0 )
		{
			usskillinfo1 -= SOCKET_STARTINDEX;		
			g_SocketItemProperty[id].cSkill_Info1 = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SOCKETSKILLINFO1, usskillinfo1 );
		}		
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cSkill_Info1, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_SocketItemProperty[id].cType1, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sValue1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cProb1, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cAP_Type1, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sAP_Per1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sAP_Count1, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_SocketItemProperty[id].sSkill_Effect_ID2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V20
		unsigned short usskillinfo2;
		if(fread(&usskillinfo2 , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( usskillinfo2 )
		{
			usskillinfo2 -= SOCKET_STARTINDEX;		
			g_SocketItemProperty[id].cSkill_Info2 = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SOCKETSKILLINFO2, usskillinfo2 );
		}		
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cSkill_Info2, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_SocketItemProperty[id].cType2, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sValue2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cProb2, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cAP_Type2, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sAP_Per2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sAP_Count2, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_SocketItemProperty[id].sSkill_Effect_ID3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V20
		unsigned short usskillinfo3;
		if(fread(&usskillinfo3 , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( usskillinfo3 )
		{
			usskillinfo3 -= SOCKET_STARTINDEX;		
			g_SocketItemProperty[id].cSkill_Info3 = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SOCKETSKILLINFO3, usskillinfo3 );
		}		
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cSkill_Info3, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_SocketItemProperty[id].cType3, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sValue3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cProb3, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cAP_Type3, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sAP_Per3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sAP_Count3, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_SocketItemProperty[id].sSkill_Effect_ID4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V20
		unsigned short usskillinfo4;
		if(fread(&usskillinfo4 , sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if( usskillinfo4 )
		{
			usskillinfo4 -= SOCKET_STARTINDEX;		
			g_SocketItemProperty[id].cSkill_Info4 = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SOCKETSKILLINFO4, usskillinfo4 );
		}		
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cSkill_Info4, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_SocketItemProperty[id].cType4, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sValue4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cProb4, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cAP_Type4, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sAP_Per4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sAP_Count4, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_SocketItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		
		if(fread(&g_SocketItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;		

		if(fread(&g_SocketItemProperty[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_SocketItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_SocketItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_SocketItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_SocketItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_SocketItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_SocketItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_SocketItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_SocketItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(id == g_MaxItemCount[_XGI_FC_SOCKET])
			break;
	}
	return TRUE;
}

BOOL _XGameItem::LoadElixirItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;	
	unsigned char ucLength=0;
	
	if(g_MaxItemCount[_XGI_FC_ELIXIR] == 0)
		return TRUE;
	
	g_ElixirItemProperty = new _XGI_ElixirItem_Property[g_MaxItemCount[_XGI_FC_ELIXIR] + 1];

	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_ELIXIR || id <= 0 || id > g_MaxItemCount[_XGI_FC_ELIXIR])
			return FALSE;
		
		g_ElixirItemProperty[id].cType = cType;
		g_ElixirItemProperty[id].cSecondType = cSecondType;
		g_ElixirItemProperty[id].sID = id;

#ifdef _XTS_ITEM_V20		
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= ELIXIR_STARTINDEX;
		g_ElixirItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_ELIXIR, uiname );
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_ElixirItemProperty[id].third_type , sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= ELIXIR_STARTINDEX;		
		g_ElixirItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_ELIXIR, uiiteminfo );
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif
		
		if(fread(&g_ElixirItemProperty[id].ucStep, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].ucReq_Level, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usValue_Min, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usValue_Max, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usCoolDownDelay, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usSuccess_Rate, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ElixirItemProperty[id].usBody_Type[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usBody_Type[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usBody_Type[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usBody_Type[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usBody_Type[4], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usBody_Type[5], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usBody_Type[6], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usBody_Type[7], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ElixirItemProperty[id].ucAddiction, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].ucVisualEffect, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usTemp[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usTemp[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usTemp[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].usTemp[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_ElixirItemProperty[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ElixirItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_ElixirItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_ElixirItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_ElixirItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_ElixirItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_ElixirItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_ElixirItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_ElixirItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(id == g_MaxItemCount[_XGI_FC_ELIXIR])
			break;
	}
	return TRUE;
}

BOOL _XGameItem::LoadBoxItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;	
	unsigned char ucLength=0;
	
	if(g_MaxItemCount[_XGI_FC_BOX] == 0)
		return TRUE;
	
	g_BoxItemProperty = new _XGI_BoxItem_Property[g_MaxItemCount[_XGI_FC_BOX] + 1];

	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_BOX || id <= 0 || id > g_MaxItemCount[_XGI_FC_BOX])
			return FALSE;
		
		g_BoxItemProperty[id].cType = cType;
		g_BoxItemProperty[id].cSecondType = cSecondType;
		g_BoxItemProperty[id].sID = id;

#ifdef _XTS_ITEM_V20		
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= BOX_STARTINDEX;
		g_BoxItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_BOX, uiname );
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_BoxItemProperty[id].third_type , sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;

#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= BOX_STARTINDEX;		
		g_BoxItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_BOX, uiiteminfo );
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_BoxItemProperty[id].usTemp0, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_BoxItemProperty[id].ucCase[0], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sCaseProb[0], sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].ucCase[1], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sCaseProb[1], sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].ucCase[2], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sCaseProb[2], sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].ucCase[3], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sCaseProb[3], sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].ucCase[4], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sCaseProb[4], sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].ucCase[5], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sCaseProb[5], sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].ucCase[6], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sCaseProb[6], sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].ucCase[7], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sCaseProb[7], sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].ucCase[8], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sCaseProb[8], sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].ucCase[9], sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sCaseProb[9], sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_BoxItemProperty[id].ucVisualEffect, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].usTemp[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].usTemp[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].usTemp[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].usTemp[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
	
		if(fread(&g_BoxItemProperty[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_BoxItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_BoxItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_BoxItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_BoxItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_BoxItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_BoxItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_BoxItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(id == g_MaxItemCount[_XGI_FC_BOX])
			break;
	}
	return TRUE;
}
BOOL _XGameItem::LoadBoxKeyItemProperty(FILE* fileptr)
{
	char	cType;
	char	cSecondType;
	short	id;	
	unsigned char ucLength=0;
	
	if(g_MaxItemCount[_XGI_FC_BOXKEY] == 0)
		return TRUE;
	
	g_BoxKeyItemProperty = new _XGI_BoxKeyItem_Property[g_MaxItemCount[_XGI_FC_BOXKEY] + 1];

	while(1)
	{
		if(fread(&cType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&cSecondType, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&id, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		
		if(cType != _XGI_FC_BOXKEY || id <= 0 || id > g_MaxItemCount[_XGI_FC_BOXKEY])
			return FALSE;
		
		g_BoxKeyItemProperty[id].cType = cType;
		g_BoxKeyItemProperty[id].cSecondType = cSecondType;
		g_BoxKeyItemProperty[id].sID = id;

#ifdef _XTS_ITEM_V20		
		unsigned int uiname;
		if(fread(&uiname, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiname -= BOXKEY_STARTINDEX;
		g_BoxKeyItemProperty[id].cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_BOXKEY, uiname );
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].cName, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif

		if(fread(&g_BoxKeyItemProperty[id].sModelIndex, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].sIcon_No, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].usItem_Rank, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].ucGrade, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].ucQuality, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].ucQuality2, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V20
		unsigned int uiiteminfo;
		if(fread(&uiiteminfo, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;
		uiiteminfo -= BOXKEY_STARTINDEX;		
		g_BoxKeyItemProperty[id].cItemInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_SEED_INFO_BOXKEY, uiiteminfo );
#else
		if(fread(&ucLength , sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].cItemInfo, sizeof(char)*ucLength, 1, fileptr) < 1)
			return FALSE;
#endif
			
		if(fread(&g_BoxKeyItemProperty[id].ucProbWeapon, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].ucProbClothes, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].ucProbEtc, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_BoxKeyItemProperty[id].usTemp[0], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].usTemp[1], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].usTemp[2], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].usTemp[3], sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&g_BoxKeyItemProperty[id].cApplyClan, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].sClanPoint1, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].sClanPoint2, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].uiPrice, sizeof(unsigned int), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_BoxKeyItemProperty[id].cDump, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].cUserTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].cNPCTrade, sizeof(char), 1, fileptr) < 1)
			return FALSE;

		if(fread(&g_BoxKeyItemProperty[id].ucFame, sizeof(unsigned char), 1, fileptr) < 1)
			return FALSE;
#ifdef _XTS_ITEM_V21
		if(fread(&g_BoxKeyItemProperty[id].cCashCheck, sizeof(char), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_BoxKeyItemProperty[id].usTempField2, sizeof(unsigned short), 1, fileptr) < 1)
			return FALSE;
#endif
#ifdef _XDEF_PERIODITEM_20061027
		if(fread(&g_BoxKeyItemProperty[id].usTimeValue, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#else
		if(fread(&g_BoxKeyItemProperty[id].sTempField3, sizeof(short), 1, fileptr) < 1)
			return FALSE;
#endif
		if(fread(&g_BoxKeyItemProperty[id].sTempField4, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&g_BoxKeyItemProperty[id].sTempField5, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(id == g_MaxItemCount[_XGI_FC_BOXKEY])
			break;
	}

	return TRUE;
}

// Item Trade list load
BOOL _XGameItem::LoadItemTradePackage(void)
{
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		_XDeleteFile(_T("Default.XIP"));
	}	
#endif

	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );

	FILE* pScriptFile = NULL;

#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		
		pScriptFile = fopen( _T("Default.XIP"), "rb" );
			
		if( !pScriptFile )
		{
			bLoadFromFile = FALSE;
			pScriptFile = g_ScriptArchive.GetPackedFile( _T("Default.XIP") );
		}
	}
	else
	{
		pScriptFile = g_ScriptArchive.GetPackedFile( _T("Default.XIP") );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		pScriptFile = fopen( _T("Default.XIP"), "rb" );
		
		if( !pScriptFile )
		{
			bLoadFromFile = FALSE;
			pScriptFile = g_ScriptArchive.GetPackedFile( _T("Default.XIP") );
		}
	}
	else
	{
		pScriptFile = g_ScriptArchive.GetPackedFile( _T("Default.XIP") );
	}
#endif

	if( !pScriptFile )
	{
		_XFatalError("Can't load item trade list");
		if(bLoadFromFile) fclose( pScriptFile );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	int packagecount = 0;
	fread(&packagecount, sizeof(int), 1, pScriptFile);
	
	if(packagecount <= 0)
	{
		if(bLoadFromFile) fclose( pScriptFile );
		else g_ScriptArchive.ClosePackage();
		return TRUE;
	}
	
	for(int i = 0 ; i < packagecount ; i++)
	{
		g_ItemTradePackage[i].packageID = i;
		fread(g_ItemTradePackage[i].packageName, sizeof(char)*32, 1, pScriptFile);
		fread(&g_ItemTradePackage[i].itemCount, sizeof(int), 1, pScriptFile);
		for(int j = 0 ; j < g_ItemTradePackage[i].itemCount ; j++)
		{
			fread(&g_ItemTradePackage[i].itemUnit[j].category, sizeof(int), 1, pScriptFile);
			fread(&g_ItemTradePackage[i].itemUnit[j].uniqueID, sizeof(int), 1, pScriptFile);
		}
	}
	
	if(bLoadFromFile) fclose( pScriptFile );
	else g_ScriptArchive.ClosePackage();

	return TRUE;
}

BOOL _XGameItem::ReLoadNickNameInfoTable(void)//별호 테이블을 스트링만 다시 설정
{
	for( int i = 0; i < g_MaxNickNameCount; i++ )
	{
#ifdef _XDEF_NEWSTRINGMANAGER
		g_NickNameInfoTable[i].cNickName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_NICNAMEREF_TXT_TITLE, g_NickNameInfoTable[i].usIndex );
		g_NickNameInfoTable[i].cNickInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_NICNAMEREF_TXT_INFO, g_NickNameInfoTable[i].usIndex );
		g_NickNameInfoTable[i].cNickInfoDetail = g_StringDataBase.GetTextString( _XSTRINGDB_SN_NICNAMEREF_TXT_DETAILINFO, g_NickNameInfoTable[i].usIndex );
#endif
	}

	return TRUE;
}

BOOL _XGameItem::LoadNickNameInfoTable(void)
{
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

	FILE* pScript = NULL;

	pScript = g_ScriptArchive.GetPackedFile( _T("NicNameRef.txt") );

	if( !pScript )
	{
		_XFatalError("Load NickNameInfo [%s] file open", _T("NicNameRef.txt"));
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	char buf[2048] = {0} ;
	
	if(fgets(buf, sizeof(buf), pScript) == NULL) 
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	g_MaxNickNameCount = atoi( buf );
	if( g_MaxNickNameCount < 1 ) 
	{
		_XFatalError("Load NickNameInfo : max nickname count %d", g_MaxNickNameCount );
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	g_NickNameInfoTable = new _XNicNameInfo[g_MaxNickNameCount + 1];
	
	int nRead = 1 ;
	int i = 0 ;
	bool bFRead = TRUE ;
	int tempMaxCount = 0;
	TCHAR tempbuffer[512];
	
	while( nRead )
	{
		if(fgets(buf, sizeof(buf), pScript) == NULL) 
		{			
			break;		
		}
		
		if( strncmp( buf, _T("_XM_EOS"), 7 ) == 0 ) break;

		if( buf[0] == ';' || buf[0] == '\n' || strlen(buf) == 0 )
		{			
			continue ;
		}
		
		//_XLog( _T("%d, %s"), i, buf );

#ifdef _XDEF_NEWSTRINGMANAGER

		nRead = sscanf( buf, "%d %s %d %s %s %d %d %d %d %d %d %d %d %d %d \
							  %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
							  &g_NickNameInfoTable[i].usIndex, tempbuffer, &g_NickNameInfoTable[i].cNickIconId, 
							  tempbuffer, tempbuffer, &g_NickNameInfoTable[i].cApplyType,
							  &g_NickNameInfoTable[i].cType1, &g_NickNameInfoTable[i].cType2, &g_NickNameInfoTable[i].cType3,
							  &g_NickNameInfoTable[i].cNickGroup, &g_NickNameInfoTable[i].cNickGrade, &g_NickNameInfoTable[i].cClanType,
							  &g_NickNameInfoTable[i].cSexType, &g_NickNameInfoTable[i].cDuplicate, &g_NickNameInfoTable[i].cPickupCnt,
							  &g_NickNameInfoTable[i]._Effect[0].usEffectIndex, &g_NickNameInfoTable[i]._Effect[0].usEffectValue, &g_NickNameInfoTable[i]._Effect[0].cEffectProbability,
							  &g_NickNameInfoTable[i]._Effect[1].usEffectIndex, &g_NickNameInfoTable[i]._Effect[1].usEffectValue, &g_NickNameInfoTable[i]._Effect[1].cEffectProbability,
							  &g_NickNameInfoTable[i]._Effect[2].usEffectIndex, &g_NickNameInfoTable[i]._Effect[2].usEffectValue, &g_NickNameInfoTable[i]._Effect[2].cEffectProbability,
							  &g_NickNameInfoTable[i]._Effect[3].usEffectIndex, &g_NickNameInfoTable[i]._Effect[3].usEffectValue, &g_NickNameInfoTable[i]._Effect[3].cEffectProbability,
							  &g_NickNameInfoTable[i]._Effect[4].usEffectIndex, &g_NickNameInfoTable[i]._Effect[4].usEffectValue, &g_NickNameInfoTable[i]._Effect[4].cEffectProbability,
							  &g_NickNameInfoTable[i].sIconNumber ) ;

		g_NickNameInfoTable[i].cNickName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_NICNAMEREF_TXT_TITLE, g_NickNameInfoTable[i].usIndex );
		g_NickNameInfoTable[i].cNickInfo = g_StringDataBase.GetTextString( _XSTRINGDB_SN_NICNAMEREF_TXT_INFO, g_NickNameInfoTable[i].usIndex );
		g_NickNameInfoTable[i].cNickInfoDetail = g_StringDataBase.GetTextString( _XSTRINGDB_SN_NICNAMEREF_TXT_DETAILINFO, g_NickNameInfoTable[i].usIndex );

		if( nRead < 30 )
		{
			bFRead = FALSE ;
			_XLog("Invalid nickname parameter %d", nRead );
			break ;
		}
#else
		nRead = sscanf( buf, "%d %s %d %s %s %d %d %d %d %d %d %d %d %d %d \
							  %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
							  &g_NickNameInfoTable[i].usIndex, g_NickNameInfoTable[i].cNickName, &g_NickNameInfoTable[i].cNickIconId, 
							  g_NickNameInfoTable[i].cNickInfo, g_NickNameInfoTable[i].cNickInfoDetail, &g_NickNameInfoTable[i].cApplyType,
							  &g_NickNameInfoTable[i].cType1, &g_NickNameInfoTable[i].cType2, &g_NickNameInfoTable[i].cType3,
							  &g_NickNameInfoTable[i].cNickGroup, &g_NickNameInfoTable[i].cNickGrade, &g_NickNameInfoTable[i].cClanType,
							  &g_NickNameInfoTable[i].cSexType, &g_NickNameInfoTable[i].cDuplicate, &g_NickNameInfoTable[i].cPickupCnt,
							  &g_NickNameInfoTable[i]._Effect[0].usEffectIndex, &g_NickNameInfoTable[i]._Effect[0].usEffectValue, &g_NickNameInfoTable[i]._Effect[0].cEffectProbability,
							  &g_NickNameInfoTable[i]._Effect[1].usEffectIndex, &g_NickNameInfoTable[i]._Effect[1].usEffectValue, &g_NickNameInfoTable[i]._Effect[1].cEffectProbability,
							  &g_NickNameInfoTable[i]._Effect[2].usEffectIndex, &g_NickNameInfoTable[i]._Effect[2].usEffectValue, &g_NickNameInfoTable[i]._Effect[2].cEffectProbability,
							  &g_NickNameInfoTable[i]._Effect[3].usEffectIndex, &g_NickNameInfoTable[i]._Effect[3].usEffectValue, &g_NickNameInfoTable[i]._Effect[3].cEffectProbability,
							  &g_NickNameInfoTable[i]._Effect[4].usEffectIndex, &g_NickNameInfoTable[i]._Effect[4].usEffectValue, &g_NickNameInfoTable[i]._Effect[4].cEffectProbability,
							  &g_NickNameInfoTable[i].sIconNumber ) ;
	
		for(int j = 0; j < 30; j++)
		{
			if( g_NickNameInfoTable[i].cNickName[j] == '_' )
				g_NickNameInfoTable[i].cNickName[j] = ' ';			
		}						  
		for( j = 0; j < 128; j++)
		{
			if( g_NickNameInfoTable[i].cNickInfo[j] == '_' )
				g_NickNameInfoTable[i].cNickInfo[j] = ' ';
			
			if( g_NickNameInfoTable[i].cNickInfoDetail[j] == '_' )
				g_NickNameInfoTable[i].cNickInfoDetail[j] = ' ';	
		}

		if( nRead < 30 )
		{
			bFRead = FALSE ;
			_XLog("Invalid nickname parameter %d", nRead );
			break ;
		}

#endif			
		tempMaxCount ++;
		++i ;
	}	

	if( tempMaxCount != g_MaxNickNameCount )
	{
		_XFatalError("diff MaxCount total:%d,read:%d", g_MaxNickNameCount, tempMaxCount );
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	g_ScriptArchive.ClosePackage();	
		
	return bFRead ;
}

void _XGameItem::ReleaseItemProperty(void)
{
	if(g_WeaponItemProperty)
	{
		delete[] g_WeaponItemProperty;
		g_WeaponItemProperty = NULL;
	}
	if(g_ClothesItemProperty)
	{
		delete[] g_ClothesItemProperty;
		g_ClothesItemProperty = NULL;
	}
	if(g_BookItemProperty)
	{
		delete[] g_BookItemProperty;
		g_BookItemProperty = NULL;
	}
	if(g_PotionItemProperty)
	{
		delete[] g_PotionItemProperty;
		g_PotionItemProperty = NULL;
	}
	if(g_ConsumingItemProperty)
	{
		delete[] g_ConsumingItemProperty;
		g_ConsumingItemProperty = NULL;
	}
	if(g_PoisonItemProperty)
	{
		delete[] g_PoisonItemProperty;
		g_PoisonItemProperty = NULL;
	}
	if(g_CounteractItemProperty)
	{
		delete[] g_CounteractItemProperty;
		g_CounteractItemProperty = NULL;
	}
	if(g_ResourceItemProperty)
	{
		delete[] g_ResourceItemProperty;
		g_ResourceItemProperty = NULL;
	}
	if(g_QuestItemProperty)
	{
		delete[] g_QuestItemProperty;
		g_QuestItemProperty = NULL;
	}
	if(g_AccessoryItemProperty)
	{
		delete[] g_AccessoryItemProperty;
		g_AccessoryItemProperty = NULL;
	}
	if(g_LifeItemProperty)
	{
		delete[] g_LifeItemProperty;
		g_LifeItemProperty = NULL;
	}
	if(g_MusicItemProperty)
	{
		delete[] g_MusicItemProperty;
		g_MusicItemProperty = NULL;
	}
	if(g_PassItemProperty)
	{
		delete[] g_PassItemProperty;
		g_PassItemProperty = NULL;
	}
	if(g_WeaponItemProperty2)
	{
		delete[] g_WeaponItemProperty2;
		g_WeaponItemProperty2 = NULL;
	}
	if(g_ClothesItemProperty2)
	{
		delete[] g_ClothesItemProperty2;
		g_ClothesItemProperty2 = NULL;
	}
	if( g_BoxItemProperty )
	{
		delete[] g_BoxItemProperty;
		g_BoxItemProperty = NULL;
	}
	if( g_BoxKeyItemProperty )
	{
		delete[] g_BoxKeyItemProperty;
		g_BoxKeyItemProperty = NULL;
	}
	if(g_WeaponItemProperty3)
	{
		delete[] g_WeaponItemProperty3;
		g_WeaponItemProperty3 = NULL;
	}
	if(g_ClothesItemProperty3)
	{
		delete[] g_ClothesItemProperty3;
		g_ClothesItemProperty3 = NULL;
	}
	if(g_NickNameInfoTable)
	{
		delete[] g_NickNameInfoTable;
		g_NickNameInfoTable = NULL;
	}
	if( g_SocketItemProperty )
	{
		delete[] g_SocketItemProperty;
		g_SocketItemProperty = NULL;
	}
	if( g_ItemMixTable )
	{
		delete[] g_ItemMixTable;
		g_ItemMixTable = NULL;
	}
	if( g_ItemInchantTable )
	{
		delete[] g_ItemInchantTable;
		g_ItemInchantTable = NULL;
	}

	SAFE_DELETE_ARRAY( g_ElixirItemProperty );	
}

BOOL _XGameItem::LoadItemMixTable(void)
{
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

	FILE* pScript = NULL;

	pScript = g_ScriptArchive.GetPackedFile( _T("item_mix_table.txt") );

	if( !pScript )
	{
		_XFatalError("Load Item Mix Info [%s] file open", _T("item_mix_table.txt"));
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	char buffer[256];
	BOOL bReadCount = FALSE;
	int  count = 0;

	while(!feof(pScript))
	{
		memset( buffer, 0, 256 );
		if(!fgets(buffer, 256, pScript))
			break;

		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		if( buffer[0] == ';' || buffer[0] == '\n' || !strlen(buffer) ) continue;

		if( !bReadCount )
		{
			sscanf(buffer, "%d", &g_maxItemMixTableCount);
			if( g_maxItemMixTableCount > 0 )
			{
				g_ItemMixTable = new _XItemMixTable[g_maxItemMixTableCount];
				bReadCount = TRUE;	
			}
		}
		else
		{
			sscanf( buffer, "%d %d %d %d %d", &g_ItemMixTable[count].ucIndex[0] , &g_ItemMixTable[count].ucIndex[1], 
				&g_ItemMixTable[count].ucIndex[2], &g_ItemMixTable[count].ucIndex[3], &g_ItemMixTable[count].ucIndex[4] );						
			count++;
		}
		if( count >= g_maxItemMixTableCount )
			break ;
	}
	
	g_ScriptArchive.ClosePackage();	
	return TRUE ;	
}

BOOL _XGameItem::LoadItemInchantTable(void)
{
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

	FILE* pScript = NULL;

	pScript = g_ScriptArchive.GetPackedFile( _T("inchantTable.txt") );

	if( !pScript )
	{
		_XFatalError("Load Inchant table [%s] file open", _T("inchantTable.txt"));
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	char buffer[256];
	BOOL bReadCount = FALSE;
	int  count = 0;

	// 제련데이타는 12개로 제한
	g_ItemInchantTable = new _XItemInchantTable[12];

	while(!feof(pScript))
	{
		memset( buffer, 0, 256 );
		if(!fgets(buffer, 256, pScript))
			break;

		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		if( !isalnum(buffer[0]) ) continue;

		unsigned int tempbuffer;
		
#ifdef _XDEF_EXPAND_INCHANT_TABLE_080425_KUKURI
		int scancount = sscanf( buffer, "%d %d %d %d %f %d %f %d %d %d %d %d %d %d", 
				&tempbuffer, &tempbuffer, &tempbuffer, &g_ItemInchantTable[count].iatteckplus, 
				&g_ItemInchantTable[count].fattecktimes, &g_ItemInchantTable[count].iforceplus, 
				&g_ItemInchantTable[count].fforcetimes, &g_ItemInchantTable[count].uiPrice, &g_ItemInchantTable[count].cType, 
				&g_ItemInchantTable[count].sID, &g_ItemInchantTable[count].sID_2, &g_ItemInchantTable[count].ucCount,
				&tempbuffer, &g_ItemInchantTable[count].effectID );
#else	
		int scancount = sscanf( buffer, "%d %d %d %d %f %d %f %d %d %d %d %d %d", 
				&tempbuffer, &tempbuffer, &tempbuffer, &g_ItemInchantTable[count].iatteckplus, 
				&g_ItemInchantTable[count].fattecktimes, &g_ItemInchantTable[count].iforceplus, 
				&g_ItemInchantTable[count].fforcetimes, &g_ItemInchantTable[count].uiPrice, &g_ItemInchantTable[count].cType, 
				&g_ItemInchantTable[count].sID, &g_ItemInchantTable[count].ucCount,
				&tempbuffer, &g_ItemInchantTable[count].effectID );
#endif
			
		count++;
		
		if( count >= 12 )
			break ;
	}

	if( count == 0 )
	{
		delete[] g_ItemInchantTable;
		g_ItemInchantTable = NULL;
	}
	
	g_ScriptArchive.ClosePackage();	
	return TRUE ;	
}

TCHAR* _XGameItem::GetItemNick(char cType, short sID)
{
#ifdef _XTS_ITEM_OPTIMIZATION
	char itemtype = GetEqualItemType(cType);
	if( itemtype != -1 )
	{
		switch(itemtype)
		{
		case _XGI_FC_WEAPON :
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);				
				if( WeaponItem )
					return WeaponItem[sID].cNick;
			}
			break;
		case _XGI_FC_CLOTHES :
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
				if( ClothesItem )
					return ClothesItem[sID].cNick;				
			}
			break;
		}
	}
#else
	switch(cType)
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].cNick;
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].cNick;
	case _XGI_FC_WEAPON2 :
		return g_WeaponItemProperty2[sID].cNick;		
	case _XGI_FC_CLOTHES2 :
		return g_ClothesItemProperty2[sID].cNick;
	case _XGI_FC_WEAPON3 :
		return g_WeaponItemProperty3[sID].cNick;
	case _XGI_FC_CLOTHES3 :
		return g_ClothesItemProperty3[sID].cNick;
	}
#endif
	return NULL;
}

TCHAR* _XGameItem::GetItemFullName(char cType, short sID)
{	
#ifdef _XTS_ITEM_V20
	switch(GetEqualItemType(cType))
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( WeaponItem )
			{
				if( WeaponItem[sID].cNick )
				{					
					if( WeaponItem[sID].usAddto )
						_snprintf( g_ItemnameString, sizeof(g_ItemnameString), _T("%s %s %s"), WeaponItem[sID].cNick, g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_ADDTO, WeaponItem[sID].usAddto ), WeaponItem[sID].cName );
					else
						_snprintf( g_ItemnameString, sizeof(g_ItemnameString), _T("%s %s"), WeaponItem[sID].cNick, WeaponItem[sID].cName );
				}
				else
				{
					if( WeaponItem[sID].usAddto )
						_snprintf( g_ItemnameString, sizeof(g_ItemnameString), _T("%s %s"), g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_ADDTO, WeaponItem[sID].usAddto ), WeaponItem[sID].cName );
					else
					{
						if(WeaponItem[sID].cName)
							strncpy( g_ItemnameString, WeaponItem[sID].cName, sizeof(g_ItemnameString) );
						else
							_snprintf(g_ItemnameString, sizeof(g_ItemnameString), _XGETINTERFACETEXT(ID_STRING_NETWORK_UNKNOWNITEM));
					}
				}
				return g_ItemnameString;
			}
		}
		break;
	case _XGI_FC_CLOTHES :
		{			
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( ClothesItem )
			{
				if( ClothesItem[sID].cNick )
				{	
					if(ClothesItem[sID].cName)
					{
						if( ClothesItem[sID].usAddto )
							_snprintf( g_ItemnameString, sizeof(g_ItemnameString), _T("%s %s %s"), ClothesItem[sID].cNick, g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_ADDTO, ClothesItem[sID].usAddto ), ClothesItem[sID].cName );
						else
							_snprintf( g_ItemnameString, sizeof(g_ItemnameString), _T("%s %s"), ClothesItem[sID].cNick, ClothesItem[sID].cName );
					}
					else
					{
						_snprintf(g_ItemnameString, sizeof(g_ItemnameString), _XGETINTERFACETEXT(ID_STRING_NETWORK_UNKNOWNITEM));
					}
				}
				else
				{
					if(ClothesItem[sID].cName)
					{
						if( ClothesItem[sID].usAddto )
							_snprintf( g_ItemnameString, sizeof(g_ItemnameString), _T("%s %s"), g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_ADDTO, ClothesItem[sID].usAddto ), ClothesItem[sID].cName );
						else
							_snprintf( g_ItemnameString, sizeof(g_ItemnameString), _T("%s"),ClothesItem[sID].cName );
					}
					else
					{
						_snprintf(g_ItemnameString, sizeof(g_ItemnameString), _XGETINTERFACETEXT(ID_STRING_NETWORK_UNKNOWNITEM));
					}
				}
				return g_ItemnameString;
			}
		}
		break;					
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
				return BookItem[sID].cName;
		}
		break;			
	case _XGI_FC_POTION :
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
				return PotionItem[sID].cName;
		}
		break;			
	case _XGI_FC_CONSUMING :
		{
			_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ConsumingItem )
			{
				return ConsumingItem[sID].cName;
//				if( ConsumingItem[sID].cNick )
//					sprintf( g_ItemnameString, "%s %s", ConsumingItem[sID].cNick, ConsumingItem[sID].cName );
//				else
//					strcpy( g_ItemnameString, ConsumingItem[sID].cName );
//				return g_ItemnameString;
			}
		}
		break;			
	case _XGI_FC_ELIXIR:
		{
			_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
				return ElixirItem[sID].cName;		
		}
		break;			
	case _XGI_FC_COUNTERACT :
		{
			_XGI_CounteractItem_Property* CounteractItem = (_XGI_CounteractItem_Property*)GetFirstTypeItemPorperty(cType);
			if( CounteractItem )
				return CounteractItem[sID].cName;		
		}
		break;			
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( ResourceItem )
				return ResourceItem[sID].cName;	
		}
		break;
	case _XGI_FC_QUEST :
		{
			_XGI_QuestItem_Property* QuestItem = (_XGI_QuestItem_Property*)GetFirstTypeItemPorperty(cType);
			if( QuestItem )
				return QuestItem[sID].cName;
		}
		break;			
	case _XGI_FC_ACCESSORY :
		{
			_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
				return AccessoryItem[sID].cName;	
		}
		break;			
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
				return LifeItem[sID].cName;
		}
		break;				
	case _XGI_FC_MUSIC :
		{
			_XGI_MusicItem_Property* MusicItem = (_XGI_MusicItem_Property*)GetFirstTypeItemPorperty(cType);
			if( MusicItem )
				return MusicItem[sID].cName;
		}			
		break;
	case _XGI_FC_PASS :
		{
			_XGI_PassItem_Property* PassItem = (_XGI_PassItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PassItem )
				return PassItem[sID].cName;
		}
		break;			
	case _XGI_FC_MONEY :
		{
			return _XGETINTERFACETEXT(ID_STRING_NETWORK_MONEY);
		}
		break;
	case _XGI_FC_SOCKET :
		{
			_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
			if( SocketItem )
				return SocketItem[sID].cName;
		}
		break;			
	case _XGI_FC_BOX :
		{
			_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxItem )
				return BoxItem[sID].cName;
		}
		break;			
	case _XGI_FC_BOXKEY :
		{
			_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxkeyItem )
				return BoxkeyItem[sID].cName;
		}
		break;
	}
#endif
	return _XGETINTERFACETEXT(ID_STRING_NETWORK_UNKNOWNITEM);
}

#ifdef _XTS_ITEM_V20
TCHAR* _XGameItem::GetItemName(char cType, short sID)
{
	if( sID <= 0 || sID > g_MaxItemCount[cType] )
		return _XGETINTERFACETEXT(ID_STRING_NETWORK_UNKNOWNITEM);
	
	switch(GetEqualItemType(cType))
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( WeaponItem )
			{
				if( WeaponItem[sID].usAddto > 0 && WeaponItem[sID].usAddto < 7 )
					_snprintf( g_ItemnameString, sizeof(g_ItemnameString), _T("%s %s"), g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_ADDTO, WeaponItem[sID].usAddto ), WeaponItem[sID].cName );
				else
					strcpy( g_ItemnameString, WeaponItem[sID].cName );
			}
			return g_ItemnameString;
		}
		break;
	case _XGI_FC_CLOTHES :
		{			
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( ClothesItem )
			{
				if( ClothesItem[sID].usAddto > 0 && ClothesItem[sID].usAddto < 7 )
					_snprintf( g_ItemnameString, sizeof(g_ItemnameString), _T("%s %s"), g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_ADDTO, ClothesItem[sID].usAddto ), ClothesItem[sID].cName );
				else
					strncpy( g_ItemnameString, ClothesItem[sID].cName, sizeof(g_ItemnameString) );
			}
			return g_ItemnameString;
		}
		break;					
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
				return BookItem[sID].cName;
		}
		break;			
	case _XGI_FC_POTION :
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
				return PotionItem[sID].cName;
		}
		break;			
	case _XGI_FC_CONSUMING :
		{
			_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ConsumingItem )
				return ConsumingItem[sID].cName;
		}
		break;			
	case _XGI_FC_ELIXIR:
		{
			_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
				return ElixirItem[sID].cName;		
		}
		break;			
	case _XGI_FC_COUNTERACT :
		{
			_XGI_CounteractItem_Property* CounteractItem = (_XGI_CounteractItem_Property*)GetFirstTypeItemPorperty(cType);
			if( CounteractItem )
				return CounteractItem[sID].cName;		
		}
		break;			
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( ResourceItem )
				return ResourceItem[sID].cName;	
		}
		break;
	case _XGI_FC_QUEST :
		{
			_XGI_QuestItem_Property* QuestItem = (_XGI_QuestItem_Property*)GetFirstTypeItemPorperty(cType);
			if( QuestItem )
				return QuestItem[sID].cName;
		}
		break;			
	case _XGI_FC_ACCESSORY :
		{
			_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
				return AccessoryItem[sID].cName;	
		}
		break;			
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
				return LifeItem[sID].cName;
		}
		break;				
	case _XGI_FC_MUSIC :
		{
			_XGI_MusicItem_Property* MusicItem = (_XGI_MusicItem_Property*)GetFirstTypeItemPorperty(cType);
			if( MusicItem )
				return MusicItem[sID].cName;
		}			
		break;
	case _XGI_FC_PASS :
		{
			_XGI_PassItem_Property* PassItem = (_XGI_PassItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PassItem )
				return PassItem[sID].cName;
		}
		break;			
	case _XGI_FC_MONEY :
		{
			return _XGETINTERFACETEXT(ID_STRING_NETWORK_MONEY);
		}
		break;
	case _XGI_FC_SOCKET :
		{
			_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
			if( SocketItem )
				return SocketItem[sID].cName;
		}
		break;			
	case _XGI_FC_BOX :
		{
			_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxItem )
				return BoxItem[sID].cName;
		}
		break;			
	case _XGI_FC_BOXKEY :
		{
			_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxkeyItem )
				return BoxkeyItem[sID].cName;
		}
		break;
	}
	return _XGETINTERFACETEXT(ID_STRING_NETWORK_UNKNOWNITEM);
}
#else
TCHAR* _XGameItem::GetItemName(char cType, short sID)
{
	if( sID <= 0 || sID > g_MaxItemCount[cType] )
		return _XGETINTERFACETEXT(ID_STRING_NETWORK_UNKNOWNITEM);
#ifdef _XTS_ITEM_OPTIMIZATION
	char itemtype = GetEqualItemType(cType);
	if( itemtype != -1 )
	{
		switch(itemtype)
		{
		case _XGI_FC_WEAPON :
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);				
				if( WeaponItem )
					return WeaponItem[sID].cName;
			}
			break;
		case _XGI_FC_CLOTHES :
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
				if( ClothesItem )
					return ClothesItem[sID].cName;				
			}
			break;			
		case _XGI_FC_BOOK :
			{
				_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
				if( BookItem )
					return BookItem[sID].cName;		
			}
			break;			
		case _XGI_FC_POTION :
			{
				_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
				if( PotionItem )
					return PotionItem[sID].cName;		
			}
			break;			
		case _XGI_FC_CONSUMING :
			{
				_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
				if( ConsumingItem )
					return ConsumingItem[sID].cName;
			}
			break;			
		case _XGI_FC_ELIXIR:
			{
				_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
				if( ElixirItem )
					return ElixirItem[sID].cName;		
			}
			break;			
		case _XGI_FC_COUNTERACT :
			{
				_XGI_CounteractItem_Property* CounteractItem = (_XGI_CounteractItem_Property*)GetFirstTypeItemPorperty(cType);
				if( CounteractItem )
					return CounteractItem[sID].cName;		
			}
			break;			
		case _XGI_FC_RESOURCE :
			{
				_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);				
				if( ResourceItem )
					return ResourceItem[sID].cName;		
			}
			break;
		case _XGI_FC_QUEST :
			{
				_XGI_QuestItem_Property* QuestItem = (_XGI_QuestItem_Property*)GetFirstTypeItemPorperty(cType);
				if( QuestItem )
					return QuestItem[sID].cName;		
			}
			break;			
		case _XGI_FC_ACCESSORY :
			{
				_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
				if( AccessoryItem )
					return AccessoryItem[sID].cName;		
			}
			break;			
		case _XGI_FC_LIFE :
			{
				_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
				if( LifeItem )
					return LifeItem[sID].cName;	
			}
			break;				
		case _XGI_FC_MUSIC :
			{
				_XGI_MusicItem_Property* MusicItem = (_XGI_MusicItem_Property*)GetFirstTypeItemPorperty(cType);
				if( MusicItem )
					return MusicItem[sID].cName;		
			}			
			break;
		case _XGI_FC_PASS :
			{
				_XGI_PassItem_Property* PassItem = (_XGI_PassItem_Property*)GetFirstTypeItemPorperty(cType);
				if( PassItem )
					return PassItem[sID].cName;		
			}
			break;			
		case _XGI_FC_MONEY :
			return _XGETINTERFACETEXT(ID_STRING_NETWORK_MONEY);
		case _XGI_FC_SOCKET :
			{
				_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
				if( SocketItem )
					return SocketItem[sID].cName;
			}
			break;			
		case _XGI_FC_BOX :
			{
				_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
				if( BoxItem )
					return BoxItem[sID].cName;
			}
			break;			
		case _XGI_FC_BOXKEY :
			{
				_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
				if( BoxkeyItem )
					return BoxkeyItem[sID].cName;
			}
			break;			
		}
	}
#else
	switch(cType)
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].cName;		
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].cName;
	case _XGI_FC_WEAPON2 :
		return g_WeaponItemProperty2[sID].cName;		
	case _XGI_FC_CLOTHES2 :
		return g_ClothesItemProperty2[sID].cName;
	case _XGI_FC_WEAPON3 :
		return g_WeaponItemProperty3[sID].cName;
	case _XGI_FC_CLOTHES3 :
		return g_ClothesItemProperty3[sID].cName;
	case _XGI_FC_BOOK :
		return g_BookItemProperty[sID].cName;		
	case _XGI_FC_POTION :
		return g_PotionItemProperty[sID].cName;		
	case _XGI_FC_CONSUMING :
		return g_ConsumingItemProperty[sID].cName;
	case _XGI_FC_ELIXIR:
		return g_ElixirItemProperty[sID].cName;		
	case _XGI_FC_COUNTERACT :
		return g_CounteractItemProperty[sID].cName;		
	case _XGI_FC_RESOURCE :
		return g_ResourceItemProperty[sID].cName;		
	case _XGI_FC_QUEST :
		return g_QuestItemProperty[sID].cName;		
	case _XGI_FC_ACCESSORY :
		return g_AccessoryItemProperty[sID].cName;		
	case _XGI_FC_LIFE :
		return g_LifeItemProperty[sID].cName;		
	case _XGI_FC_MUSIC :
		return g_MusicItemProperty[sID].cName;		
	case _XGI_FC_PASS :
		return g_PassItemProperty[sID].cName;		
	case _XGI_FC_MONEY :
		return _XGETINTERFACETEXT(ID_STRING_NETWORK_MONEY);
	case _XGI_FC_SOCKET :
		return g_SocketItemProperty[sID].cName;
	case _XGI_FC_BOX :
		return g_BoxItemProperty[sID].cName;
	case _XGI_FC_BOXKEY :
		return g_BoxKeyItemProperty[sID].cName;
	}
#endif
	return _XGETINTERFACETEXT(ID_STRING_NETWORK_UNKNOWNITEM);
}
#endif

char _XGameItem::GetThird(char cType, short sID)	// hotblood->2004.10.14
{
#ifdef _XTS_ITEM_OPTIMIZATION
/*	if( _XGI_FC_WEAPON == GetEqualItemType(cType) )
	{
		_XGI_WeaponItem_Property* tempWeaponItemPtr = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);
		if( tempWeaponItemPtr )
			return tempWeaponItemPtr[sID].cThirdType;
	}*/

	char itemtype = GetEqualItemType(cType);
	
	switch(itemtype)
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( WeaponItem )
				return WeaponItem[sID].cThirdType;
		}
		break;
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
				return BookItem[sID].cThirdType;		
		}
		break;			
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
				return LifeItem[sID].cThirdType;	
		}
		break;				
	}
#else
	switch(cType)
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].cThirdType;
	case _XGI_FC_WEAPON2 :
		return g_WeaponItemProperty2[sID].cThirdType;
	case _XGI_FC_WEAPON3 :
		return g_WeaponItemProperty3[sID].cThirdType;
	}
#endif
	return 0;
}

unsigned short _XGameItem::GetMaxDurability(char cType, short sID)	// hotblood->2004.10.14
{	
#ifdef _XTS_ITEM_OPTIMIZATION
	switch(GetEqualItemType(cType))
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = 	(_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);
			if( WeaponItem )
				return WeaponItem[sID].usDurability;
		}
		break;			
	case _XGI_FC_CLOTHES :
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
				return ClothesItem[sID].usDurability;
		}
		break;		
	case _XGI_FC_LIFE :	
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
				return LifeItem[sID].usDurability;
		}
		break;			
	}
#else
	switch(cType)
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].usDurability;		
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].usDurability;
	case _XGI_FC_WEAPON2 :
		return g_WeaponItemProperty2[sID].usDurability;		
	case _XGI_FC_CLOTHES2 :
		return g_ClothesItemProperty2[sID].usDurability;
	case _XGI_FC_WEAPON3 :
		return g_WeaponItemProperty3[sID].usDurability;	
	case _XGI_FC_CLOTHES3 :
		return g_ClothesItemProperty3[sID].usDurability;
	case _XGI_FC_LIFE :	
		return g_LifeItemProperty[sID].usDurability;
	}
#endif
	return 0;
}

unsigned char _XGameItem::GetStrength( char cType, short sID )
{
	switch(cType)
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].cStrength;		
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].cStrength;
	case _XGI_FC_WEAPON2 :
		return g_WeaponItemProperty2[sID].cStrength;
	case _XGI_FC_CLOTHES2 :
		return g_ClothesItemProperty2[sID].cStrength;
	case _XGI_FC_WEAPON3 :
		return g_WeaponItemProperty3[sID].cStrength;	
	case _XGI_FC_CLOTHES3 :
		return g_ClothesItemProperty3[sID].cStrength;
	case _XGI_FC_LIFE :	
		return g_LifeItemProperty[sID].cStrength;
	}

	return 0;
}
//Author : 양희왕
unsigned char _XGameItem::GetNature( char cType, short sID )
{
	switch(cType)
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].cNature;		
	case _XGI_FC_WEAPON2 :
		return g_WeaponItemProperty2[sID].cNature;
	case _XGI_FC_WEAPON3 :
		return g_WeaponItemProperty3[sID].cNature;	
	}

	return 0;
}

char _XGameItem::GetPocketNum(char cType, short sID)	// hotblood->2004.10.14
{	
#ifdef _XTS_ITEM_OPTIMIZATION
	if( _XGI_FC_CLOTHES == GetEqualItemType(cType) )
	{
		_XGI_ClothesItem_Property* tempClothesItemPtr = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
		if( tempClothesItemPtr )
			return tempClothesItemPtr[sID].cPocket;
	}	
#else	
	switch(cType)
	{
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].cPocket;
	case _XGI_FC_CLOTHES2 :
		return g_ClothesItemProperty2[sID].cPocket;
	case _XGI_FC_CLOTHES3 :
		return g_ClothesItemProperty3[sID].cPocket;
	}
#endif
	return 0;
}

unsigned int _XGameItem::GetItemPrice( char cType, short sID)	// hotblood->2005.01.31
{
#ifdef _XTS_ITEM_OPTIMIZATION
	switch(GetEqualItemType(cType)) 
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);
			if( WeaponItem )
				return WeaponItem[sID].uiPrice;
		}
		break;			
	case _XGI_FC_CLOTHES :
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
				return ClothesItem[sID].uiPrice;
		}
		break;		
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
				return BookItem[sID].uiPrice;
		}
		break;			
	case _XGI_FC_POTION :
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
				return PotionItem[sID].uiPrice;
		}
		break;			
	case _XGI_FC_CONSUMING :
		{
			_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ConsumingItem )
				return ConsumingItem[sID].uiPrice;
		}
		break;			
	case _XGI_FC_ELIXIR:
		{
			_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
				return ElixirItem[sID].uiPrice;
		}
		break;			
	case _XGI_FC_COUNTERACT :
		break;
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( ResourceItem )
				return ResourceItem[sID].uiPrice;
		}
		break;			
	case _XGI_FC_QUEST :
		break;
	case _XGI_FC_ACCESSORY :
		{
			_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
				return AccessoryItem[sID].uiPrice;
		}
		break;
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
				return LifeItem[sID].uiPrice;
		}
		break;			
	case _XGI_FC_SOCKET :
		{
			_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
			if( SocketItem )
				return SocketItem[sID].uiPrice;
		}
		break;			
	case _XGI_FC_BOX :
		{
			_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxItem )
				return BoxItem[sID].uiPrice;		
		}
		break;			
	case _XGI_FC_BOXKEY :
		{
			_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxkeyItem )
				return BoxkeyItem[sID].uiPrice;		
		}
		break;
	}
#else
	switch(cType) 
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].uiPrice;
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].uiPrice;
	case _XGI_FC_WEAPON2 :
		return g_WeaponItemProperty2[sID].uiPrice;
	case _XGI_FC_CLOTHES2 :
		return g_ClothesItemProperty2[sID].uiPrice;
	case _XGI_FC_WEAPON3 :
		return g_WeaponItemProperty3[sID].uiPrice;
	case _XGI_FC_CLOTHES3 :
		return g_ClothesItemProperty3[sID].uiPrice;
	case _XGI_FC_BOOK :
		return g_BookItemProperty[sID].uiPrice;
	case _XGI_FC_POTION :
		return g_PotionItemProperty[sID].uiPrice;
	case _XGI_FC_CONSUMING :
		return g_ConsumingItemProperty[sID].uiPrice;
	case _XGI_FC_ELIXIR:
		return g_ElixirItemProperty[sID].uiPrice;
	case _XGI_FC_COUNTERACT :
		break;
	case _XGI_FC_RESOURCE :
		return g_ResourceItemProperty[sID].uiPrice;
	case _XGI_FC_QUEST :
		break;
	case _XGI_FC_ACCESSORY :
		return g_AccessoryItemProperty[sID].uiPrice;
	case _XGI_FC_LIFE :
		return g_LifeItemProperty[sID].uiPrice;
	case _XGI_FC_SOCKET :
		return g_SocketItemProperty[sID].uiPrice;
	case _XGI_FC_BOX :
		return g_BoxItemProperty[sID].uiPrice;		
	case _XGI_FC_BOXKEY :
		return g_BoxKeyItemProperty[sID].uiPrice;		
	}
#endif
	
	return 0;
}

char _XGameItem::GetItemSecond( char cType, short sID)	// hotblood->2005.01.31
{
#ifdef _XTS_ITEM_OPTIMIZATION	
	switch(GetEqualItemType(cType)) 
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( WeaponItem )
				return WeaponItem[sID].cSecondType;
		}
		break;			
	case _XGI_FC_CLOTHES :
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
				return ClothesItem[sID].cSecondType;
		}
		break;		
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
				return BookItem[sID].cSecondType;
		}
		break;			
	case _XGI_FC_POTION :
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
				return PotionItem[sID].cSecondType;
		}
		break;			
	case _XGI_FC_CONSUMING :
		{
			_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ConsumingItem )
				return ConsumingItem[sID].cSecondType;
		}
		break;			
	case _XGI_FC_ELIXIR:
		{
			_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
				return ElixirItem[sID].cSecondType;
		}
		break;			
	case _XGI_FC_COUNTERACT :
		break;
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( ResourceItem )
				return ResourceItem[sID].cSecondType;
		}
		break;			
	case _XGI_FC_QUEST :
		break;
	case _XGI_FC_ACCESSORY :
		{
			_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
				return AccessoryItem[sID].cSecondType;
		}
		break;
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
				return LifeItem[sID].cSecondType;
		}
		break;			
	case _XGI_FC_SOCKET :
		{
			_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
			if( SocketItem )
				return SocketItem[sID].cSecondType;
		}
		break;			
	case _XGI_FC_BOX :
		{
			_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxItem )
				return BoxItem[sID].cSecondType;		
		}
		break;			
	case _XGI_FC_BOXKEY :
		{
			_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxkeyItem )
				return BoxkeyItem[sID].cSecondType;		
		}
		break;
	}
#else
	switch(cType) 
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].cSecondType;
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].cSecondType;
	case _XGI_FC_WEAPON2 :
		return g_WeaponItemProperty2[sID].cSecondType;
	case _XGI_FC_CLOTHES2 :
		return g_ClothesItemProperty2[sID].cSecondType;
	case _XGI_FC_WEAPON3 :
		return g_WeaponItemProperty3[sID].cSecondType;
	case _XGI_FC_CLOTHES3 :
		return g_ClothesItemProperty3[sID].cSecondType;
	case _XGI_FC_BOOK :
		return g_BookItemProperty[sID].cSecondType;
	case _XGI_FC_POTION :
		return g_PotionItemProperty[sID].cSecondType;
	case _XGI_FC_CONSUMING :
		return g_ConsumingItemProperty[sID].cSecondType;
	case _XGI_FC_ELIXIR:
		return g_ElixirItemProperty[sID].cSecondType;
	case _XGI_FC_COUNTERACT :
		break;
	case _XGI_FC_RESOURCE :
		return g_ResourceItemProperty[sID].cSecondType;
	case _XGI_FC_QUEST :
		break;
	case _XGI_FC_ACCESSORY :
		return g_AccessoryItemProperty[sID].cSecondType;
	case _XGI_FC_LIFE :
		return g_LifeItemProperty[sID].cSecondType;
	case _XGI_FC_SOCKET :
		return g_SocketItemProperty[sID].cSecondType;
	case _XGI_FC_BOX :
		return g_BoxItemProperty[sID].cSecondType;
	case _XGI_FC_BOXKEY :
		return g_BoxKeyItemProperty[sID].cSecondType;
	}
#endif
	return 0;
}

unsigned char _XGameItem::GetItemGrade( char cType, short sID)
{	
#ifdef _XTS_ITEM_OPTIMIZATION	
	switch(GetEqualItemType(cType)) 
	{
	case _XGI_FC_WEAPON :	
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( WeaponItem )
				return WeaponItem[sID].ucGrade;
		}
		break;			
	case _XGI_FC_CLOTHES :
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
				return ClothesItem[sID].ucGrade;
		}
		break;		
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
				return BookItem[sID].ucGrade;
		}
		break;			
	case _XGI_FC_POTION :
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
				return PotionItem[sID].ucGrade;
		}
		break;			
	case _XGI_FC_CONSUMING :
		{
			_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ConsumingItem )
				return ConsumingItem[sID].ucGrade;
		}
		break;			
	case _XGI_FC_ELIXIR:
		{
			_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
				return ElixirItem[sID].ucGrade;
		}
		break;			
	case _XGI_FC_COUNTERACT :
		break;
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( ResourceItem )
				return ResourceItem[sID].ucGrade;
		}
		break;			
	case _XGI_FC_QUEST :
		{
			_XGI_QuestItem_Property* QuestItem = (_XGI_QuestItem_Property*)GetFirstTypeItemPorperty(cType);
			if( QuestItem )
				return QuestItem[sID].ucGrade;
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
				return AccessoryItem[sID].ucGrade;
		}
		break;
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
				return LifeItem[sID].ucGrade;
		}
		break;			
	case _XGI_FC_SOCKET :
		{
			_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
			if( SocketItem )
				return SocketItem[sID].ucGrade;
		}
		break;			
	case _XGI_FC_BOX :
		{
			_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxItem )
				return BoxItem[sID].ucGrade;		
		}
		break;			
	case _XGI_FC_BOXKEY :
		{
			_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxkeyItem )
				return BoxkeyItem[sID].ucGrade;		
		}
		break;
	}
#else
	switch(cType) 
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].ucGrade;
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].ucGrade;
	case _XGI_FC_WEAPON2 :
		return g_WeaponItemProperty2[sID].ucGrade;
	case _XGI_FC_CLOTHES2 :
		return g_ClothesItemProperty2[sID].ucGrade;
	case _XGI_FC_WEAPON3 :
		return g_WeaponItemProperty3[sID].ucGrade;
	case _XGI_FC_CLOTHES3 :
		return g_ClothesItemProperty3[sID].ucGrade;	
	case _XGI_FC_BOOK :
		return g_BookItemProperty[sID].ucGrade;
	case _XGI_FC_POTION :
		return g_PotionItemProperty[sID].ucGrade;
	case _XGI_FC_CONSUMING :
		return g_ConsumingItemProperty[sID].ucGrade;
	case _XGI_FC_ELIXIR:
		return g_ElixirItemProperty[sID].ucGrade;
	case _XGI_FC_COUNTERACT :
		return g_CounteractItemProperty[sID].ucGrade;
	case _XGI_FC_RESOURCE :
		return g_ResourceItemProperty[sID].ucGrade;
	case _XGI_FC_QUEST :
		return g_QuestItemProperty[sID].ucGrade;
	case _XGI_FC_ACCESSORY :
		return g_AccessoryItemProperty[sID].ucGrade;
	case _XGI_FC_LIFE :
		return g_LifeItemProperty[sID].ucGrade;
	case _XGI_FC_SOCKET :
		return g_SocketItemProperty[sID].ucGrade;
	case _XGI_FC_BOX :
		return g_BoxItemProperty[sID].ucGrade;
	case _XGI_FC_BOXKEY :
		return g_BoxKeyItemProperty[sID].ucGrade;
	}	
#endif
	return 0;
}

D3DCOLOR _XGameItem::GetGradeColor( char cType, short sID )
{
#ifdef _XTS_ITEM_OPTIMIZATION
	switch(GetEqualItemType(cType)) 
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( WeaponItem )
			{
				#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
				if( _XGameItem::GetSetItem(cType, sID) )
				{
					return D3DCOLOR_ARGB(255,255,76,255);
				}
				else
				#endif
				{
					if( WeaponItem[sID].ucGrade > 0 && WeaponItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
						return g_ItemGradeColorTable[WeaponItem[sID].ucGrade];
					else if( WeaponItem[sID].ucGrade == 50 || WeaponItem[sID].ucGrade == 51 ) // 새로운 기보 강무기는 인터페이스를 그레이드 1로 처리한다. 
						return g_ItemGradeColorTable[1];
					else
						return _XSC_INFORMATION;
				}
			}
		}
		break;			
	case _XGI_FC_CLOTHES :
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
			{
				#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
				if( _XGameItem::GetSetItem(cType, sID) )
				{
					return D3DCOLOR_ARGB(255,255,76,255);
				}
				else
				#endif
				{
					if( ClothesItem[sID].ucGrade > 0 && ClothesItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
						return g_ItemGradeColorTable[ClothesItem[sID].ucGrade];
					else 
						return _XSC_INFORMATION;
				}
			}				
		}
		break;		
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
			{
				if( BookItem[sID].ucGrade > 0 && BookItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
					return g_ItemGradeColorTable[BookItem[sID].ucGrade];
				else 
					return _XSC_INFORMATION;
			}
		}
		break;			
	case _XGI_FC_POTION :
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
			{
				if( PotionItem[sID].ucGrade > 0 && PotionItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
					return g_ItemGradeColorTable[PotionItem[sID].ucGrade];
				else 
					return _XSC_INFORMATION;
			}
		}
		break;			
	case _XGI_FC_CONSUMING :
		{
			_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ConsumingItem )
			{
				if( ConsumingItem[sID].ucGrade > 0 && ConsumingItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
					return g_ItemGradeColorTable[ConsumingItem[sID].ucGrade];
				else 
					return _XSC_INFORMATION;
			}
		}
		break;			
	case _XGI_FC_ELIXIR:
		{
			_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
			{
				if( ElixirItem[sID].ucGrade > 0 && ElixirItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
					return g_ItemGradeColorTable[ElixirItem[sID].ucGrade];
				else 
					return _XSC_INFORMATION;
			}
		}
		break;			
	case _XGI_FC_COUNTERACT :
		break;
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( ResourceItem )
			{
				if( ResourceItem[sID].ucGrade > 0 && ResourceItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
					return g_ItemGradeColorTable[ResourceItem[sID].ucGrade];
				else 
					return _XSC_INFORMATION;
			}
		}
		break;			
	case _XGI_FC_QUEST :
		{
			_XGI_QuestItem_Property* QuesstItem = (_XGI_QuestItem_Property*)GetFirstTypeItemPorperty(cType);
			if( QuesstItem )
			{
				if( QuesstItem[sID].ucGrade > 0 && QuesstItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
					return g_ItemGradeColorTable[QuesstItem[sID].ucGrade];
				else 
					return _XSC_INFORMATION;
			}				
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
			if( _XGameItem::GetSetItem(cType, sID) )
			{
				return D3DCOLOR_ARGB(255,255,76,255);
			}
			else
			#endif
			{
				_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
				if( AccessoryItem )
				{
					if( AccessoryItem[sID].ucGrade > 0 && AccessoryItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
						return g_ItemGradeColorTable[AccessoryItem[sID].ucGrade];
					else 
						return _XSC_INFORMATION;
				}
			}
		}
		break;
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
			{
				if( LifeItem[sID].ucGrade > 0 && LifeItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
					return g_ItemGradeColorTable[LifeItem[sID].ucGrade];
				else 
					return _XSC_INFORMATION;
			}
		}
		break;			
	case _XGI_FC_SOCKET :
		{
			_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
			if( SocketItem )
			{
				if( SocketItem[sID].ucGrade > 0 && SocketItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
					return g_ItemGradeColorTable[SocketItem[sID].ucGrade];
				else 
					return _XSC_INFORMATION;
			}
		}
		break;			
	case _XGI_FC_BOX :
		{
			_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxItem )
			{
				if( BoxItem[sID].ucGrade > 0 && BoxItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
					return g_ItemGradeColorTable[BoxItem[sID].ucGrade];
				else 
					return _XSC_INFORMATION;
			}	
		}
		break;			
	case _XGI_FC_BOXKEY :
		{
			_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxkeyItem )
			{
				if( BoxkeyItem[sID].ucGrade > 0 && BoxkeyItem[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
					return g_ItemGradeColorTable[BoxkeyItem[sID].ucGrade];
				else 
					return _XSC_INFORMATION;
			}
		}
		break;
	}	
#else
	switch(cType) 
	{
	case _XGI_FC_WEAPON :
		{
			if( g_WeaponItemProperty[sID].ucGrade > 0 && g_WeaponItemProperty[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_WeaponItemProperty[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case _XGI_FC_CLOTHES :
		{
			if( g_ClothesItemProperty[sID].ucGrade > 0 && g_ClothesItemProperty[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_ClothesItemProperty[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;		
	case _XGI_FC_WEAPON2 :
		{
			if( g_WeaponItemProperty2[sID].ucGrade > 0 && g_WeaponItemProperty2[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_WeaponItemProperty2[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case _XGI_FC_CLOTHES2 :
		{
			if( g_ClothesItemProperty2[sID].ucGrade > 0 &&g_ClothesItemProperty2[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_ClothesItemProperty2[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case _XGI_FC_WEAPON3 :
		{
			if( g_WeaponItemProperty3[sID].ucGrade > 0 && g_WeaponItemProperty3[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_WeaponItemProperty3[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case _XGI_FC_CLOTHES3 :
		{
			if( g_ClothesItemProperty3[sID].ucGrade > 0 &&g_ClothesItemProperty3[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_ClothesItemProperty3[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case _XGI_FC_BOOK :
		{
			if( g_BookItemProperty[sID].ucGrade > 0 && g_BookItemProperty[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_BookItemProperty[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case _XGI_FC_POTION :
		{
			if( g_PotionItemProperty[sID].ucGrade > 0 && g_PotionItemProperty[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_PotionItemProperty[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			if( g_ConsumingItemProperty[sID].ucGrade > 0 && g_ConsumingItemProperty[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_ConsumingItemProperty[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			if( g_ResourceItemProperty[sID].ucGrade > 0 && g_ResourceItemProperty[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_ResourceItemProperty[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case  _XGI_FC_QUEST :
		{
			if( g_QuestItemProperty[sID].ucGrade > 0 && g_QuestItemProperty[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_QuestItemProperty[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			if( g_AccessoryItemProperty[sID].ucGrade > 0 && g_AccessoryItemProperty[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_AccessoryItemProperty[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case _XGI_FC_LIFE :
		{
			if( g_LifeItemProperty[sID].ucGrade > 0 && g_LifeItemProperty[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_LifeItemProperty[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case _XGI_FC_SOCKET :
		{
			if( g_SocketItemProperty[sID].ucGrade > 0 && g_SocketItemProperty[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_SocketItemProperty[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	case _XGI_FC_BOX :
		{
			if( g_BoxItemProperty[sID].ucGrade > 0 && g_BoxItemProperty[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_BoxItemProperty[sID].ucGrade];
			else 
				return _XSC_INFORMATION;				
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			if( g_BoxKeyItemProperty[sID].ucGrade > 0 && g_BoxKeyItemProperty[sID].ucGrade < _XDEF_MAXITEMGRADECOLORCOUNT )
				return g_ItemGradeColorTable[g_BoxKeyItemProperty[sID].ucGrade];
			else 
				return _XSC_INFORMATION;
		}
		break;
	}
#endif
	return _XSC_INFORMATION;
}

unsigned int _XGameItem::GetAddEffectFromSocket(unsigned int index1, unsigned int index2, unsigned int index3, unsigned int index4)
{
	for( int i1 = 0; i1 < g_maxItemMixTableCount; i1++ )
	{
		if( index1 == g_ItemMixTable[i1].ucIndex[0] )
		{
			for( int i2 = i1; i2 < g_maxItemMixTableCount; i2++ )
			{
				if( index2 == g_ItemMixTable[i2].ucIndex[1] )
				{
					for( int i3 = i2; i3 < g_maxItemMixTableCount; i3++ )
					{
						if( index3 == g_ItemMixTable[i3].ucIndex[2] )
						{
							for( int i4 = i3; i4 < g_maxItemMixTableCount; i4++ )
							{
								if( index4 == g_ItemMixTable[i4].ucIndex[3] )
								{
									return g_ItemMixTable[i4].ucIndex[4];
								}
								else
								{
									return g_ItemMixTable[i3].ucIndex[4];
								}
							}
						}
						else
						{
							return g_ItemMixTable[i2].ucIndex[4];
						}
					}
				}
			}
		}
	}
	return 0;
}

unsigned int _XGameItem::GetInchantEffectId(char cType, short sID, char strength) 
{
	int second = 0;
	int third = 0;

#ifdef _XTS_ITEM_OPTIMIZATION
	if( _XGI_FC_WEAPON == GetEqualItemType(cType) )
	{
		_XGI_WeaponItem_Property* tempWeaponItemPtr = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);
		if( tempWeaponItemPtr )
		{
			second = tempWeaponItemPtr[sID].cSecondType;
			third = tempWeaponItemPtr[sID].cThirdType;
		}
	}	
#else
	if( cType == _XGI_FC_WEAPON )
	{
		second = g_WeaponItemProperty[sID].cSecondType;
		third = g_WeaponItemProperty[sID].cThirdType;

	}
	else if( cType == _XGI_FC_WEAPON2 )
	{
		second = g_WeaponItemProperty2[sID].cSecondType;
		third = g_WeaponItemProperty2[sID].cThirdType;
	}
	else if( cType == _XGI_FC_WEAPON3 )
	{
		second = g_WeaponItemProperty3[sID].cSecondType;
		third = g_WeaponItemProperty3[sID].cThirdType;
	}
#endif

	switch(second) 
	{
	case _XGI_SC_WEAPON_SHORT :
		{
			switch(third) 
			{
			case _XGI_TC_WEAPON_SWORD :		// 검
			case _XGI_TC_WEAPON_CLUB :		// 봉
			case _XGI_TC_WEAPON_AXE :		// 부	
			case _XGI_TC_WEAPON_SECRET :	// 단검			
				{
					switch(strength) 
					{
					case 3 :
						return 400;						
					case 4 :
						return 401;
					case 5 :
						return 402;
					case 6 :
						return 403;
					case 7 :
						return 404;
					case 8 :
						return 405;
					case 9 :
						return 406;
					case 10 :
						return 436;
					case 11 :
						return 437;
					case 12 :
						return 438;
					}
				}
				break;
			case _XGI_TC_WEAPON_KNIFE :	// 도
				{
					switch(strength) 
					{
					case 3 :
						return 422;
					case 4 :
						return 423;
					case 5 :
						return 424;
					case 6 :
						return 425;
					case 7 :
						return 426;
					case 8 :
						return 427;
					case 9 :
						return 428;
					case 10 :
						return 445;
					case 11 :
						return 446;
					case 12 :
						return 447;
					}
				}
				break;
			case _XGI_TC_WEAPON_FIST :	// 권각
				{
					switch(strength) 
					{
					case 3 :
						return 407;
					case 4 :
						return 408;
					case 5 :
						return 409;
					case 6 :
						return 410;
					case 7 :
						return 411;
					case 8 :
						return 412;
					case 9 :
						return 413;
					case 10 :
						return 439;
					case 11 :
						return 440;
					case 12 :
						return 441;
					}
				}
				break;
			case _XGI_TC_WEAPON_RING :	// 쌍환
				{
					switch(strength) 
					{
					case 3 :
						return 414;
					case 4 :
						return 415;
					case 5 :
						return 416;
					case 6 :
						return 417;
					case 7 :
						return 418;
					case 8 :
						return 419;
					case 9 :
						return 420;
					case 10 :
						return 442;
					case 11 :
						return 443;
					case 12 :
						return 444;
					}
				}
				break;
			}
		}
		break;
	case _XGI_SC_WEAPON_LONG : // 장병기
		{
			switch(third) 
			{
			case _XGI_TC_WEAPON_GLAIVE :	//대도		
			case _XGI_TC_WEAPON_SPEAR :     //창술				
			case _XGI_TC_WEAPON_STAFF :		//곤술
				{
					switch(strength) 
					{
					case 3 :
						return 429;						
					case 4 :
						return 430;
					case 5 :
						return 431;
					case 6 :
						return 432;
					case 7 :
						return 433;
					case 8 :
						return 434;
					case 9 :
						return 435;
					case 10 :
						return 448;
					case 11 :
						return 449;
					case 12 :
						return 450;
					}
				}
				break;
			}
		}
		break;
	case _XGI_SC_WEAPON_SPECIAL :// 특수무기
		{
			switch(third) 
			{
			case _XGI_TC_WEAPON_SERING : // 강, 륜 공통 사용
				{
					switch(strength) 
					{
					case 0 :
					case 1 :
					case 2 :
						return 421;
					case 3 :
						return 407;
					case 4 :
						return 408;
					case 5 :
						return 409;
					case 6 :
						return 410;
					case 7 :
						return 411;
					case 8 :
						return 412;
					case 9 :
						return 413;
					case 10 :
						return 439;
					case 11 :
						return 440;
					case 12 :
						return 441;
					}
				}
				break;
			}
		}
		break;
	}
	return 0;
}

TCHAR* _XGameItem::GetItemNameAddtoString(char cType, short sID)
{
#ifdef _XTS_ITEM_V20
	unsigned short addto = 0;
	switch(GetEqualItemType(cType))
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = 	(_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);
			if( WeaponItem )
				addto = WeaponItem[sID].usAddto;
		}
		break;			
	case _XGI_FC_CLOTHES :
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
				addto = ClothesItem[sID].usAddto;
		}
		break;
	}
	if( addto > 0 && addto < 7 )	// 0 - Null 7 - Origin
		return g_StringDataBase.GetTextString( _XSTRINGDB_SN_ITEMTABLE_ADDTO, addto );

#endif
	return NULL;
}

BOOL _XGameItem::CheckDump(char cType, short sID)
{	
#ifdef _XTS_ITEM_OPTIMIZATION	
	switch(GetEqualItemType(cType)) 
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( WeaponItem )
				return WeaponItem[sID].cDump;
		}
		break;			
	case _XGI_FC_CLOTHES :
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
				return ClothesItem[sID].cDump;
		}
		break;		
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
				return BookItem[sID].cDump;
		}
		break;			
	case _XGI_FC_POTION :
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
				return PotionItem[sID].cDump;
		}
		break;			
	case _XGI_FC_CONSUMING :
		{
			_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ConsumingItem )
				return ConsumingItem[sID].cDump;
		}
		break;			
	case _XGI_FC_ELIXIR:
		{
			_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
				return ElixirItem[sID].cDump;
		}
		break;			
	case _XGI_FC_COUNTERACT :
		break;
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( ResourceItem )
				return ResourceItem[sID].cDump;
		}
		break;			
	case _XGI_FC_QUEST :
		{
			_XGI_QuestItem_Property* QuestItem = (_XGI_QuestItem_Property*)GetFirstTypeItemPorperty(cType);
			if( QuestItem )
				return QuestItem[sID].cDump;
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
				return AccessoryItem[sID].cDump;
		}
		break;
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
				return LifeItem[sID].cDump;
		}
		break;			
	case _XGI_FC_SOCKET :
		{
			_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
			if( SocketItem )
				return SocketItem[sID].cDump;
		}
		break;			
	case _XGI_FC_BOX :
		{
			_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxItem )
				return BoxItem[sID].cDump;		
		}
		break;			
	case _XGI_FC_BOXKEY :
		{
			_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxkeyItem )
				return BoxkeyItem[sID].cDump;		
		}
		break;
	}	
#else

	switch(cType)	// 0 : 버릴 수 있음  1: 버릴수 없음
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].cDump;
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].cDump;
	case _XGI_FC_WEAPON2 :
		return g_WeaponItemProperty2[sID].cDump;
	case _XGI_FC_CLOTHES2 :
		return g_ClothesItemProperty2[sID].cDump;
	case _XGI_FC_WEAPON3 :
		return g_WeaponItemProperty3[sID].cDump;
	case _XGI_FC_CLOTHES3 :
		return g_ClothesItemProperty3[sID].cDump;
	case _XGI_FC_BOOK	:
		return g_BookItemProperty[sID].cDump;
	case _XGI_FC_POTION :
		return g_PotionItemProperty[sID].cDump;
	case _XGI_FC_CONSUMING :
		return g_ConsumingItemProperty[sID].cDump;
	case _XGI_FC_ELIXIR :
		return g_ElixirItemProperty[sID].cDump;
	case _XGI_FC_RESOURCE :
		return g_ResourceItemProperty[sID].cDump;
	case _XGI_FC_QUEST :
		return g_QuestItemProperty[sID].cDump;
	case _XGI_FC_ACCESSORY :
		return g_AccessoryItemProperty[sID].cDump;
	case _XGI_FC_LIFE :
		return g_LifeItemProperty[sID].cDump;
	case _XGI_FC_SOCKET :
		return g_SocketItemProperty[sID].cDump;
	case _XGI_FC_BOX :
		return g_BoxItemProperty[sID].cDump;
	case _XGI_FC_BOXKEY :
		return g_BoxKeyItemProperty[sID].cDump;
	}

#endif
	return FALSE;
}

BOOL _XGameItem::CheckUserTrade( char cType, short sID )
{
#ifdef _XTS_ITEM_OPTIMIZATION	
	switch(GetEqualItemType(cType)) 
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( WeaponItem )
			{
#ifdef _XDEF_PERIODITEM_20061027
				if(WeaponItem[sID].cCashCheck == 2 || WeaponItem[sID].cCashCheck == 3)
					return TRUE;
				else
#endif
				return WeaponItem[sID].cUserTrade;
			}
		}
		break;			
	case _XGI_FC_CLOTHES :
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
			{
#ifdef _XDEF_PERIODITEM_20061027
				if(ClothesItem[sID].cCashCheck == 2 || ClothesItem[sID].cCashCheck == 3)
					return TRUE;
				else
#endif
				return ClothesItem[sID].cUserTrade;
			}
		}
		break;		
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
				return BookItem[sID].cUserTrade;
		}
		break;			
	case _XGI_FC_POTION :
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
				return PotionItem[sID].cUserTrade;
		}
		break;			
	case _XGI_FC_CONSUMING :
		{
			_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ConsumingItem )
				return ConsumingItem[sID].cUserTrade;
		}
		break;			
	case _XGI_FC_ELIXIR:
		{
			_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
				return ElixirItem[sID].cUserTrade;
		}
		break;			
	case _XGI_FC_COUNTERACT :
		break;
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( ResourceItem )
				return ResourceItem[sID].cUserTrade;
		}
		break;			
	case _XGI_FC_QUEST :
		{
			_XGI_QuestItem_Property* QuestItem = (_XGI_QuestItem_Property*)GetFirstTypeItemPorperty(cType);
			if( QuestItem )
				return QuestItem[sID].cUserTrade;
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
			{
#ifdef _XDEF_PERIODITEM_20061027
				if(AccessoryItem[sID].cCashCheck == 2 || AccessoryItem[sID].cCashCheck == 3)
					return TRUE;
				else
#endif
				return AccessoryItem[sID].cUserTrade;
			}
		}
		break;
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
				return LifeItem[sID].cUserTrade;
		}
		break;			
	case _XGI_FC_SOCKET :
		{
			_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
			if( SocketItem )
				return SocketItem[sID].cUserTrade;
		}
		break;			
	case _XGI_FC_BOX :
		{
			_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxItem )
				return BoxItem[sID].cUserTrade;		
		}
		break;			
	case _XGI_FC_BOXKEY :
		{
			_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxkeyItem )
				return BoxkeyItem[sID].cUserTrade;		
		}
		break;
	}
#else
	switch(cType)	// 0 : 버릴 수 있음  1: 버릴수 없음
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].cUserTrade;
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].cUserTrade;
	case _XGI_FC_WEAPON2 :
		return g_WeaponItemProperty2[sID].cUserTrade;
	case _XGI_FC_CLOTHES2 :
		return g_ClothesItemProperty2[sID].cUserTrade;
	case _XGI_FC_WEAPON3 :
		return g_WeaponItemProperty3[sID].cUserTrade;
	case _XGI_FC_CLOTHES3 :
		return g_ClothesItemProperty3[sID].cUserTrade;
	case _XGI_FC_BOOK	:
		return g_BookItemProperty[sID].cUserTrade;
	case _XGI_FC_POTION :
		return g_PotionItemProperty[sID].cUserTrade;
	case _XGI_FC_CONSUMING :
		return g_ConsumingItemProperty[sID].cUserTrade;
	case _XGI_FC_ELIXIR :
		return g_ElixirItemProperty[sID].cUserTrade;
	case _XGI_FC_RESOURCE :
		return g_ResourceItemProperty[sID].cUserTrade;
	case _XGI_FC_QUEST :
		return g_QuestItemProperty[sID].cUserTrade;
	case _XGI_FC_ACCESSORY :
		return g_AccessoryItemProperty[sID].cUserTrade;
	case _XGI_FC_LIFE :
		return g_LifeItemProperty[sID].cUserTrade;
	case _XGI_FC_SOCKET :
		return g_SocketItemProperty[sID].cUserTrade;
	case _XGI_FC_BOX :
		return g_BoxItemProperty[sID].cUserTrade;
	case _XGI_FC_BOXKEY :
		return g_BoxKeyItemProperty[sID].cUserTrade;
	}

#endif
	return FALSE;
}

BOOL _XGameItem::CheckNPCTrade( char cType, short sID )
{
#ifdef _XTS_ITEM_OPTIMIZATION	
	switch(GetEqualItemType(cType)) 
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( WeaponItem )
				return WeaponItem[sID].cNPCTrade;
		}
		break;			
	case _XGI_FC_CLOTHES :
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
				return ClothesItem[sID].cNPCTrade;
		}
		break;		
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
				return BookItem[sID].cNPCTrade;
		}
		break;			
	case _XGI_FC_POTION :
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
				return PotionItem[sID].cNPCTrade;
		}
		break;			
	case _XGI_FC_CONSUMING :
		{
			_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ConsumingItem )
				return ConsumingItem[sID].cNPCTrade;
		}
		break;			
	case _XGI_FC_ELIXIR:
		{
			_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
				return ElixirItem[sID].cNPCTrade;
		}
		break;			
	case _XGI_FC_COUNTERACT :
		break;
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( ResourceItem )
				return ResourceItem[sID].cNPCTrade;
		}
		break;			
	case _XGI_FC_QUEST :
		{
			_XGI_QuestItem_Property* QuestItem = (_XGI_QuestItem_Property*)GetFirstTypeItemPorperty(cType);
			if( QuestItem )
				return QuestItem[sID].cNPCTrade;
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
				return AccessoryItem[sID].cNPCTrade;
		}
		break;
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
				return LifeItem[sID].cNPCTrade;
		}
		break;			
	case _XGI_FC_SOCKET :
		{
			_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
			if( SocketItem )
				return SocketItem[sID].cNPCTrade;
		}
		break;			
	case _XGI_FC_BOX :
		{
			_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxItem )
				return BoxItem[sID].cNPCTrade;		
		}
		break;			
	case _XGI_FC_BOXKEY :
		{
			_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxkeyItem )
				return BoxkeyItem[sID].cNPCTrade;		
		}
		break;
	}
#else
	switch(cType)	// 0 : 버릴 수 있음  1: 버릴수 없음
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].cNPCTrade;
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].cNPCTrade;
	case _XGI_FC_WEAPON2 :
		return g_WeaponItemProperty2[sID].cNPCTrade;
	case _XGI_FC_CLOTHES2 :
		return g_ClothesItemProperty2[sID].cNPCTrade;
	case _XGI_FC_WEAPON3 :
		return g_WeaponItemProperty3[sID].cNPCTrade;
	case _XGI_FC_CLOTHES3 :
		return g_ClothesItemProperty3[sID].cNPCTrade;
	case _XGI_FC_BOOK	:
		return g_BookItemProperty[sID].cNPCTrade;
	case _XGI_FC_POTION :
		return g_PotionItemProperty[sID].cNPCTrade;
	case _XGI_FC_CONSUMING :
		return g_ConsumingItemProperty[sID].cNPCTrade;
	case _XGI_FC_ELIXIR :
		return g_ElixirItemProperty[sID].cNPCTrade;
	case _XGI_FC_RESOURCE :
		return g_ResourceItemProperty[sID].cNPCTrade;
	case _XGI_FC_QUEST :
		return g_QuestItemProperty[sID].cNPCTrade;
	case _XGI_FC_ACCESSORY :
		return g_AccessoryItemProperty[sID].cNPCTrade;
	case _XGI_FC_LIFE :
		return g_LifeItemProperty[sID].cNPCTrade;
	case _XGI_FC_SOCKET :
		return g_SocketItemProperty[sID].cNPCTrade;
	case _XGI_FC_BOX :
		return g_BoxItemProperty[sID].cNPCTrade;
	case _XGI_FC_BOXKEY :
		return g_BoxKeyItemProperty[sID].cNPCTrade;
	}

#endif
	return FALSE;	
}

unsigned int _XGameItem::GetInchantPlusPoint( char cSecond, int maxvalue, int grade )
{
	if( grade == 0 )
		return 0;

	int result = 0;
	if( grade-1 >= 0 )
		grade--;

	if( cSecond == _XGI_SC_WEAPON_SPECIAL )	// 강무기
	{
		result = g_ItemInchantTable[grade].iforceplus + (int)( (float)maxvalue * g_ItemInchantTable[grade].fforcetimes );
	}
	else	// 일반 무기
	{
		result = g_ItemInchantTable[grade].iatteckplus + (int)( (float)maxvalue * g_ItemInchantTable[grade].fattecktimes );
	}

	if( result > 0 )
		return result;
	else
		return 0;
}

BOOL _XGameItem::CheckClan(int cType, int sID)
{
#ifdef _XTS_ITEM_OPTIMIZATION
	switch(GetEqualItemType(cType))
	{
	case _XGI_FC_WEAPON:
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( WeaponItem )
			{
				if( WeaponItem[sID].cApplyClan == 11 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
						return TRUE;
				}
				else if( WeaponItem[sID].cApplyClan == 12 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM	 ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( WeaponItem[sID].cApplyClan == 13 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( WeaponItem[sID].cApplyClan == 20 )
				{
					return TRUE;
				}
				else
				{
					if( WeaponItem[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
						return TRUE;
				}
			}				
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
			{
				if( ClothesItem[sID].cApplyClan == 11 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
						return TRUE;
				}
				else if( ClothesItem[sID].cApplyClan == 12 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( ClothesItem[sID].cApplyClan == 13 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( ClothesItem[sID].cApplyClan == 20 )
				{
					return TRUE;
				}
				else
				{
					if( ClothesItem[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
						return TRUE;
				}
			}				
		}		
		break;		
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
			{
				if( BookItem[sID].cApplyClan == 11 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
						return TRUE;
				}
				else if( BookItem[sID].cApplyClan == 12 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( BookItem[sID].cApplyClan == 13 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( BookItem[sID].cApplyClan == 20 )
				{
					return TRUE;
				}
				else
				{
					if( BookItem[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
						return TRUE;
				}
			}				
		}
		break;
	case _XGI_FC_POTION:
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
			{
				if( PotionItem[sID].cApplyClan == 11 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
						return TRUE;
				}
				else if( PotionItem[sID].cApplyClan == 12 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( PotionItem[sID].cApplyClan == 13 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( PotionItem[sID].cApplyClan == 20 )
				{
					return TRUE;
				}
				else
				{
					if( PotionItem[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
						return TRUE;
				}
			}				
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ConsumingItem )
			{
				if( ConsumingItem[sID].cApplyClan == 11 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
						return TRUE;								
				}
				else if( ConsumingItem[sID].cApplyClan == 12 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( ConsumingItem[sID].cApplyClan == 13 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( ConsumingItem[sID].cApplyClan == 20 )
				{
					return TRUE;
				}
				else
				{
					if( ConsumingItem[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
						return TRUE;
				}
			}				
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
			{
				if( ElixirItem[sID].cApplyClan == 11 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
						return TRUE;
				}
				else if( ElixirItem[sID].cApplyClan == 12 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( ElixirItem[sID].cApplyClan == 13 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( ElixirItem[sID].cApplyClan == 20 )
				{
					return TRUE;
				}
				else
				{
					if( ElixirItem[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
						return TRUE;
				}
			}				
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);				
			if( ResourceItem )
			{
				if( ResourceItem[sID].cApplyClan == 11 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
						return TRUE;
				}
				else if( ResourceItem[sID].cApplyClan == 12 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( ResourceItem[sID].cApplyClan == 13 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( ResourceItem[sID].cApplyClan == 20 )
				{
					return TRUE;
				}
				else
				{
					if( ResourceItem[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
						return TRUE;
				}
			}				
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
			{
				if( AccessoryItem[sID].cApplyClan == 11 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
						return TRUE;
				}
				else if( AccessoryItem[sID].cApplyClan == 12 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( AccessoryItem[sID].cApplyClan == 13 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( AccessoryItem[sID].cApplyClan == 20 )
				{
					return TRUE;
				}
				else
				{
					if( AccessoryItem[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
						return TRUE;
				}
			}				
		}
		break;
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
			{
				if( LifeItem[sID].cApplyClan == 11 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
						return TRUE;
				}
				else if( LifeItem[sID].cApplyClan == 12 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( LifeItem[sID].cApplyClan == 13 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( LifeItem[sID].cApplyClan == 20 )
				{
					return TRUE;
				}
				else
				{
					if( LifeItem[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
						return TRUE;
				}
			}				
		}
		break;	
	case _XGI_FC_SOCKET :
		{
			_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
			if( SocketItem )
			{
				if( SocketItem[sID].cApplyClan == 11 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
						return TRUE;
				}
				else if( SocketItem[sID].cApplyClan == 12 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( SocketItem[sID].cApplyClan == 13 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( SocketItem[sID].cApplyClan == 20 )
				{
					return TRUE;
				}
				else
				{
					if( SocketItem[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
						return TRUE;
				}
			}				
		}
		break;
	case _XGI_FC_BOX :
		{
			_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxItem )
			{
				if( BoxItem[sID].cApplyClan == 11 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
						return TRUE;
				}
				else if( BoxItem[sID].cApplyClan == 12 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( BoxItem[sID].cApplyClan == 13 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( BoxItem[sID].cApplyClan == 20 )
				{
					return TRUE;
				}
				else
				{
					if( BoxItem[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
						return TRUE;
				}
			}				
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxkeyItem )
			{
				if( BoxkeyItem[sID].cApplyClan == 11 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
						return TRUE;
				}
				else if( BoxkeyItem[sID].cApplyClan == 12 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( BoxkeyItem[sID].cApplyClan == 13 )
				{
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
						g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
						return TRUE;
				}
				else if( BoxkeyItem[sID].cApplyClan == 20 )
				{
					return TRUE;
				}
				else
				{
					if( BoxkeyItem[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
						return TRUE;
				}
			}				
		}
		break;			
	}
#else
	switch(cType) 
	{
	case _XGI_FC_WEAPON:
		{
			if( g_WeaponItemProperty[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_WeaponItemProperty[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM	 ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_WeaponItemProperty[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_WeaponItemProperty[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_WeaponItemProperty[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			if( g_ClothesItemProperty[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_ClothesItemProperty[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_ClothesItemProperty[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_ClothesItemProperty[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_ClothesItemProperty[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}		
		break;
	case _XGI_FC_WEAPON2:
		{
			if( g_WeaponItemProperty2[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_WeaponItemProperty2[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_WeaponItemProperty2[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_WeaponItemProperty2[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_WeaponItemProperty2[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}		
		break;
	case _XGI_FC_CLOTHES2:
		{
			if( g_ClothesItemProperty2[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_ClothesItemProperty2[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_ClothesItemProperty2[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_ClothesItemProperty2[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_ClothesItemProperty2[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}		
		break;
	case _XGI_FC_WEAPON3:
		{
			if( g_WeaponItemProperty3[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_WeaponItemProperty3[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_WeaponItemProperty3[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_WeaponItemProperty3[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_WeaponItemProperty3[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}		
		break;
	case _XGI_FC_CLOTHES3:
		{
			if( g_ClothesItemProperty3[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_ClothesItemProperty3[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_ClothesItemProperty3[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_ClothesItemProperty3[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_ClothesItemProperty3[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}		
		break;
	case _XGI_FC_BOOK :
		{
			if( g_BookItemProperty[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_BookItemProperty[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_BookItemProperty[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_BookItemProperty[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_BookItemProperty[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}
		break;
	case _XGI_FC_POTION:
		{
			if( g_PotionItemProperty[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_PotionItemProperty[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_PotionItemProperty[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_PotionItemProperty[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_PotionItemProperty[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			if( g_ConsumingItemProperty[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;								
			}
			else if( g_ConsumingItemProperty[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_ConsumingItemProperty[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_ConsumingItemProperty[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_ConsumingItemProperty[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			if( g_ElixirItemProperty[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_ElixirItemProperty[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_ElixirItemProperty[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_ElixirItemProperty[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_ElixirItemProperty[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			if( g_ResourceItemProperty[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_ResourceItemProperty[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_ResourceItemProperty[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_ResourceItemProperty[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_ResourceItemProperty[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			if( g_AccessoryItemProperty[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_AccessoryItemProperty[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_AccessoryItemProperty[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_AccessoryItemProperty[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_AccessoryItemProperty[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}
		break;
	case _XGI_FC_LIFE :
		{
			if( g_LifeItemProperty[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_LifeItemProperty[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_LifeItemProperty[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_LifeItemProperty[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_LifeItemProperty[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}
		break;	
	case _XGI_FC_SOCKET :
		{
			if( g_SocketItemProperty[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_SocketItemProperty[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_SocketItemProperty[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_SocketItemProperty[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_SocketItemProperty[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}
		break;
	case _XGI_FC_BOX :
		{
			if( g_BoxItemProperty[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_BoxItemProperty[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_BoxItemProperty[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_BoxItemProperty[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_BoxItemProperty[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			if( g_BoxKeyItemProperty[sID].cApplyClan == 11 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
					return TRUE;
			}
			else if( g_BoxKeyItemProperty[sID].cApplyClan == 12 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_BoxKeyItemProperty[sID].cApplyClan == 13 )
			{
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM	||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
					return TRUE;
			}
			else if( g_BoxKeyItemProperty[sID].cApplyClan == 20 )
			{
				return TRUE;
			}
			else
			{
				if( g_BoxKeyItemProperty[sID].cApplyClan == g_pLocalUser->m_CharacterInfo.groupindex )
					return TRUE;
			}
		}
		break;
	}
#endif
	return FALSE;
}

// 금액에 , 와 색깔 표시
BOOL _XGameItem::GetMoneyColor(TCHAR* money, int& colorindex, BOOL mode )
{
	int length = strlen(money);
	if( length > 16 )
		return FALSE;

	TCHAR tempMoneyBuff[64];
	memset( tempMoneyBuff, 0, sizeof(TCHAR)*64);

	if( mode )
	{
		int i = 0, j = 0;
		
		while ( i < length ) 
		{
			if( money[i] != ',' )
			{
				tempMoneyBuff[j] = money[i];
				j++;
			}
			i++;
		}
		strcpy( money, tempMoneyBuff );
	}
	else
	{		
		int i = 0, j = 0;
		int rank = 0;
		bool flag = true;
		while ( i < length ) 
		{
			if( i!=0 && ((length-i)%3)==0 && flag )
			{
				tempMoneyBuff[j] = ',';
				flag = false;
			}
			else
			{
				tempMoneyBuff[j] = money[i];
				i++;
				flag = true;
			}
			j++;
		}
		
		strcpy( money, tempMoneyBuff );
		
		if( length >= 5 )
		{
			colorindex = length - 4;
			if( colorindex > 5 )
				colorindex = 5;
		}
		else
			colorindex = 0;
	}
	
	return TRUE;
}

BOOL _XGameItem::CheckFame(unsigned char ucfame)
{
	if( ucfame == 0 )
		return TRUE;

#ifdef _XTS_FAME
	if( ucfame > 100 )	// 악명
	{
		if( g_pLocalUser->m_CharacterInfo.Get_Notoriety() > 0 )			
		{
			int notoriety = g_FameStr->GetInfamyIndex(g_pLocalUser->m_CharacterInfo.Get_Notoriety());
			int honor = g_FameStr->GetFameIndex(g_pLocalUser->m_CharacterInfo.Get_honor());

#ifdef _XTS_ITEM_V22
			if(ucfame == 199)
			{
				if(g_pLocalUser->m_CharacterInfo.Get_Notoriety() >= g_pLocalUser->m_CharacterInfo.Get_honor())	// 악명이 명성보다 높을 경우 가능
					return TRUE;
				else
					return FALSE;
			}
#endif
			
			if( notoriety >= ucfame-101 )
				return TRUE;
		}
	}
	else	// 명성
	{
		if( g_pLocalUser->m_CharacterInfo.Get_honor() > 0 )
		{
			int notoriety = g_FameStr->GetInfamyIndex(g_pLocalUser->m_CharacterInfo.Get_Notoriety());
			int honor = g_FameStr->GetFameIndex(g_pLocalUser->m_CharacterInfo.Get_honor());

#ifdef _XTS_ITEM_V22
			if(ucfame == 99)
			{
				if(g_pLocalUser->m_CharacterInfo.Get_Notoriety() < g_pLocalUser->m_CharacterInfo.Get_honor())	// 명성이 악명보다 높을 경우 가능
					return TRUE;
				else
					return FALSE;
			}
#endif

			if( honor >= ucfame-1 )
				return TRUE;
		}
	}
#endif
	return FALSE;
}

BOOL _XGameItem::CheckAccessoryReq(int cType, int sID)
{
	//original stats로 체크 한다.

	if(cType != _XGI_FC_ACCESSORY)
		return 0;

	int retvalue = 0;

#ifdef _XTS_ITEM_V22

	// 근력
	if(g_pLocalUser->m_CharacterInfo.Get_strength() >= g_AccessoryItemProperty[sID].sReqStr)
		retvalue = 0;
	else
		return 1;

	// 건강
	if(g_pLocalUser->m_CharacterInfo.Get_constitution() >= g_AccessoryItemProperty[sID].sReqHealth)
		retvalue = 0;
	else
		return 2;

	// 진기
	if(g_pLocalUser->m_CharacterInfo.Get_zen() >= g_AccessoryItemProperty[sID].sReqJingi)
		retvalue = 0;
	else
		return 3;

	// 지혜
	if(g_pLocalUser->m_CharacterInfo.Get_intelligence() >= g_AccessoryItemProperty[sID].sReqInt)
		retvalue = 0;
	else
		return 4;

	// 민첩
	if(g_pLocalUser->m_CharacterInfo.Get_dexterity() >= g_AccessoryItemProperty[sID].sReqDex)
		retvalue = 0;
	else
		return 5;

	unsigned char ucfame = g_AccessoryItemProperty[sID].ucFame;

	if(ucfame == 199)
	{
		if(g_pLocalUser->m_CharacterInfo.Get_Notoriety() >= g_pLocalUser->m_CharacterInfo.Get_honor())	// 악명이 명성보다 높을 경우 가능
			retvalue = 0;
		else
			return 6;
	}

	if(ucfame == 99)
	{
		if(g_pLocalUser->m_CharacterInfo.Get_Notoriety() < g_pLocalUser->m_CharacterInfo.Get_honor())	// 명성이 악명보다 높을 경우 가능
			retvalue = 0;
		else
			return 7;
	}


	return retvalue;
	
#else
	return 0;
#endif	
}

BOOL _XGameItem::CheckItemTempField4( char cType, short sID )
{
#ifdef _XDEF_WAREHOSE_RESTRAINT_OF_TRADE //Author : 양희왕
	switch( cType )
	{
	case _XGI_FC_WEAPON :
		if(g_WeaponItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_WEAPON2 :	
		if(g_WeaponItemProperty2[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_WEAPON3 :	
		if(g_WeaponItemProperty3[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_CLOTHES :
		if(g_ClothesItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_CLOTHES2 :	
		if(g_ClothesItemProperty2[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_CLOTHES3 :	
		if(g_ClothesItemProperty3[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_BOOK :		
		if(g_BookItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_POTION :		
		if(g_PotionItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_CONSUMING :	
		if(g_ConsumingItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_ELIXIR :		
		if(g_ElixirItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_COUNTERACT :	
		if(g_CounteractItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_RESOURCE :	
		if(g_ResourceItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_QUEST :		
		if(g_QuestItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_ACCESSORY :	
		if(g_AccessoryItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_LIFE :		
		if(g_LifeItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_MUSIC :	
		if(g_MusicItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_PASS :		
		if(g_PassItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_SOCKET :		
		if(g_SocketItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_BOX :		
		if(g_BoxItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	case _XGI_FC_BOXKEY :		
		if(g_BoxKeyItemProperty[sID].sTempField4 == 1 )
			return TRUE;
		break;
	}
#endif //Last Updated :07/04/26

	return FALSE;
}

int _XGameItem::GetSetItem( char cType, short sID )
{
#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트아이템
	switch( cType )
	{
	case _XGI_FC_WEAPON :
		return static_cast<int>(g_WeaponItemProperty[sID].sGroup);
		break;
	case _XGI_FC_WEAPON2 :	
		return static_cast<int>(g_WeaponItemProperty2[sID].sGroup);
		break;
	case _XGI_FC_WEAPON3 :	
		return static_cast<int>(g_WeaponItemProperty3[sID].sGroup);
		break;
	case _XGI_FC_CLOTHES :
		return static_cast<int>(g_ClothesItemProperty[sID].ucGroup);
		break;
	case _XGI_FC_CLOTHES2 :	
		return static_cast<int>(g_ClothesItemProperty2[sID].ucGroup);
		break;
	case _XGI_FC_CLOTHES3 :	
		return static_cast<int>(g_ClothesItemProperty3[sID].ucGroup);
		break;
	case _XGI_FC_ACCESSORY :	
		return static_cast<int>(g_AccessoryItemProperty[sID].sGroup);
		break;
	default :
		return 0;
	}
#endif

	return -1;
}

unsigned char _XGameItem::GetSocketCheck( char cType, short sID ) //미사용
{
#ifdef _XDEF_ITEM_V24
	switch( cType )
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].ucSocketCheck;
		break;
	case _XGI_FC_WEAPON2 :	
		return g_WeaponItemProperty2[sID].ucSocketCheck;
		break;
	case _XGI_FC_WEAPON3 :	
		return g_WeaponItemProperty3[sID].ucSocketCheck;
		break;
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].ucSocketCheck;
		break;
	case _XGI_FC_CLOTHES2 :	
		return g_ClothesItemProperty2[sID].ucSocketCheck;
		break;
	case _XGI_FC_CLOTHES3 :	
		return g_ClothesItemProperty3[sID].ucSocketCheck;
		break;
	default :
		return 0;
	}
#endif
	return 0;
}

short _XGameItem::GetTempField5( char cType, short sID ) // 수집가능 조합가능 검사
{
	switch( cType )
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty[sID].sTempField5;
		break;
	case _XGI_FC_WEAPON2 :	
		return g_WeaponItemProperty2[sID].sTempField5;
		break;
	case _XGI_FC_WEAPON3 :	
		return g_WeaponItemProperty3[sID].sTempField5;
		break;
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty[sID].sTempField5;
		break;
	case _XGI_FC_CLOTHES2 :	
		return g_ClothesItemProperty2[sID].sTempField5;
		break;
	case _XGI_FC_CLOTHES3 :	
		return g_ClothesItemProperty3[sID].sTempField5;
		break;
	default :
		return 0;
	}
	return -1; // 에러..
}


//////////////////////////////////////////////////////////////////////
// _XGameItemList
//////////////////////////////////////////////////////////////////////

_XGameItemList::_XGameItemList()
{
	currentrotateangle = 0.0f;
}
	
	
_XGameItemList::~_XGameItemList()
{


}

#define _XDEF_WORLDITEM_CULLINGRADIUS		30.0f

void _XGameItemList::CheckInsideFrustum( void )
{
	//assert( pQuadTree );
	if( listEmpty() ) return;

	resetList();
	
	_XGameItem* curItem;
	
	do 
	{
		curItem = (_XGameItem*)currentItem();
		
		if( curItem ) 
		{
			curItem->m_InsideFrustum = 
				( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( curItem->m_Position, 0.7f ) != NOT_VISIBLE );			

			if( curItem->m_InsideFrustum )
			{
				FLOAT dx, dz;
				dx = (g_pLocalUser->m_Position.x - curItem->m_Position.x);
				dz = (g_pLocalUser->m_Position.z - curItem->m_Position.z);
				curItem->m_InsideFrustum = (_XFastDistance( dx, dz ) <= _XDEF_WORLDITEM_CULLINGRADIUS); 
			}
		}
		else
		{
			break;
		}
		
		nextItem();
	} while( !atHeadOfList() );
}


#define _XDEF_ITEMSELFDESTROYTIMER		120000

void _XGameItemList::Render( void )
{	
	if( listEmpty() ) return;
	
	resetList();

	_XGameItem* curItem;

	currentrotateangle+=1.0f;
	if( currentrotateangle >= 360.0f )
	{
		currentrotateangle = 0.0f;
	}

	D3DXMATRIX	rotmat;
	D3DXMATRIX	curmat;		
	D3DXMatrixRotationY( &rotmat, _X_RAD( currentrotateangle ) );

	//_XMeshContainer* pArrowModel = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ArrowModel;	

	D3DXVECTOR3 nRayDir;
	D3DXVec3Normalize( &nRayDir, &g_vPickRayDir );
	BOOL intersect = FALSE;

	do 
	{
		curItem = (_XGameItem*)currentItem();

		if( curItem )
		{
			if( curItem->m_InsideFrustum && curItem->m_pMeshContainer )
			{
				//_XDWINPRINT("Item world id  : %d ", curItem->m_ItemWorldID);

				// 게임아이템 OBB로 체크하기 
				FLOAT dx, dz;
				D3DXVECTOR3 mpos = curItem->m_Position;
				mpos.y -= curItem->m_pMeshContainer->OBB_Center.y;
				mpos.y += fabs( curItem->m_pMeshContainer->OBB_Extent[1] );
				
				dx = (curItem->m_Position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
				dz = (curItem->m_Position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
				FLOAT cameradistance = _XFC_sqrt( dx*dx + dz*dz );
				
				
				// Check intersect plane space...
				intersect = RayAndSphereIntersect( g_vPickRayOrig, nRayDir, mpos, curItem->m_pMeshContainer->m_ObjectRadius );
				
				BOOL clothescategory = (curItem->m_cType == _XGI_FC_CLOTHES) || (curItem->m_cType == _XGI_FC_CLOTHES2) || (curItem->m_cType == _XGI_FC_CLOTHES3);

				if( intersect )
				{
					if( curItem->m_pMeshContainer )
					{
						// 게임아이템 OBB로 체크하는 함수
						D3DXVECTOR3 PickedPoint;						
						intersect = curItem->m_pMeshContainer->InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir, mpos, &PickedPoint );
						
						if( intersect )
						{
							if( cameradistance < g_LastPickedObjectDistance )
							{
								g_LastPickedObjectDistance = cameradistance;
								g_LastPickedObjectType = _XPICKINGTARGET_ITEM;
								g_LastPickedObject = curItem;
								
								CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
								eodapp->ReadyCursor( _XMOUSETYPE_PICKUP );
							}

							if( (curItem->m_cType == _XGI_FC_CLOTHES && curItem->m_cSecond == _XGI_SC_CLOTHES_SHOES) ||
								(curItem->m_cType == _XGI_FC_CLOTHES2 && curItem->m_cSecond == _XGI_SC_CLOTHES_SHOES) ||
								(curItem->m_cType == _XGI_FC_CLOTHES3 && curItem->m_cSecond == _XGI_SC_CLOTHES_SHOES) )
								curItem->m_pMeshContainer->Render_OriginalPos( mpos, 0, NULL, TRUE );
							else
								curItem->m_pMeshContainer->Render_OriginalPos( mpos, 0, &rotmat, clothescategory );
						}				
						else
						{
							if( (curItem->m_cType == _XGI_FC_CLOTHES && curItem->m_cSecond == _XGI_SC_CLOTHES_SHOES) ||
								(curItem->m_cType == _XGI_FC_CLOTHES2 && curItem->m_cSecond == _XGI_SC_CLOTHES_SHOES) ||
								(curItem->m_cType == _XGI_FC_CLOTHES3 && curItem->m_cSecond == _XGI_SC_CLOTHES_SHOES) )
								curItem->m_pMeshContainer->Render_OriginalPos( mpos, 0, NULL, TRUE );
							else
							{
								curItem->m_pMeshContainer->Render_OriginalPos( mpos, 0, &rotmat, clothescategory );
							}
						}
					}
				}
				else
				{
					if( (curItem->m_cType == _XGI_FC_CLOTHES && curItem->m_cSecond == _XGI_SC_CLOTHES_SHOES) ||
						(curItem->m_cType == _XGI_FC_CLOTHES2 && curItem->m_cSecond == _XGI_SC_CLOTHES_SHOES) ||
						(curItem->m_cType == _XGI_FC_CLOTHES3 && curItem->m_cSecond == _XGI_SC_CLOTHES_SHOES) )
						curItem->m_pMeshContainer->Render_OriginalPos( mpos, 0, NULL, TRUE );
					else
					{
						curItem->m_pMeshContainer->Render_OriginalPos( mpos, 0, &rotmat, clothescategory );
					}
				}

#ifdef _XDWDEBUG								
				if( g_LodTerrain.m_ViewOBBObject )
				{
					D3DXMATRIX posmat;
					D3DXMatrixIdentity( &posmat );
					posmat._41 = mpos.x;
					posmat._42 = mpos.y;
					posmat._43 = mpos.z;
					curItem->m_pMeshContainer->m_OBBBox.Render( posmat, NULL );
					g_LastUsedRenderStyle = NULL;
				}
#endif
				
				if(g_ViewItemNameTooltip)
				{
					gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

					mpos.y = curItem->m_Position.y + (curItem->m_pMeshContainer->OBB_Center.y + fabs( curItem->m_pMeshContainer->OBB_Extent[1] )) + 0.3f;

					D3DXVECTOR3 rvec;
										
					D3DXVec3Project( &rvec, &mpos, &g_MainViewPort,
									 &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
									 &g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL );
					
					D3DCOLOR itemnamecolor = _XGameItem::GetGradeColor(curItem->m_cType, curItem->m_sID);					

					if( !g_pLocalUser->m_InParty )
					{
						TCHAR str[512];
						memset(str, 0, sizeof(str));
												
						if(curItem->m_cType == _XGI_FC_MONEY)
						{
							_snprintf(str, sizeof(str), _XGETINTERFACETEXT(ID_STRING_NPCSKILL_MONEY), curItem->m_uiCount); // "금 %d"
						}
						else
						{
							if( curItem->m_ucCount > 1)
							{
								LPTSTR param1 = _XGameItem::GetItemName(curItem->m_cType, curItem->m_sID);
								int    param2 = curItem->m_ucCount;

								// "%s %d개"
								strncpy( str, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_GAMEITEM_1570, param1, &param2 ), sizeof(str) );
							}
							else
							{
#ifdef _XTS_ITEM_V20
								strncpy( str, _XGameItem::GetItemFullName(curItem->m_cType, curItem->m_sID), sizeof(str) );
#else
								if( curItem->m_cType == _XGI_FC_WEAPON  || curItem->m_cType == _XGI_FC_CLOTHES  ||
									curItem->m_cType == _XGI_FC_WEAPON2 || curItem->m_cType == _XGI_FC_CLOTHES2 || 
									curItem->m_cType == _XGI_FC_WEAPON3 || curItem->m_cType == _XGI_FC_CLOTHES3 )
								{	
									_snprintf( str, sizeof(str), _T("%s %s"), _XGameItem::GetItemNick(curItem->m_cType, curItem->m_sID), _XGameItem::GetItemName(curItem->m_cType, curItem->m_sID) );
								}
								else
								{
									strncpy( str, _XGameItem::GetItemName(curItem->m_cType, curItem->m_sID), sizeof(str) );
								}
#endif
							}

#ifdef _XDWDEBUG
//							FLOAT dx = (g_pLocalUser->m_Position.x - curItem->m_Position.x);
//							FLOAT dz = (g_pLocalUser->m_Position.z - curItem->m_Position.z);
//							FLOAT distance = _XFC_sqrt( dx*dx + dz*dz );
//							TCHAR tempView[256];						
//							_snprintf( tempView, sizeof(tempView), _T("Owner ID[%d] / My ID[%d] : distance [%.2f]"), curItem->m_usOwnerID, g_pLocalUser->m_Uniqueid, distance );
#endif
						}

						if( str[0] != 0 )
						{
							g_ItemTooltip.SetToolTipStringEx( rvec.x, rvec.y, str, itemnamecolor, 32, TRUE, FALSE );
#ifdef _XDWDEBUG
//							g_ItemTooltip.SetToolTipStringEx( rvec.x, rvec.y+20, tempView, 0xffffff00, 32, TRUE, FALSE );
#endif
							g_ItemTooltip.DrawAlpha();
						}
						
						POINT pt = g_ItemTooltip.GetWindowPos();
						SIZE  sz = g_ItemTooltip.GetWindowSize();
						
						ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
						if( ((scrnpos->x > pt.x )   && 
							(scrnpos->x < pt.x + sz.cx ) &&
							(scrnpos->z > pt.y )   && 
							(scrnpos->z < pt.y + sz.cy) ) )
						{
							g_LastPickedObjectDistance = cameradistance;
							g_LastPickedObjectType = _XPICKINGTARGET_ITEM;
							g_LastPickedObject = curItem;
							
							CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
							eodapp->ReadyCursor( _XMOUSETYPE_PICKUP );
						}
					}
					else // 파티일때는 소유자를 표시한다.
					{
						TCHAR str[64];
						memset( str, 0, sizeof(TCHAR)*64);
						int partyindex = -1;
						if(curItem->m_cType == _XGI_FC_MONEY)
						{
							for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++)
							{
								if( g_pLocalUser->m_PartyInfo[i].playeruniqueid != 0 )
								{
									if( g_pLocalUser->m_PartyInfo[i].playeruniqueid == curItem->m_usOwnerID )
									{
										partyindex = i;
										break;
									}
								}
							}
							
							_snprintf(str, sizeof(str), _T("%s %d"), _XGETINTERFACETEXT(ID_STRING_NETWORK_MONEY), curItem->m_uiCount);
						
						}
						else
						{
							for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT; i++)
							{
								if( g_pLocalUser->m_PartyInfo[i].playeruniqueid != 0 )
								{
									if( g_pLocalUser->m_PartyInfo[i].playeruniqueid == curItem->m_usOwnerID )
									{
										partyindex = i;
										break;
									}
								}
							}
							
							if( curItem->m_ucCount > 1)
							{
								LPTSTR param1 = _XGameItem::GetItemName(curItem->m_cType, curItem->m_sID);
								int    param2 = curItem->m_ucCount;
								
								// "%s %d개"
								strncpy( str, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_GAMEITEM_1570, param1, &param2 ), sizeof(str) );
							}
							else
							{
#ifdef _XTS_ITEM_V20
								strncpy( str, _XGameItem::GetItemFullName(curItem->m_cType, curItem->m_sID), sizeof(str) );
#else
								if( curItem->m_cType == _XGI_FC_WEAPON  || curItem->m_cType == _XGI_FC_CLOTHES  ||
									curItem->m_cType == _XGI_FC_WEAPON2 || curItem->m_cType == _XGI_FC_CLOTHES2 || 
									curItem->m_cType == _XGI_FC_WEAPON3 || curItem->m_cType == _XGI_FC_CLOTHES3 )
								{	
									_snprintf( str, sizeof(str), _T("%s %s"), _XGameItem::GetItemNick(curItem->m_cType, curItem->m_sID), _XGameItem::GetItemName(curItem->m_cType, curItem->m_sID) );
								}
								else
								{
									strncpy( str, _XGameItem::GetItemName(curItem->m_cType, curItem->m_sID), sizeof(str) );
								}
#endif
							}
						}

						if( partyindex == -1 )
						{
							if( str[0] != 0 )
							{
								g_ItemTooltip.SetToolTipStringEx( rvec.x, rvec.y, str, itemnamecolor, 32, TRUE, FALSE );
								g_ItemTooltip.DrawAlpha();
							}
							
							POINT pt = g_ItemTooltip.GetWindowPos();
							SIZE  sz = g_ItemTooltip.GetWindowSize();
							
							ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
							if( ((scrnpos->x > pt.x )   && 
								(scrnpos->x < pt.x + sz.cx ) &&
								(scrnpos->z > pt.y )   && 
								(scrnpos->z < pt.y + sz.cy) ) )
							{
								g_LastPickedObjectDistance = cameradistance;
								g_LastPickedObjectType = _XPICKINGTARGET_ITEM;
								g_LastPickedObject = curItem;
								
								CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
								eodapp->ReadyCursor( _XMOUSETYPE_PICKUP );
							}
						}
						else
						{
							if( str[0] != 0 )
							{
								// 아이템 바탕 ( 94 X 38)
								int length = (int)(g_XBaseFont->GetWidth(str)>>1);
								if( length <= 47 ) length = 47;

								_XDrawSolidBar( rvec.x-6-length,  rvec.y-38, rvec.x+6+length, rvec.y , D3DCOLOR_ARGB(180,61,68,73) );
								_XDrawRectAngle( rvec.x-6-length,  rvec.y-38, rvec.x+6+length, rvec.y , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );	
								
								g_XBaseFont->SetColor( itemnamecolor );
								g_XBaseFont->PrintAlign( rvec.x+2, rvec.y-31,  1.0f, _XFONT_ALIGNTYPE_CENTER, str );

								if( g_pLocalUser->m_PartyInfo[partyindex].playername[0] != 0 )
								{
									if( strcmp(g_pLocalUser->m_PartyInfo[partyindex].playername,g_pLocalUser->m_CharacterInfo.charactername ) == 0 )
									{
										g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
									}
									else
									{
										g_XBaseFont->SetColor( D3DCOLOR_ARGB(255,170,164,154) );
									}
									TCHAR playername[64];
									_snprintf(playername, sizeof(playername), "[%s]", g_pLocalUser->m_PartyInfo[partyindex].playername);
									g_XBaseFont->PrintAlign( rvec.x+2, rvec.y-14, 1.0f, _XFONT_ALIGNTYPE_CENTER, playername );
								}
								g_XBaseFont->Flush();
								
								ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
								if( ((scrnpos->x > rvec.x-6-length) && 
									(scrnpos->x < rvec.x+6+length ) &&
									(scrnpos->z > rvec.y-38 ) && 
									(scrnpos->z < rvec.y) ) )
								{
									g_LastPickedObjectDistance = cameradistance;
									g_LastPickedObjectType = _XPICKINGTARGET_ITEM;
									g_LastPickedObject = curItem;
									
									CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
									eodapp->ReadyCursor( _XMOUSETYPE_PICKUP );
								}
							}

						}
					}
					

					gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );
				}
			}
			nextItem();
		}
		else
		{
			break;
		}				
	} while( !atHeadOfList() );
}

_XGameItem* _XGameItemList::FindWorldItem( int itemworldid )
{
	if( listEmpty() ) return NULL;
	
	_XGameItem* pItem;
	
	resetList();
	do 
	{
		pItem = (_XGameItem*)currentItem();
		if( !pItem ) break;
		
		if( pItem->m_ItemWorldID == itemworldid )
		{
			return pItem;			
		}
		
		nextItem();
	} while( !atHeadOfList() );

	return NULL;
}

void _XGameItemList::RemoveWorldItem( int itemworldid )
{
	if( listEmpty() ) return;

	_XGameItem* pItem;

	resetList();
	do 
	{
		pItem = (_XGameItem*)currentItem();
		if( !pItem ) break;

		if( pItem->m_ItemWorldID == itemworldid )
		{
			if( g_LastPickedObject == (_XItem*)pItem )
			{
				g_LastPickedObject = NULL;
				g_LastPickedObjectType = _XPICKINGTARGET_NONE;
			}

			deleteItem( pItem );
			break;
		}
		
		nextItem();
	} while( !atHeadOfList() );
}

void _XGameItemList::RemoveWorldItem(void)
{
	if(listEmpty())
		return;
	
	_XGameItem* pItem;
	
	resetList();
	do 
	{
		pItem = (_XGameItem*)currentItem();
		if(!pItem)
			break;
		
		//if((g_LocalSystemTime - pItem->m_LastReceiveMessageTime) > 10000)
		if(!pItem->m_bLiveWorldID) // 2004.06.03->oneway48 insert
		{
			if(g_LastPickedObject == (_XItem*)pItem)
			{
				g_LastPickedObject = NULL;
				g_LastPickedObjectType = _XPICKINGTARGET_NONE;
			}
			
			deleteItem(pItem);
		}
		nextItem();
	} while(!atHeadOfList());
	
}
// 2004.06.03->oneway48 insert 
void _XGameItemList::SetDefaultLiveWorldID()
{
	if(listEmpty())
		return;
	
	_XGameItem* pItem;
	
	resetList();
	do 
	{
		pItem = (_XGameItem*)currentItem();
		if(!pItem)
			break;		
		pItem->m_bLiveWorldID = FALSE;
		
		nextItem();
	} while(!atHeadOfList());
}
// insert end


// 2004.06.25->oneway48 insert 
BOOL _XGameItemList::GrabWorldItem()
{
	if(listEmpty())
		return false;

	_XGameItem* pItem;
	_XGameItem* pNearItem = NULL;
	resetList();
	do 
	{
		pItem = (_XGameItem*)currentItem();
		
		if(pItem)
		{
			if( pItem->m_InsideFrustum )
			{
				if( pItem->m_usOwnerID == 0 || pItem->m_usOwnerID == g_pLocalUser->m_Uniqueid )
				{
					FLOAT dx = (g_pLocalUser->m_Position.x - pItem->m_Position.x);
					FLOAT dz = (g_pLocalUser->m_Position.z - pItem->m_Position.z);
					FLOAT distance = _XFC_sqrt( dx*dx + dz*dz );
					if( !pNearItem )
					{
						pNearItem = pItem;
					}
					else
					{
						FLOAT ndx = (g_pLocalUser->m_Position.x - pNearItem->m_Position.x);
						FLOAT ndz = (g_pLocalUser->m_Position.z - pNearItem->m_Position.z);
						FLOAT ndistance = _XFC_sqrt( ndx*ndx + ndz*ndz );
						
						if( distance < ndistance )
							pNearItem = pItem;				
					}				
				}
			}				
			nextItem();
		}
	} while(!atHeadOfList());

	if( pNearItem )
	{
 		FLOAT ldx = (g_pLocalUser->m_Position.x - pNearItem->m_Position.x);
		FLOAT ldz = (g_pLocalUser->m_Position.z - pNearItem->m_Position.z);
		FLOAT ldistance = _XFC_sqrt( ldx*ldx + ldz*ldz );
		
		if( ldistance < 5.0f )
		{ 
 			g_pLocalUser->m_PickItem = pNearItem;
			return TRUE;
		}
	}
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// _XUserItem
//////////////////////////////////////////////////////////////////////
_XUserItem::_XUserItem()
{
	Reset();
}

_XUserItem::~_XUserItem()
{
}

void _XUserItem::Reset( void )
{
	Set_m_cType(-1);
	Set_m_cSecond(0);
	Set_m_sID(0);
	m_cIdentify			= 0;

	Set_m_cThird(0);
	Set_m_ucCount(0);
	m_cReserve			= 0;

	Set_m_usCurDur(0);
	Set_m_usMaxDur(0);

	for( int i = 0; i < 4; i++ )
	{
		Set_m_sSocket( i, 0 );
	}

	Set_m_ucStrength(0);
	Set_m_ucSharpness(0);
	Set_m_ucWeaponStrength(0); //Author : 양희왕

	Set_m_ucSocketCount(0);

	m_usCurProtect  = 0;
	m_usCurMaxProtect = 0;
	//m_ucIncProtect = 0;
	m_usUsedProtect = 0;
	
	m_bDurabilityDecrease = FALSE;
	m_bNewItem			= FALSE;
	m_bQuestReward		= FALSE;
	
	m_dwCooldownStartTime = 0;

	m_ucFlag1		= 0;
	m_ucFlag2		= 0;
	m_ucFlag3		= 0;
	m_usValue1		= 0;

	Set_m_ucYear(0);
	Set_m_ucMonth(0);
	Set_m_ucDay(0);
	Set_m_ucHour(0);
	Set_m_ucMinute(0);
}

BOOL _XUserItem::IsStackable(void)
{
	switch(Get_m_cType())
	{
	case _XGI_FC_CLOTHES :
	case _XGI_FC_CLOTHES2 :
	case _XGI_FC_CLOTHES3 :	
	case _XGI_FC_ACCESSORY :
	case _XGI_FC_MUSIC :	
	case _XGI_FC_SOCKET :
	case _XGI_FC_ELIXIR :
	case _XGI_FC_BOX :
		return FALSE;

	case _XGI_FC_WEAPON :
	case _XGI_FC_WEAPON2 :
	case _XGI_FC_WEAPON3 :
		{
			if(Get_m_cSecond() == _XGI_SC_WEAPON_HIDDEN)
				return TRUE;
			else
				return FALSE;
		}
		break;
	case _XGI_FC_LIFE :
		{
			switch(Get_m_cSecond()) 
			{
			case _XGI_SC_LIFE_SOCKET :
				return FALSE;
/*			case _XGI_SC_LIFE_CONSUMING :
			case _XGI_SC_LIFE_INCHANTSTUFF :
			case _XGI_SC_LIFE_SOCKETSSTUFF :
			case _XGI_SC_LIFE_SMLETITEM_SCALES :	//Author : 양희왕
			case _XGI_SC_LIFE_SMLETITEM_TEAR: //Author : 양희왕*/
			default :
				return TRUE;
			}
		}
		break;
	case _XGI_FC_BOOK :
		{
			switch(Get_m_cSecond()) 
			{
			case _XGI_SC_BOOK_SKILL:
			case _XGI_SC_BOOK_ABILITY :
			case _XGI_SC_BOOK_STUDY:
				return FALSE;
			case _XGI_SC_BOOK_SCROLL :
			case _XGI_SC_BOOK_STUFF :
			case _XGI_SC_BOOK_PAPER :
				return TRUE;
			}
		}
		break;
	case _XGI_FC_POTION :
		{
			switch(Get_m_cSecond())
			{
			case _XGI_SC_POTION_NOTMERGE :
				return FALSE;
			default :
				return TRUE;
			}
		}
		break;
	case _XGI_FC_CONSUMING :			
	case _XGI_FC_COUNTERACT :
	case _XGI_FC_RESOURCE :
	case _XGI_FC_QUEST :	
	case _XGI_FC_PASS :
	case _XGI_FC_MONEY :
	case _XGI_FC_BOXKEY :
		return TRUE;

	default :
		return FALSE;
	}
	return FALSE;
}

void _XUserItem::Copy(_XUserItem useritem)
{
	Set_m_cType(useritem.Get_m_cType());
	Set_m_cSecond(useritem.Get_m_cSecond() );
	Set_m_sID( useritem.Get_m_sID() );
	m_cIdentify		= useritem.m_cIdentify;
	
	Set_m_cThird(useritem.Get_m_cThird());
	Set_m_ucCount(useritem.Get_m_ucCount());
	
	m_cReserve		= useritem.m_cReserve;
	
	Set_m_usCurDur(useritem.Get_m_usCurDur());
	Set_m_usMaxDur( useritem.Get_m_usMaxDur());

	Set_m_ucSharpness( useritem.Get_m_ucSharpness() );


	Set_m_ucWeaponStrength( useritem.Get_m_ucWeaponStrength() ); //Author : 양희왕

	for( int i = 0; i < 4; i++ )
		Set_m_sSocket(i, useritem.Get_m_sSocket(i) );

	Set_m_ucStrength( useritem.Get_m_ucStrength() );

	Set_m_ucSocketCount( useritem.Get_m_ucSocketCount() );

	//Author : 양희왕 _XDEF_SMELT_ITEM 
	m_usCurProtect  = useritem.m_usCurProtect;
	m_usCurMaxProtect = useritem.m_usCurMaxProtect;
	//m_ucIncProtect = useritem.m_ucIncProtect;
	m_usUsedProtect = useritem.m_usUsedProtect;
	
	m_bDurabilityDecrease = useritem.m_bDurabilityDecrease;	
	m_bQuestReward		= useritem.m_bQuestReward;

	m_ucFlag1		= useritem.m_ucFlag1;
	m_ucFlag2		= useritem.m_ucFlag2;
	m_ucFlag3		= useritem.m_ucFlag3;
	m_usValue1		= useritem.m_usValue1;

	Set_m_ucYear(useritem.Get_m_ucYear());
	Set_m_ucMonth(useritem.Get_m_ucMonth());
	Set_m_ucDay(useritem.Get_m_ucDay());
	Set_m_ucHour(useritem.Get_m_ucHour());
	Set_m_ucMinute(useritem.Get_m_ucMinute());

}

//////////////////////////////////////////////////////////////////////
// _XWarehouseItem
//////////////////////////////////////////////////////////////////////
_XWarehouseItem::_XWarehouseItem()
{
	ResetWarehouseItem();
}

_XWarehouseItem::~_XWarehouseItem()
{
	
}

void _XWarehouseItem::ResetWarehouseItem(void)
{
	Set_m_cType(-1);
	Set_m_cSecond(0);
	Set_m_sID(0);
	m_cIdentify			= 0;
	
	Set_m_cThird(0);
	Set_m_ucCount(0);
	m_cReserve			= 0;
	
	Set_m_usCurDur(0);
	Set_m_usMaxDur(0);

#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
	m_usCurProtect  = 0;
	//m_ucIncProtect = 0;
	m_usCurMaxProtect = 0;
	m_usUsedProtect = 0;
#endif

	for( int i = 0; i < 4; i++ )
	{
		Set_m_sSocket(i,0);
	}

	Set_m_ucSocketCount(0);

	Set_m_ucStrength(0);
	Set_m_ucSharpness(0);
	Set_m_ucWeaponStrength(0);
	
	m_InventoryNumber	= 0;
	m_WarehouseNumber	= 0;
	m_ReceiveCount		= 0;
}

//////////////////////////////////////////////////////////////////////
// _XTradeItem
//////////////////////////////////////////////////////////////////////
_XTradeItem::_XTradeItem()
{
	ResetTradeItem();
}

_XTradeItem::~_XTradeItem()
{
}

void _XTradeItem::ResetTradeItem(void)
{	
	Set_m_cType(-1);
	Set_m_cSecond(0);
	Set_m_sID(0);
	m_cIdentify			= 0;
	
	Set_m_cThird(0);
	Set_m_ucCount(0);
	m_cReserve			= 0;
	
	Set_m_usCurDur(0);
	Set_m_usMaxDur(0);

#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
	m_usCurProtect  = 0;
	m_usCurMaxProtect = 0;
	//m_ucIncProtect = 0;
	m_usUsedProtect = 0;
#endif

	for( int i = 0; i < 4; i++ )
	{
		Set_m_sSocket(i,0);
	}
	
	Set_m_ucSocketCount(0);

	Set_m_ucStrength(0);
	Set_m_ucSharpness(0);
	Set_m_ucWeaponStrength(0);
	
	m_bDurabilityDecrease = FALSE;
	m_bNewItem			= FALSE;
	
	m_InventoryNumber	= 0;
	m_ItemNo			= 0;
}

//////////////////////////////////////////////////////////////////////
// _XCollectionItem
//////////////////////////////////////////////////////////////////////
_XCollectionResourceItem::_XCollectionResourceItem()
{
	ResetCollectionResourceItem();
}

_XCollectionResourceItem::~_XCollectionResourceItem()
{
}

void _XCollectionResourceItem::ResetCollectionResourceItem(void)
{	
	Set_m_cType(-1);
	Set_m_cSecond(0);
	Set_m_sID(0);
	m_cIdentify			= 0;
	
	Set_m_cThird(0);
	Set_m_ucCount(0);
	m_cReserve			= 0;
	
	Set_m_usCurDur(0);
	Set_m_usMaxDur(0);

#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
	m_usCurProtect  = 0;
	//m_ucIncProtect = 0;
	m_usCurMaxProtect = 0;
	m_usUsedProtect = 0;
#endif
	
	for( int i = 0; i < 4; i++ )
	{
		Set_m_sSocket(i,0);
	}
	
	Set_m_ucSocketCount(0);

	Set_m_ucStrength(0);
	Set_m_ucSharpness(0);
	Set_m_ucWeaponStrength(0); //Author : 양희왕
	
	m_bDurabilityDecrease = FALSE;
	m_bNewItem			= FALSE;
	
	m_Index				= -1;
	m_bComplete			= false;
}

/*
// 2004.06.14->oneway48 insert
//////////////////////////////////////////////////////////////////////
// _XUsingItem
//////////////////////////////////////////////////////////////////////
_XUsingItem::_XUsingItem()
{
	ResetUsingItem();
}

_XUsingItem::~_XUsingItem()
{
}

void _XUsingItem::ResetUsingItem(void)
{	
	m_cType				=-1;
	m_cSecond			= 0;
	m_sID				= 0;
	m_cIdentify			= 0;
	
	m_cThird			= 0;
	m_ucCount			= 0;
	m_cReserve			= 0;
	
	m_usCurDur			= 0;
	m_usMaxDur			= 0;

	m_ucSocket[0]		= 0;
	m_ucSocket[1]		= 0;
	m_ucSocket[2]		= 0;
	m_ucSocket[3]		= 0;
	m_ucSocketCount		= 0;	
	
	m_ItemNo			= 0;
	m_InventoryNumber	= 0;
	m_StartTime			= 0;
}
// insert end
*/
//////////////////////////////////////////////////////////////////////
// _XMouseSlotItem
//////////////////////////////////////////////////////////////////////
_XMouseSlotItem::_XMouseSlotItem()
{
	ResetMouseSlotItem();
}

_XMouseSlotItem::~_XMouseSlotItem()
{
}

void _XMouseSlotItem::ResetMouseSlotItem(void)
{
	Set_m_cType(-1);
	Set_m_cSecond(0);
	Set_m_sID(0);
	m_cIdentify			= 0;
	
	Set_m_cThird(0);
	Set_m_ucCount(0);
	m_cReserve			= 0;
	
	Set_m_usCurDur(0);
	Set_m_usMaxDur(0);

	for( int i = 0; i < 4; i++ )
	{
		Set_m_sSocket(i,0);
	}
	
	Set_m_ucSocketCount(0);

	Set_m_ucStrength(0);
	Set_m_ucSharpness(0);
	Set_m_ucWeaponStrength(0);
	
	m_InventoryNumber	= -1;
}

//////////////////////////////////////////////////////////////////////
// _XDisarmamentItem
//////////////////////////////////////////////////////////////////////
_XDisarmamentItem::_XDisarmamentItem()
{
}

_XDisarmamentItem::~_XDisarmamentItem()
{
}

void _XDisarmamentItem::Create(short sID, D3DXVECTOR3 position, _XAnimMeshAbstractClass* pMesh)
{
	m_vecPosition		= position;
	m_pMeshContainer	= pMesh;
	m_sID				= m_sID;
	m_DirectingStep		= _XDEF_DISARM_DIRECTING_START;
	m_LastProcessTime	= 0;
	m_StartProcessTime	= 0;
	m_RotateAngle		= 0.0f;
	m_CurrentPoint		= 0;
}

void _XDisarmamentItem::Render(void)
{
	D3DXMATRIX		rotmat;
	D3DXVECTOR3		startposition, secondposition, thirdposition, endposition;

	switch(m_DirectingStep)
	{
	case _XDEF_DISARM_DIRECTING_START :
		{
			m_LastProcessTime = g_LocalSystemTime;
			m_DirectingStep = _XDEF_DISARM_DIRECTING_LENEAR;
		}
		break;
	case _XDEF_DISARM_DIRECTING_LENEAR :
		{
			if(m_StartProcessTime == 0)
				m_StartProcessTime = g_LocalSystemTime;

			m_RotateAngle += 90.0f;
			if(m_RotateAngle >= 360.0f)
				m_RotateAngle = 0.0f;

			D3DXMatrixRotationX(&rotmat, _X_RAD(m_RotateAngle));

			DWORD elapsedtime = g_LocalSystemTime - m_LastProcessTime;
			if((g_LocalSystemTime - m_StartProcessTime) >= 200)
			{
				m_DirectingStep = _XDEF_DISARM_DIRECTING_CURVE;
				
				_XDWINPRINT("경과 시간 : %d ", g_LocalSystemTime - m_StartProcessTime);

				// curve 만들기
				startposition = m_vecPosition;
				endposition = startposition;
				endposition.x += 2.0f;
				endposition.z += 2.0f;
				_XOBBData* pObb;
				endposition.y = g_LodTerrain.GetTerrainAndOBBHeight(endposition, pObb);
				secondposition = startposition + (endposition - startposition)*0.25f;
				thirdposition = startposition + (endposition - startposition)*0.75f;

				secondposition.y += 1.5f;
				thirdposition.y += 0.5f;

				_XDWINPRINT("startposition : %f/%f/%f ", startposition.x, startposition.y, startposition.z);
				_XDWINPRINT("secondposition : %f/%f/%f ", secondposition.x, secondposition.y, secondposition.z);
				_XDWINPRINT("thirdposition : %f/%f/%f ", thirdposition.x, thirdposition.y, thirdposition.z);
				_XDWINPRINT("endposition : %f/%f/%f ", endposition.x, endposition.y, endposition.z);

				m_PathID = g_LodTerrain.m_PathObjectManager->MakeCurve(startposition, secondposition, thirdposition, endposition, 100);
				m_LastProcessTime = g_LocalSystemTime;
			}
			else
			{
				m_vecPosition.y += (0.008f * elapsedtime);
				m_LastProcessTime = g_LocalSystemTime;

				_XDWINPRINT("== 무장 해제 weapon y position [%f/%d]", m_vecPosition.y, elapsedtime);
			}
		}
		break;
	case _XDEF_DISARM_DIRECTING_CURVE :
		{
			m_RotateAngle += 90.0f;
			if(m_RotateAngle >= 360.0f)
				m_RotateAngle = 0.0f;
			
			D3DXMatrixRotationX(&rotmat, _X_RAD(m_RotateAngle));

//			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_ViewPathObject = TRUE;

			DWORD elapsedtime = g_LocalSystemTime - m_LastProcessTime;
			if(m_CurrentPoint <= 100)
			{
				m_CurrentPoint = (int)(0.125 * elapsedtime);
				m_vecPosition = g_LodTerrain.m_PathObjectManager->m_vPathPoint[m_CurrentPoint];
				break;
			}
			else
			{
				m_CurrentPoint = 100;
				m_vecPosition = g_LodTerrain.m_PathObjectManager->m_vPathPoint[m_CurrentPoint];
				m_DirectingStep = _XDEF_DISARM_DIRECTING_END;
				break;
			}
		}
		break;
	case _XDEF_DISARM_DIRECTING_END :
		{
			D3DXMatrixIdentity(&rotmat);
			g_LodTerrain.m_PathObjectManager->ReleaseCurve(m_PathID);
		}
		break;
	}
	
	m_pMeshContainer->Render_OriginalPos(m_vecPosition, 0, &rotmat);
}

void _XDisarmamentItem::Delete(void)
{
	SAFE_DELETE(m_pMeshContainer);
}

char GetEqualItemType( char cType )
{
	if( cType < 0 || cType >= _XDEF_ITEMTYPE_COUNT )
		return -1;
	
	switch(cType) 
	{
	case _XGI_FC_WEAPON :
	case _XGI_FC_WEAPON2 :
	case _XGI_FC_WEAPON3 :
		return _XGI_FC_WEAPON;
	case _XGI_FC_CLOTHES :
	case _XGI_FC_CLOTHES2 :
	case _XGI_FC_CLOTHES3 :
		return _XGI_FC_CLOTHES;
	default :
		return cType;	
	}
}

void* GetFirstTypeItemPorperty( char cType )
{
	switch(cType) 
	{
	case _XGI_FC_WEAPON :
		return g_WeaponItemProperty;
	case _XGI_FC_WEAPON2 :	
		return g_WeaponItemProperty2;
	case _XGI_FC_WEAPON3 :	
		return g_WeaponItemProperty3;
	case _XGI_FC_CLOTHES :
		return g_ClothesItemProperty;
	case _XGI_FC_CLOTHES2 :	
		return g_ClothesItemProperty2;
	case _XGI_FC_CLOTHES3 :	
		return g_ClothesItemProperty3;
	case _XGI_FC_BOOK :		
		return g_BookItemProperty;
	case _XGI_FC_POTION :		
		return g_PotionItemProperty;
	case _XGI_FC_CONSUMING :	
		return g_ConsumingItemProperty;
	case _XGI_FC_ELIXIR :		
		return g_ElixirItemProperty;
	case _XGI_FC_COUNTERACT :	
		return g_CounteractItemProperty;
	case _XGI_FC_RESOURCE :	
		return g_ResourceItemProperty;
	case _XGI_FC_QUEST :		
		return g_QuestItemProperty;
	case _XGI_FC_ACCESSORY :	
		return g_AccessoryItemProperty;
	case _XGI_FC_LIFE :		
		return g_LifeItemProperty;
	case _XGI_FC_MUSIC :	
		return g_MusicItemProperty;
	case _XGI_FC_PASS :		
		return g_PassItemProperty;	
	case _XGI_FC_SOCKET :		
		return g_SocketItemProperty;
	case _XGI_FC_BOX :		
		return g_BoxItemProperty;
	case _XGI_FC_BOXKEY :		
		return g_BoxKeyItemProperty;
	default:
		return NULL;
	}
}

void PlayItemUseSound( char cType, short sID )
{
	int soundindex = -1;
#ifdef _XTS_ITEM_OPTIMIZATION
	switch(GetEqualItemType(cType)) 
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);
			if( WeaponItem )
			{
				if( WeaponItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ WeaponItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_CLOTHES :
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
			{
				if( ClothesItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ ClothesItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
			{
				if( BookItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ BookItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_POTION :
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
			{
				if( PotionItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ PotionItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ConsumingItem )
			{
				if( ConsumingItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ ConsumingItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
			{
				if( ElixirItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ ElixirItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_COUNTERACT :
		{
			_XGI_CounteractItem_Property* CounteractItem = (_XGI_CounteractItem_Property*)GetFirstTypeItemPorperty(cType);
			if( CounteractItem )
			{
				if( CounteractItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ CounteractItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);
			{
				if( ResourceItem )
				{
					if( ResourceItem[sID].ucQuality2 > 0 )
						soundindex = g_ItemUseSoundIndexTable[ ResourceItem[sID].ucQuality2 ];
				}
			}
		}
		break;
	case _XGI_FC_QUEST :
		{
			_XGI_QuestItem_Property* QuestItem = (_XGI_QuestItem_Property*)GetFirstTypeItemPorperty(cType);
			if( QuestItem )
			{
				if( QuestItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ QuestItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
			{
				if( AccessoryItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ AccessoryItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
			{
				if( LifeItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ LifeItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_MUSIC :
		{
			_XGI_MusicItem_Property* MusicItem = (_XGI_MusicItem_Property*)GetFirstTypeItemPorperty(cType);
			if( MusicItem )
			{
				if( MusicItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ MusicItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_PASS :
		{
			_XGI_PassItem_Property* PassItem = (_XGI_PassItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PassItem )
			{
				if( PassItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ PassItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_MONEY :
		break;
	case _XGI_FC_SOCKET :
		{
			_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
			if( SocketItem )
			{
				if( SocketItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ SocketItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_BOX :
		{
			_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxItem )
			{
				if( BoxItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ BoxItem[sID].ucQuality2 ];
			}
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxkeyItem )
			{
				if( BoxkeyItem[sID].ucQuality2 > 0 )
					soundindex = g_ItemUseSoundIndexTable[ BoxkeyItem[sID].ucQuality2 ];
			}
		}
		break;
	}	
#else
	switch( cType )
	{
	case _XGI_FC_WEAPON		:
		if( g_WeaponItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_WeaponItemProperty[sID].ucQuality2 ];
		break;
	case _XGI_FC_CLOTHES	:
		if( g_ClothesItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_ClothesItemProperty[sID].ucQuality2 ];
		break;	
	case _XGI_FC_WEAPON2		:
		if( g_WeaponItemProperty2[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_WeaponItemProperty2[sID].ucQuality2 ];
		break;
	case _XGI_FC_CLOTHES2	:
		if( g_ClothesItemProperty2[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_ClothesItemProperty2[sID].ucQuality2 ];
		break;
	case _XGI_FC_WEAPON3		:
		if( g_WeaponItemProperty3[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_WeaponItemProperty3[sID].ucQuality2 ];
		break;
	case _XGI_FC_CLOTHES3	:
		if( g_ClothesItemProperty3[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_ClothesItemProperty3[sID].ucQuality2 ];
		break;
	case _XGI_FC_BOOK		:
		if( g_BookItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_BookItemProperty[sID].ucQuality2 ];
		break;
	case _XGI_FC_POTION		:
		if( g_PotionItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_PotionItemProperty[sID].ucQuality2 ];
		break;
	case _XGI_FC_CONSUMING		:
		if( g_ConsumingItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_ConsumingItemProperty[sID].ucQuality2 ];
		break;
	case _XGI_FC_ELIXIR:
		if( g_ElixirItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_ElixirItemProperty[sID].ucQuality2 ];
		break;
	case _XGI_FC_COUNTERACT :
		if( g_CounteractItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_CounteractItemProperty[sID].ucQuality2 ];
		break;
	case _XGI_FC_RESOURCE	:
		if( g_ResourceItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_ResourceItemProperty[sID].ucQuality2 ];
		break;
	case _XGI_FC_QUEST		:
		if( g_QuestItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_QuestItemProperty[sID].ucQuality2 ];
		break;
	case _XGI_FC_ACCESSORY	:
		if( g_AccessoryItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_AccessoryItemProperty[sID].ucQuality2 ];
		break;
	case _XGI_FC_LIFE		:
		if( g_LifeItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_LifeItemProperty[sID].ucQuality2 ];
		break;
	case _XGI_FC_MUSIC		:
		if( g_MusicItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_MusicItemProperty[sID].ucQuality2 ];
		break;
	case _XGI_FC_PASS		:
		if( g_PassItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_PassItemProperty[sID].ucQuality2 ];
		break;
	case _XGI_FC_MONEY		:		
		break;
	case _XGI_FC_SOCKET		:
		if( g_SocketItemProperty[sID].ucQuality2 > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_SocketItemProperty[sID].ucQuality2 ];
		break;
	}
#endif

	if( soundindex > -1 )
	{
		_XPlayInterfaceSound( soundindex );
	}
}

void PlayItemEquipSound( char cType, short sID )
{
	int soundindex = -1;
#ifdef _XTS_ITEM_OPTIMIZATION
	switch(GetEqualItemType(cType)) 
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);
			if( WeaponItem )
			{
				if( WeaponItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ WeaponItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_CLOTHES :
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
			{
				if( ClothesItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ ClothesItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_BOOK :
		{
			_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BookItem )
			{
				if( BookItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ BookItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_POTION :
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
			{
				if( PotionItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ PotionItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_CONSUMING :
		{
			_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ConsumingItem )
			{
				if( ConsumingItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ ConsumingItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
			{
				if( ElixirItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ ElixirItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_COUNTERACT :
		{
			_XGI_CounteractItem_Property* CounteractItem = (_XGI_CounteractItem_Property*)GetFirstTypeItemPorperty(cType);
			if( CounteractItem )
			{
				if( CounteractItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ CounteractItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ResourceItem )
			{
				if( ResourceItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ ResourceItem[sID].ucQuality ];
			}				
		}
		break;
	case _XGI_FC_QUEST :
		{
			_XGI_QuestItem_Property* QuestItem = (_XGI_QuestItem_Property*)GetFirstTypeItemPorperty(cType);
			if( QuestItem )
			{
				if( QuestItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ QuestItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
			{
				if( AccessoryItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ AccessoryItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if( LifeItem )
			{
				if( LifeItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ LifeItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_MUSIC :
		{
			_XGI_MusicItem_Property* MusicItem = (_XGI_MusicItem_Property*)GetFirstTypeItemPorperty(cType);
			if( MusicItem )
			{
				if( MusicItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ MusicItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_PASS :
		{
			_XGI_PassItem_Property* PassItem = (_XGI_PassItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PassItem )
			{
				if( PassItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ PassItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_MONEY :
		break;
	case _XGI_FC_SOCKET :
		{
			_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
			if( SocketItem )
			{
				if( SocketItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ SocketItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_BOX :
		{
			_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxItem )
			{
				if( BoxItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ BoxItem[sID].ucQuality ];
			}
		}
		break;
	case _XGI_FC_BOXKEY :
		{
			_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
			if( BoxkeyItem )
			{
				if( BoxkeyItem[sID].ucQuality > 0 )
					soundindex = g_ItemUseSoundIndexTable[ BoxkeyItem[sID].ucQuality ];
			}
		}
		break;
	}
#else
	switch( cType )
	{
	case _XGI_FC_WEAPON		:
		if( g_WeaponItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_WeaponItemProperty[sID].ucQuality ];		
		break;
	case _XGI_FC_CLOTHES	:
		if( g_ClothesItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_ClothesItemProperty[sID].ucQuality ];
		break;	
	case _XGI_FC_WEAPON2		:
		if( g_WeaponItemProperty2[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_WeaponItemProperty2[sID].ucQuality ];		
		break;
	case _XGI_FC_CLOTHES2	:
		if( g_ClothesItemProperty2[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_ClothesItemProperty2[sID].ucQuality ];
		break;
	case _XGI_FC_WEAPON3		:
		if( g_WeaponItemProperty3[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_WeaponItemProperty3[sID].ucQuality ];		
		break;
	case _XGI_FC_CLOTHES3	:
		if( g_ClothesItemProperty3[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_ClothesItemProperty3[sID].ucQuality ];
		break;
	case _XGI_FC_BOOK		:
		if( g_BookItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_BookItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_POTION		:
		if( g_PotionItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_PotionItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_CONSUMING		:
		if( g_ConsumingItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_ConsumingItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_ELIXIR		:
		if( g_ElixirItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_ElixirItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_COUNTERACT :
		if( g_CounteractItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_CounteractItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_RESOURCE	:
		if( g_ResourceItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_ResourceItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_QUEST		:
		if( g_QuestItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_QuestItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_ACCESSORY	:
		if( g_AccessoryItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_AccessoryItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_LIFE		:
		if( g_LifeItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_LifeItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_MUSIC		:
		if( g_MusicItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_MusicItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_PASS		:
		if( g_PassItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_PassItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_MONEY		:		
		break;
	case _XGI_FC_SOCKET		:
		if( g_SocketItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_SocketItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_BOX :
		if( g_BoxItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_BoxItemProperty[sID].ucQuality ];
		break;
	case _XGI_FC_BOXKEY :
		if( g_BoxKeyItemProperty[sID].ucQuality > 0 )
			soundindex = g_ItemUseSoundIndexTable[ g_BoxKeyItemProperty[sID].ucQuality ];
		break;
	}
#endif

	if( soundindex > -1 )
	{
		_XPlayInterfaceSound( soundindex );
	}
}


int CheckCashItem( char cType, short sID )
{
	if( cType < 0 || cType >= _XDEF_ITEMTYPE_COUNT )
		return 0;
	
	switch(cType) 
	{
	case _XGI_FC_WEAPON :
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);
			if(WeaponItem)
			{
				if(WeaponItem[sID].cCashCheck != 0)
					return WeaponItem[sID].cCashCheck;
			}
		}
		break;
	case _XGI_FC_CLOTHES :
		{
			XGI_ClothesItem_Property* ClothesItem = (XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
			{
				if( ClothesItem[sID].cCashCheck != 0 ) 
					return ClothesItem[sID].cCashCheck;
			}
		}
		break;
	case _XGI_FC_POTION :
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
			{
				if( PotionItem[sID].cCashCheck != 0 )
					return PotionItem[sID].cCashCheck;
			}
		}
		break;
	case _XGI_FC_ELIXIR :
		{
			XGI_ElixirItem_Property* ElixirItem = (XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
			{
				if( ElixirItem[sID].cCashCheck != 0 )
					return ElixirItem[sID].cCashCheck;
			}
		}
		break;
	case _XGI_FC_RESOURCE :
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ResourceItem )
			{
				if( ResourceItem[sID].cCashCheck != 0 )
					return ResourceItem[sID].cCashCheck;
			}
		}
		break;
	case _XGI_FC_ACCESSORY :
		{
			XGI_AccessoryItem_Property* AccessoryItem = (XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
			{
				if( AccessoryItem[sID].cCashCheck != 0 )
					return AccessoryItem[sID].cCashCheck;
			}
		}
		break;
	case _XGI_FC_LIFE :
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if(LifeItem)
			{
				if(LifeItem[sID].cCashCheck != 0)
					return LifeItem[sID].cCashCheck;
			}
		}
		break;
	default:
		break;
	}
	
	return 0;

}



_XCashItemPackage::_XCashItemPackage()
{
	m_indexCashItemPackage	= 0;			// 패키지 인덱스
	m_CashItemType	= 0;
	m_CashItemID	= 0;
	m_indexPackageIcon	= 0;		// 2D 비트맵 아이콘 인덱스
#ifdef _XDEF_EXPANDSTRINGDBCATEGORY_20070321	
	m_PackageName = NULL;	
	m_PackageDiscription = NULL;
//	memset( m_PackageName,0, sizeof(TCHAR)*ITEM_NAME_LENGTH);	// 패키지 이름
//	memset( m_PackageDiscription,0, sizeof(TCHAR)*1024);	// 패키지 설명
#else
	memset( m_PackageName,0, sizeof(TCHAR)*ITEM_NAME_LENGTH);	// 패키지 이름
	memset( m_PackageDiscription,0, sizeof(TCHAR)*1024);	// 패키지 설명
#endif
		
}
_XCashItemPackage::~_XCashItemPackage()
{
}

_XCashItemPackageManager::_XCashItemPackageManager()
{
}
_XCashItemPackageManager::~_XCashItemPackageManager()
{
	_XCashItemPackage* currentcashitempackage = NULL;
	list <_XCashItemPackage*>::iterator iter_cashitempackage;
	for(iter_cashitempackage = m_listCashItemPackage.begin() ; iter_cashitempackage != m_listCashItemPackage.end() ; ++iter_cashitempackage)
	{
		currentcashitempackage = *iter_cashitempackage;
		SAFE_DELETE(currentcashitempackage);
	}
	m_listCashItemPackage.clear();
}
BOOL _XCashItemPackageManager::LoadCashItemPackage(FILE* fileptr)
{
	if( !fileptr ) return FALSE;
	
	TCHAR buffer[1024];
#ifdef _XDEF_EXPANDSTRINGDBCATEGORY_20070321
	TCHAR temp[1024];
	TCHAR temp1[1024];
#endif
	_XCashItemPackage* newcashitempackage = NULL;
	
	do 
	{
		if(feof(fileptr))
			break;
		
		memset(buffer, 0, sizeof(TCHAR)*1024);
		if(!fgets(buffer, 1024, fileptr))
			break;
		
		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			newcashitempackage = new _XCashItemPackage;
			
#ifdef _XDEF_EXPANDSTRINGDBCATEGORY_20070321
			
			memset(temp, 0, sizeof(TCHAR)*1024);
			memset(temp1, 0, sizeof(TCHAR)*1024);
			sscanf( buffer, "%d %s %d %d %d %s", 
				&newcashitempackage->m_indexCashItemPackage,	// 패키지 인덱스
				temp,											// 패키지 이름
				&newcashitempackage->m_CashItemType,
				&newcashitempackage->m_CashItemID,
				&newcashitempackage->m_indexPackageIcon,		// 2D 비트맵 아이콘 인덱스
				temp1											// 패키지 설명
				);
			newcashitempackage->m_PackageName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_CACHITEMNAME, newcashitempackage->m_indexCashItemPackage );
			newcashitempackage->m_PackageDiscription = g_StringDataBase.GetTextString( _XSTRINGDB_SN_CACHITEMDESCRIPTION, newcashitempackage->m_indexCashItemPackage );
			
			int _length = strlen(newcashitempackage->m_PackageName);

			if( g_LanguageType != _XLANGUAGE_TYPE_TAIWANESE && g_LanguageType != _XLANGUAGE_TYPE_JAPANESE ) // 일본과 대만은 띄어쓰기 처리 안함.
			{
				for(int j = 0; j < _length; j++)
				{
					if( newcashitempackage->m_PackageName[j] == '_' )
						newcashitempackage->m_PackageName[j] = ' ';			
				}						  
			}
			_length = strlen(newcashitempackage->m_PackageDiscription);
			for( int j = 0; j < _length; j++)
			{
				if( g_LanguageType != _XLANGUAGE_TYPE_TAIWANESE && g_LanguageType != _XLANGUAGE_TYPE_JAPANESE ) // 일본과 대만은 띄어쓰기 처리 안함.
				{
					if( newcashitempackage->m_PackageDiscription[j] == '_' )
						newcashitempackage->m_PackageDiscription[j] = ' ';		
				}
				
				if( newcashitempackage->m_PackageDiscription[j] == '/' )
				{
					newcashitempackage->m_PackageDiscription[j] = '\n';	
				}		
			}

//			sscanf( buffer, "%d %s %d %d %d %s", 
//				&newcashitempackage->m_indexCashItemPackage,	// 패키지 인덱스
//				newcashitempackage->m_PackageName,				// 패키지 이름
//				&newcashitempackage->m_CashItemType,
//				&newcashitempackage->m_CashItemID,
//				&newcashitempackage->m_indexPackageIcon,		// 2D 비트맵 아이콘 인덱스
//				newcashitempackage->m_PackageDiscription	// 패키지 설명
//				);
//			
//			for(int j = 0; j < ITEM_NAME_LENGTH; j++)
//			{
//				if( newcashitempackage->m_PackageName[j] == '_' )
//					newcashitempackage->m_PackageName[j] = ' ';			
//			}						  
//			for( j = 0; j < 1024; j++)
//			{
//				if( newcashitempackage->m_PackageDiscription[j] == '_' )
//					newcashitempackage->m_PackageDiscription[j] = ' ';		
//				
//				if( newcashitempackage->m_PackageDiscription[j] == '/' )
//				{
//					newcashitempackage->m_PackageDiscription[j] = '\n';	
//				}
//			}
#else

			sscanf( buffer, "%d %s %d %d %d %s", 
				&newcashitempackage->m_indexCashItemPackage,	// 패키지 인덱스
				newcashitempackage->m_PackageName,				// 패키지 이름
				&newcashitempackage->m_CashItemType,
				&newcashitempackage->m_CashItemID,
				&newcashitempackage->m_indexPackageIcon,		// 2D 비트맵 아이콘 인덱스
				newcashitempackage->m_PackageDiscription	// 패키지 설명
				);
			
			if( g_LanguageType != _XLANGUAGE_TYPE_TAIWANESE && g_LanguageType != _XLANGUAGE_TYPE_JAPANESE ) // 일본과 대만은 띄어쓰기 처리 안함.
			{
				for(int j = 0; j < ITEM_NAME_LENGTH; j++)
				{				
					if( newcashitempackage->m_PackageName[j] == '_' )
						newcashitempackage->m_PackageName[j] = ' ';
				}
			}
			for( int j = 0; j < 1024; j++)
			{
				if( g_LanguageType != _XLANGUAGE_TYPE_TAIWANESE && g_LanguageType != _XLANGUAGE_TYPE_JAPANESE ) // 일본과 대만은 띄어쓰기 처리 안함.
				{
					if( newcashitempackage->m_PackageDiscription[j] == '_' )
						newcashitempackage->m_PackageDiscription[j] = ' ';		
				}
				
				if( newcashitempackage->m_PackageDiscription[j] == '/' )
				{
					newcashitempackage->m_PackageDiscription[j] = '\n';	
				}
			}
#endif
			
			m_listCashItemPackage.push_back(newcashitempackage);
		}
	} while(!feof(fileptr));
	
	return TRUE;
}