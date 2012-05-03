#include "..\stdafx.h"
#include "Network.h"
//#include "DSComm.h"
#include "..\packet_queue\packet_queue.h"
#include "../Mylibrary/gGlobal.h"


extern _j_GlobalLog *			g_pSystemLog;

void MessageInsert( _h_Recv_Overlapped * pOv, const int size ) ;
/*#################################################################################
//
//  OV_Unit
//
//###############################################################################*/
#ifdef _DEBUG
void COV_UNIT::Initialize( char * buf, int cap )
{
	ZeroMemory( this, sizeof(COV_UNIT) ) ;
	ZeroMemory( buf, cap ) ;

	m_wsabuf.buf = buf ;
	m_iCapacity = cap ;
}

#else

void COV_UNIT::Initialize( char * buf )
{
	ZeroMemory( this, sizeof(COV_UNIT) ) ;
	m_wsabuf.buf = buf ;
}

#endif

void COV_UNIT::SetSendData( char * buf, int length )
{
#ifdef _DEBUG
	if( m_wsabuf.len != 0 )
		printf( " ##### SOCKET :: UNIT not Initialize msg(%u) #####\n", static_cast<u_char>(buf[2]) ) ;
	if( length <= 0 )
		printf( " ##### SOCKET :: Length Error msg(%u) #####\n", static_cast<u_char>(buf[2]) ) ;
#endif
	CopyMemory( m_wsabuf.buf, buf, length ) ;
	m_wsabuf.len = length ;
}


int COV_UNIT::SendPacket( const SOCKET sock, const bool bZeroCheck )
{

	if( bZeroCheck == true )
	{
		u_short * usLen = (u_short*)&m_wsabuf.buf[0] ;

		if( *usLen == 0 )
		{
			printf("[SOCK_ERROR] SendPacket() m_wsabuf.buf[0] == 0 \n") ;
			return -1 ;
		}
	}
	

	int err ;
	err = WSASend( sock, &m_wsabuf, 1, &m_wsabuf.len, 0, (LPWSAOVERLAPPED)this, NULL ) ;
	if( err == SOCKET_ERROR )
	{
		err = WSAGetLastError() ;
		if( err == WSA_IO_PENDING )
		{
			return 0 ;
		}
		else
		{
//#ifdef _DEBUG
			//printf( " #### SOCKET :: SendError(%d) ####\n", err ) ;
//#endif 
			return err ;
		}
	}

	return 0 ;

}

/*

void CDSRecv_Overlapped::reset()
{
	Internal			= InternalHigh	= 0;	
	Offset				= OffsetHigh	= 0;
	hEvent				= NULL;	
	m_wsabuf.buf = m_buf ;
	m_wsabuf.len = 0 ;
	pRealBuf = m_buf ;		// Add...
	RemainBytes = 0 ;		// Add...
	m_dwFlag = 0 ;
	m_iMode = mode_recv ;
}

void CDSRecv_Overlapped::move( int remain ) 
{
	TRY{
		RemainBytes = remain ;
		memmove( m_buf, pRealBuf, RemainBytes ) ;
		pRealBuf = m_buf ;
	}
	CATCH_ALL{

		::PrintConsole( "[EXCEPTION] Recv Overlapped::move\n" ) ;

		reset() ;
	}
}

// Send Request

// Recv Request
bool CDSRecv_Overlapped::RecvPacket( const SOCKET sock )
{
	int err ;

	DWORD len = 0 ;
	TRY{
		m_wsabuf.len = m_bufSize - RemainBytes ;
		m_wsabuf.buf = m_buf + RemainBytes ;
		memset( m_wsabuf.buf, 0, m_wsabuf.len ) ;
		m_iMode = mode_recv ;
	}
	CATCH_ALL{
		reset() ;
		printf("%s:%d> Exception here!\n", __FILE__, __LINE__);
		return false ;
	}
	if( SOCKET_ERROR == WSARecv( sock, &m_wsabuf, 1, &len, &m_dwFlag, (LPWSAOVERLAPPED)this, NULL ) )
	{
		err = WSAGetLastError() ;
		if( err == WSA_IO_PENDING )
		{
			return true ;
		}
		else
		{
#ifdef _DEBUG
			printf( "Recv Trigger DS::Error Code( %d )", err ) ;			
#endif
			return false ;
		}
	}

	return true ;
}
*/



