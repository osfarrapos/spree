#pragma once

// 장원전

struct _sHeader;

#define MSG_NO_CB_SCHEDULE			176		// 장원전 : 스케줄 관리 패킷

#define max_count_of_challenge_list		20		// 장원전 도전자 최대 20개

struct _sCastleInfo
{
	int				iOrIndex;			// default 0 : for monster, or Org/Ally Index
	u_char			ucType;				// 0-Organization, 1-Ally
	u_short			usMemberCount;		// 인원수

	u_char			ucLevel;			// 장원 레벨

	/* 아래 변수 ucCurMatchCount, ucMatchState 설명
		1차전 예정이라면 : Count = 0, State = 0,
		2차전 중이라면 : Count = 1, state = 1,
		3차전 중이라면 : Count = 2, state = 2
	*/
	u_char			ucCurMatchCount;	// 0, 1, 2차 방어전
	u_char			ucMatchState;		// 0:시작전, 1:전쟁중

	char			cOrName[_XDEF_ORGANIZATIONNAMELENGTH + 1];
	char			cMasterName[13];
	char			cMasterMonpa;

	u_int			uiTotalMoney;
	u_int			uiPartMoney;

	u_short			usWinCount;			// 성공 회수

	// 장원 소유 날짜 
	_sND_TimeUnit	timeUnit;

	_sND_TimeUnit	Schedule[3];		// 3차전까지의 스케줄
};

struct __cb_client_challenge_info
{
	u_short			usMemberCount;
	
	u_char			ucMoonpa : 5;		// 문파 index
	u_char			ucRes1 : 1;
	u_char			ucState : 1;		// 0:normal   1:fighting state
	u_char			ucType : 1;			// 0:Org	1:Ally

	char			cGroupName[_XDEF_ORGANIZATIONNAMELENGTH + 1];
	char			cMasterName[13];
};

enum EN_CB_SCHEDULE
{
	en_cb_owner_req_schedule_list = 0,	// 0 장주의 스케줄 리스트 요청 패킷
	en_cb_owner_schedule_list,			// 1 장주의 스케줄 리스트

	en_cb_req_schedule_list,			// 2 일반 단주(회주) 스케줄 리스트 요청 패킷
	en_cb_schedule_list,				// 3 스케줄 리스트

	en_cb_req_challenge_list,			// 4 도전 목록 요청
	en_cb_challenge_list,				// 5 도전 목록

	en_cb_req_change_time = 20,			// 20 장주의 시간 변경 요청
	en_cb_change_time_success,			// 21 시간 변경 요청 성공
	en_cb_change_time_fail,				// 22 시간 변경 요청 실패

	en_cb_req_challenge = 40,			// 40 회주(단주)의 도전 신청
	en_cb_challenge_success,			// 41 도전 신청 성공
	en_cb_challenge_fail,				// 42 도전 신청 실패 : 돈부족,Full,중복,현재장주,잠시후,시스템에러
	en_cb_challenge_cancel,				// 43 도전 신청 취소

	en_cb_u_are_owner = 60,				// 60 자신이 장원의 소유주임을 알려준다
	en_cb_u_are_not_owner,				// 61 더이상 장원의 소유주가 아님을 알린다 (장원의 소유가 바뀌었을 때, 이전 소유주에게)
	en_cb_not_owner,					// 62 owner_req_shcedule_list가 왔을 때 응답으로 갈 수 있다
	en_cb_no_such_castle,				// 63 요청한 장원은 없는 장원이다
	en_cb_cur_owner_info,				// 64 장원이 있는 지역에 들어갈 때, 현 장원소유주의 정보를 보내준다
	
	en_cb_map_tax_rate = 70,			// 70 유저가 필드에 들어왔을 때 세율 정보 보내줌
	en_cb_set_total_tax,				// 71  장원 세금 세팅

	en_cb_tax_request_take,				// 72 장주가 세금을 거둬간다
	en_cb_tax_answer_take,				// 73 72번의 응답, 성공/실패 모두

	en_cb_schedule_error = 100,			// 100 에러
};

 
struct MSG_CB_SCHEDULE
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;

};

// en_cb_owner_req_schedule_list = 0,
struct MSG_CB_OWNER_REQ_SCHEDULE_LIST : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;			// 자신이 장주인 맵 인덱스
};

