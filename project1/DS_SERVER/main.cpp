#define _EXCLUDE_GLOBAL_FILE_OBJ
#define _EXCLUDE_GLOBAL_TIME_OBJ
#define _EXCLUDE_GLOBAL_USER_LOCK_OBJ

#include "stdafx.h"
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
//#include "Global\Global_define.h"
#include "Global\Global_Lock.h"
#include "MAPManager\MAPManager.h"
#include "Network\Network.h"
#include "Network\LogSvrLink.h"
#include "Billing\BillingSvrLink.h"
//#include "Network\DSComm.h"
#include "packet_queue\packet_queue.h"
#include "ole_db_module\ole_db_module.h"
#include "ole_db_module\db_command.h"
#include "Global\Global_LogFile.h"
#include "Global\Global_ServerInfo.h"
#include "Global\Global_UniqueID.h"
//#include "ole_db_module\db_Con.h"
#include "Global\Trace.h"
#include "Ranking_Manager\Ranking_Manager.h"
#include "NDExceptionReport\NDExceptionReport.h"
#include "CS\party_manager.h"
#include "Global\tcp_packets_party.h"
#include "CS\Organization.h"
#include "CS\party_manager.h"
#include "ole_db_module\sql_convert.h"

#include "CS/Castle.h"
#ifdef _PD_CASTLE_STEP_2_
#include "Cs/TaxSystem.h"
#include "Cs/CombatRecord.h"
#endif// _PD_CASTLE_STEP_2_

#ifdef _PD_GAME_EVENT_
#include "GEvent/GameEvent.h"
#endif

#ifdef _PD_MASTER_PUPIL_
#include "CS/MasterPupil.h"
#endif



//=================================================================================
//== THREAD Funcions===============================================================
//=================================================================================
// Accept Thread
DWORD WINAPI th_AcceptThread( LPVOID lp ) ;
// Recv Thread
DWORD WINAPI th_Recvthread( LPVOID lp ) ;
// Packet Thread
DWORD WINAPI th_PacketProcess( LPVOID lp ) ;
DWORD WINAPI th_UpdateProcess( LPVOID lp ) ;
// BACKUP(ACCOUNT,CHARACTER) Thread
DWORD WINAPI th_CharacBackupthread( LPVOID lp ) ;
DWORD WINAPI th_AccountBackupThread( LPVOID lp ) ;
// UDP Socket recv, send Thread
//DWORD WINAPI th_UDPRecvThread( LPVOID lp ) ;
// UDP socket Threads
DWORD WINAPI th_UDPRecvThread( LPVOID lp ) ;
DWORD WINAPI th_UDPSendThread( LPVOID lp ) ;
DWORD WINAPI th_UDPGMSPacketWorkThread( LPVOID lp ) ;
DWORD WINAPI th_UDPLoginPacketWorkThread( LPVOID lp ) ;
// CS work thread
unsigned long __stdcall	th_CSWorkThread( LPVOID lp) ;
unsigned long __stdcall	th_ORWorkThread( LPVOID lp) ;
unsigned long __stdcall th_CSChatWorkThread( LPVOID lp) ;

DWORD WINAPI th_CommandBackupThread( LPVOID lp ) ;
DWORD WINAPI th_ConnectCountRecorderThread( LPVOID lp ) ;

// ranking version read thread 
unsigned long __stdcall	th_RankingVersionChecker( LPVOID lp) ;



// Ping thread
DWORD WINAPI th_UDPPingThread( LPVOID lp ) ;

int	g_arRankTableVersion[_MAX_RANK] = {0};

u_int					g_uiCompanyCode = 0 ;


//=================================================================================
//== Global funcions===============================================================
//=================================================================================
// Release global objects
void DestroyGlobalObject() ;
// Initialize global objects
// Backup All Character data to DB
int CharacDataBackup( int iOption = 0 );	// iOption = 0 : sync backup, 1 : unsync backup
// Backup all Character data to FILE
int CharacFileBackup();
// Backup memory to FILE
int MemoryBackup();
//bool sendGS(char * pMsgBuf, int size);
SOCKET RestoreSocket( const unsigned short usPortNum ) ;

void	ShowAccountList();
void	ShowAccountInfo(char * cAccount);
void	ShowCharacList();
void	ShowCharacInfo(char * cCharac);
void    ShowSkill(char * cCharac) ;
void	ShowServerList();

void reserveAccountLogout( int serverNum ) ;
void AccountLogout(int serverNum, COLE_DB_Executer * pDBCommand, C_USER_DB_Command * pUserCommand ) ;
int	CharacRestore(char * cFileName);
void GSDisConnectInfoSend( char cServerNum ) ;

bool ReadInitValueFile( const char * const fileName, int & life, int & force, int & con ) ;
bool ReadInitEventFile( const char * const fileName, _TempEvent* event ) ;

_TempEvent event[64] = {0 } ;


// Create UDP socket
SOCKET UDPSocket( unsigned short port ) ;


_j_GlobalLog *			g_pSystemLog		= NULL ;	// Log message saver
// 동접 로그
_j_GlobalLog *			g_pGMStLog		= NULL ;
// 익셉션 리포트.
NDExceptionReport*		g_pNDExceptionReport  = NULL;
_h_CompletionPort *		g_pIOCP				= NULL ;	// IOCP
_queue_Packet *			g_pPacketQ			= NULL ;	// Packet queue
_queue_Packet *			g_pUpdatePacketQ	= NULL ;	// OR Packet queue
_queue_Packet *			g_pCSPacketQ		= NULL ;	// PARTY Packet queue
_queue_Packet *			g_pORPacketQ		= NULL ;	// OR Packet queue
_queue_Packet *			g_pChatPacketQueue	= NULL ;	// Chat Packet queue
_j_queue_UDP_Packet *	g_pUDPSendPacketQ	= NULL ;
_j_queue_UDP_Packet *	g_pUDPRecvPacketQ   = NULL ;
_j_queue_UDP_Packet *	g_pUDPLoinPacketQ   = NULL ;
_j_AccMAPManager *		g_pAccMapManager    = NULL ;	// AccMap Manager
// Backup 대기 객체
_j_BackupReady *		g_pBackupReady		= NULL ;
HANDLE						g_hInitEvent		= NULL ;	// Thread event handler
//_h_Global_Time			g_GTime ;		
// Global OLE_DB object
CDB_Connector	*			g_pDBCon			= NULL ;
CDB_Connector *			g_pUSERDBCon		= NULL ;
CDB_Connector *			g_pEventDBCon		= NULL ;
COLE_DB_Executer *		g_pDB_Command		= NULL;		//oledb
C_USER_DB_Command *		g_pUser_Command		= NULL ;	// user oledb

/*
#ifdef _CIS_20070326_
CDB_Connector*			g_pDBCon_CIS = NULL;
CDBCmd_CIS*				g_pDBCmd_CIS = NULL;
#endif
*/

// 2006.06.02 Molla, Nexon Web Shop
#ifdef ITEMSHOP_PATCH_200607
CDB_Connector*					g_pDBCon_WebShop	= NULL;
CDBCommand_WebShop*		g_pDBCmd_WebShop	= NULL;
#endif

_j_ServerInfo			g_ServerInfo;				// GS Server infomation object
_j_ServerInfo *			g_pServerInfo = &g_ServerInfo ;
//_j_ShowText				g_ShowText;
// Uinqueue Generater
_j_UniqueIDQueue *		g_pUniqueIDQueue	= NULL ;
// DS Com Queue
_DSCom_Queue *			g_pDSComQueue		= NULL ;
CUserMemSaveTimer *		g_pMemSaveTimer		= NULL ;
//CTQueue<_CharacLog> * g_pCharacLogQueue = NULL ;
CLogSvr_Com *			g_pLogSvrCom		= NULL ;

// System Trace
CMYTrace *				g_pTrace			= NULL ;
// party
_j_PartyQueue *			g_pPartyQueue		= NULL ;
_j_PartyManager *		g_pPartyManager		= NULL ;
_h_Party_Timer *		g_pPartyTimer		= NULL ;
_j_MemeberQueue *		g_pMemberQueue		= NULL ;

// Ranking Manager 
CRankingManager	*		g_pRankingManager = NULL ;

COrganizationManager *	g_pOrganizationManager = NULL ;

#ifdef _PD_ALLY_ORG_STEP_1_
CAllyManager *		g_pAllyManager = NULL;
#endif


u_int					g_serverStateFlag ;

CND_PacketLengthChecker * g_pPacketChecker = NULL ;

#ifdef _PD_GAME_EVENT_
CGameEventManager *		g_pGameEvent = NULL;
#endif

CConnectionManager *	g_pGMSAcceptor = NULL;

#ifdef _PD_MASTER_PUPIL_
CDSMasterSet *			g_pMPSet = NULL;
#endif


const char * traceName[_TRACE_CLASS_MAX] = 
{ 
		"ERROR", 
		"BACKUP", 
		"UPDATE", 
		"LOGIN", 
		"CHARAC", 
		"GMS", 
		"SYSTEM",
		"DB",
		"MESSAGE",
		"BILLING",
} ;

//////////////////////////////////////////////////////////////////////////
// 2006.05.30 Molla, Nexon Web Shop
#ifdef ITEMSHOP_PATCH_200607
#include "./Packet_Queue/WebShopPktQueue.h"

HANDLE hWebShopPktProcInfo;
std::list<WEB_SHOP_PKT_PROC_INFO*> listWebShopPktProcInfo;
CRITICAL_SECTION csWebShopPktProcInfo;
unsigned long __stdcall	WebShopPktProc(LPVOID lp);

#define NEXON_ITEM_SHOP_IP_CNT	2
char* pszNexonItemShopIP[] = {
	"211.218.232.32",	// 넥슨 테스트 서버
	"210.101.85.170",	// 넥슨 정식 서버
	"210.101.85.171",	// 넥슨 정식 서버
	"210.101.85.172",	// 넥슨 정식 서버
	"210.101.85.173",	// 넥슨 정식 서버
	"221.147.34.150"	// 윤군 개발 피씨(테스트 용)
};
u_long ulIP_NexonItemShop[NEXON_ITEM_SHOP_IP_CNT] = {0};
#endif

/*
Order Notification을 하는 서버
Test : 211.218.232.32
Real : 210.101.85.170~173

Gateway DB
Test : 210.101.85.177, 2145
Real : 210.101.85.60, 2145
*/
//////////////////////////////////////////////////////////////////////////

//exception report
//NDExceptionReport*  g_pNDExceptionReport  = NULL;


//=================================================================================
// console Control...
#define ASCII_CODE_ESC			27
#define ASCII_CODE_ENTER		13
#define ASCII_CODE_BACKSPACE	8

HANDLE hOut1, hOut2 ;


list<CTraceAbleLock*> CTraceAbleLock::g_LockList(0) ;

//=================================================================================
// Critical sections
CTraceAbleLock g_csDebufPrint ;
#define print_lock() g_csDebufPrint.tlock(__FILE__, __LINE__)
#define print_unlock() g_csDebufPrint.tunlock(__FILE__, __LINE__)



// This funs is Console controler
int ProcessCommand( char * cmd ) ;
BOOL KeyboardHandler(DWORD dwEvent ) ;
void ShutDownServer() ;
bool SockInit() ;

int g_iCharacBackupTime = _BACKUP_TIME_CHARAC_;
int g_iAccBackupTime = _BACKUP_TIME_USER_;

bool g_Threading = true ;
bool g_MainThreading = true ;
bool g_bConsoleCapture = true ;	// 콘솔 출력을 파일로 캡처한다.
bool g_bCloseServer = true ;	// true : 서버 점검중, false : 정상

// billing free login
bool g_bIsFreeLogin = false ;
bool g_bItemShopTestVersion = false;
bool g_bCashItemtest = false;

int g_MaxConnection = static_cast<int>(_FOREST_MAX_USER) ;

char g_ForestName[13] ;
bool g_bWanCon = false ;


int g_iInitLifeValue ;			// 체력
int g_iInitForceValue ;			// 내력
int g_iInitConcentValue ;		// 영력

#ifdef _LOGIN_STRESS_TEST_
unsigned long __stdcall th_loginTest( LPVOID lp ) ;
#endif


#ifdef _METEL_LOGIN_200608_
#pragma comment(lib, "GameAPI.lib")

void BillingIdentifyCallback(int iResult, char* szResultMsg, char* szUserID, int iBillingType, int iPoints, int iFreePoints);
void BillingLoginCallback(int iResult, char* szResultMsg, int iLoginSN, int iBillingType, int iUsedPoints, int iPoints, int iUsedFreePoints, int iFreePoints, TGATime *lpTime, TGATime* lpEndTime);
void BillingLogoutCallback(int iResult, char* szResultMsg, int iLoginSN, int iUsedPoints, int iPoints, int iUsedFreePoints, int iFreePoints, TGATime *lpTime, TGATime* lpEndTime);

extern CRITICAL_SECTION g_csBillingUser;
#endif //_METEL_LOGIN_200608_

#ifdef _VINA_SHUTDOWN_20061023_
CONSOLE_COMMAND g_ShutdownStep = CONSOLECMD_FORCED_LOGOFF;
int		g_iShutdownHour = 0;
#endif

#ifdef ITEMSHOP_PATCH_200607
#define INCLUDE_ODBC_HEADER
#include "./MyLibrary/gGlobal.h"
#endif

#ifdef _PD_COMPANY_JAPAN_
#include "Japan RMS/RocAuthSession.h"
#endif

#ifdef	_NEXON_NETWORK_
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "./Mylibrary/gGlobal.h"
#include "./SmallNet/Packet.h"
#include "./SmallNet/PktMgr.h"
#include "./SmallNet/PktQue.h"
#include "./SmallNet/Sock.h"
#include "./SmallNet/SmallNet.h"
#include "./SmallNet/PktProc/_PktDef.h"
#include "./SmallNet/PktProc/Nexon_PCBangAuth.h"



//CSock	gNexonPCB_sock;
//CS		gNexonPCB_cs;

//std::map<string/*strAccount*/, HANDLE /*hThreadSync*/>	gmapNXPCB_LoginSync;
//CS														gcsNXPCB_LoginSync;

char	NEXONPCB_AUTH_SERVER_IP[16];
WORD	NEXONPCB_AUTH_SERVER_PORT;
#endif

#ifdef _CIS_20070326_
#include "./ItemShop/CIS.h"
extern COdbc*	g_pCIS;
#endif

#ifdef	_NEXON_NETWORK_
unsigned __stdcall Thread_NEXONPCB_AliveSend(void* pv) {
	// 적당한 타이머가 없는데다, 새로 만들 시간도 없고 해서 그냥 ...

	HANDLE	h = CreateEvent(NULL, FALSE, FALSE, NULL);

	while( g_Threading ) {
		WaitForSingleObject(h, 60000);
	
		//gNexonPCB_cs.Lock();
		NEXONPCB_AliveNtf();
		//gNexonPCB_cs.Unlock();
	}

	CloseHandle(h);
	return 0;
}
#endif

