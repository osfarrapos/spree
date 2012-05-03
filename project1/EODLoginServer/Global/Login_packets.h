#pragma once 

#pragma pack( push, enter_Login_Packet )
#pragma pack(1)

#define _COUNT_MAX_GROUP_				122

#ifdef _PD_UNI_SERVER_

	// 서버군 번호의 마지막 2개의 인덱스를 사용.
	#define _GR_INDEX_TRAIN_				120
	#define _GR_INDEX_UNION_				121

#else

	#define _GR_INDEX_TRAIN_				17

#endif



#define MSG_NO_DEQUENCE_NUMBER			0
#define MSG_NO_LOGIN					1
#define MSG_NO_GMLOGIN					124

#define MSG_NO_LOGIN_SUCCESS			3
#define MSG_NO_LOGIN_FAIL				4
#define MSG_NO_LOGIN_FOREST_LIST		5		// ps 에 접속하면 이 패킷을 보내주고 연결을 끊는다.
#define MSG_NO_SET_VERSION				100

struct _sHeader {
	short		sLength : 12 ;
	short		sCrypto : 2 ;
	short		sCompressed : 2 ;
};

//#define MSG_NO_DEQUENCE_NUMBER			0
struct MSG_SEQUENCE_NUMBER
{
	_sHeader			Header ;
	u_char				cMessage ;
	u_char				ucStartSeq ;
};

#define _PASSPORT_LENGTH_		(96)

struct MSG_LOGIN
{
	_sHeader			Header ;
	u_char				ucMessage ;

	u_int				uiLMBC_Code ;		// Login Mode by Company

	u_short				sVersionIndex ;
	DWORD				dwClientVer ;
	char				cForestSelectNum ;		// 선택 군

	char				passport[_PASSPORT_LENGTH_] ;
} ;

//#define  _FOREST_STATE_RUNNING				0					// 정상 작동중
//#define  _FOREST_STATE_OFF					-1					// 서비스 중지
//#define  _FOREST_STATE_CHECKUP_				-2					// 서버군 점검중
//#define  _FOREST_STATE_FULL_				-3					// 포화상태
struct _ForestInfo 
{
	int		nVersion;
	DWORD	dwClientVer ;
	bool	bServerRun ;
	char	cForestState[_COUNT_MAX_GROUP_] ;			//	-3, 포화상태 -2, 점검중 -1 : 오프중 ,  0~100 정상	
};



//#define MSG_NO_LOGIN_FOREST_LIST		5
struct MSG_LOGIN_FOREST_LIST
{	
	struct _ForestInfo_packet 
	{
		int		nVersion;		
		bool	bServerRun ;
		char	cForestState[_COUNT_MAX_GROUP_] ;			//	-3, 포화상태 -2, 점검중 -1 : 오프중 ,  0~100 정상	
	} ForestInfo ;	
};


//#define MSG_NO_LOGIN_SUCCESS					3
struct MSG_LOGIN_SUCCESS
{
	_sHeader			Header ;

	u_char				cMessage ;
	
	u_short				usServerPort ;		// 접속할 GS Port
	u_long				ulServerIP ;		// 접속할 GS IP

	u_short				usMessengerPort ;	// 접속할 Messenger Server Port
	u_long				ulMessengerIP ;		// 접속할 Messenger Server IP

	u_short				usUniqueID ;		// 이 값을 게임 내내 유지한다.

	char				cNDAccount[en_max_lil+1] ;	// 이 값으로 게임서버 들어오면 됩니다. SV_ENTER_GAME
} ;



