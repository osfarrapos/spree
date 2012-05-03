#ifndef _ND_SERVER_TEST_ACCEPT_EX_EXAMPLE_
#define _ND_SERVER_TEST_ACCEPT_EX_EXAMPLE_

#pragma warning(disable:4786)

#include <mswsock.h>
#include <queue>
using namespace std;

#include "../lib_ND_Util/ND_Thread.h"


#define _RECV_BUF_SIZE_				8192		
#define _BIG_UNIT_SIZE_				1536

#ifndef _MEGA_
#define _MEGA_	1048576
#endif


#define _KEEPALIVE_TIME_					30000L
#define _KEEPALIVE_INTERVAL_				500L

namespace ND_Net {

//#define _MAX_PACKET_SLOT_			196608     

class CSocketBuffer ;

class CBaseUser
{
public:
	void SetNetBuffer( CSocketBuffer * pBuffer ) 
	{		
		m_pNetIO = pBuffer ;
	}

	virtual void OnConnectionClose() = 0 ;
	virtual bool Parsing( DWORD recvbytes ) = 0 ;

	CSocketBuffer *			m_pNetIO ;

protected:


} ;
/// Mode : IO
enum cp_mode
{
	mode_recv = 0,
	mode_send_complete,
	mode_accept_complete, 
	mode_close_socket, 
	mode_end
} ;


class COV_UNIT : public WSAOVERLAPPED		// _h_Overlapped
{
public:
	WSABUF		m_wsabuf ;
	DWORD		m_dwFlag ;			
	int			m_iMode ;			// 
	
#ifdef _DEBUG
	u_long		m_iCapacity ;
#endif
	
#ifdef _DEBUG
	void	Initialize( char * buf, int cap ) ;
#else
	void	Initialize( char * buf ) ;
#endif
	
	//void	SetSendData( char * buf, int length ) ;
	int		AddSendData( char * buf, int length ) ;
	int		SendPacket( const SOCKET sock ) ;
	void	SetOperation( int mode ) { m_iMode = mode ; }
} ;




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class CServerSocket ;

class CSocketBuffer : public COV_UNIT
{

public:	
	CServerSocket * const m_server ;
	SOCKET		socket ;
	SOCKADDR_IN	addr ;
	bool		m_bAccepted ;
	CBaseUser *	m_pUser ;					// 유저를 가리키는 포인터	

	char *		pRealBuf ;	
	int			RemainBytes ;
	char		m_buf[_RECV_BUF_SIZE_] ;	// 버퍼로 사용할 것임.
	

	static	LONG	m_glCreateCount ;

	/*
	long		m_lTotalSendByte ;
	long		m_lStartTime ;			//
	long		m_lEndTime ;		]
	*/
	
protected:
		
	
public:
	CSocketBuffer( CServerSocket * server) : m_server(server), m_bAccepted(false)		
	{ 
		::InterlockedIncrement(&m_glCreateCount); 
		release() ;
	}
	~CSocketBuffer()
	{ 
		::InterlockedDecrement(&m_glCreateCount) ; 
	} 

	static LONG GetUsingCount() 
	{ 		
		return m_glCreateCount ;
	}

	bool RecvRequest() ;	// Recv 요청을 한다. 

	bool InitSocket() ;						// 소켓 핸들을 새로 할당한다..
	bool ReleaseSocket() ;					// 소켓 핸들을 해제한다.

	inline void release()
	{		
		memset( &addr, 0 , sizeof(addr) ) ;
		m_bAccepted = false ;	
		m_pUser = NULL ;
		
		pRealBuf = m_buf ;
		RemainBytes = 0 ;
		memset( m_buf, 0, _RECV_BUF_SIZE_) ;
		
		COV_UNIT::Internal = InternalHigh	= 0;	
		COV_UNIT::Offset = OffsetHigh	= 0;
		COV_UNIT::hEvent = NULL;	
		COV_UNIT::m_wsabuf.buf = m_buf ;
		COV_UNIT::m_wsabuf.len = 0 ;
		COV_UNIT::m_dwFlag = 0 ;
		COV_UNIT::m_iMode = 0 ;	
	}

	
	void move( int remain )  ;	
	void SetUser( CBaseUser * pUser ) { m_pUser = pUser ; }
	void SetMode( int mode ) { m_iMode = mode ; }
	
	COV_UNIT * ReserveBuffer( int size ) ;		// 
	int  SendRequest( COV_UNIT * pOV ) ;		// Unit Data Send...
	int  SendRequest( char * buf, int size ) ;	// Single Send Request
	int  SendRequest( int size , char * buf) ;	// Single Send Request

	bool IsConnected() ;

	void Shutdown( int how ) ;

