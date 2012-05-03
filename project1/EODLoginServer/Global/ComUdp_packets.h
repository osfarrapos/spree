#pragma once

#pragma pack( push, enter_game )
#pragma pack(1)



/*==========================================================================================]
[				GS MSG 패킷 MESSAGE 번호													]
[===========================================================================================]
[	설명 : DS->GS  패킷은 U 로 시작한다..													]
[			GSM 관려 메시지 번호와 중복을 방지하기 위해 GMS->GS 메시지 번호를 100 번부터	]
[			시작합니다. UMSG_SHOWTEXT_REQ_DS_GS(공지사항) 메시지에 대해서 결과 Result 를	]
[			받을지 말지 결정을 못했습니다.(안받아도 될꺼같아서..) 몬스터 정보 보기라든가	]
[			GS 정보 요청에 대한 MSG 가 몇개 더 추가될 예정입니다. 나중에 추가된건만 따로	]
[			모아서 다시 보내드리겠습니다.	확인하시고 피드백 주세요.						]
[											jkHong Add...									]
[==========================================================================================*/

//[to GS 관련]===============================================================================================

// DS -> GS
#define		UMSG_NO_GSINFO_REQ_DS_GS		100	// GS에 서버 정보 요청
#define		UMSG_NO_GSINFO_RES_GS_DS		101	// 서버 정보 응답 *
#define		UMSG_NO_USER_LOGOUT_DS_GS		102	// 강제 로그아웃
#define		UMSG_SHOWTEXT_REQ_DS_GS			103	// GS에 공지사항 출력 요청
#define		UMSG_SHOWTEXT_RES_GS_DS			104	// 공지사항 출력 결과 *
#define		UMSG_SYSTEM_OFF_REQ_NUM			110	// 시스템 종료 DS -> GS

#define		UMSG_NO_SETPOSITION_REQ			119	// 캐릭터 위치 초기화


//#define UMSG_NO_GSINFO_REQ_DS_GS				100	// 비 주기적인 요청 패킷이다..
struct UMSG_GSINFO_REQ_DS_GS					// # DS->GS GS에 서버 정보를 요청한다.
{
	int					iKey;					//유효 확인 키
	char				cMessage;				//메세지 번호		
	char				cServerNum;				//서버 번호(헤더 정보 규칙을 맞추기 위해..)
};


//#define UMSG_NO_GSINFO_RES_GS_DS				101	//주기적 응답, 또는 비 주기적 요청에 대한 응답
struct UMSG_GSINFO_RES_GS_DS					// # GS->DS GS의 서버 정보를 DS 에 알린다(UPDATE)
{
	int					iKey;					//유효 확인 키
	char				cMessage;				// MSG_NO_GSINFO_RES_GS_DS(5)
	char				cServerNum;

	char				cState;					// 서버의 상태
	int					iConnectCount;			//들어온 유저
	int					iPlayingCount;			//게임중인 유저

	struct  _ServerInfo{						// 서버 정보		
		char		cServerName[13];
		char		cOS[16];					// 사용 OS
		char		cIP[16];					// 사용 IP
		char		cCPU;						// CPU 사용률
		int			iTotalMem;					// 총 메모리
		int			iUseMem;					// 사용중인 메모리
		int			iTotalHDD;					// 총 디스크 용량
		int			iUseHDD;					// 사용중인 디스크 용량
	} SVR_INFO;
};

// #define		UMSG_NO_USER_LOGOUT_DS_GS		102		// 강제 로그아웃
struct UMSG_USER_LOGOUT_DS_GS
{
	int					iKey;					//유효 확인 키
	char				cMessage;				//메세지 번호	
	char				cServerNum;				//서버번호 (헤더정보 규칙을 맞추기 위해..)

	char				cAccount[en_max_lil+1];			//사용자 계정
	char				cReason;				//이유

	// 이 패킷은 예외적으로 uid를 포함하지 않아도 됨.
	// 0 : 다른 곳에서 로그인
	// 1 : 운영자에의한 로그아웃
	// 2 : 제제( 접금... )	
};


