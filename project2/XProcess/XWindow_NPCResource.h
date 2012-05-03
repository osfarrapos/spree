// XWindow_NPCResource.h: interface for the _XWindow_NPCResource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCRESOURCE_H__0CE3C03E_6D26_4354_912B_3D746FE8EF0A__INCLUDED_)
#define AFX_XWINDOW_NPCRESOURCE_H__0CE3C03E_6D26_4354_912B_3D746FE8EF0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_NPCResource  : public _XWindow
{
public:	
	_XImageStatic*		m_imageNeeItemBorder;			// 필요아이템 바탕 이미지
	_XImageStatic*		m_imageCompleteNeeItemBorder;	// 전부 모은 필요아이템 바탕 이미지
	_XListBox*			m_listboxResourceItem;			// 자원을 표시하는 리스트 박스
	_XButton*			m_btnExchange;					// 자원으로 교환하는 버튼 
	int					m_indexSelectedPackage;			// NPC가 가지고 있는 패키지 인덱스
	int					m_indexSelectedResourceItem;	// 선택된 자원 아이템 인덱스
	bool				m_bProcessExchange;				// 교환중인지 아닌지 알 수 있는 변수 
	_XStatic			m_BorderTile;
	
public:
	_XWindow_NPCResource();
	virtual ~_XWindow_NPCResource();
	
	BOOL			Initialize();
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	void			SetListItem(void);							// 자원 리스트를 구성한다.
	void			SetSelectedResourceItem(int resourceid);	// 선택된 자원 아이템의 인덱스를 얻어낸다.
	void			SetPackageIndex(int index)	{ m_indexSelectedPackage = index; };

	void			SetProgressExchange(bool bexchange )	{ m_bProcessExchange = bexchange; };
	bool			GetProgressExchange(void)		{ return m_bProcessExchange; };					
	
	bool			CheckInventorySpace(void);					// 보상을 받을 수 있는 공간이 있는지 체크한다.

	inline void SetStrength( bool bRes ) { m_bStrength = bRes; }
	inline bool GetStrength() const { return m_bStrength; }

private:
	bool m_bStrength; //Author : 양희왕 // breif : 제련여부에 대한 질의
};

#endif // !defined(AFX_XWINDOW_NPCRESOURCE_H__0CE3C03E_6D26_4354_912B_3D746FE8EF0A__INCLUDED_)
