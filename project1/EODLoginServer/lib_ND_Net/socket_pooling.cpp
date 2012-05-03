#include "stdafx.h"
#include "socket_pooling.h"


void PrintConsole( const char *format,... ) ;

namespace ND_Net {

LPFN_ACCEPTEX				CServerSocket::pAcceptEx = NULL ;
LPFN_GETACCEPTEXSOCKADDRS	CServerSocket::pGetAcceptExSockaddrs = NULL ;
LPFN_DISCONNECTEX			CServerSocket::pDisconnectEx = NULL ;
LPFN_TRANSMITFILE			CServerSocket::pTransmitFile = NULL ;



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

/*
int COV_UNIT::AddSendData( char * buf, int length )
{
#ifdef _DEBUG
	if( length <= 0 )
		::PrintConsole( " ##### SOCKET :: Add : Length Error msg(%u) #####\n", static_cast<u_char>(buf[2]) ) ;
#endif
	CopyMemory( m_wsabuf.buf + m_wsabuf.len, buf, length ) ;
	m_wsabuf.len += length ;
	
#ifdef _DEBUG
	if( m_wsabuf.len > m_iCapacity )
	{
		::PrintConsole( " ##### SOCKET :: Packsize overflow msg(%u) ####\n", static_cast<unsigned char>(buf[2]) ) ;		
	}
	
	
	//if( m_iCapacity != _BIG_UNIT_SIZE_ )
	//{
	//	printf( " ##### SOCKET :: This Unit cannot AddSendData msg(%u) #####\n", 
	//		static_cast<unsigned char>(buf[2]) ) ;
	//}
	
#endif
	
	return m_wsabuf.len ;
}
*/

int COV_UNIT::SendPacket( const SOCKET sock )
{
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
#ifdef _DEBUG
			::PrintConsole( " #### SOCKET :: SendError(%d) ####\n", err ) ;
#endif 
			return err ;
		}
	}
	
	return 0 ;
}


/*
bool CBaseUser::Parsing( DWORD recvbytes )
{
	throw ;
	//m_pNetIO->reset() ;

	return true ;
}
*/


//#################################################################################
//
//	CRecv_Overlapped : 오버랩드 구조체. 
//
//###################################################################### LHS Added....
/*
void CSocketBuffer::Init( CBaseUser * pUser )
{
	Internal			= InternalHigh	= 0;	
	Offset				= OffsetHigh	= 0;
	hEvent				= NULL;
	memset( m_buf, 0, _RECV_BUF_SIZE_ ) ;
	m_wsabuf.buf = m_buf ;
	m_wsabuf.len = 0 ;
	pRealBuf = m_buf ;		// Add...
	RemainBytes = 0 ;		// Add...
	m_dwFlag = 0 ;
	m_iMode = 0 ;
	m_bAccepted = false ;

	
	//m_lTotalSendByte = 0 ;
	//m_lStartTime = 0 ;
	//m_lEndTime = 0 ;
	

	m_pUser = pUser ;	
}
*/

bool CSocketBuffer::InitSocket()
{
	socket = ::WSASocket( AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED ) ;
	if( socket == INVALID_SOCKET )
	{
		::PrintConsole( "[error] ::WSASocket -> socket == INVALID_SOCKET code: %d\n", WSAGetLastError() ) ;
		return false ;
	}
	return true ;
}

bool CSocketBuffer::ReleaseSocket()
{
	if( closesocket( socket ) != 0 )
	{
		::PrintConsole( "[error] closesocket error code:%d\n", WSAGetLastError() ) ;
		return false ;
	}
	socket = INVALID_SOCKET ;
	return true ;
}


void CSocketBuffer::move( int remain ) 
{
	try {
		RemainBytes = remain ;
		memmove( m_buf, pRealBuf, RemainBytes ) ;
		pRealBuf = m_buf ;
	}
	catch ( ... ) {		
		::PrintConsole( "[EXCEPTION] Recv Overlapped::move\n" ) ;
		//reset() ;
	}
}

