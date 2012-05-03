#pragma once

//////////////////////////////////////////////////////////////////////////
//#define _PD_SECOND_JOB_						// 20080320 : 2차 전직 - 상태효과와 클래스 적용.
//#define _PD_ITEM_PM_COMMON_1024				// 20080320 : 아이테의 공성/방어력/공회피율 물리/기공에 모두 적용하고, 버그 수정.
// 위 2 개 : 20080911 오후에 유진씨 요청으로 지움. 다시 되돌릴 것.
//////////////////////////////////////////////////////////////////////////

/*
// 20080905 : 긴급패치로 7월 버전으로 돌림
#define _PD_DEL_NOENEMY_EFFECT_					// 20080905 : 무적 상태효과 제거.
#define _PD_ITEM_V25_							// 20080320 : 아이템 테이블 2.5 버전. 의상과 무공서에 CharacGrade 추가.

#define _PD_SETITEM_							// 20080710 : 세트 아이템 적용.
#define _PD_SEPARATE_SOCKET_					// 20080710 소켓 분리 기능 추가.
#define _PD_ITEM_V24_SOCKET_					// 20080710 아이템 v2.4 적용과 소켓 삽입 조건 검사 추가. _PD_SEPARATE_SOCKET_ 같이 포함할 것.

#define _PD_MASK_PENALTY_						// 20080603 : 복면 사용시, 경험치 습득 불가, 아이템 드롭 불가 설정.

#define _PD_GAME_EVENT_							// 20080513 : Game Event(DB/DS/GS) : Free Match 

#define _PD_DELAY_BOX_SPAWN_					// 20080508 : 비나 - 패왕화로의 스폰은 진위장군 사후로 미룬다.
#define _PD_PKEVENT_BOX_SPAWN_					// 20080325 : 구궁패왕화로의 생성을 진위장군 스폰 시점으로 옮긴다. : DB/DS/GS & Client
*/

// 20080905 : 긴급패치로, 예전 버전으로 돌림. 
#define _PD_COMMON_RANK_						// 20080902 : 공통 랭크 추가.( CommonRank.txt )
#define _PD_CASTLE_CHALLENGE_LEVEL_LIMIT_		// 20080901 : 108 레벨 이상만 도전 가능.
#define _PD_RETURN_LOBBY						// 20080901 : 다른 캐릭터 선택하기. 20080903 15시 이후에는 DS 와 함께 전달할 것.
#define _PD_DEL_NOENEMY_EFFECT_					// 20080901 : 무적 상태효과 제거.
#define _PD_BUFF_NPC_CHECK_DIST_				// 20080901 : 점혈술 NPC 와의 거리 체크.

#define _PD_LVPK_REGION_ATTACK_MOD_				// 20080828 : 영역기의 대상만 대전이 걸림.
#define _PD_CASTLE_MOB_ATTACKED_ONLY_ATTACKER_	// 20080828 : 장원전에서, 영영기 사용시, 수성측 공격은 몬스터에 적용되지 않도록 한다. ( _PD_CASTLE_STEP_1_ 와  같이 디파인 할 것 )

#define _PD_BUGFIX_20080630_					// 20080828 : 7/3일 패치에 수정될 버그. 상태효과 값의 동기화, 무공 시전시간 체크.
#define _PD_BUGFIX_20080616_					// 20080828 : 6/26 일 패치시 수정될 버그. : 다른 나라는 영역기 들어갈 때, 같이 들어갈 것.

#define _PD_MASK_PENALTY_						// 20080603 : 복면 사용시, 경험치 습득 불가, 아이템 드롭 불가 설정.
#define _PD_PKEVENT_3_HOURS_					// 20080929 : 패왕동 출입시간을 3시간으로. ( 원래 1시간 )
#define _PD_DELAY_BOX_SPAWN_					// 20080508 : 비나 - 패왕화로의 스폰은 진위장군 사후로 미룬다.
#define _PD_PKEVENT_BOX_SPAWN_					// 20080325 : 구궁패왕화로의 생성을 진위장군 스폰 시점으로 옮긴다. : DB/DS/GS & Client


#define _PD_GAME_EVENT_							// 20080513 : Game Event(DB/DS/GS) : Free Match 



//////////////////////////////////////////////////////////////////////////
// 20080320 들어간 리뉴얼 버전.
//#define _PD_SPELL_WEAPON_PENALTY_				// 20080320 : 강무기 패널치 조건 -- 지혜->민첩 으로.

