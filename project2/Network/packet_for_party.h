#pragma once

// Party
// Main = 14
// 160 - 167

struct _sHeader;

#define MSG_NO_PARTY_JOIN_REQ				160		// 파티원 추가 요청 또는 생성 요청
#define MSG_NO_PARTY_MEMBER_UPDATE			161		// 파티원 추가에서 삭제 등 관리용
#define MSG_NO_PARTY_INFO					162		// 파티에 가입하는 구성원에게 한번씩만 보내고 이후로는 관리메세지를 통해 변경
#define MSG_NO_PARTY_RESPONSE				163		// 요청 결과
#define MSG_NO_PARTY_ETC					164		// 미정
#define MSG_NO_PARTY_MEMBER_INFO			165		// 같은 존에 있는 파티원의 정보

// PARTY_JOIN_REQ : S <=> C
// 파티장이거나 속해있지 않은 유저가 다른 유저에게 파티 신청을 할 때, cTo에 대상의 이름을 
// 적어 서버에 보냄. 서버는 대상을 찾아 조건을 검사한 후 대상에게 같은 패킷을 보내거나, 
// 신청자에게 RESPONSE을 보냄.
//#define MSG_NO_PARTY_JOIN_REQ				112
struct MSG_PARTY_JOIN_REQ
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cTo[13] ;				// 신청자
} ;

// PARTY_JOIN_RESPONSE : S <=> C
// JOIN_REQ 에 대한 응답으로 쓰인다. cCmd는 상황에 따라 다른 값이다.
// cCmd = -1 : ( S -> C ) 파티 생성에 실패했을 경우 ( 거의 갈일이 없을 듯 )
// cCmd = 0 : ( C -> S ) JOIN_REQ를 받았을 때, 신청자의 이름을 채워 서버에 보낸다.
// 파티 생성에 성공하면, 곧바로 PARTY_INFO 가 갈것이고, 실패하면 cCmd 가 -1로 갈것이다. 
// cCmd = 1 : ( C -> S ) 대상이 거부를 눌렀을 때, 신청자의 의름을 채워 서버에 보낸다. 
// ( S -> C ) 신청자에게 대상이 거부를 눌렀음을 알린다.
// cCmd = 2 : ( S -> C ) 파티 신청을 했는데, 대상의 정보가 서버에 없다. 즉 대상이 없다는 의미. 
// cCmd = 3 : ( S -> C ) 대상이 이미 다른 파티에 속해있다.
// cCmd = 4 : ( S -> C ) 파티 인원이 꽉 차서 더이상 신청이 안된다.
// cCmd = 5 : ( S <=> C ) ... 
// cCmd = 6 : ( S -> C ) 대상이 다른 작업중이다. 잠시후 시도하라.
// cCmd = 7 : ( S -> C ) 우호적 관계에 있지않은 문파이다.

//#define MSG_NO_PARTY_RESPONSE				115
struct MSG_PARTY_JOIN_RESPONSE
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cCmd ;
	char				cToName[13] ;			// 대상.
} ;



/* =====================================================================================
             MEMBER_UPDATE : 파티원의 정보를 업데이트 하거나, 가입/탈퇴등을 알린다.

             Cmd      0 - 캐릭터를 추가 : cCharacName 유효 S->C
                      1 - 캐릭터 정보 변경( 서버 이동 ) S->C
                      2 - 캐릭터 정상 상태로.
                      3 - 캐릭터 자리비움 상태로.
===================================================================================== */
//#define MSG_NO_PARTY_MEMBER_UPDATE		113
struct MSG_PARTY_MEMBER_UPDATE
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cCommand ;				// 어떤 메시지인가
	char				cMemberNo ;				// 멤버 번호 
	char				cCharacName[13] ;		// 대상 캐릭터 이름
	u_short				ucUID ;					// unique id	
	char				cServerNo ;				
} ;

#define _XDEF_PARTY_MAX_MEMBER			9

//#define MSG_NO_PARTY_INFO			114
struct MSG_PARTY_INFO
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cShareExp ;				// 경험치 분배 방식 0 = 개인 취득, 1 = 공동 취득, 
	char				cShareItem ;			// 아이템 분배 방식 
	char				cCaptainNo ;			
	
	struct {		
		char			cMemberNo ;
		char			cMemberName[13] ;
		u_short			usUID ;
		char			cServerNo ;
#ifdef _XTS_PARTYMOREINFO
		short			sInnerLevel ;	// 내공수위(총합)
		char			cMoonpa ;		// 문파
		char			cClass ;		// 역할
		int				iHonor ;		// 명성
#endif

#ifdef _XDEF_PARTY_NOTAVAILABLE
		u_char			ucGameState : 2;		// 기본=0 자리비움=1
        u_char			ucClassGrade : 2;	// 역할 등급 : 0 ~ 3 : 20080410
		u_char			ucReserved : 4;
#endif
	} Member[_XDEF_PARTY_MAX_MEMBER] ;
} ;