/*#################################################################################
//
//  CSendOverlappedSlot : 
//
//###############################################################################*/
bool CSendOverlappedSlot::CreateOVSlot( int slotcount, int megasize )
{
	m_iGetSlot = 0 ;
	m_iPosition = 0 ;
	if( megasize < 1 || megasize > 150 )
	{
		printf( "OV Slot :: Buf Size Abnormal\n" ) ;
		return false ;
	}
	m_iTotalSize = _MEGA_ * megasize ;
	m_nOVSlot = slotcount ;

	m_pOVSlot = new COV_UNIT[m_nOVSlot] ;

	m_pSendBuf = new char[m_iTotalSize] ;
	if( !m_pSendBuf || !m_pOVSlot )
		return false ;

	return true ;
}


void CSendOverlappedSlot::Get_SendUnit( COV_UNIT ** ppSender, int reqSize )
{
#ifdef _DEBUG
	if( reqSize <= 2 ) {
		printf( " #### Slot Size Error ####\n" ) ;
	}
#endif 

	send_ov_lock() ;

	*ppSender = &m_pOVSlot[m_iGetSlot] ;
	
	if( m_iPosition + reqSize >= m_iTotalSize )
	{
		//  다시 처음으로 간다. 
		m_iPosition = 0 ;
	}

#ifdef _DEBUG
	(*ppSender)->Initialize( &m_pSendBuf[m_iPosition], reqSize ) ;
#else
	(*ppSender)->Initialize( &m_pSendBuf[m_iPosition] ) ;
#endif

	m_iPosition += reqSize ;

	m_iGetSlot = ( m_iGetSlot + 1 ) % m_nOVSlot ;

	send_ov_unlock() ;
}

//#################################################################################
//
//	_h_Overlapped : 오버랩드 구조체. 
//
//###################################################################### LHS Added....
void _h_Recv_Overlapped::reset()
{
	Internal			= InternalHigh	= 0;	
	Offset				= OffsetHigh	= 0;
	hEvent				= NULL;
	if(m_pBuf)
		memset( m_pBuf, 0, iBufSize ) ;
	m_wsabuf.buf = m_pBuf ;
	pRealBuf = m_pBuf ;		// Add...
	RemainBytes = 0 ;		// Add...
	m_dwFlag = 0 ;
	m_bMode = true ;

	m_usLastParsingSize = 0;
	m_ucLastParsingMsg = 0;

	m_dwTotalRecvBytes = 0;
	m_iLastErrorCode = 0;
}

void _h_Recv_Overlapped::move( int remain ) 
{
	try {
		RemainBytes = remain ;
		memmove( m_pBuf, pRealBuf, RemainBytes ) ;
		pRealBuf = m_pBuf;
	}
	catch ( ... ) {
		::PrintConsole( "[EXCEPTION] Recv Overlapped::move\n" ) ;
		reset() ;
	}
}

// Send Request

// Recv Request
bool _h_Recv_Overlapped::RecvPacket( const SOCKET sock )
{
	int err ;

	DWORD len = 0 ;
	__try {
		m_wsabuf.len = iBufSize - RemainBytes ;
		if( m_wsabuf.len > 16384 )
			m_wsabuf.len = 16384;
		m_wsabuf.buf = m_pBuf + RemainBytes ;
		memset( m_wsabuf.buf, 0, m_wsabuf.len ) ;
		m_bMode = true ;
	}
	__except ( EXCEPTION_EXECUTE_HANDLER ) {
		reset() ;
		return false ;
	}
	if( SOCKET_ERROR == WSARecv( sock, &m_wsabuf, 1, &len, &m_dwFlag, (LPWSAOVERLAPPED)this, NULL ) )
	{
		err = WSAGetLastError() ;
		if( err == WSA_IO_PENDING || err == WSAEWOULDBLOCK )
		{
			return true ;
		}
		else
		{
			return false ;
		}
	}

	return true ;
}

