// XLocalUser.h
//
// Author : Iljun, Park  /  Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#ifndef _XLOCALUSER_H_
#define _XLOCALUSER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XObjectManager.h"
#include "XLodTerrain.h"
#include "XActionTable.h"
#include "XBipedController.h"
#include "XGameItem.h"
#include "XSkillItem.h"
#include "XWindow_ChatBalloon.h"
#include "XUser.h"
#include "XPathFinder.h"
#include "XCharacterState.h"
#include "XCastleBaseType.h"

#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장
	#define _XDEF_WAREHOUSE_TOTALCOUNT		60
#else
	#define _XDEF_WAREHOUSE_TOTALCOUNT		40
#endif

#define	_XDEF_IMPACTPOWER		0.5f
#define _XDEF_FOVINCREASERATE	3.0f
#define _XDEF_FOVDECREASERATE	5.0f					
#define _XDEF_MAXFOV			(g_DefaultCameraFov + 30.0f)

// ---------------------------------------------------------------------------------------
// 유저 관련 enumeration type

// 인벤토리 슬롯 타입
typedef enum tagInventoryslotType
{
	_XINVENTORY_SLOTTYPE_AMOR=0,			// 0  장착될 경우 Pocket 공간을 봐야한다. 
	_XINVENTORY_SLOTTYPE_PANTS,				// 1  장착될 경우 Pocket 공간을 봐야한다. 
	_XINVENTORY_SLOTTYPE_ADAE,				// 2
	_XINVENTORY_SLOTTYPE_SHOES,				// 3
	_XINVENTORY_SLOTTYPE_CAP,				// 4
	_XINVENTORY_SLOTTYPE_GAP_UPPER,			// 5
	_XINVENTORY_SLOTTYPE_GAP_LOWER,			// 6
	_XINVENTORY_SLOTTYPE_MASK,				// 7  마스크
	_XINVENTORY_SLOTTYPE_CLOTH_RESERVE1,	// 8  의상 예약 분.
	_XINVENTORY_SLOTTYPE_CLOTH_RESERVE2,	// 9  의상 예약 2.
	
	_XINVENTORY_SLOTTYPE_WEAPON_1,			// 10
	_XINVENTORY_SLOTTYPE_WEAPON_2,			// 11
	
	_XINVENTORY_SLOTTYPE_ACCESSORY_1,		// 12 위의 0,1번 Pocket공간에 따라서 위에서부터 활성화 되어야한다.  상의용...
	_XINVENTORY_SLOTTYPE_ACCESSORY_2,		// 13
	_XINVENTORY_SLOTTYPE_ACCESSORY_3,		// 14
	_XINVENTORY_SLOTTYPE_ACCESSORY_4,		// 15
	_XINVENTORY_SLOTTYPE_ACCESSORY_5,		// 16 하의용 악세사리.
	_XINVENTORY_SLOTTYPE_ACCESSORY_6,		// 17
	_XINVENTORY_SLOTTYPE_ACCESSORY_7,		// 18
	_XINVENTORY_SLOTTYPE_ACCESSORY_8,		// 19	
	
	_XINVENTORY_SLOTTYPE_EXTRA_POCKET,		// 20 소지품 장착 공간.
	
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	_XINVENTORY_SLOTTYPE_CASHBACKPACK1,			// 21 //Author : 양희왕 //breif : 캐시 행낭 추가
	_XINVENTORY_SLOTTYPE_CASHBACKPACK2,			// 22
#else
	_XINVENTORY_SLOTTYPE_RESERVE1,				// 21
	_XINVENTORY_SLOTTYPE_RESERVE2,				// 22
#endif

	_XINVENTORY_SLOTTYPE_BACKPACK1,			// 23
	_XINVENTORY_SLOTTYPE_BACKPACK2,			// 24 백팩 두번째.

	_XINVENTORY_SLOTTYPE_POCKETSTART = 25,				// 25	주머니 시작위치 
	_XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART = 50,		// 45	배낭   시작위치
	_XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART = 75,		// 70	배낭2  시작위치
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	_XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART = 100,		
	_XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART = 125,
	
	_XINVENTORY_SLOTTYPE_TOTALCOUNT = 150,
	_XINVENTORY_SLOTTYPE_MONEYINDEX = 224
#else
	_XINVENTORY_SLOTTYPE_TOTALCOUNT = 100,
	_XINVENTORY_SLOTTYPE_MONEYINDEX = 127 // 돈 인덱스
#endif		

} _XINVENTORY_SLOTTYPE;

// 전투 상태
typedef enum tagUserStateSeq
{
	_XUSER_STATE_NONE = 0,
	_XUSER_STATE_1,
	_XUSER_STATE_2,
	_XUSER_STATE_3,
	_XUSER_STATE_4,
	_XUSER_STATE_5,
	_XUSER_STATE_6,
	_XUSER_STATE_7,
	_XUSER_STATE_8,
	_XUSER_STATE_9,
	_XUSER_STATE_10,
	_XUSER_STATE_LAST
}_XUSER_STATE_SEQ;

// 애니매이션 순서
typedef enum tagAnimationOrder
{
	_XANI_ORDER_READY = 0,
	_XANI_ORDER_REPEAT,
	_XANI_ORDER_FIRE 
}_XANIMATION_ORDER;

typedef enum tagTroubleState
{
	_XDEF_CONVSTATE_NORMAL = 0,
	_XDEF_CONVSTATE_FURY,
	_XDEF_CONVSTATE_CONVERGENCE
}_XUSER_TROUBLE_STATE;

typedef enum tagUserState
{
	_XDEF_USERSTATE_NORMAL = 0,
	_XDEF_USERSTATE_TRAINING,				// 수련
	
	_XDEF_USERSTATE_MATCH_REQUEST,			// 비무 신청중/신청받은중

	_XDEF_USERSTATE_MATCH_MAKETABLE,		// 비무첩 작성중
	_XDEF_USERSTATE_MATCH_WAITING,			// 기다리고 있음

	_XDEF_USERSTATE_MATCH_MATCHING,			// 비무 시작
	_XDEF_USERSTATE_MATCH_BOW,				// 포권
	_XDEF_USERSTATE_MATCH_POSITIONING,		// 자리 싸움
	_XDEF_USERSTATE_MATCH_COUNTING,			// 시작 카운트중
	_XDEF_USERSTATE_MATCH_FIGHT,			// 비무중

	_XDEF_USERSTATE_MATCH_END,				// 비무 종료 - 대기중
	_XDEF_USERSTATE_MATCH_RESULT,			// 비무 종료 - 결과받고 10초간
	_XDEF_USERSTATE_MATCH_WAITEXIT,			// 비무 종료 - 비무장 나갈수 있음

	_XDEF_USERSTATE_PK_READY,				// PK 준비 상태
	_XDEF_USERSTATE_PK_FIGHT,				// PK 상태
	_XDEF_USERSTATE_PK_END					// PK End
}_XUSER_STATE;

