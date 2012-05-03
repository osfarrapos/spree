#include "../stdafx.h"
#include "CombatRecord.h"
#include "..\Global\tcp_packets_party.h"
#include "Organization.h"

#ifdef _PD_CASTLE_STEP_2_

extern COrganizationManager *	g_pOrganizationManager ;

//////////////////////////////////////////////////////////////////////////
NDQueueMemoryPool<COrgRecord*>				* NDDList<COrgRecord*>::m_pPool				= NULL ;
NDQueueMemoryPool<_sOrgRecord>				* NDDList<_sOrgRecord>::m_pPool				= NULL ;

void CRecordOfCombat::Create()
{
	size_t size = 2048;
	if( InitializeCriticalSectionAndSpinCount( &m_cs, (0x80000000)|2000 )  == FALSE ) {
		printf( "[INIT ERROR]  RecordOfCombat Error Critical Section\n" ) ;
		throw "" ;
	}
	if(false ==NDDList<COrgRecord*>::InitializeMemoryPool(size)){
		printf("NDDList Initialize Error!  : m_pool_record");
	}	
	if(false ==NDDList<_sOrgRecord>::InitializeMemoryPool(MAX_ORG_COUNT*MAX_ORG_COUNT_INSERVER)){
		printf("NDDList Initialize Error!  : m_pool_record");
	}
	m_pool_count = 1024;
	
	COrgRecord *pOrg = NULL;	
	int i=0;	
	memset(m_combating_org,0,128);
	
	for(i=0;i<m_pool_count;i++)
	{			
		pOrg = new COrgRecord;
		if(pOrg)
		{
			m_pool_record.push_back(pOrg);
		}
	}
	m_pStartOrgRecord = NULL;
	m_pStartOrgRecord = NULL;
	m_bUse_record     = false;
	m_sManorIndex     = 0;


}


void CRecordOfCombat::Destory()
{
	COrgRecord *pOrg = NULL;
	NDDList<_sOrgRecord>::ReleaseMemoryPool();
	
	while(false == m_pool_record.empty() )
	{		
		pOrg = m_pool_record.pop_front();
		delete pOrg;
		pOrg = NULL;
	}
	m_pool_record.ReleaseMemoryPool();
	DeleteCriticalSection(&m_cs);
}
/*
bool	CRecordOfCombat::Initialize()
{
	size_t size = 2048;
	m_pStartOrgRecord = NULL;
	m_bUse_record = false;
	if( InitializeCriticalSectionAndSpinCount( &m_cs, (0x80000000)|2000 )  == FALSE ) {
		printf( "[INIT ERROR]  RecordOfCombat Error Critical Section\n" ) ;
		throw "" ;
	}
	if(false ==NDDList<COrgRecord*>::InitializeMemoryPool(size)){
		printf("NDDList Initialize Error!  : m_pool_record");
	}	
	if(false ==NDDList<_sOrgRecord>::InitializeMemoryPool(MAX_ORG_COUNT*MAX_ORG_COUNT_INSERVER)){
		printf("NDDList Initialize Error!  : m_pool_record");
	}
	m_pool_count = 1024;
	
	COrgRecord *pOrg = NULL;	
	int i=0;	
	memset(m_total_rank,0,sizeof(m_total_rank));
	
	for(i=0;i<m_pool_count;i++)
	{			
		pOrg = new COrgRecord;
		if(pOrg)
		{
			m_pool_record.push_back(pOrg);
		}
	}		

}*/



