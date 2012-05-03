#pragma once

struct _sHeader ;

//#################################################################################
//
//  Items, Trade, Use...
//  Main = 14, 15, 16, 17, 18
//  112 ~ 151
//
//#################################################################################
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define _V1_COUNT_INVEN_ITEM_			108		// 25(장착) + 75(인벤토리) + 8(임시)

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	#define _V1_COUNT_REAL_INVEN_			150		// 실제 인벤토리
#else
	#define _V1_COUNT_REAL_INVEN_			100		// 실제 인벤토리
#endif
#define _V1_INVEN_EXC_EQUIP_			75		// 장착을 제외한 인벤토리 개수.
#define _V1_INVEN_START_BACKPACK_		50		// 
#define _V1_TEMP_INVEN_COUNT_			8
#define _V1_TEMP_INVEN_START_			100
#define _V1_TEMP_INVEN_END_				108		// 108은 포함되지 않음.

#define _TOTAL_INVENTORY_				100		// 전체 개수 2004.10.19->hotblood insert

#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장
    #define _TOTAL_STORAGE_					60		// 창고 리스트 개수
#else
    #define _TOTAL_STORAGE_					40		// 창고 리스트 개수
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 인벤토리 정보.
// 14, 15 : 112 ~ 127

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 인벤토리 아이템 조작, 아이템  
// 14
//#define MSG_NO_STORAGE_INFO             113     // 창고 박스의 내용요청에 대한 서버의 응답.s -> c 
#define MSG_NO_INVEN_MOVE				114		// 인벤토리 내에서의 위치 이동:장착,탈착,배낭으로,인벤으로 등 모두 처리. 
#define MSG_NO_ITEM_GRAB_REQ			115		// 아이템 먹기
#define MSG_NO_ITEM_GRAB_SUCCESS		115		// 아이템 먹기 성공/실패 : 당사자에게만 간다. **
#define MSG_NO_MONEY_GRAB_SUCCESS		116		// 돈을 먹었다.

#define MSG_NO_INVEN_DROP				117		// 아이템 바닥에 놓기. ( 유저의 요청,응답 )
#define MSG_NO_ITEM_DROP				118		// 아이템 바닥에 놓기. ( 몬스터 )
#define MSG_NO_EQUIP_CHANGE				119		// 장착 변화

#define MSG_NO_MONEY_DROP				120		// 돈 내려놓기 ( 유저의 요청,응답 )
// 15
///////////////      아이템 사용      ////////////////////////////////
#define MSG_NO_INVEN_INFO				121		// 캐릭터의 인벤토리 정보 
#define MSG_NO_USE_ITEM_REQ				122		// 아이템 사용요청.
#define MSG_NO_USE_ITEM_RESULT			122		// 아이템 사용결과
#define MSG_NO_ELIXIR_USE_RESULT		231		// 영약 사용 결과
#define MSG_NO_ELIXIR_INTOX_TIME        113		// 중독시간 끝
#define MSG_NO_TEMP_INVEN_CTRL          123		// 템프 인벤에서 인벤으로 옮길때
#define MSG_NO_TEMP_INVEN_CTRL_RESULT	123		// 결과 패킷

// Item Socket 2004.11.4->hotblood insert
#define MSG_NO_ITEM_EQUIP_SOCKET		125		// 아이템소켓에 장식물을 넣는다.   
#define MSG_NO_ITEM_EQUIP_SOCKET_RESULT 125
//#define MSG_NO_ITEM_ON_LAND				125		// 바닥에 있는 아이템들의 정보를 알려준다.

#define MSG_NO_ITEM_REMOVE				126		// 바닥에 있는 아이템을 삭제하라.

// 2004.06.18->oneway48 insert
#define MSG_NO_NPC_ITEM_REPAIR_REQUEST	132		// 아이템 수리 요청 패킷
#define MSG_NO_NPC_ITEM_REPAIR_RESULT	132		// 아이템 수리 요청 패킷

#define MSG_NO_PERIOD_ITEM_BASE			143		// 기간제 아이템 관련 

// Item broadcasting
#define MSG_NO_WORLD_ITEM_REQUEST		144		// 아이템 브로드캐스트 요청 패킷
#define MSG_NO_WORLD_ITEM_INFO			144		// 주변 아이템의 상황을 보내주는 패킷

// 인벤토리 조작
#define MSG_NO_INVEN_CTRL				145		// 인벤터리 조작.
#define MSG_NO_INVEN_REQ				146		// 인벤토리 요청

#define MSG_NO_BLOODPOINT_CTRL			127		// 혈정 포인트 관련 패킷

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/* =====================================================================================
	캐릭터의 인벤토리 정보.

  #define MSG_NO_INVEN_INFO				// 캐릭터의 인벤토리 정보 
===================================================================================== */
/*
struct MSG_INVEN_INFO
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	unsigned int		uiMoney ;			// 돈
	_sUser_InvenItem	Item[_TOTAL_INVENTORY_] ;			// 
} ;
*/
// insert end
// 영약 
struct Elixir_Data
{
	struct _Data
	{
		short __Data[10]; 
		char  __Grade;	
	};
	_Data Data[5];	// 오의별 데이터. 
					// 0 근력 
					// 1 진기 
					// 2 지혜			
					// 3 건강 
					// 4 민첩 
					// 보조 영약 먹었을 경우의 테이터는 어떻게 할 것인가???
	
	short _sIntoxication; // 중독 수치.
	char  Temp[3];  // 예약공간.
};

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
struct MSG_INVEN_INFO
{
	_sHeader header;
	u_char ucMessage;
	u_char ucInvenMode;
};
/*
0 - Elixir
1 - Inventory : Pocket, Backpack, Money, Inven
2 - Cash Inventory : Cash backpack & Cash Inventory.
*/

enum EN_INVEN_INFO_MODE
{
	en_inven_info_mode_elixir=0,
	en_inven_info_mode_inventory=1
};

struct MSG_ELIXIR_DATA : public MSG_INVEN_INFO
{
	Elixir_Data         E_Data;
};
 
struct MSG_INVENTORY : public MSG_INVEN_INFO
{
	char cAmorPocketNum;	// 상의포켓 숫자
	char cPantsPocketNum;	// 하의포켓 숫자
	char cBackPackExtend;	// 배낭      
	unsigned int uiMoney;	// 돈

	u_char ucItemCount;	// 이어서 오게되는 행랑 정보의 개수.
	char cInvenPack[ sizeof(_sUser_Inven_v1_all) * _V1_COUNT_REAL_INVEN_ ];
};
#else
// 2004. 10.15
struct MSG_INVEN_INFO
{
	_sHeader			header ;
	u_char				ucMessage ;
	u_char              ucItemCount;
	char		        cAmorPocketNum;			// 상의포켓 숫자
	char			    cPantsPocketNum;		// 하의포켓 숫자	
	char                cBackPackExtend;         // 배낭      
	unsigned int		uiMoney ;				 // 돈
	Elixir_Data         E_Data  ;                // 영약 데이터
	
	char				cInvenPack[ sizeof(_sUser_Inven_v1_all) * _V1_COUNT_REAL_INVEN_ ] ;
};
#endif
/* =====================================================================================
유저의 창고 정보. 

  #define MSG_NO_STORAGE_INFO			// 유저의 창고.
===================================================================================== */
// 유저의 스토리지 목록 요청을 받으면 서버에서 전송하는 패킷. 
// 2004.05.20->oneway48 modify
struct MSG_STR_ITEM_INFO
{
	_sHeader			header ;
	u_char				ucMessage ;
	u_char              ucSavedItem;  // 총 저장된 아이템 갯수.
	u_char              ucTotalSlot;  // 총 저장 슬롯 갯수.
	u_char              ucLastItemIdx ;	// 만기후 창고의 마지막에 있는 아이템 인덱스 

	WORD                wYear   ;
	u_char              ucMonth ;
	u_char              ucDay   ;
	u_char              ucHour  ;
  
	unsigned int		uiMoney ;     // 돈
	char				cStorage[sizeof(_sUser_Inven_v1_all)*_TOTAL_STORAGE_] ;
};

// modify end

