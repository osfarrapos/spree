// XUser.h: interface for the XUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XUSERINFO_H_
#define _XUSERINFO_H_

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
#include "XCharacterState.h"
#include "XNetwork.h"

#include "XMemoryPool.h"
#include <map>

#ifdef _XDWDEBUG
#define VIEWBATTLELOG if(g_ViewBattleLog)
#else
#define VIEWBATTLELOG _XCOMMENT
#endif

// -------------------------------------------------------------------------------------------
// 유저 관련 Define 

// 캐릭터 기본 데이터 
#define _XDEF_MAX_CHARACTERCOUNT			3		// 유저가 소유할수 있는 최대 캐릭터 수	
#define _XDEF_MAX_USERNAMESTRINGLENGTH		13		// 최대 캐릭터 이름 길이

//	동작별 체력 증가율 
#define _XDEF_HEALTHINCREASERATE_IDLE		4.0f	// 정지시 증가율
#define _XDEF_HEALTHINCREASERATE_WALK		4.0f	// 걷기시 증가율
#define _XDEF_HEALTHDECREASERATE_RUN		1.0f	// 뛰기시 감소율

// 경공을 사용할수 있는 minimum 정기 percentage
#define _XDEF_RUN_LIMITSTLENGTH				20.0f

//	이동 속도/물리 관련 
#define _XDEF_USERPOSITION_EPSILON			0.1f
#define _XDEF_CHARACTERFRICTION				250.0f	// 캐릭터에 적용될 마찰력
#define _XDEF_CHARACTERGRAVITY				-16.0f	// 캐릭터에 적용될 중력값 
#define _XDEF_MAXJUMPVELOCITY				125.0f
#define _XDEF_MAXJUMPHEIGHT					6.0f

// 피킹 관련
#define _XDEF_COLLIDERADIUS					0.55f
#define _XDEF_COLLIDEOFFSET_HUMAN			0.6f

// 오성 관련 최대/최소치 
#define _XDEF_HELATHSYSTEM_VITALITYMIN		10.0f	// 생기 최소값
#define _XDEF_HELATHSYSTEM_VITALITYMAX		150.0f	// 생기 최대값

#define _XDEF_HELATHSYSTEM_ZENMIN			10.0f	// 진기 최소값
#define _XDEF_HELATHSYSTEM_ZENMAX			150.0f	// 진기 최대값

#define _XDEF_HELATHSYSTEM_SOULMIN			10.0f	// 영기 최소값
#define _XDEF_HELATHSYSTEM_SOULMAX			150.0f	// 영기 최대값

#define _XDEF_HELATHSYSTEM_HEARTMIN			10.0f	// 심기 최소값
#define _XDEF_HELATHSYSTEM_HEARTMAX			150.0f	// 심기 최대값

#define _XDEF_HELATHSYSTEM_JUNGGIMIN		10.0f	// 정기 최소값
#define _XDEF_HELATHSYSTEM_JUNGGIMAX		150.0f	// 정기 최대값

// 캐릭터 동작 관련	
#define _XDEF_SENDTIME_IDLEMOTION			8000	// 정지시 포지션 패킷 전송 간격
#define _XDEF_CHANGEIDLEMOTIONTIME			10000	// 특수 동작 관련 관련

// 최대 파티원수
#define _XDEF_MAXPARTYMEMBERCOUNT			9

// 최대 보유 별호 수
#define _XDEF_NICKNAME_TOTALCOUNT		212
// ---------------------------------------------------------------------------------------
// 유저 관련 enumeration type

// 문파
typedef enum tagGroupType
{
	_XGROUP_NANGIN			= 0,
	_XGROUP_GAEBANG,
	_XGROUP_BEEGOONG,
	
	_XGROUP_SORIM,
	_XGROUP_NOCKRIM,
	_XGROUP_MOODANG,

	_XGROUP_MAKYO,
	_XGROUP_SEGA,
	_XGROUP_NOFIXED,
	
	_XGROUP_MAXGROUPCOUNT
} _XGROUPTYPE;

enum
{
	_XGROUPTYPE_NONE = 0,
	_XGROUPTYPE_WHITE,	// 백도
	_XGROUPTYPE_BLACK	// 흑도
};

typedef enum tagClassType
{
	_XROLLCLASS_NONE		=0, // 0 낭인 
	_XROLLCLASS_BATTLE,			// 1 전투형
	_XROLLCLASS_HEALER,			// 2 치유형
	_XROLLCLASS_COMPLETE,		// 3 완전형
	_XROLLCLASS_SPELLER,		// 4 기공
	_XROLLCLASS_MURDER,			// 5 살수
	_XROLLCLASS_HOSIM,			// 6 호심
	_XROLLCLASS_JUSOOL,			// 7 주술
	_XROLLCLASS_WOIGONG,		// 8 외공
	_XROLLCLASS_SOHWAN,			// 9 소환
	_XROLLCLASS_SUNDO,			// 10 선도
	_XROLLCLASS_MADO,			// 11 마도
	_XROLLCLASS_MAXCOUNT
} _XROLLCLASSTYPE;

