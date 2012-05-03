#ifndef _XGAME_ITEM_DEFINE_
#define _XGAME_ITEM_DEFINE_

#define _XDEF_MAXCOUNT_WEAPONITEMSEED		379
#define _XDEF_MAXCOUNT_WEAPONITEM			29115
#define _XDEF_MAXCOUNT_CLOTHESITEM			28899
#define _XDEF_MAXCOUNT_QUESTITEMSEED		32
#define _XDEF_MAXCOUNT_ETCITEMSEED			32
#define _XDEF_MAXCOUNT_RECOVERYITEMSEED		44


#include "d3dx9.h"
#include "XKernel.h"

#define _XDEF_ITEMTYPE_COUNT				21

// =======================================================
// 아이템 대분류
//
// 0 : 무기
// 1 : 의상
// 2 : 비급
// 3 : 회복류
// 4 : 투척류
// 5 : 중독 주문서
// 6 : 해독 주문서
// 7 : 자원
// 8 : 퀘스트 아이템
// 9 : 악세사리 (귀걸이/반지/목걸이/배낭)
// 10 : 생활 도구
// 11 : 악기
// 12 : 통행증
// 13 : 돈(금)
// 14 : 소켓
// 15 : 무기2
// 16 : 의상2
// 17 : 패왕궤
// 18 : 패왕궤 열쇠
// 19 : 무기3
// 20 : 의상3
//
// ========================================================
typedef enum _XGI_FirstCategory
{
	_XGI_FC_WEAPON = 0,			// 무기 = 0
	_XGI_FC_CLOTHES,			// 의상 = 1
	_XGI_FC_BOOK,				// 비급 = 2
	_XGI_FC_POTION,				// 회복 = 3
	_XGI_FC_CONSUMING,			// 투척 = 4
	_XGI_FC_ELIXIR,				// 영약 = 5
	_XGI_FC_COUNTERACT,			// 해독 주문서 = 6
	_XGI_FC_RESOURCE,			// 자원 = 7
	_XGI_FC_QUEST,				// 퀘스트 = 8
	_XGI_FC_ACCESSORY,			// 악세사리 = 9
	_XGI_FC_LIFE,				// 생활 도구 = 10
	_XGI_FC_MUSIC,				// 악기 = 11
	_XGI_FC_PASS,				// 통행중 = 12
	_XGI_FC_MONEY,				// 돈 = 13
	_XGI_FC_SOCKET,				// 소켓 = 14
	_XGI_FC_WEAPON2,			// 무기2 = 15
	_XGI_FC_CLOTHES2,			// 의상2 = 16
	_XGI_FC_BOX,				// 패왕궤 = 17
	_XGI_FC_BOXKEY,				// 패왕궤 열쇠 = 18
	_XGI_FC_WEAPON3,			// 무기3 = 19
	_XGI_FC_CLOTHES3,			// 의상3 = 20
};


// ========================================================
// 무기 아이템 중분류 : 소분류
//
// 0 - 단병기 : 0-검법  1-도법  2-타구봉  3-타격  4-권각  5-암타  6-부법
// 1 - 장병기 : 0-대도  1-창술  2-곤술
// 2 - 연병기 : 0-편술  1-추술  2-사술
// 3 - 암병기 : 0-표창  1-비도  2-투척  3-점혈  4-발사  5-소모(폭탄)
// 4 - 악병기 : 0-령  1-고  2-소  3-금  4-적
// 5 - 특수무기 : 0-륜
//
// =========================================================
typedef enum _XGI_SecondCategory_Weapon		// 무기 중분류
{
	_XGI_SC_WEAPON_SHORT = 0,			// 단병기 = 0
	_XGI_SC_WEAPON_LONG,				// 장병기 = 1
	_XGI_SC_WEAPON_SOFT,				// 연병기 = 2
	_XGI_SC_WEAPON_HIDDEN,				// 암병기 = 3
	_XGI_SC_WEAPON_MUSICAL,				// 악병기 = 4
	_XGI_SC_WEAPON_SPECIAL				// 특수무기 = 5
};

typedef enum _XGI_ThirdCategory_Weapon_Short	// 단병기 소분류
{
	_XGI_TC_WEAPON_SWORD = 0,			// 검법 = 0
	_XGI_TC_WEAPON_KNIFE,				// 도법 = 1
	_XGI_TC_WEAPON_CLUB,				// 타구봉 = 2
	_XGI_TC_WEAPON_HIT,					// 타격 = 3
	_XGI_TC_WEAPON_FIST,				// 권각 = 3
	_XGI_TC_WEAPON_SECRET,				// 암타 = 5
	_XGI_TC_WEAPON_AXE,					// 부법 = 6
	_XGI_TC_WEAPON_RING					// 륜 = 7
};

typedef enum _XGI_ThirdCategory_Weapon_Long		// 장병기 소분류
{
	_XGI_TC_WEAPON_GLAIVE = 0,			// 대도 = 0
	_XGI_TC_WEAPON_SPEAR,				// 창술 = 1
	_XGI_TC_WEAPON_STAFF,				// 곤술 = 2
};

typedef enum _XGI_ThirdCategory_Weapon_Soft
{
	_XGI_TC_WEAPON_STEELWHIP = 0,		// 편술 = 0
	_XGI_TC_WEAPON_HAMMER,				// 추술 = 1
	_XGI_TC_WEAPON_CHAIN				// 사술 = 2
};

