// XWindow_TradeConfirm.h: interface for the _XWindow_TradeConfirm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TRADECONFIRM_H__8CD96B77_63C7_4E40_9666_B18620B1771D__INCLUDED_)
#define AFX_XWINDOW_TRADECONFIRM_H__8CD96B77_63C7_4E40_9666_B18620B1771D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 수량 입력 창 
 * 해당 모드에 따른 수량 입력
 * 
*/

/// 수량 입력 모드 
typedef enum _XTrade_Mode
{
	_XTRADE_NONE = 0,
	_XDROP_MONEY,						///< 돈 버리기 
	_XWAREHOUSE_TOWAREHOUSE_MONEY,		///< 인벤 -> 창고
	_XWAREHOUSE_FROMWAREHOUSE_MONEY,	///< 창고 -> 인벤
	_XPCTRADE_TOTRADE_MONEY,			///< 인벤 -> 거래창
	_XPCTRADE_FROMTRADE_MONEY,			///< 거래 -> 인벤
	_XNPCTRADE_BUY,						///< 상인 -> 인벤
	_XPCTRADE_FROMTRADE_ITEM,			///< 거래 -> 인벤 
	_XWAREHOUSE_FROMWAREHOUSE_ITEM,		///< 창고 -> 인벤
	_XCONTRIBUTION_FROMCONTRI_ITEM,		
	_XNPCTRADE_SELL = 11,				///< 인벤 -> 상인 
	_XPCTRADE_TOTRADE_ITEM,				///< 인벤 -> 거래 
	_XPERSONALSTORE_TOTRADE_ITEM,		///< 인벤 -> 개인 상점
	_XWAREHOUSE_TOWAREHOUSE_ITEM,		///< 인벤 -> 창고 
	_XCONTRIBUTION_TOCONTRI_ITEM,		
	_XDROP_ITEM,						///< 아이템 버리기

	_XCONTRIBUTION_TOCONTRIBUTION_MONEY,	///< 인벤 -> 기여
	_XCONTRIBUTION_TOCONTRIBUTION_ITEM,		///< 인벤 -> 기여
	
	_XDROP_WASTEBASKET_ITEM,			///< 휴지통 버리기 - 머지 아이템
	
}_XTRADE_MODE;

class _XWindow_TradeConfirm : public _XWindow  
{
public :
	_XButton*		m_AllButton;// 2004.05.17->oneway48 insert
	
	_XButton*		m_OKButton;
	_XButton*		m_CancelButton;
	
	_XIMEContainer	m_IMEControl;		///< 입력창
	
	DWORD			m_Price;			///< 가격
	DWORD			m_TradeCount;		///< 수량
	
	char			m_SelectedCategory1;
	short			m_SelectedItemID;
	int				m_InventoryNumber;
	
	TCHAR			m_CountString[64];
	
	_XTRADE_MODE	m_TradeMode;
	
	/// PC간 거래에서 사용
	int				m_PCTradeSlotIndex;
	/// 개인상점에서 사용
	int				m_PersonalStoreSlotIndex;
	
public:
	_XWindow_TradeConfirm();
	virtual ~_XWindow_TradeConfirm();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	BOOL			CheckIMEBoxArea(void);
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	
	void			GenerateItemPrice(void);	///< 가격 결정
	int				PriceCalc(); /// 판매 가격 결정 Author : 양희왕
	
	void			Reset(void);
};

#endif // !defined(AFX_XWINDOW_TRADECONFIRM_H__8CD96B77_63C7_4E40_9666_B18620B1771D__INCLUDED_)
