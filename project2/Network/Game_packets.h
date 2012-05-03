#pragma once

#pragma pack(push, enter_game)
#pragma pack(1)

struct _sHeader
{
	u_short	sLength : 12;		// -2048 - 2047
	u_short	sCrypto : 2;		// 0 : false, 나머지는 true
	u_short	sCompressed : 2;	// 0 : flase, 나머지는 true
	
	_sHeader() {}
	_sHeader(u_short len) : sLength(len), sCrypto(0), sCompressed(0) {}
};

#define _MAX_QUEST_INVENTORY_				5


// 아래 구조체 하나로 모든 장착 정보를 얻데이트 할 수 있도록, 장착정보를 모두 포함한다. 
// 캐릭터 리스트에 들어갈 구조체

struct _CharacList // # [내부] 캐릭터의 기본 정보 # 
{ 
	int				iUniqueID ;				// 캐릭터의 유니크 아이디 
	char			cName[13] ;				// 캐릭터 의 이름 
	char			cNick[13] ;				// 캐릭터 별칭 

	char			cZone ; 

	char			cSex ;					// 성별 0 : 남자, 1 : 여자 
	char			cFace ;					// 얼굴형 : max 16종 
	char			cHair ;					// 머리 스타일 : max 16종 
	char			cGroup ;				// 소속 문파

	char			cAmorType;				// 상의	0
	char			cPantsType;				// 하의	1
	char			cCapType;				// 모자 2
	char			cAdaeType;				// 아대 3
	char			cShoesType;				// 신발 4
	char			cBackPackType;			// 배낭 
	char            cWeaponType ;           // 무기 타입
	char			cMaskType;				// 치장 타입

	short			sAmor ;					// 상의 0 
	short			sPants ;				// 하의 1 
	short			sCap ;					// 모자 4 
	short			sAdae ;					// 아대 2 
	short			sShoes ;				// 신발 3 
	short			sBackPack ;				// 배낭 7 
	
	short			sSelectWeaponID ;		// 선택한 무기 인덱스. 0, 1 ( 각각 무기1, 2 ) 

	short			sMask;					// 치장 8
	u_char			ucWeaponStrength ;		// 현재 무기의 제련 정도.

	short			sStr ;					// 근력 <= 원랙는 정기 
	short			sZen ;					// 진기 <= 그대로 
	short			sIntelligence ;			// 지혜 <= 영기 
	short			sConstitution ;			// 건강 <= // 건강 <= 생기 
	short			sDexterity ;			// 민첩 <= 심기 

	short			sInnerLevel ;			// 내공 수위

	// 2004/11/10 Ray 추가. class, classgrade
	char			cClass ;				// 역할 
	char			cClassGrade ;			// 역할 등급
	
	char			cGM ;					// GM 여부..
	
	// 2005/01/07 Ray 추가 : 선택창에서 캐릭터의 위치 표시.
	float			fPosX ;					// 최종 위치.
	float			fPosZ ;					//

#ifdef _CHARAC_BLOCKING
	int				iBlockingEndDate ;		// 블럭킹 만료 시간
#endif	
//#ifdef _XTS_SERVERUNIFICATION	// 통합 완료 이후에도 계속 쓰임
#ifdef _XTS_NEWCHARACLIST
	u_char				ucChangeNameFlag : 2 ;	// 0 - Not, 1 - Possible Change Name
	u_char				ucLeftFlag : 6 ;		// 아직 안쓰임.
#endif
} ;

// 퀘스트 정보
struct _sCharacQuestState
{
	bool	bReword  : 1 ;	// 보상을 받았는가
	u_char	ucNodeID : 7 ;	// 현 Node 아이디 언사인드 캐릭터로 해야한다.

	// 추가된 것.
	u_short usQuestID;      // 현 협객, 기연, 무림 퀘스트.

	_sCharacQuestState()
	{
		bReword = false;
		ucNodeID = 0;
	}
} ;

