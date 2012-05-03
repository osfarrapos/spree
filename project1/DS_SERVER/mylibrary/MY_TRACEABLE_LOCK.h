/*================================================================================
CRITACAL_SECTION TRACE Util
class CTraceAbleLock

설명
CRITACAL_SECTION 의 상태를 추적한다.
lock/unlock 이 발생할때마다. 발생위치를 저장하고 카운팅해주므로
dead lock 이 발생했을때 dead lock 발생 위치와 원인을 파악하기 용이하다.

by. ozzywow 

=================================================================================*/


#pragma once

#include <list>
using namespace std ;


class CTraceAbleLock
{

private:

	CRITICAL_SECTION 	cs ;
	int					m_iLockingCount ;					// lockking count
	char				m_cCurlockingPos[512] ;				// 현재 locking 위치
	char				m_cLastlockingPos[512] ;				// 마자막 locking 위치
	int					m_iBadCheck ;						// check sum data

	static	list<CTraceAbleLock*>	g_LockList ;			// 등록된 Lock list

public:	
	static list<CTraceAbleLock*> * GetLockListPtr() { return &g_LockList ; }
	// 등록된 lock을 모두 해제한다.(프로그램 종료시)
	static void Destroy()
	{		
		CTraceAbleLock * pCurLock = NULL ;				

		int i = 0 ;
		for( list<CTraceAbleLock*>::iterator itr = g_LockList.begin() 
			; itr != g_LockList.end()
			; ++itr)
		{
			pCurLock = *itr ;
			delete pCurLock ;
		}
	}


	CTraceAbleLock() : m_iLockingCount(0) , m_iBadCheck( 0 )
	{		
#if (_WIN32_WINNT > 0x0403)
		if( InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 ) == false )
		{
			throw "" ;
		}
#else
		InitializeCriticalSection( &cs ) ;
#endif

		memset( m_cCurlockingPos, 0, 512 ) ;
		memset( m_cLastlockingPos, 0, 512 ) ;	
		
		g_LockList.push_back( this ) ;						// CTraceAbleLock 객체가 하나 생성될때마다 LockList 에 넣어준다
	} ;
	
	~CTraceAbleLock()
	{	
		try
		{	
			DeleteCriticalSection( &cs ) ; 
		}
		catch (...) 
		{		
		}		
	} ;		
	

protected:


public:
	// lock
	inline void tlock( const char * cFileName, const int iLine ) 
	{ 			
		try
		{
			if( m_iBadCheck != 0 )	// 초기화 되지 않은 객체를 사용할 경우를 검사한다.
			{
				printf( "[lock_exception] %s, %d \n", cFileName, iLine ) ;
				return ;
			}

			sprintf(m_CurlockingPos, "ready(lock): file:%s, line:%d, count:%d\n", cFileName, iLine, m_lockingCount ) ;		

			EnterCriticalSection(&cs) ;

			++m_iLockingCount ;		
			//sprintf(m_CurlockingPos, "complete: file:%s, line:%d, count:%d\n", cFileName, iLine, m_lockingCount ) ;
#ifdef _CRITICAL_SECTION_DEBUG
			// locking count 가 1보다 클경우 출력해준다
			if( m_iLockingCount > 1 )
			{
				printf( m_cCurlockingPos ) ;
			}
#endif

			sprintf( m_cLastlockingPos, "last(lock): file:%s, line:%d, count:%d\n", cFileName, iLine, m_lockingCount ) ;
		}
		catch (...) 
		{
			printf("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}

	} ;

	// unlock
	inline void tunlock(  const char * cFileName, const int iLine ) 
	{ 
		try
		{

			if( m_iBadCheck != 0 )	// 초기화 되지 않은 객체를 사용할 경우를 검사한다.
			{
				printf( "[lock_exception] %s, %d \n", cFileName, iLine ) ;
				return ;
			}

			--m_iLockingCount ;
			sprintf(m_cCurlockingPos, "ready(unlock): file:%s, line:%d, count:%d\n", cFileName, iLine, m_lockingCount ) ;
#ifdef _CRITICAL_SECTION_DEBUG
			// locking count 가 0보다 작을경우 출력해준다
			if( m_iLockingCount < 0 )
			{
				printf( m_cCurlockingPos ) ;
			}
#endif

			LeaveCriticalSection(&cs) ; 

			sprintf( m_cLastlockingPos, "last(unlock): file:%s, line:%d, count:%d\n", cFileName, iLine, m_lockingCount ) ;
		}
		catch (...) 
		{
			printf("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}		
	} ;	

	// Lock 객체 검사하는 함수
	inline int GetCurCount() { return m_iLockingCount ; } ;
	inline char * GetCurLockingPos() { return m_cCurlockingPos ; } ;
	inline char * GetLastLockingPos() { return m_cLastlockingPos ; } ;
};

