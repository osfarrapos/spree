// XProc_SelectCharacter.h: interface for the XProc_SelectCharacter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPROC_SELECTCHARACTER_H__8ADB701F_F4F4_4082_A6EC_FDD912F19D75__INCLUDED_)
#define AFX_XPROC_SELECTCHARACTER_H__8ADB701F_F4F4_4082_A6EC_FDD912F19D75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XKernel.h"
#include "XLodTerrain.h"
#include "XBipedController.h"
#include "XUser.h"
#include "XMeshContainer.h"

class XProc_SelectCharacter : public _XProcess  
{

public:

	XProc_SelectCharacter();
	virtual ~XProc_SelectCharacter();

	BOOL			InitializeProcess(void);
	void			DestroyProcess( void );

	bool			InitDeviceObject( void );
	void			ReleaseDeviceObject( void );
	
	void			PauseProcess( void );
	void			UnPauseProcess( void );

	void			Draw( void );
	bool			Process( void );
	
	void			MessageProc( UINT message, WPARAM wparam, LPARAM lparam );	
	void			OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam );
	void			OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam );
	void			OnUserMessage_GetHost( UINT message, WPARAM wparam, LPARAM lparam ){};

	bool			OnKeyboardPoll(_XInput *pInput);
	bool			OnKeyDown(WPARAM wparam, LPARAM lparam);
	bool			OnKeyUp(WPARAM wparam, LPARAM lparam);

	bool			OnMousePoll(MouseState *pState, ScrnPos *pPos);
	bool			OnMouseButton(_XMouseButton p_MB, bool bPushed);
	void			OnMouseWheel(short zDelta);
	void			OnMouseLBtnDblClick( WPARAM wparam );

	void			OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam );	
	void			OnWindowSizeChangeNotify( void );
	void			OnPaint(WPARAM wparam, LPARAM lparam);

	void			ConfirmDevice(void);

	void			SetCharacterInfoWindow( void );
	void			DrawFadeInOut( D3DCOLOR color = 0x000000 );

	void			GetVectorFromMousePos( POINT& pt, D3DXVECTOR3& raydir, D3DXVECTOR3& rayorg );

	BOOL			CreateOffScreen( void );
	
	// ºô¸µ Ã³¸®
	void			ProcessBilling( void );

	_XWindowManager* GetWindowManagerPtr( void) { return &(this->m_WindowManager);	};

public:	
	_XWindowManager		m_WindowManager;

	int					m_SelectedCharacter;
	BOOL				m_LockCharacterSelection;
	_XModelDescriptor	m_PreviewCharacter[_XDEF_MAX_CHARACTERCOUNT];

	DWORD				m_CurrentCharacterLevelIndex;
	DWORD				m_CurrentCharacterSubLevel;

	_XTextureManager	m_TextureArchive;
	_XTextureManager	m_TextureArchive2;	
	
	TCHAR				m_String_None[5];
	TCHAR				m_String_LevelStar[128];
	
	_XImageStatic		m_BackgroundStatic;
	int					m_AlphaValue;

	FLOAT				m_CurrentRotateAngle;
	FLOAT				m_TargetRotateAngle;
	FLOAT				m_RotateDirection;

	BOOL				m_FirstRotateAnimation;
	FLOAT				m_FirstRotateAngle;

	FLOAT				m_CharacterLocalRotateAngle[3];

	BOOL				m_PrevProcess_Login;

	BOOL				m_CharacterDeleteMode;
	DWORD				m_CharacterDeleteModeTimer;
	D3DXVECTOR3			m_PrevSelectedCharacterPosition;
	
	BOOL				m_CharacterSelectedMode;
	DWORD				m_CharacterSelectedModeTimer;
	
	D3DLIGHT9			m_SelectedCharacterLight;
	
	_XImageStatic		m_BottomOverlayLeftImage;
	_XImageStatic		m_BottomOverlayRightImage;

	_XOBBData			m_CharacterOBB;

	TCHAR				m_NewCreatedCharacterName[13];
	
	int					m_LivePacketTimer;
};

#endif // !defined(AFX_XPROC_SELECTCHARACTER_H__8ADB701F_F4F4_4082_A6EC_FDD912F19D75__INCLUDED_)
