#include "..\stdafx.h"
#include <atldbcli.h>

#pragma warning(disable:4786)

#include <list>
using namespace std;

#include "db_command.h"

extern CMYTrace *			g_pTrace ;
extern CharacFileBackup();



int COLE_DB_Executer::GetUID(char * cName)
{
	
	try{
		
		HRESULT hr;	
		

		m_pGetUID->ResetAccessor() ;
		
		
		memcpy(m_pGetUID->GetAccessor()->m_chr_name, cName, 13 );
		
		
		hr = m_pGetUID->OpenCommand( *m_Session ) ;
		m_pGetUID->CloseCommand() ;
		
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) m_pDBCon->CallReConTh() ;
			return 0 ;
		}
		
		if( m_pGetUID->GetAccessor()->m_result == 1 ) {
			return 0 ;
		}		
		
	}
	catch (...) {
		m_pGetUID->CloseCommand() ;
		return 0 ;
	}
	
	return 	m_pGetUID->GetAccessor()->m_iUniqueID ;
	
}


bool COLE_DB_Executer::UpdateFriendList( int iUID, list<_Member> * pList ) 
{
	try{
		
		
		HRESULT hr;
		
		list<_Member>::iterator memberListIter ;
		
		int  iresult=0;
		
		m_pUpdateFriendList->ResetAccessor() ;
		
		m_pUpdateFriendList->GetAccessor()->m_iUniqueID = iUID ;
		
		
		int i = 0 , j = 0;
		for( memberListIter = pList->begin() ;
		memberListIter != pList->end() ;
		memberListIter++ ) 
		{
			if( memberListIter->characName[0] != 0 )
			{
				if( memberListIter->characName[0] != _GROUP_PARSING_CHAR_ ) 
				{
					m_pUpdateFriendList->GetAccessor()->m_list_data[i++] = _CHARAC_PARSING_CHAR_ ;
				}
				
				memberListIter->characName[15] = 0;
				j =  strlen(memberListIter->characName) ;			
				memcpy( &m_pUpdateFriendList->GetAccessor()->m_list_data[i], memberListIter->characName, j) ;
				i = i + j ;
			}
			else
			{
				break ;
			}
			
		}
		//m_pUpdateFriendList->GetAccessor()->m_list_data[i] = 0 ;
		
		
		
		
		hr = m_pUpdateFriendList->OpenCommand(*m_Session) ;
		m_pUpdateFriendList->CloseCommand() ;
		
		
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) m_pDBCon->CallReConTh() ;
			return false;
		}		
		
	}
	catch (...) {
		m_pUpdateFriendList->CloseCommand() ;
		return false ;
	}
	
	return true ;
	
}


bool COLE_DB_Executer::GetOffMailList( char * cName, int * iUID, list<_Mail> * pMailList ) 
{
	
	try{
		
		HRESULT hr;
		
		m_pGetOffMailList->ResetAccessor() ;

		memcpy( m_pGetOffMailList->GetAccessor()->m_chr_name, cName, 13 ) ;		
		pMailList->clear() ;
		
		hr = m_pGetOffMailList->OpenCommand(*m_Session) ;
		m_pGetOffMailList->CloseCommand() ;
		
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) m_pDBCon->CallReConTh() ;
			return false;
		}
		
		if( m_pGetOffMailList->GetAccessor()->m_result > 0 ) {
			//::PrintConsole("[ERROR] if( m_pGetOffMailList->m_result > 0 ) \n") ;
			return false ;
		}
		
		*iUID = m_pGetOffMailList->GetAccessor()->m_iUniqueID ;

		_Mail mail[MAX_MAIL_COUNT] = {0};
		memcpy(mail, m_pGetOffMailList->GetAccessor()->m_mail_list, sizeof(mail)) ;
		
		for(int i = 0; i < MAX_MAIL_COUNT; i++)
		{
			if( mail[i].uiIndex > 0 ) {
				pMailList->push_back(mail[i]) ;
			}		
		}	
		
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_DB_Command::GetMailList \n");
		m_pGetOffMailList->CloseCommand() ;
		return false ;
	}
	
	return true ;
	
}



