#pragma once

#pragma pack( push, enter_global_struct )
#pragma pack(1)




/*#################################################################################

규칙 : 
1. 서버간 통신( TCP : GS<=>DS ) 패킷은 첫글자로 'C'를 넣는다.
( 클라이언트와 GS의 통신 패킷과 겹칠수 있으므로 )
2. 서버간 통신( UDP ) 는 'U' 를  사용한다.

3. DS->GS 로 보내는 응답은 모두 int uid를 포함해야 한다. 

8개 단위로 구분하는 이유는 if 문의 과도한 검사를 방지하기 위해서임.
message 를 8로 나누어( 쉬프트 연산 ' >> 3 ' 과 동일 ) 몫과 나머지로 
구분해 몫과 나머지로 메시지를 구분한다. 

( 그래서 음수는 왠만하면 피함. 
물론 음수를 써도, 그대로 적용할 수 있음, 다만 알아보기 힘듦.) 

추가 :
(창고 관련 패킷)10/21
#define CMSG_NO_DEPOT_REQ_GS_DS				25		// GS->DS Depot data request..
#define CMSG_NO_DEPOT_RES_GS_DS				26		// DS->GS Depot data result..
#define CMSG_NO_UPDATE_DEPOT_				53			// _USER_DEPOT

#################################################################################*/


/*
	인벤 구조체
*/



union __item_detail
{
	char		weapon_none ;
	char		cloth_equippos ;			// 의상일 경우 장착 위치
	char		book_none ;
	char		potion_none ;
	char		backpack_count ;			// 배낭의 경우 용량
} ; 


union __item_or_money
{
	struct
	{
		u_short	 usCurDur ;		// 현재 내구력
		char	 cIdentify     : 4;	// 감정 상태
		u_char   ucSocketCount : 4; 	// 소켓이 몇개까지 뚫릴 수 있는가?
		u_char   ucCount ;		// 최대 머지 255개
	} S_NormalItem;
	struct 
	{
		u_int	 uiCount ;	 	// 머니..
	} S_MoneyItem;
} ;


struct _sServer_InvenItem
{
	unsigned __int64    ui64UniqueId;
	char				cType ;					// 아이템 대분류
	char				cSecond ;
	short				sID ;					// 아이템 소분류. (아이템 아이디)

	__item_detail		detail ;				// 아이템 타입에 따라 다른 정보.	
	__item_or_money		Item_Money ;			 		
	u_char              ucSocket[4];            
} ;


struct __inven_uselimit_info_v1	// 사용제한 등의 기타 정보.
{
	u_char				ucFlag1 ;		// Flag1 = 사용기한이 있는가 없는가..

	u_short				usResaved		: 1;	// 기본강도 설정이 제대로 되었는가, 처음 초기화시에 한번 쓰인다. : 서버만.
	u_short				usUsedProtect	: 1;	// 용루를 사용한 적이 있는가.
	u_short				usCurProtect	: 5;	// 무기의 경우, 바른 용린의 수. 0 - 15
	u_short				usCurMaxProtect	: 5;	// 최대강도

	u_short				usReserved		: 4;	// 예약분.u_short				usValue1 ;		// 

	u_short				usValue1 ;		// 

	u_char				ucYear ;		// Year - 2000 : 5 ~ 시작.
	u_char				ucMonth ;		// Month
	u_char				ucDay ;			// Day
	u_char				ucHour ;		// Hour
	u_char				ucMinute ;		// Minute 
} ;



struct _sServer_InvenItem_v1
{
	u_int64					ui64UniqueId;	// 8 
	char					cType ;			// 아이템 대분류 9
	char					cSecond ;		// Second는 초기화 될 때마다, 새로이 설정한다.
	short					sID ;			// 아이템 소분류. (아이템 아이디) 12

	__item_or_money			Item_Money ;	// 16 		

	// 클라이언트 패킷 자르기 1. 소켓/기타 정보의 유무에 따라, 잘라서 보낸다.
	// 무기/의상의 경우에는 있고, 아니면 없다. 
	short					sSocket[4];		// 24

	u_char					ucStrength ;	// 무기의 경우, 강도.
	u_char					ucSharpness ;	// 무기의 경우, 예기.

	// 클라이언트 패킷 자르기 2. 사용제한
	__inven_uselimit_info_v1	uselimit ;

	inline void convert_from_v0( _sServer_InvenItem * pv0 )
	{
		ui64UniqueId	= pv0->ui64UniqueId ;
		cType			= pv0->cType ;
		cSecond			= pv0->cSecond ;
		sID				= pv0->sID ;
		Item_Money.S_MoneyItem.uiCount = pv0->Item_Money.S_MoneyItem.uiCount ;	// 카운트하나만 복사하면 다 된거다.
		sSocket[0]		= static_cast<short>(pv0->ucSocket[0]) ;
		sSocket[1]		= static_cast<short>(pv0->ucSocket[1]) ;
		sSocket[2]		= static_cast<short>(pv0->ucSocket[2]) ;
		sSocket[3]		= 0 ;

		uselimit.ucFlag1	= 0;
		uselimit.usResaved	= 0;
		uselimit.usUsedProtect = 0;
		uselimit.usCurProtect	= 0;
		uselimit.usCurMaxProtect= 0;
		uselimit.usReserved		= 0;
		uselimit.usValue1	= 0 ;		// 

		uselimit.ucYear	= 0 ;		// Year - 2000 : 5 ~ 시작.
		uselimit.ucMonth	= 0 ;		// Month
		uselimit.ucDay	= 0 ;			// Day
		uselimit.ucHour	= 0 ;		// Hour
		uselimit.ucMinute	= 0 ;		// Minute 
	}
} ;

/*#################################################################################

기본 구조체 

#################################################################################*/
// 캐릭터 기본 정보 : 기본 정보

#ifdef CHARAC_CREATE_DATE_20070309
struct _sCharac_Create_Date 
{
	short sYear	: 6;	//(0 ~ 63) : Base 2000
	short sMonth : 4;	//(0 ~ 15)
	short sDay : 6;		//(0 ~ 63)
};
#endif

struct _CHARAC_BASIC {
	//	ND_V01_Charac
	int		iUniqueID;				// 유니크아이디	4
    char	cAccount[en_max_lil+1];				// 유저 아이디  17   <----요거 바뀌었어요
	char	cChrName[13];			// 캐릭터 이름  30
	char	cChrNic[13];			// 캐릭터 별호  43
	char	cSex;					// 성별  44
	char	cParty;					// 파티  45
	char	cGamete[13];				// 배우자  58
	char	cHair;					// 머리모양  59
	char	cFace;					// 얼굴모양	60
	char	cLuck;					// 운빨  61

	char	cClass ;				// 역할 : 기본값 0
	char	cClassGrade ;			// 역할 등급 : 기본값 0		: 2004/11/10 추가. Ray
	int		iContribution ;			// 문파 기여도 : 기본값 0	: 2004/11/10 추가. Ray

	char	cGM_Check ;				// GM 캐릭터 여부..
	DWORD	dwPlayTime ;				// 설정

	u_char	ucChangeName ;			// 이름 변경 가능 플래그 0 - not, 1 - possible

#ifdef CHARAC_CREATE_DATE_20070309
	_sCharac_Create_Date	CharacCreateDate;
#endif

} ;



//// 캐릭터의 오기 관련 정보와 위치
struct _CHARAC_CUR_BASIC {
	//	ND_V01_CurBasicInfo
	short	sZone;					//지역  2
	short	sY;						//  4
	float	fX;						//위치좌표 8
	float	fZ;						// 12
	short	sLifePower;				//현재 체력  14
	short	sForcePower;			//현재 내력  16
	short	sConcentrationPower;	//현재 영력  18

	// 추가
	char	cRespawnServerNo ;		// 부활 지정된 서버 번호
	char	cRespawnPosName[13] ;	// 부활 위치의 이름
	float	fRespawnPosX ;			//
	float	fRespawnPosZ ;			//
};



// 2004/06/16 업데이트
//// 오기최대값과 경험치
struct _CHARAC_LEVEL {
	//	ND_V01_5StarLevel
	short	sMaxLifePower;				//생명  24  <= 생력
	short	sMaxForcePower;				//내공  26  <= 내력
	short	sMaxConcentrationPower;		//의지력 28 ? 그냥 영력으로...	
	short	sConstitution;				//건강  36 <= 생기
	short	sZen;						//진기  40 <= 진기
	short	sIntelligence;				//지혜  44 <= 영기
	short	sDexterity;					//민첩  48 <= 심기
	short	sStr;						//근력  52 <= 정기
	short	sLeftPoint;					//남은 포인트
	short	sMasteryPoint  ;			// 마스터링 포인트...
};

// 경험치와 상태
// 2004/04/12 : 피로도 타입 변경, 외상/내상 추가
struct _CHARAC_STATE {
	//	ND_V01_ChatacState
	short	sInnerLevel;			// 내공수위 4
	unsigned int uiJin;				// 진
	int		iGong;					//	악명
	short	sRetribution;			//	업보  6
	int		iHonor;					//	명성  8
	short	sShowdown;				//비무전적	10
	u_short	usFatigue;				// 피로도 : 기본값 0
	short	sWoundValue ;			// 외상 : 기본값 0
	short	sInsideWoundValue ;		// 내상 : 기본값 0 	
	short	sFuryParameter ;		// 분노 파라미터.
	int		sLevelUpGameSecond ;	// 레벨 업 한 게임내 초 시간.( getGlobalTime() )
	int		iORIndex ;				// 조직 인덱스..	

