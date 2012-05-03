#ifndef _GLOBAL_DEFINE_H
#define _GLOBAL_DEFINE_H


#define _COUNT_MAX_GROUP_				122

// 서버군 번호의 마지막 2개의 인덱스를 사용.
#define _GR_INDEX_TRAIN_				120
#define _GR_INDEX_UNION_				121


// 컴파일 날짜
#define  _COMPILE_DATE_	__TIMESTAMP__

// 프로그램 초기화 

#define _GENERAL_QUEUE_MEMORY_SIZE			1024 //(카운트임)
#define _UPDATE_QUEUE_MEMORY_SIZE			4096
#define _PARTY_QUEUE_MEMORY_SIZE			1024
#define _OR_QUEUE_MEMORY_SIZE				1024
#define _CHAT_QUEUE_MEMORY_SIZE				2048
#define _UDP_SEND_QUEUE_MEMORY_SIZE			2048
#define _UDP_RECV_QUEUE_MEMORY_SIZE			1024
#define _UDP_LOGIN_QUEUE_MEMORY_SIZE		2048

#define _PERMISSIBLE_MAX_USER				10000	// DS_SERVER 에서 허용하는 최대 유저다..(최대 캐릭이랑 다름)
#define _PERMISSIBLE_MAX_CHARAC				(_PERMISSIBLE_MAX_USER * 3)	// 허용 가능한 최대 캐릭이다
#define _UNIQUEID_MAX_COUNT					65000U

// DS Send OV Slot 
#define OV_SLOT_MAX_COUNT					10000U
#define OV_SLOT_MAX_SIZE					10U			// mega
#define _RECV_BUF_SIZE_						(4 * _MEGA_)



#define _FOREST_MAX_USER					2500 // 초기 허용 인원 설정값..
#define _GM_CLASS_NUM						19
#define _ROYAL_CLASS_NUM					18

// Thread 생성 개수 
#define _THREADNUMBER_PACKET_PROCESS_		4		//8	// 큐에 있는 패킷을 처리하는 함수. 왠만하면 위에꺼 * 2
#define _THREADNUMBER_UPDATE_PROCESS_		1		//8	// 큐에 있는 패킷을 처리하는 함수. 왠만하면 위에꺼 * 2
#define _THREADNUMBER_UDP_PACKET_RECV_		2		//4	// recv 처리하는 함수.
#define _THREADNUMBER_UDP_PACKET_SEND_		1		//4	// secv 처리하는 함수.
#define _THREADNUMBER_UDP_PACKET_WORK_		1		//8	// udp work 처리하는 함수. (주로 GMS 패킷이다.)
#define _THREADNUMBER_UDP_PACKET_LOGIN_		4		//8	// udp login 처리하는 함수.
#define _THREADNUMBER_CHARAC_BACKUP_		2		// 캐릭 백업 쓰레드 갯수
#define _THREADNUMBER_USER_BACKUP_			1		// 유저 백업 쓰레드 갯수
#define _THREADNUMBER_PARTY_WORK			1		// PARTY 작업 쓰레드
#define _THREADNUMBER_OR_WORK				1		// OR 작업 쓰레드
#define _THREADNUMBER_CS_CHAT_WORK			2		// CS Chat thread

// BACKUP 주기
#define _BACKUP_TIME_USER_					50	// 
#define _BACKUP_TIME_CHARAC_				50	// 

#define _BACKUP_SYNC_OPTION					0
#define _BACKUP_UNSYNC_OPTION				1

// Memory size define
#define _MEGA_								1048576
#define _PAGE_SIZE_							4096

// UDP Socket 정보
#define PASS_KEY							1003
#define PING_PORT							19999

#define UDP_PACKET_CUT_SIZE					300


// Buf 길이 정보
#define UDP_SEND_BUF_SIZE					32767
#define UDP_RECV_BUF_SIZE					32767

// TCP MSG 정보
#define HEAD_SIZE							2		// MSG 패킷 헤더 사이즈

#define PWD_ALLLOGOUT						1111

