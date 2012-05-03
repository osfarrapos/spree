#pragma once

// Match

struct _sHeader;

#define MSG_NO_MATCH_LIST					59		// 비무 : 리스트 관리 및 정보
#define MSG_NO_MATCH_READY					60		// 비무 준비 : 신청/응답/순서 등등
#define MSG_NO_MATCH_CTRL					61		// 비무 : 시작/중간/결과
#define MSG_NO_MATCH_NOTICE					62		// 비무 에러/결과를 주위에 알림

#define MSG_NO_FREE_MATCH					179		// 자유 비무 패킷

enum
{
	en_match_list_mode_register = 0,		// 0 등록 : 요청과 OK응답
	en_match_list_mode_delete,				// 1 해지 요청 : 요청과 응답
	en_match_list_mode_not_map,				// 2 등록 실패를 알림
	en_match_list_mode_req_list,			// 3 리스트 요청 : 정보 요청 및 리뉴얼 요청
	en_match_list_mode_list_update,			// 4 인원정보 리스트 업데이트 : 리스트를 초기화하고 업데이트
	en_match_list_mode_list_continue		// 5 인원정보 리스트 : 이어짐 : 초기화 없이 추가
};

//#define MST_NO_MATCH_LIST
struct MSG_MATCH_LIST
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;
};

struct __fighterInfo
{
	u_char			ucLength;				// 이 구조체의 길이
	char			cMoonpa;				// 문파
	char			cClass;					// 역할
	u_char			ucSex : 2;				// 성별
	u_char			ucMatchGrade : 6;		// 비무 등급
	u_short			uid;					// 유니크 아이디
	u_short			usWinRate;				// 승률
	u_char			ucMatchState;			// 비무 상태
	u_char			ucInnerLevel;			// 내공 수위
	char			cCharacName[13];		// 이름의 길이 + 1로 자른다.
};

const int size_bimu_pack = 2040;

struct MSG_MATCH_LIST_INFO : public MSG_MATCH_LIST
{
	u_char			ucCount;				// 인원정보 개수
	
	char			cPack[size_bimu_pack];	// __fighterInfo가 ucCount 만큼 포함되어있음
};

enum
{
	en_match_ready_challenge_to_other = 0,		// 0 c->s 누군가에게 비무를 신청한다
	en_match_ready_challenge_from_other,		// 1 s->c 누군가가 나에게 비무를 신청하고 있다.
	en_match_ready_challenge_cancel,			// 2 c->s s->c : 취소하거나 취소됐음을 알려줌
	en_match_ready_reply,						// 3 요청에 응답, 또는 다른 이유를 보냄
	en_match_ready_start_make_table = 10,		// 10 비무첩 작성을 시작하라
	en_match_ready_update_waittime,				// 11 대기시간 업데이트
	en_match_ready_update_recordtype,			// 12 공식/비공식 변경
	en_match_ready_update_matchtype,			// 13 비무 형태 변경 : 집단전에서만
	en_match_ready_update_matchtime,			// 14 비무 시간 변경 : 2분부터 시작
	en_match_ready_update_disable_hyperrun,		// 15 경공 사용 유무 변경
	en_match_ready_update_keep_buff,			// 16 무공 효과의 유지 여부 변경
	en_match_ready_update_balance,				// 17 비무 균형 조절
	en_match_ready_req_add_item,				// 18 아이템 올리기 요청 : 응답 22 or 100
	en_match_ready_req_del_item,				// 19 아이템 내리기 요청 : 응답 22 or 100
	en_match_ready_add_item,					// 20 비무첩에 아이템을 추가
	en_match_ready_del_item,					// 21 비무첩에서 아이템을 삭제
	en_match_ready_set_my_item,					// 22 비무첩 아이템을 올리거나 내린 결과 : 그대로 세팅
	en_match_ready_update_after_draw,			// 23 집단전 무승부 일 경우, 다음 방식 결정 0종료/1선봉 재대결
	en_match_ready_req_select_member,			// 24 집단전 : 인원 선택 요청
	en_match_ready_select_member,				// 25 집단전 : 인원 선택 했음
	en_match_ready_deselect_member,				// 26 집단전 : 인원 선택 취소 ('---'로 표시)
	
