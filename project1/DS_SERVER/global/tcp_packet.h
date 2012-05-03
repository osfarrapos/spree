#pragma once

#include "Global_struct.h"
#include "..\Billing\Bill_Packet.h"

#pragma pack( push, enter_tcp_packet )
#pragma pack(1)

struct CMSG_PROTOTYPE {
	u_short				usLength ;
	u_char				cMessage;
};


#define MAX_CHAT_LENGTH			81

/*

0 ~ 179 까지만 쓰자..

*/

// DS 에서만 쓰는 가상패킷
#define CMSG_NO_DISCONNECT					255
struct CMSG_DISCONNECT						//# 연결이 끊겼을 때마지막 패킷 #
{
	unsigned short		sLength;
	u_char				cMessage ;	
} ;


/* =====================================================================================
로그인과 서버 이동 인증 : 0 - 7 까지 - (동기 메시지)
===================================================================================== */
#define CMSG_NO_GSCONNECT_GS_DS					2
#define CMSG_NO_GSCONNECT_DS_GS					CMSG_NO_GSCONNECT_GS_DS
#define CMSG_NO_ENTERSERVER_CHECK_REQ_GS_DS		3
#define CMSG_NO_ENTERSERVER_CHECK_RESULT_DS_GS	4

// billing packet
#define CMSG_NO_BILL_REQ						6		// 과금 관련  요청 메시지 
#define CMSG_NO_BILL_INFO						7		// 과금정보 알림

/* =====================================================================================
로비에서 - 캐릭터 생성/삭제/선택 관련 8 - 15
===================================================================================== */
#define CMSG_NO_CHARAC_SELECT_GS_DS				8		// 캐릭터 창에서 캐릭터를 선택했다.
#define CMSG_NO_CHARAC_CREATE_GS_DS				9		// 캐릭터 생성
#define CMSG_NO_CHARAC_DELETE_GS_DS				10		// 캐릭터 삭제
#define CMSG_NO_USER_CHARAC_LIST_DS_GS			11		// 캐릭터 리스트
#define CMSG_NO_CHARAC_RESULT_DS_GS				12		// 위의 3개의 패킷에 대한 응답으로 쓰인다.
// 자세한 건 패킷 정의부에 있다.
#define CMSG_NO_CHARAC_LIST_REQ_GS_DS			13		// 캐릭터 리스트를 재요청

/* =====================================================================================
로그아웃과 서버 이동 요청 : 16 - 23
===================================================================================== */
#define CMSG_NO_LOGOUT_GS_DS					16		// 단순 로그아웃
#define CMSG_NO_REQ_SV_MOVE_GS_DS				18		// 서버간 이동 요청
#define CMSG_NO_SV_MOVE_RESULT_DS_GS			19		// 결과
#define CMSG_NO_UNI_GROUP_LOBBY					20
#define	CMSG_NO_CHECK_DSGS_PACKET				21
#define CMSG_NO_ACCOUNT_INFO					22		// 계정 관련된 정보 패킷 : 0. 플레이 시간.


/* =====================================================================================
캐릭터 정보 관련 또는 로비로 빠져나기기  24 - 39
인벤토리/무공/퀘스트.. 등의 모든 정보 업데이트는 여기에서.
===================================================================================== */
#define CMSG_NO_CHARAC_ALL_INFO_DS_GS			24		// DS->GS ALL CHARAC INFO REQUEST MESSAGE
#define CMSG_NO_DEPOT_REQ_GS_DS					25		// GS->DS Depot data request..
#define CMSG_NO_DEPOT_RES_GS_DS					26		// DS->GS Depot data result..


#define CMSG_NO_CHARAC_RETURN_LOBBY_GS_DS		39		// 존에서 로비로 나감.

/* =====================================================================================
채팅 메시지 : 운영/공지 포함. 40 - 47
===================================================================================== */
#define CMSG_NO_ZONE_CHAT_WHISPER_GS_DS			40		// GS<>DS 귓속말

#define CMSG_NO_SAVE_FATIGUE_WEIGHT				(44)

/* =====================================================================================
유저 정보 업데이트 정보  48 ~
===================================================================================== */
#define CMSG_NO_UPDATE_CHARAC_ALL_				57			// 캐릭터의 모든 정보 업데이트
#define CMSG_NO_ALL_UPDATE_RESULT_				57		// all update 에 대한 결과

/* =====================================================================================
환경정보 업데이트  64 ~
===================================================================================== */
// 랭킹 별호와 관련하여 DS <-> GS 동기화를 유지하기 위한 패킷이다.
// DS 는 일정 주기로 DB에서 랭킹정보를 산출하고.. 그 정보를 유지하며.. GS의 요청에 따라
// 랭킹 정보를 응답해준다..
#define CMSG_NO_RANKING_INFO_REQ				64			// 랭킹 정보를 요구
#define CMSG_NO_RANKING_INFO_RES				65			// 랭킹 정보를 응답
#define CMSG_NO_RANKING_UPDATE					66			// 랭킹 정보를 업데이트 한다..(비무등..)


#define CMSG_NO_QUESTRANK_DS_REQ				67			// 퀘스트랭킹 정보를 요구
#define CMSG_NO_QUESTRANK_DS_RES				CMSG_NO_QUESTRANK_DS_REQ	// 퀘스트랭킹 정보를 응답
#define CMSG_NO_QUESTRANK_DS_UPDATE				68			// 퀘스트랭킹 정보를 업데이트

#define CMSG_NO_SHIFTSTATEFLAG					70			// 서버군 상태변경요청(and 대답 - 서버군의 모든 서버에 브로드캐스팅 해야함)
#define CMSG_NO_QUESTCOUNT_REQ					71			// 퀘스트 가운트 요청(업데이트 요청 포함)
#define CMSG_NO_QUESTCOUNT_RES					CMSG_NO_QUESTCOUNT_REQ	//퀘스트 카운트 응답

/* =====================================================================================
이벤트 관련 메세지. 90 ~ 99 번까지. 사용.
===================================================================================== */
#define CMSG_NO_EVENT_AUTH_REQ					90			// 이벤트 인증 요청 패킷 : 보답은 각 컨텐츠 패킷으로 대체함.
#define CMSG_NO_EVENT_RESULT					CMSG_NO_EVENT_AUTH_REQ


// 자유 비무 이벤트 패킷
#define CMSG_NO_FREE_MATCH						(91)
// GM 이벤트.
#define CMSG_NO_GM_EVENT						(92)		// GM 이벤트 패킷.


#define CMSG_NO_PKEVENT							98		// 흑백 대전 관련 패킷.


/*==========================================================================================]
[				DS <-> GS																	]
[===========================================================================================]
[	설명 :	UDP 메시지였던것들을 TCP 패킷 구조로 변경하였음 ( 2005-03-8 )					]
[											jkHong Add...	-								]
[==========================================================================================*/
// DS -> GS
#define		CMSG_NO_GSINFO_REQ_DS_GS			100	// GS에 서버 정보 요청
#define		CMSG_NO_GSINFO_RES_GS_DS			101	// 서버 정보 응답 *
#define		CMSG_NO_USER_LOGOUT_DS_GS			102	// 강제 로그아웃
#define		CMSG_SHOWTEXT_REQ_DS_GS				103	// GS에 공지사항 출력 요청
#define		CMSG_NO_CREATE_ITEM_GS_DS			105 // ITEM 생성
#define		CMSG_SYSTEM_OFF_REQ_NUM				110
#define		CMSG_N0_GS_ON_DS_GS					111	// 시스템 재시작 DS -> GS
#define		CMSG_N0_GS_OFF_DS_GS				112	// 시스템 종료 DS -> GS

#define		CMSG_NO_ALL_SPELL_REQ				(113)	// 백도/흑도 또는 전체에 해당 버프 적용. :GS->DS
#define		CMSG_NO_ALL_SPELL_APPLY				CMSG_NO_ALL_SPELL_REQ	// 113 결과로 DS -> GS 에 보내는 패킷. 받으면 적용.
#define		CMSG_NO_QUEST_REWARD_NOTICE			(114)	// 퀘스트를 깼을 경우, 보상을 전 서버에 알릴 때.
#define		CMSG_NO_ALL_SYSTEM_MSG				(115)	// GS->DS 로 전서버에 메세지 출력.

#define		CMSG_NO_LTS							(116)	// 위치 추적.

//////////////////////////////////////////////////////////////////////////
// 친구 소환 : 20070604 추가.
//////////////////////////////////////////////////////////////////////////
#define		CMSG_NO_RECALL_FRIEND				(117)

#define		CMSG_NO_SETPOSITION_REQ_DS_GS		120	// 캐릭터 위치 초기화 요청

/*
#define MSG_NO_ITEM_SHOP						131
#define CMSG_NO_ITEM_SHOP						MSG_NO_ITEM_SHOP

(131)번은 위와 같이 사용되고 있음.
(139)번도 위와 같이 사용되고 있음.
ProjectRoot/ItemShop/ItemShop_Packet.h 에 정의되었음
*/

#define CMSG_NO_COMMOM_REQ						(135)
#define CMSG_NO_COMMON_ACK						(135)
#define CMSG_NO_COMMON_NTF						(135)

#define		CMSG_NO_CONSOLE_COMMAND				(139)


//////////////////////////////////////////////////////////////////////////
//	135번 윤군이 [범용패킷]으로 사용합니다.
//	빈 번호 찾기 어렵..
//////////////////////////////////////////////////////////////////////////



//  DS -> GS  (GMS command)  140~
#define		CMSG_GM_EDIT_COLLECTION_NUM		140		// 수집(명품) 데이터 에디트
#define		CMSG_GM_EDIT_EXPRATE_NUM		141		// 레벨 배율 조정

#define		CMSG_GM_EDIT_VAR_NO				142		// 에디트 대표 번호.
#define		CMSG_GM_EDIT_ITEM_NUM			143		// 아이템 정보 수정
#define		CMSG_GM_EDIT_NICK_ID_NUM		144		// 별호 수정
#define		CMSG_GM_EDIT_INCHANTVALU_NUM	145		// 제련 수정

#define		CMSG_ENV_SETTING_NUM			(146)	// DS 에서 서버 환경을 조절할 때 쓰인다.


// 142 ~ 149 번 까지는 ray 가 모드를 나눠 사용합니다.  아래쪽에 있습니다.

#define		CMSG_GM_EDIT_LEVEL_NUM			150		//	레벨 에디트
#define		CMSG_GM_EDIT_VITAL_NUM			151		//	생 에디트
#define		CMSG_GM_EDIT_ZEN_NUM			152		//	진 에디트
#define		CMSG_GM_EDIT_SOUL_NUM			153		//	지 에디트
#define		CMSG_GM_EDIT_HEART_NUM			154		//	민 에디트
#define		CMSG_GM_EDIT_JUNGGI_NUM			155		//	근 에디트
#define		CMSG_GM_EDIT_LEFTPOINT_NUM		156		//	남은 포인트 에디트
#define		CMSG_GM_EDIT_ZONE_NUM			157		//	존 바꾸기
#define		CMSG_GM_EDIT_CODE_NUM			158		//	코드 바꾸기
#define		CMSG_GM_EDIT_PARTY_NUM			159		//	파티 바꾸기
#define		CMSG_GM_EDIT_NAME_NUM			160		//	이름 바꾸기(같은 이름이 존재하는지 조사는 이미 했음)
#define		CMSG_GM_EDIT_NICK_NUM			161		//	닉네임 바꾸기(같은 닉네임이 존재하는지 조사는 이미 했음)
#define		CMSG_GM_ADD_INVITEM_NUM			162		//	인벤토리에 아이템 추가
#define		CMSG_GM_REM_INVITEM_NUM			163		//	인벤토리에 아이템 제거
#define		CMSG_GM_ADD_DEPITEM_NUM			164		//	저장소에 아이템 추가
#define		CMSG_GM_REM_DEPITEM_NUM			165		//	저장소에 아이템 제거
#define		CMSG_GM_ADD_INVGOLD_NUM			166		//	인벤토리의 돈 가감
#define		CMSG_GM_ADD_DEPGOLD_NUM			167		//	저장소의 돈 가감
#define		CMSG_GM_ADD_SKILL_NUM			168		//	스킬 추가
#define		CMSG_GM_REM_SKILL_NUM			169		//	스킬 제거
#define		CMSG_GM_EDIT_SKILL_NUM			170		//	스킬 에디트(스킬 step변화)
#define		CMSG_GM_EDIT_CONTRIBUTE_NUM		171		//	문파 기여도 수정

#define		CMSG_GM_ADD_MASTERY_NUM			172		//	오의 추가
#define		CMSG_GM_REM_MASTERY_NUM			173		//	오의 삭제
#define		CMSG_GM_EDIT_MASTERY_NUM			174		//	오의 에디트(오의 포인트 변화)
#define		CMSG_GM_EDIT_LEFTMASTERYPOINT_NUM	175	//	남은 오의 포인트 에디트
#define		CMSG_GM_EDIT_CHARAC_ROLE_NUM		176		//	역할 바꾸기
#define		CMSG_GM_EDIT_CHARAC_CLASSGRADE_NUM	177	//	역할등급 바꾸기
#define		CMSG_GM_EDIT_CHARAC_MUTE_TIME_NUM	178		// 채팅금지시키기
#define		CMSG_GM_EDIT_JIN_NUM			179		// 진 수정
// 180 미만 


// 180 ~ 209 파티 메시지.





//#define CMSG_NO_UPDATE_CHARAC_ALL_				57			// 캐릭터의 모든 정보 업데이트
//#define CMSG_NO_ALL_UPDATE_RESULT_				57		// all update 에 대한 결과
struct MSG_UPDATE_CHARAC_ALL
{
	u_short				usLength;
	u_char				cMessage;

