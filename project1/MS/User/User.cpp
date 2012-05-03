#include "..\stdafx.h"
#include "..\VMManager\VMManager.h"
#include "..\Map\MAPManager.h"
#include "..\ole-db\db_command.h"
#include "User.h"
#include "..\Global\Trace.h"

extern _j_MemeberQueue * g_pMemberQueue ;
extern _j_BackupReady * g_pBackupReady ; 
extern CMYTrace *		g_pTrace ;




void _FriendInfo::releas()
{
	user_lock() ;
	try{
	
		m_iUniqueID = 0 ;
		memset(m_characName, 0, 13) ;
		m_ds_com = NULL ;
		m_state = 0 ;
		m_option = 0 ;
		m_gm_class = 0 ;
		m_pChattingFriend = NULL ;
		cBackupState = 0 ;
		FriendList.clear() ;
		MailList.clear() ;	
		
		dwLastChatTime = 0 ;
		dwBadChatCount = 0 ;
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;
}

bool _FriendInfo::IsAlreadyFriend(char * name )
{
	bool bReturn = false ;
	
	user_lock() ;
	try{
		int iFriend_Max = 0 ;
		for(itrMember m_itrFriendList = FriendList.begin() ;
		m_itrFriendList != FriendList.end() ;
		m_itrFriendList++) {
			if( iFriend_Max++ >= MAX_MAKE_FRIEND ) break ;
			if(m_itrFriendList->characName[0] == name[0]) {
				if( strcmp(m_itrFriendList->characName, name) == 0 ) {
					
					bReturn = true ;
					break ;
				}
			}
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;

	return bReturn ;
}

bool _FriendInfo::AddFriend( char * name , _FriendInfo * pFriend )
{

	if( name[0] == 0 || name[0] == _CHARAC_PARSING_CHAR_ )
	{
		return false ;
	}

	bool bRet = false ;

	user_lock() ;
	try{
		if( FriendList.size() < MAX_MAKE_FRIEND )
		{
			_Member member={0} ;
			memcpy( member.characName, name , 15 ) ;
			member.characName[15] = 0;
			member.pFriendInfo = pFriend ;
			FriendList.push_back( member ) ;
			bRet = true ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
		bRet = false ;
	}
	
	user_unlock() ;

	return bRet ;
}

bool _FriendInfo::DeleteFriend( char * name )
{

	

	bool bReturn = false ;

	user_lock() ;
	try{
		
		if( FriendList.empty() == FALSE )
		{			
			int iFriend_Max = 0 ;
			for(itrMember m_itrFriendList = FriendList.begin(); 
			m_itrFriendList != FriendList.end(); 
			m_itrFriendList++ ) {
				if( iFriend_Max++ >= MAX_MAKE_FRIEND ) break ;
				if( m_itrFriendList->characName[0] == name[0] ) {
					if( strcmp( m_itrFriendList->characName, name ) == 0 ) {				
						FriendList.erase( m_itrFriendList ) ;	
						bReturn = true ;
						break;
					}
				}
			}
		}		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
		bReturn =false ;
	}	
	user_unlock() ;
	return bReturn ;
}

bool _FriendInfo::MoveFriend(char * name , char * group )
{
		
	bool bReturn = false ;

	_Member member = {0} ;


	user_lock() ;
	try{
		if( FriendList.empty() == false )
		{
			int iFriend_Max = 0 ;
			for(itrMember m_itrFriendList = FriendList.begin(); 
			m_itrFriendList != FriendList.end(); 
			m_itrFriendList++ ) {
				if( iFriend_Max++ >= MAX_MAKE_FRIEND ) break ;
				if( m_itrFriendList->characName[0] == name[0] ) {
					if( strcmp( m_itrFriendList->characName, name ) == 0 ) {
						memcpy( member.characName, m_itrFriendList->characName, 13 ) ;
						member.pFriendInfo = m_itrFriendList->pFriendInfo ;				
						bReturn = true ;
						break;
					}
				}
			}
			
			if( bReturn )
			{
				bReturn = false ;
				
				if( strcmp( "\r친구", group ) == 0 )
				{
					FriendList.erase( m_itrFriendList ) ;			
					FriendList.push_front( member ) ;
					bReturn = true ;
				}
				else
				{
					int iFriend_Max = 0 ;
					for(itrMember itrGroup = FriendList.begin(); 
					itrGroup != FriendList.end(); 
					itrGroup++ ) {
						if( iFriend_Max++ >= MAX_MAKE_FRIEND ) break ;
						if( itrGroup->characName[0] == group[0] ) {
							if( strcmp( itrGroup->characName, group ) == 0 ) {
								FriendList.erase( m_itrFriendList ) ;	
								if( itrGroup == FriendList.end() )
								{
									FriendList.push_back( member ) ;
								}
								else
								{	
									++itrGroup ;
									FriendList.insert( itrGroup, member ) ;
								}
								
								bReturn = true ;
								break;
							}
						}
					}
				}
				
				
			}
		}	
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
		bReturn = false ;
	}	
	user_unlock() ;

	
	return bReturn ;
	
}

bool _FriendInfo::GroupRename(char * oldName, char * newName )
{
	bool bRet = false ;
	user_lock() ;
	try{
		int iFriend_Max = 0 ;
		for(itrMember m_itrFriendList = FriendList.begin(); 
		m_itrFriendList != FriendList.end(); 
		m_itrFriendList++ ) {
			if( iFriend_Max++ >= MAX_MAKE_FRIEND ) break ;
			if( m_itrFriendList->characName[1] == oldName[0] ){
				if( strcmp( &m_itrFriendList->characName[1], oldName ) == 0 ) {
					strcpy( &m_itrFriendList->characName[1], newName ) ;
					
					bRet = true ;
				}
			}
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;
	return bRet ;
}

void _FriendInfo::FriendListConvertBinary(char * cList )
{
	int i = 0 ;

	user_lock() ;
	try{
		int iFriend_Max = 0 ;
		for(itrMember m_itrFriendList = FriendList.begin(); 
		m_itrFriendList != FriendList.end(); 
		m_itrFriendList++ ) {
			if( iFriend_Max++ >= MAX_MAKE_FRIEND ) break ;
			m_itrFriendList->characName[15] = 0;
			i = i + strlen(m_itrFriendList->characName) ;
			memcpy(&cList[i], m_itrFriendList->characName, strlen(m_itrFriendList->characName) ) ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;
}

void _FriendInfo::SetFriendListPakcet(MMSG_FRIEND_LIST_RES * pMsgFriendListRes )
{
	int i = 0, j = 0 ;	

	user_lock() ;

	try{
		int iFriend_Max = 0 ;
		for(itrMember m_itrFriendList = FriendList.begin() ;
		m_itrFriendList != FriendList.end() ;
		m_itrFriendList++ ) {
			if( iFriend_Max++ >= MAX_MAKE_FRIEND ) break ;
			//j =  strlen(m_itrFriendList->characName) ;
			if( m_itrFriendList->characName[0] == 0 )
			{
				continue ;
			}
			memcpy( pMsgFriendListRes->_friends[i].cChoiceName, m_itrFriendList->characName, 15) ;
			
			if( m_itrFriendList->pFriendInfo ) {
				pMsgFriendListRes->_friends[i].cState = m_itrFriendList->pFriendInfo->m_state ;
			}
			else {
				pMsgFriendListRes->_friends[i].cState = 0 ;	// 로그아웃..
			}
			++i ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}
	user_unlock() ;

	// 가변길이 패킷 길이 측정..
	pMsgFriendListRes->cCount = i ;
	pMsgFriendListRes->sLength = sizeof(_sHeader) + 2 + ((sizeof(_Friend) * i)) ;
	pMsgFriendListRes->cMessage = MMSG_NO_FRIEND_LIST_RES_S_C ;
}



void _FriendInfo::SetMailListPacket(MMSG_MAIL_LIST_RES * pMsgMailListRes )
{	
	int i = 0 ;
	user_lock() ;
	try{
		int iMail_Max = 0 ;
		for( itrMail m_itrMailList = MailList.begin() ; m_itrMailList != MailList.end() ; m_itrMailList++ )
		{
			if( iMail_Max++ > MAX_MAIL_COUNT ) break ;
			pMsgMailListRes->mail[i].uiIndex = m_itrMailList->uiIndex ;
			pMsgMailListRes->mail[i].check = m_itrMailList->check ;
			i++ ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;
}


bool _FriendInfo::ReadFromDB( COLE_DB_Executer * pDB )
{
	bool bRes ;
	user_lock() ;	
	try{
		bRes = pDB->GetMyData( this ) ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;
	return bRes ;
}

void _FriendInfo::UpdateDB(COLE_DB_Executer * pDB )
{
	user_lock() ;
	try{
		if( pDB->UpdateMyData( this ) )
		{
			_BackupState.bFrindListBackup = BACKUP_NONE ;		
			_BackupState.bMailListBackup = BACKUP_NONE ;		
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;
}


void _FriendInfo::DeleteCharacter( COLE_DB_Executer * pDB )
{

	MMSG_CUT_FRIEND_REPORT msgCutFriendReport = {0} ;
	msgCutFriendReport.sLength = sizeof(MMSG_CUT_FRIEND_REPORT) ;
	msgCutFriendReport.sCrypto = false ;
	msgCutFriendReport.sCompressed = false ;
	msgCutFriendReport.cMessage = MMSG_NO_CUT_FRIEND_REPORT_S_C ;

	_Mail mail = {0} ;	
	_FriendInfo * pFriendInfo ;
	

	_FriendInfo * arpTempFriendList[MAX_MAKE_FRIEND] = {0} ;
	int iFriend_Max = 0 ;

	user_lock() ;
	try{
		for(itrMember m_itrFriendList = FriendList.begin() ; 
		m_itrFriendList!= FriendList.end() ; 
		m_itrFriendList++ ) 
		{
			if( iFriend_Max >= MAX_MAKE_FRIEND ) break ;
			if( m_itrFriendList->characName[0] == _GROUP_PARSING_CHAR_ ) 
				continue ;			
			
			pFriendInfo = g_pMemberQueue->FindNode( m_itrFriendList->characName ) ;
			if( !pFriendInfo ) {
				// DB에서 읽어온다..						
				pFriendInfo = g_pMemberQueue->GetNode( m_itrFriendList->characName ) ;				
				if( !pFriendInfo ) {
					::PrintConsole("[INFO] !pFriendInfo \n") ;
					continue ;
				}				
				
				bool bRes = pFriendInfo->ReadFromDB( pDB ) ;
				if( bRes == false ) {
					::PrintConsole( "[ERROR] pFriendInfo->ReadFromDB == false (%s)", m_itrFriendList->characName ) ;
					continue ;
				}
				
			}
			
			arpTempFriendList[iFriend_Max] = pFriendInfo ;

			++iFriend_Max ;
		} // for
		pDB->UpdateMyData( this ) ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;

	try{
		for( int i = 0 ; i < MAX_MAKE_FRIEND ; ++i )
		{
			pFriendInfo = arpTempFriendList[i] ;
			if( pFriendInfo )
			{
				if( pFriendInfo->DeleteFriend( m_characName ) )
				{
					pFriendInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
				}			
				
				if( pFriendInfo->IsLogon() ) 
				{
					memcpy( msgCutFriendReport.cCharacName, m_characName, 13 ) ;							
					int iRes = pFriendInfo->SendMsg( (char*)&msgCutFriendReport, msgCutFriendReport.sLength, _SEND_MODE_ME_ ) ;
					if(iRes != 0){
						::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
						::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
						continue ;
					}
				}
				else 
				{
					
					if( pFriendInfo->GetMailCurCount() < MAX_MAIL_COUNT ) {
						
						u_int uiMailIndex = 0 ;							
						
						// 차후에 로그인 했을때 전서구로 알려줘야 한다..
						// 이 곳에 구현하자..
						if ( pDB->MSMessage( &uiMailIndex, m_characName, MSMESSAGE_DEL_FRIEND) )
						{					
							memset(&mail, 0, sizeof(mail)) ;
							mail.check = false ;
							mail.uiIndex = uiMailIndex ;
							
							pFriendInfo->AddMail( mail ) ;
							
							pFriendInfo->_BackupState.bMailListBackup = BACKUP_READY ;
						}								
						
					}							
					
				}
				g_pBackupReady->Insert( pFriendInfo ) ;
			}
			
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}

	
}

void _FriendInfo::Login(_h_DS_Com * ds_com )
{
	user_lock() ;
	try{
		m_ds_com = ds_com ;
		m_state = _CHARAC_STATE_LOGON ;
		ds_com->m_pFriendInfo = this ; // 나의 친구 리스트 구조체 포인터를 recv OV 에 저장..
		// 차후부터는 맵으로 찾을꺼 없이 이걸로 바로 찾아서 쓴다.. ^^		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;
}

void _FriendInfo::Logout()
{
	user_lock() ;
	try{
		m_ds_com = NULL ;
		m_state = _CHARAC_STATE_LOGOFF ;	
		m_pChattingFriend = NULL ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;

	
	try{		
		MMSG_FRIEND_STATE_REPORT msgFriendStateReport = {0} ;
		msgFriendStateReport.sLength = sizeof(MMSG_FRIEND_STATE_REPORT) ;
		msgFriendStateReport.cMessage = MMSG_NO_FRIEND_STATE_REPORT_S_C ;			
		memcpy(msgFriendStateReport.cCharacName, GetName(), 13) ;
		msgFriendStateReport.cState = _CHARAC_STATE_LOGOFF ;			
		SendMsg( (char*)&msgFriendStateReport, msgFriendStateReport.sLength, _SEND_MODE_MY_FRIENDS ) ;		
		g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "[INFO] Log out (%s) \n", m_characName ) ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}
	
}

void _FriendInfo::DisconnectForLogout()
{ 
	user_lock() ;
	try{
		if( m_ds_com ) 
		{
			m_ds_com->m_pFriendInfo = NULL ;
			m_ds_com->DisConnect() ;
			
			m_ds_com = NULL ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;
}


bool _FriendInfo::AddMail(_Mail &mail )
{
	
	bool bRet = false ;

	user_lock() ;
	try{
		if( FriendList.size() < MAX_MAIL_COUNT ) 
		{
			MailList.push_back( mail ) ;
			bRet = true ;
		}		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
		bRet = false ;
	}	
	user_unlock() ;

	return bRet ;
}

bool _FriendInfo::DeleteMail(UINT uiIndex )
{

	bool bReturn = false ;

	user_lock() ;
	try{

		if( FriendList.empty() == false )
		{
			int iMail_Max = 0 ;
			for(itrMail m_itrMailList = MailList.begin() ;
			m_itrMailList != MailList.end() ;
			m_itrMailList++ )
			{
				if( iMail_Max++ > MAX_MAIL_COUNT ) break ;
				if( m_itrMailList->uiIndex == uiIndex )
				{				
					MailList.erase( m_itrMailList ) ;
					
					bReturn = true ;
					break ;
				}
			}
		}
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;
	return bReturn ;
}

int _FriendInfo::SendMsg(char * buf, int size, char mode )
{
	if( buf == NULL || size < 2 )
	{
		::PrintConsole( "[SO_ERR] if( buf == NULL || size(%d) < 2 ) \n", size ) ;
		::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
		return -1 ;
	}


	int iError = 0 ;
	if( mode == _SEND_MODE_ME_ )
	{
		user_lock() ;
		try{
			if ( m_ds_com && m_ds_com->m_pNetIO ) {
				iError = m_ds_com->m_pNetIO->SendRequest( size, buf) ;
				if( iError != 0 )
				{
					::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iError ) ;
					::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
					//m_ds_com->DisConnect() ;
				}			
			}
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
		}		
		user_unlock() ;
	}
	else
	{
		
		int pos = 0 ;
		int iFriend_Max = 0 ;
		_FriendInfo * arpTempFriendList[MAX_MAKE_FRIEND] = {0} ;			
		user_lock() ;
		try{
			if( !FriendList.empty() )
			{
				pos = 1 ;				
				for( itrMember m_itrFriendList = FriendList.begin() ; 
				m_itrFriendList != FriendList.end()  ;
				m_itrFriendList++ ) 
				{
					if( iFriend_Max >= MAX_MAKE_FRIEND ) break ;
					pos = 2 ;
					if( m_itrFriendList->pFriendInfo 
						&& m_itrFriendList->pFriendInfo->m_ds_com 
						&& m_itrFriendList->pFriendInfo->m_ds_com->m_pNetIO ) 
					{
						arpTempFriendList[iFriend_Max] = m_itrFriendList->pFriendInfo ;

						/*
						pos = 3 ;
						if( m_itrFriendList->pFriendInfo->SendMsg( buf, size, _SEND_MODE_ME_ ) != 0 )
						{
							pos = 4 ;
							iError = -1 ;
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iError ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
						}
						*/
					}
					++iFriend_Max ;
				}				
			}		
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] %s %d pos :%d \n", __FILE__, __LINE__ , pos) ;
		}		
		user_unlock() ;	


		// 가져온 리스트에 send
		for( int i = 0 ; i < MAX_MAKE_FRIEND ; ++i )
		{
			pos = 3 ;
			if( arpTempFriendList[i] )
			{
				if( arpTempFriendList[i]->SendMsg( buf, size, _SEND_MODE_ME_ ) != 0 )
				{
					pos = 4 ;
					iError = -1 ;
					::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iError ) ;
					::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
				}
			}					
		}

	}

	return  iError ;
	
}

void _FriendInfo::AllFriendsLink()
{

	_FriendInfo * pFriendInfo ;
	itrMember FriendsListIter ;
	
	_FriendInfo * arpTempFriendList[MAX_MAKE_FRIEND] = {0} ;

	user_lock() ;

	try{	
		int iFriend_Max = 0 ;
		for( itrMember m_itrFriendList = FriendList.begin(); 
		m_itrFriendList != FriendList.end()  ;
		m_itrFriendList++ ) 
		{
			if( iFriend_Max >= MAX_MAKE_FRIEND ) break ;
			if( m_itrFriendList->characName[0] != _GROUP_PARSING_CHAR_ ) {	// 캐릭터 이름..
				
				// 친구의 오브젝트에 가서 정보를 갱신..			
				pFriendInfo	= g_pMemberQueue->FindNode(m_itrFriendList->characName) ;
				if( pFriendInfo ) 
				{
					if( pFriendInfo->IsLogon() ) 
					{
						arpTempFriendList[iFriend_Max] = pFriendInfo ;
						// 내쪽 리스트에 친구 구조체의 포인터를 등록시킴
						m_itrFriendList->pFriendInfo = pFriendInfo ;
					}
					else
					{
						m_itrFriendList->pFriendInfo = NULL ;
					}
				}
				else {	// 로그오프중일 경우..				
					m_itrFriendList->pFriendInfo = NULL ;
				}
			}			
			++iFriend_Max ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	

	user_unlock() ;

	// 로그인한 친구들을 찾아 정보를 갱신한다..	
	for( int i = 0 ; i < MAX_MAKE_FRIEND ; ++i )
	{
		if( pFriendInfo = arpTempFriendList[i] )
		{
			if( pFriendInfo->IsLogon() ) 
			{
				// 친구의 리스트에 나를 연결시킴..
				pFriendInfo->FriendLink( this ) ;
			}
		}
	}

}

void _FriendInfo::FriendLink(_FriendInfo * pFriend )
{
	char * name = pFriend->GetName() ;

	user_lock() ;

	try{
		int iFriend_Max = 0 ;
		// 친구쪽 리스트에 내 구조체 포인터를 등록시킴
		for( itrMember m_itrFriendList = FriendList.begin() ;
		m_itrFriendList != FriendList.end() ;
		m_itrFriendList++ ) {
			if( iFriend_Max++ >= MAX_MAKE_FRIEND ) break ;
			if( m_itrFriendList->characName[0] == name[0] ) {
				if( strcmp(m_itrFriendList->characName, name) == 0 ) {
					m_itrFriendList->pFriendInfo = pFriend ;
					break ;
				}
			}
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}
	
	user_unlock() ;
}


void _FriendInfo::SetMyChatFriend(char * name )
{

	if( name == NULL )
	{
		m_pChattingFriend = NULL ;
		return ;
	}

	if( m_pChattingFriend )
	{
		char * characName = m_pChattingFriend->GetName() ;
		if( characName[0] == name[0] )
		{		
			if( strcmp( characName, name ) == 0  )
			{
				return ;
			}
		}
	}
	


	user_lock() ;

	try{
		int iFriend_Max = 0 ;
		for( itrMember m_itrFriendList = FriendList.begin() ; 
		m_itrFriendList != FriendList.end(); 
		m_itrFriendList++)
		{
			if( iFriend_Max++ >= MAX_MAKE_FRIEND ) break ;
			if( m_itrFriendList->characName[0] == name[0] ) {
				if( strcmp(m_itrFriendList->characName, name) ==0 ) {
					if( m_itrFriendList->pFriendInfo ) {															
						m_pChattingFriend = (_FriendInfo*)m_itrFriendList->pFriendInfo ;
						break;
					}
				}
			}
			
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}
	
	user_unlock() ;
}

int _FriendInfo::GetGroupCurCount()
{
	int iRes = 0 ;
	
	if( FriendList.size() == 0 )
	{
		return 0 ;
	}

	user_lock() ;
	try{
		int iFriend_Max = 0 ;
		// 그릅은 8개 까지 생성 가능하다.. 생성된 그릅 개수 카운트 검사..				
		for( itrMember m_itrFriendList = FriendList.begin() ;
		m_itrFriendList != FriendList.end() ;
		m_itrFriendList++ ) {
			if( iFriend_Max++ >= MAX_MAKE_FRIEND ) break ;
			if( m_itrFriendList->characName[0] == _GROUP_PARSING_CHAR_ ) {
				++iRes ;						
			}
		}
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}
	
	user_unlock() ;

	
	return iRes ;
}


int _FriendInfo::GetMemberCount()
{
	int iRes = 0 ;
	
	user_lock() ;
	try{
		int iFriend_Max = 0 ;
		// 생성된 맴버 개수 카운트 검사..				
		for( itrMember m_itrFriendList = FriendList.begin() ;
		m_itrFriendList != FriendList.end() ;
		m_itrFriendList++ ) {
			if( iFriend_Max++ >= MAX_MAKE_FRIEND ) break ;
			if( m_itrFriendList->characName[0] != _GROUP_PARSING_CHAR_ && m_itrFriendList->characName[0] != 0 ) {
				++iRes ;						
			}
		}
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}
	
	user_unlock() ;
	
	
	return iRes ;
}

void _FriendInfo::MailDrop( u_int uiIndex, COLE_DB_Executer * pDB  )
{

	user_lock() ;
	try{
		int iMail_Max = 0 ;
		for( itrMail m_itrMailList = MailList.begin() ; m_itrMailList != MailList.end() ; m_itrMailList++ )
		{
			if( iMail_Max++ > MAX_MAIL_COUNT ) break ;
			if( m_itrMailList->uiIndex == uiIndex )
			{
				MailList.erase( m_itrMailList ) ;
				pDB->DropMail( uiIndex ) ;			
				
				break;
			}
		}		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s %d \n", __FILE__, __LINE__ ) ;
	}	
	user_unlock() ;
	
}