// 수련 종류 - 서버와 맞아야 함
typedef enum tagTrainingState
{
	_XTRAINING_NONE = 0,
	_XTRAINING_MEDITATION,		// 운기 조식
	_XTRAINING_DUMMY,			// 더미 치기
	_XTRAINING_SPEED,			// 경공
	_XTRAINING_SPELL,			// 기공
	_XTRAINING_SAND,
	_XTRAINING_THROW,
	_XTRAINING_GROUP,
	_XTRAINING_CLOSED,

	// by mahwang
	_XTRAINING_FIREWOODCUT,
	_XTRAINING_WATERDRAW,

}_XTRAINING_CLASS;

// 애니매이션 Reference table
enum tagMatchAnimation
{
	_XAN_MATCH_START_MALE_NONE = 0,
	_XAN_MATCH_START_MALE_SHORTWEAPON,
	_XAN_MATCH_START_MALE_LONGWEAPON,
	_XAN_MATCH_MALE_CANCEL,

	_XAN_MATCH_START_FEMALE_NONE,
	_XAN_MATCH_START_FEMALE_SHORTWEAPON,
	_XAN_MATCH_START_FEMALE_LONGWEAPON,
	_XAN_MATCH_FEMALE_CANCEL,

	// 낭인 부법 대기 자세
	_XAN_IDLE_AXE_MALE,
	_XAN_IDLE_AXE_FEMALE
};

// 외치기 mode
enum tagShoutMode
{
	_XSHOUT_MODE_NONE = 0,		// 외치기 불가
	_XSHOUT_MODE_WHITE = 96,		// 백도 외치기
	_XSHOUT_MODE_BLACK = 97,		// 흑도 외치기
	_XSHOUT_MODE_ALL = 98,		// 전체 외치기
	_XSHOUT_MODE_CLAN = 99		// 문파 외치기
};


// ---------------------------------------------------------------------------------------
// 로컬 유저의 행동 로그
typedef struct _XUSERACTIONLOG
{
	// 전투 여부
	BOOL		isAttack;
	int			attackTime;
	int			attackTargetType;
	
	// 몬스터 히트 여부
	BOOL		isHit;
	int			hitTime;
	int			hitTargetType;
	
	// 몹 죽음 여부( 서버용 )
	BOOL		isKill;
	int			killTime;
	int			killTargetType;
	
	// 아이템 파괴 여부( 내구도로 인한 파괴 )
	BOOL		isDestroyItem;
	int			DestroyItemTime;
	char		DestroyItemType;
	short		DestroyItemID;
	
	// 몬스터 변신 여부
	BOOL		isChange;
	int			changeTime;
	int			changeTargetType;
	int			changeTargetUniqueID;
	
	// 레벨업
	BOOL		isLevelUp;
	int			levelupTime;
	
	// 거래
	BOOL		isTrade;
	int			tradeTime;
	
	// 대화
	BOOL		isTalk;
	int			talkTime;
	
	// 죽음
	BOOL		isDead;
	int			deadTime;
	
	// 부활
	BOOL		isRevive;
	int			reviveTime;
	
	_XUSERACTIONLOG()
	{
		isAttack = isLevelUp = isTrade = isTalk = isDead = isRevive = isKill = isHit = isChange = isDestroyItem =FALSE;
		attackTime = levelupTime = tradeTime = talkTime = deadTime = reviveTime = killTime = hitTime = killTime = DestroyItemTime = 0;
		killTargetType = attackTargetType = hitTargetType = changeTargetType = DestroyItemID = 0;
		changeTargetUniqueID = DestroyItemType = -1;
	}		
}_XUSERACTIONLOG;	


// ----------------------------------------------------------------------------------------------
// 파티 정보
typedef struct _XUSERPARTYINFO
{
	unsigned short	playeruniqueid;
	
	short			posX;
	short			posZ;
	
	short			innerlevel;

	char			serverno;
		
	char			vitalpercent;
	char			zenpercent;

	char			clan;			// 문파
	char			clanclass;		// 역할
    short           nClassGrade;    // 직책 등급

	bool			bOut;			// 자리 비움 flag
	bool			bFarAway;		// 거리 이탈 flag

	int				honor ;			// 명성

	int				partycount;		// 파티 퀘스트에서 자기가 가지고 있는 파티카운터의 총합 
	
	BOOL			isLeader;		// 파티장인지를 알 수 있는 변수 
	BOOL			isValid;		// 파티원 정보가 있는지를 알 수 있는 변수 
	BOOL			isMVP;			// 파티 퀘스트에서 자기 자신이 MVP인지를 알 수 있는 변수

	TCHAR			playername[13];	
	
	_XUSERPARTYINFO()
	{
		memset(playername, 0, sizeof(TCHAR)*13);
		serverno = posX = posZ = innerlevel = vitalpercent = zenpercent = playeruniqueid = clan = clanclass = honor = 0;
		isLeader = isValid = isMVP = FALSE;
		partycount = 0;
		bOut = bFarAway = false;
	}
	
}_XUSERPARTYINFO;

// 아이템 타일
typedef struct _XTileItem
{
	short		sX;
	short		sZ;
} _XTILE_ITEM;

// 더미 치기 수련 관련
struct _XDUMMY_INFO
{
	short		skillid;
	bool		bWarning;
	bool		bWarningSkillState;
	bool		bComplete;
	char		cTrainingGrade;
	char		cReserved;
};

// 기공 수련 관련
struct _XSPELL_INFO : public _XDUMMY_INFO
{
	bool		bSetTargetPosition;
	void		Reset( void )
	{
				skillid = 0;
				bWarning = false;
				bWarningSkillState = false;
				bSetTargetPosition = false;
				cTrainingGrade = 0;
	}
};

