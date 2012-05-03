#include "..\stdafx.h"
#include "../myServer/FileLog.h"
#include "./ODBC_.h"

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
		Logout(_T("COdbc::COdbc: #ERR 환경 핸들 생성 실패"));
	}

	ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		Logout(_T("COdbc::COdbc: #ERR 환경 핸들 속성 설정 실패"));
	}
}

COdbc::~COdbc() {
	if(hStmt != NULL) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = NULL;
	}

	if(hDbc != NULL) {
		SQLDisconnect(hDbc);
		Logout(_T("COdbc::~COdbc: 디비 연결 끊기 성공"));
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		hDbc = NULL;
	}

	if(hEnv != NULL) {
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		hEnv = NULL;
	}
}

BOOL COdbc::Connect() {
	return this->Connect(ConnectInfo.pszConStr, ConnectInfo.pszId, ConnectInfo.pszPass);
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
		Logout(_T("COdbc::Connect: ODBC 접속 정보 백업"));
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
			Logout(_T("COdbc::Connect: #ERR 연결 핸들 생성 실패"));
		}
	}
	Logout(_T("COdbc::Connect: 디비 연결 핸들 생성 성공"));

	SQLSetConnectAttr(hDbc, 0, (void*)SQL_LOGIN_TIMEOUT, 0);
	ret = SQLConnect(hDbc, (SQLTCHAR*)pszConStr, SQL_NTS, (SQLTCHAR*)pszId, SQL_NTS, (SQLTCHAR*)pszPass, SQL_NTS);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		Logout(_T("COdbc::Connect: #ERR 디비 연결 실패"));
		return FALSE;
	}
	Logout(_T("COdbc::Connect: 디비 연결 성공"));

	// 명령 핸들 생성
	if(hStmt != NULL) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = NULL;
	}
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		Logout(_T("COdbc::Connect: #ERR 명령 핸들 생성 실패"));
		return FALSE;
	}
	Logout(_T("COdbc::Connect: 디비 명령 핸들 생성 성공"));
	Logout(_T("COdbc::Connect: ODBC 모듈 사용 준비 완료"));
	return TRUE;
}

void COdbc::Disconnect() {
	Logout(_T("COdbc::Disconnect: 디비 연결 종료 시도 DSN=%s, ID=%s"), ConnectInfo.pszConStr, ConnectInfo.pszId);
	//memset(&ConnectInfo, 0, sizeof(COdbcConnectInfo)); // 재연결 시도할 때, 필요한 정보

	if(hStmt != NULL) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = NULL;
		Logout(_T("COdbc::Disconnect: 디비 명령 핸들 삭제"));
	}
	if(hDbc != NULL) {
		SQLDisconnect(hDbc);
		Logout(_T("COdbc::Disconnect: 디비 연결 종료"));
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		hDbc = NULL;
		Logout(_T("COdbc::Disconnect: 디비 연결 핸들 삭제"));
	}
	
	Logout(_T("COdbc::Connect: ODBC 모듈 제거 완료"));
}

BOOL COdbc::Exec() {
	static BOOL bStatus = TRUE;	// ODBC 모듈 Connect 상태
	static int iTimeInterval = 0;		// 일정 시간 후, 재연결 시도
	if(bStatus == FALSE) {
		// 연결이 끊어졌을 경우, 일정 시간 후 연결을 재시도하도록..
		if(GetTickCount() - iTimeInterval < 600000/*10분=600초*/) {
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
		_GetDiagRec();

		/*
		if(!_Recover(RECOVER_HANDLE) || !Exec()) {
			if(!_Recover(RECOVER_CONNECT) || !Exec()) {
				bStatus = FALSE;
				iTimeInterval = GetTickCount();
				return FALSE;
			}
		}
		*/
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
		if(GetTickCount() - iTimeInterval < 600000/*10분=600초*/) {
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
		_GetDiagRec();

		/*
		if(!_Recover(RECOVER_HANDLE) || !Exec(pszSql)) {
			if(!_Recover(RECOVER_CONNECT) || !Exec(pszSql)) {
				bStatus = FALSE;
				iTimeInterval = GetTickCount();
				return FALSE;
			}
		}
		*/
		if(!_Recover(RECOVER_CONNECT) || !Exec(pszSql)) {
			bStatus = FALSE;
			iTimeInterval = GetTickCount();
			return FALSE;
		}
		return TRUE;
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
		Logout(_T("COdbc::_Recover: 명령 핸들 복구 시도함"));
		if(hStmt != NULL) {
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			hStmt = NULL;
		}
		ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
		if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			Logout(_T("COdbc::_Recover: #ERR 명령 핸들 생성 실패"));
			param = RECOVER_CONNECT;
			goto labelRecover;
		}
		return TRUE;

	case RECOVER_CONNECT:
		Logout(_T("COdbc::_Recover: 연결 복구 시도함"));
		return this->Connect();
	}

	return TRUE;
}

void COdbc::_GetDiagRec() {
	SQLRETURN SqlRet;
	SQLINTEGER iNativeError;
	SQLTCHAR pszSqlState[8], pszMsg[1024];
	SQLSMALLINT siMsgLen;

	SqlRet = SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, pszSqlState, &iNativeError, pszMsg, sizeof(pszMsg), &siMsgLen);
	if(iNativeError != 0xCCCCCCCC) {
		Logout(_T("COdbc::_Recover: #ERR SQLSTATE:%s, NativeError=%d, 진단정보:%s"), (LPTSTR)pszSqlState, iNativeError, (LPTSTR)pszMsg);
	}
	/*for(int iRecCnt = 1; ; iRecCnt++) {
	SqlRet = SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, iRecCnt, pszSqlState, &iNativeError, pszMsg, sizeof(pszMsg), &siMsgLen);
	Logout(_T("COdbc::_Recover: #ERR SQLSTATE:%s, NativeError=%d, 진단정보:%s"), (LPTSTR)pszSqlState, iNativeError, (LPTSTR)pszMsg);
	if(SQL_NO_DATA == SqlRet) break;
	}*/
}
