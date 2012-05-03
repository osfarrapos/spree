// XProc_SelectHero.h: interface for the XProc_SelectHero class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPROC_SELECTHERO_H__A28F404E_E1CF_4679_B45D_F057A23AD614__INCLUDED_)
#define AFX_XPROC_SELECTHERO_H__A28F404E_E1CF_4679_B45D_F057A23AD614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class XProc_SelectHero : public _XProcess  
{
private:
	int						m_iDrawPosX;
	int						m_iDrawPosY;
	_XTextureManager		m_TextureArchive;
	_XCamera				m_Player;
	_XImageStatic			m_HeroCharacterImage[9];
	_XImageStatic			m_HeroNameImage[9];
	int						m_iSelectedHero;

	void			DrawHeroSpeech(int select);
	
public:
	_XButton*	m_StartButton;
	_XButton*	m_SkipButton;
public:
	XProc_SelectHero();
	virtual ~XProc_SelectHero();

	BOOL			InitializeProcess(void);
	void			DestroyProcess( void );

	bool			InitDeviceObject( void );
	void			ReleaseDeviceObject( void );
	
	void			PauseProcess( void );
	void			UnPauseProcess( void );

	void			Draw( void );
	bool			Process( void );	
};

#endif // !defined(AFX_XPROC_SELECTHERO_H__A28F404E_E1CF_4679_B45D_F057A23AD614__INCLUDED_)
