// XCharacterState.h: interface for the _XCharacterState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCHARACTERSTATE_H__BBF56854_4252_425E_BFEE_914C7293E217__INCLUDED_)
#define AFX_XCHARACTERSTATE_H__BBF56854_4252_425E_BFEE_914C7293E217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#pragma warning (disable : 4786)
#include <map>
#include <list>
using namespace std;

// 효과 특성 Effect Property : 15개 
#define _XCL_EFFECT_PROP_NONE				(0)
#define _XCL_EFFECT_PROP_GENERAL			(0x01)		// 일정 시간 동안 유지 : 공성치
#define _XCL_EFFECT_PROP_INTERVAL			(0x01<<1)	// 일정 시간 동안 반복 : 중독
#define _XCL_EFFECT_PROP_INITIAL_ONCE		(0x01<<2)	// 시전 시점에 한번 적용되고 끝
#define _XCL_EFFECT_PROP_ADDITIVE			(0x01<<3)	// 공격시 상대방에게 추가로 감
#define _XCL_EFFECT_PROP_REFLECTION			(0x01<<4)	// 방어시 반사 가능
#define _XCL_EFFECT_PROP_ONLY_ONCE			(0x01<<5)	// 한번 성공하면 사라짐
#define _XCL_EFFECT_PROP_FEED				(0x01<<6)	// 공격시 데미지로 부터 돌려받음
// 나머지는 예약

// Feed 특성일 때 어디서 얻어올 것인지
enum EN_EFFECT_FROM_TARGET
{
	en_eff_from_none = 0,					// 기본 속성
	en_eff_from_hit,						// 준 데미지에서
	en_eff_from_life,						// 상대방의 체력
	en_eff_from_force,						// 상대방의 내력
	en_eff_from_concentration				// 상대방의 의지
};

// Effect Property
enum EN_EFFECT_VALUE_TYPE
{
	en_eff_vt_add_value = 0,				// value
	en_eff_vt_add_percent,					// percent
	en_eff_vt_sub_value,					// value
	en_eff_vt_sub_percent					// percent
};

// 어디에 적용할지 대상 타입의 번호.
enum EN_EFFECT_WHERE_APPLY
{
	en_eff_where_none=0,					// 0 부터 시작해서 최대 255...
	en_eff_where_attack_rate,				// 공격 성공치 : 'attack_rate'
	en_eff_where_avoid_rate,				// 회피 성공치 : 'avoid_rate'
	en_eff_where_attack_damage,				// 3 공격력
	en_eff_where_defence, 					// 4 방어력
	en_eff_where_defence_rate,				// 5 방어 확률
	en_eff_where_m_attack_rate,				// 6 기공 공격 성공치
	en_eff_where_m_avoid_rate,				// 7 기공 회피 성공치
	en_eff_where_m_attack_damage,			// 8 기공 공격력
	en_eff_where_m_defence,					// 9 기공 방어력
	en_eff_where_m_defence_rate,				// 10 기공 방어 확률
	
	en_eff_where_life,						// 11 생명력
	en_eff_where_force,						// 12 내력
	en_eff_where_concentration,				// 13 의지
	en_eff_where_max_life,					// 14 최대 생명력(체력)
	en_eff_where_max_force,					// 15 최대 내력
	en_eff_where_max_concentration,			// 16 최대 의지 
	en_eff_where_strength,					// 17		// 근력 보너스
	en_eff_where_zen,						// 18		// 진기
	en_eff_where_intelligence,				// 19		// 지혜
	en_eff_where_constitution,				// 20		// 건강
	
	en_eff_where_dexterity,					// 21		// 민첩
	en_eff_where_critical_rate,				// 22 치명타 확률 증가.
	en_eff_where_cri_avoid_rate,			// 23 치명타 회피 확률 증가
	en_eff_where_ref_p_damage,				// 24 물리 데미지 반사
	en_eff_where_ref_m_damage,				// 25 기공 공격력 반사
	en_eff_where_ref_hon_damage,			// 26 혼의 데미지 반사
	en_eff_where_ref_sin_damage,			// 27 신의 데미지 반사
	
