#include "stdafx.h"

#include "Network\Network.h"
//#include "VMManager\VMManager.h"
#include "Map\MapManager.h"
#include "Queue\Packet_Queue.h"
#include "Threads\Backup_Thread.h"
#include "Global\Global_Log_Define.h"
#include "Global\Trace.h"
#include "OLE-DB\sql_convert.h"
#include "MyLibrary/MY_OLE_DB.H"


const char * traceName[_TRACE_CLASS_MAX] = { 
	"ERROR",
	"SYSTEM",
	"DB",
	"MESSAGE"
} ;



//==================================================================
// Object
//_h_VMManager *	g_pVMManager = NULL ;
_j_GlobalLog *	g_pSystemLog = NULL ;
_queue_Packet * g_pPacketQueue = NULL ;
_h_CompletionPort *	g_pIOCP			= NULL ;	// IOCP
CDB_Connector *		g_pDBCon = NULL ;

_DSCom_Queue *	g_pDSComQueue = NULL ;

_j_Templet_Timer * g_pLogWriteTimer ;
//CTQueue<_ServerState> * g_pTempQueue = NULL ;

CMYTrace *		g_pTrace = NULL ;


CTQueue<_ServerState> * g_pServerStateQueue ;
CTQueue<_ItemLog> * g_pItemLogQueue ;
CTQueue<_CharacLog> * g_pCharacLogQueue ;
CTQueue<_LoginLog> * g_pLoginLogQueue ;


//==================================================================
// Global CRITICAL_SECTION
CRITICAL_SECTION		g_csDebufPrint ;		// 콘솔..


HANDLE				g_hCreateEvent		= NULL ;

//==================================================================
// Threads
DWORD WINAPI th_AcceptThread( LPVOID lp ) ;
DWORD WINAPI th_Recvthread( LPVOID lp ) ;
DWORD WINAPI th_WorkThread( LPVOID lp) ;

// ping thread
DWORD WINAPI th_UDPPingThread( LPVOID lp ) ;

//==================================================================
// Global Functions
int ProcessCommand( char * cmd ) ;
bool InitializeCS() ;
void DestroyGlobalObject()  ;
BOOL KeyboardHandler(DWORD dwEvent ) ;


//==================================================================
// console Control...
#define ASCII_CODE_ESC			27
#define ASCII_CODE_ENTER		13
#define ASCII_CODE_BACKSPACE	8
HANDLE hOut1, hOut2 ;

bool g_pThreadRunning = true ;
char g_ip[17] = {0};

