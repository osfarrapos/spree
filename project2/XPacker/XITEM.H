#ifndef __ITEM_H
#define __ITEM_H        


class item {
      protected:
           item *left;
           item *right;
      public:
           item();
           virtual ~item();
           item *link(item *ip);
           item *unlink(void);
           item *getleft(void)  { return left;  }
           item *getright(void) { return right; }
      };

#endif