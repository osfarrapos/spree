#include "..\stdafx.h"
#include "Organization.h"

#include "Castle.h"

#include "..\Global\Global_ServerInfo.h"
#include "..\Network\LogSvrLink.h"
#pragma warning(disable:4509)
extern _j_ServerInfo		g_ServerInfo;			// 서버정보
extern _j_queue_UDP_Packet *	g_pUDPSendPacketQ ;
extern CLogSvr_Com *		g_pLogSvrCom;
extern _DSCom_Queue *			g_pDSComQueue;
extern COrganizationManager *	g_pOrganizationManager;

#ifdef _PD_ALLY_ORG_STEP_1_
extern CAllyManager *			g_pAllyManager;
#endif


COrganizationManager::COrganizationManager( size_t size ) : m_InitSize ( size ), m_pGameDB( NULL ), m_ORPool( size )
{

	_Organization * pOr = m_pOrganization = new _Organization[size] ;
	assert( m_pOrganization ) ;
	m_MapOR.clear() ;	

	for( u_int i = 0 ; i < size ; i++ )
	{
		m_ORPool.insert( pOr ) ;
		++pOr ;
	}	
}

COrganizationManager::~COrganizationManager()
{
	delete [] m_pOrganization  ;
}

bool COrganizationManager::ReadORDataFromDB( COLE_DB_Executer * GameDB )
{
	m_pGameDB = GameDB ;
	return GameDB->GetOrInfo( this ) ;
}


#ifdef _PD_ALLY_ORG_STEP_1_
void COrganizationManager::CheckInitialAlly( COLE_DB_Executer * GameDB )
{
	CAllyOrganization * pAlly = NULL;

	printf( "\nOrganization & Ally Check\n" );
	_Organization * pOR = NULL ;

	or_lock() ;
	try
	{
		ORMap::iterator OrMapItr =  m_MapOR.begin() ;
		while( OrMapItr != m_MapOR.end() )
		{
			pOR = OrMapItr->second ;

			if( pOR->m_iAllyIndex )
			{
				pAlly = g_pAllyManager->find_ally( pOR->m_iAllyIndex );
				if( !pAlly )
				{
					GameDB->AllyUpdateOfOrg( pOR->m_iIndex, 0 );
					pOR->m_iAllyIndex = 0;
				}
			}

			++OrMapItr;
		}
	}
	catch (...)
	{
		::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
	}		
	or_unlock() ;
}
#endif

const int COrganizationManager::CreateOR( IN OUT  _Organization & or )
{
	TRY{
		or.m_iIndex = m_pGameDB->InsertOrganization( or ) ;
		if( or.m_iIndex > 0 ) 
		{
			InsertOR( or ) ;

			_CharacLog * pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
			if( pCharacLog )
			{
				memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

				pCharacLog->code = _LOG_OR_CREATE_ ;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
				memcpy( pCharacLog->cAccount, or.m_cOrName, 24) ;
				pCharacLog->cAccount[en_max_lil] = 0;
#else
				memcpy( pCharacLog->account, or.m_cOrName, 12) ;
#endif
				memcpy( pCharacLog->charac, or.m_cCaptainName , 13) ;
				pCharacLog->charac_uid = or.m_iIndex ;
				pCharacLog->zone = or.m_cBaseParty ;
				g_pLogSvrCom->InsertCharacLog( pCharacLog ) ;
			}


			for( int i = 0 ; i < _MAX_OR_UNIT_NUM ; i++ )
			{
				if( or.m_MemberList[i].m_cName[0] != 0 )
				{
					pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
					if( pCharacLog )
					{
						memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

						pCharacLog->code = _LOG_OR_JOIN_ ;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
						memcpy( pCharacLog->cAccount, or.m_cOrName, en_max_lil ) ;
						pCharacLog->cAccount[en_max_lil] = 0;
#else
						memcpy( pCharacLog->account, or.m_cOrName, 12 ) ;
						pCharacLog->account[12] = 0;
#endif
						memcpy( pCharacLog->charac, or.m_MemberList[i].m_cName , 13) ;						
						g_pLogSvrCom->InsertCharacLog( pCharacLog ) ;
					}
					
				}
			}
		}
	}
	CATCH_ALL{
		::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		
	}
	
	return or.m_iIndex ;
}


bool COrganizationManager::InsertOR( const _Organization & or )
{	
	bool bRet = false ;


	if( _Organization * pOr = m_ORPool.get() )
	{
		*pOr = or ;

		or_lock() ;
		TRY{
			ORMap::iterator OrMapItr =  m_MapOR.find( pOr->m_iIndex ) ;
			if( OrMapItr == m_MapOR.end() )
			{		
				m_MapOR[pOr->m_iIndex] = pOr ;
				bRet = true ;
			}
		}
		CATCH_ALL{
			::PrintConsole("[EXCEPTION] ==> %s(%d) \n",__FILE__, __LINE__) ;
			bRet = false ;
		}		
		or_unlock() ;
	}	
	return bRet ;
}

