
#include "..\stdafx.h"
//#include "..\Global\DS_Packets.h"
#include "..\MAPManager\MAPManager.h"
#include "..\ole_db_module\db_command.h"
#include "..\Network\Network.h"
#include "..\Network\LogSvrLink.h"
#include "..\Packet_Queue\Packet_Queue.h"
#include "..\Global\Global_Define.h"
#include "..\Global\Global_ServerInfo.h"
//#include "..\Global\GMS_Packets.h"
#include "..\Global\Global_LogFile.h"
#include "..\Global\Global_UniqueID.h"
#include "..\Global\Global_Function.h"
#include "..\Billing\BillingSvrLink.h"

#include "../Cs/Castle.h"


#include <MY_CRYPTO.H>
#ifdef _CHARACLIST_DELETE_CHARACTER_20061214_
#include "..\Global\tcp_packets_party.h"
extern COrganizationManager *	g_pOrganizationManager ;
#endif

//#include "..\NDExceptionReport\NDExceptionReport.h"

//extern NDExceptionReport*  g_pNDExceptionReport ;

extern u_int					g_uiCompanyCode ;

extern _j_GlobalLog *			g_pGMStLog;

extern _j_ServerInfo			g_ServerInfo;		// 서버 정보
//extern _j_ShowText				g_ShowText;

extern _j_AccMAPManager *	g_pAccMapManager;
extern _j_queue_UDP_Packet *	g_pUDPSendPacketQ ;
extern _j_queue_UDP_Packet *	g_pUDPRecvPacketQ ;
extern _j_queue_UDP_Packet *	g_pUDPLoinPacketQ ;

//extern _j_UserLoginLog		g_UserLoginLog;				// 유저 접속 성공 로그파일
//extern _j_UserLoginFailLog	g_UserLoginFailLog;			// 유저 접속 실패 로그 파일
extern _j_UniqueIDQueue *		g_pUniqueIDQueue ;

// S2S log
//extern CTQueue<_CharacLog> * g_pCharacLogQueue ;
extern CLogSvr_Com *			g_pLogSvrCom ;	
extern bool g_Threading ;
extern bool g_MainThreading ;
extern int g_MaxConnection ;

extern bool g_bWanCon ;

// mem save 
extern CUserMemSaveTimer *		g_pMemSaveTimer ;

// oledb
extern CDB_Connector	*		g_pDBCon ;
extern CDB_Connector *			g_pUSERDBCon ;

extern _j_BackupReady *		g_pBackupReady ;

extern _DSCom_Queue *		g_pDSComQueue ;

extern bool g_bCloseServer ;
extern HANDLE g_hInitEvent;

#ifdef _NXPCB_AWARD_200703_
#include "../MyLibrary/CriticalSection.h"
std::map<string, CUser*>	g_mapNXAcc; // <넥슨계정, 유저객체>
CS							g_csNXAcc;
#endif

//char * GetMyIP( char mode /*0:LAN, 1:WAN*/);

void AccountLogout(int serverNum, COLE_DB_Executer * pDBCommand, C_USER_DB_Command * pUserCommand ) ;

PUSER AddUserFromCharacName( COLE_DB_Executer * pDBCommand, C_USER_DB_Command * pUserCommand, char * cname );
PUSER AddUserFromAccount( COLE_DB_Executer * pDBCommand, C_USER_DB_Command * pUserCommand, char * acc );

SOCKET RestoreSocket( const unsigned short usPortNum, char mode ) ;

SOCKET		g_UDPsocket = INVALID_SOCKET ;							// send socket
unsigned short g_usPortNum = 0 ;



// Send queue 에 들어 있는 패킷을 꺼내서 실제로 send 한다..
DWORD WINAPI th_UDPSendThread( LPVOID lp )
{
	// 초기화
	_packetUDP	* packet;	
	SOCKADDR_IN * toAddr ;										// Send addr pointer
	int			toLen = sizeof(SOCKADDR_IN) ;					// send addr len
	
	int			nReturn ;									// send result				
	char	*	sendBuf ;										// Send buffer pointer
	int			bufSize = sizeof(char) * UDP_SEND_BUF_SIZE ;	// Send buffer size
	int			sendSize ;										// send data size
	
	g_usPortNum = g_ServerInfo.GetDSUdpPort() ;

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, 50, NULL, NULL, TRUE ) ;


	// loop
	while (g_Threading) 
	{		

// 1. Check queue		
		if( g_pUDPSendPacketQ->empty() ) 
		{
			
			WaitForSingleObject( hTimer, INFINITE ) ;		
			continue ;
		}
		else 
		{			
// 2. Get packet at the queue
			packet = g_pUDPSendPacketQ->get() ;								// 패킷을 얻고
		
			if(packet == NULL) 
			{
				WaitForSingleObject( hTimer, INFINITE ) ;
				continue;
			}		

		}		

// 3. send packet 
		
		sendSize = packet->sBufSize ;
		if( sendSize > sizeof(_packetUDP ) )
		{
			continue ;
		}
		sendBuf  = packet->buf ;
		toAddr = &packet->addr ;
		
		try{
			// 서버에게 데이타 출력 요청
			nReturn =sendto(g_UDPsocket, sendBuf, sendSize, 0, (sockaddr*)toAddr, toLen);	
			Sleep(5) ;

			if (nReturn == 0)
			{
				::PrintConsole("[SO_ERR] send failed / 서버 접속 종료\n");

				if( g_UDPsocket == INVALID_SOCKET ) 
				{
					closesocket( g_UDPsocket ) ;					
					g_UDPsocket = RestoreSocket( g_usPortNum, 0 ) ;
					if( g_UDPsocket == INVALID_SOCKET )
					{
						::PrintConsole("[SO_ERR] UDP Socket Restore failed \n ") ;						
					}
				}
				continue ;
			}
			else if (nReturn == SOCKET_ERROR)
			{
				
				::PrintConsole("[SO_ERR] send failed / send 실패. Error No.%d\n", WSAGetLastError());
				::PrintConsole("{%s} \n", inet_ntoa( toAddr->sin_addr ) ) ;
				if( g_UDPsocket == INVALID_SOCKET ) 
				{
					closesocket( g_UDPsocket ) ;					
					g_UDPsocket = RestoreSocket( g_usPortNum, 0 ) ;
					if( g_UDPsocket == INVALID_SOCKET )
					{
						::PrintConsole("[SO_ERR] UDP Socket Restore failed \n ") ;						
					}
				}
				continue ;
			}
			else if (nReturn != sendSize)
			{
				::PrintConsole("[SO_ERR] send failed / 비정상적인 전송. Error No.%d\n", WSAGetLastError());
				
				if( g_UDPsocket == INVALID_SOCKET ) 
				{
					closesocket( g_UDPsocket ) ;					
					g_UDPsocket = RestoreSocket( g_usPortNum, 0 ) ;
					if( g_UDPsocket == INVALID_SOCKET )
					{
						::PrintConsole("[SO_ERR] UDP Socket Restore failed \n ") ;						
					}
				}

				continue ;
			}
			continue ;

		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_UDPSendThread \n") ;
			Sleep(50) ;
			continue ;
		}	

	} //while

	::PrintConsole("DWORD WINAPI th_UDPSendThread( LPVOID lp ) Exit \n");
	printf( "DWORD WINAPI th_UDPSendThread( LPVOID lp ) Exit \n " ) ;
	return 0 ;
}

// recv 받은 데이타를 Recv queue 에 넣는다.. (데이타 처리 작업은 작업쓰레드가 한다.)
DWORD WINAPI th_UDPRecvThread( LPVOID lp )
{
	
// 1. 초기화	
	SOCKADDR_IN fromAddr ;		// recvfrom 에 인자로 
	int			frimLen = sizeof(SOCKADDR_IN) ;			// recvfrom 에 쓰일 변수	
	char		recvbuf[UDP_RECV_BUF_SIZE] ;			// 받기 버퍼
	int			bufSize = sizeof(char) * UDP_RECV_BUF_SIZE ;
	int			nReturn = 0 ;


	try {
		// Initailize socket	
		if( g_UDPsocket == INVALID_SOCKET ) 
		{
			g_UDPsocket = RestoreSocket( g_usPortNum, 0 ) ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] 'th_UDPRecvThread' Thread - 'g_UDPsocket' socket \n ") ;
		return 0 ;
	}

    
	while (g_Threading) {
// 2. Recv

		try{
			Sleep(5) ;
			nReturn = recvfrom( g_UDPsocket, recvbuf, bufSize, 0, (sockaddr*)&fromAddr, &frimLen ) ;			

			if (nReturn == 0)
			{
				::PrintConsole("[SO_ERR] UDP Recv failed / 서버 접속 종료(%s)\n", inet_ntoa(fromAddr.sin_addr) );
				
				if( g_UDPsocket == INVALID_SOCKET ) 
				{
					closesocket( g_UDPsocket ) ;
					g_UDPsocket = RestoreSocket( g_usPortNum, 0 ) ;
					if( g_UDPsocket == INVALID_SOCKET )
					{
						::PrintConsole("[SO_ERR] UDP Socket Restore failed \n ") ;						
					}
				}
				
				continue ;
			}
			else if (nReturn == SOCKET_ERROR)
			{			
				nReturn = WSAGetLastError() ;
				if( nReturn == WSAETIMEDOUT )
				{
					continue ;
				}
				if( nReturn != 0 )
				{
					::PrintConsole("[SO_ERR] UDP Recv failed / Recv 실패. Error No.%d <%s>\n", nReturn, inet_ntoa(fromAddr.sin_addr) );
					if( g_UDPsocket == INVALID_SOCKET ) 
					{
						closesocket( g_UDPsocket ) ;					
						g_UDPsocket = RestoreSocket( g_usPortNum, 0 ) ;
						if( g_UDPsocket == INVALID_SOCKET )
						{
							::PrintConsole("[SO_ERR] UDP Socket Restore failed \n ") ;
						}
					}
					continue ;
				}				
			}


			try {

				if( nReturn > 0 ) {				

					if( fromAddr.sin_addr.S_un.S_addr == 0 )
					{
						::PrintConsole("address zero \n") ;
					}

					// 3. 큐에 넣기				
					// Login Packet 은 LoginPacketQueue  에 넣고..
					switch( recvbuf[4] )
					{
					case UMSG_LS_DS_TEST_NUM:
					case UMSG_LOGIN_LS_DS_NUM:
					case UMSG_LOGIN_INFO_REQ_LS_DS_NUM:
					case MSG_NO_LOCAL_COMMANDER:
						g_pUDPLoinPacketQ->insert_udp( &fromAddr, recvbuf, nReturn ) ;
						break;
					default:
						g_pUDPRecvPacketQ->insert_udp(&fromAddr, recvbuf, nReturn) ;
						break;
					}
				}
				else {
					continue ;
				}
			}
			catch ( ... ) {
				::PrintConsole("[EXCEPTION] g_pUDPLoinPacketQ insert \n" ) ;
				continue ;
			}
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_UDPRecvThread \n") ;
		}
		

	}

	::PrintConsole("DWORD WINAPI th_UDPRecvThread( LPVOID lp ) Exit \n");
	printf( "DWORD WINAPI th_UDPRecvThread( LPVOID lp ) Exit \n " ) ;

	return 0 ;
}


// 원래는 Network.cpp 에 있어야 하지만, 거기에 있으면,
// 헤더랑 전역 변수를 사용하기가 까다롭다. 그래서 여기로 옮겨 작업한다.
void CConnectionManager::DSRecv()
{
	SetEvent( m_hInitEvent );

	DWORD CompKey, dwNumBytes; 
	int bReturn;
	CIO_UNIT * pOverlapped;
	CConnectionObject * pCon;
	BOOL bParse = FALSE;

	while( 1 ) {	// 그냥 무한루프다.
		bReturn = m_IOCP.GetStatus( &CompKey, &dwNumBytes, (WSAOVERLAPPED**)&pOverlapped, INFINITE ) ;

		try
		{
			if( bReturn && CompKey ) 
			{		// Operation Success
				switch( pOverlapped->m_iMode ) 
				{
				case iomode_recv:
					// recv 이면...
					if( dwNumBytes > 0 ) 
					{
						// 파싱 시작
						pCon = (CConnectionObject*)CompKey;

						bParse = pCon->ParseAndProcess( dwNumBytes ) ;	// 

						if( bParse == FALSE ) 
						{
							pCon->Close();
							AddConObject( pCon );

							PrintConsole( "[TCP GMS] Parsing Error - Disconnect!\n" );
						}
						else
						{
							if( FALSE == pCon->RecvRequest() )
							{
								pCon->Close();
								AddConObject( pCon );

								PrintConsole( "[TCP GMS] RecvRequest Error - Disconnect!\n" );
							}
						}
					}
					else 
					{
						// 소켓 닫기 
						pCon = (CConnectionObject*)CompKey;

						pCon->Close();
						AddConObject( pCon );

						PrintConsole( "[TCP GMS] Recv Zero Disconnect\n" );
					}
					break ;
				case iomode_send_complete:
					// send 이면...
					break;
				case iomode_end:
					PrintConsole( "[TCP GMS] th_GMSRecvThread Return2\n " );
					return ;

				}	// switch
			} // if Return.
			else if( CompKey )
			{	// GetStatus 가 0을 리턴. 뭔가 이상이 있다. 
				pCon = (CConnectionObject*)CompKey;
				if( pCon )
				{
					pCon->Close();
					AddConObject( pCon );

					PrintConsole( "[TCP GMS] GetStatus Return Zero - Disconnect!\n" );
				}
			}
			else if( bReturn )
			{
				if( pOverlapped->m_iMode == iomode_end )
				{
					return ;
				}
			}
		}
		catch(...)
		{
		}
	}

	PrintConsole( "[TCP GMS] th_GMSRecvThread Return\n " ) ;
	return ;
}

// 이것도 역시 Network.cpp에 있어야 한다.
BOOL CConnectionObject::ParseAndProcess( int recvbytes )
{
	BOOL ret = TRUE;
	int count = 0;
	__mid_packet_unit unit;

	m_recvBuffer.NoticeRecv( recvbytes );

	try
	{
		while( m_recvBuffer.GetPacket( unit ) && count < 50 )
		{
			// Proecess

			g_pSystemLog->LogFileWrite( " GMS - Tcp : unit length = %d\n", unit.length );

			g_pUDPRecvPacketQ->insert_tcp( this, unit.packet, unit.length );
			++count;
		}

		if( count >= 50 )
			ret = FALSE;

	}
	catch( ... )
	{
		ret = FALSE;
	}

	return ret;
}

DWORD WINAPI th_UDPLoginPacketWorkThread( LPVOID lp ) 
{
	
	_packetUDP	*		packet;					// packet node pointer
	char *				buf ;					// recv buffer pointer	
	SOCKADDR_IN	*		fromAddr ;				// recv client address pointer			
	const int			fromLen = sizeof(SOCKADDR_IN);
												// LS UDP Recv port number
	const u_short		usLSUDPRecvPortNum = htons( g_ServerInfo.GetLSUdpPort() ) ;	
												
	PUSER				pUser ;
	int					iUid ;					// 유저의 acc_id	
	u_short				usTempUid ;
	char				cResultValue ;
	int					iGlobalTime ;

	_sMemSave_timerentry entry ={0} ;
	// Login 관련..
	UMSG_LOGIN_LS_DS	*			pMsgLogin  = NULL;					// LS->DS LOGIN 요청
	UMSG_LOGIN_DS_LS_RESULT			msgLoginResult = {0} ;				// DS->LS LOGIN 성공 응답		
	msgLoginResult.iKey		= PASS_KEY;	
	msgLoginResult.cMessage = UMSG_LOGIN_DS_LS_RESULT_NUM;
	
	CMSG_USER_LOGOUT_DS_GS			msgUserOut = {0} ;
	msgUserOut.usLength = sizeof(CMSG_USER_LOGOUT_DS_GS) ;
	msgUserOut.cMessage = CMSG_NO_USER_LOGOUT_DS_GS ;
	


	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, 20, NULL, NULL, TRUE ) ;


	COLE_DB_Executer *	g_UDP_DB_Command = new COLE_DB_Executer( g_pDBCon ) ;
	assert( g_UDP_DB_Command ) ;


	C_USER_DB_Command *	g_UDP_USER_DB_Command = new C_USER_DB_Command( g_pUSERDBCon ) ;
	assert( g_pUSERDBCon ) ;


	// loop
	while (g_Threading) 
	{

		// 1. 큐 검사		
		if( g_pUDPLoinPacketQ->empty() )
		{		
			WaitForSingleObject( hTimer, INFINITE ) ;
			continue ;
		}
		else
		{
			try{
				// 2. 큐에서 가져오기
				packet = g_pUDPLoinPacketQ->get() ;								// 패킷을 얻고
		
				if(packet == NULL) 
				{
					WaitForSingleObject( hTimer, INFINITE ) ;
					continue;
				}

			}
			catch(...){
				::PrintConsole("[EXCEPTION] g_pUDPLoinPacketQ->get()\n ");
				continue;
			}		

		}	

		int pos = 0 ;
		try{			
			
			// pointer setting
			//buf = packet->buf ;
			buf = packet->buf ;
			fromAddr = &packet->addr ;
			if( buf[4] != MSG_NO_LOCAL_COMMANDER )
				fromAddr->sin_port = usLSUDPRecvPortNum ;		// exchange port ( Login server UDP Recv port )

			// Login 처리..
			// REQ : CMSG_LOGIN_LS_DS (LS->DS)
			// RES : CMSG_LOGIN_DS_LS_RESULT (DS->GS)
			//		 If success then set result = 'last server num'
			//		 if Failed then set result = 'error result num'

			if( buf[4] == UMSG_LOGIN_LS_DS_NUM )
			{			

				pos = 1;
				pMsgLogin = (UMSG_LOGIN_LS_DS *)buf ;

				// iTempUniqueID == 0 이면 무시하라..
				if(pMsgLogin->iTempUniqueID == 0 || pMsgLogin->cAccount[0] == 0 ) 
				{
					g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => Fail - Info Error %d\n", pMsgLogin->iTempUniqueID );
					continue;
				}

				g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "[LOGIN RECV] Login Request Acc : %s, UID %d \n", pMsgLogin->cAccount, pMsgLogin->iTempUniqueID );

				/// 접속 제한 범위(동접제한)를 넘었을경우 GM 을 제외한 유저는 접속을 거부한다.
				if( pMsgLogin->cClass < _GM_CLASS_NUM )
				{
					if( g_MaxConnection < g_ServerInfo.GetForestUserCount() )
					{
						// 서버 상태를 Full 상태로 바꾼다.
						g_ServerInfo.SetForestState( _FOREST_STATE_FULL_ ) ;

						pos = 2;
						msgLoginResult.cServerNo = _ERRNO_IS_FULL_ ;					
						g_pUDPSendPacketQ->insert_udp(fromAddr, (char *)&msgLoginResult, sizeof(msgLoginResult)) ;				

						g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => Forest is full \n" );

						continue ;
					}	

				}
				

				// 서버 상태를 정상 상태로 바꾼다..
				g_ServerInfo.SetForestState( _FOREST_STATE_RUNNING ) ;
		

				// 초기화
				pUser = NULL;				// User info struct				
				iUid = 0;					// 유저의 acc_id					
				iGlobalTime = CUser::get_time_by_sec() ;
				cResultValue = 0 ;

				pos = 3;
                				
				// uid 발급
				usTempUid = g_pUniqueIDQueue->GetUID() ;
				pos = 4;
				if( usTempUid == 0 )
				{
					g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => UID Create Fail\n" );
					cResultValue = _ERRNO_STATE_ERROR_ ;
					goto lable_end ;
				}

				pos = 5;
				// DB인증
				bool bRes = false ;

				switch( g_uiCompanyCode )
				{
				case en_lmbc_nexon:
					bRes = g_UDP_USER_DB_Command->DSNexonLogin( pMsgLogin->iAccUid, pMsgLogin->cAccount, g_ForestNum, usTempUid ) ;
					iUid = pMsgLogin->iAccUid ;
					break;
				default:
					bRes = g_UDP_USER_DB_Command->DSLogin( IN OUT pMsgLogin->cAccount, g_ForestNum, usTempUid, &iUid ) ;
				}
			
				pos = 6;
				if( bRes == false )
				{
					pos = 7;
					g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => g_USER_DB->DSNexonLogin/ acc:%s Uid\n", pMsgLogin->cAccount );
					cResultValue = _ERRNO_STATE_ERROR_ ;
					goto lable_end ;
				}			

				// USER cID 를 KEY 로 갖는 맵에서 찾아본다..				
				pUser = g_pAccMapManager->Find_User(pMsgLogin->cAccount);
						
				pos = 8;

				// 메모리 맵에 유저가 없을경우 DB에서 정보를 긁어온다..	
				if(pUser == NULL) 
				{
					pUser = g_pAccMapManager->GetWritePtr();	// Get user memory pointer
					if(pUser == NULL) {
#ifdef _TRACE_					
						g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => Memory Insufficient\n" );
#endif					
						cResultValue = _ERRNO_STATE_ERROR_ ;
						goto lable_end ;
					}

					pos = 9;

										
					// 유저 기본 키 정보 입력
					memcpy(pUser->cNDAccount, pMsgLogin->cAccount, en_max_lil+1);
					pUser->iUid = iUid; 

					// DB LOADING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


					pos = 10;
					// DB에서 창고(Depot) 데이타를 읽어온다.
					if( g_UDP_DB_Command->pr_GetDepot(pUser->iUid, pUser) == false)
					{
						pos = 11;

						PrintConsole( "  => DB Loading(pr_GetDepot) failed (account: %s : %d) \n", pUser->cNDAccount, pUser->iUid );

						g_pAccMapManager->PutMemPtr( pUser ) ;		// 실패했을 경우 맵을 다시 반환한다..
						pUser = NULL ;
						cResultValue = _ERRNO_STATE_ERROR_ ;
						goto lable_end;
					}

					if( false == g_UDP_DB_Command->GetAccountItemUseStatus( pUser->iUid, pUser ) )
					{
						pos = 11;

						PrintConsole( "  => DB Loading(GetAccountItemUseStatus) failed (account: %s : %d) \n", pUser->cNDAccount,pUser->iUid );

						g_pAccMapManager->PutMemPtr( pUser ) ;		// 실패했을 경우 맵을 다시 반환한다..
						pUser = NULL ;
						cResultValue = _ERRNO_STATE_ERROR_ ;
						goto lable_end;
					}
					pos = 12;
					// DB 에서 캐릭 정보를 가져온다.(모든 데이타를 읽음)
					if(	g_UDP_DB_Command->Get_Charac_List(pUser->iUid, pUser) == false){		// Read charac list from DB
						pos = 13;

						PrintConsole( "  => DB Loading(Get_Charac_List) failed (acc : %s) \n", pUser->cNDAccount);

						g_pAccMapManager->PutMemPtr( pUser ) ;		// 맵을 다시 반환한다..
						pUser = NULL ;
						cResultValue = _ERRNO_STATE_ERROR_ ;
						goto lable_end;	
						
					}	
	
					pos = 14;
					// INSERT MAP !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					g_pAccMapManager->Insert(pUser);
					pos = 15;

					g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => Success User Acc : %s \n", pUser->cNDAccount);

					pUser->cState = US_LOGIN_ING;		// 유저 상태 변경

					pUser->bCheckCurEvent = 0;

#ifdef _NXPCB_AWARD_200703_
					memcpy(pUser->cNXAcc, pMsgLogin->cNXAcc, en_max_lil+1);
#endif
				}//if(pUser == NULL)


				else{ //(pUser != NULL) 맵에 데이타가 있는 경우다..					
					pos = 16;

#ifdef _NXPCB_AWARD_200703_
					memcpy(pUser->cNXAcc, pMsgLogin->cNXAcc, en_max_lil+1);
#endif
                    
					// 5초 안에 똑같은 로그인이 들어오면 실패 처리한다.
					if(pUser->iTime + 3 > iGlobalTime)
					{						

						g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => RE:Time Check Error : in 3 sec A<%s>\n", pUser->cNDAccount );

						pUser->iTime = iGlobalTime + 3 ;
						pUser = NULL ;
						cResultValue = _ERRNO_STATE_ERROR_ ;
						goto lable_end;	
					}




					switch(pUser->cState) {	// 유저 상태 검사

					case US_NONSTATE:						

						g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => RE: Nonstate A<%s> \n", pUser->cNDAccount);		

						pUser->cState = US_LOGIN_ING;

						break;

					case US_LOGIN_ING:	// Loging

						g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => RE: Login ing A<%s>\n", pUser->cNDAccount);	

						pUser->cState = US_LOGIN_ING;

						break;

					case US_MOVE:	// 서버 이동중에 로그인은 무시
							g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => RE: While Server Moving A<%s>\n", pUser->cNDAccount );
							pUser->cState = US_LOGIN_ING ;								
//						}
						break;

					case US_LOBBY:
						g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => RE: In Lobby A<%s>\n", pMsgLogin->cAccount );

						// GS 에 지금 로그인중인 계정 강제 종료 요청
						memcpy( msgUserOut.cNDAccount, pMsgLogin->cAccount, en_max_lil+1);
						msgUserOut.cReason = 0 ;
												
						{
							_h_DS_Com * pDSCom ;
							if( pDSCom = g_ServerInfo.GetDSCom( pUser->iServer ) )
							{
								pDSCom->Send( msgUserOut.usLength, (char *)&msgUserOut ) ;
							}

						}
						
				
						pUser->cState = US_NONSTATE;							
						for(int i=0; i<3; i++){
							if( pUser->stCharac[i].CHARAC_BASIC.iUniqueID != 0 )
							{
								pUser->stCharac[i].CHARAC_ETC.cState = CS_NONSTATE;									
							}
						}

						pos = 17;
						// 메모리 삭제 예약..
						g_pMemSaveTimer->Reserve_save( pUser, pUser->iUid, CUserMemSaveTimer::get_time_by_sec() + _MEM_SAVE_DELAY_TIME_KICK ) ;
						pos = 18;
						// 결과 
						cResultValue = _ERRNO_USING_ACCOUNT_;

						break;

					case US_FIELD:		// 게임진행중
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  RE: ALIVE USER A<%s>\n", pUser->cNDAccount );
#endif													

						pos = 19;
						// GS 에 지금 로그인중인 계정 강제 종료 요청						
						memcpy(msgUserOut.cNDAccount, pMsgLogin->cAccount, en_max_lil+1);
						msgUserOut.cReason = 0 ;						

						{
							_h_DS_Com * pDSCom ;
							if( pDSCom = g_ServerInfo.GetDSCom( pUser->iServer ) )
							{
								pDSCom->Send( msgUserOut.usLength, (char *)&msgUserOut ) ;
							}

						}
						
						for(int i=0; i<3; i++){
							if( pUser->stCharac[i].CHARAC_BASIC.iUniqueID != 0 )
							{
								pUser->stCharac[i].CHARAC_ETC.cState = CS_NONSTATE ;									
							}								
						}


						pos = 20;
						// Result							
						pUser->cState = US_NONSTATE ;
						cResultValue = _ERRNO_USING_ACCOUNT_ ;

						
						// 메모리 삭제 예약..
						g_pMemSaveTimer->Reserve_save( pUser, pUser->iUid, CUserMemSaveTimer::get_time_by_sec() + _MEM_SAVE_DELAY_TIME_LOGOUT ) ;
						pos = 21;
						
						break;

					case US_LOCK:		// 연속된 로그인 실패로 계정이 잠긴 상태
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => RE: Locked User A<%s>\n", pMsgLogin->cAccount );
#endif							
						cResultValue = _ERRNO_STATE_ERROR_;
						break;
					}//switch(pUser->cState)
					
				}// else			

lable_end:			

				// 로그인 성공/실패 메시지 (공통)
				memcpy(msgLoginResult.cAccount, pMsgLogin->cAccount, en_max_lil+1);
				msgLoginResult.iTempID = pMsgLogin->iTempUniqueID;
				msgLoginResult.cForestNo = pMsgLogin->cServerNum ;
				msgLoginResult.usUid = usTempUid ;

				if( g_bCloseServer && pMsgLogin->cClass < _GM_CLASS_NUM )
				{
					cResultValue = _ERRNO_STATE_ERROR_ ;
					g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => Create dump data <acc:%s>\n", pMsgLogin->cAccount ) ;
				}

				pos = 22;
				if( cResultValue < 0 )
				{
					// 실패 메시지.
					msgLoginResult.cServerNo = cResultValue ;
				}
				else
				{
					pos = 23;
					if( pUser )
					{
						// 사용자 등급 저장하기
						pUser->cClass = pMsgLogin->cClass ;

						// 마지막 접속 서버 번호 얻어오기..					
						pUser->iServer = g_UDP_DB_Command->GetLastServer( iUid );
						if( pUser->iServer == 0 || pUser->iServer >= GS_MAX || g_ServerInfo.SelectState(pUser->iServer) != GS_OK ) 
						{
							pUser->iServer = g_ServerInfo.GetRandServerNum() ;
#ifdef _TRACE_					
							g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "[INFO] Last GS (%d) \n", pUser->iServer );			
#endif
						}
						pos = 24;

						pUser->iIp = pMsgLogin->iIp;				
						pUser->usTempUniqueID = usTempUid ;				
						pUser->iTime = iGlobalTime ;
										
						CTime ct = CTime::GetCurrentTime() ;
						pUser->dwLoginTime = static_cast<DWORD>((ct.GetTime()) / 60) ;

#ifdef _PD_VINA_PCROOM_INFO_
						pUser->ucPCRoomInfo = pMsgLogin->ucPCRoomInfo;
#endif

						pUser->bCheckCurEvent = 0;

						// 위에부분에서 정상 동작하는 서버로 셋팅은 끝났음..
						msgLoginResult.cServerNo = (char)pUser->iServer ;

						
						for(int i=0; i<3; i++)
						{
							if( pUser->stCharac[i].CHARAC_BASIC.iUniqueID != 0 )
							{
								pUser->stCharac[i].CHARAC_STATE.sMonsterKill = 0 ;		
								//pUser->stCharac[i].CHARAC_INVENTORY.sBloodPoint = 0 ;
							}								
						}
						

						//IF_NOT_FREE_LOGIN	// 20060723 지운다. 이제 빌링은 없다. 
					}
					else
					{
						msgLoginResult.cServerNo = _ERRNO_STATE_ERROR_ ;
					}
				}

#ifdef _METEL_LOGIN_200608_
				if(pUser != NULL) {
					// M-etel 빌링 정보 저장
					// Client가 GS에 접속 후, GS->DS 요청이 오면, 빌링 정보를 DS->GS ack.
					memcpy( pUser->cPassword, pMsgLogin->cPass, en_max_lpl+1 ) ;	
					pUser->bPCBang = (BOOL)pMsgLogin->bPCBang;
					memcpy(&pUser->MetelBillingInfo, &pMsgLogin->MetelBillingInfo, sizeof(METEL_BILLING_INFO));
				}
#endif //_METEL_LOGIN_200608_

				pos = 25;
				fromAddr->sin_port = usLSUDPRecvPortNum ;			// exchange send port
				g_pUDPSendPacketQ->insert_udp(fromAddr, (char *)&msgLoginResult, sizeof(msgLoginResult)) ;

				if( cResultValue >= 0 )
				{
					pos = 26;
					// 성공이면 로그 남기기..
					_CharacLog * pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
					if( pCharacLog && pUser )
					{
						memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

						pCharacLog->code = _LOG_CHARAC_LOGIN_ ;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
						memcpy( pCharacLog->cAccount, pUser->cNDAccount, en_max_lil);
						pCharacLog->cAccount[en_max_lil] = 0;
#else
						memcpy( pCharacLog->account, pUser->cNDAccount, 12);
						pCharacLog->account[12] = 0;
#endif
						in_addr addr = {0};
						addr.S_un.S_addr = pUser->iIp ;
						memcpy( pCharacLog->ip, inet_ntoa( addr ), 17) ;
						pCharacLog->zone = pUser->iServer ;						
						g_pLogSvrCom->InsertCharacLog( pCharacLog ) ;
					}
				}

				g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "[INFO] UDP Send LS(%s) A(%s) : Server(%d), TempID(%d)\n"
					, inet_ntoa( fromAddr->sin_addr )
					, msgLoginResult.cAccount
					, msgLoginResult.cServerNo
					, msgLoginResult.iTempID ) ;
				
			} // if( buf[4] == UMSG_LOGIN_LS_DS_NUM )


			/* ========================================================================
			LS <-> DS Msg ( 121 ~ )
			======================================================================== */
			//#define		UMSG_LOGIN_INFO_REQ_LS_DS_NUM	123			
			else if( buf[4] == UMSG_LOGIN_INFO_REQ_LS_DS_NUM )
			{				

				pos = 23 ; 

				// LS->DS 시스템 정보 요청 처리
				UMSG_LOGIN_INFO_REQ_LS_DS *		pMsgLoginInfoReq = (UMSG_LOGIN_INFO_REQ_LS_DS *)buf ;

				UMSG_LOGIN_INFO_RES_DS_LS 		msgLoginInfoRes = {0} ;	
				msgLoginInfoRes.iKey = PASS_KEY ;
				msgLoginInfoRes.cMessage = UMSG_LOGIN_INFO_RES_DS_LS_NUM ;
				msgLoginInfoRes.cForestNum = g_ForestNum ;

				/*=========================================================================================================
				// 서버가 정상작동 상태라면 동접률을 보내주고.. 정상작동아 아닌 상태라면(점검중, 폭주상태) 서버 상태 번호를
				// 보내준다.
				==========================================================================================================*/

				if( g_MaxConnection > g_ServerInfo.GetForestUserCount() )
				{						
					g_ServerInfo.SetForestState( _FOREST_STATE_RUNNING ) ;
				}
				else 
				{
					// 서버 상태를 Full 상태로 바꾼다.
					g_ServerInfo.SetForestState( _FOREST_STATE_FULL_ ) ;
				}



				if( _FOREST_STATE_RUNNING == g_ServerInfo.GetForestState() )
				{
					if( g_bCloseServer == true )
					{
						msgLoginInfoRes.cForestState = _FOREST_STATE_CHECKUP_ ;
					}
					else
					{
						msgLoginInfoRes.cForestState = static_cast<char>((g_ServerInfo.GetForestUserCount() / static_cast<float>(g_MaxConnection) ) * 100) ;
					}						
				}
				else
				{
					msgLoginInfoRes.cForestState = g_ServerInfo.GetForestState() ;
				}
				//==========================================================================================================
				g_pUDPSendPacketQ->insert_udp(fromAddr, (char *)&msgLoginInfoRes, sizeof(msgLoginInfoRes) ) ;		

				continue;

			}
			else if( buf[4] == UMSG_LS_DS_TEST_NUM )
			{
				// Test Packet 처리.
			}
			// GS SERVER SHUTDOWN MSG
			else if( buf[4] == MSG_GS_SHUTDOWN_NUM )
			{
				MSG_GS_SHUTDOWN * pGsShutdown = (MSG_GS_SHUTDOWN*)buf ;
				// 해당 GS 의 캐릭터들을 로그아웃 시킨다.				
				AccountLogout( pGsShutdown->iServerNum, g_UDP_DB_Command, g_UDP_USER_DB_Command ) ;
			}
			/* ========================================================================
			Local Commander <-> DS Msg ( 121 ~ )
			======================================================================== */
			//#define		MSG_NO_LOCAL_COMMANDER	123			
			else if( buf[4] == MSG_NO_LOCAL_COMMANDER )
			{
				MSG_LOCAL_COMMANDER * plc = reinterpret_cast<MSG_LOCAL_COMMANDER*>(buf);

				MSG_LOCAL_COMMANDER pkLC;

				PrintConsole( "[RECV] Local Commander Packet ( mode : %u )\n", plc->ucMode );

				switch( plc->ucMode )
				{
				case en_lc_ds_open_req:
					g_bCloseServer = false;
					pkLC.ucMode = en_ds_lc_open_result;
					break;
				case en_lc_ds_close_req:
					g_bCloseServer = true;
					pkLC.ucMode = en_ds_lc_close_result;
					break;
				case  en_lc_ds_off_req:
					pkLC.ucMode = en_ds_lc_off_result;
					break;
				case en_lc_ds_on_req:
					pkLC.ucMode = en_ds_lc_on_result;
					break;
				}

				g_pUDPSendPacketQ->insert_udp(fromAddr, (char *)&pkLC, sizeof(pkLC) ) ;		
			}
			else
			{
				::PrintConsole("[info] Bad packet : %d \n", static_cast<int>(buf[4]) ) ;
			}
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_UDPLoginPacketWorkThread (pos:%d)\n",pos) ;
		}				

	} //while

	::PrintConsole("DWORD WINAPI th_UDPLoginPacketWorkThread( LPVOID lp ) Exit \n") ;
	printf("DWORD WINAPI th_UDPLoginPacketWorkThread( LPVOID lp ) Exit \n") ;

	delete g_UDP_DB_Command ;
	delete g_UDP_USER_DB_Command ;

	return 0 ;
}



