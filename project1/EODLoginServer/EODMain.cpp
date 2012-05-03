#define _EXCLUDE_FILE_OBJ

#include "stdafx.h"
//#include <iostream>
#include "MyServer\myServer.h"
#include "Ole-DB\db_command.h"
#include "Packet_Queue\Packet_Queue.h"
#include "Timer\TimeOut.h"
#include "Global\NDTrace.h"
#include "Billing\BillingSvrLink.h"
#include "ole-db\sql_convert.h"
#ifdef _IP_FILTER_
#include "global\IP_Filter.h"
#endif

#include "RocWorks/RocAuthSession.h"


#ifdef _METEL_LOGIN_200608_
#pragma comment(lib, "GameAPI.lib")

void BillingIdentifyCallback(int iResult, char* szResultMsg, char* szUserID, int iBillingType, int iPoints, int iFreePoints);
void BillingLoginCallback(int iResult, char* szResultMsg, int iLoginSN, int iBillingType, int iUsedPoints, int iPoints, int iUsedFreePoints, int iFreePoints, TGATime *lpTime, TGATime* lpEndTime);
void BillingLogoutCallback(int iResult, char* szResultMsg, int iLoginSN, int iUsedPoints, int iPoints, int iUsedFreePoints, int iFreePoints, TGATime *lpTime, TGATime* lpEndTime);

unsigned __stdcall MetelLoginProc(void* pVoid);

#include "./Odbc/ODBC_.h"
extern COdbc*	g_pDummyAccount;
#include <set>
extern std::set<DWORD/*PCBang IP LIst*/>	g_setPCBangIp;
extern CRITICAL_SECTION						g_csPCBangIp;
BOOL GetPCBangIpList(COdbc* pDB, std::set<DWORD>* pIpSet);

extern CRITICAL_SECTION						g_csMetelBillingLogin;
extern CRITICAL_SECTION						g_csMetelBillingInfo;
#endif //_METEL_LOGIN_200608_

LONG	ND_Net::CSocketBuffer::m_glCreateCount = 0 ;
char * traceName[_TRACE_CLASS_MAX] = { "NONE", "ERROR", "DB", "LOGIN","LOGIN_SUCCESS","SYSTEM","BILLING" } ;

int	   traceColor[_TRACE_CLASS_MAX] = { _ns_trace_::_FONT_COLOR_RED,
										_ns_trace_::_FONT_COLOR_RED, 
										_ns_trace_::_FONT_COLOR_BLACK, 
										_ns_trace_::_FONT_COLOR_BLACK, 
										_ns_trace_::_FONT_COLOR_BLACK, 
										_ns_trace_::_FONT_COLOR_BLACK 
} ;

bool				g_bContinue			= true ;		// 서버 온 상태


CLinePrintFile *		g_pErrMsg			= NULL ;		// 에러메세지 출력기
CMYTrace *				g_pTrace			= NULL ;		// trace

//_h_VMManager *			g_pVMManager		= NULL ;		// Virtual memory Alloc

// UDP packet queue
_j_queue_UDP_Packet *	g_pUDPSendPacketQ	= NULL ;
_j_queue_UDP_Packet *	g_pUDPRecvPacketQ	= NULL ;
// PacketQueue
_queue_Packet *			g_pLSPacketQueue	= NULL ;


_j_UserQueue *			g_pUserQueue		= NULL ;
myServer *				g_pLSServer			= NULL ;
PSServer *				g_pPSServer			= NULL ;

_h_ServerInfo *			g_pServerInfo		= NULL ; 
//_h_DS_Com *				g_pDSCom			= NULL ;
// OLE DB Connector
CDB_Connector	*		g_pDBCon			= NULL ;
COLE_DB_Executer *		pDBExecutor  = NULL ;


HANDLE					g_hCreateEvent		= NULL ;
_j_Templet_Timer *		g_pTimeOut			= NULL ;

CCapsulateCrypto	*	g_pCapsulateCrypto	= NULL ;

#ifdef _PD_COMPANY_ACCLAIM_
COLE_DB_Executer *		g_pMainAcclaimDBCon = NULL ; 
#endif

#ifdef _IP_FILTER_
CIpFilter *				g_pIPFilter = NULL ;
#endif


int nHeaderSize = sizeof(_sHeader) ;
char g_cVersinFile[128] = {0} ;

u_int					g_uiCompanyCode = 0 ;
int						g_iUseAcclaimAuth = 0 ;


//////////////////////////////////////////////////////////////////////////
DWORD WINAPI th_AcceptThread( LPVOID lp ) ;
DWORD WINAPI th_PS_AcceptThread( LPVOID lp ) ;
// UDP socket Threads
DWORD WINAPI th_UDPRecvThread( LPVOID lp ) ;
DWORD WINAPI th_UDPSendThread( LPVOID lp ) ;
DWORD WINAPI th_UDPRecvPacketWorkThread( LPVOID lp ) ;
// TCP socket Threads
DWORD WINAPI th_TCPWorkThread( LPVOID lp ) ;
// Version reload thread
DWORD WINAPI th_VersionReloadThread( LPVOID lp ) ;
// Pint Thread
DWORD WINAPI th_UDPPingThread( LPVOID lp ) ;



//=================================================================================
// console Control...
#define ASCII_CODE_ESC			27
#define ASCII_CODE_ENTER		13
#define ASCII_CODE_BACKSPACE	8

HANDLE hOut1, hOut2 ;
CRITICAL_SECTION		g_csDebufPrint ;


