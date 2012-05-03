// XCollectionManager.h: interface for the _XCollectionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCOLLECTIONMANAGER_H__0DA607A9_F668_4B6F_A297_E800D2956473__INCLUDED_)
#define AFX_XCOLLECTIONMANAGER_H__0DA607A9_F668_4B6F_A297_E800D2956473__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XCollectionItem.h"

#include <list>
using namespace std;

struct _s_Collection_Package
{
	int		nNPCId;
	int		nCount;
	vector<int>	vCollectionId;

	_s_Collection_Package() : nNPCId(0), nCount(0)
	{
	}
};

typedef map<int, _s_Collection_Package*> _map_CollectionPackage;

class _XCollectionManager  
{
public:
	list <_XCollectionItem*>		m_listCollectionItem;	// 수집 리스트

#ifdef _XTS_COLLECTION_PACKAGE_071023_KUKURI
	_map_CollectionPackage			m_mapCollectionPackage;
#endif
	
public:
	_XCollectionManager();
	virtual ~_XCollectionManager();
	
	void	Release(void);
	
	BOOL	LoadCollectionItem(LPCTSTR filename);
	BOOL	LoadCollectionItem(FILE* fileptr);

};

#endif // !defined(AFX_XCOLLECTIONMANAGER_H__0DA607A9_F668_4B6F_A297_E800D2956473__INCLUDED_)
