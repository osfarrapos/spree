#pragma once

// Quest
// Main = 9, 10, 11
// 72 - 95

struct _sHeader;

//=================================================================================
// QUEST 관련 9 : 72~79
// 요청/통보 패킷
#define MSG_NO_QUEST_DATA					72		// 캐릭터의 퀘스트 데이타
#define MSG_NO_QUEST_SCON_REQUEST			73		// 트리거 성공조건 인증 요청 패킷

#define MSG_NO_QUEST_COMPLETE_DATA			74		// 성공한 퀘스트 실패한 퀘스트 리스트 요청 결과 패킷 
#define MSG_NO_QUEST_COMPLETEDATA_REQ   MSG_NO_QUEST_COMPLETE_DATA // 성공한 퀘스트 실패한 퀘스트 리스트 요청 패킷 
#define MSG_NO_QUEST_USER_CANCEL			75		// 유저의 취소 통보 패킷 : 취소 조건에 걸릴 경우 클라이언트가 보낼 수 있고, 
												// 유저의 입력을 통해서 보낼 수 있다. 
// 결과 패킷
#define MSG_NO_QUEST_SCON_RESULT			76		// 트리거 성공조건 인증 결과
#define MSG_NO_QUEST_FCON_RESULT			77		// 트리거 실패조건 인증 결과
#define MSG_NO_QUEST_SET_RESULT				78		// 퀘스트 결과 설정
#define MSG_NO_QUEST_SET_SELECTION_NO		79      // 선택지 번호 설정
#define MSG_NO_QUESTITEM_RECEIVE			80		// 퀘스트 아이템 S->C
#define MSG_NO_QUESTITEM_REMOVE				81		// 퀘스트 아이템 삭제
#define MSG_NO_LEARN_SKILL					82		// 무공 습득
#define MSG_NO_FORGET_SKILL					83		// 무공 해제
#define MSG_NO_UPDATE_STATUS				84		// 캐릭 스탯 변경
#define MSG_NO_FORCED_MOVE					85		// 캐릭 강제 이동
#define MSG_NO_JOIN_GROUP					86		// 문파 가입
#define MSG_NO_QUEST_SETCOUNT				87		// 카운터 세팅요청.
#define MSG_NO_QUEST_SETCOUNT_RESULT   MSG_NO_QUEST_SETCOUNT    // 세팅중 에러발생시.....
#define MSG_NO_QUEST_REDISTRIBUTION			88      // 오기 재 설정.

#define MSG_NO_QUEST_VALIDCHECK				89      // 퀘스트 보안 코드 패킷 :  퀘스트가 유효한지 검사를 한다.
#define MSG_NO_QUEST_VALIDCHECK_RESULT MSG_NO_QUEST_VALIDCHECK  	

#define MSG_NO_QUEST_VALIDCHECK_INGAME		91
#define MSG_NO_QUEST_VALIDCHECK_INGAME_RESULT MSG_NO_QUEST_VALIDCHECK_INGAME 

#define MSG_NO_QUESTNODEBACK_ITEM_RESULT	92 
#define MSG_NO_MONSTER_KILL					93		// 몬스터 죽은 체크 

#define MSG_NO_QUEST_PARTY_SETCOUNT_RESULT	94		// 파티 퀘스트에서 카운트 결과
#define MSG_NO_QUESTRANK_REQ				95		// 퀘스트 랭크	
#define MSG_NO_QUESTRANK_RES			MSG_NO_QUESTRANK_REQ

#define _QUEST_COMPLETE_SUCCESS_			0x7f		// 퀘스트 성공으로 완수
#define _QUEST_COMPLETE_FAIL_				0x7e		// 퀘스트 실패로 완수
#define _QUEST_CANCEL_FAIL_					0x7d        // 퀘스트 취소 -> 리셋 불가. 
#define _QUEST_CANCEL_RESET_				0x00        // 퀘스트 취소 -> 리셋가능. 생각 해볼것.

// 퀘스트 인증 요청
struct MSG_QUEST_AUTH_REQUEST
{
	_sHeader			header     ;
	u_char				ucMessage  ;
	char                cNodeID    ;                // 노드 넘버로.
	u_short             usQuestID  ;			    // 퀘스트 아이디.
	u_short				usUniqueID ;			    // USER의 SERVER내에서의 UNIQUE_ID : 필요한가?

#ifdef _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI
	u_char				ucDungeon;					// 1-for dungeon, 0-default
	u_char				ucReserve;
	short				bBypassTorF;				// 바이패스되는 컨디션을 True혹은 False로 설정한다. 
#else
	BOOL                bBypassTorF;				// 바이패스되는 컨디션을 True혹은 False로 설정한다. 
#endif
};

