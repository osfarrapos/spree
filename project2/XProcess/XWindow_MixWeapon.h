// XWindow_MixWeapon.h: interface for the _XWindow_MixWeapon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MIXWEAPON_H__E5A7E291_C410_4180_9F91_8748EC517CA7__INCLUDED_)
#define AFX_XWINDOW_MIXWEAPON_H__E5A7E291_C410_4180_9F91_8748EC517CA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include <vector>

typedef struct _sItemSlot
{
	int		cType;
	int		sID;
	int		nInvenNumber;

	_sItemSlot()
	{
		cType = 0;
		sID = 0;
		nInvenNumber = -1;
	}

}_XItemSlot;

typedef struct _sMixCost
{
	int		rank;
	DWORD	cost;
	
	_sMixCost()
	{
		rank = 0;
		cost = 0;
	}

}_XMixCost;

typedef vector<_XMixCost*> _XVecMixCost;

class _XWindow_MixWeapon  : public _XWindow
{
private :
	_XButton*	m_pOKButton;
	_XButton*	m_pCancelButton;

	POINT		m_SlotPosition[3];

	_XVecMixCost	m_vecMixCost;
	
	DWORD		m_SelectedWeaponCost;

public :
	_XItemSlot	m_ItemSlot[3];

public:
	_XWindow_MixWeapon();
	virtual ~_XWindow_MixWeapon();

	BOOL		Initialize();
	void		DestroyWindow();

	void		Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL		Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	void		ShowWindow(BOOL show);

	BOOL		CheckItem(int cType, int sID, int nInvenNumber);
	void		SetItemSlot(int cType, int sId, int nInvenNumber);
	void		ResetItemSlot(void);
	void		ResetItemSlot(int index);
	
	BOOL		LoadCostScript(void);

	DWORD		GetSelectedWeaponCost()		{	return m_SelectedWeaponCost;	};

};

#endif // !defined(AFX_XWINDOW_MIXWEAPON_H__E5A7E291_C410_4180_9F91_8748EC517CA7__INCLUDED_)
