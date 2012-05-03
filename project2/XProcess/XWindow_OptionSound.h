// XWindow_OptionSound.h: interface for the _XWindow_OptionSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_OPTIONSOUND_H__50765C4E_0A51_4A7F_B773_3307140D4783__INCLUDED_)
#define AFX_XWINDOW_OPTIONSOUND_H__50765C4E_0A51_4A7F_B773_3307140D4783__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_OptionSound : public _XWindow  
{

public:	

	_XCheckButton*		m_pUseBGMButton;
	_XHSlider*			m_pBGMVolumeSlider;

	_XCheckButton*		m_pUseEffectSoundButton;
	_XHSlider*			m_pEffectSoundVolumeSlider;

	_XCheckButton*		m_pUseEnvSoundButton;
	_XHSlider*			m_pEnvSoundVolumeSlider;

	_XCheckButton*		m_pUseGuideSoundButton;

public:
	_XWindow_OptionSound();
	virtual ~_XWindow_OptionSound();

	BOOL Initialize( void );
	void DestroyWindow( void );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
};

#endif // !defined(AFX_XWINDOW_OPTIONSOUND_H__50765C4E_0A51_4A7F_B773_3307140D4783__INCLUDED_)