bool _h_Recv_Overlapped::Parsing(DWORD bufLen)
{

	try{		

		u_short		head;
		int	copiedBytes = 0;		//카피된 데이타 길이(헤더 카피가 이루어졌는지 여부를 판단..)
		bool state = true;			// false 이면 while loop 를 빠져나온다..
		int availableBytes = 0;		//패킷의 길이 HEADER_LEN + BODY(head.sLength)	
		int HEADER_LEN = sizeof(head);

		RemainBytes += bufLen;		//전에 이완되어온 데이타 길이 + 새로받은 데이타길이
		pRealBuf = m_pBuf;	//버퍼커서를 맨 앞으로..

		m_dwTotalRecvBytes += bufLen;

		while( state )	//나머지 데이타가 0보다 클때까지..
		{
			/*******************************************************/
			/* Header Empty */
			/*******************************************************/
			if( copiedBytes == 0 )	//카피된 바이트가 없을경우
			{
				if( RemainBytes >= HEADER_LEN )	// 헤더길이만큼만 뽑는다.
				{				
					memcpy(&head, pRealBuf, HEADER_LEN);	//헤더 길이만큼 데이타를 뽑아내고..				
					copiedBytes += HEADER_LEN ;	//카피된 데이타 길이+뽑아낸 데이타 길이				
					continue;
				}
				else	// 뽑은게 헤더길이보다 작은경우.. 다음 큐로 미룬다..
				{
					try {
						memmove(m_pBuf, pRealBuf, RemainBytes);				
						pRealBuf = m_pBuf + RemainBytes;
						state = false;				
					}
					catch (...) {
						::PrintConsole( "[EXCEPTION] memmove : parsinglength = %d\n", RemainBytes ) ;
						return false ;
					}				
				}
			}

			/*******************************************************/
			/* Header FULL */		//쪼개지지 않고 데이다 다 받음..
			/*******************************************************/		
			else if (copiedBytes >= HEADER_LEN )
			{
				availableBytes = static_cast<int>(head);	// 헤더를 포함해서 바디까지 패킷 하나..
				if( (RemainBytes >= availableBytes) && ( 0 < availableBytes ))	// 나머지가 받을양보다 크거나 같을때..
				{
					// 메시지 번호에 따라 분류하자.. 
					MessageInsert( this, availableBytes ) ;

					pRealBuf += availableBytes ;			// 복사한만큼 메모리 번지를 점프하네..
					RemainBytes -= availableBytes ;		// 나머지 데이타를 복사한만큼 빼네..
					copiedBytes = 0 ;				

					if(RemainBytes == 0){
						state = false;
					}
				}
				else	// 나머지가 받을양보다 적은경우니깐 데이타 바디가 짤린 경우겠다..
				{				
					try {
						memmove(m_pBuf, pRealBuf, RemainBytes);				
						pRealBuf = m_pBuf + RemainBytes;
						state = false;				
					}
					catch (...) {
						::PrintConsole( "[EXCEPTION] memmove : parsinglength = %d\n", RemainBytes ) ;
						return false ;
					}				
				} // end else
			}// end else if

		} /* End While*/
		return true;
	} // end parsing()	
	catch (...) {
		::PrintConsole("[EXCEPTION] Parsing func error \n") ;
		return false ;
	}
}

	

void _h_Recv_Overlapped::ShowState()
{
	printf( "  - Last : Msg(%u), Size(%u)\n", GetLastParsingMsg(), GetLastParsingSize() );
	printf( "  - Recv %u, Err %d\n", m_dwTotalRecvBytes, m_iLastErrorCode );

	g_pSystemLog->LogFileWrite( "  - Last : Msg(%u), Size(%u)\n", GetLastParsingMsg(), GetLastParsingSize() );
	g_pSystemLog->LogFileWrite( "  - Recv %u, Err %d\n", m_dwTotalRecvBytes, m_iLastErrorCode );
}



//#################################################################################
//
//	_h_CompletionPort 구현부
//
//###################################################################### LHS Added....
bool _h_CompletionPort::Create( int n )
{
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, n);

	return (m_hIOCP != NULL) ;
}

bool _h_CompletionPort::AssociateSocket( _h_Recv_Overlapped * pOv )
{	
	return (CreateIoCompletionPort((HANDLE)pOv->m_socket, m_hIOCP, (ULONG_PTR)pOv,0) == m_hIOCP);
}

bool _h_CompletionPort::GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds)
{
	return (GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, pCompKey, ppo, dwMilliseconds) == TRUE);
}


/*#################################################################################

DS_Com

#################################################################################*/

/* =====================================================================================
createDSCom() : 생성
===================================================================================== */
bool _h_DS_Com::createDSCom( const int iSendSlotMaxCount, const int iSendSlotMaxMegaSize, const int iRecvBufSize  )
{
	if( iSendSlotMaxCount <= 0  || iSendSlotMaxMegaSize <= 0 )
	{
		return false ;
	}

	try
	{		
		m_RecvOV.m_pDSComm = this;			


		// send ov slot create		( 2004.6.17		ozzywow add )
		if ( false == m_sendSlot.CreateOVSlot( iSendSlotMaxCount , iSendSlotMaxMegaSize ) )
		{
			printf( "InnerCom::SendSlot Create Failed\n" ) ;
			return false ;
		}

		// Recv ov create
		if( false == m_RecvOV.CreateRecvOV( iRecvBufSize ) )
		{
			printf( "InnerCom::Recv OV Create Failed\n" ) ;
			return false ;
		}
	}
	catch (...) 
	{
		::PrintConsole("[EXCEPTION] createDSCom \n" ) ;
		return false ;
	}
	

	return true ;
}



