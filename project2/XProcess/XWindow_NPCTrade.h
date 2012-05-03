// XWindow_NPCTrade.h: interface for the _XWindow_NPCTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCTRADE_H__E74C98E5_B9B4_43A5_9FB9_AADEDC404E7D__INCLUDED_)
#define AFX_XWINDOW_NPCTRADE_H__E74C98E5_B9B4_43A5_9FB9_AADEDC404E7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable: 4786)

#include <list>
#include "XKernel.h"
#include "XNPCScript.h"

using namespace std;

/**
 * \brief NPC 거래창 
 * 
*/

#define _XDEF_MAX_NPCTRADESLOT			(25)

/// 판매목록 탭 - 최대 5개 
typedef enum
{
	_XTRADETAB_ONE = 0,
	_XTRADETAB_TWO,
	_XTRADETAB_THREE,
	_XTRADETAB_FOUR,
	_XTRADETAB_FIVE
}_XNPCTRADETAB;		

typedef enum tagXNPCTRADEITEMEDGEICONTYPE
{
	_XNT_ITEMEDGEICONTYPE_EMPTY = 0,
	_XNT_ITEMEDGEICONTYPE_NORMAL,
	_XNT_ITEMEDGEICONTYPE_DISABLESLOT,
} _XNPCTRADEITEMEDGEICONTYPE;

/// 암상인 아이템 
struct _XGambleItem 
{
	int Type;
	int Id;
	int Price;
	int cluindex;
	int ItemNo;
};

enum
{
	_MODE_NORMAL = 0,		// 돈으로 구입
	_MODE_CLANPOINT = 1,	// 문파 기여도로 구입
	_MODE_MNDPOINT = 2		// 사제 기여도로 구입
};

class _XWindow_NPCTrade : public _XWindow  
{
// add data ->2005.1.5
private:
	_XImageStatic*		m_NPCJobIcon;
	_XImageStatic		m_ItemEdgeIcon[3];
	_XCheckButton*		m_SelectTabButtton[5];	
	BOOL				m_BlackMarketMode;		///< 암상인모드

	int					m_nPriceMode;			///< 돈으로 사는지, 문파기여도로 사는지
	
public :
	int*				m_GambleItemPatten;		///< 겜블아이템 패턴리스트
	_XGambleItem*		m_GambleItemInfo;		///< 겜블 아이템 리스트

	_XNPCTRADETAB		m_SelectedTab;	
	
	int					m_OwnerNPCID;
	int					m_ItemPackageID;
	int					m_AdditionalItemPackageID;
	int					m_SpecialItemPackgeID;
	
	BOOL 				m_bClanMode;					///< 문파판별
	BOOL 				m_bHonorMode1;					///< 명성판별 : 할인율 적용할 때 사용
	BOOL 				m_bHonorMode2;					///< 명성판별 : 조건에 맞는 명성이면 체크 -> 추가 아이템 리스트 
	BOOL 				m_bCustomerMode;				///< 단골판별

	_XMouseSlotItem		m_MouseSlotItem;

	BOOL				m_bHaveMouseSlot;
	BOOL				m_bDragState;
	POINT				m_IconDragStartPos;
	int					m_LastDraggingSlot;

	BOOL				m_bDrawSelectedItem;

	list <_XTradeItem>	m_TradeItemList;		///< 판매 아이템 리스트
	POINT				m_SlotPosition[_XDEF_MAX_NPCTRADESLOT];	///< 판매 아이템 위치

	BOOL				m_bIsBuy;
	int					m_MaxViewCount;
	BOOL				m_bTradeMenu;
	
public:
	_XWindow_NPCTrade();
	virtual ~_XWindow_NPCTrade();
	
	BOOL			Initailize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusedobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	void			MoveWindow( int X, int Y ); 

	void			ChangeTab(_XNPCTRADETAB tab);

	void			SetTradeMode(_XNPCScriptItem* npcsubscript);
	void			SetOwnerNPCID(int npcid)
	{
		m_OwnerNPCID = npcid;
	}
	
	void			SetBlackMarketMode( BOOL mode )		///< 암상인 모드 
	{
		m_BlackMarketMode = mode;
	}
	void			SetItemPackageID(int packageid);
	void			SetAdditionalItemPackageID(int packageid);
	void			SetSpecialItemPackageID(int packageid);

	void			ProcessBuy(void);
	void			ProcessSell(void);

	void			ProcessDblClick(void);

	void			SetDefaultPosition( void );	
	void			ShowWindow( BOOL show );	

	void			DrawTradeItemIcon(void);
	void			DrawSelectedItem(void);
	void			DrawDragIcon(void);
	int				GetSlotIndex(void);
	void			SetListItem(void);

	void			DrawItemTooltip(int slotnumber, RECT iconrect);

	BOOL			GetItemPropertyFromSlot(int selectedslot, char& cType, short& sID, int& itemNo);
	BOOL			GetItemPropertyFromNo(int itemNo, char& cType, short& sID, int& slotnumber);

	void			SetNPCJobIcon(int jobindex);
	BOOL			CheckCondition(char cType, int sId);
	int				PriceCalc(int slotnumber, int count = 1);		///< 가격 계산
	
	void			LoadGambleItemTradePackage(int pattenindex);	///< 겜블 아이템 목록 읽기

	void			SetPriceMode(int mode)	{	m_nPriceMode = mode;	};
	int				GetPriceMode()			{	return m_nPriceMode;	};
};

#endif // !defined(AFX_XWINDOW_NPCTRADE_H__E74C98E5_B9B4_43A5_9FB9_AADEDC404E7D__INCLUDED_)
