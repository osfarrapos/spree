#pragma once

#pragma pack(push, enter_login)
#pragma pack(1)

struct sHeader;

#define MSG_NO_LOGIN					1
#define MSG_NO_LOGIN_SUCCESS			3
#define MSG_NO_LOGIN_FAIL				4

#ifdef _XADMINISTRATORMODE
#define MSG_NO_GMLOGIN					124
#endif

#define MSG_NO_SEQUENCE_NUMBER 0 

#define _PASSPORT_LENGTH_NEXON_		(96)

struct MSG_SEQUENCE_NUMBER 
{ 
	_sHeader Header;
	u_char ucMessage;

	u_char ucStartSeq;
};

// 로그인 관련 : 1-5
// 로그인 C->S
//#define MSG_NO_LOGIN						1



#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

enum LOGIN_MODE_BY_COMPANY
{
	en_lmbc_nexon	= 0xa04b93f2,
#ifdef _PREOBVERSION 		
	en_lmbc_metel	= 0x3d732b0f,
#else
	en_lmbc_metel	= 0x3d732b0e, 
#endif
	en_lmbc_vina	= 0x8c203d51,
	en_lmbc_acclaim	= 0xef6c9aa1,
	en_lmbc_pwc		= 0xef6c9aa2,
	en_lmbc_rocworks= 0xef6c9aa3,
	en_lmbc_russia = 0xad890bd0,
};

enum LOGIN_ID_LENGTH		// Login ID Length
{
	en_lil_nexon	= 24,
	en_lil_metel	= 24,
	en_lil_vina		= 24,
	en_lil_acclaim	= 24,
	en_lil_pwc		= 24,
	en_lil_rocworks	= 24,
	en_lil_playten	= 24,

	en_max_lil = 24,
};

enum LOGIN_PASSWORD_LENGTH
{
	en_lpl_nexon	= 24,
	en_lpl_metel	= 24,
	en_lpl_vina		= 24,
	en_lpl_acclaim	= 32,
	en_lpl_pwc		= 32,
	en_lpl_rocworks	= 32,
	en_lpl_playten  = 32,

#ifdef _XENGLISH
	en_max_lpl = 32,
#else
	
	#ifdef _XJAPANESE
		en_max_lpl = 32,
	#else
		#ifdef _XRUSSIAN
			en_max_lpl = 32,
		#else
			en_max_lpl = 24,
		#endif
	#endif

#endif
};


/*
#ifdef _XDEF_NEXONLOGIN

struct MSG_LOGIN
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_int				uiLMBC_Code ;		// Login Mode by Company
	
	u_short				sVersionIndex ;
	DWORD				dwClientVer ;
	char				cForestSelectNum ;		// 선택 군
} ;

struct MSG_LOGIN_NEXON : public MSG_LOGIN
{
	char				passport[_PASSPORT_LENGTH_NEXON_] ;
} ;

#else
*/

#define _PASSPORT_LENGTH_                  (96)

#define _PD_LOGIN_PACKET_MODE_

#ifdef _PD_LOGIN_PACKET_MODE_
struct MSG_LOGIN
{	
	_sHeader    header;	
	u_char      ucMessage;	
	
	u_int       uiLMBC_Code;        // Login Mode by Company	
	
	u_short     sVersionIndex;	
	DWORD       dwClientVer;
	
	char        cForestSelectNum;   // 선택 군	
	char        passport[_PASSPORT_LENGTH_];
};

#else

//#define MSG_NO_LOGIN  1

struct MSG_LOGIN 
{
	_sHeader			header;
	u_char				ucMessage;

	char				cID[13];			//	아이디
	char				cPassword[13];		//	패스워드
	short				sVersionIndex;		//  Patch number
	DWORD				dwClientVer;		//  Client program version
	char				cForestSelectNum ;	//  선택 군
};

#endif

// 로그인 결과 : S->C
// 성공 : 이동할 서버의 아이피, 포트 가 유효.
//#define MSG_NO_LOGIN_SUCCESS				3
/*
struct MSG_LOGIN_SUCCESS
{
	_sHeader			header;
	u_char				ucMessage;

	unsigned short		usServerPort;		// 클라이언트 htons 로 바꿔서 사용한다
	unsigned long		ulServerIP;			// 클라이언트 이값을 그대로 사용한다

	unsigned short		usMessengerPort;
	unsigned long		ulMessengerIP;

	u_short				usUniqueID;			// 이 값을 게임 내내 유지한다. 바뀐 데이타가 2번 이상 오면 끊는다. 

#ifdef _XDEF_NEXONLOGIN
	char				cNDAccount[en_lil_nexon+1];	// 이 값으로 게임서버 들어오면 됩니다. SV_ENTER_GAME
#endif
};
*/

//#define MSG_NO_LOGIN_SUCCESS                                                             3

struct MSG_LOGIN_SUCCESS
{	
	_sHeader            Header;	
	u_char              cMessage;
		
	u_short             usServerPort;       // 접속할 GS Port
	u_long              ulServerIP;         // 접속할 GS IP
		
	u_short             usMessengerPort;    // 접속할 Messenger Server Port	
	u_long              ulMessengerIP;      // 접속할 Messenger Server IP
		
	u_short             usUniqueID;         // 이 값을 게임 내내 유지한다.
		
#ifdef _PD_LOGIN_PACKET_MODE_	
	char                cNDAccount[en_max_lil+1] ;  // 이 값으로 게임서버 들어오면 됩니다. SV_ENTER_GAME
#endif	
};



// 로그인 실패
#define MSG_ERROR_CODE_VERSION_FAILED		10
//#define MSG_NO_LOGIN_FAIL					4
struct MSG_LOGIN_FAIL 
{
	_sHeader			header;
	u_char				ucMessage;