/* =====================================================================================
releaseDSCom() : 해제
===================================================================================== */
#ifdef _METEL_LOGIN_200608_
#include "..\MapManager\MapManager.h"
extern _j_AccMAPManager *		g_pAccMapManager;	// AccMap Manager
extern std::map<tstring /*tstrUserAccount*/, CUser* /*pUser*/> g_mapBillingUser;
extern CRITICAL_SECTION g_csBillingUser;
#endif //_METEL_LOGIN_200608_

extern _j_ServerInfo		g_ServerInfo;			// 서버정보

bool _h_DS_Com::Send(  int size, char * buf )
{
	if( buf == NULL ) return false ;
	else {
#ifdef _METEL_LOGIN_200608_
#ifdef _METEL_LOGIN_NOT_USE_CALL_
#else
		if(g_ServerInfo.get_billing_use()) {
			CMSG_PROTOTYPE* p = (CMSG_PROTOTYPE*)buf;
			//////////////////////////////////////////////////////////////////////////
			//Logout("send: MSG_NO=%d, size=%d", p->cMessage, size);
			//////////////////////////////////////////////////////////////////////////
			
			if(p->cMessage == CMSG_NO_USER_LOGOUT_DS_GS) {
				CMSG_USER_LOGOUT_DS_GS* p_ = (CMSG_USER_LOGOUT_DS_GS*)buf;

				tstring tstr = p_->cNDAccount;
				EnterCriticalSection(&g_csBillingUser);
				CUser* pUser = g_mapBillingUser[tstr];
				LeaveCriticalSection(&g_csBillingUser);
				if(pUser != NULL) {

					BillingLogout(pUser->cNDAccount, pUser->MetelBillingInfo.iLoginSN, -1);
					Logout(_T("BillingLogout(%s, %d), CMSG_NO_USER_LOGOUT_DS_GS"), pUser->cNDAccount, pUser->MetelBillingInfo.iLoginSN);
					pUser->bBillingLogin = FALSE;
					EnterCriticalSection(&g_csBillingUser);
					g_mapBillingUser.erase(tstr);
					LeaveCriticalSection(&g_csBillingUser);
				}
			}
		}
#endif
#endif
	}


	u_short * usLen = (u_short*)&buf[0] ;

	if( size <= 2 || *usLen <= 2 || *usLen != size )
	{
		printf( "[SEND ERR] Send size = %d \n", size ) ;
		printf( "  -> in_packet_len : %u \n", *usLen ) ;
		printf( "  -> msg num : %u \n", static_cast<u_char>(buf[2]) ) ;
		::PrintConsole( "[SEND ERR] Send size = %d \n", size ) ;
		::PrintConsole( "  -> in_packet_len : %u \n", *usLen ) ;
		::PrintConsole( "  -> msg num : %u \n", static_cast<u_char>(buf[2]) ) ;
		return false ;
	}
	//if( false == get_tcp_state() )
	//{
	//	printf( "InnerCom::Tcp Not Connected\n" ) ;
	//	return true ;
	//}

	try
	{
		COV_UNIT * pUnit ;

		m_sendSlot.Get_SendUnit( &pUnit, size ) ;
		pUnit->SetOperation( mode_send_complete ) ;
		pUnit->SetSendData( buf, size ) ;

		if( 0 != pUnit->SendPacket( m_dssock, true ) )
		{
			return false ;		
		}
		m_dwTotalSendBytes += size;
		return true ;
	}
	catch (...) 
	{
		::PrintConsole("[EXCPTION] _h_DS_Com::Send \n") ;
	}
	return false ;
}


void _h_DS_Com::ShowState( int iServer )
{
	printf( "\n####  %d DS Com Status ###\n", iServer );
	printf( "  - Last Proc : Msg %u, Size %u\n", get_last_msg(), get_last_size() );
	printf( "  - Send : %u\n", m_dwTotalSendBytes );

	g_pSystemLog->LogFileWrite( "\n####  %d DS Com Status ###\n", iServer );
	g_pSystemLog->LogFileWrite( "  - Last Proc : Msg %u, Size %u\n", get_last_msg(), get_last_size() );
	g_pSystemLog->LogFileWrite( "  - Send %u\n", m_dwTotalSendBytes );
	m_RecvOV.ShowState();
}

void _h_DS_Com::ConnectCom( SOCKET gsSock )
{
	m_dssock = gsSock;
	m_RecvOV.m_socket = gsSock;	
}

/* =====================================================================================
releaseDSCom() : 해제
===================================================================================== */
void _h_DS_Com::DisConnectCom()
{
	try
	{
		m_RecvOV.reset() ;
		if( m_dssock ) {
			closesocket( m_dssock ) ;
			m_dssock = NULL ;
		}

	}
	catch (...) 
	{
		::PrintConsole("[EXCEPTION]  _h_DS_Com::releaseDSCom \n") ;
	}	

}


