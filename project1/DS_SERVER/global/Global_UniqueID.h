#include <queue>
#include <vector>

using namespace std;


enum{
	_NONE_,
	_READY_,
	_USE_,
} ;
struct UniqueID_Info {
	u_short UID ;
	char	StateFlag ;	// 
	DWORD	time ;
} ;

class _j_UniqueIDQueue 
{
private :	
	unsigned int threadID ;		
	HANDLE m_hThread ;
	static unsigned int __stdcall  th_UidManagerThread( LPVOID lp )	;

protected:

	UniqueID_Info * _UniqueID_Info ;

	queue<UniqueID_Info*> m_UniqueIDPool ;
	queue<UniqueID_Info*> m_ReadyQueue ;	

	unsigned short m_tempUID ;
	CTraceAbleLock m_csUniq;
#define uid_lock()		m_csUniq.tlock( __FILE__, __LINE__ ) ;
#define uid_unlock()	m_csUniq.tunlock( __FILE__, __LINE__ ) ;



public:	
	_j_UniqueIDQueue() 
	{		
		_UniqueID_Info = new UniqueID_Info[_UNIQUEID_MAX_COUNT] ;
		assert( _UniqueID_Info ) ;
		memset( _UniqueID_Info, 0, sizeof(UniqueID_Info) * _UNIQUEID_MAX_COUNT ) ;
	}
	~_j_UniqueIDQueue() 
	{
		CloseHandle( m_hThread ) ;	
		delete [] _UniqueID_Info ;
	}

	static DWORD get_time_by_sec() { return GetTime_ByIntSec() ; } ;

	bool Initalize(){		
				
		uid_lock() ;
		for(u_short i = 1 ; i < _UNIQUEID_MAX_COUNT ; i++) {
			_UniqueID_Info[i].UID = i ;
			_UniqueID_Info[i].StateFlag = _NONE_ ;
            
			m_UniqueIDPool.push( &_UniqueID_Info[i] ) ;
		}
		
		uid_unlock() ;

		m_hThread = (HANDLE)::_beginthreadex(0, 0, th_UidManagerThread, this, 0, &threadID);
		if (m_hThread == INVALID_HANDLE_VALUE)
		{		
			return false ;
		}		
		return true ;
	}

	// main functions
	unsigned short	GetUID() ;	
	bool UseUID(unsigned short uid) ;
	void RecycleUID(unsigned short uid) ;		// 서버 이동시 임시로 이곳에 등록했다가 사용한다.

	void CheckReady() ;	// 현재 시간과 비교해서 오래 된것은 지운다.
	
	
private:
};