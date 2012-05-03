#include "..\stdafx.h"
#include <atldbcli.h>

#include "db_command.h"


void C_USER_DB_Command::CallReConTh()
{ 
	printf( "[WARNING] USER DB DB link is disconnected \n" ) ;
	::PrintConsole( "[WARNING] USER DB link is disconnected \n" ) ;
	m_pDBCon->CallReConTh() ; 
}

// User data base
bool C_USER_DB_Command::DSLogin(IN OUT char * acc, int forest, u_short temp_uid, int * uid) 
{
	if( acc == NULL )
		return false ;

	try{

		HRESULT hr;	

		m_pDSLogin->ResetAccessor() ;

		memcpy( m_pDSLogin->GetAccessor()->m_acc, acc, en_max_lil+1 ) ;
		m_pDSLogin->GetAccessor()->m_forest = forest ;
		m_pDSLogin->GetAccessor()->m_temp_uid = temp_uid ;

		hr = m_pDSLogin->OpenCommand(*m_session);	
		m_pDSLogin->CloseCommand() ;

		*uid = m_pDSLogin->GetAccessor()->m_uid ;
	

		if(FAILED(hr)){ 
			::PrintConsole( "[LOGIN_FAILED] HRESULT FAILED  s: %s,  t: %s \n",acc, m_pDSLogin->GetAccessor()->m_acc ) ;
			DBErrorOutput( hr ) ;
			m_pDSLogin->CloseCommand() ;
			if(hr == E_FAIL) 
				CallReConTh();
			return false;
		}

		if( strncmp( acc, m_pDSLogin->GetAccessor()->m_acc, en_max_lil+1 ) != 0 )
		{
			::PrintConsole( "[LOGIN_FAILED] strcmp failed s: %s,  t: %s \n",acc, m_pDSLogin->GetAccessor()->m_acc ) ;
			return false ;
		}


		return true;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] C_USER_DB_Command::DSLogin \n") ;
		m_pDSLogin->CloseCommand() ;
		return false;
	}	
}


bool C_USER_DB_Command::DSGetAccountUID( IN char * acc, int& uid )
{
	if( acc == NULL )
		return false ;

	bool result = true ;

	try{
		HRESULT hr;	

		m_pDSGetAccountUID->ResetAccessor() ;

		m_pDSGetAccountUID->GetAccessor()->SetAccount( acc ) ;

		hr = m_pDSGetAccountUID->OpenCommand(*m_session);	
		m_pDSGetAccountUID->CloseCommand() ;

		if(FAILED(hr)){ 
			::PrintConsole( "[LOGIN_FAILED] DSGetAccountUID\n" ) ;
			DBErrorOutput( hr ) ;
			m_pDSGetAccountUID->CloseCommand() ;
			result = false ;
		}

		uid = m_pDSGetAccountUID->GetAccessor()->GetAccUID() ;
		if( 0 == uid )	// UID 가 0이면 실패다. 
			result = false ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] C_USER_DB_Command::DSGetAccountUID \n") ;
		m_pDSGetAccountUID->CloseCommand() ;
		result = false ;
	}

	return result ; 
}


bool C_USER_DB_Command::DSNexonLogin( IN int acc_uid, IN char * acc, IN short forest, IN u_short temp )
{
	if( acc == NULL )
		return false ;

	bool result = true ;

	try{
		HRESULT hr;	

		m_pDSNexonLogin->ResetAccessor() ;

		m_pDSNexonLogin->GetAccessor()->SetAccUid( acc_uid ) ;
		m_pDSNexonLogin->GetAccessor()->SetAccount( acc ) ;
		m_pDSNexonLogin->GetAccessor()->SetForest( forest ) ;
		m_pDSNexonLogin->GetAccessor()->SetTempUid( temp ) ;

		hr = m_pDSNexonLogin->OpenCommand(*m_session);	
		m_pDSNexonLogin->CloseCommand() ;

		if(FAILED(hr)){ 
			::PrintConsole( "[LOGIN_FAILED] DSNexonLogin\n" ) ;
			DBErrorOutput( hr ) ;
			m_pDSNexonLogin->CloseCommand() ;
			if(hr == E_FAIL) 
				CallReConTh();
			result = false ;
		}

		else if( strncmp( acc, m_pDSNexonLogin->GetAccessor()->GetAccount(), en_max_lil+1 ) != 0 )
		{
			::PrintConsole( "[LOGIN_FAILED] strcmp failed s: %s,  t: %s \n",acc, m_pDSNexonLogin->GetAccessor()->GetAccount() ) ;
			result = false ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] C_USER_DB_Command::DSNexonLogin\n") ;
		m_pDSNexonLogin->CloseCommand() ;
		result = false ;
	}

	return result ; 
}


