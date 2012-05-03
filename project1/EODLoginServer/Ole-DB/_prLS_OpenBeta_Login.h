#pragma once

class C_prLS_OpenBeta_LoginAccessor
{
public:

	TCHAR m_acc[en_max_lil+1];
	TCHAR m_pw[en_max_lpl+1];
	LONG m_class ;
	INT	 m_state ;
	INT	 m_forest ;
	LONG m_result;
	DEFINE_COMMAND(C_prLS_OpenBeta_LoginAccessor, "{ CALL dbo._prLS_Login(?,?,?,?,?,?) }")

	BEGIN_PARAM_MAP(C_prLS_OpenBeta_LoginAccessor)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY(1, m_acc)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY(2, m_pw)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_class)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_state)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_forest)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_result)
	END_PARAM_MAP()

	void ResetAccessor( )
	{
		memset(m_acc, 0, en_max_lil+1) ;
		memset(m_pw, 0 , en_max_lpl+1 ) ;
		m_class = 0 ;
		m_state = 0 ;
		m_forest = 0 ;
		m_result = 0 ;
	}
};

#ifdef _METEL_LOGIN_200608_
class C_prLS_MetelLogin_Accessor
{
public:
	DEFINE_COMMAND(C_prLS_MetelLogin_Accessor, "{ CALL dbo._prLS_MetelLogin(?,?,?,?,?) }")

	BEGIN_PARAM_MAP(C_prLS_MetelLogin_Accessor)
		SET_PARAM_TYPE(DBPARAMIO_INPUT )
		COLUMN_ENTRY(1, m_acc)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY(2, m_class)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_state)
		SET_PARAM_TYPE(DBPARAMIO_INPUT )
		COLUMN_ENTRY(4, m_forest)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_result)
	END_PARAM_MAP()

	inline void SetForest( char fr )	{ m_forest = fr; }
	void ResetAccessor( )
	{
		memset(m_acc, 0, en_max_lil+1) ;
		m_class = 0 ;
		m_state = 0 ;
		m_forest = 0 ;
		m_result = 0 ;
	}

	inline char * GetAccount()	{ return m_acc ; }
	inline short GetClass()		{ return m_class ; }
	inline short GetState()		{ return m_state ; }
	inline short GetForest()	{ return m_forest ; }
	inline int GetResult()		{ return m_result ; }

protected:
	CHAR m_acc[en_max_lil+1];
	SHORT m_class ;
	SHORT m_state ;
	SHORT m_forest ;
	INT m_result;
};
#endif //_METEL_LOGIN_200608_

class C_prLS_NexonLogin_Accessor
{
public:
	DEFINE_COMMAND(C_prLS_NexonLogin_Accessor, "{ CALL dbo._prLS_NexonLogin(?,?,?,?,?,?,?,?) }")

	BEGIN_PARAM_MAP(C_prLS_NexonLogin_Accessor)
		SET_PARAM_TYPE(DBPARAMIO_INPUT )
		COLUMN_ENTRY(1, m_acc)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY(2, m_ndAcc)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY(3, m_class)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_state)
		SET_PARAM_TYPE(DBPARAMIO_INPUT )
		COLUMN_ENTRY(5, m_forest)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_nCount)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(7, m_acc_uid)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(8, m_result)
	END_PARAM_MAP()


	void ResetAccessor( )
	{
		memset(m_acc, 0, en_max_lil+1) ;
		memset(m_ndAcc, 0 , en_max_lil+1 ) ;
		m_class = 0 ;
		m_state = 0 ;
		m_forest = 0 ;
		m_nCount = 0 ;
		m_acc_uid = 0 ;
		m_result = 0 ;
	}

	inline void SetForest( char fr )	{ m_forest = fr; }
	inline void SetAccount( const char * nexon_acc )	{ strncpy( m_acc, nexon_acc, en_max_lil+1 ) ; }
	inline char * GetAccount()	{ return m_acc ; }
	inline char * GetNDAccount()	{ return m_ndAcc ; }
	inline short GetClass()		{ return m_class ; }
	inline short GetState()		{ return m_state ; }
	inline short GetForest()	{ return m_forest ; }
	inline short GetCount()		{ return m_nCount ; }
	inline int GetAccUid()		{ return m_acc_uid ; }
	inline int GetResult()		{ return m_result ; }


protected:
	CHAR m_acc[en_max_lil+1];
	CHAR m_ndAcc[en_max_lil+1] ;				// ±∏∑Ê¿Ô∆– ∑Œ±◊¿Œ æ∆¿Ã¥Ÿ. 
	SHORT m_class ;
	SHORT m_state ;
	SHORT m_forest ;
	SHORT m_nCount ;
	INT	m_acc_uid ;
	INT m_result;
};



class C_prGMLS_OpenBeta_LoginAccessor
{
public:
	
