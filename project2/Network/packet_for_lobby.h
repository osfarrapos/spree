#pragma once

struct _sHeader;
struct _CharacList;

// Lobby
// Main = 1
// 8 - 15

// CHARAC_캐릭터 정보와 로비 8 - 15 ++ 재시작 패킷.
#define MSG_NO_CHARAC_REQ_LIST			8		// 캐릭터 리스트를 요구한다. C->S
#define MSG_NO_CHARAC_LIST				9		// 캐릭터 리스트 요구에 응하는 패킷.

#define MSG_NO_SHIFT_STATE				MSG_NO_CHARAC_REQ_LIST	// 서버상태(ex: 봉문해제)가 변경되었다는것을 알리는 패킷. S->C

#define MSG_NO_CHARAC_SELECT            10      // 캐릭터 창에서 캐릭터를 선택했다.
#define MSG_NO_CHARAC_CREATE            11		// 캐릭터 생성
#define MSG_NO_CHARAC_DELETE			12		// 캐릭터 삭제
#define MSG_NO_CHARAC_RESULT			13		// 캐릭터 생성/삭제 의 결과

// 빌링 관련 정보
#define MSG_NO_BILL_INFO				15		// 빌링 관련 정보

//#define MSG_NO_CHARAC_REQ_LIST		8 
struct MSG_CHARAC_REQ_LIST
{
	_sHeader			header;
	u_char				ucMessage;
};

//#define MSG_NO_SHIFT_STATE				MSG_NO_CHARAC_REQ_LIST	// 서버상태(ex: 봉문해제)가 변경되었다는것을 알리는 패킷. S->C
struct MSG_SHIFT_STATE
{
	_sHeader			header;
	u_char				ucMessage;
	
	u_int				uiStateFlag;		//서버군의 32bit상태정보(1: 봉문해제)
};

//#define MSG_NO_CHARAC_LIST			9
struct MSG_CHARAC_LIST
{
	_sHeader			header;
	u_char				ucMessage;

	char				nCharac;						// 캐릭터 개수
	_CharacList			List[3];						// 캐릭터 정보
	u_int				uiStateFlag;					//서버군의 32bit상태정보(1: 봉문해제)
	
#ifdef _XDEF_SHUTDOWN_20061027	
	short               sFatigueWeight ;				//베트남 shutdown 피로도(단위: 분)	
#endif

#ifdef _XVNBILLINGSYSTEM	// 사용하지 않음
	char				cBillMethod;					// 1: 회원가입 후 2일 24일간 무료
														// 2: 이벤트 무료
														// 3: 구룡입문 정액
														// 4: 구룡입문 정량
														// 5: 구룡기행 정액
														// 6: 구룡기행 정량
	
	DWORD				dwRemainTime;					// cBillMethod가 1,4,6일 경우 = 남은시간(sec)
														// 3,5일 경우 = Zero
	
	char				szExpireDate[12+1];				// 1,3,5일 경우 = YYYYMMDDHHMM
#endif	
	
#if defined(_XDEF_VINA_PCROOM_INFO) || defined(_XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI)
	u_int				ucPCRoomInfo : 4;
	u_int				uiSDSMode : 4;
	u_int				uiReserve : 24;
#endif

};

//#define MSG_NO_CHARAC_SELECT			10
struct MSG_CHARAC_SELECT
{
	_sHeader			header;
	u_char				ucMessage;

	char				cSelectName[13];		//캐릭터 이름
};

//#define MSG_NO_CHARAC_CREATE			11

struct MSG_CHARAC_CREATE 
{ 
	_sHeader			header ; 
	u_char				ucMessage ; 

	char				cName[13] ;				// 캐릭터의 이름 
	
	// 기수치 
	char				cConstitution ;			// 건강 : 5기를 수치로 받는다. ( 8~18 사이이며, 합계는 반드시 50이어야 한다. 
	char				cZen ;					// 진기 : 쟁여놓는 기수치는 없다. 
	char				cIntelligence ;			// 의지 
	char				cDexterity ;			// 민첩 
	char				cStr ;					// 근력
	