DWORD WINAPI th_UDPGMSPacketWorkThread( LPVOID lp )
{
	_packetUDP	*		packet;					// packet node pointer
	char *				buf  ;					// recv buffer pointer	
	SOCKADDR_IN *		fromAddr  ;				// recv client address pointer		
	const int			fromLen = sizeof(SOCKADDR_IN);	

	PUSER				pUser ;
	PCHARAC				pCharac ;
	int					pos ;
		

	_gmsHeader		*	pGmsHeader ;
	time_t				now ;
	tm					when ;

	u_int				uiNow ;

	CConnectionObject *		pConObj;
	int						fromType;	//0 - udp, 1 - tcp


	MSG_CREATE_ITEM_REQ *		pMsgCreateItem =  NULL ;	
	CMSG_GSINFO_RES_GS_DS *		pMsgGsinfoRes = NULL;	
	MSG_SETPOS_REQ *			pMsgSetposReq = NULL;


	

	MSG_SYSTEM_INFO_RES			msgSystemInfoRes;
	msgSystemInfoRes.header.iKey = PASS_KEY;
	msgSystemInfoRes.header.ucMessage = MSG_SYSTEM_INFO_RES_NUM;


	
	MSG_SETPOS_RES				msgSetPosRes;
	msgSetPosRes.header.iKey = PASS_KEY;
	msgSetPosRes.header.ucMessage = MSG_SETPOS_RES_NUM;

	MSG_CMD_USERKILL	*		pMsgCmdUserKill = NULL;
	

	MSG_QUEST_RES				msgQuestRes;
	msgQuestRes.header.iKey = PASS_KEY ;
	msgQuestRes.header.ucMessage = MSG_QUEST_REQ_NUM;

	MSG_MARTIAL_RES				msgMartialRes;
	msgMartialRes.header.iKey = PASS_KEY;
	msgMartialRes.header.ucMessage = MSG_MARTIAL_RES_NUM;		



	COLE_DB_Executer *	g_UDP_DB_Command = new COLE_DB_Executer( g_pDBCon ) ;
	assert( g_UDP_DB_Command ) ;

	C_USER_DB_Command *	g_UDP_USER_DB_Command = new C_USER_DB_Command(g_pUSERDBCon) ;
	assert( g_UDP_USER_DB_Command ) ;

	_j_Crypto	*  pCrypto = new _j_Crypto( 8192 ) ;
	assert( pCrypto ) ;
	if( pCrypto->Init( CRYPTO_KEY_FILE ) == false )
	{
		g_pSystemLog->LogFileWrite( "  -> th_UDPGMSPacketWorkThread :: CRYPTO INITIALIZE FAILED\n" );
		printf( "  -> th_UDPGMSPacketWorkThread :: CRYPTO INITIALIZE FAILED\n" );
		_endthreadex(0);
		return 0 ;
	}

	SetEvent( g_hInitEvent );

	// loop
	while (g_Threading) 
	{

		// 1. 큐 검사
		
		if( g_pUDPRecvPacketQ->empty() )
		{
		
			Sleep(20) ;
			continue ;
		}
		else
		{		
			// 2. 큐에서 가져오기
			packet = g_pUDPRecvPacketQ->get() ;								// 패킷을 얻고
	
			if(packet == NULL) 
			{
				Sleep(20) ;
				continue;
			}	

		}		

		try{
			// 3. 처리

			pos = 0 ;
			pCharac = NULL;
			pUser = NULL;
			
			

			/* ========================================================================
			GMS <-> DS Msg ( 1 ~ 99)
			======================================================================== */

			if( packet->buf[4] < 1 )
			{
				::PrintConsole( "[ERROR] FOUND BAD PACKET(%d) IN th_UDPGMSPacketWorkThread \n", static_cast<int>(packet->buf[4])) ;
				continue ;
			}

			pGmsHeader = (_gmsHeader*)packet->buf ;
			if( pGmsHeader->iKey != PASS_KEY )
			{
				::PrintConsole( "[ERROR] FOUND BAD_KEY HAVING PACKET IN th_UDPGMSPacketWorkThread \n") ;
				continue ;
			}


			time( &now ) ;			
			uiNow = static_cast<u_int>(now) ;

			if( pGmsHeader->ucMessage != MSG_SYSTEM_TIME_REQ_NUM && pGmsHeader->ucMessage != MSG_CMD_USERKILL_NUM )
			{
				if( pCrypto->Decryption( (char*)&packet->buf[9], packet->sBufSize - 9, pGmsHeader->uiTime % 100 ) == false )
				{
					::PrintConsole( "[ERROR] DECRYPTION FAILED IN th_UDPGMSPacketWorkThread \n" ) ;
					continue ;
				}

				when = *localtime( &now );

				pGmsHeader->cGMName[en_charac_name_length] = 0 ;

				// LOG PRINT
				g_pGMStLog->LogFileWrite( "[%d:%d:%d] MSG:%u GM:%s\n"
					, when.tm_hour
					, when.tm_min
					, when.tm_sec
					, pGmsHeader->ucMessage
					, pGmsHeader->cGMName ) ;

				PrintConsole( "[RECV] MSG:%u GM:%s\n", pGmsHeader->ucMessage, pGmsHeader->cGMName ) ;
			}			
			
			fromType	= packet->iFromConType;
			pConObj		= packet->pGMSTcpCon;
			buf			= packet->buf ;
			fromAddr	= &packet->addr ;

			if (buf[4] < 120) {	// GMS -> DS 패킷
				switch( buf[4])
				{				
				case MSG_SYSTEM_INFO_REQ_NUM:
					{
						pos = 1 ;

						//1. MSG NUM 을 바꾼다..
						//buf[4] = UMSG_NO_GSINFO_REQ_DS_GS;

						MSG_SYSTEM_INFO_REQ * pMsgSystemInfoReq =  (MSG_SYSTEM_INFO_REQ*)buf ;

						
						// 서버 정보를 GMS 에 보낸다.
						char cServerNum = msgSystemInfoRes.cServerNum = pMsgSystemInfoReq->cServer_num ;

						msgSystemInfoRes.iCurLimitConnection = g_MaxConnection ;
						memcpy(&msgSystemInfoRes._ServerSystemInfo, &g_ServerInfo.m_ServerSystemInfo[cServerNum], sizeof(_SERVER_SYSTEM_INFO));								
						memcpy(msgSystemInfoRes._ServerNetInfo.cIP, g_ServerInfo.m_ServerNetInfo[cServerNum].cIP, 16) ;
						msgSystemInfoRes._ServerNetInfo.sPort = g_ServerInfo.m_ServerNetInfo[cServerNum].sPort ;				
						//memcpy(&msgSystemInfoRes._ServerStateInfo, &g_ServerInfo.m_ServerStateInfo[buf[5]], sizeof(_SERVER_STATE_INFO));

						msgSystemInfoRes._ServerStateInfo.cState = g_ServerInfo.m_ServerStateInfo[cServerNum].cState ;
						msgSystemInfoRes._ServerStateInfo.iConnectCount = g_ServerInfo.m_ServerStateInfo[cServerNum].iConnectCount ;
						msgSystemInfoRes._ServerStateInfo.iPlayingCount = g_ServerInfo.GetForestUserCount() ;
						msgSystemInfoRes.iLevelExp = g_ServerInfo.m_ServerStateInfo[cServerNum].iLevelExp ;
						msgSystemInfoRes.iSkillExp = g_ServerInfo.m_ServerStateInfo[cServerNum].iSkillExp ;

						if( g_bCloseServer == true )
						{
							msgSystemInfoRes.bServerState = false ; 
						}
						else
						{
							msgSystemInfoRes.bServerState = true ;
						}
						
						if( fromType == 0 )
							g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&msgSystemInfoRes, sizeof(MSG_SYSTEM_INFO_RES));
						else
							pConObj->SendRequest( &msgSystemInfoRes, sizeof(MSG_SYSTEM_INFO_RES) );
						
						continue;
					}
					break;				
				case MSG_MESSAGE_DIRECT_NUM:
					// 긴급 공지 발송 GMS->DS->GS
					{
						MSG_MESSAGE_DIRECT * pMsgMessageDirect = (MSG_MESSAGE_DIRECT*)buf ;
						pos = 6 ;
						//pMsgMessageDirect->cString[MAX_CHAT_LENGTH-1] = 0 ;
						// 긴급 공지 발송 메시지를.. UMSG_SHOWTEXT_REQ 로 바꿔준다..
						CMSG_SHOWTEXT_REQ msgShowText;
						msgShowText.cMessage = CMSG_SHOWTEXT_REQ_DS_GS ;
						msgShowText.usLength = sizeof(CMSG_SHOWTEXT_REQ) ;

						strncpy(msgShowText.cString, pMsgMessageDirect->cString, MAX_CHAT_LENGTH );
						msgShowText.cString[MAX_CHAT_LENGTH-1] = 0;
						
						//g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Send Quick news \n");				
						//g_pTrace->OutputString( _TRACE_CLASS_GMS, "  - %s \n", pMsgMessageDirect->cString);

						_h_DS_Com * pDSCom ;
						if( pDSCom = g_ServerInfo.GetDSCom( pMsgMessageDirect->cServerNum ) )
						{
							pDSCom->Send( msgShowText.usLength, (char *)&msgShowText ) ;
						}
						
						continue;
					}
					break;
				case MSG_SYSTEM_OFF_REQ_NUM:
					// GMS->DS 시스템 종료 요청
					{
						MSG_SYSTEM_OFF_REQ *pMsgSystemOff = (MSG_SYSTEM_OFF_REQ*)buf ;
						CMSG_SYSTEM_OFF_REQ msgSysOff = {0} ;
						msgSysOff.usLength = sizeof(CMSG_SYSTEM_OFF_REQ) ;
						msgSysOff.cMessage = CMSG_SYSTEM_OFF_REQ_NUM ;

						::PrintConsole("[GMS] Server shutdown request : %d \n", pMsgSystemOff->cServer_num ) ;

						pos = 7 ;

						// -1 이면 DS종료
						if( pMsgSystemOff->cServer_num == -1 )
						{
							bool bRuningGS = false ;
							for(int i=0; i < GS_MAX; i++){
								if(g_ServerInfo.m_ServerStateInfo[i].cState != 0){
									printf("\n[WORNING] Please Shutdown GS(No.%d) before the DS is getting end. \n", i);
									::PrintConsole("\n[WORNING] Please Shutdown GS(No.%d) before the DS is getting end. \n", i);
									bRuningGS = true ;
								}
							}

							// 운행중인 GS가 없으면 내려라.. (당연하자나!)
							if( bRuningGS == false )
							{

								MSG_SYSTEM_OFF_REQ_FOR_GS msgSystemOffReqForGs = {0} ;
								msgSystemOffReqForGs.iKey = PASS_KEY ;
								msgSystemOffReqForGs.cMessage = CMSG_SYSTEM_OFF_REQ_NUM ;
								msgSystemOffReqForGs.cServer_num = pMsgSystemOff->cServer_num ;
								

								// MS 도 자동 종료 시키자..
								SOCKADDR_IN	* pAddr = g_ServerInfo.GetMSSockAddrIn() ;
								g_pUDPSendPacketQ->insert_udp( pAddr, (char*)&msgSystemOffReqForGs, sizeof(MSG_SYSTEM_OFF_REQ_FOR_GS) ) ;


								::PrintConsole( "[AUTO SHUTDOWN] This is getting end by GMS. \n") ;
								g_MainThreading = false ;	// 이게 false 가 되는순간 메인쓰레드가 종료됨
							}
							
						}
						else
						{
							_h_DS_Com * pDSCom ;
							if( pDSCom = g_ServerInfo.GetDSCom( pMsgSystemOff->cServer_num ) )
							{
								pDSCom->Send( msgSysOff.usLength, (char *)&msgSysOff ) ;
							}
						}

						
						continue;
					}
					break;
				case MSG_SYSTEM_TIME_REQ_NUM:
					// 시스템 시간 요청
					{
						pos = 8 ;
						MSG_SYSTEM_TIME_RES msgSystemTimeRes;
						msgSystemTimeRes.header.iKey = PASS_KEY;
						msgSystemTimeRes.header.ucMessage  = MSG_SYSTEM_TIME_RES_NUM;
						time_t now ;
						time( &now ) ;
						msgSystemTimeRes.uiTime = static_cast<u_int>(now) ;
						msgSystemTimeRes.usServerIndex = g_ForestNum ;

						if( fromType == 0 )
							g_pUDPSendPacketQ->insert_udp( fromAddr, (char*)&msgSystemTimeRes, sizeof(MSG_SYSTEM_TIME_RES));
						else 
							pConObj->SendRequest( &msgSystemTimeRes, sizeof(MSG_SYSTEM_TIME_RES) );

						continue;

					}
					break;

				case MSG_GM_REQ_SERVER_PLAYER_NUM:
					{
						MSG_GM_SERVER_PLAYER pkPlayer;
						ZeroMemory( &pkPlayer, sizeof(pkPlayer) );
						pkPlayer.header.iKey = PASS_KEY;
						pkPlayer.header.ucMessage = MSG_GM_SERVER_PLAYER_NUM;
						time_t now;
						time(&now);
						pkPlayer.uiTime = static_cast<u_int>(now);

						pkPlayer.ucGreoupNo = static_cast<u_char>(g_ForestNum);

						pkPlayer.usTotalConnectCount = g_ServerInfo.GetForestUserCount();
						
						// 접속 인원 구하기.
						g_ServerInfo.GetEachServerConPlayer( &pkPlayer );

						if( fromType == 0 )
							g_pUDPSendPacketQ->insert_udp( fromAddr, (char*)&pkPlayer, sizeof(pkPlayer));
						else 
							pConObj->SendRequest( &pkPlayer, sizeof(pkPlayer) );

					}
					break;

				case MSG_SETPOS_REQ_NUM:
					// GMS -> GS 캐릭터 위치 초기화 요청
					{
						pos = 9 ;
						pMsgSetposReq = (MSG_SETPOS_REQ *)buf;

						
						pCharac = g_pAccMapManager->Find_Charac(pMsgSetposReq->cCharacName);
						

						if(pCharac == NULL){
							// GMS 에 로그인중이지 않다는 MSG Packet send					
							memcpy(msgSetPosRes.cCharacName, pMsgSetposReq->cCharacName, 13);
							msgSetPosRes.cResult = -1;

							if( fromType == 0 )
								g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&msgSetPosRes, sizeof(MSG_SETPOS_RES));
							else
								pConObj->SendRequest( &msgSetPosRes, sizeof(MSG_SETPOS_RES) );
							
						}
						else
						{
							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								// GS 에 Packet send

								// GS 에 위치 초기화 요청
								CMSG_SETPOSITION_REQ msgSetposReq = {0} ;
								msgSetposReq.usLength = sizeof(CMSG_SETPOSITION_REQ) ;
								msgSetposReq.cMessage = CMSG_NO_SETPOSITION_REQ_DS_GS ;		
								
								memcpy(msgSetposReq.cCharacName, pCharac->CHARAC_BASIC.cChrName, 13);
								msgSetposReq.fInitPosX = pMsgSetposReq->fX;
								msgSetposReq.fInitPosZ = pMsgSetposReq->fZ;

								// DS -> GS
								_h_DS_Com * pDSCom ;
								if( pDSCom = g_ServerInfo.GetDSCom( static_cast<char>(pCharac->CHARAC_CUR_BASIC.sZone) ) )
								{									
									if( pDSCom->Send( msgSetposReq.usLength, (char *)&msgSetposReq ) == false ){
										::PrintConsole("[SO_ERR] MSG_SETPOS_REQ_NUM \n");
									}
								}
								
							}						

							// 캐릭터 위치 변경
							pCharac->CHARAC_CUR_BASIC.fX = pMsgSetposReq->fX;
							pCharac->CHARAC_CUR_BASIC.fZ = pMsgSetposReq->fZ;

							g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Set position success! (CHR:%s, x:%f, z:%f) \n", 
								pCharac->CHARAC_BASIC.cChrName,pCharac->CHARAC_CUR_BASIC.fX,pCharac->CHARAC_CUR_BASIC.fZ);

							pCharac->CHARAC_ETC._backup.bBackupStateCurBasicInfo = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}
						continue;

					} //if(buf[4] == MSG_SETPOS_REQ_NUM)
					break;
				case MSG_CREATE_ITEM_REQ_NUM:
					{
						pos = 10 ;
						pMsgCreateItem = (MSG_CREATE_ITEM_REQ *)buf ;
						

						CMSG_CREATE_ITEM_GS_DS msgCreateItem = {0} ;
						msgCreateItem.usLength = sizeof(CMSG_CREATE_ITEM_GS_DS) ;
						msgCreateItem.cMessage = CMSG_NO_CREATE_ITEM_GS_DS ;
						msgCreateItem.cFirstType = pMsgCreateItem->cFstType ;
						msgCreateItem.sSecondType = pMsgCreateItem->sScdType ; 

						char cServerNum = 0 ;
						if ( pMsgCreateItem->cPosType == 0 ) 
						{

						
							pCharac = g_pAccMapManager->Find_Charac(pMsgCreateItem->cCharacName) ;
						

							if ( !pCharac ) continue ;

							cServerNum = static_cast<char>(pCharac->CHARAC_CUR_BASIC.sZone) ;
							msgCreateItem.fX = pCharac->CHARAC_CUR_BASIC.fX ;
							msgCreateItem.fZ = pCharac->CHARAC_CUR_BASIC.fZ ;

							

						}
						else if ( pMsgCreateItem->cPosType == 1 ) 
						{

							cServerNum = pMsgCreateItem->pos.cServerNum ;
							msgCreateItem.fX = pMsgCreateItem->pos.fX ;
							msgCreateItem.fZ = pMsgCreateItem->pos.fZ ;

						}

						// DS -> GS
						_h_DS_Com * pDSCom ;
						if( pDSCom = g_ServerInfo.GetDSCom( cServerNum ) )
						{									
							if( pDSCom->Send( msgCreateItem.usLength, (char *)&msgCreateItem ) == false ){
								::PrintConsole("[SO_ERR] UMSG_SETPOSITION_REQ \n");
							}
						}

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Create item req! (CHR:%s, f_type:%d, s_type:%d) \n", 
							pMsgCreateItem->cCharacName, pMsgCreateItem->cFstType, pMsgCreateItem->sScdType );

						continue;

					}
					break;
				case MSG_CMD_USERKILL_NUM:
					{				

						pos = 11 ;

						pMsgCmdUserKill = (MSG_CMD_USERKILL *)buf;

						if( pMsgCmdUserKill->cAccount[0] == 0 ) continue ;

						pUser = g_pAccMapManager->Find_User(pMsgCmdUserKill->cAccount);					

						if(pUser == NULL){
							MSG_CMD_USERKILL_RESULT msgCmdUserkillResult;
							msgCmdUserkillResult.header.iKey = PASS_KEY;
							msgCmdUserkillResult.header.ucMessage = MSG_CMD_USERKILL_RESULT_NUM;
							memcpy(msgCmdUserkillResult.cAccount, pMsgCmdUserKill->cAccount, en_max_lil+1);

							if( 0 == fromType )
								g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&msgCmdUserkillResult, sizeof(MSG_CMD_USERKILL_RESULT));
							else
								pConObj->SendRequest(&msgCmdUserkillResult, sizeof(MSG_CMD_USERKILL_RESULT));
						}
						else if( pUser->cState != US_NONSTATE ){		// 유저 상태 검사 (로그아웃 중이면 무시)

							CMSG_USER_LOGOUT_DS_GS msgUserkill = {0} ;
							msgUserkill.usLength = sizeof(CMSG_USER_LOGOUT_DS_GS) ;
							msgUserkill.cMessage = CMSG_NO_USER_LOGOUT_DS_GS ;							
							
							msgUserkill.cReason = 1;	//운영자에 의한 로그아웃
							memcpy(msgUserkill.cNDAccount,  pUser->cNDAccount, en_max_lil+1);

							// DS -> GS
							_h_DS_Com * pDSCom ;
							if( pDSCom = g_ServerInfo.GetDSCom( pUser->iServer ) )
							{									
								if( pDSCom->Send( msgUserkill.usLength, (char *)&msgUserkill ) == false ){
									::PrintConsole("[SO_ERR] MSG_CMD_USERKILL_NUM \n");
								}
							}
						}	

						//g_UDP_USER_DB_Command->DSLogout( pMsgCmdUserKill->cAccount ) ;

						g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[INFO] MSG_CMD_USERKILL_NUM (CHR:%s) \n", pMsgCmdUserKill->cAccount );

						continue;
					}
					break;
				case MSG_CHARAC_REQ_NUM:
					// 캐릭터 정보 요청
					{
						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_CHARAC_REQ_NUM \n") ;

						pos = 12 ;
						
						MSG_CHARAC_REQ *	pMsgCharacReq = NULL;
						pMsgCharacReq = (MSG_CHARAC_REQ *)buf;

						
						pCharac = g_pAccMapManager->Find_Charac(pMsgCharacReq->cChrName);
						

						if( pCharac == NULL ) 
						{
							char cAcc[en_max_lil+1] = {0} ;
							PUSER pUser = NULL ;
							g_UDP_DB_Command->GetAccName(pMsgCharacReq->cChrName, cAcc) ;

							if( cAcc[0] == 0 )
							{
								continue ;
							}

						
							pUser = g_pAccMapManager->Find_User( cAcc ) ;

							g_pTrace->OutputString( _TRACE_CLASS_GMS, "  Find : Acc<%s>\n", cAcc ) ;

							if( pUser==NULL )
							{							

						
								pUser = g_pAccMapManager->GetWritePtr();	// Get pointer(map) vitual memory							


								if( pUser == NULL )
								{

									continue ;
								}

								//memset(pUser,0,sizeof(_USER));
								pUser->iTime = CUser::get_time_by_sec() ;
								memcpy( pUser->cNDAccount, cAcc, en_max_lil+1) ;

								bool bRes = g_UDP_USER_DB_Command->DSGetAccountUID( pUser->cNDAccount, pUser->iUid ) ;
								if( bRes == false )
								{

									::PrintConsole("[ERROR] GMS Node login failed \n") ;

									// 맵을 다시 반환한다..
									g_pAccMapManager->PutMemPtr( pUser ) ;

									continue ;
								}

								g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] GetDepot (uniqueID: %d) \n", pUser->iUid);

								if(g_UDP_DB_Command->pr_GetDepot( pUser->iUid, pUser ) == false){
									::PrintConsole("[DB_ERR] GetDepot failed (accout: %s) \n", cAcc );

									// 맵을 다시 반환한다..
									g_pAccMapManager->PutMemPtr( pUser ) ;


									continue;
								}

								if( false == g_UDP_DB_Command->GetAccountItemUseStatus( pUser->iUid, pUser ) )
								{
									pos = 11;

									g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => DB Loading(GetAccountItemUseStatus) failed (account: %s : %d) \n", pUser->cNDAccount,pUser->iUid );

									g_pAccMapManager->PutMemPtr( pUser ) ;		// 실패했을 경우 맵을 다시 반환한다..
									
									continue ;
									
								}

								// Get Character info
								if(	g_UDP_DB_Command->Get_Charac_List( pUser->iUid, pUser )  == false){		// Read charac list from DB


									::PrintConsole("[DB_ERR] : CID - %s Get failed what user info \n", pUser->cNDAccount);

									// 맵을 다시 반환한다..
									g_pAccMapManager->PutMemPtr( pUser ) ;
									continue;
								}								


								if(pUser) g_pAccMapManager->Insert(pUser);
								pCharac = g_pAccMapManager->Find_Charac(pMsgCharacReq->cChrName);

								// 메모리 삭제 예약..
								g_pMemSaveTimer->Reserve_save( pUser, pUser->iUid, CUserMemSaveTimer::get_time_by_sec() + _MEM_SAVE_DELAY_TIME_KICK ) ;
							}
						}
						

						if(pCharac){
							MSG_CHARAC_RES	msgCharacRes;
							msgCharacRes.header.iKey = PASS_KEY;
							msgCharacRes.header.ucMessage = MSG_CHARAC_RES_NUM;
							memcpy(msgCharacRes.cChrName, pMsgCharacReq->cChrName, 13);	
							memcpy(&msgCharacRes.CHARAC_BASIC, &pCharac->CHARAC_BASIC, sizeof(_CHARAC_BASIC) ) ;
							memcpy(&msgCharacRes.CHARAC_CUR_BASIC, &pCharac->CHARAC_CUR_BASIC, sizeof(_CHARAC_CUR_BASIC) ) ;
							memcpy(&msgCharacRes.CHARAC_LEVEL, &pCharac->CHARAC_LEVEL, sizeof(_CHARAC_LEVEL) ) ;
							memcpy(&msgCharacRes.CHARAC_STATE, &pCharac->CHARAC_STATE, sizeof(_CHARAC_STATE) ) ;

							if( 0 == fromType )
								g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&msgCharacRes, sizeof(MSG_CHARAC_RES));
							else
								pConObj->SendRequest( &msgCharacRes, sizeof(MSG_CHARAC_RES) );


							g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Charac info respone (%s) \n", pMsgCharacReq->cChrName ) ;
						}
						else
						{
							::PrintConsole("[ERROR] pCharac == NULL (%s) \n", pMsgCharacReq->cChrName) ;
						}
						continue;
					}
					break;
				case MSG_CHARAC_UPDATE_NUM:
					// 캐릭터 정보 업데이트
					{

						pos = 13 ;

						MSG_CHARAC_UPDATE *		pMsgCharacUpdate = NULL;
						pMsgCharacUpdate = (MSG_CHARAC_UPDATE *)buf;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_CHARAC_UPDATE_NUM (%s) \n", pMsgCharacUpdate->cChrName ) ;
						
						pCharac = g_pAccMapManager->Find_Charac(pMsgCharacUpdate->cChrName);
						
						//char * pAccount = pCharac->CHARAC_BASIC.cAccount ;

						if(pCharac){
							memcpy(&pCharac->CHARAC_BASIC, &pMsgCharacUpdate->CHARAC_BASIC, sizeof(_CHARAC_BASIC)) ;
							//pCharac->CHARAC_BASIC.pAccount = pAccount ;
							memcpy(&pCharac->CHARAC_CUR_BASIC, &pMsgCharacUpdate->CHARAC_CUR_BASIC, sizeof(_CHARAC_CUR_BASIC)) ;
							memcpy(&pCharac->CHARAC_LEVEL, &pMsgCharacUpdate->CHARAC_LEVEL, sizeof(_CHARAC_LEVEL) ) ;
							memcpy(&pCharac->CHARAC_STATE, &pMsgCharacUpdate->CHARAC_STATE, sizeof(_CHARAC_STATE) ) ;					

							pCharac->CHARAC_ETC._backup.bBackupStateBasic = BS_READY ;
							pCharac->CHARAC_ETC._backup.bBackupStateCurBasicInfo = BS_READY ;
							pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_READY ;										
							pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;

							pCharac->charac_lock() ;
							try{
								g_UDP_DB_Command->Update_Charac_All( pCharac->CHARAC_BASIC.iUniqueID, pCharac ) ;
							}
							catch(...){
								::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
							}
							pCharac->charac_unlock() ;
							
						}
						continue;
					}
					break;
				case MSG_USER_DEPOT_REQ_NUM:
					{

						pos = 14 ;
						MSG_USER_DEPOT_REQ * pMsgUserDepotReq = (MSG_USER_DEPOT_REQ*)buf ;


						if( pMsgUserDepotReq->cAccount[0] == 0 ) {
							::PrintConsole("[ME_ERR] if( pMsgUserDepotReq->cAccount[0] == 0 ) \n") ;
							continue ;
						}

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_USER_DEPOT_REQ_NUM (acc:%s) \n", pMsgUserDepotReq->cAccount ) ;
						
						pUser = g_pAccMapManager->Find_User(pMsgUserDepotReq->cAccount) ;
						
						if( pUser==NULL )
						{
							pUser = g_pAccMapManager->GetWritePtr();	// Get pointer(map) vitual memory

							if( pUser == NULL )
								continue ;

							//memset(pUser,0,sizeof(_USER));
							memcpy( pUser->cNDAccount, pMsgUserDepotReq->cAccount, en_max_lil+1) ;
							pUser->iTime = CUser::get_time_by_sec() ;

							bool bRes = g_UDP_USER_DB_Command->DSGetAccountUID( pUser->cNDAccount, pUser->iUid ) ;
							if( bRes == false ) {
								// 맵을 다시 반환한다..
						
								g_pAccMapManager->PutMemPtr( pUser ) ;
						
								continue ;
							}

							g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] GetDepot (uniqueID: %d) \n", pUser->iUid);
	
							if(g_UDP_DB_Command->pr_GetDepot( pUser->iUid, pUser ) == false){
								::PrintConsole("[DB_ERR] GetDepot failed (account: %s) \n", pUser->cNDAccount );

								// 맵을 다시 반환한다..
						
								g_pAccMapManager->PutMemPtr( pUser ) ;
						

								continue;
							}

							if( false == g_UDP_DB_Command->GetAccountItemUseStatus( pUser->iUid, pUser ) )
							{
								pos = 11;

								PrintConsole( "  => DB Loading(GetAccountItemUseStatus) failed (account: %s : %d) \n", pUser->cNDAccount,pUser->iUid );

								g_pAccMapManager->PutMemPtr( pUser ) ;		// 실패했을 경우 맵을 다시 반환한다..

								continue ;
							}

							// Get Character info
							if(	g_UDP_DB_Command->Get_Charac_List( pUser->iUid, pUser )  == false){		// Read charac list from DB								
								::PrintConsole("[DB_ERR] : CID - %s Get failed what user info \n", pUser->cNDAccount);
								// 맵을 다시 반환한다..
								g_pAccMapManager->PutMemPtr( pUser ) ;
								continue;
							}


							if(pUser)
							{
								g_pAccMapManager->Insert(pUser);
							}	
						}						


						if( pUser ) 
						{
							g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Account depot info request form GMS (%s) \n", pMsgUserDepotReq->cAccount) ;

							MSG_USER_DEPOT_RES msgUserDepotRes;
							msgUserDepotRes.header.iKey = PASS_KEY ;
							msgUserDepotRes.header.ucMessage = MSG_USER_DEPOT_RES_NUM ;
							memcpy(msgUserDepotRes.cAccount, pUser->cNDAccount, en_max_lil+1) ;

							char * DepotBuf = (char*)&pUser->stUserDepot ;
							int count = sizeof(_USER_DEPOT) / UDP_PACKET_CUT_SIZE ;
							int i = 0 ;
							for( i = 0; i < count; i++ )
							{
								msgUserDepotRes.cNum = i ;
								msgUserDepotRes.iSize = UDP_PACKET_CUT_SIZE ;
								memcpy( &msgUserDepotRes.pInvData, &DepotBuf[i*UDP_PACKET_CUT_SIZE], UDP_PACKET_CUT_SIZE ) ;

								if( 0 == fromType )
                                    g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&msgUserDepotRes, sizeof(MSG_USER_DEPOT_RES));
								else
									pConObj->SendRequest( &msgUserDepotRes, sizeof(MSG_USER_DEPOT_RES) );
								
							}

							msgUserDepotRes.cNum = i ;
							msgUserDepotRes.iSize = sizeof(_USER_DEPOT) - (i*UDP_PACKET_CUT_SIZE);
							memcpy( &msgUserDepotRes.pInvData, 
								&DepotBuf[i*UDP_PACKET_CUT_SIZE], 
								msgUserDepotRes.iSize ) ;

							if( 0 == fromType )
								g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&msgUserDepotRes, sizeof(MSG_USER_DEPOT_RES));
							else
								pConObj->SendRequest( &msgUserDepotRes, sizeof(MSG_USER_DEPOT_RES) );
							
						}
						continue;
					}
					break;
				case MSG_INVEN_REQ_NUM:
					{

						pos = 15 ;

						int x = 0 ;
						try{
							MSG_INVEN_REQ * pMsgInvenReq = (MSG_INVEN_REQ*)buf ;

							g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_INVEN_REQ_NUM (chr:%s)\n", pMsgInvenReq->cCharacName) ;

							pCharac = g_pAccMapManager->Find_Charac(pMsgInvenReq->cCharacName) ;

							x = 1 ;

							if( pCharac ) 
							{
								g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Charac inventory info request from GMS (%s) \n", pMsgInvenReq->cCharacName) ;

								MSG_INVEN_RES msgInvenRes;
								msgInvenRes.header.iKey = PASS_KEY ;
								msgInvenRes.header.ucMessage = MSG_INVEN_RES_NUM ;
								memcpy(msgInvenRes.cCharacName, pCharac->CHARAC_BASIC.cChrName, 13) ;

								x =2 ;

								char * InvenBuf = (char*)&pCharac->CHARAC_INVENTORY ;
								int count = sizeof(_CHARAC_INVENTORY) / UDP_PACKET_CUT_SIZE ;
								int i = 0 ;
								for( i = 0; i < count; i++ )
								{
									msgInvenRes.cNum = i ;
									msgInvenRes.iSize = UDP_PACKET_CUT_SIZE  ;
									memcpy( msgInvenRes.pInvData, &InvenBuf[i*UDP_PACKET_CUT_SIZE], UDP_PACKET_CUT_SIZE ) ;
									
									if( 0 == fromType )
										g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&msgInvenRes, sizeof(MSG_INVEN_RES) );
									else
										pConObj->SendRequest( &msgInvenRes, sizeof(MSG_INVEN_RES) );
									
								}
								x=4 ;

								msgInvenRes.cNum = i ;
								msgInvenRes.iSize = sizeof(_CHARAC_INVENTORY) - (i*UDP_PACKET_CUT_SIZE)  ;
								memcpy( msgInvenRes.pInvData, 
									&InvenBuf[i*UDP_PACKET_CUT_SIZE], 
									msgInvenRes.iSize ) ;

								x =5;

								if( 0 == fromType )
									g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&msgInvenRes, sizeof(MSG_INVEN_RES) );
								else
									pConObj->SendRequest( &msgInvenRes, sizeof(MSG_INVEN_RES) );

								x=6;
							}
						}
						catch (...) {
							::PrintConsole("[] -> x : %d \n", x) ;
						}
					}
					break;
				case MSG_USER_DEPOT_UPDATE_NUM:
					{
						pos = 16 ;

						MSG_USER_DEPOT_UPDATE * pMsgUserDepotUpdate = (MSG_USER_DEPOT_UPDATE*)buf ;

						if( pMsgUserDepotUpdate->cAccount[0] == 0 ){
							::PrintConsole("[ME_ERR] pMsgUserDepotUpdate->cAccount[0] == 0 \n") ;
							continue ;
						}

						PrintConsole( "[INFO] Recv MSG_USER_DEPOT_UPDATE_NUM (acc:%s)\n", pMsgUserDepotUpdate->cAccount) ;
						
						pUser = g_pAccMapManager->Find_User(pMsgUserDepotUpdate->cAccount) ;

						if( pUser ) 
						{
							memcpy(&pUser->stUserDepot, &pMsgUserDepotUpdate->stDepot, sizeof(_USER_DEPOT)) ;

							pUser->user_lock() ;
							try{
								if( !g_UDP_DB_Command->UpdateAccountItemUseStatus( pUser->iUid, pUser ) )
								{
									PrintConsole( "[DB_ERR] AIUS Backup failed (%s)\n ", pUser->cNDAccount );
								}

								if(!g_UDP_DB_Command->pr_UpdateDepot(pUser->iUid, pUser) ) 
								{
									// 로그에 남기자..
									PrintConsole( "[DB_ERR] Failed backup what account depot (%s) ", pUser->cNDAccount );						}		
								else
								{							
									pUser->cDepotBackupState = BS_COMPLETED;							
									PrintConsole( "[BACKUP] Backup Account's depot ( Charac name : %s ) \n", pUser->cNDAccount ) ;		
								}
							}
							catch(...){
								__EXCEPT_STR_PRINT() ;
							}							
							pUser->user_unlock() ;

							PrintConsole( "[GMS] Depot is saved to DB. (%s) \n", pCharac->CHARAC_BASIC.cChrName) ;
						}
					}
					break;
				case MSG_INVEN_UPDATE_NUM:
					{
						pos = 17 ;

						MSG_INVEN_UPDATE * pMsgInvenUpdate = (MSG_INVEN_UPDATE*)buf ;

						PrintConsole( "[INFO] Recv MSG_INVEN_UPDATE_NUM (chr:%s) \n", pMsgInvenUpdate->cCharacName ) ;

						pCharac = g_pAccMapManager->Find_Charac(pMsgInvenUpdate->cCharacName) ;

						if( pCharac ) 
						{
							memcpy(&pCharac->CHARAC_INVENTORY, &pMsgInvenUpdate->stInven, sizeof(_CHARAC_INVENTORY)) ;
							g_UDP_DB_Command->inven_update( pCharac->CHARAC_BASIC.iUniqueID, pCharac ) ;

							PrintConsole( "[GMS] Inventory is saved to DB. (%s) \n", pCharac->CHARAC_BASIC.cChrName) ;
						}
					}
					break;
				case MSG_QUEST_REQ_NUM:
					{
						pos = 18;

   						MSG_QUEST_REQ * pMsgQuestReq = (MSG_QUEST_REQ*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_QUEST_REQ_NUM (%s) \n", pMsgQuestReq->cCharacName) ;
						
						pCharac = g_pAccMapManager->Find_Charac(pMsgQuestReq->cCharacName) ;
		
						if( pCharac )
						{
							// 나눠서 보내자
							MSG_QUEST_RES msgQuestRes;
							msgQuestRes.header.iKey = PASS_KEY ;
							msgQuestRes.header.ucMessage = MSG_QUEST_RES_NUM ;
							memcpy( msgQuestRes.cCharacName, pCharac->CHARAC_BASIC.cChrName, 13 ) ;
							
							char * cBuf = (char*)&pCharac->CHARAC_QUEST_DATA ;
							int count = sizeof(_CHARAC_QUEST_DATA) / UDP_PACKET_CUT_SIZE ;
							int i = 0 ;
							for( i = 0; i < count; i++ )
							{
								msgQuestRes.cNum = i ;
								msgQuestRes.iSize = UDP_PACKET_CUT_SIZE ;
								memcpy( &msgQuestRes.pQuestData, &cBuf[i*UDP_PACKET_CUT_SIZE], UDP_PACKET_CUT_SIZE ) ;

								if( 0 == fromType )
									g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&msgQuestRes, sizeof(MSG_QUEST_RES) );
								else
									pConObj->SendRequest( &msgQuestRes, sizeof(MSG_QUEST_RES) );

							}

							msgQuestRes.cNum = i ;
							msgQuestRes.iSize = sizeof(_CHARAC_QUEST_DATA) - (i*UDP_PACKET_CUT_SIZE) ;
							memcpy( &msgQuestRes.pQuestData, 
								&cBuf[i*UDP_PACKET_CUT_SIZE], 
								msgQuestRes.iSize ) ;
							
							if( 0 == fromType )
								g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&msgQuestRes, sizeof(MSG_QUEST_RES) );
							else
								pConObj->SendRequest( &msgQuestRes, sizeof(MSG_QUEST_RES) );
						}
					}
					break;
					
				case MSG_GM_EDIT_MAINQUEST_TRG_NUM:
					{
						MSG_GM_EDIT_MAINQUEST_TRG * pMsgGmEditMainQuestTrg = (MSG_GM_EDIT_MAINQUEST_TRG*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] MSG_GM_EDIT_MAINQUEST_TRG_NUM (chr:%s) \n", 
													pMsgGmEditMainQuestTrg->cCharacName );

						PCHARAC pCharac = g_pAccMapManager->Find_Charac(pMsgGmEditMainQuestTrg->cCharacName) ;

						if( pCharac )						
						{
							_sCharacMainQuestState * pMainQuest = (_sCharacMainQuestState *)pCharac->CHARAC_QUEST_DATA.cMainQuestData ;

							pMainQuest->ucNodeID = pMsgGmEditMainQuestTrg->cTrigger ;

							_sRunningQuest * pRunningQuest = (_sRunningQuest*)pCharac->CHARAC_QUEST_DATA.cCurQuestData ;
							pRunningQuest[0].cNodeID = pMsgGmEditMainQuestTrg->cTrigger;

							pCharac->CHARAC_ETC._backup.bBackupStateQuest = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}
					}
					break;

				case MSG_GM_ADD_DOINGQUEST_NUM:
					{
						MSG_GM_ADD_DOINGQUEST * pMsgGmAddDoingQuest = (MSG_GM_ADD_DOINGQUEST*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] MSG_GM_ADD_DOINGQUEST_NUM (chr:%s) \n", 
							pMsgGmAddDoingQuest->cCharacName );

						PCHARAC pCharac = g_pAccMapManager->Find_Charac(pMsgGmAddDoingQuest->cCharacName) ;

						if( pCharac )
						{
							_sRunningQuest * pRunningQuest = (_sRunningQuest*)pCharac->CHARAC_QUEST_DATA.cCurQuestData ;
							for( int i = 1  ; i < _MAX_CURQUEST_NUM_ ; i++ )
							{
								if( pRunningQuest->bBeUse == false ) 
								{
									++pCharac->CHARAC_QUEST_DATA.ucCurQuestNum ;
									pRunningQuest->bBeUse = true ;
									pRunningQuest->usQuestID = pMsgGmAddDoingQuest->sQuestID ;
									pRunningQuest->cNodeID = pMsgGmAddDoingQuest->cQuestTrigger ;
									pRunningQuest->dwQuestStartTime = pMsgGmAddDoingQuest->iTime ;

#ifdef _PD_QUEST_MAX_600_
									_sCharacQuestState * pCharacQuestState = (_sCharacQuestState*)pCharac->CHARAC_QUEST_DATA.cQuestData ;
									for( int i = 0  ; i < _QUEST_1_NUMBER_ ; i++ )
									{
										if( pCharacQuestState->usQuestID == 0 ) 
										{											
											pCharacQuestState->usQuestID = pMsgGmAddDoingQuest->sQuestID ;
											pCharacQuestState->ucNodeID = pMsgGmAddDoingQuest->cQuestTrigger ;
											pCharacQuestState->bReword = false ;											
											break ;
										}
										pCharacQuestState++ ;
									}
									pCharacQuestState = (_sCharacQuestState*)pCharac->CHARAC_QUEST_DATA.QuestData2.cQuestData2 ;
									for( int i = 0  ; i < _QUEST_2_NUMBER_ ; i++ )
									{
										if( pCharacQuestState->usQuestID == 0 ) 
										{											
											pCharacQuestState->usQuestID = pMsgGmAddDoingQuest->sQuestID ;
											pCharacQuestState->ucNodeID = pMsgGmAddDoingQuest->cQuestTrigger ;
											pCharacQuestState->bReword = false ;											
											break ;
										}
										pCharacQuestState++ ;
									}
#else
									_sCharacQuestState * pCharacQuestState = (_sCharacQuestState*)pCharac->CHARAC_QUEST_DATA.cQuestData ;
									for( int i = 0  ; i < _MAX_CURQUEST_NUM_ ; i++ )
									{
										if( pCharacQuestState->usQuestID == 0 ) 
										{											
											pCharacQuestState->usQuestID = pMsgGmAddDoingQuest->sQuestID ;
											pCharacQuestState->ucNodeID = pMsgGmAddDoingQuest->cQuestTrigger ;
											pCharacQuestState->bReword = false ;											
											break ;
										}
										pCharacQuestState++ ;
									}
#endif

									break ;
								}
								pRunningQuest++ ;
							}

							pCharac->CHARAC_ETC._backup.bBackupStateQuest = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break;

					//struct _CHARAC_QUEST_DATA 
					//{
					//	//	ND_V01_User_Quest_Data
					//	BYTE    ucCurQuestNum;     // 현 몇개의 퀘스트가 실행되고 있는가.
					//	BYTE    cCurQuestData[690];// 런닝 퀘스트 저장. 런닝5개 + 제룡 1개 // _sRunningQuest[6]
					//	BYTE    cMainQuestData[4]; // 제룡행 퀘스트 저장.		// _sCharacMainQuestState[1]
					//	BYTE    cQuestData[240];   // 기연행 협객행 저장		// _sCharacQuestState[80]
					//};

				case MSG_GM_REM_DOINGQUEST_NUM:
					{
						MSG_GM_REM_DOINGQUEST * pMsgGmRemDoingQuest = (MSG_GM_REM_DOINGQUEST*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] MSG_GM_REM_DOINGQUEST_NUM (chr:%s) \n", 
							pMsgGmRemDoingQuest->cCharacName );

						PCHARAC pCharac = g_pAccMapManager->Find_Charac(pMsgGmRemDoingQuest->cCharacName) ;

						if( pCharac )
						{
							_sRunningQuest * pRunningQuest = (_sRunningQuest*)pCharac->CHARAC_QUEST_DATA.cCurQuestData ;
							for( int i = 0  ; i < _MAX_CURQUEST_NUM_ ; i++ )
							{
								if( pRunningQuest->usQuestID == pMsgGmRemDoingQuest->sQuestID ) 
								{
									--pCharac->CHARAC_QUEST_DATA.ucCurQuestNum ;
									memset( pRunningQuest, 0 , sizeof(_sRunningQuest) ) ;								

#ifdef _PD_QUEST_MAX_600_
									_sCharacQuestState * pCharacQuestState = (_sCharacQuestState*)pCharac->CHARAC_QUEST_DATA.cQuestData ;
									for( int i = 0  ; i < _QUEST_1_NUMBER_ ; i++ )
									{
										if( pCharacQuestState->usQuestID == pMsgGmRemDoingQuest->sQuestID ) 
										{									
											memset( pCharacQuestState, 0 , sizeof(_sCharacQuestState) ) ;								
											break ;
										}
										pCharacQuestState++ ;
									}

									pCharacQuestState = (_sCharacQuestState*)pCharac->CHARAC_QUEST_DATA.QuestData2.cQuestData2 ;
									for( int i = 0  ; i < _QUEST_2_NUMBER_ ; i++ )
									{
										if( pCharacQuestState->usQuestID == pMsgGmRemDoingQuest->sQuestID ) 
										{									
											memset( pCharacQuestState, 0 , sizeof(_sCharacQuestState) ) ;								
											break ;
										}
										pCharacQuestState++ ;
									}
#else
									_sCharacQuestState * pCharacQuestState = (_sCharacQuestState*)pCharac->CHARAC_QUEST_DATA.cQuestData ;
									for( int i = 0  ; i < _QUEST_MAX_NUMBER_ ; i++ )
									{
										if( pCharacQuestState->usQuestID == pMsgGmRemDoingQuest->sQuestID ) 
										{									
											memset( pCharacQuestState, 0 , sizeof(_sCharacQuestState) ) ;								
											break ;
										}
										pCharacQuestState++ ;
									}
#endif

									break ;
								}
								pRunningQuest++ ;
							}							

							pCharac->CHARAC_ETC._backup.bBackupStateQuest = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}						

					}
					break;

				case MSG_GM_EDIT_DOINGQUEST_TRG_NUM:
					{
						MSG_EDIT_DOINGQUEST_TRG * pMsgGmEditDoingQuestTrg = (MSG_EDIT_DOINGQUEST_TRG*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] MSG_GM_EDIT_DOINGQUEST_TRG_NUM (chr:%s) \n", 
							pMsgGmEditDoingQuestTrg->cCharacName );

						PCHARAC pCharac = g_pAccMapManager->Find_Charac(pMsgGmEditDoingQuestTrg->cCharacName) ;

						if( pCharac)
						{
							_sRunningQuest * pRunningQuest = (_sRunningQuest*)pCharac->CHARAC_QUEST_DATA.cCurQuestData ;							
							for( int i = 0  ; i < _MAX_CURQUEST_NUM_ ; i++ )
							{
								if( pRunningQuest->usQuestID == pMsgGmEditDoingQuestTrg->sQuestID ) 
								{
									pRunningQuest->cNodeID = pMsgGmEditDoingQuestTrg->cQuestTrigger;

#ifdef _PD_QUEST_MAX_600_
									_sCharacQuestState * pQuestState = (_sCharacQuestState *)pCharac->CHARAC_QUEST_DATA.cQuestData ;
									for( int i = 0 ; i < _QUEST_1_NUMBER_ ; i++ )
									{
										if(pQuestState->usQuestID == pMsgGmEditDoingQuestTrg->sQuestID )
										{
											pQuestState->ucNodeID = pMsgGmEditDoingQuestTrg->cQuestTrigger ;
											break;
										}
										++pQuestState ;
									}

									pQuestState = (_sCharacQuestState *)pCharac->CHARAC_QUEST_DATA.QuestData2.cQuestData2 ;
									for( int i = 0 ; i < _QUEST_2_NUMBER_ ; i++ )
									{
										if(pQuestState->usQuestID == pMsgGmEditDoingQuestTrg->sQuestID )
										{
											pQuestState->ucNodeID = pMsgGmEditDoingQuestTrg->cQuestTrigger ;
											break;
										}
										++pQuestState ;
									}

#else
									_sCharacQuestState * pQuestState = (_sCharacQuestState *)pCharac->CHARAC_QUEST_DATA.cQuestData ;
									for( int i = 0 ; i < _QUEST_MAX_NUMBER_ ; i++ )
									{
										if(pQuestState->usQuestID == pMsgGmEditDoingQuestTrg->sQuestID )
										{
											pQuestState->ucNodeID = pMsgGmEditDoingQuestTrg->cQuestTrigger ;
											break;
										}
										++pQuestState ;
									}

#endif
									break;
								}
								pRunningQuest++ ;
							}				


							pCharac->CHARAC_ETC._backup.bBackupStateQuest = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
						
					}
					break;

				case MSG_GM_REM_DONEQUEST_NUM:
					{
						MSG_GM_REM_DONEQUEST * pMsgGmRemDoneQuest = (MSG_GM_REM_DONEQUEST*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] MSG_GM_REM_DONEQUEST_NUM (chr:%s) \n", 
							pMsgGmRemDoneQuest->cCharacName );

						PCHARAC pCharac = g_pAccMapManager->Find_Charac(pMsgGmRemDoneQuest->cCharacName) ;
						if( pCharac )
						{							

#ifdef _PD_QUEST_MAX_600_

							_sCharacQuestState * pQuestState = (_sCharacQuestState*)pCharac->CHARAC_QUEST_DATA.cQuestData ;
							for( int i = 0 ; i < _QUEST_1_NUMBER_ ; i++ )
							{
								if( pQuestState->usQuestID == pMsgGmRemDoneQuest->sQuestID )								
								{		
									--pCharac->CHARAC_QUEST_DATA.sCompleteQuestNum ;
									memset( pQuestState, 0, sizeof(_sCharacQuestState) ) ;
									break;
								}
								pQuestState++ ;
							}

							pQuestState = (_sCharacQuestState*)pCharac->CHARAC_QUEST_DATA.QuestData2.cQuestData2 ;
							for( int i = 0 ; i < _QUEST_2_NUMBER_ ; i++ )
							{
								if( pQuestState->usQuestID == pMsgGmRemDoneQuest->sQuestID )								
								{		
									--pCharac->CHARAC_QUEST_DATA.sCompleteQuestNum ;
									memset( pQuestState, 0, sizeof(_sCharacQuestState) ) ;
									break;
								}
								pQuestState++ ;
							}

#else
							_sCharacQuestState * pQuestState = (_sCharacQuestState*)pCharac->CHARAC_QUEST_DATA.cQuestData ;
							for( int i = 0 ; i < _QUEST_MAX_NUMBER_ ; i++ )
							{
								if( pQuestState->usQuestID == pMsgGmRemDoneQuest->sQuestID )								
								{		
									--pCharac->CHARAC_QUEST_DATA.sCompleteQuestNum ;
									memset( pQuestState, 0, sizeof(_sCharacQuestState) ) ;
									break;
								}
								pQuestState++ ;
							}
#endif

							pCharac->CHARAC_ETC._backup.bBackupStateQuest = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}
						
					}
					break;


				case MSG_QUEST_UPDATE_NUM:
					{

						pos = 19 ;

						MSG_QUEST_UPDATE * pMsgQuestUpdate = (MSG_QUEST_UPDATE*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_QUEST_UPDATE_NUM (chr:%s) \n", pMsgQuestUpdate->cCharacName ) ;
						
						pCharac = g_pAccMapManager->Find_Charac(pMsgQuestUpdate->cCharacName) ;
						
						if( pCharac )
						{					
							memcpy( &pCharac->CHARAC_QUEST_DATA, &pMsgQuestUpdate->CHARAC_QUEST_DATA, sizeof(_CHARAC_QUEST_DATA)) ;	
							g_UDP_DB_Command->pr_Update_QuestInfo(pCharac->CHARAC_BASIC.iUniqueID, pCharac) ;

							g_pTrace->OutputString( _TRACE_CLASS_GMS, "[GMS] Quest data is saved to DB (%s) \n", pCharac->CHARAC_BASIC.cChrName) ;

							pCharac->CHARAC_ETC._backup.bBackupStateQuest = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}		
						continue;
					}

					break;

				case MSG_MARTIAL_REQ_NUM:
					{

						pos = 20 ;

						MSG_MARTIAL_REQ * pMsgMartialReq = (MSG_MARTIAL_REQ*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_MARTIAL_REQ_NUM (chr:%s)\n", pMsgMartialReq->cCharacName) ;
	
						pCharac = g_pAccMapManager->Find_Charac(pMsgMartialReq->cCharacName) ;

						if( pCharac )
						{
							memset( &msgMartialRes, 0, sizeof(msgMartialRes)) ;
							msgMartialRes.header.iKey = PASS_KEY ;
							msgMartialRes.header.ucMessage = MSG_MARTIAL_RES_NUM ;
							
							memcpy( msgMartialRes.cCharacName, pCharac->CHARAC_BASIC.cChrName, 13) ;
							memcpy( &msgMartialRes.CHARAC_MARTIAL_DATA, &pCharac->CHARAC_MARTIAL_DATA, sizeof(_CHARAC_MARTIAL_DATA)) ;
							
							if( 0 == fromType )
								g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&msgMartialRes, sizeof(msgMartialRes));
							else
								pConObj->SendRequest( &msgMartialRes, sizeof(msgMartialRes) );

							
							pCharac->CHARAC_ETC._backup.bBackupStateMartial = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
						continue;
					}
					break;

	
				//#define		MSG_GM_EDIT_LEVEL_NUM			61		//	레벨 에디트
				case	MSG_GM_EDIT_LEVEL_NUM:
					{

						MSG_GM_EDIT_LEVEL * pMsgGmEditLevel = (MSG_GM_EDIT_LEVEL*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_LEVEL_NUM (chr:%s) \n", pMsgGmEditLevel->cCharacName) ;
		
						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditLevel->cCharacName ) ;					

						if( pCharac )
						{							
							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								CMSG_GM_EDIT_LEVEL	msgGmEditLevel ={0} ;
								msgGmEditLevel.usLength = sizeof(CMSG_GM_EDIT_LEVEL) ;
								msgGmEditLevel.cMessage = CMSG_GM_EDIT_LEVEL_NUM ;
								memcpy( msgGmEditLevel.cCharacName, pMsgGmEditLevel->cCharacName, 13 ) ;
								msgGmEditLevel.iLevel = pMsgGmEditLevel->iLevel ;
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditLevel, msgGmEditLevel.usLength ) ;
							}

							pCharac->CHARAC_STATE.sInnerLevel = pMsgGmEditLevel->iLevel ;

							pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break;
				//#define		MSG_GM_EDIT_VITAL_NUM			62		//	생 에디트
				case MSG_GM_EDIT_VITAL_NUM :
					{
                        MSG_GM_EDIT_VITAL * pMsgGmEditVital = (MSG_GM_EDIT_VITAL*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_VITAL_NUM (chr:%s) \n", pMsgGmEditVital->cCharacName ) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditVital->cCharacName ) ;

						if( pCharac )
						{
							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								CMSG_GM_EDIT_VITAL msgGmEditVital = {0} ;
								msgGmEditVital.usLength = sizeof(CMSG_GM_EDIT_VITAL) ;
								msgGmEditVital.cMessage = CMSG_GM_EDIT_VITAL_NUM ;
								memcpy( msgGmEditVital.cCharacName, pMsgGmEditVital->cCharacName, 13 ) ;
								msgGmEditVital.sVital = pMsgGmEditVital->sVital ;
								msgGmEditVital.uiHP = pMsgGmEditVital->uiHP ;
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditVital, msgGmEditVital.usLength ) ;
							}
							pCharac->CHARAC_LEVEL.sConstitution = pMsgGmEditVital->sVital ;
							pCharac->CHARAC_LEVEL.sMaxLifePower = pMsgGmEditVital->uiHP ;

							pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}

					}
					break ;
				//#define		MSG_GM_EDIT_ZEN_NUM				63		//	진 에디트
				case MSG_GM_EDIT_ZEN_NUM :
					{

						MSG_GM_EDIT_ZEN * pMsgGmEditZen = (MSG_GM_EDIT_ZEN*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_ZEN_NUM (chr:%s) \n", pMsgGmEditZen->cCharacName) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditZen->cCharacName ) ;

						if( pCharac )
						{
							CMSG_GM_EDIT_ZEN msgGmEditZen = {0} ;
							msgGmEditZen.usLength = sizeof(CMSG_GM_EDIT_ZEN) ;
							msgGmEditZen.cMessage = CMSG_GM_EDIT_ZEN_NUM ;
							memcpy( msgGmEditZen.cCharacName, pMsgGmEditZen->cCharacName, 13) ;
							msgGmEditZen.sZen = pMsgGmEditZen->sZen ;
							msgGmEditZen.uiMP = pMsgGmEditZen->uiMP ;
							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditZen, msgGmEditZen.usLength ) ;
							}
							pCharac->CHARAC_LEVEL.sZen = pMsgGmEditZen->sZen ;
							pCharac->CHARAC_LEVEL.sMaxForcePower = pMsgGmEditZen->uiMP ;

							pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break ;
				//#define		MSG_GM_EDIT_SOUL_NUM			64		//	지 에디트
				case MSG_GM_EDIT_SOUL_NUM:
					{

						MSG_GM_EDIT_SOUL * pMsgGmEditSoul = (MSG_GM_EDIT_SOUL*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_SOUL_NUM (%s)\n", pMsgGmEditSoul->cCharacName ) ;
						
						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditSoul->cCharacName ) ;

						if( pCharac )
						{	
							CMSG_GM_EDIT_SOUL msgGmEditSoul = {0} ;
							msgGmEditSoul.usLength = sizeof(CMSG_GM_EDIT_SOUL) ;
							msgGmEditSoul.cMessage = CMSG_GM_EDIT_SOUL_NUM ;
							memcpy( msgGmEditSoul.cCharacName, pMsgGmEditSoul->cCharacName, 13) ;
							msgGmEditSoul.sSoul = pMsgGmEditSoul->sSoul ;
							msgGmEditSoul.uiSoul =  pMsgGmEditSoul->uiSoul ;
							
							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditSoul, msgGmEditSoul.usLength ) ;
							}
							pCharac->CHARAC_LEVEL.sIntelligence = pMsgGmEditSoul->sSoul ;
							pCharac->CHARAC_LEVEL.sMaxConcentrationPower = pMsgGmEditSoul->uiSoul ;

							pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break ;
				//#define		MSG_GM_EDIT_HEART_NUM			65		//	민 에디트
				case MSG_GM_EDIT_HEART_NUM:
					{

						MSG_GM_EDIT_HEART * pMsgGmEditHeart = (MSG_GM_EDIT_HEART*)buf ; 

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_HEART_NUM (%s) \n", pMsgGmEditHeart->cCharacName) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditHeart->cCharacName ) ;

						if( pCharac )
						{	
							CMSG_GM_EDIT_HEART msgGmEditHeart = {0} ;
							msgGmEditHeart.usLength = sizeof(CMSG_GM_EDIT_HEART) ;
							msgGmEditHeart.cMessage = CMSG_GM_EDIT_HEART_NUM ;
							memcpy( msgGmEditHeart.cCharacName, pMsgGmEditHeart->cCharacName, 13) ;
							msgGmEditHeart.sDexterity = pMsgGmEditHeart->sDexterity ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditHeart, msgGmEditHeart.usLength ) ;
							}
							pCharac->CHARAC_LEVEL.sDexterity = pMsgGmEditHeart->sDexterity ;

							pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break ;
				//#define		MSG_GM_EDIT_JUNGGI_NUM			66		//	근 에디트
				case MSG_GM_EDIT_JUNGGI_NUM:
					{

						MSG_GM_EDIT_JUNGGI * pMsgGmEditJunggi = (MSG_GM_EDIT_JUNGGI*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_JUNGGI_NUM (%s)\n", pMsgGmEditJunggi->cCharacName) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditJunggi->cCharacName ) ;

						if( pCharac )
						{	
							CMSG_GM_EDIT_JUNGGI msgGmEditJunggi = {0} ;
							msgGmEditJunggi.usLength = sizeof(CMSG_GM_EDIT_JUNGGI ) ;
							msgGmEditJunggi.cMessage = CMSG_GM_EDIT_JUNGGI_NUM	;
							memcpy( msgGmEditJunggi.cCharacName, pMsgGmEditJunggi->cCharacName, 13) ;
							msgGmEditJunggi.sStr = pMsgGmEditJunggi->sStr ;
							
							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditJunggi, msgGmEditJunggi.usLength ) ;
							}
							pCharac->CHARAC_LEVEL.sStr = pMsgGmEditJunggi->sStr ;

							pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break ;
				//#define		MSG_GM_EDIT_LEFTPOINT_NUM		67		//	남은 포인트 에디트
				case MSG_GM_EDIT_LEFTPOINT_NUM:
					{

						MSG_GM_EDIT_LEFTPOINT * pMsgGmEditLeftPoint = (MSG_GM_EDIT_LEFTPOINT*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_LEFTPOINT_NUM (%s)\n", pMsgGmEditLeftPoint->cCharacName) ;
					
						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditLeftPoint->cCharacName ) ;

						if( pCharac )
						{	
							CMSG_GM_EDIT_LEFTPOINT msgGmEditLeftpoint = {0} ;
							msgGmEditLeftpoint.usLength = sizeof(CMSG_GM_EDIT_LEFTPOINT) ;
							msgGmEditLeftpoint.cMessage = CMSG_GM_EDIT_LEFTPOINT_NUM ;
							memcpy( msgGmEditLeftpoint.cCharacName, pMsgGmEditLeftPoint->cCharacName, 13 ) ;
							msgGmEditLeftpoint.sLeftPoint = pMsgGmEditLeftPoint->sLeftPoint ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditLeftpoint, msgGmEditLeftpoint.usLength ) ;
							}
							pCharac->CHARAC_LEVEL.sLeftPoint = pMsgGmEditLeftPoint->sLeftPoint ;

							pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break;
				//#define		MSG_GM_EDIT_ZONE_NUM			68		//	존 바꾸기
				case MSG_GM_EDIT_ZONE_NUM:
					{

						MSG_GM_EDIT_ZONE * pMsgGmEditZone = (MSG_GM_EDIT_ZONE*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_ZONE_NUM To %d (%s)\n", pMsgGmEditZone->iZone, pMsgGmEditZone->cCharacName) ;

						if( GS_OK != g_ServerInfo.SelectState( static_cast<char>(pMsgGmEditZone->iZone) ) )
						{
							break;
						}
				
						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditZone->cCharacName ) ;

						if( pCharac )
						{	
							CMSG_GM_EDIT_ZONE msgGmEditZone = {0} ;
							msgGmEditZone.usLength = sizeof(CMSG_GM_EDIT_ZONE) ;
							msgGmEditZone.cMessage = CMSG_GM_EDIT_ZONE_NUM ;
							memcpy( msgGmEditZone.cCharacName, pMsgGmEditZone->cCharacName, 13 ) ;
							msgGmEditZone.iZone = pMsgGmEditZone->iZone ;
							msgGmEditZone.fX = pMsgGmEditZone->fX ;
							msgGmEditZone.fZ = pMsgGmEditZone->fZ ;
							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditZone, msgGmEditZone.usLength ) ;
							}
							pCharac->CHARAC_CUR_BASIC.sZone = pMsgGmEditZone->iZone ;
							pCharac->CHARAC_CUR_BASIC.fX = pMsgGmEditZone->fX ;
							pCharac->CHARAC_CUR_BASIC.fZ = pMsgGmEditZone->fZ ;

							pCharac->CHARAC_ETC._backup.bBackupStateCurBasicInfo = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break;
				//#define		MSG_GM_EDIT_CODE_NUM			69		//	코드 바꾸기
				case MSG_GM_EDIT_CODE_NUM:
					{

						MSG_GM_EDIT_CODE * pMsgGmEditCode = (MSG_GM_EDIT_CODE*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_CODE_NUM (%s)\n", pMsgGmEditCode->cCharacName) ;
						
						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditCode->cCharacName ) ;
						
						if( pCharac )
						{	
							CMSG_GM_EDIT_CODE msgGmEditCode = {0} ;
							msgGmEditCode.usLength = sizeof(CMSG_GM_EDIT_CODE) ;
							msgGmEditCode.cMessage = CMSG_GM_EDIT_CODE_NUM ;
							memcpy( msgGmEditCode.cCharacName, pMsgGmEditCode->cCharacName, 13 ) ;
							msgGmEditCode.fZ = pMsgGmEditCode->fZ ;
							msgGmEditCode.fX = pMsgGmEditCode->fX ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditCode, msgGmEditCode.usLength ) ;
							}
							pCharac->CHARAC_CUR_BASIC.fX = pMsgGmEditCode->fX ;
							pCharac->CHARAC_CUR_BASIC.fZ = pMsgGmEditCode->fZ ;

							pCharac->CHARAC_ETC._backup.bBackupStateCurBasicInfo = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break ;

				case MSG_GM_EDIT_GMCLASS_NUM:
					{
						MSG_GM_EDIT_GMCLASS * pMsgGmEdit_GMClass = (MSG_GM_EDIT_GMCLASS*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_GMCLASS_NUM (%s) \n", pMsgGmEdit_GMClass->cCharacName) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEdit_GMClass->cCharacName ) ;
						if( pCharac )
						{
							pCharac->CHARAC_BASIC.cGM_Check = pMsgGmEdit_GMClass->cClass ;							
							pCharac->CHARAC_STATE.iBlockingEndDate = pMsgGmEdit_GMClass->iBlockingEndTime ;

							UMSG_GM_CHARAC_SET_DS_MS	msgGmCharacSet = {0} ;
							msgGmCharacSet.iKey = PASS_KEY ;
							msgGmCharacSet.cMessage = UMSG_GM_CHARAC_SET_DS_MS_NUM ;
							memcpy( msgGmCharacSet.cCharacName, pMsgGmEdit_GMClass->cCharacName, 13 ) ;
							msgGmCharacSet.cClass = pMsgGmEdit_GMClass->cClass ;

							SOCKADDR_IN	* pAddr = g_ServerInfo.GetMSSockAddrIn() ;
							if( 0 == fromType )
								g_pUDPSendPacketQ->insert_udp( pAddr, (char*)&msgGmCharacSet, sizeof(UMSG_GM_CHARAC_SET_DS_MS) ) ;
							else 
								pConObj->SendRequest( &msgGmCharacSet, sizeof(msgGmCharacSet) );

							pCharac->CHARAC_ETC._backup.bBackupStateBasic = BS_READY ;
							pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}						
					}
					break;

				//#define		MSG_GM_EDIT_PARTY_NUM			60		//	파티 바꾸기
				case MSG_GM_EDIT_PARTY_NUM:
					{

						MSG_GM_EDIT_PARTY * pMsgGmEditParty = (MSG_GM_EDIT_PARTY*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_PARTY_NUM (%s) \n", pMsgGmEditParty->cCharacName ) ;
						
						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditParty->cCharacName ) ;

						if( pCharac )
						{	
							CMSG_GM_EDIT_PARTY msgGmEditParty = {0} ;
							msgGmEditParty.usLength = sizeof(CMSG_GM_EDIT_PARTY) ;
							msgGmEditParty.cMessage = CMSG_GM_EDIT_PARTY_NUM ;
							memcpy( msgGmEditParty.cCharacName, pMsgGmEditParty->cCharacName, 13) ;
							msgGmEditParty.cParty = pMsgGmEditParty->cParty ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditParty, msgGmEditParty.usLength ) ;
							}
							pCharac->CHARAC_BASIC.cParty = pMsgGmEditParty->cParty ;

							pCharac->CHARAC_ETC._backup.bBackupStateBasic = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break ;
				//#define		MSG_GM_EDIT_NAME_NUM			61		//	이름 바꾸기(같은 이름이 존재하는지 조사는 이미 했음)
				case  MSG_GM_EDIT_NAME_NUM:
					{
						MSG_GM_EDIT_NAME * pMsgGmEditName = (MSG_GM_EDIT_NAME*)buf ;					

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_NAME_NUM (%s -> %s) \n", pMsgGmEditName->cCharacName, pMsgGmEditName->Name) ;


						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditName->cCharacName ) ;
						_USER * pUser = g_pAccMapManager->Find_User( pCharac->CHARAC_BASIC.cAccount ) ;
						if( pCharac && pUser )
						{
							// 유저 상태 검사 (로그아웃 중이면 무시)
							if( pUser->cState != US_NONSTATE || pCharac->CHARAC_ETC.cState != CS_NONSTATE )
							{

								CMSG_USER_LOGOUT_DS_GS msgUserkill = {0} ;
								msgUserkill.usLength = sizeof(CMSG_USER_LOGOUT_DS_GS) ;
								msgUserkill.cMessage = CMSG_NO_USER_LOGOUT_DS_GS ;							

								msgUserkill.cReason = 1;	//운영자에 의한 로그아웃
								memcpy(msgUserkill.cNDAccount,  pUser->cNDAccount, en_max_lil+1);

								// DS -> GS
								_h_DS_Com * pDSCom ;
								if( pDSCom = g_ServerInfo.GetDSCom( pUser->iServer ) )
								{									
									if( pDSCom->Send( msgUserkill.usLength, (char *)&msgUserkill ) == false ){
										::PrintConsole("[SO_ERR] MSG_CMD_USERKILL_NUM \n");
									}
								}
							}
							if( g_UDP_DB_Command->CharacRename( pCharac->CHARAC_BASIC.cChrName, pMsgGmEditName->Name ) == true )
							{
								g_pAccMapManager->CharacRename( pCharac->CHARAC_BASIC.cChrName, pMsgGmEditName->Name ) ;
							}
						}
						else
						{
							g_UDP_DB_Command->CharacRename( pMsgGmEditName->cCharacName, pMsgGmEditName->Name ) ;
						}

					}
					break ;				
				//#define		MSG_GM_EDIT_NICK_NUM			62		//	닉네임 바꾸기(같은 닉네임이 존재하는지 조사는 이미 했음)
				case MSG_GM_EDIT_NICK_NUM :
					{

						MSG_GM_EDIT_NICK * pMsgGmEditNick = (MSG_GM_EDIT_NICK*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_NICK_NUM (%s)\n", pMsgGmEditNick->cCharacName) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditNick->cCharacName ) ;

						if( pCharac )
						{	
							CMSG_GM_EDIT_NICK msgGmEditNick = {0} ;
							msgGmEditNick.usLength = sizeof(CMSG_GM_EDIT_NICK) ;
							msgGmEditNick.cMessage = CMSG_GM_EDIT_NICK_NUM ;
							memcpy( msgGmEditNick.cCharacName, pMsgGmEditNick->cCharacName, 13) ;
							memcpy( msgGmEditNick.Name, pMsgGmEditNick->Name, 13 ) ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditNick, msgGmEditNick.usLength );
							}
							memcpy( pCharac->CHARAC_BASIC.cChrNic, pMsgGmEditNick->cCharacName, 13 ) ;

							pCharac->CHARAC_ETC._backup.bBackupStateBasic = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break ;

				case MSG_GM_ADD_INVITEM_NUM :
					{

						MSG_GM_ADD_INVITEM * pMsgGmAddInvitem = (MSG_GM_ADD_INVITEM*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_ADD_INVITEM_NUM (%s)\n", pMsgGmAddInvitem->cCharacName ) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmAddInvitem->cCharacName ) ;

						u_int64	 i64ItemUID = SM_Item_Make_UniqueID() ;

						if( pCharac )
						{	
							CMSG_GM_ADD_INVITEM msgGmAddInvenItem = {0} ;
							msgGmAddInvenItem.usLength = sizeof(CMSG_GM_ADD_INVITEM) ;
							msgGmAddInvenItem.cMessage = CMSG_GM_ADD_INVITEM_NUM ;
							memcpy( msgGmAddInvenItem.cCharacName, pMsgGmAddInvitem->cCharacName, 13 ) ;
							msgGmAddInvenItem.ui64UniqueId = i64ItemUID ;
							msgGmAddInvenItem.cFirstType = pMsgGmAddInvitem->cFirstType ;
							msgGmAddInvenItem.cSecondType = pMsgGmAddInvitem->cSecondType ;
							msgGmAddInvenItem.sItemID = pMsgGmAddInvitem->sItemID ;
							msgGmAddInvenItem.ucItemCount = pMsgGmAddInvitem->ucItemCount ;
							msgGmAddInvenItem.ucSlotCount = pMsgGmAddInvitem->ucSlotCount ;
							msgGmAddInvenItem.usDurability = pMsgGmAddInvitem->usDurability ;
							msgGmAddInvenItem.ucInchant = pMsgGmAddInvitem->ucInchant ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmAddInvenItem, msgGmAddInvenItem.usLength ) ;
							}

							bool bAddSuccess = false ;

							_sServer_InvenItem_v1 * Inven = (_sServer_InvenItem_v1*)pCharac->CHARAC_INVENTORY.cInventory ;
							for( int i = 25 ; i < 100 ; i++ )
							{
								if( Inven[i].sID == 0 )
								{
									ZeroMemory( &Inven[i], sizeof(_sServer_InvenItem_v1) );
									Inven[i].ui64UniqueId = i64ItemUID ;
									Inven[i].sID = pMsgGmAddInvitem->sItemID ;
									Inven[i].Item_Money.S_NormalItem.ucCount = pMsgGmAddInvitem->ucItemCount ;
									Inven[i].cType = pMsgGmAddInvitem->cFirstType ;
									Inven[i].cSecond = pMsgGmAddInvitem->cSecondType ;
									Inven[i].Item_Money.S_NormalItem.usCurDur = pMsgGmAddInvitem->usDurability ;
									Inven[i].Item_Money.S_NormalItem.ucSocketCount = pMsgGmAddInvitem->ucSlotCount ;
									Inven[i].ucStrength = pMsgGmAddInvitem->ucInchant ;

#ifdef _PD_GM_ADDITEM_MODIFY_
									// 기간 설정.
									if( 2 <= pMsgGmAddInvitem->cCashCheck && pMsgGmAddInvitem->usTimeValue )
									{
										// 기간제 아이템의 기간 설정.
										CND_RealTime::CalcAfterDay( &Inven[i].uselimit, pMsgGmAddInvitem->usTimeValue );
									}	
#endif


									bAddSuccess = true ;

									break;
								}
							}

							if( bAddSuccess )
							{
								// 성공이면 로그 남기기..
 								_ItemLog * pItemLog =  g_pLogSvrCom->GetItemLogSlot() ;
								if( pItemLog )
								{
									memset( pItemLog, 0, sizeof(_ItemLog) ) ;

									pItemLog->item_uid = i64ItemUID ;
									pItemLog->code = _LOG_ITEM_ADD_FROM_GMS_SUCC ;
									pItemLog->zone = 0 ;
									sprintf( pItemLog->from, "GMS->INVEN" ) ;
									pItemLog->to_uid = pCharac->CHARAC_BASIC.iUniqueID ;
									memcpy( pItemLog->to, pCharac->CHARAC_BASIC.cChrName, 13 ) ;
									pItemLog->type1 = pMsgGmAddInvitem->cFirstType ;
									pItemLog->type2 = pMsgGmAddInvitem->sItemID ;
									
									g_pLogSvrCom->InsertItemLog( pItemLog ) ;

								}
							}
							else
							{
 								_ItemLog * pItemLog =  g_pLogSvrCom->GetItemLogSlot() ;
								if( pItemLog )
								{
									memset( pItemLog, 0, sizeof(_ItemLog) ) ;

									pItemLog->item_uid = i64ItemUID ;
									pItemLog->code = _LOG_ITEM_ADD_FROM_GMS_FAIL ;
									pItemLog->zone = 0 ;
									sprintf( pItemLog->from, "GMS->INVEN" ) ;
									pItemLog->to_uid = pCharac->CHARAC_BASIC.iUniqueID ;
									memcpy( pItemLog->to, pCharac->CHARAC_BASIC.cChrName, 13 ) ;
									pItemLog->type1 = pMsgGmAddInvitem->cFirstType ;
									pItemLog->type2 = pMsgGmAddInvitem->sItemID ;
									
									g_pLogSvrCom->InsertItemLog( pItemLog ) ;

								}
							}


							pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}
					}
					break ;
				//#define		MSG_GM_REM_INVITEM_NUM			64		//	인벤토리에 아이템 제거
				case MSG_GM_REM_INVITEM_NUM :
					{

						MSG_GM_REM_INVITEM * pMsgGmRemInvitem = (MSG_GM_REM_INVITEM*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_REM_INVITEM_NUM (%s) \n", pMsgGmRemInvitem->cCharacName ) ;

						if( pMsgGmRemInvitem->ucSlotNo == 7 )	// 7번슬롯은 수정할 수 없는 슬롯이다.
							continue ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmRemInvitem->cCharacName ) ;

						if( pCharac )
						{	
							CMSG_GM_REM_INVITEM msgGmRemInvitem = {0} ;
							msgGmRemInvitem.usLength = sizeof(CMSG_GM_REM_INVITEM) ;
							msgGmRemInvitem.cMessage = CMSG_GM_REM_INVITEM_NUM ;
							memcpy( msgGmRemInvitem.cCharacName, pMsgGmRemInvitem->cCharacName, 13) ;
							msgGmRemInvitem.cFirstType = pMsgGmRemInvitem->cFirstType ;
							msgGmRemInvitem.cSecondType = pMsgGmRemInvitem->cSecondType ;
							msgGmRemInvitem.ucSlotNo = pMsgGmRemInvitem->ucSlotNo ;
							msgGmRemInvitem.sItemID = pMsgGmRemInvitem->sItemID ;
							
							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmRemInvitem, msgGmRemInvitem.usLength ) ;
							}
							

							_sServer_InvenItem_v1 * Inven = (_sServer_InvenItem_v1*)pCharac->CHARAC_INVENTORY.cInventory ;

							
							if( Inven[pMsgGmRemInvitem->ucSlotNo].sID == pMsgGmRemInvitem->sItemID )
							{
 								_ItemLog * pItemLog =  g_pLogSvrCom->GetItemLogSlot() ;
								if( pItemLog )
								{
									memset( pItemLog, 0, sizeof(_ItemLog) ) ;

									pItemLog->item_uid = Inven[pMsgGmRemInvitem->ucSlotNo].ui64UniqueId ;
									pItemLog->code = _LOG_ITEM_REM_FROM_GMS ;
									pItemLog->zone = 0 ;
									sprintf( pItemLog->from, "GMS->INVEN" ) ;
									pItemLog->to_uid = pCharac->CHARAC_BASIC.iUniqueID ;
									memcpy( pItemLog->to, pCharac->CHARAC_BASIC.cChrName, 13 ) ;
									pItemLog->type1 = Inven[pMsgGmRemInvitem->ucSlotNo].cType ;
									pItemLog->type2 = Inven[pMsgGmRemInvitem->ucSlotNo].sID ;
									
									g_pLogSvrCom->InsertItemLog( pItemLog ) ;

								}

								memset( &Inven[pMsgGmRemInvitem->ucSlotNo], 0, sizeof(_sServer_InvenItem_v1) ) ;

							}

							pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}

					}
					break ;
				//#define		MSG_GM_ADD_DEPITEM_NUM			65		//	저장소에 아이템 추가
				case MSG_GM_ADD_DEPITEM_NUM :
					{

						MSG_GM_ADD_DEPITEM * pMsgGmAddDepitem = (MSG_GM_ADD_DEPITEM*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_ADD_DEPITEM_NUM (%s)\n", pMsgGmAddDepitem->cAccount ) ;

						pUser = g_pAccMapManager->Find_User( pMsgGmAddDepitem->cAccount ) ;

						bool bAddSuccess = false ;

						if( pUser )
						{	

							const u_int64	ui64ItemUID  = SM_Item_Make_UniqueID() ;

							CMSG_GM_ADD_DEPITEM msgGmAddDepot = {0} ;
							msgGmAddDepot.usLength = sizeof(CMSG_GM_ADD_DEPITEM) ;
							msgGmAddDepot.cMessage = CMSG_GM_ADD_DEPITEM_NUM ;
							memcpy(msgGmAddDepot.cAccount, pUser->cNDAccount, en_max_lil+1 ) ;
							msgGmAddDepot.ui64UniqueId = ui64ItemUID ;
							msgGmAddDepot.cFirstType = pMsgGmAddDepitem->cFirstType ;
							msgGmAddDepot.cSecondType = pMsgGmAddDepitem->cSecondType ;
							msgGmAddDepot.sItemID = pMsgGmAddDepitem->sItemID ;
							msgGmAddDepot.ucItemCount = pMsgGmAddDepitem->ucItemCount ;
							msgGmAddDepot.ucSlotCount = pMsgGmAddDepitem->ucSlotCount ;
							msgGmAddDepot.ucInchant = pMsgGmAddDepitem->ucInchant ;

							if( pUser->cState == US_FIELD )
							{
								sendGS( (char)pUser->iServer, (char*)&msgGmAddDepot, msgGmAddDepot.usLength ) ;
							}

							_sServer_InvenItem_v1 * Inven = (_sServer_InvenItem_v1*)pUser->stUserDepot.cDepot ;

							for( int i = 0 ; i < INVEN_MAX_SLOT ; i++ )
							{
								if( Inven[i].sID == 0 )
								{
									ZeroMemory( &Inven[i], sizeof(_sServer_InvenItem_v1) );
									Inven[i].ui64UniqueId = ui64ItemUID ;
									Inven[i].sID = pMsgGmAddDepitem->sItemID ;
									Inven[i].Item_Money.S_NormalItem.ucCount = pMsgGmAddDepitem->ucItemCount ;
									Inven[i].cType = pMsgGmAddDepitem->cFirstType ;
									Inven[i].cSecond = pMsgGmAddDepitem->cSecondType ;
									Inven[i].Item_Money.S_NormalItem.ucSocketCount = pMsgGmAddDepitem->ucSlotCount ;
									Inven[i].ucStrength = pMsgGmAddDepitem->ucInchant ;
#ifdef _PD_GMS_ADD_DEPOT_DUR_
									Inven[i].Item_Money.S_NormalItem.usCurDur = pMsgGmAddDepitem->usDurability;
#endif

									bAddSuccess = true ;
									break;
								}
							}

							if( bAddSuccess )
							{
								// 성공이면 로그 남기기..
 								_ItemLog * pItemLog =  g_pLogSvrCom->GetItemLogSlot() ;
								if( pItemLog )
								{
									memset( pItemLog, 0, sizeof(_ItemLog) ) ;

									pItemLog->item_uid = ui64ItemUID ;
									pItemLog->code = _LOG_ITEM_ADD_FROM_GMS_SUCC ;
									pItemLog->zone = 0 ;
									sprintf( pItemLog->from, "GMS->DEPOT" ) ;										
									pItemLog->to_uid = pUser->iUid ;
									memcpy( pItemLog->to, pUser->cNDAccount, 13 ) ;
									pItemLog->to[en_charac_name_length] = 0 ;
									pItemLog->type1 = pMsgGmAddDepitem->cFirstType ;
									pItemLog->type2 = pMsgGmAddDepitem->sItemID ;
									
									g_pLogSvrCom->InsertItemLog( pItemLog ) ;

								}
							}
							else
							{
 								_ItemLog * pItemLog =  g_pLogSvrCom->GetItemLogSlot() ;
								if( pItemLog )
								{
									memset( pItemLog, 0, sizeof(_ItemLog) ) ;

									pItemLog->item_uid = ui64ItemUID ;
									pItemLog->code = _LOG_ITEM_ADD_FROM_GMS_FAIL ;
									pItemLog->zone = 0 ;
									sprintf( pItemLog->from, "GMS->DEPOT" ) ;
									pItemLog->to_uid = pUser->iUid ;
									memcpy( pItemLog->to, pUser->cNDAccount, 13 ) ;
									pItemLog->to[en_charac_name_length] = 0 ;
									pItemLog->type1 = pMsgGmAddDepitem->cFirstType ;
									pItemLog->type2 = pMsgGmAddDepitem->sItemID ;
									
									g_pLogSvrCom->InsertItemLog( pItemLog ) ;

								}
							}

							pUser->cDepotBackupState = BS_READY ;
							g_pBackupReady->InsertAcc( pUser ) ;

						}
					}
					break ;
				//#define		MSG_GM_REM_DEPITEM_NUM			66		//	저장소에 아이템 제거
				case MSG_GM_REM_DEPITEM_NUM :
					{

						MSG_GM_REM_DEPITEM * pMsgGmRemDepitem = (MSG_GM_REM_DEPITEM*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_REM_DEPITEM_NUM (%s) \n", pMsgGmRemDepitem->cAccount) ;
						
						pUser = g_pAccMapManager->Find_User( pMsgGmRemDepitem->cAccount ) ;
						if( pUser )
						{		
							CMSG_GM_REM_DEPITEM msgGmRemDepot = {0} ;
							msgGmRemDepot.usLength = sizeof( CMSG_GM_REM_DEPITEM ) ;
							msgGmRemDepot.cMessage = CMSG_GM_REM_DEPITEM_NUM ;
							memcpy( msgGmRemDepot.cAccount, pMsgGmRemDepitem->cAccount, en_max_lil+1 ) ;
							msgGmRemDepot.cFirstType = pMsgGmRemDepitem->cFirstType ;
							msgGmRemDepot.cSecondType = pMsgGmRemDepitem->cSecondType ;
							msgGmRemDepot.cSlotNo = pMsgGmRemDepitem->cSlotNo ;
							msgGmRemDepot.sItemID = pMsgGmRemDepitem->sItemID  ;
							

							if( pUser->cState == US_FIELD )
							{
								sendGS( pUser->iServer, (char*)&msgGmRemDepot, msgGmRemDepot.usLength ) ;
							}
							

							_sServer_InvenItem_v1 * Inven = (_sServer_InvenItem_v1*)pUser->stUserDepot.cDepot ;

							
							if( Inven[pMsgGmRemDepitem->cSlotNo].sID == pMsgGmRemDepitem->sItemID )
							{
 								_ItemLog * pItemLog =  g_pLogSvrCom->GetItemLogSlot() ;
								if( pItemLog )
								{
									memset( pItemLog, 0, sizeof(_ItemLog) ) ;

									pItemLog->item_uid = Inven[pMsgGmRemDepitem->cSlotNo].ui64UniqueId ;
									pItemLog->code = _LOG_ITEM_REM_FROM_GMS ;
									pItemLog->zone = 0 ;
									sprintf( pItemLog->from, "GMS->DEPOT" ) ;
									pItemLog->to_uid = pUser->iUid ;
									memcpy( pItemLog->to, pUser->cNDAccount, 13 ) ;
									pItemLog->to[en_charac_name_length] = 0 ;
									pItemLog->type1 = Inven[pMsgGmRemDepitem->cSlotNo].cType ;
									pItemLog->type2 = Inven[pMsgGmRemDepitem->cSlotNo].sID ;
									
									g_pLogSvrCom->InsertItemLog( pItemLog ) ;

								}

								memset( &Inven[pMsgGmRemDepitem->cSlotNo], 0, sizeof(_sServer_InvenItem_v1) ) ;

							}

							pUser->cDepotBackupState = BS_READY ;
							g_pBackupReady->InsertAcc( pUser ) ;

						}
					}
					break ;
				//#define		MSG_GM_ADD_INVGOLD_NUM			67		//	인벤토리의 돈 가감
				case MSG_GM_ADD_INVGOLD_NUM :
					{

						MSG_GM_ADD_INVGOLD * pMsgGmAddInvGold = (MSG_GM_ADD_INVGOLD*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_ADD_INVGOLD_NUM (%s, gold:%d)\n", pMsgGmAddInvGold->cCharacName, pMsgGmAddInvGold->iGold ) ;
						
						pCharac = g_pAccMapManager->Find_Charac( pMsgGmAddInvGold->cCharacName ) ;
						if( pCharac )
						{	
							CMSG_GM_ADD_INVGOLD msgGmAddInvgold = {0} ;
							msgGmAddInvgold.usLength = sizeof(CMSG_GM_ADD_INVGOLD) ;
							msgGmAddInvgold.cMessage = CMSG_GM_ADD_INVGOLD_NUM ;
							memcpy( msgGmAddInvgold.cCharacName, pMsgGmAddInvGold->cCharacName, 13 ) ;
							msgGmAddInvgold.iGold = pMsgGmAddInvGold->iGold ;							

							if( pMsgGmAddInvGold->iGold > 0 )
							{
								if( 0xffffffffU - pCharac->CHARAC_INVENTORY.iMoney >= static_cast<u_int>(pMsgGmAddInvGold->iGold) )
								{
									pCharac->CHARAC_INVENTORY.iMoney += static_cast<u_int>(pMsgGmAddInvGold->iGold);

									if( pCharac->CHARAC_ETC.cState == CS_FIELD )
									{
										sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmAddInvgold, msgGmAddInvgold.usLength ) ;
									}
								}
								else
									continue;
							}
							else
							{
								int minus_money = pMsgGmAddInvGold->iGold * (-1);
								if( pCharac->CHARAC_INVENTORY.iMoney >= static_cast<u_int>(minus_money) )
								{
									pCharac->CHARAC_INVENTORY.iMoney -= static_cast<u_int>(minus_money);

									if( pCharac->CHARAC_ETC.cState == CS_FIELD )
									{
										sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmAddInvgold, msgGmAddInvgold.usLength ) ;
									}
								}
								else
									continue;
							}
							
							/* // 200080214 삭제
							if( 0 > (pCharac->CHARAC_INVENTORY.iMoney + pMsgGmAddInvGold->iGold) )
								continue ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmAddInvgold, msgGmAddInvgold.usLength ) ;
							}
							
							pCharac->CHARAC_INVENTORY.iMoney = pCharac->CHARAC_INVENTORY.iMoney + pMsgGmAddInvGold->iGold ;
							*/ // 200080214 삭제

							pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}
					}
					break ;
				//#define		MSG_GM_ADD_DEPGOLD_NUM			68		//	저장소의 돈 가감
				case MSG_GM_ADD_DEPGOLD_NUM :
					{

						MSG_GM_ADD_DEPGOLD * pMsgGmAddDepGold = (MSG_GM_ADD_DEPGOLD*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_ADD_DEPGOLD_NUM (%s) \n", pMsgGmAddDepGold->cAccount) ;

						pUser = g_pAccMapManager->Find_User( pMsgGmAddDepGold->cAccount ) ;
						if( pUser )
						{	
							CMSG_GM_ADD_DEPGOLD msgGmAddDepgold = {0} ;
							msgGmAddDepgold.usLength = sizeof(CMSG_GM_ADD_DEPGOLD) ;
							msgGmAddDepgold.cMessage = CMSG_GM_ADD_DEPGOLD_NUM	;
							memcpy(msgGmAddDepgold.cAccount, pMsgGmAddDepGold->cAccount, en_max_lil+1 ) ;
							msgGmAddDepgold.iGold = pMsgGmAddDepGold->iGold ;

							if( pMsgGmAddDepGold->iGold >= 0 )
							{
								if( 0xffffffffU - pUser->stUserDepot.iMoney >= static_cast<u_int>(pMsgGmAddDepGold->iGold) )
								{
									pUser->stUserDepot.iMoney += static_cast<u_int>(pMsgGmAddDepGold->iGold);

									if( pUser->cState == US_FIELD )
									{
										sendGS( (char)pUser->iServer, (char*)&msgGmAddDepgold, msgGmAddDepgold.usLength ) ;
									}
								}
								else
									continue;
							}
							else
							{
								int minus_money = pMsgGmAddDepGold->iGold * (-1);
								if( pUser->stUserDepot.iMoney >= static_cast<u_int>(minus_money) )
								{
									pUser->stUserDepot.iMoney -= static_cast<u_int>(minus_money);

									if( pUser->cState == US_FIELD )
									{
										sendGS( (char)pUser->iServer, (char*)&msgGmAddDepgold, msgGmAddDepgold.usLength ) ;
									}
								}
								else
									continue;
							}

							/* // 200080214 삭제
							if( 0 > (pUser->stUserDepot.iMoney + pMsgGmAddDepGold->iGold) )
								continue ;

							if( pUser->cState == US_FIELD )
							{
								sendGS( (char)pUser->iServer, (char*)&msgGmAddDepgold, msgGmAddDepgold.usLength ) ;
							}
							
							pUser->stUserDepot.iMoney = pUser->stUserDepot.iMoney + pMsgGmAddDepGold->iGold ;
							*/ // 200080214 삭제

							pUser->cDepotBackupState = BS_READY ;
							g_pBackupReady->InsertAcc( pUser ) ;
						}
					}
					break ;
				//#define		MSG_GM_ADD_SKILL_NUM			69		//	스킬 추가
				case MSG_GM_ADD_SKILL_NUM :
					{

						MSG_GM_ADD_SKILL * pMsgGmAddSkill = (MSG_GM_ADD_SKILL*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_ADD_SKILL_NUM (%s)\n", pMsgGmAddSkill->cCharacName) ;
						
						pCharac = g_pAccMapManager->Find_Charac( pMsgGmAddSkill->cCharacName ) ;
						if( pCharac )
						{	
							CMSG_GM_ADD_SKILL msgGmAddSkill = {0} ;
							msgGmAddSkill.usLength=sizeof(CMSG_GM_ADD_SKILL) ;
							msgGmAddSkill.cMessage = CMSG_GM_ADD_SKILL_NUM ;
							memcpy( msgGmAddSkill.cCharacName, pMsgGmAddSkill->cCharacName, 13) ;
							msgGmAddSkill.sSkillID = pMsgGmAddSkill->sSkillID ;
							msgGmAddSkill.sSkillStep = pMsgGmAddSkill->sSkillStep ;
							msgGmAddSkill.iSkillExp = pMsgGmAddSkill->iSkillExp ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmAddSkill, msgGmAddSkill.usLength ) ;
							}
							
							_sCharac_Skill * pSkill = (_sCharac_Skill*)pCharac->CHARAC_MARTIAL_DATA.MartialData.cMartialData ;
							for( int i = 0 ; i < MARTIAL_MAX_SLOT ; i++ )
							{
								if( pSkill[i].id == 0 )
								{
									pSkill[i].id = pMsgGmAddSkill->sSkillID ;
									pSkill[i].cStep = (char)pMsgGmAddSkill->sSkillStep ;									
									pSkill[i].iCurSkillExp = pMsgGmAddSkill->iSkillExp ;
									
									break;
								}

							}

							pCharac->CHARAC_ETC._backup.bBackupStateMartial = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}
					}
					break ;
				//#define		MSG_GM_REM_SKILL_NUM			70		//	스킬 제거
				case MSG_GM_REM_SKILL_NUM :
					{

						MSG_GM_REM_SKILL * pMsgGmRemSkill = (MSG_GM_REM_SKILL*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_REM_SKILL_NUM (%s)\n", pMsgGmRemSkill->cCharacName ) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmRemSkill->cCharacName ) ;
						if( pCharac )
						{
							CMSG_GM_REM_SKILL msgGmRemSkill = {0} ;
							msgGmRemSkill.usLength = sizeof(CMSG_GM_REM_SKILL) ;
							msgGmRemSkill.cMessage =CMSG_GM_REM_SKILL_NUM	 ;
							memcpy(msgGmRemSkill.cCharacName, pMsgGmRemSkill->cCharacName, 13) ;
							msgGmRemSkill.sSkillID = pMsgGmRemSkill->sSkillID ;
						

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmRemSkill, msgGmRemSkill.usLength ) ;
							}
							
							_sCharac_Skill * pSkill = (_sCharac_Skill*)pCharac->CHARAC_MARTIAL_DATA.MartialData.cMartialData ;
							for( int i = 0 ; i < MARTIAL_MAX_SLOT ; i++ )
							{
								if( pSkill[i].id == pMsgGmRemSkill->sSkillID )
								{
									memset( &pSkill[i], 0, sizeof(_sCharac_Skill) ) ;
								}
							}

							pCharac->CHARAC_ETC._backup.bBackupStateMartial = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}
					}
					break ;
				//#define		MSG_GM_EDIT_SKILL_NUM			71		//	스킬 에디트(스킬 step변화)
				case MSG_GM_EDIT_SKILL_NUM :
					{

						MSG_GM_EDIT_SKILL * pMsgGmEditSkill = (MSG_GM_EDIT_SKILL*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_SKILL_NUM (%s) \n", pMsgGmEditSkill->cCharacName ) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditSkill->cCharacName ) ;
						if( pCharac )
						{			
							CMSG_GM_EDIT_SKILL msgGmEditSkill = {0} ;
							msgGmEditSkill.usLength = sizeof(CMSG_GM_EDIT_SKILL) ;
							msgGmEditSkill.cMessage = CMSG_GM_EDIT_SKILL_NUM ;
							memcpy( msgGmEditSkill.cCharacName, pMsgGmEditSkill->cCharacName, 13 ) ;
							msgGmEditSkill.cSkillState = pMsgGmEditSkill->cSkillState ;
							msgGmEditSkill.sSkillID = pMsgGmEditSkill->sSkillID ;
							msgGmEditSkill.sSkillStep = pMsgGmEditSkill->sSkillStep ;
							msgGmEditSkill.iSkillExp = pMsgGmEditSkill->iSkillExp ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditSkill, msgGmEditSkill.usLength ) ;
							}
							
							_sCharac_Skill * pSkill = (_sCharac_Skill*)pCharac->CHARAC_MARTIAL_DATA.MartialData.cMartialData ;
							for( int i = 0 ; i < MARTIAL_MAX_SLOT ; i++ )
							{
								if( pSkill[i].id == pMsgGmEditSkill->sSkillID )
								{
									pSkill[i].cStep = (char)pMsgGmEditSkill->sSkillStep ;
									pSkill[i].ucSkillState = pMsgGmEditSkill->cSkillState ;
									pSkill[i].iCurSkillExp = pMsgGmEditSkill->iSkillExp ;
									break;
								}								
							}

							pCharac->CHARAC_ETC._backup.bBackupStateMartial = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}
					}
					break ;

				case MSG_GM_EDIT_CONTRIBUTE_NUM:
				{
					MSG_GM_EDIT_CONTRIBUTE * pMsgGmRemContribute = (MSG_GM_EDIT_CONTRIBUTE*)buf ;

					g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_CONTRIBUTE_NUM (%s)\n", pMsgGmRemContribute->cCharacName ) ;

					pCharac = g_pAccMapManager->Find_Charac( pMsgGmRemContribute->cCharacName ) ;
					if( pCharac )
					{
						CMSG_GM_EDIT_CONTRIBUTE msgGmEditContribute = {0} ;
						msgGmEditContribute.usLength = sizeof(CMSG_GM_EDIT_CONTRIBUTE) ;
						msgGmEditContribute.cMessage = CMSG_GM_EDIT_CONTRIBUTE_NUM ;
						memcpy( msgGmEditContribute.cCharacName, pMsgGmRemContribute->cCharacName, 13 ) ;
						msgGmEditContribute.iContribute = pMsgGmRemContribute->iContribute ;

						if( pCharac->CHARAC_ETC.cState == CS_FIELD )
						{
							sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditContribute, msgGmEditContribute.usLength ) ;
						}


						pCharac->CHARAC_BASIC.iContribution = pMsgGmRemContribute->iContribute ;
						pCharac->CHARAC_ETC._backup.bBackupStateBasic = BS_READY ;
						g_pBackupReady->InsertCharac( pCharac ) ;
					}
					
				}
					break ;

				case MSG_GM_ADD_MASTERY_NUM :
				{
					MSG_GM_ADD_MASTERY * pMsgGmAddMastery = (MSG_GM_ADD_MASTERY*)buf ;

					g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_ADD_MASTERY_NUM (%s)\n", pMsgGmAddMastery->cCharacName) ;

					pCharac = g_pAccMapManager->Find_Charac( pMsgGmAddMastery->cCharacName ) ;
					if( pCharac )
					{
						int iStartPage = pMsgGmAddMastery->cPage * 24 ;
						bool bRet = false ;
						_sServer_Mastery_Info * pMastery = (_sServer_Mastery_Info*)pCharac->CHARAC_MARTIAL_DATA.MartialData.cMasteryData ;
						for ( int i = 0 ; i < 24  ; i++ )
						{
							if( pMastery[iStartPage].skillID == 0 )
							{
								pMastery[iStartPage].skillID = pMsgGmAddMastery->sMasteryID ;
								pMastery[iStartPage].cPoint = pMsgGmAddMastery->cPoint ;								
								bRet = true ;

								break ;									
							}
							++iStartPage ;
						}
						if( bRet && pCharac->CHARAC_ETC.cState == CS_FIELD )
						{						
							CMSG_GM_ADD_MASTERY msgGmAddMastery = {0} ;
							msgGmAddMastery.usLength = sizeof(CMSG_GM_ADD_MASTERY) ;
							msgGmAddMastery.cMessage = CMSG_GM_ADD_MASTERY_NUM ;
							memcpy ( msgGmAddMastery.cCharacName, pMsgGmAddMastery->cCharacName, 13 ) ;
							msgGmAddMastery.cPage = pMsgGmAddMastery->cPage ;
							msgGmAddMastery.cPoint = pMsgGmAddMastery->cPoint ;
							msgGmAddMastery.sMasteryID = pMsgGmAddMastery->sMasteryID ;

							sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmAddMastery, msgGmAddMastery.usLength ) ;							
						}

						pCharac->CHARAC_ETC._backup.bBackupStateMartial = BS_READY ;
						g_pBackupReady->InsertCharac( pCharac ) ;
						
					}
				}
					break ;

				case MSG_GM_REM_MASTERY_NUM :
					{
						MSG_GM_REM_MASTERY * pMsgGmRemMastery = (MSG_GM_REM_MASTERY*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_REM_MASTERY_NUM (%s) \n", pMsgGmRemMastery->cCharacName  ) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmRemMastery->cCharacName ) ;
						if( pCharac )
						{							
							bool bRet = false ;
							_sServer_Mastery_Info * pMastery = (_sServer_Mastery_Info*)pCharac->CHARAC_MARTIAL_DATA.MartialData.cMasteryData ;
							for ( int i = 0 ; i < 24*4  ; i++ )
							{
								if( pMastery[i].skillID == pMsgGmRemMastery->sMasteryID )
								{
									memset( &pMastery[i], 0, sizeof(_sServer_Mastery_Info) ) ;									
									bRet = true ;

									break ;									
								}						
							}

							if( bRet && pCharac->CHARAC_ETC.cState == CS_FIELD )
							{						
								CMSG_GM_REM_MASTERY msgGmRemMastery = {0} ;
								msgGmRemMastery.usLength = sizeof(CMSG_GM_REM_MASTERY) ;
								msgGmRemMastery.cMessage = CMSG_GM_REM_MASTERY_NUM ;
								memcpy ( msgGmRemMastery.cCharacName, pMsgGmRemMastery->cCharacName, 13 ) ;								
								msgGmRemMastery.sMasteryID = pMsgGmRemMastery->sMasteryID ;

								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmRemMastery, msgGmRemMastery.usLength ) ;

							}
							pCharac->CHARAC_ETC._backup.bBackupStateMartial = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}

					}
					break ;

				case MSG_GM_EDIT_MASTERY_NUM :
					{
						MSG_GM_EDIT_MASTERY * pMsgEditMastery = (MSG_GM_EDIT_MASTERY*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_MASTERY_NUM (%s)\n", pMsgEditMastery->cCharacName) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgEditMastery->cCharacName ) ;
						if( pCharac )
						{
							bool bRet = false ;
							_sServer_Mastery_Info * pMastery = (_sServer_Mastery_Info*)pCharac->CHARAC_MARTIAL_DATA.MartialData.cMasteryData ;
							for ( int i = 0 ; i < 24*4  ; i++ )
							{
								if( pMastery[i].skillID == pMsgEditMastery->sMasteryID )
								{
									pMastery[i].cPoint = pMsgEditMastery->cPoint ;
									bRet = true ;

									break ;									
								}						
							}

							if( bRet && pCharac->CHARAC_ETC.cState == CS_FIELD )
							{						
								CMSG_GM_EDIT_MASTERY msgGmEditMastery = {0} ;
								msgGmEditMastery.usLength = sizeof(CMSG_GM_EDIT_MASTERY) ;
								msgGmEditMastery.cMessage = CMSG_GM_EDIT_MASTERY_NUM ;
								memcpy ( msgGmEditMastery.cCharacName, pMsgEditMastery->cCharacName, 13 ) ;								
								msgGmEditMastery.cPoint = pMsgEditMastery->cPoint ;
								msgGmEditMastery.sMasteryID = pMsgEditMastery->sMasteryID ;

								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditMastery, msgGmEditMastery.usLength ) ;

							}
							pCharac->CHARAC_ETC._backup.bBackupStateMartial = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}
					}
					break ;

				case MSG_GM_EDIT_LEFTMASTERYPOINT_NUM :
					{
						MSG_GM_EDIT_LEFTMASTERYPOINT * pMsgGmEditLeftPoint = (MSG_GM_EDIT_LEFTMASTERYPOINT*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_LEFTMASTERYPOINT_NUM (%s) \n", pMsgGmEditLeftPoint->cCharacName) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditLeftPoint->cCharacName ) ;
						if( pCharac )
						{							
							pCharac->CHARAC_LEVEL.sMasteryPoint = pMsgGmEditLeftPoint->sPoint ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{						
								CMSG_GM_EDIT_LEFTMASTERYPOINT	msgGmEditLeftPoint = {0 } ;
								msgGmEditLeftPoint.usLength = sizeof(CMSG_GM_EDIT_LEFTMASTERYPOINT) ;
								msgGmEditLeftPoint.cMessage = CMSG_GM_EDIT_LEFTMASTERYPOINT_NUM ;
								memcpy ( msgGmEditLeftPoint.cCharacName, pMsgGmEditLeftPoint->cCharacName, 13 ) ;
								msgGmEditLeftPoint.sPoint = pMsgGmEditLeftPoint->sPoint ;


								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditLeftPoint, msgGmEditLeftPoint.usLength ) ;

							}
							pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break ;

				case MSG_GM_EDIT_CHARAC_ROLE_NUM :
					{
						MSG_GM_EDIT_CHARAC_ROLE * pMsgGmEditCharacRole = (MSG_GM_EDIT_CHARAC_ROLE*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_CHARAC_ROLE_NUM (%s) \n", pMsgGmEditCharacRole->cCharacName) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditCharacRole->cCharacName ) ;
						if( pCharac && pCharac->CHARAC_ETC.cState == CS_FIELD )
						{
							pCharac->CHARAC_BASIC.cClass = pMsgGmEditCharacRole->cClass ;

							CMSG_GM_EDIT_CHARAC_ROLE	msgGmEditCharacRole = {0} ;
							msgGmEditCharacRole.usLength = sizeof(CMSG_GM_EDIT_CHARAC_ROLE) ;
							msgGmEditCharacRole.cMessage = CMSG_GM_EDIT_CHARAC_ROLE_NUM ;
							memcpy( msgGmEditCharacRole.cCharacName, pMsgGmEditCharacRole->cCharacName, 13 ) ;
							msgGmEditCharacRole.cClass = pMsgGmEditCharacRole->cClass ;

							sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditCharacRole, msgGmEditCharacRole.usLength ) ;

							pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}
						else
						{
							// pCharac 을 찾아, 역할설정을 바꾸고, 스킬을 삭제한다..
						}
					}
					break ;

				case MSG_GM_EDIT_CHARAC_CLASSGRADE_NUM :
					{
						MSG_GM_EDIT_CHARAC_CLASSGRADE * pMsgGmEditCharacClassGrade = (MSG_GM_EDIT_CHARAC_CLASSGRADE*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_CHARAC_CLASSGRADE_NUM (%s) \n", pMsgGmEditCharacClassGrade->cCharacName) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditCharacClassGrade->cCharacName ) ;
						if( pCharac )
						{
							pCharac->CHARAC_BASIC.cClassGrade = pMsgGmEditCharacClassGrade->cClassGrade ;

							CMSG_GM_EDIT_CHARAC_CLASSGRADE	msgGmEditCharacClassGrade = {0} ;
							msgGmEditCharacClassGrade.usLength = sizeof(CMSG_GM_EDIT_CHARAC_CLASSGRADE) ;
							msgGmEditCharacClassGrade.cMessage = CMSG_GM_EDIT_CHARAC_CLASSGRADE_NUM ;
							memcpy( msgGmEditCharacClassGrade.cCharacName, pMsgGmEditCharacClassGrade->cCharacName, 13 ) ;
							msgGmEditCharacClassGrade.cClassGrade = pMsgGmEditCharacClassGrade->cClassGrade ;

							sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditCharacClassGrade, msgGmEditCharacClassGrade.usLength ) ;

							pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
						else
						{
							char cAcc[en_max_lil+1] = {0, } ;
							PUSER pUser = NULL ;

						}
					}
					break ;

				case MSG_GM_EDIT_MUTE_TIME_NUM :
					{
						MSG_GM_EDIT_MUTE_TIME * pMsgGmEditMuteTime = (MSG_GM_EDIT_MUTE_TIME*)buf ;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_MUTE_TIME_NUM (%s) \n", pMsgGmEditMuteTime->cCharacName) ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditMuteTime->cCharacName ) ;
						if( pCharac )
						{
							pCharac->CHARAC_STATE.iMuteTime = pMsgGmEditMuteTime->iMuteTime ;

							pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

							CMSG_GM_EDIT_CHARAC_MUTE msgGmEditCharacMute = {0} ;
							msgGmEditCharacMute.cMessage = CMSG_GM_EDIT_CHARAC_MUTE_TIME_NUM ;
							msgGmEditCharacMute.usLength = sizeof(CMSG_GM_EDIT_CHARAC_MUTE) ;
							memcpy( msgGmEditCharacMute.cCharacName, pCharac->CHARAC_BASIC.cChrName, 13 ) ;
							msgGmEditCharacMute.iTime = pMsgGmEditMuteTime->iMuteTime ;

							sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditCharacMute, msgGmEditCharacMute.usLength ) ;

							pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break ;

				case MSG_GM_EDIT_LIMIT_CONNECTION_NUM :
					{
						MSG_GM_EDIT_LIMIT_CONNECTION * pMsgGmEditLimitConnection = (MSG_GM_EDIT_LIMIT_CONNECTION*)buf ;


						// -1 이면 서버 점검중으로 변환
						if( pMsgGmEditLimitConnection->bServerState == false )
						{
							g_bCloseServer = true ;
							g_MaxConnection = pMsgGmEditLimitConnection->iLimitCount ;
							::PrintConsole( "[GMS] Set server outer close & limit connection count : %d \n", pMsgGmEditLimitConnection->iLimitCount  ) ;
						}
						else
						{
							g_bCloseServer = false ;
							g_MaxConnection = pMsgGmEditLimitConnection->iLimitCount ;
							::PrintConsole( "[GMS] Set server outer open & limit connection count : %d \n", pMsgGmEditLimitConnection->iLimitCount ) ;
						}
						
					}
					break ;

				case MSG_GM_EDIT_JIN_NUM :
					{

						MSG_GM_EDIT_JIN * pMsgGmEditJin = (MSG_GM_EDIT_JIN*)buf ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditJin->cCharacName ) ;
						if( pCharac )
						{
							pCharac->CHARAC_STATE.uiJin = pMsgGmEditJin->uiJin ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								CMSG_GM_EDIT_JIN msgGmEditJin = {0} ;
								msgGmEditJin.usLength = sizeof(CMSG_GM_EDIT_JIN) ;
								msgGmEditJin.cMessage = CMSG_GM_EDIT_JIN_NUM ;
								memcpy( msgGmEditJin.cCharacName, pMsgGmEditJin->cCharacName, 13 ) ;
								msgGmEditJin.uiJin = pMsgGmEditJin->uiJin ;

								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditJin, msgGmEditJin.usLength ) ;
							}

							pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}

					}
					break ;

				case MSG_GM_EDIT_MAINQUESTID_NUM :
					{
						MSG_GM_EDIT_MAINQUESTID * pMsgGmEditMainquestID = (MSG_GM_EDIT_MAINQUESTID*)buf ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditMainquestID->cCharacName ) ;
						if( pCharac )
						{

							_sCharacMainQuestState * pMainQuest = (_sCharacMainQuestState *)pCharac->CHARAC_QUEST_DATA.cMainQuestData ;
							pMainQuest->usQuestID = pMsgGmEditMainquestID->usQuestID ;				
							

							_sRunningQuest * pRunningQuest = (_sRunningQuest*)pCharac->CHARAC_QUEST_DATA.cCurQuestData ;
							pRunningQuest->usQuestID = pMsgGmEditMainquestID->usQuestID ;
							pRunningQuest->dwQuestStartTime = pMsgGmEditMainquestID->iTime ;

							pCharac->CHARAC_ETC._backup.bBackupStateQuest = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

						}
					}
					break ;

				case MSG_GM_EDIT_INTOXICATION_NUM :
					{
						MSG_GM_EDIT_INTOXICATION * pMsgGmEditToxi = (MSG_GM_EDIT_INTOXICATION*)buf ;
						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditToxi->cCharacName ) ;
						if( pCharac )
						{
							Elixir_Data	* pElixirData = (Elixir_Data*)&pCharac->CHARAC_INVENTORY.cElixirData ;
							pElixirData->_sIntoxication = pMsgGmEditToxi->sIntoxication ;

							pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}						

					}
					break ;


				case MSG_GM_EDIT_ELIXIR_NUM :
					{
						MSG_GM_EDIT_ELIXIR * pMsgGmElixir = (MSG_GM_EDIT_ELIXIR*)buf ;
						pCharac = g_pAccMapManager->Find_Charac( pMsgGmElixir->cCharacName ) ;
						if( pCharac )
						{
							char type = pMsgGmElixir->cElixirType ;
							Elixir_Data	* pElixirData = (Elixir_Data*)&pCharac->CHARAC_INVENTORY.cElixirData ;
							pElixirData->Data[type].__Grade = pMsgGmElixir->cGrade ;
							memcpy( pElixirData->Data[type].__Data, pMsgGmElixir->asData, sizeof(pElixirData->Data[type].__Data) ) ;							

							pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}						
					}
					break ;

				case MSG_GM_EDIT_NICK_ID_NUM :
					{
						MSG_GM_EDIT_NICK_ID * pMsgEditNickID = (MSG_GM_EDIT_NICK_ID*)buf ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgEditNickID->cCharacName ) ;
						if( pCharac )
						{
							if( pCharac->CHARAC_NIC.sSelectNic == pMsgEditNickID->sSlotNum )
							{
								pCharac->CHARAC_NIC.sUsedTime = 0 ;
							}
							pCharac->CHARAC_NIC.sSelectNic = pMsgEditNickID->sSlotNum ;
							pCharac->CHARAC_NIC.usNicID[pMsgEditNickID->sSlotNum] = pMsgEditNickID->usNickID ;

							pCharac->CHARAC_ETC._backup.bBackupStateNic = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								CMSG_GM_EDIT_NICK_ID msgGmEditNicID = {0} ;
								msgGmEditNicID.usLength = sizeof(CMSG_GM_EDIT_NICK_ID) ;
								msgGmEditNicID.ucMessage = CMSG_GM_EDIT_NICK_ID_NUM ;
								memcpy( msgGmEditNicID.cCharacName, pMsgEditNickID->cCharacName, 13 ) ;
								msgGmEditNicID.sSlotNum = pMsgEditNickID->sSlotNum ;
								msgGmEditNicID.usNickID = pMsgEditNickID->usNickID ;

								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditNicID, msgGmEditNicID.usLength ) ;
							}
						}
					}
					break;

				case MSG_GM_COPY_CHARAC_NUM:
					{
						MSG_GM_COPY_CHARAC * pMsgGmCopyCharac = (MSG_GM_COPY_CHARAC*)buf ;
						pMsgGmCopyCharac->cSourceCharacName[en_charac_name_length] = 0 ;
						pMsgGmCopyCharac->cTargetCharacName[en_charac_name_length] = 0 ;
						_C_CHARAC * pSourceCharac = g_pAccMapManager->Find_Charac( pMsgGmCopyCharac->cSourceCharacName ) ;
						_C_CHARAC * pTargetCharac = g_pAccMapManager->Find_Charac( pMsgGmCopyCharac->cTargetCharacName ) ;
						
						if( pSourceCharac == NULL )
						{
							char cAcc[en_max_lil+1] = {0} ;
							PUSER pUser = NULL ;
							g_UDP_DB_Command->GetAccName(pMsgGmCopyCharac->cSourceCharacName, cAcc) ;

							if( cAcc[0] == 0 )
							{
								continue ;
							}


							pUser = g_pAccMapManager->Find_User( cAcc ) ;



							if( pUser==NULL )
							{							


								pUser = g_pAccMapManager->GetWritePtr();	// Get pointer(map) vitual memory							


								if( pUser == NULL )
								{

									continue ;
								}

								//memset(pUser,0,sizeof(_USER));
								pUser->iTime = CUser::get_time_by_sec() ;
								memcpy( pUser->cNDAccount, cAcc, en_max_lil+1) ;

								bool bRes = g_UDP_USER_DB_Command->DSGetAccountUID( pUser->cNDAccount, pUser->iUid ) ;
								if( bRes == false )
								{

									::PrintConsole("[ERROR] %s, %d \n", __FILE__, __LINE__) ;

									// 맵을 다시 반환한다..
									g_pAccMapManager->PutMemPtr( pUser ) ;

									continue ;
								}

#ifdef _TRACE_
								g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] GetDepot (uniqueID: %d) \n", pUser->iUid);
#endif
								if(g_UDP_DB_Command->pr_GetDepot( pUser->iUid, pUser ) == false){
									::PrintConsole("[DB_ERR] (copy) GetDepot failed (account: %s) \n", cAcc );

									// 맵을 다시 반환한다..
									g_pAccMapManager->PutMemPtr( pUser ) ;


									continue;
								}

								if( false == g_UDP_DB_Command->GetAccountItemUseStatus( pUser->iUid, pUser ) )
								{
									pos = 11;

									PrintConsole( "  => DB Loading(copy:GetAccountItemUseStatus) failed (account: %s : %d) \n", pUser->cNDAccount,pUser->iUid );

									g_pAccMapManager->PutMemPtr( pUser ) ;		// 실패했을 경우 맵을 다시 반환한다..

									continue ;
								}

								// Get Character info
								if(	g_UDP_DB_Command->Get_Charac_List( pUser->iUid, pUser )  == false){		// Read charac list from DB

									::PrintConsole("[DB_ERR] : CID - %s Get failed what account info \n", pUser->cNDAccount);
									::PrintConsole("[ERROR] %s, %d \n", __FILE__, __LINE__) ;


									// 맵을 다시 반환한다..
									g_pAccMapManager->PutMemPtr( pUser ) ;
									continue;
								}								


								if(pUser) g_pAccMapManager->Insert(pUser);
								pSourceCharac = g_pAccMapManager->Find_Charac( pMsgGmCopyCharac->cSourceCharacName );

								// 메모리 삭제 예약..
								g_pMemSaveTimer->Reserve_save( pUser, pUser->iUid, CUserMemSaveTimer::get_time_by_sec() + _MEM_SAVE_DELAY_TIME_KICK ) ;
							}
						}

						if( pTargetCharac == NULL )
						{
							char cAcc[en_max_lil+1] = {0} ;
							PUSER pUser = NULL ;
							g_UDP_DB_Command->GetAccName(pMsgGmCopyCharac->cTargetCharacName, cAcc) ;

							if( cAcc[0] == 0 )
							{
								continue ;
							}

							pUser = g_pAccMapManager->Find_User( cAcc ) ;

							if( pUser==NULL )
							{							

								pUser = g_pAccMapManager->GetWritePtr();	// Get pointer(map) vitual memory							


								if( pUser == NULL )
								{

									continue ;
								}

								//memset(pUser,0,sizeof(_USER));
								pUser->iTime = CUser::get_time_by_sec() ;
								memcpy( pUser->cNDAccount, cAcc, en_max_lil+1) ;

								bool bRes = g_UDP_USER_DB_Command->DSGetAccountUID( pUser->cNDAccount, pUser->iUid ) ;
								if( bRes == false )
								{

									::PrintConsole("[ERROR] %s, %d \n", __FILE__, __LINE__) ;

									// 맵을 다시 반환한다..
									g_pAccMapManager->PutMemPtr( pUser ) ;

									continue ;
								}

#ifdef _TRACE_
								g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] GetDepot (uniqueID: %d) \n", pUser->iUid);
#endif
								if(g_UDP_DB_Command->pr_GetDepot( pUser->iUid, pUser ) == false){
									::PrintConsole("[DB_ERR] GetDepot failed (account: %s) \n", cAcc );
									::PrintConsole("[ERROR] %s, %d \n", __FILE__, __LINE__) ;

									// 맵을 다시 반환한다..
									g_pAccMapManager->PutMemPtr( pUser ) ;


									continue;
								}

								if( false == g_UDP_DB_Command->GetAccountItemUseStatus( pUser->iUid, pUser ) )
								{
									pos = 11;

									g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => DB Loading(GetAccountItemUseStatus) failed (account: %s : %d) \n", pUser->cNDAccount,pUser->iUid );

									g_pAccMapManager->PutMemPtr( pUser ) ;		// 실패했을 경우 맵을 다시 반환한다..

									continue ;
								}

								// Get Character info
								if(	g_UDP_DB_Command->Get_Charac_List( pUser->iUid, pUser )  == false){		// Read charac list from DB


									::PrintConsole("[DB_ERR] : CID - %s Get failed what account info \n", pUser->cNDAccount);
									::PrintConsole("[ERROR] %s, %d \n", __FILE__, __LINE__) ;

									// 맵을 다시 반환한다..
									g_pAccMapManager->PutMemPtr( pUser ) ;
									continue;
								}								


								if(pUser) g_pAccMapManager->Insert(pUser);
								pTargetCharac = g_pAccMapManager->Find_Charac( pMsgGmCopyCharac->cTargetCharacName );

								// 메모리 삭제 예약..
								g_pMemSaveTimer->Reserve_save( pUser, pUser->iUid, CUserMemSaveTimer::get_time_by_sec() + _MEM_SAVE_DELAY_TIME_KICK ) ;
							}							
						}

						if( pSourceCharac && pTargetCharac )
						{
							int		iUniqueID ;				// 유니크아이디	4
							char	cAccount[en_max_lil+1];				// 유저 아이디  17   <----요거 바뀌었어요
							char	cChrName[13];			// 캐릭터 이름  30
							char	cChrNic[13];			// 캐릭터 별호  43

							iUniqueID = pTargetCharac->CHARAC_BASIC.iUniqueID ;
							memcpy( cAccount, pTargetCharac->CHARAC_BASIC.cAccount, en_max_lil+1 ) ;
							memcpy( cChrName, pTargetCharac->CHARAC_BASIC.cChrName, 13 ) ;
							memcpy( cChrNic, pTargetCharac->CHARAC_BASIC.cChrNic, 13 ) ;

							memcpy( pTargetCharac, pSourceCharac, sizeof( _CHARAC ) ) ;

							pTargetCharac->CHARAC_BASIC.iUniqueID = iUniqueID ;
							memcpy( pTargetCharac->CHARAC_BASIC.cAccount, cAccount, en_max_lil+1 ) ;
							memcpy( pTargetCharac->CHARAC_BASIC.cChrName, cChrName, 13 ) ;
							memcpy( pTargetCharac->CHARAC_BASIC.cChrNic, cChrNic, 13 ) ;

							pTargetCharac->CHARAC_ETC._backup.bBackupStateQuest = BS_READY ;
							g_pBackupReady->InsertCharac( pTargetCharac ) ;

						}
						else
						{
							::PrintConsole( "[GMS-ERROR] Copy character failed (%s->%s)\n"
								, pMsgGmCopyCharac->cSourceCharacName, pMsgGmCopyCharac->cTargetCharacName ) ;
						}
					}
					break ;

				case MSG_GM_EDIT_COLLECTION_NUM :
					{
						MSG_GM_EDIT_COLLECTION * pMsgGmEditCollection = (MSG_GM_EDIT_COLLECTION*)buf ;
						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditCollection->sCharacName ) ;
						if( pCharac )
						{
#ifdef _PD_ADD_EVENT_COLLECTION_
							ckCollectionData* pCollection = NULL;
							if( pMsgGmEditCollection->usCollectionIndex < BASE_EVENT_COLLECTION_INDEX )
							{
								pCollection = (ckCollectionData*)pCharac->CHARAC_INVENTORY.cCollectionData ;
								if( pMsgGmEditCollection->ucEditFlag == 0 )
								{
									pCollection[pMsgGmEditCollection->usCollectionIndex].SetCompleted() ;
								}
								else if( pMsgGmEditCollection->ucEditFlag == 1 )
								{
									pCollection[pMsgGmEditCollection->usCollectionIndex].m_usData = 0 ;
								}	
							}
							else
							{
								pCollection = (ckCollectionData*)pCharac->CHARAC_INVENTORY.cEventCollectionData ;
								if( pMsgGmEditCollection->ucEditFlag == 0 )
								{
									pCollection[pMsgGmEditCollection->usCollectionIndex-BASE_EVENT_COLLECTION_INDEX].SetCompleted() ;
								}
								else if( pMsgGmEditCollection->ucEditFlag == 1 )
								{
									pCollection[pMsgGmEditCollection->usCollectionIndex-BASE_EVENT_COLLECTION_INDEX].m_usData = 0 ;
								}	
							}
							
							
#else
							ckCollectionData* pCollection = (ckCollectionData*)pCharac->CHARAC_INVENTORY.cCollectionData ;
							if( pMsgGmEditCollection->ucEditFlag == 0 )
							{
								pCollection[pMsgGmEditCollection->usCollectionIndex].SetCompleted() ;
							}
							else if( pMsgGmEditCollection->ucEditFlag == 1 )
							{
								pCollection[pMsgGmEditCollection->usCollectionIndex].m_usData = 0 ;
							}							

#endif

							CMSG_GM_EDIT_COLLECTION msgGmEditColloection = {0} ;
							msgGmEditColloection.usLength = sizeof( msgGmEditColloection) ;
							msgGmEditColloection.cMessage = CMSG_GM_EDIT_COLLECTION_NUM ;
							memcpy( msgGmEditColloection.sCharacName, pMsgGmEditCollection->sCharacName, 13 ) ;
							msgGmEditColloection.ucEditFlag = pMsgGmEditCollection->ucEditFlag ;
							msgGmEditColloection.usCollectionIndex = pMsgGmEditCollection->usCollectionIndex ;

							sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditColloection, msgGmEditColloection.usLength ) ;

						}


					}
					break ;

				case MSG_GM_EDIT_SERVER_EXPRATE_NUM :
					{
						MSG_GM_EDIT_EXPRATE * pMsgGmEditExprate = (MSG_GM_EDIT_EXPRATE*)buf ;
						CMSG_GM_EDIT_EXPRATE msgGmEditExprate = {0} ;
						msgGmEditExprate.usLength = sizeof(CMSG_GM_EDIT_EXPRATE) ;
						msgGmEditExprate.ucMessage = CMSG_GM_EDIT_EXPRATE_NUM ;
						msgGmEditExprate.iLevelExp = pMsgGmEditExprate->iLevelExp ;
						msgGmEditExprate.iSkillExp = pMsgGmEditExprate->iSkillExp ;

						sendGS( pMsgGmEditExprate->cServerNum, (char*)&msgGmEditExprate, msgGmEditExprate.usLength ) ;						
					}
					break ;

				case MSG_GM_EDIT_INCHANTVALUE_NUM :
					try
					{					
						MSG_GM_EDIT_INCHANTVALUE * pMsgGmEditInchantValue = (MSG_GM_EDIT_INCHANTVALUE*)buf ;
						CMSG_GM_EDIT_INCHANTVALUE msgGmEditInchantValue = {0} ;
						msgGmEditInchantValue.usLength = sizeof(CMSG_GM_EDIT_INCHANTVALUE) ;
						msgGmEditInchantValue.ucMessage = CMSG_GM_EDIT_INCHANTVALU_NUM ;
						msgGmEditInchantValue.ucWhere = pMsgGmEditInchantValue->ucWhere ;						
						memcpy( msgGmEditInchantValue.sCharacName, pMsgGmEditInchantValue->sCharacName, 13 ) ;
						msgGmEditInchantValue.sSlotNumber = pMsgGmEditInchantValue->sSlotNumber ;
						msgGmEditInchantValue.ui64UniqueId = pMsgGmEditInchantValue->ui64UniqueId ;
						msgGmEditInchantValue.ucInchantValue = pMsgGmEditInchantValue->ucInchantValue ;

						if( pMsgGmEditInchantValue->ucWhere == 0 )
						{
							pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditInchantValue->sCharacName ) ;
							if( pCharac )
							{
								if( pCharac->CHARAC_ETC.cState == CS_FIELD )
								{
									sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditInchantValue, msgGmEditInchantValue.usLength ) ;
								}							

								_sServer_InvenItem_v1 * Inven = (_sServer_InvenItem_v1*)pCharac->CHARAC_INVENTORY.cInventory ;
								if( Inven[msgGmEditInchantValue.sSlotNumber].ui64UniqueId == msgGmEditInchantValue.ui64UniqueId )
								{
									Inven[msgGmEditInchantValue.sSlotNumber].ucStrength = msgGmEditInchantValue.ucInchantValue ;
								}
								pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_READY ;
								g_pBackupReady->InsertCharac( pCharac ) ;
							}							
						}
						else
						{
							pUser = g_pAccMapManager->Find_User( pMsgGmEditInchantValue->cAccount ) ;
							if( pUser )
							{
								if( pUser->cState == US_FIELD )
								{
									sendGS( pUser->iServer, (char*)&msgGmEditInchantValue, msgGmEditInchantValue.usLength ) ;
								}

								_sServer_InvenItem_v1 * Inven = (_sServer_InvenItem_v1*)pUser->stUserDepot.cDepot ;
								if( Inven[msgGmEditInchantValue.sSlotNumber].ui64UniqueId == msgGmEditInchantValue.ui64UniqueId )
								{
									Inven[msgGmEditInchantValue.sSlotNumber].ucStrength = msgGmEditInchantValue.ucInchantValue ;
								}

								pUser->cDepotBackupState = BS_READY ;
								g_pBackupReady->InsertAcc( pUser ) ;
							}
						}					
						

					}
					catch(...)
					{
						__EXCEPT_STR_PRINT() ;
					}
					break ;


				case MSG_GM_EDIT_VAR_NO:
					try
					{
						MSG_GM_EDIT_VAR * pGMEditV = (MSG_GM_EDIT_VAR*)buf ;
						
						switch( pGMEditV->ucMode )
						{
						case en_gms_ds_edit_mode_honor:
							{
								MSG_GM_EDIT_HONOR * pGMEditHonor = static_cast<MSG_GM_EDIT_HONOR*>(pGMEditV) ;

								pCharac = g_pAccMapManager->Find_Charac( pGMEditHonor->cCharacName ) ;

								if( pCharac )
								{
								
									pCharac->CHARAC_STATE.iHonor = pGMEditHonor->iValue ;

									if( pCharac->CHARAC_ETC.cState == CS_FIELD )
									{
										CMSG_GM_EDIT_HONOR_VAR msgGmEditHonor ;
										::ZeroMemory( &msgGmEditHonor, sizeof(msgGmEditHonor) ) ;
										msgGmEditHonor.usLength = sizeof(CMSG_GM_EDIT_HONOR_VAR) ;
										msgGmEditHonor.ucMessage = CMSG_GM_EDIT_VAR_NO ;
										msgGmEditHonor.ucMode = en_ds_gs_edit_mode_honor ;

										memcpy( msgGmEditHonor.cCharacName, pGMEditHonor->cCharacName, 13 ) ;
										msgGmEditHonor.iValue = pGMEditHonor->iValue ;

										sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditHonor, msgGmEditHonor.usLength ) ;
									}

									pCharac->CHARAC_ETC._backup.bBackupStateCurBasicInfo = BS_READY ;
									g_pBackupReady->InsertCharac( pCharac ) ;
								}
							}
							
							break ;
						case en_gms_ds_edit_mode_fame:
							{
								MSG_GM_EDIT_HONOR * pGMEditHonor = static_cast<MSG_GM_EDIT_HONOR*>(pGMEditV) ;

								pCharac = g_pAccMapManager->Find_Charac( pGMEditHonor->cCharacName ) ;

								if( pCharac )
								{

									pCharac->CHARAC_STATE.iGong = pGMEditHonor->iValue ;

									if( pCharac->CHARAC_ETC.cState == CS_FIELD )
									{
										CMSG_GM_EDIT_HONOR_VAR msgGmEditHonor ;
										::ZeroMemory( &msgGmEditHonor, sizeof(msgGmEditHonor) ) ;
										msgGmEditHonor.usLength = sizeof(CMSG_GM_EDIT_HONOR_VAR) ;
										msgGmEditHonor.ucMessage = CMSG_GM_EDIT_VAR_NO ;
										msgGmEditHonor.ucMode = en_ds_gs_edit_mode_fame ;

										memcpy( msgGmEditHonor.cCharacName, pGMEditHonor->cCharacName, 13 ) ;
										msgGmEditHonor.iValue = pGMEditHonor->iValue ;

										sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditHonor, msgGmEditHonor.usLength ) ;
									}

									pCharac->CHARAC_ETC._backup.bBackupStateCurBasicInfo = BS_READY ;
									g_pBackupReady->InsertCharac( pCharac ) ;
								}
							}
							break ;
						}
					}
					catch( ... )
					{
					}
					break ;

				case MSG_GM_EDIT_BLOOD_POINT_NUM:
					{
						MSG_GM_EDIT_BLOOD_POINT * pMsgGmEditBlood = (MSG_GM_EDIT_BLOOD_POINT*)buf ;

						pCharac = g_pAccMapManager->Find_Charac( pMsgGmEditBlood->szCharacName ) ;
						if( pCharac )
						{
							pCharac->CHARAC_INVENTORY.sBloodPoint = pMsgGmEditBlood->sBloodPoint ;

							if( pCharac->CHARAC_ETC.cState == CS_FIELD )
							{
								CMSG_GM_EDIT_BLOOD msgGmEditBlood;
								memcpy( msgGmEditBlood.cCharacName, pMsgGmEditBlood->szCharacName, 13 );
								msgGmEditBlood.sBloodPoint = pMsgGmEditBlood->sBloodPoint;

								sendGS( (char)pCharac->CHARAC_CUR_BASIC.sZone, (char*)&msgGmEditBlood, msgGmEditBlood.usLength ) ;
							}

							pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;
						}
					}
					break ;

				case MSG_GM_EDIT_ITEM_PERIOD_NUM:
					{
						MSG_GM_EDIT_ITEM * pEditItem = (MSG_GM_EDIT_ITEM *)buf;
						if( en_gm_edit_item_aius == pEditItem->ucMode )
						{
							MSG_GM_EDIT_ITEM_AIUS * pAius = static_cast<MSG_GM_EDIT_ITEM_AIUS*>(pEditItem);

							// /* // 테스트 기간에만 보여준다.
							PrintConsole( "[RECV] MSG_GM_EDIT_ITEM_AIUS ---\n" );
							PrintConsole( " Acc=%s, Item = %d-%d EndTime = %u\n", pAius->szAccount, pAius->aius.grade.cItemType, pAius->aius.grade.sItemID, pAius->aius.uiEndTime );
							PrintConsole( " %u-%u-%u %u:%u\n", pAius->aius.grade.uiYear+2000, pAius->aius.grade.uiMonth, pAius->aius.grade.uiDay, pAius->aius.grade.uiHour, pAius->aius.grade.uiMinute );
							// */

							if( pAius->szAccount[0] == 0 ) {
								::PrintConsole("[ME_ERR] if( pMsgUserDepotReq->cAccount[0] == 0 ) \n") ;
								continue ;
							}

							g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_EDIT_ITEM_AIUS (acc:%s) \n", pAius->szAccount ) ;

							pUser = g_pAccMapManager->Find_User(pAius->szAccount) ;

							if( NULL == pUser )
								pUser = AddUserFromAccount( g_UDP_DB_Command, g_UDP_USER_DB_Command, pAius->szAccount );

							if( pUser )
							{
								CopyMemory( &pUser->AIUS, &pAius->aius, sizeof(_ACCOUNT_ITEM_USE_STATUS) );

								if( US_NONSTATE == pUser->cState )
								{
									// 게임 서버로 전송
									CMSG_GM_EDIT_AIUS msgAIUS;
									CopyMemory( msgAIUS.cAccount, pAius->szAccount, en_max_lil+1 );

									CopyMemory( &msgAIUS.aius, &pAius->aius, sizeof(_ACCOUNT_ITEM_USE_STATUS) );

									_h_DS_Com * pDSCom = g_ServerInfo.GetDSCom( pUser->iServer );
									if( pDSCom )
										pDSCom->Send( msgAIUS.usLength, (char*)&msgAIUS );
								}

								pUser->cDepotBackupState = BS_READY ;
								g_pBackupReady->InsertAcc( pUser ) ;
							}
							else
							{
								g_pTrace->OutputString( _TRACE_CLASS_GMS, "[ERROR] Cannot load account info. (acc:%s) \n", pAius->szAccount ) ;
							}
						}
						else if( en_gm_edit_item_edit_item == pEditItem->ucMode )
						{
							MSG_GM_EDIT_ONE_ITEM * pOneItem = static_cast<MSG_GM_EDIT_ONE_ITEM*>(pEditItem);
							// /*
							PrintConsole( "[RECV] MSG_GM_EDIT_ONE_ITEM ---\n" );
							PrintConsole( " Chr=%s, slot = %u\n", pOneItem->szCharacName, pOneItem->slot );
							// */
						}
						else if( en_gm_edit_item_edit_period == pEditItem->ucMode )
						{
							MSG_GM_EDIT_ITEM_PERIOD * pEditPeriod = static_cast<MSG_GM_EDIT_ITEM_PERIOD*>(pEditItem);
							// /*
							PrintConsole( "[RECV] MSG_GM_EDIT_ITEM_PERIOD ---\n" );
							PrintConsole( " Chr=%s, slot = %u\n", pEditPeriod->szCharacName, pEditPeriod->slot );
							PrintConsole( " %u-%u-%u %u:%u\n", pEditPeriod->info.ucYear+1900, pEditPeriod->info.ucMonth, 
								pEditPeriod->info.ucDay, pEditPeriod->info.ucHour, pEditPeriod->info.ucMinute );
							// */
							if( pEditPeriod->slot >= _MAX_INVEN_SLOT_COUNT_ )
							{
								PrintConsole( "[ERROR] Edit Item Period - Slot index (%u)\n", pEditPeriod->slot );
								continue;
							}

							pCharac = g_pAccMapManager->Find_Charac( pEditPeriod->szCharacName );

							if( NULL == pCharac )
							{
								pUser = AddUserFromCharacName( g_UDP_DB_Command, g_UDP_USER_DB_Command, pEditPeriod->szCharacName );
								if( pUser )
									pCharac = g_pAccMapManager->Find_Charac( pEditPeriod->szCharacName );
							}

							if( pCharac )
							{
								_sServer_InvenItem_v1 * pInven = (_sServer_InvenItem_v1 *)pCharac->CHARAC_INVENTORY.cInventory;
								if( pInven[pEditPeriod->slot].sID )
								{
									__inven_uselimit_info_v1 * info = &pInven[pEditPeriod->slot].uselimit;
									info->ucFlag1 = 1;
									info->ucYear = pEditPeriod->info.ucYear;
									info->ucMonth = pEditPeriod->info.ucMonth;
									info->ucDay = pEditPeriod->info.ucDay;
									info->ucHour = pEditPeriod->info.ucHour;
									info->ucMinute = pEditPeriod->info.ucMinute;
								}
								else
								{
									PrintConsole( "[ERROR] Empty slot(%u)\n", pEditPeriod->slot );
								}

								if( pCharac->CHARAC_ETC.cState == CS_FIELD )
								{
									CMSG_GM_EDIT_PERIOD msgPeriod;

									CopyMemory( msgPeriod.cCharacName, pEditPeriod->szCharacName, en_charac_name_length+1 );
									msgPeriod.slot = pEditPeriod->slot;
									CopyMemory( &msgPeriod.info, &pEditPeriod->info, sizeof(__inven_uselimit_info_v1) );

									_h_DS_Com * pDSCom = g_ServerInfo.GetDSCom( pCharac->CHARAC_CUR_BASIC.sZone );
									if( pDSCom )
										pDSCom->Send( msgPeriod.usLength, (char*)&msgPeriod );
								}

								pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_READY;
								g_pBackupReady->InsertCharac(pCharac);
							}
							else
							{
								g_pTrace->OutputString( _TRACE_CLASS_GMS, "[ERROR] Cannot  load charac info. (chr:%s)\n", pEditPeriod->szCharacName );
							}
						}
						else if( en_gm_edit_item_del_period == pEditItem->ucMode )
						{
							MSG_GM_EDIT_DEL_PERIOD * pDelPeriod = static_cast<MSG_GM_EDIT_DEL_PERIOD*>(pEditItem);
							// /*
							PrintConsole( "[RECV] MSG_GM_EDIT_DEL_PERIOD ---\n" );
							PrintConsole( " Chr=%s, slot = %u\n", pDelPeriod->szCharacName, pDelPeriod->slot );
							// */
							if( pDelPeriod->slot >= _MAX_INVEN_SLOT_COUNT_ )
							{
								PrintConsole( "[ERROR] Edit Item Period - Slot index (%u)\n", pDelPeriod->slot );
								continue;
							}

							pCharac = g_pAccMapManager->Find_Charac( pDelPeriod->szCharacName );

							if( NULL == pCharac )
							{
								pUser = AddUserFromCharacName( g_UDP_DB_Command, g_UDP_USER_DB_Command, pDelPeriod->szCharacName );
								if( pUser )
									pCharac = g_pAccMapManager->Find_Charac( pDelPeriod->szCharacName );
							}

							if( pCharac )
							{
								_sServer_InvenItem_v1 * pInven = (_sServer_InvenItem_v1 *)pCharac->CHARAC_INVENTORY.cInventory;
								if( pInven[pDelPeriod->slot].sID )
								{
									__inven_uselimit_info_v1 * info = &pInven[pDelPeriod->slot].uselimit;
									info->ucFlag1 = 0;
									info->ucYear = 0;
									info->ucMonth = 0;
									info->ucDay = 0;
									info->ucHour = 0;
									info->ucMinute = 0;
								}
								else
								{
									PrintConsole( "[ERROR] Empty slot(%u)\n", pDelPeriod->slot );
								}

								if( pCharac->CHARAC_ETC.cState == CS_FIELD )
								{
									CMSG_GM_EDIT_DEL_PERIOD msgPeriod;

									CopyMemory( msgPeriod.cCharacName, pDelPeriod->szCharacName, en_charac_name_length+1 );
									msgPeriod.slot = pDelPeriod->slot;
	
									_h_DS_Com * pDSCom = g_ServerInfo.GetDSCom( pCharac->CHARAC_CUR_BASIC.sZone );
									if( pDSCom )
										pDSCom->Send( msgPeriod.usLength, (char*)&msgPeriod );
								}

								pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_READY;
								g_pBackupReady->InsertCharac(pCharac);
							}
							else
							{
								g_pTrace->OutputString( _TRACE_CLASS_GMS, "[ERROR] Cannot  load charac info. (chr:%s)\n", pDelPeriod->szCharacName );
							}
						}
						else if( en_gm_edit_item_edit_skintear == pEditItem->ucMode )
						{
							MSG_GM_EDIT_EDIT_SKINTEAR * pEditSkin = static_cast<MSG_GM_EDIT_EDIT_SKINTEAR*>(pEditItem);
							// /*
							PrintConsole( "[RECV] MSG_GM_EDIT_EDIT_SKINTEAR ---\n" );
							PrintConsole( " Chr=%s, slot = %u\n", pEditSkin->szCharacName, pEditSkin->slot );
							// */
							if( 0 == pEditSkin->ucWhere )
							{
								if( pEditSkin->slot >= _MAX_INVEN_SLOT_COUNT_ )
								{
									PrintConsole( "[ERROR] Edit Item SkinTear - Slot index (%u)\n", pEditSkin->slot );
									continue;
								}

								pCharac = g_pAccMapManager->Find_Charac( pEditSkin->szCharacName );

								if( NULL == pCharac )
								{
									pUser = AddUserFromCharacName( g_UDP_DB_Command, g_UDP_USER_DB_Command, pEditSkin->szCharacName );
									if( pUser )
										pCharac = g_pAccMapManager->Find_Charac( pEditSkin->szCharacName );
								}

								if( pCharac )
								{
									_sServer_InvenItem_v1 * pInven = pCharac->CHARAC_INVENTORY.GetSlot(pEditSkin->slot);
									if( pInven )
									{
										pInven->uselimit.usResaved = pEditSkin->info.usResaved;
										if( 0 == pEditSkin->info.usResaved )
										{
											pInven->uselimit.usCurMaxProtect = 0;
											pInven->uselimit.usCurProtect = 0;
											pInven->uselimit.usUsedProtect = 0;
										}
										else
										{
											pEditSkin->info.usResaved = 1;
											pInven->uselimit.usCurMaxProtect = pEditSkin->info.usCurMaxProtect;
											pInven->uselimit.usCurProtect = pEditSkin->info.usCurProtect;
											pInven->uselimit.usUsedProtect = pEditSkin->info.usUsedProtect;
										}

										if( pCharac->CHARAC_ETC.cState == CS_FIELD )
										{
											CMSG_GM_EDIT_EDIT_SKINTEAR msgSkin;

											CopyMemory( msgSkin.cCharacName, pEditSkin->szCharacName, en_charac_name_length+1 );
											msgSkin.ucWhere = 0;
											msgSkin.slot = pEditSkin->slot;
											CopyMemory( &msgSkin.info, &pEditSkin->info, sizeof(__inven_uselimit_info_v1) );

											_h_DS_Com * pDSCom = g_ServerInfo.GetDSCom( pCharac->CHARAC_CUR_BASIC.sZone );
											if( pDSCom )
												pDSCom->Send( msgSkin.usLength, (char*)&msgSkin );
										}

										pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_READY;
										g_pBackupReady->InsertCharac(pCharac);
									}
								}
								else
								{
									g_pTrace->OutputString( _TRACE_CLASS_GMS, "[ERROR] Cannot  load charac info. (chr:%s)\n", pEditSkin->szCharacName );
								}
							}
							else
							{
								// Depot
								pUser = g_pAccMapManager->Find_User( pEditSkin->szCharacName );
								if( NULL == pUser )
                                    pUser = AddUserFromAccount( g_UDP_DB_Command, g_UDP_USER_DB_Command, pEditSkin->szCharacName );

								if( pUser )
								{
									_sServer_InvenItem_v1 * pInven = pUser->stUserDepot.GetSlot(pEditSkin->slot);

									pInven->uselimit.usResaved = pEditSkin->info.usResaved;
									if( 0 == pEditSkin->info.usResaved )
									{
										pInven->uselimit.usCurMaxProtect = 0;
										pInven->uselimit.usCurProtect = 0;
										pInven->uselimit.usUsedProtect = 0;
									}
									else
									{
										pEditSkin->info.usResaved = 1;
										pInven->uselimit.usCurMaxProtect = pEditSkin->info.usCurMaxProtect;
										pInven->uselimit.usCurProtect = pEditSkin->info.usCurProtect;
										pInven->uselimit.usUsedProtect = pEditSkin->info.usUsedProtect;
									}

									pUser->cDepotBackupState = BS_READY ;
									g_pBackupReady->InsertAcc( pUser ) ;

									if( pUser->cState == US_FIELD )
									{
										CMSG_GM_EDIT_EDIT_SKINTEAR msgSkin;

										CopyMemory( msgSkin.cCharacName, pEditSkin->szCharacName, sizeof(pEditSkin->szCharacName) );
										msgSkin.ucWhere = 1;
										msgSkin.slot = pEditSkin->slot;
										CopyMemory( &msgSkin.info, &pEditSkin->info, sizeof(__inven_uselimit_info_v1) );

										sendGS( pUser->iServer, (char*)&msgSkin, msgSkin.usLength ) ;
									}
								}
							}
						}
					}
					break;

				case MSG_GM_EVENT_NUM:
					{
						MSG_GM_EVENT * pGmEvent = reinterpret_cast<MSG_GM_EVENT*>(buf);
						switch( pGmEvent->ucMode )
						{
						case en_gm_event_instant_buff:
							{
								MSG_GM_EVENT_INSTANT_BUFF * pBuff = static_cast<MSG_GM_EVENT_INSTANT_BUFF*>(pGmEvent);
								PrintAll( "[GM EVENT] Instant : Buff %d-%d )\n", pBuff->buffIndex, pBuff->buffStep );

								//
								if( !(pBuff->buffStep>0 && pBuff->buffStep<=12 ) )
								{
									PrintAll( " ==> Step Error : 1 - 12\n" );
									break;
								}

								CMSG_GM_EVENT_INSTANT_BUFF cmsg_instant;
								cmsg_instant.usLength = sizeof(CMSG_GM_EVENT_INSTANT_BUFF);
								cmsg_instant.cMessage = CMSG_NO_GM_EVENT;
								cmsg_instant.ucMode = en_cgm_event_instant_buff;

								cmsg_instant.ucTarget = pBuff->ucTarget;
								cmsg_instant.buffIndex = pBuff->buffIndex;
								cmsg_instant.buffStep = pBuff->buffStep-1;


								for( int i = 0; i < 23 ; ++i )
								{
									if( pBuff->TargetGS[i] )
									{
										// 패킷을 보낸다.
										sendGS( i+1, (char*)&cmsg_instant, cmsg_instant.usLength ) ;
									}
								}
							}
							break;
						}
					}
					break;

				case MSG_GM_ORG_CTRL_NUM:
					{
						MSG_GM_ORG_CTRL * p = reinterpret_cast<MSG_GM_ORG_CTRL*>(buf);
						if( en_org_ctrl_charac_reset == p->ucMode )
						{
							MSG_GM_ORG_CHARAC_RESET * pReset = static_cast<MSG_GM_ORG_CHARAC_RESET*>(p);

							PrintConsole( "[GM ORG] Recv Charac Reset : <%s> - <%d>\n", pReset->cCharacName, pReset->iOrgIndex );
							
							int ret = g_pOrganizationManager->PopORUnit( pReset->iOrgIndex, pReset->cCharacName );
							if( -1 == ret || 1 == ret )
							{
								// 해체 / 성공
								PCHARAC pCharac = g_pAccMapManager->Find_Charac( pReset->cCharacName );
								if( pCharac )
								{
									// 백업으로 
									if( CS_FIELD == pCharac->CHARAC_ETC.cState )
									{
										SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS msgUnitControl ={0} ;
										msgUnitControl.usLength = sizeof(SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS) ;
										msgUnitControl.cMessage = SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS ;
										msgUnitControl.iIndex = pReset->iOrgIndex ;
										memcpy( msgUnitControl._unit.m_cName, pReset->cCharacName, 13 ) ;
										msgUnitControl.cCode = SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_KICK_ ;

										g_ServerInfo.send_to_all_gs( &msgUnitControl, msgUnitControl.usLength );
										
									}
									else
									{
										pCharac->CHARAC_STATE.iORIndex = 0;
									}
										
									pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY;
									g_pBackupReady->InsertCharac(pCharac);
								}
								else
								{
									// DB 에 직접 업데이트
									int cuid = g_UDP_DB_Command->GetCharacCUID( pReset->cCharacName );
									if( cuid )
										g_UDP_DB_Command->GMS_ResetCharacOrg( cuid );
								}
							}
							else if( 0 == ret )
							{
								// 실패를 보내거나 말거나. 
							}
							
						}
						else if( en_org_ctrl_entrust_captain == p->ucMode )
						{
							MSG_GM_ORG_ENTRUST_CAPTAIN * pEntrust = static_cast<MSG_GM_ORG_ENTRUST_CAPTAIN*>(p);

							PrintConsole( "[GM ORG] Recv Entrust Captain : <%s> - <%d>\n", pEntrust->cNewName, pEntrust->iOrgIndex );

							if( g_pOrganizationManager->ExchangeCaptain( pEntrust->iOrgIndex, pEntrust->cNewName ) )
							{
								SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS msgUnitControl ={0, };
								msgUnitControl.usLength = sizeof(SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS);
								msgUnitControl.cMessage = SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS;
								msgUnitControl.iIndex = pEntrust->iOrgIndex;	
								memcpy(msgUnitControl._unit.m_cName, pEntrust->cNewName, 13);	
								msgUnitControl.cCode = SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_ENTRUST_;

								g_pDSComQueue->GlobalSend( msgUnitControl.usLength, (char*)&msgUnitControl );
							}
						}
						else if( en_org_ctrl_kick == p->ucMode )
						{
							MSG_GM_ORG_KICK * pKick = static_cast<MSG_GM_ORG_KICK*>(p);

							PrintConsole( "[GM ORG] Recv Kick : <%s> - <%d>\n", pKick->cCharacName, pKick->iOrgIndex );

							int ret = g_pOrganizationManager->PopORUnit( pKick->iOrgIndex, pKick->cCharacName );
							if( -1 == ret || 1 == ret )
							{
								// 해체 / 성공
								PCHARAC pCharac = g_pAccMapManager->Find_Charac( pKick->cCharacName );
								if( pCharac )
								{
									// 백업으로 
									if( CS_FIELD == pCharac->CHARAC_ETC.cState )
									{
										SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS msgUnitControl ={0} ;
										msgUnitControl.usLength = sizeof(SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS) ;
										msgUnitControl.cMessage = SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS ;
										msgUnitControl.iIndex = pKick->iOrgIndex ;
										memcpy( msgUnitControl._unit.m_cName, pKick->cCharacName, 13 ) ;
										msgUnitControl.cCode = SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_KICK_ ;

										g_ServerInfo.send_to_all_gs( &msgUnitControl, msgUnitControl.usLength );

									}
									else
									{
										pCharac->CHARAC_STATE.iORIndex = 0;
									}

									pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY;
									g_pBackupReady->InsertCharac(pCharac);
								}
								else
								{
									// DB 에 직접 업데이트
									int cuid = g_UDP_DB_Command->GetCharacCUID( pKick->cCharacName );
									if( cuid )
										g_UDP_DB_Command->GMS_ResetCharacOrg( cuid );
								}
							}
							else if( 0 == ret )
							{
								// 실패를 보내거나 말거나. 
							}

						}
						else if( en_org_ctrl_destroy_org == p->ucMode )
						{
							//g_pOrganizationManager->GMProc_DestroyOrg( static_cast<MSG_GM_ORG_DESTROY_ORG*>(p) );
						}
					}
					break;

				case MSG_GM_EVENT_REWARD_REQ_NUM:
					{
						MSG_GM_EVENT_REWARD_REQ * pReq = reinterpret_cast<MSG_GM_EVENT_REWARD_REQ*>( buf );

						pReq->pCharacName[en_charac_name_length] = 0;

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[GM RECV] Event Item Insert : C<%s>, EV(%d) Item(%d,%d)\n", 
							pReq->pCharacName, pReq->iEventID, pReq->cItemType, pReq->sItemID );

						pCharac = g_pAccMapManager->Find_Charac( pReq->pCharacName );

						if( !pCharac )
						{
							if( AddUserFromCharacName( g_UDP_DB_Command, g_UDP_USER_DB_Command, pReq->pCharacName ) )
								pCharac = g_pAccMapManager->Find_Charac( pReq->pCharacName );
						}

						if( pCharac )
						{
							u_int64	 i64ItemUID = SM_Item_Make_UniqueID() ;

							bool bAddSuccess = false ;
							int nCount = 50;

							_sServer_InvenItem_v1 * Inven = (_sServer_InvenItem_v1*)pCharac->CHARAC_INVENTORY.cInventory ;
							// 행낭 개수 구하기.
							if( Inven[23].sID != 0 )
								nCount += 25;
							if( Inven[24].sID != 0 )
								nCount += 25;

							for( int i = 25 ; i < nCount ; i++ )
							{
								if( Inven[i].sID == 0 )
								{
									ZeroMemory( &Inven[i], sizeof(_sServer_InvenItem_v1) );

									Inven[i].ui64UniqueId = i64ItemUID;
									Inven[i].cType = pReq->cItemType;
									Inven[i].cSecond = pReq->cItemSecondType;
									Inven[i].sID = pReq->sItemID;
                                    								
									Inven[i].Item_Money.S_NormalItem.usCurDur = pReq->usDurability;
									Inven[i].Item_Money.S_NormalItem.ucSocketCount = pReq->ucSlotCount;
									Inven[i].Item_Money.S_NormalItem.ucCount = static_cast<u_char>(pReq->sItemCount);

									Inven[i].ucStrength = pReq->ucInchant;

									bAddSuccess = true ;

									break;
								}
							}

							if( bAddSuccess )
							{
								// 성공이면 로그 남기기..
								_ItemLog * pItemLog =  g_pLogSvrCom->GetItemLogSlot() ;
								if( pItemLog )
								{
									memset( pItemLog, 0, sizeof(_ItemLog) ) ;

									pItemLog->item_uid = i64ItemUID;
									pItemLog->code = _LOG_ITEM_ADD_FROM_GMS_SUCC;
									pItemLog->zone = 0;
									sprintf( pItemLog->from, "GMS->INVEN" );
									pItemLog->to_uid = pCharac->CHARAC_BASIC.iUniqueID;
									memcpy( pItemLog->to, pCharac->CHARAC_BASIC.cChrName, 13 );
									pItemLog->type1 = pReq->cItemType;
									pItemLog->type2 = pReq->sItemID;

									g_pLogSvrCom->InsertItemLog( pItemLog );
								}

								MSG_GM_EVENT_REWARD_RES pkRes;
							}
							else
							{
								_ItemLog * pItemLog =  g_pLogSvrCom->GetItemLogSlot();
								if( pItemLog )
								{
									memset( pItemLog, 0, sizeof(_ItemLog) );

									pItemLog->item_uid = i64ItemUID;
									pItemLog->code = _LOG_ITEM_ADD_FROM_GMS_FAIL;
									pItemLog->zone = 0;
									sprintf( pItemLog->from, "GMS->INVEN" );
									pItemLog->to_uid = pCharac->CHARAC_BASIC.iUniqueID;
									memcpy( pItemLog->to, pCharac->CHARAC_BASIC.cChrName, 13 );

									pItemLog->type1 = pReq->cItemType;
									pItemLog->type2 = pReq->sItemID;

									g_pLogSvrCom->InsertItemLog( pItemLog );
								}
							}


							pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_READY ;
							g_pBackupReady->InsertCharac( pCharac ) ;

							// 실패다. 실패 메시지 되돌린다.
							MSG_GM_EVENT_REWARD_RES		pkRes;
							pkRes.header.iKey = PASS_KEY;
							pkRes.header.ucMessage = MSG_GM_EVENT_REWARD_RES_NUM;
							pkRes.header.uiTime = pReq->header.uiTime;

							pkRes.iEventID = pReq->iEventID;

							if( bAddSuccess )
								pkRes.iResult = 1;
							else
								pkRes.iResult = 2;

							if( fromType == 0 )
								g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&pkRes, sizeof(MSG_GM_EVENT_REWARD_RES));
							else
								pConObj->SendRequest( &pkRes, sizeof(MSG_GM_EVENT_REWARD_RES) );

							g_pSystemLog->LogFileWrite( "[GMS EVNET ITEM INSERT] C<%s> Item( %d,%dn%u ) : %s\n", 
								pReq->pCharacName, pReq->cItemType, pReq->sItemID, pReq->sItemCount, (pkRes.iResult) ? "Success" : "Fail" );

						}
						else
						{
							// 실패다. 실패 메시지 되돌린다.
							MSG_GM_EVENT_REWARD_RES		pkRes;
							pkRes.header.iKey = PASS_KEY;
							pkRes.header.ucMessage = MSG_GM_EVENT_REWARD_RES_NUM;
							pkRes.header.uiTime = pReq->header.uiTime;

							pkRes.iEventID = pReq->iEventID;
							pkRes.iResult = 0;

							if( fromType == 0 )
								g_pUDPSendPacketQ->insert_udp(fromAddr, (char*)&pkRes, sizeof(MSG_GM_EVENT_REWARD_RES));
							else
								pConObj->SendRequest( &pkRes, sizeof(MSG_GM_EVENT_REWARD_RES) );

							g_pSystemLog->LogFileWrite( "[GMS EVNET ITEM INSERT] C<%s> Item( %d,%d,%u ) : %s\n", 
								pReq->pCharacName, pReq->cItemType, pReq->sItemID, pReq->sItemCount, "Fail" );
						}
					}
					break;
					
			#ifdef _CHARACLIST_DELETE_CHARACTER_20061214_
				case MSG_GM_DELETE_CHARAC_NUM:		//캐릭터 삭제(캐릭터가 로그아웃 상태일때)
					{
						g_pTrace->OutputString( _TRACE_CLASS_GMS, "[INFO] Recv MSG_GM_DELETE_CHARAC_NUM \n");

						MSG_GM_DELETE_CHARAC * pGMDeleteCharac = reinterpret_cast<MSG_GM_DELETE_CHARAC*>( buf );


						//////////////////////////////캐릭터 찾기
						pGMDeleteCharac->pCharacName[en_charac_name_length] = 0;
						pCharac = g_pAccMapManager->Find_Charac( pGMDeleteCharac->pCharacName );
						
						//실패
						if(pCharac == NULL)
						{
							::PrintConsole( "[ERROR] if( pCharac == NULL )  \n %s, %d \n", __FILE__, __LINE__ ) ;
							break;
						}

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "  Find : Charac<%s>\n", pGMDeleteCharac->pCharacName ) ;
						
						//////////////////////////////유저찾기
						pUser =  g_pAccMapManager->Find_User( pCharac->CHARAC_BASIC.cAccount ) ;

						//실패
						if(pUser == NULL)
						{
							::PrintConsole( "[ERROR] if( pUser == NULL )  \n %s, %d \n", __FILE__, __LINE__ ) ;
							break;
						}

						g_pTrace->OutputString( _TRACE_CLASS_GMS, "  Find : Acc<%s>\n", pCharac->CHARAC_BASIC.cAccount ) ;

						///////////////////////////////캐릭터 상태 : 캐릭터가 로그아웃상태 아니면 실패
						//if(pUser->cState != US_NONSTATE || pCharac->CHARAC_ETC.cState != CS_NONSTATE)
						if(pUser->cState != US_NONSTATE)
						{
							::PrintConsole( "[ERROR] if( pUser->cState != US_NONSTATE ) pUser->cState: %d, pCharac->CHARAC_ETC.cState : %d \n %s, %d \n", pUser->cState, pCharac->CHARAC_ETC.cState, __FILE__, __LINE__ ) ;
							break;
						}
						::PrintConsole( "[SUCCESS] DeleteCharac if( pUser->cState == US_NONSTATE ) pUser->cState: %d, pCharac->CHARAC_ETC.cState : %d \n %s, %d \n", pUser->cState, pCharac->CHARAC_ETC.cState, __FILE__, __LINE__ ) ;

						///////////////////////////////캐릭터 삭제
						int iCharacUID = pCharac->CHARAC_BASIC.iUniqueID ;
						int iResult = g_UDP_DB_Command->pr_Charac_Delete( iCharacUID ) ;

						//실패
						if(iResult != 0)
						{
							::PrintConsole( "[ERROR] if( g_UDP_DB_Command->pr_Charac_Delete( iCharacUID ) != 0 ) \n %s, %d \n", __FILE__, __LINE__ ) ;
							break;
						}

						///////////////////////////////캐릭터 삭제 성공
						
						// pCharac 메모리 삭제 부분
						pUser->user_lock() ;

						// 성공이면 로그 남기기..
						_CharacLog * pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
						if( pCharacLog )
						{
							memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

							pCharacLog->code = _LOG_CHARAC_DELETE;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
							memcpy( pCharacLog->cAccount, pUser->cNDAccount, en_max_lil) ;
							pCharacLog->cAccount[en_max_lil] = 0 ;
#else
							memcpy( pCharacLog->account, pUser->cNDAccount, 12) ;
							pCharacLog->account[12] = 0 ;
#endif
							memcpy( pCharacLog->charac, pCharac->CHARAC_BASIC.cChrName, en_charac_name_length);
							pCharacLog->charac[en_charac_name_length] = 0;
							pCharacLog->charac_uid = pCharac->CHARAC_BASIC.iUniqueID ;
							in_addr addr = {0};
							addr.S_un.S_addr = pUser->iIp ;
							memcpy( pCharacLog->ip, inet_ntoa( addr ), 17) ;
							pCharacLog->level = pCharac->CHARAC_STATE.sInnerLevel ;
							pCharacLog->point = pCharac->CHARAC_LEVEL.sLeftPoint ;								
							pCharacLog->zone = pUser->iServer ;
							int iMoreInfoLog = 1;
							memcpy( &iMoreInfoLog, &pCharacLog->moreInfo[0], sizeof(iMoreInfoLog) ) ; //GMS 캐릭터 삭제 로그
							g_pLogSvrCom->InsertCharacLog( pCharacLog ) ;
							::PrintConsole( "[LOG_WRITE] DeleteCharac chr_name : %s ) \n %s, %d \n", pCharac->CHARAC_BASIC.cChrName, __FILE__, __LINE__ ) ;
						}

						// 분파에 가입되어 있다면 분파에서 삭제해라..
						if( pCharac->CHARAC_STATE.iORIndex > 0 )
						{
							_Organization * pOR = g_pOrganizationManager->FindOrganization( pCharac->CHARAC_STATE.iORIndex ) ;
							_Unit * pUnit = NULL ;
							if( pOR )
							{
								pUnit = pOR->FindUnit( pCharac->CHARAC_BASIC.cChrName ) ;
							}

							if( pUnit && pOR )
							{

								if( pUnit->m_cClass == _Unit::_UNIT_CALSS_CAPTAIN_UNIT_ )
								{							
									for( int i = 0 ; i < _MAX_OR_UNIT_NUM ; i++ )
									{
										if( pOR->m_MemberList[i].m_cName[0] != 0 && pOR->m_MemberList[i].m_cClass != _Unit::_UNIT_CALSS_CAPTAIN_UNIT_ )
										{
											g_pOrganizationManager->ExchangeCaptain( pOR->m_iIndex, pOR->m_MemberList[i].m_cName ) ;

											SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS msgORUnitControl = {0} ;
											msgORUnitControl.usLength = sizeof(SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS) ;
											msgORUnitControl.cMessage = SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS ;
											msgORUnitControl.iIndex = pOR->m_iIndex ;
											msgORUnitControl._unit = pOR->m_MemberList[i] ;
											msgORUnitControl.cCode = SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_ENTRUST_ ;

											g_pDSComQueue->GlobalSend( msgORUnitControl.usLength, (char*)&msgORUnitControl ) ;
											break ;
										}
									}

								}

								_Unit tempUnit = *pUnit ;
								int iRet = g_pOrganizationManager->PopORUnit( pCharac->CHARAC_STATE.iORIndex, pCharac->CHARAC_BASIC.cChrName ) ;
								if( iRet == -1 )
								{
#ifdef _PD_CASTLE_STEP_1_
									CDSCastleManager::GetObj().notice_group_breakup( en_cb_group_type_org, pCharac->CHARAC_STATE.iORIndex );
#endif
									SMSG_OR_RESULT_DS_GS msgORResult = {0} ;
									msgORResult.usLength = sizeof(SMSG_OR_RESULT_DS_GS) ;
									msgORResult.cMessage = SMSG_NO_OR_RESULT_DS_GS ;
									msgORResult.iIndex = pCharac->CHARAC_STATE.iORIndex ;
									msgORResult.iCode = SMSG_OR_RESULT_DS_GS::_OR_RESULT_DELETE_ ;
									g_pDSComQueue->GlobalSend( msgORResult.usLength, (char*)&msgORResult ) ;
								}
								else
								{
									SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS msgOrUnitControlReq = {0} ;
									msgOrUnitControlReq.usLength = sizeof(SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS) ;
									msgOrUnitControlReq.cMessage = SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS ;
									msgOrUnitControlReq.iIndex = pCharac->CHARAC_STATE.iORIndex ;
									msgOrUnitControlReq._unit = tempUnit ;
									msgOrUnitControlReq.cCode = SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_DELETE_ ;

									g_pDSComQueue->GlobalSend( msgOrUnitControlReq.usLength, (char*)&msgOrUnitControlReq ) ;						
								}
							}			

						}

						// MS 에도 삭제사실을 통보해야한다.
						UMSG_CHARAC_DELETE_DS_MS msgCharacDelete = {0} ;
						msgCharacDelete.iKey = PASS_KEY ;
						msgCharacDelete.cMessage = UMSG_CHARAC_DELETE_DS_MS_NUM ;

						memcpy( msgCharacDelete.cCharacName, pGMDeleteCharac->pCharacName, 13 ) ;
						msgCharacDelete.iUid = pCharac->CHARAC_BASIC.iUniqueID ;
						SOCKADDR_IN	* pAddr = g_ServerInfo.GetMSSockAddrIn() ;

						g_pUDPSendPacketQ->insert_udp( pAddr, (char*)&msgCharacDelete, sizeof(msgCharacDelete) ) ;


						--pUser->cCharacCount;	//My character count --				

						// 맵에서 삭제한다.
						pCharac->charac_lock() ;
						try{
							if( g_pAccMapManager->CharacErase(pCharac) == false ) 
							{
								::PrintConsole( "[ERROR] if( g_pAccMapManager->CharacErase(pCharac) == false )  \n %s, %d \n", __FILE__, __LINE__ ) ;
							}
						}
						catch(...){
							::PrintConsole("[EXEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
						}
						pCharac->charac_unlock() ;
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_CHARAC,"[INFO] Delete one character (%s) \n",pGMDeleteCharac->pCharacName);
#endif
						pUser->user_unlock() ;


					}
					break;
			#endif//MSG_GM_DELETE_CHARAC_NUM

				default: 
					;
				}				

			}
		}
		catch (...) {						
			g_pSystemLog->LogFileWrite( "[EXCEPTION] th_UDPGMSPacketWorkThread (pos.%d)\n", pos  ) ;
			::PrintConsole( "[EXCEPTION] th_UDPGMSPacketWorkThread (pos.%d)\n", pos ) ;
		}

				

	} //while


	::PrintConsole("DWORD WINAPI th_UDPGMSPacketWorkThread( LPVOID lp ) Exit \n") ;
	printf("DWORD WINAPI th_UDPGMSPacketWorkThread( LPVOID lp ) Exit \n") ;

	delete g_UDP_DB_Command ;	

	_endthreadex(0);
	return 0 ;

}


