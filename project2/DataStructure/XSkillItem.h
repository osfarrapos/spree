// XSkillItem.h: interface for the _XSkillItem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XSKILLITEM_H_
#define _XSKILLITEM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XGameItemDefine.h"

#pragma warning (disable : 4786)
#include <map>
using namespace std;

#define _XDEF_MAXSKILLCOUNT							2000
#define _XDEF_SKILLTABLE_HEADERNAMELENGTH			48
#define _XDEF_MAX_LEARNMASTERYSKILL					96

#ifdef _XDEF_ADD_MAX_LEARNSKILL_080522_KUKURI
	#define _XDEF_MAX_LEARNSKILL						80
#else
	#define _XDEF_MAX_LEARNSKILL						60
#endif


typedef enum _XSI_SkillCType
{
	_XSI_CTYPE_MARTIALARTS = 1,		// 무공
	_XSI_CTYPE_SPELL				// 기공
};

/*
typedef enum _XSI_SkillSType
{
	_XSI_STYPE_FIST = 1,			// 권각
	_XSI_STYPE_WEAPON,				// 무기
	_XSI_STYPE_HIDDEN,				// 암기
	_XSI_STYPE_GROUP,				// 진법
	_XSI_STYPE_FORCE,				// 내가 기공
	_XSI_STYPE_MAGIC,				// 기문 방술
	_XSI_STYPE_MEDITATION,			// 내공 심법
	_XSI_STYPE_SPEED				// 경공
};*/


// skill stype 변경 - 05.01.26
// [개방] 1 공통 / 2 내공심법 / 3 경공 / 4 낙봉술 / 5 휘봉술 / 6 취권    / 7 규화공 / 8 강룡공 / 9 창명공 / 10 취선공 / 11 혼원공 / 12 주전공 / 13 보법
// [비궁] 1 공통 / 2 내공심법 / 3 경공 / 4 호접기 / 5 비접기 / 6 단검술 / 7 신녀공 / 8 음한공 / 9 -        / 10 천환공 / 11 인환공 / 12 지환공 / 13 보법
// [소림] 1 공통 / 2 내공심법 / 3 경공 / 4 파곤술 / 5 포곤술 / 6 권각술 / 7 여래공 / 8 천불공 / 9 수미공 / 10 -        / 11 금강공 / 12 항마공 / 13 보법
// [녹림] 1 공통 / 2 내공심법 / 3 경공 / 4 진창세 / 5 기창세 / 6 격부세 / 7 대력공 / 8 제왕공 / 9 패왕공 / 10 -        / 11 갑신공 / 12 파쇄공 / 13 보법

typedef enum _XSI_SkillStype
{
	_XSI_STYPE_01 = 1,
	_XSI_STYPE_02,
	_XSI_STYPE_03,
	_XSI_STYPE_04,
	_XSI_STYPE_05,
	_XSI_STYPE_06,
	_XSI_STYPE_07,
	_XSI_STYPE_08,
	_XSI_STYPE_09,
	_XSI_STYPE_10,
	_XSI_STYPE_11,
	_XSI_STYPE_12,
	_XSI_STYPE_13
};

typedef enum _XSI_ActionType
{
	_XSI_ACTIONTYPE_MASTERYSKILL = 1,
	_XSI_ACTIONTYPE_COMBATSKILL,
	_XSI_ACTIONTYPE_BUFFSPELL,
	_XSI_ACTIONTYPE_DEBUFFSPELL,
	_XSI_ACTIONTYPE_HOSTILESPELL,
	_XSI_ACTIONTYPE_DEHOSTILESPELL,
};