// 성별
typedef enum tagGenderType
{
	_XGENDER_MAN	= 0,
	_XGENDER_WOMAN
} _XGENDERTYPE;

// 전투모드/평화모드/PK모드
typedef enum tagBattleType
{
	_XBATTLE_PEACE = 0,
	_XBATTLE_BATTLE,
	_XBATTLE_PK
} _XBATTLEMODETYPE;

// 공격 타겟
typedef enum tagPickingTargetType
{
	_XPICKINGTARGET_NONE = 0,
	_XPICKINGTARGET_PC,
	_XPICKINGTARGET_MOB,
	_XPICKINGTARGET_NPC,
	_XPICKINGTARGET_ITEM,
	_XPICKINGTARGET_DUMMY,
	_XPICKINGTARGET_SEAT,
	_XPICKINGTARGET_SELF,
	_XPICKINGTARGET_COLLECT,// by mahwang
	_XPICKINGTARGET_FUNCTIONALOBJECT
} _XPICKINGTARGETTYPE;

// 이동 동작 타입
typedef enum tagMoveType
{
	_XMOVETYPE_RUN = 0,
	_XMOVETYPE_KYUNGGONG
} _XMOTION_MOVETYPE;

// 실제 동작별 타입
typedef enum tagMotionclassType
{
	_XACTION_NONE	=	0,
	_XACTION_IDLE,
	_XACTION_MOVE,
	_XACTION_ATTACK,
	_XACTION_DEFENCE,
	_XACTION_BLOW,
	_XACTION_COLLAPSE,
	_XACTION_DIE,	
	_XACTION_REBIRTH,
	_XACTION_ENTERING,
	_XACTION_DYING,
	_XACTION_RESCUE,
	_XACTION_SPECIAL,
	_XACTION_TALK,
	_XACTION_MATCH_START,
	_XACTION_MATCH_CANCEL,
	// by mahwang
	_XACTION_COLLECT,
	_XACTION_FIREWOODCUT,
	_XACTION_WATERDRAW,
	_XACTION_LPS,
	_XACTION_INSENSIBLE			// 기절 - 상태효과
	
} _XMOTIONCLASS_TYPE;

// 회전 방향
typedef enum tagRotateDirection
{
	_XROTATE_NONE = 0,
	_XROTATE_CLOCKWISE,
	_XROTATE_COUNTCLOCKWISE
}_XROTATE_DIRECTION;

typedef enum
{
	_XEN_4EFF_NONE,
	_XEN_4EFF_MINUS = 1,
	_XEN_4EFF_PLUS,
	_XEN_4EFF_HON,
	_XEN_4EFF_SIN
}_XEN_4EFF_SYS;

#define _XDEF_FASTMOVEINDEX_NONE			0
#define _XDEF_FASTMOVEINDEX_SHORTWEAPON		3
#define _XDEF_FASTMOVEINDEX_LONGWEAPON		6
#define _XDEF_FASTMOVEINDEX_SWORD			9
#define _XDEF_FASTMOVEINDEX_BATTLESWORD		12

// ---------------------------------------------------------------------------------------
// 레벨 LUT Structure
typedef struct
{
	DWORD level;					// 내공 단계	응신입기혈 ~ XXX
	DWORD sublevel;					// 내공 수위	1 ~ 12 성
	DWORD innerlevel;				// 총 수위		innerlevel...
	DWORD needjinexperience;		// 필요 진 경험치	
	DWORD needgongexperience;		// 필요 공 경험치	
	DWORD accumulatejinexperience;	// 누적 진 경험치
} _XUSERLEVEL_LUTTYPE;

// ---------------------------------------------------------------------------------------
// Point table
typedef struct
{
	int vitality;
	int inner;
	int concenturate;
	
	int need_constitution;
	int need_zen;
	int need_intelligence;

	int need_strength;
	int need_dexterity;
} _XUSER_POINTTABLE;

