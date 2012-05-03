#pragma once

// Skill, Quick slot
// Main = 12
// 96 - 103

struct _sHeader;
struct _sCharac_Skill;

#ifdef _XDEF_ADD_MAX_LEARNSKILL_080522_KUKURI //Author : 양희왕 //breif : 국내는 스킬이 80개로 늘어남
    #define _COUNT_LEARN_SKILL_				80
#else
    #define _COUNT_LEARN_SKILL_				60
#endif

#define MSG_NO_SKILL_INFO				96		// 캐릭터의 무공 정보
#define MSG_NO_SKILL_SELECT				97		// 무공 선택 메세지
#define MSG_NO_SKILL_CTRL_REQ			98		// 무공을 배우겠다 C->S
#define MSG_NO_SKILL_CTRL				98		// 무공 조작

#define MSG_NO_SKILL_INFO_MASTERY		99		// 마스터리 스킬의 정보
#define MSG_NO_SKILL_INFO_COOLTIME		100		// 쿨타임 정보

#define MSG_NO_SKILL_ACTION				101     // 행동 스킬의 사용 : 상태효과 취소 등의 일을 한다.
#define MSG_NO_SKILL_INFO_REQ           102     // 스킬 인포 요청.

#define MSG_NO_MASTERY_POINT_USE		108		// 마스터리 포인트 사용 패킷
#define MSG_NO_MASTERY_POINT_RESULT		108		// 남은 포인트 수와 변경된 스킬들의 포인트 값


struct MSG_SKILL_INFO
{
	_sHeader	header ;
	u_char		ucMessage ;
	
	u_char		ucLearnSkillCount ;		// 배운 스킬의 개수
	
	_sCharac_Skill skill_info[_COUNT_LEARN_SKILL_] ;
} ;

struct MSG_SKILL_SELECT
{
	_sHeader			header;
	u_char				ucMessage;

	char				cSelectLevel;			// 선택 수위
	short				sSkillID;				// 선택 무공의 아이디
};

enum 
{
	en_skill_ctrl_req_learn_from_npc=0,		// 0	npc 로부터 스킬을 배운다.
	en_skill_ctrl_req_step_up,				// 1	스킬의 단계를 상승시킨다.
	en_skill_ctrl_req_passive,				// 2	패시브 스킬을 on/off 시킨다.
	en_skill_ctrl_req_reserve1,				// 3	
	en_skill_ctrl_req_reserve2,				// 4
	en_skill_ctrl_req_del_skill,			// 5	// 스킬의 삭제 요청
	en_skill_ctrl_req_complete_training		// 6	스킬의 훈련 상태를 훈련 완료 상태로 변경한다.
} ;

union __how_to_skill
{
	struct 
	{										// for Mode 0 : 배우기 요청.
		u_short		usNpcID ;
		u_char		ucIndexinList ;			// NPC 스크립트의 몆 번째 인덱스인가. Zero-Base.
		short		sSkillID ;				// index - skill id pair.
		u_char		ucClassIndex ;			// 클래스 인덱스. 완전형/치유형.. 등등 디파인된 번호
	} S_from_npc ;
	
	struct  
	{										// Mode 1 : 단계 상승 요청
		u_char		ucSkillSlotIndex;		// 스킬 슬롯에서 인덱스
	} S_step_up;
	
	struct 
	{										// Mode 5 : 지우기 요청.
		u_char		ucSkillSlotIndex ;		// 스킬 슬롯에서의 인덱스
	} S_delete_skill ;

	struct {
		u_char		ucLearnSkillIndex ;		// 스킬 정보에서의 인덱스.
		short		sSkillID ;				// 스킬의 아이디 ( 확인용 )
	} S_complete_learn ;
	
	struct {
		u_char		ucLearnSkillIndex ;		// 대상 패시브 스킬의 슬롯 번호.
		short		sSkillID ;				// 인증으로 사용하기 위해, 패시브 스킬의 아이디를 넣는다.
		
		char		cSelectStep ;			// 선택 단계( 나중에 성장이 된다면 필요할 듯 )
		char		cOnOff ;				// 0 : Off, 1 : On
	} S_Passive_OnOff ;
};

struct MSG_SKILL_CTRL_REQ
{
	_sHeader		header;
	u_char			ucMessage;

	char			cMode;					// 0 : NPC로부터 전수 1..

	__how_to_skill	howto;
};

