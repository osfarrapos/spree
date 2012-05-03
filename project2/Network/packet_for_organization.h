#pragma once

//#################################################################################
//
//  Organization 패킷 : Main 21 사용 
//  21 : 168 ~ 175
//
//#################################################################################

#define  _MAX_OR_UNIT_NUM 100

// 최종 접속 종료 시간 정보
struct _OutTime
{
	u_char	year ;
	u_char	month ;
	u_char	day ;
	u_char	h ;
	u_char  m ;
	u_char	s ;

	_OutTime & _OutTime::operator=( const _OutTime& rhs ) 
	{
		if( this == &rhs ) { return *this ; }
		
		year = rhs.year;
		month = rhs.month;
		day = rhs.day;
		h = rhs.h;
		m = rhs.m;
		s = rhs.s;
		return *this ;
	}

	void reset() 
	{		
		year = 0;
		month = 0;
		day = 0;
		h = 0;
		m = 0;
		s = 0;		
	} ;
} ;

class _Unit
{
public:	
	TCHAR			m_cName[13] ;	
	bool			m_bOnline ;
	char			m_cClass ;				// 계급
	enum{
		_UNIT_CLASS_DEFAULT_UNIT_,			// 기본 조직원
		_UNIT_CLASS_SPECIAL_UNIT_,			// 상급 조직원
		_UNIT_CALSS_CAPTAIN_UNIT_,			// 절대 지휘자..
	} ;
	short			m_sInnerLevel ;
	char			m_cZone ;	

	_Unit & _Unit::operator=( const _Unit& rhs ) 
	{
		if( this == &rhs ) { return *this ; }
		
		//memcpy( this, &rhs, sizeof(_Unit) ) ;
		memset( m_cName, 0, sizeof( TCHAR ) * 13 );
		strcpy( m_cName, rhs.m_cName );
		m_bOnline = rhs.m_bOnline;
		m_cClass = rhs.m_cClass;
		m_sInnerLevel = rhs.m_sInnerLevel;
		m_cZone = rhs.m_cZone;
		return *this ;
	}

	inline void reset() 
	{		
		//memset(this, 0, sizeof(_Unit) ) ; 
		memset( m_cName, 0, sizeof( TCHAR ) * 13 );
		m_bOnline = false;
		m_cClass = 0;
		m_sInnerLevel = 0;
		m_cZone = 0;

	}
};


// 조직의 기본 데이타다.. 패킷으로 보낼때 이런 구조체 단위로 보내면 처리하기 편리하겠다..
class _OrganizationBase
{
public:
	int				m_iIndex ;
	char			m_cName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;			// 조직 이름
	char			m_cProperty ;			// 조직 속성(분파 | 방회)
	char			m_cBaseParty ;			// 소속 문파
	u_int			m_uiExperience ;		// 조직 경험치
	u_int			m_uiContribution ;		// 조직의 문파 기여도
	u_short			m_usCurMemberNum ;		// 현재 인원	
	char			m_cCaptainName[13] ;	
	
	_OrganizationBase & _OrganizationBase::operator=( const _OrganizationBase& rhs ) 
	{
		if( this == &rhs ) { return *this ; }
		
		memcpy( this, &rhs, sizeof(_OrganizationBase) ) ;
		return *this ;
	}	
};

struct _sHeader ;

// 분파 메시지..
#define MSG_NO_ORGANIZATION_CREATE_REQ			168	// 조직 생성
#define MSG_NO_ORGANIZATION_UNIT_UPDATE_		169	// 조직원 추가, 삭제, 정보변경, 장 위님, 클래스 변경 등...(요청과 응답 모두 쓰임)
#define MSG_NO_ORGANIZATION_INFO_REQ			170	// 조직 정보 요청
#define MSG_NO_ORGANIZATION_INFO_RES			170	// 조직 정보 응답
#define MSG_NO_ORGANIZATION_RESULT				172	// REQ 에 대한 응답 및 실패 메시지..
#define MSG_NO_ORGANIZATION_CLIENT				173	// // 권유 관련 요청과 응답..
#define MSG_NO_ORGANIZATION_GLOBAL_INFO			174	// 주변 캐릭의 조직 정보
#define MSG_NO_ORGANIZATION_NOTICE_REQ			175	// 공지 수정 요청
#define MSG_NO_ORGANIZATION_NOTICE				175	// 공지 수정 응답