// ---------------------------------------------------------------------------------------
// User model structure
typedef struct _XUSER_MODELDESCRIPTTYPE
{	
	short int 	face;				// 얼굴	

	short int 	hair;				// 머리스타일
	short int 	haircolor;			// 머리 색상

	short int 	upperbody;			// 상체
	short int 	lowerbody;			// 하체 

	short int 	cap;				// 모자

	short int 	lhand;				// 왼손
	short int 	rhand;				// 오른손
	
	short int 	lfoot;				// 왼발
	short int 	rfoot;				// 오른발 

	short int 	adae;				// 아대
	short int 	earring;			// 귀걸이
	short int	backpack;			// 배낭
	
	short int   mask;				// 마스크, 상용화의상.

	short int 	item1;				// 장착 아이템 인덱스
	short int 	item2;
	short int 	item3;
	short int 	item4;
	short int 	item5;

	_XGI_FirstCategory 	upperbodyCategory;			// 상체
	_XGI_FirstCategory 	lowerbodyCategory;			// 하체 

	_XGI_FirstCategory 	capCategory;				// 모자

	//_XGI_FirstCategory 	lhandCategory;				// 왼손
	//_XGI_FirstCategory 	rhandCategory;				// 오른손
	
	_XGI_FirstCategory 	lfootCategory;				// 왼발
	_XGI_FirstCategory 	rfootCategory;				// 오른발 

	_XGI_FirstCategory 	adaeCategory;				// 아대
	//_XGI_FirstCategory 	earringCategory;			// 귀걸이
	_XGI_FirstCategory 	backpackCategory;			// 배낭
	_XGI_FirstCategory 	maskCategory;				// 치장 
	_XGI_FirstCategory 	item1Category;				// 장착 아이템 인덱스
	
	
} _XUSER_MODELDESCRIPTTYPE;

// ---------------------------------------------------------------------------------------
// User animation structure
typedef struct 
{	
	_XMOTIONCLASS_TYPE		motionclass;		// 액션 클래스
	
	_XBATTLEMODETYPE		idletype;			// 대기 타입
	_XMOTION_MOVETYPE		movetype;			// 이동 타입
	DWORD					attacktype;			// 공격 타입
} _XUSER_ANIMATIONDESCRIPTTYPE;

// ---------------------------------------------------------------------------------------
// 유저의 분파 정보
struct _XUSERORGANIZATIONINFO
{
	int				m_iIndex;				
	char			m_cName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;			// 조직 이름	
	char			m_cProperty ;			// 조직 속성(분파 | 방회)	
	char			m_cClass ;				// 계급
	enum{
		_UNIT_CLASS_DEFAULT_UNIT_,			// 기본 조직원
		_UNIT_CLASS_SPECIAL_UNIT_,			// 상급 조직원
		_UNIT_CALSS_CAPTAIN_UNIT_,			// 절대 지휘자..
	} ;
	
	inline void Reset()
	{
		m_iIndex = 0;
		memset( m_cName, 0, sizeof( TCHAR ) * 9 );
		m_cProperty = 0;
		m_cClass = 0;
	};
};

//유저의 방회 정보 (영웅단 동맹)
struct _XUSERALIENCEINFO
{
	int				m_iIndex;				
	char			m_cName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;			// 조직 이름	
	char			m_cProperty ;			// 조직 속성(분파 | 방회)	
	char			m_cClass ;				// 계급
	enum{
		_UNIT_CLASS_NO_UNIT_,				// 아직 등급을 가지고 있지 않다. 정보를 받아서 등급을 세팅해야 한다.
		_UNIT_CLASS_DEFAULT_UNIT_,			// 기본 방회원
			_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_,	// 영웅단 동맹 부회주(영웅단 단주)
			_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_,		// 영웅단 동맹 회주
	} ;
	
	inline void Reset()
	{
		m_iIndex = 0;
		memset( m_cName, 0, sizeof( TCHAR ) * 9 );
		m_cProperty = 0;
		m_cClass = 0;
	};
};

// ---------------------------------------------------------------------------------------
// 캐릭터 개별 정보 구조
class _XCHARACTER_INFO
{
public:

	// 캐릭터 기본 데이터	
	TCHAR							charactername[_XDEF_MAX_USERNAMESTRINGLENGTH]; // 유저아이디
	int								m_NickNameList[_XDEF_NICKNAME_TOTALCOUNT]; //자기 자신이 가지고 있는 별호 리스트
	TCHAR							m_NickName[256]; //자기 자신이 가지고 있는 별호 이름
	int								m_CurrentNIckIndex;	// 선택된 별호 인덱스
	_XGROUPTYPE						groupindex;							 // 캐릭터 문파
	_XGENDERTYPE					gender;								 // 캐릭터 성별

	// 존 관련
	DWORD							zoneuniqid;							 // 존 유니크 아이디

private:
		// 오성 관련
	_XDEF_ENCVALUE(short, constitution )						 // 건강 <= 생기 
	_XDEF_ENCVALUE(short, zen )									 // 진기 <= 그대로 
	_XDEF_ENCVALUE(short, intelligence )						 // 지혜 <= 영기 
	_XDEF_ENCVALUE(short, dexterity )							 // 민첩 <= 심기 
	_XDEF_ENCVALUE(short, strength )							 // 근력 <= 정기	
	
	// 현재 Power
	_XDEF_ENCVALUE(short, current_lifepower )					 // 현재 생명
	_XDEF_ENCVALUE(short, current_forcepower )					 // 현재 내공
	_XDEF_ENCVALUE(short, current_concentrationpower )			 // 현재 의지
	