	short	sPeaceMode ;			// 태세. 0 - 평화모드,  1 - 전투 모드 : 2005-04-14 Ray..

	int		iMuteTime ;				//	채팅금지 시간..(초)
	int		iHonorGaveDate ;		//	명성 평가해준 날짜 : 다른 것으로 쓰인다.
	int		iHonorTakeDate ;		//	명성 평가받은 날짜
	int		iHiding ;				//	은신중
	int		iBlockingEndDate ;		//	블럭킹 종료 날짜(~까지)
	
	short	sPkPrevDeadMode ;
	short	sPkDeadCount ;
	short	sPkKillCount ;
	short	sMonsterKill ;				// 현재 죽인 몬스터 수

	// Not save
	short	sPartyIndex ;			// 현재 의협중이라면 인덱스값, 아니면 0.
	short	sPartySlotNo ;			// 파티중의 슬롯 번호 ???

#ifdef _PD_CASTLE_STEP_1_
	u_short usCombatDeadCount;		// 한번의 장원전 중 죽은 회수
	u_short usCombatLeftPenaltyTime;// 죽은 후 남은 패널티 시간.

	int		iCombatLastDeadTime;	// 장원전 중 마지막으로 죽은 시간. 유효성 판단에 쓰임.
#endif

#ifdef _PD_LEVEL_RATE_SAVE_
	short	sLevelModifyFlag;		// 레벨테이블이 변경되면, DB 작업으로 1로 설정될 것이다.
	double	dLevelRate;				// 현 레벨의 성취 비율.
#endif
#ifdef  _ORG_PROMOTION_	// 단해체시간,탈퇴시간 추가
	int		iOrgDisbandTime;		///<단 해체 시간
	int		iOrgDropTime;			///<단 탈퇴 시간 
#endif//_ORG_PROMOTION_
};


struct _CHARAC_QUEST_DATA 
{
	BYTE    ucCurQuestNum;      // 현 몇개의 퀘스트가 실행되고 있는가.
	BYTE    cCurQuestData[162]; // 런닝 퀘스트 저장. 런닝5개 + 제룡 1개 
	BYTE    cMainQuestData[4];  
	BYTE    ucMainStoryNum;     // 제룡행 몇 장인가?
	short   sCompleteQuestNum;  // 완료된 퀘스트 갯수.    
	BYTE    cQuestData[900];    // 기연행 협객행 저장 
	BYTE	cChapterFreeQuest[100] ; // 쳅터 완료
#ifdef _PD_QUEST_MAX_600_
	struct __quest_data2
	{
		BYTE	cQuestData2[900];	// 퀘스트 정보 확장 ( +300 개 )
	} QuestData2;
#endif
};

// 유저에게 보내는 마스터리 스킬의 포인트 정보.
struct _sMasterySkill ;
struct _sServer_Mastery_Info
{
	short				skillID ;			// 마스터리 스킬의 아이디
	char				cPoint ;			// 찍은 포인트. 0일 수 없다. 0이면 가지 않음.
	char				cReserve ;			// 예약. 안쓰임.

	_sMasterySkill *	pMastery ;		// 레퍼런스 포인터.
} ;

struct _CHARAC_MARTIAL_DATA {
	union
	{
		struct _Martial {
			BYTE	cMartialData[600];		// 배운 스킬 정보
			BYTE	cMasteryData[384];		// 찍은 오의 정보
			BYTE	cCoolTimeData[256] ;	// 256 bytes sizeof() * _COUNT_COOL_TIME_
		} MartialData ;
		BYTE		cData[sizeof(_Martial)] ;
	} ;
	union
	{		
		struct _BookInfo {
			short		sBookID ;
			u_int		uiBookLeftTime ;			// 남은 시간(초단위) : 기본값 0

			short		sBookPad1 ;
			short		sBookPad2 ;					// 예약.

			short		sBookReserved1 ;			// 예약 사항
			short		sBookReserved2 ;			// 
			short		sBookReserved3 ;			// 
			int			iBookReserved4 ;
		} S_Bookinfo ;
		BYTE		cBookData[sizeof(_BookInfo)] ;
	} ;
#ifdef _PD_SKILL_EXTEND_
	BYTE cExtendMartial[400];
#endif
} ;


struct Elixir_Data
{
	struct _Data
	{
	public:
		short __Data[10]; 
		char  __Grade;
	
		short __Get_Data( char idx ){ return __Data[ idx ]; }
		void  __Set_Data( char idx, short Value ){ __Data[ idx ] = Value; }
		char  __Get_Grade(){ return __Grade; }
		void  __Set_Grade( char Value ){ __Grade = Value; } 
	};
public:	
	_Data Data[5];		  // 오의별 데이터. 
	// 0 근력 
	// 1 진기 
	// 2 지혜			
	// 3 건강 
	// 4 민첩 

	short _sIntoxication; // 중독 수치.

protected:
	char  Temp[3];		  // [0]번째는 타이머 사용중이었는지 만약 사용중이었다면 다음 로그인 시에 타이머에 재 등록한다.

public:
	char _Get_IsTimerAct(){ return Temp[0]; }
	void _Set_IsTimerAct(char cValue ){	Temp[0] = cValue; }

	short _Get_Add_Data( char idx ) // 인덱스가 오의를 나타낸다. 오의중 하나를 합산해서 리턴하는 함수.
	{
		short sSum = 0;
		char cGrade = Data[idx].__Get_Grade();
		for( char cc = 0; cc < cGrade; ++cc )
		{
			sSum += Data[idx].__Get_Data(cc);
		}
		return sSum;
	}

	/* 중독 수치 */ 
	short _Get_Intox(){ return _sIntoxication; }
	void  _Set_Intox( short Value ){ _sIntoxication = Value; }

	void  _Init()
	{
		memset( &Data, 0, sizeof(_Data) * 5 ); 
		_sIntoxication = 0;
		Temp[0] = 0;
		Temp[1] = 0;
		Temp[2] = 0;
	}

	/* 각각의 등급 */ 
	char _Get_Grade( char idx ){ return Data[idx].__Get_Grade(); }
	void _Set_Grade( char idx, char Value ){ Data[idx].__Set_Grade( Value ); }

	/* 영약을 섭취했을 때 가감되는 수치 */ 
	void _Set_ElixirValue( char idx, short Value, char Grade )
	{
		Data[ idx ].__Set_Data( Grade,Value ); 
		if( !( Data[ idx ].__Get_Grade() >= Grade + 1 ) ) 
			Data[ idx ].__Set_Grade( Grade + 1 );
	}
};

//수집 데이터
#define COLLECTIONDATA_COMPLETEFLAG_INDEX	(15)
class ckCollectionData
{
public:
	unsigned short	m_usData;
	inline unsigned short	GetData()	{	return m_usData;	}
	bool					IsThisTrue(unsigned char index);
	void					SetTrue(unsigned char index);
	void					SetFalse(unsigned char index);

	bool					IsThisCompleted();
	void SetCompleted()
	{
		m_usData = m_usData | 0x8000 ;
	} ;
};


#define MAX_COLLECTION_INDEX_COUNT	(512)
struct _CHARAC_COLLECTION_DATA	//DB에 저장하는 데이터 단위
{
	ckCollectionData		m_aData[MAX_COLLECTION_INDEX_COUNT];
};

#ifdef _PD_ADD_EVENT_COLLECTION_
#define BASE_EVENT_COLLECTION_INDEX				(1000)
#endif


#ifdef _PD_CASH_INVEN_
	#define _MAX_INVEN_SLOT_COUNT_		(158)
	#define _CASH_INVEN_START_SLOT_		(100)
	#define _TEMP_INVEN_START_SLOT_		(150)
#else
	#define _MAX_INVEN_SLOT_COUNT_		(108)
	#define _TEMP_INVEN_START_SLOT_		(100)
#endif

#define _INVEN_SIZE_				(3888)
#define _CASH_INVEN_SIZE_			(1800)

#define _MED_CHECK_SIZE	110
#define _COLLECTION_DATA_SIZE	sizeof(_CHARAC_COLLECTION_DATA)
struct _CHARAC_INVENTORY 
{
	BYTE    cInventory[_INVEN_SIZE_];	
#ifdef _PD_CASH_INVEN_
	BYTE	cCashInven[_CASH_INVEN_SIZE_];
#endif
	short	sSelectWeaponID ;			//없을땐 0, 장착중이면 인덱스를 주는 것이 아니라 ID 를 준다.
	unsigned int	iMoney  ;			//돈
	
	BYTE	cElixirData[_MED_CHECK_SIZE];			//진국씨에게 통보 할 것.
	BYTE	cCollectionData[_COLLECTION_DATA_SIZE];//명품 데이터
#ifdef _PD_ADD_EVENT_COLLECTION_
	BYTE	cEventCollectionData[_COLLECTION_DATA_SIZE];//명품 데이터
#endif

	short	sVersion ;					// item version

	short	sBloodPoint ;				// 혈정 포인트

