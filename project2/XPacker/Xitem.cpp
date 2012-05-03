#include "stdafx.h"
#include "XItem.h"

item::item()
{
      left = right = this;
}

item::~item()
{
      if(left != this) unlink();
}

item *item::link(item *ip)
{
     if(ip == NULL) return NULL;
     if(left != this) unlink();
     right = ip;
     left = ip->left;
     ip->left->right = this;
     ip->left = this;
     return ip;
}

item *item::unlink(void)
{
     left->right = right;
     right->left = left;
     left = right = this;
     return left;
}
