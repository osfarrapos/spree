// XProc_CreateCharacter.h: interface for the XProc_CreateCharacter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPROC_CREATECHARACTER_H__8ADB701F_F4F4_4082_A6EC_FDD912F19D75__INCLUDED_)
#define AFX_XPROC_CREATECHARACTER_H__8ADB701F_F4F4_4082_A6EC_FDD912F19D75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XLodTerrain.h"
#include "X3PCamera.h"
#include "XBipedController.h"
#include "XUser.h"



typedef enum
{
	CCPROC_START = 0,
	CCPROC_IDLE,
	CCPROC_SELECTCHARACTER,
	CCPROC_END
}CCPROCESS;

#define _XGROUP_COUNT 6

class XProc_CreateCharacter : public _XProcess  
{

public:

	XProc_CreateCharacter();
	virtual ~XProc_CreateCharacter();

	BOOL			InitializeProcess( void );
	void			DestroyProcess( void );
	
	bool			InitDeviceObject( void );
	void			ReleaseDeviceObject( void );
	
	void			PauseProcess( void );
	void			UnPauseProcess( void );

	void			ConfirmDevice(void);
	void			DrawFadeInOut(void);

	void			Draw( void );
	bool			Process( void );
	
	void			MessageProc( UINT message, WPARAM wparam, LPARAM lparam );	
	void			OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam );	
	bool			OnKeyboardPoll(_XInput *pInput);
	bool			OnKeyDown(WPARAM wparam, LPARAM lparam);
	bool			OnKeyUp(WPARAM wparam, LPARAM lparam);
	bool			OnMousePoll(MouseState *pState, ScrnPos *pPos);
	bool			OnMouseButton(_XMouseButton p_MB, bool bPushed);
    void			OnMouseWheel( short zDelta );
	void			OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam );	
	void			OnWindowSizeChangeNotify( void );
	void			OnPaint(WPARAM wparam, LPARAM lparam);

	void			SetCharacter( void );
	
	BOOL			CreateOffScreen( void );

	BOOL			LoadMinimapTexture( int index, LPTSTR zonefilenmae );

public:
	_XWindowManager		m_WindowManager;

	FLOAT				m_OriginTargetDistance;
	FLOAT				m_OriginRotateAngle;

	DWORD				m_PrevSendedCreateCharacterTime;
	
	_XTextureManager	m_TextureArchive;	
	_XModelDescriptor	m_PreviewCharacter;
	_XUSER_MODELDESCRIPTTYPE m_PreviewModelDescript;
	FLOAT			    m_PreviewCharacterRotateAngle;

	_XImageStatic		m_BackgroundStatic;
	int					m_AlphaValue;
	
	TCHAR				m_CharacterNameString[13];
	int					m_Parameter[5];
	_XGENDERTYPE		m_CharacterGender;

	int					m_FaceTypeManIndex;
	int					m_FaceTypeWomanIndex;
	int					m_HairTypeWomanIndex;
	int					m_HairTypeManIndex;
	int					m_ClothTypeManIndex;
	int					m_ClothTypeWomanIndex;

	D3DXVECTOR3			m_CameraMovingTarget;
	FLOAT				m_CameraMovingTargetDistance;
	BOOL				m_CameraMoving;

	_XImageStatic		m_BottomOverlayLeftImage;
	_XImageStatic		m_BottomOverlayRightImage;
	
	_XImageStatic		m_ZoneNameImage;

	int					m_SelectedZone;
	LPDIRECT3DTEXTURE9	m_pMinimapTexture[_XGROUP_COUNT];
	int					m_pMinimapTextureAlphaLevel[_XGROUP_COUNT];
	_XImageStatic		m_MinimapImageStatic;
	
	int					m_LivePacketTimer;
};

#endif // !defined(AFX_XPROC_CREATECHARACTER_H__8ADB701F_F4F4_4082_A6EC_FDD912F19D75__INCLUDED_)
