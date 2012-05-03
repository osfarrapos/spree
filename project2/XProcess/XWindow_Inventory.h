// XWindow_Inventory.h: interface for the _XWindow_Inventory class.
//
// Author		: Iljun, Park / Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_INVENTORY_H__F2E99A16_EC25_4EB8_93A5_947127E98C3C__INCLUDED_)
#define AFX_XWINDOW_INVENTORY_H__F2E99A16_EC25_4EB8_93A5_947127E98C3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XGameItem.h"

/**
 * \brief 인벤토리
 * 
 * 
 * 
 * 
 * 
*/

typedef enum
{
	_XINVENTORYBASETAB_POCKET	= 0,	///< 주머니
	_XINVENTORYBASETAB_BACKPACK,		///< 배낭
	_XINVENTORYBASETAB_BACKPACK2,		///< 배낭2
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER
	_XINVENTORYBASETAB_CASHBACKPACK1,	///< Author : 양희왕 //breif : 캐쉬행낭
	_XINVENTORYBASETAB_CASHBACKPACK2
#else
	_XINVENTORYBASETAB_CHARINFO,		///< 캐릭창	
#endif
}_XInventoryWindowBaseTab;

typedef enum
{
	_XINVENTORYSLOTTAB_1	= 0,
	_XINVENTORYSLOTTAB_2,	
}_XInventoryWindowSlotTab;

/// 장착 에러메세지 코드 
typedef enum
{
	_XEQUIPERROR_TYPE = 0,			///< 타입이 틀림
	_XEQUIPERROR_CLAN,				///< 문파가 틀림
	_XEQUIPERROR_SEX,				///< 성별이 틀림
	_XEQUIPERROR_POCKET,			///< 장착할 아이템의 주머니가 장착한 아이템의 주머니보다 적은 경우
	_XEQUIPERROR_ACCESSORY,			///< 악세서리 공간이 없을 때 
	_XEQUIPERROR_OTHER,				///< 없는 아이템일 때 
	_XEQUIPERROR_NO,				///< 통과
	_XEQUIPERROR_QUESTHOLDITEM,		///< 강호풍운록으로 인해서 아이템이 고정되어있는 경우
	_XEQUIPERROR_LEVEL,				///< 레벨이 모자람
	_XEQUIPERROR_ACCOVERLAPPING,	///< 일반 신물 중복됨
	_XEQUIPERROR_PERIODITEM_ACCOVERLAPPING,	///< 기간제 아이템 중복 불가 아이템이 중복됨
	_XEQUIPERROR_FAMEOVERLAPPING,	///< 명성, 악명 아이템 중복됨
	_XEQUIPERROR_NOOVERLAPPING,		///< 중복장착 불가 아이템
	_XEQUIPERROR_STR,				///< 근력 부족
	_XEQUIPERROR_CONSTITUTION,		///< 건강 부족
	_XEQUIPERROR_ZEN,				///< 진기 부족
	_XEQUIPERROR_INT,				///< 지혜 부족
	_XEQUIPERROR_DEX,				///< 민첩 부족
	_XEQUIPERROR_FAME,				///< 명성 부족
	_XEQUIPERROR_INFAMY,			///< 악명 부족
	_XEQUIPERROR_GRADE,				///< 등급이 다름
	_XEQUIPERROR_CLASS,				///< 직책이 다름
}_XInventoryEquipErrorCode;

typedef enum tagXINVENITEMEDGEICONTYPE
{
	_XITEMEDGEICONTYPE_EMPTY = 0,
	_XITEMEDGEICONTYPE_NORMAL,
	_XITEMEDGEICONTYPE_DISABLESLOT,
	_XITEMEDGEICONTYPE_DISABLEBACKPACK,
	
} _XINVENITEMEDGEICONTYPE;

