#ifndef _EODDEF_GLOBALOBJECT
#define _EODDEF_GLOBALOBJECT

#include "XSTWrapper.h"
#include "XSTWrapperCore.h"

#ifdef _XUSEFMOD
	#include "XFMODWrapper.h"
#else
	#include "XAudioSystem.h"
	using namespace Audio;
#endif

#include "XKernel.h"
#include "XLodTerrain.h"
#include "XLocalUser.h"
#include "XMob.h"
#include "XNpc.h"
#include "XGameItem.h"
#include "XBipedController.h"
#include "XAnimMeshContainer.h"
#include "XMeshContainer.h"
#include "XNetwork.h"
#include "XMessageBox.h"
#include "XWindow_NPCName.h"
#include "XProc_MainGameDef.h"
#include "EODDEF_ModelData.h"
#include "XQuestScriptManager.h"
#include "XAreaManager.h"
#include "XWordFilteringManager.h"
#include "XStringResourceManager.h"
#include "XStringDataBase.h"
#include "XParticleList.h"
#include "XScreenMotionBlur.h"
#include "XPostProcessCore.h"
#include "XPathFinder.h"
#include "XParticleEffectManager.h"
#include "XWindow_Tutorial.h"	// 2005.03.03->hotblood
#include "XHTMLWrapper.h"
#include "XMADWrapper.h"
#include "XCollectionManager.h"
#include "XResourceManager.h"
#include "XWindow_NPCBuff.h"
#include "XFame.h"
#include "XBattleLog.h"
#include "XCastleManager.h"
#include "XMatchEventManager.h"
#include "XSkillMastery.h"

//=================================================================================

// define
#ifndef					_XDEF_SOUNDEFFECTARCHIVENAME
#define					_XDEF_SOUNDEFFECTARCHIVENAME	"Sound\\SOUNDEFFECT.XP"
#endif

#ifndef					_XDEF_INTERFACESOUNDARCHIVENAME
#define					_XDEF_INTERFACESOUNDARCHIVENAME	"Sound\\INTERFACESOUND.XP"
#endif

//=================================================================================

// Texture mip level	
#define					_XDEF_TERRAINMIPLEVEL				1
#define					_XDEF_TERRAINDETAILMIPLEVEL			3
#define					_XDEF_WATERMIPLEVEL					1
#define					_XDEF_OBJECTMIPLEVEL				1
#define					_XDEF_CHARACTERMIPLEVEL				1

// 텍스쳐에 사용되는 MipMap level을 조정하여 메모리 사용량을 조절함. 
int						g_TextureMipLevel_TerrainDiffuse	=	_XDEF_TERRAINMIPLEVEL;			// 지형 Diffuse map
int						g_TextureMipLevel_TerrainDetail		=	_XDEF_TERRAINDETAILMIPLEVEL;	// 지형 Detail map, Splatting 방식으로 바뀌면서 현재는 사용 안함.
int						g_TextureMipLevel_Water				=	_XDEF_WATERMIPLEVEL;			// EMBM을 사용하지 않는 물결 Texture
int						g_TextureMipLevel_Object			=	_XDEF_OBJECTMIPLEVEL;			// 건물들에 대한 Texture
int						g_TextureMipLevel_CharacterModel	=	_XDEF_CHARACTERMIPLEVEL;		// 캐릭터와 몬스터,NPC들에 대한 Texture mipmap

//=================================================================================

// Graphic resource

_XPackageArchive		g_ModelArchive;															// 모든 3D Model(X3D) 및 Biped에 대한 파일들의 팩.
_XPackageArchive		g_ThreadModelArchive;													// Monster와 NPC는 게임 중 실시간으로 모델을 Load함. Multi-Thread loading으로 인하여 Model archive를 사용하지 않고 별도로 사용.
_XPackageArchive		g_AniArchive;															// 모든 Animation key file을 Load 하는 팩.
_XPackageArchive		g_ScriptArchive;

_XTextureManager		g_TerrainTextureArchive;												// 지형의 Texture 데이터 관련 Pack
_XTextureManager		g_WorldObjectTextureArchive;											// 지형의 건물에 관련된 Texture pack
_XTextureManager		g_RTimeObjectTextureArchive;											// 풀에 관련된 Texture pack
_XTextureManager		g_DummyObjectTextureArchive;											// 수련등과 같은 LPS등의 Object들의 Texture pack
_XTextureManager		g_EffectTextureArchive;													// Visual effect와 Env effect에 관련된 Texture pack
_XTextureManager		g_MainInterfaceTextureArchive;											// UI 용 Texture pack

HCURSOR					g_hGameCursor[_XMOUSETYPE_TOTALCOUNT];									// 윈도우용 Mouse cursor들의 대한 배열, Resource에서 Load됨.
_XBitmap*				g_MouseImage[_XMOUSETYPE_TOTALCOUNT];									// 윈도우용 Cursor를 사용하지 않고 3D 에 직접 Draw하는 Cursor들, 현재 사용안됨.
_XMOUSETYPE				g_CurrentMouseMode = _XMOUSETYPE_PEACE;									// 현재 Mouse의 Pointer type.
_XMOUSETYPE				g_PrevMouseMode = _XMOUSETYPE_BATTLE;									// 이전의 Pointer type
_XMOUSETYPE				g_ReadyMouseCursorMode = _XMOUSETYPE_PEACE;								// Process를 한바퀴 돌면서 최종적으로 결정된 Point type을 저장.
int						g_MousePointerPriorityTable[_XMOUSETYPE_TOTALCOUNT] =					// Mouse pointer type의 우선순위.
{ 
						0,0,0,0, 2,2, 1,1, 3,3,3,3, 4,4,4,4,4,4, 3,3,3,3,3,3,3,3, 5, 1,1,1,1, 2,2, 2,2, 2,2, 2,2, 2,2, 2,2
};

POINT					g_MousePointerHotspotCoord[_XMOUSETYPE_TOTALCOUNT] =					// 각 마우스의 Hot spot (focus point)에 대한 정의
{
						{ 1,1 },	// peace
						{ 1,1 },	// peace
						{ 1,1 },	// battle
						{ 1,1 },	// battle
						{ 1,1 },	// select
						{ 1,1 },	// select
						{ 4,2 },	// pickup
						{ 4,2 },	// pickupclick
						{ 9,9 },	// resize diag_left
						{ 8,9 },	// resize diag_right
						{14,3 },	// resize horiz
						{ 3,14},	// resize vert				

						{15,15},	// sand glass 1
						{15,15},	// sand glass 2
						{15,15},	// sand glass 3
						{15,15},	// sand glass 4
						{15,15},	// sand glass 5
						{15,15},	// sand glass 6

						{1,1},		// 
						{10,1},		// 
						{26,1},		// 
						{30,10},	// 
						{26,26},	// 
						{10,30},	// 
						{1,26},		// 
						{1,10},		// 
						{13,13},	// Cross pointer

						{16,15},
						{16,15},
						{ 4, 5},
						{ 7,29},
};