bool COrganizationManager::DeleteOR(  const int index )
{	
	bool bRet = false ;
#ifdef _PD_ALLY_ORG_STEP_1_

	int iAllyIndex = 0;

	or_lock() ;
	try{
		ORMap::iterator OrMapItr =  m_MapOR.find( index ) ;
		if( OrMapItr != m_MapOR.end() )
		{
			_Organization * pOr = OrMapItr->second ;			

			iAllyIndex = pOr->m_iAllyIndex;

			m_MapOR.erase( OrMapItr ) ;
			m_ORPool.insert( pOr ) ;
			if( m_pGameDB->DeleteOrganization( index ) )
			{
				bRet = true ;

				_CharacLog * pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
				if( pCharacLog )
				{
					memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

					pCharacLog->code = _LOG_OR_DESTROY ;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
					memcpy( pCharacLog->cAccount, pOr->m_cOrName, en_max_lil) ;
					pCharacLog->cAccount[en_max_lil] = 0;
#else
					memcpy( pCharacLog->account, pOr->m_cOrName, 12) ;
					pCharacLog->account[12]= 0;
#endif
					memcpy( pCharacLog->charac, pOr->m_cCaptainName, en_charac_name_length) ;
					pCharacLog->charac[en_charac_name_length] = 0;
					pCharacLog->charac_uid = pOr->m_iIndex ;
					pCharacLog->zone = pOr->m_cBaseParty ;
					g_pLogSvrCom->InsertCharacLog( pCharacLog ) ;
				}
			}
			else
			{
				bRet = false ;
			}			
		}
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] ==> %s(%d) \n",__FILE__, __LINE__) ;
	}		
	or_unlock() ;

	if( bRet && iAllyIndex > 0 )
	{
		CAllyOrganization * pAlly = g_pAllyManager->find_ally( iAllyIndex );
		if( pAlly )
		{
			// 영웅단이 나가는 것을 알린다.
			if( pAlly->leave_org( index ) )
			{
				if( m_pGameDB )
					m_pGameDB->AllyUpdateOfOrg( index, 0 );	// 나가는 거니까 0 으로.

				// 해체.
				// 1. 우선 탈퇴를 알리고, 
				// 2. 해체를 알린다.
				SMSG_ALLY_LEAVE_SUCCESS pkSuccess;
				pkSuccess.iAllyIndex = iAllyIndex;
				pkSuccess.iOrgLeave = index;

				g_pDSComQueue->GlobalSend( pkSuccess.usLength, (char*)&pkSuccess );

				// 맵에서 삭제하고, 
				g_pAllyManager->del_ally( iAllyIndex );

				pAlly->reset_ally( m_pGameDB );

				// 포인터 반환.
				g_pAllyManager->push( pAlly );

				SMSG_ALLY_BREAKUP_SUCCESS pkBreakup;
				pkBreakup.iAllyIndex = iAllyIndex;
				pkBreakup.ucBreakupReason = 1;		// passive breakup

				g_pDSComQueue->GlobalSend( pkBreakup.usLength, (char*)&pkBreakup );

				g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Leave & Scatter\n" );
				g_pSystemLog->LogFileWrite( "Leave & Scatter\n" );

				m_pGameDB->AllyBreak( iAllyIndex, 2 );	// passive

			}
			else
			{
				// 탈퇴만 알린다.
				if( m_pGameDB )
					m_pGameDB->AllyUpdateOfOrg( index, 0 );	// 나가는 거니까 0 으로.

				pAlly->db_update( m_pGameDB );

				SMSG_ALLY_LEAVE_SUCCESS pkSuccess;
				pkSuccess.iAllyIndex = iAllyIndex;
				pkSuccess.iOrgLeave = index;

				g_pDSComQueue->GlobalSend( pkSuccess.usLength, (char*)&pkSuccess );

				g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Leave\n" );
				g_pSystemLog->LogFileWrite( "Leave\n" );
			}
		}
	}

#else

	or_lock() ;
	try{
		ORMap::iterator OrMapItr =  m_MapOR.find( index ) ;
		if( OrMapItr != m_MapOR.end() )
		{
			_Organization * pOr = OrMapItr->second ;			
			m_MapOR.erase( OrMapItr ) ;
			m_ORPool.insert( pOr ) ;
			if( m_pGameDB->DeleteOrganization( index ) )
			{
				bRet = true ;
				_CharacLog * pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
				if( pCharacLog )
				{
					memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

					pCharacLog->code = _LOG_OR_DESTROY ;
					memcpy( pCharacLog->account, pOr->m_cOrName , 12) ;
					memcpy( pCharacLog->charac, pOr->m_cCaptainName , 13) ;
					pCharacLog->charac_uid = pOr->m_iIndex ;
					pCharacLog->zone = pOr->m_cBaseParty ;
					g_pLogSvrCom->InsertCharacLog( pCharacLog ) ;
				}
			}
			else
			{
				bRet = false ;
			}			
		}
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] ==> %s(%d) \n",__FILE__, __LINE__) ;
	}		
	or_unlock() ;

#endif
	
	return bRet ;

}

_Organization * COrganizationManager::GetORInfo( const int index )
{
	_Organization * pOr = NULL ;
	or_lock() ;
	try{
		ORMap::iterator OrMapItr =  m_MapOR.find( index ) ;	
		if( OrMapItr != m_MapOR.end() )
		{
			pOr = OrMapItr->second ;
		}	
	}
	catch(...)	{
		::PrintConsole("[EXCEPTION] ==> %s(%d) \n",__FILE__, __LINE__) ;
	}
	or_unlock() ;	
	return pOr ;
}


bool COrganizationManager::PutORUnit( const int index, const _Unit & unit )
{	 
	TRY{
		if( _Organization * pOr = FindOrganization( index ) )
		{		// _EMPTY_UNIT_SLOT_  : 빈 슬롯을 찾는다.
			if( _Unit * pUnit = FindUnit( pOr, _EMPTY_UNIT_SLOT_ ) )
			{
				++(pOr->m_usCurMemberNum) ;
				*pUnit = unit ;
				m_pGameDB->UpdateOrganization( *pOr ) ;


				_CharacLog * pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
				if( pCharacLog )
				{
					memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

					pCharacLog->code = _LOG_OR_JOIN_ ; 
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
					memcpy( pCharacLog->cAccount, pOr->m_cOrName, en_max_lil) ;
					pCharacLog->cAccount[en_max_lil] = 0;
#else
					memcpy( pCharacLog->account, pOr->m_cOrName, 12) ;
					pCharacLog->account[12] = 0;
#endif
					memcpy( pCharacLog->charac, pUnit->m_cName, en_charac_name_length) ;
					pCharacLog->charac[en_charac_name_length] = 0;
					g_pLogSvrCom->InsertCharacLog( pCharacLog );
				}

				return true ;
			}
		}
	} 
	CATCH_ALL{
		::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		
	}	
	return false ;	
}


