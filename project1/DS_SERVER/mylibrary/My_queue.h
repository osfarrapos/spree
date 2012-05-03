/*================================================================================
 by ozzywow
=================================================================================*/


#pragma once

//#pragma warning(disable:4786)
#include <vector>
#include <queue>
#include <list>
#include <map>

using namespace std ;

template <typename TNode>
class CRotationQueue
{
protected:
	unsigned int m_iput ;
	unsigned int m_iget ;
	const size_t m_nsize ;
	unsigned int m_iTemp ;
	unsigned int m_iCurCount ;
	
	CRITICAL_SECTION	cs ;
	
	TNode *	m_pNode ;
	
	inline void lock() { EnterCriticalSection( &cs ) ; }
	inline void unlock() { LeaveCriticalSection( &cs ) ; }	
	
	
public:
	
	CRotationQueue( size_t size ) : m_iput(0), m_iget(0), m_nsize(size), m_iCurCount(0), m_pNode(NULL) 
	{	
		if( size > 0 )
		{
#if (_WIN32_WINNT > 0x0403)
			if( FALSE == InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 ) )
			{
				throw "" ;
			}
#else
			InitializeCriticalSection( &cs ) ;
#endif
			//m_nsize = size ;
			m_pNode = new TNode[m_nsize] ;
			//memset( m_pNode, 0, sizeof(m_pNode) ) ;
		}
		else
		{
			throw "" ;
		}
		
	}
	
	~CRotationQueue() 
	{ 
		delete [] m_pNode ;
		DeleteCriticalSection( &cs ) ; 
	}	
	
	
	void insert( TNode * pNode ) 
	{
		if( pNode	== NULL ) {
			return;
		}
		
		lock() ;
		
		m_iTemp = (m_iput+1)%m_nsize ;
		if( m_iTemp == m_iget )
		{
			unlock() ;
			return ;
		}
		
		memcpy( &m_pNode[m_iput], pNode, sizeof(TNode) ) ;
		m_iput = m_iTemp ;
		++m_iCurCount ;
		
		unlock() ;	
	}

	TNode * get() 
	{
		TNode * pTemp = NULL ;
		lock() ;	
		
		
		if( m_iCurCount > 0 && m_iget != m_iput )
		{	
			pTemp = &m_pNode[m_iget] ;
			m_iget = (++m_iget) % m_nsize ;
			
			--m_iCurCount ;
		}
		
		unlock() ;
		return pTemp ;
	}
	
	inline bool empty() 
	{ 
		bool bRet ;
		lock() ;
		if( m_iCurCount == 0 )
			bRet = true ;
		else
			bRet = false ;
		unlock() ;

		return bRet ; 
	} ;
	
	inline unsigned int GetCurCount() 
	{ 
		int iRet ;
		lock() ;
		iRet = m_iCurCount ;
		unlock() ;
		return iRet ; 
	} ;
	
} ;

template <typename TNode>
class CSAFE_Vector
{
protected:	
	
	CRITICAL_SECTION	cs ;
	
	inline void lock() { EnterCriticalSection( &cs ) ; } ;
	inline void unlock() { LeaveCriticalSection( &cs ) ; } ;

	vector<TNode>	m_NodePool ;
public:	
		 	 
	
	CSAFE_Vector( size_t size = 0 ) 
	{
				
#if (_WIN32_WINNT > 0x0403)
		if( FALSE == InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 ) )
		{
			throw "" ;
		}
#else
		InitializeCriticalSection( &cs ) ;
#endif

		if( size > 0 )
			m_NodePool.reserve( size ) ;	// 미리 크기를 예약..		
		
	};
	~CSAFE_Vector()
	{
		m_NodePool.clear() ;		
		DeleteCriticalSection( &cs ) ;
	}	
	
	inline TNode get() 
	{
		TNode temp = 0 ;
		lock() ;
		if( !m_NodePool.empty() )
		{
			temp = m_NodePool.back() ;		
			m_NodePool.pop_back() ;
		}		
		unlock() ;		
		return  temp ;		
	}

	inline void insert( TNode node ) 
	{
		lock() ;
		m_NodePool.push_back( node ) ;
		unlock() ;
	}	
};



template <typename TNode>
class CSAFE_Queue
{
protected:	
	
	CRITICAL_SECTION	cs ;
	
	inline void lock() { EnterCriticalSection( &cs ) ; } 
	inline void unlock() { LeaveCriticalSection( &cs ) ; } 
	