	_sServer_InvenItem_v1 * GetSlot( u_char slot )
	{
		_sServer_InvenItem_v1 * p = NULL;
		if( slot < _MAX_INVEN_SLOT_COUNT_ )
		{
#ifdef _PD_CASH_INVEN_
			if( slot < _CASH_INVEN_START_SLOT_ )
			{
				p = reinterpret_cast<_sServer_InvenItem_v1*>( &cInventory[slot*sizeof(_sServer_InvenItem_v1)] );
			}
			else if( slot < _TEMP_INVEN_START_SLOT_ )
			{
				p = reinterpret_cast<_sServer_InvenItem_v1*>( &cCashInven[(slot-_CASH_INVEN_START_SLOT_)*sizeof(_sServer_InvenItem_v1)] );
			}
			else
			{
				p = reinterpret_cast<_sServer_InvenItem_v1*>( &cInventory[(slot-_TEMP_INVEN_START_SLOT_+_CASH_INVEN_START_SLOT_)*sizeof(_sServer_InvenItem_v1)] );
			}				
#else
			p = reinterpret_cast<_sServer_InvenItem_v1*>( &cInventory[slot*sizeof(_sServer_InvenItem_v1)] );
#endif
		}
		return p;
	}
};

/*
#define _MAX_NIC_COUNT	41`
enum{
	_RANK_SHOWDOWN_NIC = 0,
	_RANK_PART_NIC,
	_RANK_LEVEL_NIC,
	_RANK_MARTIAL_NIC,
	_GEN_SHOWDOWN_NIC = 5,
	_GEN_QUEST_NIC,
	_GEN_PART_NIC,
	_GEN_MARTIAL_NIC,
	_USE_NIC = 10,
	_SELECTED_SLOT_NIC = 40,		// 40번 슬롯에는 선택한 슬롯 번호가..
} ;
*/




enum 
{
	_NIC_COUNT_NOTHING			= 1,
	_NIC_COUNT_SPECIAL_SHOWDOWN = 10,
	_NIC_COUNT_SPECIAL_MOOMPA	= 10,
	_NIC_COUNT_SPECIAL_LEVEL	= 10,
	_NIC_COUNT_SPECIAL_MARTIAL	= 50,
	_NIC_COUNT_GENERAL_SHOWDOWN	= 10,
	_NIC_COUNT_GENERAL_QUEST	= 50,
	_NIC_COUNT_GENERAL_MOONPA	= 1,
	_NIC_COUNT_GENERAL_MAKER	= 10,
	_NIC_COUNT_GENERAL_GATER	= 10,
	_NIC_COUNT_GENERAL_EVENT	= 20,
	_NIC_COUNT_GENERAL_MONSTER	= 30,
	_NIC_COUNT_TOTAL			= 212,
} ;

struct _CHARAC_NIC {

	short			sSelectNic ;		// 선택한 별호
	short			sUsedTime ;			// 사용되어진 시간

	union
	{		
		struct _NicData {
			u_short			usNothing[_NIC_COUNT_NOTHING] ;			// 0번 슬롯은 아무것도 선택하지 않았을때를 생각해 비워둔다.
			struct 						// 특수 별호
			{
				u_short		usShowdown[_NIC_COUNT_SPECIAL_SHOWDOWN] ;	// 특수비무별호
				u_short		usMoonpa[_NIC_COUNT_SPECIAL_MOOMPA] ;		// 특수문파별호
				u_short		usLevel[_NIC_COUNT_SPECIAL_LEVEL] ;		// 특수등급별호
				u_short		usMartial[_NIC_COUNT_SPECIAL_MARTIAL] ;		// 특수무공별호
			} _SpecialNic ;

			struct 						// 일반별호
			{
				u_short		usShowdown[_NIC_COUNT_GENERAL_SHOWDOWN] ;		// 비무별호
				u_short		usQuest[_NIC_COUNT_GENERAL_QUEST] ;		// 풍운록 별호
				u_short		usMoonpa[_NIC_COUNT_GENERAL_MOONPA] ;		// 문파 별호
				u_short		usMaker[_NIC_COUNT_GENERAL_MAKER] ;		// 제작별호
				u_short		usGater[_NIC_COUNT_GENERAL_GATER] ;		// 수집별칭
				u_short		usEvent[_NIC_COUNT_GENERAL_EVENT] ;		// 이벤트별호
				u_short		usMonster[_NIC_COUNT_GENERAL_MONSTER] ;
			} _GeneralNic ;
		} _Nic ;
		u_short				usNicID[_NIC_COUNT_TOTAL] ;
		BYTE				bitData[sizeof(_NicData)] ;
	} ;

};



struct _CHARAC_MATCH {
	int					iWinScore ;				// 승 회수.
	int					iLostScore ;			// 패배 회수
	int					iTieScore ;				// 무승부의 회수
	int					iSeriesCount ;			// 연승 또는 연패 : 위의 PrevMatchResult 에 따라 다른 의미.
	int					iMaxSeriesWinCount ;	// 최대 연승 기록.
	int					iMaxSeriesLoseCount ;	// 최대 연패 기록
	int					iAbuseCount ;			// 비정상 패배의 회수(누적)
	short				cPrevMatchResult ;		// 0 : 승, 1 : 패, 2 : 무
} ;



#define _MAX_CHARAC_BH_ENTRY_				32			// 위에 두개 합 : 저장 개수 때문에 
#define _MAX_CHARAC_EFFECT_ENTRY_			32			//
#define _MAX_EXTRA_EFFECT_ENTRY_			10
#define _MAX_CHARAC_SPECIAL_BUFF_			12
/* =====================================================================================
_sDB_BHEntry : 디비에 저장될 Buff/Hostile 정보 
_sUser_BHEntry : 유저에게 보낼 데이타.
===================================================================================== */
typedef struct 
{
	short			skillID ;			// 스킬의 아이디
	unsigned short	usLeftTimeSec ;		// 남은 시간
	char			skillLevel ;		// 스킬의 단계
} _sDB_BHEntry, _sUser_BHEntry ;


/* =====================================================================================
효과 요소 정보 : DB 또는 User에게 보낼 것.	
===================================================================================== */
typedef struct 
{
	short				id ;
	unsigned short		usLeftTimeSec ;		// 남은 시간 ( '초', ms 까지 정확할 필요 없다. )
	short				value ;				// 효과 요소는 단계가 없다. 그래서 값이 필요.
} _sDB_EffectEntry, _sUser_EffectEntry ;


struct _CHARAC_EFFECT
{
	union
	{			
		struct _EffectData {
			_sDB_BHEntry		BHEntry[_MAX_CHARAC_BH_ENTRY_] ;
			_sDB_EffectEntry	EffectEntry[_MAX_CHARAC_EFFECT_ENTRY_] ;
			_sDB_EffectEntry	ExtraEntry[_MAX_EXTRA_EFFECT_ENTRY_] ;
		} _data ;		
		BYTE					byData[sizeof(_EffectData)] ;
	};

#ifdef _PD_BUFF_COUNT_MODIFY_
	union
	{
		_sDB_BHEntry			Special[_MAX_CHARAC_SPECIAL_BUFF_];
		BYTE					bySpecial[sizeof(_sDB_BHEntry)*_MAX_CHARAC_SPECIAL_BUFF_];
	};
#endif	// _PD_BUFF_COUNT_MODIFY_
} ;


// 캐릭터의 아이템 사용 현황.
// 1. 외치기
// 2. 제련
struct _CHARAC_ITEM_USE_STATUS
{
	int		shout_left_time ;		// 둘다 0 이 아닐 때 의미가 있다. 
	short	shout_type ;
} ;


struct _sND_AIUS_Grade
{
	// 년/월/일/시/분/초. End Time
	u_int			uiYear		: 5 ;	// Base 2000. Year = 2000 + uiYear( 0-31 )
	u_int			uiMonth		: 4 ;	// Value 0 - 11 ( 0 - 15 )
	u_int			uiDay		: 6 ;	// Value 1 - 31 ( 0 - 63 )
	u_int			uiHour		: 5 ;	// Value 0 - 23 ( 0 - 31 ) 
	u_int			uiMinute	: 6 ;	// Value 0 - 59 ( 0 - 63 )
	u_int			uiSecond	: 6 ;	// Value 0 - 59 ( 0 - 63 )

	u_char			ucReserve ;		// 그냥 8 byte 맞추려고 넣음.
	char			cItemType ;		// 사용중인 아이템.
	short			sItemID ;		// 사용중인 아이템.
} ;

struct _ACCOUNT_ITEM_USE_STATUS
{
	_sND_AIUS_Grade		grade ;
	DWORD				uiEndTime ;		// 
	bool				bChanged ;		// 
} ;

#ifdef _VINA_SHUTDOWN_20061023_
struct _FATIGUE_ELEMENT_
{
	short				sFatigueWeight ; //피로도(단위 : 분) 
	int					iLogoutTime ;	//로그아웃시간( 단위 : Hour) // 단위가 Day->Hour로 2007.04.09일 수정되었음
#ifdef _PD_SDS_GOSTOP_
	u_char				ucFatigueMode;
#endif	// _PD_SDS_GOSTOP_
} ;
#endif	// _VINA_SHUTDOWN_20061023_


