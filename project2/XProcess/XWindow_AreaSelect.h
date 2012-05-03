// XWindow_AreaSelect.h: interface for the _XWindow_AreaSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_AREASELECT_H__EAE538D5_7D3C_448B_A29C_20E45B0EC5A9__INCLUDED_)
#define AFX_XWINDOW_AREASELECT_H__EAE538D5_7D3C_448B_A29C_20E45B0EC5A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XArea;

class _XWindow_AreaSelect : public _XWindow  
{
public:
	_XListBox*			m_pListBox;

	_XArea*				m_pFocusedArea;

public:
	_XWindow_AreaSelect();
	virtual ~_XWindow_AreaSelect();

	BOOL				Initialize(void);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				ShowWindow(BOOL show);
	void				MoveWindow(int X, int Y);

	void				SetBlackClan( void );
	void				SetWhiteClan( void );
	void				InsertAreaList( int areaindex , LPTSTR zonename);
	
	void				SelectClan( int clanindex );
};

#endif // !defined(AFX_XWINDOW_AREASELECT_H__EAE538D5_7D3C_448B_A29C_20E45B0EC5A9__INCLUDED_)
