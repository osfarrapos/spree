#pragma once

#include "..\MyServer\myServer.h"
#include "../myServer/MY_QUEUE.H"

#pragma warning(disable:4786)
#include <queue>
#include <map>
using namespace std ;

#define _MAX_PACKET_SIZE		2048
#define _MAX_UDP_PACKET_SIZE	2048
#define _MAX_TCP_PACKET_SIZE	2048


struct _packetUDP {
	SOCKADDR_IN		addr ;
	int				sBufSize ;	
	char			buf[_MAX_UDP_PACKET_SIZE] ;	
};


struct _Packet {
	_h_User *	pUser ;
	char		buf[_MAX_TCP_PACKET_SIZE] ;
	short		len ;
} ;



#define	  MAX_PACKET_COUNT	16384	//이정도면 겹치지 않을만큼 충분할꺼야..



class _queue_Packet : public CRotationQueue<_Packet>
{


public:
	
	_queue_Packet(size_t size) : CRotationQueue<_Packet>(size)
	{		
	}
	~_queue_Packet() 
	{ 
	}	
	
	void insert(_h_User * pUser, char * buf, int size) ;
} ;



class _j_queue_UDP_Packet : public CRotationQueue<_packetUDP>
{

public:

	_j_queue_UDP_Packet(size_t size = 10) : CRotationQueue<_packetUDP>(size)
	{	
	}
	~_j_queue_UDP_Packet() 
	{ 	
	}
	
	void insert( SOCKADDR_IN * addr, char * buf, int bufSize ) ;	
};






#define _MAX_USER_COUNT_	10000


class _j_UserQueue
{
private:

	_h_User * m_pUser ;	// 유저 풀 메모리..

	int m_nMax;	
	int m_nGet;
	
	CRITICAL_SECTION	cs_queue ;
	
	inline void lock() { EnterCriticalSection( &cs_queue ) ; } ;
	inline void unlock() { LeaveCriticalSection( &cs_queue ) ; } ;


	map<int,_h_User*>	m_StandbyUserMap ;
	map<int,_h_User*>::iterator m_itr ;
	CRITICAL_SECTION	cs_map ;
	inline void map_lock() { EnterCriticalSection( &cs_map ) ; }
	inline void map_unlock() { LeaveCriticalSection( &cs_map ) ; }
	

public:	
		 	 
	
	_j_UserQueue( size_t size ) : m_pUser(NULL), m_nMax(size), m_nGet(0)
	{
		m_pUser = new _h_User[size] ;
		

		if( InitializeCriticalSectionAndSpinCount( &cs_queue, (0x80000000)|2000 ) == FALSE ) {
			printf( " Error Critical Section - cs_queue\n" ) ;		
		}		
		if( InitializeCriticalSectionAndSpinCount( &cs_map, (0x80000000)|2000 ) == FALSE ) {
			printf( " Error Critical Section - cs_map\n" ) ;		
		}		
	};
	~_j_UserQueue()
	{
		if( m_pUser )
			delete [] m_pUser ;
		DeleteCriticalSection( &cs_queue ) ;
		DeleteCriticalSection( &cs_map ) ;
	}
	
	inline bool Init() 
	{
		m_StandbyUserMap.clear() ;
		return true ;
	}
	
	
	inline _h_User * GetUser() 
	{
		_h_User * temp ;
		lock() ;
		try{
			temp = &m_pUser[m_nGet % m_nMax] ;		
			m_nGet++;
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] %s, %d\n", __FILE__, __LINE__ ) ;
		}		
		unlock() ;

		temp->init() ;
		return  temp ;		
	}

	inline _h_User * FindUser( const int uid ) 
	{
		_h_User * pTemp = NULL ;

		map_lock() ;
		try{
			m_itr = m_StandbyUserMap.find( uid ) ;
			if( m_itr != m_StandbyUserMap.end() )
			{
				pTemp = m_itr->second ;
			}		
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] %s, %d\n", __FILE__, __LINE__ ) ;
		}
		
		map_unlock() ;

		return pTemp ;		
	}
	
	inline void InsertStandbyMap( _h_User * pUser )
	{
		if( pUser )
		{
			pUser->bIsInMap = true ;	// 이 플래그를 이용해 나중에 맵을 검색하지 않고는 맵에 등록 되었는지를 확인한다.

			map_lock() ;
			try{
				m_StandbyUserMap[pUser->m_id] = pUser ;
			}
			catch (...) {
				::PrintConsole("[EXCEPTION] %s, %d\n", __FILE__, __LINE__ ) ;
			}
			
			map_unlock() ;
		}		
	}

	inline void DeleteStandbyMap( int uid )
	{
		map_lock() ;
		try{
			m_itr = m_StandbyUserMap.find( uid ) ;
			if( m_itr != m_StandbyUserMap.end() )
			{
				m_StandbyUserMap.erase( m_itr ) ; 
			}
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] %s, %d\n", __FILE__, __LINE__ ) ;
		}
		
		map_unlock() ;
	}
	
	
protected:
private:
};




