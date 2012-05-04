/*

class CThread
{
private:
	HANDLE		m_hThread ;

public :
	CThread() : m_hThread(INVALID_HANDLE_VALUE)
	{
	}

	~CThread()
	{
		if( m_hThread != INVALID_HANDLE_VALUE )
		{
			::CloseHandle( m_hThread ) ;
		}
	}

	HANDLE GetHandle() const  	{ return m_hThread ; }

	bool CreateThread( void *pFunc)
	{
		if( m_hThread == INVALID_HANDLE_VALUE )
		{
			u_int threadID = 0 ;

			m_hThread = ::_beginthreadex(0, 0, pFunc, (void*)this, 0, &threadID ) ;

			if( m_hThread == INVALID_HANDLE_VALUE )
			{
				return false ;
			}
			return true ;
		}
		else
		{
			return false ;
		}
	}

	void Terminate(	DWORD exitCode = 0 )
	{
		if (!::TerminateThread(m_hThread, exitCode))
		{
			// TODO we could throw an exception here...
		}
	}
};
*/



#include <queue>
using namespace std ;

template <typename T>
class CEventTimer : public CNDThread
{
protected :
private:
	priority_queue<T>	m_EventEntryQueue ;
	CRITICAL_SECTION	m_cs ;

	inline void lock() { EnterCriticalSection( &m_cs ) ; }
	inline void unlock() { LeaveCriticalSection( &m_cs ) ; }
	
	static unsigned long __stdcall th_EventTimerThread( LPVOID lp ) ;

public :

	CEventTimer()
	{
#if (_WIN32_WINNT > 0x0403)
		if( FALSE == InitializeCriticalSectionAndSpinCount( &m_cs, (0x80000000)|2000 ) )
		{
			throw "" ;
		}
#else
		InitializeCriticalSection( &m_cs ) ;
#endif
	}
	virtual ~CEventTimer() 
	{
		DeleteCriticalSection( &m_cs ) ;
	}


	virtual int Run( void ) = 0 ;
	//{
	//	Worker() ;
	//}
	
	//virtual int Worker() = 0 ;

	inline bool GetMessage( DWORD time, T& entry )
	{
		lock() ;
		
		if( m_EventEntryQueue.empty() ) {
			unlock() ;
			return false ;
		}
		entry = m_EventEntryQueue.top() ;
		if( entry.dwTime <= time ) {
			m_EventEntryQueue.pop() ;
			unlock() ;
			return true ;
		}		
		unlock() ;
		return false ;
	}

	inline void SendMessage( T& entry)
	{
		lock() ;
		m_EventEntryQueue.push( entry ) ;		
		unlock() ;
	}
};