// XWindow_Pocket.h: interface for the _XWindow_Pocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_POCKET_H__C4B8742B_3AFA_4133_98A3_AD182C672141__INCLUDED_)
#define AFX_XWINDOW_POCKET_H__C4B8742B_3AFA_4133_98A3_AD182C672141__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#define _XDEF_POCKET_MAX_SLOTNUMBER		8

class _XWindow_Pocket : public _XWindow  
{
private:
	_XImageStatic		m_ItemEdgeIcon[2];
	POINT				m_SlotPosition[_XDEF_POCKET_MAX_SLOTNUMBER];
	
	POINT				m_IconDragStartPos;
	BOOL				m_bDragState;
	int					m_LastDraggingSlot;	

public:
	_XUserItem			m_AccessoryItemList[_XDEF_POCKET_MAX_SLOTNUMBER];
	_XMouseSlotItem		m_MouseSlotItem;	
	BOOL				m_bHaveMouseSlot;

public:
	_XWindow_Pocket();
	virtual ~_XWindow_Pocket();

	void				ResetPocketItem(void);

	BOOL				Initialize( void );
	void				DestroyWindow( void );

	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );	
	void				DrawSelectedItem(void);

	int					SearchSlotIndex(void);
	void				InsertAccessoryItem(int index, int sId, int second);
	void				CopyPocketItem(int invenindex, int pocketindex);
	
	void				DrawDragIcon(void);
};

#endif // !defined(AFX_XWINDOW_POCKET_H__C4B8742B_3AFA_4133_98A3_AD182C672141__INCLUDED_)
