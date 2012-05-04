#pragma once

#ifdef _PD_CASTLE_STEP_1_

#include "CastleStructure.h"

#include <vector>
#include <list>
#include <algorithm>
using namespace std;

#ifdef	 _PD_CASTLE_STEP_2_
class CRecordOfCombat;
#endif// _PD_CASTLE_STEP_2_


class COLE_DB_Executer;		// DB 연결 객체 포인터를 갖기 위해서.

enum
{
	en_cb_group_type_org=0,
	en_cb_group_type_ally,
	en_cb_group_type_monster
};

class utChallengeList	// 단일 링크드 리스트
{
public:
	utChallengeList()
	{
		if( false == init_challenge() )
		{
			printf( "[FAIL] Combat Schedule List - Need more memory\n" );
			throw "";
		}
	}

	bool init_challenge();			// 리스트를 초기화. 메모리 확보.
	void reset_all_challenge();		// 리스트를 Pool 로 모두 옮긴다. schedule 초기화.
	void reset_all_challenge( COLE_DB_Executer * pDB );


	bool insert_challenge( int org, short type, u_short num, char * masterName, char * orgName );			// 꽉 차면 false.
	bool insert_challenge( __cb_challenge_info * pinfo );
	int delete_challenge( int org, u_char type );

public:	// manage
	void make_schedule();
	void send_req_schedule();

	u_char fill_packet_challenge_list( CMSG_CB_MANOR_INFORMATION& info );

public:	// debug
	void pring_list();

#ifdef	_PD_CASTLE_STEP_2_
int fill_index(int* pIndex)
{
    __cb_challenge_info * p = NULL;
	int count = 0;		// 리턴값으로 쓰인다.

	list<__cb_challenge_info *>::iterator itr = m_scheduleList.begin();

	while( itr != m_scheduleList.end() && count < max_count_of_challenge_list*2 )
	{
		p = *itr;
		pIndex[count] = p->iIndex;
		g_pSystemLog->LogFileWrite( "[Combat_Record] challenge_List count(%d) Index(%d) -fill_index-\n",count,p->iIndex );
		++count;
		itr++;
	}
	return count;	
}
#endif//_PD_CASTLE_STEP_2_

protected:
	void push_back_list( __cb_challenge_info * p );
	__cb_challenge_info * pop_list( int index, u_char type );
	void push_pool( __cb_challenge_info * p )
	{
		if( m_poolList.end() != find( m_poolList.begin(), m_poolList.end(), p ) )
			m_poolList.push_back( p );
	}
	__cb_challenge_info * get_from_pool()
	{
		__cb_challenge_info * ret = NULL;
		if( m_poolList.empty() )
		{
			ret = new __cb_challenge_info;
		}
		else
		{
			ret = m_poolList.front();
			m_poolList.pop_front();
		}
		return ret;
	}

protected:
	// DB초기화시에, m_scheduleList 의 개수가 20을 넘어도 모두 리스트에 포함시킨다.
	// GS 에도 똑같이 전달하고, 개수가 20개 미만이 될 경우부터, 도전 신청이 가능하도록 한다.
	list<__cb_challenge_info *>		m_scheduleList;		// 도전 스케줄
	list<__cb_challenge_info *>		m_poolList;			// 스케줄 유닛 풀
	/*
	상태에 따라, 스케줄 관리를 하거나 하지 않는다.
	만약, 중간에 오류가 생기면		( normal -> error!! )
	, 리스트를 초기화하고,			( abnormal_req )
	DS 에 정보를 다시 요청하고		( abnormal_req -> abnormal_wait )
	재구성한 후						( abnoraml_wait -> normal )
	스케줄 관리를 다시 시작한다.
	*/
	int				m_iListState;

	enum
	{
		en_cb_schlist_state_not_init=0,
		en_cb_schlist_state_normal,
		en_cb_schlist_state_abnormal_req,		// 이 상태일 때는 스케줄 관리를 하지 않는다.
		en_cb_schlist_state_abnormal_wait
	};
};



/*////////////////////////////////////////////////////////////////////////
//
//  DS 스케줄러 관리자 
// 
//
////////////////////////////////////////////////////////////////////////*/
/*
첫번째 안 : 
DS 장원 정보를 GS 와 별도로 갖고, 
Info 클래스(개별 정보)와 매니저로 구분한다. 

매니저는 개별 정보를 벡터로 관리한다.
*/

