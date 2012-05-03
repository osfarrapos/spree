#pragma once

#ifdef _PD_CASTLE_STEP_2_

#include <map>
#include "../Global/Global_struct.h"


using namespace std;
class COrgRecord;
struct CMSG_RECORD_UPDATE_GS_DS;

typedef	map<int,COrgRecord*>	MapOrgRecord;

//////////////////////////////////////////////////////////////////////////
///	\class	CRecordOfCombat
/// \brief  장원전 기록을 관리 하는 클래스 - 개인 전적, 영웅단별 전적, 장원전참전기록 
class CRecordOfCombat 
{
public:
	CRecordOfCombat(){}
	~CRecordOfCombat(){}
	void Create();
	void Destory();

public:
	void LockRecord()	{ EnterCriticalSection( &m_cs ); }
	void UnLockRecord()	{ LeaveCriticalSection( &m_cs ); }

	bool			GetUseRecord(){return m_bUse_record;}
	void			SetUseRecord(bool  bUsed){m_bUse_record = bUsed;}
	int				GetManorIndex(){return m_sManorIndex;}
	void			SetManorIndex(int  iIndex){m_sManorIndex = iIndex;}
	int*			GetCombatingOrg(){return m_combating_org;}
	void			SetCombatingOrg(int *combatingOrg)
	{
		memcpy(m_combating_org,combatingOrg,sizeof(int)*MAX_ORG_COUNT);
	}
	
	bool	Initialize();
	/// \brief	장원전 시작되면 개인,타단과의전적 초기화 
	///         참가하는 단만 초기화이므로 단 인덱스를 받아 초기화.
	bool	ResetWarRecord();
	
	/// \brief	단별 장원별 전적 셋팅 
	bool	LoadCombatRecord(int iAllyIndex,int iManorIndex,int iEntryCount,int iWinCount,int iLoseCount,DBTIMESTAMP combatDateDB);
	
	/// \brief	대 영웅단 전적 셋팅 
	bool	LoadVsOrgRecord(int iAllyIndex,int iManorIndex,int iVSIndex,int iWinCount,int iLoseCount,int iVSAllyIndex,u_char* name);

	/// \brief 단에 장원별 전적을 합쳐 Total 전적을 만든다.
	void	CombineManorRecord();
	/// \brief 패킷의 내용을 채워준다.
	void	FillPacket(int orgIndex,CMSG_RECORD_INFORMATION_DS_GS& info);
	/// \brief 패킷의 내용으로 업데이트 시킨다.
	void	UpdateRecord(CMSG_RECORD_UPDATE_GS_DS& info);
		
	/// \brief 단 인덱스로 단의 장원전적 을 찾는다.
	COrgRecord*	FindOrgRecord(int index)
	{
		COrgRecord*	pOrgRecord = NULL;
		LockRecord();
		map<int,COrgRecord*>::iterator itr = m_map_orgRecord.find(index);
		if(itr != m_map_orgRecord.end()){
			pOrgRecord = itr->second;		}
		UnLockRecord();
		return pOrgRecord;
	}

	//	m_pool_record의 풀 추가 
	void AddPool();	
	 MapOrgRecord* GetMapOrg() {return &m_map_orgRecord;}
private:	
	CRITICAL_SECTION			m_cs ;
	///< 영웅단or방회 인덱스 COrgRecord* 정보 맵
	MapOrgRecord				m_map_orgRecord;	
	COrgRecord*					m_pStartOrgRecord;
	bool						m_bUse_record;
	///< COrgRecord pool
	NDDList< COrgRecord* >		m_pool_record;	
	///<	m_pool_record의 카운트 
	int							m_pool_count;
	///<	현재 진행되고 장원인덱스 
	int							m_sManorIndex;	
	int							m_combating_org[MAX_ORG_COUNT];
};

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
/// \class COrgRecord
/// \brief 영웅단 장원전 기록
///        개인별 전적, 타 영웅단과의 전적, 장원별참전기록
class COrgRecord
{
public:
	COrgRecord();
	~COrgRecord();
public:
	void Lock()		{ EnterCriticalSection( &m_cs ); }
	void UnLock()	{ LeaveCriticalSection( &m_cs ); }

