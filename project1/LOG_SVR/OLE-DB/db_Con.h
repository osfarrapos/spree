#pragma	   once

#include <objbase.h>
#include <atldbcli.h>
#include <oledb.h>

/*
	OLE DB Connection Module
	
	2004.06.03		ozzywow
*/


class _j_DB_Con
{

protected:
	HRESULT hr ;
	CDataSource _db ;

public:	
	
	bool init( LPCOLESTR ConStr );					// 초기화(연결)
	void release();									// 해제	(끊기)	
	HRESULT GetHresult() { return hr ; } 
	HRESULT SessionLink( CSession * Session ) ;		// 세션 연결	
};