	en_match_ready_free_request_match = 60,		// 60 자유비무 요청 패킷
	en_match_ready_free_match_full,				// 61 자유비무 - 인원 초과 실패
	en_match_ready_free_no_money,				// 62 자유비무 - 돈이 없어서 못들어감

	en_match_ready_confirm = 80,				// 80 c->s s->c1,c2 확인 버튼 눌렀을 때, 서버로 요청/응답 받거나 알려짐
	en_match_ready_cancel_all,					// 81 비무 준비를 완전히 해제 한다
	
	en_match_ready_errno_cannotupdate = 100,	// 100 비무 준비 에러 : 변경할 수 없음
	en_match_ready_errno_no_item				// 101 비무 준비 에러 : 그런 아이템 없음
};

//#define MSG_NO_MATCH_READY
struct MSG_MATCH_READY
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;
};

// Mode : en_match_ready_challenge_to_other
struct MSG_MATCH_READY_CHALLENGE_TO : public MSG_MATCH_READY
{
	u_char			ucMatchType;				// 개인/집단 구분 : 개인0 집단1
	u_short			usTo;						// 대상의 아이디
};

// Mode : en_match_ready_challenge_from_other
struct MSG_MATCH_READY_CHALLENGE_FROM : public MSG_MATCH_READY
{
	u_char			ucMatchType;				// 개인/집단만 구분 : 개인0, 집단1
	u_short			usFrom;						// 보낸 상대의 유니크 아이디
	char			cName[13];					// 이름
};

// Mode : en_match_ready_challenge_cancel
struct MSG_MATCH_READY_CHALLENGE_CANCEL : public MSG_MATCH_READY
{
	u_short			usFrom;
	// 신청자가 서버에 취소를 알릴때
	// 피신청자에게 신청자의 취소를 알릴때
	// 자신의 아이디 일때 서버에서 취소시킨것 
};

// Mode : en_match_ready_reply
struct MSG_MATCH_READY_REPLY : public MSG_MATCH_READY
{
	u_char			ucReply;
	u_short			usFrom;			// 누구에게 응답하는지

	/* ucReply : 
		0 - OK 승락, c2->s
		1 - c2->s 거절 : 대상이 거절했습니다.
		2 - s->c1 : 대상은 현재 비무중입니다.
		3 - s->c1 : 대상은 현재 비무 준비중입니다.
		4 - s->c1 : 대상과의 레벨 차이가 너무 큼
		5 - s->c1 : 이전 요청을 먼저 취소하십시오.
		6 - s->c1 : 자신이 비무중이므로 요청할 수 없습니다.
		7 - s->c1 : 대상이 없습니다.
		8 - s->c1 : 집단전은 행수만 요청할 수 있습니다.
		9 - s->c1 : 대상은 행수가 아닙니다. (집단전에서)
		100 - s->c1 : 그 비무는 아직 준비중에 있습니다.
	*/
};

// 비무첩에서 보이는 상대방의 정보
struct __matchMemberInfo
{
	u_char		ucTeam : 2;			// 0:신청자팀 1:피신청자팀
	u_char		ucNumber : 6;		// 0~8 팀내의 순번 : 개인전에서는 0으로

	u_char		ucSex : 2;
	u_char		ucMoonpa : 6;	

	char		cClass;				// 역할
	u_char		ucLevel;			// 레벨
	u_short		usWinRate;			// 승률
	u_char		ucMatchGrade;		// 계급장
	u_char		ucVitalRate;		// 체력
	char		cName[13];			// 이름
};

// Mode : en_match_ready_start_make_table
struct MSG_MATCH_READY_START_MAKE_TABLE : public MSG_MATCH_READY
{
	u_char				ucMatchType;		// 개인/집단에 따라 비무첩의 작성을 시작한다.

	u_short				usWaitTime;			// 대기 시간

	__matchMemberInfo	MemberInfo[2];		// 선봉 (또는 참가자) 두 캐릭터의 정보
};

