#ifdef	_SMALL_NETWORK_

#ifndef	_SMALL_NET_
#define	_SMALL_NET_

class CNetClient {
private:
	CS			cs;

	int			iUid;
	// 초기값 = -1, 정상 사용중 > 0, 정상 사용중 전 임시 사용중 = 0
	// 서버에 접속한 클라이언트에게 Uid를 할당하기 전까지 임시로 0 값을 줌

	CSock*		pSock;
	CPktQue*	pRecvQue;
	CPktQue*	pSendQue;

	double		dRecvedBytes;
	double		dSentBytes;

public:
	CNetClient() {
		cs.Init();
		Create();
	}
	~CNetClient() {
		try {
			SAFE_DEL(pSock);
			SAFE_DEL(pRecvQue);
			SAFE_DEL(pSendQue);
			cs.Del();
		}
		catch(...) {
			Logout(_T("#ERR Occur exception while running CNetClient::~CNetClient()"));
		}
	}
	BOOL Create() {
		pSock = new CSock; assert(pSock);
		pRecvQue = new CPktQue; assert(pRecvQue);
		pSendQue = new CPktQue; assert(pSendQue);

		Init();
		return TRUE;
	}
	BOOL Init() {
		iUid = -1;
		pSock->Init();
		pRecvQue->Init();
		pSendQue->Init();
		dRecvedBytes = 0;
		dSentBytes = 0;
		return TRUE;
	}
	void Finish() {
		cs.Lock();
		if(iUid < 0) {
			cs.Unlock();
			return;
		}

		iUid = -1;
		pSock->Finish();
		cs.Unlock();
	}

	int			GetUid() { cs.Lock(); int iUid = this->iUid; cs.Unlock(); return iUid; }
	void		SetUid(int iUid) { cs.Lock(); this->iUid = iUid; cs.Unlock(); }
	CSock*		GetSock() { return pSock; }
	CPktQue*	GetRecvQue() { return pRecvQue; }
	CPktQue*	GetSendQue() { return pSendQue; }

	double		GetRecvedBytes() { return dRecvedBytes; }
	void		AddRecvedBytes(double dRecvedBytes) { this->dRecvedBytes += dRecvedBytes; }
	double		GetSentBytes() { return dSentBytes; }
	void		AddSentBytes(double dSentBytes) { this->dSentBytes += dSentBytes; }
};

struct PACKET_HANDLER_FUNCTION_POINTER_WRAPPER {
	void (*pf)(IN CNetClient*, IN CPacket*);
};

enum NET_EVENT {
	NETEVENT_CONNECTED,
	NETEVENT_DISCONNECTED,
};

class CSmallNet {
public:
	struct SMALL_NET_INFO {
		int			i256PoolSize;		// 미리 생성할 256 패킷 개수
		int			i1024PoolSize;	//                      1024
		int			iRecvProcNum;	// RecvProc 쓰레드 생성 개수
		int			iSendProcNum;	// SendProc
		int			iPktProcNum;		// PktProc

		SMALL_NET_INFO() {
			i256PoolSize = i1024PoolSize = 10;
			iRecvProcNum = iSendProcNum = iPktProcNum = 1;
		}
		const SMALL_NET_INFO& operator = (const SMALL_NET_INFO& _sni) {
			i256PoolSize = _sni.i256PoolSize;
			i1024PoolSize = _sni.i1024PoolSize;
			iRecvProcNum = _sni.iRecvProcNum;
			iSendProcNum = _sni.iSendProcNum;
			iPktProcNum = _sni.iPktProcNum;
			return _sni;
		}
		void FillInfo(int i256PoolSize, int i1024PoolSize, int iRecvProcNum, int iSendProcNum, int iPktProcNum) {
			this->i256PoolSize = i256PoolSize;
			this->i1024PoolSize = i1024PoolSize;
			this->iRecvProcNum = iRecvProcNum;
			this->iSendProcNum = iSendProcNum;
			this->iPktProcNum = iPktProcNum;
		}
	};

	struct CONNECT_INFO {
		TCHAR	pszIp[IPV4_BUF_SIZE];
		TCHAR	pszDomain[256];
		WORD	wPort;

