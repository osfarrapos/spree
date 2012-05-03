#pragma once

// Monster, Move, Restart
// Main = 2, 3, 4, 5
// 16 - 47

struct _sHeader;

// Zone : Move, Monster, Restart etc..
#define MSG_NO_CHARAC_INFO						16		// 캐릭터 상세정보 (자기자신에게만)
#define MSG_NO_ZONE_RESTART						17		// 캐릭터 사망후 재시작 패킷
#define MSG_NO_CHARAC_SET_STATE					18		// 캐릭터 상태 설정. 플래그에 따라 다른 의미.

#define MSG_NO_STATE_CHANGE_REQUEST				19		// 캐릭터의 상태 변경 요청 : 분노의 사용으로 인한 폭주 사용 등등
#define MSG_NO_STATE_CHANGE_RESULT				19		// 상태 변경 요청의 응답

#define MSG_NO_CHARAC_GAMESTARTSIGNAL			20		// 게임 시작 시그널
#define MSG_NO_CHARAC_RETURN_LOBBY				21		// 존에서 로비로 나감
#define MSG_NO_DISCONNECT						22		// 서버에서만 쓰는 메시지로 연결이 끊어졌을 때
#define MSG_NO_LOGOUT							23		// 로그 아웃

enum EN_MOB_WALK_MODE
{
	MOB_WALK_MODE=0,
	MOB_RUN_MODE
};

enum EN_DEAD_FROM
{
	en_dead_from_monster = 0,
	en_dead_from_player,
	en_dead_from_monster_in_combat,				// 장원전 중인 플레이어가 장원 몬스터에게
	en_dead_from_player_in_combat				// 장원전 중인 플레이어가 장원전 중인 플레이어에게
};

// 필드 들어가기 전에 받아서 캐릭터 상태를 업데이트 한다.
// 기력, 최대기력, 오기 경험치 
//#define MST_NO_CHARAC_INFO					16
struct MSG_CHARAC_INFO		// 2004/08/14 업데이트
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cZone ;					// 읽어들일 존 번호...
	
	// 현재 Power 
	short				sCurLifePower ;			// 현재 생명
	short				sCurForcePower ;		// 현재 내공
	short				sCurConcentrationPower ;// 현재 의지
	
	// 최대 Power
	short				sMaxLifePower ;			// 최대 생명
	short				sMaxForcePower ;		// 최대 내공
	short				sMaxConcentrationPower ;// 최대 의지
	
	short				sLeftPoint ;
	short				sLeftMasteryPoint;		// 남은 마스터리 포인트
	
	// 진과 공 : 최대 진과 공은 내공수위에 의해 결정되므로, 포함시키지 않아도 된다. 
	unsigned int		uiLeft_Jin ;			// 현재 진
#ifdef _XTS_FAME
	int					iNotoriety ;			// 악명 수치로 변경.
#else
	unsigned int		uiLeft_Gong ;			// 현재 공
#endif
	
	short				sRetribution ;			// 업보 
	int					sHonor;					//명성  8
	short				sShowdowm ;				// 비무 전적
	
	// 위치 정보
	float				fInitPosX ;				// 초기 위치 
	float				fInitPosZ ;				// 초기 위치
	int					iTimeCode ;				// 현재 게임 시간
	short				sStandingOBB ;			// 서있는 장애물 
	
	short				sWoundRate ;			// 외상 비율 0 - 100
	short				sInsideWoundRate ;		// 내상 비율 0 - 100
	u_short				usFatigueRate ;			// 피로도	 0 - 100
	
	short				sFuryParameter;			// 분노 파라미터 : 0 - 10000 실제값

	char				cRespawnServerNo;		// 부활 위치로 저장된 서버 번호 : 0이면 없음
	char				cRespawnPosName[13];	// 부활 위치 이름. ServerNo이 0이 아닌 경우에만 유효

	// 비무 전적 정보
	int					iWinScore;
	int					iLostScore;
	int					iTieScore;
	int					iSeriesCount;

	int					iMaxSeriesWinCount;		// 최대 연승 기록
	int					iMaxSeriesLoseCount;	// 최대 연패 기록
	int					iAbuseCount;
	char				cPrevMatchResult;		// 0:승, 1:패, 2:무
	
	// 역할, 역할 등급, 기여도
	char				cClass;
	char				cClassGrade;
	int					iContribution;
	
	char				cGM ;					// 0: 일반 캐릭터, 1이상: GM 캐릭터
	
	int					iLeftLevelupLimit;		// 11성에서 12성까지의 남은 제한 시간 : 0이면 없음
	
#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
	u_char				ucPeaceMode : 2;		// 0:평화모드   1:전투모드   2:PK모드
	u_char				ucHideMode : 1;			// 0:은신해제   1:은신설정상태
	u_char				ucClothView : 1;		// 0:치장의상 보이기(Default)   1:일반의상 
	u_char				ucCharacModeReserve : 4;
#else
	char				cPeaceMode ;			// 0 - 평화 모드. 1 - 전투 모드.	
	char				cHindingMode ;			// 0 - 은신해제, 1 - 은신설정상태
#endif

#ifdef _XDEF_CASTLEBATTLE_MOD_CHARACINFO
	u_char				ucPrevDeadFrom;			// 누구에게 죽었는가 EN_DEAD_FROM
	u_char				ucDeadCountInCombat;	// 장원전 중 죽은 회수

	u_short				usLeftCombatPenaltyTime;	// 장원전 부활 패널티
#endif
} ;


// 사망후 재시작 패킷 
// 영향받는 수치 = 경험치(진), 위치, 체력만 회복(1/4)되고, 나머지는 그대로 유지한다. 
//#define MSG_NO_ZONE_RESTART					17
struct MSG_ZONE_RESTART
{
	_sHeader			header;
	u_char				ucMessage;