class _XMovePathData : public _XItem
{
public:
	D3DXVECTOR3	nexttargetposition;
	/*
public:
	_XMovePathData()
	{		
		Reset();
	}
	void Reset( void )
	{
		nexttargetposition.x = 0.0f;
		nexttargetposition.y = 0.0f;
		nexttargetposition.z = 0.0f;
	}
	*/
};

// 유저의 파라미터 관련
class _XSU_Parameter
{
	_XDEF_ENCVALUE(short,org);
	_XDEF_ENCVALUE(short,result);

	_XDEF_ENCVALUE(char,bBuff);

	_XDEF_ENCVALUE(char,applyCount);
	_XDEF_ENCVALUE(char,applyCountAdditional);

	FLOAT		bonus;

#ifdef _XTS_BASEMINMAXWEAPONDAMAGE
	FLOAT		org_min;
	FLOAT		org_max;
	FLOAT		result_min;
	FLOAT		result_max;
	FLOAT		bonus_min;
	FLOAT		bonus_max;
#endif
};

// User 상태효과 (condition) 관련 lock
struct _XUSERLOCK
{
	DWORD	movelock : 1;
	DWORD	tradelock : 1;
	DWORD	battlelock : 1;
	DWORD	skilllock : 1;
	DWORD	medlock : 1;
	DWORD	spelllock : 1;
	DWORD	speedlock : 1;
	DWORD	chatlock : 1;
	DWORD	modelock : 1;
	DWORD	partylock : 1;
	DWORD	camerarotatelock : 1;
	DWORD	camerazoomlock : 1;
	DWORD	camerareverse : 1;
	DWORD	adddelaytime : 1;
	DWORD	notdrawusername : 1;

	// 2006.01.17 추가
	DWORD	disableskill : 1;
	DWORD	disablestype : 1;

	DWORD	reserved : 15;

	short	disableskillindex;
	short	disablestypeindex;
};

// 영약 관련 데이타
struct _XUSERELIXIRINFO
{
	struct _Data
	{
		short __Data[10]; 
		char  __Grade;	
	};

	_Data Data[5];	// 오의별 데이터. 
					// 0 근력 
					// 1 진기 
					// 2 지혜			
					// 3 건강 
					// 4 민첩 
					// 보조 영약 먹었을 경우의 테이터는 어떻게 할 것인가???
	
	short _sIntoxication; // 중독 수치.
	char  Temp[3];  // 예약공간.

	void		Reset( void )
	{
		for( int i = 0; i < 5; ++i )
		{
			Data[i].__Grade = 0;
			for( int ii = 0; ii < 10; ++ii )
				Data[i].__Data[ii] = 0;
		}
		_sIntoxication = 0;
		for( i = 0; i < 3; ++i )
			Temp[i] = 0;
	}
	
	int GetTotalBonus( int index )
	{
		int result = 0;
		for( int i = 0; i < Data[index].__Grade; ++i )
		{
			result += Data[index].__Data[i];
		}
		return result;
	}

	_XUSERELIXIRINFO()
	{
		Reset();
	}
};

struct _XMatchMemberInfo
{
	unsigned char		ucTeam : 2;
	unsigned char		ucNumber : 6;

	unsigned char		ucSex : 2;
	unsigned char		ucMoonpa : 6;

	char				cClass;
	unsigned char		ucLevel;
	unsigned short		usWinRate;
	unsigned char		ucMatchGrade;
	unsigned char		ucVitalRate;
	char				cName[13];

	void Reset()
	{
		ucTeam = 0;
		ucNumber = 0;
		ucSex = 0;
		ucMoonpa = 0;
		cClass = 0;
		ucLevel = 0;
		usWinRate = 0;
		ucMatchGrade = 0;
		ucVitalRate  = 0;
		memset(cName, 0, sizeof(cName));
	}
};

struct _XMatchTableInfo
{
	unsigned short		usWaitTime;
	char				cRecordType;
	char				cMatchType;
	char				cMatchTime;
	char				cUseHyperrun;
	char				cUseBuff;
	char				cMatchBalance;

	_XUserItem			myBetItem[6];
	_XUserItem			targetBetItem[6];

	int					myBetMoney;
	int					targetBetMoney;

	DWORD				dwWaitStartTime;

	void Reset()
	{
		usWaitTime = 0;
		cRecordType = 0;
		cMatchType = 0;
		cMatchTime = 0;
		cUseHyperrun = 0;
		cUseBuff = 0;
		cMatchBalance = 0;
		dwWaitStartTime = 0;

		for(int i = 0 ; i < 6 ; ++i)
		{
			myBetItem[i].Reset();
			targetBetItem[i].Reset();
		}

		myBetMoney = 0;
		targetBetMoney = 0;
	}
};

//Author : 양희왕
typedef enum tagPCroomState
{
	_XDEF_PCROOM_ERROR = 0,
	_XDEF_PCROOM_INTERNET,
	_XDEF_PCROOM_RATIOPC,
	_XDEF_PCROOM_RATIOCAFE,
	_XDEF_PCROOM_PREPAID,
	_XDEF_PCROOM_PREMIUM,
	_XDEF_PCROOM_NONE
} _XUSER_PCROOM_STATE;
//Last Updated : 07/03/15

// PK범 위치 추적 or 특정인 위치 추적
typedef struct 
{
	u_char		ucPKType;					// PK범 위치 추적 or 특정인 위치 추적
	int			nStartTrackingTime;		// 위치추적신청시간
	u_char		ucTimeMode;					// 의뢰한 위치추적 내용 ( 1) 24시간, 2) 48시간 ) 
	char		szCharacName[_XDEF_MAX_USERNAMESTRINGLENGTH];
}_XPKTRACING_INFO;

enum MasterNDisciple //Author : 양희왕 //breif : 현재 로컬인 내가 스승인지,, 제자인지 아무것도 아닌지 표현
{
	_XDEF_NONE = 0,
	_XDEF_MASTER, 
	_XDEF_DESCIPLE
};


class _XLocalUser : public _XUser
{

// Construction/Destruction
public:
	_XLocalUser();
	~_XLocalUser();

// Attributes
private:

//Author : 양희왕
	struct State
	{
		DWORD dwError:1;
		DWORD dwInternet:1;
		DWORD dwRatioPc:1;
		DWORD dwRatio:1;
		DWORD dwPrepaid:1;
		DWORD dwPremium:1;
		DWORD dwNone:1;
	};

