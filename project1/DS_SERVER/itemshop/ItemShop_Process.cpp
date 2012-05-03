#include "..\stdafx.h"

#ifdef ITEMSHOP_PATCH_200607

#include "..\MAPManager\MAPManager.h"
#include "..\Billing\BillingSvrLink.h"
#include "..\Ranking_Manager\Ranking_Manager.h"

#define INCLUDE_ODBC_HEADER
#include "../MyLibrary/gGlobal.h"
#include "../MyLibrary/gDefine.h"
#include "./ItemShop_Packet.h"

#define GAME_IDX__NINE_DRAGONS		25

void ItemShop_PersonalCashInvenReq(MSG_GS2DS_ITEM_SHOP__PERSONAL_CASH_INVEN_REQ* pRecvData, COdbc* pDB_ItemShop, _h_DS_Com* pDSComm) {
	MSG_DS2GS_ITEM_SHOP__PERSONAL_CASH_INVEN_ACK ack;
	char	pSendBuf[1024];
	int		iOffset = sizeof(CMSG_ITEM_SHOP_BASE), iOffsetBackUp = 0;
	memset(&ack, 0, sizeof(ack));

	iOffsetBackUp = iOffset; iOffset += sizeof(UCHAR); // for ack.ucRowCount

	// 직접 구매한 아이템 리스트 쿼리
	//////////////////////////////////////////////////////////////////////////
	struct stResult {
		int		iOrderNo;		// 주문번호
		int		iProductNo;	// 상품번호
		int		iItemUid;		// 샵 아이템 인덱스
	};

	TCHAR pszQuery[256] = {0};
	int iBindColCnt = 0;
	stResult	res;
	BOOL bRet = FALSE;

	/*
	_stprintf(pszQuery, _T("{CALL db_shop_gateway_ninedragons.dbo.ssp_order_get_list_extend(%d, %d, N'%s', %d, %d, %d)}"),
		GAME_IDX__NINE_DRAGONS,	// game idx
		pRecvData->sServerIdx,		// server idx
		pRecvData->pszAccount,		// user account
		0,											// present(선물하기 여부)
		1,											// page no.
		pRecvData->wInvenSize);		// rows per page
	Logout(_T("%s"), pszQuery);
	bRet = pDB_ItemShop->Prepare((SQLTCHAR*)pszQuery);
	*/
	_stprintf(pszQuery, _T("exec db_shop_gateway_ninedragons.dbo.ssp_order_get_list_extend %d, %d, N'%s', %d, %d, %d"),
		GAME_IDX__NINE_DRAGONS,	// game idx
		pRecvData->sServerIdx,		// server idx
		pRecvData->pszAccount,		// user account
		0,											// present(선물하기 여부)
		1,											// page no.
		pRecvData->wInvenSize);		// rows per page
	Logout(_T("%s"), pszQuery);
	if(FALSE == pDB_ItemShop->Exec(pszQuery)) {
		Logout(_T("#ERR %s(%d): pDB_ItemShop->Exec() 실패"), _T(__FILE__), __LINE__);
		pDB_ItemShop->Clear();
		return;
	}

	while(TRUE) {
		if(0 < pDB_ItemShop->GetResultColCnt()) {
			switch(iBindColCnt++) {
			case 0:
				pDB_ItemShop->BindCol(1, SQL_C_SLONG, &res.iOrderNo, 0);
				pDB_ItemShop->BindCol(3, SQL_C_SLONG, &res.iProductNo, 0);
				pDB_ItemShop->BindCol(5, SQL_C_SLONG, &res.iItemUid, 0);

				while(pDB_ItemShop->Fetch()) {
#ifdef _DEBUG
					_tprintf(_T("iOrderNo=%d iProductNo=%d iItemUid=%d\n"),
						res.iOrderNo,
						res.iProductNo,
						res.iItemUid);
#endif
					++ack.ucRowCount;

					memcpy(pSendBuf + iOffset, &res.iOrderNo, sizeof(int)); iOffset += sizeof(int);
					memcpy(pSendBuf + iOffset, &res.iProductNo, sizeof(int)); iOffset += sizeof(int);
					memcpy(pSendBuf + iOffset, &res.iItemUid, sizeof(int)); iOffset += sizeof(int);
				}
				break;
			}
		}

		if(!pDB_ItemShop->MoreResult()) {
			pDB_ItemShop->Clear();
			break;
		}
	}

	memcpy(pSendBuf + iOffsetBackUp, &ack.ucRowCount, sizeof(UCHAR));
	memcpy(pSendBuf + iOffset, pRecvData->pszAccount, ACCOUNT_BUF_SIZE); iOffset += ACCOUNT_BUF_SIZE;

	ack.usLength = iOffset;
	ack.ucMsg = CMSG_NO_ITEM_SHOP;
	ack.ucMode = PKTMODE_PERSONAL_CASH_INVEN_ACK;
	memcpy(pSendBuf, &ack, sizeof(CMSG_ITEM_SHOP_BASE));
	pDSComm->Send(iOffset, pSendBuf);
}