bool C_USER_DB_Command::DSLogout(char * acc) 
{
	if( acc == NULL )
		return false ;

	int pos = 0 ;
	try{		

		HRESULT hr;	
		
		m_pDSLogout->ResetAccessor() ;
		pos = 1 ;

		memcpy( m_pDSLogout->GetAccessor()->m_acc, acc, en_max_lil+1 ) ;
		pos = 2 ;

		hr = m_pDSLogout->OpenCommand(*m_session);	
		pos = 3 ;
		m_pDSLogout->CloseCommand() ;

		pos = 4 ;
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			m_pDSLogout->CloseCommand() ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}	
		pos = 5 ;

		return true;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_DB_Command::DSLogout pos:%d \n", pos) ;
		m_pDSLogout->CloseCommand() ;
		return false;
	}	
}

bool C_USER_DB_Command::DSEnterCheck(char * acc, int forest, int state, int uid) 
{
	try{

		HRESULT hr;	

		m_pDSEnterCheck->ResetAccessor() ;

		strncpy( m_pDSEnterCheck->GetAccessor()->m_acc, acc, en_max_lil+1) ;
		m_pDSEnterCheck->GetAccessor()->m_forest = forest ;
		m_pDSEnterCheck->GetAccessor()->m_state = state ;
		m_pDSEnterCheck->GetAccessor()->m_temp_uid = uid ;


		hr = m_pDSEnterCheck->OpenCommand(*m_session);	
		m_pDSEnterCheck->CloseCommand() ;

		if(FAILED(hr)){ 			
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}	

		if( m_pDSEnterCheck->GetAccessor()->m_result == 1 )
			return false ;


		return true;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_DB_Command::DSEnterCheck \n") ;
		m_pDSEnterCheck->CloseCommand() ;
		return false;
	}

}

bool C_USER_DB_Command::DSNexonEnterCheck(char * acc, int forest, int state, int uid) 
{
	try{

		HRESULT hr;	

		m_pDSNexonEnterCheck->ResetAccessor() ;

		strncpy( m_pDSNexonEnterCheck->GetAccessor()->m_acc, acc, en_max_lil+1) ;
		m_pDSNexonEnterCheck->GetAccessor()->m_forest = forest ;
		m_pDSNexonEnterCheck->GetAccessor()->m_state = state ;
		m_pDSNexonEnterCheck->GetAccessor()->m_temp_uid = uid ;


		hr = m_pDSNexonEnterCheck->OpenCommand(*m_session);	
		m_pDSNexonEnterCheck->CloseCommand() ;

		if(FAILED(hr)){ 			
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}	

		if( m_pDSNexonEnterCheck->GetAccessor()->m_result == 1 )
			return false ;


		return true;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_DB_Command::DSEnterCheck \n") ;
		m_pDSNexonEnterCheck->CloseCommand() ;
		return false;
	}
}



bool C_USER_DB_Command::AllLogout(int pwd){


	try{

		HRESULT hr;	

		m_pAllLogout->ResetAccessor() ;

		m_pAllLogout->GetAccessor()->m_pwd = pwd;

		hr = m_pAllLogout->OpenCommand(*m_session);	
		m_pAllLogout->CloseCommand() ;

		if(FAILED(hr)){ 
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}	

		return true;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_DB_Command::AllLogout \n") ;
		m_pAllLogout->CloseCommand() ;
		return false;
	}

}


bool C_USER_DB_Command::PWCheck(int uid, char * pw)
{
	try{

		HRESULT hr;	

		m_pPW_CHECK->ResetAccessor() ;

		m_pPW_CHECK->GetAccessor()->m_unique_id = uid ;
		memcpy(m_pPW_CHECK->GetAccessor()->m_pw, pw, 13) ;

		hr = m_pPW_CHECK->OpenCommand(*m_session);	
		m_pPW_CHECK->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false;
		}
		

		if( m_pPW_CHECK->GetAccessor()->m_result == 0 )
			return false ;
		else
			return true ;

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_DB_Command::PWCheck \n") ;
		m_pPW_CHECK->CloseCommand() ;
		return false;
	}	

}

