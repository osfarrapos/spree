// XWindow_MatchWindow.h: interface for the _XWindow_MatchWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MATCHWINDOW_H__F0B68740_1A9A_44BC_B0E3_0F4A5197FECB__INCLUDED_)
#define AFX_XWINDOW_MATCHWINDOW_H__F0B68740_1A9A_44BC_B0E3_0F4A5197FECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_MatchWindow  : public _XWindow
{
public :
	_XButton*			m_AcceptButton;
	_XButton*			m_RefuseButton;
	_XButton*			m_DetailInfoButton;
	
	char				m_cMatchType;
	unsigned short		m_usFromID;
	short				m_sInnerLevel;
	char				m_cGroupIndex;

	char				m_cPartyMemberCount;
	char				m_cRoundCount;
	short				m_sAverageLevel;
	short				m_sMaxLevel;

	TCHAR				m_FMessageString[1024];
	TCHAR				m_SMessageString[1024];

public:
	_XWindow_MatchWindow();
	virtual ~_XWindow_MatchWindow();

public :
	BOOL				Initialize(void);
	void				DestroyWindow(void);

	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void				Reset(void);
	void				ShowWindow(BOOL show);

	void				Test(void);

};

#endif // !defined(AFX_XWINDOW_MATCHWINDOW_H__F0B68740_1A9A_44BC_B0E3_0F4A5197FECB__INCLUDED_)
