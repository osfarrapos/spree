// XWindow_CashItem.h: interface for the _XWindow_CashItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CASHITEM_H__FFED4173_0888_4E02_950A_0E45CB61DB0B__INCLUDED_)
#define AFX_XWINDOW_CASHITEM_H__FFED4173_0888_4E02_950A_0E45CB61DB0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 아이템 몰 창 
 * 자신의 구입한 아이템과 선물로 받은 아이템 목록을 볼 수 있는 창 
 * 
 *
 * 서버에서 자신이 산 아이템과 선물로 받은 아이템 목록을 보내주면 
 * 마우스 드래그나 클릭으로 자신의 인벤으로 아이템을 옮겨서 사용할 수 있음.
*/

/// 자신이 구입한 아이템 구조체
struct _XCASHITEM_BUY 
{
	_XUserItem Item;
	unsigned short usCash;

	void Reset()
	{
		Item.Reset();
		usCash = 0;
	} ;
};

/// 선물로 받은 아이템 구조체
struct _XCASHITEM_PRESENT 
{
	_XUserItem Item;
	
	TCHAR	FromUserName[13];
	TCHAR	FromMessage[41];

	void Reset()
	{
		Item.Reset();

		memset(FromUserName, 0, sizeof(TCHAR)*13);
		memset(FromMessage, 0, sizeof(TCHAR)*41);
	} ;
};


class _XWindow_CashItem : public _XWindow  
{
private:
	_XCheckButton*				m_pBuyItem;
	_XCheckButton*				m_pPresentItem;
	_XVScrollBar*				m_pCashItemListScrollBar;
	int							m_CashItemListScrollPos;

	list<_XCASHITEM_BUY*>		m_BuyItemList;
	list<_XCASHITEM_PRESENT*>	m_PresentItemList;
	int							m_CashItemMode;
	/// 한 화면에 보이는 자신이 산 아이템 위치 ( 최대 7개 )
	POINT						m_BuyItemSlotPosition[7];	
	/// 한 화면에 보이는 선물받은 아이템 위치 ( 최대 5개 )
	POINT						m_PresentItemSlotPosition[5];	

	_XUserItem*					m_SelectedItem;

public:
	_XWindow_CashItem();
	virtual ~_XWindow_CashItem();

	BOOL	Initialize(void);
	void	DestroyWindow(void);
	
	void	Draw(_XGUIObject*& pfocusedobject = g_CurrentFocusedObject);
	BOOL	Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL	ProcessWheelMessage( short zDelta );

	void	DrawBuyItemList(void);
	void	DrawPresentItemList(void);

	void	MoveWindow( int X, int Y );
	void	ShowWindow(BOOL show);
	
	void	SetMode(int mode);
	void	BuyItemReload(void);
	void	PresentItemReload(void);
	void	AllMoveItem(void);

	int		SearchSlotRegion(RECT* pRect = NULL);
	_XUserItem* GetCashItem() { return m_SelectedItem; };

};

#endif // !defined(AFX_XWINDOW_CASHITEM_H__FFED4173_0888_4E02_950A_0E45CB61DB0B__INCLUDED_)
