#include "..\stdafx.h"

#include "db_command.h"

void DestroyGlobalObject() ;

extern CLinePrintFile *		g_pErrMsg ;		// 에러메세지 출력기

void DBErrorOutput(HRESULT hrErr)
{
	CDBErrorInfo ErrorInfo;
	ULONG        cRecords;
	HRESULT      hr;
	ULONG        i;
	CComBSTR     bstrDesc, bstrHelpFile, bstrSource;
	GUID         guid;
	DWORD        dwHelpContext;
	WCHAR        wszGuid[40];
	USES_CONVERSION;
	
	// If the user passed in an HRESULT then trace it
	if (hrErr != S_OK)
	{
		g_pErrMsg->LinePrint("OLE DB Error Record dump for hr = 0x%x\n", hrErr);
		PrintConsole("OLE DB Error Record dump for hr = 0x%x\n", hrErr);
	}
	
	LCID lcLocale = GetSystemDefaultLCID();
	
	hr = ErrorInfo.GetErrorRecords(&cRecords);
	if (FAILED(hr) && ErrorInfo.m_spErrorInfo == NULL)
	{
		g_pErrMsg->LinePrint("No OLE DB Error Information found: hr = 0x%x\n", hr);
		PrintConsole("No OLE DB Error Information found: hr = 0x%x\n", hr);
	}
	else
	{
		for (i = 0; i < cRecords; i++)
		{
			hr = ErrorInfo.GetAllErrorInfo(i, lcLocale, &bstrDesc, &bstrSource, &guid,
				&dwHelpContext, &bstrHelpFile);
			if (FAILED(hr))
			{
				g_pErrMsg->LinePrint("OLE DB Error Record dump retrieval failed: hr = 0x%x\n", hr);
				PrintConsole("OLE DB Error Record dump retrieval failed: hr = 0x%x\n", hr);
				return;
			}
			StringFromGUID2(guid, wszGuid, sizeof(wszGuid) / sizeof(WCHAR));
			g_pErrMsg->LinePrint("Row #: %4d Source: \"%s\" Description: \"%s\" Help File: \"%s\" Help Context: %4d GUID: %s\n",
				i, OLE2T(bstrSource), OLE2T(bstrDesc), OLE2T(bstrHelpFile), dwHelpContext, OLE2T(wszGuid));
			PrintConsole("Row #: %4d Source: \"%s\" Description: \"%s\" Help File: \"%s\" Help Context: %4d GUID: %s\n",
				i, OLE2T(bstrSource), OLE2T(bstrDesc), OLE2T(bstrHelpFile), dwHelpContext, OLE2T(wszGuid));
			bstrSource.Empty();
			bstrDesc.Empty();
			bstrHelpFile.Empty();
		}
		g_pErrMsg->LinePrint("OLE DB Error Record dump end\n");
	}
}


void COLE_DB_Executer::CallReConTh()
{ 	
	printf( "[WORNING] ACC DB link is disconnected \n" ) ;
	::PrintConsole( "[WORNING] ACC DB link is disconnected \n" ) ;

#ifdef _IF_DB_LINK_DISCONNECT__SERVER_KILL_
	DestroyGlobalObject() ;
#endif

#ifdef _DB_LINK_AUTO_RECONNECT
	m_pDBCon->CallReConTh() ; 
#else
	return ;
#endif
}


#ifdef _PD_COMPANY_JAPAN_

