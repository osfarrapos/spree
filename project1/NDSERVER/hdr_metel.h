#pragma once

//////////////////////////////////////////////////////////////////////////
// Contents
//////////////////////////////////////////////////////////////////////////
#define _PD_ITEM_V25_							// 20080910 : 아이템 테이블 2.5 버전. 의상과 무공서에 CharacGrade 추가.

#define _PD_LEVEL_RATE_SAVE_					// 20080131 : 레벨의 비율(0-1 까지)을 저장하여, 변경 플래그가 켜지면, 비유을 조정하게 한다. DS/GS


#define _PD_GAME_EVENT_							// 20080103 Game Event(DB/DS/GS) : Free Match 

#define _PD_ITEM_PM_COMMON_1024					// 20080103 : 아이테의 공성/방어력/공회피율 물리/기공에 모두 적용하고, 버그 수정.

#define _PD_MONSTER_REF_1_5_					// 20080103 : monster ref 1.5
#define _PD_ITEM_RENEWAL_1008_					// 20080103 : 무기 암상인 리뉴얼 : 무기 조합

#define _PD_GAMBLE_FIX_0821_					// 20080103 : 겜블 변경.

#define _PD_SEPARATE_SOCKET_					// 20080103 : 소켓 분리 기능 추가.

#define _PD_ITEM_V24_SOCKET_					// 20080115 : 아이템 v2.4 적용과 소켓 삽입 조건 검사 추가. _PD_SEPARATE_SOCKET_ 같이 포함할 것.


#define _PD_4_EFFECT_ADD_						// 20080103 : 음양혼신 공격/저항력 추가. : 공격력/반사/흡수/저항력 : 스크립트(penalty_of_zone.txt)

#define _PD_RECALL_FRIEND_						// 20080103 : 친구소환 20070604 시작.

#define _PD_PARTY_STATE_ADD_					// 20070426 동행의 상태 강화 자리비움 : DS 
#define _PD_CHARAC_LTS_							// 20080103 : LTS IJA. 국내 20070329 : DS/CS 패킷.

#define	_MASTERY_NEED_PAGE_POINT_				// 20080103 : 필요 포인트 변경부분
#define _PD_SKILL_ADD_FROM_MASTERY_				// 20080103 : 오의에 의한 스킬 추가.
#define _PD_PVP_EFFECT_MODIFY_					// 20080103 : 효과에 따라, PVP 감소 효과가 다르게 적용.

#define _PD_MONEY_CAN_DROP_						// 돈 버리기 가능

#define _PD_MONSTER_SKILL_DAM_					// 20080103 : 몬스터의 스킬 공격력 및 공성치 적용. 2006/08/18 Ray.


//////////////////////////////////////////////////////////////////////////

#define	_PD_PCROOM_INFO_						// PC 방 혜택 주어진다. 5기 +5.

// 해킹 방지
#define _PD_SKIP_CHECK_							// 패킷 시퀀스가 안맞아도 넘어간다.
#define _PD_PACKET_CRYPTO_CHECK_				// 패킷들의 암호화를 체크한다.
#define _PD_CRYPTO_GETKEY_MOD_					// 20080103 : 
#define _PD_RECV_HACKPROGRAM_LIST_				// 20080103 : 
#define _PD_CHECK_VERSION_BY_CHANNEL_			// 20080103 : 
#define _PD_CHECK_ATK_IN_CHANNEL_				// 20080103 : 채널링 중에 전투가 오면, 끊는다.
//#define _PD_VINA_DEL_NO_CLASS_SKILL_			// 20080103 : 직책에 맞지 않는 무공 삭제.
#define _PD_CHECK_ATKPACKET_DISTANCE_			// 20080103 : 거리 체크하기. 체크되면, 공격이 실패된다.


//////////////////////////////////////////////////////////////////////////
// CIS
//////////////////////////////////////////////////////////////////////////
#define	_CIS_20070326_		// 아이템몰



//////////////////////////////////////////////////////////////////////////
// Event 
//////////////////////////////////////////////////////////////////////////
#define _PD_LEVEL_PRESENT_						// 20080822 : 레벨 달성 선물 주기.


//////////////////////////////////////////////////////////////////////////
// 보안 모듈.
//////////////////////////////////////////////////////////////////////////
#define _PD_APEX_INCLUDE_
//#define _PD_APEX_TEST_MODE_