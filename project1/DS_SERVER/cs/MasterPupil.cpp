#include "../stdafx.h"

#include "..\Network\Network.h"
#include "../Network/LogSvrLink.h"

#include "..\MapManager\MAPManager.h"

#include "MasterPupil.h"


extern _j_ServerInfo *		g_pServerInfo ;
extern _DSCom_Queue *		g_pDSComQueue ;


extern CLogSvr_Com *		g_pLogSvrCom;

extern _j_AccMAPManager *	g_pAccMapManager;


#ifdef _PD_MASTER_PUPIL_


extern CDSMasterSet *			g_pMPSet;


u_int CDSMasterSet::m_uiBaseKillCount = 20;
u_int CDSMasterSet::m_uiExtendKillCount = 80;
u_int CDSMasterSet::m_uiMaxMasterTotalPoint = 500000;
u_int CDSMasterSet::m_uiMaxPupilTotalPoint = 100000;



void CMP_PupilPosRecalc::push_point( u_int point, u_char rank, u_char arr_idx )
{
	point = point + rank*500;
	if( 0 == m_count )
	{
		m_pupil[0].point = point;
		m_pupil[0].rank = rank;
		m_pupil[0].arr_idx = arr_idx;

		++m_count;
	}
	else if( m_count < 4 )
	{
		int i;
		for( i = m_count-1 ; i >= 0 ; --i )
		{
			if( point > m_pupil[i].point || 
				( point == m_pupil[i].point && rank < m_pupil[i].rank ) )
			{
				CopyMemory( &m_pupil[i+1], &m_pupil[i], sizeof(__temp_mp_rank_calc) );
				m_pupil[i].point = point;
				m_pupil[i].rank = rank;
				m_pupil[i].arr_idx = arr_idx;
			}
			else
			{
				m_pupil[i+1].point = point;
				m_pupil[i+1].rank = rank;
				m_pupil[i+1].arr_idx = arr_idx;
				break;
			}
		}
		++m_count;
	}
}

void CMP_PupilPosRecalc::recalc_pos()
{
	/*
	// 모든 값이 0 으로 초기화 되어 있으므로, 변하는 값만 대입.
	int cur_pos = ::GetMaxPupilRank( m_pupil[0].point );
	if( 2 == cur_pos )	// 첫째 제자는 2,  둘째부터 다시 계산.
	{
		m_pupil[0].position = 2;

		if( count > 1 )
		{
			cur_pos = ::GetMaxPupilRank( m_pupil[1].point );
			if( cur_pos > 0 )
				m_pupil[1].position = 1;
		}
	}
	else if( 1 == cur_pos )	// 첫번 제자만 1, 나머지는 0.
	{
		m_pupil[0].position = 1;
	}
	*/
}


//////////////////////////////////////////////////////////////////////////
//
//	CDSMaster : 사제관계 클래스.
//
//////////////////////////////////////////////////////////////////////////
void CDSMaster::AddPupilFromDB( _sCSPK_Pupil& pupil )
{
	int slot = 0;
	for( ; slot < NDD_MP_MAX_PUPIL_COUNT; ++slot )
	{
		if( 0 == m_Pupil[slot].iPupilIndex )
			break;
	}

	if( NDD_MP_MAX_PUPIL_COUNT > slot )
	{
		// Pupil 정보 삽입.
		m_Pupil[slot].Reset();
		m_Pupil[slot].SetInfo( pupil );

	}
}


void CDSMaster::ShowInfo()
{
	printf( "  Master <%s> - <%d>\n", GetMasterName(), GetIndex() );
	int count = GetPupilCount();
	for( int i = 0 ; i < count; ++i )
		printf( "    Pupil[%d] : C<%s> - <%d>\n\n", m_Pupil[i].cPupilName, m_Pupil[i].iPupilIndex );
}


void CDSMaster::InitByDefault( char * master_name )
{
	m_uiTotalKillCount = 0;
	m_uiMasterKillCount = 0;
	m_uiMasterTotalPoint = 0;		// 스승 포인트. ( 소진에 영향을 받지 않는 누적 수치. )
	m_uiMasterCurPoint = 0;			// 소진시 사용되는 포인트. 

	m_ucMasterRank = 1;
	m_ucMasterSex = 0;
	m_ucMasterMoonpa = 0;
	m_ucMasterClass = 0;			// 
	m_ucMasterGrade = 0;			// 마스터의 클래스 그레이드
	m_ucMasterLevel = 0;			// 마스터의 레벨
    
	m_ucBonusSkillStep = 0;		// 스승의 레벨에 따라, 제자들이 받는 보너스 스킬 스텝.

	m_bApplyMasterEffect = false;		// 사제 관계 혜택이 적용되고 있는가.

	ZeroMemory( m_cMasterName, en_charac_name_length+1 );
	strncpy( m_cMasterName, master_name, en_charac_name_length );
	
	m_ucMasterNickLength = 0;		// 사호의 길이( byte )
	ZeroMemory( m_cMasterNick, NDD_MP_MASTERNICK_LENGTH+1 );

	ZeroMemory( m_Pupil, sizeof(m_Pupil) );		// 제자 정보.

	CND_TimeUnitFunc::GetTime_Current( m_timeCreateDate );
	CND_TimeUnitFunc::GetTime_Current( m_timeLastCon );
}


u_char CDSMaster::CanAddNewPupil( char * p_name )
{
	u_char ret = en_mp_apply_no_error;
	if( IsFullPupil() )
		ret = en_mp_apply_error_pupils_over;
	else
	{
		if( false == AddReservePupil( p_name ) )
			ret = en_mp_apply_error_wait;
	}

	return ret;
}

bool CDSMaster::AddReservePupil( char * p_name )
{
	// 현재 시간 
	time_t cur;
	bool ret = false;
	time( &cur );

	for( int i = 0; i < NDD_MP_MAX_RESERVE_PUPIL; ++i )
	{
		if( 0 == m_ReserveApplyPupil[i].dwApplyTime ||
			m_ReserveApplyPupil[i].dwApplyTime + NDD_MP_RESERVE_WAIT_TIME < static_cast<DWORD>(cur) )
		{
			// 시간으로 빈공간을 찾아, 이름을 백업하고 리턴.
			m_ReserveApplyPupil[i].dwApplyTime = static_cast<DWORD>(cur);
			strncpy( m_ReserveApplyPupil[i].cApplyName, p_name, en_charac_name_length );
			m_ReserveApplyPupil[i].cApplyName[en_charac_name_length] = 0;
			ret = true;
			break;
		}
	}

	return ret;
}


bool CDSMaster::CheckNewPupil( char * p_name )
{
	time_t cur;
	bool ret = false;
	time( &cur );

	for( int i = 0; i < NDD_MP_MAX_RESERVE_PUPIL; ++i )
	{
		if( p_name[0] == m_ReserveApplyPupil[i].cApplyName[0] &&
			0 == strncmp( p_name, m_ReserveApplyPupil[i].cApplyName, en_charac_name_length ) )
		{
			if( m_ReserveApplyPupil[i].dwApplyTime + NDD_MP_RESERVE_WAIT_TIME >= static_cast<DWORD>(cur) )
			{
				m_ReserveApplyPupil[i].dwApplyTime = 0;
				ret = true;
				break;
			}
		}
	}

	return ret;
}


void CDSMaster::SendMPInfo_Again( char * to_name, u_char zone )
{
	CMSG_MP_INFO_MASTER pkInfo;
	pkInfo.SetFromName( to_name );

	FillPacket_MyMasterInfo( &pkInfo );

	g_pServerInfo->send_to_gs( &pkInfo, pkInfo.usLength, static_cast<short>(zone) );
}

void CDSMaster::FillPacket_MyMasterInfo( CMSG_MP_INFO_MASTER * pInfo )
{
	pInfo->iMasterIndex = GetIndex();			// 
	pInfo->uiTotalKillCount = GetTotalKillCount();
	pInfo->uiMasterKillCount = GetMasterKillCount();
	pInfo->uiMasterTotalPoint = GetTotalPoint();
	pInfo->uiMasterCurPoint = GetCurPoint();

	pInfo->ucMasterRank = GetMasterRank();

	pInfo->ucMasterSex = GetMasterSex();
	pInfo->ucMasterInnerLevel = GetMasterLevel();
	pInfo->ucMasterMoonpa = GetMasterMoonpa();
	pInfo->ucMasterClass = GetMasterClass();
	pInfo->ucMasterClassgrade = GetMasterClassGrade();

	pInfo->ucMasterConZone = GetMasterCurZone();
	pInfo->ucMasterConnected = GetMasterConnectState();
	pInfo->ucBonusSkillStep = GetBonusSkillStep();

	pInfo->ucApplyMasterEffect = GetApplyEffect();
	pInfo->SetMasterName( GetMasterName() );
	pInfo->SetMasterNick( GetMasterNick() );

	CopyMemory( &pInfo->Pupils[0], &m_Pupil[0], sizeof(_sCSPK_Pupil)*NDD_MP_MAX_PUPIL_COUNT );
}

