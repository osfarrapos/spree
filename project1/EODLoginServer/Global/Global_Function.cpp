#define WIN32_LEAN_AND_MEAN
#include "..\stdafx.h"
#include <windows.h>
#include <Mmsystem.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../myServer/MY_TRACE.H"
#include "Global_Function.h"


extern CLinePrintFile *		g_pErrMsg ;		// 에러메세지 출력기
extern CRITICAL_SECTION		g_csDebufPrint ;		// 콘솔.
extern HANDLE hOut2 ;
char strconsole[256] ;
DWORD	dwWritten ;



//#################################################################################
//
//	_h_ErrorRecord
//
//###################################################################### LHS Added....
bool _h_ErrorRecord::Init( char * name )
{
	SYSTEMTIME curTime ;
	GetLocalTime( &curTime ) ;

	char filename[40] ;
	sprintf( filename, "LogFiles/%s%02d%02d_%02d%02d.csv", name, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute ) ;
	
	if( ( m_fpErrmsg = fopen( filename, "at" ) ) == NULL )
		return false ;

	fprintf( m_fpErrmsg, "FAIL1 not register,2 login,3 retry,4 password,5 unknown\n%02d-%02d %02d:%02d,Account,Result,IP,PORT\n", curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond ) ;
	//fflush( m_fpErrmsg ) ;
	
	InitializeCriticalSection( &m_csFile ) ;
	
	return true ;
}

void _h_ErrorRecord::ErrMsg( const char *format,... )
{
	EnterCriticalSection( &m_csFile ) ; 
	try {

		GetLocalTime( &m_time ) ;

		va_list arglist ;
		va_start(arglist, format) ;
		vsprintf(m_szBuffer, format, arglist) ;
		va_end(arglist) ;

		fputs( m_szBuffer, m_fpErrmsg ) ;
		//fflush( m_fpErrmsg ) ;
	}
	catch (...) {
		printf( "[%02d-%02d%02d%02d] EXCEPTION ErrMsg\n", m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond ) ;
	}

	LeaveCriticalSection( &m_csFile ) ;
}


void _h_ErrorRecord::write_LoginTry( char * acc, unsigned int ip )
{
	EnterCriticalSection( &m_csFile ) ;

	try {
		GetLocalTime( &m_time ) ;
		ip_char( ip, m_cIP ) ;

		fprintf( m_fpErrmsg, "[%02d-%02d%02d%02d] LoginTry, acc=%s, ip=%s\n", m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond, acc, m_cIP ) ;
		//fflush( m_fpErrmsg ) ;
	}
	catch ( ... ) {
		printf( "EXCEPTION : write_LoginTry ( code = %d )\n", GetLastError() ) ;
	}

	LeaveCriticalSection( &m_csFile ) ;
}

void _h_ErrorRecord::write_LoginResult( char * acc, int uid, unsigned int ip, bool bSuccess /* = true */ )
{
	EnterCriticalSection( &m_csFile ) ;	

	try {

		GetLocalTime( &m_time ) ;		
		ip_char( ip, m_cIP ) ;

		if( bSuccess ) {
			fprintf( m_fpErrmsg, "[%02d-%02d%02d%02d] LoginResult=Ok, acc=%s, ip=%s, uid=%d\n", m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond, acc, m_cIP, uid ) ;
		}
		else {
			fprintf( m_fpErrmsg, "[%02d-%02d%02d%02d] LoginResult=No, acc=%s, ip=%s, res=%d\n", m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond, acc, m_cIP, uid ) ;
		}

		//fflush( m_fpErrmsg ) ;
	}
	catch ( ... ) {
		printf( "EXCEPTION : write_LoginResult ( code = %d )\n", GetLastError() ) ;
	}


	LeaveCriticalSection( &m_csFile ) ;
}

void _h_ErrorRecord::write_SendError( int err )
{
	EnterCriticalSection( &m_csFile ) ;

	try {
		GetLocalTime( &m_time ) ;
		fprintf( m_fpErrmsg, "[%02d-%02d%02d%02d] Exception, Send Error, Code=%d\n", m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond, err ) ;
		//fflush( m_fpErrmsg ) ;
	}
	catch ( ... ) {
		printf( "EXCEPTION : write_SendError\n" ) ;
	}

	LeaveCriticalSection( &m_csFile ) ;
}