	char				cSex ;					// 성별 0 : 남자, 1 : 여자 
	char				cFace ;					// 얼굴형 : max 종류수 관계 없다. 
	char				cHair ;					// 머리 스타일 : max 종류수 관계없다. 
	
	char				cInitZone;				// 처음 위치할 서버 : 캐릭터를 만들때 묻는다. 
	
	u_char				ucClothSet ;			// 세트 번호 : 0, 1, 2 : 0은 아무것도 걸치지 않음.
	/* 2005/01/08 삭제 : 생성시 캐릭터 세트롤 함.
	short				sAmor ;					// 상의 : 2종 밖에 없는뎅 ㅡㅡ
	short				sPants ;				// 하의 : 2종 밖에 없다. 
	short				sShoes ;				// 신발 :
	*/
} ;

//#define MSG_NO_CHARAC_DELETE			12
struct MSG_CHARAC_DELETE
{
	_sHeader			header;
	u_char				ucMessage;

	char				cName[13];				// 캐릭터 이름
	char				cPassword[13];			// 비밀번호
};

//#define MSG_NO_CHARAC_RESULT			13
struct MSG_CHARAC_RESULT
{
	_sHeader			header;
	u_char				ucMessage;

	unsigned char		ucPrevCommand;			// 어떤 메세지의 결과인가
	unsigned char		ucResult;				// 0이면 성공, 1이면 실패 이유

	// ucPrevCommand : 캐릭터 생성에 대한 응답이면, 이 값은 MSG_NO_CHARAC_CREATE 이고, 
	// 성공이라면, ucResult 는 0이 될것이고, 아니면, 실패 이유가 될것이다.
	// MSG_CHARAC_CREATE 에 대한 대답으로 
	//   1 : 이미 있는 유저.  2: 사용할 수 없는 이름. 3: 기수치 오류  4: 기본 아이템 오류 5: 더이상 만들수 없음.
	// MSG_CHARAC_DELETE 의 대한 응답으로
	//   10 : 삭제할 수 없음.
	// MSG_CHARAC_SELECT 에 대한 응답으로 
	//   20 : 캐릭터를 선택할 수 없음.
	//   21 : 해당서버 OFF
};

//#define MSG_NO_BILL_INFO				15		// 빌링 관련 정보

struct _BillInfo {
	union
	{
		struct {
			char			cWHO ;				// 접속 유형
			char			cWhere ;			// 접속 장소
			char			cETC ;				// == 0
		} ;
		char				cBillingStatus[3] ;
	};
	char				cBillPayType[2];		// 지불방식
	char				cBillMethod[2] ;		// 과금방식
	char				cBillExpire[12] ;		// 과금 종료일YYYYMMDD
	int					iBillRemain ;			// 남은 시간 및 포인트
	
	char				cPCRoomEndDate[20] ;	// 피시방 과금 종료일 정보 (앞에2바이트는 종류, 나머지 뒤에는 날짜 정보)
    int					iAlertCode ;			// AlertMessage code
	char				cPedding[8] ;			// 예약필드
} ;

struct MSG_BILL_INFO
{
	_sHeader			header;
	u_char				ucMessage;
	
	_BillInfo			BillData;
} ;


// Packet_Result
#define _PACKET_RESULT_AUTH_OK_		1
#define _PACKET_RESULT_NONERROR_	0

// Result of alert ( _BillInfo::iAlertCode )
#define _PACKET_ALERT_LOGIN_INFO			 2	// 로그인했을때 빌링정보
#define _PACKET_ALERT_REMAIN_TIME			 1	// 사용자의 남은 시간
#define _PACKET_ALERT_NOT_ENOUGH_POINT		 0	// 남은시간 없다(강제종료)
#define _PACKET_ALERT_REBATE				-1	// 환불조치 되었음(강제종료)
#define _PACKET_ALERT_DOUBLE_LOGIN			-2	// 중족 로그인(강제종료)
#define _PACKET_ALERT_RENEW					-3	// 예약되어 있던 대기상품으로 대체
#define _PACKET_ALERT_ETC_ERROR_KILL		-4	// 기타 오류(강제 종료)

