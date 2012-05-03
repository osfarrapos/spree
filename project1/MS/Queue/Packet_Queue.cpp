#include "..\stdafx.h"
#include "..\VMManager\VMManager.h"
#include "..\MAP\MAPManager.h"
#include "..\Network\Network.h"
#include "Packet_Queue.h"


/*==============================================================
[ CLASS name : _queue_Packet									]
[ Recv TCP packet queue											]
==============================================================*/
void _queue_Packet::insert( _h_DS_Com * pDSCom, char * buf, int size )
{	

	if( !pDSCom || buf == NULL || buf[0] == 0 || size < 1 ){ 
		return;
	}

	_packet packet ={0} ;
	packet.pDSComm = pDSCom ;
	memcpy( packet.buf, buf, size ) ;

	CRotationQueue<_packet>::insert( &packet ) ;
	
}


/*==============================================================
[ CLASS name : _j_queue_UDP_Packet								]
[ UDP packet queue												]
==============================================================*/
void _j_queue_UDP_Packet::insert( SOCKADDR_IN * addr, char * buf, int bufSize )
{
	
	if( buf	== NULL || addr == NULL || bufSize  < 1 ) {
		return;
	}

	_packetUDP udp_packet = {0} ;	
	memcpy( &udp_packet.addr, addr, sizeof(SOCKADDR_IN) ) ;
	memcpy( &udp_packet.buf, buf, bufSize ) ;
	udp_packet.sBufSize = bufSize ;

	CRotationQueue<_packetUDP>::insert( &udp_packet ) ;
	
}
