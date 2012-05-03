#pragma once

#include "../MyLibrary/NDUtilClass.h"

#ifdef _PD_MASTER_PUPIL_


class CMP_PupilPosRecalc
{
public:
	CMP_PupilPosRecalc()
	{
		Reset();
	}

	struct __temp_mp_rank_calc
	{
		u_int	point;
		u_char  rank;
		u_char  arr_idx;
		u_char	position;
		u_char  temp;
	};

public:
	void Reset()
	{
		m_count = 0;
		ZeroMemory( m_pupil, sizeof(m_pupil) );
	}

	int size()		{ return m_count; }

	void push_point( u_int point, u_char rank, u_char arr_idx );
	void recalc_pos();

	u_int get_point( int index )
	{
		int point = 0;

		if( index < NDD_MP_MAX_PUPIL_COUNT )
			point = m_pupil[index].point;

		return point;
	}

	void Print()
	{
		printf( "==== Recalc Object ====\n" );
		printf( " Count = %d\n", size() );
		for( int i = 0; i < size(); ++i )
		{
			printf( "Seq : %u, Rank %u, Point %u\n", m_pupil[i].arr_idx, m_pupil[i].rank, m_pupil[i].point );
		}
	}

protected:
	int						m_count;
	__temp_mp_rank_calc		m_pupil[NDD_MP_MAX_PUPIL_COUNT];
};



// 구조체의 경우, 패킷으로 사용하는 것은 cspk 를 붙인다.
// 클라이언트와 통신하는 경우, clpk 를 붙인다.

class CDSMaster
{
public:
	CDSMaster()
		: m_iIndex(0), m_uiTotalKillCount(0),m_uiMasterKillCount(0), m_ucBonusSkillStep(0)
	{
	}

	void Reset()
	{
		ZeroMemory( this, sizeof(CDSMaster) );
	}

	void ShowInfo();

	void		InitByDefault( char * master_name );

	void		SetIndex( int index )				{ m_iIndex = index; }
	void		SetTotalKillCount( u_int count )	{ m_uiTotalKillCount = count; }
	void		SetMasterKillCount( u_int count )	{ m_uiMasterKillCount = count; }
	void		SetMasterRank( u_char rank )		{ m_ucMasterRank = rank; }
	void		SetTotalPoint( u_int point )		{ m_uiMasterTotalPoint = point; }
	void		SetCurPoint( u_int point )			{ m_uiMasterCurPoint = point; }
	
	void		SetMasterLevel( u_char lv)			{ m_ucMasterLevel = lv; }
	void		SetMasterSex( u_char sex )			{ m_ucMasterSex = sex; }
	void		SetMasterMoonpa( u_char moonpa )	{ m_ucMasterMoonpa = moonpa; }
	void		SetMasterClass( u_char cls )		{ m_ucMasterClass = cls; }
	void		SetMasterGrade( u_char grade )		{ m_ucMasterGrade = grade; }
	void		SetMasterBonusSkillStep( u_char step )	{ m_ucBonusSkillStep = step; }

	void		SetMasterName(char * name )	
	{
		if( name )
		{
			strncpy( m_cMasterName, name, en_charac_name_length );
			m_cMasterName[en_charac_name_length] = 0;
		}	
		else
			ZeroMemory( m_cMasterName, en_charac_name_length+1 );
	}
	void		SetMasterNick( char * nick )
	{
		if( nick )
		{
			strncpy( m_cMasterNick, nick, NDD_MP_MASTERNICK_LENGTH );
			m_cMasterNick[NDD_MP_MASTERNICK_LENGTH] = 0;
		}
		else
			ZeroMemory( m_cMasterNick, NDD_MP_MASTERNICK_LENGTH+1 );
	}