//#################################################################################
//
//	CNDIOCPObejct 구현부
//
//###################################################################### LHS Added....
bool CNDIOCPObejct::Create( int n )
{
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, n);

	return (m_hIOCP != NULL) ;
}

bool CNDIOCPObejct::AssociateSocket( SOCKET s, DWORD CompKey )
{

	return (CreateIoCompletionPort((HANDLE)s, m_hIOCP, CompKey, 0) == m_hIOCP);	
}

int CNDIOCPObejct::GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds)
{
	return (GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, pCompKey, ppo, dwMilliseconds));
}


void CNDIOCPObejct::PostStatus( DWORD CompKey, DWORD dwNumBytes, WSAOVERLAPPED * pOV )
{
	::PostQueuedCompletionStatus( m_hIOCP, dwNumBytes, CompKey, pOV ) ;
}

void CNDIOCPObejct::PostCloseWorkThread()
{
	ZeroMemory( &m_OVUnit, sizeof(m_OVUnit) ) ;

	m_OVUnit.m_iMode = iomode_end ;

	PostQueuedCompletionStatus( m_hIOCP, 0, 0, (LPOVERLAPPED)&m_OVUnit ) ;
}


//////////////////////////////////////////////////////////////////////////
/*/////////////////////////////////////////////////////////////////////////
CRecvBuffer
/////////////////////////////////////////////////////////////////////////*/
void CRecvBuffer::NoticeRecv( int RecvLength )
{
	m_iCurEndPos += RecvLength;
	if( m_iCurEndPos > MAX_OF_RECV_BUFFER )
	{
		throw "[CRecvBuffer] Exception : Buffer Overflow.";
	}
}

bool CRecvBuffer::GetPacket( __mid_packet_unit& unit )
{
	// true 일 때는 parsing 을 계속하고, 
	// false 일 때는 Pos 변수들과 버퍼를 정돈하고 리턴한다.
	bool ret = false;

	u_short packet_len = 0;

	// 패킷 길이( u_short )를 온전히 읽을 수 있는지 확인하고, 
	if( m_iCurStartPos < m_iCurEndPos - (int)sizeof(u_short) )
	{
		CopyMemory( &packet_len, &m_cbRecvBuffer[m_iCurStartPos], sizeof(u_short) );
		if( packet_len > MAX_OF_RECV_BUFFER )
		{
			throw "[CRecvBuffer] Exception : Packet Length Overflow" ;
		}

		// 패킷이 완전한지 확인하고, 리턴한다. 
		if( m_iCurEndPos - m_iCurStartPos >= packet_len + (int)sizeof(u_short) )
		{
			unit.length = packet_len ;
			unit.packet = &m_cbRecvBuffer[m_iCurStartPos+sizeof(u_short)];

			m_iCurStartPos += packet_len;
			m_iCurStartPos += sizeof(u_short);

			ret = true;
		}
		else
		{
			// 읽기 버퍼를 버퍼의 맨앞으로 옮긴다.
			if( 0 != m_iCurStartPos )
			{
				MoveMemory( &m_cbRecvBuffer[0], &m_cbRecvBuffer[m_iCurStartPos], m_iCurEndPos-m_iCurStartPos );
				m_iCurEndPos -= m_iCurStartPos;
				m_iCurStartPos = 0;			
				ZeroMemory( &m_cbRecvBuffer[m_iCurEndPos], MAX_OF_RECV_BUFFER-m_iCurEndPos );
			}

			ret = false;
		}
	}
	else
		ret = false;


	return ret;
}


/*/////////////////////////////////////////////////////////////////////////
CConnectionObject
/////////////////////////////////////////////////////////////////////////*/
void CConnectionObject::Connect( char * ip, unsigned short port )
{
	strncpy( m_ip, ip, 16 );
	m_port = port;

	m_sockConnect = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( INVALID_SOCKET == m_sockConnect )
	{
		m_iConState = FALSE;
	}
	else
	{
		// Connect Try.
		SOCKADDR_IN		TargetAddr;
		TargetAddr.sin_family = AF_INET;
		TargetAddr.sin_port = htons( m_port );
		TargetAddr.sin_addr.s_addr = inet_addr( m_ip );

		if( SOCKET_ERROR == connect( m_sockConnect, (SOCKADDR*)&TargetAddr, sizeof( TargetAddr ) ) )
		{
			// Connect Fail
			m_iLastError = WSAGetLastError();
			closesocket( m_sockConnect );
			m_iConState = FALSE;
			m_sockConnect = INVALID_SOCKET;

#ifdef _DEBUG
			char errmsg[128];
			sprintf( errmsg, "connect fail : %d\n", m_iLastError );
			OutputDebugString( errmsg );
#endif
		}

		// Connect Success
		m_iConState = TRUE;
	}
}


