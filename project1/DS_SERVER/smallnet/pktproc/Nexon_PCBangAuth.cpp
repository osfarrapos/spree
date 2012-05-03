#include "../../stdafx.h"
#include "../../MAPManager/MAPManager.h"

#ifdef	_NEXON_NETWORK_
#ifdef	_NXPCB_AWARD_200703_

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "../../Mylibrary/gGlobal.h"
#include "../Packet.h"
#include "../PktMgr.h"
#include "../PktQue.h"
#include "../Sock.h"
#include "../SmallNet.h"
#include "./_PktDef.h"
#include "./Nexon_PCBangAuth.h"

extern _j_AccMAPManager *	g_pAccMapManager;		// Account ID KEY MAP
extern _j_ServerInfo		g_ServerInfo;			// 서버정보
//extern CSock	gNexonPCB_sock;
//extern CS		gNexonPCB_cs;
extern int		g_ForestNum;
//extern std::map<string/*strAccount*/, HANDLE /*hThreadSync*/>	gmapNXPCB_LoginSync;
//extern CS														gcsNXPCB_LoginSync;

extern std::map<string, CUser*>		g_mapNXAcc; // <넥슨계정, 유저객체>
extern CS							g_csNXAcc;

extern char		NEXONPCB_AUTH_SERVER_IP[16];
extern WORD		NEXONPCB_AUTH_SERVER_PORT;

#define FLAG_LOGIN	0
#define FLAG_LOGOUT	1

#define	DUMP_PACKET(_packet)\
{\
	int iPktSize = (int)ConvertByteOrder2((WORD)_packet->GetHeader()->GetPktSize()) + 3/*packet header*/;\
	char pLog[1024] = {0};\
	for(int i = 0; i < iPktSize; ++i) {\
		sprintf(pLog+(i*3), "%02X ", (BYTE)*(_packet->GetBuf() + i));\
	}\
	pLog[i*3-1] = NULL;\
	Logout("Dump %3d bytes: %s", iPktSize, pLog);\
	printf("Dump %3d bytes: %s\n", iPktSize, pLog);\
}

#define FILL_UP_HEADER(_packet, _packet_id)\
{\
	CPktHeader*	ph = _packet->GetHeader();\
	ph->SetMark();\
	ph->SetPktSize((WORD)_packet->GetBufOffset());\
	ph->SetPktId(_packet_id);\
}

/*
bool NEXONPCB_Connect(char* pszIP, WORD wPort, int iForestNumOfServer) {
	gNexonPCB_sock.CloseSocket();
	gNexonPCB_sock.CreateTcpSocket();
	BOOL bRet = gNexonPCB_sock.Connect(pszIP, wPort);
	if(bRet == FALSE) {
		Logout("Fail to connect to NEXON PCB AUTH SERVER.");
		printf("Fail to connect to NEXON PCB AUTH SERVER. \n");
		return false;
	}
	Logout("Success to connect NEXON PCB AUTH SERVER.");
	printf("Success to connect NEXON PCB AUTH SERVER. \n");

	gNexonPCB_cs.Lock();
	NEXONPCB_InitNtf(iForestNumOfServer);
	gNexonPCB_cs.Unlock();
	return true;
}
*/

