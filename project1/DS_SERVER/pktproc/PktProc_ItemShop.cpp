#include "..\stdafx.h"

#ifdef ITEMSHOP_PATCH_200607

#define INCLUDE_ODBC_HEADER
#include "../MyLibrary/gGlobal.h"

void SamplePktProc(WEB_SHOP_PKT_PROC_PARAM* pParam, char* pRecvPkt, COdbc* pDB_Account, COdbc* pDB_NDGame, COdbc* pDB_ItemShop) {
	struct stPARAM {
		int a;
		int b;
		int c;
	};

	struct stRESULT {
		int a;
		int b;
		int c;
	};

	struct stRESULT2 {
		TCHAR psz[256];
	};

	TCHAR pszQuery[256] = {0};
	int iBindColCnt = 0;
	stPARAM param;
	stRESULT res;
	stRESULT2 res2;
	BOOL bRet = FALSE;

	iBindColCnt = 0;
	param.b = 100;
	bRet = pDB_ItemShop->BindParam(1, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER, 0, &param.a);
	//bRet = pDB_ItemShop->BindParam(2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, &param.b);
	bRet = pDB_ItemShop->BindParam(2, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER, 0, &param.c);

	if(pszQuery[0] == NULL)
		_stprintf(pszQuery, _T("{? = CALL WebItemShop.dbo.sp_Test3(%d, N'sample', ?)}"), param.b);
	bRet = pDB_ItemShop->Prepare((SQLTCHAR*)pszQuery);
	if(FALSE == pDB_ItemShop->Exec()) {
		Logout(_T("#ERR %s(%d): pDB_ItemShop->Exec() 실패"), _T(__FILE__), __LINE__);
		pDB_ItemShop->Clear();
		SAFE_DELETE(pDB_ItemShop);
		return;
	}

	while(TRUE) {
		if(0 < pDB_ItemShop->GetResultColCnt()) {
			switch(iBindColCnt++) {
			case 0:
				pDB_ItemShop->BindCol(1, SQL_C_SLONG, &res.a, 0);
				pDB_ItemShop->BindCol(2, SQL_C_SLONG, &res.b, 0);
				pDB_ItemShop->BindCol(3, SQL_C_SLONG, &res.c, 0);
				while(pDB_ItemShop->Fetch()) {
					::PrintConsole(_T("%d %d %d \n"), res.a, res.b, res.c);
				}
				break;

			case 1:
				pDB_ItemShop->BindCol(1, SQL_C_TCHAR, res2.psz, sizeof(res2.psz));
				while(pDB_ItemShop->Fetch()) {
					::PrintConsole(_T("%s \n"), res2.psz);
				}
				break;

			default:
				Logout(_T("실행될 수 없는 코드!!"));
				break;
			}
		}

		if(!pDB_ItemShop->MoreResult()) {
			pDB_ItemShop->Clear();
			break;
		}
	}

	::PrintConsole(_T("param.a=%d param.b=%d param.c=%d \n"), param.a, param.b, param.c);
}

