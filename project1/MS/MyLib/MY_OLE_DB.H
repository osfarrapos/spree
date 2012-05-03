/*================================================================================
 by ozzywow
=================================================================================*/



#pragma	   once


#include <objbase.h>
#include <atldbcli.h>
#include <oledb.h>
#include "MY_QUEUE.H"

#include "NDUtilClass.h"



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

	CDB_Session * pNext ;
	CDB_Session() : pNext(NULL)
	{		
	}

	inline void CloseSession()
	{
		m_session.Close() ;		
	}
	inline CSession * GetSessionPtr() 
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
	OLE DB Connection library
	class CDB_Con
	
	2004.06.03		by. ozzywow  

	This class is not surport DSN Connection type. 
	Only for OLE Connection from OLE string...	

	Library link : ole32.lib
==========================================================*/
class CDB_Connector : public CNDThread
{

protected:

	HRESULT hr ;
	CDataSource _db ;
	LPCOLESTR lpConStr ;

	DWORD dwDBReConnectThreadID ; 
	HANDLE hDBReConnectThread ;

	CPool<CDB_Session>	m_SessionPool ;	
	
	int Run()
	{
		bThreadRun = true;		
		
		
		while(1)
		{			
			hr = ReTryConnect() ;
			if( FAILED(hr) ) 
			{
				Sleep(1000);
				continue ;
			}

			Sleep(3000);
			hr = ReSessionLink() ;
			if( FAILED(hr) )
			{
				printf("** Session link failed\n") ;
				// todo : 여기에 에러처리..
				continue ;
			}

			printf("** Session link success\n") ;
			break ;			
		}
				
		bThreadRun = false ;

		return 0;
	}
	
public:			

	bool bThreadRun;
	CDB_Connector() : m_SessionPool(0), bThreadRun(false)
	{		
	}
	~CDB_Connector() 
	{		
	}

	inline CDB_Session * NewSession()
	{
		CDB_Session * pTemp = new CDB_Session ;		
		if( pTemp )
		{
			HRESULT hr = SessionLink( pTemp->GetSessionPtr() ) ;
			if( FAILED(hr) )
			{
				delete pTemp ;
				pTemp = NULL ;
			}
			else
			{
				m_SessionPool.push_back( pTemp ) ;
			}
			CSession s = *pTemp->GetSessionPtr() ;

		}	
		return pTemp ;
	}

	inline void FreeSession( CDB_Session * p )
	{		
		SessionClose( p->GetSessionPtr() ) ;
		m_SessionPool.pop( p ) ;
		delete p ;
	}	

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

		if(FAILED(hr)) 
		{			
			//DBErrorConsole( hr ) ;
			return false;
		}
		return true ;		

	}

	void release()
	{		
		_db.Close();
		CoUninitialize();	// release Com Library

	}

	HRESULT ReSessionLink()
	{
		HRESULT hr ;
		for( CDB_Session * p = m_SessionPool.GetFirst() ; p != NULL; p = p->pNext )
		{
			SessionClose( p->GetSessionPtr() ) ;
			hr = SessionLink( p->GetSessionPtr() ) ;
			if( FAILED(hr) )
			{
				return hr ;
			}
		}
		return hr ;
	}

	HRESULT ReTryConnect()
	{		
		printf("** DB Reconnecting ...\n") ;
		_db.Close() ;
		HRESULT hr = _db.OpenFromInitializationString( lpConStr ) ;		
		if(FAILED(hr))
		{
			printf("** DB Reconnect failed\n") ;
		}
		else
		{
			printf("** DB Reconnect success\n") ;
		}
		return hr ;
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
			Start() ;
		}	
	}
	

	HRESULT GetHresult() { return hr ; } 
};


/*=========================================================
	OLE DB Template Accessor Commander library
	class CDB_Command
	
	2004.06.03		by. ozzywow  	
==========================================================*/
template <class TAccessor>
class CDB_Command
{
protected:		
	CCommand<CAccessor<TAccessor> ,CRowset > m_command ;	
public:
	inline CDB_Command()
	{
		//m_command.AllocateAccessorMemory(1) ;		
	}
	inline ~CDB_Command()
	{	
	}

	inline HRESULT OpenCommand( CSession session)
	{		
		return  m_command.Open( session) ;
	}

	inline HRESULT OpenCommand( CSession session, CString qry )
	{		
		return  m_command.Open( session, qry) ;
	}

	inline void CloseCommand()
	{		
		m_command.ReleaseCommand() ;
		m_command.Close() ;		
	}

	inline TAccessor * GetAccessor()
	{		
		return static_cast<TAccessor*>(&m_command) ;
	}	

	inline void ResetAccessor()
	{
		memset( static_cast<TAccessor*>(&m_command), 0, sizeof(TAccessor) ) ;
	}

	inline CCommand<CAccessor<TAccessor> ,CRowset > * GetCustomer()
	{
		return &m_command ;
	}

private:
};
