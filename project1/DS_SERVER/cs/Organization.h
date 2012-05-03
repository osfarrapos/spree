#pragma once
#include <map>


using namespace std ;


#define _EMPTY_UNIT_SLOT_			"\0\0\0\0\0\0\0\0\0\0\0\0\0"
#define _ORGANIZATION_INIT_SIZE_	10000
#define _MAX_OR_UNIT_NUM	100
#define _SIZE_OF_OR_MEM_LIST_DATA	sizeof(_Unit) * _MAX_OR_UNIT_NUM
#define _SIZE_OF_OR_OUTTIME_LIST_DATA	sizeof(_OutTime) * _MAX_OR_UNIT_NUM

// 조직의 구성 유닛..

#pragma pack( push, enter_organization )
#pragma pack(1)


class _Unit
{
public:	
	char			m_cName[13] ;	
	bool			m_bOnline ;
	char			m_cClass ;		
	enum{
		_UNIT_CLASS_DEFAULT_UNIT_,			// 기본 조직원
		_UNIT_CLASS_SPECIAL_UNIT_,			// 상급 조직원
		_UNIT_CALSS_CAPTAIN_UNIT_,			// 절대 지휘자..
	} ;
	short			m_sInnerLevel ;
	char			m_cZone ;


	_Unit & _Unit::operator=( const _Unit& rhs ) 
	{
		if( this == &rhs ) { return *this ; }

		memcpy( this, &rhs, sizeof(_Unit) ) ;
		return *this ;
	}

	inline void reset() { memset(this, 0, sizeof(_Unit) ) ; }
};


struct _OutTime
{
	u_char	year ;
	u_char	month ;
	u_char	day ;
	u_char	h ;
	u_char  m ;
	u_char	s ;
	
	_OutTime & _OutTime::operator=( const _OutTime & rhs )
	{
		if( this == &rhs ) { return *this ; }
		memcpy( this, &rhs, sizeof( _OutTime ) ) ;
		return *this ;
	}

} ;


// 조직의 기본 데이타다.. 패킷으로 보낼때 이런 구조체 단위로 보내면 처리하기 편리하겠다..
#define _OR_NOTICE_SIZE		300
class _OrganizationBase
{
public:
	int				m_iIndex ;
	char			m_cOrName[en_or_name_length+1] ;			// 조직 이름
	char			m_cProperty ;			// 조직 속성(분파 | 방회)
	char			m_cBaseParty ;			// 소속 문파
	u_int			m_uiExperience ;		// 조직 경험치
	u_int			m_uiContribution ;		// 단 기여도~로 변경.<-조직의 문파 기여도 : 기여도 할때 더 찾아보고 단 기여도로 변신시키기 - `07.07.24
	u_short			m_usCurMemberNum ;		// 현재 인원	
	char			m_cCaptainName[13] ;
#ifdef	_ORG_PROMOTION_  //_OrganizationBase 에 변수 추가
	char			m_org_level;			//현재 단의 레벨(1 ~ 5)
	//u_int			m_contribution;			// 단 기여도 위 문파기여도를 단 기여도로 바꿈 테스트후 삭제	
	DWORD			m_dwBloodPoint_Buf_Time;// 혈정을 기부하여 버프를 받을 수 있는 시간 
	_sND_TimeUnit	m_ally_withdraw_time;	// 회에서 나온 시간 이 시간으로 부터 4일이 지나야 한다.
#endif//_ORG_PROMOTION_ 

	_OrganizationBase & _OrganizationBase::operator=( const _OrganizationBase& rhs ) 
	{
		try
		{
			if( this == &rhs ) { return *this ; }

			m_iIndex = rhs.m_iIndex ;
			memcpy( m_cOrName, rhs.m_cOrName, sizeof(m_cOrName) ) ;	// 조직 이름
			m_cProperty = rhs.m_cProperty ;							// 조직 속성(분파 | 방회)
			m_cBaseParty = rhs.m_cBaseParty ;						// 소속 문파
			m_uiExperience = rhs.m_uiExperience ;					// 조직 경험치
			m_uiContribution = rhs.m_uiContribution ;				// 조직의 문파 기여도
			m_usCurMemberNum = rhs.m_usCurMemberNum ;				// 현재 인원	
			memcpy( m_cCaptainName, rhs.m_cCaptainName, sizeof( m_cCaptainName ) ) ;
#ifdef	_ORG_PROMOTION_ 
			m_iOrg_level			 = rhs.miOrg_level;
			//m_contribution			 = rhs.m_contribution;
			m_dwBloodPoint_Buf_Time	 = rhs.m_dwBloodPoint_Buf_Time;
			memcpy(&m_ally_withdraw_time,rhs.m_ally_disband_time,sizeof(_sND_TimeUnit));
#endif//_ORG_PROMOTION_ 
		}
		catch(...)
		{
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}
		
		return *this ;
	}	
};

