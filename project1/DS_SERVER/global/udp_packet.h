#pragma once
#pragma pack( push, enter_udp_packet )
#pragma pack(1)


/*==========================================================================================]
[				LS MSG 패킷 MESSAGE 번호													]
[===========================================================================================]
[	설명 : DS<->LS  패킷은 U 로 시작한다..													]
[			다른 UDP 관려 메시지 번호와 중복을 방지하기 위해 LS->DS 메시지 번호를 200 번부터]
[			시작합니다.																		]
[											jkHong Add...									]
[==========================================================================================*/
#define		UMSG_LS_DS_TEST_NUM				120		// LS/DS 간 통신 테스트
#define		UMSG_LOGIN_LS_DS_NUM			121
#define		UMSG_LOGIN_DS_LS_RESULT_NUM		122
#define		UMSG_LOGIN_INFO_REQ_LS_DS_NUM	123
#define		UMSG_LOGIN_INFO_RES_DS_LS_NUM	124
#define		UMSG_CHARAC_DELETE_DS_MS_NUM	125		// 메신저 서버에 캐릭터 삭제를 알린다.
#define		UMSG_GM_CHARAC_SET_DS_MS_NUM	126		// 메신저에 특정 캐릭터의 GM 속성을 설정한다.
#define		UMSG_CHARAC_OR_KICK_NUM			127

// #define UMSG_LOGIN_DS_NUM	121
struct UMSG_LOGIN_LS_DS						//# LS->DS 로긴 요청 #
{
	int			iKey;
	char		cMessage ;				//	CMSG_NO_LOGIN_LS_DS(0)
	char		cServerNum ;

	char		cAccount[en_max_lil+1];			//	구룡 계정
#ifdef _NXPCB_AWARD_200703_
	char		cNXAcc[en_max_lil+1];			//	넥슨 계정
#endif
	char		cPass[en_max_lpl+1];				// password
	int			iAccUid ;				// 
	int			iTempUniqueID ;			//  LS에서 부여한 UID
	u_int		iIp;					//	접속자의  IP
	int			iTime;					//	접속 시도 시간
	char		cClass ;

#ifdef _PD_VINA_PCROOM_INFO_
	u_char		ucPCRoomInfo;			// 0 - Home, 1~ PC Room.
#endif

#ifdef _METEL_LOGIN_200608_
	bool		bPCBang;				// PCBang User or not?
	METEL_BILLING_INFO	MetelBillingInfo;
#endif //_METEL_LOGIN_200608_
};


// 로그인 결과 패킷
// #define UMSG_LOGIN_DS_LS_RESULT_NUM				122		// DS->LS LOGIN RESPONSE MESSAGE SUCCESS
// 오류 번호
#define _ERRNO_USING_ACCOUNT_					-2
#define _ERRNO_DB_ERROR_						-3
#define _ERRNO_STATE_ERROR_						-4
#define _ERRNO_IS_FULL_							-5
struct UMSG_LOGIN_DS_LS_RESULT
{
	int			iKey ;
	char		cMessage;				// 1
	char		cForestNo ;				// 서버군 번호

	char		cAccount[en_max_lil+1];			//
	u_short		usUid ;
	int			iTempID ;	
	// 시도를 할 수 있기 때문에, 계정만으로는 구분할 수 없다. 

	char		cServerNo ;				// 성공일 경우( sServerNo > 0 )엔 서버의 번호가 
	// 실패일 경우에는 에러 번호가 들어간다. 
	// 0번과 -1은 게임서버내에서 서용.
	// -2 : 사용중인 계정
	// -3 : DB 오류
	// -4 : 상태 오류/그냥 실패로 보여줌.
};


//#define		UMSG_LOGIN_INFO_REQ_LS_DS_NUM	123
struct UMSG_LOGIN_INFO_REQ_LS_DS
{
	int			iKey ;
	char		cMessage ;
	char		cServerNum ;
};

//_FOREST_STATE_RUNNING = 0,			// 정상 작동중
//_FOREST_STATE_ISFULL = -1,			// 서버 포화상태
//_FOREST_STATE_CHECKUP_ = -2,			// 서버군 점검중
//#define		UMSG_LOGIN_INFO_RES_DS_LS_NUM	124
struct UMSG_LOGIN_INFO_RES_DS_LS
{
	int			iKey ;
	char		cMessage ;
	char		cForestNum ;

	char		cForestState ;	
};

//#define		UMSG_CHARAC_DELETE_DS_MS_NUM	125
struct UMSG_CHARAC_DELETE_DS_MS {
	int			iKey ;
	char		cMessage ;

	char		cCharacName[13] ;
	int			iUid ;
};


// GM Charac setting
//#define		UMSG_GM_CHARAC_SET_DS_MS_NUM	126
struct UMSG_GM_CHARAC_SET_DS_MS {
	int			iKey ;
	char		cMessage ;

	char		cCharacName[13] ;
	char		cClass ;
};


//#define  UMSG_CHARAC_OR_KICK_NUM			127
struct UMSG_CHARAC_OR_KICK {
	int			iKey ;
	char		cMessage ;

	char		cCharacName[13] ;
	char		cORName[en_or_name_length+1] ;
};



struct UMSG_LS_DS_TEST
{
	int			iKey;
	char	cMessage;
};


#define	MSG_NO_LOCAL_COMMANDER			15	// DS 와의 통신 패킷이다. 번호 고치지 말것.
struct MSG_LOCAL_COMMANDER
{
	int			iKey;
	char		cMessage;

	u_char		ucMode;			// EN_LC_DS_PACKET_MODE 를 사용한다.

	MSG_LOCAL_COMMANDER()
		: iKey(PASS_KEY), cMessage(MSG_NO_LOCAL_COMMANDER), ucMode(0)
	{
	}

	MSG_LOCAL_COMMANDER( u_char mode )
		: iKey(PASS_KEY), cMessage(MSG_NO_LOCAL_COMMANDER), ucMode(mode)
	{
	}
};


