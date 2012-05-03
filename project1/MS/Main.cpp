
#include "stdafx.h"

#include "Network\Network.h"
#include "VMManager\VMManager.h"
#include "Map\MapManager.h"
#include "Queue\Packet_Queue.h"
#include "OLE-DB\db_command.h"
#include "OLE-DB\sql_convert.h"

#include "Global\Global_Lock.h"
#include "MyLib/MY_TRACE.H"




LONG	ND_Net::CSocketBuffer::m_glCreateCount = 0 ;
//#include <MY_SOCKET.H>
//CPool<CSocketSession> CSocketSession::m_pool(100) ;

//==================================================================
// Object
//_h_VMManager *		g_pVMManager = NULL ;
_j_GlobalLog *		g_pSystemLog = NULL ;
#ifdef _LOGIN_FILE_LOG_WRITE_
_j_GlobalLog *		g_pLoginLog = NULL ;
#endif
_queue_Packet *		g_pPacketQueue = NULL ;
_queue_Packet *		g_pChatPacketQueue = NULL ;
_queue_Packet *		g_pLoginPacketQueue = NULL ;
_j_queue_UDP_Packet * g_pUDPPacket = NULL ;
_j_MemeberQueue *	g_pMemberQueue = NULL ;
_j_BackupReady *	g_pBackupReady = NULL ;
_j_ComQueue *		g_pComQueue = NULL ;
_j_Templet_Timer *	g_pTimeOutTimer = NULL ;

CDB_Connector	*	g_pDBCon = NULL ;
myServer *			g_pMSServer = NULL ;

CMYTrace *			g_pTrace = NULL ;
const char * traceName[_TRACE_CLASS_MAX] = 
{ 
	"ERROR", 
		"BACKUP", 
		"UPDATE", 
		"LOGIN", 		
		"MESSAGE",
		"SYSTEM",
		"DB"	
} ;

//==================================================================
// Global CRITICAL_SECTION
CRITICAL_SECTION		g_csDebufPrint ;		// 콘솔..

HANDLE				g_hCreateEvent		= NULL ;



//==================================================================
// Threads

DWORD WINAPI th_WorkThread( LPVOID lp) ;
DWORD WINAPI th_ChatWorkThread( LPVOID lp) ;
DWORD WINAPI th_LoginWorkThread( LPVOID lp) ;
DWORD WINAPI th_FriendListBackupthread( LPVOID lp ) ;	// 캐릭터 백없 쓰레드


// ping thread
DWORD WINAPI th_UDPRecvThread( LPVOID lp ) ;
// udp  workthread
DWORD WINAPI th_UDPWorkThread( LPVOID lp ) ;

DWORD WINAPI th_PingThread( LPVOID lp ) ;

//==================================================================
// Global Functions
int ProcessCommand( char * cmd ) ;
bool InitializeCS() ;
void DestroyGlobalObject()  ;
BOOL KeyboardHandler(DWORD dwEvent ) ;
#ifdef _BACKUP_DB_CON
bool FriendListRestore(char * fileName ) ;
#endif


//==================================================================
// console Control...
#define ASCII_CODE_ESC			27
#define ASCII_CODE_ENTER		13
#define ASCII_CODE_BACKSPACE	8
HANDLE hOut1, hOut2 ;

bool g_pThreadRunning = true ;
bool g_pMainThreadRunning = true ;

int g_iCurLoginMsgNum ;
int g_iCurLoginMsgPos ;