#ifdef _PD_CHARAC_LTS_

enum EN_LTS_DATA_COUNT
{
	en_lts_lvpk_data_count = 30,
	en_lts_pk_data_count = 10,
	en_lts_sp_data_count = 1,
	en_lts_pos_info_count = 11,
};


//위치추적의 기본 정보
struct _sLTS_Data
{
	int			iCharacUID;					// 캐릭터 유니크 아이디.
	int			iStartTrackingTime;			// 위치추적신청시간
	u_short		usReserve;
	u_char		ucTimeMode;					// 의뢰한 위치추적 내용 ( 1) 24시간, 2) 48시간 ) 
	char		szCharacName[en_charac_name_length+1];
	_sLTS_Data()
		: iStartTrackingTime(0), usReserve(0), ucTimeMode(0)
	{}
};

struct _CHARAC_LTS
{
	_sLTS_Data		m_pLvPkData[en_lts_lvpk_data_count];
	_sLTS_Data		m_pPkData[en_lts_pk_data_count];
	_sLTS_Data		m_pSpData[en_lts_sp_data_count];
};

#endif	// _PD_CHARAC_LTS_



#ifdef _PD_MASTER_PUPIL_

struct _CHARAC_MP
{
	u_char				ucMPPosition;		// EN_MP_RELATION
	u_char				ucRes;				// 그냥.
	u_short				usRes;				// 그냥.
	int					iMasterIndex;		// 스승 인덱스.
};

#endif	// _PD_MASTER_PUPIL_


struct _CHARAC_ETC {
	//	기타

	char				cState ;		//캐릭터상태(접속유무 기타등)	

	union{
		short					_sBACKUP_STATE_CHECKED ;	//0:백업완료 1:데이타 변동
		struct {
			bool				bBackupStateBasic : 1;	//0:백업완료 1:데이타 변동
			bool				bBackupStateLevel : 1;	//0:백업완료 1:데이타 변동
			bool				bBackupStateState : 1;	//0:백업완료 1:데이타 변동
			bool				bBackupStateCurBasicInfo : 1;	//0:백업완료 1:데이타 변동
			bool				bBackupStateInven : 1;	//0:백업완료 1:데이타 변동
			bool				bBackupStateQuest : 1;	//0:백업완료 1:데이타 변동
			bool				bBackupStateMartial : 1;	//0:백업완료 1:데이타 변동	
			bool				bBackupStateNic : 1 ;		//0:백업완료 1:데이타 변동

			bool				bBackupStateEffect : 1 ;
			bool				bBackupStateMatch : 1 ;
			bool				bBackupStatePkRecord : 1 ;
			bool				bBackupStateCharacItemUseStatus : 1 ;
			bool				bBackupState_4 : 1 ;
			bool				bBackupState_3 : 1 ;
			bool				bBackupState_2 : 1 ;
			bool				bBackupState_1 : 1 ;
		} _backup;		
	} ;	
};




/* =====================================================================================
PK 전적에 쓰인다.
===================================================================================== */

#define _PK_RECORD_WIN_COUNT_		10
#define _PK_RECORD_LOSE_COUNT_		10
#define _PK_RECORD_ABUSING_CHECK_	20

enum
{
	en_pk_option_none=0,		// 0 : No Meanings
	en_pk_option_win_normal,	// 1 : Win - Normal
	en_pk_option_win_mask,		// 2 : Win - Mask
	en_pk_option_lose_normal,	// 3 : Lose - Normal
	en_pk_option_lose_mask,		// 4 : Lose - Mask
};


struct __cntPkRecordEntry		// 전적 요소 : 상대방의 이름과, 날짜, 문파.
{
	union {
		struct {
			u_int		uiOption	: 3 ;	// 0 : None, 1 : win-Normal, 2 : Win-Mask, 3 : lose-normal,
			// 4 : lose-mask, 5 : Res, ...
			u_int		uiYear		: 5 ;	// 년도. 2006 + year
			u_int		uiMonth		: 6 ;	// 1 - 12 월
			u_int		uiDay		: 6 ;	// 1 - 31
			u_int		uiHour		: 6 ;	//
			u_int		uiMinute	: 6 ;	// 0 - 59
		} ;
		u_int			uiValue ;			// 
	} ;


	char		name[13] ;			// 복면인은 Option 2에 NULL 로 감. 

	char		cMoonpa ;			// 문파 정보.
} ;

struct __cntPkCount				// 한 문파, 혹은 한 직책에 대한 승/패 수.
{
	int			iWin ;
	int			iLose ;
} ;


struct __svrPkRecordEntry
{
	u_int		uiTime ;			// 서버에서는 GetGameTime의 리턴값으로 검사를 한다.
	int			iCharacUID ;		// DB의 캐릭터 아이디.
} ;


struct _CHARAC_PK_RECORD
{
	int				iKillCount ;
	int				iDeadCount ;

	BYTE			bWinRecord[sizeof(__cntPkRecordEntry)*_PK_RECORD_WIN_COUNT_] ;
	BYTE			bLoseRecord[sizeof(__cntPkRecordEntry)*_PK_RECORD_LOSE_COUNT_] ;		// 
	BYTE			bAbusingRecord[sizeof(__svrPkRecordEntry)*_PK_RECORD_ABUSING_CHECK_] ;

	BYTE			bMoonpaCount[sizeof(__cntPkCount)*_NUM_MOONPA_] ;
	BYTE			bClassCount[sizeof(__cntPkCount)*_NUM_CLASS_] ;
} ;

struct __client_quickslot
{
	u_char		slotType;		// _XQS_
	short 		skillID;
	short 		lifeSkillID;
	char		itemCType;
	short 		itemSID;
	u_short		invenNumber;	
	u_char		reserved;		// must be zero
	u_char		reserved2;		// must be zero
};	// 12 bytes

// 30개. : 360 bytes

#define _QUICKSLOT_COUNT_		(30)		//
#define _HELPLOG_MAX_SIZE_		(4*4)


struct _CHARAC_ETC_QUICKSLOT
{
	__client_quickslot		m_QuickSlot[_QUICKSLOT_COUNT_];
	u_char					m_helplog[_HELPLOG_MAX_SIZE_];
	bool					m_bChanged;
};

#ifdef _PD_PKEVENT_BOX_SPAWN_

struct _CHARAC_BW_WAR
{
	int				iWinTime;				// time() 함수의 리턴값.
};

#endif	// _PD_PKEVENT_BOX_SPAWN_

class _CHARAC {			/* 캐릭터 구조체에 대한 구조체	*/
public:
	_CHARAC_BASIC			CHARAC_BASIC;			//ND_V01_Charac
	_CHARAC_CUR_BASIC		CHARAC_CUR_BASIC;		//ND_V01_CurBasicInfo
	_CHARAC_LEVEL			CHARAC_LEVEL;			//ND_V01_5StarLevel
	_CHARAC_STATE			CHARAC_STATE;			//ND_V01_CharacState	
	_CHARAC_INVENTORY		CHARAC_INVENTORY;		//ND_V01_Inventory
	_CHARAC_QUEST_DATA		CHARAC_QUEST_DATA;		//ND_V01_User_Quest_Data
	_CHARAC_MARTIAL_DATA	CHARAC_MARTIAL_DATA;	//ND_V01_Charac_Martial_Data
	_CHARAC_NIC				CHARAC_NIC ;			//ND_V01_Charac_Nic
	_CHARAC_MATCH			CHARAC_MATCH ;			//ND_V01_Charac_Match
	_CHARAC_EFFECT			CHARAC_EFFECT ;
	_CHARAC_PK_RECORD		CHARAC_PK_RECORD ;
	_CHARAC_ITEM_USE_STATUS		CHARAC_ITEM_STATUS ;

#ifdef _PD_PKEVENT_BOX_SPAWN_
	_CHARAC_BW_WAR			CHARAC_BW_WAR;
#endif	// _PD_PKEVENT_BOX_SPAWN_

#ifdef _PD_MASTER_PUPIL_
	_CHARAC_MP				CHARAC_MP;
#endif	// _PD_MASTER_PUPIL_

	_CHARAC_ETC				CHARAC_ETC;				//기타정보	

	inline	void ResetCharac() { memset( this, 0, sizeof(*this) ) ; }
} ;

class _C_CHARAC : public _CHARAC
{
public:
	_CHARAC_ETC_QUICKSLOT		CHARAC_ETC_QUICKSLOT;

#ifdef _PD_CHARAC_LTS_
	_CHARAC_LTS					CHARAC_LTS;
#endif

public:
	CTraceAbleLock m_charac_cs;
#define charac_lock()		m_charac_cs.tlock( __FILE__, __LINE__ ) ;
#define charac_unlock()		m_charac_cs.tunlock( __FILE__, __LINE__ ) ;

	inline void Reset() 
	{
		ResetCharac();
		ZeroMemory( &CHARAC_ETC_QUICKSLOT, sizeof(CHARAC_ETC_QUICKSLOT) ) ;
#ifdef _PD_CHARAC_LTS_
		ZeroMemory( &CHARAC_LTS, sizeof(CHARAC_LTS) );
#endif

	}

