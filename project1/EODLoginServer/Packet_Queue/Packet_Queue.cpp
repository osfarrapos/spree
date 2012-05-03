#include "..\stdafx.h"
#include "..\MyServer\myServer.h"
#include "..\Billing\BillingSvrLink.h"
#include "Packet_Queue.h"


void _queue_Packet::insert( _h_User * pUser, char * buf, int size )
{	
	
	if( !pUser || buf[0] == 0 ){ 
		return;
	}
	lock() ;	
	try {
		
		m_iTemp = (m_iput+1)%m_nsize ;
		if( m_iTemp != m_iget )
		{
			m_pNode[m_iput].pUser = pUser ;
			memcpy( m_pNode[m_iput].buf, buf, size ) ;		
			m_pNode[m_iput].len = size ;
			
			m_iput = m_iTemp ;
			++m_iCurCount ;
		}		
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] 'void _queue_Packet::insert(_h_Recv_Overlapped * pRecv_ov, int size )' Exception \n") ;		
	}
	unlock() ;
	
}


void _j_queue_UDP_Packet::insert( SOCKADDR_IN * addr, char * buf, int bufSize ) 		
{	
	if( buf	== NULL || addr == NULL || addr->sin_port == 0 ) {
		return;
	}


	
	lock() ;
	try {
		m_iTemp = (m_iput+1)%m_nsize ;
		if( m_iTemp != m_iget )
		{
			memcpy( &m_pNode[m_iput].addr, addr, sizeof(SOCKADDR_IN) ) ;
			memcpy( m_pNode[m_iput].buf, buf, bufSize) ;
			m_pNode[m_iput].sBufSize = bufSize ;
			
			m_iput = m_iTemp ;
			++m_iCurCount ;			
		}		

	}
	catch (...) {
		printf("[EXCEPTION] 'void _j_queue_UDP_Packet::insert( SOCKADDR_IN * addr, char * buf, int bufSize )' Exception \n") ;		
	}
	unlock() ;	
}