sDate					g_ServerGameTime;					// 서버로 부터 받은 time code
int						g_ServerTimeCode;					// g_ServerGameTime을 integer형으로 바꾼 Sec단위의 값.
DOUBLE					g_fServerTimeCode = 0.;				// g_ServerGameTime을 float형으로 바꾼 MSec단위의 값.
DWORD					g_LocalServerTimeCode;				// 서버와 시간을 비교하기 위한 현재 system time.

int						g_LastRequestCharacterNameID = 0;  // Client가 다른 Character의 이름을 못받았을때 마지막으로 캐릭터 이름을 요청한 ID
//=================================================================================

// Effect manager

_XParticleEffectManager	g_ParticleEffectManager;			// 비쥬얼 이펙트 중 화면 이펙트에 관여하는 이펙트 관리자.

_XParticleGroupManager	g_CharacterEffectGroupManager;		// 캐릭터의 이펙트를 관리하는 이펙트 관리자. VisualEffect.XES
_XParticleGroupManager	g_WorldEffectGroupManager;			// 환경 이펙트를 관리하는 이펙트 관리자.	EnvEffect.XES
_XParticleGroupInstanceManager g_WorldEffectInstanceManager;// g_WorldEffectGroupManager로부터 복사되어 생성된 월드상의 Effect, 바닥 클릭 시 생성되는 화살표같은 이펙트들을 관리.
_XParticleGroupInstanceManager g_PostProcessWorldEffectInstanceManager;	// Bloom filter에 영향을 받지 않아야 하는 Effect들을 따로 관리하는 매니져, 타격 수치 같은 이펙트들.
_XEffect_SwordTrace		g_SwordEffectController;			// 캐릭터의 검흔을 관리해주는 공통 객체.

#ifdef _XDEF_USESCREEENMOTIONBLUR 
_XScreenMotionBlur		g_ScreenMotionBlurEffectManager;	// 경공 사용시 화면의 Motion blur 기능을 관리해 줌. 눈이 아픈 관계로 기능 제외되었음.
BOOL					g_ScrrenMotionBlurMode = FALSE;
#endif

_XPostProcessCore		g_PostProcessManager;				// 화면 Bloom filter 후처리 (일명 뽀샤시)를 해주는 매니져.
_XScreenBlurCore		g_ObjectBlurProcessManager;			// 캐릭터의 모션블러를 구현하려 했었으나 현재는 사용안됨.

//=================================================================================

_XWordFilteringManager	g_WordFilteringManager;				// 욕 필터링 관련 매니져.

//=================================================================================
// Picking terrain

BOOL					g_MouseFirstClickPassed;
D3DXVECTOR3				g_vPickRayDir;
D3DXVECTOR3				g_vPickInfinityRayDir;
D3DXVECTOR3				g_vPickRayOrig;
FLOAT					g_fBary1	=	0.0f;
FLOAT					g_fBary2	=	0.0f;
FLOAT					g_fDist		=	0.0f;

D3DXVECTOR3				g_vMousePosition;
D3DXMATRIX				g_IdentMatrix;
//=================================================================================
// Cursor

HCURSOR					g_hDefaultcursor; 

//=================================================================================
// Sound effect & bgm...

#ifdef _XUSEFMOD	
		
#else
	_XAudioSystem			g_XAudioSystem;
	ISound*					g_pBGMSoundObject[3];

	int						g_MaxSoundEffectCount = _XDEF_MAXSOUNDEFFECTCOUNT;
	int						g_MaxInterfaceSoundEffectCount = _XDEF_MAXINTERFACESOUND;
	int						g_MaxEnvSoundEffectCount = _XDEF_MAXENVIRONMENTSOUND;

	ISound3D*				g_pSoundEffect[ _XDEF_MAXSOUNDEFFECTCOUNT ];
	ISound**				g_pInterfaceSound = NULL;

	_XAudioPackageArchive	g_SoundEffectArchive;
	_XAudioPackageArchive	g_EnvSoundEffectArchive;
	_XAudioPackageArchive	g_InterfaceSoundArchive;
	_XAudioPackageArchive	g_NPCSoundArchive;
	ISound3D*				g_NPCSoundObject;
#endif

#ifdef _XDWDEBUG
	BOOL				g_BgmPlayFlag	=	FALSE;
#else
	BOOL				g_BgmPlayFlag	=	!TRUE;
#endif

FLOAT					g_pBGMSoundMaxVolume	   = 0.65f;
IEnvSound3D				g_pEnvironmentSound[_XDEF_MAXENVIRONMENTSOUND];
BOOL					g_BgmPlayOnce			   = FALSE;
BOOL					g_InterfaceSoundEffectFlag = TRUE;
BOOL					g_SoundEffectFlag		   = TRUE;
BOOL					g_3DSoundEffectFlag		   = TRUE;

//=================================================================================
// Network

_XNetwork				g_NetworkKernel;								// 네트워크를 관리하는 객체.
BOOL					g_SinglePlayMode = FALSE;
BOOL					g_ZeroLevelMode = FALSE;


//=================================================================================
// Global object

_XLodTerrain			g_LodTerrain;									// 지형을 관리하는 객체.
_XLocalUser				g_CurrentUser;									// 현재 유저를 관리하는 객체.
_XHeroLocalUser			g_HeroCurrentUser;	// hotblood zerolevel		// 제로레벨 영웅용 캐릭터 객체, 현재 사용되지 않음.
_XLocalUser*			g_pLocalUser;		// hotblood zerolevel		// 로컬의 유저가 제로레벨의 영웅일때나 실제 플레이 캐릭터의 포인터가 이곳에 들어감.
int						g_CharacternamePointSize = 0;
_XList					g_Chatlist;
_XList					g_SystemMsgList;
_XList					g_CenterSystemMessageList;

_XUserList				g_Userlist;
_XMobList				g_Moblist;
_XNPCObjectManager		g_Npclist;
_XGameItemList			g_GameItemlist;

_XDummyList				g_DummyList;
_XSeatList				g_SeatList;

_XPICKINGTARGETTYPE		g_LastPickedObjectType = _XPICKINGTARGET_NONE;
FLOAT					g_LastPickedObjectDistance = 9999.0f;
POINT					g_LastPickedPoint  = { 0, 0 };
_XItem*					g_LastPickedObject = NULL;

_XQuestScriptManager	g_QuestScriptManager;			//퀘스트 스크립트
_XAreaManager			g_AreaManager;

_XCollectionManager		g_CollectionManager;
_XResourceManager		g_ResourceManager;