//=================================================================================
// Global Functions
// Console..
int ProcessCommand( char * cmd ) ;
BOOL KeyboardHandler(DWORD dwEvent ) ;
int GetVersionFile(char * fileName) ;
int SetVersionFile(char * fileName, int iVersion) ;	

void DestroyGlobalObject() ;

bool bFreeLoginServer = false ;
bool bTestLoginServer  = false ;		// 수련 서버( 혹은 내부 서버 ) 로그인 인가.
bool g_bIsFreeLogin = false ;

/*#################################################################################

	EODMain.cpp - Login Server Main 함수.

#################################################################################*/
int main( int argc, char * argv[] )
{
	// Console...
	InitializeCriticalSectionAndSpinCount( &g_csDebufPrint, (0x80000000)|2000 ) ;

	DWORD dwTemp ;
	HANDLE hIn ;
	char cmdbuf[128] = {0} ;
	INPUT_RECORD rec ;
	CONSOLE_SCREEN_BUFFER_INFO Info ;
	bool bInputMode = false ;		// 입력 모드냐 출력 모드냐.( 입력모드이면 화면1, 출력모드이면 화면2 )
	char c ;
	int writepos = 0;
	int ret = 0;	

	//int currServerNumber = 0 ;
	DWORD dwThreadID ;
	HANDLE hThread ;
	int i = 0 ;

	g_pErrMsg = new CLinePrintFile ;
	assert( g_pErrMsg ) ;
	if( g_pErrMsg->Create( "LogFiles\\system_error", "at", "TXT") == false ) 
	{
		printf( "Please Make Folder 'LogFiles'\n" ) ;
		goto error_leave ;
	}
	
	g_pCapsulateCrypto = new CCapsulateCrypto( 0 ) ;
	if( g_pCapsulateCrypto->Init( "lump.dat" ) == false )
	{
		printf( "Working Folder - lump.dat : File not found\n" );
		g_pErrMsg->LinePrint( "g_pCapsulateCrypto->Init( 'lump.dat' ) == false\n" ) ;
		goto error_leave ;
	}
	
	g_uiCompanyCode = en_lmbc_nexon ;

#ifdef _PD_COMPANY_METEL_
	g_uiCompanyCode = en_lmbc_metel ;
	
#ifndef _PD_INNER_TEST_		// ! ndef!!!

#ifdef _METEL_LOGIN_200608_
	InitializeCriticalSectionAndSpinCount(&g_csPCBangIp, (0x80000000)|2000);
	InitializeCriticalSectionAndSpinCount(&g_csMetelBillingLogin, (0x80000000)|2000);
	InitializeCriticalSectionAndSpinCount(&g_csMetelBillingInfo, (0x80000000)|2000);

	// Dummy Account DB에 접속
	g_pDummyAccount = new COdbc;
	if(FALSE == g_pDummyAccount->Connect(_T("DummyAccount"), _T("gamedbo"), _T("~ckdlsk)(zld"))) {
		g_pErrMsg->LinePrint( "Fail to connect to DummyAccount DB!!\n" );
		Logout(_T("Fail to connect to DummyAccount DB!!"));
	}
	else {
		g_pErrMsg->LinePrint( "Success to connect to DummyAccount DB!!\n" );
		Logout(_T("Success to connect to DummyAccount DB!!"));

		// get&set PCBang IP List
		g_setPCBangIp.clear();
		if(FALSE == GetPCBangIpList(g_pDummyAccount, &g_setPCBangIp)) {
			g_pErrMsg->LinePrint( "Fail to get the list of PCBang Ip!!\n" );
			Logout(_T("Fail to get the list of PCBang Ip!!"));
		}
	}

	// init Billing API
	ret = InitBilling(-1, BillingIdentifyCallback, BillingLoginCallback, BillingLogoutCallback);
	if(ret < 0) {
		printf("Fail to InitBilling() \n") ;
		g_pErrMsg->LinePrint( "Fail to InitBilling()\n" );
		goto error_leave ;
	}
	else {
		printf("Success to InitBilling() \n") ;
		
		_beginthreadex(NULL, 0, MetelLoginProc, NULL, 0, NULL);
		printf("Run MetelLoginProc() \n") ;
	}
#endif //_METEL_LOGIN_200608_

#endif	// n : _PD_INNER_TEST_

#endif //_PD_COMPANY_METEL_

#ifdef _PD_COMPANY_VINA_	
	g_uiCompanyCode = en_lmbc_vina;
#endif

#ifdef _PD_COMPANY_ACCLAIM_
	g_uiCompanyCode = en_lmbc_acclaim;
#endif
	
#ifdef _PD_COMPANY_PWC_
	g_uiCompanyCode = en_lmbc_pwc;
#endif
	
#ifdef _PD_COMPANY_JAPAN_
	g_uiCompanyCode = en_lmbc_rocworks;
#endif

#ifdef _PD_COMPANY_RUSSIA_
	g_uiCompanyCode = en_lmbc_russia;
#endif



	g_pTrace = new CMYTrace ;
	assert( g_pTrace ) ;	
	for( i = 0 ; i < _TRACE_CLASS_MAX ; i++ )
	{
		g_pTrace->SetHeadTitle( i , traceName[i] ) ;
	}			

	printf( "## EOD Login Server - Version %f ##\n\n", _VERSION_ ) ;
	printf( " - Crypto Modify\n" );
	g_pErrMsg->LinePrint( "## EOD Login Server - Version %f ##\n\n", _VERSION_ ) ;
	g_pErrMsg->LinePrint( " - Crypto Modify\n" );


	if( argv[3] )
	{
		sprintf( g_cVersinFile, "%s%s", argv[3], _FILE_VERSION_FILE_PATH ) ;
	}
	else
	{
		sprintf( g_cVersinFile, "%s", _FILE_VERSION_FILE_PATH ) ;
	}
	
#ifdef _IP_FILTER_
	g_pIPFilter = new CIpFilter(_FILE_FILTER_IP_LIST) ;
	assert(g_pIPFilter) ;	
#endif

	
	{
		WCHAR indyAccString[512] = {0, } ;
		
		// db connection string
		CSqlConnectorString	sqlConStr( CRYPTO_KEY_FILE ) ;	//CRYPTO_KEY_FILE			"lump.dat"		
		sqlConStr.ReadConStrFile( SQL_CONNECTION_STRING ) ;	//SQL_CONNECTION_STRING		"nfofile\\data.enc"
			

		// SQL Server link
		if( strcmp( argv[2], "idc" ) == 0 ) 
		{
			MultiByteToWideChar(CP_ACP, 0, sqlConStr.GetConStr(IDC_ACCOUNT_DB_CONNECT_STRING)
				, -1, indyAccString, strlen(sqlConStr.GetConStr(IDC_ACCOUNT_DB_CONNECT_STRING) ) ) ;
			printf( "The idc link !\n" ) ;
			g_pErrMsg->LinePrint( "The idc link !\n" ) ;
		}	
		else if( strcmp( argv[2], "extra" ) == 0)
		{
			MultiByteToWideChar(CP_ACP, 0, sqlConStr.GetConStr(EXTRA_ACCOUNT_DB_CONNECT_STRING)
				, -1, indyAccString, strlen(sqlConStr.GetConStr(EXTRA_ACCOUNT_DB_CONNECT_STRING) ) ) ;
			printf( "The extra link !\n" ) ;
			g_pErrMsg->LinePrint( "The extra link !\n" ) ;
		}		
		else
		{
			g_pErrMsg->LinePrint( "Not set DB option param !\n" ) ;
			goto error_leave ;
		}


		if( strcmp( argv[3], "-gm" ) == 0 )
		{
			printf( "THE FREE LOGIN SERVER FOR GM\n" ) ;
			g_pErrMsg->LinePrint( "THE FREE LOGIN SERVER FOR GM\n" ) ;
			bFreeLoginServer = true ;
		}

		if( strcmp( argv[4], "-test") == 0 )
		{
			printf( "THE TEST LOGIN SERVER FOR TEST\n" ) ;
			g_pErrMsg->LinePrint( "THE TEST LOGIN SERVER FOR TEST\n" ) ;
			bTestLoginServer = true ;
		}

		// Create DB Connection object
		printf( "DB Connect - Account DB\n" ) ;
		g_pErrMsg->LinePrint( "DB Connect - Account DB\n" ) ;

		g_pDBCon = new CDB_Connector() ;
		if(!g_pDBCon->initialize( indyAccString ))
		{	
			char str[256] = {0};
			WideCharToMultiByte(CP_ACP, 0, indyAccString, -1, str, 256, NULL, NULL);

			g_pErrMsg->LinePrint("Error : Initailize OLE-DB failed. indyAccString=%s \n", str) ;
			//printf("Press any key to continue. \n");
			//std::cin.get();
			goto error_leave;
		}

		sqlConStr.Reset() ;
	}

	pDBExecutor = new COLE_DB_Executer(g_pDBCon) ;
	assert( pDBExecutor ) ;


	g_hCreateEvent = CreateEvent( NULL, FALSE, FALSE, NULL ) ;	// 비신호상태의 자동 이벤트 생성
	if( g_hCreateEvent == INVALID_HANDLE_VALUE ) {
		g_pErrMsg->LinePrint( "Error : CreateEvent Failed\n" ) ;
		printf( "Error : CreateEvent Failed\n" ) ;
		goto error_leave ;
	}


	g_pErrMsg->LinePrint( "[INIT] Template Timer\n" ) ;
	g_pTimeOut = new _j_Templet_Timer ;
	if( g_pTimeOut->create() == false ) {
		printf( "[FAIL] g_pTimeOut->create()\n" ) ;
		g_pErrMsg->LinePrint( "[FAIL] g_pTimeOut->create()\n" ) ;
		goto error_leave ; 
	}


	//Packet pooling
	/* Create Send packet queue for UDP type */	
	printf( "[INIT] Create Send Packet Queue for UDP Type \n" ) ;	
	g_pErrMsg->LinePrint( "[INIT] Create Send Packet Queue for UDP Type \n" ) ;	
	g_pUDPSendPacketQ = new _j_queue_UDP_Packet(MAX_PACKET_COUNT) ;
	assert( g_pUDPSendPacketQ ) ;		
	
	/* Create Recv packet queue for UDP type */	
	printf( "[INIT] Create Recv Packet Queue for UDP Type \n" ) ;	
	g_pErrMsg->LinePrint( "[INIT] Create Recv Packet Queue for UDP Type \n" ) ;	
	g_pUDPRecvPacketQ = new _j_queue_UDP_Packet(MAX_PACKET_COUNT) ;
	assert( g_pUDPRecvPacketQ ) ;		
	
	// create ls packet queue	
	printf( "[INIT] Create Recv Packet Queue for UDP Type \n" ) ;	
	g_pErrMsg->LinePrint( "[INIT] Create Recv Packet Queue for UDP Type \n" ) ;	
	g_pLSPacketQueue = new _queue_Packet(MAX_PACKET_COUNT) ;
	assert( g_pLSPacketQueue ) ;
	
	
	// g_pEnv 
	printf( "Start : Environment_Reader\n" ) ;
	g_pErrMsg->LinePrint( "Start : Environment_Reader\n" ) ;
	g_pServerInfo = new _h_ServerInfo ;
	if( !g_pServerInfo ) 
		goto error_leave ;
	if( g_pServerInfo->create( _FILE_SERVER_ENVIRONMENT_ ) == false ) {
		printf( "==> Error : ServerInfo->create failed\n" ) ;
		goto error_leave ;
	}
	printf( "Complete : Server Info\n" ) ;

			
	// 버젼 읽기
	{
		int iPatchNum ;
		DWORD dwClientVer ;

		if( bTestLoginServer == true )
		{
			pDBExecutor->VersionControl( iPatchNum, dwClientVer, 0, 1 ) ;
			g_pServerInfo->SetVersion( iPatchNum, dwClientVer ) ;	
		}
		else
		{
			pDBExecutor->VersionControl( iPatchNum, dwClientVer, 0, 0 ) ;
			g_pServerInfo->SetVersion( iPatchNum, dwClientVer ) ;	
		}	
		
	}

#ifdef _PD_COMPANY_JAPAN_
	if( FALSE == CRocAuthSession::SetRocworksAuthServerInformation( g_pServerInfo->get_roc_auth_ip(), g_pServerInfo->get_roc_auth_port() ) )
	{
		g_pErrMsg->LinePrint( "[EXIT]  Check RMS Setting\n " );
		printf( "[EXIT]  Check RMS Setting\n " );
		goto error_leave ; 
	}
#endif
	

	// Create user queue
	g_pUserQueue = new _j_UserQueue(_MAX_USER_COUNT_) ;
	if( g_pUserQueue->Init() == false ) {
		g_pErrMsg->LinePrint( HTML("[EXIT]  g_pUserQueue->Init() \n ", _ns_trace_::_FONT_COLOR_RED, _NBLINK_ ) ) ;
		goto error_leave ; 
	}	

	

	printf( "[INFO] CREATE Version Reload Thread!!\n") ;
	// Create Version reload thread 
	hThread = BEGINTHREADEX( NULL, 0, th_VersionReloadThread, NULL, 0, &dwThreadID ) ;
	if( hThread != INVALID_HANDLE_VALUE ) { CloseHandle(hThread) ; hThread = INVALID_HANDLE_VALUE ; }
	else { 
		g_pErrMsg->LinePrint( "[FAIL] Create th_VersionReloadThread failed\n" ) ; 
		goto error_leave ; 
	}	
	printf( " $$ Create VersionReloadThread (1) !!\n") ;

	
	printf( "[INFO] Create Udp send threads!!\n") ;
	g_pErrMsg->LinePrint( "[INFO] Create Udp send threads!!\n") ;
	// Send 스레드 생성 : UDP	
	for ( i = 0 ; i < _MAX_UDP_SEND_THREAD_COUNT_ ; i++ ) {
		hThread = BEGINTHREADEX( NULL, 0, th_UDPSendThread, NULL, 0, &dwThreadID ) ;
		if( hThread != INVALID_HANDLE_VALUE ) { CloseHandle(hThread) ; hThread = INVALID_HANDLE_VALUE ; }
		else { 
			g_pErrMsg->LinePrint( "[FAIL] UDP Send thread failed\n" ) ; 
			goto error_leave ; 
		}
		Sleep(500) ;
		
	}
	printf( " -> Success Create UDPSendThread (%d) !!\n",  _MAX_UDP_SEND_THREAD_COUNT_) ;

	
	printf( "[INFO] Create Udp recv threads!!\n") ;
	g_pErrMsg->LinePrint( "[INFO] Create Udp recv threads!!\n") ;
	for (i = 0 ; i < _MAX_UDP_RECV_THREAD_COUNT_ ; i++ ) {
		// Recv 쓰레드 생성 : UDP	
		hThread = BEGINTHREADEX( NULL, 0, th_UDPRecvThread, NULL, 0, &dwThreadID ) ;
		if( hThread != INVALID_HANDLE_VALUE ) { CloseHandle(hThread) ; hThread = INVALID_HANDLE_VALUE ; }
		else { 
			g_pErrMsg->LinePrint( "[FAIL] UDP Recv thread failed\n" ) ; 
			goto error_leave ; 
		}		
		Sleep(500) ;
	}
	printf( " -> Success Create UDPRecvThread (%d) !!\n",  _MAX_UDP_RECV_THREAD_COUNT_) ;


	printf( "[INFO] Create th_UDPRecvPacketWorkThread!!\n") ;
	g_pErrMsg->LinePrint( "[INFO] Create th_UDPRecvPacketWorkThread!!\n") ;
	for ( i = 0 ; i < _MAX_UDP_WORK_THREAD_COUNT_ ; i++ ) {

		// Work 스레드 생성 : UDP		
		hThread = BEGINTHREADEX( NULL, 0, th_UDPRecvPacketWorkThread, NULL, 0, &dwThreadID ) ;
		if( hThread != INVALID_HANDLE_VALUE ) { CloseHandle(hThread) ; hThread = INVALID_HANDLE_VALUE ; }
		else { 
			g_pErrMsg->LinePrint( "[FAIL] UDP th_UDPRecvPacketWorkThread failed\n" ) ; 
			goto error_leave ; 
		}
	}
	printf( " -> Success Create th_UDPRecvPacketWorkThread (%d) !!\n",  _MAX_UDP_WORK_THREAD_COUNT_) ;
		

	printf( "[INFO] Create th_TCPWorkThread!!\n") ;
	g_pErrMsg->LinePrint( "[INFO] Create th_TCPWorkThread!!\n") ;
	for ( i = 0 ; i < _MAX_TCP_WORK_THREAD_COUNT_ ; i++ ) {
		// TCPWorkThread
		hThread = BEGINTHREADEX( NULL, 0, th_TCPWorkThread, NULL, 0, &dwThreadID ) ;
		if( hThread != INVALID_HANDLE_VALUE ) { CloseHandle(hThread) ; hThread = INVALID_HANDLE_VALUE ; }
		else { 
			g_pErrMsg->LinePrint( "[FAIL] TCP th_TCPWorkThread failed\n" ) ; 
			goto error_leave ; 
		}
	}
	printf( " -> Create TCPWorkThread (%d) !!\n",  _MAX_TCP_WORK_THREAD_COUNT_) ;


	// Ping Thread 시작.
	printf( "[INFO] Create th_UDPPingThread!!\n") ;
	g_pErrMsg->LinePrint( "[INFO] Create th_UDPPingThread!!\n") ;
	hThread = BEGINTHREADEX( NULL, 0, th_UDPPingThread, NULL, 0, &dwThreadID ) ;
	if( hThread == INVALID_HANDLE_VALUE )
	{
		printf("Error : PingThread Failed\n" ) ;
		goto error_leave ;
	}
	printf( " -> Create PingThread (%d) !!\n",  1) ;

	
	CloseHandle( hThread ) ;


//	if( strcmp( argv[2], "idc" ) == 0 ) 
	{
		printf( "[info] Login Accept - TCP( %s, %u )\n" ,GetMyIP(1), LS_ACCEPT_PORT ) ;
		g_pErrMsg->LinePrint( "[info] Login Accept - TCP( %s, %u )\n" ,GetMyIP(1), LS_ACCEPT_PORT ) ;
		// LS create server
		g_pLSServer = new myServer ;
		if( g_pLSServer->CreateServerSocket( 4, GetMyIP(1), (u_short)LS_ACCEPT_PORT, 10000, 10, 10000, 50, 100, 100 ) == FALSE )		
		{
			g_pErrMsg->LinePrint( "[FAIL] g_pServer->CreateServerSocket()\n" );
			goto error_leave ; 
		}

		// PS create server
		printf( "[info] Version Accept - TCP( %s, %u )\n" ,GetMyIP(1), PS_ACCEPT_PORT ) ;
		g_pErrMsg->LinePrint( "[info] Version Accept - TCP( %s, %u )\n" ,GetMyIP(1), PS_ACCEPT_PORT ) ;
		g_pPSServer = new PSServer ;
		if( g_pPSServer->CreateServerSocket( 4, GetMyIP(1), (u_short)PS_ACCEPT_PORT, 10000, 10, 10000, 50, 100, 100) == FALSE )
		{
			g_pErrMsg->LinePrint( "[FAIL] g_pPSServer->CreateServerSocket()\n" );
			goto error_leave ; 
		}
	}	

	// console 을 위하여...
	hIn = GetStdHandle( STD_INPUT_HANDLE ) ;
	hOut1 = GetStdHandle( STD_OUTPUT_HANDLE ) ;
	
	SetConsoleCtrlHandler( (PHANDLER_ROUTINE) KeyboardHandler, TRUE ) ;
	
	hOut2 = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, 0 ) ;
	
	wsprintf( cmdbuf, "\n[cmd] " ) ;
	WriteConsole( hOut1, cmdbuf, (DWORD)strlen(cmdbuf), &dwTemp, NULL ) ;
	
	wsprintf( cmdbuf, " *** Output Screen*** \nPress ESC to go cmd console\n" ) ;
	WriteConsole( hOut2, cmdbuf, (DWORD)strlen(cmdbuf), &dwTemp, NULL ) ;
	
	SetConsoleActiveScreenBuffer( hOut2 ) ;
	
	GetConsoleScreenBufferInfo( hOut1, &Info ) ;
	SetConsoleScreenBufferSize( hOut2, Info.dwSize ) ;
	
