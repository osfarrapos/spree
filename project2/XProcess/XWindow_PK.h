// XWindow_PK.h: interface for the _XWindow_PK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PK_H__AF9FD9FA_6D21_457A_9FDF_6954F07379E6__INCLUDED_)
#define AFX_XWINDOW_PK_H__AF9FD9FA_6D21_457A_9FDF_6954F07379E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_PK : public _XWindow
{
private :
	_XImageStatic*	m_TimeImage;

public:
	_XWindow_PK();
	virtual ~_XWindow_PK();

	BOOL		Initialize(void);
	void		DestroyWindow(void);
	
	void		Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL		Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

};

#endif // !defined(AFX_XWINDOW_PK_H__AF9FD9FA_6D21_457A_9FDF_6954F07379E6__INCLUDED_)
