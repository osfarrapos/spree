#include "stdafx.h"
#include "XList.h"

list::list()
{
      anchor = cip = NULL;
}

list::~list()
{
      if(anchor != NULL) disposeList();
}

item *list::insertItem(item *ip)
{
     if(ip == NULL) return NULL;
     if(anchor == NULL) return anchor = cip = ip;
     return ip->link(cip);
}

item *list::removeItem(item *ip)
{
     if(ip == NULL) return NULL;
     if(ip->getright() == ip) anchor = cip = NULL;
     else {
          if(ip == anchor) anchor = anchor->getright();
          if(cip == ip) cip = cip->getright();
     }
     return ip->unlink();
}

BOOL list::deleteItem(item *ip)
{
	 if(ip == NULL) return FALSE;
     if(ip->getright() == ip) anchor = cip = FALSE;
     else {
          if(ip == anchor) anchor = anchor->getright();
          if(cip == ip) cip = cip->getright();
     }
     delete ip;
	 ip = NULL;
	 return TRUE;
}


item *list::prevItem(void)
{
     if(cip != NULL) cip = cip->getleft();
     return cip;
}

item *list::nextItem(void)
{
     if(cip != NULL) cip = cip->getright();
     return cip;
}

void list::disposeList(void)
{
     while(!listEmpty())
           delete removeItem(currentItem());
}

void list::currentItemSwap( void )
{	 
	 item *tip = cip;
	 
	 removeItem(cip);
	 
	 nextItem();
	 
	 insertItem( tip );
	 cip = tip;
	 
}

item *list::getItem( int Pos )
{
	 resetList();
	 for( int i=0; i<Pos; i++ ) nextItem();
	 return cip;
}

int list::searchItem( item* ip )
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

BOOL list::movePosition( int curPosition, int newPosition )
{	
	if( curPosition == newPosition ) return FALSE;	
	
	// 옮기려는 스프라이트가 조상(리스트의 헤더)일때 조상 재설정
	item* curItem = getItem( curPosition );

	if( !curItem ) return FALSE;
	
	if( firstItem()==curItem )
	{
		if( newPosition == 0 ) return FALSE;
		setAnchor( curItem->getright() );
		curItem->unlink();
		resetList();
		for(int i=0; i < newPosition; i++) nextItem();
		insertItem( curItem );
		return TRUE;
	}
	
	if( newPosition > 0 )
	{
		curItem->unlink();
		resetList();
		for(int i=0; i < newPosition; i++) nextItem();
		insertItem( curItem );
	}
	else // 리스트의 헤더로 옮기고자 할때 헤더 재설정 
	{
		curItem->unlink();
		resetList();
		insertItem( curItem );
		setAnchor( curItem );

	}
	return TRUE;
}
