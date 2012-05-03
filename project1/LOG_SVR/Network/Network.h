#pragma once

#ifndef _NETWORK_H_AWEGFASDFAWE
#define _NETWORK_H_AWEGFASDFAWE


#define _MAX_CONNECTION_					100	// 로그서버에 연결되는 최대


#define _RECV_BUF_SIZE_			1048576 * 2



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

class _h_Recv_Overlapped : public _h_Overlapped
{
public:
	int			iBufSize ;
	char *		pRealBuf ;
	int			RemainBytes ;
	
	// 추가

	SOCKET		m_socket;					// 소켓		
	
	CRITICAL_SECTION m_csRecvOV ;		// 보낼 데이타 요청에 쓰임.
	   
    
	//char		m_buf[_RECV_BUF_SIZE_] ;	// 버퍼로 사용할 것임.
	char *		m_pBuf ;
	
public:
	_h_Recv_Overlapped()	
	{ 
		Internal			= InternalHigh	= 0;	
		Offset				= OffsetHigh	= 0;
		hEvent				= NULL;		
		m_pBuf				= NULL ;

		RemainBytes = 0 ;		// Add...
		m_dwFlag = 0 ;
		m_bMode = true ;	
		
		m_pDSComm = NULL;	
		
		if( InitializeCriticalSectionAndSpinCount( &m_csRecvOV, (0xf0000000)|2000 )  == FALSE ) {
			printf( "INIT - Recv OV Critical section : Create Critical Section Failed\n" ) ;		
		}	
	}
	~_h_Recv_Overlapped()	{
		DeleteCriticalSection( &m_csRecvOV ) ;
		if( m_pBuf )
		{
			delete [] m_pBuf ;
		}
		
	} 
	
	bool CreateRecvOV( int size ) {
		iBufSize = size ;
		
		m_pBuf = NULL;		
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
	
	// Add jk.Hong
	bool Parsing(void * pPacketQ, DWORD bufLen);
	
	inline void Lock() { EnterCriticalSection(&m_csRecvOV); }
	inline void UnLock() { LeaveCriticalSection(&m_csRecvOV); }
} ;




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


#define _DS_SEND_BUFFER_SIZE_			4096	//Send 될 데이타가 대기하는 버퍼..

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
	
	CRITICAL_SECTION g_csSendData ;		// 보낼 데이타 요청에 쓰임.

	/*
	버퍼 2개를 놓고, 처음 요청이 들어오면, 1번 버퍼에 쓰고, 바로 보낸다. 
	다시 요청이 들어오면, 1번 보내기 작업이 끝났을 경우, 2번 버퍼에 쓰고 보낸다. 
	1번 작업이 아직 끝나지 않았다면, 그냥 2번 버퍼에 옮기기만 하고, 보내지 않다가
	1번 작업이 끝났을 때 2번 버퍼의 내용을 보내고, 이후의 요청이 들어오면 1번 버퍼에
	쓰고 있다가 2번 작업이 끝났을 때, 1번을 보낸다. 이런 식으로 번갈아 가며 버퍼를 보낸다. 
	물론 보낼 데이타가 없으면 잠시 쉰다. 
	*/
	int				m_iBuf_1_size ;		// 버퍼 1의 데이타 길이 : 
	int 			m_iBuf_2_size ;		// 버퍼 2의 데이타 길이
	int				m_i1_write ;		// 버퍼 1 보내진 바이트 수 : send 이후에 조정
	int				m_i2_write ;		// 버퍼 2 보내진 바이트 수

	char			m_buf_1[_DS_SEND_BUFFER_SIZE_] ;		// 64kbytes 버퍼
	char			m_buf_2[_DS_SEND_BUFFER_SIZE_] ;

	bool			m_bUse ;				// 작업중이냐 아니냐
	bool			m_bSelect ;				// false = 1, true = 2 ; 

	
public:
	
	//char			m_cCharacName[13] ;	

	SOCKADDR_IN		m_sockAddr ;	


	_h_Recv_Overlapped  m_RecvOV ;
	_h_Overlapped		m_SendOV ;				// send 에 쓰일 OV : 절대로 겹치지 않는다. 그래서 단 하나.


	// 현제 보내고 있는 버퍼 또는 바로 이전에 보냈던 버퍼.

	_h_DS_Com() {

		memset( &m_RecvOV, 0, sizeof(_h_Recv_Overlapped) ) ;	
		memset( &m_SendOV, 0, sizeof(_h_Overlapped) ) ;	
		m_dssock = 0 ;

		m_iBuf_1_size = 0 ;
		m_iBuf_2_size = 0 ;
		m_i1_write = 0 ;
		m_i2_write = 0 ;

		memset( m_buf_1, 0, _DS_SEND_BUFFER_SIZE_ ) ;
		memset( m_buf_2, 0, _DS_SEND_BUFFER_SIZE_ ) ;	
		m_bUse = false ;
		m_bSelect = false ;
	}

	void init() 
	{
		m_dssock = 0 ;
		
		m_bUse = false ;
		m_bSelect = false ;
	}

	SOCKET get_sock() { return m_dssock ; }		

	bool createDSCom() ;		// 

	void ConnectCom( SOCKET gsSock) ;
	void DisConnectCom() ;


	void releaseDSCom() ;		//
	int send_request( u_short size, char * pdata ) ;	// 0 : 성공  1 : 버퍼오버  2 : send error
	bool send_complete( int iwrite ) ;		// 바로 전의 send 가 보낸 데이타 량.
	void CloseSock() { 
		shutdown( m_dssock, SD_BOTH ) ;
		closesocket( m_dssock ) ; 
	}
	

} ;

class _DSCom_Queue
{
	
protected:	
	_h_DS_Com	m_DSCom[_MAX_CONNECTION_] ;
	
	
public:
	
	
	bool Init( )
	{
		for( int i = 0 ; i < _MAX_CONNECTION_ ; i++ )
		{
			if( !m_DSCom[i].createDSCom() ) 
			{
				return false ;
			}			
		}
		return true ;
	}
	
	_h_DS_Com * GetDSCom( )
	{
		for( int i = 0 ; i < _MAX_CONNECTION_ ; i++ )
		{
			if( m_DSCom[i].get_sock() == 0 ) {
				m_DSCom[i].init() ;
				return &m_DSCom[i] ;
			}
		}
		return NULL ;
	}
	
	
};


#endif