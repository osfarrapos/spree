#include "stdafx.h"
#include "MAPManager\MAPManager.h"
//#include "DB\DBLogin.h"
#include "ole_db_module\ole_db_module.h"
#include "ole_db_module\db_command.h"
#include "Global\Global_ServerInfo.h"
#include "Global\Global_UniqueID.h"
#include "Global\tcp_packets_party.h"
#include "CS\party_manager.h"
#include "Network\LogSvrLink.h"

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

#endif	// _NEXON_NETWORK_

extern _j_AccMAPManager *		g_pAccMapManager;	// AccMap Manager class pointer..
extern _j_ServerInfo			g_ServerInfo;				// GS Server infomation object


extern _j_UniqueIDQueue *		g_pUniqueIDQueue ;
extern CUserMemSaveTimer *		g_pMemSaveTimer	;

extern _j_queue_UDP_Packet *	g_pUDPLoinPacketQ ;
extern _j_PartyManager *		g_pPartyManager ;

extern CLogSvr_Com *			g_pLogSvrCom ;
extern bool g_Threading ; 
extern char g_ForestName[13] ;


void	ShowAccountList()
{
	
	PUSER pUser;
	int	count;
	count = 0;

	

	TRY{		

		printf("ACCOUNT      UNIQUEID SERVER SELECTCHARAC MAXCHARAC  IP_ADDRESS       STATE\n");
		char ipAdd[17];

		pUser = g_pAccMapManager->MoveBeginUser();		
		
		while (pUser) 
		{
			memset(ipAdd,0,17);
			ip_char(pUser->iIp, ipAdd);

			
			printf("%-12s %8d %6d %12s %9d %15s %6d\n", 
				pUser->cNDAccount, pUser->iUid, pUser->iServer, pUser->cSelChrName, pUser->cCharacCount, 
				ipAdd, pUser->cState);

			count++;

			pUser = g_pAccMapManager->MoveNextUser( pUser );

		}//while			
			
		

		printf("\n _____ total : %d \n", count);	
	}
	CATCH_ALL{
		printf("[EXCEPTION] ShowAccountList() \n");
		::PrintConsole("[EXCEPTION] ShowAccountList() \n");		

	}


	
}

void	ShowAccountInfo(char * cAccount)
{

	PUSER pUser;
	

	TRY{	
	
		pUser = g_pAccMapManager->Find_User(cAccount);	

		if(pUser == NULL){
			printf("Not find! \n");

			return;
		}
		else{
			printf("==================================== \n");
			printf("ACCOUNT : %s \n", pUser->cNDAccount);
			printf("UNIQUEID : %d \n", pUser->iUid );
			printf("SERVER : %d \n", pUser->iServer);		
			printf("MEMSTATE : %d \n", pUser->bMemState );		
			printf("STATE : %d \n", pUser->cState);		
			printf("TEMPID : %u \n", pUser->usTempUniqueID );
			printf("DEPOT_BS : %d \n", pUser->cDepotBackupState);		
			printf("CHARAC_BS : %d,%d,%d \n", pUser->stCharac[0].CHARAC_ETC._sBACKUP_STATE_CHECKED
				, pUser->stCharac[0].CHARAC_ETC._sBACKUP_STATE_CHECKED
				, pUser->stCharac[0].CHARAC_ETC._sBACKUP_STATE_CHECKED);		
			printf("==================================== \n");
		}	
	}
	CATCH_ALL {
		printf("[EXCEPTION] ShowAccountInfo() \n");
		::PrintConsole("[EXCEPTION] ShowAccountInfo() \n");		
	}	
}

