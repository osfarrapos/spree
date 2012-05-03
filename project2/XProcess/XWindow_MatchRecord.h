// XWindow_MatchRecord.h: interface for the _XWindow_MatchRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MATCHRECORD_H__38C1E23C_2786_4F79_8F75_84B22BFB177E__INCLUDED_)
#define AFX_XWINDOW_MATCHRECORD_H__38C1E23C_2786_4F79_8F75_84B22BFB177E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_MatchRecord : public _XWindow
{
private : 
	_XStatic			m_BorderTile;

	_XCheckButton*		m_chkbtnMatchInfo;
	_XCheckButton*		m_chkbtnMatchTablePVP;
	_XCheckButton*		m_chkbtnMatchTableGroup;
	_XCheckButton*		m_chkbtnMatchTableRage;

	_XImageStatic		m_LeftTitleBarImage;
	_XImageStatic		m_RightTitleBarImage;

	_XButton*			m_btnConfirm;

	_XImageStatic		m_GradeBorderImage;
	_XImageStatic		m_GradeImage;

public:
	_XWindow_MatchRecord();
	virtual ~_XWindow_MatchRecord();

	BOOL			Initialize();

	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);

	void			ChangeTab(int tab);
	void			EnableTablePVPButton(BOOL enable);

};

#endif // !defined(AFX_XWINDOW_MATCHRECORD_H__38C1E23C_2786_4F79_8F75_84B22BFB177E__INCLUDED_)
