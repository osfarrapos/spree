
#if !defined(AFX_EMPEROROFDRAGONS_H__785380DA_DB6C_4B1F_A438_1D6C9065ED3E__INCLUDED_)
#define AFX_EMPEROROFDRAGONS_H__785380DA_DB6C_4B1F_A438_1D6C9065ED3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786) 

#include "resource.h"
#include "XKernel.h"

extern _XTextureManager		g_TerrainTextureArchive;

// Game process...
#include "XProc_NineDragonsTitle.h"
#include "XProc_FirstLoad.h"
#include "XProc_LoginServer.h"
#include "XProc_CreateCharacter.h"
#include "XProc_SelectCharacter.h"
#include "XProc_Loading.h"
#include "XProc_MainGame.h"
#include "XProc_Flash.h"
#include "XProc_SelectHero.h"	//  hotblood Zerolevel
#include "XProc_ZeroLevel.h"	// hotblood Zerolevel
#include "XProc_ServerUnification.h"

BOOL CheckCalledLauncher( void );

class CEmperorOfDragonsApp : public X3DApp
{
// public functions ////////////////////////////////////////////////////////
public:
	CEmperorOfDragonsApp();	
	~CEmperorOfDragonsApp();

// public members //////////////////////////////////////////////////////////
public:
	// INITIALIZATION ////////////////////////////////////
	bool Startup( void );
	void Shutdown( void );
	
	bool InitGameObjects( void );
	void ReleaseGameObjects( void );
		
	bool	InitDeviceObjects( void );
	HRESULT RestoreDeviceObjects( void );	
	HRESULT InvalidateDeviceObjects( void );
	HRESULT DeleteDeviceObjects( void );	

	void InitializeStringData( void );
	bool CreateCharacterModelContainer( void );

	// EVENT MANAGEMENT //////////////////////////////////
	bool KeyboardPoll(_XInput *pInput);
	bool KeyDown(WPARAM wparam, LPARAM lparam);
	bool KeyUp(WPARAM wparam, LPARAM lparam);

	bool MousePoll(MouseState *pState, ScrnPos *pPos);
	bool MouseButton(_XMouseButton p_MB, bool bPushed);
	void OnMouseMove( WPARAM wparam, LPARAM lparam );
	bool MouseWheel( WPARAM wparam );
	void MouseLBtnDblClick( WPARAM wparam );
	void MouseRBtnDblClick( WPARAM wparam );
	void MouseMBtnDblClick( WPARAM wparam );
	