int C_USER_DB_Command::IsNewJoiner( int uid )
{

	try{

		HRESULT hr;	

		m_pDSNewJoinerCheck->ResetAccessor() ;

		m_pDSNewJoinerCheck->GetAccessor()->m_uid = uid ;
		
		hr = m_pDSNewJoinerCheck->OpenCommand(*m_session);	
		m_pDSNewJoinerCheck->CloseCommand() ;

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return 0;
		}			
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_DB_Command::AllLogout \n") ;
		m_pDSNewJoinerCheck->CloseCommand() ;
		return 0;
	}	

	return m_pDSNewJoinerCheck->GetAccessor()->m_iResult ;
}


bool C_USER_DB_Command::IsPcbang( char * const ip ) 
{	
	try{		
		m_pDSGetIsPcbang->ResetAccessor() ;
		memcpy( m_pDSGetIsPcbang->GetAccessor()->m_ip, ip, 15 ) ;
		const HRESULT hr = m_pDSGetIsPcbang->OpenCommand( *m_session ) ;
		m_pDSGetIsPcbang->CloseCommand() ;

		if( FAILED(hr) )
		{
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) CallReConTh();
			return false ;
		}
	}
	catch(...) {
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		m_pDSGetIsPcbang->CloseCommand() ;
	}

	if( m_pDSGetIsPcbang->GetAccessor()->m_result > 0 )
		return true ;

	return false ;

}

short C_USER_DB_Command::DSEventCheck( IN short evid, IN char * key, IN char * acc, IN char * chrName, 
									  IN short group, OUT int& index, OUT short& packageID )
{
	short result = 0;
	try
	{
		m_prEventCheckAndGive->ResetAccessor();
		// Input
		m_prEventCheckAndGive->GetAccessor()->m_sEventID = evid;
		strncpy( m_prEventCheckAndGive->GetAccessor()->m_key, key, _EVENT_KEY_MAX_LENGTH_ );
		strncpy( m_prEventCheckAndGive->GetAccessor()->m_account, acc, 50 );
		strncpy( m_prEventCheckAndGive->GetAccessor()->m_chrName, chrName, en_charac_name_length+1 );
		m_prEventCheckAndGive->GetAccessor()->m_sGroupNo = group;
		// Output 
		m_prEventCheckAndGive->GetAccessor()->m_idx = 0;
		m_prEventCheckAndGive->GetAccessor()->m_packageID = 0;
		m_prEventCheckAndGive->GetAccessor()->m_result = 0;

		HRESULT hr = m_prEventCheckAndGive->OpenCommand( *m_session ) ;
		m_prEventCheckAndGive->CloseCommand() ;

		if( FAILED(hr) )
		{
			DBErrorOutput( hr ) ;
			throw "";
		}

		index = m_prEventCheckAndGive->GetAccessor()->m_idx;
		packageID = m_prEventCheckAndGive->GetAccessor()->m_packageID;
		result = m_prEventCheckAndGive->GetAccessor()->m_result;
	}
	catch( ... )
	{
		PrintConsole( "[DB - EXCEPTION] Event Check And Give\n" );
		m_prEventCheckAndGive->CloseCommand();

		index = 0;
		packageID = 0;
		result = 0;
	}

	return result;
}

short C_USER_DB_Command::DSEventPreCheck( IN short evid, IN char * acc, IN char * chrName, short group )
{
	short result = 0;
	try
	{
		m_prEventPreCheck->ResetAccessor();
		m_prEventPreCheck->GetAccessor()->m_sEventID = evid;
		strncpy( m_prEventPreCheck->GetAccessor()->m_account, acc, 50 );
		strncpy( m_prEventPreCheck->GetAccessor()->m_chrName, chrName, en_charac_name_length );
		m_prEventPreCheck->GetAccessor()->m_sGroupNo = g_ForestNum;

		m_prEventPreCheck->GetAccessor()->m_result = 0;

		HRESULT hr = m_prEventPreCheck->OpenCommand( *m_session ) ;
		m_prEventPreCheck->CloseCommand() ;

		if( FAILED(hr) )
		{
			DBErrorOutput( hr ) ;
			throw "";
		}

		result = m_prEventPreCheck->GetAccessor()->m_result;

	}
	catch( ... )
	{
		PrintConsole( "[DB - EXCEPTION] Event Pre Check\n" );
		m_prEventPreCheck->CloseCommand();

		result = 0;
	}
	return result;
}


