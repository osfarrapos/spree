#pragma once


#include <MY_QUEUE.H>
#define _MAX_PACKET_SIZE		20476
#define _MAX_UDP_PACKET_SIZE	16384
//#define _MAX_PACKET_NUMBER_		65536		// 16384 32767 65536

struct _packet {
	//_h_Recv_Overlapped	*	pRecv_ov;
	_h_DS_Com				*		pDSComm;
	char buf[_MAX_PACKET_SIZE] ;	// 유저로 부터 들어오는 데이타는 이 크기보다 큰건 없을 것이다. 아마도 ^^
};

struct _packetUDP {
	int					iFromConType;	// 0 - UDP, 1 - TCP
	CConnectionObject *	pGMSTcpCon;		// GMS 연결 객체.

	SOCKADDR_IN		addr ;
	int				sBufSize ;	
	char			buf[_MAX_UDP_PACKET_SIZE] ;
};


class _queue_Packet : public CRotationQueue<_packet>
{

public:

	_queue_Packet(size_t size) : CRotationQueue<_packet>(size)
	{		
	}
	~_queue_Packet() 
	{ 
	}	

	void insert( const _h_Recv_Overlapped * const pRecv_ov, const int size) ;
} ;



class _j_queue_UDP_Packet : public CRotationQueue<_packetUDP>
{

public:

	_j_queue_UDP_Packet(size_t size) : CRotationQueue<_packetUDP>(size)
	{	
	}
	~_j_queue_UDP_Packet() 
	{ 	
	}

	void insert_udp( SOCKADDR_IN * addr, char * buf, int bufSize ) ;	
	void insert_tcp( CConnectionObject * con, char * buf, int bufsize );
};


class CND_PacketLengthChecker
{
public:
	void init() ;
	void add_packet( u_char packno, u_short length ) ;
	bool check_packet( CMSG_CHECK_DSGS_PACKET * checkpacket ) ;
	void show_all() ;
	void fill_packet( CMSG_CHECK_DSGS_PACKET * pcheck ) ;
	void show_result( CMSG_CHECK_DSGS_PACKET * pcheck ) ;


protected:
	map<u_char,u_short>  m_packetLength ;
} ;