typedef enum _XSI_UsefulType
{
	_XSI_USEFULTYPE_PASSIVESKILL = 1,
	_XSI_USEFULTYPE_ACTIVESKILL,
	_XSI_USEFULTYPE_CHARGESKILL,
	_XSI_USEFULTYPE_PASSIVESPELL,
	_XSI_USEFULTYPE_CASTINGSPELL,
	_XSI_USEFULTYPE_CHANNELINGSPELL,
	_XSI_USEFULTYPE_FINISHSKILL,
	_XSI_USEFULTYPE_ROUNDINGSPELL,
	_XSI_USEFULTYPE_HIDDENMA,
	_XSI_USEFULTYPE_SMASHINGSKILL,
	_XSI_USEFULTYPE_ACTIONSKILL
};

typedef enum _XSI_TargetType
{
	_XSI_TARGETTYPE_SELF = 1,
	_XSI_TARGETTYPE_OTHER_ENEMY,
	_XSI_TARGETTYPE_OTHER_FRIEND,
	_XSI_TARGETTYPE_SELFOTHER_FRIEND,
	_XSI_TARGETTYPE_SELFOTHER_ENEMY,
	_XSI_TARGETTYPE_OTHER_ALL,
	_XSI_TARGETTYPE_ALL,
	_XSI_TARGETTYPE_PARTY,
	_XSI_TARGETTYPE_POINT,
	_XSI_TARGETTYPE_COMA
};

typedef enum _XSI_Combination
{
	_XSI_COMBINATION_MU = 1,		// 무
	_XSI_COMBINATION_KUA,			// 퀘
	_XSI_COMBINATION_JUNG,			// 정
	_XSI_COMBINATION_GANG,			// 강
	_XSI_COMBINATION_YU,			// 유
	_XSI_COMBINATION_JOONG,			// 중
	_XSI_COMBINATION_GYUNG,			// 경
	_XSI_COMBINATION_HU,			// 허
	_XSI_COMBINATION_SIL,			// 실
	_XSI_COMBINATION_PAE,			// 폐
	_XSI_COMBINATION_GAE,			// 개
	_XSI_COMBINATION_JI,			// 지
	_XSI_COMBINATION_CHON			// 천
};

typedef enum _XSI_FERA
{
	_XSI_FERA_MU = 1,				// 무
	_XSI_FERA_MOK,					// 목
	_XSI_FERA_HWA,					// 화
	_XSI_FERA_TO,					// 토
	_XSI_FERA_KUM,					// 금
	_XSI_FERA_SU,					// 수
	_XSI_FERA_JI,					// 지
	_XSI_FERA_CHON					// 천
};

enum _XSI_PARAMETER
{
	_XSI_PARAM_NONE = 0,
	_XSI_PARAM_DISTANCE,
	_XSI_PARAM_RADIUS,
	_XSI_PARAM_MINATTACKDAMAGE,
	_XSI_PARAM_MAXATTACKDAMAGE,
	_XSI_PARAM_ATTACKRATING,
	_XSI_PARAM_AVOIDRATING,
	_XSI_PARAM_COSTFORCE,
	_XSI_PARAM_COSTMANA,
	_XSI_PARAM_COSTENERGY,
	_XSI_PARAM_ADDBONUS,
	_XSI_PARAM_NUMBER,
	_XSI_PARAM_EFFTIME,
	_XSI_PARAM_EFFPROB_1,
	_XSI_PARAM_EFFVALUE_1,
	_XSI_PARAM_ADDPROB_1,
	_XSI_PARAM_ADDCOUNT_1,
	_XSI_PARAM_ADDVALUE_1,
	_XSI_PARAM_EFFPROB_2,
	_XSI_PARAM_EFFVALUE_2,
	_XSI_PARAM_ADDPROB_2,
	_XSI_PARAM_ADDVALUE_2,
	_XSI_PARAM_ADDCOUNT_2,
	_XSI_PARAM_EFFPROB_3,
	_XSI_PARAM_EFFVALUE_3,
	_XSI_PARAM_ADDPROB_3,
	_XSI_PARAM_ADDVALUE_3,
	_XSI_PARAM_ADDCOUNT_3,
	_XSI_PARAM_EFFPROB_4,
	_XSI_PARAM_EFFVALUE_4,
	_XSI_PARAM_ADDPROB_4,
	_XSI_PARAM_ADDVALUE_4,
	_XSI_PARAM_ADDCOUNT_4,
	_XSI_PARAM_EFFPROB_5,
	_XSI_PARAM_EFFVALUE_5,
	_XSI_PARAM_EFFPROB_6,
	_XSI_PARAM_EFFVALUE_6,
	_XSI_PARAM_EFFPROB_7,
	_XSI_PARAM_EFFVALUE_7,
	_XSI_PARAM_EFFPROB_8,
	_XSI_PARAM_EFFVALUE_8,
	_XSI_PARAM_TOTALCOUNT
};