	float				fRestartPosX;			// 재시작 위치
	float				fRestartPosZ;			// 재시작 위치

	unsigned int		uiJin;					// 남은 진

	short				sVitalPower;			// 남은 체력
	char				cSeverNum ;				// 서버 번호
	
};

//#define MSG_NO_CHARAC_GAMESTARTSIGNAL				20
struct MSG_CHARAC_GAMESTARTSIGNAL
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usUniqueID;
};

//#define MSG_NO_CHARAC_RETURN_LOBBY				21
struct MSG_CHARAC_RETURN_LOBBY
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usUniqueID;
};

//#define MSG_NO_DISCONNECT							22
struct MSG_DISCONNECT
{
	_sHeader			header;
	u_char				ucMessage;
};

//#define MSG_NO_LOGOUT								23
struct MSG_LOGOUT
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usUniqueID;
};


// Zone 존내의 행동
// 이동, 채팅. 갘정 etc..
#define MSG_NO_ZONE_MOVE							24		// 캐릭터 이동
#define MSG_NO_ZONE_POSITIONING						25		// 존이동시 초기위치를 보내는 메세지, 강제 Positioning에 쓰인다.
#define MSG_NO_ZONE_REQ_CHARAC_INFO					26		// 다른 캐릭터의 상세 정보를 요구한다.
#define MSG_NO_ZONE_CHARAC_INFO						27		// 캐릭터의 상세 정보 (다른 유저에게 공개하는 정보)

#define MSG_NO_ZONE_CHAT_NORMAL						28		// 일반 채팅
#define MSG_NO_MONSTER_WARP							29
#define MSG_NO_MONSTER_MOVE							30		// 몬스터 이동
#define MSG_NO_MONSTER_VITAL						31		// 몬스터 체력
#define MSG_NO_CHARAC_CTRL							32		// 케릭터 콘트롤 메시지.(사망,...) S->C
#define MSG_NO_CHARAC_SELECT_MODE					33		// C->S : 사망후에 부활 방식 선택.
#define MSG_NO_CHARAC_REBIRTH_TARGET				34		// 누군가를 살린다.(응답으로 같은패킷. -1 실패. 이외는 성공. )
#define MSG_NO_MONSTER_TRANSFORM					35		// 몬스터가 변신한다. 

//#define MSG_NO_ZONE_MOVE							24
struct MSG_ZONE_MOVE
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usUniqueID;					// 유니크 아이디

	float				fX;							// 서있는 X좌표
	float				fZ;							// 서있는 Z좌표
	float				fgotoX;						// 이동할 X좌표
	float				fgotoZ;						// 이동할 Z좌표
	short				sStandingOBB;				// 밟고 서있는 OBB인덱스
	short				sCharacIndex : 4;			// 캐릭터 인덱스
	short				sAnimationNumber : 12;		// 애니매이션 번호
};

// 이동 시스템 변경 =============================
struct MSG_MOVE
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				move_mode;
	u_char				sequence;

	enum
	{
		en_move_mode_req_move = 0,		// 0 이동 요청, 실패하면 error가 간다,
		en_move_mode_player_move,		// 1 req_move의 일반적인 응답으로 주변의 다른 플레이어에게도 전달된다
		en_move_mode_charac_stop,		// 2 멈춰있을 때 C->S
		en_move_mode_stop,				// 3 멈춰있을 때 S->C

		en_move_mode_lps_jump_req = 10,	// 10 LPS등의 점프 이동요청에 쓰인다.
		en_move_mode_lps_jump_success,	// 11 점프 이동 요청 성공

		en_move_mode_goto_pos = 20,		// 20 서버 -> 클라이언트로 보내는 이동 명령 :sequence도 세팅
		en_move_mode_set_sequence,		// 21 시퀀스를 다시 세팅

		en_move_mode_error = 30,		// 30 OBB 또는 tile 위를 움직이려 한다.
	};
};

// en_move_mode_req_move = 0
struct MSG_MOVE_REQ_MOVE : public MSG_MOVE
{
	float				goto_x;
	float				goto_z;

	u_char				move_speed;		// 100이 정상, -30%이면 70, +50%이면 150
	u_char				direction;		// 방향 - 사용안함

	u_short				standing_obb;
	u_short				charac_index : 4;
	u_short				ani_number : 12;
};

// en_move_mode_player_move = 1
struct MSG_MOVE_PLAYER_MOVE : public MSG_MOVE 
{
	float				goto_x;
	float				goto_z;

	u_short				usUID;			// unique id

	u_char				move_speed;		// 100이 정상, -30%이면 70, +50%이면 150
	u_char				direction;		// 방향 - 사용안함

	u_short				standing_obb;
	u_short				charac_index : 4;
	u_short				ani_number : 12;
};

// en_move_mode_charac_stop = 2
struct MSG_MOVE_CHARAC_STOP : public MSG_MOVE
{
	float				x;
	float				z;

	u_char				move_speed;		// 100이 정상, -30%이면 70, +50%이면 150
	u_char				direction;		// 방향 - 사용안함

	u_short				standing_obb;
	u_short				charac_index : 4;
	u_short				ani_number : 12;
};

// en_move_mode_stop = 3
struct MSG_MOVE_STOP : public MSG_MOVE 
{
	float				x;
	float				z;

	u_short				usUID;

	u_char				move_speed;		// 100이 정상, -30%이면 70, +50%이면 150
	u_char				direction;		// 방향 - 사용안함

	u_short				standing_obb;
	u_short				charac_index : 4;
	u_short				ani_number : 12;
};

// en_move_mode_lps_jump_req = 10
struct MSG_MOVE_LPS_JUMP_REQ : public MSG_MOVE
{
	float				next_x;
	float				next_z;

