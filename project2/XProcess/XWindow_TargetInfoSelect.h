// _XWindow_TargetInfoSelect.h: interface for the _XWindow_TargetInfoSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX__XWINDOW_TARGETINFOSELECT_H__1851B579_B5CC_4BF2_A53F_69895E254BAC__INCLUDED_)
#define AFX__XWINDOW_TARGETINFOSELECT_H__1851B579_B5CC_4BF2_A53F_69895E254BAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

class _XWindow_TargetInfoSelect : public _XWindow
{
public:
	_XWindow_TargetInfoSelect();
	virtual ~_XWindow_TargetInfoSelect();

	BOOL	Initialize();
	
	void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void	ShowWindow(BOOL show);
	void	MoveWindow( int X, int Y );

};

#endif // !defined(AFX__XWINDOW_TARGETINFOSELECT_H__1851B579_B5CC_4BF2_A53F_69895E254BAC__INCLUDED_)