int main( int argc, char * argv[] )
{	
	

	DWORD dwThreadID, dwTemp ;  ;
	HANDLE hThread=NULL ; 
	
	// Console
	HANDLE hIn ;
	char cmdbuf[128] ;
	INPUT_RECORD rec ;
	CONSOLE_SCREEN_BUFFER_INFO Info ;
	bool bInputMode = false ;		// 입력 모드냐 출력 모드냐.( 입력모드이면 화면1, 출력모드이면 화면2 )
	char c ;
	int writepos = 0;
	int ret = 0;
	u_short usAcceptPort ;
	
	// temp 
	int i = 0 ;  // loop 용 임시 변수

	g_pSystemLog = new _j_GlobalLog ;
	assert(g_pSystemLog) ;
	if( !g_pSystemLog->LogFileCreate(argv[1], "w+") ){
		printf( "LogFile Create Failed\n\rPlease make 'logfile' folder\n" ) ;
		goto label_end ;
	}
	printf("[INIT] Create / Initalize System error log file writer \n ") ;

	if( argc != 3 )
	{
		g_pSystemLog->LogFileWrite( "Log Server Need 2 Arguments.\n\rDB String Name & Local IP\n" ) ;
		goto label_end ;
	}
	
	
	sprintf( g_ip, argv[2] ) ;

	sprintf( cmdbuf, "EOD Log Server\n  - Version 1.0\n  - Compile %s\n", __DATE__ ) ;
	printf( "%s", cmdbuf ) ;

#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
	printf( "  - Account Length normal\n" );
#else
	printf( "  - Account Length Extended\n" );
#endif


	// Global CRITICAL_SECTIOM Initailze
	if ( InitializeCS() == false ) {
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Global CRITICAL_SECTIOM Initailze\n" ) ;
		return 0 ;
	}

	// 소켓 초기화
	WSADATA wsd ;
	if( WSAStartup( MAKEWORD( 2, 2 ), &wsd ) != 0 )	{
		g_pSystemLog->LogFileWrite( "[INIT FAIL] WSAStartup Failed : %d\n", WSAGetLastError() ) ;
		goto label_end ;		
	}

	// Seletec DB Target
	{//: A	Begin
		// db connection string
		WCHAR oleString[512] = {0} ;
		// db connection string
		CSqlConnectorString	sqlConStr( CRYPTO_KEY_FILE ) ;
		sqlConStr.ReadConStrFile( SQL_CONNECTION_STRING ) ;

		int iIndex = -1 ;
		int iIDC = -1 ;
		bool bIDC = false ;
		char * pString = NULL ;
		const char * pConString = NULL ;

		iIndex = DBF_GetGroupIndex( argv[1] ) ;
		iIDC = DBF_IsIDC( argv[1] ) ;

		if( iIndex < 0 || iIDC < 0 )
		{
			g_pSystemLog->LogFileWrite( "[INIT FAIL] Arg[1] is not proper\n" ) ;
			goto label_end ;
		}

		bIDC = ( iIDC == 0 ) ;
		pString = DBF_GetDBStringName(log_db_string, iIndex, bIDC) ;
		pConString = sqlConStr.GetConStr( pString ) ;
		if( NULL == pConString )
		{
			g_pSystemLog->LogFileWrite( "[INIT FAIL] %s DB Connect string name do not exist\n", pString ) ;
			goto label_end ;
		}
		
		MultiByteToWideChar(CP_ACP, 0, pConString, -1, oleString, strlen(pConString) ) ;
		usAcceptPort = ACCEPT_PORT_BASE + iIndex ;
		{
			printf( "DB - Use %s - %d\n", bIDC ? "IDC" : "EXTRA", iIndex ) ;
			g_pSystemLog->LogFileWrite( "DB - Use %s - %d\n", bIDC ? "IDC" : "EXTRA", iIndex ) ;
		}
		
		g_pDBCon = new CDB_Connector ;
		assert(g_pDBCon) ;
		if( g_pDBCon->initialize( oleString ) == false )
		{
			g_pSystemLog->LogFileWrite( "[ERROR] g_pDBCon->init failed \n") ;
			goto label_end ;
		}
		
		sqlConStr.Reset() ;

	}// : A end
	



	g_hCreateEvent = CreateEvent( NULL, FALSE, FALSE, NULL ) ;	// 비신호상태의 자동 이벤트 생성
	if( g_hCreateEvent == INVALID_HANDLE_VALUE ) {
		printf( "Error : CreateEvent Failed\n" ) ;
		goto label_end ;
	}

	// Packet Queue
	g_pPacketQueue = new _queue_Packet(_QUEUE_MEMORY_SIZE) ;
	printf("[INIT] g_pPacketQueue queue memory \n " ) ;
	assert(g_pPacketQueue) ;
	

	// DS Com Queue	
	g_pDSComQueue = new _DSCom_Queue ;
	assert( g_pDSComQueue ) ;
	if( g_pDSComQueue->Init() == false ) {
		g_pSystemLog->LogFileWrite( "[INIT FAIL] DSComQueue->Init() failed\n" ) ;
		goto label_end ;		
	}


	g_pServerStateQueue = new CTQueue<_ServerState>(_LOG_STATE_NODE_MAX_NUM_) ;
	assert(g_pServerStateQueue) ;
	
	
	g_pItemLogQueue = new CTQueue<_ItemLog>(_LOG_ITEM_NODE_MAX_NUM_) ;
	assert(g_pItemLogQueue) ;
	
	g_pCharacLogQueue = new CTQueue<_CharacLog>(_LOG_CHARAC_NODE_MAX_NUM_) ;
	assert(g_pCharacLogQueue) ;
	

	g_pLoginLogQueue = new CTQueue<_LoginLog>(_LOG_LOGIN_NODE_MAX_NUM_) ;
	assert( g_pLoginLogQueue ) ;
	


	// Log Write timer
	g_pLogWriteTimer = new _j_Templet_Timer(_BACKUP_QUEUE_MAX_SIZE) ;
	assert(g_pLogWriteTimer) ;
	if( g_pLogWriteTimer->createThread(_THREADNUMBER_BACKUP_) == false )
	{
		g_pSystemLog->LogFileWrite( "[ERROR] g_pLogWriteTimer->create() failed \n") ;
		goto label_end ;
	}
	

	// Trace
	g_pTrace = new CMYTrace ;
	assert( g_pTrace ) ;

	for( i = 0 ; i < _TRACE_CLASS_MAX ; i++ )
	{
		g_pTrace->SetHeadTitle( i , traceName[i] ) ;
	}

	// IOCP	CREATE
	printf( "[INIT] IO Completion Port Create\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] IO Completion Port Create\n" ) ;
	g_pIOCP = new _h_CompletionPort ;
	assert( g_pIOCP ) ;
	if( g_pIOCP->Create( _THREADNUMBER_PACKET_RECV_ ) == false ) {
		g_pSystemLog->LogFileWrite( "[INIT FAIL] IOCP->Create failed\n" ) ;
		
		goto label_end ;		
	}


	// WorkThreads	
	printf( "[INIT] Create Work Thread n = %d\n", _THREADNUMBER_WORK_ ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Work Thread n = %d\n", _THREADNUMBER_WORK_ ) ;
	for( i = 0 ; i < _THREADNUMBER_WORK_ ; i++ ) {
		hThread = BEGINTHREADEX( NULL, 0, th_WorkThread, NULL, 0, &dwThreadID ) ;
		
		if( hThread != INVALID_HANDLE_VALUE ) { 
			CloseHandle(hThread) ; 
			hThread = INVALID_HANDLE_VALUE ; 
		}
		else { 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] work thread failed\n" ) ; 
			goto label_end ; 
		}
	}


	
	// Recv Threads
	// 수신 스레드 -IOCP 큐에 박힌 결과패킷을 처리
	printf( "[INIT] Create Recv Thread n = %d\n", _THREADNUMBER_PACKET_RECV_ ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Recv Thread n = %d\n", _THREADNUMBER_PACKET_RECV_ ) ;
	for( i = 0 ; i < _THREADNUMBER_PACKET_RECV_ ; i++ ) {
		hThread = BEGINTHREADEX( NULL, 0, th_Recvthread, NULL, 0, &dwThreadID ) ;
		
		if( hThread != INVALID_HANDLE_VALUE ) { 
			CloseHandle(hThread) ; 
			hThread = INVALID_HANDLE_VALUE ; 
		}
		else { 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] recv thread failed\n" ) ; 
			goto label_end ; 
		}
	}

	
	// ping Threads	
	printf( "[INIT] Create ping Thread\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create ping Thread\n" ) ;
	hThread = BEGINTHREADEX( NULL, 0, th_UDPPingThread, (LPVOID)(usAcceptPort + 10000), 0, &dwThreadID ) ;	
	if( hThread != INVALID_HANDLE_VALUE ) { 
		CloseHandle(hThread) ; 
		hThread = INVALID_HANDLE_VALUE ; 
	}
	else { 
		g_pSystemLog->LogFileWrite( "[INIT FAIL] ping thread failed\n" ) ; 
		goto label_end ; 
	}
	

	// Accept Threads
	// GS 의 접속을 기다린다.	
	printf( "[INIT] Create User Accept Thread\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create User Accept Thread\n" ) ;
	hThread = BEGINTHREADEX( NULL, 0, th_AcceptThread, (LPVOID)(usAcceptPort), 0, &dwThreadID ) ;	
	if( hThread != INVALID_HANDLE_VALUE ) { 
		CloseHandle(hThread) ; 
		hThread = INVALID_HANDLE_VALUE ; 
	}
	else { 
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Accept thread failed\n" ) ; 
		goto label_end ; 
	}

	
	printf( "\n\nLog Server Start!!!\n" ) ;
	

	// Console
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
	
	while( 1 ) {
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
								if( MessageBox( NULL, "Close Server?", "Confirm", MB_OKCANCEL ) == IDOK ) {
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
			::PrintConsole( "[EXCEPTION] Consele \n" ) ;
			continue;
		}
		Sleep(5);
	} // while



label_end:	

	Sleep( 10000 ) ;
	// 종료 처리한다. 	
	DestroyGlobalObject() ;	// Global Object 해제	
	return 0;		

}