// Mode : en_match_ready_update_waittime,					// 11 대기시간 업데이트
// Mode : en_match_ready_update_recordtype,					// 12 공식/비공식 변경
// Mode : en_match_ready_update_matchtype,					// 13 비무 형태 변경 : 집단전에서만
// Mode : en_match_ready_update_matchtime,					// 14 비무 시간 변경 : 2분부터 시작
// Mode : en_match_ready_update_disable_hyperrun,			// 15 경공 사용 유무 변경
// Mode : en_match_ready_update_keep_buff,					// 16 무공 효과의 유지 여부 변경
// Mode : en_match_ready_update_balance,					// 17 비무 균형 조절
// Mode : en_match_ready_update_after_draw,					// 23 집단전 무승부 일 경우, 다음 방식 결정 0종료/1선봉 재대결
struct MSG_MATCH_READY_UPDATE : public MSG_MATCH_READY
{
	u_short				usValue;

	/*
		waittime			-> 0 : 대기시간 남은 초 0xffff 알수 없음 (너무 오래 걸릴경우)
		recordtype			-> 0 : 공식0/비공식1
		matchtype			-> 개인1 집단10 : 집단전에서만 의미 : 아래의 비무 형태
		matchtime			-> 개인2분 집단 3분
		disable_hyperrun	-> 0: 경공사용0/미사용1
		keep_buff			-> 0 : 현상태유지0/초기화 1 -> 초기화는 실제로 들어갈 때 이루어 짐
		balance				->50 : 10-90까지 10-신청자에게 불리 90-피신청자에게 불리
	*/
};


#define _MAX_COUNT_OF_MATCH_ITEM_			6

// Mode : en_match_ready_req_add_item
struct MSG_MATCH_READY_ITEM_ADD_REQ : public MSG_MATCH_READY
{
	u_char				ucCount;

	_sitem_pair			AddItems[_MAX_COUNT_OF_MATCH_ITEM_];
};

// Mode : en_match_ready_req_del_item
struct MSG_MATCH_READY_ITEM_DEL_REQ : public MSG_MATCH_READY
{
	u_char				ucCount;

	_sitem_pair			AddItems[_MAX_COUNT_OF_MATCH_ITEM_];	// 슬롯번호는 비무첩에서의 슬롯
};

// Mode : en_match_ready_add_item
struct MSG_MATCH_READY_ITEM_ADD_ITEM : public MSG_MATCH_READY
{
	u_char				ucCount;
	_sUser_Inven		AddItems[_MAX_COUNT_OF_MATCH_ITEM_];	// 슬롯번호는 비무첩에서의 슬롯
};

// Mode : en_match_ready_del_item
typedef MSG_MATCH_READY_ITEM_DEL_REQ	MSG_MATCH_READY_DEL_ITEM;

// Mode : en_match_ready_set_my_item
struct MSG_MATCH_READY_SET_MY_ITEM : public MSG_MATCH_READY
{
	u_char				ucCount;
	_sUser_Inven		SetItems[_MAX_COUNT_OF_MATCH_ITEM_];	// 슬롯 번호는 인벤토리 슬롯
};

// Mode : en_match_ready_req_select_member
struct MSG_MATCH_READY_REQ_SELECT_MEMBER : public MSG_MATCH_READY
{
	u_char				ucNumber;			// 순번
	char				cName[13];			// 수정할 캐릭터 이름
};

// Mode : en_match_ready_select_member
struct MSG_MATCH_READY_SELECT_MEMBER : public MSG_MATCH_READY
{
	__matchMemberInfo	info;
};

// Mode : en_match_ready_deselect_memeber
struct MSG_MATCH_READY_DESELECT_MEMBER : public MSG_MATCH_READY
{
	u_char				ucNumber;			// 삭제할 순번
};