/* =====================================================================================
	인벤토리 내에서의 위치 이동:장착,탈착,배낭으로,인벤으로 등 모두 처리. 
	요청과 응답은 성공일 경우(정상일 경우) 동일하고, 실패(취소시킴)는 cTo == -1

    단, 무기의 선택일 경우( 단축키나 기타 방법으로 처리된다.) 
	cFrom과 cTo를 동일하게 설정한다. 즉. 12 번 슬롯의 무기를 선택하고 싶을 경우
	cFrom = 12, cTo = 12 로 보내고 응답을 받음. 

  #define MSG_NO_INVEN_MOVE						// 인벤토리 내에서의 위치 이동:장착,탈착,배낭으로,인벤으로 등 모두 처리. 
===================================================================================== */
struct MSG_INVEN_MOVE
{
	_sHeader			header ;
	u_char				ucMessage ;

	char				cAmorPocket;
	char				cPantsPocket;
	char                cBackPackExtend;// 백팩값
	char				cFlag;
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
    u_char				cFrom ;
	u_char				cTo ;
#else
	char				cFrom ;
	char				cTo ;
#endif
	u_char				ucSendCount;
} ;

/* =====================================================================================
	주울 때는 아래 페킷으로 서버에 요청하고 결과를 Grab_Success 패킷으로 받는다. 
	다른 유저에게는 Grab_Success 대신에 아이템 삭제 패킷(Item_Remove)이 간다. 

  #define MSG_NO_ITEM_GRAB_REQ					// 아이템 먹기
===================================================================================== */
struct MSG_ITEM_GRAB_REQ
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	float               fX; // 2004.06.03->oneway48 insert
	float               fZ;	

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
    u_char				cTo ;					// 인벤토리 슬롯 어디로 넣을 것인가.
#else
	char				cTo ;					// 인벤토리 슬롯 어디로 넣을 것인가.
#endif
	int					iItemWorldID ;			// 아이템의 월드 아이디
} ;


/* =====================================================================================

  #define MSG_NO_ITEM_GRAB_SUCCESS				// 아이템 먹기 성공/실패 : 당사자에게만 간다. 다른 유저에게는 
===================================================================================== */
struct MSG_ITEM_GRAB_SUCCESS
{
	_sHeader			header ;
	u_char				ucMessage ;
	
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
    u_char				cInvenSlot ;			// 인벤토리 인덱스. 255이면 실패, 254는 슬롯 없음. 
#else
	char				cInvenSlot ;			// 인벤토리 인덱스. -1이면 실패, -2는 슬롯 없음. 
#endif
	int					ItemWorldID ;			// 아이템의 월드 아이디
	_sUser_Inven_v1_all ItemInfo ;
	
	char				cCount ;				//
	_sitem_pair			result[_V1_INVEN_EXC_EQUIP_] ;			// 결과
} ;



/* =====================================================================================
	돈을 먹었다. 돈을 업데이트

  #define MSG_NO_MONEY_GRAB_SUCCESS			// 돈을 먹었다.
===================================================================================== */
struct MSG_MONEY_GRAB_SUCCESS
{
	_sHeader			header ;
	u_char				ucMessage ;

	int					iItemWorldID ;
	u_int				uiMoney ;
} ;

	
/* =====================================================================================
	아이템을 버리고자 할 때 버리려는 아이템의 인벤번호를 채워서 서버에 보낸다.
	
	요청과 결과가 동일하다. 
	성공은 동일한 패킷이고, 스태커블 아이템의 경우 원래 개수에서 uiCount 의 값을 뺀다. 
	실패는 cFrom = -1 이다. 
	이 패킷은 MSG_INVEN_MOVE 와 같이, 자신에게만 가고, 인벤조작에만 쓰인다. 
	성공하게 되면, ITEM_DROP 메세지가 간다. 
	
  #define MSG_NO_INVEN_DROP					// 아이템 바닥에 놓기. ( 몬스터 )
===================================================================================== */
struct MSG_INVEN_DROP
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char                cBackPackExtend;		// 백팩값.
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	u_char				cInvenIndex;		// 버리려는 아이템의 인벤토리 인덱스
#else
    char				cInvenIndex;		// 버리려는 아이템의 인벤토리 인덱스
#endif
	unsigned int		uiCount;			// 버리려는 개수 
} ;


/* =====================================================================================
	몬스터가 떨어뜨리는 아이템과 유저가 버리는 아이템이 바닥에 떨어질 때, 
	이 메시지가 주위에 알려진다. 아이템의 개수에 따라 _sItem_Drop 이 잘려져 가므로
	길이가 변한다. 
	
  #define MSG_NO_ITEM_DROP					// 아이템 바닥에 놓기. ( 몬스터 )
===================================================================================== */
struct MSG_ITEM_DROP 
{
	_sHeader		header ;
	u_char			ucMessage ;
	
	char			cNoItems ;					// 놓여지는 아이템의 개수. 나중에 압축할 꺼니까 자르지 않고 보낸다. 

	u_short			usUniqueID ;					// 경험치 최고 유저
	
	_sItem_Drop		items[14] ;					// 몬스터는 최대 8개의 아이템을 가질수 있으므로..
} ;


/* =====================================================================================
	장착 변경 패킷 : 유니크 아이디에 해당하는 캐릭터의 장착이 변경되었다. (자신이면 무시)
	무기가 변경되었다면, 
	Index = _EQUIP_POS_WEAPON_1_ 
	id = 무기 아이디, 
	로 가데 된다. 
	
	Index 는 다음중 하나의 값이다. 
	#define _EQUIP_POS_AMOR_		0		// 상의		: 선택 - 보임,   겜 - 보임.
	#define _EQUIP_POS_PANTS_		1		// 하의		: 선택 - 보임,   겜 - 보임.
	#define _EQUIP_POS_CAP_			2		// 모자		: 선택 - 보임,   겜 - 보임
	#define _EQUIP_POS_ADAE_		3		// 아대		: 선택 - 보임,   겜 - 보임
	#define _EQUIP_POS_SHOES_		4		// 신발		: 선택 - 보임,   겜 - 보임
	#define _EQUIP_POS_EARRING_		7		// 귀부분	: 선택 - 보임,   겜 - 보임.
	#define _EQUIP_POS_BACKPACK_	11		// 배낭     : 선택 - 보임,   겜 - 보임.
	#define _EQUIP_POS_WEAPON_1_	12		// 무기는 이값으로

	#define MSG_NO_EQUIP_CHANGE				
===================================================================================== */
struct MSG_EQUIP_CHANGE
{
	_sHeader		header ;
	u_char			ucMessage ;

	u_short			usUniqueID ;

	char			cIndex ;	
	char            cType; // 아이템 타입

	short			sID ;
	u_char			ucStrength;		// 무기의 경우 유효
};


//////////////////////////////////////////////////////////////////////////
// 돈 내려놓기
// MSG_NO_MONEY_DROP
struct MSG_MONEY_DROP
{
	_sHeader		header ;
	u_char			ucMessage ;		//

	bool			bSuccess ;		// 요청일때는 무시, 응답일때는 성공(true)실패(false)
	u_int			uiCount ;		// 내려놓은 양 : 성공일때 이 값을 자기 인벤토리에서 빼야한다.
} ;


//=================================================================================
// 2003/04/23 추가 
// #define MSG_NO_ITEM_ON_LAND				// 바닥에 있는 아이템들의 정보를 알려준다.	
struct MSG_ITEM_ON_LAND
{
	_sHeader		header ;
	u_char			ucMessage ;

	char			cNoItems ;					// 이 패킷은 잘라서 보낸다. 따라서.. 클라이언트 파싱 조심.

	_sItem_Drop		items[50] ;				// 바닥 아이템이 100개를 넘어서면 이어서 계속 보낸다.
} ;


//=================================================================================
// 바닥에서 id 에 해당하는 아이템을 삭제하라. 
// #define MSG_NO_ITEM_REMOVE				// 바닥에 있는 아이템을 삭제하라.
struct MSG_ITEM_REMOVE
{
	_sHeader		header ;
	u_char			ucMessage ;

	int				iItemWorldID ;				// 삭제해야 할 아이템의 아이디. 
} ;