	char				cAccount[en_max_lil+1] ;			// 계정
	char				cCharacName[13] ;		// 캐릭이름

	_CHARAC				charac;					// 캐릭터 이름은 이 안에 포함됨.
	_USER_DEPOT			storage;				// 창고 정보..
	_ACCOUNT_ITEM_USE_STATUS	aius ;			// 

	_CHARAC_ETC_QUICKSLOT		quickslot;		// 

#ifdef _PD_CHARAC_LTS_
	_CHARAC_LTS			characLts;
#endif

	u_int				uiCheckFlag_Login : 1;			// Is Check Login
	u_int				uiCheckFlag : 31;				// Reserve

#ifdef _PD_EVENT_INVITE_FRIEND_
	int					iIF_DBIndex;
	int					iIF_EndTime;			//
#endif	// _PD_EVENT_INVITE_FRIEND_
};


struct CMSG_ALL_UPDATE_RESULT
{
	u_short				usLength;
	u_char				cMessage;

	char				cAccount[en_max_lil+1] ;			// 계정
	char				cCharacName[13] ;		// 캐릭이름

	char				cResult ;				// 0.성공, 1.실패	
};


//#define CMSG_NO_SHIFTSTATEFLAG					70			// 서버군 상태변경요청(and 대답 - 서버군의 모든 서버에 브로드캐스팅 해야함)
struct MSG_SHIFTSTATEFLAG
{
	u_short				usLength;
	u_char				cMessage;

	u_int				uiStateFlag;
};



//#define CMSG_NO_QUESTCOUNT_REQ					71			// 퀘스트 가운트 요청(업데이트 요청 포함)
//#define CMSG_NO_QUESTCOUNT_RES					CMSG_NO_QUESTCOUNT_REQ	//퀘스트 카운트 응답
struct MSG_QUESTCOUNT_REQ
{
	u_short				usLength;
	u_char				cMessage;

	u_short				usQuestID;
	u_char				ucQuestType: 4;	//카운트 타잎. 0: 시작, 1: 완료

	u_char				ucReqType: 4;	//0: 데이터요청, 1: 업데이트요청

	int					iCount;
};

//#define CMSG_NO_QUESTCOUNT_RES					CMSG_NO_QUESTCOUNT_REQ	//퀘스트 카운트 응답
struct MSG_QUESTCOUNT_RES
{
	u_short				usLength;
	u_char				cMessage;

	u_short				usQuestID;
	u_char				ucQuestType;	//카운트 타잎. 0: 시작, 1: 완료

	int					iCount;
};

/* =====================================================================================
이벤트 관련 메세지. 90 ~ 99 번까지. 사용.
===================================================================================== */
//#define CMSG_NO_EVENT_AUTH_REQ					90			// 이벤트 인증 요청 패킷 : 보답은 각 컨텐츠 패킷으로 대체함.
//#define CMSG_NO_EVENT_RESULT					CMSG_NO_EVENT_AUTH_REQ

#define _EVENT_KEY_MAX_LENGTH_			32	// 31글자까지만 허용.
struct CMSG_EVENT_AUTH_REQ
{
	u_short				usLength;
	u_char				cMessage ;

	u_char				ucMode;

	CMSG_EVENT_AUTH_REQ( u_short length, u_char mode )
		: usLength(length), cMessage(CMSG_NO_EVENT_AUTH_REQ), ucMode(mode)
	{
	}
};


enum EN_EVENT_AUTH_MODE
{
	en_event_auth_req_with_key = 0,	// 
	en_event_auth_cancel=1,
	en_event_auth_success=2,
	en_event_auth_check_req=3,

	// Level Preset 관련 모드
	en_event_auth_lp_getnotrecv_req=5,	// gs->ds : 최초 접속시에, 아직 안받아간 선물이 있는지 확인 요청한다.
	en_event_auth_lp_check_req=6,		// gs->ds : 레벨 업 시에, ds 알린다. DB 에 아직 안받아간 것으로 기록.
	en_event_auth_lp_recv_req=7,		// gs->ds : 클라이언트에서 '받기' 버튼 누르면, GS 에서 DS 에서 받을 수 있는지 확인 요청한다.
	en_event_auth_lp_cancel=8,			// gs->ds : 만약, 선물 주기에 실패하면, DS 에, 그 계정/레벨에 대하여, 받아간 것을 취소한다.

	// Invite Friend
	en_event_auth_friend_info_req=10,		// 친구 초대 : 경공 이벤트 정보 요청
	en_event_auth_friend_hyperrun_end=11,	// 친구 초대 : 경공 사용 시간이 만료됨
};


// mode 0 : check key and request
struct CMSG_EVENT_AUTH_REQ_WITH_KEY : public CMSG_EVENT_AUTH_REQ
{
	char				cNDAccount[en_max_lil+1];				// 유저 아이디  17   <----요거 바뀌었어요
	char				cCharacName[13] ;

	short				sEventID ;
	char				cKey[_EVENT_KEY_MAX_LENGTH_] ;

	CMSG_EVENT_AUTH_REQ_WITH_KEY() 
		: CMSG_EVENT_AUTH_REQ(sizeof(CMSG_EVENT_AUTH_REQ_WITH_KEY), en_event_auth_req_with_key)
	{
	}
};

// mode 1 : cancel key.,
struct CMSG_EVENT_CANCEL : public CMSG_EVENT_AUTH_REQ
{
	short				sEventID ;
	int					index;	

	CMSG_EVENT_CANCEL() 
		: CMSG_EVENT_AUTH_REQ(sizeof(CMSG_EVENT_CANCEL), en_event_auth_cancel)
	{
	}
};

// 2 : success
struct CMSG_EVENT_SUCCESS : public CMSG_EVENT_AUTH_REQ
{
	short				sEventID ;
	int					index;	

	CMSG_EVENT_SUCCESS() 
		: CMSG_EVENT_AUTH_REQ(sizeof(CMSG_EVENT_SUCCESS), en_event_auth_success)
	{
	}
};

#define _NDD_PRECHECK_EVENT_COUNT_				(24)

struct CMSG_EVENT_CHECK_REQ : public CMSG_EVENT_AUTH_REQ
{
	char				cNDAccount[en_max_lil+1];				// 유저 아이디  17   <----요거 바뀌었어요
	char				cCharacName[en_charac_name_length+1] ;

	short				curEventList[_NDD_PRECHECK_EVENT_COUNT_];		// 최대 8개까지만. 8개 넘어갈리가 없다.

	CMSG_EVENT_CHECK_REQ()
		: CMSG_EVENT_AUTH_REQ( sizeof(CMSG_EVENT_CHECK_REQ), en_event_auth_check_req )
	{
		ZeroMemory( cCharacName, sizeof(cCharacName) );
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
		ZeroMemory( curEventList, sizeof(curEventList) );
	}
};


// Level Preset 관련 모드
// en_event_auth_lp_getnotrecv_req=5,	// gs->ds : 최초 접속시에, 아직 안받아간 선물이 있는지 확인 요청한다.
struct CMSG_LP_GETNOTRECV_REQ : public CMSG_EVENT_AUTH_REQ
{
	char				cNDAccount[en_max_lil+1];				// 유저 아이디  17   <----요거 바뀌었어요

	// 계정만 있으면 됨.
	CMSG_LP_GETNOTRECV_REQ()
		: CMSG_EVENT_AUTH_REQ( sizeof(CMSG_LP_GETNOTRECV_REQ), en_event_auth_lp_getnotrecv_req )
	{
	}

	void SetUserInfo( const char * acc )
	{
		strncpy( cNDAccount, acc, en_max_lil);
		cNDAccount[en_max_lil] = 0;
	}
};

// en_event_auth_lp_check_req=6,		// gs->ds : 레벨 업 시에, ds 알린다. DB 에 아직 안받아간 것으로 기록.
struct CMSG_LP_CHECK_REQ : public CMSG_EVENT_AUTH_REQ
{
	short		sEventID;
	short		sLevel;

	char		cNDAccount[en_max_lil+1];				// 유저 아이디  17   <----요거 바뀌었어요
	char		cName[en_charac_name_length+1];

	CMSG_LP_CHECK_REQ()
		: CMSG_EVENT_AUTH_REQ( sizeof(CMSG_LP_CHECK_REQ), en_event_auth_lp_check_req )
	{
		sLevel = 0;
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
		ZeroMemory( cName, sizeof(cName) );
	}

	void SetUserInfo( const char * acc, const char * chr )
	{
		strncpy( cNDAccount, acc, en_max_lil);
		cNDAccount[en_max_lil] = 0;

		strncpy( cName, chr, en_charac_name_length );
		cName[en_charac_name_length] = 0;
	}
};


// en_event_auth_lp_recv_req=7,		// gs->ds : 클라이언트에서 '받기' 버튼 누르면, GS 에서 DS 에서 받을 수 있는지 확인 요청한다.
struct CMSG_LP_RECV_REQ : public CMSG_EVENT_AUTH_REQ
{
	short		sEventID;
	short		sLevel;

	char		cNDAccount[en_max_lil+1];				// 유저 아이디  17   <----요거 바뀌었어요
	char		cName[en_charac_name_length+1];

	CMSG_LP_RECV_REQ()
		: CMSG_EVENT_AUTH_REQ( sizeof(CMSG_LP_RECV_REQ), en_event_auth_lp_recv_req )
	{
		sLevel = 0;
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
		ZeroMemory( cName, sizeof(cName) );
	}

	void SetUserInfo( const char * acc, const char * chr )
	{
		strncpy( cNDAccount, acc, en_max_lil);
		cNDAccount[en_max_lil] = 0;

		strncpy( cName, chr, en_charac_name_length );
		cName[en_charac_name_length] = 0;
	}
};



// en_event_auth_lp_cancel=8,			// gs->ds : 만약, 선물 주기에 실패하면, DS 에, 그 계정/레벨에 대하여, 받아간 것을 취소한다.
struct CMSG_LP_CANCEL : public CMSG_EVENT_AUTH_REQ
{
	short		sEventID;
	short		sLevel;			// 해당레벨.

	char		cNDAccount[en_max_lil+1];				// 유저 아이디  17   <----요거 바뀌었어요

	CMSG_LP_CANCEL()
		: CMSG_EVENT_AUTH_REQ( sizeof(CMSG_LP_CANCEL), en_event_auth_lp_cancel )
	{
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
	}

	void SetUserInfo( const char * acc )
	{
		strncpy( cNDAccount, acc, en_max_lil);
		cNDAccount[en_max_lil] = 0;
	}
};

// Invite Friend
// en_event_auth_friend_info_req=10,		// 친구 초대 : 경공 이벤트 정보 요청
struct CMSG_FRIEND_INFO_REQ : public CMSG_EVENT_AUTH_REQ 
{
	short		sEventID;

	char		cNDAccount[en_max_lil+1];

	CMSG_FRIEND_INFO_REQ()
		: CMSG_EVENT_AUTH_REQ( sizeof(CMSG_FRIEND_INFO_REQ), en_event_auth_friend_info_req )
	{
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
	}

	void SetUserInfo( const char * acc )
	{
		if( acc )
		{
			strncpy( cNDAccount, acc, en_max_lil );
			cNDAccount[en_max_lil] = 0;
		}
	}
};

// en_event_auth_friend_hyperrun_end=11,	// 친구 초대 : 경공 사용 시간이 만료됨.
struct CMSG_FRIEND_HYPERRUN_END : public CMSG_EVENT_AUTH_REQ
{
	int			index;

	CMSG_FRIEND_HYPERRUN_END()
		: CMSG_EVENT_AUTH_REQ(sizeof(CMSG_FRIEND_HYPERRUN_END), en_event_auth_friend_hyperrun_end )
	{
	}
};


enum DB_RESULT_EVENT_CODE
{
	_db_result_not_found = 0,		// No key
	_db_result_used = 1,			// used
	_db_result_wait_a_moment = 2,	// wait
	_db_result_success = 3,			// success
	_db_result_not_checked = 4,		// not checked, 이벤트를 진행한 적이 없다.
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
	en_event_errno_not_premium,		// 프리미엄 유저가 아니다.

	en_event_is_going_on=50,		// 이벤트가 진행중이다.( 신규유저 이벤트와 같이, 접속할 때 미리 알려준다. )

	en_event_errno_system=100,		// 패킷 길이등의 시스템 에러.
	en_event_errno_over_insert,		// 제한 입력횟수 초과
} ;



struct CMSG_EVENT_RESULT		// 이벤트 실패 또는 에러시에 보낸다. 
{
	u_short				usLength;
	u_char				cMessage ;				// CMSG_NO_EVENT_AUTH_REQ

	char				cCharacName[13] ;		// 

	u_char				ucEventResultMode;		// 0 - One, 1 - PreNotice

	CMSG_EVENT_RESULT( u_short size, u_char mode )
		: usLength(size), cMessage(CMSG_NO_EVENT_AUTH_REQ), ucEventResultMode(mode)
	{
		ZeroMemory( cCharacName, sizeof(cCharacName) );
	}
};


enum EN_EVENT_RESULT_MODE
{
	en_dsevent_result_one_event=0,
	en_dsevent_result_pre_notice=1,

	// Level Present
	en_dsevent_result_lp_getnotrecv_ans=5,	// ds->gs : 응답
	en_dsevent_result_lp_check_ans=6,			// ds->gs : 응답.
	en_dsevent_result_lp_recv_ans=7,			// ds->gs : 응답

	// Invite Friend : Hyperrun 
	en_dsevent_result_friend_info=10,			// ds->gs : 
};