class _XWindow_NPCTrade;
class _XWindow_PCTrade;
class _XWindow_Warehouse;
class _XWindow_RepairItem;
class _XWindow_EnterBossRoom;
class _XWindow_SocketPlugIn;
class _XWindow_SocketPlugInExp;
class _XWindow_CInfo;
class _XWindow_Contribution;
class _XWindow_Pocket;
class _XWindow_PersonalStore;
class _XWindow_TradeConfirm;
class _XWindow_Gamble;
class _XWindow_CashItem;
class _XWindow_Inchant;
class _XWindow_MixWeapon;
class _XWindow_NPCResource_New;

class _XWindow_Inventory : public _XWindow  
{
	
public :
	_XButton*				m_InvenList;
	_XButton*				m_InvenHat;
	_XButton*				m_InvenEarring;
	_XButton*				m_InvenNecklace;
	_XButton*				m_InvenJacket;
	_XButton*				m_InvenMantle;
	_XButton*				m_InvenRing;
	_XButton*				m_InvenWeapon;
	_XButton*				m_InvenFist;
	_XButton*				m_InvenPants;
	_XButton*				m_InvenSupport;
	_XButton*				m_InvenAmmunition;
	_XButton*				m_InvenFootwear;

	_XButton*				m_btnMoney;
		
	_XCheckButton*			m_BaseTabButton[5]; //Author : 양희왕 //breif : 행낭 확장 
	_XButton*				m_CashItemButton;
	BOOL					m_bIteminCashBackPack[2];				//Author : 양희왕 //breif : 캐시 행낭 아이템 있는지 여부
		
	_XInventoryWindowBaseTab m_SelectedBaseTab;		///< 기본 인벤토리 리스트 탭
	_XInventoryWindowBaseTab m_LastDraggingBaseTab;	///< 아이템을 드래그 했을 당시의 탭 

	// Mouse slot
	int						m_LastDraggingSlot;
	BOOL					m_bHaveMouseSlot;
	_XMouseSlotItem			m_MouseSlotItem;

	// Drag
	BOOL					m_bDragState;
	POINT					m_IconDragStartPos;

	_XWindow_NPCTrade*		m_pNPCTradeWindow;
	_XWindow_PCTrade*		m_pPCTradeWindow;
	_XWindow_Warehouse*		m_pWarehouseWindow;
	_XWindow_RepairItem*	m_pRepairItem_Window; 
	_XWindow_EnterBossRoom*	m_pEnterBossRoom_Window;
	_XWindow_SocketPlugIn*  m_pSocketPlugIn_Window;
	_XWindow_SocketPlugInExp*  m_pSocketPlugInEx_Window;
	_XWindow_Contribution*	m_pContribution_Window;
	_XWindow_Pocket*		m_pPocket_Window;
	_XWindow_PersonalStore* m_pPersonalStore_Window;
	_XWindow_TradeConfirm*	m_pTradeConfirm_Window;
	_XWindow_Gamble*		m_pGamble_Window;
	_XWindow_CashItem*		m_pCashItem_Window;
	_XWindow_Inchant*		m_pInchant_Window;
	_XWindow_MixWeapon*		m_pMixWeapon_Window;
	_XWindow_NPCResource_New* m_pNPCResource_New_Window;

	int						m_iSocketIndexList[5];		///< 소켓 장착창에서 선택한 소켓 아이템 리스트 
	int						m_UsedLifeItemIndex;		///< 소켓 장착창에서 선택한 라이프 아이템
	BOOL					m_bLockInventory;			///< 인벤 잠금(이동 불가)
	BOOL					m_bDrawSelectedItem;	

	_XImageStatic			m_GrabItemImage;			///< 아이템 습득시 효과 이미지
	BOOL					m_bDrawGrabItem;			///< 아이템 습득시 효과 플래그
	LONG					m_SpeedCounter;				///< 아이템 습득시 효과 속도
	FLOAT					m_EffectFrame;				
	int						m_EffectPrevFrame;

	int						m_ElixirItemEffectFrame;			///< 영약 효과 프레임 관련  ---┓
	int						m_ElixirItemEffectFrameCount;
	int						m_ElixirItemEffectFrameChange;		/// ---------------------------┛	