	unsigned char		ucErrorCode;		// 에러 코드 - 10 : 버전정보가 다름
};


// login error code
/*
#define _LOGIN_ERROR_CODE_BADUSER			1	// 로그인 할 수 없는 유저 (없는 유저 포함)
#define _LOGIN_ERROR_CODE_OVERTIME			3	// 제한된 로그인 시간을 넘김
#define _LOGIN_ERROR_CODE_BADPASSWORD		4	// 패스워드 틀림
#define _LOGIN_ERROR_CODE_ALREADY			5	// 이미 로그인중
#define _LOGIN_ERROR_CODE_BADVERSION		10	// 버젼이 다름
#define _LOGIN_ERROR_CODE_SERVERISFULL		12	// 선택한 서버군은 이미 꽉 차버렸음..
#define _LOGIN_ERROR_CODE_ETC				13	// 기타 오류..
*/
#define _LOGIN_ERROR_CODE_BADUSER		1	// 접속이 허용된 계정이 아닙니다. 
#define _LOGIN_ERROR_CODE_BLOCK			2	// 해당 계정은 사용이 중지 되었습니다. 
#define _LOGIN_ERROR_CODE_OVERTIME		3	// 사용량이 많아 접속이 원활하지 않습니다. 
#define _LOGIN_ERROR_CODE_BADPASSWORD	4	// 비밀번호가 일치하지 않습니다. 
#define _LOGIN_ERROR_CODE_ALREADY		5	// 이미 접속중이거나 접속 완료가 원활히 이루어지지 않은 계정입니다. 
#define _LOGIN_ERROR_CODE_CHECKUP		6	// 테스트 시간이 아닙니다.(서버 점검중입니다.) 
#define _LOGIN_ERROR_CODE_OFF			7	// 서버 점검중입니다.(로그인서버가 꺼져있을경우) 
#define _LOGIN_ERROR_CODE_NO_USER		8	// 없는 사용자 
#define _LOGIN_ERROR_CODE_TEEN_USER		9	// 연령(15세 미만) 
#define _LOGIN_ERROR_CODE_BADVERSION	10	// 버젼이 다름 
#define _LOGIN_ERROR_CODE_SERVERISFULL	12	// 선택한 서버군은 이미 꽉 차버렸음.. 
#define _LOGIN_ERROR_CODE_ETC			13	// 기타 오류.. 
#define _LOGIN_ERROR_CODE_BILL_NO_INFO	15	// 인증정보 없음, 상품구매 바람
#define _LOGIN_ERROR_CODE_BILL_NO_MONEY	16	// 잔액 부족
#define _LOGIN_ERROR_CODE_NOT_LMBC_CODE	17	// 지역 코드( 회사 코드 ) 이상
#define _LOGIN_ERROR_CODE_NOT_UNI_NEXON	18	// 넥슨 통합 계정이 아닙니다. 먼저 통합 바람.

// 대만 빌링용 message
#define _LOGIN_ERROR_CODE_NOT_PCBANG	20	// PC방 유저가 아님	: 대만 빌링용
enum 
{
	_LOGIN_ERROR_CODE_METEL_E00 = 21,		// Can't open aplist.txt
	_LOGIN_ERROR_CODE_METEL_E01,			// Invalid data in aplist.txt
	_LOGIN_ERROR_CODE_METEL_E02,			// Fail to connect to SQLAP
	_LOGIN_ERROR_CODE_METEL_E03,			// Fail to create event
	_LOGIN_ERROR_CODE_METEL_E04,			// Calling API before InitBilling
	_LOGIN_ERROR_CODE_METEL_E05,			// Bad message format
	_LOGIN_ERROR_CODE_METEL_E06,			// Timeout for SQLAP response
	_LOGIN_ERROR_CODE_METEL_E07,			// Fail to send message to SQLAP
	_LOGIN_ERROR_CODE_METEL_E08,			// Too many user login, max 320000
	_LOGIN_ERROR_CODE_METEL_E09,			// IP address is incorrect
	_LOGIN_ERROR_CODE_METEL_E10,			// Port value is out of range
	_LOGIN_ERROR_CODE_METEL_E11,			// Fail to create thread
	_LOGIN_ERROR_CODE_METEL_E12,			// API is already running
	_LOGIN_ERROR_CODE_METEL_E13,			// Crypt key has not been set
	_LOGIN_ERROR_CODE_METEL_E14,			// Message queue is full
	_LOGIN_ERROR_CODE_METEL_E15,			// SQLAP is dead
	_LOGIN_ERROR_CODE_METEL_E16,			// Checksum error in received message
	_LOGIN_ERROR_CODE_METEL_E17,			// Crypt key is too long or too short

	_LOGIN_ERROR_CODE_METEL_E50,			// SQL Error
	_LOGIN_ERROR_CODE_METEL_E51,			// AP exception
	_LOGIN_ERROR_CODE_METEL_E52,			// Incorrect ID or Password
	_LOGIN_ERROR_CODE_METEL_E53,			// User has not registered the game
	_LOGIN_ERROR_CODE_METEL_E54,			// This account is forbidden
	_LOGIN_ERROR_CODE_METEL_E55,			// Points is not enough to play
	_LOGIN_ERROR_CODE_METEL_E56,			// The game registration is forbidden
	_LOGIN_ERROR_CODE_METEL_E57				// Can't find the LoginSN in DB
};

#define _LOGIN_ERROR_CODE_BLOCK_CODE_1  101 // 영구블럭 
#define _LOGIN_ERROR_CODE_BLOCK_CODE_2  102 // 계정압류 
#define _LOGIN_ERROR_CODE_BLOCK_CODE_3  103 // 임시블럭

#pragma pack(pop, enter_login)