#ifdef _PD_VINA_PCROOM_INFO_
	wsprintf( cmdbuf, "- PC Room Version for Vina.\n" ) ;
	WriteConsole( hOut2, cmdbuf, (DWORD)strlen(cmdbuf), &dwTemp, NULL ) ;
#endif
	
	while( g_bContinue ) {
		ReadConsoleInput( hIn, &rec, 1, &dwTemp ) ;
		
		try{
			if( rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown == TRUE ) {
				if( bInputMode ) {	// input 모드 일때만 입력을 처리한다. 
					c = rec.Event.KeyEvent.uChar.AsciiChar ;
					if(  c == ASCII_CODE_ESC ) {	
						bInputMode = false ;
						SetConsoleActiveScreenBuffer( hOut2 ) ;
					}
					else if( c == ASCII_CODE_ENTER ) {
						if( writepos == 0 ) {
							putchar( '\n' ) ;
						}
						else {
							cmdbuf[writepos] = 0 ;
							printf("\nCommand = %s\n", cmdbuf ) ;
							writepos = 0 ;
							
							ret = ProcessCommand( cmdbuf ) ;
							if( ret == 1 ) {
								if( MessageBox( NULL, "Shutdown Server?", "Warnning", MB_OKCANCEL ) == IDOK ) 
								{
									PrintConsole( "[QUIT] From Command\n" ) ; 
									break ;
								}
							}
						}
						printf( "[cmd] " ) ;
					}
					else if( c == ASCII_CODE_BACKSPACE ) {
						if( writepos == 0 )
							continue ;
						else { 
							putchar(c) ;
							putchar(' ') ;
							putchar(c) ;
							writepos-- ;
						}
					}
					else {
						// 모드를 바꾼다. 
						if( writepos >= 80 )
							continue ;
						cmdbuf[writepos] = c ;
						// printf( "%c", c ) ;
						putchar(c) ;
						writepos++ ;
					}
				} // if mode
				else {
					c = rec.Event.KeyEvent.uChar.AsciiChar ;
					if(  c == ASCII_CODE_ESC ) {	
						bInputMode = true ;
						SetConsoleActiveScreenBuffer( hOut1 ) ;
					}
				}
			}
		}
		catch (...) {
			::PrintConsole( "[EXCEPTION] Consele 부분 반복문 예외 오류 발생\n" ) ;
			continue;
		}
		Sleep(5);
	}

