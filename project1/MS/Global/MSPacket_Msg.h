#pragma once

#pragma pack( push, enter_ )
#pragma pack(1)

#include "Global_Define.h"
#include "..\NetWork\NetWork.h"
#include <list>
using namespace std;


class _FriendInfo ;
class _h_DS_Com ;

struct _Member {	
	char			characName[16] ;	// 캐릭터 이름 또는 그룹 이름으로 쓰인다.	
	_FriendInfo *	pFriendInfo ;		// 해당 _FriendList 의 *(포인터)
};

struct _Mail {							// 메일(전서구)
	UINT			uiIndex ;			// 메일 인덱스
	bool			check ;				// true : 읽음 , flase : 새로도착(안읽음)	
};


struct _MailInfo {
	UINT			uiIndex ;			// 메일 인덱스
	char			cFrom[13] ;			// 보낸사람
	struct _Date {						// 받은시간
		unsigned	year : 12 ;				
		unsigned	month : 4 ;
		unsigned	day : 5 ;
		unsigned	hour : 5 ;
		unsigned	minute : 6 ;
	} date;	// 4Byte

	char			cMode ;				// 구분
	enum
	{
		_PRIVATE,	
		_PUBLIC,
		_FROM_GM,
		_SYSTEM_MSG_OR_KICK,
	};

	char			cStrLen ;	
	char			cText[101] ;		// 내용

};

struct _Friend {
	char			cChoiceName[15] ;	// 친구 이름.. 또는 그룹 이름..
	char			cState	;			// 상태..
} ;


//==============================================================


// 연결이 끊겼을때 내부적으로만 쓰는 패킷
#define MSG_NO_DISCONNECT					0
struct MSG_DISCONNECT						//# 연결이 끊겼을 때마지막 패킷 #
{
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	
	u_char				cMessage ;	
	void*				pDSComm ;
} ;


// DS 로 부터 캐릭터 삭제를 통보받는다.
#define		UMSG_CHARAC_DELETE_DS_MS_NUM	125
struct UMSG_CHARAC_DELETE_DS_MS {
	int			iKey ;
	char		cMessage ;
	
	char		cCharacName[13] ;
	int			iUid ;
};


// GM Charac setting
#define		UMSG_GM_CHARAC_SET_DS_MS_NUM	126
struct UMSG_GM_CHARAC_SET_DS_MS {
	int			iKey ;
	char		cMessage ;
	
	char		cCharacName[13] ;
	char		cClass ;
};


enum
{
	en_or_name_length=24,
};

#define  UMSG_CHARAC_OR_KICK_NUM			127
struct UMSG_CHARAC_OR_KICK {
	int			iKey ;
	char		cMessage ;

	char		cCharacName[13] ;
	char		cORName[en_or_name_length+1] ;
};


#define		MSG_SYSTEM_OFF_REQ_NUM			10	// 시스템 종료 GMS -> DS
struct MSG_SYSTEM_OFF_REQ {
	int		iKey;
	char	cMessage;
	char	cServer_num;	
};



// GMS 로그인
#define  MMSG_NO_GM_LOGIN		18
struct MMSG_GM_LOGIN {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true

	u_char				cMessage ;
	char				cCharacName[13] ;	
};




//==============================================================
/*
설명 : 메신저 서버 패킷은 MMSG 로 시작한다..
		클라이언트와 서버는 패킷 해더의 sCrypto, sCompressed 의 값을 항상 확인해야 한다..
		sCrypto 값이 1 인 경우에는 패킷이 암호화 되어 있음을 의미한다.
		sCompressed 값이 1 인 경우에는 패킷이 압축되어 있음이다.(길이가 긴 채팅 메시지 경우 압축함)		
*/		


#define MMSG_NO_LOGIN_C_S				161
#define MMSG_NO_LOGIN_RESULT			162		// 로그인 결과..

#define MMSG_NO_LOGOUT_C_S				164		// 로그오프 요청
#define MMSG_NO_LOGOUT_REPORT_S_C		165		// 친구가 로그오프 했음..

