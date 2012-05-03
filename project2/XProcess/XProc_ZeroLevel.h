// XProc_ZeroLevel.h: interface for the XProc_ZeroLevel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPROC_ZEROLEVEL_H__C7F96E72_E70F_4F3B_8C1B_0A1399E1A028__INCLUDED_)
#define AFX_XPROC_ZEROLEVEL_H__C7F96E72_E70F_4F3B_8C1B_0A1399E1A028__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XProc_MainGameDef.h"
#include "XLodTerrain.h"
#include "XRenderStateControl.h"
#include "xmob.h"
#include "XNPC.h"
#include "XHeroLocalUser.h"
#include "XWindow_Inventory.h"
#include "XWindow_NPCScript.h"//2004.07.01->oneway48 insert

#define MAXSPEECHCOUNT 55	// 한 라인의 최대글자수 

// ID 성별(남자0,여자1) 얼굴(부정0,긍정1) 머리(0) 상의 하의 모자 왼쪽팔(아대) 오른쪽팔(아대) 왼쪽다리(신발) 오른쪽다리(신발) 무기 무공
struct _APPEARANCETYPE
{
	int gender;
	int face;
	int hair;
	int ubody;
	int lbody;
	int hat;
	int lhand;
	int rhand;
	int lfoot;
	int rfoot;
	int weapon;
	int skill;
	int attackhitvalue; 
	int health;	
};

struct _ZEROLEVEL_CHAR_INFO
{
	int id;
	float posx;
	float posy;
	int jobindex;			// 역할
	int appearanceindex;	// 외모
	float rotangle;
};

class XProc_ZeroLevel : public _XProcess  
{
private:
	// Loading Process -----------------------------------------------┓
	_XTextureManager			m_TextureArchive;

	_XCamera					m_Player;
	_XStatic					m_BackgroundStatic;
	_XStatic					m_ProgressStatic;
	_XStatic					m_ProgressBarStatic;

	POINT						messagecoord;
	TCHAR						m_Message[256];
			
	BOOL						m_DataLoaded;
	
	int							m_BacgroundTextureIndex;
	int							m_ProgressTextureIndex;
		
	int							m_Progress;
	
	_XTextureManager			m_HeroCharacterTextureArchive;
	_XBipedController			m_ZeroLevelHerobipedModel[9];
	_XAnimMeshAbstractClass*	m_pZeroLevelHeroModel[9];

	unsigned short				m_MobCount;		
	FLOAT						m_fAIDelayTime;
	_XMobList					m_MobList;
	_APPEARANCETYPE*			m_pAppearanceList;
	_ZEROLEVEL_CHAR_INFO*		m_pZerolevelCharacterList;

	_XGROUPTYPE					m_HeroGroupType;
	_XGROUPTYPE					m_EnemyGroupType;
	int							m_iTotalMobCount;
	_XGENDERTYPE				m_HeroGender;
	_XTimeCounter				m_ZeroSystemTimeCounter;
	FLOAT						m_fElapsedFrameTime;
	FLOAT						m_fLocalSystemTime;
	DWORD						m_LocalSystemTime;

	void AddMob();		
	void LoadZeroLevelInfo();	

	void HeroCharacterLoading();
	void HeroCharacterLoadingInitDeviceObject();
	void HeroCharacterLoadingDestroyProcess();
	bool HeroCharacterLoadingProcess( void );
	void HeroCharacterDraw( void );
	int  GetAppearanceIndex(int index);
	void HeroStateSetting();

	// 이벤트 관련
	float						m_fRotateFirstSceneAngle;
	BOOL						m_bFirstEventSceneEnd;
	BOOL						m_bEndEventSceneStart;
	BOOL						m_bTalkSceneEnd;
	BOOL						m_bHeroTalking;
	BOOL						m_bEnemyHeroTalking;
	BOOL						m_bMouseButtonFlag;
	BOOL						m_bEnemyHeroMotioinFlag;
	D3DXVECTOR3					m_vEnemyHeroPos;
	int							m_iEventState;
	FLOAT						m_fEventCtrlTime;
	FLOAT						m_fEventDelayTime;
	_XImageStatic				m_TalkHeroBackImage[2];
	_XImageStatic				m_TalkHeroImage[2];
	BOOL						m_TalkHeroImageOverlap;
	BOOL						m_TalkEnemyHeroImageOverlap;
	RECT						m_TalkHeroImageSecondRect;
	_XImageStatic				m_NextClickImage;
	int							m_NextClickImageCount;
	int							m_NextClickImageCountRate;
	FLOAT						m_fNextClickTime;

