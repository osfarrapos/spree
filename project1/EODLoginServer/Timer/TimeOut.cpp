#include "..\stdafx.h"
#include "..\Packet_Queue\Packet_Queue.h"
#include "..\Global\Global_Function.h"
#include "TimeOut.h"


extern _j_queue_UDP_Packet *	g_pUDPSendPacketQ ;
extern _h_ServerInfo *		g_pServerInfo ; 


DWORD WINAPI th_TimeOut( LPVOID lp ) 
{
	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;

	static int threadCount = 0 ;


	if( threadCount++ > 0 )
	{
		return 0 ;
	}

	
	LARGE_INTEGER li = {0, } ;
	
		
	_j_Templet_Timer * pTimer ;
	pTimer = (_j_Templet_Timer*)lp ;

	char	ip[17] ;	

	//_Packet * pPacket ;
	_Temp_timerentry entry = {0} ;
	_h_User * pUser ;
	DWORD curTime ;
				
	printf( "  => Start Timer\n" ) ;
	
	SetWaitableTimer( hTimer, &li, 50, NULL, NULL, TRUE ) ;
	
	while(pTimer->bThreadRun) {
		WaitForSingleObject( hTimer, INFINITE ) ;
	
		curTime = GetTime_ByIntSec() ;
		
		while ( pTimer->Get_TimeMessage( curTime, entry ) == true ) 
		{
			
			switch( entry.cWork ) {
				/*
			case _TM_TEST :
				entry.time = GetTime_ByIntSec() + 3 ;
				printf("entry value (%d)\n", entry.iValue++ ) ;
				printf("s entry time (%d)\n", entry.time ) ;
				printf("c entry time (%d)\n", curTime ) ;
				pTimer->send_Message( entry ) ;
				break; 
				*/
			case _TM_SESSION_OVERTIME_:
				pUser = (_h_User*)entry.pVoid ;
				if( pUser && pUser->m_id == entry.iValue )	// 접속만 하고 아직까지 로그인을 안한 사용자다.. 끊어버려라.
				{
#ifdef _TRACE_
					ip_char( pUser->iIp, ip ) ;
					g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[KICK OUT] Overtime connection client (IP:%s)\n", ip ) ;
#endif

					pUser->DisConnect() ;
				}			
				break;
			case _TM_DS_INFO_REQ:
				{				
				
					UMSG_LOGIN_INFO_REQ_LS_DS	msgLoginInfoReq = {0} ;
					msgLoginInfoReq.cMessage = UMSG_LOGIN_INFO_REQ_LS_DS_NUM ;
					msgLoginInfoReq.iKey = PASS_KEY ;
					msgLoginInfoReq.cServerNum = (char)entry.iValue ;

					g_pUDPSendPacketQ->insert( (SOCKADDR_IN*)entry.pVoid , (char*)&msgLoginInfoReq, sizeof(msgLoginInfoReq) ) ;

					// n 초 주기로 반복해서 요청..
					entry.time = GetTime_ByIntSec() + 3 ;
					pTimer->send_Message( entry ) ;	

					if( g_pServerInfo->m_ForestCheckFlag[msgLoginInfoReq.cServerNum] < -21 )
					{
						g_pServerInfo->SetForestState( _FOREST_STATE_OFF, msgLoginInfoReq.cServerNum ) ;
						g_pServerInfo->m_ForestCheckFlag[msgLoginInfoReq.cServerNum] = 0 ;
					}

					--g_pServerInfo->m_ForestCheckFlag[msgLoginInfoReq.cServerNum] ;				
				}				
				break;
			case _TM_DS_CHECK :
				{

				}
			default:
				;
			}
		}
	}
	
	return 0 ;
}






















