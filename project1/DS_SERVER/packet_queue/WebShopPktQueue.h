#pragma once

#ifdef ITEMSHOP_PATCH_200607

//////////////////////////////////////////////////////////////////////////
// 2006.05.30 Molla, Nexon Web Shop
// safe-thread class
//////////////////////////////////////////////////////////////////////////

class CWebShopPktQueue {
public:
	enum QUEUE_RESULT {
		QRES_OK					= 1,

		QRES_NOT_INITIALIZED	= 0,	// 객체가 초기화 되지 않았음
		
		// push() 할 때
		QRES_OVERFLOW			= -100,	// overflow
		QRES_IMPROPER_PARAM		= -101, // 부적절한 함수 파라미터
		
		// pop() 할 때
		QRES_NOT_ENOUGH_DATA	= -200	// pop() 할 때, 큐 버퍼에 충분한 데이터 없음
	};

public:
	char* pBuf;

	int f; // front indicator
	int r; // rear indicator

	CRITICAL_SECTION csThis;

private:
	enum {
		QUEUE_BUF_SIZE = 819200
	};

public:
	CWebShopPktQueue();
	virtual ~CWebShopPktQueue();

	BOOL InitThis();
	void ReInitThis();

public:
	QUEUE_RESULT Push(IN const char* pData, IN int iDataSize);
	QUEUE_RESULT Pop(OUT char* pOutBuf);

protected:
	int _GetFreeSize();			// 큐 버퍼 여유 공간 크기
	int _GetFrontFreeSize();	// 큐 버퍼 앞쪽 여유 공간 크기

	int _GetDataSize();
	int _GetFrontDataSize();

	// 패킷 타입에 따른 데이터 크기 구하기
};

#endif //ITEMSHOP_PATCH_200607