// 도전 목록 리스트에 있는 것들로 초기화 한다.
bool CRecordOfCombat::ResetWarRecord()
{
	COrgRecord		*	pOrg = NULL;
	_Organization	*	pOrganization = NULL; 

	for(int i=0;i<MAX_ORG_COUNT;i++)
	{
		if(m_combating_org[i])
		{
			pOrg = FindOrgRecord(m_combating_org[i]);
			if(pOrg)
			{
				pOrg->ResetWarRecord();
				pOrg->SetManorIndex(m_sManorIndex);
				g_pSystemLog->LogFileWrite( "[Combat_Record] Reset OrgIndex(%d) -ResetWarRecord \n", m_combating_org[i] );				
			}
			else// 없으면 처음으로 장원전에 참가하는 단이므로 단 정보를 생성한다. 
			{
				AddPool();
                pOrganization = g_pOrganizationManager->FindOrganization(m_combating_org[i]);
				if(pOrganization)
				{
					LockRecord();			
					pOrg = m_pool_record.pop_front();
					pOrg->SetAllyIndex(m_combating_org[i]);
					pOrg->SetManorIndex(m_sManorIndex);
					UnLockRecord();
					g_pSystemLog->LogFileWrite( "[Combat_Record] Reset Add OrgIndex(%d) - ResetWarRecord \n",m_combating_org[i] );				
					m_map_orgRecord.insert(map<int,COrgRecord*>::value_type(m_combating_org[i],pOrg));					
				}				
			}//else
		}//if
	}//for
	return true;
}


bool CRecordOfCombat::LoadCombatRecord(int iAllyIndex,int iManorIndex,int iEntryCount,int iWinCount,int iLoseCount,DBTIMESTAMP combatDateDB)
{
	COrgRecord		*	pOrg = NULL;
	_sCombatRecord	*	pPerManorRecord = NULL; 
	pOrg = FindOrgRecord(iAllyIndex);
	_sND_TimeUnit time;
	time.uiYear  = combatDateDB.year-2000;
	time.uiMonth = combatDateDB.month;
	time.uiDay   = combatDateDB.day;
	g_pSystemLog->LogFileWrite( "[Combat_Record]  LoadCombatRecord- Year(%d)  Month(%d) Day(%d)",combatDateDB.year,combatDateDB.month,combatDateDB.day);				
	g_pSystemLog->LogFileWrite( "[Combat_Record]  LoadCombatRecord- Year(%d)  Month(%d) Day(%d)",time.uiYear,time.uiMonth,time.uiDay);

	if(!pOrg){
		AddPool();
		LockRecord();
		pOrg = m_pool_record.pop_front();
		pOrg->SetAllyIndex(iAllyIndex);
		UnLockRecord();	
		m_map_orgRecord.insert(map<int,COrgRecord*>::value_type(iAllyIndex,pOrg));			
	}
	else // 단정보가 있었던 경우 
	{
		pPerManorRecord = pOrg->FindCombatRecord(iManorIndex);		
	}
	if(!pPerManorRecord)
	{
			pPerManorRecord = pOrg->GetEmptyCombatRecord();
			if(!pPerManorRecord){
				return false;	}   //+ Log
	}				
	LockRecord();
	pPerManorRecord->SetCombatCount(iWinCount + iLoseCount);
	pPerManorRecord->SetEnrtyCount(iEntryCount);
	pPerManorRecord->SetLoseCount(iLoseCount);
	pPerManorRecord->SetWinCount(iWinCount);
	pPerManorRecord->SetManorIndex(iManorIndex);
	pPerManorRecord->SetCombatDate(&time);
	UnLockRecord();	
	g_pSystemLog->LogFileWrite( "[Combat_Record]  Manor Combat Record Load orgIndex (%d) manorIndex(%d) year(%d)- LoadCombatRecord \n",iAllyIndex, iManorIndex,pPerManorRecord->GetCombatDate().uiYear);				

	return true;
}

