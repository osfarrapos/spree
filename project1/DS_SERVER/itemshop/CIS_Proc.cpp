#include "..\stdafx.h"

#ifdef _CIS_20070326_

#include "..\MAPManager\MAPManager.h"
#include "..\Billing\BillingSvrLink.h"
#include "..\Ranking_Manager\Ranking_Manager.h"

#define INCLUDE_ODBC_HEADER
#include "../MyLibrary/gGlobal.h"
#include "../MyLibrary/gDefine.h"
#include "./CIS.h"

void CIS_ShowCashInvenReq(MSG_COMMON* pRecv, COdbc* pCIS, _h_DS_Com* pDSComm) {
	MSG_COMMON	ack;
	ack.ucMessage = CMSG_NO_COMMON_ACK;
	ack.wMode = 13;
	memcpy(ack.DS2GS_CIS_CASH_INVEN_ACK.szAcc, pRecv->GS2DS_CIS_CASH_INVEN_REQ.szAcc, en_max_lil);
	ack.DS2GS_CIS_CASH_INVEN_ACK.szAcc[en_max_lil] = NULL;
	ack.DS2GS_CIS_CASH_INVEN_ACK.usTotalCnt = 0;
	ack.DS2GS_CIS_CASH_INVEN_ACK.ucRowCount = 0;
	BYTE*		pBuf = (BYTE*)ack.data;
	int			iOffset = (en_max_lil+1) + sizeof(USHORT) + sizeof(UCHAR);

	struct stResult_0 {
		int		iPkgGuid;	// DB's Guid
		int		iPkgIdx;	// Package Index
	};

	struct stResult_1 {
		int		iTotalItemCnt;	// 캐쉬인벤의 (모든 페이지를 포함한) 총 아이템 개수; 몇 페이지를 보여줄지 계산하기 위해
	};

	TCHAR	szQuery[256] = {0};
	int		iResTableCnt = 0;
	stResult_0	res_0;
	stResult_1	res_1;
	BOOL	bRet = FALSE;

	if( pRecv->GS2DS_CIS_CASH_INVEN_REQ.wPageNum >= 128 || pRecv->GS2DS_CIS_CASH_INVEN_REQ.wPageNum == 0 )
	{
		Logout("#ERR : Cash Inven Req : Page Num Error (%u ==> 1)", pRecv->GS2DS_CIS_CASH_INVEN_REQ.wPageNum );
		pRecv->GS2DS_CIS_CASH_INVEN_REQ.wPageNum = 1;
	}

	sprintf(szQuery, "exec cash_item_db.dbo.isp_get_cash_item_list %d, %d, %d, N'%s', %d",
	//sprintf(szQuery, "exec Item_mall_global.dbo.isp_get_cash_item_list %d, %d, %d, N'%s', %d",
		pRecv->GS2DS_CIS_CASH_INVEN_REQ.wNumPerPage,
		pRecv->GS2DS_CIS_CASH_INVEN_REQ.wPageNum,
		g_ForestNum,
		pRecv->GS2DS_CIS_CASH_INVEN_REQ.szAcc,
		pRecv->GS2DS_CIS_CASH_INVEN_REQ.bIsPresent ? 1 : 0);
	Logout("%s", szQuery);
#ifdef _CIS_20070326_LOG_
	PrintConsole("%s \n", szQuery);
#endif
	if(FALSE == pCIS->Exec(szQuery)) {
		Logout("#ERR Fail while pCIS->Exec() : %s", szQuery);
		pCIS->Clear();
		return;
	}

	while(TRUE) {
		if(0 < pCIS->GetResultColCnt()) {
			switch(iResTableCnt++) {
			case 0:
				{
					pCIS->BindCol(1, SQL_C_SLONG, &res_0.iPkgGuid, 0);
					pCIS->BindCol(2, SQL_C_SLONG, &res_0.iPkgIdx, 0);

					int		iFetchCnt = 0; // Fetch 된 데이터 중 앞부분의 불필요한 데이터를 버리기 위해
					while(pCIS->Fetch()) {
						if(iFetchCnt < (pRecv->GS2DS_CIS_CASH_INVEN_REQ.wPageNum - 1) * pRecv->GS2DS_CIS_CASH_INVEN_REQ.wNumPerPage) {
							iFetchCnt++;
							continue;
						}

#ifdef _CIS_20070326_LOG_
						PrintConsole("iItemCode=%d iItemIdx=%d \n",
							res_0.iPkgGuid,
							res_0.iPkgIdx);
#endif

						++ack.DS2GS_CIS_CASH_INVEN_ACK.ucRowCount;

						memcpy(pBuf + iOffset, &res_0.iPkgGuid, sizeof(int)); iOffset += sizeof(int);
						memcpy(pBuf + iOffset, &res_0.iPkgIdx, sizeof(int)); iOffset += sizeof(int);
					}
				}
				break;

			case 1:
				{
					pCIS->BindCol(1, SQL_C_SLONG, &res_1.iTotalItemCnt, 0);

					while(pCIS->Fetch()) {
#ifdef _CIS_20070326_LOG_
						PrintConsole("iTotalItemCnt=%d \n",
							res_1.iTotalItemCnt);
#endif

						ack.DS2GS_CIS_CASH_INVEN_ACK.usTotalCnt = res_1.iTotalItemCnt;
					}
				}
				break;

			default:
				Logout("#ERR Default at switch(iResTableCnt++)");
				PrintConsole("#ERR Default at switch(iResTableCnt++) \n");
				break;
			}
		}

		if(!pCIS->MoreResult()) {
			pCIS->Clear();
			break;
		}
	}

	ack.usLength = sizeof(u_short) + sizeof(u_char) + sizeof(WORD) + iOffset;
#ifdef _CIS_20070326_LOG_
	{
		char	log[1024] = {0};
		char	logTemp[8] = {0};
		BYTE*	p = (BYTE*)&ack;
		for(USHORT us = 0; us < ack.usLength; ++us) {
			sprintf(logTemp, "%02X ", p[us]);
			strcat(log, logTemp);
		}
		Logout("%s", log);
		PrintConsole("%s\n", log);
	}
#endif
	if(pDSComm != NULL) pDSComm->Send(ack.usLength, (char*)&ack);
}