#define MMSG_NO_MAKE_FRIEND_REQ_C_S_C	166		// 친구 맺기 제안..
#define MMSG_NO_MAKE_FRIEND_CON_C_S		168		// 친구 맺기 제안에 대한 등답(Yes/No)
#define MMSG_NO_MAKE_FRIEND_RES_S_C		169		// 친구 등록 요청 결과

#define MMSG_NO_CUT_FRIEND_REQ_C_S		170		// 친구 의절 요청..
#define MMSG_NO_CUT_FRIEND_REPORT_S_C	171		// 친구 삭제 알림~

#define MMSG_NO_FRIEND_LIST_REQ_C_S		172		// 서버에 친구 리스트 요청
#define MMSG_NO_FRIEND_LIST_RES_S_C		173		// 친구 리스트 결과

#define MMSG_NO_MY_STATE_UPDATE_C_S		174		// 나의 상태 정보 업데이트 요청
#define MMSG_NO_FRIEND_STATE_REPORT_S_C	175		// 친구의 상태 정보 변경 사항..
#define MMSG_NO_CHAT					176		// 일반 채팅 메시지..
#define MMSG_NO_CHAT_GM					177		// 운영자 채팅 메시지..
//#define MMSG_NO_CHAT_GROUP_CHOICE_C_S	177		// 그릅 채팅(확정 되지 않음..)

#define MMSG_NO_MAKE_GROUP_REQ_C_S		178		// 그릅 만들기 요청
#define MMSG_NO_MAKE_GROUP_RES_S_C		179		// 그릅 만들기 응답

#define MMSG_NO_CUT_GROUP_REQ_C_S		180		// 그릅 삭제 요청
#define MMSG_NO_CUT_GROUP_RES_S_C		181		// 그릅 삭제 응답

#define MMSG_NO_JOIN_GROUP_REQ_C_S		182		// 친구를 그릅에 등록하도록 요청..
#define MMSG_NO_JOIN_GROUP_RES_S_C		183		// 친구를 그릅에 등록하도록 요청..

#define MMSG_NO_RENAME_GROUP_REQ_C_S	184		// 그릅 이름 변경
#define MMSG_NO_RENAME_GROUP_RES_S_C	185		// 그릅 이름 변경 결과

#define MMSG_NO_CHAT_WHISPER			186		// 귓속말
#define MMSG_CHAT_RESULT_S_C			187		// 채팅 메시지 결과패킷..(실패했을 경우만 간다.)



//======================================================================================
// Mail(전서구) 
#define MMSG_NO_MAIL_LIST_REQ_C_S		200		// 메일 리스트를 요청한다
#define MMSG_NO_MAIL_LIST_RES_S_C		201		// 메일 리스트를 반환받는다.
#define MMSG_NO_MAIL_TEXT_REQ_C_S		202		// 메일 본문을 요청한다.
#define MMSG_NO_MAIL_TEXT_RES_S_C		203		// 메일 본문을 반환받는다.
#define MMSG_NO_MAIL_SEND_REQ_C_S		204		// 메일을 발송한다.
#define MMSG_NO_MAIL_SEND_RES_S_C		205		// 메일 발송에 대한 결과
#define MMSG_NO_MAIL_RECV_S_C			206		// 로그인 중에 메일이 도착했을 경우..
#define MMSG_NO_MAIL_FAIL_S_C			207		// 각종 요청에 대한 실패
#define MMSG_NO_MAIL_DROP_C_S			208		// 메일 삭제
#define MMSG_NO_MAIL_DROP_S_C			209		// 메일 삭제 결과
#define MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S	230	// 영웅단 메일 발송 요청

#define MMSG_NO_MAIL_SEND_REQ_FROM_GM	231		// 메일을 발송한다.



struct _sHeader {
	short		sLength : 12 ;		// -2048 ~ 2047
	short		sCrypto : 2 ;		// 0 : false , 나머지는 true
	short		sCompressed : 2 ;	//0 : false , 나머지는 true
};


