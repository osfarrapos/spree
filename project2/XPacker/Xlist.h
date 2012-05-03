#ifndef __LIST_H
#define __LIST_H     

#include <windows.h>
#include "XItem.h"

class list : public item {
      private:
          item *anchor;
          item *cip;

      public:
          list();
          virtual ~list();

          int listEmpty(void){     return (anchor == NULL); }
          int atHeadOfList(void){  return ((anchor != NULL) && (cip == anchor)); }
          int atEndOfList(void){   return ((anchor != NULL) && (cip == anchor->getleft())); }
          item *currentItem(void){ return cip; }
          item *firstItem(void){   return (cip = anchor); }
          void resetList(void){    cip = anchor; }
          void setCurrentItem(item *ip){ cip = ip; }

          item *insertItem(item *ip);
          item *removeItem(item *ip);
          item *prevItem(void);
          item *nextItem(void);

		  BOOL deleteItem(item *ip);
		  
		  void setAnchor( item *ip ){ anchor = ip; }
          virtual void disposeList(void);

		  virtual BOOL movePosition( int curPosition, int newPosition );
		  void currentItemSwap( void );
		  item *getItem( int Pos );	
		  int  searchItem( item* ip );	  
		  
};   //end of class

#endif
