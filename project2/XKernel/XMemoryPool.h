//***********************************************************************************
//	File		:	XMemoryPool.h
//	Desc		:
//	Begin		:	2004.9.23
//	Copyright	:	¨Ï 2004 by Mahwang CO., Ltd
//	Author		:	Mahwang
//	Update		:
//***********************************************************************************
#pragma once

#include "XDataPool.h"

#include <crtdbg.h>
#include <stack>
#include <algorithm>

template<typename ENTITY>
class XMemoryPool
{
private:
protected:
//	typedef std::stack<ENTITY*, std::list<ENTITY*>>	sddef_ENTITY;
	typedef std::stack<ENTITY*>	sddef_ENTITY;
	sddef_ENTITY				m_sdStack;

	ENTITY*						m_pReservePool;
	int							m_nReserveCount;

	typedef std::list<ENTITY*>	sldef_ENTITY;
	sldef_ENTITY				m_slAddPool;

	int							m_nTotalCount;

public:

	XMemoryPool()
	{
		m_pReservePool		= NULL;
		m_nReserveCount		= 0;
		m_nTotalCount		= 0;
	}
	virtual	~XMemoryPool()
	{
		_ASSERT(m_sdStack.empty() == true);
		_ASSERT(m_pReservePool == NULL);
		_ASSERT(m_slAddPool.empty() == true);
	}
	void DeleteData()
	{
		_ASSERT(m_nTotalCount == m_sdStack.size());

		while (!m_sdStack.empty())
		{
			m_sdStack.pop();
		}

		if (m_pReservePool != NULL)
		{
			delete[] m_pReservePool;
			m_pReservePool = NULL;
		}
		m_nReserveCount = 0;

		std::for_each(m_slAddPool.begin(), m_slAddPool.end(), DELETEOBJECT());
		m_slAddPool.clear();

		m_nTotalCount		= 0;
	}

	void Reserve(int count)
	{
		_ASSERT(m_sdStack.empty() == true);
		_ASSERT(m_slAddPool.empty() == true);

		_ASSERT(m_pReservePool == NULL);
		_ASSERT(m_nReserveCount == 0);

		_ASSERT(m_nTotalCount == 0);

		m_pReservePool = new ENTITY[count];
		m_nTotalCount = count;

		for (int i = 0; i < count; ++ i)
		{
			m_sdStack.push(&m_pReservePool[i]);
		}
	}

	ENTITY* New()
	{
		ENTITY* pEntity = NULL;
		if (!m_sdStack.empty())
		{
			pEntity = m_sdStack.top();
			m_sdStack.pop();
			return pEntity;
		}

		pEntity = new ENTITY;
		m_slAddPool.push_back(pEntity);
		++ m_nTotalCount;

		return pEntity;
	}

	void Delete(ENTITY* pEntity)
	{
		m_sdStack.push(pEntity);
		_ASSERT(m_sdStack.size() <= m_nTotalCount);
	}

	int		size()								{ return m_sdStack.size(); }
	int		reserve_size()						{ return m_nReserveCount; }
	int		totoal_size()						{ return m_nTotalCount; }
};