// en_cb_onwer_schedule_list
struct MSG_CB_OWNER_SCHEDULE_LIST : public MSG_CB_SCHEDULE
{
	u_int			uiSaveMoney;		// 장원 창고의 저장된 돈

	_sCastleInfo	CastleInfo;

	__cb_client_challenge_info	info[max_count_of_challenge_list];	// 최대 20개
};

// en_cb_req_schedeule_list
struct MSG_CB_REQ_SCHEDULE_LIST : public MSG_CB_SCHEDULE
{
	//
};

// en_cb_schedule_list
struct MSG_CB_SCHEDULE_LIST : public MSG_CB_SCHEDULE
{
	_sCastleInfo	CastleInfo;			// 장원 정보 (시간포함)
};

// en_cb_req_challenge_list
struct MSG_CB_REQ_CHALLENGE_LIST : public MSG_CB_SCHEDULE
{
};

// en_cb_challenge_list
struct MSG_CB_CHALLENGE_LIST : public MSG_CB_SCHEDULE
{
	__cb_client_challenge_info		info[max_count_of_challenge_list];
};

// en_cb_req_change_time 
struct MSG_CB_REQ_CHANGE_TIME : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;

	_sND_TimeUnit	time[3];
};

// en_cb_change_time_success
struct MSG_CB_CHANGE_TIME_SUCCESS : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;

	_sND_TimeUnit	timeResult[3];		// 변경된 시간
};

// en_cb_change_time_fail
struct MSG_CB_CHANGE_TIME_FAIL : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;

	u_char			errorCode;		// 0 - 고칠수 있는 시간이 지났다
};


// en_cb_req_challenge
struct MSG_CB_REQ_CHALLENGE : public MSG_CB_SCHEDULE
{
	u_int			uiNeedMoney;	// 필요한 돈을 확인차 넣음
};

// en_cb_challenge_success
struct MSG_CB_CHALLENGE_SUCCESS : public MSG_CB_SCHEDULE
{
	u_int			uiLeftMoney;	// 남은돈

	__cb_client_challenge_info	challengeInfo;
};

// en_cb_challenge_fail
struct MSG_CB_CHALLENGE_FAIL : public MSG_CB_SCHEDULE
{
	u_char			errorCode;
	/*
		0 : 돈 부족
		1 : 도전 목록이 꽉 찼다
		2 : 단주/회주가 아니다
		3 : 현재 장주이므로 도전할 수 없다
		4 : 잠시후에 다시 시도 할 것
		5 : 시스템 에러
		6 : 장원 도전 시간이 아닙니다.
		7 : 벌써 도전 되어 있음
		8 : 단주/회주의 레벨이 낮다. // 2008.09.02 베트남 요청 사항
	*/
};

// en_cb_challenge_cancel
struct MSG_CB_CHALLENGE_CANCEL : public MSG_CB_SCHEDULE
{
	// 이전 도전 신청을 취소함
};

// en_cb_u_are_owner = 60
struct MSG_CB_U_ARE_OWNER : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;			// 맵의 장원 장주다
	
	u_char			ucRes1 : 7;
	u_char			ucType : 1;			// 0-Org 1-Ally

	u_char			ucScheduleCanChange;	// 0-no 1-yes 

	/*
		스케줄 조정 시간
		장원전 종료시에는 바로 가능.
		중간에 들어올 경우, ucScheduleCanChange를 1로 받으면,
		그날 24시 전까지 가능. 이후는 실패
	*/
};

// en_cb_u_are_not_owner = 61
struct MSG_CB_U_ARE_NOT_OWNER : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;
};

// en_cb_not_owner		// 62 owner_req_shcedule_list가 왔을 때 응답으로 갈 수 있다
struct MSG_CB_NOT_OWNER : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;		// 맵의 장원 장주다
};

// en_cb_no_such_castle		// 없는 장원
struct MSG_CB_NO_SUCH_CASTLE : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;
};

// en_cb_cur_owner_info		// 장원 현 소유주
struct MSG_CB_CUR_OWNER_INFO : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;		// 0이면 장원이 없음

	_sCastleInfo	OwnerInfo;

	u_char			ucBonusExpRate;	// 보너스 경험치율 - 흑/백도 속성이 맞을 때만 유효,흑백도 속성은 OwnerInfo 의 cMasterMoonpa 를 보고 판단해야 함.
};

