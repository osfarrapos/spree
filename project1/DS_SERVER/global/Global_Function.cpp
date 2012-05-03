#define _WIN32_WINNT	0x500
#define WIN32_LEAN_AND_MEAN
#include "..\stdafx.h"
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#include "Global_Define.h"
#include "Global_Function.h"
#include "Global_UniqueID.h"

extern bool g_bConsoleCapture ;
extern HANDLE hOut2 ;

extern bool g_bCashItemtest;


extern _j_UniqueIDQueue *		g_pUniqueIDQueue ;

WORD MonthDayAdd[13] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
WORD MonthDayAdd2[13] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

int CND_TimeUnitFunc::GetElapsedDay( _sND_TimeUnit& t )
{
	if( 0 == t.uiYear % 4 )
	{
		return static_cast<int>(MonthDayAdd2[t.uiMonth-1] + t.uiDay);
	}
	else
	{
		return static_cast<int>(MonthDayAdd[t.uiMonth-1] + t.uiDay);
	}
}


time_t CND_TimeUnitFunc::GetDiffSecond( _sND_TimeUnit& from, _sND_TimeUnit& to )
{
	// 먼저 선/후 평가.
	if( from.GetValueType() >= to.GetValueType() )
		return 0;

	// 날짜 차이 계산.
	int elapsed1 = GetElapsedDay(from);
	int elapsed2 = GetElapsedDay(to);

	int diffYear = to.uiYear - from.uiYear;
	int yundal = to.uiYear/4 - from.uiYear/4;

	if( 0 == (from.uiYear%4) )
	{
		if( diffYear && elapsed1 < MonthDayAdd2[2] )
			++yundal;
	}
	if( diffYear > 0 && 0 == (to.uiYear%4) )
		--yundal;

	unsigned int diffDay = static_cast<unsigned int>(diffYear*365 + elapsed2 - elapsed1 + yundal);

	//	printf( "  Diff Day = %u\n", diffDay );

	// 시간 차이 계산.
	elapsed1 = from.uiHour*3600 + from.uiMinute*60 + from.uiSecond;
	elapsed2 = to.uiHour*3600 + to.uiMinute*60 + to.uiSecond;

	return (diffDay*86400 + elapsed2 - elapsed1);
};


void CND_TimeUnitFunc::GetTime_BeforeSecond( _sND_TimeUnit& output, _sND_TimeUnit& from, u_short second )
{
	// 이 함수는 second 가 1시간 이내일 때만 작동하게 되어 있다.
	time_t cur;
	time(&cur);
	tm * ptm = localtime( &cur );

	_sND_TimeUnit curTime;
	curTime.uiYear		= ptm->tm_year-100;
	curTime.uiMonth		= ptm->tm_mon+1;
	curTime.uiDay		= ptm->tm_mday;
	curTime.uiHour		= ptm->tm_hour;
	curTime.uiMinute	= ptm->tm_min;
	curTime.uiSecond	= ptm->tm_sec;

	cur += GetDiffSecond( curTime, from );
	cur -= second;

	tm * p = localtime( &cur );

	output.uiYear	= p->tm_year-100;
	output.uiMonth	= p->tm_mon+1;
	output.uiDay	= p->tm_mday;
	output.uiHour	= p->tm_hour;
	output.uiMinute = p->tm_min;
	output.uiSecond = p->tm_sec;
}

void CND_TimeUnitFunc::GetTime_Current( _sND_TimeUnit& output )
{
	time_t cur;
	time(&cur);
	tm * ptm = localtime( &cur );

	output.uiYear		= ptm->tm_year-100;
	output.uiMonth		= ptm->tm_mon+1;
	output.uiDay		= ptm->tm_mday;
	output.uiHour		= ptm->tm_hour;
	output.uiMinute		= ptm->tm_min;
	output.uiSecond		= ptm->tm_sec;
}

void CND_TimeUnitFunc::GetTime_FromSecond( _sND_TimeUnit& output, time_t cur )
{
	tm * ptm = localtime( &cur );

	output.uiYear		= ptm->tm_year-100;
	output.uiMonth		= ptm->tm_mon+1;
	output.uiDay		= ptm->tm_mday;
	output.uiHour		= ptm->tm_hour;
	output.uiMinute		= ptm->tm_min;
	output.uiSecond		= ptm->tm_sec;
}

