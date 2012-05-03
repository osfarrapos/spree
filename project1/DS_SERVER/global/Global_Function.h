#pragma once

#include "stdafx.h"

#include <Iphlpapi.h>
#include <time.h>

#define SAFE_DELETE(x) if(x) delete x; x = NULL;
#define SAFE_DELETE_ARRAY(x) if(x) delete [] x; x = NULL;

#ifndef SAFE_CLOSE_HANDLE
#define SAFE_CLOSE_HANDLE(h) if((h) != NULL) { CloseHandle(h); (h) = NULL; }
#endif

#ifndef CS_INIT
#define CS_INIT(cs)					InitializeCriticalSectionAndSpinCount(&cs, (0x80000000)|2000)
#define CS_DEINIT(cs)				DeleteCriticalSection(&cs);
#define CS_LOCK(cs)					EnterCriticalSection(&cs);
#define CS_UNLOCK(cs)				LeaveCriticalSection(&cs);
#endif


#define consoloBufSize	2048

// gf_ : global function

typedef unsigned (__stdcall *PTHREAD_START) (void *) ;

#define BEGINTHREADEX(psa,cbStack,pfnStartAddr,pvParam,fdwCreate,pdwThreadID) \
	((HANDLE) _beginthreadex((void*)(psa),(unsigned)(cbStack), \
	(PTHREAD_START)(pfnStartAddr),(void*)(pvParam), \
(unsigned)(fdwCreate),(unsigned*)(pdwThreadID)))



// singleton 객체
template<class T>
class ckSingleton
{
protected:
	static T* ms_obj;		// member static

public:
	ckSingleton()
	{
#ifdef ASSERT
		ASSERT(!ms_obj && "중복 생성 하지마세요.");
#endif

		int offset = (int)(T*)1-(int)(ckSingleton<T>*)(T*)1; // 다중상속을 위해
		ms_obj = (T*)((int)this+offset);
	}

	virtual ~ckSingleton()
	{
#ifdef ASSERT
		ASSERT(ms_obj);
#endif
		ms_obj = NULL;
	}

	inline static T& GetObj()
	{
#ifdef ASSERT
		ASSERT(ms_obj);
#endif
		return (*ms_obj);
	}

	inline static T* GetObjPtr()
	{
		return ms_obj;
	}

	static bool CreateSingleton()
	{
#ifdef ASSERT
		ASSERT(!ms_obj);
#endif
		T* pTemp = new T;
#ifdef ASSERT
		ASSERT(pTemp);
#endif
		return true;
	}

	static void DestroySingleton()
	{
#ifdef ASSERT
		ASSERT(ms_obj);
#endif
		delete ms_obj;
		ms_obj = NULL;
	}
};

template <class T> T* ckSingleton<T>::ms_obj = 0;


//=================================================================================
//  에러가 나면 내용을 파일에 기록한다. 

// #define _FILE_ERROR_MSG_	"errmsg\\errmsg.txt"

class _h_ErrorRecord
{
protected:
	FILE *			m_fpErrmsg ;
	char			m_szBuffer[consoloBufSize] ;
	CTraceAbleLock  m_csFile ;

#define record_lock()	m_csFile.tlock( __FILE__, __LINE__ ) ;
#define record_unlock() m_csFile.tunlock( __FILE__, __LINE__ ) ;

public:
	_h_ErrorRecord() : m_fpErrmsg(NULL) {}
	~_h_ErrorRecord() { 

		SYSTEMTIME curTime ;
		GetLocalTime( &curTime ) ;
		fprintf( m_fpErrmsg, "중지 시간 : %02d월 %02d일 %02d시 %02d분 %02d초 \n", curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond ) ;
		fflush( m_fpErrmsg ) ;

		if( m_fpErrmsg ) fclose( m_fpErrmsg ) ; 		
	}

	// 파일을 생성한다.
	bool Init( char * name ) ;
	void ErrMsg( const char *format,... ) ;		// 에러메세지를 파일에 쓴다. 
} ;



// 초로 계산했을 때.
#define _SECOND_YEAR_		32140800			// 1년 
#define _SECOND_MONTH_		2678400				// 31 일 
#define _SECOND_DAY_		86400				// 1일
#define _SECOND_HOUR_		3600				// 1시간
#define _SECOND_MINUTE_		60					// 1분

#define _GAME_TIME_MONTH_		864000
#define _GAME_TIME_DAY_			28800