void ItemShop_PresentCashInvenReq(MSG_GS2DS_ITEM_SHOP__PRESENT_CASH_INVEN_REQ* pRecvData, COdbc* pDB_ItemShop, _h_DS_Com* pDSComm) {
	MSG_DS2GS_ITEM_SHOP__PRESENT_CASH_INVEN_ACK ack;
	char	pSendBuf[1024];
	int		iOffset = sizeof(CMSG_ITEM_SHOP_BASE), iOffsetBackUp = 0;
	memset(&ack, 0, sizeof(ack));

	iOffsetBackUp = iOffset; iOffset += sizeof(UCHAR); // for ack.ucRowCount

	// 선물 받은 아이템 리스트 쿼리
	//////////////////////////////////////////////////////////////////////////
	struct stResult {
		int		iOrderNo;		// 주문번호
		int		iProductNo;	// 상품번호
		int		iItemUid;		// 샵 아이템 인덱스
	};

	TCHAR pszQuery[256] = {0};
	int iBindColCnt = 0;
	stResult	res;
	BOOL bRet = FALSE;

	/*
	_stprintf(pszQuery, _T("{CALL db_shop_gateway_ninedragons.dbo.ssp_order_get_list_extend(%d, %d, N'%s', %d, %d, %d)}"),
		GAME_IDX__NINE_DRAGONS,	// game idx
		pRecvData->sServerIdx,		// server idx
		pRecvData->pszAccount,		// user account
		1,											// present(선물하기 여부)
		1,											// page no.
		pRecvData->wInvenSize);		// rows per page
	Logout(_T("%s"), pszQuery);
	bRet = pDB_ItemShop->Prepare((SQLTCHAR*)pszQuery);
	*/
	_stprintf(pszQuery, _T("exec db_shop_gateway_ninedragons.dbo.ssp_order_get_list_extend %d, %d, N'%s', %d, %d, %d"),
		GAME_IDX__NINE_DRAGONS,	// game idx
		pRecvData->sServerIdx,		// server idx
		pRecvData->pszAccount,		// user account
		1,											// present(선물하기 여부)
		1,											// page no.
		pRecvData->wInvenSize);		// rows per page
	Logout(_T("%s"), pszQuery);
	if(FALSE == pDB_ItemShop->Exec(pszQuery)) {
		Logout(_T("#ERR %s(%d): pDB_ItemShop->Exec() 실패"), _T(__FILE__), __LINE__);
		pDB_ItemShop->Clear();
		return;
	}

	while(TRUE) {
		if(0 < pDB_ItemShop->GetResultColCnt()) {
			switch(iBindColCnt++) {
			case 0:
				pDB_ItemShop->BindCol(1, SQL_C_SLONG, &res.iOrderNo, 0);
				pDB_ItemShop->BindCol(3, SQL_C_SLONG, &res.iProductNo, 0);
				pDB_ItemShop->BindCol(5, SQL_C_SLONG, &res.iItemUid, 0);

				while(pDB_ItemShop->Fetch()) {
#ifdef _DEBUG
					_tprintf(_T("iOrderNo=%d iProductNo=%d iItemUid=%d\n"),
						res.iOrderNo,
						res.iProductNo,
						res.iItemUid);
#endif
					++ack.ucRowCount;

					memcpy(pSendBuf + iOffset, &res.iOrderNo, sizeof(int)); iOffset += sizeof(int);
					memcpy(pSendBuf + iOffset, &res.iProductNo, sizeof(int)); iOffset += sizeof(int);
					memcpy(pSendBuf + iOffset, &res.iItemUid, sizeof(int)); iOffset += sizeof(int);
				}
				break;
			}
		}

		if(!pDB_ItemShop->MoreResult()) {
			pDB_ItemShop->Clear();
			break;
		}
	}

	memcpy(pSendBuf + iOffsetBackUp, &ack.ucRowCount, sizeof(UCHAR));
	memcpy(pSendBuf + iOffset, pRecvData->pszAccount, ACCOUNT_BUF_SIZE); iOffset += ACCOUNT_BUF_SIZE;

	ack.usLength = iOffset;
	ack.ucMsg = CMSG_NO_ITEM_SHOP;
	ack.ucMode = PKTMODE_PRESENT_CASH_INVEN_ACK;
	memcpy(pSendBuf, &ack, sizeof(CMSG_ITEM_SHOP_BASE));
	pDSComm->Send(iOffset, pSendBuf);
}