typedef enum _XGI_ThirdCategory_Weapon_Hidden
{
	_XGI_TC_WEAPON_DART = 0,			// 표창 = 0
	_XGI_TC_WEAPON_DAGGER,				// 비도 = 1
	_XGI_TC_WEAPON_JAVELIN,				// 투척 = 2
	_XGI_TC_WEAPON_NEEDLE,				// 점혈 = 3
	_XGI_TC_WEAPON_ARROWLAUNCHER,		// 발사 = 4
	_XGI_TC_WEAPON_MINE					// 소모(폭탄) = 5
};

typedef enum _XGI_ThirdCategory_Weapon_Musical
{
	_XGI_TC_WEAPON_RYUNG = 0,			// 령 = 0
	_XGI_TC_WEAPON_GO,					// 고 = 1
	_XGI_TC_WEAPON_SO,					// 소 = 2
	_XGI_TC_WEAPON_KUEM,				// 금 = 3
	_XGI_TC_WEAPON_JUCK					// 적 = 4
};

typedef enum _XGI_ThirdCategory_Weapon_Special
{
	_XGI_TC_WEAPON_SERING = 0				// 륜 = 0
};


// ===========================================================
// 의상 아이템 중분류
//
// 0 : 상의
// 1 : 하의
// 2 : 아대
// 3 : 신발
// 4 : 모자
// 5 : 흉갑
// 6 : 신갑
// 7 : 배낭
//
// ===========================================================
typedef enum _XGI_SecondCategory_Clothes
{
	_XGI_SC_CLOTHES_UPPER = 0,		// 상의 = 0
	_XGI_SC_CLOTHES_LOWER,			// 하의 = 1
	_XGI_SC_CLOTHES_GLOVES,			// 아대(장갑) = 2
	_XGI_SC_CLOTHES_SHOES,			// 신발 = 3
	_XGI_SC_CLOTHES_CAP,			// 모자 = 4
	_XGI_SC_CLOTHES_CLOAK,			// 흉갑 = 5
	_XGI_SC_CLOTHES_ARMOR,			// 신갑 = 6
	_XGI_SC_CLOTHES_BACKPACK,		// 배낭 = 7
	_XGI_SC_CLOTHES_MASK			// 치장 = 8
};


// ============================================================
// 비급 아이템 중분류
//
// 0 : 무공비급
// 1 : 무공서
// 2 : 생활비급
// 3 : 미정
// 4 : 일반 책
// 5 : 두루마리
// 6 : 무공비급2(중복 스킬도 읽을 수 있음)
//
// ============================================================
typedef enum _XGI_SecondCategory_Book
{
	_XGI_SC_BOOK_SKILL = 0,		// 무공비급 = 0		
	_XGI_SC_BOOK_STUDY,			// 무공서	= 1
	_XGI_SC_BOOK_ABILITY,		// 생활비급 = 2
	_XGI_SC_BOOK_SCROLL,		// 두루마리	= 3
	_XGI_SC_BOOK_STUFF,			// 일반 책	= 4
	_XGI_SC_BOOK_PAPER,			// 종이		= 5
	_XGI_SC_BOOK_SKILL2,		// 무공비급 = 6
};

typedef enum _XGI_ThirdCategory_Book_Help
{
	_XGI_TC_BOOK_APPOINTMENT = 0,	// 지정무공서 = 0
	_XGI_TC_BOOK_GROUP,				// 문파무공서 = 1
	_XGI_TC_BOOK_DUTIES,			// 직책무공서 = 2
	_XGI_TC_BOOK_SERIES,			// 계열무공서 = 3
};
// =============================================================
// 회복 아이템 중분류
//
// 0 : 내상 회복제
// 1 : 외상 회복제
// 2 : 피로 회복제
//
// =============================================================
typedef enum _XGI_SecondCategory_Potion
{
	_XGI_SC_POTION_HERT_IN = 0,		// 0. 내상 치료제
	_XGI_SC_POTION_HERT_OUT,		// 1. 외상 치료제
	_XGI_SC_POTION_FATIGUE,			// 2. 피로 회복제
	_XGI_SC_POTION_NOTFIXED,		// 3. 없음
	_XGI_SC_POTION_FOOD,			// 4. 음식
	_XGI_SC_POTION_EVENT,			// 5. 신약
	_XGI_SC_POTION_HWATA,			// 6. 화타신단
	_XGI_SC_POTION_MASK,			// 7. 복면
	_XGI_SC_POTION_ANTI,			// 8. 해독제 
	_XGI_SC_POTION_ADDEXP,			// 9. 경험치/숙련도 증가
	_XGI_SC_POTION_RESETPOINT,		// 10. 오기/오의 초기화
	_XGI_SC_POTION_NICKNAME,		// 11. 별호 변경
	_XGI_SC_POTION_INCHANTRATE,		// 12. 제련 아이템 증발 방지
	_XGI_SC_POTION_SHOUT,			// 13. 외치기
	_XGI_SC_POTION_RECALL,			// 14. 소환령/호출령 //Author : 양희왕 추가
	_XGI_SC_POTION_FIREWORK,		// 15. 폭죽류
	_XGI_SC_POTION_NOTMERGE,		// 16. merge 되지 않는 포션류
	_XGI_SC_POTION_COOLDOWN,		// 17. 쿨다운 시간 따로 사용하는 포션류
	_XGI_SC_POTION_EVENT2			// 18. 신약 아이콘 모자른다고해서 추가 //Author : 양희왕
};


// =============================================================
//	투척 아이템 중분류
// 
// 0 : 화살
// 1 : 비살
// 2 : 탄
//
// =============================================================
typedef enum _XGI_SecondCategory_Throw
{
	_XGI_SC_THROW_ARROW = 0,		// 화살 = 0
	_XGI_SC_THROW_THROW,			// 비살 = 1
	_XGI_SC_THROW_BULLET			// 탄 = 2
};


