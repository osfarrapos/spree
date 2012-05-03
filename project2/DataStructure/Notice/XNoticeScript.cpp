// _XNoticeScript.cpp: implementation of the _XNoticeScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XNoticeScript.h"
#include "EODEXTDEF_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XNoticeScript::_XNoticeScript() : m_nMaxInfo(0), m_pInfo(NULL), m_bFirst(TRUE)
{

}

_XNoticeScript::~_XNoticeScript()
{

}


void _XNoticeScript::Destroy()
{
	m_bFirst = TRUE;
	m_nMaxInfo = 0;
	m_pStream = NULL;

	if(m_pInfo)
        delete [] m_pInfo;
}

BOOL _XNoticeScript::Load( LPTSTR szFileName )
{
	if( !m_bFirst )
		return FALSE;

	m_bFirst = FALSE;

	BOOL bResult;
	
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];									

	_splitpath( szFileName, drive, dir, fname, ext );
	ext[4] = 0;

	//확장자가 txt 인지 확인
	if( stricmp( ext, ".txt" ) )
		return FALSE;

	bResult = ImportTXT( szFileName );
	g_ScriptArchive.ClosePackage();
	m_pStream = NULL;

	return bResult;
}

BOOL _XNoticeScript::ImportTXT( LPTSTR szFileName )
{
	TCHAR pBuffer[256] = {0,};

	if(OpenStream( szFileName ) == -1)
		return FALSE;

	//총 갯수가 몇개인가 확인후  scriptpackage 를 닫고 m_pStream 초기화도 같이 
	GetMaxDataCountFromFile( "_XM_EOS" );

	if(OpenStream( szFileName ) == -1)
		return FALSE;

	//총 갯수만큼 데이터 정보를 생성한다
	if( !m_pInfo && m_nMaxInfo )
	{
        m_pInfo = new sInfo[m_nMaxInfo];
		ZeroMemory( m_pInfo, sizeof( sInfo ) * m_nMaxInfo ); 
	}

	if( GetDataFromFileL( _T("_XM_NOTICE")) == -1 )
		return FALSE;

	for( int i = 0; i < m_nMaxInfo; i++ )
	{
		fgets( pBuffer, 256, m_pStream ); //한줄씩 읽는다

		//데이터 정보인지 검사한다
		if(CheckCommentFromFile( pBuffer )) 
		{
			i--;
			continue;
		}

		//각각의 데이터를 저장한다
		sscanf( pBuffer, "%d%d%d%d%d", &m_pInfo[i].nMsgID, 
								     &m_pInfo[i].nStartTime, 
								     &m_pInfo[i].nInterval, 
								     &m_pInfo[i].nPrintCount,
									 &m_pInfo[i].bLargeFont);

	}

	if( GetDataFromFileL( _T("_XM_EOS")) == -1 )
		return FALSE;

	return TRUE;
}

//라인 한줄씩 검색한다
BOOL _XNoticeScript::GetDataFromFileL( LPTSTR pString ) 
{
	TCHAR pBuffer[256] = {0,};
	int nSize = strlen( pString );

	while( !feof( m_pStream ) )
	{
		fgets( pBuffer, 256, m_pStream );		

		//찾는 문자열이 있는지에 대한 검색
		if( !strncmp(pBuffer, pString, nSize ) ) 
		{
			return TRUE;
		}
	}
	return ErrorCloseStream( "전체파일에서 검색할수 없습니다");	
}

BOOL _XNoticeScript::GetMaxDataCountFromFile( LPTSTR pString )
{
	int nCount = 0;
	TCHAR pBuffer[256] = {0,};

	while( !feof( m_pStream ) ) //파일의 끝까지 검색
	{
		pBuffer[0] = 0;
		fgets( pBuffer, 256, m_pStream );		

		if( !strncmp( pBuffer, pString, 7 ) )
		{
			m_nMaxInfo = nCount; //END 를 만났을때까지에 전체 갯수를저장			
			return TRUE;
		}

		// 한줄마다 유효한 데이터 인지 검색한다
		if( CheckCommentFromFile(pBuffer ) )
		{
			continue;
		}

		//유효한데이터의 갯수
		nCount++;
	}

	return FALSE;
}

BOOL _XNoticeScript::CheckCommentFromFile( LPTSTR pString )
{
	// ; # 엔터나 공백 인줄인 경우는 읽지않는다..	
	if( !strncmp( pString, ";", 1 ) || !strncmp( pString, "_", 1 ) || pString[0] == 10 || pString[0] == 13 )
		return TRUE;

	return FALSE;
}

BOOL _XNoticeScript::OpenStream( LPTSTR szFileName )
{
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	m_pStream = g_ScriptArchive.GetPackedFile(szFileName);

	if ( m_pStream == NULL)
	{
		return ErrorCloseStream( "파일을 찾지 못했습니다.");				
	}	

	return TRUE;
}

BOOL _XNoticeScript::ErrorCloseStream( LPTSTR pMsg )
{
	if( pMsg != NULL )
	{
		TCHAR szBuffer[256];
		memset(szBuffer, 0, sizeof(szBuffer));

		_snprintf(szBuffer, sizeof(szBuffer), "[File: %s][Line: %d]\n[Note : %s]", __FILE__, __LINE__, pMsg);	
		MessageBox(NULL, szBuffer, "ERROR", MB_ICONERROR);	

	}

	return -1;
}