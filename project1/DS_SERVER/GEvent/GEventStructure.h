#pragma once

#ifdef _PD_GAME_EVENT_

//////////////////////////////////////////////////////////////////////////
// Game Event 보상 정보
//////////////////////////////////////////////////////////////////////////

#define FREEMATCH_SERVER_INDEX			(17)

// 보상 타입.
enum
{
	en_ge_reward_nick=0,			// 별호 보상
	en_ge_reward_item,				// 아이템 보상 : 남녀 공통일 때.
	en_ge_reward_item_to_male,		// 남자에게만 보상
	en_ge_reward_item_to_female,	//
};

// 보상 정보.
struct __ge_reward
{
	u_short		reward_index;
	u_short		reward_type;		// en_fm_reward...

	union
	{
		struct 
		{
			char		cType;
			u_char		ucCount;
			short		sID;
		} item;

		struct 
		{
			u_short		usNickID;
			u_short		usValue1;

			u_short		usInfo1;
			u_short		usInfo2;
		} nick;
	};

	__ge_reward *	pNext;

	__ge_reward()
	{
		ZeroMemory( this, sizeof(__ge_reward) );
	}

	void release()
	{
		__ge_reward * pCur = pNext;
		__ge_reward * pDel = pNext;
		while( pCur )
		{
			pDel = pCur;
			pCur = pCur->pNext;
			delete pDel;			
		}
	}
	void add( __ge_reward * p )		// 외부에서, 생성된 객체.
	{
		__ge_reward * pos = this;
		while( pos->pNext )
		{
			pos = pos->pNext;
		}
		pos->pNext = p;
		p->pNext = NULL;
	}

	void Print()
	{
		printf( "  Reward %2u\n", reward_index );

		__ge_reward * p = this;

		while( p )
		{
			switch( p->reward_type )
			{
			case en_ge_reward_nick:			// 별호 보상
				printf( "    Nick : %d\n", p->nick.usNickID );
				break;
			case en_ge_reward_item:			// 아이템 보상 : 남녀 공통일 때.
				printf( "    Item : %d - %d\n", p->item.cType, p->item.sID );
				break;
			case en_ge_reward_item_to_male:		// 남자에게만 보상
				printf( "    Item To Male : %d - %d\n", p->item.cType, p->item.sID );
				break;
			case en_ge_reward_item_to_female:	//
				printf( "    Item To Fema : %d - %d\n", p->item.cType, p->item.sID );
				break;
			}

			p = p->pNext;
		}

	}
};



//////////////////////////////////////////////////////////////////////////
// 이벤트 타입과 이벤트의 상태
//////////////////////////////////////////////////////////////////////////
enum GEVENT_TYPE
{
	GEVENT_TYPE_SYSTEM=0,				// 모든 예약된 타이머를 삭제한다.
	GEVENT_TYPE_FREE_MATCH,				// 2007 09 12 현재는 이거 하나밖에 없다.
	GEVENT_TYPE_COUNT,					// 이 값은 배열 등의 생성에 쓰인다.
};

enum GEVENT_STATE
{
	GEVENT_STATE_NOT_RESERVE=0,			// 아직 예약되지 않은 상태
	GEVENT_STATE_RESERVED,				// 예약된 이벤트
	GEVENT_STATE_START,					// 진행중인 이벤트
	GEVENT_STATE_ENDED,					// 끝난 이벤트.
};


//////////////////////////////////////////////////////////////////////////
// 타이머에 들어가는 인자 정보.
//////////////////////////////////////////////////////////////////////////
enum
{
	EN_GEVENT_INFO_SIZE=56,			// 인자는 56 바이트를 넘어서는 안된다.
};

struct __gevent_timer
{
	time_t			event_time;
	int				event_type;		// << GEVENT_TYPE 에 따라, arg 를 적당한 구조체로 캐스팅 한다.
	char			arg[EN_GEVENT_INFO_SIZE];

	char * GetInfo()	{ return arg; }
};

