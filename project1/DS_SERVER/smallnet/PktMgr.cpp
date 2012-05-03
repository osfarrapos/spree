#include "../stdafx.h"

#ifdef	_SMALL_NETWORK_

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "../Mylibrary/gGlobal.h"
#include "./Packet.h"
#include "./PktMgr.h"

CPktMgr*	CPktMgr::pThis = NULL;

BOOL CPktPool::Create(PKT_CELL cell, int iStackSize, BOOL bEmptyPool) {
	if(iStackSize <= 0) return FALSE;

	if(ppPkt != NULL) Destroy();

	cs.Lock();
	ppPkt = new CPacket*[iStackSize]; assert(ppPkt);
	iIdx = 0;

	if(bEmptyPool == FALSE) {
		CPacket* pPkt = NULL;
		for(int i = 0; i < iStackSize; ++i) {
			pPkt = new CPacket; assert(pPkt);
			pPkt->Create(cell);

			// push
			ppPkt[iIdx++] = pPkt;
		}
	}
	cs.Unlock();
	return TRUE;
}

void CPktPool::Destroy() {
	if(ppPkt == NULL) return;

	CPacket* pPkt = NULL;
	cs.Lock();
	while(TRUE) {
		// pop
		if(iIdx == 0) break;
		else pPkt = ppPkt[--iIdx];
		
		SAFE_DEL(pPkt);
	}

	delete [] ppPkt;
	ppPkt = NULL;
	cs.Unlock();
	return;
}

BOOL CPktPool::Push(CPacket* pPkt) {
	if(ppPkt == NULL || pPkt == NULL) return FALSE;

	cs.Lock();
	ppPkt[iIdx++] = pPkt;
	cs.Unlock();
	return TRUE;
}

CPacket* CPktPool::Pop() {
	if(ppPkt == NULL) return NULL;

	CPacket* pPkt = NULL;
	cs.Lock();
	if(iIdx == 0) pPkt = NULL;
	else pPkt = ppPkt[--iIdx];
	cs.Unlock();
	return pPkt;
}

int	CPktPool::GetSize() {
	if(ppPkt == NULL) return -1;

	int iSize = 0;
	cs.Lock();
	iSize = iIdx;
	cs.Unlock();
	return iSize;
}

CPktMgr* CPktMgr::CreateInstance() {
	if(pThis == NULL) {
		pThis = new CPktMgr; assert(pThis);

		pThis->cs.Lock();
		pThis->pPool256 = new CPktPool; assert(pThis->pPool256);
		pThis->pPool256->Create(CELL_256, PKTPOOLSIZE_256, FALSE);

		pThis->pPool1024 = new CPktPool; assert(pThis->pPool1024);
		pThis->pPool1024->Create(CELL_1024, PKTPOOLSIZE_1024, FALSE);
		pThis->cs.Unlock();
	}
	return pThis;
}

CPktMgr* CPktMgr::CreateInstance(int i256PoolSize, int i1024PoolSize) {
	if(pThis == NULL) {
		pThis = new CPktMgr; assert(pThis);

		pThis->cs.Lock();
		pThis->pPool256 = new CPktPool; assert(pThis->pPool256);
		pThis->pPool256->Create(CELL_256, i256PoolSize, FALSE);

		pThis->pPool1024 = new CPktPool; assert(pThis->pPool1024);
		pThis->pPool1024->Create(CELL_1024, i1024PoolSize, FALSE);
		pThis->cs.Unlock();
	}
	return pThis;
}

void CPktMgr::DestroyInstance() {
	try {
		if(pThis != NULL) {
			pThis->cs.Lock();
			SAFE_DEL(pThis->pPool256);
			SAFE_DEL(pThis->pPool1024);
			pThis->cs.Unlock();
			
			SAFE_DEL(pThis);
		}
	}
	catch(...) {
		Logout(_T("#ERR Occur an exception while running CPktMgr::DestroyInstance()"));
	}
}

CPktPool*	CPktMgr::GetPool(PKT_CELL cell) {
	CPktPool* pPool = NULL;

	cs.Lock();
	switch(cell) {
	case CELL_256:
		pPool = pPool256;
		break;
	case CELL_1024:
		pPool = pPool1024;
		break;
	default:
		pPool = NULL;
		break;
	}
	cs.Unlock();
	return pPool;
}

int CPktMgr::GetSize(PKT_CELL cell) {
	int iSize = 0;

	cs.Lock();
	switch(cell) {
	case CELL_256:
		iSize = pPool256->GetSize();
		break;
	case CELL_1024:
		iSize = pPool1024->GetSize();
		break;
	default:
		iSize = 0;
		break;
	}
	cs.Unlock();
	return iSize;
}

CPacket* GetPkt(PKT_CELL cell) {
	CPktMgr* pPktMgr = CPktMgr::GetInstance();
	CPktPool* pPool = NULL;
	CPacket* pPkt = NULL;

	pPktMgr->cs.Lock();
	if(pPktMgr == NULL) {
		pPktMgr->cs.Unlock();
		return NULL;
	}

	pPool = pPktMgr->GetPool(cell);
	if(pPool == NULL) pPkt = NULL;
	else pPkt = pPool->Pop();
	pPktMgr->cs.Unlock();

	if(pPkt != NULL) pPkt->Init();
	return pPkt;
}

void FreePkt(CPacket* pPkt) {
	CPktMgr* pPktMgr = CPktMgr::GetInstance();
	CPktPool* pPool = NULL;

	pPktMgr->cs.Lock();
	if(pPktMgr == NULL || pPkt == NULL) {
		pPktMgr->cs.Unlock();
		return;
	}

	pPool = pPktMgr->GetPool(pPkt->GetCell());
	if(pPool != NULL) pPool->Push(pPkt);
	else {
		SAFE_DEL(pPkt);
	}
	pPktMgr->cs.Unlock();
	pPkt = NULL;
	return;
}

#endif //_SMALL_NETWORK_