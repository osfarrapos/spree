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

BOOL NetEventProc(CNetClient* pClient, const NET_EVENT NetEvent) {
	static TCHAR pszLog[256] = {0};

	switch(NetEvent) {
	case NETEVENT_CONNECTED:
		{
			CSmallNet::CONNECT_INFO& ci = CSmallNet::GetInstance()->GetConnectInfo();
			_stprintf(pszLog, _T("Connection established IP=%s"), ci.pszDomain[0] != NULL ? ci.pszDomain : ci.pszIp);
			Logout(_T("%s"), pszLog); _tprintf(_T("%s \n"), pszLog);
		}
		break;

	case NETEVENT_DISCONNECTED:
		{
			_stprintf(pszLog, _T("Disconnected from server Uid=%d, dRecvedBytes=%g, dSentBytes=%g"),
				pClient->GetUid(), pClient->GetRecvedBytes(), pClient->GetSentBytes());
			Logout(_T("%s"), pszLog); _tprintf(_T("%s \n"), pszLog);
		}
		break;
	}
	return TRUE;
}

#endif //_SMALL_NETWORK_