void CDSMaster::FillPacket_StrikeResult( CMSG_MP_STRIKE_RESULT * pResult )
{
	//pResult->iMasterIndex = GetIndex();
	//pResult->iPupilIndex = 

	pResult->uiMasterTotalPoint = m_uiMasterTotalPoint;
	pResult->uiMasterCurPoint = m_uiMasterCurPoint;
	pResult->ucBonusSkillStep = m_ucBonusSkillStep;

	ZeroMemory( pResult->ChangePosition, sizeof(pResult->ChangePosition) );
	int j = 0;
	for( int i = 0; i < NDD_MP_MAX_PUPIL_COUNT; ++i )
	{
		if(m_Pupil[i].iPupilIndex )
		{
			pResult->ChangePosition[j].iPupilIndex = m_Pupil[i].iPupilIndex;
			pResult->ChangePosition[j].ucNewPos= m_Pupil[i].ucPupilPosition;
			++j;
		}
	}
}


bool CDSMaster::ConnectNotice( int cuid, u_char state, u_char zone )
{
	bool ret = false;

	// 서버 이동 중에 발생한, 변경 정보를 다시 보내줘야 한다.
	if( cuid == GetIndex() )
	{
		if( 1 == state && 2 == m_ucMasterConnectState )
		{
			SendMPInfo_Again( GetMasterName(), zone );
		}
		m_ucMasterConnectState = state;
		m_ucMasterZone = zone;
		ret = true;
	}
	else
	{
		for( int i = 0; i < NDD_MP_MAX_PUPIL_COUNT; ++i )
		{
			if( cuid == m_Pupil[i].iPupilIndex )
			{
				if( 1 == state && 2 == m_Pupil[i].ucConnectState )
				{
					SendMPInfo_Again( m_Pupil[i].cPupilName, zone );		// 
				}
				m_Pupil[i].ucConnectState = state;
				m_Pupil[i].ucZone = zone;
				ret = true;
				break;
			}
		}
	}
    
	return ret;
}



void CDSMaster::ChangeMasterNick( char * nick )
{
	// 모든 서버에 보낸다. : 우선 한 곳에만.
	SetMasterNick( nick );

	CMSG_MP_UPDATE_MASTER_NICK pkUpdateNick;
	pkUpdateNick.iMasterIndex = GetIndex();
	pkUpdateNick.SetNick( GetMasterNick() );

	PrintConsole( "[MP] MP(%d) Update Master Nick(%s)\n", GetIndex(), nick );

	SendToAll( (char*)&pkUpdateNick, pkUpdateNick.usLength );
}


bool CDSMaster::AddNewPupil( _sCSPK_Pupil& pupil )
{
	bool ret = false;
	if( false == IsFullPupil() )
	{
		for( int i = 0; i < NDD_MP_MAX_PUPIL_COUNT ; ++i )
		{
			if( 0 == m_Pupil[i].iPupilIndex )
			{
				// 초기화 먼저하고 세팅.
				m_Pupil[i].SetInfo( pupil );
				ret = true;
				break;
			}
		}
	}

	return ret;
}



void CDSMaster::SendToAll( char * data, u_short length )
{
	short zones[NDD_MP_MAX_PUPIL_COUNT+1] = {0,0,0,0,0 };			// 5개면 된다.
	short add_z = 0;
	int zc = 0;
	int z = 0;

	if( 1 == m_ucMasterConnectState )
		zones[z++] = static_cast<short>(m_ucMasterZone);

	for( int i = 0; i < NDD_MP_MAX_PUPIL_COUNT; ++i )
	{
		if( 1 == m_Pupil[i].ucConnectState )
		{
			add_z = static_cast<short>(m_Pupil[i].ucZone);

			for( zc = 0; zc < z ; ++zc )
			{
				if( add_z == zones[zc] )
					break;
			}
			// 없다면 추가.
			if( zc == z )
			{
				zones[z++] = add_z;
			}
		}

	}

	if( z > 0 )
	{
		PrintConsole( "[MP] Send GS(%d) : %d, %d, %d, %d, %d\n", z, zones[0], zones[1], zones[2], zones[3], zones[4] );
		g_pServerInfo->send_to_some_gs( data, length, zones, z );
	}
}


u_char CDSMaster::StrikeReq( char * pupil, COLE_DB_Executer * pGameBD )
{
	// error code
	// 0 - not found master info.
	// 1 - pupil is not member.
	// 2 - progressing other strike
	// 3 - minimum 72 hours.
	
	u_char code = 0;

	int index = GetPupilArrIndex( pupil );
	if( -1 != index )
	{
		// 있으면.
		// 시간을 검사한다.
		if( m_Pupil[index].CanBeStruck() )
		{
			if( pGameBD->MP_BackupPupil( m_Pupil[index].iPupilIndex, 1 ) )
			{
				PCHARAC pTarget = g_pAccMapManager->Find_Charac( pupil );	
				if( pTarget )
				{
					pTarget->CHARAC_MP.ucMPPosition = en_mp_relation_none;
					pTarget->CHARAC_MP.ucRes = 0;
					pTarget->CHARAC_MP.usRes = 0;
					pTarget->CHARAC_MP.iMasterIndex = 0;
				}

			
				CMSG_MP_STRIKE_RESULT pkResult;

				pkResult.iMasterIndex = GetIndex();
				pkResult.iPupilIndex = m_Pupil[index].iPupilIndex;

				pkResult.uiMasterTotalPoint = SubTotalPoint( m_Pupil[index].uiCurPoint / 2 );		// 기여 포인트의 50% 삭감 
				pkResult.uiMasterCurPoint = m_uiMasterCurPoint;
				pkResult.ucBonusSkillStep = GetBonusSkillStep();

				m_Pupil[index].Reset();

				if( GetPupilCount() > 0 )
					RecalcPosition( &pkResult );

				PrintConsole( "[MP] Send Strike result : M<%d> - P<%d>\n", pkResult.iMasterIndex, pkResult.iPupilIndex );

				SendToAll( (char*)&pkResult, pkResult.usLength );
			}
			else 
				code = 4;
		}
		else
		{
			code = 3;
		}
	}
	else
	{
		code = 1;
	}

	return code;
}


u_char CDSMaster::LeaveReq( char * pupil, COLE_DB_Executer * pGameBD )
{
	u_char code = 0;

	int index = GetPupilArrIndex( pupil );
	if( -1 != index )
	{
		// 있으면.
		// 시간을 검사한다.
		if( m_Pupil[index].CanBeStruck() )
		{
			if( pGameBD->MP_BackupPupil( m_Pupil[index].iPupilIndex, 3 ) )
			{
				PCHARAC pTarget = g_pAccMapManager->Find_Charac( pupil );	
				if( pTarget )
				{
					pTarget->CHARAC_MP.ucMPPosition = en_mp_relation_none;
					pTarget->CHARAC_MP.ucRes = 0;
					pTarget->CHARAC_MP.usRes = 0;
					pTarget->CHARAC_MP.iMasterIndex = 0;
				}

				CMSG_MP_LEFT_PUPIL pkResult;

				pkResult.iMasterIndex = GetIndex();
				pkResult.iPupilIndex = m_Pupil[index].iPupilIndex;

				pkResult.uiMasterTotalPoint = SubTotalPoint( m_Pupil[index].uiCurPoint / 2 );		// 기여 포인트의 50% 삭감 
				pkResult.uiMasterCurPoint = m_uiMasterCurPoint;
				pkResult.ucBonusSkillStep = GetBonusSkillStep();

				m_Pupil[index].Reset();

				if( GetPupilCount() > 0 )
					RecalcPosition( &pkResult );

				PrintConsole( "[MP] Send Left result : M<%d> - P<%d>\n", pkResult.iMasterIndex, pkResult.iPupilIndex );

				SendToAll( (char*)&pkResult, pkResult.usLength );
			}
			else
				code = 4;
		}
		else
		{
			code = 3;
		}
	}
	else
	{
		code = 1;
	}


	return code;
}


