#ifdef	_SMALL_NETWORK_

#ifndef	_PKTQUE_
#define _PKTQUE_

class CPktQue {
public:
	enum QUEUE_RESULT {
		QRES_OK							= 1,
		QRES_NOT_INITIALIZED		= 0,	// 객체가 초기화 되지 않았음

		// push() 할 때
		QRES_OVERFLOW				= -100,	// 버퍼 넘침
		QRES_IMPROPER_PARAM	= -101, // 부적절한 함수 파라미터

		// pop() 할 때
		QRES_NOT_ENOUGH_DATA	= -200,	// 하나 이상의 온전한 패킷 데이터 없음
		QRES_BROKEN_PKT			= -201	// 무결성 깨진 패킷
	};

private:
	enum { 
		QUEUE_BUF_SIZE = 40960
	};

private:
	CS		cs; // for this
	char*	pBuf;
	int		f;	// front indicator
	int		r;	// rear indicator

public:
	CPktQue();
	~CPktQue();
	BOOL	Create();
	void	Init();	// reusable init

public:
	QUEUE_RESULT Push(IN CPacket* pPkt, IN int iDataSize);
	QUEUE_RESULT Pop(OUT CPacket* pPkt);

protected:
	int			_GetFreeSize();			// 버퍼 전체의 여유 공간 크기
	int			_GetFrontFreeSize();	// 버퍼 앞쪽의 여유 공간 크기, 앞쪽: f or r ~ 버퍼끝

	int			_GetDataSize();			// 버퍼 전체의 데이터 크기
	int			_GetFrontDataSize();	// 버퍼 앞쪽의 데이터 크기
};

#endif //_PKTQUE_

#endif //_SMALL_NETWORK_