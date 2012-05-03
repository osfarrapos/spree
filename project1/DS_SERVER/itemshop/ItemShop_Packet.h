#pragma once

#ifdef ITEMSHOP_PATCH_200607

#pragma pack( push, enter_item_shop )
#pragma pack(1)

#ifndef ACCOUNT_BUF_SIZE
#define ACCOUNT_BUF_SIZE		(en_max_lil+1)
#endif

#define MSG_NO_ITEM_SHOP			131
#define CMSG_NO_ITEM_SHOP		MSG_NO_ITEM_SHOP

enum PKTMODE_ITEM_SHOP {
	PKTMODE_PERSONAL_CASH_INVEN_REQ,
	PKTMODE_PERSONAL_CASH_INVEN_ACK,
	PKTMODE_PRESENT_CASH_INVEN_REQ,
	PKTMODE_PRESENT_CASH_INVEN_ACK,

	PKTMODE_MOVE_PERSONAL_PACKAGE_REQ,
	PKTMODE_MOVE_PERSONAL_PACKAGE_ACK,
	PKTMODE_MOVE_PRESENT_PACKAGE_REQ,
	PKTMODE_MOVE_PRESENT_PACKAGE_ACK,

	PKTMODE_MOVE_PERSONAL_PACKAGE_NTF,
	PKTMODE_MOVE_PRESENT_PACKAGE_NTF
};

struct MSG_ITEM_SHOP_BASE {
	USHORT	Header;
	UCHAR		ucMsg;
	UCHAR		ucMode;		// enum PKTMODE_ITEM_SHOP
};

struct CMSG_ITEM_SHOP_BASE {
	USHORT	usLength;
	UCHAR		ucMsg;
	UCHAR		ucMode;		// enum PKTMODE_ITEM_SHOP
};

typedef struct _MSG_U2GS_ITEM_SHOP__CASH_INVEN_REQ : public MSG_ITEM_SHOP_BASE {
	WORD	wInvenSize;		// 현재는 최대 45칸
} MSG_U2GS_ITEM_SHOP__CASH_INVEN_REQ, MSG_U2GS_ITEM_SHOP__PERSONAL_CASH_INVEN_REQ, MSG_U2GS_ITEM_SHOP__PRESENT_CASH_INVEN_REQ;

typedef struct _MSG_GS2DS_ITEM_SHOP__CASH_INVEN_REQ : public CMSG_ITEM_SHOP_BASE {
	short		sServerIdx;		// 서버 인덱스
	WORD	wInvenSize;		// 현재는 최대 45칸
	TCHAR	pszAccount[ACCOUNT_BUF_SIZE];
} MSG_GS2DS_ITEM_SHOP__CASH_INVEN_REQ, MSG_GS2DS_ITEM_SHOP__PERSONAL_CASH_INVEN_REQ, MSG_GS2DS_ITEM_SHOP__PRESENT_CASH_INVEN_REQ;

typedef struct _MSG_DS2GS_ITEM_SHOP__CASH_INVEN_ACK : public CMSG_ITEM_SHOP_BASE {
	UCHAR	ucRowCount;		// 반환되는 RowCount
	// rep. as ucRowCount
	int			iOrderNo;			// 주문번호
	int			iProductNo;		// 상품번호
	int			iItemUid;			// 패키지 인덱스
	// end of rep.

	TCHAR	pszAccount[ACCOUNT_BUF_SIZE];
} MSG_DS2GS_ITEM_SHOP__CASH_INVEN_ACK, MSG_DS2GS_ITEM_SHOP__PERSONAL_CASH_INVEN_ACK, MSG_DS2GS_ITEM_SHOP__PRESENT_CASH_INVEN_ACK;

