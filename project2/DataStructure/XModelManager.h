// XModelManager.h: interface for the _XModelManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMODELMANAGER_H__D9F2A480_2AA1_497D_8571_878ADFB95121__INCLUDED_)
#define AFX_XMODELMANAGER_H__D9F2A480_2AA1_497D_8571_878ADFB95121__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XBipedController.h"
#include "XAnimMeshContainer.h"
#include <list>

class _XModelManager  
{
public:
	typedef std::vector<_XAnimMeshAbstractClass*>	svdef_XAnimMeshAbstractClassr;	

	_XAnimMeshAbstractClass*						m_pDefaultAnimMeshAbstractClass;
	svdef_XAnimMeshAbstractClassr					m_svAnimMeshAbstractclassr;

public:
	_XModelManager();
	virtual ~_XModelManager();

	void	CreateData();
	void	DestroyData();

	BOOL	CreateAnimMeshAbstractClass(_XAnimMeshAbstractClass** ppAnimMeshAbstractClass, 
										LPCTSTR lpModelName, LPCTSTR lpLodModelName1 = NULL, LPCTSTR lpLodModelName2 = NULL );
};

#endif // !defined(AFX_XMODELMANAGER_H__D9F2A480_2AA1_497D_8571_878ADFB95121__INCLUDED_)
