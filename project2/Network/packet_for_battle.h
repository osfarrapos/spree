#pragma once

// Battle
// Main = 6, 7, 8
// 48 - 71

struct _sHeader;


#define MSG_NO_NPC_BUFF_REQ						48		// NPC 버프 요청 패킷
#define MSG_NO_NPC_BUFF_RESULT					48		// NPC 버프 결과 패킷

#define MSG_NO_B_ATKRES_FROM_MOB				50		// S->C :  몬스터가 유저를 공격

#define MSG_NO_SELF_CHANNEL_START_REQUEST		64		// 셀프 채널링( 운기조식 스펠 ) 사용 시작 요청.
#define MSG_NO_SELF_CHANNEL_CTRL				64		// 셀프 채널링( 운기조식 스펠 ) 사용 결과.
#define MSG_NO_SELF_CHANNEL_INFO				65

#define MSG_NO_REGION_DAMAGE					66		// 영역 공격의 결과

// 미니게임 패킷 수정 후
#define MSG_NO_MINIGAME_START_REQ_C_S		51		// 미니게임 시작 요청
#define MSG_NO_MINIGAME_MISSION_STAGE_S_C	52		// 수행할 단계 (종료 알림 메시지로도 쓰임)
#define MSG_NO_MINIGAME_MAST_STAGE_C_S		52		// 완료한 단계 (종료 요청 메시지로도 쓰임)
#define MSG_NO_MINIGAME_FAIL_S_C			53		// 실패 패킷
#define MSG_NO_MINIGAME_CTRL				54		// 허가 혹은 불가... S->C
#define MSG_NO_MINIGAME_RESULT				55		// 결과.(부가 효과가 있을 때만 간다. )

#define MSG_NO_PK_RECORD					58		// PK 전적


// S->C 몬스터가 유저를 공격
//#define MSG_NO_B_ATKRES_FROM_MOB				34
// 가변길이 패킷 : ucAddPassiveType 이 0이 아닐 경우, 뒤의 float 2 개가 같이 감.
// 0인 경우, 위치값은 가지 않음.
struct MSG_B_ATKRES_FROM_MOB
{
	_sHeader			header;
	u_char				ucMessage;
	
	u_short				usAttackerID;
	u_short				usDefenderID;

	u_char				ucToType : 1;			// 0 : 몬스터   1 : 캐릭터
	u_char				ucAtkInfo : 3;			// atkinfo : 0 for Hit, 1 Critical, 2 Evasion, 3 Block, 4 Shock
	u_char				ucAtkType : 2;			// 몬스터가 어떤 유형의 공격을 하는가

	u_char				ucAddPassiveType : 2;	// 몬스터 패시브에 의한 추가 효과, 0:없음 1:무장해제

	short				sDamage;				// 몬스터가 입힌 데미지
};

struct MSG_B_ATKRES_FROM_MOB_WITH_POS
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_short				usAttackerID ;
	u_short				usDefenderID ;
	
	// atkinfo : 0 for Hit, 1 Critical, 2 Evasion, 3 Block, 4 Shock, 5, 6, 7
	// 
	u_char				ucToType : 1 ;			// 0 몬스터, 1 캐릭터
	u_char				ucAtkInfo : 3 ;			// 0 ~ 7
	u_char				ucAtkType : 2 ;			// 몬스터가 어떤 유형의 공격을 하는가.
	u_char				ucAddPassiveType : 2 ;	// 몬스터 패시브에 의한 추가 효과 : 0 - 없음. 1 - 패시브 스킬 발동.
	short				sDamage ;				// 몬스터가 입힌 데미지.

	char				cPassiveTitle;			// 패시브 기술
	float				fItemDropPosX ;			// 무장해제 발동시 아이템이 떨어질 위치.
	float				fItemDropPosZ ;	
};

enum EN_MOB_PASSIVE_TITLE
{
	mob_passive_none = 0,			// 없음
	mob_passive_release_arm,		// 무장 해제
	mob_passive_steal_item,			// 아이템 강탈
	mob_passive_count
};

/* =====================================================================================
영역 공격의 결과를 주위에 알린다.
===================================================================================== */

#define _COUNT_REGION_ATK_RESULT_			40	// 영역 공격의 대상은 최대 40을 넘을 수 없다.

struct __regionAtkResult
{
	u_char			ucObjType : 4 ;			// 0 몬스터, 1 캐릭터, 2 는 기타( 불,냉기,기운... )
	u_char			ucMobDead : 1;			// 0 : alive, 1 : dead (몬스터에게만 유효)
	u_char			ucResult : 3 ;			// atkinfo : 0 for Hit, 1 Critical, 2 Evasion, 3 Block, 4 Shock, 5, 6, 7
	
	char			cMobVitalRate ;			// 몬스터일 때만, 유효. : 캐릭터는 그냥 맞았다는 정도면 됨.
											// 그리고, 캐릭터에게는 life 패킷이 따로 먼저 간다.
	
	u_short			usID ;					// 피해자의 아이디.
} ;

struct MSG_REGION_DAMAGE
{
	_sHeader			header ;			// 길이는 6 + ucObjectCount * sizeof(__regionAtkResult) 가 된다. 
	u_char				ucMessage ;
	
	u_char				ucAttackerType ;	// 0 몬스터, 1 캐릭터, 2 는 기타( 불,냉기,기운... )
	u_short				usAttackerID ;		// 몬스터나 캐릭터의 아이디.
	
	short				sSkillID ;			// 공격 스킬의 아이디.
	
	u_char				ucObjectCount ;		// 포함된 개체수.
	
	__regionAtkResult	Result[_COUNT_REGION_ATK_RESULT_] ;
} ;


// Self Channeling Attack.
//#define MSG_NO_SELF_CHANNEL_START_REQUEST		64		// 셀프 채널링( 운기조식 스펠 ) 사용 시작 요청.
struct MSG_SELF_CHANNEL_START_REQUEST
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cMode;					// 0 : 시작 요청, 1 : 계속, 2 : 중지
	
	u_char				ucLearnSkillIndex ;		// 슬롯 인덱스로 보내고
	short				skillID ;				// 아이디 인증. 
} ;

