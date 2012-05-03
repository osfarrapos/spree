#pragma once
/** 
	@file packet_for_ctrl_gms.h
*/

// GMS
// Main = 9
// 72 - 79

#define MSG_NO_CTRL_MONSTER_SPAWN		248
#define MSG_NO_SERVER_INFO				249
#define MSG_NO_CTRL_CHARAC_SETTING		250		//S->C

// 몬스터 소환
struct MSG_CTRL_MONSTER_SPAWN
{
	_sHeader			header;
	u_char				ucMessage;

	short				sMobType;			// 소환할 몬스터 타입

	float				fX;
	float				fZ;

	char				cNumber;			// 소환할 마리수 : 최대 24
};

//#define MSG_NO_CTRL_CHARAC_SETTING			250
enum __ctrl_charac_enum
{
	ctrl_ch_point=0,
	ctrl_ch_position,
	ctrl_ch_charac_moonpa,
	ctrl_ch_name,
	ctrl_ch_item,
	ctrl_ch_money,
	ctrl_ch_skill,
	ctrl_ch_quest,
	ctrl_ch_class_grade,
	ctrl_ch_set_transparent,	// 투명 상태 변경
	ctrl_ch_set_all_point,		// 모든 스탯(파워) 초기화 : 진기,생명,지혜 제외.	
	ctrl_ch_contribution,		// 기여도 설정 : 추가부분 2005/03/24
	ctrl_ch_mastery_point,		// 마스터리 포인트 세팅
	ctrl_ch_set_mutetime,		// 채팅 금지 또는 해제
	ctrl_ch_full_fury,			// 분노를 꽉 채움
	ctrl_ch_full_bloodpoint		// 혈정 포인트 꽉 채움
} ;

// 5기/레벨/포인트 조작
struct __ctrl_charac_point
{
	char		cWhere ;		// 0 : 건강과 생명, 1 : 진기와 내공, 2. 지혜와 의지, 3. 민첩 4, 근력 5 - Left Point
	short		sValue1 ;		// 기 포인트 값 
	short		sValue2 ;		// 현재값과 최대값( Power ) 를 이값을 셋팅.
} ;


// 캐릭터 위치 조작.
struct __ctrl_charac_position
{
	float		fX ;
	float		fZ ;
	char		cZone ;			// 0이거나, 현재 서버라면 서버이동 없음.
} ;

// 문파 변경.
struct __ctrl_charac_moonpa
{
	char		cToMoonpa ;		// 문파를 바꾼다.
} ;

// 이름이나, 닉네임 바꾸기.
struct __ctrl_charac_nick_name
{
	char		cMode ;			// 0 은 이름 바꾸기, 1은 닉네임 바꾸기.
	
	char		cName[13] ;
} ;


// 인벤토/저장소 아이템 변경
struct __ctrl_charac_item
{
	char				cMode ;		// 0 - inven item setting, 1 - storage item setting
	_sUser_Inven_v1_all useritem;
} ;

// 돈 셋팅
struct __ctrl_charac_money 
{
	char		cWhere ;			// 0 - 인벤토리, 1 - 창고.(Depot)
	u_int		uiGold ;			// 최종 결과값임. 받으면 그대로 셋팅.
} ;


struct __ctrl_charac_skill
{
	char		cMode ;				// 0 - Add Skill, 1 - Remove Skill, 2 - Edit Skill(Set Step)
	
	u_char		ucSkillSlot ;
	short		sSkillID ;
	char		cSkillStep ;
	
	// 2006.02.06 insert
	int			iSkillExp;
} ;

struct __ctrl_quest_data
{
	char		cMode ;			// 0 - Add Skill, 1 - Remove Skill, 2 - Edit Skill(Set Step), 3 : trainstate=1
	
	u_short		usQuestID ;	// 변경될 퀘스트 아이디. 
	char		cNodeID;    // 변경될 노드 아이디. 
};

struct __ctrl_class_update
{
	char		cClass;
	char		cGrade;
	short		sAddMasteryPoint;
};

struct __ctrl_charac_set_trasparent
{
	char		cMode;			// 0:Off   1:On
};


struct __ctrl_charac_all_point
{
	short		sStrength ;					// 근력 스탯
	short		sZen;						// 진기
	short		sIntelligence;				// 지혜
	short		sConstitution;				// 건강
	short		sDexterity ;				// 민첩 스탯
	short		sMaxLifePower ;				// 최대 체력( 상태 효과 미포함인 기본 최대 체력 )
	short		sMaxForcePower ;			// 최대 내력
	short		sMaxConcentrationPower ;	// 최대 의지 
	short		sLeftStat;					// 남은 스탯 포인트	
	short		sLeftMastery;				// 남은 오의 포인트
};

