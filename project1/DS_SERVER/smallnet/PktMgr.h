#ifdef	_SMALL_NETWORK_

#ifndef	_PKTMGR_
#define _PKTMGR_

class CPktPool {
private:
	CPacket**	ppPkt;				// CPacket을 stack 처럼 관리
	int			iIdx;				// ppPkt의 stack index
	CS			cs;

public:
	CPktPool()
		: ppPkt(NULL)
	{
		// cell			: CPacket의 buffer size
		// iStackSize	: ppPkt의 stack size
		//					  stack size는 bEmptyPool 값과 무관하게 zero일 수 없음
		// bEmptyPool	: empty stack으로 생성할지의 여부

		cs.Init();
	}
	~CPktPool() {
		try {
			Destroy();
			cs.Del();
		}
		catch(...) {
			Logout(_T("#ERR Occur an exception while running ~CPktPool()"));
		}
	}

	BOOL	Create(PKT_CELL cell, int iStackSize, BOOL bEmptyPool);
	void	Destroy();
	BOOL	Push(CPacket* pPkt);
	CPacket*	Pop();
	int		GetSize();
};

class CPktMgr {
public:
	enum PKT_POOL_SIZE {
		PKTPOOLSIZE_256 = 10,
		PKTPOOLSIZE_1024 = 10,
	};

public:
	CS				cs;
private:
	static CPktMgr*	pThis;		// singleton

	CPktPool*		pPool256;	// 버퍼가  256bytes인 패킷의 풀; CPktPool: stack 구조
	CPktPool*		pPool1024;	// 버퍼가 1024bytes인 패킷의 풀

public:
	CPktMgr()
		: pPool256(NULL), pPool1024(NULL)
	{
		cs.Init();
	}
	~CPktMgr() {
		try {
			cs.Del();
		}
		catch(...) {
			Logout(_T("#ERR Occur an exception while running ~CPktMgr()"));
		}
	}

	static CPktMgr*	CreateInstance();
	static CPktMgr*	CreateInstance(int i256PoolSize, int i1024PoolSize);
	static void		DestroyInstance();
	static CPktMgr*	GetInstance() { return pThis; }

	CPktPool*	GetPool(PKT_CELL cell);
	int			GetSize(PKT_CELL cell);
};

// Interface Functions: Designed for easy using
CPacket*	GetPkt(PKT_CELL cell);
void		FreePkt(CPacket* pPkt);

#endif //_PKTMGR_
#endif //_SMALL_NETWORK_