//#define MSG_NO_ORGANIZATION_CREATE_REQ			// 조직 생성
struct MSG_ORGANIZATION_CREATE_REQ
{
	_sHeader			header ;
	u_char				ucMessage ;

	char				cOrganazationName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;	
};

//#define MSG_NO_ORGANIZATION_UNIT_UPDATE_		// 삭제, 가입 권한자 부여, 클래스 변경 등... (요청과 응답 모두 쓰임)
struct MSG_ORGANIZATION_UNIT_UPDATE_
{
	_sHeader			header ;
	u_char				ucMessage ;	
	
	_Unit				_unit ;
	int					iCode ;
	enum{				// iCode
				//받기 전용..
		_OR_UNIT_JOIN_,					// 조직원을 추가하라.. S -> C
		_OR_UNIT_DROP_,					// 조직원 삭제하라  S -> C		
		_OR_UNIT_KICK_,					// 강제 삭제
		// 주고 받기.. 
		_OR_UNIT_ENTRUST_,				// 장 위임 ( 장 만 가능) S -> C
		_OR_UNIT_UPDATE_,				// 조직원 업데이트( 로그인 상태, 클래스 조정, 경험치 변경등..) C -> S -> C
	} ;
	_OutTime			m_outTime ;		// (iCode == _OR_UNIT_UPDATE_ && m_outTime.year > 0) 일때만 시간을 갱신하세요..
};

//#define MSG_NO_ORGANIZATION_INFO_REQ			// 조직 전체 정보 요청
struct MSG_ORGANIZATION_INFO_REQ
{
	_sHeader			header ;
	u_char				ucMessage ;

	int					index ;
	char				cCode ;
	enum{
		_REQ_MY_OR_INFO_,			// 내 조직 정보 요청
		_REQ_OR_GLOBAL_INFO,		// 주변 캐릭의 조직정보 요청
	};
	// 실패시 MSG_ORGANIZATION_RESULT에 결과로 돌려줄 코드
	enum{
		_FAILE_CODE_NOT_INNER_,				// 조직에 속해있지 않음..		
		_FAILE_CODE_ETC_,						// 기타 예외적인 오류
	};
};

// %% 주의 %% 이 패킷은 가변길이 패킷입니다. 조직원의 인원수에 따라 패킷 길이가 달라짐..
//#define MSG_NO_ORGANIZATION_INFO_RES		// 조직 정보 응답
struct MSG_ORGANIZATION_INFO_RES
{
	_sHeader			header ;
	u_char				ucMessage ;

	bool				bCreateAble ;					// true : 새로 만듬,
	_OrganizationBase	_organizationBase ;	
	char				cMemberCount ;					// 조직원 인원..
	_Unit				_memberList[_MAX_OR_UNIT_NUM] ;	// 조직원들의 리스트
	_OutTime			outTimeList[_MAX_OR_UNIT_NUM] ;
};


//#define MSG_NO_ORGANIZATION_RESULT				// REQ 에 대한 응답 및 실패 메시지..
struct MSG_ORGANIZATION_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;

	int					iCode ;						// 실패 코드입니다.
	
	// 실패시 MSG_ORGANIZATION_RESULT에 결과로 돌려줄 코드
	enum{
		_OR_DESTROY_,		// 조직이 삭제되었다..(ucFromMessage 와 상관 없음)
		
		_FAILED_CODE_ALREADY_INNER_,				// 이미 조직에 속한자..
		_FAILED_CODE_NEED_MORE_MEMBER_,			// 초기 발기 인원이 부족함(5명 이상)
		_FAILED_CODE_NOT_PARTY_JOINNER_,			// 파티 상태가 아닙니다.
		_FAILED_CODE_CANT_PARTY_LEADER,			// 파티장만 가능합니다.
		_FAILED_CODE_UNLIKE_MOONPA,				// 다른 문파가 있습니다.
		_FAILED_CODE_UNLIKE_ZONE,				// 다른 지역에 있습니다.
		_FAILED_CODE_USING_NAME,					// 사용중인 이름입니다.
		_FAILED_CODE_ETC_,						// 기타 예외적인 오류
		_FAILED_CODE_FULL,						// 허용 인원이 꽉 차 있다..
		_FAILED_CODE_NEED_MORE_LEVEL,			// 레벨이 부족함..
	};
	
};


