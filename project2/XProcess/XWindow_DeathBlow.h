// XWindow_DeathBlow.h: interface for the _XWindow_DeathBlow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_DEATHBLOW_H__7D43A8D5_D064_4A63_99B4_B574B147C0B4__INCLUDED_)
#define AFX_XWINDOW_DEATHBLOW_H__7D43A8D5_D064_4A63_99B4_B574B147C0B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_DeathBlow : public _XWindow  
{
public:
	_XButton*		m_LeftDeathBlowButton;
	_XButton*		m_RightDeathBlowButton;
	
	_XImageStatic	m_LevelUpBorderImage;
	
	LONG				m_SpeedCounter;	
	FLOAT				m_SelectedEffectFrame;
	int					m_SelectedEffectPrevFrame;
	
public:
	_XWindow_DeathBlow();
	virtual ~_XWindow_DeathBlow();
	
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void SetDefaultPosition( void );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void SetButtonStatus( BOOL Show );
};

#endif // !defined(AFX_XWINDOW_DEATHBLOW_H__7D43A8D5_D064_4A63_99B4_B574B147C0B4__INCLUDED_)