// =============================================================
// 중독 아이템 중분류
// 
// 0 : 풍
// 1 : 뇌 
// 2 : 독
// 3 : 빙
// 4 : 토
// 5 : 화
// 6 : 수
// 7 : 마
// 8 : 정
// 9 : 청
// 10 : 목
// 11 : 금
// 12 : 보
// 13 : 패
// 14 : 명
//
// ===============================================================
typedef enum _XGI_SecondCategory_Poison
{
	_XGI_SC_POISON_PUNG = 0,		// 풍 = 0
	_XGI_SC_POISON_NUE,				// 뇌 = 1
	_XGI_SC_POISON_DOCK,			// 독 = 2
	_XGI_SC_POISON_BING,			// 빙 = 3
	_XGI_SC_POISON_TO,				// 토 = 4
	_XGI_SC_POISON_HWA,				// 화 = 5
	_XGI_SC_POISON_SU,				// 수 = 6
	_XGI_SC_POISON_MA,				// 마 = 7
	_XGI_SC_POISON_JUNG,			// 정 = 8
	_XGI_SC_POISON_CHUNG,			// 청 = 9
	_XGI_SC_POISON_MOCK,			// 목 = 10
	_XGI_SC_POISON_KEUM,			// 금 = 11
	_XGI_SC_POISON_BO,				// 보 = 12
	_XGI_SC_POISON_PAE,				// 패 = 13
	_XGI_SC_POISON_MYUNG			// 명 = 14
};

// ===============================================================
// 영약 아이템 중분류
typedef enum _XGI_SecondCategory_Elixir
{
	_XGI_SC_ELIXIR_SUPPORT = 0,		// 보조영약 = 0
	_XGI_SC_ELIXIR_STRENGTH,		// 근력영약 = 1
	_XGI_SC_ELIXIR_INNER,			// 진기영약 = 2
	_XGI_SC_ELIXIR_HEART,			// 지혜영약 = 3
	_XGI_SC_ELIXIR_HEALTH,			// 건강영약 = 4
	_XGI_SC_ELIXIR_DEX				// 민첩영약 = 5	
};

// ===============================================================
// 해독 아이템 중분류
//
// 0 : 풍
// 1 : 뇌 
// 2 : 독
// 3 : 빙
// 4 : 토
// 5 : 화
// 6 : 수
// 7 : 마
// 8 : 정
// 9 : 청
// 10 : 목
// 11 : 금
// 12 : 보
// 13 : 패
// 14 : 명
//
// ===============================================================
typedef enum _XGI_SecondCategory_Counteract
{
	_XGI_SC_COUNTERACT_PUNG = 0,	// 풍 = 0
	_XGI_SC_COUNTERACT_NUE,			// 뇌 = 1
	_XGI_SC_COUNTERACT_DOCK,		// 독 = 2
	_XGI_SC_COUNTERACT_BING,		// 빙 = 3
	_XGI_SC_COUNTERACT_TO,			// 토 = 4
	_XGI_SC_COUNTERACT_HWA,			// 화 = 5
	_XGI_SC_COUNTERACT_SU,			// 수 = 6
	_XGI_SC_COUNTERACT_MA,			// 마 = 7
	_XGI_SC_COUNTERACT_JUNG,		// 정 = 8
	_XGI_SC_COUNTERACT_CHUNG,		// 청 = 9
	_XGI_SC_COUNTERACT_MOCK,		// 목 = 10
	_XGI_SC_COUNTERACT_KEUM,		// 금 = 11
	_XGI_SC_COUNTERACT_BO,			// 보 = 12
	_XGI_SC_COUNTERACT_PAE,			// 패 = 13
	_XGI_SC_COUNTERACT_MYUNG		// 명 = 14
};


// ================================================================
// 자원 아이템 중분류
//
// 0 : 생산
// 1 : 채집
// 2 : 선물
// 3 : 표국 이용권
//
// ================================================================
typedef enum _XGI_SecondCategory_Resource
{
	_XGI_SC_RESOURCE_PRODUCTION = 0,	// 생산 = 0
	_XGI_SC_RESOURCE_COLLECTION,		// 채집 = 1
	_XGI_SC_RESOURCE_PRESENT,			// 선물 = 2
	_XGI_SC_RESOURCE_MOVE				// 표국 이용권 = 3
};


// ================================================================
// 퀘스트 아이템 중분류
//
// ================================================================
typedef enum _XGI_SecondCategory_Quest
{
};



// ================================================================
// 악세사리 중분류
//
// 0 : 반지
// 1 : 부적
// 2 : 배낭
// 3 : 목걸이
// 4 : 귀걸이
//
// ================================================================
typedef enum _XGI_SecondCategory_Accessory
{
	_XGI_SC_ACCESSORY_RING = 0,			// 반지 = 0
	_XGI_SC_ACCESSORY_CHARM,			// 부적 = 1
	_XGI_SC_ACCESSORY_BACKPACK,			// 배낭 = 2
	_XGI_SC_ACCESSORY_NECKLACE,			// 목걸이 = 3
	_XGI_SC_ACCESSORY_EARRING,			// 귀걸이 = 4

	_XGI_SC_ACCESSORY_STRENGTH = 11,	// 근력증가 = 11
	_XGI_SC_ACCESSORY_CONSTITUTION,		// 건강증가 = 12
	_XGI_SC_ACCESSORY_ZEN,				// 진기증가 = 13
	_XGI_SC_ACCESSORY_INT,				// 지혜증가 = 14
	_XGI_SC_ACCESSORY_DEX,				// 민접증가 = 15

	_XGI_SC_ACCESSORY_ARMOR_MULTI = 50,	// 상갑, 하갑 중복가능
	_XGI_SC_ACCESSORY_ARMOR				// 상갑, 하갑 중복불가
};

