/*
TCP
	- server -
	CreateTcpSocket();
	Bind(server_port);
	Listen();
	Accept(pClientSock); // pClientSock->addr 값이 채워진다.
	Recv()/Send();
	CloseSocket();

	- client -
	CreateTcpSocket();
	Connect(server_ip, server_port);
	Recv()/Send();
	CloseSocket();

UDP
	CreateUdpSocket();
	Bind(SERV_PORT);
	RecvFrom(...)/SendTo(...);
	CloseSocket();
*/

#ifndef _SOCK_
#define _SOCK_

/* gDefine.h에 include 되어 있음
#ifdef INCLUDE_NETWORK_BASE_HEADER
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#endif
*/

#ifndef WINSOCK_LIB_EXPORTS // for DLL
#ifndef LINK_WINSOCK_LIB
#define LINK_WINSOCK_LIB
//#pragma comment(lib, "ws2_32.lib")
#endif
#endif

#define IPv4	// 32bit: 8비트씩 4부분 10진수로 ex)202.30.64.22
//#define IPv6	// 128bit: 16비트씩 8부분 16진수로 ex)2001:0230:abcd:ffff:0000:0000:ffff:1111

#ifdef IPv4
#define IP_BUF_SIZE		16
//#define SOCKADDR_IN_	SOCKADDR_IN
//#define IN_ADDR_		IN_ADDR
#else ifdef IPv6
#define IP_BUF_SIZE		40
//#define SOCKADDR_IN_	SOCKADDR_IN6
//#define IN_ADDR_		IN6_ADDR
#endif

//struct sockaddr_in	addr; // = SOCKADDR_IN
//struct sockaddr_in6	addr; // = SOCKADDR_IN6
//struct in_addr	ip; // = IN_ADDR
//struct in6_addr	ip; // = IN6_ADDR

#ifdef IPv4

class CSock {
public:
	static int iSockDllRefCnt;
protected:
	SOCKET hSock;
	SOCKADDR_IN addr;

	/* addrX의 활용처 ====================
	CSock 클래스가
		TCP Server의 Server로 사용되었을 경우 ☞ 서버 Accept 정보 저장(리슨 포트)
		TCP Server의 Client로 사용되었을 경우 ☞ 클라이언트의 접속 정보 저장(클라이언트 아이피, 클라이언트가 접속된 서버쪽 포트)
		TCP Client로 사용되었을 경우 ☞ 서버의 접속 정보 저장(서버 아이피, 서버의 리슨 포트)
		UDP Recver로 사용되었을 경우 ☞ 사용되지 않음. sendto()를 위한 peer 정보는 별도로 저장
		UDP	Sender로 사용되었을 경우 ☞ 사용되지 않음. peer 정보는 멀티쓰레딩 환경을 고려해 지역변수 사용
	===================================== */

	// 없어도 되지만.. 편의성을 위해(로그 출력시에 자주 씀)
	TCHAR pszIP[IP_BUF_SIZE];

public:
	CSock();
	virtual ~CSock();
	BOOL Init();
	BOOL Finish();

public:
	BOOL CreateTcpSocket();
	BOOL CreateUdpSocket();
	BOOL Shutdown(IN int iFlag);
	void CloseSocket();
	BOOL Bind(IN USHORT usPort);
	BOOL Listen(IN int iBackLog = 100/*SOMAXCONN*/);
	BOOL Accept(OUT CSock* pClientSock);
	BOOL Connect(IN ULONG ulIP, IN USHORT usPort);		// 실행속도 가장 좋음
	BOOL Connect(IN LPCTSTR pszIP, IN USHORT usPort);	// 실행속도 중간

	// for TCP
	int		Recv(OUT char* pRecvBuf, IN int iBufLen);
	int		Send(IN const char* pSendBuf, IN int iSendDataLen);
	
	// for UDP
	int		RecvFrom(OUT char* pRecvBuf, IN int iBufLen, OUT SOCKADDR_IN& addr);
	int		SendTo(IN char* pSendBuf, IN int iSendDataLen, IN SOCKADDR_IN& addr);				// 실행속도 가장 좋음
	int		SendTo(IN char* pSendBuf, IN int iSendDataLen, IN ULONG ulIP, IN USHORT usPort);	// 실행속도 중간
	int		SendTo(IN char* pSendBuf, IN int iSendDataLen, IN LPCTSTR pszIP, IN USHORT usPort);	// 실행속도 가장 느림

public: // Get/Set Methods
	SOCKET	GetSock() { return hSock; }
	void			SetSock(SOCKET s) { hSock = s; }
	LPCTSTR	GetIP();
	USHORT	GetPort();
	SOCKADDR_IN&	GetSockAddr() { return addr; }
	void			SetSockAddr(SOCKADDR_IN* pAddr) { memcpy(&addr, pAddr, sizeof(SOCKADDR_IN)); }

	void		Domain2StringIP(IN LPCTSTR pszDomain, OUT LPTSTR pszIP, IN int iIPBufSize);
	void		NumericIP2StringIP(IN ULONG ulIP, OUT LPTSTR pszIP, IN int iIPBufSize);
	ULONG	StringIP2NumericIP(IN LPCTSTR pszIP);
};

#else ifdef IPv6
#endif

#endif //_SOCK_