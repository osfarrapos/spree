// XWindow_PCTrade.h: interface for the _XWindow_PCTrade class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_PCTRADE_H_
#define _XWINDOW_PCTRADE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief PC 거래창 
 * 
 * 상대방의 m_TargetConfirmIcon 이미지가 뜬 상태에서 자신의 ok 버튼을 누르면 거래 성립
 * 자신의 m_ConfirmIcon 이미지가 뜬 상태에서 상대방이 아이템을 추가하거나 내리거나 하면
 * 자신의 상태는 m_WaitIcon 로 변함( 상대방도 마찬가지)
*/

#define _XDEF_PCTRADE_MAX_SLOTNUMBER		10		///< 거래할 수 있는 최대 아이템 갯수 


class _XWindow_PCTrade : public _XWindow  
{
public :
	_XImageStatic*			m_TargetWaitIcon;		///< 상대방 대기 이미지
	_XImageStatic*			m_TargetConfirmIcon;	///< 상대방 완료 이미지
	
	_XImageStatic*			m_WaitIcon;				///< 자신의 대기 이미지
	_XImageStatic*			m_ConfirmIcon;			///< 자신의 완료 이미지

	_XButton*				m_OKButton;
	_XButton*				m_CancelButton;
	
	_XButton*				m_TradeMoneyButton;			///< 거래창의 금액 버튼
	_XButton*				m_InventoryMoneyButton;		///< 인벤의 금액 버튼
	
	BOOL					m_bRequest;			///< 요청 중인지
	unsigned short			m_TargetID;
	TCHAR					m_TargetName[_XDEF_MAX_USERNAMESTRINGLENGTH];
	WORD					m_TradeServerSlotNo;	///< 서버와 일치해야 거래 성립
	
	/// Target 정보
	_XTradeItem				m_TargetTradeItem[_XDEF_PCTRADE_MAX_SLOTNUMBER];
	DWORD					m_TargetMoney;
	
	/// 내 정보
	_XTradeItem				m_SelfTradeItem[_XDEF_PCTRADE_MAX_SLOTNUMBER];
	DWORD					m_SelfMoney;
	
	POINT					m_TargetSlotPosition[_XDEF_PCTRADE_MAX_SLOTNUMBER];
	POINT					m_SelfSlotPosition[_XDEF_PCTRADE_MAX_SLOTNUMBER];
	
	BOOL					m_bWaitTrade;
	
	_XMouseSlotItem			m_MouseSlotItem;
	BOOL					m_bHaveMouseSlot;
	BOOL					m_bDragState;
	POINT					m_IconDragStartPos;
	
	int						m_LastDraggingSlot;
	BOOL					m_bIsTarget;

	BOOL					m_bDrawSelectedItem;
	BOOL					m_bTrading;
	
public:
	_XWindow_PCTrade();
	virtual ~_XWindow_PCTrade();
	
	BOOL					Initialize( void );
	void					DestroyWindow( void );
	
	void					ResetTradeInfo(void);
	
	void					Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL					Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void					DrawDragIcon(void);
	void					DrawSelectedItem(void);
	void					DrawItemTooltip(int slotnumber, RECT iconrect);
	
	void					SaveWindowProperty( void );
	void					ShowWindow(BOOL show);
	
	int						SearchFullSlotIndex(void);
	int						SearchEmptySlotIndex(void);
	int						GetNextEmptySlot(void);
	
	void					RestoreTrade(void);
	void					ProcessDblClick(void);

	void					CopyItemToSlot(int slotnumber, int invennumber);	
	
	void					SetDefaultPosition( void );
	void					ResetTradeSetting( void );
};

#endif // !defined(_XWINDOW_PCTRADE_H_)