// en_cb_schedule_error : 아직 정의 되지 않은 에러
struct MSG_CB_SCHEDULE_ERROR : public MSG_CB_SCHEDULE
{
	u_char			ucErrorCode;
};

// en_cb_map_tax_rate = 70	: 세율
struct MSG_CB_NO_MAP_TAX_RATE : public MSG_CB_SCHEDULE
{
	u_char			ucMapRate;
};

// en_cb_tax_set_total_tax = 71,	: 장원 세금 세팅
struct MSG_CB_TAX_SET : public MSG_CB_SCHEDULE
{
	u_char			ucManorIndex;
	
	u_int			uiTotalMoney;		// display 용도
	u_int			uiPartMoney;
};

// en_cb_tax_request_take = 72 : 장주가 세금을 거둬간다
struct MSG_CB_TAX_REQUEST_TAKE : public MSG_CB_SCHEDULE
{
	u_char			ucCastleIndex;		// 소유한 장원 인덱스

	u_int			uiRequestMoney;
};

// en_cb_tax_answer_take = 73 : 72의 응답
struct MSG_CB_TAX_ANSWER_TAKE : public MSG_CB_SCHEDULE
{
	u_char			ucResult;				// 0:Sucess
	u_int			uiTakeMoney;			// 성공일 때, 찾아가는 돈
	u_int			uiLastMoney;			// 성공일 때, 찾은 후 유저의 돈

	/*	
		ucResult
		0 - Success
		1 - 돈이 없다
		2 - 장주가 아니다
		3 - 소유한 장원이 없다
	*/
};


// 장원전 진행 상태
enum EN_COMBAT_STATUS
{
	en_combat_status_none = 0,			// 장원전과 관련 없는 상태
	en_combat_status_before,			// 시작 전 30분 ~ 시작 직전
	en_combat_status_current,			// 시작 후 ~ 종료 전
	en_combat_status_after,				// 종료 이후
};

// 장원전의 단계를 알리는 이벤트
enum EN_COMBAT_EVENT
{
	en_combat_event_ready = 0,			// 장원전 시작 전 준비 단계 이벤트
	en_combat_event_start,				// 장원전 시작으로 알리는 이벤트
	en_combat_event_end,				// 장원전 종료 이벤트
};


/* 장원전 상태와 이벤트
event :     ready -   start -   end
status : none - before - current - after - none
*/


#define MSG_NO_COMBAT					177			// 장원전 진행 패킷

struct MSG_COMBAT
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucMode;			// EN_COMBAT_PACKET
};

enum EN_COMBAT_PACKET
{
	// 0 - 19 진행 : en_cb_prog_
	// 20 - 39 협박 : en_cb_threat_
	// 40 - 59 장원전 정보 : en_cb_info
	// 60 - 공지 및 사지후, 기타 : en_cb_etc

	en_cb_prog_ready_event = 0,			// 0 준비를 알린다 (30분전에 통보)
	en_cb_prog_start_event,				// 1 시작
	en_cb_prog_end_event,				// 2 종료

	en_cb_threat_start_req = 20,		// 20 협박 시작 요청 : c->s
	en_cb_threat_start_ok,				// 21 협박 시작 허락 : s->c
	en_cb_threat_start_error,			// 22 협박 시작 에러 : s->c
	en_cb_threat_start_notice,			// 23 협박 시작을 알림 : s-> all c
	en_cb_threat_cancel_req,			// 24 협박 중지 요청 : c->s
	en_cb_threat_cancel_ok,				// 25 협박 중지 허락 : s->c
	en_cb_threat_complete_req,			// 26 협박 완료 요청 : c->s
	en_cb_threat_complete_ok,			// 27 협박 완료 허락 : s->c
	en_cb_threat_complete_error,		// 28 협박 완료 실패 : s->c
	en_cb_threat_complete_notice,		// 29 협박 완료를 알림 : s-> all c
	
	en_cb_info_current_info = 40,		// 40 중간에 들어오는 유저에게 가는 현재 장원전 정보
	en_cb_dead_penalty,					// 41 장원전 중 죽을때 마다 가게됨
	en_cb_threatpoint_set,				// 42 장원혈 세팅

	en_cb_etc_loud_chat = 60,			// 60 사자후
	en_cb_etc_notice,					// 61 장원전 공지 사항
	en_cb_etc_cannot_loud,				// 61 외침을 사용할 수 없다.

