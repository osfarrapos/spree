// XWindow_CastleBattleTimeChange.h: interface for the _XWindow_CastleBattleTimeChange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CASTLEBATTLETIMECHANGE_H__363E00D4_C940_44F4_A53A_35FF843545F0__INCLUDED_)
#define AFX_XWINDOW_CASTLEBATTLETIMECHANGE_H__363E00D4_C940_44F4_A53A_35FF843545F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_CastleBattleTimeChange : public _XWindow  
{
public:
	_XButton*			m_btnTimeChange;

	_XListBox*			m_listboxTimeChange1;
	_XListBox*			m_listboxTimeChange2;
	_XListBox*			m_listboxTimeChange3;
	
	int					m_ChangeTime[3]; //적용된 변경 시간 
	int					m_tempChangeTime[3]; //적용할 변경 시간 
	
	_XStatic			m_BorderTile;

private :
	_XCastleInfo*		m_pCurCastleInfo;

public:
	_XWindow_CastleBattleTimeChange();
	virtual ~_XWindow_CastleBattleTimeChange();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	
	void			ChangeTime(void);
	void			SetListItem(void);
};

#endif // !defined(AFX_XWINDOW_CASTLEBATTLETIMECHANGE_H__363E00D4_C940_44F4_A53A_35FF843545F0__INCLUDED_)