//#define _PD_COLLECT_PACKET_MODIFY_				// 20080320 : 명품/수집 등의 패킷 수정 ; Client.


#define _PD_ITEM_V25_							// 20080320 : 아이템 테이블 2.5 버전. 의상과 무공서에 CharacGrade 추가.
#define _MASTERY_SECOND_PATCH_					// 20080320 : 2차 오의 상태효과 적용.


#define _PD_SETITEM_							// 20080320 : 세트 아이템 적용.
#define _PD_MOB_APPLYEFFECT_MOD_				// 20080320 : 몬스테에게 적용되는 상태효과 중, 특정 상태효과는 다른 것으로 바뀐다.

#define _PD_CASTLE_RENEWAL_EXP_					// 20080320 : 장원전 리뉴얼 - 보너스 경험치 주기.
#define _PD_CASTLE_RENEWAL_1_					// 20080320 : 장원전 리뉴얼 - 스케줄링, 버프 추가.

#define _PD_ITEM_V24_SOCKET_					// 20080320 : 아이템 v2.4 적용과 소켓 삽입 조건 검사 추가. _PD_SEPARATE_SOCKET_ 같이 포함할 것.



//#define _PD_MONSTER_REF_1_5_					// 20080320 : monster ref 1.5 
//#define _PD_ITEM_RENEWAL_1008_					// 20080320 : 무기 암상인 리뉴얼 : 무기 조합

//////////////////////////////////////////////////////////////////////////


#define _PD_PKBUG_MASK__ATTACKER_NAME_			// 20080122 : PK 공격자가 복면인일 경우, 이름이 뜨지 않도록.

//#define _PD_GAMBLE_FIX_0821_					// 2008 겜블 변경.

#define _PD_SEPARATE_SOCKET_					// 20070710 소켓 분리 기능 추가.

#define _PD_RECALL_FRIEND_						// 친구 소환

//
#define _PD_CHARAC_LTS_							// LTS IJA. 디파인 하나로 줄였어여. : DS/DB( GetCharacUID() )
#define _PD_CHARAC_LTS_MASK_BUG_				// 복면인을 특정인 위치추적으로 추가할 수 있는 버그 수정.

#define _PD_LEVEL_RATE_SAVE_					// 레벨의 비율(0-1 까지)을 저장하여, 변경 플래그가 켜지면, 비유을 조정하게 한다. DS/GS
#define _PD_WEAPON_PROTECT_						// 용린/용루 시스템. 시작 : 20070628

#define _PD_SAFEZONE_HEAVY_TIME_DEC_			// 2007 06 12 중범죄자 시간 감소.
#define _PD_PARTY_STATE_ADD_					// 20070604 동행의 상태 강화 자리비움 : DS 

#define	_MASTERY_NEED_PAGE_POINT_				// 필요 포인트 변경부분 : 20070516
#define _PD_SKILL_ADD_FROM_MASTERY_				// 오의에 의한 스킬 추가. : 20070516

#define _PD_CASTLE_STEP_2_						// 장원전 : 2단계 전적/세금/1차버그수정 : 20070427

#define _PD_EXPBONUS_SAVE_						// 경험치/숙련도 보너스를 저장하고 있는다.
#define _PD_CASTLE_STEP_1_

//#define _PD_VINA_PCROOM_INFO_					// 비나 피씨방 정보 포함. DS 20070919
#define _PD_SDS_GOSTOP_							// 20070913 작업 시작. DS
#define _VINA_SHUTDOWN_20061023_				//베트남 shutdown

#define	_CIS_20070326_		// 아이템몰
//#define	_CIS_20070326_LOG_

//////////////////////////////////////////////////////////////////////////
// Event 
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 해킹 방지
//////////////////////////////////////////////////////////////////////////
#define _PD_CRYPTO_GETKEY_MOD_					// 20071115
#define _PD_RECV_HACKPROGRAM_LIST_				// 
#define _PD_CHECK_VERSION_BY_CHANNEL_			// 20071112
#define _PD_CHECK_ATK_IN_CHANNEL_				// 20071105 채널링 중에 전투가 오면, 끊는다.
#define _PD_VINA_DEL_NO_CLASS_SKILL_			// 직책에 맞지 않는 무공 삭제.
#define _PD_CHECK_ATKPACKET_DISTANCE_			// P37 20071012 : 거리 체크하기. 체크되면, 공격이 실패된다.
#define _PD_PACKET_CRYPTO_CHECK_				// 패킷들의 암호화를 체크한다.

