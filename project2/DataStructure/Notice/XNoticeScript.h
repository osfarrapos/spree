// _XNoticeScript.h: interface for the _XNoticeScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX__XNOTICESCRIPT_H__8BE9A109_D653_40B4_B96D_F3DF8847C527__INCLUDED_)
#define AFX__XNOTICESCRIPT_H__8BE9A109_D653_40B4_B96D_F3DF8847C527__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>
#include <stdio.h>
#include <crtdbg.h>

//#include "Xkernel.h"

class _XNoticeScript  
{
public:
	_XNoticeScript();
	~_XNoticeScript();

	BOOL Load(  LPTSTR szFileName );
	void Destroy();

private:
	BOOL ImportTXT( LPTSTR szFileName );
	BOOL GetDataFromFileL( LPTSTR pString = NULL );
	BOOL GetMaxDataCountFromFile( LPTSTR pString = NULL );
	BOOL CheckCommentFromFile( LPTSTR pString = NULL);
	BOOL OpenStream( LPTSTR szFileName );
	BOOL ErrorCloseStream( LPTSTR pMsg );

	struct sInfo
	{
		int nMsgID;
		int nStartTime;
		int nInterval;
		int nPrintCount;
		BOOL bLargeFont;

		int GetMsgID() { return nMsgID; }
		int GetStartTime() { return nStartTime; }
		int GetInterval() { return nInterval; }
		int GetPrintCount() { return nPrintCount; }
		BOOL GetLargeFont() { return bLargeFont; }		
	};

public:
	int GetMaxInfo() { return m_nMaxInfo; }
	sInfo GetInfo( int nID ) { return m_pInfo[nID]; }
	sInfo* GetInfo() { return m_pInfo; }

private:	
	sInfo*		m_pInfo;
	FILE*		m_pStream;

	int			m_nMaxInfo;

	BOOL	m_bFirst;

};

#endif // !defined(AFX__XNOTICESCRIPT_H__8BE9A109_D653_40B4_B96D_F3DF8847C527__INCLUDED_)