// 공지사항, 게임서버 최대 배열수... 
#define SHOW_MAX							20	// 공지사항 리스트 갯수
#define GS_MAX								35	// 게임서버 최대


#define CRYPTO_KEY_FILE						"Env\\lump.dat"
#define SQL_CONNECTION_STRING				"Env\\data.enc"

// GS Environment file path
#define SERVER_ENVIRONMENT_FILE				"Env\\ServerEnv.inf"
// GS Position list file path
#define SERVER_POSITION_FILE				"Env\\PosList.inf"

// Log server info file path
#define LOG_SVR_INFO_FILE					"Env\\LogServer.Inf"
// Log server info file path
#define BILLING_SVR_INFO_FILE				"Env\\BillingServer.Inf"

// Initialize value whitd Charac create 
#define INIT_VALUE_REF_FILE					"Env\\InitValue.inf"
#define INIT_EVENT_FILE						"Env\\event.txt"

#define _SYSTEM_LOG_FILE_					"logfile\\DS_SystemLogFile"
#define _PARTY_LOG_FILE_					"logfile\\Party"
#define _TEMP_LOG_FILE_						"logfile\\temp"
#define _GMS_LOG_FILE_						"logfile\\GMS"

#ifdef _PD_GAME_EVENT_
#define FILE_FREE_MATCH_SCRIPT				"Env\\FreeMatch.txt"
#endif
	


// Log 관련
#define LOG_MAX_LINE						80000	// 로그 최대 라인

// 인벤토리 슬롯 최대 갯수
#define INVEN_MAX_SLOT						58

// 무공 슬롯 최대 갯수
#define MARTIAL_MAX_SLOT					60

/* ============================================================================
캐릭터 상태
============================================================================ */
#define CS_NONSTATE							0		//	비 사용 상태
#define CS_STANDBY							1		//	선택을 기다리는 상태
//#define CS_SELECTED						2		//	캐릭터 선택창에서 선택되어진 상태  //안쓸까보다~ 
//#define CS_SV_ENTER						3		//	서버(필드(존))로 진입하는중  // 이것두 안쓸까보다.. 간단하게 가자.. 
#define CS_FIELD							3		//	필드에 진입 완료 상태(게임중)
#define CS_MOVE								4		//	서버 이동중
#define CS_ROBBY							5		//  로비에 있음..


/* ============================================================================
유저 상태
============================================================================ */
#define US_NONSTATE						0		//	로그아웃 상태(비 사용)
#define US_LOGIN_ING					1		//	로그인 진행중
#define US_LOBBY						2		//	GS 진입
#define US_FIELD						3		//	게임 진행중
#define US_MOVE							4		//	서버 이동중
#define US_LOCK							99		//	계정 잠김 상태

// 유저 메모리 포인터 상태
#define	US_SAVED						0		// 저장됨
#define US_USING						1		// 사용중


/* ============================================================================
백업 상태
============================================================================ */
#define BS_COMPLETED					0		// 백업 완료
#define BS_READY						1		// 백업 대기


/* ============================================================================
퀘스트 상수
============================================================================ */
#define _MAX_CURQUEST_NUM_                  6           // 동시수행가능한 퀘스트 수 (DB에 저장되는 퀘스트의 갯수)
#ifdef _PD_QUEST_MAX_600_
#define _QUEST_1_NUMBER_					(300)
#define _QUEST_2_NUMBER_					(300)
#define _QUEST_MAX_NUMBER_					(_QUEST_1_NUMBER_+_QUEST_2_NUMBER_)		    // 제룡행을 제외한 퀘스트 숫자.

#else
#define _QUEST_MAX_NUMBER_					300		    // 제룡행을 제외한 퀘스트 숫자.
#endif
//===============================================================================

/* ==============================================================================
에러 메시지 구분
=============================================================================== */
#define INIT		"INIT"		//초기화 관련 메시지
#define INFO		"INFO"		//정보 알림 메시지
#define BACKUP		"BACKUP"	//백업정보 메시지
#define DB_ERR		"DB_ERR"	// DB 에러
#define SO_ERR		"SO_ERR"	//socket 에러
#define ME_ERR		"ME_ERR"	//메모리 관련 에러
#define EXCEPTION	"EXCEPTION"	//예외