struct MSG_CTRL_CHARAC_SETTING
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cCommand ;			// __ctrl_charac_enum 값.
	
	union 
	{
		__ctrl_charac_point			point ;
		__ctrl_charac_position		position ;
		__ctrl_charac_moonpa		moonpa ;
		__ctrl_charac_nick_name		nickname ;
		__ctrl_charac_item			item ;
		__ctrl_charac_money			money ;
		__ctrl_charac_skill			skill ;
		__ctrl_quest_data			quest;
		__ctrl_class_update			Class;
		__ctrl_charac_set_trasparent	Transparent;
		__ctrl_charac_all_point		AllPoint;
		int							iContribution;
		short						sMasteryPoint;
		int							iMuteTime;
	} ;
} ;

#define MSG_NO_GM_REQUEST					252			// C->S : gm 클라이언트의 요청
enum		// 요청 패킷
{
	en_gm_req_nextto_charac=0,		// gm 이 캐릭터 옆으로 간다.
	en_gm_req_charac_move,			// 대상을 이동 시킴.
	en_gm_req_npc_kill_sign,		// npc를 없앤다.
	en_gm_req_enter_warp,			// 운영자 자신이 워프 공간으로 들어간다. : 미구현
	en_gm_req_charac_moveto_warp,	// 대상 캐릭터를 워프 공간으로 들여보낸다. : 미구현
} ;

enum		// 결과 or 오류 번호.
{
	en_gm_result_success=0,				// 성공이다.( 이 값은 안갈수도 있다. )
	en_gm_result_charac_not_connect,	// 접속해 있지 않다.
	en_gm_result_charac_other_zone,		// 다른 필드에 있다.
	en_gm_result_npc_not_found,			// 그런 npc 없다.
	en_gm_result_server_off,			// 대상 서버는 꺼져 있다.
} ;

struct MSG_GM_REQUEST		// 이거 자체로는 쓰지 않고, 상속받아 쓴다.
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucRequest ;
} ;


#define MSG_NO_GM_RESULT					252			// 같은 번호로 처리.
struct MSG_GM_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucRequest ;
	u_char				ucResult ;		// 결과.( 실패 했을 때 간다. )
	u_char				ucInfo ;		// 결과에 따라 의미가 다르다.
} ;


// en_gm_req_moveto_charac 일 때, : 성공하면 CharacSetting으로 갈 것이다.
// 응답은 MSG_CTRL_CHARAC_SETTING::ctrl_ch_position, 실패하면, MSG_GM_RESULT
struct MSG_GM_REQ_NEXTTO_CHARAC : public MSG_GM_REQUEST
{
	char			cToCharac[13] ;		// 캐릭터 이름.
} ;


// en_gm_req_charac_move,			// 대상을 이동 시킴.
// gm에게는 MSG_GM_RESULT::success, 캐릭터에게는 MSG_CTRL_CHARAC_SETTING::ctrl_ch_position
// 실패시에는 MSG_GM_RESULT::en_gm_result_charac_not_connect, or en_gm_result_charac_other_zone
struct MSG_GM_REQ_CHARAC_MOVE : public MSG_GM_REQUEST
{
	char			cTargetName[13] ;	// 옮길 대상 이름.
	
	char			cServerNo ;			// 어떤 맵으로 옮길 것인가.
	float			fX ;				// 어디로.
	float			fZ ;				// 
} ;

// en_gm_req_npc_kill_sign
// 성공시에는 주변에 이 패킷이 다시 가고, 실패시에는 en_gm_result_npc_not_found로 간다. 
struct MSG_GM_REQ_NPC_KILL_SIGN : public MSG_GM_REQUEST
{
	u_char			ucType ;			// 몬스터 0 , 기타..
	u_short			usKillTarget ;		// 없앨 타겟.
} ;


//en_gm_req_enter_warp,			// 운영자 자신이 워프 공간으로 들어간다. : 미구현
struct MSG_GM_REQ_ENTER_WARP : public MSG_GM_REQUEST
{
	
} ;




//en_gm_req_charac_moveto_warp,	// 대상 캐릭터를 워프 공간으로 들여보낸다. : 미구현
struct MSG_GM_REQ_CHARAC_MOVETO_WARP : public MSG_GM_REQUEST
{
} ;

enum EN_CTRL_SERVER_INFO
{
	en_server_info_pk_zone = 0,			// pk 지역 여부. 못받으면 pk 지역이 아님
};

struct MSG_SERVER_INFO
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;
};