#define MMSG_NO_CONNECT_PERM			0
struct MMSG_CONNECT_PERM {
	short				sLength : 12 ;
	short				sCrypto : 2 ;
	short				sCompressed : 2 ;
	u_char				cMessage ;
};

//#define MMSG_NO_LOGIN_C_S
struct MMSG_MSLOGIN {

	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cCharacName[13] ;	
};

//#define MMSG_NO_LOGIN_RESULT
struct MMSG_MSLOGIN_RESULT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cResult ;			// 0 : 성공

											// 1 : 잘못된 캐릭터 이름
											// 2 : 아이피 주소가 다름
											// 3 : 이미 로그인중...		
											// 4 : (db)기타오류												
};

//#define MMSG_NO_LOGOUT_C_S
struct MMSG_MSLOGOUT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;
};

//#define MMSG_NO_LOGOUT_REPORT_S_C
struct MMSG_LOGOUT_REPORT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cCharacName[13] ;
};

// #define MMSG_NO_MAKE_FRIEND_REQ_C_S_C	// 친구 맺기 제안..			<1>
struct MMSG_MAKE_FRIEND_REQUEST {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cCharacName[13] ;	
};


//#define MMSG_NO_MAKE_FRIEND_CON_C_S		// 친구 맺기 제안에 대한 응답(Yes/No)
struct MMSG_MAKE_FRIEND_CONFIRM {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cCharacName[13] ;	
	char				cResult ;			//0 : 승낙(yes) , 1 : 거부(no)
};

//#define MMSG_NO_MAKE_FRIEND_RES_S_C
struct MMSG_MAKE_FRIEND_RES {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cCharacName[13] ;
	char				cState ;			// 친구 상태 정보..
	char				cResult ;			// 0 : 성공	(양쪽다 통보)
											// 1 : 상대가 거부 (거부당한 캐릭터(cFromCharacName)에게 통보)

											//(여기 부터는 'MMSG_MAKE_FRIEND_REQ' Packet 을 보낸 캐릭터에게만 통보한다.)
											// 2 : 상대 캐릭터가 없거나.. 로그오프중..
											// 3 : 상대 캐릭터의 친구 허용 인원 초과..
											// 4 : 나의 캐릭터 허용 인원 초과.
											// 5 : 이미 등록된 캐릭터
											// 6 : GM 캐릭터 입니다.
};

//#define MMSG_NO_CUT_FRIEND_REQ_C_S		// 친구 의절..
struct MMSG_CUT_FRIEND_REQ {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;
	
	char				cFriendCharac[13] ;	// 삭제할 친구..

};

//#define MMSG_NO_CUT_FRIEND_REPORT_S_C		// 친구 의절 알림.. (삭제 대상자에게 자신이 삭제되었다는걸 알림..)
struct MMSG_CUT_FRIEND_REPORT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cCharacName[13] ;	

};

//#define MMSG_NO_FRIEND_LIST_REQ_C_S
struct MMSG_FRIEND_LIST_REQ {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;
};

//#define MMSG_NO_FRIEND_LIST_RES_S_C
struct MMSG_FRIEND_LIST_RES {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cCount ;			// 맽에꺼 갯수..
	_Friend				_friends[58];
};


enum
{
	_CHARAC_STATE_LOGOFF,	// 0.오프라인
	_CHARAC_STATE_LOGON,	// 1.온라인
	_CHARAC_STATE_FIGHT,	// 2.전투중
	_CHARAC_STATE_ABSENCE,	// 3.부재중
	_CHARAC_STATE_MOVING,	// 4.서버이동중
} ;


//#define MMSG_NO_MY_STATE_UPDATE_C_S
struct MMSG_MY_STATE_UPDATE {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cState ;
											// 0 로그오프
											// 1 온라인
											// 2 전투중											
											// 3 자리비움
											// 4 서버 이동중..
};