// mode : 
enum EN_LC_DS_PACKET_MODE
{
	en_lc_ds_open_req=0,
	en_ds_lc_open_result,

	en_lc_ds_close_req,
	en_ds_lc_close_result,

	en_lc_ds_off_req,
	en_ds_lc_off_result,

	en_lc_ds_on_req,					//
	en_ds_lc_on_result,					//
};

/*==========================================================================================]
[				GMS MSG 패킷 MESSAGE 번호													]
[===========================================================================================]
[	설명 : GMS <-> DS 와 통신하는 MSG 패킷은 M 으로 시작한다.								]
[											jkHong Add...									]
[==========================================================================================*/

//[GMS 관련]===============================================================================================

// 121 ~ 127 번은 로그인 서버 메시지.

// GMS -> DS
#define		MSG_SYSTEM_INFO_REQ_NUM			1	// 시스템 정보 요청 GMS -> DS *
#define		MSG_SYSTEM_INFO_RES_NUM			2	// 시스템 정보 응답 DS -> GMS
//#define		MSG_MESSAGE_INFO_REQ_NUM		3	// 공지 정보 요청 GMS -> DS *
//#define		MSG_MESSAGE_INFO_NUM			4	// 공지 정보 응답 DS -> GMS
//#define		MSG_MESSAGE_SEND_NUM			5	// 공지 추가 요청 GMS -> DS *
//#define		MSG_MESSAGE_EDIT_SEND_NUM		6	// 공지 수정 요청 GMS -> DS *
//#define		MSG_MESSAGE_DELETE_SEND_NUM		7	// 공지 삭제 요청 GMS -> DS *
//#define		MSG_MESSAGE_RESULT_NUM			8	// 공지사항 추가/삭제/수정 에 대한 결과 DS->GMS
#define		MSG_MESSAGE_DIRECT_NUM			9	// 공지사항 바로 때리기.. GMS->DS

#define		MSG_SYSTEM_OFF_REQ_NUM			10	// 시스템 종료 GMS -> DS *

#define		MSG_SYSTEM_TIME_REQ_NUM			11	// 시스템 시간 요청(시간 동기화) GMS->DS
#define		MSG_SYSTEM_TIME_RES_NUM			12	// 시스템 시간 응답 DS->GMS
#define		MSG_GM_REQ_SERVER_PLAYER_NUM	13	// 각 서버별 동접수를 요청한다.
#define		MSG_GM_SERVER_PLAYER_NUM		14	// 각 서버별 동접수를 요청한다.

// 15 번 LC 패킷으로 사용중.

#define		MSG_SETPOS_REQ_NUM				20	// 캐릭터 위치정보 초기화
#define		MSG_SETPOS_RES_NUM				21	// 캐릭터 위치정보 초기화 응답
#define		MSG_CREATE_ITEM_REQ_NUM			22	// create item request
#define		MSG_CREATE_ITEM_RES_NUM			23	// create item result

#define		MSG_CMD_USERKILL_NUM			31	// 사용자 강제 종료
#define		MSG_CMD_USERKILL_RESULT_NUM		32	// 사용자 강제 종료

#define		MSG_CHARAC_REQ_NUM				41	// 캐릭터 정보 요청
#define		MSG_CHARAC_RES_NUM				42	// 캐릭터 정보 응답
#define		MSG_CHARAC_UPDATE_NUM			43	// 캐릭터 정보 업데이트

#define		MSG_USER_DEPOT_REQ_NUM			44	// 창고 정보 요청
#define		MSG_USER_DEPOT_RES_NUM			45	// 창고 정보 응답
#define		MSG_INVEN_REQ_NUM				46	// 인벤토리 정보 요청
#define		MSG_INVEN_RES_NUM				47	// 인벤토리 정보 응답
#define		MSG_USER_DEPOT_UPDATE_NUM		48	// 창고 정보 업데이트
#define		MSG_INVEN_UPDATE_NUM			49	// 인벤토리 정보 업데이트

#define		MSG_QUEST_REQ_NUM				50	// 퀘스트 요청
#define		MSG_QUEST_RES_NUM				51	// 퀘스트 응답
#define		MSG_QUEST_UPDATE_NUM			52	// 퀘스트 업데이트


#define		MSG_MARTIAL_REQ_NUM				53	// 무공 요청
#define		MSG_MARTIAL_RES_NUM				54	// 무공 응답
#define		MSG_MARTIAL_UPDATE_NUM			55	// 무공 업데이트..