	void		SetCreateDate( DBTIMESTAMP& create_date )
	{
		m_timeCreateDate.uiYear		= create_date.year - 2000;
		m_timeCreateDate.uiMonth	= create_date.month;
		m_timeCreateDate.uiDay		= create_date.day;
		m_timeCreateDate.uiHour		= create_date.hour;
		m_timeCreateDate.uiMinute	= create_date.hour;
		m_timeCreateDate.uiSecond	= create_date.second;

	}
	void		SetLastConDate( DBTIMESTAMP&  con_date )
	{
		m_timeLastCon.uiYear	= con_date.year - 2000;
		m_timeLastCon.uiMonth	= con_date.month;
		m_timeLastCon.uiDay		= con_date.day;
		m_timeLastCon.uiHour	= con_date.hour;
		m_timeLastCon.uiMinute	= con_date.hour;
		m_timeLastCon.uiSecond	= con_date.second;
	}
	void		UpdateLastConDate()		// 현재 시간( systemtime 으로 업데이트 )
	{
		time_t cur;
		time(&cur);
		tm * ptm = localtime( &cur );

		m_timeLastCon.uiYear		= ptm->tm_year-100;
		m_timeLastCon.uiMonth		= ptm->tm_mon+1;
		m_timeLastCon.uiDay			= ptm->tm_mday;
		m_timeLastCon.uiHour		= ptm->tm_hour;
		m_timeLastCon.uiMinute		= ptm->tm_min;
		m_timeLastCon.uiSecond		= ptm->tm_sec;
	}
	

	void		AddPupilFromDB( _sCSPK_Pupil& pupil );		// pupil 로 부터 추가한다.
	
    int			GetIndex()							{ return m_iIndex; }
	u_int		GetTotalKillCount()					{ return m_uiTotalKillCount; }
	u_int		GetMasterKillCount()				{ return m_uiMasterKillCount; }
	u_char		GetMasterRank()						{ return m_ucMasterRank; }
	u_char		IncreaseMasterRank()
	{
		++m_ucMasterRank;
		m_ucMasterRank = min( m_ucMasterRank, NDD_MP_MAX_MASTER_LEVEL );
		return m_ucMasterRank;
	}

	u_char		GetBonusSkillStep()					{ return m_ucBonusSkillStep; }
	u_char		GetMasterConnectState()				{ return m_ucMasterConnectState; }

	bool		GetApplyEffect()					{ return m_bApplyMasterEffect; }

	u_int		GetTotalPoint()		{ return m_uiMasterTotalPoint;	}
	u_int		GetCurPoint()		{ return m_uiMasterCurPoint;	}
	char *		GetMasterName()		{ return m_cMasterName;			}
	char *		GetMasterNick()		{ return m_cMasterNick;			}

	u_char GetMasterSex()			{ return m_ucMasterSex; }
	u_char GetMasterMoonpa()		{ return m_ucMasterMoonpa; }
	u_char GetMasterClass()			{ return m_ucMasterClass; }
	u_char GetMasterClassGrade()	{ return m_ucMasterGrade; }
	u_char GetMasterLevel()			{ return m_ucMasterLevel; }
	u_char GetMasterCurZone()		{ return m_ucMasterZone; }

	_sND_TimeUnit * GetLastConTime()	{ return &m_timeLastCon; }
	_sND_TimeUnit * GetCreateTime()		{ return &m_timeCreateDate; }

	short GetMoonpaProp()	
	{
		return g_sMoonpaProperty[m_ucMasterMoonpa];
	}


	void MakeBoardInfo( _sPacketMPBoard * board )
	{
		board->ucMasterLevel = GetMasterLevel();
		board->ucMasterRank = GetMasterRank();
		board->ucPupilCount = static_cast<u_char>(GetPupilCount());
		board->ucMoonpa = GetMasterMoonpa();		// 1 - 6
		board->ucClass = GetMasterClass();
		board->ucClassGrade = GetMasterClassGrade();
		board->ucOrg = 0;		// 0,1,2
		board->ucEtc = 0;


		ZeroMemory( board->cMasterName, sizeof(board->cMasterName) );
		strncpy( board->cMasterName, m_cMasterName, en_charac_name_length );

		ZeroMemory( board->cMasterNick, sizeof(board->cMasterNick) );
		strncpy( board->cMasterNick, m_cMasterNick, NDD_MP_MASTERNICK_LENGTH );
	}

	u_char		GetRelationn( int uid )
	{
		// 채워 넣을 것.
		return 1;
	}