	TCHAR m_acc[en_max_lil+1];
	TCHAR m_pw[en_max_lpl+1];
	LONG m_class ;
	INT	 m_state ;
	INT	 m_forest ;
	LONG m_result;
	
	DEFINE_COMMAND(C_prGMLS_OpenBeta_LoginAccessor, "{ CALL dbo._prGMLS_Login(?,?,?,?,?,?) }")
		
		BEGIN_PARAM_MAP(C_prGMLS_OpenBeta_LoginAccessor)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY(1, m_acc)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY(2, m_pw)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_class)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_state)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_forest)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_result)
		END_PARAM_MAP()
		
	void ResetAccessor( )
	{
		memset(m_acc, 0, en_max_lil+1 ) ;
		memset(m_pw, 0 , en_max_lpl+1 ) ;
		m_class = 0 ;
		m_state = 0 ;
		m_forest = 0 ;
		m_result = 0 ;
	}
};


class _prLSUpdateServerStatusAccessor
{
public:
	short	uid;
	short	status_code ;

	DEFINE_COMMAND(_prLSUpdateServerStatusAccessor, "{ CALL dbo._prLSUpdateServerStatus(?,?) }" )

	BEGIN_PARAM_MAP(_prLSUpdateServerStatusAccessor)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY( 1, uid )
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY( 2, status_code )
	END_PARAM_MAP()
};

class prVersionControlAccessor
{
public:
	int		iPatchNum ;
	DWORD	dwClientVer ;
	int		iMode ;
	int		iIsTest ;
	

	DEFINE_COMMAND( prVersionControlAccessor, "{ CALL dbo._prVersionControl_with_Testserver(?,?,?,?) } " )

	BEGIN_PARAM_MAP( prVersionControlAccessor )
		SET_PARAM_TYPE( DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY( 1, iPatchNum )
		SET_PARAM_TYPE( DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY( 2, dwClientVer )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 3, iMode )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 4, iIsTest )
	END_PARAM_MAP()

	void ResetAccessor( ) { memset( this, 0, sizeof(*this) ) ; }
	
};


class _sp_AcclaimAuthAccessor
{
public:
	TCHAR		unique[60] ;

	BEGIN_COLUMN_MAP(_sp_AcclaimAuthAccessor)
		COLUMN_ENTRY(1, unique )
	END_COLUMN_MAP()
};


class _sp_AcclaimAuth1Accessor
{
public:
	TCHAR		unique[60] ;

	BEGIN_COLUMN_MAP(_sp_AcclaimAuth1Accessor)
		COLUMN_ENTRY(1, unique )
	END_COLUMN_MAP()
};

class _sp_AcclaimLoginAccessor
{
public:
	TCHAR m_acc[en_max_lil+1];
	LONG m_class ;
	INT	 m_state ;
	INT	 m_forest ;
	LONG m_result;

	DEFINE_COMMAND(_sp_AcclaimLoginAccessor, "{ CALL dbo._prLS_AcclaimLogin(?,?,?,?,?) }")

	BEGIN_PARAM_MAP(_sp_AcclaimLoginAccessor)
		SET_PARAM_TYPE(DBPARAMIO_INPUT )
		COLUMN_ENTRY(1, m_acc)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(2, m_class)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_state)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_forest)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_result)
	END_PARAM_MAP()

	void ResetAccessor( )
	{
		memset(m_acc, 0, en_max_lil+1) ;
		m_class = 0 ;
		m_state = 0 ;
		m_forest = 0 ;
		m_result = 0 ;
	}
} ;


class _sp_DS_LogoutAccessor
{
public:

	LONG		m_RETURN_VALUE;
	char		m_acc[en_max_lil+1];

	DEFINE_COMMAND(_sp_DS_LogoutAccessor, "{ ? = CALL dbo._prDS_Logout(?) }")


		BEGIN_PARAM_MAP(_sp_DS_LogoutAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_acc)			
		END_PARAM_MAP()

	void ResetAccessor()
	{
		m_RETURN_VALUE = 0;
		memset( m_acc, 0, en_max_lil+1 );
	}
};


#ifdef _PD_VINA_PCROOM_INFO_
class _sp_LS_Vina_PCRoomAccessor
{
protected:
	char	m_ip[16];
	int		m_result;

public:
	DEFINE_COMMAND(_sp_LS_Vina_PCRoomAccessor, "{ CALL dbo._prLS_GetIsPcbang(?,?) }" )

		BEGIN_PARAM_MAP(_sp_LS_Vina_PCRoomAccessor)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1, m_ip)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(2, m_result)
		END_PARAM_MAP()


	void ResetAccessor()
	{
		ZeroMemory( m_ip, sizeof(m_ip) );
		m_result = 0;
	}

	void SetIP( char * ip )
	{
		strncpy( m_ip, ip, 16 );
		m_ip[15] = 0;
	}

	u_char GetResult()	{ return static_cast<u_char>(m_result); }
};
#endif