// time 관련
#define _HOUR_			3600		//한시간 = 3600초
#define _YEAR_			32140800	//1년
#define _SINCE_			(33 * _YEAR_)	// 1970~2003년 까지 초..

//#define _MINUTE_MIL_SEC_ (1000 * 60)
//#define _HOUR_MIL_SEC_	(1000 * 60 * 60)	// 1시간의 밀리세컨드
//#define _DAY_MIL_SEC_	(1000 * 60 * 60 * 24)	// 하루 밀리세컨드


#define _DAY_SEC_	(60 * 60 * 24)			// 하루 세컨드
#define _MINUTE_SEC_	(60)			// 하루 세컨드


#define _SEC_SINCE_1970_ (31557600 * (2005-1970))



#define	_MEM_SAVE_DELAY_TIME_LOGOUT		(_MINUTE_SEC_ * 1)
#define _MEM_SAVE_DELAY_TIME_KICK		(_MINUTE_SEC_ * 3)
#define _UNIQUE_ID_LIVE_TIME_SEC		30
#define _RELOGIN_ABLE_TIME_SEC			5
#define _PARTY_MOVE_OUT_TIME_SEC		_MINUTE_SEC_ * 3
#define _GS_INFO_LOG_SAVE_TIME_SEC		_MINUTE_SEC_ * 5

/* ====================

party 관련

=====================*/
#define MAX_PARTY_MEMBER					9		// 파티원 최고
// GS 최대 갯수..
#define MAX_SERVER_COUNT	32


// 익셉션 처리관련.
#undef TRY
#undef CATCH_ALL

#define TRY					__try
#define CATCH_ALL			__except(g_pNDExceptionReport->GenerateExceptionReport(GetExceptionInformation()),EXCEPTION_EXECUTE_HANDLER)


// MS Transport Provider IOCTL to control
// reporting PORT_UNREACHABLE messages
// on UDP sockets via recv/WSARecv/etc.
// Path TRUE in input buffer to enable (default if supported),
// FALSE to disable.
#define SIO_UDP_CONNRESET_WSAIOW (IOC_VENDOR,12)

// 장원의 최대 돈.
#define MAX_CASTLE_MONEY		(4200000000U)


enum en_db_string_where
{
	game_db_string = 0,
	account_db_string,
	log_db_string,
	event_db_string,

#ifdef _CIS_20070326_
	cis_db_string,
#endif
} ;

// 서버 실행 모드 : 모드에 따라, 메모리 사용량이 다르다.
enum EN_ND_SERVICE_MODE
{
	en_service_mode_standard=0,
	en_service_mode_outertest,
	en_service_mode_innertest,
};


/* =====================================================================================
문파 구분
===================================================================================== */
#define _MOONPA_NO_NANGIN_				0		// 낭인
#define _MOONPA_NO_GAEBANG_				1		// 개방
#define _MOONPA_NO_BEEGOONG_			2		// 비궁(이전 배교)
#define _MOONPA_NO_SORIM_				3		// 소림
#define _MOONPA_NO_NOKLIM_				4		// 녹림
#define _MOONPA_NO_MOODANG_				5		// 무당
#define _MOONPA_NO_MAGYO_				6		// 마교
#define _MOONPA_NO_SEGA_				7		// 세가
#define _MOONPA_NO_HEOKRYONG_			8		// 미정

#define _NUM_MOONPA_					9		// 최대 문파수(리스트수 설정시 사용)		 
#define	_NUM_CLASS_						8		// 최대 직책수

// 0 : 낭인, 1 : 백도, -1 : 흑도
const short g_sMoonpaProperty[_NUM_MOONPA_] = 
{ 
	0, 	// 낭인0
		1,	// 개방1
		-1,	// 비궁2
		1,	// 소림3
		-1,	// 녹림4
		1,	// 무당5
		-1,	// 마교6
		0,	// 세가7
		0	// 흑룡방8
} ;


