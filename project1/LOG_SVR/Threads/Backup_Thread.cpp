#include "..\stdafx.h"
#include "..\OLE-DB\db_Con.h"
#include "..\OLE-DB\db_command.h"
#include "..\VMManager\VMManager.h"
#include "..\Map\MAPManager.h"
#include "Backup_Thread.h"
#include "..\Global\Trace.h"
#include "../MyLibrary/MY_OLE_DB.H"

extern bool g_pThreadRunning ;

extern CDB_Connector * g_pDBCon ;
extern CMYTrace *		g_pTrace ;


extern CTQueue<_ServerState> * g_pServerStateQueue ;
extern CTQueue<_ItemLog> * g_pItemLogQueue ;
extern CTQueue<_CharacLog> * g_pCharacLogQueue ;
extern CTQueue<_LoginLog> * g_pLoginLogQueue ;

extern _j_GlobalLog * g_pSystemLog ;


/*#################################################################################

	CS Timer Thread

#################################################################################*/
DWORD WINAPI th_LogWriteTimer( LPVOID lp ) 
{
	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;

	LARGE_INTEGER li = {0, } ;
	
	_Temp_timerentry entry ;

	_j_Templet_Timer * pTimer ;
	pTimer = (_j_Templet_Timer*)lp ;

	
	// OLE DB COMMAND TABLE OBJECT	
	HRESULT hr ;

	C_Insert_ServerState InsertServerStateLog ;
	hr = InsertServerStateLog.SessionLink( g_pDBCon ) ;
	if( FAILED(hr) ){
		g_pSystemLog->LogFileWrite( "[FAIL] C_Insert_ServerState session link\n" ) ;
		_endthreadex(0);
		return 0 ;
	}

	C_Insert_Item	InsertItemLog ;
	hr = InsertItemLog.SessionLink( g_pDBCon ) ;
	if( FAILED(hr) ){
		g_pSystemLog->LogFileWrite( "[FAIL] C_Insert_Item session link\n" ) ;
		return 0 ;
	}


	C_Insert_Charac	InsertCharacLog ;
	hr = InsertCharacLog.SessionLink( g_pDBCon ) ;
	if( FAILED(hr) ){
		g_pSystemLog->LogFileWrite( "[FAIL] C_Insert_Charac session link\n" ) ;
		return 0 ;
	}


	C_Insert_LoginLog InsertLoginLog ;
	hr = InsertLoginLog.SessionLink( g_pDBCon ) ;
	if( FAILED(hr) ){
		g_pSystemLog->LogFileWrite( "[FAIL] C_Insert_LoginLog session link\n" ) ;
		return 0 ;
	}
	
	
	/*
	_ItemLog itemlog = {0} ;	
	itemlog.code = 0 ;
	itemlog.zone = 0 ;
	itemlog.from_uid = 0 ;
	sprintf(itemlog.from, "test") ;
	itemlog.to_uid = 0 ;
	sprintf(itemlog.to, "test" );
	itemlog.money = 0 ;
	itemlog.type1 = 0 ;
	itemlog.type2 = 0 ;
	
	InsertItemLog.Execute( &itemlog ) ;
	

	_CharacLog log_Charac = {0} ;
	sprintf( log_Charac.account, "ozzywow") ;
	sprintf( log_Charac.charac, "백송") ;
	log_Charac.code = 1 ;
	sprintf ( log_Charac.ip, "218.147.34.133" ) ;
	log_Charac.level = 100 ;
	log_Charac.point = 1 ;
	log_Charac.zone  = 1 ;
	InsertCharacLog.Execute( &log_Charac ) ;
	*/

	/* 통합 로그인
	_LoginLog log_LoginLog = {0} ;
	log_LoginLog.acc_uid = 1 ;
	sprintf( log_LoginLog.acc, "test" ) ;
	sprintf( log_LoginLog.charac, "test" ) ;
	log_LoginLog.inner_level = 1 ;
	sprintf( log_LoginLog.ip, "0.0.0.0" ) ;
	sprintf( log_LoginLog.login_time, "2005-11-3 10:10:10") ;
	sprintf( log_LoginLog.logout_time, "2005-11-3 10:10:10") ;
	log_LoginLog.m_class = 1 ;
	log_LoginLog.party = 1 ;
	log_LoginLog.pc_pay_code[0] = 'T' ;
	sprintf( log_LoginLog.pc_uid, "1111" ) ;
	log_LoginLog.pe_pay_code[0] = 'R' ;
	log_LoginLog.play_time = 100 ;
	log_LoginLog.type = 1 ;
	log_LoginLog.zone_no = 1 ;
	InsertLoginLog.Execute( &log_LoginLog ) ;
	*/
	

	_ServerState * pStateNode ;
	_ItemLog * pItemNode ;
	_CharacLog * pCharacNode ;
	_LoginLog * pLoginLogNode ;


	printf( "  => Start Timer\n" ) ;
		
	SetWaitableTimer( hTimer, &li, 50, NULL, NULL, TRUE ) ;

	while(1) {

		WaitForSingleObject( hTimer, INFINITE ) ;
		
		while ( pTimer->Get_TimeMessage( entry ) == true ) {


			switch( entry.Section ) {
			case MSG_NO_SERVER_STATE :
				{
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[DB_SAVE] SvrLog \n" ) ;
#endif					
					pStateNode = g_pServerStateQueue->Use_Pop() ;

					if( pStateNode ) 
					{
						if ( InsertServerStateLog.Execute( pStateNode ) == false )
						{
							::PrintConsole( "[ERROR] InsertServerState.Execute \n " ) ;
						}

						g_pServerStateQueue->Ready_Put( pStateNode ) ;
					}					
					
				}
				break;

			case MSG_NO_ITEM :
				{
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[DB_SAVE] ItemLog \n" ) ;
#endif
					
					pItemNode = g_pItemLogQueue->Use_Pop() ;
										
					if( pItemNode ) 
					{
						if ( InsertItemLog.Execute( pItemNode ) == false )
						{
							::PrintConsole( "[ERROR] InsertItemLog.Execute \n " ) ;
							
							printf( "[ERROR] InsertItemLog.Execute \n " ) ;
							printf( "item_uid : %d ,code : %d , zone : %d ,from_uid : %d , from : %s ,to_uid : %d , to : %s ,money : %u , type1 : %d type2 : %d \n"
								,pItemNode->item_uid
								,pItemNode->code
								,pItemNode->zone
								,pItemNode->from_uid
								,pItemNode->from
								,pItemNode->to_uid
								,pItemNode->to
								,pItemNode->money
								,pItemNode->type1
								,pItemNode->type2 ) ;		
						}
						
						g_pItemLogQueue->Ready_Put( pItemNode ) ;
					}
					
				}
				break ;
				
			case MSG_NO_CHARACTER :
				{
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[DB_SAVE] CharacLog \n" ) ;
#endif
					
					pCharacNode = g_pCharacLogQueue->Use_Pop() ;
					
					if( pCharacNode ) 
					{

						if ( InsertCharacLog.Execute( pCharacNode ) == false )
						{
							::PrintConsole( "[ERROR] InsertCharacLog.Execute \n " ) ;
						}
						
						g_pCharacLogQueue->Ready_Put( pCharacNode ) ;
					}

					
				}
				break ;
			case MSG_NO_LOGINLOG:
				{
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[DB_SAVE] LoginLog \n" ) ;
#endif
					
					pLoginLogNode= g_pLoginLogQueue->Use_Pop() ;
					
					if( pLoginLogNode ) 
					{
						
						if ( InsertLoginLog.Execute( pLoginLogNode ) == false )
						{
							::PrintConsole( "[ERROR] InsertLoginLog.Execute \n " ) ;
						}
						
						g_pLoginLogQueue->Ready_Put( pLoginLogNode ) ;
					}

				}
				break ;
			
			} // switch
			
			//Sleep( 5 ) ;
			
				
		}
	}

	::PrintConsole("[THREAD_EXIT] %s, %d \n", __FILE__, __LINE__ ) ;

	return 0 ;
}