// 퀘스트 취소 통보 패킷
typedef MSG_QUEST_AUTH_REQUEST MSG_QUEST_USER_CANCEL;

struct _sCharacQuestState;
struct _sQuestItem;
struct _sRunningQuest;
struct _sRunningQuestPacketData;

#define _QUEST_MAX_NUMBER_  300
#define _MAX_CURQUEST_NUM_  6

//=================================================================================
// 캐릭터의 퀘스트 정보 : MSG_NO_QUEST_DATA				50		// 캐릭터의 퀘스트 데이타
// 캐릭터 선택시에 인벤토리 다음 패킷으로 전송함. 
// 이 패킷을 통해 클라이언트는 퀘스트 정보를 업데이트 한다.
struct MSG_QUEST_DATA 
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	_CHARAC_COLLECTION_DATA	CollectionData;		//명품수집 데이터
	
#ifdef _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI
	_CHARAC_COLLECTION_DATA	EventCollectionData;	// 명품수집 데이터 확장
#endif
	
	char                cCountRQ;									// 현 런닝 퀘스트의 갯수. 
	_sCharacMainQuestState MainQuestData;							// 제룡행 저장 공간.	
	_sRunningQuestPacketData RunningQuestData[_MAX_CURQUEST_NUM_];  // 6개 . 제룡행 까지. 최소 한개. 최대 6개. 가변형. 
	u_char              ucChapterFreeQuestData[99];                 // 99 바이트.  HHH
	
};

struct MSG_QUESTCOMPLETE_DATA 
{
	_sHeader             header;
	u_char               ucMessage;
	short                sCountCQ;            // 현 완료 퀘스트의 갯수.
	_sCharacQuestState   QuestData[_QUEST_MAX_NUMBER_]; // 완료된 것만 보낼때 쓰는 패킷. 80개로 줄여놨다.
};

struct MSG_QUEST_COMPLETEDATA_REQ
{
	_sHeader			header    ;
	u_char				ucMessage ;
}; //  더 필요한 것이 있는가?

// 캐릭터의 퀘스트 정보
//struct MSG_QUEST_DATA {
//	_sHeader			header ;
//	u_char				ucMessage ;
//	_sCharacQuestState	QuestData[_QUEST_MAX_NUMBER_] ;			    // 제룡행이 아닌 일반적인 퀘스트들을 저장할 때 쓴다. 숫자는 200개 정도.
//	_sCharacMainQuestState MainQuestData;                           // 제룡행 저장 공간.	
//	_sRunningQuestPacketData RunningQuestData[_MAX_CURQUEST_NUM_];  // 6개 . 제룡행 까지. 
//};


// 조건 인증 결과 패킷
// CResult 값 
//	0 : 성공
//		CInfo : 0 다음 단계, 1 퀘스트 성공 완료, 2 인벤 full
//	1 : 인증실패 
//		CInfo : 인증 실패 조건의 아이디
struct MSG_QUEST_AUTH_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char                cNodeID;                    // 노드 넘버로.
	u_short             usQuestID;
	u_short             usNextQuestID;              // 제룡행 완료일 경우 다음 퀘스트. 65535일때 제룡행 퀘스트 끝
	unsigned char		cResult ;					// 결과 
	unsigned char		cInfo ;						// 결과 정보.
	char				cNeedInven;					// 필요한 인벤수(INVEN_FULL)일때 
};

// 퀘스트 결과 강제 세팅
struct MSG_QUEST_SET_RESULT
{
	
	_sHeader			header ;
	u_char				ucMessage ;					// 
	
	char				cResult ;					// 셋팅 값 : 성공완료, 실패완료, 초기화.. 등등.
	    
	u_short             usQuestID;
	u_int               uiMoney;					// 결과 후의 최종 유저 머니.
	_sSkill_pair        SkillDelete[5];				// 없애는 스킬.
	u_char              ucNum;						// 없애는 아이템 갯수
	_sitem_pair         ItemDelete[20];				// 없애는 아이템. 
} ;


//#define MSG_NO_QUEST_SET_SELECTION_NO         79      // 선택지 번호 설정
struct MSG_QUEST_SET_SELECTION_NO 
{
	_sHeader			header        ; 
	u_char				ucMessage     ;
	u_short             usQuestID     ;
	u_char              ucSelectionNo;         // 선택지 번호 세팅하는 패킷.      
} ;