//#define MMSG_NO_FRIEND_STATE_REPORT_S_C
struct MMSG_FRIEND_STATE_REPORT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cCharacName[13] ;
	char				cState ;
};

//#define MMSG_NO_CHAT
//#define MMSG_NO_CHAT_GM					177		// 운영자 채팅 메시지..
struct MMSG_CHAT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;
	
	char				cFromCharacName[13] ;	// 챗팅 메시지의 경우에만 유일하게 From 과 To 가 존재.. 음..ㅡㅡ;
	char				cToCharacName[13] ; 
	char				cChatMsg[81] ;
};


// #define MMSG_CHAT_RESULT_S_C				
enum							// MMSG_CHAT_RESULT::cCode
{
	_NOT_FIND_USER_ = 0,		// 캐릭터가 로그오프중 이거나 없는 캐릭터 일때..
	//_LOGOFF_FRIEND_,			// 메신저 채팅에서 친구가 로그오프상태 일때..
} ;
struct MMSG_CHAT_RESULT
{
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cToCharacName[13] ;	//	대상 캐릭터 이름
	char				cCode ;				//	_NOT_FIND_USER_
};

////#define MMSG_NO_CHAT_GROUP_CHOICE_C_S	13
//struct MMSG_CHAT_GROUP_CHOICE {
//	short				sLength : 12 ;		// -2048 ~ 2047
//	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
//	short				sCompressed : 2 ;	//0 : false , 나머지는 true
//	char				cMessage ;
//	char				cChoiceNum ;
//	char				cChoiceName[13][58] ;	// 가변길이 친구 리스트..
//};


//#define MMSG_NO_MAKE_GROUP_REQ_C_S		// 그릅 만들기 요청
struct MMSG_MAKE_GROUP_REQ {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;
	
	char				cGroupName[15] ;
};

//#define MMSG_NO_MAKE_GROUP_RES_S_C		// 그릅 만들기 응답
struct MMSG_MAKE_GROUP_RES {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;
	
	char				cGroupName[15] ;
	char				cResult ;			// 0 성공, 1 실패
};											

//#define MMSG_NO_CUT_GROUP_REQ_C_S			// 그릅 삭제 요청
struct MMSG_CUT_GROUP_REQ {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cGroupName[15] ;
};
//#define MMSG_NO_CUT_GROUP_RES_S_C			// 그릅 삭제 응답
struct MMSG_CUT_GROUP_RES {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cGroupName[15] ;
	char				cResult ;
};

//#define MMSG_NO_JOIN_GROUP_REQ_C_S		// 친구를 그릅에 등록하도록 요청..
struct MMSG_JOIN_GROUP_REQ {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cGroupName[15] ;
	char				cFriendName[13] ;
};

//#define MMSG_NO_JOIN_GROUP_RES_S_C		// 친구를 그릅에 등록하도록 요청..
struct MMSG_JOIN_GROUP_RES {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cGroupName[15] ;
	char				cFriendName[13] ;
	char				cResult ;			// 0성공 1실패
};

//#define MMSG_NO_RENAME_GROUP_REQ_C_S		// 그릅 이름 변경
struct MMSG_RENAME_GROUP_REQ {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;
	
	char				cGroupName[15] ;
	char				cReName[15] ;
};

//#define MMSG_NO_RENAME_GROUP_RES_S_C		// 그릅 이름 변경 결과
struct MMSG_RENAME_GROUP_RES {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;
	
	char				cResult ;			// 0: 성공  1: 실패
};


//======================================================================================
// Mail(전서구) 

//struct _Mail {							// 메일(전서구)
//	UINT			uiIndex ;			// 메일 인덱스
//	bool			check ;				// false : 읽음 , thue : 새로도착(안읽음)	
//};
//
//struct _MailInfo {
//	UINT			uiIndex ;			// 메일 인덱스
//	char			cFrom[13] ;			// 보낸사람
//	int				iTime ;				// 시간
//	char			cText[101] ;		// 내용
//};