//#define MSG_NO_SELF_CHANNEL_START_CTRL	64		// 셀프 채널링( 운기조식 스펠 ) 사용 시작 요청.
struct MSG_SELF_CHANNEL_CTRL		// 자신에게만 감. 서버에서 제어.
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cResultInfo ;		// 0 : 시작하라. 1 : 완료(타임아웃) 
	// 2 : 공격 등에 의해 취소 되었다.
	// 3 : 그런 스킬 없다(스킬 아이디 안맞음) 
	// 4 : 스킬은 운기 조식 스킬이 아니다. 
	
	short				skillID ;			// 이 스킬을 시작하라.
} ;

//#define MSG_NO_SELF_CHANNEL					65
struct MSG_SELF_CHANNEL_INFO		// 주위에 알려주는 패킷 : 자신은 무시.
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cFlag ;			// 0 : 스킬 시전 시작. 1 : 스킬 시전 중지. 2: 시전중
	
	u_short				usID ;			// 자신이면 무시.
	short				skillID ;		// 
} ;

//=======================================================================
// 미니게임 패킷 수정
//=======================================================================
//#define MSG_NO_MINIGAME_START_REQ_C_S		51		// 미니게임 시작 요청
//#define MSG_NO_MINIGAME_MISSION_STAGE_S_C	52		// 수행할 단계 (종료 알림 메시지로도 쓰임)
//#define MSG_NO_MINIGAME_MAST_STAGE_C_S		52		// 완료한 단계 (종료 요청 메시지로도 쓰임)
//#define MSG_NO_MINIGAME_FAIL_S_C			53		// 실패 패킷
//#define MSG_NO_MINIGAME_CTRL				54		// 허가 혹은 불가... S->C
//#define MSG_NO_MINIGAME_RESULT				55		// 결과.(부과 효과가 있을 때만 간다. )
struct MSG_MINIGAME_START_REQ 
{
	_sHeader			header ;
	u_char				ucMessage ;

	short				sMiniGameID ;			// 미니게임 종류
	u_char				ucLearnSkillIndex ;		// 무공의 슬롯 번호
	short				skillID ;				// 무공의 아이디.. 
};

struct MSG_MINIGAME_MISSION_STAGE
{
	_sHeader			header ;
	u_char				ucMessage ;

	int					iKey ;						// 해킹 방지를 위한 키 이다.
	char				cMissionStage ;				// 진행할 단계 (0 이면 종료, -1 이면 몸에의한 종료)
};

struct MSG_MINIGAME_MAST_STAGE {
	_sHeader			header ;
	u_char				ucMessage ;

	int					iKey ;						// 인증키 이다.
	char				cMastStage ;				// 완료한 단계(0 이면 종료 요청한거다)
};


struct MSG_MINIGAME_FAIL {
	_sHeader			header ;
	u_char				ucMessage ;

	u_char				ucCommand ;					// 실패한 패킷 번호
	u_char				ucCode ;					// 실패 코드
};

enum {								// FAIL CODE
	_FAIL_CODE_HAVENT_SKILL,
	_FAIL_CODE_BED_KEY,
	_FAIL_CODE_ERROR,
	_FAIL_CODE_NEED_LAST_LEVEL,
	_FAIL_CODE_NEED_FULL_POWER
};

/*============================================================================

  MSG_MINIGAME_FAIL 설명

	ucCommand : 어떤 패킷에 대한 결과 패킷인지 구분하기 위해..
	ucCode	  : 실패 코드

--------------------------------------------+-----------------------------------+---------------------------
 	ucCommand								|	iCode							|		내용
--------------------------------------------+-----------------------------------+---------------------------
 	MSG_NO_MINIGAME_START_REQ_C_S			|	_FAIL_CODE_HAVENT_SKILL			|	해당 무공 없음
 											|	_FAIL_CODE_ERROR				|	기타 오류..					
											|	_FAIL_CODE_NEED_LAST_LEVEL		|	마지막 레벨이 아님..
											|	_FAIL_CODE_NEED_FULL_POWER		|	건강 또는 내공이 꽉 차지 않았음
--------------------------------------------+-----------------------------+---------------------------------
	MSG_NO_MINIGAME_MAST_STAGE_C_S			|	_FAIL_CODE_BED_KEY				|	나쁜 인증키.. (ㅡㅡ;)
											|	_FAIL_CODE_ERROR				|	기타 오류..											
--------------------------------------------+-----------------------------+---------------------------------

=============================================================================*/

enum {
	minigame_en_allow_start = 0,
	minigame_en_not_allow_or_finish,
	minigame_en_cancel_from_monster
} ;

// 수련(미니게임) 종료 모드
enum {
	_MODE_END_TRAINING_SELF_CANCEL,			// 사용자 취소..
	_MODE_END_TRAINING_COMPLETE			// 성공적 완료
};

struct MSG_MINIGAME_CTRL
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucCtrl ;			// 0 : Yes(시작), 1 : No,or 정상종료 2 : 몹에 의한 중지.
	short				skillID ;				// 무공의 아이디.. 
	short				sMiniGameID ;			// 미니게임 종류
	
	u_short				usID ;					// 
} ;

struct MSG_MINIGAME_RESULT					// 부과 효과가 있을때 자신에게만.
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	short				sAddBufEffect ;			// 추가 효과.
	u_short				usApplyTime ;			// 시간.
	u_char				ucEffectPercent ;		// 효과 %.. 이런게 필요할지 모르겠음.
} ;


// 전투 관련 패킷 변경 2004.08.17

#define MSG_NO_MARTIAL_READY					68
#define MSG_NO_MARTIAL_FIRE						69
#define MSG_NO_MARTIAL_RESULT					70
#define MSG_NO_MARTIAL_END						71

// 무공 패킷의 martial type에 들어간다.
enum SKILL_USEFUL_TYPE_EN
{
	en_martial_ut_passive_skill=1,		// 01
	en_martial_ut_active_skill,			// 02
	en_martial_ut_charge_skill,			// 03
	en_martial_ut_passive_spell,		// 04
	en_martial_ut_casting_spell,		// 05
	en_martial_ut_channeling_spell,		// 06
	en_martial_ut_finish_skill,			// 07
	en_martial_ut_rounding_spell,		// 08
	en_martial_ut_hidden_ma,			// 09
	en_martial_ut_smashing_skill,		// 10
	en_martial_ut_action_skill			// 11
};

