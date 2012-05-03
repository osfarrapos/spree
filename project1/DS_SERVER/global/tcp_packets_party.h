#pragma once

// GS <-> CS
#pragma pack( push, enter_member )
#pragma pack(1)


//==============================================================
class _Member 
{
public :

	_CHARAC *		pCharac ;				// 캐릭 포인터	
	u_short			uid ;
	u_short			usMoveCheckIndex ;		// 서버이동 유효성 검사 인덱스
	union {
		struct  _attribute {
			bool	bParty : 1 ;			// 파티 여부
			bool	bCaptian : 1 ;			// 방장 여부
			bool	bFriend : 1 ;
			u_char	ucAbsent : 2;			// 정상(0) 자리비움(1)
			bool	bTemp5 : 1 ;
			bool	bTemp6 : 1 ;
			bool	bIsMoveState : 1 ;
		} attribute ;
		char		cCheck ;
	};
public :
	_Member()
	{
		reset() ;		
	}
	~_Member()
	{
	}


	void reset()
	{
		pCharac = NULL ;				// 캐릭 포인터	
		uid = 0 ;
		usMoveCheckIndex = 0 ;		// 서버이동 유효성 검사 인덱스
		cCheck = 0 ;		
	}
};




//==============================================================
// Party message
#define SMSG_NO_CREATE_PARTY_REQ_GS_CS		(181)
#define SMSG_NO_CREATE_PARTY_RES_CS_GS		(182)
#define SMSG_NO_DELETE_PARTY_REQ_GS_CS		(183)
#define SMSG_NO_DELETE_PARTY_RES_CS_GS		(184)
#define SMSG_NO_JOIN_PARTY_REQ_GS_CS		(185)
#define SMSG_NO_JOIN_PARTY_RES_CS_GS		(186)
#define SMSG_NO_KICK_PARTY_REQ_GS_CS		(187)
#define SMSG_NO_KICK_PARTY_RES_CS_GS		(188)
#define SMSG_NO_MOVE_PARTY_REQ_GS_CS		(189)
#define SMSG_NO_MOVE_PARTY_RES_CS_GS		(190)
#define SMSG_NO_UPDATE_LEVEL_REQ_GS_CS		(191)
#define SMSG_NO_UPDATE_LEVEL_RES_CS_GS		(192)
#define SMSG_NO_PARTY_INFO_REQ_GS_CS		(193)
#define SMSG_NO_PARTY_INFO_RES_CS_GS		(194)
#define SMSG_NO_ENTRUST_REQ_GS_CS			(195)
#define SMSG_NO_ENTRUST_RES_CS_GS			(196)
#define SMSG_NO_CHANGE_ITEM_PROP			(197)		// 아이템 분배 방식 변경
#define SMSG_NO_CHANGE_EXP_PROP				(198)		// 경험치 분배 방식 변경
#define SMSG_NO_MOVE_COMPLETE				(199)
#define SMSG_NO_UPDATE_STATE				(200)		// 상태 변경 패킷 : 0 - 정상, 1 - 자리비움.
#define CMSG_NO_MASTER_AND_PUPIL			(201)	// 사제 관계 패킷.


//#define SMSG_NO_CREATE_PARTY_REQ_GS_CS
struct SMSG_CREATE_PARTY_REQ_GS_CS {
	u_short		usLength ;
	u_char		cMessage ;

	char		cServerNum ;
	u_short		uid1 ;			// 파티장	
	char		cCharacName1[13] ;
	short		sInnerLevel1 ;
	u_short		uid2 ;			// 파티원	
	char		cCharacName2[13] ;
	short		sInnerLevel2 ;
};

// #define SMSG_NO_CREATE_PARTY_RES_CS_GS
struct SMSG_CREATE_PARTY_RES_CS_GS {	// 생성 요청한 서버에만 전파
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;		// 0 : 파티 생성 실패, !0 : 파티 인덱스	
	short		sMoonpaProp ;		// 0낭인 1백도 -1 흑도

	u_short		uid1 ;	
	u_short		uid2 ;	
};

// #define SMSG_NO_DELETE_PARTY_REQ_GS_CS	
struct SMSG_DELETE_PARTY_REQ_GS_CS {	// 파티 삭제(해산) 요청
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;	
};
// #define SMSG_NO_DELETE_PARTY_RES_CS_GS	
struct SMSG_DELETE_PARTY_RES_CS_GS {	// 성공 : 파티 포함된 모든 서버에 전파, 실패 : 요청한 서버에만 전파
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	char		cResult ;			// 0: 실패, 1: 성공
};

// #define SMSG_NO_JOIN_PARTY_REQ_GS_CS
struct SMSG_JOIN_PARTY_REQ_GS_CS {
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	char		cServerNum ;
	u_short		uid ;
	char		cCharacName[13] ;
	short		sInnerLevel ;
};

// #define SMSG_NO_JOIN_PARTY_RES_CS_GS
struct SMSG_JOIN_PARTY_RES_CS_GS {	// 성공 : 파티 포함된 모든 서버에 전파, 실패 : 요청한 서버에만 전파
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;	
	short		sMoonpaProp ;		// 0낭인 1백도 -1 흑도

	char		cServerNum ;
	u_short		uid ;
	char		cCharacName[13] ;
	short		sInnerLevel ;
	char		cSlotNo	;			// -1 실패, 0 < 성공
};

// #define SMSG_NO_KICK_PARTY_REQ_GS_CS		
struct SMSG_KICK_PARTY_REQ_GS_CS {	
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	u_short		uid ;
	char		cSlotNo ;
	char		cMode ;			// 0: 자진, 1: 강제.. 2. 연결 끊김.. 또는 로그아웃..
};
// #define SMSG_NO_KICK_PARTY_RES_CS_GS		
struct SMSG_KICK_PARTY_RES_CS_GS {	// 성공 : 파티 포함된 모든 서버에 전파, 실패 : 요청한 서버에만 전파	
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	u_short		uid ;		
	char		cSlotNo ;
	char		cMode ;			// 0: 자진, 1: 강제.. 2. 연결 끊김.. 또는 로그아웃..
	char		cResult	;		//	-1:실패, 0:성공, 1:파티원 미달등으로 인해 해산, 2:파티장 위임
	char		cCapNo ;		// 자동 위임된 경우.. 새로 위임된 파티장의 Slot번호..
};

// #define SMSG_NO_MOVE_PARTY_REQ_GS_CS
struct SMSG_MOVE_PARTY_REQ_GS_CS {
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;	
	char		cServerNum ;
	char		cSlotNo ;
	u_short		uid ;	
};

// #define SMSG_NO_MOVE_PARTY_RES_CS_GS
struct SMSG_MOVE_PARTY_RES_CS_GS {	// 성공 : 파티 포함된 모든 서버에 전파, 실패 : 요청한 서버에만 전파
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	char		cServerNum ;	// 0 : Move 실패
	u_short		uid ;	
	char		cSlotNo ;
	char		cCharacName[13] ;
};

// #define SMSG_NO_UPDATE_LEVEL_REQ_GS_CS	
struct SMSG_UPDATE_LEVEL_REQ_GS_CS {	// 레벨정보 업데이트..
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	short		sInnerLevel ;
	u_short		uid ;	
	char		cSlotNo ;
};

// #define SMSG_NO_UPDATE_LEVEL_RES_CS_GS
struct SMSG_UPDATE_LEVEL_RES_CS_GS {		// SMSG_UPDATE_LEVEL_REQ_GS_CS 와 같은 구조체임
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	short		sInnerLevel ;	// 0 : level  update  실패
	u_short		uid ;	
	char		cSlotNo ;
};

// #define SMSG_NO_PARTY_INFO_REQ_GS_CS
struct SMSG_PARTY_INFO_REQ_GS_CS {
	u_short		usLength ;
	u_char		cMessage ;

	bool		bCreateAble ;	

	short		sPartyIndex ;
};

// #define SMSG_NO_PARTY_INFO_RES_CS_GS
struct SMSG_PARTY_INFO_RES_CS_GS {
	u_short		usLength ;
	u_char		cMessage ;

	bool		bCreateAble ;	

	short		sPartyIndex ;

	struct IN_MEMBER {	
		u_short	uid ;	
		char	cCharacName[13] ;
		char	cServerNum ;
		short	sInnerLevel ;
		char	cMoonpa ;
		char	cClass ;
#ifdef _PD_PARTY_INFO_CLASSGRADE_
		char	cClassGrade;
#endif
		int		iHonor ;
		

		union {
			struct  _attribute {
				bool	bParty : 1 ;
				bool	bCaptian : 1 ;	
				bool	bFriend : 1 ;
				u_char	ucAbsent : 2;			// 정상(0) 자리비움(1)
				bool	bTemp5 : 1 ;
				bool	bTemp6 : 1 ;
				bool	bIsMoveState : 1 ;
			} attribute ;
			char		cCheck ;
		} ;
	}  member[MAX_PARTY_MEMBER] ;	

};

//#define SMSG_NO_ENTRUST_REQ_GS_CS
struct SMSG_ENTRUST_REQ_GS_CS {
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	u_short		uid ;			//	0 : 자동 위임	
	char		cSlotNo ;
};
//#define SMSG_NO_ENTRUST_RES_CS_GS
struct SMSG_ENTRUST_RES_CS_GS {		// 성공 : 파티 포함된 모든 서버에 전파, 실패 : 요청한 서버에만 전파
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	u_short		uid ;			// 위임받은자 uid, 0 : 실패	
	char		cSlotNo ;

};

//#define SMSG_NO_CHANGE_ITEM_PROP			17
//#define SMSG_NO_CHANGE_EXP_PROP				16		// 경험치 분재 방식 변경
// Item, Exp 분배 모드 공동으로 사용한다.
struct SMSG_CHANGE_PROP {
	u_short		usLength ;
	u_char		cMessage ;
	short		sPartyIndex ;
	char		cProperty ;
};