// MSG_SKILL_CTRL : 스킬 추가 혹은 업데이트 패킷
enum 
{
	en_skill_ctrl_mode_add=0,				//	0	스킬 추가 하라.( Learn의 응답 또는 서버에서 추가. )
	en_skill_ctrl_mode_learn_fail,			//	1	배우기 실패.
	en_skill_ctrl_mode_step_up_fail,		//	2	스텝 업 실패 요인
	en_skill_ctrl_mode_delete,				//	3	지워라...
	en_skill_ctrl_mode_update_step,			//	4	스킬 정보 업데이트 : 서버에서 자동으로 혹은 요청에 의해.
	en_skill_ctrl_mode_update_exp,			//	5	경험치 
	en_skill_ctrl_mode_update_mission,		//	6	 
	en_skill_ctrl_mode_complete_learn,		//	7	 
	en_skill_ctrl_mode_update_training,		//	8	수련 완료 설정.
	en_skill_ctrl_mode_down_exp,			//	9   스킬 경험치 삭감 퀘스트에서 쓰임.
	en_skill_ctrl_mode_off_use,				//	10  스킬을 일시적으로 쓰지 못하게 한다.  
	en_skill_ctrl_mode_on_use,				//	11  스킬을 다시 사용하게 한다. 
	en_skill_ctrl_mode_passive,				//	12	패시브 스킬의 상태를 변경하라.	
	en_skill_ctrl_mode_cooltime,			//	13	쿨타임 업데이트

	en_skill_ctrl_mode_bookinfo,			//	14	무공서 정보 : 있다면, 보내고 없다면, 보내지 않는다.
	en_skill_ctrl_mode_bookend,				//	15	무공서의 시간이 끝났다.
	en_skill_ctrl_mode_reset_mastery,		//  16	오의를 모두 날린다.	

	en_skill_ctrl_mode_del_bonus_hyperrun,	//  17  보너스 경공을 지운다.(슬롯번호는 250번 사용) 
} ;

union __skill_ctrl
{
	struct									// Mode 0
	{			
		u_char		ucLearnSkillIndex ;		// 스킬 슬롯 번호.
		short		sAddSkillID ;			// 추가되는 무공 아이디.
		char		cStep ;					// 단계 셋팅.
		u_char		ucSkillState ;			// 0 : 사용 가능 1 : 수련 필요.
	} S_skill_add ;
	
	struct									// Mode Learn Fail, Step up fail
	{			
		u_char		ucFailReason ;			// 실패 이유.
	} S_learn_fail ;
	
	struct									//  삭제 요청
	{
		u_char		ucLearnSkillIndex ;		// 삭제할 스킬 슬롯의 인덱스.
	} S_skill_delete ;
	
	struct									// 단계 상승 결과. 혹은 서버에서의 셋팅. 
	{					
		u_char		ucLearnSkillIndex ;
		char		cStep ;
	} S_skill_update_step ;
	
	struct	
	{					
		u_char		ucLearnSkillIndex ;		//
		int			iCurSkillExp ;			// 현재 경험치
	} S_skill_update_exp ;
	
	struct
	{					
		u_char		ucLearnSkillIndex ;		//
		char		cCurMission ;			// 미션 진행 카운트	
	} S_skill_update_mission_count ;

	struct {
		u_char		ucLearnSkillIndex ;		// 스킬 정보에서의 인덱스.
		short		sSkillID ;				// 스킬의 아이디 ( 확인용 )
	} S_complete_learn ;
	
	struct {
		u_char		ucLearnSkillIndex ;
		u_char		ucSkillState ;			// 0 : 사용 가능 1 : 수련 필요.
	} S_update_training ;

	struct {
		u_char		ucLearnSkillIndex ;		// 대상 패시브 스킬의 슬롯 번호.
		short		sSkillID ;				// 인증으로 사용하기 위해, 패시브 스킬의 아이디를 넣는다.
		
		char		cSelectStep ;			// 선택 단계( 나중에 성장이 된다면 필요할 듯 )
		char		cOnOff ;				// 0 : Off, 1 : On
	} S_Passive_OnOff ;

	struct {
		short		skillID ;				// 해당 스킬 
		u_short		usTimeUpdate ;			// 지연 시간 업데이트
	} S_Cooltime_Update ;

	struct {
		short		sBookID ;				// 무공서 아이디 : 0이면 무시.
		u_int		uiLeftSecond ;			// 남은 시간 : 0이면 무시.
	} S_Bookinfo ;

	// en_skill_ctrl_mode_reset_mastery
	struct {
		short		sLeftMasteryPoint;		// 오의 포인트 업데이트
	} S_Mastery_Reset;
	

	// en_skill_ctrl_mode_del_bonus_hyperrun
	struct {
		u_char		slot;
	} S_Del_Bonus_Hyperrun;
} ;

//#define MSG_NO_SKILL_CTRL				99		// 무공 조작.
struct MSG_SKILL_CTRL
{
	_sHeader		header ;
	u_char			ucMessage ;
	
	char			cMode ;					// ctrl mode :
	
