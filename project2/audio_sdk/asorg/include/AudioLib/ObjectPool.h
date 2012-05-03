/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef OBJECTPOOL_H__
#define OBJECTPOOL_H__

#include "Audio.h"

#define DEFINE_POOL(ClassName) \
	public: \
	void SetPoolIndex(int32 iIndex) { m_iPoolIndex = iIndex; } \
	int32 GetPoolIndex()			{ return m_iPoolIndex; } \
	static bool ReservePool(uint32 nNumObjs)	{ return s_ObjPool.Reserve(nNumObjs); } \
	static ClassName* CreateObject() \
	{  return s_ObjPool.CreateObject();  } \
	static void DestroyObject(ClassName* pObj) \
	{  s_ObjPool.DestroyObject(pObj);  } \
	static void TermPool() \
	{  s_ObjPool.Term();  } \
	friend class ObjectPool<ClassName>;\
	private: \
	int32 m_iPoolIndex; \
	static ObjectPool<ClassName> s_ObjPool;

#define IMPLEMENT_POOL(ClassName) \
	ObjectPool<ClassName> ClassName::s_ObjPool; 

namespace Audio
{

template <class T>
class ObjectPool
{
public:
	ObjectPool()	{}
	~ObjectPool()	{ Term(); }

	bool Reserve(uint32 nNumObjs);
	void Term();

	T* CreateObject();
	void DestroyObject(T*);
	void DestroyAllObjects();


private:

	std::vector<T*>		m_aPool;
	std::vector<int32>	m_aFree;
};

template<class T>
bool ObjectPool<T>::Reserve(uint32 nNumObjs)
{
	T* pT;
	m_aPool.reserve(nNumObjs);
	m_aFree.reserve(nNumObjs);
	int iPoolStart = m_aPool.size();
	for(int i = 0; i < nNumObjs; i++)
	{
		pT = new T;
		if(!pT)
			return false;
		pT->SetPoolIndex(-1);
		m_aPool.push_back(pT);
		m_aFree.push_back(iPoolStart + i);
	}
	return true;
}


template<class T>
void ObjectPool<T>::Term()
{
	for(int i = 0; i < m_aPool.size(); i++)
	{
		//if( m_aPool[i] )
		{
			delete m_aPool[i];
			//m_aPool[i] = NULL;
		}
	}
	m_aPool.clear();
	m_aFree.clear();
}


template<class T>
T* ObjectPool<T>::CreateObject()
{
	if(!m_aFree.size())
		Reserve(m_aPool.size() * 2 + 1);
	int32 iIndex = m_aFree.back();
	m_aFree.pop_back();
	T* pObj = m_aPool[iIndex];
	assert(pObj->GetPoolIndex() == -1);
	pObj->SetPoolIndex(iIndex);
	return pObj;
}


template<class T>
void ObjectPool<T>::DestroyObject(T* pObj)
{
	assert(pObj->GetPoolIndex() != -1);
	m_aFree.push_back(pObj->GetPoolIndex());
	pObj->SetPoolIndex(-1);
}


}; // namespace Audio




#endif // OBJECTPOOL_H__
