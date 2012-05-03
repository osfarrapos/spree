#pragma	   once

#include <objbase.h>
#include <atldbcli.h>
#include <oledb.h>

#include "NDUtilClass.h"

/*=========================================================
	OLE DB Connection library
	class CDB_Connector
	
	2004.06.03		by. ozzywow  

	This class is not surport DSN Connection type. 
	Only for OLE Connection from OLE string...	

	Library link : ole32.lib
==========================================================*/
class CDB_Connector : CNDThread
{

protected:

	HRESULT hr ;
	CDataSource _db ;
	LPCOLESTR lpConStr ;

	DWORD dwDBReConnectThreadID ; 
	HANDLE hDBReConnectThread ; 
	

	int Run()
	{
		while(1)
		{			
			hr = ReTryConnect() ;
			if( !FAILED(hr) ) 
			{			
				bThreadRun = false;					
				break;
			}								
			Sleep(10000);
		}
		// 
		return 0;
	}
	
public:		

	bool bThreadRun;

	bool initialize(LPCOLESTR ConStr) /* = OLESTR(str) */
	{	
		lpConStr = ConStr ;

		// Initialize the OLE COM Library (if STM Model use CoInisialize )
		hr = CoInitializeEx(NULL, COINIT_MULTITHREADED) ;
		
		if (hr != S_OK)	// MTM Model Com Lib Initialize
			if (hr != S_FALSE)
				return false;	

		// DB Connect	
		hr = _db.OpenFromInitializationString( lpConStr );	
		if(FAILED(hr)) {			
			return false;
		}
		return true ;		

	}

	void release()
	{
		_db.Close();
		CoUninitialize();	// release Com Library

	}

	HRESULT ReTryConnect()
	{
		_db.Close() ;
		return _db.OpenFromInitializationString( lpConStr ) ;		
	}


	HRESULT SessionLink(CSession * pSession )
	{
		//Session open			--Ninedragons
		return pSession->Open(_db);	
	}

	void SessionClose(CSession * pSession )
	{
		pSession->Close() ;
	}

	CDataSource & GetDataSourse() { return _db ; }

	// OLE-DB ReConnection Thread creat & running..
	void CallReConTh()
	{		
		if( bThreadRun == false ){							// Running thread is true		
			bThreadRun = true;
			CNDThread::Start() ;
		}	
	}

	HRESULT GetHresult() { return hr ; } 
};



/*=========================================================
	OLE DB Session library
	class CDB_Session
	
	2004.06.03		by. ozzywow  	
==========================================================*/
class CDB_Session
{
	
protected:	
	CSession m_session ;	
	
public:	
	HRESULT OpenSession( CDB_Connector * pDBCon )
	{		
		return m_session.Open( pDBCon->GetDataSourse() ) ;
	}
	void CloseSession()
	{
		m_session.Close() ;		
	}
	CSession * GetSessionPtr() 
	{ 
		return &m_session ; 
	}

	// Tracsaction function
	HRESULT StartTran(ISOLEVEL ioslevel = ISOLATIONLEVEL_READCOMMITTED)
	{
		return m_session.StartTransaction( ioslevel ) ;
	}

	HRESULT CommitTran()
	{
		return m_session.Commit() ;
	}

	HRESULT AbortTran()
	{
		return m_session.Abort() ;
	}
	
private:
};




/*=========================================================
	OLE DB Template Accessor Commander library
	class CDB_Command
	
	2004.06.03		by. ozzywow  	
==========================================================*/
template <typename TAccessor, typename TRowset=CRowset>
class CDB_Command
{
protected:
	CCommand<CAccessor<TAccessor>, TRowset > * m_pSet ;
public:
	CDB_Command()
	{
		m_pSet = new CCommand<CAccessor<TAccessor>, TRowset > ;		
	}
	~CDB_Command()
	{
		delete m_pSet ;
	}

	HRESULT OpenCommand( CSession session )
	{
		return  m_pSet->Open( session ) ;
	}

	void CloseCommand()
	{
		m_pSet->Close() ;
		m_pSet->ReleaseCommand() ;
	}

	TAccessor * GetAccessor()
	{
		return (TAccessor * ) m_pSet ;
	}

private:
};