int ProcessCommand( char * cmd )
{
	static char command[128] = { 0, } ;
	static char arg1[128] = { 0, } ;
	static char arg2[128] = { 0, } ;
	
	int nString = 0 ;
	int nCount = 0;
	
	
	// Command string parsing 
	nString = sscanf( cmd, "%s %s %s", command, arg1, arg2 ) ;
	if( nString > 0 ) {		// Max 3
		
		// 종료
		if ( strcmp(command, "quit") == 0 ) {
			return 1 ;
		}
		else if ( strcmp(command, "list") == 0 ) {
			
		}
#ifdef _____TEST
		else if( strcmp(command, "test") == 0 )
		{
			MSG_CHARACTER msgCharac = {0} ;
			msgCharac.cMessage = MSG_NO_CHARACTER ;
			msgCharac.sLength = sizeof( MSG_CHARACTER  ) ;
			sprintf( msgCharac.log_Charac.account, "ozzywow") ;
			sprintf( msgCharac.log_Charac.charac, "백송") ;
			msgCharac.log_Charac.code = 1 ;
			sprintf ( msgCharac.log_Charac.ip, "218.147.34.133" ) ;
			msgCharac.log_Charac.level = 100 ;
			msgCharac.log_Charac.point = 1 ;
			msgCharac.log_Charac.zone  = 1 ;
			
			g_pPacketQueue->test( &msgCharac ) ;
		}
#endif

		else if( strcmp(command, "set") == 0 )
		{
			if( strcmp( arg1, "trace") == 0 )
			{
				if( strcmp( arg2, "on") == 0 )
				{
					g_pTrace->SetAllTrace( true ) ;
				}
				else if( strcmp( arg2, "off") == 0 )
				{
					g_pTrace->SetAllTrace( false ) ;
				}
			}
		}
		else if( strcmp( command, "show") == 0 )
		{
			if( strcmp( arg1, "trace" ) == 0 )
			{
				printf("Title | class | value \n") ;
				_TraceData * pTraceData = g_pTrace->ShowHeadTitleList() ;
				for ( int i = 0 ; i < _TRACE_CLASS_MAX ; i++ )
				{
					printf( "- %s : %d : %d \n", (&pTraceData[i])->cHeadTitle, i, (&pTraceData[i])->bActive) ;
				}
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


//== 전역 동기화 객체 초기화 ===============================================================================
bool InitializeCS()
{
	if( InitializeCriticalSectionAndSpinCount( &g_csDebufPrint, (0xf0000000)|2000 )  == FALSE ) {
		printf( " Error Critical Section - g_csDebufPrint\n" ) ;
		return false ;
	}	
	return true ;
}


//== 전역 객체 지우기 ===============================================================================
void DestroyGlobalObject() 
{
	WSACleanup() ;

	g_pThreadRunning = false;	// 쓰레드 자동종료..
	Sleep(3000) ;

	
	//// 전역 동기화 객체 해제		
	if( g_pPacketQueue )
		delete g_pPacketQueue ;	
	
//	g_pVMManager->Destroy() ;
//	delete g_pVMManager ;
	if( g_pSystemLog )
	{
		g_pSystemLog->LogFileClose() ;
		delete g_pSystemLog ;
	}

	if( g_pDSComQueue )
		delete g_pDSComQueue ;
		

	if( g_pIOCP )
		delete g_pIOCP ;				// IOCP	
		
	
	// 전역 CRITICAL_SECTION 해제
	DeleteCriticalSection(&g_csDebufPrint) ;
	CloseHandle( g_hCreateEvent) ;
	
}


