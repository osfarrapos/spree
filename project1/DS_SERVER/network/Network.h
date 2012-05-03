#pragma once

//#include <WinSock2.h>

#ifndef _NETWORK_H_AWEGFASDFAWE
#define _NETWORK_H_AWEGFASDFAWE

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

	void	SetSendData( char * buf, int length ) ;
	//int		AddSendData( char * buf, int length ) ;
	int		SendPacket( const SOCKET sock, const bool bZeroCheck = false ) ;
	void	SetOperation( int mode ) { m_iMode = mode ; }

	void    reset() 
	{
		m_wsabuf.buf = NULL ;
		m_wsabuf.len = 0 ;
		m_dwFlag = 0 ;
		m_iMode = 0 ;

#ifdef _DEBUG
		m_iCapacity = 0 ;
#endif
	}
} ;


/*

class CDSRecv_Overlapped : public COV_UNIT
{
public:
	char *		pRealBuf ;
	int			RemainBytes ;

	// 추가
	char *		m_buf ;	// 버퍼로 사용할 것임.
	size_t		m_bufSize ;

public:
	CDSRecv_Overlapped( size_t size ) : m_bufSize(size)
	{ 
		m_buf = new char[size] ;
	}
	~CDSRecv_Overlapped()	
	{
		delete [] m_buf ;
	} 
	
	bool RecvPacket( const SOCKET sock ) ;	// Recv 요청을 한다. 
	void move( int remain ) ;
	void reset() ;
} ;

*/



class CSendOverlappedSlot
{
protected:
	int					m_iPosition ;
	int					m_iTotalSize ;
	int					m_iGetSlot ;		// 0~Max까지 순환한다. 
	int					m_nOVSlot ;			// OV 슬롯의 개수

	char *				m_pSendBuf ;
	COV_UNIT *			m_pOVSlot ; 

	CTraceAbleLock		m_csSendOvSlot ;

	//inline void lock()		{ EnterCriticalSection( &m_csOLSlot ) ; }
	//inline void unlock()	{ LeaveCriticalSection( &m_csOLSlot ) ; }
#define send_ov_lock()		m_csSendOvSlot.tlock( __FILE__, __LINE__ ) ;
#define send_ov_unlock()	m_csSendOvSlot.tunlock( __FILE__, __LINE__) ;
	
public:
	CSendOverlappedSlot() {
		m_iPosition = 0 ;
		m_iTotalSize = 0 ;
		m_iGetSlot = 0 ;
		m_nOVSlot = 0 ;

		m_pSendBuf = NULL ;		
		m_pOVSlot = NULL ;
		

		/*
		if( InitializeCriticalSectionAndSpinCount( &m_csOLSlot, (0x80000000)|2000 )  == FALSE ) {
			printf( "[INIT ERROR] OL Slot cs Error Critical Section\n" ) ;
			throw ;
		}
		*/
	}

	~CSendOverlappedSlot() {		// 소멸자		
		//DeleteCriticalSection( &m_csOLSlot ) ; 

		delete m_pSendBuf ;
		delete m_pOVSlot ;
	}

	bool CreateOVSlot( int slotcount, int megasize ) ;					// Create Slot

	void Get_SendUnit( COV_UNIT ** ppSender, int reqSize ) ;		// 항상 성공 : 
	//void Get_BigUnit( COV_UNIT ** ppSender ) ;		// 1536 만큼씩 가져간다.


//#ifdef _DEBUG
//	void print_state() ;
//#endif
} ;


//#################################################################################
//
//	_h_Overlapped : 커스텀 오버랩드 패킷
//                  move 패킷을 전송할 때, 다 보내지 않았는데 변경되는 경우가 있음
//                  그래서 send 한 번할 때마다 nSend 를 증가시켜 횟수를 저장한다. 
//                  결과를 받았을 때 nSend 가 1 이 되면 그 패킷을 delete( 만약 슬롯을
//                  사용한다면 put ) 한다.  0726
//
//###################################################################### LHS Added....
// 크기 160 bytes 정도