// mode 0
struct CMSG_ONE_EVENT_RESULT : public CMSG_EVENT_RESULT		// 이벤트 실패 또는 에러시에 보낸다. 
{
	short				sEventID;

	char				cItemType ;				// 아이템 타입	: package type = -1;
	short				sItemID ;				// 아이템 아이디
	u_char				ucItemCount ;			// 기본 1

	u_char				ucResult ;				//
	int					index;					// 

	CMSG_ONE_EVENT_RESULT()
		: CMSG_EVENT_RESULT( sizeof(CMSG_ONE_EVENT_RESULT), en_dsevent_result_one_event )
	{
	}
};


// mode 1
struct CMSG_PRE_NOTICE_EVENT_RESULT : public CMSG_EVENT_RESULT		// 이벤트 실패 또는 에러시에 보낸다. 
{
	char				cAccount[en_max_lil+1];
	struct 
	{
		short			sEventID;
		u_char			ucResult;
	} EventResult[_NDD_PRECHECK_EVENT_COUNT_];

	CMSG_PRE_NOTICE_EVENT_RESULT()
		: CMSG_EVENT_RESULT(sizeof(CMSG_PRE_NOTICE_EVENT_RESULT), en_dsevent_result_pre_notice )
	{
		ZeroMemory( cAccount, sizeof(cAccount) );
		ZeroMemory( EventResult, sizeof(EventResult) );
	}
};


// Level Present
// en_event_result_lp_getnotrecv_ans=5,	// ds->gs : 응답
struct CMSG_LP_GETNOTRECV_ANS : public CMSG_EVENT_RESULT
{
	short		sEventID;
	short		sLevel;			// -1 : 실패, >=0 있다.

	char		cNDAccount[en_max_lil+1];

	CMSG_LP_GETNOTRECV_ANS()
		: CMSG_EVENT_RESULT( sizeof(CMSG_LP_GETNOTRECV_ANS), en_dsevent_result_lp_getnotrecv_ans )
	{
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
	}

	void SetAccount( const char * acc )
	{
		strncpy( cNDAccount, acc, en_max_lil );
		cNDAccount[en_max_lil] = 0;
	}
};

// en_event_result_lp_check_ans=6,			// ds->gs : 응답.
struct CMSG_LP_CHECK_ANS : public CMSG_EVENT_RESULT
{
	short		sEventID;
	short		sLevel;			// -1 : fail. else OK

	char		cNDAccount[en_max_lil+1];

	CMSG_LP_CHECK_ANS()
		: CMSG_EVENT_RESULT( sizeof(CMSG_LP_CHECK_ANS), en_dsevent_result_lp_check_ans )
	{
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
	}

	void SetAccount( const char * acc )
	{
		strncpy( cNDAccount, acc, en_max_lil );
		cNDAccount[en_max_lil] = 0;
	}
};


// en_event_result_lp_recv_ans=7,			// ds->gs : 응답
struct CMSG_LP_RECV_ANS : public CMSG_EVENT_RESULT
{
	short		sEventID;
	short		sLevel;			// 입력 레벨

	u_char		ucResult;		// 결과 0 - fail, 1 - success.

	char		cNDAccount[en_max_lil+1];

	CMSG_LP_RECV_ANS()
		: CMSG_EVENT_RESULT( sizeof(CMSG_LP_RECV_ANS), en_dsevent_result_lp_recv_ans )
	{
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
	}

	void SetAccount( const char * acc )
	{
		strncpy( cNDAccount, acc, en_max_lil );
		cNDAccount[en_max_lil] = 0;
	}
};

// Invite Friend : Hyperrun 
//	en_event_result_friend_info=10,			// ds->gs : 
struct CMSG_FRIEND_INFO : public CMSG_EVENT_RESULT
{
	u_char			ucResult;		// 0 : no, 1 : ok.

	char			cNDAccount[en_max_lil+1];

	short			sEventID;

	int				iDB_Index;
	int				end_time;

	char			cFromWho[en_max_lil+1];

	CMSG_FRIEND_INFO()
		: CMSG_EVENT_RESULT( sizeof(CMSG_FRIEND_INFO), en_dsevent_result_friend_info )
	{
		ucResult = 0;
		end_time = 0;
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
		ZeroMemory( cFromWho, sizeof(cFromWho) );
	};

	void SetAccount( char * acc )
	{
		strncpy( cNDAccount, acc, en_max_lil );
		cNDAccount[en_max_lil] = 0;
	}
};


#ifdef _PD_GAME_EVENT_
//////////////////////////////////////////////////////////////////////////
// 자유 비무 : 
//////////////////////////////////////////////////////////////////////////
//#define CMSG_NO_FREE_MATCH						(91)	
struct CMSG_FREE_MATCH			// CMSG_FM_	으로 시작.
{
	u_short				usLength;				// 
	u_char				cMessage;				//

	u_char				ucFMWork;				// FREE_MATCH_WORK 를 그대로 쓴다.

	CMSG_FREE_MATCH( u_short length, u_char mode )
		: usLength( length ), cMessage( CMSG_NO_FREE_MATCH ), ucFMWork( mode )
	{
	}
};


enum FREE_MATCH_DS_PACKET
{
	en_dsfm_ready=0,				// ready 이벤트 처리.
	en_dsfm_start,					// start 이벤트 처리.
	en_dsfm_end,					// end 이벤트 처리
	en_dsfm_notice,					// 공지 사항
	en_dsfm_notice_winner,			// 공지 : 우승자 정보를 공지
	en_dsfm_notice_nowinner,		// 공지 : 무승부 공지
	en_dsfm_to_winner,				// 승자에게만 가능 메시지.

	en_dsfm_req_result,				// GS 에 결과를 요청한다.
	en_dsfm_result_info,			// req_result 의 응답으로 GS 에서 우승자 정보를 얻는다.

	en_dsfm_send_result,			// 결과를 클라이언트에 보내도록 한다.

	en_dsfm_reward_nick,			// 별호 보상 지급,
	en_dsfm_reward_item,			// 아이템 보상 지급
	en_dsfm_buff_all,				// 현 지역의 모든 유저.
	en_dsfm_clean,					// 완전 종료.

	en_dsfm_req_cur_matchinfo=50,	// 다음 매치 정보를 요청한다. : 초기에 한번 요청한다.
	en_dsfm_cur_matchinfo,			// 다음 매치 정보를 알려준다.
	en_dsfm_cur_nick_owner,			// 비무대회 별호의 최종 소유자 정보를 알려준다.
};


//en_dsfm_ready=0,				// ready 이벤트 처리.
struct CMSG_FM_READY : public CMSG_FREE_MATCH
{
	_sFreeMatchEvent		event_info;		

	time_t					event_t_time;

	CMSG_FM_READY()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_READY), en_dsfm_ready )
	{
	}
};


//en_dsfm_start,				// start 이벤트 처리.
struct CMSG_FM_START : public CMSG_FREE_MATCH
{
	// 내용 필요 없음.
	time_t					event_t_time;

	CMSG_FM_START()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_START), en_dsfm_start )
	{
	}
};


//en_dsfm_end,					// end 이벤트 처리
struct CMSG_FM_END : public CMSG_FREE_MATCH
{
	// 내용 필요 없음.
	time_t					event_t_time;

	CMSG_FM_END()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_END), en_dsfm_end )
	{
	}
};


//en_dsfm_notice,				// 공지 사항
struct CMSG_FM_NOTICE : public CMSG_FREE_MATCH
{
	// 공지 사항
	u_short					text_id;
	u_short					text_arg1;

	CMSG_FM_NOTICE()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_NOTICE), en_dsfm_notice )
	{
	}
};


//en_dsfm_notice_winner,		// 공지 : 우승자 정보를 공지
struct CMSG_FM_NOTICE_WINNER : public CMSG_FREE_MATCH
{
	// 
	u_short					text_id;

	char					cWinnerName[en_charac_name_length+1];

	CMSG_FM_NOTICE_WINNER()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_NOTICE_WINNER), en_dsfm_notice_winner )
	{
	}
};


//en_dsfm_notice_nowinner,		// 공지 : 무승부 공지
struct CMSG_FM_NOTICE_NOWINNER : public CMSG_FREE_MATCH
{
	u_short					text_id;

	CMSG_FM_NOTICE_NOWINNER()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_NOTICE_NOWINNER), en_dsfm_notice_nowinner )
	{
	}
};


//en_dsfm_to_winner,			// 승자에게만.
struct CMSG_FM_TO_WINNER : public CMSG_FREE_MATCH
{
	int						cuid;
	u_short					text_id;				// DS -> GS

	CMSG_FM_TO_WINNER()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_TO_WINNER), en_dsfm_to_winner )
	{
	}
};


//	en_dsfm_req_result,
struct CMSG_FM_REQ_RESULT : public CMSG_FREE_MATCH
{
	// GS 에 랭킹을 계산하라고 명령 내리는 패킷이다.

	CMSG_FM_REQ_RESULT()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_REQ_RESULT), en_dsfm_req_result )
	{
	}
};

//en_dsfm_result_info
struct CMSG_FM_RESULT_INFO : public CMSG_FREE_MATCH
{
	u_char					ucResult;

	_sFM_Record				winnerRecord;

	CMSG_FM_RESULT_INFO()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_RESULT_INFO), en_dsfm_result_info )
	{
	}
};

//en_dsfm_send_result,			// 결과를 보내준다.
struct CMSG_FM_SEND_RESULT : public CMSG_FREE_MATCH
{
	// 결과를 Client 에 보내라고 명령 내리는 패킷이다.
	u_char					ucResult;

	/*
	enum EN_FM_RESULT
	{
	en_fm_result_one_winner=0,	// One winner
	en_fm_result_more_winner,	// two or more winners.
	en_fm_result_none,		// no winner
	};
	*/

	_sFM_Record				winnerRecord;

	CMSG_FM_SEND_RESULT()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_SEND_RESULT), en_dsfm_send_result )
	{
	}
};


//en_dsfm_reward_nick,			// 별호 보상 지급,
struct CMSG_FM_REWARD_NICK : public CMSG_FREE_MATCH
{
	u_short					usRank;
	u_short					usNickID;

	int						iWinnerCUID;

	CMSG_FM_REWARD_NICK()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_REWARD_NICK), en_dsfm_reward_nick ), usNickID(0)
	{
	}
};


//en_dsfm_reward_item,
struct CMSG_FM_REWARD_ITEM : public CMSG_FREE_MATCH
{
	struct {
		u_char					ucToWhom;		// en_ge_reward_item, 
												// en_ge_reward_item_to_male,en_ge_reward_item_to_female
		char					cItemType;
		short					sItemID;
		u_char					ucCount;
	} Item[8] ;
	

	CMSG_FM_REWARD_ITEM()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_REWARD_ITEM), en_dsfm_reward_item )
	{
		ZeroMemory( Item, sizeof(Item) );
	}
};


//en_dsfm_buff_all,			// 현 지역의 모든 유저.
struct CMSG_FM_BUFF_ALL : public CMSG_FREE_MATCH
{
	short					sGameServerIndex;			// 17번으로 고정일 것이다.

	short					sBuffIndex;					// 
	u_short					usBuffTime;					// 

	u_char					ucBuffStep;					// 아마 1일 것이다.

	CMSG_FM_BUFF_ALL()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_BUFF_ALL), en_dsfm_buff_all )
	{
	}
};


//en_dsfm_clean,				// 완전 종료.
struct CMSG_FM_CLEAN : public CMSG_FREE_MATCH
{
	// 마무리.
	_sFreeMatchEvent		event_info;		

	CMSG_FM_CLEAN()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_CLEAN), en_dsfm_clean )
	{
	}
};


//en_dsfm_req_cur_matchinfo=50,	// 다음 매치 정보를 요청한다. : 초기에 한번 요청한다.
struct CMSG_FM_REQ_CUR_MATCHINFO : public CMSG_FREE_MATCH
{

	CMSG_FM_REQ_CUR_MATCHINFO()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_REQ_CUR_MATCHINFO), en_dsfm_req_cur_matchinfo )
	{
	}
};


//en_dsfm_cur_matchinfo,			// 다음 매치 정보를 알려준다.
struct CMSG_FM_CUR_MATCHINFO : public CMSG_FREE_MATCH
{
	u_char					event_state;		// EN_FM_STEP

	_sFreeMatchEvent		event_info;			// 다음 이벤트의 정보.

	time_t					event_time;

	CMSG_FM_CUR_MATCHINFO()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_CUR_MATCHINFO), en_dsfm_cur_matchinfo )
	{
		ZeroMemory( &event_info, sizeof(_sFreeMatchEvent) );
	}
};


#define FM_RANK_MAX_COUNT			(48)
//en_dsfm_cur_nick_owner
struct CMSG_FM_CUR_NICK_OWNER : public CMSG_FREE_MATCH
{
	u_short						rank_count;
	_sFM_LastRewardNickInfo		nick[FM_RANK_MAX_COUNT];

	CMSG_FM_CUR_NICK_OWNER()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_CUR_NICK_OWNER), en_dsfm_cur_nick_owner ), rank_count(0)
	{
	}
};

#endif	// _PD_GAME_EVENT_


// #define CMSG_NO_GM_EVENT						(92)		// GM 이벤트 패킷.
// 20080803
struct CMSG_GM_EVENT 
{
	u_short				usLength;
	u_char				cMessage;

	u_char				ucMode;
};

enum 
{
	en_cgm_event_instant_buff=0,					// 인스탄트 버프
};

struct CMSG_GM_EVENT_INSTANT_BUFF : public CMSG_GM_EVENT
{
	u_char				ucTarget;		// 0 - all, 1 - white, 2 - black

