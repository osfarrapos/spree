// XResourceItem.cpp: implementation of the _XResourceItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XResourceItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// class _XResourceItem 
//////////////////////////////////////////////////////////////////////
_XResourceItem::_XResourceItem()
{
	m_ResourceIndex		= 0;
	m_currentcountNeeditem = 0;
	m_totalcountNeeditem = 0;
	memset( m_ResourceName, 0, sizeof(TCHAR)*256 );
	
}

_XResourceItem::~_XResourceItem()
{
//	Release();
}

void _XResourceItem::Release()
{
//	_XCollectionResourceItem*	currentneeditem = NULL;
//	
//	list <_XCollectionResourceItem *>::iterator iter;
//	for(iter = m_listNeedItem.begin() ; iter != m_listNeedItem.end() ; iter++)
//	{
//		currentneeditem  = *iter;
//		if(currentneeditem)
//		{
//			delete currentneeditem;
//		}
//	}	
//	m_listNeedItem.clear();	

}

//////////////////////////////////////////////////////////////////////
// class _XResourcePackage 
//////////////////////////////////////////////////////////////////////
_XResourcePackage::_XResourcePackage()
{
	m_ResourcePackageIndex		= 0;	
}

_XResourcePackage::~_XResourcePackage()
{
//	Release();
}

void _XResourcePackage::Release()
{
//	list <int>::iterator iter_resourceitemindex;
//	for(iter_resourceitemindex = m_listResourceItemIndex.begin() ; iter_resourceitemindex != m_listResourceItemIndex.end() ; )
//	{
//		iter_resourceitemindex = m_listResourceItemIndex.erase(iter_resourceitemindex);
//	}
//	m_listResourceItemIndex.clear();

}


//////////////////////////////////////////////////////////////////////
// class _XResourceGroupItem 
//////////////////////////////////////////////////////////////////////
_XResourceGroupItem::_XResourceGroupItem()
{
	m_ResourceGroupItemIndex		= 0;	
	m_ResourceGroupSubItemIndex		= 0;
	m_ResourcePackageIndex			= 0;
	m_bSelectedGroupSubItem			= false;
	memset( m_ResourceGroupSubItemName, 0, sizeof(TCHAR)*256 );
}

_XResourceGroupItem::~_XResourceGroupItem()
{
//	Release();
}

void _XResourceGroupItem::Release()
{
//	list <int>::iterator iter_packageindex;
//	for(iter_packageindex= m_listResourcePackageIndex.begin() ; iter_packageindex != m_listResourcePackageIndex.end() ; )
//	{
//		iter_packageindex = m_listResourcePackageIndex.erase(iter_packageindex);
//	}
//	m_listResourcePackageIndex.clear();

}






