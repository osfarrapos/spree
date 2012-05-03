// XProc_MainGame.h: interface for the XProc_MainGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPROC_MAINGAME_H__A7EB9391_2F00_49ED_87CE_87E0E4569F23__INCLUDED_)
#define AFX_XPROC_MAINGAME_H__A7EB9391_2F00_49ED_87CE_87E0E4569F23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XProc_MainGameDef.h"
#include "XWindow_CInfo.h"
#include "XWindow_QuestWindow.h"
#include "XWindow_Inventory.h"
#include "XWindow_WorldMinimap.h"
#include "XWindow_NPCSkill.h"
#include "XLodTerrain.h"
#include "XBillBoard.h"
#include "XRenderStateControl.h"
#include "XUser.h"
#include "XNPC.h"
#include "XWindow_NPCTrade.h"
#include "XWindow_PCTrade.h"
#include "XWindow_NPCScript.h"//2004.07.01->oneway48 insert
#include "XWindow_MainFrame.h"
#include "Xwindow_JinStatus.h"
#include "Xwindow_ChattingDefault.h"
#include "XWindow_LifeSkill.h"


//	Author : 양희왕 
#ifdef _XDEF_NOTICE_STRING_PROCESS 
#include "Notice/XNoticeProcess.h"
#endif

#ifdef _ACCLAIM_IGAADSYSTEM
	typedef enum _XACCLAIMAD_MAINGAME
	{
		_XACCLAIMAD_MAINGAME_NONE = 0,
		_XACCLAIMAD_MAINGAME_NPC,
		_XACCLAIMAD_MAINGAME_AREACHANGE,
		_XACCLAIMAD_MAINGAME_MEDITATION,
		_XACCLAIMAD_MAINGAME_DEATH
	};
	
	typedef enum _XACCLAIMAD_DEATHPHASE
	{
		_XACCLAIMAD_DEATHPHASE_NONE = 0,
		_XACCLAIMAD_DEATHPHASE_STATICAD,
		_XACCLAIMAD_DEATHPHASE_VIDEOAD,
		_XACCLAIMAD_DEATHPHASE_RESULTVIEW
	};

#endif

class _XWindow_Pocket;
class _XWindow_Hedek;
class _XWindow_PVPBroker;
class _XWindow_PersonalStore;
class _XWindow_AddSaveItem;
class _XWindow_NPCCollection;
class _XWindow_NPCResource;
class _XWindow_NPCBuff;
class _XWindow_MoveVillage;
class _XWindow_NPCResource_New;

struct _XST_MoveZoneInfo
{
	int		targetzone;			// 이동할 지역 index
	int		nextwarpzone;		// 이동할 지역의 warp index (area)
	int		currentwarpzone;	// 현재 있는 곳의 warp index(area)

	_XST_MoveZoneInfo()
	{
		targetzone = -1;
		nextwarpzone = -1;
		currentwarpzone = -1;
	}
};

class _XGraphPoint : public _XItem
{
public:
	DWORD level;
};

//#define _XDEF_NEWSPECIALBGMPROCESS_071123_KERYGMA

class XProc_MainGame : public _XProcess  
{
public:
	XProc_MainGame();
	virtual ~XProc_MainGame();

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
	void OnMouseMBtnDblClick( WPARAM wparam );
	void OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam );
	void OnWindowSizeChangeNotify( void );
	void OnPaint(WPARAM wparam, LPARAM lparam);

// Common data----------------------------------------------------------------------------
public:
	