struct MSG_MATCH_READY_REQ_FREE_MATCH : public MSG_MATCH_READY
{
	u_int				uiNeedMoney;		// 100
};

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// 유저의 비무 상태
enum
{
	en_matchstate_none = 0,					// 0 비무 안하고 있음
	en_matchstate_making_table,				// 1 비무첩 작성중
	en_matchstate_waiting,					// 2 비무를 기다리고 있음
	en_matchstate_matching,					// 3 비무중에 있음
	en_matchstate_end,						// 4 비무가 끝나고 대기중
	en_matchstate_result,					// 5 결과가 갔고 10초간 애니매이션 중
	en_matchstate_wait_exit,				// 6 나갈수 있는 상태이고 10초후면 강제로 나가게 된다
	en_matchstate_just_end					// 7 비무가 끝났을때 Notice 패킷으로 한번만 옴
};


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// 비무 형태
enum
{
	en_matchtype_none = 0,					// 0
	en_matchtype_pvp,						// 1 1vs1 비무
	en_matchtype_free_match,				// 2 자유 비무
	en_matchtype_group_deathmatch = 10,		// 10 집단전 - 전멸전
	en_matchtype_group_chief,				// 11 집단전 - 행수전
	en_matchtype_group_serial,				// 12 집단전 - 연승전
	en_matchtype_group_each,				// 13 집단전 - 단승전
	en_matchtype_race_personal = 20,		// 20 비무대회 - 개인전
	en_matchtype_race_group					// 21 비무대회 - 집단전
};


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// MSG_MATCH_CTRL : 실제 비무의 시작과 자리잡기, 중간 시작, 결과 산출 등등

struct MSG_MATCH_CTRL
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucMode;
};

enum
{
	en_match_ctrl_start = 0,				// 0 : 비무 시작
	en_match_ctrl_wait,						// 1 : 비무장에 자리 없음. 기다리기
	en_match_ctrl_start_count,				// 2 : 카운트 시작하라.
	en_match_ctrl_start_fight,				// 3 : 실제 전투 시작.
	en_match_ctrl_before_5_sec,				// 4 : 5초 남았음

	en_freematch_ctrl_enter = 40,			// 40 : 자유비무장 입장 : 요청에 의한 결과
	en_freematch_ctrl_req_into_field,		// 41 : 자유비무장 전투지역으로 입장 요청
	en_freematch_ctrl_into_field,			// 42 : 전투지역으로 입장
	en_freematch_ctrl_req_safezone,			// 43 : 안전지대로 가기 요청
	en_freematch_ctrl_go_safezone,			// 44 : 안전지대로 감
	en_freematch_ctrl_req_exit,				// 45 : 비무장 떠나기 요청
	en_freematch_ctrl_exit,					// 46 : 비무장 떠남
	en_freematch_ctrl_not_matching,			// 47 : 자유비무 상태가 아니므로 요청을 받아들일 수 없음

	en_match_ctrl_end = 100,				// 100 : 비무 종료 결과가 포함
	en_match_ctrl_update_record_each,		// 101 : 전적 업데이트
	en_match_ctrl_request_exit,				// 102 : 나가기 요청
	en_match_ctrl_cannot_exit,				// 103 : 아직 나갈 수 없다
	en_match_ctrl_exit,						// 104 : 나가기 - 자기 위치 포함

	en_match_ctrl_system_error = 200,		// 200 : 비무 시스템 에러
};

struct MSG_MATCH_CTRL_START : public MSG_MATCH_CTRL
{
	u_char				ucStartWith;		// 0:비무시작위치 1:비무대기위치
	float				fPosX;
	float				fPosZ;
	short				sMatchFieldIndex;	// 비무장 인덱스
};

struct MSG_MATCH_CTRL_WAIT : public MSG_MATCH_CTRL
{
	u_short				usLeftWaitTime;
};


struct MSG_MATCH_CTRL_END : public MSG_MATCH_CTRL
{
	u_char				ucWinTeam;			// 0:신청자, 1:피신청자, 2:무승부
	
	// 이거 받으면, 승자는 승리 애니메이션 취하고, 패자는 패자 애니메이션.
	// 10초 동안은, 나갈수 없으며, 
	// 다음 10초동안은 esc 로 나갈 수 있고, 
	// 타임오버되면, 서버에서 끝낸다.
};

// en_match_ctrl_update_record_each,	// 101 : 전적 업데이트 패킷을 각자에게 보냄.
struct MSG_MATCH_CTRL_UPDATE_RECORD : public MSG_MATCH_CTRL 
{
	// 전적 업데이트
};

// en_match_ctrl_request_exit,	102 : 나가기 요청
struct MSG_MATCH_CTRL_REQUEST_EXIT : public MSG_MATCH_CTRL
{
	// 내용 없슴.
};


// en_match_ctrl_cannot_exit,			// 103 : 아직 나갈 수 없다. 
struct MSG_MATCH_CTRL_CANNOT_EXIT : public MSG_MATCH_CTRL
{
	// 내용 없음.
};