enum LOGIN_MODE_BY_COMPANY
{
	en_lmbc_nexon = 0xa04b93f2,
	en_lmbc_metel = 0x3d732b0e, 
	en_lmbc_vina =  0x8c203d51,
	en_lmbc_acclaim = 0xef6c9aa1,
	en_lmbc_pwc = 0xef6c9aa2,
	en_lmbc_rocworks = 0xef6c9aa3,
	en_lmbc_russia = 0xad890bd0,
} ;

enum CHARAC_NAME_LENGTH
{
	en_charac_name_length = 12,
};

enum LOGIN_ID_LENGTH		// Login ID Lenght
{
	en_max_lil		= 24,
};

#define en_max_lpl		(24)

// Acclaim과 PWC 설정.
#ifdef _PD_COMPANY_ACCLAIM_
#undef en_max_lpl
#define en_max_lpl		(32)
#endif

#ifdef _PD_COMPANY_PWC_
#undef en_max_lpl
#define en_max_lpl		(32)
#endif

#ifdef _PD_COMPANY_JAPAN_
#undef en_max_lpl
#define en_max_lpl		(32)
#endif

#ifdef _PD_COMPANY_RUSSIA_
#undef en_max_lpl
#define en_max_lpl		(32)
#endif

#define IPV4_BUF_SIZE		16


// 영웅단 이름의 길이.
enum
{
	en_or_name_length = 24, 
} ;

// 동맹의 최대 영웅단수
#define _NDD_MAX_ORG_IN_ALLY_		(5)		// 동맹 구성시 최대 영웅단 수는 5이다.

#define __EXCEPT_STR_PRINT()		::PrintConsole("[EXCEPTION %s, %d \n", __FILE__, __LINE__ )

#ifndef _FREE_LOGIN_MODE
#define _FREE_LOGIN_MODE
extern bool g_bIsFreeLogin ;
#endif
#define IF_FREE_LOGIN			if( g_bIsFreeLogin == true )
#define IF_NOT_FREE_LOGIN		if( g_bIsFreeLogin == false )


#ifdef _METEL_LOGIN_200608_
struct METEL_BILLING_LOGIN {
	HANDLE	h;
	char	pszId[en_max_lil+1];
	char	pszPass[en_max_lpl+1];
	char	pszIp[IPV4_BUF_SIZE];
};

struct METEL_BILLING_INFO {
	// total (4*6) + (24*2) + 4 = 76 bytes

	int		iLoginSN;
	int		iBillingType;
	int		iUsedPoint;
	int		iPoint;
	int		iUsedFreePoint;
	int		iFreePoint;
	TGATime	Time;
	TGATime	EndTime;

	int		iCallbackResult;
};
#endif //_METEL_LOGIN_200608_


/* //////////////////////////////////////////////////////////////////////////
GS 와 클라이언트에서 시간을 표시하기 위해 사용하는 구조체다. 
절대로, 내용을 바꾸지 말 것.!!!
*/ //////////////////////////////////////////////////////////////////////////
struct _sND_TimeUnit	// client 에 보내는 패킷에 포함됨. 바뀌게 되면 클라이언트에게 알려줘야 한다. 
{
	// 년/월/일/시/분/초. End Time
	u_int			uiYear		: 5 ;	// Base 2000. Year = 2000 + uiYear( 0-31 )
	u_int			uiMonth		: 4 ;	// Value 0 - 11 ( 0 - 15 )
	u_int			uiDay		: 6 ;	// Value 1 - 31 ( 0 - 63 )
	u_int			uiHour		: 5 ;	// Value 0 - 23 ( 0 - 31 ) 
	u_int			uiMinute	: 6 ;	// Value 0 - 59 ( 0 - 63 )
	u_int			uiSecond	: 6 ;	// Value 0 - 59 ( 0 - 63 )


	_sND_TimeUnit()
		: uiYear(0), uiMonth(0), uiDay(0), uiHour(0), uiMinute(0), uiSecond(0)
	{
	}