// #define		UMSG_SHOWTEXT_REQ_DS_GS			103	// GS에 공지사항 출력 요청
struct UMSG_SHOWTEXT_REQ{
	int			iKey;
	char		cMessage;	
	char		cServerNum;

	char		cString[51];
};

// #define		UMSG_SHOWTEXT_RES_GS_DS			104	// 공지사항 출력 결과
struct UMSG_SHOWTEXT_RES{
	int			iKey;
	char		cMessage;	
	char		cServerNum;

	char		cResult;
};

// #define		MSG_SYSTEM_OFF_REQ_NUM			110	// 시스템 종료 DS -> GS
struct UMSG_SYSTEM_OFF_REQ {
	int		iKey;
	char	cMessage;
	char	cServer_num;	
};

struct UMSG_SETPOSITION_REQ{
	int			iKey;
	char		cMessage;
	char		cServerNum;
	char		cCharacName[13];	// 캐릭터 이름(계정으로 해야하나? 이팀장님 피드백 주세요.)
};





/*==========================================================================================]
[				LS MSG 패킷 MESSAGE 번호													]
[===========================================================================================]
[	설명 : DS<->LS  패킷은 U 로 시작한다..													]
[			다른 UDP 관려 메시지 번호와 중복을 방지하기 위해 LS->DS 메시지 번호를 200 번부터]
[			시작합니다.																		]
[											jkHong Add...									]
[==========================================================================================*/
#define		UMSG_LOGIN_LS_DS_NUM				121
#define		UMSG_LOGIN_DS_LS_RESULT_NUM		122
#define		UMSG_LOGIN_INFO_REQ_LS_DS_NUM	123
#define		UMSG_LOGIN_INFO_RES_DS_LS_NUM	124
#define		MSG_CMD_USERKILL_NUM			31	// 사용자 강제 종료

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
	int			iTempID ;				//  LS에서 부여한 UID
	u_int		iIp;					//	접속자의  IP
	int			iTime;					//	접속 시도 시간
	char		cClass ;
	
#ifdef _PD_VINA_PCROOM_INFO_
	u_char		ucPCRoomInfo;			// 0 - None, 1 ~. PC Bang grade( use 1 only )
#endif
	
#ifdef _PD_COMPANY_METEL_
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
	char		cForestNo ;				// Forest Index number

	char		cAccount[en_max_lil+1];			//	계정
	u_short		usUid;					//	DS 에서 부여받은 uid
	int			iTempID ;				//  LS에서 부여한 UID
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


//#define		UMSG_LOGIN_INFO_RES_DS_LS_NUM	124
struct UMSG_LOGIN_INFO_RES_DS_LS
{
	int			iKey ;
	char		cMessage ;
	char		cForestNum ;

	char		cForestState ;	
		//#define  _FOREST_STATE_RUNNING = 0			// 정상 작동중
		//#define  _FOREST_STATE_ISFULL = -1			// 서버 포화상태
		//#define  _FOREST_STATE_CHECKUP_ = -2			// 서버군 점검중
};


struct _gmsHeader {
	int			iKey;					//유효 확인 키
	char		cMessage;				// MSG_NO_GSINFO_RES_GS_DS(5)
	u_int		uiTime;
	char		cGMName[13];
};


//#define		MSG_CMD_USERKILL				31	// 사용자 강제 종료
struct MSG_CMD_USERKILL {
	_gmsHeader	header;
	char		cAccount[en_max_lil+1];			//	계정
};


/*==========================================================================================]
[				PING MESSAGE PACKET															]
[===========================================================================================]
[	설명 : 서버의 실행 여부를 판단하기 위해.. 모든 서버에 쓰이는 PING PACKET 입니다. 		]
[											jkHong Add...									]
[==========================================================================================*/
#define		UMSG_NO_PING			0 		// 서버는 이패킷을 받으면 바로 리턴해주기만 하면 된다.
struct	UMSG_PING
{
	int			iKey ;
	char		cMessage ;
	int			iIndex ;					//	순차 증가 (패킷을 구분하기 위해서)
	DWORD		time ;						//	= GetTickCount() , 패킷을 보낸 시간 (이것으로 양방향 속도를 구한다.)
};


#pragma pack( pop, enter_game )