	_XStatic				m_BorderTile;
	_XImageStatic			m_ItemEdgeIcon[4];
	_XImageStatic*			m_ExpandDockingBorder;	
	_XImageStatic*			m_imageCashItemBorder;	
	
	BOOL					m_bExpandDockingBorderFlag;
	list <int>				m_TradeItemList;

	int						m_ContributionItemInvenNumber;
	int						m_ElixirInvenNumber;
	int						m_BookInvenNumber;
	
	int						m_nPriceMode;
	_XImageStatic*			m_pClanPointImage;
	_XImageStatic*			m_pImageWasteBasket; //Author : 양희왕 //breif : 쓰레기통 이미지 변경된것
	BOOL					m_bWasteBasketLock; //Author : 양희왕 //breif : 휴지통 버릴 때,, 이미지 잡고있는다
	
public:
	_XWindow_Inventory();
	virtual ~_XWindow_Inventory();
	
	BOOL					Initialize( void );
	void					DestroyWindow( void );
	
	void					Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL					Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);	
	
	void					MoveWindow(int X, int Y);
	void					ShowWindow(BOOL show);
	
	void					SetDefaultPosition( void );
	void					SaveWindowProperty( void );
			
	void					ChangeBaseTab(int selectedtab);
	void					ChangeSlotTab(_XInventoryWindowSlotTab selectedtab);	

	void					DrawPocketList( void );
	void					DrawBackPackList( void );
	void					DrawPassList( void );
	void					DrawCharEquipList( void );
	
	void					DrawGrabItem(); 
	void					DrawItemTooltip(int slotnumber, RECT iconrect);	

	char					GetSocketListCount();
	char					GetSocketListInsertPos();
	void					ResetSocketIndexList();
	void					ResetSocketIndexList_GambleBox();
	
	// Left click
	BOOL					ProcessNPCTrade(void);
	BOOL					ProcessFromWarehouse(void);
	BOOL					ProcessPCTrade(void);
	BOOL					ProcessItemDrop(void);
	BOOL					ProcessRepairItem(); 
	BOOL					ProcessFromSocketPlugIn(void);
	void					ProcessInvenMove(void);
	BOOL					ProcessGamble(void);
	BOOL					ProcessWasteBasket(void);

	// Right click
	BOOL					ProcessSellToNPC(void);
	BOOL					ProcessToWarehouse(void);
	BOOL					ProcessToPCTrade(void);
	BOOL					ProcessToRepair(void);
	BOOL					ProcessItemEquip(void);
	BOOL					ProcessToSocketPlugIn(void);
	BOOL					ProcessToSocketPlugInEx(void);
	BOOL					ProcessToContribution(void);
	BOOL					ProcessFromPocket(void);
	BOOL					ProcessToPersonalStore(void);
	BOOL					ProcessToGamble(void);
	BOOL					ProcessToEnterBossRoom(void);
	BOOL					ProcessToInchant(void);
	BOOL					ProcessToSmeltItem(void); //Author : 양희왕
	BOOL					ProcessToSocketDetach(void); //Author : 양희왕
	BOOL					ProcessToMixWeapon(void);
	BOOL					ProcessToNPCResourceNew(void);


	int						GetUserItemListEmptyCount();	///< 비어있는 인벤토리 칸 찾기
    int                     GetCashBackpackStartSlotPosition(); // Author : 양희왕 //breif : 캐시행낭 시작 슬롯
	_XInventoryEquipErrorCode	CheckSlotType( int slotindex );		
	int						SearchSlotRegion( RECT* pRect = NULL );
    void                    SetTabButtonPosition(); //Author : 양희왕 //breif : 버튼 위치 조정

	void					DrawIcon( BOOL smallicon, int X, int Y, _XUserItem* pItem = NULL, BOOL bMarkStackable = TRUE , BOOL busingitem = TRUE ); // 2004.06.10->oneway48 insert
	void					DrawIcon_LifeSkill( BOOL smallicon, int X, int Y, _XCollectionResourceItem* pItem = NULL, BOOL bMarkStackable = TRUE , BOOL busingitem = TRUE ); // 2004.06.10->oneway48 insert
	void					DrawIcon_Gamble( BOOL smallicon, int X, int Y, _XUserItem* pItem = NULL, BOOL bMarkStackable = TRUE , BOOL busingitem = TRUE, BOOL checksex = TRUE ); 
	void					DrawDragIcon( void );
	
	void					GetIconRect(char cType, short sID, int& imageindex, RECT& rect, BOOL smallicon);
	char					GetMaxSocketCount(char ctype, short sID);
	void					DrawSelectedItem( void );
	void					DrawSocketItem( void );	
	void					DrawRepairItem(void);
	void					DrawMixWeaponItem(void);
	void					DrawNPCResource_New_Item(void);

	void					ProcessDblClick(void);
	void					OnLButtonUseWarehouse(void);
	void					OnRButtonUseWarehouse(void);

	BOOL					CheckIsRead( int sID );			///< 읽을 수 있는 비급인지 체크 
	BOOL					CheckIsInternalUse( int sID );	///< 복용할 수 있는 영약인지 체크
	BOOL					CheckDropArea();				///< 아이템 버리기 체크
	BOOL					CheckOverlapIndex(int index);	///< 중복된 인덱스가 있는 경우 하나만 그림
	BOOL					CheckItemEquipState(); //Author : 양희왕 //brief : 아이템 교체 가능한 상태인가 체크한다.
	void					SetExpandDockingBorderFlag(BOOL flag);
	void					ResetMouseSlot(void);

	void					SetPriceMode(int mode);
	int						GetPriceMode()			{	return m_nPriceMode;		};

	void SetNPCTradeWindowPtr(_XWindow_NPCTrade* windowptr)		{	m_pNPCTradeWindow = windowptr;	}
	void SetPCTradeWindowPtr (_XWindow_PCTrade* windowptr)		{	m_pPCTradeWindow = windowptr;	}
	void SetWarehouseWindowPtr(_XWindow_Warehouse* windowptr)	{	m_pWarehouseWindow = windowptr;	}
	void SetRepairItemWindowPtr(_XWindow_RepairItem* windowptr)	{	m_pRepairItem_Window = windowptr;	}
	void SetEnterBossRoomWindowPtr(_XWindow_EnterBossRoom* windowptr)	{	m_pEnterBossRoom_Window = windowptr;	}
	void SetSocketPlugInWindowPtr( _XWindow_SocketPlugIn* windowptr) { m_pSocketPlugIn_Window = windowptr; }	
	void SetSocketPlugInExpWindowPtr( _XWindow_SocketPlugInExp* windowptr) { m_pSocketPlugInEx_Window = windowptr; }
	void SetContributionWindowPtr( _XWindow_Contribution* windowptr) { m_pContribution_Window = windowptr; }
	void SetPocketWindowPtr( _XWindow_Pocket* windowptr) { m_pPocket_Window = windowptr;	};
	void SetPersonalStoreWindowPtr( _XWindow_PersonalStore* windowptr) { m_pPersonalStore_Window = windowptr;	};
	void SetTradeConfirmWindowPtr( _XWindow_TradeConfirm* windowptr) { m_pTradeConfirm_Window = windowptr; 	};
	void SetGambleWindowPtr( _XWindow_Gamble* windowptr) { m_pGamble_Window = windowptr;	};
	void SetCashItemWindowPtr( _XWindow_CashItem* windowptr ) { m_pCashItem_Window = windowptr;	};
	void SetInchantWindowPtr( _XWindow_Inchant* windowptr ) { m_pInchant_Window = windowptr; };
	void SetMixWeaponWindowPtr(_XWindow_MixWeapon* windowptr)	{ m_pMixWeapon_Window = windowptr; };
	void SetNPCResource_New_WindowPtr(_XWindow_NPCResource_New* windowptr)	{m_pNPCResource_New_Window = windowptr; };
};

#endif // !defined(AFX_XWINDOW_INVENTORY_H__F2E99A16_EC25_4EB8_93A5_947127E98C3C__INCLUDED_)