// Command Param: 13 extra0 wan (Server Group, DB String, ?)
int main(int argc, char* argv[])
{
#ifdef _VINA_SHUTDOWN_20061023_
	g_iShutdownHour = GetPrivateProfileInt("SHUTDOWN", "Hour", 0, "./Env/VN_Shutdown.ini");
#endif

	/*
#ifdef	_NEXON_NETWORK_
	{
		// 패킷 매니저 생성
		if(FALSE == CPktMgr::CreateInstance(10, 10)) {
			Logout("#ERR Fail to running CPktMgr::CreateInstance()");
			printf("#ERR Fail to running CPktMgr::CreateInstance() \n");
			return 0;
		}

		bool bRet = NEXONPCB_Connect(NEXONPCB_AUTH_SERVER_IP, NEXONPCB_AUTH_SERVER_PORT, atoi(argv[1]));
		if(bRet == false) {
			// ...error
		}

		HANDLE h = (HANDLE)_beginthreadex(NULL, 0, Thread_NEXONPCB_AliveSend, NULL, 0, NULL);
		CloseHandle(h);
	}
#endif
	*/
#ifdef ITEMSHOP_PATCH_200607
	{
		for(int i = 0; i < NEXON_ITEM_SHOP_IP_CNT; i++) {
			ulIP_NexonItemShop[i] = inet_addr(pszNexonItemShopIP[i]);
		}
	}
#endif

	if( argc < 3 )
	{
		printf( "[INIT FAIL] Need 2 or more Arguments. Group Number & DB String Name, ...\n" ) ;
		goto label_end ;
	}

	printf( "sizeof(MSG_UPDATE_CHARAC_ALL) : size = %d\n", sizeof(MSG_UPDATE_CHARAC_ALL) );
	printf( "sizeof(CMSG_CHARAC_ALL_INFO_DS_GS) = %u\n", sizeof(CMSG_CHARAC_ALL_INFO_DS_GS)); 

	g_uiCompanyCode = en_lmbc_nexon ;

	int iTempCount;		// 생성자에 들어가는 임시 변수.

	int ret = 0;
#ifdef _PD_COMPANY_METEL_
	g_uiCompanyCode = en_lmbc_metel ;
#ifdef _METEL_LOGIN_200608_
	InitializeCriticalSectionAndSpinCount( &g_csBillingUser, (0x80000000)|2000 );

	ret = InitBilling(-1, BillingIdentifyCallback, BillingLoginCallback, BillingLogoutCallback);
	if(ret < 0) {
		printf( "[INIT ERROR] - Fail to InitBilling()\n" );
	}
	else {
		Logout(_T("Success to InitBilling()"));
		printf("Success to InitBilling() \n") ;
	}
#endif //_METEL_LOGIN_200608_
#endif

#ifdef _PD_COMPANY_VINA_	
	g_uiCompanyCode = en_lmbc_vina ;
#endif

#ifdef _PD_COMPANY_ACCLAIM_
	g_uiCompanyCode = en_lmbc_acclaim ;
#endif

#ifdef _PD_COMPANY_PWC_
	g_uiCompanyCode = en_lmbc_pwc ;
#endif

#ifdef _PD_COMPANY_JAPAN_
	g_uiCompanyCode = en_lmbc_rocworks ;
#endif

#ifdef _PD_COMPANY_RUSSIA_
	g_uiCompanyCode = en_lmbc_russia ;
#endif

#ifdef _PD_COMPANY_NXINTS_
	g_bCashItemtest = true;
#endif
	
	// 서버 군 설정
	g_ForestNum = atoi(argv[1]) ;
	if( strlen( argv[2] ) < 13 ) 
	{
		strcpy( g_ForestName, argv[2] ) ;
	}
	else
	{
		printf( "[INIT FAIL] Second Arguement is abnormal. too long.\n" ) ; 
		goto label_end ;
	}


	if( argc > 3 )
	{
		if( 0 == strcmp( "wan", argv[3] ) )
			g_bWanCon = true ;
	}

	if( argc > 4 )
	{
		if( 0 == strcmp( argv[4], "testshop" ) )
			g_bItemShopTestVersion = true;
	}

	//  System log
	g_pSystemLog = new _j_GlobalLog ;
	assert(g_pSystemLog) ;
	if( g_pSystemLog->LogFileCreate(_SYSTEM_LOG_FILE_,"w+") == false )
	{
		printf( "[INIT FAIL] SystemLogfile create Failed : Need %s\n", _SYSTEM_LOG_FILE_ ) ;
		goto label_end ;
	}

	//connection log
	g_pGMStLog = new _j_GlobalLog ;
	assert(g_pGMStLog) ;
	if( g_pGMStLog->LogFileCreate(_GMS_LOG_FILE_,"w+") == false )
	{
		printf( "[INIT FAIL] GMS Logfile create Failed : Need %s\n", _GMS_LOG_FILE_ ) ;
		goto label_end ;	
	}

	printf( "[INIT] Exception Report\n" ) ;
	g_pNDExceptionReport = new NDExceptionReport;
	assert( g_pNDExceptionReport ) ;
	if( !g_pNDExceptionReport )
	{
		printf( "[INIT FAIL] Exception Report File Create.\n" ) ;
		goto label_end ;
	}

#ifdef _PD_LEVEL_PRESENT_
	printf( " - Include Level Present\n" );
	g_pSystemLog->LogFileWrite( " - Include Level Present\n" );
#endif
#ifdef _PD_PKEVENT_BOX_SPAWN_
	printf( " = Include PKEVENT Box Spawn\n" );
	g_pSystemLog->LogFileWrite( " = Include PKEVENT Box Spawn\n" );
#endif

#ifdef _PD_EVENT_INVITE_FRIEND_
	printf( " = Include Invite Friend Event\n" );
	g_pSystemLog->LogFileWrite( " = Include Invite Friend Event\n" );
#endif

#ifdef _PD_CASH_INVEN_
	printf( " - Include Cash Inven & Depot extend.\n" );
	g_pSystemLog->LogFileWrite( " - Include Cash Inven & Depot extend.\n" );
#endif

	printf( " = Add Cash item test mode : test cashitme on/off\n" );
	g_pSystemLog->LogFileWrite( " = Add Cash item test mode : test cashitme on/off\n" );

#ifdef _PD_SKILL_EXTEND_
	printf( " - Extended SKill Slot version( both 80 & 100 )\n" );
	g_pSystemLog->LogFileWrite( " - Extended SKill Slot version( both 80 & 100 )\n" );
#else
	printf( " - SKill Slot Max Count = 60\n" );
	g_pSystemLog->LogFileWrite( " - SKill Slot Max Count = 60\n" );
#endif
	


	DWORD dwAcceptThreadID, dwTemp ; 
	HANDLE hAcceptThread=NULL ; 
	HANDLE hIn ;
	char cmdbuf[128] ;
	INPUT_RECORD rec ;
	CONSOLE_SCREEN_BUFFER_INFO Info ;
	bool bInputMode = false ;		// 입력 모드냐 출력 모드냐.( 입력모드이면 화면1, 출력모드이면 화면2 )
	bool bRet;
	char c ;
	int writepos = 0;

	//int ret = 0;

	// 서버 정보 읽어오기..
 	if( !g_ServerInfo.Init( SERVER_ENVIRONMENT_FILE, g_ForestNum ) ){ 
		printf( "[INIT FAIL] Parsing Server Info File is failed\n" ) ; 
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Parsing Server Info File is failed\n" ) ; 
		goto label_end ; 
	}

#ifdef	_NEXON_NETWORK_
	{
		// 음.. 지금 CSmallNet이 싱글턴으로 되어 있는데 설계가 잘못된 것 같음..
		// 어떻게 하면 공동 사용이 가능한지 다시 생각해 볼 것
		CSmallNet::SMALL_NET_INFO sni;
		sni.i256PoolSize = 10;
		sni.i1024PoolSize = 10;
		sni.iPktProcNum = 1;
		sni.iRecvProcNum = 1;
		sni.iSendProcNum = 1;

		CSmallNet::CONNECT_INFO ci;
		strcpy(ci.pszIp, NEXONPCB_AUTH_SERVER_IP);
		ci.wPort = NEXONPCB_AUTH_SERVER_PORT;

		if( NEXONPCB_AUTH_SERVER_PORT && !CSmallNet::CreateInstance(sni, ci, NetEventProc, PktHandler)) {
			printf("Fail to Nexon PC Auth Server( CSmallNet) \n");
			CSmallNet::DestroyInstance();
#ifndef _NXPCB_INACT_AWARD_200703_
			return 0;
#endif
		}
		else
			printf("Success to Nexon PC Auth Server( CSmallNet). \n");
		Sleep(100);
		if( NEXONPCB_AUTH_SERVER_PORT )
		{
			NEXONPCB_InitNtf( atoi(argv[1]) );

			HANDLE h = (HANDLE)_beginthreadex(NULL, 0, Thread_NEXONPCB_AliveSend, NULL, 0, NULL);
			CloseHandle(h);
		}
	}
#endif

#ifdef _PD_COMPANY_JAPAN_
	if( FALSE == CRocAuthSession::SetRocworksAuthServerInformation( g_ServerInfo.get_roc_auth_ip(), g_ServerInfo.get_roc_auth_port() ) )
	{
		g_pSystemLog->LogFileWrite( "[EXIT]  Check RMS Setting\n " );
		printf( "[EXIT]  Check RMS Setting\n " );
		goto label_end ; 
	}
#endif
	
	// 캐릭터 생성시 초기 수치값 읽어오기..
	if( ReadInitValueFile( INIT_VALUE_REF_FILE, g_iInitLifeValue, g_iInitForceValue, g_iInitConcentValue ) == false )
	{
		printf( "[INIT FAIL] Parsing Character InitValue File is failed\n" ) ; 
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Parsing Character InitValue File is failed\n" ) ; 
		goto label_end ;
	}	

	// Thread event handler
	g_hInitEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ;
	if( g_hInitEvent == NULL ) {
		printf( "[INIT FAIL] Global Event Object Failed\n" ) ;
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Global Event Object Failed\n" ) ;
		goto label_end ;
	}

	g_pPacketChecker = new CND_PacketLengthChecker ;
	g_pPacketChecker->init() ;
	
	g_pSystemLog->Reflush() ;
			
	printf( ">>>>>>>>>>>>>>>>> DS_SERVER (%s) <<<<<<<<<<<<<<<<\n\n", __TIMESTAMP__);
	printf( "------------------------------------------------------------------------\n");
	printf( "INITAILIZE INFOMATION --------------------------------------------------\n\n");

	g_pSystemLog->LogFileWrite( ">>>>>>>>>>>>>>>>> DS_SERVER (%s) <<<<<<<<<<<<<<<<\n\n", __TIMESTAMP__);
	g_pSystemLog->LogFileWrite( "------------------------------------------------------------------------\n");
	g_pSystemLog->LogFileWrite( "INITAILIZE INFOMATION --------------------------------------------------\n\n");

	printf( "[INIT] Show Info\n" ) ;
	
	// 소켓 초기화
	WSADATA wsd ;
	if( WSAStartup( MAKEWORD( 2, 2 ), &wsd ) != 0 )	{
		printf( "[INIT FAIL] WSAStartup fail : %d\n", WSAGetLastError() ) ;
		g_pSystemLog->LogFileWrite( "[INIT FAIL] WSAStartup fail : %d\n", WSAGetLastError() ) ;
		goto label_end ;		
	}

{	
	// db connection string

	WCHAR oleString[128] = {0};
	WCHAR userdb_oleString[512] = {0};
	WCHAR eventDBConString[512] = {0};
	
	printf( "[INIT] Read Key box\n" ) ;
	
	CSqlConnectorString	sqlConStr( CRYPTO_KEY_FILE ) ;

	printf( "[INIT] Get Con String\n" ) ;

	try
	{
		if( sqlConStr.ReadConStrFile(SQL_CONNECTION_STRING) == false )
		{
			printf( "[INIT FAIL] Init DB Con string\n" ) ;
			g_pSystemLog->LogFileWrite( "[INIT FAIL] Init DB Con string\n" ) ;
			goto label_end ;
		}
	}
	catch( ... )
	{
		printf( "[INIT FAIL] Exception in ReadConStrFile.\n" ) ;
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Exception in ReadConStrFile.\n" ) ;
		goto label_end ;
	}

	printf( "[INIT] Get DB Con string from input\n" ) ;

	int iIndex = -1 ;
	int iIDC = -1 ;
	bool bIDC = false ;
	char * pString = NULL ;
	const char * pGameConString = NULL ;
	const char * pAccConString = NULL ;

	iIndex = DBF_GetGroupIndex( argv[2] ) ;
	iIDC = DBF_IsIDC( argv[2] ) ;

	if( iIndex < 0 || iIDC < 0 )
	{
		printf( "[INIT FAIL] Arg[2] is not proper\n" ) ;
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Arg[2] is not proper\n" ) ;
		goto label_end ;
	}

	bIDC = ( iIDC == 0 ) ;
	
	// Game DB
	pString = DBF_GetDBStringName(game_db_string, iIndex, bIDC) ;
	pGameConString = sqlConStr.GetConStr( pString ) ;

	if( NULL == pGameConString )
	{
		printf( "[INIT FAIL] [%s] Game DB Connect string name do not exist\n", pString ) ;
		g_pSystemLog->LogFileWrite( "[INIT FAIL] [%s] Game DB Connect string name do not exist\n", pString ) ;
		goto label_end ;
	}

	pString = DBF_GetDBStringName(account_db_string, iIndex, bIDC) ;
	pAccConString = sqlConStr.GetConStr( pString ) ;

	if( NULL == pAccConString )
	{
		printf( "[INIT FAIL] [%s] Account DB Connect string name do not exist\n", pString ) ;
		g_pSystemLog->LogFileWrite( "[INIT FAIL] [%s] Account DB Connect string name do not exist\n", pString ) ;
		goto label_end ;
	}

	MultiByteToWideChar(CP_ACP, 0, pGameConString, -1, oleString, strlen(pGameConString) ) ;

	MultiByteToWideChar(CP_ACP, 0, pAccConString, -1, userdb_oleString, strlen(pAccConString) ) ;	
	{
		char msg[512] ;
		sprintf( msg, "Use %s - %d", bIDC ? "IDC" : "EXTRA", iIndex ) ;

		printf( "[INIT INFO] %s\n", msg );
		g_pSystemLog->LogFileWrite( "[INIT INFO] %s\n", msg );

		Sleep(2000);
	}
	

#ifdef _INNER_SERVER
	g_bCloseServer = false ;
#endif

	/*
#ifdef _CIS_20070326_
	const char*	pszCISConnString = NULL ;
	WCHAR		pwszCISConnString[256] = {0};

	pString = DBF_GetDBStringName(cis_db_string, iIndex, false);	// true=REAL_CONN_STRING, false=TEST_CONN_STRING
	pszCISConnString = sqlConStr.GetConStr( pString ) ;
	MultiByteToWideChar(CP_ACP, 0, pszCISConnString, -1, pwszCISConnString, strlen(pszCISConnString));
#endif
	*/

	printf( "[INIT] Con String - Reset\n" ) ;
	// 
	sqlConStr.Reset() ;

	// ole-db connecter
	printf( "[INIT] Game DB Connector Initialize \n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Game DB Connector Initialize \n");
	g_pDBCon = new CDB_Connector ;
	if( !g_pDBCon->initialize( oleString ) ) // extra0: PROVIDER=SQLOLEDB;SERVER=221.147.34.10;UID=gamesa;PWD=ozzy2000;DATABASE=ND_GAME_1
	{
		DBErrorConsole( g_pDBCon->GetHresult() ) ;
		printf( "[INIT FAIL] Error : Game DB Connection Fail\n" ) ;
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Error : Game DB Connection Fail\n" ) ;
		goto label_end ;
	}

	// ole-db connecter
	printf( "[INIT] Account DB Connector Initialize \n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Account DB Connector Initialize \n");
	g_pUSERDBCon = new CDB_Connector ;
	if( !g_pUSERDBCon->initialize( userdb_oleString ) ) // extra0: PROVIDER=SQLOLEDB;SERVER=221.147.34.10;UID=gamesa;PWD=ozzy2000;DATABASE=NineDragons
	{
		DBErrorConsole( g_pUSERDBCon->GetHresult() ) ;
		printf( "[INIT FAIL] Error : Account DB Connection Fail\n" ) ;
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Error : Account DB Connection Fail\n" ) ;
		goto label_end ;
	}

	/*
#ifdef _CIS_20070326_
	g_pDBCon_CIS = new CDB_Connector;
	if(!g_pDBCon_CIS->initialize(pwszCISConnString)) {
		DBErrorConsole(g_pDBCon_CIS->GetHresult());
		_tprintf(_T("Error: g_pDBCon_CIS->init() Failed"));
		goto label_end;
	}

	g_pDBCmd_CIS = new CDBCmd_CIS(g_pDBCon_CIS);
	assert(g_pDBCmd_CIS);
#endif
	*/

	//////////////////////////////////////////////////////////////////////////
	// 2006.06.02 Molla, Nexon Web Shop
#ifdef ITEMSHOP_PATCH_200607
	/*
	g_pDBCon_WebShop = new CDB_Connector;
	if(!g_pDBCon_WebShop->initialize(L"PROVIDER=SQLOLEDB;SERVER=221.147.34.129;UID=sa;PWD=xogmltkfkd;DATABASE=pubs")) {
		DBErrorConsole(g_pDBCon_WebShop->GetHresult());
		_tprintf(_T("Error: g_pDBCon_WebShop->init() Failed"));
		Sleep(7000);
		goto label_end;
	}

	g_pDBCmd_WebShop = new CDBCommand_WebShop(g_pDBCon_WebShop);
	assert(g_pDBCmd_WebShop);
	*/
#endif
	//////////////////////////////////////////////////////////////////////////

}

	// OLE-DB Module Initialize	
	printf( "[INIT] Game DB Command OLE-DB Module Initialize \n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Game DB OLE-DB Module Initialize \n");
	g_pDB_Command = new COLE_DB_Executer(g_pDBCon) ;
	assert( g_pDB_Command ) ;


	// OLE-DB Module Initialize
	printf( "[INIT] Account DB Command OLE-DB Module Initialize \n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Account DB OLE-DB Module Initialize \n");
	g_pUser_Command = new C_USER_DB_Command(g_pUSERDBCon) ;
	assert( g_pUser_Command ) ;


	// read server state form db
	g_pDB_Command->GetServerStateFlag( &g_serverStateFlag ) ;
#ifdef _PD_COMPANY_VINA_
	if( g_serverStateFlag == 0 )
		g_serverStateFlag = 1;		// 비나 게임즈에서는 기본적으로 6개 문파를 다 연다.
#endif

#ifdef _PD_MASTER_PUPIL_

	g_pMPSet = new CDSMasterSet;
	if( false == g_pMPSet->CreateMasterPupilManager( g_pDB_Command, 3000, 200, 2000 ) )
	{
		g_pSystemLog->LogFileWrite( "[INIT] CDSMasterSet::CreateMasterPupilManager Failed\n" );
		goto label_end;
	}
#endif

	
	// Log server
	printf( "[INIT] CLogSvr_Com Initialize \n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] CLogSvr_Com Initialize \n");
	g_pLogSvrCom = new CLogSvr_Com ;
	{
		char ip[17] = {0} ;
		u_short port ;
		if( g_pLogSvrCom->ReadInfoFile( LOG_SVR_INFO_FILE, static_cast<short>( atoi(argv[1]) ), ip, &port ) == false )
		{
			printf( "[INIT FAIL] Log SVR Info file read failed\n" );
			g_pSystemLog->LogFileWrite( "[INIT FAIL] Log SVR Info file read failed\n" );
			goto label_end ;
		}
		if( g_pLogSvrCom->createCSCom( ip/*logsvr ip*/, port/*logsvr port*/) == false )
		{
			printf( "[INIT FAIL] Log SVR Connect failed\n" );
			g_pSystemLog->LogFileWrite( "[INIT FAIL] Log SVR Info file read failed\n" );
			goto label_end ;
		}
	}


	int i;
	printf( "[INIT] CLogSvr_Com Initialize \n" ) ;
	g_pTrace = new CMYTrace ; 	
	assert( g_pTrace ) ;	
	for( i = 0 ; i < _TRACE_CLASS_MAX ; i++ )
	{
		g_pTrace->SetHeadTitle( i , traceName[i] ) ;
	}

	// GMS 메시지는 항상 트레이스한다.
	g_pTrace->ShowTraceString( _TRACE_CLASS_GMS, 1 ) ;

	

	// Uniqueue ID 관리자..
	g_pUniqueIDQueue = new _j_UniqueIDQueue ;
	assert(g_pUniqueIDQueue) ;
	if ( g_pUniqueIDQueue->Initalize(/*_UNIQUEID_MAX_COUNT*/) == false ) {
		printf( "[INIT FAIL] g_pUniqueIDQueue->Initalize() Fail : Check Memory\n" );
		g_pSystemLog->LogFileWrite( "[INIT FAIL] g_pUniqueIDQueue->Initalize() Fail : Check Memory\n" );
		goto label_end ;		
	}
	printf( "g_pUniqueIDQueue->Initalize() \n" ) ;
	g_pSystemLog->LogFileWrite( "g_pUniqueIDQueue->Initalize() \n" );


	/* 패킷 큐 생성, IOCP 객체 생성 & 읽기 스레드, 처리 스레드 생성  */
	//  1. 패킷 큐 클래스 생성	g_pPacketQ
	printf( "[INIT] Packet Queue Create\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Packet Queue Create\n");
	iTempCount = _GENERAL_QUEUE_MEMORY_SIZE;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/4;
	g_pPacketQ = new _queue_Packet(iTempCount) ;
	assert( g_pPacketQ ) ;	

	printf( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packet))/_MEGA_ ) ;
	g_pSystemLog->LogFileWrite( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packet))/_MEGA_ );


	printf( "[INIT] UpdatePacket Queue Create\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] UpdatePacket Queue Create\n");
	iTempCount = _UPDATE_QUEUE_MEMORY_SIZE;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/8;
	else if( en_service_mode_outertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/4;
	g_pUpdatePacketQ = new _queue_Packet(iTempCount) ;
	assert( g_pUpdatePacketQ ) ;	

	printf( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packet))/_MEGA_ ) ;
	g_pSystemLog->LogFileWrite( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packet))/_MEGA_ );

	
	//  Party 관련 packet queue 생성..
	printf( "[INIT] Party Packet Queue Create\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Party Packet Queue Create\n");
	iTempCount = _PARTY_QUEUE_MEMORY_SIZE;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/4;
	g_pCSPacketQ = new _queue_Packet(iTempCount) ;
	assert( g_pCSPacketQ ) ;	

	printf( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packet))/_MEGA_ ) ;
	g_pSystemLog->LogFileWrite( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packet))/_MEGA_ );


	//  OR 관련 packet queue 생성..
	printf( "[INIT] OR Packet Queue Create\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] OR Packet Queue Create\n");
	iTempCount = _OR_QUEUE_MEMORY_SIZE;
	g_pORPacketQ = new _queue_Packet(iTempCount) ;
	assert( g_pORPacketQ ) ;	

	printf( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packet))/_MEGA_ ) ;
	g_pSystemLog->LogFileWrite( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packet))/_MEGA_ );


	//  Chat 관련 packet queue 생성..
	printf( "[INIT] Chat Packet Queue Create\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Chat Packet Queue Create\n");
	iTempCount = _CHAT_QUEUE_MEMORY_SIZE;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/8;
	else if( en_service_mode_outertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/2;
	g_pChatPacketQueue = new _queue_Packet(iTempCount) ;
	assert( g_pChatPacketQueue ) ;	

	printf( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packet))/_MEGA_ ) ;
	g_pSystemLog->LogFileWrite( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packet))/_MEGA_ );



	/* Create Send packet queue for UDP type */
	printf( "[INIT] Create Send Packet Queue for UDP Type \n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Send Packet Queue for UDP Type \n");
	iTempCount = _UDP_SEND_QUEUE_MEMORY_SIZE;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/8;
	else if( en_service_mode_outertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/2;
	g_pUDPSendPacketQ = new _j_queue_UDP_Packet(iTempCount) ;
	assert( g_pUDPSendPacketQ ) ;	

	printf( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packetUDP))/_MEGA_ ) ;
	g_pSystemLog->LogFileWrite( "-> Size of UDP Type Send packet Q : %d Mega\n", (iTempCount * sizeof(_packetUDP))/_MEGA_ );


	/* Create Recv packet queue for UDP type */
	printf( "[INIT] Create Recv Packet Queue for UDP Type \n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Recv Packet Queue for UDP Type \n");
	iTempCount = _UDP_RECV_QUEUE_MEMORY_SIZE;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/4;
	g_pUDPRecvPacketQ = new _j_queue_UDP_Packet(iTempCount) ;
	assert( g_pUDPRecvPacketQ ) ;	
		
	printf( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packetUDP))/_MEGA_ ) ;
	g_pSystemLog->LogFileWrite( "-> Size of UDP Type Send packet Q : %d Mega\n", (iTempCount * sizeof(_packetUDP))/_MEGA_ );


	/* Create Login packet queue for UDP type */
	printf( "[INIT] Create UDP Login Packet Queue for UDP Type \n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create UDP Login Packet Queue for UDP Type \n");
	iTempCount = _UDP_LOGIN_QUEUE_MEMORY_SIZE*2 ;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/8;
	else if( en_service_mode_outertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/2;
	g_pUDPLoinPacketQ = new _j_queue_UDP_Packet(iTempCount) ;		// 로그인 큐는 패킷이 작고 매우 순환적이다.	
	assert( g_pUDPLoinPacketQ ) ;												// 그래서 슬롯 갯수도.. n * 2  이다.

	printf( "-> Size of packet Q : %d Mega\n", (iTempCount * sizeof(_packetUDP))/_MEGA_ ) ;
	g_pSystemLog->LogFileWrite( "-> Size of UDP Type Send packet Q : %d Mega\n", (iTempCount * sizeof(_packetUDP))/_MEGA_ );


	// Initailize global socket
	if( !SockInit() ){
		printf( "[INIT FAIL] 'SockInit()' Socket initailize failed\n" );
		g_pSystemLog->LogFileWrite( "[INIT FAIL] 'SockInit()' Socket initailize failed\n" );
		goto label_end ;
	}


	
	// USER ACC MAP 
	printf( "[INIT] User AccID MAP Create\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] User AccID MAP Create\n");
	iTempCount = _PERMISSIBLE_MAX_USER;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/100;
	else if( en_service_mode_outertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/5;
	g_pAccMapManager = new _j_AccMAPManager(iTempCount) ;
	assert( g_pAccMapManager ) ;


	// Backup 대기 객체
	printf( "[INIT] BackupReay Create \n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] BackupReay Create \n" ) ;
	g_pBackupReady = new _j_BackupReady ;
	assert( g_pBackupReady ) ;
	if ( g_pBackupReady->init() == false ) {
		printf( "[INIT FAIL] g_pBackupReady init failed\n" );
		g_pSystemLog->LogFileWrite( "[INIT FAIL] g_pBackupReady init failed\n" );
		goto label_end ;
	}

	// 2. IOCP	CREATE
	printf( "[INIT] IO Completion Port Create\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] IO Completion Port Create\n" ) ;
	g_pIOCP = new _h_CompletionPort ;
	assert( g_pIOCP ) ;
	if( g_pIOCP->Create(1) == false ) {
		printf( "[INIT FAIL] IOCP->Create failed\n" );
		g_pSystemLog->LogFileWrite( "[INIT FAIL] IOCP->Create failed\n" );
		goto label_end ;		
	}

	// DS Com Queue
	printf( "[INIT] DS Com Create\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] DS Com Create\n" ) ;
	g_pDSComQueue = new _DSCom_Queue ;
	assert( g_pDSComQueue ) ;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		bRet = g_pDSComQueue->Init( OV_SLOT_MAX_COUNT/20, OV_SLOT_MAX_SIZE/2, _RECV_BUF_SIZE_/2 );
	else if( en_service_mode_outertest == g_ServerInfo.get_service_mode() )
		bRet = g_pDSComQueue->Init( OV_SLOT_MAX_COUNT/10, OV_SLOT_MAX_SIZE/2, _RECV_BUF_SIZE_/2 );
	else
		bRet = g_pDSComQueue->Init( OV_SLOT_MAX_COUNT, OV_SLOT_MAX_SIZE, _RECV_BUF_SIZE_ );
	
	if( bRet == false ) 
	{
		printf( "[INIT FAIL] g_pDSComQueue->Init() failed\n" );
		g_pSystemLog->LogFileWrite( "[INIT FAIL] g_pDSComQueue->Init() failed\n" );
		goto label_end ;		
	}

	// Party Queue
	g_pPartyQueue = new _j_PartyQueue ;
	assert(g_pPartyQueue) ;


	// Party Map Manager
	g_pPartyManager = new _j_PartyManager ;
	assert(g_pPartyManager) ;
	if ( g_pPartyManager->Initalize() == false ) {
		g_pSystemLog->LogFileWrite( "[INIT FAIL] PartyManager Initalize failed \n");
		printf( "[INIT FAIL] PartyManager Initalize failed\n" );
		goto label_end ;
	}
	g_pSystemLog->LogFileWrite( "Party Manager : %x\n", g_pPartyManager );

	g_pPartyTimer = new _h_Party_Timer ;
	assert(g_pPartyTimer) ;
	if( g_pPartyTimer->create() == false )
	{
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Party_Timer Initalize failed \n");
		printf( "[INIT FAIL] Party_Timer Initalize failed\n" );
		goto label_end ;
	}

	// Member Queue
	g_pMemberQueue = new _j_MemeberQueue ;
	assert(g_pMemberQueue) ;
	iTempCount = _PERMISSIBLE_MAX_CHARAC;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/100;
	else if( en_service_mode_outertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/5;
	if ( g_pMemberQueue->Create( iTempCount ) == false ) {
		g_pSystemLog->LogFileWrite( "[INIT FAIL] MemberQueue Create Failed\n");
		printf( "[INIT FAIL] MemberQueue Create Failed\n" );
		goto label_end ;
	}

	// Mem Save
	printf( "[INIT] Mem save Initializing\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Mem save Initializing\n" ) ;
	g_pMemSaveTimer = new CUserMemSaveTimer(g_pAccMapManager) ;
	assert( g_pMemSaveTimer ) ;



	printf("[INIT] Ranking manager initialize \n" ) ;
	g_pRankingManager = new CRankingManager ;
	assert( g_pRankingManager ) ;	
	
	// 랭킹 초기화		
	bool bGetRank = g_pDB_Command->GetRankVersionInfo() ;	
	if( bGetRank == false )
	{
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Set Ranking info failed\n" );
		printf( "[INIT FAIL] Set Ranking info failed\n" );
		goto label_end ;
	}

		
	// 분파/ 방회
	printf( "[INIT] Organization manager initialize \n" ) ;
	iTempCount = _ORGANIZATION_INIT_SIZE_;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/100;
	else if( en_service_mode_outertest == g_ServerInfo.get_service_mode() )
		iTempCount = iTempCount/10;
	g_pOrganizationManager = new COrganizationManager( iTempCount ) ;
	assert( g_pOrganizationManager ) ;	

		
	bRet = g_pOrganizationManager->ReadORDataFromDB( g_pDB_Command ) ;	 		
	if( bRet == false  )
	{
		printf( "[INIT FAIL] Organization data read failed\n" );
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Organization data read failed\n" );
		goto label_end ;
	}

#ifdef _PD_ALLY_ORG_STEP_1_
	g_pAllyManager = new CAllyManager( 1000 );
	
	if( !g_pAllyManager || false == g_pAllyManager->read_ally_info( g_pOrganizationManager, g_pDB_Command ) )
	{
		if( g_pAllyManager )
		{
			printf( "[INIT FAIL] Ally Manager failed -> Check Ally DB Tables or SPs.\n" );
			g_pSystemLog->LogFileWrite( "[INIT FAIL] Ally Manager failed -> Check Ally DB Tables or SPs.\n" );
		}
		else
		{
			printf( "[INIT FAIL] Ally Manager failed -> not enough memory.\n" );
			g_pSystemLog->LogFileWrite( "[INIT FAIL] Ally Manager failed -> not enough memory.\n" );
		}
		goto label_end;
	}

	g_pOrganizationManager->CheckInitialAlly( g_pDB_Command );
	
#endif

#ifdef _PD_CASTLE_STEP_1_
	// 아직 DestroySingleton 호출을 하지 않았다. 처리할 것.
	if( false == CDSCastleManager::CreateSingleton() || false == CDSCastleManager::GetObj().read_castle_info_from_db( g_pDB_Command ) )
	{
		printf( "[INIT FAILED] Read Castle Information Failed\n" );
		g_pSystemLog->LogFileWrite( "[INIT FAILED] Read Castle Information Failed\n" );
		goto label_end;
	}
	// check owner & challengers.
	CDSCastleManager::GetObj().Check_OrgAlly( g_pOrganizationManager, g_pAllyManager );

#endif	// _PD_CASTLE_STEP_1_

#ifdef _PD_CASTLE_STEP_2_
	if(false == CManorTaxManager::CreateSingleton())
	{
		printf( "[INIT FAILED] Read CManorTaxManager Info Failed\n" );
		g_pSystemLog->LogFileWrite( "[INIT FAILED] Read CManorTaxManager Info Failed\n" );
		goto label_end;
	}

	CDSCastleManager::GetObj().SetRecord();
	if(false == CDSCastleManager::GetObj().read_record())
	{
		printf( "[INIT FAILED] Read CRecordOfCombat Info Failed\n" );
		g_pSystemLog->LogFileWrite( "[INIT FAILED] Read CRecordOfCombat Info Failed\n" );
		goto label_end;
	}	
#endif//_PD_CASTLE_STEP_2_

#ifdef _PD_GAME_EVENT_
	printf( "[INIT] Start : Free Match Manager\n" );
	g_pGameEvent =  new CGameEventManager( g_pDB_Command );
	if( !g_pGameEvent || false == g_pGameEvent->CreateGEventManager( 100, 1000 ) )
	{
		printf( "[INIT FAILED] New/Read Game Event : Failed\n" );
		g_pSystemLog->LogFileWrite( "[INIT FAILED] New/Read Game Event : Failed\n" );
		goto label_end;
	}
	printf( "[INIT] Success : Free Match Manager\n" );
#endif
	
	// 3. 패킷 처리 스레드 -패킷큐에 박힌 패킷을 뽑아다가 처리해준다..	
	iTempCount = _THREADNUMBER_PACKET_PROCESS_;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		iTempCount = 2;
	printf( "[INIT] Create Packet process Thread n = %d \n", iTempCount ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Packet process Thread n = %d \n", iTempCount ) ;
	for( i = 0 ; i < iTempCount ; i++ ) {
		ResetEvent( g_hInitEvent );
		hAcceptThread = BEGINTHREADEX( NULL, 0, th_PacketProcess, NULL, 0, &dwAcceptThreadID ) ;
		if( hAcceptThread != INVALID_HANDLE_VALUE ) {	
			CloseHandle(hAcceptThread) ; 
			hAcceptThread = INVALID_HANDLE_VALUE ; 
			if( WAIT_TIMEOUT ==  WaitForSingleObject( g_hInitEvent, 20000 ) )
			{
				printf( "[INIT FAIL] Timeout : th_packetprocess\n" );
				g_pSystemLog->LogFileWrite( "[INIT FAIL] Timeout : th_packetprocess\n" );
				goto label_end;
			}
		}
		else { 
			printf( "[INIT FAIL] Packet thread failed\n" ); 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] Packet thread failed\n" ); 
			goto label_end ; 
		}		
	}


	// 업데이트 쓰레드..
	printf( "[INIT] Create Update process Thread n = %d \n", _THREADNUMBER_UPDATE_PROCESS_ ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Update process Thread n = %d \n", _THREADNUMBER_UPDATE_PROCESS_ ) ;
	for( i = 0 ; i < _THREADNUMBER_UPDATE_PROCESS_ ; i++ ) {
		ResetEvent( g_hInitEvent );
		hAcceptThread = BEGINTHREADEX( NULL, 0, th_UpdateProcess, NULL, 0, &dwAcceptThreadID ) ;
		if( hAcceptThread != INVALID_HANDLE_VALUE ) {	
			CloseHandle(hAcceptThread) ; 
			hAcceptThread = INVALID_HANDLE_VALUE ; 
			if( WAIT_TIMEOUT ==  WaitForSingleObject( g_hInitEvent, 20000 ) )
			{
				printf( "[INIT FAIL] Timeout th_UpdateProcess\n" ); 
				g_pSystemLog->LogFileWrite( "[INIT FAIL] Timeout th_UpdateProcess\n" ); 
				goto label_end ; 
			}
		}
		else { 
			printf( "[INIT FAIL] Update thread failed\n" ); 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] Update thread failed\n" ); 
			goto label_end ; 
		}		
	}
	


	// 백업쓰레드
	printf( "[INIT] Create CHARACTER Backup process Thread n = %d \n", _THREADNUMBER_CHARAC_BACKUP_) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create CHARACTER Backup process Thread n = %d \n", _THREADNUMBER_CHARAC_BACKUP_) ;
	for( i = 0 ; i < _THREADNUMBER_CHARAC_BACKUP_ ; i++ ) {
		ResetEvent( g_hInitEvent );
		hAcceptThread = BEGINTHREADEX( NULL, 0, th_CharacBackupthread, NULL, 0, &dwAcceptThreadID ) ;
		if( hAcceptThread != INVALID_HANDLE_VALUE ) {	
			CloseHandle(hAcceptThread) ; 
			hAcceptThread = INVALID_HANDLE_VALUE ; 

			if( WAIT_TIMEOUT ==  WaitForSingleObject( g_hInitEvent, 20000 ) )
			{
				printf( "[INIT FAIL] Timeout th_CharacBackupthread\n" ); 
				g_pSystemLog->LogFileWrite( "[INIT FAIL] Timeout th_CharacBackupthread\n" ); 
				goto label_end ; 
			}
		}
		else { 
			printf( "[INIT FAIL] CharacBackup thread failed\n" ); 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] CharacBackup thread failed\n" ); 
			goto label_end ; 
		}		
	}

	printf( "[INIT] Create USER Backup process Thread n = %d \n", _THREADNUMBER_USER_BACKUP_) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create USER Backup process Thread n = %d \n", _THREADNUMBER_USER_BACKUP_) ;
	for( i = 0 ; i < _THREADNUMBER_USER_BACKUP_ ; i++ ) {
		ResetEvent( g_hInitEvent );
		hAcceptThread = BEGINTHREADEX( NULL, 0, th_AccountBackupThread, NULL, 0, &dwAcceptThreadID ) ;
		if( hAcceptThread != INVALID_HANDLE_VALUE ) {
			CloseHandle(hAcceptThread) ;
			hAcceptThread = INVALID_HANDLE_VALUE ;
			if( WAIT_TIMEOUT ==  WaitForSingleObject( g_hInitEvent, 20000 ) )
			{
				printf( "[INIT FAIL] Timeout th_AccountBackupthread\n" ); 
				g_pSystemLog->LogFileWrite( "[INIT FAIL] Timeout th_AccountBackupthread\n" ); 
				goto label_end ; 
			}
		}
		else {
			printf( "[INIT FAIL] UserBackup thread failed\n" );
			g_pSystemLog->LogFileWrite( "[INIT FAIL] UserBackup thread failed\n" );
			goto label_end ;
		}
	}


	printf( "[INIT] Create Party Work process Thread n = %d \n", _THREADNUMBER_PARTY_WORK) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Party Work process Thread n = %d \n", _THREADNUMBER_PARTY_WORK) ;
	for( i = 0 ; i < _THREADNUMBER_PARTY_WORK ; i++ ) {
		hAcceptThread = BEGINTHREADEX( NULL, 0, th_CSWorkThread, NULL, 0, &dwAcceptThreadID ) ;
		if( hAcceptThread != INVALID_HANDLE_VALUE ) {
			CloseHandle(hAcceptThread) ;
			hAcceptThread = INVALID_HANDLE_VALUE ;
		}
		else {
			printf( "[INIT FAIL] Party Work thread failed\n" );
			g_pSystemLog->LogFileWrite( "[INIT FAIL] Party Work thread failed\n" );
			goto label_end ;
		}
	}


	printf( "[INIT] Create Organization Work process Thread n = %d \n", _THREADNUMBER_OR_WORK) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Organization Work process Thread n = %d \n", _THREADNUMBER_OR_WORK) ;
	for( i = 0 ; i < _THREADNUMBER_OR_WORK ; i++ ) {
		hAcceptThread = BEGINTHREADEX( NULL, 0, th_ORWorkThread, g_pORPacketQ, 0, &dwAcceptThreadID ) ;
		if( hAcceptThread != INVALID_HANDLE_VALUE ) {
			CloseHandle(hAcceptThread) ;
			hAcceptThread = INVALID_HANDLE_VALUE ;
		}
		else {
			printf( "[INIT FAIL] Organization Work thread failed\n" );
			g_pSystemLog->LogFileWrite( "[INIT FAIL] Organization Work thread failed\n" );
			goto label_end ;
		}
	}


	printf( "[INIT] Create CS Chat Work process Thread n = %d \n", _THREADNUMBER_CS_CHAT_WORK) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Chat CS Work process Thread n = %d \n", _THREADNUMBER_CS_CHAT_WORK) ;
	for( i = 0 ; i < _THREADNUMBER_CS_CHAT_WORK ; i++ ) {
		hAcceptThread = BEGINTHREADEX( NULL, 0, th_CSChatWorkThread, NULL, 0, &dwAcceptThreadID ) ;
		if( hAcceptThread != INVALID_HANDLE_VALUE ) {
			CloseHandle(hAcceptThread) ;
			hAcceptThread = INVALID_HANDLE_VALUE ;
		}
		else {
			printf( "[INIT FAIL] CS CHAT thread failed\n" );
			g_pSystemLog->LogFileWrite( "[INIT FAIL] CS CHAT thread failed\n" );
			goto label_end ;
		}
	}
	// UDP Send thread
	for( i = 0 ; i < _THREADNUMBER_UDP_PACKET_SEND_ ; i++ ) 
	{				
		g_pSystemLog->LogFileWrite( "[INIT] Create UDP Send process Thread n = 1 \n" ) ;
		hAcceptThread = BEGINTHREADEX( NULL, 0, th_UDPSendThread, NULL, 0, &dwAcceptThreadID ) ;
		if( hAcceptThread != INVALID_HANDLE_VALUE ) {	
			CloseHandle(hAcceptThread) ; 
			hAcceptThread = INVALID_HANDLE_VALUE ; 
		}
		else { 
			printf( "[INIT FAIL] UDP Send thread failed\n" ); 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] UDP Send thread failed\n" ); 
			goto label_end ; 
		}	
	}
	printf( "[INIT] Create UDP Send process Thread n = %d \n",  _THREADNUMBER_UDP_PACKET_SEND_) ;
	
	// UDP Recv thread
	for( i = 0 ; i < _THREADNUMBER_UDP_PACKET_RECV_ ; i++ ) {
		
		g_pSystemLog->LogFileWrite( "[INIT] Create UDP Recv process Thread n = 1 \n" ) ;
		hAcceptThread = BEGINTHREADEX( NULL, 0, th_UDPRecvThread, NULL, 0, &dwAcceptThreadID ) ;
		if( hAcceptThread != INVALID_HANDLE_VALUE ) {	
			CloseHandle(hAcceptThread) ; 
			hAcceptThread = INVALID_HANDLE_VALUE ; 
		}
		else { 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] UDP Recv thread failed\n" ) ; 
			goto label_end ; 
		}		
	}
	printf( "[INIT] Create UDP Recv process Thread n = %d \n",  _THREADNUMBER_UDP_PACKET_RECV_) ;

	printf( "[INIT] GMS Tcp Acceptor Start\n" ) ;
	g_pGMSAcceptor = new CConnectionManager;
	if( !g_pGMSAcceptor )
	{
		g_pSystemLog->LogFileWrite( "[INIT FAIL] GMS Acceptor Cannot Create\n" );
		goto label_end;
	}
	if( false == g_pGMSAcceptor->InitConnectionManager( CConnectionManager::en_work_mode_ds, 64, 
		g_ServerInfo.GetGMSAcceptIP(), g_ServerInfo.GetGMSAcceptPort() ) )
	{
		g_pSystemLog->LogFileWrite( "[INIT FAIL] GMS Acceptor Initialization failed\n" );
		goto label_end;
	}
	printf( " - Success\n" ) ;

	// UDP Recv packet work thread
	for( i = 0 ; i < _THREADNUMBER_UDP_PACKET_WORK_ ; i++ ) {
		ResetEvent( g_hInitEvent );
		g_pSystemLog->LogFileWrite( "[INIT] Create UDP th_UDPGMSPacketWorkThread process Thread n = 1 \n" ) ;
		hAcceptThread = BEGINTHREADEX( NULL, 0, th_UDPGMSPacketWorkThread, NULL, 0, &dwAcceptThreadID ) ;
		if( hAcceptThread != INVALID_HANDLE_VALUE ) {	
			CloseHandle(hAcceptThread) ; 
			hAcceptThread = INVALID_HANDLE_VALUE ; 

			if( WAIT_TIMEOUT == WaitForSingleObject( g_hInitEvent, 5000 ) )
			{
				g_pSystemLog->LogFileWrite( "[INIT FAIL] UDP th_UDPGMSPacketWorkThread Not Initialized\n" ); 
				goto label_end;
			}
		}
		else { 
			g_pSystemLog->LogFileWrite( "[INIT FAIL] UDP th_UDPGMSPacketWorkThread thread failed\n" ); 
			goto label_end; 
		}		

	}
	printf( "[INIT] Create UDP th_UDPGMSPacketWorkThread process Thread n = %d \n", _THREADNUMBER_UDP_PACKET_WORK_ ) ;

	


	// UDP Login packet work thread
	iTempCount = _THREADNUMBER_UDP_PACKET_LOGIN_;
	if( en_service_mode_innertest == g_ServerInfo.get_service_mode() )
		iTempCount = 2;
	for( i = 0 ; i < iTempCount ; i++ ) {

		g_pSystemLog->LogFileWrite( "[INIT] Create UDP th_UDPLoginPacketWorkThread process Thread n = %d \n", iTempCount ) ;
		hAcceptThread = BEGINTHREADEX( NULL, 0, th_UDPLoginPacketWorkThread, NULL, 0, &dwAcceptThreadID ) ;
		if( hAcceptThread != INVALID_HANDLE_VALUE ) {	
			CloseHandle(hAcceptThread) ; 
			hAcceptThread = INVALID_HANDLE_VALUE ; 
		}
		else { 
			g_pSystemLog->LogFileWrite( "[INIF FAIL] UDP th_UDPLoginPacketWorkThread thread failed\n" ); 
			goto label_end ; 
		}		

	}
	printf( "[INIT] Create UDP th_UDPLoginPacketWorkThread process Thread n = %d \n", iTempCount ) ;

	

	// 4. 읽기 스레드 -IOCP 큐에 박힌 결과패킷을 처리
	printf( "[INIT] Create Recv Thread\n" );
	g_pSystemLog->LogFileWrite( "[INIT] Create Recv Thread\n" );
	
	hAcceptThread = BEGINTHREADEX( NULL, 0, th_Recvthread, NULL, 0, &dwAcceptThreadID ) ;
	if( hAcceptThread != INVALID_HANDLE_VALUE ) { 
		CloseHandle(hAcceptThread) ; 
		hAcceptThread = INVALID_HANDLE_VALUE ; 
	}
	else { 
		g_pSystemLog->LogFileWrite( "[INIT FAIL] Recv thread failed\n" ); 
		goto label_end ; 
	}