// ================================================================
// 생활 중분류
//
// 0 : 장식도구
// 1 : 투척보조류
//
// ================================================================
typedef enum _XGI_SecondCategory_Life
{
	_XGI_SC_LIFE_SOCKET = 0,
	_XGI_SC_LIFE_CONSUMING,
	_XGI_SC_LIFE_INCHANTSTUFF,
	_XGI_SC_LIFE_SOCKETSSTUFF,
	_XGI_SC_LIFE_SMLETITEM_SCALES , //Author : 양희왕
	_XGI_SC_LIFE_SMLETITEM_TEAR,//Author : 양희왕
	_XGI_SC_LIFE_MIX_SOCKET_NORMAL,		// 조합아이템 : 소켓(일반)
	_XGI_SC_LIFE_MIX_SOCKET_PERIOD,		// 조합아이템 : 소켓(기간제)
	_XGI_SC_LIFE_MIX_PENALTY			// 조합아이템 : 패널티 감소
};

// ================================================================
// 생활 도구 아이템
// 악기 아이템
// 통행증 아이템
// 돈 
//
// 중분류 없음
//
// ================================================================


///////////////////////////////////////////////////
// Item table structure


// ------------------------------------------------
// 아이템 공통

#define ITEM_NAME_LENGTH		(33)

typedef struct XGI_Item_Base
{
	char			cType ;				// 대분류
	char			cSecondType ;		// 중분류
	short			sID ;				// 아이디
	short			sModelIndex ;		// 클라이언트 모델 인덱스.
	
	short			sIcon_No  ;         // 아이콘 넘버.
	unsigned short	usItem_Rank;        // 아이템 등급.
#ifdef _XTS_ITEM_V20
	TCHAR*			cItemInfo;			// 아이템 인포메이션 인덱스
#else
	TCHAR			cItemInfo[57] ;     // 아이템 인포메이션.
#endif


	char			cApplyClan;
	short			sClanPoint1;		// 무게
	short			sClanPoint2;
	
	unsigned int	uiPrice ;			// 가격

#ifdef _XTS_ITEM_V20
	TCHAR*		 	cName;				// 서버도 이름을 갖자.
#else
	TCHAR			cName[ITEM_NAME_LENGTH] ;			// 서버도 이름을 갖자.
#endif

	unsigned char	ucGrade;			// 등급
	unsigned char	ucQuality;			// 재료
	unsigned char	ucQuality2;			// new

	char	cDump;				//버리기 여부
	char	cUserTrade;			//노점을 사용한 버리기 여부
	char	cNPCTrade;			//NPC판매, 문파기여 여부

	unsigned char	ucFame;		// 악명 
	
#ifdef _XTS_ITEM_V21
	char			cCashCheck;			// 0:일반아이템  1:일반캐쉬아이템  2:캐쉬기간제아이템  3:일반기간제아이템
#else
	unsigned short	usTempField2;
#endif
	
#ifdef _XDEF_PERIODITEM_20061027
	unsigned short	usTimeValue;
#else
	short	sTempField3;
#endif
	short	sTempField4;
	short	sTempField5;

#ifdef _XTS_ITEM_V20
	XGI_Item_Base()
	{
		cItemInfo = NULL;
		cName = NULL;
	}
#else
	XGI_Item_Base()
	{
		memset( cItemInfo, 0, sizeof(TCHAR) * 57 );
		memset( cName, 0, sizeof(TCHAR) * ITEM_NAME_LENGTH );
	}
#endif
}_XGI_Item_Base;


// ------------------------------------------------
// 무기 관련 
typedef enum _XGI_WeaponProp_material
{
	_XGI_WP_MATERIAL_FIST = 0,
	_XGI_WP_MATERIAL_WOOD,
	_XGI_WP_MATERIAL_METAL	
};

typedef enum _XGI_WeaponProp_soundmaterial
{
	_XGI_WP_SOUNDMATERIAL_FIST = 0,
	_XGI_WP_SOUNDMATERIAL_LIGHTWOOD,
	_XGI_WP_SOUNDMATERIAL_HEAVYWOOD,
	_XGI_WP_SOUNDMATERIAL_LIGHTMETAL,	
	_XGI_WP_SOUNDMATERIAL_MEDIUMMETAL,
	_XGI_WP_SOUNDMATERIAL_HEAYVYMETAL
};

typedef enum _XGI_WeaponProp_weightclass
{
	_XGI_WP_WEIGHTCLASS_LIGHT = 0,
	_XGI_WP_WEIGHTCLASS_MIDIUM,
	_XGI_WP_WEIGHTCLASS_HEAVY	
};

