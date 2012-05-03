// XWindow_RotateWindow.h: interface for the _XWindow_RotateWindow class.
//
// Created Date		: 2003.05.06
// Author			: Sohyun, Park (kukuri)
// Desc				: Character Zoom Window - Character Create
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KUXWINDOW_ROTATEWINDOW_H__067A8CFD_FEFA_414D_B6FF_A18A2ABB559F__INCLUDED_)
#define AFX_KUXWINDOW_ROTATEWINDOW_H__067A8CFD_FEFA_414D_B6FF_A18A2ABB559F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_RotateWindow : public _XWindow  
{
public :
	_XHSlider*			m_BaseSlider;
	_XButton*			m_LeftButton;
	_XButton*			m_RightButton;
	
	int					m_OriginSlidingPosition;
	
public:
	_XWindow_RotateWindow();
	virtual ~_XWindow_RotateWindow();
	
	BOOL				Initialize(_XTextureManager& texturearchive);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				SubSlidingPosition(void);
	void				AddSlidingPosition(void);
	
};

#endif // !defined(AFX_KUXWINDOW_ROTATEWINDOW_H__067A8CFD_FEFA_414D_B6FF_A18A2ABB559F__INCLUDED_)
