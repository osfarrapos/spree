#pragma once

#include <winsock2.h>
#include <mswsock.h>

#include <NDUtilClass.h>
#include <MY_QUEUE.H>


namespace	ns_iocp_mode
{
	enum
	{
		mode_recv = 0,
		mode_send_complete,
		mode_accept_complete, 
		mode_close_socket, 
		mode_end
	} ;
}



class COV_UNIT : public WSAOVERLAPPED	
{

public :

	WSABUF	m_wsabuf ;
	DWORD	m_dwFlag ;	
	int		m_iMode ;

	inline COV_UNIT( char * const buf = NULL )
	{
		Initialize( buf ) ;		
	}
	
	inline void AddSendData( char * const buf, const size_t size )
	{
		if( size == 0 )	return ;		
		memcpy( &m_wsabuf.buf[m_wsabuf.len], buf, size ) ;
		m_wsabuf.len = m_wsabuf.len + size ;
	}		
	
	inline void SetMode( const int iMode ) { m_iMode = iMode ; }
	inline const int GetMode() { return m_iMode ; }	
	
	inline void Initialize( char * const buf )
	{
		memset( this, 0, sizeof(COV_UNIT) ) ;
		m_wsabuf.buf = buf ;
	}
};

class CSendOVSlot
{
protected:
	const u_int	m_uiMaxSlot ;			// 슬롯 최대 갯수	
	u_int		m_uiCurSlotPos ;		// 슬롯 가져올 위치
	u_int		m_uiBufMaxSize ;		// 전송버퍼 최대 크기
	u_int		m_uiBufCurPos ;			// 전송버퍼 지금 위치

	COV_UNIT * m_pSendOVSlot ;			// 전송 슬롯
	char *		m_pSendBuf ;			// 공용 전송 버퍼

	CRITICAL_SECTION	m_cs ;

	inline lock() { EnterCriticalSection( &m_cs ) ; }
	inline unlock() { LeaveCriticalSection( &m_cs ) ; }
	
public :
	inline CSendOVSlot(const size_t slotSize, const size_t bufSize) : 	m_uiMaxSlot(slotSize), m_uiCurSlotPos(0), m_uiBufMaxSize(0), m_uiBufCurPos(0)
	{
#if (_WIN32_WINNT > 0x0403)
		if( FALSE == InitializeCriticalSectionAndSpinCount( &m_cs, (0x80000000)|2000 ) )
		{
			throw "" ;
		}
#else
		InitializeCriticalSection( &m_cs ) ;
#endif

		m_pSendOVSlot = new COV_UNIT[slotSize] ;
		m_pSendBuf = new char[bufSize] ;
	}

	inline ~CSendOVSlot()
	{
		DeleteCriticalSection( &m_cs ) ;
		delete [] m_pSendOVSlot ;
		delete [] m_pSendBuf ;
	}

	inline void GetSendSlot( COV_UNIT ** ppOVUint, const size_t size )
	{		
		if( size == 0  ) 
		{
			*ppOVUint = NULL ;
			return ;
		}

		lock() ;
		*ppOVUint = &m_pSendOVSlot[m_uiCurSlotPos] ;			// 슬롯 하나 얻어오기..
		if( m_uiBufCurPos + size > m_uiBufMaxSize )
		{
			m_uiBufCurPos = 0 ;
		}		

		(*ppOVUint)->m_wsabuf.buf = &m_pSendBuf[m_uiBufCurPos] ;	// 버퍼 매칭..	

		m_uiBufCurPos = m_uiBufCurPos + size ;					// 버퍼 리포인팅
		m_uiCurSlotPos = (m_uiCurSlotPos + 1) % m_uiMaxSlot ;	// 슬롯 리포인팅
		unlock() ;
	}
};

#define _SESSION_RECV_BUF_SIZE	8192		// 세션별 수신 버퍼 크기..
#define _SESSION_OBJECT_COUNT	4096		// 세션 객체 풀링에 생성할 세션객체 갯수..
template<typename t>
class CSocketSession : public COV_UNIT
{
protected:	
	char		m_pRecvBuf[_SESSION_RECV_BUF_SIZE] ;
	char *		m_pRealBuf ;				// 수신버퍼의 지금 위치..
	size_t		m_remainByte ;				// 파싱되지 않은 예약된 데이타 크기

	SOCKET		m_sock ;
	SOCKADDR_IN	m_addr ;