	// 최대 Power
	_XDEF_ENCVALUE(short, max_lifepower )						 // 최대 생명
	_XDEF_ENCVALUE(short, max_forcepower )						 // 최대 내공
	_XDEF_ENCVALUE(short, max_concentrationpower )				 // 최대 의지

	_XDEF_ENCVALUE(short, externalwoundrate )					 // 외상 
	_XDEF_ENCVALUE(short, internalwoundrate )					 // 내상 
	_XDEF_ENCVALUE(WORD, fatiguerate )							 // 피로도 
	
	_XDEF_ENCVALUE(short, furyparameter )						 // 분노
	
	_XDEF_ENCVALUE(short, leftpoint )							 // 잔여 기포인트
	_XDEF_ENCVALUE(short, leftmasterypoint )					 // 잔여 마스터리 포인트

	_XDEF_ENCVALUE(short, retribution )							 //	업보
	_XDEF_ENCVALUE(int, honor )	 								 //명성  8
	_XDEF_ENCVALUE(short, showdown )							 // 비무 전적
	
	// 유저 레벨	
	_XDEF_ENCVALUE(DWORD, jinlevel )							 // 진 축적치
#ifdef _XTS_FAME
	_XDEF_ENCVALUE(int, Notoriety )								 // 악명
#else
	_XDEF_ENCVALUE(DWORD, gonglevel )							 // 공 축적치
#endif
	
	_XDEF_ENCVALUE(DWORD, level )								 // 유저 내공 단계 : 응신입기혈 ~ XXX
	_XDEF_ENCVALUE(DWORD, sublevel )							 // 유저 12성
	_XDEF_ENCVALUE(DWORD, innerlevel )							 // 내공 수위 : 진이 쌓이면 올라감.4

	_XDEF_ENCVALUE(char, cClass )								 // 역할
	_XDEF_ENCVALUE(char, cClassGrade )							 // 역할 등급
	_XDEF_ENCVALUE(int, iContribution )			   				 // 문파 기여도

	_XDEF_ENCVALUE(int, gm_level )								 // gm level.


/*
	// 오성 관련
	short							constitution;						 // 건강 <= 생기 
	short							zen;								 // 진기 <= 그대로 
	short							intelligence;						 // 지혜 <= 영기 
	short							dexterity;							 // 민첩 <= 심기 
	short							strength;							 // 근력 <= 정기	
	
	// 현재 Power
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
#ifdef _XTS_FAME
	int								Notoriety;							 // 악명
#else
	DWORD							gonglevel;							 // 공 축적치
#endif
	
	DWORD							level;								 // 유저 내공 단계 : 응신입기혈 ~ XXX
	DWORD							sublevel;							 // 유저 12성
	DWORD							innerlevel;							 // 내공 수위 : 진이 쌓이면 올라감.4

	char							cClass;								 // 역할
	char							cClassGrade;						 // 역할 등급
	int								iContribution;			   			 // 문파 기여도
*/
public:
	// Zero Level 때문에 나중에 삭제할 것
	DWORD							maxattacklevel;						 // 공격력
	DWORD							minattacklevel;						 // 공격력
	DWORD							attacksuccesslevel;					 // 공격 성공치
	DWORD							defencelevel;						 // 방어력
	
	// 비무 관련
	int								winscore;
	int								losescore;
	int								tiescore;
	int								seriesscore;
	int								max_series_wincount;
	int								max_series_losecount;
	int								abusecount;
	int								prevmatchresult;
	
	int								blockingenddate;
//#ifdef _XTS_SERVERUNIFICATION	// 통합 완료 이후에도 계속 쓰임
#ifdef _XTS_NEWCHARACLIST
	int								ucChangeNameFlag;	///< 중복이름 체크플래그 0 - Not, 1 - Possible Change Name 
	int								ucLeftFlag;			// 아직 안쓰임.
#endif
	char							cHindingMode ;			// 랭킹별호 관련 :  0 - 은신해제, 1 - 은신설정상태
		
	D3DXVECTOR2						lastposition;						//	캐릭터의 최종위치
	unsigned char					weaponStrength;			// 무기 제련 수치 
	
	// 모델
	_XUSER_MODELDESCRIPTTYPE		modeldescript;
	_XUSER_ANIMATIONDESCRIPTTYPE	animationdescript;
	
	DWORD							conditionState;						// Condition (상태효과)
	
	_XUSERORGANIZATIONINFO			m_OrganizationInfo;
	
	_XUSERALIENCEINFO				m_AlienceInfo;	//방회 정보(영웅단동맹)
	
	// 음양혼신 공격력중 최대값
	_XEN_4EFF_SYS					m_4Eff_MaxDam;

	// 음양혼신 저항력
	BOOL							m_b4Eff_ResPlus;
	BOOL							m_b4Eff_ResMinus;
	BOOL							m_b4Eff_ResHon;
	BOOL							m_b4Eff_ResSin;
	
