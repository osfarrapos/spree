#ifdef	_SMALL_NETWORK_

#ifndef	_PKT_DEF_
#define	_PKT_DEF_

BOOL NetEventProc(CNetClient* pClient, const NET_EVENT NetEvent);
void PktHandler(CNetClient* _pClient, CPacket* _pRecvPkt);


#if defined(_NEXON_NETWORK_)
enum PKT_DEFINITION {
	NEXONPCB_LOGINOUT		= 39,
	NEXONPCB_INIT_NTF		= 40,
	NEXONPCB_ALIVE_NTF		= 100,

	END_OF_PKT_DEFINITION
};

//bool	NEXONPCB_Connect(char* pszIP, WORD wPort, int iForestNumOfServer);
bool	NEXONPCB_InitNtf(int iForestNum);
bool	NEXONPCB_AliveNtf();
bool	NEXONPCB_LoginReq(char* pszAccount, char* pszCharName, DWORD dwIP, UCHAR ucCanTry = 0);
bool	NEXONPCB_LogoutNtf(char* pszAccount, char* pszCharName, DWORD dwIP, UCHAR ucCanTry = 0);
void	PKTHDR_NEXONPCB_LoginAck(CUser* pUser, CPacket* pRecvPkt);

//#define NEXONPCB_AUTH_SERVER_IP		"211.218.237.83"	// test 인증 서버
//#define NEXONPCB_AUTH_SERVER_PORT	10118
//#define NEXONPCB_AUTH_SERVER_IP		"211.218.232.68"		// 정식 인증 서버
//#define NEXONPCB_AUTH_SERVER_PORT	10003

#endif

#endif // _PKT_DEF_
#endif //_SMALL_NETWORK_