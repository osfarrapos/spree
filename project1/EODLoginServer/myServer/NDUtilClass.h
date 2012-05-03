#ifndef _THIS_IS_ND_UTIL_CLASS_COLLECTION_2004_0707_ASLKJFNLVLJI__
#define _THIS_IS_ND_UTIL_CLASS_COLLECTION_2004_0707_ASLKJFNLVLJI__

#include <windows.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>

class CNDThread 
{
public :
	
	CNDThread()	:  m_hThread(INVALID_HANDLE_VALUE)
	{		
	}
	
	~CNDThread()
	{
		if (m_hThread != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(m_hThread);
		}
		
		m_hThread = INVALID_HANDLE_VALUE ;
	}
	
	
	HANDLE GetHandle() const
	{
		return m_hThread;
	}
	
	bool Start()
	{
		if (m_hThread == INVALID_HANDLE_VALUE)
		{
			unsigned int threadID = 0;
			
			m_hThread = (HANDLE)::_beginthreadex(0, 0, ThreadFunction, this, 0, &threadID);
			
			if (m_hThread == INVALID_HANDLE_VALUE)
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
	
	bool Wait() const
	{
		if (!Wait(INFINITE))
		{
			return false ;
		}
		return true ;
	}
	
	bool Wait(DWORD timeoutMillis) const
	{
		// TODO base class? Waitable?
		bool ok;
		
		DWORD result = ::WaitForSingleObject(m_hThread, timeoutMillis);
		
		if (result == WAIT_TIMEOUT)
		{
			ok = false;
		}
		else if (result == WAIT_OBJECT_0)
		{
			ok = true;
		}
		else
		{
			ok = false ;
		}
		
		return ok;
	}
	
	static unsigned int __stdcall ThreadFunction(void *pV)
	{
		int result = 0;
		
		CNDThread* pThis = (CNDThread*)pV;
		
		if (pThis)
		{
			try
			{
				result = pThis->Run();
				pThis->Release() ;				
			}
			catch(...)
			{
				printf( "CThread::Run Exception\n" ) ;
			}
		}
		
		return result;
	}
	
	void Terminate(
		DWORD exitCode /* = 0 */)
	{
		if (!::TerminateThread(m_hThread, exitCode))
		{
			// TODO we could throw an exception here...
		}
	}

	void Release() {
		m_hThread = INVALID_HANDLE_VALUE ;
	}
	
private :
	
	virtual int Run() = 0;
	
	HANDLE m_hThread;
	
	// No copies do not implement
	CNDThread(const CNDThread &rhs);
	CNDThread &operator=(const CNDThread &rhs);
		
};


class CLockObject
{
public:
	CLockObject( CRITICAL_SECTION& cs ) : m_cs(cs)
	{
		EnterCriticalSection( &m_cs ) ;
	}
	~CLockObject()
	{
		LeaveCriticalSection( &m_cs ) ;
	}
	
protected:
	CRITICAL_SECTION&	m_cs ;
	
} ;

class CTrace
{
public:
	CTrace( char * pname )
	{
		if( pname )
		{
			m_pFName = new char[strlen(pname) + 1] ;
			strcpy( m_pFName, pname ) ;

			printf( "Enter Function : %s\n", m_pFName ) ;
		}
	}

	~CTrace()
	{
		if( m_pFName )
		{
			printf( "Leave Function : %s\n", m_pFName ) ;
			delete [] m_pFName ;
		}
	}

protected:
	char * m_pFName ;
	
} ;


template <typename T>
struct _node 
{
	T	node ;
	_node *		prev ;
	_node *		next ;
} ;

template <typename T>
class NDQueueMemoryPool ;


template <typename T>
class NDQueue
{
public:
	NDQueue() : m_pHead(NULL), m_pTail(NULL), m_iUsedCount(0) 
	{
		m_pHead = new _node<T> ;
		m_pTail = new _node<T> ;

		m_pHead->prev = m_pHead ;
		m_pHead->next = m_pTail ;
		m_pTail->prev = m_pHead ;
		m_pTail->next = m_pTail ;
	}

	~NDQueue() 
	{
		delete m_pHead ;
		delete m_pTail ;
	}


	void init_queue() 
	{ 
		m_pHead->prev = m_pHead ;
		m_pHead->next = m_pTail ;
		m_pTail->prev = m_pHead ;
		m_pTail->next = m_pTail ;

		m_iUsedCount = 0 ;
	}

	void put( T * p ) 
	{
#ifdef TRACE_FUCTION
		CTrace trace( "NDQueue::put()" ) ;
#endif
		_node<T> * pNode = m_pPool->get_free_node() ;
		if( pNode )
		{
			// 데이타 셋팅
			pNode->node = *p ;

			m_pTail->prev->next = pNode ;
			pNode->prev = m_pTail->prev ;
			m_pTail->prev = pNode ; 
			pNode->next = m_pTail ;

			++m_iUsedCount ;
		}
	}

	void pop()
	{
		if( m_bLog )
			printf( "pop()" ) ;
#ifdef TRACE_FUCTION
		CTrace trace( "NDQueue::get()" ) ;
#endif
		_node<T> * t = m_pHead->next ;
		if( t == m_pTail )
			return ;

		m_pHead->next = t->next ;
		t->next->prev = m_pHead ;

		--m_iUsedCount ;

		m_pPool->add_free_node( t ) ;
	}

	T& top()
	{
		return m_pHead->next->node ;
	}

	bool empty() 
	{
		if( m_pHead->next == m_pTail )
			return true ;
		else 
			return false ;
	}

	void release() 
	{
		while( !empty() )
			pop() ;
	}

	int get_using_count() { return m_iUsedCount ; }


	//////////////////////////////////////////////////////////////////////////
	static bool InitializeMemoryPool( int nCount )
	{
		m_pPool = new NDQueueMemoryPool<T>( nCount ) ;
		if( NULL == m_pPool )
			return false ;

		return true ;
	}
	
	static void ReleaseMemoryPool()
	{
		if( m_pPool )
			delete m_pPool ;
		m_pPool = NULL ;
	}
	//////////////////////////////////////////////////////////////////////////
	

	static int m_bLog ;

private:
	_node<T> *	m_pHead ;
	_node<T> *	m_pTail ;

	int			m_iUsedCount ;

	static NDQueueMemoryPool<T> *	m_pPool ;
} ;



template <typename T>
class NDQueueMemoryPool
{
public:
	NDQueueMemoryPool( int nPoolCount ) : m_pNodeMemory(NULL), m_iUsedCount(0) 
	{
		if( nPoolCount > 0 )
		{
			m_pHead = new _node<T> ;
			m_pTail = new _node<T> ;

			m_pHead->prev = m_pHead ;
			m_pHead->next = m_pTail ;
			m_pTail->prev = m_pHead ;
			m_pTail->next = m_pTail ;

			if( FALSE == InitializeCriticalSectionAndSpinCount( &m_cs, 0x80001000 ) )
			{
				throw ;
			}

			m_pNodeMemory = new _node<T>[nPoolCount] ;

			for( int i = 0 ; i < nPoolCount ; ++i )
				add_free_node( &m_pNodeMemory[i] ) ;

			m_iUsedCount = 0 ;
			m_iTotalCount = nPoolCount ;
		}
		else
			throw ;
	}
	~NDQueueMemoryPool()
	{
		if( m_pNodeMemory )
			delete [] m_pNodeMemory ;
		m_pNodeMemory = NULL ;
	}

	void add_free_node( _node<T> * pNode ) 
	{
#ifdef TRACE_FUCTION
	//	CTrace trace( "NDQueueMemoryPool::add_free_node()" ) ;
#endif
		CLockObject lock( m_cs ) ;
		//
		if( pNode )
		{
			try
			{
				m_pTail->prev->next = pNode ;
				pNode->prev = m_pTail->prev ;
				m_pTail->prev = pNode ; 
				pNode->next = m_pTail ;
				
				--m_iUsedCount ;
			}
			catch ( ... ) 
			{
				printf("%s:%d> Exception here!\n", __FILE__, __LINE__);
			}
		}
	}

	_node<T> * get_free_node()
	{
#ifdef TRACE_FUCTION
		CTrace trace( "NDQueueMemoryPool::get_free_node()" ) ;
#endif
		CLockObject lock( m_cs ) ;

		_node<T> * t = NULL ;
		try
		{
			if( empty() )
			{
				if( false == expand_free_node() )
					return NULL ;
			}

			t = m_pHead->next ;
			
			m_pHead->next = t->next ;
			t->next->prev = m_pHead ;
			
			++m_iUsedCount ;
		}
		catch ( ... ) 
		{
			printf("%s:%d> Exception here!\n", __FILE__, __LINE__);
		}


#ifdef _DEBUG
		static DWORD nCount = 0 ;

		if( 0 == ++nCount % 50000 )
			printf( "Memory Pool Count : %6d Total = %6d\n", m_iUsedCount, m_iTotalCount ) ;
#endif
		
		return t ;
	}

	bool empty() 
	{
		CLockObject lock( m_cs ) ;

		if( m_pHead->next == m_pTail )
			return true ;
		else 
			return false ;
	}


	bool expand_free_node() 
	{
		try
		{
			// 100개 단위로 만든다.
			_node<T> * p = new _node<T>[128] ;
			if( p )
			{
#ifdef _DEBUG
				printf( "expand node!!\n" ) ;
#endif
				_node<T> * pCur ;
				for( int i = 0 ; i < 128 ; ++i )
				{
					pCur = &p[i] ;

					m_pTail->prev->next = pCur ;
					pCur->prev = m_pTail->prev ;
					m_pTail->prev = pCur ; 
					pCur->next = m_pTail ;
				}

				m_iUsedCount = m_iTotalCount ;
				m_iTotalCount += 128 ;

				return true ;
			}
			else
				return false ;
		}
		catch ( ... ) 
		{
			// 
			printf("%s:%d> Exception here!\n", __FILE__, __LINE__);
		}

		return false ;
	}

	/*
	void release_free_list( NDQueue<T> * pQueue )
	{
#ifdef TRACE_FUCTION
		CTrace trace( "NDQueueMemoryPool::release_free_list()" ) ;
#endif
		// 리스트 통째로 넣기.
		try
		{
			_node<T> * pNode = pQueue->get() ;

			while( pNode )
			{
				add_free_node( pNode ) ;
				
				pNode = pQueue->get() ;
			}
		}
		catch ( ... ) 
		{
			//
		}
		
	}
  */

	int	get_using_count() { return m_iUsedCount ; }



private:
	_node<T> *	m_pNodeMemory ;
	int			m_iUsedCount ;

	int			m_iTotalCount ;

	_node<T> *	m_pHead ;
	_node<T> *	m_pTail ;

	CRITICAL_SECTION m_cs ;
} ;


#endif