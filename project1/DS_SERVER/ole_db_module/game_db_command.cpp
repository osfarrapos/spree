#include "..\stdafx.h"
#include <atldbcli.h>
#include "..\CS\Organization.h"
//#include "db_Con.h"
#include "db_command.h"

#include "..\MAPManager\MAPManager.h"
#include "..\Ranking_Manager\Ranking_Manager.h"
#include "..\Global\Global_ServerInfo.h"

#include "..\Network\LogSvrLink.h"

#ifdef _RESET_LEVEL
#include "..\Global\reset_level.h"
#endif

#include "../CS/Castle.h"
#ifdef	 _PD_CASTLE_STEP_2_
#include "../CS/combatrecord.h"
#endif// _PD_CASTLE_STEP_2_

#ifdef _PD_GAME_EVENT_
#include "../GEvent/GameEvent.h"
#endif

#ifdef _PD_MASTER_PUPIL_
#include "../CS/MasterPupil.h"
#endif

extern int CharacFileBackup( void );
//extern _j_DB_Con * g_pDBCon ;


extern CRankingManager * g_pRankingManager ;

extern _DSCom_Queue *		g_pDSComQueue ;

extern int g_iInitLifeValue ;			// 체력
extern	int g_iInitForceValue ;			// 내력
extern	int g_iInitConcentValue ;		// 영력

extern int	g_arRankTableVersion[_MAX_RANK] ;

extern _j_BackupReady *		g_pBackupReady ;

extern CLogSvr_Com *		g_pLogSvrCom;



bool IsBadItem( _sServer_InvenItem_v1 * pItem )
{
	bool ret = false;
	if( pItem ) 
	{
		if( pItem->cType == 1 )
		{
			switch( pItem->sID )
			{
			case 9403:
			case 9404:
				ret = true;
				break;
			}
		}
		else if( pItem->cType == 9 )
		{
			if( 473 == pItem->sID )
				ret = true;
		}
	}

	return ret ;
}





void COLE_DB_Executer::CallReConTh()
{ 
	printf( "[WORNING] GAME DB link is disconnected \n"  ) ;
	::PrintConsole( "[WORNING] GAME DB link is disconnected \n"  ) ;
	m_pDBCon->CallReConTh() ; 
	return ;
}

bool COLE_DB_Executer::GetCharacAllInfo(int uid, _C_CHARAC * pCharac )
{

	if( pCharac == NULL || uid == 0 )
	{
		::PrintConsole("[ERROR] GetCharacAllInfo() -> if( pCharac == NULL || uid == 0 ) \n" ) ;
		return false ;
	}

	try	{

		// 상태		
		pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED = BS_COMPLETED;

		// 기본 캐릭 정보를 읽어온다.
		if( Get_Charac_Info( uid, pCharac ) == false )
		{
			::PrintConsole("[ERROR] Get_Charac_Info \n") ;
			return false ;
		}

		if( pr_Get_CharacState( uid, pCharac ) == false )
		{
			::PrintConsole("[ERROR] pr_Get_CharacState \n") ;
			return false ;
		}

		// Read binary data
		if( pr_Get_QuestInfo( uid, pCharac ) == false){
			::PrintConsole("[ERROR] pr_Get_QuestInfo \n") ;
			return false;
		}
		if( pr_Get_MartialData( uid, pCharac ) == false){
			::PrintConsole("[ERROR] pr_Get_MartialData \n") ;
			return false;
		}
		if( inven_get( uid, pCharac ) == false){
			::PrintConsole("[ERROR] inven_get \n") ;
			return false;
		}
		if( GetNic( uid, pCharac ) == false)
		{
			::PrintConsole("[ERROR] GetNic \n") ;
			return false ;
		}
		if( GetMatch( uid, pCharac ) == false )
		{
			::PrintConsole("[ERROR] GetMatch \n") ;
			return false ;
		}
		if( GetEffect( uid, pCharac ) == false )
		{
			::PrintConsole("[ERROR] GetEffect \n") ;
			return false ;
		}

		if( GetPkRecord( uid, pCharac ) == false )
		{
			::PrintConsole( "[ERROR] GetPkRecord\n" ) ;
			return false ;
		}

		if( GetCharacItemUseStatus( uid, pCharac ) == false )
		{
			PrintConsole( "[ERROR] GetCharacItemUseStatu\n" ) ;
			return false ;
		}

		if( false == GetCharacQuickSlot( uid, pCharac ) )
		{
			return false ;
		}

#ifdef _PD_CHARAC_LTS_
		if( false == GetCharacLts( uid, pCharac ) )
			return false;
#endif

#ifdef _PD_PKEVENT_BOX_SPAWN_
		if( false == GetBWWarWinTime( uid, pCharac->CHARAC_BW_WAR.iWinTime ) )
		{
			return false;
		}
#endif

#ifdef _PD_MASTER_PUPIL_
		if( false == MP_GetCharacMPInfo( uid, pCharac ) )
		{
			return false;
		}
#endif
		return true ;

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::GetCharacAllInfo \n") ;		
		return false;
	}
	
}


// Charac data base
int COLE_DB_Executer::GetLastServer(int unique_id) 
{
	int pos = 0 ;
	try{

		pos = 1; 

		HRESULT hr;	

		
		
		m_prDSGetLastServer->ResetAccessor() ;
		pos = 2; 
		
		m_prDSGetLastServer->GetAccessor()->m_unique_id = unique_id ;

		pos = 3; 
		hr = m_prDSGetLastServer->OpenCommand( *m_session ) ;
		pos = 4; 
		m_prDSGetLastServer->CloseCommand() ;
		pos = 5; 

		if(FAILED(hr)){ 
			pos = 6; 
			DBErrorOutput( hr ) ;			
			if(hr == E_FAIL) CallReConTh();
			return false;
		}			
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::GetLastServer (pos.%d) \n", pos) ;
		m_prDSGetLastServer->CloseCommand() ;
		return false;
	}
	return m_prDSGetLastServer->GetAccessor()->m_last_server ;

}
void COLE_DB_Executer::SetLastServer(int unique_id, int last_server) 
{
	int pos = 0 ;
	try{

		HRESULT hr;	

		pos = 1 ;

		m_prDSSetLastServer->ResetAccessor() ;
		pos = 2 ;

		m_prDSSetLastServer->GetAccessor()->m_unique_id = unique_id ;
		m_prDSSetLastServer->GetAccessor()->m_last_server = last_server ;

		pos = 3 ;


		hr = m_prDSSetLastServer->OpenCommand(*m_session);	
		m_prDSSetLastServer->CloseCommand() ;

		pos = 4 ;

		if(FAILED(hr)){ 
			DBErrorOutput( hr ) ;			
			if(hr == E_FAIL) CallReConTh();			
		}			
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::SetLastServer (pos. %d)\n",pos) ;
		m_prDSSetLastServer->CloseCommand() ;
	}
}


bool COLE_DB_Executer::SelectCharac( char * characName )
{
	try{
		HRESULT hr ;

		m_pSelectCharac->ResetAccessor() ;
		
		memcpy( m_pSelectCharac->GetAccessor()->m_select_charac, characName, 13 ) ;

		hr = m_pSelectCharac->OpenCommand(*m_session) ;
		m_pSelectCharac->CloseCommand() ;

		if(FAILED(hr)) {
			DBErrorOutput( hr ) ;
			if( hr == E_FAIL ) CallReConTh() ;
			return false ;
		}

		return true ;

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::SelectCharac \n") ;
		m_pSelectCharac->CloseCommand() ;
		return false ;
	}
	
}

int COLE_DB_Executer::pr_Charac_Create( char * acc, CMSG_CHARAC_CREATE_GS_DS * pCreate, int accid, short luck, int * uid ) // Create character
{

	try{

		HRESULT hr;
		int result = 0;
		m_pCreateCharac->ResetAccessor() ;

		strncpy(m_pCreateCharac->GetAccessor()->m_acc, acc, en_max_lil+1);
		memcpy(m_pCreateCharac->GetAccessor()->m_name, pCreate->cName, 13);
		m_pCreateCharac->GetAccessor()->m_vsex = pCreate->cSex;

		m_pCreateCharac->GetAccessor()->m_Constitution = pCreate->sConstitution;
		m_pCreateCharac->GetAccessor()->m_Zen = pCreate->sZen;
		m_pCreateCharac->GetAccessor()->m_Intelligence = pCreate->sIntelligence;
		m_pCreateCharac->GetAccessor()->m_Dexterity = pCreate->sDexterity;
		m_pCreateCharac->GetAccessor()->m_Str = pCreate->sStr;

		m_pCreateCharac->GetAccessor()->m_vhair = pCreate->cHair;
		m_pCreateCharac->GetAccessor()->m_vface = pCreate->cFace;
		m_pCreateCharac->GetAccessor()->m_vluck = luck;
		m_pCreateCharac->GetAccessor()->m_vamor = pCreate->sAmor;
		m_pCreateCharac->GetAccessor()->m_vpants = pCreate->sPants;
		m_pCreateCharac->GetAccessor()->m_zone = pCreate->cInitZone;
		m_pCreateCharac->GetAccessor()->m_x = pCreate->fInitPosX;
		m_pCreateCharac->GetAccessor()->m_z = pCreate->fInitPosZ;		
		m_pCreateCharac->GetAccessor()->m_sLifePower = pCreate->sConstitution * g_iInitLifeValue ;
		m_pCreateCharac->GetAccessor()->m_sForcePower = pCreate->sZen * g_iInitForceValue;
		m_pCreateCharac->GetAccessor()->m_sConcentrationPower = pCreate->sIntelligence * g_iInitConcentValue;
		
		//m_pCreateCharac->m_vmaxheart = pCreate->sDexterity * 10;
		//m_pCreateCharac->m_vmaxstamina = pCreate->sStr * 10;
		m_pCreateCharac->GetAccessor()->m_acc_id = accid;	//ozzywow edit 2004.8.25

		hr = m_pCreateCharac->OpenCommand(*m_session);
		if(FAILED(hr)){			
			DBErrorOutput(hr) ;
			if( hr == 0x80040e14 || hr == 0x80004005 )
			{
				hr = S_OK ;
			}
			else
			{
				DBErrorOutput( hr ) ;
				if(hr == E_FAIL)  {	CallReConTh(); 	}
				m_pCreateCharac->CloseCommand() ;
				return -1;
			}
		}

		m_pCreateCharac->CloseCommand() ;

		

		result = m_pCreateCharac->GetAccessor()->m_result;
		*uid = m_pCreateCharac->GetAccessor()->m_vunique_id;


		return result;	

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_Charac_Create \n") ;
		m_pCreateCharac->CloseCommand() ;
		return -1;
	}

	
}

int COLE_DB_Executer::pr_Charac_Delete( int uid ) {					// Delete Character
	

	try{

		HRESULT hr;
		int result = 0;

		m_pCharacDelete->ResetAccessor() ;
		
		m_pCharacDelete->GetAccessor()->m_uid = uid;

		hr = m_pCharacDelete->OpenCommand(*m_session);
		m_pCharacDelete->CloseCommand() ;	

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return -1;
		}	

		result = m_pCharacDelete->GetAccessor()->m_result;

		return result;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_Charac_Delete \n") ;
		m_pCharacDelete->CloseCommand() ;
		return -1;
	}

	
}

bool COLE_DB_Executer::pr_Update_LevelAll( int uid, _C_CHARAC * pCharac ){
	

	try{


		HRESULT hr;
		int result=0;	
		m_pUpdateLevelAll->ResetAccessor() ;

		m_pUpdateLevelAll->GetAccessor()->m_uniqueID = uid;


		memcpy( &m_pUpdateLevelAll->GetAccessor()->m_data, &pCharac->CHARAC_LEVEL, sizeof(_CHARAC_LEVEL) ) ;

		hr = m_pUpdateLevelAll->OpenCommand(*m_session);
		m_pUpdateLevelAll->CloseCommand() ;

		if(FAILED(hr)){		
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}

		result = m_pUpdateLevelAll->GetAccessor()->m_result;

		if(result != 0)	return false;

		return true;

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_Update_LevelAll \n") ;
		m_pUpdateLevelAll->CloseCommand() ;
		return false;
	}
	
	
}

bool COLE_DB_Executer::pr_Get_Attr( int uid, _C_CHARAC * pCharac ){ // Get level info
	
	try{

		HRESULT hr;
		int result=0;
		m_pGet5StarLevel->ResetAccessor() ;

		m_pGet5StarLevel->GetAccessor()->m_UniqueID = uid;



		hr = m_pGet5StarLevel->OpenCommand(*m_session);
		m_pGet5StarLevel->CloseCommand() ;


		if(FAILED(hr)){		
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}

		memcpy( &pCharac->CHARAC_LEVEL, &m_pGet5StarLevel->GetAccessor()->m_data, sizeof(_CHARAC_LEVEL) ) ;

		result = m_pGet5StarLevel->GetAccessor()->m_result;

		if(result != 0)	return false;

		return false;	

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_Get_Attr \n") ;
		m_pGet5StarLevel->CloseCommand() ;
		return false;
	}
	
	
}

bool COLE_DB_Executer::pr_Update_CharacState( int uid, _C_CHARAC * pCharac ) {
	
	try{

		HRESULT hr;
		int result=0;
		m_pUpdateCharacState->ResetAccessor() ;

		m_pUpdateCharacState->GetAccessor()->m_uniqueID = uid;

		memcpy( &m_pUpdateCharacState->GetAccessor()->m_data, &pCharac->CHARAC_STATE, sizeof(_CHARAC_STATE) ) ;

		hr = m_pUpdateCharacState->OpenCommand(*m_session);
		m_pUpdateCharacState->CloseCommand() ;


		if(FAILED(hr)) {
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}

		result = m_pUpdateCharacState->GetAccessor()->m_result;
		if(result != 0) return false;

		return true;

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_Update_CharacState \n") ;
		m_pUpdateCharacState->CloseCommand() ;
		return false;
	}
	
	
}
bool COLE_DB_Executer::pr_Get_CharacState( int uid, _C_CHARAC * pCharac ) {
	
	try{

		HRESULT hr;
		int result=0;
		m_pGetCharacState->ResetAccessor() ;

		m_pGetCharacState->GetAccessor()->m_UniqueID = uid;


		hr = m_pGetCharacState->OpenCommand(*m_session);
		m_pGetCharacState->CloseCommand() ;


		if(FAILED(hr)){ 		
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}

		memcpy( &pCharac->CHARAC_STATE, &m_pGetCharacState->GetAccessor()->m_data, sizeof(_CHARAC_STATE) ) ;

		result = m_pGetCharacState->GetAccessor()->m_result;

		if(result != 0) return false;

		return true;

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_Get_CharacState \n") ;
		m_pGetCharacState->CloseCommand() ;
		return false;
	}
	
	
}

bool COLE_DB_Executer::pr_Update_CurBasicInfo( int uid, _C_CHARAC * pCharac ) {
	

	try{

		HRESULT hr;
		int result=0;
		m_pUpdateCurBasicInfo->ResetAccessor() ;


		m_pUpdateCurBasicInfo->GetAccessor()->m_UniqueID = uid;
		m_pUpdateCurBasicInfo->GetAccessor()->m_vzone = pCharac->CHARAC_CUR_BASIC.sZone;
		m_pUpdateCurBasicInfo->GetAccessor()->m_vx = pCharac->CHARAC_CUR_BASIC.fX;
		m_pUpdateCurBasicInfo->GetAccessor()->m_vy = pCharac->CHARAC_CUR_BASIC.sY;
		m_pUpdateCurBasicInfo->GetAccessor()->m_vz = pCharac->CHARAC_CUR_BASIC.fZ;
		m_pUpdateCurBasicInfo->GetAccessor()->m_sLifePower = pCharac->CHARAC_CUR_BASIC.sLifePower;
		m_pUpdateCurBasicInfo->GetAccessor()->m_sForcePower = pCharac->CHARAC_CUR_BASIC.sForcePower;
		m_pUpdateCurBasicInfo->GetAccessor()->m_sConcentrationPower = pCharac->CHARAC_CUR_BASIC.sConcentrationPower;

		m_pUpdateCurBasicInfo->GetAccessor()->m_sRespawnServerNo = pCharac->CHARAC_CUR_BASIC.cRespawnServerNo ;
		memcpy( m_pUpdateCurBasicInfo->GetAccessor()->m_sRespawnPosName, pCharac->CHARAC_CUR_BASIC.cRespawnPosName, 13 ) ;
		m_pUpdateCurBasicInfo->GetAccessor()->m_fRespawnPosX = pCharac->CHARAC_CUR_BASIC.fRespawnPosX ;
		m_pUpdateCurBasicInfo->GetAccessor()->m_fRespawnPosZ = pCharac->CHARAC_CUR_BASIC.fRespawnPosZ ;


		hr = m_pUpdateCurBasicInfo->OpenCommand(*m_session);
		m_pUpdateCurBasicInfo->CloseCommand() ;



		if(FAILED(hr)) {		
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}

		result = m_pUpdateCurBasicInfo->GetAccessor()->m_result;

		if(result != 0)	return false;

		return true;	



	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_Update_CurBasicInfo \n") ;
		m_pUpdateCurBasicInfo->CloseCommand() ;
		return false;
	}

	
	
}

bool COLE_DB_Executer::pr_Get_CurBasicInfo( int uid, _C_CHARAC * pCHARAC ){
	

	try{

		HRESULT hr;
		int result=0;
		m_pGetCurBasicInfo->ResetAccessor() ;

		m_pGetCurBasicInfo->GetAccessor()->m_UniqueID = uid;


		hr = m_pGetCurBasicInfo->OpenCommand(*m_session);
		m_pGetCurBasicInfo->CloseCommand() ;

		if(FAILED(hr)){		
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}


		pCHARAC->CHARAC_CUR_BASIC.sZone = m_pGetCurBasicInfo->GetAccessor()->m_vzone;
		pCHARAC->CHARAC_CUR_BASIC.fX = m_pGetCurBasicInfo->GetAccessor()->m_vx;
		pCHARAC->CHARAC_CUR_BASIC.sY = m_pGetCurBasicInfo->GetAccessor()->m_vy;
		pCHARAC->CHARAC_CUR_BASIC.fZ = m_pGetCurBasicInfo->GetAccessor()->m_vz;
		pCHARAC->CHARAC_CUR_BASIC.sLifePower = m_pGetCurBasicInfo->GetAccessor()->m_sLifePower;
		pCHARAC->CHARAC_CUR_BASIC.sForcePower = m_pGetCurBasicInfo->GetAccessor()->m_sForcePower;
		pCHARAC->CHARAC_CUR_BASIC.sConcentrationPower = m_pGetCurBasicInfo->GetAccessor()->m_sConcentrationPower;
		pCHARAC->CHARAC_CUR_BASIC.cRespawnServerNo = (char)m_pGetCurBasicInfo->GetAccessor()->m_sRespawnServerNo ;
		memcpy( pCHARAC->CHARAC_CUR_BASIC.cRespawnPosName, m_pGetCurBasicInfo->GetAccessor()->m_sRespawnPosName, 13) ;
		pCHARAC->CHARAC_CUR_BASIC.fRespawnPosX = m_pGetCurBasicInfo->GetAccessor()->m_fRespawnPosX ;
		pCHARAC->CHARAC_CUR_BASIC.fRespawnPosZ = m_pGetCurBasicInfo->GetAccessor()->m_fRespawnPosZ ;


		result = m_pGetCurBasicInfo->GetAccessor()->m_result;

		if(result != 0) return false;
		return true;


	}
	catch (...) {
		
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_Get_CurBasicInfo \n") ;
		m_pGetCurBasicInfo->CloseCommand() ;
		return false;
	}
	
	
}


