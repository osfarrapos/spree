#pragma once

#define _PD_RETURN_LOBBY						// 20080731 : 다른 캐릭터 선택하기. 20080903 15시 이후에는 DS 와 함께 전달할 것.

#define _PD_LVPK_REGION_ATTACK_MOD_				// 20080720 : 영역기의 대상만 대전이 걸림.
#define _PD_CASTLE_MOB_ATTACKED_ONLY_ATTACKER_	// 20080720 : 장원전에서, 영영기 사용시, 수성측 공격은 몬스터에 적용되지 않도록 한다. ( _PD_CASTLE_STEP_1_ 와  같이 디파인 할 것 )
//#define _PD_CHARAC_LTS_REGION_ATTACK_EXC_		// 20080716 : 영역기 사용시, LVPK 리스트에 캐릭터가있을 경우, 공격이 들어가지 않도록 한다. _PD_CHARAC_LTS_ 
//#define _PD_LEVEL_RANK_OVERLAP_					// 20080710 : 레벨/경험치가 같을 경우, 순위가 중복될 수 있도록 수정.

#define _PD_COMMON_RANK_						// 20080702 : 공통 랭크 추가.( CommonRank.txt )


#define _PD_BUGFIX_20080630_					// 20080630 : 7/3일 패치에 수정될 버그. 상태효과 값의 동기화, 무공 시전시간 체크.
#define _PD_BUGFIX_20080616_					// 20080610 : 6/26 일 패치시 수정될 버그. : 다른 나라는 영역기 들어갈 때, 같이 들어갈 것.

#define _PD_SKILL_EXTEND_						// 20080529 : 스킬 슬롯의 개수 확장(80개). DB/DS/GS ExtendSkillSlot.doc 참고.
//#define _PD_SKILL_EXTEND_100_					// 20080529 : 스킬 슬롯의 개수를 100 개까지 확장. GS 만 업데이트

#define _PD_QUEST_FOR_DUNGEON_					// 20080529


#define _PD_ITEM_BONUS_SPELL_APPLY_				// 20080507 : 아이템/캐릭터의 보너스 Apply 가, 기공에서 적용되지 않던 것 적용. : 국내 적용후, 없앤다.
#define _PD_MOB_M_DEFENCE_FIX_					// 20080507 : 몬스터 물리/기공 방어 계산 오류 수정 + 캐릭터 기공 방어 계산 오류. : 국내 적용후, 없앤다.

#define _PD_CASH_INVEN_							// 20080529 : 캐쉬 인벤 및 표국 확장 : DS/GS/GMS/Client : CashInventory.doc

#define _PD_CLOTH_VIEW_SELECT_					// 20080422 : 의상 보기를 치장의상과 일반의상 중에 선택할 수 있도록 한다. : client

#define _PD_ADD_EVENT_COLLECTION_				// 20080403 : 수집 데이타 부족으로, 이벤트성 수집은 1000 번 부터 사용하도록, 확장.

#define _PD_PKEVENT_BOX_SPAWN_					// 20080422 : 구궁패왕화로의 생성을 진위장군 스폰 시점으로 옮긴다. : DB/DS/GS & Client

#define _PD_SECOND_JOB_							// 20080324 : 2차 전직 - 상태효과와 클래스 적용.
#define _PD_ITEM_V25_							// 20080324 : 아이템 테이블 2.5 버전. 의상과 무공서에 CharacGrade 추가.
#define _MASTERY_SECOND_PATCH_					// 20080324 : 2차 오의 상태효과 적용.

#define _PD_SPELL_WEAPON_PENALTY_				// 20080324 : 강무기 패널치 조건 -- 지혜->민첩 으로.

#define _PD_SETITEM_							// 20080227 : 세트 아이템 적용.
#define _PD_MOB_APPLYEFFECT_MOD_				// 20080227 : 몬스테에게 적용되는 상태효과 중, 특정 상태효과는 다른 것으로 바뀐다.

#define _PD_COLLECT_PACKET_MODIFY_				// 20080324 : 수집 패킷의 필요 정보 부족으로 버그 발생 -> 필요 정보 추가

#define _PD_MOB_PARAM_INCREASE_					// 20080122 : 몬스터 전투 파라미터들의 타입이 short => int 로 확장. 
#define _PD_LEVEL_RATE_SAVE_					// 레벨의 비율(0-1 까지)을 저장하여, 변경 플래그가 켜지면, 비유을 조정하게 한다. DS/GS

