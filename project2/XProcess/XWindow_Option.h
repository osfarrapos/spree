// XWindow_Option.h: interface for the _XWindow_Option class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_OPTION_H_
#define _XWINDOW_OPTION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum
{
	_XOPTIONTAB_GRAPHIC = 0,
	_XOPTIONTAB_SOUND,
	_XOPTIONTAB_GAME,
	_XOPTIONTAB_CONMMUNITY
} _XOptionWindowTab;

class _XWindow_Option : public _XWindow  
{
public:

	_XStatic			m_BorderTile;
	_XImageStatic		m_BorderGradationTitle;
		
	_XCheckButton*		m_OptionTabButton[4];

	_XOptionWindowTab	m_SelectedTab;
	
	////////////////////////////////////////////////////////////////////
	// Sound menu
	_XCheckButton*		m_pUseBMGButton;	
	_XHSlider*			m_pBGMVolumeSlider;		
	_XCheckButton*		m_pUseSoundEffectButton;
	_XHSlider*			m_pEffectSoundVolumeSlider;

	////////////////////////////////////////////////////////////////////
	// Sound menu
	_XCheckButton*		m_pCommunityButton;	

	////////////////////////////////////////////////////////////////////
	// System menu
	_XButton*			m_ExitButton;
	_XButton*			m_GotoCharacterSelectButton;	

	_XWindow*			m_pSubWindow[4];
	
public:
	_XWindow_Option();
	virtual ~_XWindow_Option();

	BOOL Initialize( void );
	void DestroyWindow( void );
	void MoveWindow( int X, int Y );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void SetDefaultPosition( void );
	void SaveWindowProperty( void );
	void ApplyOptionProperties( void );

	void DrawSubDlg_Sound( _XGUIObject*& pfocusobject = g_CurrentFocusedObject  );
	void DrawSubDlg_Screen( _XGUIObject*& pfocusobject = g_CurrentFocusedObject  );
	void DrawSubDlg_Visual( _XGUIObject*& pfocusobject = g_CurrentFocusedObject  );
	void DrawSubDlg_Community( _XGUIObject*& pfocusobject = g_CurrentFocusedObject  );
	void DrawSubDlg_Game( _XGUIObject*& pfocusobject = g_CurrentFocusedObject  );
	void DrawSubDlg_Quick( _XGUIObject*& pfocusobject = g_CurrentFocusedObject  );
	void DrawSubDlg_System( _XGUIObject*& pfocusobject = g_CurrentFocusedObject  );

	void ChangeTab( _XOptionWindowTab selecttab );	

	void ConfirmDevice( void );

	void ShowWindow(BOOL show);
	void SendRankHidingPacket( void );
	void SetRankHidingState( char check );
};

#endif // !defined(AFX_XWINDOW_OPTION_H__E9A5E061_F621_4A8C_8966_0BDDBC7D7145__INCLUDED_)
