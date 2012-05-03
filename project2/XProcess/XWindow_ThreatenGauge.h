// XWindow_ThreatenGauge.h: interface for the _XWindow_ThreatenGauge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_THREATENGAUGE_H__4A8F0578_7152_461A_9F51_EBBAEA8D825F__INCLUDED_)
#define AFX_XWINDOW_THREATENGAUGE_H__4A8F0578_7152_461A_9F51_EBBAEA8D825F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_ThreatenGauge : public _XWindow
{
private :
	_XStickGauge*		m_ThreatenGaugeBorder;

public:
	_XWindow_ThreatenGauge();
	virtual ~_XWindow_ThreatenGauge();

	BOOL			Initialize( void );
	void			DestroyWindow( void );
	
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void			ShowWindow(BOOL show);

	void			SetParameter(FLOAT percentage);
};

#endif // !defined(AFX_XWINDOW_THREATENGAUGE_H__4A8F0578_7152_461A_9F51_EBBAEA8D825F__INCLUDED_)