	en_eff_where_add_minus_damage,			// 28 음의 데미지 추가
	en_eff_where_add_plus_damage,			// 29 양의 데미지 추가
	en_eff_where_add_hon_damage,			// 30 혼의 데미지 추가
	en_eff_where_add_sin_damage,			// 31 신의 데미지 추가
	
	en_eff_where_wound_out,					// 32 외상
	en_eff_where_wound_in,					// 33 내상
	
	en_eff_where_regen_life,				// 34 자동 증가 : 생명
	en_eff_where_regen_force,				// 35 자동 증가 : 내공
	en_eff_where_regen_concentration,		// 36 자동 증가 : 영력
	
	en_eff_where_resist_minus,				// 37 음의 저항력
	en_eff_where_resist_plus,				// 38 양의 저항력
	en_eff_where_resist_hon,				// 39 혼의 저항력
	en_eff_where_resist_sin,				// 40 신의 저항력
	
	en_eff_where_dam_to_force,				// 41 내공 방어
	en_eff_where_dam_to_concentration,		// 42 의지 방어
	
	en_eff_where_give_life,					// 43 자신의 생명 전달
	en_eff_where_give_force,				// 44 자신의 내공 전달
	en_eff_where_trans_force_to_life,		// 45 시전자의 내공을 생명으로 전환
	en_eff_where_trans_life_to_force,		// 46 시전자의 생명을 내공으로 전환

	// 2005.01.07 추가

	// 2005.07.18 이름 변경
	en_eff_where_incre_sk_mabi_prob,		// 47 마비 확률 증가

	en_eff_where_add_condition,				// 48 상태 추가
	en_eff_where_remove_condition,			// 49 상태 해제
	
	// 2005.01.18 추가
	en_eff_where_remove_spell,				// 50 스펠 해제
	en_eff_where_move_speed,				// 51 이동 속도 % 증가 / 감소
	en_eff_where_attack_speed,				// 52 공격 속도 % 증가 / 감소

	// 2005.03.18 추가
	en_eff_where_absolute_avoid,			// 53 극-회피 n회 회피 v apply_additive
	en_eff_where_absolute_reflect,			// 54 극-반사 n회 반사 v apply_additive
	en_eff_where_absolute_attack,			// 55 극-공격	v apply_additive
	en_eff_where_absolute_critical,			// 56 극-치명	v apply_additive
	en_eff_where_feed_life, 				// 57 상대방의 생명을 빼앗는다.	v initial_once
	en_eff_where_feed_force,		 		// 58 상대방의 내공을 빼앗는다.	v initial_once
	en_eff_where_feed_concentration,		// 59 상대방의 의지를 빼앗는다.	v initial_once
	en_eff_where_ref_minus_damage,			// 60 음의 데미지 반사	v reflect
	en_eff_where_ref_plus_damage,			// 61 양의 데미지 반사	v reflect
	
	// 2005.04.27 추가
	en_eff_where_add_minus_spell_dam,		// 62 음의 기공에 추가 데미지를 준다. general, apply_additive
	en_eff_where_add_plus_spell_dam,		// 63 양의 기공에 추가 데미지를 준다. general, apply_additive
	en_eff_where_add_hon_spell_dam,			// 64 혼의 기공에 추가 데미지를 준다. general, apply_additive
	en_eff_where_add_sin_spell_dam,			// 65 신의 기공에 추가 데미지를 준다. general, apply_additive
	en_eff_where_shield_skill_atk,			// 66 흡수 - 무공. 데미지 n 만큼 흡수. apply_additive
	en_eff_where_shield_spell_atk,			// 67 흡수 - 기공. 데미지 n 만큼 흡수. apply_additive
	en_eff_where_shield_minus_atk,			// 68 흡수 - 음. 데미지 n 만큼 흡수. apply_additive
	en_eff_where_shield_plus_atk,			// 69 흡수 - 양. 데미지 n 만큼 흡수. apply_additive
	en_eff_where_shield_hon_atk,			// 70 흡수 - 혼. 데미지 n 만큼 흡수. apply_additive
	en_eff_where_shield_sin_atk,			// 71 흡수 - 신. 데미지 n 만큼 흡수. apply_additive
	en_eff_where_shield_all_atk,			// 72 흠수 - 완. 데미지 n 만큼 훕수. apply_additive
	en_eff_where_all_stat,					// 73 중요 증가/감소
	