	t *			m_pUser ;					// 템플릿 유저 객체..	

public:
	inline void SetUser(t * pUser) { m_pUser = pUser ; }
	inline t * GetUser() const { return m_pUser ; }
	inline SOCKET GetSocket() { return m_sock ; }
	inline char * GetRecvBuf() { return m_pRealBuf ; }
	inline void SetAddr( SOCKADDR * pAddr ) { memcpy( &m_addr, pAddr, sizeof(SOCKADDR) ) ; }
	
public:
	CSocketSession * pNext ;

	inline CSocketSession() : m_remainByte(0), m_pRealBuf(m_pRecvBuf), m_sock(INVALID_SOCKET), m_pUser(NULL)
	{
		memset(m_pRecvBuf, 0, _SESSION_RECV_BUF_SIZE) ;
		Initialize() ;
	}
	~CSocketSession()
	{
	}

	inline int Initialize() { 

		COV_UNIT::Initialize( m_pRealBuf ) ;

		m_sock = ::WSASocket( AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED ) ;
		if( m_sock == INVALID_SOCKET )
		{
			return WSAGetLastError() ;
		}
		return 0 ;
	}
	

	inline int SendRequest( COV_UNIT * const pOVUnit )
	{
		if( SOCKET_ERROR == ::WSASend( m_sock, &pOVUnit->m_wsabuf, 1, &pOVUnit->m_wsabuf.len, 0, (LPWSAOVERLAPPED)pOVUnit, NULL ) )
		{
			int err = WSAGetLastError() ;
			if( err != WSA_IO_PENDING )
			{
				return err ;
			}
		}
		return 0 ;
	}

	inline bool RecvRequest()
	{		
		DWORD recvLengh = 0 ;
		m_wsabuf.len = _SESSION_RECV_BUF_SIZE - m_remainByte ;
		m_wsabuf.buf = m_pRealBuf ;
		m_iMode = ns_iocp_mode::mode_recv ;

		if( SOCKET_ERROR == ::WSARecv( m_sock, &m_wsabuf, 1, &recvLengh, &m_dwFlag, (LPWSAOVERLAPPED)this, NULL ) )
		{
			int err = WSAGetLastError() ;
			if( err == WSA_IO_PENDING )
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

	inline bool MoveRemainData( const size_t remainSize )
	{
		m_remainByte = remainSize ;
		memmove( m_pRecvBuf, m_pRealBuf, remainSize ) ;
		m_pRealBuf = m_pRecvBuf ;		
	}
};


class CCompletinPort
{
private :
	HANDLE				m_hIOCP ;		

protected:	

public:
	CCompletinPort( const int iThreadNum = 0 ) : m_hIOCP(NULL)
	{		
		m_hIOCP = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, iThreadNum ) ;
		if( m_hIOCP == NULL )
		{
			throw "" ;
		}		
	}
	
	bool Associate( const SOCKET sock )
	{
		if( ::CreateIoCompletionPort( reinterpret_cast<HANDLE>(sock), m_hIOCP, (DWORD)sock, 0) == m_hIOCP )
		{
			return true ;
		}
		return false ;
	}

	
	int GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds = INFINITE )
	{
		return (::GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, pCompKey, ppo, dwMilliseconds));
	}
	
	
	void PostStatus( DWORD CompKey, DWORD dwNumBytes, WSAOVERLAPPED * pOV )
	{
		::PostQueuedCompletionStatus( m_hIOCP, dwNumBytes, CompKey, pOV ) ;
	}
	

	void PostExitWorkThread( const int iPostCount = 0 )
	{
		COV_UNIT OVUnit ;
		OVUnit.m_iMode = ns_iocp_mode::mode_end ;
		
		if( iPostCount > 0 )
		{
			// 쓰레드 갯수만큼 종료 메시지롤 날린다.
			for( int i = 0 ; i < iPostCount ; i++ )
			{
				::PostQueuedCompletionStatus( m_hIOCP, 0, 0, (LPOVERLAPPED)&OVUnit ) ;
			}		
		}
		else
		{
			// 오직 한번 날릴뿐이다..
			::PostQueuedCompletionStatus( m_hIOCP, 0, 0, (LPOVERLAPPED)&OVUnit ) ;
		}
		
	}


	~CCompletinPort()
	{
		::CloseHandle( m_hIOCP ) ;
	}

	
private:
};


enum 
{
	_HANDLE_ACCEPT_POST_EVENT = 0 ,
	_HANDLE_NO_ACCEPT_EVENT,
	_HANDLE_SHUTDOWN_EVENT,
	_HANDLE_NUM,
};

class CAcceptWorker : public CCompletinPort, public CNDThread
{
protected:

	SOCKET			m_listenSock ;
	SOCKADDR_IN		m_addr ;	
	const int		m_iThreadNum ;

protected:
	// Accept 포팅 관련 갯수..
	long			m_nMaxPendingAccept ;		// Accept 를 몇번...
	long			m_nMinPendingAccept ;		// 최소 포스팅 accept
	long			m_nCurPendingAccept ;		// 현재 펜딩된 Accept 의 회수.	

	HANDLE			m_hWaitHandleEvent[_HANDLE_NUM] ;		

	DWORD			m_dwByteReceived ;
	

	static LPFN_ACCEPTEX				pAcceptEx ;
	static LPFN_GETACCEPTEXSOCKADDRS	pGetAcceptExSockaddrs ;
	static LPFN_DISCONNECTEX			pDisconnectEx ;
	static LPFN_TRANSMITFILE			pTransmitFile ;

	virtual void Accept() = 0 ;			// Accept 처리될 함수..
	virtual int Worker() = 0;			// iocp 처리 함수..
	inline void SetEventHandle(int iEvent) { SetEvent(m_hWaitHandleEvent[iEvent]) ; }

	//static unsigned int __stdcall WorkerThread( LPVOID lp ) ;
	static unsigned int __stdcall CAcceptWorker::WorkerThread( LPVOID lp )
	{
		int result = 0 ;
		CAcceptWorker * pthis = (CAcceptWorker*)lp ;
		if( pthis )
		{
			try
			{
				result = pthis->Worker() ;
			}
			catch ( ... ) 
			{
				printf( "[EXCEPTION]Worker\n" ) ;
			}
		}
		
		return result ;
	}

public:

	CAcceptWorker( const int iThreadNum ) :  CCompletinPort( iThreadNum ), m_iThreadNum(iThreadNum), m_nCurPendingAccept(0)
	{
		if( iThreadNum < 1 )
		{
			throw "" ;
		}
		
	}
	virtual ~CAcceptWorker()
	{
	}

	int AcceptEx( SOCKET clientSock,  char * buf, LPOVERLAPPED ovlp) 
	{
		return pAcceptEx( m_listenSock, clientSock, buf, 0, 
			sizeof( SOCKADDR_IN ) + 16, sizeof( SOCKADDR_IN ) + 16 , &m_dwByteReceived, ovlp ) ;
	}

	BOOL DisconnectEx( SOCKET clientSock, LPOVERLAPPED ovlp, DWORD flag = TF_REUSE_SOCKET )
	{
		return pDisconnectEx( clientSock, ovlp, flag, 0 ) ;
	}

	void Shutdown()
	{
		CCompletinPort::PostExitWorkThread( m_iThreadNum ) ;
		SetEvent( m_hWaitHandleEvent[_HANDLE_SHUTDOWN_EVENT] ) ;
	}

	int CreateWorker( const char * const ip, const u_short port, const int iMinPend, const int iMaxPend, const int iBacklog = SOMAXCONN )
	{
		if( ip == NULL || port <= 0 || iMinPend <= 0 || iMaxPend <=0 )
		{
			return false ;
		}

		m_nMinPendingAccept = iMinPend ;
		m_nMaxPendingAccept = iMaxPend ;


		int i ;
		for( i = 0 ; i < _HANDLE_NUM ; i++ )
		{
			if( NULL == ( m_hWaitHandleEvent[i] = CreateEvent( NULL, TRUE, FALSE, NULL ) ) )
			{			
				return -1 ;
			}
		}

		

		m_listenSock = ::WSASocket( AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED ) ;
		
		if( m_listenSock == INVALID_SOCKET )
		{
			return WSAGetLastError() ;
		}

		m_addr.sin_port = htons( port ) ;
		m_addr.sin_addr.s_addr = inet_addr( ip ) ;
		m_addr.sin_family = AF_INET ;

		if( SOCKET_ERROR == bind( m_listenSock, reinterpret_cast<struct sockaddr *>(&m_addr), sizeof(SOCKADDR_IN) ) )
		{
			return WSAGetLastError() ;
		}

		if( SOCKET_ERROR == listen( m_listenSock, iBacklog ) )
		{
			return WSAGetLastError() ;
		}	

		// Load Extention function pointer (WSAIoctl)
		
		GUID guidAccept = WSAID_ACCEPTEX ;
		if( LoadExtensionFunc( m_listenSock, guidAccept, (void**)&pAcceptEx ) == false ) return -1 ;

		GUID guidDisconnect = WSAID_DISCONNECTEX ;
		if( LoadExtensionFunc( m_listenSock, guidDisconnect, (void**)&pDisconnectEx ) == false ) return -1 ;

		GUID guidGetAddr = WSAID_GETACCEPTEXSOCKADDRS ;
		if( LoadExtensionFunc( m_listenSock, guidGetAddr, (void**)&pGetAcceptExSockaddrs ) == false ) return -1 ;

		GUID guidTransmit = WSAID_TRANSMITFILE ;
		if( LoadExtensionFunc( m_listenSock, guidTransmit, (void**)&pTransmitFile ) == false ) return -1 ;
		

		if( false == CCompletinPort::Associate( m_listenSock ) )
		{
			return -1 ;
		}
		


		unsigned int threadid ;
		HANDLE handle ;
		
				
		for( i = 0 ; i < m_iThreadNum ; i++ )
		{
			handle = (HANDLE)::_beginthreadex( 0, 0, WorkerThread, (void*)this, 0, &threadid ) ;	// 쓰레드 내에서 Work()  호출..
			if( handle == INVALID_HANDLE_VALUE )
			{
				return -1 ;
			}
		}
		
		// 자체 스레드 시작.
		Start() ;
		
		return 0 ;
		

	}

