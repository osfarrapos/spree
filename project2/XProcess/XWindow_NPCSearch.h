// XWindow_NPCSearch.h: interface for the _XWindow_NPCSearch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCSEARCH_H__F20441F8_D076_4FE7_B8E6_9E3A0775D828__INCLUDED_)
#define AFX_XWINDOW_NPCSEARCH_H__F20441F8_D076_4FE7_B8E6_9E3A0775D828__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_NPCSearch : public _XWindow  
{
public:
	_XImageStatic	m_BorderGradationTitle;

	_XListBox*		m_listboxNPCJob;			// NPC 직업을 나타내는 리스트

public:
	_XWindow_NPCSearch();
	virtual ~_XWindow_NPCSearch();
	
public:
	BOOL	Initialize(void);	
	void	MoveWindow( int X, int Y );
	void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void	ShowWindow(BOOL show);
};

#endif // !defined(AFX_XWINDOW_NPCSEARCH_H__F20441F8_D076_4FE7_B8E6_9E3A0775D828__INCLUDED_)