void C_USER_DB_Command::DSEventCancel( IN short evid, IN int index )
{
	try
	{
		m_prEventCancel->ResetAccessor();
		m_prEventCancel->GetAccessor()->m_sEventID = evid;
		m_prEventCancel->GetAccessor()->m_idx = index;

		HRESULT hr = m_prEventCancel->OpenCommand( *m_session ) ;
		m_prEventCancel->CloseCommand() ;

		if( FAILED(hr) )
		{
			DBErrorOutput( hr ) ;
			throw "";
		}
	}
	catch( ... )
	{
		PrintConsole( "[DB - EXCEPTION] Event Cancel\n" );
		m_prEventCancel->CloseCommand();
	}
}

void C_USER_DB_Command::DSEventSuccess( IN short evid, IN int index )
{
	try
	{
		m_prEventSuccess->ResetAccessor();
		m_prEventSuccess->GetAccessor()->m_sEventID = evid;
		m_prEventSuccess->GetAccessor()->m_idx = index;

		HRESULT hr = m_prEventSuccess->OpenCommand( *m_session ) ;
		m_prEventSuccess->CloseCommand() ;

		if( FAILED(hr) )
		{
			DBErrorOutput( hr ) ;
		}
	}
	catch( ... )
	{
		PrintConsole( "[DB - EXCEPTION] Event Success\n" );
		m_prEventSuccess->CloseCommand();
	}
}


#ifdef _PD_ACCOUNT_PLAY_TIME_
void C_USER_DB_Command::AddPlayTime( IN int iAccUid, IN char * acc, IN int time, IN short gn )
{
	try
	{
		m_prAddPlayTime->ResetAccessor();
		m_prAddPlayTime->GetAccessor()->SetUniqueID( iAccUid );
		m_prAddPlayTime->GetAccessor()->SetAccountName( acc );
		m_prAddPlayTime->GetAccessor()->SetTime( time );
		m_prAddPlayTime->GetAccessor()->SetGroup( gn );

		HRESULT hr = m_prAddPlayTime->OpenCommand( *m_session );
		m_prAddPlayTime->CloseCommand();

		if( FAILED(hr) )
		{
			DBErrorOutput( hr ) ;
		}		
	}
	catch( ... )
	{
		PrintConsole( "[DB - EXCEPTION] AddPlayTime\n" );
		m_prAddPlayTime->CloseCommand();
	}
}
#endif

#ifdef _VINA_SHUTDOWN_20061023_
bool C_USER_DB_Command::GetFatigueInfo( int uid, CUser * pUser )
{
	//uid 검사
	try{

		HRESULT hr;

		m_pGetFatigueInfo->ResetAccessor() ;

		m_pGetFatigueInfo->GetAccessor()->SetUniqueID( uid ) ;
		
		hr = m_pGetFatigueInfo->OpenCommand(*m_session);
		m_pGetFatigueInfo->CloseCommand() ;

		if(FAILED(hr)){
			::PrintConsole( "[GET_FATIGUE_FAILED] HRESULT FAILED  unique_id : %d \n", uid ) ;
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}

		pUser->FatigueElement.sFatigueWeight = m_pGetFatigueInfo->GetAccessor()->GetFatigueWeight() ;
		pUser->FatigueElement.iLogoutTime = m_pGetFatigueInfo->GetAccessor()->GetLogoutTime() ;
		::PrintConsole( "[GET_FATIGUE_SUCCESS] unique_id : %d, fatigue_weight : %d \n", uid, m_pGetFatigueInfo->GetAccessor()->GetFatigueWeight()  ) ;
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] C_USER_DB_Command::GetFatigueInfo\n") ;
		m_pGetFatigueInfo->CloseCommand() ;
		return false ;
	}

	//pUser 정보 저장
	return true ;
}