/*
2005/07/22.   ozzywow 

--------------------------------------------------------------------------
MSG_BILL_INFO 패킷에 대한 설명 : 사용자의 과금 정보입니다.
--------------------------------------------------------------------------

cBillingStatus ( 사용자의 접속 상태 ) 
--------------------------------------------------------------------------
|     값        |             설명                                       |
--------------------------------------------------------------------------
| HO			|	개인사용자											 |
| PC			|	게임방 사용자										 |
| SP			|	ISP 사용자											 |
--------------------------------------------------------------------------

cBillPayType ( 결제(지불) 방식 정보)
--------------------------------------------------------------------------
|     값        |             설명                                       |
--------------------------------------------------------------------------
| AO			|	후불 결제											 |
| DO			|	직불												 |
| FO			|	무료												 |
| PO			|	선불결제(일반적인 모든 경우 적용)					 |
--------------------------------------------------------------------------

cBillMethod ( 과금 방식 정보)
--------------------------------------------------------------------------
|     값        |             설명                                       |
--------------------------------------------------------------------------
| FM,FW, FD		|	월정액, 주말 월 정액, 일정액						 |
| FD,DB,HB		|	일정액, 기간정액, Day Block, Hour Block				 |
| TH			|	정량 												 |
| TB			|	종량, 사용자의 잔액에서 차감하는것을 의미, Time Block|
| PM, PD		|	플래티넘 월(일) 정액                                 |
| NO			|	무료 사용자(시간 정량)                               |
--------------------------------------------------------------------------
*/

#define MSG_NO_BILLING_INFO_REQ		45
#define MSG_NO_BILLING_INFO_ACK		46

#define _VNBILLING_BILLMETHOD_FREEOFCHARGE					1	//회원가입후 2일 24일간 무료
#define _VNBILLING_BILLMETHOD_FREEOFCHARGE_EVENT			2	//이벤트 무료
#define _VNBILLING_BILLMETHOD_BASIC_FIXEDAMOUNTSYSTEM		3	//구룡입문 정액
#define _VNBILLING_BILLMETHOD_BASIC_FIXEDQUANTITYSYSTEM		4	//구룡입문 정량
#define _VNBILLING_BILLMETHOD_PREMIUM_FIXEDAMOUNTSYSTEM		5	//구룡기행 정액
#define _VNBILLING_BILLMETHOD_PREMIUM_FIXEDQUANTITYSYSTEM	6	//구룡기행 정량

//#ifdef _XVNBILLINGSYSTEM

struct MSG_BILLING_INFO_REQ 
{
	_sHeader		header;
	u_char			ucMessage;
};

struct MSG_BILLING_INFO_ACK 
{
	_sHeader		header;
	u_char			ucMessage;

	UCHAR			ucType; // 2: 기획서 5-3
							// 3: 기획서 5-4 & 5-5

	union 
	{
		// ucType = 1
		char	dummy[32];

		// ucType = 2
		struct	EXPIRE {
			char	szBillingInfoRecordTime[12+1]; // 결제정보 수신 시간
			char	cBillMethod;
				// 1: 회원가입후 2일 24일간 무료
				// 2: 이벤트 무료
				// 3: 구룡입문 정액
				// 4: 구룡입문 정량
				// 5: 구룡기행 정액
				// 6: 구룡기행 정량
			DWORD	dwRemainTime;		// cBillMethod가 1,4,6일 경우 = 남은시간(sec)
							// 3,5일 경우 = Zero
			char	szExpireDate[12+1];	// 1,3,5일 경우 = YYYYMMDDHHMM
			char	dummy;
		} Expire;

		// ucType = 3
		struct EXPIRE_COUNTDOWN {
			UCHAR	ucCount;	// 0 ~ 5
			char	dummy[31];
		} ExpireCountDown;

	}; //end of union
};

//#endif