	int								m_nClothView;

	// Constructor...
	_XCHARACTER_INFO()
	{
		memset( this, 0, sizeof(_XCHARACTER_INFO) );
		animationdescript.idletype	  = _XBATTLE_PEACE;
		animationdescript.motionclass = _XACTION_IDLE;
		animationdescript.attacktype  = _XAN_NI_ATTACK_KK;
		animationdescript.movetype    = _XMOVETYPE_RUN;
		m_OrganizationInfo.Reset();
		m_AlienceInfo.Reset();

		Set_constitution(0);
		Set_zen(0);						
		Set_intelligence(0);
		Set_dexterity(0);					
		Set_strength(0);
		Set_current_lifepower(0);
		Set_current_forcepower(0);
		Set_current_concentrationpower(0);
		Set_max_lifepower(0);
		Set_max_forcepower(0);
		Set_max_concentrationpower(0);
		Set_externalwoundrate(0);
		Set_internalwoundrate(0);			
		Set_fatiguerate(0);
		Set_furyparameter(0);
		Set_leftpoint(0);
		Set_leftmasterypoint(0);
		Set_retribution(0);
		Set_honor(0);
		Set_showdown(0);
		Set_jinlevel(0);
#ifdef _XTS_FAME
		Set_Notoriety(0);
#else
		Set_gonglevel(0);
#endif
		Set_level(0);
		Set_sublevel(0);					
		Set_innerlevel(0);
		Set_cClass(0);
		Set_cClassGrade(0);
		Set_iContribution(0);
		Set_gm_level(0);

		m_nClothView = 0;
	}

};

typedef struct _XUser_AttackResult
{
	unsigned short			usDefenderID;

	unsigned char			ucToType;
	unsigned char			ucSequence;	
	
	char					cSkillLevel;	// 0~11
	DWORD					dwChargeTime;   // ms

	int						skillType;
	short					sSkillID;
	FLOAT					fX;
	FLOAT					fZ;
	FLOAT					fRange;

	short					sAnimation;

	BOOL					display[3];
	unsigned short			usAtkInfo[3];
	
	unsigned short			sDamage[3];

	char					cDefenderVitalRate[3];

	short					sAddEffect[3];
	short					sEff_Time[3];
	
	char					cBattleResult;
}_XUSER_ATTACKRESULT;

typedef enum _XUser_AttackSequence
{
	_XDEF_ATK_SEQ_NONE = 0,
	_XDEF_ATK_SEQ_FIRST,
	_XDEF_ATK_SEQ_SECOND,
	_XDEF_ATK_SEQ_THIRD
}_XUSER_ATTACKSEQUENCE;

typedef enum _XUser_PersonalStoreState
{
	_XUSER_PERSONALSTORE_NONE = 0,
		_XUSER_PERSONALSTORE_OWNER_OPEN,
		_XUSER_PERSONALSTORE_OWNER_PREPARE,
		_XUSER_PERSONALSTORE_OWNER_MODIFY,
		_XUSER_PERSONALSTORE_CLIENT_INTEREST,
		_XUSER_PERSONALSTORE_CLIENT_MODIFY
}_XUSER_PERSONALSTORESTATE;

struct _XRegionAtkResult
{
	int					nObjectType;
	int					nID;
};

struct _XUser_RegionDamage
{
	BOOL				bValid;
	int					nObjectCnt;
	int					nSkillID;

	_XRegionAtkResult	sResult[_COUNT_REGION_ATK_RESULT_];

	_XUser_RegionDamage() : bValid(FALSE), nObjectCnt(0), nSkillID(0)
	{
		memset(sResult, 0, sizeof(sResult));
	}
};

struct MnDMasterInfo //유저의 사제 정보  //Author : 양희왕 //breif : 사제시스템
{
	int nMnDClass; //스승, 제자 여부

	int nMasterIndex; //스승의 인덱스
	int nMasterRank; //스승 등급

	TCHAR szMasterName[64]; // 스승 이름
	unsigned int nMasterContribution; //스승에게 들어가는 기여도
	unsigned int nMasterTotalContribution; //스승 기여도 (누적된 기여도)
	unsigned int nTotalKillCount; //사제원들이 죽인 킬 카운트

	unsigned int nLocalUserContribution; //본인 기여도
	unsigned int nCurKillCount; //본인이 죽인 카운트

	int nMasterConnectState;	// 0 - 로그아웃, 1 - 접속중.
	int nBonusSkillStep;
	int nSex;
	int nLevel;              
	int nMoonpa;
	int nClass;
	int nClassGrade;                                                

	int nTotalNumber; //인원수
	TCHAR szMasterNickName[64]; // 사호