//////////////////////////////////////////////////////////////////////////
// 장원의 개별 정보.
//////////////////////////////////////////////////////////////////////////
class CDSCastleInfo
{
public:
	CDSCastleInfo()
		: m_bInitedFlag(false), m_ucRes1(0), m_sMapID(0), m_iDB_SeqIndex_of_Owner(0)
	{
		InitializeCriticalSectionAndSpinCount( &m_cs, 0x80001000 );
		ZeroMemory( &m_CastleInfo, sizeof(m_CastleInfo) );
	}

	//void set_castle_info( short id, _sCastleInfo& info );		// 장원 정보를 초기화 한다.
	//////////////////////////////////////////////////////////////////////////
	// set
	//////////////////////////////////////////////////////////////////////////
	void set_from_info( CDSCastleInfo& info );
	void set_from_info( short map_id, _sCastleInfo * pInfo );
	void reset_info();

	void reset_challenge_list()	{ m_ChallengeList.reset_all_challenge(); }
	void reset_challenge_list( COLE_DB_Executer * pDB )
	{
		m_ChallengeList.reset_all_challenge( pDB );
	}
	//
	void set_map_index( short id )				{ m_sMapID = id; }
	void set_db_seq_index( int index )			{ m_iDB_SeqIndex_of_Owner = index; }
	void set_or_index( int or, u_char type )	{ m_CastleInfo.iOrIndex = or; m_CastleInfo.ucType = type; }
	void set_or_member_count( u_short count )	{ m_CastleInfo.usMemberCount = count; }
	void set_castle_level( u_char level )		{ m_CastleInfo.ucLevel = level; }
#ifdef _PD_CASTLE_RENEWAL_1_
	void set_cur_match_count( u_char count )	{ m_CastleInfo.ucCurMatchCount_not = count; }
#else
	void set_cur_match_count( u_char count )	{ m_CastleInfo.ucCurMatchCount = count; }
#endif
	void set_or_master_moonpa( char moonpa )	{ m_CastleInfo.cMasterMoonpa = moonpa; }
	void set_total_money( u_int money )			{ m_CastleInfo.uiTotalMoney = money; }
	void set_part_money( u_int money )			{ m_CastleInfo.uiPartMoney = money; }
	void set_win_count( u_short count )			{ m_CastleInfo.usWinCount = count; }
	void set_or_name( char * name )	
	{
		strncpy( m_CastleInfo.cOrName, name, en_or_name_length );
		m_CastleInfo.cOrName[en_or_name_length] = 0;
	}
	void set_or_master_name( char * name )		
	{ 
		strncpy( m_CastleInfo.cMasterName, name, en_or_name_length );
		m_CastleInfo.cMasterName[en_or_name_length] = 0;
	}

#ifdef _PD_CASTLE_RENEWAL_1_
	void set_date_of_own_from_time( int t )
	{
		m_CastleInfo.timeOfGettCastle.uiSecond = 0;
		m_CastleInfo.timeOfGettCastle.uiMinute = 0;
		m_CastleInfo.timeOfGettCastle.uiHour = t % 100;
		m_CastleInfo.timeOfGettCastle.uiDay = ( t%10000 ) / 100;
		m_CastleInfo.timeOfGettCastle.uiMonth = ( t%1000000 ) / 10000;
		m_CastleInfo.timeOfGettCastle.uiYear = (t/1000000) - 2000;
	}
	void set_date_of_own_from_time( u_int y, u_int m, u_int d, u_int h )	// 조작 없이 set 만.
	{
		m_CastleInfo.timeOfGettCastle.uiYear = y;
		m_CastleInfo.timeOfGettCastle.uiMonth = m;
		m_CastleInfo.timeOfGettCastle.uiDay = d;
		m_CastleInfo.timeOfGettCastle.uiHour = h;
		m_CastleInfo.timeOfGettCastle.uiMinute = 0;
		m_CastleInfo.timeOfGettCastle.uiSecond = 0;
	}
	void set_combat_date_from_time( int t )
	{
		m_CastleInfo.timeNextSchedule.uiSecond = 0;
		m_CastleInfo.timeNextSchedule.uiMinute = 0;
		m_CastleInfo.timeNextSchedule.uiHour = t % 100;
		m_CastleInfo.timeNextSchedule.uiDay = ( t%10000 ) / 100;
		m_CastleInfo.timeNextSchedule.uiMonth = ( t%1000000 ) / 10000;
		m_CastleInfo.timeNextSchedule.uiYear = (t/1000000) - 2000;
	}
	void set_combat_date_from_time( u_int y, u_int m, u_int d, u_int h )
	{
		m_CastleInfo.timeNextSchedule.uiSecond = 0;
		m_CastleInfo.timeNextSchedule.uiMinute = 0;
		m_CastleInfo.timeNextSchedule.uiHour = h;
		m_CastleInfo.timeNextSchedule.uiDay = d;
		m_CastleInfo.timeNextSchedule.uiMonth = m;
		m_CastleInfo.timeNextSchedule.uiYear = y;
	}
	void set_combat_date_from_time( _sND_TimeUnit * timeUnit )
	{
		CopyMemory( &m_CastleInfo.timeNextSchedule, timeUnit, sizeof(_sND_TimeUnit) );
	}