void CDSMaster::RecalcPosition( CMSG_MP_STRIKE_RESULT * pResult )
{
    u_char prev_pos[NDD_MP_MAX_PUPIL_COUNT] = {0, };	// 미리 기억해 놓는다
	// 현재의 최고 제장 등급.
	int max_seq = -1;
	u_int max_point=0;
	
	int i = 0;
	u_char max_position = 0;
	for( ; i < NDD_MP_MAX_PUPIL_COUNT; ++i )
	{
		prev_pos[i] = m_Pupil[i].ucPupilPosition;		// 미리 기억.
		if( max_position < prev_pos[i] )
			max_position = prev_pos[i];

		if( max_point < m_Pupil[i].uiToMasterPoint )
			max_point = m_Pupil[i].uiToMasterPoint;
	}

	// max_pos 가 0 이면, 최고 포인트를 가진 
}



void CDSMaster::AddPoint( int cuid, int value )
{
	int pupil_slot = GetPupilSlotNumber( cuid );
	if( pupil_slot >= 0 )
	{
		if( 0 != GetMasterConnectState() )
		{
			// 스승의 전체 포인트를 올린다.
			if( m_uiMasterTotalPoint < CDSMasterSet::GetMaxMasterTotalPoint() )
			{
				if( CDSMasterSet::GetMaxMasterTotalPoint() - m_uiMasterTotalPoint < static_cast<u_int>(value) )
				{
					m_uiMasterTotalPoint = CDSMasterSet::GetMaxMasterTotalPoint();
				}
				else
				{
					m_uiMasterTotalPoint += static_cast<u_int>(value);
				}
			}

			// 스승의 현재 포인트를 올린다.
			if( m_uiMasterCurPoint < CDSMasterSet::GetMaxMasterTotalPoint() )
			{
				if( CDSMasterSet::GetMaxMasterTotalPoint() - m_uiMasterCurPoint < static_cast<u_int>(value) )
				{
					m_uiMasterCurPoint = CDSMasterSet::GetMaxMasterTotalPoint();
				}
				else
				{
					m_uiMasterCurPoint += static_cast<u_int>(value);
				}
			}

			// 제자의 전달 포인트를 올린다.
			if( m_Pupil[pupil_slot].uiToMasterPoint < CDSMasterSet::GetMaxPupilTotalPoint() )
			{
				if( CDSMasterSet::GetMaxPupilTotalPoint() - m_Pupil[pupil_slot].uiToMasterPoint < static_cast<u_int>(value) )
				{
					m_Pupil[pupil_slot].uiToMasterPoint = CDSMasterSet::GetMaxPupilTotalPoint();
				}
				else
				{
					m_Pupil[pupil_slot].uiToMasterPoint += static_cast<u_int>(value);
				}
			}
		}

		// 제자의 현재 포인트를 올린다.
		// 제자의 전달 포인트를 올린다.
		if( m_Pupil[pupil_slot].uiCurPoint < CDSMasterSet::GetMaxPupilTotalPoint() )
		{
			if( CDSMasterSet::GetMaxPupilTotalPoint() - m_Pupil[pupil_slot].uiCurPoint < static_cast<u_int>(value) )
			{
				m_Pupil[pupil_slot].uiCurPoint = CDSMasterSet::GetMaxPupilTotalPoint();
			}
			else
			{
				m_Pupil[pupil_slot].uiCurPoint += static_cast<u_int>(value);
			}
		}

		CMSG_MP_UPDATE_POINT pkUpdate;
		pkUpdate.iMasterIndex = GetIndex();
		pkUpdate.uiMasterTotalPoint = m_uiMasterTotalPoint;
		pkUpdate.uiMasterCurPoint = m_uiMasterCurPoint;
		pkUpdate.iFromCharac = cuid;
		pkUpdate.uiPupilCurPoint = m_Pupil[pupil_slot].uiCurPoint;

		SendToAll( (char*)&pkUpdate, pkUpdate.usLength );

	}
	// slot 에 없다면 이상한 것이다.
		
}


void CDSMaster::AddKillCount( int cuid, int value )		// 여기서, value 는 항상 1이다. 그래서, 위처럼 철저히 검사하지 않는다.
{
	// 킬 카운트 도달하는지 봐야 한다.
	m_uiTotalKillCount += static_cast<u_int>(value);

	if( m_uiTotalKillCount > CDSMasterSet::GetExtendKillCount() )
	{
		// Event 발생.
		m_uiTotalKillCount = 0;
	}

	CMSG_MP_UPDATE_KILL_COUNT pkCount;
	pkCount.iMasterIndex = GetIndex();
	pkCount.uiTotalKillCount = m_uiTotalKillCount;

	pkCount.iFromCharac = cuid;
	pkCount.uiValue = m_uiTotalKillCount;

	SendToAll( (char*)&pkCount, pkCount.usLength );
}


