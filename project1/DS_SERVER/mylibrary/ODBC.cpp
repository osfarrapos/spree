#include <Windows.h>
#include <tchar.h>
#include <sql.h>
#include <sqlext.h>
#include "./FileLog.h"
#include "./ODBC.h"

COdbc::COdbc() {
	hEnv = NULL;
	hDbc = NULL;
	hStmt = NULL;

	iAffectedCnt = -1;
	siCol = 0;

	for(int i = 0; i < MAX_COLUMN; i++) {
		iColInfo[i] = 0;
	}

	// 환경 핸들 생성
	SQLRETURN ret;
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		Logout(_T("COdbc::COdbc: #ERR allocate env' handle"));
	}

	ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		_GetDiagRec( SQL_HANDLE_ENV );
		Logout(_T("COdbc::COdbc: #ERR SetEnvAttr Env' Property"));
	}
}

COdbc::~COdbc() {
	if(hStmt != NULL) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = NULL;
	}

	if(hDbc != NULL) {
		SQLDisconnect(hDbc);
		Logout(_T("COdbc::~COdbc: Disconnect DB Connection."));
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		hDbc = NULL;
	}

	if(hEnv != NULL) {
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		hEnv = NULL;
	}
}

BOOL COdbc::Connect() {
	if( ConnectInfo.pszId[0] )
		return this->Connect(ConnectInfo.pszConStr, ConnectInfo.pszId, ConnectInfo.pszPass);
	else
		return this->Connect( NULL, ConnectInfo.pszConStr );
}

BOOL COdbc::Connect(COdbcConnectInfo info) {
	memcpy(&ConnectInfo, &info, sizeof(COdbcConnectInfo));
	return this->Connect(ConnectInfo.pszConStr, ConnectInfo.pszId, ConnectInfo.pszPass);
}

BOOL COdbc::Connect(LPTSTR pszConStr, LPTSTR pszId, LPTSTR pszPass) {
	this->Disconnect();

	if(pszConStr[0] == NULL || pszId[0] == NULL || pszPass[0] == NULL) {
		Logout(_T("COdbc::Connect: Param is NULL. pszConStr=%s, pszId=%s, pszPass=%s"), pszConStr, pszId, pszPass);
		return FALSE;
	}
	if(0 == lstrlen(ConnectInfo.pszConStr)) {
		Logout(_T("COdbc::Connect: ODBC Backup connection information."));
		lstrcpy(ConnectInfo.pszConStr, pszConStr);
		lstrcpy(ConnectInfo.pszId, pszId);
		lstrcpy(ConnectInfo.pszPass, pszPass);
	}
	Logout(_T("COdbc::Connect: DSN=%s, ID=%s"), pszConStr, pszId);

	SQLRETURN ret;

	// 연결 핸들 생성
	if(hDbc == NULL) {
		ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
		if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			Logout(_T("COdbc::Connect: #ERR Allocate DB Connection Handle"));
			return FALSE;
		}
	}
	Logout(_T("COdbc::Connect: Success - Allocate DB Connection Handle"));

	SQLSetConnectAttr(hDbc, 0, (void*)SQL_LOGIN_TIMEOUT, 0);
	ret = SQLConnect(hDbc, (SQLTCHAR*)pszConStr, SQL_NTS, (SQLTCHAR*)pszId, SQL_NTS, (SQLTCHAR*)pszPass, SQL_NTS);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		_GetDiagRec( SQL_HANDLE_DBC );
		Logout(_T("COdbc::Connect: #ERR Connect DB"));
		return FALSE;
	}
	Logout(_T("COdbc::Connect: Success DB Connect"));

	// 명령 핸들 생성
	if(hStmt != NULL) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = NULL;
	}
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		Logout(_T("COdbc::Connect: #ERR Allocate Statement Handle"));
		return FALSE;
	}
	Logout(_T("COdbc::Connect: Ready to ODBC!!"));
	return TRUE;
}