bool COLE_DB_Executer::UpdateMailList( int iUID, list<_Mail> * pMailList ) 
{
	
	
	try{
		
		
		HRESULT hr;
		
		m_pUpdateMailList->ResetAccessor() ;
		
		m_pUpdateMailList->GetAccessor()->m_iUniqueID = iUID ;
		
		_Mail mail[MAX_MAIL_COUNT] = {0};
		list<_Mail>::iterator	itrMail ;
		
		int i = 0 ;
		for(itrMail = pMailList->begin(); itrMail != pMailList->end(); itrMail++ )
		{		
			mail[i].uiIndex = itrMail->uiIndex ;
			mail[i].check = itrMail->check ;		
			i++ ;
		}
		memcpy( m_pUpdateMailList->GetAccessor()->m_mail_list, mail, sizeof(mail) ) ;


		
		
		hr = m_pUpdateMailList->OpenCommand(*m_Session) ;
		m_pUpdateMailList->CloseCommand() ;
		
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) m_pDBCon->CallReConTh() ;
			return false;
		}
		
		// ???
		if( m_pUpdateMailList->GetAccessor()->m_RETURN_VALUE ){
			::PrintConsole("[ERROR] m_pUpdateMailList->m_RETURN_VALUE \n") ;
			return false ;
		}
		
		
	}
	catch (...) {
		m_pUpdateMailList->CloseCommand() ;
		return false ;
	}
	
	return true ;
	
}


bool COLE_DB_Executer::GetMyData(_FriendInfo * pFriendInfo ) 
{
	list<_Member> * pFriendList = pFriendInfo->GetFriendListPtr() ;
	list<_Mail> * pMailList = pFriendInfo->GetMailListPtr() ;

	int pos = 0;
	try{
		
		HRESULT hr;
		
		m_pMS_Get_MyData->ResetAccessor() ;
		
		memcpy( m_pMS_Get_MyData->GetAccessor()->m_chr_name, pFriendInfo->GetName(), 13 ) ;	
		
		pFriendList->clear() ;
		pMailList->clear() ;		
		
		pos = 1;
		
		hr = m_pMS_Get_MyData->OpenCommand(*m_Session) ;
		m_pMS_Get_MyData->CloseCommand() ;
		
		
		if(FAILED(hr)){ 
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) m_pDBCon->CallReConTh() ;
			return false;
		}
		
		pos = 2;

		if( m_pMS_Get_MyData->GetAccessor()->m_result > 0 ) {
			//::PrintConsole("[ERROR] if( m_pGetOffMailList->m_result > 0 ) \n") ;
			return false ;
		}

		pos = 3;
		// 유니크 아이디 셋팅..
		pFriendInfo->SetUniqueID( m_pMS_Get_MyData->GetAccessor()->m_iUniqueID ) ;
		pFriendInfo->SetGmClass( m_pMS_Get_MyData->GetAccessor()->m_gm ) ;

		// 친구 리스트 파싱..
		_Member member = {0} ;
		int j = 0 ;
		int i = 0 ;
		int diff = 0;
		pos = 4;
		for(i = 0 ; i < 760; i++) {			
			if ( m_pMS_Get_MyData->GetAccessor()->m_friend_list[i] == _GROUP_PARSING_CHAR_ ) {	// 그릅명
				if(i == 0) {
					j = i ;
				}
				else {
					pos = 5;
					if( i-j < 16 )
					{
						memset( &member, 0 , sizeof(_Member) ) ;
						memcpy(member.characName, &m_pMS_Get_MyData->GetAccessor()->m_friend_list[j], i-j) ;
						member.pFriendInfo = NULL ;
	#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_DB, " + %s \n", member.characName) ;
	#endif
						j = i ;
						pos = 6;
						if( member.characName[0] != 0 )
						{
							pFriendList->push_back(member) ;
						}
					}
					else
						j = i;
					
				}			
				
			}
			else if( m_pMS_Get_MyData->GetAccessor()->m_friend_list[i] == _CHARAC_PARSING_CHAR_ ) 
			{	// 캐릭터 이름
				pos = 8;
				if(i == 0) {
					j = i+1 ;
				}
				else {
					pos = 9;
					if( i-j < 16 )
					{
						memset( &member, 0 , sizeof(_Member) ) ;
						memcpy(member.characName, &m_pMS_Get_MyData->GetAccessor()->m_friend_list[j], i-j) ;	// '#'_CHARAC_PASING_CHAR_문자는 빼고 저장한다.
						member.pFriendInfo = NULL ;

						diff = i-j;
						pos = 10;
						j = i+1;
					
						if( member.characName[0] != 0 && member.characName[0] != _CHARAC_PARSING_CHAR_)
						{
							pFriendList->push_back(member) ;
						}
					}
					else
						j = i+1;

				}
				
			}
			else if( m_pMS_Get_MyData->GetAccessor()->m_friend_list[i] == 0 ) 
			{
				pos = 20;
				if( i > 0 ) {
					if( i-j < 16 )
					{
						memset( &member, 0 , sizeof(_Member) ) ;
						memcpy(member.characName, &m_pMS_Get_MyData->GetAccessor()->m_friend_list[j], i-j) ;	
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_DB, " - %s \n", member.characName) ;
#endif
						if( member.characName[0] != 0 )
						{
							pFriendList->push_back(member) ;
						}
					}
				}
				break ;
			}
			
		}
		

		pos = 30;
		// 메일 파싱
		_Mail mail[MAX_MAIL_COUNT] = {0};
		memcpy(mail, m_pMS_Get_MyData->GetAccessor()->m_mail_list, sizeof(mail)) ;
		
		pos = 31;
		for(i = 0; i < MAX_MAIL_COUNT; i++)
		{
			if( mail[i].uiIndex > 0 ) {
				pMailList->push_back(mail[i]) ;
			}		
		}

		return true ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_DB_Command::GetMyData pos(%d) \n", pos );
		m_pMS_Get_MyData->CloseCommand() ;
		return false ;
	}
	
}


