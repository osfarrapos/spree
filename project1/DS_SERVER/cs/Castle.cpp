#include "../stdafx.h"

#include "..\Network\Network.h"
#include "../Network/LogSvrLink.h"

#include "Organization.h"

#include "Castle.h"

#ifdef	 _PD_CASTLE_STEP_2_
#include "combatrecord.h"
#endif// _PD_CASTLE_STEP_2_

#include "..\Global\tcp_packets_party.h"

#ifdef _PD_ALLY_ORG_STEP_1_
extern CAllyManager *	g_pAllyManager;
extern COrganizationManager *	g_pOrganizationManager ;
#endif



#ifdef _PD_CASTLE_STEP_1_

extern _j_ServerInfo *			g_pServerInfo ;
extern _DSCom_Queue *		g_pDSComQueue ;

extern CLogSvr_Com *		g_pLogSvrCom;

//////////////////////////////////////////////////////////////////////////
bool utChallengeList::init_challenge()
{
	// max_count_of_challenge_list
	int i;
	__cb_challenge_info * p;
	for( i=0; i<max_count_of_challenge_list; ++i )
	{
		p = new __cb_challenge_info;
		if( p )
			push_pool( p );
	}

	m_iListState = en_cb_schlist_state_normal;

	return true;
}


void utChallengeList::reset_all_challenge()
{
	// free schedule list
	list<__cb_challenge_info*>::iterator itr = m_scheduleList.begin();
	while( itr != m_scheduleList.end() )
	{
		(*itr)->reset();
		push_pool( *itr );

		++itr;
	}

	m_scheduleList.clear();
}

void utChallengeList::reset_all_challenge( COLE_DB_Executer * pDB )
{
	// pDB 로 현재 모든 도전 목록을 complete 상태로 바꾼다.
	if( NULL == pDB )
	{
		reset_all_challenge();
		return;
	}

	__cb_challenge_info * pInfo;

	list<__cb_challenge_info*>::iterator itr = m_scheduleList.begin();
	while( itr != m_scheduleList.end() )
	{
		pInfo = *itr;

		
		pDB->CompleteChallenge( pInfo->iDBSeqNumber );

		pInfo->reset();

		push_pool( *itr );

		++itr;
	}

	m_scheduleList.clear();
}

// insert challenge : 도전 정보 추가
bool utChallengeList::insert_challenge( int org, short type, u_short num, char * masterName, char * orgName )
{
	__cb_challenge_info * p = get_from_pool();

	if( p )
	{
		p->iIndex = org;
		p->ucType = (type) ? 1 : 0;
		p->ucState  = 0;
		p->usMemberCount = num;
		strncpy( p->cMasterName, masterName, en_charac_name_length );
		p->cMasterName[en_charac_name_length] = 0;
		strncpy( p->cGroupName, orgName, en_or_name_length );
		p->cGroupName[en_or_name_length] = 0;

		push_back_list( p );

		return true;
	}
	else
		return false;
}


bool utChallengeList::insert_challenge( __cb_challenge_info * pinfo )
{
	__cb_challenge_info * p = get_from_pool();

	if( p )
	{
		CopyMemory( p, pinfo, sizeof(__cb_challenge_info) );
		push_back_list( p );
		return true;
	}
	else
		return false;
}

int utChallengeList::delete_challenge( int org, u_char type )
{
	int db_seq = 0;
	__cb_challenge_info * p = pop_list( org, type );

	if( p )
	{
		db_seq = p->iDBSeqNumber;
		ZeroMemory( p, sizeof(__cb_challenge_info) );
		push_pool( p );
	}

	return db_seq;
}

//////////////////////////////////////////////////////////////////////////
u_char utChallengeList::fill_packet_challenge_list( CMSG_CB_MANOR_INFORMATION& info )
{
	__cb_challenge_info * p = NULL;
	u_char count = 0;		// 리턴값으로 쓰인다.

	list<__cb_challenge_info *>::iterator itr = m_scheduleList.begin();

	while( itr != m_scheduleList.end() && count < max_count_of_challenge_list*2 )
	{
		p = *itr;

		CopyMemory( &info.challengeList[count], p, sizeof(__cb_challenge_info) );

		++count;

		++itr;
	}

	return count;
}


//////////////////////////////////////////////////////////////////////////

// protected
void utChallengeList::push_back_list( __cb_challenge_info * p )
{
	m_scheduleList.push_back( p );
}

__cb_challenge_info * utChallengeList::pop_list( int index, u_char type )
{
	__cb_challenge_info * pInfo = NULL;
	__cb_challenge_info * pRet = NULL;
	list<__cb_challenge_info*>::iterator itr = m_scheduleList.begin();

	while( itr != m_scheduleList.end() )
	{
		pInfo = *itr;
		if( pInfo->iIndex == index && pInfo->ucType == type )
		{
			pRet = pInfo;
			m_scheduleList.erase( itr );
			break;
		}

		++itr;
	}

	return pRet;
}


// debug
void utChallengeList::pring_list()
{
	int i = 0;
	__cb_challenge_info * p = NULL;
	list<__cb_challenge_info*>::iterator itr = m_scheduleList.begin();
	while( itr != m_scheduleList.end() )
	{
		p = *itr;

		printf( "  [%2d-DB(%d)] %4s : %5d - Name(%s), Master(%s)\n",
			++i, p->iDBSeqNumber, ( p->ucType == 0 ) ? "Org" : "Ally", p->iIndex, 
			p->cGroupName, p->cMasterName );

		++itr;
	}
}

/*////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	CDSCastleInfo : 장원 개별 정보
		- 소유 정보
		- 스케줄 정보
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////*/

void CDSCastleInfo::set_from_info( CDSCastleInfo& info )
{
}

// 이 함수는 소유가 바뀌었을 때 호출된다.
void CDSCastleInfo::set_from_info( short map_id, _sCastleInfo * pInfo )
{
	if( map_id )
	{
		set_map_index( map_id );
		set_db_seq_index(0);		// 우선 0으로 초기화. 바깥에서 DB 호출한 후 세팅한다. 
		CopyMemory( &m_CastleInfo, pInfo, sizeof(m_CastleInfo) );
		set_init_flag( true );
	}
}