	typedef struct 
	{
		State PCROOM_State;
		DWORD	dwRemain;
	}PCROOM_App;

	PCROOM_App m_dwPCroomInfo;
//Last Updated : 07/03/15

	short					m_FinallySelectedSkillID; //Author : 양희왕 //breif : _XDEF_SKILLDEALY_MODIFY
	_XUserSkill*			m_FinallySelectedSkillItem;


	bool	m_bGuardMob; //Author : 양희왕 //breif : 베트남 shift 클릭

	int	m_nNotoriousValue; //Author : 양희왕 //breif : 평판 시스템 

	BOOL m_bChangingWeapon; //Author : 양희왕 //breif : 무기를 교체 중인지 판단
	BOOL m_bChangingWeaponPacket; //Author : 양희왕 //breif : 패킷 받았는 지 판단
	int	m_nChangeWeaponModel;

public:
	
	
	int						m_iNetworkKey;					// 해킹 방지를 위한 키값 hotblood 20040824
	short					m_sMiniGameId;					// 미니게임 종류 hotblood 20040824
	
	// 유저 기본
#if defined(_XDEF_ITEMMALL_AUTOLOGIN_20061120) || defined(_XDEF_ITEMMALL_AUTOLOGIN_SE_20070514)
	TCHAR					m_ItemMallWebLoginID[33];	
#endif
	TCHAR					m_UserName[33];
	TCHAR					m_UserPW[33];

	_XCHARACTER_INFO		m_AllCharacterInfo[ _XDEF_MAX_CHARACTERCOUNT ];	
	int						m_CharacterCount;
	int						m_SelectedCharacter;
	D3DXVECTOR3				m_NextRespawnPosition;

	// 유저 행동 로그
	_XUSERACTIONLOG			m_CharacterLog;	

	// 파티 정보
	int						m_TotalCountOfPartyQuestCounter;			// 파티퀘스트에서 파티 카운터의 총합을 나타냄 
	int						m_PartySelfIndex;							// 파티 리스트 배열에서 자기 자신의 위치를 나타내는 인덱스
	int						m_PartyMemberCount;							// 파티원 총합
	int						m_PartyMemberIndex;							// 파티관련 패킷을 보낼때 쓰이는 파티원 관련 인덱스
	_XUSERPARTYINFO			m_PartyInfo[_XDEF_MAXPARTYMEMBERCOUNT];

	// 유저 정보
	int						m_SkillCombo;
	int						m_SkillStarLevel;
	int						m_MartialStep;
		
	int						m_ExperiencePercentage;

	int						LifePowerPercentage;					 // 현재 생명 %
	int						ForcePowerPercentage;					 // 현재 내공 %
	int						ConcentrationPowerPercentage;			 // 현재 의지 %

	// 인벤토리 아이템
	_XUserItem				m_UserItemList[_XINVENTORY_SLOTTYPE_TOTALCOUNT];
	DWORD					m_Money;									// 현금
	char		            m_cAmorPocketNum;					        // 상의포켓 숫자
	char			        m_cPantsPocketNum;							// 하의포켓 숫자
	char					m_cPocketNum;								// 임시 추후 삭제
	
	char					m_cBackPackExtend;							// 배낭      
	_XUSERELIXIRINFO		m_ElixirInfo;								// 영약관련 데이타정보
	DWORD					m_IntoxicationTimer;							// 중독 타이머
	
	_XUserItem*				m_SelectedWeaponItem;
	_XGameItem*				m_PickItem;

	int						m_nCashBackPackMaxValue;				//Author : 양희왕 //breif : 캐시 행낭 최대값
	
	// 창고 아이템 관련
	_XUserItem				m_WarehouseItemList[_XDEF_WAREHOUSE_TOTALCOUNT];
	DWORD					m_WarehouseMoney;
	int						m_WarehouseSlotCount;
	
	// mastery 스킬 관련
	_XMasterySkillItem		m_MasterySkillList[_XDEF_MAX_LEARNMASTERYSKILL];
			
	// 무공 관련	
//	_XSkillItem				m_SkillList[_XDEF_MAX_LEARNSKILL];
	_XUserSkill				m_SkillList[_XDEF_MAX_LEARNSKILL];
	short					m_SelectedSkillID;
//	_XSkillItem*			m_SelectedSkillItem;
	_XUserSkill*			m_SelectedSkillItem;
	short					m_ActiveSlotID;

	short					m_NextSkillID;

	// 이벤트로 경공 주기 추가
	_XUserSkill				m_EventSkillList;		// 1개만
	_XTimeUnit				m_EventSkillEndTime;

	// BOOL 변수 ==================================
	BOOL					m_bUsingSkill : 1;
	BOOL					m_bCheckDelayTime : 1;
	BOOL					m_bRestartActive : 1;
	BOOL					m_bStartCharge : 1;
	BOOL					m_bCheckFirstFrame : 1;
	BOOL					m_bWaitPassiveReq : 1;
	BOOL					m_bAddDelayTime : 1;

	BOOL					m_ChargeSendStartPacket : 1;
	BOOL					m_RoundingSendPacket : 1;
	BOOL					m_CastingSendPacket : 1;
	BOOL					m_HiddenSendStartPacket : 1;
	BOOL					m_HiperRunStopSignalSended : 1;			// 경공을 강제로 종료시(내력==0) 서버로 보내는 패킷 전송 여부
	BOOL					m_HiperRunFailedFlag : 1;
	BOOL					m_FinishSendPacket : 1;
	BOOL					m_SmashingSendPacket : 1;
	BOOL					m_UseSpeedSkill : 1;
	BOOL					m_bLeftClickProcess : 1;
	BOOL					m_bRightClickProcess : 1;
	BOOL					m_bSaveRespawnPos : 1;
	BOOL					m_bSendTrainingStartPacket : 1;

