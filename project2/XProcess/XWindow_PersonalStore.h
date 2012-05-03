// XWindow_PersonalStore.h: interface for the _XWindow_PersonalStore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PERSONALSTORE_H__0F90F35D_0121_4D17_BFBB_1FFD203F0B5E__INCLUDED_)
#define AFX_XWINDOW_PERSONALSTORE_H__0F90F35D_0121_4D17_BFBB_1FFD203F0B5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#define _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER		9

class _XWindow_PersonalStore : public _XWindow  
{		
public:
	_XStatic			m_BorderTile;

	_XButton*			m_btnOpen;
	_XButton*			m_btnCancle;
	_XButton*			m_btnInputOK;
	
	
	_XTradeItem			m_TradeItem[_XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER];
	POINT				m_TradeItemSlotPosition[_XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER];
	DWORD				m_TradeItemPrice[_XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER];
	int					m_TotalCountTradeItem;

	int					m_ServerPersonalStoreID;
	int					m_OwnerUniqueID;
	TCHAR				m_OwnerName[_XDEF_MAX_USERNAMESTRINGLENGTH];
	TCHAR				m_PersonalStoreName[128]; // 18자
	
	int					m_LastDraggingSlot;
	int					m_SelectedSlotNumber;
	
	DWORD				m_PrevPacketSendedTime;

	_XIMEContainer		m_StoreNameIMEControl;				// 상점 이름 입력창
	_XIMEContainer		m_TradeItemPriceIMEControl;			// 거래 물건 가격 입력창
	TCHAR				m_strPrice[64];			
	DWORD				m_Price;							// 입력창에 입력된 가격
	
	FLOAT				m_RotateAngle;
	D3DXMATRIX			m_RHWMatrix;
	_X3PCamera			m_ModelViewCamera;
	D3DVIEWPORT9		m_ModelViewPort;	
	
public:
	_XWindow_PersonalStore();
	virtual ~_XWindow_PersonalStore();
	
	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void				MoveWindow(int X, int Y);
	void				ShowWindow(BOOL show);
	
	BOOL				ProcessPreparingPersonalStore(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				ProcessOpeningPersonalStore(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				ProcessInterestingPersonalStore(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				ProcessModifingPersonalStore(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				ResetTradeInfo(void);
	
	int					SearchFullSlotIndex(void);
	int					SearchEmptySlotIndex(void);
	int					GetNextEmptySlot(void);
	
	void				CopyItemToSlot(int slotnumber, int invennumber);
	
	void				RenderTradeItem3D( void );
	
	BOOL				CheckPersonalStoreNameIMEBoxArea(void);
	BOOL				CheckTradeItemPriceIMEBoxArea(void);
};

#endif // !defined(AFX_XWINDOW_PERSONALSTORE_H__0F90F35D_0121_4D17_BFBB_1FFD203F0B5E__INCLUDED_)