// jk.Hong Edite
bool COLE_DB_Executer::Get_Charac_List( int acc_uid, _USER * pUser)
{

	if( pUser == NULL ) return false ;

	try{

		HRESULT hr;
		int result=0;
		m_pCharacCount->ResetAccessor() ;
		int	UniqueID[3];
		
		m_pCharacCount->GetAccessor()->m_acc_uid =  acc_uid ;
		hr = m_pCharacCount->OpenCommand(*m_session);		

		if(FAILED(hr)){		
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();			
			return false;
		}
		
		
		pUser->cCharacCount = (char)m_pCharacCount->GetAccessor()->m_nCharac;
		UniqueID[0] = m_pCharacCount->GetAccessor()->m_UniID1;
		UniqueID[1] = m_pCharacCount->GetAccessor()->m_UniID2;
		UniqueID[2] = m_pCharacCount->GetAccessor()->m_UniID3;


		for( int i = 0 ; i < 3 ; i++ )
		{
			pUser->stCharac[i].Reset() ;
		}		

		PCHARAC pCharac;
		bool bRet ;
		for(int i=0; i < m_pCharacCount->GetAccessor()->m_nCharac; i++)
		{			
			pCharac = &pUser->stCharac[i];			


			pCharac->charac_lock() ;
			try{
				bRet = GetCharacAllInfo( UniqueID[i], pCharac ) ;
			}
			catch(...){
				::PrintConsole( "[EXCEPTION] g_DB_Command->GetCharacAllInfo( uid, pCharac) acc:%d\n", acc_uid) ;
				bRet = false ;
			}
			pCharac->charac_unlock() ;

			if( bRet == false )
			{
				return false ;
			}

			// 계정 설정
			memcpy( pCharac->CHARAC_BASIC.cAccount, pUser->cNDAccount, en_max_lil+1 ) ;

			pCharac->CHARAC_STATE.sMonsterKill = 0 ;

			// 변경된 데이타가 있으면 업데이트 하도록..
			g_pBackupReady->InsertCharac( pCharac ) ;
			
		}//for

		return true;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::Get_Charac_List \n") ;
		m_pCharacCount->CloseCommand() ;
		return false;
	}
	
}
bool COLE_DB_Executer::Get_Charac_Info( int uid, _C_CHARAC * pCharac )
{
	

	try{


		HRESULT hr;
		int result=0;
		m_pGetCharacInfo->ResetAccessor() ;

		m_pGetCharacInfo->GetAccessor()->m_UniID = uid;
		hr = m_pGetCharacInfo->OpenCommand(*m_session);
		m_pGetCharacInfo->CloseCommand() ;


		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			::PrintConsole( " -- Get_Charac_Info : %d\n", uid ) ;
			if(hr == E_FAIL) 
				CallReConTh();
			return false;
		}


		result = m_pGetCharacInfo->GetAccessor()->m_result;

		if(result != 0 ) {
			return false;
		}

		// Setting data
		pCharac->CHARAC_BASIC.iUniqueID = uid;
		memcpy(pCharac->CHARAC_BASIC.cChrName, m_pGetCharacInfo->GetAccessor()->m_name, 13);
		pCharac->CHARAC_BASIC.cSex = (char)m_pGetCharacInfo->GetAccessor()->m_vsex;
		pCharac->CHARAC_BASIC.cParty = (char)m_pGetCharacInfo->GetAccessor()->m_vparty;
		memcpy(pCharac->CHARAC_BASIC.cGamete, m_pGetCharacInfo->GetAccessor()->m_vgamete, 13);
		pCharac->CHARAC_BASIC.cHair = (char)m_pGetCharacInfo->GetAccessor()->m_vhair;
		pCharac->CHARAC_BASIC.cFace = (char)m_pGetCharacInfo->GetAccessor()->m_vface;
		pCharac->CHARAC_BASIC.cLuck = (char)m_pGetCharacInfo->GetAccessor()->m_vluck;
		

		pCharac->CHARAC_LEVEL.sConstitution = m_pGetCharacInfo->GetAccessor()->m_sConstitution ;
		pCharac->CHARAC_LEVEL.sZen = m_pGetCharacInfo->GetAccessor()->m_sZen ;
		pCharac->CHARAC_LEVEL.sIntelligence = m_pGetCharacInfo->GetAccessor()->m_sIntelligence ;
		pCharac->CHARAC_LEVEL.sDexterity = m_pGetCharacInfo->GetAccessor()->m_sDexterity ;	
		pCharac->CHARAC_LEVEL.sStr = m_pGetCharacInfo->GetAccessor()->m_sStr ;	
		pCharac->CHARAC_LEVEL.sMaxLifePower = m_pGetCharacInfo->GetAccessor()->m_sMaxLifePower;
		pCharac->CHARAC_LEVEL.sMaxForcePower = m_pGetCharacInfo->GetAccessor()->m_sMaxForcePower;
		pCharac->CHARAC_LEVEL.sMaxConcentrationPower = m_pGetCharacInfo->GetAccessor()->m_sMaxConcentrationPower;		
		pCharac->CHARAC_LEVEL.sLeftPoint = m_pGetCharacInfo->GetAccessor()->m_left_point;
		pCharac->CHARAC_LEVEL.sMasteryPoint = m_pGetCharacInfo->GetAccessor()->m_master_point ;

		pCharac->CHARAC_CUR_BASIC.sZone = m_pGetCharacInfo->GetAccessor()->m_zone;
		pCharac->CHARAC_CUR_BASIC.fX = m_pGetCharacInfo->GetAccessor()->m_x ;
		pCharac->CHARAC_CUR_BASIC.sY = m_pGetCharacInfo->GetAccessor()->m_y ;
		pCharac->CHARAC_CUR_BASIC.fZ = m_pGetCharacInfo->GetAccessor()->m_z ;
		pCharac->CHARAC_CUR_BASIC.sLifePower = m_pGetCharacInfo->GetAccessor()->m_sLifePower ;
		pCharac->CHARAC_CUR_BASIC.sForcePower = m_pGetCharacInfo->GetAccessor()->m_sForcePower ;
		pCharac->CHARAC_CUR_BASIC.sConcentrationPower = m_pGetCharacInfo->GetAccessor()->m_sConcentrationPower ;
		//pCharac->CHARAC_CUR_BASIC.sHeartPower = m_pGetCharacInfo->GetAccessor()->m_heart_power ;
		//pCharac->CHARAC_CUR_BASIC.sta = m_pGetCharacInfo->GetAccessor()->m_stamina_power ;
		pCharac->CHARAC_CUR_BASIC.cRespawnServerNo = (char)m_pGetCharacInfo->GetAccessor()->m_respawnServerNo ;
		memcpy(pCharac->CHARAC_CUR_BASIC.cRespawnPosName, m_pGetCharacInfo->GetAccessor()->m_respawnPosName, 13) ;
		pCharac->CHARAC_CUR_BASIC.fRespawnPosX = m_pGetCharacInfo->GetAccessor()->m_respawnPosx ;
		pCharac->CHARAC_CUR_BASIC.fRespawnPosZ = m_pGetCharacInfo->GetAccessor()->m_respawnPosz ;

		pCharac->CHARAC_BASIC.cClass = (char)m_pGetCharacInfo->GetAccessor()->m_class ;
		pCharac->CHARAC_BASIC.cClassGrade = (char)m_pGetCharacInfo->GetAccessor()->m_classGrade ;
		pCharac->CHARAC_BASIC.iContribution = m_pGetCharacInfo->GetAccessor()->m_iContribution ;
		pCharac->CHARAC_BASIC.cGM_Check = static_cast<char>(m_pGetCharacInfo->GetAccessor()->m_gm) ;
		//pCharac->CHARAC_STATE.sLevelUpGameSecond = m_pGetCharacInfo->GetAccessor()->m_levelup_time ;		
		pCharac->CHARAC_BASIC.dwPlayTime = m_pGetCharacInfo->GetAccessor()->m_dwPlay_time ;
		pCharac->CHARAC_BASIC.ucChangeName = static_cast<u_char>(m_pGetCharacInfo->GetAccessor()->m_ChangeName) ;
	
#ifdef CHARAC_CREATE_DATE_20070309
		pCharac->CHARAC_BASIC.CharacCreateDate.sYear = static_cast<short> (m_pGetCharacInfo->GetAccessor()->m_CharacCreateDate.year) ;	//캐릭터생성날짜
		pCharac->CHARAC_BASIC.CharacCreateDate.sMonth = static_cast<short> (m_pGetCharacInfo->GetAccessor()->m_CharacCreateDate.month) ;	//캐릭터생성날짜
		pCharac->CHARAC_BASIC.CharacCreateDate.sDay = static_cast<short> (m_pGetCharacInfo->GetAccessor()->m_CharacCreateDate.day) ;	//캐릭터생성날짜
#endif
		return true;

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::Get_Charac_Info \n") ;
		m_pGetCharacInfo->CloseCommand() ;
		return false;
	}

	
	
}


bool COLE_DB_Executer::inven_get( int unique, _C_CHARAC * pCharac )
{
	try{

		HRESULT hr;
		m_pGetInven->ResetAccessor() ;

		m_pGetInven->GetAccessor()->m_unique_id = unique;
		
		hr = m_pGetInven->OpenCommand(*m_session);
		m_pGetInven->CloseCommand() ;


		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}

		if( m_pGetInven->GetAccessor()->m_money >= static_cast<__int64>(0xffffffff) )
			m_pGetInven->GetAccessor()->m_data.iMoney = 0xffffffffU;
		else 
			m_pGetInven->GetAccessor()->m_data.iMoney = static_cast<u_int>(m_pGetInven->GetAccessor()->m_money);
		
		memcpy( &pCharac->CHARAC_INVENTORY, &m_pGetInven->GetAccessor()->m_data, sizeof(_CHARAC_INVENTORY));


		if( pCharac->CHARAC_INVENTORY.sVersion == 0 )
		{
			_sServer_InvenItem inven[_V0_COUNT_INVEN_ITEM_] ;
			memcpy( &inven[0], &pCharac->CHARAC_INVENTORY.cInventory[0], sizeof(inven) ) ;
			memset( &pCharac->CHARAC_INVENTORY.cInventory[0], 0, sizeof(pCharac->CHARAC_INVENTORY.cInventory) ) ;

			ConvertToLastVersion( pCharac->CHARAC_INVENTORY.sVersion
				, (_sServer_InvenItem_v1*)&pCharac->CHARAC_INVENTORY.cInventory[0]
				, inven ) ;

			pCharac->CHARAC_INVENTORY.sVersion = 1 ;
			
			// 버젼이 변경되었다면 바로 DB에 업데이트 해줘라.
			if( inven_update( unique, pCharac ) == false )
			{
				::PrintConsole("[DB-ERROR] Inven_update failed (%s)\n", pCharac->CHARAC_BASIC.cChrName ) ;
			}
		}


		return true;



	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::inven_get \n") ;
		m_pGetInven->CloseCommand() ;
		return false;
	}
	
		
}


bool COLE_DB_Executer::inven_update( int unique, _C_CHARAC * pCharac )
{
	if( pCharac == NULL || unique <= 0 )
	{
		::PrintConsole("[UPDATE_ERROR] if( pCharac == NULL || unique == 0 ) \n" ) ;
		return false ;
	}

	if( pCharac->CHARAC_INVENTORY.sVersion != 1 )
	{
		::PrintConsole( "[UPDATE_ERROR] Bad inven update(chr:%s, ver:%d) \n", pCharac->CHARAC_BASIC.cChrName, pCharac->CHARAC_INVENTORY.sVersion ) ;
		return false ;
	}

	bool ret = true;

	try{		

		HRESULT hr;
		m_pUpdateInven->ResetAccessor() ;

		m_pUpdateInven->GetAccessor()->m_unique_id = unique;		
		m_pUpdateInven->GetAccessor()->m_money = static_cast<__int64>(pCharac->CHARAC_INVENTORY.iMoney);
		memcpy(&m_pUpdateInven->GetAccessor()->m_data, &pCharac->CHARAC_INVENTORY, sizeof(_CHARAC_INVENTORY));

		hr = m_pUpdateInven->OpenCommand(*m_session);
		m_pUpdateInven->CloseCommand() ;
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			ret = false;
		}
		else
		{
			_ItemLog * pItemLog =  g_pLogSvrCom->GetItemLogSlot() ;
			if( pItemLog )
			{
				memset( pItemLog, 0, sizeof(_ItemLog) ) ;

				pItemLog->item_uid = 0 ;
				pItemLog->code = _LOG_ITEM_DS_UPDATE_MONEY_ ;
				pItemLog->zone = pCharac->CHARAC_STATE.sInnerLevel ;
				sprintf( pItemLog->from, "DS->INVEN" ) ;
				pItemLog->to_uid = pCharac->CHARAC_BASIC.iUniqueID ;
				memcpy( pItemLog->to, pCharac->CHARAC_BASIC.cChrName, 13 ) ;
				pItemLog->money = pCharac->CHARAC_INVENTORY.iMoney ;
				pItemLog->type1 = 0 ;
				pItemLog->type2 = 0 ;

				g_pLogSvrCom->InsertItemLog( pItemLog ) ;

			}
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::inven_update \n") ;
		m_pUpdateInven->CloseCommand() ;
		ret = false;
	}

	return ret;
}


/*
bool COLE_DB_Executer::inven_update_v2( int unique, _C_CHARAC * pCharac )
{
	if( pCharac == NULL || unique <= 0 )
	{
		::PrintConsole("[UPDATE_ERROR] if( pCharac == NULL || unique == 0 ) \n" ) ;
		return false ;
	}
    	
	try{		
        
		if( pCharac->CHARAC_INVENTORY.sVersion != 1 )
		{
			::PrintConsole( "[UPDATE_ERROR] Bad inven update(chr:%s, ver:%d) \n", pCharac->CHARAC_BASIC.cChrName, pCharac->CHARAC_INVENTORY.sVersion ) ;
			return false ;
		}

		HRESULT hr;
		m_pUpdateInven->ResetAccessor() ;

		m_pUpdateInven->GetAccessor()->m_unique_id = unique;		
		m_pUpdateInven->GetAccessor()->m_money = static_cast<__int64>(pCharac->CHARAC_INVENTORY.iMoney);
		memcpy(&m_pUpdateInven->GetAccessor()->m_data, &pCharac->CHARAC_INVENTORY, sizeof(_CHARAC_INVENTORY));

		hr = m_pUpdateInven->OpenCommand(*m_session);
		m_pUpdateInven->CloseCommand() ;
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}

		_ItemLog * pItemLog =  g_pLogSvrCom->GetItemLogSlot() ;
		if( pItemLog )
		{
			memset( pItemLog, 0, sizeof(_ItemLog) ) ;

			pItemLog->item_uid = 0 ;
			pItemLog->code = _LOG_ITEM_DS_UPDATE_MONEY_ ;
			pItemLog->zone = pCharac->CHARAC_STATE.sInnerLevel ;
			sprintf( pItemLog->from, "DS->INVEN" ) ;
			pItemLog->to_uid = pCharac->CHARAC_BASIC.iUniqueID ;
			memcpy( pItemLog->to, pCharac->CHARAC_BASIC.cChrName, 13 ) ;
			pItemLog->money = pCharac->CHARAC_INVENTORY.iMoney ;
			pItemLog->type1 = 0 ;
			pItemLog->type2 = 0 ;
			
			g_pLogSvrCom->InsertItemLog( pItemLog ) ;

		}

		return true;


	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::inven_update \n") ;
		m_pUpdateInven->CloseCommand() ;
		return false;
	}
}


bool COLE_DB_Executer::inven_update_v3( int unique, _C_CHARAC * pCharac )
{
	if( pCharac == NULL || unique <= 0 )
	{
		::PrintConsole("[UPDATE_ERROR] if( pCharac == NULL || unique == 0 ) \n" ) ;
		return false ;
	}

	try{		

		if( pCharac->CHARAC_INVENTORY.sVersion != 1 )
		{
			::PrintConsole( "[UPDATE_ERROR] Bad inven update_v3(chr:%s, ver:%d) \n", pCharac->CHARAC_BASIC.cChrName, pCharac->CHARAC_INVENTORY.sVersion ) ;
			return false ;
		}

		HRESULT hr;
		m_pUpdateInven_v3->ResetAccessor() ;

		m_pUpdateInven_v3->GetAccessor()->m_unique_id = unique;		
		m_pUpdateInven_v3->GetAccessor()->m_money = static_cast<__int64>(pCharac->CHARAC_INVENTORY.iMoney);
		memcpy(&m_pUpdateInven->GetAccessor()->m_data, &pCharac->CHARAC_INVENTORY, sizeof(_CHARAC_INVENTORY));

		hr = m_pUpdateInven_v3->OpenCommand(*m_session);
		m_pUpdateInven_v3->CloseCommand() ;
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}

		_ItemLog * pItemLog =  g_pLogSvrCom->GetItemLogSlot() ;
		if( pItemLog )
		{
			memset( pItemLog, 0, sizeof(_ItemLog) ) ;

			pItemLog->item_uid = 0 ;
			pItemLog->code = _LOG_ITEM_DS_UPDATE_MONEY_ ;
			pItemLog->zone = pCharac->CHARAC_STATE.sInnerLevel ;
			sprintf( pItemLog->from, "DS->INVEN" ) ;
			pItemLog->to_uid = pCharac->CHARAC_BASIC.iUniqueID ;
			memcpy( pItemLog->to, pCharac->CHARAC_BASIC.cChrName, 13 ) ;
			pItemLog->money = pCharac->CHARAC_INVENTORY.iMoney ;
			pItemLog->type1 = 0 ;
			pItemLog->type2 = 0 ;

			g_pLogSvrCom->InsertItemLog( pItemLog ) ;

		}

		return true;


	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::inven_update_v3 \n") ;
		m_pUpdateInven_v3->CloseCommand() ;
		return false;
	}
}
*/

// 퀘스트
bool COLE_DB_Executer::pr_Update_QuestInfo( int uid, _C_CHARAC * pCharac )
{
	
	try{

		HRESULT hr;
		m_pUpdateQuest->ResetAccessor() ;

		m_pUpdateQuest->GetAccessor()->m_unique_id = uid;
		
#ifdef _PD_QUEST_MAX_600_
		memcpy(m_pUpdateQuest->GetAccessor()->m_QuestData, &pCharac->CHARAC_QUEST_DATA, sizeof(pCharac->CHARAC_QUEST_DATA)-sizeof(_CHARAC_QUEST_DATA::__quest_data2));
		memcpy(m_pUpdateQuest->GetAccessor()->m_QuestData2, &pCharac->CHARAC_QUEST_DATA.QuestData2, sizeof(pCharac->CHARAC_QUEST_DATA.QuestData2) );
#else
		memcpy(m_pUpdateQuest->GetAccessor()->m_QuestData, &pCharac->CHARAC_QUEST_DATA, sizeof(pCharac->CHARAC_QUEST_DATA));
#endif



		hr = m_pUpdateQuest->OpenCommand(*m_session);
		m_pUpdateQuest->CloseCommand() ;


		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}

		return true;

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_Update_QuestInfo \n") ;
		m_pUpdateQuest->CloseCommand() ;
		return false;
	}
	
	
}

bool COLE_DB_Executer::pr_Get_QuestInfo( int uid, _C_CHARAC * pCharac )
{
	
	try{

		HRESULT hr;
		m_pGetQuest->ResetAccessor() ;

		m_pGetQuest->GetAccessor()->m_unique_id = uid;




		hr = m_pGetQuest->OpenCommand(*m_session);
		m_pGetQuest->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}
#ifdef _PD_QUEST_MAX_600_
		memcpy(&pCharac->CHARAC_QUEST_DATA, m_pGetQuest->GetAccessor()->m_QuestData, sizeof(pCharac->CHARAC_QUEST_DATA)-sizeof(_CHARAC_QUEST_DATA::__quest_data2));
		memcpy(&pCharac->CHARAC_QUEST_DATA.QuestData2, m_pGetQuest->GetAccessor()->m_QuestData2, sizeof(_CHARAC_QUEST_DATA::__quest_data2));
#else

		memcpy(&pCharac->CHARAC_QUEST_DATA, m_pGetQuest->GetAccessor()->m_QuestData, sizeof(pCharac->CHARAC_QUEST_DATA));

#endif

		return true;


	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_Get_QuestInfo \n") ;
		m_pGetQuest->CloseCommand() ;
		return false;
	}
	
	
}

// 무공
bool COLE_DB_Executer::pr_Get_MartialData( int uid, _C_CHARAC * pCharac ){
	
	try{

		HRESULT hr;
		m_pGetMartial->ResetAccessor() ;

		m_pGetMartial->GetAccessor()->m_unique_id = uid;

		hr = m_pGetMartial->OpenCommand(*m_session);
		m_pGetMartial->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}

		memcpy( &pCharac->CHARAC_MARTIAL_DATA, &m_pGetMartial->GetAccessor()->m_data, sizeof(_CHARAC_MARTIAL_DATA));   

		return true;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_Get_MartialData \n") ;
		m_pGetMartial->CloseCommand() ;
		return false;
	}

	
}

bool COLE_DB_Executer::pr_Update_MartialData( int uid, _C_CHARAC * pCharac ){

	
	try{

		HRESULT hr;
		m_pUpdateMartial->ResetAccessor() ;

		m_pUpdateMartial->GetAccessor()->m_unique_id = uid;
		memcpy( &m_pUpdateMartial->GetAccessor()->m_data, &pCharac->CHARAC_MARTIAL_DATA, sizeof(_CHARAC_MARTIAL_DATA));   



		hr = m_pUpdateMartial->OpenCommand(*m_session);
		m_pUpdateMartial->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}


		return true;

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_Update_MartialData \n") ;
		m_pUpdateMartial->CloseCommand() ;
		return false;
	}

	
}