//================================== HSS Added.. 20030721
//#define MSG_NO_USE_ITEM_REQ				101		// 아이템 사용요청.
struct MSG_USE_ITEM_REQ	
{
	_sHeader			header ;
	u_char				ucMessage ;
	u_char				ucInvenNumber;
	char                cIsLpsKey;   // LPS 키 인가 : 0 일 경우 일반 아이템 유징. 1일 경우 LPS를 위한 아이템 유징.

	char				cIsFail;	// 영약 복용시에 fail로 처리가 되어야하는지.... 1일경우 0 일경우 정상처리로.

	unsigned short      sDNPCID  ;   // DNPCID
};

//	#define MSG_NO_USE_ITEM_RESULT				102		// 아이템 사용결과 
struct MSG_USE_ITEM_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
    u_char				ucResult;				// 성공 or 실패 & 실패 이유. // 2004.06.09->oneway48 insert
												// -1 일 경우 넘어온 아이템이 이상한 경우.
												// -2 일 경우 쿨 다운 시간이 안맞는 것이다 .	
#else
	char				cResult;				// 성공 or 실패 & 실패 이유. // 2004.06.09->oneway48 insert
												// -1 일 경우 넘어온 아이템이 이상한 경우.
												// -2 일 경우 쿨 다운 시간이 안맞는 것이다 .	
#endif
	u_char				ucInvenIndex ;			// 소모한 아이템의 인벤번호 
	u_char				ucCount;				// 현재 남아있는 개수 0개면 해당 인벤토리 슬롯을 삭제 
	
												//	추가 : 리졸트에서 cResult 값이 - 일경우 
												//		에러코드 
												//		
												//		-1 보내온 인벤터리 인덱스가 장착 인덱스거나 백팩 공간보다 크다. 
												//		-2 보내온 인벤터리 인덱스의 아이템이 없다.
												//		-3 아이템의 갯수가 이상하다. 

};

//#define MSG_NO_ELIXIR_USE_RESULT		231		// 영약 사용 결과
struct MSG_ELIXIR_USE_RESULT
{
	_sHeader			header      ;			
	u_char				ucMessage   ;				
	Elixir_Data         E_Data      ;
};

//#define MSG_NO_ELIXIR_INTOX_TIME            113                         
struct MSG_ELIXIR_INTOX_TIME
{
	_sHeader            header      ;
	u_char              ucMessage   ;
	short               sElixirIntox; // 중              
};

// 2005.02.23->hotblood insert ------------------------------------------------------------------=
// 상의나 하의가 깨졌을 때  클라이언트에 인벤에 저장된 정보 전달하기 위해서 쓰는 구조체이다. 
#define  _TEMP_INVEN_MAX_COUNT_ 8

struct _sindex_pair_
{
	u_char        cAccessoryIdx;
	u_char        cInvenIdx;
};
 
struct _sAccessoryItem
{
	char				cSecond ;   // 중분류.
	short				sID     ;	// 아이템 소분류. (아이템 아이디)
};

#ifdef _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI

struct MSG_TEMP_INVEN 
{
	_sHeader			header;
	u_char				ucMessage;
	char				cTo;
};

struct MSG_TEMP_INVEN_MOVE : public MSG_TEMP_INVEN
{
	// cTo == 0
	_sindex_pair_		IndexPair[_TEMP_INVEN_MAX_COUNT_];	// 0일 경우 이것으로 ....
};

struct MSG_TEMP_INVEN_INFO : public MSG_TEMP_INVEN
{
	// cTo == 1
	_sUser_Inven_v1_all	TInven[_TEMP_INVEN_MAX_COUNT_];		//1일 경우 이것으로 ....
};

#else

struct MSG_TEMP_INVEN 
{
	_sHeader           header      ;
	u_char             ucMessage   ; 
	char               cTo         ; // 0 일 경우 인벤으로 들어간 것 .
					 // 1 일 경우 템프 인벤으로 들어간 것이다. 
	union 
	{
		_sindex_pair_      IndexPair[_TEMP_INVEN_MAX_COUNT_]; // 0일 경우 이것으로 ....
		_sAccessoryItem	   TInven[_TEMP_INVEN_MAX_COUNT_];   // 1일 경우 이것으로 ....
	};			
};

#endif

// #define MSG_NO_TEMP_INVEN_CTRL          123		// 템프 인벤에서 템프 인벤으로 옮길때
struct MSG_TEMP_INVEN_CTRL
{
	_sHeader           header      ;
	u_char             ucMessage   ;
	char               cTIdx       ; // 빼려고 하는 아이템의 Temp 인벤터리 인덱스.
	char               cWhere      ; // 0 이면 인벤으로 1이면 밖으로( 즉 없애는 것 )
};

// #define MSG_NO_TEMP_INVEN_CTRL_RESULT	123		// 결과 패킷
struct MSG_TEMP_INVEN_CTRL_RESULT 
{
	_sHeader           header      ;
	u_char             ucMessage   ;
	char               cTIdx       ; // 빼낸 아이템 Temp 인벤터리 인덱스. 
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	u_char               cInvenIdx   ; // 들어가게 되는 인벤의 인덱스. 결과 값도 여기서... 음수일 경우. 실패로....
#else
	char               cInvenIdx   ; // 들어가게 되는 인벤의 인덱스. 결과 값도 여기서... 음수일 경우. 실패로....
#endif
};
// ----------------------------------------------------------------------------------------------=

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 16 : 128 ~ 135
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef STORAGE_TEST
//#define MSG_NO_STR_RESERVE_SLOT        128  
#endif

#define MSG_NO_STR_BUY_SLOT			    128  
#define MSG_NO_STR_BUY_SLOT_RESULT      128      

#define MSG_NO_STR_BUY_SLOT_CANCEL        147
#define MSG_NO_STR_BUY_SLOT_CANCEL_RESULT 147
#define	MSG_NO_TEMP_INVEN				148		// 상.하의가 파괴됐을때 오는 패킷
#define MSG_NO_STR_BUY_SLOT_RELEASE     149

// 2004.06.02->oneway48 modify
#define MSG_NO_STR_ITEM_EJECTEX        150 
#define MSG_NO_STR_ITEM_EJECTEX_RESULT 150
// modify end

#define MSG_NO_PVP_TRADE_CANCEL        129      // 트레이드 캔슬.                                          
#define MSG_NO_NPC_TRADE_REQ		   130	    // NPC에게 사거나 팔때, (실제로 행동이 일어날 때)
#define MSG_NO_NPC_TRADE_RESULT		   MSG_NO_NPC_TRADE_REQ //130 NPC 거래 결과.

// 창고 관련 HHH
#define MSG_NO_STR_ITEM_REQ            133		// 창고 박스의 내용을 요청함. c -> s
#define MSG_NO_STORAGE_INFO		    MSG_NO_STR_ITEM_REQ // 창고 박스의 내용요청에 대한 서버의 응답.s -> c 


#define MSG_NO_STR_ITEM_SAVE           134		// 인벤에서 스토리지로 아이템을 저장함.
#define MSG_NO_STR_ITEM_SAVE_RESULT    134      // 인벤에서 스토리지로 아이템을 저장요청시에 서버의 응답.

#define MSG_NO_STR_ITEM_EJECT          135		// 스토리지에서 인벤으로 아이템을 저장함.
#define MSG_NO_STR_ITEM_EJECT_RESULT   135    
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
struct _sUser_InvenItem	;
struct _tradeitem ;
/*
#ifdef STORAGE_TEST // 스토리지 테스트 
struct MSG_STR_RESERVE_SLOT
{
_sHeader			header  ;
u_char				ucMessage ;
char				cReserveType;
};
#endif
*/
// 2004.05.20->oneway48 insert
struct MSG_STR_BUY_SLOT
{
	_sHeader			header  ;
	u_char				ucMessage ;
	u_char				ucReserveType;
	u_int               uiPayMoney;
};

// 2004.05.22->oneway48 modify
struct MSG_STR_BUY_SLOT_RESULT
{
	_sHeader            header;
	u_char              ucMessage; 
	u_char              ucTotalUsableSlot; // 쓸 수 있는 슬롯 총 갯수.
	short				sCheckYear   ;
	u_char				ucCheckMonth ;
	u_char				ucCheckDay   ;
	u_char				ucCheckHour  ;    
	u_int               uiMoney;		   // 남은돈 .
};
// modify end

