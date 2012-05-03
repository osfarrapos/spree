// XWindow_EventNotice.h: interface for the _XWindow_EventNotice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_EVENTNOTICE_H__D0E34362_7E12_4CA1_8F1F_AC43DD7E8BC6__INCLUDED_)
#define AFX_XWINDOW_EVENTNOTICE_H__D0E34362_7E12_4CA1_8F1F_AC43DD7E8BC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 이벤트 공지창 - 고정
 * event.html 을 보여준다.
 * 크기 고정 
 * XProc_MainGame의 InitializeProcess() 함수 LoadEventNoticeInfo() 함수로 On/Off 함
*/

class _XWindow_EventNotice : public _XWindow  
{
	_XImageStatic		m_HTMLViewStatic;		
	_XButton*			m_pClosebutton;
	_XCheckButton*		m_pCheckbutton;

public:
	_XWindow_EventNotice();
	virtual ~_XWindow_EventNotice();

	BOOL 		Initialize( void );	

	void 		Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL		Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void		ShowWindow(BOOL show);
	void 		MoveWindow( int X, int Y );

	void		SetHelpHtml( int helpid );
	void		SetHelpHtml();

	void		SetEventNotice(void);
};

#endif // !defined(AFX_XWINDOW_EVENTNOTICE_H__D0E34362_7E12_4CA1_8F1F_AC43DD7E8BC6__INCLUDED_)