time_t CND_TimeUnitFunc::Get_CurrentSecond()
{
	time_t cur;
	time(&cur);
	return cur;
}

time_t CND_TimeUnitFunc::Get_FromTimeUnit( _sND_TimeUnit& input )
{
	time_t ret = Get_CurrentSecond();

	// 현재 시간과의 차이를 구하여, 더하거나 빼거나.

	_sND_TimeUnit cur_time;
	GetTime_FromSecond( cur_time, ret );

	if( input.GetValueType() >= cur_time.GetValueType() )
	{
		time_t diff = GetDiffSecond( cur_time, input );
		ret += diff;
	}
	else
	{
		time_t diff = GetDiffSecond( input, cur_time );
		ret -= diff;
	}

	return ret;
}




#ifdef _PD_CHARAC_LTS_
bool _C_CHARAC::lts_add_pk( _sLTS_Data& data )
{
	int slot = -1;
	int i = 0;
	int min_time = 0x7fffffff;
	int min_slot = 0;
	_sLTS_Data * p = NULL;
	bool ret = true;

	charac_lock();

	for( i = 0; i < en_lts_sp_data_count; ++i )
	{
		if( CHARAC_LTS.m_pSpData[i].iCharacUID == data.iCharacUID )
		{
			ret = false;
			break;
		}
	}

	if( ret )
	{
		for( i = 0; i < en_lts_pk_data_count ; ++i )
		{
			p = &CHARAC_LTS.m_pPkData[i];
			// 동일인물?
			if( p->iCharacUID == data.iCharacUID )
			{
				slot = i;
				break;
			}
		}
		if(slot == -1)
		{
			for( i = 0; i < en_lts_pk_data_count ; ++i )
			{
				p = &CHARAC_LTS.m_pPkData[i];
				//빈 슬롯?
				if( p->iCharacUID == 0 )
				{
					slot = i;
					break;
				}
			}
	        
			if(slot == -1)
			{
				for( i = 0; i < en_lts_pk_data_count ; ++i )
				{
					p = &CHARAC_LTS.m_pPkData[i];
					if( p->iStartTrackingTime < min_time )
					{
						//오래된 곳?
						min_time = p->iStartTrackingTime;
						slot = i;
					}
				}
			}

		}

		if( slot == -1 )
			slot = min_slot;

		CopyMemory( &CHARAC_LTS.m_pPkData[slot], &data, sizeof(_sLTS_Data) );

	}

	charac_unlock();

	return ret;
}


bool _C_CHARAC::lts_del_pk( char * pDel )
{
	int i = 0;
	charac_lock();

	for( ; i < en_lts_pk_data_count ; ++i )
	{
		if( pDel[0] == CHARAC_LTS.m_pPkData[i].szCharacName[0] )
		{
			if( 0 == strncmp( pDel, CHARAC_LTS.m_pPkData[i].szCharacName, en_charac_name_length ) )
			{
				ZeroMemory( &CHARAC_LTS.m_pPkData[i], sizeof(_sLTS_Data) );
				break;
			}
		}
	}

	charac_unlock();

	return true;
}


bool _C_CHARAC::lts_add_sp( _sLTS_Data& data )
{
	bool ret = true;

	charac_lock();

	//자료가 오래된 것 삽입한다.
	int slot = -1;
	int i = 0;
	int min_time = 0x7fffffff;
	int min_slot = 0;
	_sLTS_Data * p = NULL;

	for( ; i < en_lts_sp_data_count ; ++i )
	{	
		for( i = 0; i < en_lts_sp_data_count ; ++i )
		{
			p = &CHARAC_LTS.m_pSpData[i];
			if( p->iStartTrackingTime < min_time )
			{
				//오래된 곳?
				min_time = p->iStartTrackingTime;
				slot = i;
			}
		}
	}

	if( slot == -1 )
		slot = min_slot;

	CopyMemory( &CHARAC_LTS.m_pSpData[slot], &data, sizeof(_sLTS_Data) );

	charac_unlock();

	return ret;
}


bool _C_CHARAC::lts_del_sp( char * pDel )
{
	int i = 0 ;
	bool ret = true;

	charac_lock();

	for( ; i < en_lts_sp_data_count ; ++i )
	{
		if( pDel[0] == CHARAC_LTS.m_pSpData[i].szCharacName[0] )
		{
			if( 0 == strncmp( pDel, CHARAC_LTS.m_pSpData[i].szCharacName, en_charac_name_length ) )
			{
				ZeroMemory( &CHARAC_LTS.m_pSpData[i], sizeof(_sLTS_Data) );
				break;
			}
		}
	}

	charac_unlock();


	return ret;
}
#endif


