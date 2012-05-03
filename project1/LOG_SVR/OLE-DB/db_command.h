#pragma	   once
#include "../MyLibrary/MY_OLE_DB.H"
#include "..\Global\Global_Log_Define.h"

class C_Insert_ServerStateAccessor
{
public:
	
	
	LONG m_RETURN_VALUE;
	//TCHAR m_server_name[21];
	//SHORT m_state;	
	_ServerState m_ServerState ;
		
	
#ifdef _LOG_WRITE_TIME
	DEFINE_COMMAND(C_Insert_ServerStateAccessor, "{ ? = CALL dbo.pr_EOD_LOG_ServerState(?,?,?) }")
#else
	DEFINE_COMMAND(C_Insert_ServerStateAccessor, "{ ? = CALL dbo.pr_EOD_LOG_ServerState(?,?) }")
#endif
		
		
	BEGIN_PARAM_MAP(C_Insert_ServerStateAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_ServerState.m_server_name)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_ServerState.m_state)
#ifdef _LOG_WRITE_TIME
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_ServerState.m_TimeStamp)
#endif
	END_PARAM_MAP()
};


class C_Insert_ServerState : public CCommand<CAccessor<C_Insert_ServerStateAccessor>, CRowset >
{

public:
	HRESULT		hr ;
	CSession	m_session ;

	C_Insert_ServerState ()
	{
#if (_WIN32_WINNT > 0x0403)
		if( InitializeCriticalSectionAndSpinCount( &m_cs, (0xf0000000)|2000 ) == FALSE ) {			
			throw ;
		}
#else	
		InitializeCriticalSection( &m_cs ) ;
#endif
	}
	~C_Insert_ServerState()
	{
		DeleteCriticalSection( &m_cs ) ;
	}

private:
	CRITICAL_SECTION m_cs ;
	void lock() { EnterCriticalSection( &m_cs ) ; }
	void unlock() { LeaveCriticalSection( &m_cs ) ; }

public:


	
	
	HRESULT SessionLink( CDB_Connector * pDB_Con ) 
	{
		hr = pDB_Con->SessionLink( &m_session ) ;

		if(FAILED(hr))
			DBErrorOutput( hr ) ;

		return hr ;
	}
	
	HRESULT OpenRowset()
	{
		hr = Open(m_session, NULL, NULL);
		return hr;
	}

	bool Execute( _ServerState * pNode )
	{		

		bool bRet = true ;

		lock() ;
		// Data param bind
		memcpy( &m_ServerState, pNode, sizeof(_ServerState) ) ;		

		// Execute
		hr = OpenRowset() ;	

		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			bRet = false ;
		}
		unlock() ;
		

		return bRet ;
	}
	
};


// Item
class C_Insert_ItemAccessor
{
public:
	
	
	LONG m_RETURN_VALUE;
	//TCHAR m_server_name[21];
	//SHORT m_state;	
	_ItemLog m_ItemLog ;
	
	
	DEFINE_COMMAND(C_Insert_ItemAccessor, "{ ? = CALL dbo.pr_EOD_LOG_ITEM (?,?,?,?,?,?,?,?,?,?,?) }")
				
		
	BEGIN_PARAM_MAP(C_Insert_ItemAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_ItemLog.item_uid )
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_ItemLog.code)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_ItemLog.zone)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(5, m_ItemLog.from_uid)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(6, m_ItemLog.from)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(7, m_ItemLog.to_uid)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(8, m_ItemLog.to)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(9, m_ItemLog.money)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(10, m_ItemLog.type1)
		SET_PARAM_TYPE(DBPARAMIO_INPUT) 
		COLUMN_ENTRY(11, m_ItemLog.type2)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(12, m_ItemLog.moreInfo)
	END_PARAM_MAP()
};




class C_Insert_Item : public CCommand<CAccessor<C_Insert_ItemAccessor>, CRowset >
{
public:
	HRESULT		hr ;
	CSession	m_session ;
	SYSTEMTIME	time ;	
	TCHAR		qry[256] ;
	struct _strTime {
		char	y[5] ;
		char	m[3] ;
		char	d[3] ;
	} strTime;
	
	void ReflushQry()
	{
		GetLocalTime( &time ) ;
		sprintf( strTime.y, "%d", time.wYear ) ;
		
		if( time.wMonth < 10 )
			sprintf( strTime.m, "0%d", time.wMonth ) ;
		else
			sprintf( strTime.m, "%d", time.wMonth ) ;
		
		if( time.wDay < 10 )
			sprintf( strTime.d, "0%d", time.wDay ) ;		
		else
			sprintf( strTime.d, "%d", time.wDay ) ;		
		
		sprintf( qry, "{ ? = CALL dbo.pr_EOD_LOG_ITEM_%s%s%s (?,?,?,?,?,?,?,?,?,?,?) }", strTime.y , strTime.m , strTime.d ) ;
	}
	

