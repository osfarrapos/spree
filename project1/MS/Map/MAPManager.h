#pragma once

#pragma warning(disable:4786)

//#define _PERMISSIBLE_MAX_USER				12000	// DS_SERVER 에서 허용하는 최대 유저다..(최대 캐릭이랑 다름)


#include <string>
#include <queue>
#include <map>
#include <list>
using namespace std;




// 사용하지 않는 Node 를 Queue에 넣어서 관리한다.
class _j_MemeberQueue
{

	
protected:
	
	//CRITICAL_SECTION	m_csMemeberQueue ;
	CTraceAbleLock  m_Lock ;

	queue<_FriendInfo*> m_NodeQueue ;
	map<std::string, _FriendInfo*> m_FriendMap ;
	//map<std::string, _FriendInfo*>::iterator m_FriendMapIter ;
	typedef map<std::string, _FriendInfo*>::iterator FriendMapIter ;

	
	_FriendInfo * m_pFriendInfo ;
	//list<_Member>::iterator m_MemberListIter ;
	//list<_Member>::iterator m_GroupListIter ;

#define userpool_lock()		m_Lock.tlock(__FILE__, __LINE__)
#define userpool_unlock()	m_Lock.tunlock(__FILE__, __LINE__)


	//inline void Lock() { EnterCriticalSection( &m_csMemeberQueue ); } 
	//inline void UnLock() { LeaveCriticalSection( &m_csMemeberQueue ); }


public:

	//_Member * pNodePtr ;
	

	_j_MemeberQueue( const size_t size) : m_pFriendInfo(NULL)
	{
		/*
		assert( m_pLock ) ;
		if( m_pLock == NULL )
		{
			throw ;
		}

  */
		/*
		if( InitializeCriticalSectionAndSpinCount( &m_csMemeberQueue, (0x80000000)|2000 )  == FALSE ) {
			printf("[error] _j_MemeberQueue::InitializeCriticalSectionAndSpinCount \n") ;
			throw ;
		}
		*/
		m_FriendMap.clear() ;

		if( Create( size ) == false )
		{
			throw ;
		}
	}
	~_j_MemeberQueue() {				
		if( m_pFriendInfo )
		{
			delete [] m_pFriendInfo ;			
		}
		//DeleteCriticalSection(&m_csMemeberQueue) ;		
	};
		

	bool		Create( const size_t size ) ;	// Node 저장 공간을 확보하고 포인터를 Queue에 등록시킨다.

	// Queue 
	_FriendInfo *	GetNode(char * cName) ;						// Queue에 있는 Node포인터를 반환받는다.
	void			PutNode(_FriendInfo * Node) ;					// 다쓴 Node 포인터를 Queue에 반환한다.
	_FriendInfo *	FindNode(char * cName) ;
	_FriendInfo *	MoveNext( _FriendInfo * p ) ;
	_FriendInfo *	GetBegin() ;
	
	
	// Map
	void			DeleteMap(char * cName) ;

	// List
	bool			AddFriend(char * cName, char * cToName) ;
	bool			DelFriend(char * cName, char * cToName) ;
	bool			TrdFriend(char * cName, char * cToNmae, char * cGroup) ;
	//bool			GetFriendList(char * cName, char * cList) ;
	//bool			Login(char * cName) ;
	bool			Logout(char * cName) ;

	bool			UpdateState(char * cName, char cState) ;
//	bool			ChatGroupChoice(char * cName, char * cList) ;
	

	u_int			GetCurMemberCount() 
	{	
		size_t size ;
		userpool_lock() ;
		try{
			size = m_FriendMap.size() ; 
		}
		catch (...) {
		}
		userpool_unlock() ;
		return size ;
	}


};


//=====================================================================

class _j_BackupReady
{
protected : 
	queue<_FriendInfo*>	backupQueue ;
	_FriendInfo * pTempFrindInfo ;

	//CRITICAL_SECTION m_csBackup;	// 백업	
	CTraceAbleLock  m_Lock ;

#define backup_lock()		m_Lock.tlock(__FILE__, __LINE__)
#define backup_unlock()		m_Lock.tunlock(__FILE__, __LINE__)
	//inline void Lock() { EnterCriticalSection( &m_csBackup ) ;	} 
	//inline void Unlock() { LeaveCriticalSection( &m_csBackup ) ; }
	
	
public:
	
		
	_j_BackupReady() 
	{
		/*
		if( InitializeCriticalSectionAndSpinCount( &m_csBackup, (0x80000000)|2000 ) == FALSE ) {
			printf( " Error Critical Section - m_csBackup\n" ) ;
			throw ;
		}
		*/
		/*
		assert( m_pLock ) ;
		if( m_pLock == NULL )
		{
			throw ;
		}
		*/
		pTempFrindInfo = NULL ;			
		
	}
	
	~_j_BackupReady() {
		//DeleteCriticalSection( &m_csBackup) ;		
		//delete m_pLock ;
	};
	
	
	// 추가
	inline void Insert(_FriendInfo * pInfo) {
		backup_lock() ;
		try{
			backupQueue.push(pInfo) ;
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}		
		backup_unlock() ;
	}

	// 꺼내기..
	inline _FriendInfo * GetNode() {
		_FriendInfo * pTemp = NULL ;

		backup_lock() ;
		try{
			if( backupQueue.empty() == false ) {
				pTemp = backupQueue.front() ;
				backupQueue.pop() ;				
			}			
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] inline _FriendInfo * GetNode() \n" ) ;
			pTemp = NULL ;
		}
		backup_unlock() ;