BOOL COdbc::Connect(HWND hWnd, LPTSTR pszConStr) {
	this->Disconnect();

	if(pszConStr[0] == NULL) {
		Logout(_T("COdbc::Connect: pszConStr is NULL"));
		return FALSE;
	}
	if(ConnectInfo.pszConStr[0] == NULL) {
		Logout(_T("COdbc::Connect: ODBC Backup connection information."));
		//ConnectInfo.hWnd = hWnd;
		lstrcpy(ConnectInfo.pszConStr, pszConStr);
	}
	Logout(_T("COdbc::Connect: ConStr=%s"), pszConStr);

	SQLRETURN ret;

	// 연결 핸들 생성
	if(hDbc == NULL) {
		ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
		if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			Logout(_T("COdbc::Connect: #ERR Allocate DB Connection Handle"));
			return FALSE;
		}
	}
	Logout(_T("COdbc::Connect: Success - Allocate DB Connection Handle"));

	SQLTCHAR			pszConnStrOut[MAX_PATH] = {0};
	SQLSMALLINT	cbConnStrOut = 0;
	SQLSetConnectAttr(hDbc, 0, (void*)SQL_LOGIN_TIMEOUT, 0);
	ret = SQLDriverConnect(hDbc, hWnd, (SQLTCHAR*)pszConStr, SQL_NTS, pszConnStrOut, MAX_PATH, &cbConnStrOut, SQL_DRIVER_NOPROMPT);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		_GetDiagRec( SQL_HANDLE_DBC );
		Logout(_T("COdbc::Connect: #ERR Connect DB"));
		return FALSE;
	}
	Logout(_T("COdbc::Connect: Success DB Connect, pszConnStrOut=%s"), pszConnStrOut);

	// 명령 핸들 생성
	if(hStmt != NULL) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = NULL;
	}
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		Logout(_T("COdbc::Connect: #ERR Allocate Statement Handle"));
		return FALSE;
	}
	Logout(_T("COdbc::Connect: Ready to ODBC(2)!!"));
	return TRUE;
}

void COdbc::Disconnect() {
	Logout(_T("COdbc::Disconnect: Try Disconnect ODBC DSN=%s, ID=%s"), ConnectInfo.pszConStr, ConnectInfo.pszId);
	//memset(&ConnectInfo, 0, sizeof(COdbcConnectInfo)); // 재연결 시도할 때, 필요한 정보

	if(hStmt != NULL) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = NULL;
		Logout(_T("COdbc::Disconnect: Free Statement Handle"));
	}
	if(hDbc != NULL) {
		SQLDisconnect(hDbc);
		Logout(_T("COdbc::Disconnect: Disconnect DB"));
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		hDbc = NULL;
		Logout(_T("COdbc::Disconnect: Free DB Connection Handle"));
	}

	Logout(_T("COdbc::Connect: Free All ODBC Handle"));
}


/*
BOOL COdbc::Exec() {
	static BOOL bStatus = TRUE;	// ODBC 모듈 Connect 상태
	static int iTimeInterval = 0;		// 일정 시간 후, 재연결 시도
	if(bStatus == FALSE) {
		// 연결이 끊어졌을 경우, 일정 시간 후 연결을 재시도하도록..
		if(GetTickCount() - iTimeInterval < 600000) {	// 10분=600초
			return FALSE;
		}
		else {
			bStatus = TRUE;
		}
	}

	SQLRETURN ret = SQLExecute(hStmt);
	if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO || ret == SQL_NO_DATA)
		return TRUE;
	else {
		_GetDiagRec( SQL_HANDLE_STMT );

		if(!_Recover(RECOVER_CONNECT) || !Exec()) {
			bStatus = FALSE;
			iTimeInterval = GetTickCount();
			return FALSE;
		}
		return TRUE;
	}
}

// SQL문을 실행한다. 실패시 진단 정보를 출력하고 FALSE를 리턴한다.
BOOL COdbc::Exec(LPTSTR pszSql) {
	static BOOL bStatus = TRUE;	// ODBC 모듈 Connect 상태
	static int iTimeInterval = 0;		// 일정 시간 후, 재연결 시도
	if(bStatus == FALSE) {
		// 연결이 끊어졌을 경우, 일정 시간 후 연결을 재시도하도록..
		if(GetTickCount() - iTimeInterval < 600000) { // 10분=600초
			return FALSE;
		}
		else {
			bStatus = TRUE;
		}
	}

	// SQL_NO_DATA를 리턴한 경우도 성공으로 취급한다. 
	// 이 경우 Fetch에서 EOF를 리턴하도록 되어 있기 때문에 진단 정보를 출력할 필요는 없다.
	SQLRETURN ret = SQLExecDirect(hStmt, (SQLTCHAR*)pszSql, SQL_NTS);
	if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO || ret == SQL_NO_DATA)
		return TRUE;
	else {
		_GetDiagRec( SQL_HANDLE_STMT );

		if(!_Recover(RECOVER_CONNECT) || !Exec(pszSql)) {
			bStatus = FALSE;
			iTimeInterval = GetTickCount();
			return FALSE;
		}
		return TRUE;
	}
}
*/