// 타겟 타입( TargetType )/ 타겟 타입( TargetType )
enum BATTLE_TARGET_TYPE_EN
{
	en_target_type_monster=0,	// 0 : 몬스터
	en_target_type_npc,			// 1 : npc.
	en_target_type_dummy,		// 2 : 더미( 수련, 관문.. )
	en_target_type_others,		// 3 : 다른 플레이어.( PvP )
	en_target_type_self,		// 4 : 자신에게 하는 것.
	en_target_type_point,		// 5 : 채널링 또는 캐스팅에서 어떤 점에 시전할 때.
	en_target_type_object		// 6 : Functional Object
};

enum EN_CHARAC_BATTLE_RESULT
{
	en_battle_result_normal=0,			// 살아있음. 계속 전투.
	en_battle_result_binsa_or_die,		// 캐릭터 : 빈사에 빠졌음, 몬스터 : 방금 죽음
	en_battle_result_honsu_or_dead,		// 캐릭터 : 혼수상태로 가라, 몬스터 : 이미 죽은 몬스터.
	en_battle_result_dead_in_match,		// 비무중 죽음.
	en_battle_result_dead_in_freematch	// 자유비무중 죽음
};


// 무공 시작
struct MARTIAL_READY
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucMartialType : 4 ;		// 무공 타입 
	u_char				ucTargetType : 3 ;		// 대상의 타입
	u_char				ucPadding1 : 1 ;		// 패딩
};

// Fire
struct MARTIAL_FIRE
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucMartialType : 4 ;		// 무공 타입 
	u_char				ucTargetType : 3 ;		// 대상의 타입
	u_char				ucPadding1 : 1 ;		// 패딩
};

// Result of fire
struct MARTIAL_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucMartialType : 4 ;		// 무공 타입 
	u_char				ucTargetType : 3 ;		// 대상의 타입
	u_char				ucPadding1 : 1 ;		// 패딩
	
	short				sLeftForce ;			// 사용후 남은 내공
};

// martial end
struct MARTIAL_END
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucMartialType : 4 ;		// 무공 타입 
	u_char				ucTargetType : 3 ;		// 대상의 타입
	u_char				ucPadding1 : 1 ;		// 패딩
	
	u_short				usAttackerID ;
	char				cReason ;
	
};

///////////////////////////////////////////////////////////////////////////////
// Active Skill
// Martial type : en_martial_ut_active_skill	// 02
///////////////////////////////////////////////////////////////////////////////
// 1. Fire
struct MSG_ACTIVE_FIRE : public MARTIAL_FIRE
{
	u_char				ucSequence : 2 ;		// 시퀀스.
	u_char				ucPadding : 6 ;			// 
	
	u_short				usTargetID ;			// 대상.
	short				sAnimation ;			// 애니메이션.
	
} ;

// 2. Result
struct MSG_ACTIVE_RESULT_MOB : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	
	short				sAnimation ;
	
	u_short				ucSequence : 2 ;		//
	u_short				usAtkInfo_1 : 3 ;		// atkinfo : 0 for Hit, 1 Critical, 2 Evasion, 3 Block, 4 Shock, 5, 6, 7
	u_short				usAtkInfo_2 : 3 ;		//
	u_short				usAtkInfo_3 : 3 ;		//
	u_short				usPadding : 5 ;			// 
	
	u_short				usDamage_1 ;
	char				cMobVitalRate_1 ;		//
	
	u_short				usDamage_2 ;
	char				cMobVitalRate_2 ;		//
	
	u_short				usDamage_3 ;
	char				cMobVitalRate_3 ;		//
	
	char				cBattleResult ;			// 전투 결과.
} ;


struct MSG_ACTIVE_RESULT_PVP : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	
	
	u_char				ucPadding : 6 ;
	short				sAnimation ;
	
	u_char				ucSequence : 2 ;
	u_short				usAtkInfo_1 : 3 ;		// 
	u_short				usAtkInfo_2 : 3 ;		//
	u_short				usAtkInfo_3 : 3 ;		//
	u_short				usPadding : 5 ;			// 
	
	u_short				usDamage_1 ;
	u_short				usDamage_2 ;
	u_short				usDamage_3 ;
	
	char				cBattleResult ;			// EN_CHARAC_BATTLE_RESULT
} ;


///////////////////////////////////////////////////////////////////////////////
// Charge Skill
// Martial type : en_martial_ut_charge_skill	// 03
///////////////////////////////////////////////////////////////////////////////
// 1. Ready
struct MSG_CHARGE_READY : public MARTIAL_READY
{
	u_short				usTargetID ;
	u_char				ucLearnSkillIndex ;		//
} ;

// 1-1 Ready Response
struct MSG_CHARGE_READY_RES : public MARTIAL_READY
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sChargeSkillID ;
} ;


// 2. Fire
struct MSG_CHARGE_FIRE : public MARTIAL_FIRE
{
	short				sChargeTime ;
} ;


// 3. Result
struct MSG_CHARGE_RESULT_MOB : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sChargeSkillID ;
	char				cSkillStep ;
	char				cMobVitalRate ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;			// 
	
	char				cBattleResult ;		// 
} ;

struct MSG_CHARGE_RESULT_PVP : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sChargeSkillID ;
	char				cSkillStep ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;			// 
	
	char				cBattleResult ;
} ;

// 4. End
struct MSG_CHARGE_END : public MARTIAL_END
{
} ;


///////////////////////////////////////////////////////////////////////////////
// Casting Spell
// Martial type : en_martial_ut_casting_spell	// 05
///////////////////////////////////////////////////////////////////////////////
// 대상이 pc 또는 npc 일때,
struct MSG_CASTING_READY_TO_PC : public MARTIAL_READY
{
	u_short				usTargetID ;
	u_char				ucLearnSkillIndex ;
	char				cStep ;
} ;

struct MSG_CASTING_READY_RES_TO_PC : public MARTIAL_READY
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sCastingSpellID ;
	char				cSpellStep ;
} ;



// 포인트.
struct MSG_CASTING_READY_TO_POINT : public MARTIAL_READY
{
	u_char				ucLearnSkillIndex ;
	char				cSpellStep ;
	float				fTargetPosX ;
	float				fTargetPosZ ;
} ;

