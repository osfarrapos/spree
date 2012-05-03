#pragma once

struct _sHeader;

#define MSG_NO_MASTER_AND_PUPIL                                (166)                   // MSG_MP_

// 스승 레벨 최대값. 0부터 4까지. 총 다섯 단계다.
#define NDD_MP_MAX_MASTER_LEVEL			(4)

// 최대 제자수
#define NDD_MP_MAX_PUPIL_COUNT			(4)

// 기본, 확장 혜택의 킬링 카운트
// 스크립트에서 정의되지 않으면, 아래 숫자를 쓴다. 
#define NDD_MP_BASE_KILLCOUNT			(100U)
#define NDD_MP_EXTEND_KILLCOUNT			(1000U)

// 사호의 최대 길이.
#define NDD_MP_MASTERNICK_LENGTH		(36)	// NULL 미포함.
#define NDD_MP_MAX_POINT				(1000000000U)	// 10억 이다. 절대로 0x8fffffff 를 넘지 않도록 할 것.
 

struct MSG_MASTER_AND_PUPIL  
{
	_sHeader	header;
	u_char	ucMessage;
	u_char	ucMP_Mode;                   // EN_CG_MP_PACKET_MODE
};

enum EN_CG_MP_PACKET_MODE	// CG = Client <=> Game Server. MP = Master&Pupil
{
	// 0 - 30 : 정보 및 관련 패킷.
	en_cg_mp_mode_info_my_master=0,				// 사제 관계 정보. 필드 진입시 전달.
	en_cg_mp_mode_info_req=1,					// 사제 관계 정보 요청. 주변인이 보낸다.
	en_cg_mp_mode_info_res=2,					// req 의 응답으로, 간략 정보가 감. 

	// 정보 업데이트
	en_cg_mp_mode_update_point=3,				// 스승 및 제자의 포인트 수정.
	en_cg_mp_mode_update_kill_count=4,			// 킬 카운트 업데이트
	en_cg_mp_mode_update_master_level=5,		// 마스터 레벨 증가 패킷.
	en_cg_mp_mode_udpate_position=6,			// 제1,2,3,4, 제자의 지위 바꾸기.
	en_cg_mp_mode_master_info_created=7,		// 사제 정보가 생성되었다.( 퀘스트 : rank 1 설정시. )
	 en_cg_mp_mode_update_con_state=8,			// 사제의 접속 정보 업데이트

	en_cg_mp_mode_bonus_setting=10,				// 스승의 혜택 On/Off
	en_cg_mp_mode_no_mp_info=11,				// 사제 관계가 없음.

	en_cg_mp_mode_change_master_nick_req=20,	// 변경 요청.
	en_cg_mp_mode_change_master_nick_res=21,	// 사호 변경을 알려준다.
	en_cg_mp_mode_update_master_nick=22,		// 제자 또는 주변에 알린다.
	

	en_cg_mp_mode_info_error=50,				// Info : error message.


	// 51 - 80 : 사제 관계 맺기 관련 패킷.
	en_cg_mp_mode_cg_apply_for_pupil=51,		// 제자 신청을 GS 로 보냄 : 제자가 되길 원하는 캐릭터가 보냄.
	en_cg_mp_mode_gc_apply_for_pupil=52,		// 제자 신청을 받음 : 스승이 받음.
	en_cg_mp_mdoe_answer_apply_from_master=53,	// 스승 : 제자 신청에 대한 응답.
	en_cg_mp_mode_answer_apply_no_to_pupil=54,	// 제자가 될 캐릭터에게 응답을 보낸다. ( yes 를 선택했을 경우, 응답대신 complete_join_pupil 이 간다. )
	en_cg_mp_mode_complete_join_pupil=55,		// 제자가 되려는 캐릭터에에게, 제자 추가가 완료되었음을 알림. 
	en_cg_mp_mode_add_new_pupil=56,				// 새로운 제자가 추가되었음을 스승 및 다른 제자에게 알림.
	en_cg_mp_mode_new_master_simple_info=57,	// 신청의 1차 응답으로, GS 에서 바로 보내는 
	
	en_cg_mp_mode_apply_error=80,				// 신청 과정의 모든 에러.

