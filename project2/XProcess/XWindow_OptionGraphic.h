// XWindow_OptionGraphic.h: interface for the _XWindow_OptionGraphic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_OPTIONGRAPHIC_H__50765C4E_0A51_4A7F_B773_3307140D4783__INCLUDED_)
#define AFX_XWINDOW_OPTIONGRAPHIC_H__50765C4E_0A51_4A7F_B773_3307140D4783__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_OptionGraphic : public _XWindow  
{

public:
	////////////////////////////////////////////////////////////////////
	// Graphic options
	_XHSlider*			m_pGammaSlider;
	
	_XHSlider*			m_pTerrainLodSlider;
	_XHSlider*			m_pSightRangeSlider;
	
	_XHSlider*			m_pGrassQualitySlider;
	_XCheckButton*		m_pGrassAnimationButton;
	_XCheckButton*		m_pTreeAnimationButton;
	
	_XCheckButton*		m_pCharacterShadowButton;
	_XCheckButton*		m_pUseBloomFilterButton;
	_XHSlider*			m_pUseBloomFilterSlider;
	
	_XHSlider*			m_pEffectQualitySlider;
	_XCheckButton*		m_pReflectionButton;
	_XCheckButton*		m_pLensFlareButton;

public:
	_XWindow_OptionGraphic();
	virtual ~_XWindow_OptionGraphic();

	BOOL Initialize( void );
	void DestroyWindow( void );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
};

#endif // !defined(AFX_XWINDOW_OPTIONGRAPHIC_H__50765C4E_0A51_4A7F_B773_3307140D4783__INCLUDED_)