		CONNECT_INFO() {
			memset(pszIp, 0, IPV4_BUF_SIZE);
			memset(pszDomain, 0, 256);
			wPort = 0;
		}
		const CONNECT_INFO& operator = (const CONNECT_INFO& _ci) {
			memcpy(pszIp, _ci.pszIp, IPV4_BUF_SIZE);
			memcpy(pszDomain, _ci.pszDomain, 256);
			wPort = _ci.wPort;
			return _ci;
		}
		void FillInfo(LPCTSTR pszIp, LPCTSTR pszDomain, WORD wPort) {
			if(pszIp != NULL) {
				memcpy(this->pszIp, pszIp, IPV4_BUF_SIZE);
				this->pszIp[IPV4_BUF_SIZE-1] = NULL;
			}
			if(pszDomain != NULL) {
				memcpy(this->pszDomain, pszDomain, 256);
				this->pszDomain[256-1] = NULL;
			}
			this->wPort = wPort;
		}

	};

	struct THREAD_PARAM {
		CSmallNet*	pNet;
		HANDLE			hThread;

		void FillInfo(CSmallNet* pSmallNet, HANDLE hKill) {
			pNet = pSmallNet;
			hThread = hKill;
		}
	};

public:
	static CSmallNet*		pThis;
	static PACKET_HANDLER_FUNCTION_POINTER_WRAPPER* pPHFPW;
	BOOL(*pfNetEvent)(IN CNetClient*, IN const NET_EVENT);	// Callback for network events
	void(*pfPktHandler)(IN CNetClient*, IN CPacket*);

private:
	CS				csDestroyThis;

	CNetClient*		pClient;
	CS				csClient;
	HANDLE			hPktProc;
	HANDLE			hSendProc;

	SMALL_NET_INFO	sni;
	CONNECT_INFO	ci;

	// For management threads created, especially killing all threads
	HANDLE			hThreadSync;	// After initiate, all threads are started to run
	BOOL			bKillThread;
	HANDLE			hKillThread[MAXIMUM_WAIT_OBJECTS];	// MAXIMUM_WAIT_OBJECTS = 64
	int				iCreatedThreadCnt;
	BOOL			bConnected;

public:
	CSmallNet();
	~CSmallNet();

	static CSmallNet*	CreateInstance(SMALL_NET_INFO&, CONNECT_INFO&, BOOL(*)(CNetClient*, const NET_EVENT), void(*)(CNetClient*, CPacket*));
	static void			DestroyInstance();
	static CSmallNet*	GetInstance() { return pThis; }

	BOOL	Send(WORD wPktId, CPacket* pSendPkt);
	
	static unsigned __stdcall RecvProc(void* pParam);	// 수신한 패킷을 RecvQue에 삽입
	static unsigned __stdcall SendProc(void* pParam);	// SendQue로부터 패킷을 꺼내서 전송
	static unsigned __stdcall PktProc(void* pParam);	// RecvQue로부터 패킷을 꺼내서 처리

	//CNetClient*	GetClient() { return pClient; }
	CONNECT_INFO&	GetConnectInfo() { return ci; }
	BOOL IsConnected()		{ return bConnected; }
	VOID ResetConnected()	{ bConnected = FALSE; }

	BOOL	_Create();
	void	_Destroy();
	BOOL	_Connect();
	void	_Disconnect();
};

#define BEGIN_PKT_HANDLER_MAP(_HDR_)\
	void _HDR_(CNetClient* _pClient, CPacket* _pRecvPkt) {\
		PACKET_HANDLER_FUNCTION_POINTER_WRAPPER* pWrapper = CSmallNet::pPHFPW;\
		if(pWrapper == NULL) {\
			pWrapper = new PACKET_HANDLER_FUNCTION_POINTER_WRAPPER[END_OF_PKT_DEFINITION];\
			assert(pWrapper);
#define DECLARE_HANDLER(_PktId) pWrapper[_PktId].pf = PKTHDR_##_PktId;
#define END_PKT_HANDLER_MAP\
		}\
		_pRecvPkt->Init();\
		pWrapper[_pRecvPkt->GetHeader()->GetPktId()].pf(_pClient, _pRecvPkt);\
	}

#endif //_SMALL_NET_
#endif //_SMALL_NETWORK_