bool COLE_DB_Executer::Update_Charac_All( int uid, _C_CHARAC * pCharac )
{
	if( uid <= 0 || pCharac == NULL )
	{
		::PrintConsole( "[ERROR] COLE_DB_Executer::Update_Charac_All -> if( uid <= 0 || pCharac == NULL ) \n" ) ;
		return false ;
	}

	try{
		if( pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED == BS_COMPLETED )
		{
			return true ;
		}

		bool result;		

		if( pCharac->CHARAC_ETC._backup.bBackupStateBasic == BS_READY ) {
			result = UpdateCharac(uid, pCharac);
			if(result == false) {
				::PrintConsole("[DB_ERR] UpdateCharac(uid, pCharac) (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName);
				//return false;			
			}
			pCharac->CHARAC_ETC._backup.bBackupStateBasic = BS_COMPLETED ;		

//			g_pTrace->OutputString( _TRACE_CLASS_DB, "[INFO] 'UpdateCharac()' (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName) ;

		}


		if( pCharac->CHARAC_ETC._backup.bBackupStateLevel == BS_READY ) {
			result = pr_Update_LevelAll( uid, pCharac );
			if(result == false) {
				::PrintConsole("[DB_ERR] pr_Update_LevelAll( uid, pCharac ) (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName);
				//return false;
			}
			pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_COMPLETED ;


//			g_pTrace->OutputString( _TRACE_CLASS_DB, "[INFO] 'pr_Update_LevelAll()' (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName) ;

		}


		if ( pCharac->CHARAC_ETC._backup.bBackupStateState == BS_READY ) {
			result = pr_Update_CharacState( uid, pCharac );
			if(result == false) {
				::PrintConsole("[DB_ERR] pr_Update_CharacState( uid, pCharac ) (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName);
				//return false;
			}
			pCharac->CHARAC_ETC._backup.bBackupStateState = BS_COMPLETED ;


//			g_pTrace->OutputString( _TRACE_CLASS_DB, "[INFO] 'pr_Update_CharacState()' (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName) ;

		}

		if ( pCharac->CHARAC_ETC._backup.bBackupStateCurBasicInfo == BS_READY ) {

			result = pr_Update_CurBasicInfo( uid, pCharac );
			if(result == false) {
				::PrintConsole("[DB_ERR] pr_Update_CurBasicInfo( uid, pCharac ) (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName);
				//return false;	
			}
			pCharac->CHARAC_ETC._backup.bBackupStateCurBasicInfo = BS_COMPLETED ;


//			g_pTrace->OutputString( _TRACE_CLASS_DB, "[INFO] 'pr_Update_CurBasicInfo()' (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName) ;

		}

		if ( pCharac->CHARAC_ETC._backup.bBackupStateInven == BS_READY ) {
			result = inven_update( uid, pCharac );
			if(result == false) {
				::PrintConsole("[DB_ERR] inven_update( uid, pCharac ) (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName);
				//return false;
			}
			pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_COMPLETED ;


//			g_pTrace->OutputString( _TRACE_CLASS_DB, "[INFO] 'inven_update_v02()' (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName) ;

		}


		if( pCharac->CHARAC_ETC._backup.bBackupStateQuest == BS_READY )	{
			result = pr_Update_QuestInfo( uid, pCharac );
			if(result == false) {
				::PrintConsole("[DB_ERR] pr_Update_QuestInfo( uid, pCharac ) (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName);
				//return false;
			}
			pCharac->CHARAC_ETC._backup.bBackupStateQuest = BS_COMPLETED ;


//			g_pTrace->OutputString( _TRACE_CLASS_DB, "[INFO] 'pr_Update_QuestInfo()' (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName) ;

		}


		if( pCharac->CHARAC_ETC._backup.bBackupStateMartial == BS_READY ) {
			result = pr_Update_MartialData( uid, pCharac );
			if(result == false) {
				::PrintConsole("[DB_ERR] pr_Update_MartialData( uid, pCharac ) (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName);
				//return false;
			}
			pCharac->CHARAC_ETC._backup.bBackupStateMartial = BS_COMPLETED ;


//			g_pTrace->OutputString( _TRACE_CLASS_DB, "[INFO] 'pr_Update_MartialData()' (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName) ;

		}


		if( pCharac->CHARAC_ETC._backup.bBackupStateNic == BS_READY ) {
			result = SetNic( uid, pCharac );
			if(result == false) {
				::PrintConsole("[DB_ERR] SetNic( uid, pCharac ) (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName);
				//return false;
			}
			pCharac->CHARAC_ETC._backup.bBackupStateNic = BS_COMPLETED ;


//			g_pTrace->OutputString( _TRACE_CLASS_DB, "[INFO] 'SetNic( uid, pCharac )' (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName) ;

		}


		if( pCharac->CHARAC_ETC._backup.bBackupStateEffect == BS_READY ) 
		{
			result = SetEffect( uid, pCharac );
			if(result == false) {
				::PrintConsole("[DB_ERR] SetEffect( uid, pCharac ) (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName);
				//return false;
			}
			pCharac->CHARAC_ETC._backup.bBackupStateEffect = BS_COMPLETED ;


//			g_pTrace->OutputString( _TRACE_CLASS_DB, "[INFO] 'SetEffect( uid, pCharac )' (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName) ;

		}

		if( pCharac->CHARAC_ETC._backup.bBackupStatePkRecord == BS_READY )
		{
			result = UpdatePkRecord( uid, pCharac );
			if(result == false) {
				::PrintConsole("[DB_ERR] UpdatePkRecord( uid, pCharac ) (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName);
				//return false;
			}
			pCharac->CHARAC_ETC._backup.bBackupStatePkRecord = BS_COMPLETED ;


//			g_pTrace->OutputString( _TRACE_CLASS_DB, "[INFO] 'UpdatePkRecord( uid, pCharac )' (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName) ;
		}

		if( pCharac->CHARAC_ETC._backup.bBackupStateCharacItemUseStatus == BS_READY )
		{
			result = UpdateCharacItemUseStatus( uid, pCharac );
			if(result == false) {
				::PrintConsole("[DB_ERR] UpdateCharacItemUseStatus( uid, pCharac ) (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName);
				//return false;
			}
			pCharac->CHARAC_ETC._backup.bBackupStateCharacItemUseStatus = BS_COMPLETED ;


//			g_pTrace->OutputString( _TRACE_CLASS_DB, "[INFO] 'UpdateCharacItemUseStatus( uid, pCharac )' (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName) ;
		}

		if( pCharac->CHARAC_ETC._backup.bBackupStateMatch == BS_READY )
		{
			result = SetMatch( uid, pCharac ) ;
			if( result == false )
			{
				::PrintConsole("[DB_ERR] SetMatch( uid, pCharac ) (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName);
				//return false;
			}
			pCharac->CHARAC_ETC._backup.bBackupStateMatch = BS_COMPLETED ;

//			g_pTrace->OutputString( _TRACE_CLASS_DB, "[INFO] 'SetMatch( uid, pCharac )' (chr : %s)\n", pCharac->CHARAC_BASIC.cChrName) ;
		}

		result = UpdateCharacQuickSlot( uid, pCharac );
		if( result == false )
		{
			PrintConsole( "[DB_ERR] UpdateCharacQuickSlot : C<%s>\n", pCharac->CHARAC_BASIC.cChrName ) ;
			// 이건 백업이 안되어도 그냥 넘어간다. 저장이 꼭 되야 하는 것은 아니다. 
		}

#ifdef _PD_CHARAC_LTS_
		result = SetCharacLts( uid, pCharac );
		if( false == result )
			PrintConsole( "[DB_ERR] SetCharacLts : C<%s>\n", pCharac->CHARAC_BASIC.cChrName );
#endif

#ifdef _PD_PKEVENT_BOX_SPAWN_
		UpdateBWWarWinTime( uid, pCharac->CHARAC_BW_WAR.iWinTime );
#endif

		pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED = BS_COMPLETED;

		return true;	
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::Update_Charac_All \n " ) ;
		return false ;
	}

}

bool COLE_DB_Executer::pr_GetDepot(int uid, CUser * pUser)
{

	if( uid <= 0 || pUser == NULL ) return false ;


	try{

		C_prDs_GetDepot			GetDepot;

		HRESULT hr;		
		GetDepot.ResetAccessor() ;

		GetDepot.GetAccessor()->m_unique_id = uid;


		hr = GetDepot.OpenCommand(*m_session);
		GetDepot.CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}	

		pUser->user_lock() ;
		try
		{
			memset( &pUser->stUserDepot, 0, sizeof(pUser->stUserDepot) ) ;
			if( GetDepot.GetAccessor()->m_d_money >= static_cast<__int64>(0xffffffffU) )
				pUser->stUserDepot.iMoney = 0xffffffffU;
			else
				pUser->stUserDepot.iMoney = static_cast<u_int>(GetDepot.GetAccessor()->m_d_money);
			pUser->stUserDepot.cCheckCount = GetDepot.GetAccessor()->m_check_count ;
			pUser->stUserDepot.sCheckYear = GetDepot.GetAccessor()->m_check_year ;
			pUser->stUserDepot.cCheckMonth = GetDepot.GetAccessor()->m_check_month ;
			pUser->stUserDepot.cCheckDay = GetDepot.GetAccessor()->m_check_day ;
			pUser->stUserDepot.cCheckHour = GetDepot.GetAccessor()->m_check_hour ;
			memcpy( &pUser->stUserDepot.cDepot[0], &GetDepot.GetAccessor()->m_depot[0], sizeof(pUser->stUserDepot.cDepot) );
#ifdef _PD_CASH_INVEN_
			memcpy( &pUser->stUserDepot.cExtendDepot[0], &GetDepot.GetAccessor()->m_extend_depot[0], sizeof(pUser->stUserDepot.cExtendDepot) );
#endif


			pUser->stUserDepot.sVersion = GetDepot.GetAccessor()->m_version ;

			if( pUser->stUserDepot.sVersion == 0 )
			{
				_sServer_InvenItem inven[_DEPOT_SLOT_NUM] = {0};
				memcpy( &inven[0], &pUser->stUserDepot.cDepot[0], sizeof(inven) ) ;

				memset( &pUser->stUserDepot.cDepot[0], 0, sizeof(pUser->stUserDepot.cDepot) ) ;

				_sServer_InvenItem_v1 * pInvenV1 = reinterpret_cast<_sServer_InvenItem_v1*>(&pUser->stUserDepot.cDepot[0]) ;
				for( int i = 0; i < _DEPOT_SLOT_NUM ; ++i )
				{
					if( inven[i].sID )
					{
						pInvenV1[i].convert_from_v0( &inven[i] ) ;
					}
				}

				pUser->stUserDepot.sVersion = 1 ;
			}
		}
		catch(...)
		{
			::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}
		pUser->user_unlock() ;


	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_GetDepot \n") ;
		//GetDepot.CloseCommand() ;
		return false ;
	}

	return true ;
}

bool COLE_DB_Executer::pr_UpdateDepot(int uid, CUser * pUser)
{

	if( pUser->stUserDepot.sVersion != 1 )
	{
		::PrintConsole("[UPDATE_ERROR] if( pUser->stUserDepot.sVersion != 1 ) (%s, %d)\n"
			, pUser->cNDAccount, pUser->stUserDepot.sVersion ) ;
		return false ;
	}

	if( uid <= 0 || pUser == NULL ) return false ;
	
	try{

		HRESULT hr;
		m_pUpdateDepot->ResetAccessor() ;

		_sServer_InvenItem_v1 * pInvenV1 = reinterpret_cast<_sServer_InvenItem_v1*>(&pUser->stUserDepot.cDepot[0]) ;

		m_pUpdateDepot->GetAccessor()->m_unique_id = uid;
		m_pUpdateDepot->GetAccessor()->m_d_money = static_cast<__int64>(pUser->stUserDepot.iMoney);	
		memcpy(m_pUpdateDepot->GetAccessor()->m_depot , pUser->stUserDepot.cDepot, sizeof(pUser->stUserDepot.cDepot));
#ifdef _PD_CASH_INVEN_
		memcpy(m_pUpdateDepot->GetAccessor()->m_extend_depot, pUser->stUserDepot.cExtendDepot, sizeof(pUser->stUserDepot.cExtendDepot) );
#endif
		m_pUpdateDepot->GetAccessor()->m_check_count = pUser->stUserDepot.cCheckCount ;
		m_pUpdateDepot->GetAccessor()->m_check_year = pUser->stUserDepot.sCheckYear ;
		m_pUpdateDepot->GetAccessor()->m_check_month = pUser->stUserDepot.cCheckMonth ;
		m_pUpdateDepot->GetAccessor()->m_check_day  = pUser->stUserDepot.cCheckDay ;
		m_pUpdateDepot->GetAccessor()->m_check_hour  = pUser->stUserDepot.cCheckHour ;

		m_pUpdateDepot->GetAccessor()->m_version = pUser->stUserDepot.sVersion ;
		hr = m_pUpdateDepot->OpenCommand(*m_session);
		m_pUpdateDepot->CloseCommand() ;		



		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();
			return false;
		}
		return true;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::pr_UpdateDepot (%s) \n", pUser->cNDAccount) ;
		m_pUpdateDepot->CloseCommand() ;
		return false;
	}
	
}


bool COLE_DB_Executer::UpdateCharac(int uid, _C_CHARAC * pCharac ) {
	

	try{


		HRESULT hr;

		m_pUpdateCharac->ResetAccessor() ;

		
		m_pUpdateCharac->GetAccessor()->m_uniqueID = uid;
		memcpy(m_pUpdateCharac->GetAccessor()->m_chr_nic, pCharac->CHARAC_BASIC.cChrNic, 13);
		m_pUpdateCharac->GetAccessor()->m_party = pCharac->CHARAC_BASIC.cParty;
		memcpy(m_pUpdateCharac->GetAccessor()->m_gamete, pCharac->CHARAC_BASIC.cGamete, 13);
		m_pUpdateCharac->GetAccessor()->m_hair = pCharac->CHARAC_BASIC.cHair;
		m_pUpdateCharac->GetAccessor()->m_face = pCharac->CHARAC_BASIC.cFace;
		m_pUpdateCharac->GetAccessor()->m_class = pCharac->CHARAC_BASIC.cClass ;
		m_pUpdateCharac->GetAccessor()->m_classGrade = pCharac->CHARAC_BASIC.cClassGrade ;
		m_pUpdateCharac->GetAccessor()->m_iContribution = pCharac->CHARAC_BASIC.iContribution ;
		m_pUpdateCharac->GetAccessor()->m_gm = pCharac->CHARAC_BASIC.cGM_Check ;
		m_pUpdateCharac->GetAccessor()->m_state = pCharac->CHARAC_ETC.cState;
		m_pUpdateCharac->GetAccessor()->m_dwPlayTime = pCharac->CHARAC_BASIC.dwPlayTime ;


		hr = m_pUpdateCharac->OpenCommand(*m_session);
		m_pUpdateCharac->CloseCommand() ;



		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();
			return false;
		}

		return true;


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::UpdateCharac \n") ;
		m_pUpdateCharac->CloseCommand() ;
		return false;
	}

	

}


bool COLE_DB_Executer::CharacRename( char * cCurName, char * cNewName )
{
	try{


		HRESULT hr;

		m_pCharacRename->ResetAccessor() ;

		memcpy( m_pCharacRename->GetAccessor()->m_cCurName, cCurName, 13 ) ;
		memcpy( m_pCharacRename->GetAccessor()->m_cNewName, cNewName, 13 ) ;


		hr = m_pCharacRename->OpenCommand(*m_session);
		m_pCharacRename->CloseCommand() ;


		// 중복된 이름..
		if( hr == DB_E_INTEGRITYVIOLATION )
		{
			m_pCharacRename->CloseCommand() ;
			return false ;
		}


		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();
		}

		
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::CharacRename \n") ;
		m_pCharacRename->CloseCommand() ;	
	}

	return  true ;

}


void COLE_DB_Executer::GetAccName(char * cCharacName, char * cAccName )
{
	try{


		HRESULT hr;

		m_pGetAccName->ResetAccessor() ;

		memcpy( m_pGetAccName->GetAccessor()->m_cCharacName, cCharacName, 13 ) ;
		

		hr = m_pGetAccName->OpenCommand(*m_session);
		m_pGetAccName->CloseCommand() ;


		memcpy(cAccName, m_pGetAccName->GetAccessor()->m_cAccName, en_max_lil+1) ;


		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) 
				CallReConTh();			
		}


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::CharacRename \n") ;
		m_pGetAccName->CloseCommand() ;
	}

}


bool COLE_DB_Executer::GetNic( int uid, _C_CHARAC * pCharac ) 
{
	try{


		HRESULT hr;

		m_pGetNic->ResetAccessor() ;
		
		m_pGetNic->GetAccessor()->m_unique_id = uid ;

		hr = m_pGetNic->OpenCommand(*m_session);
		m_pGetNic->CloseCommand() ;

		memcpy( &pCharac->CHARAC_NIC, &m_pGetNic->GetAccessor()->m_data, sizeof( _CHARAC_NIC ) ) ;


		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::GetNic \n") ;
		m_pGetNic->CloseCommand() ;
		return false ;
	}
	return true ;
}

bool COLE_DB_Executer::SetNic( int uid, _C_CHARAC * pCharac ) 
{
	try{


		HRESULT hr;

		m_pSetNic->ResetAccessor() ;

		m_pSetNic->GetAccessor()->m_unique_id = uid ;
		memcpy( &m_pSetNic->GetAccessor()->m_data, &pCharac->CHARAC_NIC, sizeof( _CHARAC_NIC ) ) ;

		hr = m_pSetNic->OpenCommand(*m_session);
		m_pSetNic->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::SetNic \n") ;
		m_pSetNic->CloseCommand() ;
		return false ;
	}
	return true ;
}



bool COLE_DB_Executer::GetMatch(int uid, _C_CHARAC * pCharac )
{
	try{

		HRESULT hr;

		m_pGetMatch->ResetAccessor() ;

		m_pGetMatch->GetAccessor()->m_unique_id = uid ;	

		hr = m_pGetMatch->OpenCommand(*m_session);
		m_pGetMatch->CloseCommand() ;

		memcpy( &pCharac->CHARAC_MATCH, &m_pGetMatch->GetAccessor()->m_data, sizeof( _CHARAC_MATCH ) ) ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::GetMatch \n") ;
		m_pGetMatch->CloseCommand() ;	
		return false ;
	}
	return true ;

}


bool COLE_DB_Executer::SetMatch(int uid, _C_CHARAC * pCharac )
{
	
	try{

		HRESULT hr;

		m_pSetMatch->ResetAccessor() ;

		m_pSetMatch->GetAccessor()->m_unique_id = uid ;
		memcpy( &m_pSetMatch->GetAccessor()->m_data, &pCharac->CHARAC_MATCH, sizeof( _CHARAC_MATCH ) ) ;

		hr = m_pSetMatch->OpenCommand(*m_session);
		m_pSetMatch->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::GetMatch \n") ;
		m_pSetMatch->CloseCommand() ;		
		return false ;
	}
	return true ;


}

#ifdef _PD_BUFF_COUNT_MODIFY_

bool COLE_DB_Executer::GetEffect(int uid, _C_CHARAC * pCharac )
{

	try{

		HRESULT hr;

		m_pGetEffect2->ResetAccessor() ;

		m_pGetEffect2->GetAccessor()->m_unique_id = uid ;	

		hr = m_pGetEffect2->OpenCommand(*m_session);
		m_pGetEffect2->CloseCommand() ;

		memcpy( &pCharac->CHARAC_EFFECT, &m_pGetEffect2->GetAccessor()->m_data, sizeof( _CHARAC_EFFECT ) ) ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::GetEffect2 \n") ;
		m_pGetEffect2->CloseCommand() ;
		return false ;
	}
	return true ;
}

bool COLE_DB_Executer::SetEffect(int uid, _C_CHARAC * pCharac )
{

	try{

		HRESULT hr;

		m_pSetEffect2->ResetAccessor() ;

		m_pSetEffect2->GetAccessor()->m_unique_id = uid ;
		memcpy( &m_pSetEffect2->GetAccessor()->m_data, &pCharac->CHARAC_EFFECT, sizeof( _CHARAC_EFFECT ) ) ;

		hr = m_pSetEffect2->OpenCommand(*m_session);
		m_pSetEffect2->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::SetEffect2 \n") ;
		m_pSetEffect2->CloseCommand() ;
		return false ;
	}
	return true ;
}

#else

bool COLE_DB_Executer::GetEffect(int uid, _C_CHARAC * pCharac )
{

	try{

		HRESULT hr;

		m_pGetEffect->ResetAccessor() ;

		m_pGetEffect->GetAccessor()->m_unique_id = uid ;	

		hr = m_pGetEffect->OpenCommand(*m_session);
		m_pGetEffect->CloseCommand() ;

		memcpy( &pCharac->CHARAC_EFFECT, &m_pGetEffect->GetAccessor()->m_data, sizeof( _CHARAC_EFFECT ) ) ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::GetEffect \n") ;
		m_pGetEffect->CloseCommand() ;
		return false ;
	}
	return true ;
}

bool COLE_DB_Executer::SetEffect(int uid, _C_CHARAC * pCharac )
{

	try{

		HRESULT hr;

		m_pSetEffect->ResetAccessor() ;

		m_pSetEffect->GetAccessor()->m_unique_id = uid ;
		memcpy( &m_pSetEffect->GetAccessor()->m_data, &pCharac->CHARAC_EFFECT, sizeof( _CHARAC_EFFECT ) ) ;

		hr = m_pSetEffect->OpenCommand(*m_session);
		m_pSetEffect->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::SetEffect \n") ;
		m_pSetEffect->CloseCommand() ;
		return false ;
	}
	return true ;
}

#endif

bool COLE_DB_Executer::GetPkRecord(int uid, _C_CHARAC * pCharac )
{
	try{

		HRESULT hr;

		m_pGetPkRecord->ResetAccessor() ;

		m_pGetPkRecord->GetAccessor()->m_unique_id = uid ;	

		hr = m_pGetPkRecord->OpenCommand(*m_session);
		m_pGetPkRecord->CloseCommand() ;

		memcpy( &pCharac->CHARAC_PK_RECORD, &m_pGetPkRecord->GetAccessor()->m_data, sizeof( _CHARAC_PK_RECORD ) ) ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::GetPkRecord \n") ;
		m_pGetPkRecord->CloseCommand() ;
		return false ;
	}
	return true ;
}


bool COLE_DB_Executer::UpdatePkRecord(int uid, _C_CHARAC * pCharac )
{

	try{

		HRESULT hr;

		m_pUpdatePkRecord->ResetAccessor() ;

		m_pUpdatePkRecord->GetAccessor()->m_unique_id = uid ;
		memcpy( &m_pUpdatePkRecord->GetAccessor()->m_data, &pCharac->CHARAC_PK_RECORD, sizeof( _CHARAC_PK_RECORD ) ) ;

		hr = m_pUpdatePkRecord->OpenCommand(*m_session);
		m_pUpdatePkRecord->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::UpdatePkRecord \n") ;
		m_pUpdatePkRecord->CloseCommand() ;
		return false ;
	}
	return true ;
}


bool COLE_DB_Executer::GetCharacItemUseStatus( int cid, _C_CHARAC * pCharac )
{
	try{

		HRESULT hr;

		m_pGetCharacItemUseStatus->ResetAccessor() ;

		m_pGetCharacItemUseStatus->GetAccessor()->SetUniqueID( cid ) ;
		
		hr = m_pGetCharacItemUseStatus->OpenCommand(*m_session);
		m_pGetCharacItemUseStatus->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}

		pCharac->CHARAC_ITEM_STATUS.shout_left_time = m_pGetCharacItemUseStatus->GetAccessor()->GetShoutLeftTime() ;
		pCharac->CHARAC_ITEM_STATUS.shout_type = m_pGetCharacItemUseStatus->GetAccessor()->GetShoutType() ;
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::GetCharacItemUseStatus\n") ;
		m_pGetCharacItemUseStatus->CloseCommand() ;
		return false ;
	}
	return true ;
}

bool COLE_DB_Executer::UpdateCharacItemUseStatus( int cid, _C_CHARAC * pCharac )
{
	bool bRet = true ;
	try{
		HRESULT hr;

		m_pUpdateCharacItemUseStatus->ResetAccessor() ;

		m_pUpdateCharacItemUseStatus->GetAccessor()->SetUniqueID( cid ) ;
		m_pUpdateCharacItemUseStatus->GetAccessor()->SetShoutLeftTime( pCharac->CHARAC_ITEM_STATUS.shout_left_time ) ;
		m_pUpdateCharacItemUseStatus->GetAccessor()->SetShoutType( pCharac->CHARAC_ITEM_STATUS.shout_type ) ;

		hr = m_pUpdateCharacItemUseStatus->OpenCommand(*m_session);
		m_pUpdateCharacItemUseStatus->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			bRet = false ;
		}
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::UpdateCharacItemUseStatus\n") ;
		m_pUpdateCharacItemUseStatus->CloseCommand() ;
		bRet = false ;
	}

	return bRet ;
}

bool COLE_DB_Executer::GetAccountItemUseStatus( int aid, CUser * pUser )
{
	bool bRet = true ;

	try
	{
		HRESULT hr ;
		m_pGetAccountItemUseStatus->ResetAccessor() ;

		_db_pr_DS_GetAccountItemUseStatus * pAccessor = m_pGetAccountItemUseStatus->GetAccessor() ;
		pAccessor->SetUniqueID( aid ) ;

		hr = m_pGetAccountItemUseStatus->OpenCommand( *m_session ) ;
		m_pGetAccountItemUseStatus->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			bRet = false ;
		}
		else
		{
			// 성공이면,
			pUser->AIUS.bChanged = false ;
			pUser->AIUS.grade.ucReserve = pAccessor->GetReserve() ;
			pUser->AIUS.grade.cItemType = pAccessor->GetItemType() ;
			pUser->AIUS.grade.sItemID = pAccessor->GetItemID() ;
			pUser->AIUS.uiEndTime = pAccessor->GetEndTime() ;

			SetRealTime( pUser->AIUS.uiEndTime, pUser->AIUS.grade ) ;
		}

		g_pTrace->OutputString( _TRACE_CLASS_UPDATE, "[AIUS] A<%s> Get Item %d-%d, Time %u\n", 
			pUser->cNDAccount, pUser->AIUS.grade.cItemType, pUser->AIUS.grade.sItemID, pUser->AIUS.uiEndTime ) ;
	}
	catch( ... )
	{
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::GetAccountItemUseStatus\n") ;
		m_pGetAccountItemUseStatus->CloseCommand() ;
		bRet = false ;
	}

	return bRet ;
}

bool COLE_DB_Executer::UpdateAccountItemUseStatus( int aid, CUser * pUser )
{
	bool bRet = true ;
	char date[17] = "2006-08-01 0:0" ;

	try
	{
		HRESULT hr ;
		m_pUpdateAccountItemUseStatus->ResetAccessor() ;

		_db_pr_DS_UpdateAccountItemUseStatus * pAccessor = m_pUpdateAccountItemUseStatus->GetAccessor() ;
		_sND_AIUS_Grade& gr = pUser->AIUS.grade ;
		pAccessor->SetUniqueID( aid ) ;
		pAccessor->SetReserve( gr.ucReserve ) ;
		pAccessor->SetItemType( gr.cItemType ) ;
		pAccessor->SetItemID( gr.sItemID ) ;
		pAccessor->SetEndTime( pUser->AIUS.uiEndTime ) ;

		if( pUser->AIUS.bChanged )
		{
			// '2006-06-29 20:24'
			sprintf( date, "%u-%2u-%2u %2u:%2u", 
				static_cast<u_int>(gr.uiYear) + 2000U, gr.uiMonth, gr.uiDay,
				gr.uiHour, gr.uiMinute ) ;
			pAccessor->SetFlag( 1 ) ;
		}
		else
			pAccessor->SetFlag( 0 ) ;

		pAccessor->SetDateString( date ) ;

		hr = m_pUpdateAccountItemUseStatus->OpenCommand( *m_session ) ;
		m_pUpdateAccountItemUseStatus->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			bRet = false ;
		}

		g_pTrace->OutputString( _TRACE_CLASS_UPDATE, "[AIUS] A<%s> Update Item %d-%d, Time %u : date %s\n", 
			pUser->cNDAccount, gr.cItemType, gr.sItemID, pUser->AIUS.uiEndTime, date ) ;
	}
	catch( ... )
	{
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::GetAccountItemUseStatus\n") ;
		m_pUpdateAccountItemUseStatus->CloseCommand() ;
		bRet = false ;
	}

	return bRet ;
}

bool COLE_DB_Executer::GetCharacQuickSlot( int cid, _C_CHARAC * pCharac )
{
	bool bRet = NULL ;

	try{

		HRESULT hr;
		m_pGetQuickSlot->ResetAccessor() ;

		m_pGetQuickSlot->GetAccessor()->SetCharacUID(cid);

		hr = m_pGetQuickSlot->OpenCommand(*m_session);
		m_pGetQuickSlot->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) 
				CallReConTh();

			ZeroMemory( pCharac->CHARAC_ETC_QUICKSLOT.m_QuickSlot, sizeof(pCharac->CHARAC_ETC_QUICKSLOT.m_QuickSlot) ) ;
			ZeroMemory( pCharac->CHARAC_ETC_QUICKSLOT.m_helplog, sizeof(pCharac->CHARAC_ETC_QUICKSLOT.m_helplog) ) ;

			bRet = false;
		}
		else
		{
			m_pGetQuickSlot->GetAccessor()->GetQuickSlot( pCharac->CHARAC_ETC_QUICKSLOT.m_QuickSlot );
			m_pGetQuickSlot->GetAccessor()->GetHelpLog( pCharac->CHARAC_ETC_QUICKSLOT.m_helplog );
			bRet = true ;
		}
	}
	catch (...) {
		PrintConsole("[EXCEPTION] COLE_DB_Executer::GetCharacQuickSlot\n") ;
		m_pGetQuickSlot->CloseCommand() ;
		ZeroMemory( pCharac->CHARAC_ETC_QUICKSLOT.m_QuickSlot, sizeof(pCharac->CHARAC_ETC_QUICKSLOT.m_QuickSlot) ) ;
		ZeroMemory( pCharac->CHARAC_ETC_QUICKSLOT.m_helplog, sizeof(pCharac->CHARAC_ETC_QUICKSLOT.m_helplog) ) ;
		bRet = false;
	}

	return bRet ;	
}