inline int getGlobalTimeByIntSec()
{
	time_t	t ;
	time( &t ) ;
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


//#################################################################################
void PrintConsole( const char *format,... ) ;
void PrintAll( const char * format, ... );
//#################################################################################

void ip_char(DWORD ip, char *buf) ;
char * ip_char( unsigned int ip );


inline char * GetMyIP( char mode = 0 /*0:LAN, 1:WAN*/)
{
	// MyIP Address
	static char m_cMyOuterip[17] = {0, } ;		// 외부
	static char m_cMyInnerip[17] = {0, } ;		// 내부

	// 한번이라도 불린다면, m_cMyInnerip에 아이피 정보가 차 있을 것이다. 
	if( m_cMyInnerip[0] )
	{
		if( mode == 0 ) 
		{
			printf( "IP = %s\n", m_cMyInnerip ) ;
			return m_cMyInnerip ;
		}
		else 
		{
			printf( "IP = %s\n", m_cMyOuterip ) ;
			return m_cMyOuterip ;
		}
	}

	int first_part = 0;

	DWORD dwSize = 0 ;
	GetIpAddrTable( NULL, &dwSize, FALSE ) ;
	
	PMIB_IPADDRTABLE pIpAddrTable = (PMIB_IPADDRTABLE)new BYTE[dwSize] ;
	
	char myOutIP[16] ;
	
	if( pIpAddrTable ) {
		
		if( GetIpAddrTable( pIpAddrTable, &dwSize, FALSE ) == NO_ERROR ) {
			
			printf( "pIpAddrTable->dwNumEntries = %u\n", pIpAddrTable->dwNumEntries ) ;
			
			if( pIpAddrTable->dwNumEntries > 0 ) 
			{
				for( int i = 0 ; i < (int)pIpAddrTable->dwNumEntries ; i++ ) 
				{
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
							if( m_cMyInnerip[0] == 0 || myOutIP[2] == '.' )	// 10.10... 로 시작하는 아이피를 우선으로 한다.
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
//#ifdef _DEBUG
//		memcpy( m_cMyInnerip, m_cMyOuterip, 17 ) ;
//#else
		if( m_cMyInnerip[0] == 0  )
		{
			memcpy( m_cMyInnerip, m_cMyOuterip, 17 ) ;
		}
//#endif

		printf( "IP = %s\n", m_cMyInnerip ) ;
		return m_cMyInnerip ;
	}
	else {
		printf( "IP = %s\n", m_cMyOuterip ) ;
		return m_cMyOuterip ;
	}
}


#define DBErrorOutput(hr)	_DBErrorOutput(hr, __FILE__, __LINE__)
inline void _DBErrorOutput(HRESULT hrErr, char * file, int line )
{
	::PrintConsole( "[OLE_ERROR] fine:%s, line%d\n", file, line ) ;

	CDBErrorInfo ErrorInfo;
	ULONG        cRecords;
	HRESULT      hr;
	ULONG        i;
	CComBSTR     bstrDesc, bstrHelpFile, bstrSource;
	GUID         guid;
	DWORD        dwHelpContext;
	WCHAR        wszGuid[40];
	USES_CONVERSION;

	// If the user passed in an HRESULT then trace it
	if (hrErr != S_OK)
		::PrintConsole("OLE DB Error Record dump for hr = 0x%x\n", hrErr);

	LCID lcLocale = GetSystemDefaultLCID();

	hr = ErrorInfo.GetErrorRecords(&cRecords);
	if (FAILED(hr) && ErrorInfo.m_spErrorInfo == NULL)
	{
		::PrintConsole("No OLE DB Error Information found: hr = 0x%x\n", hr);
	}
	else
	{
		for (i = 0; i < cRecords; i++)
		{
			hr = ErrorInfo.GetAllErrorInfo(i, lcLocale, &bstrDesc, &bstrSource, &guid,
				&dwHelpContext, &bstrHelpFile);
			if (FAILED(hr))
			{
				::PrintConsole("OLE DB Error Record dump retrieval failed: hr = 0x%x\n", hr);
				return;
			}
			StringFromGUID2(guid, wszGuid, sizeof(wszGuid) / sizeof(WCHAR));
			::PrintConsole("Row #: %4d Source: \"%s\" Description: \"%s\" Help File: \"%s\" Help Context: %4d GUID: %s\n",
				i, OLE2T(bstrSource), OLE2T(bstrDesc), OLE2T(bstrHelpFile), dwHelpContext, OLE2T(wszGuid));
			bstrSource.Empty();
			bstrDesc.Empty();
			bstrHelpFile.Empty();
		}
		::PrintConsole("OLE DB Error Record dump end\n");
	}
}

#define DBErrorConsole(hr)	_DBErrorConsole(hr, __FILE__, __LINE__)
inline void _DBErrorConsole(HRESULT hrErr, char * file, int line )
{
	printf( "[OLE_ERROR] fine:%s, line%d\n", file, line ) ;

	CDBErrorInfo ErrorInfo;
	ULONG        cRecords;
	HRESULT      hr;
	ULONG        i;
	CComBSTR     bstrDesc, bstrHelpFile, bstrSource;
	GUID         guid;
	DWORD        dwHelpContext;
	WCHAR        wszGuid[40];
	USES_CONVERSION;

	// If the user passed in an HRESULT then trace it
	if (hrErr != S_OK)
		printf("OLE DB Error Record dump for hr = 0x%x\n", hrErr);

	LCID lcLocale = GetSystemDefaultLCID();

	hr = ErrorInfo.GetErrorRecords(&cRecords);
	if (FAILED(hr) && ErrorInfo.m_spErrorInfo == NULL)
	{
		printf("No OLE DB Error Information found: hr = 0x%x\n", hr);
	}
	else
	{
		for (i = 0; i < cRecords; i++)
		{
			hr = ErrorInfo.GetAllErrorInfo(i, lcLocale, &bstrDesc, &bstrSource, &guid,
				&dwHelpContext, &bstrHelpFile);
			if (FAILED(hr))
			{
				printf("OLE DB Error Record dump retrieval failed: hr = 0x%x\n", hr);
				return;
			}
			StringFromGUID2(guid, wszGuid, sizeof(wszGuid) / sizeof(WCHAR));
			printf("Row #: %4d Source: \"%s\" Description: \"%s\" Help File: \"%s\" Help Context: %4d GUID: %s\n",
				i, OLE2T(bstrSource), OLE2T(bstrDesc), OLE2T(bstrHelpFile), dwHelpContext, OLE2T(wszGuid));
			bstrSource.Empty();
			bstrDesc.Empty();
			bstrHelpFile.Empty();
		}
		printf("OLE DB Error Record dump end\n");
	}
}


unsigned __int64 SM_Item_Make_UniqueID() ;


struct _TempEvent
{
	int ItemFType ;
	int ItemSType ;	
	int ItemID ;
	int ItemDur ;
	int SkillID ;
};


bool sendGS(short sSvrNum, char * pMsgBuf, int size);


inline bool IsFriendly( const short sClassProp1, const short sClassProp2 )
{
	bool bIsFriendly = false ;

	if( sClassProp1 > 0 && sClassProp2 < 0 )
	{
		bIsFriendly = false ;
	}
	else if( sClassProp1 < 0 && sClassProp2 > 0 )
	{
		bIsFriendly = false ;
	}
	else
	{
		bIsFriendly = true ;
	}

	return bIsFriendly ;
}


inline bool WhatIsMoonpaProp( const short sClassProp1, const short sClassProp2, OUT short& sClassProp )
{	

	if( IsFriendly(sClassProp1, sClassProp2) == true)
	{
		if( sClassProp1 > 0 || sClassProp2 > 0 )
		{
			sClassProp = 1 ;	// 백도
		}
		else if( sClassProp1 < 0 || sClassProp2 < 0 )
		{
			sClassProp = -1 ;	//흑도
		}
		else
		{
			sClassProp = 0 ;
		}
		return true ;
	}
	else
	{
		sClassProp = 0 ;
		return false ;
	}	
}

struct _sND_AIUS_Grade ;
void SetRealTime( long cur, _sND_AIUS_Grade& time ) ;


struct __inven_uselimit_info_v1;
class CND_RealTime
{
public:
	static void CalcAfterDay( __inven_uselimit_info_v1 * uselimit, u_short dayafter );
} ;



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
char * DBF_GetDBStringName( en_db_string_where where, int n, bool bIDC ) ;
// -1 - fail, 0 - idc, 1 - extra
int DBF_IsIDC( const char * input ) ;
// fail : -1, else return group number
int DBF_GetGroupIndex( const char * input ) ;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
inline void ShowMsg( char * msg )
{
#ifdef _CONSOLE
	printf( "%s\n", msg );
#else
	MessageBox( NULL, msg, "INFO", MB_OK );
#endif
}