	queue<TNode>	m_NodeQueue ;
public:	
	
	
	CSAFE_Queue() 
	{
		
#if (_WIN32_WINNT > 0x0403)
		if( FALSE == InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 ) )
		{
			throw "" ;
		}
#else
		InitializeCriticalSection( &cs ) ;
#endif		
		
	};
	~CSAFE_Queue()
	{		
		DeleteCriticalSection( &cs ) ;
	}	
	
	inline TNode get() 
	{
		TNode temp = 0 ;
		lock() ;
		if( !m_NodeQueue.empty() )
		{
			temp = m_NodeQueue.front() ;
			m_NodeQueue.pop() ;
		}		
		unlock() ;		
		return  temp ;		
	}
	
	inline void insert( TNode node ) 
	{
		lock() ;
		m_NodeQueue.push( node ) ;
		unlock() ;
	}
};

template <typename TNode>
class CPool
{
protected:
	TNode * pMem ;
	TNode * pFirst ;
	TNode * pEnd ;
	size_t	m_size ;

public:
	CPool(size_t size) : pMem(NULL), pFirst(NULL), pEnd(NULL), m_size(0)
	{
		pMem = new TNode[size] ;
		for( u_int i = 0 ; i < size; i++)
		{
			push_back( &pMem[i] ) ;			// 단방향 링크드 리스트를 만든다..			
		}
	}
	~CPool()
	{
		delete [] pMem ;
	}

	inline TNode * GetFirst() {	return pFirst ;	}
	inline TNode * GetLast() {	return pEnd ; }
	inline size_t GetSize() { return m_size ;}

	inline TNode * pop( TNode * p = NULL )			// 풀에서 꺼내줌..
	{
		if( p == NULL ) p = pFirst ;
		if( m_size > 0 )
		{

			TNode * pPrev = NULL ;
			TNode * pTemp = NULL ;
			for( pTemp = pFirst ; pTemp != NULL ; pTemp = pTemp->pNext )
			{				
				if( p == pTemp )						// 조건이 맞을때만 삭제한다.
				{
					if( pPrev == NULL )					// 첫번째 노드
					{
						pFirst = pTemp->pNext ;
					}
					else								// 중간 노드..
					{
						pPrev->pNext = pTemp->pNext ;
					}
					--m_size ;
					break ;
				}
				pPrev = pTemp ;
			}
			
			return pTemp ;			
		}
		else
		{
			return NULL ;
		}	
		
	}

	inline void push_back(void * pNode)	// 풀에 넣어줌..
	{
		TNode * pTemp = (TNode*)pNode ;
		if( m_size == 0 )
		{
			pFirst = pEnd = pTemp ;
			m_size = 1 ;
		}
		else
		{
			pEnd->pNext = pTemp ;
			pTemp->pNext = NULL ;
			pEnd = pTemp ;
			++m_size ;
		}		
	}
	
};



/*

// !! USEFULL HINT !!

#include <My_Queue.h>

typedef CRationalMem<int> NewInt ;

NewInt::RationalNode * NewInt::pFreeList ;

NewInt::newMemPool(1024) ;

NewInt * pMem1 = new NewInt ;
NewInt * pMem2 = new NewInt ;
NewInt * pMem3 = new NewInt ;
delete pMem1 ;
delete pMem3 ;
delete pMem2 ;	
*/
template <typename T>
class CRationalMem : public T
{

private:
	//T Node ;
	

public:	

	CRationalMem * pNext ;
	static CRationalMem *		pNewList ;
	static CRationalMem *		pFreeList ;
	static list<CRationalMem*>	UsingList ;
	static size_t m_size ;


	CRationalMem() : pNext(NULL)
	{		
	}
	~CRationalMem()
	{
	}


	// Allocate memory pool..
	static void newMemPool( size_t size )
	{		
		m_size = size ;
		UsingList.clear() ;

		pNewList = pFreeList = new CRationalMem[size] ;
		for( int i = 0 ; i < size - 1 ; ++i )
		{
			(&pFreeList[i])->pNext = &pFreeList[i+1] ;
		}
		(&pFreeList[size-1])->pNext = NULL ;		
	}

	// DeAllocate momory pool..
	static void deleteMemPool()
	{
		m_size = 0  ;

		delete [] pNewList ;
		pNewList = pFreeList = NULL ;		
	}

	void * operator new(size_t size)
	{
		if( NULL != pFreeList->pNext )
		{
			CRationalMem * pTemp = pFreeList ;			

			pFreeList = pFreeList->pNext ;

			if( pTemp )
			{
				UsingList.push_back( pTemp ) ;
			}

			return pTemp ;			
		}		
		return NULL ;
	}

	void operator delete(void * pDelete, size_t size)
	{
	
		CRationalMem * pTemp = static_cast<CRationalMem*>(pDelete) ;
		if( pTemp )
		{
			pTemp->pNext = pFreeList ;
			pFreeList = pTemp ;
		}
	
		
	}	

	static size_t size() { return m_size ; } 
	
	/*
	CRationalMem & operator=( const CRationalMem & rhs )
	{
		if( this == &rhs )
		{
			return *this ;
		}
		Node = rhs.Node ;
		pNext = rhs.pNext ;

		return *this ;
	}
	*/
	
};