// Login Err code
#define _LOGIN_ERROR_CODE_BADUSER			1	// 접속이 허용된 계정이 아닙니다.
#define _LOGIN_ERROR_CODE_BLOCK				2	// 해당 계정은 사용이 중지 되었습니다.
#define _LOGIN_ERROR_CODE_OVERTIME			3	// 사용량이 많아 접속이 원활하지 않습니다.
#define _LOGIN_ERROR_CODE_BADPASSWORD		4	// 비밀번호가 일치하지 않습니다.
#define _LOGIN_ERROR_CODE_ALREADY			5	// 이미 접속중이거나 접속 완료가 원활히 이루어지지 않은 계정입니다.
#define _LOGIN_ERROR_CODE_CHECKUP			6	// 테스트 시간이 아닙니다.(서버 점검중입니다.)
#define _LOGIN_ERROR_CODE_OFF				7	// 서버 점검중입니다.(로그인서버가 꺼져있을경우)
#define _LOGIN_ERROR_CODE_NO_USER			8	// 없는 사용자
#define _LOGIN_ERROR_CODE_TEEN_USER			9	// 연령(15세 미만) 제한자.
#define _LOGIN_ERROR_CODE_BADVERSION		10	// 버젼이 다름
#define _LOGIN_ERROR_CODE_SERVERISFULL		12	// 선택한 서버군은 이미 꽉 차버렸음..
#define _LOGIN_ERROR_CODE_ETC				13	// 기타 오류..
#define _LOGIN_ERROR_CODE_BILL_NO_INFO		15	// 인증정보 없음, 상품구매 바람
#define _LOGIN_ERROR_CODE_BILL_NO_MONEY		16	// 잔액 부족
#define _LOGIN_ERROR_CODE_NOT_LMBC_CODE		17	// 지역 코드( 회사 코드 ) 이상
#define _LOGIN_ERROR_CODE_NOT_UNI_NEXON		18	// 넥슨 통합 계정이 아닙니다. 먼저 통합 바람.
#define _LOGIN_ERROR_CODE_LOGIN_FAIL		19	// 로그인 실패(ID or Pass 중 뭐가 틀렸는지 모름)
#define _LOGIN_ERROR_CODE_NOT_PCBANG_USER	20	// PC방 유저 아님
enum {
	_LOGIN_ERROR_CODE_METEL_E00 = 21,			// Can't open aplist.txt
	_LOGIN_ERROR_CODE_METEL_E01,				// Invalid data in aplist.txt
	_LOGIN_ERROR_CODE_METEL_E02,				// Fail to connect to SQLAP
	_LOGIN_ERROR_CODE_METEL_E03,				// Fail to create event
	_LOGIN_ERROR_CODE_METEL_E04,				// Calling API before InitBilling
	_LOGIN_ERROR_CODE_METEL_E05,				// Bad message format
	_LOGIN_ERROR_CODE_METEL_E06,				// Timeout for SQLAP response
	_LOGIN_ERROR_CODE_METEL_E07,				// Fail to send message to SQLAP
	_LOGIN_ERROR_CODE_METEL_E08,				// Too many user login, max 320000
	_LOGIN_ERROR_CODE_METEL_E09,				// IP address is incorrect
	_LOGIN_ERROR_CODE_METEL_E10,				// Port value is out of range
	_LOGIN_ERROR_CODE_METEL_E11,				// Fail to create thread
	_LOGIN_ERROR_CODE_METEL_E12,				// API is already running
	_LOGIN_ERROR_CODE_METEL_E13,				// Crypt key has not been set
	_LOGIN_ERROR_CODE_METEL_E14,				// Message queue is full
	_LOGIN_ERROR_CODE_METEL_E15,				// SQLAP is dead
	_LOGIN_ERROR_CODE_METEL_E16,				// Checksum error in received message
	_LOGIN_ERROR_CODE_METEL_E17,				// Crypt key is too long or too short

	_LOGIN_ERROR_CODE_METEL_E50,/*39*/			// SQL Error
	_LOGIN_ERROR_CODE_METEL_E51,				// AP exception
	_LOGIN_ERROR_CODE_METEL_E52,				// Incorrect ID or Password
	_LOGIN_ERROR_CODE_METEL_E53,				// User has not registered the game
	_LOGIN_ERROR_CODE_METEL_E54,				// This account is forbidden
	_LOGIN_ERROR_CODE_METEL_E55,				// Points is not enough to play
	_LOGIN_ERROR_CODE_METEL_E56,				// The game registration is forbidden
	_LOGIN_ERROR_CODE_METEL_E57,				// Can't find the LoginSN in DB

	_LOGIN_ERROR_CODE_METEL_UNKNOWN/*47*/
};
#define _LOGIN_ERROR_CODE_BLOCK_CODE_1		101	// 영구블럭
#define _LOGIN_ERROR_CODE_BLOCK_CODE_2		102	// 계정압류
#define _LOGIN_ERROR_CODE_BLOCK_CODE_3		103	// 임시블럭



// 로그인 실패 
//#define MSG_NO_LOGIN_FAIL						4
struct MSG_LOGIN_FAIL
{
	_sHeader			Header ;

	u_char				cMessage ;
	u_char				ucErrorCode ;	// Login Err code
} ;




struct MSG_SET_VERSION {
	_sHeader			Header ;

	u_char				cMessage ; 
	u_short				usVersion ;
	DWORD				dwClientVer ;	

	char				cIsTest ;			// 0. 정식, 1. 테스트
};

#pragma pack( pop, enter_Login_Packet )