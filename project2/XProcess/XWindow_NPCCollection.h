// XWindow_NPCCollection.h: interface for the _XWindow_NPCCollection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCCOLLECTION_H__C76E0C72_0E4E_4F46_B780_BA9503C5FBDB__INCLUDED_)
#define AFX_XWINDOW_NPCCOLLECTION_H__C76E0C72_0E4E_4F46_B780_BA9503C5FBDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XCollectionItem.h"

class _XWindow_NPCCollection  : public _XWindow
{
public:	
	_XImageStatic*		m_imageSelectedRewardItemBorder;		// 선택된 보상아이템 바탕 이미지
	_XImageStatic*		m_imageRewardItemBorder;				// 보상아이템 바탕 이미지
	_XButton*			m_btnRewardSet[4];						// 보상 세트를 선택하는 버튼
	_XListBox*			m_listboxCollectionItem;				// 수집을 표시하는 리스트 박스
	_XCollectionItem*	m_pSelectedCollectionItem;				// 선택된 수집 아이템
	int					m_indexSelectedRewardSet;				// 선택된 보상 세트 인덱스
	bool				m_bProcessSelectRewardSet;
	bool				m_bRewardSuccess;						// 보상받았을때 뜨는 메세지 처리
	
	_XStatic			m_BorderTile;
	
private :
	int					m_nOwnerNPCId;							// 창이 뜬 npc id
	
public:
	_XWindow_NPCCollection();
	virtual ~_XWindow_NPCCollection();
	
	BOOL			Initialize();
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	void			SetListItem(void);							// 수집 리스트를 구성한다.
	void			SetSelectedCollectionItemIndex(int index);	// 선택된 수집 아이템을 복사한다
	bool			CheckInventorySpace(int rewardsetindex);	// 보상을 받을 수 있는 공간이 있는지 체크한다.

	BOOL			IsMyOwnerPackage(int collectionid);			// 현재 npc에 속한 수집목록인가?
	
	void			SetRewardSetIndex(int index)	{ m_indexSelectedRewardSet = index; };
	int				GetRewardSetIndex(void)			{ return m_indexSelectedRewardSet; };
	
	void			SetProcessReward(bool bprocess)	{ m_bProcessSelectRewardSet = bprocess; };
	bool			GetProcessReward(void)			{ return m_bProcessSelectRewardSet; };

	void			SetOwnerNPCId(int id)			{ m_nOwnerNPCId = id;		};

};
#endif // !defined(AFX_XWINDOW_NPCCOLLECTION_H__C76E0C72_0E4E_4F46_B780_BA9503C5FBDB__INCLUDED_)
