#pragma once

#pragma warning(disable:4786)

#include <queue>
using namespace std;


template <class Type>
class CTQueue
{
protected : 
	queue<Type*>	ReadyQueue ;
	queue<Type*>	UseQueue ;
	
	const int		m_iMaxQueueSize ;	// queue 의 최대 사이즈
	Type		*	m_pMemHead ;
	

	inline void ReadyLock() { EnterCriticalSection( &m_ReadyQueue_cs ) ; } 
	inline void ReadyUnlock() { LeaveCriticalSection( &m_ReadyQueue_cs ) ; } 	
	
	inline void UseLock() { EnterCriticalSection( &m_UseQueue_cs ) ; } 
	inline void UseUnlock() { LeaveCriticalSection( &m_UseQueue_cs ) ; } 	
		
public:
	
	CRITICAL_SECTION m_ReadyQueue_cs;	// 대기큐 섹션
	CRITICAL_SECTION m_UseQueue_cs;		// 대기큐 섹션
		
	
	CTQueue( int iMaxQueueSize ) : m_iMaxQueueSize(iMaxQueueSize), m_pMemHead(NULL)
	{
#if (_WIN32_WINNT > 0x0403)
		if( InitializeCriticalSectionAndSpinCount( &m_ReadyQueue_cs, (0xf0000000)|2000 ) == FALSE ) {			
			throw ;
		}	
		
		if( InitializeCriticalSectionAndSpinCount( &m_UseQueue_cs, (0xf0000000)|2000 ) == FALSE ) {			
			throw ;
		}	
#else
		InitializeCriticalSection( &m_ReadyQueue_cs ) ;
		InitializeCriticalSection( &m_UseQueue_cs ) ;
#endif

		if( init(iMaxQueueSize) == false ) throw ;

		
	};
	
	~CTQueue() {
		DeleteCriticalSection( &m_ReadyQueue_cs) ;
		DeleteCriticalSection( &m_UseQueue_cs) ;

		delete [] m_pMemHead ;
	};
	
	bool init( int iNodeCount ) 
	{	// 초기화
			
		Type  *	pNode = m_pMemHead = new Type[iNodeCount] ;
		if( pNode == NULL ) 
		{
			return false ;
		}	
		
		
		for( int i=0; i<iNodeCount; i++)
		{
			Ready_Put( pNode ) ;
			pNode++ ;
		}		
		
		return true ;
	}
	
	// 대기 큐에 넣기 
	inline void Ready_Put(Type * pNode) 
	{
		if( pNode )
		{
			ReadyLock() ;
			ReadyQueue.push(pNode) ;
			ReadyUnlock() ;
		}		
	}
	
	// 대기 큐에서 꺼내기
	inline Type * Ready_Pop() {

		Type  *	pNode = NULL ;

		ReadyLock() ;

		try{
			if( ReadyQueue.empty() ) 
			{
				pNode = NULL ;
			}
			else
			{
				pNode = ReadyQueue.front() ;			
				ReadyQueue.pop() ;
				memset( pNode, 0, sizeof(Type) ) ;
			}
		}
		catch (...) 
		{
			::PrintConsole("[EXCEPTION] inline Type * Ready_Pop() \n" ) ;
			pNode = NULL ;
		}

		ReadyUnlock() ;

		return pNode ;

		/*	// 원래 코드
		try{
			ReadyLock() ;
			if( ReadyQueue.empty() ) {
				ReadyUnlock() ;
				return NULL ;
			}
			
			pNode = ReadyQueue.front() ;			
			ReadyQueue.pop() ;
			ReadyUnlock() ;
			memset( pNode, 0, sizeof(Type) ) ;
			return pNode ;
		}
		catch (...) {
			
			ReadyUnlock() ;
			::PrintConsole("[EXCEPTION] inline Type * Ready_Pop() \n" ) ;
			return NULL ;
		}
		*/
		
	} 

	// 사용 큐에 넣기
	inline void Use_Put(Type * pNode) {
		if( pNode ) 
		{
			UseLock() ;
			UseQueue.push(pNode) ;
			UseUnlock() ;
		}		
	}
	
	// 사용 큐에서 꺼내기
	inline Type * Use_Pop() {

		Type  *	pNode = NULL ;

		UseLock() ;

		try{
			if( UseQueue.empty() ) 
			{
				pNode = NULL ;
			}
			else
			{
				pNode = UseQueue.front() ;			
				UseQueue.pop() ;
			}
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] inline Type * Use_Pop() \n" ) ;
			pNode = NULL ;
		}
		UseUnlock() ;

		return pNode ;

		/*	// 원래 코드 :20060418
		try{
			UseLock() ;
			if( UseQueue.empty() ) {
				UseUnlock() ;
				return NULL ;
			}
			
			pNode = UseQueue.front() ;			
			UseQueue.pop() ;
			UseUnlock() ;
			return pNode ;
		}
		catch (...) {
			UseUnlock() ;
			::PrintConsole("[EXCEPTION] inline Type * Use_Pop() \n" ) ;
			return NULL ;
		}
		*/
		
	} 


	
	inline int	GetReadyCount() {
		return ReadyQueue.size() ;
	} ;
	inline int	GetUseCount() {
		return UseQueue.size() ;
	} ;	
	
	
};





