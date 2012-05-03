#pragma once

#pragma pack( push, enter_ )
#pragma pack(1)

#include <list>
using namespace std;


// 2004.05.27->oneway48 insert : 전서구 
struct _Mail {							// 메일(전서구)
	UINT			uiIndex ;			// 메일 인덱스
	bool			check ;				// true : 읽음 , false : 새로도착(안읽음)	
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
	char			cMode ;
	enum
	{
		_PRIVATE,
		_PUBLIC,
		_FROM_GM,
		_SYSTEM_MSG_OR_KICK,	//[%s단] 에서\n 강제추방 되었습니다.\n
	};

	char			cStrLen ;
	char			cText[101] ;		// 내용
};
// 2004.05.27->oneway48 insert end 

struct _Member {	
	char			characName[15] ;	// 캐릭터 이름 또는 그룹 이름으로 쓰인다.	
	void *			pFriendInfo ;		// 해당 _FriendList 의 *(포인터)
};

struct _Friend {
	char			cChoiceName[15] ;	// 친구 이름.. 또는 그룹 이름..
	char			cState	;			// 상태..
} ;


/*
struct _FriendInfo {
	char		characName[13] ;	// 캐릭터 이름 또는 그룹 이름으로 쓰인다.	
	_h_DS_Com *	ds_com;				// NULL 이면 로그오프중		
	char		state ;				// 캐릭터 상태..
	char		option ;			// 친구 맺기 신청 거부, 귓말 거부.. 등..
	// 0 . 정상.
	// 1 . 친구 맺기 거부
	// 2 . 귓말 거부..
	
	//	int			Chat_Count ;		// Chat 명령이 보내질 
	//	_h_DS_Com * Chat_Com[50] ;
	
	// Chatting Character Info(2004.03.10 Add ozzywow)
	char		m_cChatting_CharacName[13] ;
	_h_DS_Com *	m_pChatting_DS_Com ;
	
	char		cBackupMode ;		// 0. None
	// 1. Backup Ready
	
	CRITICAL_SECTION	cs ;
	
	_FriendInfo() {
		InitializeCriticalSectionAndSpinCount( &cs, (0xf0000000)|2000 ) ;
	} ;
	~_FriendInfo() { DeleteCriticalSection( &cs ) ; } ;
	void lock() { EnterCriticalSection(&cs) ; } ;
	void unlock() { LeaveCriticalSection(&cs) ; } ;
	//void lock(){ return ;} ;
	//void unlock(){ return;} ;
	
	list<_Member> * pFriendList ;
};
*/

//==============================================================

/*
// 연결이 끊겼을때 내부적으로만 쓰는 패킷
#define MSG_NO_DISCONNECT					-99
struct MSG_DISCONNECT						//# 연결이 끊겼을 때마지막 패킷 #
{
	unsigned short		sLength;
	char				cMessage ;	
	void*				pDSComm ;
} ;
*/

//==============================================================


//==============================================================
/*
설명 : 메신저 서버 패킷은 MMSG 로 시작한다..
		클라이언트와 서버는 패킷 해더의 sCrypto, sCompressed 의 값을 항상 확인해야 한다..
		sCrypto 값이 1 인 경우에는 패킷이 암호화 되어 있음을 의미한다.
		sCompressed 값이 1 인 경우에는 패킷이 압축되어 있음이다.(길이가 긴 채팅 메시지 경우 압축함)		
*/		

#define MMSG_NO_CONNECT_PERM			0 //2004.06.22->oneway48 insert

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
#define MMSG_NO_CHAT					176		// 채팅 메시지..
#define MMSG_NO_CHAT_GM					177		// 2004.05.20->oneway48 insert : 운영자 채팅 메시지
//#define MMSG_NO_CHAT_GROUP_CHOICE_C_S	177		// 그릅 채팅(확정 되지 않음..)

#define MMSG_NO_MAKE_GROUP_REQ_C_S		178		// 그릅 만들기 요청
#define MMSG_NO_MAKE_GROUP_RES_S_C		179		// 그릅 만들기 응답

#define MMSG_NO_CUT_GROUP_REQ_C_S		180		// 그릅 삭제 요청
#define MMSG_NO_CUT_GROUP_RES_S_C		181		// 그릅 삭제 응답