	_C_CHARAC() 
	{
		Reset() ;
	}
	~_C_CHARAC() 
	{
		Reset() ;
	}

#ifdef _PD_CHARAC_LTS_
	bool lts_add_pk( _sLTS_Data& data );
	bool lts_del_pk( char * pDel );
	bool lts_add_sp( _sLTS_Data& data );
	bool lts_del_sp( char * pDel );
#endif

	
private:
	/*
	_C_CHARAC & _C_CHARAC::operator=( const _C_CHARAC & rhs )
	{
		try{
			if( this == &rhs ) { return *this ; }
			memcpy( this, &rhs, sizeof(_CHARAC) ) ;
		}
		catch(...){
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}
		return *this ;
	}

	_C_CHARAC & _C_CHARAC::operator=( const _CHARAC & rhs )
	{
		try{
			if( this == &rhs ) { return *this ; }
			memcpy( this, &rhs, sizeof(_CHARAC) ) ;
		}
		catch(...){
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}
		return *this ;
	}
	*/
	
	

};

#define PCHARAC _C_CHARAC*

//////////////////////////////////////////////////////
//	이곳에 정의되는 구조체들은 가상 메모리에 저장될	//
//	정보들에 대한 구조체다.. 그러니 패킷 구조체랑은	//
//	별개이다.. 여기 정의된 구조체를 단위로 가상메모	//
//	리에 저장이 된다..								//
//////////////////////////////////////////////////////

#define	_DEPOT_SLOT_NUM	40

#define  _DEPOT_BYTE_SIZE_		(sizeof(_sServer_InvenItem_v1) * _DEPOT_SLOT_NUM)
#define _EXT_DEPOT_BYTE_SIZE_	(sizeof(_sServer_InvenItem_v1) * 20)

struct _USER_DEPOT
{
	BYTE			cDepot[_DEPOT_BYTE_SIZE_];
#ifdef _PD_CASH_INVEN_
	BYTE			cExtendDepot[_EXT_DEPOT_BYTE_SIZE_];	// 창고 확장 20개.
#endif
	unsigned int	iMoney     ; // u_int 맞다. 
	char			cCheckCount; 
	short			sCheckYear ; 
	char			cCheckMonth;
	char			cCheckDay  ;
	char			cCheckHour ;

	short			sVersion ;

	_sServer_InvenItem_v1 * GetSlot( u_char slot )
	{
		_sServer_InvenItem_v1 * p = NULL;
		if( slot < _DEPOT_SLOT_NUM )
			p = reinterpret_cast<_sServer_InvenItem_v1*>( &cDepot[slot*sizeof(_sServer_InvenItem_v1)] );
#ifdef _PD_CASH_INVEN_
		else if( slot < (_DEPOT_SLOT_NUM+20) )
		{
			p = reinterpret_cast<_sServer_InvenItem_v1*>( &cExtendDepot[(slot-_DEPOT_SLOT_NUM)*sizeof(_sServer_InvenItem_v1)] );
		}
#endif
		return p;
	
	}

};

#ifdef ITEMSHOP_PATCH_200607
// 선물상자 구조체
#define PRESENT_BOX_SLOT_SIZE		25
#define PRESENT_MESSAGE_BUF_SIZE	100
struct PRESENT_BOX {
	u_short		usItem;			// 아이템 인덱스
	u_char		ucItemCnt;		// 아이템 개수
	u_short		usExpireDay;	// 유효일 수
	char		pszMsg[PRESENT_MESSAGE_BUF_SIZE];	// 선물 메시지
};
#endif

#ifdef	_NXPCB_AWARD_200703_
struct NEXON_PCB_INFO {
	UCHAR	ucResult;
	UCHAR	ucAddrDesc;
	UCHAR	ucAccDesc;
	UCHAR	ucOption;
	int		iArgument;
	bool	bIsPCBBillType;
};
#endif

class _USER {							//	USER 계정에 대한 구조체..
public:
	int					iUid;					//	유니크 아이디	
	char				cNDAccount[en_max_lil+1] ;		//	구룡 계정 : 이건 Define 안걸고 고쳐도 된다. 
#ifdef _NXPCB_AWARD_200703_
	char				cNXAcc[en_max_lil+1];			//	넥슨 계정
#endif
	char				cPassword[en_max_lpl+1] ;			//	패스워드	
	u_short				usTempUniqueID;			//	LS 가 부여한 임시 유니크아이디
	u_int				iIp ;					//  USER 의 IP Add
	int					iTime ;					//  USER 의 접속 접근 시간
	DWORD				dwLoginTime ;			//  로그인한시간(분단위)
	int					iServer;				//	라스트 서버	
	char				cCharacCount;			//	캐릭터의 갯수
	char				cSelChrName[13];		//	선택된 캐릭터 이름
	char				cState;					//  상태	
	_USER_DEPOT			stUserDepot;			//	유저 고창
	_C_CHARAC			stCharac[3];			//	캐릭터 3개를 배열로..
	unsigned			cDepotBackupState : 1 ;	//	창고의 백업 상태
	unsigned			bMemState : 1 ;			// 메모리가 사용중인지..
	unsigned			bCheckCurEvent : 1;		// 0 - not checked, 1 - checked. 20080204
	unsigned			padding : 5 ;			// 차후에 쓸꺼..
	
	char				cClass ;				// 접속자 등급 18: 관계사, 19 : 내부인원

	_ACCOUNT_ITEM_USE_STATUS	AIUS ;			// 

#ifdef ITEMSHOP_PATCH_200607
	//PRESENT_BOX			PresentBox[PRESENT_BOX_SLOT_SIZE];
#endif

#ifdef _METEL_LOGIN_200608_
	BOOL			bPCBang;			// 피씨방 유저인지 아닌지(TRUE = 피씨방 유저)
	BOOL			bBillingLogin;		// 빌링 로그인 상태(TRUE = BillingLogin() 호출되고, BillingLogout() 아직 호출되지 않은 상태)
										// BillingLogin() 중복 호출을 방지하기 위한 변수
	METEL_BILLING_INFO	MetelBillingInfo;
#endif //_METEL_LOGIN_200608_

#ifdef _VINA_SHUTDOWN_20061023_
	_FATIGUE_ELEMENT_  FatigueElement ;
	#ifdef _PD_VINA_PCROOM_INFO_
	u_char				ucPCRoomInfo;	// 0 - Home, else PC Room.
	#endif
#endif

#ifdef	_NXPCB_AWARD_200703_
	NEXON_PCB_INFO	NexonPcbInfo;
#endif

#ifdef _PD_EVENT_INVITE_FRIEND_
	int				iIF_DBIndex;
	int				iIF_EndTime;
#endif

	inline void Reset() 
	{ 

		iUid = 0 ;							//	유니크 아이디	
		memset( cNDAccount, 0, en_max_lil+1 ) ;			//	계정
		memset( cPassword, 0, 13 ) ;		//	패스워드	
		usTempUniqueID = 0 ;				//	LS 가 부여한 임시 유니크아이디
		iIp = 0  ;							//  USER 의 IP Add
		iTime = 0 ;							//  USER 의 접속 접근 시간
		dwLoginTime = 0 ;					//  로그인한시간
		iServer = 0 ;						//	라스트 서버	
		cCharacCount = 0 ;					//	캐릭터의 갯수
		memset( cSelChrName, 0, 13 );		//	선택된 캐릭터 이름
		cState = 0 ;						//  상태	
		memset( &stUserDepot, 0, sizeof(_USER_DEPOT) );			//	유저 창고
		
		
		cDepotBackupState = 0 ;				//	창고의 백업 상태
		bMemState = 0 ;						// 메모리가 사용중인지..
		padding = 0 ;						// 차후에 쓸꺼..
		
		ZeroMemory( &AIUS, sizeof(_ACCOUNT_ITEM_USE_STATUS) ) ;
        
		for( int i = 0 ; i < 3 ; i++ )
		{
			stCharac[i].Reset() ;
		}

#ifdef _METEL_LOGIN_200608_
		bPCBang = FALSE;
		bBillingLogin = FALSE;
		memset(&MetelBillingInfo, 0, sizeof(METEL_BILLING_INFO));
		MetelBillingInfo.iLoginSN = -1;
#endif //_METEL_LOGIN_200608_

#ifdef	_NXPCB_AWARD_200703_
		memset( cNXAcc, 0, en_max_lil+1 ) ;			//	계정
		memset(&NexonPcbInfo, 0, sizeof(NEXON_PCB_INFO));
#endif
#ifdef _VINA_SHUTDOWN_20061023_
		memset( &FatigueElement, 0, sizeof(FatigueElement) );
#endif
#ifdef _PD_EVENT_INVITE_FRIEND_
		iIF_DBIndex = 0;
		iIF_EndTime = 0;
#endif
	}	
	
} ;


class CUser : public _USER
{

private :

public :
	CTraceAbleLock m_user_cs;

#define user_lock()			m_user_cs.tlock( __FILE__, __LINE__ ) ;
#define user_unlock()		m_user_cs.tunlock( __FILE__, __LINE__ ) ;

	static int	get_time_by_sec() { return GetTime_ByIntSec() ; } ;


	SOCKADDR_IN		m_myLsAddr ;

	CUser() 
	{
        Reset() ;
	}
	~CUser() 
	{
		Reset() ;
	}