struct _XSI_SkillEffect
{
	short		id;
	short		prob;
	short		value;
};

struct _XSI_SkillAdditive
{
	short		id;
	short		prob;
	short		value;
	short		count;
};

typedef struct 
{
	short	minAttackDamage;		// 최소 공격력
	short	maxAttackDamage;		// 최대 공격력
	short	attackRating;			// 공격 성공치
	short	avoidRating;			// 회피 성공치
	short	costForce;				// 소모 내공
	short	costHeart;				// 소모 지혜 - 사용하지 않음

	short	reqItemCategory;		// 필요 아이템 카테고리
	short	costMind;				// 소모 의지
	float	finishSpeed;			// 피니쉬 스피드

	int		skillExp;				// 필요한 skill exp
	int		casting;				// 시전 요구 시간
	int		delay;					// 공격 지연 시간
	int		coolDown;				// 재 장전 시간
	char	requirementMission;		// 미션 요구 여부
	short	objectID;				// 미션 ID
	short	addBonus;				// 추가 보너스
	short	number;					// 최대 회수
	short	requirementItemID;		// 필요 item ID
	short	itemConsumed;			// item 사용량
	unsigned short		effectTime;	// 효과 적용시간

	// 효과
	short	effectCount;
	_XSI_SkillEffect*	effect;

	// additive effect
	short	additiveCount;
	_XSI_SkillAdditive*	additiveEffect;

}_XSKILLDETAILINFOSTRUCTURE;


typedef struct 
{
#ifdef _XDEF_NEWSTRINGMANAGER	
	LPTSTR	skillName;					// 기술 이름
	
#ifdef _XJAPANESE
	LPTSTR	skillHiraganaName;			// 기술 이름 : 히라가나
#endif

#else
	TCHAR	skillName[16];				// 기술 이름
#endif
	char	skillGrade;					// 기술 단계
	char	skillCType;					// 기술 분류
	char	skillSType;					// 기술 세부 종류
	char	actionType;					// 공격 종류
	char	usefulType;					// 기술 종류
	char	targetType;					// 대상 종류
	char	clanType;					// 문파 분류
	char	weaponCType;				// 사용 무기 중분류
	char	weaponSType;				// 사용 무기 소분류
	char	classGrade;					// 제자 단계
	short	requirementSkill01;			// 요구 기술 01
	short	requirementSkill02;			// 요구 기술 02
	short	requirementCondition;		// 요구 컨디션
	short	insideType;					// 내공심법 연계 운기요상
	short	requirementLevel;			// 성취단계 요구치
	int		requirementContribution;	// 습득시 소비 문파 기여도
	short	effectApplyType;			// 적용 대상
	
	short	angle;						// 적용 반경 - bsk로 바꾸면서 이름 변경

	char	requirementExcer;			// 수련이 필요한가
	char	martialNumber;				// 초식 개수
	char	attackPointFor1;			// 1초 타격 회수
	char	attackPointFor2;			// 2초 타격 회수
	char	attackPointFor3;			// 3초 타격 회수
	int		speed;						// 공격 속도
	char	combination;				// 연속 계열
	float	distance;					// 공격 거리
	float	radius;						// 공격 범위
	float	finishAttackRange;			// Finish attack range
	float	finishAttackSpeed;			// Finish attack speed
	char	FERA;						// 무공 속성
	char	FERP;						// 오행 속성
	char	specialType;				// 역할 속성
	short	visualEffect;				// 이펙트
	short	stance_male;				// 기본 자세 - 남자
	short	stance_female;				// 기본 자세 - 여자
	short	motion_male;				// 기술 동작 - 남자
	short	intermotion1_male;			// 보간 동작1 - 남자
	short	intermotion2_male;			// 보간 동작2 - 남자
	short	motion_female;				// 기술 동작 - 여자
	short	intermotion1_female;		// 보간 동작1 - 여자
	short	intermotion2_female;		// 보간 동작2 - 여자

#ifdef _XDEF_NEWSTRINGMANAGER	
	LPTSTR	concept;					// 기술 설명
#else
	TCHAR	concept[128];				// 기술 설명
#endif
	
	_XSKILLDETAILINFOSTRUCTURE		detailInfo[12];		// 세부 정보

	// skill 세부 정보 - for tooltip
#ifdef _XDEF_NEWSTRINGMANAGER
	LPTSTR	information;
	int		wordorder;
#else
	TCHAR	information[512];
#endif
	char	parameter[10];
	
}_XSKILLINFOSTRUCTURE;


