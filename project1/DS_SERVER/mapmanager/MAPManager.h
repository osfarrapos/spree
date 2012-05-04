#pragma once

//#pragma warning(disable:4786)

#include <MY_UTIL.H>

#include <string>
#include <queue>
#include <map>
using namespace std;



class _j_AccMAPManager
{
///////////////////////// account/////////////////////////////////////////
private:	
	map<std::string, PUSER> m_UserMap;
	queue<PUSER>		m_UserQueue ;
	CUser* m_pUser;
		

	CTraceAbleLock	user_map_cs ;
#define user_map_lock()		user_map_cs.tlock( __FILE__, __LINE__ ) ;
#define user_map_unlock()	user_map_cs.tunlock( __FILE__, __LINE__ ) ;


///////////////////////// Charac/////////////////////////////////////////
private:	
	map<std::string, PCHARAC> m_CharacMap;	
	PCHARAC m_pCharac;



	CTraceAbleLock	charac_map_cs ;
#define charac_map_lock()		charac_map_cs.tlock( __FILE__, __LINE__ ) ;
#define charac_map_unlock()		charac_map_cs.tunlock( __FILE__, __LINE__ ) ;
    

public:

	///
	_j_AccMAPManager( size_t size )
	{
		try{	

			m_pUser = NULL;
			m_pCharac = NULL ;

			m_UserMap.clear() ;
			m_CharacMap.clear() ;


			
			CUser * pUser  = m_pUser = new CUser[size] ;
			assert( m_pUser ) ;
			

			for(u_int i = 0; i < size; i++ ){
				//m_pArUser[i] = m_pUser+i;        
				m_UserQueue.push( pUser ) ;
				++pUser ;
			}
		}
		catch(...)
		{			
			throw "" ;
		}	
		
	}
	~_j_AccMAPManager()
	{
		delete [] m_pUser ;
		
	}

	inline size_t GetUserMapSize() { return m_UserMap.size(); }

	//bool Create(_h_VMManager * const pVM, const size_t size, const size_t count );	//+
	void Insert(PUSER pUser);
	bool Erase(PUSER pUser);
	PUSER Find_User(char * szIndex);	
	PUSER GetWritePtr();						//+
	void PutMemPtr( PUSER pUser ) ;

	PUSER MoveBeginUser();
	PUSER MoveNextUser(PUSER pUser);
	//bool IfEndUser( PUSER  pUser );

 //CHARAC///////////////////////////////////////////////////////////////////////
	

public:	

	inline size_t GetCharacdMapSize() { return m_CharacMap.size() ; }

	void CharacInsert(PCHARAC pCharac);
	bool CharacErase(PCHARAC pCharac) ;
	PCHARAC Find_Charac(char * szIndex);		
	PCHARAC MoveBeginCharac();
	PCHARAC MoveNextCharac(PCHARAC);
	//bool IfEndCharac( PCHARAC );
	bool CharacRename( char * oldName, char * newName ) ;

};

class _j_BackupReady
{
private : 	

	vector<PCHARAC>	 *	m_pCharacReady ;
	vector<PUSER> *		m_pAccReady ;


	CTraceAbleLock m_csABackup;	// 계정 백업
	CTraceAbleLock m_csCBackup;	// 캐릭터 백업

#define ABackup_lock()  m_csABackup.tlock( __FILE__, __LINE__ ) ;
#define ABackup_unlock() m_csABackup.tunlock( __FILE__, __LINE__ ) ;
#define CBackup_lock()  m_csCBackup.tlock( __FILE__, __LINE__ ) ;
#define CBackup_unlock() m_csCBackup.tunlock( __FILE__, __LINE__ ) ;

	
public:	

	_j_BackupReady() 
	{

		m_pCharacReady = new vector<PCHARAC> ;
		m_pAccReady = new vector<PUSER> ;
	};

	~_j_BackupReady() {

		delete m_pCharacReady ;
		delete m_pAccReady ;
		
	};