int  COrganizationManager::PopORUnit( const int index, const char * name )
{
	/*
	time_t cur;
	time(&cur);
	tm * curDay = localtime( &cur );
	char szOrgDisbandDate[20];
	sprintf( szOrgDisbandDate, "%04d%02d%02d%02d", m_FirstCombatDate.year, m_FirstCombatDate.month, m_FirstCombatDate.day, m_FirstCombatDate.hour);
		return atoi(szFirstCombatDate);
	*/

	// -1 : 파산, 0: 실패, 1:성공
	TRY{
		if( _Organization * pOr = FindOrganization( index ) )
		{
			int slotNum ;
			if( _Unit * pUnit = FindUnit( pOr, name, &slotNum  ) )
			{
				// 단주는 탈퇴할 수 없다..
				if( pUnit->m_cClass == _Unit::_UNIT_CALSS_CAPTAIN_UNIT_ )
				{
					return 0 ;
				}

				// 3명 이하에서 탈퇴자가 발생하면 영웅단은 해체된다..
				if( pOr->m_usCurMemberNum < 4 )
				{
				//해체 하기 전에 단장을 찾아 해체 시간을 기록해준다.
				//온라인 중이면 해체 시간을 기록 할 수 있도록 보내준다(패킷 생성해야 함)
#ifdef	_ORG_PROMOTION_	//단 해체시 단장에게 해체시간 기록
					if(_Unit *pCaptainUnit = FindCaptain())
					{
						// 오프라인 중이면 캐릭터 이름으로 DB에 바로 해체 시간 저장 
						//- ND_V01_Charac
					}
					else //혹 캡틴이 널이면 단주 이름으로 직접 DB에만 시간 써줌 
					{
						//pOr->m_cCaptainName
					}
#endif//_ORG_PROMOTION_
					DeleteOR( index ) ;
					return -1 ;
				}

				// 대상이 오프라인이라면 전서구로 알려줘야 한다.
				if( pUnit->m_bOnline == false )
				{
					UMSG_CHARAC_OR_KICK msgCharacORKick = {0} ;
					msgCharacORKick.iKey = PASS_KEY ;
					msgCharacORKick.cMessage = UMSG_CHARAC_OR_KICK_NUM ;
					memcpy( msgCharacORKick.cCharacName, name, 13 ) ;
					memcpy( msgCharacORKick.cORName, pOr->m_cOrName, en_or_name_length+1 ) ;

					SOCKADDR_IN	* pAddr = g_ServerInfo.GetMSSockAddrIn() ;
					g_pUDPSendPacketQ->insert_udp( pAddr, (char*)&msgCharacORKick, sizeof(UMSG_CHARAC_OR_KICK) ) ;
				}

				_CharacLog * pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
				if( pCharacLog )
				{
					memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

					pCharacLog->code = _LOG_OR_OUT_ ;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
					memcpy( pCharacLog->cAccount, pOr->m_cOrName, en_max_lil);
					pCharacLog->cAccount[en_max_lil] = 0;
#else
					memcpy( pCharacLog->account, pOr->m_cOrName, 12);
					pCharacLog->account[12] = 0;
#endif
					memcpy( pCharacLog->charac, pUnit->m_cName, en_charac_name_length);
					pCharacLog->charac[en_charac_name_length] = 0;
					g_pLogSvrCom->InsertCharacLog( pCharacLog );
				}

				--(pOr->m_usCurMemberNum) ;
				memset( pUnit, 0, sizeof(_Unit) ) ;

				memset( &pOr->m_outTime[slotNum], 0, sizeof(_OutTime) ) ;

				m_pGameDB->UpdateOrganization( *pOr ) ;

				return 1 ;
			}
		}
	}
	CATCH_ALL{
		::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		
	}
	
	return 0 ;	
}


bool COrganizationManager::UpdateORUnit( const int index, const _Unit & unit, const _OutTime & outTime )
{
	TRY
	{
		
		if( _Organization * pOr = FindOrganization( index ) )
		{
			int slotNum ;
			if( _Unit * pUnit = FindUnit( pOr, unit.m_cName, &slotNum ) )
			{

				// 클래스 업 일경우..
				if( pUnit->m_cClass != unit.m_cClass && unit.m_cClass == _Unit::_UNIT_CLASS_SPECIAL_UNIT_ )
				{
					int iSpecialUnitCount  = 0 ;
					for( int i = 0 ; i < _MAX_OR_UNIT_NUM ; i++ )
					{
						if( pOr->m_MemberList[i].m_cClass == _Unit::_UNIT_CLASS_SPECIAL_UNIT_ )
						{
							// 스페셜클래스가 9명 이상이면 실패처리한다..
							if( ++iSpecialUnitCount >= 9 )
							{
								return false ;
							}
						}
					}				
				}

				*pUnit = unit ;

				if( outTime.year > 0 )
					pOr->SetOutTime( slotNum, outTime ) ;

				m_pGameDB->UpdateOrganization( *pOr ) ;
				return true ;
			}
		}
	}
	CATCH_ALL
	{
		::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		
	}
	
	return false ;	
}

bool COrganizationManager::SetUnitClass(  const int index, const char * name, const char Class  )
{
	TRY
	{
		if( _Organization * pOr = FindOrganization( index ) )
		{
			if( _Unit * pUnit = FindUnit( pOr, name ) )
			{
				pUnit->m_cClass = Class ;
				m_pGameDB->UpdateOrganization( *pOr ) ;
				return true ;
			}
		}
	}
	CATCH_ALL
	{
		::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		
	}
	
	return false ;
}

bool COrganizationManager::SetORExp(  const  int index, const u_int exp	)
{
	TRY
	{
		if( _Organization * pOr = FindOrganization( index ) )
		{
			pOr->m_uiExperience = exp ;
			m_pGameDB->UpdateOrganization( *pOr ) ;
			return true ;		
		}
	}
	CATCH_ALL
	{
		::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		
	}
	
	return false ;
}


bool COrganizationManager::ExchangeCaptain( const int index, const char * name )
{
	TRY
	{
		if( _Organization * pOr = FindOrganization( index ) )
		{		
			memcpy( pOr->m_cCaptainName, name, 13 ) ;

			int iRet = 0 ;
			for( int i = 0 ; i < _MAX_OR_UNIT_NUM ; i++ )
			{
				if( pOr->m_MemberList[i].m_cClass == _Unit::_UNIT_CALSS_CAPTAIN_UNIT_ )
				{
					pOr->m_MemberList[i].m_cClass = _Unit::_UNIT_CLASS_DEFAULT_UNIT_ ;
					++iRet ;
				}
				if( pOr->m_MemberList[i].m_cName[0] == name[0] &&  strcmp( pOr->m_MemberList[i].m_cName, name ) == 0  )
				{
					pOr->m_MemberList[i].m_cClass = _Unit::_UNIT_CALSS_CAPTAIN_UNIT_ ;
					++iRet ;
				}
				if( iRet == 2 ) break;
			}

			_CharacLog * pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
			if( pCharacLog )
			{
				memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

				pCharacLog->code = _LOG_OR_ENTRUST_ ;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
				memcpy( pCharacLog->cAccount, pOr->m_cOrName, en_max_lil);
				pCharacLog->cAccount[en_max_lil] = 0;
#else
				memcpy( pCharacLog->account, pOr->m_cOrName, 12) ;
				pCharacLog->account[12] = 0;
#endif
				memcpy( pCharacLog->charac, name, en_charac_name_length);
				pCharacLog->charac[en_charac_name_length] = 0;
				g_pLogSvrCom->InsertCharacLog( pCharacLog ) ;
			}

			m_pGameDB->UpdateOrganization( *pOr ) ;
			return true ;
		}
	}
	CATCH_ALL
	{
		::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		
	}
	
	return false ;
}