PUSER AddUserFromAccount( COLE_DB_Executer * pDBCommand, C_USER_DB_Command * pUserCommand, char * acc )
{
	PUSER pUser = NULL;
	pUser = g_pAccMapManager->Find_User( acc );

	g_pTrace->OutputString( _TRACE_CLASS_GMS, "  Find : Acc<%s>\n", acc );

	if( pUser == NULL )
	{							
		pUser = g_pAccMapManager->GetWritePtr();	// Get pointer(map) vitual memory							
		if( pUser == NULL )
			return NULL;

		pUser->iTime = CUser::get_time_by_sec();
		memcpy( pUser->cNDAccount, acc, en_max_lil+1);

		bool bRes = pUserCommand->DSGetAccountUID( pUser->cNDAccount, pUser->iUid );
		if( bRes == false )
		{
			::PrintConsole("  - Not Found Account A<%s>\n", pUser->cNDAccount ); 
			g_pAccMapManager->PutMemPtr( pUser ) ;
			return NULL;
		}

		if( pDBCommand->pr_GetDepot( pUser->iUid, pUser ) == false)
		{
			::PrintConsole("[DB_ERR] GetDepot failed (accout: %s) \n", acc );
			g_pAccMapManager->PutMemPtr( pUser ) ;
			return NULL;
		}

		if( false == pDBCommand->GetAccountItemUseStatus( pUser->iUid, pUser ) )
		{
			g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => DB Loading(GetAccountItemUseStatus) failed (account: %s : %d) \n", pUser->cNDAccount,pUser->iUid );
			g_pAccMapManager->PutMemPtr( pUser ) ;		// 실패했을 경우 맵을 다시 반환한다..
			return NULL;
		}

		// Get Character info
		if(	pDBCommand->Get_Charac_List( pUser->iUid, pUser )  == false)
		{		// Read charac list from DB
			::PrintConsole("[DB_ERR] : CID - %s Get failed what user info \n", pUser->cNDAccount);
			g_pAccMapManager->PutMemPtr( pUser ) ;
			return NULL;
		}

		g_pAccMapManager->Insert(pUser);

		g_pMemSaveTimer->Reserve_save( pUser, pUser->iUid, CUserMemSaveTimer::get_time_by_sec() + _MEM_SAVE_DELAY_TIME_KICK ) ;
	}

	return pUser;
}