//#define MMSG_NO_MAIL_LIST_REQ_C_S		200		// 메일 리스트를 요청한다
struct MMSG_MAIL_LIST_REQ {
	_sHeader			header ;
	u_char				cMessage ;	
};
//#define MMSG_NO_MAIL_LIST_RES_S_C		201		// 메일 리스트를 반환받는다.
struct  MMSG_MAIL_LIST_RES{
	_sHeader			header ;
	u_char				cMessage ;

	_Mail				mail[MAX_MAIL_COUNT] ;
};
//#define MMSG_NO_MAIL_TEXT_REQ_C_S		202		// 메일 본문을 요청한다.
struct  MMSG_MAIL_TEXT_REQ {
	_sHeader			header ;
	u_char				cMessage ;
	u_int				uiIndex ;	
};
//#define MMSG_NO_MAIL_TEXT_RES_S_C		203		// 메일 본문을 반환받는다.
struct  MMSG_MAIL_TEXT_RES {
	_sHeader			header ;
	u_char				cMessage ;
	
	_MailInfo			mailInfo ;
};
//#define MMSG_NO_MAIL_SEND_REQ_C_S		204		// 메일을 발송한다.
//#define MMSG_NO_MAIL_SEND_REQ_FROM_GM	231		// GM 메일을 발송한다.
struct MMSG_MAIL_SEND_REQ {						// (가변길이)
	_sHeader			header ;
	u_char				cMessage ;				// header(3) + MailHeader(22) + strLen
	char				cToCharac[13] ;
	char				cStrLen ;

	char				cText[101] ;
};
//#define MMSG_NO_MAIL_SEND_RES_S_C		205		// 메일 발송에 대한 결과
struct MMSG_MAIL_SEND_RES {
	_sHeader			header ;
	u_char				cMessage ;
};

//#define MMSG_NO_MAIL_RECV_S_C			206		// 로그인 중에 메일이 도착했을 경우..
struct MMSG_MAIL_RECV {
	_sHeader			header ;
	u_char				cMessage ;
	u_int				uiIndex ;				// 도착한 메일의 인덱스
};


//#define MMSG_NO_MAIL_FAIL_S_C			206		// 각종 요청에 대한 실패
// cCmd : MMSG_NO_MAIL_LIST_RES_S_C : cResult == NULL
// cCmd : MMSG_NO_MAIL_TEXT_RES_S_C : cResult == NULL
// cCmd : MMSG_NO_MAIL_SEND_RES_S_C : cResult == 0 : 해당 캐릭터 이름 존재하지 않음
//				     							  1 : 상대방 메일박스가 꽉차서 더이상 못받음
//												  2 : 상대는 수신 거부중
//												  3 : 기타 오류
//												  4 : GM 에게는 쪽지를 보낼 수 없습니다.
//#define MMSG_NO_MAIL_FAIL_S_C			207		// 각종 요청에 대한 실패
struct MMSG_MAIL_FAIL {
	_sHeader			header ;
	u_char				cMessage ;
	u_char				cCmd ;			// define 된 메시지 번호를 그대로 사용한다.
	char				cResult ;		// 결과다.
};

//#define MMSG_NO_MAIL_DROP_C_S			208		// 메일 삭제
struct MMSG_MAIL_DROP {
	_sHeader			header ;
	u_char				cMessage ;
	u_int				uiIndex ;				// 삭제할 메일 인덱스..
};

//#define MMSG_NO_MAIL_DROP_S_C			209		// 메일 삭제 결과
struct MMSG_MAIL_DROP__RES {
	_sHeader			header ;
	u_char				cMessage ;
	u_int				uiIndex ;				// 삭제한 메일 인덱스..
};

//#define MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S	230	// 영웅단 메일 발송 요청
struct MMSG_MAIL_PUBLIC_SEND_REQ {
	_sHeader			header ;
	u_char				cMessage ;

	char				cNames[100][13] ;
	char				cText[101] ;
};



#pragma pack( pop, enter_ )