// Send Request

// Recv Request
bool CSocketBuffer::RecvRequest()
{
	int err ;
	
	DWORD len = 0 ;
	__try {
		m_wsabuf.len = _RECV_BUF_SIZE_ - RemainBytes ;
		m_wsabuf.buf = m_buf + RemainBytes ;
		//memset( m_wsabuf.buf, 0, m_wsabuf.len ) ;
		m_iMode = mode_recv ;
	}
	__except ( EXCEPTION_EXECUTE_HANDLER ) {
		//release() ;

		::PrintConsole( "[EXCEPTION] Recv Request \n" ) ;
		return false ;
	}
	if( SOCKET_ERROR == WSARecv( socket, &m_wsabuf, 1, &len, &m_dwFlag, (LPWSAOVERLAPPED)this, NULL ) )
	{
		err = WSAGetLastError() ;
		if( err == WSA_IO_PENDING )
		{
			return true ;
		}
		else
		{
			::PrintConsole( "Recv Trigger Error Code( err: %d, sock:%d )\n", err, socket ) ;
			return false ;
		}
	}
	
	return true ;
}

bool CSocketBuffer::IsConnected()
{
	if( INVALID_SOCKET != socket )
	{
		int seconds ;
		int bytes = sizeof(seconds) ;

		if( NO_ERROR == ::getsockopt( socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&seconds, (PINT)&bytes ) )
		{
			return true ;
		}		
	}
	
	return false ;
}



COV_UNIT * CSocketBuffer::ReserveBuffer( int size )
{
	COV_UNIT * p = NULL ;
	m_server->Get_SendUnit( &p, size ) ;
	return p ;
}


int CSocketBuffer::SendRequest( COV_UNIT * pOV )
{
	pOV->SetOperation( mode_send_complete ) ;

//	::InterlockedExchangeAdd( &m_lTotalSendByte, pOV->m_wsabuf.len ) ;
	
	if( SOCKET_ERROR == WSASend( socket, &pOV->m_wsabuf, 1, &pOV->m_wsabuf.len, 0, (LPWSAOVERLAPPED)pOV, NULL ) ) 
	{
		int err = WSAGetLastError() ;
		if( err != WSA_IO_PENDING )
		{
#ifdef _DEBUG
			::PrintConsole( " #### SOCKET :: SendError(%d) ####\n", err ) ;
#endif 
			return err ;
		}
	}

	return 0 ;
}


void CSocketBuffer::Shutdown( int how )
{
	::shutdown( socket, how ) ;
}


int CSocketBuffer::SendRequest( char * buf, int size )

{
	COV_UNIT * pOV ;
	m_server->Get_SendUnit( &pOV, size ) ;

	pOV->SetSendData( buf, size ) ;

	return SendRequest( pOV ) ;
}

int CSocketBuffer::SendRequest( int size, char * buf )
{	
	COV_UNIT * pOV ;
	m_server->Get_SendUnit( &pOV, size ) ;
	
	pOV->SetSendData( buf, size ) ;
	
	return SendRequest( pOV ) ;
}