	BOOL					m_bIsBossBattle : 1;     
	BOOL					m_bExitBossBattleRoom : 1;
	BOOL					m_bBossBattleEndProcess : 1;
	BOOL					m_bSuccessBossBattle : 1;
	BOOL					m_bWaitBossBattle : 1;
	BOOL					m_bWaitFlagInfo : 1;
	BOOL					m_bKeyLPSProcess : 1;
	BOOL					m_bPartyProgress : 1;				// 파티에 관련된행동을 하고 있다는 것을 알려주는 변수(파티맺기 중...등등)
	BOOL					m_bPartyQuestProgress : 1;			// 파티퀘스트를 진행 중이라는 것을 알려주는 변수(파티에 관련된 동작을 막는다)
	BOOL					m_InParty : 1;						// 파티를 맺고 있다는 것을 알려주는 변수
	BOOL					m_IsServerMoveInParty : 1;
	BOOL					m_bGrabItem : 1;				// 아이템 줍기 관련 - 차후 확장
	BOOL					m_bVirtualMode : 1;				// 튜토리얼 모드로 들어가기 위한 변수 
	// =================================================

	BOOL					m_bWaitDyingSkip : 1;
	BOOL					m_PvpNPCLock : 1;					// 거래시 npc Lock 관련
	BOOL					m_bSendFinishFirePacket : 1;		// Finish Fire packet을 보냈음
	BOOL					m_bSendFinishPosUpdatePacket : 1;	// Finish 후 위치 update packet 보냈음
	BOOL					m_bChannelingSendStartPacket : 1;	// Channeling start packet을 보냈음
	BOOL					m_bReadingBook : 1;					// 비급 읽는 중인지 
	BOOL					m_bInternalUse : 1;					// 영약을 복용 중인지
	BOOL					m_bSocketPlugIn : 1;				// 소켓을 다는 중인지
	BOOL					m_bSocketPlugInLock : 1;			// 게이지가 나타난 상태에서 넣고 빼기 잠금
	BOOL					m_bGambleBoxOpen : 1;				// 겜블박스를 여는 중인지
	BOOL					m_bGambleBoxOpenLock : 1;			// 게이지가 나타난 상태에서 넣고 빼기 잠금
	BOOL					m_bViewMasterySkill : 1;			// 선행 퀘스트를 완료하여 오의 테이블이 열림
	BOOL					m_bHaveStance : 1;					// stance 자세를 가지고 있음 (activeskill or 검)
	BOOL					m_bTargetInfoLock : 1;				// 인물보기를 요청중인지
	BOOL					m_bSocketPlugInEXLock : 1;			// 게이지가 나타난 상태에서 넣고 빼기 잠금
	BOOL					m_bUseMouseChargeSkill : 1;			// charge skill 시전중에 마우스로 퀵슬롯 클릭
	BOOL					m_ActionSendPacket : 1;				// action skill
	BOOL					m_bAutoMeditation : 1;				// 자동 운기 조식
	BOOL					reserved : 14;
	
	
	// 전투 관련
	int						m_CurrentSelectedSkillType;
	_XUSER_STATE_SEQ		m_CurrentBattleState;
	_XStickGauge			m_MiniHpBorder; // 2004.06.18->oneway48 insert
	_XStickGauge			m_MonsterMiniHpBorder; // 2004.06.18->oneway48 insert
	_XImageStatic			m_imageMiniHpBackBorder;

	// 공통
	DWORD					m_ChangeDelayStartTime;
	
	// Passive 관련
	unsigned short			m_LastPassiveUsefulType : 8;
	unsigned short			m_LastPassiveSType : 8;
	DWORD					m_LastPassiveOnOffTime;
	
	// Charge skill 관련
	DWORD					m_ChargeStartTime;
	int						m_ChargeIterateValue;

	// Rounding spell 관련
	DWORD					m_RoundingDelayStartTime;

	// Casting spell 관련
	DWORD					m_CastingSpellRepeatTime;

	// Hidden spell 관련
	DWORD					m_HiddenSendLastPacketTime;
	DWORD					m_HiddenReceivePacketTime;

	// Channeling spell 관련
	DWORD					m_ChannelingSendLastPacketTime;
	DWORD					m_ChannelingSpellRepeatTime;

	// 영역 공격 관련
	FLOAT					m_RegionTargetPosX;
	FLOAT					m_RegionTargetPosZ;
		
	// Finish skill 관련 - user.h에 define
//	FLOAT					m_FinishSpeed;	

	// Smashing skill 관련

	// Spell 사용시 target - server로 보낸것 저장
	_XItem*					m_SpellAttackTarget;
	_XPICKINGTARGETTYPE		m_SpellAttackTargetType;
	
	// 동작 관련	
	DWORD					m_SpecialActionTimer;
	DWORD					m_PrevPacketSendedTime;
	D3DXVECTOR3				m_LastInsertedMoveTargetEffectPosition;
	int						m_AttackPointNumber;
	int						m_CurrentAttackPoint;

	// Stance motion 처리 관련 - Battle/Peace Model change
	DWORD					m_LastModeChangeTime;

	// Targetting 관련
	_XItem*					m_ViewTarget;
	_XPICKINGTARGETTYPE		m_ViewTargetType;

	// World item 관련
	_XTILE_ITEM				m_CurrentTileIndex;
	DWORD					m_LastItemRequestSendTime;

	// 사후 처리 관련
	DWORD					m_DyingStartTime;
	int						m_LastCheckedDyingStartTime;
	DWORD					m_DyingTimer;
	TCHAR					m_RespawnName[13];
	
	// 영약 관련
	_XUSER_STATE_SEQ		m_CurrentInternalUseState;
	_XUSER_STATE_SEQ		m_CurrentReadBookState;

	// 유저의 상태 관련 - 분노, 폭주 외 상태 이상
	_XUSER_TROUBLE_STATE	m_UserTroubleState;
	
	// 수련 관련
	_XUSER_STATE			m_UserState;
	_XTRAINING_CLASS		m_SelectedTrainingMode;
	_XUSER_STATE_SEQ		m_CurrentTrainingState;

	// by mahwang
	int						m_nTrainingCount;

	// 수련에 사용되는 인증 키
	int						m_ServerKey;
	_XDUMMY_INFO			m_TrainingDummyInfo;
	_XSPELL_INFO			m_TrainingSpellInfo;

	// Path finder	
	FLOAT					m_LeftFinalTargetLength;
	D3DXVECTOR3				m_FinalTargetPosition;
	int						m_PathNodeCount;
	int						m_CurrentNavigatePathNodeIndex;
	FINFO					m_PathPointArray[300];
	void					CreateMovePath( void );
	
	// 보스전 관련 
	int						m_BossRoomID;
	DWORD					m_BossStartTime;
	DWORD					m_BossTimeLimit;
	short					m_BossRoomStageNumber;

