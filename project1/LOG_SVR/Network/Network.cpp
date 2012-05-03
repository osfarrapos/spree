#include "..\stdafx.h"
#include "Network.h"
#include "..\VMManager\VMManager.h"
#include "..\Queue\packet_queue.h"



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
	memset( m_pBuf, 0, iBufSize ) ;
	m_wsabuf.buf = m_pBuf ;
	pRealBuf = m_pBuf ;		// Add...
	RemainBytes = 0 ;		// Add...
	m_dwFlag = 0 ;
	m_bMode = true ;
}

void _h_Recv_Overlapped::move( int remain ) 
{
	try {
		RemainBytes = remain ;
		memmove( m_pBuf, pRealBuf, RemainBytes ) ;
		pRealBuf = m_pBuf;
	}
	catch ( ... ) {
#ifdef _TEST_
		::PrintConsole( "[EXCEPTION] Recv Overlapped::move\n" ) ;
#endif
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

bool _h_Recv_Overlapped::Parsing(void  * _pPacketQ, DWORD bufLen)
{

	_queue_Packet	* pPacketQ;
	pPacketQ = (_queue_Packet *)_pPacketQ;

	short	head;
	int	copiedBytes = 0;		//카피된 데이타 길이(헤더 카피가 이루어졌는지 여부를 판단..)
	bool state = true;			// false 이면 while loop 를 빠져나온다..
	int availableBytes = 0;		//패킷의 길이 HEADER_LEN + BODY(head.sLength)	
	int HEADER_LEN = sizeof(head);

	RemainBytes += bufLen;		//전에 이완되어온 데이타 길이 + 새로받은 데이타길이
	pRealBuf = m_pBuf;	//버퍼커서를 맨 앞으로..

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
			availableBytes = head;	// 헤더를 포함해서 바디까지 패킷 하나..
			if( (RemainBytes >= availableBytes) && ( 0 < availableBytes ) )	// 나머지가 받을양보다 크거나 같을때..
			{					
				
				pPacketQ->insert(this, availableBytes);
												

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
	return (CreateIoCompletionPort((HANDLE)pOv->m_socket, m_hIOCP, (DWORD)pOv,0) == m_hIOCP);
}

bool _h_CompletionPort::GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds)
{
	return (GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, pCompKey, ppo, dwMilliseconds) == TRUE);
}


/*#################################################################################

DS_Com

#################################################################################*/


int _h_DS_Com::send_request( u_short size, char * pdata )
{
	int iRet =0;
	int iError =0;

	EnterCriticalSection( &g_csSendData ) ;

	try {
		if( m_bSelect ) 
		{	
			// true 라면, 2번 버퍼를 보내는 중이므로 1 번 버퍼에 쓴다.
			// 			
			if( m_iBuf_1_size + size > _DS_SEND_BUFFER_SIZE_ )
				iRet = 1 ;
			else 
			{
				memcpy( &m_buf_1[m_iBuf_1_size], pdata, size ) ;
				m_iBuf_1_size += size ;

				if( !m_bUse ) 
				{	// 이전 작업이 끝난 후이므로, 바로 보낸다. 
					// send 한다.
					m_SendOV.m_bMode = false ;		// send = false 
					m_SendOV.m_wsabuf.buf = m_buf_1 ;
					m_SendOV.m_wsabuf.len = m_iBuf_1_size ;//m_i1_write ;
					m_i1_write = 0 ;

					if( SOCKET_ERROR == WSASend( m_dssock, &m_SendOV.m_wsabuf, 1, &m_SendOV.m_dwFlag, 0, (LPWSAOVERLAPPED)&m_SendOV, NULL ) ) 
					{
						iError = WSAGetLastError();
						if( WSAEWOULDBLOCK == iError || ERROR_IO_PENDING == iError) 
						{
							iRet = 0 ;
							m_bUse = true ;			// 
							m_bSelect = false ;
						}
						else {
							iRet = 2 ;
						}
					}
					else		// 바로 성공했다. 그러면 wouldblock과 동일하게 처리.
					{
						iRet = 0 ;
						m_bUse = true ;			// 
						m_bSelect = false ;
					}
				}
			}
		}
		else 
		{			
			// false 라면, 1번 버퍼를 보내는 중이므로 2 번 버퍼에 쓴다.
			// 
			if( m_iBuf_2_size + size > _DS_SEND_BUFFER_SIZE_ )
				iRet = 1 ;
			else 
			{
				memcpy( &m_buf_2[m_iBuf_2_size], pdata, size ) ;
				m_iBuf_2_size += size ;

				if( !m_bUse ) 
				{	// 이전 작업이 끝난 후이므로, 바로 보낸다. 
					// send 한다.
					m_SendOV.m_bMode = false ;		// send = false 
					m_SendOV.m_wsabuf.buf = m_buf_2 ;
					m_SendOV.m_wsabuf.len = m_iBuf_2_size ;
					m_i2_write = 0 ;

					if( SOCKET_ERROR == WSASend( m_dssock, &m_SendOV.m_wsabuf, 1, &m_SendOV.m_dwFlag, 0, (LPWSAOVERLAPPED)&m_SendOV, NULL ) ) 
					{						
						iError = WSAGetLastError();
						if( WSAEWOULDBLOCK == iError || ERROR_IO_PENDING == iError) 
						{
							iRet = 0 ;
							m_bUse = true ;			// 
							m_bSelect = true ;
						}
						else {
							iRet = 2 ;
						}
					}
					else		// 바로 성공했다. 그러면 wouldblock과 동일하게 처리.
					{
						iRet = 0 ;
						m_bUse = true ;			// 
						m_bSelect = true ;
					}
				}
			}
		}
	}

	catch(...) {
		LeaveCriticalSection( &g_csSendData ) ;
		::PrintConsole("[EXCEPTION] send_request() \n");

	}
	LeaveCriticalSection( &g_csSendData ) ;

	return iRet ;
}

/* =====================================================================================
complete 함수가 불렸을 때, m_bUse 는 false 일리 없다. 
===================================================================================== */
bool _h_DS_Com::send_complete( int iwrite )
{
	bool bRet =0;		// false 이면 send 에러.
	int	iError =0;

	// 아직 처리 못했음...

	EnterCriticalSection( &g_csSendData ) ;

	try {
		if( m_bSelect ) 
		{		
			m_i2_write += iwrite ;
			if( m_i2_write == m_iBuf_2_size ) 
			{
				m_iBuf_2_size = 0 ;
				m_i2_write = 0 ;
				m_bUse = false ;			// 사용끝.
				bRet = true ;
			}
			else 
			{		// 아직 다 못 보냈다. 다시 한번 보낸다. 
				//m_i2_write -= iwrite ;
				m_SendOV.m_wsabuf.buf = &m_buf_2[m_i2_write] ;
				m_SendOV.m_wsabuf.len = m_iBuf_2_size - m_i2_write ;

				if( SOCKET_ERROR == WSASend( m_dssock, &m_SendOV.m_wsabuf, 1, &m_SendOV.m_dwFlag, 0, (LPWSAOVERLAPPED)&m_SendOV, NULL ) ) 
				{
					iError = WSAGetLastError();
					if( WSAEWOULDBLOCK == iError || WSA_IO_PENDING == iError ) 
					{
						bRet = true ;
					}
					else {
						bRet = false ;
					}
				}
			}

		}
		else 
		{		
			m_i1_write += iwrite ;
			if( m_i1_write == m_iBuf_1_size ) 
			{
				m_iBuf_1_size = 0 ;
				m_i1_write = 0 ;
				m_bUse = false ;			// 사용끝.
				bRet = true ;
			}
			else 
			{		// 아직 다 못 보냈다. 다시 한번 보낸다. 
				//m_i1_write -= iwrite ;
				m_SendOV.m_wsabuf.buf = &m_buf_1[m_i1_write] ;
				m_SendOV.m_wsabuf.len = m_iBuf_1_size - m_i1_write ;

				if( SOCKET_ERROR == WSASend( m_dssock, &m_SendOV.m_wsabuf, 1, &m_SendOV.m_dwFlag, 0, (LPWSAOVERLAPPED)&m_SendOV, NULL ) ) 
				{
					iError = WSAGetLastError();
					if( WSAEWOULDBLOCK == iError || WSA_IO_PENDING == iError ) 
					{
						bRet = true ;
					}
					else {
						bRet = false ;
					}
				}
			}
		}
	}

	catch (...) {
		LeaveCriticalSection( &g_csSendData ) ;
		::PrintConsole("[EXCEPTION] send_request() \n");
	} 

	LeaveCriticalSection( &g_csSendData ) ;




	return bRet ;

}

/* =====================================================================================
createDSCom() : 생성
===================================================================================== */
bool _h_DS_Com::createDSCom( )
{
	if( InitializeCriticalSectionAndSpinCount( &g_csSendData, (0xf0000000)|2000 )  == FALSE ) {
		printf( "INIT - DS_Com : Create Critical Section Failed\n" ) ;
		return false ;
	}	


	// Recv ov create
	if( false == m_RecvOV.CreateRecvOV( _RECV_BUF_SIZE_ ) )
	{
		printf( "InnerCom::Recv OV Create Failed\n" ) ;
		return false ;
	}		

	return true ;
}


/* =====================================================================================
releaseDSCom() : 해제
===================================================================================== */
void _h_DS_Com::releaseDSCom()
{
	try{
		if( m_dssock ) {
			shutdown(m_dssock, SD_BOTH) ;
			closesocket( m_dssock ) ;
			m_dssock = NULL ;
		}
				
		DeleteCriticalSection(&g_csSendData);		
	}
	catch ( ...	) {
		::PrintConsole("[EXCEPTION] void _h_DS_Com::releaseDSCom() \n ") ;
	}	

}



void _h_DS_Com::ConnectCom( SOCKET gsSock )
{
	m_dssock = gsSock;
	m_RecvOV.m_socket = gsSock;	
	m_RecvOV.m_pDSComm = this;	
	m_SendOV.m_pDSComm = this;
}

/* =====================================================================================
releaseDSCom() : 해제
===================================================================================== */
void _h_DS_Com::DisConnectCom()
{
	try{
		m_RecvOV.reset() ;
		if( m_dssock ) {
			closesocket( m_dssock ) ;
			m_dssock = NULL ;
		}
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION]  _h_DS_Com::releaseDSCom \n") ;
	}	
	
}