	TCHAR*						m_HeroSpeech[9];
	TCHAR*						m_EnemyHeroSpeech[9];
	TCHAR*						m_CurrentHeroSpeech[3];
	TCHAR*						m_CurrentEnemyHeroSpeech[3];
	TCHAR*						m_CurrentTalkStageArray;
	int							m_CurrentTalkStage;
	int							m_iCurrentHeroLine;
	int							m_iCurrentEnemyHeroLine;
	int							m_SpeechPositionCounter;	
	DWORD						m_SpeechPositionTimeConter;
	int							m_iHeroImgageDrawPosX;
	int							m_iHeroImgageDrawPosY;
	int							m_iEnemyHeroImgageDrawPosX;
	int							m_iEnemyHeroImgageDrawPosY;
	float						m_iEventCameraDistance;
	BOOL						m_bAllEventEnd;
	int							m_iEventFadeAlphaLevel;
	int							m_iCurrentTutorial;
	BOOL						m_bTutorialModeOn;
	FLOAT						m_fHelpTime;
	FLOAT						m_fHelpLimiteTime;
	
	void FirstScene();
	void EventScene();
	void DrawEventScene();
	void TextSpeechHero();
	void SpeechHero();
	void SpeechEnemyHero();
	void TutorialMode();
	//----------------------------------------------------------------┛
	
public:
	XProc_ZeroLevel();
	virtual ~XProc_ZeroLevel();

	_XTextureManager*	GetTextureZero( void ) { return &m_TextureArchive;	};
	BOOL GetEndEventSceneStart() { return m_bEndEventSceneStart;	};
	BOOL GetFirstEventSceneEnd() { return m_bFirstEventSceneEnd; };
	BOOL GetAllEventEnd() { return m_bAllEventEnd; };
	int  GetCurrentTutorial() { return m_iCurrentTutorial;	};
	BOOL GetTutorialModeOn() { return m_bTutorialModeOn; 	};
	void SetTutorialModeOn( BOOL modeon ) { m_bTutorialModeOn = modeon;	};
	
	void SetHeroGroupIndex(_XGROUPTYPE index) { m_HeroGroupType = index; }; // hotblood 20040901	
	void SetEnemyGroupIndex(_XGROUPTYPE index) { m_EnemyGroupType = index; };	// hotblood 20040901
	void SetHeroCharacterGender(_XGENDERTYPE gender) { m_HeroGender = gender; };

	BOOL InitializeProcess( void );
	void DestroyProcess( void );

	bool InitDeviceObject( void );
	void ReleaseDeviceObject( void );
	
	void PauseProcess( void );
	void UnPauseProcess( void );

	void Draw( void );
	bool Process( void );	
	
	void MessageProc( UINT message, WPARAM wparam, LPARAM lparam );	
	void OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam );	
	void OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam );
	bool OnKeyboardPoll(_XInput *pInput);
	bool OnKeyDown(WPARAM wparam, LPARAM lparam);
	bool OnKeyUp(WPARAM wparam, LPARAM lparam);
	bool OnMousePoll(MouseState *pState, ScrnPos *pPos);
	bool OnMouseButton(_XMouseButton p_MB, bool bPushed);
	void OnMouseWheel( short zDelta );
	void OnMouseLBtnDblClick( WPARAM wparam );
	void OnMouseRBtnDblClick( WPARAM wparam );
	void OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam );
	void OnWindowSizeChangeNotify( void );	

	void UpdateProgress( int increase );

	void SetMessage( LPTSTR message );

	// Interface data-------------------------------------------------------------------------
	_XWindow_Inventory*		m_pInventoryWindowPtr;

	DWORD					m_CurrentStringWidth;
	FLOAT					m_CurrenStringScrollEffectProgress;
	int						m_CurrenStringScrollEffectDirection;
	int						m_CurrenStringScrollEffectPosition;
	FLOAT					m_CurrenStringScrollEffectSpeed;	
		
// Sound data-----------------------------------------------------------------------------	
	
	// Interface sound
	
	// Game sound effect	
	BOOL					m_BgmPlaying;	

	int						m_CurrentBGMType;		// 0 이면 기본 배경 1이면 환경 사운드
	DWORD					m_BGMPlayTimer;
	int						m_ChangingBGM;
	TCHAR					m_NextBGMSoundName[32];
	