#ifdef ITEMSHOP_PATCH_200607
	// Nexon's ItemShop 패킷 프로시저 쓰레드 생성
	hWebShopPktProcInfo = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(hWebShopPktProcInfo == NULL) {
		g_pSystemLog->LogFileWrite( "[INIT FAIL] hWebShopPktProcInfo = CreateEvent() failed\n" ) ; 
		goto label_end ; 
	}
	
	printf("[INIT] Create WebShopPktProc \n") ;
	g_pSystemLog->LogFileWrite("[INIT] Create WebShopPktProc \n") ;
	{
		HANDLE hWebShopPktProc = INVALID_HANDLE_VALUE;
		for(int i = 0; i < 1; ++i) {
			hWebShopPktProc = (HANDLE)_beginthreadex(NULL, 0, (unsigned (__stdcall *)(void*))WebShopPktProc, NULL, 0, NULL);
			if(hWebShopPktProc != INVALID_HANDLE_VALUE) {
				CloseHandle(hWebShopPktProc);
				hWebShopPktProc = INVALID_HANDLE_VALUE;
			}
			else { 
				g_pSystemLog->LogFileWrite( "[INIT FAIL] WebShopPktProc failed\n" ); 
				goto label_end ; 
			}
		}
	}
#endif

	// Ping Thread 생성 
	printf( "[INIT] Create Ping Thread\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create Ping Thread\n" ) ;
	hAcceptThread = BEGINTHREADEX( NULL, 0, th_UDPPingThread, (LPVOID)(PING_PORT), 0, &dwAcceptThreadID ) ;
	if( hAcceptThread == INVALID_HANDLE_VALUE ) {
		g_pSystemLog->LogFileWrite( "[FAIL] Ping Thread\n" ) ;
		goto label_end ;
		// 에러처리 해줘라..
	}
	printf( "-> INFO : Ping Port = %u\n", PING_PORT ) ;
	g_pSystemLog->LogFileWrite( "-> INFO : Ping Port = %u\n", PING_PORT ) ;


	// ranking version read thread
	hAcceptThread = BEGINTHREADEX( NULL, 0, th_RankingVersionChecker, (LPVOID)0, 0, &dwAcceptThreadID ) ;
	if( hAcceptThread == INVALID_HANDLE_VALUE ) {
		g_pSystemLog->LogFileWrite( "[FAIL] ranking version check Thread\n" ) ;
		goto label_end ;
		// 에러처리 해줘라..
	}


	hAcceptThread = BEGINTHREADEX( NULL, 0, th_ConnectCountRecorderThread, NULL, 0, &dwAcceptThreadID ) ;
	if( hAcceptThread == INVALID_HANDLE_VALUE ) {	
		g_pSystemLog->LogFileWrite( "[FAIL] th_ConnectCountRecorderThread \n" ) ;
		goto label_end ;
	}
	

	
	// GS 의 접속을 기다린다.
	// Accept Thread 생성 
	printf( "[INIT] Create User Accept Thread\n" ) ;
	g_pSystemLog->LogFileWrite( "[INIT] Create User Accept Thread\n" ) ;
	hAcceptThread = BEGINTHREADEX( NULL, 0, th_AcceptThread, (LPVOID)(g_ServerInfo.GetDSAcceptPort()), 0, &dwAcceptThreadID ) ;
	if( hAcceptThread == INVALID_HANDLE_VALUE ) {
		g_pSystemLog->LogFileWrite( "[FAIL] Accept Thread\n" ) ;
		goto label_end ;
		// 에러처리 해줘라..
	}
	printf( "-> INFO : Accept Port = %u\n", g_ServerInfo.GetDSAcceptPort() ) ;
	printf( "------------------------------------------------------------------------\n");

	g_pSystemLog->LogFileWrite( "-> INFO : Accept Port = %u\n", g_ServerInfo.GetDSAcceptPort() ) ;
	g_pSystemLog->LogFileWrite( "------------------------------------------------------------------------\n");


