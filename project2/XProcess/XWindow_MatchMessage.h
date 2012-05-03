// XWindow_MatchMessage.h: interface for the _XWindow_MatchMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MATCHMESSAGE_H__28860E66_199D_4987_BA42_BB99C124E55E__INCLUDED_)
#define AFX_XWINDOW_MATCHMESSAGE_H__28860E66_199D_4987_BA42_BB99C124E55E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_MatchMessage : public _XWindow
{
private :
	_XImageStatic	m_NumberBigImage[6];

	_XImageStatic	m_TimeOverImage;
	_XImageStatic	m_StartMatchImage;
	_XImageStatic	m_EndMatchImage;
	_XImageStatic	m_TimeOverEndImage;

public :
	BOOL			m_bCountdown;
	DWORD			m_dwCountdownStartTime;


public:
	_XWindow_MatchMessage();
	virtual ~_XWindow_MatchMessage();

	BOOL			Initialize();
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void			ResetData(void);
};

#endif // !defined(AFX_XWINDOW_MATCHMESSAGE_H__28860E66_199D_4987_BA42_BB99C124E55E__INCLUDED_)