class _CmpTimer		// priority queue 의 삽입 순서를 결정하는 Compare 클래스.
{
public:
	inline bool operator()(const __gevent_timer* p1, const __gevent_timer* p2) const
	{
		if( p1 && p2 )
		{
			return (p1->event_time >= p2->event_time);
		}
		else
			return true;
	}
};


//////////////////////////////////////////////////////////////////////////
// 스케줄에 있는 이벤트들을 저장하는 구조체.
// 이벤트 종류( GEVENT_TYPE ) 에 따라, 상속 받아 사용한다.
//////////////////////////////////////////////////////////////////////////
// 멤버 event_state 를 제외하고, 나머지는 정적 정보다.
#pragma pack(push, gevent)
#pragma pack(1)

struct _sGEvent
{
	u_char				event_type;			// GEVENT_TYPE
	u_char				event_state;		// GEVENT_STATE
	u_short				event_temp_index;	// 

	// Ready/Start/End
	_sND_TimeUnit 		ready_time;
	_sND_TimeUnit 		start_time;
	_sND_TimeUnit 		end_time;

	_sGEvent()
		: event_type(0), event_state(GEVENT_STATE_NOT_RESERVE)
	{
		ZeroMemory( &ready_time, sizeof(_sND_TimeUnit) );
		ZeroMemory( &start_time, sizeof(_sND_TimeUnit) );
		ZeroMemory( &end_time, sizeof(_sND_TimeUnit) );
	}

	_sGEvent( u_char type )
		: event_type(type), event_state(GEVENT_STATE_NOT_RESERVE)
	{
		ZeroMemory( &ready_time, sizeof(_sND_TimeUnit) );
		ZeroMemory( &start_time, sizeof(_sND_TimeUnit) );
		ZeroMemory( &end_time, sizeof(_sND_TimeUnit) );
	}	

	_sGEvent( const _sGEvent& rh )
		: event_type(rh.event_type), event_state(rh.event_state)
	{
		CopyMemory( &ready_time, &rh.ready_time, sizeof(_sND_TimeUnit) );
		CopyMemory( &start_time, &rh.start_time, sizeof(_sND_TimeUnit) );
		CopyMemory( &end_time, &rh.end_time, sizeof(_sND_TimeUnit) );
	}
};

// 시스템 이벤트는 _sGEvent을 그대로 사용한다. : GEVENT_TYPE_SYSTEM

// 자유비무 이벤트를 위한 구조체 : GEVENT_TYPE_FREE_MATCH 
// GS 통신에서 쓰이므로, 바뀌면, GS도 함께 바꿔야 한다.
struct _sFreeMatchEvent : public _sGEvent
{
	u_char			match_type;
	u_char			match_index;			// 아무 의미 없다. 

	short			from_level;
	short			to_level;

	u_short			fm_rank;				// 대회의 등급 - 별호의 구분 부여를 위해 쓰인다.

	u_short			reward_index;

	short			pos_region;
	short			respawn_region;

	short			buff_index;
	u_short			buff_time;
	u_char			buff_step;
};


// 우승자 정보.
struct _sFM_Record
{
	int					chr_index;				// 캐릭터 인덱스

	u_short				kill_count;				// 죽인 회수
	u_short				dead_count;				// 죽은 회수

	int					last_score;				// 최종 점수.

	char				chr_name[en_charac_name_length+1];
	char				pad[3];

	_sFM_Record()
	{
		Reset();
	}

	void Reset()
	{
		ZeroMemory( this, sizeof(_sFM_Record) );
	}
};


enum EN_FM_RESULT		// 자유비무의 결과.
{
	en_fm_result_one_winer=0,	// One winner
	en_fm_result_more_winner,	// two or more winners.
	en_fm_result_none,		// no winner
};
// 대회 결과 정보
struct _sFM_Result
{
	int					result_info;		// EN_FM_RESULT
	_sFM_Record			winnerInfo;
};


// 최종 보상 별호 소유자 정보.
// 별호의 구분이 비무대회의 등급(fm_rank)으로 구분되므로, 
// fm_rank 값을 기억하고, 보상 별호의 내용이 바뀔 수 있으므로, 함께 저장한다.
struct _sFM_LastRewardNickInfo
{
	u_short				fm_rank;				// 
	u_short				reward_nick_id;