bool COLE_DB_Executer::UpdateCharacQuickSlot( int cid, _C_CHARAC * pCharac )
{
	bool bRet = NULL ;

	try{

		HRESULT hr;
		m_pUpdateQuickSlot->ResetAccessor() ;

		m_pUpdateQuickSlot->GetAccessor()->SetCharacUID(cid);
		m_pUpdateQuickSlot->GetAccessor()->SetQuickSlot(pCharac->CHARAC_ETC_QUICKSLOT.m_QuickSlot);
		m_pUpdateQuickSlot->GetAccessor()->SetHelplog(pCharac->CHARAC_ETC_QUICKSLOT.m_helplog);

		hr = m_pUpdateQuickSlot->OpenCommand(*m_session);
		m_pUpdateQuickSlot->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			bRet = false;
		}
		else
		{
			bRet = true ;
		}
	}
	catch (...) {
		PrintConsole("[EXCEPTION] COLE_DB_Executer::UpdateCharacQuickSlot\n") ;
		m_pUpdateQuickSlot->CloseCommand() ;
		bRet = false;
	}

	return bRet ;
	
}


#ifdef _PD_CHARAC_LTS_
bool COLE_DB_Executer::GetCharacLts( int uid, _C_CHARAC * pCharac )
{
	bool bRet = false ;

	try{

		HRESULT hr;
		m_pGetLts->ResetAccessor() ;

		m_pGetLts->GetAccessor()->set_charac_uid( uid );

		hr = m_pGetLts->OpenCommand(*m_session);
		m_pGetLts->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) 
				CallReConTh();

			ZeroMemory( &pCharac->CHARAC_LTS, sizeof(pCharac->CHARAC_LTS) ) ;
	
			bRet = false;
		}
		else
		{
			if( 1 == m_pGetLts->GetAccessor()->get_result() )
			{
				// fail.
				ZeroMemory( &pCharac->CHARAC_LTS, sizeof(pCharac->CHARAC_LTS) ) ;
			}
			else
			{
				m_pGetLts->GetAccessor()->get_lts( &pCharac->CHARAC_LTS );
			}

			bRet = true ;
		}
	}
	catch (...) {
		PrintConsole("[EXCEPTION] COLE_DB_Executer::GetCharacLts\n") ;
		m_pGetLts->CloseCommand() ;
		
		bRet = false;
	}

	return bRet ;	
}


bool COLE_DB_Executer::SetCharacLts( int uid, _C_CHARAC * pCharac )
{
	bool bRet = false ;

	try{

		HRESULT hr;
		m_pSetLts->ResetAccessor() ;

		m_pSetLts->GetAccessor()->set_list( uid, &pCharac->CHARAC_LTS );

		hr = m_pSetLts->OpenCommand(*m_session);
		m_pSetLts->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) 
				CallReConTh();

			bRet = false;
		}
		else
		{
			bRet = true ;
		}
	}
	catch (...) {
		PrintConsole("[EXCEPTION] COLE_DB_Executer::SetCharacLts\n") ;
		m_pSetLts->CloseCommand() ;

		bRet = false;
	}

	return bRet ;	
}

int COLE_DB_Executer::GetCharacCUID( char * name )
{
	int ret = 0; 

	try{

		HRESULT hr;
		m_pGetCUID->ResetAccessor() ;

		m_pGetCUID->GetAccessor()->set_charac_name( name );

		hr = m_pGetCUID->OpenCommand(*m_session);
		m_pGetCUID->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) 
				CallReConTh();

			ret = 0;
		}
		else
		{
			ret = m_pGetCUID->GetAccessor()->get_charac_uid();
		}
	}
	catch (...) {
		PrintConsole("[EXCEPTION] COLE_DB_Executer::GetCharacCUID\n") ;
		m_pGetCUID->CloseCommand() ;

		ret = 0;
	}

	return ret;
}


#endif


void COLE_DB_Executer::GetBackupData() 
{
	try{


		HRESULT hr;

		m_pGetBackupData->ResetAccessor() ;

		m_pGetBackupData->GetAccessor()->m_unique_id = 1 ;
		sprintf( m_pGetBackupData->GetAccessor()->m_date_from, "2004-10-05") ;
		sprintf( m_pGetBackupData->GetAccessor()->m_date_to, "2004-10-07") ;
		

		hr = m_pGetBackupData->OpenCommand(*m_session);
		m_pGetBackupData->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			
		}


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] COLE_DB_Executer::SetNic \n") ;
		m_pGetBackupData->CloseCommand() ;		
	}
	
}


int COLE_DB_Executer::InsertOrganization( IN const _Organization & or ) 
{
	try{
		HRESULT hr ;

		m_pInsertOrganization->ResetAccessor() ;

		memcpy( m_pInsertOrganization->GetAccessor()->m_name, or.m_cOrName, en_or_name_length+1 ) ;
		m_pInsertOrganization->GetAccessor()->m_sProp		= or.m_cProperty ;
		m_pInsertOrganization->GetAccessor()->m_sBaseParty	= or.m_cBaseParty ;
		m_pInsertOrganization->GetAccessor()->m_uiExp		= or.m_uiExperience ;
		m_pInsertOrganization->GetAccessor()->m_uiContribution	= or.m_uiContribution ;
		m_pInsertOrganization->GetAccessor()->m_usCurMem	= or.m_usCurMemberNum ;
		memcpy( m_pInsertOrganization->GetAccessor()->m_cCaptainName, or.m_cCaptainName, 13 ) ;
		memcpy( m_pInsertOrganization->GetAccessor()->m_MemListData, or.m_MemberList, _SIZE_OF_OR_MEM_LIST_DATA ) ;

		hr = m_pInsertOrganization->OpenCommand( *m_session ) ;

		// 중복된 이름..
		if( hr == DB_E_INTEGRITYVIOLATION )
		{
			m_pInsertOrganization->CloseCommand() ;
			return -1 ;
		}

		m_pInsertOrganization->CloseCommand() ;

		if(FAILED(hr))
		{
			DBErrorOutput( hr ) ;
			if( hr == E_FAIL) CallReConTh() ;
		}		
	}
	catch(...){
		::PrintConsole("[EXCEPTION] ==> %s(%d) \n",__FILE__, __LINE__) ;
		m_pInsertOrganization->CloseCommand() ;
		m_pInsertOrganization->GetAccessor()->m_index = 0 ;
	}

	return m_pInsertOrganization->GetAccessor()->m_index ;

}

bool COLE_DB_Executer::UpdateOrganization( IN const _Organization & or ) 
{
	HRESULT hr ;
	try{
		m_pUpdateOrganization->ResetAccessor() ;
		m_pUpdateOrganization->GetAccessor()->m_index		= or.m_iIndex ;
		m_pUpdateOrganization->GetAccessor()->m_uiExp		= or.m_uiExperience ;
		m_pUpdateOrganization->GetAccessor()->m_uiContribution	= or.m_uiContribution ;
		m_pUpdateOrganization->GetAccessor()->m_usCurMem	= or.m_usCurMemberNum ;
		memcpy( m_pUpdateOrganization->GetAccessor()->m_cCaptainName, or.m_cCaptainName, 13 ) ;
		memcpy( m_pUpdateOrganization->GetAccessor()->m_MemListData, or.m_MemberList, _SIZE_OF_OR_MEM_LIST_DATA ) ;

		memcpy( m_pUpdateOrganization->GetAccessor()->m_cNotice, or.m_cNotice, sizeof(or.m_cNotice) ) ;
		memcpy( m_pUpdateOrganization->GetAccessor()->m_outTime, or.m_outTime, sizeof( or.m_outTime ) ) ;


		hr = m_pUpdateOrganization->OpenCommand( *m_session ) ;
		m_pUpdateOrganization->CloseCommand() ;

		if(FAILED(hr))
		{
			DBErrorOutput( hr ) ;
			if( hr == E_FAIL) CallReConTh() ;
		}		
	}
	catch(...){
		::PrintConsole("[EXCEPTION] ==> %s(%d) \n",__FILE__, __LINE__) ;
		m_pUpdateOrganization->CloseCommand() ;
		return false ;
	}
	return true ;
}

bool COLE_DB_Executer::DeleteOrganization( int index ) 
{
	try{
		m_pDeleteOrganization->ResetAccessor() ;
		
		m_pDeleteOrganization->GetAccessor()->m_index = index ;

		HRESULT hr = m_pDeleteOrganization->OpenCommand( *m_session ) ;
		m_pDeleteOrganization->CloseCommand() ;

		if(FAILED(hr))
		{
			DBErrorOutput( hr ) ;
			if( hr == E_FAIL) CallReConTh() ;		

			return false ;
		}		

	}
	catch(...){
		::PrintConsole("[EXCEPTION] ==> %s(%d) \n",__FILE__, __LINE__) ;
		m_pDeleteOrganization->CloseCommand() ;
		return false ;
	}
	return true ;
}



bool COLE_DB_Executer::GetRank_Level()
{

	if( g_pRankingManager == NULL )
	{
		return false ;
	}


	/*
	CString			Query = "SELECT   TOP 9 chr_name, inner_level, exp, moonpa, class \
								FROM      ND_V01_Ranking_Level \
								ORDER BY inner_level DESC, exp DESC" ;
								*/
	CString			Query = "SELECT   TOP 9 chr_name, inner_level, exp, moonpa, class FROM ND_V01_Ranking_Level";

	C_Rank_Level 	RankLevel ;
	//CRank_LevelAccessor

	HRESULT hr = RankLevel.OpenCommand( *m_session, Query ) ;
	if( FAILED(hr) )
	{
		DBErrorOutput( hr ) ;
		if(hr==E_FAIL) CallReConTh();	
	}	

	CRankInfo * pRankInfo = g_pRankingManager->GetRankInfoPtr( _ranking::_RANK_TYPE_LEVEL ) ;
	if( pRankInfo == NULL )
	{
		return false ;
	}

	_Rank	rank = {0} ;
	short sRanking = 0 ;

/*
#ifdef _PD_LEVEL_RANK_OVERLAP_
	short insert_slot = 0;
	u_int prev_exp = 0;
	short prev_level = 0;

	while ( RankLevel.GetCustomer()->MoveNext() == S_OK ) 
	{
		memcpy( rank.cCharacName, RankLevel.GetAccessor()->m_cCharacName, 13 );
		rank.sInnerLevel = RankLevel.GetAccessor()->m_inner_level;
		rank.sExp = RankLevel.GetAccessor()->m_exp;
		rank.cParty = static_cast<char>(RankLevel.GetAccessor()->m_moonpa);
		rank.cClass = static_cast<char>(RankLevel.GetAccessor()->m_class);

		if( prev_exp != rank.sExp || prev_level != rank.sInnerLevel )
		{
			prev_level = rank.sInnerLevel;
			prev_exp = rank.sExp;
			sRanking = insert_slot;
		}

		rank.sRanking = sRanking;
		
		pRankInfo->InsertRank( rank, insert_slot );

		++insert_slot;

		
	}
#else
*/
	while ( RankLevel.GetCustomer()->MoveNext() == S_OK ) 
	{
		rank.sRanking = sRanking ;
		++sRanking ;
		memcpy( rank.cCharacName, RankLevel.GetAccessor()->m_cCharacName, 13 ) ;
		rank.sInnerLevel = RankLevel.GetAccessor()->m_inner_level ;
		rank.sExp = RankLevel.GetAccessor()->m_exp ;
		rank.cParty = static_cast<char>(RankLevel.GetAccessor()->m_moonpa) ;
		rank.cClass = static_cast<char>(RankLevel.GetAccessor()->m_class) ;

		pRankInfo->InsertRank( rank ) ;
	}
//#endif


	RankLevel.CloseCommand() ;

	return true ;

}




bool COLE_DB_Executer::GetOrInfo(  OUT COrganizationManager * pOrManager )
{
	CString			szQuery ;
	C_ORSelect		OrSelect ;

#ifdef _PD_ALLY_ORG_STEP_1_
	szQuery = "SELECT  \
			  [index], \
			  [name], \
			  property, \
			  base_party, \
			  [exp], \
			  contribution, \
			  cur_mem, \
			  captain_name, \
			  mem_list, \
			  notice, \
			  out_time, \
			  ally_index \
			  FROM ND_V01_Organization" ;
#else
	szQuery = "SELECT  \
			  [index], \
			  [name], \
			  property, \
			  base_party, \
			  [exp], \
			  contribution, \
			  cur_mem, \
			  captain_name, \
			  mem_list, \
			  notice, \
			  out_time \
			  FROM ND_V01_Organization" ;
#endif


	
	HRESULT hr = OrSelect.OpenCommand( *m_session, szQuery ) ;	
		
	if( FAILED(hr) )
	{
		DBErrorOutput( hr ) ;
		if(hr==E_FAIL) CallReConTh();	
		return false ;
	}		
	
	_Organization Or ;	

	OrSelect.ResetAccessor() ;
		
	

	while ( OrSelect.GetCustomer()->MoveNext() == S_OK ) 
	{
		Or.m_iIndex = OrSelect.GetAccessor()->m_index ;		
		memcpy( Or.m_cOrName, OrSelect.GetAccessor()->m_name, en_or_name_length+1 ) ;		
		Or.m_cProperty = static_cast<char>(OrSelect.GetAccessor()->m_sProp) ;
		Or.m_cBaseParty = static_cast<char>(OrSelect.GetAccessor()->m_sBaseParty) ;
		Or.m_uiExperience = OrSelect.GetAccessor()->m_uiExp ;
		Or.m_uiContribution = OrSelect.GetAccessor()->m_uiContribution ;
		Or.m_usCurMemberNum = OrSelect.GetAccessor()->m_usCurMem ;
		memcpy( Or.m_cCaptainName, OrSelect.GetAccessor()->m_cCaptainName, 13 ) ;	
		memcpy( Or.m_MemberList, OrSelect.GetAccessor()->m_MemListData, _SIZE_OF_OR_MEM_LIST_DATA ) ;	

		memcpy( Or.m_cNotice, OrSelect.GetAccessor()->m_cNotice, sizeof(Or.m_cNotice) ) ;
		memcpy( Or.m_outTime, OrSelect.GetAccessor()->m_outTime, sizeof(Or.m_outTime) ) ;

#ifdef _PD_ALLY_ORG_STEP_1_
		Or.m_iAllyIndex = OrSelect.GetAccessor()->m_iAllyIndex;
#endif


		for( int i = 0 ; i < Or.m_usCurMemberNum ; i++ )
		{
			Or.m_MemberList[i].m_bOnline = false ;
		}

		pOrManager->InsertOR( Or ) ;
	}

	OrSelect.CloseCommand() ;

	return true ;
}

#ifdef _PD_ALLY_ORG_STEP_1_
int COLE_DB_Executer::GetAllyInfo( CAllyManager * pAllyManager, COrganizationManager * pOrManager )
{
	CString			szQuery ;
	C_AllyRead		AllyRead ;

	szQuery = "SELECT [ally_index], \
		[ally_org_1], [ally_org_2], [ally_org_3], [ally_org_4], [ally_org_5], \
		[ally_name], [ally_base_property], [ally_money], [ally_state] \
		FROM [ND_V01_ALLY_ORG]";

	HRESULT hr = AllyRead.OpenCommand( *m_session, szQuery ) ;	

	if( FAILED(hr) )
	{
		DBErrorOutput( hr ) ;
		if(hr==E_FAIL) CallReConTh();	
		return 1 ;
	}		

	AllyRead.ResetAccessor() ;

	CAllyOrganization Ally;
	__org_state_in_ally org;
	short state = 0;

	int nCount = 0;
	hr = AllyRead.GetCustomer()->MoveNext();
	while ( hr == S_OK ) 
	{
		Ally.reset_ally(NULL);
		ZeroMemory( &org, sizeof(org) );

		state = AllyRead.GetAccessor()->GetState();

		if( 0 == state )
		{
			Ally.set_index( AllyRead.GetAccessor()->GetAllyIndex() );
			Ally.set_property( AllyRead.GetAccessor()->GetProperty() );
			
			org.iOrIndex = AllyRead.GetAccessor()->GetOrg1();
			Ally.set_org_info( 0, org );

			org.iOrIndex = AllyRead.GetAccessor()->GetOrg2();
			Ally.set_org_info( 1, org );

			org.iOrIndex = AllyRead.GetAccessor()->GetOrg3();
			Ally.set_org_info( 2, org );

			org.iOrIndex = AllyRead.GetAccessor()->GetOrg4();
			Ally.set_org_info( 3, org );

			org.iOrIndex = AllyRead.GetAccessor()->GetOrg5();
			Ally.set_org_info( 4, org );


			pAllyManager->copy_and_insert_ally( Ally );

			++nCount;
		}

		hr = AllyRead.GetCustomer()->MoveNext();
	}

	printf( "[ALLY] Read %d Allies Information\n", nCount );

	AllyRead.CloseCommand() ;
	return 0 ;
}


int COLE_DB_Executer::AllyCreate( int org1, int org2, char * orgName, short pr )
{
	int index = 0;

	try{
		m_pAllyCreate->ResetAccessor();
		m_pAllyCreate->GetAccessor()->SetOrg1( org1 );
		m_pAllyCreate->GetAccessor()->SetOrg2( org2 );
		m_pAllyCreate->GetAccessor()->SetName( orgName );
		m_pAllyCreate->GetAccessor()->SetProperty( pr );

		HRESULT hr = m_pAllyCreate->OpenCommand( *m_session );
		m_pAllyCreate->CloseCommand();

		if(FAILED(hr))
		{
			DBErrorOutput( hr );
			if( hr == E_FAIL) CallReConTh();		

			index = 0;
		}		
		else
		{
			index = m_pAllyCreate->GetAccessor()->GetIndex();
		}
	}
	catch(...){
		::PrintConsole("[EXCEPTION] ==> AllyCreate\n" );
		m_pAllyCreate->CloseCommand();
		index = 0;
	}

	return index;
}

void COLE_DB_Executer::AllyUpdate( int index, int org2, int org3, int org4, int org5, short pr )
{
	try{
		m_pAllyUpdate->ResetAccessor();
		m_pAllyUpdate->GetAccessor()->SetIndex( index );
		m_pAllyUpdate->GetAccessor()->SetOrg2( org2 );
		m_pAllyUpdate->GetAccessor()->SetOrg3( org3 );
		m_pAllyUpdate->GetAccessor()->SetOrg4( org4 );
		m_pAllyUpdate->GetAccessor()->SetOrg5( org5 );
		m_pAllyUpdate->GetAccessor()->SetProperty( pr );

		HRESULT hr = m_pAllyUpdate->OpenCommand( *m_session );
		m_pAllyUpdate->CloseCommand();

		if(FAILED(hr))
		{
			DBErrorOutput( hr );
			if( hr == E_FAIL) CallReConTh();		
		}		

	}
	catch(...){
		::PrintConsole("[EXCEPTION] ==> AllyUpdate\n" );
		m_pAllyUpdate->CloseCommand();
	}
}

void COLE_DB_Executer::AllyBreak( int index, short reason )
{
	try{
		m_pAllyBreakup->ResetAccessor();
		m_pAllyBreakup->GetAccessor()->SetIndex( index );
		m_pAllyBreakup->GetAccessor()->SetState( reason );

		HRESULT hr = m_pAllyBreakup->OpenCommand( *m_session );
		m_pAllyBreakup->CloseCommand();

		if(FAILED(hr))
		{
			DBErrorOutput( hr );
			if( hr == E_FAIL) CallReConTh();		
		}		

	}
	catch(...){
		::PrintConsole("[EXCEPTION] ==> AllyBreak\n" );
		m_pAllyBreakup->CloseCommand();
	}
}


void COLE_DB_Executer::AllyUpdateOfOrg( int org, int ally )
{
	try{
		m_pAllyUpdateOfOrg->ResetAccessor();
		m_pAllyUpdateOfOrg->GetAccessor()->SetIndex( org );
		m_pAllyUpdateOfOrg->GetAccessor()->SetAllyIndex( ally );

		HRESULT hr = m_pAllyUpdateOfOrg->OpenCommand( *m_session );
		m_pAllyUpdateOfOrg->CloseCommand();

		if(FAILED(hr))
		{
			DBErrorOutput( hr );
			if( hr == E_FAIL) CallReConTh();		
		}		

	}
	catch(...){
		::PrintConsole("[EXCEPTION] ==> m_pAllyUpdateOfOrg\n" );
		m_pAllyUpdateOfOrg->CloseCommand();
	}
}

#ifdef _PD_CASTLE_STEP_1_
int COLE_DB_Executer::AllyGetMasterOrgInAllAlly( int ally_index )
{
	int first_org=0;
	HRESULT hr;
	try
	{
		m_pAllyGetFirstOrg->ResetAccessor();
		m_pAllyGetFirstOrg->GetAccessor()->SetAllyIndex( ally_index );

		hr = m_pAllyGetFirstOrg->OpenCommand( *m_session );
		m_pAllyGetFirstOrg->CloseCommand();
		if( FAILED(hr) )
		{
			DBErrorOutput( hr );
			if( hr == E_FAIL )
				CallReConTh();
		}
		else
			first_org = m_pAllyGetFirstOrg->GetAccessor()->GetFirstOrgIndex();
	}
	catch( ... )
	{
		PrintConsole( "[EXCEPTION] Get First Org Index in ally.\n" );
		first_org = 0;	// 못찾은 것으로 한다.
	}
	return first_org;
}
#endif