	__skill_ctrl	ctrl ;
} ;


// 실패 이유 :

enum
{
	_SKILL_CAN_LEARN_SUCCESS_ =	0,		// 배울수 있다.
	_SKILL_CAN_LEARN_CONTRIBUTION_,		// 기여도 부족.
	_SKILL_CAN_LEARN_LEVEL_, 			// 성취 단계 모자름.
	_SKILL_CAN_LEARN_PRE1_,				// 선수 무공 1 없음.
	_SKILL_CAN_LEARN_PRE2_,				// 선수 무공 2 없음. 
	_SKILL_CAN_LEARN_CLAN_,				// 문파 조건 안맞음.
	_SKILL_CAN_LEARN_SKILL_INDEX_,		// id에 해당하는 스킬이 없다.
	_SKILL_CAN_LEARN_SKILL_FULL_,		// 더이상 스킬 슬롯이 없다.
	_SKILL_CAN_LEARN_SKILL_ALREADY_,	// 이미 배운적이 있다. 
	_SKILL_CAN_LEARN_NO_MONEY_,			// 돈이 없다. 
	_SKILL_CAN_LEARN_STEP_FULL_,		// 단계가 12성에 도달했다.
	_SKILL_CAN_LEARN_MORE_SKILLEXP_,	// 스킬 경험치가 더 필요하다.
	_SKILL_CAN_LEARN_GRADE_,			// 직책 등급이 모자라다
} ;


// Mastery Skill ==============================================================

// 유저에게 보내는 마스터리 스킬의 포인트 정보.
struct _sChr_Mastery_Info
{
	short		skillID ;			// 마스터리 스킬의 아이디
	char		cPoint ;			// 찍은 포인트. 0일 수 없다. 0이면 가지 않음.
	char		cReserve ;			// 예약. 안쓰임.
} ;


//#define MSG_NO_SKILL_INFO_MASTERY		99		// 마스터리 스킬의 정보
#define _COUNT_MASTERY_SKILL_			96			// 마스터리 스킬의 개수.

struct MSG_SKILL_INFO_MASTERY
{
	_sHeader	header ;
	u_char		ucMessage ;
	
	u_char		ucMasteryCount ;		// 포인트 찍은 마스터리 스킬의 정보
	
	_sChr_Mastery_Info	mastery_info[_COUNT_MASTERY_SKILL_] ;	// DB 에는 _sDB_Mastery_Info 로. 
} ;


//#define MSG_NO_MASTERY_POINT_USE		108		// 마스터리 포인트 사용 패킷.
struct MSG_MASTERY_POINT_USE
{
	_sHeader			header ;
	u_char				ucMessage ;			// 1포인트씩 사용하는 패킷.
	
	u_char				ucPageNo : 3 ;		// 0,1,2의 페이지 번호.
	u_char				ucPadding1 : 5 ;	//
	
	short				sMasteryID ;		// 마스터리 스킬의 아이디.
} ;

//#define MSG_NO_MASTERY_POINT_RESULT		108		// 남은 포인트 수와 변경된 스킬들의 포인트 값.
struct MSG_MASTERY_POINT_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;			// 
	
	u_char				ucPageNo : 3 ;		// 
	u_char				ucFail : 5 ;		// 0 은 성공, 1은 찍을 수 없음.
	
	short				sMasteryID ;
	char				ucMasteryStep ;		// 최종 결과값.
	
	short				sLeftMasteryPoint ;	// 남은 마스터리 포인트.
} ;

//#define MSG_NO_SKILL_INFO_REQ                  102     // 스킬 인포 요청.
struct MSG_SKILL_INFO_REQ
{
	_sHeader	header ;
	u_char		ucMessage ;
};

#define _COUNT_COOL_TIME_		32

// 오의 정보 다음에 쿨타임 정보가 갑니다. ( 없어도, 개수르  0으로 설정해서 보냅니다. )
// 유저에게 보내는 쿨타임 리스트
struct _sChr_Cool_Info
{
	short		skillID ;
	u_short		usLeftCoolTime ;		// 남은 쿨타임. 초단위.
} ;


struct MSG_SKILL_INFO_COOLTIME
{
	_sHeader	header ;
	u_char		ucMessage ;
	
	u_char		ucCoolTimeListCount ;		// 쿨타임 정보의 개수. 0 이면, 파싱 끝.
	
	_sChr_Cool_Info		cooltime[_COUNT_COOL_TIME_] ;
} ;


// 취소 스킬의 사용 :

