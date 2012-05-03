//#include <Windows.h>
#include "../stdafx.h"

#ifdef ITEMSHOP_PATCH_200607
#include "../Global/Global_Function.h"
#include "./WebShopPktQueue.h"

CWebShopPktQueue::CWebShopPktQueue() {
	CS_INIT(csThis);
	pBuf = NULL; // InitThis();
	ReInitThis();
}

CWebShopPktQueue::~CWebShopPktQueue() {
	SAFE_DELETE_ARRAY(pBuf);
	CS_DEINIT(csThis);
}

BOOL CWebShopPktQueue::InitThis() {
	if(pBuf == NULL) {
		pBuf = new char[QUEUE_BUF_SIZE];
		if(pBuf == NULL)
			return FALSE;
		else
			return TRUE;
	}
	return TRUE;
}

void CWebShopPktQueue::ReInitThis() {
	CS_LOCK(csThis);
	f = r = 0;
	CS_UNLOCK(csThis);
}

CWebShopPktQueue::QUEUE_RESULT CWebShopPktQueue::Push(IN const char* pData, IN int iDataSize) {
	if(pBuf == NULL) {
		return QRES_NOT_INITIALIZED;
	}

	if(pData == NULL || iDataSize == 0) {
		return QRES_IMPROPER_PARAM;
	}

	CS_LOCK(csThis);
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
		else if(iDataSize < iFrontFree) {
			memcpy(pBuf + f, pData, iDataSize);
			f = 0;
		}
	}
	else if(iDataSize > iFree) {
		CS_UNLOCK(csThis);
		return QRES_OVERFLOW;
	}
	CS_UNLOCK(csThis);
	return QRES_OK;
}

CWebShopPktQueue::QUEUE_RESULT CWebShopPktQueue::Pop(OUT char* pOutBuf) {
	if(pBuf == NULL) {
		return QRES_NOT_INITIALIZED;
	}

	#define RESERVED_CHAR_SIZE	1 /*ReservedChar*/
	#define CONTENT_LEN_SIZE	2 /*ContentLength(2)*/
	#define PKT_HEADER_SIZE		(RESERVED_CHAR_SIZE + CONTENT_LEN_SIZE)

	CS_LOCK(csThis);

	// read packet header
	int iDataSize = _GetDataSize();
	int iFrontDataSize = _GetFrontDataSize();
	int r_old = r;

	if(iDataSize < PKT_HEADER_SIZE) {
		CS_UNLOCK(csThis);
		return QRES_NOT_ENOUGH_DATA;
	}
	
	if(iFrontDataSize > PKT_HEADER_SIZE) {
		memcpy(pOutBuf, pBuf + r, PKT_HEADER_SIZE);
		r += PKT_HEADER_SIZE;
	}
	else if(iFrontDataSize < PKT_HEADER_SIZE) {
		memcpy(pOutBuf, pBuf + r, iFrontDataSize);
		memcpy(pOutBuf + iFrontDataSize, pBuf, PKT_HEADER_SIZE - iFrontDataSize);
		r = PKT_HEADER_SIZE - iFrontDataSize;
	}
	else if(iFrontDataSize == PKT_HEADER_SIZE) {
		memcpy(pOutBuf, pBuf + r, PKT_HEADER_SIZE);
		r = 0;
	}
	int iContentLen = *(LPWORD)(pOutBuf + RESERVED_CHAR_SIZE);

	// read packet data
	iDataSize = _GetDataSize();
	iFrontDataSize = _GetFrontDataSize();

	if(iDataSize < iContentLen) {
		r = r_old;
		CS_UNLOCK(csThis);
		return QRES_NOT_ENOUGH_DATA;
	}

	if(iFrontDataSize == iContentLen) {
		memcpy(pOutBuf, pBuf + r, iContentLen);
		r = f;
	}
	else if(iFrontDataSize > iContentLen) {
		memcpy(pOutBuf, pBuf + r, iContentLen);
		r += iContentLen;
	}
	else if(iFrontDataSize < iContentLen) {
		memcpy(pOutBuf, pBuf + r, iFrontDataSize);
		memcpy(pOutBuf + iFrontDataSize, pBuf, iContentLen - iFrontDataSize);
		r = iContentLen - iFrontDataSize;
	}

	CS_UNLOCK(csThis);
	return QRES_OK;

	#undef RESERVED_CHAR_SIZE
	#undef CONTENT_LEN_SIZE
	#undef PKT_HEADER_SIZE
}

int CWebShopPktQueue::_GetFreeSize() {
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

// 큐 버퍼 앞쪽 여유 공간 크기
int CWebShopPktQueue::_GetFrontFreeSize() {
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

int CWebShopPktQueue::_GetDataSize() {
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

int CWebShopPktQueue::_GetFrontDataSize() {
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

#endif // ITEMSHOP_PATCH_200607