enum
{
	en_mod_target_type = 0,
	en_mod_distance,
	en_mod_min_dam,
	en_mod_max_dam,
	en_mod_attackrate,
	en_mod_avoidrate,
	en_mod_costinner,
	en_mod_costmind,
	en_mod_finishspeed,
	en_mod_castingtime,
	en_mod_delay,
	en_mod_cooldown,
	en_mod_incre_effecttime,
	en_mod_additive_1_value,
	en_mod_additive_2_value,
	en_mod_additive_3_value,
	en_mod_additive_4_value,
	en_mod_additive_1_prob,
	en_mod_additive_2_prob,
	en_mod_additive_3_prob,
	en_mod_additive_4_prob,
	en_mod_additive_1_time,
	en_mod_additive_2_time,
	en_mod_additive_3_time,
	en_mod_additive_4_time,
	en_mod_total_count
};

// 상태효과로 인한 스킬 값 변경을 위한 테이블 - 클라이언트에서 사용할 것만 저장
typedef struct  
{
	short	minAttackDamage;	// 최소 공격력
	short	maxAttackDamage;	// 최대 공격력
	short	attackRating;		// 공격 성공치
	short	avoidRating;		// 회피 성공치
	short	costForce;			// 소모 내공
	short	costMind;			// 소모 의지
	float	finishSpeed;		// Finish speed
	int		casting;			// 시전 요구 시간
	int		delay;				// 공격 지연 시간
	int		coolDown;			// 재 장전 시간
	
	unsigned short			effectTime;				// 효과 적용시간
	_XSI_SkillAdditive		additiveEffect[4];		// additive 효과

}_XSKILLDETAILINFO_MOD_STRUCTURE;

struct _XSKILLINFO_MOD_STRUCTURE
{
	char	targetType;				// 대상 종류
	float	distance;				// 공격 거리

	_XSKILLDETAILINFO_MOD_STRUCTURE detailInfo[12];

	BOOL	bUse[en_mod_total_count];			// 사용 가능 여부

	_XSKILLINFO_MOD_STRUCTURE() : targetType(0), distance(0)
	{
		memset(detailInfo, 0, sizeof(detailInfo));
		memset(bUse, 0, sizeof(bUse));
	}
};

// 상태효과로 인한 스킬 테이블 내용 변경을 위한 구조체
extern map <short, _XSKILLINFO_MOD_STRUCTURE*> g_Skill_Mod_Property;

// Skill  습득 관련
typedef struct _XSI_LearnSkill_Item
{
	int		nCategory;
	short	sID;
}_XSI_LEARNSKILL_ITEM;

typedef struct _XSI_LearnSkill_Package
{
	TCHAR					cPackageName[32];
	int						nSkillCount;
	_XSI_LEARNSKILL_ITEM*	skillItem;
}_XSI_LEARNSKILL_PACKAGE;


