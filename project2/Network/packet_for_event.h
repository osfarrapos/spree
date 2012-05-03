#pragma once

struct _sHeader ;

/* =====================================================================================
   이벤트 관련 패킷 : 요청과 응답(응답은 있을수도, 없을 수도 있다)
===================================================================================== */

#define MSG_NO_EVENT_REQUEST      253	// 이벤트 관련 요청 - 이벤트 종류에 따라, 다른 패킷이 사용될 것이다.( 상속으로 이뤄진다. )
#define MSG_NO_EVENT_RESULT       MSG_NO_EVENT_REQUEST // 이벤트 결과( 결과가 필요하다면 )


#define MSG_NO_NATIONAL_DIST	254		//국가별로 다른 의미로 사용되는 패킷. 모드로 구분한다


struct MSG_EVENT_REQ
{
    _sHeader header ;
	u_char   ucMessage ;
    u_short  usEventID ;        // 이값을 정의하고, 값에 따라 상속이 다름.
};

// 다음은 이벤트 아이디 값입니다. 이벤트가 중복될 경우, 구분하기 위해 쓰입니다.
enum EN_EVENT_ID
{
    en_event_ministop_200508_1=1,								// 1 행운의 SMS 이벤트  : 2005년 8월 이벤트.
		en_event_familymart_200511=2,							// 2 패밀리 마트 이벤트 : 2005년 11월 진행	
		en_event_sms_quiz_200511=3,								// 3 퀴즈 맞추	
}; 

// 1. 미니스톱 이벤트.
struct MSG_MINISTOP_EVENT_REQ : public MSG_EVENT_REQ
{
	char cKey[16] ;                       // 15자리 키값을 사용한다.
};

#define _EVENT_FAMILYMART_2_KEY_LENGTH_            9            // 8글자 + NULL(1)
struct MSG_FAMILYMART_2_EVENT_REQ : public MSG_EVENT_REQ

{
	char cKey[_EVENT_FAMILYMART_2_KEY_LENGTH_] ;     // 8자리 키값.마지막 바이트는 항상 0 이어야 함.
} ;


#define _EVENT_KEY_MAX_LENGTH_            32         // 31글자 + NULL(1)
struct MSG_EVENT_REQ_WITH_KEY : public MSG_EVENT_REQ
{
	char cKey[_EVENT_KEY_MAX_LENGTH_] ;
} ;

enum EN_ENENT_RESULT
{
	en_event_result_one_event = 0,
	en_event_result_notice,
	en_event_result_invite_friend,		// 미국 : 친구 초대 이벤트의 경공 추가
};

// 실패시의 응답입니다.
struct MSG_EVENT_RESULT                 // 오직 실패만 기록한다.
{
    _sHeader  header ;
    u_char    ucMessage ;               // MSG_NO_EVENT_RESULT
	
	#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 리뉴얼 아이콘	
	u_char ucEventResultMode;			//0 - 일반 : 단일 이벤트의 결과 1 - 노티스 : Type 2 이벤트들의 실행 여부
	#else
	u_short   usEventID ;
	u_char    ucResult ;                // Find EN_EVENT_ERRNO

	u_char		ucDetailInfo;			// ucResult 값이 en_event_errno_no_item_slot 일때, 
										// 필요 슬롯의 개수를 의미한다.
	#endif
};

// mode 0 == en_event_result_one_event
struct MSG_ONE_EVENT_RESULT : public MSG_EVENT_RESULT
{
	u_short	usEventID;
	u_char	ucResult;	// Find EN_EVENT_ERRNO

	u_char	ucDetailInfo;	// ucResult 값이 en_event_errno_no_item_slot 일때, // 필요 슬롯의 개수를 의미한다.
};

// mode 1 == en_event_result_notice
struct MSG_PRE_NOTICE_EVENT_RESULT : public MSG_EVENT_RESULT
{
	struct 
	{
		u_short	usEventID;
		u_char	ucResult;	// Find EN_EVENT_ERRNO
	}EventResult[8];
};

// en_event_result_invite_friend
struct MSG_INVITE_FRIEND_RESULT : public MSG_EVENT_RESULT
{
	short			sAddHyperRunID;		// 추가되는 경공 ID
	_sND_TimeUnit	EndTime;			// 경공이 사라지는 시간
};

// 이벤트 결과 및 에러 코드.
enum EN_EVENT_ERRNO
{
	en_event_success = 0,
	en_event_errno_blank,			// 꽝입니다. 다음에...
	en_event_errno_no_key,			// 잘못된 키값.
	en_event_errno_wait,			// 잠시후에 다시 시도해주세요.
	en_event_errno_event_end,		// 이미 끝난 이벤트 입니다.
	en_event_errno_no_event,		// 진행중인 이벤트가 아닙니다.
	en_event_errno_used,			// 이미 사용한 키값 입니다.
	en_event_errno_no_item_slot,	// 인벤토리가 꽉찼음
	en_event_errno_no_charac,		// 유저가 팅겼다.	: 로그를 위해서만 쓰인다.
	en_event_errno_fail_insert_item,// 인증은 성공했지만, 아이템을 넣지 못했다.
	en_event_errno_no_premium_user, // 프리미엄 유저가 아닙니다.
	en_event_is_going_on=50,		// 이벤트가 진행중이다.( 신규유저 이벤트와 같이, 접속할 때 미리 알려준다. )

	
	en_event_errno_system=100,		// 패킷 길이등의 시스템 에러..
	en_event_errno_over_insert,		// 제한 입력횟수 초과
	
};


#define MSG_NO_US_AD_REWARD_REQ                       157
#define MSG_NO_US_AD_REWARD_ACK                       157 

