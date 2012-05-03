#include "..\stdafx.h"
#include "..\Network\Network.h"
#include "..\VMManager\VMManager.h"
#include "..\Queue\Packet_Queue.h"
#include "..\Global\Global_LogFile.h"
#include "..\Map\MapManager.h"

extern _queue_Packet * g_pPacketQueue ;
extern _h_CompletionPort *	g_pIOCP	;	// IOCP
extern _j_GlobalLog * g_pSystemLog ;
extern _DSCom_Queue *	g_pDSComQueue ;

extern HANDLE				g_hCreateEvent ;
extern bool g_pThreadRunning ;
extern char g_ip[17] ;


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


	int nReturn = 0;
	SOCKADDR_IN gsAddr ;
	int size = sizeof( gsAddr ) ;
	
			
	// listen socket create
	sListen = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED ) ;
	if( sListen == INVALID_SOCKET )
	{
		g_pSystemLog->LogFileWrite( "[FAIL] Create socket fail : %d\n", WSAGetLastError() ); 
		goto label_Accept_end ;
	}

	// addr...
	sockAddr.sin_family = AF_INET ;
	sockAddr.sin_port = htons( accport ) ;
	sockAddr.sin_addr.s_addr = inet_addr( g_ip ) ;
	printf( "[info] tcp binding ip (%s) \n" ,g_ip ) ;


	if( bind( sListen, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR_IN) ) == SOCKET_ERROR)
	{
		g_pSystemLog->LogFileWrite( "[FAIL] Bind listen socket fail : %d\n", WSAGetLastError() ); 
		goto label_Accept_end ;
	}

	if(listen(sListen, SOMAXCONN) != 0)		// 백로그 값을 올리는게 날지....
	{
		g_pSystemLog->LogFileWrite( "[FAIL] listen() fail : %d\n", WSAGetLastError() ); 
		goto label_Accept_end ;
	}

	// socket option settting...
	// accept 의 리턴 소켓은 리슨 소켓의 속성을 이어받는다. 그래서 리슨 소켓의 속성을 설정한다. 
	// 근데.. 진짜냐???
	nZero = 0 ;
	nRet = setsockopt(sListen, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero)) ;
	if (SOCKET_ERROR == nRet) 
	{ 
		g_pSystemLog->LogFileWrite( "[FAIL] setsockopt(SNDBUF) : %d\n", WSAGetLastError() );
		goto label_Accept_end ;
	}
	nZero = 0 ;
	nRet = setsockopt(sListen, SOL_SOCKET, SO_RCVBUF, (char *)&nZero, sizeof(nZero)) ;
	if (SOCKET_ERROR == nRet) 
	{
		g_pSystemLog->LogFileWrite( "[FAIL] setsockopt(SO_RCVBUF) : %d\n", WSAGetLastError() );
		goto label_Accept_end ;
	}

	
	// socket option setting end...
	
	//SetEvent(g_hCreateEvent) ;
	
	while( g_pThreadRunning )
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

				nRet = 0 ;

				pDSCom = g_pDSComQueue->GetDSCom() ;
				if( !pDSCom ) 

				{
					::PrintConsole( "[ERROR] pDSCom == NULL \n") ;
					continue ;
				}
				
				pDSCom->ConnectCom(sGS) ;		
				

								
				// IOCP handle 과 OV구조체 연결...
				if(!g_pIOCP->AssociateSocket(&pDSCom->m_RecvOV)) {
					::PrintConsole( "[ERROR] Associate : code = %d\n", GetLastError() ) ;
					closesocket(sGS);
					continue;
				}

				nRet = 4 ;

				// WSARecv 로 최초 작업 시작을 걸어준다..						
				if(!pDSCom->m_RecvOV.RecvPacket( sGS )) {
					::PrintConsole("[ERROR] Accetp Thread : Recv : code = %d\n", GetLastError() ) ;
					closesocket(sGS);
					continue;
				}

				nRet = 5 ;
			}
			catch (...) {
				::PrintConsole("[EXCEPTION] th_AcceptThread - 'OV Create' or  'g_pIOCP->AssociateSocket' (pos.%d)\n", nRet);
				DWORD i = GetLastError() ;
				continue;
			}
			

			::PrintConsole("[INFO] Client Connect success! (IP:%s) \n", inet_ntoa(gsAddr.sin_addr));			
			
            
		} // else
	} // while

	closesocket( sListen ) ;