list<CTraceAbleLock*> CTraceAbleLock::g_LockList(0) ;

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
	int iIndex = -1 ;
	
	// temp 
	int i = 0 ;  // loop 용 임시 변수

	// Class 초기화
	// system log
	{	
		char logfileName[32] = {0};
		sprintf( logfileName, "SystemLog_%s", argv[2] ) ;
		g_pSystemLog = new _j_GlobalLog ;
		assert(g_pSystemLog) ;
		if( !g_pSystemLog->LogFileCreate( logfileName , "w+") )
		{
			printf( "System LogFile Create Failed", "Error", MB_OK );
			printf( "press enter\n" );
			getchar();
			return 0;
		}
		printf("[INIT] Create / Initalize System error log file writer \n ") ;
	}

	if( argc < 3 )
	{
		g_pSystemLog->LogFileWrite( "[INIT FAIL] MS must has two arguments, Group No And DB Name\n\rMS Terminated\n" ) ;
		Sleep(100);
		return 0 ;
	}

	printf( "## EOD Messenger Server - Version 0.01 ##\n\n" ) ;

	// Global CRITICAL_SECTIOM Initailze
	if ( InitializeCS() == false ) 
	{
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Global CRITICAL_SECTIOM Initailze\n" ) ;
		goto label_end ;
		
	}

	// 소켓 초기화
	WSADATA wsd ;
	if( WSAStartup( MAKEWORD( 2, 2 ), &wsd ) != 0 )	
	{
		g_pSystemLog->LogFileWrite( "[INIT FAIL] WSAStartup Failed : %d\n", WSAGetLastError() ) ;
		goto label_end ;		
	}


	{//: A	Begin
	// db connection string
	WCHAR oleString[512] = {0} ;
	// db connection string
	CSqlConnectorString	sqlConStr( CRYPTO_KEY_FILE ) ;
	sqlConStr.ReadConStrFile( SQL_CONNECTION_STRING ) ;

	int iIDC = -1 ;
	bool bIDC = false ;
	char * pString = NULL ;
	const char * pGameConString = NULL ;

	iIndex = DBF_GetGroupIndex( argv[2] ) ;
	iIDC = DBF_IsIDC( argv[2] ) ;

	if( iIndex < 0 || iIDC < 0 )
	{
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Arg[2] is not proper\n" ) ;
		goto label_end ;
	}

	bIDC = ( iIDC == 0 ) ;
	
	// Game DB
	pString = DBF_GetDBStringName(game_db_string, iIndex, bIDC) ;
	pGameConString = sqlConStr.GetConStr( pString ) ;

	if( NULL == pGameConString )
	{
		g_pSystemLog->LogFileWrite( "[INIT FAIL] %s Game DB Connect string name do not exist" ) ;
		goto label_end ;
	}

	MultiByteToWideChar(CP_ACP, 0, pGameConString, -1, oleString, strlen(pGameConString) ) ;
	{
		printf( "Use %s - %d", bIDC ? "IDC" : "EXTRA", iIndex ) ;
		g_pSystemLog->LogFileWrite( "Use %s - %d\n", bIDC ? "IDC" : "EXTRA", iIndex ) ;
		Sleep(500);
	}

	

	// DB Connection / Initialize
	// ole-db connecter	
	printf( "[INIT] OLE-DB Conneter Initialize \n" ) ;	
	g_pDBCon = new CDB_Connector ;
	
	assert(g_pDBCon) ;
	if( !g_pDBCon->initialize( oleString ) )
	{
		g_pSystemLog->LogFileWrite( "[INIT FAIL] g_pDBCon->init( )  Failed\n" ) ;
		goto label_end ;
	}

	sqlConStr.Reset() ;

	}
	

	




	g_hCreateEvent = CreateEvent( NULL, FALSE, FALSE, NULL ) ;	// 비신호상태의 자동 이벤트 생성
	if( g_hCreateEvent == INVALID_HANDLE_VALUE ) 
	{
		printf( "Error : CreateEvent Failed\n" ) ;
		goto label_end ;
	}


	g_pTrace = new CMYTrace ;
	assert( g_pTrace ); 

	for( i = 0 ; i < _TRACE_CLASS_MAX ; i++ )
	{
		g_pTrace->SetHeadTitle( i , traceName[i] ) ;
	}




	

#ifdef _LOGIN_FILE_LOG_WRITE_
	{	
		char logfileName[32] = {0};
		sprintf( logfileName, "Login_%s", argv[2] ) ;
		g_pLoginLog = new _j_GlobalLog ;
		assert(g_pLoginLog) ;
		if( !g_pLoginLog->LogFileCreate( logfileName , "w+") )
		{
			g_pSystemLog->LogFileWrite( "[WARNING] Login LogFile Create Failed\n" ) ;
		}
		printf("[INIT] Create / Initalize System error log file writer \n ") ;
	}