unsigned short gf_Get_CurrTime( unsigned int currTick )
{
	return (unsigned short) ( ( currTick / 1000U ) % 7200U ) ;
}


//#################################################################################
//
//	_h_ErrorRecord
//
//###################################################################### LHS Added....
bool _h_ErrorRecord::Init( char * name )
{
	TRY{
		SYSTEMTIME curTime ;
		GetLocalTime( &curTime ) ;

		char filename[40] ;
		sprintf( filename, "logfile\\%s%02d%02d_%2dh%2dm.txt"
			, name, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute ) ;

		if( ( m_fpErrmsg = fopen( filename, "at" ) ) == NULL )
			return false ;

		fprintf( m_fpErrmsg, "가동 시간 : %02d월 %02d일 %02d시 %02d분 %02d초 \n"
			, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond ) ;
		fflush( m_fpErrmsg ) ;
		

		return true ;
	}
	CATCH_ALL {
		::PrintConsole("_h_ErrorRecord::Init") ;
		return false ;
	}
	
}

void _h_ErrorRecord::ErrMsg( const char *format,... )
{	

	if( strlen( format ) >= consoloBufSize ) 
	{
		printf( "[Error] over size string (%u) \n", strlen( format )  ) ;
	}

	record_lock() ;
	TRY{
		va_list arglist ;
		va_start(arglist, format) ;
		vsprintf(m_szBuffer, format, arglist) ;
		va_end(arglist) ;

		fputs( m_szBuffer, m_fpErrmsg ) ;
		fflush( m_fpErrmsg ) ;
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _h_ErrorRecord::ErrMsg \n") ;
	}
	

	record_unlock() ;
}


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

	print_lock() ;
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

		if( g_bConsoleCapture ){
			pos = 2 ;
			// 콘솔창의 출력을 로그로 남긴다..
			g_pSystemLog->LogFileWrite( "%s %s", cWriteDate, strconsole);
		}
	}
	catch(...) {
		g_pSystemLog->LogFileWrite("[EXCEPTION] PrintConsole (%d) \n", pos) ;
	}

	print_unlock() ;
}

void PrintAll( const char * format, ... )
{
	static DWORD	dwWritten ;
	static char strconsole[consoloBufSize] ;
	static char cWriteDate[32] ;
	static SYSTEMTIME	time ;

	print_lock() ;
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

		g_pSystemLog->LogFileWrite( "%s %s", cWriteDate, strconsole);
	}
	catch(...) {
		g_pSystemLog->LogFileWrite("[EXCEPTION] PrintConsole (%d) \n", pos) ;
	}
	print_unlock() ;
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


char * ip_char( unsigned int ip ) 
{
	static char buf[16] ;
	ip_char( ip, buf ) ;
	return buf ;
}


unsigned __int64 SM_Item_Make_UniqueID()
{
	static u_short	  usItemUID = 1;
	unsigned __int64 ui64Temp = 0L;
	SYSTEMTIME Date;
	GetSystemTime(&Date);
	u_char  * pcTemp = NULL;
	u_short * psTemp = NULL;
	

	pcTemp    = reinterpret_cast<u_char*>( &ui64Temp );
	pcTemp[0] = 100 + g_ForestNum ; //static_cast<u_char>( g_sServerNo );
	pcTemp[1] = static_cast<u_char>( Date.wYear - 2000 );
	pcTemp[2] = static_cast<u_char>( Date.wMonth );
	pcTemp[3] = static_cast<u_char>( Date.wDay );
	pcTemp[4] = static_cast<u_char>( Date.wHour );
	pcTemp[5] = static_cast<u_char>( Date.wMinute );

	psTemp    = reinterpret_cast<u_short*>( &pcTemp[6] );
	*psTemp   = usItemUID ; 	
	++usItemUID;

	return ui64Temp;
}