TCHAR					g_BgmList[_XDEF_MAXBGMCOUNT][_XDEF_MAXBGMCHAR];
int						g_FadeInOutLevel		= 0;
BOOL					g_DrawFadeInOutFlag		= FALSE;
BOOL					g_WarpProcessFadeInOutFlag	= FALSE;
BOOL					g_CameraWork = FALSE;
BOOL					g_NPCDialogScreenEffectFlag = FALSE;
BOOL					g_LMenuButtonClicked = FALSE;
BOOL					g_bNewClanQuest	= FALSE;	//봉분 해제 여부
//=================================================================================
// Script

BOOL					g_ScriptStarted = FALSE;
_XNPCObject*			g_LastNPCScriptOwner = NULL;

//=================================================================================
// FunctionalObject Script

BOOL					g_bFunctionalObjectScriptStarted = FALSE;
_XFunctionalObject*		g_LastFunctionalObject = NULL;


//=================================================================================
// ETC
#ifdef _XDWDEBUG	
BOOL					g_SpiderMode = FALSE;
#endif

BOOL					g_FreeMoveMode = FALSE;
BOOL					g_FreeMoveMode_CursorView = TRUE;
BOOL					g_EnvTimeFlowStop = FALSE;
BOOL					g_InnerUser = FALSE;

#define _XDEF_TOTALLEVELNAME			21

//=================================================================================
// Interface String pointer

#ifdef _XDEF_NEWSTRINGMANAGER

LPTSTR					g_JoinGroupName[9];
LPTSTR					g_LevelName[_XDEF_TOTALLEVELNAME];
LPTSTR					g_LevelName_CH[_XDEF_TOTALLEVELNAME];
LPTSTR					g_stringExperienceSharingForm[5];
LPTSTR					g_stringItemSharingForm[5];

LPTSTR					g_MonsterName[_XDEF_MODELCLASSCOUNT_MOB];		// Monster name
#ifdef _XJAPANESE
LPTSTR					g_MonsterHiraganaName[_XDEF_MODELCLASSCOUNT_MOB];		// Monster hiragana name
#endif

TCHAR					g_NPCName[_XDEF_MODELCLASSCOUNT_NPC][41];		// NPC Name
LPTSTR					g_TargetFullName[_XTARGET_MAX];
LPTSTR					g_UserStatus[5];
LPTSTR					g_SunMoonIconText[4];
LPTSTR					g_QuestGroupNameString[30];
LPTSTR					g_ChatEmotionString[13];
//#ifdef _XDEF_CASHITEM_SHOUT
int						g_ChatFunctionButtonStringID[_XCHATMODE_TOTALCOUNT];
//#else
//int						g_ChatFunctionButtonStringID[6];
//#endif
LPTSTR					g_pClothesItemClanName[9];
LPTSTR					g_pClothesItemSecondName[8];
LPTSTR					g_pSkillUsefulTypeName[11];
LPTSTR					g_BillingStatusString[4];		// HO:개인사용자, PC:피씨방사용자, SP:ISP사용자, PM:프리미엄 사용자
LPTSTR					g_BillingPaymentTypeString[2];	// 지불방식 AO:후불, DO:직불, PO:선불	
LPTSTR					g_BillingMethodString[2];		// 과금방식 FM:월정액, FW:주말 월 정액
LPTSTR					g_HonorName[9];
LPTSTR					g_ZoneExplaneString[9];
LPTSTR					g_szServerNameString[_COUNT_MAX_GROUP_];

#else

TCHAR					g_JoinGroupName[9][16];
TCHAR					g_LevelName[_XDEF_TOTALLEVELNAME][16];
TCHAR					g_LevelName_CH[_XDEF_TOTALLEVELNAME][16];
TCHAR					g_stringExperienceSharingForm[5][32];
TCHAR					g_stringItemSharingForm[5][32];
TCHAR					g_MonsterName[_XDEF_MODELCLASSCOUNT_MOB][41];
TCHAR					g_NPCName[_XDEF_MODELCLASSCOUNT_NPC][41];
TCHAR					g_TargetFullName[_XTARGET_MAX][16];
TCHAR					g_UserStatus[5][20];
TCHAR					g_SunMoonIconText[4][16];
TCHAR					g_QuestGroupNameString[30][16];
TCHAR					g_ChatEmotionString[13][32];
TCHAR					g_ChatFunctionButtonString[6][16];
TCHAR					g_pClothesItemClanName[9][16];
TCHAR					g_pClothesItemSecondName[8][16];
TCHAR					g_pSkillUsefulTypeName[11][16];
TCHAR					g_BillingStatusString[4][16];		// HO:개인사용자, PC:피씨방사용자, SP:ISP사용자, PM:프리미엄 사용자
TCHAR					g_BillingPaymentTypeString[2][16];	// 지불방식 AO:후불, DO:직불, PO:선불	
TCHAR					g_BillingMethodString[2][16];		// 과금방식 FM:월정액, FW:주말 월 정액
TCHAR					g_HonorName[9][16];
TCHAR					g_ZoneExplaneString[9][256]; 
TCHAR					g_szServerNameString[_COUNT_MAX_GROUP_][32];

#endif

//=================================================================================



_XNPCMotionExistTable	g_NPCMotionExistTable[_XDEF_MODELCLASSCOUNT_NPC];

// 명성 이름들
D3DCOLOR				g_HonorColor[9];
//=================================================================================
// Zone informations...
_XZone_Info				g_ZoneInfoTable[_XDEF_MAXZONECOUNT];
DWORD					g_CurrentZoneInfoIndex = 1; // 디폴트 낙양성 설정
int						g_CurrentHoveredWarpzoneIndex = -1;
int						g_ClickedWarpZoneIndex = -1;
int						g_ChannelTempZoneInfoIndex = -1; 

//////////////////////////////////////////////////////////////////////////
// 무림 명숙 PreCaching 정보
_XGuardMobPrecachingInfo g_GuardMobPrecachingInfo[_XDEF_MAXZONECOUNT];

//=================================================================================
// Flash 파일명 리스트 
TCHAR*					g_SceneList[_XDEF_MAXFLASHCOUNT];

//=================================================================================
// GUI Object
BOOL					g_ViewUserNameTooltip = TRUE;
BOOL					g_ViewMyNameTooltip = TRUE;
BOOL					g_ViewItemNameTooltip = TRUE;
BOOL					g_ViewMyBattleGauge = TRUE;
BOOL					g_ViewMonsterBattleGauge = FALSE;
BOOL					g_DenyMatch = FALSE;
BOOL					g_ChattingAutoClose = FALSE;
BOOL					g_bSelectClothesMask = TRUE;