	// 81 - 110 : 파문 및 하산.
	en_cg_mp_mode_strike_pupil_req=81,			// 제자 파문 요청
	en_cg_mp_mode_strike_result=82,				// 스승 및 다른 제자에게 감.
	en_cg_mp_mode_you_struck=83,				// 파문 제자에게 파문을 알림.

	en_cg_mp_mode_strike_error=90,				// 파문 에러.

	en_cg_mp_mode_leave_req=91,					// 하산 요청.
	en_cg_mp_mode_you_left=92,					// 하산에 성공.
	en_cg_mp_mode_left_pupil=93,				// 제자가 하산했다. 스승 및 다른 제자에게 전달.

	en_cg_mp_mode_left_error=100,				// 하산 에러.

	en_cg_mp_mode_ban_pupil_req=101,			// 강제추방 요청.
	en_cg_mp_mode_pupil_banned=102,				// 강제추방되었다. 사제 정보를 포함하며, 다른 제자에게도 보냄.
	en_cg_mp_mode_ban_error=110,				// 강제추방 에러.

};

//////////////////////////////////////////////////////////////////////////

struct _sPacketPupil			// 패킷에 쓰이느 제자 스트럭쳐.
{
	u_int				uiTotalPoint;
	u_int				uiCurPoint;
	u_char				ucSex;
	u_char				ucLevel;		//  
	u_char				ucMoonpa;		// 
	u_char				ucClass;
	u_char				ucClassGrade;
	u_char				ucPosition;		// 0 - 제1제자, 1.2.3 - 2,3,4 제자.
	u_char				ucCurZone;		// 아래 ConnectState 가 1 일때만 의미가 있음.
	u_char				ucConnectState;	// 0 - 로그아웃, 1 - 접속중.
	char				cName[en_charac_name_length+1];

};


//////////////////////////////////////////////////////////////////////////

// 0 - 30 : 정보 및 관련 패킷.
//en_cg_mp_mode_info_my_master=0,				// 사제 관계 정보. 필드 진입시 전달.
struct MSG_MP_INFO_MY_MASTER : public MSG_MASTER_AND_PUPIL
{
	int					iMasterIndex;

	u_char				ucMasterRank;

	u_int				uiTotalKillCount;
	u_int				uiMasterTotalPoint;
	u_int				uiMasterCurPoint;

	u_char				ucMasterCurZone;			// 아래 ConnectState 가 1 일 때만, 의마가 있음.
	u_char				ucMasterConnectState;		// 0 - 로그아웃, 1 - 접속중.
	u_char				ucBonusSkillStep;

	u_char				ucSex;
	u_char				ucLevel;	
	u_char				ucMoonpa;
	u_char				ucClass;
	u_char				ucClassGrade;

	char				cMasterName[en_charac_name_length+1];
	char				cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];

	// 위로는 스승의 정보, 아래로는 제자들의 정보.
	_sPacketPupil		Pupils[NDD_MP_MAX_PUPIL_COUNT];
};

//en_cg_mp_mode_info_req=1,					// 사제 관계 정보 요청. 주변인이 보낸다.
struct MSG_MP_INFO_REQ : public MSG_MASTER_AND_PUPIL
{
	int					iMasterIndex;		// 마스터 인덱스. 
};

//en_cg_mp_mode_info_res=2,					// req 의 응답으로, 간략 정보가 감. 
struct MSG_MP_INFO_RES : public MSG_MASTER_AND_PUPIL 
{
	int					iMasterIndex;
	
	u_char				ucMasterLevel;

	u_char				ucMasterConnectState;		// 0 - 로그아웃, 1 - 접속중.
	u_char				ucBonusSkillStep;

	u_char				ucSex;
	u_char				ucLevel;	
	u_char				ucMoonpa;
	u_char				ucClass;
	u_char				ucClassGrade;

	u_char				ucCountOfPupils;			// 제자수.

	char				cMasterName[en_charac_name_length+1];

	char				cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];
};



// 정보 업데이트
//en_cg_mp_mode_update_point=3,				// 스승 및 제자의 포인트 수정.
struct MSG_MP_UPDATE_POINT : public MSG_MASTER_AND_PUPIL
{
	u_int				uiMasterTotalPoint;
	u_int				uiMasterCurPoint;