struct MSG_CASTING_READY_RES_TO_POINT : public MARTIAL_READY
{
	u_short				usAttackerID ;
	short				sCastingSpellID ;
	char				cSpellStep ;
	float				fTargetPosX ;
	float				fTargetPosZ ;
} ;


// 2. Fire
struct MSG_CASTING_FIRE : public MARTIAL_FIRE
{
	// 없음. 
} ;


// 3. Result
struct MSG_CASTING_RESULT_TO_MOB : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sCastingSpellID ;
	char				cSpellStep ;
	char				cMobVitalRate ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;			// 
	
	char				cBattleResult ; 
} ;

struct MSG_CASTING_RESULT_TO_PVP : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sCastingSpellID ;
	char				cSpellStep ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;			// 
	
	char				cBattleResult ; 
} ;


struct MSG_CASTING_RESULT_TO_POINT : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	short				sCastingSpellID ;
	char				cSpellStep ;
	float				fTargetPosX ;
	float				fTargetPosZ ;	
} ;

// 4. End
struct MSG_CASTING_END : public MARTIAL_END
{
} ;


///////////////////////////////////////////////////////////////////////////////
// Channeling Spell
// Martial type : en_martial_ut_channeling_spell	// 06
// Ready 이후, Fire & Result 연속
///////////////////////////////////////////////////////////////////////////////
// 1. Ready
struct MSG_CHANNEL_READY_SELF : public MARTIAL_READY
{
	u_char				ucLearnSkillIndex;
	char				cSpellStep;
};

struct MSG_CHANNEL_READY_POINT : public MARTIAL_READY
{
	u_char				ucLearnSkillIndex ;
	char				cSpellStep ;
	float				fTargetPosX ;
	float				fTargetPosZ ;
} ;

struct MSG_CHANNEL_READY_PC : public MARTIAL_READY
{
	u_char				ucLearnSkillIndex ;
	char				cSpellStep ;
	u_short				usTargetID ;				// PC의 아이디.
} ;

struct MSG_CHANNEL_READY_RES_SELF : public MARTIAL_READY 
{
	u_short				usAttackerID;				// 이 유저가
	short				sChannelSpellID;			// 이 스킬의 
	char				cSpellStep;					// 이 단계로 제자리에서 셀프 채널링을 시작하려 한다.
};


struct MSG_CHANNEL_READY_RES_POINT : public MARTIAL_READY
{
	u_short				usAttackerID ;
	short				sChannelSpellID ;
	char				cSpellStep ;
	float				fTargetPosX ;
	float				fTargetPosZ ;
} ;

struct MSG_CHANNEL_READY_RES_PC : public MARTIAL_READY
{
	u_short				usAttackerID ;
	short				sChannelSpellID ;
	char				cSpellStep ;
	u_short				usTargetID ;
} ;


// 2. Fire
struct MSG_CHANNEL_FIRE : public MARTIAL_FIRE
{
	//
} ;


// 3. Result
struct MSG_CHANNEL_RESULT_POINT : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	short				sChannelSpellID ;
	char				cSpellStep ;
	float				fTargetPosX ;
	float				fTargetPosZ ;
	float				fRadius;			// 적용 반경
} ;

// 캐릭터에게 쏠 때 : en_target_type_others
struct MSG_CHANNEL_RESULT_PC : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	short				sChannelSpellID ;
	char				cSpellStep ;
	u_short				usTargetID ;
	float				fRadius;			// 적용 반경
	char				cBattleResult;		// 대상이 죽었는지 살았는지

	u_char				usAtkInfo : 3;
	u_char				ucPadding : 5;
	u_short				sDamage;
} ;

// en_target_type_monster, en_target_type_npc, en_target_type_dummy
struct MSG_CHANNEL_RESULT_NPC : public MARTIAL_RESULT 
{
	u_short				usAttackerID;
	short				sChannelSpellID;
	char				cSpellStep;
	u_short				usTargetID;
	float				fRadius;
	char				cVitalRate;
	char				cBattleResult;
	
	u_char				usAtkInfo : 3;
	u_char				ucPadding : 5;
	u_short				sDamage;
};


// 4. End
struct MSG_CHANNEL_END : public MARTIAL_END
{
} ;


///////////////////////////////////////////////////////////////////////////////
// Finish Skill
// Martial type : en_martial_ut_finish_skill	// 07
///////////////////////////////////////////////////////////////////////////////
// 1. Ready
struct MSG_FINISH_READY : public MARTIAL_READY
{
	u_short				usTargetID ;
	u_char				ucLearnSkillIndex ;
	char				cSkillStep ;
} ;


struct MSG_FINISH_READY_RES : public MARTIAL_READY
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sFinishSkillID ;
	char				cSkillStep ;
} ;


// 2. Fire
struct MSG_FINISH_FIRE : public MARTIAL_FIRE
{
	float				fTargetPosX ;
	float				fTargetPosZ ;
} ;


// 3. Result
struct MSG_FINISH_RESULT_TO_MOB : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sFinishSkillID ;
	char				cSkillStep ;
	
	float				fTargetPosX ;
	float				fTargetPosZ ;
	
	char				cMobVitalRate ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;
	
	char				cBattleResult ;
} ;

struct MSG_FINISH_RESULT_TO_PC : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sFinishSkillID ;
	char				cSkillStep ;
	
	float				fTargetPosX ;
	float				fTargetPosZ ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;
	
	char				cBattleResult ;
} ;

// 4. End
struct MSG_FINISH_END : public MARTIAL_END
{
} ;


///////////////////////////////////////////////////////////////////////////////
// Rounding Spell
// Martial type : en_martial_ut_rounding_spell	// 08
///////////////////////////////////////////////////////////////////////////////
// 2. Fire
struct MSG_ROUNDING_FIRE : public MARTIAL_FIRE
{
	u_short				usTargetID ;
	u_char				ucLearnSkillIndex ;
	char				cSpellStep ;
} ;


// 3. Result
struct MSG_ROUNDING_RESULT_TO_MOB : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sRoundingSpellID ;
	char				cSpellStep ;
	char				cMobVitalRate ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;
	
	char				cBattleResult ;
} ;

struct MSG_ROUNDING_RESULT_TO_PC : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sRoundingSpellID ;
	char				cSpellStep ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;
	
	char				cBattleResult ;
} ;