void CIS_MoveCashItemReq(MSG_COMMON* pRecv, COdbc* pCIS, _h_DS_Com* pDSComm) {
	MSG_COMMON	ack;
	ack.ucMessage = CMSG_NO_COMMON_ACK;
	ack.wMode = 18;
	memcpy(ack.DS2GS_CIS_MOVE_ITEM_ACK.szAcc, pRecv->GS2DS_CIS_MOVE_ITEM_REQ.szAcc, en_max_lil);
	ack.DS2GS_CIS_MOVE_ITEM_ACK.szAcc[en_max_lil] = NULL;
	ack.DS2GS_CIS_MOVE_ITEM_ACK.ucPkgCnt = pRecv->GS2DS_CIS_MOVE_ITEM_REQ.ucPkgCnt;
	BYTE*		pBuf = (BYTE*)ack.data;
	int			iOffsetReq = (en_max_lil+1) + sizeof(UCHAR);
	int			iOffsetAck = (en_max_lil+1) + sizeof(UCHAR);

	struct stResult {
		int		iRes;	// 성공/실패 여부
		int		iPkgIdx;
	};

	TCHAR	szQuery[256] = {0};
	int		iResTableCnt = 0;
	stResult	res;
	BOOL	bRet = FALSE;
	int		iPkgGuid = 0;

	for(UCHAR uc = 0; uc < pRecv->GS2DS_CIS_MOVE_ITEM_REQ.ucPkgCnt; uc++) {
		memcpy(&iPkgGuid, (BYTE*)pRecv->data + iOffsetReq, sizeof(int)); iOffsetReq += sizeof(int);

		sprintf(szQuery, "exec cash_item_db.dbo.isp_item_moveto_mailbag %d",
		//sprintf(szQuery, "exec Item_mall_global.dbo.isp_item_moveto_mailbag %d",
			iPkgGuid);
		Logout("%s", szQuery);
#ifdef _CIS_20070326_LOG_
		PrintConsole("%s \n", szQuery);
#endif
		if(FALSE == pCIS->Exec(szQuery)) {
			Logout("#ERR Fail while pCIS->Exec() : %s", szQuery);
			pCIS->Clear();
			return;
		}

		iResTableCnt = 0;
		while(TRUE) {
			if(0 < pCIS->GetResultColCnt()) {
				switch(iResTableCnt++) {
				case 0:
					{
						pCIS->BindCol(1, SQL_C_SLONG, &res.iRes, 0);
						pCIS->BindCol(2, SQL_C_SLONG, &res.iPkgIdx, 0);

						while(pCIS->Fetch()) {
#ifdef _CIS_20070326_LOG_
							PrintConsole("iRes=%d iPkgIdx=%d \n", res.iRes, res.iPkgIdx);
#endif
							Logout("iRes=%d iPkgIdx=%d", res.iRes, res.iPkgIdx);

							bool b;
							if(res.iRes == TRUE) {
								b = true;

								memcpy(pBuf + iOffsetAck, &iPkgGuid, sizeof(int)); iOffsetAck += sizeof(int);
								memcpy(pBuf + iOffsetAck, &res.iPkgIdx, sizeof(int)); iOffsetAck += sizeof(int);
								memcpy(pBuf + iOffsetAck, &b, sizeof(bool)); iOffsetAck += sizeof(bool);
							}
							else {
								b = false;

								--ack.DS2GS_CIS_MOVE_ITEM_ACK.ucPkgCnt;
							}
						}
					}
					break;

				default:
					Logout("#ERR Default at switch(iResTableCnt++)");
					PrintConsole("#ERR Default at switch(iResTableCnt++) \n");
					break;
				}
			}

			if(!pCIS->MoreResult()) {
				pCIS->Clear();
				break;
			}
		}
	}

	ack.usLength = sizeof(u_short) + sizeof(u_char) + sizeof(WORD) + iOffsetAck;
#ifdef _CIS_20070326_LOG_
	{
		char	log[1024] = {0};
		char	logTemp[8] = {0};
		BYTE*	p = (BYTE*)&ack;
		for(USHORT us = 0; us < ack.usLength; ++us) {
			sprintf(logTemp, "%02X ", p[us]);
			strcat(log, logTemp);
		}
		Logout("%s", log);
		PrintConsole("%s\n", log);
	}
#endif
	if(pDSComm != NULL) pDSComm->Send(ack.usLength, (char*)&ack);
}

