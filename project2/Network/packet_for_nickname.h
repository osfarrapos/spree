#pragma once



//#################################################################################
//
//  Nic 패킷 
//  27: 216 ~ 223
//
//#################################################################################


struct _sHeader ;

#define MSG_NO_NIC_LIST_REQ_C_S			216			// 별호 리스트를 요청
#define MSG_NO_NIC_LIST_S_C				216			// 별호 리스트를 반환
//#define MSG_NO_NIC_ADD_REQ_C_S			217			// 별호 추가를 요청
#define MSG_NO_NIC_SET_S_C				217			// 슬롯에 별호를 설정한다.
#define MSG_NO_NIC_DELETE_REQ_C_S		217			// 별호 삭제
#define MSG_NO_NIC_SELECT_REQ_C_S		218			// 별호를 선택한다.
#define MSG_NO_NIC_SELECT_S_C			218			// 선택한 별호 정보를 반환한다.
#define MSG_NO_NIC_RES					219			// 기타 REQ 패킷에 대한 성공 또는 실패 반환값..
#define MSG_NO_NIC_USEFULL_TIME_S_C		220			// 선택한 별호의 남은 유효 사용시간(초)
#define MSG_NO_RANK_HIDING				221			// 은신 설정,해제  ( C<->S ) 
#define MSG_NO_HONOR_PLAYER				222			// 플레이어의 명성 평가

//#define MSG_NO_NIC_LIST_REQ_C_S
struct MSG_NIC_LIST_REQ {
	_sHeader	header ;
	u_char		ucMessage ;	
};
//#define MSG_NO_NIC_LIST_S_C	
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
			unsigned short   usNothing[_NIC_COUNT_NOTHING] ;   // 0번 슬롯은 아무것도 선택하지 않았을때를 생각해 비워둔다.
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
struct MSG_NIC_LIST {
	_sHeader	header ;
	u_char		ucMessage ;
	
	_CHARAC_NIC	nicData ;
};

/*
//#define MSG_NO_NIC_ADD_REQ_C_S
struct MSG_NIC_ADD_REQ {
	_sHeader	header ;
	u_char		ucMessage ;

	char		cSlotNum ;
	u_short		usNicID ;
};
*/

//#define MSG_NO_NIC_SET_S_C	
struct MSG_NIC_SET {
	_sHeader	header ;
	u_char		ucMessage ; 

	short		sSlotNum ;
	u_short		usNicID ;
};
//#define MSG_NO_NIC_SELECT_REQ_C_S
struct MSG_NIC_SELECT_REQ {
	_sHeader	header ;
	u_char		ucMessage ;

	short		sSlotNum ;	// 보안을 위해.. 슬롯번호로 요청한다. (0 이면.. 선택 해제, -1 이면 개인별호 선택 )	
	enum{
		_SELECT_FREE = 0,		// 별호 선택 해제
		_SELECT_PERSONAL = -1,	// 개인 별호 선택
		_SELECT_FREEMATCH = -2,	// 자유 비무 별호 선택
	} ;
};

//#define MSG_NO_NIC_SELECT_S_C	
struct MSG_NIC_SELECT {
	_sHeader	header ;
	u_char		ucMessage ;
	
	u_short		usID ;			// 캐릭터의 uid
	u_short		usNicID ;		// 슬롯번호로 요청받아.. 아이디로 반환한다. (자신을 포함한 주변에 보내진다.)
								// 0 이면 선택 해제..	'0xff' 이면 개인별호 선택
	enum{
		_SELECTED_FREE_ = 0,			// 별호 선택 해제됨
		_SELECTED_PERSONAL_ = 0xffff,		// 개인 별호 선택됨
	} ;
};
//MSG_ZONE_CHARAC_INFO 참고

//#define MSG_NO_NIC_RES		
struct MSG_NIC_RES {
	_sHeader	header ;
	u_char		ucMessage ;

	u_char		ucCommand ;
	u_char		ucCode ;
};

/*============================================================================

  MSG_NIC_RES 설명

	ucCommand : 어떤 패킷에 대한 결과 패킷인지 구분하기 위해..
	ucCode	  : 성공 또는 실패 코드이다..

--------------------------------------------+-----------------------------------+---------------------------
 	ucCommand								|	iCode							|		뜻
--------------------------------------------+-----------------------------------+---------------------------
 	MSG_NO_NIC_LIST_REQ_C_S					|	_RESULT_CODE_SUCCESS			|	? (성공하면 리스트가 감)
 											|	_RESULT_CODE_NOTCASE			|	조건 불만족
											|	_RESULT_CODE_ERROR				|	기타 오류
--------------------------------------------+-----------------------------+---------------------------------
	MSG_NO_NIC_UPDATE_REQ_C_S				|	_RESULT_CODE_SUCCESS			|	앗싸~ 성공
											|	_RESULT_CODE_NOTCASE			|	조건 불만족
											|	_RESULT_CODE_ERROR				|	기타 오류
--------------------------------------------+-----------------------------+---------------------------------
	MSG_NO_NIC_ADD_REQ_C_S					|	_RESULT_CODE_SUCCESS			|	앗싸~ 성공
											|	_RESULT_CODE_NOTCASE			|	실패
											|	_RESULT_CODE_ERROR				|	기타 오류
--------------------------------------------+-----------------------------+---------------------------------
	MSG_NO_NIC_SELECT_REQ_C_S				|	_RESULT_CODE_SUCCESS			|	? (성공하면 선택한 NicID 가 감)
											|	_RESULT_CODE_NOTCASE			|	실패
											|	_RESULT_CODE_ERROR				|	기타 오류
--------------------------------------------+-----------------------------+---------------------------------
*/

//#define MSG_NO_NIC_USEFULL_TIME_S_C		220			// 선택한 별호의 남은 유효 사용시간(초)
struct MSG_NIC_USEFULL_TIME {
	_sHeader	header ;
	u_char		ucMessage ;

	short		time ;
};

//#define MSG_NO_NIC_DELETE_REQ_C_S		217			// 별호 삭제 요청
struct MSG_NIC_DELETE_REQ {
	_sHeader	header ;
	u_char		ucMessage ;

	short		sSlotNum ;	//	- 삭제할 슬롯번호	
};

//#define MSG_NO_RANK_HIDING				221			// 은신 설정,해제  ( C<->S ) 
struct MSG_RANK_HIDING
{
	_sHeader	header ;
	u_char		ucMessage ;
	char		cMode ;
	enum
	{
		_HIDE_OFF=0,	// 은신 해제
		_HIDE_ON,		// 은신 설정
	};
};

//#define MSG_NO_HONOR_PLAYER				222			// 플레이어의 명성 평가
struct MSG_HONOR_PLAYER 
{
	_sHeader	header ;
	u_char		ucMessage ;
	
	char		cMode ;
	enum
	{
		_GIVE_REQ = 0,				// 명성 부여 요청
		_GIVE_SUCCESSED,			// 명성 부여 성공
		_GIVE_FAILED,				// 명성 부여 실패(시간 제한)
		_TAKE_FROM_PLAYER,			// PC로 부터 명성을 부여받았습니다.
		_TAKE_FROM_SERVER,			// server 로 부터 명성을 부여 받았습니다.
	} ;
	char		cToCharacName[13] ;
	int			iHohorValue ;		// +부여, - 하락
};