#define MMSG_NO_JOIN_GROUP_REQ_C_S		182		// 친구를 그릅에 등록하도록 요청..
#define MMSG_NO_JOIN_GROUP_RES_S_C		183		// 친구를 그릅에 등록하도록 요청..


#define MMSG_NO_RENAME_GROUP_REQ_C_S	184		// 그릅 이름 변경 요청 
#define MMSG_NO_RENAME_GROUP_RES_S_C	185		// 그릅 이름 변경 결과
#define MMSG_NO_CHAT_WHISPER			186		// 귓속말
#define MMSG_CHAT_RESULT_S_C			187		// 귓속말 상대 없을 경우 


// 2004.07.23->oneway48 insert 
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
// insert end

// 영웅단 전체 메일
//======================================================================================
#define MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S 230 // 영웅단 메일 발송 요청


// 친구 소환
//======================================================================================
#define MSG_NO_RECALL_FRIEND			226		// 친구소환.

// 특정 정보 알리기
//======================================================================================
#define MSG_NO_NOTIEFY_INFO				227		// 주위에 특정 정보를 알린다.


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
	char				cState;				// 상태 정보 필요
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

//#define MMSG_NO_MY_STATE_UPDATE_C_S
struct MMSG_MY_STATE_UPDATE {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cState ;
								// 0 로그오프
								// 1 온라인
								// 2 전투 중
								// 3 부재 중
								// 4 서버이동 중								
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
//#define MMSG_NO_CHAT_GM					// 2004.05.20->oneway48 insert : 177 운영자 채팅 메시지..
//#define MMSG_NO_CHAT_WHISPER			186		// 귓속말
struct MMSG_CHAT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cFromCharacName[13] ;	// 챗팅 메시지의 경우에만 유일하게 From 과 To 가 존재.. 음..ㅡㅡ;
	char				cToCharacName[13] ; 
	char				cChatMsg[_XDEF_CHATMAXLENGTH] ;
};

// 2005. 1. 28 insert --------------------------------------------------------------=
enum	// MMSG_CHAT_RESULT::cCode
{
	_NOT_FIND_USER_ = 0,		// 캐릭터가 로그오프중 이거나 없는 캐릭터 일때..
} ;

struct MMSG_CHAT_RESULT
{
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
	short				sCompressed : 2 ;	//0 : false , 나머지는 true
	u_char				cMessage ;

	char				cToCharacName[13] ;	//	대상 캐릭터 이름
	char				cCode ;				//	_NOT_FIND_USER_
} ;
// ---------------------------------------------------------------------------------=

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

// 2004.07.23->oneway48 insert 
//======================================================================================
// Mail(전서구) 

//struct _Mail {							// 메일(전서구)
//	UINT			uiIndex ;			// 메일 인덱스
//	bool			check ;				// true : 읽음 , false : 새로도착(안읽음)	
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

	_Mail				mail[20] ;	
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

// insert end

//2004.06.22->oneway48 insert
//#define MMSG_NO_CONNECT_PERM			0
struct MMSG_CONNECT_PERM {
	short				sLength : 12 ;
	short				sCrypto : 2 ;
	short				sCompressed : 2 ;
	u_char				cMessage ;
};

//#define MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S 230 // 영웅단 메일 발송 요청
struct MMSG_MAIL_PUBLIC_SEND_REQ {
 _sHeader   header ;
 u_char    cMessage ;

 char    cNames[100][13] ;
 char    cText[101] ;
};

//#define MSG_NO_RECALL_FRIEND			226		// 친구소환.
struct MSG_RECALL_FRIEND
{
	_sHeader	header;
	u_char	ucMessage;

	u_char	ucFriendMode;	// EN_MSG_RECALL_FRIEND
};

enum EN_MSG_RECALL_FRIEND
{
	en_rf_send_request_recall=0,		// 친구 소환 요청을 서버로 전송
	en_rf_recv_request_recall,			// 친구 소환 요청을 서버에서 받음.

	en_rf_send_request_goto,	// 친구가 있는 곳으로 가겠다는 요청.
	en_rf_recv_request_goto,	// 바로 위의 요청을 받았다.

	en_rf_send_answer_yes=10,			// 친구 소환 요청 수락.
	en_rf_send_answer_no,				// 친구 소환 요청 거부.