//#define MSG_NO_ORGANIZATION_CLIENT				// 권유 관련 요청과 응답..
struct MSG_ORGANIZATION_CLIENT_						// Client <-> GS <-> Client
{
	_sHeader			header ;
	u_char				ucMessage ;

	int					iCode ;	
	char				cName[13] ;			// 가입을 요청할때는 상대방 이름.. 응답할때는 발신자 이름..
	
	
	enum{
		// 요청
		_OR_REQ_INVITE_,					// 피 조직원에게 우리 조직에 들어오라고 권유
		_OR_REQ_ENTRUST_,					// 니가 짱 먹으라고 권유..

		_OR_REQ_MOONPA_CHECK_,				// 파티원들의 문파가 동일한지 검사한다.
	
		// 응답
		_OR_REQ_INVITE_OK_,					// 조직에 가입하겠다.
		_OR_REQ_INVITE_NO_,					// 조직에 가입하지 않겠다.		
		_OR_REQ_ENTRUST_OK_,				// 짱 하겠다.
		_OR_REQ_ENTRUST_NO_,				// 짱 안하겠다.
		_OR_REQ_LOGOFF_,					// 상대방은 로그오프중..
		_OR_REQ_ALREADY_INNER_,				// 이미 가입한 사람

		_OR_REQ_MOONPA_OK_,					// 문파가 정상이다.
		
	} ;
};

//#define MSG_NO_ORGANIZATION_GLOBAL_INFO			174	// 주변 캐릭의 조직 정보
struct MSG_ORGANIZATION_GLOBAL_INFO 
{
	_sHeader			header ;
	u_char				ucMessage ;

	int				m_iIndex ;
	char			m_cName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;			// 조직 이름
	char			m_cProperty ;			// 조직 속성(분파 | 방회)
	char			m_cBaseParty ;			// 소속 문파
	u_int			m_uiExperience ;		// 조직 경험치	
};

//#define MSG_NO_ORGANIZATION_NOTICE_REQ			175	// 공지 수정 요청 / 응답
struct MSG_ORGANIZATION_NOTICE_REQ 
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	int					m_seqNum ;	// 0, 1, 2(LAST)
	int					m_iIndex ;
	union
	{
		struct _Notice_Data
		{
			char		writer[13] ;
			char		writedate[15] ;
			char		notice[72] ;
		} m_NoticeData ;

		char			m_cNotice[100] ;		/*#define _OR_NOTICE_ONE_PIECE_SIZE		100*/		
	};	
	
};

//#define MSG_NO_ORGANIZATION_NOTICE				175	// 공지 수정 요청 / 응답
struct MSG_ORGANIZATION_NOTICE 
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	union
	{
		struct _Notice_Data
		{
			char		writer[13] ;
			char		writedate[15] ;
			char		notice[272] ;
		} m_NoticeData ;

		char			m_cNotice[300] ;
	};
};




#ifdef _XTS_ALIANCE_20061018

//////////////////////////////////////////////////////////////////////////
// 동맹 관련 패킷.
//////////////////////////////////////////////////////////////////////////
#define MSG_NO_ALLY_BASE						171	// 동맹에 관련된 모든 패킷.

enum EN_ALLY_PACKET_MODE
{
	en_ally_mode_request_ally=0,				// 0 가입 권유
		en_ally_mode_request_from,				// 1 누간가에게 권유가 왔다
		en_ally_mode_response_yes,				// 2 권유의 응답으로 Yes
		en_ally_mode_response_no,				// 3 권유의 응답으로 No
		en_ally_mode_request_ally_cancel,		// 4 가입 권유 취소하기.
		en_ally_mode_ally_is_made,				// 5 동맹이 결성되었다.		
		en_ally_mode_org_enter_ally,            // 6 자신의 영웅단이 동맹에 가입됬다.		
		en_ally_mode_request_banish,            // 7 추방 요청		
		en_ally_mode_banish_complete,           // 8 추방되었다고, 동맹주에게 알려줌.
		