enum US_AD_REWARD_TYPE 
{
    UART_UNKNOWN			= 0,
    UART_DEAD				= 0x0001,
	UART_MEDITATION			= 0x0002,
	UART_AFTER_GAME_LOADING = 0x0004,
	UART_GOLD				= 0x1000,
	UART_ITEM				= 0x2000
};



struct MSG_US_AD_REWARD_REQ 
{
	_sHeader				header;
	u_char					ucMessage;
    US_AD_REWARD_TYPE       Type;		// 동영상 종류에 따른 보상 타입
}; 

typedef struct	ADItemInfo
{									
	char                    cClass1;	// 대분류
	WORD					wClass3;    // 소분류
	UCHAR					ucQty;      // 수량
}_ADItemInfo;

struct MSG_US_AD_REWARD_ACK 
{
	_sHeader                header;
	u_char                  ucMessage;
	DWORD                   dwGold;     // 지급 금액
	UCHAR                   ucItemCnt;  // 아이템 개수 
	_ADItemInfo				aditeminfo;	// ucItemCnt 개수만큼 반복
};


//#define MSG_NO_NATIONAL_DIST 254      // 국가별로 다른 의미로 사용되는 패킷. 모드로 구분한다.

struct MSG_NATIONAL_DIST 
{
	_sHeader				header;
	u_char					ucMessage;

	u_char					ucMode;
};

enum EN_NATIONAL_DIST_MODE
{
	en_national_acl_see_ad = 0,		// acclaim : 광고를 다보았을때
	en_national_res_see_ad,

	en_national_vina_sds_ctrl = 20,	// vina : 셧다운 제도 컨트롤
	
	en_national_metel_apex_kill_user = 40,	// metel(_XTAIWANESE) : apex kill message
};

// en_nationla_vina_sds_ctrl
struct MSG_NDIST_VINA_SDS_CTRL : public MSG_NATIONAL_DIST
{
	// 0:stop request & result	
	// 1:continue request & result 
	// 2:request fail
	u_char					ucStop;

	short					sCurFatigue;		// On 일 때만 유효. 나머지는 무시. - 경과시간(분)
	u_char					ucElapsedSecond;	// On 일 때만 유효. 나머지는 무시. - 경과시간(초)
};

// en_national_metel_apex_kill_user
struct MSG_NDIST_METEL_APEX_KILL_USER : public MSG_NATIONAL_DIST
{
	u_char					ucError;			// 0 - default
};


#define MSG_NO_PRESENT	201	// 각종 이벤트 혹은, 레벨 달성 등의 선물을 알리는 패킷.

enum EN_EVENT_TYPE
{
	en_event_type_basic = 0,	// 기본 쿠폰 이벤트 
	en_event_type_no_key_event,	// 키 없이 광고글과 같이 처리
	en_event_type_pre_notice_no_key,	// 접속시 미리 이벤트 진행 상황을 알려주는 이벤트
	en_event_type_pre_notice,	// 아직은 2번과 같다.
	en_event_type_level_present	// 레벨별 선물
};

struct MSG_PRESENT
{
	_sHeader header;
	u_char ucMessage;
	u_char ucMode;	// 아래의 EN_PRESENT_MODE
	short sEventID;	// 이벤트 아이디.
};

enum EN_PRESENT_MODE
{
	en_present_mode_notice = 0,	// 어떤 선물이 있다 : 받을 선물이 있을 경우, 알려준다. 
	en_present_mode_req_present,	// c -> s : 서버에 선물을 요청한다. : 이벤트 타입도 들어간다.
	en_present_mode_recv_present,	// s -> c : 선물을 준다.
	
	en_present_mode_error = 255	// 에러 메시지 : req_present 의 응답으로 사용된다. 
};

//en_present_mode_notice=0
struct MSG_PRESENT_NOTICE : public MSG_PRESENT
{
	u_short usPresentPackageIndex;	// 선물 패키지 인덱스 : 필요 없으면 안써도 됨. 
	u_char ucNeedInvenSlotCount;	// 선물을 받기 위해 필요한 인벤토리 슬롯 수.
	int iInfo;	// 레벨별 선물인 경우, 레벨.
};

//          en_present_mode_req_present=1,                          // c -> s : 서버에 선물을 요청한다. : 이벤트 타입도 들어간다.
// event id 만 채워서 보내면 됨.
struct MSG_PRESENT_REQ_PRESENT : public MSG_PRESENT
{
	int iInfo;	// 아직 안쓰임.
};

//          en_present_mode_recv_present=2,                         // s -> c : 선물을 준다.
struct MSG_PRESENT_RECV_PRESENT : public MSG_PRESENT
{
	// 이 패킷은 선물 패키지의 내용에 때라, 여러 번 갈 수 있음.
	int iInfo;	// 레벨 달성 이벤트 일 때, 레벨. 
	
	_sUser_Inven_v1_all UserInven;	// 아이템 정보
};

//          en_present_mode_error=255,                                  // 에러 메시지 : req_present 의 응답으로 사용된다. 
struct MSG_PRESENT_ERROR : public MSG_PRESENT
{
	u_char ucErrorCode;	// 아래의 EN_PRESENT_ERROR 
	int iInfo;	// 레벨별 선물일 경우, 레벨
};

enum EN_PRESENT_ERROR 
{
	en_present_error_no_event = 0,	// 그런 이벤트 없다. 
	en_present_error_already_recv,	// 이미 받아간 선물.
	en_present_error_need_inven	// 인벤 공간이 부족. 다시 시도할 것.
};

 