	short				buffIndex;		// 
	char				buffStep;		// 
};


//////////////////////////////////////////////////////////////////////////
// 흑백 대전
//////////////////////////////////////////////////////////////////////////
//#define CMSG_NO_PKEVENT							98		// 흑백 대전 관련 패킷.
struct CMSG_PKEVENT
{
	u_short				usLength;
	u_char				cMessage;

	u_char				event;
	// 0 - 흑백대전의 승자를 알리는 패킷.
	// 

	u_char				value1;
	u_char				value2;

	u_short				value3;

	u_int				value4;
};

/* =====================================================================================
로그인과 서버 이동 인증 : 0 - 7 까지.	
#define CMSG_NO_GSCONNECT_GS_DS					2
#define CMSG_NO_ENTERSERVER_CHECK_REQ_GS_DS		3
#define CMSG_NO_ENTERSERVER_CHECK_RESULT_DS_GS	4

빌링 관련..
#define CMSG_NO_GAME_SIGNAL						5
===================================================================================== */
// 로그인 인증 요청
//#define CMSG_NO_LOGIN_LS_DS						0		// LS->DS LOGIN REQUEST MESSAGE

// GS가 DS에 Connect 직후에 보내어, 서버의 정보를 알려준다.
//#define CMSG_NO_GSCONNECT_GS_DS					2
struct CMSG_GSCONNECT_GS_DS					// # GS->DS GS가 최초로 DS 에 접속한다.
{
	u_short				usLength;
	u_char				cMessage;				// CMSG_NO_GSCONNECT_GS_DS(4)
	char				cServerNum;				// 
};

//#define CMSG_NO_GSCONNECT_DS_GS					2
struct CMSG_GSCONNECT_DS_GS					// # GS->DS GS가 최초로 DS 에 접속한다.
{
	u_short				usLength;
	u_char				cMessage;				// CMSG_NO_GSCONNECT_GS_DS(4)
	char				cServerList[GS_MAX] ; // GS_MAX == 35
	u_int				uiServerStat;		//32bit 서버 상태 플래그
	//1: 봉문해제
};


// 게임 처음 접속 인증 요청과 서버 이동 인증
// 로그인 서버를 통과한 유저가 처음으로 게임 서버에 접속했을 때
// 게임 서버에서 DS 에 유저의 인증을 위해 보내는 패킷.
//#define CMSG_NO_ENTERSERVER_CHECK_REQ_GS_DS		3
struct CMSG_ENTERSERVER_CHECK_REQ_GS_DS
{
	u_short				usLength;
	u_char				cMessage;				// CMSG_NO_ENTERSERVER_CHECK_REQ_GS_DS 5
	char				cAccount[en_max_lil+1];			// 
	u_short				usUid;					// 
	u_int				iIP;					// 혹시 있을지모르는 해킹에 대비해서 아이피 검사까지 수행한다.
	int					iTime;					//
	char				cServerNum;
};


// 게임 접속이나 서버 이동의 인증(검사)의 결과는 성공일 경우, 캐릭터 리스트 패킷으로 대체하며
// 실패했을 때는 다음 패킷으로 한다.
//#define CMSG_NO_ENTERSERVER_CHECK_RESULT_DS_GS	4
struct CMSG_ENTERSERVER_CHECK_RESULT_DS_GS
{
	u_short				usLength;
	u_char				cMessage;				// CMSG_NO_ENTERSERVER_CHECK_RESULT_DS_GS 6
	char				cAccount[en_max_lil+1];			// 
	u_short				usUid;					// Account 만으로는 유일하지 않음.
};




/* =====================================================================================
로비에서 - 캐릭터 생성/삭제/선택 관련 8 - 15
#define CMSG_NO_CHARAC_SELECT_GS_DS				8		// 캐릭터 창에서 캐릭터를 선택했다.
#define CMSG_NO_CHARAC_CREATE_GS_DS				9		// 캐릭터 생성
#define CMSG_NO_CHARAC_DELETE_GS_DS				10		// 캐릭터 삭제
#define CMSG_NO_USER_CHARAC_LIST_DS_GS			11		// 캐릭터 리스트
#define CMSG_NO_CHARAC_RESULT_DS_GS				12		// 위의 3개의 패킷에 대한 응답으로 쓰인다.
#define CMSG_NO_CHARAC_LIST_REQ_GS_DS			13		// 캐릭터 리스트를 재요청
===================================================================================== */														// 자세한 건 패킷 정의부에 있다.

// 캐릭터 선택 메시지.  캐릭터 정보 혹은 CHARAC_RESULT
struct CMSG_CHARAC_SELECT_GS_DS				// # GS->DS 캐릭터 창에서 캐릭터를 선택했다. #
{
	u_short				usLength;
	u_char				cMessage ;				//	CMSG_NO_CHARAC_SELECT_GS_DS(8)

	u_short				usUid ;					// uid
	char				cAccount[en_max_lil+1];			// 
	char				cChrName[13];			//	캐릭터 이름
} ;


// 캐릭터 생성 메시지. CHARAC_RESULT
struct CMSG_CHARAC_CREATE_GS_DS				// #  GS->DS 캐릭터 생성 #
{
	u_short				usLength;
	u_char				cMessage ;				// CMSG_NO_CHARAC_CREATE_GS_DS(9)
	char				cAccount[en_max_lil+1];			// 

	char				cName[13];				// 캐릭터의 이름

	short				sConstitution;			// 건강 <= 생기	: 5기를 수치로 받는다. ( 8~18 사이이며, 합계는 반드시 50이어야 한다. 
	short				sZen;					// 진기 : 쟁여놓는 기수치는 없다. 
	short				sIntelligence;			// 지혜 <= 영기
	short				sDexterity;				// 민첩 <= 심기
	short				sStr;					// 근력

	char 				cSex;					// 성별 0 : 여자, 1 : 남자	

	char				cFace;					// 얼굴형 : max 16종 이지만 그냥 
	char				cHair;					// 머리 스타일 : max 16종 

	short				sAmor;					// 상의 : 2종 밖에 없는뎅 ㅡㅡ
	short				sPants;				// 하의 : 2종 밖에 없다. 

	short				sAdea ;
	short				sShoes ;


	char				cInitZone;				// 처음 위치할 서버
	float				fInitPosX;				// 초기 위치
	float				fInitPosZ;				//
} ;


// 캐릭터 삭제 요청 메시지 : 응답은 CHARAC_RESULT
struct CMSG_CHARAC_DELETE_GS_DS				// # GS->DS 캐릭터 삭제 #
{
	u_short				usLength;
	u_char				cMessage ;				// CMSG_NO_CHARAC_DELETE_GS_DS(10)
	char				cChrName[13];			//	캐릭터 이름

	char				cAccount[en_max_lil+1];			// 
	char				cPassword[13];			//	패스워드
} ;


// 캐릭터 리스트
struct CMSG_USER_CHARAC_LIST_DS_GS
{
	u_short				usLength;			
	u_char				cMessage;

	u_short				usUid;					// 
	char				cSelChrName[13];		// 선택한 캐릭터 정보

	_CharacList			List[3];				//

	_ACCOUNT_ITEM_USE_STATUS aius ;				// 

#if defined(_PD_PCROOM_INFO_) || defined(_PD_VINA_PCROOM_INFO_)
	u_char				ucWhereConnect;		// 어디에서 접속했는가.
	u_char				ucUserGrade;		// 유저의 등급.
#endif
#ifdef _VINA_SHUTDOWN_20061023_
	short				sFatigueWeight ;	//베트남 shutdown
#ifdef _PD_SDS_GOSTOP_
	u_char				ucFatigueMode;
#endif
#endif
};

#define ErrorMessageNum_CanNotSelect		20
#define ErrorMessageNum_ServerStateErr		21
// 로비에서의 캐릭터 조작의 응답.
struct CMSG_CHARAC_RESULT_DS_GS				// # DS->GS 캐릭터 생성/삭제/선택의 실패 결과 #
{
	u_short				usLength;
	u_char				cMessage ;				// CMSG_NO_CHARAC_RESULT_DS_GS(12)		
	char				cChrName[13];			// 생성이면 유저의 계정이고 선택,삭제 이면 캐릭의 아이디로 쓴다.

	u_short				usUid;					// 
	int					iCharacUid ;			// 캐릭터 유니크 아이디.

	char				cPrevCommand;			// 어떤 메세지의 결과인가. 
	char				cResult ;				// 0이면 성공, 아니면 실패 이유.
	// cPrevCommand : 캐릭터 생성에 대한 응답이면, 이 값은 MSG_NO_CHARAC_CREATE 이고, 
	// 성공이라면, ucResult 는 0이 될것이고, 아니면, 실패 이유가 될것이다.
	// MSG_CHARAC_CREATE 에 대한 대답으로, 단 2~5 번 오류는 Game Server에서 설정한다. 
	//   1 : 이미 사용중인 이름.  2: 사용할 수 없는 이름. 3: 기수치 오류  4: 기본 아이템 오류 5: 더이상 만들수 없음.
	// MSG_CHARAC_DELETE 의 대한 응답으로
	//   1 : 암호가 틀렸을 경우, 2: 다른 모든 오류
	// MSG_CHARAC_SELECT 에 대한 응답으로 
	//   20 : 캐릭터를 선택할 수 없음.
	// MSG_CHARAC_SELECT 에 대한 응답으로 
	//   21 : 해당 서버가 비정상적입니다.
} ;


// 캐릭터 리스트 요청
// CMSG_NO_CHARAC_LIST_REQ_GS_DS
struct CMSG_CHARAC_LIST_REQ_GS_DS
{
	u_short				usLength ;
	u_char				cMessage ;

	char				cAccount[en_max_lil+1];			// 
} ;


/* =====================================================================================
로그아웃, 서버이동 요청.  16 - 23
#define CMSG_NO_LOGOUT_GS_DS					16		// 단순 로그아웃
#define CMSG_NO_USER_LOGOUT_DS_GS				17		// 강제 로그아웃
#define CMSG_NO_REQ_SV_MOVE_GS_DS				18		// 서버간 이동 요청
===================================================================================== */
// 로그 아웃 패킷 
//#define CMSG_NO_LOGOUT_GS_DS					16
struct CMSG_LOGOUT_GS_DS
{
	u_short				usLength;
	u_char				cMessage;

	char				cAccount[en_max_lil+1];			// 
	u_short				usUid;					// 그냥 덤으로 보낸다. 하지만 비교는 할 것.
};

// 서버 이동 요청 
// 이 패킷은 요청이 아니라, '통보' 로 바뀌었음. 응답할 필요 없으며, 무시함.
// SelectCharac 은 로비에서 바로 서버이동을 할 경우 필요함. 
// 이 경우, 위치 정보를 업데이트 하면 않됨. ( bUpdatePos == false )
// 필드에서의 이동인 경우, bUpdatePos == true 가 되고, 위치 정보를 업데이트시킨다. 
struct CMSG_REQ_SV_MOVE_GS_DS
{
	u_short				usLength;
	u_char				cMessage;

	char				cAccount[en_max_lil+1];			// 
	char				cSelectCharac[13];		// 선택된 캐릭터
	int					iTime;					// 서버이동 시간 범위를 검사하기 위해..
	bool				bUpdatePos;				// 아래의 포지션값을 업데이트 할 것인가.
	char				cZone;					// 어떤 서버로 가는가. 0일 때는 밑의 값 무시.
	float				fGotoPosX;				// 목표 서버에서의 처음 위치
	float				fGotoPosZ;				//
};

//#define CMSG_NO_SV_MOVE_RESULT_DS_GS			19		// 결과
struct CMSG_SV_MOVE_RESULT_DS_GS
{
	u_short				usLength;
	u_char				cMessage;	
	u_short				usUid;
}; 


//#define CMSG_NO_ACCOUNT_INFO					22		// 계정 관련된 정보 패킷 : 0. 플레이 시간.
enum 
{
	en_cai_add_play_time=0,				// Add Play Time.
};
struct CMSG_ACCOUNT_INFO
{
	u_short				usLength;
	u_char				cMessage;

	u_char				ucMode;			// 

	char				cNDAccount[en_max_lil+1];

	CMSG_ACCOUNT_INFO( u_short size, u_char mode )
		: usLength(size), cMessage(CMSG_NO_ACCOUNT_INFO), ucMode( mode )
	{
	}
};

struct CMSG_AI_ADD_PLAY_TIME : public CMSG_ACCOUNT_INFO
{
	int					iPlayTime;		// added time : unit - second

	CMSG_AI_ADD_PLAY_TIME( char * acc, int playtime )
		: CMSG_ACCOUNT_INFO( sizeof(CMSG_AI_ADD_PLAY_TIME), en_cai_add_play_time )
	{
		strncpy( cNDAccount, acc, en_max_lil+1 );
		cNDAccount[en_max_lil] = 0;
		iPlayTime = playtime;
	}
};

/* =====================================================================================
캐릭터 정보 관련 또는 로비로 빠져나기기  24 - 39
#define CMSG_NO_CHARAC_ALL_INFO_DS_GS			24		// DS->GS ALL CHARAC INFO REQUEST MESSAGE
#define CMSG_NO_DEPOT_REQ_GS_DS					25		// GS->DS Depot data request..
#define CMSG_NO_DEPOT_RES_GS_DS					26		// DS->GS Depot data result..

#define CMSG_NO_CHARAC_RETURN_LOBBY_GS_DS		39		// 존에서 로비로 나감.
===================================================================================== */
// 
/*
#define _MAX_GAVE_ITEM_COUNT	16
struct _GaveItemList 
{
	size_t			uiCount ;
	_GaveItem		itemlist[_MAX_GAVE_ITEM_COUNT] ;
};
*/
struct CMSG_CHARAC_ALL_INFO_DS_GS
{
	u_short				usLength;
	u_char				cMessage;