void	ShowCharacList()
{


	PCHARAC	pCharac;
	int	count = 0;

	

	TRY{
			
		pCharac = g_pAccMapManager->MoveBeginCharac();
				
			
		printf("CHARACNAME   ACCOUNT      UNIQUEID ZONE STATE\n");
		while (pCharac ) {
			
			printf("->") ;

			
			printf("%-12s %12s %8d %4d %4d |\n", 
				pCharac->CHARAC_BASIC.cChrName, pCharac->CHARAC_BASIC.cAccount, pCharac->CHARAC_BASIC.iUniqueID, pCharac->CHARAC_CUR_BASIC.sZone ,pCharac->CHARAC_ETC.cState);
			

			count++;
			if( count > 20000)
			{
				break ;
			}

			pCharac = g_pAccMapManager->MoveNextCharac( pCharac );
				

		}//while

		

		printf("\n _____ total : %d \n", count);	
	}
	CATCH_ALL {
		printf("[EXCEPTION] ShowCharacList() \n");
		::PrintConsole("[EXCEPTION] ShowCharacList() \n");

	}	
	
}

void	ShowCharacInfo(char * cCharac)
{
	TRY{
		PCHARAC	pCharac;
		
		pCharac = g_pAccMapManager->Find_Charac(cCharac);
		

		if(pCharac == NULL){
			printf("Not find! \n");
			return;
		}
		else{
			printf("==================================== \n");
			printf("CHARAC NAME : %s \n", pCharac->CHARAC_BASIC.cChrName);
			printf("ID : %d \n", pCharac->CHARAC_BASIC.iUniqueID);
			printf("SERVER : %d \n", pCharac->CHARAC_CUR_BASIC.sZone);		
			printf("==================================== \n");
		}	
	}
	CATCH_ALL {
		printf("[EXCEPTION] ShowCharacInfo() \n");
		::PrintConsole("[EXCEPTION] ShowCharacInfo() \n");
		return;
	}
	
	
}

void ShowSkill( char * cCharac )
{
	TRY{
		PCHARAC	pCharac;
		
		pCharac = g_pAccMapManager->Find_Charac(cCharac);
		

		if(pCharac == NULL){
			printf("Not find! \n");
			return;
		}
		else{
			printf("==================================== \n");
			printf("CHARAC NAME : %s \n", pCharac->CHARAC_BASIC.cChrName);
			
			_sCharac_Skill * skill = (_sCharac_Skill*)pCharac->CHARAC_MARTIAL_DATA.MartialData.cMartialData ;
			for( int i = 0 ; i < 60 ; i++ )
			{
				printf("- %d : %d \n", i, skill[i].id) ;
			}

			printf("==================================== \n");
		}	
	}
	CATCH_ALL {
		printf("[EXCEPTION] ShowCharacInfo() \n");
		::PrintConsole("[EXCEPTION] ShowCharacInfo() \n");
		return;
	}
	

}

void	ShowServerList(){

	TRY{
		int iTotalCount = 0 ;
		int iPlayCount = 0 ;
		printf("SERVER_NAME     NUM IP               UDP_PORT  STATE CON_COUNT  PLY_COUNT\n");
		for(int i=1; i<GS_MAX; i++) {	

			//if(g_ServerInfo.m_ServerStateInfo[i].cState > 0){
				printf("%-16s%-4d %-17s %-10d %-6d %-11d %-9d \n", 
					g_ServerInfo.m_ServerSystemInfo[i].cServerName, i, g_ServerInfo.m_ServerNetInfo[i].cIP, g_ServerInfo.m_ServerNetInfo[i].sPort, g_ServerInfo.m_ServerStateInfo[i].cState,
					g_ServerInfo.m_ServerStateInfo[i].iConnectCount, g_ServerInfo.m_ServerStateInfo[i].iPlayingCount);
				
				iTotalCount = iTotalCount + g_ServerInfo.m_ServerStateInfo[i].iConnectCount ;
				iPlayCount = iPlayCount + g_ServerInfo.m_ServerStateInfo[i].iPlayingCount ;
			//}
			
		}//for

		printf("TOTAL : %d / %d  Connect / Play \n", iTotalCount, iPlayCount ) ;
	}
	CATCH_ALL {
		return;
	}

}



