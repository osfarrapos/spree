// XWindow_OptionGame.h: interface for the _XWindow_OptionGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_OPTIONGAME_H__50765C4E_0A51_4A7F_B773_3307140D4783__INCLUDED_)
#define AFX_XWINDOW_OPTIONGAME_H__50765C4E_0A51_4A7F_B773_3307140D4783__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_OptionGame : public _XWindow  
{

public:
	////////////////////////////////////////////////////////////////////
	// Game options	
	_XCheckButton*		m_pGameGuideButton;
	
	_XCheckButton*		m_pUserNameTooltipButton;
	_XCheckButton*		m_pMyNameTooltipButton;
	_XCheckButton*		m_pItemNameTooltipButton;
	_XCheckButton*		m_RankHidingButton;

	_XCheckButton*		m_pCameraQuaterViewButton;
	_XCheckButton*		m_pCameraUseFirstPersonViewButton;

	_XCheckButton*		m_pViewBattleGaugeButton;
	_XCheckButton*		m_pViewMonsterGaugeButton;
	
	_XCheckButton*		m_pDenyMatchButton;
	_XCheckButton*		m_pChattingAutoClose;
	
	_XCheckButton*		m_pSelectClothesButton;
	
#ifdef _XDEF_KEYCHANGE_20070115
	_XCheckButton*		m_pCheckBtnKeyTypeA;	// 단축키가 숫자키
	_XCheckButton*		m_pCheckBtnKeyTypeB;	// 단축키가 Function 키
#endif

public:
	_XWindow_OptionGame();
	virtual ~_XWindow_OptionGame();

	BOOL Initialize( void );
	void DestroyWindow( void );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void ShowWindow(BOOL show);
};

#endif // !defined(AFX_XWINDOW_OPTIONGAME_H__50765C4E_0A51_4A7F_B773_3307140D4783__INCLUDED_)