//#define SMSG_NO_MOVE_COMPLETE					17
struct SMSG_MOVE_COMPLETE {
	u_short		usLength ;
	u_char		cMessage ;
	short		sPartyIndex ;
	u_short		uid ;
	char		cSlotNo ;
};

//SMSG_NO_UPDATE_STATE
struct SMSG_UPDATE_STATE
{
	u_short		usLength;
	u_char		cMessage;

	u_char		ucMode;		// 0 - 정상상태, 1 - 자리비움.

	short		sPartyIndex;
	u_char		slot;
	u_short		usValue;
};



// 210~225 : 이 범위안을 꼭 지킬 것.
#define SMSG_NO_OR_SOME_REQ_GS_DS			210
#define SMSG_NO_OR_INFO_RES_DS_GS			211
#define SMSG_NO_OR_CREATE_REQ_GS_DS			212
#define SMSG_NO_OR_CTRL						(213)			// 20080630 : OR_CTRL 로 변경.
#define SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS 214
#define SMSG_NO_OR_RESULT_DS_GS				215
#define SMSG_NO_OR_NOTICE_EDIT_GS_DS_GS		216
//#define SMSG_NO_ALLY_BASE					217		// 동맹 기본 패킷. 아래에 있음.

//#define SMSG_NO_OR_SOME_REQ_GS_DS
struct SMSG_OR_SOME_REQ_GS_DS
{
	u_short		usLength ;
	u_char		cMessage ;

	int			iIndex ;
	char		cCode ;		
	enum
	{
		_OR_SOME_REQ_INFO_,
		_OR_SOME_REQ_DELETE,
	} ;
};

//#define SMSG_NO_OR_INFO_RES_DS_GS
struct SMSG_OR_INFO_RES_DS_GS
{
	u_short		usLength ;
	u_char		cMessage ;

	_Organization	_organization ;
	char		cCode ;
	enum{
		_INFO_REQ_,
		_CREATE_REQ_,
		_CREATE_FAIL_USING_NAME_,
		_CREATE_FAIL_ETC_,
	} ;
};

//#define SMSG_NO_OR_CREATE_REQ_GS_DS
struct SMSG_OR_CREATE_REQ_GS_DS
{
	u_short		usLength ;
	u_char		cMessage ;

	_Organization	_organization ;
};

//SMSG_NO_OR_BASE_INFO_UPDATE
struct SMSG_OR_BASE_INFO_UPDATE
{
	u_short		usLength ;
	u_char		cMessage ;

	_OrganizationBase _organizationBase ;
};

//#define SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS
struct SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS
{
	u_short		usLength ;
	u_char		cMessage ;

	int			iIndex ;
	_Unit		_unit ;
	char		cCode ;
	enum{
		_UNIT_CONTROL_CREATE_,
		_UNIT_CONTROL_DELETE_,
		_UNIT_CONTROL_KICK_,
		_UNIT_CONTROL_UPDATE_,
		_UNIT_CONTROL_ENTRUST_,
	} ;

	_OutTime	outTime ;

};


//#define SMSG_NO_OR_RESULT_DS_GS
struct SMSG_OR_RESULT_DS_GS
{
	u_short		usLength ;
	u_char		cMessage ;
		
	int			iIndex ;
	int			iCode ;
	enum{
		_OR_RESULT_DELETE_,
	} ;
};



//#define SMSG_NO_OR_NOTICE_EDIT_GS_DS_GS		216
struct SMSG_OR_NOTICE_EDIT 
{
	u_short		usLength ;
	u_char		cMessage ;

	int			iIndex ;
	char		cNotice[_OR_NOTICE_SIZE] ;	/*#define _OR_NOTICE_SIZE		300*/
};



#ifdef _PD_ALLY_ORG_STEP_1_

#define SMSG_NO_ALLY_BASE					(217)		// 동맹 기본 패킷.
struct SMSG_ALLY_BASE 
{
	u_short			usLength;
	u_char			ucMessage;

	u_char			ucMode;					// 모드에 따라 구분하여 사용할 것.

	SMSG_ALLY_BASE( u_short len, u_char mode )
		: usLength(len), ucMessage(SMSG_NO_ALLY_BASE), ucMode(mode)
	{
	}
};

// SMSG_ALLY_BASE::ucMode
enum EN_DS_ALLY_PACKET_MODE
{
	en_ds_ally_request_create=0,		// 0 생성 요청
	en_ds_ally_create_success,			// 1 생성 성공 : 요청한 서버에만 전달.
	en_ds_ally_create_fail,				// 2 생성 실패 : 패킷에 실패 원인이 써있다.
	en_ds_ally_create_notice,			// 3 모든 서버에 알린다. GS에서 모든 멤버에 알리는 식으로 사용된다.

	en_ds_ally_request_info=10,			// 10 정보요청
	en_ds_ally_info,					// 11 위 정보 요청의 응답.
	en_ds_ally_no_info,					// 12 그런 동맹 없다. 
	en_ds_ally_request_all_ally,		// 13 모든 영웅단의 정보를 요청한다. 시작시에.
	en_ds_ally_answer_info,				// 14 13번의 응답.

	en_ds_ally_request_add_org=20,		// 20 동맹 추가 요청
	en_ds_ally_add_success,				// 21 동맹 추가 요청 성공
	en_ds_ally_add_fail,				// 22 동맹 추가 요청 실패.
	en_ds_ally_add_notice,				// 23

	en_ds_ally_request_leave=30,		// 30 동맹 탈퇴 요청
	en_ds_ally_leave_success,			// 31 동맹 탈퇴 성공
	en_ds_ally_leave_fail,				// 32 동맹 탈퇴 실패 - 이유가... 없을수도.

	en_ds_ally_request_breakup=40,		//  해산 요청
	en_ds_ally_breakup_success,			// 해산 요청 성공
	en_ds_ally_breakup_fail,			// 해산 실패.

	en_ds_ally_request_banish=50,		// 추방 요청. : 회주만이 할 수 있다.
	en_ds_ally_banish_success,			// 정상
	en_ds_ally_banish_fail,				// 만약, 영웅단 정보가 이상하면.. 
};

//en_ds_ally_request_create=0,		// 0 생성 요청
struct SMSG_ALLY_REQUEST_CREATE : public SMSG_ALLY_BASE
{
	int				iOrg1;			//
	int				iOrg2;			//

	char			cName1[13];		// 영웅단주의 이름.
	char			cName2[13];		// 영웅단주의 이름.

	SMSG_ALLY_REQUEST_CREATE()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_CREATE), en_ds_ally_request_create )
	{
	}
};


//en_ds_ally_create_success,			// 1 생성 성공 : 정보가 포함된다.
struct SMSG_ALLY_CREATE_SUCCESS : public SMSG_ALLY_BASE
{
	int				iAllyIndex;		// 생성된 영웅단의 정보

	int				iOrg1;
	int				iOrg2;

	char			cOrgName1[en_or_name_length+1];	// 동맹의 이름.
	char			cOrgName2[en_or_name_length+1];	// 동맹의 이름.

	char			cMasterName[13];		// 동맹 회주의 이름.

	short			sProperty;		// -1/1 흑도/백도

	char			cName1[13];		// 영웅단주의 이름.
	char			cName2[13];		// 영웅단주의 이름.

	SMSG_ALLY_CREATE_SUCCESS()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_CREATE_SUCCESS), en_ds_ally_create_success )
	{
	}
};

// en_ds_ally_create_fail,				// 2 생성 실패 : 패킷에 실패 원인이 써있다.
struct SMSG_ALLY_CREATE_FAIL : public SMSG_ALLY_BASE
{
	int				iOrg1;
	int				iOrg2;

	char			cName1[13];		// 영웅단주의 이름.
	char			cName2[13];		// 영웅단주의 이름.

	SMSG_ALLY_CREATE_FAIL()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_CREATE_FAIL), en_ds_ally_create_fail )
	{
	}
};

// en_ds_ally_create_notice
struct SMSG_ALLY_CREATE_NOTICE : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	int				iOrg1;
	int				iOrg2;

	short			sProperty;

	char			cOrgName1[en_or_name_length+1];
	char			cOrgName2[en_or_name_length+1];

	char			cMasterName[13];

	SMSG_ALLY_CREATE_NOTICE()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_CREATE_NOTICE), en_ds_ally_create_notice )
	{
	}
};

//en_ds_ally_request_info,			//  정보요청
struct SMSG_ALLY_REQUEST_INFO : public SMSG_ALLY_BASE
{
	char			cName[13];		// 

	int				iAllyIndex;

	SMSG_ALLY_REQUEST_INFO()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_INFO), en_ds_ally_request_info )
	{
	}
};


struct SMSG_ALLY_INFO : public SMSG_ALLY_BASE
{
	char			cName[13];		// 

	int				iAllyIndex;
	short			sProperty;
	// 이하 5개의 영웅단 정보가 들어간다.
	__ally_client_info		org[_NDD_MAX_ORG_IN_ALLY_];

	char			cMasterName[13];		// 동맹 회주의 이름.

	SMSG_ALLY_INFO()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_INFO), en_ds_ally_info )
	{
	}
};

//en_ds_ally_no_info,					// 5 그런 동맹 없다. 
struct SMSG_ALLY_NO_INFO : public SMSG_ALLY_BASE
{
	char			cName[13];		// 

	int				iAllyIndex;
	u_char			ucError;		// 에러 코드.


	SMSG_ALLY_NO_INFO()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_NO_INFO), en_ds_ally_no_info )
	{
	}
};

