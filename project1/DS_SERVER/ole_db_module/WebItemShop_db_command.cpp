#include "..\stdafx.h"
#include <atldbcli.h>

#include "db_command.h"

#ifdef ITEMSHOP_PATCH_200607
//////////////////////////////////////////////////////////////////////////¡å¡å¡å
// 2006.06.02 Molla, Nexon Web Shop
void CDBCommand_WebShop::CallReConTh()
{ 
	printf( "[WARNING] CIS DB link is disconnected \n" ) ;
	::PrintConsole( "[WARNING] CIS DB link is disconnected \n" ) ;
	m_pDBCon->CallReConTh() ; 
}

BOOL CDBCommand_WebShop::Test() {
	try{
		m_pTest->ResetAccessor();

		// set input data
		m_pTest->GetAccessor()->iRet = 0;
		m_pTest->GetAccessor()->iVar1 = 100;
		m_pTest->GetAccessor()->iVar2 = 0;

		// command
		HRESULT hRes = m_pTest->OpenCommand(*m_session);
		if(FAILED(hRes)) {
#ifdef _DEBUG
			AtlTraceErrorRecords(hRes) ;
#endif
			DBErrorOutput(hRes) ;
			if(hRes == E_FAIL) CallReConTh();	
		}
		
		::PrintConsole("iVar1=%d, iVar2=%d, iRet=%d \n",
			m_pTest->GetAccessor()->iVar1,
			m_pTest->GetAccessor()->iVar2,
			m_pTest->GetAccessor()->iRet);

		m_pTest->CloseCommand();

		if(FAILED(hRes)) {
			DBErrorOutput(hRes);
			if(hRes == E_FAIL) CallReConTh();
			return FALSE;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		m_pTest->CloseCommand();
		return FALSE;
	}

	return TRUE;
}

BOOL CDBCommand_WebShop::Test2() {
	try{
		CString strQuery = "SELECT * FROM WebItemShop.dbo.tTest";

		HRESULT hRes = m_pTest2->OpenCommand(*m_session, strQuery);
		if(FAILED(hRes)) {
#ifdef _DEBUG
			AtlTraceErrorRecords(hRes) ;
#endif
			DBErrorOutput(hRes) ;
			if(hRes == E_FAIL) CallReConTh();	
		}

		int a, b, c;
		a = b = c = 0;
		while(m_pTest2->GetCustomer()->MoveNext() == S_OK) {
			a = m_pTest2->GetAccessor()->iIdx;
			b = m_pTest2->GetAccessor()->iColumn1;
			c = m_pTest2->GetAccessor()->iColumn2;
		}

		m_pTest2->CloseCommand();
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		m_pTest->CloseCommand();
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////¡ã¡ã¡ã
#endif // ITEMSHOP_PATCH_200607