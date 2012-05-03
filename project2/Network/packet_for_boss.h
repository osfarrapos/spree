#pragma once

/*========================================================================================

  예외전투 방 (보스전방, 수련방 등..) 

  Message Scope : 23( 184 ~ 191), 24 ( 192 ~ 199 )

  우리 서버는 패킷번호가 고갈되어 가고 있다.  아껴쓰자.. 

=========================================================================================*/

#define MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S	184	// 전투방 생성 요청
#define MSG_NO_EX_BATTLE_INVITE_S_C				185	// 전투방 생성후 해당 캐릭들에게 초대
#define MSG_NO_EX_BATTLE_ENTER_REQ_C_S			186	// 초대받은 캐릭터가 전투방에 입장할때..
#define MSG_NO_EX_BATTLE_LEAVE_REQ_C_S			187	// 방에 있는 캐릭터가 나갈때..

#define MSG_NO_EX_BATTLE_RESULT_S_C				188	// REQ 메시지들에 대한 응답 메시지
#define MSG_NO_EX_BATTLE_PLAY_VIDEO				189	// 플래시 무비를 보여줘라..
#define MSG_NO_EX_BATTLE_TIME_S_C				190	// 서버와 클라이언트의 시간 동기화..

#define MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S		191 // 룸 타입에 대한 정보를 얻어온다.
#define MSG_NO_EX_BATTLE_TYPE_INFO_RES_S_C		192 // 람 타입에 대한 정보 응답.
#define MSG_NO_EX_BATTLE_CUR_STAGE_S_C			193	// 진행중인 보스전의 현재 스테이지 번호..


// 룸 모드
//enum {
//	_MODE_EX_BATTLE_ROOM_NORMAL,
//		_MODE_EX_BATTLE_ROOM_SOLO,
//		_MODE_EX_BATTLE_ROOM_PARTY,
//};

//#define MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S		// 전투방 생성 요청
struct  MSG_EX_BATTLE_ROOM_CREATE_REQ 
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cType ;						// 생성할 보스방 맵번호	
	char				cLevel ;					// 보스룸 난이도 (0-easy, 1-normal, 2-hard, 3-expert)
	char				cEnterItemSlotNum[3] ;		// 입장용 아이템이 있는 인벤 슬롯 번호..

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI	
	char				cTicket;					// 패(캐쉬템)인지 령인지 구분 (0 : 령3개소모    1 : 패1개소모)
#endif

	unsigned char		uMode : 4 ;					// 진행 종료(0.Normal, 1.개인, 2.동행 )	
	unsigned char		uPadding : 4 ;

};

//#define MSG_NO_EX_BATTLE_INVITE_S_C					// 전투방 생성후 해당 캐릭들에게 초대
struct  MSG_EX_BATTLE_INVITE {
	_sHeader			header ;
	u_char				ucMessage ;

	int					iRoomID ;					// 룸 멤버쉽 아이디? ( 방 열쇠 역할 ^^ )
	char				cType ;
	float				fX ;
	float				fZ ;
};

//#define MSG_NO_EX_BATTLE_ENTER_REQ_C_S				// 초대받은 캐릭터가 전투방에 입장할때..
struct  MSG_EX_BATTLE_ENTER {
	_sHeader			header ;
	u_char				ucMessage ;

	int					iRoomID ;					// 룸 멤버쉽 아이디? ( 방 열쇠 역할 ^^ )
};

// LEAVE 패킷이 과연 필요할까? 우선 만들어보세~
//#define MSG_NO_EX_BATTLE_LEAVE_REQ_C_S				// 방에 있는 캐릭터가 나갈때..
struct  MSG_EX_BATTLE_LEAVE {
	_sHeader			header ;
	u_char				ucMessage ;

	char				cCause ;					// 나가는 이유? ( 0. 탈출 1.사망, 2.로그아웃)	
};




/*============================================================================

  MSG_NO_EX_BATTLE_RESULT_S_C 설명

	ucCommand : 어떤 패킷에 대한 결과 패킷인지 구분하기 위해..
	iCode	  : 성공 또는 실패 코드이다..
	
  --------------------------------------------+-----------------------------------+---------------------------
  ucCommand								|	iCode							|		뜻
  --------------------------------------------+-----------------------------------+---------------------------
  MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S	|	_RESULT_CODE_SUCCESS			|	앗싸~ 성공
									    |	_RESULT_CODE_NOTCASE			|	조건 불만족
										|	_RESULT_CODE_ERROR				|	더이상 방 생성 불가
										|	_RESULT_CODE_ALREADY_PARTY		|	파티가 이미 보스전중이다.											
  --------------------------------------------+-----------------------------+---------------------------------
  MSG_NO_EX_BATTLE_ENTER_REQ_C_S		|	_RESULT_CODE_SUCCESS			|	앗싸~ 성공
										|	_RESULT_CODE_NOTCASE			|	조건 불만족
										|	_RESULT_CODE_ERROR				|	기타 오류
  --------------------------------------------+-----------------------------+---------------------------------
  MSG_NO_EX_BATTLE_LEAVE_REQ_C_S		|	_RESULT_CODE_SUCCESS			|	앗싸~ 성공
										|	_RESULT_CODE_NOTCASE			|	실패 (이땐 어쩌지?  ㅠ.ㅠ)
										|	_RESULT_CODE_ERROR				|	기타 오류
  --------------------------------------------+-----------------------------+---------------------------------
  MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S	|	_RESULT_CODE_ALREADY_PARTY		|	파티가 이미 보스전중이다.
  --------------------------------------------+-----------------------------+---------------------------------


=============================================================================*/