	// 2005.05.30 추가
	en_eff_where_weapon_dam,				// 74 무기 데미지 증가/감소

	// 2005.06.15 추가
	en_eff_where_rebirth_target,			// 75 부활 시킨다
	
	// 2005.07.12 추가
	en_eff_where_weapon_max_dam,			// 76 무기 최대 데미지 증가
	en_eff_where_weapon_min_dam,			// 77 무기 최소 데미지 증가
	en_eff_where_item_drop_percent,			// 78 아이템 드롭 확률 증가
	en_eff_where_balance,					// 79 균형 증가/감소
	en_eff_where_cri_dam_bonus,				// 80 치명타 공격력 % 증가
	en_eff_where_sk_target_modify,			// 81 무공의 타겟 타입 변경
	en_eff_where_sk_effectapply_modify,		// 82 무공의 적용 타입 변경
	en_eff_where_sk_angle_modify,			// 83 무공의 영역 속성 변경
	en_eff_where_sk_fera_modify,			// 84 무공의 무공 속성 변경
	en_eff_where_sk_ferp_modify,			// 85 무공의 기공 속성 변경
	en_eff_where_sk_distance,				// 86 무공의 distance 증가/감소
	en_eff_where_sk_radius,					// 87 무공의 radius 증가/감소
	en_eff_where_sk_min_dam,				// 88 무공의 최소 데미지 증가/감소
	en_eff_where_sk_max_dam,				// 89 무공의 최대 데미지 증가/감소
	en_eff_where_sk_attackrate,				// 90 무공의 공성치 증가/감소
	en_eff_where_sk_avoidrate,				// 91 무공의 회피 증가/감소
	en_eff_where_sk_costinner,				// 92 무공의 소모 내력 감소
	en_eff_where_sk_costmind,				// 93 무공의 소모 영력 감소
	en_eff_where_sk_finishspeed,			// 94 피니쉬 스피드 감소
	en_eff_where_sk_castingtime,			// 95 캐스팅 감소
	en_eff_where_sk_delay,					// 96 무공 딜레이 감소
	en_eff_where_sk_cooldown,				// 97 무공 쿨다운 감소

	// 2005.07.18 이름 변경
	en_eff_where_sk_incre_effecttime,		// 98 무공의 적용시간 증가

	// 2005.07.18 추가
	en_eff_where_sk_additive_1_time,		// 99 무공의 상태효과 1 적용 시간 증가
	en_eff_where_sk_additive_1_value,		// 100 무공의 상태효과 1 값 증가
	en_eff_where_sk_additive_1_prob,		// 101 무공의 상태효과 1 확률 증가
	en_eff_where_sk_additive_2_time,		// 102 무공의 상태효과 2 적용 시간 증가
	en_eff_where_sk_additive_2_value,		// 103 무공의 상태효과 2 값 증가
	en_eff_where_sk_additive_2_prob,		// 104 무공의 상태효과 2 확률 증가
	en_eff_where_sk_additive_3_time,		// 105 무공의 상태효과 3 적용 시간 증가
	en_eff_where_sk_additive_3_value,		// 106 무공의 상태효과 3 값 증가
	en_eff_where_sk_additive_3_prob,		// 107 무공의 상태효과 3 확률 증가
	en_eff_where_sk_additive_4_time,		// 108 무공의 상태효과 4 적용 시간 증가
	en_eff_where_sk_additive_4_value,		// 109 무공의 상태효과 4 값 증가
	en_eff_where_sk_additive_4_prob,		// 110 무공의 상태효과 4 확률 증가
	en_eff_where_incre_sk_mabi_time,		// 111 무공 마비 시간 증가
	en_eff_where_incre_sp_mabi_prob,		// 112 기공 마비 확률
	en_eff_where_incre_sp_mabi_time,		// 113 기공 마비 시간 
	