//en_ds_ally_request_all_ally
struct SMSG_ALLY_REQUEST_ALL_ALLY : public SMSG_ALLY_BASE
{
	// 내용 필요 없슴.
	SMSG_ALLY_REQUEST_ALL_ALLY()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_ALL_ALLY), en_ds_ally_request_all_ally )
	{
	}
};

// en_ds_ally_answer_info
struct SMSG_ALLY_ANSWER_INFO : public SMSG_ALLY_BASE
{
	u_char			ucContinueInfo;	// 0 - End of Infor. 1 - Continue..

	int				iAllyIndex;
	short			sProperty;
	// 이하 5개의 영웅단 정보가 들어간다.
	__ally_client_info		org[_NDD_MAX_ORG_IN_ALLY_];

	char			cMasterName[13];		// 동맹 회주의 이름.

	SMSG_ALLY_ANSWER_INFO()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_ANSWER_INFO), en_ds_ally_answer_info )
	{
		ucContinueInfo = 0;		// 기본값을 넣어줘야 기존 코드가 그대로 동작할 것이다.
	}
};


// en_ds_ally_request_add_org,			// 20 동맹 추가 요청
struct SMSG_ALLY_REQUEST_ADD_ORG : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	int				iOrgAdd;		// 추가되는 영웅단 인덱스.

	char			cName[13];		// 영웅단주의 이름.
	char			cNameAdd[13];		// 영웅단주의 이름.

	SMSG_ALLY_REQUEST_ADD_ORG()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_ADD_ORG), en_ds_ally_request_add_org )
	{
	}
};


//en_ds_ally_add_success,				// 21 동맹 추가 요청 성공
struct SMSG_ALLY_ADD_SUCCESS : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	u_char				ucSlot;			// 0 ~ 4. 전서버의 동맹 정보를 동일하게 설정하기 위해 사용한다.
	__ally_client_info	info;

	char			cName[13];		// 영웅단주의 이름.
	char			cNameAdd[13];		// 영웅단주의 이름.

	SMSG_ALLY_ADD_SUCCESS()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_ADD_SUCCESS), en_ds_ally_add_success )
	{
	}
};

//en_ds_ally_add_fail,				// 22 동맹 추가 요청 실패.
struct SMSG_ALLY_ADD_FAIL : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	int				iOrgAdd;

	u_char			ucError;

	char			cName[13];		// 영웅단주의 이름.
	char			cNameAdd[13];		// 영웅단주의 이름.

	SMSG_ALLY_ADD_FAIL()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_ADD_FAIL), en_ds_ally_add_fail )
	{
	}
};

//en_ds_ally_add_notice,				// 23
struct SMSG_ALLY_ADD_NOTICE : public SMSG_ALLY_BASE
{
	int				iAllyIndex;		// 동맹 인덱스.

	int				iOrg;			// 추가된 영웅단의 인덱스
};

//en_ds_ally_request_leave,			// 동맹 탈퇴 요청
struct SMSG_ALLY_REQUEST_LEAVE : public SMSG_ALLY_BASE
{
	int				iAllyIndex;
	int				iOrgLeave;

	char			cName[13];

	SMSG_ALLY_REQUEST_LEAVE()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_LEAVE), en_ds_ally_request_leave )
	{
	}
};
//en_ds_ally_leave_success,			// 동맹 탈퇴 성공
struct SMSG_ALLY_LEAVE_SUCCESS : public SMSG_ALLY_BASE
{
	int				iAllyIndex;
	int				iOrgLeave;

	char			cName[13];

	SMSG_ALLY_LEAVE_SUCCESS()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_LEAVE_SUCCESS), en_ds_ally_leave_success )
	{
	}
};
//en_ds_ally_leave_fail,				// 동맹 탈퇴 실패 - 이유가... 없을수도.
struct SMSG_ALLY_LEAVE_FAIL : public SMSG_ALLY_BASE
{
	int				iAllyIndex;
	int				iOrgLeave;

	u_char			ucError;

	char			cName[13];

	SMSG_ALLY_LEAVE_FAIL()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_LEAVE_FAIL), en_ds_ally_leave_fail )
	{
	}
};

//en_ds_ally_request_breakup,			//  해산 요청
struct SMSG_ALLY_REQUEST_BREAKUP : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	u_char			ucBreakupReason;	// 0 - request

	char			cName[13];

	SMSG_ALLY_REQUEST_BREAKUP()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_BREAKUP), en_ds_ally_request_breakup )
	{
	}
};
//en_ds_ally_breakup_success,			// 해산 요청 성공
struct SMSG_ALLY_BREAKUP_SUCCESS : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	u_char			ucBreakupReason;	// 0 - request, 1 - leave

	char			cName[13];

	SMSG_ALLY_BREAKUP_SUCCESS()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_BREAKUP_SUCCESS), en_ds_ally_breakup_success )
	{
	}
};
//en_ds_ally_breakup_fail,			// 해산 실패.
struct SMSG_ALLY_BREAKUP_FAIL : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	u_char			ucError;		//

	char			cName[13];

	SMSG_ALLY_BREAKUP_FAIL()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_BREAKUP_FAIL), en_ds_ally_breakup_fail )
	{
	}
};

//en_ds_ally_request_banish=50,		// 추방 요청. : 회주만이 할 수 있다.
struct SMSG_ALLY_REQUEST_BANISH : public SMSG_ALLY_BASE
{
	int				iAllyIndex;
	int				iOrgIndex;

	char			cName[13];

	SMSG_ALLY_REQUEST_BANISH()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_BANISH), en_ds_ally_request_banish )
	{
	}
};


//en_ds_ally_banish_success,			// 정상
struct SMSG_ALLY_BANISH_SUCCESS : public SMSG_ALLY_BASE
{
	int				iAllyIndex;
	int				iOrgIndex;

	char			cName[13];

	SMSG_ALLY_BANISH_SUCCESS()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_BANISH_SUCCESS), en_ds_ally_banish_success )
	{
	}
};

//en_ds_ally_banish_fail,				// 만약, 영웅단 정보가 이상하면.. v
struct SMSG_ALLY_BANISH_FAIL : public SMSG_ALLY_BASE
{
	int				iAllyIndex;
	int				iOrgIndex;

	char			cName[13];

	SMSG_ALLY_BANISH_FAIL()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_BANISH_FAIL), en_ds_ally_banish_fail )
	{
	}
};

#endif	// ally

#define SMSG_NO_PARTY_CHAT					240		// 파티채팅
#define SMSG_NO_OR_CHAT						241		// 분파 채팅

//#define SMSG_NO_PARTY_CHAT
struct SMSG_PARTY_CHAT {
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	char		cName[13] ;
	char		cChatMsg[81] ;
};

struct SMSG_OR_CHAT 
{
	u_short		usLength ;
	u_char		cMessage ;

#ifdef _PD_ALLY_ORG_STEP_1_
	int			iAllyIndex;
	int			iOrgIndex;
#else
	int			index ;
#endif
	char		cName[13] ;
	char		cChatMsg[81] ;
};

#ifdef _PD_CASTLE_STEP_1_
//////////////////////////////////////////////////////////////////////////
// 장원전 관련 패킷.
//////////////////////////////////////////////////////////////////////////
#include "../Cs/CastleStructure.h"

#define CMSG_NO_COMBAT_PACKET					(218)		// 동맹 기본 패킷.

struct CMSG_COMBAT_PACKET
{
	u_short				usLength;
	u_char				ucMessage;

	u_char				ucMode;			// 바로 아래의 EN_DS_COMBAT

	CMSG_COMBAT_PACKET( u_short size, u_char mode )
		: usLength(size), ucMessage(CMSG_NO_COMBAT_PACKET), ucMode(mode)
	{
	}
};

enum EN_DS_COMBAT
{
	en_dscb_req_manor_information=0,	// 0 스케줄 정보 요청.
	en_dscb_manor_information,			// 스케줄 정보 : 받았을 때, 겹치지 않는다면, 항상 추가하면 된다. 

	en_dscb_req_change_time,			// 장원전 시간 변경 요청. : 실패 패킷 있음.
	en_dscb_time_changed,				// 장원전 스케줄이 변경되었다. 통보 패킷.

	en_dscb_challenge_insert,			// 도전 추가 : 통보
	en_dscb_challenge_delete,			// 도전 삭제 : 통보

	en_dscb_challenge_reset,			// 도전 정보를 모두 삭제한다.

	en_dscb_owner_info_changed,			// 장원 정보 변경.
	
	en_dscb_combat_start,				// 장원전 시작
	en_dscb_combat_end,					// 장원전 끝.

    en_dscb_record_info_req = 50,		//GS에서  단의 정보를 요청
	en_dscb_record_info_res,			// 모든 정보 리턴
	en_dscb_record_update,				// 장원전이 끝나면 장원전에 참가한 단들 업데이트
	en_dscb_record_reset,				// 장원전이 시작 되면 


	en_dscb_error=100,					// 각종 에러.
};

// en_dscb_req_manor_information
struct CMSG_CB_REQ_MANOR_INFORMATION : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;		// 0 - all info. etc - a info.

	_sND_TimeUnit			DefaultSchedule[3];

	/*
	요청할 때, 기본 스케줄 정보를 채워서 요청하고,
	DS 에서 이를 기본으로 스케줄을 초기화해서 보내준다.
	*/

	CMSG_CB_REQ_MANOR_INFORMATION()
		: CMSG_COMBAT_PACKET(sizeof(CMSG_CB_REQ_MANOR_INFORMATION), en_dscb_req_manor_information )
	{
	}
};

// en_dscb_manor_information
struct CMSG_CB_MANOR_INFORMATION : public CMSG_COMBAT_PACKET
{
	// 여러 장원의 정보가 가야 하므로, 연속해서 보내고, 마지막에는 1로 보낸다.
	u_char					ucIsEnd;			// 0 : continue info. 1 : end info.