void CDSCastleInfo::add_challenge_list( __cb_challenge_info * pInfo )
{
	m_ChallengeList.insert_challenge( pInfo );
}

int CDSCastleInfo::del_challenge( __cb_challenge_info * pInfo )
{
	return m_ChallengeList.delete_challenge( pInfo->iIndex, pInfo->ucType );
}

int CDSCastleInfo::del_challenge( int index, u_char type )
{
	return m_ChallengeList.delete_challenge( index, type );
}

void CDSCastleInfo::change_owner_to_monster()
{
	set_or_index( 0, en_cb_group_type_monster );
	set_or_member_count(0);
	set_castle_level(0);
	set_cur_match_count(0);
	set_or_name( "monsters" );
	set_or_master_name( "monster" );
	set_or_master_moonpa(0);
	set_total_money(0);
	set_part_money(0);
	set_win_count(0);

	time_t curTimeT;
	time( &curTimeT );		// 현재 시간.

	tm * localT = localtime( &curTimeT );	// Convert time_t to tm.(local coreection)
	set_date_of_own_from_time( localT->tm_year-100, localT->tm_mon+1, localT->tm_mday, localT->tm_hour );

	// 20080324 : 다음 스케쥴 설정 추가
	int default_hour = static_cast<int>(CDSCastleManager::GetObj().get_default_match_hour());
	make_default_schedule( default_hour, default_hour, default_hour );
}


//////////////////////////////////////////////////////////////////////////
void CDSCastleInfo::make_default_schedule( int firstHour, int secondHour, int thirdHour )
{
	// 장원이 처음 게임에 업데이트 될 때 딱 한 번 불린다.!!!
	// See GS make_next_match_schedule
	
	// 첫째날을 잡은 후, 공통으로 2,3차전 일정을 잡는다.
	time_t curTimeT;
	time( &curTimeT );		// 현재 시간.

	tm * localT = localtime( &curTimeT );	// Convert time_t to tm.(local coreection)
	tm firstScheduleLocalT;
	int defaultHour = firstHour;

	ZeroMemory( &firstScheduleLocalT, sizeof(tm) );

	if( localT->tm_hour >= defaultHour-1 )
	{
		// 다음날로 미룬다.
		//curTimeT += ( 하루 );
		curTimeT += ( _SECOND_DAY_ );		// 다음날을 초로 계산한 것.
	}

	// 1치전 설정.
	tm * targetLocalT = localtime( &curTimeT );
#ifdef _PD_CASTLE_RENEWAL_1_
	m_CastleInfo.timeNextSchedule.uiYear = targetLocalT->tm_year - 100;	
	m_CastleInfo.timeNextSchedule.uiMonth = targetLocalT->tm_mon+1;
	m_CastleInfo.timeNextSchedule.uiDay = targetLocalT->tm_mday;
	m_CastleInfo.timeNextSchedule.uiHour = defaultHour;
	m_CastleInfo.timeNextSchedule.uiMinute = 0;		
	m_CastleInfo.timeNextSchedule.uiSecond = 0;

	// 2차전 설정.
	CopyMemory( &m_CastleInfo.timeunit_not_use[0], &m_CastleInfo.timeNextSchedule, sizeof(_sND_TimeUnit) );
	CopyMemory( &m_CastleInfo.timeunit_not_use[1], &m_CastleInfo.timeNextSchedule, sizeof(_sND_TimeUnit) );

#else
	m_CastleInfo.Schedule[0].uiYear = targetLocalT->tm_year - 100;	
	m_CastleInfo.Schedule[0].uiMonth = targetLocalT->tm_mon+1;
	m_CastleInfo.Schedule[0].uiDay = targetLocalT->tm_mday;
	m_CastleInfo.Schedule[0].uiHour = defaultHour;
	m_CastleInfo.Schedule[0].uiMinute = 0;		
	m_CastleInfo.Schedule[0].uiSecond = 0;

	// 2차전 설정.
	curTimeT += ( _SECOND_DAY_ );
	defaultHour = secondHour;
	targetLocalT = localtime( &curTimeT );
	m_CastleInfo.Schedule[1].uiYear = targetLocalT->tm_year - 100;	
	m_CastleInfo.Schedule[1].uiMonth = targetLocalT->tm_mon+1;
	m_CastleInfo.Schedule[1].uiDay = targetLocalT->tm_mday;
	m_CastleInfo.Schedule[1].uiHour = defaultHour;
	m_CastleInfo.Schedule[1].uiMinute = 0;		
	m_CastleInfo.Schedule[1].uiSecond = 0;

	// 3차전 설정.
	curTimeT += ( _SECOND_DAY_ );
	defaultHour = thirdHour;
	targetLocalT = localtime( &curTimeT );
	m_CastleInfo.Schedule[2].uiYear = targetLocalT->tm_year - 100;	
	m_CastleInfo.Schedule[2].uiMonth = targetLocalT->tm_mon+1;
	m_CastleInfo.Schedule[2].uiDay = targetLocalT->tm_mday;
	m_CastleInfo.Schedule[2].uiHour = defaultHour;
	m_CastleInfo.Schedule[2].uiMinute = 0;		
	m_CastleInfo.Schedule[2].uiSecond = 0;
#endif
}


//////////////////////////////////////////////////////////////////////////
void CDSCastleInfo::fill_packet_manor_information( CMSG_CB_MANOR_INFORMATION& info )
{
	/* 
	u_char					ucIsEnd;			// 0 : continue info. 1 : end info.
	u_char					ucMapID;			// 존 인덱스 : 0 이면, 장원 정보가 하나도 없다는 의미다.
	_sCastleInfo			castleInfo;			// 장원 정보. 채우는 함수도 만들어야 한다.
	u_char					ucCount;			// 도전자 개수.
	__cb_challenge_info		challengeList[max_count_of_challenge_list*2];		//
	*/
	info.ucMapID = (u_char)get_map_index();
	CopyMemory( &info.castleInfo, &m_CastleInfo, sizeof(_sCastleInfo) );
	ZeroMemory( info.challengeList, sizeof(info.challengeList) );

	info.ucCount = m_ChallengeList.fill_packet_challenge_list( info );
}