PUSER AddUserFromCharacName( COLE_DB_Executer * pDBCommand, C_USER_DB_Command * pUserCommand, char * cname )
{
	char cAcc[en_max_lil+1] = {0};
	PUSER pUser = NULL;

	pDBCommand->GetAccName( cname, cAcc);
	if( cAcc[0] == 0 )
		return NULL;

	pUser = g_pAccMapManager->Find_User( cAcc );

	g_pTrace->OutputString( _TRACE_CLASS_GMS, "  Find : Acc<%s>\n", cAcc );

	if( pUser == NULL )
	{							
		pUser = g_pAccMapManager->GetWritePtr();	// Get pointer(map) vitual memory							
		if( pUser == NULL )
			return NULL;

		pUser->iTime = CUser::get_time_by_sec();
		memcpy( pUser->cNDAccount, cAcc, en_max_lil+1);

		bool bRes = pUserCommand->DSGetAccountUID( pUser->cNDAccount, pUser->iUid );
		if( bRes == false )
		{
			::PrintConsole("  - Not Found Account A<%s>\n", pUser->cNDAccount ); 
			g_pAccMapManager->PutMemPtr( pUser ) ;
			return NULL;
		}

		if( pDBCommand->pr_GetDepot( pUser->iUid, pUser ) == false)
		{
			::PrintConsole("[DB_ERR] GetDepot failed (accout: %s) \n", cAcc );
			g_pAccMapManager->PutMemPtr( pUser ) ;
			return NULL;
		}

		if( false == pDBCommand->GetAccountItemUseStatus( pUser->iUid, pUser ) )
		{
			g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => DB Loading(GetAccountItemUseStatus) failed (account: %s : %d) \n", pUser->cNDAccount,pUser->iUid );
			g_pAccMapManager->PutMemPtr( pUser ) ;		// 실패했을 경우 맵을 다시 반환한다..
			return NULL;
		}

		// Get Character info
		if(	pDBCommand->Get_Charac_List( pUser->iUid, pUser )  == false)
		{		// Read charac list from DB
			::PrintConsole("[DB_ERR] : CID - %s Get failed what user info \n", pUser->cNDAccount);
			g_pAccMapManager->PutMemPtr( pUser ) ;
			return NULL;
		}

		g_pAccMapManager->Insert(pUser);
		
		g_pMemSaveTimer->Reserve_save( pUser, pUser->iUid, CUserMemSaveTimer::get_time_by_sec() + _MEM_SAVE_DELAY_TIME_KICK ) ;
	}

	return pUser;
}