	u_short				lps_index;

	u_char				check_type;		// 무엇을 체크하는지?
	u_short				check_index;	// 체크하는 것의 index
};

// en_move_mode_lps_jump_success = 11
struct MSG_MOVE_LPS_JUMP_SUCCESS : public MSG_MOVE
{
	float				goto_x;
	float				goto_z;

	u_short				usUID;
};

// en_move_mode_goto_pos = 20
struct MSG_MOVE_GOTO_POS : public MSG_MOVE
{
	float				goto_x;
	float				goto_z;

	u_short				usUID;
};

// en_move_mode_set_seqence = 21
struct MSG_MOVE_SET_SEQUENCE : public MSG_MOVE
{
};

// en_move_mode_error = 30
struct MSG_MOVE_ERROR : public MSG_MOVE
{
	u_char				ucCode;

	/* 
		0 - OBB 또는 Tile위를 움직이려 한다. 실패
		1 - 점프 이동의 정보 이상으로 요청 실패
	*/
};

// ==========================================

// 강제 이동, 시간 강제 할당
//#define MSG_NO_ZONE_POSITIONING					25
struct MSG_ZONE_POSITIONING
{
	_sHeader			header;
	u_char				ucMessage;

	char				cZone;

	float				fInitPosX;					// 위치
	float				fInitPosZ;					// 위치
	short				sStandingOBB;
	int					iTimeCode;					// 현재 게임 시간
}; 

//#define MSG_NO_ZONE_REQ_CHARAC_INFO				26
struct MSG_ZONE_REQ_CHARAC_INFO
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usUniqueID;					// 알고자 하는 유저의 유니크 아이디
};

//#define MSG_NO_ZONE_CHARAC_INFO					27
struct MSG_ZONE_CHARAC_INFO
{
	_sHeader			header;
	u_char				ucMessage;
	
	char				cName[13];					// 캐릭터 이름
	char				cNickName[13];				// 별호	

	u_short				usSelectNicID ;				// 선택한 특수 별호 ID	'0xffff' 이면 cNickName(개인별호 선택상태)
 
	
	u_short				usUniqueID;
	
	char				cGM			: 5 ;			// GM 이면 1 
	char 				cSex		: 3 ;			// 캐릭터 성별 0 : 남자, 1 : 여자.
	
	char				cFace;						// 얼굴
	char				cHair;						// 머리 모양
	
	char				cGroup;						// 소속 문파
	
	char				cAmorType;					// 상의	0
	char				cPantsType;					// 하의	1
	char				cCapType;					// 모자 2
	char				cAdaeType;					// 아대 3
	char				cShoesType;					// 신발 4
	char				cBackPackType;				// 배낭 
	char                cWeaponType ;               // 무기 타입
	
	short				sAmor;						// 상의 0
	short				sPants;						// 하의 1
	short				sCap;						// 모자 2
	short				sAdae;						// 아대 3
	short				sShoes;						// 신발 4
	short				sBackPack;					// 배낭
	
	short				sUseWeapon;					// 사용 무기
	
	char				cCharacState;				// 캐릭터의 상태 : 0 = Alive, 1 : 빈사, 2 : 혼수  3: 분노  4 : 폭주

	float				fCurPosX ;					// 
	float				fCurPosZ ;					//

	DWORD				dwConditionState ;			// 유저의 상태 : Condition과 기타 상태(비무,파티,장)...
	
	char				cMoveSpeed;
	char				cAttackSpeed;

	u_char				ucGamingStatePK : 1;		// 1이면 PK, 0이면 스탠스
	u_char				ucGamingStatePE : 1;		// 1이면 문파 공적
	u_char				ucGamingStateSafe : 2;		// 0:일반 1:안전지대 2:경범 3:중범 
	u_char				ucGamingStateAbsent : 2;	// 0:Normal  1:자리비움
	u_char				ucGamingStateFM : 1;		// 0:일반	1:자유비무중
	u_char				ucGamingState : 1;			// reserved
	
	short				sHyperRunID ;			// 경공 스킬 아이디
	u_char				ucHyperRunStep ;		// 0 - 11 까지. ( 즉, 스킬 아이디가 유효할 때만 이 값 참조 )
	int					iOR_Index ;				// 분파, 방회	
	char				cOR_Class ;				// 계급
#ifdef _XTS_ALIANCE_20061018	
	int                 iAllyIndex;				// 동맹 정보.
#endif	//#ifdef _XTS_ALIANCE_20061018

#ifdef _XTS_FAME
	u_char				ucFameClass : 4 ;		// 악명 클래스
	u_char				ucHonorClass : 4 ;		// 명성 클래스

	#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI

		// 음양혼신 공격력중 최대값을 가진 속성에 1 설정, 다른 것들은 0
		u_char				uc4Eff_MaxDam_minus : 1;
		u_char				uc4Eff_MaxDam_plus : 1;
		u_char				uc4Eff_MaxDam_hon : 1;
		u_char				uc4Eff_MaxDam_sin : 1;

		// 음양혼신 저항력중 1 이상인 곳에 1을 설정
		u_char				uc4Eff_Res_minus : 1;
		u_char				uc4Eff_Res_plus : 1;
		u_char				uc4Eff_Res_hon : 1;
		u_char				uc4Eff_Res_sin : 1;

	#else
		u_char				ucReserve1 ;			// 
	#endif //_XDEF_4EFF_SYSTEM_070625_KUKURI
	