#ifdef _LOGIN_STRESS_TEST_	
	hAcceptThread = BEGINTHREADEX( NULL, 0, th_loginTest, NULL, 0, &dwAcceptThreadID ) ;
	if( hAcceptThread == INVALID_HANDLE_VALUE )
	{
		goto label_end ;
	}
#endif

	// 더이상 쓸일이 없는 핸들들이니..닫는다. 
	CloseHandle(hAcceptThread) ;
	CloseHandle( g_hInitEvent ) ;


	// console 을 위하여...
	hIn = GetStdHandle( STD_INPUT_HANDLE ) ;
	hOut1 = GetStdHandle( STD_OUTPUT_HANDLE ) ;

	SetConsoleCtrlHandler( (PHANDLER_ROUTINE) KeyboardHandler, TRUE ) ;

	hOut2 = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, 0 ) ;


	wsprintf( cmdbuf, "\n[cmd] " ) ;
	WriteConsole( hOut1, cmdbuf, (DWORD)strlen(cmdbuf), &dwTemp, NULL ) ;
	
	//////////////////////////////////////////////////////////////////////////
	// Output 창.

	wsprintf( cmdbuf, " *** Output Screen*** \nPress ESC to go cmd console\n" ) ;
	WriteConsole( hOut2, cmdbuf, (DWORD)strlen(cmdbuf), &dwTemp, NULL ) ;

