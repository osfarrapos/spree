#include "..\stdafx.h"
#include "Network.h"
#include "..\VMManager\VMManager.h"
#include "..\Queue\packet_queue.h"
#include "..\Map\MapManager.h"

extern _queue_Packet * g_pPacketQueue ;
extern _queue_Packet * g_pChatPacketQueue ;
extern _queue_Packet * g_pLoginPacketQueue ;
extern _j_ComQueue * g_pComQueue ;
extern _j_Templet_Timer * g_pTimeOutTimer ;
extern _j_MemeberQueue * g_pMemberQueue ;
extern CMYTrace *		g_pTrace ;


/*#################################################################################

DS_Com

#################################################################################*/

using namespace ND_Net ;


bool _h_DS_Com::Parsing( DWORD recvbytes )
{


	int pos = 0 ;

	ds_lock() ;
	CSocketBuffer * pSocket = m_pNetIO ;
	ds_unlock() ;
	try{
		
		if( pSocket == NULL || pSocket->pRealBuf == NULL )
		{
			::PrintConsole("[parsing failed] m_pNetIO == NULL \n") ;
			return false ;
		}
		
		int i = 0 ; 	
		int parsinglength = pSocket->RemainBytes + recvbytes ;
		DWORD time = timeGetTime() ;
		int packetsize ; // = (unsigned char)m_IORecv.pRealBuf[0] ;
		u_char msgNum  ;
		
		pos = 1 ;
		
		if( parsinglength < 2 )
			return true; 
		
		_sHeader * pHead ;
		
		while( 1 ) {
			pHead = (_sHeader*)pSocket->pRealBuf ;	
			packetsize = pHead->sLength ;
			if( packetsize < 2 || packetsize > _MAX_PACKET_SIZE )
			{
				::PrintConsole( "[parsing failed] lenth:%d, msg:%u\n", packetsize, (u_char)pSocket->pRealBuf[2] ) ;
				return false ;
			}

			pos = 2 ;
			
			if( packetsize <= parsinglength ) {
				
				msgNum = (u_char)pSocket->pRealBuf[2] ;
				
				if( msgNum == MMSG_NO_LOGIN_C_S 
					|| msgNum == MMSG_NO_GM_LOGIN
					|| msgNum == MMSG_NO_LOGOUT_C_S
					|| msgNum == MSG_NO_DISCONNECT )
				{
					g_pLoginPacketQueue->insert( this, pSocket->pRealBuf, packetsize ) ;
				}
				else if( msgNum == MMSG_NO_CHAT 
					|| msgNum == MMSG_NO_CHAT_GM 
					|| msgNum == MMSG_NO_CHAT_WHISPER
					|| msgNum == MMSG_NO_MAIL_SEND_REQ_FROM_GM )
				{				
					g_pChatPacketQueue->insert( this, pSocket->pRealBuf, packetsize ) ;
				}
				else
				{			
					g_pPacketQueue->insert( this, pSocket->pRealBuf, packetsize ) ;			
				}
				
				
				pos = 3 ;
				
				
				parsinglength -= packetsize ;
				pSocket->pRealBuf += packetsize ;

				pos = 4 ;
				
				if( parsinglength == 0 ) {
					pSocket->pRealBuf = pSocket->m_buf ;
					pSocket->RemainBytes = 0 ;
					break ;
				}				

				else if( parsinglength < 2 ){
					break ;
				}

				pos = 6 ;
				
				if( i++ > 50 ) {
					
					::PrintConsole( "[OVERFLOW] Parsing Overflow\n" ) ;
					
					return false ;
				}

				pos = 7 ;
			}
			else 
				break ;
		} // while
		
		if( parsinglength > 1 ) {
			try {
				memmove( pSocket->m_buf, pSocket->pRealBuf, parsinglength ) ;
				pSocket->RemainBytes = parsinglength ;
				pSocket->pRealBuf = pSocket->m_buf ;
			}
			catch (...) {
				::PrintConsole( "[PARSING EXCEPTION] memmove : parsinglength = %d\n", parsinglength ) ;
				
				return false ;
			}
		}
		
		return true ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s, %d, pos:%d \n", __FILE__, __LINE__, pos ) ;
		return false ;
	}	

	return true ;
}


void _h_DS_Com::OnConnectionClose()
{
	int pos = 0 ;

	ds_lock() ;
	try{		
		
		pos = 1 ;
		if( m_pFriendInfo )
		{
			// 로그 오프상태로 전환...
			m_pFriendInfo->Logout() ;
			m_pFriendInfo = NULL ;
		}	// end if	
		
		pos = 2 ;

		CSocketBuffer * pSocket = m_pNetIO ;
		if( pSocket )
		{			
			pSocket->m_pUser = NULL ;
			m_pNetIO = NULL ;
		}
		
		pos = 3 ;	
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] OnConnectionClose pos : %d\n", pos ) ;
	}
	ds_unlock() ;

	g_pComQueue->Insert(this) ;
	
}