void CConnectionObject::SendRequest( void * data, u_short length )
{
	if( m_iConState )
	{
		CIO_UNIT * pUnit = m_refSendBuffer.GetSendBuffer();
		pUnit->SetData( data, length );
		if( SOCKET_ERROR == WSASend( m_sockConnect, pUnit->GetWSABuf(), 1, 
			pUnit->GetPLength(), 0, static_cast<LPWSAOVERLAPPED>(pUnit), NULL ) )
		{
			m_iLastError = WSAGetLastError();
			if( ERROR_IO_PENDING != m_iLastError )
			{
				Close();

				char errmsg[128];
				sprintf( errmsg, "Connection Object %d Close - Send Error %d\n", GetIndex(), m_iLastError );
				ShowMsg( errmsg );
			}
		}
	}
	else
	{
		char errmsg[128];
		sprintf( errmsg, "Connection Object %d - not connected", GetIndex() );
		ShowMsg( errmsg );
	}
}



BOOL CConnectionObject::RecvRequest()
{
	BOOL ret = TRUE;

	DWORD err = 0 ;
	DWORD len = 0 ;

	m_ovRecv.m_iMode = iomode_recv ;
	if( SOCKET_ERROR == WSARecv( GetSocket(), m_recvBuffer.GetWSABuf(), 1, &len, 
		&m_ovRecv.m_dwFlag, (LPWSAOVERLAPPED)&m_ovRecv, NULL ) )
	{
		err = ::WSAGetLastError() ;
		if( WSA_IO_PENDING != err )
			ret = FALSE;
	}

	return ret;
}


void CConnectionObject::Close()
{
	// Close...
	if( INVALID_SOCKET != m_sockConnect )
		closesocket( m_sockConnect );
	m_sockConnect = INVALID_SOCKET;
	m_iConState = FALSE;
}

/*	// UDPProcess.cpp 로 옮긴다.
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
			printf( " unit length = %d\n", unit.length );
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
*/ 
/*/////////////////////////////////////////////////////////////////////////
CConnectionManager
/////////////////////////////////////////////////////////////////////////*/
bool CConnectionManager::InitConnectionManager( int workmode, int bufsize, char * ip, unsigned short port )
{
	bool ret = false;

	try
	{
		if( FALSE == InitializeCriticalSectionAndSpinCount( &m_csLock, 0x80000100 ) )
			throw "(Send Buffer) Create Critical Section Fail";

		m_hInitEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ;

		m_pSendBuffer = new CSendBuffer( bufsize );
		if( !m_pSendBuffer )
		{
			ShowMsg( "[Connection Manager] Create SendBuffer failed" );
			return false;
		}

		if( false == m_IOCP.Create( 1 ) )		// Read 쓰레드는 하나만 쓸 것이다. 
		{
			ShowMsg( "[Connection Manager] IOCP Create failed" );
			return false;
		}

		m_iWorkMode = workmode;

		ResetEvent( m_hInitEvent );

		StartRecv();
		if( WAIT_TIMEOUT == WaitForSingleObject( m_hInitEvent, 5000 ) )
		{
			ret = false;
		}
		else
		{
			if( en_work_mode_gms == m_iWorkMode )
			{
				// ConnectAll( *pdsinfo );

				ret = true;
			}
			else if( en_work_mode_ds == m_iWorkMode )
			{
				CConnectionObject * pObj;

				for( int i = 0 ; i < bufsize ; ++i )
				{
					pObj = new CConnectionObject( *m_pSendBuffer );

					if( pObj )
						AddConObject( pObj );

					Sleep(5);
				}

				ResetEvent( m_hInitEvent );
				StartAccept( ip, port );
				if( WAIT_TIMEOUT == WaitForSingleObject( m_hInitEvent, 50000 ) )
				{
					ShowMsg( "[Connection Manager] StartAccept Timeout!" );
					ret = false;
				}
				else
					ret = true;
			}
			else
			{
				ShowMsg( "[Connection Manager] Work Mode is not defined\n" );
				ret = false;
			}
		}
	}
	catch( char * msg )
	{
		char errmsg[128];
		sprintf( errmsg, "[Connenction Manager] Exception : %s", msg );
		ShowMsg( errmsg );
	}
	catch( ... )
	{
		ShowMsg( "[Connenction Manager] Exception" );
	}

	return ret;
}