int COLE_DB_Executer::RocLogin( IN OUT char * acc, const char * pw, char * cClass, char * state, char * forest, bool bCheckPassword )
{
	if( acc == NULL || pw == NULL || cClass == NULL || state == NULL || forest == NULL )
		return -1 ;

	HRESULT hr;
	int  iresult=0;

	m_LS_Login.GetAccessor()->ResetAccessor() ;

	memcpy(m_LS_Login.GetAccessor()->m_acc, acc, en_max_lil+1);
	memcpy(m_LS_Login.GetAccessor()->m_pw, pw, en_max_lpl+1);
	m_LS_Login.GetAccessor()->m_forest = *forest;

	try{
		hr = m_LS_Login.OpenCommand( *m_pDBSession->GetSessionPtr() ) ;
	}
	catch (...) {
		m_LS_Login.CloseCommand() ;
		return -1;
	}

	if(FAILED(hr)){ 			
		if(hr == E_FAIL)
		{
			CallReConTh() ;
		}

		::PrintConsole("[ERROR] DB::Login(%s, %s) \n", acc, pw ) ;
		DBErrorOutput(hr) ;

		m_LS_Login.CloseCommand() ;
		return -1;
	}		

	iresult = m_LS_Login.GetAccessor()->m_result;
	
	bool bCopy = false ;		// 영문 대소문자 구분이 필요할 때는, false
								// 대소문자 구분이 필요없을 때는 true 다.

	if( bCopy )
		memcpy(acc, m_LS_Login.GetAccessor()->m_acc, en_max_lil ) ;
	else
	{
		if( strcmp( acc, m_LS_Login.GetAccessor()->m_acc ) != 0 )
		{
			iresult = _LOGIN_DB_RETURN__NOT_FOUND_ ;
		}
	}

	*cClass = (char)m_LS_Login.GetAccessor()->m_class ;
	*state = (char)m_LS_Login.GetAccessor()->m_state ;
	*forest = (char)m_LS_Login.GetAccessor()->m_forest ;
	
	m_LS_Login.CloseCommand() ;

	return iresult;
}

#endif

int COLE_DB_Executer::Login( IN OUT char * acc, const char * pw, char * cClass, char * state, char * forest )
{
	if( acc == NULL || pw == NULL || cClass == NULL || state == NULL || forest == NULL )
		return -1 ;

	HRESULT hr;
	int  iresult=0;

	m_LS_Login.GetAccessor()->ResetAccessor() ;

	memcpy(m_LS_Login.GetAccessor()->m_acc, acc, en_max_lil+1);
	memcpy(m_LS_Login.GetAccessor()->m_pw, pw, en_max_lpl+1);
	m_LS_Login.GetAccessor()->m_forest = *forest;

	try{
		hr = m_LS_Login.OpenCommand( *m_pDBSession->GetSessionPtr() ) ;
	}
	catch (...) {
		m_LS_Login.CloseCommand() ;
		return -1;
	}

	if(FAILED(hr)){ 			
		if(hr == E_FAIL)
		{
			CallReConTh() ;
		}

		::PrintConsole("[ERROR] DB::Login(%s, %s) \n", acc, pw ) ;
		DBErrorOutput(hr) ;

		m_LS_Login.CloseCommand() ;
		return -1;
	}		

	iresult = m_LS_Login.GetAccessor()->m_result;
	
	bool bCopy = false ;		// 영문 대소문자 구분이 필요할 때는, false
								// 대소문자 구분이 필요없을 때는 true 다.

#ifdef _PD_COMPANY_VINA_
	bCopy = true;
#endif

	if( bCopy )
		memcpy(acc, m_LS_Login.GetAccessor()->m_acc, en_max_lil ) ;
	else
	{
		if( strcmp( acc, m_LS_Login.GetAccessor()->m_acc ) != 0 )
		{
			iresult = _LOGIN_DB_RETURN__NOT_FOUND_ ;
		}
	}

	*cClass = (char)m_LS_Login.GetAccessor()->m_class ;
	*state = (char)m_LS_Login.GetAccessor()->m_state ;
	*forest = (char)m_LS_Login.GetAccessor()->m_forest ;
	
	m_LS_Login.CloseCommand() ;

	return iresult;
}


