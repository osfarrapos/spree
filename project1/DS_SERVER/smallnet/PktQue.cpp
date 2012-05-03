#include "../stdafx.h"

#ifdef	_SMALL_NETWORK_

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "../Mylibrary/gGlobal.h"
#include "./Packet.h"
#include "./PktQue.h"

CPktQue::CPktQue()
: pBuf(NULL)
{
	cs.Init();
	Create();
}

CPktQue::~CPktQue() {
	try {
		SAFE_DEL_ARRAY(pBuf);
		cs.Del();
	}
	catch(...) {
		Logout(_T("#ERR Occur an exception while running CPktQue::~CPktQue()"));
	}
}

BOOL CPktQue::Create() {
	pBuf = new char[QUEUE_BUF_SIZE]; assert(pBuf);
	Init();
	return TRUE;
}

void CPktQue::Init() {
	cs.Lock();
	f = r = 0;
	cs.Unlock();
}

int CPktQue::_GetFreeSize() {
	if(f == r) {
		return QUEUE_BUF_SIZE - 1;
	}
	else if(f > r) {
		return (QUEUE_BUF_SIZE - f) + r - 1;
	}
	else /*if(f < r)*/ {
		return r - f - 1;
	}
}

int CPktQue::_GetFrontFreeSize() {
	if(f >= r) {
		if(r == 0)
			return QUEUE_BUF_SIZE - f - 1;
		else
			return QUEUE_BUF_SIZE - f;
	}
	else /*if(f < r)*/ {
		return r - f - 1;
	}
}

int CPktQue::_GetDataSize() {
	if(f > r) {
		return f - r;
	}
	else if(f < r) {
		return QUEUE_BUF_SIZE - r + f;
	}
	else /*if(f == r)*/ {
		return 0;
	}
}

int CPktQue::_GetFrontDataSize() {
	if(f > r) {
		return f - r;
	}
	else if(f < r) {
		return QUEUE_BUF_SIZE - r;
	}
	else /*if(f == r)*/ {
		return 0;
	}
}

CPktQue::QUEUE_RESULT CPktQue::Push(CPacket *pPkt, int iDataSize) {
	if(pPkt == NULL || iDataSize <= 0) {
		Logout(_T("#ERR(%d) QRES_IMPROPER_PARAM"), __LINE__);
		return QRES_IMPROPER_PARAM;
	}

	QUEUE_RESULT QueRes = QRES_OK;
	char* pData = pPkt->GetBuf();

	cs.Lock();

	int iFree = _GetFreeSize();
	int iFrontFree = _GetFrontFreeSize();

	if(iDataSize <= iFree) {
		if(iDataSize < iFrontFree) {
			memcpy(pBuf + f, pData, iDataSize);
			f += iDataSize;
		}
		else if(iDataSize > iFrontFree) {
			memcpy(pBuf + f, pData, iFrontFree);
			memcpy(pBuf, pData + iFrontFree, iDataSize - iFrontFree);
			f = iDataSize - iFrontFree;
		}
		else /*if(iDataSize == iFrontFree)*/ {
			memcpy(pBuf + f, pData, iDataSize);
			f = 0;
		}
	}
	else if(iDataSize > iFree) {
		QueRes = QRES_OVERFLOW;
	}

	cs.Unlock();
	return QueRes;
}

CPktQue::QUEUE_RESULT CPktQue::Pop(CPacket* pPkt) {
	if(pPkt == NULL) {
		Logout(_T("#ERR(%d) QRES_IMPROPER_PARAM"), __LINE__);
		return QRES_IMPROPER_PARAM;
	}

	QUEUE_RESULT QueRes = QRES_OK;
	char* pOutBuf = pPkt->GetBuf();

	cs.Lock();

	// read packet header
	int iDataSize = _GetDataSize();
	int iFrontDataSize = _GetFrontDataSize();
	int r_old = r;

	if(iDataSize < TCP_PH_SIZE) {
		cs.Unlock();
		return QRES_NOT_ENOUGH_DATA;
	}

	if(iFrontDataSize > TCP_PH_SIZE) {
		memcpy(pOutBuf, pBuf + r, TCP_PH_SIZE);
		r += TCP_PH_SIZE;
	}
	else if(iFrontDataSize < TCP_PH_SIZE) {
		memcpy(pOutBuf, pBuf + r, iFrontDataSize);
		memcpy(pOutBuf + iFrontDataSize, pBuf, TCP_PH_SIZE - iFrontDataSize);
		r = TCP_PH_SIZE - iFrontDataSize;
	}
	else /*if(iFrontDataSize == TCP_PH_SIZE)*/ {
		memcpy(pOutBuf, pBuf + r, TCP_PH_SIZE);
		r += TCP_PH_SIZE;
		if(r == QUEUE_BUF_SIZE) r = 0;
	}

#if defined(_NEXON_NETWORK_)
	// 하나의 패킷 중 헤더를 제외한 데이터 크기
	int iContentLen = (int)(ConvertByteOrder2(pPkt->GetHeader()->GetPktSize()) - 1);
#else
	// 하나의 패킷 중 헤더를 제외한 데이터 크기
	int iContentLen = pPkt->GetHeader()->GetPktSize() - TCP_PH_SIZE;
#endif

	// read packet data
	pOutBuf = pPkt->GetContents();
	iDataSize = _GetDataSize();
	iFrontDataSize = _GetFrontDataSize();

	if(iDataSize < iContentLen) {
		r = r_old;
		QueRes = QRES_NOT_ENOUGH_DATA;
	}
	else /*if(iDataSize >= iContentLen)*/ {
		if(iFrontDataSize == iContentLen) {
			memcpy(pOutBuf, pBuf + r, iContentLen);
			r += iContentLen;
			if(r == QUEUE_BUF_SIZE) r = 0;
		}
		else if(iFrontDataSize > iContentLen) {
			memcpy(pOutBuf, pBuf + r, iContentLen);
			r += iContentLen;
		}
		else /*if(iFrontDataSize < iContentLen)*/ {
			memcpy(pOutBuf, pBuf + r, iFrontDataSize);
			memcpy(pOutBuf + iFrontDataSize, pBuf, iContentLen - iFrontDataSize);
			r = iContentLen - iFrontDataSize;
		}
	}

	cs.Unlock();
	return QueRes;
}

#endif //_SMALL_NETWORK_