	// 2006.01.17 추가
	en_eff_where_m_balance,					// 114  기공 밸런스
	en_eff_where_m_critical_rate,			// 115  기공 치명타 성공률
	en_eff_where_m_cri_avoid_rate,			// 116	기공 치명타 회피율
	en_eff_where_p_critical_rate,			// 117	물리 치명타 성공률
	en_eff_where_p_cri_avoid_rate,			// 118	물리 치명타 회피율
	en_eff_where_monster_taunt,				// 119	몬스터 타운트
	en_eff_where_monster_detaunt,			// 120  몬스터 디타운트
	en_eff_where_monster_hate,				// 121 	몬스터 해이트
	en_eff_where_p_balance,					// 122	물리 밸런스
	en_eff_where_max_life_and_force,		// 123	최대 생명과 내공 상승
	en_eff_where_disable_skill,				// 124 	특정 무공 사용 불가
	en_eff_where_disable_s_type,			// 125 	특정 s type 무공 사용 불가
	en_eff_where_cancel_current,			// 126 	현재 시전중인 무공을 취소 시킨다
	en_eff_where_cancel_hyperrun,			// 127 	시전중인 경공을 취소 시킨다
	en_eff_where_destroy_force,				// 128 	내공 파괴
	en_eff_where_resist_prob_for_mabi,		// 129 	마비 저항 확률
	
	en_eff_where_delete_all_buff,			// 130	버프 삭제 : 2006/02/01 : initial
	en_eff_where_resistance_poison_pk,		// 131	패왕동 독무 내성 : 2006/02/27 : only apply & food - 5 
	en_eff_where_no_effect,					// 132 
	en_eff_where_fury,						// 133	분노 수치 증감. initial (해당 효과 발동 시 추가 n포인트 획득), general (분노 수치 획득 상황 발생 시 n% 추가 획득)
	en_eff_where_remove_buff,				// 134	debuff n개
	en_eff_where_remove_hostile,			// 135	dehostile n개
	en_eff_where_sk_feed_life,				// 136	무공시 흡수 : feed
	en_eff_where_sk_feed_force,				// 137	무공시 흡수 : feed
	en_eff_where_sk_feed_concentration,		// 138	무공시 흡수 : feed
	en_eff_where_sp_feed_life,				// 139	기공시 흡수 : feed
	en_eff_where_sp_feed_force,				// 140	기공시 흡수 : feed
	en_eff_where_sp_feed_concentration,		// 141	기공시 흡수 : feed

	en_eff_where_sr_safe,					// 142	안전 지역 상태효과.
	en_eff_where_light_crime,				// 143	경범죄 상태효과.
	en_eff_where_sr_heavy_crime,			// 144	중범죄 상태효과.
	
	// 2007.07.19 추가
	en_eff_where_resist_all,				// 145	 모든 저항력 증가
	
	// 2008.03.03 추가
	en_eff_resistance_battleroom_poison,	// 146	결 독무 저항
	en_reduce_rb_time,						// 147	부활 휴우증 시간 감소 : general/additive
	en_reduce_rb_exp,						// 148	부활 휴우증 경험치하락% 감소 : general/additive
	en_protect_rate,						// 149	제련 성공 확률이 % 단위로 증가 : general/additive
	en_dam_inc_for_human_type,				// 150	인간형 몬스터에게 공격력이 %단위로 증가 : general/additive
	en_dam_inc_for_animal_type,				// 151  동물형 몬스터에게 공격력이 %단위로 증가 : general/additive
	en_dam_inc_at_day,						// 152	낮 시간에 공격력이 %단위로 증가 : general/additive
	en_exp_inc_at_day,						// 153	낮 시간에 경험치가 %단위로 증가 : general/additive
	en_dam_inc_at_night,					// 154	밤 시간에 공격력이 %단위로 증가 : general/additive
	en_exp_inc_at_night,					// 155	밤 시간에 경험치가 %단위로 증가 : general/additive

} ;