// 파티 기타 명령어.
// cCmd : 
// 0 : C->S 해체 요청으로 파티장의 권한이다.
// 0 : S->C 파티원들에게 파티의 해체를 알린다.
// 1 : C->S 경험치 분배 방식을 변경했을 때( 파티장의 권한) 
//				-------------------------------------------------  
//				uFlag1					uFlag2			설명
//				-------------------------------------------------
//				0(경험치 분배)			0				개인분배
//										1				공동분배
//				-------------------------------------------------
//				1(아이템 분배)			0				개인분배
//										1				순차분배
//										2				랜덤분배
//				-------------------------------------------------
// 1 : S->C 경험치 분배 방식이 바뀌었을 때, 파티원들에게 알림
// 2 : S->C 서버쪽의 파티 이상으로 해체되었다.
// 3 : C->S 만약, 파티원의 정보(MEMBER_INFO)에 아직 등록되지 않은 캐릭터의 정보가 들어
// 오면 이 번호로 서버에 파티 정보를 다시 요청한다. 
// 4 : C->S cInfo = 멤버번호 : 추방 요청
// S->C cInfo = 멤버번호 : 파티장에 의해 추방되었음.
// S->C cInfo = -1 : 파티장의 추방요청 이상..
// 5 : C->S cInfo = 나의멤버번호 : 탈퇴 요청
// S->C cInfo = 멤버번호 : 탈퇴 하였음.
// 6 : C->S cInfo = 멤버번호, 파티장에 의한 권한 위임 요청
// S->C cInfo = 멤버번호. 파티장이 권한을 위임했다.
// 7 : S->C 잠시후에 시도하십시오.( 무슨 명령이든 )
// 8 : C->S 동행장 : 보스전 같이 할래?
// 8 : S->C 서버 : 동장이 보스전 하자는데~
// 9 : C->S 동행원 : 그러지뭐.. (uFlag1: 슬롯번호, uFlag2: 0.거부, 1.승낙)
// 9 : S->C(동장) : 보스전에 참여하는 파티원을 동행장에게 보낸다. (uFlag1: 슬롯번호, uFlag2: 0.거부, 1.승낙)
//10 : C->S (동장) : 보스전 들어가기 취소..
//10 : S->C (동행원) : 동장이 보스전 들어가기를 취소했음을 동행원들에게 알림..
//	info : 0,진입으로 인한 다이얼로그 박스 소멸  , 1, 동행장이 진입을 취소함버튼취소
	   
 //#define MSG_NO_PARTY_ETC				116		// 미정
struct MSG_PARTY_ETC
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cCmd ;
	union{		
		char				cInfo ;
		struct {
			unsigned	uFlag1 : 4 ;
			unsigned	uFlag2 : 4 ;				
		} _Flag ;
	} ;
	
} ;

// 같은 서버에 있는 파티원의 상세정보를 알린다.
//#define MSG_NO_PARTY_MEMBER_INFO		117
struct MSG_PARTY_MEMBER_INFO
{
	_sHeader			header ;
	u_char				ucMessage ;
	char				cMemberNo ;			// 멤버 번호 : 이값으로 구분함.
	
	char				cServerNo ;			// 맵 인덱스
	short				sPosX ;
	short				sPosZ ;
	short				sInnerLevel ;		// 내공수위(총합)
	char				cVitalPercent ;		// 체력 백분율
	char				cZenPercent ;		// 내력 백분율
	
	union {
		struct _SlotCheck {
			unsigned	slot0 : 1 ;
			unsigned	slot1 : 1 ;
			unsigned	slot2 : 1 ;
			unsigned	slot3 : 1 ;
			unsigned	slot4 : 1 ;
			unsigned	slot5 : 1 ;
			unsigned	slot6 : 1 ;
			unsigned	slot7 : 1 ;
			unsigned	slot8 : 1 ;			
		} _slot ;
		WORD sCheck ;
	};
	
} ;
	