#ifdef _PD_CASTLE_STEP_2_
//////////////////////////////////////////////////////////////////////////
void CDSCastleInfo::tax_add( u_int tax, u_int ref_total, bool bTemp )
{
	// 세금을 추가한다. 
	CMSG_MANOR_TAX_SET_TOTAL_MONEY pkSetTotalMoney;
	pkSetTotalMoney.ucMapID = static_cast<u_char>(get_map_index());

	lock();

	if( MAX_CASTLE_MONEY - m_CastleInfo.uiPartMoney >= tax )
		m_CastleInfo.uiPartMoney += tax;
	else
		m_CastleInfo.uiPartMoney = MAX_CASTLE_MONEY;
	m_CastleInfo.uiTotalMoney = ref_total;

	pkSetTotalMoney.uiTotalMoney = m_CastleInfo.uiTotalMoney;
	pkSetTotalMoney.uiPartMoney = m_CastleInfo.uiPartMoney;

	unlock();

	if( bTemp )
		pkSetTotalMoney.ucType = 1;
	else
		pkSetTotalMoney.ucType = 0;

	g_pServerInfo->send_to_all_gs( &pkSetTotalMoney, pkSetTotalMoney.usLength );

	// 로그 추가.
	g_pLogSvrCom->LogWrite_ItemTaxAdd( get_map_index(), ref_total, tax, bTemp );
}


void CDSCastleInfo::tax_request_take( u_int req_money, CMSG_MANOR_TAX_ANSWER_TAKE * pAnswer, COLE_DB_Executer * pDB )
{
	lock();

	if( m_CastleInfo.uiPartMoney >= req_money )
	{
		m_CastleInfo.uiPartMoney -= req_money;
		pAnswer->ucResult = 0;
	}
	else
	{
		pAnswer->ucResult = 1;
	}

	pAnswer->uiLeftPartMoney = m_CastleInfo.uiPartMoney;

	unlock();

	// 로그 추가.
	if( 0 == pAnswer->ucResult )
	{
		g_pLogSvrCom->LogWrite_ItemTaxTake( pAnswer->cCharacName, get_map_index(), req_money );
		if( pDB )
			pDB->UpdateDefendCastleInfo( this, 1 );
	}
}

#endif


//////////////////////////////////////////////////////////////////////////