void PktProc_AdmSearch(WEB_SHOP_PKT_PROC_PARAM* pParam, char* pRecvPkt, COdbc* pDB_Account, COdbc* pDB_NDGame, COdbc* pDB_ItemShop) {
	// get packet header
	struct ADM_SEARCH {
		DWORD	dwTransactionId;
		UCHAR	ucIdLen;
		TCHAR	pszUserId[12+1];
	};

	int iRecvPktOffset = 1;
	ADM_SEARCH AdmSearch;
	AdmSearch.dwTransactionId = *(LPDWORD)(pRecvPkt+iRecvPktOffset); iRecvPktOffset += sizeof(DWORD);
	AdmSearch.ucIdLen = pRecvPkt[iRecvPktOffset]; iRecvPktOffset += 1;
	memset(AdmSearch.pszUserId, 0, (12+1)*sizeof(TCHAR));
	memcpy(AdmSearch.pszUserId, pRecvPkt+iRecvPktOffset, AdmSearch.ucIdLen); iRecvPktOffset += AdmSearch.ucIdLen;

	Logout(_T("QueryType=0x%x, TransactId=%d, IdLen=%d, Id=%s"), 
		(UCHAR)pRecvPkt[0],
		AdmSearch.dwTransactionId,
		AdmSearch.ucIdLen,
		AdmSearch.pszUserId);

	//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	// ... spCall
	struct GetAccByNX_Param {
		TCHAR	pszAccount[12];
	};
	struct GetAccByNX_Res {
		int		iRet;
	};

	TCHAR pszQuery[256] = {0};
	int iBindColCnt = 0;
	//GetAccByNX_Param	param;
	GetAccByNX_Res		res;
	BOOL bRet = FALSE;

	iBindColCnt = 0;
	bRet = pDB_Account->BindParam(1, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER, 0, &res.iRet);

	if(pszQuery[0] == NULL) {
		_stprintf(pszQuery, _T("{? = CALL ND_GAME_1.dbo._prDS_GetAccByNX(N'%s')}"), AdmSearch.pszUserId);
		Logout(_T("%s"), pszQuery);
	}
	bRet = pDB_Account->Prepare((SQLTCHAR*)pszQuery);
	if(FALSE == pDB_Account->Exec()) {
		Logout(_T("#ERR %s(%d): pDB_Account->Exec() 실패"), _T(__FILE__), __LINE__);
		pDB_Account->Clear();
		return;
	}

	while(TRUE) {
		if(!pDB_Account->MoreResult()) {
			pDB_Account->Clear();
			break;
		}
	}
	//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

	//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	// send result
	int iPktLen = 9;
	int iOffset = 0; // pSendPkt->m_wsabuf.buf's offset

	CSendPkt* pSendPkt = new CSendPkt;
	pSendPkt->m_wsabuf.buf = new char[iPktLen];
	pSendPkt->m_wsabuf.len = iPktLen;
	pSendPkt->m_gs_info.ipAddr = pParam->pClient->m_RecvOV.m_gs_info.ipAddr;
	pSendPkt->m_bMode = false;

	pSendPkt->m_wsabuf.buf[iOffset++] = (char)0xAA;								// reserved char
	*((LPWORD)(pSendPkt->m_wsabuf.buf+iOffset)) = 6; iOffset += sizeof(WORD);	// content length
	pSendPkt->m_wsabuf.buf[iOffset++] = 16;										// content type
	*((LPDWORD)(pSendPkt->m_wsabuf.buf+iOffset)) = AdmSearch.dwTransactionId; iOffset += sizeof(DWORD);

	// content
	if(res.iRet == 0)
		pSendPkt->m_wsabuf.buf[iOffset++] = 2;	// QR_OK(0), QR_INCORRECT(1), QR_NOTEXIST(2)
	else
		pSendPkt->m_wsabuf.buf[iOffset++] = 0;	// QR_OK(0), QR_INCORRECT(1), QR_NOTEXIST(2)

	if(0 != pSendPkt->SendPacket(pParam->pClient->m_RecvOV.m_socket)) {
		SAFE_DELETE_ARRAY(pSendPkt->m_wsabuf.buf);
		SAFE_DELETE(pSendPkt);
		throw false;
	}
	//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
}