	int				GetAllyIndex(){return m_iAllyIndex;}
	void			SetAllyIndex(int  iAllyIndex){m_iAllyIndex = iAllyIndex;}
	unsigned short	GetWinCount()	{return m_usWinCount;}
	void			SetWinCount(unsigned short winCount){m_usWinCount = winCount;}
	unsigned short	GetLoseCount()	{return m_usLoseCount;}
	void			SetLoseCount(unsigned short loseCount){m_usLoseCount = loseCount;}
	unsigned short	GetCombatCount()	{return m_usCombatCount;}
	void			SetCombatCount(unsigned short combatCount){m_usCombatCount = combatCount;}
	unsigned short	GetRecordState(){return m_usRecordState;}
	void			SetRecordState(unsigned short state){m_usRecordState = state;}
	unsigned short	GetEntryCount()	{return m_usEntryCount;}
	void			SetEntryCount(unsigned short entryCount){m_usEntryCount = entryCount;}
	int				GetManorIndex(){return m_sManorIndex;}
	void			SetManorIndex(int  iIndex){m_sManorIndex = iIndex;}

	/// \brief 리스트에 org data 넣기 
	void	PushBackListOrgrecord(_sOrgRecord& record);
	
	/// \brief	비어 있는 장원별 전투기록 얻다
	_sCombatRecord*	GetEmptyCombatRecord();
	
	///	\brief	장원 인덱스로 기록 정보 얻다.
	_sCombatRecord* FindCombatRecord(short index);	

	/// \brief _sCombatRecord 를 받아 count 값에 맞게 복사 해준다
	void FillCombatRecord(_sCombatRecord &record,int count);
	/// \brief _sOrgRecord 를 받아 count 값에 맞게 복사
	int  FillOrgRecord(_sOrgRecord* record);

	/// \brief 패킷의 내용으로 장원전에서의 전적 업데이트
	void UpdateCombatRecord(_sCombatRecord &record,int count);

	/// \brief 패킷의 내용으로 타단과의전적 업데이트 
	void UpdateOrgRecord(_sOrgRecord* pRecord,int count);

	/// \brief DB에 _cUpdateOrgStandingsAccessor  타단과의 전적 업데이트
	/// \return return값은 싸운 단의 갯수 
	int UpdateOrgStandings(BYTE* pos);

    /// \brief	all get againstOrg index
	int AllAgainstOrgIndex(int* pAgainst);

	///	\brief	타 단인덱스로 전적 얻다. 
	_sOrgRecord*	FindVSOrgRecord(short index);	
	
	///\brief 초기화 함수 
	bool	InitializeOrzRecord();	
	void	Destory();

	/// \brief 전적 초기화 함수 : 장원전 시작시 개인,영웅단 모두 Reset시킨다.
	bool	ResetWarRecord();
	
	/// \brief 장원별 전적을 합쳐 단 전적에 기록 
	void	CombineManorRecord();
	
	/// \brief 대 단 별 메모리 풀 추가 
	void	AddOrgPool();

	void ViewAllOrgRecord();

private:
	CRITICAL_SECTION	m_cs ;
	int					m_iAllyIndex;	///<	단 인덱스 
	unsigned short		m_usWinCount;	///<	총 승수
	unsigned short		m_usLoseCount;	///<	총 패
	unsigned short		m_usCombatCount;///<	총 전적 
	unsigned short		m_usRecordState;///<	기록상태 
	unsigned short		m_usEntryCount;	///<	기록상태 
	short				m_sManorIndex;	///<	현재 진행중인 인덱스, 진행중이 아니라면 마지막 장원전 인덱스 
	NDDList<_sOrgRecord>		m_list_orgRecord ;	///< 단별 전적 기록
	
	///< 각 장원별 전투기록(우선 장원은 최대 4개로 제한한다) = `07.03.26 현재 장원 2개
	_sCombatRecord*				m_array_combatRecord[MAX_MANOR];
};



#endif 