/*
void CConnectionManager::ConnectAll( CDSInformation& dsinfo )
{
	int i = 0;
	int count = dsinfo.GetCount();
	__ds_info * pInfo;
	CConnectionObject * pCon;
	while( i < count )
	{
		pInfo = dsinfo.GetDSInfo(i);

		pCon = new CConnectionObject( *m_pSendBuffer );
		if( pCon )
		{
			pCon->SetIndex( pInfo->m_index );
			pCon->Connect( pInfo->m_ip, pInfo->m_port );

			if( TRUE == pCon->GetState() )
			{
				if( m_IOCP.AssociateSocket( pCon->GetSocket(), (DWORD)pCon ) )
					pCon->RecvRequest();
				else
					printf( "[CConnectionManger] Fail Associate.\n" );
			}

			m_ConList.push_back( pCon );
		}
		else
		{
			throw "[CConnectionManger] new Connection Object Failed" ;
		}

		++i;
	}

	PrintState();
}
*/
void CConnectionManager::StartRecv()
{
	HANDLE hThread ;
	DWORD dw ;
	hThread = BEGINTHREADEX( NULL, 0, th_GMSRecvThread, this, 0, &dw ) ;
	if( hThread != NULL ) 
	{
		CloseHandle(hThread) ; 
		hThread = NULL ; 
	}
	else 
	{
		printf( "FAIL : Worker Thread Failed in SessionManager\n" ) ;
	}
}

void CConnectionManager::StartAccept( char * ip, unsigned short port )
{
	HANDLE hThread ;
	DWORD dw ;

	strncpy( m_AcceptIP, ip, 16 );
	m_AcceptPort = port;

	hThread = BEGINTHREADEX( NULL, 0, th_GMSAcceptThread, this, 0, &dw ) ;
	if( hThread != NULL ) 
	{
		CloseHandle(hThread) ; 
		hThread = NULL ; 
	}
	else 
	{
		printf( "FAIL : Worker Thread Failed in SessionManager\n" ) ;
	}
}

/*
bool CConnectionManager::RetryConnect_NotConnectedObject()
{
	bool ret = false;

	return ret;
}
*/

unsigned int __stdcall CConnectionManager::th_GMSRecvThread( LPVOID lp )
{
	CConnectionManager * pManager = (CConnectionManager*)lp;

	pManager->m_bRecvThreadState = true;

	if( en_work_mode_ds == pManager->m_iWorkMode )	// DS 에서 사용될 때.
	{
		pManager->DSRecv();
	}
	else //
	{
		//pManager->GMSRecv();
	}

	pManager->m_bRecvThreadState = false;

	_endthreadex(0);
	return 0U;
}


unsigned int __stdcall CConnectionManager::th_GMSAcceptThread( LPVOID lp )
{
	CConnectionManager * pManager = (CConnectionManager*)lp;

	pManager->m_bAcceptThreadState = true;

	pManager->DSAccept();

	pManager->m_bAcceptThreadState = false;

	_endthreadex(0);
	return 0U;
}

/* // UDPProcess.cpp 로 옮긴다.
void CConnectionManager::GMSRecv()
*/

/* UDPProcess.cpp 로 이동.
void CConnectionManager::DSRecv()
*/