class _h_DS_Com ;
class _h_Overlapped : public WSAOVERLAPPED
{
public:
	WSABUF		m_wsabuf ;
	bool		m_bMode ;			// 
	DWORD		m_dwFlag ;			
	_h_DS_Com *	m_pDSComm;
} ;

class _queue_Packet ;
class _h_Recv_Overlapped : public _h_Overlapped
{
public:
	// 서버 정보 구조체..
	struct _GS_INFO 
	{
		unsigned long		ipAddr ;
		short				state ;
	} ;		

	// 추가
	_GS_INFO	m_gs_info;					// 연결된 서버정보
	SOCKET		m_socket;					// 소켓		

	
	u_short		m_usLastParsingSize;
	u_char		m_ucLastParsingMsg;

	DWORD		m_dwTotalRecvBytes;
	int			m_iLastErrorCode;

	int			iBufSize ;
	char *		pRealBuf ;
	int			RemainBytes ;

	//CRITICAL_SECTION m_csRecvOV ;		// 보낼 데이타 요청에 쓰임.
	char *		m_pBuf ;

public:
	_h_Recv_Overlapped()	
	{ 
		Internal			= InternalHigh	= 0;	
		Offset				= OffsetHigh	= 0;
		hEvent				= NULL;		
		
		RemainBytes = 0 ;		// Add...
		m_dwFlag = 0 ;
		m_bMode = true ;	

		m_gs_info.ipAddr = 0;
		m_gs_info.state = 0;

		m_usLastParsingSize = 0;
		m_ucLastParsingMsg = 0;

		m_dwTotalRecvBytes = 0;
		m_iLastErrorCode = 0;

		m_pDSComm = NULL;	
		pRealBuf = NULL ;
		m_pBuf = NULL ;

		/*
		if( InitializeCriticalSectionAndSpinCount( &m_csRecvOV, (0x80000000)|2000 )  == FALSE ) {
			printf( "INIT - Recv OV Critical section : Create Critical Section Failed\n" ) ;		
		}
		*/
	}
	~_h_Recv_Overlapped()	{
		//DeleteCriticalSection( &m_csRecvOV ) ;
		if(m_pBuf != NULL) {
			delete [] m_pBuf ;
			m_pBuf = NULL;
		}
	} 

	bool CreateRecvOV( int size ) {
		iBufSize = size ;

		m_pBuf = new char[iBufSize] ;
		if( m_pBuf )
		{
			m_wsabuf.buf = m_pBuf ;
			pRealBuf = m_pBuf ;		// Add...

			return true ;
		}

		return false ;
		
	}
	
	bool RecvPacket( const SOCKET sock ) ;	// Recv 요청을 한다. 
	void move( int remain ) ;
	void reset() ;

	inline void SetLastParsingSize( u_short size )	{ m_usLastParsingSize = size; }
	inline void SetLastParsingMsg( u_char msg )		{ m_ucLastParsingMsg = msg; }

	inline u_short GetLastParsingSize()				{ return m_usLastParsingSize; }
	inline u_char GetLastParsingMsg()				{ return m_ucLastParsingMsg; }

	inline void SetErrorCode( int err )				{ m_iLastErrorCode = err; }
	inline int GetErrorCode()						{ return m_iLastErrorCode; }

	// Add jk.Hong
	bool Parsing(DWORD bufLen);

	//inline void Lock() { EnterCriticalSection(&m_csRecvOV); }
	//inline void UnLock() { LeaveCriticalSection(&m_csRecvOV); }

	int SendPacket(const SOCKET sock) {
		int err ;
		err = WSASend( sock, &m_wsabuf, 1, &m_wsabuf.len, 0, (LPWSAOVERLAPPED)this, NULL ) ;
		if( err == SOCKET_ERROR ) {
			err = WSAGetLastError() ;
			if( err != WSA_IO_PENDING ) 
			{
				m_iLastErrorCode = err;
				return err ;
			}
		}

		return 0 ;
	}

	void ShowState();
} ;
typedef _h_Recv_Overlapped	CSendPkt;