// Interface data-------------------------------------------------------------------------
	// Chatting...	
	_XIMEContainer			m_ChatEditContainer;
	TCHAR					m_LastSendedChatMessage[_XDEF_CHATMAXLENGTH];
	
	TCHAR					m_LastMsg[_XDEF_CHATMAXLENGTH];			// 중복처리 --=
	int						m_OverlappingMsgCount;	// -----------=
	DWORD					m_LastSendChatMessageTime;	// 마지막으로 채팅 메시지를 보낸 시간

	
	_XWindow_NPCScript*		m_pNPCScriptWindowPtr;
	_XWindow_NPCTrade*		m_pNPCTradeWindowPtr;
	_XWindow_Warehouse*		m_pWarehouseWindowPtr;
	_XWindow_PCTrade*		m_pPCTradeWindowPtr;
	_XWindow_NPCSkill*		m_pNPCSkillWindowPtr;
	_XWindow_SocketPlugIn*  m_pSocketPlugInWindowPtr;
	_XWindow_Contribution*  m_pContributionWindowPtr;
	_XWindow_RepairItem*	m_pRepairItemWindowPtr;
	_XWindow_Pocket*		m_pPocketItemWindowPtr;
	_XWindow_Hedek*			m_pHedekWindowPtr;
	_XWindow_PersonalStore*	m_pPersonalStoreWindowPtr;
	_XWindow_Gamble*		m_pGambleBoxWindowPtr;
	_XWindow_SocketPlugInExp* m_pSocketPlugInExWindowPtr;
	_XWindow_Inchant*		m_pInchantWindowPtr;
	_XWindow_PVPBroker*		m_pPVPBrokerWindowPtr;
	_XWindow_AddSaveItem*	m_pAddSaveItemWindowPtr;
	_XWindow_NPCCollection*	m_pNPCCollectionWindowPtr;
	_XWindow_NPCResource*	m_pNPCResourceWindowPtr;
	_XWindow_NPCResource_New* m_pNPCResource_New_windowptr;
	_XWindow_NPCBuff*		m_pNPCBuffWindowPtr;
	_XWindow_MoveVillage*	m_pMoveVillageWindowPtr;
	
	_XWindow_MainFrame*		m_pMainFrameWindowPtr;

	DWORD					m_CurrentStringWidth;
	FLOAT					m_CurrenStringScrollEffectProgress;
	int						m_CurrenStringScrollEffectDirection;
	int						m_CurrenStringScrollEffectPosition;
	FLOAT					m_CurrenStringScrollEffectSpeed;
	DWORD					m_InfoHelperStartTimer;
	BOOL					m_InfoHelperSystemView;
	DWORD					m_InfoHelperScrollTimer;
	DWORD					m_InfoHelperInsertTimer;
	_XStringItem*			m_pInfoHelperString;
	_XList					m_InformationStringList;
	int						m_LastInsertedInformationStringIndex;
	DWORD					m_LastWeaponChangeTime;
	DWORD					m_CurrentEnvironmentTime;
	
	// 게임상에서 숫자를 표시할 때 쓰이는 이미지( 시간표시...)
	_XImageStatic			m_NumberSmallImage[12];

	// 보스전일때 독무 
	int						m_imagePoisonFogIndex;
	
#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
	int						m_GameGuardAuthenticationCount;
	BOOL					m_CompleteGameGuardAuth;
	BOOL					m_CancleGameGuardAuth;
#endif
	
// Sound data-----------------------------------------------------------------------------	
	
	// Interface sound
	
	// Game sound effect
	int						m_CurrentBGMType;		// 0 이면 기본 배경 1이면 환경 사운드
	DWORD					m_BGMPlayTimer;
	int						m_ChangingBGM;
	void					ProcessChangeBgm( void );
	void					ProcessBgmTimer( void );

#ifdef _XDEF_NEWSPECIALBGMPROCESS_071123_KERYGMA
	BOOL					m_PreviousBGMPlayFlag;
	BOOL					m_SpecialBGMLoopPlayFlag;
	DWORD					m_SpecialBGMPlayTimer;
#endif
	BOOL					m_SpecialBGMPlayMode;
	TCHAR					m_SpecialBGMFileName[64];	

#ifdef _XDEF_NEWSPECIALBGMPROCESS_071123_KERYGMA
	void					SetSpecialBGMPlay( LPTSTR filename, BOOL loop = FALSE );
#else
	void					SetSpecialBGMPlay( LPTSTR filename );
#endif

	void					StopSpecialBGMPlay( void );
	
// Timer data-----------------------------------------------------------------------------
	U32						m_timerMove;
	float					m_fMoveFactor;
	DWORD					m_LastKeyDownTime;
	DWORD					m_LastPickedItemTime;
	DWORD					m_PCRoomConnectNoticeTimer;
	BOOL					m_ExperiencePCRoomConnect;
	void					ProcessPCRoomConnectNotice( void );
	
