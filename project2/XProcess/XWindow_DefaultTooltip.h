// XWindow_DefaultTooltip.h: interface for the _XWindow_DefaultTooltip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_DEFAULTTOOLTIP_H__A2939C6B_1C77_4645_9BAB_7E543B4B6BE2__INCLUDED_)
#define AFX_XWINDOW_DEFAULTTOOLTIP_H__A2939C6B_1C77_4645_9BAB_7E543B4B6BE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XKernel.h"

/**
 * \brief 툴팁창 
 * 클릭시 뜨는 모든 툴팁
 *
 * 개선의 요지가 있음. 
 * 확장성이 너무 떨어짐(인터페이스 구성 관련 기획팀과 상의가 필요)
 *
 * 일반 정보와 확장 정보(소켓아이템이 장착된 아이템들- 무기, 의상) 툴팁이 존재
 * 정보에 따른 사이즈 조절( 가로는 불변)
*/

/** 하나의 툴팁에 필요한 정보들
 *	하나의 툴팁에 필요한 정보가 늘어 날 경우 확장 해야 함 		
*/
 
typedef struct _tag_ItemInfo
{
	// New -----------------=
	TCHAR NickBuf[256];
	TCHAR NameBuf[256];

	TCHAR Temp_Data_Buf[256];
	
	TCHAR DescBuf[256];
	TCHAR LData_Buf[512];
	TCHAR LData_Buf2[512];
	
	TCHAR Dur_Data_Buf[256];
	TCHAR Strength_Data_Buf[256]; //Author: 양희왕
	TCHAR Data_Buf[16][256];
	TCHAR S_Data_Buf[5][256];
	TCHAR SetItemSkill_Data_Buf[10][256]; //Author : 양희왕 //breif : 셋트 아이템 툴팁
	TCHAR SetItem_Data_Buf[7][256]; //Author : 양희왕 //breif : 셋트 아이템 툴팁
	TCHAR E_Data_Buf[10][256];	

	int Data_Value1;
	int Data_Value2;
	int Data_Value3;
	int Data_Value4;
	
} _ITEMTOOLTIP_INFO;


/// 툴팁 타입
typedef enum 
{
	TMODE_NONE=0,					
	TMODE_DEFAULTITEM,				///< 기본툴팁 , 인벤
	TMODE_CINFOITEM,				///< 인물정보창의 장착된 아이템 
	TMODE_NPCTRADE_INVENITEM,		///< npc거래시 인벤 아이템
	TMODE_NPCTRADE_NPCITEM,			///< npc거래서 판매 아이템
	TMODE_WAREHOUSE_WAREHOUSEITEM,	///< 창고 아이템
	TMODE_PCTRADE_TARGETITEM,		///< pc 거래시 상대방 아이템
	TMODE_PCTRADE_SELFITEM,			///< pc 거래시 자신의 아이템
	TMODE_QUEST,					///< 자신의 quest창 아이템
	TMODE_NPCQUEST,					///< npc 모드의 퀘스트 아이템
	TMODE_PERSONALSTORE_TRADEITEM,	///< 노점 아이템
	TMODE_REPAIRITEM,				///< 수리 아이템
	TMODE_GAMBLE,					///< 암상인 판매 아이템
	TMODE_TARGETINFO,				///< 상대방 인물정보창 아이템
	TMODE_CASHITEM,					///< 캐쉬 아이템
	TMODE_INCHANT,					///< 제련창 아이템 //용루용린창 아이템 추가Author 양희왕 
	TMODE_LIFESKILLCOLLECTIONITEM,	///< 수집 아이템
	TMODE_LIFESKILLRESOURCEITEM,	///< 재료 아이템
	TMODE_NPCCOLLECTIONITEM,		///< npc수집 아이템
	TMODE_NPCRESOURCEITEM,			///< npc재료 아이템
	TMODE_SKILL,					///< 스킬
	TMODE_TRADESKILL,				///< 무공전수창 스킬
	TMODE_MASTERYSKILL				///< 오의
}_TOOLTIP_MODE;

class _XWindow_MasterySkill;
class _XWindow_PersonalStore;
class _XWindow_TargetInfo;
class _XWindow_NPCCollection;
class _XWindow_NPCResource;
class _XWindow_NPCResource_New;

