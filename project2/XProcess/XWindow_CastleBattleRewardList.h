// XWindow_CastleBattleRewardList.h: interface for the _XWindow_CastleBattleRewardList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CASTLEBATTLEREWARDLIST_H__485D9C85_6B23_40F6_911B_FB845B388C33__INCLUDED_)
#define AFX_XWINDOW_CASTLEBATTLEREWARDLIST_H__485D9C85_6B23_40F6_911B_FB845B388C33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_CastleBattleRewardList : public _XWindow  
{
private :
	int				nRank;
	int				nPoint;
	DWORD			dwExp;
	int				nFame;
	DWORD			dwMoney;
	
	int				nNormalPoint;
	int				nNormalFame;
	DWORD			dwNormalMoney;
	
public:
	_XWindow_CastleBattleRewardList();
	virtual ~_XWindow_CastleBattleRewardList();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);

	void			SetSpecialAward(MSG_CB_RECORD_WINNER_AWARD* award);
	void			SetNormalAward(MSG_CB_RECORD_PVP_AWARD* award);
};

#endif // !defined(AFX_XWINDOW_CASTLEBATTLEREWARDLIST_H__485D9C85_6B23_40F6_911B_FB845B388C33__INCLUDED_)