bool COLE_DB_Executer::GetMyDataByUID(_FriendInfo * pFriendInfo, const int uid ) 
{
	list<_Member> * pFriendList = pFriendInfo->GetFriendListPtr() ;
	list<_Mail> * pMailList = pFriendInfo->GetMailListPtr() ;

	try{
		
		HRESULT hr;
		
		m_pMS_GetMyDataByUID->ResetAccessor() ;
		
		m_pMS_GetMyDataByUID->GetAccessor()->m_iUniqueID = uid ;	
		
		pFriendList->clear() ;
		pMailList->clear() ;		
		
		
		hr = m_pMS_GetMyDataByUID->OpenCommand(*m_Session) ;
		m_pMS_GetMyDataByUID->CloseCommand() ;
		
		
		if(FAILED(hr)){ 
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) m_pDBCon->CallReConTh() ;
			return false;
		}
		
		if( m_pMS_GetMyDataByUID->GetAccessor()->m_result > 0 ) {
			//::PrintConsole("[ERROR] if( m_pGetOffMailList->m_result > 0 ) \n") ;
			return false ;
		}

		// 유니크 아이디 셋팅..
		pFriendInfo->SetUniqueID( m_pMS_GetMyDataByUID->GetAccessor()->m_iUniqueID ) ;
		pFriendInfo->SetGmClass( m_pMS_GetMyDataByUID->GetAccessor()->m_gm ) ;

		// 친구 리스트 파싱..
		_Member member = {0} ;
		int j = 0 ;
		int i = 0 ;
		for(i = 0 ; i < 760; i++) {			
			if ( m_pMS_GetMyDataByUID->GetAccessor()->m_friend_list[i] == _GROUP_PARSING_CHAR_ ) {	// 그릅명
				if(i == 0) {
					j = i ;
				}
				else {
					if( i-j < 16 )
					{
						memset( &member, 0 , sizeof(_Member) ) ;
						memcpy(member.characName, &m_pMS_GetMyDataByUID->GetAccessor()->m_friend_list[j], i-j) ;
						member.pFriendInfo = NULL ;
	#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_DB, " + %s \n", member.characName) ;
	#endif
						j = i ;
						if( member.characName[0] != 0 )
						{
							pFriendList->push_back(member) ;
						}
					}
					else
						j = i;
					
				}			
				
			}
			else if( m_pMS_GetMyDataByUID->GetAccessor()->m_friend_list[i] == _CHARAC_PARSING_CHAR_ ) {	// 캐릭터 이름
				if(i == 0) {
					j = i+1 ;
				}
				else {		
					if( i-j < 16 )
					{
						memset( &member, 0 , sizeof(_Member) ) ;
						memcpy(member.characName, &m_pMS_GetMyDataByUID->GetAccessor()->m_friend_list[j], i-j) ;	// '#'_CHARAC_PASING_CHAR_문자는 빼고 저장한다.
						member.pFriendInfo = NULL ;
	#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_DB, "    └ %s \n", member.characName) ;
	#endif
						j = i+1 ;

						if( member.characName[0] != 0  && member.characName[0] != _CHARAC_PARSING_CHAR_ )
						{
							pFriendList->push_back(member) ;
						}
					}
					else
						j = i+1;
				}
				
			}
			else if( m_pMS_GetMyDataByUID->GetAccessor()->m_friend_list[i] == 0 ) 
			{
				
				
				if( i > 0 ) {
					if( i-j < 16 )
					{
						memset( &member, 0 , sizeof(_Member) ) ;
						memcpy(member.characName, &m_pMS_GetMyDataByUID->GetAccessor()->m_friend_list[j], i-j) ;	
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_DB, " - %s \n", member.characName) ;
#endif
						if( member.characName[0] != 0 )
						{
							pFriendList->push_back(member) ;
						}
					}

				}
				break ;
				
			}
			
		}
		

		// 메일 파싱
		_Mail mail[MAX_MAIL_COUNT] = {0};
		memcpy(mail, m_pMS_GetMyDataByUID->GetAccessor()->m_mail_list, sizeof(mail)) ;
		
		for(i = 0; i < MAX_MAIL_COUNT; i++)
		{
			if( mail[i].uiIndex > 0 ) {
				pMailList->push_back(mail[i]) ;
			}		
		}