class _Organization : public _OrganizationBase
{
public :
	_Unit				m_MemberList[_MAX_OR_UNIT_NUM] ;			// 조직 명단..

	char				m_cNotice[_OR_NOTICE_SIZE] ;
	_OutTime			m_outTime[_MAX_OR_UNIT_NUM] ;

#ifdef _PD_ALLY_ORG_STEP_1_
	int					m_iAllyIndex;
#endif

	inline void SetOutTime( const int slot, const _OutTime& outTime )
	{
		if( slot >= 0 && slot < _MAX_OR_UNIT_NUM )
			m_outTime[slot] = outTime ;
	}


	_Organization & _Organization::operator=( const _Organization& rhs ) 
	{
		try
		{
			if( this == &rhs ) { return *this ; }

			_OrganizationBase::operator = ( rhs ) ;

			memcpy( m_MemberList, rhs.m_MemberList, sizeof( m_MemberList ) ) ;
			memcpy( m_cNotice, rhs.m_cNotice, sizeof( m_cNotice ) ) ;
			memcpy( m_outTime, rhs.m_outTime, sizeof( m_outTime ) ) ;

#ifdef _PD_ALLY_ORG_STEP_1_
			m_iAllyIndex = rhs.m_iAllyIndex;
#endif
			
		}
		catch(...)
		{
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}
		
		return *this ;
	}

	

	inline _Unit * FindUnit( const char * name, OUT int * pSlotNum = NULL ) 
	{		
		_Unit * pUnit = NULL ;
		try
		{
			for( int i = 0 ; i < _MAX_OR_UNIT_NUM ; i++ )
			{
				if( m_MemberList[i].m_cName[0] == name[0] )
				{
					if( strcmp( m_MemberList[i].m_cName, name ) == 0 )
					{
						pUnit = &m_MemberList[i] ;
						if( pSlotNum )
						{
							*pSlotNum = i ;
						}
						break ;
					}					
				}			
			}
		}
		catch(...)
		{
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}	

		return pUnit ;
	}
	#ifdef	_ORG_PROMOTION_
	//해체시간을 기록하기 위해 단장을 찾아라~!!!
	inline _Unit * FindCaptain()
	{
		_Unit * pUnit = NULL ;
		try
		{
			for( int i = 0 ; i < _MAX_OR_UNIT_NUM ; i++ )
			{
				if( m_MemberList[i].m_cClass == _Unit::_UNIT_CALSS_CAPTAIN_UNIT_ )
				{
					pUnit = &m_MemberList[i] ;
					if( pSlotNum )
					{
						*pSlotNum = i ;
					}
					break ;
					}					
				}			
			}
		}
		catch(...)
		{
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}	

		return pUnit ;
	}


	#endif//_ORG_PROMOTION_

};

//en_ds_ally_info,					// 4 위 정보 요청의 응답.
struct __ally_client_info
{
	int				iOrIndex;
	char			cOrName[en_or_name_length+1];
};


#pragma pack( pop, enter_organization )


class COLE_DB_Executer ;
class COrganizationManager 
{
private :	
	typedef map<int,_Organization*> ORMap ;
	size_t							m_InitSize ;
	_Organization *					m_pOrganization ;

	COLE_DB_Executer *				m_pGameDB ;

	ORMap							m_MapOR ;
	CSAFE_Vector<_Organization* >	m_ORPool ;	
	CTraceAbleLock					m_or_cs ;

#define or_lock()					m_or_cs.tlock( __FILE__, __LINE__ ) ;
#define or_unlock()					m_or_cs.tunlock( __FILE__, __LINE__ ) ;
	
	
public :
	COrganizationManager( size_t size ) ;
	~COrganizationManager() ;

	// Manager part
	bool ReadORDataFromDB( COLE_DB_Executer * GameDB ) ;
#ifdef _PD_ALLY_ORG_STEP_1_
	void CheckInitialAlly( COLE_DB_Executer * GameDB );
#endif
	const int CreateOR( IN OUT  _Organization & or ) ;			// 새로운 Organization을 생성하고 OR Index 반환
	bool InsertOR( const _Organization & or ) ;			// 맵에 삽입만 한다.
	bool DeleteOR( const int index ) ;							// Index로 찾아서 삭제	
	_Organization * GetORInfo( const int index );

