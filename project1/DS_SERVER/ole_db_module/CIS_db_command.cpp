#include "..\stdafx.h"
#include <atldbcli.h>

#include "db_command.h"

/*
#ifdef _CIS_20070326_

void CDBCmd_CIS::CallReConTh()
{ 
	printf( "[WORNING] ACC DB link is disconnected \n" ) ;
	::PrintConsole( "[WORNING] ACC DB link is disconnected \n" ) ;
	m_pDBCon->CallReConTh() ; 
}

bool CDBCmd_CIS::GetCashItemList(BYTE byPageSize, BYTE bySetPage, SHORT shGameServerIdx, char* szAcc, BYTE byPresent) {
	try{
		m_pGetCashItemList->ResetAccessor();

		// set input data
		m_pGetCashItemList->GetAccessor()->byPageSize = byPageSize;
		m_pGetCashItemList->GetAccessor()->bySetPage = bySetPage;
		m_pGetCashItemList->GetAccessor()->shGameServerIdx = shGameServerIdx;
		memcpy(m_pGetCashItemList->GetAccessor()->szAcc, szAcc, en_max_lil);
		m_pGetCashItemList->GetAccessor()->szAcc[en_max_lil] = NULL;
		m_pGetCashItemList->GetAccessor()->byPresent = byPresent;
		m_pGetCashItemList->GetAccessor()->iTotalRec = 0;

		// command
		HRESULT hRes = m_pGetCashItemList->OpenCommand(*m_session);
		if(FAILED(hRes)) {
#ifdef _DEBUG
			AtlTraceErrorRecords(hRes) ;
#endif
			DBErrorOutput(hRes) ;
			if(hRes == E_FAIL) CallReConTh();	
		}
		
		//::PrintConsole("iVar1=%d, iVar2=%d, iRet=%d \n",
		//	m_pTest->GetAccessor()->iVar1,
		//	m_pTest->GetAccessor()->iVar2,
		//	m_pTest->GetAccessor()->iRet);

		m_pGetCashItemList->CloseCommand();

		if(FAILED(hRes)) {
			DBErrorOutput(hRes);
			if(hRes == E_FAIL) CallReConTh();
			return false;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		m_pGetCashItemList->CloseCommand();
		return false;
	}

	return true;
}


#endif //_CIS_20070326_
*/