void CIS_PresentMsgReq(MSG_COMMON* pRecv, COdbc* pCIS, _h_DS_Com* pDSComm) {
	MSG_COMMON	ack;
	ack.ucMessage = CMSG_NO_COMMON_ACK;
	ack.wMode = 23;
	memcpy(ack.DS2GS_CIS_PRESENT_MSG_ACK.szTo, pRecv->GS2DS_CIS_PRESENT_MSG_REQ.szTo, en_max_lil);
	ack.DS2GS_CIS_PRESENT_MSG_ACK.szTo[en_max_lil] = NULL;

	struct stResult {
		char		szFrom[en_max_lil+1];	// 선물한 유저의 Account
		DBTIMESTAMP	time;					// 선물 받은 날짜(년/월/일/시/분/초); DBTIMESTAMP
		char		szMsg[PRESENT_MSG_SIZE+1];	// 선물 메시지
	};

	TCHAR	szQuery[256] = {0};
	int		iResTableCnt = 0;
	stResult	res;
	BOOL	bRet = FALSE;

	sprintf(szQuery, "exec cash_item_db.dbo.isp_get_item_detail %d",
	//sprintf(szQuery, "exec Item_mall_global.dbo.isp_get_item_detail %d",
		pRecv->GS2DS_CIS_PRESENT_MSG_REQ.iPkgGuid);
	Logout("%s", szQuery);
#ifdef _CIS_20070326_LOG_
	PrintConsole("%s \n", szQuery);
#endif
	if(FALSE == pCIS->Exec(szQuery)) {
		Logout("#ERR Fail while pCIS->Exec() : %s", szQuery);
		pCIS->Clear();
		return;
	}

	while(TRUE) {
		if(0 < pCIS->GetResultColCnt()) {
			switch(iResTableCnt++) {
			case 0:
				{
					pCIS->BindCol(1, SQL_C_CHAR, res.szFrom, en_max_lil+1);
					pCIS->BindCol(2, SQL_C_TYPE_TIMESTAMP, &res.time, 0);
					pCIS->BindCol(3, SQL_C_CHAR, res.szMsg, PRESENT_MSG_SIZE+1);

					while(pCIS->Fetch()) {
#ifdef _CIS_20070326_LOG_
						PrintConsole("szFrom=%s y/m/d=%d/%d/%d h/m/s=%d/%d/%d szMsg=%s \n",
							res.szFrom,
							res.time.year,
							res.time.month,
							res.time.day,
							res.time.hour,
							res.time.minute,
							res.time.second,
							res.szMsg);
#endif

						memcpy(ack.DS2GS_CIS_PRESENT_MSG_ACK.szFrom, res.szFrom, en_max_lil);
						ack.DS2GS_CIS_PRESENT_MSG_ACK.szFrom[en_max_lil] = NULL;
						ack.DS2GS_CIS_PRESENT_MSG_ACK.time.wYear = res.time.year;
						ack.DS2GS_CIS_PRESENT_MSG_ACK.time.wMonth = res.time.month;
						ack.DS2GS_CIS_PRESENT_MSG_ACK.time.wDay = res.time.day;
						ack.DS2GS_CIS_PRESENT_MSG_ACK.time.wHour = res.time.hour;
						ack.DS2GS_CIS_PRESENT_MSG_ACK.time.wMinute = res.time.minute;
						ack.DS2GS_CIS_PRESENT_MSG_ACK.time.wSecond = res.time.second;
						memcpy(ack.DS2GS_CIS_PRESENT_MSG_ACK.szMsg, res.szMsg, PRESENT_MSG_SIZE);
						ack.DS2GS_CIS_PRESENT_MSG_ACK.szMsg[PRESENT_MSG_SIZE] = NULL;
					}
				}
				break;

			default:
				Logout("#ERR Default at switch(iResTableCnt++)");
				PrintConsole("#ERR Default at switch(iResTableCnt++) \n");
				break;
			}
		}

		if(!pCIS->MoreResult()) {
			pCIS->Clear();
			break;
		}
	}

	ack.usLength = sizeof(u_short) + sizeof(u_char) + sizeof(WORD) + (en_max_lil+1) + (en_max_lil+1) + sizeof(SYSTEMTIME) + (PRESENT_MSG_SIZE+1);
#ifdef _CIS_20070326_LOG_
	{
		char	log[1024] = {0};
		char	logTemp[8] = {0};
		BYTE*	p = (BYTE*)&ack;
		for(USHORT us = 0; us < ack.usLength; ++us) {
			sprintf(logTemp, "%02X ", p[us]);
			strcat(log, logTemp);
		}
		Logout("%s", log);
		PrintConsole("%s\n", log);
	}
#endif
	if(pDSComm != NULL) pDSComm->Send(ack.usLength, (char*)&ack);
}

#endif // _CIS_20070326_