	void set_cur_state( u_char match )	{ m_CastleInfo.ucCombatState = match; }
	u_char get_cur_state()				{ return m_CastleInfo.ucCombatState; }
#else
	void set_date_of_own_from_time( int t )
	{
		m_CastleInfo.timeUnit.uiSecond = 0;
		m_CastleInfo.timeUnit.uiMinute = 0;
		m_CastleInfo.timeUnit.uiHour = t % 100;
		m_CastleInfo.timeUnit.uiDay = ( t%10000 ) / 100;
		m_CastleInfo.timeUnit.uiMonth = ( t%1000000 ) / 10000;
		m_CastleInfo.timeUnit.uiYear = (t/1000000) - 2000;
	}
	void set_date_of_own_from_time( u_int y, u_int m, u_int d, u_int h )	// 조작 없이 set 만.
	{
		m_CastleInfo.timeUnit.uiYear = y;
		m_CastleInfo.timeUnit.uiMonth = m;
		m_CastleInfo.timeUnit.uiDay = d;
		m_CastleInfo.timeUnit.uiHour = h;
		m_CastleInfo.timeUnit.uiMinute = 0;
		m_CastleInfo.timeUnit.uiSecond = 0;
	}
	void set_combat_date_from_time( int t, int slot )
	{
		if( slot < 3 )
		{
			m_CastleInfo.Schedule[slot].uiSecond = 0;
			m_CastleInfo.Schedule[slot].uiMinute = 0;
			m_CastleInfo.Schedule[slot].uiHour = t % 100;
			m_CastleInfo.Schedule[slot].uiDay = ( t%10000 ) / 100;
			m_CastleInfo.Schedule[slot].uiMonth = ( t%1000000 ) / 10000;
			m_CastleInfo.Schedule[slot].uiYear = (t/1000000) - 2000;
		}
	}
	void set_combat_date_from_time( int slot, u_int y, u_int m, u_int d, u_int h )
	{
		if( slot < 3 )
		{
			m_CastleInfo.Schedule[slot].uiSecond = 0;
			m_CastleInfo.Schedule[slot].uiMinute = 0;
			m_CastleInfo.Schedule[slot].uiHour = h;
			m_CastleInfo.Schedule[slot].uiDay = d;
			m_CastleInfo.Schedule[slot].uiMonth = m;
			m_CastleInfo.Schedule[slot].uiYear = y;
		}
	}
	void set_combat_date_from_time( int slot, _sND_TimeUnit * timeUnit )
	{
		if( slot < 3 )
		{
			CopyMemory( &m_CastleInfo.Schedule[slot], timeUnit, sizeof(_sND_TimeUnit) );
		}
	}


	void set_cur_state( u_char match )	{ m_CastleInfo.ucMatchState = match; }
	u_char get_cur_state()				{ return m_CastleInfo.ucMatchState; }

#endif


	void change_owner_to_monster();


	// 도전 목록을 추가한다.
	void add_challenge_list( __cb_challenge_info * pInfo );
	int del_challenge( __cb_challenge_info * pInfo );
	int del_challenge( int index, u_char type );

	//////////////////////////////////////////////////////////////////////////
	// get
	//////////////////////////////////////////////////////////////////////////
	short get_map_index()					{ return m_sMapID; }
	int get_db_seq_index()					{ return m_iDB_SeqIndex_of_Owner; }