		en_ally_mode_request_ally_info=20,		// 20 동맹의 상세 정보 요청.
		en_ally_mode_ally_all_info,				// 21 동맹의 모든 정보.
		en_ally_mode_notice_enter_new_org,		// 23 새로운 영웅단이 들어왔다.
		en_ally_mode_notice_leave_org,			// 24 영웅단이 탈퇴했다.
		en_ally_mode_scattered,					// 25 동맹이 해체되었다.		
		en_ally_mode_req_leave,					// 26 탈퇴 요청
		en_ally_mode_allow_leave,				// 27 탈퇴 요청 수락.
		en_ally_mode_req_scatter,				// 28 해산 요청
		en_ally_mode_allow_scatter,				// 29 해산 요청 수락.		
		en_ally_mode_request_other_ally,        // 30 다른 동맹의 정보 요청		
		en_ally_mode_other_ally_info,           // 31 다른 동맹의 정보		
		en_ally_mode_notice_banish_org,         // 32 영웅단이 추방되었다.
		en_ally_mode_no_ally_info,				// 33 동맹의 정보가 없다.
		

		en_ally_mode_error_we_already_ally=100,	// 100 이미 동맹을 맺고 있다.		
		en_ally_mode_error_they_already_ally,	// 101 대상 영웅단은 이미 동맹을 맺고 있다.		
		en_ally_mode_error_cannot_ally,         // 102 흑/백도 의 이유로 동맹을 맺을 수 없다.		
		en_ally_mode_error_not_found_info,		// 103 요청한 동맹을 찾을 수 없습니다.		
		en_ally_mode_error_cannot_find_user,	// 104 유저를 찾을 수 없다.		
		en_ally_mode_error_i_am_busy,           // 105 현재 요청중.		
		en_ally_mode_error_busy,				// 106 대상이 다른 일 중.		
		en_ally_mode_error_cannot_request,      // 107 영웅단이 없거나, 단주가 아니다. : Add
		en_ally_mode_error_not_hero,            // 108 대상은 영웅단 단주가 아니다.
		en_ally_mode_error_in_combat,			// 109 영웅단은 장원전에 신청한 상태이므로 방회를 결성할 수 없다
		en_ally_mode_error_target_in_combat,	// 110 타겟이 장원전에 신청한 상태이므로 추가할 수 없다.

};
//#define MSG_NO_ALLY_BASE						171	// 동맹에 관련된 모든 패킷.
struct MSG_ALLY_BASE
{
	_sHeader      header;
	u_char        ucMessage;
	
	u_char        ucMode;	
};

struct __ally_client_info
{
	int		iOrIndex;
	char	iOrName[_XDEF_ORGANIZATIONNAMELENGTH+1];
};


// en_ally_mode_request_ally : C->S, 동맹을 요청할 때,
struct MSG_ALLY_REQUEST_ALLY : public MSG_ALLY_BASE
{
	u_short         usWhom;   // 누구에게 요청하는 지.
};

// en_ally_mode_request_from : S->C, 동맹 요청이 왔다.
struct MSG_ALLY_REQEUST_FROM :public MSG_ALLY_BASE
{
	u_short         usFrom;
	u_char          ucType;   // 0 : 영웅단, 1 :동맹
	char            cName[_XDEF_ORGANIZATIONNAMELENGTH+1];
};

//en_ally_mode_response_yes : 응답으로 Yes 를 보낸다.
struct MSG_ALLY_MODE_RESPONSE_YES :public MSG_ALLY_BASE
{
};

//en_ally_mode_response_no
struct MSG_ALLY_MODE_RESPONSE_NO :public MSG_ALLY_BASE
{
};

//en_ally_mode_request_ally_cancel : 권유를 취소.
struct MSG_ALLY_MODE_REQUEST_ALLY_CANCEL :public MSG_ALLY_BASE
{
};