#endif

	// Packet Queue
	g_pPacketQueue = new _queue_Packet(_QUEUE_MEMORY_SIZE) ;
	printf("[INIT] g_pPacketQueue queue memory \n " ) ;
	assert(g_pPacketQueue) ;
	
	
	// Chat Packet Queue
	g_pChatPacketQueue = new _queue_Packet(_CHAT_QUEUE_MEMORY_SIZE) ;
	printf("[INIT] g_pChatPacketQueue queue memory \n " ) ;
	assert(g_pChatPacketQueue) ;
	

	// Login Packet Queue
	g_pLoginPacketQueue = new _queue_Packet(_CHAT_QUEUE_MEMORY_SIZE) ;
	printf("[INIT] g_pLoginPacketQueue queue memory \n " ) ;
	assert(g_pLoginPacketQueue) ;
		
		

	// UPD Packet Queue
	g_pUDPPacket = new _j_queue_UDP_Packet(_UDP_QUEUE_SIZE) ;
	printf("[INIT] g_pUDPPacket queue memory \n " ) ;
	assert(g_pLoginPacketQueue) ;
	

	// _h_Ds_com 
	printf( "[INIT] _j_ComQueue Create\n" ) ;
	g_pComQueue = new _j_ComQueue(_SOCKET_POOL_COUNT_) ;
	assert( g_pComQueue ) ;
	
	// Member Queue
	g_pMemberQueue = new _j_MemeberQueue(_PERMISSIBLE_MAX_USER) ;
	printf("[INIT] g_pMemberQueue queue memory \n " ) ;
	assert(g_pMemberQueue) ;	
	

	// Backup 
	printf( "[INIT] _j_BackupReady Create\n" ) ;
	g_pBackupReady = new _j_BackupReady ;
	assert(g_pBackupReady) ;
	


	// _j_Templet_Timer
	g_pTimeOutTimer = new _j_Templet_Timer ;
	if( !g_pTimeOutTimer->create() ) 
	{
		printf("Error : Initailize _j_Templet_Timer failed \n") ;
		goto label_end;
	}



