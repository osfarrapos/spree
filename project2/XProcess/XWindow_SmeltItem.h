// XWindow_SmeltItem.h: interface for the XWindow_SmeltItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_SMELTITEM_H__29EA878E_1C7F_4683_B4AD_5B9C7817193B__INCLUDED_)
#define AFX_XWINDOW_SMELTITEM_H__29EA878E_1C7F_4683_B4AD_5B9C7817193B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"
#include "XWindow_ChargeGauge.h"

#include <list>
using namespace std;

//Author : 양희왕
//breif : 용린 용루 출력 창
//함수 포인터로 바꾸자
enum ItemFlag 
{
	FLAG_NONE = 0,
	TEAR, 
	SCALES 
};

enum ItemStep
{
	ITEMSTEP_NONE = 0,
	ITEMSTEP_READY,
	ITEMSTEP_CONFIRM,
	ITEMSTEP_PROGRESS
};

class XWindow_SmeltItem : public _XWindow  
{
public:
	XWindow_SmeltItem();
	virtual ~XWindow_SmeltItem();

	BOOL	Initialize( void );
	void	DestroyWindow( void );
	
	void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void	ShowWindow(BOOL show);
	void	MoveWindow( int X, int Y );

	void	SetItemFlag( const ItemFlag flag, const ItemStep step ); //아이템 종류에 따른 처리
	void	MessagePrint(int msgindex);
	int	CheckWeaponGrade(int searchslot);

	int	SearchSlotSmeltItem();
	
	BOOL	ProcessSmeltCashItem(int slotindex);
	BOOL	ProcessSmelt();

	inline void SetUserItem( _XUserItem& Item ) { m_pUserItem = &Item; }
	
	inline void SetUserItemindex( int nIndex ) { m_nUserIndex = nIndex; }
	inline void	SetSmeltItemindex( int nIndex ) 
	{ 
		m_nSmeltIndex = nIndex;  
		m_ltSlotNumber.push_back(nIndex); 
	}
	inline void SetCount(int nCount ) { m_nCount = nCount; }

	inline void SetSmeltNextIndex( int nIndex ) { m_nSmeltNextIndex = nIndex; }
	
	inline const int GetUserItemindex() const { return m_nUserIndex; }
	inline const int GetSmeltItemindex() const { return m_nSmeltIndex; }
	inline const int GetSmeltNextIndex() const { return m_nSmeltNextIndex; }

	inline const int GetCount() const { return m_nCount; }
	
	inline const _XUserItem* GetUserItem() const { return m_pUserItem; }
	inline const ItemFlag& GetItemFlag() const { return m_ItemFlag; }
	inline const ItemStep& GetItemStep() const { return m_ItemStep; }

private:
	void	Draw_Tear();
	void	Draw_Scales();

	void	Process_Tear();
	void	Process_Scales();

	void	RebuildItemInfo_Tear();
	void	RebuildItemInfo_Scales();

	void	SetItemStep_Tear();
	void	SetItemStep_Scales();

	void	DrawItem();
	void	DrawInterface( ItemStep Step );
	
	int	GetItemModelIndex();

	BOOL	CheckIMEBoxArea();

	void	GetMaxScaleCount( int& nMaxCount );

private:
	ItemFlag	m_ItemFlag;
	ItemStep	m_ItemStep;

	_XButton*	m_pConfirmButton;
	_XButton*	m_pCancleButton;

	_XImageStatic* m_pSocketItemLeftView;
	_XImageStatic* m_pSocketItemRightView;

	_XImageStatic*	m_slotImage;

	_XImageStatic* m_pLeftTopView;
	_XImageStatic* m_pRightTopView;
	_XImageStatic* m_pLeftDownView;
	_XImageStatic* m_pRightDownView;

	_XWindow_ChargeGauge*	m_pChargeGaugeWindow;

	FLOAT	m_fItemtRotateAngle;
	D3DXMATRIX	m_ItemRHWMatrix;
	_X3PCamera	m_ItemModelViewCamera;
	D3DVIEWPORT9	m_ItemModelViewPort;
	TCHAR	m_strInfoText[128];

	_XUserItem*	m_pUserItem;
	_XUserItem* m_pSmletItem;
	/// [0] 용루 용린 위치...
	/// [1] 은 실제 등록한 아이템
	_XUserItem	m_UserStuff[2];
	_XIMEContainer	m_IMEControl;

	int m_nCount;	//바르는 횟수
	int m_nUserIndex; //무기의 위치
	int m_nSmeltIndex; //용루 / 용린의 위치
	int m_nSmeltNextIndex; //용루 / 용린의 위치
	unsigned char	m_ucResult;

	list<int> m_ltSlotNumber; //용린 사용시 슬롯 넘버 저장
};

#endif // !defined(AFX_XWINDOW_SMELTITEM_H__29EA878E_1C7F_4683_B4AD_5B9C7817193B__INCLUDED_)