struct MSG_STR_BUY_SLOT_CANCEL
{
	_sHeader            header;
	u_char              ucMessage;
	u_int               uiMoney;		   // 원래 있던 돈 전체.
};
//insert end

// 유저가 스토리지 목록을 요청할 때 사용한다. 
struct MSG_STR_ITEM_REQ
{
	_sHeader			header    ;
	u_char				ucMessage ;
}; //  더 필요한 것이 있는가?

typedef MSG_STR_ITEM_REQ  MSG_STR_BUY_SLOT_RELEASE;// 2004.06.01->oneway48 insert : 창고 사용 해제


// 인벤에서 스토리지로 넣는 것을 요청하는 패킷.
struct _SM_Storage_Inven;



struct MSG_STR_ITEM_SAVE
{
	_sHeader			header     ;
	u_char				ucMessage  ;
	char                cItemNum   ;                 // 저장 요청하는 아이템 숫자.   
	_SM_Storage_Inven   Storage[20];				 // 스토리지에 한번에 쓸 수 있는 숫자.
	unsigned int		uiMoney    ;		
};

typedef MSG_STR_ITEM_SAVE  MSG_STR_ITEM_SAVE_RESULT;

// 2004.06.02->oneway48 insert
struct  MSG_STR_ITEM_EJECTEX
{
	_sHeader			header     ;
	u_char				ucMessage  ;
	char                cItemNum   ;                  //    
	_SM_Storage_Inven   Storage[20];                  // 다시 돌아갈때는 유저인벤에 인덱스를 포함한다. 
	unsigned int		uiMoney    ;				  // USER 전체 돈
};

typedef MSG_STR_ITEM_EJECTEX  MSG_STR_ITEM_EJECTEX_RESULT;
// insert end

// 스토리지에서 아이템 인벤으로 넣는 것을 요청하는 패킷. 
struct MSG_STR_ITEM_EJECT
{
	_sHeader			header     ;
	u_char				ucMessage  ;
	char                cItemNum   ;                 // 저장 요청하는 아이템 숫자.   
	_SM_Storage_Inven   Storage[20];				 // 다시 돌아갈때는 유저인벤에 인덱스를 포함한다. 
	unsigned int		uiMoney    ;		
};

typedef MSG_STR_ITEM_EJECT MSG_STR_ITEM_EJECT_RESULT;

/* =====================================================================================
		 NPC에게 살 때 bBuyFromNPC = 1
		: sNPCID와 cItemNo는 NPC에 해당하는 정보이고, cItemNo과 cItemType, sItemID는 인증에 쓰인다.

		 NPC에게 팔 때 bBuyFromNPC = 0
		: 이때 cItemNo은 캐릭터의 인벤토리 인덱스이고, 인증으로 Type,ID 가 쓰이고, 
		uiNeedMoney 도 인증에 쓰인다. 
  
		 NPC에게 문파 기여할 때 bBuyFromNPC = 2
		기여할 돈과 아이템 등.
	
		 NPC에게 치료를 받을 때 bBuyFromNPC = 5 ( 이때는 ucCount와 uiNeedMoney 만 본다. )
		ucCount = 1 로 무조건 설정하고, uiNeedMoney 에는 필요한돈이 들어간다.
		uiNeedMoney 는 외상과 내상의 합이 될 것이다. 
		Trade_Result 로 간다. 

  #define MSG_NO_NPC_TRADE_REQ			82		// NPC에게 사거나 팔때, (실제로 행동이 일어날 때)
===================================================================================== */

enum EN_NPC_TRADE_MODE
{
	en_npc_trade_mode_sell = 0,					//npc에게 팔때
	en_npc_trade_mode_buy = 1,					//npc에게서 살때
	en_npc_trade_mode_contribute_moonpa = 2,	//npc에게 문파 기여
	en_npc_trade_mode_heal = 5,					//npc로부터 치료
	en_npc_trade_mode_gamble = 6,				//npc에게 흑점
	en_npc_trade_mode_move_village = 7,			//np에게 표국이동 요청
	en_npc_trade_mode_buy_contribute = 8,		//npc에게 문파기여도로 산다
};

struct MSG_NPC_TRADE_REQ
{
	_sHeader			header      ;
	u_char				ucMessage   ;
	char				cBuyFromNPC ;	// EN_NPC_TRADE_MODE

	u_short				usNPCID     ;	// NPC 아이디
	u_char				ucItemNo     ;	// NPC에게 살때는 NPC 가 가진 아이템 리스트의 순번	
										// NPC에게 팔때는 유저의 인벤토리 번호. 문파 기여도에서도 유저 인벤토리 번호.

	char				cItemType   ;	// 아이템
	short				sItemID     ;	// 아이템 
	u_char				ucCount     ;	// 개수

	unsigned int		uiNeedMoney ;	// NPC에게 살때는 필요한 돈, NPC에게 팔때는 보상가.
} ;

/* =====================================================================================
	NPC 거래의 결과. 
	거래의 경우에만, 인벤토리 슬롯의 지정을 서버에서 한다. 
	성공인 경우, 인벤토리 슬롯과 돈을 서버에서 보내준 값으로 그대로 업데이트한다.

  #define MSG_NO_NPC_TRADE_RESULT			83		// NPC 거래 결과.	
===================================================================================== */
struct MSG_NPC_TRADE_RESULT
{
	_sHeader			header      ;
	u_char				ucMessage   ;

	char                cResult;		// 0 : 사고 팔기
										// 1 : 문파 기여도
										// 2 : 문파 기여도로 사기 성공
										// 5 : 치료 성공
										// -1 : 치료 실패
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	u_char				cInvenIndex ;	// 실패인 경우 255, 성공일 때는 실제 인벤토리 인덱스. 225로 가면 문파기여도 결과, Result가 5,-1 이면 무시.
                                        // 253 이면, 거래할 수 없는 아이템.
                                        // 254 : 가격이 틀림.
                                        // 252 : 개수이상.
                                        // 245 : 문파기여도 모자름.
                                        // 244 : 문파기여도가 틀림. 
#else
	char				cInvenIndex ;	// -1 : 실패
										// 성공이면 인벤 인덱스
										// 127 : 문파 기여도 결과
										// -11 : 문파 기여도 모자람
										// -12 : 문파 기여도가 다름
#endif

	_sUser_Inven_v1_all	Item		;

	unsigned int		uiLeftMoney ;	// 거래결과로 남은 현재 돈. NPC에게서 문파기여도를 얻을 때.  문파 기여도 총 합.
} ;


// 아이템 수리용 임시 패킷. : 요청 패킷과 결과 패킷 모두 가변 길이.
//#define MSG_NO_NPC_ITEM_REPAIR_REQUEST		132		// 아이템 수리 요청 패킷
//#define MSG_NO_NPC_ITEM_REPAIR_RESULT			132		// 아이템 수리 요청 패킷
struct MSG_NPC_ITEM_REPAIR_REQUEST
{
	_sHeader			header      ;			// 길이 = 11 + cRepairCount 
	u_char				ucMessage   ;
	
	u_short				usNPCID ;				// 요청하고 있는 NPC 아이디.
	char                cMode;                  // mode가 0일 경우 아이템 가격 전송요청 . // mode가 1일 경우 아이템 수리 요청.

	unsigned int		uiNeedMoney ;			// 인증용 : 서버랑 맞추기 위해서.
	
	u_char				ucRepairCount ;			// 수리하려는 아이템의 개수
	
	u_char				ucInvenIndex[_V1_COUNT_REAL_INVEN_] ;			// 수리하려는 아이템의 슬롯 번호.
} ;

struct MSG_NPC_ITEM_REPAIR_RESULT
{
	_sHeader			header      ;			// 길이 = 8 + sizeof(__Repair_Result) * cRepairCount ;
	u_char				ucMessage   ;

	char                cMode       ;           // mode가 0일 경우 아이템 가격 전송.
	
	unsigned int		uiLeftMoney ;			// 남은 돈.

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	u_char				ucRepairCount ;			// 수리하려는 아이템의 개수
#else
	char				cRepairCount ;			// 수리하려는 아이템의 개수
#endif
	
