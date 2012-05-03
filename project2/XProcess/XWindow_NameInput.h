// XWindow_NameInput.h: interface for the _XWindow_NameInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NAMEINPUT_H__9636C40D_D30C_4654_96FA_EBB30BD8F340__INCLUDED_)
#define AFX_XWINDOW_NAMEINPUT_H__9636C40D_D30C_4654_96FA_EBB30BD8F340__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum _XNameInput_Mode
{
	_XNAMEINPUT_GROUPNAME = 0,
		_XNAMEINPUT_CHARACTERNAME,
		_XNAMEINPUT_CHANGEGROUPNAME

}_XNAMEINPUT_MODE;

class _XWindow_NameInput : public _XWindow  
{
public:	
	_XButton*		m_OKButton;
	_XButton*		m_CancelButton;
	
	_XIMEContainer	m_IMEControl;
	
	_XNAMEINPUT_MODE	m_NameInputMode;

public:
	_XWindow_NameInput();
	virtual ~_XWindow_NameInput();

	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	BOOL			CheckIMEBoxArea(void);
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	
};

#endif // !defined(AFX_XWINDOW_NAMEINPUT_H__9636C40D_D30C_4654_96FA_EBB30BD8F340__INCLUDED_)