extern map <short, _XSKILLINFOSTRUCTURE*>		g_SkillProperty;
extern int										g_SkillGroupStartIndex[12];
extern map <int, _XSI_LEARNSKILL_PACKAGE>		g_SkillPackage;


class _XSkillItem
{	
public:
	_XSkillItem();
	virtual ~_XSkillItem();
	
	static BOOL			LoadSkillProperty( void );
	static BOOL			LoadSkillDetailProperty(FILE* fileptr, _XSKILLINFOSTRUCTURE* skillinfo);	
	static void			ReleaseSkillProperty(void);

	static void			ReleaseSkillModStructure(void);

	static BOOL			ReLoadSkillProperty( void );

	static BOOL			LoadSkillPackage(void);
	static void			ReleaseSkillPackage(void);

	static BOOL			LoadSkillMasteryProperty(void);
	static void			ReleaseSkillMasteryProperty(void);
	static BOOL			LoadSkillMasteryDisplayProperty(void);
	static void			InitializeStructure(void);

	static BOOL			LoadSkillConceptProperty(void);
	static BOOL			LoadMasterySkillConceptProperty(void);
	static BOOL			ParseClanConcept(FILE* fileptr);
	static BOOL			ParseClassConcept(FILE* fileptr);
	static BOOL			ParseSTypeConcept(FILE* fileptr);
	static BOOL			ParseSkillConcept(FILE* fileptr);
	static BOOL			ParseMasterySkillConcept(FILE* fileptr);
	static char			GetParameter(char* arg);

	static short		GetParameterValue(short skillid, char skilllevel, int param);
	static short		GetMasteryParameterValue(short skillid, char skilllevel, int param);
	
	static BOOL			FindSkillProperty(short skillid);
	static BOOL			FindMasterySkillProperty(short skillid);

	static void			ResetSkillModTable(void);

	static char			GetTargetType(short skillid);
	static float		GetDistance(short skillid);

	static short		GetMinAttackDamage(short skillid, char level);
	static short		GetMaxAttackDamage(short skillid, char level);
	static short		GetAttackRating(short skillid, char level);
	static short		GetAvoidRating(short skillid, char level);
	static short		GetCostForce(short skillid, char level);
	static short		GetCostMind(short skillid, char level);
	static float		GetFinishSpeed(short skillid, char level);
	static int			GetCasting(short skillid, char level);
	static int			GetDelay(short skillid, char level);
	static int			GetCoolDown(short skillid, char level);

	static unsigned short	GetEffectTime(short skillid, char level);
	static short			GetAdditiveValue(short skillid, char level, int index);
	static short			GetAdditiveProb(short skillid, char level, int index);
	static short			GetAdditiveCount(short skillid, char level, int index);
	
	static BOOL			LoadBattleParameter(void);
	static BOOL			GetCommand(char* cmd, FILE* fileptr, bool bnumber = false);
};


// ============================================================================
// Matsery skill
// ============================================================================
struct _XSI_MasterySkillDisplayStruct		// for mastery window
{
	int			skillid;
	int			needprev;
};

//extern _XSI_MasterySkillDisplayStruct	g_SkillMasteryDisplayProperty[2000];
//extern map <int, TCHAR*>		g_SkillMasteryNameProperty;


struct _XSI_MasteryDetailStruct
{
	short				sReqMp;				// 포인트를 올리기 위해 필요한 마스터리 스탯

	char				effectCount;
	_XSI_SkillEffect*	effect;				// 최대 20단계의 이펙트 리스트를 가짐	
	
	char				additiveCount;
	_XSI_SkillAdditive*	additiveEffect;		// 최대 20단계의 additive 이펙트 리스트를 가짐
};


struct _XSI_MasterySkillStruct
{
	short			sMasteryID;				// 오의 아이디
#ifdef _XDEF_NEWSTRINGMANAGER		
	LPTSTR			cName;					// 오의 이름
#else
	char			cName[25];				// 오의 이름
#endif