#ifdef _PD_GAME_EVENT_
	wsprintf( cmdbuf, "# DEF : _GAME_EVENT_\n" ) ;
	WriteConsole( hOut2, cmdbuf, (DWORD)strlen(cmdbuf), &dwTemp, NULL ) ;

	g_pSystemLog->LogFileWrite( "# DEF : _GAME_EVENT_\n" );
#endif

#ifdef _PD_MASTER_PUPIL_
	wsprintf( cmdbuf, "# DEF : _PD_MASTER_PUPIL_\n" ) ;
	WriteConsole( hOut2, cmdbuf, (DWORD)strlen(cmdbuf), &dwTemp, NULL ) ;

	g_pSystemLog->LogFileWrite( "# DEF : _PD_MASTER_PUPIL_\n" );
#endif
	
	//////////////////////////////////////////////////////////////////////////
	

	SetConsoleActiveScreenBuffer( hOut2 ) ;

	GetConsoleScreenBufferInfo( hOut1, &Info ) ;
	SetConsoleScreenBufferSize( hOut2, Info.dwSize ) ;

	PrintConsole("===================================================== \n") ;

	g_ServerInfo.ShowSettings(false);
	g_ServerInfo.ShowSettings(true);

	int iCnt = 0;
	while( g_MainThreading ) {
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

							PrintConsole( "====================================================\n" ) ;
						}
						else {
							cmdbuf[writepos] = 0 ;
							printf("\nCommand = %s\n", cmdbuf ) ;
							writepos = 0 ;

							ret = ProcessCommand( cmdbuf ) ;
							if( ret == 1 ) {
								if( MessageBox( NULL, "Shutdown Server Really?", "Confirm", MB_OKCANCEL ) == IDOK ) {
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
			continue;
		}
		Sleep(5);
	}

label_end:
	// 종료 처리한다. 
	printf( "See log file for detail information\n" );
	Sleep(12000) ;
	ShutDownServer() ;		// Class 객체 해제
	DestroyGlobalObject() ;	// Global Object 해제
#ifdef _METEL_LOGIN_200608_
	Logout(_T("Success to CloseBilling()"));
	CloseBilling();
#endif //_METEL_LOGIN_200608_
	return 0 ;
}

void ShutDownServer() 
{
	// 로그인 중인 계정의 DB 를 로그아웃 상태로 변경한다.
	int nReturn;
	
	nReturn = CharacDataBackup();
	if(nReturn == -1){
		printf("[INFO] Character DB backup failed. Retry file backup. \n");
		CharacFileBackup();
	}
	else{
		printf("[INFO] Character DB backup success! (%d) \n", nReturn);
	}

	g_Threading = false ;

	printf( "[INFO] Closing Thread... \n") ;
	for( int i = 10 ; i > 0  ; i--)
	{
		printf(" -> %d \n", i) ;
		Sleep( 1000 ) ;
	}

	try{
		try{
			g_pUser_Command->AllLogout(PWD_ALLLOGOUT);		
		}
		catch(...){
			::PrintConsole( "[exception] %s, %d \n ", __FILE__, __LINE__ ) ;
		}
	}
	catch (...) {
		printf("[EXCEPTION] g_pDB_Command->AllLogout() \n");
	}

	WSACleanup() ;

#ifdef _PD_GAME_EVENT_
	if( g_pGameEvent )
		delete g_pGameEvent;
	g_pGameEvent = NULL;
#endif
}