	u_char					ucMapID;			// 존 인덱스 : 0 이면, 장원 정보가 하나도 없다는 의미다.

	_sCastleInfo			castleInfo;			// 장원 정보. 채우는 함수도 만들어야 한다.

	u_char					ucCount;			// 도전자 개수.
	__cb_challenge_info		challengeList[max_count_of_challenge_list*2];		//

	CMSG_CB_MANOR_INFORMATION()
		: CMSG_COMBAT_PACKET(sizeof(CMSG_CB_MANOR_INFORMATION), en_dscb_manor_information )
	{
	}

	inline void set_length()
	{
		usLength = sizeof(CMSG_CB_MANOR_INFORMATION) - ( sizeof(__cb_challenge_info) * (max_count_of_challenge_list*2 - ucCount) );
	}
};

// en_dscb_req_change_time,			// 장원전 시간 변경 요청.
struct CMSG_CB_REQ_CHANGE_TIME : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;

	_sND_TimeUnit			Schedule[3];		// 

	CMSG_CB_REQ_CHANGE_TIME()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_REQ_CHANGE_TIME), en_dscb_req_change_time )
	{
	}
};

// en_dscb_time_changed,			장원전 스케줄이 변경되었다. 통보 패킷.
struct CMSG_CB_TIME_CHANGED : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;			// 해당 맵.

	_sND_TimeUnit			Schedule[3];		// 스케줄

	CMSG_CB_TIME_CHANGED()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_TIME_CHANGED), en_dscb_time_changed )
	{
	}
};

// en_dscb_challenge_insert,		도전 추가 : 통보 : 
struct CMSG_CB_CHALLENGE_INSERT : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;

	__cb_challenge_info		info;		// 도전자 정보.

	CMSG_CB_CHALLENGE_INSERT()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_CHALLENGE_INSERT), en_dscb_challenge_insert )
	{
	}
};

// en_dscb_challenge_delete,		도전 삭제 : 통보
struct CMSG_CB_CHALLENGE_DELETE : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;

	__cb_challenge_info		info;

	CMSG_CB_CHALLENGE_DELETE()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_CHALLENGE_DELETE), en_dscb_challenge_delete )
	{
	}
};


//en_dscb_challenge_reset,			// 도전 정보를 모두 삭제한다.
struct CMSG_CB_CHALLENGE_RESET : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;

	CMSG_CB_CHALLENGE_RESET()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_CHALLENGE_RESET), en_dscb_challenge_reset )
	{
	}
};


// en_dscb_owner_info_changed : gs->ds, ds->gs
// 받으면, ChangeOwner 에 따라, 소유를 변경하고
// 도전 목록을 초기화한다.
struct CMSG_CB_OWNER_INFO_CHANGED : public CMSG_COMBAT_PACKET
{
	u_char					ucChangeOwner;		// 0 - not changed, 1 - changed

	u_char					ucMapID;

	_sCastleInfo			castleInfo;

	CMSG_CB_OWNER_INFO_CHANGED()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_OWNER_INFO_CHANGED), en_dscb_owner_info_changed )
	{
	}
};


// en_dscb_combat_start,				// 장원전 시작
struct CMSG_CB_COMBAT_START : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;

	CMSG_CB_COMBAT_START()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_COMBAT_START), en_dscb_combat_start )
	{
	}
};


// en_dscb_combat_end,					// 장원전 끝.
struct CMSG_CB_COMBAT_END : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;

	CMSG_CB_COMBAT_END()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_COMBAT_END), en_dscb_combat_end )
	{
	}
};


// en_dscb_error
struct CMSG_CB_ERROR : public CMSG_COMBAT_PACKET
{
	u_char					ucPrevMode;			// 

	u_char					ucErrorCode;		// 


	CMSG_CB_ERROR()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_ERROR), en_dscb_error )
	{
	}
};


#endif	// _PD_CASTLE_STEP_1_


#ifdef _PD_CASTLE_STEP_2_
//=====================================
//
//장원 세금 시스템 패킷
//
//=====================================
//1.장원 세금 수입 저장 요청/응답
//2.장원 세금 총 수입 정보 요청/응답
//3.장원 세금 수입 지출 요청/응답
//=====================================
#define CMSG_NO_MANOR_TAX	219

struct CMSG_MANOR_TAX
{
	u_short		usLength;
	u_char		ucMessage;

	u_char		ucMode;		//en_manor_tax_pkt_type

	CMSG_MANOR_TAX(u_short length, u_char mode)
		:usLength(length), ucMessage(CMSG_NO_MANOR_TAX), ucMode(mode)
	{}

};

enum EN_MANOR_TAX_PKT_GS_DS
{
	en_manor_tax_info_req_ds_gs=0,		// DS 에서 GS 로 쌓여진 세금 정보 요청
	en_manor_tax_info_gs_ds,			// 10초후 이 패킷을 정리해 DB에 저장, GS에 모든 정보 다시 뿌려줌.

	en_manor_tax_req_start_compute_gs_ds,	// 장원이 한시간 마다 세금 집계 요청.
	en_manor_tax_req_total_gs_ds,			// 약 30초 후, 전체 세금을 요청한다.
	en_manor_tax_notice_total_tax_ds_gs,	// 전체 세금을 알려줌.
	en_manor_tax_reset_common_tax,			// 거둬들인 세금을 초기화한다.

	en_manor_tax_set_total_money=30,		// 이건 필요한건가...
	en_manor_tax_req_take,					// 클라이언트에서 요청왔다. DS 에 요청하자.
	en_manor_tax_answer_take,				// 위에꺼 응답.


	en_manor_tax_challenge_gs_ds = 40,		//도전 세금
};

// 1시간 마다 정보 요청
//en_manor_tax_info_req_ds_gs
struct CMSG_MANOR_TAX_INFO_REQ_DS_GS : public CMSG_MANOR_TAX
{
	//u_char		ucMapID;		// 맵 번호
	//u_char		ucResult;		// 10초 후에 어디서 온 정보 인지 알기 위해서

	CMSG_MANOR_TAX_INFO_REQ_DS_GS()
		:CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_INFO_REQ_DS_GS), en_manor_tax_info_req_ds_gs)
	{}
};


// en_manor_tax_info_gs_ds
struct CMSG_MANOR_TAX_INFO_GS_DS : public CMSG_MANOR_TAX
{
	u_char		ucMapID;
	u_char		ucManorLevelRate;	// 장원이 있는 맵 세율
	u_char		ucCommonLevelRate;	// 공통 세율
	u_char		ucRegionOrGlobal;	// 지역/공통 구분
	u_int		uiTotalMoney;		// 총 수입 정보
	u_char		ucResult;			// GS에서 모든 정보가 다 왔는지 알기 위해서

	CMSG_MANOR_TAX_INFO_GS_DS()
		:CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_INFO_GS_DS), en_manor_tax_info_gs_ds)
	{}
};

//	en_manor_tax_req_start_compute_gs_ds,		// 장원이 한시간 마다 세금 집계 요청.
struct CMSG_MANOR_TAX_REQ_START_COMPUTE_GS_DS : public CMSG_MANOR_TAX
{
	u_char		ucMapID;			// 맵/장원 아이디.

	CMSG_MANOR_TAX_REQ_START_COMPUTE_GS_DS()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_REQ_START_COMPUTE_GS_DS), en_manor_tax_req_start_compute_gs_ds )
	{
	}
};


//	en_manor_tax_req_total_gs_ds,		// 약 30초 후, 전체 세금을 요청한다.
struct CMSG_MANOR_TAX_REQ_TOTAL_GS_DS : public CMSG_MANOR_TAX
{
	u_char		ucMapID;			// 맵/징원 아이디.

	CMSG_MANOR_TAX_REQ_TOTAL_GS_DS()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_REQ_TOTAL_GS_DS), en_manor_tax_req_total_gs_ds )
	{
	}
};

//	en_manor_tax_reset_common_tax
struct CMSG_MANOR_TAX_RESET_COMMON_TAX : public CMSG_MANOR_TAX
{
	CMSG_MANOR_TAX_RESET_COMMON_TAX()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_RESET_COMMON_TAX), en_manor_tax_reset_common_tax )
	{
	}
};


//	en_manor_tax_set_total_money=30,		// 이건 필요한건가...
struct CMSG_MANOR_TAX_SET_TOTAL_MONEY : public CMSG_MANOR_TAX
{
	u_char		ucMapID;			// 장원 아이디.

	u_int		uiTotalMoney;		// 
	u_int		uiPartMoney;		// 

	u_char		ucType;		// 0 - from tax, 1 - from challenge, 2 - Take.

	CMSG_MANOR_TAX_SET_TOTAL_MONEY()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_SET_TOTAL_MONEY), en_manor_tax_set_total_money )
	{
	}
};


//	en_manor_tax_req_take,					// 클라이언트에서 요청왔다. DS 에 요청하자.
struct CMSG_MANOR_TAX_REQ_TAKE : public CMSG_MANOR_TAX
{
	char		cCharacName[en_charac_name_length+1];

	u_char		ucMapID;			// 장원 아이디.

	u_int		uiRequestMoney;

	CMSG_MANOR_TAX_REQ_TAKE()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_REQ_TAKE), en_manor_tax_req_take )
	{
	}
};


//	en_manor_tax_answer_take,				// 위에꺼 응답.
struct CMSG_MANOR_TAX_ANSWER_TAKE : public  CMSG_MANOR_TAX
{
	char		cCharacName[en_charac_name_length+1];

	u_char		ucMapID;			//

	u_char		ucResult;			// 결과. 
	u_int		uiTakeMoeny;			// 성공시 : 가져가려던 돈.
	u_int		uiLeftPartMoney;