struct MSG_ROUNDING_FIRE_RES : public MARTIAL_FIRE  // Fire에 대한 답, Fire를 보내고 나면 바로 옴
{
	//
	u_short				usAttackerID ;
	u_short				usTargetID ;
	
	short				sSkillID ;
	char				cSpellStep ;		// Result Step
	
	short				sLeftForce ;
} ;

struct MSG_ROUNDING_FIRE_TO_POINT : public MARTIAL_FIRE
{
	u_char				ucLearnSkillIndex;
	char				cSpellStep;

	float				fTargetX;
	float				fTargetZ;
};

struct MSG_ROUNDING_FIRE_RES_TO_POINT : public MARTIAL_FIRE
{
	u_short				usAttackerID;
	short				sSkillID;
	char				cSpellStep;

	float				fTargetX;
	float				fTargetZ;

	short				sLeftForce;
};

struct MSG_ROUNDING_RESULT_TO_POINT : public MARTIAL_RESULT
{
	u_short				usAttackerID;
	short				sRoundingSpellID;
	char				cSpellStep;

	float				fTargetX;
	float				fTargetZ;
};

//////////////////////////////////////////////////////////////////////////
// Smashing Skill : 일격기
// Martial Type : en_martial_ut_smashing_skill,		// 02
// Fire & Result : Fire 하면, 바로 Result 만 간다.
// vs Monster, vs Player 에 따라 다르므로 결과 패킷 2개.
//////////////////////////////////////////////////////////////////////////
// 1. Fire
struct MSG_SMASHING_FIRE : public MARTIAL_FIRE
{
	u_short				usTargetID ;			// 대상.
	
	u_char				ucLearnSkillIndex ;		// 시전 일격기의 스킬 슬롯 번호.
	char				cSpellStep ;
} ;


// 2. Result : To Monster : Rounding 과 유사.
struct MSG_SMASHING_RESULT_MOB : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	
	short				sSmashingSkillID ;
	char				cSkillStep ;
	
	char				cMobVitalRate ;
	
	u_char				ucAtkInfo : 3 ;			// atkinfo : 0 for Hit, 1 Critical, 2 Evasion, 3 Block, 4 Shock, 5, 6, 7
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;
	
	char				cBattleResult ;
} ;

// To Other
struct MSG_SMASHING_RESULT_PVP : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sSmashingSkillID ;
	char				cSkillStep ;
	
	u_char				ucAtkInfo : 3 ;			// atkinfo : 0 for Hit, 1 Critical, 2 Evasion, 3 Block, 4 Shock, 5, 6, 7
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;
	
	char				cBattleResult ;
} ;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Action skill
// 행동은 단타이므로, fire 보내면, 바로 result 가 갑니다.
struct MSG_ACTION_FIRE : public MARTIAL_FIRE
{
	u_short				usTargetID;				// 대상 id
	u_char				ucLearnSkillIndex;		
	char                cSkillStep ;            // 단계.
};

struct MSG_ACTION_RESULT : public MARTIAL_RESULT
{
	u_short				usAttackerID;
	u_short				usTargetID;
	short				sActionSkillID ;		// 해당 스킬
	char                cSkillStep ;
	u_char				ucResult;				// 0성공 1실패 2내력부족 3복면상태가 아님 4대상없음
};

enum EN_ACTION_RESULT 
{
	en_action_result_success = 0,				// 성공
	en_action_result_fail_or_avoid,				// 회피
	en_action_result_need_force,				// 내력 부족
	en_action_result_no_mask,					// 대상이 복면 상태가 아닙니다.
	en_action_result_no_target,					// 대상이 없다.
	en_action_result_no_effect,					// 
	en_action_result_no_skill,					// 그런 스킬 없음.
	en_action_result_cool_time_error,			// 
	en_action_result_end						// 
};


// 무공 사용 실패 이유들 : charge, casting, finish 등으로 분리되있던거, 하나로 통합.
enum EN_MARTIAL_END_ERROR
{
	en_martial_end_normal=0,			// 정상.
	en_martial_end_no_martial,			// 없는 무공 : 준비 단계에서 실패.
	en_martial_end_no_force,			// 내력 모자름.
	en_martial_end_no_select,			// Ready 가 되지 않았거나, 없는 무공.
	en_martial_end_invalid_target,		// 타겟이 없다. : 4
	en_martial_end_need_training,		// 수련이 필요하다. 5
	en_martial_end_cooltime_error,		// 쿨타임이 지나지 않았다. 6
	en_martial_end_distance_too_far		// 거리가 너무 멀다. 7
} ;

#define _INVALID_SKILL_SLOT_			0xff		// 무효 스킬 슬롯값으로, skill add, find 등의 실패 리턴값으로 쓰인다. 


// 상태 효과
#define MSG_NO_EFFECT_CTRL					56			// 이팩트/상태를 추가 삭제
#define MSG_NO_EFFECT_INFO					57			// 필드 들어갈 때 자신의 상태 정보를 알려준다.

// Buff/Hostile의 정보
struct _sUser_BHEntry
{
	short				skillID;					// 스킬 아이디
	unsigned short		usLeftTimeSec;				// 남은 시간
	char				skillLevel;					// 스킬의 단계
};

// 효과 요소 정보
struct _sUser_EffectEntry
{
	short				id;
	unsigned short		usLeftTimeSec;				// 남은 시간 (초)
	short				value;						// 값
};

struct _sUser_ExtraEntry
{
	char				cWhere ;			// 0 - 9 : 10 칸 예약되 있음.( 음식, 술, 강호풍운록.. )
	short				id ;
	unsigned short		usLeftTimeSec ;		// 남은 시간 ( '초', ms 까지 정확할 필요 없다. )
	short				value ;				// 효과 요소는 단계가 없다. 그래서 값이 필요.
};

//Author : 양희왕 //breif : 점혈술 버프 추가
struct _sUser_SpecialBuff
{
	short	skillID;	// 스킬의 아이디
	unsigned short	usLeftTimeSec;	// 남은 시간
	char	skillLevel;	// 스킬의 단계
	u_char	slot;
};

