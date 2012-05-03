#ifndef _GLOBAL_DEFINE_H
#define _GLOBAL_DEFINE_H

// 프로그램 초기화 
#define _TOTAL_VITUAL_MEMORY_SIZE			400U
#define _QUEUE_MEMORY_SIZE					16384 //4096

// Thread 생성 개수 
#define _THREADNUMBER_PACKET_RECV_			2		//4	// recv 처리하는 함수.
#define _THREADNUMBER_WORK_					4		// 일반 메시지 처리하는 쓰레드수..
#define _THREADNUMBER_BACKUP_				16		// 백업 쓰레드..


// GS Environment file path
#define CRYPTO_KEY_FILE						"lump.dat"
#define SQL_CONNECTION_STRING				"log.enc"


//TCP Socket 정보	
#define ACCEPT_PORT_BASE					30000

	
#define _COUNT_FIELD_						25

// Buf 길이 정보
#define UDP_SEND_BUF_SIZE					4096
#define UDP_RECV_BUF_SIZE					4096

// TCP MSG 정보
#define HEAD_SIZE							2		// MSG 패킷 헤더 사이즈

	
// GS 서버 정보
#define MAX_SERVER							25	//->GsSvMx
//#define MAX_USER							1000


// Log 관련
#define LOG_MAX_LINE						10000	// 로그 최대 라인
#define _CONSOLE_OUPUT_CATCH_LOG_					// 콘솔메시지를 로그로 남김 ㅋ


// time 관련
#define _HOUR_			3600		//한시간 = 3600초
#define _YEAR_			32140800	//1년
#define _SINCE_			(33 * _YEAR_)	// 1970~2003년 까지 초..

#define _TEST_
#define		_LOG_WRITE_


// UDP PASS KEY
#define  _SERVER_AUTHOR_	1003


// 로그 큐 노드 갯수..
#define  _LOG_ITEM_NODE_MAX_NUM_		400000
#define  _LOG_CHARAC_NODE_MAX_NUM_		50000
#define  _LOG_STATE_NODE_MAX_NUM_		10000
#define  _LOG_LOGIN_NODE_MAX_NUM_		50000
#define _BACKUP_QUEUE_MAX_SIZE			(_LOG_ITEM_NODE_MAX_NUM_+_LOG_CHARAC_NODE_MAX_NUM_+_LOG_STATE_NODE_MAX_NUM_+_LOG_LOGIN_NODE_MAX_NUM_)


enum en_db_string_where
{
	game_db_string = 0,
	account_db_string,
	log_db_string,
} ;

#endif 