	// OR Part
	bool PutORUnit( const int index, const _Unit & unit ) ;	
	int PopORUnit( const int index, const char * name ) ;	// -1 : 파산, 0: 실패, 1:성공

	bool UpdateORUnit( const int index, const _Unit & unit, const _OutTime & outTime ) ;
	bool SetUnitClass( const int index, const char * name, const char Class ) ;
	bool SetORExp( const  int index, const u_int exp) ;	
	bool ExchangeCaptain( const int index, const char * name) ;
	bool BaseUpdate( const  int index, const _OrganizationBase & or_base  ) ;	
	bool DBUpdate( const _Organization * pOr ) ;	
	inline _Organization * FindOrganization( const int index ) 
	{
		_Organization * pOR = NULL ;
		or_lock() ;
		try
		{
			ORMap::iterator OrMapItr =  m_MapOR.find( index ) ;
			if( OrMapItr != m_MapOR.end() )
			{
				pOR = OrMapItr->second ;
			}
		}
		catch (...)
		{
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}		
		or_unlock() ;

		return pOR ;
	}

	inline _Unit * FindUnit( _Organization * const pOr, const char * name, OUT int * pSlotNum = NULL ) 
	{		
		
		if( NULL == pOr ) return NULL ;
		_Unit * pUnit = pOr->FindUnit( name, pSlotNum ) ;		
		return pUnit ;
	}	

	inline bool IsUsingName( const char * name )
	{
		bool bRet = false ;
		or_lock() ;
		try
		{
			ORMap::iterator OrMapItr =  m_MapOR.begin() ;	
			_Organization * pOR ;
			while ( OrMapItr != m_MapOR.end() )
			{		
				pOR = OrMapItr->second ;
				if( name[0] == pOR->m_cOrName[0] )
				{
					if( strcmp( name, pOR->m_cOrName) == 0 )
					{
						bRet = true ;
						break ;
					}
				}
				++OrMapItr ;
			}
		}
		catch(...)
		{
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}		
		or_unlock() ;

		return bRet ;
	}

	void ShowAllOrganization();
};
	

#ifdef _PD_ALLY_ORG_STEP_1_

#include "..\Global\tcp_packets_party.h"

struct __org_state_in_ally
{
	int					iOrIndex;		// 영웅단 인덱스.
	_Organization *		pOrg;			// 정보가 있다면, 이 것으로 쓰는데, 진짜로 쓸지는 확정되지 않았음.

	__org_state_in_ally()
		: iOrIndex(0), pOrg(NULL)
	{}
	__org_state_in_ally( int idx, _Organization * p )
		: iOrIndex(idx), pOrg(p)
	{}
};

class CAllyOrganization 
{
public:
	enum	// 동맹의 상태 : m_iAllyState
	{
		en_ally_state_not_used=0,		// 사용되지 않는 상태.
		en_ally_state_ready_ally,		// 결성을 위해 준비준인 상태
		en_ally_state_made_ally,		// 결성이 완료된 상태.
	};

public:
	CAllyOrganization()
	{
		reset_ally( NULL );
		m_bInPool = false;
	}

	void reset_ally( COLE_DB_Executer * pDB );

	void copy_set( CAllyOrganization & refAlly )
	{
		set_index( refAlly.get_index() );
		CopyMemory( m_orgArrayInAlly, refAlly.m_orgArrayInAlly, sizeof(m_orgArrayInAlly) );
		m_sProperty = refAlly.get_property();
		CopyMemory( m_cMasterName, refAlly.m_cMasterName, sizeof(m_cMasterName) );
	}

	// 자신의 상태를 체크하여, 잘못된 정보가 있다면 삭제하거나, 
	// 동맹이 해체될 수 있다.
	bool check_info( COrganizationManager * pOrManager );
	//////////////////////////////////////////////////////////////////////////
	inline int get_index()					{ return m_iIndex; }
	inline void set_index( int index )		{ m_iIndex = index; }
	inline short get_property()				{ return m_sProperty; }
	inline void set_property( short pr )	{ m_sProperty = pr; }

	inline void set_master_name( char * name )	
	{
		CopyMemory( m_cMasterName, name, en_charac_name_length+1 );
		m_cMasterName[en_charac_name_length] = 0;
	}

	int get_org_index( int slot )
	{
		return m_orgArrayInAlly[slot].iOrIndex;;
	}

	inline char * get_master_name()	{ return m_cMasterName; }