	inline	_C_CHARAC * characFind( char * cCharacName ) 
	{
		if( cCharacName == NULL || cCharacName[0] == 0 )
		{
			return NULL ;
		}
		_C_CHARAC * pCharac = NULL ;

		user_lock() ;
		try{
			for( int i = 0 ; i < 3 ; i++ )
			{
				if( strcmp( stCharac[i].CHARAC_BASIC.cChrName, cCharacName ) == 0 )
				{
					pCharac = &stCharac[i] ;
					break ;
				}			
			}
		}
		catch(...){
			::PrintConsole( "[EXCEPTION] %s %d \n", __FILE__, __LINE__) ;
		}		
		user_unlock() ;

		return pCharac ;
	}

} ;

#define PUSER CUser*


// 아래 구조체 하나로 모든 장착 정보를 얻데이트 할 수 있도록, 장착정보를 모두 포함한다. 
// 캐릭터 리스트에 들어갈 구조체
struct _CharacList								// # [내부] 캐릭터의 기본 정보 #
{
	int					iUniqueID;				// 캐릭터의 유니크 아이디
	char				cName[13] ;				// 캐릭터 의 이름
	char				cNick[13] ;				// 캐릭터 별칭

	char				cZone ;

	char				cSex ;					// 성별 0 : 남자, 1 : 여자
	char				cFace ;					// 얼굴형 : max 16종 
	char				cHair ;					// 머리 스타일 : max 16종 
	char				cGroup ;				// 소속 문파

	char				cAmorType ;					// 상의	0
	char				cPantsType ;				// 하의	1
	char				cCapType ;					// 모자 4
	char				cAdaeType ;					// 아대 2
	char				cShoesType ;				// 신발 3	
	char				cBackPackType ;				// 배낭 7
	char                cWeaponType ;               // 무기 타입
	char				cMaskType ;				// 복면.

	short				sAmor ;					// 상의	0
	short				sPants ;				// 하의	1
	short				sCap ;					// 모자 4
	short				sAdae ;					// 아대 2
	short				sShoes ;				// 신발 3	
	short				sBackPack ;				// 배낭 7	
	short				sSelectWeaponID ;		// 선택한 무기 인덱스. 0, 1 ( 각각 무기1, 2 )
	short				sMask ;					//
	u_char				ucWeaponStrength ;		// 현재 무기의 제련 정도.

	short				sStr ;					// 근력 <= 원랙는 정기
	short				sZen ;					// 진기 <= 그대로
	short				sIntelligence ;			// 지혜 <= 영기
	short				sConstitution ;			// 건강 <= 생기
	short				sDexterity ;			// 민첩 <= 심기

	short				sInnerLevel ;			// 내공 수위 

	// 2004/11/10 Ray 추가. class, classgrade
	char				cClass ;				// 역할 
	char				cClassGrade ;			// 역할 등급
	char				cGM ;					// GM 여부..  (1 이상 GM,  -1이하 :블럭킹)

	// 2005/01/07 Ray 추가 : 선택창에서 캐릭터의 위치 표시.
	float				fPosX ;					// 최종 위치.
	float				fPosZ ;					//

	int					iBlockingEndDate ;		// 블럭킹 만료 시간

	u_char				ucChangeNameFlag : 2 ;	// 0 - Not, 1 - Possible Change Name
	u_char				ucLeftFlag : 6 ;		// 아직 안쓰임.

} ;

// 인벤토리

// 기존은 0 : 2005-10-13일 현재.
// 인벤토리의 최종 버전은 1이다.
#define _VERSION_INFO_INVEN_LAST_		1
#define _V0_COUNT_INVEN_ITEM_			103		// 기존의 슬롯 개수.
#define _INVENTORY_MAX_COUNT_           95		// v0에서의 인벤토리 개수. 이 값은 템프인벤슬롯의 시작번호와 같다.
#define _TEMP_INVEN_MAXNUM_             8       // 합쳐서 103
#define _V1_COUNT_INVEN_ITEM_			108		// 25(장착) + 75(인벤토리) + 8(임시)


#define _ITEM_TYPE_COUNT_			20 // 대분류 종류 10가지.


#define _ITEM_TYPE_WEAPON_			0  // 무기 
#define _ITEM_TYPE_CLOTH_			1  // 의상
#define _ITEM_TYPE_BOOK_			2  // 비급
#define _ITEM_TYPE_POTION_			3  // 회복류	   // 사용에 해당함. 
#define _ITEM_TYPE_CONSUMING_		4  //

#define _ITEM_TYPE_ELIXIR_          5

#define _ITEM_TYPE_COUNTERACT_		6  // 해독 주문서 // 사용에 해당함. 
#define _ITEM_TYPE_RESOURCE_		7  // 자원
#define _ITEM_TYPE_QUEST_			8  // 퀘스트용 아이템 
#define _ITEM_TYPE_ACCESSORY_		9  // 악세사리:귀걸이/반지/목걸이/배낭
#define _ITEM_TYPE_LIFE_			10 // 생활 도구
#define _ITEM_TYPE_MUSIC_			11 // 악기
#define _ITEM_TYPE_PASS_			12 // 통행증
#define _ITEM_TYPE_MONEY_			13 // 돈(금)
#define _ITEM_TYPE_SOCKET_          14 // 소켓 삽입용 메터리얼....
#define _ITEM_TYPE_WEAPON2_			15 // 무기 확장판
#define _ITEM_TYPE_CLOTH2_			16 // 옷   확장판
#define _ITEM_TYPE_BOX_             17
#define _ITEM_TYPE_BOXKEY_          18 
#define _ITEM_TYPE_WEAPON3_			19



enum equip_pos			// 아이템 장착 위치 인덱스.
{
	pos_amor=0,         // 0  장착될 경우 Pocket 공간을 봐야한다. 
	pos_pants,			// 1  장착될 경우 Pocket 공간을 봐야한다. 
	pos_adae,			// 2
	pos_shoes,			// 3
	pos_cap,			// 4
	pos_gap_upper,		// 5
	pos_gap_lower,		// 6
	pos_backpack,		// 7
	pos_weapon_1,		// 8
	pos_weapon_2,		// 9

	pos_accessory_1,	// 10 위의 0,1번 Pocket공간에 따라서 위에서부터 활성화 되어야한다.  상의용...
	pos_accessory_2,	// 11
	pos_accessory_3,	// 12
	pos_accessory_4,	// 13

	pos_accessory_5,	// 14  하의용.
	pos_accessory_6,	// 15
	pos_accessory_7,	// 16
	pos_accessory_8,	// 17
	pos_backpack2,      // 18 백팩 두번째.
	pos_reserved        // 19 예약공간.  지금은 쓰지 않는다. 
} ;


// v1 인벤토리는 장착이 25 입니다.
enum v1_equip_pos
{
	v1_pos_amor=0,                            // 0  장착될 경우 Pocket 공간을 봐야한다. 
	v1_pos_pants,                               // 1  장착될 경우 Pocket 공간을 봐야한다. 
	v1_pos_adae,                               // 2
	v1_pos_shoes,                             // 3
	v1_pos_cap,                               // 4
	v1_pos_gap_upper,                      // 5
	v1_pos_gap_lower,                       // 6

	v1_pos_mask,                               // 7  마스크
	v1_pos_cloth_reserve,     // 8  의상 예약 분.
	v1_pos_cloth_reserve2,   // 9  의상 예약 2.

	v1_pos_weapon_1,                       // 10
	v1_pos_weapon_2,                       // 11

	v1_pos_amor_acc_1,                   // 12 위의 0,1번 Pocket공간에 따라서 위에서부터 활성화 되어야한다.  상의용...
	v1_pos_amor_acc_2,                    // 13
	v1_pos_amor_acc_3,                    // 14
	v1_pos_amor_acc_4,                    // 15

	v1_pos_pants_acc_1,                   // 16 하의용 악세사리.
	v1_pos_pants_acc_2,                   // 17
	v1_pos_pants_acc_3,                   // 18
	v1_pos_pants_acc_4,                   // 19

	v1_pos_extra_pocket,      // 20 소지품 장착 공간.
	v1_pos_reserve1,                          // 21
	v1_pos_reserve2,                          // 22

	v1_pos_backpack1,                       // 23
	v1_pos_backpack2                        // 24 백팩 두번째.
} ;


struct _sUser_InvenItem						// 유저의 인벤토리에 들어갈 아이템 항목
{	
	char				cType   ;		// 아이템 대분류
	char				cSecond ;
	short				sID     ;		// 아이템 소분류. (아이템 아이디)
	char				cIdentify     : 4;	// 감정 상태
	u_char              ucSocketCount : 4;      // 소켓이 몇개가 열렸는지.
	u_char				ucCount  ;		// 최대 머지 255개
	u_short				usCurDur ;		// 현재 내구력
	u_char              ucSocket[4];        	// 소켓 1,2,3,4
} ;	