DWORD WINAPI th_ConnectCountRecorderThread( LPVOID lp ) 
{

	_ServerState * pServerLog ;

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, (60000), NULL, NULL, TRUE ) ;

	while( g_Threading ) 
	{
		WaitForSingleObject( hTimer, INFINITE ) ;			

		pServerLog = g_pLogSvrCom->GetServerLogSlot() ;
		if( pServerLog )
		{
			memcpy( pServerLog->m_server_name, g_ForestName, 13 ) ;
			pServerLog->m_state = g_ServerInfo.GetForestUserCount() ;

			g_pLogSvrCom->InsertServerLog( pServerLog ) ;
		}

	} // while		

	printf("[WORNING] th_ConnectCountRecorderThread exit \n");
	::PrintConsole("[WORNING] th_ConnectCountRecorderThread exit \n");

	return 0;

}




DWORD WINAPI th_CommandBackupThread( LPVOID lp ) 
{

	int iOption = 0;
	if( lp )
	{
		iOption = *((int*)lp) ;
	}


	// oledb
	extern CDB_Connector *	g_pDBCon;

	COLE_DB_Executer *	g_DB_Command = new COLE_DB_Executer(g_pDBCon) ;
	assert( g_DB_Command ) ;

	PCHARAC		pCharac = NULL;
	PUSER		pUser = NULL ;
	int	count = 0;
	bool bRet ;
	
	if( iOption == 0 )
	{
		pCharac = g_pAccMapManager->MoveBeginCharac();	

		while (pCharac) {

			::PrintConsole( "[BACKUP] Start 'sync' data backup \n") ;

			pCharac->charac_lock() ;
			try{
				if( pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED != BS_COMPLETED )
				{
					try{
						bRet = g_DB_Command->Update_Charac_All(pCharac->CHARAC_BASIC.iUniqueID, pCharac) ;
					}
					catch(...){
						::PrintConsole( "[exception] %s, %d \n ", __FILE__, __LINE__ ) ;
					}			

					if( bRet == false ){
						::PrintConsole("[DB_ERR] Character data backup failed (%s)\n", pCharac->CHARAC_BASIC.cChrName);
					}
					else
					{
						pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED = BS_COMPLETED ;
						count++;
					}


				}

				pUser = g_pAccMapManager->Find_User( pCharac->CHARAC_BASIC.cAccount ) ;
				if( pUser ) 
				{							
					pUser->user_lock() ;
					try{
						if( pUser->cDepotBackupState != BS_COMPLETED )
						{

							if( !g_DB_Command->UpdateAccountItemUseStatus( pUser->iUid, pUser ) )
							{
								::PrintConsole( "[DB_ERR] AIUS Backup failed (%s)\n ", pUser->cNDAccount );
							}

							if(!g_DB_Command->pr_UpdateDepot(pUser->iUid, pUser) ) {
								// 로그에 남기자..
								::PrintConsole( "[DB_ERR] User depot data backup failed (%s) ", pUser->cNDAccount );
							}		
							else{							
								pUser->cDepotBackupState = BS_COMPLETED;							
								//g_pTrace->OutputString( _TRACE_CLASS_BACKUP, "[BACKUP] Backup Account's depot ( Charac name : %s ) \n", pUser->cNDAccount ) ;		
							}
						}					
					}
					catch(...){
						__EXCEPT_STR_PRINT() ;
					}					
					pUser->user_unlock() ;
				}			
			}
			catch(...){
				::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
			}
			pCharac->charac_unlock() ;

			pCharac = g_pAccMapManager->MoveNextCharac( pCharac );
			Sleep(10) ;


		} // while		
	}
	else
	{

		::PrintConsole( "[BACKUP] Start 'unsync' data backup \n") ;

		pCharac = g_pAccMapManager->MoveBeginCharac();	

		while (pCharac) {
			
			try{
				if( pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED != BS_COMPLETED )
				{
					try{
						bRet = g_DB_Command->Update_Charac_All(pCharac->CHARAC_BASIC.iUniqueID, pCharac) ;
					}
					catch(...){
						::PrintConsole( "[exception] %s, %d \n ", __FILE__, __LINE__ ) ;
					}			

					if( bRet == false ){
						::PrintConsole("[DB_ERR] Character data backup failed (%s)\n", pCharac->CHARAC_BASIC.cChrName);
					}
					else
					{
						pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED = BS_COMPLETED ;
						count++;
					}


				}

				pUser = g_pAccMapManager->Find_User( pCharac->CHARAC_BASIC.cAccount ) ;
				if( pUser ) 
				{						
					
					try{
						if( pUser->cDepotBackupState != BS_COMPLETED )
						{
							if( !g_DB_Command->UpdateAccountItemUseStatus( pUser->iUid, pUser ) )
							{
								::PrintConsole( "[DB_ERR] AIUS Backup failed (%s)\n ", pUser->cNDAccount );
							}

							if(!g_DB_Command->pr_UpdateDepot(pUser->iUid, pUser) ) {
								// 로그에 남기자..
								::PrintConsole( "[DB_ERR] User depot data backup failed (%s) ", pUser->cNDAccount );
							}
							else{							
								pUser->cDepotBackupState = BS_COMPLETED;							
								//g_pTrace->OutputString( _TRACE_CLASS_BACKUP, "[BACKUP] Backup Account's depot ( Charac name : %s ) \n", pUser->cNDAccount ) ;		
							}
						}					
					}
					catch(...){
						__EXCEPT_STR_PRINT() ;
					}					
					
				}			
			}
			catch(...){
				::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
			}			

			pCharac = g_pAccMapManager->MoveNextCharac( pCharac );
			Sleep(10) ;


		} // while		
	}

	

	printf("[Backup complete] %d\n", count ) ;
	printf("[INFO] Complete Character DB backup. (Count :%d) \n", count);
	::PrintConsole("[INFO] Complete Character DB backup. (Count :%d) \n", count);

	return 0;

}