int ProcessCommand( char * cmd )
{
	static char command[128] = {0} ;
	static char arg1[128] = {0} ;
	static char arg2[128] = {0} ;
	static char arg3[128] = {0} ;

	int nString = 0 ;
	int nCount = 0;

	memset( command, 0, 128 ) ;
	memset( arg1, 0, 128 ) ;
	memset( arg2, 0, 128 ) ;
	memset( arg3, 0, 128 ) ;


	// Command string parsing 
	nString = sscanf( cmd, "%s %s %s %s", command, arg1, arg2, arg3 ) ;
	if( nString > 0 ) {		// Max 3
		switch( command[0] ) {	
		case '/':
			printf("[INFO] System duration queue report \n") ;
			printf("   ->TCP Work packet queue / Duration : %u, WorkThread : %d \n", g_pPacketQ->GetCurCount(), _THREADNUMBER_PACKET_PROCESS_ ) ;
			printf("   ->TCP Update packet queue / Duration : %u, WorkThread : %d \n\n", g_pUpdatePacketQ->GetCurCount(), _THREADNUMBER_UPDATE_PROCESS_ ) ;
			printf("   ->UDP send work packet queue / Duration : %u, WorkThread : %d \n", g_pUDPSendPacketQ->GetCurCount(), _THREADNUMBER_UDP_PACKET_SEND_ ) ;
			printf("   ->UDP work packet queue / Duration : %u, WorkThread : %d \n", g_pUDPRecvPacketQ->GetCurCount(), _THREADNUMBER_UDP_PACKET_WORK_ ) ;
			printf("   ->UDP login packet queue / Duration : %u, WorkThread : %d \n\n", g_pUDPLoinPacketQ->GetCurCount(), _THREADNUMBER_UDP_PACKET_LOGIN_ ) ;			
			printf("   ->CHARAC_BACKUP ready queue / Charac ready : %u, WorkThread : %d \n", g_pBackupReady->GetCharacReadySize(), _THREADNUMBER_CHARAC_BACKUP_ ) ;			
			printf("   ->ACC_BACKUP ready queue / Acc Ready : %u, WorkThread : %d \n\n", g_pBackupReady->GetUserReadySize(), _THREADNUMBER_USER_BACKUP_ ) ;			


			printf("\n!! COUNT !!\n") ;
			printf( "  ->CurUserMapCount : %d \n", g_pAccMapManager->GetUserMapSize() ) ;
			printf( "  ->CurCharacMapCount : %d \n", g_pAccMapManager->GetCharacdMapSize() ) ;
			printf( "  ->CurPlayerCount : %d \n", g_pServerInfo->GetForestUserCount() ) ;

			g_pSystemLog->Reflush() ;
			g_pGMStLog->Reflush() ;

			break;

		case '*' :
			{
				CMSG_SHOWTEXT_REQ msgShowText = {0} ;
				msgShowText.cMessage = CMSG_SHOWTEXT_REQ_DS_GS ;
				msgShowText.usLength = sizeof(CMSG_SHOWTEXT_REQ) ;
				strcpy(msgShowText.cString, arg1) ;

				printf( "[MSG] %s \n", arg1 ) ;

				g_pDSComQueue->GlobalSend( msgShowText.usLength, (char*)&msgShowText ) ;
			}
			break ;


		case 'a':
			if( strcmp( command, "alllogout" ) == 0 ) 
			{
				try{
					if(g_pUser_Command->AllLogout(PWD_ALLLOGOUT)){					
						printf("[INFO] Alllogout \n");
						::PrintConsole("[INFO] Alllogout \n");
					}
				}
				catch(...)
				{
				}
			}
			break;

		case 'b':
			if( strcmp( command, "backup") == 0){
				if(strcmp( arg1, "db") == 0)
				{					
					if( strcmp( arg2, "-sync") == 0 )
					{
						CharacDataBackup(_BACKUP_SYNC_OPTION);
					}
					else if( strcmp( arg2, "-unsync") == 0 )
					{
						CharacDataBackup(_BACKUP_UNSYNC_OPTION);
					}
					else
					{
						printf( "==========================[worning]========================== \n ") ;
						printf( "charac data backup commend is need option ('-sync' or '-unsync'). \n ") ;
						printf( "option '-sync' is more safely then '-unsync' if system haven't any dead-lock\n ") ;
						printf( "but if system have some dead locking, rather then to use option '-unsync' . \n ") ;
						printf( "============================================================== \n ") ;
					}
					
				}
				else if(strcmp( arg1, "file") == 0){
					nCount = CharacFileBackup();
					printf("[INFO] Complete Character FILE backup. (Count :%d) \n", nCount);
					::PrintConsole("[INFO] Complete Character FILE backup. (Count :%d) \n", nCount);
				}
			}
			break;

		case 'c':
#ifdef _PD_CASTLE_STEP_1_
			if( 0 == strcmp( command, "castle" ) )
			{
				if( nString >= 2 )
				{
					if( 0 == strcmp( arg1, "info" ) )
					{
						int castle_index = atoi( arg1 );
						CDSCastleManager::GetObj().print_castle_info( castle_index );
					}
					else if( 0 == strcmp( arg1, "resetstate" ) )
					{
						CDSCastleManager::GetObj().reset_state();
					}
					else if( 0 == strcmp( arg1, "resetmonster" ) )
					{
						CDSCastleManager::GetObj().cmd_change_owner_to_monster( atoi(arg2) );
					}
				}
				else
				{
					printf( "\n'castle' command neees more arguments.\n" );
				}
			}
#endif
#ifdef _CIS_20070326_
			if( 0 == strcmp( command, "cis" ) ) {
				if( strcmp( arg1, "/pkt0" ) == 0 ) {
					MSG_COMMON	req;
					strcpy(req.GS2DS_CIS_CASH_INVEN_REQ.szAcc, "ragbook2");
					req.GS2DS_CIS_CASH_INVEN_REQ.bIsPresent = true;
					req.GS2DS_CIS_CASH_INVEN_REQ.wPageNum = 2;
					req.GS2DS_CIS_CASH_INVEN_REQ.wNumPerPage = 2;
					CIS_ShowCashInvenReq(&req, g_pCIS, NULL);
				}
				else if( strcmp( arg1, "/pkt1" ) == 0 ) {
					MSG_COMMON	req;
					strcpy(req.GS2DS_CIS_PRESENT_MSG_REQ.szTo, "ragbook2");
					req.GS2DS_CIS_PRESENT_MSG_REQ.iPkgGuid = 6;
					CIS_PresentMsgReq(&req, g_pCIS, NULL);
				}
			}
#endif
			break;

		case 'd':
			if( strcmp( command, "db") == 0 )
			{
				if( strcmp( arg1, "reconnect" ) == 0 )
				{
					if( strcmp( arg2, "-game") == 0 )
					{
						g_pDBCon->CallReConTh() ; 
					}
					else if( strcmp( arg2, "-acc" ) == 0 )
					{
						g_pUSERDBCon->CallReConTh() ;
					}
					else
					{
						printf( "==========================[worning]========================== \n ") ;
						printf( "DB Reconnect command is need option ( one choice :  -game, -acc ) \n" ) ;
						printf( "============================================================== \n ") ;
					}
				}
			}
			else if( strcmp( command, "dscom" ) == 0 )
			{
				if( strcmp( arg1, "testpacket" ) == 0 )
				{
					CMSG_CHECK_DSGS_PACKET check ;
					check.usLength = sizeof(check) ;
					check.cMessage = CMSG_NO_CHECK_DSGS_PACKET ;
					check.ucMode = en_check_packet_test_request ;		// request.

					if( g_pPacketChecker )
					{
						g_pPacketChecker->fill_packet( &check ) ;

						_h_DS_Com * pCom = g_pServerInfo->GetFirstCom() ;
						if( pCom )
						{
							pCom->Send( check.usLength, (char*)&check ) ;	
							printf( "\n Send Testpacket\n" );
						}
						else
							printf( "\n No Game Server Connections\n" ) ;
					}
				}
				else if( 0 == strcmp( arg1, "testgs" ) )
				{
					g_ServerInfo.SendTimeoutCheck();

					printf( "\n Send Complete, Please wait...(10 sec)\n" );

					Sleep( 10000U );

					g_ServerInfo.ShowTimeoutCheckResult();
				}
				else if( 0 == strcmp( arg1, "showstate" ) )
				{
					int i = atoi(arg2);
					_h_DS_Com * pDSComm =  g_ServerInfo.GetDSCom( static_cast<short>(i) );
					if( pDSComm )
					{
						pDSComm->ShowState( i );
					}
					else
					{
						printf( "\n Not found ds com %d\n", i );
					}
				}
			}
			break ;

		case 'f':
			if( strcmp( command, "find" ) == 0 )
			{
				PUSER user = g_pAccMapManager->Find_User( arg1 );
				if( user )
				{
					printf( "\nAccount <%s> : State = %d\n", arg1, user->cState );
				}
				else
				{
					printf( "\nAccount <%s> not found\n", arg1 );
				}
			}
			break;

		case 'g':
			{
				if( strcmp( command, "get" ) == 0 ) {

		#ifdef _VINA_SHUTDOWN_20061023_
					if( strcmp( arg1, "shutdown" ) == 0 ) {
						//g_ShutdownStep에 따라서 현재 상태 출력
						switch(g_ShutdownStep)
						{
						case CONSOLECMD_DECREASE_ACQUISITION_POINT_OF_JIN:		// 경험치 포인트만 패널티 적용
							printf("current status: step 0(penalty 0% after 5 hours playing)\n");
							break;
						case CONSOLECMD_DECREASE_ALL_ACQUISITION_POINT:			// 모든 포인트에 대해 패널티 적용
							break;
						case CONSOLECMD_FORCED_LOGOFF:										// 유저 강제 종료
							printf("current status: step 2(block the user after 5 hours playing)\n");
							break;						
						}
					}
		#endif

	#ifdef _PD_CASTLE_STEP_1_
					if(strcmp(arg1, "defendcastle") == 0)
					{
						g_pDB_Command->ShowDefendCastleInfo();
					}
					else if(strcmp(arg1, "attackcastle") == 0)
					{
						g_pDB_Command->ShowAttackCastleInfo();
					}
					else if(strcmp(arg1, "battle") == 0)
					{
						g_pDB_Command->GetManorCombatInfo( NULL );
					}
					else if(strcmp(arg1, "defenddespot") == 0)
					{
						g_pDB_Command->GetDefendDespotInfo( NULL );
					}
	#endif
	#ifdef CHARAC_CREATE_DATE_20070309
					if(strcmp(arg1, "get_charac_createdate") == 0)
					{
						if(strcmp( arg2, "") !=0 ){
						
							PCHARAC	pCharac;
							
							pCharac = g_pAccMapManager->Find_Charac(arg2);
							

							if(pCharac == NULL){
								printf("Not find! \n");
								break;
							}

							printf("CharacCreateDate : 20%02d - %02d - %02d \n", \
								pCharac->CHARAC_BASIC.CharacCreateDate.sYear, \
								pCharac->CHARAC_BASIC.CharacCreateDate.sYear, \
								pCharac->CHARAC_BASIC.CharacCreateDate.sYear);
						}
					}
	#endif
				}
			}
			break;

		case  'h':
			if(strcmp( command, "help") == 0)
			{
				if( strcmp( arg1, "shutdown" ) == 0 ) 
				{
#ifdef _VINA_SHUTDOWN_20061023_
					printf("\n\
						shutdown steps ------------------------------\n\
						step 0: penalty 0% after 5 hours playing\n\
						step 1: reserved\n\
						step 2: block the user after 5 hours playing\n\
						---------------------------------------------\n\
						shutdown function ---------------------------\n\
						help shutdown					: show this\n\
						get shutdown						: show current status\n\
						set shutdown testmode on	: \n\
						set shutdown testmode off	: \n\
						---------------------------------------------\n");
#endif
				}
				else
				{
					FILE *	stream;

					char cFile[64] = {0};
					char buf[1024] = {0};	

					sprintf(cFile,"Env\\help.txt");
					stream = fopen(cFile,"r");

					fread(buf, 1, sizeof(buf), stream);				

					fclose(stream);

					printf(buf);
				}
			}
			break;

		case 'i':
			if( strcmp( command, "invenget" ) == 0 )
			{
				int unique = atoi( arg1 );
				_C_CHARAC temp;
				if( g_pDB_Command->inven_get( unique, &temp ) )
					printf( "\n unique %d => Money %u\n", unique, temp.CHARAC_INVENTORY.iMoney );
				else
					printf("\n Fail\n" );
			}
			else if( strcmp( command, "invegupdate" ) == 0 )
			{
				int unique = atoi( arg1 );
				_C_CHARAC temp;
				if( g_pDB_Command->inven_get( unique, &temp ) )
				{
					temp.CHARAC_INVENTORY.iMoney = static_cast<u_int>(atoi(arg2));
					printf( "\n unique %d => Up Money %u\n", unique, temp.CHARAC_INVENTORY.iMoney );
					g_pDB_Command->inven_update( unique, &temp );
				}
				else
					printf("\n Fail\n" );
				
			}
			break;

		case 'l' :
			{
			
				if( strcmp( command, "login") == 0 ) 
				{
					UMSG_LOGIN_LS_DS msglogin = {0} ;
					msglogin.cMessage = UMSG_LOGIN_LS_DS_NUM ;
					msglogin.iKey = PASS_KEY ;
					msglogin.cServerNum = 0 ;
					sprintf( msglogin.cAccount, arg1 ) ;
					msglogin.iTempUniqueID = 1 ;


					SOCKADDR_IN addr ;
					g_pUDPLoinPacketQ->insert_udp( &addr, (char*)&msglogin, sizeof(msglogin) ) ;
				}
				

			}
			break;

#ifdef _PD_MASTER_PUPIL_
		case 'm':
			{
				if( 0 == strcmp( command, "mp" ) )
				{
					if( 0 == strcmp( arg1, "board" ) )
					{
						if( 0 == strcmp( arg2, "detail" ) )
						{
							g_pMPSet->PrintBoard();
						}
						else if( 0 == strcmp( arg2, "simple" ) )
						{
							g_pMPSet->PrintSimpleInfo();
						}
					}
					else if( 0 == strcmp( arg1, "master" ) )
					{
						g_pMPSet->ShowMaster(atoi( arg2 ));
					}
					
				}
			}
			break;
#endif

		case 'p':
			{
				if( strcmp( command, "pcroom" ) == 0 )
				{
#ifdef _PD_BUFF_COUNT_MODIFY_
					printf( "\n PC Room buff added\n" );
#else
					printf( "\n PC Room buff not added\n" );
#endif
				}
			}
			break;


		case 'q':
			if( strcmp( command, "quit" ) == 0 ) {			

				g_pSystemLog->Reflush() ;
				g_pGMStLog->Reflush() ;

				bool bGSIsRun = false ;
				for(int i=0; i < GS_MAX; i++){
					if(g_ServerInfo.m_ServerStateInfo[i].cState != 0){
						printf("\n[WARNING] Please Shutdown Game Server %d first\n", i);
						::PrintConsole("\n[WARNING] Please Shutdown Game Server %d first\n", i);
						bGSIsRun = true ;
					}
				}

				if( strcmp( arg1, "-force") == 0  )
				{
					return 1 ;
				}
				else
				{
					if( bGSIsRun == false )
					{
						return 1 ;
					}
				}
			}			
			break;
		
		case 'r':
#ifdef _PD_GAME_EVENT_
			if( 0 == strcmp( command, "reload" ) )
			{
				if( 0 == strcmp( arg1, "freematch" ) )
				{
					g_pGameEvent->ResetEventManager();
				}
			}
#endif
			break;
		
		
		case 's':
			// sql connection script 를 암호화 파일로 변환한다.
			if( strcmp( command, "show")  == 0){
				if(strcmp( arg1, "accountlist") == 0){
					ShowAccountList();                    					
				}
				else if(strcmp( arg1, "accountinfo") == 0){
					if(strcmp( arg2, "") !=0 ){
						ShowAccountInfo(arg2);
					}
				}

				else if(strcmp( arg1, "characlist") == 0){
					ShowCharacList();
				}

				else if(strcmp( arg1, "characinfo") == 0){
					if(strcmp( arg2, "") !=0 ){
						ShowCharacInfo(arg2);
					}
				}

				else if(strcmp( arg1, "skill") == 0){
					if(strcmp( arg2, "") !=0 ){
						ShowSkill(arg2);
					}
				}

				else if(strcmp( arg1, "serverlist" ) == 0){
					ShowServerList();				
				}

				else if( strcmp( arg1, "maxcon") == 0 )
				{
					printf("[INFO] max connection : %d, cur connection : %d \n", g_MaxConnection, g_ServerInfo.GetForestUserCount() ) ;
				}

				else if( strcmp (arg1, "system") == 0 )
				{
					printf("COMPILE_DATE : %s \n",_COMPILE_DATE_ ) ;
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
				else if( strcmp( arg1, "badinven") == 0 )
				{					
					try{
						g_pDB_Command->SelectInvenInfo( 0 ) ;
					}
					catch(...){
						::PrintConsole( "[exception] %s, %d \n ", __FILE__, __LINE__ ) ;
					}
				}
				else if( strcmp( arg1, "baddepot") == 0 )
				{					
					try{
						g_pDB_Command->SelectDepotInfo( 0 ) ;
					}
					catch(...){
						::PrintConsole( "[exception] %s, %d \n ", __FILE__, __LINE__ ) ;
					}
				}
				else if( strcmp( arg1, "rank" ) == 0 )
				{
					CRankInfo * pRank = g_pRankingManager->GetRankInfoPtr( _ranking::_RANK_TYPE_LEVEL ) ;
					if( pRank )
					{
						_Rank	rank[9] ;
						pRank->GetRankInfo( rank, 9 ) ;
						printf( "[[ LEVEL RANKING ]]\n" ) ;

						for( int i = 0 ; i < 9 ; ++i )
						{
							printf( "- %d : %s \n", rank[i].sRanking+1, rank[i].cCharacName ) ;
						}
					}
				}
				else if( strcmp (arg1, "lock" ) == 0 )
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
				else if( strcmp( arg1, "logserver" ) == 0 )
				{
					if( INVALID_SOCKET == g_pLogSvrCom->GetSocket() )
						printf( "\nLog Server Not Connected\n" ) ;
					else
						printf( "\nLog Server Connected\n" ) ;
				}
				else if( strcmp( arg1, "setting" ) == 0 )
				{
					printf( "\n");
					g_ServerInfo.ShowSettings(false);
				}
				else if( strcmp( arg1, "org" ) == 0 )
				{
					g_pOrganizationManager->ShowAllOrganization();
				}
#ifdef _PD_ALLY_ORG_STEP_1_
				else if( strcmp( arg1, "ally" ) == 0 )
				{
					g_pAllyManager->show_all_ally();
				}
#endif
				else if( 0 == strcmp( arg1, "gms" ) )
				{
					g_pGMSAcceptor->PrintState();
				}
#ifdef _PD_CASTLE_STEP_1_
				else if( 0 == strcmp( arg1, "castles" ) )
				{
					CDSCastleManager::GetObj().print_castle_list();
				}
#endif
#ifdef _PD_GAME_EVENT_
				else if( 0 == strcmp( arg1, "freematch" ) )
				{
					g_pGameEvent->PrintCurEvent();
				}
				else if( 0 == strcmp( arg1, "fmreward" ) )
				{
					g_pGameEvent->PrintReward();
				}
#endif

			}
			else if( strcmp( command, "sql") == 0 )
			{
				CSqlConnectorString	sqlConvert(CRYPTO_KEY_FILE) ;
				bool bRes = false ;
				if( strcmp( arg3, "-d" ) == 0 )	// 복호화 한다
				{
					bRes = sqlConvert.ConvertScript( arg1, arg2, false ) ;
				}
				else if( strcmp( arg3, "-e" ) == 0 )	// 암호화 한다.
				{
					bRes = sqlConvert.ConvertScript( arg1, arg2, true ) ;
				}

				if( bRes == true )
				{
					printf( "-Converting successed \n") ;
				}
				else
				{
					printf( "-Converting failed \n") ;
				}
			}
			else if( strcmp( arg1, "partyqueue") == 0 )
			{
				printf( "[INFO] Party queue size (%u) \n",g_pPartyQueue->GetSize() ) ;
			}
			else if(strcmp(command, "set") == 0){
				if(strcmp( arg1, "chrbackuptime") == 0){
					if( (5000 < atoi(arg2)) && (600000 > atoi(arg2)) ){
						g_iCharacBackupTime = atoi(arg2);
						printf("[INFO] Set Character Backup Period (SET : %d ms)",g_iCharacBackupTime);
					}					
				}

				else if( strcmp( arg1, "baddepot") == 0 )
				{					
					g_pDB_Command->SelectDepotInfo( 1 ) ;				
				}

				else if( strcmp( arg1, "badinven") == 0 )
				{					
					g_pDB_Command->SelectInvenInfo( 1 ) ;				
				}
#ifdef _RESET_LEVEL
				else if( strcmp( arg1, "levelreset") == 0 )
				{					
					try{
						char cFirst = 0 ;
						if( strcmp(arg2, "-first") == 0 )
						{
							cFirst = 1 ;
						}
						g_pDB_Command->ResetLevelInfo( cFirst) ;
					}
					catch(...){
						::PrintConsole( "[exception] %s, %d \n ", __FILE__, __LINE__ ) ;
					}
				}
#endif
				else if(strcmp( arg1, "accbackuptime") == 0){
					if( (5000 < atoi(arg2)) && (600000 > atoi(arg2)) ){
						g_iAccBackupTime = atoi(arg2);
						printf("[INFO] Set Account Backup Period (SET : %d ms)",g_iAccBackupTime);
					}					
				}
				else if(strcmp( arg1, "maxcon") == 0 )
				{
					if( _PERMISSIBLE_MAX_USER > atoi(arg2) )
					{
						g_MaxConnection = atoi( arg2 ) ;
						printf("[INFO] Set maxconnection : %d \n", g_MaxConnection ) ;
					}
					else
					{
						printf("[INFO] Overrun system max(%d) \n", _PERMISSIBLE_MAX_USER) ;
					}
					
				}
	#ifdef _PD_CASTLE_STEP_1_
					if(strcmp(arg1, "defendcastle") == 0)
					{
						//g_pDB_Command->UpdateDefendCastleInfo();
					}
					else if(strcmp(arg1, "battle") == 0)
					{
						g_pDB_Command->SetManorCombatInfo();
					}
	#endif
				else if( strcmp( arg1, "trace") == 0 )
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
				else if( strcmp( arg1, "capture" ) == 0 )
				{
					g_bConsoleCapture = atoi(arg3) > 0 ? true : false  ;
				}
				else if( strcmp( arg1, "server") == 0 )
				{
					if( strcmp( arg2, "close") == 0 )
					{
						g_bCloseServer = true ;
						printf(" -> set server state : close \n") ;
					}
					else if( strcmp( arg2, "open") == 0 )
					{
						g_bCloseServer = false ;
						printf(" -> set server state : open \n") ;
					}
					else
					{
						if( g_bCloseServer )
							printf( " -> current server state = close\n" ) ;
						else 
							printf( " -> current server state = open\n" ) ;
					}
				}
#ifdef _VINA_SHUTDOWN_20061023_
				else if( strcmp( arg1, "shutdown" ) == 0 ) 
				{
					CONSOLE_COMMAND Command = CONSOLECMD_TESTMODE_OFF;

					/*
					if( strcmp( arg2, "s0" ) == 0 ) g_ShutdownStep = CONSOLECMD_DECREASE_ACQUISITION_POINT_OF_JIN;
					else if( strcmp( arg2, "s1" ) == 0 ) ;
					else if( strcmp( arg2, "s2" ) == 0 ) g_ShutdownStep = CONSOLECMD_FORCED_LOGOFF;
					else*/ if( strcmp( arg2, "testmode" ) == 0 ) {
						if( strcmp( arg3, "on" ) == 0 ) {
							Command = CONSOLECMD_TESTMODE_ON;
						}else if(strcmp( arg3, "off" ) == 0 ) {
							Command = CONSOLECMD_TESTMODE_OFF;
						}
					}
					
					//모든 GS에 셧다운 설정 변경 패킷 전송;	// CMSG_NO_CONSOLE_COMMAND
					CMSG_CONSOLE_COMMAND pkt;
					pkt.usLength = sizeof(CMSG_CONSOLE_COMMAND);
					pkt.ucMessage = CMSG_NO_CONSOLE_COMMAND;
					pkt.ucCommand = (UCHAR)g_ShutdownStep;
					pkt.iUnion = (int)Command;
		
					//출력
					/*switch(g_ShutdownStep)
					{
					case CONSOLECMD_DECREASE_ACQUISITION_POINT_OF_JIN:		// 경험치 포인트만 패널티 적용
						printf("current shutdown status: step 0(penalty 0% after 5 hours playing)\n");
						break;
					case CONSOLECMD_DECREASE_ALL_ACQUISITION_POINT:			// 모든 포인트에 대해 패널티 적용
						break;
					case CONSOLECMD_FORCED_LOGOFF:										// 유저 강제 종료
						printf("current shutdown status: step 2(block the user after 5 hours playing)\n");
						break;
					}*/
					
					switch(Command)
					{
					case CONSOLECMD_TESTMODE_ON:
						printf("current shutdown status: TESTMODE_ON\n");
						break;
					case CONSOLECMD_TESTMODE_OFF:
						printf("current shutdown status: TESTMODE_OFF\n");
						break;
					}

					_h_DS_Com * pDSComm = NULL ;	
					for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) 
					{
						pDSComm = g_pServerInfo->GetDSCom(i) ;
						if(pDSComm)
						{
							if(pDSComm->Send(pkt.usLength, (char*)&pkt) == false)
							{
								::PrintConsole( "[SO_ERR(%d:%s)] pDSComm->Send(CMSG_NO_CONSOLE_COMMAND) \n", __LINE__, __FILE__) ;
							}
						}
					}

				}
#endif
			}
			
			break;

		case 't':
			if( strcmp( command, "test" ) == 0 )
			{
				if( 0 == strcmp( arg1, "cashitem" ) )
				{
					if( 0 == strcmp( arg2, "on" ) )
					{
						g_bCashItemtest = true;

						CMSG_ENV_SETTING pkEnv( en_env_setting_cashitem_period_test );
						pkEnv.iValue = 1;		// on

						printf( "\n  Cash item test mode on!\n" );

						_h_DS_Com * pDSComm = NULL ;	
						for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) 
						{
							pDSComm = g_pServerInfo->GetDSCom(i) ;
							if(pDSComm)
								pDSComm->Send(pkEnv.usLength, (char*)&pkEnv);
						}
					}
					else if( 0 == strcmp( arg2, "off") )
					{
						g_bCashItemtest = false;

						CMSG_ENV_SETTING pkEnv( en_env_setting_cashitem_period_test );
						pkEnv.iValue = 0;		// on

						printf( "\n  Cash item test mode off!\n" );

						_h_DS_Com * pDSComm = NULL ;	
						for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) 
						{
							pDSComm = g_pServerInfo->GetDSCom(i) ;
							if(pDSComm)
								pDSComm->Send(pkEnv.usLength, (char*)&pkEnv);
						}
					}
					else
					{
						if( g_bCashItemtest )
							printf( "\n  Cur : Cash Item Test mode on.\n" );
						else
							printf( "\n  Cur : Cash Item Test mode off.\n" );
					}
				
				}
#ifdef	_NXPCB_AWARD_200703_
				else if( strcmp( arg1, "nxpcblogin" ) == 0 ) {
					if( strcmp( arg2, "0" ) == 0 ) {
						// 정액제 아이피
						NEXONPCB_LoginReq("yoon1106", "", StringIP2NumericIP("221.147.34.184"));
					}
					else if( strcmp( arg2, "1" ) == 0 ) {
						// 미가맹 피씨방 아이피
						NEXONPCB_LoginReq("yoon1106", "", StringIP2NumericIP("221.147.34.243"));
					}
					else if( strcmp( arg2, "2" ) == 0 ) {
						// 정량제 피씨방 아이피
						NEXONPCB_LoginReq("yoon1106", "", StringIP2NumericIP("221.147.34.153"));
					}
					else {
						NEXONPCB_LoginReq("yoon1106", "", StringIP2NumericIP(arg2));
					}
				}
				else if( strcmp( arg1, "nxpcblogout" ) == 0 ) {
					if( strcmp( arg2, "0" ) == 0 ) {
						// 정액제 아이피
						NEXONPCB_LogoutNtf("yoon1106", "", StringIP2NumericIP("221.147.34.184"));
					}
					else if( strcmp( arg2, "1" ) == 0 ) {
						// 미가맹 피씨방 아이피
						NEXONPCB_LogoutNtf("yoon1106", "", StringIP2NumericIP("221.147.34.243"));
					}
					else if( strcmp( arg2, "2" ) == 0 ) {
						// 정량제 피씨방 아이피
						NEXONPCB_LogoutNtf("yoon1106", "", StringIP2NumericIP("221.147.34.153"));
					}
					else {
						NEXONPCB_LogoutNtf("yoon1106", "", StringIP2NumericIP(arg2));
					}
				}
				else if( strcmp( arg1, "nxpcbrecon" ) == 0 )
				{
					CSmallNet::GetInstance()->_Disconnect();
				}
#endif
			}
			break ;		
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


//== 전역 객체 지우기 ===============================================================================
void DestroyGlobalObject() 
{			
	
	// class 해제..	
	g_pSystemLog->LogFileClose() ;
	delete  g_pSystemLog;		// 에러메시지 출력기	
	g_pGMStLog->LogFileClose() ;
	delete g_pGMStLog ;
	delete  g_pNDExceptionReport ;
	delete	g_pIOCP;			// IOCP
	delete	g_pPacketQ;			// 패킷 큐	
	delete g_pUpdatePacketQ ;
	delete  g_pCSPacketQ ;
	delete g_pChatPacketQueue ;
	delete  g_pUDPSendPacketQ ;
	delete  g_pUDPRecvPacketQ ;
	delete  g_pUDPLoinPacketQ ;
	delete g_pAccMapManager;	// AccMap Manager class pointer..
	delete g_pDB_Command;
	delete g_pUser_Command ;
	g_pDBCon->release() ;
	delete g_pDBCon ;
	g_pUSERDBCon->release() ;
	delete g_pUSERDBCon ;	
	//g_pEventDBCon->release() ;
	//delete g_pEventDBCon ;

#ifdef ITEMSHOP_PATCH_200607
	if(g_pDBCon_WebShop != NULL) {
		g_pDBCon_WebShop->release();
		delete g_pDBCon_WebShop;
		g_pDBCon_WebShop = NULL;
	}
	if(g_pDBCmd_WebShop != NULL) {
		delete g_pDBCmd_WebShop;
		g_pDBCmd_WebShop = NULL;
	}
#endif

	delete g_pBackupReady ;
	delete g_pUniqueIDQueue ;	
	delete g_pDSComQueue ;
	delete g_pMemSaveTimer ;
	delete g_pLogSvrCom ;
	delete g_pTrace ;
	delete g_pPartyQueue ;
	delete g_pPartyManager ;
	delete g_pPartyTimer ;
	delete g_pMemberQueue ;

	delete g_pRankingManager ;

	delete g_pOrganizationManager ;

#ifdef _PD_CASTLE_STEP_2_
	CDSCastleManager::GetObj().GetRecord()->Destory();
#endif//_PD_CASTLE_STEP_2_


	// 전역 동기화 객체 해제
#ifdef _METEL_LOGIN_200608_
	DeleteCriticalSection(&g_csBillingUser);
#endif // _METEL_LOGIN_200608_

#ifdef	_NEXON_NETWORK_
	//CPktMgr::DestroyInstance();
	CSmallNet::DestroyInstance();
#endif
}


DWORD WINAPI th_AcceptThread( LPVOID lp )
{
	_h_DS_Com		*	pDSCom;
	DWORD dwFlags = 0;
	SOCKET sListen;
	
	SOCKET sGS ;
	unsigned short accport = (unsigned short)lp ;
	int         nRet;
	int         nZero = 0 ;
	SOCKADDR_IN sockAddr ;
	
			
	// listen socket create
	sListen = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED ) ;
	if( sListen == INVALID_SOCKET )
	{
		g_pSystemLog->LogFileWrite( "  -> Create listen socket failed(%d)\n", WSAGetLastError() );
		goto label_Accept_end ;
	}

	// addr...
	sockAddr.sin_family = AF_INET ;
	sockAddr.sin_port = htons( accport ) ;	
	//if( g_bWanCon )
	{
		sockAddr.sin_addr.s_addr = inet_addr( g_ServerInfo.GetGSAcceptIP() ) ;
		printf( "[info] outer tcp binding ip (%s) \n", g_ServerInfo.GetGSAcceptIP() ) ;
	}
	/*
	else
	{
		sockAddr.sin_addr.s_addr = inet_addr( GetMyIP(0)) ;
		printf( "[info] inner tcp binding ip (%s) \n" ,GetMyIP(0) ) ;
	}
	*/


	if( bind( sListen, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR_IN) ) == SOCKET_ERROR)
	{
		g_pSystemLog->LogFileWrite( "  -> Bind Fail : %d\n", WSAGetLastError() ); 
		goto label_Accept_end ;
	}

	if(listen(sListen, SOMAXCONN) != 0)		// 백로그 값을 올리는게 날지....
	{
		g_pSystemLog->LogFileWrite( "  -> Listen Fail : %d\n", WSAGetLastError() ); 
		goto label_Accept_end ;
	}

	// socket option settting...
	// accept 의 리턴 소켓은 리슨 소켓의 속성을 이어받는다. 그래서 리슨 소켓의 속성을 설정한다. 
	// 근데.. 진짜냐???
	nZero = 0 ;
	nRet = setsockopt(sListen, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero)) ;
	if (SOCKET_ERROR == nRet) 
	{ 
		g_pSystemLog->LogFileWrite( "  -> setsockopt(SO_SNDBUF) fail : %d\n", WSAGetLastError() );
		goto label_Accept_end ;
	}
	nZero = 0 ;
	nRet = setsockopt(sListen, SOL_SOCKET, SO_RCVBUF, (char *)&nZero, sizeof(nZero)) ;
	if (SOCKET_ERROR == nRet) 
	{
		g_pSystemLog->LogFileWrite( "  -> setsockopt(SO_RCVBUF) fail : %d\n", WSAGetLastError() );
		goto label_Accept_end ;
	}
	// socket option setting end...

	int nReturn = 0;
	SOCKADDR_IN gsAddr ;
	int size = sizeof( gsAddr ) ;
	while( g_Threading )
	{
		try {
			sGS = WSAAccept( sListen, (SOCKADDR*)&gsAddr, &size, NULL, 0 ) ;// AcceptCallBack, 0 ) ;
		}
		catch ( ... ) {
			::PrintConsole( "[EXCEPTION] in WSAAccept : code = %d\n", GetLastError() ) ;
			continue ;
		}
		if( sGS == INVALID_SOCKET ) {	continue ;	}
		else {	//// Success
			try{
				// 새 클라이언트가 들어왔으므로 새 오버랩 구조체를 생성

				// Send OV Create

				pDSCom = g_pDSComQueue->GetDSCom() ;
				if( !pDSCom ) 
				{
					::PrintConsole( "[EXCEPTION] pDSCom == NULL \n") ;
					continue ;
				}

				pDSCom->ConnectCom(sGS) ;

				// GS Address info
				pDSCom->m_RecvOV.m_gs_info.ipAddr = gsAddr.sin_addr.s_addr;
				pDSCom->m_RecvOV.m_gs_info.state = 1;

				// IOCP handle 과 OV구조체 연결...
				if(!g_pIOCP->AssociateSocket( &pDSCom->m_RecvOV )) {
					::PrintConsole( "[ERROR] DS Con : in WSAAccept : code = %d\n", GetLastError() ) ;
					closesocket(sGS);
					continue;
				}	

				// WSARecv 로 최초 작업 시작을 걸어준다..						
				if(!pDSCom->Recv( sGS )) {
					::PrintConsole("[ERROR] in WSAAccept Recv: code = %d, <%s>\n", GetLastError(), inet_ntoa( gsAddr.sin_addr ) ) ;
					closesocket(sGS);
					continue;
				}

#ifdef ITEMSHOP_PATCH_200607
				//////////////////////////////////////////////////////////////////////////
				// 2006.05.29 Molla, Nexon Web Shop
				for(int i = 0; i < NEXON_ITEM_SHOP_IP_CNT; i++) {
					if(ulIP_NexonItemShop[i] == pDSCom->m_RecvOV.m_gs_info.ipAddr) {
						if(FALSE == pDSCom->pWebShopPktQueue->InitThis()) {
							::PrintConsole("%s(%d), pDSCom->pWebShopPktQueue->InitThis() 실패 \n", __FILE__, __LINE__);
							g_pSystemLog->LogFileWrite("%s(%d), pDSCom->pWebShopPktQueue->InitThis() 실패 \n", __FILE__, __LINE__);
						}
						pDSCom->pWebShopPktQueue->ReInitThis();

						// send InitPkt
						int iPktLen = 22;
						int iOffset = 0; // pSendPkt->m_wsabuf.buf's offset

						CSendPkt* pSendPkt = new CSendPkt;
						pSendPkt->m_wsabuf.buf = new char[iPktLen];
						pSendPkt->m_wsabuf.len = iPktLen;
						pSendPkt->m_gs_info.ipAddr = pDSCom->m_RecvOV.m_gs_info.ipAddr;
						pSendPkt->m_bMode = false;

						pSendPkt->m_wsabuf.buf[iOffset++] = (char)0xAA;								// reserved char
						*((LPWORD)(pSendPkt->m_wsabuf.buf+iOffset)) = 19; iOffset += sizeof(WORD);	// content length
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x7e;									// content type

						pSendPkt->m_wsabuf.buf[iOffset++] = 0x1b;	// content
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x43;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x4f;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x4e;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x4e;

						pSendPkt->m_wsabuf.buf[iOffset++] = 0x45;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x43;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x54;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x45;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x44;

						pSendPkt->m_wsabuf.buf[iOffset++] = 0x20;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x53;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x45;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x52;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x56;

						pSendPkt->m_wsabuf.buf[iOffset++] = 0x45;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x52;
						pSendPkt->m_wsabuf.buf[iOffset++] = 0x00;

						if(0 != pSendPkt->SendPacket(pDSCom->m_RecvOV.m_socket)) {
							SAFE_DELETE_ARRAY(pSendPkt->m_wsabuf.buf);
							SAFE_DELETE(pSendPkt);
							break;
						}

						break;
					}
				}
				//////////////////////////////////////////////////////////////////////////
#endif // ITEMSHOP_PATCH_200607
			}
			catch (...) {
				::PrintConsole("[EXCEPTION] th_AcceptThread - 'OV Create' or  'g_pIOCP->AssociateSocket' \n");
				continue;
			}
			

			::PrintConsole("[INFO]Sever to Server Connect success! (IP:%s) \n", inet_ntoa(gsAddr.sin_addr));			
			
            
		} // else
	} // while

	closesocket( sListen ) ;