// CHANGKHAN's EDITION   !GMS 의 Message Num 는 1~99 까지 사용하세요.
#define		MSG_GM_EDIT_PARTY_NUM			60		//	파티 바꾸기
#define		MSG_GM_EDIT_LEVEL_NUM			61		//	레벨 에디트
#define		MSG_GM_EDIT_VITAL_NUM			62		//	생 에디트
#define		MSG_GM_EDIT_ZEN_NUM				63		//	진 에디트
#define		MSG_GM_EDIT_SOUL_NUM			64		//	지 에디트
#define		MSG_GM_EDIT_HEART_NUM			65		//	민 에디트
#define		MSG_GM_EDIT_JUNGGI_NUM			66		//	근 에디트
#define		MSG_GM_EDIT_LEFTPOINT_NUM		67		//	남은 포인트 에디트
#define		MSG_GM_EDIT_ZONE_NUM			68		//	존 바꾸기
#define		MSG_GM_EDIT_CODE_NUM			69		//	코드 바꾸기
#define		MSG_GM_EDIT_GMCLASS_NUM			70		//	GM CLASS 바꾸기
#define		MSG_GM_EDIT_NAME_NUM			71		//	이름 바꾸기(같은 이름이 존재하는지 조사는 이미 했음)
#define		MSG_GM_EDIT_NICK_NUM			72		//	닉네임 바꾸기(같은 닉네임이 존재하는지 조사는 이미 했음)
#define		MSG_GM_ADD_INVITEM_NUM			73		//	인벤토리에 아이템 추가
#define		MSG_GM_REM_INVITEM_NUM			74		//	인벤토리에 아이템 제거
#define		MSG_GM_ADD_DEPITEM_NUM			75		//	저장소에 아이템 추가
#define		MSG_GM_REM_DEPITEM_NUM			76		//	저장소에 아이템 제거
#define		MSG_GM_ADD_INVGOLD_NUM			77		//	인벤토리의 돈 가감
#define		MSG_GM_ADD_DEPGOLD_NUM			78		//	저장소의 돈 가감
#define		MSG_GM_ADD_SKILL_NUM			79		//	스킬 추가
#define		MSG_GM_REM_SKILL_NUM			80		//	스킬 제거
#define		MSG_GM_EDIT_SKILL_NUM			81		//	스킬 에디트(스킬 step변화)
#define		MSG_GM_EDIT_MAINQUEST_TRG_NUM	82		//	실행중인 제룡행 퀘스트의 트리거를 바꿈
#define		MSG_GM_ADD_DOINGQUEST_NUM		83		//	실행중리스트에 퀘스트를 삽입
#define		MSG_GM_REM_DOINGQUEST_NUM		84		//	실행중리스트에서 퀘스트를 제거
#define		MSG_GM_EDIT_DOINGQUEST_TRG_NUM	85		//	실행중리스트의 퀘스트 트리거를 바꿈
#define		MSG_GM_REM_DONEQUEST_NUM		86		//	실행종료리스트에서 퀘스트를 삭제
#define		MSG_GM_EDIT_CONTRIBUTE_NUM		87		//	문파기여도 바꾸기

#define		MSG_GM_ADD_MASTERY_NUM			88		//	오의 추가
#define		MSG_GM_REM_MASTERY_NUM			89		//	오의 삭제
#define		MSG_GM_EDIT_MASTERY_NUM			90		//	오의 에디트(오의 포인트 변화)
#define		MSG_GM_EDIT_LEFTMASTERYPOINT_NUM	91	//	남은 오의 포인트 에디트
#define		MSG_GM_EDIT_CHARAC_ROLE_NUM			92		//	역할 바꾸기
#define		MSG_GM_EDIT_CHARAC_CLASSGRADE_NUM	93	//	역할등급 바꾸기

#define		MSG_GM_EDIT_MUTE_TIME_NUM			94		// 채팅 금지시키기
#define		MSG_GM_EDIT_LIMIT_CONNECTION_NUM	95		// 접속제한..
#define		MSG_GM_EDIT_JIN_NUM					96	// 진(경험치) 바꾸기

#define		MSG_GM_EDIT_MAINQUESTID_NUM			97	//제룡행 퀘스트 ID 바꾸기
#define		MSG_GM_EDIT_INTOXICATION_NUM		98	//중독 에디트
#define		MSG_GM_EDIT_ELIXIR_NUM				99	//영약 파라메터 에디트
#define		MSG_GM_EDIT_NICK_ID_NUM				100	// 별호 수정
#define		MSG_GM_COPY_CHARAC_NUM				101	// 캐릭터 복사


#define		MSG_GM_EDIT_COLLECTION_NUM			102	// 수집(명품) 데이터 에디트
#define		MSG_GM_EDIT_SERVER_EXPRATE_NUM		103	// 서버의 경험치 배율 조정
#define		MSG_GM_EDIT_INCHANTVALUE_NUM		104	// 아이템의 인첸트 값을 변경
//#define MSG_GM_EDIT_VAR_NO			105			// 에디트 대표 번호. 아래쪽.
#define		MSG_GM_EVENT_REWARD_REQ_NUM			106 // 이벤트 보상 요청
#define		MSG_GM_EVENT_REWARD_RES_NUM			107 // 이벤트 보상 결과

#ifdef _CHARACLIST_DELETE_CHARACTER_20061214_
	#define MSG_GM_DELETE_CHARAC_NUM			108		//캐릭터 삭제
#endif

#define MSG_GM_EDIT_BLOOD_POINT_NUM				109		//혈정포인트 수정

// 20070703 추가
#define MSG_GM_EDIT_ITEM_PERIOD_NUM				110		// 기행 아이템의 기간 설정.


// 20080803 추가.
#define MSG_GM_EVENT_NUM						(111)		// 이벤트 : 돌발 + 예약 + 예약 취소 + 예약 정보 + 예약 정보 요청.

#define MSG_GM_ORG_CTRL_NUM						(112)		// 영웅단 관련 메시지. 20081001


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 105 번 이하는 아래쪽에 Ray 가 추가합니다.


struct _gmsHeader {
	int			iKey;					//유효 확인 키
	u_char		ucMessage;				// MSG_NO_GSINFO_RES_GS_DS(5)
	u_int		uiTime;
	char		cGMName[13];

	_gmsHeader()
		: iKey(PASS_KEY)
	{
	}
};



struct MSG_GM_EDIT_COLLECTION
{
	_gmsHeader		header ;
	char			sCharacName[13];
	u_char			ucEditFlag;		//0: 대상 컬랙션을 완료化 해 주세요
									//1: 대상 컬랙션을 모두 0으로 클리어 해 주세요(완료->미완)
	u_short			usCollectionIndex;
};


struct	MSG_GM_EDIT_CHARAC_CLASSGRADE
{
	_gmsHeader	header;
	char				cCharacName[13];
	char				cClassGrade;

};	

