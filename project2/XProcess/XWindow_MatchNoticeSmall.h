// XWindow_MatchNoticeSmall.h: interface for the _XWindow_MatchNoticeSmall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MATCHNOTICESMALL_H__0FF851BE_8A8B_40E3_AB5F_1002BFEC28A0__INCLUDED_)
#define AFX_XWINDOW_MATCHNOTICESMALL_H__0FF851BE_8A8B_40E3_AB5F_1002BFEC28A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_MatchNoticeSmall : public _XWindow
{
private :
	_XImageStatic		m_DefaultRecordImage;

	_XStickGauge*		m_MyVitalGauge;
	_XStickGauge*		m_TargetVitalGauge;

public:
	_XWindow_MatchNoticeSmall();
	virtual ~_XWindow_MatchNoticeSmall();

	BOOL			Initialize();

	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void			SetParameter();

};

#endif // !defined(AFX_XWINDOW_MATCHNOTICESMALL_H__0FF851BE_8A8B_40E3_AB5F_1002BFEC28A0__INCLUDED_)