	/*	ucResult
	0 - Success
	1 - 돈이 없다.
	2 - 장주가 아니다.
	3 - 소유한 장원이 없다.
	*/

	CMSG_MANOR_TAX_ANSWER_TAKE()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_ANSWER_TAKE), en_manor_tax_answer_take )
	{
	}
};

//en_manor_tax_challenge_gs_ds = 40,		//도전 세금
struct CMSG_MANOR_TAX_CHALLENGE_GS_DS : public CMSG_MANOR_TAX
{
	u_char	ucMapID;
	u_int	uiChallengeTax;

	CMSG_MANOR_TAX_CHALLENGE_GS_DS()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_CHALLENGE_GS_DS), en_manor_tax_challenge_gs_ds)
	{
	}
};

#endif //_PD_CASTLE_STEP_2_

#ifdef _PD_CASTLE_STEP_2_

// en_dscb_record_info_req
struct CMSG_RECORD_INFORMATION_REQ : public CMSG_COMBAT_PACKET
{
	int		m_org_index;		// 0 - all info. etc - a info.

	CMSG_RECORD_INFORMATION_REQ()
		: CMSG_COMBAT_PACKET(sizeof(CMSG_RECORD_INFORMATION_REQ), en_dscb_record_info_req )
	{
	}
};

// en_dscb_record_info_res
struct CMSG_RECORD_INFORMATION_DS_GS : public CMSG_COMBAT_PACKET
{
	int						m_org_index;
	_sCombatRecord			m_combat_record[MAX_MANOR];
	short					m_combat_org_count;		// 전 장원에서 싸운 단의 갯수 
	_sOrgRecord				m_org_record[MAX_ORG_COUNT];


	CMSG_RECORD_INFORMATION_DS_GS()
		: CMSG_COMBAT_PACKET(sizeof(CMSG_RECORD_INFORMATION_DS_GS), en_dscb_record_info_res )
	{
	}

	inline void set_length()
	{
		usLength = sizeof(CMSG_RECORD_INFORMATION_DS_GS) - ( sizeof(_sOrgRecord) * (MAX_ORG_COUNT - m_combat_org_count) );
	}
};

//en_dscb_record_update
struct CMSG_RECORD_UPDATE_GS_DS : public CMSG_COMBAT_PACKET
{
	int						m_org_index;
	_sCombatRecord			m_combat_record[MAX_MANOR];
	short					m_combat_org_count;		// 전 장원에서 싸운 단의 갯수 
	_sOrgRecord				m_org_record[MAX_ORG_COUNT];
    
	CMSG_RECORD_UPDATE_GS_DS()
		: CMSG_COMBAT_PACKET(sizeof(CMSG_RECORD_UPDATE_GS_DS), en_dscb_record_update )
	{
	}
	inline void set_length()
	{
		usLength = sizeof(CMSG_RECORD_UPDATE_GS_DS) - ( sizeof(_sOrgRecord) * (MAX_ORG_COUNT - m_combat_org_count) );
	}
};
//en_dscb_record_reset
struct CMSG_RECORD_RESET : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;    

	CMSG_RECORD_RESET()
		: CMSG_COMBAT_PACKET(sizeof(CMSG_RECORD_RESET), en_dscb_record_reset )
	{
	}	
};

#endif	//_PD_CASTLE_STEP_2_



#ifdef _PD_MASTER_PUPIL_

//////////////////////////////////////////////////////////////////////////
//	Master & Pupil : GS <=> DS 간 패킷.
//////////////////////////////////////////////////////////////////////////
struct CMSG_MASTER_AND_PUPIL
{
	u_short				usLength;
	u_char				cMessage;

	u_char				ucMP_Mode;		// EN_DG_MP_PACKET_MODE

	CMSG_MASTER_AND_PUPIL( u_short size, u_char mode )
		: usLength(size), cMessage(CMSG_NO_MASTER_AND_PUPIL), ucMP_Mode(mode)
	{
	}
};


enum EN_DG_MP_PACKET_MODE
{
	// 0 - 50 : 사제 관계 정보 관련 패킷.
	en_dg_mp_mode_info_master=0,				// 사제 관계 정보. ( req 의 응답이다. )
	en_dg_mp_mode_info_req=1,					// 사제 정보 요청.
	en_dg_mp_mode_no_info=2,					// 만약 사제 정보가 없다면, 이 패킷으로 알린다.

	en_dg_mp_mode_add_point=3,					// 포인트 증가 요청.
	en_dg_mp_mode_update_point=4,				// 포인트 변경 알림 패킷.
	en_dg_mp_mode_add_kill_count=5,				// 킬 카운트 증가 요청.
	en_dg_mp_mode_update_kill_count=6,			// 킬 카운트 변경 알림 패킷.
	en_dg_mp_mode_update_rank=7,				// GS->DS->GS 랭크 업데이트

	en_dg_mp_mode_create_mp_req=8,				// 사제 정보 생성 요청.
	en_dg_mp_mode_create_success=9,				// 사제 정보 생성 성공.
	en_dg_mp_mode_create_fail=10,				// 사제 정보 생성 실패.

	en_dg_mp_mode_notice_connection=11,			// 접속/끊김 알림 패킷.
	en_dg_mp_mode_change_master_nick_req=12,	// 사호 변경 알림.
	en_dg_mp_mode_update_master_nick=13,		// 사호 변경하라.

	en_dg_mp_mode_update_pupil_position=14,		// 제자의 서열이 변했다. (변경사항만 전달)

	en_dg_mp_mode_chat=20,						// 사제간 그룹 채팅.

	en_dg_mp_mode_info_error=50,				// 정보 관련 에러 메시지.


	// 51 - 80 : 사제 관계 맺기 관련 패킷.
	en_dg_mp_mode_gd_apply_for_pupil=51,
	en_dg_mp_mode_dg_apply_for_pupil=52,
	en_dg_mp_mode_apply_for_pupil_error=53,		// 신청자에게 돌려준다. 
	en_dg_mp_mode_answer_yes=54,				// '예'를 선택했다.
	en_dg_mp_mode_answer_apply_to_pupil_no=55,	// '아니오'를 선택했을 때.
	en_dg_mp_mode_complete_join_pupil=56,		// 신청자에게 전달.
	en_dg_mp_mode_add_new_pupil=57,				// 스승 및 다른 제자에게 전달.
	en_dg_mp_mode_master_simple_info_to_pupil=58,	// 신정자에게 스승의 간략 정보를 알려준다.

	en_dg_mp_mode_apply_error=80,				// 관계 관련 에러.

	// 81 - 100 : 파문 및 하산.
	en_dg_mp_mode_strike_pupil_req=81,			// 스승 : 파문 요청.
	en_dg_mp_mode_strike_result=82,				// 파문 성공시 스승 및 다른 제자에게.
	en_dg_mp_mode_you_struck=83,				// 파문 캐릭터에게 보낸다.
	
	en_dg_mp_mode_strike_error=90,				// 파문 실패시 에러 정보.



	en_dg_mp_mode_leave_req=91,					// 하산 요청.
	en_dg_mp_mode_you_left=92,					// 하산 성공시 신청 제자에게 감.
	en_dg_mp_mode_left_pupil=93,				// 스승 및 다른 제자에게 하산을 알린다.

	en_dg_mp_mode_left_error=100,				// 하산 과정의 에러.

	// 메모리 추가/삭제 정보 교환.
	en_dg_mp_mode_del_from_memory=101,			// 메모리에서 내림.

	// 151 ~ 170 : 사제 신청 거부 및 게시판 관리
	en_dg_mp_mode_accept_pupil_mode = 151,		// 사제 신청 거부 관련.
	en_dg_mp_mode_board_initial_req = 152,		// 처음 요청할 때.( 게시판을 열 때 )
	en_dg_mp_mode_board_initial_open = 153,		// 처음 요청시의 응답. ( 1 페이지 정보가 포함된다. )

	en_dg_mp_mode_board_req_page=154,			// 게시판 정보 요청. 페이지 단위.
	en_dg_mp_mode_board_page_info=155,			// 게시판 정보.

	en_dg_mp_mode_board_error = 170,			// 에러.

};



// 0 - 50 : 사제 관계 정보 관련 패킷.
//	en_dg_mp_mode_info_master=0,				// 사제 관계 정보. ( req 의 응답이다. )
struct CMSG_MP_INFO_MASTER : public CMSG_MASTER_AND_PUPIL
{
	char					cFromName[en_charac_name_length+1];

	int						iMasterIndex;			// 

	//!// 아래로 사제 정보가 온다.
	u_int					uiTotalKillCount;
	u_int					uiMasterKillCount;
	u_int					uiMasterTotalPoint;
	u_int					uiMasterCurPoint;

	u_char					ucMasterRank;

	u_char					ucMasterSex;
	u_char					ucMasterInnerLevel;
	u_char					ucMasterMoonpa;
	u_char					ucMasterClass;
	u_char					ucMasterClassgrade;

	u_char					ucMasterConZone;
	u_char					ucMasterConnected;
	u_char					ucBonusSkillStep;

	u_char					ucApplyMasterEffect;

	char					cMasterName[en_charac_name_length+1];
	char					cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];

	_sCSPK_Pupil			Pupils[NDD_MP_MAX_PUPIL_COUNT];

	CMSG_MP_INFO_MASTER()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_INFO_MASTER), en_dg_mp_mode_info_master )
	{
	}

	inline void SetFromName( char * name )
	{
		CopyMemory( cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}

	inline void SetMasterName( char * name )
	{
		ZeroMemory( cMasterName, en_charac_name_length+1 );
		strncpy( cMasterName, name, en_charac_name_length );
	}

	inline void SetMasterNick( char * nick )
	{
		ZeroMemory( cMasterNick, NDD_MP_MASTERNICK_LENGTH+1 );
		strncpy( cMasterNick, nick, NDD_MP_MASTERNICK_LENGTH );
	}
};