/*
float CSocketBuffer::net_stat()
{
	long lTime = m_lEndTime - m_lStartTime ;

	if( lTime > 0 )
	{
		long total = m_lTotalSendByte ;
		return static_cast<float>(total) / static_cast<float>(lTime) ;
	}
	else
	{
		return 0.0f ;
	}
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
		::PrintConsole( "OV Slot :: Buf Size Abnormal\n" ) ;
		return false ;
	}
	m_iTotalSize = _MEGA_ * megasize ;
	m_nOVSlot = slotcount ;

	m_pOVSlot = new COV_UNIT[m_nOVSlot] ;
	
	m_pSendBuf = new char[m_iTotalSize] ;		// 40 메가 예약.
	if( !m_pSendBuf || !m_pOVSlot )
		return false ;
	
	return true ;
}


void CSendOverlappedSlot::Get_SendUnit( COV_UNIT ** ppSender, int reqSize )
{
#ifdef _DEBUG
	if( reqSize <= 0 ) {
		::PrintConsole( " #### Slot Size Error ####\n" ) ;		
	}
#endif 
	
	lock() ;

	try{
		*ppSender = &m_pOVSlot[m_iGetSlot] ;
		
		if( m_iPosition + reqSize > m_iTotalSize )
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
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
	}
	
	
	unlock() ;
}

void CSendOverlappedSlot::Get_BigUnit( COV_UNIT ** ppSender )
{
	lock() ;

	try{
		*ppSender = &m_pOVSlot[m_iGetSlot] ;
		
		if( m_iPosition + _BIG_UNIT_SIZE_ > m_iTotalSize )
		{
#ifdef _DEBUG
			//print_state() ;
#endif
			m_iPosition = 0 ;
		}
		
#ifdef _DEBUG
		(*ppSender)->Initialize( &m_pSendBuf[m_iPosition], _BIG_UNIT_SIZE_ ) ;
#else
		(*ppSender)->Initialize( &m_pSendBuf[m_iPosition] ) ;
#endif
		
		m_iPosition += _BIG_UNIT_SIZE_ ;
		
		m_iGetSlot = ( m_iGetSlot + 1 ) % m_nOVSlot ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
	}
	
	
	unlock() ;
}


#ifdef _DEBUG 

void CSendOverlappedSlot::print_state()
{
	::PrintConsole( "Pos = %8d, Slot = %7d\n", m_iPosition, m_iGetSlot ) ;
}

#endif




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
	try{
		RemainBytes = remain ;
		memmove( m_buf, pRealBuf, RemainBytes ) ;
		pRealBuf = m_buf ;
	}
	catch(...){
		
		printf( "[EXCEPTION] Recv Overlapped::move\n" ) ;
		
		reset() ;
	}
}

// Send Request

// Recv Request
bool CDSRecv_Overlapped::RecvPacket( const SOCKET sock )
{
	int err ;
	
	DWORD len = 0 ;
	try{
		m_wsabuf.len = m_bufSize - RemainBytes ;
		m_wsabuf.buf = m_buf + RemainBytes ;
		memset( m_wsabuf.buf, 0, m_wsabuf.len ) ;
		m_iMode = mode_recv ;
	}
	catch(...){
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




	
//#################################################################################
//
//	_h_CompletionPort 구현부
//
//###################################################################### LHS Added....
bool _h_CompletionPort::Create( int nConcurrentWorker )
{
	m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, nConcurrentWorker ) ;
	
	return (m_hIOCP != NULL) ;
}

bool _h_CompletionPort::AssociateSocket( CSocketBuffer * pB )
	{
	return (CreateIoCompletionPort(reinterpret_cast<HANDLE>(pB->socket), m_hIOCP, reinterpret_cast<DWORD>(pB), 0) == m_hIOCP);	
}

bool _h_CompletionPort::AssociateSocket( SOCKET s )
{
	return (CreateIoCompletionPort(reinterpret_cast<HANDLE>(s), m_hIOCP, (DWORD)s, 0) == m_hIOCP);	
}

int _h_CompletionPort::GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds)
{
	return (GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, pCompKey, ppo, dwMilliseconds));
}


void _h_CompletionPort::PostStatus( DWORD CompKey, DWORD dwNumBytes, WSAOVERLAPPED * pOV )
{
	::PostQueuedCompletionStatus( m_hIOCP, dwNumBytes, CompKey, pOV ) ;
}


void _h_CompletionPort::PostCloseWorkThread()
{
	ZeroMemory( &m_OVUnit, sizeof(m_OVUnit) ) ;
	
	m_OVUnit.m_iMode = mode_end ;
	
	PostStatus( 0, 0, (LPOVERLAPPED)&m_OVUnit ) ;
}



/*#################################################################################
//
//  CServerSocket : 서버의 소켓 관리자.
//
//###############################################################################*/
BOOL CServerSocket::CreateServerSocket( int nWorker, char * listenip, u_short listenport,
									   int nSendSlotCount,
									   int nBufMegaSize,
									   int nSockBufPool, 
									   int backlog /* = 10 */, 
									   int nAcceptPostMax /* = 20  */, 
									   int nAcceptPostMin /* = 10 */ )
{
	int i = 0 ;

	if( !InitializeCriticalSectionAndSpinCount( &m_csPool, 0x80000000 | 2000 ) )
	{
		::PrintConsole( "Critical Section Error\n" ) ;
		return FALSE ;
	}

	m_nMaxPendingAccept = nAcceptPostMax ;
	m_nMinPendingAccept = nAcceptPostMin ;
	m_nCurPendingAccept = 0 ;
	
	if( NULL == ( m_hAcceptPostEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ) )
	{
		::PrintConsole( "Create Event Failed\n" ) ;
		return FALSE ;
	}

	if( NULL == ( m_hNoAcceptWaitEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ) )
	{
		::PrintConsole( "Create Event Failed\n" ) ;
		return FALSE ;
	}

	if( NULL == ( m_hShutDownEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ) )
	{
		::PrintConsole( "Create Event Failed\n" ) ;
		return FALSE ;
	}



	m_sizeofaddr = sizeof( SOCKADDR_IN ) + 16 ;

	m_listensocket = ::WSASocket( AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED ) ;
	
	if( m_listensocket == INVALID_SOCKET )
	{
		::PrintConsole( "WSASocket Error : Code %d\n", ::WSAGetLastError() ) ;
		return FALSE ;
	}

	m_addr.sin_port = htons( listenport ) ;
	m_addr.sin_addr.s_addr = inet_addr( listenip ) ;
	m_addr.sin_family = AF_INET ;

	if( SOCKET_ERROR == ::bind( m_listensocket, reinterpret_cast<struct sockaddr *>(&m_addr), sizeof(SOCKADDR_IN) ) )
	{
		::PrintConsole( "Bind Error : Code %d\n", ::WSAGetLastError() ) ;
		return FALSE ;
	}

	if( SOCKET_ERROR == ::listen( m_listensocket, 10 ) )
	{
		::PrintConsole( "Listen Error : Code %d\n", ::WSAGetLastError() ) ;
		return FALSE ;
	}

	if( !LoadExtensionFunctions() )
	{
		return FALSE ;
	}
	

	if( false == m_sendSlot.CreateOVSlot( nSendSlotCount, nBufMegaSize ) )
	{
		::PrintConsole( "SendSlot Create Failed\n" ) ;
		return FALSE ;
	}

	// 리슨 소켓을 iocp 에 묶는다.
	if( !m_iocp.Create( nWorker ) )
	{
		return FALSE ;
	}


	m_nMaxPoolCount = nSockBufPool ;
	
	
	CSocketBuffer * pBuf = NULL ;
		
	for( i = 0 ; i < nSockBufPool ; i++ )
	{
		pBuf = new CSocketBuffer(this) ;
		if( pBuf ){
			if( pBuf->InitSocket() )
			{
				if( m_iocp.AssociateSocket( pBuf ) )
				{
					pool_lock() ;
					m_UsingPool.push( pBuf ) ;
					pool_unlock() ;					
				}
				else
				{
					int iError = WSAGetLastError() ;
					::PrintConsole( " m_iocp.AssociateSocket( pBuf ) == false (err_code :%d)\n", iError ) ;
					return FALSE ;
				}
			}
			else
			{
				::PrintConsole( " init socket error \n" ) ;
				return FALSE ;
			}
			
		}
		
		else
			return FALSE ;
	}



	if( !m_iocp.AssociateSocket( m_listensocket ) )
	{
		return FALSE ;
	}

	m_hAcceptPostEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ;
	if( !m_hAcceptPostEvent )
	{
		return FALSE ;
	}

	unsigned int threadid ;
	HANDLE handle ;

	m_nWorkerThread = nWorker ;

	for( i = 0 ; i < nWorker ; i++ )
	{
		handle = (HANDLE)::_beginthreadex( 0, 0, WorkerThread, (void*)this, 0, &threadid ) ;
		if( handle == INVALID_HANDLE_VALUE )
		{
			return FALSE ;
		}
	}

	// 자체 스레드 시작.
	Start() ;

	return TRUE ;
}