	// 가입된 영웅단의 수를 리턴한다.
	int	get_org_count()
	{
		int n = 0;
		if( m_orgArrayInAlly[0].iOrIndex )		++n;
		if( m_orgArrayInAlly[1].iOrIndex )		++n;
		if( m_orgArrayInAlly[2].iOrIndex )		++n;
		if( m_orgArrayInAlly[3].iOrIndex )		++n;
		if( m_orgArrayInAlly[4].iOrIndex )		++n;
		return n;		
	}
	bool org_is_in_ally( int org )
	{
		if( org == m_orgArrayInAlly[0].iOrIndex ||
			org == m_orgArrayInAlly[1].iOrIndex ||
            org == m_orgArrayInAlly[2].iOrIndex ||
            org == m_orgArrayInAlly[3].iOrIndex ||
            org == m_orgArrayInAlly[4].iOrIndex )
			return true;
		else 
			return false;
	}

	// 더 가입할 수 있는가.
	bool can_add_org( int org )
	{
		if( get_org_count() < _NDD_MAX_ORG_IN_ALLY_ && false == org_is_in_ally( org ) )
			return true;
		else 
			return false;
	}

	bool leave_org( int org );		// true : 해체, false 그대로.

	u_char add_org_info( __org_state_in_ally& refOrg )
	{
		u_char slot = 1;
		for( ; slot < _NDD_MAX_ORG_IN_ALLY_ ; ++slot )
		{
			if( 0 == m_orgArrayInAlly[slot].iOrIndex )
				break;
		}

		if( slot < _NDD_MAX_ORG_IN_ALLY_ )
		{
			CopyMemory( &m_orgArrayInAlly[slot], &refOrg, sizeof(__org_state_in_ally) );
		}

		return slot;
	}

	void db_update( COLE_DB_Executer * pDB );

