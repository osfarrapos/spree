#pragma once

#pragma warning(disable:4786)

//#define _PERMISSIBLE_MAX_USER				12000	// DS_SERVER 에서 허용하는 최대 유저다..(최대 캐릭이랑 다름)


#include "..\Global\Global_Define.h"
#include "..\Global\Global_Lock.h"
#include "..\Network\Network.h"
#include "..\Global\MSPacket_Msg.h"
#include <string>
#include <queue>
#include <map>
#include <list>
using namespace std;

class CTraceAbleLock ;

/*
#include <MY_QUEUE.h>
typedef CRationalMem<CTraceAbleLock> TLock ;
*/


struct _Member ;
struct _Mail ;

enum{
	_SEND_MODE_ME_,
	_SEND_MODE_MY_FRIENDS		
} ;


class COLE_DB_Executer ;
class _h_DS_Com ;
struct MMSG_FRIEND_LIST_RES ;
struct MMSG_MAIL_LIST_RES ;

class _FriendInfo
{

public:		
	
	_FriendInfo() //: m_pLock(new CTraceAbleLock)
	{
		
		//m_pLock = new TLock ;
		/*
		assert( m_pLock ) ;
		if( m_pLock == NULL )
		{
			throw ;
		}
		*/
		/*
		if( InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 ) == false )
		{
			throw ;
		}
		*/

		releas() ;
	}
	~_FriendInfo() {				
		//delete m_pLock ;
		/*
		DeleteCriticalSection( &cs ) ; 
		*/
	} 
	
protected :
	
	int			m_iUniqueID ;
	char		m_characName[13] ;		// 캐릭터 이름 또는 그룹 이름으로 쓰인다.	
	_h_DS_Com *	m_ds_com;				// NULL 이면 로그오프중			
	char		m_state ;				// 캐릭터 상태..
	char		m_option ;				// 친구 맺기 신청 거부, 귓말 거부.. 등..
	// 0 . 정상.
	// 1 . 친구 맺기 거부
	// 2 . 귓말 거부..
	
	char		m_gm_class ;			// gm 여부 확인
	//	int			Chat_Count ;		// Chat 명령이 보내질 
	//	_h_DS_Com * Chat_Com[50] ;
	
	// Chatting Character Info(2004.03.10 Add ozzywow)	
	_FriendInfo *	m_pChattingFriend ;
	
	list<_Member> FriendList ;
	list<_Mail>	  MailList ;
	
	typedef list<_Member>::iterator itrMember ;
	typedef list<_Mail>::iterator itrMail ;

	
	//CRITICAL_SECTION	cs ;
	CTraceAbleLock  m_Lock ;
	

#define user_lock()		m_Lock.tlock(__FILE__, __LINE__)
#define user_unlock()	m_Lock.tunlock(__FILE__, __LINE__)
	
	//void lock() { EnterCriticalSection(&cs) ; } ;
	//void unlock() { LeaveCriticalSection(&cs) ; } ;	
	
public:

	// 채팅 매크로 막자!
	DWORD		dwLastChatTime ;
	DWORD		dwBadChatCount ;

	union{
		char	cBackupState ;
		struct  {
			bool		bFrindListBackup : 1 ;		// 0. None, // 1. Backup Ready
			bool		bMailListBackup : 1 ;		// 0. None, // 1. Backup Ready
			bool		bBackup1 :1 ;
			bool		bBackup2 :1 ;
			bool		bBackup3 :1 ;
			bool		bBackup4 :1 ;
			bool		bBackup5 :1 ;
			bool		bBackup6 :1 ;
		} _BackupState ;
	} ;
	
	inline int	GetUniqueID() { return m_iUniqueID ; } 
	inline void SetUniqueID( int iUID ) { m_iUniqueID = iUID ; } 

	bool IsAlreadyFriend( char * name ) ;
	bool AddFriend( char * name , _FriendInfo * pFriend ) ;
	bool DeleteFriend( char * name ) ;
	bool MoveFriend( char * name , char * group ) ;
	bool GroupRename( char * oldName, char * newName ) ;
	bool AddMail( _Mail &mail ) ;
	bool DeleteMail( UINT uiIndex ) ;
	void FriendListConvertBinary( char * cList ) ;
	void SetFriendListPakcet( MMSG_FRIEND_LIST_RES * pMsgFriendListRes ) ;
	void SetMailListPacket( MMSG_MAIL_LIST_RES * pMsgMailListRes ) ;
	bool ReadFromDB( COLE_DB_Executer * pDB ) ;
	void UpdateDB( COLE_DB_Executer * pDB ) ;
	void DeleteCharacter( COLE_DB_Executer * pDB ) ;

	void Login( _h_DS_Com * ds_com ) ;	
	void Logout() ;	
	void DisconnectForLogout() ;	
	inline list<_Member> * GetFriendListPtr() { return &FriendList ; } 
	inline list<_Mail> * GetMailListPtr() { return &MailList ; } 
	inline char * GetName() { return m_characName ; }
	inline void SetName( char * name ) { memcpy( m_characName, name, 13 ) ; }
	inline void SetState( char state ) { m_state = state ;}
	inline char GetState( ) { return m_state ; }
	inline void SetOption( char option ) { m_option = option ; }
	inline char GetOption( ) { return m_option ; }
	inline bool IsLogon() { if( m_ds_com ) return true ; else return false ; }
	int	SendMsg( char * buf, int size, char mode = _SEND_MODE_ME_ ) ;
	void AllFriendsLink() ;
	void FriendLink( _FriendInfo * pFriend ) ;
	
	void SetMyChatFriend( char * name ) ;	
	inline _FriendInfo * GetMyChatFriend() { return m_pChattingFriend ; }	
	inline size_t GetFriendCurCount() 
	{ 
		size_t size ;
		user_lock() ;
		try{
			size = FriendList.size() ; 
		}
		catch (...) {
		}		
		user_unlock() ;
		return size ;
	}
	inline size_t GetMailCurCount() 
	{ 
		size_t size ;
		user_lock() ;
		try{
			size = MailList.size() ; 
		}
		catch (...) {
		}		
		user_unlock() ;
		return size ;
	}
	int GetGroupCurCount() ;
	int GetMemberCount() ;
	void MailDrop( 	u_int uiIndex, COLE_DB_Executer * pDB ) ;
	inline void SetGmClass( char gm ) { m_gm_class = gm ; } ;
	inline char GetGmClass() { return m_gm_class ; } ;
	inline bool IsGm() 
	{ 
		if( m_gm_class > 0 ) 
			return true ;
		else
			return false ;
	}
	

	void releas() ;
};