bool NEXONPCB_InitNtf(int iForestNum) {
#ifdef _NXPCB_INACT_AWARD_200703_
	if(CSmallNet::GetInstance() == NULL) return true;
#else
	if(CSmallNet::GetInstance() == NULL) return false;
#endif

	UCHAR	ucGameSn = 18;	// 구룡쟁패 = 18
	UCHAR	ucDomainSn = 0;

	#define SERVER_GROUP_0		"server_group_0"
	#define SERVER_GROUP_1		"server_group_1"
	#define SERVER_GROUP_2		"server_group_2"
	#define SERVER_GROUP_3		"server_group_3"
	#define SERVER_GROUP_4		"server_group_4"
	#define	TEST_GROUP_1		"test_group_1"
	char	pszDomainName[256] = {0};
	
	switch(iForestNum) {
	case 0:		// 의천
		ucDomainSn = 0;
		strcpy(pszDomainName, SERVER_GROUP_0);
		break;
	case 1:
		ucDomainSn = 5;
		strcpy(pszDomainName, TEST_GROUP_1);
	case 12:	// 태산
		ucDomainSn = 1;
		strcpy(pszDomainName, SERVER_GROUP_1);
		break;
	case 13:	// 황하
		ucDomainSn = 2;
		strcpy(pszDomainName, SERVER_GROUP_2);
		break;
	case 14:	// 적벽
		ucDomainSn = 3;
		strcpy(pszDomainName, SERVER_GROUP_3);
		break;
	case 15:	// 천산
		ucDomainSn = 4;
		strcpy(pszDomainName, SERVER_GROUP_4);
		break;
	default:
		// ... error!!
		break;
	}

	CPacket*	pSendPkt = GetPkt(CELL_256);
	*pSendPkt << ucGameSn << ucDomainSn << pszDomainName;
	return ( TRUE == CSmallNet::GetInstance()->Send(NEXONPCB_INIT_NTF, pSendPkt) );

	/*
	FILL_UP_HEADER(pSendPkt, NEXONPCB_INIT_NTF);
	DUMP_PACKET(pSendPkt);

	int iSentBytes = gNexonPCB_sock.Send(pSendPkt->GetBuf(), pSendPkt->GetBufOffset());
	if(iSentBytes == SOCKET_ERROR) {
		bool bRet = NEXONPCB_Connect(NEXONPCB_AUTH_SERVER_IP, NEXONPCB_AUTH_SERVER_PORT, g_ForestNum);
		if(bRet == false) {
			// ...error
			return false;
		}
		else
			iSentBytes = gNexonPCB_sock.Send(pSendPkt->GetBuf(), pSendPkt->GetBufOffset());
	}
	Logout("NEXONPCB_INIT_NTF SentBytes=%d GameSn=%d DomainSn=%d DomainName=%s", iSentBytes, ucGameSn, ucDomainSn, pszDomainName);
	printf("NEXONPCB_INIT_NTF SentBytes=%d GameSn=%d DomainSn=%d DomainName=%s\n", iSentBytes, ucGameSn, ucDomainSn, pszDomainName);
	FreePkt(pSendPkt);
	return true;
	*/
}

bool NEXONPCB_AliveNtf() {
#ifdef _NXPCB_INACT_AWARD_200703_
	if(CSmallNet::GetInstance() == NULL) return true;
#else
	if( NEXONPCB_AUTH_SERVER_PORT )
	{
		if(CSmallNet::GetInstance() == NULL) return false;
	}
	else
		return true;
#endif

	CPacket*	pSendPkt = GetPkt(CELL_256);
	return ( TRUE == CSmallNet::GetInstance()->Send(NEXONPCB_ALIVE_NTF, pSendPkt) );

	/*
	FILL_UP_HEADER(pSendPkt, NEXONPCB_ALIVE_NTF);
	DUMP_PACKET(pSendPkt);

	int iSentBytes = gNexonPCB_sock.Send(pSendPkt->GetBuf(), pSendPkt->GetBufOffset());
	if(iSentBytes == SOCKET_ERROR) {
		bool bRet = NEXONPCB_Connect(NEXONPCB_AUTH_SERVER_IP, NEXONPCB_AUTH_SERVER_PORT, g_ForestNum);
		if(bRet == false) {
			// ...error
			return false;
		}
		else
			iSentBytes = gNexonPCB_sock.Send(pSendPkt->GetBuf(), pSendPkt->GetBufOffset());
	}
	Logout("NEXONPCB_ALIVE_NTF SentBytes=%d", iSentBytes);
	printf("NEXONPCB_ALIVE_NTF SentBytes=%d\n", iSentBytes);
	FreePkt(pSendPkt);
	return true;
	*/
}

