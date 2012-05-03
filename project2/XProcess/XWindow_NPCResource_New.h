// XWindow_NPCResource_New.h: interface for the _XWindow_NPCResource_New class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCRESOURCE_NEW_H__C00902D8_511E_49CB_94FF_B2B191695762__INCLUDED_)
#define AFX_XWINDOW_NPCRESOURCE_NEW_H__C00902D8_511E_49CB_94FF_B2B191695762__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

enum
{
	_TAB_LIST = 0,	// 교환 가능 목록
	_TAB_ITEM = 1,	// 필요 아이템
};

struct _XItem_Info
{
	int	cType;
	int	sID;
	int	ucCount;
	int nInvenNumber;

	int x;
	int y;

	BOOL bDragComplete;
};

class _XWindow_NPCResource_New : public _XWindow
{
private :
	_XStatic		m_BorderTile;
	_XListBox*		m_pResourceList;

	_XImageStatic*	m_pCompleteItemBorder;
	_XImageStatic*	m_pEmptyItemBorder;

	_XButton*		m_pExchangeBtn;
	_XButton*		m_pCloseBtn;

	int				m_nSelectedTab;
	int				m_nSelectedResourcePackageIndex;
	int				m_nSelectedResourceItem;

	BOOL			m_bProgressExchange;

	_XItem_Info		m_tItemInfo[17];

private:
	bool m_bStrength; //Author : 양희왕 // breif : 제련여부에 대한 질의
	
public:
	_XWindow_NPCResource_New();
	virtual ~_XWindow_NPCResource_New();

	BOOL			Initialize();
	void			DestroyWindow();
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			ShowWindow(BOOL show);
	void			MoveWindow(int X, int Y);

	void			SetResourceList();
	void			ChangeTab(int tab);

	void			EnableExchangeButton(BOOL enable);
	void			SetItemBasicInfo();

	BOOL			CheckAllResourceItem();

	BOOL			CheckItem(int cType, int sID, int invennumber, int& index);
	void			SetItem(int index, int invennumber);
	BOOL			CheckInventorySpace();

	void			GetItemSlotIndex(int* itemslotindex);
	BOOL			SetNeedItemComplete(int index);

	void			SetSelectedTab(int tab)		{	m_nSelectedTab = tab;	};
	int				GetSelectedTab()			{	return m_nSelectedTab;	};

	void			SetSelectedResourcePackageIndex(int index)	{	m_nSelectedResourcePackageIndex = index;	}
	int				GetSelectedResourcePackageIndex()			{	return m_nSelectedResourcePackageIndex;		}

	void			SetSelectedResourceItem(int index)	{	m_nSelectedResourceItem = index;	}
	int				GetSelectedResourceItem()			{	return m_nSelectedResourceItem;		}

	void			SetProgressExchange(BOOL progress)	{	m_bProgressExchange = progress;		}
	BOOL			GetProgressExchange()				{	return m_bProgressExchange;			}

	inline void SetStrength( bool bRes ) { m_bStrength = bRes; }
	inline bool GetStrength() const { return m_bStrength; }
};

#endif // !defined(AFX_XWINDOW_NPCRESOURCE_NEW_H__C00902D8_511E_49CB_94FF_B2B191695762__INCLUDED_)