#define _PD_EXPBONUS_SAVE_
//#define _PD_QUEST_MAX_600_					// 20071206 퀘스트 개수 확장 : 300 => 600 개로 확장.

#define _PD_CASTLE_RENEWAL_EXP_					// 20071226 장원전 리뉴얼 - 보너스 경험치 주기.
#define _PD_CASTLE_RENEWAL_1_					// 20071226 장원전 리뉴얼 - 스케줄링, 버프 추가.

#define _PD_GAME_EVENT_							// 20071226 Game Event(DB/DS/GS) : Free Match 

#define _PD_ITEM_PM_COMMON_1024					// 20071126 : 아이테의 공성/방어력/공회피율 물리/기공에 모두 적용하고, 버그 수정.

#define _PD_MONSTER_REF_1_5_					// 20071126 : monster ref 1.5
#define _PD_ITEM_RENEWAL_1008_					// 20071126 : 무기 암상인 리뉴얼 : 무기 조합


#define _PD_GAMBLE_FIX_0821_					// 겜블 변경.

#define _PD_SEPARATE_SOCKET_					// 20070710 소켓 분리 기능 추가.
#define _PD_ITEM_V24_SOCKET_					// 20071101 아이템 v2.4 적용과 소켓 삽입 조건 검사 추가. _PD_SEPARATE_SOCKET_ 같이 포함할 것.

#define _PD_4_EFFECT_ADD_						// 음양혼신 공격/저항력 추가. : 공격력/반사/흡수/저항력 : 스크립트(penalty_of_zone.txt)

#define _PD_RECALL_FRIEND_						// 친구소환 20070604 시작.

#define _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_		// 20070601 로그 서버 패킷 변경.

#define _PD_WEAPON_PROTECT_						// 용린/용루 시스템. 시작 : 20070420 : 20070510 정식.

#define _PD_CASTLE_STEP_2_						// 장원전 : 2단계 전적/세금/1차버그수정 : 20070403

#define _PD_PARTY_STATE_ADD_					// 20070321 동행의 상태 강화 자리비움 : DS : 국내 20070329 : CL 패킷.
#define _PD_CHARAC_LTS_							// LTS IJA. 국내 20070329 : DS/CS 패킷.


#define	_MASTERY_NEED_PAGE_POINT_				// 필요 포인트 변경부분
#define _PD_SKILL_ADD_FROM_MASTERY_				// 오의에 의한 스킬 추가.
#define _PD_PVP_EFFECT_MODIFY_					// 효과에 따라, PVP 감소 효과가 다르게 적용.

#define _PD_CASTLE_STEP_1_						// 국내 : 20070223 up.

#define _PD_MONSTER_SKILL_DAM_					// 몬스터의 스킬 공격력 및 공성치 적용. 2006/08/18 Ray.

// 11/07 Update to Nexon. 
#define _PD_EXP_NOTICE_


//////////////////////////////////////////////////////////////////////////
// 해킹 방지
//////////////////////////////////////////////////////////////////////////
#define _PD_CRYPTO_GETKEY_MOD_					// 20080721
#define _PD_RECV_HACKPROGRAM_LIST_				// 20080721
#define _PD_CHECK_VERSION_BY_CHANNEL_			// 20080721
#define _PD_PACKET_CRYPTO_CHECK_				// 20080721 : 패킷들의 암호화를 체크한다.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

#define ITEMSHOP_PATCH_200607					// ItemShop : 정식에 2006/08/17 올림. by Ray.
#define	_NXPCB_AWARD_200703_					// 넥슨 피씨방 혜택
#define	_NXPCB_TEMPORALY_BLOCK_AWARD_	


//////////////////////////////////////////////////////////////////////////
// Game Guard
//////////////////////////////////////////////////////////////////////////
#define _PD_USE_GAMEGUARD_						// 20080311 : 게임 가드 인증 사용.
//#define _PD_GAMEGUARD_VER_1_					// 20080311 : 게임가드 인증 버전 1 사용. : 2008/02/28 이전. 컴파일 전, lib 교체. 배포시. Dll 포함.
#define _PD_GAMEGUARD_VER_2_					// 20080311 : 게임가드 인증 버전 2 사용. : 2008/02/28 이후. 컴파일 전, lib 교체. 배포시. Dll 포함.

//////////////////////////////////////////////////////////////////////////
// Event 
//////////////////////////////////////////////////////////////////////////
//#define _PD_LEVEL_PRESENT_						// 20080324 : 레벨 달성 선물 주기.
#define _PD_EVENT_INVITE_FRIEND_				// 20080406 : 친구 초대 이벤트(미국)