// 스킬 효과와 컨티션 정보 - 둘다 있을 경우 두번 옴.
// 가변 길이 패킷 cCount 만큼
// Buff인지 Hostile인지는 클라이언트에서 알아서 처리해야함
enum
{
	eff_info_mode_my_bh_list = 0,
	eff_info_mode_my_entry_list,
	eff_info_mode_other_bh_list,
	eff_info_mode_my_extra_list,		// 음식, 술등의 기타 상태효과
	eff_info_mode_other_entry_list,		// 다른 캐릭터의 상태효과 리스트.
	eff_info_mode_special_buff_list	// 스페셜 버프 리스트 //Author : 양희왕 //breif : 점혈술 버프 추가
};

struct MSG_EFFECT_INFO
{
	_sHeader			header;
	u_char				ucMessage;

	char				cMode;
};

#define _MAX_CHARAC_BUFF_					16			// 최대 Buff 의 개수
#define _MAX_CHARAC_HOSTILE_				16			// 최대 Hostile의 개수
#define _MAX_CHARAC_BH_ENTRY_				32			// 위에 두개 합 : 저장 개수 때문에 

#define _MAX_CHARAC_EFFECT_ENTRY_			32			//
#define _MAX_CHARAC_CONDITION_ENTRY_		16			// 
#define _MAX_EXTRA_EFFECT_ENTRY_			10			// extra 최대값 10
#define _MAX_CHARAC_SPECIAL_BUFF_			1			// 스페셜 버프 최대값인가..


struct MSG_EFFECT_MY_BH_INFO : public MSG_EFFECT_INFO		// 상태 효과 정보 (가변)
{
	char				cCount;
	_sUser_BHEntry		entry[_MAX_CHARAC_BH_ENTRY_];
};

struct MSG_EFFECT_MY_ENTRY_INFO : public MSG_EFFECT_INFO	// 효과 리스트	(가변)
{
	char				cCount;
	_sUser_EffectEntry	entry[_MAX_CHARAC_EFFECT_ENTRY_];
};

struct MSG_EFFECT_OTHER_INFO : public MSG_EFFECT_INFO		// 스킬 이펙트만 온다, 컨티션은 Zone charac info에 묻어옴
{
	u_short				usID;
	char				cCount;
	short				id[_MAX_CHARAC_BH_ENTRY_];
};

// 자신의 기타 상태효과 리스트.
struct MSG_EFFECT_MY_EXTRA_INFO : public MSG_EFFECT_INFO
{
	char				cCount ;		// 카운트로 잘라서 보낸다.
	
	_sUser_ExtraEntry	entry[_MAX_EXTRA_EFFECT_ENTRY_] ;
} ;

//Author : 양희왕 //breif : 점혈술 버프 추가
struct MSG_EFFECT_SPECIAL_BUFF_INFO : public MSG_EFFECT_INFO
{
	char	cCount;
	_sUser_SpecialBuff	entry[_MAX_CHARAC_SPECIAL_BUFF_];
} ;

enum EN_EFFECT_CTRL_REQUEST_MODE
{
	en_eff_ctrl_rq_timeout_buff = 0,		// 버프 스킬 타임아웃 요청
	en_eff_ctrl_rq_timeout_hostile,			// 하슬 스킬 타임아웃 요청
	en_eff_ctrl_rq_timeout_effect,			// 효과 요소 타임아웃 요청
	en_eff_ctrl_rq_timeout_special,			// 스페셜 버프의 삭제( id 란에, 슬롯 번호를 쓸것 ) //Author : 양희왕
	
	en_eff_ctrl_rq_calc_buff,				// 버프에 있는 인터벌 효과의 계산을 요청한다. 예를 들어, 2초마다 체력 회복.
	en_eff_ctrl_rq_calc_hostile,			// 하슬에 있는 인터벌 효과의 계산을 요청한다. 예를 들어, 3초마다 독. 
	en_eff_ctrl_rq_calc_effect				// 효과 요소의 인터벌 계산을 요청한다. 예를 들어, 3초마다 독.
};

struct MSG_EFFECT_CTRL_REQUEST 
{
	_sHeader			header;
	u_char				ucMessage;

	char				cMode;

	short				id;
};

enum EN_EFFECT_CTRL_RESULT_MODE
{
	// 스킬
	en_eff_ctrl_rs_add_skill=0,				// 0	스킬 추가. 
	en_eff_ctrl_rs_update_skill,			// 1	스킬 시간, 단계 : 위와 이건, 전투중 서버에서 보냄.
	en_eff_ctrl_rs_del_skill,				// 2	효과 삭제 : Client 의 요청에 의해, 또는 서버에서 보냄.
	en_eff_ctrl_rs_del_skill_rq_fail,		// 3	시간이 안맞음 : 지우지 말고, time으로 업데이트
	en_eff_ctrl_rs_del_all_buff,			// 4	모든 버프 효과 해제.
	en_eff_ctrl_rs_del_all_hostile,			// 5	모든 하슬 표과 해제.
	en_eff_ctrl_rs_skill_reserve3,			// 6
	en_eff_ctrl_rs_skill_reserve4,			// 7
	
	// 자기 효과
	en_eff_ctrl_rs_add_effect,				// 8	상태 추가 ( 상태의 아이디 )
	en_eff_ctrl_rs_update_effect,			// 9	상태 업데이트
	en_eff_ctrl_rs_del_effect,				// 10	상태 삭제.
	en_eff_ctrl_rs_del_effect_rq_fail,		// 11	시간 안맞음. : 지우지 말고, time으로 업데이트
	en_eff_ctrl_rs_del_all_effect,			// 12	걸려 있는 모든 효과 해제. 상태 초기화.
	en_eff_ctrl_rs_update_value_only,		// 13	id에 해당하는 효과 값만 업데이트, 시간은 유지
	//en_eff_ctrl_rs_condition_reserve2,		// 13
	en_eff_ctrl_rs_condition_reserve3,		// 14
	en_eff_ctrl_rs_condition_reserve4,		// 15
	
	// 다른 사람의 정보 조정.
	en_eff_ctrl_rs_other_add_skill,			// 16	스킬 추가
	en_eff_ctrl_rs_other_del_skill,			// 17	스킬 삭제
	en_eff_ctrl_rs_other_del_all_skill,		// 18	모든 스킬 삭제
	en_eff_ctrl_rs_other_skill_res,			// 19	
	en_eff_ctrl_rs_other_add_effect,		// 20	효과 추가
	en_eff_ctrl_rs_other_del_effect,		// 21	효과 삭제
	en_eff_ctrl_rs_other_del_all_effect,	// 22	모든 효과 삭제
	en_eff_ctrl_rs_other_effect_res,			// 23