label_Accept_end:
	::PrintConsole("DWORD WINAPI th_AcceptThread( LPVOID lp ) Exit \n") ;
	printf( "[INFO] th_AcceptThread Return\n " ) ;

	_endthreadex(0) ;

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
	MSG_DISCONNECT  pkDisconnect;
	pkDisconnect.sLength = sizeof( MSG_DISCONNECT ) ;
	pkDisconnect.cMessage = MSG_NO_DISCONNECT ;
	


	bool bParse = false ;
	char ip_addr[16] = {0};


	while( g_pThreadRunning ) {	
		try{
			bReturn = g_pIOCP->GetStatus( &CompKey, &dwNumBytes, (WSAOVERLAPPED**)&pOverlapped, INFINITE ) ;
				
			pRecv_ov = (_h_Recv_Overlapped*)pOverlapped ;		
			sRecv_sock = pRecv_ov->m_socket;			
		}
		catch (...) {
			::PrintConsole( "[EXCEPTION] g_pIOCP->GetStatus()\n" ) ;			
			continue;
		}
		
				
		if( bReturn ) {		// Operation Success
			if( pOverlapped->m_bMode ) {	// Is recv mode	..				
				if( dwNumBytes > 0 ) {
					try{	//parsing
						bParse = pRecv_ov->Parsing(g_pPacketQueue, dwNumBytes);						

						//pRecv_ov->reset();

						if( bParse == false ) {	//parsing fail
							//on_state() ;						
							::PrintConsole( "  ### put Disconnect (Parsing Error) : %u\n", pRecv_ov->m_socket ) ;						
							pkDisconnect.pDSComm = pRecv_ov->m_pDSComm ;		
							pRecv_ov->pRealBuf = pRecv_ov->m_pBuf ;
							memcpy(pRecv_ov->m_pBuf, (char*)&pkDisconnect, sizeof(pkDisconnect));							

							g_pPacketQueue->insert( pRecv_ov, sizeof(pkDisconnect) ) ;

							//off_state() ;						
							continue ;
						}

						if(!pRecv_ov->RecvPacket( sRecv_sock )){	// recv				
							::PrintConsole("[ERROR] Recv Thread : code = %d\n", GetLastError() ) ;
							closesocket(sRecv_sock);
							
							pkDisconnect.pDSComm = pRecv_ov->m_pDSComm ;
							pRecv_ov->pRealBuf = pRecv_ov->m_pBuf ;
							memcpy(pRecv_ov->m_pBuf, (char*)&pkDisconnect, sizeof(pkDisconnect));
							
							g_pPacketQueue->insert( pRecv_ov, sizeof(pkDisconnect) ) ;


							continue;
						}
					}
					catch (...) {
						::PrintConsole( "[EXCEPTION] DB-Call LoginPro\n" ) ;
						continue;
					}					
					
				}
				else {	// recv fale

					try{
						memset(ip_addr, 0, 16);
						pDSComm = (_h_DS_Com *)pRecv_ov->m_pDSComm ;						
						
						if ( pDSComm ) {
							ip_char(pDSComm->m_sockAddr.sin_addr.S_un.S_addr, ip_addr);											
							::PrintConsole( "[SO_FAIL] Recv error(Close socket) : socket = %u, ip = %s \n", pRecv_ov->m_socket, ip_addr ) ;
							
							closesocket(sRecv_sock);						
							
							pkDisconnect.pDSComm = pRecv_ov->m_pDSComm ;
							pRecv_ov->pRealBuf = pRecv_ov->m_pBuf ;
							memcpy(pRecv_ov->m_pBuf, (char*)&pkDisconnect, sizeof(pkDisconnect));

							g_pPacketQueue->insert( pRecv_ov, sizeof(pkDisconnect) ) ;

						}
						
				
						continue;
					}
					catch (...) {
						::PrintConsole( "[EXCEPTION] OV release \n" ) ;
						continue;
					}				
					
				}
			}
			else {	// Is send mode...				
				if( dwNumBytes > 0 ) 
				{
					pDSComm = (_h_DS_Com *)pRecv_ov->m_pDSComm;					

					try{
						if( pDSComm->send_complete(dwNumBytes) == false ) 
							::PrintConsole("[SO_ERR]send_complete false\n" ) ;
					}
					catch (...) {
						::PrintConsole( "[EXCEPTION] pDSComm->send_complete()\n" ) ;
						continue;
					}					
					
				}
				else // send fail (dwNumBytes <= 0)
				{
					printf( "send fail \n" ) ;
				}
			}
		}


		else if(!bReturn) {	// GetStatus == false
			if( pOverlapped ) {					
				if( pOverlapped->m_bMode ) {		// true : Is read mode

					try{
						closesocket(sRecv_sock);
						memset(ip_addr,0,16);
						pDSComm = (_h_DS_Com *)pRecv_ov->m_pDSComm ;						
						ip_char(pDSComm->m_sockAddr.sin_addr.S_un.S_addr, ip_addr);						
						::PrintConsole( "[SO_FAIL] Return 0 :1 recv thread socket error (ErrorCode: %d, ip: %s\n", WSAGetLastError(),ip_addr ) ;
												
						pkDisconnect.pDSComm = pRecv_ov->m_pDSComm ;
						pRecv_ov->pRealBuf = pRecv_ov->m_pBuf ;
						memcpy(pRecv_ov->m_pBuf, (char*)&pkDisconnect, sizeof(pkDisconnect));
						
						g_pPacketQueue->insert( pRecv_ov, sizeof(pkDisconnect) ) ;
						
					
						//delete pRecv_ov->m_pDSComm;
						//delete pRecv_ov;
					}
					catch (...) {
						::PrintConsole( "[EXCEPTION] OV release (DISCONNECT) \n" ) ;						
					}

					
				}

			}
			else {									
				::PrintConsole( "Return 0 :2 recv thread socket error : %d\n", WSAGetLastError() ) ;
			}			
		}		
	}

	::PrintConsole("DWORD WINAPI th_Recvthread( LPVOID lp ) Exit \n") ;

	g_pSystemLog->LogFileWrite( "[EXIT] th_RecvThread Return\n " ) ;	
	return 0 ;
}






















