// XModelManager.cpp: implementation of the _XModelManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XModelManager.h"

#include "XDataPool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XModelManager::_XModelManager()
{
	m_pDefaultAnimMeshAbstractClass		= NULL;
}

_XModelManager::~_XModelManager()
{

}
void _XModelManager::CreateData()
{
	m_pDefaultAnimMeshAbstractClass = _XAnimMeshAbstractClass::CreateAnimationMesh();
}

void _XModelManager::DestroyData()
{
	std::for_each(m_svAnimMeshAbstractclassr.begin(), m_svAnimMeshAbstractclassr.end(), DELETEOBJECT());
	m_svAnimMeshAbstractclassr.clear();	

	SAFE_DELETE(m_pDefaultAnimMeshAbstractClass);
}

BOOL _XModelManager::CreateAnimMeshAbstractClass( _XAnimMeshAbstractClass** ppAnimMeshAbstractClass, 
												  LPCTSTR lpModelName, LPCTSTR lpLodModelName1, LPCTSTR lpLodModelName2 )
{
	svdef_XAnimMeshAbstractClassr::iterator it;
	for (it = m_svAnimMeshAbstractclassr.begin(); it != m_svAnimMeshAbstractclassr.end(); ++ it)
	{
		if (stricmp((*it)->m_strFileName.c_str(), lpModelName) == 0)
		{
			(*ppAnimMeshAbstractClass) = (*it);
			return FALSE;
		}
	}

	(*ppAnimMeshAbstractClass) = _XAnimMeshAbstractClass::CreateAnimationMesh();
	(*ppAnimMeshAbstractClass)->m_strFileName = lpModelName;

	if( lpLodModelName1 )
	{
		if( strlen( lpLodModelName1 ) )
		{
			(*ppAnimMeshAbstractClass)->m_strLodFileName[0] = lpLodModelName1;

			if( lpLodModelName2 )
			{
				if( strlen( lpLodModelName2 ) )
				{
					(*ppAnimMeshAbstractClass)->m_strLodFileName[1] = lpLodModelName2;
				}
			}
		}
	}
	
	m_svAnimMeshAbstractclassr.push_back((*ppAnimMeshAbstractClass));

	return TRUE;
}