	/*
	void net_stat_start( DWORD time )	{ m_lStartTime = time ; m_lTotalSendByte = 0 ; }
	void net_stat_end( DWORD time )		{ m_lEndTime = time ; }
	float net_stat() ;
	void net_stat_reset()				{ ::InterlockedExchange( &m_lTotalSendByte, 0 ) ; }
	*/
} ;

class CSendOverlappedSlot
{
protected:
	int					m_iPosition ;
	int					m_iTotalSize ;
	int					m_iGetSlot ;		// 0~Max까지 순환한다. 
	int					m_nOVSlot ;			// OV 슬롯의 개수
	
	char *				m_pSendBuf ;
	COV_UNIT *			m_pOVSlot ; 
	
	CRITICAL_SECTION	m_csOLSlot ;
	
	inline void lock()		{ EnterCriticalSection( &m_csOLSlot ) ; }
	inline void unlock()	{ LeaveCriticalSection( &m_csOLSlot ) ; }
	
public:
	CSendOverlappedSlot() {
		m_iPosition = 0 ;
		m_iTotalSize = 0 ;
		m_pSendBuf = NULL ;
		m_iGetSlot = 0 ;
		
		if( InitializeCriticalSectionAndSpinCount( &m_csOLSlot, (0x80000000)|2000 )  == FALSE ) {
			printf( "[INIT ERROR] OL Slot cs Error Critical Section\n" ) ;
			throw ;
		}
	}
	
	~CSendOverlappedSlot() {		// 소멸자
		DeleteCriticalSection( &m_csOLSlot ) ; 
	}
	
	bool CreateOVSlot( int slotcount, int megasize ) ;					// Create Slot
	
	void Get_SendUnit( COV_UNIT ** ppSender, int reqSize ) ;		// 항상 성공 : 
	void Get_BigUnit( COV_UNIT ** ppSender ) ;		// 1536 만큼씩 가져간다.

#ifdef _DEBUG
	void print_state() ;
#endif
} ;


/*#################################################################################
//
//  _h_CompletionPort
//
//###############################################################################*/
class _h_CompletionPort
{
protected:
	HANDLE				m_hIOCP ;
	
	COV_UNIT			m_OVUnit ;		// 끝낼 때 필요함.

	int					m_nWorker ;		// 워커 스레드 몇 개.
	
public:
	_h_CompletionPort(int nConcurrentWorker) : m_hIOCP(NULL), m_nWorker(nConcurrentWorker) 
	{
	}

	bool Create( int nWorker ) ;					// 동시 스레드 수를 지정한다. 
	bool AssociateSocket( CSocketBuffer * pB ) ;		// 소켓을 연관시키는데, 이때 키 값도 똑같은 값으로 전달한다.
	bool AssociateSocket( SOCKET s ) ;
	
	// GetQueue~~ 요청
	int GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds = INFINITE) ;
	void PostStatus( DWORD CompKey, DWORD dwNumBytes, WSAOVERLAPPED * pOV ) ;
	
	void PostCloseWorkThread() ;
	void Close() {
		CloseHandle( m_hIOCP ) ;
	}
	
} ;


//
static BOOL LoadExtensionFunction( SOCKET s, GUID functionID, void **ppFunc ) ;

/*#################################################################################
//
//  CAcceptSocket :
//
//###############################################################################*/
class CServerSocket : public ND_Util::CThread
{
public :

	CServerSocket() : m_iocp(0), m_sendSlot(), m_iTempBuf(0)
	{
	}

	BOOL CreateServerSocket( 
		int nWorker,				// IO 처리 워커 스레드의 개수
		char * listenip,			// Accept 소켓으 주소
		u_short listenport,			// Accept 소켓의 bind 포트
		int nSendSlotCount,			// 쓰기 Overlapped 의 개수
		int nBufMegaSize,			// 쓰기 공용버퍼의 크기(Mega단위)
		int nSockBufPool,			// 소켓 버퍼의 풀링 개수
		int backlog,				// 리슨 소켓의 백로그
		int nAcceptPostMax,			// 최대 Accept 회수
		int nAcceptPostMin			// 최소 Accept 회수
		) ;		// Initialize

	void CloseListenSocket() ;		// 


	BOOL DisconnectEx( CSocketBuffer * pSockBuf, LPOVERLAPPED pUnit, DWORD flag ) 
	{
		return pDisconnectEx( pSockBuf->socket, pUnit, flag, 0 ) ;
	}

	BOOL TransmitFile( CSocketBuffer * pSockBuf, LPOVERLAPPED pUnit ) 
	{
		return pTransmitFile( pSockBuf->socket, 0, 0, 0, pUnit, 0, TF_DISCONNECT | TF_REUSE_SOCKET ) ;
	}
	

