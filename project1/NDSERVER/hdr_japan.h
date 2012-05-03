#pragma once


//////////////////////////////////////////////////////////////////////////
// 컨텐츠
//////////////////////////////////////////////////////////////////////////

#define _PD_CASH_INVEN_							// 20080926 : 캐쉬 인벤 및 표국 확장 : DS/GS/GMS/Client : CashInventory.doc

#define _PD_COMMON_RANK_						// 20080829 : 공통 랭크 추가.( CommonRank.txt )

#define _PD_SETITEM_							// 20080822 : 세트 아이템 적용.
#define _PD_ITEM_V25_							// 20080822 : 아이템 테이블 2.5 버전. 의상과 무공서에 CharacGrade 추가.

#define _PD_RETURN_LOBBY						// 20080814 : 다른 캐릭터 선택하기. 20080903 15시 이후에는 DS 와 함께 전달할 것.
#define _PD_CLOTH_VIEW_SELECT_					// 20080814 : 의상 보기를 치장의상과 일반의상 중에 선택할 수 있도록 한다. : client

#define _PD_ADD_EVENT_COLLECTION_				// 20080418 : 수집 데이타 부족으로, 이벤트성 수집은 1000 번 부터 사용하도록, 확장.
#define _PD_COLLECT_PACKET_MODIFY_				// 20080421 : 

#define _PD_PKEVENT_3_HOURS_					// 20080929 : 패왕동 출입시간을 3시간으로. ( 원래 1시간 )
#define _PD_PKEVENT_BOX_SPAWN_					// 20080526 : 구궁패왕화로의 생성을 진위장군 스폰 시점으로 옮긴다. : DB/DS/GS & Client

#define _PD_PATROL_RESPAWN_BUGFIX_				// 20080326 : 경비 NPC 리스폰 안되는 문제 해결.

#define _PD_WEAPON_PROTECT_						// 20080225 : 용린/용루 시스템.

#define _PD_ITEM_PM_COMMON_1024					// 20080103 : 아이테의 공성/방어력/공회피율 물리/기공에 모두 적용하고, 버그 수정.

#define _PD_MONSTER_REF_1_5_					// 20080103 : monster ref 1.5
#define _PD_ITEM_RENEWAL_1008_					// 20080103 : 무기 암상인 리뉴얼 : 무기 조합

#define _PD_GAMBLE_FIX_0821_					// 20080103 : 겜블 변경.

#define _PD_SEPARATE_SOCKET_					// 20080103 : 소켓 분리 기능 추가.

#define _PD_ITEM_V24_SOCKET_					// 20080107 : 아이템 v2.4 적용과 소켓 삽입 조건 검사 추가. _PD_SEPARATE_SOCKET_ 같이 포함할 것.

#define _PD_4_EFFECT_ADD_						// 20080103 : 음양혼신 공격/저항력 추가. : 공격력/반사/흡수/저항력 : 스크립트(penalty_of_zone.txt)

#define _PD_RECALL_FRIEND_						// 20080103 : 친구소환 20070604 시작.

#define _PD_PARTY_STATE_ADD_					// 20080103 : 동행의 상태 강화 자리비움 : DS 
#define _PD_CHARAC_LTS_							// 20080103 : LTS IJA. 국내 20070329 : DS/CS 패킷.

#define	_MASTERY_NEED_PAGE_POINT_				// 20080103 : 필요 포인트 변경부분
#define _PD_SKILL_ADD_FROM_MASTERY_				// 20080103 : 오의에 의한 스킬 추가.
#define _PD_PVP_EFFECT_MODIFY_					// 20080103 : 효과에 따라, PVP 감소 효과가 다르게 적용.

#define _PD_MONSTER_SKILL_DAM_					// 20080103 : 몬스터의 스킬 공격력 및 공성치 적용. 2006/08/18 Ray.


//////////////////////////////////////////////////////////////////////////
// 비컨텐츠
//////////////////////////////////////////////////////////////////////////
// 해킹 방지
#define _PD_PACKET_CRYPTO_CHECK_				// 20080103 : 패킷들의 암호화를 체크한다.
#define _PD_CRYPTO_GETKEY_MOD_					// 20080103 : 
#define _PD_RECV_HACKPROGRAM_LIST_				// 20080103 : 
#define _PD_CHECK_VERSION_BY_CHANNEL_			// 20080103 : 
#define _PD_CHECK_ATK_IN_CHANNEL_				// 20080103 : 채널링 중에 전투가 오면, 끊는다.
#define _PD_CHECK_ATKPACKET_DISTANCE_			// 20080103 : 거리 체크하기. 체크되면, 공격이 실패된다.


//////////////////////////////////////////////////////////////////////////
// Log DB : 계정 길이 증가
#define _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_		// 20080107 로그 서버 패킷 변경.

//////////////////////////////////////////////////////////////////////////
//
#define	_CIS_20070326_							// 아이템몰
#define _CIS_20070326_LOG_

//////////////////////////////////////////////////////////////////////////
// Event 
//////////////////////////////////////////////////////////////////////////