error_leave :
	Sleep(10000) ;
	DestroyGlobalObject() ;	
	return 0 ;
}



DWORD WINAPI th_VersionReloadThread( LPVOID lp )
{

	HANDLE hTimer ;
	LARGE_INTEGER li = {0} ;
	int nVersion ;
	DWORD dwClientVer ;

	hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;

	SetWaitableTimer( hTimer, &li, 5000, NULL, NULL, TRUE ) ;

	while (g_bContinue) 
	{

		WaitForSingleObject( hTimer, INFINITE ) ;

		
		if( bTestLoginServer == true )
		{
			pDBExecutor->VersionControl( nVersion, dwClientVer, 0, 1 ) ;			
		}
		else
		{
			pDBExecutor->VersionControl( nVersion, dwClientVer, 0, 0 ) ;			
		}	
		g_pServerInfo->SetVersion( nVersion, dwClientVer ) ;

		
	}

	return 0 ;
}




int GetVersionFile(char * fileName){

	try {
		char buf[16] = {0};
		
		int nVersion = 0 ;
		
				
		FILE *	stream = NULL;
		
		try{
			stream = fopen(fileName,"r");
			//stream = fopen("Patch.PSC","r");
		}
		catch (...) {		
			::PrintConsole("[EXCEPTION] File open exception \n") ;
			//g_pErrorMsg->write_exception("[EXCEPTION] File open exception \n") ;
			fclose(stream);		
			return -1;
		}
		
		if(stream == NULL) {
			::PrintConsole("[EXCEPTION] File stream is NULL \n") ;
			//g_pErrorMsg->write_exception("[EXCEPTION] File stream is NULL \n") ;
			fclose(stream);
			return -1;
		}		
				
		
		if(fgets(buf, sizeof(buf), stream) == NULL) {
			fclose(stream) ;
			::PrintConsole("[ERROR] Version load error \n"  ) ;
			return -1 ;
		}

		fclose( stream ) ;
		nVersion = atoi(buf);	
	
		return nVersion;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] File stream work \n") ;
		return -1 ;
	}
	
}