enum EN_CANCEL_SKILL_INDEX 
{
	en_cancel_skill_cancel_mask=0,       // 0 자기 복면 해제 요청 : MSG_ACTION_SKILL 
    en_cancel_skill_cancel_extra=1,      // 1 Extra 효과들의 취소 : slot = 0 ~ 9 : MSG_ACTION_SKILL_EXTRA_CANCEL
                                         // 우선 : 4번 슬롯만 허용하도록 하드코딩한다.
    en_cancel_skill_cannot=100,          // 100 : 취소할 수 없음. : MSG_CANCEL_SKILL_CANNOT
} ; 

//#define MSG_NO_SKILL_ACTION    101      // 행동 스킬의 사용 : 상태효과 취소 등의 일을 한다.

struct MSG_CANCEL_SKILL                   // 행동 스킬 관련 패킷
{
	_sHeader	header ;
	u_char		ucMessage ;
	u_char		ucMode ;                  // 위의 EN_ACTION_SKILL_INDEX
} ; 

struct MSG_CANCEL_SKILL_EXTRA : public MSG_CANCEL_SKILL
{
	char		cSlotNo ;                 // 0 - 9 : 현재는 4번만 허용.
} ;

enum EN_CANCEL_SKILL_CANNOT_REASON 
{
	en_cannot_act_no_mask_effect=0,       // 복면 효과 없음.
	en_cannot_act_no_extra_effect,        // 해당 엑스트라 효과 없음.
} ; 

struct MSG_CANCEL_SKILL_CANNOT : public MSG_CANCEL_SKILL
{
	u_char		ucReason ;                      
} ;


//////////////////////////////////////////////////////////////////////////
// Quick slot and helper record

#define MSG_NO_QUICKSLOT                200     // 퀵슬롯 관련 메시지.

/*////////////////////////////////////////////////////////////////////////

	_XQS_SLOTTYPE_NONE = 0,
	_XQS_SLOTTYPE_SKILL,
	_XQS_SLOTTYPE_ITEM,
	_XQS_SLOTTYPE_LIFESKILL				  
////////////////////////////////////////////////////////////////////////*/

struct __client_quickslot

{
	
	u_char      slotType;                        // _XQS_	
	short       skillID;	
	short       lifeSkillID;	
	char        itemCType;	
	short       itemSID;	
	u_short     invenNumber;     	
	u_char      reserved;                        // must be zero	
	u_char      reserved2;                       // must be zero	
};           // 12 bytes

enum EN_QUICKSLOT_MODE
{
	en_quickslotmode_allinfo=0,
	en_quickslotmode_update,
	en_quickslotmode_reset,
	
	en_helplog_allinfo=10,                   // 크기가 작으므로, 한꺼번에 모두.
    en_helplog_update,
    en_helplog_reset,
};

struct MSG_QUICKSLOT
{	
	_sHeader    header;	
	u_char      ucMessage;						// MSG_NO_QUICKSLOT	
	u_char      ucQuickSlotMode;				//	
};


//allinfo - 처음에 한번. 캐릭터 정보 보낸 후? 아이템 정보 보낸 후? S->C
//update  - 수시로. C->S
//reset   - 수시로. C->S

#define _QUICKSLOT_COUNT_  30

struct MSG_QUICKSLOT_ALLINFO : public MSG_QUICKSLOT
{	
	__client_quickslot      m_QuickSlot[_QUICKSLOT_COUNT_];	
};

struct MSG_QUICKSLOT_UPDATE_UNIT : public MSG_QUICKSLOT
{	
	u_char					slotNumber;
	__client_quickslot      slotCont;
};

struct MSG_QUICKSLOT_RESET_UNIT : public MSG_QUICKSLOT
{	
	u_char					slotNumber;                     // 리셋 슬롯 인덱스.
};







//////////////////////////////////////////////////////////////////////////
// Help log

#define _HELPLOG_MAX_SIZE_                  (4*4)

struct MSG_HELPLOG_ALLINFO : public MSG_QUICKSLOT
{
       u_char				ucHelpLog[_HELPLOG_MAX_SIZE_];
       
	   MSG_HELPLOG_ALLINFO()
			: MSG_QUICKSLOT( /*sizeof(MSG_HELPLOG_ALLINFO), en_helplog_allinfo*/ )
       {
       }

};

struct MSG_HELPLOG_UPDATE : public MSG_QUICKSLOT
{
       u_char                ucHelpLog[_HELPLOG_MAX_SIZE_];

       MSG_HELPLOG_UPDATE()
			: MSG_QUICKSLOT( /*sizeof(MSG_HELPLOG_UPDATE), en_helplog_update*/ )
       {
       }

};
 
struct MSG_HELPLOG_RESET : public MSG_QUICKSLOT
{
       MSG_HELPLOG_RESET()
            : MSG_QUICKSLOT( /*sizeof(MSG_HELPLOG_RESET), en_helplog_reset*/ )
       {
       }
};