// User data------------------------------------------------------------------------------
	BOOL					m_EnableAttack;
	BOOL					m_ProcessLocalUserDieFlag;
	BOOL					m_UserDieScreenEffectFlag;
	int						m_UserDieScreenAlphaLevel;

	void					ProcessLocalUserDie(void);
	
// Mouse handler--------------------------------------------------------------------------	
	

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
	void					SendSignal_ProcessStop( void );
	void					Process_ProcessStop( void );
	
	BOOL					m_bGotoLoginProcess; 
	BOOL					m_GameExitFlag;
	DWORD					m_GameExitSignalTime;
	BOOL					m_ProgramExitFlag;
	void					Process_GameExit( void );
	
	void					SetNormalNoticeMessage( LPSTR message );
	void					ViewNormalNoticeMessage( void );

	DWORD					m_LastTextureHitrateCheckTime1;
	DWORD					m_LastTextureHitrateCheckTime2;
	DWORD					m_LastMeshHitrateCheckTime;
	void					CheckTextureUseHitrate( void );

// Terrain data----------------------------------------------------------------------------
public:		
	BOOL					LoadTerrain( LPSTR Filename );

// Renderer handler------------------------------------------------------------------------
public:
	void					InitializeRenderState( void );	
	
#ifdef _XADMINISTRATORMODE
	_XRenderStateControl	m_RenderStateController;
	void					ProcRenderStateCommand( LPSTR commandstring );	
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

	DWORD					m_LastGraphUpdateTime;
	DWORD					m_GraphLogList[10][600];
	DWORD					m_GraphLogListInsertPoint;
	void					DrawGraphLog( void );

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
	void					DrawPickedObjectInfoTooltip( void );	
	void					ProcessInterface( void );
	BOOL					LoadInformationString( void );
	void					ProcessInformationSystem( void );
	void					CheckLandMarkPoint( void );
	
// Game process handler--------------------------------------------------------------------
	int						m_PrevZoneInfoIndex;
	int						m_CurrentWarpZoneIndex;
	int						m_NextWarpZoneIndex;
	DWORD					m_PrevCheckedWarpingZoneTimer;
	BOOL					m_MoveServer;		// 네트워크 재접속 전까지 유효
	BOOL					m_MovingGameServer;	// 서버 이동후 게임에 들어 올때 까지 유효 
	BOOL					m_LockMoveProcess;	
	DWORD					m_WarpStartTime;
	BOOL					m_NoDisplayZoneInfo;     // 비무가 한번 끝났을때 지역표시 않함. 
	
	int						m_NextWarpZoneIndex_COMMONGATE; // 통합워프존에 서 마지막 선택한 본당인덱스

// Script handler--------------------------------------------------------------------------
		
	int						StartNPCScript( void );
	void					ProcessNPCScriptAutoPopUp(void);

#ifdef _ACCLAIM_IGAADSYSTEM
	_XACCLAIMAD_MAINGAME	m_AcclaimADMode_MainGame;
	_XACCLAIMAD_DEATHPHASE  m_AcclaimADMode_DeathPhase;
	DWORD					m_DeathADResultViewCounter;
	BOOL					m_DeathADRecievedResult;
	
	DWORD                   m_DeathAD_Gold;     // 지급 금액
	UCHAR                   m_DeathAD_ItemCnt;  // 아이템 개수 
	_ADItemInfo*			m_pDeathAD_ADIteminfo;	// ucItemCnt 개수만큼 반복

	BOOL					m_DeathADRecievedResultAutoCancelCounter;
	int						m_DeathStaticBackgroundAlphaLevel;

	_XImageStatic			m_AcclaimAdResultTitleLeftBar;	
	_XImageStatic			m_AcclaimAdResultTitleCenterBar;	
	_XImageStatic			m_AcclaimAdResultTitleRightBar;
	_XImageStatic			m_AcclaimAdResultItemIconStatic;
	
	int						m_ADScreeenDimmedLevel;
	BOOL					m_UseNPCAD;
#endif
	