int COLE_DB_Executer::GMLogin( IN OUT char * acc, const char * pw, char * cClass, char * state, char * forest )
{

	if( acc == NULL || pw == NULL || cClass == NULL || state == NULL || forest == NULL )
		return -1 ;

	HRESULT hr;

	int  iresult=0;
	
	//char sourceAcc[13] ;
	//memcpy( sourceAcc, acc, 13 ) ;

	m_GMLS_Login.GetAccessor()->ResetAccessor() ;
	//m_LS_Login.ResetAccessor() ;
	memcpy(m_GMLS_Login.GetAccessor()->m_acc, acc, 13);
	memcpy(m_GMLS_Login.GetAccessor()->m_pw, pw, 13);

	try{
		hr = m_GMLS_Login.OpenCommand( *m_pDBSession->GetSessionPtr() ) ;
	}
	catch (...) {
		m_GMLS_Login.CloseCommand() ;
		return -1;
	}

	if(FAILED(hr)){ 			
		if(hr == E_FAIL)
		{
			CallReConTh() ;
		}

		
		::PrintConsole("[ERROR] DB::GMLogin(%s, %s) \n", acc, pw ) ;
		DBErrorOutput(hr) ;

		return -1;
	}		

	iresult = m_GMLS_Login.GetAccessor()->m_result;
	
	// DB 에서 읽어온 ID 로 바꿔줘야 한다.. (DS 맵 문제 때문이다..)
	//memcpy(acc, m_LS_Login.GetAccessor()->m_acc, 13 ) ;

	// 소문자로 변환해서 비교한다..
	/*
	if( strcmp( strlwr(sourceAcc), strlwr(m_LS_Login.GetAccessor()->m_acc) ) != 0 )
	{
		iresult = _LOGIN_DB_RETURN__NOT_FOUND_ ;
	}
	*/

	if( strcmp( acc, m_GMLS_Login.GetAccessor()->m_acc ) != 0 )
	{
		iresult = _LOGIN_DB_RETURN__NOT_FOUND_ ;
	}

	// 소문자로 변환해서 비교한다..
	//if( strcmp( pw, m_LS_Login.GetAccessor()->m_pw ) != 0 )
	//{
	//	iresult = _LOGIN_DB_RETURN__NOT_MATCH_PASSWORD ;
	//}


	

	*cClass = (char)m_GMLS_Login.GetAccessor()->m_class ;
	*state = (char)m_GMLS_Login.GetAccessor()->m_state ;
	*forest = (char)m_GMLS_Login.GetAccessor()->m_forest ;
	
	m_GMLS_Login.CloseCommand() ;

	return iresult;
}



void COLE_DB_Executer::ServerStatusUpdate(const short uid, short state )
{
	HRESULT hr ;


	if( state > 0 )
	{
		state = _FOREST_STATE_RUNNING ;
	}
	

	// on
	if( state == 0 )
	{
		state = 1 ;
	}
	// over connect
	else if( state == _FOREST_STATE_FULL_ )
	{
		state = 2 ;		
	}
	// checkup
	else if( state == _FOREST_STATE_CHECKUP_ )
	{
		state = 3 ;
	}
	// off
	else if( state == _FOREST_STATE_OFF )
	{
		state = 0 ;
	}

	m_ServerStatusUpdate.GetAccessor()->uid = uid ;
	m_ServerStatusUpdate.GetAccessor()->status_code = state ;

	try{
		hr = m_ServerStatusUpdate.OpenCommand( *m_pDBSession->GetSessionPtr() ) ;
	}
	catch (...) {
		m_ServerStatusUpdate.CloseCommand() ;		
	}

	if( FAILED(hr) )
	{
		if( hr == E_FAIL )
		{
			CallReConTh() ;
		}
		
		::PrintConsole("[ERROR] DB::ServerStatusUpdate(%d, %d) \n", uid, state ) ;
		DBErrorOutput(hr) ;

	}

	m_ServerStatusUpdate.CloseCommand() ;
}


void COLE_DB_Executer::VersionControl(int & iPatchNum, DWORD & dwClientVer, int iMode, int iIsTest )
{
	HRESULT hr ;

	m_VersionControl.GetAccessor()->ResetAccessor() ;
	
	m_VersionControl.GetAccessor()->iMode = iMode ;
	m_VersionControl.GetAccessor()->iPatchNum = iPatchNum ;
	m_VersionControl.GetAccessor()->dwClientVer = dwClientVer ;
	m_VersionControl.GetAccessor()->iIsTest = iIsTest ;

	try{
		hr = m_VersionControl.OpenCommand( *m_pDBSession->GetSessionPtr() ) ;
	}
	catch (...) {
		m_VersionControl.CloseCommand() ;
	}
	
	if( FAILED(hr) )
	{
		if( hr == E_FAIL )
		{
			CallReConTh() ;
		}
		
		::PrintConsole("[ERROR] DB::VersionControl(%d, %u, %d, %d) \n", iPatchNum, dwClientVer, iMode, iIsTest ) ;
		DBErrorOutput(hr) ;
	}

	iPatchNum = m_VersionControl.GetAccessor()->iPatchNum ;
	dwClientVer = m_VersionControl.GetAccessor()->dwClientVer ;

	m_VersionControl.CloseCommand() ;

}



