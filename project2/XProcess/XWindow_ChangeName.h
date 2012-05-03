// XWindow_ChangeName.h: interface for the _XWindow_ChangeName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CHANGENAME_H__BE56CEA9_844C_4F86_A9B9_CAD79A6EACDB__INCLUDED_)
#define AFX_XWINDOW_CHANGENAME_H__BE56CEA9_844C_4F86_A9B9_CAD79A6EACDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/*! 캐릭명 입력창 
	- 서버 통합이 끝나면 쓰이지 않음.
	- 서버통합화면과 캐릭선택화면 두 곳에서 쓰임
*/
class _XWindow_ChangeName : public _XWindow  
{
private:
	int					m_SelectedIndex;
	_XIMEContainer		m_ChangeNameEdit;

public:
	_XWindow_ChangeName();
	virtual ~_XWindow_ChangeName();

	BOOL				Initialize(_XTextureManager& texturearchive1, _XTextureManager& texturearchive2);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void				MoveWindow(int X, int Y);
	int					GetSelectedIndex(void) { return m_SelectedIndex; };
	void				SetSelectedIndex(int index) { m_SelectedIndex = index; };
	TCHAR*				GetChangedName(void) { return m_ChangeNameEdit.GetText(); };
	void				ChangeNameFiltering(void);
	
	void				ShowWindow(BOOL show);
};

#endif // !defined(AFX_XWINDOW_CHANGENAME_H__BE56CEA9_844C_4F86_A9B9_CAD79A6EACDB__INCLUDED_)