	short			sTreeLevel;				// 오의 트리 단계

	char			cClanType;				// 문파 분류
	char			cClass;					// 역할 분류
	char			cClassGrade;			// 역할 등급

	char			cMasteryType;			// 오의 분류 - 페이지 1, 2, 3

	char			cMaxMP;					// 찍을 수 있는 최대 포인트
	
	short			sNeedMasteryID;			// 요구 기술 : 반드시 마스터 해야 함
	short			sNeedPageTotalPoint;	// 처음 찍을 때 필요한 페이지의 토탈 포인트
	
	char			cApplyType;				// 적용 대상
	
#ifdef _XTS_MASTERY_04
	short			sAddSkill;				// 추가 무공
#endif

	char			cTargetSkill_Stype;		// 대상이 되는 계열 무공

	short			sTargetSkill_1;			// 대상 무공 1
	short			sTargetSkill_2;			// 대상 무공 2
	short			sTargetSkill_3;			// 대상 무공 3
	short			sTargetSkill_4;			// 대상 무공 4

	char			cWeaponSType;			// 대상 무기 Second Type
	char			cWeaponTType;			// 대상 무기 Third Type

	char			cInside_Type;			// 내력 속성
	char			cSpecial_Type;			// 특수 속성

#ifdef _XDEF_NEWSTRINGMANAGER		
	LPTSTR			pContext;				// 설명
#else
	char			pContext[128];			// 설명
#endif

	_XSI_MasteryDetailStruct	detailInfo[20];

	// skill 세부 정보 - for tooltip
#ifdef _XDEF_NEWSTRINGMANAGER		
	LPTSTR	information;
	int		wordorder;
#else
	TCHAR	information[512];
#endif
	char	parameter[14];
};

extern map <short, _XSI_MasterySkillStruct*>	g_SkillMasteryProperty;

class _XMasterySkillItem
{
public :
	short			skillid;
	char			cPoint;

public :
	_XMasterySkillItem()
	{
		skillid = 0;
		cPoint = 0;
	}
};

// ============================================================================
// Skill Concept - for skill window
// ============================================================================
struct _XSI_SkillConceptStruct
{
#ifdef _XDEF_NEWSTRINGMANAGER
	LPTSTR					clan_concept;
	LPTSTR					stype_concept[30];
	LPTSTR					stype_name[30];
#else
	TCHAR					clan_concept[512];
	TCHAR					stype_concept[30][512];
	TCHAR					stype_name[30][64];
#endif

	short					class_stype[10][30];

	_XSI_SkillConceptStruct()
	{
#ifdef _XDEF_NEWSTRINGMANAGER
		clan_concept	=	NULL;
		memset( stype_concept, 0, sizeof(LPTSTR) * 30 );
		memset( stype_name, 0, sizeof(LPTSTR) * 30 );
#else
		memset(clan_concept, 0, sizeof(clan_concept));
		memset(stype_concept, 0, sizeof(stype_concept));
		memset(stype_name, 0, sizeof(stype_name));
#endif
		memset(class_stype, 0, sizeof(class_stype));
	}
};

extern _XSI_SkillConceptStruct	g_SkillConceptProperty[9];

// ============================================================================
// 유저가 소유한 스킬
// ============================================================================
class _XUserSkill
{
public:
	short			m_sSkillID;					// 무공 아이디
	char			m_cSkillLevel;				// 성장 단계
	
	int				m_iSkillExp;				// 무공 경험치
	char			m_cCurMission;				// 미션 진행 카운트
	
	DWORD			m_dwCooldownStartTime;		// Cool down start time
	DWORD			m_dwCooldownTime;			// Cool down time

	unsigned char	m_ucSkillState;
	
	char			m_cSelectedSkillLevel;		// 퀵슬롯에서 조절된 성
	char			m_cOnOff;					// 0:Off  1:On	passive용

public :
	_XUserSkill();
	virtual ~_XUserSkill();
};

#endif // !defined(_XSKILLITEM_H_)