_XToolTip				g_InterfaceTooltip;
BOOL					g_ViewGuide = TRUE;
BOOL					g_AlwaysViewUserNameAndItemTooltip = TRUE;
_XWindow_NPCName		g_NPCNameWindow;

_XToolTip				g_ItemTooltip;
_XToolTip				g_WorldObjectTooltip;
_XToolTip				g_OperaterTooltip;
_XToolTip				g_FameTooltip;
int						g_UsernameTooltipViewTimer;
int						g_ItemnameTooltipViewTimer;

_XTLVERTEX				g_ScreenBurnOut[4];

// Global utility object...
_XMessageBox			g_MessageBox;

WORD					g_CommonIntersectionIndices[24] = {0,1,2,2,3,0, 4,5,6,6,7,4,
														   0,2,1,2,0,3, 4,6,5,6,4,7 };

WORD g_CollectonWordFilter[16] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000 };
//=================================================================================
// AI Handler
_XPathFinder			g_PathFinder;

// ===============================================================================
// MAD Wrapper
#ifdef _XDEF_MADSYSTEM
_XMADWrapper			g_MadWrapper;
#endif

#ifdef _ACCLAIM_IGAADSYSTEM
#include "XIGAADWrapperCore.h"
_XIGAADWrapperCore		g_IGAADWrapper;
#endif


//=================================================================================
// Object shadow methods
LPDIRECT3DSURFACE9      g_lpShadowSurface;
LPDIRECT3DTEXTURE9      g_lpShadowTexture;
D3DXMATRIX				g_ShadowDefaultoffset;
DWORD					g_ShadowTextureSize			= 256;
DWORD				    g_CloudShadowTextureStage   = 2;
BOOL					g_bCharacterShadow			= TRUE;
BOOL					g_bBuildingCharacterShadow	= TRUE;
BOOL					g_bCloudShadow				= !TRUE;
BOOL					g_bDrawMiniHPBar			= TRUE;

D3DCOLOR				g_ShadowTextureClearColor;
D3DCOLOR				g_ShadowTextureRenderColor;

BOOL					g_bUseRealtimeGenerateObject = TRUE;
BOOL					g_bUseRealtimeGenerateObjectAni = TRUE;

WORD					g_CriticalEffectSoundIndex[2];

WORD					g_DefenceEffectSoundIndex[2][3];
WORD					g_BlowEffectSoundIndex[2][3];
WORD					g_DieEffectSoundIndex[2][2];
WORD					g_WalkEffectSoundIndex[2];

// =====================================================================
// 게임 설정 ( Game Option )
#ifdef _XDEF_KEYCHANGE_20070115
#ifdef _XTAIWANESE
BOOL					g_bGameOptionKeyTypeASelected= !TRUE;	//한자 문화권에서는 기본을 B Type으로 설정한다.
#else
BOOL					g_bGameOptionKeyTypeASelected= TRUE;
#endif
#endif

// =====================================================================
// 커뮤니티 설정 ( Community Option )
BOOL					g_bCommunityOptionRejectWhisper			= !TRUE;
BOOL					g_bCommunityOptionRejectFriend			= !TRUE;
BOOL					g_bCommunityOptionRejectParty			= !TRUE;
BOOL					g_bCommunityOptionRefuseOrganization	= !TRUE;
BOOL					g_bCommunityOptionRejectPvPTrade		= !TRUE;
BOOL					g_bCommunityOptionRejectTargetInfo		= !TRUE;

BOOL					g_bCommunityOptionMessengerAutoLogin	= TRUE;
#ifdef _XDEF_ZONECHATTING_20070108
BOOL					g_bCommunityOptionDisableZoneChatting	= !TRUE;
#endif


int						ID_SR_INTERFACE_DROP_METAL_003_WAV;
int						ID_SR_INTERFACE_DROP_METAL_002_WAV;
int						ID_SR_INTERFACE_DROP_METAL_001_WAV;
int						ID_SR_INTERFACE_DROP_LIGHT_001_WAV;
int						ID_SR_INTERFACE_DROP_HOOP_001_WAV;
int						ID_SR_INTERFACE_DROP_GLASS_DROP_001_WAV;
int						ID_SR_INTERFACE_DROP_CLOTH_001_WAV;
int						ID_SR_INTERFACE_DROP_CHOP_002_WAV;
int						ID_SR_INTERFACE_DROP_CHOP_001_WAV;
int						ID_SR_INTERFACE_COOLDOWN_WAV;
int						ID_SR_INTERFACE_CLOTHESDROP01_WAV;
int						ID_SR_INTERFACE_CHOICEICON01_WAV;
int						ID_SR_INTERFACE_CHARACTER_START_WAV;
int						ID_SR_INTERFACE_BURNANGER01_WAV;
int						ID_SR_INTERFACE_BIGBUTTONCLICK01_WAV;
int						ID_SR_INTERFACE_ANGERFIRE01_WAV;
int						ID_SR_INTERFACE_WOODDROP01_WAV;
int						ID_SR_INTERFACE_WINDOWOPEN01_WAV;
int						ID_SR_INTERFACE_WINDOWCLOSE01_WAV;
int						ID_SR_INTERFACE_WARNING01_WAV;
int						ID_SR_INTERFACE_USE_FIRE_WAV;
int						ID_SR_INTERFACE_USE_EAT_004_WAV;
int						ID_SR_INTERFACE_USE_EAT_003_WAV;
int						ID_SR_INTERFACE_USE_EAT_002_WAV;
int						ID_SR_INTERFACE_USE_EAT_001_WAV;
int						ID_SR_INTERFACE_USE_BOOK_001_WAV;
int						ID_SR_INTERFACE_TUTO_01_WAV;
int						ID_SR_INTERFACE_TARGETING00_WAV;
int						ID_SR_INTERFACE_STARTGAME_WAV;
int						ID_SR_INTERFACE_SMALLBUTTONCLICK_WAV;
int						ID_SR_INTERFACE_SLIDEBUTTON01_WAV;
int						ID_SR_INTERFACE_SKILLBOOKDROP01_WAV;
int						ID_SR_INTERFACE_SKILL_DOWN_WAV;
int						ID_SR_INTERFACE_SKILL_CLICK_WAV;
int						ID_SR_INTERFACE_REBORN_WAV;
int						ID_SR_INTERFACE_REBIRTH01_WAV;
int						ID_SR_INTERFACE_QUICKBUTTONCLICK01_WAV;
int						ID_SR_INTERFACE_QUESTSUCCESS01_WAV;
int						ID_SR_INTERFACE_QUESTFIND01_WAV;
int						ID_SR_INTERFACE_QUESTFAIL01_WAV;
int						ID_SR_INTERFACE_PICK_ITEM_001_WAV;
int						ID_SR_INTERFACE_NORMALSTART01_WAV;
int						ID_SR_INTERFACE_MONEY_01_WAV;
int						ID_SR_INTERFACE_LEVELUP01_WAV;
int						ID_SR_INTERFACE_LEVELUP_INTRO_WAV;
int						ID_SR_INTERFACE_LEVELFAIL_WAV;
int						ID_SR_INTERFACE_LEARNSKILL01_WAV;
int						ID_SR_INTERFACE_ERROR01_WAV;
int						ID_SR_INTERFACE_DROPICON01_WAV;
int						ID_SR_INTERFACE_DROP_WOOD_002_WAV;
int						ID_SR_INTERFACE_DROP_WOOD_001_WAV;
int						ID_SR_INTERFACE_DROP_WHEEP_001_WAV;
int						ID_SR_INTERFACE_DROP_SHINE_001_WAV;
int						ID_SR_INTERFACE_DROP_PAPER_001_WAV;

