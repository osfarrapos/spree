#include "../stdafx.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "../Mylibrary/gGlobal.h"
#include "./Sock.h"

int CSock::iSockDllRefCnt = 0;

CSock::CSock() {
	Init();

	if(iSockDllRefCnt++ == 0) {
		WSADATA wsaData;
		if(0 != WSAStartup(MAKEWORD(2,2), &wsaData)) {
			Logout(_T("#ERR(%d) fail WSAStartup(2.2). WSAGetLastError=%d"), __LINE__, WSAGetLastError());
			assert(!"#ERR 소켓 DLL 초기화 실패");
		}
	}
}

CSock::~CSock() {
	try {
		CloseSocket();

		// release socket dll
		if(--iSockDllRefCnt <= 0) {
			WSACleanup();
		}
	}
	catch(...) {
		Logout(_T("#ERR Occur an exception while running ~CSock()"));
	}
}

BOOL CSock::Init() {
	hSock = INVALID_SOCKET;
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	memset(pszIP, 0, IP_BUF_SIZE*sizeof(TCHAR));
	return TRUE;
}

BOOL CSock::Finish() {
	CloseSocket();
	return TRUE;
}

BOOL CSock::CreateTcpSocket() {
	if(hSock != INVALID_SOCKET) return TRUE;

	hSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(hSock == INVALID_SOCKET) {
		Logout(_T("#ERR(%d) fail WSASocket(SOCK_STREAM). WSAGetLastError=%d"), __LINE__, WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

BOOL CSock::CreateUdpSocket() {
	if(hSock != INVALID_SOCKET) return TRUE;

	hSock = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(hSock == INVALID_SOCKET) {
		Logout(_T("#ERR(%d) fail WSASocket(SOCK_DGRAM). WSAGetLastError=%d"), __LINE__, WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

void CSock::CloseSocket() {
	if(hSock != INVALID_SOCKET) {
		//Shutdown(SD_BOTH);
		closesocket(hSock);
		hSock = INVALID_SOCKET;
	}
}

BOOL CSock::Shutdown(int iFlag) {
	/* iFlag :
		#define SD_RECEIVE      0x00
		#define SD_SEND         0x01
		#define SD_BOTH         0x02
	*/

	if(SOCKET_ERROR == shutdown(hSock, iFlag)) {
		Logout(_T("#ERR(%d) fail shutdown(iFlag=%d). WSAGetLastError=%d"), __LINE__, iFlag, WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

BOOL CSock::Bind(USHORT usPort) {
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(usPort);
	if(SOCKET_ERROR == bind(hSock, (SOCKADDR*)&addr, sizeof(SOCKADDR))) {
		Logout(_T("#ERR(%d) fail bind(). WSAGetLastError=%d"), __LINE__, WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

BOOL CSock::Listen(IN int iBackLog) {
	if(SOCKET_ERROR == listen(hSock, iBackLog)) {
		Logout(_T("#ERR(%d) fail listen(). WSAGetLastError=%d"), __LINE__, WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

BOOL CSock::Accept(OUT CSock* pClientSock) {
	int	iAddrLen = sizeof(SOCKADDR);
	pClientSock->hSock = WSAAccept(hSock, (SOCKADDR*)&pClientSock->addr, &iAddrLen, NULL, 0);
	if(pClientSock->hSock == INVALID_SOCKET) {
		Logout(_T("#ERR(%d) fail WSAAccept(). WSAGetLastError=%d"), __LINE__, WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

BOOL CSock::Connect(IN ULONG ulIP, IN USHORT usPort) {
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ulIP;
	addr.sin_port = htons(usPort);
	if( INVALID_SOCKET == hSock )
		CreateTcpSocket();

	if(SOCKET_ERROR == connect(hSock, (SOCKADDR*)&addr, sizeof(SOCKADDR))) {
		Logout(_T("#ERR(%d) fail connect(). WSAGetLastError=%d"), __LINE__, WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

BOOL CSock::Connect(IN LPCTSTR pszIP, IN USHORT usPort) {
#ifdef _UNICODE
	char mbcsIP[IP_BUF_SIZE] = {0}; // 임시로 생성되었다 삭제될 메모리이므로, IPv4/IPv6 버퍼 크기에 신경쓰지 말고, 코드 깔끔하게..
	WideCharToMultiByte(CP_ACP, 0, pszIP, -1, mbcsIP, IP_BUF_SIZE, NULL, NULL);
	return Connect(inet_addr(mbcsIP), usPort);
#else
	return Connect(inet_addr(pszIP), usPort);
#endif
}

/*
return:
	- recved bytes
	- SOCKET_ERROR
	- 0 = closed connection
*/
int CSock::Recv(OUT char* pRecvBuf, IN int iBufLen) {
	int iRecvedBytes = recv(hSock, pRecvBuf, iBufLen, 0);
	if(iRecvedBytes == SOCKET_ERROR) {
		Logout(_T("#ERR(%d) fail recv(). WSAGetLastError=%d"), __LINE__, WSAGetLastError());
		return SOCKET_ERROR;
	}
	return iRecvedBytes;
}

/*
return:
	- sent bytes
	- SOCKET_ERROR
	- 0 = closed connection
*/
int CSock::Send(IN const char* pSendBuf, IN int iSendDataLen) {
	int iTotalSentBytes = 0;
	int iSentBytes = 0;
	while(iTotalSentBytes < iSendDataLen) {
		iSentBytes = send(hSock, pSendBuf + iTotalSentBytes, iSendDataLen - iTotalSentBytes, 0);
		if(iSentBytes == SOCKET_ERROR) {
			Logout(_T("#ERR(%d) fail send(iSendDataLen=%d). WSAGetLastError=%d"), __LINE__, iSendDataLen, WSAGetLastError());
			return SOCKET_ERROR;
		}
		iTotalSentBytes += iSentBytes;
	}
	return iTotalSentBytes;
}

/*
return:
	- recved bytes
	- SOCKET_ERROR
	- 0 = closed connection
*/
int CSock::RecvFrom(OUT char* pRecvBuf, IN int iBufLen, OUT SOCKADDR_IN& addr) {
	int iAddrSize = sizeof(SOCKADDR);
	int iRecvedBytes = recvfrom(hSock, pRecvBuf, iBufLen, 0, (SOCKADDR*)&addr, &iAddrSize);
	if(iRecvedBytes == SOCKET_ERROR) {
		Logout(_T("#ERR(%d) fail recvfrom(). WSAGetLastError=%d"), __LINE__, WSAGetLastError());
		return SOCKET_ERROR;
	}
	return iRecvedBytes;
}

/*
return:
	- sent bytes
	- SOCKET_ERROR
	- 0 = closed connection
*/
int CSock::SendTo(IN char* pSendBuf, IN int iSendDataLen, IN SOCKADDR_IN& addr) {
	int iSentBytes = sendto(hSock, pSendBuf, iSendDataLen, 0, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	if(iSentBytes == SOCKET_ERROR) {
		Logout(_T("#ERR(%d) fail sendto(). WSAGetLastError=%d"), __LINE__, WSAGetLastError());
		return SOCKET_ERROR;
	}
	return iSentBytes;
}

int CSock::SendTo(IN char* pSendBuf, IN int iSendDataLen, IN ULONG ulIP, IN USHORT usPort) {
	SOCKADDR_IN addr; // 멀티쓰레딩 환경을 고려하여, 맴버 변수 대신 로컬 변수 사용
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ulIP;
	addr.sin_port = htons(usPort);
	return SendTo(pSendBuf, iSendDataLen, addr);
}

int CSock::SendTo(IN char* pSendBuf, IN int iSendDataLen, IN LPCTSTR pszIP, IN USHORT usPort) {
	return SendTo(pSendBuf, iSendDataLen, StringIP2NumericIP(pszIP), usPort);
}

LPCTSTR CSock::GetIP() {
	if(addr.sin_addr.s_addr == 0)
		return NULL;

	if(pszIP[0] == NULL) {
		char* mbcsIP = inet_ntoa(addr.sin_addr);

#ifdef _UNICODE
		MultiByteToWideChar(CP_ACP, 0, mbcsIP, -1, pszIP, IP_BUF_SIZE);
#else
		strcpy(pszIP, mbcsIP);
#endif
		return pszIP;
	}
	else
		return pszIP;
}

USHORT CSock::GetPort() { 
	if(addr.sin_port == 0)
		return 0;

	return htons(addr.sin_port);
}

void Domain2StringIP(IN LPCTSTR pszDomain, OUT LPTSTR pszIP, IN int iIPBufSize) {
#ifdef _UNICODE
	char mbcsDomain[256] = {0};
	WideCharToMultiByte(CP_ACP, 0, pszDomain, -1, mbcsDomain, 256, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, inet_ntoa(*(IN_ADDR*)gethostbyname(mbcsDomain)->h_addr_list[0]), -1, pszIP, iIPBufSize);
#else
	strcpy(pszIP, inet_ntoa(*(IN_ADDR*)gethostbyname(pszDomain)->h_addr_list[0]));
#endif
}

void NumericIP2StringIP(IN ULONG ulIP, OUT LPTSTR pszIP, IN int iIPBufSize) {
#ifdef _UNICODE
	MultiByteToWideChar(CP_ACP, 0, inet_ntoa(*(IN_ADDR*)&ulIP), -1, pszIP, iIPBufSize);
#else
	strcpy(pszIP, inet_ntoa(*(IN_ADDR*)&ulIP));
#endif
}

ULONG StringIP2NumericIP(IN LPCTSTR pszIP) {
#ifdef _UNICODE
	char mbcsIP[IP_BUF_SIZE] = {0}; // 임시로 생성되었다 삭제될 메모리이므로, IPv4/IPv6 버퍼 크기에 신경쓰지 말고, 코드 깔끔하게..
	WideCharToMultiByte(CP_ACP, 0, pszIP, -1, mbcsIP, IP_BUF_SIZE, NULL, NULL);
	return inet_addr(mbcsIP);
#else
	return inet_addr(pszIP);
#endif
}