struct MSG_SVRINFO_PK_ZONE : public MSG_SERVER_INFO
{
	u_char			ucPKZone;			// 0:normal  1:pkzone
};


//Author : 양희왕	
#if defined(_ACCLAIM_VERSION) /*|| defined(_XVIETNAMESE)*/ || defined(_XJAPANESE) || defined(_XKOREAN)
	#define _XENABLE_GAMEGUARD
#endif

#ifdef _XUSEAPEXMODULE
	#undef _XENABLE_GAMEGUARD
#endif

#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT)
	#ifdef _XENABLE_GAMEGUARD
		#define _XUSE_GAMEGUARD
		#include "NPGameLib.h"
		#pragma message( "Include NPGameLib.h : Ctrl_GMS.h" )
		extern CNPGameLib	g_GameGuardInstance;
	#endif
#endif

#ifdef _XUSE_GAMEGUARD

#define MSG_NO_GGAUTH_REQ       158
#define MSG_NO_GGAUTH_ACK       158

typedef struct MSG_GGAUTH 
{
	_sHeader	header;
	u_char	ucMessage;
	GG_AUTH_DATA	data;
} MSG_GGAUTH_REQ, MSG_GGAUTH_ACK;

#endif
//Last updated: 07/01/18


/** @struct NEXON_PCB_INFO
 	@breif 넥슨 피씨방 인증 정보
 */
struct NEXON_PCB_INFO {
	UCHAR   ucResult; 

/*!
@retval 0 Error	- 넥슨으로부터 결과값을 받지 못했음
@retval I 체험판 - Internet
@retval P 인포샵 - Ratio PC
@retval F 정액피씨방 - Flat Cafe
@retval R 종량피씨방 - Ratio Cafe
@retval M 정량피씨방 - Prepaid
@retval C Address - Coupon Cafe Coupon
*/
	UCHAR   ucAddrDesc; 
	UCHAR   ucAccDesc; 
	UCHAR   ucOption; 

	int	iArgument; 
	bool	bIsPCBBillType; 
};


#define MSG_NO_COMMOM_REQ				67	/*!< */
#define MSG_NO_COMMON_ACK				67	/*!< */
#define MSG_NO_COMMON_NTF				67	/*!< */

/*
0: GS2C_피씨방정보_NTF: 유저가 필드 입장시 피씨방 유저인지 아닌지
1: GS2C_점혈술남은시간동기화_NTF: 10분 마다 서버와 클라이언트 간에 점혈술 남은 시간을 동기화 한다.
2: DS2GS_NTF   [유저의 피씨방 정보]
*/

typedef struct MSG_COMMON 
{
	_sHeader               header;
	u_char                 ucMessage;
	WORD                   wMode;

	union {
		BYTE  data[1024];
		
		// wMode = 0, 2
		struct NXPCB_INFO 
		{
			char	acc[en_max_lil+1];
			NEXON_PCB_INFO info; // 피씨방 정보
			BYTE	 dummy[1024-(en_max_lil+1)-sizeof(NEXON_PCB_INFO)];
		} NXPCB_Info;
		
		// wMode = 1
		struct NXPCB_TOUCH_ATTACK_REMAIN_MIN
		{
			WORD wRemainMin; // 점혈술 남은 시간 분(min) 단위로 전송
			BYTE dummy[1024-2];
		} NXPCBTouchAttackRemainMin;
		
		// wMode = 10
		struct _CIS_CASH_INVEN_REQ {
			bool	bIsPresent;		// 0: 일반 아이템, 1: 선물 아이템
			WORD	wPageNum;		// 페이지 번호
			WORD	wNumPerPage;	// 페이지 당 자료수
		} CIS_CASH_INVEN_REQ;
		
		// wMode = 11
		struct _CIS_CASH_INVEN_ACK {			
			USHORT  usTotalCnt;     // 캐쉬인벤의 (모든 페이지를 포함한) 총 아이템 개수; 몇 페이지를 보여줄지 계산하기 위해
			UCHAR	ucRowCount;		// 페이지에 보여줄 총 아이템 개수
			// ucRowCount 만큼 반복
			int		iPkgGuid;	// 아이템의 Guid
			int		iPkgIdx;	// 패키지 인덱스
		} CIS_CASH_INVEN_ACK;
		
		// wMode = 15
		struct _CIS_MOVE_ITEM_REQ {
			UCHAR	ucPkgCnt;	// 이동할 아이템 개수
			// ucPkgCnt 만큼 반복
			int		iPkgGuid;	// 아이템의 Guid
			int		iPkgIdx;	// 패키지 인덱스

		} CIS_MOVE_ITEM_REQ;
		
		// wMode = 16
		struct _CIS_MOVE_ITEM_ACK {
			UCHAR	ucPkgCnt;	// 옮기기에 성공한 패키지 아이템 개수
			// ucPkgCnt 만큼 반복
			int		iPkgGuid;	// 아이템의 Guid
			
			UCHAR	ucItemCnt;	// 행낭으로 옮겨진 아이템 개수
			// ucItemCnt 만큼 반복
//			char	cSlot;		// 행낭의 슬롯 인덱스
//			char	cType;		// 아이템 분류
//			short	sId;
//			UCHAR	ucItemNum;	// 아이템의 뭉탱이 개수
			_sUser_Inven_v1_all	Item;
		} CIS_MOVE_ITEM_ACK;
		
		// wMode = 20
		struct _CIS_PRESENT_MSG_REQ {
			int		iPkgGuid;	// 아이템의 Guid
		} CIS_PRESENT_MSG_REQ;
		
		// wMode = 21
		struct _CIS_PRESENT_MSG_ACK {
			char		acc[en_max_lil+1];	// 선물한 유저의 Account
			SYSTEMTIME	time;				// 선물 받은 날짜(년/월/일/시/분/초); DBTIMESTAMP
			char		msg[100+1];			// 선물 메시지
		} CIS_PRESENT_MSG_ACK;
	};
} MSG_COMMON, MSG_COMMON_REQ, MSG_COMMON_ACK, MSG_COMMON_NTF;