	#ifdef _XTS_VIEWPKTARGETLEVEL
		short				sInnerLevel;			// level
	#else
		u_short				usReserve2 ;			//
	#endif //_XTS_VIEWPKTARGETLEVEL

#else
	int					iHonor;					// 명성
#endif	//#ifdef _XTS_FAME


#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
	u_char				ucClothView : 1;		// 0:치장의상   1:일반의상
	u_char				ucTemp1 : 3;
#else
	u_char              ucTemp1 : 4 ;
#endif

	u_char              ucWeaponStrength : 4 ;   // 무기의 제련 회수. 0 - 15
	char				cMaskType;				/// 복면 타입
	short				sMask;					// 복면
	
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	u_short				usFMNick;
#endif

#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	 short				sSetVisualEffect1;	// 세트 아이템의 비주얼 이펙트
	 short				sSetVisualEffect2;
#endif
};

struct MSG_CHARAC_SET_STATE
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cFlag ;			// EN_STATE_PACKET_FLAG
	
	__charac_state		state ;
};

// 일반 채팅
// sLength = strlen(cSay) + 18 (NULL 문자 포함 )
// 따라서 채팅 문자열: strlen(cSay) = sLength - 18 
// 잘못된 패킷이 들어와도 채팅 문자열의 마지막은 서버가 0으로 셋팅할 것임
// cSay[ucLength-18] = 0 ; 또한 ucLength 가 68 을 넘는 것은 패기함
//#define MSG_NO_ZONE_CHAT_NORMAL					28

#define _XDEF_CHATMAXLENGTH		81

struct MSG_ZONE_CHAT_NORMAL
{
	_sHeader			header;
	u_char				ucMessage;

	char				cMode;		//	0-15일반A(칼라값), 16 의협 32 귀속말 64 브로드, 96-99 외치기 127 운영자의 메세지. 색지정 됨.
									// 50: [Vina Shutdown] 피로도 공지(1시간마다 출력)


	char				cName[13];
	char				cSay[_XDEF_CHATMAXLENGTH];		//	채팅 메시지( strlen + 1 : NULL 포함 ) 
														// _XDEF_CHATMAXLENGTH은 최대길이다..보낼 때 짤라서 보냄.
														// 모드가 50일때 오늘 남은 피로도 

};

//#define MSG_NO_MONSTER_WARP		 : 29 몬스터의 순간이동 
struct MSG_MONSTER_WARP
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobType : 13;				// 몬스터 타입 (0-8192)
	u_short				usInfo : 3;					// 0 : 몬스터 생성
													// 1 : 워프에 의한 사라짐 (위치 무시)
													// 2 : 워프에 의한 나타나기

	u_short				usMobID;					// 몬스터의 아이디. 0 - 4191 까지는 안전하게 쓸수 있다. 4000마리...

	char				cMobVitalRate;				// -1이면 죽은 것.

	float				fGotoX;
	float				fGotoZ;

	u_char				ucMobWalkMode : 4 ;		// 몬스터의 걷기 모드 : 0 일반, 1 : 달리거나 순간이동...
	u_char				ucMobSizeType : 4 ;		// 0 : 일반 크기, 1 ~ 15 : 변신 크기.
};

//#define MSG_NO_MONSTER_MOVE						30
struct MSG_MONSTER_MOVE
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_short				usMobType ;			// 몬스터의 타입
	
	u_short				usMobID ;			// 몬스터의 아이디. 
	
	float				fX ;					
	float				fZ ;
	float				fGotoX ;
	float				fGotoZ ;
	
	char				cMobVitalRate ;			// 몬스터 체력
	
	u_char				ucMobWalkMode : 1;		// 몬스터의 걷기 모드 : 0 일반, 1 : 달리거나 순간이동...
	u_char				ucSpeedRate: 7;			//(몬스터의 본래속도/5)의 값 (ex: 원래속도 = 100/5 = 20)

} ;

//#define MSG_NO_MONSTER_VITAL						31
struct MSG_MONSTER_VITAL
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobID;
	char				cMobVitalRate;		// -1 이면 죽은것
};

// 몬스터 변신 패킷 : 받으면 변신 시작. 
//#define MSG_NO_MONSTER_TRANSFORM		35		// 몬스터가 변신한다. 
struct MSG_MONSTER_TRANSFORM
{
	_sHeader			header ;
	u_char				ucMessage ;
	char				cChangeMode ;			// 0 : 변신, 1 : 원래로 돌아가기.				
												// ( 이펙트가 다를 듯 해서, 모드를 구분해 둔다. )

	u_short				usMobID ;				// 대상 몬스터
	u_short				usChangeType ;			// 변화하는 타입
	
	char				cMobVitalRate ;			// 몬스터 체력
	
	
} ;

// 사후 처리 ===========================================================

enum EN_CHARAC_CTRL_FLAG
{
	charac_ctrl_binsa=0,				// 체력 0.. 빈사상태로 설정하라. : usTime
	charac_ctrl_dead,					// 혼수 상태로 설정 : usTime 으로 카운트
	charac_ctrl_freeze,					// 꼼짝마 : 운영상 필요할 듯.
	charac_ctrl_release,				// Freeze 해제
	charac_ctrl_awake_binsa,			// 빈사 상태인 유저 깨우기
	charac_ctrl_awake_honsu,
	charac_ctrl_erase,					// 지워라..( 다른 곳에서 잠시후에 나타난다. )
	charac_ctrl_dead_in_match,			// 비무중 사망
	charac_ctrl_dead_by_pk,				// 주위에 알리는 패킷 : 자신이면 무시
	charac_ctrl_dead_in_fm,				// 자유비무중 사망
} ;

union __charac_ctrl
{
	u_short		usTime ;	// Time(초) 동안 움직이지 못하도록.. 0xffff 이면 영구히... 
	// 단 .Release 받거나 시간 지나면 풀어진다. 
} ;