int SetVersionFile(char * fileName, int iVersion){
	
	try {
		char buf[16] = {0};	

		itoa(iVersion, buf, 10) ;		
		
		FILE *	stream = NULL;
		
		try{
			stream = fopen(fileName,"w");
			//stream = fopen("Patch.PSC","r");
		}
		catch (...) {		
			::PrintConsole("[EXCEPTION] File open exception \n") ;
			//g_pErrorMsg->write_exception("[EXCEPTION] File open exception \n") ;
			fclose(stream);		
			return -1;
		}
		
		if(stream == NULL) {
			::PrintConsole("[EXCEPTION] File stream is NULL \n") ;
			//g_pErrorMsg->write_exception("[EXCEPTION] File stream is NULL \n") ;
			fclose(stream);
			return -1;
		}		
		
		if( fputs( buf, stream ) == EOF )
		{
			fclose(stream) ;
			::PrintConsole("[ERROR] Version Save error \n"  ) ;
			return -1 ;
		}
		
		
		fclose( stream ) ;		
		
		return 0;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] File stream work \n") ;
		return -1 ;
	}
	
}


extern bool g_bDebugGroup[5] ;

int ProcessCommand( char * cmd )
{
	static char command[128] = { 0, } ;
	static char arg1[128] = { 0, } ;
	static char arg2[128] = { 0, } ;
	static char arg3[128] = { 0, } ;
	
	int nString = 0 ;
	int nCount = 0;
	int ret = 0 ;

	char * stateString[] = {"RUN", "OFF", "CHECKUP", "FULL", "?" } ;
	
	
	// Command string parsing 
	nString = sscanf( cmd, "%s %s %s %s", command, arg1, arg2, arg3 ) ;
	if( nString > 0 ) {		// Max 3
		if( strcmp(command, "!") == 0 ) 
		{
			int iPatchNum ;
			DWORD dwClientVer ;
			g_pServerInfo->GetVersion( iPatchNum, dwClientVer ) ;
			printf(" -Info : VersionIndex : %d, %u \n" , iPatchNum, dwClientVer ) ;
			return 1 ;
		}
		else if( strcmp( command, "/") == 0 )
		{			
			printf( "  ->PS Socket queue : %u \n" , g_pPSServer->GetQueueCount() ) ; 
			printf( "  ->LS Socket queue : %u \n\n" , g_pLSServer->GetQueueCount() ) ;

			printf( "  ->send packet queue : %u \n", g_pUDPSendPacketQ->GetCurCount() ) ;
			printf( "  ->recv packet queue : %u \n", g_pUDPRecvPacketQ->GetCurCount() ) ;

		}
		else if( strcmp( command, "db") == 0 )
		{
			if( strcmp( arg1, "reconnect" ) == 0 )
			{				
				if( strcmp( arg2, "-acc" ) == 0 )
				{
					g_pDBCon->CallReConTh() ;
				}
				else
				{
					printf( "==========================[worning]========================== \n ") ;
					printf( "DB Reconnect command is need option ( one choice :  -game, -acc ) \n" ) ;
					printf( "============================================================== \n ") ;
				}
			}
		}
		else if( strcmp( command, "testlogin" ) == 0 )
		{
			if( nString < 4 )
			{
				printf( "\n testlogin have 3 parameters\n" ) ;
			}
			else
			{
				int iPatchNum ;
				DWORD dwClientVer ;
				g_pServerInfo->GetVersion( iPatchNum, dwClientVer ) ;

				MSG_LOGIN login ;
				ZeroMemory( &login, sizeof(login) ) ;
				login.Header.sLength = sizeof(login) ;
				login.ucMessage = MSG_NO_LOGIN ;
				login.uiLMBC_Code = g_uiCompanyCode ;
				login.sVersionIndex = static_cast<short>( iPatchNum ) ;
				login.dwClientVer = dwClientVer ;
				login.cForestSelectNum = 0 ;

				CND_LoginChecker check(g_uiCompanyCode) ;
				check.lc_MakePacket( g_uiCompanyCode, login.passport, _PASSPORT_LENGTH_, arg1, arg2, arg3 ) ;

				_h_User * pUser = g_pUserQueue->GetUser() ;
				pUser->m_id = rand() +1 ;

				g_pLSPacketQueue->insert( pUser, (char*)&login, sizeof(login) ) ;

				printf( "\n Test Login Packet Insert!\n" ) ;
			}
		}
		else if( strcmp( command, "count") == 0 )
		{
			printf(" Connection count : LS(%d), PS(%d \n", g_pLSServer->GetCount(), g_pPSServer->GetCount() ) ;
		}
		else if( strcmp(command, "shutdown") == 0 ) 
		{
			return 1 ;
		}
		else if( strcmp(command, "quit") == 0 ) 
		{
			return 1 ;
		}		
		else if( strcmp( command, "show") == 0 ) {
			if( strcmp( arg1, "version") == 0 ) {
				int iPatchNum ;
				DWORD dwClientVer ;
				g_pServerInfo->GetVersion( iPatchNum, dwClientVer ) ;
				printf(" -Info : VersionIndex : %d, %u \n" , iPatchNum, dwClientVer ) ;
			}
			else if( strcmp( arg1, "trace") == 0 )
			{
				printf("Title | class | value \n") ;
				_TraceData * pTraceData = g_pTrace->ShowHeadTitleList() ;
				for ( int i = 0 ; i < _TRACE_CLASS_MAX ; i++ )
				{
					printf( "- %s : %d : %d \n", (&pTraceData[i])->cHeadTitle, i, (&pTraceData[i])->bActive) ;
				}
				
			}
			else if( strcmp( arg1, "serverlist") == 0 )
			{
				printf("FOREST   |   STATE(0:RUN, -1:OFF, -2:CHECKUP, -3:FULL), \n") ;
				int state ;				
				char * pStr ;
				for( int i = 0 ; i < _COUNT_MAX_GROUP_ ; i++ )
				{		
					state = g_pServerInfo->GetForestState( i )  ;
					switch( state ) {
					case _FOREST_STATE_RUNNING:
						pStr = stateString[0] ;
						break;
					case _FOREST_STATE_OFF:
						pStr = stateString[1] ;
						continue;
						break;
					case _FOREST_STATE_CHECKUP_:
						pStr = stateString[2] ;
						break;
					case _FOREST_STATE_FULL_:
						pStr = stateString[3] ;
						break;
					default:
						pStr = stateString[4] ;
						break;
					}
					printf(" %d  :  %s		%d per \n", i, pStr, g_pServerInfo->GetForestListPacket()->ForestInfo.cForestState[i] ) ;
				}
			}
			else if( strcmp( arg1, "dbstatus" ) == 0 )
			{
				printf( "\nDB Connection Status\n" ) ;

				if( g_pDBCon->IsConnected() )
					printf( "  Account DB : Connected\n" ) ;
				else 
					printf( "  Account DB : Not Connected\n" ) ;
			}
		}
		if( strcmp( command, "set") == 0 )
		{
			if( strcmp( arg1, "trace") == 0 )
			{
				if( strcmp(arg2, "on") == 0 )
				{
					g_pTrace->SetAllTrace( true ) ;
				}
				else if( strcmp(arg2, "off") == 0 )
				{
					g_pTrace->SetAllTrace( false ) ;
				}
				else
				{
					g_pTrace->ShowTraceString( atoi(arg2), atoi(arg3) > 0 ? true : false ) ;
				}			
			}
			else if( strcmp( arg1, "ds") == 0 )
			{				
				char nForestIndex = static_cast<char>(atoi(arg2)) ;
				char state = static_cast<char>(atoi(arg3)) ;
				if( nForestIndex >= 0 && nForestIndex < _COUNT_MAX_GROUP_ && state <= 0 )
				{
					g_pServerInfo->m_MsgLoginForestList.ForestInfo.cForestState[nForestIndex] = state ;
					g_pServerInfo->m_ForestInfo.cForestState[nForestIndex] = state ; 
				}				
			}
			else if( 0 == strcmp( arg1, "version" ) )
			{
				if( 4 == nString )
				{
					int patch = atoi(arg2) ;
					DWORD client = static_cast<DWORD>(atoi(arg3)) ;
					
					if( bTestLoginServer )
					{
						pDBExecutor->VersionControl( patch, client, 1, 1 ) ;
					}
					else
					{
						pDBExecutor->VersionControl( patch, client, 1, 0 ) ;
					}

					g_pServerInfo->SetVersion( patch, client ) ;					

					int iPatchNum ;
					DWORD dwClientVer ;
					g_pServerInfo->GetVersion( iPatchNum, dwClientVer ) ;
					printf(" Set VersionIndex : %d, %u \n" , iPatchNum, dwClientVer ) ;
				}
				else
				{
					printf( "  => [USAGE] set version mv sv\n" ) ;
				}
			}
		}
		
		else if ( strcmp( command, "logflush") == 0 ) {

		}
		else if( strcmp( command, "debug" ) == 0 )
		{
			if( strcmp( arg1, "dsudp" ) == 0 )
			{
				if( strcmp( arg2, "on" ) == 0 )
				{
					printf( "  ==> DS UDP Debug On!\n" ) ;
					g_bDebugGroup[0] = true ;
				}
				else if( strcmp( arg2, "off" ) == 0 )
				{
					printf( "  ==> DS UDP Debug Off!\n" ) ;
					g_bDebugGroup[0] = false ;
				}
				else
				{
					if( g_bDebugGroup[0] )
					{
						printf( "  ==> Cur DS UDP Debug On\n" ) ;
					}
					else
						printf( "  ==> Cur DS UDP Debug Off\n" ) ;
				}
			}
			else if( strcmp( arg1, "loginudp" ) == 0 )
			{
				if( strcmp( arg2, "on" ) == 0 )
				{
					printf( "  ==> Login UDP Debug On!\n" ) ;
					g_bDebugGroup[1] = true ;
				}
				else if( strcmp( arg2, "off" ) == 0 )
				{
					printf( "  ==> Login UDP Debug Off!\n" ) ;
					g_bDebugGroup[1] = false ;
				}
				else
				{
					if( g_bDebugGroup[1] )
					{
						printf( "  ==> Cur Login UDP Debug On\n" ) ;
					}
					else
						printf( "  ==> Cur Login UDP Debug Off\n" ) ;
				}
			}
			else
			{
				printf( " ==> [USAGE] debug dsudp/loginudp on/off\n" ) ;
			}
		}
		else if( strcmp( command, "use" ) == 0 ) 
		{
			if( strcmp( arg1, "acclaimlogin" ) == 0 )
			{
				int n = atoi( arg2 ) ;
				switch( n )
				{
				case 0:
					g_iUseAcclaimAuth = 0 ;
					printf( "\nUse Acclaim Login : authenticate()\n" ) ;
					break ;
				case 1:
					g_iUseAcclaimAuth = 1 ;
					printf( "\nUse Acclaim Login : authenticate1()\n" ) ;
					break ;
				case -1:
					g_iUseAcclaimAuth = -1 ;
					printf( "\nDon't Use Acclaim Login Function.\n" ) ;
					break ;
				default:
					printf( "\n Third Parameter is incorrect, please enter -1,0,1\n" ) ;
					break ;
				}
			}
			else
			{
				printf( "\nuse command - wrong parameter\n" ) ;
				printf( "[USAGE] use acclaimlogin n\n" ) ;
				printf( "  n = -1 : Don't use authenticate\n" ) ;
				printf( "  n = 0 : use authenticate()\n" ) ;
				printf( "  n = 1 : use authenticate1\n" ) ;
				
			}
		}
	}
	
	return 0 ;
}


