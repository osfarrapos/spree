// XWindow_GuideLine.h: interface for the _XWindow_GuideLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_GUIDELINE_H__B172FE47_103E_4903_9921_71E49CE55D6D__INCLUDED_)
#define AFX_XWINDOW_GUIDELINE_H__B172FE47_103E_4903_9921_71E49CE55D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

enum GuideLineStep
{
	GS_FIRST,
	GS_SECOND,
	GS_THIRD,
};

class _XWindow_GuideLine : public _XWindow  
{
private:
	GuideLineStep m_GuideLineStep;
		
public:
	_XWindow_GuideLine();
	virtual ~_XWindow_GuideLine();

	BOOL		Initialize( void );
	void		Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL		Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );	

	void		ShowWindow(BOOL show);
	void		MoveWindow(int X, int Y);
	void		SetGuideLIneStep(GuideLineStep step);
};

#endif // !defined(AFX_XWINDOW_GUIDELINE_H__B172FE47_103E_4903_9921_71E49CE55D6D__INCLUDED_)