bool COrganizationManager::BaseUpdate(const  int index, const _OrganizationBase & or_base  )
{
	if( index == 0 ) return false ;

	bool bRet = false ;
	TRY
	{
		if( _Organization * pOr = FindOrganization( index ) )
		{		
			*(dynamic_cast<_OrganizationBase*>(pOr)) = or_base ;
			bRet = m_pGameDB->UpdateOrganization( *pOr ) ;			
		}

	}
	CATCH_ALL
	{
		::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);

	}
	
	return bRet ;
}


bool COrganizationManager::DBUpdate( const _Organization * pOr) 
{
	bool bRet = false ;
	TRY
	{
		bRet = m_pGameDB->UpdateOrganization( *pOr ) ;
	}
	CATCH_ALL
	{
		::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
	}

	return bRet ;
}

void COrganizationManager::ShowAllOrganization()
{
	printf( "\nOrganization List\n" );
	_Organization * pOR = NULL ;
	or_lock() ;
	try
	{
		ORMap::iterator OrMapItr =  m_MapOR.begin() ;
		while( OrMapItr != m_MapOR.end() )
		{
			pOR = OrMapItr->second ;

			printf( "  ORG : %3d - <%s> - Cap<%s>\n", pOR->m_iIndex, pOR->m_cOrName, pOR->m_cCaptainName );

			++OrMapItr;
		}
	}
	catch (...)
	{
		::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
	}		
	or_unlock() ;
}


#ifdef _PD_ALLY_ORG_STEP_1_
//////////////////////////////////////////////////////////////////////////
// CAllyManager/CAllyOrganization 구현부
//////////////////////////////////////////////////////////////////////////
void CAllyOrganization::reset_ally( COLE_DB_Executer * pDB )
{
	_Organization * pOrg = NULL;
	if( pDB )
	{
		for( int i = 0 ; i < _NDD_MAX_ORG_IN_ALLY_ ; ++i )
		{
			if( m_orgArrayInAlly[i].iOrIndex )
			{
				pDB->AllyUpdateOfOrg( m_orgArrayInAlly[i].iOrIndex, 0 );
				pOrg = m_orgArrayInAlly[i].pOrg;
				if( !pOrg )
					pOrg = g_pOrganizationManager->FindOrganization( m_orgArrayInAlly[i].iOrIndex );

				if( pOrg )
					pOrg->m_iAllyIndex = 0;
			}
		}
	}

	m_iIndex = 0;
	ZeroMemory( m_orgArrayInAlly, sizeof(m_orgArrayInAlly) );
	m_sProperty = 0;
	ZeroMemory( m_cMasterName, sizeof(m_cMasterName) );
}

bool CAllyOrganization::check_info( COrganizationManager * pOrManager )
{
	_Organization * pOrg = NULL;
	int i = 0;
	for( ; i < _NDD_MAX_ORG_IN_ALLY_ ; ++i )
	{
		pOrg = pOrManager->FindOrganization( m_orgArrayInAlly[i].iOrIndex );
		if( !pOrg )
		{
			ZeroMemory( &m_orgArrayInAlly[i], sizeof(__org_state_in_ally) );
		}
		else
		{
			m_orgArrayInAlly[i].pOrg = pOrg;
		}
	}

	if( get_org_count() < 2 || NULL == m_orgArrayInAlly[0].pOrg )
	{
		// 해체시킨다.
		return false;
	}

	CopyMemory( m_cMasterName, m_orgArrayInAlly[0].pOrg->m_cCaptainName, 13 );
	m_cMasterName[en_charac_name_length] = 0;

	return true;
}

void CAllyOrganization::db_update( COLE_DB_Executer * pDB )
{
	// DB 에 업데이트한다.
	pDB->AllyUpdate( get_index(), m_orgArrayInAlly[1].iOrIndex, 
		m_orgArrayInAlly[2].iOrIndex, m_orgArrayInAlly[3].iOrIndex,
		m_orgArrayInAlly[4].iOrIndex, get_property() );
}


bool CAllyOrganization::leave_org( int org )		// true : 해체, false 그대로.
{
	int slot = 0;
	for( ; slot < _NDD_MAX_ORG_IN_ALLY_ ; ++slot )
	{
		if( m_orgArrayInAlly[slot].iOrIndex == org )
		{
			if( m_orgArrayInAlly[slot].pOrg )
			{
				m_orgArrayInAlly[slot].pOrg->m_iAllyIndex = 0;
			}
			else
			{
				_Organization * pOrg = g_pOrganizationManager->FindOrganization( org );
				pOrg->m_iAllyIndex = 0;
			}

			m_orgArrayInAlly[slot].iOrIndex = 0;
			m_orgArrayInAlly[slot].pOrg = NULL;
			break;
		}
	}

	if( slot == 0 || get_org_count() < 2 )
		return true;		// 해체시킨다.
	else
		return false;		// 그대로.
}