struct MSG_CHARAC_CTRL
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cFlag ;
	
	u_short				uid ;			// uid 가 혼수 혹은 빈사에 빠졌다.
	
	__charac_ctrl		context ;
} ;

enum EN_CHARAC_SELECT_MODE
{
	en_charac_select_at_city = 0,			// 마을
	en_charac_select_at_save_position,		// 여장 푼 곳
	en_charac_select_at_last_position,		// 그 자리
	en_charac_select_wait_other_help,		// 도움을 기다린다.
	en_charac_select_cancel_wait,			// 빈사에서 기다리지 않고 바로 혼수상태로 들어간다.
	en_charac_select_spell_rebirth,			// 누군가의 도움으로 부활 되었다
	en_charac_select_item_use,				// 화타신단으로 부활
	en_charac_select_in_castle,				// 공수 관계 없이 이 모드로 요청
};

struct MSG_CHARAC_SELECT_MODE
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cMode ;				// 0 : Waiting, 1 : Return, 2 : Rebirth, 
	char				cSelectPosition ;	// 모드가 1일때, 선택한 위치 인덱스. : 아직 안쓰임. 0으로 셋.
} ; 


// 빈사 혹은 혼수 상태의 유저를 깨우려 할때, 
struct MSG_CHARAC_REBIRTH_TARGET_REQ
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_short				target ;		// 대상의 아이디를 써서 보낸다.
	
	char				cFlag ;			// 0 : 빈사상태 깨우기,  1 : 아이템으로 살리기, 2 : 스킬로 살리기.
	char				cIndex ;		// 플래그값이 1이면 사용될 아이템의 인벤인덱스, 2이면 런스킬의 인덱스.
} ;


// uid가 타겟을 깨웠다. 또는 실패했다. 
struct MSG_CHARAC_REBIRTH_TARGET
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cFlag ;			// 0 : 빈사상태 깨우기,  1 : 혼수상태 깨우기. -1 실패.
	u_short				uid ;			
	u_short				usTargetID ;
} ;

// 부활 처리
#define MSG_NO_SAVE_RESPAWN_POS_REQUEST		251			// C->S 여장 풀기 요청 패킷
#define MSG_NO_SAVE_RESPAWN_POS_RESULT		251			// S->C 위 여장 풀기 결과. Yes/No와 간단한 이유

struct MSG_SAVE_RESPAWN_POS_REQUEST
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usNPCID;		// 여장 풀기 NPC의 아이디
};

struct MSG_SAVE_RESPAWN_POS_RESULT
{
	_sHeader			header;
	u_char				ucMessage;

	char				cResult;		// 0: 성공  1: NPC 아이디 없음  2: 역할 없음
	char				cName[13];
};

// State Change 요청
enum STATE_CHANGE_ENUM
{
	state_change_en_normal_fury = 0,			// 0부터 다시 시작 : C->S 분노 값이 모두 소비되면 서버에 요청한다.
	state_change_en_start_fury,					// 분노 시작 : S->C
	state_change_en_start_convergence,			// 폭주 시작 : C<->S 요청과 응답
	state_change_en_peace_mode,					// 평화 모드로 바꿀 때
	state_change_en_battle_mode,				// 전투 모드로 바꿀 때
	state_change_en_to_pk_mode,					// pk 모드로 바꿀 때
	state_change_en_to_peace_mode,				// pk에서 평화로 전환
	state_change_en_to_battle_mode,				// pk에서 전투로 전환
	state_change_en_notice_to_peace_mode,		// 누군가가 pk에서 평화로 전환
	state_change_en_notice_to_battle_mode,		// 누군가가 pk에서 전투로 전환
	state_change_en_notice_to_pk_mode,			// 누군가가 pk로 전환
	state_change_en_on_pe_mode,					// 문파 공적 모드
	state_change_en_off_pe_mode,				// 
	state_change_en_notice_on_pe_mode,			// 주위에
	state_change_en_notice_off_pe_mode,
	state_change_en_req_pe_end,					// 15 C->S PE 가 끝났을 때, 서버에 요청. : MSG_STATE_CHANGE_REQUEST
	state_change_en_res_not_pe_end,				// MSG_STATE_CHANGE_TO_PE : req_pe_end 의 No 응답. 아직 끝나지 않았음. 시간 업데이트
	state_change_en_enter_tutorial_req,			// 17 튜토리얼 들어갈 때 요청
	state_change_en_enter_tutorial,				// 18 튜토리얼 들어가라.
	
	state_change_en_leave_tutorial_req,			// 19 투토리얼 나오기 요청.
	state_change_en_leave_tutorial,				// 20 튜토리얼 나가라.
	
	state_change_en_saferegion_req,				// 21 안전 지역 속성 변경 요청 0:일반 1:안전
	state_change_en_saferegion_notice,			// 22 주위에 상태 변화를 알린다 0:일반 1:안전 2:경범 3:중범

	state_change_en_to_normal_state,			// 23 자리비움 -> 정상
	state_change_en_to_absent_state,			// 24 정상 -> 자리비움

	state_change_en_clothview_select_deck = 30,	// 30 의상보이기 : 치장의상 (요청/응답)
	state_change_en_clothview_select_normal,	// 31 의상보이기 : 일반의상 (요청/응답)
};

struct MSG_STATE_CHANGE 
{
	_sHeader			header;
	u_char				ucMessage;
	
	u_char				ucMode;
};

struct MSG_STATE_CHANGE_REQUEST : public MSG_STATE_CHANGE
{
	char				cMapID;
	char				cAreaCategory;
	char				cAreaID;
};

struct MSG_STATE_CHANGE_SAFEREGION_REQUEST : public MSG_STATE_CHANGE
{
	u_char				ucSafeRegionState;		// 0,1 일반과 안전지대 전환 요청 2:경범, 3:중범 추가
};