	inline bool LoadExtensionFunc( const SOCKET s, const GUID funcID, void **ppFunc ) 
	{
		DWORD dwReturnedByte ;

		if( 0 != WSAIoctl( s, SIO_GET_EXTENSION_FUNCTION_POINTER, (void*)&funcID, sizeof(GUID), ppFunc, sizeof(void*), &dwReturnedByte, 0, 0 ) )
		{
			return false ;
		}
		else
		{
			return true ;
		}
	}	
	

private:

	int Run()
	{
		// Accept 실행
		long i = 0 ;
		long nDef = ( m_nMinPendingAccept + m_nMaxPendingAccept ) / 2 ;
		for( i = 0 ; i < nDef ; i++ )
		{
			Accept() ;
		}
				
		
		if( SOCKET_ERROR == WSAEventSelect( m_listenSock, m_hWaitHandleEvent[_HANDLE_NO_ACCEPT_EVENT], FD_ACCEPT ) )
		{
			printf( "WSAEventSelect Error : %d\n", ::WSAGetLastError() ) ;
			printf( "CServerSocket Run Return\n" ) ;
			return 0 ;
		}		
		
		
		DWORD WaitResult ; 
		BOOL bContineServer = TRUE ;
		long temp ;
		
		while( bContineServer ) 
		{
			WaitResult = ::WaitForMultipleObjects( 3, m_hWaitHandleEvent, FALSE, 20000 ) ;
			
			switch( WaitResult ) {
			case WAIT_OBJECT_0 + _HANDLE_ACCEPT_POST_EVENT :		// Accept 요청이 들어왔다. 비교하고 호출.
				//printf( "   ++ AcceptPostEvent ++  \n" ) ;
				ResetEvent( m_hWaitHandleEvent[_HANDLE_ACCEPT_POST_EVENT] ) ;
				
				if( m_nCurPendingAccept < m_nMaxPendingAccept )
				{
					Accept() ;
				}
				break;
				
			case WAIT_OBJECT_0 + _HANDLE_NO_ACCEPT_EVENT :	// 대기중인 Accept 가 없다. 이거 들어오면 무조건 10번씩 호출하자.
				
				ResetEvent( m_hWaitHandleEvent[_HANDLE_NO_ACCEPT_EVENT] ) ;
				if( m_nCurPendingAccept < m_nMinPendingAccept )
					temp = m_nMinPendingAccept - m_nCurPendingAccept ;
				else if( m_nCurPendingAccept > m_nMaxPendingAccept )
					temp = 0 ;
				else
					temp = 10 ;
				
				printf( "****  NoAcceptWait Event : Accept %d\n", temp ) ;
				
				for( i = 0 ; i < temp ; i++ )
				{
					Accept() ;
				}
				break;
				
			case WAIT_OBJECT_0 + _HANDLE_SHUTDOWN_EVENT :	// ShutDown 시키자.
				ResetEvent( m_hWaitHandleEvent[_HANDLE_SHUTDOWN_EVENT] ) ;
				bContineServer = FALSE ;
				break ;
				
			case WAIT_TIMEOUT:
				printf( "Timeout : Cur Pending = %u\n", m_nCurPendingAccept ) ;
				SetEvent( m_hWaitHandleEvent[_HANDLE_NO_ACCEPT_EVENT] ) ;
				break ;
				
			default:
				printf( "Error : WaitForMultipleObjects Return %u\n", WaitResult ) ;
			}
		}
		
		printf( "CServerSocket Run Thread Return\n" ) ;
		
		return 0 ;
		
	}	

};

