#pragma	   once


#include "Accessors.h"


class COLE_DB_Executer
{

private:

	CDB_Connector * const m_pDBCon ;
	CDB_Session * const  m_pDBSession ;	
	const CSession *	const	m_Session ;
		
	auto_ptr<C_prMs_UpdateFriendList>	m_pUpdateFriendList ;	
	auto_ptr<C_prMs_GetOffFriendList>	m_pGetOffFriendList ;	
	auto_ptr<C_prMs_GetUID>				m_pGetUID ;
	auto_ptr<C_prMs_GetOffMailList>		m_pGetOffMailList ;
	auto_ptr<C_prMs_MailBox>			m_pGetMailBox ;

	auto_ptr<C_prMs_SendMail>			m_pSendMail ;
	auto_ptr<C_prMs_DropMail>			m_pDropMail ;
	auto_ptr<C_prMs_UpdateMailList>		m_pUpdateMailList ;
	auto_ptr<C_prMs_SendMSMessage>		m_pSendMSMessage ;
	auto_ptr<C_prMs_DeleteCharac>		m_pMS_DeleteCharac ;

	auto_ptr<C_prMS_GetFriendList_N_MailList>		m_pMS_Get_MyData ;
	auto_ptr<C_prMS_UpdateFriendList_N_MailList>	m_pMS_Update_MyData ;

	auto_ptr<C_prMS_GetFriendList_N_MailList_by_UID> m_pMS_GetMyDataByUID ;

	CRITICAL_SECTION	g_csDB ;

	inline void lock() { EnterCriticalSection( &g_csDB ) ; }
	inline void unlock() { LeaveCriticalSection( &g_csDB ) ; }


public:	

	//bool bThreadRun;	
	

	COLE_DB_Executer(CDB_Connector * pDBCon) : 
		m_pDBCon(pDBCon), 
		m_pDBSession( pDBCon->NewSession() ),
		m_Session(m_pDBSession->GetSessionPtr()),
		m_pMS_Get_MyData(new C_prMS_GetFriendList_N_MailList),
		m_pMS_Update_MyData(new C_prMS_UpdateFriendList_N_MailList),
		m_pUpdateFriendList(new C_prMs_UpdateFriendList),
		m_pGetOffFriendList(new C_prMs_GetOffFriendList),
		m_pGetOffMailList(new C_prMs_GetOffMailList),		
		m_pUpdateMailList(new C_prMs_UpdateMailList),
		m_pGetUID(new C_prMs_GetUID),
		m_pGetMailBox(new C_prMs_MailBox),
		m_pSendMail(new C_prMs_SendMail),
		m_pDropMail(new C_prMs_DropMail),
		m_pSendMSMessage(new C_prMs_SendMSMessage),
		m_pMS_DeleteCharac(new C_prMs_DeleteCharac),
		m_pMS_GetMyDataByUID(new C_prMS_GetFriendList_N_MailList_by_UID)
	{
		// Initailze CriticalSection..
		if( InitializeCriticalSectionAndSpinCount( &g_csDB, (0x80000000)|2000 )  == FALSE ) {
			printf( " g_csDB Error Critical Section\n" ) ;
			throw ;
		}	
	}

	~COLE_DB_Executer()
	{
		m_pDBCon->FreeSession( m_pDBSession ) ;
		DeleteCriticalSection( &g_csDB ) ;	
	}
	
	bool GetMyData( _FriendInfo * pFriendInfo ) ;
	bool GetMyDataByUID( _FriendInfo * pFriendInfo, const int uid ) ;
	bool UpdateMyData( _FriendInfo * pFriendInfo ) ;
	
	//bool GetOffFriendList(char * cName, int * iUID, list<_Member> * pList) ;
	bool UpdateFriendList(int iUID, list<_Member> * pList) ;
	bool GetOffMailList(char * cName, int * iUID, list<_Mail> * pMailList) ;
	bool UpdateMailList(int iUID, list<_Mail> * pMailList ) ;
	

	int GetUID(char * cName) ;
	
	bool GetMail(u_int index, _MailInfo * mailInfo) ;	
	bool SendMail(u_int * index, char * cFromName, char * text, char size, char cMode ) ;
	bool DropMail(u_int index) ;
	bool MSMessage(u_int * index, char * cFromName, char cMode ) ;
	void MSDeleteCharac(char * characName ) ;
	

	

protected:
private:
};