	en_rf_send_goto_answer_yes,//
	en_rf_send_goto_answer_no,

	en_rf_recv_answer_yes=20,			// 요청 수락 패킷을 받음. ==> 성공했다.
	en_rf_recv_answer_no,				// 요청 거부 패킷을 받음.

	en_rf_recv_goto_answer_yes,// 
	en_rf_recv_goto_answer_no,// 

	en_rf_show_me=30,	//소환되었음을 알림
	en_rf_show_animation,	// 이동 직전/이동 직후에 주변에 알려, 애니메이션 하도록 한다.

	en_rf_fail_to_request=50,			// 요청이 실패했다. 
};

// en_rf_send_request_recall=0,		// 친구 소환 요청을 서버로 전송
struct MSG_RF_SEND_REQUEST_RECALL : public MSG_RECALL_FRIEND
{
	u_char	cNeedItemSlot;	// 필요 아이템 슬롯의 인덱스. 서버에서 검사시에 쓰인다.

	char	cTargetName[en_charac_name_length+1];	// 대상의 이름.

	float	fTargetX;	// 클라이언트에서 미리 계산하여, 목표점을 보낸다.
	float	fTargetZ;	// 서버에서는 거리(5) 검사만 한다.
};

// en_rf_recv_request_recall, // 친구 소환 요청을 서버에서 받음.
struct MSG_RF_RECV_REQUEST_RECALL : public MSG_RECALL_FRIEND
{
	u_char	ucFromZoneIndex;	// 어느 지역에서 소환하고 있는가.
	char	cFromName[en_charac_name_length+1];
};

//en_rf_send_request_goto, // 친구가 있는 곳으로 가겠다는 요청.
struct MSG_RF_SEND_REQUEST_GOTO : public MSG_RECALL_FRIEND
{
	u_char	ucNeedItemSlot;
	char	cTargetName[en_charac_name_length+1];
};

//en_rf_recv_request_goto, // 바로 위의 요청을 받았다.
struct MSG_RF_RECV_REQUEST_GOTO : public MSG_RECALL_FRIEND
{
	char	cFromName[en_charac_name_length+1];
};
	
// en_rf_send_answer_yes=10, // 친구 소환 요청 수락.
struct MSG_RF_SEND_ANSWER_YES : public MSG_RECALL_FRIEND
{
	char cFromName[en_charac_name_length+1];
};


//	en_rf_send_answer_no, // 친구 소환 요청 거부.
struct MSG_RF_SEND_ANSWER_NO : public MSG_RECALL_FRIEND
{
	u_char	ucNoReason;		
	char	cFromName[en_charac_name_length+1];
	/*
		ucNoReason : 
		0 - 거절을 선택했을 때.
		1 - 현재 요청을 받을 수 없는 상태다. 
			( 요청받았을 때, 상태검사룰 수행하여, 못 받는 상태이면 이 값으로 보낸다. )
	*/
};

//en_rf_send_goto_answer_yes,//
struct MSG_RF_SEND_GOTO_ANSWER_YES : public MSG_RECALL_FRIEND
{
	char	cFromName[en_charac_name_length+1];
	float	fTargetX;	// 위치를 채워서 보낸다. 
	float	fTargetZ;
};

//en_rf_send_goto_answer_no,//
struct MSG_RF_SEND_GOTO_ANSWER_NO : public MSG_RECALL_FRIEND
{
	u_char	ucNoReason;
	char	cFromName[en_charac_name_length+1];
};

//	en_rf_recv_answer_yes=20, // 요청 수락 패킷을 받음. ==> 성공했다.
struct MSG_RF_RECV_ANSWER_YES : public MSG_RECALL_FRIEND
{
	char	cTargetName[en_charac_name_length+1]; // 초기에 요청보냈던 상대의 이름.
	u_char	ucItemSlot;	// 친구 소환 아이템이 있던 자리.
	u_char	ucLeftCount;	// 남은 개수. 0이면 완전 삭제.
};


//	en_rf_recv_answer_no, // 요청 거부 패킷을 받음.
struct MSG_RF_RECV_ANSWER_NO : public MSG_RECALL_FRIEND
{
	u_char	ucNoReason;	// MSG_RF_SEND_ANSWER_NO 의 ucReason 과 같다.
	char	cTargetName[en_charac_name_length+1];
};