BOOL KeyboardHandler(DWORD dwEvent )
{
	switch( dwEvent ) {
	case CTRL_C_EVENT:
		return TRUE ;
		break;
	case CTRL_BREAK_EVENT:
		return TRUE ;
		break;
	case CTRL_CLOSE_EVENT:		
		return TRUE ;
		break;
	}
	
	return TRUE ;
}



void DestroyGlobalObject()
{

	g_bContinue = false ;

	g_pErrMsg->FileClose();

	Sleep( 6000 ) ;		// 쓰레드가 안전하게 종료될때까지 기다린다..

#ifdef _METEL_LOGIN_200608_

#ifndef _PD_INNER_TEST_
	DeleteCriticalSection(&g_csPCBangIp);
	DeleteCriticalSection(&g_csMetelBillingLogin);
	DeleteCriticalSection(&g_csMetelBillingInfo);

	g_setPCBangIp.clear();

	if(g_pDummyAccount != NULL) {
		g_pDummyAccount->Disconnect();
		delete g_pDummyAccount;
		g_pDummyAccount = NULL;
	}

	CloseBilling();
	Logout(_T("Success to CloseBilling()"));

#endif	// _PD_INNER_TEST_

#endif //_METEL_LOGIN_200608_

	SAFE_DELETE( g_pServerInfo ); 	
	//g_pDSCom->releaseDSCom() ;
	//SAFE_DELETE( g_pDSCom ) ;
	
	SAFE_DELETE( g_pDBCon ) ;
	SAFE_DELETE( g_pTrace ) ;
	SAFE_DELETE( g_pErrMsg);		// 에러메세지 출력기	
	// UDP packet queue	
	SAFE_DELETE( g_pUDPSendPacketQ);
	SAFE_DELETE( g_pUDPRecvPacketQ);	
	// PacketQueue	
	SAFE_DELETE( g_pLSPacketQueue );	
	
	SAFE_DELETE( g_pUserQueue );	
	SAFE_DELETE( g_pLSServer );	
	SAFE_DELETE( g_pPSServer );		
	SAFE_DELETE( g_pTimeOut );
	SAFE_DELETE( g_pCapsulateCrypto ) ;
	

	DeleteCriticalSection( &g_csDebufPrint );	


	CloseHandle( g_hCreateEvent ) ;	
	//g_pVMManager->Destroy() ;
	//SAFE_DELETE( g_pVMManager);		// Virtual memory Alloc
}





