	void GetAcceptExSockaddrs(
		PVOID pOutputBuffer,       
		DWORD receiveDataLength,  
		DWORD localAddressLength,  
		DWORD remoteAddressLength,  
		LPSOCKADDR *ppLocalSockaddr,  
		LPINT pLocalSockaddrLength,  
		LPSOCKADDR *ppRemoteSockaddr,  
		LPINT pRemoteSockaddrLength ) 
	{
		pGetAcceptExSockaddrs( pOutputBuffer, receiveDataLength, localAddressLength,  
									remoteAddressLength, ppLocalSockaddr, pLocalSockaddrLength, 
									ppRemoteSockaddr, pRemoteSockaddrLength ) ;
	}

	bool AcceptCompleted( CSocketBuffer * pBuffer, DWORD dwReceiveByte ) ;


	void DisconnectForReuse( CSocketBuffer * pBuffer ) ;

	void AcceptRequest() ;


	//////////////////////////////////////////////////////////////////////////
	// Shutdown
	void Shutdown() ;


	//////////////////////////////////////////////////////////////////////////
	// Virtural

	//virtual void OnConnectionEstablished( CSocketBuffer * pSock ) ;	// 바깥에서 이거 오버로딩 : 

	//////////////////////////////////////////////////////////////////////////
	// Socket Attach/Detach, PostIOCP
	void PostStatus( DWORD dwNumBytes, CSocketBuffer * pBuffer ) ;
	void PostCloseWorkThread() ;

	//////////////////////////////////////////////////////////////////////////
	// 
	void Get_SendUnit( COV_UNIT ** ppOV, int size ) { m_sendSlot.Get_SendUnit( ppOV, size ) ; }
	void Get_BigUnit( COV_UNIT ** ppOV ) { m_sendSlot.Get_BigUnit( ppOV ) ; }


protected:
	BOOL LoadExtensionFunctions() ;
	BOOL LoadAcceptEx( SOCKET s ) ;
	BOOL LoadGetAcceptExSockaddrs( SOCKET s ) ;
	BOOL LoadDisconnectEx( SOCKET s ) ;
	BOOL LoadTransmitFile( SOCKET s ) ;
	
	BOOL CreateAcceptSocket() ;

	void Accept() ;

	int AcceptEx( CSocketBuffer * pSockBuf ) 
	{
		return pAcceptEx( m_listensocket, pSockBuf->socket, pSockBuf->m_buf, 0, 
			m_sizeofaddr, m_sizeofaddr, &m_byteReceived, static_cast<LPOVERLAPPED>(pSockBuf) ) ;
	}

	virtual int Run( void ) ;		// 이 객체의 스레드
	virtual int Worker( void ) ;	// iocp 처리할 워커 스레드.

	static unsigned int __stdcall WorkerThread( LPVOID lp ) ;

private:
	CRITICAL_SECTION		m_csPool ;				// pool 락.

protected:
	SOCKET					m_listensocket ;
	_h_CompletionPort		m_iocp ;
	CSendOverlappedSlot		m_sendSlot ;

	SOCKADDR_IN				m_addr ;

	int						m_sizeofaddr ;
	unsigned long			m_byteReceived ;

	long					m_nMaxPendingAccept ;		// Accept 를 몇번...
	long					m_nMinPendingAccept ;		// 최소 포스팅 accept
	long					m_nCurPendingAccept ;		// 현재 펜딩된 Accept 의 회수.

	long					m_nMaxPoolCount ;
	
	int						m_nWorkerThread ;		// 

	HANDLE					m_hAcceptPostEvent ;	// Accept 포스트 요청 이벤트
	HANDLE					m_hNoAcceptWaitEvent ;	// 대기중인 Accept요청이 없을 때 발생.
	HANDLE					m_hShutDownEvent ;		// 서버 닫을 때.

	//ND_Util::CQueueFixedSize<CSocketBuffer>		m_queuePool ;
	queue<CSocketBuffer*>	m_UsingPool ;			// 사용중인 소캣 풀(socket handle 이 할당된..)
	queue<CSocketBuffer*>	m_RecyclePool ;			// 사용중이지 않은 소켓 풀..( closesocket 된..)

protected:
	inline void 			pool_lock() { EnterCriticalSection( &m_csPool ) ; }
	inline void				pool_unlock() { LeaveCriticalSection( &m_csPool ) ; }
	

public:
	int						m_iTempBuf ;
	
protected:
	static LPFN_ACCEPTEX				pAcceptEx ;
	static LPFN_GETACCEPTEXSOCKADDRS	pGetAcceptExSockaddrs ;
	static LPFN_DISCONNECTEX			pDisconnectEx ;
	static LPFN_TRANSMITFILE			pTransmitFile ;
} ;


} // namespace nd_net


#endif