//메인 퀘스트 저장과 관련된 구조체. 
struct _sCharacMainQuestState
{
	bool			bReword  : 1 ;	// 보상을 받았는가
	unsigned char	ucNodeID : 7 ;	// 현 Node 아이디
	unsigned short  sQuestID;       // 현 제룡행 퀘스트 아이디. 워드로 하는 것이 낫지 않을까?
	u_char			padding;        // 여분의 공간.
	
	_sCharacMainQuestState()
	{
		sQuestID = 0;
		ucNodeID = 0;
		bReword  = false;
	}
} ;

// 퀘스트 아이템 구조체
struct _sQuestItem
{
	short			sID;
	unsigned int	uiCount;

	_sQuestItem() : sID(0), uiCount(0)
	{}
};


#define QUEST_HISTORY_COUNT 50 

struct _sQuestHistory
{
	char cNodeHistory;
	char cBranchHistory;
};

struct _sRunningQuest
{
	u_short usQuestID;
	char  cNodeID;
	int   dwQuestStartTime;
	int	  dwQuestTimer;
	char  cTimerNodeNo;
	bool  bBeUse;
	
	char   cTFRetry;       // 재 보상을 하려 할때 트루 액션 리트라이인지 폴스액션 리트라이인지.
	u_char ucHistoryCount; // 제한이 255개.9+
	
	_sQuestHistory QuestHistory[QUEST_HISTORY_COUNT]; // 50개까지로 줄여졌다. 
	_sRunningQuest()
	{
		usQuestID        = 0;
		dwQuestTimer     = 0;
		dwQuestStartTime = 0;
		ucHistoryCount   = 0;  // 퀘스트의 트리거 결과를 저장하기위한 히스토리에 저장된 갯수.
		cTFRetry         = 0;  // 재 보상일 경우 그 전에 fail 액션을 실행한 건지, true 액션을 실행한 건지. 							    
		cNodeID          = 0;  
		cTimerNodeNo     = 0;
		bBeUse           = false;
	}
} ; 



// 런닝퀘스트데이터 내려 보낼때 히스토리도 포함되어야 한다. 
// 히스토리 데이터는 분기와는 상관없이 노드값만 보내주면 된다.
struct _sRunningQuestPacketData
{
	u_short usQuestID;
	int   dwQuestStartTime;
	int	  dwQuestTimer;
	char  cTimerNodeNo;
	u_char ucSelectNo; 
	u_char ucCounter[4];
	
	_sRunningQuestPacketData()
	{
		usQuestID		 = 0;
		dwQuestStartTime = 0;
		dwQuestTimer	 = 0;
		cTimerNodeNo	 = 0;
		ucSelectNo       = 0;
		ucCounter[0]     = 0;
		ucCounter[1]     = 0;
		ucCounter[2]     = 0;
		ucCounter[3]     = 0;
	}
	
} ;

#ifdef _XDEF_EXPAND_ORGANIZATIONNAME
#define					_XDEF_ORGANIZATIONNAMELENGTH			24
#else
#define					_XDEF_ORGANIZATIONNAMELENGTH			8
#endif
// 랭크 퀘스트 관련 구조체 
struct QUEST_RANK_RECORD
{
	unsigned char	m_ucClan;
	char			m_acOrgName[_XDEF_ORGANIZATIONNAMELENGTH+1];		//개인: 無,			동행: 無,		단: 영웅단 이름
	char			m_acName[13];		//개인: 이름,		동행: 행수이름,	단: 단주이름
	unsigned short	m_usLevelOrCount;	//개인: 성취단계,	동행: 인원수,	단: 단인원
	unsigned short	m_usQuestData;		//KillCount, Time, ItemCount
};

#define MAX_QUEST_RANK_RECORD	(10)

struct _QUEST_RANK_DATA
{
	QUEST_RANK_RECORD	m_aRankRecord[MAX_QUEST_RANK_RECORD];
};