	C_Insert_Item ()
	{
#if (_WIN32_WINNT > 0x0403)
		if( InitializeCriticalSectionAndSpinCount( &m_cs, (0xf0000000)|2000 ) == FALSE ) {			
			throw ;
		}
#else	
		InitializeCriticalSection( &m_cs ) ;
#endif
		}

	~C_Insert_Item()
	{
		DeleteCriticalSection( &m_cs ) ;
	}

private:
	CRITICAL_SECTION m_cs ;
	void lock() { EnterCriticalSection( &m_cs )  ; }
	void unlock() { LeaveCriticalSection( &m_cs ) ; }
	
public:

	
	HRESULT SessionLink( CDB_Connector * pDB_Con ) 
	{
		hr = pDB_Con->SessionLink( &m_session ) ;

		if(FAILED(hr))
			DBErrorOutput( hr ) ;


		return hr ;
	}
	
	HRESULT OpenRowset()
	{
#ifdef _NEO_LOG
		ReflushQry() ;
		hr = Open(m_session, qry, NULL);
#else
		hr = Open(m_session, NULL, NULL);
#endif
		return hr;
	}
	
	bool Execute( _ItemLog * pNode )
	{
		bool bRet = true ;
		lock() ;
		// Data param bind
		memcpy( &m_ItemLog, pNode, sizeof(_ItemLog) ) ;		
				
		// Execute
		hr = OpenRowset() ;	
		
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			bRet = false ;
		}		
		unlock() ;

		return bRet ;
	}
	
};


// Character
class C_Insert_CharacAccessor
{
public:
	
	
	LONG m_RETURN_VALUE;	
	_CharacLog m_CharacLog ;
	
	
	DEFINE_COMMAND(C_Insert_CharacAccessor, "{ ? = CALL dbo.pr_EOD_LOG_CHARAC (?,?,?,?,?,?,?,?,?) }")
		
		
	BEGIN_PARAM_MAP(C_Insert_CharacAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_CharacLog.account)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_CharacLog.charac_uid)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_CharacLog.charac)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(5, m_CharacLog.code)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(6, m_CharacLog.zone)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(7, m_CharacLog.level)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(8, m_CharacLog.point)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(9, m_CharacLog.ip)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(10, m_CharacLog.moreInfo)

	END_PARAM_MAP()
};


class C_Insert_Charac : public CCommand<CAccessor<C_Insert_CharacAccessor>, CRowset >
{
public:
	HRESULT		hr ;
	CSession	m_session ;
	SYSTEMTIME	time ;	
	TCHAR		qry[256] ;
	struct _strTime {
		char	y[5] ;
		char	m[3] ;
		char	d[3] ;
	} strTime;
	
	void ReflushQry()
	{
		GetLocalTime( &time ) ;
		sprintf( strTime.y, "%d", time.wYear ) ;
		
		if( time.wMonth < 10 )
			sprintf( strTime.m, "0%d", time.wMonth ) ;
		else
			sprintf( strTime.m, "%d", time.wMonth ) ;
		
		if( time.wDay < 10 )
			sprintf( strTime.d, "0%d", time.wDay ) ;		
		else
			sprintf( strTime.d, "%d", time.wDay ) ;		
		
		sprintf( qry, "{ ? = CALL dbo.pr_EOD_LOG_CHARAC_%s%s%s (?,?,?,?,?,?,?,?,?) }", strTime.y , strTime.m , strTime.d ) ;
	}

	C_Insert_Charac ()
	{
#if (_WIN32_WINNT > 0x0403)
		if( InitializeCriticalSectionAndSpinCount( &m_cs, (0xf0000000)|2000 ) == FALSE ) {			
			throw ;
		}
#else	
		InitializeCriticalSection( &m_cs ) ;
#endif
	}
	~C_Insert_Charac()
	{
		DeleteCriticalSection( &m_cs ) ;
	}

private:
	CRITICAL_SECTION m_cs ;
	void lock() { EnterCriticalSection( &m_cs )  ; }
	void unlock() { LeaveCriticalSection( &m_cs )  ; }

public:
	
	
	HRESULT SessionLink( CDB_Connector * pDB_Con ) 
	{
		hr = pDB_Con->SessionLink( &m_session ) ;

		if(FAILED(hr))
			DBErrorOutput( hr ) ;

		return hr ;
	}
	