#ifdef _ACCLAIM_RUBICONADSYSTEM	
	void					SetUIforRubiconADSystem( void );
#endif
	
	int						m_indexNPCSubScriptQA;
	BOOL					m_bNPCDialogWideView;
	BOOL					m_NPCDialogScreenEffectFlag;
	int						m_NPCDialogScreenAlphaLevel;
	BOOL					m_bNPCFadeInOutFlag;
	BOOL					m_bNPCItemTrade;
	BOOL					m_bAutoNPCScriptPopUp;
	DWORD					m_NPCWideViewExitTimer;//2004.07.06->oneway48 insert

	_XNPCObject*			m_TempNPCObject;
	BOOL					m_bNPCDialogMode;		// 대화할 때 PC가 Rendering 되지 않도록 하기 위한 변수 

	float					m_NPCWideViewMove;
	float					m_NPCWindowMoveFast;
	float					m_NPCWindowMoveSlow;
	
	_XTLVERTEX				m_GridTopScreenVertex[4];
	_XTLVERTEX				m_GridBottomScreenVertex[4];
	
	void					DrawNPCFadeInOut();
	void					DrawNPCDialogWideView();
	void					DrawNPCWindowMove();//2004.07.01->oneway48 insert	
	void					SetNPCMode(_XNPC_MODE npcmode);//2004.07.01->oneway48 insert
	void					WindowCloseBeforeWideView();//2004.07.01->oneway48 insert
	void					BaseInterfaceWindowOpen(); // Author : 양희왕 // breif : 윈도우 오픈

	BOOL					m_bNPCTalkLog;
	BOOL					m_bMoveNPCWindow;
//	BOOL					m_bMiniGameStart;
//	BOOL					m_bMiniGameDraw;	

#ifdef _XDEF_NOTICE_STRING_PROCESS	//	Author : 양희왕 
	_XNoticeProcess*		m_pXNoticeString;
#endif

// Tutorial Manager ------------------------------------------------------------------------=
	void					LoadTutorialInfo( void );
	void					SaveTutorialInfo( void );

// Event Notice ----------------------------------------------------------------------------=
	void					LoadEventNoticeInfo( void );
	void					SaveEventNoticeInfo( void );

// FunctionalObject Script handler--------------------------------------------------------------------------
	BOOL					m_bFunctionalObjectWideView;
	BOOL					m_bAutoFunctionalObjectScriptPopUp;
	BOOL					m_bFunctionalObjectFadeInOut;
	BOOL					m_bExitFunctionalObjectWideView;

	_XFunctionalObject*		m_TempFunctionalObject;
	
	int						StartFunctionalObjectScript( void );
	void					DrawFunctionalObjectWideView();
	void					DrawFunctionalObjectFadeInOut();
	
