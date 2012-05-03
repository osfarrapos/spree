// XWindow_RepairItem.h: interface for the _XWindow_RepairItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_REPAIRITEM_H__D790CEA8_22F4_4CD4_BE98_19D07BC27CAA__INCLUDED_)
#define AFX_XWINDOW_REPAIRITEM_H__D790CEA8_22F4_4CD4_BE98_19D07BC27CAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XLocalUser.h"
#include <list>
using namespace std;

/**
 * \brief 수리 창
 * 
 * 전체수리 클릭시 인벤을 검사해서 내구력이 떨어진 아이템 최대 16개를 자동으로 
 * 수리창으로 옮긴다.
 * 수리할 아이템 갯수에 따라 게이지 딜레이 타임이 늘어난다.
*/

/// 한 번에 수리할 수 있는 최대 아이템 갯수
#define _XDEF_REPAIR_MAX_SENDCOUNT (16)

typedef enum 
{
	RM_NONE = 0,	
	RM_READYREPAIR,	
	RM_UNDERREPAIR,
	RM_ENDREPAIR
} REPAIRMODE;

class _XWindow_RepairItem  : public _XWindow  
{
private:
	_XImageStatic		m_ItemEdgeIcon;
	_XListBox*			m_pRepairListBox;
	FLOAT				m_EndRepairDelayTime;

public:		
	_XButton*			m_SelectedAllButton;
	_XButton*			m_RepairButton;
	_XButton*			m_SelectedCancleButton;	
	
	POINT				m_RepairSlotPosition[_XDEF_REPAIR_MAX_SENDCOUNT];
	int					m_SelectedSlotNo;
	list<int>			m_SelectedSlotNoList;	///< 인벤의 선택된 아이템 리스트 
	DWORD				m_NeedMoney;
	BOOL				m_bRepairWeapon;

	// drag 
	_XMouseSlotItem		m_MouseSlotItem;
	BOOL				m_bHaveMouseSlot;
	BOOL				m_bDragState;
	BOOL				m_bDrawSelectedItem;
	
	POINT				m_IconDragStartPos;	
	int					m_LastDraggingSlot;

	REPAIRMODE			m_RepairMode;	
	
public:
	_XWindow_RepairItem();
	virtual ~_XWindow_RepairItem();
	
	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void				MoveWindow( int X, int Y );
	void				ShowWindow(BOOL show);
	BOOL				ProcessWheelMessage( short zDelta );

	void				DrawSelectedItem(void);
	void				DrawDragIcon(void);	
	
	void				SetRepairListItem(void);
	void				ResetRepairListData(void);	
	void				DrawDefaultSlot(void);
	void				DrawRepairText(void);
	int					GetRepairSlotIndex(void);

	void				SetEndDelayTime() { m_EndRepairDelayTime = g_fLocalSystemTime; };	
};

#endif // !defined(AFX_XWINDOW_REPAIRITEM_H__D790CEA8_22F4_4CD4_BE98_19D07BC27CAA__INCLUDED_)
