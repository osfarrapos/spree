#pragma once 
/*
	시스템 메시지들..
	//  DB Error Code : ODBC 또는 DB 함수들의 에러코드
	#define LOGIN_ERROR_NO_ID				1				// 등록되지 않은 유저
	#define LOGIN_ERROR_ALREADY_LOGIN		2				// 다른 곳에서 로그인 했습니다
	#define LOGIN_ERROR_SUCCESS_MASTER		-1				// 운영자다 : 성공 
	#define LOGIN_ERROR_SUCCESS				0				// 일반 유저다 : 성공 
	#define LOGIN_ERROR_INCORRECT_PASSWORD	3				// 암호가 틀렸습니다. 
	#define LOGIN_ERROR_SQL_ERROR			5				// 알 수 없는  에러 입니다. 
*/


// Forest State
#define  _FOREST_STATE_RUNNING				0					// 정상 작동중
#define  _FOREST_STATE_OFF					-1					// 서비스 중지
#define  _FOREST_STATE_CHECKUP_				-2					// 서버군 점검중
#define  _FOREST_STATE_FULL_				-3					// 포화상태

// Login DB return value
#define  _LOGIN_DB_RETURN__NOT_FOUND_			1	// 없는 계정
#define  _LOGIN_DB_RETURN__NOT_MATCH_PASSWORD	2	// 암호틀림
#define  _LOGIN_DB_RETURN__AREADY_LOGINER		3	// 이미 로그인중
#define	 _LOGIN_DB_RETURN__NOT_UNI_NEXON		4	// 넥슨 아이디가 없다. 통합하라. 
#define  _LOGIN_DB_RETURN__BLOCKING_USER		6	// 블럭킹 당한 유저
#define  _LOGIN_DB_RETURN__BLOCKING_CASE		100	// 이것보다 크면 블럭상태이다.
#define  _LOGIN_DB_RETURN__ERROR_				9

// 클래스 번호..
#define _CLASS_BLOCK_NOTHING_				0		// 인증되지 않은 계정
#define _CLASS_BLOCK_TAKING_				1		// 압류계정
#define _CLASS_BLOCK_OFF_					2		// 정지된 계정
#define _CLASS_BLOCK_STOP_					3		// 일시 정지된 계정
#define _CLASS_BLOCK_TEEN_					4		// 제한연령 미달자..
#define _CLASS_BLOCK_TESTER_ONLY			5		// 테스트 서버만 접속 가능
#define _CLASS_BLOCK_GENERAL				12		// 기본적인 사용자
#define _CLASS_BLOCK_SUPPORTER				18		// 협력사 제공계정
#define _CLASS_BLOCK_INSIDER_SEC			19		// 내부 인원 하위
#define _CLASS_BLOCK_INSIDER_FIR			20		// 내부 인원 상위