// 0 ~ 15 번까지 가능.
struct _sCharac_Skill
{
	short			id;
	char			cStep;				// 무공 성장 단계

	u_char			ucSkillState : 2;	// 0 : 전수 완료   1 : 미완료
	u_char			ucPassiveState : 1;	// Passive on/off
	u_char			ucCurMission : 5;	// 미션 진행 카운트 0-31

	int				iCurSkillExp;
};

struct _sSkill_pair
{
	short	    sSkillID; // 스킬 아이디
	char        number;   // 스킬 성 -1로 갈경우 삭제.
};

// 아이템 관련
union __item_or_money
{
	struct
	{
		u_short	 usCurDur ;			  // 현재 내구력
	    char	 cIdentify     : 4;	  // 감정 상태
	    u_char   ucSocketCount : 4;   // 소켓이 몇개까지 뚫릴 수 있는가?
	    u_char   ucCount ;			  // 최대 머지 255개
	} S_NormalItem;
	struct 
	{
		u_int	 uiCount ;	 		  // 머니..
  	} S_MoneyItem;
} ;	// ==> 이전과 동일.

struct __inven_uselimit_info_v1	// 사용제한 등의 기타 정보.
{
	u_char				ucFlag1 ;		// Flag1 = 사용기한이 있는가 없는가..
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
	u_short				usResaved		: 1;	// 기본강도 설정이 제대로 되었는가, 처음 초기화시에 한번 쓰인다. : 서버만.
	u_short				usUsedProtect	: 1;	// 용루를 사용한 적이 있는가.
	u_short				usCurProtect	: 5;	// 무기의 경우, 바른 용린의 수. 0 - 15
	u_short				usCurMaxProtect	: 5;	// 최대강도

	u_short				usReserved		: 4;	// 예약분.
#else
	u_char				ucFlag2 ;		// 
	u_char				ucFlag3 ;
#endif
	u_short				usValue1 ;		// 

	u_char				ucYear ;		// Year - 2000 : 5 ~ 시작.
	u_char				ucMonth ;		// Month
	u_char				ucDay ;			// Day
	u_char				ucHour ;		// Hour
	u_char				ucMinute ;		// Minute 
} ;

// 무기/의상을 제외한 아이템들의 정보.
struct _sUser_InvenItem
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
    u_char					cSlot ;			// 슬롯 번호.
#else
	char					cSlot ;			// 슬롯 번호.
#endif
	char					cType ;			// 아이템 대분류 9
	char					cSecond ;		// Second는 초기화 될 때마다, 새로이 설정한다.
	short					sID ;			// 아이템 소분류. (아이템 아이디) 12
    
	__item_or_money			Item_Money ;		// 16 		

	u_char					ucIncludeLimit : 1 ;// 뒤에 사용제한 정보가 오는가. 0-no, 1-yes
	u_char					ucBooleanRes : 7 ;	// 
} ;

// ---------------------------------------------------------------------=
// 무기/의상이 아닌 경우, ucIncludeLimit 가 1이면, 이 구조체이다.
struct _sUser_Inven_v1_limit : public _sUser_InvenItem
{
	__inven_uselimit_info_v1	uselimit ;
} ;
// ---------------------------------------------------------------------=


// ---------------------------------------------------------------------=
// 무기/의상의 정보는 이것으로. 
struct _sUser_Inven_v1_extension : public _sUser_InvenItem
{
	short					sSocket[4];		// 24

	u_char					ucStrength ;	// 무기의 경우, 강도.

	//Author : 양희왕
	//u_char	ucCurProtect : 4;	// 무기의 경우, 바른 용린의 수. 0 - 15
	//u_char	ucIncProtect : 3;	// 최대강도 중가분.
	//u_char	ucUsedProtect : 1;	// 용루를 사용한 적이 있는가.
	u_char					ucSharpness ;	// 무기의 경우, 예기.
} ;

