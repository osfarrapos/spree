#pragma		once
#include <Winsock2.h>
#include <mswsock.h>
#include <string>
#include <queue>
#include <map>
using namespace std;


/*#################################################################################

CS 통신 객체

#################################################################################*/


template <class Type>
class CTQueue
{
protected : 
	queue<Type*>	ReadyQueue ;
	queue<Type*>	UseQueue ;	
	Type  *			m_pNode ;


#define ReadyLock() m_ReadyQueue_cs.tlock( __FILE__, __LINE__ ) ;
#define ReadyUnlock() m_ReadyQueue_cs.tunlock( __FILE__, __LINE__ ) ;
#define UseLock() m_UseQueue_cs.tlock( __FILE__, __LINE__ ) ;
#define UseUnlock() m_UseQueue_cs.tunlock( __FILE__, __LINE__ ) ;
	

public:

	CTraceAbleLock m_ReadyQueue_cs;	// 대기큐 섹션
	CTraceAbleLock m_UseQueue_cs;		// 사용중인 큐 섹션


	CTQueue(size_t slotCount ) 
	{


		m_pNode = new Type[slotCount] ;
		assert( m_pNode ) ;		
		for( u_int i=0; i<slotCount; i++)
		{			
			Ready_Put( &m_pNode[i] ) ;			
		}		
	};

	~CTQueue() {
		delete [] m_pNode ;
	};


	// 대기 큐에 넣기 
	inline void Ready_Put(Type * pNode) {
		if( pNode )
		{
			ReadyLock() ;
			ReadyQueue.push(pNode) ;
			ReadyUnlock() ;
		}		
	}

	// 대기 큐에서 꺼내기
	inline Type * Ready_Pop() {
		Type * p = NULL ;
		TRY
		{
			ReadyLock() ;
			if( ReadyQueue.empty() ) {
				ReadyUnlock() ;
				return NULL ;
			}

			p = ReadyQueue.front() ;			
			ReadyQueue.pop() ;
			ReadyUnlock() ;
			return p ;
		}
		CATCH_ALL
		{

			ReadyUnlock() ;
			::PrintConsole("[EXCEPTION] inline Type * Ready_Pop() \n" ) ;
			return NULL ;
		}

	} 

	// 사용 큐에 넣기
	inline void Use_Put(Type * pNode) {
		if( pNode ) 
		{
			UseLock() ;
			UseQueue.push(pNode) ;
			UseUnlock() ;
		}		
	}

	// 사용 큐에서 꺼내기
	inline Type * Use_Pop() {
		Type * p = NULL ;
		TRY
		{
			UseLock() ;
			if( UseQueue.empty() ) {
				UseUnlock() ;
				return NULL ;
			}

			p = UseQueue.front() ;			
			UseQueue.pop() ;
			UseUnlock() ;
			return p ;
		}
		CATCH_ALL
		{
			UseUnlock() ;
			::PrintConsole("[EXCEPTION] inline Type * Use_Pop() \n" ) ;
			return NULL ;
		}

	} 



	inline int	GetReadyCount() {
		return ReadyQueue.size() ;		
	}
	inline int	GetUseCount() {
		return UseQueue.size() ;
	}


};

#define _LOG_QUEUE_COUNT		8096
class CLogSvr_Com 
{
protected:
	SOCKET			m_cssock ;			// DS 서버와 통신 소켓.

	HANDLE			m_hIOCP ;			// 
	HANDLE			m_hThread ;

	CSendOverlappedSlot		m_sendslot ;
	_h_Recv_Overlapped		m_recvOV ;


	CTQueue<_CharacLog>			m_CharacLogQueue ;
	CTQueue<_ItemLog>			m_ItemLogQueue ;
	CTQueue<_ServerState>		m_ServerLogQueue ;
	CTQueue<_LoginLog>			m_LoginLogQueue ;


	COV_UNIT	m_SendOV ;				// send 에 쓰일 OV : 절대로 겹치지 않는다. 그래서 단 하나.
	// 현제 보내고 있는 버퍼 또는 바로 이전에 보냈던 버퍼.

	char			m_ip[17] ;
	short			m_port ;

	BOOL			m_bIsCreated ;		// cs 연결이 이루어진 상태냐, 아니냐.

public:

	CLogSvr_Com() : m_CharacLogQueue(_LOG_QUEUE_COUNT), m_ServerLogQueue(1024), m_ItemLogQueue(4096), m_LoginLogQueue(4096), m_bIsCreated(FALSE)
	{
	}

	bool ReadInfoFile( const char * fileName, const short sGroupNo, char * const cIP, u_short * const port ) ;

	// IOCP 관련 부분
	bool Associate( SOCKET s ) { 
		return (CreateIoCompletionPort((HANDLE)s, m_hIOCP, s, 0) == m_hIOCP) ;
	}
	int GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds = INFINITE) {
		return (GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, pCompKey, ppo, dwMilliseconds));
	}
	SOCKET get_sock() { return m_cssock ; }

	int send_request( u_short size, char * pdata ) ;	// 0 : 성공  1 : 버퍼오버  2 : send error

	bool send_complete( int iwrite ) ;		// 바로 전의 send 가 보낸 데이타 량.
	

	// 공통 부분 : 이름 중복을 왠만하면 피하자.
	bool createCSCom( const char * cIP, const unsigned short port ) ;		// 	
	void releaseCSCom() ;		//

	bool RestoreConnection( ) ;	// CS 와 연결을 복구한다. ozzywow

	friend DWORD WINAPI th_LogSendthread( LPVOID lp ) ;
	friend DWORD WINAPI th_StatusCheck( LPVOID lp ) ;		// 연결 검사

	inline SOCKET GetSocket() { return m_cssock ; }
	
	bool RecvReq() 
	{
		return m_recvOV.RecvPacket( m_cssock ) ;		
	}


	// 로그 쓰기 함수..
	_CharacLog * GetCharacLogSlot() ;
	void InsertCharacLog( _CharacLog * pLog ) ;

	_ServerState * GetServerLogSlot() ;
	void InsertServerLog( _ServerState * pLog ) ;

	_ItemLog * GetItemLogSlot() ;
	void InsertItemLog( _ItemLog * pLog ) ;

	_LoginLog * GetLoginLogSlot() ;
	void InsertLoginLog( _LoginLog * pLog ) ;


	void LogWrite_ChangeName( char * from, char * to, short group ) ;
	void LogWrite_ItemTaxAdd( short map_id, u_int total, u_int part, bool bTemp );
	void LogWrite_ItemTaxTake( char * from, short map_id, u_int take );
	
	
} ;
