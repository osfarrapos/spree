#pragma  once

// Log 구조체 선언..

struct  _ServerState
{	
	TCHAR m_server_name[21];
	SHORT m_state;
};


//EVENT CODE
#define _LOG_ITEM_DNPC_DROP_	      (1)
#define _LOG_ITEM_PC_DROP_             15
#define _LOG_ITEM_PC_GRAP_             16  // 일단 퀘스트 아이템 받기에도 쓴다. 
#define _LOG_ITEM_PVP_TRADE_           17
#define _LOG_ITEM_BUY_FROM_NPC_        18  // 상점에서 물건을 산다. 
#define _LOG_ITEM_SELL_TO_NPC_         19  // 상점에서 물건을 판다. 
#define _LOG_ITEM_SELL_CLAN_GIVE_      20  // 문파기부 아이템 
#define _LOG_ITEM_CLAN_GIVE_MONEY_     21  // 문파 기부 돈 .
#define _LOG_ITEM_QUEST_GIVE_          22  // 퀘스트에서 아이템 줌. 
#define _LOG_ITEM_QUSET_REMOVE_        23  // 퀘스트에서 아이템 뺏음. 
#define _LOG_ITEM_MONEY_INVAILD_       24  // 돈이 로그아웃시에 액수가 허용 차이치를 넘어감으로 로그를 남긴다. 
#define _LOG_ITEM_BREAK_ITEM_          25  // 아이템이 깨지다. 의상 칼
#define _LOG_ITEM_SAVE_STORAGE_        26  // 창고에 넣다.
#define _LOG_ITEM_TAKEOUT_STORAGE_     27  // 창고에서 꺼내다.
#define _LOG_ITEM_SOCKET_INSERT_       28  // 소켓 끼워넣기.
#define _LOG_ITEM_CLAN_GIVE_KICKUSER_  29  // 문파 기여에서 불법적인 시도 혹은 잘못된 데이터로 인하여 유저를 킥함.
#define _LOG_ITEM_STORAGE_KICKUSER_    30  // 창고 개설시 불법적인 시도 혹은 잘못된 데이터로 인해 유저를 킥함.
#define _LOG_ITEM_PSSHOP_TRADE_        31  // 개인상점에서 물품 판매기록. 
#define _LOG_ITEM_SAVEMONEY_STORAGE    32  // 창고에 돈을 입금함. 
#define _LOG_ITEM_TAKEOUTMONEY_STORAGE 33  // 창고에서 돈을 출금함. 
#define _LOG_ITEM_BUYSTORAGE_          34  // 창고 계약 시에 들어간 돈.
#define _LOG_ITEM_GAMBLE_              35  // 겜블로그.
#define _LOG_ITEM_ELIXIR_              36  // 영약로그.

#ifdef ITEM_V13
#define _LOG_ITEM_BOXGAMBLE_           37  // 박스 겜블 로그.
#endif

#define _LOG_ITEM_ADD_FROM_GMS_SUCC		40	// GMS로부터 아이템 삽입
#define _LOG_ITEM_ADD_FROM_GMS_FAIL		41	// GMS로부터 아이템 삽입
#define _LOG_ITEM_REM_FROM_GMS			42	// GMS로 부터 아이템 삭제

#define _LOG_ITEM_DS_UPDATE_MONEY_		43	// DS 에서 돈을 업데이트한다.

#define _LOG_ITEM_ADD_FROM_EVENT_SUCC	45	// event로 부터 아이템 삽입 성공(미니스탑)
#define _LOG_ITEM_ADD_FROM_EVENT_FAIL	45	// event로 부터 아이템 삽입 실패(미니스탑)

#define _LOG_ITEM_INCHANT_RESULT_		50	// 제련 결과.
#define _LOG_ITEM_INCHANT_SUCCESS_		51	// 제련 성공 
#define _LOG_ITEM_INCHANT_FAIL_			52	// 제련 실패
#define _LOG_ITEM_INCHANT_REMOVE_		53	// 제련으로 인한 아이템 삭제.

#define _LOG_ITEM_RESOURCE_REMOVE		61
#define _LOG_ITEM_COLLECT_GET			62
#define _LOG_ITEM_COLLECT_REMOVE		63
#define _LOG_ITEM_RESOURCE_GET			64

#define _LOG_ITEM_KILLMONSTER			71	//몬스터를 죽였다옹(쓸 데가 없어서 ItemLog에 씀)

#define _LOG_ITEM_PK_RESULT_			72	// pk 결과를 쓴다.

#define _LOG_ITEM_NAME_CHANGE_			73	// 캐릭터 이름 변경 로그 : from -> to.

#define _LOG_ITEM_TAX_ADD_				80	// 거둬들인 세금/보유금의 양 : DS
#define _LOG_ITEM_TAX_TAKE_				81	// 가져간 보유 금액