// =====================================
// character state(effect) table =======
struct _XCharacterStateTable
{
	short			id;

#ifdef _XDEF_NEWSTRINGMANAGER	
	LPTSTR			name;
	LPTSTR			concept;
#else
	TCHAR			name[64];
	TCHAR			concept[128];
#endif

	char			type;					// 0 : buff, 1 : hostile
	unsigned char	iconNumber;
	unsigned char	effectNumber;	
	
	// from server script
	unsigned short	effect_prop;			// 속성 - property
	char			value_type;				// 적용 값을 계산하는 방법
	unsigned char	where;					// 적용 될 곳
	unsigned char	from;					// 상대방에게 적용할 곳
	unsigned char	condition_flag_id;
	char			count;
	
	short			prob;
	short			value;
	short			additiveCount;
};
extern map <int, _XCharacterStateTable*>	g_CharacterStateTable;

// =====================================
// character condition table ===========
struct _XCharacterCondition
{
	short			id;
	
#ifdef _XDEF_NEWSTRINGMANAGER
	LPTSTR			name;
#else
	TCHAR			name[16];
#endif

	int				visualEffect[5];

	unsigned short	movelock : 1;
	unsigned short	tradelock : 1;
	unsigned short	battlelock : 1;
	unsigned short	medlock : 1;
	unsigned short	spelllock : 1;
	unsigned short	speedlock : 1;
	unsigned short	modelock : 1;
	unsigned short	skilllock : 1;
	unsigned short	chattinglock : 1;
	unsigned short	partylock : 1;
	unsigned short	reserved : 6;

	short			soundprop[5];
	short			soundindex[5];

#ifdef _XDEF_NEWSTRINGMANAGER
	LPTSTR			concept;
#else
	TCHAR			concept[128];
#endif
};
extern _XCharacterCondition g_CharacterConditionTable[32];

// =====================================
// character state	====================
class _XCharacterState  
{
public :

	char				m_FromType;			// 0:skill, 1:condition, 2:item

	short				m_sSkillID;
	char				m_cStep;

	char				m_cSlotIndex;

	short				m_sEffectID;
	short				m_sValue;
	
	unsigned short		m_usLeftTimeSec;

	DWORD				m_dwStateStartTime;	
	bool				m_bDeleteReqPacket;
	

public:
	_XCharacterState();
	virtual ~_XCharacterState();

};

enum
{
	en_from_skill = 0,
	en_from_effect,
	en_from_extra,
	en_from_passive,
	en_from_nickname,
	en_from_extraskill,		// 안전 지역 버프
	en_from_specialbuff		// 장원전/은거고수버프 에서 사용
};

// =====================================
// character state list	================
class _XCharacterStateList
{
public :
	map <int, _XCharacterState*>	m_mapCharacterStateList;
	int								m_TotalListCount;

public :
	_XCharacterStateList();
	virtual ~_XCharacterStateList();

	// fromtype -  0:skill  1:effect  2:item
	void					InsertState(unsigned short uid, short id, unsigned short lefttime, char fromtype, short val = 0, short skillid = 0, char step = 0);
	void					DeleteState(unsigned short uid, char fromtype, short effectid, short skillid = 0);
	
	void					InsertStateFromSpell(short skillid, char skillstep, unsigned short lefttime, int effectindex);
	void					UpdateStateFromSpell(short skillid, char skillstep, unsigned short lefttime, int effectindex);
	void					DeleteStateFromSpell(short skillid, char skillstep, int effectindex);

	void					InsertStateFromEffect(unsigned short uid, short effectid, unsigned short lefttime, short value);
	void					UpdateStateFromEffect(unsigned short uid, short effectid, unsigned short lefttime, short value);
	void					DeleteStateFromEffect(unsigned short uid, short effectid);

	void					InsertStateFromExtra(unsigned short uid, short effectid, char slotindex, unsigned short lefttime, short value);
	void					UpdateStateFromExtra(unsigned short uid, short effectid, char slotindex, unsigned short lefttime, short value);
	void					DeleteStateFromExtra(unsigned short uid, short effectid, char slotindex);

	void					InsertStateFromPassive(short skillid, char skillstep, int effectindex);
	void					DeleteStateFromPassive(short skillid, char skillstep, int effectindex);