struct MSG_STATE_CHANGE_SAFEREGION_NOTICE : public MSG_STATE_CHANGE
{
	u_char				ucSafeRegionState;
	u_short				usCharacterUID;
};

// 처음 또는 중간에 pk 모드로 전환되면, 자신에게, MSG_STATE_CHANGE_TO_PK 가 가고,
// 주변에는 MSG_STATE_CHANGE_NOTICE 가 간다.

// pk 모드에서, 평화/전투 모드로 변경될 때는, 자신에게 CHANGE_RESULT 가 한번 간 후에, 
// 주위에 CHANGE_NOTICE 가 가게 된다. 

struct MSG_STATE_CHANGE_RESULT : public MSG_STATE_CHANGE
{
	u_char				ucResult;				// 0은 OK, 나머지는 No
};

// 자기 자신
struct MSG_STATE_CHANGE_TO_PK : public MSG_STATE_CHANGE
{
	u_short				ucLeftTime;				// 최대 600초
};


// 다른 사람 - pk/pe
struct MSG_STATE_CHANGE_NOTICE : public MSG_STATE_CHANGE 
{
	u_short				usUniqueID;
};


// pe mode --------------------------------------------------------┐
// on_pe, off_pe
struct MSG_STATE_CHANGE_TO_PE : public MSG_STATE_CHANGE
{
	u_short				usLeftTime;				// 남은 시간 (초)
};
//-----------------------------------------------------------------┘

// state_change_en_to_normal_state
// state_change_en_to_absent_state
struct MSG_STATE_CHANGE_ABSENT : public MSG_STATE_CHANGE
{
	u_short				usUniqueID;
};

// state_change_en_clothview_select_deck 
struct MSG_STATE_CHANGE_CLOTHVIEW_SELECT_DECK : public MSG_STATE_CHANGE
{
};

// state_change_en_clothview_select_normal
struct MSG_STATE_CHANGE_CLOTHVIEW_SELECT_NORMAL : public MSG_STATE_CHANGE
{
};


#define MSG_NO_STANCE_MOTION_CHANGE			36		// 스탠스 모션 변경
struct MSG_STANCE_MOTION_CHANGE
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usID;
	short				sStanceMotion;
};


// 비무 관련
#define MSG_NO_OBJECT_SIMPLE_INFO			37		// Object들의 브로드캐스팅 정보
#define MSG_NO_OBJECT_INFO_REQUEST			38		// 상세 정보 요청 패킷
#define MSG_NO_OBJECT_DETAIL_INFO			39		// Request로 요청하고 이패킷으로 응답

// 비무 깃발과 그외 오브젝트들
enum EN_OBJECT_TYPE
{
	en_object_type_sega = 0,				// 세가, 혈맹의 정보
	en_object_type_match_flag,				// 비무 깃발의 정보
	em_object_type_simple_info,		// 캐릭터 간략 정보
	en_object_type_items_info,		// 아이템 정보
	en_object_type_ability_info,	// 능력치 정보
	en_object_type_all_info,		// 전체 정보
};

struct MSG_OBJECT_SIMPLE_INFO
{
	_sHeader			header;
	u_char				ucMessage;

	char				cObjectType;		// EN_OBJECT_TYPE
};

// 비무 깃발의 단순 정보 : 주위로 5초에 한번씩 브로드캐스팅
struct MSG_OBJ_MATCH_FLAG_INFO : public MSG_OBJECT_SIMPLE_INFO
{
	char				cMatchMode;

	u_short				usMatchID;			// 비무의 아이디

	float				fFlagPosX;			// 깃발의 위치
	float				fFlagPosZ;
};

struct MSG_OBJECT_INFO_REQUEST
{
	_sHeader			header;
	u_char				ucMessage;

	char				cObjectType;		// EN_OBJECT_TYPE
};

struct MSG_MATCH_FLAG_INFO_REQUEST : public MSG_OBJECT_INFO_REQUEST 
{
	u_short				usID;
};

struct MSG_SEGA_INFO_REQUEST : public MSG_OBJECT_INFO_REQUEST
{
	u_short				usID;
};

//MSG_NO_OBJECT_INFO_REQUEST
struct MSG_CHARAC_MORE_INFO_REQ : public MSG_OBJECT_INFO_REQUEST
{
	char				cTo_or_From_Name[13] ;	
	char				cFlag ;
	enum
	{
		_request,			// a->b 에게 정보보기 요청
		_response_yes,		// b->a 에게 승락
		_response_no,		// b->a 에게 거부
		_response_cancel,	// 정보보기 취소
		_response_error,	// 에러(상대가 로그오프중 또는 알수 없는 에러)
	};
};

struct MSG_OBJECT_DETAIL_INFO
{
	_sHeader			header;
	u_char				ucMessage;

	char				cObjectType;
};

struct MSG_MATCH_FLAG_DETAIL_INFO : public MSG_OBJECT_DETAIL_INFO 
{
	char				cMatchMode;

	u_short				usMatchID;

	u_short				usMatchCharac1;
	u_short				usMatchCharac2;
};

struct MSG_CHARAC_MORE_INFO : public MSG_OBJECT_DETAIL_INFO
{
	// TODO : 캐릭터의 정보(간략정보, 전체정보, 장착 아이템 정보 등... )	

	char	cCharacName[13] ;

	// 간략정보
	short	sInnerLevel ;	// 성취단계
	char	cMoonpa ;		// 문파
	char	cClass ;		// 직책	
	char	cClassGrade ;	// 역할 단계

	// 능력치 정보
	short	sConstitution;				//건강
	short	sZen;						//진기
	short	sIntelligence;				//지혜
	short	sDexterity;					//민첩
	short	sStr;						//근력


