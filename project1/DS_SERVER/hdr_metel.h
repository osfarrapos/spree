#pragma once

#include "./Billing/GameAPI.h"
#define _METEL_LOGIN_200608_
#define _METEL_LOGIN_NOT_USE_CALL_		// 20080812 : 

#define _PD_LEVEL_RATE_SAVE_			// 20080131 : 레벨에서의 비율(0-1 까지)을 저장하여, 변경 플래그가 켜지면, 비유을 조정하게 한다. DS/GS/DB/GMS

//////////////////////////////////////////////////////////////////////////
#define	_PD_ALLY_ORG_STEP_1_			// 영웅단 동맹 시작 : Ray. 11월 28일. : 1218일 청홍 시작	
#define _PD_GM_ADDITEM_MODIFY_					// 기간제 아이템 추가시 기간 설정 코드 추가 : 20061220 설정

#define	_PD_PCROOM_INFO_			// PC 방 혜택 주어진다. 5기 +5.
#define _PD_INVEN_SAVE_CHANGED_		// M-etel 요청사항
// 캐릭터 생성시 백업으로 호출되던 Inven V2 Procedure 를 Inven V3 로 변경한다.

#define _PD_GAME_EVENT_					// 20080103 : 게임 이벤트 처리 : CGameEventManager : GS/DS
#define _PD_RECALL_FRIEND_				// 20080103 : 친구 소환
#define _PD_BUFF_COUNT_MODIFY_			// 20080103 : 스페셜 버프 추가. DB/GS 작업 필요.

#define _PD_CHARAC_LTS_					// 20080103 : 추가. 위치추적 시스템. 20070329

#define _PD_GM_ADDITEM_MODIFY_					// 기간제 아이템 추가시 기간 설정 코드 추가 : 20061220 설정
#define _CHARACLIST_DELETE_CHARACTER_20061214_ //캐릭터 삭제


#define	_CIS_20070326_					// 공용 Cash Item Shop : 20080201


//////////////////////////////////////////////////////////////////////////
// 이벤트 관련
//////////////////////////////////////////////////////////////////////////
#define _PD_LEVEL_PRESENT_					// 20080822 : 특정 레벨 도달시 선물 : GS/DS GS - script 포함.