	struct __Repair_Result
	{ 
		u_char				ucInvenIndex ;			// 실패인 경우 -1, 성공일 때는 실제 인벤토리 인덱스.
		u_short				usCurDurability ;
		u_short				usMaxDurability ;
	} _S_Repair_Result[_V1_COUNT_REAL_INVEN_] ;
} ;

/* =====================================================================================
//#define _EQUIP_POS_AMOR_		0		// 상의		: 선택 - 보임,   겜 - 보임.
//#define _EQUIP_POS_PANTS_		1		// 하의		: 선택 - 보임,   겜 - 보임.
//#define _EQUIP_POS_CAP_		2		// 모자		: 선택 - 보임,   겜 - 보임
//#define _EQUIP_POS_ADAE_		3		// 아대		: 선택 - 보임,   겜 - 보임
//#define _EQUIP_POS_SHOES_		4		// 신발		: 선택 - 보임,   겜 - 보임
//
//#define _EQUIP_POS_GAP_UPPER_	5		// 갑주 상  : 선택 - 안보임, 겜 - 안보임
//#define _EQUIP_POS_GAP_LOWER_	6		// 갑주 하  : 선택 - 안보임, 겜 - 안보임
//
//#define _EQUIP_POS_EARRING_	7		// 귀부분	: 선택 - 보임,   겜 - 보임.
//#define _EQUIP_POS_RING_A_	8		// 반지 1   : 선택 - 안보임, 겜 - 안보임.
//#define _EQUIP_POS_RING_B_	9		// 반지 1   : 선택 - 안보임, 겜 - 안보임.
//#define _EQUIP_POS_NECKLACE_	10		// 목부분   : 선택 - 안보임, 겜 - 안보임.
//#define _EQUIP_POS_BACKPACK_	11		// 배낭     : 선택 - 보임,   겜 - 보임.
//
//// ==> 선택된 무기는 인덱스로 갖고 있는다. 각각 0, 1, 2에 대응한다. 
//#define _EQUIP_POS_WEAPON_1_	12		// 무기1 : 선택 - 보임, 겜 - 보임
//#define _EQUIP_POS_WEAPON_2_	13		// 무기2 : 요건 의상이 아니다. 주의!
//#define _EQUIP_POS_WEAPON_3_	14		// 무기3 : 요건 의상이 아니다. 주의!
//#define _EQUIP_POS_MOUSE_		15		// 마우스 포인터에 붙은 아이템.
===================================================================================== */







//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 캐릭터간 아이템 거래 : PvP   17 : 136 ~ 143
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define MSG_NO_PVP_TRADE_REQ			136		// 피씨간 아이템 거래 요청/거절/취소... 등등.
#define MSG_NO_PVP_TRADE_START			137		// 두 캐릭터의 거래 시작 : 이거 받으면 거래창이 떠야 한다. 
#define MSG_NO_PVP_TRADE_ADD			138		// 거래 리스트에 아이템을 올려 놓는다.
#define MSG_NO_PVP_TRADE_DELETE			139		// 거래 리스트에서 아이템을 가져온다.
#define MSG_NO_PVP_TRADE_UPDATE_INFO	140		// 상대방이 올린 아이템들의 정보.
#define MSG_NO_PVP_TRADE_MONEY          141     // 거래 목록에서 돈의 증감만이 있을 경우. 
#define MSG_NO_PVP_TRADE_END            142     // 거래가 끝남을 알림. 에러포함.




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=================================================================================
#define SUB_MSG_TRADE_REQEST			0		// 거래 요청
#define SUB_MSG_TRADE_REJECT			1		// 거래 요청 거절 또는 요청 취소
#define SUB_MSG_TRADE_ALLOW				2		// 거래 요청 수락
#define SUB_MSG_TRADE_CONFIRM			3		// '확인' 버튼 클릭
#define SUB_MSG_TRADE_CANCEL			4		// '취소' 버튼 클릭 또는 상대방의 취소. 따라서 받으면 거래 초기화.

// 서버에서만 보내는 메시지.
//#define SUB_MSG_TRADE_COMPLETE			5		// 거래 성립. ( 서버가 알아서 보내는 패킷, 클라이언트는 보내지 못함 )
#define SUB_MSG_TRADE_BUSY				6		// 상대방이 거래중이다.
#define SUB_MSG_TRADE_UNKNOWN			7       // 상대방이 없거나 상대방의 타겟 아이디가 이상하다.
#define SUB_MSG_TRADE_SELFREJECT        8       // 상대가가 트레이드를 신청한 후 거절를 해버렸다.

// 아이템 거래 : PvP A->B에게 신청할 때.
// 거래 요청시		   A->Server		: cSub = REQUEST,iTargetID = B.
// 거래 요청패킷		: Server->B		: cSub = REQUEST,iTargetID = A.
// 거래 요청 허락 시 : B->Server		: cSub = ALLOW,	 iTargetID = A.
// 거래 요청 거절 :	   B->Server		: cSub = REJECT, iTargetID = A.  => A가 요청한 거래 거절.
//						  Server->A		: cSub = REJECT, iTargetID = B.  
// 거래 요청 취소 :    A->Server		: cSub = REJECT, iTargetID = B.  => B에게 요청한 거래 취소.
// 거래 중 취소 : (AorB)->Server		: cSub = CANCEL, iTargetID = (A or B) 취소를 누른 쪽.
// 따라서 거래중일때 이 패킷을 받으면(bOK가 true건 false이건) 무조건 거래 취소.
// 거래중일 때 : bOK = 가 true 인 것을 받으면, 상대방이 '확인' 버튼을 클릭한 것.
//=================================================================================
//#define MSG_NO_PVP_TRADE_REQ			96	// 피씨간 아이템 거래 요청
struct MSG_PVP_TRADE_REQ
{
	_sHeader			header    ;
	u_char				ucMessage ;
	char				cSub      ;			// 상황에 따라 의미가 다름.
	u_short 			iTargetID ;			// 거래를 신청하려는 대상의 아이디
} ;

struct MSG_PVP_TRADE_CANCEL
{
	_sHeader			header    ;
	u_char				ucMessage ;
	//char				 cSub      ;		// 상황에 따라 의미가 다름.
	unsigned int		uiMoney   ;		   // 더해지는 돈.
	u_short 			iTargetID ;			
	_sitem_pair         result[10];        // 결과의 저장.   
} ;



//=================================================================================
// 거래 허가와 동시에 거래 시작 : 요청한 유저와 대상 유저에게 보내진다.
//#define MSG_NO_PVP_TRADE_START		97		

struct MSG_PVP_TRADE_START
{
	_sHeader			header     ;
	u_char				ucMessage  ;
	u_short				iReqID     ;			// 신청자 아이디.
	u_short				iTargetID  ;			// 대상 아이디.
	WORD                TradeSlotNO;            // 트레이드 슬롯 넘버. 
} ;

struct MSG_PVP_TRADE_END 
{
	_sHeader            header   ;
	u_char				ucMessage;
	int                 Reason   ;               // reason = 1 정상 완료.
	_sitem_pair         User[10] ;	
	unsigned int        uiTotalMoney;
	u_short             iTargetID;											 													 
};
//====================================================================================================//
// 거래창에 아이템을 추가한다. : 추가시에는 추가되는 아이템 정보만 쓴다.							  //
// ***********  이 설정은 패킷의 길이가 너무 길기 때문에 나중에 꼭 변경하도록 하자. **************    //
//====================================================================================================//
/*struct _sitem_pair {
	char			slot ;			// 슬롯
	unsigned short	number ;		// 개수
};*/

//=================================================================================
// 거래창의 '보내기'를 누르면 선택된 아이템들의 정보를 이 패킷에 담아 보낸다.
// 이때, 클라이언트는 인벤에서 선택된 아이템들은 임시로 빼고, 추가할 아이템이 있으면 
// 다시 선택하여 '보내기'를 누르고, 역시 선택된 아이템들의 정보를 이 패킷에 담아 보낸다. 
// 돈의 추가도 이 패킷으로 모두 처리한다.
// 논스태커블 아이템의 경우, number는 무시하고 slot 번호만 유효한다. 
// 스태커블 아이템은 number 만큼을 추가하는 것으로 간주한다. slot = -1 전까지 유효. 
//=================================================================================
// 자세한 사항은 패킷설명.doc 참고.
//#define MSG_NO_PVP_TRADE_ADD			98