int CharacDataBackup( int iOption )
{

	TRY{
		DWORD dwThreadID = 0 ; 
		HANDLE hThread=NULL ; 		

		hThread = BEGINTHREADEX( NULL, 0, th_CommandBackupThread, &iOption, 0, &dwThreadID ) ;
		if( hThread != INVALID_HANDLE_VALUE ) {	
			CloseHandle(hThread) ; 
			hThread = INVALID_HANDLE_VALUE ; 
			return 0 ;
		}
		else { 
			printf( "[error] create thread failed \n" ) ;
			return -1 ;
		}
	}
	CATCH_ALL{
		 __EXCEPT_STR_PRINT() ;
	}

	return -1 ;

}

int CharacFileBackup()
{	
	PUSER	pUser = NULL;

	int	count = 0;

	FILE *	stream;
	SYSTEMTIME curTime ;

	char cFile[64] ={0};
	//char buf[64] = {0};	

	GetLocalTime( &curTime ) ;
	sprintf(cFile,"backup\\backup_%d_%d_%d[%d_%d_%d].bak", 
		curTime.wYear,curTime.wMonth,curTime.wDay,curTime.wHour,curTime.wMinute,curTime.wSecond);

	stream = fopen(cFile,"w");

	

	pUser =g_pAccMapManager->MoveBeginUser();
		
	
	TRY{
		while (pUser) {			

			fwrite(pUser, 1, sizeof(_USER), stream);		

			pUser = g_pAccMapManager->MoveNextUser( pUser );
			count++;
		} // while
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] CharacFileBackup() - fwrite(pUser, 1, sizeof(_USER), stream) \n");
	}		
	
	

	fclose(stream);
	
	return count;

}