class _XWindow_DefaultTooltip : public _XWindow  
{
private:	
	_TOOLTIP_MODE		m_CurrentTooltipMode;
	SIZE				m_TooltipSize;				///< 기본 아이템
	SIZE				m_ExpandTooltipSize;		///< 소켓이 장착된 아이템일 경우(무기,의상)
	SIZE				m_SetItemExpandTooltipSize; // Author : 양희왕 //breif : 세트아이템 사이즈
	int					m_CurrentDrawPosY;

	_ITEMTOOLTIP_INFO	m_CurrentTooltipInfo;

	/**
	 * m_CurrentSlotNumber, m_ItemType, m_ItemId 각각 tooltipmode에 맞는 변수들만 사용 
	*/
	int					m_CurrentSlotNumber;
	int					m_ItemType;
	int					m_ItemId;
	int					m_CurrentItemSockekRealCount;	///< 장착된 소켓 아이템 갯수
	int					m_CurrentItemSocketCount;		///< 장착할 수 있는 소켓 갯수
	int					m_DeleteSkillId;				///< 삭제할 스킬 아이디
	
	int					m_nPriceMode;					// 금전=0 or 문파기여도=1

	_XUserItem			m_AddSocketItemList[4];			///< 소켓 아이템 정보

	_XImageStatic*		m_TooltipEdgeImage[4];			
	_XImageStatic*		m_TooltipIconBoxImage[2];		

	_XImageStatic*		m_MoneyImage;
	_XImageStatic*		m_ContributionImage;
	_XStickGauge*		m_SkillExp;
	_XButton*			m_DeleteSkillBtn;

	_XImageStatic*		m_pClanPointImage;

	_XWindow_NPCTrade*		m_pNPCTradeWindow;
	_XWindow_PCTrade*		m_pPCTradeWindow;
	_XWindow_Warehouse*		m_pWarehouseWindow;
	_XWindow_RepairItem*	m_pRepairItem_Window; 
	_XWindow_EnterBossRoom*	m_pEnterBossRoom_Window;
	_XWindow_SocketPlugIn*  m_pSocketPlugIn_Window;
	_XWindow_NPCScript*		m_pNPCScript_Window;
	_XWindow_PersonalStore* m_pPersonalStore_Window;
	_XWindow_Contribution*	m_pContribution_Window;
	_XWindow_TargetInfo*	m_pTargetInfo_Window;
	_XWindow_CashItem*		m_pCashItem_Window;
	_XWindow_NPCCollection*	m_pNPCCollection_Window;
	_XWindow_NPCResource*	m_pNPCResource_Window;
	_XWindow_Inchant* m_pInchant_Window;
	_XWindow_NPCResource_New* m_pNPCResource_New_Window;
	
private:
	void	ResetTooltipInfo();	

#ifdef _XTS_ITEM_OPTIMIZATION
	void	SetInfoWeaponItem();
	void	SetInfoClothesItem();
#endif
	void	SetInfoWeapon();
	void	SetInfoClothes();
	void	SetInfoWeapon2();
	void	SetInfoClothes2();
	void	SetInfoWeapon3();
	void	SetInfoClothes3();
	void	SetInfoBook();
	void	SetInfoPotion();
	void	SetInfoConsuming();
	void	SetElixir();
	void	SetInfoResource();
	void	SetInfoQuest();
	void	SetInfoAccessory();
	void	SetInfoLife();
	void	SetInfoSocket();
	void	SetInfoBox();
	void	SetInfoBoxKey();

#ifdef _XTS_ITEM_OPTIMIZATION
	void	DrawTextWeaponItem();
	void	DrawTextClothesItem();
#endif
	void	DrawTextWeapon();	
	void	DrawTextClothes();	
	void	DrawTextWeapon2();
	void	DrawTextClothes2();
	void	DrawTextWeapon3();
	void	DrawTextClothes3();
	void	DrawTextBook();
	void	DrawTextPotion();
	void	DrawTextConsuming();
	void	DrawElixir();
	void	DrawTextResource();
	void	DrawTextQuest();
	void	DrawTextAccessory();
	void	DrawTextLife();
	void	DrawTextSocket();
	void	DrawTextBox();
	void	DrawTextBoxKey();
	unsigned char GetInchantInfo();