inline void ConvertToLastVersion( OUT short & sInvenVersion, OUT _sServer_InvenItem_v1 * pInvenV1 , OUT _sServer_InvenItem * pInven)
{
	if( sInvenVersion == 0 )
	{
		::ZeroMemory( pInvenV1, sizeof(_sServer_InvenItem_v1)* _V1_COUNT_INVEN_ITEM_ ) ;

		int i ;

		// 장착 부분 변환.
		for( i = 0 ; i < pos_reserved + 1 ; ++i )
		{
			if( pInven[i].sID )
			{
				switch( i )
				{
				case pos_amor:			// 0  장착될 경우 Pocket 공간을 봐야한다. 
					pInvenV1[v1_pos_amor].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_pants:			// 1  장착될 경우 Pocket 공간을 봐야한다. 
					pInvenV1[v1_pos_pants].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_adae:			// 2
					pInvenV1[v1_pos_adae].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_shoes:			// 3
					pInvenV1[v1_pos_shoes].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_cap:			// 4
					pInvenV1[v1_pos_cap].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_gap_upper:		// 5
					pInvenV1[v1_pos_gap_upper].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_gap_lower:		// 6
					pInvenV1[v1_pos_gap_lower].convert_from_v0( &pInven[i] ) ;
					break ;

				case pos_backpack:		// 7
					pInvenV1[v1_pos_backpack1].convert_from_v0( &pInven[i] ) ;
					break ;


				case pos_weapon_1:		// 8
					pInvenV1[v1_pos_weapon_1].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_weapon_2:		// 9
					pInvenV1[v1_pos_weapon_2].convert_from_v0( &pInven[i] ) ;
					break ;


				case pos_accessory_1:	// 10 위의 0,1번 Pocket공간에 따라서 위에서부터 활성화 되어야한다.  상의용...
					pInvenV1[v1_pos_amor_acc_1].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_accessory_2:	// 11
					pInvenV1[v1_pos_amor_acc_2].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_accessory_3:	// 12
					pInvenV1[v1_pos_amor_acc_3].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_accessory_4:	// 13
					pInvenV1[v1_pos_amor_acc_4].convert_from_v0( &pInven[i] ) ;
					break ;


				case pos_accessory_5:	// 14  하의용.
					pInvenV1[v1_pos_pants_acc_1].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_accessory_6:	// 15
					pInvenV1[v1_pos_pants_acc_2].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_accessory_7:	// 16
					pInvenV1[v1_pos_pants_acc_3].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_accessory_8:	// 17
					pInvenV1[v1_pos_pants_acc_4].convert_from_v0( &pInven[i] ) ;
					break ;


				case pos_backpack2:		// 18 백팩 두번째.
					pInvenV1[v1_pos_backpack2].convert_from_v0( &pInven[i] ) ;
					break ;
				}
			}
		}


		int j; 
		for( i = pos_reserved + 1, j = 1 ; i < _V0_COUNT_INVEN_ITEM_ ; ++i, ++j )
		{
			if( pInven[i].sID )
			{
				pInvenV1[j+v1_pos_backpack2].convert_from_v0( &pInven[i] ) ;
			}
		}

		sInvenVersion = 1 ;
	}
}





/*

// 유니크 아이디 만들때 호출하는 함수임. 파싱시에 구조를 이해하는데 도움이 될 것 같아서 첨부함
extern volatile LONG lUniqueItemID;
extern volatile u_short usItemUniqueIDHour;

unsigned __int64 CItemManager::SM_Item_Make_UniqueID( )
{
unsigned __int64 ui64Temp = 0L;
SYSTEMTIME Date;
GetSystemTime(&Date);
u_char  * pcTemp = NULL;
u_short * psTemp = NULL;

pcTemp    = reinterpret_cast<u_char*>( &ui64Temp );
pcTemp[0] = static_cast<u_char>( g_sServerNo );
pcTemp[1] = static_cast<u_char>( Date.wYear - 2000 );
pcTemp[2] = static_cast<u_char>( Date.wMonth );
pcTemp[3] = static_cast<u_char>( Date.wDay );
pcTemp[4] = static_cast<u_char>( Date.wHour );
pcTemp[5] = static_cast<u_char>( Date.wMinute );

psTemp    = reinterpret_cast<u_short*>( &pcTemp[6] );
*psTemp   = static_cast<u_short>( lUniqueItemID ); 

if(	usItemUniqueIDHour != Date.wHour ){ lUniqueItemID = 0L; usItemUniqueIDHour = Date.wHour;}
else{ InterlockedIncrement( &lUniqueItemID ); }

return ui64Temp;
}
*/



struct _sCharac_Skill
{
	short	id ;
	char	cStep ;			    // 무공 성장 단계

	u_char	ucSkillState : 2 ;	// 0 : 전수 완료  1 : 미완료 : 수련시에만 사용가능. 2, 3,...
	u_char	ucPassiveState : 1 ;// 0 : Off, 1 : On for passive only
	u_char	ucCurMission : 5 ;	// 미션 진행 카운트	0 - 31
	int		iCurSkillExp ;		// 지금 누적된 경험치

	u_char	ucPassiveSelectStep ;	// 
	u_char	ucTemp ;
};


//#ifdef NEW_QUEST_PACKET
/* ============================================================================
퀘스트 정보 
============================================================================ */
struct _sCharacQuestState
{
	bool	bReword  : 1 ;	// 보상을 받았는가
	u_char	ucNodeID : 7 ;	// 현 Node 아이디 언사인드 캐릭터로 해야한다.
	u_short usQuestID;      // 현 협객, 기연, 무림 퀘스트.

	_sCharacQuestState()
	{
		bReword   = false;
		ucNodeID  = 0;
		usQuestID = 0;
	}
} ;

/* ============================================================================
메인 퀘스트 저장과 관련된 구조체. 
============================================================================ */
struct _sCharacMainQuestState
{
	bool	bReword  : 1 ;	// 보상을 받았는가
	u_char	ucNodeID : 7 ;	// 현 Node 아이디
	u_short usQuestID;      // 현 제룡행 퀘스트 아이디. 워드로 하는 것이 낫지 않을까?
	u_char  padding;        // 여분의 공간.

	_sCharacMainQuestState()
	{
		padding   = 0;      // 여분의 저장 플레그를 위한 공간임.
		usQuestID = 0;
		ucNodeID  = 0;
		bReword   = false;
	}
} ;
//==========================================
// 퀘스트 아이템 구조체 (속성 無)
//==========================================
struct _sQuestItem
{
	short		sID ;			// 아이템 소분류. (아이템 아이디)	
	u_int		uiCount ;		// 돈이나 화폐류/투척류의 수량.
	_sQuestItem() : sID(0), uiCount(0)
	{}
} ;

/*struct _sQuestItem_Pair
{
char cType;
union {
struct {
char		cSecond ;		// 아이템 중분류
char		cIdentify ;		// 0,1,2,3 까지만.
short		sID ;			// 아이템 소분류. (아이템 아이디)  ** 안보인다면.. 필요 없음.
} S_detail ;
u_int			uiCount ;
} ;
u_char				ucCount ;		// 개수
char                padding[4];
};*/ 

//===========================================
// 퀘스트 관련 구조체.
//===========================================
#define QUEST_HISTORY_COUNT 50 

struct _sQuestHistory
{
	char cNodeHistory;	 // -1
	char cBranchHistory;	// 0
};

struct _sRunningQuest
{
	u_short usQuestID;

	char  cNodeID;
	int   dwQuestStartTime;
	int	  dwQuestTimer;
	char  cTimerNodeNo;
	bool  bBeUse;

	char   cTFRetry      : 4 ;     /* 재 보상을 하려 할때 true액션 retry인지 false액션 retry인지 */ 

	bool   bCounter1Flag : 1;      /* 카운터가 보상물을 세고 있는 것인가? 아니면 그냥 일반 카운터인가  필요 없을 수도...*/ 
	bool   bCounter2Flag : 1;
	bool   bCounter3Flag : 1;
	bool   bCounter4Flag : 1;


	u_char ucSelectNo;		       /* 최대 선택가능한 지역 넘버는 255개 */ 
	u_char ucCounter[4];           /* 갯수 카운터. */ 
	short  sMobID[4];              /* 카운터플레그에 따라서 몹아이디인지 혹은 아이템 아이디인지가 바뀐다 */ 