	u_short				usUid;

	_CHARAC				charac;					// 캐릭터 이름은 이 안에 포함됨.
	_USER_DEPOT			storage;				// 창고 정보..

	_CHARAC_ETC_QUICKSLOT	quickslot;			//

#ifdef _PD_CHARAC_LTS_
	_CHARAC_LTS			characLts;
#endif

	u_int				uiCheckFlag_Login : 1;		// Is Check current event.
	u_int				uiCheckFlag : 31;				// Reserve

#ifdef _PD_EVENT_INVITE_FRIEND_
	int					iIF_DBIndex;
	int					iIF_EndTime;
#endif // _PD_EVENT_INVITE_FRIEND_
};

//#define CMSG_NO_DEPOT_REQ_GS_DS					25		// GS->DS Depot data request..
struct CMSG_DEPOT_REQ_GS_DS
{
	u_short				usLength ;
	u_char				cMessage ;

	char				cAccount[en_max_lil+1];			// 
};


//#define CMSG_NO_DEPOT_RES_GS_DS					26		// DS->GS Depot data result..
struct CMSG_DEPOT_RES_DS_GS
{
	u_short				usLength ;
	u_char				cMessage ;

	u_short				usTempUniqueID ;
	_USER_DEPOT			USER_DEPOT;
};



// 필드에서 로비로
// 이 메시지는 단순히 통지 메시지다. 응답은 없음.
struct CMSG_CHARAC_RETURN_LOBBY_GS_DS
{
	u_short				usLength;
	u_char				cMessage;

	char				cCName[en_charac_name_length+1];			// 
	char				cNDAccount[en_max_lil+1];
};


//#define CMSG_NO_RANKING_INFO_REQ				64			
struct CMSG_RANKING_INFO_REQ {
	u_short				usLength ;
	u_char				cMessage ;	

	short				sType ;
};


struct _Rank {
	short			sRanking ;			// 랭킹 순번
	char			cCharacName[13] ;	// 캐릭이름	( 또는 영웅단 이름 )
	short			sInnerLevel ;		// level
	u_int			sExp ;				// 경험치 ( 또는 영웅단의 경험치 )
	u_int			sContribution ;		// 기여도 ( 또는 영웅단의 문파 기여도 )
	char			cParty ;			// 문파 ( 또는 영웅단의 소속 문파 )
	char			cClass ;			// 역할

	_CHARAC_MATCH	m_MatchPoint ;	// 비무전적( 비무랭킹의 경우 )

	_Rank & _Rank::operator=( const _Rank& rhs ) 
	{
		try{
			if( this == &rhs ) { return *this ; }
			memcpy( this, &rhs, sizeof(_Rank) ) ;
		}
		catch(...){
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}
		return *this ;
	}	

	
};


//#define CMSG_NO_RANKING_INFO_RES				65			// 랭킹 정보를 응답
struct CMSG_RANKING_INFO_RES {			// 가변길이...
	u_short				usLength ;		// Header + 2 + (sizeof(_Rank) * sRankCount)
	u_char				cMessage ;

	short				sType ;

	short				sRankCount ;
	_Rank				rank[100] ;	// 가변길이..
};

//#define CMSG_NO_RANKING_UPDATE					66
struct CMSG_RANKING_UPDATE {
	u_short				usLength ;		// Header + 2 + (sizeof(_Rank) * sRankCount)
	u_char				cMessage ;

	short				sType ;

	short				sRankCount ;
	_Rank				rank[100] ;	// 가변길이..
};




//퀘스트 랭킹 데이터
struct QUEST_RANK_RECORD
{
	unsigned char	m_ucClan;
	char			m_acOrgName[en_or_name_length+1];		//개인: 無,			동행: 無,		단: 영웅단 이름
	char			m_acName[13];		//개인: 이름,		동행: 행수이름,	단: 단주이름
	unsigned short	m_usLevelOrCount;	//개인: 성취단계,	동행: 인원수,	단: 단인원
	unsigned short	m_usQuestData;		//KillCount, Time, ItemCount
};

#define MAX_QUEST_RANK_RECORD	(10)

struct _QUEST_RANK_DATA
{
	QUEST_RANK_RECORD	m_aRankRecord[MAX_QUEST_RANK_RECORD];
};


//#define CMSG_NO_QUESTRANK_DS_REQ				67			// 퀘스트랭킹 정보를 요구
struct MSG_QUESTRANK_DS_REQ
{
	u_short				usLength;
	u_char				cMessage;

	u_short				usQuestID;
};

//#define CMSG_NO_QUESTRANK_DS_RES				CMSG_NO_QUESTRANK_DS_REQ			// 퀘스트랭킹 정보를 응답
struct MSG_QUESTRANK_DS_RES
{
	u_short				usLength;
	u_char				cMessage;

	u_short				usQuestID;
	_QUEST_RANK_DATA	Data;
};

//#define CMSG_NO_QUESTRANK_DS_UPDATE				69			// 퀘스트랭킹 정보를 업데이트
struct MSG_QUESTRANK_DS_UPDATE
{
	u_short				usLength;
	u_char				cMessage;

	u_short				usQuestID;
	_QUEST_RANK_DATA	Data;
};


/*==========================================================================================]
[				DS <-> GS																	]
[===========================================================================================]
[	설명 :	UDP 메시지였던것들을 TCP 패킷 구조로 변경하였음 ( 2005-03-8 )					]
[											jkHong Add...	-								]
[==========================================================================================*/
//#define CMSG_NO_GSINFO_REQ_DS_GS				100	// 비 주기적인 요청 패킷이다..
struct CMSG_GSINFO_REQ_DS_GS					// # DS->GS GS에 서버 정보를 요청한다.
{
	u_short				usLength ;	
	u_char				cMessage;				//메세지 번호			
};


//#define CMSG_NO_GSINFO_RES_GS_DS				101	//주기적 응답, 또는 비 주기적 요청에 대한 응답
struct CMSG_GSINFO_RES_GS_DS					// # GS->DS GS의 서버 정보를 DS 에 알린다(UPDATE)
{
	u_short				usLength ;
	u_char				cMessage;				// MSG_NO_GSINFO_RES_GS_DS(5)
	
	char				cRealSN;
	_SERVER_SYSTEM_INFO	_ServerSystemInfo;
	_SERVER_STATE_INFO	_ServerStatInfo;
}; 

// #define		CMSG_NO_USER_LOGOUT_DS_GS		102		// 강제 로그아웃
struct CMSG_USER_LOGOUT_DS_GS
{
	u_short				usLength ;
	u_char				cMessage;

	char				cNDAccount[en_max_lil+1];			//사용자 계정
	char				cReason;				//이유

	// 이 패킷은 예외적으로 uid를 포함하지 않아도 됨.
	// 0 : 다른 곳에서 로그인
	// 1 : 운영자에의한 로그아웃
	// 2 : 제제( 접금... )	
	// 3 : 결제된 캐쉬 모두 소모(재결제해야 함)
};


// #define		CMSG_SHOWTEXT_REQ_DS_GS			103	// GS에 공지사항 출력 요청
struct CMSG_SHOWTEXT_REQ{
	u_short				usLength ;
	u_char				cMessage;

	char				cString[MAX_CHAT_LENGTH];
};


//#define		CMSG_NO_CREATE_ITEM_GS_DS		105 // 필드에 ITEM DROP(생성)
struct CMSG_CREATE_ITEM_GS_DS {
	u_short				usLength ;
	u_char				cMessage;

	char				cFirstType ;		// 대분류
	short				sSecondType ;		// 소분류
	float				fX ;				// x
	float				fZ ;				// y
};

// #define		CMSG_SYSTEM_OFF_REQ_NUM			110	// 시스템 종료 DS -> GS
struct CMSG_SYSTEM_OFF_REQ {
	u_short				usLength ;
	u_char				cMessage;	
};

//#define		CMSG_NO_SETPOSITION_REQ_DS_GS	120	// 캐릭터 위치 초기화 요청
struct CMSG_SETPOSITION_REQ{
	u_short				usLength ;
	u_char				cMessage;

	char				cCharacName[13];	// 캐릭터 이름
	float				fInitPosX;			// 초기 위치
	float				fInitPosZ;			//	
};


//#define		CMSG_NO_ALL_SPELL_REQ				113	// 백도/흑도 또는 전체에 해당 버프 적용. :GS->DS
//#define		CMSG_NO_ALL_SPELL_APPLY				114	// 113 결과로 DS -> GS 에 보내는 패킷. 받으면 적용.
struct CMSG_ALL_SPELL_REQ
{
	u_short				usLength ;
	u_char				ucMessage ;

	char				cApplyTarget ;			// -1 흑도, 1 백도.

	short				spell ;					//
	char				step ;					//
} ;

typedef CMSG_ALL_SPELL_REQ	CMSG_ALL_SPELL_APPLY ;

struct CMSG_QUEST_REWARD_NOTICE
{
	u_short				usLength;
	u_char				ucMessage;

	char				cTargetMap;			// 100 미만일 때는, 맵 인덱스, 이상일 때는 그룹 인덱스
	char				cTargetSex;			// 0 : 전체, 1 : 남자, 2 : 여자.
	char				cTargetCommunity;	// 0 : 없음. 1 : 동행, 2 : 영웅단.
	int					iTargetMoonpa;		// 문파 인덱스. 체크하는 함수 만들것.

	int					iGroupIndex;		// 동행이나, 영웅단 인덱스.

	_effect				effect;
};


//#define		CMSG_NO_ALL_SYSTEM_MSG				115	// GS->DS 로 전서버에 메세지 출력.
struct CMSG_ALL_SYSTEM_MSG
{
	u_short				usLength ;
	u_char				ucMessage ;

	char				cMode ;					//	0-15일반(칼라값), 16 동행 ,17 분파챗 32 귀속말 33 복면일반  34 복면귓말 64 브로드, 127 운영자의 메세지. 색지정 됨. -1 은 채팅 금지상태.

	char				cName[13] ;
	char				cSay[MAX_CHAT_LENGTH];				//	채팅 메시지( strlen + 1 : NULL 포함 ) // 51은 최대길이다..보낼 때 짤라서 보냄.

	char				cMoonpa ;
} ;



// 130 ~ 139 번 까지 예약됨. 사용할 때, 이호석에게 말해주세요.
//#define		CMSG_NO_CONSOLE_COMMAND		139
enum CONSOLE_COMMAND {
	// Vina Shutdown
	CONSOLECMD_DECREASE_ACQUISITION_POINT_OF_JIN,		// 경험치 포인트만 패널티 적용
	CONSOLECMD_DECREASE_ALL_ACQUISITION_POINT,			// 모든 포인트에 대해 패널티 적용
	CONSOLECMD_FORCED_LOGOFF,							// 유저 강제 종료
	
	CONSOLECMD_TESTMODE_ON,								// 테스트 모드 on
	CONSOLECMD_TESTMODE_OFF,							// 테스트 모드 off

	// insert below for other command cases
};
struct		CMSG_CONSOLE_COMMAND {
	u_short			usLength ;
	u_char			ucMessage ;

	UCHAR			ucCommand;	// CONSOLE_COMMAND를 형변환해서 사용
	int					iUnion;			// 자유롭게 사용하는 변수
};


//////////////////////////////////////////////////////////////////////////
//	135번 윤군이 [범용패킷]으로 사용합니다.
//	빈 번호 찾기 어렵..
//////////////////////////////////////////////////////////////////////////
//#define MSG_NO_COMMOM_REQ		67
//#define MSG_NO_COMMON_ACK		67
//#define MSG_NO_COMMON_NTF		67

//#define CMSG_NO_COMMOM_REQ		135
//#define CMSG_NO_COMMON_ACK		135
//#define CMSG_NO_COMMON_NTF		135

#define PRESENT_MSG_SIZE		100

struct _sUser_Inven_v1_all {};	// 컴파일 문제로 DS에 Dummy Structure 생성