// en_match_ctrl_exit,					// 104 : 나가기 - 자신의 위치 포함.
struct MSG_MATCH_CTRL_EXIT : public MSG_MATCH_CTRL
{
	// 위치 이동이 주 목적이다. 
	float			fPosX ;
	float			fPosZ ;
	
};

struct MSG_MATCH_CTRL_SYSTEM_ERROR : public MSG_MATCH_CTRL
{
	u_char				ucReason ;	
	/*	Reason Code :
	0 : 그냥 에러.
	*/
};

struct MSG_MATCH_CTRL_FREE_ENTER : public MSG_MATCH_CTRL
{
	float				fPosX;
	float				fPosZ;

	u_int				uiSetMoney;		// 비무장 비용을 치른 후 남은 돈
};

struct MSG_MATCH_CTRL_FREE_INTO_FIELD : public MSG_MATCH_CTRL
{
	float				fPosX;
	float				fPosZ;
};

struct MSG_MATCH_CTRL_FREE_GO_SAFEZONE : public MSG_MATCH_CTRL
{
	float				fPosX;
	float				fPosZ;
};

struct MSG_MATCH_CTRL_FREE_EXIT : public MSG_MATCH_CTRL
{
	float				fPosX;
	float				fPosZ;
};


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// MSG_MATCH_NOTICE : 비무 홍보와 주위에 시작 및 결과를 알리는 패킷
struct MSG_MATCH_NOTICE 
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucMode;
};

enum
{
	en_match_ntc_inform = 0,			// 비무 홍보 패킷
	en_match_ntc_sb_info,				// 1 전광판 소 (개인/연승/단승)
	en_match_ntc_lb_info,				// 2 전광판 대 (전멸/행수)
	en_match_ntc_sb_update_vital,		// 3 전광판 소 (개인/연승/단승)
	en_match_ntc_lb_update_vital,		// 4 전광판 대 (전멸/행수)
	en_match_ntc_inform_end				// 비무 홍보 끝
	
};

struct MSG_MATCH_NOTICE_INFORM : MSG_MATCH_NOTICE
{
	u_short				usUniqueID;		// 유니크 아이디 : Length = 20 + strlen(cInform) 
	u_char				ucLevel;		// 레벨
	char				cName[13];
	char				cInform[81];	// 알림글 : 끝의 '\0' 포함한 글자수로 잘라주세요.
};

struct MSG_MATCH_NOTICE_INFORM_END : public MSG_MATCH_NOTICE
{
	u_short				usUniqueID ;
};

struct __matchRecord
{
	u_char				ucMember0 : 4;
	u_char				ucMember1 : 4;
};


// 비무 전광판은, 비무자의 영역인덱스를 사용하여 표현한다.
struct MSG_MATCH_NOTICE_SB_INFO : public MSG_MATCH_NOTICE 
{
	u_short				usMatchRegionIndex;		// 비무자의 영역 인덱스

	u_char				ucMatchType;
	u_char				ucMatchState;

	char				cChiefName0[13];
	char				cChiefName1[13];

	__matchMemberInfo	Player[2];				// 현재 비무중인 2사람

	u_char				ucMemberCount;			// 참여 인원 : 집단일 때만

	// 0 ~ 15 : 16가지
	// 0-빈공간, 1-승리, 2-패배, 3-무승부
	// 4-비무중, 5-비무전,6-비무중혼수상태
	__matchRecord		RecordList[9];
};

struct MSG_MATCH_NOTICE_LB_INFO : public MSG_MATCH_NOTICE
{
	u_short				usMatchRegionIndex;

	u_char				ucMatchType;

	char				cChiefName0[13];
	char				cChiefName1[13];

	__matchMemberInfo	Player[2];

	u_char				ucMemberCount;

	// 0 ~ 15 : 16가지
	// 0-빈공간, 1-승리, 2-패배, 3-무승부
	// 4-비무중, 5-비무전,6-비무중혼수상태
	__matchRecord		RecordList[9];
};

struct MSG_MATCH_NOTICE_SB_UPDATE_VITAL : public MSG_MATCH_NOTICE
{
	u_short				usMatchRegionIndex;