	u_char				where;			// 0 -

	u_int				uiTotalPoint;
	u_char				uiCurPoint;
};

// en_cg_mp_mode_update_kill_count=4,			// 킬 카운트 업데이트
struct MSG_MP_UPDATE_KILL_COUNT : public MSG_MASTER_AND_PUPIL
{
	u_int				uiMyKillCount;			// 
	u_int				uiTotalKillCount;		// 
};

//en_cg_mp_mode_update_master_level=5,		// 마스터 레벨 증가 패킷.
struct MSG_MP_UPDATE_MASTER_LEVEL : public MSG_MASTER_AND_PUPIL
{
	u_char				ucMasterLevel;		// 스승 레벨 업데이트.
};

//en_cg_mp_mode_udpate_position=6;			// 제1,2,3,4, 제자의 지위 바꾸기.
struct MSG_MP_UPDATE_POSITION : public MSG_MASTER_AND_PUPIL
{
	// name1 이 position1 이 되고, name2 가 position2 가 되었슴.
	char				cName1[en_charac_name_length+1];
	u_char				ucPosition1;

	char				cName2[en_charac_name_length+1];
	u_char				ucPosition2;
};

// en_cg_mp_mode_master_info_created=7,		// 사제 정보가 생성되었다.( 퀘스트 : rank 1 설정시. )
struct MSG_MP_MASTER_INFO_CREATED : public MSG_MASTER_AND_PUPIL
{
	int					iMasterIndex;
	
	u_char				ucMasterLevel;

	u_char				ucSex;
	u_char				ucLevel;	
	u_char				ucMoonpa;
	u_char				ucClass;
	u_char				ucClassGrade;
};

// en_cg_mp_mode_update_con_state=8,			// 사제의 접속 정보 업데이트
struct MSG_MP_UPDATE_CON_STATE : public MSG_MASTER_AND_PUPIL
{
	u_char				ucConState;		// 0 - not, 1 - field, 2 - server move.
	u_char				ucZone;
	char				cName[en_charac_name_length+1];
};


//en_cg_mp_mode_bonus_setting=10,				// 스승의 혜택 On/Off
struct MSG_MP_BONUS_SETTING : public MSG_MASTER_AND_PUPIL
{
	u_char				ucBonusSkillStep;			// 무공 성 보너스 

	u_char				ucApplyMasterEffect;		// 0 - Off, 1 - On.
};


//en_cg_mp_mode_no_mp_info=11,				// 사제 관계가 없음.
struct MSG_MP_NO_MP_INFO : public MSG_MASTER_AND_PUPIL
{
	
};


// en_cg_mp_mode_change_master_nick_req=20,	// 변경 요청.
struct MSG_MP_CHANGE_MASTER_NICK_REQ : public MSG_MASTER_AND_PUPIL
{
	char				cNick[NDD_MP_MASTERNICK_LENGTH+1];
};


// en_cg_mp_mode_change_master_nick_res=21,	// 사호 변경을 알려준다.
// 요청한 개인(스승)에게만 가고, 제자들에게는 MSG_MP_UPDATE_MASTER_NICK 이 간다. 
struct MSG_MP_CHANGE_MASTER_NICK_RES : public MSG_MASTER_AND_PUPIL
{
	u_char				ucSuccessFail;		// 0 - Fail, 1 - Success
	
	char				cNick[NDD_MP_MASTERNICK_LENGTH+1];
};


// en_cg_mp_mode_update_master_nick=22,		// 제자 또는 주변에 알린다.
struct MSG_MP_UPDATE_MASTER_NICK : public MSG_MASTER_AND_PUPIL
{
	int					iMasterIndex;
	char				cNick[NDD_MP_MASTERNICK_LENGTH+1];
};


//en_cg_mp_mode_info_error=50,				// Info : error message.
struct MSG_MP_INFO_ERROR : public MSG_MASTER_AND_PUPIL
{
	u_char				ucErrorCode;			// ...
};



//////////////////////////////////////////////////////////////////////////
// 51 - 80 : 사제 관계 맺기 관련 패킷.
//en_cg_mp_mode_cg_apply_for_pupil=51,		// 제자 신청을 GS 로 보냄 : 제자가 되길 원하는 캐릭터가 보냄.
struct MSG_MP_CG_APPLY_FOR_PUPIL : public MSG_MASTER_AND_PUPIL
{
	// 인덱스는 알 수 없으므로, 이름으로 요청한다.
	char				cMasterName[en_charac_name_length+1];
};