	en_cb_record_pkt_info_req = 80,		// 80 자기단 장원 전황 정보 요청
	en_cb_record_pkt_info_res,			// 81 자기단의 장원 전황 정보 응답
	en_cb_record_pkt_add_combat,		// 82 장원전 중 전적에 대한 정보
	en_cb_record_pkt_pvp_award,			// 83 장원전 기여보상
	en_cb_record_pkt_manor_rank,		// 84 장원전 승리측 대전 점수 순위
	en_cb_record_pkt_winner_award,		// 85 장원전 특별 보상
};

/*
struct __combat_both_side_info
{
	u_char			ucDefenderType;		// o-org 1-ally 2-monster
	int				iDefenderIndex;
	char			cDefenderGroupName[_XDEF_ORGANIZATIONNAMELENGTH + 1];

	u_char			ucAttackerType;
	int				iAttackerIndex;
	char			cAttackerGroupName[_XDEF_ORGANIZATIONNAMELENGTH + 1];
};*/

struct __combat_side_info 
{
	u_char			ucType;				// group type
	int				iIndex;
	char			cGroupName[_XDEF_ORGANIZATIONNAMELENGTH+1];
};


// en_cb_prog_ready_event = 0
struct MSG_CB_PROG_READY_EVENT : public MSG_COMBAT
{
	u_short					usLeftTimeSec;

	__combat_side_info		Defender;

	u_char					ucAttackerCount;	// 이 개수에 따라 잘라서 갑니다.
	__combat_side_info		Attackers[max_count_of_challenge_list];
};

// en_cb_prog_start_event
struct MSG_CB_PROG_START_EVENT : public MSG_COMBAT
{
	__combat_side_info		Defender;

	u_char					ucAttackerCount;
	__combat_side_info		Attackers[max_count_of_challenge_list];
};

// en_cb_prog_end_event
struct MSG_CB_PROG_END_EVENT : public MSG_COMBAT
{
	u_char					ucWinSide;		// 0:수성성공	1:공성성공

	u_char					ucGroupType;	// 0:org	1:ally		2:monster
	int						iIndex;
	char					cGroupName[_XDEF_ORGANIZATIONNAMELENGTH+1];
};

// en_cb_threat_start_req
struct MSG_CB_THREAT_START_REQ : public MSG_COMBAT
{

};

// en_cb_threat_start_ok
struct MSG_CB_THREAT_START_OK : public MSG_COMBAT
{
	u_char					ucProgressRate;		// 100까지 가면 성공
};

// en_cb_threat_start_error
struct MSG_CB_THREAT_START_ERROR : public MSG_COMBAT
{
	u_char			ucError;
};

// en_cb_threat_start_notice
struct MSG_CB_THREAT_START_NOTICE : public MSG_COMBAT
{
	u_char			ucCurrOwnerSide;		// 0:defender 1:attacker
	
	u_char			ucGroupType;			// 0:org 1:ally
	int				iIndex;
};

// en_cb_threat_cancel_req
struct MSG_CB_THREAT_CANCEL_REQ : public MSG_COMBAT
{

};

// en_cb_threat_cancel_ok
struct MSG_CB_THREAT_CANCEL_OK : public MSG_COMBAT
{
	u_char			ucGroupType;			// 0:org 1:ally
	int				iIndex;
};

// en_cb_threat_complete_req
struct MSG_CB_THREAT_COMPLETE_REQ : public MSG_COMBAT
{

};

// en_cb_threat_complete_ok
struct MSG_CB_THREAT_COMPLETE_OK : public MSG_COMBAT
{

};

// en_cb_threat_complete_error
struct MSG_CB_THREAT_COMPLETE_ERROR : public MSG_COMBAT
{
	u_char			ucError;
};

// en_cb_threat_complete_notice
struct MSG_CB_THREAT_COMPLETE_NOTICE : public MSG_COMBAT
{
	u_char			ucNewOwnerSide;		// 0:defender 1:attacker

	u_char			ucGroupType;
	int				iIndex;
};

// en_cb_info_current_info
struct MSG_CB_INFO_CURRENT_INFO : public MSG_COMBAT
{
	u_char			ucCurrStatus;		// EN_COMBAT_STATUS

	u_char			ucGroupType;		// 현재 임시 소유측
	int				iindex;

	u_short			usElapsedTime;

	__combat_side_info	Defender;