void CDSMaster::RecalcPosition( CMSG_MP_LEFT_PUPIL * pResult )
{
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
NDQueueMemoryPool<__master_board_unit>	* __master_board_page::m_pBoardPool = NULL;

bool __master_board_page::AddBoard( CDSMaster * p )
{
	// 리턴값은, 삽입 성공/실패.
	_node<__master_board_unit> * n = m_pBoardPool->get_free_node();
	if( n )
	{
		// 리스트 삽입. 삽입 위치를 먼저 결정한다.
		n->node.Set( p );

		if( false == m_pParent->InsertNodeMap( p->GetIndex(), n ) )
		{
			m_pBoardPool->add_free_node( n );
			return false;
		}

		if( 0 == GetCount() )
		{
			n->prev = NULL;
			n->next = NULL;
			m_pFirst = n;
			++m_uiCount;
		}
		else 
		{
			// 맨 처음것보다 높다면, 그 앞에 넣는다.
			if( m_pFirst->node.Compare( p->GetTotalPoint(), p->GetMasterRank() ) < 0 )
			{
				m_pFirst->prev = n;
				n->next = m_pFirst;
				n->prev = NULL;
				m_pFirst = n;

				++m_uiCount;
			}
			else
			{
				// 

				u_int count = 1;
				int com = 0;
				_node<__master_board_unit> * pCur = m_pFirst->next;
				_node<__master_board_unit> * pPrev = m_pFirst;

				while( pCur && count < GetCount() )
				{
					com = pCur->node.Compare( p->GetTotalPoint(), p->GetMasterRank() );
					if( com >= 0 )		// 같거나, 랭크가 작은 것은 뒤로.
					{
						pPrev = pCur;
						pCur = pCur->next;
						++count;
					}
					else
						break;
				}

				// pPrev 다음에 넣는다. pPrev 는 NULL 일 수 없다.
				if( pPrev->next )
					pPrev->next->prev = n;
				n->prev = pPrev;
				n->next = pPrev->next;
				pPrev->next = n;

				++m_uiCount;
			}
		}

		n->node.SetParent( this );
	}

	return ( NULL != n );
}


bool __master_board_page::CreateBoardUnit( int count )
{
	bool ret = true;

	if( NULL == m_pBoardPool )
	{
		m_pBoardPool = new NDQueueMemoryPool<__master_board_unit>(count);
		if( NULL == m_pBoardPool )
			ret = false;
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////////
// _MPBoardList 구현부
//////////////////////////////////////////////////////////////////////////

bool _MPBoardList::CreateBoardList( int prop, int page_res, int board_res )
{
	bool ret = false;

	m_ListProperty = prop;

	ret = __master_board_page::CreateBoardUnit( board_res );
	if( ret )
	{
		// Page Reserve.
		__master_board_page page;
		page.m_uiCount = 0;
		page.m_pFirst = NULL;
		page.m_pParent = this;

		for( int i = 0; i < page_res; ++i )
		{
			page.SetPage(i);
			m_pages.push_back(page);
		}

		ret = true;
	}

	if( false == ret )
	{
		printf( "[INIT FAIL] CreateBoardList Failed.\n" );
	}

	return ret;
}

void _MPBoardList::AddMPBoard( CDSMaster * pMaster )
{
	if( !pMaster )
		return;

	if( IsFull() )
		return;

	// 이미 있다면, 무시하고 넘어간다.
	if( m_nodeMap.end() != m_nodeMap.find( pMaster->GetIndex() ) )
		return;

	// 처음 들어가는 거라면, 페이지를 삽입하고, 그 페이지에 추가한 후 리턴.
	// 중간 페이지부터 검색한다.
	// 비교값이 - 이면, 앞으로, + 이면 뒤로 가는데, 첫 검색 페이지의 비교값의 부호를 기억한다.
	// 부호값이 바뀌거나, 유효 페이지를 넘어가는지 검사하면서 반복.
	// 삽입한다.

	int total_page = m_iTotalPage;
	if( 0 == total_page )
	{
		// 페이지를 추가하고, 처음에 삽입한다.
		__master_board_page * pPage = InsertPage();
		if( pPage )
		{
			if( pPage->AddBoard( pMaster ) )
				IncreaseCount();
		}
	}
	else
	{
		int start_page = total_page/2;
		int com = 0;
		bool prev_sign = false;		// true = +, false = -.

		com = m_pages[start_page].Compare( pMaster->GetTotalPoint(), pMaster->GetMasterRank() );
		if( com < 0 )
		{
			prev_sign = false;
			--start_page;
		}
		else
		{
			prev_sign = true;
			++start_page;
		}

		while( start_page >= 0 && start_page < total_page )
		{
			com = m_pages[start_page].Compare( pMaster->GetTotalPoint(), pMaster->GetMasterRank() );
			if( com < 0 )
			{
				if( prev_sign )		// 부호가 바뀌었다면 중지.
					break;
				--start_page;
			}
			else
			{
				if( false == prev_sign )
				{
					--start_page;	// 뒤로 갈 때, 부호가 바뀌면, 바로 앞 페이지에 삽입한다.
					break;
				}

				++start_page;
			}
		}
		// 최종 수정.
		if( start_page < 0 )
			start_page = 0;
		if( start_page >= total_page )
			start_page = total_page - 1;

		// 현재 페이지에 삽입하고, 뒤를 정리한다.
		if( m_pages[start_page].AddBoard( pMaster ) )
			IncreaseCount();

		if( m_pages[start_page].GetCount() > NDD_MP_MAX_ITEMCOUNT_PER_BOARD )
			ArrangePage( start_page, true );		// Insert로 호출한다.
	}
}

void _MPBoardList::DelMPBoard( int master_index )
{
	TPNodeMap::iterator itr = m_nodeMap.find( master_index );
	if( itr != m_nodeMap.end() )
	{
		// 찾았다. pParentPage 를 구하여, 개수를 줄인다.
		_node<__master_board_unit> * pNode = itr->second;
		__master_board_page * pPage = pNode->node.GetParentPage();

		if( pNode->prev )
			pNode->prev->next = pNode->next;
		if( pNode->next )
			pNode->next->prev = pNode->prev;

		if( pPage->m_pFirst == pNode )
		{
			// 처음 것이라면, 삭제후 수정.
			pPage->m_pFirst = pNode->next;
		}

		pPage->FreeNode( pNode );
		pPage->DecreaseCount();
		DecreaseCount();

		if( pPage->GetCount() < NDD_MP_MIN_ITEMCOUNT_PER_BOARD )
			ArrangePage( pPage->GetPage(), false );

		m_nodeMap.erase( itr );
	}
}


void _MPBoardList::UpdateMPBoardNick( int master_index, char * nick )
{
	TPNodeMap::iterator itr = m_nodeMap.find( master_index );
	if( itr != m_nodeMap.end() )
	{
		// 찾았다. pParentPage 를 구하여, 개수를 줄인다.
		_node<__master_board_unit> * pNode = itr->second;

		ZeroMemory( &pNode->node.BoardInfo.cMasterNick[0], NDD_MP_MASTERNICK_LENGTH+1 );
		if( nick )
		{
			strncmp( &pNode->node.BoardInfo.cMasterNick[0], nick, NDD_MP_MASTERNICK_LENGTH );
		}
	}
}


__master_board_page * _MPBoardList::InsertPage()
{
	__master_board_page * p = NULL;
	if( m_iTotalPage < static_cast<int>(m_pages.size()) )
	{
		p = &m_pages[m_iTotalPage];
		p->SetPage(m_iTotalPage);

		++m_iTotalPage;
	}
	return p;	
}

void _MPBoardList::ArrangePage( int start_page, bool Insert_Delete )
{
	if( Insert_Delete )
	{
		// Insert
		int move_count = 0;
		u_int next_page = start_page;

		__master_board_page * pPage = NULL;
		_node<__master_board_unit> * pUnit = NULL;

		pPage = &m_pages[start_page]; 
		pPage->SetCount(0);
		pUnit = m_pages[start_page].m_pFirst; 

		while( pUnit )
		{
			if( move_count > 0 && 0 == ( move_count%NDD_MP_AVR_ITEMCOUNT_PER_BOARD ) )
			{
				next_page = start_page+1;
				if( next_page < m_pages.size() )
				{
					m_pages[next_page].m_pFirst = pUnit;
					pPage = &m_pages[next_page];
					pPage->SetPage( next_page );
					pPage->SetCount(0);
					++start_page;
				}
				else
				{
					pUnit->node.SetParent(pPage);
					break;
				}
			}

			pPage->IncreaseCount();

			pUnit->node.SetParent( pPage );

			pUnit = pUnit->next;
			++move_count;
		}

		m_iTotalPage = next_page+1;

	}
	else
	{
		// Delete
		// 개수가 0이면, 그냥 지운다.
		// 아니면, 위랑 똑같다.
		int move_count = 1;
		u_int next_page = start_page;

		__master_board_page * pPage = NULL;
		_node<__master_board_unit> * pUnit = NULL;

		pPage = &m_pages[start_page]; 
		if( 0 == pPage->GetCount() )
		{
			pPage->Reset();
			m_iTotalPage = start_page;
			return;
		}

		pPage->SetCount(1);
		pUnit = m_pages[start_page].m_pFirst->next; 

		while( pUnit )
		{
			pPage->IncreaseCount();
			if( 0 == ( move_count%NDD_MP_AVR_ITEMCOUNT_PER_BOARD ) )
			{
				next_page = start_page+1;
				if( next_page < m_pages.size() )
				{
					m_pages[next_page].m_pFirst = pUnit;
					pPage = &m_pages[next_page];
					pPage->SetPage( next_page );
					++start_page;
				}
				else
				{
					pUnit->node.SetParent(pPage);
					break;
				}
			}

			pUnit->node.SetParent( pPage );

			pUnit = pUnit->next;
			++move_count;
		}

		m_iTotalPage = next_page+1;

	}
}


bool _MPBoardList::GetPageInfo( int page, CMSG_MP_BOARD_PAGE_INFO * pInfo )
{
	pInfo->ucCount = 0;

	if( page >= GetPageCount() || page < 0 )
	{
		return false;
	}
	else
	{
		// 내용을 넣는다.
		__master_board_page * pPage = &m_pages[page];
		int count = static_cast<int>(pPage->GetCount());
		_node<__master_board_unit> * pUnit = pPage->m_pFirst;

		int i;
		for( i = 0 ; i < count ; ++i )
		{
			// 
			if( pUnit )
			{
				CopyMemory( &pInfo->board[i], &pUnit->node.BoardInfo, sizeof(_sPacketMPBoard) );
				pUnit = pUnit->next;
			}
			else
				break;
		}

		// 유효한 개수만큼만.
		pInfo->ucCount = static_cast<u_char>(i);
	}

	return true;
}


void _MPBoardList::GetInitialInfo( CMSG_MP_BOARD_INITIAL_OPEN * pInfo )
{
	// 내용을 넣는다.
	__master_board_page * pPage = &m_pages[0];
	int count = static_cast<int>(pPage->GetCount());
	_node<__master_board_unit> * pUnit = pPage->m_pFirst;
	int i;
	for( i = 0 ; i < count ; ++i )
	{
		// 
		if( pUnit )
		{
			CopyMemory( &pInfo->board[i], &pUnit->node.BoardInfo, sizeof(_sPacketMPBoard) );
			pUnit = pUnit->next;
		}
		else
			break;
	}

	// 유효한 개수만큼만.
	pInfo->ucCount = static_cast<u_char>(i);

}



void _MPBoardList::PrintList()
{
	printf( "  Total Page : %d - Total Unit = %d\n", m_iTotalPage, m_iTotalBoardUnit );

	__master_board_page * pPage = NULL;
	_node<__master_board_unit> * pNode = NULL;
	__master_board_unit * pUnit = NULL;
	int new_line = 0;

	for( int page = 0; page < m_iTotalPage ; ++page )
	{
		pPage = &m_pages[page];

		// 페이지 정보 표시.
		printf( "    = Page : %d Count = %d : First = 0x%08x =\n", pPage->GetPage(), pPage->GetCount(), pPage->m_pFirst );
		printf( "      " );

		pNode = pPage->m_pFirst;

		new_line = 1;
		for( u_int unit = 0; unit < pPage->GetCount(); ++unit )
		{
			// 게시판 정보 표시.
			if( pNode )
			{
				pNode->node.Print();

				if( 0 == (new_line%3) )		// 3개마다 새줄.
				{
					printf( "\n      " );
				}

				++new_line;
				pNode = pNode->next;
			}
			else
			{
				break;
			}
		}
		printf( "\n" );
	}

}



//////////////////////////////////////////////////////////////////////////
//
//	CDSMasterSet : 사제 관계 정보 유지 및 스승 게시판 관리.
//
//////////////////////////////////////////////////////////////////////////
bool CDSMasterSet::CreateMasterPupilManager( COLE_DB_Executer * pDB, int init_master, int init_page, int init_board )
{
	bool bSuccess = true;

	m_MasterPool.expand( init_master );

	m_pGameDB = pDB;

	InitializeCriticalSectionAndSpinCount( &m_csMasterMap, 0x80001000 );

	m_MasterMap.clear();

	switch( g_pServerInfo->get_service_mode() )
	{
	case en_service_mode_innertest:
		bSuccess = CreateMPBoard( init_page/20, init_board/20 );
		break;
	case en_service_mode_outertest:
		bSuccess = CreateMPBoard( init_page/10, init_board/10 );
		break;
	case en_service_mode_standard:
		bSuccess = CreateMPBoard( init_page, init_board );
		break;
	}

	return bSuccess;
}

void CDSMasterSet::DestroyManager()
{
	DeleteCriticalSection( &m_csMasterMap );
}

CDSMaster * CDSMasterSet::FindAndMakeMasterIndex( int cuid )
{
	// 
	CDSMaster * pMaster = FindMaster(cuid);

	if( NULL == pMaster )
	{
		// MP_GetMasterFromPupil : 함수는 Pupil Table을 먼저 조사한 후, 없으면, Master Table도 조사한다.
		int master_index = m_pGameDB->MP_GetMasterFromPupil( cuid );
		if( master_index )
		{
			pMaster = AddMasterInfo_FromDB( master_index );
		}
	}

	return pMaster;
}


CDSMaster * CDSMasterSet::FindMaster( int master_uid )
{
	CDSMaster * pMaster = NULL;
	map<int, CDSMaster*>::iterator itr;

	LockMasterMap();

	itr = m_MasterMap.find( master_uid );
	if( itr != m_MasterMap.end() )
		pMaster = itr->second;

	UnlockMasterMap();

	return pMaster;
}


CDSMaster * CDSMasterSet::AddMasterInfo_FromDB( int master_uid )
{
	// DB 에서 읽어와서 Set 에 등록한다.
	CDSMaster * pNew = m_MasterPool.get_node();
	if( pNew )
	{
		pNew->SetIndex( master_uid );
		if( true == m_pGameDB->MP_ReadAndFillMasterInfo( pNew ) )
		{
			m_MasterMap.insert( map<int,CDSMaster*>::value_type( master_uid, pNew ) );
		}
		else
		{
			m_MasterPool.free_node( pNew );
			pNew = NULL;
		}
	}

	return pNew;
}


void CDSMasterSet::DSProc_MasterInfoReq( CMSG_MP_INFO_REQ * pMP, _h_DS_Com * pGS )
{
	PrintConsole( "[MP] Recv : Info Req <%s> - master <%d>\n", pMP->cFromName, pMP->iMasterIndex );
	CDSMaster * pMaster = FindAndMakeMasterIndex( pMP->iMasterIndex );
	if( pMaster )
	{
		CMSG_MP_INFO_MASTER pkInfo;
		pkInfo.SetFromName( pMP->cFromName );

		pMaster->FillPacket_MyMasterInfo( &pkInfo );

		pGS->Send( pkInfo.usLength, (char*)&pkInfo );

		PrintConsole( "[MP] To C<%s> : Master Info <%s:%d>\n", pMP->cFromName, pkInfo.cMasterName, pkInfo.iMasterIndex );
	}
	else
{	
		CMSG_MP_NO_INFO pkNoInfo;
		pkNoInfo.SetName( pMP->cFromName );
		pkNoInfo.iMasterIndex = pMP->iMasterIndex;

		pGS->Send( pkNoInfo.usLength, (char*)&pkNoInfo );

		PrintConsole( "[MP] To C<%s> : No Master Info <%d>\n", pkNoInfo.cFromName, pkNoInfo.iMasterIndex );
	}
}


void CDSMasterSet::DSProc_AddPoint( CMSG_MP_ADD_POINT * pMP )
{
	CDSMaster * pMaster = FindMaster( pMP->iMasterIndex );
	if( pMaster )
	{
		pMaster->AddPoint( pMP->iFromCharac, pMP->iAddValue );
	}
}


void CDSMasterSet::DSProc_AddKillCount( CMSG_MP_ADD_KILL_COUNT * pMP )
{
	CDSMaster * pMaster = FindMaster( pMP->iMasterIndex );
	if( pMaster )
	{
		pMaster->AddKillCount( pMP->iFromCharac, pMP->iAddValue );
	} 
}


void CDSMasterSet::DSProc_CreateMPReq( CMSG_MP_CREATE_MP_REQ * pMP, _C_CHARAC * pCharac, _h_DS_Com * pGS )
{
	if( FindMaster( pMP->iMasterIndex ) )
	{
		// 이미 있다면 뭔가 이상한 것이다.
		CMSG_MP_CREATE_FAIL pkFail;
		pkFail.SetName( pMP->cFromName );
		pkFail.iMasterIndex = pMP->iMasterIndex;
		pkFail.ucFailCode = 0;				// 시스템 에러.

		PrintConsole( "[MP] Create MP Info Fail : Already existed<%s>\n", pMP->cFromName );

		pGS->Send( pkFail.usLength, (char*)&pkFail );
	}

	pCharac->CHARAC_MP.ucMPPosition = en_mp_relation_none;
	pCharac->CHARAC_MP.ucRes = 0;
	pCharac->CHARAC_MP.usRes = 0;
	pCharac->CHARAC_MP.iMasterIndex = 0;

	CDSMaster * pNew = m_MasterPool.get_node();
	if( pNew )
	{
		pNew->InitByDefault( pMP->cFromName );		// 
		pNew->SetIndex( pMP->iMasterIndex );

		pNew->SetMasterSex( pCharac->CHARAC_BASIC.cSex );
		pNew->SetMasterLevel( pMP->ucLevel );
		pNew->SetMasterMoonpa( pMP->ucMoonpa );
		pNew->SetMasterClass( pMP->ucClass );
		pNew->SetMasterGrade( pMP->ucClassGrade );

		if( m_pGameDB->MP_CreateMasterInfo( pNew ) )
		{

			AddToMasterMap( pMP->iMasterIndex, pNew );

			// 보드에 추가하기.
			AddBoard( pNew );

			if( pMP->iMasterIndex == pCharac->CHARAC_BASIC.iUniqueID )
				pCharac->CHARAC_MP.ucMPPosition = en_mp_relation_master;
			else
				pCharac->CHARAC_MP.ucMPPosition = en_mp_relation_pupil;
			pCharac->CHARAC_MP.ucRes = 0;
			pCharac->CHARAC_MP.usRes = 0;
			pCharac->CHARAC_MP.iMasterIndex = pMP->iMasterIndex;

			CMSG_MP_CREATE_SUCCESS pkSuccess;
			pkSuccess.SetName( pMP->cFromName );
			pkSuccess.iMasterIndex = pMP->iMasterIndex;
			pkSuccess.ucMasterSex = pNew->GetMasterSex();
			pkSuccess.ucMasterInnerLevel = pNew->GetMasterLevel();
			pkSuccess.ucMasterMoonpa = pNew->GetMasterMoonpa();
			pkSuccess.ucMasterClass = pNew->GetMasterClass();
			pkSuccess.ucMasterClassgrade = pNew->GetMasterClassGrade();			

			PrintConsole( "[MP] Create Success : Master Index = %d\n", pNew->GetIndex() );

			pGS->Send( pkSuccess.usLength, (char*)&pkSuccess );
		}
		else
		{
			m_MasterPool.free_node( pNew );
			// 실패.
			// 시스템 에러를 보낸다.
			CMSG_MP_CREATE_FAIL pkFail;
			pkFail.SetName( pMP->cFromName );
			pkFail.iMasterIndex = pMP->iMasterIndex;
			pkFail.ucFailCode = 1;				// 시스템 에러.

			PrintConsole( "[MP] Create MP Info Fail : Fail to DB Update<%s>\n", pMP->cFromName );

			pGS->Send( pkFail.usLength, (char*)&pkFail );
		}
	}
	else
	{
		// 시스템 에러를 보낸다.
		CMSG_MP_CREATE_FAIL pkFail;
		pkFail.SetName( pMP->cFromName );
		pkFail.iMasterIndex = pMP->iMasterIndex;
		pkFail.ucFailCode = 1;				// 시스템 에러.

		PrintConsole( "[MP] Create MP Info Fail : System<%s>\n", pMP->cFromName );

		pGS->Send( pkFail.usLength, (char*)&pkFail );
	}
}


void CDSMasterSet::DSProc_NoticeCon( CMSG_MP_NOTICE_CONNECTION * pMP )
{
	CDSMaster * pMaster = FindMaster( pMP->iMasterIndex );
	if( pMaster )
	{
		PrintConsole( "[MP] M:%d - C:%d Connect Notice(Zone - %u, state %u)\n", 
			pMP->iMasterIndex, pMP->iCharacIndex, pMP->ucCurZone, pMP->ucConnectState );

		u_char prev_state = pMaster->GetMasterConnectState();

		if( 0 == pMP->ucConnectState )
		{
			// DB 에 저장.
			m_pGameDB->MP_UpdateMasterInfo( pMaster );
		}

		pMaster->ConnectNotice( pMP->iCharacIndex, pMP->ucConnectState, pMP->ucCurZone );

		pMaster->SendToAll( (char*)pMP, pMP->usLength );

		if( pMP->iMasterIndex == pMP->iCharacIndex )
		{
			// 스승의 접속 또는 해제는 게시판에 알려야 한다.
			// 단, 이동은 전하지 않는다.
			if( 0 == pMP->ucConnectState )
			{
				DelBoard( pMaster );
			}
			else if( 0 == prev_state && 1 == pMP->ucConnectState )
			{
				// 로그인 이면, ( 서버이동은 알리지 않는다. )
				AddBoard( pMaster );
			}
		}
	}
}


void CDSMasterSet::DSProc_ChangeMasterNick( CMSG_MP_CHANGE_MASTER_NICK_REQ * pMP )
{
	CDSMaster * pMaster = FindMaster( pMP->iMasterIndex );
	if( pMaster )
	{
		// DB 에 쓰기가 빠져있음.
		pMaster->ChangeMasterNick( pMP->cNick );
		m_pGameDB->MP_UpdateMasterNick( pMaster );
		BoardUpdateNick( pMaster );
	}
}


void CDSMasterSet::DSProc_GroupChat( CMSG_MP_CHAT * pMP )
{
	CDSMaster * pMaster = FindMaster( pMP->iMasterIndex );
	if( pMaster )
	{
		pMaster->SendToAll( (char*&)pMP, pMP->usLength );
	}
}


void CDSMasterSet::DSProc_ApplyForPupil( CMSG_MP_GD_APPLY_FOR_PUPIL * pMP, _h_DS_Com * pGS )
{
	// 우선 스승을 찾는다.
	PCHARAC pTarget = g_pAccMapManager->Find_Charac( pMP->cToMasterName );	
	if( pTarget )
	{
		// 스승 정보를 찾아, 신 제자에게는 간략 정보를 보내고, 
		// 스승에게는 신청을 보낸다.
		// 만약 사제 정보가 없다면, 오류 메시지를 보낸다.
		CDSMaster * pMaster = FindMaster( pTarget->CHARAC_MP.iMasterIndex );
		if( pMaster )
		{
			// 더 들어갈 수 있는지 확인한다.
			u_char ret = en_mp_apply_no_error;
			if( en_mp_apply_no_error == ( ret = pMaster->CanAddNewPupil( pMP->cFromPupilName ) ) )
			{
				CMSG_MP_DG_APPLY_FOR_PUPIL pkToMaster;
				pkToMaster.ucPupilMoonpa = pMP->ucPupilMoonpa;
				pkToMaster.ucPupilCurLevel = pMP->ucPupilCurLevel;
				pkToMaster.ucPupilClass = pMP->ucPupilClass;
				pkToMaster.ucPupilClassGrade = pMP->ucPupilClassGrade;

				pkToMaster.SetName( pMP->cFromPupilName, pMP->cToMasterName );

				PrintConsole( "[MP] Apply Pupil<%s> : To Master<%s>\n", pkToMaster.cFromPupilName, pkToMaster.cToMasterName );

				g_pServerInfo->send_to_gs( &pkToMaster, pkToMaster.usLength, pTarget->CHARAC_CUR_BASIC.sZone );

				// 신청한 캐릭터에게 스승의 간략 정보를 준다.
				CMSG_MP_MASTER_SIMPLE_INFO_TO_PUPIL pkSimpleInfo;
				pkSimpleInfo.SetName( pMP->cFromPupilName, pMP->cToMasterName );
				pkSimpleInfo.ucMasterRank = pMaster->GetMasterRank();
				pkSimpleInfo.ucMasterLevel = pMaster->GetMasterLevel();
				pkSimpleInfo.ucMasterMoonpa = pMaster->GetMasterMoonpa();
				pkSimpleInfo.ucMasterClass = pMaster->GetMasterClass();
				pkSimpleInfo.ucMasterClassGrade = pMaster->GetMasterClassGrade();
				pkSimpleInfo.ucCurZone = pMaster->GetMasterCurZone();
				pkSimpleInfo.SetNick( pMaster->GetMasterNick() );

				pGS->Send( pkSimpleInfo.usLength, (char*)&pkSimpleInfo );
			}
			else
			{
				// 꽉 참.
				CMSG_MP_APPLY_FOR_PUPIL_ERROR pkError;
				pkError.SetName( pMP->cFromPupilName, pMP->cToMasterName );

				pkError.ucErrorCode = ret;		// 제자가 꽉 찼다.

				PrintConsole( "[MP] Apply Pupil<%s> Error : no room(%u)\n", pkError.cFromPupilName, pkError.ucErrorCode );

				g_pServerInfo->send_to_gs( &pkError, pkError.usLength, pTarget->CHARAC_CUR_BASIC.sZone );
			}
		}
		else
		{
			// 사제 정보가 없다는 것은, 자격이 없다는 것이다.
			CMSG_MP_APPLY_FOR_PUPIL_ERROR pkError;
			pkError.SetName( pMP->cFromPupilName, pMP->cToMasterName );

			pkError.ucErrorCode = en_mp_apply_error_target_cannot_master;		// 스승 자격이 없다.

			PrintConsole( "[MP] Apply Pupil<%s> Error : not master(%u)\n", pkError.cFromPupilName, pkError.ucErrorCode );

			g_pServerInfo->send_to_gs( &pkError, pkError.usLength, pTarget->CHARAC_CUR_BASIC.sZone );
		}
	}
	else
	{
		// 없을 때, 
		CMSG_MP_APPLY_FOR_PUPIL_ERROR pkError;
		pkError.SetName( pMP->cFromPupilName, pMP->cToMasterName );

		pkError.ucErrorCode = en_mp_apply_error_target_not_found;		// 마스터를 찾을 수 없다.

		PrintConsole( "[MP] Apply Pupil<%s> Error : Not found master(%u)\n", pkError.cFromPupilName, pkError.ucErrorCode );

		pGS->Send( pkError.usLength, (char*)&pkError );
	}
	
}


void CDSMasterSet::DSProc_ApplyForPupilError( CMSG_MP_APPLY_FOR_PUPIL_ERROR * pMP )
{
	// Pupil 을 찾아서, 에러 메시지를 전달한다.
	PCHARAC pTarget = g_pAccMapManager->Find_Charac( pMP->cFromPupilName );	
	if( pTarget )
	{
		PrintConsole( "[MP] Error To C<%s>\n", pMP->cFromPupilName );
		g_pServerInfo->send_to_gs( pMP, pMP->usLength, pTarget->CHARAC_CUR_BASIC.sZone );
	}
}

void CDSMasterSet::DSProc_ApplyAnswerYes( CMSG_MP_ANSWER_YES * pMP, _h_DS_Com * pMasterGS )
{
	CDSMaster * pMaster = FindMaster( pMP->iMasterIndex );
	if( pMaster )
	{
		PCHARAC pTarget = g_pAccMapManager->Find_Charac( pMP->cPupilName );	
		if( pTarget )
		{
			if( pMaster->CheckNewPupil( pMP->cPupilName ) )
			{
				_sCSPK_Pupil tempP;
				tempP.Reset();
				tempP.SetName( pMP->cPupilName );
				tempP.SetIndex( pTarget->CHARAC_BASIC.iUniqueID );
				tempP.ucSex = static_cast<u_char>(pTarget->CHARAC_BASIC.cSex);
				tempP.ucLevel = static_cast<u_char>(pTarget->CHARAC_STATE.sInnerLevel);
				tempP.ucMoonpa = static_cast<u_char>(pTarget->CHARAC_BASIC.cParty);
				tempP.ucClass = static_cast<u_char>(pTarget->CHARAC_BASIC.cClass );
				tempP.ucClassGrade = static_cast<u_char>(pTarget->CHARAC_BASIC.cClassGrade);
				tempP.ucZone = static_cast<u_char>(pTarget->CHARAC_CUR_BASIC.sZone);
				tempP.ucPupilPosition = 3;

				PrintConsole( "[MP] MP:%d, Add Pupil \n", pMP->iMasterIndex );

				if( true == pMaster->AddNewPupil( tempP ) )
				{
					// DB 에 쓴다.
					
					if( true == m_pGameDB->MP_AddNewPupil( tempP, pMaster->GetIndex() ) )
					{
						PrintConsole( "   - Write DB : Add Pupil C<%s> - %d \n", tempP.cPupilName, tempP.iPupilIndex );

						pTarget->CHARAC_MP.ucMPPosition = en_mp_relation_pupil;
						pTarget->CHARAC_MP.ucRes = 0;
						pTarget->CHARAC_MP.usRes = 0;
						pTarget->CHARAC_MP.iMasterIndex = pMaster->GetIndex();


						CMSG_MP_COMPLETE_JOIN_PUPIL pkComplete;
						pkComplete.SetName( pMP->cPupilName, pMaster->GetMasterName() );

						g_pServerInfo->send_to_gs( &pkComplete, pkComplete.usLength, pTarget->CHARAC_CUR_BASIC.sZone );

						CMSG_MP_INFO_MASTER pkInfo;
						pkInfo.SetFromName( pMP->cPupilName );
						pMaster->FillPacket_MyMasterInfo( &pkInfo );

						g_pServerInfo->send_to_gs( &pkInfo, pkInfo.usLength, pTarget->CHARAC_CUR_BASIC.sZone );


						CMSG_MP_ADD_NEW_PUPIL pkNew;
						pkNew.iMasterIndex = pMP->iMasterIndex;
						pkNew.SetInfo( tempP );

						pMaster->SendToAll( (char*)&pkNew, pkNew.usLength );
					}
					else
					{
						// DB 쓰기 실패.
						// pMaster 에서 삭제하고, 실패를 보낸다.
						PrintConsole( "   - Fail to write DB C<%s>\n", pMP->cPupilName );

						SendApplyError( pMP->cPupilName, en_mp_apply_error_fail_to_add_pupil, pTarget->CHARAC_CUR_BASIC.sZone, NULL );
						SendApplyError( pMP->cFromMasterName, en_mp_apply_error_fail_to_add_pupil, 0, pMasterGS );
					}

				}
				else
				{
					// 추가 실패.
					PrintConsole( "   - Fail to add Pupil to mp info C<%s>\n", pMP->cPupilName );

					SendApplyError( pMP->cPupilName, en_mp_apply_error_fail_to_add_pupil, pTarget->CHARAC_CUR_BASIC.sZone, NULL );
					SendApplyError( pMP->cFromMasterName, en_mp_apply_error_fail_to_add_pupil, 0, pMasterGS );
				}
			}
			else
			{
				PrintConsole( "   - Fail : Cannot add Pupil C<%s>\n", pMP->cPupilName );
				// 실패 메시지.
				SendApplyError( pMP->cPupilName, en_mp_apply_error_fail_to_add_pupil, pTarget->CHARAC_CUR_BASIC.sZone, NULL );
				SendApplyError( pMP->cFromMasterName, en_mp_apply_error_fail_to_add_pupil, 0, pMasterGS );
			}
		}
		else
		{
			PrintConsole( "   - Fail : Cannot find Pupil C<%s>\n", pMP->cPupilName );

			SendApplyError( pMP->cFromMasterName, en_mp_apply_error_fail_to_add_pupil, 0, pMasterGS );
		}

		// 중간에 캐릭터가 나가버리면 안된다.
	}
	else
	{
		PrintConsole( "   - Fail : not found master:%d \n", pMP->iMasterIndex );
		// 사제 정보가 없다는 건 이상한 거다.
		SendApplyError( pMP->cFromMasterName, en_mp_apply_error_fail_to_add_pupil, 0, pMasterGS );
	}
}



void CDSMasterSet::DSProc_ApplyAnswerNo( CMSG_MP_ANSWER_APPLY_TO_PUPIL_NO * pMP )
{
	PCHARAC pTarget = g_pAccMapManager->Find_Charac( pMP->cPupilName );
	if( pTarget )
	{
		PrintConsole( "[MP] Master(%d) Answer no to C<%s>\n", pMP->cFromMasterName, pMP->cPupilName );
		
		g_pServerInfo->send_to_gs( pMP, pMP->usLength, pTarget->CHARAC_CUR_BASIC.sZone );
	}
}



void CDSMasterSet::DSProc_ApplyError( CMSG_MP_APPLY_ERROR * pMP )
{
	// Pupil 을 찾아서, 에러 메시지를 전달한다.
	PCHARAC pTarget = g_pAccMapManager->Find_Charac( pMP->cFromPupilName );	
	if( pTarget )
	{
		PrintConsole( "[MP] Error To C<%s>\n", pMP->cFromPupilName );
		g_pServerInfo->send_to_gs( pMP, pMP->usLength, pTarget->CHARAC_CUR_BASIC.sZone );
	}
}


void CDSMasterSet::SendApplyError( char * name, u_char code, short gs, _h_DS_Com * pGS )
{
	CMSG_MP_APPLY_ERROR pkError; 
	pkError.SetName( name );
	pkError.ucErrorCode = code;

	if( pGS )
	{
		pGS->Send( pkError.usLength, (char*)&pkError );
	}
	else if( gs )
	{
		g_pServerInfo->send_to_gs( &pkError, pkError.usLength, gs );
	}
}

//////////////////////////////////////////////////////////////////////////
// 파문/강제추방/하산 처리.
//////////////////////////////////////////////////////////////////////////
void CDSMasterSet::DSProc_StrikeReq( CMSG_MP_STRIKE_PUPIL_REQ * pMP, _h_DS_Com * pGS )
{
	// 
	PrintConsole( "[MP] Recv Strike Req : Master(%d) - Pupil<%s>\n", pMP->iMasterIndex, pMP->cPupilName );


	CDSMaster * pMaster = FindMaster( pMP->iMasterIndex );
	if( pMaster )
	{
		u_char code = pMaster->StrikeReq( pMP->cPupilName, m_pGameDB );
		if( code )
		{
			SendStrikeError( pMP->iMasterIndex, pMP->cPupilName, code, pGS );
		}
	}
	else 
	{
		SendStrikeError( pMP->iMasterIndex, pMP->cPupilName, 0, pGS );
	}
}


void CDSMasterSet::DSProc_LeaveReq( CMSG_MP_LEAVE_REQ * pMP, _h_DS_Com * pDSComm )
{
	PrintConsole( "[MP] Recv Leave Req : Master(%d) C<%s> \n", pMP->iMasterIndex, pMP->cPupilName );

	CDSMaster * pMaster = FindMaster( pMP->iMasterIndex );
	if( pMaster )
	{
		u_char code = pMaster->LeaveReq( pMP->cPupilName, m_pGameDB );
		if( code )
		{
			SendLeaveError( pMP->iMasterIndex, pMP->cPupilName, code, pDSComm );
		}
	}
	else
	{
		SendLeaveError( pMP->iMasterIndex, pMP->cPupilName, 0, pDSComm );
	}
}



bool CDSMasterSet::DBProc_StrikePupil( int strike_pupil )
{
	// strike = 1;
	return m_pGameDB->MP_BackupPupil( strike_pupil, 1 );
}

bool CDSMasterSet::DBProc_BanPupil( int ban_pupil )
{
	// ban = 2;
	return m_pGameDB->MP_BackupPupil( ban_pupil, 2 );
}

bool CDSMasterSet::DBProc_LeavePupil( int leave_pupil )
{
	// leave = 3;
	return m_pGameDB->MP_BackupPupil( leave_pupil, 3 );
}


void CDSMasterSet::SendStrikeError( int master, char * pupil_name, u_char code, _h_DS_Com * pGS )
{
	CMSG_MP_STRIKE_ERROR pkError;
	pkError.iMasterIndex = master;
	pkError.SetName( pupil_name );
	pkError.ucErrorCode = code;

	PrintConsole( "[MP] Strike Error. Master:%d, PUpile<%s>, Code:%u\n", 
		master, pupil_name, code );

	pGS->Send( pkError.usLength, (char*)&pkError );
}

void CDSMasterSet::SendLeaveError( int master, char * pupil_name, u_char code, _h_DS_Com * pGS )
{
	CMSG_MP_LEFT_ERROR pkError;
	pkError.SetName( pupil_name );
	pkError.ucErrorCode = code;

	PrintConsole( "[MP] Left Error. PUpile<%s>, Code:%u\n", pupil_name, code );

	pGS->Send( pkError.usLength, (char*)&pkError );
}


void CDSMasterSet::ShowMaster( int master )
{
	if( 0 == master )
	{	
		printf( "\n==  All Master Information\n" );
		CDSMaster * pMaster = NULL;
		map<int, CDSMaster*>::iterator itr = m_MasterMap.begin();
		while( itr != m_MasterMap.end() )
		{
			pMaster = itr->second;
			pMaster->ShowInfo();
			++itr;
		}
	}	
	else
	{
		CDSMaster * pMaster = NULL;
		map<int, CDSMaster*>::iterator itr = m_MasterMap.find(master);
		if( itr != m_MasterMap.end() )
		{
			pMaster = itr->second;
			pMaster->ShowInfo();
		}
		else
		{
			printf( " No master info.(%d)\n", master );
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 게시판 관련 함수
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CDSMasterSet::CreateMPBoard( int page_res, int board_res )
{
	if( false == m_total_board.CreateBoardList( 0, page_res, board_res ) )
	{
		printf( "[INIT FAIL] MPBoard - CraeteMPBoard() Failed\n" );
		return false;
	}

	if( false == m_white_board.CreateBoardList( 1, page_res, board_res ) )
	{
		printf( "[INIT FAIL] MPBoard - CraeteMPBoard() Failed\n" );
		return false;
	}

	if( false == m_black_board.CreateBoardList( -1, page_res, board_res ) )
	{
		printf( "[INIT FAIL] MPBoard - CraeteMPBoard() Failed\n" );
		return false;
	}

	return true;
}

bool CDSMasterSet::AddBoard( CDSMaster * pMaster )
{
	PrintConsole( "[MPBoard] Add - C<%s> : %d\n", pMaster->GetMasterName(), pMaster->GetIndex() );

	if( pMaster->GetMoonpaProp() > 0 )
	{
		m_white_board.AddMPBoard( pMaster );
	}
	else
	{
		m_black_board.AddMPBoard( pMaster );
	}

	m_total_board.AddMPBoard( pMaster );

	return true;
}

void CDSMasterSet::DelBoard( CDSMaster * pMaster )
{
	PrintConsole( "[MPBoard] Del - C<%s> : %d\n", pMaster->GetMasterName(), pMaster->GetIndex() );
	if( pMaster->GetMoonpaProp() > 0 )
	{
		m_white_board.DelMPBoard( pMaster->GetIndex() );
	}
	else
	{
		m_black_board.DelMPBoard( pMaster->GetIndex() );
	}

	m_total_board.DelMPBoard( pMaster->GetIndex() );
}


void CDSMasterSet::BoardUpdateNick( CDSMaster * pMaster )
{
	if( pMaster->GetMoonpaProp() > 0 )
	{
		m_white_board.UpdateMPBoardNick( pMaster->GetIndex(), pMaster->GetMasterNick() );
	}
	else
	{
		m_black_board.UpdateMPBoardNick( pMaster->GetIndex(), pMaster->GetMasterNick() );
	}

	m_total_board.UpdateMPBoardNick( pMaster->GetIndex(), pMaster->GetMasterNick() );
}


bool CDSMasterSet::GetBoardInfo( int page, int prop, CMSG_MP_BOARD_PAGE_INFO * pInfo )
{
	if( prop < 0 )
	{
		// 흑도. m_black_board
		return m_black_board.GetPageInfo( page, pInfo );
	}
	else if( prop > 0 )
	{
		// 백도. m_white_board
		return m_white_board.GetPageInfo( page, pInfo );
	}
	else
	{
		// 낭인. m_total_borad.
		return m_total_board.GetPageInfo( page, pInfo );
	}
}


// en_dg_mp_mode_board_initial_req:
void CDSMasterSet::DSProc_BoardInitialReq( CMSG_MP_BOARD_INITIAL_REQ * pMP, _h_DS_Com * pDSComm )
{
	_MPBoardList * pList = NULL;
	if( pMP->cSide < 0 )
		pList = &m_black_board;
	else if( pMP->cSide == 0 )
		pList = &m_total_board;
	else 
		pList = &m_white_board;

	CMSG_MP_BOARD_INITIAL_OPEN pkOpen;
	pkOpen.Reset();
	pkOpen.SetName( pMP->cCharacName );
	pkOpen.cSide = pMP->cSide;
	pkOpen.ucTotalPage = static_cast<u_char>(pList->GetPageCount());
    
	// 페이지가 있을 때만, 채워보내면 된다.
	if( 0 < pkOpen.ucTotalPage )
		pList->GetInitialInfo( &pkOpen );

	pkOpen.SetLength();

	PrintConsole( "[MPBoard] C<%s> Send Initial Open : Total %u\n", pkOpen.cCharacName, pkOpen.ucTotalPage );

	pDSComm->Send( pkOpen.usLength, (char*)&pkOpen );

}

// en_dg_mp_mode_board_req_page:
void CDSMasterSet::DSProc_BoardPageReq( CMSG_MP_BOARD_REQ_PAGE * pMP, _h_DS_Com * pDSComm )
{
	_MPBoardList * pList = NULL;
	if( pMP->cSide < 0 )
		pList = &m_black_board;
	else if( pMP->cSide == 0 )
		pList = &m_total_board;
	else 
		pList = &m_white_board;

	CMSG_MP_BOARD_PAGE_INFO pkPage;
	pkPage.Reset( pMP->ucReqPage );
	pkPage.SetName( pMP->cCharacName );
	pkPage.cSide = pMP->cSide;
	pkPage.ucTotalPage = static_cast<u_char>(pList->GetPageCount());

	pList->GetPageInfo( pkPage.ucPage, &pkPage );

	pkPage.SetLength();

	PrintConsole( "[MPBoard] C<%s> Send Page Info : Page(%u) Count(%u)\n", pkPage.cCharacName, pkPage.ucPage, pkPage.ucCount );

	pDSComm->Send( pkPage.usLength, (char*)&pkPage );
}


void CDSMasterSet::PrintBoard()
{
	// 보드의 내용을 표시한다.
	printf( "Total Board\n" );
	m_total_board.PrintList();

	printf( "\nWhite Board\n" );
	m_white_board.PrintList();

	printf( "\nBlack Board\n" );
	m_black_board.PrintList();
}

void CDSMasterSet::PrintSimpleInfo()
{
	PrintAll( "\n== MP Board Information==\n" );
	PrintAll( "  Total Board : Page - %d, Unit = %d\n", m_total_board.GetPageCount(), m_total_board.GetUnitCount() );
	PrintAll( "  White Board : Page - %d, Unit = %d\n", m_white_board.GetPageCount(), m_white_board.GetUnitCount() );
	PrintAll( "  Black Board : Page - %d, Unit = %d\n", m_black_board.GetPageCount(), m_black_board.GetUnitCount() );
}

#endif