// en_cg_mp_mode_gc_apply_for_pupil=52,		// 제자 신청을 받음 : 스승이 받음.
struct MSG_MP_GC_APPLY_FOR_PUPIL : public MSG_MASTER_AND_PUPIL
{
	u_char				ucLevel;
	u_char				ucMoonpa;
	u_char				ucClass;
	u_char				ucClassGrade;

	char				cMasterName[en_charac_name_length+1];
	char				cFromPupil[en_charac_name_length+1];
};

// en_cg_mp_mdoe_answer_apply_from_master=53,	// 스승 : 제자 신청에 대한 응답.
struct MSG_MP_ANSWER_APPLY_FROM_MASTER : public MSG_MASTER_AND_PUPIL
{
	u_char				ucNoYes;			// 0 - No, 1 - Yes.
	char				cNewPupil[en_charac_name_length+1];
};

//en_cg_mp_mode_answer_apply_no_to_pupil=54,	// 제자가 될 캐릭터에게 응답을 보낸다. ( yes 를 선택했을 경우, 응답대신 complete_join_pupil 이 간다. )
struct MSG_MP_ANSWER_APPLY_NO_TO_PUPIL : public MSG_MASTER_AND_PUPIL
{
	char				cFromMaster[en_charac_name_length+1];
};

// en_cg_mp_mode_complete_join_pupil=55,		// 제자가 되려는 캐릭터에에게, 제자 추가가 완료되었음을 알림. 
struct MSG_MP_COMPLETE_JOIN_PUPIL : public MSG_MASTER_AND_PUPIL
{
};

//en_cg_mp_mode_add_new_pupil=56,				// 새로운 제자가 추가되었음을 스승 및 다른 제자에게 알림.
struct MSG_MP_ADD_NEW_PUPIL : public MSG_MASTER_AND_PUPIL
{
	_sPacketPupil		NewPupil;
};

//en_cg_mp_mode_new_master_simple_info=57 // 신청의 1차 응답으로, GS 에서 바로 보내는 
struct MSG_MP_NEW_MASTER_SIMPLE_INFO : public MSG_MASTER_AND_PUPIL
{
	char					cMasterName[en_charac_name_length+1];

	u_char					ucMasterLevel;
	u_char					ucMasterMoonpa;
	u_char					ucMasterClass;
	u_char					ucMasterClassGrade;
	u_char					ucCurZone;

	char					cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];
};

// en_cg_mp_mode_apply_error=80,				// 신청 과정의 모든 에러.
enum EN_MP_APPLY_ERROR		// MSG_MP_APPLY_ERROR
{
	en_mp_apply_no_error=0,						// 0 값은 그냥 쓴다. 이 값으로는 패킷이 가지 않는다.
	en_mp_apply_error_wait=1,					// 10 초 제한 : 잠시후 다시 시도.
	en_mp_apply_error_pupil_level_limit=2,		// 73 레벨 이상 캐릭터는 제자가 될 수 없습니다.
	en_mp_apply_error_level_diff_limit=3,		// 스승과 제자는 13레벨 이상 차이가 나야합니다.
	en_mp_apply_error_already_has_master=4,		// 이미 다른 스승이 있습니다.
	en_mp_apply_error_pupils_over=5,			// 더 이상 제자를 받을 수 없습니다.
	en_mp_apply_error_you_are_master=6,			// 현재 스승이다.
};

struct MSG_MP_APPLY_ERROR : public MSG_MASTER_AND_PUPIL
{
	// 
	u_char				ucErrorCode;
};


//////////////////////////////////////////////////////////////////////////
// 81 - 100 : 파문 및 하산.
//en_cg_mp_mode_strike_pupil_req=81,			// 제자 파문 요청
struct MSG_MP_STRIKE_PUPIL_REQ : public MSG_MASTER_AND_PUPIL
{
	// 파문하려는 제자의 이름을 넣는다.
	char				cPupilName[en_charac_name_length+1];
};