//#define		MSG_GM_ADD_MASTERY_NUM			88		//	오의 추가
struct	MSG_GM_ADD_MASTERY
{
	_gmsHeader	header;
	char				cCharacName[13];
	short				sMasteryID;
	char				cPoint;
	char				cPage;
};
//#define		MSG_GM_REM_MASTERY_NUM			89		//	오의 삭제
struct	MSG_GM_REM_MASTERY
{
	_gmsHeader	header;
	char				cCharacName[13];
	short				sMasteryID;
};
//#define		MSG_GM_EDIT_MASTERY_NUM			90		//	오의 에디트(오의 포인트 변화)
struct	MSG_GM_EDIT_MASTERY
{
	_gmsHeader	header;
	char				cCharacName[13];
	short				sMasteryID;
	char				cPoint;
};
//#define		MSG_GM_EDIT_LEFTMASTERYPOINT_NUM	91	//	남은 오의 포인트 에디트
struct	MSG_GM_EDIT_LEFTMASTERYPOINT
{
	_gmsHeader	header;
	char				cCharacName[13];
	short				sPoint;
};
//#define		MSG_GM_EDIT_CHARAC_ROLE_NUM		92		//	역할 바꾸기
struct	MSG_GM_EDIT_CHARAC_ROLE
{
	_gmsHeader	header;
	char				cCharacName[13];
	char				cClass;
};

// vitual message
#define		MSG_GS_SHUTDOWN_NUM				(-1)		//  GS와 연결이 끊어짐..
struct MSG_GS_SHUTDOWN
{
	int		iKey ;
	char	cMessage ;
	int		iServerNum ;
};


// #define		MSG_SYSTEM_INFO_REQ_NUM			1	// 시스템 정보 요청 GMS->DS
struct MSG_SYSTEM_INFO_REQ {				//서버 시스템 정보 요청
	_gmsHeader	header;
	char	cServer_num;						//서버 번호
};

// #define		MSG_SYSTEM_INFO_RES_NUM			2	// 시스템 정보 응답 DS->GMS
struct MSG_SYSTEM_INFO_RES					// # UMSG_GSINFO_RES_GS_DS 와 같다..
{
	_gmsHeader			header;
	char				cServerNum;	

	_SERVER_SYSTEM_INFO		_ServerSystemInfo;				// 서버정보	
	_SERVER_NET_INFO_GMS	_ServerNetInfo;
	_SERVER_STATE_INFO		_ServerStateInfo;

	int					iCurLimitConnection ;
	bool				bServerState ;			// true : open, false : close 

	int					iLevelExp;
	int					iSkillExp;

};

#ifndef MAX_CHAT_LENGTH
#define MAX_CHAT_LENGTH		(81)
#endif
//#define		MSG_MESSAGE_DIRECT_NUM			9	// 공지사항 바로 때리기.. GMS->DS
struct MSG_MESSAGE_DIRECT {
	_gmsHeader	header;
	char	cServerNum;		// 99 is brodcasting

	char	cString[MAX_CHAT_LENGTH];
};


// #define		MSG_SYSTEM_OFF_REQ_NUM			10	// 시스템 종료 GMS -> DS
struct MSG_SYSTEM_OFF_REQ {
	_gmsHeader	header;
	char	cServer_num;	
};

struct MSG_SYSTEM_OFF_REQ_FOR_GS {
	int		iKey ;
	char	cMessage ;
	char	cServer_num;	
};

//#define		MSG_SYSTEM_TIME_REQ_NUM			11	// 시스템 시간 요청(시간 동기화) GMS->DS
struct MSG_SYSTEM_TIME_REQ {
	_gmsHeader	header;
};

//#define		MSG_SYSTEM_TIME_RES_NUM			12	// 시스템 시간 응답 DS->GMS
struct MSG_SYSTEM_TIME_RES {
	_gmsHeader	header;
	u_int		uiTime;

	u_short		usServerIndex;		//서버군 번호
};

//#define		MSG_GM_REQ_SERVER_PLAYER_NUM	13	// 각 지역별 동접 요청.
struct MSG_GM_REQ_SERVER_PLAYER
{
	_gmsHeader	header;
	u_int		uiTime;
};


//#define		MSG_GM_SERVER_PLAYER_NUM		14	// 각 서버별 동접수를 요청한다.
struct MSG_GM_SERVER_PLAYER 
{
	_gmsHeader	header;
	u_int		uiTime;

	u_char		ucGreoupNo;					// Group Inodex

	u_short		usTotalConnectCount;		// 서버군 동접.

	struct 
	{
		u_short		usPlayerCount;			// Count of Connected Player.
		u_short		usConnectCount;
	} Count[35];
};

// 캐릭터 위치 변경
//	#define		MSG_SETPOS_REQ_NUM				20	// 캐릭터 위치 정보 초기화 GMS -> DS
struct MSG_SETPOS_REQ {
	_gmsHeader	header;
	char	cCharacName[13];

	float	fX ;
	float	fZ ;
};

// #define		MSG_SETPOS_RES_NUM				21	// 캐릭터 위치정보 초기화 응답
struct MSG_SETPOS_RES {
	_gmsHeader	header;
	char	cCharacName[13];
	char	cResult;
};

//#define		MSG_CREATE_ITEM_REQ_NUM			22	// create item request
struct MSG_CREATE_ITEM_REQ {
	_gmsHeader	header;

	char	cFstType ;
	short	sScdType ;

	char	cPosType ;
	union {
		char	cCharacName[13] ;
		struct _pos {
			char	cServerNum ;
			float	fX ;
			float	fZ ;
		} pos ;		
	};
};

//#define		MSG_CREATE_ITEM_RES_NUM			23	// create item result
struct MSG_CREATE_ITEM_RES{
	_gmsHeader	header;

	char	cResult	;
};

//#define		MSG_CMD_USERKILL				31	// 사용자 강제 종료
struct MSG_CMD_USERKILL {
	_gmsHeader	header;
	char		cAccount[en_max_lil+1];
};

//#define		MSG_CMD_USERKILL_RESULT_NUM		32	// 사용자 강제 종료
struct MSG_CMD_USERKILL_RESULT {
	_gmsHeader	header;
	char		cAccount[en_max_lil+1];
	char		cResult;
};

//#define		MSG_CHARAC_REQ_NUM				41	// 캐릭터 정보 요청
struct MSG_CHARAC_REQ {
	_gmsHeader	header;
	char		cChrName[13];
};