// Create Threads

	// BackupThreads
	printf( "[INIT] Create Backup Thread n = %d\n", _THREADNUMBER_CHATWORK_ ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Backup Thread n = %d\n", _THREADNUMBER_CHATWORK_ ) ;
	for( i = 0 ; i < 1 ; i++ ) 
	{
		hThread = BEGINTHREADEX( NULL, 0, th_FriendListBackupthread, NULL, 0, &dwThreadID ) ;
		
		if( hThread != INVALID_HANDLE_VALUE ) 
		{ 
			CloseHandle(hThread) ; 
			hThread = INVALID_HANDLE_VALUE ; 
		}
		else 
		{ 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] Backup Thread failed\n" ) ; 
			goto label_end ; 
		}
	}


	// WorkThreads	
	printf( "[INIT] Create Work Thread n = %d\n", _THREADNUMBER_TCP_WORK_ ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Work Thread n = %d\n", _THREADNUMBER_TCP_WORK_ ) ;
	for( i = 0 ; i < _THREADNUMBER_TCP_WORK_ ; i++ ) 
	{
		hThread = BEGINTHREADEX( NULL, 0, th_WorkThread, NULL, 0, &dwThreadID ) ;
		
		if( hThread != INVALID_HANDLE_VALUE ) 
		{ 
			CloseHandle(hThread) ; 
			hThread = INVALID_HANDLE_VALUE ; 
		}
		else 
		{ 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] work thread failed\n" ) ; 
			
			goto label_end ; 
		}
	}


	// ChatWorkThreads
	// 수신 스레드 -IOCP 큐에 박힌 결과패킷을 처리
	printf( "[INIT] Create Chatting Thread n = %d\n", _THREADNUMBER_CHATWORK_ ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Chatting Thread n = %d\n", _THREADNUMBER_CHATWORK_ ) ;
	for( i = 0 ; i < _THREADNUMBER_CHATWORK_ ; i++ ) 
	{
		hThread = BEGINTHREADEX( NULL, 0, th_ChatWorkThread, NULL, 0, &dwThreadID ) ;
		
		if( hThread != INVALID_HANDLE_VALUE ) 
		{ 
			CloseHandle(hThread) ; 
			hThread = INVALID_HANDLE_VALUE ; 
		}
		else 
		{ 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] ChatWork thread failed\n" ) ; 
			goto label_end ; 
		}
	}

	// LoginWorkThreads
	// 수신 스레드 -IOCP 큐에 박힌 결과패킷을 처리
	printf( "[INIT] Create Login Thread n = %d\n", _THREADNUMBER_LOGINWORK_ ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Login Thread n = %d\n", _THREADNUMBER_LOGINWORK_ ) ;
	for( i = 0 ; i < _THREADNUMBER_LOGINWORK_ ; i++ ) 
	{
		hThread = BEGINTHREADEX( NULL, 0, th_LoginWorkThread, NULL, 0, &dwThreadID ) ;
		
		if( hThread != INVALID_HANDLE_VALUE ) 
		{ 
			CloseHandle(hThread) ; 
			hThread = INVALID_HANDLE_VALUE ; 
		}
		else 
		{ 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] Login thread failed\n" ) ; 
			goto label_end ; 
		}
	}

	
	// recv Threads	
	printf( "[INIT] Create Recv Thread\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Recv Thread\n" ) ;
	hThread = BEGINTHREADEX( NULL, 0, th_UDPRecvThread, (LPVOID)(_PING_PORT_), 0, &dwThreadID ) ;	
	if( hThread != INVALID_HANDLE_VALUE ) 
	{ 
		CloseHandle(hThread) ; 
		hThread = INVALID_HANDLE_VALUE ; 
	}
	else 
	{ 
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Recv thread failed\n" ) ; 
		goto label_end ; 
	}

	// udp workthread	
	printf( "[INIT] Create udpwork Thread n = %d\n", 4 ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Recv Thread n = %d\n", 4 ) ;
	for( i = 0 ; i < _THREADNUMBER_UDP_WORK_ ; i++ ) 
	{
		hThread = BEGINTHREADEX( NULL, 0, th_UDPWorkThread, NULL, 0, &dwThreadID ) ;
		
		if( hThread != INVALID_HANDLE_VALUE ) 
		{ 
			CloseHandle(hThread) ; 
			hThread = INVALID_HANDLE_VALUE ; 
		}
		else
		{ 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] udpwork thread failed\n" ) ; 
			goto label_end ; 
		}
	}

	// recv Threads	
	printf( "[INIT] Create ping Thread\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create ping Thread\n" ) ;
	hThread = BEGINTHREADEX( NULL, 0, th_PingThread, (LPVOID)(21097 + iIndex * 100), 0, &dwThreadID ) ;	
	if( hThread != INVALID_HANDLE_VALUE ) 
	{ 
		CloseHandle(hThread) ; 
		hThread = INVALID_HANDLE_VALUE ; 
	}
	else 
	{ 
		g_pSystemLog->LogFileWrite( "[INIT FAIL] ping thread failed\n" ) ; 
		goto label_end ; 
	}


	
	// MS create server	
	g_pMSServer = new myServer ;
	if( g_pMSServer->CreateServerSocket( 4, GetMyIP(1), 7411, _SEND_SLOT_COUNT_, 10, _SOCKET_POOL_COUNT_, 50, 50, 50 ) == FALSE )
	{		
		g_pSystemLog->LogFileWrite( "[INIT FAIL] g_pServer->CreateServerSocket() return false\n" ) ;
		return 0 ;
	}
	printf("Accept Bind (ip:%s, port:%d) \n", GetMyIP(1), 7411) ;
	
	printf( "\n\nMessenger Server Start!!!\n" ) ;
	
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
	
	while( g_pMainThreadRunning ) {
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
							g_pSystemLog->xFileFlush();
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
	} // while



label_end:	

	// 종료 처리한다. 	
	DestroyGlobalObject() ;	// Global Object 해제	
	return 0;		

}