// 원본 .
/*struct MSG_PVP_TRADE_ADD
{
	_sHeader			header ;		// 2 bytes : size = 127 bytes... 
	char				ucMessage ;		// 3 bytes
	
	unsigned int		uiAddMoney ;	// 	추가되는 돈
	
	_sitem_pair			items[40] ;		// 여기에 쓰이는 인덱스는 인벤토리의 인덱스
} ;*/

// 유저가 트레이드 슬롯에 아이템을 무빙 할 때마다. Sending이 이뤄지게 된다. 
struct MSG_PVP_TRADE_ADD
{
	_sHeader		header       ;	// 2 bytes : size = 127 bytes... 
	u_char			ucMessage	 ;	// 3 bytes	
	WORD            SlotNo       ;  // Trade Slot Number.
	_sitem_pair		item         ;	// 한번에 하나씩만. 
	unsigned char   TWindowIndex ;  // 트레이딩 윈도우 인덱스.  
    char            cResetConf   ;  // 확인 버튼이 클릭된 상태에서... 다시 초기화 되었다. 
	char            cAddCount    ;  // 기존에 올라가있는 아이템 슬롯의 아이템 값을 올린다.  
} ;

// 수정된 트레이드 슬롯상에서의 아이템 제거 패킷. 아이템을 직접 넘길 필요가 없다..
struct MSG_PVP_TRADE_DELETE
{
	_sHeader		header       ;	// 2 bytes : size = 127 bytes... 
	u_char			ucMessage    ;	// 3 bytes 	
	u_char          Inven_Index  ;  // 트레이드 윈도우에서 지워져서 인벤으로 돌아갈 아이템의 인벤 인덱스. 
	WORD            SlotNo       ;  // Trade Slot Number
    u_short         ucSubjectID  ;	// 지우려고 하는 넘의 아이디.
	unsigned char	number       ;	
	unsigned char   TWindowIndex ;  // 트레이딩 윈도우인덱스.   
	char            cResetConf   ;  // 확인 버튼이 클릭된 상태에서... 다시 초기화 되었다. 1로 오면 버튼이 눌려있는 상
};
//#define MSG_NO_PVP_TRADE_DELETE		99
//typedef MSG_PVP_TRADE_ADD MSG_PVP_TRADE_DELETE ;

struct MSG_PVP_TRADE_MONEY
{
	_sHeader		header     ;	// 2 bytes : size = 127 bytes... 
	u_char			ucMessage  ;	// 3 bytes	
	WORD            SlotNo     ;    // Trade Slot Number.
	unsigned int	uiMoney	   ;	// 더해지는 돈.
    char            AddOrSub   ;    // 뺄것인가 더할 것인가. 1이면 더하기 0이면 빼기다. 
	char            cIsOppe    ;    // 1 : 상대 0 : 자신 
} ;
//=================================================================================
// 삽입할 아이템의 정보 : 상대방이 아이템을 올리면, 그 정보를 이 패킷으로 보낸다.
// 중간에서 서버는 인덱스를 조작하고, 아이템의 정보를 표시하기 위해 _sUser_InvenItem 으로 바꾼다. 
//
// 만약 A 가 거래창에 아이템을 올리면, A는 서버에 INSERT 패킷으로 보내고, 서버는 
// 받은 패킷으로 INSERT_INFO 패킷을 구성하여 B에게 보낸다. cNo -1까지.
//=================================================================================
//#define MSG_NO_PVP_TRADE_UPDATE_INFO		100
/*struct MSG_PVP_TRADE_UPDATE_INFO
{
	_sHeader			header ;
	char				ucMessage ;
	unsigned int		uiMoney ;
	_sUser_InvenItem	items[40] ;			// 
}; 원본. */


struct MSG_PVP_TRADE_UPDATE_INFO 
{                                
	_sHeader			header  ;
	u_char				ucMessage ;
	_sUser_Inven_v1_all item	;
	unsigned char       TradeWindowSlotNo; // 트레이드 윈도우 상의 슬롯
};


//////////////////////////////////////////////////////////////////////////
// 18 예약 ...
//////////////////////////////////////////////////////////////////////////

struct MSG_WORLD_ITEM
{
	_sHeader			header;
	u_char				ucMessage;
	u_char				ucPackNum; // 2004.05.28->oneway48 insert: 패킷 순서 넘버
	short				sNearItemNo;
	_sItem_Drop			DropItem[80]; 
};

struct MSG_WORLD_ITEM_REQUEST
{
	_sHeader			header;
	u_char				ucMessage;
};

enum _inven_ctrl_mode
{
	inven_ctrl_item_count=0,	// 아이템의 개수를 바꿔라.
		inven_ctrl_money_set,		// 돈의 양을 셋.
		inven_ctrl_item_dur_set,	// 내구력을 셋.
} ;

struct MSG_INVEN_CTRL_RESULT
{
	_sHeader        header ;
	u_char          ucMessage ;
	
	char            mode;			// _inven_ctrl_mode 에 따라.
	
	union 
	{
		//++++++ 아이템의 개수를 설정한다. ++++++//
		struct {
			char				cIdx ;			//
			u_char              ucCount ;	
			char				padding[2] ; 
		} _S_Item_Count  ;
		
		//++++++ 돈의 양을 설정한다. ++++++//
		struct {
			u_int			uiCount ;				// 돈 셋.
		} _S_Money_Set ;
		
		//++++++ 내구력을 셋팅한다. ++++++//
		struct {
			char				cIdx ;			
			u_short				usDurability ;		// 내구.
		} _S_Dur_Set ;
		
	};
} ;

// 2004.11.4->hotblood insert
struct MSG_ITEM_EQUIP_SOCKET
{
	_sHeader             header          ;
	u_char               ucMessage       ;
	u_char               ucToolItemIdx    ;
	u_char               ucObjItemIdx     ;
	char                 cAddonCounter   ; // 몇개나 삽입 시도중인가?
	u_char               ucAddonItemIdx[4]; // 최대 4개 이므로...
	char                 cMode           ; //  0일 경우 소켓에 삽입 1일 경우 박스 갬블. 
};

struct MSG_ITEM_EQUIP_SOCKET_RESULT
{
	_sHeader             header      ;
	u_char               ucMessage   ;
	_sUser_Inven_v1_all  CompleteItem;
	char                 cMode           ; //  0일 경우 소켓에 삽입 1일 경우 박스 갬블. 
	
#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
	_sitem_pair			leftItemInfo[4];
#endif
};

//#define MSG_NO_INVEN_REQ       146
struct MSG_INVEN_REQ
{
	_sHeader		header ;
	u_char			ucMessage ;
};

// 제련 ------------------------------------------------------------------------------------┓
#define MSG_NO_INCHANT_CTRL					124							// 제련 관련 패킷.

struct MSG_INCHANT_PACKET 
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucMode ;
} ;

// 제련은, 클라이언트에서 req 를 보내면, 
// 서버에서는 간단한 체크만 하고, start_inchant 를 보내준다.
// 
enum
{
	inchant_mode_str_req=0,		// 제련 요청
	inchant_mode_str_req_fail,	// 제련 실패했을 때, 이유와 함께. 어떤 실패인지. 
							// 0 - 부분실패, 1 - 완전실패.
	inchant_mode_str_start_inchant,	// 제련을 시작한다.( 클라인트 표시시작 )
	inchant_mode_str_complete,	// 제련 디스플레이가 완료됬다. 서버에 마지막 처리를 요청한다.
	inchant_mode_str_result,	// 제련 결과

	//Author : 양희왕  breif : 용린/용루 시스템	
	inchant_mode_protect_add_count_req=40,	// 용린 바르기 요청 c->s
	inchant_mode_protect_add_count_success,	// 용린 바르기 성공 s->c
	inchant_mode_protect_add_count_fail,	// 용린 바르기 실패 s->c
	inchant_mode_protect_inc_str_req=50,	// 용루 바르기 요청 c->s
	inchant_mode_protect_inc_str_success,	// 용루 바르기 성공 s->c
	inchant_mode_protect_inc_str_fail,	// 용루 바르기 실패 s->c
	