bool NEXONPCB_LoginReq(char* pszAccount, char* pszCharName, DWORD dwIP, UCHAR ucCanTry) {
#ifdef _NXPCB_INACT_AWARD_200703_
	if(CSmallNet::GetInstance() == NULL) return true;
#else
	if( NEXONPCB_AUTH_SERVER_PORT )
	{
		if(CSmallNet::GetInstance() == NULL) return false;
	}
	else
		return true;
#endif

	UCHAR	ucLoginFlag = FLAG_LOGIN;

	CPacket*	pSendPkt = GetPkt(CELL_256);
	*pSendPkt << ucLoginFlag << pszAccount << pszCharName;
	memcpy(pSendPkt->GetBuf() + pSendPkt->GetBufOffset(), &dwIP, sizeof(DWORD)); // ip는 little-endian
	pSendPkt->AddBufOffset((int)sizeof(DWORD));
	*pSendPkt << ucCanTry;
	Logout( "NEXONPCB_LOGIN_REQ : Acc=%s IP=%s", pszAccount, ip_char(dwIP) );
	return ( TRUE == CSmallNet::GetInstance()->Send(NEXONPCB_LOGINOUT, pSendPkt) );

	/*
	FILL_UP_HEADER(pSendPkt, NEXONPCB_LOGINOUT);
	DUMP_PACKET(pSendPkt);

	int iSentBytes = gNexonPCB_sock.Send(pSendPkt->GetBuf(), pSendPkt->GetBufOffset());
	if(iSentBytes == SOCKET_ERROR) {
		bool bRet = NEXONPCB_Connect(NEXONPCB_AUTH_SERVER_IP, NEXONPCB_AUTH_SERVER_PORT, g_ForestNum);
		if(bRet == false) {
			// ...error
			return false;
		}
		else
			iSentBytes = gNexonPCB_sock.Send(pSendPkt->GetBuf(), pSendPkt->GetBufOffset());
	}
	Logout("NEXONPCB_LOGIN_REQ SentBytes=%d Acc=%s IP=%08X CanTry=%d", iSentBytes, pszAccount, dwIP, ucCanTry);
	printf("NEXONPCB_LOGIN_REQ SentBytes=%d Acc=%s IP=%08X CanTry=%d \n", iSentBytes, pszAccount, dwIP, ucCanTry);
	FreePkt(pSendPkt);
	return true;
	*/
}

bool NEXONPCB_LogoutNtf(char* pszAccount, char* pszCharName, DWORD dwIP, UCHAR ucCanTry) {
#ifdef _NXPCB_INACT_AWARD_200703_
	if(CSmallNet::GetInstance() == NULL) return true;
#else
	if( NEXONPCB_AUTH_SERVER_PORT )
	{
		if(CSmallNet::GetInstance() == NULL) return false;
	}
	else
		return true;
#endif

	UCHAR	ucLoginFlag = FLAG_LOGOUT;

	CPacket*	pSendPkt = GetPkt(CELL_256);
	*pSendPkt << ucLoginFlag << pszAccount << pszCharName;
	memcpy(pSendPkt->GetBuf() + pSendPkt->GetBufOffset(), &dwIP, sizeof(DWORD)); // ip는 little-endian
	pSendPkt->AddBufOffset((int)sizeof(DWORD));
	*pSendPkt << ucCanTry;
	return ( TRUE == CSmallNet::GetInstance()->Send(NEXONPCB_LOGINOUT, pSendPkt) );

	/*
	FILL_UP_HEADER(pSendPkt, NEXONPCB_LOGINOUT);
	DUMP_PACKET(pSendPkt);

	int iSentBytes = gNexonPCB_sock.Send(pSendPkt->GetBuf(), pSendPkt->GetBufOffset());
	if(iSentBytes == SOCKET_ERROR) {
		bool bRet = NEXONPCB_Connect(NEXONPCB_AUTH_SERVER_IP, NEXONPCB_AUTH_SERVER_PORT, g_ForestNum);
		if(bRet == false) {
			// ...error
			return false;
		}
		else
			iSentBytes = gNexonPCB_sock.Send(pSendPkt->GetBuf(), pSendPkt->GetBufOffset());
	}
	Logout("NEXONPCB_LOGOUT_REQ SentBytes=%d Acc=%s IP=%08X CanTry=%d", iSentBytes, pszAccount, dwIP, ucCanTry);
	printf("NEXONPCB_LOGOUT_REQ SentBytes=%d Acc=%s IP=%08X CanTry=%d \n", iSentBytes, pszAccount, dwIP, ucCanTry);
	FreePkt(pSendPkt);
	return true;
	*/
}

void PKTHDR_NEXONPCB_LoginAck(CUser* pUser, CPacket* pRecvPkt) {
	if(CSmallNet::GetInstance() == NULL) return;

	char	pszAcc[256] = {0};
	char	pszCharName[256] = {0};
	UCHAR	ucResult = 0;
	UCHAR	ucAddressDesc = 0;
	UCHAR	ucAccountDesc = 0;
	UCHAR	ucOption = 0;
	int		iArgument = 0;
	bool	bIsGameBangBillType = false;

	DUMP_PACKET(pRecvPkt);

	*pRecvPkt >> pszAcc >> pszCharName >> ucResult >> ucAddressDesc >> ucAccountDesc >> ucOption >> iArgument >> bIsGameBangBillType;

	Logout("NEXONPCB_LOGIN_ACK RecvedBytes=%d Acc=%s Result=%d AddrDesc=%c AccDesc=%c Option=%d Argument=%d IsGameBangBillType=%d",
		pRecvPkt->GetBufOffset(), pszAcc, ucResult, ucAddressDesc, ucAccountDesc, ucOption, iArgument, bIsGameBangBillType);
	//printf("NEXONPCB_LOGIN_ACK RecvedBytes=%d Acc=%s Result=%d AddrDesc=%c AccDesc=%c Option=%d Argument=%d IsGameBangBillType=%d \n",
	//	pRecvPkt->GetBufOffset(), pszAcc, ucResult, ucAddressDesc, ucAccountDesc, ucOption, iArgument, bIsGameBangBillType);

	// 패킷 받은 이후 처리
	if(pUser != NULL) {
		pUser->NexonPcbInfo.ucResult = ucResult;
		pUser->NexonPcbInfo.ucAddrDesc = ucAddressDesc;
		pUser->NexonPcbInfo.ucAccDesc = ucAccountDesc;
		pUser->NexonPcbInfo.ucOption = ucOption;
		pUser->NexonPcbInfo.iArgument = iArgument;
		pUser->NexonPcbInfo.bIsPCBBillType = bIsGameBangBillType;
	}
}