#endif


#ifdef _RESET_LEVEL
void COLE_DB_Executer::ResetLevelInfo( char cFirst )
{
	CDB_Command<CCharacState>		characStateSelect ;
	HRESULT hr ;
	
	CCommand<CNoAccessor, CNoRowset, CNoMultipleResults>		characStateUpdate ;
	CString szUpdateQry ;

	if( cFirst == 1 )
	{
		// gong 을 0으로 초기화 한다.---------------------------------------
		szUpdateQry = "UPDATE ND_V01_CharacState SET gong = 0" ;
		hr = characStateUpdate.Open( *m_session, szUpdateQry ) ;
		if( FAILED(hr) )
		{
			printf( "[error] UPDATE ND_V01_CharacState SET gong = 0  failed \n" ) ;
		}
		characStateUpdate.Close() ;
		//------------------------------------------------------------------
	}
	


	// All select 'charac_state' table
	CString	szQuery = "SELECT   unique_id, inner_level, jin, gong  FROM  ND_V01_CharacState WHERE gong = 0" ;
	hr = characStateSelect.OpenCommand( *m_session, szQuery ) ;	

	if( FAILED(hr) )
	{
#ifdef _DEBUG
		AtlTraceErrorRecords( hr ) ;
#endif
		DBErrorOutput( hr ) ;
	}

	u_long	ulNewJin ;
	int		iLevel ;

	int		iTotalCount = 0 ;
	int		iFailedCount = 0;
	int		iCompleteCount = 0 ;	

	// move next rowset
	while ( characStateSelect.GetCustomer()->MoveNext() == S_OK ) 
	{

		if( 0 == (iTotalCount % 10) )
			Sleep(10);

		++iTotalCount ;

		iLevel = characStateSelect.GetAccessor()->m_inner_level ;

		// level 37 이상만
		if( /*iLevel >= 37 && iLevel <= 240 && */ characStateSelect.GetAccessor()->m_gong == 0 )
		{
			ulNewJin = static_cast<u_long>(characStateSelect.GetAccessor()->m_jin * g_fParam[iLevel]) ;
		}
		else
		{
			ulNewJin = characStateSelect.GetAccessor()->m_jin ;
		}

		printf("-%u \n", characStateSelect.GetAccessor()->m_uid ) ;		

		// 
		szUpdateQry.Format( "UPDATE ND_V01_CharacState SET jin = %d, gong = %d WHERE unique_id = %u"
			, ulNewJin, 1, characStateSelect.GetAccessor()->m_uid ) ;

		hr = characStateUpdate.Open( *m_session, szUpdateQry ) ;
		if( FAILED(hr) )
		{
			++iFailedCount ;
			::PrintConsole("[ERROR] %u \n", characStateSelect.GetAccessor()->m_uid) ;
		}
		else
		{
			++iCompleteCount ;
		}
		characStateUpdate.Close() ;
	}

	characStateSelect.CloseCommand() ;

}
#endif

bool COLE_DB_Executer::CheckAndChangeName( char * curName, char * newName, int& result )
{
	//
	bool bRet = false ;
	HRESULT hr;

	m_pCheckAndChangeName->ResetAccessor() ;

	CopyMemory( m_pCheckAndChangeName->GetAccessor()->curName, curName, 13 ) ;
	CopyMemory( m_pCheckAndChangeName->GetAccessor()->newName, newName, 13 ) ;
	m_pCheckAndChangeName->GetAccessor()->result = 0 ;

    hr = m_pCheckAndChangeName->OpenCommand(*m_session);
	m_pCheckAndChangeName->CloseCommand() ;

	if(FAILED(hr))
	{
		bRet = false ;
		result = 3 ;
		DBErrorOutput( hr ) ;
		if(hr==E_FAIL) CallReConTh();			
		return false ;
	}
	else
	{
		bRet = true ;
		result = m_pCheckAndChangeName->GetAccessor()->result ;
	}

	return bRet ;
}



void COLE_DB_Executer::SelectInvenInfo( char cMode )
{

	CString			szQuery ;

	C_InvenSelect	invenSelect ;

	C_prDs_UpdateInven updateInven ;

	szQuery = "SELECT *  FROM      VIEW_inven_search" ;

	HRESULT hr = invenSelect.OpenCommand( *m_session, szQuery ) ;	

	if( FAILED(hr) )
	{
#ifdef _DEBUG
		AtlTraceErrorRecords( hr ) ;
#endif
		DBErrorOutput( hr ) ;

	}		


	printf( "==== Bad item in inven ====\n") ;
	::PrintConsole( "==== Bad item in inven ====\n") ;

	bool bIsEdited ;
	
	_sServer_InvenItem_v1 * pInven ;
	while ( invenSelect.GetCustomer()->MoveNext() == S_OK ) 
	{
		pInven = (_sServer_InvenItem_v1 *)invenSelect.GetAccessor()->m_inven ;
		
		bIsEdited = false ;
		for( int i = 0 ; i < 95 ; i++ )
		{
			if( IsBadItem( &pInven[i] ) )
			{
				printf( "- name : %s , type:%d,id:%d\n", invenSelect.GetAccessor()->m_name, pInven[i].cType, pInven[i].sID ) ;
				::PrintConsole( "- name : %s , type:%d,id:%d\n", invenSelect.GetAccessor()->m_name, pInven[i].cType, pInven[i].sID ) ;

				memset( &pInven[i], 0 , sizeof(_sServer_InvenItem_v1) ) ;

				bIsEdited = true ;
			}
		}	

		if( cMode && bIsEdited )
		{
			updateInven.ResetAccessor() ;
			updateInven.GetAccessor()->m_unique_id = invenSelect.GetAccessor()->m_index ;
			memcpy( updateInven.GetAccessor()->m_data.cInventory, invenSelect.GetAccessor()->m_inven, _INVEN_SIZE_) ;
			updateInven.GetAccessor()->m_data.sSelectWeaponID = invenSelect.GetAccessor()->m_select_weapon ;
			updateInven.GetAccessor()->m_data.iMoney = invenSelect.GetAccessor()->m_uiMoney ;			
			memcpy( updateInven.GetAccessor()->m_data.cElixirData, invenSelect.GetAccessor()->cElixirData, sizeof(invenSelect.GetAccessor()->cElixirData) ) ;
			updateInven.GetAccessor()->m_data.sVersion = invenSelect.GetAccessor()->sVersion ;			
			memcpy( updateInven.GetAccessor()->m_data.cCollectionData, invenSelect.GetAccessor()->cCollectionData, sizeof(invenSelect.GetAccessor()->cCollectionData) ) ;
			updateInven.GetAccessor()->m_data.sBloodPoint = invenSelect.GetAccessor()->sBloodPoint ;			

			hr = updateInven.OpenCommand( *m_session ) ;

			if( hr == S_OK )
			{
				printf( " = update inven %s\n", invenSelect.GetAccessor()->m_name ) ;
				::PrintConsole( " = update inven %s\n", invenSelect.GetAccessor()->m_name ) ;
			}
			else
			{
				printf( " = failed (%s) \n",  invenSelect.GetAccessor()->m_name ) ;
				::PrintConsole( " = failed (%s) \n",  invenSelect.GetAccessor()->m_name ) ;
			}

			updateInven.CloseCommand() ;
		}
	}

	printf( "=====================================\n") ;
	::PrintConsole( "=====================================\n") ;

	invenSelect.CloseCommand() ;
}


void COLE_DB_Executer::SelectDepotInfo( char cMode ) 
{
	CString			szQuery ;
	C_DepotSelect	depotSelect ;

	C_prDs_UpdateDepot	updateDepot ;

	bool bFound;

	szQuery = "SELECT   * 	FROM      ND_V01_Depot" ;

	HRESULT hr = depotSelect.OpenCommand( *m_session, szQuery ) ;	

	if( FAILED(hr) )
	{
#ifdef _DEBUG
		AtlTraceErrorRecords( hr ) ;
#endif
		DBErrorOutput( hr ) ;
	}		

	printf( "==== Bad item in depot ====\n") ;
	::PrintConsole( "==== Bad item in depot ====\n") ;

	int iCount = 0 ;
	int iZeroVersionCount = 0 ;
	
	while ( depotSelect.GetCustomer()->MoveNext() == S_OK ) 
	{
		++iCount ;

		bFound = false;

		_sServer_InvenItem_v1 * pInven = (_sServer_InvenItem_v1 *)&depotSelect.GetAccessor()->m_depot[0] ;
		for( int i = 0 ; i < 40  ; i++ )
		{
			if( IsBadItem( &pInven[i] ) )
			{
				bFound = true;
				printf( "[V2-bad] uid:%d, slot:%d id:%d\n", depotSelect.GetAccessor()->m_unique_id, i, pInven[i].sID ) ;					

				if( 1 == cMode )
				{
					ZeroMemory( &pInven[i], sizeof( _sServer_InvenItem_v1 ) );
				}
			}

			if( bFound && 1 == cMode )
			{
				// udpate depot
				updateDepot.ResetAccessor();
				updateDepot.GetAccessor()->m_unique_id = depotSelect.GetAccessor()->m_unique_id;
				CopyMemory( updateDepot.GetAccessor()->m_depot, depotSelect.GetAccessor()->m_depot, _DEPOT_BYTE_SIZE_ );
				updateDepot.GetAccessor()->m_d_money = depotSelect.GetAccessor()->m_d_money;
				updateDepot.GetAccessor()->m_check_count = depotSelect.GetAccessor()->m_check_count;
				updateDepot.GetAccessor()->m_check_year = depotSelect.GetAccessor()->m_check_year;
				updateDepot.GetAccessor()->m_check_month = depotSelect.GetAccessor()->m_check_month;
				updateDepot.GetAccessor()->m_check_day = depotSelect.GetAccessor()->m_check_day;
				updateDepot.GetAccessor()->m_check_hour = depotSelect.GetAccessor()->m_check_hour;
				updateDepot.GetAccessor()->m_version = depotSelect.GetAccessor()->m_version;

				hr = updateDepot.OpenCommand( *m_session ) ;

				if( hr == S_OK )
				{
					printf( " = update depot %d\n", updateDepot.GetAccessor()->m_unique_id ) ;
					::PrintConsole( " = update depot %d\n", updateDepot.GetAccessor()->m_unique_id ) ;
				}
				else
				{
					printf( " = failed (%d) \n", updateDepot.GetAccessor()->m_unique_id ) ;
					::PrintConsole( " = failed (%d) \n", updateDepot.GetAccessor()->m_unique_id ) ;
				}

				updateDepot.CloseCommand() ;
			}
		}
	}

	printf( "====================================%d/%d\n", iZeroVersionCount, iCount ) ;
	

	depotSelect.CloseCommand() ;
}


bool COLE_DB_Executer::GetRankVersionInfo()
{
	try
	{
		CString				szQuery ;
		C_prReflushedCheck	rankTableVersion ;


		szQuery = "SELECT   rank_type, rank_version \
			FROM      ND_V01_RANKING_VERSION \
			ORDER BY rank_type" ;



		HRESULT hr = rankTableVersion.OpenCommand( *m_session, szQuery ) ;	

		if( FAILED(hr) )
		{
#ifdef _DEBUG
			AtlTraceErrorRecords( hr ) ;
#endif
			DBErrorOutput( hr ) ;

			return false ;
		}

		int iRankType ;
		int iRankVersion ;
		while ( rankTableVersion.GetCustomer()->MoveNext() == S_OK ) 
		{
			iRankType = rankTableVersion.GetAccessor()->m_rank_type ;
			iRankVersion = rankTableVersion.GetAccessor()->m_rank_version ;
			if( iRankType < 0 )
			{
				continue ;
			}
			// 버젼이 다르다면 해당 랭킹 정보를 다시 읽어온다.
			if( g_arRankTableVersion[iRankType] != iRankVersion )
			{
				switch( iRankType )
				{
				case _ranking::_RANK_TYPE_LEVEL :
					if( GetRank_Level())
					{
						CRankInfo * pRankInfo = g_pRankingManager->GetRankInfoPtr( _ranking::_RANK_TYPE_LEVEL )	;
						if( pRankInfo ) 
						{
							CMSG_RANKING_INFO_RES msgRankingInfoRes = {0} ;

							msgRankingInfoRes.cMessage = CMSG_NO_RANKING_INFO_RES ;
							msgRankingInfoRes.sType = _ranking::_RANK_TYPE_LEVEL ;
							msgRankingInfoRes.sRankCount = pRankInfo->GetMaxCount() ;
							pRankInfo->GetRankInfo( msgRankingInfoRes.rank, msgRankingInfoRes.sRankCount ) ;

							msgRankingInfoRes.usLength = 3/*header*/ + 2 + (sizeof(_Rank) * msgRankingInfoRes.sRankCount) ;

							if( msgRankingInfoRes.usLength <= sizeof(CMSG_RANKING_INFO_RES) )
							{
								g_pDSComQueue->GlobalSend( msgRankingInfoRes.usLength, (char*)&msgRankingInfoRes ) ;
							}							

							g_arRankTableVersion[iRankType] = iRankVersion ;

							SYSTEMTIME time ;
							GetLocalTime( &time ) ;
							

//							printf( "[RANK_INFO] reflushed rank info (%u/%u/%u  %u:%u) \n",  time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute) ;
//							::PrintConsole( "[RANK_INFO] reflushed rank info (%u/%u/%u  %u:%u) \n",  time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute) ;
							
						}

					}
					else
					{
						::PrintConsole("[ERROR] if( GetRank_Level() == false ) \n" ) ;						
					}
					break ;
				}
			}		
		}
		//
		rankTableVersion.CloseCommand() ;
	}
	catch(...)
	{
		::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		return false ;
	}
	

	return true ;
}


void COLE_DB_Executer::CharacLogout( int uid )
{
	try{

		HRESULT hr;

		m_pCharacLogout->ResetAccessor() ;

		m_pCharacLogout->GetAccessor()->m_uid = uid ;
		hr = m_pCharacLogout->OpenCommand(*m_session);
		m_pCharacLogout->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
		}
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ )  ;
		m_pCharacLogout->CloseCommand() ;		
	}

}


void COLE_DB_Executer::GetServerStateFlag( void * pFlag ) 
{

	try{
		C_prGetForestStateFlag getForestStateFlag ;
		HRESULT hr ;	

		getForestStateFlag.ResetAccessor() ;
		hr = getForestStateFlag.OpenCommand( *m_session ) ;
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();
		}

		getForestStateFlag.CloseCommand() ;

		memcpy( pFlag, getForestStateFlag.GetAccessor()->m_flag, sizeof(getForestStateFlag.GetAccessor()->m_flag) ) ;
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ )  ;
	}	
}


void COLE_DB_Executer::SetServerStateFlag( void * pFlag )
{

	try{
		C_prSetForestStateFlag setForestStateFlag ;		
		HRESULT hr ;	

		setForestStateFlag.ResetAccessor() ;
		memcpy( setForestStateFlag.GetAccessor()->m_flag, pFlag, sizeof(setForestStateFlag.GetAccessor()->m_flag) ) ;
		hr = setForestStateFlag.OpenCommand( *m_session ) ;
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();
		}

		setForestStateFlag.CloseCommand() ;
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ )  ;
	}

}


bool COLE_DB_Executer::GetCountLimit( IN const int id, IN const short type, OUT int & count, OUT short & flag  )
{
	try
	{
		C_prDSGetCountLimit getCountLimit ;
		getCountLimit.ResetAccessor() ;
		getCountLimit.GetAccessor()->m_id = id ;
		getCountLimit.GetAccessor()->m_type = type ;

		HRESULT hr = getCountLimit.OpenCommand( *m_session ) ;
		if( FAILED(hr) )
		{
			DBErrorOutput( hr ) ;
			if( hr == E_FAIL ) CallReConTh() ;

			getCountLimit.CloseCommand() ;

			return false ;
		}

		getCountLimit.CloseCommand() ;

		count = getCountLimit.GetAccessor()->m_count ;
		flag = getCountLimit.GetAccessor()->m_flag ;
	}
	catch(...)
	{
		::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
	}

	return true ;
}

void COLE_DB_Executer::SetCountLimit( IN const int id, IN const short type, IN const int count, IN const int flag ) 
{
	try
	{
		C_prDSSetCountLimit setCountLimit ;
		setCountLimit.ResetAccessor() ;
		setCountLimit.GetAccessor()->m_id = id ;
		setCountLimit.GetAccessor()->m_type = type ;
		setCountLimit.GetAccessor()->m_count = count ;
		setCountLimit.GetAccessor()->m_flag = flag ;

		HRESULT hr = setCountLimit.OpenCommand( *m_session ) ;
		if( FAILED(hr) )
		{
			DBErrorOutput( hr ) ;
			if( hr == E_FAIL ) CallReConTh() ;
		}
		setCountLimit.CloseCommand() ;
	}
	catch(...)
	{
		::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
	}

}

bool COLE_DB_Executer::QuestRank( IN const u_short questID
								 , IN OUT _QUEST_RANK_DATA * questData								 
								 , IN const short flag/*0:get, 1:set*/ )
{
	try
	{
		CDB_Command<C_prQuestRankAccessor>		questRankData ;
		questRankData.ResetAccessor() ;
		questRankData.GetAccessor()->m_questID = questID ;
		questRankData.GetAccessor()->m_flag = flag ;

		if( flag == 0 )
		{
			memset( questData, 0, sizeof(_QUEST_RANK_DATA) ) ;
		}
		else if( flag == 1 )
		{
			memcpy( questRankData.GetAccessor()->m_questRankData, questData, sizeof(_QUEST_RANK_DATA) ) ;
		}		

		HRESULT hr = questRankData.OpenCommand( *m_session ) ;
		
		if( FAILED(hr) )
		{
			DBErrorOutput( hr ) ;
			if( hr == E_FAIL ) CallReConTh() ;
			return false ;
		}

		if( flag == 0 )
		{
			memcpy( questData, questRankData.GetAccessor()->m_questRankData, sizeof(_QUEST_RANK_DATA) ) ;
		}

		questRankData.CloseCommand() ;
	}
	catch(...)
	{
		__EXCEPT_STR_PRINT() ;
	}

	return true ;
	
}

#ifdef _PD_CASTLE_STEP_1_

int COLE_DB_Executer::ShowDefendCastleInfo()
{
	CString strQry;
	C_prDSGetDefendCastle DefendCastle;

	strQry.Format( "SELECT dfd_num, unique_id, dfd_name, dfd_danname, dfd_startday, dfd_scs_cnt, dfd_cnt, dfd_level, \
				   manor_PartMoney, manor_TotalMoney, FstCbt_Date, ScdCbt_Date, ThdCbt_Date, manorcode, dancode \
				   FROM ND_V01_Defend_Castle" );

	HRESULT hr = DefendCastle.OpenCommand(*m_session, strQry);
	if(FAILED(hr))
	{
		DBErrorOutput(hr);
		if(hr==E_FAIL) CallReConTh();
		return 1;
	}

	DefendCastle.ResetAccessor();

	//변수 
	int	iDefendCastleUniqueId=0;
	int	iOwenerUniqueId=0;
	char	szOwenerName[en_charac_name_length+1]="";
	char	szAllyOrgName[32+1]="";
	int		iOwnerShipFirstDate=0;
	int		iDefendSuccessNum=0;
	int		iManorLevel=0;
	int		iManorPartMoney=0;
	int		iManorTotalMoney=0;
	int		iFirstCombatDate=0;
	int		iSecondCombatDate=0;
	int		iThirdCombatDate=0;
	int		iManorUniqueId=0;
	int		iAllyOrgUniqueId=0;

	// 하나밖에 없을 테니, while 대신 if 로 한다.
	while(DefendCastle.GetCustomer()->MoveNext() == S_OK)
	{
		//장원전 수성 정보를 얻는다.
		iDefendCastleUniqueId = DefendCastle.GetAccessor()->GetDefendCastleUniqueId();
		iOwenerUniqueId = DefendCastle.GetAccessor()->GetOwenerUniqueId();
		strncpy(szOwenerName, DefendCastle.GetAccessor()->GetOwenerName(), en_charac_name_length+1);
		strncpy(szAllyOrgName, DefendCastle.GetAccessor()->GetAllyOrgName(), 32+1);
		iOwnerShipFirstDate = DefendCastle.GetAccessor()->GetOwnerShipFirstDate();
		iDefendSuccessNum = DefendCastle.GetAccessor()->GetDefendSuccessNum();
		iManorLevel = DefendCastle.GetAccessor()->GetManorLevel();
		iManorPartMoney = DefendCastle.GetAccessor()->GetManorPartMoney();
		iManorTotalMoney = DefendCastle.GetAccessor()->GetManorTotalMoney();
		iFirstCombatDate = DefendCastle.GetAccessor()->GetFirstCombatDate();
		iSecondCombatDate = DefendCastle.GetAccessor()->GetSecondCombatDate();
		iThirdCombatDate = DefendCastle.GetAccessor()->GetThirdCombatDate();
		iManorUniqueId = atoi(DefendCastle.GetAccessor()->GetManorUniqueId());
		iAllyOrgUniqueId = atoi(DefendCastle.GetAccessor()->GetDanType());

		printf( "[DefendCastle] Castle : < %d >       \
			iOwenerUniqueId   =%4d, szOwenerName=%s, szAllyOrgName=%s \n\
			, iDefendSuccessNum=%4d, iManorLevel =%d, iManorPartMoney=%d, iManorTotalMoney=%d\n\
			, iManorUniqueId=%d, iAllyOrgUniqueId=%d, m_OwnerShipFirstDate=%d,	\
			, m_FirstCombatDate=%d, m_SecondCombatDate= %d, m_ThirdCombatDate=%d\n"
			,	iDefendCastleUniqueId, iOwenerUniqueId, szOwenerName, szAllyOrgName  \
			,	iDefendSuccessNum, iManorLevel, iManorPartMoney, iManorTotalMoney  \
			,	iManorUniqueId, iAllyOrgUniqueId  \
			,	iOwnerShipFirstDate, iFirstCombatDate, iSecondCombatDate, iThirdCombatDate);
	}

	DefendCastle.CloseCommand();
	return 0;
}


