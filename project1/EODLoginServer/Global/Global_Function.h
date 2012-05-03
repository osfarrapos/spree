#pragma once

#ifndef _EXCLUDE_WINSOCK
#include <winsock2.h>
#include <mswsock.h>
#endif
#include <stdio.h>
#include <Iphlpapi.h>

#include <atldbcli.h>

#define consoloBufSize	2048
#define SAFE_DELETE(x) if(x) { delete x; x = NULL ;} ;


//=================================================================================
//  에러가 나면 내용을 파일에 기록한다. 

// #define _FILE_ERROR_MSG_	"errmsg\\errmsg.txt"

class _h_ErrorRecord
{
protected:
	FILE *			m_fpErrmsg ;
	char			m_szBuffer[1024] ;
	CRITICAL_SECTION m_csFile ;
	char			m_cIP[16] ;
	SYSTEMTIME		m_time ;

public:
	_h_ErrorRecord() : m_fpErrmsg(NULL) 
	{
		InitializeCriticalSection(&m_csFile) ;
	}
	~_h_ErrorRecord() { 
		if( m_fpErrmsg ) fclose( m_fpErrmsg ) ; 
		DeleteCriticalSection( &m_csFile ) ;
	}

	// 파일을 생성한다.
	bool Init( char * name ) ;
	void ErrMsg( const char *format,... ) ;		// 에러메세지를 파일에 쓴다. 

	void write_SendError( int err ) ;			// 
	void write_RecvError( int err, int sub ) ;
	void write_LoginTry( char * acc, unsigned int ip ) ;
	void write_LoginResult( char * acc, int uid, unsigned int ip, bool bSuccess = true ) ;
	void write_Result( char * acc, int uid, unsigned int ip, unsigned short port, int err, char * type ) ;

	void write_connect( unsigned int ip, unsigned short port ) ;
	void write_exception( char * errMsg ) ;
	void flush() {
		fflush(m_fpErrmsg) ;
	};

} ;



// 초로 계산했을 때.
#define _SECOND_YEAR_		32140800			// 1년 
#define _SECOND_MONTH_		2678400				// 31 일 
#define _SECOND_DAY_		86400				// 1일
#define _SECOND_HOUR_		3600				// 1시간
#define _SECOND_MINUTE_		60					// 1분

#define _GAME_TIME_YEAR_		10368000
#define _GAME_TIME_MONTH_		864000
#define _GAME_TIME_DAY_			28800
//=================================================================================
//=================================================================================
struct sDate
{
	int Year ;
	int Month ;
	int Day ;
	int Second ;
} ;



inline int getGlobalTimeByIntSec()
{
	time_t	t ;
	t = time(NULL);
	return static_cast<int>(t) ;	
} ;
#define GetTime_ByIntSec() getGlobalTimeByIntSec()

inline time_t getGlobalTimeByTimeTSec()
{
	time_t	t ;
	time( &t ) ;
	return t ;	
} ;
#define GetTime_By_TimeTSec() getGlobalTimeByTimeTSec()

/*
inline DWORD getGlobalTimeByMilliSec()
{
	return timeGetTime() ;
} ;
#define GetTime_ByMilliSec() getGlobalTimeByMilliSec() ;
*/


void ip_char(DWORD ip, char *buf) ;
//char * ip_char( unsigned int ip ) ;


//#################################################################################
void PrintConsole( const char *format,... ) ;
//#################################################################################



inline char * GetMyIP( char mode = 0 /*0:LAN, 1:WAN*/)
{
	
	
	static char m_cMyOuterip[17] = {0};		// 외부
	static char m_cMyInnerip[17] = {0};		// 내부
	
	int first_part = 0;
	
	DWORD dwSize = 0 ;
	GetIpAddrTable( NULL, &dwSize, FALSE ) ;
	
	PMIB_IPADDRTABLE pIpAddrTable = (PMIB_IPADDRTABLE)new BYTE[dwSize] ;
	
	char myOutIP[16] ;
	
	if( pIpAddrTable ) {
		
		if( GetIpAddrTable( pIpAddrTable, &dwSize, FALSE ) == NO_ERROR ) {
			
			printf( "pIpAddrTable->dwNumEntries = %u\n", pIpAddrTable->dwNumEntries ) ;
			
			if( pIpAddrTable->dwNumEntries > 0 ) {
				
				//				char szIP[16] ;
				
				for( int i = 0 ; i < (int)pIpAddrTable->dwNumEntries ; i++ ) {
					
					in_addr ina ;
					ina.S_un.S_addr = pIpAddrTable->table[i].dwAddr ;
					
					strcpy( myOutIP, inet_ntoa( ina ) ) ;

					first_part = atoi( myOutIP );
					
					if( first_part == 0 || first_part == 127 )
						;
					else
					{
						if( first_part == 192 || first_part == 10 )
						{
							if( m_cMyInnerip[2] != '.' )
							{
								strcpy( m_cMyInnerip, myOutIP ) ;
							}		
						}
						else
						{
							strcpy( m_cMyOuterip, myOutIP ) ;
						}
						
						
					}
				}
			}
		}
	}
	
	delete[] pIpAddrTable ;

	if( m_cMyOuterip[0] == 0 )
	{
		strcpy( m_cMyOuterip, m_cMyInnerip );
	}
	
	if( mode == 0 ) {		
#ifdef _DEBUG
		memcpy( m_cMyInnerip, m_cMyOuterip, 17 ) ;
#else
		if( m_cMyInnerip[0] == 0  )
		{
			memcpy( m_cMyInnerip, m_cMyOuterip, 17 ) ;
		}
#endif
		return m_cMyInnerip ;
	}
	else {
		return m_cMyOuterip ;
	}
}