	// extra 효과
	en_eff_ctrl_rs_add_extra,				// 24	extra 효과의 추가(음식, 투척보조류, 퀘스트)
	en_eff_ctrl_rs_update_extra,			// 25	extra 효과의 수정
	en_eff_ctrl_rs_del_extra,				// 26	extra 효과의 삭제
	
	// 무공/경공 취소
	en_eff_ctrl_rs_cancel_current = 30,		// 30	시전중인 스킬(스펠) 취소
	en_eff_ctrl_rs_cancel_hyperrun,			// 31	시전중인 경공 취소

	//Author : 양희왕 //breif : 점혈술 추가
	// special buff
	en_eff_ctrl_rs_add_special = 40,		// 40      스페셜 버프의 추가
	en_eff_ctrl_rs_update_special,			// 41      스페셜 버프의 갱신(add 와 같음)
	en_eff_ctrl_rs_del_special,				// 42      스페셜 버프의 삭제
} ;

/*
en_eff_ctrl_rs_cancel_current = 30,	// 30	시전중인 스킬(스펠) 취소
en_eff_ctrl_rs_cancel_hyperrun		// 31	시전중인 경공 취소
*/
struct MSG_EFFECT_CTRL_RESULT		// 상태효과를 추가/삭제/업데이트하기 위해 쓰임.
{
	_sHeader			header ;
	u_char				ucMessgae ;
	
	char				cMode ;				// 
} ;

/*
아래와 같은 Mode 에서 유효.
en_eff_ctrl_rs_add_skill=0,			// 0	스킬 추가. 
en_eff_ctrl_rs_update_skill,		// 1	스킬 시간, 단계 : 위와 이건, 전투중 서버에서 보냄.
en_eff_ctrl_rs_del_skill,			// 2	효과 삭제 : Client 의 요청에 의해, 또는 서버에서 보냄.
en_eff_ctrl_rs_del_skill_rq_fail,	// 3	시간이 안맞음 : 지우지 말고, time으로 업데이트
en_eff_ctrl_rs_del_all_buff,		// 4	모든 버프 효과 해제.
en_eff_ctrl_rs_del_all_hostile,		// 5	모든 하슬 표과 해제.
en_eff_ctrl_rs_skill_reserve3,		// 6
en_eff_ctrl_rs_skill_reserve4,		// 7
*/
struct MSG_EFFECT_CTRL_RESULT_MY_SPELL : public MSG_EFFECT_CTRL_RESULT
{
	short				id;				// 스펠 아이디
	unsigned short		lefttimesec;	// 초 단위 남은 시간
	char				cStep;
};

/*
아래와 같은 Mode 에서 유효.
// 자기 효과
en_eff_ctrl_rs_add_effect,			// 8	상태 추가 ( 상태의 아이디 )
en_eff_ctrl_rs_update_effect,		// 9	상태 업데이트
en_eff_ctrl_rs_del_effect,			// 10	상태 삭제.
en_eff_ctrl_rs_del_effect_rq_fail,	// 11	시간 안맞음. : 지우지 말고, time으로 업데이트
en_eff_ctrl_rs_del_all_effect,		// 12	걸려 있는 모든 효과 해제. 상태 초기화.
en_eff_ctrl_rs_condition_reserve2,	// 13
en_eff_ctrl_rs_condition_reserve3,	// 14
en_eff_ctrl_rs_condition_reserve4,	// 15
*/
struct MSG_EFFECT_CTRL_RESULT_MY_EFFECT : public MSG_EFFECT_CTRL_RESULT
{
	short				id;
	unsigned short		lefttimesec;
	short				value;
};

/*
Mode가 아래와 같을 때 유효.
en_eff_ctrl_rs_other_add_skill,		// 16	스킬 추가
en_eff_ctrl_rs_other_del_skill,		// 17	스킬 삭제
en_eff_ctrl_rs_other_del_all_skill,	// 18	모든 스킬 삭제
en_eff_ctrl_rs_other_skill_res,		// 19	예약.
en_eff_ctrl_rs_other_add_effect,	// 20	효과 추가
en_eff_ctrl_rs_other_del_effect,	// 21	효과 삭제
em_eff_ctrl_rs_other_del_all_effect,// 22	모든 효과 삭제
en_eff_ctrl_rs_other_effect_res		// 23
*/
struct MSG_EFFECT_CTRL_RESULT_OTHER : public MSG_EFFECT_CTRL_RESULT
{
	unsigned short		uid;
	short				id;
	char				step;
};

/*
Mode가 아래와 같을 때 유효.
en_eff_ctrl_rs_add_extra,			// 24	엑스트라 효과의 추가.(음식과 투척보조류, 퀘스트 )
en_eff_ctrl_rs_update_extra,		// 25	엑스트라 효과의 수정.	
en_eff_ctrl_rs_del_extra			// 26	엑스트라 효과의 삭제.
*/
struct MSG_EFFECT_CTRL_RESULT_EXTRA : public MSG_EFFECT_CTRL_RESULT
{
	u_char				slot ;			// 0 - 9 슬롯 번호.
	
	short				id ;
	short				value ;				// 
	u_short				usLeftTimeSec ;		// 남은 시간(초)
} ;

/* 
Mode가 아래와 같을 때 유효	
en_eff_ctrl_rs_add_special			// 40	스페셜 버프 추가
en_eff_ctrl_rs_update_special,		// 41	스페셜 버프의 갱신(add 와 같음)
en_eff_ctrl_rs_del_special,			// 42	스페셜 버프의 삭제
*/
struct MSG_EFFECT_CTRL_RESULT_ADD_SPECIAL : public MSG_EFFECT_CTRL_RESULT
{
	u_char				slot;			// 0-3

	short				id;
	u_char				level;

	//Author : 양희왕 //breif : 점혈술 버프 
	u_short				usLeftSecond;    // 남은 시간( 초 )
};

//Author : 양희왕 //breif : 점혈술 버프 
struct MSG_EFFECT_CTRL_RESULT_UPDATE_SPECIAL : public MSG_EFFECT_CTRL_RESULT
{
	u_char	slot;
	