typedef struct _MSG_COMMON {
	u_short			usLength;
	u_char			ucMessage;

	WORD			wMode;
/*
	//////////////////////////////////////////////////////////////////////////
	// 넥슨 피씨방 인증 및 혜택
	0: GS2C_NTF		[유저의 피씨방 정보]
	1: GS2C_NTF		[점혈술 남은 시간 동기화] 10분 마다 서버와 클라이언트 간에 점혈술 남은 시간을 동기화 한다.

	2: DS2GS_NTF	[유저의 피씨방 정보]
	
	//////////////////////////////////////////////////////////////////////////
	// 범용 CIS(Cash Item Shop)
	10: C2GS_REQ	[아이템 리스트 요청] 캐쉬인벤의 아이템 리스트 요청
	11: GS2C_ACK	[아아템 리스트 응답] 캐쉬인벤의 아이템 리스트 응답
	12: GS2DS_REQ	[아이템 리스트 요청] 캐쉬인벤의 아이템 리스트 요청
	13: DS2GS_ACK	[아아템 리스트 응답] 캐쉬인벤의 아이템 리스트 응답

	15:	C2GS_REQ	[아이템 이동 요청] 캐쉬인벤에서 행낭으로 아이템 이동 요청
	16:	GS2C_ACK	[아이템 이동 응답] 캐쉬인벤에서 행낭으로 아이템 이동 응답
	17:	GS2DS_REQ	[아이템 이동 요청] 캐쉬인벤에서 행낭으로 아이템 이동 요청
	18:	DS2GS_ACK	[아이템 이동 응답] 캐쉬인벤에서 행낭으로 아이템 이동 응답

	20: C2GS_REQ	[선물 아이템 상세정보 요청]
	21: GS2C_ACK	[선물 아이템 상세정보 응답]
	22: GS2DS_REQ	[선물 아이템 상세정보 요청]
	23: DS2GS_ACK	[선물 아이템 상세정보 응답]
*/

	union {
		BYTE	data[1024];

#ifdef _NXPCB_AWARD_200703_
		// wMode = 0, 2
		struct _NXPCB_INFO {
			char			acc[en_max_lil+1];
			NEXON_PCB_INFO	info;			// 피씨방 정보
		} NXPCB_Info;

		// wMode = 1
		struct _NXPCB_TOUCH_ATTACK_REMAIN_MIN {
			WORD			wRemainMin;		// 점혈술 남은 시간 분(min) 단위로 전송
		} NXPCBTouchAttackRemainMin;
#endif

#ifdef _CIS_20070326_
		// wMode = 10
		struct _CIS_CASH_INVEN_REQ {
			bool	bIsPresent;		// 0: 일반 아이템, 1: 선물 아이템
			WORD	wPageNum;		// 페이지 번호; 유저에게 보여지는 것과 같이, 1 base-index
			WORD	wNumPerPage;	// 페이지 당 자료수
		} CIS_CASH_INVEN_REQ;

		// wMode = 11
		struct _CIS_CASH_INVEN_ACK {
			USHORT	usTotalCnt;		// 캐쉬인벤의 (모든 페이지를 포함한) 총 아이템 개수; 몇 페이지를 보여줄지 계산하기 위해
			UCHAR	ucRowCount;		// 페이지에 보여줄 총 아이템 개수
				// ucRowCount 만큼 반복
				int		iPkgGuid;	// 아이템의 Guid
				int		iPkgIdx;	// 패키지 인덱스
		} CIS_CASH_INVEN_ACK;

		// wMode = 12
		struct _GS2DS_CIS_CASH_INVEN_REQ {
			char	szAcc[en_max_lil+1];
			bool	bIsPresent;		// 0: 일반 아이템, 1: 선물 아이템
			WORD	wPageNum;		// 페이지 번호; 유저에게 보여지는 것과 같이, 1 base-index
			WORD	wNumPerPage;	// 페이지 당 자료수
		} GS2DS_CIS_CASH_INVEN_REQ;

		// wMode = 13
		struct _DS2GS_CIS_CASH_INVEN_ACK {
			char	szAcc[en_max_lil+1];
			USHORT	usTotalCnt;		// 캐쉬인벤의 (모든 페이지를 포함한) 총 아이템 개수; 몇 페이지를 보여줄지 계산하기 위해
			UCHAR	ucRowCount;		// 페이지에 보여줄 총 아이템 개수
				// ucRowCount 만큼 반복
				int		iPkgGuid;	// 아이템의 Guid
				int		iPkgIdx;	// 패키지 인덱스
		} DS2GS_CIS_CASH_INVEN_ACK;

		// wMode = 15
		struct _CIS_MOVE_ITEM_REQ {
			UCHAR	ucPkgCnt;	// 이동할 아이템 개수
				// ucPkgCnt 만큼 반복
				int		iPkgGuid;	// 아이템의 Guid
				int		iPkgIdx;	// 패키지 인덱스
		} CIS_MOVE_ITEM_REQ;

		// wMode = 16
		struct _CIS_MOVE_ITEM_ACK {
			UCHAR	ucPkgCnt;	// 옮기기에 성공한 패키지 아이템 개수
				// ucPkgCnt 만큼 반복
				int		iPkgGuid;	// 아이템의 Guid
			
			UCHAR	ucItemCnt;	// 행낭으로 옮겨진 아이템 개수
				// ucItemCnt 만큼 반복
				//char	cSlot;		// 행낭의 슬롯 인덱스
				//char	cType;		// 아이템 분류
				//short	sId;
				//UCHAR	ucItemNum;	// 아이템의 뭉탱이 개수
				_sUser_Inven_v1_all	Item;
		} CIS_MOVE_ITEM_ACK;

		// wMode = 17
		struct _GS2DS_CIS_MOVE_ITEM_REQ {
			char	szAcc[en_max_lil+1];
			UCHAR	ucPkgCnt;	// 이동할 아이템 개수; GS에서 빈슬롯 검사후 ucPkgCnt 수정하여 DS로 전송
				// ucPkgCnt 만큼 반복
				int		iPkgGuid;	// 아이템의 Guid
		} GS2DS_CIS_MOVE_ITEM_REQ;

		// wMode = 18
		struct _DS2GS_CIS_MOVE_ITEM_ACK {
			char	szAcc[en_max_lil+1];
			UCHAR	ucPkgCnt;	// 옮기기에 성공한 패키지 아이템 개수; 쿼리 결과에 따라 ucPkgCnt 수정하여 GS로 전송
				// ucPkgCnt 만큼 반복
				int		iPkgGuid;	// 아이템의 Guid
				int		iPkgIdx;	// 패키지 인덱스
				bool	bRes;		// 캐쉬인벤에서 아이템 삭제 성공/실패 여부
		} DS2GS_CIS_MOVE_ITEM_ACK;

		// wMode = 20
		struct _CIS_PRESENT_MSG_REQ {
			int		iPkgGuid;	// 아이템의 Guid
		} CIS_PRESENT_MSG_REQ;

		// wMode = 21
		struct _CIS_PRESENT_MSG_ACK {
			char		szFrom[en_max_lil+1];	// 선물한 유저의 Account
			SYSTEMTIME	time;					// 선물 받은 날짜(년/월/일/시/분/초); DBTIMESTAMP
			char		szMsg[PRESENT_MSG_SIZE+1];	// 선물 메시지
		} CIS_PRESENT_MSG_ACK;

		// wMode = 22
		struct _GS2DS_CIS_PRESENT_MSG_REQ {
			char	szTo[en_max_lil+1];
			int		iPkgGuid;	// 아이템의 Guid
		} GS2DS_CIS_PRESENT_MSG_REQ;

		// wMode = 23
		struct _DS2GS_CIS_PRESENT_MSG_ACK {
			char		szTo[en_max_lil+1];		// 선물 받은 유저의 Account
			char		szFrom[en_max_lil+1];	// 선물한 유저의 Account
			SYSTEMTIME	time;					// 선물 받은 날짜(년/월/일/시/분/초); DBTIMESTAMP
			char		szMsg[PRESENT_MSG_SIZE+1];	// 선물 메시지
		} DS2GS_CIS_PRESENT_MSG_ACK;
#endif
	};
} MSG_COMMON, MSG_COMMON_REQ, MSG_COMMON_ACK, MSG_COMMON_NTF;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//#define		CMSG_GM_EDIT_COLLECTION_NUM		140		// 수집(명품) 데이터 에디트
struct CMSG_GM_EDIT_COLLECTION
{	
	u_short				usLength ;
	u_char				cMessage;

	char				sCharacName[13];
	u_char				ucEditFlag;		//0: 대상 컬랙션을 완료化 해 주세요
	//1: 대상 컬랙션을 모두 0으로 클리어 해 주세요(완료->미완)
	u_short				usCollectionIndex;
};

//#define		CMSG_GM_EDIT_EXPRATE_NUM		141		// 레벨 배율 조정
struct CMSG_GM_EDIT_EXPRATE
{
	u_short			usLength ;
	u_char			ucMessage ;
	
	int				iLevelExp;
	int				iSkillExp;
};



//#define		CMSG_GM_ADD_MASTERY_NUM			172		//	오의 추가
struct	CMSG_GM_ADD_MASTERY
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	short				sMasteryID;
	char				cPoint;
	char				cPage;
};
//#define		CMSG_GM_REM_MASTERY_NUM			173		//	오의 삭제
struct	CMSG_GM_REM_MASTERY
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	short				sMasteryID;
};
//#define		CMSG_GM_EDIT_MASTERY_NUM			174		//	오의 에디트(오의 포인트 변화)
struct	CMSG_GM_EDIT_MASTERY
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	short				sMasteryID;
	char				cPoint;
};
//#define		CMSG_GM_EDIT_LEFTMASTERYPOINT_NUM	175	//	남은 오의 포인트 에디트
struct	CMSG_GM_EDIT_LEFTMASTERYPOINT
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	short				sPoint;
};
//#define		CMSG_GM_EDIT_CHARAC_ROLE_NUM			176		//	역할 바꾸기
struct	CMSG_GM_EDIT_CHARAC_ROLE
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	char				cClass;
};
//#define		CMSG_GM_EDIT_CHARAC_CLASSGRADE_NUM	177	//	역할등급 바꾸기
struct	CMSG_GM_EDIT_CHARAC_CLASSGRADE
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	char				cClassGrade;
};	




//#define	CMSG_NO_GM_EDIT_LEVEL_NUM						//	레벨 에디트
struct CMSG_GM_EDIT_LEVEL	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	int					iLevel;
};
//#define	CMSG_NO_GM_EDIT_VITAL_NUM						//	생 에디트
struct CMSG_GM_EDIT_VITAL	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	short				sVital;
	unsigned int		uiHP;					//바뀌게 되는 체력
};
//#define	CMSG_NO_GM_EDIT_ZEN_NUM							//	진 에디트
struct CMSG_GM_EDIT_ZEN	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sZen;
	unsigned int		uiMP;					//바뀌게 되는 내력
};
//#define	CMSG_NO_GM_EDIT_SOUL_NUM							//	지 에디트
struct CMSG_GM_EDIT_SOUL	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sSoul;
	unsigned int		uiSoul;					//바뀌게 되는 영력
};
//#define	CMSG_NO_GM_EDIT_HEART_NUM						//	민 에디트
struct CMSG_GM_EDIT_HEART	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sDexterity;
};
//#define	CMSG_NO_GM_EDIT_JUNGGI_NUM						//	근 에디트
struct CMSG_GM_EDIT_JUNGGI	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sStr;
};
//#define	CMSG_NO_GM_EDIT_LEFTPOINT_NUM					//	남은 포인트 에디트
struct CMSG_GM_EDIT_LEFTPOINT	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sLeftPoint;
};
//#define	CMSG_NO_GM_EDIT_ZONE_NUM							//	존 바꾸기
struct CMSG_GM_EDIT_ZONE	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	int					iZone;
	float				fX;
	float				fZ;
};
//#define		CMSG_GM_EDIT_CODE_NUM					//	코드 바꾸기
struct CMSG_GM_EDIT_CODE	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	float				fX;
	float				fZ;
};
//#define	CMSG_NO_GM_EDIT_PARTY_NUM						//	파티 바꾸기
struct CMSG_GM_EDIT_PARTY	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	char				cParty;
};
//#define	CMSG_NO_GM_EDIT_NAME_NUM							//	이름 바꾸기(같은 이름이 존재하는지 조사는 이미 했음)
struct CMSG_GM_EDIT_NAME	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	char				Name[13];
};
//#define	CMSG_GM_EDIT_NICK_NUM							//	닉네임 바꾸기(같은 닉네임이 존재하는지 조사는 이미 했음)
struct CMSG_GM_EDIT_NICK	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	char				Name[13];
};

//#define		CMSG_GM_ADD_INVITEM_NUM				//	인벤토리에 아이템 추가
struct CMSG_GM_ADD_INVITEM	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID

	u_int64             ui64UniqueId; // 

	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	unsigned char		ucItemCount;
	unsigned short		usDurability;
	unsigned char		ucSlotCount;			//아이템에 슬롯이 있는 경우 사용

	unsigned char		ucInchant;					//아이템의 제련값

};

//#define		CMSG_GM_REM_INVITEM_NUM			163		//	인벤토리에 아이템 제거
struct CMSG_GM_REM_INVITEM	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	u_char				ucSlotNo;
};

struct CMSG_GM_ADD_DEPITEM	{
	u_short				usLength ;
	u_char				cMessage;
	char				cAccount[en_max_lil+1];			//UniqueUserID

	u_int64             ui64UniqueId; // 

	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	unsigned char		ucItemCount;
	unsigned char		ucSlotCount;			//아이템에 슬롯이 있는 경우 사용
	unsigned char		ucInchant;					//아이템의 제련값
};




//#define	CMSG_NO_GM_REM_DEPITEM_NUM						//	저장소에 아이템 제거

struct CMSG_GM_REM_DEPITEM	{
	u_short				usLength ;
	u_char				cMessage;
	char				cAccount[en_max_lil+1];			// 
	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	char				cSlotNo;
};


//#define	CMSG_NO_GM_ADD_INVGOLD_NUM						//	인벤토리의 돈 가감
struct CMSG_GM_ADD_INVGOLD	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	int					iGold;
};


//#define	CMSG_NO_GM_ADD_DEPGOLD_NUM						//	저장소의 돈 가감
struct CMSG_GM_ADD_DEPGOLD	{
	u_short				usLength ;
	u_char				cMessage;
	char				cAccount[en_max_lil+1];			// 
	int					iGold;
};
//#define	CMSG_NO_GM_ADD_SKILL_NUM							//	스킬 추가
struct CMSG_GM_ADD_SKILL	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sSkillID;
	short				sSkillStep;	
	int					iSkillExp;
};
//#define	CMSG_NO_GM_REM_SKILL_NUM							//	스킬 제거
struct CMSG_GM_REM_SKILL	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sSkillID;
};
//#define	CMSG_NO_GM_EDIT_SKILL_NUM						//	스킬 에디트(스킬 st
struct CMSG_GM_EDIT_SKILL	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sSkillID;
	short				sSkillStep;
	char				cSkillState ;			// 수련이 필요 여부(0 완료, 1 필요)
	int					iSkillExp;
};