int MemoryBackup()
{
	PCHARAC	pCharac;

	int	count = 0;

	FILE *	stream;
	SYSTEMTIME curTime ;

	char cFile[64] ={0};
	//char buf[64] = {0};	

	GetLocalTime( &curTime ) ;
	sprintf(cFile,"backup\\memory_backup_%d_%d_%d[%d_%d_%d].bak", 
		curTime.wYear,curTime.wMonth,curTime.wDay,curTime.wHour,curTime.wMinute,curTime.wSecond);

	stream = fopen(cFile,"w");


	pCharac = g_pAccMapManager->MoveBeginCharac();	

	TRY{
		while (pCharac) 
		{
			fwrite(pCharac, 1, sizeof(_CHARAC), stream);
			pCharac = g_pAccMapManager->MoveNextCharac( pCharac );		

		} // while
	}
	CATCH_ALL
	{
		::PrintConsole("[EXCEPTION] MemoryBackup() - fwrite(pCharac, 1, sizeof(_CHARAC), stream); \n");
	}

	fclose(stream);

	return count;

}

SOCKET UDPSocket( unsigned short port ) 
{
	TRY{
		SOCKET sock ;

		
		SOCKADDR_IN local ;
		local.sin_family = AF_INET ;
		local.sin_port = htons( port ) ;
		local.sin_addr.s_addr = htonl( INADDR_ANY ) ;

		sock = socket( AF_INET, SOCK_DGRAM, 0 ) ;

		if( sock == INVALID_SOCKET )
		{			
			g_pSystemLog->LogFileWrite( "Error : socket() Failed %d\n", WSAGetLastError() ) ;

			return INVALID_SOCKET ;
		}

		if( bind( sock, (SOCKADDR*)&local, sizeof(local) ) == SOCKET_ERROR )
		{			
			g_pSystemLog->LogFileWrite( "Error : Recv bind Error %d\n", WSAGetLastError() ) ;

			return INVALID_SOCKET ;
		}

		//setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeo, sizeof(nTimeo) ) ;

		return sock ;
	}
	CATCH_ALL {
		::PrintConsole("[EXCPTION] UDPSocket() \n") ;
		return INVALID_SOCKET ;
	}

	
}

void reserveAccountLogout( int serverNum )
{
	MSG_GS_SHUTDOWN msgGSShutdown = {0} ;
	msgGSShutdown.iKey = PASS_KEY ;
	msgGSShutdown.cMessage = MSG_GS_SHUTDOWN_NUM ;
	msgGSShutdown.iServerNum = serverNum ;

	SOCKADDR_IN addr ;
	g_pUDPLoinPacketQ->insert_udp( &addr, (char*)&msgGSShutdown, sizeof(MSG_GS_SHUTDOWN) ) ;
}