void _h_ErrorRecord::write_RecvError( int err, int sub )
{
	EnterCriticalSection( &m_csFile ) ;		
	
	try {
		GetLocalTime( &m_time ) ;
		fprintf( m_fpErrmsg, "[%02d-%02d%02d%02d] Thread Error, Code=%d, Sub=%d\n", m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond, err, sub ) ;
		//fflush( m_fpErrmsg ) ;
	}
	catch ( ... ) {
		printf( "EXCEPTION : write_SendError\n" ) ;
	}
	
	LeaveCriticalSection( &m_csFile ) ;
}



void _h_ErrorRecord::write_connect( unsigned int ip, unsigned short port )
{
	EnterCriticalSection( &m_csFile ) ;
	try {
		GetLocalTime( &m_time ) ;	
		ip_char( ip, m_cIP ) ;
		fprintf( m_fpErrmsg, "[%02d-%02d%02d%02d] Connect ip = %s, port = %u\n", m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond, 
			m_cIP, port ) ;
		//fflush( m_fpErrmsg ) ;
	}
	catch ( ... ) {
		printf( "EXCEPTION : write_connect\n" ) ;
	}


	LeaveCriticalSection( &m_csFile ) ;
}



/* =====================================================================================
	write_result : 로그인 시도 및 결과를 씀.
===================================================================================== */
void _h_ErrorRecord::write_Result( char * acc, int uid, unsigned int ip, unsigned short port, int err, char * type ) 
{
	EnterCriticalSection( &m_csFile ) ;
	
	try {
		GetLocalTime( &m_time ) ;	
		ip_char( ip, m_cIP ) ;
		fprintf( m_fpErrmsg, "%02d-%02d %d:%02d,%s,%s(%d),%s,%u\n", 
			m_time.wMonth, m_time.wDay, m_time.wHour, m_time.wMinute, acc, type, err, m_cIP, port ) ;
		//fflush( m_fpErrmsg ) ;
	}
	catch ( ... ) {
		printf( "EXCEPTION : write_result\n" ) ;
	}
	
	
	LeaveCriticalSection( &m_csFile ) ;
}


/* =====================================================================================
write_Exception : 예외 오류
===================================================================================== */
void _h_ErrorRecord::write_exception( char * errMsg )
{
	EnterCriticalSection( &m_csFile ) ;

	
	try {
#ifdef _TEST_
		printf( errMsg ) ;
#endif
		fprintf( m_fpErrmsg, errMsg ) ;
		//fflush( m_fpErrmsg ) ;
	}
	catch ( ... ) {
		printf( "EXCEPTION : write_connect\n" ) ;
	}


	LeaveCriticalSection( &m_csFile ) ;
}




void ip_char(DWORD ip, char *buf)
{
	unsigned int temp ;
	int i, j = 0 ;
	char v1, v2 ;
	char * p = buf ;

	for( i = 0 ; i < 4 ; i++ )
	{
		temp = ( ip >> (i*8) ) & 0x000000ff ;

		v1 = (temp / 100) % 10 ;
		if( v1 )
		{
			p[j] = v1 + '0' ;
			++j ;
		}
		temp = temp % 100 ;

		v2 = temp / 10 ;
		if( v1 || v2 )
		{
			p[j] = v2 + '0' ;
			++j ;
		}

		p[j] = temp % 10 + '0' ;
		++j ;

		p[j] = '.' ;
		++j ;
	}

	i = j-1 ;
	while( i < 16 )
	{
		p[i] = 0 ;
		++i ;
	}
}

/*
char * ip_char( unsigned int ip ) 
{
	static char buf[16] ;
	ip_char( ip, buf ) ;
	return buf ;
}
*/



//#################################################################################
// PrintConsole : 콘솔 창에 메세지 띄우기
//#################################################################################
void PrintConsole( const char *format,... )
{
	
	static DWORD	dwWritten ;
	static char strconsole[consoloBufSize] ;
	static char cWriteDate[32] ;
	static SYSTEMTIME	time ;
	
	try{
		if( strlen( format ) >= consoloBufSize ) 
		{
			printf( "[Error] over size string (%u) \n", strlen( format )  ) ;
		}
	}
	catch(...){
		printf("[EXCEPTION] %s,%d \n", __FILE__, __LINE__ ) ;
	}
	
	EnterCriticalSection( &g_csDebufPrint ) ;
	int pos = 0 ;
	try{	
		va_list arglist ;		
		va_start(arglist, format) ;
		vsprintf(strconsole, format, arglist) ;
		va_end(arglist) ;
		
		pos = 1 ;
		
		GetLocalTime(&time ) ;
		sprintf( cWriteDate, "[%u]-[%u:%u:%u]", time.wDay, time.wHour, time.wMinute, time.wSecond ) ;
		WriteConsole( hOut2, cWriteDate, (DWORD)strlen( cWriteDate ), &dwWritten, NULL ) ;
		WriteConsole( hOut2, strconsole, (DWORD)strlen( strconsole ), &dwWritten, NULL ) ;
		
#ifdef _CONSOLE_OUPUT_CATCH_LOG_		
		// 콘솔창의 출력을 로그로 남긴다..		
		g_pErrMsg->LinePrint("%s %s", cWriteDate, strconsole) ;
#endif
	}
	catch(...) {
		printf("[EXCEPTION] PrintConsole (%d) \n", pos) ;
	}
	
	
	LeaveCriticalSection( &g_csDebufPrint ) ;
}


