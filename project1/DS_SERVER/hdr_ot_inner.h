#pragma once

#define _PD_SKILL_EXTEND_				// 20080519 : DB/DS/GMS/GS 스킬의 개수 확장

#define _PD_CASH_INVEN_					// 20080414 : 캐쉬 인벤 및 표국 확장 : DS/GS/GMS/Client

//#define _PD_PARTY_INFO_CLASSGRADE_		// 20080410 : 파티 정보에 역할 등급을 추가. GS

#define _PD_ADD_EVENT_COLLECTION_		// 20080327 : DS/GS/GMS 수집 데이타 부족으로 확장함 : DB-inven_v04 추가. \캐릭터\수집정보확장.doc

#define _PD_PKEVENT_BOX_SPAWN_			// 20080306 : 구궁패왕화로의 생성을 진위장군 스폰 시점으로 옮긴다. : DB/DS/GS & Client

//#define _PD_QUEST_MAX_600_				// 20071206 : 퀘스트 개수 확장 : 300 -> 600
#define _PD_CASTLE_RENEWAL_1_			// 20071128 장원전 리뉴얼 - 스케줄링, 버프 추가.
#define _PD_GAME_EVENT_					// 게임 이벤트 처리 : 20070928 : CGameEventManager : GS/DS
//	#define _ORG_PROMOTION_					// 영웅단 승급 작업일자 : 2007.07.29 ~ (예정)08.10 author : YunTaek
#define _PD_LEVEL_RATE_SAVE_			// 레벨에서의 비율(0-1 까지)을 저장하여, 변경 플래그가 켜지면, 비유을 조정하게 한다. DS/GS/DB/GMS
#define _PD_RECALL_FRIEND_				// 친구 소환 기능 구현 : 20070608 시작.
#define _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_	// 20070601 : 로그DB 수정 시작.
#define _PD_BUFF_COUNT_MODIFY_			// 20070422 : 스페셜 버프 추가. DB/GS 작업 필요.

#define _PD_CASTLE_STEP_2_				// 20070403 시작.
#define _PD_CHARAC_LTS_					// 20070328 추가. 위치추적 시스템.


//	#define _PD_CASTLE_EVENT_1_
//#define CHARAC_CREATE_DATE_20070309
#define _PD_GM_ADDITEM_MODIFY_					// 기간제 아이템 추가시 기간 설정 코드 추가 : 20061220 설정
#define _CHARACLIST_DELETE_CHARACTER_20061214_ //캐릭터 삭제
#define	_PD_ALLY_ORG_STEP_1_			// 영웅단 동맹 시작 : Ray. 11월 28일.
//#define _VINA_SHUTDOWN_20061023_		//베트남 shutdown : Ray - 제가 잠깐 잠갔습니다. 20061025 새벽.
#define ITEMSHOP_PATCH_200607
#define _PD_ACCOUNT_PLAY_TIME_			// Log - Account Play Time. In Account DB

#define _PD_CASTLE_STEP_1_				// DS/GS/GMS _CHARAC_STATE 가 바뀐다.
//#define _GMS_SERVERNAME_LENGTH_INCREASED_	// 


#define _SMALL_NETWORK_					// 작업일자: 2007.02.23
#define _NEXON_NETWORK_					// 작업일자: 2007.02.23; 이전에 _SMALL_NETWORK_과 함께 선언되어야 합니다.
#define _NXPCB_AWARD_200703_			// _SMALL_NETWORK_, _NEXON_NETWORK_과 함께 선언되어야 합니다.
#define _NXPCB_INACT_AWARD_200703_		// _NXPCB_AWARD_200703_ 기능을 비활성화 하고 싶을 때

//#define	_CIS_20070326_					// 공용 Cash Item Shop
//#define _CIS_20070326_LOG_


//////////////////////////////////////////////////////////////////////////
// 이벤트 관련
//////////////////////////////////////////////////////////////////////////
//#define _PD_LEVEL_PRESENT_				// 2008022 : 특정 레벨 도달시 선물 : GS/DS GS - script 포함.

#define _PD_EVENT_INVITE_FRIEND_		// 20080327 : 미국 친구 소환 이벤트 : DB/GS