// 무기/의상의 경우, ucIncludeLimit 가 1이면, 이 구조체로 형변환.
struct _sUser_Inven_v1_all : public _sUser_Inven_v1_extension
{
	__inven_uselimit_info_v1	uselimit ;
} ;
// ---------------------------------------------------------------------=
struct _sUser_Inven
{
	u_char ucIdx;
	_sUser_Inven_v1_all Item;
};

// 스토리지
struct _SM_Storage_Inven
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
    u_char				cIdx;			// 서버쪽 스토리지 인덱스 or 유저 인벤토리 인덱스.
#else
	char				cIdx;			// 서버쪽 스토리지 인덱스 or 유저 인벤토리 인덱스.
#endif
	u_char      		usNumber;		// 저장할 갯수. 
	char                cIsstackable;   // 셀 수 있는 것인지 나타내는 변수임. 
};

// 몬스터가 바닥에 떨군 아이템
struct _sItem_Drop						// 바닥에 있는 아이템 항목 하나. 
{
	int				iItemWorldID;
	// 2004.05.31->oneway48 modify
	float			fX;
	float			fZ;
	//	short			fX;
	//	short			fZ;
	// modify end 
	u_short			usOwner ;
	char			cType;				// 아이템 대분류 2^4 : 16
	
	union {
		struct {
			char				cSecond ;		// 아이템 중분류
			char				cIdentify ;		// 0,1,2,3 까지만.
			short				sID ;			// 아이템 소분류. (아이템 아이디)  ** 안보인다면.. 필요 없음.
		} S_detail ;
		u_int			uiCount ;
	};
	
	u_char				ucCount ;		// 개수
};

struct _sitem_pair
{
	u_char			slot;				// 슬롯
	u_char			number;				// 개수
};

struct _tradeitem
{
	char			index;
	unsigned short	quantity;
};

//수집 데이터
#define COLLECTIONDATA_COMPLETEFLAG_INDEX	(15)
extern WORD							g_CollectonWordFilter[16];
class ckCollectionData
{
private:
	unsigned short	m_usData;
public:
	inline unsigned short	GetData()	{	return m_usData;	}	
	inline void ckCollectionData::SetTrue(unsigned char index) { m_usData = m_usData | g_CollectonWordFilter[index]; };	
	inline void ckCollectionData::SetFalse(unsigned char index) { m_usData = m_usData & (g_CollectonWordFilter[index] ^ USHRT_MAX); };
	inline void ckCollectionData::SetCompleted() { m_usData = m_usData | g_CollectonWordFilter[COLLECTIONDATA_COMPLETEFLAG_INDEX]; };
	
	inline bool ckCollectionData::IsThisTrue(unsigned char index)
	{
		if((m_usData & g_CollectonWordFilter[index]) == 0)
			return false;
		return true;
	};	
	inline bool ckCollectionData::IsThisCompleted()
	{
		if((m_usData & g_CollectonWordFilter[COLLECTIONDATA_COMPLETEFLAG_INDEX]) == 0)
			return false;
		return true;
	};	
};

#define MAX_COLLECTION_INDEX_COUNT	(512)
struct _CHARAC_COLLECTION_DATA	//DB에 저장하는 데이터 단위
{
	ckCollectionData		m_aData[MAX_COLLECTION_INDEX_COUNT];
};

union __charac_state
{
	short				sWoundRate ;			// 외상 비율 0 - 100
	short				sInsideWoundRate ;		// 내상 비율 0 - 100
	u_short				usFatigueRate ;			// 피로도	 0 - 100
	
	short				sRetribution ;			// 업보 
	int					sHonor ;
	
	short				sShowdowm ;				// 비무 전적
	short				sFuryParameter;			// 폭주 값 (현재 분노 수치)
	