	u_char				ucAttackerCount;
	__combat_side_info	Attackers[max_count_of_challenge_list];
};

#define MAX_CHAT_LENGTH			81			// Zone_chat에서 쓰인 DEFINE

// en_cb_etc_loud_chat
struct MSG_CB_ETC_LOUD_CHAT : public MSG_COMBAT
{
	u_char			ucChat;				// not used
	
	char			cCharName[13];

	char			msg[MAX_CHAT_LENGTH];
};

enum
{
	en_cb_loud_error_not_master = 0,		// 총대장이 아니다
	en_cb_loud_error_over_count,			// 외침 횟수를 넘었다
	en_cb_loud_error_not_time,				// 외침 사용 불가 시간
};

struct MSG_CB_ETC_CANNOT_LOUD : public MSG_COMBAT
{
	u_char			ucErrorCode;
};

// en_cb_etc_notice
struct MSG_CB_ETC_NOTICE : public MSG_COMBAT
{
	u_char			ucChat;
	
	char			msg[MAX_CHAT_LENGTH];
};

// en_cb_dead_penalty
struct MSG_CB_DEAD_PENALTY : public MSG_COMBAT
{
	u_char			ucDeadCountInCombat;
	u_short			usLeftCombatPenalryTime;
};

struct MSG_CB_THREATPOINT_SET : public MSG_COMBAT
{
	u_short			usThreatPoint;
};

// 전적 관련

// 다른 단과의 전적
struct _PerOrgRecord
{
	int				iAllyIndex;
	char			vsOrgName[_XDEF_ORGANIZATIONNAMELENGTH+1];
	char			vsAllyName[_XDEF_ORGANIZATIONNAMELENGTH+1];
	unsigned short	usWinCount;
	unsigned short	usLoseCount;
	unsigned short	usPercentageOfWin;
};

// 한개의 랭킹 정보
struct _OneRank
{
	char			charName[13];		// 캐릭터 이름
	short			sRank;
	short			sCombatPoint;		// 대전 점수: 승수-패수
	unsigned int	uiExp;				// 경험치
	int				iHonor;				// 명성
	unsigned int	uiMoney;			// 포상금
};

// en_cb_record_pkt_info_req = 80			// 80 자기단의 장원전황 정보 요청
struct MSG_CB_RECORD_INFO_REQ : public MSG_COMBAT
{
	int				index;					// 영웅단 인덱스 - 방회라도 영웅단 인덱스를 준다
};

// en_cb_record_pkt_info_res				// 81 자기단의 장원전황 정보 응담
struct MSG_CB_RECORD_INFO_RES : public MSG_COMBAT
{
	u_short			usEntryCount;			// 장원전 참가 횟수

	u_short			usWinCount;
	u_short			usLoseCount;
	u_short			ucCombatCount;

	u_short			usCumulationPercentage;	// 누적 승률

	u_short			usManorIndex;

	_sND_TimeUnit	lastManorCombatDay;		// 마지막으로 치근 장원전 일

	u_short			usPerOrgRecordCount;
	_PerOrgRecord	_vsOrgRecordList[100];	// 타단과의 대전 기록
};

// en_cb_record_pkt_add_combat				// 82 장원전 중 전적에 대한 정보
struct MSG_CB_RECORD_COMBAT_ADD : public MSG_COMBAT
{
	_PerOrgRecord	_vsOrgRecord;
};

// en_cb_record_pkt_pvp_award				// 83 장원전 기여 보상
struct MSG_CB_RECORD_PVP_AWARD : public MSG_COMBAT
{
	short			sPvPPoint;			// 승수 - 대전점수
	int				iFame;				// 명성
	u_int			uiMoney;			// 포상금
	short			sRank;				// 둥수: 0이면 등수 없음, 1-10일 경우 특별 보상 있음
};

// en_cb_record_pkt_manor_rank				// 84 장원전 승리측 대전점수 순위
struct MSG_CB_RECORD_RANK : public MSG_COMBAT
{
	_OneRank		_rankList[10];
};

// en_cb_record_pkt_winner_award
struct MSG_CB_RECORD_WINNER_AWARD : public MSG_COMBAT
{
	short			sRank;				// 순위
	short			sCombatPoint;		// 대전점수
	u_int			uiExp;				// 경험치
	int				iHonor;				// 명성
	u_int			uiMoney;			// 포상금
};