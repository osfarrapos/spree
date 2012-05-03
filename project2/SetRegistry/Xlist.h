#ifndef _XLIST_H_
#define _XLIST_H_     

#include <windows.h>
#include "XItem.h"

class _XList : public _XItem {
      private:
          _XItem *anchor;
          _XItem *cip;
	
	  public:
		  int     itemcount;
		  
      public:
          _XList();
          virtual ~_XList();

          int listEmpty(void){     return (anchor == NULL); }
          int atHeadOfList(void){  return ((anchor != NULL) && (cip == anchor)); }
          int atEndOfList(void){   return ((anchor != NULL) && (cip == anchor->getleft())); }
          _XItem *currentItem(void){ return cip; }
          _XItem *firstItem(void){   return (cip = anchor); }
          void resetList(void){    cip = anchor; }
          void setCurrentItem(_XItem *ip){ cip = ip; }

          _XItem *insertItem(_XItem *ip);
          _XItem *removeItem(_XItem *ip);
          _XItem *prevItem(void);
          _XItem *nextItem(void);

		  BOOL deleteItem(_XItem *ip);
		  BOOL deleteItem(int Pos);
		  
		  void setAnchor( _XItem *ip ){ anchor = ip; }
          virtual void disposeList(void);

		  virtual BOOL movePosition( int curPosition, int newPosition );
		  virtual BOOL movePosition( _XItem* curPosItem, int newPosition );
		  void currentItemSwap( void );
		  _XItem *getItem( int Pos );	
		  int  searchItem( _XItem* ip );	  
		  int  getitemcount( void ){ return itemcount; }
		  
		  inline BOOL checkrange( int Pos );
};   //end of class

#endif
