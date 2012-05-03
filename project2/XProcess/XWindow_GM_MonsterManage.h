// XWindow_GM_MonsterManage.h: interface for the XWindow_GM_MonsterManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_GM_MONSTERMANAGE_H__B836BB81_2CB0_465B_8830_F4BC380D49A0__INCLUDED_)
#define AFX_XWINDOW_GM_MONSTERMANAGE_H__B836BB81_2CB0_465B_8830_F4BC380D49A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_GM_MonsterManage : public _XWindow  
{
public:
	_XIMEContainer	m_SpawnCountIMEControl;

	int				m_SelectedMonsterType;

	_XListBox*		m_pMonsterListBox;

public:
	_XWindow_GM_MonsterManage();
	virtual ~_XWindow_GM_MonsterManage();


	BOOL						Initialize( void );
	void						DestroyWindow( void );
	
	void						Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL						Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void						MoveWindow( int X, int Y );
	void						ShowWindow( BOOL show );
	
	void						UpdateMonsterList( void );
};

#endif // !defined(AFX_XWINDOW_GM_MONSTERMANAGE_H__B836BB81_2CB0_465B_8830_F4BC380D49A0__INCLUDED_)
