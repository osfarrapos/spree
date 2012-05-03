#include "..\stdafx.h"

#include "Global_UniqueID.h"


unsigned short	_j_UniqueIDQueue::GetUID() 
{
	if( m_UniqueIDPool.empty() ) {
		return 0 ;
	}

	UniqueID_Info * pUIDInfo ;	
	uid_lock() ;
	//pUIDInfo = m_UniqueIDPool.back() ;
	//m_UniqueIDPool.pop_back() ;		
	pUIDInfo = m_UniqueIDPool.front() ;
	m_UniqueIDPool.pop() ;
	

	if( pUIDInfo )
	{
		pUIDInfo->StateFlag = _READY_ ;
		pUIDInfo->time = _j_UniqueIDQueue::get_time_by_sec() ;
		m_ReadyQueue.push( pUIDInfo ) ;	
		
	}	
	uid_unlock() ;

	return pUIDInfo->UID ;	
}




bool _j_UniqueIDQueue::UseUID(unsigned short uid) // private
{
	if( uid == 0 || uid > _UNIQUEID_MAX_COUNT || _UniqueID_Info[uid].StateFlag != _READY_ ) 
		return false ;

    _UniqueID_Info[uid].StateFlag = _USE_ ;	
	return true ;
}
	
void _j_UniqueIDQueue::RecycleUID(unsigned short uid)
{
	if( uid == 0 || uid > _UNIQUEID_MAX_COUNT || _UniqueID_Info[uid].StateFlag != _USE_	) return ;

	_UniqueID_Info[uid].StateFlag = _READY_ ;	
	_UniqueID_Info[uid].time = _j_UniqueIDQueue::get_time_by_sec() ;

	uid_lock() ;
	TRY
	{
		m_ReadyQueue.push( &_UniqueID_Info[uid] ) ;	
	}
	CATCH_ALL
	{
		::PrintConsole( "[EXCEPTION] %s, %d \n" __FILE__, __LINE__ ) ;
	}
	
	uid_unlock() ;
}


void _j_UniqueIDQueue::CheckReady()
{
	DWORD thisTime = _j_UniqueIDQueue::get_time_by_sec() ;
	UniqueID_Info * pUIDInfo ;

		
	uid_lock() ;
	TRY{
		while ( !m_ReadyQueue.empty() ) {
			pUIDInfo = m_ReadyQueue.front() ;
			if( pUIDInfo && pUIDInfo->time + _UNIQUE_ID_LIVE_TIME_SEC < thisTime )
			{
				if( pUIDInfo->StateFlag == _READY_ )
				{
					pUIDInfo->StateFlag = _NONE_ ;
					//m_UniqueIDPool.push_back( pUIDInfo ) ;					
					m_UniqueIDPool.push( pUIDInfo ) ;
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[INFO]tempUID (%u) recycle \n", pUIDInfo->UID) ;
#endif			
				}
				m_ReadyQueue.pop() ;
			}
			else
			{
				break ;
			}
		}
	}
	CATCH_ALL {
		m_ReadyQueue.pop() ;
		::PrintConsole("[EXCEPTION] _j_UniqueIDQueue::CheckReady(int thisTime) \n") ;
	}
	uid_unlock() ;		
	
	
}

unsigned int __stdcall _j_UniqueIDQueue::th_UidManagerThread(LPVOID lp )
{

	_j_UniqueIDQueue * pThis = (_j_UniqueIDQueue*)lp ;
	
	while ( pThis ) {	

		TRY {			
			pThis->CheckReady() ;			
		}
		CATCH_ALL {
		}


		Sleep(100) ;		
	}
	::PrintConsole("DWORD WINAPI th_UidManagerThread( LPVOID lp ) Exit\n") ;
	printf("DWORD WINAPI th_UidManagerThread( LPVOID lp ) Exit\n") ;
	return 0 ;	
}
