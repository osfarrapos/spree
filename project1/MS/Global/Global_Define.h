#ifndef _GLOBAL_DEFINE_H
#define _GLOBAL_DEFINE_H



#ifdef _NDEBUG
// 프로그램 초기화 
//#define _TOTAL_VITUAL_MEMORY_SIZE			100U
#define _QUEUE_MEMORY_SIZE					4096
#define _CHAT_QUEUE_MEMORY_SIZE				4096
#define _UDP_QUEUE_SIZE						4096
#define _PERMISSIBLE_MAX_USER				200	// DS_SERVER 에서 허용하는 최대 유저다..(최대 캐릭이랑 다름)
#define _SEND_SLOT_COUNT_					100
#define _SOCKET_POOL_COUNT_					100
#define _CRITICAL_SECTION_POOL_SIZE			1000
#else
// 프로그램 초기화 
//#define _TOTAL_VITUAL_MEMORY_SIZE			200U
#define _QUEUE_MEMORY_SIZE					16384 //4096
#define _CHAT_QUEUE_MEMORY_SIZE				16384 //4096
#define _UDP_QUEUE_SIZE						4096
#define _PERMISSIBLE_MAX_USER				20000	// DS_SERVER 에서 허용하는 최대 유저다..(최대 캐릭이랑 다름)
#define _SEND_SLOT_COUNT_					100000
#define _SOCKET_POOL_COUNT_					10000
#endif

// Thread 생성 개수 
#define _THREADNUMBER_PACKET_RECV_			4		//4	// recv 처리하는 함수.
//#define _THREADNUMBER_PACKET_PROCESS_		4		//8	// 큐에 있는 패킷을 처리하는 함수. 왠만하면 위에꺼 * 2
#define _THREADNUMBER_TCP_WORK_				4		// 일반 메시지 처리하는 쓰레드수..
#define _THREADNUMBER_UDP_WORK_				1		// 일반 메시지 처리하는 쓰레드수..
#define _THREADNUMBER_CHATWORK_				8		// 채팅 메시지 처리하는 워크쓰레드수..
#define _THREADNUMBER_LOGINWORK_			8		// 로그인 메시지 처리하는 워크쓰레드 수..
#define _THREADNUMBER_BACKUP_				2		// 백업 쓰레드..

#define _BACKUP_INTAERVAL_TIME_				100		// 백업 시간 간격

#define _ACCEPT_SUCCESS_TIME_				12000


enum en_db_string_where
{
	game_db_string = 0,
	account_db_string,
	log_db_string,
	event_db_string,
} ;


// 구분자..
#define _GROUP_PARSING_CHAR_				'\r'		// '\r'그룹이름 구분
#define _CHARAC_PARSING_CHAR_				'\n'		// '\n'캐릭이름 구분..



//TCP Socket 정보	
#define ACCEPT_PORT							7411


// GS Environment file path
#define CRYPTO_KEY_FILE						"lump.dat"
#define SQL_CONNECTION_STRING				"data.enc"

	
// GS 서버 정보
#define MAX_SERVER							25	//->GsSvMx
//#define MAX_USER							1000

// GS 서버 상태
#define GS_NONSTATE							0		// 꺼진상태
#define GS_OK								1		// 정상상태
#define GS_OVER								3		// 포화
#define GS_ERROR							4		// 비정상 상태

// Log 관련
#define LOG_MAX_LINE						0	// 로그 최대 라인
#define _CONSOLE_OUPUT_CATCH_LOG_					// 콘솔메시지를 로그로 남김 ㅋ


/* ============================================================================
캐릭터 상태
============================================================================ */
/*
#define CHARAC_STATE_LOGOFF						0	//	로그오프 상태
#define CHARAC_STATE_LOGON						1	// 로그온 상태
#define CHARAC_STATE_PLAY						2	// 전투중
#define CHARAC_STATE_3							3
#define CHARAC_STATE_4							4
*/



/* ============================================================================
Messenger Server
============================================================================ */
#define MAX_MAKE_FRIEND						58
#define MAX_MEMBER							50
#define MAX_GROUP							8
#define MAX_MAIL_COUNT						20

/* ============================================================================
Backup
============================================================================ */
#define BACKUP_NONE							0
#define BACKUP_READY						1


/* ============================================================================
시스템에서 전서구를 통해 사용자에게 보내지는 메시지 모드
============================================================================ */
#define MSMESSAGE_CUT_FRIEND				1
#define MSMESSAGE_DEL_FRIEND				2		// 캐릭터 삭제됨

/* ============================================================================
유저 상태
============================================================================ */


// time 관련
#define _HOUR_			3600		//한시간 = 3600초
#define _YEAR_			32140800	//1년
#define _SINCE_			33 * _YEAR_	// 1970~2003년 까지 초..


#define		_LOG_WRITE_


// UDP PASS KEY
#define  _SERVER_AUTHOR_	1003
#define  _PING_PORT_		10000 + ACCEPT_PORT

// GM CLASS NUM
#define  _GM_CLASS_NUM_			10
#define  _GM_RETURN_MESSAGE		"GM에게는 귓속말을 보낼수 없습니다.\n"





// MS Transport Provider IOCTL to control
// reporting PORT_UNREACHABLE messages
// on UDP sockets via recv/WSARecv/etc.
// Path TRUE in input buffer to enable (default if supported),
// FALSE to disable.
#define SIO_UDP_CONNRESET_WSAIOW (IOC_VENDOR,12)




#endif 