//	en_dg_mp_mode_info_req=1,					// 사제 정보 요청.
struct CMSG_MP_INFO_REQ : public CMSG_MASTER_AND_PUPIL
{
	char					cFromName[en_charac_name_length+1];

	int						iMasterIndex;	// 

	CMSG_MP_INFO_REQ()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_INFO_REQ), en_dg_mp_mode_info_req )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_no_info=2,					// 만약 사제 정보가 없다면, 이 패킷으로 알린다.
struct CMSG_MP_NO_INFO : public CMSG_MASTER_AND_PUPIL
{
	char					cFromName[en_charac_name_length+1];

	int						iMasterIndex;		// 이 값은 필요가 없을지도.

	CMSG_MP_NO_INFO()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_NO_INFO), en_dg_mp_mode_no_info )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_add_point=3,					// 포인트 증가 요청.
struct CMSG_MP_ADD_POINT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	int						iFromCharac;
	int						iAddValue;			// 더하라.

	CMSG_MP_ADD_POINT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_ADD_POINT), en_dg_mp_mode_add_point )
	{
	}
};

//	en_dg_mp_mode_update_point=4,				// 포인트 변경 알림 패킷.
struct CMSG_MP_UPDATE_POINT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	u_int					uiMasterTotalPoint;			// 최종값.
	u_int					uiMasterCurPoint;			// 최종값


	int						iFromCharac;
	u_int					uiPupilCurPoint;			// 

	CMSG_MP_UPDATE_POINT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_UPDATE_POINT), en_dg_mp_mode_update_point )
	{
	}

};

//	en_dg_mp_mode_add_kill_count=5,				// 킬 카운트 증가 요청.
struct CMSG_MP_ADD_KILL_COUNT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	int						iFromCharac;
	int						iAddValue;			// 더하라

	CMSG_MP_ADD_KILL_COUNT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_ADD_KILL_COUNT), en_dg_mp_mode_add_kill_count )
	{
	}

};

//	en_dg_mp_mode_update_kill_count=6,			// 킬 카운트 변경 알림 패킷.
struct CMSG_MP_UPDATE_KILL_COUNT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	u_int					uiTotalKillCount;

	int						iFromCharac;
	u_int					uiValue;			// 최종값

	CMSG_MP_UPDATE_KILL_COUNT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_UPDATE_KILL_COUNT), en_dg_mp_mode_update_kill_count )
	{
	}

};

// en_dg_mp_mode_update_rank=7,				// GS->DS->GS 랭크 업데이트
struct CMSG_MP_UPDATE_RANK : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	u_char					ucRank;				// 이 값으로 업데이트하고, 클라이언트에 알린다. 

	CMSG_MP_UPDATE_RANK()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_UPDATE_RANK), en_dg_mp_mode_update_rank )
	{
	}
};


// en_dg_mp_mode_create_mp_req=8,				// 사제 정보 생성 요청.
struct CMSG_MP_CREATE_MP_REQ : public CMSG_MASTER_AND_PUPIL
{
	char					cFromName[en_charac_name_length+1];

	int						iMasterIndex;

	u_char					ucLevel;			// 현재레벨/문파 등의 정보 동기화를 위해 필요함.
	u_char					ucMoonpa;
	u_char					ucClass;
	u_char					ucClassGrade;

	CMSG_MP_CREATE_MP_REQ()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_CREATE_MP_REQ), en_dg_mp_mode_create_mp_req )
	{
	}

	void SetName( char * name )
	{
		CopyMemory( cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}
};

// en_dg_mp_mode_create_success=9,				// 사제 정보 생성 성공.
struct CMSG_MP_CREATE_SUCCESS : public CMSG_MASTER_AND_PUPIL
{
	char					cFromName[en_charac_name_length+1];

	int						iMasterIndex;		// 

	u_char					ucMasterSex;
	u_char					ucMasterInnerLevel;
	u_char					ucMasterMoonpa;
	u_char					ucMasterClass;
	u_char					ucMasterClassgrade;


	// 사제관계의 스승 정보만 간단히 보냄.
	// 그냥 초기 정보로 셋팅하고, 알려준다.
	CMSG_MP_CREATE_SUCCESS()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_CREATE_SUCCESS), en_dg_mp_mode_create_success )
	{
	}

	void SetName( char * name )
	{
		CopyMemory( cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}
};

// en_dg_mp_mode_create_fail=10,				// 사제 정보 생성 실패.
struct CMSG_MP_CREATE_FAIL : public CMSG_MASTER_AND_PUPIL
{
	char					cFromName[en_charac_name_length+1];

	int						iMasterIndex;

	u_char					ucFailCode;		// 

	// 

	CMSG_MP_CREATE_FAIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_CREATE_FAIL), en_dg_mp_mode_create_fail )
	{
	}

	void SetName( char * name )
	{
		CopyMemory( cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}
};

// en_dg_mp_mode_notice_connection=11,			// 접속/끊김 알림 패킷.
struct CMSG_MP_NOTICE_CONNECTION : public CMSG_MASTER_AND_PUPIL
{
	u_char					ucConnectState;		// 0 - logout, 1 - enter field, 2 - Server move.

	u_char					ucCurZone;			// Zone 인덱스를 포함해서 보내다. 

	int						iMasterIndex;		// 사제 정보를 찾는데 쓰인다.
	int						iCharacIndex;		// 사제 정보에서 캐릭터를 찾는다.

	CMSG_MP_NOTICE_CONNECTION()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_NOTICE_CONNECTION), en_dg_mp_mode_notice_connection )
	{
	}
};


// en_dg_mp_mode_change_master_nick_req=12,	// 사호 변경 알림.
struct CMSG_MP_CHANGE_MASTER_NICK_REQ : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	char					cNick[NDD_MP_MASTERNICK_LENGTH+1];

	CMSG_MP_CHANGE_MASTER_NICK_REQ()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_CHANGE_MASTER_NICK_REQ), en_dg_mp_mode_change_master_nick_req )
	{
	}

	void SetNick( char * nick )
	{
		ZeroMemory( cNick, sizeof(cNick) );
		strncpy( cNick, nick, NDD_MP_MASTERNICK_LENGTH );
	}
};

// en_dg_mp_mode_update_master_nick=13,		// 사호 변경하라.
struct CMSG_MP_UPDATE_MASTER_NICK : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	char					cNick[NDD_MP_MASTERNICK_LENGTH+1];

	CMSG_MP_UPDATE_MASTER_NICK()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_UPDATE_MASTER_NICK), en_dg_mp_mode_update_master_nick )
	{
	}

	void SetNick( char * nick )
	{
		ZeroMemory( cNick, sizeof(cNick) );
		strncpy( cNick, nick, NDD_MP_MASTERNICK_LENGTH );
	}
};

//en_dg_mp_mode_update_pupil_position=14,		// 제자의 서열이 변했다. (변경사항만 전달)
struct CMSG_MP_UPDATE_PUPIL_POSITION : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterindex;
	struct {
		int					iPupilIndex;				//
		u_char				ucNewPos;
	} ChangePosition[NDD_MP_MAX_PUPIL_COUNT];

	CMSG_MP_UPDATE_PUPIL_POSITION()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_UPDATE_PUPIL_POSITION), en_dg_mp_mode_update_pupil_position )
	{
		ZeroMemory( ChangePosition, sizeof(ChangePosition) );
	}
};


// en_dg_mp_mode_chat
struct CMSG_MP_CHAT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	char					cFromName[en_charac_name_length+1];

	char					cChat[MAX_CHAT_LENGTH];

	CMSG_MP_CHAT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_CHAT), en_dg_mp_mode_chat )
	{
	}
};



//	en_dg_mp_mode_info_error=50,				// 정보 관련 에러 메시지.
struct CMSG_MP_INFO_ERROR : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	u_char					ucErrorCode;		// 미정.

	CMSG_MP_INFO_ERROR()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_INFO_ERROR), en_dg_mp_mode_info_error )
	{
	}

};




// 51 - 80 : 사제 관계 맺기 관련 패킷.
//	en_dg_mp_mode_gd_apply_for_pupil=51,
struct CMSG_MP_GD_APPLY_FOR_PUPIL : public CMSG_MASTER_AND_PUPIL
{
	u_char					ucPupilMoonpa;			// 흑/백도. 0/1
	u_char					ucPupilCurLevel;		// 레벨차 13이상.
	u_char					ucPupilClass;
	u_char					ucPupilClassGrade;

	char					cFromPupilName[en_charac_name_length+1];
	char					cToMasterName[en_charac_name_length+1];

