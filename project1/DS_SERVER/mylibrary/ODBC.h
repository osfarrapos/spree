#pragma once

#ifndef LINK_ODBC32_LIB
#define LINK_ODBC32_LIB
#pragma comment(lib, "odbc32.lib")
#endif

/*
#ifndef ODBC_EXPORTS
#ifndef _ODBC_LIB_
#define _ODBC_LIB_
#ifdef _UNICODE
#pragma comment(lib, "uODBC.lib")
#else
#pragma comment(lib, "mODBC.lib")
#endif
#endif
#endif
*/

class COdbcConnectInfo {
public:
	enum	{
		_BUF_SIZE = 256
	};

public:
	TCHAR	pszConStr[_BUF_SIZE];
	TCHAR	pszId[_BUF_SIZE];
	TCHAR	pszPass[_BUF_SIZE];

public:
	COdbcConnectInfo() {
		memset(pszConStr, 0, _BUF_SIZE*sizeof(TCHAR));
		memset(pszId, 0, _BUF_SIZE*sizeof(TCHAR));
		memset(pszPass, 0, _BUF_SIZE*sizeof(TCHAR));
	}
	~COdbcConnectInfo() {}
};

/*
#ifdef ODBC_EXPORTS
class __declspec(dllexport) COdbc
#else
class __declspec(dllimport) COdbc
#endif
{
*/
class COdbc {
public:
	enum {
		MAX_COLUMN = 100,
	};

	enum CQUERY_RESULT {
		CQUERY_NULL = -1,
		CQUERY_EOF = -2,
		CQUERY_NO_COL = -3,
		CQUERY_ERROR = -4
	};

private:
	SQLHENV			hEnv;	// 환경 핸들
	SQLHDBC			hDbc;	// 연결 핸들
	SQLHSTMT			hStmt;	// 명령 핸들

	SQLINTEGER		iAffectedCnt;	// 영향받은 레코드 개수
	SQLSMALLINT	siCol;	// 컬럼 개수

	SQLINTEGER		iColInfo[MAX_COLUMN];	// 최대 컬럼 개수는 100개

	COdbcConnectInfo	ConnectInfo;

public:
	COdbc();
	~COdbc();

public:
	BOOL	Connect();
	BOOL	Connect(COdbcConnectInfo info);
	BOOL	Connect(LPTSTR pszConStr, LPTSTR pszId, LPTSTR pszPass);
	BOOL	Connect(HWND hWnd, LPTSTR pszConStr);
	void	Disconnect();
	BOOL	Exec();
	BOOL	Exec(LPTSTR pszSql);
	void	Clear(); // 커서 해제 및 언바인딩

	int GetResultColCnt() {
		SQLNumResultCols(hStmt, &siCol);
		return (int)siCol;
	}

	int GetResultRowCnt() {
		SQLRowCount(hStmt, &iAffectedCnt);
		return (int)iAffectedCnt;
	}

	BOOL BindParam(SQLUSMALLINT usiParamNum, SQLSMALLINT siInpuOutputType, SQLSMALLINT siCTypeID, SQLSMALLINT siSQLTypeID, SQLUINTEGER uiColumnSize, SQLPOINTER pVal) {
		SQLRETURN ret = SQLBindParameter(hStmt, usiParamNum, siInpuOutputType, siCTypeID, siSQLTypeID, uiColumnSize, 0, pVal, 0, &iColInfo[usiParamNum-1]);
		if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			return TRUE;
		else {
			Logout(_T("#ERR %s(%d): SQLBindParameter() 실패"), _T(__FILE__), __LINE__);
			return FALSE;
		}
	}

	BOOL BindCol(SQLUSMALLINT usiColNum, SQLSMALLINT siCTypeID, SQLPOINTER pVal, SQLLEN iBufLen) {
		SQLRETURN ret = SQLBindCol(hStmt, usiColNum, siCTypeID, pVal, iBufLen, &iColInfo[usiColNum-1]);
		if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			return TRUE;
		else {
			Logout(_T("#ERR %s(%d): SQLBindCol() 실패"), _T(__FILE__), __LINE__);
			return FALSE;
		}
	}

	BOOL Prepare(SQLTCHAR* pszQuery) {
		SQLRETURN ret = SQLPrepare(hStmt, pszQuery, SQL_NTS);
		if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			return TRUE;
		else {
			Logout(_T("#ERR %s(%d): SQLPrepare() 실패"), _T(__FILE__), __LINE__);
			return FALSE;
		}
	}

	BOOL Fetch() {
		if(SQL_NO_DATA == SQLFetch(hStmt)) return FALSE;
		else return TRUE;
	}

	BOOL MoreResult() {
		if(SQL_SUCCESS == SQLMoreResults(hStmt)) return TRUE;
		else return FALSE;
	}

public:
	LPCTSTR GetDSN() { return (LPCTSTR)ConnectInfo.pszConStr; }
	LPCTSTR GetID() { return (LPCTSTR)ConnectInfo.pszId; }
	LPCTSTR GetPass() { return (LPCTSTR)ConnectInfo.pszPass; }

private:
	enum RECOVER_PARAM {
		RECOVER_HANDLE,
		RECOVER_CONNECT
	};

	BOOL	_Recover(RECOVER_PARAM param);
	void		_GetDiagRec( int sql_handle );
};
