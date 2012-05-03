#include "..\stdafx.h"

//#include "..\MAPManager\MAPManager.h"
#include "..\Network\LogSvrLink.h"


// S2S log
extern bool g_Threading ;
#define _LOG_SEND_BUF_SIZE	4096 
DWORD WINAPI th_LogSendthread( LPVOID lp )
{
	CLogSvr_Com * pLogSvrCom = (CLogSvr_Com*)lp ;
	
	MSG_CHARACTER msgCharacLog = {0} ;
	msgCharacLog.sLength = sizeof(MSG_CHARACTER) ;
	msgCharacLog.cMessage = MSG_NO_CHARACTER ;

	MSG_SERVER_STATE msgServerLog  = {0} ;
	msgServerLog.sLength = sizeof(MSG_SERVER_STATE) ;
	msgServerLog.cMessage = MSG_NO_SERVER_STATE ;

	MSG_ITEM msgItem = {0} ;
	msgItem.sLength = sizeof( MSG_ITEM ) ;
	msgItem.cMessage = MSG_NO_ITEM ;

	MSG_LOGINLOG msgLoginLog = {0} ;
	msgLoginLog.sLength = sizeof( MSG_LOGINLOG ) ;
	msgLoginLog.cMessage = MSG_NO_LOGINLOG ;

	char cSendBuf[_LOG_SEND_BUF_SIZE] = {0} ;
	int iCurPoint = 0 ;
	int iSend = 0 ;

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	LARGE_INTEGER li = {0} ;

	SetWaitableTimer( hTimer, &li, 500, NULL, NULL, TRUE ) ;

	_CharacLog * pCharacLog ;
	_ServerState * pServerLog ;
	_ItemLog * pItemLog ;
	_LoginLog * pLoginLog ;

	bool bConnected = false;
	int count = 0;

	if( pLogSvrCom->get_sock() )
		bConnected = true;
	
	while (g_Threading) 
	{
		pCharacLog = NULL ;
		pServerLog = NULL ;
		pItemLog = NULL ;		

		WaitForSingleObject( hTimer, INFINITE ) ;
		iCurPoint = 0 ;
		TRY
		{
			if( false == bConnected )
			{
				++count ;
				if( count > 500 )
				{
					if( pLogSvrCom->RestoreConnection() )
					{
						bConnected = true;
					}
					else
					{
						count = 0;
						continue;
					}
				}
				else
					continue;
			}

			while ( pLogSvrCom->m_CharacLogQueue.GetUseCount() ) 
			{
				if( (_LOG_SEND_BUF_SIZE - iCurPoint) < sizeof(MSG_CHARACTER) )
				{
					break ;
				}

				pCharacLog = pLogSvrCom->m_CharacLogQueue.Use_Pop() ;
				if( pCharacLog )
				{
					// send to log_svr			
					memcpy( &cSendBuf[iCurPoint], &msgCharacLog, 3 /*header size*/) ;
					memcpy( &cSendBuf[iCurPoint+3], pCharacLog, sizeof(_CharacLog) ) ;			
					iCurPoint = iCurPoint + 3 + sizeof(_CharacLog) ;

					pLogSvrCom->m_CharacLogQueue.Ready_Put( pCharacLog ) ;
				}		
				
			}	

			while ( pLogSvrCom->m_ServerLogQueue.GetUseCount() ) 
			{
				if( (_LOG_SEND_BUF_SIZE - iCurPoint) < sizeof(MSG_SERVER_STATE) )
				{
					break ;
				}

				pServerLog = pLogSvrCom->m_ServerLogQueue.Use_Pop() ;
				if( pServerLog )
				{
					// send to log_svr			
					memcpy( &cSendBuf[iCurPoint], &msgServerLog, 3 /*header size*/) ;
					memcpy( &cSendBuf[iCurPoint+3], pServerLog, sizeof(_ServerState) ) ;			
					iCurPoint = iCurPoint + 3 + sizeof(_ServerState) ;

					pLogSvrCom->m_ServerLogQueue.Ready_Put( pServerLog ) ;
				}			
			}	

			while ( pLogSvrCom->m_ItemLogQueue.GetUseCount() ) 
			{
				if( (_LOG_SEND_BUF_SIZE - iCurPoint) < sizeof(MSG_ITEM) )
				{
					break ;
				}

				pItemLog = pLogSvrCom->m_ItemLogQueue.Use_Pop() ;
				if( pItemLog ) 
				{
					// send to log_svr			
					memcpy( &cSendBuf[iCurPoint], &msgItem, 3 /*header size*/) ;
					memcpy( &cSendBuf[iCurPoint+3], pItemLog, sizeof(_ItemLog) ) ;			
					iCurPoint = iCurPoint + 3 + sizeof(_ItemLog) ;

					pLogSvrCom->m_ItemLogQueue.Ready_Put( pItemLog ) ;
				}			
			}	

			while ( pLogSvrCom->m_LoginLogQueue.GetUseCount() ) 
			{
				if( (_LOG_SEND_BUF_SIZE - iCurPoint) < sizeof(MSG_LOGINLOG) )
				{
					break ;
				}

				pLoginLog = pLogSvrCom->m_LoginLogQueue.Use_Pop() ;
				if( pLoginLog ) 
				{
					// send to log_svr			
					memcpy( &cSendBuf[iCurPoint], &msgLoginLog, 3 /*header size*/) ;
					memcpy( &cSendBuf[iCurPoint+3], pLoginLog, sizeof(_LoginLog) ) ;
					iCurPoint = iCurPoint + 3 + sizeof(_LoginLog) ;

					pLogSvrCom->m_LoginLogQueue.Ready_Put( pLoginLog ) ;
				}			
			}	
		

			if ( iCurPoint )
			{
				iSend = pLogSvrCom->send_request( iCurPoint, cSendBuf ) ;			
				if( iSend == SOCKET_ERROR ) {
					::PrintConsole("[ERROR] pLogSvrCom->send_request %d\n", WSAGetLastError() );
					bConnected = false;
				}
			}
		}
		CATCH_ALL
		{
			g_pSystemLog->LogFileWrite( "[EXCEPTION] LogSVR Thread\n" ) ;
			printf( "[EXCEPTION] LogSVR Thread\n" ) ;
			pLogSvrCom = (CLogSvr_Com*)lp ;
		}
	}

	::PrintConsole("DWORD WINAPI th_LogSendthread( LPVOID lp ) Exit \n");
	printf("DWORD WINAPI th_LogSendthread( LPVOID lp ) Exit \n");	
	return 0 ;
}