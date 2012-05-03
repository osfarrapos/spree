// XWindow_ZoomWindow.h: interface for the _XWindow_ZoomWindow class.
//
// Created Date		: 2003.05.06
// Author			: Sohyun, Park (kukuri)
// Desc				: Character Zoom Window - Character Create
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KUXWINDOW_ZOOMWINDOW_H__6060E818_D09D_42E0_80C4_2551387E11FF__INCLUDED_)
#define AFX_KUXWINDOW_ZOOMWINDOW_H__6060E818_D09D_42E0_80C4_2551387E11FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XHSlider.h"

class _XWindow_ZoomWindow : public _XWindow  
{
public :
	_XHSlider*			m_BaseSlider;
	_XButton*			m_LeftButton;
	_XButton*			m_RightButton;
	
	int					m_OriginSlidingPosition;
	
public:
	_XWindow_ZoomWindow();
	virtual ~_XWindow_ZoomWindow();
	
	BOOL				Initialize(_XTextureManager& texturearchive);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				SubSlidingPosition(void);
	void				AddSlidingPosition(void);
};

#endif // !defined(AFX_KUXWINDOW_ZOOMWINDOW_H__6060E818_D09D_42E0_80C4_2551387E11FF__INCLUDED_)
