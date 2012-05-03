#pragma once

#define MSG_NO_PS_CTRL						151		// PS 관련 메세지는 모두 이것으로 처리.

enum {
	ps_ctrl_start_req=0,		//      151							// 개인 상점 오픈을 요청함 151 
	ps_ctrl_start_result,		// 1  _NO_PS_STARTREQ_RESULT           MSG_NO_PS_STARTREQ			//  오픈 요청 결과

	ps_ctrl_broad_req_cast,		// 2 #define MSG_NO_PS_BRD_CSTREQ                224							// 224  
	ps_ctrl_broad_cast,			// 3 #define MSG_NO_PS_BRD_CST                   MSG_NO_PS_BRD_CSTREQ		// 224

	ps_ctrl_stop_req,			// 4 #define MSG_NO_PS_STOPREQ                   225							// 225
	ps_ctrl_stop_result,		// 5 #define MSG_NO_PS_STOPREQ_RESULT            MSG_NO_PS_STOPREQ			// 225 

	ps_ctrl_shopitem_list_req,	// 6 #define MSG_NO_PS_SHOPITEM_LIST             226							// 226
	ps_ctrl_shopitem_list_result,	// 7#define MSG_NO_PS_SHOPITEM_LIST_RESULT      MSG_NO_PS_SHOPITEM_LIST		// 226

	ps_ctrl_shopitem_buy_req,	// 8 #define MSG_NO_PS_SHOPITEM_BUY              227							// 227
	ps_ctrl_shopitem_buy_result,// 9 #define MSG_NO_PS_SHOPITEM_BUY_RESULT       MSG_NO_PS_SHOPITEM_BUY		// 227

	ps_ctrl_stop_broad_cast,	// 10 #define MSG_NO_PS_STOP_BRDCST               228	// 228 관심인에게 상점이 닫힘을 알림.
	ps_ctrl_shopitem_sold,		// 11 #define MSG_NO_PS_SHOPITEM_SOLD_BRDCST      229	// 229 관심인에게 물건이 팔림을 알린다. 
	ps_ctrl_item_buy_to_owner	// 12 #define MSG_NO_PS_ITEM_BUY_TOOWNER		  230	// 230 관심인이 물건을 산 후의 주인장의 아이템과 현금 변화.
} ;


//MSG_NO_PS_CTRL 151
struct MSG_PS_CTRL
{
	_sHeader			header ;
	u_char				ucMessage ;

	u_char				ucCtrl ;		// 위의 ps_ctrl_ 상수들.
} ;

// HHH 재 디파인이다. 나중에 처리할 것. 
#define PSSHOP_MAX_ITEM_LIST            9           // 판매가능 최대 아이템.
#define PSSHOP_MAX_SHOP_NAME            37          // 최대 상점 이름 사이즈.
#define PSSHOP_MAX_WATCHER_NUM          10          // 최대 구경꾼 숫자.

// ps_ctrl_start_req 최대한 심플하게 만듭시다. 
struct MSG_PS_CTRL_START_REQ : public MSG_PS_CTRL
{
	char                cMode         ; // 모드에 따라서 
										// 0 이면 최초 생성 
										// 1 이면 임시로 닫았던 샾을 다시 open함.
	char                PSName[PSSHOP_MAX_SHOP_NAME]    ; // 방이름. 
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
    u_char                InvenIdx[PSSHOP_MAX_ITEM_LIST]   ; // 최대 9개 아이템.
#else
	char                InvenIdx[PSSHOP_MAX_ITEM_LIST]   ; // 최대 9개 아이템.
#endif
	u_char              ucItemCount[PSSHOP_MAX_ITEM_LIST]; // 아이템 각각의 올려놓은 갯수. 인벤의 갯수와 다를 수 있음을 유의하라. 
	u_int               ItemPrice[PSSHOP_MAX_ITEM_LIST]  ; // 최대 9개 아이템 가격. 
	char                ItemNum;        // 아이템 갯수. 
};

// ps_ctrl_start_result
struct MSG_PS_CTRL_START_RESULT  : public MSG_PS_CTRL
{
	char                cMode     ; // 모드에 따라서							닫기 결과일때 
									// 0 이면 shop 생성한 결과                       0이면 완전히 닫기 결과  
									// 1 이면 임시로 닫았던 샾을 다시 open한 결과    1이면 임시로 닫기 결과  
									// 2 이면 임시로 닫기 요청에 대한 결과 

	char                cReturn   ; // 리턴 코드. 정상이면 보내온 아이템 갯수와 같을 것이다. -1일 경우 아예 슬롯을 빌리지 못함. 
									// cMode가 2 이면 1이 정상 값 			

	u_short             usPSSlotNo; // 빌린 슬롯 넘버
};



// ps_ctrl_broad_req_cast
struct MSG_PS_CTRL_BROAD_REQ_CAST : public MSG_PS_CTRL
{
	u_short             usPSSLotNo;
	char                cMode     ; // STOPREQ로 쓰일때 쓰는 변수 .
									// 0이면 상점 닫기 
									// 1이면 아이템 바꾸기. 
									// 2이면 관심자가 창을 닫는다. 
};