int ProcessCommand( char * cmd )
{
	static char command[128] = { 0, } ;
	static char arg1[128] = { 0, } ;
	static char arg2[128] = { 0, } ;
	static char arg3[128] = {0} ;
	
	int nString = 0 ;
	int nCount = 0;
	
	
	// Command string parsing 
	nString = sscanf( cmd, "%s %s %s %s", command, arg1, arg2, arg3 ) ;
	if( nString > 0 ) {		// Max 3
		
		// 종료
		if ( strcmp(command, "quit") == 0 ) {
			SYSTEMTIME	time ;
			GetLocalTime(&time ) ;
			::PrintConsole( "[%d:%d:%d][QUIT] QUIT \n", time.wHour, time.wMinute, time.wSecond ) ;
			return 1 ;
		}
		else if ( strcmp(command, "show") == 0 ) {
			if( strcmp( arg1, "lock" ) == 0 )
			{

				list<CTraceAbleLock*> * pLockList = CTraceAbleLock::GetLockListPtr() ;
				printf("size: %d \n",pLockList->size() ) ;
				CTraceAbleLock * pCurLock = NULL ;				

				int i = 0 ;
				for( list<CTraceAbleLock*>::iterator itr = pLockList->begin() ; itr != pLockList->end(); ++itr)
				{
					pCurLock = *itr ;
					if( pCurLock == NULL )
					{
						printf("[error] pCurLock \n" ) ;
						break ;
					}
					else
					{
						//pCurLock->tlock(__FILE__, __LINE__ ) ;
						//pCurLock->tunlock(__FILE__, __LINE__ ) ;
						if( pCurLock->GetCurCount() != 0 )
						{
							printf( " cur-> %s \n", pCurLock->GetCurLockingPos() ) ;
							printf( " last-> %s \n", pCurLock->GetLastLockingPos() ) ;
						}						
					}
					++i ;
				}
				printf( "complete: %d\n", i ) ;

			}			
			else if( strcmp( arg1, "list") == 0 )
			{
				int iLoginer = 0 , iTotal = 0 ;
				_FriendInfo * pFriendInfo = g_pMemberQueue->GetBegin() ;				
				printf("CHARACTER    STATE\n") ;
				while ( pFriendInfo ) 
				{
					if( pFriendInfo->IsLogon() )
					{
						++iLoginer ;
					}
					printf( "%-12s %8d \n", pFriendInfo->GetName(), pFriendInfo->GetState() ) ;
					pFriendInfo = g_pMemberQueue->MoveNext( pFriendInfo ) ;
					++iTotal ;
				}
				printf("report : Loginer(%d) / Total(%d) \n", iLoginer, iTotal ) ;
			}
			else if( strcmp( arg1, "mslist") == 0 )
			{
				_FriendInfo * pFriendInfo ;
				list<_Member>::iterator itr ;

				
				pFriendInfo = g_pMemberQueue->FindNode( arg2 ) ;
				

				if( pFriendInfo )
				{
					list<_Member> * pFriendList = pFriendInfo->GetFriendListPtr() ;
										
					printf( "** Friend list (%s)", arg2 ) ;
					
					for( itr = pFriendList->begin() ; 
					itr != pFriendList->end() ;
					itr++ )
					{
						printf( "  -- %s \n", itr->characName) ;
					}
					
					printf( "** end (%s) \n", arg2 ) ;
					
					
				}
				else
				{ 
					printf( " Not fount (%s) ! \n", arg2 ) ;
				}			
				
			}
			else if( strcmp( arg1, "maxcon") == 0 )
			{
				printf("Connection : %u \n", g_pMemberQueue->GetCurMemberCount() ) ;
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
		}
		else if( strcmp( command, "set") == 0 )
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
					g_pTrace->ShowTraceString( atoi(arg2), ( (atoi(arg3)) > (0) ? true : false ) ) ;
				}			
			}
		}

#ifdef _BACKUP_DB_CON
		else if( strcmp( command, "restore") == 0 )
		{			
			FriendListRestore( arg1 ) ;
		}
