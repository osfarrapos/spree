// XNetwork_XTrap.cpp : XTrap 관련
///////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "XKernel.h"
#include "XNetwork.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"

#ifdef _XTAIWANESE
#else
	#ifdef _XVIETNAMESE
	#else
		#ifdef _ACCLAIM_VERSION
		#else
			#ifdef _PWC_VERSION

			#else
				#ifdef _XJAPANESE

				#else
					#ifdef _XRUSSIAN

					#else
					//#define _XENABLE_XTRAP
					#endif				
				#endif				
			#endif
		#endif
	#endif
#endif

#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG)
	#ifdef _XENABLE_XTRAP
		#define _XUSE_XTRAP
	#endif
#endif

#ifdef _XUSE_XTRAP
	#include "XTrap.h"
	#pragma comment(lib, "./Library/XTrapIC_mtDll.lib")
#endif

void _XNetwork::MSGXTrapSCRequest(MSG_XTRAP_SC_REQUEST* request)
{
//	unsigned char c_hashcode[CREATEKF_OUT_SIZE*2];
	XC_CLIENTBUF	c_hashcode[2];
	ZeroMemory(c_hashcode, sizeof(c_hashcode));

#ifdef _XUSE_XTRAP
	int ret = CreateKFEx(NULL, (PUCHAR)request->InputCode, 2, (PUCHAR)c_hashcode);

	if(ret == 0)
	{
		_XLog("Create Key Fail ===");
	}
	else
	{
		_XLog("Create Key Success ===");
	}
#endif
	
	int size = sizeof(MSG_XTRAP_CS_HASHCODE);
	MSG_XTRAP_CS_HASHCODE xtrapcs;
	ZeroMemory(&xtrapcs, size);

	xtrapcs.header.sLength = size;
	xtrapcs.ucMessage = MSG_NO_CHECK_XTRAP;
	xtrapcs.ucMode = en_xtrap_cs_hashcode;
	
	memcpy(&xtrapcs.hashcode, c_hashcode, CREATEKF_OUT_SIZE*2);

	_XSendPacket_Encapsulate((char*)&xtrapcs);

	_XLog("Send XTrap CS HashCode ===");
}


void _XNetwork::MSGXTrapSCExit(MSG_XTRAP_SC_EXIT* exit)
{
	InsertChatString(_T("XTrap_SC_Exit"), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	_XLog("Error<XTrap> : exitcode [%d]", exit->ucExitCode);

#ifdef _XUSE_XTRAP
	SendLogToMgr();
#endif

	// 처리가 더 필요한가?
//	ProcessNetworkError(); 
}

void _XNetwork::MSGXTrapSCErrorNo(MSG_XTRAP_SC_ERRNO* err)
{
	InsertChatString(_T("XTrap_SC_Err_No"), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
	_XLog("Error<XTrap> : no[%d]", err->ucError);

#ifdef _XUSE_XTRAP
	SendLogToMgr();
#endif

//	ProcessNetworkError();
}

void _XNetwork::SendXTrapSetOptGameInfo(void)
{
	TCHAR lastselectedserver[32];
	_snprintf(lastselectedserver, sizeof(lastselectedserver), "%d", ((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_LastSelectedServer);

#ifdef _XUSE_XTRAP
	SetOptGameInfo(g_pLocalUser->m_UserName, lastselectedserver, NULL, NULL, 0);
#endif
}