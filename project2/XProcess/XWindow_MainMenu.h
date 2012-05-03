// XWindow_MainMenu.h: interface for the _XWindow_MainMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MAINMENU_H__F3680A78_D260_4E0C_95B6_1663829A02CD__INCLUDED_)
#define AFX_XWINDOW_MAINMENU_H__F3680A78_D260_4E0C_95B6_1663829A02CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_MainMenu : public _XWindow  
{
private:
	_XImageStatic	m_GuideLineImage;
public:
	_XImageStatic*	m_pMessengerStatusStatic;
	_XImageStatic*	m_pEmailIcon;
	_XImageStatic*  m_pMessengerIcon;

	_XButton*		m_pMainButton;

	_XImageStatic	m_MainButtonRoundEdge;
	BOOL			m_MainButtonRoundEdgeAlphaIncrease;
	int				m_MainButtonRoundEdgeAlphaLevel;

	_XButton*		m_MeditationButton;
	int				m_MeditationIndex;
	
	_XImageStatic	m_MedEffectImage;
	DWORD			m_PrevDrawTime;
	FLOAT			m_EffectFrame;

	BOOL			m_bDrawMedEffect;
	BOOL			m_bDrawMedEffectInfinite;
	
public:
	_XWindow_MainMenu();
	virtual ~_XWindow_MainMenu();
	
	
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void SetDefaultPosition( void );
	void SaveWindowProperty( void );
	
	void MoveWindow( int X, int Y );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void DrawTooltip(void);
	
	void SetMessengerStatus( int currentuserstatus );
	
	void SetMedButton(void);
	void DrawMedEffect(int count);

	void SetDrawGuideLine(BOOL flag);

	void ShowWindow(BOOL show);
};

#endif // !defined(AFX_XWINDOW_MAINMENU_H__F3680A78_D260_4E0C_95B6_1663829A02CD__INCLUDED_)