int						ID_SR_INTERFACE_TRA_COMBO01_WAV;
int						ID_SR_INTERFACE_MISS_01_WAV;
int						ID_SR_INTERFACE_KIPOINT_01_WAV;
int						ID_SR_INTERFACE_BUTTON_WAV;
int						ID_SR_INTERFACE_BLUE_01_WAV;
int						ID_SR_INTERFACE_WHITE_01_WAV;
int						ID_SR_INTERFACE_BLACK_01_WAV;
int						ID_SR_INTERFACE_TRA_UP01_WAV;
int						ID_SR_INTERFACE_TRA_OK01_WAV;
int						ID_SR_INTERFACE_TRA_MISS01_WAV;
int						ID_SR_INTERFACE_QUICK_USE_WAV;

int						ID_SR_INTERFACE_BOSSBUTTON00_WAV;
int						ID_SR_INTERFACE_BOSSBUTTON01_WAV;
int						ID_SR_INTERFACE_EARTHQUAKE_WAV;
int						ID_SR_INTERFACE_EARTHQUAKE01_WAV;
int						ID_SR_INTERFACE_USE_ELIXIR01_WAV;
int						ID_SR_INTERFACE_ELIXIR_RUN_WAV;
int						ID_SR_INTERFACE_ELIXIR_YES_WAV;
int						ID_SR_INTERFACE_ELIXIR_NO_WAV;
int						ID_SR_INTERFACE_OPEN_BOX_WAV;
int						ID_SR_INTERFACE_INCHANT_01_WAV;
int						ID_SR_INTERFACE_INCHANT_02_WAV;
int						ID_SR_INTERFACE_INCHANT_03_WAV;
int						ID_SR_INTERFACE_MASK_ON;
int						ID_SR_INTERFACE_MASK_OFF;
int						ID_SR_INTERFACE_CHRISTMAS_WAV;

#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER
int						ID_SR_INTERFACE_RECALL_WAV;
int						ID_SR_INTERFACE_RECALLEFFECT_WAV;
#endif

// 3D sound
int						ID_SR_INTERFACE_TURN_POINT00_WAV;

int						g_WeaponSoundCount;
int*					g_WeaponSoundIndexList = NULL;

int						g_MobWeaponSoundCount;
int*					g_MobWeaponSoundIndexList = NULL;

int*					g_ItemUseSoundIndexTable = NULL;
int						g_ItemUseSoundIndexTableSize = 0;

int						g_PVPBattleBGMStartIndex;
int						g_PVPBattleBGMEndIndex;
int						g_PVPBattleFireworkSoundIndex;

int						g_BloodInSoundIndex;
int						g_BloodOutSoundIndex;

#ifdef _XDWDEBUG
// =====================================================================
// Debug object
HINSTANCE				g_RichEditDLL = NULL;
HWND					g_hEffectScriptWindow = NULL;		// Effect script editor window handle
BOOL					g_ViewScriptWindow = FALSE;
extern					LRESULT	CALLBACK _XKernelEffectScriptWindowProc( HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam );
_XList					g_DebugOutputlist[4];
_XIMEContainer			g_DebugCommandEditContainer;
BOOL					g_ViewPathFindInfo = FALSE;
BOOL					g_EnableUserPacketFiltering = FALSE;
TCHAR					g_PacketFilteringTargetName[13];
BOOL					g_UserTracing = FALSE;
TCHAR					g_UserTraceTargetName[13];
#endif

// 아이템몰 웹 브라우져 
HWND					g_hItemMallWindow = NULL;		// Item Mall Web Browser window handle
extern LRESULT CALLBACK _XKernelItemMallWindowProc( HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam );

HWND					g_hItemMallWebBrowserObject = NULL;		// Item Mall Web Browser window handle
extern LRESULT CALLBACK _XKernelItemMallWebBrowserWindowProc( HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam );
BOOL					g_bShowItemMallWindow = FALSE;
BOOL					g_bItemMallLogOff = FALSE;
BOOL					g_bTestWebPageAddress= FALSE;

// 웹 게시판
#ifdef _XDEF_WEBBOARD_20061211
HWND					g_hWebBoardWindow = NULL;					// WebBoard Web Browser window handle
extern LRESULT CALLBACK _XKernelWebBoardWindowProc( HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam );
HWND					g_hIWebBoardWebBrowserObject = NULL;		// WebBoard Web Browser window handle
extern LRESULT CALLBACK _XKernelWebBoardWebBrowserWindowProc( HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam );
#endif

// 이벤트창
#ifdef _XDEF_EVENTWINDOW_WEBPAGE_20070214
HWND					g_hWebEventWindow = NULL;					// WebBoard Web Browser window handle
extern LRESULT CALLBACK _XKernelWebEventWindowProc( HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam );
HWND					g_hEventWebBrowserObject = NULL;		// WebBoard Web Browser window handle
extern LRESULT CALLBACK _XKernelWebEventWebBrowserWindowProc( HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam );
#endif

#ifdef _ACCLAIM_RUBICONADSYSTEM
HWND					g_hRubiconADWindow = NULL;					
extern LRESULT CALLBACK _XKernelRubiconADWindowProc( HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam );
HWND					g_hRubiconADWebBrowserObject = NULL;		
extern LRESULT CALLBACK _XKernelRubiconADWebBrowserWindowProc( HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam );
POINT					g_RubiconADWindowPosition = { 0, 0 };
SIZE					g_RubiconADWindowSize = { 0, 0 };
BOOL					g_ViewRubiconAD = TRUE;
DWORD					g_RubiconADRefreshTimer = 0;
BOOL					g_RubiconADFirstTimeView = TRUE;
int						g_RubiconADFreeLevel	= 0;
#endif

// =====================================================================
// String Resource Manager
_XStringResourceManager	g_StrManager;