	inchant_mode_combine_item_req = 100,	// 무기 암상인 리뉴얼 : 조합 요청
	inchant_mode_combine_success,			// 성공
	inchant_mode_combine_fail,				// 실패
} ;

// 요청 패킷 : 대상 무기의 인벤토리내 슬롯 번호와, 
// 필요아이템이 있는 슬롯의 번호와, 필요 아이템의 개수를 채워서 서버에 요청한다.
struct MSG_INCHANT_STR_REQ : public MSG_INCHANT_PACKET
{
	u_char			cTargetWeaponSlot ;
	u_char			cNeedItemSlot ;
	u_char			ucNeedItemCount ;		// 인증용.
	u_int			uiNeedMoney ;			// 인증용
} ;

struct MSG_INCHANT_STR_REQ_FAIL : public MSG_INCHANT_PACKET
{
	u_char			ucFailReason ;
	// 1 not_item_type,		// 제련할 수 없는 아이템이다.
	// 2 more_need_item,		// 재료가 더 필요하다.
	// 3 more_need_money,		// 돈이 더 필요하다.
	// 4 not_match_needs,		// 필요 정보가 다르다.
	// 5 cannot_more_inchant,	// 더이상 제련할 수 없음
	// 6 wait,					// 잠시후에 다시.
}
 ;

struct MSG_INCHANT_STR_START_INCHANT : public MSG_INCHANT_PACKET 
{
	u_char			ucResult ;				
	// 0 : 성공 : 이 경우일 때만, 결과 요청 패킷을 보낸다..
	// 1 : 부분 실패 ( 실패 1 )
	// 2 : 완전 실패 ( 실패 2 )
	// 3 : 부분 실패 : 용린이 사용되어 무기는 파괴되지 않음.
    // 4 : 완전 실패 : 용린이 사용되어 무기는 파괴되지 않음.
} ;


struct MSG_INCHANT_STR_RESULT : public MSG_INCHANT_PACKET
{
	u_char			ucResult ;			// 위의 START_INCHANT 패킷의 결과와 동일.
	u_char			ucInchantGrade ;	// 대상 아이템의 Strength

	u_char			cSlot ;				// 실패시 혈정이 남을 공간 : 무기 공간이 될 것이다.
	u_char			ucLeftItemCount ;	// 남을 혈정의 개수.
	u_int			uiLeftMoney ;		// 처리 결과 남은 돈.

	// 이외의 아이템 처리 패킷은 가지 않는다. ( 아이템 삭제 패킷 등 )
	// 따라서, 남은 개수가 0이면, 슬롯을 비운다.

#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
	u_char	ucLeftProtectCount;	// 용린을 바른 적이 있다면, 실패(1,2경우) 시에, -1 하여 갈 것이다.
	u_char	ucIncMaxStr;	// 용루를 바른 적이 있다면, 원래값에서 -1 하여 갈 것이다.
#endif
} ;

#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
// inchant_mode_protect_add_count_req
struct MSG_INCHANT_PROTECT_ADD_COUNT_REQ : public MSG_INCHANT_PACKET 
{
	u_char	ucWeaponSlot;	// 용린을 바르려는 무기의 인벤토리 위치.( 반드시 인벤토리로 옮긴 후 하도록 검사 )
	u_char	ucProtectItemSlot;	// 용린의 위치 ==> 슬롯 하나씩만 사용한다.( 슬롯하나에 5, 다른 슬롯에 3개 있으면, 최대5개를 바를 수 있다. )
	u_char	ucUseCount;	// 용린을 몇 개 바를 것인가.
};

// inchant_mode_protect_add_count_success,	// 용린 바르기 성공 s->c
struct MSG_INCHANT_PROTECT_ADD_COUNT_SUCCESS : public MSG_INCHANT_PACKET 
{
	u_char	ucWeaponSlot;	// 용린을 바른 무기의 인벤토리 위치.
    u_char	ucReqProtectCount;	// 요청한 용린 회수.
    u_char	ucProtectCount;	// 최종 용린의 회수. <== 해당 아이템에 적용할 것.
    u_char	ucProtectItemSlot;	// 사용한 용린의 위치.
    u_char	ucLeftProtectItem;	// 사용하고 남은 용린의 개수( 0 이면 슬롯을 비울 것. )
};

// inchant_mode_protect_add_count_fail,	// 용린 바르기 실패 s->c
struct MSG_INCHANT_PROTECT_ADD_COUNT_FAIL : public MSG_INCHANT_PACKET 
{
	u_char	ucWeaponSlot;	// 시도한 무기의 인벤토리 위치
	u_char	ucFailReason;
	/* ucFailReason : 0 은 가지 않음.
				  1 : 용린이 부족하다.
				  2 : 무기에 요청한 만큼 바를 수 없다.(더이상 바를 수없다.)
				  3 : 무기가 아니다.
	*/
};

// inchant_mode_protect_inc_str_req=50,	// 용루 바르기 요청 c->s
struct MSG_INCHANT_PROTECT_INC_STR_REQ : public MSG_INCHANT_PACKET 
{
	u_char	ucWeaponSlot;	// 용루를 바르려는 무기의 인벤토리 위치.( 반드시 인벤토리로 옮긴 후 하도록 검사 )
	u_char	ucIncStrItemSlot;	// 용루의 위치
};

//          inchant_mode_protect_inc_str_success,                             // 용루 바르기 성공 s->c
struct MSG_INCHANT_PROTECT_INC_STR_SUCCESS : public MSG_INCHANT_PACKET 
{
	u_char	ucWeaponSlot;
    u_char	ucWeaponIncStr; // 무기의 증가된 최대강도.(표시 = 무기기본강도 + 이값 )
							// 무기에 설정한다. ucIncProtect 값에 설정하고, ucUsedProtect 값을 1로 할 것.

	u_char	ucIncStrItemSlot; // 용루의 위치
	u_char	ucLeftIncStrItemCount;     // 사용하고 남은 용루의 개수. 0이면 삭제.
};

//inchant_mode_protect_inc_str_fail,	// 용루 바르기 실패 s->c
struct MSG_INCHANT_PROTECT_INC_STR_FAIL : public MSG_INCHANT_PACKET 
{
	u_char	ucWeaponSlot;	// 요청했던 무기의 슬롯.
	u_char	ucFailReason;	// 실패 원인
                                                                                                                                                  
	/*
	1 - 용루가 없다. 또는 용루가 아니다.
	2 - 이미 용루를 바른적이 있다.
	3 - 무기 정보 이상.
	4 - 기타... 
	*/
};
#endif
// -----------------------------------------------------------------------------------------┛

//inchant_mode__combine_item_req=100,             // 무기 암상인 리뉴얼 : 조합 요청
struct MSG_COMBINE_ITEM_REQ : public MSG_INCHANT_PACKET
{
	u_char			ucWeaponSlot;		// 무기 
	u_char			ucAddItem1Slot;		// 조합 아이템 1
	u_char			ucAddItem2Slot;		// 조합 아이템 2

	u_int			uiNeedMoney;		// 필요한 돈
};

//inchant_mode__combine_success,               // 조합 성공.
struct MSG_COMBINE_SUCCESS : public MSG_INCHANT_PACKET
{
	_sUser_Inven_v1_all		WeaponItem;	// 결과 무기 아이템

	u_char			ucAddItem1Slot;		// 보조 아이템 1
	u_char			ucLeftAddItem1;		// 남은 보조 아이템 1 : 0이면 슬롯 삭제.

	u_char			ucAddItem2Slot;		// 보조 아이템 2
	u_char			ucLeftAddItem2;		// 남은 보조 아이템 2 : 0이면 슬롯 삭제.

	unsigned int	uiLeftMoney;		// 남은 돈
};

//inchant_mode_combine_fail
struct MSG_COMBINE_FAIL : public MSG_INCHANT_PACKET
{
	u_char			ucFailReason;
	
