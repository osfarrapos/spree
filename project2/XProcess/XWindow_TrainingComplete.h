// XWindow_TrainingComplete.h: interface for the _XWindow_TrainingComplete class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TRAININGCOMPLETE_H__6A13BC9B_F5C2_4904_A037_FEDEB984971C__INCLUDED_)
#define AFX_XWINDOW_TRAININGCOMPLETE_H__6A13BC9B_F5C2_4904_A037_FEDEB984971C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_TrainingComplete : public _XWindow
{
private :
	_XImageStatic		m_CompleteImage;
	int					m_CompleteAlphaValue;

	_XImageStatic		m_UpImage;
	
	int					m_SelectedImage;

public :
	DWORD				m_CompleteTime;


public:
	_XWindow_TrainingComplete();
	virtual ~_XWindow_TrainingComplete();

	BOOL				Initialize(void);

	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void				ShowWindow(BOOL show);

	inline void			SetSelectedImage(int index)	{	m_SelectedImage = index;	};	// 0 : Compelte  1: Up

};

#endif // !defined(AFX_XWINDOW_TRAININGCOMPLETE_H__6A13BC9B_F5C2_4904_A037_FEDEB984971C__INCLUDED_)