label_Accept_end:

	_endthreadex(0) ;

	::PrintConsole("DWORD WINAPI th_AcceptThread( LPVOID lp ) Exit \n") ;
	printf( "DWORD WINAPI th_AcceptThread( LPVOID lp ) Exit \n " ) ;
	return 0 ;
}


DWORD WINAPI th_Recvthread( LPVOID lp )
{
	DWORD CompKey, dwNumBytes ;
	int bReturn ;
	_h_Overlapped * pOverlapped ;
	_h_Recv_Overlapped * pRecv_ov ;
	_h_DS_Com *		pDSComm;
	int ParsingLength = 0 ;
	int packetsize = 0 ;	
	SOCKET	sRecv_sock;

	//DISCONNECT MESSAGE
	CMSG_DISCONNECT pkDisconnect ;
	pkDisconnect.sLength = sizeof( CMSG_DISCONNECT ) ;
	pkDisconnect.cMessage = CMSG_NO_DISCONNECT ;
#ifdef _PD_DSCOM_DETAIL_
	int count = 0;
#endif
		

	bool bParse = false ;
	char ip_addr[17] = {0};

#ifdef ITEMSHOP_PATCH_200607
	//////////////////////////////////////////////////////////////////////////
	// 2006.05.29 Molla, Nexon Web Shop
	CSendPkt* pSendPkt = NULL;
	WEB_SHOP_PKT_PROC_INFO* pInfo = NULL;
	CWebShopPktQueue::QUEUE_RESULT QRes;

	CS_INIT(csWebShopPktProcInfo);
	//////////////////////////////////////////////////////////////////////////
#endif

	goto label_Continue_Recvthread;
label_Continue_Recvthread:
	while( g_Threading ) {	
		try{
			bReturn = g_pIOCP->GetStatus( &CompKey, &dwNumBytes, (WSAOVERLAPPED**)&pOverlapped, INFINITE ) ;

			pRecv_ov = (_h_Recv_Overlapped*)pOverlapped ;
			sRecv_sock = pRecv_ov->m_socket;
		}
		catch (...) {
			::PrintConsole( "[EXCEPTION] g_pIOCP->GetStatus()\n" ) ;			
			continue;
		}

#ifdef ITEMSHOP_PATCH_200607
		//////////////////////////////////////////////////////////////////////////▼▼▼
		// 2006.05.29 Molla, Nexon Web Shop
		#define DISCONNECT_COM \
			pRecv_ov->m_gs_info.state = GS_NONSTATE;\
			_h_DS_Com * pDSComm  = pRecv_ov->m_pDSComm ;\
			if( pDSComm ) {\
				pDSComm->DisConnectCom() ;\
			}

		for(int i = 0; i < NEXON_ITEM_SHOP_IP_CNT; i++) {
			if(ulIP_NexonItemShop[i] == pRecv_ov->m_gs_info.ipAddr) {
				if(pRecv_ov->m_bMode == true) { // recv mode
					if(dwNumBytes == 0) {
						DISCONNECT_COM;
						goto label_Continue_Recvthread;
					}

					//////////////////////////////////////////////////////////////////////////
					// 이 기능을 사용하지 않기 위해서 삽입한 코드
					goto label_Continue_Recvthread;
					//////////////////////////////////////////////////////////////////////////

					//::PrintConsole("%s dwNumBytes=%d \n", pRecv_ov->m_pBuf, dwNumBytes);

					QRes = pRecv_ov->m_pDSComm->pWebShopPktQueue->Push(pRecv_ov->m_pBuf, dwNumBytes);
					if(QRes != CWebShopPktQueue::QRES_OK) {
						if(QRes == CWebShopPktQueue::QRES_NOT_INITIALIZED) {
							::PrintConsole("큐 초기화 안했네.. -ㄴ-;; \n");
							g_pSystemLog->LogFileWrite("큐 초기화 안했네.. -ㄴ-;; \n");
							goto label_Continue_Recvthread;
						}
						else if(QRes == CWebShopPktQueue::QRES_OVERFLOW) {
							::PrintConsole("큐 오버플로.. -ㄴ-;; \n");
							g_pSystemLog->LogFileWrite("큐 오버플로.. -ㄴ-;; \n");
							goto label_Continue_Recvthread;
						}
						else if(QRes == CWebShopPktQueue::QRES_IMPROPER_PARAM) {
							::PrintConsole("CWebShopPktQueue::QRES_IMPROPER_PARAM, 이런 경우도 있는겨? -ㄴ-;; \n");
							g_pSystemLog->LogFileWrite("CWebShopPktQueue::QRES_IMPROPER_PARAM, 이런 경우도 있는겨? -ㄴ-;; \n");
							goto label_Continue_Recvthread;
						}
					}

					pInfo = new WEB_SHOP_PKT_PROC_INFO;
					pInfo->pClient = pRecv_ov->m_pDSComm;

					CS_LOCK(csWebShopPktProcInfo);
					listWebShopPktProcInfo.push_back(pInfo);
					CS_UNLOCK(csWebShopPktProcInfo);
					SetEvent(hWebShopPktProcInfo);

					pRecv_ov->RemainBytes = 0;
					if(!pRecv_ov->RecvPacket( sRecv_sock )) {
						DISCONNECT_COM;
						goto label_Continue_Recvthread;
					}
					goto label_Continue_Recvthread;
				}
				else { // send mode
					pSendPkt = (CSendPkt*)pOverlapped;

					SAFE_DELETE_ARRAY(pSendPkt->m_wsabuf.buf);
					SAFE_DELETE(pSendPkt);

					goto label_Continue_Recvthread;
				}
				
				break;
			}
		} // end of for(int i = 0; i < NEXON_ITEM_SHOP_IP_CNT; i++) {

		#undef DISCONNECT_COM
		//////////////////////////////////////////////////////////////////////////▲▲▲
#endif // ITEMSHOP_PATCH_200607

		if( bReturn ) {		// Operation Success
			if( pOverlapped->m_bMode ) {	// Is recv mode	..				
				if( dwNumBytes > 0 ) {
					try{	//parsing			
#ifdef _PD_DSCOM_DETAIL_
						PrintConsole( "[Parsing(%d) : %d]\n", ++count, dwNumBytes );
#endif
						bParse = pRecv_ov->Parsing(dwNumBytes);												

						//pRecv_ov->reset();

						if( bParse == false ) {	//parsing fail
							//on_state() ;							
							::PrintConsole( "  ### put Disconnect (Parsing Error) : %u\n", pRecv_ov->m_socket ) ;						
							memcpy(pRecv_ov->m_pBuf, (char*)&pkDisconnect, sizeof(pkDisconnect));
							g_pPacketQ->insert( pRecv_ov, sizeof(pkDisconnect) ) ;							
							//off_state() ;						
							continue ;
						}

						if(!pRecv_ov->RecvPacket( sRecv_sock )){	// recv				
							::PrintConsole("[ERROR] in RecvThread : code = %d\n", GetLastError() ) ;
							
							// GS 연결이 끊어졌을경우 해당 서버의 유저들을 강제 로그아웃 시키자..
							for(int i=0; i<GS_MAX; i++){						
								if( (_h_DS_Com *)pRecv_ov->m_pDSComm == g_ServerInfo.m_ServerNetInfo[i].ds_com ){
									reserveAccountLogout(i);

									// GS 정보 초기화
									g_ServerInfo.m_ServerStateInfo[i].cState = GS_NONSTATE;
									g_ServerInfo.m_ServerStateInfo[i].iConnectCount = 0;
									g_ServerInfo.m_ServerStateInfo[i].iPlayingCount = 0;
									g_ServerInfo.m_ServerNetInfo[i].ds_com = NULL ;
									

									// GS가 끊어진 사실을 다른 GS 들에게 알려주자..
									GSDisConnectInfoSend( i ) ;

									break;
								}
							}
						
							// release Connection
							pRecv_ov->m_gs_info.state = GS_NONSTATE;
							_h_DS_Com * pDSComm  = pRecv_ov->m_pDSComm ;
							if( pDSComm )
							{
								pDSComm->DisConnectCom() ;								
							}

							continue;
						}
#ifdef _PD_DSCOM_DETAIL_
						else
						{
							PrintConsole( "[RES RECV]\n" );
						}
#endif
					}
					catch (...) {
						::PrintConsole( "[EXCEPTION] DB-Call LoginPro\n" ) ;
						continue;
					}					
					
				}
				else {	// recv fale

					int pos = 0  ;
					int i=0 ;
					try{
						memset(ip_addr, 0, sizeof(ip_addr));
						ip_char(pRecv_ov->m_gs_info.ipAddr, ip_addr);
						::PrintConsole( "[SO_ERR] Recv error(Close socket) : socket = %u, ip = %s \n", pRecv_ov->m_socket, ip_addr ) ;

						pos = 1 ;
						
						// GS 연결이 끊어졌을경우 해당 서버의 유저들을 강제 로그아웃 시키자..
						for(i=0; i<GS_MAX; i++){						
							if( (_h_DS_Com *)pRecv_ov->m_pDSComm == g_ServerInfo.m_ServerNetInfo[i].ds_com ){							
								reserveAccountLogout(i);

								pos = 2 ;

								// GS 정보 초기화
								g_ServerInfo.m_ServerStateInfo[i].cState = GS_NONSTATE;
								g_ServerInfo.m_ServerStateInfo[i].iConnectCount = 0;
								g_ServerInfo.m_ServerStateInfo[i].iPlayingCount = 0;
								g_ServerInfo.m_ServerNetInfo[i].ds_com = NULL ;

#ifdef _PD_CASTLE_STEP_1_
								CDSCastleManager::GetObj().notice_server_down( i );
#endif

								break;
							}
						}

						pos = 3 ;

						// release Connection
						pRecv_ov->m_gs_info.state = GS_NONSTATE;
						_h_DS_Com * pDSComm  = pRecv_ov->m_pDSComm ;
						if( pDSComm )
						{
							pos = 5 ;
							pDSComm->ShowState( i );
							pDSComm->DisConnectCom() ;		
						}
						continue;
					}
					catch (...) {
						::PrintConsole( "[EXCEPTION] OV release (pos:%d, i:%d) \n", pos, i ) ;
						continue;
					}				
					
				}
			}
			else {	// Is send mode...				
				if( dwNumBytes > 0 ) 
				{
					pDSComm = (_h_DS_Com *)pRecv_ov->m_pDSComm;					

					try{
						if( pDSComm )						
						{
							//if( pDSComm->send_complete(dwNumBytes) == false ) 
							//	::PrintConsole("[SO_ERR]send_complete false\n" ) ;
							//printf( " ** Send to GS \n" );
						}						
					}
					catch (...) {
						::PrintConsole( "[EXCEPTION] pDSComm->send_complete()\n" ) ;
						continue;
					}					
					
				}
				else // send fail (dwNumBytes <= 0)
				{
					printf( " -- send error : 3\n" ); 
					/*
					int pos = 0 ;  
					try{						
						
						memset(ip_addr, 0, sizeof(ip_addr));
						ip_char(pRecv_ov->m_gs_info.ipAddr, ip_addr);
						::PrintConsole( "[SO_ERR] Return 0 :3 recv thread socket error (ErrorCode: %d, ip: %s)\n", WSAGetLastError(),ip_addr ) ;

						pos = 1 ;

						// GS 연결이 끊어졌을경우 해당 서버의 유저들을 강제 로그아웃 시키자..
						for(int i=0; i<GS_MAX; i++){						
							if( (_h_DS_Com *)pRecv_ov->m_pDSComm == g_ServerInfo.m_ServerNetInfo[i].ds_com ){							
								reserveAccountLogout(i);

								// GS 정보 초기화
								g_ServerInfo.m_ServerStateInfo[i].cState = GS_NONSTATE;
								g_ServerInfo.m_ServerStateInfo[i].iConnectCount = 0;
								g_ServerInfo.m_ServerStateInfo[i].iPlayingCount = 0;
								g_ServerInfo.m_ServerNetInfo[i].ds_com = NULL ;
								
								break;
							}
						}

						pos = 2 ;

						// release Connection
						pRecv_ov->m_gs_info.state = GS_NONSTATE;
						_h_DS_Com * pDSComm  = pRecv_ov->m_pDSComm ;
						if( pDSComm )
						{
							pDSComm->DisConnectCom() ;							
						}
						
					}
					catch (...) {
						char log[128] = {0} ;
						sprintf( log, "[EXCEPTION] OV release (DISCONNECT) (pos.%d) \n", pos) ;
						::PrintConsole( log ) ;
					}*/
				}
			}
		}


		else if(!bReturn) {	// GetStatus == false
			if( pOverlapped ) {					
				if( pOverlapped->m_bMode ) {		// true : Is read mode

					int pos = 0 ;  
					try{						
						
						memset(ip_addr, 0, sizeof(ip_addr));
						ip_char(pRecv_ov->m_gs_info.ipAddr, ip_addr);
						::PrintConsole( "[SO_ERR] Return 0 :4 recv thread socket error (ErrorCode: %d, ip: %s)\n", WSAGetLastError(),ip_addr ) ;

						pos = 1 ;

						// GS 연결이 끊어졌을경우 해당 서버의 유저들을 강제 로그아웃 시키자..
						for(int i=0; i<GS_MAX; i++){						
							if( (_h_DS_Com *)pRecv_ov->m_pDSComm == g_ServerInfo.m_ServerNetInfo[i].ds_com ){							
								reserveAccountLogout(i);

								// GS 정보 초기화
								g_ServerInfo.m_ServerStateInfo[i].cState = GS_NONSTATE;
								g_ServerInfo.m_ServerStateInfo[i].iConnectCount = 0;
								g_ServerInfo.m_ServerStateInfo[i].iPlayingCount = 0;
								g_ServerInfo.m_ServerNetInfo[i].ds_com = NULL ;
								
								break;
							}
						}

						pos = 2 ;

						// release Connection
						pRecv_ov->m_gs_info.state = GS_NONSTATE;
						_h_DS_Com * pDSComm  = pRecv_ov->m_pDSComm ;
						if( pDSComm )
						{
							pDSComm->DisConnectCom() ;							
						}
						
					}
					catch (...) {
						char log[128] = {0} ;
						sprintf( log, "[EXCEPTION] OV release (DISCONNECT) (pos.%d) \n", pos) ;
						::PrintConsole( log ) ;
					}

					
				}
				else
					printf( " -- send error : 4\n" );

			}
			else {									
				::PrintConsole( "Return 0 :7 recv thread socket error : %d\n", WSAGetLastError() ) ;
			}			
		}		
	}

	::PrintConsole("DWORD WINAPI th_Recvthread( LPVOID lp ) Exit \n") ;
	printf("DWORD WINAPI th_Recvthread( LPVOID lp ) Exit \n") ;

	g_pSystemLog->LogFileWrite( "[EXIT] th_RecvThread Return\n " ) ;

	return 0 ;
}