	/*
		0 : 조합할 돈의 양의 틀림
		1 : 조합할 무기 아이템을 등록하여 주십시오
		2 : 조합을 할 수 없는 무기 아이템 입니다
		3 : 제련된 아이템은 조합을 할 수 없습니다.
		4 : 조합에 필요한 재료가 아닙니다.
		5 : 소켓 조합 아이템을 먼저 등록하여 주십시오.
		6 : 이미 재료가 등록되어 있습니다.
		7 :	조합 시 %s 금전이 필요합니다. 조합을 진행하시겠습니까?
		8 : 조합을 취소하였습니다.
		9 : 재료가 부족합니다.
		10 : 금액이 부족합니다.
	*/
};



// 혈정 ------------------------------------------------------------------------------------┓
enum EN_BLOODPOINT_PACKET_MODE
{
	en_bppk_mode_setting_point = 0,		// 혈정 포인트 세팅
	en_bppk_mode_setting_monster_kill,	// 현재 몬스터 킬수
	en_bppk_mode_request_item,			// 혈정으로 바꾸기 요청
	en_bppk_mode_give_item,				// 혈정 아이템 주기
	en_bppk_mode_error					// 요청 실패시 에러 코드
};

struct MSG_BLOODPOINT_CTRL
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;
};

// mode : en_bppk_mode_setting_point
struct MSG_BLOODPOINT_SETTING_POINT : public MSG_BLOODPOINT_CTRL
{
	short			sPoint;
};

// mode : en_bppk_mode_setting_monster_kill
struct MSG_BLOODPOINT_SETTING_MONSTER_KILL : public MSG_BLOODPOINT_CTRL
{
	short			sMonsterKill;
};

// mode : en_bppk_mode_request_item
struct MSG_BLOODPOINT_REQEUST_ITEM : public MSG_BLOODPOINT_CTRL
{
	u_char			cInsertedSlot;		// 빈슬롯 또는 혈정이 있는 슬롯 : 혈정이 들어갈 자리.
} ;

// mode : en_bppk_mode_give_item
struct MSG_BLOODPOINT_GIVE_ITEM : public MSG_BLOODPOINT_CTRL
{
	u_char			cInsertedSlot;
	u_char			ucCount;
};

// mode : en_bppk_mode_error 
struct MSG_BLOODPOINT_ERROR : public MSG_BLOODPOINT_CTRL
{
	u_char			ucError;			// 0혈정포인트가 부족합니다. 1슬롯이상
};


// 기간제 아이템 관련 
//#define MSG_NO_PERIOD_ITEM_BASE                                 143
enum EN_PITEM_MODE
{            
	en_pitem_add=0,                                        
	en_pitem_set_period=1,
	en_pitem_remove=2,						// 기간에 다되어 사라짐.
	en_storage_pitem_remove = 3,			// 창고에 있던 아이템이 사라질 때 : 창고를 열때, 창고에서 찾을 때 가게 된다.
};

struct MSG_PERIOD_ITEM_BASE   //  : 기간제 아이템 - 기간설정/삽입/삭제
{
	_sHeader	header;
	u_char		ucMessage;
	u_char		ucMode;                          // EN_PITEM_MODE
};

struct MSG_PITEM_ADD : public MSG_PERIOD_ITEM_BASE
{
	u_char	ucPkgCnt;              // 캐쉬인벤에서 패키지 개수
	// rep. as ucPkgCnt
	int		iOrderNo;              // 주문번호
	int		iProductNo;            // 상품번호
	int		iPkgIdx;               // 선택된 패키지의 인덱스
	
	u_char	ucItemCnt;
	// rep. as ucItemCnt;
	_sUser_Inven_v1_all                      client_item;         // client_item ucCount이 반복된다.	
};

// 기존 아이템에 기간을 설정할 때.
struct MSG_PITEM_SET_PERIOD : public MSG_PERIOD_ITEM_BASE
{
	u_char						slot;                  // 슬롯 인덱스.
	__inven_uselimit_info_v1	period;                // 기간 정보.
};

// 기간제 아이템의 사용기간이 끝났을 때.
struct MSG_PITEM_REMOVE : public MSG_PERIOD_ITEM_BASE
{
	u_char  slot;
};

// 창고에 있던 아이템이 사라질 때 : 창고를 열때, 창고에서 찾을 때 가게 된다.
struct MSG_STORAGE_PITEM_REMOVE : public MSG_PERIOD_ITEM_BASE
{
	char	cType;		// 아이템 정보 - 벌써 사라졌음. 
	short	sID;
};

//Author : 양희왕 //breif : 소켓제거
#define MSG_NO_ITEM_SOCKET_GAMBLE	125	// 소켓 장식 또는 박스 겜블, 소켓 분리에 모두 쓰임.

// 아이템 제련 - 소켓에 아이템 삽입
enum 
{
	en_socket_gamble_mode_equip_socket = 0, // 0  무기/의상에 소켓 박기 : 기존의 EQUIP_SOCKET
	en_socket_gamble_mode_boxgamble,	// 1  박스 겜블 : 기존의 EQUIP_SOCKET
	en_socket_gamble_mode_equip_result,	// 2  소켓 장식의 결과. : 기존의 EQUIP_SOCKET_RESULT
	en_socket_gamble_mode_boxgamble_result,	// 3  박스 겜블의 결과. : 기존의 EQUIP_SOCKET_RESULT

	en_socket_gamble_mode_separate_socket = 10,	// 10  소켓 분리 요청
	en_socket_gamble_mode_separate_success,	// 11  소켓 분리 성공
	en_socket_gamble_mode_separate_fail,	// 12  소켓 분리 실패.
};

struct MSG_SOCKET_GAMBLE	// MSG_SM_
{
	_sHeader	header;
	u_char	ucMessage;
	u_char	ucMode;	// en_socket_gamble
};

// en_socket_gamble_mode_equip_socket
// en_socket_gamble_mode_boxgamble
struct MSG_SM_EQUIP_SOCKET : public MSG_SOCKET_GAMBLE
{
	u_char ucToolItemIdx;
	u_char ucObjItemIdx;
	char cAddonCounter;	// 몇개나 삽입 시도중인가?
	u_char ucAddonItemIdx[4];	// 최대 4개 이므로... 
};

 
// en_socket_gamble_mode_equip_result
// en_socket_gamble_mode_boxgamble_result
struct MSG_SM_EQUIP_RESULT : public MSG_SOCKET_GAMBLE
{
	_sUser_Inven_v1_all CompleteItem; // 완성되어진 아이템 하나.

#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
    _sitem_pair leftItemInfo[4];
#endif
};


// en_socket_gamble_mode_separate_socket=10,  // 10  소켓 분리 요청
struct MSG_SM_SEPARATE_SOCKET : public MSG_SOCKET_GAMBLE
{
	u_char ucTargetSlot;	// 무기/의상이 있는 인벤토리 인덱스
	u_char ucSocketSlot;	// 분리시킬 소켓의 인덱스( 0 ~ 3 )
	u_char ucToolSlot;	// 장식분리 도구가 있는 인벤토리 인덱스
	u_char ucEmptySlot;	// 분리된 소켓이 들어간 빈 인벤토리 인덱스.
};

// en_socket_gamble_mode_separate_success,                 // 11  소켓 분리 성공
struct MSG_SM_SEPARATE_SUCCESS : public MSG_SOCKET_GAMBLE 
{
	u_char ucTargetSlot;	// 무기/의상이 있던 인벤토리 인덱스.
	_sUser_Inven_v1_all TargetItem; // 소켓 분리 후의 무기/의상 정보.
	u_char ucResultSocketSlot;	// 분리된 소켓이 들어갈 인벤토리 인덱스.(요청에서, empty slot)
	_sUser_Inven_v1_all ResultSocket;	// 분리된 소켓의 정보.

	u_char ucToolSlot;	// 장식 분리도구의 인벤토리 인덱스
	u_char ucLeftToolCount;	// 장식 분리도구의 남은 개수.
};

// en_socket_gamble_mode_separate_fail,                          // 12  소켓 분리 실패
struct MSG_SM_SEPARATE_FAIL : public MSG_SOCKET_GAMBLE
{
	u_char ucFailReason;

	/* 기획서 메시지 그대로.

	1 : Message#1 - 장식물이 없거나 장착 공간이 없는 물품입니다.

	2 : Message#2 - 평화상태에서만 장식물을 분리할 수 있습니다.

	3 : Message#3 - 행낭에 여유 공간이 없어서 장식물 분리에 실패하였습니다.

	*/
}; 
