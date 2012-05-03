#pragma once


#define _MAX_PACKET_SIZE		2048
struct _packet {
	//_h_Recv_Overlapped	*	pRecv_ov;
	_h_DS_Com				*		pDSComm;
	char buf[_MAX_PACKET_SIZE] ;	// 유저로 부터 들어오는 데이타는 이 크기보다 큰건 없을 것이다. 아마도 ^^
};

/*==============================================================
[ CLASS name : _queue_Packet									]
[ Recv TCP packet queue											]
==============================================================*/


class _queue_Packet
{
protected:
	int m_iput ;
	int m_iget ;
	int m_nsize ;
	
	int m_temp ;
	int m_iCurCount ;
	
	CRITICAL_SECTION	cs ;
	
	_packet *	m_pPackets ;	

	inline void lock() { EnterCriticalSection( &cs ) ; }
	inline void unlock() { LeaveCriticalSection( &cs ) ; }
	
public:
	
	_queue_Packet( int iMaxSize ) : m_iput(0), m_iget(0), m_nsize(iMaxSize), m_iCurCount(0), m_pPackets(NULL) {
		if( InitializeCriticalSectionAndSpinCount( &cs, (0xf0000000)|2000 )  == FALSE ) {
			printf( " cs Error Critical Section\n" ) ;
		}
		if( Create(iMaxSize) == false ) throw ;
	}
	~_queue_Packet() 
	{ 
		DeleteCriticalSection( &cs ) ; 
		delete [] m_pPackets ;
	}
	
	bool Create( int nPacket ) ;				 
	
	// Recf_Overlapped 구조체 포인터를 넘겨주네.. ㅡㅡ;
	void insert(_h_Recv_Overlapped * pRecv_ov, int size) ;	
	void test(MSG_CHARACTER * msg) ;	
	_packet * get() ;
	
	inline bool empty(){
		return ( m_iCurCount == 0 ) ? true : false ;
	}
	
public:

	
} ;