	u_char CanAddNewPupil( char * p_name );
	bool AddReservePupil( char * p_name );
	bool CheckNewPupil( char * p_name );
	bool IsFullPupil()
	{
		return ( m_Pupil[0].iPupilIndex && m_Pupil[1].iPupilIndex && m_Pupil[2].iPupilIndex && m_Pupil[3].iPupilIndex );
	}
	
	bool IsEmptyConnetion()	
	{
		// 원래 NDD_MAX_PUPIL_COUNT 만큼 반복문을 써야 하지만, 4개밖에 안돼서 그냥 모두 검사한다. 
		bool ret = false;
		if( false == m_Pupil[0].IsConnected() &&
			false == m_Pupil[1].IsConnected() &&
			false == m_Pupil[2].IsConnected() &&
			false == m_Pupil[3].IsConnected() )
			ret = true;

		return ret;
	}

	int GetPupilCount()
	{
		int count = 0;
		if( m_Pupil[0].iPupilIndex )
			++count;
		if( m_Pupil[1].iPupilIndex )
			++count;
		if( m_Pupil[2].iPupilIndex )
			++count;
		if( m_Pupil[3].iPupilIndex )
			++count;

		return count;
	}

	int GetPupilSlotNumber( int cuid )
	{
		int slot = -1;
		for( int i = 0; i < NDD_MP_MAX_PUPIL_COUNT ; ++i )
		{
			if( cuid == m_Pupil[0].iPupilIndex )
			{
				slot = i;
				break;
			}
		}
		return slot;
	}


	int GetPupilID( char * name )
	{
		int cuid = 0;
		for( int i = 0; i < NDD_MP_MAX_PUPIL_COUNT; ++i )
		{
			if( m_Pupil[i].cPupilName[0] == name[0] &&
				0 == strncmp( m_Pupil[i].cPupilName, name, en_charac_name_length ) )
			{
				cuid = m_Pupil[i].iPupilIndex;
				break;
			}
		}
		return cuid;
	}

	int GetPupilArrIndex( char * name )
	{
		int arr = -1;
		for( int i = 0; i < NDD_MP_MAX_PUPIL_COUNT; ++i )
		{
			if( m_Pupil[i].cPupilName[0] == name[0] &&
				0 == strncmp( m_Pupil[i].cPupilName, name, en_charac_name_length ) )
			{
				arr = i;
				break;
			}
		}
		return arr;
	}
	int GetCharacIndex( char * name )
	{
		int cuid = 0;
		if( m_cMasterName[0] == name[0] && 
			0 == strncmp( m_cMasterName, name, en_charac_name_length ) )
		{
			cuid = GetIndex();
		}
		else
		{
			for( int i = 0; i < NDD_MP_MAX_PUPIL_COUNT; ++i )
			{
				if( m_Pupil[i].cPupilName[0] == name[0] &&
					0 == strncmp( m_Pupil[i].cPupilName, name, en_charac_name_length ) )
				{
					cuid = m_Pupil[i].iPupilIndex;
					break;
				}
			}
		}
		return cuid;
	}

	u_char GetCountConCharac()
	{
		// 스승도 포함한다.
		u_char count = 0;
		if( m_ucMasterConnectState )
			++count;

		if( m_Pupil[0].IsConnected() )
			++count;
		if( m_Pupil[1].IsConnected() )
			++count;
		if( m_Pupil[2].IsConnected() )
			++count;
		if( m_Pupil[3].IsConnected() )
			++count;

		return count;

	}

	bool IsFullConnetion()
	{
		// 원래 NDD_MAX_PUPIL_COUNT 만큼 반복문을 써야 하지만, 4개밖에 안돼서 그냥 모두 검사한다. 
		bool ret = false;
		if( m_Pupil[0].IsConnected() &&
			m_Pupil[1].IsConnected() &&
			m_Pupil[2].IsConnected() &&
			m_Pupil[3].IsConnected() )
			ret = true;
		return ret;
	}

	void SendMPInfo_Again( char * to_name, u_char zone );
	void FillPacket_MyMasterInfo( CMSG_MP_INFO_MASTER * pInfo );
	void FillPacket_StrikeResult( CMSG_MP_STRIKE_RESULT * pRsult );

