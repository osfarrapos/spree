#ifndef _THIS_IS_MYSERVER_TEST_SDLKFJLSKDJ__
#define _THIS_IS_MYSERVER_TEST_SDLKFJLSKDJ__

#include "..\lib_ND_Net\socket_pooling.h"

class _h_User : public ND_Net::CBaseUser
{
//protected:
public:

	_h_User()
	{
		m_id = 0 ;
	}
	virtual ~_h_User()
	{
		m_id = 0 ;
	}
	
	int				m_id ;
	u_char			m_seq ;

	char			cID[en_max_lil+1] ;		// account
	u_int			iIp ;
	char			cStep ;			// 패킷을 하나 받을때다 스텝을 1씩 증가시켜서 		
	char			cClass ;
	bool			bIsInMap ;		// 맵에 등록 되었는지..
	char			cSelectForest ;	// 선택한 서버군..
	
#ifdef _METEL_LOGIN_200608_
	BOOL			bPCBang;		// 피씨방 유저인지 아닌지(TRUE = 피씨방 유저)
	METEL_BILLING_INFO	MetelBillingInfo;
#endif //_METEL_LOGIN_200608_
	
	inline void init() {
		m_id = 0 ;
		m_seq = 0 ;
		memset(cID, 0, en_max_lil+1 ) ;
		iIp = 0 ;
		cStep = 0 ;		
		cClass = 0 ;
		bIsInMap = false ;
		cSelectForest = 0 ;
		m_pNetIO = NULL ;

#ifdef _METEL_LOGIN_200608_
		bPCBang = FALSE;
		memset(&MetelBillingInfo, 0, sizeof(METEL_BILLING_INFO));
#endif //_METEL_LOGIN_200608_
	}

	inline void reset()
	{
		m_id = 0 ;
		m_seq = 0 ;
		memset(cID, 0, en_max_lil+1 ) ;
		iIp = 0 ;
		cStep = 0 ;		
		cClass = 0 ;
		bIsInMap = false ;
		cSelectForest = 0 ;
		
#ifdef _METEL_LOGIN_200608_
		bPCBang = FALSE;
		memset(&MetelBillingInfo, 0, sizeof(METEL_BILLING_INFO));
#endif //_METEL_LOGIN_200608_
	}

	inline BOOL CreateUser( int id, ND_Net::CSocketBuffer * pIO ) 
	{
		if( pIO )
		{
			m_id = id ;
			m_seq = m_id % KEY_RANGE ;

			if( m_seq == 0 ) 
				m_seq = 1 ;
#ifdef _METEL_LOGIN_200608_
			bPCBang = FALSE;
			memset(&MetelBillingInfo, 0, sizeof(METEL_BILLING_INFO));
			MetelBillingInfo.iLoginSN = -1;
#endif //_METEL_LOGIN_200608_
			SetNetBuffer( pIO ) ;
			pIO->SetUser( this ) ;

			return TRUE ;
		}
		else
			return FALSE ;
	}

	/* =====================================================================================
	parsing() : parse recv data
	===================================================================================== */
	virtual bool Parsing( DWORD recvbytes ) ;
	void OnConnectionClose() ;
	void DisConnect() ;
	
	
	inline SOCKET get_sock()	{ return m_pNetIO->socket ; }
	inline int get_id()		{ return m_id ; }
} ;

class myServer : public ND_Net::CServerSocket
{
public:
	
	long g_ncount1 ;

	DWORD m_wait_time ;
	
	
	myServer() : ND_Net::CServerSocket()
	{ 
		g_ncount1 = 0 ; 
		m_wait_time = INFINITE ;
		
		set_server_type_ls();
	}
	
	virtual int Worker() ;
	
	inline u_int GetCount() 
	{ 
		if( g_ncount1 >= INT_MAX )
		{
			g_ncount1 = 0 ;
		}
		return InterlockedIncrement( &g_ncount1 ) ;
	}	

	inline size_t GetQueueCount() { return m_UsingPool.size() ;	}
	
} ;



class PSServer : public ND_Net::CServerSocket
{
public:

	long g_ncount2 ;

	DWORD m_wait_time ;

	PSServer() : ND_Net::CServerSocket()
	{ 
		g_ncount2 = 0 ; 
		m_wait_time = INFINITE ;

		set_server_type_ps();
	}
	
	
	virtual int Worker() ;	

	inline int GetCount()  
	{ 
		if( g_ncount2 >= INT_MAX )
		{
			g_ncount2 = 0 ;
		}
		return InterlockedIncrement( &g_ncount2 ) ;
	}	

	inline size_t GetQueueCount() { return m_UsingPool.size() ;	}
} ;




#endif