void CConnectionManager::DSAccept()
{
	SOCKET		sUser;
	int         nZero = 0;
	SOCKADDR_IN sockAddr;
	LINGER      lingerStruct;		// SO_LINGER 옵션을 위한 변수
	int n = 0;
	int timeo = 1000;

	lingerStruct.l_onoff = 1;		
	lingerStruct.l_linger = 0;

	m_sockAccept = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED );
	if( m_sockAccept == INVALID_SOCKET )
	{
		return;
	}

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons( m_AcceptPort );
	sockAddr.sin_addr.s_addr = inet_addr( m_AcceptIP ); //   htonl( INADDR_ANY ) ;

	if( bind( m_sockAccept, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR_IN) ) == SOCKET_ERROR)
	{
		printf( "Bind Fail : %d\n", WSAGetLastError() );
		return;
	}


	if( listen( m_sockAccept, 10) != 0)		// 백로그 값을 올리는게 날지....
	{
		printf( "Listen Fail : %d\n", WSAGetLastError() );
		return;
	}

	printf( "[GMS Tcp Bind : %s:%u\n", m_AcceptIP, m_AcceptPort );


	nZero = TRUE ;
	::setsockopt( m_sockAccept, SOL_SOCKET, SO_REUSEADDR, ( char* )&nZero, sizeof( nZero ) );
	timeo = 4000 ;
	setsockopt( m_sockAccept, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeo, sizeof(int) ) ;
	timeo = 4000 ;	// 타임 아웃 10초. 
	setsockopt( m_sockAccept, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeo, sizeof(int) ) ;

	SetEvent( m_hInitEvent );

	CConnectionObject * pCon = NULL;
	SOCKADDR_IN useradr;
	int size = sizeof( useradr ) ;

	// 실제로 Accept 한다. 
	while( 1 )
	{
		size = sizeof( useradr );

		sUser = WSAAccept( m_sockAccept, (SOCKADDR*)&useradr, &size, NULL, 0 ) ;// AcceptCallBack, 0 ) ;
		if( sUser != INVALID_SOCKET )
		{
			try
			{
				pCon = GetConObject();
				if( pCon == NULL )
				{
					closesocket( sUser );

					PrintConsole( "[TCP GMS] No Connection Object!\n" );
				}
				else
				{
					PrintConsole( "[TCP GMS] Success \n" );

					pCon->SetSocket( sUser );
					pCon->SetState( TRUE );

					if( false == m_IOCP.AssociateSocket( pCon->GetSocket(), (DWORD)(pCon) ) )
					{
						PrintConsole( "[TCP GMS] Associate fail\n" );
						pCon->Close();

						AddConObject( pCon );
					}

					if( false == pCon->RecvRequest() )
					{
						PrintConsole( "[TCP GMS] RecvRequest fail\n" );
						pCon->Close();

						AddConObject( pCon );
					}
				}
			}
			catch(...)
			{
			}
		}
		else 
		{
			int err = WSAGetLastError();
			continue ;
		}
	}

	printf( "[EXIT] GMS Accept Thread Return\n " ) ;

	closesocket( m_sockAccept ) ;
	m_sockAccept = INVALID_SOCKET ;
}

//////////////////////////////////////////////////////////////////////////

void CConnectionManager::SendTo( int index, void * data, u_short length )
{
	CConnectionObject * pCon = FindConnection( index );
	if( pCon )
	{
		pCon->SendRequest( data, length );
	}
	else
	{
		char errmsg[128];
		sprintf( errmsg, "Con : Index %d not found\n", index ) ;
		ShowMsg( errmsg );
	}
}

CConnectionObject * CConnectionManager::FindConnection( int index )
{
	CConnectionObject * pCon = NULL;

	EnterCriticalSection( &m_csLock );
	
	list<CConnectionObject *>::iterator itr = m_ConList.begin();

	while( itr != m_ConList.end() )
	{
		if( index == (*itr)->GetIndex() )
			break;
		++itr;
	}

	if( itr != m_ConList.end() )
		pCon = *itr;

	LeaveCriticalSection( &m_csLock );

	return pCon;
	
}

//////////////////////////////////////////////////////////////////////////
void CConnectionManager::PrintState()
{
#ifdef _CONSOLE
	PrintStateToConsole();
#else
	PrintStateToMSGWIN();
#endif
}

void CConnectionManager::PrintStateToConsole()
{
	CConnectionObject * pCon;

	EnterCriticalSection( &m_csLock );

	list<CConnectionObject*>::iterator itr = m_ConList.begin();

	printf( " ======== DS Connection Information =========\n" );

	while( itr != m_ConList.end() )
	{
		pCon = *itr;

		if( TRUE == pCon->GetState() )
			printf( "  DS(%d) - IP(%s) Port(%u) : Connected\n", pCon->GetIndex(), pCon->GetIP(), pCon->GetPort() );
		else
			printf( "  DS(%d) - IP(%s) Port(%u) : Not Connected(err %d)\n", pCon->GetIndex(), pCon->GetIP(), pCon->GetPort(), pCon->GetError() );

		itr++;
	}

	LeaveCriticalSection( &m_csLock );

	printf( "=============================================\n" );
}

void CConnectionManager::PrintStateToMSGWIN()
{
	char msg[2048] = {0,};
	char temp[128] = {0,};

	CConnectionObject * pCon;

	EnterCriticalSection( &m_csLock );

	list<CConnectionObject*>::iterator itr = m_ConList.begin();

	while( itr != m_ConList.end() )
	{
		pCon = *itr;

		if( TRUE == pCon->GetState() )
			sprintf( temp, "  Con(%d) - IP(%s) Port(%u) : Connected\r", pCon->GetIndex(), pCon->GetIP(), pCon->GetPort() );
		else
			sprintf( temp, "  Con(%d) - IP(%s) Port(%u) : Not Connected(err %d)\r", pCon->GetIndex(), pCon->GetIP(), pCon->GetPort(), pCon->GetError() );

		strcat( msg, temp );

		itr++;
	}

	LeaveCriticalSection( &m_csLock );

	MessageBox( NULL, msg, "Connection State", MB_OK );
}