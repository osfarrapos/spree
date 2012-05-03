#include "../../stdafx.h"

#ifdef	_SMALL_NETWORK_

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "../../Mylibrary/gGlobal.h"
#include "../Packet.h"
#include "../PktMgr.h"
#include "../PktQue.h"
#include "../Sock.h"
#include "../SmallNet.h"
#include "./_PktDef.h"

#if defined(_NEXON_NETWORK_)
void PKTHDR_NEXONPCB_LOGINOUT(CNetClient*, CPacket*);

BEGIN_PKT_HANDLER_MAP(PktHandler)
	DECLARE_HANDLER(NEXONPCB_LOGINOUT)
END_PKT_HANDLER_MAP
#endif

#endif //_SMALL_NETWORK_