	// 별호 효과 
	void					InsertStateFromNicknameEffect(short effectid, short value );
	void					UpdateStateFromNicknameEffect(short effectid, short value );
	void					DeleteStateFromNicknameEffect(void);

	// extra slot이면서 skill id를 사용하는 경우 - 안전 지역 관련 버프 등
	void					InsertStateFromExtraSkill(unsigned short uid, short skillid, char skillstep, char slotindex, unsigned short lefttime, int effectindex);
	void					UpdateStateFromExtraSkill(unsigned short uid, short skillid, char skillstep, char slotindex, unsigned short lefttime, int effectindex);
	void					DeleteStateFromExtraSkill(unsigned short uid, short skillid, char skillstep, int effectindex ,char slotindex);

	// 장원전에서 사용하는 버프
	void					InsertStateFromSpecialBuff(unsigned short uid, short skillid, char skillstep, char slotindex, unsigned short lefttime = 0, int effectindex = 0);
	void					UpdateStateFromSpecialBuff(unsigned short uid, short skillid, char skillstep, char slotindex, unsigned short lefttime = 0, int effectindex = 0);
	void					DeleteStateFromSpecialBuff(unsigned short uid, short skillid, char skillstep, int effectindex, char slotindex);

	int						FindState(char fromtype, short effectid, short skillid, char slotindex = 0);
	int						FindStateExtraSkill(char fromtype, char slotindex);
	int						FindStateSpecialBuff(char slotindex, int& skillid, int& skilllevel);

	void					Release(void);

	static BOOL				LoadCharacterStateTable(void);
	static void				ReleaseCharacterStateTable(void);
	static BOOL				ReloadCharacterStateTable(void);

	static BOOL				LoadEffectScriptServerFormat(_XPackageArchive& scriptpackage);
	static BOOL				GetCommand(char* cmd, FILE* fileptr, bool bnumber = false);
	static BOOL				ParseDescriptor(FILE* fileptr);
	static BOOL				ParseEffectScript(FILE* fileptr);
	
	static void				InsertEffectScriptName(char* eff_name, short eff_where);
	static void				ReleaseEffectScriptName(void);
	static unsigned char	GetEffectWhereFromName(char* eff_name);
	static unsigned char	GetEffectProperty(char* property);
	static unsigned char	GetEffectFromID(char* from);
	static char				GetEffectValueType(char* value);

	static BOOL				LoadCharacterConditionTable(_XPackageArchive& scriptpackage);
	
	void		ProcessState(void);
	void		UpdateState(unsigned short uid, short id, unsigned short lefttime, char fromtype, short val = 0, short skillid = 0, char step = 0);

//	void		AddResult(short id, short val);
//	void		DeleteResult(short id);

	void		DeleteAllBuff(void);
	void		DeleteAllHostile(void);

	void		DeleteAllEffect(unsigned short uid);
	void		DeleteAllSkill(unsigned short uid);
	
	void		DeleteAll(void);		// 모든 상태효과 삭제, Result, Effect, icon 모두 삭제

	// Condition
	void		InsertConditionState(unsigned short uid, int index);
	void		DeleteConditionState(unsigned short uid, int index);
	BOOL		FindConditionState(unsigned short uid, int index);
	
	// 상태효과 Renewal
	void		ApplyBonusPoint(void);
	void		ApplyBonusAdditional(void);
	void		ApplyFinalValue(void);

	void		GetCharacterBaseValue(void);

	void		AddStrengthBonus(short id, short val);
	void		AddStrengthBonus(short percentage);

	void		AddZenBonus(short id, short val);
	void		AddZenBonus(short percentage);

	void		AddIntBonus(short id, short val);
	void		AddIntBonus(short percentage);

	void		AddConstitutionBonus(short id, short val);
	void		AddConstitutionBonus(short percentage);

	void		AddDexBonus(short id, short val);
	void		AddDexBonus(short percentage);

	void		AddPAttackRateBonus(short id, short val);
	void		AddPAttackRateBonus(short percentage);

	void		AddPAvoidRateBonus(short id, short val);
	void		AddPAvoidRateBonus(short percentage);