//#define MSG_NO_QUESTITEM_RECEIVE			120
struct MSG_QUESTITEM_RECEIVE
{
	_sHeader			header ;
	u_char				ucMessage ;
	u_short             usQuestID;		
#ifdef _XDEF_PERIODITEM_20061027	
	_sUser_Inven_v1_all userItem;                                       // 기간제 아이텝 삽입을 위해 완전한 정보로 준다.
#else
	char				cItemType;					// 타입에 따라 일반인지 아닌지 체크 
	unsigned short		cItemID;
	char				cInvenIndex ;				// 인벤토리 인덱스. -1이면 실패, 108 이면 돈이다. 
	unsigned int		iCount;
#endif
	char				cCountablity;				// 임시다 DBUpdate후 삭제될 데이터 
};

//#define MSG_NO_QUESTITEM_REMOVE			121 // 퀘스트 아이템 삭제
/*
struct MSG_QUESTITEM_REMOVE
{
	_sHeader			header;
	u_char				ucMessage;

	short				sQuestID;
	char				cInvenIndex;
	unsigned int		iCount;
};	
*/
struct MSG_QUESTITEM_REMOVE
{
	_sHeader			header ;
	u_char				ucMessage ;
	u_short				usQuestID;
	char				cSlotNum ;				//  -1이면 실패
	unsigned int		iCount;
	_sitem_pair         ResItem[58] ;           // 변한 아이템 인벤터리 슬롯의 목록.
};


//#define MSG_NO_LEARN_SKILL				122
struct MSG_LEARN_SKILL
{
	_sHeader			header;
	u_char				ucMessage;
	
	char				cSlotNo ;			    // 배운 스킬의 슬롯번호.
	short				sSkillID;	            // 
	u_char				ucSkillLev;				// 배운 스킬 성.
	u_char				ucSkillState ;			// 0 : 사용 가능, 1 : 수련 필요.
};

//#define MSG_NO_FORGET_SKILL				123
struct MSG_FORGET_SKILL
{
	_sHeader			header;
	u_char				ucMessage;

	short				sSkillID;
};

#define ID_JINEXP							0		// 경험치
#define ID_VITALITY							1		// 생기
#define ID_ZEN								2		// 진기
#define ID_SOUL								3		// 영기
#define ID_HEART							4		// 심기
#define ID_STAMINA							5		// 정기
#define ID_VITALPOWER						6		// 체력
#define ID_INNERPOWER						7		// 내력
#define ID_SOULPOWER						8		// 영력
#define ID_HEARTPOWER						9		// 심력
#define ID_STAMINAPOWER						10		// 정력
#define ID_HONOR							11		// 명성
//#define ID_RETRIBUTION						12		// 업
#define ID_FAME                             12          // 악명   

//#define MSG_NO_UPDATE_STATUS				124
struct MSG_UPDATE_STATUS
{
	_sHeader			header;
	u_char				ucMessage;

	char				cStatusType;
	unsigned int		uiUpdatedValue;
};

//#define MSG_NO_FORCED_MOVE				125
struct MSG_FORCED_MOVE
{
	_sHeader			header;
	u_char				ucMessage;

	float				fposX;
	float				fposZ;
};

//#define MSG_NO_JOIN_GROUP					126
struct MSG_QUEST_JOIN_GROUP
{
	_sHeader			header;
	u_char				ucMessage;

	char				cGroup;
};

// 클라이언트 에서 서버로.
#ifdef _XTS_NEWPARTYCOUNTER
struct MSG_QUEST_SETCOUNT
{	
	_sHeader            header    ;	
	u_char              ucMessage ;	
	u_short				usQuestId ;	
	char				cCounterNo;             // 업데이트해야할 계수기.	
	char                cNum      ;             // 더해질 숫자.	
	u_short				usSum	  ;				// 클라이언트에서 저장중인 총 카운트 숫자.
	
};
#else
struct MSG_QUEST_SETCOUNT
{	
	_sHeader				header    ;	
	u_char					ucMessage ;	
	u_short					usQuestId ;	
	char					cCounterNo;             // 업데이트해야할 계수기.	
	u_char					ucNum     ;             // 더해질 숫자.	
	u_char					ucSum     ;             // 클라이언트에서 저장중인 총 카운트 숫자.	
}; 
#endif