	_sRunningQuest()
	{
		usQuestID        = 0;
		dwQuestTimer     = 0;
		dwQuestStartTime = 0;
		cTFRetry         = 0;	   /*재 보상일 경우 그 전에 fail 액션을 실행한 건지, true 액션을 실행한 건지.*/ 

		bCounter1Flag    = false;  /*한 bit씩을 카운터 플레그를 나타내는데 사용한다.*/ 
		bCounter1Flag    = false; 
		bCounter1Flag    = false; 
		bCounter1Flag    = false; 

		cNodeID          = 0;  
		cTimerNodeNo     = 0;
		bBeUse           = false; 
		ucCounter[0]     = 0;      /* 루프보다 이게 낫다 */ 
		ucCounter[1]     = 0;
		ucCounter[2]     = 0;
		ucCounter[3]     = 0;
		sMobID[0]        = 0;
		sMobID[1]        = 0;
		sMobID[2]        = 0;
		sMobID[3]        = 0;
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




/* =====================================================================================
서버 정보 관련..
===================================================================================== */

// 서버 정보 구조체(GLOBAL USE)

struct  _SERVER_SYSTEM_INFO{						// 서버 정보		
#ifdef _GMS_SERVERNAME_LENGTH_INCREASED_
	char		cServerName[57];
#else
	char		cServerName[13];
#endif
	char		cOS[16];					// 사용 OS		
	char		cCPU;						// CPU 사용률
	int			iTotalMem;					// 총 메모리
	int			iUseMem;					// 사용중인 메모리
	int			iTotalHDD;					// 총 디스크 용량
	int			iUseHDD;					// 사용중인 디스크 용량	

};

class _h_DS_Com ;
struct _SERVER_NET_INFO {
	_h_DS_Com *	ds_com;
	char		cIP[16];					// 사용 IP
	short		sPort;						// UDP 포트다.
	SOCKADDR_IN	sockAddr ;
};

struct _SERVER_NET_INFO_GMS {	
	char		cIP[16];					// 사용 IP
	short		sPort;						// UDP 포트다.	
};

struct _SERVER_STATE_INFO {
	char		cState;
	int			iConnectCount;
	int			iPlayingCount;

	int			iLevelExp;
	int			iSkillExp;

};


struct _POS_LIST {
	float	x;
	float	z;
	char	cName[16];
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// \class _sCombatRecord
/// \brief 영웅단의 장원의 참전기록 - 장원은 총 4개 예정 
class _sCombatRecord
{
public:	
	_sCombatRecord(){InitializeCombatRecord();}
	~_sCombatRecord(){};

public:	
	inline	unsigned short	GetManorIndex(){return m_sManorIndex;}
	inline	void	SetManorIndex(short index){m_sManorIndex = index;}
	inline	unsigned short	GetEnrtyCount(){return m_sEntryCount;}
	inline	void	SetEnrtyCount(short entryCount){m_sEntryCount = entryCount;}
	inline	unsigned short	GetWinCount()	{return m_usWinCount;}
	inline	void	SetWinCount(unsigned short winCount){m_usWinCount = winCount;}
	inline	unsigned short	GetLoseCount()	{return m_usLoseCount;}
	inline	void	SetLoseCount(unsigned short loseCount){m_usLoseCount = loseCount;}
	inline	unsigned short	GetCombatCount()	{return m_usCombatCount;}
	inline	void	SetCombatCount(unsigned short combatCount){m_usCombatCount = combatCount;}
	inline	_sND_TimeUnit		GetCombatDate()	{return m_combat_date;}
	inline	void	SetCombatDate(_sND_TimeUnit* date)
	{
		memcpy(&m_combat_date,date,sizeof(_sND_TimeUnit));
	}


	bool	InitializeCombatRecord()
	{
		m_sManorIndex	= 0;
		m_sEntryCount	= 0;
		m_usWinCount	= 0;
		m_usLoseCount	= 0;
		m_usCombatCount	= 0;
		return true;
	}

private:	
	short				m_sManorIndex;	///<	장원 인덱스
	short				m_sEntryCount;	///<	장원전 참가 횟수 
	unsigned short		m_usWinCount;	///<	장원전 별  승수
	unsigned short		m_usLoseCount;	///<	장원전 별  패
	unsigned short		m_usCombatCount;///<	장원전 별  전적 
	_sND_TimeUnit		m_combat_date;	///<	장원전 최후 날짜 
};





// 방회 최대 인원수 :200명 
const	unsigned short MAX_PERSONAL_COUNT		= 256;
// 총20개(자신 빼고) 방회라고 하고 최대 5*21 = 105개
const	unsigned short MAX_ORG_COUNT			= 128;
// 장원 갯수 4개 예정 
const	unsigned short MAX_MANOR				= 2;
// 장원전 공성,수성 성공단 대전점수별 순위권 추가보상 10위까지 지급
// 등수는 각 _sPersonalRecord에 갖고 있으므로 10개만 랭킹 체크 
const	unsigned short	MANOR_RANK_ANNOUNCE		= 10;
// 장원전에 참가한 단의 총 단의 갯수 `07.04.04 현재 의천에 총 영웅단 수가 998개 
const   unsigned short  MAX_ORG_COUNT_INSERVER	= 256;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// \class	_sOrgRecord
/// \brief	대상 영웅단과의 승/패 기록 
class _sOrgRecord
{
public:
	_sOrgRecord()
	{
		m_orgIndex = 0;
		InitializeOrzRecord();
	}
	~_sOrgRecord(){}
public:
	inline	unsigned short	GetIndex()	{return m_orgIndex;}
	inline	void	SetIndex(int index){m_orgIndex = index;}
	inline	unsigned short	GetAllyIndex()	{return m_AllyIndex;}
	inline	void	SetAllyIndex(int index){m_AllyIndex = index;}
	inline	unsigned short	GetWinCount()	{return m_usWinCount;}
	inline	void	SetWinCount(unsigned short winCount){m_usWinCount = winCount;}
	inline	unsigned short	GetLoseCount()	{return m_usLoseCount;}
	inline	void	SetLoseCount(unsigned short loseCount){m_usLoseCount = loseCount;}
	inline	unsigned short	GetCombatCount()	{return m_usCombatCount;}
	inline	void	SetCombatCount(unsigned short combatCount){m_usCombatCount = combatCount;}
    inline	u_char*	GetVsOrgName()	{return m_vsOrgName;}
	inline	void	SetVsOrgName(u_char* name)
	{
		memcpy(m_vsOrgName,name,24);
		m_vsOrgName[24]=0;
	}

	bool _sOrgRecord::InitializeOrzRecord()
	{
		m_orgIndex		= 0;
		m_usWinCount	= 0; 
		m_usLoseCount	= 0;
		m_usCombatCount	= 0;	
		return true;
	}	


	bool _sOrgRecord::AddRecord(const bool bResult)
	{
		if(bResult) //true
		{
			++m_usWinCount;
			++m_usCombatCount;
		}
		else 
		{
			++m_usLoseCount;
			++m_usCombatCount;
		}
		return true;
	}

	_sOrgRecord&  operator =(const _sOrgRecord& rhs)
	{
		m_orgIndex		= rhs.m_orgIndex;
		SetVsOrgName( const_cast<u_char*>(rhs.m_vsOrgName) );
		m_usWinCount	= rhs.m_usWinCount;
		m_usLoseCount	= rhs.m_usLoseCount;
		m_usCombatCount = rhs.m_usCombatCount;
		m_AllyIndex		= rhs.m_AllyIndex;
		return *this;
	}

private:
	///<	타 영웅단(회) 인덱스
	int					m_orgIndex;	
	///<	타 단 의 이름
	u_char				m_vsOrgName[25];
	int					m_AllyIndex;		//타단 인덱스 0이면 단이고 0이 아니면 회의 인덱스이다
	///<	대 영웅단별 승수
	unsigned short		m_usWinCount;
	///<	대 영웅단별 패
	unsigned short		m_usLoseCount;
	///<	대 영웅단별 전적 
	unsigned short		m_usCombatCount;	
};

struct _effect
{
	short		id ;				// effect table(EffectScript.txt 참조)에 정의되어 있는 id
	short		prob ;				// 상태효과가 발동될 확률(skill table 참조)
	short		value ;				// 상태효과가 발동될 때 적용될 값(skill table 참조)
	u_short		effect_prop ;		// % 위의 enum 값. 스킬 테이블에서 읽힐때 복사됨.
	//////////////////////////////////////////////////////////////////////////
	// Property
	// none = 없음. 이것으로 설정하면, 처리되지 않음.
	// general 		= 일반 속성
	// initial_once		= 처음에 한번 적용되고 없어짐
	// apply_additive	= 시전시 대상에게 적용
	// bonus_additive	= 기존의 Additive : 공격시에 보너스로 감.
	// reflection		= 방어시마다 되돌림
	// only_once		= 단 한번 : 성공하면 사라짐
	// feed			= 공격시 From 에서 돌려받음

	char		value_type ;		// %
	//////////////////////////////////////////////////////////////////////////
	// Value Type : 적용값 계산에 사용.
	// add_value		= 값으로 더함
	// add_percent		= %로 더함
	// sub_value		= 값으로 빼기
	// sub_percent		= %로 빼기

	u_char		where ;				// % 어디에 적용...
	u_char		from ;				// % 어디에서 받을 것인가.
	//////////////////////////////////////////////////////////////////////////
	// From Target Type : 'feed', 'trans' 속성에서만 적용, none 은 쓰지 않음. 
	// none  기본 속성 : 이건 쓰면 안됨. 스크립트 에러로 처리함.
	// hit,			준 데미지에서 
	// life,		상대방의 체력에서
	// force,		상대방의 내력에서
	// concentration	상대방의 의지에서

	u_char		condition_flag_id ;	// %
	// EN_CONDITION_ID 참조

	u_short		count ;				// apply_additive가 적용될 시간

	char		bInterval ;			// 간격을 가진 것인가. 0/1 : 대부분 0, 중독 등만 1
	// true일 경우, 3초마다 적용됨

	char		cPadding ;			// 패딩.


	_effect *	pNext ;		// 생성 시킬때 NULL 로. 
	// 하나의 스킬/스펠은 여러 개의 effect로 구성되어 있을 수 있으므로,
	// 각 effect들이 리스트로 구성된다. 마지막 effect는 NULL 값을 갖게 됨.

	_effect()
	{
		::ZeroMemory( this, sizeof(_effect) ) ;
	}
} ;

#pragma pack( pop, enter_global_struct )