// =====================================================================
// HTML Script parser
_XHTMLWrapper			g_HTMLWrapper;
_XHTMLWrapper			g_EventHTMLWrapper;	// 이벤트 공지용
_XHTMLWrapper			g_BookOpenHTMLWrapper;	// 책읽기용

// =====================================================================
// Speed Tree
BOOL					g_UseSpeedTreeShader = TRUE;
_XST_WrapperCore		g_XST_Wrapper;


// =====================================================================
// NPC Sound Lookup Table

TCHAR*					g_NPCSoundAllTable[150];
TCHAR*					g_NPCSoundLUT[150];
int						g_NPCSoundUsedTable[150];
int						g_NPCSoundCount = 0;

// =====================================================================
// Scene
int						g_indexScene = -1;


// =====================================================================
// Log
BOOL					g_ViewPacketLog = FALSE;
BOOL					g_ViewBattleLog = FALSE;

#ifdef _XGMCLIENT
BOOL					g_ViewBattleDetail = TRUE;
#else
BOOL					g_ViewBattleDetail = FALSE;
#endif


#ifdef _XDWDEBUG

BOOL					g_ViewDamageValue = TRUE;

#else

#ifdef _XTAIWANESE
BOOL					g_ViewDamageValue = TRUE;
#else
	#ifdef _XJAPANESE
		BOOL			g_ViewDamageValue = TRUE;
	#else
		#if defined(_XENGLISH) || defined(_XRUSSIAN)
			BOOL			g_ViewDamageValue = TRUE;
		#else
			BOOL			g_ViewDamageValue = FALSE;
		#endif
	#endif
#endif

#endif

// =====================================================================
// for Speed Hack Check
DWORD					g_SendTimerPacketTime = 0;

// =====================================================================
// Tutorial On/Off Check
_XTUTORIALINFO*			g_TutorialInfo = NULL;

// =====================================================================
// Poison Fog On/Off Check
BOOL					g_ViewPoisonFog = FALSE;
BOOL					g_DeleteStartViewPoisonFog = FALSE;

// =====================================================================
// Billing 관련
_XBillingInfo				g_LocalUserBillingInformations;
int							g_BillingRemainTimeCounter = 0;
BOOL						g_SendBillingRequestPacket = FALSE;

BOOL						g_ViewLocalUserBillingInfo = FALSE;

// =====================================================================
// 비무 관련
#ifdef _XDEF_NEWSTRINGMANAGER
LPTSTR						g_MatchGradeName[10];
#else
TCHAR						g_MatchGradeName[10][32];
#endif
int							g_MatchFightMoney[6];

// =====================================================================
// NPC Buff 관련
//typedef map<int, _XNPCBUFFSTR*> _map_NPCBuffStr;

int							g_NPCBuffCount = 0;
//_map_NPCBuffStr				g_NPCBuffStr;
//_XNPCBUFFSTR*				g_NPCBuffStr = NULL;
_XNPCBUFFLIST				g_NPCBuffList;

// =====================================================================
// 명성/악명 관련
_XFame*						g_FameStr;
int							g_FameStrLength = 0;

// =====================================================================
// 전투 로그 관련
_XBattleLog					g_BattleLog;

// =====================================================================
// Mastery Skill
// _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI 가 define 되었을 때 사용
_XSkillMastery				g_SkillMasteryManager;


// =====================================================================
// Mastery Skill
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_00_00[24];		// 낭인(00) - 공통(00) 

#ifdef _XTS_MASTERYOPTIONAL
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_00[24];		// 개방(01) - 공통(00)
#endif		
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_01[3][24];		// 개방(01) - 전투(01)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_02[3][24];		// 개방(01) - 치유(02)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_03[3][24];		// 개방(01) - 만능(03)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_04[3][24];		// 개방(01) - 기공(04)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_05[3][24];		// 개방(01) - 살수(05)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_06[3][24];		// 개방(01) - reserved(06)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_07[3][24];		// 개방(01) - reserved(07)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_08[3][24];		// 개방(01) - reserved(08)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_09[3][24];		// 개방(01) - reserved(09)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_10[3][24];		// 개방(01) - reserved(10)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_11[3][24];		// 개방(01) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_00[24];		// 비궁(02) - 공통(00)
#endif
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_01[3][24];		// 비궁(02) - 전투(01)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_02[3][24];		// 비궁(02) - 치유(02)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_03[3][24];		// 비궁(02) - 만능(03)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_04[3][24];		// 비궁(02) - 기공(04)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_05[3][24];		// 비궁(02) - 살수(05)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_06[3][24];		// 비궁(02) - reserved(06)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_07[3][24];		// 비궁(02) - reserved(07)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_08[3][24];		// 비궁(02) - reserved(08)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_09[3][24];		// 비궁(02) - reserved(09)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_10[3][24];		// 비궁(02) - reserved(10)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_11[3][24];		// 비궁(02) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_00[24];		// 소림(03) - 공통(00)
#endif
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_01[3][24];		// 소림(03) - 전투(01)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_02[3][24];		// 소림(03) - 치유(02)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_03[3][24];		// 소림(03) - 만능(03)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_04[3][24];		// 소림(03) - 기공(04)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_05[3][24];		// 소림(03) - reserved(05)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_06[3][24];		// 소림(03) - reserved(06)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_07[3][24];		// 소림(03) - reserved(07)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_08[3][24];		// 소림(03) - reserved(08)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_09[3][24];		// 소림(03) - reserved(09)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_10[3][24];		// 소림(03) - reserved(10)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_11[3][24];		// 소림(03) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_00[24];		// 녹림(04) - 공통(00)
#endif
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_01[3][24];		// 녹림(04) - 전투(01)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_02[3][24];		// 녹림(04) - 치유(02)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_03[3][24];		// 녹림(04) - 만능(03)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_04[3][24];		// 녹림(04) - 기공(04)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_05[3][24];		// 녹림(04) - reserved(05)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_06[3][24];		// 녹림(04) - reserved(06)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_07[3][24];		// 녹림(04) - reserved(07)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_08[3][24];		// 녹림(04) - reserved(08)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_09[3][24];		// 녹림(04) - reserved(09)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_10[3][24];		// 녹림(04) - reserved(10)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_11[3][24];		// 녹림(04) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_00[24];		// 무당(05) - 공통(00)
#endif
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_01[3][24];		// 무당(05) - 전투(01)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_02[3][24];		// 무당(05) - 치유(02)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_03[3][24];		// 무당(05) - 만능(03)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_04[3][24];		// 무당(05) - 기공(04)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_05[3][24];		// 무당(05) - reserved(05)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_06[3][24];		// 무당(05) - reserved(06)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_07[3][24];		// 무당(05) - reserved(07)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_08[3][24];		// 무당(05) - reserved(08)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_09[3][24];		// 무당(05) - reserved(09)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_10[3][24];		// 무당(05) - reserved(10)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_11[3][24];		// 무당(05) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_00[24];		// 마교(06) - 공통(00)
#endif
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_01[3][24];		// 마교(06) - 전투(01)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_02[3][24];		// 마교(06) - 치유(02)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_03[3][24];		// 마교(06) - 만능(03)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_04[3][24];		// 마교(06) - 기공(04)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_05[3][24];		// 마교(06) - reserved(05)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_06[3][24];		// 마교(06) - reserved(06)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_07[3][24];		// 마교(06) - reserved(07)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_08[3][24];		// 마교(06) - reserved(08)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_09[3][24];		// 마교(06) - reserved(09)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_10[3][24];		// 마교(06) - reserved(10)
_XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_11[3][24];		// 마교(06) - reserved(11)