class _h_CompletionPort
{
protected:
	HANDLE				m_hIOCP ;

public:
	bool Create( int n ) ;					// 동시 스레드 수를 지정한다. 
	bool AssociateSocket( _h_Recv_Overlapped * pOv  ) ;		// 소켓을 연관시키는데, 이때 키 값도 똑같은 값으로 전달한다.

	// GetQueue~~ 요청
	bool GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds = INFINITE) ;

} ;


//#define _DS_SEND_BUFFER_SIZE_			131072	//Send 될 데이타가 대기하는 버퍼..
#include "../Packet_Queue/WebShopPktQueue.h"
class _h_DS_Com
{
	/* =====================================================================================
	36개의 이벤트를 이용하여, 동기 메시지를 구현하고, 세마포로 36개의 이벤트를 관리한다. 
	함수 이름이 request/wait 로 시작하는 함수는 동기 요청과 결과다.
	세마포로 유효 이벤트를 기다리고, 큐로는 유효 이벤트의 인덱스를 얻는다.
	===================================================================================== */
	// sync 부분 : 2003/08/21
	// sync 는 유저의 접속 스레드에서만 쓰이고, 나머지 겜 진행에서는 async 로 처리한다. 
protected:

	SOCKET			m_dssock ;			// DS 서버와 통신 소켓.	
	//CRITICAL_SECTION g_csSendData ;		// 보낼 데이타 요청에 쓰임.
	

	bool			m_bUse ;				// 작업중이냐 아니냐
	bool			m_bSelect ;				// false = 1, true = 2 ; 

	u_char			m_ucLastProcMsg;		// 
	u_short			m_usLastProcSize;		// 

	DWORD			m_dwTotalSendBytes;


public:
	_h_Recv_Overlapped  m_RecvOV ;
	CSendOverlappedSlot	m_sendSlot ;	// send ov slot

	inline void set_last_msg( u_char msg )		{ m_ucLastProcMsg = msg; }
	inline void set_last_size( u_short size )	{ m_usLastProcSize = size; }

	inline u_char get_last_msg()				{ return m_ucLastProcMsg; }
	inline u_short get_last_size()				{ return m_usLastProcSize; }
	

	_h_DS_Com() {

		/*
		if( InitializeCriticalSectionAndSpinCount( &g_csSendData, (0x80000000)|2000 )  == FALSE ) {
			printf( "INIT - DS_Com : Create Critical Section Failed\n" ) ;
			throw ;
		}
		*/

		init() ;

#ifdef ITEMSHOP_PATCH_200607
		pWebShopPktQueue = new CWebShopPktQueue;
		assert(pWebShopPktQueue);
#endif
	}
	~_h_DS_Com() {
#ifdef ITEMSHOP_PATCH_200607
		SAFE_DELETE(pWebShopPktQueue);
#endif

		//DeleteCriticalSection(&g_csSendData);
	}

	void init() 
	{
		m_dssock = 0 ;

		m_bUse = false ;
		m_bSelect = false ;

		m_dwTotalSendBytes = 0;

		m_RecvOV.reset();
	}

	SOCKET get_sock() { return m_dssock ; }	

	bool createDSCom( const int iSendSlotMaxCount, const int iSendSlotMaxMegaSize, const int iRecvBufSize ) ;		// 
	void ConnectCom( SOCKET gsSock) ;
	void DisConnectCom() ;
	

	bool Send( int size, char * buf ) ;
	bool Recv( SOCKET sock ) {
		return m_RecvOV.RecvPacket( sock ) ;
	}

	void ShowState( int iServer );
	
#ifdef ITEMSHOP_PATCH_200607
public:
	CWebShopPktQueue* pWebShopPktQueue;
#endif
} ;


class _DSCom_Queue
{

protected:	
	_h_DS_Com	m_DSCom[GS_MAX] ;


public:

	
	bool Init( const int iSendSlotCount, const int iSendSlotMegaSize, const int iRecvBufSize )
	{
		for( int i = 0 ; i < GS_MAX ; i++ )
		{
			if( !m_DSCom[i].createDSCom( iSendSlotCount, iSendSlotMegaSize, iRecvBufSize ) ) 
			{
				return false ;
			}			
		}
		return true ;
	}