void PktProc_CharSearch(WEB_SHOP_PKT_PROC_PARAM* pParam, char* pRecvPkt, COdbc* pDB_Account, COdbc* pDB_NDGame, COdbc* pDB_ItemShop) {
	// get packet header
	struct CHAR_SEARCH {
		DWORD	dwTransactionId;
		UCHAR	ucNameLen;
		TCHAR	pszName[en_charac_name_length+1];
	};

	int iRecvPktOffset = 1;
	CHAR_SEARCH CharSearch;
	CharSearch.dwTransactionId = *(LPDWORD)(pRecvPkt+iRecvPktOffset); iRecvPktOffset += sizeof(DWORD);
	CharSearch.ucNameLen = pRecvPkt[iRecvPktOffset]; iRecvPktOffset += 1;
	memset(CharSearch.pszName, 0, (en_charac_name_length+1)*sizeof(TCHAR));
	memcpy(CharSearch.pszName, pRecvPkt+iRecvPktOffset, CharSearch.ucNameLen); iRecvPktOffset += CharSearch.ucNameLen;

	Logout(_T("QueryType=0x%x, TransactId=%d, IdLen=%d, Id=%s"), 
		(UCHAR)pRecvPkt[0],
		CharSearch.dwTransactionId,
		CharSearch.ucNameLen,
		CharSearch.pszName);

	//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	// ... spCall
	struct GetCharacByNX_Param {
		TCHAR	pszCharName[en_charac_name_length];
	};
	struct GetCharacByNX_Res {
		int		iRet;
	};

	TCHAR pszQuery[256] = {0};
	int iBindColCnt = 0;
	//GetCharacByNX_Param	param;
	GetCharacByNX_Res		res;
	BOOL bRet = FALSE;

	iBindColCnt = 0;
	bRet = pDB_NDGame->BindParam(1, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER, 0, &res.iRet);

	if(pszQuery[0] == NULL) {
		_stprintf(pszQuery, _T("{? = CALL ND_GAME_1.dbo._prDS_GetCharacByNX(N'%s')}"), CharSearch.pszName);
		Logout(_T("%s"), pszQuery);
	}
	bRet = pDB_NDGame->Prepare((SQLTCHAR*)pszQuery);
	if(FALSE == pDB_NDGame->Exec()) {
		Logout(_T("#ERR %s(%d): pDB_NDGame->Exec() 실패"), _T(__FILE__), __LINE__);
		pDB_NDGame->Clear();
		return;
	}

	while(TRUE) {
		if(!pDB_NDGame->MoreResult()) {
			pDB_NDGame->Clear();
			break;
		}
	}
	//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

	//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	// send result
	int iPktLen = 9;
	int iOffset = 0; // pSendPkt->m_wsabuf.buf's offset

	CSendPkt* pSendPkt = new CSendPkt;
	pSendPkt->m_wsabuf.buf = new char[iPktLen];
	pSendPkt->m_wsabuf.len = iPktLen;
	pSendPkt->m_gs_info.ipAddr = pParam->pClient->m_RecvOV.m_gs_info.ipAddr;
	pSendPkt->m_bMode = false;

	pSendPkt->m_wsabuf.buf[iOffset++] = (char)0xAA;								// reserved char
	*((LPWORD)(pSendPkt->m_wsabuf.buf+iOffset)) = 6; iOffset += sizeof(WORD);	// content length
	pSendPkt->m_wsabuf.buf[iOffset++] = 17;										// content type
	*((LPDWORD)(pSendPkt->m_wsabuf.buf+iOffset)) = CharSearch.dwTransactionId; iOffset += sizeof(DWORD);

	// content
	if(res.iRet == 0)
		pSendPkt->m_wsabuf.buf[iOffset++] = 2;	// QR_OK(0), QR_INCORRECT(1), QR_NOTEXIST(2)
	else
		pSendPkt->m_wsabuf.buf[iOffset++] = 0;	// QR_OK(0), QR_INCORRECT(1), QR_NOTEXIST(2)

	if(0 != pSendPkt->SendPacket(pParam->pClient->m_RecvOV.m_socket)) {
		SAFE_DELETE_ARRAY(pSendPkt->m_wsabuf.buf);
		SAFE_DELETE(pSendPkt);
		throw false;
	}
	//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
}

void PktProc_NewShopItem(WEB_SHOP_PKT_PROC_PARAM* pParam, char* pRecvPkt, COdbc* pDB_Account, COdbc* pDB_NDGame, COdbc* pDB_ItemShop) {}
void PktProc_NewShopGift(WEB_SHOP_PKT_PROC_PARAM* pParam, char* pRecvPkt, COdbc* pDB_Account, COdbc* pDB_NDGame, COdbc* pDB_ItemShop) {}

#endif // ITEMSHOP_PATCH_200607
