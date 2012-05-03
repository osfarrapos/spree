// TcpResource.cpp: implementation of the TcpResource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "WWII.h"
#include "TcpResource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TcpResource::TcpResource()
{
	bInit		= FALSE;
	bLogin		= FALSE;
	bNewid		= FALSE;
	bChangePW	= FALSE;
	bJoinChat	= FALSE;
	bCreateChat	= FALSE;
	bCreateGame	= FALSE;
	bJoinGame	= FALSE;
	bChatList	= FALSE;
	bChatUsers	= FALSE;
	bGameList	= FALSE;
}
TcpResource::~TcpResource()
{

}


