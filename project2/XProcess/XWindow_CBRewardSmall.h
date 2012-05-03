// XWindow_CBRewardSmall.h: interface for the _XWindow_CBRewardSmall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CBREWARDSMALL_H__8776B874_C720_4A5F_A3F0_14C740379C76__INCLUDED_)
#define AFX_XWINDOW_CBREWARDSMALL_H__8776B874_C720_4A5F_A3F0_14C740379C76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_CBRewardSmall  : public _XWindow
{
private :
	int				nPoint;
	int				nFame;
	DWORD			dwMoney;

public:
	_XWindow_CBRewardSmall();
	virtual ~_XWindow_CBRewardSmall();

	BOOL			Initialize( void );
	void			DestroyWindow( void );
	
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void			ShowWindow(BOOL show);
	void			SetAward(MSG_CB_RECORD_PVP_AWARD* award);

};

#endif // !defined(AFX_XWINDOW_CBREWARDSMALL_H__8776B874_C720_4A5F_A3F0_14C740379C76__INCLUDED_)
