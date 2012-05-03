//***********************************************************************************
//	File		:	_XDataPool.h
//	Desc		:
//	Begin		:	2004.9.23
//	Copyright	:	¨Ï 2004 by Mahwang CO., Ltd
//	Author		:	Mahwang
//	Update		:
//***********************************************************************************
#ifndef _XDATAPOOL_H_
#define _XDATAPOOL_H_

#include <vector>
#include <list>
#include <algorithm>

struct DELETEOBJECT
{
	template <typename T>
	void operator()(T* ptr) const 
	{
		delete ptr;
	}
};


template<typename ENTITY>
class _XDataPool
{
private:
protected:
	typedef std::vector<ENTITY*>	sv_ENTITY;
	sv_ENTITY						m_svStack;

	ENTITY*							m_pReservePool;
	int								m_nReserveCount;

	typedef std::list<ENTITY*>		sldef_ENTITY;
	sldef_ENTITY					m_slAddPool;

	int								m_nTotalCount;

	sv_ENTITY::iterator				m_CurIt;

public:

	_XDataPool()
	{
		m_pReservePool		= NULL;
		m_nReserveCount		= 0;
		m_nTotalCount		= 0;
		m_CurIt				= NULL;
	}
	virtual	~_XDataPool()
	{
		assert(m_svStack.empty() == true);
		assert(m_pReservePool == NULL);
		assert(m_slAddPool.empty() == true);
	}
	void DeleteData()
	{
		assert(m_nTotalCount == m_svStack.size());

		m_svStack.clear();

		SAFE_DELETE_ARRAY(m_pReservePool);
		m_nReserveCount = 0;

		if (!m_slAddPool.empty())
			std::for_each(m_slAddPool.begin(), m_slAddPool.end(), DELETEOBJECT());
		m_slAddPool.clear();

		m_nTotalCount		= 0;
	}

	void Reserve(int count)
	{
		assert(m_svStack.empty() == true);
		assert(m_slAddPool.empty() == true);

		assert(m_pReservePool == NULL);
		assert(m_nReserveCount == 0);

		assert(m_nTotalCount == 0);

		m_pReservePool = new ENTITY[count];
		m_nTotalCount = count;

		m_svStack.reserve(count);
		for (int i = 0; i < count; ++ i)
		{
			m_svStack.push_back(&m_pReservePool[i]);
		}
	}

	void Reset()
	{
		m_CurIt = m_svStack.begin();
	}

	virtual ENTITY* New()
	{
		ENTITY* pEntity = NULL;
		if (m_CurIt != m_svStack.end())
		{
			pEntity = *m_CurIt;
			++m_CurIt;
			return pEntity;
		}

		pEntity = new ENTITY;
		m_slAddPool.push_back(pEntity);
		m_svStack.push_back(pEntity);
		++ m_nTotalCount;

		m_CurIt = m_svStack.end();
		return pEntity;
	}

	int		size()								{ return m_svStack.size(); }
	int		reserve_size()						{ return m_nReserveCount; }
	int		totoal_size()						{ return m_nTotalCount; }
};

#endif