	inline int get_owner_index()			{ return m_CastleInfo.iOrIndex; }
	inline u_char get_owner_type()			{ return m_CastleInfo.ucType; }
	inline char * get_owner_name()			{ return m_CastleInfo.cOrName; }
	inline u_char get_level()				{ return m_CastleInfo.ucLevel; }
#ifdef _PD_CASTLE_RENEWAL_1_
	inline u_char get_cur_match_count()		{ return m_CastleInfo.ucCurMatchCount_not; }
#else
	inline u_char get_cur_match_count()		{ return m_CastleInfo.ucCurMatchCount; }
#endif
	inline char * get_master_name()			{ return m_CastleInfo.cMasterName; }
	inline u_int get_total_money()			{ return m_CastleInfo.uiTotalMoney; }
	inline u_int get_part_money()			{ return m_CastleInfo.uiPartMoney; }
	inline u_short get_win_count()			{ return m_CastleInfo.usWinCount; }

#ifdef _PD_CASTLE_RENEWAL_1_
	inline _sND_TimeUnit * get_own_date()	{ return &m_CastleInfo.timeOfGettCastle; }
	inline _sND_TimeUnit * get_schedule()	{ return &m_CastleInfo.timeNextSchedule; }
#else
	inline _sND_TimeUnit * get_own_date()	{ return &m_CastleInfo.timeUnit; }
	inline _sND_TimeUnit * get_schedule( int slot )	{ return &m_CastleInfo.Schedule[slot]; }
#endif
	inline _sCastleInfo * get_castle_info()	{ return &m_CastleInfo; }


	//////////////////////////////////////////////////////////////////////////
	// 스케줄 관련 함수.
	//////////////////////////////////////////////////////////////////////////
	void make_default_schedule( int firstHour, int secondHour, int thirdHour );

	//////////////////////////////////////////////////////////////////////////
	// fill information packet.
	//////////////////////////////////////////////////////////////////////////
	void fill_packet_manor_information( CMSG_CB_MANOR_INFORMATION& info );	

#ifdef _PD_CASTLE_STEP_2_
	//////////////////////////////////////////////////////////////////////////
	// 세금.
	//////////////////////////////////////////////////////////////////////////
	void tax_add( u_int tax, u_int ref_total, bool bTemp );
	void tax_request_take( u_int req_money, CMSG_MANOR_TAX_ANSWER_TAKE * pAnswer, COLE_DB_Executer * pDB );

	int	fill_challenge_index(int* pIndex)
	{
		int count =  m_ChallengeList.fill_index(pIndex);
		return count;
	}
#endif//_PD_CASTLE_STEP_2_

protected:
	inline void lock()		{ EnterCriticalSection(&m_cs); }
	inline void unlock()	{ LeaveCriticalSection(&m_cs); }


public:
	//////////////////////////////////////////////////////////////////////////
	// 테스트 함수 ///////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	void test_make_random_schedule();				// 
	void print_info();

public:
	bool is_inited()				{ return m_bInitedFlag; }
	void set_init_flag( bool b )	{ m_bInitedFlag = b; }

protected:
	bool						m_bInitedFlag;		// 0 : Not initiailized, 1 : Initialized.
	u_char						m_ucRes1;			// not userd
	short						m_sMapID;
	int							m_iDB_SeqIndex_of_Owner;
	_sCastleInfo				m_CastleInfo;		// 장원 정보

	utChallengeList				m_ChallengeList;

	CRITICAL_SECTION			m_cs;
};


/*////////////////////////////////////////////////////////////////////////

	장원전 정보 관리자 Manager

////////////////////////////////////////////////////////////////////////*/
class CDSCastleManager : public ckSingleton<CDSCastleManager>
{
public:
	CDSCastleInfo * get_castle_info( short index )
	{
		size_t i;
		for( i = 0 ; i < m_Castles.size() ; ++i )
		{
			if( index == m_Castles[i]->get_map_index() )
				break;
		}

		if( i < m_Castles.size() )
			return m_Castles[i];
		else
			return NULL;
	}
	CDSCastleInfo * get_castle_info_by_db_seq( int seq )
	{
		size_t i;
		for( i = 0 ; i < m_Castles.size() ; ++i )
		{
			if( seq == m_Castles[i]->get_db_seq_index() )
				break;
		}

		if( i < m_Castles.size() )
			return m_Castles[i];
		else
			return NULL;
	}

	CDSCastleInfo * get_castle_by_owner( u_char type, int index )
	{
		size_t i;
		for( i = 0 ; i < m_Castles.size() ; ++i )
		{
			if( type == m_Castles[i]->get_owner_type() && index == m_Castles[i]->get_owner_index() )
				break;
		}

		if( i < m_Castles.size() )
			return m_Castles[i];
		else
			return NULL;
	}