#ifdef _METEL_LOGIN_200608_
int COLE_DB_Executer::MetelLogin( IN OUT char * acc, char * cClass, char * state, char * forest )
{
	if( acc == NULL || cClass == NULL || state == NULL || forest == NULL )
		return -1 ;

	HRESULT hr;
	int  iresult=0;
	
	m_LS_MetelLogin.GetAccessor()->ResetAccessor() ;

	memcpy(m_LS_MetelLogin.GetAccessor()->GetAccount(), acc, en_max_lil+1);
	m_LS_MetelLogin.GetAccessor()->SetForest( *forest );

	try{
		hr = m_LS_MetelLogin.OpenCommand( *m_pDBSession->GetSessionPtr() ) ;
	}
	catch (...) {
		m_LS_MetelLogin.CloseCommand() ;
		return -1;
	}

	if(FAILED(hr)){ 			
		if(hr == E_FAIL)
		{
			CallReConTh() ;
		}

		::PrintConsole("[ERROR] DB::Login(%s) \n", acc ) ;
		DBErrorOutput(hr) ;

		m_LS_MetelLogin.CloseCommand() ;
		return -1;
	}		

	iresult = m_LS_MetelLogin.GetAccessor()->GetResult();
	
	if( strcmp( acc, m_LS_MetelLogin.GetAccessor()->GetAccount() ) != 0 )
	{
		iresult = _LOGIN_DB_RETURN__NOT_FOUND_ ;
	}

	*cClass = (char)m_LS_MetelLogin.GetAccessor()->GetClass() ;
	*state = (char)m_LS_MetelLogin.GetAccessor()->GetState() ;
	*forest = (char)m_LS_MetelLogin.GetAccessor()->GetForest() ;
	
	m_LS_MetelLogin.CloseCommand() ;

	return iresult;
}
#endif //_METEL_LOGIN_200608_

int COLE_DB_Executer::NexonLogin( const char * nexon_acc, char * ndAcc, char * cClass, char * state, char * forest, short * nCount, int * pAccUid )
{
	if( nexon_acc == NULL || ndAcc == NULL || cClass == NULL || state == NULL || forest == NULL )
		return -1 ;

	HRESULT hr;

	int  iresult=0;
	
	m_LS_NexonLogin.GetAccessor()->ResetAccessor() ;
	
	m_LS_NexonLogin.GetAccessor()->SetAccount(nexon_acc) ;
	m_LS_NexonLogin.GetAccessor()->SetForest( *forest );

	try{
		hr = m_LS_NexonLogin.OpenCommand( *m_pDBSession->GetSessionPtr() ) ;
	}
	catch (...) {
		m_LS_NexonLogin.CloseCommand() ;
		return -1;
	}

	if(FAILED(hr))
	{
		::PrintConsole("[ERROR] DB::NexonLogin(%s) \n", nexon_acc ) ;
		DBErrorOutput(hr) ;

		m_LS_NexonLogin.CloseCommand() ;
		return -1;
	}		

	iresult = m_LS_NexonLogin.GetAccessor()->GetResult();
	
	if( strncmp( nexon_acc, m_LS_NexonLogin.GetAccessor()->GetAccount(), en_max_lil+1 ) != 0 )
	{
		iresult = _LOGIN_DB_RETURN__NOT_FOUND_ ;
	}

	if( m_LS_NexonLogin.GetAccessor()->GetCount() != 1 )
		iresult = _LOGIN_DB_RETURN__NOT_UNI_NEXON ;

	strncpy( ndAcc, m_LS_NexonLogin.GetAccessor()->GetNDAccount(), en_max_lil );
	ndAcc[en_max_lil] = 0;
	*cClass = static_cast<char>( m_LS_NexonLogin.GetAccessor()->GetClass() ) ;
	*state = static_cast<char>( m_LS_NexonLogin.GetAccessor()->GetState() ) ;
	*nCount = m_LS_NexonLogin.GetAccessor()->GetCount() ;
	*pAccUid = m_LS_NexonLogin.GetAccessor()->GetAccUid() ;
	
	m_LS_NexonLogin.CloseCommand() ;

	return iresult;
}