#ifdef _XUSEAPEXMODULE

#define MSG_NO_SECURITY                                    (49)                                  // 보안 모듈의 패킷 전송에 쓰인다. 

struct MSG_SECURITY 
{
	_sHeader                         header;
	u_char                             ucMessage;                    //
	u_char                             ucMode;                                       //  각 보안 모듈별로 분리해서 사용. 기본. 10개씩. 25개의 모듈을 설정할 수 있게 한다.
	
	MSG_SECURITY( u_short size, u_char mode )
			: header(size), ucMessage(MSG_NO_SECURITY), ucMode(mode)
	{
	}
};

 

enum ENUM_SECURITY_COM_MODE										// MSG_SECURITY::ucMode 
{
     // 0 - 9 : M-Etel Apex Module
     en_apex_client_login=0,									// 클라이언트에 Apex 실행 시점을 알려준다.
	 en_apex_client_login_result,								// CHCStart 의 리턴값을  받는다.
     en_apex_req_client_data,									// 1 서버에서 클라이언트로.
     en_apex_client_data,										// 2 클라이언트에서 서버
	 en_security_mode_none = 255                   // 이건, case 문 때문에 그냥 추가한 것.
};

#define APEX_MAX_LENGTH							(2048)
#define APEX_MAX_PACKETLENGTH					2048

// 0 - 9 : metel apex 
// en_apex_client_login=0,                         // 클라이언트에 Apex 실행 시점을 알려준다.
struct MSG_APEX_CLIENT_LOGIN : public MSG_SECURITY
{
	MSG_APEX_CLIENT_LOGIN() : MSG_SECURITY( sizeof(MSG_APEX_CLIENT_LOGIN), en_apex_client_login )
	{
	}
};

struct MSG_APEX_CLIENT_LOGIN_RESULT : public MSG_SECURITY
{
	long                    ret_value;                         // CHCStart 의 리턴값.
	MSG_APEX_CLIENT_LOGIN_RESULT()
        : MSG_SECURITY( sizeof(MSG_APEX_CLIENT_LOGIN_RESULT), en_apex_client_login_result )
    {
    }
};



// en_apex_req_client_data,                      // 1 서버에서 클라이언트로.
struct MSG_APEX_REQ_CLIENT_DATA : public MSG_SECURITY
{
	u_short               data_length;                                               // 
	char                   data[APEX_MAX_LENGTH];                       // APEX_MAX_LENGTH 보다 짧아도 자르지 않습니다. 

	MSG_APEX_REQ_CLIENT_DATA() : MSG_SECURITY( sizeof(MSG_APEX_REQ_CLIENT_DATA), en_apex_req_client_data )
	{
		ZeroMemory( data, sizeof(data) );
	}
}; 

// en_apex_client_data,                             // 2 클라이언트에서 서버로.
struct MSG_APEX_CLIENT : public MSG_SECURITY
{
	u_short               data_length;
	char                   data[APEX_MAX_LENGTH];

	MSG_APEX_CLIENT() : MSG_SECURITY( sizeof(MSG_APEX_CLIENT), en_apex_client_data )
	{
		ZeroMemory( data, sizeof(data) );
	}
};

#endif