SOCKET RestoreSocket( const unsigned short usPortNum, char mode ) 
{
	try{
		SOCKADDR_IN udpAddr ;
		SOCKET s ;
		
		Sleep( 2000 ) ;


		const char * pIP ;
		//if( g_bWanCon )
			pIP = g_ServerInfo.GetGSAcceptIP();
		//else 
		//	pIP = GetMyIP(0) ;	// 내부 아이피 얻어오기..


		printf(" -> UDP Binding IP: %s, Port %u \n", pIP, usPortNum ) ;

		udpAddr.sin_family = AF_INET ;
		udpAddr.sin_port = htons( usPortNum ) ;// htons( g_pServerInfo->get_udp_port( 0 ) ) ;
		udpAddr.sin_addr.s_addr = inet_addr(pIP) ;// g_pServerInfo->get_ip( 0 ) ;


		// 소켓 초기화 먼저 하고
		s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ;
		if( s == INVALID_SOCKET ) {
			::PrintConsole( "ServerCom Thread Error\n" ) ;
			return INVALID_SOCKET ;
		}


		DWORD dwBytesReturned = 0;
		BOOL bNewBehavior = FALSE;
		DWORD status;

		// disable  new behavior using
		// IOCTL: SIO_UDP_CONNRESET
		status = WSAIoctl(s, SIO_UDP_CONNRESET,
			&bNewBehavior, sizeof(bNewBehavior),
			NULL, 0, &dwBytesReturned,
			NULL, NULL);


		int nTimeout = 5000 ;
		if( setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeout, sizeof(nTimeout)) != 0 )
		{
			::PrintConsole( "[ERROR] setsockopt(...) error (code : %d) \n", WSAGetLastError() ) ;
			return INVALID_SOCKET ;
		}			

		if (SOCKET_ERROR == status)
		{
			DWORD dwErr = WSAGetLastError();
			if (WSAEWOULDBLOCK == dwErr)
			{
				// nothing to do
				return(INVALID_SOCKET);
			}
			else
			{
				printf("WSAIoctl(SIO_UDP_CONNRESET) Error: %d\n", dwErr);
				return(INVALID_SOCKET);
			}
		}

		if( bind( s, (SOCKADDR *)&udpAddr, sizeof( SOCKADDR_IN ) ) == SOCKET_ERROR ) {
			::PrintConsole( "Error : ServerComThread socket bind code : %d\n", WSAGetLastError() ) ;
			closesocket( s ) ;
			return INVALID_SOCKET ;
		}

		return s ;
	}
	catch (...) {
		::PrintConsole( "[EXCEPTION] 'RestoreSocket()' Exception \n " ) ;
		return INVALID_SOCKET ;
	}

}