typedef struct XGI_WeaponItem_Property : public _XGI_Item_Base
{
	// 대분류 : 무기 0
	// 
	// 속성 :
#ifdef _XTS_ITEM_V20
	TCHAR*			cNick;
	unsigned short	usAddto;
#else
	char			cNick[ITEM_NAME_LENGTH];
#endif


	char			cThirdType ;
	short			sMinPower ;			// 최소 공격력
	short			sMaxPower ;			// 최대 공격력
	unsigned short	usDurability ;		// 최대 내구
	char			cBalance ;			// 조화 ; 최대, 최소 공격력의 평균을 정함.
	char			cAttackRate;
#ifdef _XTS_ITEM_V21
	short			sCriticalRate;
#else
	char			cCriticalRate;
#endif
	char			cSpeed;
	char			cStrength ;			// 무기의 강도
	char			cRange ;			// 공격 거리
	char			cNature ;			// 특화 속성
	short			sReqLevel ;			// 요구 내공 수위
	short			sReqSim ;			// 요구 민첩
	short			sReqJung ;			// 요구 근력
	char			cRank ;				// 제련 등급 : 아이템이 제련될 수 있는 최대 등급
	short			sHidden_ID ;		// 무림기보 ID
	char			cHidden_Value ;		// 무림기보가 될 확률
	unsigned char	ucMaxSocket;
	char			cRepairProb ;		// 수리 확률 : 수리성공일 경우, 일정치까지만 오른다.
	//short			sModel_Type;
	short			sG_Group;			//Gamble용
	short			sG_Case;			//Gamble용
	//char			cQuality ;			// 재질 정보 : 사운드에 사용된다. 클라이언트만.

	D3DXVECTOR3		vecItemPosition[4];	
	
	unsigned short	usAdd_SEID[5];
	unsigned short	usAdd_Value[5];
	unsigned char	ucAdd_Prob[5];
	unsigned char	ucAP_Type[5];
	unsigned short	usAP_Per[5];

	//SPECIAL	modified by changkhan	2004.10.13
	unsigned short	usSpecial_ID_1;
	unsigned short	usSpecial_Time_1;
	unsigned char	ucSpecial_Value_1;
	unsigned char	ucSApply_Type_1;
	unsigned short	usSpecial_Prob_1;

	unsigned short	usSpecial_ID_2;
	unsigned short	usSpecial_Time_2;
	unsigned char	ucSpecial_Value_2;
	unsigned char	ucSApply_Type_2;
	unsigned short	usSpecial_Prob_2;

	unsigned short	usSpecial_ID_3;
	unsigned short	usSpecial_Time_3;
	unsigned char	ucSpecial_Value_3;
	unsigned char	ucSApply_Type_3;
	unsigned short	usSpecial_Prob_3;

	unsigned short	usSpecial_ID_4;
	unsigned short	usSpecial_Time_4;
	unsigned char	ucSpecial_Value_4;
	unsigned char	ucSApply_Type_4;
	unsigned short	usSpecial_Prob_4;

	short			sGroup; //Author : 양희왕 //breif : 08/01/29 sAccord 를 변경.. 세트아이템으로 사용
	
#ifdef _XDEF_ITEM_V24
	unsigned char	ucSocketCheck;
#endif

#ifdef _XTS_ITEM_V20
	XGI_WeaponItem_Property()
	{
		cNick = NULL;
		usAddto = 0;
	}
#else
	XGI_WeaponItem_Property()
	{
		memset( cNick, 0, sizeof(char) * ITEM_NAME_LENGTH );
	}
#endif

}_XGI_WeaponItem_Property;


// ------------------------------------------------
// 의상 관련
typedef struct XGI_ClothesItem_Property : public _XGI_Item_Base
{
	// 문파는 의상의 카테고리 3으로 인식한다.
#ifdef _XTS_ITEM_V20
	TCHAR*			cNick;
	unsigned short	usAddto;
#else
	char			cNick[ITEM_NAME_LENGTH];
#endif

	short			sG_Group;			// Gamble용 
	short			sG_Case;			// Gamble용 

	char			cPrecedence ;		// 우선 순위: 의상과 방어구의 우선 순위
	char			cClan ;				// 요구 - 문파 특성 : 
#ifdef _XDEF_ITEM_V25
	unsigned char	ucCharacGrade;		// 2차 전직 전용 의상 표시
#endif
	char			cSexType ;			// 요구 - 성별
	//char			cAccord ;			// 조화 : 의상의 조화
	unsigned char	ucGroup;			//Author : 양희왕 //breif : 08/01/29 cAccord 를 변경.. 세트아이템으로 사용

	// 속성
	char			cClass ;			// 종류(계급) : 의상의 종류 혹은 계급
	char			cRepairProb ;		// 수리 확률
	//char			cQuality ;			// 재질 정보
	char			cRank ;				// 제련 등급:제련될 수 있는 최대 등급

	float			fCenter_X ;			// 의상의 중심점 : 클라이언트 데이타.
	float			fCenter_Y ;			//
	float			fCenter_Z ;

	short			sHidden_ID ;		// 무림기보 ID
	char			cHidden_Value ;		// 무림기보가 될 확률

	// 기능
	short			sDefence ;			// 방어력
	short			sStamina ;			// 정력
	char			cWarm ;				// 보온 효과

	char			cCool_Resistance ;	// 음
	char			cHot_Resistance ;	// 양
	char			cSpirit_Resistance;	// 혼
	char			cBody_Resistance ;	// 신
	
	unsigned short	usDurability ;		// 최대 내구
	char			cStrength ;			// 의상의 강도

	unsigned short	usAdd_SEID[5];
	unsigned short	usAdd_Value[5];
	unsigned char	ucAdd_Prob[5];
	unsigned char	ucAP_Type[5];
	unsigned short	usAP_Per[5];

	//
	char			cPocket;
	
	// 요구
	short			sReqLevel ;		// 요구 내공 수위
	short			sReqSim ;		// 요구 심기
	short			sReqJung ;		// 정기

	//SPECIAL	modified by changkhan	2004.10.13
	unsigned short	usSpecial_ID_1;
	unsigned short	usSpecial_Time_1;
	unsigned char	ucSpecial_Value_1;
	unsigned char	ucSApply_Type_1;
	unsigned short	usSpecial_Prob_1;

	unsigned short	usSpecial_ID_2;
	unsigned short	usSpecial_Time_2;
	unsigned char	ucSpecial_Value_2;
	unsigned char	ucSApply_Type_2;
	unsigned short	usSpecial_Prob_2;

	unsigned short	usSpecial_ID_3;
	unsigned short	usSpecial_Time_3;
	unsigned char	ucSpecial_Value_3;
	unsigned char	ucSApply_Type_3;
	unsigned short	usSpecial_Prob_3;

	unsigned short	usSpecial_ID_4;
	unsigned short	usSpecial_Time_4;
	unsigned char	ucSpecial_Value_4;
	unsigned char	ucSApply_Type_4;
	unsigned short	usSpecial_Prob_4;

	//socket
	unsigned char	ucMaxSocket;
	char			cAttackRate;
	char			cEvasionRate;

#ifdef _XDEF_ITEM_V24
	unsigned char	ucSocketCheck;
#endif

#ifdef _XTS_ITEM_V20
	XGI_ClothesItem_Property()
	{
		usAddto = 0;
		cNick = NULL;
	}
#else
	XGI_ClothesItem_Property()
	{
		memset( cNick, 0, sizeof(char) * ITEM_NAME_LENGTH );
	}
#endif

}_XGI_ClothesItem_Property;