	HRESULT OpenRowset()
	{
#ifdef _NEO_LOG
		ReflushQry() ;
		hr = Open(m_session, qry, NULL);
#else
		hr = Open(m_session, NULL, NULL);
#endif
		return hr;
	}


	
	bool Execute( _CharacLog * pNode )
	{	
		bool bRet = true ;

		lock() ;
		// Data param bind
		memcpy( &m_CharacLog, pNode, sizeof(_CharacLog) ) ;		
		
		// Execute
		hr = OpenRowset() ;	
		
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			bRet = false ;
		}
		unlock() ;
		

		return bRet ;
	}
	
};

class C_InsertLoginLog_Accessor
{
public:
	_LoginLog	m_log ;
	

	DEFINE_COMMAND ( C_InsertLoginLog_Accessor, "{ CALL pr_EOD_LOG_Login_log(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) }" )

	BEGIN_PARAM_MAP( C_InsertLoginLog_Accessor )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 1, m_log.acc_uid )
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
		COLUMN_ENTRY( 2, m_log.account ) 
#else
		COLUMN_ENTRY( 2, m_log.acc ) 
#endif
		COLUMN_ENTRY( 3, m_log.charac )
		COLUMN_ENTRY( 4, m_log.zone_no )
		COLUMN_ENTRY( 5, m_log.inner_level )
		COLUMN_ENTRY( 6, m_log.party )
		COLUMN_ENTRY( 7, m_log.m_class )
		COLUMN_ENTRY( 8, m_log.ip )
		COLUMN_ENTRY( 9, m_log.pe_pay_code )
		
		COLUMN_ENTRY( 10, m_log.pc_pay_code )
		COLUMN_ENTRY( 11, m_log.pc_uid )
		COLUMN_ENTRY( 12, m_log.login_time )
		COLUMN_ENTRY( 13, m_log.logout_time )
		COLUMN_ENTRY( 14, m_log.play_time )
		COLUMN_ENTRY( 15, m_log.type )
	END_PARAM_MAP()
		

};


class C_Insert_LoginLog : public CCommand<CAccessor<C_InsertLoginLog_Accessor>, CRowset >
{
public:
	HRESULT		hr ;
	CSession	m_session ;
	SYSTEMTIME	time ;	
	TCHAR		qry[256] ;
	struct _strTime {
		char	y[5] ;
		char	m[3] ;
		char	d[3] ;
	} strTime;
	
	/*
	void ReflushQry()
	{
		GetLocalTime( &time ) ;
		sprintf( strTime.y, "%d", time.wYear ) ;
		
		if( time.wMonth < 10 )
			sprintf( strTime.m, "0%d", time.wMonth ) ;
		else
			sprintf( strTime.m, "%d", time.wMonth ) ;
		
		if( time.wDay < 10 )
			sprintf( strTime.d, "0%d", time.wDay ) ;		
		else
			sprintf( strTime.d, "%d", time.wDay ) ;		
		
		sprintf( qry, "{ ? = CALL pr_EOD_LOG_Login_log_%s%s%s (?,?,?,?,?,?,?,?,?) }", strTime.y , strTime.m , strTime.d ) ;
	}
	*/

	C_Insert_LoginLog ()
	{
#if (_WIN32_WINNT > 0x0403)
		if( InitializeCriticalSectionAndSpinCount( &m_cs, (0xf0000000)|2000 ) == FALSE ) {			
			throw ;
		}
#else	
		InitializeCriticalSection( &m_cs ) ;
#endif
	}
	~C_Insert_LoginLog()
	{
		DeleteCriticalSection( &m_cs ) ;
	}

private:
	CRITICAL_SECTION m_cs ;
	void lock() { EnterCriticalSection( &m_cs )  ; }
	void unlock() { LeaveCriticalSection( &m_cs )  ; }

public:
	
	
	HRESULT SessionLink( CDB_Connector * pDB_Con ) 
	{
		hr = pDB_Con->SessionLink( &m_session ) ;

		if(FAILED(hr))
			DBErrorOutput( hr ) ;

		return hr ;
	}
	


	HRESULT OpenRowset()
	{
		hr = Open(m_session, NULL, NULL);
		return hr;
	}


	
	bool Execute( _LoginLog * pNode )
	{	
		bool bRet = true ;

		lock() ;
		// Data param bind
		memcpy( &m_log, pNode, sizeof(_LoginLog) ) ;		
		
		// Execute
		hr = OpenRowset() ;	
		
		if(FAILED(hr)){
			DBErrorOutput( hr ) ;
			bRet = false ;
		}
		unlock() ;
		

		return bRet ;
	}
	
};