#ifdef ITEMSHOP_PATCH_200607
//////////////////////////////////////////////////////////////////////////▼▼▼
// 2006.05.30 Molla, Nexon Web Shop
//#define INCLUDE_ODBC_HEADER
//#include "./MyLibrary/gGlobal.h"

void SamplePktProc(WEB_SHOP_PKT_PROC_PARAM* pParam, char* pRecvPkt, COdbc* pDB_Account, COdbc* pDB_NDGame, COdbc* pDB_ItemShop);
void PktProc_AdmSearch(WEB_SHOP_PKT_PROC_PARAM* pParam, char* pRecvPkt, COdbc* pDB_Account, COdbc* pDB_NDGame, COdbc* pDB_ItemShop);
void PktProc_CharSearch(WEB_SHOP_PKT_PROC_PARAM* pParam, char* pRecvPkt, COdbc* pDB_Account, COdbc* pDB_NDGame, COdbc* pDB_ItemShop);
void PktProc_NewShopItem(WEB_SHOP_PKT_PROC_PARAM* pParam, char* pRecvPkt, COdbc* pDB_Account, COdbc* pDB_NDGame, COdbc* pDB_ItemShop);
void PktProc_NewShopGift(WEB_SHOP_PKT_PROC_PARAM* pParam, char* pRecvPkt, COdbc* pDB_Account, COdbc* pDB_NDGame, COdbc* pDB_ItemShop);

unsigned long __stdcall	WebShopPktProc(void* pVoid) {

	WEB_SHOP_PKT_PROC_INFO* pInfo = NULL;
	DWORD dwEventObjectRes = 0;
	#define MAX_RUNNING_CNT		3
	int iRunningCnt = 0;
	CWebShopPktQueue::QUEUE_RESULT QRes;
	#define MAX_PKT_SIZE	256
	char pRecvPktData[MAX_PKT_SIZE] = {0};
	WEB_SHOP_PKT_PROC_PARAM PktProcParam;
	BOOL bRet = FALSE;

	COdbc* pDB_Account = NULL;
	COdbc* pDB_NDGame = NULL;
	COdbc* pDB_ItemShop = NULL;

	goto Label_Exit_WebShopPktProc; // 디비 연결 없고, 쓰레드 종료
	MessageBox(NULL, _T("실행될 수 없는 코드, WebShopPktProc"), NULL, 0);

	// Account DB
	pDB_Account = new COdbc;
	bRet = pDB_Account->Connect(_T("NDGame01"), _T("gamesa"), _T("ozzy2000"));
	if(bRet == FALSE) {
		::PrintConsole("%s(%d) Account 디비 연결 실패 \n", __FILE__, __LINE__);
		g_pSystemLog->LogFileWrite("%s(%d) Account 디비 연결 실패 \n", __FILE__, __LINE__);
		goto Label_Exit_WebShopPktProc;
	}

	// ND Game DB
	pDB_NDGame = new COdbc;
	bRet = pDB_NDGame->Connect(_T("NDGame01"), _T("gamesa"), _T("ozzy2000"));
	if(bRet == FALSE) {
		::PrintConsole("%s(%d) ND 게임 디비 연결 실패 \n", __FILE__, __LINE__);
		g_pSystemLog->LogFileWrite("%s(%d) ND 게임 디비 연결 실패 \n", __FILE__, __LINE__);
		goto Label_Exit_WebShopPktProc;
	}

	// Item Shop DB
	pDB_ItemShop = new COdbc;
	bRet = pDB_ItemShop->Connect(_T("ItemShopTest"), _T("shop_gw_user_nine"), _T("shop_gw_user_nine_password"));
	if(bRet == FALSE) {
		::PrintConsole("%s(%d) 아이템 샵 디비 연결 실패 \n", __FILE__, __LINE__);
		g_pSystemLog->LogFileWrite("%s(%d) 아이템 샵 디비 연결 실패 \n", __FILE__, __LINE__);
		goto Label_Exit_WebShopPktProc;
	}

	while(g_Threading) {
		if(listWebShopPktProcInfo.empty()) {
			dwEventObjectRes = WaitForSingleObject(hWebShopPktProcInfo, 1024);

			if(dwEventObjectRes == WAIT_TIMEOUT) continue;
			else if(dwEventObjectRes != WAIT_OBJECT_0) {
				::PrintConsole("WebShopPktProc, WaitForSingleObject() 실패, GetLastError=%d \n", GetLastError());
				g_pSystemLog->LogFileWrite("WebShopPktProc, WaitForSingleObject() 실패, GetLastError=%d \n", GetLastError()) ;
				goto Label_Exit_WebShopPktProc;
			}
		}

		// 이벤트 발생시킨 클라이언트 정보 얻기
		CS_LOCK(csWebShopPktProcInfo);
		if(!listWebShopPktProcInfo.empty()) {
			pInfo = listWebShopPktProcInfo.front();
			listWebShopPktProcInfo.pop_front();
		}
		else {
			CS_UNLOCK(csWebShopPktProcInfo);
			continue;
		}
		CS_UNLOCK(csWebShopPktProcInfo);
		if(pInfo == NULL) continue;

		iRunningCnt = 0;
		while(iRunningCnt++ < MAX_RUNNING_CNT) {
			// pop a packet from PktQue
			QRes = pInfo->pClient->pWebShopPktQueue->Pop(pRecvPktData);
			if(QRes != CWebShopPktQueue::QRES_OK) {
				if(QRes == CWebShopPktQueue::QRES_NOT_ENOUGH_DATA) break;
				else if(QRes == CWebShopPktQueue::QRES_NOT_INITIALIZED) {
					::PrintConsole("pWebShopPktQueue가 초기화 되지 않았음 \n");
					g_pSystemLog->LogFileWrite("pWebShopPktQueue가 초기화 되지 않았음 \n");
					goto Label_Exit_WebShopPktProc;
				}
			}

			PktProcParam.pClient = pInfo->pClient;
			try {
				// SamplePktProc(&PktProcParam, pRecvPktData, pDB_ItemShop); // for test

				// 패킷 처리 프로세스
				switch((UCHAR)pRecvPktData[0]/*QueryType*/) {
				case 16: // Q_ADM_SEARCH
					PktProc_AdmSearch(&PktProcParam, pRecvPktData, pDB_Account, pDB_NDGame, pDB_ItemShop);
					break;

				case 17: // Q_CHAR_SEARCH
					PktProc_CharSearch(&PktProcParam, pRecvPktData, pDB_Account, pDB_NDGame, pDB_ItemShop);
					break;

				case 100: // Q_NEW_SHOP_ITEM
					PktProc_NewShopItem(&PktProcParam, pRecvPktData, pDB_Account, pDB_NDGame, pDB_ItemShop);
					break;

				case 101: // Q_NEW_SHOP_GIFT
					PktProc_NewShopGift(&PktProcParam, pRecvPktData, pDB_Account, pDB_NDGame, pDB_ItemShop);
					break;

				default:
					::PrintConsole(_T("%s(%d) 넥슨 웹 서버로부터 인증되지 않은 패킷이 수신되었음 \n"), __FILE__, __LINE__);
					break;
				}
			}
			catch(...) {
				::PrintConsole(_T("%s(%d), catch(...) \n"), __FILE__, __LINE__);
				g_pSystemLog->LogFileWrite(_T("%s(%d), catch(...) \n"), __FILE__, __LINE__);
			}
		}

		if(iRunningCnt == MAX_RUNNING_CNT && pInfo->pClient->get_sock() != INVALID_SOCKET) {
			// 패킷큐에 처리할 데이타가 남아있음을 알림
			CS_LOCK(csWebShopPktProcInfo);
			listWebShopPktProcInfo.push_back(pInfo);
			CS_UNLOCK(csWebShopPktProcInfo);
		}
		else {
			SAFE_DELETE(pInfo);
		}
	} //while(TRUE)

	#undef MAX_RUNNING_CNT
	#undef MAX_PKT_SIZE

Label_Exit_WebShopPktProc:
	SAFE_DELETE(pDB_Account);
	SAFE_DELETE(pDB_NDGame);
	SAFE_DELETE(pDB_ItemShop);
	std::list<WEB_SHOP_PKT_PROC_INFO*>::iterator it = listWebShopPktProcInfo.begin();
	for(WEB_SHOP_PKT_PROC_INFO* pInfo = NULL; it != listWebShopPktProcInfo.end(); it++) {
		pInfo = *it;
		SAFE_DELETE(pInfo);
	}
	listWebShopPktProcInfo.clear();

	SAFE_CLOSE_HANDLE(hWebShopPktProcInfo);
	CS_DEINIT(csWebShopPktProcInfo);

	::PrintConsole("unsigned long __stdcall	WebShopPktProc(void* pVoid) Exit \n") ;
	printf("unsigned long __stdcall	WebShopPktProc(void* pVoid) Exit \n") ;
	g_pSystemLog->LogFileWrite( "[EXIT] WebShopPktProc Return\n " ) ;
	return 0 ;
}
//////////////////////////////////////////////////////////////////////////▲▲▲
#endif // ITEMSHOP_PATCH_200607