	inline size_t GetCharacReadySize() { return m_pCharacReady->size() ; }
	inline size_t GetUserReadySize() { return m_pAccReady->size() ; }
	

	bool init() {	// 초기화
		m_pCharacReady->reserve( _PERMISSIBLE_MAX_USER ) ;
		m_pCharacReady->clear() ;
		m_pAccReady->reserve( _PERMISSIBLE_MAX_USER  ) ;
		m_pAccReady->clear() ;
		return true ;
	} ;


	// 캐릭턱 관련
	inline void GetCharacBackupNode( vector<PCHARAC> * vpCharacReady )
	{
		CBackup_lock() ;
		TRY{
			*vpCharacReady = *m_pCharacReady ;
			m_pCharacReady->clear() ;		
		}
		CATCH_ALL
		{
			::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
		}		
		CBackup_unlock() ;
	}
	
	inline void InsertCharac(PCHARAC pcharac) {		// 캐릭터 포인터를 백업 대기에 삽입
		CBackup_lock() ;
		TRY{
			m_pCharacReady->push_back( pcharac ) ;
		}
		CATCH_ALL
		{
			::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
		}		
		CBackup_unlock() ;
	}

	inline void InitCharac() {	// 백업 대기 초기화		
		CBackup_lock() ;
		TRY{
			m_pCharacReady->clear() ;
		}
		CATCH_ALL
		{
			::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
		}
		CBackup_unlock() ;
	} 
	
	inline bool IsCharacEmpty(){
		bool bRet ;
		CBackup_lock() ;
		TRY{
			bRet = m_pCharacReady->empty() ;
		}
		CATCH_ALL
		{
			::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
		}
		CBackup_unlock() ;
		return  bRet ;
	} 


	// 계정관련	
	inline void GetUserBackupNode( vector<PUSER> * vpAccReady )
	{		
		ABackup_lock() ;
		TRY{
			*vpAccReady = *m_pAccReady ;
			m_pAccReady->clear() ;
		}
		CATCH_ALL
		{
			::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
		}
		ABackup_unlock() ;
	}


	inline void InsertAcc(PUSER puser) 
	{
		ABackup_lock() ;
		TRY{
			m_pAccReady->push_back( puser ) ;
		}
		CATCH_ALL
		{
			::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
		}
		ABackup_unlock() ;
	}

	inline void InitAcc() 
	{	
		ABackup_lock() ;
		TRY{
			m_pAccReady->clear() ;
		}
		CATCH_ALL
		{
			::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
		}		
		ABackup_unlock() ;
	}

	inline bool IsAccEmpty() 
	{
		bool bRet ;
		ABackup_lock() ;
		TRY{
			bRet = m_pAccReady->empty() ; 
		}
		CATCH_ALL
		{
			::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__) ;
		}
		ABackup_unlock() ;

		return bRet ;
		
	}	

} ;


struct _sMemSave_timerentry {
	PUSER			pUser ;
	int				iID ;
	DWORD			dwTime ;	// 변수명 바뀌면 라이브러리에서 오류남..
	bool operator<(const _sMemSave_timerentry& x) const { return dwTime >= x.dwTime ; } 
};


class CUserMemSaveTimer : public CEventTimer<_sMemSave_timerentry>
{

private:
	_j_AccMAPManager * m_pAccMapManager ;
	
public:
	CUserMemSaveTimer(_j_AccMAPManager * p) : m_pAccMapManager(p)
	{
		if( CEventTimer<_sMemSave_timerentry>::CNDThread::Start() == false )
		{
			throw "" ;
		}
	}

	~CUserMemSaveTimer()
	{
	}

	virtual int Run( void ) ;

	static DWORD get_time_by_sec() { return GetTime_ByIntSec() ; } ;

	inline void Reserve_save( PUSER pUser, int id, DWORD evenTime )
	{
		_sMemSave_timerentry	entry = {0} ;
		entry.pUser = pUser ;
		entry.iID = id ;
		entry.dwTime = evenTime ;

		CEventTimer<_sMemSave_timerentry>::SendMessage( entry ) ;
	}
};

