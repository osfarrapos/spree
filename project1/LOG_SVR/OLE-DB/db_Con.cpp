#include "../stdafx.h"
#include "db_Con.h"

/*
	OLE DB Connection Module
	
	2004.06.03		ozzywow
*/

bool _j_DB_Con::init(LPCOLESTR ConStr) /* = OLESTR(str) */
{	
	// Initialize the OLE COM Library (if STM Model use CoInisialize )
	if (CoInitializeEx(NULL, COINIT_MULTITHREADED) != S_OK)	// MTM Model Com Lib Initialize
		return false;	

	// DB Connect	
	hr = _db.OpenFromInitializationString( ConStr );	
	if(FAILED(hr))
		return false;
	return true ;		
	
}

void _j_DB_Con::release()
{
	_db.Close();
	CoUninitialize();	// release Com Library
	
}


HRESULT _j_DB_Con::SessionLink(CSession * pSession )
{
	//Session open			--Ninedragons
	return pSession->Open(_db);	
}

