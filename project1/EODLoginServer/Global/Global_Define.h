#pragma once

#define _VERSION_	0.8


// 프로그램 초기화 
//#define _TOTAL_VITUAL_MEMORY_SIZE			100U



#define PASS_KEY	1003
#define PING_PORT	21099


//=================================================================================
// accept port
#define LS_ACCEPT_PORT						48300U
#define PS_ACCEPT_PORT						50001U
// Local UPD port
#define UDP_RECV_PORT						17001U
#define UDP_SEND_PORT						17002U


#define IDC_ACCOUNT_DB_CONNECT_STRING		"IDC_ACCOUNT_DB_CONNECT_STRING"
#define EXTRA_ACCOUNT_DB_CONNECT_STRING		"EXTRA_ACCOUNT_DB_CONNECT_STRING"
#ifdef _PD_COMPANY_ACCLAIM_
#define ACCLAIM_ACCOUNT_DB_CONNECT_STRING	"ACCLAIM_ACCOUNT_DB_CONNECT_STRING"
#endif


#define CRYPTO_KEY_FILE					"lump.dat"
#define SQL_CONNECTION_STRING			"nfofile\\data.enc"


#define _FILE_SERVER_ENVIRONMENT_		"nfofile\\ServerEnv.inf"		// txt 형식.
#define _FILE_VERSION_FILE_PATH			"Patch.PSC"
#define _FILE_FILTER_IP_LIST			"nfofile\\iplist.txt"



#define _COUNT_FIELD_					25
//#define _COUNT_MAX_GROUP_				18


/* ============================================================================
네트워크 상수 : 읽기 버퍼, 쓰기 버퍼 크기와 쓰기에 쓸 패킷의 수
============================================================================ */
#define _RECV_BUF_SIZE_					8192


// THREADS MAX COUNT
#define _MAX_UDP_RECV_THREAD_COUNT_		1
#define _MAX_UDP_SEND_THREAD_COUNT_		1
#define _MAX_UDP_WORK_THREAD_COUNT_		16
#define _MAX_TCP_WORK_THREAD_COUNT_		16


//#define _MAX_ACCEPT_				64 * 2
//#define _TEST_



typedef unsigned (__stdcall *PTHREAD_START) (void *) ;

#define BEGINTHREADEX(psa,cbStack,pfnStartAddr,pvParam,fdwCreate,pdwThreadID) \
	((HANDLE) _beginthreadex((void*)(psa),(unsigned)(cbStack), \
	(PTHREAD_START)(pfnStartAddr),(void*)(pvParam), \
	(unsigned)(fdwCreate),(unsigned*)(pdwThreadID)))

struct _CryptoKeyInfo {
	char	x ;
	char	y ;
	char	keyBoxPath[64] ;
};




#ifndef _FREE_LOGIN_MODE
#define _FREE_LOGIN_MODE
extern bool g_bIsFreeLogin ;
#endif
#define IF_FREE_LOGIN			if( g_bIsFreeLogin == true )
#define IF_NOT_FREE_LOGIN		if( g_bIsFreeLogin == false )




#define IF_DSUDPDEBUG			if(g_bDebugGroup[0])
#define IF_LOGINUDPDEBUG		if(g_bDebugGroup[1])

#ifndef _UNICODE
typedef std::string		tstring;
#else
typedef std::wstring	tstring;
#endif

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