// Mastery Class Name
TCHAR*				g_SkillMasteryNameProperty_00_00;			// 낭인(00) - 공통(00)

#ifdef _XTS_MASTERYOPTIONAL
TCHAR*				g_SkillMasteryNameProperty_01_00;			// 개방(01) - 공통(00)
#endif
TCHAR*				g_SkillMasteryNameProperty_01_01[3];		// 개방(01) - 전투(01)
TCHAR*				g_SkillMasteryNameProperty_01_02[3];		// 개방(01) - 치유(02)
TCHAR*				g_SkillMasteryNameProperty_01_03[3];		// 개방(01) - 만능(03)
TCHAR*				g_SkillMasteryNameProperty_01_04[3];		// 개방(01) - 기공(04)
TCHAR*				g_SkillMasteryNameProperty_01_05[3];		// 개방(01) - 살수(05)
TCHAR*				g_SkillMasteryNameProperty_01_06[3];		// 개방(01) - reserved(06)
TCHAR*				g_SkillMasteryNameProperty_01_07[3];		// 개방(01) - reserved(07)
TCHAR*				g_SkillMasteryNameProperty_01_08[3];		// 개방(01) - reserved(08)
TCHAR*				g_SkillMasteryNameProperty_01_09[3];		// 개방(01) - reserved(09)
TCHAR*				g_SkillMasteryNameProperty_01_10[3];		// 개방(01) - reserved(10)
TCHAR*				g_SkillMasteryNameProperty_01_11[3];		// 개방(01) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
TCHAR*				g_SkillMasteryNameProperty_02_00;			// 비궁(02) - 공통(00)
#endif
TCHAR*				g_SkillMasteryNameProperty_02_01[3];		// 비궁(02) - 전투(01)
TCHAR*				g_SkillMasteryNameProperty_02_02[3];		// 비궁(02) - 치유(02)
TCHAR*				g_SkillMasteryNameProperty_02_03[3];		// 비궁(02) - 만능(03)
TCHAR*				g_SkillMasteryNameProperty_02_04[3];		// 비궁(02) - 기공(04)
TCHAR*				g_SkillMasteryNameProperty_02_05[3];		// 비궁(02) - 살수(05)
TCHAR*				g_SkillMasteryNameProperty_02_06[3];		// 비궁(02) - reserved(06)
TCHAR*				g_SkillMasteryNameProperty_02_07[3];		// 비궁(02) - reserved(07)
TCHAR*				g_SkillMasteryNameProperty_02_08[3];		// 비궁(02) - reserved(08)
TCHAR*				g_SkillMasteryNameProperty_02_09[3];		// 비궁(02) - reserved(09)
TCHAR*				g_SkillMasteryNameProperty_02_10[3];		// 비궁(02) - reserved(10)
TCHAR*				g_SkillMasteryNameProperty_02_11[3];		// 비궁(02) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
TCHAR*				g_SkillMasteryNameProperty_03_00;			// 소림(03) - 공통(00)
#endif
TCHAR*				g_SkillMasteryNameProperty_03_01[3];		// 소림(03) - 전투(01)
TCHAR*				g_SkillMasteryNameProperty_03_02[3];		// 소림(03) - 치유(02)
TCHAR*				g_SkillMasteryNameProperty_03_03[3];		// 소림(03) - 만능(03)
TCHAR*				g_SkillMasteryNameProperty_03_04[3];		// 소림(03) - 기공(04)
TCHAR*				g_SkillMasteryNameProperty_03_05[3];		// 소림(03) - reserved(05)
TCHAR*				g_SkillMasteryNameProperty_03_06[3];		// 소림(03) - reserved(06)
TCHAR*				g_SkillMasteryNameProperty_03_07[3];		// 소림(03) - reserved(07)
TCHAR*				g_SkillMasteryNameProperty_03_08[3];		// 소림(03) - reserved(08)
TCHAR*				g_SkillMasteryNameProperty_03_09[3];		// 소림(03) - reserved(09)
TCHAR*				g_SkillMasteryNameProperty_03_10[3];		// 소림(03) - reserved(10)
TCHAR*				g_SkillMasteryNameProperty_03_11[3];		// 소림(03) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
TCHAR*				g_SkillMasteryNameProperty_04_00;			// 녹림(04) - 공통(00)
#endif
TCHAR*				g_SkillMasteryNameProperty_04_01[3];		// 녹림(04) - 전투(01)
TCHAR*				g_SkillMasteryNameProperty_04_02[3];		// 녹림(04) - 치유(02)
TCHAR*				g_SkillMasteryNameProperty_04_03[3];		// 녹림(04) - 만능(03)
TCHAR*				g_SkillMasteryNameProperty_04_04[3];		// 녹림(04) - 기공(04)
TCHAR*				g_SkillMasteryNameProperty_04_05[3];		// 녹림(04) - reserved(05)
TCHAR*				g_SkillMasteryNameProperty_04_06[3];		// 녹림(04) - reserved(06)
TCHAR*				g_SkillMasteryNameProperty_04_07[3];		// 녹림(04) - reserved(07)
TCHAR*				g_SkillMasteryNameProperty_04_08[3];		// 녹림(04) - reserved(08)
TCHAR*				g_SkillMasteryNameProperty_04_09[3];		// 녹림(04) - reserved(09)
TCHAR*				g_SkillMasteryNameProperty_04_10[3];		// 녹림(04) - reserved(10)
TCHAR*				g_SkillMasteryNameProperty_04_11[3];		// 녹림(04) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
TCHAR*				g_SkillMasteryNameProperty_05_00;			// 무당(05) - 공통(00)
#endif
TCHAR*				g_SkillMasteryNameProperty_05_01[3];		// 무당(05) - 전투(01)
TCHAR*				g_SkillMasteryNameProperty_05_02[3];		// 무당(05) - 치유(02)
TCHAR*				g_SkillMasteryNameProperty_05_03[3];		// 무당(05) - 만능(03)
TCHAR*				g_SkillMasteryNameProperty_05_04[3];		// 무당(05) - 기공(04)
TCHAR*				g_SkillMasteryNameProperty_05_05[3];		// 무당(05) - reserved(05)
TCHAR*				g_SkillMasteryNameProperty_05_06[3];		// 무당(05) - reserved(06)
TCHAR*				g_SkillMasteryNameProperty_05_07[3];		// 무당(05) - reserved(07)
TCHAR*				g_SkillMasteryNameProperty_05_08[3];		// 무당(05) - reserved(08)
TCHAR*				g_SkillMasteryNameProperty_05_09[3];		// 무당(05) - reserved(09)
TCHAR*				g_SkillMasteryNameProperty_05_10[3];		// 무당(05) - reserved(10)
TCHAR*				g_SkillMasteryNameProperty_05_11[3];		// 무당(05) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
TCHAR*				g_SkillMasteryNameProperty_06_00;			// 마교(06) - 공통(00)
#endif
TCHAR*				g_SkillMasteryNameProperty_06_01[3];		// 마교(06) - 전투(01)
TCHAR*				g_SkillMasteryNameProperty_06_02[3];		// 마교(06) - 치유(02)
TCHAR*				g_SkillMasteryNameProperty_06_03[3];		// 마교(06) - 만능(03)
TCHAR*				g_SkillMasteryNameProperty_06_04[3];		// 마교(06) - 기공(04)
TCHAR*				g_SkillMasteryNameProperty_06_05[3];		// 마교(06) - reserved(05)
TCHAR*				g_SkillMasteryNameProperty_06_06[3];		// 마교(06) - reserved(06)
TCHAR*				g_SkillMasteryNameProperty_06_07[3];		// 마교(06) - reserved(07)
TCHAR*				g_SkillMasteryNameProperty_06_08[3];		// 마교(06) - reserved(08)
TCHAR*				g_SkillMasteryNameProperty_06_09[3];		// 마교(06) - reserved(09)
TCHAR*				g_SkillMasteryNameProperty_06_10[3];		// 마교(06) - reserved(10)
TCHAR*				g_SkillMasteryNameProperty_06_11[3];		// 마교(06) - reserved(11)