void AccountLogout(int serverNum, COLE_DB_Executer * pDBCommand, C_USER_DB_Command * pUserCommand )
{

	PUSER pUser = NULL;
	_C_CHARAC * pCharac = NULL;
	int	ComplCount;
	int TotalCount;
	bool bRet = false ;


	ComplCount = 0;
	TotalCount = 0;

	int pos = 0 ;
	

	
	pUser = g_pAccMapManager->MoveBeginUser();			

	try{
		while ( pUser ) {

			pUser->user_lock() ;
			try{
				pos = 1 ;

				if( pUser->iServer == serverNum )
				{
					pos = 2 ;
					if( pUser->cState != US_NONSTATE )
					{
						pos = 3 ;
						// 캐릭터 백업	

						pCharac = NULL ;
						for( int i = 0 ; i < 3 ; i++ )
						{
							if( strcmp( pUser->stCharac[i].CHARAC_BASIC.cChrName, pUser->cSelChrName ) == 0 )
							{
								pCharac = &pUser->stCharac[i] ;
								break ;
							}			
						}
						//pCharac = pUser->characFind( pUser->cSelChrName )  ;

						pos = 4 ;

						if( pCharac )
						{
							pCharac->charac_lock() ;
							try{
								if( pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED != BS_COMPLETED )
								{
									pos = 5 ;

									bRet = pDBCommand->Update_Charac_All(pCharac->CHARAC_BASIC.iUniqueID, pCharac) ;
									if( bRet == false )
									{							
										::PrintConsole("[INFO] Backup failed (acc:%s, chr:%s, c_uid:%d)\n"
											, pUser->cNDAccount, pCharac->CHARAC_BASIC.cChrName, pCharac->CHARAC_BASIC.iUniqueID );
										//Sleep(500) ;	// 쉬어본다.
									}
									else
									{
										//Sleep(10) ;
										//g_pTrace->OutputString( _TRACE_CLASS_BACKUP, "[BACKUP] Backup character info ( Charac name : %s ) \n", pCharac->CHARAC_BASIC.cChrName ) ;		
									}

									pos = 6 ;

									pCharac->CHARAC_STATE.sPartyIndex = 0 ;
									pCharac->CHARAC_STATE.sPartySlotNo = 0 ;
								}							
							}
							catch(...){
								::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
							}
							pCharac->charac_unlock() ;							
						}					


						for(int i=0; i<3; i++){
							pUser->stCharac[i].CHARAC_ETC.cState = CS_NONSTATE;						
						}

						pos = 7 ;

						if( pUser->cDepotBackupState != BS_COMPLETED ) 
						{
							pos = 8 ;

							if( !pDBCommand->UpdateAccountItemUseStatus( pUser->iUid, pUser ) )
							{
								::PrintConsole( "[DB_ERR] AIUS Backup failed (%s)\n ", pUser->cNDAccount );
							}

							bRet = pDBCommand->pr_UpdateDepot(pUser->iUid, pUser) ;
							if( bRet == false  ) 
							{
								// 로그에 남기자..
								::PrintConsole( "[DB_ERR] User depot backup failed (%s) \n", pUser->cNDAccount );
								pUser->cDepotBackupState = BS_READY;
							}		
							else
							{
								pUser->cDepotBackupState = BS_COMPLETED;
								//g_pTrace->OutputString( _TRACE_CLASS_BACKUP, "[BACKUP] Backup Account's depot ( Charac name : %s ) \n", pUser->cNDAccount ) ;		
							}

							pos = 9 ;
						}		

						pUser->bCheckCurEvent = 0;		// 로그아웃 일때, 초기화.

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[INFO] Return temp_uid (%d) \n", pUser->usTempUniqueID) ;
#endif			
						// 유니크 아이디 반납
						g_pUniqueIDQueue->RecycleUID( pUser->usTempUniqueID ) ;

						pos = 10 ;

						// User  초기화..
						pUser->usTempUniqueID = 0 ;
						pUser->cState = US_NONSTATE;
						pUser->iTime = 0 ;
						memset(pUser->cSelChrName, 0, en_charac_name_length+1);

						pos = 11 ;

						ComplCount++;

#ifdef	_NXPCB_AWARD_200703_
						NEXONPCB_LogoutNtf(pUser->cNXAcc, "", (DWORD)pUser->iIp);
#endif
						
						try{
							bRet = pUserCommand->DSLogout( pUser->cNDAccount ) ;
						}
						catch(...){
							::PrintConsole( "[exception] %s, %d \n ", __FILE__, __LINE__ ) ;
						}					


						if( bRet == false )	// DB 에서 로그아웃 처리~
						{
							::PrintConsole("[DB_ERR] pUserDB_Command->DSLogout( pUser->cAccount ) == false \n" ) ;							
						}

						pos = 12 ;

						// 메모리 삭제 예약..
						g_pMemSaveTimer->Reserve_save( pUser, pUser->iUid, CUserMemSaveTimer::get_time_by_sec() + _MEM_SAVE_DELAY_TIME_LOGOUT ) ;
					}			

					TotalCount++;

				}

				pos = 13 ;				
				
			}
			catch(...){
				::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
			}
			pUser->user_unlock() ;

			Sleep(3) ;
			pUser = g_pAccMapManager->MoveNextUser( pUser );			
			
		}	// while


	}
	catch(...){			
		::PrintConsole( "[EXCEPTION]  GS(No.%d) Disconnected. And Exception \n", serverNum ) ;
	}					
		
	
	
	// 로그 남기자..
	::PrintConsole("[INFO] GS(No.%d) Disconnected, User data backup success (%d/%d) \n", serverNum, ComplCount, TotalCount);		
	
}