	// 접속 상태를 업데이트한다. 캐릭터가 없다면, false 리턴.
	bool ConnectNotice( int cuid, u_char state, u_char zone );
	void ChangeMasterNick( char * nick );

	bool AddNewPupil( _sCSPK_Pupil& pupil );

	// 현재 접속 지역에 모두 뿌리기.
	void SendToAll( char * data, u_short length );

	// 파문 관련 함수. 
	u_char StrikeReq( char * pupil, COLE_DB_Executer * pGameBD );
	// 하산 관련 함수
	u_char LeaveReq( char * pupil, COLE_DB_Executer * pGameBD );

	// 다음은 같은 기능을 하는 함수다. 나중에 하나로 묶자.
	void RecalcPosition( CMSG_MP_STRIKE_RESULT * pResult );
	void RecalcPosition( CMSG_MP_LEFT_PUPIL * pResult );


	void AddPoint( int cuid, int value );
	void AddKillCount( int cuid, int value );


	// 포인트 연사 함수
	u_int SubTotalPoint( u_int sub )
	{
		if( m_uiMasterTotalPoint >= sub )
			m_uiMasterTotalPoint -= sub;
		else
			m_uiMasterTotalPoint = 0;

		if( m_uiMasterCurPoint >= sub )
			m_uiMasterCurPoint -= sub;
		else
			m_uiMasterCurPoint = 0;

		sub = m_uiMasterTotalPoint;

		return sub;
	}


	union 
	{
		int				m_iIndex;				// 사제 인덱스 : 스승의 캐릭터 유니크 아이디.
		CDSMaster *		pNext;					// for SimpleMemPool 
	};


	typedef struct __reserve_pupils
	{
		DWORD			dwApplyTime;			// 신청받은 시간.
		char			cApplyName[en_charac_name_length+1];
		char			cPad0;
		short			sPad;
	} _sReservePupil;


protected:
	
	u_int			m_uiTotalKillCount;		// 총 킬카운트.
	u_int			m_uiMasterKillCount;		// 

	u_int			m_uiMasterTotalPoint;		// 스승 포인트. ( 소진에 영향을 받지 않는 누적 수치. )
	u_int			m_uiMasterCurPoint;			// 소진시 사용되는 포인트. 

	u_char			m_ucMasterRank;			// 스승 레벨 ( u_short 지만, DB 에는 short 로 저장하고, casting 한다. )

	u_char			m_ucMasterSex;
	u_char			m_ucMasterMoonpa;
	u_char			m_ucMasterClass;		// 
	u_char			m_ucMasterGrade;		// 마스터의 클래스 그레이드
	u_char			m_ucMasterLevel;		// 마스터의 레벨

	u_char			m_ucBonusSkillStep;		// 스승의 레벨에 따라, 제자들이 받는 보너스 스킬 스텝.
	u_char			m_ucMasterZone;				// 스승이 있는 지역.
	u_char			m_ucMasterConnectState;	// 0 - not connected, 1 - connected

	bool			m_bApplyMasterEffect;		// 사제 관계 혜택이 적용되고 있는가.

	_sReservePupil	m_ReserveApplyPupil[NDD_MP_MAX_RESERVE_PUPIL];		// 최대 5명까지 중복가능하도록 한다.

	char			m_cMasterName[en_charac_name_length+1];
	u_char			m_ucMasterNickLength;		// 사호의 길이( byte )
	char			m_cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];

	_sND_TimeUnit	m_timeCreateDate;			// 사제 정보가 생성된 날짜.
	_sND_TimeUnit	m_timeLastCon;				// 스승의 마지막 로그인 시간.

	_sCSPK_Pupil	m_Pupil[NDD_MP_MAX_PUPIL_COUNT];		// 제자 정보.
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct __master_board_page;

// 스승 정보창에 표시되는 스승 정보 구조체.
// 스승의 인덱스는 역시, 캐릭터 인덱스를 사용한다.
struct __master_board_unit
{
	u_int							uiTotalPoint;
	__master_board_page *			pParentPage;
	_sPacketMPBoard					BoardInfo;			// 실제 정보를 참조한다.