int COLE_DB_Executer::GetDefendCastleInfo( int dfd_num, CDSCastleInfo * pCastle )
{
	// 특정 장원의 자세한 정보( 도전 리스트만 빼고 )를 채운다.
	CString strQry;
	C_prDSGetDefendCastle DefendCastle;

	strQry.Format( "SELECT dfd_num, unique_id, dfd_name, dfd_danname, dfd_startday, dfd_scs_cnt, dfd_cnt, dfd_level, \
				   manor_PartMoney, manor_TotalMoney, FstCbt_Date, ScdCbt_Date, ThdCbt_Date, manorcode, dancode \
				   FROM ND_V01_Defend_Castle \
			       WHERE dfd_num = %d", dfd_num );

	HRESULT hr = DefendCastle.OpenCommand(*m_session, strQry);
	if(FAILED(hr))
	{
		DBErrorOutput(hr);
		if(hr==E_FAIL) CallReConTh();
		return 1;
	}

	DefendCastle.ResetAccessor();

	//변수 
	/*
	int	iDefendCastleUniqueId=0;
	int	iOwenerUniqueId=0;
	char	szOwenerName[en_charac_name_length+1]="";
	char	szAllyOrgName[32+1]="";
	int		iOwnerShipFirstDate=0;
	int		iDefendSuccessNum=0;
	int		iManorLevel=0;
	int		iManorPartMoney=0;
	int		iManorTotalMoney=0;
	int		iFirstCombatDate=0;
	int		iSecondCombatDate=0;
	int		iThirdCombatDate=0;
	int		iManorUniqueId=0;
	int		iAllyOrgUniqueId=0;
	*/

	// 하나밖에 없을 테니, while 대신 if 로 한다.
	if(DefendCastle.GetCustomer()->MoveNext() == S_OK)
	{
		/*
		//장원전 수성 정보를 얻는다.
		iDefendCastleUniqueId = DefendCastle.GetAccessor()->GetDefendCastleUniqueId();
		iOwenerUniqueId = DefendCastle.GetAccessor()->GetOwenerUniqueId();
		strncpy(szOwenerName, DefendCastle.GetAccessor()->GetOwenerName(), en_charac_name_length+1);
		strncpy(szAllyOrgName, DefendCastle.GetAccessor()->GetAllyOrgName(), 32+1);
		iOwnerShipFirstDate = DefendCastle.GetAccessor()->GetOwnerShipFirstDate();
		iDefendSuccessNum = DefendCastle.GetAccessor()->GetDefendSuccessNum();
		iManorLevel = DefendCastle.GetAccessor()->GetManorLevel();
		iManorPartMoney = DefendCastle.GetAccessor()->GetManorPartMoney();
		iManorTotalMoney = DefendCastle.GetAccessor()->GetManorTotalMoney();
		iFirstCombatDate = DefendCastle.GetAccessor()->GetFirstCombatDate();
		iSecondCombatDate = DefendCastle.GetAccessor()->GetSecondCombatDate();
		iThirdCombatDate = DefendCastle.GetAccessor()->GetThirdCombatDate();
		iManorUniqueId = atoi(DefendCastle.GetAccessor()->GetManorUniqueId());
		iAllyOrgUniqueId = atoi(DefendCastle.GetAccessor()->GetDanType());
		*/
		//DefendCastle.GetAccessor()->GetAllyOrgName(),
		if( pCastle )
		{
			pCastle->set_or_index( DefendCastle.GetAccessor()->GetOwenerUniqueId(), 
				(u_char)atoi(DefendCastle.GetAccessor()->GetDanType()) ) ;
			pCastle->set_castle_level( DefendCastle.GetAccessor()->GetManorLevel() );
			pCastle->set_or_name( DefendCastle.GetAccessor()->GetAllyOrgName() );
			//pCastle->set_or_member_count();
			pCastle->set_or_master_name( DefendCastle.GetAccessor()->GetOwenerName() );
			//pCastle->set_or_master_moonpa();
			pCastle->set_total_money( DefendCastle.GetAccessor()->GetManorTotalMoney() );
			pCastle->set_part_money( DefendCastle.GetAccessor()->GetManorPartMoney() );
			pCastle->set_cur_match_count( DefendCastle.GetAccessor()->GetMatchCount() );
			pCastle->set_win_count( DefendCastle.GetAccessor()->GetDefendSuccessNum() );

			pCastle->set_date_of_own_from_time( DefendCastle.GetAccessor()->GetOwnerShipFirstDate() );
#ifdef _PD_CASTLE_RENEWAL_1_
			pCastle->set_combat_date_from_time( DefendCastle.GetAccessor()->GetFirstCombatDate() );
#else
			pCastle->set_combat_date_from_time( DefendCastle.GetAccessor()->GetFirstCombatDate(), 0 );
			pCastle->set_combat_date_from_time( DefendCastle.GetAccessor()->GetSecondCombatDate(), 1 );
			pCastle->set_combat_date_from_time( DefendCastle.GetAccessor()->GetThirdCombatDate(), 2 );
#endif
		}
		else
		{
			/*
			printf( "[DefendCastle] Castle : < %d >       \
				iOwenerUniqueId   =%4d, szOwenerName=%s, szAllyOrgName=%s \n\
				, iDefendSuccessNum=%4d, iManorLevel =%d, iManorPartMoney=%d, iManorTotalMoney=%d\n\
				, iManorUniqueId=%d, iAllyOrgUniqueId=%d, m_OwnerShipFirstDate=%d,	\
				, m_FirstCombatDate=%d, m_SecondCombatDate= %d, m_ThirdCombatDate=%d\n"
				,	iDefendCastleUniqueId, iOwenerUniqueId, szOwenerName, szAllyOrgName  \
				,	iDefendSuccessNum, iManorLevel, iManorPartMoney, iManorTotalMoney  \
				,	iManorUniqueId, iAllyOrgUniqueId  \
				,	iOwnerShipFirstDate, iFirstCombatDate, iSecondCombatDate, iThirdCombatDate);
				*/
		}
	}

	DefendCastle.CloseCommand();

	return 0;
}



int COLE_DB_Executer::ShowAttackCastleInfo()
{
	CString strQry;
	C_prDSGetAttackCastle AttackCastle;

	// Order by date. 추가할 것.
	strQry.Format( "SELECT atk_num, unique_id, atk_name, atk_danname, atk_confirm, manorcode, dancode \
				   FROM ND_V01_Attack_Castle WHERE atk_confirm='1'" );

	HRESULT hr = AttackCastle.OpenCommand(*m_session, strQry);
	if(FAILED(hr))
	{
		DBErrorOutput(hr);
		if(hr==E_FAIL) CallReConTh();
		return 1;
	}

	AttackCastle.ResetAccessor();

	//변수
	int	iAttackCastleUniqueId =0;		//공성번호
	int	iChallengerUniqueId =0;			//도전자  uid(캐릭)
	char szChallengerName[en_charac_name_length+1]="";			//공성자 캐릭 명
	char szAllyOrgName[32+1]="";				//공성 신청단 명
	short sApplyTrueOrFalse=0;			//공성 신청 여부 1)true(default) 0) false;
	int	iManorUniqueId=0;				//장원 코드
	int	iAllyOrgUniqueId=0;				//단회 코드	//소유자 타입0)monster, 1)영웅단, 2) 동맹

	while(AttackCastle.GetCustomer()->MoveNext() == S_OK)
	{
		
		iAttackCastleUniqueId = AttackCastle.GetAccessor()->GetAttackCastleUniqueId();		//공성번호
		iChallengerUniqueId = AttackCastle.GetAccessor()->GetChallengerUniqueId();			//도전자  uid(캐릭)
		strncpy(szChallengerName, AttackCastle.GetAccessor()->GetChallengerName(), en_charac_name_length+1);			//공성자 캐릭 명
		strncpy(szAllyOrgName, AttackCastle.GetAccessor()->GetAllyOrgName(), 32+1);				//공성 신청단 명
		sApplyTrueOrFalse = atoi( AttackCastle.GetAccessor()->GetAtkConfirm());			//공성 신청 여부 1)true(default) 0) false;
		iManorUniqueId = atoi(AttackCastle.GetAccessor()->GetManorUniqueId());				//장원 코드
		iAllyOrgUniqueId = atoi(AttackCastle.GetAccessor()->GetDanType());				//단회 코드	//소유자 타입0)monster, 1)영웅단, 2) 동맹

		printf( "[Attack Castle Info]  Castle < %d > \
				iChallengerUniqueId=%d, szChallengerName=%s, szAllyOrgName=%s \
				,sApplyTrueOrFalse=%d, iManorUniqueId=%d, iAllyOrgUniqueId=%d\n"
				,iAttackCastleUniqueId, iChallengerUniqueId, szChallengerName, szAllyOrgName,sApplyTrueOrFalse \
				,iManorUniqueId, iAllyOrgUniqueId);
	}

	AttackCastle.CloseCommand();

	return 0;
}


int COLE_DB_Executer::GetAttackCastleInfo( int map_id, CDSCastleInfo * pCastle )
{
	// 특정 장원의 도전 목록을 읽는다.
	CString strQry;
	C_prDSGetAttackCastle AttackCastle;

	// Order by date. 추가할 것.
	strQry.Format( "SELECT atk_num, unique_id, atk_name, atk_danname, atk_confirm, manorcode, dancode \
				   FROM ND_V01_Attack_Castle WHERE manorcode='%02d' AND atk_confirm='1'", map_id );

	HRESULT hr = AttackCastle.OpenCommand(*m_session, strQry);
	if(FAILED(hr))
	{
		DBErrorOutput(hr);
		if(hr==E_FAIL) CallReConTh();
		return 1;
	}

	AttackCastle.ResetAccessor();

	__cb_challenge_info	unit;		 

	//변수
	int	iAttackCastleUniqueId =0;		//공성번호
	int	iChallengerUniqueId =0;			//도전자  uid(캐릭)
	char szChallengerName[en_charac_name_length+1]="";			//공성자 캐릭 명
	char szAllyOrgName[32+1]="";				//공성 신청단 명
	short sApplyTrueOrFalse=0;			//공성 신청 여부 1)true(default) 0) false;
	int	iManorUniqueId=0;				//장원 코드
	int	iAllyOrgUniqueId=0;				//단회 코드	//소유자 타입0)monster, 1)영웅단, 2) 동맹
	
	while(AttackCastle.GetCustomer()->MoveNext() == S_OK)
	{
		unit.reset();
		unit.iDBSeqNumber = AttackCastle.GetAccessor()->GetAttackCastleUniqueId();
		unit.iIndex = AttackCastle.GetAccessor()->GetChallengerUniqueId();
		unit.ucMoonpa = 0;		// 영웅단과 검사시에 채워 넣는다.
		unit.ucRes1 = 0;
		unit.ucType = atoi(AttackCastle.GetAccessor()->GetDanType());
		unit.ucState = 0;		// 게임서버에서 알아서 한다. DB 에 저장되지 않는다.
		unit.usMemberCount = 0;	// 영웅단과 검사시에 채워 넣는다.
		strncpy( unit.cGroupName, AttackCastle.GetAccessor()->GetAllyOrgName(), 32 );
		unit.cGroupName[32] = 0;
		strncpy( unit.cMasterName, AttackCastle.GetAccessor()->GetChallengerName(), en_charac_name_length );
		unit.cMasterName[en_charac_name_length] = 0;

		if( pCastle )
		{
			// 공성 리스트( 도전자 리스트)를 넣는다.
			pCastle->add_challenge_list( &unit );
		}
		else
		{
			/*	정애씨 원본 코드
			iAttackCastleUniqueId = AttackCastle.GetAccessor()->GetAttackCastleUniqueId();		//공성번호
			iChallengerUniqueId = AttackCastle.GetAccessor()->GetChallengerUniqueId();			//도전자  uid(캐릭)
			strncpy(szChallengerName, AttackCastle.GetAccessor()->GetChallengerName(), en_charac_name_length+1);			//공성자 캐릭 명
			strncpy(szAllyOrgName, AttackCastle.GetAccessor()->GetAllyOrgName(), 32+1);				//공성 신청단 명
			sApplyTrueOrFalse = AttackCastle.GetAccessor()->GetApplyTrueOrFalse();			//공성 신청 여부 1)true(default) 0) false;
			iManorUniqueId = atoi(AttackCastle.GetAccessor()->GetManorUniqueId());				//장원 코드
			iAllyOrgUniqueId = atoi(AttackCastle.GetAccessor()->GetDanType());				//단회 코드	//소유자 타입0)monster, 1)영웅단, 2) 동맹
			*/
			
			printf( "[Attack Castle Info]  Castle < %d > \
					iChallengerUniqueId=%d, szChallengerName=%s, szAllyOrgName=%s \
					,sApplyTrueOrFalse=%d, iManorUniqueId=%d, iAllyOrgUniqueId=%d\n"
					,unit.iIndex, 0, unit.cMasterName, unit.cGroupName, sApplyTrueOrFalse \
					,iManorUniqueId, iAllyOrgUniqueId);
		}
	}

	AttackCastle.CloseCommand();

	return 0;
}

int COLE_DB_Executer::GetManorCombatInfo( CDSCastleManager * pCastleManager )
{
	CString strQry;
	C_prDSGetManorCombat ManorCombat;

	strQry = "SELECT  \
			 dfd_num,  \
			 atk_num,  \
			 bt_grade,  \
			 bt_dfd_result,  \
			 bt_atk_result  \
			 FROM ND_V01_Battle  \
			 WHERE dfd_num = 3 and atk_num = 4";

	HRESULT hr = ManorCombat.OpenCommand(*m_session, strQry);
	
	if(FAILED(hr))
	{ 
		DBErrorOutput(hr);
		if(hr==E_FAIL) CallReConTh();
		return 1;
	}
	
	ManorCombat.ResetAccessor();
	
	//hr = ManorCombat.GetCustomer()->MoveNext();
	
	//변수
	int	iDefendCastleUniqueId =0;		//수성 번호
	int	iAttackCastleUniqueId =0;		//공성 번호
	int	iChallengerNumber=0;				//도전 차수
	int iDefendCastleResult=0;				//승패 여부 ( 0)None, 1)도전자 승 2)도전자 패
	int iAttackCastleResult=0;				//승패 여부 ( 0)None, 1)도전자 승 2)도전자 패

	while(ManorCombat.GetCustomer()->MoveNext() == S_OK)
	{
		//장원전 수성 정보를 얻는다.
		iDefendCastleUniqueId = ManorCombat.GetAccessor()->GetDefendCastleUniqueId();		//수성 번호
		iAttackCastleUniqueId = ManorCombat.GetAccessor()->GetAttackCastleUniqueId();		//공성 번호
		iChallengerNumber = ManorCombat.GetAccessor()->GetChallengerNumber();				//도전 차수
		iDefendCastleResult = atoi(ManorCombat.GetAccessor()->GetDefendCastleResult());				//승패 여부 ( 0)None, 1)도전자 승 2)도전자 패
		iAttackCastleResult = atoi(ManorCombat.GetAccessor()->GetAttackCastleResult());				//승패 여부 ( 0)None, 1)도전자 승 2)도전자 패

		if( pCastleManager )
		{
			// 장원 소유 정보를 장원의 아이디에 따라, 입력한다.
		}
		else
		{
			printf( "[Manor Combat Castle] < %d >,\
					iAttackCastleUniqueId=%d, iChallengerNumber=%d, iDefendCastleResult=%d, iAttackCastleResult=%d \n"
					,iDefendCastleUniqueId, iAttackCastleUniqueId, iChallengerNumber, iDefendCastleResult, iAttackCastleResult);

		}
	}

	
	ManorCombat.CloseCommand();

	return 0;
}


int COLE_DB_Executer::GetDefendDespotInfo( CDSCastleManager * pCastleManager )
{
	// 현재 어떤 장원이 있는지 간략 정보만 검색한다.
	int nCount = 0;
	CString strQry;
	C_prDSGetDefendDespot DefendDespot;

	strQry= " SELECT * FROM ND_V01_Defend_Despot";

	HRESULT hr = DefendDespot.OpenCommand(*m_session, strQry);

	if(FAILED(hr))
	{ 
		DBErrorOutput(hr);
		if(hr==E_FAIL) CallReConTh();
		return 1;
	}

	DefendDespot.ResetAccessor();

	int iManorUniqueId = 0;
	int iDefendCastleUniqueId = 0;

	while( DefendDespot.GetCustomer()->MoveNext() == S_OK)
	{
		iManorUniqueId = atoi(DefendDespot.GetAccessor()->GetManorUniqueId());
		iDefendCastleUniqueId = DefendDespot.GetAccessor()->GetDefendCastleUniqueId();

		if( pCastleManager )
		{
			// 인덱스를 포함한 간략 정보만 입렧한다.
			pCastleManager->add_castle_index_only( iManorUniqueId, iDefendCastleUniqueId );
		}
		else
		{
			printf( "[Manor Defend Despot] Read Manor id = %d, row index = %d, \n", 
				iManorUniqueId ,iDefendCastleUniqueId );
		}
		++nCount;
	}

	DefendDespot.CloseCommand();
	return nCount;
}

int COLE_DB_Executer::InsertDefendCastleInfo( int map_id )
{

	return 0;
}


int COLE_DB_Executer::InsertAttackCastle( int map_id, __cb_challenge_info * pInfo )
{
	int db_seq = 0;
	HRESULT hr;
	/*
		Apply - 0 : 취소, 1 : 신청, 2 : 도전완료되었음.
	*/

	try{
		m_pUpdateAttackCastle->ResetAccessor();

		//변수 셋팅.
		m_pUpdateAttackCastle->GetAccessor()->SetChallengerUniqueId( pInfo->iIndex );
		m_pUpdateAttackCastle->GetAccessor()->SetDBSequence(0);		// 이건 그냥 초기화.
		m_pUpdateAttackCastle->GetAccessor()->SetFlag_Insert();		// Insert 로 플래그 설정.
		m_pUpdateAttackCastle->GetAccessor()->SetChallengerName( pInfo->cMasterName );
		m_pUpdateAttackCastle->GetAccessor()->SetAllyOrgName( pInfo->cGroupName );
		m_pUpdateAttackCastle->GetAccessor()->SetApplyTrueOrFalse( 1 );
		m_pUpdateAttackCastle->GetAccessor()->SetManorUniqueId( map_id );
		m_pUpdateAttackCastle->GetAccessor()->SetAllyOrgUniqueId( pInfo->ucType );

		hr = m_pUpdateAttackCastle->OpenCommand(*m_session);
		if(FAILED(hr))
		{
			DBErrorOutput( hr );
			if( hr == E_FAIL)
				CallReConTh();
			db_seq = 0;
		}
		else
		{
			db_seq = m_pUpdateAttackCastle->GetAccessor()->GetDBSequence();
		}

		m_pUpdateAttackCastle->CloseCommand();
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] ==> Insert AttackCastle\n" );
		m_pUpdateAttackCastle->CloseCommand();
		db_seq = 0;
	}

	return db_seq;
}


void COLE_DB_Executer::CompleteChallenge( int db_seq )
{
	HRESULT hr;
	/*
		Apply - 0 : 취소, 1 : 신청, 2 : 도전완료되었음.
	*/

	try{
		m_pUpdateAttackCastle->ResetAccessor();

		//변수 셋팅.
		m_pUpdateAttackCastle->GetAccessor()->SetDBSequence(db_seq);		
		m_pUpdateAttackCastle->GetAccessor()->SetFlag_Update();		// Insert 로 플래그 설정.
		m_pUpdateAttackCastle->GetAccessor()->SetApplyTrueOrFalse( 2 );
		
		hr = m_pUpdateAttackCastle->OpenCommand(*m_session);
		if(FAILED(hr))
		{
			DBErrorOutput( hr );
			if( hr == E_FAIL)
				CallReConTh();
		}

		m_pUpdateAttackCastle->CloseCommand();
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] ==> Complete Challenge\n" );
		m_pUpdateAttackCastle->CloseCommand();
	}
}


void COLE_DB_Executer::CancelChallenge( int db_seq )
{
	HRESULT hr;
	/*
		Apply - 0 : 취소, 1 : 신청, 2 : 도전완료되었음.
	*/

	try{
		m_pUpdateAttackCastle->ResetAccessor();

		//변수 셋팅.
		m_pUpdateAttackCastle->GetAccessor()->SetDBSequence(db_seq);		
		m_pUpdateAttackCastle->GetAccessor()->SetFlag_Update();		// Insert 로 플래그 설정.
		m_pUpdateAttackCastle->GetAccessor()->SetApplyTrueOrFalse( 0 );

		hr = m_pUpdateAttackCastle->OpenCommand(*m_session);
		if(FAILED(hr))
		{
			DBErrorOutput( hr );
			if( hr == E_FAIL)
				CallReConTh();
		}

		m_pUpdateAttackCastle->CloseCommand();
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] ==> Cancel Challenge\n" );
		m_pUpdateAttackCastle->CloseCommand();
	}
}


bool COLE_DB_Executer::FillCastleDetailInfo( short castle_id, int db_seq_index, CDSCastleInfo * info )
{
	// 1. db_seq_index 로 ND_V01_Defend_Castle 테이블에서 장원의 소유 정보를 채운다.
	// ( GetDefendCastleInfo ) : 장원의 소유 정보 및 장원전 스케줄 정보
	// 2. castle_id 로 ND_V01_Attack_Castle 테이블에서 스케줄 정보를 채운다.
	// ( GetAttackCastleInfo ) : 장원의 도전자 정보를 얻는다.
	// 제대로 채워지면, 정보가 채워졌음을 알린다.

	// 1.
	GetDefendCastleInfo( db_seq_index, info );

	// 2.
	GetAttackCastleInfo( castle_id, info );

	info->set_init_flag(true);

	return true;
}

int COLE_DB_Executer::CastleInsert( short castle_id )
{
	// _prDS_CastleInsert 함수 호출.
	m_pCastleInsert->ResetAccessor();

	m_pCastleInsert->GetAccessor()->SetUniqueID( castle_id );

	HRESULT hr = m_pCastleInsert->OpenCommand( *m_session );

	if( FAILED(hr) )
	{
		DBErrorOutput( hr ) ;
	}

	m_pCastleInsert->CloseCommand() ;

	return m_pCastleInsert->GetAccessor()->GetResult();
}


int COLE_DB_Executer::UpdateDefendCastleInfo( CDSCastleInfo * pInfo, int insert_or_update	/* 0 - insert, 1 - update */  )
{
	int dfd = 0;

	try{
		m_pSetDefendCastle->ResetAccessor();

		//수성 정보 세팅한다.
		m_pSetDefendCastle->GetAccessor()->SetOwnerUniqueId( pInfo->get_owner_index() );
		m_pSetDefendCastle->GetAccessor()->SetOwnerName( pInfo->get_master_name() );
		m_pSetDefendCastle->GetAccessor()->SetAllyOrgName( pInfo->get_owner_name() );

		m_pSetDefendCastle->GetAccessor()->SetOwnerShipFirstDate( pInfo->get_own_date() );

		m_pSetDefendCastle->GetAccessor()->SetDefendSuccessNum( (u_char)pInfo->get_win_count() );
		m_pSetDefendCastle->GetAccessor()->SetDefendMatchNum( pInfo->get_cur_match_count() );
		m_pSetDefendCastle->GetAccessor()->SetManorLevel( pInfo->get_level() );
		
		m_pSetDefendCastle->GetAccessor()->SetManorPartMoney( pInfo->get_part_money() );
		m_pSetDefendCastle->GetAccessor()->SetManorTotalMoney( pInfo->get_total_money() );

#ifdef _PD_CASTLE_RENEWAL_1_
		m_pSetDefendCastle->GetAccessor()->SetFirstCombatDate( pInfo->get_schedule() );
		m_pSetDefendCastle->GetAccessor()->SetSecondCombatDate( pInfo->get_schedule() );
		m_pSetDefendCastle->GetAccessor()->SetThirdCombatDate( pInfo->get_schedule() );
#else
		m_pSetDefendCastle->GetAccessor()->SetFirstCombatDate( pInfo->get_schedule(0) );
		m_pSetDefendCastle->GetAccessor()->SetSecondCombatDate( pInfo->get_schedule(1) );
		m_pSetDefendCastle->GetAccessor()->SetThirdCombatDate( pInfo->get_schedule(2) );
#endif

		m_pSetDefendCastle->GetAccessor()->SetManorCode( pInfo->get_map_index() );
		m_pSetDefendCastle->GetAccessor()->SetTypeCode( pInfo->get_owner_type() );

		m_pSetDefendCastle->GetAccessor()->SetFlag( insert_or_update );

		m_pSetDefendCastle->GetAccessor()->SetDfdNum( pInfo->get_db_seq_index() );

		HRESULT hr = m_pSetDefendCastle->OpenCommand(*m_session);
		if(FAILED(hr))
		{
			DBErrorOutput( hr );
		}

		m_pSetDefendCastle->CloseCommand();

		if( insert_or_update == 0 )	// insert
			dfd = m_pSetDefendCastle->GetAccessor()->GetDfdNum();
	}
	catch(...){
		
		::PrintConsole("[EXCEPTION] ==> Update DefendCastle\n" );
		
		dfd = 0;
	}

	return dfd;
}