#ifdef _PRINT_OUT
		for( list<_Member>::iterator itr = pFriendList->begin() ; itr != pFriendList->end() ; ++itr )
		{
			printf( "%s \n", itr->characName ) ;
		}
#endif

		
		
		return true ;
		
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_DB_Command::GetMailList \n");
		m_pMS_GetMyDataByUID->CloseCommand() ;
		return false ;
	}
	
}


bool COLE_DB_Executer::UpdateMyData( _FriendInfo * pFriendInfo ) 
{
	try{
		
		list<_Mail> * pMailList = pFriendInfo->GetMailListPtr() ;
		list<_Member> * pFriendList = pFriendInfo->GetFriendListPtr() ;
		list<_Mail>::iterator	itrMail ;
		list<_Member>::iterator memberListIter ;

		_Mail mail[MAX_MAIL_COUNT] = {0};		

		HRESULT hr;
		
		m_pMS_Update_MyData->ResetAccessor() ;
		
		m_pMS_Update_MyData->GetAccessor()->m_iUniqueID = pFriendInfo->GetUniqueID() ;
		

		// 메일 리스트 업데이트
		int i = 0 , j = 0;
		for(itrMail = pMailList->begin(); itrMail != pMailList->end(); itrMail++ )
		{		
			mail[i].uiIndex = itrMail->uiIndex ;
			mail[i].check = itrMail->check ;		
			i++ ;
		}
		memcpy( m_pMS_Update_MyData->GetAccessor()->m_mail_list, mail, sizeof(mail) ) ;
	
		
		
		// 친구 리스트 업데이트
		j = i = 0 ;
		for( memberListIter = pFriendList->begin() ;
		memberListIter != pFriendList->end() ;
		memberListIter++ ) {
			if( memberListIter->characName[0] != 0 )
			{
				if( memberListIter->characName[0] != _GROUP_PARSING_CHAR_ ) 
				{
					m_pMS_Update_MyData->GetAccessor()->m_friend_list[i++] = _CHARAC_PARSING_CHAR_ ;
				}
				
				memberListIter->characName[15] = 0;
				j =  strlen(memberListIter->characName) ;			
				memcpy( &m_pMS_Update_MyData->GetAccessor()->m_friend_list[i], memberListIter->characName, j) ;
				i = i + j ;
			}
			else
			{
				break ;
			}
		}
		//m_pMS_Update_MyData->GetAccessor()->m_friend_list[i] = 0 ;
			
		
		hr = m_pMS_Update_MyData->OpenCommand(*m_Session) ;
		m_pMS_Update_MyData->CloseCommand() ;
		
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) m_pDBCon->CallReConTh() ;
			return false;
		}
		
		// ???
		if( m_pMS_Update_MyData->GetAccessor()->m_RETURN_VALUE ){
			::PrintConsole("[ERROR] m_pUpdateMailList->m_RETURN_VALUE \n") ;
			return false ;
		}
		
		
	}
	catch (...) {
		m_pMS_Update_MyData->CloseCommand() ;
		return false ;
	}
	
	return true ;
}