	_h_DS_Com * GetDSCom( )
	{
		for( int i = 0 ; i < GS_MAX ; i++ )
		{
			if( m_DSCom[i].get_sock() == 0 ) {
				m_DSCom[i].init() ;
				return &m_DSCom[i] ;
			}
		}
		return NULL ;
	}

	bool GlobalSend( int size, char * buf )
	{		
		for( int i = 0 ; i < GS_MAX ; i++ )
		{
			if( m_DSCom[i].get_sock() != 0 ) 
			{
				m_DSCom[i].Send( size, buf );
			}
		}
		return true ;
	}
};

#endif

#ifdef ITEMSHOP_PATCH_200607
struct WEB_SHOP_PKT_PROC_INFO {
	_h_DS_Com* pClient;
};

struct WEB_SHOP_PKT_PROC_PARAM {
	_h_DS_Com* pClient;
};
#endif

//#################################################################################
//
//	Net Base Structures
//
//###################################################################### LHS Added....
#define MAX_OF_RECV_BUFFER		(32768)
#define	MAX_OF_SEND_BUFFER		(16384)		// 그냥 큰 값.
/// Mode : IO
enum io_mode
{
	iomode_recv = 0,
	iomode_send_complete,
	iomode_accept_complete, 
	iomode_close_socket, 
	iomode_end
};

struct CIO_UNIT : public WSAOVERLAPPED
{
	DWORD		m_dwFlag;		// Zero	
	int			m_iMode;		// io_mode
	u_long		m_ulTempSize;

	WSABUF		m_wsaBuf;

	char		m_data[MAX_OF_SEND_BUFFER];

	CIO_UNIT()
	{
		ZeroMemory( this, sizeof(CIO_UNIT) );
		m_wsaBuf.buf = m_data;
		m_wsaBuf.len = 0;
	}

	void SetData( void * pData, int length )
	{
		// 모든 패킷의 앞에 2바이트 u_short 로 길이를 붙인다.
		m_iMode = iomode_send_complete;
		u_short usLen = static_cast<u_short>(length);
		CopyMemory( m_data, &usLen, sizeof(usLen) );
		CopyMemory( &m_data[sizeof(usLen)], pData, length );
		m_wsaBuf.len = (u_long)(length+sizeof(usLen));
	}

	WSABUF * GetWSABuf()	{ return &m_wsaBuf; }
	u_long * GetPLength()	{ return &m_ulTempSize; }

};



//#################################################################################
//
//	CNDIOCPObejct : IOCP 관리자.
//
//###################################################################### LHS Added....
class CNDIOCPObejct
{
public:
	bool Create( int n );					// 동시 스레드 수를 지정한다. 
	bool AssociateSocket( SOCKET s, DWORD CompKey );		// 소켓을 연관시키는데, 이때 키 값도 똑같은 값으로 전달한다.

	// GetQueue~~ 요청
	int GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds = INFINITE);
	void PostStatus( DWORD CompKey, DWORD dwNumBytes, WSAOVERLAPPED * pOV );

	void PostCloseWorkThread();
	void Close() {
		CloseHandle( m_hIOCP );
	}


protected:
	HANDLE				m_hIOCP;

	CIO_UNIT			m_OVUnit ;		// 끝낼 때 필요함.
} ;

#include <list>
using namespace std;

/*
Send 는 간단히 Send 버퍼 일정개수를 돌려가며 쓰도록 하자.
30개 이상만 되도, Send 버퍼가 중복될 일은 없을 것이다. 
그래서 100개로 하자. 

이 객체는 모든 CConnectionObject 가 사용한다. 


그러나, 원래는 Send Buffer는 사용/해제를 반복하며 메모리 풀에서 이뤄져야 한다.
*/
class CSendBuffer
{
public:
	CSendBuffer( int nBuffer ) : m_iSendBufferPosition(0), m_iSendBufferSize(nBuffer), m_pBufferUnit(NULL)
	{
		m_pBufferUnit = new CIO_UNIT[m_iSendBufferSize];
		if( !m_pBufferUnit )
			throw "(Send Buffer) Create Buffer Fail";

		if( FALSE == InitializeCriticalSectionAndSpinCount( &m_csSendBuffer, 0x80000100 ) )
			throw "(Send Buffer) Create Critical Section Fail";
	};