int COLE_DB_Executer::SetManorCombatInfo()
{
	try{
		m_pSetManorCombat->ResetAccessor();
		
		int iOwenerUniqueId = 20;
		int iChallengerUniqueId = 2;
		int iCallengerNumber = 1;
		int iDefendCastleResult = 1;
		int iAttackCastleResult = 0;
		int iManorUniqueId = 1;
		short sFlag = 0;//insert

		char szTempDefend[3], szTempAttack[3], szTempManor[3];
		sprintf(szTempDefend, "%d", iDefendCastleResult);
		sprintf(szTempAttack, "%d", iAttackCastleResult);
		sprintf(szTempManor, "%d", iManorUniqueId);

		//수성_공성 정보 세팅한다.
		m_pSetManorCombat->GetAccessor()->SetOwenerUniqueId(iOwenerUniqueId);
		m_pSetManorCombat->GetAccessor()->SetChallengerUniqueId(iChallengerUniqueId);
		m_pSetManorCombat->GetAccessor()->SetCallengerNumber(iCallengerNumber);
		m_pSetManorCombat->GetAccessor()->SetDefendCastleResult(szTempDefend);
		m_pSetManorCombat->GetAccessor()->SetAttackCastleResult(szTempAttack);
		m_pSetManorCombat->GetAccessor()->SetManorUniqueId(szTempManor);
		m_pSetManorCombat->GetAccessor()->SetFlag(sFlag);

		m_pSetManorCombat->CloseCommand();

		HRESULT hr = m_pSetManorCombat->OpenCommand(*m_session);
		if(FAILED(hr))
		{
			DBErrorOutput( hr );
			if( hr == E_FAIL) CallReConTh();
			return 0;
		}
		//출력
		printf( "[Manor] Read ManorCombat Information\n" );
		printf( "[ManorCombat]  \n\
			iOwenerUniqueId=%d, iChallengerUniqueId=%d, iCallengerNumber=%d, iDefendCastleResult=%d, iAttackCastleResult=%d  iManorUniqueId=%d sFlag=%d\n"
			,iOwenerUniqueId, iChallengerUniqueId, iCallengerNumber, iDefendCastleResult, iAttackCastleResult, iManorUniqueId, sFlag);
		
	}
	catch(...){
		::PrintConsole("[EXCEPTION] ==> Update ManorCombat\n" );
		m_pSetManorCombat->CloseCommand();
		return 0;		
	}
	return 1;
}
#endif

#ifdef	_PD_CASTLE_STEP_2_
int COLE_DB_Executer::GetAllCombatRecord()
{
	int nCount = 0;
	CString	strQry;
	C_prDSGetCombatRecord	combatRecord;
	int iOrgIndex(0),iManorIndex(0),iRecordCount(0),iVSIndex(0),iWinCount(0),iLoseCount(0),i(0);
	int iVSAllyIndex(0);// 타단의 회 인덱스 0이면 단이고 etc : 회의 인덱스
	bool bValid = false;
	BYTE orgRecord[4000];
	BYTE *pos;
	
	size_t recordSize = 40;

	strQry = " SELECT * FROM ND_V01_Dan_Standings_bi";

	HRESULT hr = combatRecord.OpenCommand(*m_session,strQry);

	if(FAILED(hr))
	{
		DBErrorOutput(hr);
		if(hr==E_FAIL)	CallReConTh();
		return -1;
	}
	combatRecord.ResetAccessor();


	while(combatRecord.GetCustomer()->MoveNext() == S_OK)
	{
		iOrgIndex		=	combatRecord.GetAccessor()->GetMyIndex();
		iManorIndex		=	atoi(combatRecord.GetAccessor()->GetManorCode());
		iRecordCount	=	combatRecord.GetAccessor()->GetRecordCount();		
		memcpy(orgRecord,combatRecord.GetAccessor()->GetCombatRecomd(),recordSize*100);

        pos = orgRecord;                

		for(i=0;i<iRecordCount;i++)
		{
			memcpy(&iVSIndex,pos,sizeof(int));				
			memcpy(&iWinCount,pos+4,sizeof(int));				
			memcpy(&iLoseCount,pos+8,sizeof(int));
			memcpy(&iVSAllyIndex,pos+12,sizeof(int));
			pos+=16;
			CDSCastleManager::GetObj().GetRecord()->LoadVsOrgRecord(iOrgIndex,iManorIndex,iVSIndex,iWinCount,iLoseCount,iVSAllyIndex,(u_char*)pos);			
			g_pSystemLog->LogFileWrite( "[Combat_Record]  -GetAllCombatRecord-RecordCount(%d) myOrg(%d) VS OtherOrg(%d) WinCount(%d) LoseCount(%d) VSAllyIndex(%d)\n",iRecordCount,iOrgIndex, iVSIndex,iWinCount,iLoseCount,iVSAllyIndex);				
			pos+=24;
		}		
		++nCount;
	}
	combatRecord.CloseCommand();
	return nCount;
}

int COLE_DB_Executer::GetCombatRecordInManor()
{
	int nCount = 0;
	CString	strQry;
	C_prDSGetRecordInManor	combatRecord;
	CRecordOfCombat* pRecord = CDSCastleManager::GetObj().GetRecord();
	int iAllyIndex(0),iManorIndex(0),iEntryCount(0),iWinCount(0),iLoseCount(0),iCombatDate(0);
	DBTIMESTAMP combatTimeByDB = {0,};
    
	strQry = " SELECT * FROM ND_V01_ManorWarRecord";	

	HRESULT hr = combatRecord.OpenCommand(*m_session,strQry);

	if(FAILED(hr))
	{
		DBErrorOutput(hr);
		if(hr==E_FAIL)	CallReConTh();
		return -1; 
	}
	combatRecord.ResetAccessor();
	while(combatRecord.GetCustomer()->MoveNext() == S_OK)
	{
		iAllyIndex	=	combatRecord.GetAccessor()->GetMyDanIndex();
		iManorIndex	=	atoi(combatRecord.GetAccessor()->GetManorCode());
		iEntryCount	=	combatRecord.GetAccessor()->GetCombatCount();
		iWinCount	=	combatRecord.GetAccessor()->GetWinCount();
		iLoseCount	=	combatRecord.GetAccessor()->GetLoseCount();
		iCombatDate =	combatRecord.GetAccessor()->GetCombatDate();
		combatTimeByDB = combatRecord.GetAccessor()->GetCombatDateDBTime();
		pRecord->LoadCombatRecord(iAllyIndex,iManorIndex,iEntryCount,iWinCount,iLoseCount,combatTimeByDB );
		g_pSystemLog->LogFileWrite( "[Combat_Record]  GetCombatRecordInManor- Year(%d)  Month(%d) Day(%d)",combatTimeByDB.year,combatTimeByDB.month,combatTimeByDB.day);				
		
		++nCount;
	}
	combatRecord.CloseCommand();
	return nCount;
}

int COLE_DB_Executer::InsertOrgStandings(int orgIndex,int againstOrgIndex)
{
	COrgRecord*		pOrg = CDSCastleManager::GetObj().GetRecord()->FindOrgRecord(orgIndex);
	_sOrgRecord* pRecord = pOrg->FindVSOrgRecord(againstOrgIndex);

	if(!pRecord)
	{
		printf( "[Combat Record]	Error Org index = %d, \n", orgIndex );
		return -1;
	}
	
	m_pInsertOrgStandings->ResetAccessor();
	m_pInsertOrgStandings->GetAccessor()->SetAgainstOrgIndex(againstOrgIndex);
	m_pInsertOrgStandings->GetAccessor()->SetLoseCount(pRecord->GetLoseCount());
	m_pInsertOrgStandings->GetAccessor()->SetManorIndex(pOrg->GetManorIndex());
	m_pInsertOrgStandings->GetAccessor()->SetOrgIndex(orgIndex);
	m_pInsertOrgStandings->GetAccessor()->SetWinCount(pRecord->GetWinCount());	

	HRESULT hr = m_pInsertOrgStandings->OpenCommand( *m_session );

	if( FAILED(hr) )
	{
		DBErrorOutput( hr ) ;
	}

	m_pCastleInsert->CloseCommand() ;
	return m_pCastleInsert->GetAccessor()->GetResult();    
}

int COLE_DB_Executer::UpdateOrgStandings_bi(int index)
{
	COrgRecord* pOrg = CDSCastleManager::GetObj().GetRecord()->FindOrgRecord(index);
	if(!pOrg)
	{
		printf( "[Combat Record]	Error Org index = %d, \n", index );
		return -1;
	}
	BYTE combat_record[4000] = {0,};
	int count = pOrg->UpdateOrgStandings(combat_record);
	int vsOrgIndex = 0;
	memcpy(&vsOrgIndex,combat_record,sizeof(int));
	g_pSystemLog->LogFileWrite( "[Combat_Record] UpdateOrgStandings_bi-vsOrgIndex(%d) ",vsOrgIndex);
	memcpy(&vsOrgIndex,combat_record+4,sizeof(int));
	g_pSystemLog->LogFileWrite( "[Combat_Record] UpdateOrgStandings_bi-WinCount(%d) ",vsOrgIndex);
	memcpy(&vsOrgIndex,combat_record+8,sizeof(int));
	g_pSystemLog->LogFileWrite( "[Combat_Record] UpdateOrgStandings_bi-LoseCount(%d) ",vsOrgIndex);
	memcpy(&vsOrgIndex,combat_record+12,sizeof(int));
	g_pSystemLog->LogFileWrite( "[Combat_Record] UpdateOrgStandings_bi-vsAllyIndex(%d)\n ",vsOrgIndex);

	if(0==count)
	{
		printf( "[Combat Record]	Error Record Zero index = %d, \n", index );		
		return -1;        
	}
	m_pUpdateOrgStandings_bi->ResetAccessor();
	m_pUpdateOrgStandings_bi->GetAccessor()->SetCombatRecord(combat_record);
	m_pUpdateOrgStandings_bi->GetAccessor()->SetManorIndex(pOrg->GetManorIndex());
	m_pUpdateOrgStandings_bi->GetAccessor()->SetOrgIndex(index);
	m_pUpdateOrgStandings_bi->GetAccessor()->SetRecordCount(count);	
	// 유효값은 영웅단이 지워졌을 때 체크해야 할듯;
	//m_pUpdateOrgStandings_bi->GetAccessor()->SetValid();
	g_pSystemLog->LogFileWrite( "[Combat_Record] MyIndex(%d) VS orgInde(%d) ManorIndex(%d)count(%d)-UpdateOrgStandings_bi\n",index,vsOrgIndex,pOrg->GetManorIndex(),count);
	HRESULT hr = m_pUpdateOrgStandings_bi->OpenCommand(*m_session);
	if(FAILED(hr))
	{
		DBErrorOutput( hr );
	}
	m_pUpdateOrgStandings_bi->CloseCommand(); 
	return 0;
}

int COLE_DB_Executer::UpdateOrgRecordInManor(int index)
{
	COrgRecord* pRecord = CDSCastleManager::GetObj().GetRecord()->FindOrgRecord(index);
	HRESULT hr;
	if(!pRecord)
	{
		g_pSystemLog->LogFileWrite( "[Combat Record]	Error Manor index = %d(1), \n", index);		
		return -1;
	}

	pRecord->SetManorIndex( CDSCastleManager::GetObj().GetRecord()->GetManorIndex() );
	_sCombatRecord* pManor = pRecord->FindCombatRecord( pRecord->GetManorIndex() );
	
	if(!pManor)
	{
		g_pSystemLog->LogFileWrite( "[Combat Record]	Error Manor index = %d(2), \n", pRecord->GetManorIndex());		
		return -1;
	}

	m_pUpdateOrgRecordInManor->ResetAccessor();
	m_pUpdateOrgRecordInManor->GetAccessor()->SetCombatCount(pManor->GetEnrtyCount());
	m_pUpdateOrgRecordInManor->GetAccessor()->SetLoseCount(pManor->GetLoseCount());
	m_pUpdateOrgRecordInManor->GetAccessor()->SetManorIndex(pRecord->GetManorIndex());
	m_pUpdateOrgRecordInManor->GetAccessor()->SetOrgIndex(index);
	m_pUpdateOrgRecordInManor->GetAccessor()->SetTotalCount(pManor->GetWinCount()+pManor->GetLoseCount());
	m_pUpdateOrgRecordInManor->GetAccessor()->SetWinCount(pManor->GetWinCount());
	g_pSystemLog->LogFileWrite( "[Combat_Record] index(%d) ManorIndex(%d) CombatCount(%d) Win(%d) Lose(%d)-UpdateOrgRecordInManor\n",index,pRecord->GetManorIndex(),pManor->GetCombatCount(),pManor->GetWinCount(),pManor->GetLoseCount());
	hr = m_pUpdateOrgRecordInManor->OpenCommand(*m_session);
	if(FAILED(hr))
	{
		DBErrorOutput( hr );
	}
	m_pUpdateOrgRecordInManor->CloseCommand();  
	return 0;
}

#endif  //_PD_CASTLE_STEP_2_


#ifdef _PD_GAME_EVENT_
void COLE_DB_Executer::GetFMWinnerInfo( CGameEventManager * pGE )
{
	CString			szQuery;
	C_prDS_FM_SelectWinnerInfo		SelectFMWinner;

	szQuery = "SELECT [fm_rank], [fm_nick_id], [fm_winner_cuid] FROM [ND_V01_FM_CUR_WINNER]";

	HRESULT hr = SelectFMWinner.OpenCommand( *m_session, szQuery );	
	if( FAILED(hr) )
	{
		DBErrorOutput( hr ) ;
		if(hr==E_FAIL) CallReConTh();	
		return;
	}		

	SelectFMWinner.ResetAccessor() ;

	hr = SelectFMWinner.GetCustomer()->MoveNext();
	while ( hr == S_OK ) 
	{
		pGE->AddWinnerInfoFromDB( SelectFMWinner.GetAccessor()->GetRank(), 
			SelectFMWinner.GetAccessor()->GetNickID(), SelectFMWinner.GetAccessor()->GetWinnerCUID() );

		hr = SelectFMWinner.GetCustomer()->MoveNext();
	}

	printf( "[GAME EVENT] Read FM Winner information.\n" );

	SelectFMWinner.CloseCommand() ;
}


void COLE_DB_Executer::UpdateFMWinner( int rank, int nick_id, int cuid, char * name )
{
	m_pFMUpdateWinnerInfo->ResetAccessor();

	m_pFMUpdateWinnerInfo->GetAccessor()->SetRank( rank );
	m_pFMUpdateWinnerInfo->GetAccessor()->SetNickID( nick_id );
	m_pFMUpdateWinnerInfo->GetAccessor()->SetWinnerInfo( cuid, name );

	HRESULT hr = m_pFMUpdateWinnerInfo->OpenCommand(*m_session);
	if(FAILED(hr))
		DBErrorOutput( hr );

	m_pFMUpdateWinnerInfo->CloseCommand();  
}
#endif

#ifdef _PD_PKEVENT_BOX_SPAWN_

bool COLE_DB_Executer::GetBWWarWinTime( int cuid, int& win_time )
{
	bool b = false;
	try
	{
		m_pGetBWWinTime->ResetAccessor();

		m_pGetBWWinTime->GetAccessor()->SetCUID( cuid );

		HRESULT hr = m_pGetBWWinTime->OpenCommand(*m_session);
		if(FAILED(hr))
		{
			DBErrorOutput( hr );
			win_time = 0;
		}
		else
		{
			win_time = m_pGetBWWinTime->GetAccessor()->GetWinTime();
			b = true;
		}

		m_pGetBWWinTime->CloseCommand();
	}
	catch( ... )
	{
		m_pGetBWWinTime->CloseCommand();
		win_time = 0;

		PrintAll( "[EXCEPTION] GetBWWarWinTime : Game DB SP\n" );
	}

	return b;
}

void COLE_DB_Executer::UpdateBWWarWinTime( int cuid, int win_time )
{
	try
	{
		m_pUpdateBWWinTime->ResetAccessor();

		m_pUpdateBWWinTime->GetAccessor()->SetWinTime( cuid, win_time );

		HRESULT hr = m_pUpdateBWWinTime->OpenCommand(*m_session);
		if(FAILED(hr))
		{
			DBErrorOutput( hr );
		}

		m_pUpdateBWWinTime->CloseCommand();
	}
	catch( ... )
	{
		m_pUpdateBWWinTime->CloseCommand();

		PrintAll( "[EXCEPTION] UpdateBWWarWinTime : Game DB SP\n" );
	}
}

#endif	// _PD_PKEVENT_BOX_SPAWN_

#ifdef _PD_LEVEL_PRESENT_
// LP : LevelPresent
short COLE_DB_Executer::LP_GetNotRecv( char * acc )
{
	// -1 : Fail, ret >= 0 : Event ID.
	short eid = -1;
	try{

		HRESULT hr;

		m_prLP_GetNotRecv->ResetAccessor();

		m_prLP_GetNotRecv->GetAccessor()->SetAcc( acc );

		hr = m_prLP_GetNotRecv->OpenCommand(*m_session);
		m_prLP_GetNotRecv->CloseCommand();

		if(FAILED(hr)){
			::PrintConsole( "[LP_GetNotRecv] HRESULT FAILED A : %s\n", acc );
			DBErrorOutput( hr );
			if(hr==E_FAIL) CallReConTh();			
			eid = -1;
		}
		else
			eid = m_prLP_GetNotRecv->GetAccessor()->GetEventID();


	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] C_USER_DB_Command::LP_GetNotRecv\n") ;
		m_prLP_GetNotRecv->CloseCommand() ;
		eid = -1;
	}

	return eid;
}

BOOL COLE_DB_Executer::LP_Check( int auid, char * acc, short eid, short level, char * name )
{
	BOOL ret = FALSE;
	try{

		HRESULT hr;

		m_prLP_Check->ResetAccessor();

		m_prLP_Check->GetAccessor()->Set( auid, acc, eid, level, name );

		hr = m_prLP_Check->OpenCommand(*m_session);
		m_prLP_Check->CloseCommand();

		if(FAILED(hr)){
			::PrintConsole( "[LP_Check] HRESULT FAILED  unique_id : %d <%s>: %d, %d \n", auid, acc, eid, level );
			DBErrorOutput( hr );
			if(hr==E_FAIL) 
				CallReConTh();			
			ret = FALSE;
		}
		else
		{
			if( 0 == m_prLP_Check->GetAccessor()->GetState() )
				ret = TRUE;
		}
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] C_USER_DB_Command::LP_Check\n") ;
		m_prLP_Check->CloseCommand() ;
	}

	return ret;
}

BOOL COLE_DB_Executer::LP_Recv( char * acc, short eid, short level, char * name )
{
	BOOL ret = FALSE;
	try{

		HRESULT hr;

		m_prLP_Recv->ResetAccessor();

		m_prLP_Recv->GetAccessor()->Set( acc, eid, level, name );

		hr = m_prLP_Recv->OpenCommand(*m_session);
		m_prLP_Recv->CloseCommand();

		if(FAILED(hr)){
			::PrintConsole( "[LP_Recv] HRESULT FAILED  A<%s>\n", acc );
			DBErrorOutput( hr );
			if(hr==E_FAIL) 
				CallReConTh();			
			ret = FALSE;
		}
		else
		{
			if( 0 == m_prLP_Recv->GetAccessor()->GetRecvFlag() )
				ret = TRUE;
		}
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] C_USER_DB_Command::LP_Recv\n") ;
		m_prLP_Recv->CloseCommand() ;
	}

	return ret;
}

VOID COLE_DB_Executer::LP_Cancel( char * acc, short eid )
{
	try{

		HRESULT hr;

		m_prLP_Cancel->ResetAccessor();

		m_prLP_Cancel->GetAccessor()->Set( acc, eid );

		hr = m_prLP_Cancel->OpenCommand(*m_session);
		m_prLP_Cancel->CloseCommand();

		if(FAILED(hr)){
			::PrintConsole( "[LP_Cancel] HRESULT FAILED  A<%s>\n", acc );
			DBErrorOutput( hr );
			if(hr==E_FAIL) 
				CallReConTh();			
		}
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] C_USER_DB_Command::LP_Cancel\n") ;
		m_prLP_Cancel->CloseCommand() ;
	}
}


#endif	// _PD_LEVEL_PRESENT_

#ifdef _PD_MASTER_PUPIL_

//////////////////////////////////////////////////////////////////////////
//  MP Procedure.
//////////////////////////////////////////////////////////////////////////
bool COLE_DB_Executer::MP_GetCharacMPInfo( int cuid, _C_CHARAC * pCharac )
{
	pCharac->CHARAC_MP.ucMPPosition = en_mp_relation_none;
	pCharac->CHARAC_MP.ucRes = 0;
	pCharac->CHARAC_MP.usRes = 0;
	pCharac->CHARAC_MP.iMasterIndex = 0;

	int master_index = MP_GetMasterFromPupil( cuid );
	if( master_index )
	{
		pCharac->CHARAC_MP.iMasterIndex = master_index;
		if( cuid == master_index )
			pCharac->CHARAC_MP.ucMPPosition = en_mp_relation_master;
		else
			pCharac->CHARAC_MP.ucMPPosition = en_mp_relation_pupil;
	}

	return true;
}


int COLE_DB_Executer::MP_GetMasterFromPupil( int cuid )			
{
	int master_index = 0;
	HRESULT hr;

	try
	{
		m_prMP_GetMasterFromPupil->ResetAccessor();
		m_prMP_GetMasterFromPupil->GetAccessor()->SetPupilIndex( cuid );

		hr = m_prMP_GetMasterFromPupil->OpenCommand( *m_session );

		m_prMP_GetMasterFromPupil->CloseCommand();

		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			master_index = 0;
		}
		else
		{
			master_index = m_prMP_GetMasterFromPupil->GetAccessor()->GetMasterIndex();

#ifdef _PD_MP_DEBUG_
			PrintAll( "[PUPIL] Pupil %d -> Master %d\n", cuid, master_index );
#endif
		}
	}
	catch(...)
	{
		m_prMP_GetMasterFromPupil->CloseCommand();
	}

	return master_index;
}