//#define		MSG_CHARAC_RES_NUM				42	// 캐릭터 정보 응답
struct MSG_CHARAC_RES {
	_gmsHeader	header;
	char		cChrName[13];
	_CHARAC_BASIC			CHARAC_BASIC;			//ND_V01_Charac
	_CHARAC_CUR_BASIC		CHARAC_CUR_BASIC;		//ND_V01_CurBasicInfo
	_CHARAC_LEVEL			CHARAC_LEVEL;			//ND_V01_5StarLevel
	_CHARAC_STATE			CHARAC_STATE;			//ND_V01_CharacState
};

//#define		MSG_CHARAC_UPDATE_NUM			43	// 캐릭터 정보 업데이트
struct MSG_CHARAC_UPDATE {
	_gmsHeader	header;
	char		cChrName[13];
	_CHARAC_BASIC			CHARAC_BASIC;			//ND_V01_Charac
	_CHARAC_CUR_BASIC		CHARAC_CUR_BASIC;		//ND_V01_CurBasicInfo
	_CHARAC_LEVEL			CHARAC_LEVEL;			//ND_V01_5StarLevel
	_CHARAC_STATE			CHARAC_STATE;			//ND_V01_CharacState
};


//#define		MSG_USER_DEPOT_REQ_NUM			44	// 창고 정보 요청
struct MSG_USER_DEPOT_REQ {
	_gmsHeader	header;
	char		cAccount[en_max_lil+1];
};

//#define		MSG_USER_DEPOT_RES_NUM			45	// 창고 정보 응답
struct MSG_USER_DEPOT_RES {
	_gmsHeader	header;
	char		cAccount[en_max_lil+1];

	int			iSize;
	char		cNum;
	char		pInvData[UDP_PACKET_CUT_SIZE];
	//_USER_DEPOT stDepot ;
};

//#define		MSG_INVEN_REQ_NUM				46	// 인벤토리 정보 요청
struct MSG_INVEN_REQ {
	_gmsHeader	header;
	char		cCharacName[13] ;
};

//#define		MSG_INVEN_RES_NUM				47	// 인벤토리 정보 응답
struct MSG_INVEN_RES {
	_gmsHeader	header;
	char		cCharacName[13] ;

	int			iSize;
	char		cNum;
	char		pInvData[UDP_PACKET_CUT_SIZE];
	//_CHARAC_INVENTORY stInven ;
};

//#define		MSG_USER_DEPOT_UPDATE_NUM		48	// 창고 정보 업데이트
struct MSG_USER_DEPOT_UPDATE {
	_gmsHeader	header;
	char		cAccount[en_max_lil+1];
	_USER_DEPOT stDepot ;
};

//#define		MSG_INVEN_UPDATE_NUM			49	// 인벤토리 정보 업데이트
struct MSG_INVEN_UPDATE {
	_gmsHeader	header;
	char		cCharacName[13] ;
	_CHARAC_INVENTORY stInven ;
};

//#define		MSG_QUEST_REQ_NUM				50	// 퀘스트 요청
struct MSG_QUEST_REQ {
	_gmsHeader	header;
	char		cCharacName[13] ;
};
//#define		MSG_QUEST_RES_NUM				51	// 퀘스트 응답
struct MSG_QUEST_RES {
	_gmsHeader	header;
	char		cCharacName[13];

	int			iSize;
	char		cNum;
	char		pQuestData[300];
	//	_CHARAC_QUEST_DATA	CHARAC_QUEST_DATA ;
};
//#define		MSG_QUEST_UPDATE_NUM			52	// 퀘스트 업데이트
struct MSG_QUEST_UPDATE {
	_gmsHeader	header;
	char		cCharacName[13] ;

	_CHARAC_QUEST_DATA CHARAC_QUEST_DATA ;

};


//#define		MSG_GM_EDIT_MAINQUEST_TRG_NUM		82		//	실행중인 제룡행 퀘스트의 트리거를 바꿈
struct MSG_GM_EDIT_MAINQUEST_TRG	{
	_gmsHeader	header;
	char				cCharacName[13];		//UniqueUserID
	u_char				cTrigger;				//바뀔 트리거(ID는 상관없이 바꿔주세요~~)
};

//#define		MSG_GM_ADD_DOINGQUEST_NUM			83		//	실행중리스트에 퀘스트를 삽입
struct MSG_GM_ADD_DOINGQUEST		{
	_gmsHeader	header;
	char				cCharacName[13];		//UniqueUserID
	unsigned short		sQuestID;
	u_char				cQuestTrigger;
	int					iTime;					//퀘스트 시작시간
};


//#define		MSG_GM_REM_DOINGQUEST_NUM			84		//	실행중리스트에서 퀘스트를 제거
struct	MSG_GM_REM_DOINGQUEST	{
	_gmsHeader	header;
	char				cCharacName[13];		//UniqueUserID
	unsigned short		sQuestID;	
};

//#define		MSG_GM_EDIT_DOINGQUEST_TRG_NUM		85		//	실행중리스트의 퀘스트 트리거를 바꿈
struct	MSG_EDIT_DOINGQUEST_TRG	{
	_gmsHeader	header;
	char				cCharacName[13];		//UniqueUserID
	unsigned short		sQuestID;
	u_char				cQuestTrigger;
};

//#define		MSG_GM_REM_DONEQUEST_NUM			86		//	실행종료리스트에서 퀘스트를 삭제
struct	MSG_GM_REM_DONEQUEST	{
	_gmsHeader	header;
	char				cCharacName[13];		//UniqueUserID
	unsigned short		sQuestID;	
};


//#define		MSG_MARTIAL_REQ_NUM				52	// 무공 요청
struct MSG_MARTIAL_REQ {
	_gmsHeader	header;
	char		cCharacName[13] ;	
};
//#define		MSG_MARTIAL_RES_NUM				53	// 무공 응답
struct MSG_MARTIAL_RES {
	_gmsHeader	header;
	char		cCharacName[13] ;

