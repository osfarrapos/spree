// XWindow_PKTracingRequest.h: interface for the _XWindow_PKTracingRequest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PKTRACINGREQUEST_H__5DDAA1D9_7113_450F_BE32_B83660887401__INCLUDED_)
#define AFX_XWINDOW_PKTRACINGREQUEST_H__5DDAA1D9_7113_450F_BE32_B83660887401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XWindow.h"

class _XWindow_PKTracingRequest  : public _XWindow  
{
public:	
	_XButton*			m_btnRequest1;
	_XButton*			m_btnRequest2;
	_XButton*			m_btnRequest3;
	_XButton*			m_btnCancle;

	_XImageStatic		m_CornerImage[2];
	TCHAR				m_cTargetName[13];	// 등록할 PK범 이름 
	TCHAR				m_cSpecialTargetName[13]; // 특정인 이름 //Author : 양희왕
	TCHAR				m_cDeleteTargetName[13];	// 동록 리스트에서 삭제할 PK범 이름
	
public:
	_XWindow_PKTracingRequest();
	virtual ~_XWindow_PKTracingRequest();
	
	BOOL				Initialize(void);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				ShowWindow(BOOL show);
};

#endif // !defined(AFX_XWINDOW_PKTRACINGREQUEST_H__5DDAA1D9_7113_450F_BE32_B83660887401__INCLUDED_)
