// XWindow_MatchInfo.h: interface for the _XWindow_MatchInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MATCHINFO_H__A0717BD6_8898_4F26_B2CE_D0134470A605__INCLUDED_)
#define AFX_XWINDOW_MATCHINFO_H__A0717BD6_8898_4F26_B2CE_D0134470A605__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_MatchInfo : public _XWindow
{
public :
	_XButton*		m_CloseButton;

	unsigned short	m_MatchID;

	TCHAR			m_NameString[128];

public:
	_XWindow_MatchInfo();
	virtual ~_XWindow_MatchInfo();

public :
	BOOL			Initialize(void);
	void			DestroyWindow(void);

	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void			Reset(void);
	void			ShowWindow(BOOL show);
};

#endif // !defined(AFX_XWINDOW_MATCHINFO_H__A0717BD6_8898_4F26_B2CE_D0134470A605__INCLUDED_)
