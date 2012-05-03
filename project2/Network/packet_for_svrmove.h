#pragma once

// Server 이동 관련
// Main = 19
// 152 - 159

struct _sHeader;

// Server간 이동, 접속 관련
#define MSG_NO_SV_ENTER_GAME			152			// 로그인 서버로 부터 게임 서버에 들어간다
#define MSG_NO_SV_ENTER_GSERVER			153			// 다른 게임 서버에서 이 서버로 들어온다.
#define MSG_NO_SV_ENTER_RESULT			154			// 유저의 접속 요청을 거부한다. 보낸 직후에 closesocket
#define MSG_NO_SV_MOVE_REQ				155			// 존 이동 요청 : 관문 또는 NPC를 이용한 서버 이동시
#define MSG_NO_SV_TOGSERVER				156			// 갈 서버의 ip, port를 가르쳐 준다.

//#define MSG_NO_SV_ENTER_GAME			104
struct MSG_SV_ENTER_GAME
{
	_sHeader			header;
	u_char				ucMessage;

	char				ID[25];

	u_short				usUniqueID;
};

// 게임 서버간 이동시 목표 서버에 연결 직후 보내야 한다. 안보내면 끊어짐.
//#define MSG_NO_SV_ENVER_GSERVER		105
struct MSG_SV_ENTER_GSERVER
{
	_sHeader			header;
	u_char				ucMessage;

	char				ID[25];

	u_short				usUniqueID;
};

//#define MSG_NO_SV_ENTER_RESULT		106
struct MSG_SV_ENTER_RESULT
{
	_sHeader			header;
	u_char				ucMessage;

	char				cResult;					// 0 성공 !0 실패
	char				cReserved;
};

//#define MSG_NO_SV_MOVE_REQ			107
struct MSG_SV_MOVE_REQ
{
	_sHeader			header;
	u_char				ucMessage;
	
	bool				bGate_NPC;					// 관문을 통한 이동이면 false, NPC를 통한 이동이면 true

	int					iNo;						// 관문이나 NPC 인덱스
	char				cGotoZone;
};

//#define MSG_NO_SV_TOGSERVER			108
struct MSG_SV_TOGSERVER
{
	_sHeader			header;
	u_char				ucMessage;

	char				cZoneNumber;				// 클라이언트가 어떤 존을 로딩할지 결정해준다. 
	                                                // == 0 일경우 서버 OFF

	unsigned short		usServerPort;				// 클라이언트 htons로 바꿔서 사용한다.
	unsigned long		ulServerIP;					// 클라이언트 이값을 그대로 사용한다.
};