	bool WndMessageHandler( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
	void MessageHandler( UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_GetHost( UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_UDPSocket(  UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam );
	void PaintHandler( void );
	void MCINotify( UINT message, WPARAM wparam, LPARAM lparam );
	void OnWindowSizeChangeNotify( void );
	void OnTimer(WPARAM wparam, LPARAM lparam);
	void OnPaint(WPARAM wparam, LPARAM lparam);	
	void OnMove(WPARAM wparam, LPARAM lparam);	
	void OnNexonMessangerEvent( WPARAM wparam, LPARAM lparam ); // Nexon messanger event

	HRESULT	ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior, D3DFORMAT backBufferFormat );

	// SCENE MANAGEMENT //////////////////////////////////
	bool Process();
	bool Render();

	// Cursor ////////////////////////////////////////////	
	void SetCursor( DWORD type, BOOL forcesetting = FALSE );
	void SetClickCursor( void );
	void ReleaseClickCursor( void );
	void ResetReadyCursor(  DWORD type  );
	void ReadyCursor(  DWORD type  );
	
	// ETC ///////////////////////////////////////////////	
	static BOOL CheckData( void );
	static BOOL ReadyPackageArchive( void );
	static void FinalizePackageArchive( void );

	void ChangeLanguage( _XLANGUAGE_TYPE language );

	static BOOL LoadSubLanguageList( void );
public:

	BOOL					m_InitializedApp;

	_XProcessManager		m_ProcessManager;


	// Game process---------------------------------------------------------
	XProc_NineDragonsTitle  m_proc_ninedragonstitle;
	XProc_FirstLoad			m_proc_firstload;
	XProc_LoginServer		m_proc_loginserver;
	XProc_SelectCharacter	m_proc_selectcharacter;
	XProc_CreateCharacter	m_proc_createcharacter;
	XProc_Loading			m_proc_loading;
	XProc_MainGame			m_proc_maingame;
	XProc_Flash				m_proc_flash;
	XProc_ZeroLevel			m_proc_zerolevel;	// hotblood zerolevel
	XProc_SelectHero		m_proc_selecthero;	// hotblood zerolevel	
	XProc_ServerUnification	m_proc_serverunification;
	
// protected functions /////////////////////////////////////////////////////
protected:	

// protected members ///////////////////////////////////////////////////////
protected:

    void VolumeControl( int value );
	void VolumeControlUp( void );
	void VolumeControlDown( void );
		
// private functions ///////////////////////////////////////////////////////
private:
	

// private members /////////////////////////////////////////////////////////
private:

};

extern void _XUpdateWindow( void );
extern BOOL _XCheckValidPopupPage( void );

#ifdef _XDEF_ITEMMALL_AUTOLOGIN_SE_20070514
long WINAPI _XDisplayHTMLPageAndPostData(HWND hwnd, LPCTSTR webPageName, LPCTSTR postData );
#endif

typedef struct _XMEMORYCHECKDATA
{
	// ----------------------------------------------------------------------
	// item section
	unsigned short					_usCurDur;				// 현재 내구력
	unsigned short					_usMaxDur;				// 맥스 내구력

	char							_cType;					// 아이템 대분류
	char							_cSecond;
	short							_sID;					// 아이템 소분류 (아이디)
	char							_cIdentify;				// 감정 상태
	char							_cThird;
	unsigned char					_ucCount;				// 최대 머지 255개

	short							_sSocket[4];
	unsigned char					_ucStrength;			// 무기의 경우 제련 수치.
	unsigned char					_ucSharpness;			// 무기의 경우, 예기.
	unsigned char					_ucWeaponStrength;		// 무기의 경우, 강도 //Author : 양희왕
	unsigned char   				_ucSocketCount;			// 소켓이 몇개가 열렸는지.
	BOOL							_bDurabilityDecrease;	

	unsigned short					_usCurProtect : 5;		// 무기의 경우, 바른 용린의 수. 0 - 15
	unsigned short					_usCurMaxProtect : 5;	// 최대강도
	//unsigned char					_ucIncProtect : 3;		// 최대강도 중가분.
	unsigned short					_usUsedProtect : 1;		// 용루를 사용한 적이 있는가.

	int								_BloodPoint;			// 혈정
	int								_FameAttrib;			// -1 : 프리 0 : 명성 1 : 악명
	DWORD							_Money;					// 현금

	// ----------------------------------------------------------------------
	// skill section


	// ----------------------------------------------------------------------
	// character section
	_XGROUPTYPE						groupindex;							 // 캐릭터 문파
	_XGENDERTYPE					gender;								 // 캐릭터 성별

	short							constitution;						 // 건강 <= 생기 
	short							zen;								 // 진기 <= 그대로 
	short							intelligence;						 // 지혜 <= 영기 
	short							dexterity;							 // 민첩 <= 심기 
	short							strength;							 // 근력 <= 정기	

	short							current_lifepower;					 // 현재 생명
	short							current_forcepower;					 // 현재 내공
	short							current_concentrationpower;			 // 현재 의지
	
	// 최대 Power
	short							max_lifepower ;						 // 최대 생명
	short							max_forcepower ;					 // 최대 내공
	short							max_concentrationpower ;			 // 최대 의지

	short							externalwoundrate;					 // 외상 
	short							internalwoundrate;					 // 내상 
	unsigned short					fatiguerate;						 // 피로도 
	
	short							furyparameter;						// 분노
	
	short							leftpoint;							 // 잔여 기포인트
	short							leftmasterypoint;					// 잔여 마스터리 포인트

	short							retribution;						 //	업보
	int								honor;								//명성  8
	short							showdown;							 // 비무 전적
	
	// 유저 레벨	
	DWORD							jinlevel;							 // 진 축적치
	int								Notoriety;							 // 악명

	DWORD							level;								 // 유저 내공 단계 : 응신입기혈 ~ XXX
	DWORD							sublevel;							 // 유저 12성
	DWORD							innerlevel;							 // 내공 수위 : 진이 쌓이면 올라감.4

	char							cClass;								 // 역할
	char							cClassGrade;						 // 역할 등급
	int								iContribution;			   			 // 문파 기여도

	int								gm_level;

	unsigned char					weaponStrength;						// 무기 제련 수치 
	
	void							ResetData(void)
	{
		_usCurDur				= _XMEMORYCHECKDATA_ENCWORD(0);	
		_usMaxDur				= _XMEMORYCHECKDATA_ENCWORD(0);	

		_cType					= (char)_XMEMORYCHECKDATA_ENCBYTE(0);	
		_cSecond				= (char)_XMEMORYCHECKDATA_ENCBYTE(0);
		_sID					= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		_cIdentify				= (char)_XMEMORYCHECKDATA_ENCBYTE(0);	
		_cThird					= (char)_XMEMORYCHECKDATA_ENCBYTE(0);
		_ucCount				= (unsigned char)_XMEMORYCHECKDATA_ENCBYTE(0);	

		_sSocket[0]				= (short)_XMEMORYCHECKDATA_ENCWORD(0);
		_sSocket[1]				= (short)_XMEMORYCHECKDATA_ENCWORD(0);
		_sSocket[2]				= (short)_XMEMORYCHECKDATA_ENCWORD(0);
		_sSocket[3]				= (short)_XMEMORYCHECKDATA_ENCWORD(0);

		_ucStrength				= (unsigned char)_XMEMORYCHECKDATA_ENCBYTE(0);	
		_ucSharpness			= (unsigned char)_XMEMORYCHECKDATA_ENCBYTE(0);	
		_ucWeaponStrength		= (unsigned char)_XMEMORYCHECKDATA_ENCBYTE(0);	
		_ucSocketCount			= (unsigned char)_XMEMORYCHECKDATA_ENCBYTE(0);	
		_bDurabilityDecrease	= (BOOL)_XMEMORYCHECKDATA_ENCINT(0);	

		_usCurProtect			= 0;
		_usCurMaxProtect		= 0;
		//_ucIncProtect			= 0;
		_usUsedProtect			= 0;

		_BloodPoint				= (int)_XMEMORYCHECKDATA_ENCINT(0);	
		_FameAttrib				= (int)_XMEMORYCHECKDATA_ENCINT(0);	
		_Money					= (DWORD)_XMEMORYCHECKDATA_ENCINT(0);	
		
		groupindex				= (_XGROUPTYPE)_XMEMORYCHECKDATA_ENCINT(0);		
		gender					= (_XGENDERTYPE)_XMEMORYCHECKDATA_ENCINT(0);	

		constitution			= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		zen						= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		intelligence			= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		dexterity				= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		strength				= (short)_XMEMORYCHECKDATA_ENCWORD(0);	

		current_lifepower		= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		current_forcepower		= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		current_concentrationpower = (short)_XMEMORYCHECKDATA_ENCWORD(0);
		
		
		max_lifepower			= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		max_forcepower			= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		max_concentrationpower  = (short)_XMEMORYCHECKDATA_ENCWORD(0);	

		externalwoundrate		= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		internalwoundrate		= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		fatiguerate				= (unsigned short)_XMEMORYCHECKDATA_ENCWORD(0);	
		
		furyparameter			= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		
		leftpoint				= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		leftmasterypoint		= (short)_XMEMORYCHECKDATA_ENCWORD(0);	

		retribution				= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		honor					= (int)_XMEMORYCHECKDATA_ENCINT(0);	
		showdown				= (short)_XMEMORYCHECKDATA_ENCWORD(0);	
		
		
		jinlevel				= (DWORD)_XMEMORYCHECKDATA_ENCINT(0);	
		Notoriety				= (int)_XMEMORYCHECKDATA_ENCINT(0);	

		level					= (DWORD)_XMEMORYCHECKDATA_ENCINT(0);	
		sublevel				= (DWORD)_XMEMORYCHECKDATA_ENCINT(0);	
		innerlevel				= (DWORD)_XMEMORYCHECKDATA_ENCINT(0);	

		cClass					= (char)_XMEMORYCHECKDATA_ENCBYTE(0);	
		cClassGrade				= (char)_XMEMORYCHECKDATA_ENCBYTE(0);	
		iContribution			= (int)_XMEMORYCHECKDATA_ENCINT(0);	

		gm_level				= (int)_XMEMORYCHECKDATA_ENCINT(0);

		weaponStrength			= (unsigned char)_XMEMORYCHECKDATA_ENCBYTE(0);	
	}

} _XMEMORYCHECKDATA;

extern _XMEMORYCHECKDATA	g_MemoryCheckData_0;
extern _XMEMORYCHECKDATA	g_MemoryCheckData_1;
extern _XMEMORYCHECKDATA	g_MemoryCheckData_2;
extern _XMEMORYCHECKDATA	g_MemoryCheckData_3;
extern _XMEMORYCHECKDATA	g_MemoryCheckData_4;
extern _XMEMORYCHECKDATA	g_MemoryCheckData_5;

#endif // !defined(AFX_EMPEROROFDRAGONS_H__785380DA_DB6C_4B1F_A438_1D6C9065ED3E__INCLUDED_)

