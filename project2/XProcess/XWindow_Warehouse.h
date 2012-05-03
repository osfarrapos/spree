// XWindow_Warehouse.h: interface for the _XWindow_Warehouse class.
//
// Author : Hangil, Yang
// Modify : Sohyun, Park
// Desc : User warehouse, Warehouse window class
// 
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_WAREHOUSE_H_
#define _XWINDOW_WAREHOUSE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable: 4786)

#include <list>
#include "XNetwork.h"
#include "XWindow_Inventory.h"
#include "XUser.h"
#include "XLocalUser.h"

using namespace std;

/**
 * \brief 창고 
 * 물품 맡기기, 꺼내기, 해제
 * 
*/

/// 한번에 이동할 수 있는 최대 갯수
#define _XDEF_WAREHOUSE_MAX_SENDCOUNT			20	

/// 이동 모드
typedef enum
{
	_XSENDMODE_NONE = 0,
	_XSENDMODE_TOWAREHOUSE_ITEM,	///< 인벤 -> 창고
	_XSENDMODE_TOINVEN_ITEM,		///< 창고 -> 인벤
	_XSENDMODE_TOWAREHOUSE_MONEY,	///< 인벤 -> 창고
	_XSENDMODE_TOINVEN_MONEY		///< 창고 -> 인벤
}_XSendMode;

class _XWindow_Warehouse : public _XWindow  
{
private:
	_XImageStatic		m_ItemEdgeIcon[3];
	int					m_SelectedTab;
public :
	_XCheckButton*		m_SelectTabButtton[3];	///< 최대 40개 이므로 //Author : 양희왕 //breif : 60개 확장

	_XButton*			m_MoveButton;
	_XButton*			m_SelectAllButton;
	_XButton*			m_CancelButton;
	_XButton*			m_StorageMoneyButton;
	_XButton*			m_InventoryMoneyButton;	

	BOOL				m_bWarehouseFirstLoad;
	BOOL				m_bWarehouseFirstView; ///< 2004.06.01->oneway48 insert : 창고 사용시 만료일을 한번만 나타나도록 한다.
	int					m_LastDraggingSlot;

	list <_XWarehouseItem*>		m_TempItemList;		///< 다중 선택된 아이템들
	POINT				m_SlotPosition[_XDEF_WAREHOUSE_MAX_SENDCOUNT];
	DWORD				m_TempMoney;

	_XSendMode			m_SendMode;
	BOOL				m_bWaitForServerResult;

	_XMouseSlotItem		m_MouseSlotItem;
	BOOL				m_bHaveMouseSlot;
	POINT				m_IconDragStartPos;
	BOOL				m_bDragState;

	BOOL				m_bProcessOneClickItem;
	BOOL				m_bDrawSelectedItem;
	
	int					m_UseSlotCount;		///<2004.05.20->oneway48 insert : 사용 가능한 칸수
	BOOL				m_bUseableWarehouse; ///< 2004.06.01->oneway48 insert : 창고를 사용할 수 있는지 없는지 체크
	BOOL				m_bWarehouseCancleMode; ///< 2004.06.01->oneway48 insert : 창고사용 모드인지 창고해제모드인지 구별
	BOOL				m_bTerminateWarehouse; ///< 2004.06.02->oneway48 insert : 기간이 만료되었을 때 
	int					m_TerminateLastItemIndex;	///< 창고 만기 시 아이템이 있는 마지막 자리 이것으로 창고 계약시 칸수를 정함.

	/// 2004.05.24->oneway48 insert : 창고 사용기간 체크 
	int					m_TerminativeYear   ;
	int					m_TerminativeMonth ;
	int					m_TerminativeDay   ;
	int					m_TerminativeHour  ;  
	
	DWORD				m_TradeDelayTime;	///< 지연시간 이 시간이 지나면 서버 결과를 False 로 바꿈
	
	DWORD				m_dwStorageInfoReqTime;		// 창고 정보를 요청한 시간
	
public:
	_XWindow_Warehouse(); 
	virtual ~_XWindow_Warehouse();

	BOOL				Initialize(void);
	void				DestroyWindow(void);

	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void				DrawSelectedItem(void);
	void				DrawDragIcon(void);
	void				DrawItemTooltip(int slotnumber, RECT iconrect);

	void				ShowWindow(BOOL show);

	int					HaveItemFromInven(int invenindex);
	int					HaveItemFromWarehouse(int slotnumber);

	void				SetTempItemFromInven(int invenindex, int count = 0);
	void				ResetTempItemFromInven(int invenindex);

	void				SetTempItemFromWarehouse(int slotindex, int count = 0);
	void				ResetTempItemFromWarehouse(int slotindex);

	void				ReleaseTempItemList(void);

	void				ItemSaveResultToWarehouse(int resultcount);
	void				ItemSaveResultToInventory(int resultcount);

	int					GetSlotIndex(void);	
	void				ChangeTab( int tab );

	void				ProcessDblClick(void);
	void				SetTerminateLastItemIndex();
};

#endif
