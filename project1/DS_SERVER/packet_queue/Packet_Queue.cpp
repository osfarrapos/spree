#include "..\stdafx.h"
#include "..\MAPManager\MAPManager.h"
#include "..\Network\Network.h"
#include "Packet_Queue.h"


/*==============================================================
[ CLASS name : _queue_Packet									]
[ Recv TCP packet queue											]
==============================================================*/


void _queue_Packet::insert( const _h_Recv_Overlapped * const pRecv_ov, const int size/*, DWORD time*/ )
{	

	
	if( pRecv_ov == NULL || size < 1  || size >= _MAX_PACKET_SIZE ) 
	{
		::PrintConsole("[ERROR] TCPPacket size over error (curSize:%d)\n", size ) ;
		return;
	}

	lock() ;
	try {
		
	
		m_iTemp = (m_iput+1) % m_nsize ;
		if( m_iTemp == m_iget )
		{
			unlock() ;
			return ;
		}
		memcpy( m_pNode[m_iput].buf, pRecv_ov->pRealBuf, size ) ;
		m_pNode[m_iput].pDSComm = pRecv_ov->m_pDSComm;		//send socket

		m_iput = m_iTemp ;		
		++m_iCurCount ;
	

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] 'void _queue_Packet::insert(_h_Recv_Overlapped * pRecv_ov, int size )' Exception \n") ;		
	}

	unlock() ;
	
}


/*==============================================================
[ CLASS name : _j_queue_UDP_Packet								]
[ UDP packet queue												]
==============================================================*/

void _j_queue_UDP_Packet::insert_udp( SOCKADDR_IN * addr, char * buf, int bufSize )
{	
	if( buf	== NULL || addr == NULL || bufSize >= _MAX_UDP_PACKET_SIZE) 
	{
		::PrintConsole("[ERROR] UDPPacket size over error (curSize:%d)\n", bufSize ) ;
		return;
	}
	
	lock() ;	
	try {		

		m_iTemp = (m_iput+1)%m_nsize ;
		if( m_iTemp == m_iget )
		{			
			return ;
		}
		
		m_pNode[m_iput].iFromConType = 0;	// udp
		m_pNode[m_iput].pGMSTcpCon = NULL;

		memcpy( &m_pNode[m_iput].addr, addr, sizeof(SOCKADDR_IN) ) ;				
		memcpy( m_pNode[m_iput].buf, buf, bufSize) ;	
		m_pNode[m_iput].sBufSize = bufSize ;
		
			
		m_iput = m_iTemp ;		
		++m_iCurCount ;		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] 'void _j_queue_UDP_Packet::insert( SOCKADDR_IN * addr, char * buf, int bufSize )' \n") ;		
	}
	unlock() ;
}

void _j_queue_UDP_Packet::insert_tcp( CConnectionObject * con, char * buf, int bufSize )
{	
	if( buf	== NULL || NULL == con || bufSize >= MAX_OF_SEND_BUFFER) 
	{
		::PrintConsole("[ERROR] TcpPacket size over error (curSize:%d)\n", bufSize ) ;
		return;
	}

	lock() ;	
	try {		

		m_iTemp = (m_iput+1)%m_nsize ;
		if( m_iTemp == m_iget )
		{			
			return ;
		}

		m_pNode[m_iput].iFromConType = 1;	// Tcp
		m_pNode[m_iput].pGMSTcpCon = con;

		ZeroMemory( &m_pNode[m_iput].addr, sizeof(SOCKADDR_IN) ) ;				
		memcpy( m_pNode[m_iput].buf, buf, bufSize) ;	
		m_pNode[m_iput].sBufSize = bufSize ;


		m_iput = m_iTemp ;		
		++m_iCurCount ;		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] 'void _j_queue_UDP_Packet::insert( SOCKADDR_IN * addr, char * buf, int bufSize )' \n") ;		
	}
	unlock() ;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CND_PacketLengthChecker::init()
{
	// 패킷 더하기.
	add_packet( CMSG_NO_CHARAC_CREATE_GS_DS, sizeof(CMSG_CHARAC_CREATE_GS_DS) ) ;
	add_packet( CMSG_NO_USER_CHARAC_LIST_DS_GS, sizeof(CMSG_USER_CHARAC_LIST_DS_GS) ) ;
	add_packet( CMSG_NO_CHARAC_ALL_INFO_DS_GS, sizeof(CMSG_CHARAC_ALL_INFO_DS_GS) ) ;
	add_packet( CMSG_NO_DEPOT_RES_GS_DS, sizeof(CMSG_DEPOT_RES_DS_GS) ) ;

	add_packet( CMSG_NO_UPDATE_CHARAC_ALL_, sizeof(MSG_UPDATE_CHARAC_ALL) ) ;
}

void CND_PacketLengthChecker::add_packet( u_char packno, u_short length ) 
{
	m_packetLength[packno] = length ;
}

bool CND_PacketLengthChecker::check_packet( CMSG_CHECK_DSGS_PACKET * checkpacket )
{

	PrintConsole( "[DS CHECK] Recv from GS : Check DSGS Packet\n" );

	bool bResult = true ;
	u_char msg ;

	checkpacket->ucMode = 1 ;

	for( int i = 0 ; i < 30 ; ++i )
	{
		msg = checkpacket->check_pair[i].msg ;
		if( msg )
		{
			if( m_packetLength.end() != m_packetLength.find( msg ) )
			{
				if( checkpacket->check_pair[i].length != m_packetLength[msg] )
				{
					bResult = false ;
					checkpacket->check_pair[i].length = m_packetLength[msg] ;
				}
				else
				{
					checkpacket->check_pair[i].length = 0 ;
				}
			}
		}
		else
		{
			break ;
		}
	}

	return bResult ;
}


void CND_PacketLengthChecker::show_all()
{
	map<u_char, u_short>::iterator itr = m_packetLength.begin() ;

	while( itr != m_packetLength.end() )
	{
		printf( " msg : %3d - Length %3d\n", itr->first, itr->second ) ;
		++itr ;
	}
}


void CND_PacketLengthChecker::fill_packet( CMSG_CHECK_DSGS_PACKET * pcheck )
{
	int i = 0 ;

	ZeroMemory( &pcheck->check_pair[0], sizeof(pcheck->check_pair) ) ;

	map<u_char, u_short>::iterator itr = m_packetLength.begin() ;

	while( i < 30 && itr != m_packetLength.end() )
	{
		pcheck->check_pair[i].msg = itr->first ;
		pcheck->check_pair[i].length = itr->second ;

		++itr ;
		++i ;
	}
}

void CND_PacketLengthChecker::show_result( CMSG_CHECK_DSGS_PACKET * pcheck )
{
	int i = 0 ;
	u_char msg ;

	
	
	if( pcheck->ucMode == 2 )
	{
		printf( "  => Recv : Check Packet : Fail\n" ) ;

		while( i < 30 )
		{
			msg = pcheck->check_pair[i].msg ;
			if( msg )
			{
				if( pcheck->check_pair[i].length > 0 )
					printf( "    Different : Msg %3u, Org %3d, Rcv %d\n", msg, \
							m_packetLength[msg], pcheck->check_pair[i].length ) ;
			}
			else
				break ;

			++i ;
		}
	}
	else
	{
		printf( "  => Recv : Check Packet : Success\n" ) ;
	}
}