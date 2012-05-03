#pragma once

#define	_PD_STORAGE_40_LIMIT_					// 20080827 : 표국을 40칸으로 제한한다.ㄴ
#define _PD_CASH_INVEN_							// 20080827 : 캐쉬 인벤 및 표국 확장 : DS/GS/GMS/Client : CashInventory.doc

#define _PD_WEAPON_PROTECT_						// 20080827 : 용린/용루 시스템. 시작 : 20070420 : 20070510 정식.

#define _PD_LVPK_REGION_ATTACK_MOD_				// 20080827 : 영역기의 대상만 대전이 걸림.
#define _PD_CASTLE_MOB_ATTACKED_ONLY_ATTACKER_	// 20080827 : 장원전에서, 영영기 사용시, 수성측 공격은 몬스터에 적용되지 않도록 한다. ( _PD_CASTLE_STEP_1_ 와  같이 디파인 할 것 )

#define _PD_SETITEM_							// 20080827 : 세트 아이템 적용.

#define _PD_COMMON_RANK_						// 20080822 : 공통 랭크 추가.( CommonRank.txt )
#define _PD_RETURN_LOBBY						// 20080822 : 다른 캐릭터 선택하기. 20080903 15시 이후에는 DS 와 함께 전달할 것.

#define _PD_BUGFIX_20080630_					// 20080702 : 7/3일 패치에 수정될 버그. 상태효과 값의 동기화, 무공 시전시간 체크.
#define _PD_BUGFIX_20080616_					// 20080702 : 6/26 일 패치시 수정될 버그. : 다른 나라는 영역기 들어갈 때, 같이 들어갈 것.

#define _PD_QUEST_FOR_DUNGEON_					// 20080702

#define _PD_ITEM_BONUS_SPELL_APPLY_				// 20080702 : 아이템/캐릭터의 보너스 Apply 가, 기공에서 적용되지 않던 것 적용. : 국내 적용후, 없앤다.
#define _PD_MOB_M_DEFENCE_FIX_					// 20080702 : 몬스터 물리/기공 방어 계산 오류 수정 + 캐릭터 기공 방어 계산 오류. : 국내 적용후, 없앤다.


#define _PD_SECOND_JOB_							// 20080827 : 2차 전직 - 상태효과와 클래스 적용.
#define _PD_ITEM_V25_							// 20080827 : 아이템 테이블 2.5 버전. 의상과 무공서에 CharacGrade 추가.
#define _MASTERY_SECOND_PATCH_					// 20080827 : 2차 오의 상태효과 적용.

#define _PD_CASTLE_RENEWAL_EXP_					// 20080827 : 장원전 리뉴얼 - 보너스 경험치 주기.

#define _PD_SPELL_WEAPON_PENALTY_				// 20080827 : 강무기 패널치 조건 -- 지혜->민첩 으로.

#define _PD_ITEM_PM_COMMON_1024					// 20080702 : 아이템의 공성/방어력/공회피율 물리/기공에 모두 적용하고, 버그 수정.

//#define _PD_4_EFFECT_ADD_						// 20080702 : 음양혼신 공격/저항력 추가. : 공격력/반사/흡수/저항력 : 스크립트(penalty_of_zone.txt)

#define _PD_SKILL_ADD_FROM_MASTERY_				// 20080702 : 오의에 의한 스킬 추가.
#define _PD_PVP_EFFECT_MODIFY_					// 20080702 : 효과에 따라, PVP 감소 효과가 다르게 적용. : 20080801 - 재혁씨 요청으로 지운다.

#define	_MASTERY_NEED_PAGE_POINT_				// 20080702 : 필요 포인트 변경부분

#define _PD_MONSTER_SKILL_DAM_					// 20080702 : 몬스터의 스킬 공격력 및 공성치 적용. 2006/08/18 Ray.

#define _PD_MOB_APPLYEFFECT_MOD_				// 20080702 : 몬스테에게 적용되는 상태효과 중, 특정 상태효과는 다른 것으로 바뀐다.

