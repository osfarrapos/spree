#include "../stdafx.h"

#ifdef	_SMALL_NETWORK_

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "../Mylibrary/gGlobal.h"
#include "./Packet.h"
#include "./PktMgr.h"
#include "./PktQue.h"
#include "./Sock.h"
#include "./SmallNet.h"
#include "./pktproc/_PktDef.h"

CSmallNet* CSmallNet::pThis = NULL;
PACKET_HANDLER_FUNCTION_POINTER_WRAPPER* CSmallNet::pPHFPW = NULL;

CSmallNet::CSmallNet()
: pfNetEvent(NULL), pfPktHandler(NULL), pClient(NULL), hPktProc(NULL), hSendProc(NULL)
{
	csDestroyThis.Init();
	csClient.Init();

	hThreadSync = NULL;
	bKillThread = FALSE;
	for(int i = 0; i < MAXIMUM_WAIT_OBJECTS; ++i) hKillThread[i] = NULL;
	iCreatedThreadCnt = 0;

	memset(&sni, 0, sizeof(SMALL_NET_INFO));
	memset(&ci, 0, sizeof(CONNECT_INFO));
}

CSmallNet::~CSmallNet() {
	try {
		csClient.Del();
		csDestroyThis.Del();
	}
	catch(...) {
		Logout(_T("#ERR Occur exception while running CSmallNet::~CSmallNet()"));
	}
}

CSmallNet* CSmallNet::CreateInstance(
									 SMALL_NET_INFO& sni,
									 CONNECT_INFO& ci,
									 BOOL(*pfNetEvent)(CNetClient*, const NET_EVENT),
									 void(*pfPktHandler)(CNetClient*, CPacket*))
{
	if(pThis == NULL) {
		// 패킷 매니저 생성
		if(FALSE == CPktMgr::CreateInstance(sni.i256PoolSize, sni.i1024PoolSize)) {
			Logout(_T("#ERR Fail to running CPktMgr::CreateInstance(%d, %d)"), sni.i256PoolSize, sni.i1024PoolSize);
			return NULL;
		}

		// 네트워크 객체 생성
		pThis = new CSmallNet; assert(pThis);
		pThis->pfNetEvent = (BOOL(*)(CNetClient*, const NET_EVENT))pfNetEvent;
		pThis->pfPktHandler = (void(*)(CNetClient*, CPacket*))pfPktHandler;
		pThis->sni = sni;
		pThis->ci = ci;
		Logout(_T("SMALL_NET_INFO %d %d %d %d %d"),
			pThis->sni.i256PoolSize,
			pThis->sni.i1024PoolSize,
			pThis->sni.iRecvProcNum,
			pThis->sni.iSendProcNum,
			pThis->sni.iPktProcNum);
		Logout(_T("CONNECT_INFO %s %d"),
			pThis->ci.pszIp,
			pThis->ci.wPort);
		if(FALSE == pThis->_Create()) {
			Logout(_T("#ERR Return false while running CSmallNet::pThis->Create()"));
			return NULL;
		}
	}
	return pThis;
}

void CSmallNet::DestroyInstance() {
	try {
		pThis->_Destroy();
		SAFE_DEL(pThis);
		SAFE_DEL_ARRAY(pPHFPW);

		CPktMgr::DestroyInstance();
	}
	catch(...) {
		Logout(_T("#ERR Occur exception while running CSmallNet::DestroyInstance()"));
	}
}