void CAllyOrganization::send_ally_info( _h_DS_Com * pCom, char * toName )
{
	SMSG_ALLY_INFO info;
	int i ;
    
	TRY
	{
		if( toName )
		{
			strncpy( info.cName, toName, en_charac_name_length );
			info.cName[en_charac_name_length] = 0;
		}
		else
		{
			ZeroMemory( info.cName, sizeof(info.cName) );
		}

		info.iAllyIndex = get_index();
		info.sProperty = get_property();
		CopyMemory( info.cMasterName, m_cMasterName, 13 );

		for( i = 0 ; i < _NDD_MAX_ORG_IN_ALLY_ ; ++i )
		{
			if( m_orgArrayInAlly[i].pOrg )
			{
				info.org[i].iOrIndex = m_orgArrayInAlly[i].pOrg->m_iIndex;
				strncpy( info.org[i].cOrName, m_orgArrayInAlly[i].pOrg->m_cOrName, en_or_name_length );
				info.org[i].cOrName[en_or_name_length] = 0;
			}
			else
			{
				ZeroMemory( &info.org[i], sizeof(__ally_client_info) );
			}
		}

		pCom->Send( info.usLength, (char*)&info );
	}
	CATCH_ALL
	{
		g_pSystemLog->LogFileWrite( "[EXCEPTION] send_ally_info\n" );
	}
}

void CAllyOrganization::fill_answer_packet( SMSG_ALLY_ANSWER_INFO& info )
{
	int i ;

	TRY
	{
		info.iAllyIndex = get_index();
		info.sProperty = get_property();
		CopyMemory( info.cMasterName, m_cMasterName, 13 );

		for( i = 0 ; i < _NDD_MAX_ORG_IN_ALLY_ ; ++i )
		{
			if( m_orgArrayInAlly[i].pOrg )
			{
				info.org[i].iOrIndex = m_orgArrayInAlly[i].pOrg->m_iIndex;
				strncpy( info.org[i].cOrName, m_orgArrayInAlly[i].pOrg->m_cOrName, en_or_name_length );
				info.org[i].cOrName[en_or_name_length] = 0;
			}
			else
			{
				ZeroMemory( &info.org[i], sizeof(__ally_client_info) );
			}
		}
	}
	CATCH_ALL
	{
		g_pSystemLog->LogFileWrite( "[EXCEPTION] send_all_ally_info\n" );
	}
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
bool CAllyManager::read_ally_info( COrganizationManager * pOrManager, COLE_DB_Executer * pDB )
{
	if( !pDB )
		return false;

	set_db_exe( pDB );
	if( 0 != m_pGameDB->GetAllyInfo( this, pOrManager ) )
	{
		return false;
	}

	check_initial_status( pOrManager );

	return true;
}

void CAllyManager::copy_and_insert_ally( CAllyOrganization& refAlly )
{
	CAllyOrganization * p = pop();
	if( p )
	{
		p->copy_set( refAlly );
		add_ally( p->get_index(), p );
	}
	else
	{
		g_pSystemLog->LogFileWrite( "[ALLY] Fail insert ally : %d\n", refAlly.get_index() );
	}
}

void CAllyManager::check_initial_status( COrganizationManager * pOrManager )
{
	// 초기 구성된 모든 동맹을 돌며, 
	// 영웅단 정보와 비교하여, 영웅단을 제외시키거나,
	// 동맹을 해체한다.
	// 추가로, 영웅단의 정보도 채운다.

	CAllyOrganization * pAlly = NULL;

	map<int, CAllyOrganization*>::iterator itr = m_mapAlly.begin();

	while( itr != m_mapAlly.end() )
	{
		pAlly = itr->second;

		if( false == pAlly->check_info( pOrManager ) )
		{
			g_pSystemLog->LogFileWrite( "[ALLY] check fail %d\n", pAlly->get_index() );

			// 맵에서 삭제하고, 메모리 반환.
			m_pGameDB->AllyBreak( pAlly->get_index(), 2 );

			pAlly->reset_ally( m_pGameDB );
			
			itr = m_mapAlly.erase( itr );

			push( pAlly );
		}

		itr++;
	}
}


//////////////////////////////////////////////////////////////////////////
// Packet 처리 함수
//////////////////////////////////////////////////////////////////////////
void CAllyManager::pk_request_create( SMSG_ALLY_REQUEST_CREATE * pCreate, _h_DS_Com * pCom )
{
	/* check two organizations.
	// make other information
	// call db procedure.
	// return.		 */
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[ALLY] Recv Create : %d + %d\n", 
		pCreate->iOrg1, pCreate->iOrg2 );
	g_pSystemLog->LogFileWrite( "[ALLY] Recv Create : %d + %d\n", pCreate->iOrg1, pCreate->iOrg2 );

	_Organization * pOrg1 = g_pOrganizationManager->FindOrganization( pCreate->iOrg1 );
	_Organization * pOrg2 = g_pOrganizationManager->FindOrganization( pCreate->iOrg2 );

	if(   m_pGameDB == NULL					|| 
		( NULL == pOrg1 || NULL == pOrg2 )	||
		( 0 != pOrg1->m_iAllyIndex || 0 != pOrg2->m_iAllyIndex ) )
	{
		SMSG_ALLY_CREATE_FAIL fail;
		fail.iOrg1 = pCreate->iOrg1;
		fail.iOrg2 = pCreate->iOrg2;

		strncpy( fail.cName1, pCreate->cName1, en_charac_name_length );
		fail.cName1[en_charac_name_length] = 0;
		strncpy( fail.cName2, pCreate->cName2, en_charac_name_length );
		fail.cName2[en_charac_name_length] = 0;
		pCom->Send( fail.usLength, (char*)&fail );

		g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[ALLY] Create fail : information has error.\n" );
		g_pSystemLog->LogFileWrite( "[ALLY] Create fail : information has error.\n" );
	}
	else
	{
		CAllyOrganization * p = pop();
		if( p )
		{

			// 조건 만족.
			// 각 영웅단의 동맹 인덱스 업데이트는 Create 프로시저 안에서 행한다.
			int allyIndex = m_pGameDB->AllyCreate( pOrg1->m_iIndex, pOrg2->m_iIndex, pOrg1->m_cOrName, g_sMoonpaProperty[pOrg1->m_cBaseParty] );
			if( allyIndex > 0 )
			{
				pOrg1->m_iAllyIndex = allyIndex;
				pOrg2->m_iAllyIndex = allyIndex;

				p->reset_ally( NULL );

				p->set_index( allyIndex );
				p->set_org_info( 0, __org_state_in_ally(pOrg1->m_iIndex, pOrg1) );
				p->set_org_info( 1, __org_state_in_ally(pOrg2->m_iIndex, pOrg2) );
				p->set_property( g_sMoonpaProperty[pOrg1->m_cBaseParty] );
				p->set_master_name( pOrg1->m_cCaptainName );

				add_ally( allyIndex, p );

				SMSG_ALLY_CREATE_SUCCESS pkSuccess;
				pkSuccess.iAllyIndex = allyIndex;
				pkSuccess.iOrg1 = pOrg1->m_iIndex;
				pkSuccess.iOrg2 = pOrg2->m_iIndex;
				pkSuccess.sProperty = p->get_property();

				strncpy( pkSuccess.cOrgName1, pOrg1->m_cOrName, en_or_name_length );
				pkSuccess.cOrgName1[en_or_name_length] = 0;
				strncpy( pkSuccess.cOrgName2, pOrg2->m_cOrName, en_or_name_length );
				pkSuccess.cOrgName2[en_or_name_length] = 0;

				strncpy( pkSuccess.cName1, pCreate->cName1, en_charac_name_length );
				pkSuccess.cName1[en_charac_name_length] = 0;
				strncpy( pkSuccess.cName2, pCreate->cName2, en_charac_name_length );
				pkSuccess.cName2[en_charac_name_length] = 0;

				CopyMemory( pkSuccess.cMasterName, p->get_master_name(), en_charac_name_length );

				pCom->Send( pkSuccess.usLength, (char*)&pkSuccess );

				// 지금 생성되었다고 모든 서버에 알린다.
				SMSG_ALLY_CREATE_NOTICE pkNotice;
				pkNotice.iAllyIndex = allyIndex;
				pkNotice.iOrg1 = pOrg1->m_iIndex;
				pkNotice.iOrg2 = pOrg2->m_iIndex;
				pkNotice.sProperty = p->get_property();
				strncpy( pkNotice.cOrgName1, pOrg1->m_cOrName, en_or_name_length );
				pkNotice.cOrgName1[en_or_name_length] = 0;
				strncpy( pkNotice.cOrgName2, pOrg2->m_cOrName, en_or_name_length );
				pkNotice.cOrgName2[en_or_name_length] = 0;
				CopyMemory( pkNotice.cMasterName, p->get_master_name(), en_charac_name_length );

				g_pDSComQueue->GlobalSend( pkNotice.usLength, (char*)&pkNotice );
				
				g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[ALLY] Create Success %d.\n", allyIndex );
				g_pSystemLog->LogFileWrite( "[ALLY] Create Success %d.\n", allyIndex );
			}		
			else
			{
				push( p );

				SMSG_ALLY_CREATE_FAIL fail;
				fail.iOrg1 = pCreate->iOrg1;
				fail.iOrg2 = pCreate->iOrg2;

				strncpy( fail.cName1, pCreate->cName1, en_charac_name_length );
				fail.cName1[en_charac_name_length] = 0;
				strncpy( fail.cName2, pCreate->cName2, en_charac_name_length );
				fail.cName2[en_charac_name_length] = 0;

				pCom->Send( fail.usLength, (char*)&fail );

				g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[ALLY] Create fail : DB create fail.\n" );
				g_pSystemLog->LogFileWrite( "[ALLY] Create fail : DB create fail.\n" );
			}
		}
		else
		{
			SMSG_ALLY_CREATE_FAIL fail;
			fail.iOrg1 = pCreate->iOrg1;
			fail.iOrg2 = pCreate->iOrg2;

			strncpy( fail.cName1, pCreate->cName1, en_charac_name_length );
			fail.cName1[en_charac_name_length] = 0;
			strncpy( fail.cName2, pCreate->cName2, en_charac_name_length );
			fail.cName2[en_charac_name_length] = 0;

			pCom->Send( fail.usLength, (char*)&fail );

			g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[ALLY] Create fail : Ally pool is exhausted.\n" );
			g_pSystemLog->LogFileWrite( "[ALLY] Create fail : Ally pool is exhausted.\n" );
		}
	}
}