// ps_ctrl_broad_cast
struct MSG_PS_CTRL_BROAD_CAST : public MSG_PS_CTRL
{
	u_short             usUID     ; // 유저 UID
	char                PSName[PSSHOP_MAX_SHOP_NAME];
};



// ps_ctrl_stop_req
// ps_ctrl_stop_result
typedef MSG_PS_CTRL_BROAD_REQ_CAST      MSG_PS_CTRL_STOP_REQ;        // 상점 접는(^^) 패킷은 브로드캐스트 리퀘스트 패킷을 쓴다. 
typedef MSG_PS_CTRL_START_RESULT		MSG_PS_CTRL_STOP_RESULT; // 상범 접기 결과 패킷은 스타트 패킷 결과 패킷을 쓴다. 



// ps_ctrl_stop_broad_cast
struct MSG_PS_CTRL_STOP_BROAD_CAST : public MSG_PS_CTRL
{
	u_short             usUID     ;  // 상점 주인의 UID
	char                cIsComplete; // 2 : 상점이 재오픈함을 브로드캐스팅.  
									 // 1 : 완전히 닫는 것임 
									 // 0 : 아이템 수정중
};

// ps_ctrl_shopitem_list_req
struct MSG_PS_CTRL_SHOPITEM_LIST_REQ : public MSG_PS_CTRL         // 아이템 리스트 요청할 때도 쓰고 관심자가 창을 닫을 때 관심자 리스트 삭제시에도 쓴다. 
{
	u_short             usUID     ;
	char                cMode     ;  // 0 이면 아이템 리스트 요청 
									 // 1 이면 관심자 리스트에서 자기를 삭제한다. 
};


struct PSItemList
{	
	char   cItemType   ;             // 올라간 아이템 타입.	
	short  sItem_Id    ;             //
	short  sSocket[4] ;
	u_char ucStrength ;

	u_short usCurDur   ;
	u_char ucSocketCount;            // 
	u_char ucItemCount ;             // 아이템 갯수.	
	u_int  uiPrice     ;             // 희망가격.

#ifdef _XDEF_SMELT_ITEM //Author : 양희왕 // 용루용린 상점표시
	u_short usUsedProtect	: 1;	// 용루를 사용한 적이 있는가.
	u_short	usCurProtect	: 5;	// 무기의 경우, 바른 용린의 수. 0 - 15
	u_short	usCurMaxProtect	: 5;	// 최대강도

	u_short	usReserved		: 5;	// 예약분.
#endif

};

// ps_ctrl_shopitem_list_result
struct MSG_PS_CTRL_SHOPITEM_LIST_RESULT : public MSG_PS_CTRL
{
	char                cReturn    ; // -1이면 관심자 리스트에 등록도 실패이며 리스트를 얻는 것도 실패하였다. 
									 // -2이면 자신이 트레이드 중이거나 트레이드 요청을 받은 상태이다. 역시 실패로 처리. 
	PSItemList          ItemList[PSSHOP_MAX_ITEM_LIST];
};


// ps_ctrl_shopitem_buy_req
struct MSG_PS_CTRL_SHOPITEM_BUY_REQ : public MSG_PS_CTRL
{
	char                cReturn      ;
	u_short             usUID        ; // 상점 주인 uid 
	char                cType        ; // 사려는 아이템의 타입 
	short               sID          ; // 사려는 아이템 아이디.              
	char                cSlotNoforBuy;
};

// ps_ctrl_item_buy_to_owner
struct MSG_PS_CTRL_ITEM_BUY_TO_OWNER : public MSG_PS_CTRL         // 유저가 아이템을 구매하였을 때 주인에게 날라가는 패킷. 
{	
	char                cReturn      ; // 결과값 성공은 1; 
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
    u_char              cIndex       ; // 인벤터리 내에서의 팔린 아이템 인덱스
#else
	char                cIndex       ; // 인벤터리 내에서의 팔린 아이템 인덱스.
#endif
	char                cPSWIndex    ; // PS 윈도우에서의 인덱스 . 
	_sUser_Inven_v1_all Item		 ;
	u_int               uiMoney      ; // 팔리고 난 후의 현금액수. 
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// ps_ctrl_shopitem_buy_result
struct MSG_PS_CTRL_SHOPITEM_BUY_RESULT : public MSG_PS_CTRL
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
    u_char              cReturn      ;  // 양수일 경우  인벤터리 들어가게 되는 인덱스 
#else
	char                cReturn      ;  // 양수일 경우  인벤터리 들어가게 되는 인덱스 
#endif
	// 음수 일 때는 에러코드 에러코드는 추후에 자세하게 설정하겠음. 
	_sUser_Inven_v1_all BuyItem		 ;
	u_int               uiMoney      ;  // 남은 돈.	확인용
};



// ps_ctrl_shopitem_sold
struct MSG_PS_CTRL_SHOPITEM_SOLD : public MSG_PS_CTRL
{
	char                cSoldSlot    ; 
	u_short             OwnerUID     ; // 가게 주인 UID
	u_short             UID          ; // 산 사람  UID 
	char                cType        ; // 팔린 아이템 타입. 확인용
	short               sID          ; // 팔린 아이템 아이디.  확인용
};
//PSHOP