	CMSG_MP_GD_APPLY_FOR_PUPIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_GD_APPLY_FOR_PUPIL), en_dg_mp_mode_gd_apply_for_pupil )
	{
	}

	inline void SetName( char * fromPupil, char * toMaster )
	{
		CopyMemory( cFromPupilName, fromPupil, en_charac_name_length );
		cFromPupilName[en_charac_name_length] = 0;
		CopyMemory( cToMasterName, toMaster, en_charac_name_length );
		cToMasterName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_dg_apply_for_pupil=52,
struct CMSG_MP_DG_APPLY_FOR_PUPIL : public CMSG_MASTER_AND_PUPIL
{
	u_char					ucPupilMoonpa;			// 흑/백도. 0/1
	u_char					ucPupilCurLevel;		// 레벨차 13이상.
	u_char					ucPupilClass;
	u_char					ucPupilClassGrade;

	char					cFromPupilName[en_charac_name_length+1];
	char					cToMasterName[en_charac_name_length+1];
	// cToMasterName 캐릭터를 찾아, MSG_MP_GC_APPLY_FOR_PUPIL 을 보낸다.

	CMSG_MP_DG_APPLY_FOR_PUPIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_DG_APPLY_FOR_PUPIL), en_dg_mp_mode_dg_apply_for_pupil )
	{
	}

	inline void SetName( char * fromPupil, char * toMaster )
	{
		CopyMemory( cFromPupilName, fromPupil, en_charac_name_length );
		cFromPupilName[en_charac_name_length] = 0;
		CopyMemory( cToMasterName, toMaster, en_charac_name_length );
		cToMasterName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_apply_for_pupil_error=53,		// 신청자에게 돌려준다. 
struct CMSG_MP_APPLY_FOR_PUPIL_ERROR : public CMSG_MASTER_AND_PUPIL
{
	char					cFromPupilName[en_charac_name_length+1];
	char					cToMasterName[en_charac_name_length+1];

	u_char					ucErrorCode;	// 미정 : 
	// 0 - none...
    // 1 - 스승을 찾을 수 없다.
	// 2 - 스승 자격이 없는 캐릭터입니다.
	// 3 - 더이상 제자를 받을 수 없습니다.

	CMSG_MP_APPLY_FOR_PUPIL_ERROR()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_APPLY_FOR_PUPIL_ERROR), en_dg_mp_mode_apply_for_pupil_error )
	{
	}

	inline void SetName( char * fromPupil, char * toMaster )
	{
		CopyMemory( cFromPupilName, fromPupil, en_charac_name_length );
		cFromPupilName[en_charac_name_length] = 0;
		CopyMemory( cToMasterName, toMaster, en_charac_name_length );
		cToMasterName[en_charac_name_length] = 0;
	}
};


//	en_dg_mp_mode_answer_yes=54,				// '예'를 선택했다.
struct CMSG_MP_ANSWER_YES : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	char					cPupilName[en_charac_name_length+1];
	char					cFromMasterName[en_charac_name_length+1];

	CMSG_MP_ANSWER_YES()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_ANSWER_YES), en_dg_mp_mode_answer_yes )
	{
	}

	inline void SetName( char * pupil_name, char * master_name )
	{
		CopyMemory( cPupilName, pupil_name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
		CopyMemory( cFromMasterName, master_name, en_charac_name_length );
		cFromMasterName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_answer_apply_to_pupil_no=55,	// '아니오'를 선택했을 때.
struct CMSG_MP_ANSWER_APPLY_TO_PUPIL_NO : public CMSG_MASTER_AND_PUPIL
{
	char					cPupilName[en_charac_name_length+1];
	char					cFromMasterName[en_charac_name_length+1];

	// gs -> ds 로, ds에서 cPupilName 을 찾아, ds->gs 로 다시 보냄. 
	// gs에서 받으면, cPupilName 에게 no 전달.

	CMSG_MP_ANSWER_APPLY_TO_PUPIL_NO()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_ANSWER_APPLY_TO_PUPIL_NO), en_dg_mp_mode_answer_apply_to_pupil_no )
	{
	}

	inline void SetName( char * pupil_name, char * master_name )
	{
		CopyMemory( cPupilName, pupil_name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
		CopyMemory( cFromMasterName, master_name, en_charac_name_length );
		cFromMasterName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_complete_join_pupil=56,		// 신청자에게 전달.
struct CMSG_MP_COMPLETE_JOIN_PUPIL : public CMSG_MASTER_AND_PUPIL
{
	// cPupilName 에게 이 패킷을 보낸다.
    char					cPupilName[en_charac_name_length+1];

	char					cMasterName[en_charac_name_length+1];

	CMSG_MP_COMPLETE_JOIN_PUPIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_COMPLETE_JOIN_PUPIL), en_dg_mp_mode_complete_join_pupil )
	{
	}

	inline void SetName( char * pupil, char * master )
	{
		ZeroMemory( cPupilName, sizeof(cPupilName) );
		strncpy( cPupilName, pupil, en_charac_name_length );

		ZeroMemory( cMasterName, sizeof(cMasterName) );
		strncpy( cMasterName, master, en_charac_name_length );
	}

};

//	en_dg_mp_mode_add_new_pupil=57,				// 스승 및 다른 제자에게 전달.
struct CMSG_MP_ADD_NEW_PUPIL : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;		// 
	_sCSPK_Pupil			newPupilInfo;		// 

	CMSG_MP_ADD_NEW_PUPIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_ADD_NEW_PUPIL), en_dg_mp_mode_add_new_pupil )
	{
	}

	void SetInfo( _sCSPK_Pupil& pupil )
	{
		CopyMemory( &newPupilInfo, &pupil, sizeof(_sCSPK_Pupil) );
	}
};

// en_dg_mp_mode_master_simple_info_to_pupil
struct CMSG_MP_MASTER_SIMPLE_INFO_TO_PUPIL : public CMSG_MASTER_AND_PUPIL
{
	char					cFromPupilName[en_charac_name_length+1];
	char					cToMasterName[en_charac_name_length+1];

	u_char					ucMasterRank;
	u_char					ucMasterLevel;
	u_char					ucMasterMoonpa;
	u_char					ucMasterClass;
	u_char					ucMasterClassGrade;
	u_char					ucCurZone;

	char					cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];

	CMSG_MP_MASTER_SIMPLE_INFO_TO_PUPIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_MASTER_SIMPLE_INFO_TO_PUPIL), en_dg_mp_mode_master_simple_info_to_pupil )
	{
	}

	void SetName( char * pupil, char * master )
	{
		ZeroMemory( cFromPupilName, sizeof(cFromPupilName) );
		strncpy( cFromPupilName, pupil, en_charac_name_length );

		ZeroMemory( cToMasterName, sizeof(cToMasterName) );
		strncpy( cToMasterName, master, en_charac_name_length );
	}
	void SetNick( char * nick )
	{
		ZeroMemory( cMasterNick, sizeof(cMasterNick) );
		strncpy( cMasterNick, nick, NDD_MP_MASTERNICK_LENGTH );
	}

};


// en_cg_mp_mode_apply_error=80,				// 신청 과정의 모든 에러.
enum EN_MP_APPLY_ERROR		// MSG_MP_APPLY_ERROR
{
	// CMSG_MP_APPLY_FOR_PUPIL_ERROR 의 코드와 같은 값을 사용할 것.
	en_mp_apply_no_error=0,						// 0 값은 그냥 쓴다. 이 값으로는 패킷이 가지 않는다.
	en_mp_apply_error_wait=1,					// 10 초 제한 : 잠시후 다시 시도.
	en_mp_apply_error_pupil_level_limit=2,		// 73 레벨 이상 캐릭터는 제자가 될 수 없습니다.
	en_mp_apply_error_level_diff_limit=3,		// 스승과 제자는 13레벨 이상 차이가 나야합니다.
	en_mp_apply_error_already_has_master=4,		// 이미 다른 스승이 있습니다.
	en_mp_apply_error_pupils_over=5,			// 더 이상 제자를 받을 수 없습니다.
	en_mp_apply_error_you_are_master=6,			// 현재 스승이다.
	en_mp_apply_error_not_match_side=7,			// 흑백도 속성이 다르다.
	en_mp_apply_error_target_cannot_master=8,	// 대상은 스승 자격이 없습니다.
	en_mp_apply_error_target_not_found=9,		// 대상을 찾을 수 없습니다.
	en_mp_apply_error_fail_to_add_pupil=10,		// 추가에 실패.
};

//	en_dg_mp_mode_apply_error=80,				// 관계 관련 에러.
struct CMSG_MP_APPLY_ERROR : public CMSG_MASTER_AND_PUPIL
{
	char					cFromPupilName[en_charac_name_length+1];

	u_char					ucErrorCode;

	CMSG_MP_APPLY_ERROR()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_APPLY_ERROR), en_dg_mp_mode_apply_error )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cFromPupilName, name, en_charac_name_length );
		cFromPupilName[en_charac_name_length] = 0;
	}
};



// 81 - 100 : 파문 및 하산.
//	en_dg_mp_mode_strike_pupil_req=81,			// 스승 : 파문 요청.
struct CMSG_MP_STRIKE_PUPIL_REQ : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	char					cPupilName[en_charac_name_length+1];

	CMSG_MP_STRIKE_PUPIL_REQ()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_STRIKE_PUPIL_REQ), en_dg_mp_mode_strike_pupil_req )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cPupilName, name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_strike_result=82,				// 파문 성공시 스승 및 다른 제자에게.
struct CMSG_MP_STRIKE_RESULT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	int						iPupilIndex;

	u_int					uiMasterTotalPoint;
	u_int					uiMasterCurPoint;
	u_char					ucBonusSkillStep;

	struct {
		int				iPupilIndex;				//
		u_char			ucNewPos;
	} ChangePosition[NDD_MP_MAX_PUPIL_COUNT];

	CMSG_MP_STRIKE_RESULT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_STRIKE_RESULT), en_dg_mp_mode_strike_result )
	{
		iMasterIndex = 0;
		iPupilIndex = 0;
		uiMasterTotalPoint = 0;
		uiMasterCurPoint = 0;
		ucBonusSkillStep = 0;

		ZeroMemory( ChangePosition, sizeof(ChangePosition) );
	}

};

//	en_dg_mp_mode_you_struck=83,				// 파문 캐릭터에게 보낸다.
struct CMSG_MP_YOU_STRIKE : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	char					cPupilName[en_charac_name_length+1];	// 쫒겨난 캐릭터의 이름.

	CMSG_MP_YOU_STRIKE()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_YOU_STRIKE), en_dg_mp_mode_you_struck )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cPupilName, name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
	}
};