BOOL COdbc::Exec() {
	SQLRETURN ret = SQLExecute(hStmt);
	if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO || ret == SQL_NO_DATA)
		return TRUE;
	else {
		_GetDiagRec( SQL_HANDLE_STMT );

		if( _Recover(RECOVER_CONNECT) )
		{
			ret = SQLExecute(hStmt);
			if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO || ret == SQL_NO_DATA)
				return TRUE;
			else
				return FALSE;
		}
		else
			return FALSE;
	}
}

// SQL문을 실행한다. 실패시 진단 정보를 출력하고 FALSE를 리턴한다.
BOOL COdbc::Exec(LPTSTR pszSql) {
	// SQL_NO_DATA를 리턴한 경우도 성공으로 취급한다. 
	// 이 경우 Fetch에서 EOF를 리턴하도록 되어 있기 때문에 진단 정보를 출력할 필요는 없다.
	SQLRETURN ret = SQLExecDirect(hStmt, (SQLTCHAR*)pszSql, SQL_NTS);
	if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO || ret == SQL_NO_DATA)
		return TRUE;
	else {
		_GetDiagRec( SQL_HANDLE_STMT );

		if( _Recover(RECOVER_CONNECT) )
		{
			ret = SQLExecDirect(hStmt, (SQLTCHAR*)pszSql, SQL_NTS);
			if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO || ret == SQL_NO_DATA)
				return TRUE;
			else
				return FALSE;
		}
		else
			return FALSE;
	}
}

// 커서를 닫고 바인딩 정보를 해제한다.
void COdbc::Clear() {
	SQLCloseCursor(hStmt);
	SQLFreeStmt(hStmt, SQL_UNBIND);
}

BOOL COdbc::_Recover(RECOVER_PARAM param) {
	SQLRETURN ret;

labelRecover:
	switch(param) {
	case RECOVER_HANDLE:
		Logout(_T("COdbc::_Recover: Try recover statement handle"));
		if(hStmt != NULL) {
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			hStmt = NULL;
		}
		ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
		if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			Logout(_T("COdbc::_Recover: #ERR fail to allocate statement handle"));
			param = RECOVER_CONNECT;
			goto labelRecover;
		}
		return TRUE;

	case RECOVER_CONNECT:
		Logout(_T("COdbc::_Recover: Try connect db(ODBC)"));
		return this->Connect();
	}

	return TRUE;
}

void COdbc::_GetDiagRec( int sql_handle ) {
	SQLINTEGER iNativeError;
	SQLTCHAR pszSqlState[8], pszMsg[SQL_MAX_MESSAGE_LENGTH];
	SQLSMALLINT siMsgLen, i;

	switch( sql_handle )
	{
	case SQL_HANDLE_ENV:
		i = 1;
		while( SQL_NO_DATA != SQLGetDiagRec(SQL_HANDLE_ENV, hEnv, i, pszSqlState, &iNativeError, pszMsg, sizeof(pszMsg), &siMsgLen) && i <= 2 )
		{
			Logout(_T("COdbc::_Recover(%d): #ERR SQL Env:%s, NativeError=%d, %s"), i, (LPTSTR)pszSqlState, iNativeError, (LPTSTR)pszMsg);
			++i;
		}
		break;
	case SQL_HANDLE_DBC:
		i = 1;
		while( SQL_NO_DATA != SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, i, pszSqlState, &iNativeError, pszMsg, sizeof(pszMsg), &siMsgLen) && i <= 2 )
		{
			Logout(_T("COdbc::_Recover(%d): #ERR SQL Dbc:%s, NativeError=%d, %s"), i, (LPTSTR)pszSqlState, iNativeError, (LPTSTR)pszMsg);
			++i;
		}
		break;
	case SQL_HANDLE_STMT:
		i = 1;
		while( SQL_NO_DATA != SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, i, pszSqlState, &iNativeError, pszMsg, sizeof(pszMsg), &siMsgLen) && i <= 2 )
		{
			Logout(_T("COdbc::_Recover(%d): #ERR SQL State:%s, NativeError=%d, %s"), (LPTSTR)pszSqlState, iNativeError, (LPTSTR)pszMsg);
			++i;
		}
		break;
	}
}
