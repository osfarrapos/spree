#pragma once

struct _sHeader;

// Level 
// Main = 13
// 104 - 111

#define MSG_NO_SET_LEVEL			104		// 캐릭터의 경험치와 맥스 체력 값
#define MSG_NO_SET_POWER			105		// 캐릭터의 현재 power 값
#define MSG_NO_LEVEL_UP_REQ			106
#define MSG_NO_POINT_USE			107

//#define MSG_NO_SET_LEVEL			64
struct MSG_SET_LEVEL
{
	_sHeader			header;
	u_char				ucMessage;

	unsigned int		uiJin;				// 진
};

//#define MSG_NO_SET_POWER			65
struct MSG_SET_POWER
{
	_sHeader			header ;
	u_char				ucMessage ;				// 필요한가..
	bool				bMax ;					// true : 최대값 셋팅, false : 현재값 셋팅.
	
	short				sLifePower ;			// 
	short				sForcePower ;			//
	short				sConcentrationPower ;			//
} ;

// 레벨 업 버튼을 눌렀을때
// 응답은 같은 메시지 번호로 MSG_LEVEL_UP_RESULT 패킷이 간다. 
// 실패하면 sInnerLevel 이 0으로 간다.
struct MSG_LEVEL_UP_REQ
{
	_sHeader			header;
	u_char				ucMessage;
};

struct MSG_LEVEL_UP_RESULT
{
	_sHeader			header;
	u_char				ucMessage;

	short				sInnerLevel;		// 내공 수위
	short				sLeftPoint;			// 결과
	short				sLeftMateryPoint;	// 남은 마스터리 포인트
	
	u_char				ucLevelupInfo ;		// 0 : 실패, 1 : 정상 레벨 업 2 : 초월모드 성공
											// 초월모드 성공시에는 경험치를 꽉 채운다! 별도의 서버 알림 없음
};

// 남아있는 기포인트를 5기에 사용한다. 
// 응답은 같은 번호로 MSG_POINT_USE_RESULT 가 간다.
struct MSG_POINT_USE_REQ
{
	_sHeader			header;
	u_char				ucMessage;

	char				cWhere;				// 0 : 생기, 1 : 내력, 2 : 영력, 3 : 심력, 4 : 정기
};

struct MSG_POINT_USE_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cWhere ;			// 0 : 생명(건강), 1 : 내공(진기), 2. 의지(지혜), 5로 가면 실패.
	short				sPoint ;			// 기 포인트
	short				sPower ;			// Power
	short				sLeftPoint ;		// 확인차 넣는다.
} ;

#define MSG_NO_OTHER_CHARAC_LEVEL_UP				109

struct MSG_OTHER_CHARAC_LEVEL_UP
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usUniqueID;
};