	_sND_TimeUnit( const _sND_TimeUnit& rh)
		: uiYear(rh.uiYear), uiMonth(rh.uiMonth), uiDay(rh.uiDay), 
		uiHour(rh.uiHour), uiMinute(rh.uiMinute), uiSecond(rh.uiSecond)
	{
	}

	void Print()
	{
		printf( "%u-%u-%u %u:%u:%u", uiYear+2000, uiMonth, uiDay, uiHour, uiMinute, uiSecond );
	}

	time_t GetValueType()
	{
		time_t value = uiSecond | (uiMinute<<6) | (uiHour<<12) | (uiDay<<17) | (uiMonth<<22) | (uiYear<<25) ;
		return value;
	}

	void SetTime( unsigned int y, unsigned int mon, unsigned int d, unsigned int h, unsigned int m, unsigned int s = 0 )
	{
		uiYear = y-2000;
		uiMonth = mon;
		uiDay = d;
		uiHour = h;
		uiMinute = m;
		uiSecond = s;
	}
} ;

//////////////////////////////////////////////////////////////////////////
// 기타 함수 디파인
//////////////////////////////////////////////////////////////////////////
class CND_TimeUnitFunc
{
public:
	static time_t	GetDiffSecond( _sND_TimeUnit& from, _sND_TimeUnit& to );
	static void		GetTime_BeforeSecond( _sND_TimeUnit& output, _sND_TimeUnit& from, u_short second );
	static int		GetElapsedDay( _sND_TimeUnit& t );
	static void		GetTime_Current( _sND_TimeUnit& output );
	static void		GetTime_FromSecond( _sND_TimeUnit& output, time_t cur );
	static time_t	Get_CurrentSecond();
	static time_t	Get_FromTimeUnit( _sND_TimeUnit& input );
};



#ifdef _PD_MASTER_PUPIL_

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

// 스승 레벨 최대값. 0부터 4까지. 총 다섯 단계다.
#define NDD_MP_MAX_MASTER_LEVEL			(4)
// 최대 제자수
#define NDD_MP_MAX_PUPIL_COUNT			(4)
// 제자 신청시 중복가능한 캐릭터 수.
#define NDD_MP_MAX_RESERVE_PUPIL		(5)		
// 제자 신청시, 응답까지의 유효시간 : 스승/제자 같이 쓰인다.
#define NDD_MP_RESERVE_WAIT_TIME		(12)		// s
// 기본, 확장 혜택의 킬링 카운트
// 스크립트에서 정의되지 않으면, 아래 숫자를 쓴다. 
#define NDD_MP_BASE_KILLCOUNT			(100U)
#define NDD_MP_EXTEND_KILLCOUNT			(1000U)

// 사호의 최대 길이.
#define NDD_MP_MASTERNICK_LENGTH		(36)		// NULL 미포함.

#define NDD_MP_MAX_POINT				(1000000000U)		// 10억 이다. 절대로 0x8fffffff 를 넘지 않도록 할 것.

// 보드에 들어가는 아이템의 최대 개수 : 8 ~ 12 개로 가변적으로 갈 수 있다.
#define NDD_MP_MAX_ITEMCOUNT_PER_BOARD		(12)
#define NDD_MP_AVR_ITEMCOUNT_PER_BOARD		(10)
#define NDD_MP_MIN_ITEMCOUNT_PER_BOARD		(8)


// 대부분의 다파인은 DS에서도 사용중이다. 따라서, 수정하면 반드시 DS 도 함께 수정할 것.
// charac :: m_masterpupil_relation 값으로 쓰인다. 
enum EN_MP_RELATION
{
	en_mp_relation_none=0,
	en_mp_relation_pupil,
	en_mp_relation_master,
};


// kill count 함수의 리턴값으로 쓰인다.
enum EN_KILLCOUNT_TYPE
{
	en_mp_killcount_type_none = 0,		// 아무혜택 없다. 
	en_mp_killcount_type_base,			// 첫번째 경험치 혜택.
	en_mp_killcount_type_extend,		// 두번재 경험치 혜택.
};


// 구조체의 경우, 패킷으로 사용하는 것은 cspk 를 붙인다.
// 클라이언트와 통신하는 경우, cspk 를 붙인다.