struct _ItemLog
{
	unsigned __int64          item_uid ;
	SHORT			 code ;		//EVENT CODE
	SHORT			 zone ;
	int				 from_uid ;
	TCHAR			 from[en_charac_name_length+1] ;
	int				 to_uid ;
	TCHAR			 to[en_charac_name_length+1] ;
	UINT			 money  ;
	SHORT			 type1  ;
	SHORT			 type2  ;
	union 
	{		
		struct Def_Item
		{
			_sServer_InvenItem	Item;
			char				padding[9];
		} _def_Item;	
		//BYTE	moreInfo[ sizeof(_sServer_InvenItem) + 9 ] ;
		struct charac_data
		{
			short sRealStrength;
			short sRealZen;
			short sRealintelligence;
			short sRealConstitution;
			short sRealDexterity;
			short sElixirStrength;
			short sElixirZen;
			short sElixirintelligence;
			short sElixirConstitution;
			short sElixirDexterity;
			char  cPadding[10];
		} chData;
	}Item_or_Byte;
};



// EVENT CODE
#define _LOG_CHARAC_CREATE		1
#define _LOG_CHARAC_DELETE		2

#define _LOG_CHARAC_LOGIN_		11
#define _LOG_CHARAC_LOGOUT_		12
#define _LOG_CHARAC_ENTER_		13
#define _LOG_CHARAC_LEAVE_		14

#define _LOG_CHARAC_LEVEL_UP_		20		// 캐릭터 레벨 업.
#define _LOG_CHARAC_BINSA_			21		// 캐릭터가 빈사 상태로
#define _LOG_CHARAC_DEAD_			22		// 캐릭터 사망.
#define _LOG_CHARAC_EXP_RECORD_		23		// 경험치 저장.
#define _LOG_CHARAC_LEVEL_UP_FAST_	24		// 레벨업이 매우 빠를 때
#define _LOG_CHARAC_QUEST_          25		// 퀘스트를 종료했음
#define _LOG_CHARAC_QUESTEXP_		26		// 퀘스트에 의해 경험치를 얻음
#define _LOG_CHARAC_RESET_STAT_		27		// 스탯 초기화.로그
#define _LOG_CHARAC_CLASSCHANGE_	28		// 역할 변경 로그

#define _LOG_CHARAC_HONOR_			30		// 명성 변화/셋팅
#define _LOG_CHARAC_FAME_			31		// 악명 변화/셋팅

#define _LOG_CHARAC_SEQUENCE_ERROR_		40	// 패킷 시퀀스 번호 에러.

#define _LOG_SKILL_ADD_				50		// 스킬의 추가 로그
#define _LOG_SKILL_STEP_UP_			51		// 스킬의 상승 로그
#define _LOG_SKILL_DEL_NORMAL_		52		// 정상 삭제.
#define _LOG_SKILL_DEL_CLASS_		53		// 역할 클래스때문에 삭제됨.
#define _LOG_SKILL_DEL_NOREF_		54		// 스킬 테이블에 없어서 삭제됨.
#define _LOG_SKILL_NOTIFY_CLASS_	55		// 스킬 테이블에 없어서 삭제됨.

// 영웅단 관련
#define _LOG_OR_CREATE_				70		// 영웅단 창설
#define _LOG_OR_DESTROY				71		// 영웅단 해체
#define _LOG_OR_JOIN_				72		// 영웅단 가입
#define _LOG_OR_OUT_				73		// 영웅단 탈퇴
#define _LOG_OR_ENTRUST_			74		// 단장 위임

// 별호 관련
#define _LOG_NIC_CREATE_			80      // 닉네임 주기
#define _LOG_NICK_DELETE_REQ_		81		//	별호 삭제 요청
#define _LOG_NICK_DELETE_			82		//	별호 삭제


// 이벤트 관련
#define _LOG_EVENT_RESULT_			90		// 각종 이벤트의 결과.

#define _LOG_EVENT_BOSS_ROOM		100		// BOSS ROOM



struct _CharacLog {
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
	char	cAccount[en_max_lil +1] ;
#else
	char	account[13] ;
#endif
	int		charac_uid ;
	char	charac[en_charac_name_length+1] ;
	short	code ;
	short	zone ;
	short	level ;
	short	point ;
	char	ip[17] ;
	BYTE	moreInfo[15] ;
};



#define	_LOG_TYPE_LOGIN			0
struct _LoginLog {
	int			acc_uid ;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
	char		cAccount[en_max_lil +1] ;
	char		cCharac[en_charac_name_length+1] ;
#else
	char		acc[12] ;
	char		charac[en_charac_name_length] ;
#endif
	short		zone_no ;
	int			inner_level	;
	short		party ;
	short		m_class ;
	char		ip[15] ;
	char		pe_pay_code[2] ;	// Bill Method
	char		pc_pay_code[2] ;	// pc방 구분
	char		pc_uid[20] ;
	char		login_time[20] ;
	char		logout_time[20] ;
	int			play_time ;
	int			type ;
};