int CharacRestore(char * cFileName){

	PUSER	pUser = NULL;
	PCHARAC	pCharac = NULL;

	int	count = 0;

	FILE *	stream;

	char cFile[64] ={0};
		
	sprintf(cFile,"backup\\%s.bak",	cFileName);

	stream = fopen(cFile,"r");

	
		
	TRY{	
		
		while (!feof(stream)) {
			
			pUser =g_pAccMapManager->GetWritePtr();

			if( fread(pUser,sizeof(_USER),1,stream) == NULL) break;

			if(pUser){				
				g_pAccMapManager->Insert(pUser);
			}


			count++;

		} // while
		fclose(stream);
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__);
	}

	

	return count;

}



bool ReadInitValueFile( const char * const fileName, int & life, int & force, int & con )
{

	FILE * fp = NULL ;
	int nRead = 0 ;
	char buf[128] = {0} ;
	char cBind[3][13] ;

	bool bRet = false ;

	TRY{	

		if( ( fp = fopen( fileName, "rt" ) ) == NULL )
		{
			return false ;
		}

		while (1) {

			if( fgets( buf, sizeof(buf), fp ) == NULL )
			{
				bRet = false ;
				break ;
			}			

			if( buf[0] == ';')
			{
				continue ;
			}

			nRead = sscanf( buf, "%s %s %s", cBind[0], cBind[1], cBind[2] ) ;

			if( nRead != 3 )		/// 읽어온 필드수를 검사한다.
			{
				continue ;
			}
			else
			{
				life = atoi(cBind[0]) ;
				force = atoi(cBind[1]) ;
				con = atoi(cBind[2]) ;

				bRet = true ;
				break ;
			}			
		}

		fclose( fp ) ;

	}
	CATCH_ALL{
		fclose(fp) ;
		return false ;
	}	

	return bRet ;
}



bool ReadInitEventFile( const char * const fileName, _TempEvent* event )
{

	FILE * fp = NULL ;
	int nRead = 0 ;
	char buf[128] = {0} ;
	char cBind[10][13] ;

	bool bRet = false ;

	TRY{	

		if( ( fp = fopen( fileName, "rt" ) ) == NULL )
		{
			return false ;
		}

		while (1) {

			if( fgets( buf, sizeof(buf), fp ) == NULL )
			{				
				break ;
			}			

			if( buf[0] == ';' || buf[0] == 10 )
			{
				continue ;
			}

			nRead = sscanf( buf, "%s %s %s %s %s %s", cBind[0], cBind[1], cBind[2], cBind[3], cBind[4], cBind[5] ) ;

			if( nRead != 6 )		// 읽어온 필드수를 검사한다.
			{
				continue ;
			}
			else
			{
				event[atoi(cBind[0])].ItemFType = atoi(cBind[1]) ;
				event[atoi(cBind[0])].ItemSType = atoi(cBind[2]) ;
				event[atoi(cBind[0])].ItemID = atoi(cBind[3]) ;					
				event[atoi(cBind[0])].ItemDur = atoi(cBind[4]) ;
				event[atoi(cBind[0])].SkillID = atoi(cBind[5]) ;

				bRet = true ;				
			}			
		}

		fclose( fp ) ;

	}
	CATCH_ALL{
		fclose(fp) ;
		return false ;
	}	

	return bRet ;
}