	MnDMasterInfo::MnDMasterInfo()
	{
		nMnDClass = 0;
		nMasterRank = 0;
		memset( szMasterNickName, 0, sizeof(szMasterNickName));
		nTotalNumber = 0;
		memset( szMasterName, 0, sizeof(szMasterName));
		nMasterContribution = 0;
		nLocalUserContribution = 0;
		nMasterTotalContribution = 0;
		nMasterRank = 0;

		nMasterConnectState = 0;
		nBonusSkillStep = 0;
		nSex = 0;
		nLevel = 0;
		nMoonpa = 0;
		nClass = 0;
		nClassGrade = 0;
	}
};

class _XMob;

// ---------------------------------------------------------------------------------------
// by mahwang
// 이제는 _XItem 을 상속 받지 않아도 상관 없으나 전역 변수로 포인트 체크 하는 부분이 있어서 
// 그냥 남겨 둔다.

// 기본 유저 클래스
class _XUser : public _XItem
{

// Construction/Destruction
public:
	_XUser();
	~_XUser();

	void InitializeData();
	void DestroyData();	

// Attributes
public:
	
	BOOL					m_RecievedCharacterInfo;
	
	// 캐릭터 기본	
	_XCHARACTER_INFO		m_CharacterInfo;
	unsigned short			m_Uniqueid;						// 로그인서버로부터 할당된아이디	
	
	// 캐릭터 상태
	BOOL					m_KillSignal;					//	캐릭터 죽음 상태
	DWORD					m_LastRecivedMessageTime;
	
	// 캐릭터 이동
	D3DXVECTOR3				m_Position;						// 현재 위치
	D3DXVECTOR3				m_PrevPosition;					// 이전 위치
	D3DXVECTOR3				m_TargetPosition;				// 이동 위치 
	_XOBBData				m_CharacterOBB;
	D3DXVECTOR3				m_Velocity;
	
	FLOAT					m_RotateAngle;					// 캐릭터 회전각	
	short					m_HiperRunSkillID;
	unsigned char			m_HiperRunStep;
	FLOAT					m_PrevProcessTime;				// 이전 프레임 타이머
	BOOL					m_CheckInFrustum;				// 프러스텀 체크
	D3DXVECTOR3				m_MoveVector;					// 이동 방향 벡터
	D3DXVECTOR3				m_CurrentPositionTerrainNormal; // 현위치 지형 폴리곤의 노말벡터
	_XOBBData*				m_pCollideOBB;
	_XOBBData*				m_pGoundCollideOBB;
		
	// 캐릭터 모델	
	_XModelDescriptor		m_ModelDescriptor;
		
	// 캐릭터 동작
	_XBATTLEMODETYPE		m_BattleMode;					// 전투 모드
	FLOAT					m_AttackRange;					// 현재 무기의 공격 가능 범위
	BOOL					m_StopAnimation;
	_XUSER_ATTACKSEQUENCE	m_CurrentAttackSequence;		// 현재 초식 - 몇번째 초식인가
	
	// 공격
	_XItem*					m_AttackTarget;					// 현재 설정된 공격 타겟
	_XPICKINGTARGETTYPE		m_AttackTargetObjectType;		// 현재 설정된 공격 타겟 종류

	// 전투 결과
	_XUSER_ATTACKRESULT		m_AttackResult;
	_XUser_RegionDamage		m_RegionDamageResult;			// 영역공격의 결과

	// Finish skill 관련
	BOOL					m_bFinishDashStart;
	FLOAT					m_FinishSpeed;
		
	// Chatting window
	_XWindow_ChatBalloon	m_ChatBalloonObject;

	// 동작 보간 관련
	FLOAT					m_LastRotateAngle;
	_XROTATE_DIRECTION		m_RotateDirection;
	
	int						m_nIdleToMoveCnt;				// MSG_ZONE_MOVE에서 사용
	
	// 무장 해제 관련
	_XDisarmamentItem*		m_DisarmamentItem;
	
	// stance motion
	short					m_StanceMotionIndex;
	BOOL					m_StanceMotionFirst;
	
	// 운기 조식 관련 무기 저장
	short					m_CurrentWeaponID;

	int						m_JobIndexOfZeroLevel;	// hotblood->2004.09.17
	
	// 캐릭터 상태 효과
	_XCharacterStateList	m_StateList;

	//캐릭터 사제 정보
	MnDMasterInfo			m_pLocalMnDInfomation; //Autor : 양희왕 //breif : 사제시스템

private:	
	// 이동 속도, 공격 속도
	short					m_MoveSpeedFromEffect;
	short					m_AttackSpeedFromEffect;
	FLOAT					m_fMoveSpeed;					// 이동 속도	
	FLOAT					m_fPrevMoveSpeed;				// 이전 이동 속도	
	FLOAT					m_fHiperMoveSpeed;				// 경공 이동 속도

public:	
	short					Get_m_MoveSpeedFromEffect(){	return _XMEMORYCHECKDATA_ENCSHORT(m_MoveSpeedFromEffect); }
	void					Set_m_MoveSpeedFromEffect(short speed){ m_MoveSpeedFromEffect = _XMEMORYCHECKDATA_ENCSHORT(speed); }
	short					Get_m_AttackSpeedFromEffect(){	return m_AttackSpeedFromEffect; }
	void					Set_m_AttackSpeedFromEffect(short speed){ m_AttackSpeedFromEffect = _XMEMORYCHECKDATA_ENCSHORT(speed); }