	~CSendBuffer()
	{
		Sleep(100);		// 버퍼 사용이 완료되는 시점을, 그냥 조금 기다리는 것으로 대체한다.
		if( m_pBufferUnit )
		{
			DeleteCriticalSection(&m_csSendBuffer);
			delete [] m_pBufferUnit;
			m_pBufferUnit = NULL;
		}
	}

	CIO_UNIT * GetSendBuffer()	
	{
		CIO_UNIT * pIO = NULL;
		int pos = 0;
		EnterCriticalSection(&m_csSendBuffer);
		pos = m_iSendBufferPosition;
		m_iSendBufferPosition = (m_iSendBufferPosition+1) % m_iSendBufferSize;
		pIO = &m_pBufferUnit[pos];
		LeaveCriticalSection(&m_csSendBuffer);
		return pIO;
	}

protected:
	int		m_iSendBufferPosition;		//
	int		m_iSendBufferSize;

	CIO_UNIT *		m_pBufferUnit;

protected:
	CRITICAL_SECTION	m_csSendBuffer;
};


// 파싱된 데이타를 받아온다.
struct __mid_packet_unit
{
	int		length;
	char *	packet;
};


class CRecvBuffer
{
public:
	CRecvBuffer() : m_iCurStartPos(0), m_iCurEndPos(0)
	{
		ZeroMemory( &m_wsaBuf, sizeof(m_wsaBuf) );
		ZeroMemory( m_cbRecvBuffer, sizeof(m_cbRecvBuffer) );
	}

	void NoticeRecv( int RecvLength );
	bool GetPacket( __mid_packet_unit& unit );	// false 일 때까지 계속 받아온다.


	char * GetEndPos()	{ return &m_cbRecvBuffer[m_iCurEndPos]; }
	int GetLeftBufferLength()	{ return (MAX_OF_RECV_BUFFER-m_iCurEndPos); }
	WSABUF * GetWSABuf()	
	{
		m_wsaBuf.buf = GetEndPos();
		m_wsaBuf.len = GetLeftBufferLength();
		ZeroMemory( m_wsaBuf.buf, m_wsaBuf.len );
		return &m_wsaBuf;
	}

protected:
	int			m_iCurStartPos;		// 
	int			m_iCurEndPos;

	WSABUF		m_wsaBuf;			// 

	char		m_cbRecvBuffer[MAX_OF_RECV_BUFFER];
};


class CConnectionObject
{
public:
	CConnectionObject( CSendBuffer& sendbuf) 
		: m_iConIndex(0), m_iConState(FALSE), 
		m_sockConnect(INVALID_SOCKET), m_refSendBuffer(sendbuf),
		m_iLastError(0)
	{
		ZeroMemory(m_ip, sizeof(m_ip));
		m_port = 0;

		::ZeroMemory( &m_ovRecv, sizeof(m_ovRecv) );
		m_ovRecv.m_iMode = iomode_recv ;
		m_ovRecv.m_dwFlag = 0 ;
	}

public:
	void Connect( char * ip, unsigned short port );		
	void SendRequest( void * data, u_short length );	// Request Sending Data.
	BOOL RecvRequest();	
	BOOL ParseAndProcess( int recvbytes );
	void Close();

public:
	inline int GetIndex()			{ return m_iConIndex; }
	inline void SetIndex( int idx )	{ m_iConIndex = idx; }

	inline int GetState()			{ return m_iConState; }
	inline void SetState( int state )	{ m_iConState = state; }

	inline SOCKET GetSocket()		{ return m_sockConnect; }
	inline void SetSocket( SOCKET s )	{ m_sockConnect = s; }

	inline char * GetIP()			{ return m_ip ; }
	inline unsigned short GetPort()	{ return m_port ; }