	// 비무 관련 - OLD
	char					m_MatchType;
	unsigned short			m_MatchID;

	// 비무 관련 - NEW
	char					m_myMatchTeamID;
	_XMatchMemberInfo		m_myMatchMemberInfo;
	_XMatchTableInfo		m_myMatchTableInfo;	
	unsigned short			m_MatchTargetID;
	DWORD					m_MatchStateChangeTime;
	int						m_MatchResult;					// 0:승리, 1:패배, 2:무승부
	int						m_MatchFieldIndex;				// 비무장 정보
	DWORD					m_MatchPRTimer;					// 비무 홍보 패킷을 5초마다 보낸다.
	unsigned char			m_MatchGrade;					// 비무 계급

	// 유저의 파라미터 관련 - CharacterInfo에서 빼낼 것
	_XSU_Parameter			m_P_AttackDamage;				// 공격력
	_XSU_Parameter			m_P_AttackRate;					// 공격 성공치
	_XSU_Parameter			m_P_Defence;					// 방어력
	_XSU_Parameter			m_P_AvoidRate;					// 회피력
	_XSU_Parameter			m_M_AttackDamage;				// 기공 공격력
	_XSU_Parameter			m_M_AttackRate;					// 기공 성공치
	_XSU_Parameter			m_M_Defence;					// 기공 방어력
	_XSU_Parameter			m_M_AvoidRate;					// 기공 회피력

	// characinfo의 값은 서버에서 오는 original 값 - 이것은 Effect까지 계산 될 값
	_XSU_Parameter			m_Strength;						// 근력
	_XSU_Parameter			m_Zen;							// 진기
	_XSU_Parameter			m_Intelligence;					// 지혜
	_XSU_Parameter			m_Dex;							// 민첩
	_XSU_Parameter			m_Constitution;					// 건강

	short					m_P_MinAttackDamage;
	short					m_P_MaxAttackDamage;
	short					m_M_MinAttackDamage;
	short					m_M_MaxAttackDamage;
	
	_XSU_Parameter			m_MaxLifePower;
	_XSU_Parameter			m_MaxForcePower;
	_XSU_Parameter			m_MaxConcentrationPower;

	// 상태효과 (condition)관련 lock
	_XUSERLOCK				m_Lock;


	//Author : 양희왕 //breif : 스승인가 제자인가
	int						m_nLocalMnDClass;
	
	// 이동 속도
private:	
	short					m_MoveSpeedFromCondition;
	
public:	
	short					Get_m_MoveSpeedFromCondition(){	return _XMEMORYCHECKDATA_ENCSHORT(m_MoveSpeedFromCondition); }
	void					Set_m_MoveSpeedFromCondition(short speed){ m_MoveSpeedFromCondition= _XMEMORYCHECKDATA_ENCSHORT(speed); };
	

	// Item으로 인한 Penalty
	short					m_PenaltyValueFromItem[2];
	
	// 모든 무기 데미지 증가/감소 Effect
	short					m_WeaponMinDamageBonus[2];	// 0:value, 1:percentage
	short					m_WeaponMaxDamageBonus[2];	// 0:value, 1:percentage
	
	int						m_ResistMinus;				// 음의 저항력
	int						m_ResistPlus;				// 양의 저항력
	int						m_ResistHon;				// 혼의 저항력
	int						m_ResistSin;				// 신의 저항력
	
	// 2007.06.26 추가---┓
	int						m_AttackDamageMinus;		// 음의 공격력
	int						m_AttackDamagePlus;			// 양의 공격력
	int						m_AttackDamageHon;			// 혼의 공격력
	int						m_AttackDamageSin;			// 신의 공격력
	// ------------------┛
	
	DWORD					m_ElixirSoundRepeatTime;	// 영약 먹는 도중 사운드 반복을 위해
	
	// 개인 상점 관련 Request Timer
	DWORD					m_PrevRequestPacketSendedTime;	// 상점 리스트를 재 요청하는 시간 


	int						m_ChatMuteTime;
	int						m_LastChatLockNoticeMinute;
	
	// PK time
	DWORD					m_PKModeLeftTime;
	DWORD					m_PKModeStartTime;
	
	// PE time
	DWORD					m_PEModeLeftTime;
	DWORD					m_PEModeStartTime;
	
	// 이전 모드
	_XBATTLEMODETYPE		m_PrevBattleMode;				// 이전 모드
	
#ifdef _XTS_BLOODPOINT
	// 혈정
	int						m_BloodPoint;
#endif

	int						m_FameAttrib;	// -1 : 프리 0 : 명성 1 : 악명

	DWORD					m_SafetyZoneCheckTimer;			// 안전 지역 체크 타이머
	int						m_InsideSafetyZoneIndex;		// 0:일반지역  1:안전지역  2:경범죄자  3:중범죄자

	int						m_UseItemInvenNumber;	// (cash)사용하려는 아이템 inven # - 메세지 박스를 거치기 때문에 저장해놓는다.

#ifdef _XDEF_CASHITEM_SHOUT
	int						m_ShoutMode;
	DWORD					m_ShoutStartTime;
	DWORD					m_ShoutLeftTime;

	void					ResetShout(void);
	void					ProcessShout(void);
#endif

	//
	// 경험치/숙련도/구룡기행 type - item ctype/sid
	//
	int						m_AccountItemcType;
	int						m_AccountItemsID;
	
	// 끝나는 시간 
	unsigned int			m_etYear		: 5;
	unsigned int			m_etMonth		: 4;
	unsigned int			m_etDay			: 6;
	unsigned int			m_etHour		: 5;
	unsigned int			m_etMinute		: 6;
	unsigned int			m_etSecond		: 6;