// ------------------------------------------------
// 비급 관련
typedef struct XGI_BookItem_Property : public _XGI_Item_Base
{
	char			cThirdType;			
	unsigned short	usSkillID;
	unsigned char	ucValue;
	unsigned char	ucSkillGroup;
	unsigned char	ucSkillClass;
	char			cSkillStep;

	unsigned char	ucSkillLevel;
	unsigned short	usAbilityID;
	unsigned short	usHiddenID;
	unsigned short	usQuestID;

	unsigned char	ucClan;
#ifdef _XDEF_ITEM_V25
	unsigned char	ucCharacGrade;		// 2차 전직자만 읽을 수 있도록
#endif
	unsigned char	ucClass;

	unsigned short	usReqSkill1;
	char			cReqSkill1Step;
	unsigned short	usReqSkill2;
	char			cReqSkill2Step;
	unsigned short	usReqSkill3;
	char			cReqSkill3Step;
	unsigned short	usReqAbility;
	char			cReqAbilityStep;

	char			cProb;
	unsigned short	usDelay;
	unsigned char	ucDelete;
	unsigned int	uiApplyTime;

	unsigned short	usCoolDown;
	unsigned char	ucApplyClan;

	unsigned short	usReqLevel;
	unsigned short	usReq_STR;
	unsigned short	usReq_Force;
	unsigned short	usReq_Intell;
	unsigned short	usReq_Const;
	unsigned short	usReq_Dex;

	unsigned short	usLife;
	unsigned short	usForce;
	unsigned short	usConcent;
	
}_XGI_BookItem_Property;


// -------------------------------------------------
// 회복 관련
typedef struct XGI_PotionItem_Property : public _XGI_Item_Base
{
	char			cClan;					// 요구 : 문파 (0이면 다쓴다)
	
	short			sApplyValue;			// 적용 타입에 따라 수치 또는 비율
	char			cApplyType;				// 0 : 수치  1 : 비율
	
	// 2004.06.16->oneway48 insert 
	short           sEffectId    ;
	short           sInCEffectId ;   
	short           sVisualEfId  ;
	// insert end
	
	short			sCooldown;				// 사용 후 딜레이
	
	// 2004.06.16->oneway48 insert 
	short           sReqLevel    ;
	short           sSimgi ;        
	short           sJungGi;
	// insert end
}_XGI_PotionItem_Property;


// -------------------------------------------------
// 투척류 
typedef struct XGI_ConsumingItem_Property : public _XGI_Item_Base
{
#ifdef _XTS_ITEM_V20
	TCHAR*			cNick;
#else
	char			cNick[ITEM_NAME_LENGTH];
#endif
	
	char			cThird_Type;
	char			cClan;
	char			cSex_Type;
	char			cClass;

	unsigned short	usSEID;
	unsigned short	usTime;	

	char			cSpeed;
	char			cStrength;
	char			cRange;

	// 추가 
	unsigned short	usAdd_SEID_1 ;			// 효과 번호
	unsigned short	usAdd_Value_1;
	unsigned char	ucAdd_Prob_1;
	unsigned char	ucAP_Type_1;
	unsigned short	usAP_Per_1;

	unsigned short	usAdd_SEID_2 ;			// 효과 번호
	unsigned short	usAdd_Value_2;
	unsigned char	ucAdd_Prob_2;
	unsigned char	ucAP_Type_2;
	unsigned short	usAP_Per_2;

	unsigned short	usAdd_SEID_3 ;			// 효과 번호
	unsigned short	usAdd_Value_3;
	unsigned char	ucAdd_Prob_3;
	unsigned char	ucAP_Type_3;
	unsigned short	usAP_Per_3;

	unsigned short	usAdd_SEID_4 ;			// 효과 번호
	unsigned short	usAdd_Value_4;
	unsigned char	ucAdd_Prob_4;
	unsigned char	ucAP_Type_4;
	unsigned short	usAP_Per_4;

	unsigned short	usAdd_SEID_5 ;			// 효과 번호
	unsigned short	usAdd_Value_5;
	unsigned char	ucAdd_Prob_5;
	unsigned char	ucAP_Type_5;
	unsigned short	usAP_Per_5;

	short			sReq_Level ;	// 요구 : 내공 수위
	short			sReq_Jungki ;		// 요구 : 정기 수치
	short			sReq_Simki ;		// 요구 : 심기 수치 

#ifdef _XTS_ITEM_V20
	XGI_ConsumingItem_Property()
	{
		cNick = 0;
	}
#else
	XGI_ConsumingItem_Property()
	{
		memset( cNick, 0, sizeof(char) * ITEM_NAME_LENGTH );
	}
#endif
}_XGI_ConsumingItem_Property;


