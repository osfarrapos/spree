// SectionList.h: interface for the CSectionList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECTIONLIST_H__50B2B913_6BAA_11D5_A852_00022A009AE8__INCLUDED_)
#define AFX_SECTIONLIST_H__50B2B913_6BAA_11D5_A852_00022A009AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XList.h"
#include "PackFileItem.h"

class CSectionList : public item
{

public:
		
	CSectionList();
	virtual		~CSectionList();

	void		deleteAllItem();
	
	item*		insertItem( CPackFileItem* sectionitem );
	BOOL		deleteItem( int index );

	CPackFileItem*   getStringItem( int index );

	void        setLinkedTreeItem( HTREEITEM treeitem ){ m_linkedTreeItem = treeitem; }

	int			m_itemcount;

	list*		m_itemlist;

	HTREEITEM	m_linkedTreeItem;
};


class CPackage
{

public:

	CPackage();
	virtual ~CPackage();

	void    deleteAllItem();
	
	item*   insertItem( CSectionList* sectionlist );
	BOOL    deleteItem( int index );

	CSectionList* getSectionList( int index );

	int     findLinkedTreeItem( HTREEITEM treeitem );

	int		m_sectioncount;

	list*	m_sectionitemlist;
};



#endif // !defined(AFX_SECTIONLIST_H__50B2B913_6BAA_11D5_A852_00022A009AE8__INCLUDED_)