	FLOAT					Get_m_fMoveSpeed(){ return (m_fMoveSpeed - 113.141592f); }
	void					Set_m_fMoveSpeed(FLOAT speed){ m_fMoveSpeed = speed + 113.141592f; }

	FLOAT					Get_m_fPrevMoveSpeed(){ return (m_fPrevMoveSpeed - 123.141592f); }
	void					Set_m_fPrevMoveSpeed(FLOAT speed){ m_fPrevMoveSpeed = speed + 123.141592f; }

	FLOAT					Get_m_fHiperMoveSpeed(){ return (m_fHiperMoveSpeed - 133.141592f); }
	void					Set_m_fHiperMoveSpeed(FLOAT speed){ m_fHiperMoveSpeed= speed + 133.141592f; }
	
public:

	// 투명하게 - GM
	_XDEF_ENCVALUE(BOOL, m_bInvisible )
		
	// 복면 
	BOOL					m_bFaceMaskMode;

	// 개인 상점 상태 표시  
	_XUSER_PERSONALSTORESTATE m_PersonalStoreState;
	TCHAR					  m_PersonalStoreName[37];

	// 비무 홍보 표시
	BOOL					m_bMatchPRMode;
	TCHAR					m_stringMatchPR[81];
	
	BOOL					m_bInPartyMember;
	
	BOOL					m_bEnemy;
	
	// 공공의 적
	BOOL					m_bPEMode;
	
	// 자리 비움 상태
	BOOL					m_bOut;

	// 비무 대회 참가중인가?
	BOOL					m_bEnterMatchEvent;	
	int						m_nWinnerNickName;
	
// static functions
public:
	static void				CreateIntersectionPoly( void );
	static BOOL				IntersectTriangle( D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2 );
	

// Get/Set functions
public:		
	virtual void			SetPosition( D3DXVECTOR3 position );	
	virtual void			SetAngle( FLOAT angle );
	virtual void			ResetProcessTime( void );
	
	virtual void			SetMoveSpeed( FLOAT movespeed )
	{
							Set_m_fPrevMoveSpeed(Get_m_fMoveSpeed());
							Set_m_fMoveSpeed(movespeed);
	}

	virtual void			PlayAttackSound( void );
	virtual void			PlayCriticalAttackSound( void );

	virtual void			SetAttackTarget( _XItem* target, _XPICKINGTARGETTYPE targettype );
	virtual void			SetBattleMode(  _XBATTLEMODETYPE mode );
	virtual void			ResetAttackTarget(void);

	virtual void			SetStopAction( void );
	virtual void			SetAnimation( _XMOTIONCLASS_TYPE motionclass, bool changemotion = true );
	virtual void			SetAnimationIndex( int animationindex, bool changemotion = true );
	virtual void			SetCharacterModel( BOOL dntchangemotion = FALSE );
	virtual void			ResetAnimationState( void );
	virtual void			UpdatePosition( void );
	virtual void			ResetZoneData( void );

	// Animation
	inline _XMOTIONCLASS_TYPE	GetMotionClass( void ){ return m_CharacterInfo.animationdescript.motionclass; }
	virtual void			SetDieMotion(void);
	virtual void			SetBlowMotion(void);
	virtual void			SetDefenceMotion(void);
		
// Operation functions
public:
	virtual BOOL			CheckIntersectMouseRay( void );	
	virtual void			Process( void );
	virtual void			Render( void );

	virtual void			CheckInsideFrustum( void );

// Pathfinder functions
public:

	FLOAT					m_PrevMoveTime;

	virtual BOOL			GetNextMove( D3DXVECTOR3& next );
	virtual bool			ProcessCollision(const D3DXVECTOR3& contactpoint, D3DXVECTOR3& movevelocity, D3DXVECTOR3& nextposition);
	
// Battle functions
	virtual void			ProcessBattleActiveSkill(void);
	virtual void			ProcessBattleChargeSkill(void);
	virtual void			ProcessBattleRoundingSpell(void);

	virtual void			ProcessBattleChannelingSpell(void);
	virtual void			ProcessBattleTargetChannelingSpell(void);
	virtual void			ProcessBattleSelfChannelingSpell(void);

	virtual void			ProcessBattleFinishSkill(void);
	virtual void			ProcessBattleCastingSpell(void);
	virtual void			ProcessBattleSmashingSkill(void);
	virtual void			ProcessBattleHiddenMA(void);
	virtual void			ProcessBattleActionSkill(void);
	