bool COLE_DB_Executer::MP_ReadAndFillMasterInfo( CDSMaster * pMaster )
{
	if(!pMaster)
		return false;

	HRESULT hr;
	bool ret = false;
	_sCSPK_Pupil pupil;


	try
	{
		m_prMP_GetMasterInfo->ResetAccessor();

		m_prMP_GetMasterInfo->GetAccessor()->SetMasterIndex( pMaster->GetIndex() );
		hr = m_prMP_GetMasterInfo->OpenCommand( *m_session );
		if( FAILED(hr) || -1 == static_cast<int>(m_prMP_GetMasterInfo->GetAccessor()->m_total_point) )
		{
			DBErrorOutput(hr);
			m_prMP_GetMasterInfo->CloseCommand();
			ret = false;
		}
		else if( -1 == static_cast<int>(m_prMP_GetMasterInfo->GetAccessor()->m_total_point) )
		{
			m_prMP_GetMasterInfo->CloseCommand();
			ret = false;
		}
		else
        {
			pMaster->SetTotalKillCount( static_cast<u_int>(m_prMP_GetMasterInfo->GetAccessor()->m_total_killcount) );
			pMaster->SetMasterKillCount(0);

			pMaster->SetMasterRank( static_cast<u_char>(m_prMP_GetMasterInfo->GetAccessor()->m_master_rank) );

			pMaster->SetTotalPoint( static_cast<u_int>(m_prMP_GetMasterInfo->GetAccessor()->m_total_point) );
			pMaster->SetCurPoint( static_cast<u_int>(m_prMP_GetMasterInfo->GetAccessor()->m_cur_point) );

			pMaster->SetMasterLevel( static_cast<u_char>( m_prMP_GetMasterInfo->GetAccessor()->m_cur_level ) );		// Rank.
			pMaster->SetMasterSex( static_cast<u_char>( m_prMP_GetMasterInfo->GetAccessor()->m_gender ) );
			pMaster->SetMasterMoonpa( static_cast<u_char>( m_prMP_GetMasterInfo->GetAccessor()->m_moonpa ) );
			pMaster->SetMasterClass( static_cast<u_char>( m_prMP_GetMasterInfo->GetAccessor()->m_class ) );
			pMaster->SetMasterGrade( static_cast<u_char>( m_prMP_GetMasterInfo->GetAccessor()->m_classgrade ) );
            
			
			pMaster->SetMasterName( m_prMP_GetMasterInfo->GetAccessor()->m_name );
			pMaster->SetMasterNick( m_prMP_GetMasterInfo->GetAccessor()->m_nick );

			pMaster->SetCreateDate( m_prMP_GetMasterInfo->GetAccessor()->m_create );
			pMaster->SetLastConDate( m_prMP_GetMasterInfo->GetAccessor()->m_last_con );

			m_prMP_GetMasterInfo->CloseCommand();

			m_prMP_GetPupilInfo->ResetAccessor();

#ifdef _PD_MP_DEBUG_
			PrintAll( "[MP] Master Info : Index(%d), Rank(%d), TotalPoint(%d), CurPoint(%d)\n", 
				pMaster->GetIndex(), pMaster->GetMasterRank(), pMaster->GetTotalPoint(), pMaster->GetCurPoint() );
			PrintAll( "    KillCount(%d), Name(%s), Nick(%s)\n",
				pMaster->GetTotalKillCount(), pMaster->GetMasterName(), pMaster->GetMasterNick() );
			PrintAll( "   Create(%d-%d-%d %d:%d)\n", 
				m_prMP_GetMasterInfo->GetAccessor()->m_create.year, 
				m_prMP_GetMasterInfo->GetAccessor()->m_create.month,
				m_prMP_GetMasterInfo->GetAccessor()->m_create.day,
				m_prMP_GetMasterInfo->GetAccessor()->m_create.hour,
				m_prMP_GetMasterInfo->GetAccessor()->m_create.minute );
#endif

			CDB_Command<_db_prDS_MP_GetPupilInfo>		GetPupilInfo;

			GetPupilInfo.ResetAccessor();

			CString str;
			str.Format( "SELECT pupil_index, pupil_name, pupil_position, total_point,\
				cur_point, my_killcount, gender, cur_level, moonpa, [class], class_grade, \
				join_date, last_connect_date FROM ND_V01_Pupil WHERE master_index = %d", pMaster->GetIndex() );

			hr = GetPupilInfo.OpenCommand( *m_session, str );
			if( FAILED(hr) )
			{
				DBErrorOutput(hr);
				GetPupilInfo.CloseCommand();
				ret = false;
			}
			else
			{
				_db_prDS_MP_GetPupilInfo * pInfo = NULL;
				while( S_OK == GetPupilInfo.GetCustomer()->MoveNext() )
				{
					pInfo = GetPupilInfo.GetAccessor();

					pupil.iPupilIndex = pInfo->pupil_index;
					pupil.uiMyKillCount = 0;
					pupil.uiToMasterPoint = static_cast<u_int>(pInfo->total_point);
					pupil.uiCurPoint = static_cast<u_int>(pInfo->cur_point);
					pupil.SetJoinDate( pInfo->join );
					pupil.ucPupilPosition = static_cast<u_char>(pInfo->pupil_pos);
					pupil.ucSex = static_cast<u_char>(pInfo->gender);
					pupil.ucLevel = static_cast<u_char>(pInfo->cur_level);
					pupil.ucMoonpa = static_cast<u_char>(pInfo->moonpa);
					pupil.ucClass = static_cast<u_char>(pInfo->_class);
					pupil.ucClassGrade = static_cast<u_char>(pInfo->classgrade);
					pupil.SetName( pInfo->pupil_name );

#ifdef _PD_MP_DEBUG_
					PrintAll( "[Pupil] Index(%d), TotalPoint(%d), CurPoint(%d), Join(%02d-%02d-%02d, %02d:%02d\n",
						pupil.iPupilIndex, pupil.uiToMasterPoint, pupil.uiCurPoint, 
						pupil.join_date.uiYear, pupil.join_date.uiMonth, pupil.join_date.uiDay, pupil.join_date.uiHour, pupil.join_date.uiMinute );
					PrintAll( "   Sex(%d), level(%d), Moonpa(%d), class(%d), Name(%s)\n", 
						pupil.ucSex, pupil.ucLevel, pupil.ucMoonpa, pupil.ucClass, pupil.cPupilName );
#endif

					pMaster->AddPupilFromDB( pupil );
				}

				ret = true;

				GetPupilInfo.CloseCommand();
			}
		}
	}
	catch( ... )
	{	
		PrintAll( "[EXCEPTION]  MP_MakeMasterInfo\n" );
		m_prMP_GetMasterInfo->CloseCommand();
		ret = false;
	}
	
	return ret;
}


bool COLE_DB_Executer::MP_ReadMPInfo( CDSMaster * pMaster )
{
	bool ret = false;
	HRESULT hr;
	if( pMaster )
	{
		try
		{
			m_prMP_GetMasterInfo->ResetAccessor();

			m_prMP_GetMasterInfo->GetAccessor()->SetMasterIndex( pMaster->GetIndex() );

			hr = m_prMP_GetMasterInfo->OpenCommand( *m_session );
			if( FAILED(hr) )
			{
				DBErrorOutput(hr);
				PrintAll( "[DB_ERR] MP_ReadMPInfo()\n" );
				m_prMP_GetMasterInfo->CloseCommand();
				ret = false;
			}
			else
			{
			}

		}
		catch( ... )
		{
			PrintAll( "[EXCEPTION]  MP_ReadMPInfo\n" );
			m_prMP_GetMasterInfo->CloseCommand();
			ret = false;

		}

		
	}

	return ret;
}


bool COLE_DB_Executer::MP_CreateMasterInfo( CDSMaster * pMaster )
{
	if( !pMaster )
		return false;

	HRESULT hr;
	bool ret = false;

	try
	{
		m_prMP_CreateMasterInfo->ResetAccessor();

		m_prMP_CreateMasterInfo->GetAccessor()->SetMasterIndex( pMaster->GetIndex() );

		m_prMP_CreateMasterInfo->GetAccessor()->m_total_point = static_cast<__int16>(pMaster->GetTotalPoint());
		m_prMP_CreateMasterInfo->GetAccessor()->m_cur_point = static_cast<__int16>(pMaster->GetCurPoint());
		m_prMP_CreateMasterInfo->GetAccessor()->m_total_killcount = static_cast<int>(pMaster->GetTotalKillCount());
		m_prMP_CreateMasterInfo->GetAccessor()->m_master_rank = static_cast<short>(pMaster->GetMasterRank());
		m_prMP_CreateMasterInfo->GetAccessor()->m_gender = static_cast<short>(pMaster->GetMasterSex());
		m_prMP_CreateMasterInfo->GetAccessor()->m_cur_level = static_cast<short>(pMaster->GetMasterLevel());
		m_prMP_CreateMasterInfo->GetAccessor()->m_moonpa = static_cast<short>(pMaster->GetMasterMoonpa());
		m_prMP_CreateMasterInfo->GetAccessor()->m_class = static_cast<short>(pMaster->GetMasterClass());
		m_prMP_CreateMasterInfo->GetAccessor()->m_classgrade =static_cast<short>(pMaster->GetMasterClassGrade());
		m_prMP_CreateMasterInfo->GetAccessor()->SetCreateDate( pMaster->GetCreateTime() );
		m_prMP_CreateMasterInfo->GetAccessor()->SetMasterName( pMaster->GetMasterName() );

		hr = m_prMP_CreateMasterInfo->OpenCommand( *m_session );
		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			PrintAll( "[DB_ERR] MP - CreateMasterInfo\n" );
		}
		else
		{
			ret = true;
		}

		m_prMP_CreateMasterInfo->CloseCommand();

	}
	catch( ... )
	{
		PrintAll( "[Exception] MP - CreateMasterInfo\n" );
		
		m_prMP_CreateMasterInfo->CloseCommand();

		ret = false;
	}

	return ret;
	
}

bool COLE_DB_Executer::MP_UpdateMasterClass( CDSMaster * pMaster )
{
	if( !pMaster )
		return false;

	HRESULT hr;
	bool ret = false;

	try
	{
		m_prMP_UpdateMasterClass->ResetAccessor();

		m_prMP_UpdateMasterClass->GetAccessor()->SetMasterIndex( pMaster->GetIndex() );

		m_prMP_UpdateMasterClass->GetAccessor()->m_moonpa = static_cast<short>(pMaster->GetMasterMoonpa());
		m_prMP_UpdateMasterClass->GetAccessor()->m_class = static_cast<short>(pMaster->GetMasterClass());
		m_prMP_UpdateMasterClass->GetAccessor()->m_classgrade =static_cast<short>(pMaster->GetMasterClassGrade());

		hr = m_prMP_UpdateMasterClass->OpenCommand( *m_session );
		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			PrintAll( "[DB_ERR] MP - UpdateMasterClass\n" );
		}
		else
		{
			ret = true;
		}

		m_prMP_UpdateMasterClass->CloseCommand();

	}
	catch( ... )
	{
		PrintAll( "[Exception] MP - UpdateMasterClass\n" );

		m_prMP_UpdateMasterClass->CloseCommand();

		ret = false;
	}

	return ret;
}

bool COLE_DB_Executer::MP_UpdateMasterNick( CDSMaster * pMaster )
{
	if( !pMaster )
		return false;

	HRESULT hr;
	bool ret = false;

	try
	{
		m_prMP_UpdateMasterNick->ResetAccessor();

		m_prMP_UpdateMasterNick->GetAccessor()->SetMasterIndex( pMaster->GetIndex() );

		m_prMP_UpdateMasterNick->GetAccessor()->SetNick( pMaster->GetMasterNick() );

		hr = m_prMP_UpdateMasterNick->OpenCommand( *m_session );
		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			PrintAll( "[DB_ERR] MP - UpdateMasterNick\n" );
		}
		else
		{
			ret = true;
		}

		m_prMP_UpdateMasterNick->CloseCommand();

	}
	catch( ... )
	{
		PrintAll( "[Exception] MP - UpdateMasterNick\n" );

		m_prMP_UpdateMasterNick->CloseCommand();

		ret = false;
	}

	return ret;
}



bool COLE_DB_Executer::MP_UpdateMasterInfo( CDSMaster * pMaster )
{
	if( !pMaster )
		return false;

	HRESULT hr;
	bool ret = false;

	try
	{
		m_prMP_UpdateMasterInfo->ResetAccessor();

		m_prMP_UpdateMasterInfo->GetAccessor()->SetMasterIndex( pMaster->GetIndex() );

		m_prMP_UpdateMasterInfo->GetAccessor()->m_total_point = static_cast<__int16>(pMaster->GetTotalPoint());
		m_prMP_UpdateMasterInfo->GetAccessor()->m_cur_point = static_cast<__int16>(pMaster->GetCurPoint());
		m_prMP_UpdateMasterInfo->GetAccessor()->m_total_killcount = static_cast<int>(pMaster->GetTotalKillCount());
		m_prMP_UpdateMasterInfo->GetAccessor()->m_master_rank = static_cast<short>(pMaster->GetMasterRank());
		m_prMP_UpdateMasterInfo->GetAccessor()->m_cur_level = static_cast<short>(pMaster->GetMasterLevel());
		m_prMP_UpdateMasterInfo->GetAccessor()->SetConDate( pMaster->GetLastConTime() );

		hr = m_prMP_UpdateMasterInfo->OpenCommand( *m_session );
		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			PrintAll( "[DB_ERR] MP - UpdateMasterInfo\n" );
		}
		else
		{
			ret = true;
		}

		m_prMP_UpdateMasterInfo->CloseCommand();

	}
	catch( ... )
	{
		PrintAll( "[Exception] MP - UpdateMasterInfo\n" );

		m_prMP_UpdateMasterInfo->CloseCommand();

		ret = false;
	}

	return ret;
}


// 제자 관련 프로시저. 
bool COLE_DB_Executer::MP_AddNewPupil( _sCSPK_Pupil& temp, int master_index )
{
	HRESULT hr;
	bool ret = false;

	try
	{
		m_prMP_AddNewPupil->ResetAccessor();

		m_prMP_AddNewPupil->GetAccessor()->m_iPupilIndex = temp.iPupilIndex;
		m_prMP_AddNewPupil->GetAccessor()->SetName( temp.cPupilName );
		m_prMP_AddNewPupil->GetAccessor()->m_iMasterIndex = master_index;
		m_prMP_AddNewPupil->GetAccessor()->m_sPosition = static_cast<short>(temp.ucPupilPosition);
		m_prMP_AddNewPupil->GetAccessor()->m_sGender = static_cast<short>(temp.ucSex);
		m_prMP_AddNewPupil->GetAccessor()->m_sCurLevel = static_cast<short>(temp.ucLevel);
		m_prMP_AddNewPupil->GetAccessor()->m_sMoonpa = static_cast<short>(temp.ucMoonpa);
		m_prMP_AddNewPupil->GetAccessor()->m_sClass = static_cast<short>(temp.ucClass);
		m_prMP_AddNewPupil->GetAccessor()->m_sClassGrade = static_cast<short>(temp.ucClassGrade);

		hr = m_prMP_AddNewPupil->OpenCommand( *m_session );
		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			PrintAll( "[DB_ERR] MP - Add new pupil( C<%s> %d)\n",temp.cPupilName, temp.iPupilIndex );
		}
		else
		{
			ret = true;
		}

		m_prMP_AddNewPupil->CloseCommand();

	}
	catch( ... )
	{
		PrintAll( "[Exception] MP - Add new pupil(%d)\n", temp.iPupilIndex );

		m_prMP_AddNewPupil->CloseCommand();

		ret = false;
	}


	return ret;
}


bool COLE_DB_Executer::MP_UpdatePupilPosition( int pupil, short pos )
{
	HRESULT hr;
	bool ret = false;

	try
	{
		m_prMP_UpdatePupilBase->ResetAccessor();

		m_prMP_UpdatePupilBase->GetAccessor()->SetPupilIndex( pupil );
		m_prMP_UpdatePupilBase->GetAccessor()->m_where = 0;		// pupil position
		m_prMP_UpdatePupilBase->GetAccessor()->m_value = pos;
		
		hr = m_prMP_UpdatePupilBase->OpenCommand( *m_session );
		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			PrintAll( "[DB_ERR] MP - Update pupil pos(%d - %d)\n", pupil, pos );
		}
		else
		{
			ret = true;
		}

		m_prMP_UpdatePupilBase->CloseCommand();

	}
	catch( ... )
	{
		PrintAll( "[Exception] Update pupil pos(%d-%d)\n", pupil, pos );

		m_prMP_UpdatePupilBase->CloseCommand();

		ret = false;
	}

	return ret;
}


bool COLE_DB_Executer::MP_UpdatePupilCurLevel( int pupil, short lv )
{
	HRESULT hr;
	bool ret = false;

	try
	{
		m_prMP_UpdatePupilBase->ResetAccessor();

		m_prMP_UpdatePupilBase->GetAccessor()->SetPupilIndex( pupil );
		m_prMP_UpdatePupilBase->GetAccessor()->m_where = 1;		// pupil level
		m_prMP_UpdatePupilBase->GetAccessor()->m_value = lv;

		hr = m_prMP_UpdatePupilBase->OpenCommand( *m_session );
		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			PrintAll( "[DB_ERR] MP - Update pupil level(%d - %d)\n", pupil, lv );
		}
		else
		{
			ret = true;
		}

		m_prMP_UpdatePupilBase->CloseCommand();

	}
	catch( ... )
	{
		PrintAll( "[Exception] Update pupil level(%d-%d)\n", pupil, lv );

		m_prMP_UpdatePupilBase->CloseCommand();

		ret = false;
	}

	return ret;
}


bool COLE_DB_Executer::MP_UpdatePupilClass( int pupil, short cls )
{
	HRESULT hr;
	bool ret = false;

	try
	{
		m_prMP_UpdatePupilBase->ResetAccessor();

		m_prMP_UpdatePupilBase->GetAccessor()->SetPupilIndex( pupil );
		m_prMP_UpdatePupilBase->GetAccessor()->m_where = 2;		// pupil class
		m_prMP_UpdatePupilBase->GetAccessor()->m_value = cls;

		hr = m_prMP_UpdatePupilBase->OpenCommand( *m_session );
		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			PrintAll( "[DB_ERR] MP - Update pupil class(%d - %d)\n", pupil, cls );
		}
		else
		{
			ret = true;
		}

		m_prMP_UpdatePupilBase->CloseCommand();

	}
	catch( ... )
	{
		PrintAll( "[Exception] Update pupil class(%d-%d)\n", pupil, cls );

		m_prMP_UpdatePupilBase->CloseCommand();

		ret = false;
	}

	return ret;
}


bool COLE_DB_Executer::MP_UpdatePupilClassGrade( int pupil, short class_grade )
{
	HRESULT hr;
	bool ret = false;

	try
	{
		m_prMP_UpdatePupilBase->ResetAccessor();

		m_prMP_UpdatePupilBase->GetAccessor()->SetPupilIndex( pupil );
		m_prMP_UpdatePupilBase->GetAccessor()->m_where = 3;		// pupil classgrade
		m_prMP_UpdatePupilBase->GetAccessor()->m_value = class_grade;

		hr = m_prMP_UpdatePupilBase->OpenCommand( *m_session );
		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			PrintAll( "[DB_ERR] MP - Update pupil class grade(%d - %d)\n", pupil, class_grade );
		}
		else
		{
			ret = true;
		}

		m_prMP_UpdatePupilBase->CloseCommand();

	}
	catch( ... )
	{
		PrintAll( "[Exception] Update pupil class grade(%d-%d)\n", pupil, class_grade );

		m_prMP_UpdatePupilBase->CloseCommand();

		ret = false;
	}

	return ret;
}


bool COLE_DB_Executer::MP_UpdatePupilMoonpa( int pupil, short moonpa )
{
	HRESULT hr;
	bool ret = false;

	try
	{
		m_prMP_UpdatePupilBase->ResetAccessor();

		m_prMP_UpdatePupilBase->GetAccessor()->SetPupilIndex( pupil );
		m_prMP_UpdatePupilBase->GetAccessor()->m_where = 4;		// pupil moonpa
		m_prMP_UpdatePupilBase->GetAccessor()->m_value = moonpa;

		hr = m_prMP_UpdatePupilBase->OpenCommand( *m_session );
		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			PrintAll( "[DB_ERR] MP - Update pupil moonpa(%d - %d)\n", pupil, moonpa );
		}
		else
		{
			ret = true;
		}

		m_prMP_UpdatePupilBase->CloseCommand();

	}
	catch( ... )
	{
		PrintAll( "[Exception] Update pupil moonpa(%d-%d)\n", pupil, moonpa );

		m_prMP_UpdatePupilBase->CloseCommand();

		ret = false;
	}

	return ret;
}


bool COLE_DB_Executer::MP_UpdatePupilGender( int pupil, short gender )
{
	HRESULT hr;
	bool ret = false;

	try
	{
		m_prMP_UpdatePupilBase->ResetAccessor();

		m_prMP_UpdatePupilBase->GetAccessor()->SetPupilIndex( pupil );
		m_prMP_UpdatePupilBase->GetAccessor()->m_where = 5;		// pupil gender
		m_prMP_UpdatePupilBase->GetAccessor()->m_value = gender;

		hr = m_prMP_UpdatePupilBase->OpenCommand( *m_session );
		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			PrintAll( "[DB_ERR] MP - Update pupil gender(%d - %d)\n", pupil, gender );
		}
		else
		{
			ret = true;
		}

		m_prMP_UpdatePupilBase->CloseCommand();

	}
	catch( ... )
	{
		PrintAll( "[Exception] Update pupil gender(%d-%d)\n", pupil, gender );

		m_prMP_UpdatePupilBase->CloseCommand();

		ret = false;
	}

	return ret;
}


bool COLE_DB_Executer::MP_BackupPupil( int pupil, short reason )
{
	HRESULT hr;
	bool ret = false;

	try
	{
		m_prMP_BackupPupil->ResetAccessor();

		m_prMP_BackupPupil->GetAccessor()->SetPupilIndex( pupil, reason );

		hr = m_prMP_BackupPupil->OpenCommand( *m_session );
		if( FAILED(hr) )
		{
			DBErrorOutput(hr);
			PrintAll( "[DB_ERR] MP - Backup Pupil(%d-%d)\n", pupil, reason );
		}
		else
		{
			ret = true;
		}

		m_prMP_BackupPupil->CloseCommand();

	}
	catch( ... )
	{
		PrintAll( "[Exception] MP - Backup Pupil(%d-%d)\n", pupil, reason );

		m_prMP_BackupPupil->CloseCommand();

		ret = false;
	}

	return ret;
}

#endif


void COLE_DB_Executer::GMS_ResetCharacOrg( int uid )
{
	CCommand<CNoAccessor, CNoRowset, CNoMultipleResults>		ResetCharacOrg;
	CString szUpdateQry ;

	szUpdateQry.Format( "UPDATE ND_V01_CharacState SET or_index = 0 WHERE unique_id = %d", uid );
	HRESULT hr = ResetCharacOrg.Open( *m_session, szUpdateQry ) ;
	if( FAILED(hr) )
	{
		printf( "[error] UPDATE ND_V01_CharacState SET or_index = 0  failed \n" ) ;
	}
	ResetCharacOrg.Close() ;
		//------------------------------------------------------------------
}