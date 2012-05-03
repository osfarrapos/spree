//TcpInit.h by nanggek
#ifndef __TCPINIT_H__
#define __TCPINIT_H__

#include <winsock2.h>
#include <afxmt.h>
#include "WW2Protocol.h"
#include "NANGReg.h"
class TcpService;
class TcpInit
{
public:
	TcpInit();
	virtual ~TcpInit();
public:
	HANDLE hbannerFile;
	HANDLE hpatchFile;
	DWORD m_dwtotlen;
	DWORD m_patchlen;
public:
	ServerInfo dummyInfo;
	SOCKET dummySock;
	HANDLE dummyThread;
	WSAEVENT	hRecvEvent;
	bool        bConnect;
	BOOL Sendable;
	BOOL BreakFlag;
	_SendQueue	SendQueue;
	TcpService* m_ptcpService;
		
public:
	void CloseDummy();
	void ProcTransPatch();
	void ProcInfoPatch();
	void SendReadPatch();
	void SendReadBanner();
	void SendDummyInit(WORD wVer, BYTE cBanner);
	BOOL ProcessQueue();
	int InsertQueue(char *Packet, int MsgSize, int TotalSendSize );
	BOOL SendPacket(char *Packet, int MsgSize );
	int ProcABATE(char* Msg);
	BOOL TcpDummyConnect(CString szip,long lport);
	void ProcWhenServer();
	void ProcTransBanner();
	void ProcInfoBanner();
	void ProcDInit();

	cmsg_base			msgbase;
	ccmsg_init			cmsginit;
	ccmsg_redbanner		cmsgredbanner;
	ccmsg_redpatch		cmsgredpatch;

	
	cmsg_servlist		msgservlist;
	csmsg_init			msginit;
	csmsg_infobanner	msginfobanner;
	csmsg_transbanner	msgtransbanner;
	csmsg_infopatch		msginfopatch;
	csmsg_transpatch	msgtranspatch;
};
#endif