	// 아이템 정보
	enum _GET_ON_ITEMS_SLOT
	{
		pos_amor=0,         // 0
		pos_pants,			// 1
		pos_adae,			// 2
		pos_shoes,			// 3
		pos_cap,			// 4
		pos_gap_upper,		// 5
		pos_gap_lower,		// 6
		pos_backpack,		// 7
		pos_weapon_1,		// 8
		pos_weapon_2,		// 9
		get_on_max
	};
	
#ifdef _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI

	_sUser_Inven_v1_all getOnItems[get_on_max];

#else

	_sUser_Inven_v1_extension getOnItems[get_on_max];

	#ifdef _XDEF_SMELT_ITEM // Author : 양희왕 
	__inven_uselimit_info_v1        weapons[2]; 
	#endif

#endif
};

#define MSG_NO_TIME_CHECK				14

#ifdef _XDEF_SERVERTIMECHECK_071112_KERYGMA

// 3분 마다 한번씩 서버에 시간을 채워서 보낸다. 

struct MSG_TIME_CHECK
{
	_sHeader			header ;
	u_char				ucMessage ;
	u_char				ucMode;				// 0 - Timer, 1 - Hack Prog. List
	int					time;				// Mode 0
	char				prog1[17];			// 16byte. Mode 1
	char				prog2[15];			// 14byte. Mode 2
} ;

#else

// 60초 마다 한번씩 서버에 시간을 채워서 보낸다. 
// MSG_NO_TIME_CHECK 14 
struct MSG_TIME_CHECK 
{ 
	_sHeader header ; 
	u_char ucMessage ; 
	DWORD time ; // 클라이언트의 현재 시간을 넣는다. : timeGetTime() 
} ;

#endif



#define MSG_NO_MONSTER_EXIST			40			// 몬스터의 존재
#define MSG_NO_REQ_SPELLEFFECT			41			// 해당 몬스터에게 걸려있는 스펠, 컨디션 상세정보 요청
#define MSG_NO_RES_SPELLEFFECT			41			// 해당 몬스터에게 걸려있는 스펠, 컨디션 상세정보
#define MSG_NO_SPELLLIST_CHANGED		42			// 스펠 데이터가 변했음
#define MSG_NO_CONDITION_CHANGED		43			// 몬스터의 컨디션이 바뀌었음

struct MSG_MONSTER_EXIST
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobType;
	u_short				usMobID;

	float				fX;
	float				fZ;
	char				cMobVitalRate;

	DWORD				dwConditionBit;		// 32가지 컨디션
};

struct MSG_REQ_SPELLEFFECT
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobID;				// 몬스터의 아이디
};

#define MAX_SPELLEFFECTDATA_PER_PACKET	8		// 패킷 하나에 들어가는 SPELL_EFFECT data의 개수

struct MSG_RES_SPELLEFFECT
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobID;

	u_char				ucRemain;				// 0:남은 데이터 없음   1:남은 데이터 있음

	short				aSpellID[MAX_SPELLEFFECTDATA_PER_PACKET];		// spell id array
	char				aSpellLevel[MAX_SPELLEFFECTDATA_PER_PACKET];	// 위 데이터의 각각의 레벨

	u_short				aEffectID[MAX_SPELLEFFECTDATA_PER_PACKET];		// Effect ID array
};

struct MSG_SPELLLIST_CHANGED
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobID;
	u_char				ucMsgType;				// 0:스펠 없어짐  1:스펠 추가  2:Effect 없어짐  3:Effect 추가됨

	short				usSpellEffectID;
	char				ucSpellLevel;
};

struct MSG_CONDITION_CHANGED
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobID;

	DWORD				dwConditionBit;		// 32가지 컨디션 데이터
};

#define MSG_NO_MONSTER_CHASE_NOTICE		26		// 몬스터가 지금 쫓고있음을 알린다.

struct MSG_MONSTER_CHASE_NOTICE
{
	_sHeader			header;
	u_char				ucMessage;
	
	unsigned short		usDNPCID;				//쫓고있는 몬스터의 ID
};

// state_change_en_enter_tutorial_req,	// 17 튜토리얼 들어갈 때 요청
struct MSG_STATE_CHANGE_ENTER_TUTORIAL_REQ : public MSG_STATE_CHANGE
{
} ;

// state_change_en_enter_tutorial,		// 18 튜토리얼 들어가라.
struct MSG_STATE_CHANGE_ENTER_TURIAL : public MSG_STATE_CHANGE
{
	u_char				ucResult ;		// 0 : 성공 
	// 1 : 이미 다 했다. 못 들어감.
} ;


// state_change_en_leave_tutorial_req,	// 19 투토리얼 나오기 요청.
struct MSG_STATE_CHANGE_LEAVE_TUTORIAL_REQ : public MSG_STATE_CHANGE
{
} ;


// state_change_en_leave_tutorial,		// 20 튜토리얼 나가라. : 요청오면 무조건 성공이다. 
struct MSG_STATE_CHANGE_LEAVE_TUTORIAL : public MSG_STATE_CHANGE
{
	char				cMapID;
	float				fX;
	float				fZ;
} ;


#define MSG_NO_ETCSET								224		// 정보의 세팅(외치기 etc.)

// ETC_SET  - 외치기 etc.
enum EN_CHAT_MODE
{
	en_chat_mode_white=96,
	en_chat_mode_black=97,
	en_chat_mode_class=98,
	en_chat_mode_all=99	
} ;

struct MSG_ETCSET
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;
};

enum EN_ETCSET_MODE
{
	en_etcset_mode_shout_set = 0,
	en_etcset_mode_shout_update,
	en_etcset_mode_shout_end,
	en_etcset_mode_shout_error
};

