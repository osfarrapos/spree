// XResourceManager.h: interface for the _XResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XRESOURCEMANAGER_H__D050BF98_8BC5_44A9_B403_7E6706B297D5__INCLUDED_)
#define AFX_XRESOURCEMANAGER_H__D050BF98_8BC5_44A9_B403_7E6706B297D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XResourceItem.h"

#include <list>
using namespace std;

class _XResourceManager  
{
public:
	
	int								m_TotalCountResourceItem;	// ÀÚ¿ø ÃÑ°¹¼ö 
	_XResourceItem*					m_arrayResourceItem;		// ÀÚ¿ø

	list <_XResourcePackage*>		m_listResourcePackage;
	list <_XResourceGroupItem*>		m_listResourceGroupItem;

public:
	_XResourceManager();
	virtual ~_XResourceManager();
	
	void	Release(void);
	
	BOOL	LoadResourceItem(LPCTSTR filename);
	BOOL	LoadResourceItem(FILE* fileptr);

};

#endif // !defined(AFX_XRESOURCEMANAGER_H__D050BF98_8BC5_44A9_B403_7E6706B297D5__INCLUDED_)