bool	CRecordOfCombat::LoadVsOrgRecord(int iorgIndex,int iManorIndex,int iVSIndex,int iWinCount,int iLoseCount,int iVSAllyIndex,u_char* name)
{
	COrgRecord		*	pOrg = NULL;
	_sOrgRecord			*pVSOrgRecord = NULL;	
	_sOrgRecord			vSOrgRecord;	
	
	pOrg = FindOrgRecord(iorgIndex);

	LockRecord();
	vSOrgRecord.SetCombatCount(iWinCount+iLoseCount);
	vSOrgRecord.SetIndex(iVSIndex);
	vSOrgRecord.SetAllyIndex(iVSAllyIndex);
	vSOrgRecord.SetLoseCount(iLoseCount);
	vSOrgRecord.SetWinCount(iWinCount);
	vSOrgRecord.SetVsOrgName(name);
	UnLockRecord();
	
	if(!pOrg)
	{
		AddPool();
		LockRecord();
		pOrg = m_pool_record.pop_front();
		pOrg->SetAllyIndex(iorgIndex);
		UnLockRecord();		
		m_map_orgRecord.insert(map<int,COrgRecord*>::value_type(iorgIndex,pOrg));		
	}
	else 
	{
		pVSOrgRecord = pOrg->FindVSOrgRecord(iVSIndex);					
	}	
	if(!pVSOrgRecord)
	{		
		pOrg->SetManorIndex(iManorIndex);
		pOrg->PushBackListOrgrecord(vSOrgRecord);
	}
	else
	{
		LockRecord();
		pVSOrgRecord->SetCombatCount(iWinCount+iLoseCount);
		pVSOrgRecord->SetIndex(iVSIndex);
		pVSOrgRecord->SetAllyIndex(iVSAllyIndex);
		pVSOrgRecord->SetLoseCount(iLoseCount);
		pVSOrgRecord->SetWinCount(iWinCount);
		pVSOrgRecord->SetVsOrgName(name);
		UnLockRecord();		
	}
	g_pSystemLog->LogFileWrite( "[Combat_Record]  myOrg(%d) VS OtherOrg(%d)  Load - LoadVsOrgRecord \n",iorgIndex, iVSIndex);				
	return true;
}

void	CRecordOfCombat::AddPool()
{
	COrgRecord * pOrg = NULL;
	int iTemp = 50;
	if(iTemp >= m_pool_record.count() )	
	{
		LockRecord();
		m_pool_count += 200;
		UnLockRecord();
		for(int i=0;i<200;i++)
		{
			pOrg = new COrgRecord;
			if(pOrg)
			{
				m_pool_record.push_back(pOrg);
			}
		}
	}	
}

void	CRecordOfCombat::FillPacket(int orgIndex,CMSG_RECORD_INFORMATION_DS_GS& info)
{
	COrgRecord* pRecord = FindOrgRecord(orgIndex);

	if( pRecord )
	{
		for(int  i=0;i<MAX_MANOR;i++)
		{
			pRecord->FillCombatRecord(info.m_combat_record[i],i);
		}
		info.m_combat_org_count = pRecord->FillOrgRecord(info.m_org_record);	
		//g_pSystemLog->LogFileWrite( "[Combat_Record] -FillPacket- vsOrgCount = (%d)\n", info.m_combat_org_count );				

	}
}

void	CRecordOfCombat::UpdateRecord(CMSG_RECORD_UPDATE_GS_DS& info)
{
    COrgRecord* pRecord = FindOrgRecord(info.m_org_index);
	if( pRecord )
	{
		for(int i=0;i<MAX_MANOR;i++)
		{
			pRecord->UpdateCombatRecord(info.m_combat_record[i],i);			
		}
		pRecord->UpdateOrgRecord(info.m_org_record,info.m_combat_org_count);
	}
	else
	{
		g_pSystemLog->LogFileWrite( "[Combat_Record]  Fail Update orgindex(%d) - UpdateRecord \n",info.m_org_index);				

	}


}
//////////////////////////////////////////////////////////////////////////

COrgRecord::COrgRecord()
{
	if( InitializeCriticalSectionAndSpinCount( &m_cs, (0x80000000)|2000 )  == FALSE ) {
		printf( "[INIT ERROR]  RecordOfCombat Error Critical Section\n" ) ;
		throw "" ;
	}
	for(int i=0;i<MAX_MANOR;i++){
		m_array_combatRecord[i] = new _sCombatRecord;	}	
	InitializeOrzRecord();
}