	unsigned short GetSmeltItem_ScaleInfo(); //Author : 양희왕
	unsigned short GetSmeltItem_MaxScaleInfo(); //Author : 양희왕
	unsigned short GetSmeltItem_TearInfo(); //Author : 양희왕
public:
	_XWindow_DefaultTooltip();
	virtual ~_XWindow_DefaultTooltip();

	BOOL	Initialize(void);
	void	DestroyWindow(void);
	
	void	Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL	Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	/// 맨처음 셋팅하는 함수로 모드에 맞는 일반 최소 크기와 확장 크기 결정 
	BOOL	SetTooltipMode( _TOOLTIP_MODE mode, int slotnumber, int type, int id, int nPriceMode = 0);

	/// 해당 모드의 일반 크기 결정
	BOOL	SetTooltipInfo();
	void	ShowWindow(BOOL show);
	void	MoveWindow(int X, int Y);		
	
	/// 모드에 따른 그리기 함수들
	void	DrawItemTooltip();
	void	DrawTooltipLine( int left, int top, int right, int bottom );
	void	DrawItemDefault();	
	void	DrawItemPrice( int nDrawY ); //Author : 양희왕 //breif : 라인 바뀌는 수가 제각각이어서 함수안에서 처리할수 없음
	void	DrawItemClanPoint( int nDrawY ); //Author : 양희왕 //breif : 라인 바뀌는 수가 제각각이어서 함수안에서 처리할수 없음
	void	DrawDurability();
	void	DrawSkillTooltip();
	void	DrawMasterySkillTooltip();
	void	DrawGambleItemTooltip();

	int		GetInnerLevel(int innerlevel);		/// 성(1~12) 계산
	int		GetDeleteSkillID() { return m_DeleteSkillId; };
	void	SetDeleteSkillId() { m_DeleteSkillId = m_ItemId; };
 
	void SetNPCTradeWindowPtr(_XWindow_NPCTrade* windowptr)		{	m_pNPCTradeWindow = windowptr;	}
	void SetPCTradeWindowPtr (_XWindow_PCTrade* windowptr)		{	m_pPCTradeWindow = windowptr;	}
	void SetWarehouseWindowPtr(_XWindow_Warehouse* windowptr)	{	m_pWarehouseWindow = windowptr;	}
	void SetRepairItemWindowPtr(_XWindow_RepairItem* windowptr)	{	m_pRepairItem_Window = windowptr;	}
	void SetEnterBossRoomWindowPtr(_XWindow_EnterBossRoom* windowptr)	{	m_pEnterBossRoom_Window = windowptr;	}
	void SetSocketPlugInWindowPtr( _XWindow_SocketPlugIn* windowptr) { m_pSocketPlugIn_Window = windowptr; }	
	void SetNPCScriptWindowPtr( _XWindow_NPCScript* windowptr) { m_pNPCScript_Window = windowptr; }		
	void SetPersonalStoreWindowPtr( _XWindow_PersonalStore* windowptr) { m_pPersonalStore_Window = windowptr; }		
	void SetContributionWinddowPtr( _XWindow_Contribution* windowptr) { m_pContribution_Window = windowptr;	}
	void SetTargetInfoWindowPtr( _XWindow_TargetInfo* windowptr) { m_pTargetInfo_Window = windowptr; }
	void SetCashItemWindowPtr( _XWindow_CashItem* windowptr ) { m_pCashItem_Window = windowptr; 	}
	void SetNPCCollectionWindowPtr( _XWindow_NPCCollection* windowptr) { m_pNPCCollection_Window = windowptr; }
	void SetNPCResourceWindowPtr( _XWindow_NPCResource* windowptr ) { m_pNPCResource_Window = windowptr; 	}
	void SetInchantWindowPtr( _XWindow_Inchant* windowptr ) { m_pInchant_Window = windowptr; 	}
	void SetNewNPCResourceWindowPtr(_XWindow_NPCResource_New* windowptr)	{m_pNPCResource_New_Window = windowptr;	}
};

#endif // !defined(AFX_XWINDOW_DEFAULTTOOLTIP_H__A2939C6B_1C77_4645_9BAB_7E543B4B6BE2__INCLUDED_)