void CServerSocket::AcceptRequest()
{
	SetEvent( m_hAcceptPostEvent ) ;
}

void CServerSocket::Accept()
{
	CSocketBuffer * pBuffer = NULL ;
	
	pool_lock() ;
	try{
		if( m_UsingPool.empty() == false )
		{
			pBuffer = m_UsingPool.front() ; 
			m_UsingPool.pop() ;
		}
		
	}
	catch (...) {
		::PrintConsole("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
	}
	pool_unlock() ;

	if( pBuffer )
	{
		pBuffer->release() ;
		//pBuffer->InitSocket() ;
		pBuffer->SetMode( mode_accept_complete ) ;

		int nEstablishedSeconds = 0;		
		int bytes = sizeof(nEstablishedSeconds);		
		int err = ::getsockopt(pBuffer->socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&nEstablishedSeconds, &bytes);
		if (nEstablishedSeconds == 0xffffffff)
		{
		}		
		else			
		{
			// nEstablishedSeconds --> 커넥션 이루어진 때로부터 현재까지 경과한 초단위 시간	
			::PrintConsole( "[ERROR] Already connected socket \n") ;
			return ;
		}

		if( FALSE == AcceptEx( pBuffer ) )
		{
			int err = WSAGetLastError() ;
			if( err != WSA_IO_PENDING )
			{
				::PrintConsole( "AcceptEx : Error %d\n", err ) ;
				return ;
			}
		}
		else
		{
			m_iocp.PostStatus( (DWORD)pBuffer, 0, static_cast<LPOVERLAPPED>(pBuffer) ) ;
		}

		::InterlockedIncrement( &m_nCurPendingAccept ) ;
		
	}
	else
	{
		if( en_server_type_version_server == get_server_type() )
			PrintConsole( "ps : pop failed\n" ) ;
		else
			PrintConsole( "ls : pop failed\n" ) ;
	}
}


bool CServerSocket::AcceptCompleted( CSocketBuffer * pBuffer, DWORD dwReceiveByte )
{

	bool bRet ;
	// Accept 가 완료되면.
	if( pBuffer && pBuffer->IsConnected() )
	{
		pool_lock() ;
		try{
			pBuffer->m_bAccepted = true ;
		}
		catch (...) {
			::PrintConsole("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
		}		
		pool_unlock() ;

		
		::InterlockedDecrement( &m_nCurPendingAccept ) ;
	
		//SetEvent( m_hAcceptPostEvent ) ;

		int nEstablishedSeconds = 0;		
		int bytes = sizeof(nEstablishedSeconds);		
		int err = ::getsockopt(pBuffer->socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&nEstablishedSeconds, &bytes);
		if (nEstablishedSeconds == 0xffffffff)
		{			
			//  현재 연결되지 않은 넘
			::PrintConsole( "[ERROR] Dissconnected socket \n") ;
			return false ;
		}		
		else			
		{
			// nEstablishedSeconds --> 커넥션 이루어진 때로부터 현재까지 경과한 초단위 시간			
		}
		

		if( SOCKET_ERROR == ::setsockopt( pBuffer->socket, SOL_SOCKET, 
			SO_UPDATE_ACCEPT_CONTEXT, (char*)&m_listensocket, sizeof(m_listensocket) ) ) 
		{
			::PrintConsole( "CServerSocket::AcceptCompleted setsockopt failed - Code %d\n", ::WSAGetLastError() ) ;
		}

		// 주소를 채우고, 
		//
		int sizeofLocal=0, sizeofRemote = 0 ;
		SOCKADDR * pLocal = NULL, * pRemote = NULL ;

		GetAcceptExSockaddrs( pBuffer->m_buf, 0, m_sizeofaddr, m_sizeofaddr, 
			&pLocal, &sizeofLocal, &pRemote, &sizeofRemote ) ;

		if( pRemote )
		{
			::CopyMemory( &pBuffer->addr, pRemote, sizeof(SOCKADDR) ) ;

			//printf( "Ip = %s\n", inet_ntoa( pBuffer->addr.sin_addr ) ) ;
		}


		// completion port 와 연결한다..
		/*
		if( m_iocp.AssociateSocket( pBuffer ) == false )
		{
			int iError = WSAGetLastError() ;
			::PrintConsole( " m_iocp.AssociateSocket( pBuf ) == false (err_code :%d)\n", iError ) ;			
		}
		*/

		//OnConnectionEstablished( pBuffer ) ;
		bRet = true ;
	
	}
	else
	{
		bRet = false ;
		
	}

	return bRet ;
}


void CServerSocket::DisconnectForReuse( CSocketBuffer * pBuffer )
{

	if( pBuffer == NULL )
		return ;

		// 
	pool_lock() ;
	try{
		if( pBuffer->m_bAccepted )
		{		
			
			COV_UNIT * pOV ;
			m_sendSlot.Get_SendUnit( &pOV, 1 ) ;
			
			pOV->SetOperation( mode_close_socket ) ;
			
			// pBuffer->SetMode( mode_close_socket ) ;
			
			//printf( "in DisconnetForReuse\n" ) ;
			if( pDisconnectEx )
			{
				if( !DisconnectEx( pBuffer, static_cast<LPOVERLAPPED>(pOV), TF_REUSE_SOCKET ) )
				{
					DWORD lastError = ::WSAGetLastError() ;
					if( ERROR_IO_PENDING != lastError )
					{
						::PrintConsole( "Error Disconnect : %d\n", lastError ) ;					
						// 이건 어떻게 하나..	
					}
				}
				else
				{
					m_iocp.PostStatus( pBuffer->socket, 0, static_cast<LPOVERLAPPED>(pOV) ) ;
				}
			}
			else
			{
				if( !TransmitFile( pBuffer, static_cast<LPOVERLAPPED>(pOV) ) )
				{
					DWORD lastError = ::WSAGetLastError() ;
					if( ERROR_IO_PENDING != lastError )
					{
						::PrintConsole( "Error Disconnect : %d\n", lastError ) ;										
						// 이건 어떻게 하나..	
						
					}
				}
				else
				{
					m_iocp.PostStatus( pBuffer->socket, 0, static_cast<LPOVERLAPPED>(pOV) ) ;
				}
			}
			
		}
		pBuffer->m_bAccepted = false ;
	}
	catch (...) {
		::PrintConsole("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
	}	
	pool_unlock() ;
	
}


BOOL CServerSocket::LoadExtensionFunctions()
{
	if( !LoadAcceptEx( m_listensocket ) )
	{
		::PrintConsole( "Load AcceptEx Failed\n" ) ;
		return FALSE ;
	}

	if( !LoadDisconnectEx( m_listensocket ) )
	{
		if( !LoadTransmitFile( m_listensocket ) )
		{
			::PrintConsole( "Load DisconnectEx Failed\n" ) ;
			return FALSE ;
		}
		else
		{
			::PrintConsole( "DisconnectEx Not Found!! => TransmitFile Loaded\n" ) ;
		}
	}

	if( !LoadGetAcceptExSockaddrs( m_listensocket ) )
	{
		::PrintConsole( "Load GetAcceptExSockaddrs Failed\n" ) ;
		return FALSE ;
	}


	return TRUE ;
}


BOOL CServerSocket::LoadAcceptEx( SOCKET s )
{
	if (pAcceptEx)
	{
		return true;
	}
	
	GUID guidAcceptEx = WSAID_ACCEPTEX;
	
	//lint -e{741} Unusual pointer cast (function qualification)
	return LoadExtensionFunction(s, guidAcceptEx, (void**)&pAcceptEx);

}

BOOL CServerSocket::LoadDisconnectEx( SOCKET s )
{
	if (pDisconnectEx)
	{
		return true;
	}
	
	GUID guidDisconnectEx = WSAID_DISCONNECTEX;
	
	//lint -e{741} Unusual pointer cast (function qualification)
	return LoadExtensionFunction(s, guidDisconnectEx, (void**)&pDisconnectEx);
}


BOOL CServerSocket::LoadGetAcceptExSockaddrs( SOCKET s ) 
{
	if (pGetAcceptExSockaddrs)
	{
		return true;
	}
	
	GUID guidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
	
	//lint -e{741} Unusual pointer cast (function qualification)
	return LoadExtensionFunction(s, guidGetAcceptExSockaddrs, (void**)&pGetAcceptExSockaddrs);
}


BOOL CServerSocket::LoadTransmitFile( SOCKET s )
{
	if (pTransmitFile)
	{
		return true;
	}
	
	GUID guidTransmitFile = WSAID_TRANSMITFILE;
	
	//lint -e{741} Unusual pointer cast (function qualification)
	return LoadExtensionFunction(s, guidTransmitFile, (void**)&pTransmitFile);

}


// Iocp의 Worker Thread
int CServerSocket::Worker( void )
{
	return 0 ;
}


int CServerSocket::Run()
{
	// Accept 실행
	long i = 0 ;
	long nDef = ( m_nMinPendingAccept + m_nMaxPendingAccept ) / 2 ;
	for( i = 0 ; i < nDef ; i++ )
	{
		Accept() ;
	}

	HANDLE WaitHandles[3] ;

	if( SOCKET_ERROR == WSAEventSelect( m_listensocket, m_hNoAcceptWaitEvent, FD_ACCEPT ) )
	{
		printf( "WSAEventSelect Error : %d\n", ::WSAGetLastError() ) ;
		printf( "CServerSocket Run Return\n" ) ;
		return 0 ;
	}

	WaitHandles[0] = m_hAcceptPostEvent ;		// Accept 신청이 들어오면.
	WaitHandles[1] = m_hNoAcceptWaitEvent ;		// 대기중인 Accept가 없을 때.
	WaitHandles[2] = m_hShutDownEvent ;			// 서버 닫기 요청이 들어오면.

	DWORD WaitResult ; 
	BOOL bContineServer = TRUE ;
	long temp ;

	while( bContineServer ) 
	{
		WaitResult = ::WaitForMultipleObjects( 3, WaitHandles, FALSE, 3000 ) ;
		
		switch( WaitResult ) {
		case WAIT_OBJECT_0:		// Accept 요청이 들어왔다. 비교하고 호출.
			//printf( "   ++ AcceptPostEvent ++  \n" ) ;
			ResetEvent( m_hAcceptPostEvent ) ;
			
			if( m_nCurPendingAccept < m_nMaxPendingAccept )
			{
				Accept() ;
			}
			else
			{
				printf( " cur accept pending (cur: %d, max: %d) \n", m_nCurPendingAccept, m_nMaxPendingAccept ) ;
			}
			break;

		case WAIT_OBJECT_0 + 1:	// 대기중인 Accept 가 없다. 이거 들어오면 무조건 10번씩 호출하자.
			
			ResetEvent( m_hNoAcceptWaitEvent ) ;


			/*
			if( m_nMaxPoolCount > (m_queuePool.size() + m_nCurPendingAccept + 100) )
			{
				CSocketBuffer * pBuf = NULL ;
				try{
					temp = m_nMaxPoolCount - m_queuePool.size() - m_nCurPendingAccept ;

					if( temp < 0 || temp > m_nMaxPoolCount )
					{
						::PrintConsole( "[EXCEPTION] if( temp < 0 || temp > m_nMaxPoolCount )  temp:%d \n", temp ) ;
						continue ;
					}

					printf( " new socketbuffer (count : %d) \n", temp ) ;

					for( int i = 0 ; i < temp && i < 100 ; i++ )
					{
						pBuf = new CSocketBuffer(*this) ;
						if( pBuf ){

							//pBuf->reset() ;
							pool_lock() ;
							try{
								m_queuePool.push( pBuf ) ;
							}
							catch (...) {
								::PrintConsole( "[EXCEPTION] %s, %d\n", __FILE__, __LINE__  ) ;
							}							
							pool_unlock() ;
							
							if( m_iocp.AssociateSocket( pBuf ) == false )
							{
								int iError = WSAGetLastError() ;
								printf( " m_iocp.AssociateSocket( pBuf ) == false (err_code :%d)\n", iError ) ;							
							}
						}
					}
					printf(" ** create socket buffer (count :%d) \n", i ) ;
				}
				catch (...) {					
					::PrintConsole( "[EXCEPTION] %s, %d\n", __FILE__, __LINE__  ) ;
				}		
			}
			*/


			
			if( m_nCurPendingAccept < m_nMinPendingAccept )
				temp = m_nMinPendingAccept - m_nCurPendingAccept ;
			//else if( m_nCurPendingAccept > m_nMaxPendingAccept )
			else
				temp = 0 ;
			//else
			//	temp = 10 ;

			if( temp < 0 || temp > m_nMaxPendingAccept )
			{
				::PrintConsole( "[EXCEPTION] if( temp < 0 || temp > m_nMaxPendingAccept )  temp:%d \n", temp ) ;
				continue ;
			}

			for( i = 0 ; i < temp && i < 100 ; i++ )
			{
				Accept() ;
			}
			break;

		case WAIT_OBJECT_0 + 2:	// ShutDown 시키자.
			ResetEvent( m_hShutDownEvent ) ;
			bContineServer = FALSE ;
			break ;

		case WAIT_TIMEOUT:
			//printf( "Timeout : Cur Pending = %u\n", m_nCurPendingAccept ) ;
			SetEvent( m_hNoAcceptWaitEvent ) ;
			break ;

		default:
			::PrintConsole( "Error : WaitForMultipleObjects Return %u\n", WaitResult ) ;
		}
	}

	::PrintConsole( "CServerSocket Run Thread Return\n" ) ;

	return 0 ;
}


void CServerSocket::Shutdown()
{
	int i = 0 ; 
	
	for( i = 0 ; i < m_nWorkerThread ; i++ )
	{
		m_iocp.PostCloseWorkThread() ;
	}

	SetEvent( m_hShutDownEvent ) ;
	
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
unsigned int __stdcall CServerSocket::WorkerThread( LPVOID lp )
{
	int result = 0 ;
	CServerSocket * pthis = (CServerSocket*)lp ;
	if( pthis )
	{
		try
		{
			result = pthis->Worker() ;
		}
		catch ( ... ) 
		{
			::PrintConsole( "[EXCEPTION]Worker\n" ) ;
		}
	}

	return result ;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
static BOOL LoadExtensionFunction( SOCKET s, GUID functionID, void **ppFunc )
{
	DWORD dwBytes = 0 ;
	
	bool ok = TRUE ;
	
	if (0 != WSAIoctl(
		s, 
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&functionID,
		sizeof(GUID),
		ppFunc,
		sizeof(void *),
		&dwBytes,
		0,
		0))
	{
		ok = FALSE ;
	}
	
	return ok ;
}


} // ND_Net