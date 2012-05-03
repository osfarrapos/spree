#include "..\stdafx.h"
#include "..\Packet_Queue\Packet_Queue.h"
#include "myServer.h"
#include "..\Timer\TimeOut.h"
#ifdef _IP_FILTER_
#include "..\global\IP_Filter.h"
#endif

#pragma warning(disable:4786)
#include <list>
using namespace std ;
using namespace ND_Net ;

extern _h_ServerInfo *		g_pServerInfo ; 
extern _queue_Packet *		g_pLSPacketQueue  ;
extern _j_UserQueue *		g_pUserQueue ;
extern _j_Templet_Timer *	g_pTimeOut ;
extern CMYTrace *			g_pTrace ;
extern CCapsulateCrypto	*	g_pCapsulateCrypto	;
#ifdef _IP_FILTER_
extern CIpFilter *				g_pIPFilter ;
#endif



bool _h_User::Parsing( DWORD recvbytes )
{

	int pos = 0 ;
	try{
		if( m_pNetIO == NULL || m_pNetIO->pRealBuf == NULL )
		{
			::PrintConsole("[parsing failed] m_pNetIO == NULL \n") ;
			return false ;
		}

		int i = 0 ; 		
		int parsinglength = m_pNetIO->RemainBytes + recvbytes ;
		int packetsize ; // = (unsigned char)m_IORecv.pRealBuf[0] ;

		pos = 1 ;
		
		
		if( parsinglength < 2 )
			return true; 

		_sHeader * pHead ;

		pos = 2 ;
		
		while( 1 ) {
			pHead = (_sHeader*)m_pNetIO->pRealBuf ;	
			packetsize = pHead->sLength ;
			if( packetsize < 2 || packetsize > 1024 )
			{
				::PrintConsole( "[parsing failed] lenth:%d, msg:%u\n", packetsize, (u_char)m_pNetIO->pRealBuf[2] ) ;
				return false ;
			}

			pos = 3 ;

			if( packetsize <= parsinglength ) 
			{
				// 암호를 풀고 capsule 을 벗긴다.
				_Decapsule_info dc_info = g_pCapsulateCrypto->Decapsulate(m_pNetIO->pRealBuf, m_seq ) ;
				if( dc_info.seq != m_seq )
				{
					::PrintConsole( "[ERROR] bed sequince number \n" ) ;
					return false ;
				}
				else
				{
					++m_seq ;
					if( m_seq == 0 ) 
						m_seq = 1 ;
				}			

				pos = 4 ;
				g_pLSPacketQueue->insert( this, dc_info.buf, dc_info.length ) ;
				

				pos = 5 ;
				
				parsinglength -= packetsize ;
				m_pNetIO->pRealBuf += packetsize ;
				
				if( parsinglength == 0 ) {
					m_pNetIO->pRealBuf = m_pNetIO->m_buf ;
					m_pNetIO->RemainBytes = 0 ;
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
			}
			else 
				break ;
		}

		pos = 7 ;
		
		if( parsinglength > 1 ) {
			try {
				memmove( m_pNetIO->m_buf, m_pNetIO->pRealBuf, parsinglength ) ;
				m_pNetIO->RemainBytes = parsinglength ;
				m_pNetIO->pRealBuf = m_pNetIO->m_buf ;
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


//void _h_User::OnConnectionClose()
//{
	//init() ;	
//}


void _h_User::DisConnect()
{
#ifdef _METEL_LOGIN_200608_
	BillingLogout(this->cID, this->MetelBillingInfo.iLoginSN, -1);
	::PrintConsole("BillingLogout(%s, %d) \n", this->cID, MetelBillingInfo.iLoginSN);
#endif //_METEL_LOGIN_200608_

	if( m_pNetIO )
		m_pNetIO->m_server->DisconnectForReuse( m_pNetIO ) ;
}

void _h_User::OnConnectionClose()
{
	if( bIsInMap )
	{
		g_pUserQueue->DeleteStandbyMap( m_id ) ;
	}		
}


int myServer::Worker()
{
	DWORD dwReceived ;
	DWORD dwComKey ;
	ND_Net::COV_UNIT * pUnit ;
	ND_Net::CSocketBuffer * pSocket ;

	char buf[] = "abcdef" ;

	_h_User * pUser =NULL ;

	bool bContinue = true ;

	int pos = 0 ;

	int count = 0 ;

	long errorCount = 0 ;

	//int iLastError ;

	MSG_LOGIN_FAIL msgLoginFail = {0} ;
	msgLoginFail.Header.sLength = sizeof(MSG_LOGIN_FAIL) ;
	msgLoginFail.cMessage = MSG_NO_LOGIN_FAIL ;
		

	_Temp_timerentry g_entry = {0} ;
	g_entry.cWork = _TM_SESSION_OVERTIME_ ;
	

	try {

		while( bContinue )
		{
			dwReceived = 0 ;
			dwComKey = 0 ;
			pUnit = 0 ;
			pSocket = NULL ;

			pos = 0 ;
			
			if( m_iocp.GetStatus( (PDWORD)&dwComKey, &dwReceived, (LPOVERLAPPED*)&pUnit, m_wait_time ) )
			{				
				if( !dwComKey || !pUnit )
				{
					::PrintConsole("if( !dwComKey || !pUnit ) \n") ;
					//break ;
					continue ;
				}

				pos = 1 ;

				//  true 일 때, pUnit 이 NULL 일 리 없다. 
				switch( pUnit->m_iMode ) {
				case mode_recv:
					if( dwReceived == 0 )
					{
						
						pos = 2 ;

						// Discconect
						pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;

						if( pSocket == NULL )
							break ;						
					
						try{
							DisconnectForReuse( pSocket ) ;
						}
						catch (...) {								
							::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
						}
						
					}
					else
					{
						pos = 3 ;
						try{
							pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;

							if( pSocket == NULL )
								break ;
														
							if( pSocket->m_pUser )
							{
								if( pSocket->m_pUser->Parsing( dwReceived ) == false )
								{
									::PrintConsole("[ERROR] parsing error \n") ;
									DisconnectForReuse( pSocket ) ;
									break ;
								}
							}
							else
							{
								::PrintConsole("[ERROR] pSocket->m_pUser == NULL \n") ;
								DisconnectForReuse( pSocket ) ;
								break;
							}

							
							if( false == pSocket->RecvRequest() )
							{
								::PrintConsole("[ERROR] false == pSocket->RecvRequest() code : %d\n", WSAGetLastError()) ;
								DisconnectForReuse( pSocket ) ;
								break ;
							}
						}
						catch (...) {
							::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
						}						
						
					}
					
					break ;

					
				case mode_send_complete:
					pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;
					pUser = (_h_User*)pSocket->m_pUser ;

					try{
						if( pUser )
						{						
							if( ++pUser->cStep >= 2 )
							{
								DisconnectForReuse( pSocket ) ;
							}
						}
					}
					catch (...) {
						::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
					}
					
					//printf( "sendcomplete : addr = %x\n", pSocket ) ;					

					//if( VersionInfo.bServerRun == false )
					//{
					//	DisconnectForReuse( pSocket ) ;						
					//}				
				

					break ;
					

				case mode_accept_complete:
					
					pos = 4 ;
					try {					
					
						pSocket = static_cast<ND_Net::CSocketBuffer*>(pUnit) ;	
						

						if( pSocket == NULL )
							break ;
					

						if( AcceptCompleted( pSocket, dwReceived ) == false )
						{
							::PrintConsole( "[ERROR] LS AcceptCompleted failed \n" ) ;
							break; ;
						}

#ifdef _IP_FILTER_
						if( g_pIPFilter->IsFilterIP( inet_ntoa( pSocket->addr.sin_addr)) )
						{
							::PrintConsole("[IP_FILTER] ip(%s) \n", inet_ntoa( pSocket->addr.sin_addr) ) ;
							DisconnectForReuse( pSocket ) ;
						}
#endif
						

						//AcceptRequest() ;					
						Accept() ;
		
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "Connect - ip:%s , sock : %d \n",inet_ntoa( pSocket->addr.sin_addr), pSocket->socket ) ;
#endif
					}
					catch ( ... ) 
					{						
						::PrintConsole( "[EXCEPTION] mode_accept_complete \n" ) ;
						continue ;
					}
					
					try{

						count = GetCount() ;						

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[INFO] LS accept %d, %s\n"
							, count , inet_ntoa( pSocket->addr.sin_addr ) ) ;
#endif
				
						
						pUser = g_pUserQueue->GetUser() ;
						if( pUser == NULL )
						{			
							::PrintConsole( "[ERROR] pUser== NULL \n" ) ;
							DisconnectForReuse( pSocket ) ;
						}

								
						if( pUser->CreateUser( count, pSocket ) )
						{
							g_entry.pVoid = pUser ;
							g_entry.time = GetTime_By_TimeTSec() + 30 ;
							g_entry.iValue = count ;
							
							g_pTimeOut->send_Message( g_entry ) ;

							//pSocket->SetMode( mode_send ) ;
							//pSocket->SetSendData( buf, strlen(buf) + 1 ) ;
							//pSocket->SendPacket( pSocket->socket ) ;
							
							pUser->iIp = pSocket->addr.sin_addr.s_addr ;							
							if( false == pSocket->RecvRequest() )
							{
								DisconnectForReuse( pSocket ) ;
								::PrintConsole("[ERROR] %s, %d\n", __FILE__, __LINE__ ) ;
							}
							else
							{
								// seq 를 보내준다.
								MSG_SEQUENCE_NUMBER msgStart = {0} ;
								msgStart.Header.sLength = sizeof( MSG_SEQUENCE_NUMBER ) ;
								msgStart.cMessage = MSG_NO_DEQUENCE_NUMBER ;
								msgStart.ucStartSeq = pUser->m_seq ;
								
								
								if( pUser->m_pNetIO->SendRequest( (char*)&msgStart, msgStart.Header.sLength ) != 0 ) 
								{				
									::PrintConsole("[ERROR] Send Request failed (%d)\n", WSAGetLastError()) ;
									DisconnectForReuse( pSocket ) ;
									continue ;
								}
							}
						}
						else
						{
							::PrintConsole( "[ERROR] pUser->CreateUser( count, pSocket ) \n" ) ;
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

						pos = 6 ;

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "DisConnect - ip:%s , sock : %d \n",inet_ntoa( pSocket->addr.sin_addr), pSocket->socket ) ;
#endif

						pos = 7 ;
						if( pSocket->m_pUser )
						{
							pUser = (_h_User*)(pSocket->m_pUser) ;
							pos = 8 ;
							if( pUser )
							{
								pUser->OnConnectionClose() ;
								pUser->reset() ;							
							}
						}

						
						pos = 9 ;
						pool_lock() ;
						try{
							pos = 10 ;
							m_UsingPool.push( pSocket ) ;
						}
						catch (...) {
							::PrintConsole("[EXCEPTION] %s, %d  pos : %d\n", __FILE__, __LINE__, pos  ) ;
						}							
						pool_unlock() ;


					}
					catch(...){
						::PrintConsole("[EXCEPTION] %s, %d pos : %d\n", __FILE__, __LINE__, pos ) ;
					}

					break ;

				case mode_end:
					bContinue = false ;
					break;					
				

				}
			}
			else
			{

				if( !pUnit || !dwComKey )
					continue ;

				pos = 6 ;

				
				int iLastError = WSAGetLastError() ;

				if( pUnit->m_iMode == 2 )
					g_pTrace->OutputString( _TRACE_CLASS_ERROR, "**ls GetStatus Error, Return 0 - Code:%d, mode:%d\n", iLastError, pUnit->m_iMode ) ;
				else
					::PrintConsole("**ls GetStatus Error, Return 0 - Code:%d, mode:%d\n", iLastError, pUnit->m_iMode ) ;
				

				switch( pUnit->m_iMode ) {
				case mode_recv:

					pos = 7 ;				

					pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;	

					try	{
						DisconnectForReuse( pSocket ) ;
					}
					catch (...) {
						::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
					}
					
					
					break;

				case mode_accept_complete:

					pos = 8 ;
				
					pSocket = static_cast<ND_Net::CSocketBuffer*>(pUnit) ;					
					
					try	{
						
						int nEstablishedSeconds = 0;		
						int bytes = sizeof(nEstablishedSeconds);		
						int err = ::getsockopt(pSocket->socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&nEstablishedSeconds, &bytes);
						if (nEstablishedSeconds == 0xffffffff)
						{
							//::PrintConsole( "[ERROR] mode_accept_complete:Dissconnected socket \n") ;

							if( pSocket->m_pUser )
							{
								pUser = (_h_User*)(pSocket->m_pUser) ;
								pos = 8 ;
								if( pUser )
								{
									pUser->OnConnectionClose() ;
									pUser->reset() ;
								}						
							}
							
							pSocket->ReleaseSocket() ;
							pSocket->InitSocket() ;
							
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
												
					}
					catch (...) {
						::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
					}

					
					::InterlockedDecrement( &m_nCurPendingAccept ) ;				
					Accept() ;

					break ;

				case mode_send_complete:
					pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;					

					try	{
						DisconnectForReuse( pSocket ) ;
					}
					catch (...) {
						::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
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

						pos = 6 ;

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "DisConnect - ip:%s , sock : %d \n",inet_ntoa( pSocket->addr.sin_addr), pSocket->socket ) ;
#endif

						g_pErrMsg->LinePrint( "[Disconnect2] ip:%s, sock %d\n", inet_ntoa( pSocket->addr.sin_addr ), pSocket->socket );

						pos = 7 ;
						if( pSocket->m_pUser )
						{
							pUser = (_h_User*)(pSocket->m_pUser) ;
							pos = 8 ;
							if( pUser )
							{
								pUser->OnConnectionClose() ;
								pUser->reset() ;							
							}
						}

						
						pos = 9 ;
						pool_lock() ;
						try{
							pos = 10 ;
							m_UsingPool.push( pSocket ) ;
						}
						catch (...) {
							::PrintConsole("[EXCEPTION] %s, %d  pos : %d\n", __FILE__, __LINE__, pos  ) ;
						}							
						pool_unlock() ;


					}
					catch(...){
						::PrintConsole("[EXCEPTION] %s, %d pos : %d\n", __FILE__, __LINE__, pos ) ;
					}
					break ;
				}
			}
		}
	}
	catch ( ... ) {		
		::PrintConsole("[EXCEPTION] %s, %d, pos:%d \n", __FILE__, __LINE__ , pos) ;
	}


	::PrintConsole("[EXIT THREAD] %s, %d \n", __FILE__, __LINE__ ) ;


	return 0 ;
}




int PSServer::Worker()
{
	DWORD dwReceived ;
	//SOCKET sock ;
	DWORD dwComKey ;
	ND_Net::COV_UNIT * pUnit ;
	ND_Net::CSocketBuffer * pSocket ;


	char buf[] = "abcdef" ;

	_h_User * pUser =NULL ;

	bool bContinue = true ;
	int count ;

	
	MSG_LOGIN_FOREST_LIST * pMsgLoginForestList = g_pServerInfo->GetForestListPacket() ;

	
	try {

		while( bContinue )
		{
			dwReceived = 0 ;
			dwComKey = 0 ;
			pUnit = 0 ;
			pSocket = NULL ;
			
			if( m_iocp.GetStatus( (PDWORD)&dwComKey, &dwReceived, (LPOVERLAPPED*)&pUnit, m_wait_time) )
			{				

				if( !dwComKey || !pUnit )
				{
					printf( "!dwComKey || !pUnit \n " ) ;
					//break ;
					continue ;
				}

				//  true 일 때, pUnit 이 NULL 일 리 없다. 				
				switch( pUnit->m_iMode ) {
				case mode_recv:
					::PrintConsole( "[WARNING] PS Recv from client \n" ) ;
					if( dwReceived == 0 )
					{						
						// Discconect
						pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;
						//printf( "recv : buf = %x\n", pSocket ) ;

						if( pSocket == NULL )
							break ;
						//printf( "Disconnect Reserve\n" ) ;
						try{
							DisconnectForReuse( pSocket ) ;
						}
						catch (...) {
							::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
						}
						
					}
					else
					{
						pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;

						if( pSocket == NULL )
							break ;

						
						//printf( "Disconnect Reserve\n" ) ;
						try{
							DisconnectForReuse( pSocket ) ;
						}
						catch (...) {
							::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
						}
						
					}
					
					break ;

				case mode_send_complete:
					
					pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;

					if( pSocket == NULL )
					{
						printf( "ps: mode_send_complete:  pSocket == NULL \n") ;
						break ;
					}

					try{
						DisconnectForReuse( pSocket ) ;
					}
					catch (...) {
						::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
					}
					//printf( "sendcomplete : addr = %x\n", pSocket ) ;					
					
					break ;
					

				case mode_accept_complete:
					{					
					//printf( "accept %d\n", ::InterlockedIncrement( &g_ncount ) ) ;										
					pSocket = static_cast<ND_Net::CSocketBuffer*>(pUnit) ;
					
					
					if( pSocket == NULL )
						break ;

					try{

						if( AcceptCompleted( pSocket, dwReceived ) == false )
						{
							::PrintConsole( "[ERROR] PS AcceptCompleted failed \n" ) ;
							break; ;
						}
						
						//AcceptRequest() ;
						Accept() ;
						
						count = GetCount() ;
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[INFO] PS accept %u, %s\n"
							, count , inet_ntoa( pSocket->addr.sin_addr ) ) ;
#endif
										
						if( 0 != pSocket->SendRequest( (char*)pMsgLoginForestList, sizeof(MSG_LOGIN_FOREST_LIST) ) )
						{							
							::PrintConsole( "[PS_ERROR] MSG_LOGIN_FOREST_LIST Send Error .. (sock : %d)\n", pSocket->socket ) ;
							DisconnectForReuse( pSocket ) ;
						}
					
					}
					catch (...) {
						::PrintConsole("[EXCEPTION] mode_accept_complete \n") ;
					}					
					
					break ;
					}

				case mode_close_socket:
					//pSocket = static_cast<ND_Net::CSocketBuffer*>(pUnit) ;
					pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;
					//printf( "close :addr = %x\n", pSocket ) ;


					//printf( "User Delete & Socket Release : id = %d\n", static_cast<_h_User*>(pSocket->m_pUser)->get_id() ) ;

					if( pSocket == NULL )
					{
						printf( "ps: mode_close_socket: pSocket == NULL \n") ;
						break ;
					}

					try{

//#ifdef _TRACE_
//						g_pTrace->OutputString( _TRACE_CLASS_SYSTEM," = close \n") ;
//#endif

						
						pool_lock() ;
						try{
							m_UsingPool.push( pSocket ) ;
						}
						catch (...) {
							printf("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
						}						
						pool_unlock() ;

					}
					catch (...) {
						::PrintConsole("[EXECPTION] mode_close_socket \n") ;
					}

					

					break ;

				case mode_end:
					bContinue = false ;
					break;
				}

			}
			else
			{				

				if( !pUnit || !dwComKey )
					continue ;

				int iLastError = WSAGetLastError() ;
				if( pUnit->m_iMode == 2 )
					g_pTrace->OutputString( _TRACE_CLASS_ERROR, "**ps GetStatus Error, Return 0 - Code:%d, mode:%d\n", iLastError, pUnit->m_iMode ) ;
				else
					::PrintConsole("**ps GetStatus Error, Return 0 - Code:%d, mode:%d\n", iLastError, pUnit->m_iMode ) ;
				
				switch( pUnit->m_iMode ) {
				case mode_recv:					
					
					pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;
					try{
						DisconnectForReuse( pSocket ) ;
					}
					catch (...) {
						printf("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
					}							
					
					
					
					break;

				case mode_accept_complete:					

					pSocket = static_cast<ND_Net::CSocketBuffer*>(pUnit) ;					

					{
						int nEstablishedSeconds = 0;		
						int bytes = sizeof(nEstablishedSeconds);		
						int err = ::getsockopt(pSocket->socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&nEstablishedSeconds, &bytes);
						if (nEstablishedSeconds == 0xffffffff)
						{
							//::PrintConsole( "[ERROR] mode_accept_complete:Dissconnected socket \n") ;

							pSocket->ReleaseSocket() ;
							pSocket->InitSocket() ;

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
					}

					::InterlockedDecrement( &m_nCurPendingAccept ) ;				
					Accept() ;
					
					break ;

				case mode_send_complete:
					pSocket = reinterpret_cast<ND_Net::CSocketBuffer*>(dwComKey) ;

					
					try{
						DisconnectForReuse( pSocket ) ;
					}
					catch (...) {
						printf("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
					}			

					break ;
				}
				
				
			}
		}
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
	}

	::PrintConsole("[EXIT THREAD] %s, %d \n", __FILE__, __LINE__ ) ;


	return 0 ;
}






















