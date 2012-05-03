#include "stdafx.h"
#include "XList.h"

_XList::_XList()
{
      anchor = cip = NULL;
	  itemcount = 0;
}

_XList::~_XList()
{
      if(anchor != NULL) disposeList();
	  itemcount = 0;
}

_XItem *_XList::insertItem(_XItem *ip)
{
     if(ip == NULL) return NULL;

	 itemcount++;

     if(anchor == NULL) return anchor = cip = ip;
     return ip->link(cip);
}

_XItem *_XList::removeItem(_XItem *ip)
{
     if(ip == NULL) return NULL;
     if(ip->getright() == ip) anchor = cip = NULL;
     else {
          if(ip == anchor) anchor = anchor->getright();
          if(cip == ip) cip = cip->getright();
     }
	 itemcount--;
     return ip->unlink();
}

BOOL _XList::deleteItem(_XItem *ip)
{
	 if(ip == NULL) return FALSE;
     if(ip->getright() == ip) anchor = cip = NULL;
     else {
          if(ip == anchor) anchor = anchor->getright();
          if(cip == ip) cip = cip->getright();
     }
	 
	 itemcount--;

     delete ip;
	 ip = NULL;
	 return TRUE;
}

BOOL _XList::deleteItem(int Pos)
{	
	return deleteItem( this->getItem( Pos ) );
}


_XItem *_XList::prevItem(void)
{
     if(cip != NULL) cip = cip->getleft();
     return cip;
}

_XItem *_XList::nextItem(void)
{
     if(cip != NULL) cip = cip->getright();
     return cip;
}

void _XList::disposeList(void)
{
     while(!listEmpty())
           delete removeItem(currentItem());
}

void _XList::currentItemSwap( void )
{	 
	 _XItem *tip = cip;
	 
	 removeItem(cip);
	 
	 nextItem();
	 
	 insertItem( tip );
	 cip = tip;
	 
}

_XItem *_XList::getItem( int Pos )
{
	 resetList();
	 for( int i=0; i<Pos; i++ ) nextItem();
	 return cip;
}

int _XList::searchItem( _XItem* ip )
{
	 BOOL i=0,Find = FALSE;
	 resetList();
	 do
	 {	 
		 if( ip == cip )
		 {
			 Find = TRUE;
		     break;
		 }
		 i++;
		 nextItem();
	 } while( !atHeadOfList() );

	 if( Find ) return i;
	 else return -1;
}

BOOL _XList::movePosition( int curPosition, int newPosition )
{	
	if( curPosition == newPosition ) return FALSE;	
	
	_XItem* curItem = getItem( curPosition );

	if( !curItem ) return FALSE;
	
	if( firstItem()==curItem )
	{
		if( newPosition == 0 ) return FALSE;
		removeItem( curItem );		
		resetList();
		for(int i=0; i < newPosition; i++) nextItem();
		insertItem( curItem );
		return TRUE;
	}
	
	if( newPosition > 0 )
	{
		removeItem( curItem );
		resetList();
		for(int i=0; i < newPosition; i++) nextItem();
		insertItem( curItem );
	}
	else // 리스트의 헤더로 옮기고자 할때 헤더 재설정 
	{		
		removeItem( curItem );
		resetList();
		insertItem( curItem );
		setAnchor( curItem );
	}
	return TRUE;
}


BOOL _XList::movePosition( _XItem* curPosItem, int newPosition )
{
	if( !curPosItem || newPosition > this->itemcount-1 ) return FALSE;
	
	// 옮기려는 스프라이트가 조상(리스트의 헤더)일때 조상 재설정
	
	if( firstItem()==curPosItem )
	{
		if( newPosition == 0 ) return FALSE;
		setAnchor( curPosItem->getright() );
		curPosItem->unlink();
		resetList();
		for(int i=0; i < newPosition; i++) nextItem();
		insertItem( curPosItem );
		return TRUE;
	}
	
	if( newPosition > 0 )
	{
		curPosItem->unlink();
		resetList();
		for(int i=0; i < newPosition; i++) nextItem();
		insertItem( curPosItem );
	}
	else // 리스트의 헤더로 옮기고자 할때 헤더 재설정 
	{
		curPosItem->unlink();
		resetList();
		insertItem( curPosItem );
		setAnchor( curPosItem );

	}
	return TRUE;
}

BOOL _XList::checkrange( int Pos )
{
	return ( Pos >= 0 && Pos < this->itemcount );
}