	BOOL					DoNotChangeMotion(void);

	// 동작 보간 관련
	virtual void			UpdateRotateAngle(void);
	virtual void			ProcessMobDie(void);

	// stance motion
	void					SetStanceMotion(short index);
	
	// 사후 처리 관련
	virtual void			SetDyingMode(void);
	virtual void			SetDieMode(void);
	virtual void			SetRebirthBinsaMode(void);
	virtual void			SetRebirthHonsuMode(void);
	
	// MISC
	virtual void			DeleteWeaponModel(short skillid);
	virtual void			DeleteWeaponModel(void);
	virtual void			ReloadWeaponModel(void);

	virtual int				GetGroupType(void);

	// 거리 이탈 체크
	void					SetFarAway(bool state, int index);
	
	virtual	BOOL			GetEnterMatchEvent(void);	// 대회 참가중인가?
	BOOL					GetFreeMatchState(void);	// 합비 내에서 위치 여부에 관계없이 영웅대회 한번 참가했음

// Overrides  functions
public:

};

//------------------------------------------------------------------------------------------	
// 유저 리스트
typedef std::map<unsigned short, _XUser*>	smdef_XUser;

struct _XUSER_ORGANIZATION_GLOBAL_INFO 
{
	int				m_iIndex ;
	char			m_cName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;			// 조직 이름
	char			m_cProperty ;			// 조직 속성(분파 | 방회)
	char			m_cBaseParty ;			// 소속 문파
	unsigned int	m_uiExperience ;		// 조직 경험치	
};

// 분파이름 리스트 
typedef std::map<int, _XUSER_ORGANIZATION_GLOBAL_INFO* > smdef_OtganizationInfo;

struct _XUSER_ALIENCE_GLOBAL_INFO 
{
	int				m_iIndex ;
	char			m_cMasterName[13] ;			// 회주 이름
	char			m_cName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;			// 조직 이름
	char			m_cProperty ;			// 조직 속성(분파 | 방회)
	char			m_cBaseParty ;			// 소속 문파
	unsigned int	m_uiExperience ;		// 조직 경험치	
};

// 방회이름 리스트 
typedef std::map<int, _XUSER_ALIENCE_GLOBAL_INFO* > smdef_AlienceInfo;

class _XUserList
{
private:
public:
	XMemoryPool<_XUser>		m_UserPool;
	smdef_XUser				m_smUserList;
	smdef_OtganizationInfo	m_sOrganizationInfoList;
	smdef_AlienceInfo		m_sAlienceInfoList;

	int						m_RenderUserZSortListLength;
	int						m_RenderUserZSortListCurrentLength;
	_XUser**				m_pRenderUserZSortList;

	
public:

	_XUserList();
	~_XUserList();

	void	disposeList(void);
	void	disposeList_zero(void);

	void	CreatePool(void);
	void	DeletePool(void);

private:
	_XUser*	NewUser();
	void	DeleteUser(_XUser* pUser);
	BOOL	AddUser( _XUser* user	 );	

public:
	_XUser* CreateUser(unsigned short usUniqueID, short sAnimationNumber,
								   FLOAT fListAngle, D3DXVECTOR3& vMoveVector,
								   D3DXVECTOR3& vSrcPosition, D3DXVECTOR3& vDestPosition);

	BOOL	AddUser_zero ( _XUser* user	)		{ return AddUser(user); }

	_XUser*	FindUser( LPSTR charactername );
	_XUser*	FindUser( unsigned short uniqueid );

	void	DeleteUser( LPSTR charactername );
	void	DeleteUser( int uniqueid );

	void    Process( void );
	void	Render( void );
	void	Process_ZeroLevel( void );
	void	RenderUserEffectAndToolTip( void );

	void	RemoveEffectInstance(int templateid);

	void	CheckInsideFrustum( void );
	void	AddOrganizationInfo( _XUSER_ORGANIZATION_GLOBAL_INFO* info );
	_XUSER_ORGANIZATION_GLOBAL_INFO*	SearchOrganizationInfo(int index);
	void AddAlienceInfo( _XUSER_ALIENCE_GLOBAL_INFO* info );
	_XUSER_ALIENCE_GLOBAL_INFO*	SearchAlienceInfo(int index);

	void	RemoveSortBuffer( _XUser* pUser );
	// 명성 단계 구하기
	int		CalcHonorStep(int count);
};


//------------------------------------------------------------------------------------------	
// _XUser global extern value
extern _XUSERLEVEL_LUTTYPE		g_UserLevelTable[241];
extern _XUSER_POINTTABLE		g_UsePointTable[12];
extern D3DXVECTOR3				g_UserIntersectionPoly[8];
extern TCHAR					g_CharacterRollNameTable[_XGROUP_MAXGROUPCOUNT][_XROLLCLASS_MAXCOUNT][5][64];

#endif // !defined(_XUSERINFO_H_)