		return pTemp ;		
	} 

	inline int	GetCount() {
		size_t size ;
		backup_lock() ;
		try
		{
			size = backupQueue.size()  ;
		}
		catch (...) {
		}
		backup_unlock() ;
		return size ;
	} 	
	
	
} ;


class _j_ComQueue
{
protected : 
	queue<_h_DS_Com *>	ComQueue ;
	_h_DS_Com * pDSCom ;
	

	//CRITICAL_SECTION m_csCom;	// 백업	
	//TLock  * const m_pLock ;
	CTraceAbleLock m_Lock ;
	
#define dspool_lock()		m_Lock.tlock(__FILE__, __LINE__)
#define dspool_unlock()		m_Lock.tunlock(__FILE__, __LINE__)
	//inline void Lock() { EnterCriticalSection( &m_csCom ) ;	} 
	//inline void Unlock() { LeaveCriticalSection( &m_csCom ) ; } 

public:
	
	
	
	_j_ComQueue( const size_t size ) : pDSCom( NULL )
	{
		/*
		if( InitializeCriticalSectionAndSpinCount( &m_csCom, (0x80000000)|2000 ) == FALSE ) {
			printf( " Error Critical Section - m_csBackup\n" ) ;
			throw ;
		}*/
		/*
		assert( m_pLock ) ;
		if( m_pLock == NULL )
		{
			throw ;
		}
		*/

		printf( " 1 \n" ) ;

		if( Create( size ) == false )
		{
			throw ;
		}
	
	}
	
	~_j_ComQueue() {

		if( pDSCom )
			delete [] pDSCom ;
		//delete m_pLock ;
		//DeleteCriticalSection( &m_csCom) ;		
	}

	bool		Create( const size_t size ) ;	// Node 저장 공간을 확보하고 포인터를 Queue에 등록시킨다
	
	// 추가
	inline void Insert(_h_DS_Com * pCom) 
	{		
		if( pCom == NULL ) return ;
		pCom->Release() ;

		dspool_lock() ;
		try{
			ComQueue.push(pCom) ;
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}		
		dspool_unlock() ;
	}
	
	// 꺼내기..
	inline _h_DS_Com * GetNode() 
	{
		_h_DS_Com * pDSCom = NULL ;			

		dspool_lock() ;
		try{
			if( ComQueue.empty() == false ) {
				pDSCom = ComQueue.front() ;
				if( pDSCom )
				{
					ComQueue.pop() ;
				}				
			}		
			
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] _h_DS_Com * GetNode() \n") ;
			pDSCom = NULL ;
		}
		dspool_unlock() ;

		if( pDSCom )
		{
			pDSCom->Release() ;
		}

		return pDSCom ;
	} 
	
	inline int	GetCount() 
	{
		size_t size ;
		//dspool_lock() ;
		try{
			size = ComQueue.size() ;
		}
		catch (...) {
		}
		//dspool_unlock() ;
		return size ;
	}
	
	
	
} ;



DWORD WINAPI th_TimeOut( LPVOID lp ) ;
struct _Temp_timerentry {	
	_h_DS_Com *				pDS_Com ;	
	DWORD					time ;
};

class _j_Templet_Timer
{
protected:	

	queue<_Temp_timerentry>	m_TimerQueue ;
	//CRITICAL_SECTION cs ;	
	CTraceAbleLock  m_Lock ;

	//inline void lock()	{ EnterCriticalSection(&cs) ; }
	//inline void unlock(){ LeaveCriticalSection(&cs) ; }
	#define timer_lock()		m_Lock.tlock(__FILE__, __LINE__)
	#define timer_unlock()		m_Lock.tunlock(__FILE__, __LINE__)
	
public:
	bool bThreadRun ;

	_j_Templet_Timer()
	{
		/*
		if( InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 )  == FALSE ) {
			printf( "[INIT ERROR] Party Timer cs Error Critical Section\n" ) ;
		}
		*/

		/*
		assert( m_pLock ) ;
		if( m_pLock == NULL )
		{
			throw ;
		}
		*/

		bThreadRun = true ;
	}
	~_j_Templet_Timer() {
		bThreadRun = false ;
		//DeleteCriticalSection( &cs ) ;
		//delete m_pLock ;
	}
	
	bool create()						// Begin Thread
	{
		HANDLE hTh ;
		DWORD dwID ;
		
		for(int i = 0; i< 1; i++){
			hTh = BEGINTHREADEX( NULL, 0, th_TimeOut, (LPVOID)this, 0, &dwID ) ;
			if( hTh == INVALID_HANDLE_VALUE ) {
				return false ;
			}
		}
		

		CloseHandle( hTh ) ;
		
		return true ;
	}
	
	
	bool Get_TimeMessage( DWORD time, _Temp_timerentry & en)
	{	
		bool bRet = false ;
		timer_lock() ;
		try{
			if( m_TimerQueue.empty() )
			{
				bRet = false ;
			}
			else
			{	
				en = m_TimerQueue.front() ;
				if( en.time <= time )
				{				
					m_TimerQueue.pop() ;
					bRet = true ;
				}			
			}
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}		
		timer_unlock() ;
		
		return bRet ;
		
	}
	
	
	void send_Message( _Temp_timerentry& en ) 
	{
		timer_lock() ;		
		try {
			m_TimerQueue.push( en ) ;
		}
		catch ( ... ) {
			::PrintConsole( "[EXCEPTION] Party Timer - send_message\n" ) ;
		}
		timer_unlock() ;
	}
	
} ;