	void					SetExpItemEndTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);
	void					ResetExpItemEndTime(void);

	//
	// 장원전 정보
	//
	bool					m_bCastleBattle;				// 장원전에 참가중인지
	bool					m_bSendThreatenPacket;			// 협박 시작 요청 패킷 보냈음
	bool					m_bThreatening;					// 협박 중

	int						m_nCastleMapId;					// 내가 속한 영웅단이 장원을 소유하고 있는가
	DWORD					m_dwRoleInCastleBattle	: 8;	// 장원전 중 나의 역할	1:총대장
	DWORD					m_dwAttackType			: 8;	// attacker:1  defender:2
	DWORD					m_dwResultCastleBattle	: 8;	// win:1	lose:2
	DWORD					m_dwDeadCount			: 8;	// 혼수 상태 횟수
	DWORD					m_dwDieStartTime;
	DWORD					m_dwDeadPenaltyTime;
	int						m_nCBMapIdInWar;				// 현재 참가 중인 장원전 map id
	
	// 장원전에 참가중인 단/회 정보
	int						m_nDefenderType;
	int						m_nDefenderIndex;

	int						m_nAttackerType[max_count_of_challenge_list];
	int						m_nAttackerIndex[max_count_of_challenge_list];
	
	
	list <_XPKTRACING_INFO*>		m_listPKTracing;		// pk범 위치 추적 정보
	
	// 비무 대회 정보
	int						m_nKillCount;
	int						m_nDeadCount;
	
	// 흑백대전 - 패왕동 출입권한 남은시간
	DWORD					m_dwBWLeftTimeFromServer;		// ms
	DWORD					m_dwBWReceiveTimeFromServer;	// ms
	
	BOOL					m_bSendFunctionalMovePacket;	// fuctional object의 이동 처리 중에 서버 허락을 받아야 하는가?
	int						m_nFuctionalNormalAction;		// 어떤 action을 실행해야 하는가? (0:없음   1:Normal  2:Exception)

	void					GetBWLeftTime(int& hour, int& min, int& sec);
	void					DrawBWLeftTime();


// Get/Set functions
public:
	void					SetNotorious(int nValue) { m_nNotoriousValue = nValue; }  //Author : 양희왕 //breif : 평판 시스템 
	const int				GetNotorious() const { return m_nNotoriousValue; }  //Author : 양희왕 //breif : 평판 시스템 

	void					ResetCastleBattleInfo();
	BOOL					IsEnemyInCB(int ortype, int orindex, int& attacktype);	// 나와 적인가? (Defender or Attacker)

	BOOL					GetEnterMatchEvent(void);	// 대회 참가중인가?
	BOOL					GetEnterCastleBattle(void);	// 장원전 참가중인가?
	BOOL					GetEnterMatchReadyArea(void);	// 대회 참가중에 부활 영역에 있는가?
	
	
#ifdef _XDWDEBUG
	void					DrawMovePath( void );
#endif

	void					SelectCharacter( int characterindex );

	_XUserItem*				GetUserItem( int firstcategory, int id );
	_XUserItem*				GetInventoryEmptySlot( void );
	int						GetUserItemInvenSlot(int firstcategory, int id);
	
	_XUserItem*				GetUserItemFromSecondType(int cType, int sType);
	int						GetUserItemInvenSlotFromSecondType(int cType, int sType);
		
	void					ResetInventoryInfo( void );
	void					ResetWarehouseInfo( void );	

	_XUserSkill*			GetSkillItem( short uniqueid );
	int						GetLearnedSkillIndex(short uniqueid, BOOL bIncludeEventSkill = FALSE);
	void					ResetSkillList(void);
	int						GetLearnedMasterySkillIndex(short uniqueid);
	
	BOOL					HaveSkillType(char clan, char stype = 0);
	
// Static functions
public:
	static	void			InitializeUserDataTable( void );
	static	void			GetUserLevel( DWORD experience, DWORD& levelindex, DWORD& sublevel );
	
// Operation functions
public:
	virtual void			SetAttackTarget( _XItem* target, _XPICKINGTARGETTYPE targettype );
	void					SendAttackSignal( void );
	void					SetViewTargetToAttackTarget(void);
	
	void					MoveStartPosition( void );
	void					ResetLocalUserData( void );
	
	void					ResetLocalUserTempData(void);	
	void					RecalcLocalUserParameter( void );
	void					UpdateLocalUserLevel( void );	
	void					UpdateLocalUserAttackLevel( void );	
	void					GetLocalUserLevel( void );

	void					SetSkillAnimation(void);
	void					SetSkillAnimation(int motionindex);
	void					ResetSkillAnimation(bool changeskill = false);

	void					RequestBattleMode(_XBATTLEMODETYPE mode);
	void					SetBattleMode(_XBATTLEMODETYPE mode);
//	void					SetActiveSkill(void);
	void					ResetActiveSkillAnimationControl(void);

	void					ProcessBattle(void);
	
	void					DrawBattle(void);
	void					DrawBattleRoundingSpell(void);
	void					DrawBattleCastingSpell(void);

	void					ProcessCommonAction(void);
	void					ProcessNormalAction(void);

	virtual void			ProcessBattleActiveSkill(void);	// animation cutting

	virtual void			ProcessBattleChargeSkill(void);	// hotblood zerolevel
	void					ProcessBattleFinishSkill(void);
	void					ProcessBattleSmashingSkill(void);
	void					ProcessBattleActionSkill(void);

	void					ProcessBattleSelfRoundingSpell(void);
	void					ProcessBattleTargetRoundingSpell(void);
	void					ProcessBattleAreaRoundingSpell(void);
	
	void					ProcessBattleSelfCastingSpell(void);
	void					ProcessBattleTargetCastingSpell(void);
	void					ProcessBattleAreaCastingSpell(void);

	void					ProcessBattleHiddenMA(void);

	void					ProcessBattleSelfChannelingSpell(void);
	void					ProcessBattleTargetChannelingSpell(void);
	void					ProcessBattleAreaChannelingSpell(void);
	BOOL					ProcessBattleChannelingNextStep(void);
	
	BOOL					CheckTargetTypeFinal(void);

	void 					ProcessCollectEvent(void);

	void					GetTargetPosition(void);
	void					GetTargetPositionIdleMode(void);
	void					GetTargetPositionBattleMode(void);
	
	BOOL					CanNotMove(void);
	BOOL					CheckValidNextPosition(void);
	void					ProcessNextMovePosition(void);
	virtual void			ProcessAnimation(void);	// hotblood zerolevel
	void					ProcessParameter(FLOAT progresstime);
	void					ProcessInternalUse(void);
	void					ProcessReadBook(void);
	void					ProcessSpeedSkill(void);

	void					CheckDelayTime(void);
	void					ApplyNextSkill(void);

	BOOL					CheckDistanceToTarget(D3DXVECTOR3 targetposition, short skillid, FLOAT& range, unsigned short mobType = 0, bool bAttackAfterMove = false);
	void					SetTargetPosition(D3DXVECTOR3 targetposition, FLOAT range);
	void					InsertTargetPositionEffect( void );