	inline int GetError()			{ return m_iLastError; }

protected:
	int				m_iConIndex;
	int				m_iConState;		// TRUE/FALSE : Connection State. Check this before send.
	int				m_iLastError;		// WSAGetLastError()

	SOCKET			m_sockConnect;		// Connected Socket ID

	char			m_ip[16];			// Target Information
	unsigned short	m_port;				//

	CRecvBuffer		m_recvBuffer;

	CIO_UNIT		m_ovRecv;
	CIO_UNIT		m_ovSend;

protected:	// Send Member
	CSendBuffer &	m_refSendBuffer;
};


class CConnectionManager
{
public:
	enum
	{
		en_work_mode_ds=0,		// ds에서 동작할 때.
		en_work_mode_gms,		// gms에서 동작할 때.
	};

	CConnectionManager()
	{}

	~CConnectionManager()
	{
		if( m_ConList.size() > 0 )
		{
			list<CConnectionObject *>::iterator itr = m_ConList.begin();
			while( itr != m_ConList.end() ) 
			{
				delete *itr;
				itr++;
			}
			m_ConList.clear();		// 안해도 된다. 
		}

		if( m_pSendBuffer )
		{
			delete m_pSendBuffer;
			m_pSendBuffer = NULL;
		}

		DeleteCriticalSection( &m_csLock );
	}

	// 이 함수는 잘 변형해서 쓰세요. 
	bool InitConnectionManager( int workmode, int bufsize, char * ip, unsigned short port );

	// GMS
//	void ConnectAll( CDSInformation& dsinfo );
	void StartRecv();
	// 연결이 안된 객체가 있다면, 다시 시도해 본다. 
//	bool RetryConnect_NotConnectedObject();
//	void GMSRecv();


	// DS
	void StartAccept( char * ip, unsigned short port );
	void DSAccept();
	void DSRecv();

	void AddConObject( CConnectionObject * pCon )
	{
		bool bFind = false;

		EnterCriticalSection( &m_csLock );
		list<CConnectionObject *>::iterator itr = m_ConList.begin();


		while( itr != m_ConList.end() )
		{
			if( pCon == *itr )
			{
				bFind = true;
				break;
			}
			itr++;
		}

		if( !bFind )	// 없으면 삽입한다.
			m_ConList.push_back( pCon );
		LeaveCriticalSection( &m_csLock );
	}
	CConnectionObject * GetConObject()
	{
		CConnectionObject * pCon = NULL;

		EnterCriticalSection( &m_csLock );

		if( m_ConList.size() > 0 )
		{
			pCon = m_ConList.front();
			m_ConList.pop_front();
		}
		else
		{
			try
			{
				pCon = new CConnectionObject( *m_pSendBuffer );
			}
			catch( ... )
			{
				ShowMsg( "[Connection Object] Create Fail" );
			}
		}
		LeaveCriticalSection( &m_csLock );
		return pCon;
	}

protected:
	// Recv Thread
	static unsigned int __stdcall th_GMSRecvThread( LPVOID lp );
	// Accept Thread
	static unsigned int __stdcall th_GMSAcceptThread( LPVOID lp );

public:	// Send
	void SendTo( int index, void * data, u_short length );

	CConnectionObject * FindConnection( int index );

public:
	// 현재 상태를 보려면, 다음 함수를 호출한다. 
	// 그 아래의 두 함수는 콘솔일 때와 윈도우 모드일 때를 나눈 것 뿐이다. 
	void PrintState();
protected:
	void PrintStateToConsole();
	void PrintStateToMSGWIN();

protected:
	list<CConnectionObject*>	m_ConList;	
	CNDIOCPObejct				m_IOCP;		// IOCP Object
	CSendBuffer	*				m_pSendBuffer;

	SOCKET						m_sockAccept;	// Accept Socket

	char						m_AcceptIP[16];
	unsigned short				m_AcceptPort;

	int							m_iWorkMode;	// work mode

	bool						m_bAcceptThreadState;
	bool						m_bRecvThreadState;

	HANDLE						m_hInitEvent;

	CRITICAL_SECTION			m_csLock;
};