int COLE_DB_Executer::NexonGMLogin( const char * nexon_acc, char * pw, char * ndAcc, char * cClass, char * state, char * forest )
{

	return 0 ;
}


int COLE_DB_Executer::AcclaimAuth( const char * acclaim_acc, const char * pw )
{
	// return : authenticate result
	static char query[256] = {0, } ;

	static int nCount = 0 ;

	try
	{
		++nCount ;
		ZeroMemory( query, sizeof(query) ) ;

		sprintf( query, "select authenticate( '%s', '%s', '999' )", acclaim_acc, pw ) ;

		PrintConsole( "[AUTH] %3d - Query : %s\n", nCount, query ) ;

		ZeroMemory( m_LS_AcclaimAuth.unique, sizeof(m_LS_AcclaimAuth.unique) ) ;

		HRESULT hr = m_LS_AcclaimAuth.Open( *(m_pDBSession->GetSessionPtr()), query ) ;

		if( FAILED(hr) )
		{
			if( hr == DB_E_ABORTLIMITREACHED )
			{
				m_LS_AcclaimAuth.unique[0] = '9' ;	// 없는 유저.
				m_LS_AcclaimAuth.unique[1] = 0 ;
				PrintConsole("[DB CALL] Fail : No user (%s): hr = 0x%x\n", acclaim_acc, hr) ;
				DBErrorOutput(hr) ;
			}
			else
			{
				m_LS_AcclaimAuth.unique[0] = '9' ;	// 없는 유저.
				m_LS_AcclaimAuth.unique[1] = 0 ;
				PrintConsole("[DB CALL] AccalimAuth(%s) : hr = 0x%x \n", acclaim_acc, hr ) ;
				DBErrorOutput(hr) ;
			}

			m_pDBCon->ResetDBConFlag();
		}
		else
		{
			m_LS_AcclaimAuth.MoveNext() ;

			PrintConsole( "[AUTH] Query Success %s\n", m_LS_AcclaimAuth.unique ) ;
		}

		m_LS_AcclaimAuth.Close() ;	
		m_LS_AcclaimAuth.ReleaseCommand() ;
	}
	catch( ... )
	{
		m_LS_AcclaimAuth.unique[0] = '3' ;
		m_LS_AcclaimAuth.unique[1] = 0 ;

		PrintConsole("[DB CALL] exception AccalimAuth(%s) \n", acclaim_acc) ;

		m_LS_AcclaimAuth.Close() ;	
		m_LS_AcclaimAuth.ReleaseCommand() ;
		
	}

	return atoi( m_LS_AcclaimAuth.unique ) ;
}

int COLE_DB_Executer::AcclaimAuth1( const char * acclaim_acc, const char * pw )
{
	// return : authenticate result
	static char query[256] = {0, } ;

	try
	{
		ZeroMemory( query, sizeof(query) ) ;

		sprintf( query, "select authenticate1( '%s', '%s', '999' )", acclaim_acc, pw ) ;

		PrintConsole( "[AUTH] Query : %s\n", query ) ;

		ZeroMemory( m_LS_AcclaimAuth1.unique, sizeof(m_LS_AcclaimAuth1.unique) ) ;

		HRESULT hr = m_LS_AcclaimAuth1.Open( *(m_pDBSession->GetSessionPtr()), query ) ;

		if( FAILED(hr) )
		{
			if( hr == DB_E_ABORTLIMITREACHED )
			{
				m_LS_AcclaimAuth1.unique[0] = '9' ;	// 없는 유저.
				m_LS_AcclaimAuth1.unique[1] = 0 ;
				PrintConsole("[DB CALL] Fail : NO user (%s) : hr = 0x%x \n", acclaim_acc, hr) ;
				DBErrorOutput(hr) ;
			}
			else
			{
				m_LS_AcclaimAuth1.unique[0] = '9' ;	// 없는 유저.
				m_LS_AcclaimAuth1.unique[1] = 0 ;
				PrintConsole("[DB CALL] AccalimAuth1(%s) : hr = 0x%x \n", acclaim_acc, hr ) ;
				DBErrorOutput(hr) ;
			}

			m_pDBCon->ResetDBConFlag();
		}
		else
		{
			m_LS_AcclaimAuth1.MoveNext() ;

			PrintConsole( "[AUTH] Query Success %s\n", m_LS_AcclaimAuth1.unique ) ;
		}



		m_LS_AcclaimAuth1.Close() ;	
		m_LS_AcclaimAuth1.ReleaseCommand() ;
	}
	catch( ... )
	{
		m_LS_AcclaimAuth1.unique[0] = '3' ;
		m_LS_AcclaimAuth1.unique[1] = 0 ;

		PrintConsole("[DB CALL] exception AccalimAuth1(%s) \n", acclaim_acc) ;

		m_LS_AcclaimAuth1.Close() ;	
		m_LS_AcclaimAuth1.ReleaseCommand() ;
	}

	return atoi( m_LS_AcclaimAuth1.unique ) ;
}