	void		AddPAttackDamageBonus(short id, short val);
	void		AddPAttackDamageBonus(short percentage);

	void		AddDefenceBonus(short id, short val);
	void		AddDefenceBonus(short percentage);

	void		AddMAttackRateBonus(short id, short val);
	void		AddMAttackRateBonus(short percentage);

	void		AddMDefenceBonus(short id, short val);
	void		AddMDefenceBonus(short percentage);

	void		AddMAvoidRateBonus(short id, short val);
	void		AddMAvoidRateBonus(short percentage);

	void		AddMAttackDamageBonus(short id, short val);
	void		AddMAttackDamageBonus(short percentage);

	void		AddMaxLifeBonus(short id, short val);
	void		AddMaxLifeBonus(short percentage);

	void		AddMaxForceBonus(short id, short val);
	void		AddMaxForceBonus(short percentage);

	void		AddMaxConcentrationBonus(short id, short val);
	void		AddMaxConcentrationBonus(short percentage);

	void		AddMoveSpeedBonus(short id, short val);
	void		AddMoveSpeedBonus(short percentage);
	
	void		AddWeaponDamage(short id, short val);
	void		AddWeaponDamage(short percentage);

	void		AddWeaponMaxDamage(short id, short val);
	void		AddWeaponMaxDamage(short percentage);

	void		AddWeaponMinDamage(short id, short val);
	void		AddWeaponMinDamage(short percentage);

	void		AddDisableSkill(short id, short val);
	void		AddDisableSType(short id, short val);
	
	void		AddResistMinus(short id, short val);
	void		AddResistMinus(short percentage);

	void		AddResistPlus(short id, short val);
	void		AddResistPlus(short percentage);

	void		AddResistHon(short id, short val);
	void		AddResistHon(short percentage);

	void		AddResistSin(short id, short val);
	void		AddResistSin(short percentage);

	void		AddResistAll(short id, short val);
	void		AddResistAll(short percentage);

	void		AddAttackDamageMinus(short id, short val);
	void		AddAttackDamageMinus(short percentage);

	void		AddAttackDamagePlus(short id, short val);
	void		AddAttackDamagePlus(short percentage);

	void		AddAttackDamageHon(short id, short val);
	void		AddAttackDamageHon(short percentage);

	void		AddAttackDamageSin(short id, short val);
	void		AddAttackDamageSin(short percentage);

	void		GetBaseBonusFromMastery(void);
	void		GetAdditionalBonusFromMastery(void);
	void		GetSkillModifiedFromMastery(void);

	void		GetAdditionalBonusFromItem(void);
	void		GetBaseBonusFromItem(void);

	// 오의 - 무공 강화
	void		ModifySkillTargetType(short skillid, char point, short id, short value);
	void		ModifySkillDistance(short skillid, char point, short id, short value);
	void		ModifySkillMinDamage(short skillid, char point, short id, short value);
	void		ModifySkillMaxDamage(short skillid, char point, short id, short value);
	void		ModifySkillAttackRate(short skillid, char point, short id, short value);
	void		ModifySkillAvoidRate(short skillid, char point, short id, short value);
	void		ModifySkillCostInner(short skillid, char point, short id, short value);
	void		ModifySkillCostMind(short skillid, char point, short id, short value);
	void		ModifySkillFinishSpeed(short skillid, char point, short id, short value);
	void		ModifySkillCasting(short skillid, char point, short id, short value);
	void		ModifySkillDelay(short skillid, char point, short id, short value);
	void		ModifySkillCooldown(short skillid, char point, short id, short value);
	void		ModifySkillIncreEffectTime(short skillid, char point, short id, short value);
	void		ModifySkillAdditiveValue(short skillid, char point, int index, short id, short value);
	void		ModifySkillAdditiveProb(short skillid, char point, int index, short id, short value);
	void		ModifySkillAdditiveTime(short skillid, char point, int index, short id, short value);

};

#endif // !defined(AFX_XCHARACTERSTATE_H__BBF56854_4252_425E_BFEE_914C7293E217__INCLUDED_)