	short				power ;					// Power 값.
	short				stat ;					// Stat 스탯 값.
	
	int					iMatchRecord;			// 비무 관련 값 세팅
} ;

enum EN_STATE_PACKET_FLAG
{
	charac_state_en_wound_rate=0,		// 외상
	charac_state_en_inside_rate,		// 내상 1
	charac_state_en_fatigue, 			// 피로도 2
	charac_state_en_retribution,		// 업보 3
	charac_state_en_honor,				// 명성 4
	charac_state_en_showdown,			// 비무전적 5
	charac_state_en_fury,				// 분노 6
	charac_state_en_fame,				// 7 악명
	
	charac_state_en_res2,				// 8
	charac_state_en_res3,				// 9
	charac_state_en_res4,				// 10
	charac_state_en_life_power,			// 체력 업데이트 11 
	charac_state_en_force_power,		// 내력 업데이트 12
	charac_state_en_concentration_power,// 의지 업데이트 13
	charac_state_en_max_life,			// 최대 체력 14
	charac_state_en_max_force,			// 최대 내력 15
	charac_state_en_max_concentration,	// 최대 의지 16
	charac_state_en_constitution,		// 건강 17
	charac_state_en_zen,				// 진기 18
	charac_state_en_intelligence,		// 지혜 19
	charac_state_en_dexterity,			// 민첩 20
	charac_state_en_strength,			// 근력 21

	charac_state_en_match_grade=30,		// 비무 계급장 업데이트 30
	charac_state_en_match_win,			// 개인 승수 업데이트 31
	charac_state_en_match_lose,			// 개인 패수 업데이트 32
	charac_state_en_match_tie,			// 개인 패수 업데이트 33
	charac_state_en_match_max_win_sr,	// 개인 최대 연승수 업데이트 34
	charac_state_en_match_cur_win_sr,	// 개인 현재 연승수 업데이트 35
	
	
	charac_state_en_match_gr_win=40,		// 그룹 승수 업데이트
	charac_state_en_match_gr_lose,			// 
	charac_state_en_match_gr_tie			//
} ;

struct _sND_TimeUnit
{
	// 년/월/일/시/분/초 EndTime
	u_int		uiYear		: 5;		// base 2000
	u_int		uiMonth		: 4;
	u_int		uiDay		: 6;
	u_int		uiHour		: 5;
	u_int		uiMinute	: 6;
	u_int		uiSecond	: 6;
};

//Author : 양희왕 // breif : 친구소환
//packet_for_pk.h 에서 이동 
#define en_charac_name_length	12

#include "packet_for_login.h"
#include "packet_for_lobby.h"
#include "packet_for_zone_monster.h"
#include "packet_for_battle.h"
#include "packet_for_quest.h"
#include "packet_for_skill.h"
#include "packet_for_level.h"
#include "packet_for_ctrl_gms.h"

#include "packet_for_items_trade.h"
#include "packet_for_messenger.h"
#include "packet_for_svrmove.h"
#include "packet_for_party.h"
#include "packet_for_boss.h"	//보스전에 관한 패킷 
#include "packet_for_nickname.h" //별호에 관한 패킷 
#include "packet_for_organization.h"	// 분파에 관한 패킷
#include "packet_for_personalstore.h"	// 개인 상점에 관한 패킷

#include "packet_for_event.h"			// Event 관련 패킷
#include "packet_for_collection.h"		// 수집
#include "packet_for_unigrouplobby.h"	// 통합서버 관련
#include "packet_for_cashitem.h"		// Cash Item Mall 관련 패킷
#include "packet_for_cb.h"				// 장원전 관련 패킷
#include "packet_for_xtrap.h"
#include "packet_for_pk.h"				// PK에 관련된 패킷 (pk범 위치 추적등..)
#include "packet_for_match.h"			// 비무
#include "packet_for_MnD.h"				// 사제 시스템


#pragma pack(pop, enter_game)