typedef struct _MSG_GS2U_ITEM_SHOP__CASH_INVEN_ACK : public MSG_ITEM_SHOP_BASE {
	UCHAR	ucRowCount;		// 반환되는 RowCount
	// rep. as ucRowCount
	int			iOrderNo;			// 주문번호
	int			iProductNo;		// 상품번호
	int			iItemUid;			// 패키지 인덱스
	// end of rep.
} MSG_GS2U_ITEM_SHOP__CASH_INVEN_ACK, MSG_GS2U_ITEM_SHOP__PERSONAL_CASH_INVEN_ACK, MSG_GS2U_ITEM_SHOP__PRESENT_CASH_INVEN_ACK;

//////////////////////////////////////////////////////////////////////////
typedef struct _MSG_U2GS_ITEM_SHOP__MOVE_PKG_REQ: public MSG_ITEM_SHOP_BASE {
	UCHAR ucPkgCnt;			// 패키지 개수
	// rep. as ucPkgCnt
	int			iOrderNo;			// 주문번호
	int			iProductNo;		// 상품번호
	int			iPkgIdx;			// 선택된 패키지의 인덱스
	// end of rep.
} MSG_U2GS_ITEM_SHOP__MOVE_PKG_REQ, MSG_U2GS_ITEM_SHOP__MOVE_PERSONAL_PKG_REQ, MSG_U2GS_ITEM_SHOP__MOVE_PRESENT_PKG_REQ;

typedef struct _MSG_GS2U_ITEM_SHOP__MOVE_PKG_ACK: public MSG_ITEM_SHOP_BASE {
	UCHAR	ucPkgCnt;		// 캐쉬인벤에서 패키지 개수
	// rep. as ucPkgCnt
	int		iOrderNo;			// 주문번호
	int		iProductNo;		// 상품번호
	int		iPkgIdx;			// 선택된 패키지의 인덱스

	UCHAR	ucItemCnt;
	// rep. as ucItemCnt
	char	cSlot;				// CharInven의 slot index
	char	cType;				// 아이템 분류
	short	sId;
	UCHAR	ucNum;			//  아이템 뭉탱이 개수
	// end of ucItemCnt
	// end of ucPkgCnt
} MSG_GS2U_ITEM_SHOP__MOVE_PKG_ACK, MSG_GS2U_ITEM_SHOP__MOVE_PERSONAL_PKG_ACK, MSG_GS2U_ITEM_SHOP__MOVE_PRESENT_PKG_ACK;

typedef struct _MSG_GS2DS_ITEM_SHOP__MOVE_PKG_NTF: public CMSG_ITEM_SHOP_BASE {
	int			iOrderNo;		// 카테고리 번호
	int			iProductNo;	// 상품번호
} MSG_GS2DS_ITEM_SHOP__MOVE_PKG_NTF, MSG_GS2DS_ITEM_SHOP__MOVE_PERSONAL_PKG_NTF, MSG_GS2DS_ITEM_SHOP__MOVE_PRESENT_PKG_NTF;

#pragma pack( pop, enter_item_shop )

class COdbc;

void ItemShop_PersonalCashInvenReq(MSG_GS2DS_ITEM_SHOP__PERSONAL_CASH_INVEN_REQ* pReq, COdbc* pDB_ItemShop, _h_DS_Com* pDSComm);
void ItemShop_PresentCashInvenReq(MSG_GS2DS_ITEM_SHOP__PRESENT_CASH_INVEN_REQ* pReq, COdbc* pDB_ItemShop, _h_DS_Com* pDSComm);
void ItemShop_MovePersonalPackageNtf(MSG_GS2DS_ITEM_SHOP__MOVE_PERSONAL_PKG_NTF* pNtf, COdbc* pDB_ItemShop, _h_DS_Com* pDSComm);
void ItemShop_MovePresentPackageNtf(MSG_GS2DS_ITEM_SHOP__MOVE_PRESENT_PKG_NTF* pNtf, COdbc* pDB_ItemShop, _h_DS_Com* pDSComm);

#endif // ITEMSHOP_PATCH_200607