/*
	CND_LoginChecker
*/
BOOL CND_LoginChecker::lc_MakePacket( u_int lmbcMode IN, char * passport OUT, int passlength IN, const char * pID IN, const char * pPW IN, const char * pIP IN )
{
	BOOL bRet = TRUE;
	int count;
	int nRand;		

	int Key;
	int temp;

	CopyMemory( &count, pPW, 4 );
	CopyMemory( &nRand, pID, 4 );
	Key = count + nRand;		// ==> Key Value

	count = 0;
	nRand = 0;

	CopyMemory( &passport[0], pID, 8 );
	CopyMemory( &passport[8], pPW, 8 );

	switch( lmbcMode )
	{
	case en_lmbc_nexon:
		{
			passport[7] += passport[0];
			passport[6] += passport[1];
			passport[5] += passport[2];
			passport[4] += passport[3];

			passport[15] += passport[8];
			passport[14] += passport[9];
			passport[13] += passport[10];
			passport[12] += passport[11];

			for( int i = 0 ; i < 4 ; ++i )
			{
				CopyMemory( &temp, &passport[i*4], 4 ) ;
				temp ^= Key ;
				CopyMemory( &passport[i*4], &temp, 4 ) ;
			}
			count += 16;

			nRand = rand() % 8 + 1;
			passport[count] = static_cast<char>(nRand);
			count += nRand;
			CopyMemory( &passport[count], pPW, en_max_lpl + 1 );
			count += en_max_lpl + 1;

			nRand = rand() % 8 + 1;
			passport[count] = static_cast<char>(nRand);
			count += nRand;
			CopyMemory( &passport[count], pID, en_max_lil + 1 );
			count += en_max_lil + 1;
		}
		break;

	case en_lmbc_metel:
		{
			for( int i = 0 ; i < 4 ; ++i )
			{
				CopyMemory( &temp, &passport[i*4], 4 ) ;
				temp ^= Key ;
				CopyMemory( &passport[i*4], &temp, 4 ) ;
			}

			count += 16;

			nRand = rand() % 8 + 1;
			passport[count] = static_cast<char>(nRand);
			count += nRand;
			CopyMemory( &passport[count], pPW, en_max_lpl + 1 );
			count += en_max_lpl + 1;

			nRand = rand() % 8 + 1;
			passport[count] = static_cast<char>(nRand);
			count += nRand;
			CopyMemory( &passport[count], pID, en_max_lil + 1 );
			count += en_max_lil + 1;
		}
		break ;

	case en_lmbc_vina:
	case en_lmbc_rocworks:
		{
			for( int i = 0 ; i < 4 ; ++i )
			{
				CopyMemory( &temp, &passport[i*4], 4 ) ;
				temp ^= Key ;
				CopyMemory( &passport[i*4], &temp, 4 ) ;
			}

			count += 16;

			nRand = rand() % 8 + 1;
			passport[count] = static_cast<char>(nRand);
			count += nRand;
			CopyMemory( &passport[count], pPW, en_max_lpl + 1 );
			count += en_max_lpl + 1;

			nRand = rand() % 8 + 1;
			passport[count] = static_cast<char>(nRand);
			count += nRand;
			CopyMemory( &passport[count], pID, en_max_lil + 1 );
			count += en_max_lil + 1;
		}
		break ;
		
	case en_lmbc_acclaim:
	case en_lmbc_pwc:
	case en_lmbc_russia:
		{
			for( int i = 0 ; i < 4 ; ++i )
			{
				CopyMemory( &temp, &passport[i*4], 4 ) ;
				temp ^= Key ;
				CopyMemory( &passport[i*4], &temp, 4 ) ;
			}

			count += 16;

			nRand = rand() % 8 + 1;
			passport[count] = static_cast<char>(nRand);
			count += nRand;
			CopyMemory( &passport[count], pPW, en_max_lpl + 1 );
			count += en_max_lpl + 1;

			nRand = rand() % 8 + 1;
			passport[count] = static_cast<char>(nRand);
			count += nRand;
			CopyMemory( &passport[count], pID, en_max_lil + 1 );
			count += en_max_lil + 1;
		}
		break ;
	default:
		bRet = FALSE;
	}

	return bRet;
}


