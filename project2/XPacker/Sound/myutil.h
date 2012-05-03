#ifndef __MYUTIL
#define __MYUTIL

#include <stdio.h>
#include <memory.h>
#include "plattype.h"

#define UNITY ((BYTE) 1)
// number of elements of vector x
#define NELEMV(x)  ( sizeof(x)/sizeof((x)[0]) )
//set bit i of x to one
#define SETBIT(x,i) ( (x)=(x)|(UNITY<<i) )
//set bit i of x to zero (only for one byte-length x)
#define ZEROSETBIT(x,i)	( (x)=(x)& (0xFF-(UNITY<<i)) ) 
//get bit i of x
#define GETBIT(x,i) ( (x)&(UNITY<<i) )
//check if button is pressed in direct input environment
#define ISPRESSED(x) ( (x) & 0x80 )
// return v if minv<v<maxv, minv if v<=minv, maxv if v>= maxv
#define MUSTIN(v, minv, maxv)  (max(min(v,maxv),minv))
// return x sign 
#define SIGN(x) ((x)!=0 ? ((x)>0 ? 1 : -1) : 0) 

// To be platform-independent: it init to zero dest

#define INITTOZERO(dest,size) (memset(dest, 0, size))
#define COPYMEMORY(dest,source,sizeByte) memcpy(dest,source,sizeByte)


void AddBackslash(char *string);
void BuildFullPath(char *nameFull,char *path, char *name, char drive,char *ext);
#endif