	void get_org_info( u_char slot, __ally_client_info * pinfo  )
	{
		if( slot < _NDD_MAX_ORG_IN_ALLY_ )
		{
			pinfo->iOrIndex = m_orgArrayInAlly[slot].iOrIndex;
			if( m_orgArrayInAlly[slot].pOrg )
				strncpy( pinfo->cOrName, m_orgArrayInAlly[slot].pOrg->m_cOrName, en_or_name_length );
			else
				ZeroMemory( pinfo->cOrName, sizeof(pinfo->cOrName) );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 초기화시 불리는 함수들.
	// org_info 를 셋팅한다.
	void set_org_info( int slot, __org_state_in_ally & refOrg )
	{
		if( slot < _NDD_MAX_ORG_IN_ALLY_ )
		{
			CopyMemory( &m_orgArrayInAlly[slot], &refOrg, sizeof(__org_state_in_ally) );
		}
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 해당 서버에 동맹 정보를 보내준다.
	void send_ally_info( _h_DS_Com * pCom, char * toName );
	void fill_answer_packet( SMSG_ALLY_ANSWER_INFO& info );

protected:
	int					m_iIndex;
	__org_state_in_ally	m_orgArrayInAlly[_NDD_MAX_ORG_IN_ALLY_];
	short				m_sProperty;		//
	char				m_cMasterName[13];

public:
	friend class CAllyManager;

	//////////////////////////////////////////////////////////////////////////
	// Pool 관련 함수.
	//////////////////////////////////////////////////////////////////////////
public:
	// 아래 3개의 함수는 Manager 의 풀 관리에 쓰인다. 
	bool is_in_pool()		{ return m_bInPool; }
	void set_in_pool()		{ m_bInPool = true; }
	void reset_in_pool()	{ m_bInPool = false; }

protected:
	bool		m_bInPool;
};


// 처음 풀의 개수는 약 50개 정도로 하자. 그정도면 충분하다.
class CAllyManager
{
public:
	CAllyManager( int poolCount )
		: m_poolTotalCount(0)
	{
		if( FALSE == InitializeCriticalSectionAndSpinCount( &m_csPool, 0x80000100 ) )
			throw "(AllyManager) Create Critical Section Fail";

		if( FALSE == InitializeCriticalSectionAndSpinCount( &m_csMap, 0x80000100 ) )
			throw "(AllyManager) Create Critical Section Fail";

		CAllyOrganization * pAlly = NULL;

		for( int i = 0 ; i < poolCount ; ++i )
		{
			pAlly = new CAllyOrganization;
			if( pAlly )
			{
				push( pAlly );
				++m_poolTotalCount;
			}
		}

		m_pGameDB = NULL;
	}

	~CAllyManager()
	{
		LockPool();

		CAllyOrganization * p = NULL;
		list<CAllyOrganization*>::iterator itr = m_AllyPool.begin();
		while( itr != m_AllyPool.end() )
		{
			try
			{
				p = *itr;
				++itr;
				delete p;
			}
			catch(...)
			{
			}
		}

		UnlockPool();

		DeleteCriticalSection( &m_csPool );
	}
	//////////////////////////////////////////////////////////////////////////
	// read_ally_info( COLE_DB_Executer );
	//////////////////////////////////////////////////////////////////////////
	// 초기에는 DB에서 동맹 정보를 읽은 후, ( read_ally_info() )
	// DB 데이터를 기본으로 동맹 정보를 구성하고. ( insert_ally
	// 각 동맹을 돌며, 잘못된 동맹 정보는 제외시킨다.( check_initial_status() )
	bool read_ally_info( COrganizationManager * pOrManager, COLE_DB_Executer * pDB );
	void copy_and_insert_ally( CAllyOrganization& refAlly );
	void check_initial_status( COrganizationManager * pOrManager );
	void set_db_exe( COLE_DB_Executer * pDB )	{ m_pGameDB = pDB; }

	//////////////////////////////////////////////////////////////////////////
	// Packet 처리 함수
	//////////////////////////////////////////////////////////////////////////
	void pk_request_create( SMSG_ALLY_REQUEST_CREATE * pCreate, _h_DS_Com * pCom );
	void pk_request_info( SMSG_ALLY_REQUEST_INFO * pInfo, _h_DS_Com * pCom );
	void pk_request_add_org( SMSG_ALLY_REQUEST_ADD_ORG * pAddOrg, _h_DS_Com * pCom );
	void pk_request_leave( SMSG_ALLY_REQUEST_LEAVE * pLeave, _h_DS_Com * pCom );
	void pk_request_breakup( SMSG_ALLY_REQUEST_BREAKUP * pBreakup, _h_DS_Com * pCom );
	void pk_request_banish( SMSG_ALLY_REQUEST_BANISH * pBanish, _h_DS_Com * pCom );
#ifdef _PD_CASTLE_STEP_2_
	void pk_request_all_ally( _h_DS_Com * pCom );
#endif
	//////////////////////////////////////////////////////////////////////////

	void show_all_ally();

public:
	inline void LockMap()		{ EnterCriticalSection(&m_csMap); }
	inline void UnlockMap()		{ LeaveCriticalSection(&m_csMap); }

	CAllyOrganization * find_ally( int index )
	{
		CAllyOrganization * p = NULL;
		LockMap();
		try
		{
			map<int, CAllyOrganization *>::iterator itr = m_mapAlly.find( index );
			if( itr != m_mapAlly.end() )
				p = itr->second;
			else
				p = NULL;
		}
		catch( ... )
		{
		}
		UnlockMap();
		return p;
	}

	int add_ally( int index, CAllyOrganization * p )
	{
		LockMap();
		if( index && p )
			m_mapAlly[index] = p;
		UnlockMap();

		return index;
	}

	void del_ally( int index )
	{
		LockMap();
		try
		{
			m_mapAlly.erase( index );
		}
		catch ( ... ) 
		{
		}
		UnlockMap();
	}
	

protected:
	CRITICAL_SECTION				m_csMap;

	map<int, CAllyOrganization *>	m_mapAlly;

	COLE_DB_Executer *				m_pGameDB;


//////////////////////////////////////////////////////////////////////////
public:
	//////////////////////////////////////////////////////////////////////////
	/// Pool //////////////////////////////////////////////////////////////////////////
	inline void LockPool()		{ EnterCriticalSection(&m_csPool); }
	inline void UnlockPool()	{ LeaveCriticalSection(&m_csPool); }

	void push( CAllyOrganization * p )
	{
		LockPool();
		if( !p->is_in_pool() )
		{
			p->set_in_pool();
			m_AllyPool.push_back( p );
		}
		UnlockPool();
	}

	CAllyOrganization * pop()
	{
		CAllyOrganization * p = NULL;
		LockPool();

		if( m_AllyPool.empty() )
		{
			p = new CAllyOrganization;
			++m_poolTotalCount;
		}
		else
		{
			p = m_AllyPool.front();
			m_AllyPool.pop_front();
		}

		if( p )
			p->reset_in_pool();

		UnlockPool();

		return p;
	}

	int get_pool_count()		{ return m_poolTotalCount; }
	int get_using_pool_count()	{ return m_poolTotalCount - (int)m_AllyPool.size(); }
	void print_status()	
	{
		printf( "Ally Pool : Total = %d, Using = %d\n", get_pool_count(), get_using_pool_count() );
	}

protected:
	list<CAllyOrganization*>	m_AllyPool;
	CRITICAL_SECTION			m_csPool;
	int							m_poolTotalCount;
	// Pool End //////////////////////////////////////////////////////////////////////////

};


#endif