//en_rf_recv_goto_answer_yes,                              // 
struct MSG_RF_RECV_GOTO_ANSWER_YES : public MSG_RECALL_FRIEND
{
	char	cTargetName[en_charac_name_length+1];
	u_char	ucItemSlot;
	u_char	ucLeftCount;
	u_char	ucTargetZone;	// 필요할지 모르겠음.

	float	fTargetX;
	float	fTargetZ;
};

//en_rf_recv_goto_answer_no,                                // 
struct MSG_RF_RECV_GOTO_ANSWER_NO : public MSG_RECALL_FRIEND
{
	u_char	ucNoReason;	// EN_RF_FAIL_REASON

	char	cTargetName[en_charac_name_length+1];
};

// en_rf_show_me=30,//소환되었음을 알림
struct MSG_RF_SHOW_ME : public MSG_RECALL_FRIEND
{
	u_short	uiUID;
	char	cName[en_charac_name_length+1];
	
	float	fPosX;
	float	fPosZ;
};

//en_rf_show_animation=31
struct MSG_RF_SHOW_ANIMATION : public MSG_RECALL_FRIEND
{
	u_char ucRFAni; // 0 - 소환자(기다리는캐릭) 마법진. 1 - 이동자의 애니메이션, 2 - 이동후의 애니메이션.
	u_short	usUID;	// 자기라면 무시한다.
};

enum
{
	en_rf_answer_no=0,	// no 선택.
	en_rf_fail_no_target=1,	// 1 타겟이 없다.
	en_rf_fail_no_item,	// 소환 아이템 없다. 
	en_rf_fail_wrong_point,	// 소환 지점의 위치가 이상하다. 
	en_rf_fail_enemy,	// 적대 세력이다.
 
	en_rf_fail_my_con_trade=10, // (자신은) 거래중이다.
	en_rf_fail_my_con_combat, // (자신은) 장원전 참여중이다.
	en_rf_fail_my_con_zone,	// (자신은) 패왕동/호출할 수 없는 지역이다.
	en_rf_fail_my_con_exbattle, // (자신은) 결 진행중이다. 
	en_rf_fail_my_busy,	// (자신은) 다른 캐릭터와 친구 소환 중이다.   
    en_rf_fail_my_state_pk_mode,	// (자신은) PK 모드이다.
    
	en_rf_fail_target_con_trade=20,	// 거래중이다.
	en_rf_fail_target_con_combat, // 장원전 참여중이다.
	en_rf_fail_target_con_zone, // 패왕동/호출할 수 없는 지역이다.
	en_rf_fail_target_con_exbattle, // 결 진행중이다.
	en_rf_fail_target_busy, // 다른 대상과 친구 소환 중이다.
    en_rf_fail_target_state_pk_mode,// 대상이 현재 PK 모드다.
};

//	en_rf_fail_to_request=50, // 요청이 실패했다. 
struct MSG_RF_FAIL_TO_REQUEST : public MSG_RECALL_FRIEND
{
	u_char	ucFail;			
	char	cTargetName[en_charac_name_length+1];
	/*
		ucFail
		  - 0 : 대상은 로그인 중이 아니다.
		  - 1 : 소환 아이템이 없다.(또는 아니다. )
		  - 2 : 요청을 보낼 수 없는 상태다.
		    ( 클라이언트에서 먼저 검사하고 보내므로, 2 값은 갈일이 없을 듯 )
		  - 3 : 소환 위치가 캐릭터와 너무 멀다.
	*/
};

//#define MSG_NO_NOTIEFY_INFO				227		// 주위에 특정 정보를 알린다.
struct MSG_NOTIFY_INFO 
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			notify_type;		// 아래의 EN_MSG_NOTIFY_INFO_TYPE  

};

enum EN_MSG_NOTIFY_INFO_TYPE
{
	en_notify_type_item_use = 0,
};

struct MSG_NI_ITEM_USE : public MSG_NOTIFY_INFO
{
	u_short			usUniqueID;			// 사용자의 아이디

	char			cItemType;			// 사용된 아이템 정보
	short			sItemID;
};

#pragma pack( pop, enter_ )