#endif

		else if ( command[0] == '/' )
		{
			
			printf("[INFO] System duration queue report \n") ;
			printf("   ->TCP Work packet queue / Duration : %d, WorkThread : %d \n", g_pPacketQueue->GetCurCount(), _THREADNUMBER_TCP_WORK_ ) ;
			printf("   ->Chat work packet queue / Duration : %d, WorkThread : %d \n", g_pChatPacketQueue->GetCurCount(), _THREADNUMBER_CHATWORK_ ) ;
			printf("   ->Login packet queue / Duration : %d, WorkThread : %d \n", g_pLoginPacketQueue->GetCurCount(), _THREADNUMBER_LOGINWORK_ ) ;
			printf("   ->UDP Work packet queue / Duration : %d, WorkThread : %d \n", g_pUDPPacket->GetCurCount(), _THREADNUMBER_UDP_WORK_ ) ;
			printf("   ->Socket pool queue size : %d / %d \n", _SOCKET_POOL_COUNT_ - g_pMSServer->GetQueueCount(), _SOCKET_POOL_COUNT_ )  ;
			printf("   ->create socket count : %d \n", ND_Net::CSocketBuffer::GetUsingCount() )  ;
			printf("   ->On client count : %d / %d \n", g_pMemberQueue->GetCurMemberCount(), _PERMISSIBLE_MAX_USER )  ;
			printf("   ->Ready baseUser count : %d / %d \n", _SOCKET_POOL_COUNT_ - g_pComQueue->GetCount(), _SOCKET_POOL_COUNT_ )  ;
			printf("   -> CurLoginMsgNum : %d, CurLoginMsgPos:%d \n", g_iCurLoginMsgNum, g_iCurLoginMsgPos ) ;
			printf(" itemp : %d \n",g_pMSServer->m_iTempBuf ) ;
		}
		
	}

	
	
	return 0 ;
}



