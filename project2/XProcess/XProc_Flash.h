// XProc_Flash.h: interface for the XProc_Flash class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPROC_FLASH_H__924311A4_E587_4631_BD07_B95038D3273D__INCLUDED_)
#define AFX_XPROC_FLASH_H__924311A4_E587_4631_BD07_B95038D3273D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "FlashControlInterface.h"

#define _XDEF_USENEWBACKGROUND

typedef enum _XMovie_Mode
{
	_XMOVIE_NONE = 0,
		_XMOVIE_QUEST,
		_XMOVIE_BOSS,
		_XMOVIE_ZEROLEVEL,
		_XMOVIE_MAP,
		_XMOVIE_CLASSEXPLAIN
}_XMOVIE_MODE;

#define _XDEF_FLASH_MAX_ZONECOUNT			17
class XProc_Flash  : public _XProcess  
{
public:
	XProc_Flash();
	virtual ~XProc_Flash();
	
public:
	CFlashControlInterface	*flashWin;
	DWORD					m_Singnaltime;
	_XMOVIE_MODE			m_MovieMode;
	
	BOOL					m_bLockEndProcess;
	BOOL					m_bClickExitFlashMap;
	
	LPDIRECT3DTEXTURE9		m_pOffScreenTexture;
		
#ifdef _XDEF_USENEWBACKGROUND
	_XStatic				m_FlashStatic;
#else
	_XImageStatic			m_FlashStatic;
#endif
	int						m_FlashScreenAlphaLevel;
	
	// 구룡전도에서 쓰이는 변수들
	_XImageStatic*			m_imageCurrentZoneMark[4];	

	BOOL					m_bInCurrentZone;
	RECT					m_MaximumMapZoneList[_XDEF_FLASH_MAX_ZONECOUNT];
	FLOAT					m_RateOfMapSizeToTerrain;
	
	DWORD					m_PrevDrawTime;
	BOOL					m_EffectOrderSub;
	FLOAT					m_EffectFrame;
		
public:	
	BOOL InitializeProcess( void );
	void DestroyProcess( void );
	
	bool InitDeviceObject( void );
	void ReleaseDeviceObject( void );
	
	void Draw( void );
	bool Process( void );

	void FlashDraw( void );
	
	void MessageProc( UINT message, WPARAM wparam, LPARAM lparam );	
	void OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam );	
	void OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam );
	bool OnKeyboardPoll(_XInput *pInput);
	bool OnKeyDown(WPARAM wparam, LPARAM lparam);
	bool OnKeyUp(WPARAM wparam, LPARAM lparam);
	bool OnMousePoll(MouseState *pState, ScrnPos *pPos);
	void OnMouseMove( WPARAM wparam, LPARAM lparam );
	bool OnMouseButton(_XMouseButton p_MB, bool bPushed);
	void OnTimer(WPARAM wparam, LPARAM lparam);	
	void OnPaint(WPARAM wparam, LPARAM lparam);	

	BOOL CreateOffScreen( void );
	
	void DrawCurrentZoneMark( int x, int y );
};

#endif // !defined(AFX_XPROC_FLASH_H__924311A4_E587_4631_BD07_B95038D3273D__INCLUDED_)