void PKTHDR_NEXONPCB_LOGINOUT(CNetClient* pClient, CPacket* pRecvPkt) {
	if(CSmallNet::GetInstance() == NULL) return;

	char	pszAcc[256] = {0};
	char	pszCharName[256] = {0};
	UCHAR	ucResult = 0;
	UCHAR	ucAddressDesc = 0;
	UCHAR	ucAccountDesc = 0;
	UCHAR	ucOption = 0;
	int		iArgument = 0;
	bool	bIsGameBangBillType = false;

	//DUMP_PACKET(pRecvPkt);

	*pRecvPkt >> pszAcc >> pszCharName >> ucResult >> ucAddressDesc >> ucAccountDesc >> ucOption >> iArgument >> bIsGameBangBillType;

	Logout("NEXONPCB_LOGIN_ACK RecvedBytes=%d Acc=%s Result=%d AddrDesc=%c AccDesc=%c Option=%d Argument=%d IsGameBangBillType=%d",
		pRecvPkt->GetBufOffset(), pszAcc, ucResult, ucAddressDesc, ucAccountDesc, ucOption, iArgument, bIsGameBangBillType);
	//printf("NEXONPCB_LOGIN_ACK RecvedBytes=%d Acc=%s Result=%d AddrDesc=%c AccDesc=%c Option=%d Argument=%d IsGameBangBillType=%d \n",
	//	pRecvPkt->GetBufOffset(), pszAcc, ucResult, ucAddressDesc, ucAccountDesc, ucOption, iArgument, bIsGameBangBillType);

	if(ucResult == 3/*Terminate*/) {
		// GS에 Logout Req
		//PUSER pUser = g_pAccMapManager->Find_User(pszAcc);
		std::string		strAcc(pszAcc);
		g_csNXAcc.Lock();
		CUser* pUser = g_mapNXAcc[strAcc];
		g_csNXAcc.Unlock();
		if(pUser == NULL) {
			Logout("NEXONPCB_TERMINATE_NTF Unable to find a user(%s) from g_pAccMapManager.", pszAcc);
			printf("NEXONPCB_TERMINATE_NTF Unable to find a user(%s) from g_pAccMapManager.\n", pszAcc);
			return;
		}

		_h_DS_Com*		pDSComm = NULL;
		pDSComm = g_ServerInfo.GetDSCom(pUser->iServer);
		if(pDSComm == NULL) {
			Logout("Unable to find GS. Fail of pDSComm = g_ServerInfo.GetDSCom(pUser->iServer=%d)", pUser->iServer);
			printf("Unable to find GS. Fail of pDSComm = g_ServerInfo.GetDSCom(pUser->iServer=%d) \n", pUser->iServer);
			return;
		}

		CMSG_USER_LOGOUT_DS_GS	LogoutReq;
		LogoutReq.usLength = sizeof(CMSG_USER_LOGOUT_DS_GS);
		LogoutReq.cMessage = CMSG_NO_USER_LOGOUT_DS_GS;
		memcpy(LogoutReq.cNDAccount, pszAcc, en_max_lil+1);
		LogoutReq.cReason = 3;
		pDSComm->Send(LogoutReq.usLength, (char*)&LogoutReq);
		Logout("CMSG_NO_USER_LOGOUT_DS_GS Acc=%s cReason=%d", LogoutReq.cNDAccount, LogoutReq.cReason);
		printf("CMSG_NO_USER_LOGOUT_DS_GS Acc=%s cReason=%d \n", LogoutReq.cNDAccount, LogoutReq.cReason);
		return;
	}
	else {
		// Login 패킷에 대한 응답
		
		//PUSER pUser = g_pAccMapManager->Find_User(pszAcc);
		std::string		strAcc(pszAcc);
		g_csNXAcc.Lock();
		CUser* pUser = g_mapNXAcc[strAcc];
		g_csNXAcc.Unlock();
		if(pUser == NULL) {
			Logout("NEXONPCB_LOGIN_ACK Unable to find a user(%s) from g_pAccMapManager.", pszAcc);
			//printf("NEXONPCB_LOGIN_ACK Unable to find a user(%s) from g_pAccMapManager. \n", pszAcc);
			return;
		}
		else {
			pUser->NexonPcbInfo.ucResult = ucResult;
			pUser->NexonPcbInfo.ucAddrDesc = ucAddressDesc;
			pUser->NexonPcbInfo.ucAccDesc = ucAccountDesc;
			pUser->NexonPcbInfo.ucOption = ucOption;
			pUser->NexonPcbInfo.iArgument = iArgument;
			pUser->NexonPcbInfo.bIsPCBBillType = bIsGameBangBillType;

			// GS에 피씨방 정보 전송
			char pSend[1024] = {0,};
			MSG_COMMON_NTF	ntf;
			ntf.usLength = sizeof(u_short) + sizeof(u_char) + sizeof(WORD) + (en_max_lil+1) + sizeof(NEXON_PCB_INFO);
			ntf.ucMessage = CMSG_NO_COMMON_NTF;
			ntf.wMode = 2;	// 2: DS2GS_NTF	[유저의 피씨방 정보]

			memcpy(ntf.NXPCB_Info.acc, pUser->cNDAccount, en_max_lil);
			ntf.NXPCB_Info.acc[en_max_lil] = NULL;
			memcpy(&ntf.NXPCB_Info.info, &pUser->NexonPcbInfo, sizeof(NEXON_PCB_INFO));

			_h_DS_Com*		pDSComm = NULL;
			pDSComm = g_ServerInfo.GetDSCom(pUser->iServer);
			if(pDSComm == NULL) {
				Logout("Unable to find GS. Fail of pDSComm = g_ServerInfo.GetDSCom(pUser->iServer=%d)", pUser->iServer);
				//printf("Unable to find GS. Fail of pDSComm = g_ServerInfo.GetDSCom(pUser->iServer=%d) \n", pUser->iServer);
				return;
			}

			pDSComm->Send(ntf.usLength, (char*)&ntf);
			Logout("CMSG_NO_COMMON_NTF Acc=%s", ntf.NXPCB_Info.acc);
			//printf("CMSG_NO_COMMON_NTF Acc=%s \n", ntf.NXPCB_Info.acc);
			return;
		}

		/*
		bool			bUserValid = false;
		std::string		strAcc(pszAcc);
		HANDLE			h = NULL;

		gcsNXPCB_LoginSync.Lock();
		if(gmapNXPCB_LoginSync.find(strAcc) != gmapNXPCB_LoginSync.end()) {
			h = gmapNXPCB_LoginSync[strAcc];
		}
		gmapNXPCB_LoginSync.erase(strAcc);
		gcsNXPCB_LoginSync.Unlock();

		if(h != NULL) {
			PUSER pUser = g_pAccMapManager->Find_User(pszAcc);
			if(pUser == NULL) {
				Logout("NEXONPCB_LOGIN_ACK Unable to find a user(%s) from g_pAccMapManager.", pszAcc);
				printf("NEXONPCB_LOGIN_ACK Unable to find a user(%s) from g_pAccMapManager. \n", pszAcc);
				return;
			}
			else {
				pUser->NexonPcbInfo.ucResult = ucResult;
				pUser->NexonPcbInfo.ucAddrDesc = ucAddressDesc;
				pUser->NexonPcbInfo.ucAccDesc = ucAccountDesc;
				pUser->NexonPcbInfo.ucOption = ucOption;
				pUser->NexonPcbInfo.iArgument = iArgument;
				pUser->NexonPcbInfo.bIsPCBBillType = bIsGameBangBillType;
			}

			SetEvent(h);
		}
		else {
			Logout("NEXONPCB_LOGIN_ACK Unable to find a user(%s) from gmapNXPCB_LoginSync.", pszAcc);
			printf("NEXONPCB_LOGIN_ACK Unable to find a user(%s) from gmapNXPCB_LoginSync. \n", pszAcc);
		}
		*/

		return;
	}
}

#endif //_NXPCB_AWARD_200703_
#endif //_NEXON_NETWORK_
