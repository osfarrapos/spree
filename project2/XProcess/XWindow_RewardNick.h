// XWindow_RewardNick.h: interface for the _XWindow_RewardNick class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_REWARDNICK_H__41BF6A7D_945F_42BE_9DBD_C73BB0436BE1__INCLUDED_)
#define AFX_XWINDOW_REWARDNICK_H__41BF6A7D_945F_42BE_9DBD_C73BB0436BE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_RewardNick  : public _XWindow
{
private : 
	int			m_nNick;

public:
	_XWindow_RewardNick();
	virtual ~_XWindow_RewardNick();

	BOOL		Initialize(void);
	void		DestroyWindow(void);
	
	void		Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL		Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void		ShowWindow(BOOL show);

	void		SetNickIndex(int index)		{		m_nNick = index;		};
	int			GetNickIndex(void)			{		return m_nNick;			};

};

#endif // !defined(AFX_XWINDOW_REWARDNICK_H__41BF6A7D_945F_42BE_9DBD_C73BB0436BE1__INCLUDED_)
