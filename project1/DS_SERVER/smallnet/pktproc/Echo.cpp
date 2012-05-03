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
#include "./Echo.h"

#endif //_SMALL_NETWORK_