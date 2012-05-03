// XWindow_ExplainCharacter.h: interface for the _XWindow_ExplainCharacter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_EXPLAINCHARACTER_H__DA5207D8_6A15_4D17_88EF_E8CC4DF75209__INCLUDED_)
#define AFX_XWINDOW_EXPLAINCHARACTER_H__DA5207D8_6A15_4D17_88EF_E8CC4DF75209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XUser.h"
class _XWindow_ExplainCharacter : public _XWindow  
{
public:
	int				m_SelectedZone;	
	_XButton*		m_btnIcon[12];

public:
	_XWindow_ExplainCharacter();
	virtual ~_XWindow_ExplainCharacter();
	
	
	BOOL				Initialize(_XTextureManager& texturearchive);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void				SimulationButtonClick( int buttonnumber );
	
};

#endif // !defined(AFX_XWINDOW_EXPLAINCHARACTER_H__DA5207D8_6A15_4D17_88EF_E8CC4DF75209__INCLUDED_)