BOOL CSmallNet::_Create() {
	Logout(_T("Start to create CSmallNet"));

	csDestroyThis.Lock();
	int i = 0;
	try {
		bConnected = FALSE;

		hThreadSync = CreateEvent(NULL, TRUE, FALSE, NULL); assert(hThreadSync);
		hPktProc = CreateEvent(NULL, FALSE, FALSE, NULL); assert(hPktProc);
		hSendProc = CreateEvent(NULL, FALSE, FALSE, NULL); assert(hSendProc);

		pClient = new CNetClient; assert(pClient);
		if(FALSE == pClient->GetSock()->CreateTcpSocket()) {
			Logout(_T("#ERR Return false while running pClient->GetSock()->CreateTcpSocket()"));
			throw false;
		}
		BOOL bNoDelay = TRUE;
		if(SOCKET_ERROR == setsockopt(pClient->GetSock()->GetSock(), IPPROTO_TCP, TCP_NODELAY, (const char*)&bNoDelay, sizeof(BOOL))) {
			Logout(_T("#ERR Return SOCKET_ERROR while running setsockopt(pClient->hSock, TCP_NODELAY = ON), WSAGetLastError=%d"), WSAGetLastError());
			throw false;
		}

		bKillThread = FALSE;
		for(i = 0; i < MAXIMUM_WAIT_OBJECTS; ++i) {
			hKillThread[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
			assert(hKillThread[i]);
		}

		#define CREATE_THREAD(_num, _thread_name)\
			for(i = 0; i < (int)_num; ++i) {\
				pParam = new CSmallNet::THREAD_PARAM;\
				pParam->pNet = this;\
				pParam->hThread = hKillThread[iCreatedThreadCnt++];\
				hThread = (HANDLE)_beginthreadex(NULL, 0, _thread_name, pParam, 0, NULL);\
				CloseHandle(hThread);\
			}
		iCreatedThreadCnt = 0;
		THREAD_PARAM* pParam = NULL;
		HANDLE hThread = NULL;
		CREATE_THREAD(sni.iRecvProcNum, RecvProc);
		CREATE_THREAD(sni.iSendProcNum, SendProc);
		CREATE_THREAD(sni.iPktProcNum, PktProc);

		if(FALSE == _Connect()) {
			Logout(_T("#ERR Return false while running CSmallNet::Connect()"));
			throw false;
		}

		// start to run all thread synchronously
		SetEvent(hThreadSync);
	}
	catch(...) {
		Sleep(1000); // Wait until all thread are started.
		Logout(_T("#ERR Fail to create CSmallNet"));
		csDestroyThis.Unlock();
		_Destroy();
		return FALSE;
	}
	csDestroyThis.Unlock();
	Logout(_T("Success to create CSmallNet"));
	return TRUE;
}

void CSmallNet::_Destroy() {
	Logout(_T("Start to destroy CSmallNet"));

	int i = 0;
	csDestroyThis.Lock();
	try {
		if(bKillThread == FALSE) {
			_Disconnect();

			SetEvent(hThreadSync);

			bKillThread = TRUE;

			for(i = iCreatedThreadCnt; i < MAXIMUM_WAIT_OBJECTS; ++i) {
				SetEvent(hKillThread[i]);
			}
			if(WAIT_TIMEOUT == WaitForMultipleObjects(MAXIMUM_WAIT_OBJECTS, hKillThread, TRUE, 5000)) {
				Logout(_T("#ERR There're threads which they aren't terminated"));
			}
			for(i = 0; i < MAXIMUM_WAIT_OBJECTS; ++i) { SAFE_CLOSE_HANDLE(hKillThread[i]); }

			SAFE_CLOSE_HANDLE(hThreadSync);
			SAFE_CLOSE_HANDLE(hPktProc);
			SAFE_CLOSE_HANDLE(hSendProc);

			csClient.Lock();
			SAFE_DEL(pClient);
			csClient.Unlock();
		}
		else {
			Logout(_T("Skip to destroy CSmallNet. Already it have destroyed"));
		}
	}
	catch(...) {
		Logout(_T("#ERR Occur exception while running CSmallNet::Destroy()"));
		Logout(_T("#ERR Fail to destroy CSmallNet"));
		csDestroyThis.Unlock();
		return;
	}
	csDestroyThis.Unlock();
	Logout(_T("Success to destroy CSmallNet"));
	return;
}

BOOL CSmallNet::_Connect() {
	csClient.Lock();

	if(ci.pszDomain[0] != NULL) {
		Domain2StringIP(ci.pszDomain, ci.pszIp, IPV4_BUF_SIZE);
	}

	Logout(_T("Start to connect to server %s %d"), ci.pszIp, ci.wPort);
	if(FALSE == pClient->GetSock()->Connect(ci.pszIp, ci.wPort)) {
		Logout(_T("#ERR Return false while running CSmallNet::pClient->GetSock()->Connect(%s, %d)"), ci.pszIp, ci.wPort);
		csClient.Unlock();
		return FALSE;
	}

	//pClient->SetUid(0);	// pClient 객체의 상태 설정; 0 = [임시사용중] 상태
	pClient->SetUid(1);	// pClient 객체의 상태 설정; 1 = [사용중] 상태
	// Acclaim에 연결시에는 [임시사용중] 상태 없음
	if(pfNetEvent != NULL)
		pfNetEvent(pClient, NETEVENT_CONNECTED);

	Logout(_T("Success to connect to server %s %d"), ci.pszIp, ci.wPort);
	csClient.Unlock();
	bConnected = TRUE;
	return TRUE;
}

void CSmallNet::_Disconnect() {
	Logout(_T("Start to disconnect from server %s"), ci.pszIp);

	csClient.Lock();
	if(pClient == NULL || pClient->GetUid() < 0) {
		Logout(_T("#ERR Fail to disconnect from server %s"), ci.pszIp);
		csClient.Unlock();
		return;
	}

	pfNetEvent(pClient, NETEVENT_DISCONNECTED);
	pClient->Finish();

	Logout(_T("Success to disconnect from server %s"), ci.pszIp);
	csClient.Unlock();
	return;
}

BOOL CSmallNet::Send(WORD wPktId, CPacket* pSendPkt) {
	csClient.Lock();
	if(pClient == NULL || pSendPkt == NULL) {
		csClient.Unlock();
		return FALSE;
	}
	csClient.Unlock();

	CPktHeader* pPh = pSendPkt->GetHeader();
	pPh->SetMark();
	pPh->SetPktSize((WORD)pSendPkt->GetBufOffset());
	pPh->SetPktId(wPktId);

	BOOL bRet = FALSE;
	int iPktSize = pSendPkt->GetBufOffset();
	CPktQue::QUEUE_RESULT QueRes = pClient->GetSendQue()->Push(pSendPkt, iPktSize);
	if(QueRes == CPktQue::QRES_OK) {
		SetEvent(hSendProc);
		bRet = TRUE;
	}
	else {
		if(QueRes == CPktQue::QRES_OVERFLOW) {
			Logout(_T("#ERR Return QRES_OVERFLOW while running pClient->GetSendQue()->Push(%p, %d)"), pSendPkt, iPktSize);
		}
		else if(QueRes == CPktQue::QRES_IMPROPER_PARAM) {
			Logout(_T("#ERR Return QRES_IMPROPER_PARAM while running pClient->GetSendQue()->Push(%p, %d)"), pSendPkt, iPktSize);
		}
		bRet = FALSE;
	}

	FreePkt(pSendPkt);
	return bRet;
}

unsigned __stdcall CSmallNet::RecvProc(void* pParam) {
	Logout(_T("Start to run CSmallNet::RecvProc"));

	CSmallNet::THREAD_PARAM param;
	memcpy(&param, pParam, sizeof(CSmallNet::THREAD_PARAM));
	SAFE_DEL(pParam);

	CSmallNet*	pNet = param.pNet;
	CNetClient*	pClient = pNet->pClient;
	CPacket*	pRecvPkt = GetPkt(CELL_1024);
	CPktQue::QUEUE_RESULT QueRes;
	int			iRecvedBytes = 0;

labelRecvProcStart:
	try {
		WaitForSingleObject(pNet->hThreadSync, INFINITE);

		// 아래 부분 수정하여, 재연결 코드 삽입할 것. 20070528
		while(!pNet->bKillThread) {

			if( FALSE == pNet->IsConnected() )	// 연결되어 있지 않다면, 
			{
				Sleep( 30000 );		// 30초마다 다시 연결한다.
				if( !pNet->_Connect() )
				{
					continue;
				}
				else
				{
					// 로그 남기기.
					Logout( _T("Reconnection Success\n") );
#ifdef	_NEXON_NETWORK_
					NEXONPCB_InitNtf( g_ForestNum );
#endif
				}
			}

			pRecvPkt->Init();

			iRecvedBytes = pClient->GetSock()->Recv(pRecvPkt->GetBuf(), (int)pRecvPkt->GetCell());
			if(iRecvedBytes == SOCKET_ERROR) {
				Logout(_T("#ERR Return SOCKET_ERROR while running pClient->GetSock()->Recv(%p, %d)"), pRecvPkt->GetBuf(), (int)pRecvPkt->GetCell());
				pNet->_Disconnect();
				pNet->ResetConnected();
				continue;
			}
			else if(iRecvedBytes == 0) {
				Logout(_T("Return value of recv() is 0,  normal disconnect from server"));
				pNet->_Disconnect();
				pNet->ResetConnected();
				continue;
			}
			else
				pClient->AddRecvedBytes((double)iRecvedBytes);

#if defined(_NEXON_NETWORK_)
			//////////////////////////////////////////////////////////////////////////
			// log send data
			char pLog[1024] = {0};
			for(int i = 0; i < iRecvedBytes; ++i) {
				sprintf(pLog+(i*3), "%02X ", (BYTE)*(pRecvPkt->GetBuf() + i));
			}
			pLog[i*3-1] = NULL;
			Logout("RecvData Dump %3d bytes: %s", iRecvedBytes, pLog);
			//printf("RecvData Dump %3d bytes: %s\n", iRecvedBytes, pLog);
			//////////////////////////////////////////////////////////////////////////
#endif

			QueRes = pClient->GetRecvQue()->Push(pRecvPkt, iRecvedBytes);
			if(QueRes != CPktQue::QRES_OK) {
				if(QueRes == CPktQue::QRES_OVERFLOW) {
					Logout(_T("#ERR Return QRES_OVERFLOW while running pClient->GetRecvQue()->Push(%p, %d)"), pRecvPkt, iRecvedBytes);
					continue;
				}
				else if(QueRes == CPktQue::QRES_IMPROPER_PARAM) {
					Logout(_T("#ERR Return QRES_IMPROPER_PARAM while running pClient->GetRecvQue()->Push(%p, %d)"), pRecvPkt, iRecvedBytes);
					continue;
				}
			}
			SetEvent(pNet->hPktProc);
		} // end of while(!pNet->bKillThread) {
	}
	catch(...) {
		Logout(_T("#ERR Occur exception while running CSmallNet::RecvProc"));
		Sleep(100);
		goto labelRecvProcStart;
	}

	Logout(_T("Start to terminate CSmallNet::RecvProc"));
	FreePkt(pRecvPkt);
	SetEvent(param.hThread);
	Logout(_T("Success to terminate CSmallNet::RecvProc"));
	return 0;
}

unsigned __stdcall CSmallNet::PktProc(void* pParam) {
	Logout(_T("Start to run CSmallNet::PktProc"));

	CSmallNet::THREAD_PARAM param;
	memcpy(&param, pParam, sizeof(CSmallNet::THREAD_PARAM));
	SAFE_DEL(pParam);

	CSmallNet*	pNet = param.pNet;
	CNetClient*	pClient = pNet->pClient;
	CPacket*	pRecvPkt = GetPkt(CELL_1024);
	CPktQue::QUEUE_RESULT QueRes;
	DWORD		dwRet = 0;

labelPktProcStart:
	try {
		WaitForSingleObject(pNet->hThreadSync, INFINITE);

		while(!pNet->bKillThread) {
			dwRet = WaitForSingleObject(pNet->hPktProc, 1024);
			if(dwRet == WAIT_TIMEOUT) continue;
			else if(dwRet != WAIT_OBJECT_0) {
				Logout(_T("#ERR Return WAIT_OBJECT_0 while running WaitForSingleObject(pNet->hPktProc, 1024) GetLastError=%d"), GetLastError());
				goto labelPktProcExit;
			}

			while(!pNet->bKillThread) {
				pRecvPkt->Init();

				QueRes = pClient->GetRecvQue()->Pop(pRecvPkt);
				if(QueRes != CPktQue::QRES_OK) {
					if(QueRes == CPktQue::QRES_NOT_ENOUGH_DATA) break;
					else if(QueRes == CPktQue::QRES_BROKEN_PKT) {
						Logout(_T("#ERR Return QRES_BROKEN_PKT while running pClient->GetRecvQue()->Pop(%p)"), pRecvPkt);
						pClient->GetRecvQue()->Init();
						continue;
					}
					else if(QueRes == CPktQue::QRES_IMPROPER_PARAM) {
						Logout(_T("#ERR Return QRES_IMPROPER_PARAM while running pClient->GetRecvQue()->Pop(%p)"), pRecvPkt);
						continue;
					}
				}

				pNet->pfPktHandler(pClient, pRecvPkt);
			}
		} // end of while(!pNet->bKillThread) {
	}
	catch(...) {
		Logout(_T("#ERR Occur exception while running CSmallNet::PktProc"));
		Sleep(100);
		goto labelPktProcStart;
	}

labelPktProcExit:
	Logout(_T("Start to terminate CSmallNet::PktProc"));
	FreePkt(pRecvPkt);
	SetEvent(param.hThread);
	Logout(_T("Success to terminate CSmallNet::PktProc"));
	return 0;
}

unsigned __stdcall CSmallNet::SendProc(void* pParam) {
	Logout(_T("Start to run CSmallNet::SendProc"));

	CSmallNet::THREAD_PARAM param;
	memcpy(&param, pParam, sizeof(CSmallNet::THREAD_PARAM));
	SAFE_DEL(pParam);

	CSmallNet*	pNet = param.pNet;
	CNetClient*	pClient = pNet->pClient;
	CPacket*	pSendPkt = GetPkt(CELL_1024);
	CPktQue::QUEUE_RESULT QueRes;
	int			iSentBytes = 0;
	DWORD		dwRet = 0;

labelSendProcStart:
	try {
		WaitForSingleObject(pNet->hThreadSync, INFINITE);

		while(!pNet->bKillThread) {
			dwRet = WaitForSingleObject(pNet->hSendProc, 1024);
			if(dwRet == WAIT_TIMEOUT) continue;
			else if(dwRet != WAIT_OBJECT_0) {
				Logout(_T("#ERR Return WAIT_OBJECT_0 while running WaitForSingleObject(pNet->hSendProc, 1024) GetLastError=%d"), GetLastError());
				goto labelSendProcExit;
			}

			while(!pNet->bKillThread) {
				pSendPkt->Init();

				QueRes = pClient->GetSendQue()->Pop(pSendPkt);
				if(QueRes != CPktQue::QRES_OK) {
					if(QueRes == CPktQue::QRES_NOT_ENOUGH_DATA) break;
					else if(QueRes == CPktQue::QRES_BROKEN_PKT) {
						Logout(_T("#ERR Return QRES_BROKEN_PKT while running pClient->GetSendQue()->Pop(%p)"), pSendPkt);
						pClient->GetSendQue()->Init();
						continue;
					}
					else if(QueRes == CPktQue::QRES_IMPROPER_PARAM) {
						Logout(_T("#ERR Return QRES_IMPROPER_PARAM while running pClient->GetSendQue()->Pop(%p)"), pSendPkt);
						continue;
					}
				}

#if defined(_NEXON_NETWORK_)
				//////////////////////////////////////////////////////////////////////////
				// log send data
				int iPktSize = (int)(ConvertByteOrder2(pSendPkt->GetHeader()->GetPktSize()) + 3);
				char pLog[1024] = {0};
				for(int i = 0; i < iPktSize; ++i) {
					sprintf(pLog+(i*3), "%02X ", (BYTE)*(pSendPkt->GetBuf() + i));
				}
				pLog[i*3-1] = NULL;
				//Logout("SendData Dump %3d bytes: %s", iPktSize, pLog);
				//printf("SendData Dump %3d bytes: %s\n", iPktSize, pLog);
				//////////////////////////////////////////////////////////////////////////

				iSentBytes = pClient->GetSock()->Send(pSendPkt->GetBuf(), iPktSize);
				if(iSentBytes == SOCKET_ERROR) {
					Logout(_T("#ERR Return SOCKET_ERROR while running pClient->GetSock()->Send(%p, %d)"), pSendPkt->GetBuf(), iPktSize);
					//goto labelSendProcExit;
				}
				else
					pClient->AddSentBytes((double)iSentBytes);
#else
				iSentBytes = pClient->GetSock()->Send(pSendPkt->GetBuf(), pSendPkt->GetHeader()->GetPktSize());
				if(iSentBytes == SOCKET_ERROR) {
					int iPktSize = pSendPkt->GetHeader()->GetPktSize();
					Logout(_T("#ERR Return SOCKET_ERROR while running pClient->GetSock()->Send(%p, %d)"), pSendPkt->GetBuf(), iPktSize);
					//goto labelSendProcExit;
				}
				else
					pClient->AddSentBytes((double)iSentBytes);
#endif
			}
		} // end of while(!pNet->bKillThread) {
	}
	catch(...) {
		Logout(_T("#ERR Occur exception while running CSmallNet::SendProc"));
		Sleep(100);
		goto labelSendProcStart;
	}

labelSendProcExit:
	Logout(_T("Start to terminate CSmallNet::SendProc"));
	FreePkt(pSendPkt);
	SetEvent(param.hThread);
	Logout(_T("Success to terminate CSmallNet::SendProc"));
	return 0;
}

#endif //_SMALL_NETWORK_