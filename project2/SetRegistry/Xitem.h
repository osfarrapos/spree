#ifndef _XITEM_H_
#define _XITEM_H_        


class _XItem {
      protected:
           _XItem *left;
           _XItem *right;
      public:
           _XItem();
           virtual ~_XItem();
           _XItem *link(_XItem *ip);
           _XItem *unlink(void);
           _XItem *getleft(void)  { return left;  }
           _XItem *getright(void) { return right; }
      };

#endif