// 서버에서 클라이언트로... ( 에러시에만.)
struct MSG_QUEST_SETCOUNT_RESULT
{
	_sHeader            header    ;
	u_char              ucMessage ;
	u_short             usQuestId ;                      // 퀘스트 아이디 	
	char                cCounterNo  ;
	int                 iRet      ;                      // 리턴값이 -1일 경우 보내온 총합과 이 맞지 않음.
	// 리턴값이 -2일 경우 더한 값이 255를 넘음. (오버 플로우)	
	u_char              ucSum     ;                      // 보내기 직전에 서버에서 저장하고 있는 총 카운트 숫자. 
};

//#define MSG_NO_QUEST_REDISTRIBUTION     88      // 오기 재 설정. 
struct MSG_QUEST_REDISTRIBUTION 
{
	_sHeader            header       ;
	u_char              ucMessage    ;
	short               sLeftPoint   ;
};


//#define MSG_NO_QUEST_VALIDCHECK         89      // 퀘스트가 유효한지 검사를 한다.
struct MSG_QUEST_VALIDCHECK
{
	_sHeader            header       ;
	u_char              ucMessage    ;
	u_short	  			usQuestID    ; // 현 퀘스트 아이디.
	char				cNodeID      ; // 현 퀘스트가 가야할 노드 번호.
	char				cNeedItemRemove; // 삭제될 아이템이 있으면 1 없으면 0;
};
typedef MSG_QUEST_VALIDCHECK MSG_QUEST_VALIDCHECK_INGAME;


// #define MSG_NO_QUEST_VALIDCHECK_RESULT MSG_NO_QUEST_VALIDCHECK 
struct MSG_QUEST_VALIDCHECK_RESULT 
{
	_sHeader            header       ;
	u_char              ucMessage    ;
	u_short				usQuestID    ; // 현 퀘스트 아이디.
	char	            cNodeID      ; // 현 퀘스트가 가야할 노드 번호.
};
typedef MSG_QUEST_VALIDCHECK_RESULT MSG_QUEST_VALIDCHECK_INGAME_RESULT;  


#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	#define _V1_COUNT_REAL_INVEN_			150		// 실제 인벤토리
#else
	#define _V1_COUNT_REAL_INVEN_			100		// 실제 인벤토리
#endif

//#define MSG_NO_QUESTNODEBACK_ITEM_RESULT 92 
struct MSG_QUEST_NODEBACK_ITEM_RESULT
{
	_sHeader            header       ;
	u_char              ucMessage    ;
	u_int               uiMoney      ;				// 최종 돈.
	int                 contribution ;              // 최종 문파기여도.
	_sSkill_pair	    removeskill[5];				// 지워지는 스킬.
	_sitem_pair         removeitem[_V1_COUNT_REAL_INVEN_];				// 없어지는 아이템. 
	_sUser_Inven_v1_all additem[10];				// 주어지는 아이템 . 최대 숫자는 10개.	
};


//#define MSG_NO_MONSTER_KILL					93 // 몬스터 죽은 체크 
struct MSG_QUEST_MONSTER_KILL
{
	_sHeader            header;
	u_char              ucMessage;
	u_short             MobType;
};

struct _sPartyCounter
{
	u_short				usPersnal_Counter[4];
	
	int AllCounterAdd(){ return(static_cast<int>( usPersnal_Counter[0] + usPersnal_Counter[1] + usPersnal_Counter[2] + usPersnal_Counter[3] ) ); }
};

// 파티퀘스트에서는 항상 카운터 결과 값을 받는다.
//#define MSG_NO_QUEST_PARTY_SETCOUNT_RESULT	94 // 파티 퀘스트에서 카운트 결과
struct MSG_QUEST_PARTY_SETCOUNT_RESULT
{
	_sHeader            header			;
	u_char              ucMessage		;
	u_short             usQuestId		;         // 퀘스트 아이디 
	char                cMvp			;         // MVP	
	_sPartyCounter		_sPCounter[9]	;         // 파티 카운터
};      

//#define MSG_NO_QUESTRANK_REQ				95		// 퀘스트 랭크	
struct MSG_QUESTRANK_REQ
{
	_sHeader			header;
	u_char				ucMessage;
	
	u_short				usQuestID;
};

//#define MSG_NO_QUESTRANK_RES			MSG_NO_QUESTRANK_REQ // 퀘스트 랭크 결
struct MSG_QUESTRANK_RES
{
	_sHeader			header;
	u_char				ucMessage;
	
	u_short				usQuestID;
	_QUEST_RANK_DATA	Data;
};