// --------------------------------------------------
// 중독류
typedef struct XGI_PoisonItem_Property : public _XGI_Item_Base
{
	char			cTarget;				// 대상
	FLOAT			fAttackRange;			// 공격 범위

	short			sDamageMax;				// 최대 공격력
	short			sDamageMin;				// 최소 공격력

	short			sEffectID;				// 효과 번호
	short			sEffectTime;			// 효과 지속 시간 : 초
	short			sEffectValue;			// 효과 수치
	short			sEffectProb;			// 효과 발동 확률

	FLOAT			fEffectRange;			// 타켓 포인트 중심으로 효과가 적용되는 범위

	short			sReqLevel;				// 요구 레벨
	short			sReqYoung;				// 요구 영기
	short			sReqJung;				// 요구 정기
}_XGI_PoisonItem_Property;


// --------------------------------------------------
// 해독류
typedef struct XGI_CounteractItem_Property : public _XGI_Item_Base
{
	char			cTarget;				// 대상
	FLOAT			fAttackRange;			// 공격 범위

	short			sDamageMax;				// 최대 공격력
	short			sDamageMin;				// 최소 공격력

	short			sEffectID;				// 효과 번호
	short			sEffectTime;			// 효과 지속 시간 : 초
	short			sEffectValue;			// 효과 수치
	short			sEffectProb;			// 효과 발동 확률

	FLOAT			fEffectRange;			// 타겟 포인트를 중심으로 효과가 적용되는 범위

	short			sReqLevel;				// 요구 레벨
	short			sReqYoung;				// 요구 영기
	short			sReqJung;				// 요구 정기
}_XGI_CounteractItem_Property;


// -------------------------------------------------
// 자원류
typedef struct XGI_ResourceItem_Property : public _XGI_Item_Base
{
}_XGI_ResourceItem_Property;


// -------------------------------------------------
// 퀘스트
typedef struct XGI_QuestItem_Property : public _XGI_Item_Base
{
	short   sQuest_Id; // 2004.06.16->oneway48 insert
	unsigned short	usSEID;
	unsigned short	usValue;
	unsigned short	usDelay;
	short			sEffectID;
}_XGI_QuestItem_Property;


// -------------------------------------------------
// 악세사리
typedef struct XGI_AccessoryItem_Property : public _XGI_Item_Base
{
	short			sG_Group;			//Gamble용
	short			sG_Case;			//Gamble용

	char			cPack ;				// 배낭의 경우에만, 수용 용량
	char			cClan;
	char			cRank ;
	unsigned short	usDurability ;		// 최대 내구

	unsigned short	usAdd_SEID[5];
	unsigned short	usAdd_Value[5];
	unsigned char	ucAdd_Prob[5];
	unsigned char	ucAP_Type[5];
	unsigned short	usAP_Per[5];

	// 요구
	short			sReqLevel ;		// 요구 내공 수위

#ifdef _XTS_ITEM_V22
	short			sReqDex ;		// 요구 심기
	short			sReqStr ;		// 정기
	short			sReqHealth;		// 필요 건강
	short			sReqJingi;		// 필요 진기
	short			sReqInt;		// 필요 지혜
#else
	short			sReqSim;		// 요구 심기
	short			sReqJung;		// 요구 정기
#endif

	//SPECIAL	modified by changkhan	2004.10.13
	unsigned short	usSpecial_ID_1;
	unsigned short	usSpecial_Time_1;
	unsigned char	ucSpecial_Value_1;
	unsigned char	ucSApply_Type_1;
	unsigned short	usSpecial_Prob_1;

	unsigned short	usSpecial_ID_2;
	unsigned short	usSpecial_Time_2;
	unsigned char	ucSpecial_Value_2;
	unsigned char	ucSApply_Type_2;
	unsigned short	usSpecial_Prob_2;

	unsigned short	usSpecial_ID_3;
	unsigned short	usSpecial_Time_3;
	unsigned char	ucSpecial_Value_3;
	unsigned char	ucSApply_Type_3;
	unsigned short	usSpecial_Prob_3;

	unsigned short	usSpecial_ID_4;
	unsigned short	usSpecial_Time_4;
	unsigned char	ucSpecial_Value_4;
	unsigned char	ucSApply_Type_4;
	unsigned short	usSpecial_Prob_4;

	//socket
	unsigned char	ucMaxSocket;
	short			sGroup; //Author : 양희왕 //breif : 08/01/29 sAccord 를 변경.. 세트아이템으로 사용

}_XGI_AccessoryItem_Property;


// ----------------------------------------------
// 생활 도구
typedef struct XGI_LifeItem_Property : public _XGI_Item_Base
{
	char			cThirdType;
	char			cClanType;
	unsigned short	usDurability ;		// 최대 내구
	unsigned char	ucCount;
	char			cBalance ;			// 조화 ; 최대, 최소 공격력의 평균을 정함.
	char			cProb;
	char			cLuck;
	char			cStrength ;			// 무기의 강도
	char			cRank ;				// 제련 등급 : 아이템이 제련될 수 있는 최대 등급
	unsigned short	usAbility_ID;
	unsigned char	ucMax_Merge;
	char			cRepair_Prob;
	short			sModel_Type;
}_XGI_LifeItem_Property;


// ----------------------------------------------
// 악기
typedef struct XGI_MusicItem_Property : public _XGI_Item_Base
{

}_XGI_MusicItem_Property;


// ----------------------------------------------
// 통행증
typedef struct XGI_PassItem_Property : public _XGI_Item_Base
{

}_XGI_PassItem_Property;



