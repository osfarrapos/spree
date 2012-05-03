#pragma once


#include "../MyLib/MY_QUEUE.H"


#define _MAX_PACKET_SIZE		4096
#define _MAX_UDP_PACKET_SIZE	4096
//#define _MAX_PACKET_NUMBER_		65536		// 16384 32767 65536

struct _packet {
	//_h_Recv_Overlapped	*	pRecv_ov;
	_h_DS_Com				*		pDSComm;
	char buf[_MAX_PACKET_SIZE] ;	// 유저로 부터 들어오는 데이타는 이 크기보다 큰건 없을 것이다. 아마도 ^^
};



struct _packetUDP {
	SOCKADDR_IN		addr ;
	int				sBufSize ;	
	char			buf[_MAX_UDP_PACKET_SIZE] ;
};


/*==============================================================
[ CLASS name : _queue_Packet									]
[ Recv TCP packet queue											]
==============================================================*/
class _queue_Packet : public CRotationQueue<_packet>
{
	
public:
	
	_queue_Packet(size_t size) : CRotationQueue<_packet>(size)
	{		
	}
	~_queue_Packet() 
	{ 
	}	
	
	void insert( _h_DS_Com * pDSCom, char * buf, int size) ;
} ;



/*==============================================================
[ CLASS name : _j_queue_UDP_Packet								]
[ UDP packet queue												]
==============================================================*/
class _j_queue_UDP_Packet : public CRotationQueue<_packetUDP>
{
	
public:
	
	_j_queue_UDP_Packet(size_t size) : CRotationQueue<_packetUDP>(size)
	{	
	}
	~_j_queue_UDP_Packet() 
	{ 	
	}
	
	void insert( SOCKADDR_IN * addr, char * buf, int bufSize ) ;	
};