COrgRecord::~COrgRecord()
{	
	DeleteCriticalSection(&m_cs);
	Destory();
}

bool COrgRecord::InitializeOrzRecord()
{
	m_usWinCount	= 0;
	m_usLoseCount	= 0;
	m_usCombatCount = 0;
	m_usRecordState = 0;
	m_sManorIndex	= 0;

	int i = 0;
	
	for(i=0;i<MAX_MANOR;i++)
	{
		m_array_combatRecord[i]->SetCombatCount(0);
		m_array_combatRecord[i]->SetEnrtyCount(0);
		m_array_combatRecord[i]->SetLoseCount(0);
		m_array_combatRecord[i]->SetManorIndex(0);
		m_array_combatRecord[i]->SetWinCount(0);
	}
	
	return true;
}

void COrgRecord::Destory()
{
	for(int i=0;i<MAX_MANOR;i++){
		delete m_array_combatRecord[i];	}
	_sOrgRecord * pOrgRecord	= NULL;		
}

// 전적 초기화 장원전 신청한 영웅단,방회들
bool COrgRecord::ResetWarRecord()
{	
	_sOrgRecord *		pOrgRecord	= NULL;
	bool bTest = m_list_orgRecord.empty();		

	while(!m_list_orgRecord.empty())
	{
		m_list_orgRecord.pop_front();		
	}
	return true;
}


_sCombatRecord* COrgRecord::FindCombatRecord(short index)
{
	_sCombatRecord* pCombatReocord = NULL;
	
	for(int i=0;i<MAX_MANOR;i++)
	{
		if(index == m_array_combatRecord[i]->GetManorIndex()){
			return m_array_combatRecord[i];		}
	}
	return pCombatReocord;
}


int  COrgRecord::FillOrgRecord(_sOrgRecord* record)
{
	int iResult = 0;
	NDDList<_sOrgRecord>::iterator begin = m_list_orgRecord.first();
	while(begin!=m_list_orgRecord.end())
	{
		memcpy(record,&begin->node,sizeof(_sOrgRecord));
		//g_pSystemLog->LogFileWrite( "[Combat_Record] - FillOrgRecord - vsOrgIndex(%d) AllyIndex(%d) Wincount(%d) LoseCount(%d)",record->GetIndex(),record->GetAllyIndex(),record->GetWinCount(),record->GetLoseCount());
		iResult++;		
		record = &record[iResult];
		begin = begin->next;			
		
	}    
	return iResult;
}

void COrgRecord::FillCombatRecord(_sCombatRecord &record,int count)
{
	if(count < MAX_MANOR)
	{
		CopyMemory(&record, m_array_combatRecord[count],sizeof(_sCombatRecord));		
		record.SetCombatDate(&m_array_combatRecord[count]->GetCombatDate());		
		//g_pSystemLog->LogFileWrite( "[Combat_Record] - FillCombatRecord -year(%d) year(%d) month(%d) EntryCount(%d)  \n",record.GetCombatDate().uiYear,record.GetCombatDate().uiMonth,m_array_combatRecord[count]->GetCombatDate().uiYear,m_array_combatRecord[count]->GetEnrtyCount());		
	}	
}

void COrgRecord::UpdateCombatRecord(_sCombatRecord &record,int count)
{
	if(count < MAX_MANOR)
	{
		CopyMemory( m_array_combatRecord[count],&record,sizeof(_sCombatRecord));
		if(record.GetCombatDate().uiMonth != 0)
			m_array_combatRecord[count]->SetCombatDate(&record.GetCombatDate());
		g_pSystemLog->LogFileWrite( "[Combat_Record]  update org(%d) manor index(%d) current Manor(%d)- UpdateCombatRecord \n",m_iAllyIndex,m_array_combatRecord[count]->GetManorIndex(),m_sManorIndex);						
	}
}