	_CHARAC_MARTIAL_DATA CHARAC_MARTIAL_DATA ;
};
//#define		MSG_MARTIAL_UPDATE_NUM			54	// 무공 업데이트..
struct MSG_MARTIAL_UPDATE {
	_gmsHeader	header;
	char		cCharacName[13] ;

	_CHARAC_MARTIAL_DATA CHARAC_MARTIAL_DATA ;
};



//#define	MSG_NO_GM_EDIT_LEVEL_NUM						//	레벨 에디트
struct MSG_GM_EDIT_LEVEL	{
	_gmsHeader	header;
	char				cCharacName[13];
	int					iLevel;
};
//#define	MSG_NO_GM_EDIT_VITAL_NUM						//	생 에디트
struct MSG_GM_EDIT_VITAL	{
	_gmsHeader	header;
	char				cCharacName[13];
	short				sVital;
	unsigned int		uiHP;					//바뀌게 되는 체력
};
//#define	MSG_NO_GM_EDIT_ZEN_NUM							//	진 에디트
struct MSG_GM_EDIT_ZEN	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sZen;
	unsigned int		uiMP;					//바뀌게 되는 내력
};
//#define	MSG_NO_GM_EDIT_SOUL_NUM							//	지 에디트
struct MSG_GM_EDIT_SOUL	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sSoul;
	unsigned int		uiSoul;					//바뀌게 되는 영력
};
//#define	MSG_NO_GM_EDIT_HEART_NUM						//	민 에디트
struct MSG_GM_EDIT_HEART	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sDexterity;
};
//#define	MSG_NO_GM_EDIT_JUNGGI_NUM						//	근 에디트
struct MSG_GM_EDIT_JUNGGI	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sStr;
};
//#define	MSG_NO_GM_EDIT_LEFTPOINT_NUM					//	남은 포인트 에디트
struct MSG_GM_EDIT_LEFTPOINT	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sLeftPoint;
};
//#define	MSG_NO_GM_EDIT_ZONE_NUM							//	존 바꾸기
struct MSG_GM_EDIT_ZONE	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	int					iZone;
	float				fX;
	float				fZ;
};
//#define		MSG_GM_EDIT_CODE_NUM					//	코드 바꾸기
struct MSG_GM_EDIT_CODE	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	float				fX;
	float				fZ;
};
//#define	MSG_NO_GM_EDIT_PARTY_NUM						//	파티 바꾸기
struct MSG_GM_EDIT_PARTY	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	char				cParty;
};
//#define	MSG_NO_GM_EDIT_NAME_NUM							//	이름 바꾸기(같은 이름이 존재하는지 조사는 이미 했음)
struct MSG_GM_EDIT_NAME	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	char				Name[13];
};
//#define	MSG_NO_GM_EDIT_NICK_NUM							//	닉네임 바꾸기(같은 닉네임이 존재하는지 조사는 이미 했음)
struct MSG_GM_EDIT_NICK	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	char				Name[13];
};
//#define	MSG_NO_GM_ADD_INVITEM_NUM						//	인벤토리에 아이템 추가
/*struct MSG_GM_ADD_INVITEM	{
int					iKey;					//유효 확인 키
char				cMessage;				// MSG_NO_GSINFO_RES_GS_DS(5)
char				cCharacName[13];			//UniqueUserID
char				cFirstType;
char				cSecondType;
short				sItemID;
unsigned char		ucItemCount;
unsigned short		usDurability;
};*/


struct MSG_GM_ADD_INVITEM	
{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	unsigned char		ucItemCount;
	unsigned short		usDurability;
	unsigned char		ucSlotCount;			//아이템에 슬롯이 있는 경우 사용
	unsigned char		ucInchant;					//아이템의 제련값

#ifdef _PD_GM_ADDITEM_MODIFY_
	char				cCashCheck;				// 기간제 아이템 삽입을 위해 변경.
	u_short				usTimeValue;			// 기간제 아이템 삽입을 위해 변경.
#endif
};

//#define	MSG_NO_GM_REM_INVITEM_NUM						//	인벤토리에 아이템 제거
struct MSG_GM_REM_INVITEM	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	u_char				ucSlotNo;
};


struct MSG_GM_ADD_DEPITEM	{
	_gmsHeader	header;
	char				cAccount[en_max_lil+1];
	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	unsigned char		ucItemCount;
	unsigned char		ucSlotCount;			//아이템에 슬롯이 있는 경우 사용
	unsigned char		ucInchant;
#ifdef _PD_GMS_ADD_DEPOT_DUR_
	unsigned short		usDurability;
#endif

};




//#define	MSG_NO_GM_REM_DEPITEM_NUM						//	저장소에 아이템 제거

struct MSG_GM_REM_DEPITEM	{
	_gmsHeader	header;
	char				cAccount[en_max_lil+1];
	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	char				cSlotNo;
};

//#define	MSG_NO_GM_ADD_INVGOLD_NUM						//	인벤토리의 돈 가감
struct MSG_GM_ADD_INVGOLD	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	int					iGold;
};


//#define	MSG_NO_GM_ADD_DEPGOLD_NUM						//	저장소의 돈 가감
struct MSG_GM_ADD_DEPGOLD	{
	_gmsHeader	header;
	char				cAccount[en_max_lil+1];
	int					iGold;
};
//#define	MSG_NO_GM_ADD_SKILL_NUM							//	스킬 추가
struct MSG_GM_ADD_SKILL	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sSkillID;
	short				sSkillStep;
	int					iSkillExp;
};

//#define	MSG_NO_GM_REM_SKILL_NUM							//	스킬 제거
struct MSG_GM_REM_SKILL	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sSkillID;
};

//#define	MSG_NO_GM_EDIT_SKILL_NUM						//	스킬 에디트(스킬 st
struct MSG_GM_EDIT_SKILL	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sSkillID;
	short				sSkillStep;
	int					iSkillExp;
	char				cSkillState ;			// 수련이 필요 여부(0 완료, 1 필요)
};