// Message proc handler--------------------------------------------------------------------

	void					FSWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					QuickSlotMessageHandling(WPARAM wparam, LPARAM lparam);
	void					OptionMenuWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					OptionWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					ChatWindowMessageHandling( WPARAM wparam, LPARAM lparam );
	void					CIWindowMessageHandling( WPARAM wparam, LPARAM lparam );
	void					MinimapWindowMessageHandling( WPARAM wparam, LPARAM lparam );
	
	void					InventoryMessageHandling(WPARAM wparam, LPARAM lparam);	

	void					PartyWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					QuestWindowMessageHandling(WPARAM wparam, LPARAM lparam);			
	void					SkillWindowMessageHandling(WPARAM wparam, LPARAM lparam);	
	void					NPCScriptWindowMessageHandling(WPARAM wparam, LPARAM lparam);	
	void					NPCTradeWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					NPCSkillWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					TradeConfirmWindowMessageHandling(WPARAM wparam, LPARAM lparam);	
	void					RequestPartyWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					PCTradeWindowMessageHanlding(WPARAM wparam, LPARAM lparam);

	void					WarehouseWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					AddSaveItemWindowMessageHandling(WPARAM wparam, LPARAM lparam);

	void					MessengerWindowMessageHandling(WPARAM wparam, LPARAM lparam); 
	void					MessengerTooltipWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					NameInputWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					EmailWindowMessageHandling(WPARAM wparam, LPARAM lparam); 
	void					MessengerDialogWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					TutorialDialogWindowMessageHandling(WPARAM wparam, LPARAM lparam);	// 2005.03.03->hotblood
	void					QuestMailWindowMessageHandling(WPARAM wparam, LPARAM lparam);	// 2005.03.03->hotblood

	void					SystemMenuWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					RebirthWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					RepairItemWindowMessageHandling(WPARAM wparam, LPARAM lparam);//2004.06.17->oneway48 insert
	void					SocketPlugInWindowMessageHandling(WPARAM wparam, LPARAM lparam);//2004.11.9->hotblood insert
	void					SocketPlugInExpandWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					PocketWindowMessageHandling(WPARAM wparam, LPARAM lparam);//2005.02.25->hotblood insert
	
	void					MeditationMessageHandling(WPARAM wparam, LPARAM lparam);// hotblood
	void					TrainingMessageHandling(WPARAM wparam, LPARAM lparam);
	void					EnterBossRoomMessageHandling(WPARAM wparam, LPARAM lparam);
	void					NickNameMessageHandling(WPARAM wparam, LPARAM lparam);
	
	void					MatchWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					MatchTablePVPWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					MatchNoticeWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	
	void					MatchPRWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					MatchRecordWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	
	void					TargetInfoWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					MainFrameWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	
	void					MasterySkillWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					ContributionMessageHandling(WPARAM wparam, LPARAM lparam);
	void					TutorialIconMessageHandling(WPARAM wparam, LPARAM lparam);
	void					HelpBookMessageHandling(WPARAM wparam, LPARAM lparam);
	
	void					GMCommandMessageHandling(WPARAM wparam, LPARAM lparam);
	void					OrganizationWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					GambleBoxWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					PersonalStoreWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					LifeSkillWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					EventWindowMessageHandling(WPARAM wparam, LPARAM lparam);	
	void					CashItemWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					NPCCollectionWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					NPCResourceWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					InchantWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	
	void					NewNPCResourceWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	
	void					NPCBuffWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					PKRecordWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					CashItemBoxWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	
	void					CashItemUseMesssageHandling(WPARAM wparam, LPARAM lparam);	// cash item 사용 관련
	void					MoveVillageWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	
	void					CastleWarWindowMessageHandling(WPARAM wparam, LPARAM lparam);
	void					PKTracingRequestWindowMessageHandling(WPARAM wparam, LPARAM lparam);//PK위치 추적 관련

	void					SmeltItemWindowMessageHandling(WPARAM wparam, LPARAM lparam);	//용루 용린 관련
	void					NewWorldMapWindowMessageHandling(WPARAM wparam, LPARAM lparam ); //신구룡전도 관련
	void					RecallItemWIndowMessageHandling( WPARAM wparam, LPARAM lparam ); //친구소환 관련
	void					SocketDetachWIndowMessageHandling( WPARAM wparam, LPARAM lparam ); //소켓 제거 관련
	
	void					MixWeaponWindowMessageHandling(WPARAM wparam, LPARAM lparam);	// 무기 조합
	
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	void					BlackListWindowMessageHandling(WPARAM wparam, LPARAM lparam);
#endif

#ifdef _XDWDEBUG
	void					DebugWindowMessageHandling(WPARAM wparam, LPARAM lparam);
#endif

	// Screen Effect function ---------------------------------------------------------
	void					DrawFadeInOut(void);

// Quest Screen Effect function -------------------------------------------------------
public :
	_XTLVERTEX				m_TopScreenVertex[4];
	_XTLVERTEX				m_BottomScreenVertex[4];
	
	void					DrawQuestWideView(void);
	void					RepositionTopBottomVertex(void);