	u_char				ucVitalRate0;
	u_char				ucVitalRate1;
};

struct MSG_MATCH_NOTICE_LB_UPDATE_VITAL : public MSG_MATCH_NOTICE
{
	u_short				usMatchRegionIndex;

	u_char				ucVitalRate0;
	u_char				ucVitalRate1;
};


//#define MSG_NO_FREE_MATCH		179

struct MSG_FREE_MATCH
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucFMMode;
};


enum EN_GS_FREE_MATCH_PACKET
{
	en_gsfm_ctrl = 0,			// 0 각 단계의 시작을 알린다
	en_gsfm_notice,				// 1 공지사항
	en_gsfm_notice_winner,		// 2 우승자 정보 공지
	en_gsfm_notice_nowinner,	// 3 무승부 정보 공지
	en_gsfm_to_winner,			// 4 승리자에게만 가는 메시지
	en_gsfm_show_result,		// 5 결과 보내기

	en_gsfm_apply_match = 20,	// 20 npc에게 비무대회 입장 요청하기
	en_gsfm_apply_success,		// 21 결과:성공
	en_gsfm_apply_fail,			// 22 결과:실패
	en_gsfm_enter_match,		// 23 비무 대회 입장하라 (위치)
	en_gsfm_req_leave,			// 24 나가기 요청
	en_gsfm_leave_match,		// 25 req_leave의 성공 응답 (위치 포함)
	en_gsfm_leave_fail,			// 26 req_leave의 실패 응답
	en_gsfm_reward_nick,		// 27 별호 보상
	en_gsfm_reward_item,		// 28 아이템 보상
	en_gsfm_set_nick,			// 29 비무대회 별호세팅

	en_gsfm_req_camera = 40,	// 40 관전 포인트로의 접속을 요청
	en_gsfm_goto_camera,		// 41 관전 포인트로 가라 (포인트 인덱스 포함)
	en_gsfm_camera_fail,		// 42 관전 포인트 관련 요청이 실패했다
	en_gsfm_req_camera_move,	// 43 다음 관전 포인트로의 이동

	en_gsfm_current_state = 60,	// 60 비무대회가 진행 중일 경우, 현재 상태를 나타낸다.
	en_gsfm_private_score,		// 61 현재 비무대회에서의 개인 기록을 보낸다.
	en_gsfm_next_match_schedule,// 62 다음 비무대회의 일정을 요청한다 ==> 응답으로 MSG_FM_CURRENT_STATE 을 쓴다
	en_gsfm_req_score,			// 63 현재의 전적을 요청한다.

};

struct __fm_info_in_packet		// 비무 대회 정보
{
	u_char			match_type;		// 비무대회 타입 인덱스(스크립트에 정의)
	
	u_short			field_index;	// 대회장 영역

	short			fm_level;		// 같은 값이면, 특정 레벨만을 위한 비무 대회
	short			to_level;

	u_short			reward_index;	// 보상 정보

	_sND_TimeUnit	start_time;		// 시작 시간 정보
	_sND_TimeUnit	end_time;		// 끝나는 시간 정보
};

// en_gsfm_ctrl
struct MSG_FM_CTRL : public MSG_FREE_MATCH
{
	u_char			ucFM_State;			// 현재 단계
	u_int			uiElapsedSecond;	// 준비/시작/종료 이후 몇초가 지났는지
	
	__fm_info_in_packet	info;
};

// en_gsfm_notice
struct MSG_FM_NOTICE : public MSG_FREE_MATCH
{
	u_short			usTextId;
	u_short			usTextArg;			// 텍스트에 들어가는 인자, 0이면 무시
};

// en_gsfm_notice_winner
struct MSG_FM_NOTICE_WINNER : public MSG_FREE_MATCH
{
	u_short			usTextId;

	char			cWinnerName[13];
};

// en_gsfm_notice_nowinner
struct MSG_FM_NOTICE_NOWINNER : public MSG_FREE_MATCH
{
	u_short			usTextId;
};

// en_gsfm_to_winner
struct MSG_FM_TO_WINNER : public MSG_FREE_MATCH
{
	u_short			usTextId;
};

// en_gsfm_show_result
struct MSG_FM_SHOW_RESULT : public MSG_FREE_MATCH
{
};

