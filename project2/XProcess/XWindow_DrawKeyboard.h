// XWindow_DrawKeyboard.h: interface for the _XWindow_DrawKeyboard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_DRAWKEYBOARD_H__BC295A0F_D918_4FDA_B15C_76EA4FCC5163__INCLUDED_)
#define AFX_XWINDOW_DRAWKEYBOARD_H__BC295A0F_D918_4FDA_B15C_76EA4FCC5163__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_DrawKeyboard : public _XWindow  
{
private:
	int 		m_ShiftDownValue;  // 0 : non clicked  54 : clicked	
	int			m_CurrentOverAreaIndex;
	int			m_OverKeyAlphaLevel;
	int			m_OverKeyAlphaLevelInterval;
	BOOL		m_ClickedMouseButton;
	int			m_TempClickedindex;

public:
	BOOL		m_LoginProcessMode;
	
public:
	_XWindow_DrawKeyboard();
	virtual ~_XWindow_DrawKeyboard();

	BOOL		Initialize( _XTextureManager& texturearchive );
	void		DestroyWindow( void );
	
	void		Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL		Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void		KeyProcess(void);
	void		SetShiftDownValue(int value) { m_ShiftDownValue = value; };
	int			GetShiftDownValue(void) { return m_ShiftDownValue; };
};

#endif // !defined(AFX_XWINDOW_DRAWKEYBOARD_H__BC295A0F_D918_4FDA_B15C_76EA4FCC5163__INCLUDED_)
