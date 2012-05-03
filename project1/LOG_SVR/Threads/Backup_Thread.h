#pragma once

#pragma warning(disable:4786)

//#define _PERMISSIBLE_MAX_USER				12000	// DS_SERVER 에서 허용하는 최대 유저다..(최대 캐릭이랑 다름)


#include <queue>

using namespace std;

DWORD WINAPI th_LogWriteTimer( LPVOID lp ) ;

struct _Temp_timerentry {	
	void *					pQueue ;
	void *					pNode ;
	short					Section ;		// 어떤 로그인지 구분
};

class _j_Templet_Timer
{

protected:

	inline void lock()	{ EnterCriticalSection(&cs) ; }
	inline void unlock(){ LeaveCriticalSection(&cs) ; }
	
		
private:
	queue<_Temp_timerentry>	m_TimerQueue ;
	CRITICAL_SECTION cs ;
	int	m_iMaxSize ;			// 큐에 이거 이상 쌓이지 않도록 처리
	
public:
	_j_Templet_Timer(int iMaxSize) : m_iMaxSize(iMaxSize)
	{
		if( InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 )  == FALSE ) {
			printf( "[INIT ERROR] Party Timer cs Error Critical Section\n" ) ;
		}
	}
	~_j_Templet_Timer() {
		DeleteCriticalSection( &cs ) ;
	}
	
	bool createThread( int iCount )						// Begin Thread
	{
		HANDLE hTh ;
		DWORD dwID ;

		for(int i = 0; i< iCount ; i++){
			hTh = BEGINTHREADEX( NULL, 0, th_LogWriteTimer, (LPVOID)this, 0, &dwID ) ;
			if( hTh == INVALID_HANDLE_VALUE ) {
				return false ;
			}
		}
		
		
#ifdef _TEST_
		printf("[PARTY] Begin Timer thread ( %d ) \n", _THREADNUMBER_BACKUP_ ) ;
#endif
		CloseHandle( hTh ) ;
		
		return true ;
	}


	bool Get_TimeMessage(_Temp_timerentry & en)
	{
		lock() ;
		if( m_TimerQueue.empty() )
		{
			unlock() ;
			return false ;
		}
		else
		{
			en = m_TimerQueue.front() ;
			m_TimerQueue.pop() ;
			unlock() ;
			return true ;
		}
		unlock() ;
		return false ;
		
	}


	void send_Message( _Temp_timerentry& en ) 
	{
		
		lock() ;
		try {
			// 큐가 무한으로 넘치는걸 방지.
			if( m_iMaxSize > m_TimerQueue.size() )
			{
				m_TimerQueue.push( en ) ;
			}			
		}
		catch ( ... ) {
			::PrintConsole( "[EXCEPTION] Party Timer - send_message\n" ) ;
		}
		unlock() ;
	}

	int Get_TimeMsgCount() { return m_TimerQueue.size() ; }	
	
	
} ;