void SetRealTime( long cur, _sND_AIUS_Grade& time )
{
	tm * pt = localtime( &cur ) ;
	time.uiYear = pt->tm_year - 100 ;
	time.uiMonth = pt->tm_mon + 1 ;
	time.uiDay = pt->tm_mday ;
	time.uiHour = pt->tm_hour ;
	time.uiMinute = pt->tm_min ;
	time.uiSecond = pt->tm_sec ;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



char * DBF_GetDBStringName( en_db_string_where where, int n, bool bIDC )
{
	static char s_cDBStringName[64] ;

	if( n > 1000 || n < 0 )
	{
		s_cDBStringName[0] = 0 ;
		return &s_cDBStringName[0] ;
	}

	switch( where )
	{
	case game_db_string:
		sprintf( s_cDBStringName, "%s_%s_%d", bIDC ? "IDC" : "EXTRA", "GAME_DB_CONNECT_STRING", n ) ;
		break ;
	case account_db_string:
		sprintf( s_cDBStringName, "%s_%s", bIDC ? "IDC" : "EXTRA", "ACCOUNT_DB_CONNECT_STRING" ) ;
		break ;
	case log_db_string:
		sprintf( s_cDBStringName, "%s_%s_%d", bIDC ? "IDC" : "EXTRA", "LOG_DB_CONNECT_STRING", n ) ;
		break ;
	case event_db_string:
		sprintf( s_cDBStringName, "%s_%s", bIDC ? "IDC" : "EXTRA", "EVENT_DB_CONNECT_STRING" ) ;
		break ;
/*
#ifdef _CIS_20070326_
	case cis_db_string:
		sprintf( s_cDBStringName, "%s_%s", bIDC ? "REAL" : "TEST", "CIS_DB_CONNECT_STRING" ) ;
		break;
#endif
*/
	}

	return &s_cDBStringName[0] ;
}



int DBF_IsIDC( const char * input )
{
	char temp[6] ;
	strncpy( temp, input, 5 ) ;

	for( int i = 0 ; i < 5 ; ++i )
	{
		temp[i] = toupper(temp[i]) ;
	}

	if( strncmp( temp, "IDC", 3 ) == 0 )
		return 0 ;
	else if( strncmp( temp, "EXTRA", 5 ) == 0 )
		return 1 ;
	else
		return -1 ;
}



int DBF_GetGroupIndex( const char * input )
{
	int i ;
	int len = strlen( input ) ;
	for( i = 0 ; i < len ; ++i )
	{
		if( isdigit(input[i]) )
			break ;
	}

	if( i == len )
		return -1 ;

	return atoi( &input[i] ) ;
}


void CND_RealTime::CalcAfterDay( __inven_uselimit_info_v1 * uselimit, u_short dayafter )
{
	if( NULL == uselimit )
		return;
	ZeroMemory(uselimit, sizeof(__inven_uselimit_info_v1) );

	time_t org_time;
	time(&org_time);

	if( g_bCashItemtest )
		org_time = 5*60*dayafter + org_time;
	else
		org_time = _SECOND_DAY_*dayafter + org_time;
	

	tm * pt = localtime( &org_time );

	/*
	struct __inven_uselimit_info_v1	// 사용제한 등의 기타 정보.
{
	u_char				ucFlag1 ;		// Flag1 = 사용기한이 있는가 없는가..
	u_char				ucFlag2 ;		// 
	u_char				ucFlag3 ;
	u_short				usValue1 ;		// 

	u_char				ucYear ;		// Year - 1900 : 5 ~ 시작.
	u_char				ucMonth ;		// Month : 0 - 11
	u_char				ucDay ;			// Day
	u_char				ucHour ;		// Hour
	u_char				ucMinute ;		// Minute 
} ;

  struct tm {
        int tm_sec;     // seconds after the minute - [0,59]
        int tm_min;     // minutes after the hour - [0,59] 
        int tm_hour;    // hours since midnight - [0,23]
        int tm_mday;    // day of the month - [1,31]
        int tm_mon;     // months since January - [0,11]
        int tm_year;    // years since 1900 
        int tm_wday;    // days since Sunday - [0,6] 
        int tm_yday;    // days since January 1 - [0,365] 
        int tm_isdst;   // daylight savings time flag 
        };
	*/
	
	uselimit->ucFlag1 = 1;		// 기간제 아이템 기본값.

	uselimit->ucYear = static_cast<u_char>( pt->tm_year );
	uselimit->ucMonth = static_cast<u_char>( pt->tm_mon + 1 );
	uselimit->ucDay = static_cast<u_char>( pt->tm_mday );
	uselimit->ucHour = static_cast<u_char>( pt->tm_hour );
	uselimit->ucMinute = static_cast<u_char>( pt->tm_min );
}