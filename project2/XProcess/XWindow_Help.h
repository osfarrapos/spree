// XWindow_Help.h: interface for the _XWindow_Help class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_HELP_H__651F4225_1E6F_4447_8D41_3D975BDC8513__INCLUDED_)
#define AFX_XWINDOW_HELP_H__651F4225_1E6F_4447_8D41_3D975BDC8513__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

enum MinigameType 
{
	MT_NONE, 
	MT_MEDITATION, 
	MT_DUMMY,
	MT_SPELL,
	MT_TUTORIAL		// zerolevel
};

class _XWindow_Help : public _XWindow  
{
private:
	_XImageStatic	m_Mini_Meditation1[5];
	MinigameType	m_MiniGameState;
	
	_XButton*		m_CloseButton;
	BOOL			m_SettingOn;

	void			DrawMeditationText();
	void			DrawDummyText();
	void			DrawSpellText();

	// zerolevel
	int				m_TutorialState;	
	void			DrawTutorialText();

public:
	_XWindow_Help();
	virtual ~_XWindow_Help();
	
	MinigameType GetMiniGameState() { return m_MiniGameState; };
	void SetMiniGameState(MinigameType type) { m_MiniGameState = type; };
	void SetTutorialState(int state) { m_TutorialState = state; 	};	// zerolevel
	BOOL CheckCloseButton() { return m_CloseButton->CheckMousePosition(); 	};	// zerolevel

	BOOL Initialize( void );
	void SetWindowSize( SIZE size );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
};

#endif // !defined(AFX_XWINDOW_HELP_H__651F4225_1E6F_4447_8D41_3D975BDC8513__INCLUDED_)