BOOL KeyboardHandler(DWORD dwEvent )
{
	switch( dwEvent ) 
	{
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
	if( InitializeCriticalSectionAndSpinCount( &g_csDebufPrint, (0x80000000)|2000 )  == FALSE ) 
	{
		printf( " Error Critical Section - g_csDebufPrint\n" ) ;
		return false ;
	}	



	return true ;
}


//== 전역 객체 지우기 ===============================================================================
void DestroyGlobalObject() 
{

	WSACleanup() ;

	Sleep( 10000 ) ;	// 백업쓰레드가 백업을 하도록 여유를 준다.

	g_pThreadRunning = false;	// 쓰레드 자동종료..
	Sleep(3000) ;

	//// 전역 동기화 객체 해제	

	SAFE_DELETE( g_pPacketQueue ) ;
	SAFE_DELETE( g_pChatPacketQueue ) ;
	SAFE_DELETE( g_pLoginPacketQueue ) ;
	SAFE_DELETE( g_pUDPPacket ) ;

	if( g_pSystemLog )
	{
		g_pSystemLog->LogFileClose() ;
		delete g_pSystemLog ;
	}
#ifdef _LOGIN_FILE_LOG_WRITE_

	if( g_pLoginLog )
	{
		g_pLoginLog->LogFileClose() ;
		delete g_pLoginLog ;
	}
#endif

	SAFE_DELETE( g_pDBCon )
#ifdef _BACKUP_DB_CON
	SAFE_DELETE( g_pBackupDBCon ) ;
#endif
	
	
	SAFE_DELETE( g_pComQueue ) ;	
	SAFE_DELETE( g_pMemberQueue ) ;
	SAFE_DELETE( g_pBackupReady ) ;
	SAFE_DELETE( g_pTimeOutTimer ) ;

	
	SAFE_DELETE( g_pMSServer ) ;

	SAFE_DELETE( g_pTrace ) ;

	//g_pVMManager->Destroy() ;
	//delete g_pVMManager ;	
	
	//TLock::deleteMemPool() ;
	
	
	// 전역 CRITICAL_SECTION 해제
	DeleteCriticalSection(&g_csDebufPrint) ;

	if( g_hCreateEvent )
		CloseHandle( g_hCreateEvent ) ;
}




#ifdef _BACKUP_DB_CON
bool FriendListRestore(char * fileName )
{
	int iUID[10000] ;
	// file Read from charac list of restore
	FILE * fp = NULL ;
	int nRead = 0 ;
	char buf[128] = {0} ;


	COLE_DB_Executer RestoreDB(g_pBackupDBCon) ;
	COLE_DB_Executer AfterDB(g_pDBCon ) ;
		
		
	try{
		
		if( ( fp = fopen( fileName, "rt" ) ) == NULL )
		{
			printf( "read file failed (%s) \n", fileName ) ; 
			return false ;
		}
		
		int i = 0 ;
		int total ;
		while (1) {
						
			if( fgets( buf, sizeof(buf), fp ) == NULL )
			{				
				break ;
			}
			if( i > 10000 )
				return false ;
			
			iUID[i++] = atoi(buf) ;
		}
		fclose( fp ) ;

		total = i ;
		printf( " ** restore charac : %d \n", total ) ;
		::PrintConsole( " ** restore charac : %d \n", total ) ;


		// looping AND restore

		_FriendInfo before_f ;
		_FriendInfo after_f ;
		list<_Member> * pBeforeFriendList ;
		list<_Member> * pAfterFriendList ;
		list<_Member>::iterator BeforeItr ;
		list<_Member>::iterator AfterItr ;

		_Member member = {0} ;

		bool bIsHad ;
		bool bIsNeedSave ;
		int iSuccessed = 0 ;
		int iFailed = 0 ;

		for( i = 0 ; i < total ; ++i )
		{
			Sleep(10) ;
			before_f.releas() ;
			after_f.releas() ;
			if( RestoreDB.GetMyDataByUID( &before_f, iUID[i] ) == false )
			{
				::PrintConsole(" ** read B-data failed : %d \n", iUID[i] ) ;
				printf(" ** read B-data failed : %d \n", iUID[i] ) ;
				++iFailed ;
				continue ;
			}
			if( before_f.GetFriendCurCount() == 0 )
			{
				continue ;
			}
			if( AfterDB.GetMyDataByUID( &after_f, iUID[i] ) == false )
			{
				::PrintConsole(" ** read A-data failed : %d \n", iUID[i] ) ;
				printf(" ** read A-data failed : %d \n", iUID[i] ) ;
				++iFailed ;
				continue ;
			}
			
			pBeforeFriendList = before_f.GetFriendListPtr() ;
			pAfterFriendList = after_f.GetFriendListPtr() ;
			// Matching

			bIsNeedSave = false ;
			for( BeforeItr = pBeforeFriendList->begin() ; BeforeItr!= pBeforeFriendList->end() ; ++BeforeItr )
			{
				bIsHad = false ;				
				for( AfterItr = pAfterFriendList->begin() ; AfterItr!= pAfterFriendList->end() ; ++AfterItr )
				{
					if( BeforeItr->characName[0] == AfterItr->characName[0] 
						&& strcmp( BeforeItr->characName, AfterItr->characName) == 0 )
					{
						bIsHad = true ;	
						break ;
					}
				}

				// 없다.. 삽입해라
				if( bIsHad == false )
				{
					memcpy( member.characName, BeforeItr->characName, 16 ) ;
					pAfterFriendList->push_back( member ) ;
					bIsNeedSave = true ;
				}
			}

			if( bIsNeedSave == true )
			{
				if( AfterDB.UpdateFriendList( iUID[i], pAfterFriendList ) == false )
				{
					::PrintConsole(" ** db backup failed : %d \n", iUID[i] ) ;
					printf(" ** db backup failed : %d \n", iUID[i] ) ;
					++iFailed ;
				}
				++iSuccessed ;
			}			
		}


		::PrintConsole( "[end] restore successed: %d\n", iSuccessed ) ;
		printf( "[end] restore successed : %d\n", iSuccessed ) ;
		::PrintConsole( "[end] restore failed: %d\n", iFailed ) ;
		printf( "[end] restore failed : %d\n", iFailed ) ;

	}
	catch(...){
		fclose(fp) ;
		return false ;
	}

	
	return true ;
	

}
#endif