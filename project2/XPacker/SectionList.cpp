// SectionList.cpp: implementation of the CSectionList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XPacker.h"
#include "SectionList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSectionList::CSectionList()
{
	m_itemcount = 0;

	m_itemlist = new list;

	m_linkedTreeItem = NULL;
}

CSectionList::~CSectionList()
{
	m_itemcount = 0;

	if(  m_itemlist ) delete m_itemlist;
}


item* CSectionList::insertItem( CPackFileItem* sectionitem )
{
	if( !sectionitem  || !this->m_itemlist ) return NULL;
	
	m_itemcount++;
	
	this->m_itemlist->resetList();
	
	return this->m_itemlist->insertItem( (item*)sectionitem );
}

void CSectionList::deleteAllItem()
{
	this->m_itemlist->disposeList();
	m_itemcount = 0;
}
	
BOOL CSectionList::deleteItem( int index )
{
	if( index >= m_itemcount ) return FALSE;

	this->m_itemlist->resetList();

	for( int i = 0; i < index; i++ ) this->m_itemlist->nextItem();

	this->m_itemlist->deleteItem( this->m_itemlist->currentItem() );	
	
	m_itemcount--;

	return TRUE;
}

CPackFileItem* CSectionList::getStringItem( int index )
{
	if( index >= m_itemcount ) return NULL;

	this->m_itemlist->resetList();

	for( int i = 0; i < index; i++ ) this->m_itemlist->nextItem();

	return ( (CPackFileItem*)this->m_itemlist->currentItem() );
}


CPackage::CPackage()
{
	m_sectioncount = 0;

	m_sectionitemlist = new list;
}

CPackage::~CPackage()
{
	m_sectioncount = 0;

	if(  m_sectionitemlist ) delete m_sectionitemlist;
}


item* CPackage::insertItem( CSectionList* sectionlist )
{
	if( !sectionlist  || !this->m_sectionitemlist ) return NULL;

	m_sectioncount++;
	
	this->m_sectionitemlist->resetList();
	
	return this->m_sectionitemlist->insertItem( (item*)sectionlist );
}

void CPackage::deleteAllItem()
{
	this->m_sectionitemlist->disposeList();
	m_sectioncount = 0;
}
	
BOOL CPackage::deleteItem( int index )
{
	if( index >= m_sectioncount ) return FALSE;

	this->m_sectionitemlist->resetList();

	for( int i = 0; i < index; i++ ) this->m_sectionitemlist->nextItem();

	this->m_sectionitemlist->deleteItem( this->m_sectionitemlist->currentItem() );	
	
	m_sectioncount--;

	return TRUE;
}

int CPackage::findLinkedTreeItem( HTREEITEM treeitem )
{
	int findindex = -1;

	if( !treeitem || !m_sectioncount ) return findindex;

	this->m_sectionitemlist->resetList();

	findindex = 0;

	do
	{

		if( ((CSectionList*)(this->m_sectionitemlist->currentItem()))->m_linkedTreeItem == treeitem )
		{
			return findindex;
		}
		
		this->m_sectionitemlist->nextItem();

		findindex++;

	}while( !this->m_sectionitemlist->atHeadOfList() );	
	
	return -1;
}

CSectionList* CPackage::getSectionList( int index )
{
	if( index >= m_sectioncount ) return NULL;

	this->m_sectionitemlist->resetList();

	for( int i = 0; i < index; i++ ) this->m_sectionitemlist->nextItem();

	return ( (CSectionList*)this->m_sectionitemlist->currentItem() );		
	
}