	int					winner_cuid;

	_sFM_LastRewardNickInfo()
		: fm_rank(0), reward_nick_id(0), winner_cuid(0)
	{
	}
};


#pragma pack( pop, gevent )

//////////////////////////////////////////////////////////////////////////
// 타이머 인자 정보
//////////////////////////////////////////////////////////////////////////
// 기본 클래스 : 이벤트 타입에 따라, __timer_arg 를 상송받아 사용한다.
struct __timer_arg
{
	u_short			event_temp_index;
	u_short			work;	
};



// 0. Event Type : GEVENT_TYPE_SYSTEM 타이머에 들어갈 인자
enum SYSTEM_EVENT_WORK	// GEVENT_TYPE_SYSTEM 의 work 정의
{
	en_gevent_syscmd_restart=0,				// 이전것을 취소하고 다시한다.
	en_gevent_syscmd_reserve_next,			// 다음 이벤트를 예약한다.
};

struct _sSystem_Arg	: public __timer_arg
{
	int				sys_cmd;			// en_gevent_syscmd_...
};

//////////////////////////////////////////////////////////////////////////

// 1. Event Type : GEVENT_TYPE_FREE_MATCH 타이머에 들어갈 변수.
enum FREE_MATCH_WORK
{
	en_fm_work_ready=0,				// ready 이벤트 처리.
	en_fm_work_start,				// start 이벤트 처리.
	en_fm_work_end,					// end 이벤트 처리
	en_fm_work_notice,				// 공지 사항
	en_fm_work_notice_winner,		// 공지 : 우승자 정보를 공지
	en_fm_work_notice_nowinner,		// 공지 : 무승부 공지
	en_fm_work_to_winner,			// 승자에게만.
	en_fm_work_send_result,			// 결과를 보내준다.
	en_fm_work_reward_nick,			// 별호 보상 지급,
	en_fm_work_reward_item,
	en_fm_work_buff_all,			// 현 지역의 모든 유저.
	en_fm_work_clean,				// 완전 종료.
	en_fm_work_type_count,
};
struct _sFreeMatch_Arg : public __timer_arg
{
	int				arg1;			// 넣을 게 없다.
	u_char			match_type;		// 
	u_char			arg2;			// 
	u_short			arg3;			// 
};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 자유 비무를 위한 정의 및 구조체
//////////////////////////////////////////////////////////////////////////
// 자유비무의 사용 제한 (Constraints) 정의
enum FREE_MATCH_CONST_FLAG
{
	// 32 개 미만으로 잡아야 한다.
	en_fm_cf_hyperrun=0,
	en_fm_cf_breath,
	en_fm_cf_logout,
	en_fm_cf_escape,
	en_fm_cf_party,
	en_fm_cf_trade,
	en_fm_cf_info,
	en_fm_cf_recall, 
	en_fm_cf_shop,
	en_fm_cf_hide_me,	
	en_fm_cf_buff_reset,	// 10
	en_fm_cf_pvp_event,
};


// 사용 가능 아이템 정보
struct __fm_use_item
{
	char		cType;
	char		dummy;
	short		sID;

	__fm_use_item( char type, short id )
		: cType( type ), dummy(0), sID(id)
	{}
};


// 단계정의
enum FREE_MATCH_STEP		// event 로도 쓴다.
{
	en_fm_step_none=0,
	en_fm_step_ready,
	en_fm_step_start,
	en_fm_step_end
};

// GEventModule 이 단계별 세부 작업을 정의하기 위해 사용한다.
struct __fm_event_work
{
	int			work;
	u_short		usOrgSecond;		// 스크립트에 정의된 초단위 시간.
	u_short		usDiffPrevSecond;	// 앞 work 와의 차이 시간( 초 단위 )

	union
	{
		struct 
		{
			u_short		textID;
			u_short		arg;
		} notice;

		struct 
		{
			short id;
			u_short time;
		} buff_all;
	};

	__fm_event_work *	pNext;

	__fm_event_work()
	{
		ZeroMemory( this, sizeof(__fm_event_work) );
	}
};




#endif	// _PD_GAME_EVENT_