//#define		MSG_GM_EDIT_GMCLASS_NUM			70		//	GM CLASS 바꾸기
struct MSG_GM_EDIT_GMCLASS	{
	_gmsHeader	header;
	char				cCharacName[13];
	char				cClass;
	int					iBlockingEndTime ;
};

//#define		MSG_GM_EDIT_CONTRIBUTE_NUM		87		//	문파기여도 바꾸기
struct	MSG_GM_EDIT_CONTRIBUTE	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	int					iContribute;
};

//#define		MSG_GM_EDIT_MUTE_TIME_NUM			94		// 채팅 금지시키기
struct MSG_GM_EDIT_MUTE_TIME
{
	_gmsHeader	header;
	char				cCharacName[13] ;

	int					iMuteTime ;		
};

//#define		MSG_GM_EDIT_LIMIT_CONNECTION_NUM	95		// 접속제한..
struct MSG_GM_EDIT_LIMIT_CONNECTION
{
	_gmsHeader	header;

	int					iLimitCount ;
	bool				bServerState ;		// t:open f: close
};

//#define		MSG_GM_EDIT_JIN_NUM					96	// 진(경험치) 바꾸기
struct MSG_GM_EDIT_JIN
{
	_gmsHeader	header;
	char		cCharacName[13];
	u_int		uiJin;
};

//#define		MSG_GM_EDIT_MAINQUESTID_NUM			97	//제룡행 퀘스트 ID 바꾸기
struct MSG_GM_EDIT_MAINQUESTID	{
	_gmsHeader		header;
	char			cCharacName[13];
	unsigned short	usQuestID;
	int				iTime;						//퀘스트 시작시간
};

//#define		MSG_GM_EDIT_INTOXICATION_NUM			98	//중독 에디트
struct MSG_GM_EDIT_INTOXICATION
{
	_gmsHeader		header;
	char			cCharacName[13];
	short			sIntoxication;
};
//#define		MSG_GM_EDIT_ELIXIR_NUM					99	//영약 파라메터 에디트
struct MSG_GM_EDIT_ELIXIR
{
	_gmsHeader		header;
	char			cCharacName[13];
	char			cElixirType;	//0 근력 //1 진기 //2 지혜 //3 건강//4 민첩
	char			cGrade;			//영약 등급
	short			asData[10];		//영약 데이터 배열
};



//#define		MSG_GM_EDIT_NICK_ID_NUM				100	// 별호 수정
struct MSG_GM_EDIT_NICK_ID
{
	_gmsHeader		header;
	char			cCharacName[13];
	short			sSlotNum ;				// 수정할 슬롯
	u_short			usNickID;				// 별호 아이디(0 이면 삭제)
};

//#define		MSG_GM_COPY_CHARAC_NUM				101	// 캐릭터 복사
struct MSG_GM_COPY_CHARAC
{
	_gmsHeader		header;
	char			cSourceCharacName[13];
	char			cTargetCharacName[13] ;
};

//#define		#define		MSG_GM_EDIT_SERVER_EXPRATE_NUM		103	// 서버의 경험치 배율 조정
struct MSG_GM_EDIT_EXPRATE
{
	_gmsHeader		header;

	char			cServerNum;
	int				iLevelExp;
	int				iSkillExp;
};


//#define		MSG_GM_EDIT_INCHANTVALUE_NUM	104	// 아이템의 인첸트 값을 변경
struct MSG_GM_EDIT_INCHANTVALUE
{
	_gmsHeader		header;

	unsigned char		ucWhere;	//0: Inven, 1: Depot

	union{
		char			sCharacName[13];	//ucWhere가 Inven일 경우 사용
		char			cAccount[en_max_lil+1];
	};

	short				sSlotNumber;
	unsigned __int64    ui64UniqueId;
	unsigned char		ucInchantValue;
};


// 주의  : #define		UMSG_LOGIN_LS_DS_NUM			121 번호 잊지 말것.
// 105번 부터 여기에서 정의함.
#define MSG_GM_EDIT_VAR_NO			105			// 에디트 대표 번호.
struct MSG_GM_EDIT_VAR
{
	_gmsHeader		header ;

	u_char			ucMode ;			// 모드에 따라, 적용한다.
};

enum
{
	en_gms_ds_edit_mode_honor=0,		// 명성
	en_gms_ds_edit_mode_fame,			// 악명
};

struct MSG_GM_EDIT_HONOR : public MSG_GM_EDIT_VAR
{
	char				cCharacName[13];			//UniqueUserID
	int					iValue;
};

typedef MSG_GM_EDIT_HONOR		MSG_GM_EDIT_FAME		;


//#define		MSG_GM_EVENT_REWARD_REQ_NUM				106 // 이벤트 보상 요청
//#define		MSG_GM_EVENT_REWARD_RES_NUM				107 // 이벤트 보상 결과

struct MSG_GM_EVENT_REWARD_REQ
{
	_gmsHeader			 header;

	int					iEventID;                          //이벤트 아이디
	char				pCharacName[13];         //캐릭명

	char				cItemType;                       //아이템타입
	char				cItemSecondType;
	short				sItemID;                            //아이템 아이디
	short				sItemCount;                     //아이템 갯수

	unsigned short		usDurability;                     //내구력
	unsigned char		ucSlotCount;                    //아이템에 슬롯이 있는 경우 사용
	unsigned char		ucInchant;                       //아이템의 제련값

	int					iResult;                                          //결과 0) 미처리, 1) 성공, 2) 실패
};


struct MSG_GM_EVENT_REWARD_RES
{
	_gmsHeader			header;

	int					iEventID;             //이벤트 아이디
	int					iResult;  //결과 0) 미처리, 1) 성공, 2) 실패
};

#ifdef _CHARACLIST_DELETE_CHARACTER_20061214_
//#define MSG_GM_DELETE_CHARAC_NUM					108	//캐릭터 삭제
struct MSG_GM_DELETE_CHARAC{
	_gmsHeader	header;
	char 	pCharacName[13];//캐릭명
};
#endif