	bool is_combat_ing()
	{
		bool b = false;
		for( size_t i = 0 ; i < m_Castles.size() ; ++i )
		{
			if( 1 == m_Castles[i]->get_cur_state() )
			{
				b = true;
				break;
			}
		}

		return b;
	}

	void reset_state()
	{
		for( size_t i = 0 ; i < m_Castles.size() ; ++i )
		{
			m_Castles[i]->set_cur_state(0);
		}
	}

	void cmd_change_owner_to_monster( int castle_index );

	u_int get_default_match_hour()	{ return m_uiDefaultMatchHour; }

	bool read_castle_info_from_db( COLE_DB_Executer * db );		// DB 에서 읽기.
	bool read_castles();
	bool read_detail_info_each_castle();
#ifdef _PD_CASTLE_STEP_2_
	bool read_record();//DB에서 장원전적 읽기
#endif

	void Check_OrgAlly( COrganizationManager * pOrgManager, CAllyManager * pAllyManager );

	CDSCastleInfo * add_castle_index_only( short index, int db_row_index );// DB 에서 장원 리스트를 읽어, 준비만 시킨다.
	void add_castle_info( short index, CDSCastleInfo& info );	// 먼저 인덱스만 추가되고, 이후 이 함수를 통해서 자세한 정보가 추가된다.

public:

	//////////////////////////////////////////////////////////////////////////
	// process gs request packets.
	//////////////////////////////////////////////////////////////////////////
	void proc_gs_req_manor_information( CMSG_CB_REQ_MANOR_INFORMATION * pReqManorInformation, _h_DS_Com * pDSCom );
	void proc_gs_req_change_time( CMSG_CB_REQ_CHANGE_TIME * pReqChangeTime, _h_DS_Com * pDSCom );
	void proc_gs_challenge_insert( CMSG_CB_CHALLENGE_INSERT * pReqChallengeInsert, _h_DS_Com * pDSCom );
	void proc_gs_challenge_delete( CMSG_CB_CHALLENGE_DELETE * pReqChallengeDelete, _h_DS_Com * pDSCom );
	void proc_gs_challenge_reset( CMSG_CB_CHALLENGE_RESET * pChallengeReset );
	void proc_gs_owner_info_changed( CMSG_CB_OWNER_INFO_CHANGED * pOwnerChanged );
	void proc_gs_combat_start( CMSG_CB_COMBAT_START * pCombatStart );
	void proc_gs_combat_end( CMSG_CB_COMBAT_END * pCombatEnd );
#ifdef	 _PD_CASTLE_STEP_2_
	void proc_gs_req_record_info(CMSG_RECORD_INFORMATION_REQ * pReqRecord, _h_DS_Com * pDSCom);
	void proc_gs_update_record_info(CMSG_RECORD_UPDATE_GS_DS * pUpdateRecord);
	void proc_gs_reset_record_info(CMSG_RECORD_RESET * pResetRecord);
#endif// _PD_CASTLE_STEP_2_


	void notice_group_breakup( u_char group_type, int index );
	void notice_server_down( int server_index );

#ifdef _PD_CASTLE_STEP_2_
	//////////////////////////////////////////////////////////////////////////
	// 세금 부분
	void tax_add_from_tax_system( short manor_index, u_int tax, u_int totol_money, bool bTemp = false );
	COLE_DB_Executer *	get_game_db_con()	{ return m_pDB; }
	CRecordOfCombat * GetRecord()	{ return m_Record;	}
	void			  SetRecord();

#endif//_PD_CASTLE_STEP_2_

	//////////////////////////////////////////////////////////////////////////
	// 장원 정보
protected:
	vector<CDSCastleInfo*>		m_Castles;		// 장원 정보는 벡터로 관리.

#ifdef	_PD_CASTLE_STEP_2_
	CRecordOfCombat* 		m_Record;	
#endif//_PD_CASTLE_STEP_2_

	u_int					m_uiDefaultMatchHour;



	//////////////////////////////////////////////////////////////////////////
	// 내부 함수
	//////////////////////////////////////////////////////////////////////////
public:
	void print_castle_list();
	void print_castle_info( short map_id );
	void print_castle_by_db_seq( int seq );

protected:
	COLE_DB_Executer *			m_pDB;

};


#endif	// _PD_CASTLE_STEP_1_