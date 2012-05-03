#pragma once

#pragma pack(push,PREV_PACK)
#pragma pack(1)

// 이곳에는 패킷에도 포함되는 구조체를 정의한다.


enum
{
	max_count_of_challenge_list=20,

	max_win_count_of_castle=255,
};



/*
__cb_challenge_info  이 구조체는 GS 와의 패킷에 쓰인다. 
따라서, 멤버가 바뀌면 반드시 GS 와 일치시킬 것.
*/



struct __cb_challenge_info
{
	int						iDBSeqNumber;		// DB 에서의 시퀀스 인덱스 : 검색도 이것으로 한다.

	int						iIndex;				// Ally or Org Index

	u_short					usMemberCount;		// 인원수.

	u_char					ucMoonpa : 5;		// not use
	u_char					ucRes1 : 1;			// 
	u_char					ucState : 1;		// 0 : normal, 1 : fighting state.
	u_char					ucType : 1;			// Organization(0) or Ally(1)

	u_short					usLoudCount;		// 초기값 0 : 최대 20

	u_short					usReserve;			// 

	char					cGroupName[en_or_name_length+1];
	char					cMasterName[en_charac_name_length+1];

	__cb_challenge_info()
	{
		reset();
	}
	inline void reset()	
	{
		ZeroMemory( this, sizeof(__cb_challenge_info) );
	}

	/*
	void convert_to_client( __cb_client_challenge_info& ref )
	{
	ref.usMemberCount = usMemberCount;
	ref.ucReserve = 0;
	ref.ucType = (ucType==0) ? 0 : 1;
	ref.ucState = (ucState==0) ? 0 : 1;
	CopyMemory( ref.cGroupName, cGroupName, en_or_name_length );
	ref.cGroupName[en_or_name_length] = 0;
	CopyMemory( ref.cMasterName, cMasterName, en_charac_name_length );
	ref.cMasterName[en_charac_name_length] = 0;
	}
	*/
};

/*
_sCastleInfo : 장원의 소유 정보
GS 와의 패킷에 사용되므로, 멤버가 바뀌면 반드시 GS 와 일치시킬 것.
*/

#ifdef _PD_CASTLE_RENEWAL_1_

struct _sCastleInfo
{
	int			iOrIndex;			// default 0
	u_char		ucType;				// 0 - Organization. 1 - Ally, 2 - Monster
	u_short		usMemberCount;		// 영웅단 또는 방회의 인원수.

	u_char		ucLevel;			// 0 - 4 장원 레벨

	u_char		ucCurMatchCount_not;	// 0,1,2 차 방어전.	 : ucCurMatchCount => renewal 에서 안씀.
	u_char		ucCombatState;		// 0 : 시작 전, 1 : 전쟁중,

	char		cOrName[en_or_name_length+1];
	char		cMasterName[en_charac_name_length+1];
	char		cMasterMoonpa;				//

	u_int		uiTotalMoney;
	u_int		uiPartMoney;

	u_short		usWinCount;			// 성공 회수.


	// 장원 소유 날자. 년/월/일/시/분/초. End Time
	_sND_TimeUnit	timeOfGettCastle;			// 소유한 날짜.
	_sND_TimeUnit	timeNextSchedule;		// 다음 대전 날짜
	_sND_TimeUnit	timeunit_not_use[2];		// 리뉴얼에서 구조체의 크기에 변경이 없도록 사용함.

	inline int get_owner_index()		{ return iOrIndex; }
	inline u_char get_owner_type()		{ return ucType; }
	inline char * get_owner_name()		{ return cOrName; }
	inline u_char get_level()			{ return ucLevel; }
	inline char * get_master_name()		{ return cMasterName; }
	inline u_int get_total_money()		{ return uiTotalMoney; }
	inline u_int get_part_money()		{ return uiPartMoney; }
	inline u_short get_win_count()		{ return usWinCount; }

	inline _sND_TimeUnit * get_own_date()	{ return &timeOfGettCastle; }
	inline _sND_TimeUnit * get_schedule( int slot )	{ return &timeNextSchedule; }
};

#else 

struct _sCastleInfo
{
	int			iOrIndex;			// default 0
	u_char		ucType;				// 0 - Organization. 1 - Ally, 2 - Monster
	u_short		usMemberCount;		// 영웅단 또는 방회의 인원수.

	u_char		ucLevel;			// 0 - 4 장원 레벨

	/* 아래 변수 ucCurMatchCount, ucMatchState 의 설명.
	1차전 예정이면 : Count = 0, State = 0,
	2차전 중이라면 : Count = 1, satet = 1,
	3차전 예정이면 : Count = 2, state = 0,	*/
	u_char		ucCurMatchCount;	// 0,1,2 차 방어전.
	u_char		ucMatchState;		// 0 : 시작 전, 1 : 전쟁중,

	char		cOrName[en_or_name_length+1];
	char		cMasterName[en_charac_name_length+1];
	char		cMasterMoonpa;				//

	u_int		uiTotalMoney;
	u_int		uiPartMoney;

	u_short		usWinCount;			// 성공 회수.


	// 장원 소유 날자. 년/월/일/시/분/초. End Time
	_sND_TimeUnit	timeUnit;

	_sND_TimeUnit	Schedule[3];		// 3차전까지의 스케줄.

	inline int get_owner_index()		{ return iOrIndex; }
	inline u_char get_owner_type()		{ return ucType; }
	inline char * get_owner_name()		{ return cOrName; }
	inline u_char get_level()			{ return ucLevel; }
	inline u_char get_cur_match_count()	{ return ucCurMatchCount; }
	inline char * get_master_name()		{ return cMasterName; }
	inline u_int get_total_money()		{ return uiTotalMoney; }
	inline u_int get_part_money()		{ return uiPartMoney; }
	inline u_short get_win_count()		{ return usWinCount; }

	inline _sND_TimeUnit * get_own_date()	{ return &timeUnit; }
	inline _sND_TimeUnit * get_schedule( int slot )	{ return &Schedule[slot]; }
};

#endif

#pragma pack(pop,PREV_PACK)