//#define		CMSG_GM_EDIT_CONTRIBUTE_NUM		171		//	문파 기여도 수정
struct CMSG_GM_EDIT_CONTRIBUTE
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	int					iContribute;
};

//#define		CMSG_GM_EDIT_CHARAC_MUTE_TIME_NUM	178		// 채팅금지시키기
struct CMSG_GM_EDIT_CHARAC_MUTE
{
	u_short				usLength ;
	u_char				cMessage ;

	char				cCharacName[13] ;
	int					iTime ;				// 제한 시간(초)
};

//#define		CMSG_GM_EDIT_JIN_NUM	179		// 진 수정
struct CMSG_GM_EDIT_JIN
{
	u_short				usLength ;
	u_char				cMessage ;

	char				cCharacName[13] ;
	u_int				uiJin ;				// 진
} ;


//#define		CMSG_GM_EDIT_NICK_ID_NUM		180		// 별호 수정
struct CMSG_GM_EDIT_NICK_ID	
{
	u_short				usLength ;
	u_char				ucMessage ;
	char				cCharacName[13] ;
	short				sSlotNum ;
	u_short				usNickID ;
};

//#define		CMSG_GM_EDIT_INCHANTVALU_NUM		182		// 별호 수정
struct CMSG_GM_EDIT_INCHANTVALUE
{
	u_short				usLength ;
	u_char				ucMessage ;	

	unsigned char		ucWhere;	//0: Inven, 1: Depot

	union{
		char			sCharacName[13];	//ucWhere가 Inven일 경우 사용

		char				cAccount[en_max_lil+1];			// 
	};

	short				sSlotNumber;
	unsigned __int64    ui64UniqueId;
	unsigned char		ucInchantValue;
};


// CMSG_ENV_SETTING_NUM		146
struct CMSG_ENV_SETTING
{
	u_short				usLength;
	u_char				ucMessage;

	u_char				ucMode;

	int					iValue;

	CMSG_ENV_SETTING( u_char mode )
		: usLength( sizeof(CMSG_ENV_SETTING)), ucMessage(CMSG_ENV_SETTING_NUM), ucMode(mode), iValue(0)
	{
	}
};

enum EN_ENV_SETTING_MODE
{
	en_env_setting_cashitem_period_test=0,
};



//#define CMSG_GM_EDIT_VAR_NO			142		// 에디트 대표 번호.
// DS -> GS
struct CMSG_GM_EDIT_VAR
{
	u_short				usLength ;
	u_char				ucMessage ;

	u_char				ucMode ;			// 에디트는 모드로 나뉘어 사용된다. 앞으로 쭈욱.

	CMSG_GM_EDIT_VAR( u_short size, u_char mode )
		: usLength(size), ucMessage(CMSG_GM_EDIT_VAR_NO), ucMode(mode)
	{
	}
};

enum
{
	en_ds_gs_edit_mode_honor=0,			// 명성 수치 에디트
	en_ds_gs_edit_mode_fame,			// 악명 수치 에디트
	en_ds_gs_edit_mode_blood,			// 혈정 수치 에디트
};

struct CMSG_GM_EDIT_HONOR_VAR : public CMSG_GM_EDIT_VAR
{
	char			cCharacName[13] ;
	int				iValue ;

	CMSG_GM_EDIT_HONOR_VAR()
		: CMSG_GM_EDIT_VAR(sizeof(CMSG_GM_EDIT_HONOR_VAR), en_ds_gs_edit_mode_honor )
	{
	}
};

struct CMSG_GM_EDIT_FAME_VAR : public CMSG_GM_EDIT_VAR
{
	char			cCharacName[13] ;
	int				iValue ;

	CMSG_GM_EDIT_FAME_VAR()
		: CMSG_GM_EDIT_VAR(sizeof(CMSG_GM_EDIT_FAME_VAR), en_ds_gs_edit_mode_fame )
	{
	}
};

struct CMSG_GM_EDIT_BLOOD : public CMSG_GM_EDIT_VAR
{
	char			cCharacName[13];
	short			sBloodPoint;

	CMSG_GM_EDIT_BLOOD()
		: CMSG_GM_EDIT_VAR(sizeof(CMSG_GM_EDIT_BLOOD), en_ds_gs_edit_mode_blood )
	{
	}
};


#include "udp_packet.h"
//#define		CMSG_GM_EDIT_ITEM_NUM			143		// 아이템 정보 수정
struct CMSG_GM_EDIT_ITEM 
{
	u_short				usLength ;
	u_char				ucMessage ;

	u_char				ucMode ;			// 에디트는 모드로 나뉘어 사용된다. 앞으로 쭈욱.

	CMSG_GM_EDIT_ITEM( u_short length, u_char mode )
		: usLength(length), ucMessage(CMSG_GM_EDIT_ITEM_NUM), ucMode(mode)
	{
	}
};

// mode 0 - en_gm_edit_item_aius
struct CMSG_GM_EDIT_AIUS : public CMSG_GM_EDIT_ITEM
{
	char						cAccount[en_max_lil+1];

	_ACCOUNT_ITEM_USE_STATUS	aius;

	CMSG_GM_EDIT_AIUS() 
		: CMSG_GM_EDIT_ITEM( sizeof(CMSG_GM_EDIT_AIUS), en_gm_edit_item_aius )
	{
		ZeroMemory( cAccount, sizeof(cAccount) );
		ZeroMemory( &aius, sizeof(aius) );
	}
};

// en_gm_edit_item_edit_item
struct CMSG_GM_EDIT_ONE_ITEM : public CMSG_GM_EDIT_ITEM
{
	char						cCharacName[en_charac_name_length+1];

	u_char						slot;
	_sServer_InvenItem_v1		item;

	CMSG_GM_EDIT_ONE_ITEM()
		: CMSG_GM_EDIT_ITEM( sizeof(CMSG_GM_EDIT_ONE_ITEM), en_gm_edit_item_edit_item )
	{
		ZeroMemory( cCharacName, sizeof(cCharacName) );
		slot = 0;
		ZeroMemory( &item, sizeof(item) );
	}
};

//en_gm_edit_item_edit_period
struct CMSG_GM_EDIT_PERIOD : public CMSG_GM_EDIT_ITEM
{
	char						cCharacName[en_charac_name_length+1];

	u_char						slot;
	__inven_uselimit_info_v1	info;

	CMSG_GM_EDIT_PERIOD()
		: CMSG_GM_EDIT_ITEM( sizeof(CMSG_GM_EDIT_PERIOD), en_gm_edit_item_edit_period )
	{
		ZeroMemory( cCharacName, sizeof(cCharacName) );
		slot = 0;
		ZeroMemory( &info, sizeof(info) );
	}
};


//en_gm_edit_item_del_period
struct CMSG_GM_EDIT_DEL_PERIOD : public CMSG_GM_EDIT_ITEM
{
	char						cCharacName[en_charac_name_length+1];

	u_char						slot;

	CMSG_GM_EDIT_DEL_PERIOD()
		: CMSG_GM_EDIT_ITEM( sizeof(CMSG_GM_EDIT_DEL_PERIOD), en_gm_edit_item_del_period )
	{
		ZeroMemory( cCharacName, sizeof(cCharacName) );
		slot = 0;
	}
};

//en_gm_edit_item_edit_skintear
struct CMSG_GM_EDIT_EDIT_SKINTEAR : public CMSG_GM_EDIT_ITEM
{
	char						cCharacName[en_max_lil+1];
	
	u_char						ucWhere;

	u_char						slot;
	__inven_uselimit_info_v1	info;

	CMSG_GM_EDIT_EDIT_SKINTEAR()
		: CMSG_GM_EDIT_ITEM( sizeof(CMSG_GM_EDIT_EDIT_SKINTEAR), en_gm_edit_item_edit_skintear )
	{
		ZeroMemory( cCharacName, sizeof(cCharacName) );
		ucWhere = 0;
		slot = 0;
		ZeroMemory( &info, sizeof(info) );
	}
};

//////////////////////////////////////////////////////////////////////////
//
//  통합 로비 콘트롤 패킷.
//
//////////////////////////////////////////////////////////////////////////
enum EN_UNION_GROUP_LOBBY_CTRL
{
	en_ug_simple_grp_info=0,			// 상위 서버군 정보. : UDS -> GS
	en_ug_select_simple_grp=1,			// 상위 서버군 선택.
	en_ug_select_simple_grp_fail=2,		// 상위 서버군 선택 실패.

	en_ug_detail_grp_info=10,			// 하위 서버군의 정보. 캐릭터 정보 위주.

	en_ug_move_request,					// 11 캐릭터와 창고 이동 요청한다.
	en_ug_move_result_complete,			// 12 이동이 성공.
	en_ug_move_result_change_name,		// 13 이름을 바꿀 필요가 있다.
	en_ug_move_result_fail,				// 14 이동 요청 실패.


	en_ug_change_name_request=20,		// 이름 바꾸기 요청.
	en_ug_change_name_result,			// 21 결과.

	en_ug_notyet_move_in_new=30,		// 30 신서버군 : 아직 통합을 완료하지 않았다.
	en_ug_force_use_charac_in_new,		// 31 신서버군 : 아직 통합을 완료하지 않은 서버군에서, 강제로 통합을 완료하도록 설정.
	en_ug_already_move_in_old,			// 32 이미 통합이 완료된 구서버군으로 접속. 메세지 보여주고 튕김.
	en_ug_force_use_reseult,			// 33 원래는 force_use 패킷 하나로 하려고 했으나, 주민번호 입력이 포함되어 요청/응답으로 나는다.

	en_ug_insert_user_pointer=40,		// DS,GS 만 쓴다. DS->GS : 유저 포인터를 인증한다.(리스트 패킷을 받은 것처럼 )
} ;

//#define CMSG_NO_UNI_GROUP_LOBBY				20

struct CMSG_UNI_GROUP_LOBBY		// 통합 서버군 패킷 베이스 : 모든 패킷은 ucMode 로 구분.
{
	u_short				usLength ;
	u_char				cMessage ;

	u_char				ucMode ;	// packet_for_lobby 의 enum(EN_UNION_GROUP_LOBBY_CTRL) 값 사용.

	char				cNDAccount[en_max_lil+1];			// 

	CMSG_UNI_GROUP_LOBBY( u_short len, u_char mode )
		: usLength(len), cMessage(CMSG_NO_UNI_GROUP_LOBBY), ucMode(mode)
	{}

	void SetAccount( char * acc )
	{
		if( acc )
		{
			strncpy( cNDAccount, acc, en_max_lil+1 ) ;
		}
	}
} ;


struct CMSG_UG_SIMPLE_GROUP_INFO : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_simple_grp_info
	u_char			ucGroup_0_NoCharac ;		// 의천의 캐릭터 수.

	struct {
		u_char			ucMoveState ;		// 이동 상태 : 0 - 미완, 1 - 완료된 상태(이동시킬수 없음)
		u_char			ucNoCharac[3] ;		// 각 구 서버군의 캐릭터 수.
	} GroupInfo[3] ;		// 그룹마다의 상태와 캐릭터 개수.

	CMSG_UG_SIMPLE_GROUP_INFO() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_SIMPLE_GROUP_INFO), en_ug_simple_grp_info )
	{}
} ;


struct CMSG_UG_SELECT_SIMPLE_GRP : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_select_simple_grp
	u_char			ucSelectNewGroupNo ;	// 선택한 

	CMSG_UG_SELECT_SIMPLE_GRP() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_SELECT_SIMPLE_GRP), en_ug_select_simple_grp )
	{}
} ;


struct CMSG_UG_SELECT_SIMPLE_GRP_FAIL : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_select_simple_grp_fail
	u_char			ucSelectSimpleGrpFail ;		// 실패 이유 : "이미 통합 완료된 서버군입니다."

	CMSG_UG_SELECT_SIMPLE_GRP_FAIL() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_SELECT_SIMPLE_GRP_FAIL), en_ug_select_simple_grp_fail )
	{}
} ;


struct CMSG_UG_DETAIL_GRP_INFO : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_detail_grp_info=10
	// 캐릭터 이름. 레벨. (창고는.. )
	u_char			ucCharacCount ;
	u_char			ucDepotCount ;

	struct {
		char		cOldGroupIndex ;// 1,2,3 / 4,5,6 / 7,8,9
		u_char		ucCharacIndex ;	// 0,1,2
		short		sLevel ;		// 성취 단계
		char		cName[13] ;		// 이름 
	} DetailInfo[9] ;	// Max 9

	CMSG_UG_DETAIL_GRP_INFO() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_DETAIL_GRP_INFO), en_ug_detail_grp_info )
	{}
} ;


struct CMSG_UG_MOVE_REQUEST : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_move_request,					// 11 캐릭터와 창고 이동 요청한다.
	u_char			ucSelectCharac[3] ;		// 선택 캐릭터 인덱스. 없으면. 0~2, 0~2 ( 0-1, 1-2 ) 없으면 0xff
	u_char			ucSelectDepotGroup ;	// 선택 창고 서버군 인덱스. 0,1,2 없으면 0xff 
	char			cPassword[13] ;			// 주민번호 뒷자리 또는 기타.

	CMSG_UG_MOVE_REQUEST() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_MOVE_REQUEST), en_ug_move_request )
	{}
} ;


struct CMSG_UG_MOVE_RESULT_COMPLETE : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_move_result_complete
	// 
	u_char			ucCompleteName[3][13] ;		// 옮긴 캐릭터 이름.
	u_short			usInnerLevel[3] ;			// 캐릭터 레벨.
	u_char			ucState[3] ;				// 0 - 완료, 1 - 이름을 바꿔야 한다.
	u_char			ucCompleteDepotGroup ;		// 없으면, 0xff

	CMSG_UG_MOVE_RESULT_COMPLETE() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_MOVE_RESULT_COMPLETE), en_ug_move_result_complete )
	{}
} ;