struct MSG_ETCSET_SHOUT_SET : public MSG_ETCSET
{
	// en_etcset_mode_shout_set : S->C
	// en_etcset_mode_shout_update : S->C

	char			cType;			// 채팅모드
	int				iLeftSecond;	// 남은시간 - 초
};

struct MSG_ETCSET_SHOUT_END : public MSG_ETCSET
{
	// en_etcset_mode_shout_end : S->C
};

struct MSG_ETCSET_SHOUT_ERROR : public MSG_ETCSET
{
	// en_etcset_mode_shout_error : S->C

	u_char			ucError;

	/*	
	  0 - not in shout mode., 정보가 남아있다면, 꺼주세요.
	*/
};


// 
// 경험치/숙련도/구룡기행
//

#define MSG_NO_ACCOUNT_ITEM_USE_STATUS			225

struct _sND_AIUS_Grade
{
	// 년/월/일/시/분/초 End Time
	u_int			uiYear		: 5;		// base 2000, Year = 2000 + uiYear
	u_int			uiMonth		: 4;
	u_int			uiDay		: 6;
	u_int			uiHour		: 5;
	u_int			uiMinute	: 6;
	u_int			uiSecond	: 6;

	u_char			ucReserve;

	char			cItemType;				// 사용중인 아이템
	short			sItemID;
} ;

struct MSG_ACCOUNT_ITEM_USE_STATUS
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;

	MSG_ACCOUNT_ITEM_USE_STATUS(u_short length, u_char mode)
	{
		header.sLength = length;
		header.sCrypto = 0;
		header.sCompressed = 0;

		ucMessage = MSG_NO_ACCOUNT_ITEM_USE_STATUS;

		ucMode = mode;
	}
};

enum EN_ACCOUNT_ITEM_USE_STATUS_MODE		// AIUS
{
		en_aius_mode_grade_set = 0,
		en_aius_mode_grade_end
};


/*
 *	
	게임서버에 들어갈때,  
	사용중인 경험치/숙련도 아이템이 있다면,
	en_aius_mode_grade_set 으로 보냅니다.
	없다면, 아무것도 가지 않습니다. 없는 것으로 설정하시면 됩니다.
		
	아이템의 사용은, 
	성공하면, en_aius_mode_grade_set 이 가고, 
	실패하면, 기존의 아이템 사용 실패만 갑니다. 
 
	그리고, set 패킷의 cItemType, sItemID 는 사용했던,
	아이템의 정보입니다. 이 정보를 이용하여, 디스플레이 하시면 됩니다.	
*				  
*/

struct MSG_AIUS_GRADE_SET : public MSG_ACCOUNT_ITEM_USE_STATUS
{
	// en_aius_mode_grade_set = 0
	_sND_AIUS_Grade			curGrade;

	MSG_AIUS_GRADE_SET() 
		: MSG_ACCOUNT_ITEM_USE_STATUS(sizeof(MSG_AIUS_GRADE_SET), en_aius_mode_grade_set)
	{
	}
};

struct MSG_AIUS_GRADE_END : public MSG_ACCOUNT_ITEM_USE_STATUS
{
	// en_aius_mode_grade_end  : s->C
	MSG_AIUS_GRADE_END()
		: MSG_ACCOUNT_ITEM_USE_STATUS(sizeof(MSG_AIUS_GRADE_END), en_aius_mode_grade_end)
	
	{
	}
};

#define MSG_NO_BW_WAR				180		// 흑백대전 관련 패킷

struct MSG_BW_WAR 
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucBWMode;

	MSG_BW_WAR(u_short size, u_char mode)
		: header(size), ucMessage(MSG_NO_BW_WAR), ucBWMode(mode)
	{
	}
};

enum EN_BW_WAR_MODE
{
	en_bwwar_left_time = 0,			// S->C 패왕동 관문의 출입 권한 남은시간 알려줌
									// 이 패킷이 가지 않으면 권한이 없는 것이다.

	en_bwwar_enter_gate_req,		// C->S 문을 통화할 수 있는지 문의
	en_bwwar_enter_gate_success,	// S->C 성공
	en_bwwar_enter_gate_fail,		// S->C 실패
};

// en_bwwar_left_time = 0
struct MSG_BW_LEFT_TIME : public MSG_BW_WAR
{
	int				iLeftTime;		// 남은시간 : 초, 0이면 권한 없음

	MSG_BW_LEFT_TIME()
		: MSG_BW_WAR(sizeof(MSG_BW_LEFT_TIME), en_bwwar_left_time)
	{
		iLeftTime = 0;
	}
};

// en_bwwar_enter_gate_req
struct MSG_BW_ENTER_GATE_REQ : public MSG_BW_WAR
{
	u_short			usGateIndex;	// Object index

	MSG_BW_ENTER_GATE_REQ()
		: MSG_BW_WAR(sizeof(MSG_BW_ENTER_GATE_REQ), en_bwwar_enter_gate_req)
	{
	}
};

// en_bwwar_enter_gate_success
struct MSG_BW_ENTER_GATE_SUCCESS : public MSG_BW_WAR
{
	u_short			usGateIndex;

	MSG_BW_ENTER_GATE_SUCCESS()
		: MSG_BW_WAR(sizeof(MSG_BW_ENTER_GATE_SUCCESS), en_bwwar_enter_gate_success)
	{
	}
};

// en_bwwar_enter_gate_fail
struct MSG_BW_ENTER_GATE_FAIL : public MSG_BW_WAR
{
	u_short			usGateIndex;

	MSG_BW_ENTER_GATE_FAIL()
		: MSG_BW_WAR( sizeof(MSG_BW_ENTER_GATE_FAIL), en_bwwar_enter_gate_fail )
	{
	}
};
