// XProc_LoginServer.h: interface for the XProc_LoginServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPROC_LOGINSERVER_H__F483169E_50C6_4322_B8A1_BD84751EB5D3__INCLUDED_)
#define AFX_XPROC_LOGINSERVER_H__F483169E_50C6_4322_B8A1_BD84751EB5D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XLensFlare.h"
#include "XBipedController.h"
#include "XAnimMeshContainer.h"
#include "XMeshContainer.h"

#define _MAX_IDSTRINGSIZE					32
#define _MAX_PWSTRINGSIZE					32

#define _XDEF_SERVERCONNECTMODE_LOGIN		0
#define _XDEF_SERVERCONNECTMODE_GAMESERVER	1

typedef enum
{
	LOGINPROC_START = 0,
	LOGINPROC_FADEEND,
	LOGINPROC_AFTERLOGIN,
	LOGINPROC_AFTERANIMATION,
	LOGINPROC_END
}LOGINPROCESS;

#ifdef _XTS_NEWCHARACLIST
#ifdef _XTESTSERVER
#define _COUNT_MAX_GROUP_					18
#else
#define _COUNT_MAX_GROUP_					122
#define _GR_INDEX_TRAIN_					120		///< 수련서버
#define _GR_INDEX_UNION_					121		///< 통합서버

#endif
#else
#define _COUNT_MAX_GROUP_					18
#endif
#define _FOREST_STATE_FULL					-3		///< 포화
#define _FOREST_STATE_STOP					-2		///< 점검중
#define _FOREST_STATE_OFF					-1		///< 정지

class CFlashControlInterface;

typedef struct _XSERVERTRAFFICDATA
{
	char trafficfactor;
	int	 serverindex;
} _XSERVERTRAFFICDATA;

class XProc_LoginServer : public _XProcess  
{
public:
	XProc_LoginServer();
	virtual ~XProc_LoginServer();
	
	TCHAR				m_IDString[256];
	TCHAR				m_PWString[256];

	_XTextureManager	 m_TextureArchive;

	int					m_ConnectMode;

	_XWindowManager		m_WindowManager;
	
	LOGINPROCESS		m_LoginProcess;

	DWORD				m_PrevSendedLoginMessageTime;
	DWORD				m_PrevSendedSelectForectTime;
	
	_XCamera			m_Player;
	_XImageStatic		m_BackgroundStatic;
	_XImageStatic		m_CIBILogoImage;	
	int					m_AlphaValue;	

	BOOL				m_PrevProcess_SelectCharacter;
	
	int					m_LastSelectedServer;	
	int					m_MaxServerCount;
	
	#ifdef _X_US_EXPANDSERVERLIST
	int					m_LastSelectedRegion_Server;
	#endif

	int					m_LastUpdateServerListTime;
	_XSERVERTRAFFICDATA	m_ServerTrafficFactor[_COUNT_MAX_GROUP_];
	_XSERVERTRAFFICDATA	m_NextServerTrafficFactor[_COUNT_MAX_GROUP_];

	int					m_GraphUpdateTimer;
	
	BOOL				m_OneTimeNoticeViewFlag;
#ifdef KR_RB_DELAYSHOWNEXONNOTICEWINDOW_070530_KERYGMA
	DWORD				m_ShowNoticeWindowDelayTimer;
#endif
	
#ifdef _XNEXON_WEBLOGIN	
	DWORD				m_ShowOneTimeAutologinServerListViewDelayTimer;	
#endif
	
public:
	BOOL InitializeProcess( void );
	void DestroyProcess( void );

	bool InitDeviceObject( void );
	void ReleaseDeviceObject( void );
	
	void PauseProcess( void );
	void UnPauseProcess( void );

	void Draw( void );
	bool Process( void );
	
	void MessageProc( UINT message, WPARAM wparam, LPARAM lparam );	
	void OnTimer(WPARAM wparam, LPARAM lparam);
	void OnPaint(WPARAM wparam, LPARAM lparam);
	void OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_GetHost( UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_UDPSocket(  UINT message, WPARAM wparam, LPARAM lparam );
	bool OnKeyboardPoll(_XInput *pInput);
	bool OnKeyDown(WPARAM wparam, LPARAM lparam);
	bool OnKeyUp(WPARAM wparam, LPARAM lparam);
	bool OnMousePoll(MouseState *pState, ScrnPos *pPos);
	bool OnMouseButton(_XMouseButton p_MB, bool bPushed);	
	void OnMouseWheel( short zDelta );
	void OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam );
	void OnWindowSizeChangeNotify( void );
	void OnNexonMessangerEvent( WPARAM wparam, LPARAM lparam ); // Nexon messanger event

	void  LoginServerRequestTimeCheck( void );

	DWORD m_ServerLoginRequestTime;
	BOOL  m_ServerLoginRequestTimeCheckFlag;
	void  GameServerRequestTimeCheck( void );
	void  CloseLoginRequestDialog( void )
	{
		  m_ServerLoginRequestTime = 0;
		  m_ServerLoginRequestTimeCheckFlag = FALSE;
	}
	
	void	DrawFadeInOut( D3DCOLOR color = 0x000000 );

	void	ProcessEnvironment(void);

	BOOL	CreateOffScreen( void );

	int		GetServerList( BOOL alwaysviewwindow );
	void	RebuildServerTrafficGraph( void );
	
	_XWindowManager* GetWindowManagerPtr( void) { return &(this->m_WindowManager);	};
};

extern void __stdcall _XCallback_LoginButton( void );
extern void __stdcall _XCallback_ProgramExit( void );

#endif // !defined(AFX_XPROC_LOGINSERVER_H__F483169E_50C6_4322_B8A1_BD84751EB5D3__INCLUDED_)