void COrgRecord::UpdateOrgRecord(_sOrgRecord* pRecord,int count)
{	
	_sOrgRecord record;
	_sOrgRecord* pVsOrg = NULL;
	int i(0);	
	while(!m_list_orgRecord.empty()&&i<105)
	{        
		m_list_orgRecord.pop_front();		
		++i;
		g_pSystemLog->LogFileWrite( "[Combat_Record] - UpdateOrgRecord - listout before combat record count(%d)",i);
	}
	
	for(i=0;i<count;i++)
	{
		memset(&record,0,sizeof(_sOrgRecord));
		memcpy(&record,pRecord,sizeof(_sOrgRecord));
		g_pSystemLog->LogFileWrite( "[Combat_Record]  orgIndex(%d) VS otherOrg(%d) WinCount(%d)  LoseCount(%d) VSAllyIndex(%d)- UpdateOrgRecord \n",m_iAllyIndex,pRecord->GetIndex(),pRecord->GetWinCount(),pRecord->GetLoseCount(),pRecord->GetAllyIndex());				
		g_pSystemLog->LogFileWrite( "[Combat_Record]  orgIndex(%d) VS otherOrg(%d) WinCount(%d)  LoseCount(%d) VSAllyIndex(%d)- UpdateOrgRecord \n",m_iAllyIndex,record.GetIndex(),record.GetWinCount(),record.GetLoseCount(),record.GetAllyIndex());				
		pRecord = &pRecord[i+1];
		m_list_orgRecord.push_back(record);
	}	
	NDDList<_sOrgRecord>::iterator it =  m_list_orgRecord.first();
	while(it != m_list_orgRecord.end())
	{
		pVsOrg = &it->node;
		g_pSystemLog->LogFileWrite( "[Combat_Record]  orgIndex(%d) VS vsOrgIndex(%d) VSAllyIndex(%d)- UpdateOrgRecord \n",m_iAllyIndex,pVsOrg->GetIndex(),pVsOrg->GetAllyIndex());				
        it = it->next;
	}
	g_pSystemLog->LogFileWrite( "[Combat_Record] recordCount(%d) realCount(%d)\n",count,i );
}

int COrgRecord::UpdateOrgStandings(BYTE* pos)
{
	int iResult = 0;	
	NDDList<_sOrgRecord>::iterator itrBegin = m_list_orgRecord.first();
	NDDList<_sOrgRecord>::iterator itrEnd	= m_list_orgRecord.end();
	_sOrgRecord* pRecord = NULL;
	
	u_int	iVSIndex   = 0;
	u_int	iWinCount  = 0;
	u_int	iLoseCount = 0;
	u_int	iAllyIndex = 0;
	u_char  name[25] = {0,};

	while(itrBegin!=itrEnd)
	{
		pRecord		= &itrBegin->node;		
		iVSIndex	= pRecord->GetIndex();
		iWinCount	= pRecord->GetWinCount();
		iLoseCount	= pRecord->GetLoseCount();
		iAllyIndex  = pRecord->GetAllyIndex();
		g_pSystemLog->LogFileWrite( "[Combat_Record] MyIndex(%d) VS CombatIndex(%d)-UpdateOrgStandings\n",GetAllyIndex(),pRecord->GetIndex());
		g_pSystemLog->LogFileWrite( "[Combat_Record] WinCount(%d) VS LoseCount(%d) AllyIndex(%d)-UpdateOrgStandings\n",iWinCount,iLoseCount,iAllyIndex );
		memcpy(name,pRecord->GetVsOrgName(),sizeof(u_char)*24);
		name[24] = 0;
		memcpy(pos,&iVSIndex,sizeof(u_int));
		g_pSystemLog->LogFileWrite( "[Combat_Record] MyIndex(%d) VS CombatIndex(%d)-UpdateOrgStandings\n",GetAllyIndex(),(u_int)*pos);
		pos += sizeof(u_int);
		memcpy(pos,&iWinCount,sizeof(u_int));
		g_pSystemLog->LogFileWrite( "[Combat_Record] WinCount(%d) ",(u_int)*pos);
		pos += sizeof(u_int);
		memcpy(pos,&iLoseCount,sizeof(u_int));
		g_pSystemLog->LogFileWrite( "LoseCount(%d) ",(u_int)*pos);
		pos += sizeof(u_int);
		memcpy(pos,&iAllyIndex,sizeof(u_int));
		g_pSystemLog->LogFileWrite( "iAllyIndex(%d) ",(u_int)*pos);
		pos += sizeof(u_int);
        memcpy(pos,name,sizeof(u_char)*24);
		g_pSystemLog->LogFileWrite( "name(%s) ",name);		
		pos += 24;
		iResult++;		
		g_pSystemLog->LogFileWrite( "count - UpdateOrgStandings(%d)\n ",iResult);		
		itrBegin = itrBegin->next;
	}
	return iResult;
}