void CAllyManager::pk_request_info( SMSG_ALLY_REQUEST_INFO * pInfo, _h_DS_Com * pCom )
{
	/* find ally information.
	// send information to pCom. */
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[ALLY] Recv : Request info for %d ally ", pInfo->iAllyIndex );
	g_pSystemLog->LogFileWrite( "[ALLY] Recv : Request info for %d ally ", pInfo->iAllyIndex );

	CAllyOrganization * pAlly = find_ally( pInfo->iAllyIndex );
	if( pAlly )
	{
		pAlly->send_ally_info( pCom, pInfo->cName );
		g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Find!!" );
		g_pSystemLog->LogFileWrite( "Find!!" );
	}
	else
	{
		SMSG_ALLY_NO_INFO noInfo;
		strncpy( noInfo.cName, pInfo->cName, en_charac_name_length );
		noInfo.cName[en_charac_name_length] = 0;
		noInfo.iAllyIndex = pInfo->iAllyIndex;
		noInfo.ucError = 0;

		pCom->Send( noInfo.usLength, (char*)&noInfo );

		g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Not Found" );
		g_pSystemLog->LogFileWrite( "Not Found" );
	}
}


void CAllyManager::pk_request_add_org( SMSG_ALLY_REQUEST_ADD_ORG * pAddOrg, _h_DS_Com * pCom )
{
	/* find ally information
	// check ally's organization ( count organization )
	// update DB
	// Send to all gs.	*/
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[ALLY] Recv : Add Org - %d + %d", 
		pAddOrg->iAllyIndex, pAddOrg->iOrgAdd );
	g_pSystemLog->LogFileWrite( "[ALLY] Recv : Add Org - %d + %d", 	pAddOrg->iAllyIndex, pAddOrg->iOrgAdd );

	CAllyOrganization * pAlly = find_ally( pAddOrg->iAllyIndex );
	bool bSuccess = false;

	if( pAlly && m_pGameDB )
	{
		if( pAlly->can_add_org( pAddOrg->iOrgAdd ) )
		{
			_Organization * p = g_pOrganizationManager->FindOrganization( pAddOrg->iOrgAdd );
			if( p )
			{
				if( pAlly->get_property() == g_sMoonpaProperty[p->m_cBaseParty] )
				{
					u_char slot = pAlly->add_org_info( __org_state_in_ally( p->m_iIndex, p ) );
					if( slot < _NDD_MAX_ORG_IN_ALLY_ )
					{
						// Success : Update DB & Send all.
						p->m_iAllyIndex = pAlly->get_index();
						m_pGameDB->AllyUpdateOfOrg( pAddOrg->iOrgAdd, pAlly->get_index() );
						pAlly->db_update( m_pGameDB );

						bSuccess = true;

						SMSG_ALLY_ADD_SUCCESS success;
						success.iAllyIndex = pAddOrg->iAllyIndex;
						success.ucSlot = slot;
						pAlly->get_org_info( slot, &success.info );
						CopyMemory( success.cName, pAddOrg->cName, en_charac_name_length );
						success.cName[en_charac_name_length] = 0;
						CopyMemory( success.cNameAdd, pAddOrg->cNameAdd, en_charac_name_length );
						success.cNameAdd[en_charac_name_length] = 0;

						g_pDSComQueue->GlobalSend( success.usLength, (char*)&success );

						g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Success Add %d org\n", pAddOrg->iOrgAdd );
						g_pSystemLog->LogFileWrite( "Success Add %d org\n", pAddOrg->iOrgAdd );
					}
				}
				else
				{
					g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Fail - Property\n" );
					g_pSystemLog->LogFileWrite( "Fail - Property\n" );
				}
			}
			else
			{
				g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Fail - Not found Org\n" );
				g_pSystemLog->LogFileWrite( "Fail - Not found Org\n" );
			}
		}
		else
		{
			g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Fail - Full\n" );
			g_pSystemLog->LogFileWrite( "Fail - Full\n" );
		}
	}
	else
	{
		g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Fail - not found\n" );
		g_pSystemLog->LogFileWrite( "Fail - not found\n" );
	}

	if( false == bSuccess )
	{
		SMSG_ALLY_ADD_FAIL fail;
		fail.iAllyIndex = pAddOrg->iAllyIndex;
		fail.iOrgAdd = pAddOrg->iOrgAdd;
		fail.ucError = 0;

		pCom->Send( fail.usLength, (char*)&fail );
	}
}