// -----------------------------------------------
// 소켓
typedef struct XGI_SocketItem_Property : public _XGI_Item_Base
{

#ifdef _XDEF_ITEM_V23 /*!< @author 양희왕 @breif 아이템 테이블 Thirdtype 추가  */
	unsigned char	ucThirdType;
#endif

	short	sSkill_Effect_ID1;

#ifdef _XTS_ITEM_V20
	TCHAR*	cSkill_Info1;
#else
	char    cSkill_Info1[57] ;     // 아이템 인포메이션.
#endif
	char	cType1;
	short	sValue1;
	char	cProb1;
	char	cAP_Type1;
	short	sAP_Per1;
	short	sAP_Count1;

	short	sSkill_Effect_ID2;
#ifdef _XTS_ITEM_V20
	TCHAR*	cSkill_Info2;
#else
	char    cSkill_Info2[57] ;     // 아이템 인포메이션.
#endif
	char	cType2;
	short	sValue2;
	char	cProb2;
	char	cAP_Type2;
	short	sAP_Per2;
	short	sAP_Count2;
	
	short	sSkill_Effect_ID3;
#ifdef _XTS_ITEM_V20
	TCHAR*	cSkill_Info3;
#else
	char    cSkill_Info3[57] ;     // 아이템 인포메이션.
#endif
	char	cType3;
	short	sValue3;
	char	cProb3;
	char	cAP_Type3;
	short	sAP_Per3;
	short	sAP_Count3;

	short	sSkill_Effect_ID4;
#ifdef _XTS_ITEM_V20
	TCHAR*	cSkill_Info4;
#else
	char    cSkill_Info4[57] ;     // 아이템 인포메이션.
#endif
	char	cType4;
	short	sValue4;
	char	cProb4;
	char	cAP_Type4;
	short	sAP_Per4;
	short	sAP_Count4;

#ifdef _XTS_ITEM_V20
	XGI_SocketItem_Property()
	{
		cSkill_Info1 = NULL;
		cSkill_Info2 = NULL;
		cSkill_Info3 = NULL;
		cSkill_Info4 = NULL;
	}
#endif
}_XGI_SocketItem_Property;

// -----------------------------------------------
// 영약
typedef struct XGI_ElixirItem_Property : public _XGI_Item_Base
{
	char			third_type;

	unsigned char	ucStep;
	unsigned char	ucReq_Level;
	unsigned short	usValue_Min;
	unsigned short	usValue_Max;
	unsigned short	usCoolDownDelay;
	unsigned short	usSuccess_Rate;

	unsigned short	usBody_Type[8];
	unsigned char	ucAddiction;
	unsigned char	ucVisualEffect;
	unsigned short	usTemp[4];	
}_XGI_ElixirItem_Property;

// -----------------------------------------------
// 패왕궤
typedef struct XGI_BoxItem_Property: public _XGI_Item_Base
{
	char			third_type;

	unsigned short	usTemp0;

	unsigned char	ucCase[10];
	short			sCaseProb[10];	

	unsigned char	ucVisualEffect;
	unsigned short	usTemp[4];	
}_XGI_BoxItem_Property;

// -----------------------------------------------
// 패왕궤 열쇠
typedef struct XGI_BoxKeyItem_Property: public _XGI_Item_Base
{
	unsigned char	ucProbWeapon;
	unsigned char	ucProbClothes;
	unsigned char	ucProbEtc;

	unsigned short	usTemp[4];	
}_XGI_BoxKeyItem_Property;

// -----------------------------------------------

// NPC 판매 관련
typedef struct XGI_ITEMTRADE_ITEM
{
	int			category;
	int			uniqueID;
}_XGI_ITEMTRADE_ITEM;


typedef struct XGI_ITEMTRADE_ITEMPACKAGE
{
	int						packageID;
	TCHAR					packageName[32];
	int						itemCount;
	_XGI_ITEMTRADE_ITEM		itemUnit[100];
}_XGI_ITEMTRADE_ITEMPACKAGE;


// 별호 관련
struct _XNicNameInfo {
	unsigned short	usIndex ;
	
#ifdef _XDEF_NEWSTRINGMANAGER		
	LPTSTR			cNickName;	
	LPTSTR			cNickInfo;
	LPTSTR			cNickInfoDetail;
#else
	char			cNickName[30] ;	
	char			cNickInfo[128] ;
	char			cNickInfoDetail[128];
#endif

	char			cNickIconId ;
	char			cApplyType;
	
	char			cType1;
	char			cType2;
	char			cType3;

	char			cNickGroup;
	char			cNickGrade;
	char			cClanType;
	char			cSexType;
	char			cDuplicate;
	char			cPickupCnt;

	struct  
	{
		unsigned short		usEffectIndex ;
		unsigned short		usEffectValue ;
		char				cEffectProbability ;
	} _Effect[5] ;
	
	short			sIconNumber;
};

// 아이템 믹스 테이블 관련
struct _XItemMixTable {
	unsigned int ucIndex[5];
};

// 아이템 제련 테이블
struct _XItemInchantTable 
{	
	int				cType;
	int				sID;
	int				sID_2;
	
	int				ucCount;
	unsigned int	uiPrice;
	int				effectID;
	int				iatteckplus;
	float			fattecktimes;
	int				iforceplus;
	float			fforcetimes;

	_XItemInchantTable() 
		: cType(0), sID(0), sID_2(0), ucCount(0), uiPrice(0), effectID(0), iatteckplus(0), fattecktimes(0), iforceplus(0), fforcetimes(0)
	{
	}
};
#endif // _XGAME_ITEM_DEFINE_