// Battle function --------------------------------------------------------------------
public :	
	BOOL					SelectSkill(short skillid, BOOL keydown = TRUE);
	BOOL					ChangeToActive(short skillid);
	BOOL					ChangeToChanneling(short skillid, BOOL keydown);
	BOOL					ChangeToHiddenMA(short skillid, BOOL keydown);
	BOOL					ChangeToOther(short skillid, BOOL keydown);
	BOOL					ChangeToPassive(short skillid);
	
	// skill 등록/사용하기 전에 조건 검사
	BOOL					CheckValidSkillActive(short skillid);
	BOOL					CheckValidSkillChanneling(short skillid);
	BOOL					CheckValidSkillHiddenMA(short skillid);
	BOOL					CheckValidSkillOther(short skillid);
	BOOL					CheckValidSkillPassive(short skillid);

	BOOL					CheckDelayTime(short skillid);
	BOOL					CheckTargetType(short skillid);
	BOOL					CheckReqCondition(short skillid);
	BOOL					CheckReqItem(short skillid);
// Virtual Space function
	void					SetEnterVirtualSpace(void);// 가상공간에 들어갈 때 세팅하는 함수
	void					SetLeaveVirtualSpace(void);// 가상공간에서 나올때 세팅하는 함수

	
// BossRoom function --------------------------------------------------------------------
	BOOL					CheckBossMap(int targetzoneindex);
	void					SetEnterBossBattleRoom(void);// 생사결에 들어갈 때 세팅하는 함수
	void					SetExitBossBattleRoom(void);// 생사결에서 나올때 세팅하는 함수

// Messenger handler ------------------------------------------------------------------
public :
	DWORD					m_MessengerConnectTimer; 
	DWORD					m_MessengerLoginTimer; 

	// 사후 처리 function -----------------------------------------------------------------
public :
	void					DrawDyingState(void);
	void					DrawComaState(void);
	void					ProcessDyingState(void);
	void					ProcessComaState(void);

	// 종료후 로그인시 캐릭터 정보 Reset
	void					ResetData();//2004.06.19->oneway48 insert

	// 수련 관련 초기화
	void					InitializeTraining(void);
	
	// 인터페이스 초기화
	void					InitializeInterface(void);
	
	// 빌링 처리
	void					ProcessBilling( void );
	
	// PK Zone 메세지 처리
	DWORD					m_dwMapEnteringTime;
	
	// pk zone으로 이동할 때 target/warpzone index 저장하는 str
	_XST_MoveZoneInfo		m_strMoveZoneInfo;
	
	// 자리비움 세팅을 위한 변수
	DWORD					m_dwLastInputTime;			// 마지막으로 키/마우스 입력 받은 시간
	
	// NPC script 창 닫기 - 예외 처리
	void					CloseNPCScriptWindow();

};


// Interface callback functions -------------------------------------------------------
extern void __stdcall _XCallback_QS_Button1( _XIMEContainer* pIMEContainer );
extern void __stdcall _XCallback_QS_Button2( _XIMEContainer* pIMEContainer );
extern void __stdcall _XCallback_QS_Button3( _XIMEContainer* pIMEContainer );
extern void __stdcall _XCallback_QS_Button4( _XIMEContainer* pIMEContainer );
extern void __stdcall _XCallback_QS_Button5( _XIMEContainer* pIMEContainer );
extern void __stdcall _XCallback_QS_Button6( _XIMEContainer* pIMEContainer );
extern void __stdcall _XCallback_QS_Button7( _XIMEContainer* pIMEContainer );
extern void __stdcall _XCallback_QS_Button8( _XIMEContainer* pIMEContainer );
extern void __stdcall _XCallback_QS_Button9( _XIMEContainer* pIMEContainer );


extern void __stdcall _XIMECallback_ChatReturn( _XIMEContainer* pIMEContainer );
extern void __stdcall _XIMECallback_ChatEscape( _XIMEContainer* pIMEContainer );
extern void __stdcall _XIMECallback_ChatTab( _XIMEContainer* pIMEContainer );
extern void __stdcall _XIMECallback_ChatUpArrow( _XIMEContainer* pIMEContainer );
extern void __stdcall _XIMECallback_ChatDownArrow( _XIMEContainer* pIMEContainer );
extern void __stdcall _XIMECallback_ChatLeftArrow( _XIMEContainer* pIMEContainer );
extern void __stdcall _XIMECallback_ChatRightArrow( _XIMEContainer* pIMEContainer );



#endif // !defined(AFX_XPROC_MAINGAME_H__A7EB9391_2F00_49ED_87CE_87E0E4569F23__INCLUDED_)