// en_ally_mode_ally_is_made :동맹이 만들어졌다.
struct MSG_ALLY_MODE_IS_MADE : public MSG_ALLY_BASE
{
	
	int		iAllyIndex;                        // 만들어진 동맹의 인덱스.
	
	int		iOr1Index;                        // 동맹주가 되는 영웅단의 인덱스 (영웅단 동맹의 동맹주 영웅단 이름)
	char	iOr1Name[_XDEF_ORGANIZATIONNAMELENGTH+1];
	
	int		iOr2Index;                        // 가입된 두번째 영웅단의 인덱스 (영웅단 동맹에 가입하는 영웅단 이름)
	char	iOr2Name[_XDEF_ORGANIZATIONNAMELENGTH+1];	
};

//		en_ally_mode_org_enter_ally,            // 6 자신의 영웅단이 동맹에 가입됬다.	
struct MSG_ALLY_MODE_ORG_ENTER_ALLY : public MSG_ALLY_BASE	// 아래의 MSG_ALLY_MODE_ALLY_ALL_INFO 와 동일하게 만든다.
{
	int						iAllyIndex;			// 동맹 인덱스
	char					cMasterName[13];	// 회주 이름
	u_char					ucNoOrg;			// 동맹맴버 개수
	__ally_client_info		info[5];			//
	
};


//en_ally_mode_request_banish,               // 7 추방 요청
struct MSG_ALLY_MODE_REQUEST_BANISH : public MSG_ALLY_BASE
{
	int		iOrgIndex;                        // 추방하려는 영웅단의 인덱스.
};

//en_ally_mode_banish_complete,				// 8 추방되었다고, 동맹주에게 알려줌.
struct MSG_ALLY_MODE_BANISH_COMPLETE : public MSG_ALLY_BASE
{
	int		iOrgIndex;                      // 
};


//en_ally_mode_request_ally_info=20,		// 20 동맹의 상세 정보 요청.
//en_ally_mode_ally_all_info,				// 21 동맹의 모든 정보.
//en_ally_mode_notice_enter_new_org,		// 23 새로운 영웅단이 들어왔다.
//en_ally_mode_notice_leave_org,			// 24 영웅단이 탈퇴했다.
//en_ally_mode_scattered,					// 25 동맹이 해체되었다.		
//en_ally_mode_req_leave,					// 26 탈퇴 요청
//en_ally_mode_allow_leave,				// 27 탈퇴 요청 수락.
//en_ally_mode_req_scatter,				// 28 해산 요청
//en_ally_mode_allow_scatter,				// 29 해산 요청 수락.		
//en_ally_mode_request_other_ally,        // 30 다른 동맹의 정보 요청		
//en_ally_mode_other_ally_info,           // 31 다른 동맹의 정보		
//en_ally_mode_notice_banish_org,         // 32 영웅단이 추방되었다.

//en_ally_mode_request_ally_info=20,// 20 동맹의 상세 정보 요청.
struct MSG_ALLY_MODE_REQUEST_ALLY_INFO : public MSG_ALLY_BASE
{
	int		iAllyIndex;					// 요청하는 동맹의 인덱스.
};

// ally_info_update 는 없다. 동맹 정보가 바뀌게 되면 이 패킷으로 주변에 알리면 된다.
// 실제로 별로 안바뀌니까.
//en_ally_mode_ally_all_info,       // 21 동맹의 모든 정보.
struct MSG_ALLY_MODE_ALLY_ALL_INFO : public MSG_ALLY_BASE
{
	// ucNoOrg 에 따라, 가변길이.ucNoOrg 로 잘라서 파싱할 것.
	int		iAllyIndex;				// 동맹 인덱스.
	char	cMasterName[13];		// 회주 이름 
	u_char	ucNoOrg;				// 동맹 멤버 개수
	__ally_client_info	info[5];    //
	
	void set_length()
	{
		header.sLength = sizeof(MSG_ALLY_BASE) + sizeof(int) +
			sizeof(u_char) + ucNoOrg*sizeof(__ally_client_info);
	}
	
};

