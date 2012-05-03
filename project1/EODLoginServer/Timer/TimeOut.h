#pragma once

#pragma warning(disable:4786)

#include <queue>
using namespace std;


enum{
	_TM_SESSION_OVERTIME_ = 0,
	_TM_DS_INFO_REQ,
	_TM_DS_CHECK,
	_TM_TEST,
} ;

DWORD WINAPI th_TimeOut( LPVOID lp ) ;
struct _Temp_timerentry {	
	char					cWork ;
	void *					pVoid ;	
	int						iValue ;
	DWORD					time ;
	bool operator<(const _Temp_timerentry& x) const { return time >= x.time ; } 
};

class _j_Templet_Timer
{
protected:
	
public:
	priority_queue<_Temp_timerentry>	m_TimerQueue ;
	CRITICAL_SECTION cs ;
	bool bThreadRun ;
	
public:
	_j_Templet_Timer() 
	{		
		if( InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 )  == FALSE ) {
			printf( "[INIT ERROR] Party Timer cs Error Critical Section\n" ) ;
		}
		bThreadRun = true ;
	}
	~_j_Templet_Timer() {
		bThreadRun = false ;
		DeleteCriticalSection( &cs ) ;
	}
	
	bool create()						// Begin Thread
	{
		HANDLE hTh ;
		DWORD dwID ;
		
		
		hTh = BEGINTHREADEX( NULL, 0, th_TimeOut, (LPVOID)this, 0, &dwID ) ;
		if( hTh == INVALID_HANDLE_VALUE ) {
			return false ;
		}
				
		
		
#ifdef _TEST_
		printf("[PARTY] Begin Timer thread ( %d ) \n", 1 ) ;
#endif
		CloseHandle( hTh ) ;
		
		return true ;
	}
	
	
	bool Get_TimeMessage( DWORD time, _Temp_timerentry & en)
	{			

		bool bRet = false ;

		lock() ;

		try{
			if( m_TimerQueue.empty() )
			{
				bRet = false ;
			}
			else
			{	
				en = m_TimerQueue.top() ;
				if( en.time < time )
				{					
					m_TimerQueue.pop() ;
					bRet = true ;
				}
			}
		}
		catch (...) {
		}		
		
		unlock() ;

		return bRet ;
		
	}
	
	
	void send_Message( _Temp_timerentry& en ) 
	{
		lock() ;
		try {
			m_TimerQueue.push( en ) ;
		}
		catch ( ... ) {
			::PrintConsole( "[EXCEPTION] Party Timer - send_message\n" ) ;
		}
		unlock() ;
	}
	
	size_t GetSize() { return m_TimerQueue.size() ; } ;
	
	
	inline void lock()	{ EnterCriticalSection(&cs) ; }
	inline void unlock(){ LeaveCriticalSection(&cs) ; }
} ;
