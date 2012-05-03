#pragma once

#define MAX_CHAR_NAME_LEN		12
#define CHAR_NAME_BUF_SIZE		(MAX_CHAR_NAME_LEN+1)

#define MSG_NO_ITEM_SHOP		131
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
	_sHeader	Header;
	UCHAR		ucMsg;
	UCHAR		ucMode;		// enum PKTMODE_ITEM_SHOP
};

struct CMSG_ITEM_SHOP_BASE {
	USHORT		usLength;
	UCHAR		ucMsg;
	UCHAR		ucMode;		// enum PKTMODE_ITEM_SHOP
};


typedef struct _MSG_U2GS_ITEM_SHOP__CASH_INVEN_REQ : public MSG_ITEM_SHOP_BASE {
	WORD	wInvenSize;		// 현재는 최대 45칸
} MSG_U2GS_ITEM_SHOP__PERSONAL_CASH_INVEN_REQ, MSG_U2GS_ITEM_SHOP__PRESENT_CASH_INVEN_REQ;



typedef struct _MSG_GS2U_ITEM_SHOP__CASH_INVEN_ACK : public MSG_ITEM_SHOP_BASE {
	UCHAR	ucRowCount;		// 반환되는 RowCount
	
	// rep. as ucRowCount
	int		iOrderNo;		// 주문번호
	int		iProductNo;		// 상품번호
	int		iPkgIdx;		// 패키지 인덱스
	// end of rep.
	
} MSG_GS2U_ITEM_SHOP__PERSONAL_CASH_INVEN_ACK, MSG_GS2U_ITEM_SHOP__PRESENT_CASH_INVEN_ACK;


typedef struct _MSG_U2GS_ITEM_SHOP__MOVE_ITEM_REQ: public MSG_ITEM_SHOP_BASE {
	UCHAR		ucPkgCnt;			// 패키지 개수
// rep. as ucPkgCnt
	int			iOrderNo;			// 주문번호
	int			iProductNo;			// 상품번호
	int			iPkgIdx;			// 선택된 패키지의 인덱스
// end of rep.
} MSG_U2GS_ITEM_SHOP__MOVE_PERSONAL_ITEM_REQ, MSG_U2GS_ITEM_SHOP__MOVE_PRESENT_ITEM_REQ;


typedef struct _MSG_GS2U_ITEM_SHOP__MOVE_ITEM_ACK: public MSG_ITEM_SHOP_BASE {

	UCHAR		ucPkgCnt;			// 패키지 개수
// rep. as ucPkgCnt
	int			iOrderNo;			// 주문번호
	int			iProductNo;			// 상품번호
	int			iPkgIdx;			// 선택된 패키지의 인덱스

	UCHAR	ucItemCnt;
	
	// rep. as ucItemCnt
	char	cSlot;	// CharInven의 slot index
	char	cType;	// 아이템 분류
	short	sId;
	UCHAR	ucNum;	//  아이템 뭉탱이 개수
	// end of rep.
// end of rep.

} MSG_GS2U_ITEM_SHOP__MOVE_PERSONAL_ITEM_ACK, MSG_GS2U_ITEM_SHOP__MOVE_PRESENT_ITEM_ACK;