// 이 패킷은 기존에 동맹에 가입된 단원들에게만 가는 것이다.
//en_ally_mode_notice_enter_new_org,// 23 새로운 영웅단이 들어왔다.
struct MSG_ALLY_MODE_NOTICE_NEW_ORG : public MSG_ALLY_BASE
{
	__ally_client_info	addInfo;              // 추가되는 영웅단의 정보.
};

//en_ally_mode_notice_leave_org,    // 24 영웅단이 탈퇴했다.
struct MSG_ALLY_MODE_NOTICE_LEAVE_ORG : public MSG_ALLY_BASE
{
	int	iLeaveOrgIndex;               // 탈퇴하는 영웅단의 인덱스.
};

//en_ally_mode_scattered,           // 25 동맹이 해체되었다.
struct MSG_ALLY_MODE_SCATTERED : public MSG_ALLY_BASE
{
	u_char	ucScatterReason;            // 0 - 동맹주의 해체. 1 - 영웅단 탈퇴에 의한 해체.
};


//en_ally_mode_req_leave,                                    // 26 탈퇴 요청
struct MSG_ALLY_MODE_REQ_LEAVE : public MSG_ALLY_BASE
{
};

//en_ally_mode_allow_leave,                                  // 27 탈퇴 요청 수락.
// 이 패킷은 영웅단 탈퇴 공지보다 앞서, 탈퇴를 요청한 단주에게만 보내지는 메시지다.
struct MSG_ALLY_MODE_ALLOW_LEAVE : public MSG_ALLY_BASE
{
};

//en_ally_mode_req_scatter,                                   // 28 해산 요청
struct MSG_ALLY_MODE_REQ_SCATTER : public MSG_ALLY_BASE
{
};

//en_ally_mode_allow_scatter,                                // 29 해산 요청 수락.
struct MSG_ALLY_MODE_ALLOW_SCATTER : public MSG_ALLY_BASE
{
};

//en_ally_mode_request_other_ally,          // 30 다른 동맹의 정보 요청
struct MSG_ALLY_MODE_REQUEST_OTHER_ALLY : public MSG_ALLY_BASE
{
	int		iAllyIndex;
};

//en_ally_mode_other_ally_info,                 // 31 다른 동맹의 정보
struct MSG_ALLY_MODE_OTHER_ALLY_INFO : public MSG_ALLY_BASE
{
	int		iAllyIndex;			// 동맹 인덱스 
	char	cAllyMaster[13];	// 동맹의 동맹주 이름 
	char	cAllyName[_XDEF_ORGANIZATIONNAMELENGTH+1];
};

//en_ally_mode_notice_banish_org,                       // 32 영웅단이 추방되었다.
struct MSG_ALLY_MODE_NOTICE_BANISH_ORG : public MSG_ALLY_BASE
{
	int		iOrgIndex;
	char	cOrgName[_XDEF_ORGANIZATIONNAMELENGTH+1];
};

//en_ally_mode_no_ally_info  // 33 동맹의 정보가 없다.
struct MSG_ALLY_MODE_NO_ALLY_INFO : public MSG_ALLY_BASE
{
	int		iAllyIndex;			// 해당 동맹의 정보가 없다. 
};


// en_ally_mode_error_we_already_ally=100,	// 100 이미 동맹을 맺고 있다.
// en_ally_mode_error_they_already_ally,	// 101 대상 영웅단은 이미 동맹을 맺고 있다.
// en_ally_mode_error_cannot_ally,			// 102 흑/백도 의 이유로 동맹을 맺을 수 없다.
// en_ally_mode_error_not_found_info,		// 103 요청한 동맹을 찾을 수 없습니다.
// en_ally_mode_error_cannot_find_user,		// 104 유저를 찾을 수 없다.
// en_ally_mode_error_i_am_busy,			// 105 현재 요청중.
// en_ally_mode_error_busy,					// 106 대상이 다른 일 중.
// en_ally_mode_error_cannot_request,       // 107 영웅단이 없거나, 단주가 아니다. 
// en_ally_mode_error_not_hero,				// 108 대상은 영웅단 단주가 아니다.


struct MSG_ALLY_ERROR : public MSG_ALLY_BASE
{
};



#endif