int COLE_DB_Executer::AcclaimLogin( const char * acclaim_acc, char * cClass, char * state, char * forest )
{
	if( acclaim_acc == NULL || cClass == NULL || state == NULL || forest == NULL )
		return -1 ;

	HRESULT hr;

	int  iresult=0;

	try
	{
		m_LS_AcclaimLogin.GetAccessor()->ResetAccessor() ;

		memcpy(m_LS_AcclaimLogin.GetAccessor()->m_acc, acclaim_acc, en_max_lil+1);
		m_LS_AcclaimLogin.GetAccessor()->m_forest = (*forest) ; 

		hr = m_LS_AcclaimLogin.OpenCommand( *m_pDBSession->GetSessionPtr() ) ;

		if(FAILED(hr))
		{ 			
			if(hr == E_FAIL)
			{
				CallReConTh() ;
			}

			::PrintConsole("[ERROR] DB::AcclaimLogin(%s) \n", acclaim_acc ) ;
			DBErrorOutput(hr) ;

			iresult = -1 ;
		}	
		else
		{

			iresult = m_LS_AcclaimLogin.GetAccessor()->m_result;
			
			*cClass = static_cast<char>(m_LS_AcclaimLogin.GetAccessor()->m_class) ;
			*state = static_cast<char>(m_LS_AcclaimLogin.GetAccessor()->m_state) ;
		}

		m_LS_AcclaimLogin.CloseCommand() ;

	}
	catch (...) {
		m_LS_AcclaimLogin.CloseCommand() ;
		iresult = -1 ;
	}

	return iresult;
}


void COLE_DB_Executer::DSLogout( const char * acc )
{
	if( acc == NULL )
		return;

	try{		

		HRESULT hr;	
		
		m_DS_Logout.GetAccessor()->ResetAccessor() ;

		memcpy( m_DS_Logout.GetAccessor()->m_acc, acc, en_max_lil+1 ) ;

		hr = m_DS_Logout.OpenCommand(*m_pDBSession->GetSessionPtr());	
		m_DS_Logout.CloseCommand() ;

		if(FAILED(hr)){
			::PrintConsole("[ERROR] DB::DSLogout\n" ) ;

			DBErrorOutput( hr ) ;

			if(hr == E_FAIL) 
				CallReConTh();
		}	

	}
	catch (...) {
		::PrintConsole("[EXCEPTION] DSLogout\n" );
		m_DS_Logout.CloseCommand();
	}	

	
}


#ifdef _PD_VINA_PCROOM_INFO_
u_char COLE_DB_Executer::VinaGetPCRommInfo( char * ip )
{
	if( NULL == ip )
		return 0;

	u_char result = 0;
	try
	{
		m_LS_Vina_IsPCRomm.GetAccessor()->ResetAccessor();

		m_LS_Vina_IsPCRomm.GetAccessor()->SetIP( ip );

		HRESULT hr = m_LS_Vina_IsPCRomm.OpenCommand( *m_pDBSession->GetSessionPtr() );
		if( FAILED(hr) )
		{
			::PrintConsole( "[ERROR] DB::_prLS_GetIsPcbang\n" );
			DBErrorOutput(hr);

			if( hr == E_FAIL )
				CallReConTh();
		}
		else
			result = m_LS_Vina_IsPCRomm.GetAccessor()->GetResult();

		m_LS_Vina_IsPCRomm.CloseCommand();

	}
	catch( ... )
	{
		::PrintConsole("[EXCEPTION] _prLS_GetIsPcbang\n" );
		m_LS_Vina_IsPCRomm.CloseCommand();
	}

	return result;
}
#endif