bool C_USER_DB_Command::UpdateFatigueInfo( int uid, short sFatigueWeight )
{
	try{

		HRESULT hr;

		m_pUpdateFatigueInfo->ResetAccessor() ;

		m_pUpdateFatigueInfo->GetAccessor()->SetUniqueID( uid ) ;
		m_pUpdateFatigueInfo->GetAccessor()->UpdateFatigueWeight( sFatigueWeight ) ;
		
		hr = m_pUpdateFatigueInfo->OpenCommand(*m_session);
		m_pUpdateFatigueInfo->CloseCommand() ;

		if(FAILED(hr)){
			::PrintConsole( "[UPDATE_FATIGUE_FAILED] HRESULT FAILED  unique_id : %d,  fatigue_weight: %d \n", uid, sFatigueWeight ) ;
			DBErrorOutput( hr ) ;
			if(hr==E_FAIL) CallReConTh();			
			return false ;
		}
		::PrintConsole( "[UPDATE_FATIGUE_SUCCESS] unique_id : %d, fatigue_weight : %d \n", uid, m_pUpdateFatigueInfo->GetAccessor()->GetFatigueWeight() ) ;
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] C_USER_DB_Command::UpdateFatigueInfo\n") ;
		m_pUpdateFatigueInfo->CloseCommand() ;
		return false ;
	}

	return true ;
}
#endif	// _VINA_SHUTDOWN_20061023_

#ifdef _PD_EVENT_INVITE_FRIEND_
bool C_USER_DB_Command::IF_GetInfo( char * acc, int &index, int &endTime, short &checkState )
{
	bool ret = true;
	try{

		HRESULT hr;

		m_pGetInviteFriendState->ResetAccessor();
		m_pGetInviteFriendState->GetAccessor()->Set( acc );

		hr = m_pGetInviteFriendState->OpenCommand(*m_session);
		m_pGetInviteFriendState->CloseCommand();

		if(FAILED(hr)){
			::PrintConsole( "[IF_GetInfo] HRESULT FAILED  A<%s>\n", acc );
			DBErrorOutput( hr );
			if(hr==E_FAIL) 
				CallReConTh();	

			ret = false;
		}
		else
		{
			index = m_pGetInviteFriendState->GetAccessor()->GetIndex();
			endTime = m_pGetInviteFriendState->GetAccessor()->GetEndTime();
			checkState = m_pGetInviteFriendState->GetAccessor()->GetState();
		}
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] C_USER_DB_Command::IF_GetInfo\n") ;
		m_pGetInviteFriendState->CloseCommand() ;
	}

	return ret;
}


void C_USER_DB_Command::IF_SetStart( int index, int endTime )
{
	try{

		HRESULT hr;

		m_pUpdateInviteFriendState->ResetAccessor();
		m_pUpdateInviteFriendState->GetAccessor()->SetStartTime( index, endTime, g_ForestNum );

		hr = m_pUpdateInviteFriendState->OpenCommand(*m_session);
		m_pUpdateInviteFriendState->CloseCommand();

		if(FAILED(hr)){
			::PrintConsole( "[IF_SetStart] HRESULT FAILED\n" );
			DBErrorOutput( hr );
			if(hr==E_FAIL) 
				CallReConTh();	
		}
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] C_USER_DB_Command::IF_SetStart\n") ;
		m_pUpdateInviteFriendState->CloseCommand() ;
	}
}


void C_USER_DB_Command::IF_EndUse( int index )
{
	try{

		HRESULT hr;

		m_pUpdateInviteFriendState->ResetAccessor();
		m_pUpdateInviteFriendState->GetAccessor()->SetEnd( index );

		hr = m_pUpdateInviteFriendState->OpenCommand(*m_session);
		m_pUpdateInviteFriendState->CloseCommand();

		if(FAILED(hr)){
			::PrintConsole( "[IF_EndUse] HRESULT FAILED\n" );
			DBErrorOutput( hr );
			if(hr==E_FAIL) 
				CallReConTh();	
		}
	}
	catch ( ... ) {
		::PrintConsole("[EXCEPTION] C_USER_DB_Command::IF_EndUse\n") ;
		m_pUpdateInviteFriendState->CloseCommand() ;
	}
}
#endif //_PD_EVENT_INVITE_FRIEND_


