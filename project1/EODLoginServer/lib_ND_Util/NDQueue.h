#ifndef _ND_SERVER_QUEUE_USINN_NODE_POOL_
#define _ND_SERVER_QUEUE_USINN_NODE_POOL_

//#include "VMManager.h"

namespace ND_Util {

struct _Node 
{
	_Node * pNext ;
} ;


template <typename Node>
class CQueueFixedSize
{
public:
	CQueueFixedSize() ;
	~CQueueFixedSize() ;
	BOOL CreateQueue() ;


	void push_back( Node * node ) ;
	Node * pop() ;

	inline int size() { return m_nSize ; }

protected:
	int			m_nSize ;				// 현재 크기
	int			m_nCapacity ;			// 최대 용량

	_Node *		m_pFirst ;
	_Node *		m_pEnd ;				// 
} ;


template<typename Node>
CQueueFixedSize<Node>::CQueueFixedSize()
{
	m_nSize = 0 ;
	m_nCapacity = 0 ;
	m_pFirst = NULL ;
	m_pEnd = NULL ;
}


template<typename Node>
CQueueFixedSize<Node>::~CQueueFixedSize()
{
}

template<typename Node>
BOOL CQueueFixedSize<Node>::CreateQueue()
{
	m_nSize = 0 ;
	m_nCapacity = 0 ;
	m_pFirst = NULL ;
	m_pEnd = NULL ;
	
	return TRUE ;
}

template<typename Node>
void CQueueFixedSize<Node>::push_back( Node * node )
{
	if( size() )
	{
		m_pEnd->pNext = node ;
		node->pNext = NULL ;
		m_pEnd = node ;
		++m_nSize ;
	}
	else 
	{
		m_pFirst = m_pEnd = node ;
		m_nSize = 1 ;
	}
}


template<typename Node>
Node * CQueueFixedSize<Node>::pop()
{
	Node * p = NULL ;
	if( size() )
	{
		p = static_cast<Node*>(m_pFirst) ;
		m_pFirst = m_pFirst->pNext ;

		--m_nSize ;

		if( 0 == size() )			// 비었다...
		{
			m_pEnd = NULL ;
		}
	}
#ifdef _DEBUG
	else	// 비었을 때...
	{
		OutputDebugString( "queue over : new\n ") ;
	}
#endif

	return p ;
}


} // ND_Util
#endif