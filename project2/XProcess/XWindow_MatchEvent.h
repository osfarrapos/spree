// XWindow_MatchEvent.h: interface for the _XWindow_MatchEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MATCHEVENT_H__F3CE7B4F_2711_49A9_9D2E_E59CE79E37A9__INCLUDED_)
#define AFX_XWINDOW_MATCHEVENT_H__F3CE7B4F_2711_49A9_9D2E_E59CE79E37A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_MatchEvent : public _XWindow
{
private :
	_XImageStatic*		m_pImgYellowNumber[10];
	_XImageStatic*		m_pImgRedNumber[10];

	_XImageStatic*		m_pImgYellowDot;
	_XImageStatic*		m_pImgRedDot;

public:
	_XWindow_MatchEvent();
	virtual ~_XWindow_MatchEvent();

	BOOL		Initialize(void);
	void		DestroyWindow(void);
	
	void		Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL		Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

};

#endif // !defined(AFX_XWINDOW_MATCHEVENT_H__F3CE7B4F_2711_49A9_9D2E_E59CE79E37A9__INCLUDED_)
