/*================================================================================

CTraceAbleLock

이 클래스는 크리티컬섹션을 용이하게 사용 가능하도록 만들었습니다.
데드락이 걸렸을경우 발생 경로를 추적 가능하며 어플리케이션에 생성되어 있는
크리티컬 섹션들의 상태를 확인할 수 있습니다.
최적화된 어플리케이션에서는  _CRITICAL_SECTION_DEBUG 옵션을 빼고 컴파일 하시기 
바랍니다. _CRITICAL_SECTION_DEBUG 옵션은 좀더 자세하게 락을 추적하게 됨으로
IO 부하가 다소 발생합니다.

by ozzywow


=================================================================================*/


#pragma once

#include <list>
using namespace std ;

class CTraceAbleLock
{

private:

	CRITICAL_SECTION 	cs ;
	int					m_lockingCount ;
	char				m_CurlockingPos[512] ;
	char				m_LastlockingPos[512] ;
	int					m_iBadCheck ;

	// 새로 생성된 객체는 이 리스트에 삽입되어 관리됩니다. 객체를 생성하기 아래 리스트를 선언해야 합니다.
	static	list<CTraceAbleLock*>	g_LockList ;

public:	
	static list<CTraceAbleLock*> * GetLockListPtr() { return &g_LockList ; }
	static void Destroy()
	{		
		CTraceAbleLock * pCurLock = NULL ;				

		int i = 0 ;
		for( list<CTraceAbleLock*>::iterator itr = g_LockList.begin() ; itr != g_LockList.end(); ++itr)
		{
			pCurLock = *itr ;
			delete pCurLock ;
		}
	}


	CTraceAbleLock() : m_lockingCount(0) , m_iBadCheck( 0 )
	{		
#if (_WIN32_WINNT > 0x0403)
		if( InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 ) == false )
		{
			throw "" ;
		}
#else
		InitializeCriticalSection( &cs ) ;
#endif

		memset( m_CurlockingPos, 0, 512 ) ;
		memset( m_LastlockingPos, 0, 512 ) ;

		
		// 생성될때마다 리스트에 삽입됩니다.
		g_LockList.push_back( this ) ;
		
	} ;
	
	~CTraceAbleLock()
	{

		// 소멸된다고 해서 특별히 하는일은 없습니다. 리스트에도 그대로 남아 있습니다.
		// 락이 소멸되는 시점은 어플리케이션이 종료될때라고 가정하기 때문입니다.
		// 사실 이 부분은 유연한 설계라고 볼 수 없군요.

		try{		
			DeleteCriticalSection( &cs ) ; 
		}
		catch (...) {
		}		
	} ;		
	

protected:


public:
	// 락을 호출한 위치를 인수로 넘겨받습니다. 데드락 발생시 경로추적을 위해서죠.
	void tlock( char * file, int line ) 
	{ 			
		try{
			if( m_iBadCheck != 0 )	// 초기화 되지 않은 객체를 사용할 경우를 검사한다.
			{
				printf( "[lock_exception] %s, %d \n", file, line ) ;
				return ;
			}

#ifdef _CRITICAL_SECTION_DEBUG
			// 버퍼에 최종 호출 위치를 저장할 뿐입니다. 만약 아래 EnterCriticalSection() 에서 데드락에 빠졌다면
			// 이 버퍼를 확인해서 최종 호출지점을 찾으세요.
			sprintf(m_CurlockingPos, "ready: file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;		
#endif

			EnterCriticalSection(&cs) ;


			// 락킹된 카운트 입니다. 한 쓰레드에서 락을 두번 걸경우는 데드락이 아니죠.. 하지만 교착상태에 빠질
			// 확률이 크기때문에 조심해야 합니다. 이 카운트는 그런 락킹을 찾아내는데 유용하게 쓰일겁니다.
			++m_lockingCount ;		
			sprintf(m_CurlockingPos, "complete: file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;
#ifdef _CRITICAL_SECTION_DEBUG
			if( m_lockingCount > 1 )
			{
				printf( m_CurlockingPos ) ;
			}
#endif
#ifdef _CRITICAL_SECTION_DEBUG
			sprintf( m_LastlockingPos, "last(lock): file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;
#endif
		}
		catch (...) {
			printf("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}

	} ;

	// unlock과 크게 다를것이 없기때문에 설명은 생략합니다.
	void tunlock( char * file, int line ) 
	{ 
		try{

			if( m_iBadCheck != 0 )	// 초기화 되지 않은 객체를 사용할 경우를 검사한다.
			{
				printf( "[lock_exception] %s, %d \n", file, line ) ;
				return ;
			}

			--m_lockingCount ;
#ifdef _CRITICAL_SECTION_DEBUG
			sprintf(m_CurlockingPos, "file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;
#endif
#ifdef _CRITICAL_SECTION_DEBUG
			if( m_lockingCount < 0 )
			{
				printf( m_CurlockingPos ) ;
			}
#endif

			LeaveCriticalSection(&cs) ; 
#ifdef _CRITICAL_SECTION_DEBUG
			sprintf( m_LastlockingPos, "last(unlock): file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;
#endif
		}
		catch (...) {
			printf("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}		
	} ;	

	// 아래 메쏘드를 통해서 현제 락 상태를 파악하시면 됩니다.

	// 현제 걸린 락킹 카운트
	inline int GetCurCount() { return m_lockingCount ; } ;
	// 데드락이 걸렸다면 락킹 호출지점
	inline char * GetCurLockingPos() { return m_CurlockingPos ; } ;
	// 데드락이 걸렸다면 현제 호출 이전의 호출지점
	inline char * GetLastLockingPos() { return m_LastlockingPos ; } ;
};