bool SockInit() {
	g_usPortNum = g_ServerInfo.GetDSUdpPort() ;
	g_UDPsocket = RestoreSocket( g_usPortNum, 0 ) ;
	if( g_UDPsocket == INVALID_SOCKET ) {
		::PrintConsole( "Socket initailize failed. \n" ) ;		
		return false ;
	}	
	return true ;
}

// GS 에 보내기 함수 ===================================================================================

extern _j_ServerInfo			g_ServerInfo ;
extern _DSCom_Queue *			g_pDSComQueue ;
bool sendGS( short sSvrNum, char * pMsgBuf, int size)
{
	// ADDRESS 정보
	SOCKADDR_IN *	pAddr = NULL;	
	short		sServerState = 0;
	int nReturn = 0;
	// 소켓에 IP 주소 부여
	//int iSvrNum = pMsgBuf[5];


	TRY{
		if(sSvrNum < 1){
			return false;
		}
		else if(sSvrNum == 99)
		{
			return g_pDSComQueue->GlobalSend( size, pMsgBuf ) ;			
		}
		else if( g_ServerInfo.GetDSCom( sSvrNum ) )
		{
			return  (g_ServerInfo.GetDSCom( sSvrNum ))->Send( size, pMsgBuf );
		}	
	}
	CATCH_ALL {
		::PrintConsole( "[EXCEPTION] 'sendGS()' Exception <svrNum: %d>\n ", sSvrNum );
		return false ;
	}
	return true ;
}