//en_cg_mp_mode_strike_result=82,				// 스승 및 다른 제자에게 감.
struct MSG_MP_STRIKE_RESULT : public MSG_MASTER_AND_PUPIL
{
	char				cStrikeCName[en_charac_name_length+1];

	u_int				uiMasterTotalPoint;
	u_int				uiMasterCurPoint;

	u_char				ucBonusSkillStep;

	u_char				ucChangedCount;		// ChangedPosition 은 ucChangedCount 개수만 유혀. 나머지는 안채움.
											// 파문으로 제1,2제자의 변화가 없을 경우엔, 0 이다.

	struct
	{
		char			cPupilName[en_charac_name_length+1];
		u_char			ucNewPosition;		// 2 - 제1제자.
	} ChangedPosition[NDD_MP_MAX_PUPIL_COUNT-1] ;
};

// en_cg_mp_mode_you_struck=83,				// 파문 제자에게 파문을 알림.
struct MSG_MP_YOU_STRUCK : public MSG_MASTER_AND_PUPIL
{
	// 	
};

// en_cg_mp_mode_strike_error=90,				// 파문 에러.
struct MSG_MP_STRIKE_ERROR : public MSG_MASTER_AND_PUPIL
{
	u_char				ucErrorCode;

	enum
	{
		en_mp_strike_error_no=0,			// 의미 없음.
		en_mp_strike_error_not_master=1,	// 파문 자격이 없음( 스승이 아님 )
		en_mp_strike_error_pupil_not_found=2,	// 파문은 제자가 연결되어 있어야 한다.
		en_mp_strike_error_system_error=255,	// 알 수 없는 에러다.
	};
};

//en_cg_mp_mode_leave_req=91,					// 하산 요청.
struct MSG_MP_LEAVE_REQ : public MSG_MASTER_AND_PUPIL
{
	//
};

//en_cg_mp_mode_you_left=92,					// 하산에 성공.
struct MSG_MP_YOU_LEFT : public MSG_MASTER_AND_PUPIL
{
	
};

//en_cg_mp_mode_left_pupil=93,				// 제자가 하산했다. 스승 및 다른 제자에게 전달.
struct MSG_MP_LEFT_PUPIL : public MSG_MASTER_AND_PUPIL
{
	char				cLeftCName[en_charac_name_length+1];
	u_int				uiMasterTotalPoint;
	u_int				uiMasterCurPoint;

	u_char				ucBonusSkillStep;

	u_char				ucChangedCount;		// ChangedPosition 은 ucChangedCount 개수만 유효. 나머지는 안채움.
											// 파문으로 제1,2제자의 변화가 없을 경우엔, 0 이다.

	struct
	{
		char			cPupilName[en_charac_name_length+1];
		u_char			ucNewPosition;		// 2 - 제1제자.
	}ChangedPosition[NDD_MP_MAX_PUPIL_COUNT-1] ;
};


//en_cg_mp_mode_left_error=100,				// 하산 에러.
struct MSG_MP_LEFT_ERROR : public MSG_MASTER_AND_PUPIL
{
	u_char				ucErrorCode;
	
	enum
	{
		en_mp_leave_error_no=0,				// 의미 없음.
		en_mp_leave_error_not_pupil=1,		// 하산할 수 없음. 제자가 아님.
		en_mp_leave_error_system_error=255,	// 알 수 없는 에러다.
	};
};


//	en_cg_mp_mode_ban_pupil_req=101,			// 강제추방 요청.
struct MSG_MP_BAN_PUPIL_REQ : public MSG_MASTER_AND_PUPIL
{
	char				cPupilName[en_charac_name_length+1];
};


//	en_cg_mp_mode_pupil_banned=102,				// 강제추방되었다. 사제 정보를 포함하며, 다른 제자에게도 보냄.
struct MSG_MP_PUPIL_BANNED : public MSG_MASTER_AND_PUPIL
{
	// 자신이면, 사제 정보를 초기화할 것.
	char				cPupilName[en_charac_name_length+1];
};

// en_cg_mp_mode_ban_error=110,				// 강제추방 에러.
struct MSG_MP_BAN_ERROR : public MSG_MASTER_AND_PUPIL
{
	u_char				ucErrorCode;	// 
};