#define _PD_LEVEL_RATE_SAVE_					// 20080516 : 레벨의 비율(0-1 까지)을 저장하여, 변경 플래그가 켜지면, 비유을 조정하게 한다. DS/GS/DB

#define _PD_ADD_EVENT_COLLECTION_				// 20080404 : 수집 데이타 부족으로, 이벤트성 수집은 1000 번 부터 사용하도록, 확장.
#define _PD_COLLECT_PACKET_MODIFY_				// 20080404 : 



#define _PD_GAMBLE_FIX_0821_					// 20080414 겜블 변경 : 10 => 3가지로 축소.

#define _PD_SEPARATE_SOCKET_					// 20080320 소켓 분리 기능 추가.
#define _PD_ITEM_V24_SOCKET_					// 20080320 아이템 v2.4 적용과 소켓 삽입 조건 검사 추가. _PD_SEPARATE_SOCKET_ 같이 포함할 것.


#define _PD_RECALL_FRIEND_						// 20080201 : 친구소환 20070604 시작.

//#define _PD_CHARAC_LTS_							// 20071119 : DS/DB( GetCharacUID() )

//////////////////////////////////////////////////////////////////////////
// 해킹 방지
//////////////////////////////////////////////////////////////////////////
#define _PD_CRYPTO_GETKEY_MOD_					// 20071126
#define _PD_RECV_HACKPROGRAM_LIST_				// 20071126
#define _PD_CHECK_VERSION_BY_CHANNEL_			// 20071126
#define _PD_CHECK_ATK_IN_CHANNEL_				// 20071126 채널링 중에 전투가 오면, 끊는다.
#define _PD_VINA_DEL_NO_CLASS_SKILL_			// 20071126 : 직책에 맞지 않는 무공 삭제.
#define _PD_CHECK_ATKPACKET_DISTANCE_			// 20071126 : 거리 체크하기. 체크되면, 공격이 실패된다.
#define _PD_PACKET_CRYPTO_CHECK_				// 패킷들의 암호화를 체크한다.
//////////////////////////////////////////////////////////////////////////


#define _PD_CASTLE_RENEWAL_1_					// 20071210 장원전 리뉴얼 - 스케줄링, 버프 추가.

#define _PD_CASTLE_STEP_2_						// P24 장원전 : 2단계 전적/세금/1차버그수정 : 20071017
#define _PD_CASTLE_STEP_1_						// P24 20071017


#define _PD_FREE_SAVE_POSITION_					// 여점 지정에 돈이 들지 않게 함 : 여점 이름이 동일하여 생긴 문제.

#define _PD_SAFEZONE_HEAVY_TIME_DEC_			// 2007 06 12 중범죄자 시간 감소.



//////////////////////////////////////////////////////////////////////////
// 광고 시스템 관련
//////////////////////////////////////////////////////////////////////////
#define _PD_AD_REWARD_200701					// 광고 보고나서 보상 받기
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 게임 가드.
//////////////////////////////////////////////////////////////////////////
//#define _PD_USE_GAMEGUARD_						// 게임 가드 인증 사용.
//#define _PD_GAMEGUARD_VER_1_					// 게임가드 인증 버전 1 사용. : 2008/02/28 이전.
//#define _PD_GAMEGUARD_VER_2_					// 게임가드 인증 버전 2 사용. : 2008/02/28 이후.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CIS 설정
//////////////////////////////////////////////////////////////////////////
#define	_CIS_20070326_							// 아이템몰
//#define	_CIS_20070326_LOG
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Event 
//////////////////////////////////////////////////////////////////////////
//#define _PD_LEVEL_PRESENT_						// 20080404 : 레벨 달성 선물 주기. 
#define _PD_EVENT_INVITE_FRIEND_				// 20080417 : 미국 경공 추가 이벤트( 친구 추천 )
//////////////////////////////////////////////////////////////////////////
