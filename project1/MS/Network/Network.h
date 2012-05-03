#pragma once

#ifndef _NETWORK_H_AWEGFASDFAWE
#define _NETWORK_H_AWEGFASDFAWE

#include "..\User\User.h"
#include "..\Global\MSPacket_Msg.h"
#include "..\lib_ND_Net\socket_pooling.h"



class _h_DS_Com : public ND_Net::CBaseUser
{
	

private:
	int m_id ;
	//CRITICAL_SECTION	cs ;
	//TLock  * const m_pLock ;

public:
	
	//char			m_cCharacName[13] ;	
	_FriendInfo *	m_pFriendInfo ;	
	MMSG_FRIEND_STATE_REPORT msgFriendStateReport ;

private:
	CTraceAbleLock m_Lock ;
#define ds_lock()		m_Lock.tlock(__FILE__, __LINE__)
#define ds_unlock()		m_Lock.tunlock(__FILE__, __LINE__)
	


public:

	void Release()
	{

		ds_lock() ;

		try{
			m_id = 0 ;
			
			//memset( m_cCharacName, 0, 13 ) ;
			memset( &msgFriendStateReport, 0, sizeof(msgFriendStateReport) ) ;
			msgFriendStateReport.sLength = sizeof(MMSG_FRIEND_STATE_REPORT) ;
			msgFriendStateReport.sCrypto = false ;
			msgFriendStateReport.sCompressed = false ;
			msgFriendStateReport.cMessage = MMSG_NO_FRIEND_STATE_REPORT_S_C ;
			
			m_pFriendInfo = NULL ;
			
			CBaseUser::m_pNetIO = NULL ;
		}
		catch (...) {
		}

		

		ds_unlock() ;
	}

	_h_DS_Com() 
	{

		/*
		assert( m_pLock ) ;
		if( m_pLock == NULL )
		{
			throw ;
		}
		*/

		/*
		if( InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 ) == false )
		{
			throw ;
		}
		*/

		//Release() ;
	}
	~_h_DS_Com()
	{
		//delete m_pLock ;
		//DeleteCriticalSection( &cs ) ; 
	}

	//inline void lock() { EnterCriticalSection( &cs ) ; }
	//inline void unlock() { LeaveCriticalSection( &cs ) ; }
	

	BOOL CreateUser( int id, ND_Net::CSocketBuffer * pIO ) 
	{
		if( pIO )
		{
			ds_lock() ;
			try{
				m_id = id ;
				SetNetBuffer( pIO ) ;
				pIO->SetUser( dynamic_cast<ND_Net::CBaseUser*>(this) ) ;
			}
			catch (...) {
				printf("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
			}			
			ds_unlock() ;
			return TRUE ;
		}
		else
			return FALSE ;
	}
	
	/* =====================================================================================
	parsing() : parse recv data
	===================================================================================== */
	bool Parsing( DWORD recvbytes ) ;
	
	
	void OnConnectionClose() ;
	
	void DisConnect()
	{
		//lock() ;
		try{			
			// 소켓이 재사용 되도록..
			if( m_pNetIO && m_id > 0 && m_pNetIO->m_server )
			{
				//shutdown( m_pNetIO->socket, SD_BOTH ) ;
				m_pNetIO->m_server->DisconnectForReuse( m_pNetIO ) ;
			}			
		}
		catch (...) {			
			printf("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}
		//unlock() ;
	}
	
	SOCKET get_sock()	{ return m_pNetIO->socket ; }
	int get_id()		{ return m_id ; }

} ;


class myServer : public ND_Net::CServerSocket
{
public:
	long g_ncount1 ;
		
	
	myServer() : ND_Net::CServerSocket()
	{ g_ncount1 = 0 ; }
	
	virtual int Worker() ;	
	inline int GetCount() 
	{ 
		if( g_ncount1 >= INT_MAX )
		{
			g_ncount1 = 0 ;
		}
		return InterlockedIncrement( &g_ncount1 ) ;
	}	

	inline size_t GetQueueCount() { return m_UsingPool.size() ; }
	
} ;



#endif