void CDSCastleInfo::print_info()
{
	printf( "\n======= Castle <ID %d - DB %d> Information =======\n", get_map_index(), get_db_seq_index() );
	printf( "Status : %s\n", ( 0 == get_cur_state() ) ? "Peace" : "Combat" );
	printf( "Owner Type = %d, Index = %d, Name = %s\n", m_CastleInfo.get_owner_type(), m_CastleInfo.get_owner_index(), m_CastleInfo.get_owner_name() );
#ifdef _PD_CASTLE_RENEWAL_1_
	printf( "Owner Date : %4u-%02u/%02u %u:%u\n", 
		m_CastleInfo.timeOfGettCastle.uiYear+2000, m_CastleInfo.timeOfGettCastle.uiMonth, m_CastleInfo.timeOfGettCastle.uiDay, 
		m_CastleInfo.timeOfGettCastle.uiHour, m_CastleInfo.timeOfGettCastle.uiMinute );
	printf( "Money : Part(%u) - Total(%u)\n", m_CastleInfo.uiPartMoney, m_CastleInfo.uiTotalMoney );
	printf( "  -  Schedule -\n" );
	printf( "[%4u-%02u/%02u %02u:%02u]\n", 
		m_CastleInfo.timeNextSchedule.uiYear+2000, m_CastleInfo.timeNextSchedule.uiMonth,
		m_CastleInfo.timeNextSchedule.uiDay, m_CastleInfo.timeNextSchedule.uiHour, m_CastleInfo.timeNextSchedule.uiMinute );
#else
	printf( "Owner Date : %4u-%02u/%02u %u:%u\n", 
		m_CastleInfo.timeUnit.uiYear+2000, m_CastleInfo.timeUnit.uiMonth, m_CastleInfo.timeUnit.uiDay, 
		m_CastleInfo.timeUnit.uiHour, m_CastleInfo.timeUnit.uiMinute );
	printf( "Money : Part(%u) - Total(%u)\n", m_CastleInfo.uiPartMoney, m_CastleInfo.uiTotalMoney );
	printf( "  -  Schedule -\n" );
	printf( "[%4u-%02u/%02u %02u:%02u], [%4u-%02u/%02u %02u:%02u], [%4u-%02u/%02u %02u:%02u]\n", 
		m_CastleInfo.Schedule[0].uiYear+2000, m_CastleInfo.Schedule[0].uiMonth,
		m_CastleInfo.Schedule[0].uiDay, m_CastleInfo.Schedule[0].uiHour, m_CastleInfo.Schedule[0].uiMinute,
		m_CastleInfo.Schedule[1].uiYear+2000, m_CastleInfo.Schedule[1].uiMonth,
		m_CastleInfo.Schedule[1].uiDay, m_CastleInfo.Schedule[1].uiHour, m_CastleInfo.Schedule[0].uiMinute,
		m_CastleInfo.Schedule[2].uiYear+2000, m_CastleInfo.Schedule[2].uiMonth,
		m_CastleInfo.Schedule[2].uiDay, m_CastleInfo.Schedule[2].uiHour, m_CastleInfo.Schedule[0].uiMinute );
#endif
	printf( "  - Challenge List -\n" );
	m_ChallengeList.pring_list();
	printf( "=====================================================\n" );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CDSCastleManager::read_castle_info_from_db( COLE_DB_Executer * db )
{
	// 어떤 장원들이 있는지 읽은 후, ( read_castles() )
	// 각각의 자세한 정보를 읽는다. ( read_detail_info_each_castle() )

	m_uiDefaultMatchHour = 20;

	if( db )
	{
		m_pDB = db;
		if( read_castles() )
		{
			return read_detail_info_each_castle();
		}
		return true;
	}
	else
		return false;
}

bool CDSCastleManager::read_castles()
{
	// DB 에 어떤 장원이 있는지 간략 정보를 구한다. ( from ND_V01_Defend_Despot )
	m_pDB->GetDefendDespotInfo( this );
	return true;
}

#ifdef _PD_CASTLE_STEP_2_
bool CDSCastleManager::read_record()
{
	// DB 에 어떤 장원이 있는지 간략 정보를 구한다. ( from ND_V01_Defend_Despot )
	m_pDB->GetCombatRecordInManor();
	m_pDB->GetAllCombatRecord();	

	return true;
}
#endif

bool CDSCastleManager::read_detail_info_each_castle()
{
	// ND_V01_Defend_Castle
	// 장원의 상세 정보를 읽는다.
	vector<CDSCastleInfo*>::iterator itr = m_Castles.begin();

	while( itr != m_Castles.end() )
	{
		if( false == (*itr)->is_inited() )
		{
			// DB 에서 개별 정보를 읽어들인다.
			m_pDB->FillCastleDetailInfo( (*itr)->get_map_index(), (*itr)->get_db_seq_index(), *itr );

		}

		++itr;
	}

	return true;
}


void CDSCastleManager::Check_OrgAlly( COrganizationManager * pOrgManager, CAllyManager * pAllyManager )
{
	CDSCastleInfo * pInfo = NULL;
	_Organization * pOrg;
	CAllyOrganization * pAlly;
	u_char group_type = 0;

	int size = m_Castles.size();

	for( int i = 0 ; i < size ; ++i )
	{
		pInfo = m_Castles[i];

		if( pInfo )
		{
			// 1. 소유 정보 검사
			//   - 없다면, 몬스터로 속성을 바꾸고, 로그로 남긴다.
			// 2. 도전 목록 검사
			//   - 없다면, 로그를 남기고( DB ) 삭제한다. ==> update 3 으로.
			switch( pInfo->get_owner_type() )
			{
			case en_cb_group_type_org:
				pOrg = pOrgManager->FindOrganization( pInfo->get_owner_index() );
				if( pOrg )
				{
					pInfo->set_or_member_count( pOrg->m_usCurMemberNum );
					pInfo->set_or_master_moonpa( pOrg->m_cBaseParty );
				}
				else
				{
					// 영웅단이 없다. 몬스터로 소유를 바꾼다.

					// Save Log
					pInfo->change_owner_to_monster();

					// DB 저장, _prDS_UpdateDefendCastle 을 0으로 호출
					int dfd = m_pDB->UpdateDefendCastleInfo( pInfo, 0 );
					pInfo->set_db_seq_index( dfd );

				}
				break;
			case en_cb_group_type_ally: 
				pAlly = pAllyManager->find_ally( pInfo->get_owner_index() );
				if( pAlly )
				{

				}
				else
				{
					// 그런 방회 없다. 지워진 방회를 조회하여, 영웅단 인덱스로 고친다.
					// 만약, 지워진 방회에도 없다면, 몬스터로 소유를 바꾼다.
					// DB 로그로 남길 것.
					int OrgIndex = m_pDB->AllyGetMasterOrgInAllAlly( pInfo->get_owner_index() );
					if( OrgIndex && ( pOrg = pOrgManager->FindOrganization( OrgIndex ) ) )
					{
						pInfo->set_or_index( OrgIndex, 0 );
						pInfo->set_or_member_count( pOrg->m_usCurMemberNum );
						//pInfo->set_castle_level( 0 );
						//pInfo->set_cur_match_count( 0 );
						//pInfo->set_or_name( pOrg->m_cOrName );
						//pInfo->set_or_master_name( pOrg->m_cCaptainName );
						pInfo->set_or_master_moonpa( pOrg->m_cBaseParty );
						//pInfo->set_total_money(0);
						//pInfo->set_part_money(0);
						//pInfo->set_win_count(0);
						
						// 소유날짜, 스케줄은 바뀌지 않는다. insert(0)
						m_pDB->UpdateDefendCastleInfo( pInfo, 0 );
						
					}
					else
					{
						pInfo->change_owner_to_monster();

						int dfd = m_pDB->UpdateDefendCastleInfo( pInfo, 0 );
						pInfo->set_db_seq_index( dfd );
					}
				}
				break;
			}
		}
	}

}


CDSCastleInfo * CDSCastleManager::add_castle_index_only( short index, int db_row_index )
{
	// 인덱스가 0이거나, 이미 있는 정보라면 그냥 리턴한다.
	if( index == 0 || get_castle_info( index ) )
		return NULL;

	CDSCastleInfo * pInfo = new CDSCastleInfo;
	if( pInfo )
	{
		pInfo->set_map_index( index );
		pInfo->set_db_seq_index( db_row_index );
		m_Castles.push_back( pInfo );		// 벡터에 추가
	}

	return pInfo;
}

void CDSCastleManager::add_castle_info( short index, CDSCastleInfo& info )
{
	if( 0 == index )
		return;

	CDSCastleInfo * pInfo = get_castle_info( index );
	if( pInfo )
	{
		pInfo->set_from_info( info );
	}
}

void CDSCastleManager::proc_gs_req_manor_information( CMSG_CB_REQ_MANOR_INFORMATION * pReqManorInformation, _h_DS_Com * pDSCom )
{
	PrintConsole( "[COMBAT] Proc Req Manor Infor (%d)\n", (int)pReqManorInformation->ucMapID );
	CMSG_CB_MANOR_INFORMATION information;

	if( 0 == pReqManorInformation->ucMapID )
	{

		m_uiDefaultMatchHour = pReqManorInformation->DefaultSchedule[0].uiHour;	// 20080324 추가.

		if( 0 == m_Castles.size() )
		{
			information.ucIsEnd = 1;
			information.ucMapID = 0;

			pDSCom->Send( information.usLength, (char*)&information );
		}
		else
		{
			// 전체 정보를 차례대로 보내고, 마지막에는 end 임을 표시할 것.
			// pDSCom에 보낸다. 
			for( size_t i = 0 ; i < m_Castles.size() ; ++i )
			{
				m_Castles[i]->fill_packet_manor_information( information );
				information.set_length();

				if( i == m_Castles.size() - 1 )		// 맨 마지막 데이타라면, 
					information.ucIsEnd = 1;
				else
					information.ucIsEnd = 0;

#ifdef _PD_CASTLE_STEP_2_
				int count = information.ucCount;
				for( int k = 0; k < count ; ++k )
				{
					if( en_cb_group_type_ally == information.challengeList[k].ucType )
					{
						CAllyOrganization * pAlly = g_pAllyManager->find_ally( information.challengeList[k].iIndex );
						if( pAlly )
						{
							pAlly->send_ally_info( pDSCom, NULL );
						}
					}
				}
#endif

				pDSCom->Send( information.usLength, (char*)&information );
			}
		}

		PrintConsole( "  => Send All Manor Infor.\n" );
	}
	else 
	{
		// 정보가 있는지 우선 검사하고,
		// 없다면, DS에 추가하고, 정보를 보내다.
		// 그냥 pDSCom 에 보낸다. ( 모든 GS에 보내지 말고, 일단은 이렇게 하자. )
		information.ucIsEnd = 1;

		CDSCastleInfo * pCastleInfo = get_castle_info( (short)pReqManorInformation->ucMapID );
		if( pCastleInfo )
		{
			// 있는 정보 그대로, 보낸다.
			pCastleInfo->fill_packet_manor_information( information );

			pDSCom->Send( information.usLength, (char*)&information );

			PrintConsole( "  => Send %d Manor Infor.\n", (int)pReqManorInformation->ucMapID );
		}
		else
		{
			// DB 에 추가하는 코드. 
			int dfd = m_pDB->CastleInsert( (short)pReqManorInformation->ucMapID );

			if( dfd > 0 )
			{
				CDSCastleInfo * pInfo = add_castle_index_only( (short)pReqManorInformation->ucMapID, dfd );

				if( pInfo )
				{
					// Get Detail Info.
					m_pDB->FillCastleDetailInfo( (short)pReqManorInformation->ucMapID, dfd, pInfo );

					// Update Combat Time to Default Time
					pInfo->make_default_schedule( (int)pReqManorInformation->DefaultSchedule[0].uiHour, 
						(int)pReqManorInformation->DefaultSchedule[1].uiHour,
						(int)pReqManorInformation->DefaultSchedule[2].uiHour );

					m_pDB->UpdateDefendCastleInfo( pInfo, 1 );

					pInfo->fill_packet_manor_information( information );

					pDSCom->Send( information.usLength, (char*)&information );

					PrintConsole( "  => Make & Send %d Manor Infor.\n", (int)pReqManorInformation->ucMapID );
				}
				else
				{
					PrintConsole( "  => Make Fail : new %d Manor Infor.\n", (int)pReqManorInformation->ucMapID );
				}
			}
			else
			{
				// error : 나중에 처리.
				PrintConsole( "  => DB Fail : %d Manor Infor.\n", (int)pReqManorInformation->ucMapID );
			}
		}
	}
}


void CDSCastleManager::proc_gs_req_change_time( CMSG_CB_REQ_CHANGE_TIME * pReqChangeTime, _h_DS_Com * pDSCom )
{
	// 스케줄 변경 요청 : 시간의 적절함은 게임서버에서 검사했다. 업데이트만 할 것.
	CDSCastleInfo * pCastle = get_castle_info( static_cast<short>( pReqChangeTime->ucMapID ) );
	if( pCastle )
	{
#ifdef _PD_CASTLE_RENEWAL_1_
		pCastle->set_combat_date_from_time( &pReqChangeTime->Schedule[0] );
#else
		pCastle->set_combat_date_from_time( 0, &pReqChangeTime->Schedule[0] );
		pCastle->set_combat_date_from_time( 1, &pReqChangeTime->Schedule[1] );
		pCastle->set_combat_date_from_time( 2, &pReqChangeTime->Schedule[2] );
#endif

		m_pDB->UpdateDefendCastleInfo( pCastle, 1 );


		CMSG_CB_TIME_CHANGED dspkTimeChanged;
		dspkTimeChanged.ucMapID = static_cast<u_char>(pCastle->get_map_index());
		CopyMemory( &dspkTimeChanged.Schedule[0], &pReqChangeTime->Schedule[0], sizeof(_sND_TimeUnit)*3 );

		g_pDSComQueue->GlobalSend( dspkTimeChanged.usLength, (char*)&dspkTimeChanged );
	}
}


void CDSCastleManager::proc_gs_challenge_insert( CMSG_CB_CHALLENGE_INSERT * pReqChallengeInsert, _h_DS_Com * pDSCom )
{
	// 도전 정보 추가 작업.
	CDSCastleInfo * pCastle = get_castle_info( static_cast<short>( pReqChallengeInsert->ucMapID ) );
	if( pCastle )
	{
		int db_seq = m_pDB->InsertAttackCastle( static_cast<int>(pReqChallengeInsert->ucMapID), &pReqChallengeInsert->info );
		if( db_seq )
		{
			pReqChallengeInsert->info.iDBSeqNumber = db_seq;
            pCastle->add_challenge_list( &pReqChallengeInsert->info );
		}

		PrintConsole( "[CASTLE] %u Challenge Insert (db = %d )\n", pReqChallengeInsert->ucMapID, db_seq ); 

		g_pDSComQueue->GlobalSend( pReqChallengeInsert->usLength, (char*)pReqChallengeInsert );
	}
}


void CDSCastleManager::proc_gs_challenge_delete( CMSG_CB_CHALLENGE_DELETE * pReqChallengeDelete, _h_DS_Com * pDSCom )
{
	// 
	CDSCastleInfo * pCastle = get_castle_info( static_cast<short>( pReqChallengeDelete->ucMapID ) );
	if( pCastle )
	{
		int db_seq = pCastle->del_challenge( &pReqChallengeDelete->info );
		if( db_seq )
		{
			m_pDB->CompleteChallenge( db_seq );
		}

		PrintConsole( "[CASTLE] %u Challenge Complete (db = %d )\n", pReqChallengeDelete->ucMapID, db_seq ); 

		g_pDSComQueue->GlobalSend( pReqChallengeDelete->usLength, (char*)pReqChallengeDelete );
	}
}


void CDSCastleManager::proc_gs_challenge_reset( CMSG_CB_CHALLENGE_RESET * pChallengeReset )
{
	CDSCastleInfo * pCastle = get_castle_info( static_cast<short>( pChallengeReset->ucMapID ) );
	if( pCastle )
	{
		PrintConsole( "[CASTLE] %u Challenge Reset\n", pChallengeReset->ucMapID ); 

		pCastle->reset_challenge_list( m_pDB );
	}
}


void CDSCastleManager::proc_gs_owner_info_changed( CMSG_CB_OWNER_INFO_CHANGED * pOwnerChanged )
{
	CDSCastleInfo * pCastle = get_castle_info( static_cast<short>( pOwnerChanged->ucMapID ) );
	if( pCastle )
	{
		if( 0 == pOwnerChanged->ucChangeOwner )
		{
			// Update & Call DB Update(1). 
			pCastle->set_castle_level( pOwnerChanged->castleInfo.ucLevel );
#ifdef _PD_CASTLE_RENEWAL_1_
			pCastle->set_cur_match_count( pOwnerChanged->castleInfo.ucCurMatchCount_not );
#else
			pCastle->set_cur_match_count( pOwnerChanged->castleInfo.ucCurMatchCount );
#endif

			pCastle->set_win_count( pOwnerChanged->castleInfo.usWinCount );

#ifdef _PD_CASTLE_RENEWAL_1_
			pCastle->set_combat_date_from_time( &pOwnerChanged->castleInfo.timeNextSchedule );
#else
			pCastle->set_combat_date_from_time( 0, &pOwnerChanged->castleInfo.Schedule[0] );
			pCastle->set_combat_date_from_time( 1, &pOwnerChanged->castleInfo.Schedule[1] );
			pCastle->set_combat_date_from_time( 2, &pOwnerChanged->castleInfo.Schedule[2] );
#endif

			pCastle->reset_challenge_list( m_pDB );

			m_pDB->UpdateDefendCastleInfo( pCastle, 1 );		// update

			PrintConsole( "[CASTLE] <%d> Castle Update\n", pCastle->get_map_index() );

		}
		else
		{
			// Changed & Call DB Insert(0).
			pCastle->set_from_info( static_cast<short>(pOwnerChanged->ucMapID), &pOwnerChanged->castleInfo );

			pCastle->reset_challenge_list( m_pDB );

			pCastle->set_db_seq_index( m_pDB->UpdateDefendCastleInfo( pCastle, 0 ) );	// Insert

			PrintConsole( "[CASTLE] <%d> Castle Update&Insert(%d)\n", pCastle->get_map_index(), pCastle->get_db_seq_index() );
		}

		g_pDSComQueue->GlobalSend( pOwnerChanged->usLength, (char*)pOwnerChanged );

		pCastle->set_cur_state( 0 );
	}
}

void CDSCastleManager::proc_gs_combat_start( CMSG_CB_COMBAT_START * pCombatStart )
{
	CDSCastleInfo * pCastle = get_castle_info( static_cast<short>(pCombatStart->ucMapID) );
	if( pCastle )
	{
		pCastle->set_cur_state( 1 );

		PrintConsole( "[CASTLE] <%d> Castle Start\n", pCastle->get_map_index() );
	}
}


void CDSCastleManager::proc_gs_combat_end( CMSG_CB_COMBAT_END * pCombatEnd )
{
	CDSCastleInfo * pCastle = get_castle_info( static_cast<short>(pCombatEnd->ucMapID) );
	if( pCastle )
	{
		pCastle->set_cur_state( 0 );

		PrintConsole( "[CASTLE] <%d> Castle End\n", pCastle->get_map_index() );
	}
#ifdef _PD_CASTLE_STEP_2_
	// Tax 초기화 요청을 GS 에 보낸다.
	CMSG_MANOR_TAX_RESET_COMMON_TAX pkResetTax;
	g_pDSComQueue->GlobalSend( pkResetTax.usLength, (char*)&pkResetTax );

	CDSCastleManager::GetObj().GetRecord()->SetManorIndex(static_cast<short>(pCombatEnd->ucMapID));    	
#endif//_PD_CASTLE_STEP_2_
}

#ifdef	_PD_CASTLE_STEP_2_
void CDSCastleManager::proc_gs_req_record_info(CMSG_RECORD_INFORMATION_REQ* pReqRecordInfo,_h_DS_Com * pDSCom)
{
	PrintConsole( "[COMBAT] Proc Req Record Info (%d)\n", (int)pReqRecordInfo->m_org_index );
	CMSG_RECORD_INFORMATION_DS_GS information;
	MapOrgRecord * pMap = NULL;
	COrgRecord *pOrgRecord = NULL;
	pMap = CDSCastleManager::GetObj().GetRecord()->GetMapOrg();
	MapOrgRecord::iterator  itrBegin = pMap->begin();
	MapOrgRecord::iterator  itrEnd	 = pMap->end();
	int i= 0,record_count = 0 ;   


	if( 0 == pReqRecordInfo->m_org_index )
	{
		for( ;itrBegin!=itrEnd;++itrBegin )
		{		
			pOrgRecord = itrBegin->second;
			if(pOrgRecord)
			{
				ZeroMemory(information.m_combat_record,sizeof(_sCombatRecord)*MAX_MANOR);
				ZeroMemory(information.m_org_record,sizeof(_sOrgRecord)*MAX_ORG_COUNT);
				information.m_org_index = 0;
				information.m_combat_org_count = 0;

				CDSCastleManager::GetObj().GetRecord()->FillPacket(pOrgRecord->GetAllyIndex(),information);				
				information.m_org_index  = pOrgRecord->GetAllyIndex();	
				information.set_length();
				i++; //size
				//g_pSystemLog->LogFileWrite( "[Combat_Record]  send CombatRecord OrgIndex(%d) send Size(%d)- proc_gs_req_record_info - \n", pOrgRecord->GetAllyIndex(),information.usLength);
				pDSCom->Send( information.usLength, (char*)&information );
			}				
			else 
				g_pSystemLog->LogFileWrite( "[Combat_Record]  Don't send CombatRecord NULL COrgRecord* - proc_gs_req_record_info - \n"  );			
		}
		PrintConsole( "  => Send All Combat Record Info.\n" );
	}
}

void CDSCastleManager::proc_gs_update_record_info(CMSG_RECORD_UPDATE_GS_DS * pUpdateRecord)
{
	if(!pUpdateRecord)
	{
        g_pSystemLog->LogFileWrite( "[Combat_Record] ERROR Packet pUpdateRecord is NULL -proc_gs_update_record_info-\n");		
		return;
	}	
	COrgRecord* pRecord = CDSCastleManager::GetObj().GetRecord()->FindOrgRecord(pUpdateRecord->m_org_index);
	int againstOrg[100] = {0,};
    CDSCastleManager::GetObj().GetRecord()->UpdateRecord(*pUpdateRecord);
	g_pSystemLog->LogFileWrite( "[Combat_Record]  update End DB update start-proc_gs_update_record_info");				
	m_pDB->UpdateOrgRecordInManor(pUpdateRecord->m_org_index);
	m_pDB->UpdateOrgStandings_bi(pUpdateRecord->m_org_index);
	
	int count = pRecord->AllAgainstOrgIndex(againstOrg);
	for(int i=0;i<count;i++)
	{
		m_pDB->InsertOrgStandings(pUpdateRecord->m_org_index,againstOrg[i]);
	}
	g_pSystemLog->LogFileWrite( "[Combat_Record] SendLength(%d)  count(%d) -proc_gs_update_record_info-\n" ,pUpdateRecord->usLength,count);	
	g_pSystemLog->LogFileWrite( "[Combat_Record] Recv  OrgIndex(%d)- CMSG_RECORD_UPDATE_GS_DS-proc_gs_update_record_info (%d)  count(%d) -proc_gs_update_record_info-\n" ,pUpdateRecord->m_org_index);	
	g_pDSComQueue->GlobalSend( pUpdateRecord->usLength, (char*)pUpdateRecord );	
}

void CDSCastleManager::proc_gs_reset_record_info(CMSG_RECORD_RESET * pResetRecord)
{
    CRecordOfCombat* pRecord = CDSCastleManager::GetObj().GetRecord();
	if(!pResetRecord)
	{
		g_pSystemLog->LogFileWrite( "[Combat_Record] ERROR Packet pRecordRecord is NULL -proc_gs_reset_record_info-\n");		
		return;
	}
	
	CDSCastleInfo * pCastle = CDSCastleManager::GetObj().get_castle_info((short)pResetRecord->ucMapID);	
	CAllyOrganization *pAlly = NULL;
	int challenge[21] = {0,};
	int challengeCount(0),k(0),i(0),j(0),pos(0);
	int combating_index[105]={0,};
	_Organization * pOrg = NULL; 
	if(pCastle)
	{
		challengeCount = pCastle->fill_challenge_index(challenge);
		g_pSystemLog->LogFileWrite( "[Combat_Record] Record-Reset 1 challengeCount(%d) challenge 1(%d) 2(%d) 3(%d) 4(%d) 5(%d)\n",challengeCount,challenge[0],challenge[1],challenge[2],challenge[3],challenge[4]);
		if(challengeCount != 0)
		{
			if(challenge[challengeCount-1] != pCastle->get_owner_index())
				if(challenge[challengeCount] == 0 )
					challenge[challengeCount] = pCastle->get_owner_index();
				else 
					challenge[challengeCount+1] = pCastle->get_owner_index();
		}
		else if(challenge[challengeCount] == 0 )
			challenge[challengeCount] = pCastle->get_owner_index();		
		g_pSystemLog->LogFileWrite( "[Combat_Record] Record-Reset 1 challengeCount(%d) challenge 1(%d) 2(%d) 3(%d) 4(%d) 5(%d)\n",challengeCount,challenge[0],challenge[1],challenge[2],challenge[3],challenge[4]);
	}
						
	for(i=0;i<=challengeCount;++i)
	{
		pAlly = g_pAllyManager->find_ally(challenge[i]); // 회 찾아오기 							
		if(pAlly)
		{
			for( j = 0; j < _NDD_MAX_ORG_IN_ALLY_ ; ++j )
			{
				k=0;
					k = pAlly->get_org_index(j);
					if(k)
					{
						combating_index[pos] = k;
						++pos;					
					}									 
			}							
		}
		else //NULL 이면  회가 아니라 단이다 
		{
			pOrg = g_pOrganizationManager->FindOrganization(challenge[i]);
			if(pOrg)
			{
				if(pos!=0)
				{
					if(combating_index[pos-1] != pOrg->m_iIndex)
					{
						combating_index[pos] = pOrg->m_iIndex;
						++pos;
					}
				}
				else
				{
					combating_index[pos] = challenge[i];
					++pos;
				}
			}		
		}
	}
	pRecord->SetCombatingOrg(combating_index);
	pRecord->ResetWarRecord();
	g_pSystemLog->LogFileWrite( "[Combat_Record] Record-Reset 1 Index(%d) Send Length(%d)-proc_gs_reset_record_info-\n",combating_index[0],pResetRecord->usLength );
	g_pSystemLog->LogFileWrite( "[Combat_Record] Record-Reset 2 Index(%d) -proc_gs_reset_record_info-\n",combating_index[1]);
	g_pSystemLog->LogFileWrite( "[Combat_Record] Record-Reset 3 Index(%d) -proc_gs_reset_record_info-\n",combating_index[2]);
	g_pSystemLog->LogFileWrite( "[Combat_Record] Record-Reset 4 Index(%d) -proc_gs_reset_record_info-\n",combating_index[3]);
	g_pDSComQueue->GlobalSend( pResetRecord->usLength, (char*)pResetRecord );		
}
	
#endif//_PD_CASTLE_STEP_2_


void CDSCastleManager::notice_group_breakup( u_char group_type, int index )
{
	// 이 함수에서는 그룹이 가진 장원이 있는지 검사한 후
	// 있다면, 몬스터 소유로 초기화하고 각 게임서버에 알려준다.
	// 없다면, 할 일 없다. 
	CDSCastleInfo * pCastle = get_castle_by_owner( group_type, index );
	if( pCastle )
	{
		if( 0 == pCastle->get_cur_state() )		// 진행중이 아닐 경우에만 설정한다.
		{
			// 몬스터로 소유를 바꾸고, 
			pCastle->change_owner_to_monster();

			// 도전 목록은 변함이 없으며

			// 새로운 DB Sequence 를 설정하고, 
			pCastle->set_db_seq_index( m_pDB->UpdateDefendCastleInfo( pCastle, 0 ) );	// Insert

			// 패킷을 보낸다.
			CMSG_CB_OWNER_INFO_CHANGED pkChanged;
			pkChanged.ucChangeOwner = 1;
			pkChanged.ucMapID = static_cast<u_char>(pCastle->get_map_index());
			CopyMemory( &pkChanged.castleInfo, pCastle->get_castle_info(), sizeof(_sCastleInfo) );

			PrintConsole( "[CASTLE] <%d> Castle owner to monster\n", pCastle->get_map_index() );

			g_pDSComQueue->GlobalSend( pkChanged.usLength, (char*)&pkChanged );
		}
	}

	int db_seq;
	for( size_t i = 0 ; i < m_Castles.size() ; ++i )
	{
		db_seq = m_Castles[i]->del_challenge( index, group_type );
		if( db_seq )
			m_pDB->CompleteChallenge( db_seq );
	}
}

void CDSCastleManager::notice_server_down( int server_index )
{
	CDSCastleInfo * pCastle = get_castle_info( static_cast<short>(server_index) );
	if( pCastle )
	{
		pCastle->set_cur_state(0);
	}
}


#ifdef _PD_CASTLE_STEP_2_
//////////////////////////////////////////////////////////////////////////
void CDSCastleManager::tax_add_from_tax_system( short manor_index, u_int tax, u_int totol_money, bool bTemp /* = false */ )
{
	CDSCastleInfo * pCastle = get_castle_info( manor_index );
	if( pCastle && en_cb_group_type_monster != pCastle->get_owner_type() )
	{
		pCastle->tax_add( tax, totol_money, bTemp );
		if( tax > 0 )
			m_pDB->UpdateDefendCastleInfo( pCastle, 1 );
	}
}
#endif

//////////////////////////////////////////////////////////////////////////

void CDSCastleManager::print_castle_list()
{
	vector<CDSCastleInfo*>::iterator itr = m_Castles.begin();

	while( itr != m_Castles.end() )
	{
		(*itr)->print_info();
		++itr;
	}
}

void CDSCastleManager::print_castle_info( short map_id )
{
	if( 0 == map_id )
	{
		// 전체 장원 정보를 보여준다.
		print_castle_list();
	}
	else
	{
		// 특정 장원 정보를 보여준다.
		CDSCastleInfo * pInfo = get_castle_info( map_id );
		if( pInfo )
		{
			pInfo->print_info();
		}
		else
		{
			printf( "\nNot found %d Castle\n", map_id );
		}
	}
}

void CDSCastleManager::print_castle_by_db_seq( int seq )
{
	CDSCastleInfo * pInfo = get_castle_info_by_db_seq( seq );
	if( pInfo )
	{
		pInfo->print_info();
	}
	else
	{
		printf( "\nNot found %d Castle(db seq)\n", seq );
	}
}

#ifdef	 _PD_CASTLE_STEP_2_
void	CDSCastleManager::SetRecord()	
{
	m_Record = new CRecordOfCombat;
	m_Record->Create();
}
#endif

void CDSCastleManager::cmd_change_owner_to_monster( int castle_index )
{
	CDSCastleInfo * pCastle = get_castle_info( castle_index );
	if( pCastle )
	{
		if( 0 == pCastle->get_cur_state() )		// 진행중이 아닐 경우에만 설정한다.
		{
			// 몬스터로 소유를 바꾸고, 
			pCastle->change_owner_to_monster();

			// 도전 목록은 변함이 없으며

			// 새로운 DB Sequence 를 설정하고, 
			pCastle->set_db_seq_index( m_pDB->UpdateDefendCastleInfo( pCastle, 0 ) );	// Insert

			// 패킷을 보낸다.
			CMSG_CB_OWNER_INFO_CHANGED pkChanged;
			pkChanged.ucChangeOwner = 1;
			pkChanged.ucMapID = static_cast<u_char>(pCastle->get_map_index());
			CopyMemory( &pkChanged.castleInfo, pCastle->get_castle_info(), sizeof(_sCastleInfo) );

			PrintConsole( "[CASTLE] <%d> Castle owner to monster\n", pCastle->get_map_index() );

			g_pDSComQueue->GlobalSend( pkChanged.usLength, (char*)&pkChanged );
		}
	}
}

//////////////////////////////////////////////////////////////////////////


#endif	// _PD_CASTLE_STEP_1_