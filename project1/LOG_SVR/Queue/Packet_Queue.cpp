#include "..\stdafx.h"
#include "..\VMManager\VMManager.h"
#include "..\MAP\MAPManager.h"
#include "..\Network\Network.h"
#include "Packet_Queue.h"


/*==============================================================
[ CLASS name : _queue_Packet									]
[ Recv TCP packet queue											]
==============================================================*/

bool _queue_Packet::Create( int nPacket )
{
	bool bReturn = false ;
	lock() ;
	try {
		m_pPackets = new _packet[nPacket] ;
		
		if( m_pPackets ==NULL ) {
			printf( "  -> _queue_Packet : MemoryCommit failed\n" ) ;
			bReturn = false ;
		}
		else
		{
			m_nsize = nPacket ;
			bReturn = true ;
		}
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] 'bool _queue_Packet::Create( _h_VMManager * pVM, int nPacket )' Exception \n") ;
		bReturn = false ;
	}
	unlock() ;
	return bReturn ;
	
}

void _queue_Packet::insert(_h_Recv_Overlapped * pRecv_ov, int size/*, DWORD time*/ )
{	
	
	
	if( pRecv_ov == NULL || size < 1) {
		return;
	}
	
	lock() ;
	try {
		m_temp = (m_iput+1) % m_nsize ;
		if( m_temp == m_iget )
		{
			unlock() ;
			return ;
		}
		memcpy( m_pPackets[m_iput].buf, pRecv_ov->pRealBuf, size ) ;
		m_pPackets[m_iput].pDSComm = (_h_DS_Com *)pRecv_ov->m_pDSComm;		//send socket
		
		m_iput = m_temp ;		
		++m_iCurCount ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] 'void _queue_Packet::insert(_h_Recv_Overlapped * pRecv_ov, int size )' Exception \n") ;		
	}
	unlock() ;
	
}



void _queue_Packet::test(MSG_CHARACTER * msg )
{	
	
	
	lock() ;
	try {
		m_temp = (m_iput+1) % m_nsize ;
		if( m_temp == m_iget )
		{
			unlock() ;
			return ;
		}
		memcpy( m_pPackets[m_iput].buf, &msg, sizeof(MSG_CHARACTER) ) ;
		m_pPackets[m_iput].pDSComm = NULL ;
		
		m_iput = m_temp ;		
		++m_iCurCount ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] 'void _queue_Packet::insert(_h_Recv_Overlapped * pRecv_ov, int size )' Exception \n") ;		
	}
	unlock() ;
	
}


_packet * _queue_Packet::get()
{
	_packet * pTemp = NULL ;
	lock() ;
	try {
		if( m_iCurCount > 0 )
		{
			pTemp = &m_pPackets[m_iget] ;	
			m_iget = (++m_iget) % m_nsize ;
			--m_iCurCount ;
		}		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] '_packet * _queue_Packet::get()' Exception \n") ;
		pTemp = NULL ;
	}
	unlock() ;
	
	return pTemp ;
	
}