//	en_dg_mp_mode_strike_error=90,				// 파문 실패시 에러 정보.
struct CMSG_MP_STRIKE_ERROR : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	char					cPupilName[en_charac_name_length+1];

	u_char					ucErrorCode;		//

	// error code
	// 0 - not found master info.
	// 1 - pupil is not member.
	// 2 - progressing other strike
	// 3 - minimum 72 hours.
	// 4 - system error.

	CMSG_MP_STRIKE_ERROR()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_STRIKE_ERROR), en_dg_mp_mode_strike_error )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cPupilName, name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
	}
};


//	en_dg_mp_mode_leave_req=91,					// 하산 요청.
struct CMSG_MP_LEAVE_REQ : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	char					cPupilName[en_charac_name_length+1];

	CMSG_MP_LEAVE_REQ()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_LEAVE_REQ), en_dg_mp_mode_leave_req )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cPupilName, name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_you_left=92,					// 하산 성공시 신청 제자에게 감.
struct CMSG_MP_YOU_LEFT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	char					cPupilName[en_charac_name_length+1];

	CMSG_MP_YOU_LEFT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_YOU_LEFT), en_dg_mp_mode_you_left )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cPupilName, name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_left_pupil=93,				// 스승 및 다른 제자에게 하산을 알린다.
struct CMSG_MP_LEFT_PUPIL : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	int						iPupilIndex;

	//!// 기타 정보를 채울 필요가 있다.
	u_int					uiMasterTotalPoint;
	u_int					uiMasterCurPoint;
	u_char					ucBonusSkillStep;

	struct {
		int				iPupilIndex;				//
		u_char			ucNewPos;
	} ChangePosition[NDD_MP_MAX_PUPIL_COUNT];

	CMSG_MP_LEFT_PUPIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_LEFT_PUPIL), en_dg_mp_mode_left_pupil )
	{
		iMasterIndex = 0;
		iPupilIndex = 0;
		uiMasterTotalPoint = 0;
		uiMasterCurPoint = 0;
		ucBonusSkillStep = 0;

		ZeroMemory( ChangePosition, sizeof(ChangePosition) );
	}
};

//	en_dg_mp_mode_left_error=100,				// 하산 과정의 에러.
struct CMSG_MP_LEFT_ERROR : public CMSG_MASTER_AND_PUPIL
{
	char					cPupilNane[en_charac_name_length+1];

	u_char					ucErrorCode;

	// error code
	// 0 - not found master info.
	// 1 - pupil is not member.
	// 2 - progressing other strike
	// 3 - minimum 72 hours.
	// 4 - system error.


	CMSG_MP_LEFT_ERROR()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_LEFT_ERROR), en_dg_mp_mode_left_error )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cPupilNane, name, en_charac_name_length );
		cPupilNane[en_charac_name_length] = 0;
	}
};

// 250 이후 : 메모리 추가/삭제 정보 교환.
//	en_dg_mp_mode_del_from_memory=101,			// 메모리에서 내림.
struct CMSG_MP_DEL_FROM_MEMORY : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	CMSG_MP_DEL_FROM_MEMORY()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_DEL_FROM_MEMORY), en_dg_mp_mode_del_from_memory )
	{
	}
};



// 151 ~ 170 : 사제 신청 거부 및 게시판 관리
// en_dg_mp_mode_accept_pupil_mode = 151,		// 사제 신청 거부 관련.
struct CMSG_MP_ACCEPT_PUPIL_MODE : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;			// 

	char					cCharacName[en_charac_name_length+1];

	u_char					ucAcceptMode;			// 0 - Yes, 1 - No. 

	CMSG_MP_ACCEPT_PUPIL_MODE() 
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_ACCEPT_PUPIL_MODE), en_dg_mp_mode_accept_pupil_mode ) 
	{
	}

	void SetName( char * name )
	{
		if( name )
		{
			ZeroMemory( cCharacName, sizeof(cCharacName) );
			strncpy( cCharacName, name, en_charac_name_length );
		}
	}
};


// en_dg_mp_mode_board_initial_req = 152,		// 처음 요청할 때.( 게시판을 열 때 )
struct CMSG_MP_BOARD_INITIAL_REQ : public CMSG_MASTER_AND_PUPIL
{
	// 처음으로 요청하는 경우다.
	// initial_open 으로 응답할 것.
	char					cCharacName[en_charac_name_length+1];

	char					cSide;			// -1 흑도, 0 - 낭인, 1 - 백도

	CMSG_MP_BOARD_INITIAL_REQ() 
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_BOARD_INITIAL_REQ), en_dg_mp_mode_board_initial_req ) 
	{
	}

	void SetName( char * name )
	{
		if( name )
		{
			ZeroMemory( cCharacName, sizeof(cCharacName) );
			strncpy( cCharacName, name, en_charac_name_length );
		}
	}
};


// en_dg_mp_mode_board_initial_open = 153,		// 처음 요청시의 응답. ( 1 페이지 정보가 포함된다. )
struct CMSG_MP_BOARD_INITIAL_OPEN : public CMSG_MASTER_AND_PUPIL
{
	char						cCharacName[en_charac_name_length+1];

	char						cSide;					// GS 에서 다시 받았을 때 필요하다.

	u_char						ucTotalPage;			// 페이지 수.

	u_char						ucCount;				// 

	// ucCount 로 잘라서 보냄.
	_sPacketMPBoard				board[NDD_MP_MAX_ITEMCOUNT_PER_BOARD];				// 최대 12개
	

	CMSG_MP_BOARD_INITIAL_OPEN() 
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_BOARD_INITIAL_OPEN), en_dg_mp_mode_board_initial_open ) 
	{
	}

	void SetName( char * name )
	{
		if( name )
		{
			ZeroMemory( cCharacName, sizeof(cCharacName) );
			strncpy( cCharacName, name, en_charac_name_length );
		}
	}

	void Reset()
	{
		ucTotalPage = 0;
		ucCount = 0;
		ZeroMemory( board, sizeof(board) );
	}

	void AddBoard( _sPacketMPBoard * b )
	{
		if( ucCount < NDD_MP_MAX_ITEMCOUNT_PER_BOARD )
		{
			CopyMemory( &board[ucCount], b, sizeof(_sPacketMPBoard) );
			++ucCount;
		}
	}


	void SetLength()
	{
		if( ucCount > NDD_MP_MAX_ITEMCOUNT_PER_BOARD )
			ucCount = NDD_MP_MAX_ITEMCOUNT_PER_BOARD;

		usLength = sizeof(CMSG_MP_BOARD_INITIAL_OPEN) - ( (NDD_MP_MAX_ITEMCOUNT_PER_BOARD - ucCount) * sizeof(_sPacketMPBoard) );
	}
};


// en_dg_mp_mode_board_req_page=154,			// 게시판 정보 요청. 페이지 단위.
struct CMSG_MP_BOARD_REQ_PAGE : public CMSG_MASTER_AND_PUPIL
{
	char						cCharacName[en_charac_name_length+1];

	char						cSide;					// 

	u_char						ucReqPage;				// 요청 페이지.

	CMSG_MP_BOARD_REQ_PAGE() 
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_BOARD_REQ_PAGE), en_dg_mp_mode_board_req_page ) 
	{
	}

	void SetName( char * name )
	{
		if( name )
		{
			ZeroMemory( cCharacName, sizeof(cCharacName) );
			strncpy( cCharacName, name, en_charac_name_length );
		}
	}
};


// en_dg_mp_mode_board_page_info=155,			// 게시판 정보.
struct CMSG_MP_BOARD_PAGE_INFO : public CMSG_MASTER_AND_PUPIL
{
	char						cCharacName[en_charac_name_length+1];

	char						cSide;					// 
	u_char						ucTotalPage;		// 전체 페이지수를 항상 보낸다.

	u_char						ucPage;				// 페이지 
	u_char						ucCount;			// 아이템 개수

	_sPacketMPBoard				board[NDD_MP_MAX_ITEMCOUNT_PER_BOARD];

	CMSG_MP_BOARD_PAGE_INFO() 
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_BOARD_PAGE_INFO), en_dg_mp_mode_board_page_info ) 
	{
	}

	void SetName( char * name )
	{
		if( name )
		{
			ZeroMemory( cCharacName, sizeof(cCharacName) );
			strncpy( cCharacName, name, en_charac_name_length );
		}
	}

	void Reset( u_char page )
	{
		ucPage = page;
		ucCount = 0;
		ZeroMemory( board, sizeof(board) );
	}

	void AddBoard( _sPacketMPBoard * b )
	{
		if( ucCount < NDD_MP_MAX_ITEMCOUNT_PER_BOARD )
		{
			CopyMemory( &board[ucCount], b, sizeof(_sPacketMPBoard) );
			++ucCount;
		}
	}


	void SetLength()
	{
		if( ucCount > NDD_MP_MAX_ITEMCOUNT_PER_BOARD )
			ucCount = NDD_MP_MAX_ITEMCOUNT_PER_BOARD;

		usLength = sizeof(CMSG_MP_BOARD_PAGE_INFO) - ( (NDD_MP_MAX_ITEMCOUNT_PER_BOARD - ucCount) * sizeof(_sPacketMPBoard) );
	}
};


// en_dg_mp_mode_board_error = 170,			// 에러.
struct CMSG_MP_BOARD_ERROR : public CMSG_MASTER_AND_PUPIL
{
	char						cCharacName[en_charac_name_length+1];

	u_char						ucError;			// 

	CMSG_MP_BOARD_ERROR() 
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_BOARD_ERROR), en_dg_mp_mode_board_error ) 
	{
	}

	void SetName( char * name )
	{
		if( name )
		{
			ZeroMemory( cCharacName, sizeof(cCharacName) );
			strncpy( cCharacName, name, en_charac_name_length );
		}
	}

	// 0 - 더이상 페이지가 없습니다.
};


#endif	// _PD_MASTER_PUPIL_



#pragma pack( pop, enter_member )