//	void					SetActiveSlotSkill(void);

	void					SetViewTarget(_XItem* target, _XPICKINGTARGETTYPE targettype, BOOL bsetpkready = FALSE);
	void					ResetViewTarget(void);	
	void					ResetAttackTarget(void);

	void					SetSpellAttackTarget(_XItem* target, _XPICKINGTARGETTYPE targettype);
	void					ResetSpellAttackTarget(void);

	void					GetItemTileIndex(FLOAT x, FLOAT z, _XTILE_ITEM& itemtile);
	void					SetTargetPositionAngle(D3DXVECTOR3 targetposition);
	void					UpdateRotateAngle(void);					

	void					ProcessTraining(void);
	void					ProcessTrainingMeditation(void);
	void					ProcessTrainingDummy(void);
	void					ProcessTrainingSpell(void);
	void					ProcessTrainingFireWood(void);
	void					ProcessTrainingWaterDraw(void);
	void					ResetTrainingMode(void);

	void					ProcessMatch(void);

	// 분노-폭주 상태처리 관련
	BOOL					IsTroubleStateNormal(void);
	BOOL					IsTroubleStateFury(void);
	BOOL					IsTroubleStateConvergence(void);
	
	int						CheckPartyMember( int partymemberuniqueid );

	void					ResetMatchState(void);

	// 아이템 집기
	void					GrabWorldItem(void);
	int						GetEmptyInventorySlot(_XGameItem* pItem);
	void					UseItem(int invennumber);

	// 캐릭터 상태 효과 처리
	void					ProcessStateList(void);
	void					DrawStateList(void);
	DWORD					GetLeftTimeFromStateList(char fromtype, short effectid, short skillid = 0, char slotindex = 0);
	void					CharacterStateChatLock(TCHAR* message);
	void					ApplyCharacterStateBonus(void);
	void					ApplyCharacterStatePenalty(void);
	void					ApplyCharacterStateFinal(void);
	_XEN_4EFF_SYS			FindMaxDamage(void);
	
	void					SetMonsterWarningMessage(unsigned short mobtype);
	
	// MISC 
	void					DeleteWeaponModel(short skillid);
	void					DeleteWeaponModel(void);
	void					ReloadWeaponModel(void);
	
	// 비무
	void					SetDefaultValueMatchTable(void);

	//기공 대미지 표시 처리
	void					ViewCastingDamage(void); //Author : 양희왕 //breif : 미국 기공 대미지 안나오던 현상
	
// Overrides  functions
public:
	void					UpdatePosition( void );
	void					SetAnimation( _XMOTIONCLASS_TYPE motionclass, bool changemotion = true );
	void					SetAnimationIndex( int animationindex, bool changemotion = true );

	void					Process( void );
	void					Render( void );

	BOOL					GetNextMove( D3DXVECTOR3& next );

	void					SetDieMotion(void);
	void					SetBlowMotion(void);
	void					SetDefenceMotion(void);

	void					ProcessRescueUser(void);
	void					ProcessMobDie(void);

	void					SetRebirthBinsaMode(void);
	void					SetDyingMode(void);
	
	short					GetInterpolateMotionIndex(short skillid);

	virtual short			GetStanceMotion(short activeslotid);	// hotblood Zerolevel
	virtual short			GetMotionIndex(short selectedskillid); // hotblood Zerolevel
	virtual char			GetUsefulType(char selectedskillid);	// hotblood Zerolevel

	// 가상공간일 때 처리하기
	void					Process_VirtualMode( void );
	void					ProcessAnimation_VirtualMode( void );
	void					ProcessBattle_VirtualMode( void );
	void					ProcessActiveSkill_VirtualMode( void );
	void					ProcessMobDie_VirtualMode( void );
	
	// LPS 시스템 처리
	BOOL					ProcessLPSSystem(_XMob* pMob);  
	
	void					CheckCollideStatusInIdle( void );
	
	void					ResetLock(BOOL bCondition = TRUE);

	BOOL					SetPKReadyMode(void);
	void					ProcessPKMode(void);
	void					ResetPKMode(void);
	void					ResetPKReadyMode(void);
	BOOL					CheckPKTarget(_XUser* _user);
	BOOL					CheckPKPenalty(_XUser* _user);

	DWORD					GetPEModeLeftTime(void);
	DWORD					GetShoutLeftTime(void);
	
	// 자리 비움 처리
	void					SetOut(BOOL state);
	
	BOOL					GetInBossRoom(void);

//Author : 양희왕
	const BOOL GetChangingWeapon() const { return m_bChangingWeapon; }
	void SetChangingWeapon( const BOOL bRes ) { m_bChangingWeapon = bRes; }
	const BOOL GetChangingWeaponPacket() const { return m_bChangingWeaponPacket; }
	const int GetChangeWeaponModel() const { return m_nChangeWeaponModel; }
	void SetChangingWeaponPacket( const BOOL bRes, const int nModel ) 
	{
		m_bChangingWeaponPacket = bRes; 
		m_nChangeWeaponModel = nModel;
	}

	inline const PCROOM_App & GetPCroomInfo() const { return m_dwPCroomInfo; }
	inline const DWORD & GetPCroomRemain() const { return m_dwPCroomInfo.dwRemain; }
	const DWORD GetPCroomState( const _XUSER_PCROOM_STATE PCroomState ) const;
	
	inline PCROOM_App SetPCroomInfo() { return m_dwPCroomInfo; }
	inline void SetPCroomRemainData( const DWORD dwData) { m_dwPCroomInfo.dwRemain = dwData; }
	void SetPCroomState( const _XUSER_PCROOM_STATE PCroomState, const DWORD dwResult );
//LastUpdated : 07/03/15

//Author : 양희왕
#ifdef _XDEF_SKILLDEALY_MODIFY
	inline void SetFinallySelectedSkillID( const short selectedskillid ) { m_FinallySelectedSkillID = selectedskillid; }
	inline const short GetFinallySelectedSkillID() const { return m_FinallySelectedSkillID; }
	inline void SetFinallySelectedSkillItem( _XUserSkill* const SelectedSkillItem ) { m_FinallySelectedSkillItem = SelectedSkillItem ; }
	inline const _XUserSkill* GetFinallySelectedSkillItem() const { return m_FinallySelectedSkillItem; }
#endif
	//Last Updated : 07/03/22


};

#endif