// en_gsfm_apply_match
struct MSG_FM_APPLY_MATCH : public MSG_FREE_MATCH
{
};

// en_gsfm_apply_success
struct MSG_FM_APPLY_SUCCESS : public MSG_FREE_MATCH
{
};

enum
{
	_fm_apply_fail_not_time = 0,	// 비무 시간이 아니다
	_fm_apply_fail_party,			// 동행 중이므로 비무에 참가할 수 없다
	_fm_apply_fail_level,			// 레벨이 틀리다
	_fm_apply_fail_not_map,			// 합비가 아님
	_fm_apply_fail_already_in_filed,// 이미 비무장안에 있다
	_fm_apply_fail_system,			// 시스템 에러
};

// en_gsfm_apply_fail
struct MSG_FM_APPLY_FAIL : public MSG_FREE_MATCH
{
	u_char			ucFail;
};

// en_gsfm_enter_match
struct MSG_FM_ENTER_MATCH : public MSG_FREE_MATCH
{
	FLOAT			fx;
	FLOAT			fz;
};

// en_gsfm_req_leave
struct MSG_FM_REQ_LEAVE : public MSG_FREE_MATCH
{
	u_char			ucReq;			// 0:나가기 요청 : 그냥 0으로 채운다
};

// en_gsfm_leave_match
struct MSG_FM_LEAVE_MATCH : public MSG_FREE_MATCH
{
	FLOAT			fx;
	FLOAT			fz;
};

// en_gsfm_leave_fail
struct MSG_FM_LEAVE_FAIL : public MSG_FREE_MATCH
{
	u_char			ucFail;			// 0 : 나갈수 없습니다.
};

//en_gsfm_reward_nick
struct MSG_FM_REWARD_NICK : public MSG_FREE_MATCH
{
    u_short         usNickID;
};

//en_gsfm_reward_item
struct MSG_FM_REWARD_ITEM : public MSG_FREE_MATCH
{
	char			cItemType;
	short			sItemID;
};

//en_gsfm_set_nick
struct MSG_FM_SET_NICK : public MSG_FREE_MATCH
{
	u_short			usNick;			// 0:리셋, 나머지는 셋
	u_short			usFMRank;
};

//en_gsfm_req_camera=40
struct MSG_FM_REQ_CAMERA : public MSG_FREE_MATCH
{
};

//en_gsfm_goto_camera
struct MSG_FM_GOTO_CAMERA : public MSG_FREE_MATCH
{
	u_char			ucCameraPos;	// 0 번으로 시작.
};

//en_gsfm_camera_fail
struct MSG_FM_CAMERA_FAIL : public MSG_FREE_MATCH
{
	u_char			ucFail;			// 0 - 빈 자리가 없습니다.
};

//en_gsfm_req_camera_move,
struct MSG_FM_REQ_CAMERA_MOVE : public MSG_FREE_MATCH
{
	u_char			ucCameraPos;	// 그냥 0번으로 보내면, 서버에서 알아서 보냄.
};

//en_gsfm_current_state=60
struct MSG_FM_CURRENT_STATE : public MSG_FREE_MATCH
{
	__fm_info_in_packet	fm_info;		// 자유비무의 정보.

	u_char				ucFMCurState;	// FREE_MATCH_STEP 
	u_int				usElapsedTime;	// 각 단계 이후, 몇 초가 지났는가.

	/*

	enum FREE_MATCH_STEP	// event 로도 쓴다.
	{
		en_fm_step_none=0,
		en_fm_step_ready,
		en_fm_step_start,
		en_fm_step_end
	}; 

	*/
};

struct __fm_private_record
{
	u_short		usKillCount;
	u_short		usDeadCount;
};

//en_gsfm_private_score
struct MSG_FM_PRIVATE_SCORE : public MSG_FREE_MATCH
{
	__fm_private_record	my_record;		// 점수는 특정식에 의해 구해진다.
};

//en_gsfm_req_score
struct MSG_FM_REQ_SCORE : public MSG_FREE_MATCH
{
	// 내용 없음
};

//en_gsfm_next_match_schedule
struct MSG_FM_NEXT_MATCH_SCHEDULE : public MSG_FREE_MATCH
{
	// 내용 없음
};