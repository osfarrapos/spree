#include "..\stdafx.h"


#include "..\Network\Network.h"
#include "..\Queue\Packet_Queue.h"
#include "..\Global\Global_LogFile.h"
#include "..\Global\Global_Log_Packet.h"
#include "..\Map\MapManager.h"
#include "..\Threads\Backup_Thread.h"
#include "..\Global\Trace.h"



// Packet Ã³¸® Functions
bool GSConnect(char cServerNum, _h_DS_Com * dsCom) ;


extern _queue_Packet *	g_pPacketQueue ;
extern _j_GlobalLog *	g_pSystemLog ;
extern _j_Templet_Timer * g_pLogWriteTimer ;

extern CMYTrace *		g_pTrace ;

extern bool g_pThreadRunning ;

extern CTQueue<_ServerState> * g_pServerStateQueue ;
extern CTQueue<_ItemLog> * g_pItemLogQueue ;
extern CTQueue<_CharacLog> * g_pCharacLogQueue ;
extern CTQueue<_LoginLog> * g_pLoginLogQueue ;



void Disconnect(_h_DS_Com * ds_com)
{
	try{
		MSG_DISCONNECT  pkDisconnect;
		pkDisconnect.sLength = sizeof( MSG_DISCONNECT ) ;
		pkDisconnect.cMessage = MSG_NO_DISCONNECT ;
		pkDisconnect.pDSComm = ds_com ;
		ds_com->m_RecvOV.pRealBuf = ds_com->m_RecvOV.m_pBuf ;
		memcpy(ds_com->m_RecvOV.m_pBuf, (char*)&pkDisconnect, sizeof(pkDisconnect));							

		g_pPacketQueue->insert( &ds_com->m_RecvOV, sizeof(pkDisconnect) ) ;

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] void Disconnect(_h_DS_Com * ds_com) \n") ;
	}

}


DWORD	WINAPI	th_WorkThread( LPVOID lp)
{

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;	
	LARGE_INTEGER li = {0, } ;
	

	int			iRes = 0 ;
	short		sRes = 0 ;
	u_short		usRes = 0 ;
	bool		bRes = 0 ;


	_packet *	pack = NULL ;
	int			Mainmsg = 0 ;	
	_h_DS_Com * pDSComm = NULL ;



	SetWaitableTimer( hTimer, &li, 50, NULL, NULL, TRUE ) ;

	while (g_pThreadRunning) {		
		

		if( g_pPacketQueue->empty() ) {

			WaitForSingleObject( hTimer, INFINITE ) ;
			continue ;
		}
		else {
			pack = g_pPacketQueue->get() ;
			if( pack == NULL ) 
			{
				WaitForSingleObject( hTimer, INFINITE ) ;
				continue ;
			}
			else {
				Mainmsg = (u_char)pack->buf[2] ;								
			}
		}
		
		switch( Mainmsg ) 
		{
		case MSG_NO_DISCONNECT :
			MSG_DISCONNECT * pMsgDisconnect ;
			pMsgDisconnect = (MSG_DISCONNECT*)pack->buf ;

			pDSComm = pMsgDisconnect->pDSComm ;

			if( pDSComm ) {
				g_pSystemLog->LogFileWrite("[DISCONNECT] DisConnect (CName:%s) \n", inet_ntoa( pDSComm->m_sockAddr.sin_addr ) ) ;
				::PrintConsole("[DISCONNECT] DisConnect (CName:%s) \n", inet_ntoa( pDSComm->m_sockAddr.sin_addr ) ) ;

				try {
					pDSComm->releaseDSCom() ;
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] Disconnect work \n") ;
					continue ;
				}
			}
			
			break ;

		case MSG_NO_SERVER_STATE :
			{
#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE,"[INFO] RECV MSG_NO_SERVER_STATE \n") ;			
#endif
				_Temp_timerentry en ;

				MSG_SERVER_STATE * pMsgServerState ;

				pMsgServerState = (MSG_SERVER_STATE *)pack->buf ;	
			
				_ServerState * pServerState =  g_pServerStateQueue->Ready_Pop() ;
				if( pServerState )
				{
					memcpy(pServerState, &pMsgServerState->log_ServerState , sizeof(_ServerState) ) ;
					
					g_pServerStateQueue->Use_Put( pServerState ) ;
					
					
					en.pQueue = g_pServerStateQueue ;				
					en.Section = MSG_NO_SERVER_STATE ;
					
					g_pLogWriteTimer->send_Message( en ) ;
				}
				
			}

			break;

		case MSG_NO_ITEM :
			{
#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE,"[INFO] RECV MSG_NO_ITEM \n") ;
#endif
				_Temp_timerentry en ;
				
				MSG_ITEM * pMsgItem ;
				
				pMsgItem = (MSG_ITEM *)pack->buf ;	
				

				_ItemLog * pItemlLog =  g_pItemLogQueue->Ready_Pop() ;
				if( pItemlLog )
				{
					memcpy(pItemlLog, &pMsgItem->log_Item , sizeof(_ItemLog) ) ;			
					
					g_pItemLogQueue->Use_Put( pItemlLog ) ;
					
					
					en.pQueue = g_pItemLogQueue ;				
					en.Section = MSG_NO_ITEM ;
					
					g_pLogWriteTimer->send_Message( en ) ;
				}			
				
			}
			break;


		case MSG_NO_CHARACTER :
			{
#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE,"[INFO] RECV MSG_NO_CHARACTER \n") ;
#endif

				_Temp_timerentry en ;
				
				MSG_CHARACTER * pMsgCharac ;
				
				pMsgCharac = (MSG_CHARACTER *)pack->buf ;	
				

				_CharacLog * pCharacLog =  g_pCharacLogQueue->Ready_Pop() ;
				if( pCharacLog )
				{
					memcpy(pCharacLog, &pMsgCharac->log_Charac , sizeof(_CharacLog) ) ;		
					
					g_pCharacLogQueue->Use_Put( pCharacLog ) ;
					
					
					en.pQueue = g_pCharacLogQueue ;				
					en.Section = MSG_NO_CHARACTER ;
					
					g_pLogWriteTimer->send_Message( en ) ;
				}
				
			}
			break;
			
		case MSG_NO_LOGINLOG :
			{
#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE,"[INFO] RECV MSG_NO_LOGINLOG \n") ;
#endif
				
				_Temp_timerentry en ;
				
				MSG_LOGINLOG * pMsgLogin ;
				
				pMsgLogin = (MSG_LOGINLOG *)pack->buf ;	
				
				
				_LoginLog * pLoginLog =  g_pLoginLogQueue->Ready_Pop() ;
				if( pLoginLog )
				{
					memcpy(pLoginLog, &pMsgLogin->log_Login , sizeof(_LoginLog) ) ;		
					
					g_pLoginLogQueue->Use_Put( pLoginLog ) ;
					
					
					en.pQueue = g_pLoginLogQueue ;				
					en.Section = MSG_NO_LOGINLOG ;
					
					g_pLogWriteTimer->send_Message( en ) ;
				}
			}
			break;



		} //switch( Mainmsg ) 
	}

		


	::PrintConsole( "[WARNING] th_WorkThread Exit \n") ;
	g_pSystemLog->LogFileWrite( "[WARNING] th_WorkThread Exit \n") ;
	return 0 ;
		
}