BOOL CND_LoginChecker::lc_CheckPakcet( u_int mode IN, char * passport IN )
{
	int			length = 0;
	int			current = 0;
	int			temp;
	BOOL		bRet = FALSE;
	char		check_value[16];
	int			Key;
	int			iIDStart ;
	int			iPWStart ;
	

	// Get and Set PW, ID.
	CopyMemory( &check_value[0], &passport[current], 16 );
	current += 16;

	length = static_cast<int>(passport[current]);
	if( 0 < length && length <= 8 )
	{
		current += length;
		iPWStart = current ;
		CopyMemory( m_password, &passport[current], en_max_lpl+1 );
		if( m_password[en_max_lpl] != 0 )
			PrintConsole( "  - lc - password error\n" );
		current += (en_max_lpl+1);
		
	}
	else
		return FALSE;

	length = static_cast<int>(passport[current]);
	if( 0 < length && length <= 8 )
	{
		current += length;
		iIDStart = current ;
		CopyMemory( m_id, &passport[current], en_max_lil+1 );
		if( m_id[en_max_lil] != 0 )
			PrintConsole( "  - lc - id error\n" );
		current += (en_max_lil+1);
	}
	else
		return FALSE;


	// Get Key.
	CopyMemory( &current, &passport[iPWStart], 4 );
	CopyMemory( &length, &passport[iIDStart], 4 );

	Key = current + length;		// ==> Key Value

	switch(mode)
	{
	case en_lmbc_nexon:
		
		CopyMemory( &passport[0], &passport[iIDStart], 8 );
		CopyMemory( &passport[8], &passport[iPWStart], 8 );

		passport[7] += passport[0];
		passport[6] += passport[1];
		passport[5] += passport[2];
		passport[4] += passport[3];

		passport[15] += passport[8];
		passport[14] += passport[9];
		passport[13] += passport[10];
		passport[12] += passport[11];

		for( current = 0 ; current < 4 ; ++current )
		{
			CopyMemory( &temp, &passport[current*4], 4 );
			temp ^= Key;
			CopyMemory( &passport[current*4], &temp, 4 );
		}

		// 메모리 비교.ㅣ
		if( memcmp( passport, check_value, 16 ) == 0 )
			bRet = TRUE;
		break;
	case en_lmbc_metel:
		CopyMemory( &passport[0], &passport[iIDStart], 8 );
		CopyMemory( &passport[8], &passport[iPWStart], 8 );

		for( current = 0 ; current < 4 ; ++current )
		{
			CopyMemory( &temp, &passport[current*4], 4 );
			temp ^= Key;
			CopyMemory( &passport[current*4], &temp, 4 );
		}

		// 메모리 비교.ㅣ
		if( memcmp( passport, check_value, 16 ) == 0 )
			bRet = TRUE;
		break;
	case en_lmbc_vina:
	case en_lmbc_rocworks:
		CopyMemory( &passport[0], &passport[iIDStart], 8 );
		CopyMemory( &passport[8], &passport[iPWStart], 8 );

		for( current = 0 ; current < 4 ; ++current )
		{
			CopyMemory( &temp, &passport[current*4], 4 );
			temp ^= Key;
			CopyMemory( &passport[current*4], &temp, 4 );
		}

		// 메모리 비교.ㅣ
		if( memcmp( passport, check_value, 16 ) == 0 )
			bRet = TRUE;
		break;
	case en_lmbc_acclaim:
	case en_lmbc_pwc:
	case en_lmbc_russia:
		CopyMemory( &passport[0], &passport[iIDStart], 8 );
		CopyMemory( &passport[8], &passport[iPWStart], 8 );

		for( current = 0 ; current < 4 ; ++current )
		{
			CopyMemory( &temp, &passport[current*4], 4 );
			temp ^= Key;
			CopyMemory( &passport[current*4], &temp, 4 );
		}

		// 메모리 비교.ㅣ
		if( memcmp( passport, check_value, 16 ) == 0 )
			bRet = TRUE;
		break;
	}
	

	return bRet;
}