int COrgRecord::AllAgainstOrgIndex(int* pAgainst)
{
	int iResult = 0;
	
	NDDList<_sOrgRecord>::iterator itrBegin = m_list_orgRecord.first();
	NDDList<_sOrgRecord>::iterator itrEnd	= m_list_orgRecord.end();
	_sOrgRecord* pRecord = NULL;

	while(itrBegin!=itrEnd)
	{
		pRecord		= &itrBegin->node;
		pAgainst[iResult] = pRecord->GetIndex();		
        iResult++;
		itrBegin = itrBegin->next;
	}
	return iResult;
}


_sCombatRecord*	COrgRecord::GetEmptyCombatRecord()
{
	_sCombatRecord* pCombatReocord = NULL;
	for(int i=0;i<MAX_MANOR;i++)
	{
		if(0 == m_array_combatRecord[i]->GetManorIndex())
		{
			return m_array_combatRecord[i];		
		}
	}
	return pCombatReocord;
}

void	COrgRecord::CombineManorRecord()
{
	Lock();
	m_usWinCount	 = 0;
	m_usLoseCount	 = 0;
	m_usCombatCount  = 0;
	m_usEntryCount	 = 0;
	UnLock();			

	for(int i=0;i<MAX_MANOR;i++)
	{
		if( 0 != m_array_combatRecord[i]->GetManorIndex() )
		{
			Lock();
			m_usWinCount	+= m_array_combatRecord[i]->GetWinCount();
			m_usLoseCount	+= m_array_combatRecord[i]->GetLoseCount();
			m_usCombatCount += m_array_combatRecord[i]->GetCombatCount();
			m_usEntryCount	+= m_array_combatRecord[i]->GetEnrtyCount();
			UnLock();			
		}
	}
}

_sOrgRecord*	COrgRecord::FindVSOrgRecord(short index)
{
	_sOrgRecord *pVSOrg = NULL;	
	NDDList<_sOrgRecord>::iterator begin = m_list_orgRecord.first();
	
	while(begin != m_list_orgRecord.end())
	{
		if(begin->node.GetIndex() == index)
		{
			pVSOrg = &begin->node;
			return pVSOrg;			
		}
		begin = begin->next;
	}
	return pVSOrg;
}

void	COrgRecord::PushBackListOrgrecord(_sOrgRecord& record)
{
	m_list_orgRecord.push_back(record);
}


void COrgRecord::ViewAllOrgRecord()
{
	_sOrgRecord* pOrgRecord = NULL;
	NDDList<_sOrgRecord>::iterator begin = m_list_orgRecord.first();	
	
	while(begin != m_list_orgRecord.end())
	{
		//log 확인을 위한 함수 
		begin = begin->next;	
	}		
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////



#endif //_COMBAT_RECORD_