void CAllyManager::pk_request_leave( SMSG_ALLY_REQUEST_LEAVE * pLeave, _h_DS_Com * pCom )
{
	/* find ally information
	// leave organization
	// check ally's organization. ( left organization )
	// update DB
	// send to all gs. */
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[ALLY] Recv : Leave Req. Ally(%d), Org(%d) - ", 
		pLeave->iAllyIndex, pLeave->iOrgLeave );
	g_pSystemLog->LogFileWrite( "[ALLY] Recv : Leave Req. Ally(%d), Org(%d) - ", 
		pLeave->iAllyIndex, pLeave->iOrgLeave );

	CAllyOrganization * pAlly = find_ally( pLeave->iAllyIndex );

	if( pAlly )
	{
		if( pAlly->leave_org(pLeave->iOrgLeave) )
		{
#ifdef _PD_CASTLE_STEP_1_
			CDSCastleManager::GetObj().notice_group_breakup( en_cb_group_type_ally, pLeave->iAllyIndex );
#endif
			// 해체.
			// 1. 우선 탈퇴를 알리고, 
			// 2. 해체를 알린다.
			SMSG_ALLY_LEAVE_SUCCESS pkSuccess;
			pkSuccess.iAllyIndex = pLeave->iAllyIndex;
			pkSuccess.iOrgLeave = pLeave->iOrgLeave;

			g_pDSComQueue->GlobalSend( pkSuccess.usLength, (char*)&pkSuccess );

			// 맵에서 삭제하고, 
			del_ally( pLeave->iAllyIndex );

			pAlly->reset_ally( m_pGameDB );

			// 포인터 반환.
			push( pAlly );

			SMSG_ALLY_BREAKUP_SUCCESS pkBreakup;
			pkBreakup.iAllyIndex = pLeave->iAllyIndex;
			pkBreakup.ucBreakupReason = 1;		// passive breakup

			g_pDSComQueue->GlobalSend( pkBreakup.usLength, (char*)&pkBreakup );

			g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Leave & Scatter\n" );
			g_pSystemLog->LogFileWrite( "Leave & Scatter\n" );

			m_pGameDB->AllyBreak( pLeave->iAllyIndex, 2 );	// passive

		}
		else
		{
			// 탈퇴만 알린다.
			if( m_pGameDB )
				m_pGameDB->AllyUpdateOfOrg( pLeave->iOrgLeave, 0 );	// 나가는 거니까 0 으로.

			pAlly->db_update( m_pGameDB );

			SMSG_ALLY_LEAVE_SUCCESS pkSuccess;
			pkSuccess.iAllyIndex = pLeave->iAllyIndex;
			pkSuccess.iOrgLeave = pLeave->iOrgLeave;

			g_pDSComQueue->GlobalSend( pkSuccess.usLength, (char*)&pkSuccess );

			g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Leave\n" );
			g_pSystemLog->LogFileWrite( "Leave\n" );
		}
	}
	else
	{
		g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "not found ally.\n" );
		g_pSystemLog->LogFileWrite( "not found ally.\n" );
	}
}

void CAllyManager::pk_request_breakup( SMSG_ALLY_REQUEST_BREAKUP * pBreakup, _h_DS_Com * pCom )
{
	/* fins ally information
	// break up ally.
	// update db
	// send to all gs. */
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[ALLY] Recv : Breakup Req. Ally(%d) - ", pBreakup->iAllyIndex );
	g_pSystemLog->LogFileWrite( "[ALLY] Recv : Breakup Req. Ally(%d) - ", pBreakup->iAllyIndex );

	CAllyOrganization * pAlly = find_ally( pBreakup->iAllyIndex );

	if( pAlly )
	{
		// 맵에서 삭제하고, 
		del_ally( pBreakup->iAllyIndex );

		pAlly->reset_ally( m_pGameDB );		// 내부 영웅단들의 동맹 인덱스를 0으로 해야 한다. m_pGameDB.

		// 포인터 반환.
		push( pAlly );

#ifdef _PD_CASTLE_STEP_1_
		CDSCastleManager::GetObj().notice_group_breakup( en_cb_group_type_ally, pBreakup->iAllyIndex );
#endif

		SMSG_ALLY_BREAKUP_SUCCESS pkBreakup;
		pkBreakup.iAllyIndex = pBreakup->iAllyIndex;
		pkBreakup.ucBreakupReason = 0;		// active breakup

		g_pDSComQueue->GlobalSend( pkBreakup.usLength, (char*)&pkBreakup );

		g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Scatter\n" );
		g_pSystemLog->LogFileWrite( "Scatter\n" );

		m_pGameDB->AllyBreak( pBreakup->iAllyIndex, 1 );
	}
	else
	{
		g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "not found ally.\n" );
		g_pSystemLog->LogFileWrite( "not found ally.\n" );
	}
}