//#define MSG_GM_EDIT_BLOOD_POINT_NUM                                                               109//혈정포인트 수정
struct MSG_GM_EDIT_BLOOD_POINT
{
	_gmsHeader			header;
	char				szCharacName[13];
	short				sBloodPoint;                                 // 수정할 혈정포인트
};


//#define MSG_GM_EDIT_ITEM_PERIOD_NUM			110		// 기행 아이템의 기간 설정.
struct MSG_GM_EDIT_ITEM
{
	_gmsHeader			header;
	u_char				ucMode;
};


enum
{
	en_gm_edit_item_aius=0,						// 구룡천하 기간 변경.
	en_gm_edit_item_edit_item,					// 아이템 변경
	en_gm_edit_item_edit_period,				// 아이템의 기간 변경 : 다른 속성은 변경하지 않는다.
	en_gm_edit_item_del_period,					// 아이템의 기간 설정을 없앤다.
	en_gm_edit_item_edit_skintear,				// 용린/용루 변경.
};

struct MSG_GM_EDIT_ITEM_AIUS : public MSG_GM_EDIT_ITEM
{
	// 패키지 정보 구조체
	char							szAccount[en_max_lil+1];		// 계정의 길이.
	_ACCOUNT_ITEM_USE_STATUS		aius;
};

struct MSG_GM_EDIT_ONE_ITEM : public MSG_GM_EDIT_ITEM
{
	char						szCharacName[en_charac_name_length+1];

	u_char						slot;		// 이 슬롯의 
	_sServer_InvenItem_v1		item;		// 아이템을 변경한다.
};

struct MSG_GM_EDIT_ITEM_PERIOD : public MSG_GM_EDIT_ITEM
{
	char						szCharacName[en_charac_name_length+1];

	u_char						slot;
	__inven_uselimit_info_v1	info;		// 아이템의 기간 부분만 변경한다.
};

struct MSG_GM_EDIT_DEL_PERIOD : public MSG_GM_EDIT_ITEM
{
	char						szCharacName[en_charac_name_length+1];

	u_char						slot;
};

// en_gm_edit_item_edit_skintear
struct MSG_GM_EDIT_EDIT_SKINTEAR : public MSG_GM_EDIT_ITEM
{
	char							szCharacName[en_max_lil+1];

	u_char							ucWhere;	// 0 Inven, 1 Depot
	u_char							slot;
	__inven_uselimit_info_v1		info;		// Protect 부분만 변경한다.
};


//#define MSG_GM_EVENT_NUM					(111)		// 이벤트 : 돌발 + 예약 + 예약 취소 + 예약 정보 + 예약 정보 요청.
struct MSG_GM_EVENT 
{
	_gmsHeader			header;
	u_char				ucMode;
};

enum {
	en_gm_event_instant_buff=0,					// 돌발 이벤트 : 흑도/백도/모두 로 구분해서 요청가능.
	en_gm_event_request_reserved_events=10,		// 현재 예약된 이벤트 내용을 요청한다.
	en_gm_event_reserved_events_info,			// 예약된 이벤트들을 알려준다.
	en_gm_event_reserve_event,					// 예약하기.
	en_gm_event_cancel_event,					// 취소하기.

};

//en_gm_event_instant_buff
struct MSG_GM_EVENT_INSTANT_BUFF : public MSG_GM_EVENT
{
	u_char			ucTarget;		// 0 - all, 1 - white side, 2 - black side

	short			buffIndex;		// 버프 스킬의 인덱스
	char			buffStep;		// 버프의 성.( 0 - 11 )

	u_char			group;			// 이건 그냥 확인용으로 넣는다.
	bool			TargetGS[23];	// true/false.
};


//#define MSG_GM_ORG_CTRL_NUM					(112)		// 영웅단 관련 메시지.
// 20081001 오전.
struct MSG_GM_ORG_CTRL
{
	_gmsHeader			header;					//
	u_char				ucMode;					//
};

// mode
enum
{
	en_org_ctrl_charac_reset=0,				// 캐릭터의 영웅단 정보를 초기화(탈퇴)한다.
	en_org_ctrl_entrust_captain=1,			// 위임.
	en_org_ctrl_kick=2,						// 탈퇴
	en_org_ctrl_destroy_org=3,				// 영웅단 없애기.

	en_org_ctrl_notice_org_destroyed=4,		// 영웅단이 해체되었다.
};


// en_org_ctrl_charac_reset=0,				// 캐릭터의 영웅단 정보를 초기화(탈퇴)한다.
struct MSG_GM_ORG_CHARAC_RESET : public MSG_GM_ORG_CTRL
{
	char			cCharacName[en_charac_name_length+1];
	int				iCuid;									// 확인용

	int				iOrgIndex;								//
};

// en_org_ctrl_entrust_captain=1,			// 위임.
struct MSG_GM_ORG_ENTRUST_CAPTAIN : public MSG_GM_ORG_CTRL
{
	char			cNewName[en_charac_name_length+1];		// 

	int				iOrgIndex;								//
};

// en_org_ctrl_kick=2,						// 탈퇴
struct MSG_GM_ORG_KICK : public MSG_GM_ORG_CTRL
{
	//
	char			cCharacName[en_charac_name_length+1];		// 

	int				iOrgIndex;								//
};

// en_org_ctrl_destroy_org=3,				// 영웅단 없애기.
struct MSG_GM_ORG_DESTROY_ORG : public MSG_GM_ORG_CTRL
{
	//
	int				iOrgIndex;			// 영웅단을 해체한다.
};

// en_org_ctrl_notice_org_destroyed=4,		// 영웅단이 해체되었다.
struct MSG_GM_ORG_NOTICE_ORG_DESTROYED : public MSG_GM_ORG_CTRL
{
	int				iOrgIndex;
	char			cOrgName[en_or_name_length+1];
};


#pragma pack( pop, enter_udp_packet )
