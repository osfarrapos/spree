#include "db_link.h"

class _j_dbComExeCute
{
	
protected:

	// OLE DB Connection Object
	CDB_Connector * m_pDBCon ;
	CSession m_Session ;

	// Command table
	CCommand<CAccessor<C_Insert_ServerState>>	m_InsertServerState ;	
	
public:	
	_j_dbComExeCute( CDB_Connector * pDBCon ) 
	{
		m_pDBCon = pDBCon ;
	}

	// Global
	bool Init() ;
	bool SetSession(CSession * pSession) ;

	// Execute Function
	C_Insert_ServerState m_InsertServerState ;
	


};