void CAllyManager::pk_request_banish( SMSG_ALLY_REQUEST_BANISH * pBanish, _h_DS_Com * pCom )
{
	/* find ally information
	// leave organization
	// check ally's organization. ( left organization )
	// update DB
	// send to all gs. */
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[ALLY] Recv : Banish Req. Ally(%d), Org(%d) - ", 
		pBanish->iAllyIndex, pBanish->iOrgIndex );
	g_pSystemLog->LogFileWrite( "[ALLY] Recv : Banish Req. Ally(%d), Org(%d) - ", 
		pBanish->iAllyIndex, pBanish->iOrgIndex );

	CAllyOrganization * pAlly = find_ally( pBanish->iAllyIndex );

	if( pAlly )
	{
		if( pAlly->leave_org(pBanish->iOrgIndex) )
		{
#ifdef _PD_CASTLE_STEP_1_
			CDSCastleManager::GetObj().notice_group_breakup( en_cb_group_type_ally, pBanish->iAllyIndex );
#endif

			// 해체.
			// 1. 우선 추방을 알리고,
			// 2. 해체를 알린다.
			SMSG_ALLY_BANISH_SUCCESS pkSuccess;
			pkSuccess.iAllyIndex = pBanish->iAllyIndex;
			pkSuccess.iOrgIndex = pBanish->iOrgIndex;
			CopyMemory( pkSuccess.cName, pBanish->cName, en_charac_name_length );
			pkSuccess.cName[en_charac_name_length] = 0;

			g_pDSComQueue->GlobalSend( pkSuccess.usLength, (char*)&pkSuccess );

			// 맵에서 삭제하고, 
			del_ally( pBanish->iAllyIndex );

			pAlly->reset_ally( m_pGameDB );

			// 포인터 반환.
			push( pAlly );

			SMSG_ALLY_BREAKUP_SUCCESS pkBreakup;
			pkBreakup.iAllyIndex = pBanish->iAllyIndex;
			pkBreakup.ucBreakupReason = 1;		// passive breakup

			g_pDSComQueue->GlobalSend( pkBreakup.usLength, (char*)&pkBreakup );

			g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Banish & Scatter\n" );
			g_pSystemLog->LogFileWrite( "Banish & Scatter\n" );

			m_pGameDB->AllyBreak( pBanish->iAllyIndex, 2 );	// passive

		}
		else
		{
			// 탈퇴만 알린다.
			if( m_pGameDB )
				m_pGameDB->AllyUpdateOfOrg( pBanish->iOrgIndex, 0 );	// 나가는 거니까 0 으로.

			pAlly->db_update( m_pGameDB );

			SMSG_ALLY_LEAVE_SUCCESS pkSuccess;
			pkSuccess.iAllyIndex = pBanish->iAllyIndex;
			pkSuccess.iOrgLeave = pBanish->iOrgIndex;

			g_pDSComQueue->GlobalSend( pkSuccess.usLength, (char*)&pkSuccess );

			g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "Banish\n" );
			g_pSystemLog->LogFileWrite( "Banish\n" );
		}
	}
	else
	{
		g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "not found ally.\n" );
		g_pSystemLog->LogFileWrite( "not found ally.\n" );
	}
}


#ifdef _PD_CASTLE_STEP_2_
void CAllyManager::pk_request_all_ally( _h_DS_Com * pCom )
{
	// 모든 영웅단의 정보를 준다. 
	SMSG_ALLY_ANSWER_INFO pkInfo;

	PrintConsole( "[ALLY] Send All Ally Information \n" );

	CAllyOrganization * pAlly = NULL;
	int i = 0;

	LockMap();

	try
	{
		if( m_mapAlly.empty() )
		{
			pkInfo.iAllyIndex = 0;
			pkInfo.ucContinueInfo = 0;		// end
			pCom->Send( pkInfo.usLength, (char*)&pkInfo );
		}
		else
		{
			map<int, CAllyOrganization*>::iterator itr = m_mapAlly.begin();

			

			while( itr != m_mapAlly.end() )
			{
				pAlly = itr->second;
				pAlly->fill_answer_packet( pkInfo );

				++itr;

				if( itr != m_mapAlly.end() )
					pkInfo.ucContinueInfo = 1;		// continue
				else
					pkInfo.ucContinueInfo = 0;		// end

				pCom->Send( pkInfo.usLength, (char*)&pkInfo );
			}
		}
	}
	catch ( ... ) 
	{
	}

	UnlockMap();

	/*
	info.iAllyIndex = get_index();
	info.sProperty = get_property();
	CopyMemory( info.cMasterName, m_cMasterName, 13 );

	for( i = 0 ; i < _NDD_MAX_ORG_IN_ALLY_ ; ++i )
	{
	if( m_orgArrayInAlly[i].pOrg )
	{
	info.org[i].iOrIndex = m_orgArrayInAlly[i].pOrg->m_iIndex;
	strncpy( info.org[i].cOrName, m_orgArrayInAlly[i].pOrg->m_cOrName, en_or_name_length );
	info.org[i].cOrName[en_or_name_length] = 0;
	}
	else
	{
	ZeroMemory( &info.org[i], sizeof(__ally_client_info) );
	}
	}
	*/
}
#endif

void CAllyManager::show_all_ally()
{
	LockMap();

	try
	{
		printf( "\n ** Ally List **\n" );
		CAllyOrganization * pAlly = NULL;
		map<int, CAllyOrganization*>::iterator itr = m_mapAlly.begin();

		while( itr != m_mapAlly.end() )
		{
			pAlly = itr->second;

			printf( " %4d : Main <%4d : %s>\n", pAlly->get_index(), 
				pAlly->m_orgArrayInAlly[0].iOrIndex, pAlly->m_orgArrayInAlly[0].pOrg->m_cOrName );
			++itr;
		}
	}
	catch ( ... ) 
	{
	}


	UnlockMap();
}

#endif


