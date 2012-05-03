#pragma	   once


#define DBErrorConsole(hr)	_DBErrorConsole(hr, __FILE__, __LINE__)
inline void _DBErrorConsole(HRESULT hrErr, char * file, int line )
{


	printf( "[OLE_ERROR] fine:%s, line%d\n", file, line ) ;

#ifdef	_DEBUG
	AtlTraceErrorRecords( hrErr ) ;
#endif

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
		printf("OLE DB Error Record dump for hr = 0x%x\n", hrErr);

	LCID lcLocale = GetSystemDefaultLCID();

	hr = ErrorInfo.GetErrorRecords(&cRecords);
	if (FAILED(hr) && ErrorInfo.m_spErrorInfo == NULL)
	{
		printf("No OLE DB Error Information found: hr = 0x%x\n", hr);
	}
	else
	{
		for (i = 0; i < cRecords; i++)
		{
			hr = ErrorInfo.GetAllErrorInfo(i, lcLocale, &bstrDesc, &bstrSource, &guid,
				&dwHelpContext, &bstrHelpFile);
			if (FAILED(hr))
			{
				printf("OLE DB Error Record dump retrieval failed: hr = 0x%x\n", hr);
				return;
			}
			StringFromGUID2(guid, wszGuid, sizeof(wszGuid) / sizeof(WCHAR));
			printf("Row #: %4d Source: \"%s\" Description: \"%s\" Help File: \"%s\" Help Context: %4d GUID: %s\n",
				i, OLE2T(bstrSource), OLE2T(bstrDesc), OLE2T(bstrHelpFile), dwHelpContext, OLE2T(wszGuid));
			bstrSource.Empty();
			bstrDesc.Empty();
			bstrHelpFile.Empty();
		}
		printf("OLE DB Error Record dump end\n");
	}
}


#include "../myServer/MY_OLE_DB.H"
#include "_prLS_OpenBeta_Login.h"

class COLE_DB_Executer 
{

protected:	

	CDB_Connector * const m_pDBCon ;
	CDB_Session * const  m_pDBSession ;
	CDB_Command<C_prLS_OpenBeta_LoginAccessor >	m_LS_Login ;
	CDB_Command<C_prGMLS_OpenBeta_LoginAccessor >	m_GMLS_Login ;
	CDB_Command<_prLSUpdateServerStatusAccessor> m_ServerStatusUpdate ;
	CDB_Command<prVersionControlAccessor>		m_VersionControl ;
#ifdef _METEL_LOGIN_200608_
	CDB_Command<C_prLS_MetelLogin_Accessor>		m_LS_MetelLogin ;
#endif //_METEL_LOGIN_200608_
	CDB_Command<C_prLS_NexonLogin_Accessor>		m_LS_NexonLogin ;
	CCommand<CAccessor<_sp_AcclaimAuthAccessor> ,CRowset > 		m_LS_AcclaimAuth ;
	CCommand<CAccessor<_sp_AcclaimAuth1Accessor> ,CRowset > 	m_LS_AcclaimAuth1 ;
	CDB_Command<_sp_AcclaimLoginAccessor>		m_LS_AcclaimLogin ;
	CDB_Command<_sp_DS_LogoutAccessor>			m_DS_Logout;
	
#ifdef _PD_VINA_PCROOM_INFO_
	CDB_Command<_sp_LS_Vina_PCRoomAccessor>		m_LS_Vina_IsPCRomm;
#endif

	
public:

	COLE_DB_Executer(CDB_Connector * pDBCon) : m_pDBCon(pDBCon), m_pDBSession( pDBCon->NewSession() )
	{
	}
	
	~COLE_DB_Executer()
	{
		if( m_pDBCon )
			m_pDBCon->FreeSession( m_pDBSession ) ;
	}

	bool IsConnected()	{ return m_pDBCon->IsConnected(); }
	void CallReConTh() ;
	bool RestoreDBCon()	{ return m_pDBCon->RestoreDBCon(); }
	void ResetDBConFlag()	{ m_pDBCon->ResetDBConFlag(); }

	int	Login(IN OUT char * acc, const char * pw, char * cClass, char * state, char * forest );
#ifdef _PD_COMPANY_JAPAN_
	int	RocLogin(IN OUT char * acc, const char * pw, char * cClass, char * state, char * forest, bool bCheckPassword );
#endif

	int	GMLogin(IN OUT char * acc, const char * pw, char * cClass, char * state, char * forest );
	void ServerStatusUpdate( const short uid, short state ) ;
	void VersionControl( int & iPatchNum, DWORD & dwClientVer, int iMode, int iIsTest ) ;

#ifdef _METEL_LOGIN_200608_
	int MetelLogin( IN OUT char * acc, char * cClass, char * state, char * forest );
#endif //_METEL_LOGIN_200608_

	int NexonLogin( const char * nexon_acc, char * ndAcc, char * cClass, char * state, char * forest, short * nCount, int * pAccUid ) ;
	int NexonGMLogin( const char * nexon_acc, char * pw, char * ndAcc, char * cClass, char * state, char * forest ) ;

	int AcclaimAuth( const char * acclaim_acc, const char * pw ) ;
	int AcclaimAuth1( const char * acclaim_acc, const char * pw ) ; 
	int AcclaimLogin( const char * acclaim_acc, char * cClass, char * state, char * forest ) ;

	void DSLogout( const char * acc );

#ifdef _PD_VINA_PCROOM_INFO_
	u_char VinaGetPCRommInfo( char * ip );
#endif

private:
};