	__master_board_unit()
	{
		uiTotalPoint = 0;
		pParentPage = NULL;
		ZeroMemory( &BoardInfo, sizeof(BoardInfo) );
	}

	void Print()
	{
		printf( "N<%s>,Rank(%u),Point(%u)", BoardInfo.cMasterName, BoardInfo.ucMasterRank, uiTotalPoint );
	}

	void Set( CDSMaster * p )
	{
		if( p )
		{
			p->MakeBoardInfo( &BoardInfo );
			uiTotalPoint = p->GetTotalPoint();
		}
	}
	void SetParent( __master_board_page * p )	{ pParentPage = p; }
	__master_board_page * GetParentPage()	{ return pParentPage; }

	__master_board_unit( const __master_board_unit& unit )
	{
		uiTotalPoint = unit.uiTotalPoint;
		CopyMemory( &BoardInfo, &unit.BoardInfo, sizeof(BoardInfo) );
	}

	int Compare( u_int point, u_char rank )
	{
		int result = 0;
		u_int o_p = uiTotalPoint;
		u_char o_r = BoardInfo.ucMasterRank;

		if( rank > o_r )
			result = -1;		// 이전으로.
		else if( rank < o_r )
			result = 1;
		else 
		{
			if( point > o_p )
				result = -1;
			else if( point < o_p )
				result = 1;
		}
		return result;
	}
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class _MPBoardList;

struct __master_board_page
{
	u_int					m_uiCount;	//개수를 센다.
	_node<__master_board_unit> *	m_pFirst;
	_MPBoardList *					m_pParent;

	__master_board_page()
		: m_uiCount(0), m_pFirst(NULL)
	{
		m_pParent = NULL;
	}

	u_char		GetFirstRank()
	{
		u_char rank = 0;
		if( m_pFirst )
			rank = m_pFirst->node.BoardInfo.ucMasterRank;
		return rank;
	}
	u_int		GetFirstPoint()
	{
		u_int point = 0;
		if( m_pFirst )
			point = m_pFirst->node.uiTotalPoint;
		return point;
	}
	int Compare( u_int point, u_char rank )
	{
		// 같으면, 뒤로 가게 하기 위해, 0 || 1 일 경우, 1로 리턴.
		int r = -1;		// 기본값 앞.
		if( m_pFirst )
		{
			int com = m_pFirst->node.Compare( point, rank );
			if( com < 0 )
				r = -1;
			else 
				r = 1;
		}
		return r;
	}

	bool AddBoard( CDSMaster * p );
	void FreeNode( _node<__master_board_unit> * pNode )
	{
		if( pNode )
			m_pBoardPool->add_free_node( pNode );
	}

	int GetPage()				{ return m_iPage; }
	void SetPage( int page )	{ m_iPage = page; }

	u_int GetCount()			{ return m_uiCount; }
	void SetCount( u_int count ){ m_uiCount = count; }
	void DecreaseCount()		{ --m_uiCount; }
	void IncreaseCount()		{ ++m_uiCount; }

	void Reset()
	{
		m_uiCount = 0;
		m_pFirst = NULL;
	}

public:
	static bool CreateBoardUnit( int count );
protected:
	int						m_iPage;	//개수를 센다.
	static NDQueueMemoryPool<__master_board_unit>	* m_pBoardPool;
};



//////////////////////////////////////////////////////////////////////////
// _MPBoardList
//////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)
#include <vector>
#include <map>
using namespace std;

class _MPBoardList
{
public:
	_MPBoardList()
		: m_iTotalPage(0), m_iTotalBoardUnit(0) 
	{
		m_ListProperty = 0;
	}

	int GetProperty()					{ return m_ListProperty; }

	// 생성자. 예약할 페이지 수와, 보드 아이템의 개수를 입력.
	bool CreateBoardList( int prop, int page_res, int board_res );



public:
	// 
	void AddMPBoard( CDSMaster * pMaster );
	void DelMPBoard( int master_index );
	void UpdateMPBoardNick( int master_index, char * nick );


