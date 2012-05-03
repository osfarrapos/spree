#include "stdafx.h"
#include <windows.h>
#include "XItem.h"

_XItem::_XItem()
{
      left = right = this;
}

_XItem::~_XItem()
{
      if(left != this) unlink();
}

_XItem *_XItem::link(_XItem *ip)
{
     if(ip == NULL) return NULL;
     if(left != this) unlink();
     right = ip;
     left = ip->left;
     ip->left->right = this;
     ip->left = this;
     return ip;
}

_XItem *_XItem::unlink(void)
{
     left->right = right;
     right->left = left;
     left = right = this;
     return left;
}