void ItemShop_MovePersonalPackageNtf(MSG_GS2DS_ITEM_SHOP__MOVE_PERSONAL_PKG_NTF* pNtf, COdbc* pDB_ItemShop, _h_DS_Com* pDSComm) {
	// 캐쉬 인벤의 아이템 삭제 요청
	//////////////////////////////////////////////////////////////////////////
	TCHAR pszQuery[256] = {0};
	BOOL bRet = FALSE;

	/*
	_stprintf(pszQuery, _T("{CALL db_shop_gateway_ninedragons.dbo.ssp_product_content_receive(%d, %d, %d, N'%s')}"),
		pNtf->iOrderNo,
		pNtf->iProductNo,
		1,
		_T(""));
	Logout(_T("%s"), pszQuery);
	bRet = pDB_ItemShop->Prepare((SQLTCHAR*)pszQuery);
	*/
	_stprintf(pszQuery, _T("exec db_shop_gateway_ninedragons.dbo.ssp_product_content_receive %d, %d, %d, N'%s'"),
		pNtf->iOrderNo,
		pNtf->iProductNo,
		1,
		_T("general reception"));
	Logout(_T("%s"), pszQuery);
	if(FALSE == pDB_ItemShop->Exec(pszQuery)) {
		Logout(_T("#ERR %s(%d): pDB_ItemShop->Exec() 실패"), _T(__FILE__), __LINE__);
		pDB_ItemShop->Clear();
		return;
	}

	while(TRUE) {
		if(!pDB_ItemShop->MoreResult()) {
			pDB_ItemShop->Clear();
			break;
		}
	}
}

void ItemShop_MovePresentPackageNtf(MSG_GS2DS_ITEM_SHOP__MOVE_PRESENT_PKG_NTF* pNtf, COdbc* pDB_ItemShop, _h_DS_Com* pDSComm) {
	// 캐쉬 인벤의 아이템 삭제 요청
	//////////////////////////////////////////////////////////////////////////
	TCHAR pszQuery[256] = {0};
	BOOL bRet = FALSE;

	/*
	_stprintf(pszQuery, _T("{CALL db_shop_gateway_ninedragons.dbo.ssp_product_content_receive(%d, %d, %d, N'%s')}"),
		pNtf->iOrderNo,
		pNtf->iProductNo,
		1,
		_T(""));
	Logout(_T("%s"), pszQuery);
	bRet = pDB_ItemShop->Prepare((SQLTCHAR*)pszQuery);
	*/
	_stprintf(pszQuery, _T("exec db_shop_gateway_ninedragons.dbo.ssp_product_content_receive %d, %d, %d, N'%s'"),
		pNtf->iOrderNo,
		pNtf->iProductNo,
		1,
		_T("general reception"));
	Logout(_T("%s"), pszQuery);
	if(FALSE == pDB_ItemShop->Exec(pszQuery)) {
		Logout(_T("#ERR %s(%d): pDB_ItemShop->Exec() 실패"), _T(__FILE__), __LINE__);
		pDB_ItemShop->Clear();
		return;
	}

	while(TRUE) {
		if(!pDB_ItemShop->MoreResult()) {
			pDB_ItemShop->Clear();
			break;
		}
	}
}

#endif // ITEMSHOP_PATCH_200607