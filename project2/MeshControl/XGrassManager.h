// XGrassManager.h: interface for the _XGrassManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XGRASSMANAGER_H__2AD63101_9ED9_427B_99B5_917802B05D82__INCLUDED_)
#define AFX_XGRASSMANAGER_H__2AD63101_9ED9_427B_99B5_917802B05D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XBipedController.h"
#include "XAnimMeshContainer.h"

#include "XObjectManager.h"
#include "XRecyclingArray.h"

#include <vector>
#include <list>

#define GRASS_MESH_COUNT	15

class _XGrassManager  
{
public:
	typedef std::vector<_XAnimMeshAbstractClass*>		svdef_XAnimMeshAbstractClass;
	typedef std::vector<_XModelDescriptor*>				svdef_XModelDescriptor;
	typedef std::list<_XRealTimeObject*>				sldef_XRealTimeObject;

public:

	int								m_nGrassTypeCount;
	_XAnimMeshAbstractClass*		m_pGrassMesh;
	_XModelDescriptor*				m_pGrassModelDescriptor;
	_XBipedController*				m_pGrassBiped;

	_XDataPool<_XRealTimeObject>	m_Pool;

	sldef_XRealTimeObject			m_slRenderObject;

	FLOAT							GRASS_ALPHA_START;
	FLOAT							GRASS_ALPHA_END;
	FLOAT							GRASS_ALPHA_DIST;

public:
	_XGrassManager();
	virtual ~_XGrassManager();

	BOOL	DeleteData( void );
	BOOL	CreateData( void );

	void	Render(const D3DXVECTOR3& vCameraPos);

public:
	_XRealTimeObject*	New_RealTimeObject()								{ return m_Pool.New(); }

	void				ResetCount()
	{
		m_Pool.Reset();
		m_slRenderObject.clear();
	}

	int					GetGrassTypeCount()										{ return m_nGrassTypeCount; }
	void				InsertObject(_XRealTimeObject* pObject)				{ m_slRenderObject.push_back(pObject); }
};

#endif // !defined(AFX_XGRASSMANAGER_H__2AD63101_9ED9_427B_99B5_917802B05D82__INCLUDED_)
