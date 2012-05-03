#ifndef __MYERROR
#define __MYERROR

#pragma warning(disable:4100)  // unreferenced formal parameter
#pragma warning(disable:4115)  // named type definition in parentheses
#pragma warning(disable:4201)  // nonstandard extension used : nameless struct/union

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;
#endif // !_HRESULT_DEFINED

#include "myassert.h"
#include "tracerror.h"

#ifdef _DEBUG	
#define DEBUGMYERROR(tab,hr)  MYERROR(tab,hr,__FILE__,__LINE__,ABORTIGNORA)
#define	DEBUGASSERT(bb) myassert(bb,"Debug Error!!",NULL)
#else
#define DEBUGMYERROR(tab,hr)  ((void)0)
#define	DEBUGASSERT(bb)	((void)0)
#endif

#define ASSERTOUTMEMORY(bb) myassert(bb, "Fatal Error!", "Out Of Memory")
#define ASSERTFILE(bb,name) myassert(bb,"File Error!", name)
#define ASSERTRANGE(bb)	myassert(bb,"Fatal Error!", "Range Error")
#define ASSERTINIT(bb) myassert(bb,"Fatal Error!", "Device Already Initialized")
#define ASSERTPOINTER(bb) myassert(bb,"Fatal Error!", "Null Pointer")
#define ASSERTCREATE(bb,name) myassert(bb,"I can't create this object", name)
#define ASSERTFILEFMT(bb,name) myassert(bb, "File corrupted!",name);

#define FATALASSERT(bb) myassert(bb,"Fatal Error!","General Error")
#define FATALMYERROR(tab,hr)  MYERROR(tab,hr,__FILE__,__LINE__,ABORTIGNORA)

//ERROR CODE TABLE FOR MYERROR
#define DITAB 10000
#define DSTAB 8000

#endif