	short	id;	// 버프 아이디
	u_char	level;	// 버프 레벨
	
	u_short	usLeftSecond;	// 남은 시간( 초 )
};

struct MSG_EFFECT_CTRL_RESULT_DEL_SPECIAL : public MSG_EFFECT_CTRL_RESULT
{	   
	u_char				slot;	// 0-3
};


// NPC Buff
// #define MSG_NO_NPC_BUFF_REQ 48 
// NPC 버프 요청 패킷. 
struct MSG_NPC_BUFF_REQ
{
	_sHeader				header;
	u_char				ucMessage;

	u_char				ucReqIndex;			// 리소스파일의 버프 리스트에서 몇번째 인가
	u_int				uiNeedMoney;
};

// #define MSG_NO_NPC_BUFF_RESULT 48 
// NPC 버프 결과 패킷.
struct MSG_NPC_BUFF_RESULT
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucResult;			// 0:성공 1:돈없음 2:권한없음(프리미엄 피씨방/정액 아님)
	u_int				uiMoney;
};



// ===========================================================
// PK 전적
#define _PK_RECORD_WIN_COUNT_				10
#define _PK_RECORD_LOSE_COUNT_				10
#define _PK_RECORD_ABUSING_CHECK_			20

enum
{
	en_pk_option_none = 0,					// 0 : no meaning
	en_pk_option_win_normal,				// 1 : win - normal
	en_pk_option_win_mask,					// 2 : win - mask
	en_pk_option_lose_normal,				// 3 : lose - normal
	en_pk_option_lose_mask					// 4 : lose_mask
};

struct __cntPkRecordEntry					// 전적 요소 : 상대방의 이름과 날짜, 문파
{
	union 
	{
		struct 
		{
			u_int			uiOption : 3;	// 0:None 1:win-normal 2:win-mask 3:lose-normal 4:lose-mask 5:Res,..

			u_int			uiYear : 5;		// 년도 2006+year
			u_int			uiMonth : 6;	// 1 - 12
			u_int			uiDay : 6;		// 1 - 31
			u_int			uiHour : 6;
			u_int			uiMinute : 6;	// 0 - 59
		};

		u_int				uiValue;
	};

	char				name[13];			// 복면인은 NULL
	char				cMoonpa;			// 문파 정보
};

struct __cntPkCount							// 한 문파, 혹은 한 직책에 대한 승/패 수
{
	int					iWin;
	int					iLose;
};

struct MSG_PK_RECORD
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucMode;				// en_pk_record 값
};

enum EN_PK_RECORD
{
	en_pk_record_win_lose_record = 0,		// 0 승패 전적
	en_pk_record_moonpa_record,				// 1 문파 전적
	en_pk_record_class_record,				// 2 직책 전적
	en_pk_record_update_win,				// 3 승 전적 갱신
	en_pk_record_update_lose,				// 4 패 전적 갱신
	en_pk_record_update_moonpa_win = 5,		// 5 문파 승 전적 갱신
	en_pk_record_update_moonpa_lose,		// 6 문파 패 전적 갱신
	en_pk_record_update_class_win,			// 7 직책 승 전적 갱신
	en_pk_record_update_class_lose,			// 8 직책 패 전적 갱신
	en_pk_record_abuse_to_win,				// 9 Abuse to winner
	en_pk_record_abuse_to_lose,				// 10 Abuse to loser
	en_pk_record_lack_honor_to_win,			// 11 명성과 악명이 모두 9이하 to winner
	en_pk_record_lack_honor_to_lose			// 12 명성과 악명이 모두 9이하 to loser
};

// en_pk_record_win_lose_record=0,                      // 0 승패 전적.
struct MSG_PK_WIN_LOSE_RECORD : public MSG_PK_RECORD
{
	// Count에 따라 pkRecord를 적절히 잘라서 복사

	int					iKillCount;			// 전체 전적 승
	int					iDeadCount;			// 전체 전적 패

	u_char				ucWinCount : 4;		// win 슬롯의 개수
	u_char				ucLoseCount : 4;	// lose 슬롯의 개수

	__cntPkRecordEntry	pkRecord[20];		// 최대 win 10 + lose 10
};

// en_pk_record_moonpa_record,				// 1 문파 전적
struct MSG_PK_MOONPA_RECORD : public MSG_PK_RECORD
{
	__cntPkCount		MoonpaRecord[9];	// 최대 9개 문파, 인텍스는 기존과 같음, 끝에 2개 예약
};

// en_pk_record_class_record,				// 2 직책 전적
struct MSG_PK_CLASS_RECORD : public MSG_PK_RECORD
{
	// 최대 8개 - 앞에 4개만 쓰임
	// 0:전투형  1:완전형  2:지략형  3:기공형
	__cntPkCount		ClassRecord[8];
};

// en_pk_record_update_win,					// 3 승 전적 갱신
struct MSG_PK_UPDATE_WIN : public MSG_PK_RECORD
{
	int					iKillCount;

	u_char				ucSlotNo;
	__cntPkRecordEntry	pkRecordEntry;		// 슬롯에 해당하는 정보를 그대로 덮어씀.
	
};

// en_pk_record_update_lose,				// 4 패 전적 갱신
struct MSG_PK_UPDATE_LOSE : public MSG_PK_RECORD
{
	int					iDeadCount;

	u_char				ucSlotNo;
	__cntPkRecordEntry	pkRecordEntry;
};

// en_pk_record_update_moonpa_win = 5,		// 5 문파 승 전적 갱신
struct MSG_PK_UPDATE_MOONPA_WIN : public MSG_PK_RECORD
{
	char				cMoonpa;
	int					iValue;
};

// en_pk_record_update_moonpa_lose,		// 6 문파 패 전적 갱신
struct MSG_PK_UPDATE_MOONPA_LOSE : public MSG_PK_RECORD
{
	char				cMoonpa;
	int					iValue;
};

// en_pk_record_update_class_win,			// 7 직책 승 전적 갱신
struct MSG_PK_UPDATE_CLASS_WIN : public MSG_PK_RECORD 
{
	char				cClass;
	int					iValue;
};

// en_pk_record_update_class_lose			// 8 직책 패 전적 갱신
struct MSG_PK_UPDATE_CLASS_LOSE : public MSG_PK_RECORD
{
	char				cClass;
	int					iValue;
};