	bool GetPageInfo( int page, CMSG_MP_BOARD_PAGE_INFO * pInfo );
	void GetInitialInfo( CMSG_MP_BOARD_INITIAL_OPEN * pInfo );

	typedef map<int,_node<__master_board_unit>* >		TPNodeMap;

	bool InsertNodeMap( int index, _node<__master_board_unit> * n ) 
	{
		pair< TPNodeMap::iterator, bool> ret_pair = m_nodeMap.insert( TPNodeMap::value_type( index, n ) );
		return ret_pair.second;
	}



	void PrintList();

	int GetPageCount()		{ return m_iTotalPage; }
	int GetUnitCount()		{ return m_iTotalBoardUnit; }


protected:
	// 꽉 찼는가.
	bool IsFull()
	{
		return ( m_iTotalBoardUnit >= (static_cast<int>(m_pages.size())*NDD_MP_AVR_ITEMCOUNT_PER_BOARD ) );
	}

	void IncreaseCount()	{ ++m_iTotalBoardUnit; }
	void DecreaseCount()	{ --m_iTotalBoardUnit; }

	// 마지막 페이지를 추가하고, 페이지를 리턴한다.
	__master_board_page * InsertPage();

	// start_page 에서 뒤로 가면서 개수를 10개 내외로 맞춘다.
	void ArrangePage( int start_page, bool Insert_Delete );

	


protected:
	int		m_ListProperty;		// -1 : 흑도, 0 - all, 1 - 백도 게시판.
	int		m_iTotalPage;		// 사용중인 전체 페이지 수.
	int		m_iTotalBoardUnit;	// 

	vector<__master_board_page>		m_pages;	// 초기에 100개 생성시킨다.

	map<int,_node<__master_board_unit>* >	m_nodeMap;

	CRITICAL_SECTION			m_csLock;
};




class COLE_DB_Executer;
// 마스터만 관리하면 된다. : DS 
class CDSMasterSet
{
public:
	CDSMasterSet()
		: m_pGameDB(NULL)
	{}
	virtual ~CDSMasterSet()
	{}

	bool CreateMasterPupilManager( COLE_DB_Executer * pDB, int init_master, int init_page, int init_master_board_count );
	void DestroyManager();

	// 캐릭터 핖드 입장시, 자신의 사제 정보를 요청하는 함수.
	CDSMaster *		FindAndMakeMasterIndex( int cuid );
	// 그냥 찾기
	CDSMaster *		FindMaster( int master_uid );
	CDSMaster *		AddMasterInfo_FromDB( int master_uid );
	void			AddToMasterMap( int master_uid, CDSMaster * p )
	{
		m_MasterMap.insert( map<int, CDSMaster*>::value_type(master_uid, p) );
	}
	


	inline void LockMasterMap()			{ EnterCriticalSection(&m_csMasterMap); }
	inline void UnlockMasterMap()		{ LeaveCriticalSection(&m_csMasterMap); }

	void DSProc_MasterInfoReq( CMSG_MP_INFO_REQ * pMP, _h_DS_Com * pGS );
	void DSProc_AddPoint( CMSG_MP_ADD_POINT * pMP );	
	void DSProc_AddKillCount( CMSG_MP_ADD_KILL_COUNT * pMP );	
	void DSProc_CreateMPReq( CMSG_MP_CREATE_MP_REQ * pMP, _C_CHARAC * pCharac, _h_DS_Com * pGS );
	void DSProc_NoticeCon( CMSG_MP_NOTICE_CONNECTION * pMP );
	void DSProc_ChangeMasterNick( CMSG_MP_CHANGE_MASTER_NICK_REQ * pMP );
	void DSProc_GroupChat( CMSG_MP_CHAT * pMP );
	// 캐릭터에게 제자 신청을 받았다. 스승을 찾아 전달할 것.
	void DSProc_ApplyForPupil( CMSG_MP_GD_APPLY_FOR_PUPIL * pMP, _h_DS_Com * pGS );
	void DSProc_ApplyForPupilError( CMSG_MP_APPLY_FOR_PUPIL_ERROR * pMP );
	void DSProc_ApplyAnswerYes( CMSG_MP_ANSWER_YES * pMP, _h_DS_Com * pMasterGS );
	void DSProc_ApplyAnswerNo( CMSG_MP_ANSWER_APPLY_TO_PUPIL_NO * pMP );
	void DSProc_ApplyError( CMSG_MP_APPLY_ERROR * pMP );
	void SendApplyError( char * name, u_char code, short gs, _h_DS_Com * pGS );
	