enum{
	_RESULT_CODE_SUCCESS,
		_RESULT_CODE_NOTCASE,
		_RESULT_CODE_ERROR,
		_RESULT_CODE_ALREADY_PARTY,
};

//#define MSG_NO_EX_BATTLE_RESULT_S_C					// REQ 메시지들에 대한 응답 메시지
struct  MSG_EX_BATTLE_RESULT 
{
	_sHeader			header ;
	u_char				ucMessage ;

	u_char				ucCommand ;					// 결과패킷과 연관된 ucMessage 번호
	int					iCode ;						// 상세정보

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
	// 만약 패나 령을 사용하였으면 결과로 인벤 위치와 갯수를 리턴한다,
	struct 
	{
		char			itemSlotNum;				// 위치
		u_char			itemCount;					// 갯수
	}_sUsedItem[3];

#endif

};

/*
보스전 비디오 관련 통신

  1. 파티장이 비디오 상영 중간에 _SKIP_VIDEO_ 를 보내면 서버는 보스몹을 소환하거나
  탈출구를 여는등의 이벤트를 발생시키고 이 메시지를 모든 캐릭들에게
  브로드케스팅 하게 된다. 이 메시지를 받는 캐릭터들은 관람중이던 비디오를 꺼버린다.
  
	2. 파티원중에 혹은 솔로보스전 에서 개인이 비디오를 다 봤을때(중간에 스킵 없이)는 서버에
	_END_VIDEO_메시지를 보낸다. 서버는 이 메시지를 받고 보스 몬스터를 소환하거나
	탈출구를 열것이다.
*/
enum{	// uCommand
	_SKIP_VIDEO_,
	_PLAY_VIDEO_,
	_END_VIDEO_,
} ;
enum{	// uStep
	_NOTHING_,
	_BOSS_STEP_,	
	_FINAL_STEP,
} ;
//#define MSG_NO_EX_BATTLE_PLAY_VIDEO				// 플래시 무비를 보여줘라..
struct MSG_EX_BATTLE_PLAY_VIDEO {
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cType ;						// 맵 번호
	u_char				uCommand ;					// 0. _SKIP_VIDEO_,  1. _PLAY_VIDEO_
	u_char	            uStep ;				   // 몇번째 비디오?(0부터 시작) 
};

//#define MSG_NO_EX_BATTLE_TIME_S_C					// 서버와 클라이언트에게 종료시간을 알려준다.(마지막꺼로 갱신)
struct MSG_EX_BATTLE_TIME {
	_sHeader			header ;
	u_char				ucMessage ; 

	u_short				usTimeCount ;				// 앞으로 남은 시간..
};

//#define MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S		191 // 룸 타입에 대한 정보를 얻어온다.
struct MSG_EX_BATTLE_TYPE_INFO_REQ {
	_sHeader			header ;
	u_char				ucMessage ; 
	char				cType ;
};


//#define MSG_NO_EX_BATTLE_TYPE_INFO_RES_S_C		192 // 룸 타입에 대한 정보 응답.
struct MSG_EX_BATTLE_TYPE_INFO 
{
	_sHeader			header ;
	u_char				ucMessage ;

	struct			// 필요한 아이템 정보
	{
		char	cType ;		// 대분류..
		short	sID ;		// 아이템 아이디..
	} _sItem[3] ;

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI

	char				isTicket;			// 인벤에 해당 결에 대한 패가 있는가? 1:있다 0:없다

	struct		// 캐시 아이템 정보
	{
		char			cType;
		short			sID;
	}_sChargedItem[3];

#endif
};

// #define MSG_NO_EX_BATTLE_CUR_STAGE_S_C			193	// 진행중인 보스전의 현재 스테이지 번호..
struct MSG_EX_BATTLE_CUR_STAGE {
	_sHeader			header ;
	u_char				usMessage ;
	
	char				cCurStage ;			// 현재 스테이지 번호..
};