// MS Transport Provider IOCTL to control
// reporting PORT_UNREACHABLE messages
// on UDP sockets via recv/WSARecv/etc.
// Path TRUE in input buffer to enable (default if supported),
// FALSE to disable.
#define SIO_UDP_CONNRESET_WSAIOW (IOC_VENDOR,12)




inline SOCKET RestoreUDPSocket( const unsigned short usPortNum, const char mode ) 
{
	
	SOCKADDR_IN udpAddr ;
	SOCKET s ;
	
	const char * ip = GetMyIP(mode) ;
	udpAddr.sin_family = AF_INET ;
	udpAddr.sin_port = htons( usPortNum ) ;// htons( g_pServerInfo->get_udp_port( 0 ) ) ;
	udpAddr.sin_addr.s_addr = inet_addr( ip ) ;

	printf("UDP Binding IP: %s \n", ip) ;
	
	
	// 소켓 초기화 먼저 하고
	s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ;
	if( s == INVALID_SOCKET ) {
		PrintConsole( "UDP ServerCom Thread Error\n" ) ;
		return INVALID_SOCKET ;
	}

	DWORD dwBytesReturned = 0;
	BOOL bNewBehavior = FALSE;
	DWORD status;
	
	// disable  new behavior using
	// IOCTL: SIO_UDP_CONNRESET
	status = WSAIoctl(s, SIO_UDP_CONNRESET,
		&bNewBehavior, sizeof(bNewBehavior),
		NULL, 0, &dwBytesReturned,
		NULL, NULL);
	
	if (SOCKET_ERROR == status)
	{
		DWORD dwErr = WSAGetLastError();
		if (WSAEWOULDBLOCK == dwErr)
		{
			// nothing to do
			return(INVALID_SOCKET);
		}
		else
		{
			printf("WSAIoctl(SIO_UDP_CONNRESET) Error: %d\n", dwErr);
			return(INVALID_SOCKET);
		}
	}
	
	if( bind( s, (SOCKADDR *)&udpAddr, sizeof( SOCKADDR_IN ) ) == SOCKET_ERROR ) {
		PrintConsole( "Error : UDP ServerComThread socket bind code : %u\n", WSAGetLastError() ) ;		
		closesocket( s ) ;
		return INVALID_SOCKET ;
	}
	
	return s ;
	
}


/*
	CND_LoginChecker
		passport 에는 아이피, 패스워드 아이디가 들어간다
		이때, 들어가는 위치는 조금씩 다른데, 무작위로 1 ~ 8 씩 건너뛰며 있다.
*/
class CND_LoginChecker
{
public:
	CND_LoginChecker( u_int lmbcMode ) 
		: m_password(NULL), m_id(NULL) 
	{
		m_password = new char[en_max_lpl+1] ;
		m_id = new char[en_max_lil+1] ;
	}
	~CND_LoginChecker()
	{
		if( m_password )
			delete m_password;
		m_password = NULL;

		if( m_id )
			delete m_id;
		m_id = NULL;
	}
	BOOL lc_MakePacket( u_int lmbcMode IN, char * passport OUT, int passlength IN, const char * pID IN, const char * pPW IN, const char * pIP IN );
	BOOL lc_CheckPakcet( u_int mode IN, char * passport );

	inline char * lc_GetIP()	{ return m_ip ; }
	inline char * lc_GetPW()	{ return m_password ; }
	inline char * lc_GetID()	{ return m_id ; }

protected:
	char		m_ip[16];
	char *		m_password;
	char *		m_id;
};