bool COLE_DB_Executer::GetMail(u_int index, _MailInfo * mailInfo) 
{	

	if( index == 0  )
	{
		return false ;
	}
	
	try{

		HRESULT hr;
		
		m_pGetMailBox->ResetAccessor() ;
		
		m_pGetMailBox->GetAccessor()->m_index = index ;
		


		hr = m_pGetMailBox->OpenCommand(*m_Session) ;
		m_pGetMailBox->CloseCommand() ;


		if(FAILED(hr)){ 
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) m_pDBCon->CallReConTh() ;
			return false;
		}
		
		mailInfo->uiIndex = index ;
		memcpy(mailInfo->cFrom , m_pGetMailBox->GetAccessor()->m_from, 13) ;
		mailInfo->cStrLen = (char)strlen(m_pGetMailBox->GetAccessor()->m_text) ;
		memcpy(mailInfo->cText , m_pGetMailBox->GetAccessor()->m_text, 101 ) ;
		mailInfo->date.year = m_pGetMailBox->GetAccessor()->m_date.year ;
		mailInfo->date.month = m_pGetMailBox->GetAccessor()->m_date.month ;
		mailInfo->date.day = m_pGetMailBox->GetAccessor()->m_date.day ;
		mailInfo->date.hour = m_pGetMailBox->GetAccessor()->m_date.hour ;
		mailInfo->date.minute = m_pGetMailBox->GetAccessor()->m_date.minute ;
		mailInfo->cMode = static_cast<char>(m_pGetMailBox->GetAccessor()->m_mode) ;
		
		return true ;	
	}
	catch (...) {
		m_pGetMailBox->CloseCommand() ;
		return false ;
	}
	
	
	
}


bool COLE_DB_Executer::SendMail(u_int * index,  char * cFromName, char * text, char size, char cMode ) 
{
	
	
	try{


		HRESULT hr;
		
		m_pSendMail->ResetAccessor() ;
				
		memcpy(m_pSendMail->GetAccessor()->m_from, cFromName, 13) ;
		memcpy(m_pSendMail->GetAccessor()->m_text, text, size) ;		
		m_pSendMail->GetAccessor()->m_mode = static_cast<short>(cMode) ;

		hr = m_pSendMail->OpenCommand(*m_Session) ;
		m_pSendMail->CloseCommand() ;

		if(FAILED(hr)){ 
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) m_pDBCon->CallReConTh() ;
			return false;
		}

		*index = m_pSendMail->GetAccessor()->m_index ;	// mail index 를 반환..
		
		return true ;
	}
	catch (...) {
		m_pSendMail->CloseCommand() ;
		return false ;
	}

	

}

bool COLE_DB_Executer::DropMail(u_int index)
{

	try{
		
		
		HRESULT hr;
		
		m_pDropMail->ResetAccessor() ;
		
		m_pDropMail->GetAccessor()->m_index = index ;
			
		
		hr = m_pDropMail->OpenCommand(*m_Session) ;
		m_pDropMail->CloseCommand() ;
		
		
		if(FAILED(hr)){ 
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) m_pDBCon->CallReConTh() ;
			return false;
		}
		
		return true ;
	}
	catch (...) {
		m_pDropMail->CloseCommand() ;
		return false ;
	}

}

bool COLE_DB_Executer::MSMessage(u_int * index, char * cFromName, char cMode ) 
{

	
	try{

		HRESULT hr;
		
		m_pSendMSMessage->ResetAccessor() ;
				
		memcpy(m_pSendMSMessage->GetAccessor()->m_from, cFromName, 13) ;
		m_pSendMSMessage->GetAccessor()->m_mode = cMode ;
		


		hr = m_pSendMSMessage->OpenCommand(*m_Session) ;
		m_pSendMSMessage->CloseCommand() ;


		if(FAILED(hr)){ 
			DBErrorOutput( hr ) ;
			if(hr == E_FAIL) m_pDBCon->CallReConTh() ;
			return false;
		}

		*index = m_pSendMSMessage->GetAccessor()->m_index ;
		
		return true ;


	}
	catch (...) {
		m_pSendMSMessage->CloseCommand() ;
		return false ;
	}	

}

void COLE_DB_Executer::MSDeleteCharac( char * characName )
{
	m_pMS_DeleteCharac->ResetAccessor() ;
}