struct CMSG_UG_MOVE_RESULT_FAIL : public CMSG_UNI_GROUP_LOBBY
{
	//	en_ug_move_result_fail,				// 14 이동 요청 실패.
	u_char			ucFail ;					// 어따쓰지...

	CMSG_UG_MOVE_RESULT_FAIL() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_MOVE_RESULT_FAIL), en_ug_move_result_fail )
	{}
} ;

struct CMSG_UG_CHANGE_NAME_REQUEST : public CMSG_UNI_GROUP_LOBBY
{
	//	en_ug_change_name_request=20,		// 이름 바꾸기 요청.
	u_char			ucIndex ;				// 0,1,2 바꾸려는 캐릭터의 인덱스.
	u_char			ucNewName[13] ;			// 새이름

	CMSG_UG_CHANGE_NAME_REQUEST() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_CHANGE_NAME_REQUEST), en_ug_change_name_request )
	{}
} ;


struct CMSG_UG_CHANGE_NAME_RESULT : public CMSG_UNI_GROUP_LOBBY
{
	//	en_ug_change_name_result,			// 21 결과.
	u_char			ucSuccess ;				// 0 - Success, 나머진 실패.
	u_char			ucIndex ;				// 
	u_char			ucNewName[13] ;			// 

	CMSG_UG_CHANGE_NAME_RESULT() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_CHANGE_NAME_RESULT), en_ug_change_name_result )
	{}
} ;


struct CMSG_UG_NOTYET_MOVE_IN_NEW : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_notyet_move_in_new : Server -> Client 통보.
	CMSG_UG_NOTYET_MOVE_IN_NEW()
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_NOTYET_MOVE_IN_NEW), en_ug_notyet_move_in_new ) 
	{}
} ;

struct CMSG_UG_FORCE_USE_CHARAC_IN_NEW : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_force_use_charac_in_new : Client -> Server 로 요청 
	char		cPW[13]	;		// 추가 2006/06/24 : 확인 용

	CMSG_UG_FORCE_USE_CHARAC_IN_NEW()
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_FORCE_USE_CHARAC_IN_NEW), en_ug_force_use_charac_in_new ) 
	{}
} ;

struct CMSG_UG_ALREADY_MOVE_IN_OLD : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_already_move_in_old : Server -> Client
	CMSG_UG_ALREADY_MOVE_IN_OLD()
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_ALREADY_MOVE_IN_OLD), en_ug_already_move_in_old ) 
	{}
} ;

struct CMSG_UG_FORCE_USE_RESULT : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_force_use_reseult
	u_char				ucResult ;	// 0 - Success, 1 - 비번 확인, 2 - 실패.

	CMSG_UG_FORCE_USE_RESULT() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_FORCE_USE_RESULT), en_ug_force_use_reseult )
	{}
} ;

struct CMSG_UG_INSERT_USER_POINTER : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_insert_user_pointer
	u_short					usTempUID ;

	CMSG_UG_INSERT_USER_POINTER()
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_INSERT_USER_POINTER), en_ug_insert_user_pointer ) 
	{}
} ;


//#define		CMSG_NO_CHECK_DSGS_PACKET		21
enum
{
	en_check_packet_test_request=0,
	en_check_packet_test_success,
	en_check_packet_test_fail,

	en_check_packet_timeout_request=10,		// 타임 아웃 패킷 요청. ( 받으면, 10초 안에 응답할 것 )
	en_check_packet_timeout_response,		// 응답.
};
struct CMSG_CHECK_DSGS_PACKET
{
	u_short				usLength ;
	u_char				cMessage ;

	u_char				ucMode ;	// 0 - request, 1 - result success, 2 - result different

	struct {
		u_char		msg ;
		short		length ;
	} check_pair[30] ;
} ;


#ifdef _VINA_SHUTDOWN_20061023_
/////////////////////////////////////////////////////
//피로도 값 DB에 저장
//#define CMSG_NO_SAVE_FATIGUE_WEIGHT		44
////////////////////////////////////////////////////
struct MSG_SAVE_FATIGUE_WEIGHT
{
	u_short			usLength;
	u_char			ucMessage;

	short			sFatigueWeight;
	char			cAccount[en_max_lil+1];
#ifdef _PD_SDS_GOSTOP_
	u_char			ucFatigueMode;			// 0 : Go, 1 : Stop.
#endif
};
#endif

#ifdef _PD_CHARAC_LTS_

//#define CMSG_NO_LTS						116		// 위치 추적.
////////////////
//기본
//위치추적 기본 패킷
struct CMSG_LTS
{
	u_short					usLength;
	u_char					ucMessage;

	u_char					ucPktMode;

	CMSG_LTS(u_short length, u_char mode)
		: usLength(length),ucMessage(CMSG_NO_LTS),ucPktMode(mode)
	{}
};

enum EN_LTS_GS_DS
{
	//pk범 위치추적
	en_lts_pk_insert_req_gs_ds =0,
	en_lts_pk_insert_success_ds_gs,
	en_lts_pk_insert_failed_ds_gs,
	en_lts_pk_del_req_gs_ds,

	// 특정인 추적
	en_lts_sp_insert_req_gs_ds = 10,
	en_lts_sp_insert_success_ds_gs,
	en_lts_sp_insert_failed_ds_gs,
	en_lts_sp_del_req_gs_ds,
};

//Pk범 위치추적 en_lts_pk_insert_req_gs_ds,
struct CMSG_LTS_PK_INSERT_REQ_GS_DS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	_sLTS_Data		PkData;

	CMSG_LTS_PK_INSERT_REQ_GS_DS()
		:CMSG_LTS(sizeof(CMSG_LTS_PK_INSERT_REQ_GS_DS), en_lts_pk_insert_req_gs_ds)
	{}
};

//	en_lts_pk_insert_success_ds_gs,
struct CMSG_LTS_PK_INSERT_SUCCESS_DS_GS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	_sLTS_Data		PkData;

	CMSG_LTS_PK_INSERT_SUCCESS_DS_GS()
		: CMSG_LTS(sizeof(CMSG_LTS_PK_INSERT_SUCCESS_DS_GS), en_lts_pk_insert_success_ds_gs)
	{}
};

//	en_lts_pk_insert_failed_ds_gs,
struct CMSG_LTS_PK_INSERT_FAILED_DS_GS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	u_char ucRes;

	CMSG_LTS_PK_INSERT_FAILED_DS_GS()
		: CMSG_LTS(sizeof(CMSG_LTS_PK_INSERT_FAILED_DS_GS), en_lts_pk_insert_failed_ds_gs)
	{}
};

//	en_lts_pk_del_req_gs_ds,
//2. pk범 위치추적
struct CMSG_LTS_PK_DEL_REQ_GS_DS : public CMSG_LTS
{
	char	szCharacName[en_charac_name_length+1];
	char	szDelCharacName[en_charac_name_length+1];

	CMSG_LTS_PK_DEL_REQ_GS_DS()
		: CMSG_LTS(sizeof(CMSG_LTS_PK_DEL_REQ_GS_DS), en_lts_pk_del_req_gs_ds)
	{}
};

//특정인
struct CMSG_LTS_SP_INSERT_REQ_GS_DS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	_sLTS_Data		SpData;

	CMSG_LTS_SP_INSERT_REQ_GS_DS()
		: CMSG_LTS(sizeof(CMSG_LTS_SP_INSERT_REQ_GS_DS), en_lts_sp_insert_req_gs_ds)
	{}
};


//	en_lts_sp_insert_success_ds_gs,
struct CMSG_LTS_SP_INSERT_SUCCESS_DS_GS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	_sLTS_Data SpData;

	CMSG_LTS_SP_INSERT_SUCCESS_DS_GS()
		: CMSG_LTS(sizeof(CMSG_LTS_SP_INSERT_SUCCESS_DS_GS), en_lts_sp_insert_success_ds_gs)
	{}
};


//	en_lts_sp_insert_failed_ds_gs,
struct CMSG_LTS_SP_INSERT_FAILED_DS_GS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	u_char	ucRes;

	CMSG_LTS_SP_INSERT_FAILED_DS_GS()
		: CMSG_LTS(sizeof(CMSG_LTS_SP_INSERT_FAILED_DS_GS), en_lts_sp_insert_failed_ds_gs)
	{}
};

//en_lts_sp_del_req_gs_ds,

struct CMSG_LTS_SP_DEL_REQ_GS_DS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	char		szDelCharacName[en_charac_name_length+1];

	CMSG_LTS_SP_DEL_REQ_GS_DS()
		: CMSG_LTS(sizeof(CMSG_LTS_SP_DEL_REQ_GS_DS), en_lts_sp_del_req_gs_ds)
	{}
};
#endif//#ifdef _PD_CHARAC_LTS_

//////////////////////////////////////////////////////////////////////////
// 친구 소환 : 20070604 추가.
//////////////////////////////////////////////////////////////////////////
//#define		CMSG_NO_RECALL_FRIEND				(117)
struct CMSG_RECALL_FRIEND
{
	u_short					usLength;
	u_char					ucMessage;

	u_char					ucFriendMode;		// EN_CMSG_RECALL_FRIEND

	u_char					ucRFMode;			// 0 - 친구 소환, 1 - 친구옆으로 이동.

	char					cFromName[en_charac_name_length+1];
	char					cTargetName[en_charac_name_length+1];



	CMSG_RECALL_FRIEND( u_short size, u_char mode, u_char rf_mode )
		: usLength( size ), ucMessage( CMSG_NO_RECALL_FRIEND ), ucFriendMode(mode), ucRFMode(rf_mode)
	{
	}

	inline void set_from_name( char * name ) 
	{ 
		CopyMemory(cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}

	inline void set_target_name( char * name ) 
	{ 
		CopyMemory(cTargetName, name, en_charac_name_length );
		cTargetName[en_charac_name_length] = 0;
	}
};


enum EN_CMSG_RECALL_FRIEND
{
	en_crf_request_recall=0,		// gs->ds->gs 요청 처리 
	en_crf_answer,					// gs->ds->gs 응답 처리
	en_crf_force_move,				// gs->ds->gs 강제 이동.( 성공시에 마지막으로 강제 이동시킨다.)
	en_crf_fail,					// 모든 단계에서의 실패 메시지.
};


//	en_crf_request_recall=0,		// gs->ds->gs 요청 처리 
struct CMSG_RF_REQUEST_RECALL : public CMSG_RECALL_FRIEND
{
	u_char				ucFromZoneIndex;		// 어느 지역에서 소환하는가.

	char				cMoonpaIndex;			// 

	CMSG_RF_REQUEST_RECALL( u_char rf_mode )
		: CMSG_RECALL_FRIEND( sizeof(CMSG_RF_REQUEST_RECALL), en_crf_request_recall, rf_mode )
	{
	}
};


//	en_crf_answer,					// gs->ds->gs 응답 처리
struct CMSG_RF_ANSWER : public CMSG_RECALL_FRIEND
{
	u_char				ucYesNo;		// 0 - yes, 1 - no
	u_char				ucReason;

	u_char				ucZoneIndex;

	float				fTargetX;
	float				fTargetZ;

	CMSG_RF_ANSWER( u_char rf_mode )
		: CMSG_RECALL_FRIEND( sizeof(CMSG_RF_ANSWER), en_crf_answer, rf_mode )
	{
	}
};


//	en_crf_force_move,				// gs->ds->gs 강제 이동.( 성공시에 마지막으로 강제 이동시킨다.)
struct CMSG_RF_FORCE_MOVE : public CMSG_RECALL_FRIEND
{
	u_char				ucFromZoneIndex;

	float				fTargetX;
	float				fTargetZ;

	CMSG_RF_FORCE_MOVE( u_char rf_mode )
		: CMSG_RECALL_FRIEND( sizeof(CMSG_RF_FORCE_MOVE), en_crf_force_move, rf_mode )
	{
	}
};


enum EN_RF_FAIL_REASON
{
	en_rf_answer_no=0,				// no 선택.

	en_rf_fail_no_target=1,			// 1 타겟이 없다.
	en_rf_fail_no_item,				// 소환 아이템 없다. 
	en_rf_fail_wrong_point,			// 소환 지점의 위치가 이상하다. 

	en_rf_fail_my_con_trade=10,		// (자신은) 거래중이다.

	en_rf_fail_my_con_combat,		// (자신은) 장원전 참여중이다.
	en_rf_fail_my_con_zone,			// (자신은) 패왕동/호출할 수 없는 지역이다.
	en_rf_fail_my_con_exbattle,		// (자신은) 결 진행중이다.

	en_rf_fail_my_busy,				// (자신은) 다른 캐릭터와 친구 소환 중이다.


	en_rf_fail_target_con_trade=20,	// 거래중이다.

	en_rf_fail_target_con_combat,	// 장원전 참여중이다.
	en_rf_fail_target_con_zone,		// 패왕동/호출할 수 없는 지역이다.
	en_rf_fail_target_con_exbattle,	// 결 진행중이다.

	en_rf_fail_target_busy,			// 다른 대상과 친구 소환 중이다.
};

//	en_crf_fail,					// 모든 단계에서의 실패 메시지.
struct CMSG_RF_FAIL : public CMSG_RECALL_FRIEND
{
	u_char				ucFailReason;

	CMSG_RF_FAIL( u_char rf_mode )
		: CMSG_RECALL_FRIEND( sizeof(CMSG_RF_FAIL), en_crf_fail, rf_mode )
	{
	}
};


#pragma pack( pop, enter_tcp_packet )