// Timer data-----------------------------------------------------------------------------
	U32						m_timerMove;
	float					m_fMoveFactor;
	DWORD					m_LastKeyDownTime;
	DWORD					m_LastPickedItemTime;
	
// User data------------------------------------------------------------------------------
	BOOL					m_EnableAttack;
	BOOL					m_ProcessLocalUserDieFlag;
	BOOL					m_UserDieScreenEffectFlag;
	int						m_UserDieScreenAlphaLevel;	
	
	void					ProcessLocalUserDie(void);

	BOOL					LoadHeroDescript( LPTSTR szFilename, int selectedhero );		// hotblood->2004.09.03

// Utility methods------------------------------------------------------------------------
	BOOL					m_FirstEntered;
	BOOL					m_SendedRequestRestart;
	
	// Mouse...	
	int						m_DuplicateCoordCount;		
	POINT					m_LastPickingPoint;
	
	//_X3DPickedPrimitive m_PickedPrimitive;
	
	void					GetVectorFromMousePos( POINT& pt, D3DXVECTOR3& raydir, D3DXVECTOR3& rayorg );
	BOOL					GetMousePosition( void );
	void					SetMovetoMousePickedPoint( void );

	// Exception 

	BOOL					m_ProcessStopFlag;
	DWORD					m_ProcessStopTime;
	void					Process_ProcessStop( void );
	
	BOOL					m_bGotoLoginProcess; //2004.06.29->oneway48 insert
	BOOL					m_GameExitFlag;
	BOOL					m_GameExitReadyFlag;
	DWORD					m_GameExitSignalTime;

	DWORD					m_LastTextureHitrateCheckTime1;
	DWORD					m_LastTextureHitrateCheckTime2;
	void					CheckTextureUseHitrate( void );

// Terrain data----------------------------------------------------------------------------
public:		
	BOOL					LoadTerrain( LPSTR Filename );	

// Renderer handler------------------------------------------------------------------------
public:
	void					InitializeRenderState( void );	
	
#ifdef _XADMINISTRATORMODE
	_XRenderStateControl	m_RenderStateController;

#endif	
#ifdef _XDWDEBUG	
	BOOL					m_ViewRuntimeDebugData;	
	BOOL					m_ViewFPS;	
	BOOL					m_ViewShadowTexture;
	void					DrawDebugState( void );

	_XMeshContainer			m_ArrowModel;
	BOOL					m_ViewArrowModel;
	BOOL					m_ViewMouseRay;

	LPD3DXMESH				m_pLightConeMesh;
	int						m_LightConeTextureIndex;
	BOOL					m_ViewLightConeModel;
	
	BOOL					m_ViewPathObject;

#endif

// Camera handler--------------------------------------------------------------------------
public:
	BOOL					m_RotateAccelerate;
	FLOAT					m_CameraRotateSpeed;		
	int						m_RotateYaw;
	int						m_RotatePitch;

	D3DXVECTOR3				m_vTempCameraPosition, m_vTempTargetPosition;
	float					m_fTempTargetDistance;
	float					m_fTempFov, m_fTempPitch, m_fTempYaw;
	
// Light handler---------------------------------------------------------------------------
public:
	
	void					InitializeLight( void );	

// Fog handler-----------------------------------------------------------------------------
public:	

// Interface handler-----------------------------------------------------------------------
	int						m_ToolTipEffectRepeater;
	BOOL					m_ToolTipEffectRepeaterFlag;

	void					DrawInterface( void );		
	void					ProcessInterface( void );
	BOOL					LoadInformationString( void );	
	
// Game process handler--------------------------------------------------------------------
	int						m_PrevZoneInfoIndex;
	int						m_CurrentWarpZoneIndex;	
	BOOL					m_LockMoveProcess;	

// Message proc handler--------------------------------------------------------------------

	void					FSWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					QuickSlotMessageHandling(WPARAM wparam, LPARAM lparam);
	void					OptionMenuWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					OptionWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					MinimapWindowMessageHandling( WPARAM wparam, LPARAM lparam );	
	void					SystemMenuWindowMessageHandling(WPARAM wparam, LPARAM lparam);	

// Battle function --------------------------------------------------------------------
public :
	BOOL					SelectChargeSkill(short skillid, BOOL keydown = TRUE);
	BOOL					SelectActiveSkill(short skillid);		
	BOOL					CheckDelayTime(short skillid);
};

#endif // !defined(AFX_XPROC_ZEROLEVEL_H__C7F96E72_E70F_4F3B_8C1B_0A1399E1A028__INCLUDED_)