struct _sCSPK_Pupil
{
	int				iPupilIndex;
	u_int			uiMyKillCount;		// 나의 킬링 카운트.
	u_int			uiToMasterPoint;	// 스승에게 전달된 포인트. 스승이 접속해 있을 때에만 올라간다.
	u_int			uiCurPoint;			// 현재 포인트 : 스승이 접속해 있지 않아도 올라간다.

	_sND_TimeUnit	join_date;

	u_char			ucPupilPosition;	// 1제자 2제자... 1 - 4 까지. 
	u_char			ucSex;				// 성. 0 - 남, 1 - 여.
	u_char			ucLevel;			// 현재 레벨.
	u_char			ucMoonpa;			// 문파.
	u_char			ucClass;			// 직책
	u_char			ucClassGrade;		// 직책 등급.
	u_char			ucConnectState;		// 0 - not, 1 - field, 2 - server move
	u_char			ucZone;				// 캐릭터가 어디에 있는가. 0 이면 접속하지 않은 것.
	char			cPupilName[en_charac_name_length+1];

	_sCSPK_Pupil()
	{
		Reset();
	}

	void SetInfo( _sCSPK_Pupil& pupil )
	{
		CopyMemory( this, &pupil, sizeof(_sCSPK_Pupil) );
		uiMyKillCount		= 0;					// SetInfo 는 DB 에서 읽을 때 설정하므로, 0으로.
	}
	void Reset()
	{
		ZeroMemory( this, sizeof(_sCSPK_Pupil) );
	}
	void SetName( char * name )
	{
		if( name )
		{
			strncpy( cPupilName, name, en_charac_name_length );
			cPupilName[en_charac_name_length] = 0;
		}
	}
	void SetIndex( int cuid )	{ iPupilIndex = cuid; }

	void SetJoinDate( DBTIMESTAMP& join )
	{
		join_date.uiYear	= join.year - 2000;
		join_date.uiMonth	= join.month;
		join_date.uiDay		= join.day;
		join_date.uiHour	= join.hour;
		join_date.uiMinute	= join.minute;
		join_date.uiSecond	= join.second;
	}

	bool CanBeStruck()
	{
		_sND_TimeUnit cur;
		CND_TimeUnitFunc::GetTime_Current( cur );

		time_t diff = CND_TimeUnitFunc::GetDiffSecond( join_date, cur );
		
#ifdef _PD_MP_DEBUG_
		return (diff > 300);		// 5분.
#else
		return (diff > 72*3600);		// 72시간.
#endif
	}

	u_int SubTotalPoint( u_int point )
	{
		return point;
	}


	void SetMyKillCount( u_int count )	{ uiMyKillCount = 0; }

	// 리턴값 : EN_KILLCOUNT_TYPE 을 사용한다. 
	u_char AddMyKillCount();

	inline bool IsConnected()	{ return (ucConnectState!=0); }
};


// 게시판에 표시될 사제 정보
struct _sPacketMPBoard
{
	u_char				ucMasterLevel;			// 스승의 레벨.
	u_char				ucMasterRank : 4;		// 스승 등급
	u_char				ucPupilCount : 4;		// 제자수( 필요하면 보임 )
	u_char				ucMoonpa;				// 문파.	
	u_char				ucClass;				// 직책
	u_char				ucClassGrade : 2;		// 직채 등급.
	u_char				ucOrg : 2;				// 영웅단 단주인가.
	u_char				ucEtc : 4;				// 기타 정보 필요하면.
	char				cMasterName[en_charac_name_length+1];
	char				cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];


	void SetName( char * name )
	{
		ZeroMemory( cMasterName, sizeof(cMasterName) );
		strncpy( cMasterName, name, en_charac_name_length );
	}

	void SetNick( char * nick )
	{
		ZeroMemory( cMasterNick, sizeof(cMasterNick) );
		strncpy( cMasterNick, nick, NDD_MP_MASTERNICK_LENGTH );
	}

};


#endif	// _PD_MASTER_PUPIL_

#endif 


