// XWindow_PVPBroker.h: interface for the _XWindow_PVPBroker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PVPBROKER_H__D96B6923_7746_4C74_98F5_B4097C0BCCDB__INCLUDED_)
#define AFX_XWINDOW_PVPBROKER_H__D96B6923_7746_4C74_98F5_B4097C0BCCDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_PVPBroker : public _XWindow
{
private : 
	_XStatic		m_Static[7];
	_XListBox*		m_MatchTargetListBox;
	int				m_ListBoxCount;

	_XStatic		m_BorderTitle;

public:
	_XWindow_PVPBroker();
	virtual ~_XWindow_PVPBroker();

	BOOL			Initialize(void);
	void			DestroyWindow(void);

	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			ShowWindow(BOOL show);
	void			MoveWindow(int X, int Y);
	
	void			SetListItem(__fighterInfo* fightinfo, int count, BOOL bContinue = FALSE);
};

#endif // !defined(AFX_XWINDOW_PVPBROKER_H__D96B6923_7746_4C74_98F5_B4097C0BCCDB__INCLUDED_)