int myServer::Worker()
{
	DWORD dwReceived ;
	DWORD dwComKey ;
	ND_Net::COV_UNIT * pUnit ;
	ND_Net::CSocketBuffer * pSocket ;

	char buf[] = "abcdef" ;

	_h_DS_Com * pDSCom =NULL ;
	_Temp_timerentry entry ;

	bool bContinue = true ;

	int pos = 0 ;
	int count =  0 ;
	DWORD dwErrorCode ;

	MMSG_CONNECT_PERM	msgConnect = {0} ;
	msgConnect.sLength = sizeof(msgConnect) ;
	msgConnect.cMessage = MMSG_NO_CONNECT_PERM ;


	while( bContinue )
	{
		dwReceived = 0 ;
		dwComKey = 0 ;
		pUnit = 0 ;
		pSocket = NULL ;
		

		pos = 0 ;

		
		if( m_iocp.GetStatus( (PDWORD)&dwComKey, &dwReceived, (LPOVERLAPPED*)&pUnit ) )
		{

			if( !dwComKey || !pUnit )
			{
				//break ;
				continue ;
			}

			pos = 1 ;

			//  true 일 때, pUnit 이 NULL 일 리 없다. 

			switch( pUnit->m_iMode ) {
			case mode_recv:
				// dwReceived == 0 이면 연결이 끊어진거다.
				if( dwReceived == 0 )
				{
					// Discconect
					pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;
					pos = 2 ;
					
					dwErrorCode = WSAGetLastError() ;
					if( dwErrorCode != WSA_IO_PENDING && dwErrorCode != 0 )
					{
						::PrintConsole( "[SOCKER_ERROR] mode_recv: if( dwReceived == 0 ) sock:%d err : %d \n", pSocket->socket ) ;
					}					
					DisconnectForReuse( pSocket ) ;										
										
				}
				else
				{
					pos = 3 ;

					
					try{
						pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;	

						
						
						
						if( pSocket->m_pUser && pSocket->m_bAccepted ) 
						{
							
							pos = 4 ;
							_h_DS_Com * pUser = (_h_DS_Com*)pSocket->m_pUser ;
							if ( pUser->Parsing( dwReceived ) )
							{								
#ifdef _TRACE_
								pos = 5 ;
								g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[RECV] packet sock No. %d \n", pSocket->socket);
#endif 								
								if( false == pSocket->RecvRequest() ) {
									::PrintConsole( "[SOCK_ERROR] * mode_recv / pSocket->RecvRequest() error sock:%d\n", pSocket->socket ) ;
									DisconnectForReuse( pSocket ) ;
								}
							}
							else {
								pos = 6 ;
								::PrintConsole("[SOCK_ERROR] parsing failed \n " ) ;
								DisconnectForReuse( pSocket ) ;
							}
						}
						else{
							pos = 6 ;
							if( pSocket->m_pUser == NULL )
							{
								::PrintConsole("[SOCK_ERROR] pSocket->m_pUser == NULL \n " ) ;
							}
							if( pSocket->m_bAccepted == false )
							{
								::PrintConsole("[SOCK_ERROR] pSocket->m_bAccepted == false \n " ) ;
							}	
							pos = 7 ;
							
							DisconnectForReuse( pSocket ) ;							
						}							
					}
					catch (...) {
						::PrintConsole("[EXCEPTION] Worker / mode_recv pos.%d\n", pos) ;	

					}
					
					
					
				}
				
				break ;

				
			case mode_send_complete:
				//pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;
				//printf( "sendcomplete : addr = %x\n", pSocket ) ;
				// dwReceived == 0 이면 연결이 끊어진거다.
				if( dwReceived == 0 )
				{
					// Discconect
					pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;
					pos = 2 ;				
					::PrintConsole( "[SOCKER_ERROR] mode_send_complete: if( dwReceived == 0 ) sock:%d \n", pSocket->socket ) ;
					DisconnectForReuse( pSocket ) ;					
					
				}

				break ;
				

			case mode_accept_complete:

				pos = 4 ;
				try {					
					//::PrintConsole( "[INFO] LS accept %d\n", ::InterlockedIncrement( &g_ncount ) ) ;

					pSocket = static_cast<ND_Net::CSocketBuffer*>(pUnit) ;
					
					if( AcceptCompleted( pSocket, dwReceived ) == false )
					{
						::PrintConsole( "[ERROR] AcceptCompleted failed \n" ) ;
						continue ;
					}
					//AcceptRequest() ;

					Accept() ;
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "Connect - ip:%s , sock : %d \n",inet_ntoa( pSocket->addr.sin_addr), pSocket->socket);
#endif 

				}
				catch ( ... ) 
				{
					::PrintConsole( "[EXCEPTION] mode_accept_complete\n" ) ;
					continue ;
				}

				try{

					

					pDSCom = g_pComQueue->GetNode() ;
					
					if( pDSCom )
					{						
						if( pDSCom->CreateUser( GetCount(), pSocket ) )
						{
							entry.pDS_Com = pDSCom ;
							entry.time = timeGetTime() + _ACCEPT_SUCCESS_TIME_ /*3000*/ ;
							g_pTimeOutTimer->send_Message( entry ) ;

							if( false == pSocket->RecvRequest() ) {
								DisconnectForReuse( pSocket ) ;	
								::PrintConsole( "[SOCK_ERROR] mode_accept_complete / pSocket->RecvRequest() error \n" ) ;
								continue ;
							}

							if( 0 != pSocket->SendRequest( (char*)&msgConnect , sizeof(msgConnect) ) )
							{
								::PrintConsole("[ERROR] SendRequest fail \n") ;
								DisconnectForReuse( pSocket ) ;								
								continue ;
							}

#ifdef _TRACE_
							g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, " User obj create success - ip:%s , sock : %d \n",inet_ntoa( pSocket->addr.sin_addr), pSocket->socket);
#endif 							
							
						}
						else{
							::PrintConsole("[ERROR] if( pDSCom->CreateUser( GetCount(), pSocket ) == false ) \n" ) ;
							DisconnectForReuse( pSocket ) ;							
						}
					}
					else{
						DisconnectForReuse( pSocket ) ;						
					}
					
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] Worker thread / mode_accept_complete \n") ;					
				}				

				break ;

			case mode_close_socket:
				pos = 5 ;
				//pSocket = static_cast<ND_Net::CSocketBuffer*>(pUnit) ;
				pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;
				//printf( "close :addr = %x\n", pSocket ) ;					

				if( pSocket == NULL )
					break ;

				try{
					

#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "User Delete & Socket Release \n"  );
#endif 

					pos = 6 ;
					// 로그아웃 시키고 초기화..
					pDSCom = (_h_DS_Com*)pSocket->m_pUser ;
					if( pDSCom )
					{
						pos = 7 ;
						pDSCom->OnConnectionClose() ;
						pDSCom->Release() ;
					}

					pos = 8 ;
					
					
					
					pos = 10 ;
					pSocket->release() ;
					pool_lock() ;
					try{						
						if( pSocket->m_bAccepted == false )
						{							
							m_UsingPool.push( pSocket ) ;
						}
					}
					catch (...) {
						::PrintConsole("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
					}						
					pool_unlock() ;
					
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "DisConnect - ip:%s , sock : %d \n",inet_ntoa( pSocket->addr.sin_addr), pSocket->socket );
#endif 


				}
				catch (...) 
				{
					::PrintConsole("[EXCEPTION] mode_close_socket pos : %d \n", pos ) ;
				}

				break ;

			case mode_end:
				bContinue = false ;
				break;					
			

			default:
				printf("default\n") ;
				break;

			}
		} // if
		else
		{

			if( !pUnit || !dwComKey )
				continue ;
			

			g_pTrace->OutputString( _TRACE_CLASS_ERROR, "**ms GetStatus Error, Return 0 - Code:%d, mode:%d\n", WSAGetLastError(), pUnit->m_iMode ) ;
						
			
			switch( pUnit->m_iMode ) {
			case mode_recv:
				
				pos = 7 ;
				
				pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;

				
				try{
					

					/*
					int nEstablishedSeconds = 0;		
					int bytes = sizeof(nEstablishedSeconds);		
					int err = ::getsockopt(pSocket->socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&nEstablishedSeconds, &bytes);
					if (nEstablishedSeconds == 0xffffffff)
					{
						::PrintConsole(" is disconnected \n" ) ;

						pDSCom = (_h_DS_Com*)pSocket->m_pUser ;
						if( pDSCom )
						{
							pos = 7 ;
							pDSCom->OnConnectionClose() ;
						}
						
						pSocket->ReleaseSocket() ;
						if( pSocket->InitSocket() )
						{
							pSocket->release() ;
							
							if( m_iocp.AssociateSocket( pSocket ) )
							{
								pool_lock() ;
								try{									
									m_UsingPool.push( pSocket ) ;
								}
								catch (...) {
									printf("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
								}						
								pool_unlock() ;
							}
							else
							{
								int iError = WSAGetLastError() ;
								::PrintConsole( " m_iocp.AssociateSocket( pBuf ) == false (err_code :%d)\n", iError ) ;							
							}
							
						}					

					}
					else
					{

						if( pSocket->m_bAccepted == false )
						{
							::PrintConsole("[SOCK_ERROR] mode_recv:pSocket->m_bAccepted == false \n " ) ;
						}	

						::PrintConsole(" is connected \n" ) ;
						DisconnectForReuse( pSocket ) ;
					}*/					
					DisconnectForReuse( pSocket ) ;
				}
				catch (...) {
					printf("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
				}
				
				
				break;
				
			case mode_accept_complete:
				
				pos = 8 ;
				
				
				pSocket = static_cast<ND_Net::CSocketBuffer*>(pUnit) ;						
				
				try
				{
					/*
					int nEstablishedSeconds = 0;		
					int bytes = sizeof(nEstablishedSeconds);		
					int err = ::getsockopt(pSocket->socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&nEstablishedSeconds, &bytes);
					if (nEstablishedSeconds == 0xffffffff)
					{
						//::PrintConsole( "[ERROR] mode_accept_complete:Dissconnected socket \n") ;

						pDSCom = (_h_DS_Com*)pSocket->m_pUser ;
						if( pDSCom )
						{
							pos = 7 ;
							pDSCom->OnConnectionClose() ;
						}
						
						pSocket->ReleaseSocket() ;
						pSocket->InitSocket() ;
						pSocket->release() ;
						
						if( m_iocp.AssociateSocket( pSocket ) )
						{
							pool_lock() ;
							try{								
								m_UsingPool.push( pSocket ) ;
							}
							catch (...) {
								printf("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
							}						
							pool_unlock() ;
						}
						else
						{
							int iError = WSAGetLastError() ;
							::PrintConsole( " m_iocp.AssociateSocket( pBuf ) == false (err_code :%d)\n", iError ) ;							
						}
						
					}		
					else			
					{
						// nEstablishedSeconds --> 커넥션 이루어진 때로부터 현재까지 경과한 초단위 시간								
						::PrintConsole( "[ERROR] mode_accept_complete:Already connected socket \n") ;
						DisconnectForReuse( pSocket ) ;
					}
					*/

					::PrintConsole("[ERROR] Accept error (code : %d) \n", GetLastError() ) ;

					DisconnectForReuse( pSocket ) ;

					::InterlockedDecrement( &m_nCurPendingAccept ) ;				
					Accept() ;
					
				}
				catch(...)
				{
					printf("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
				}
				
				
					

				break;


			case mode_send_complete :
				
				pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;
								
				try{
					/*
					int nEstablishedSeconds = 0;		
					int bytes = sizeof(nEstablishedSeconds);		
					int err = ::getsockopt(pSocket->socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&nEstablishedSeconds, &bytes);
					if (nEstablishedSeconds == 0xffffffff)
					{
						::PrintConsole(" is disconnected \n" ) ;
						
						pDSCom = (_h_DS_Com*)pSocket->m_pUser ;
						if( pDSCom )
						{
							pos = 7 ;
							pDSCom->OnConnectionClose() ;
						}
						
						
						pSocket->ReleaseSocket() ;
						pSocket->InitSocket() ;
						pSocket->release() ;
						
						if( m_iocp.AssociateSocket( pSocket ) )
						{
							pool_lock() ;
							try{								
								m_UsingPool.push( pSocket ) ;
							}
							catch (...) {
								printf("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
							}						
							pool_unlock() ;
						}
						else
						{
							int iError = WSAGetLastError() ;
							::PrintConsole( " m_iocp.AssociateSocket( pBuf ) == false (err_code :%d)\n", iError ) ;	
						}
						
						
					}
					else
					{
						if( pSocket->m_bAccepted == false )
						{
							::PrintConsole("[SOCK_ERROR] mode_recv:pSocket->m_bAccepted == false \n " ) ;
						}	
						::PrintConsole(" is connected \n" ) ;
						DisconnectForReuse( pSocket ) ;
					}
					*/
					DisconnectForReuse( pSocket ) ;
					::PrintConsole("error : mode_send_complete \n") ;
					
				}
				catch (...) {
					printf("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
				}					
				
				
				break ;
				
			}// switch
			

		}// else

	} // while
	


	::PrintConsole(NULL, "worker thread exit","worning",MB_OK) ;

	return 0 ;
}