// 물리 데미지
FLOAT				g_PD_Chr_Step_Mul[11];
FLOAT				g_PD_Chr_Level_Mul[11];
FLOAT				g_PD_Str_Mul[11];
FLOAT				g_PD_Weapon_Str_Add[11];

// 기공 데미지
FLOAT				g_MD_Chr_Step_Mul[11];
FLOAT				g_MD_Chr_Level_Mul[11];
FLOAT				g_MD_Int_Mul[11];
FLOAT				g_MD_Spell_Mul[11];
FLOAT				g_MD_Spell_Add[11];
FLOAT				g_MD_Zen_Mul[11];

// 물리 방어력
FLOAT				g_PF_Chr_Step_Mul[11];
FLOAT				g_PF_Chr_Level_Mul[11];
FLOAT				g_PF_Str_Mul[11];

// 기공 방어력
FLOAT				g_MF_Chr_Step_Mul[11];
FLOAT				g_MF_Chr_Level_Mul[11];
FLOAT				g_MF_Int_Mul[11];
FLOAT				g_MF_Zen_Mul[11];

// 물리 공격 성공률
FLOAT				g_PAR_Chr_Step_Mul[11];
FLOAT				g_PAR_Chr_Level_Mul[11];
FLOAT				g_PAR_Dex_Mul[11];

// 기공 공격 성공률
FLOAT				g_MAR_Chr_Step_Mul[11];
FLOAT				g_MAR_Chr_Level_Mul[11];
FLOAT				g_MAR_Int_Mul[11];

// 물리 회피율
FLOAT				g_PDR_Chr_Step_Mul[11];
FLOAT				g_PDR_Chr_Level_Mul[11];
FLOAT				g_PDR_Dex_Mul[11];

// 기공 회피율
FLOAT				g_MDR_Chr_Step_Mul[11];
FLOAT				g_MDR_Chr_Level_Mul[11];
FLOAT				g_MDR_Int_Mul[11];

#ifdef _XTS_BASEMINMAXWEAPONDAMAGE
// 물리 무기 기본 공격력
FLOAT				g_PD_Base_Min_Weapon_Mul[10];
FLOAT				g_PD_Base_Max_Weapon_Mul[10];
#endif

#ifdef _XTS_NEWBATTLEPARAMETER
FLOAT				g_PAB_Str_Mul[11];		// 물리 공격 균형 : Attack Balance
FLOAT				g_PAB_dex_Mul[11];		// 물리 공격 균형 : Attack Balance
FLOAT				g_MAB_Int_Mul[11];		// 기공 공격 균형
FLOAT				g_CRI_AR_Dex_Mul[11];	// 치명타 성공 확률
FLOAT				g_CRI_AR_Int_Mul[11];	// 치명타 성공 확률

FLOAT				g_PF_Vital_Mul[11];		// 물리 방어력
FLOAT				g_PAR_Str_Mul[11];		// 물리 공격 성공률
FLOAT				g_MAR_Dex_Mul[11];		// 기공 공격 성공률
FLOAT				g_MDR_Dex_Mul[11];		// 기공 회피율
FLOAT				g_PAB_Int_Mul[11];		// 물리 공격 균형
FLOAT				g_CRI_MAR_Dex_Mul[11];	// 치명타 성공 확률
#endif


// 유료화 아이템 패키지 테이블 
#ifdef _XTS_ITEMMALLBROWSER
_XCashItemPackageManager				g_CashItemPackageManger;
#endif


// 베트남 셧다운 
#ifdef _XDEF_SHUTDOWN_20061027
DWORD				g_PlayTime = 0;
DWORD				g_PlayLeftTime = 0;
#endif

// 지역 대화 딜레이 타임 ( 한번 쓰면 10초 후에 다시 사용할 수 있다. )
#ifdef _XDEF_ZONECHATTING_20070108
DWORD				g_DelayTimeZoneChatting	= 0;
BOOL				g_bEnableZoneChatting	= TRUE;	
BOOL				g_bProgressZoneChatting	= FALSE;	//현재 지역대화 채팅으로 설정되어 있음을 알리는 변수
#endif

// 장원전 관련 DS
_XCastleManager		g_CBManager;

// 맵의 세율
int					g_nMapTaxRate = 0;

// 비무 대회 
_XMatchEventManager	g_MatchEventManager;


//////////////////////////////////////////////////////////////////////////
// Interface texture name

TCHAR				g_MI_CharTextureName[32];
TCHAR				g_MI_KiUpTextureName[32];
TCHAR				g_MI_LifeSkillTextureName[32];

#endif
