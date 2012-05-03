#pragma once

#include <list>
using namespace std ;

//class CTraceAbleLock ;
//extern list<CTraceAbleLock*>	g_LockList ;

class CTraceAbleLock
{

private:

	CRITICAL_SECTION 	cs ;
	int					m_lockingCount ;
	char				m_CurlockingPos[512] ;
	char				m_LastlockingPos[512] ;

	static list<CTraceAbleLock*>	g_LockList ;

public:	

	static list<CTraceAbleLock*> * GetLockListPtr() { return &g_LockList ; }

	CTraceAbleLock() : m_lockingCount(0) 
	{		
		if( InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 ) == false )
		{
			throw ;
		}

		memset( m_CurlockingPos, 0, 512 ) ;
		memset( m_LastlockingPos, 0, 512 ) ;

		//tlock(__FILE__, __LINE__ ) ;
		//tunlock(__FILE__, __LINE__ ) ;
		
		g_LockList.push_back( this ) ;
		
	} ;
	
	~CTraceAbleLock()
	{
		//static int i = 0 ;
		try{
			//++i ; 
			DeleteCriticalSection( &cs ) ; 
		}
		catch (...) {
			//printf("!%d", i) ;
		}		
	} ;	
	

protected:


public:
	void tlock( char * file, int line ) 
	{ 	
		sprintf(m_CurlockingPos, "ready: file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;		
		try{			
			EnterCriticalSection(&cs) ; 			
		}
		catch (...) {
			printf("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}
		++m_lockingCount ;		
		sprintf(m_CurlockingPos, "complete: file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;
		if( m_lockingCount > 1 )
		{
			printf( m_CurlockingPos ) ;
		}
		
		sprintf( m_LastlockingPos, "last(lock): file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;

	} ;
	void tunlock( char * file, int line ) 
	{ 			
		--m_lockingCount ;
		sprintf(m_CurlockingPos, "file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;

		if( m_lockingCount < 0 )
		{
			printf( m_CurlockingPos ) ;
		}

		try{
			LeaveCriticalSection(&cs) ; 
		}
		catch (...) {
			printf("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}

		sprintf( m_LastlockingPos, "last(unlock): file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;

		
	} ;	

	inline int GetCurCount() { return m_lockingCount ; } ;
	inline char * GetCurLockingPos() { return m_CurlockingPos ; } ;
	inline char * GetLastLockingPos() { return m_LastlockingPos ; } ;
};