	//////////////////////////////////////////////////////////////////////////
	// 파문/강제추방/하산.
	enum
	{
		en_mp_leave_reason_none = 0,
		en_mp_leave_reason_strike =1,
		en_mp_leave_reason_ban = 2,
		en_mp_leave_reason_leave = 3,
	};

	void DSProc_StrikeReq( CMSG_MP_STRIKE_PUPIL_REQ * pMP, _h_DS_Com * pGS );
	void DSProc_LeaveReq( CMSG_MP_LEAVE_REQ * pMP, _h_DS_Com * pDSComm );

	bool DBProc_StrikePupil( int strike_pupil );
	bool DBProc_BanPupil( int ban_pupil );
	bool DBProc_LeavePupil( int leave_pupil );

	void SendStrikeError( int master, char * pupil_name, u_char code, _h_DS_Com * pGS );
	void SendLeaveError( int master, char * pupil_name, u_char code, _h_DS_Com * pGS );

	void ShowMaster( int master );

protected:
	NDSimpleMemPool<CDSMaster>			m_MasterPool;
	map<int, CDSMaster *>				m_MasterMap;		// 스승 정보 집합.
	COLE_DB_Executer *					m_pGameDB;			// 
	CRITICAL_SECTION					m_csMasterMap;

//////////////////////////////////////////////////////////////////////////
//  스승 게시판 관리.
//////////////////////////////////////////////////////////////////////////
public:
	bool CreateMPBoard( int page_res, int board_res );		// 생성자. 함수 내부에서 개수를 결정하자.

	// 사제 정보의 추가/삭제. 접속하면 추가, 끊어지면 삭제한다.
	bool AddBoard( CDSMaster * pMaster );
	void DelBoard( CDSMaster * pMaster );
	void BoardUpdateNick( CDSMaster * pMaster );

	// 성공/실패. 개수는 함수 내부에서 설정한다.ㄴ
	bool GetBoardInfo( int page, int prop, CMSG_MP_BOARD_PAGE_INFO * pInfo );


	void PrintBoard();
	void PrintSimpleInfo();

	// en_dg_mp_mode_board_initial_req:
	void DSProc_BoardInitialReq( CMSG_MP_BOARD_INITIAL_REQ * pMP, _h_DS_Com * pDSComm );
	// en_dg_mp_mode_board_req_page:
	void DSProc_BoardPageReq( CMSG_MP_BOARD_REQ_PAGE * pMP, _h_DS_Com * pDSComm );

protected:
	_MPBoardList			m_total_board;		// 백도/흑도. 낭인의 요청에 쓰인다.
	_MPBoardList			m_white_board;		// 백도 리스트.
	_MPBoardList			m_black_board;		// 흑도 리스트.



	//////////////////////////////////////////////////////////////////////////
	// 버프 혜택.
	//////////////////////////////////////////////////////////////////////////
public:
	inline static u_int GetBaseKillCount()			{ return m_uiBaseKillCount; }
	inline static u_int GetExtendKillCount()		{ return m_uiExtendKillCount; }
	inline static u_int GetMaxMasterTotalPoint()	{ return m_uiMaxMasterTotalPoint; }
	inline static u_int GetMaxPupilTotalPoint()		{ return m_uiMaxPupilTotalPoint; }



protected:
	static void SetBaseKillCount( u_int count )		{ m_uiBaseKillCount = count; }
	static void SetExtendKillCount( u_int count )	{ m_uiExtendKillCount = count; }

	static u_int m_uiBaseKillCount;
	static u_int m_uiExtendKillCount;

	static u_int m_uiMaxMasterTotalPoint;
	static u_int m_uiMaxPupilTotalPoint;
};



#endif
