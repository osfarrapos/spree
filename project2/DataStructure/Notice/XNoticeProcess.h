// XWindow_NoticeProcess.h: interface for the XWindow_NoticeProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX__XNOTICEPROCESS_H__3B906D3A_0CAB_4068_B77F_91A948E6EE53__INCLUDED_)
#define AFX__XNOTICEPROCESS_H__3B906D3A_0CAB_4068_B77F_91A948E6EE53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>
#include <vector>
using namespace std;

#include "XNoticeScript.h"

class _XNoticeProcess  
{
public:
	static _XNoticeProcess*  GetInstance();
	~_XNoticeProcess();

	bool SetNoticeProfile( int nMsgType, LPTSTR pMsg, int nMinStartTime, int nMinInterval, int nPrintCount, BOOL bLargeFont ); //메시지 셋팅 시간은 분단위...
	bool SetNoticeProfile( LPTSTR szFileName  ); //파일
	void ModifyNoticeProfile( int nMsgType, LPTSTR pMsg = NULL, int nMinStartTime = -1, int nMinInterval = -1 ); // 지정된 숫자에 정보들을 수정
	void Update(float fAccumTime);
	void Destroy();

	void CompulsionMessage( int nID ); //현재 메시지 강제로 출력

	inline double GetCurrentTime() { return m_dCurrnetTime; };  //현재 프로세스가 진행된 시간을 얻어온다
	
private:
	_XNoticeProcess();

	struct sMsgInfo
	{
		int nMsgID;
		int nIntervalTime;
		int nStartTime;
		int nPrintCount;
		bool bPrint;
		BOOL bLargeFont;

		sMsgInfo()
		{
			nMsgID = -1;
			nIntervalTime = 0;
			nStartTime = 0;
			nPrintCount = -1;
			bPrint = true;
			bLargeFont = FALSE;
		}

		void SetInfo( int nID, int nStart, int nInterval, int nCount, BOOL bLarge )
		{
			nMsgID = nID;
			//초단위로 바꿔주자...
			nStartTime = nStart * 60;
			nIntervalTime = nInterval * 60;
			nPrintCount = nCount;
			bLargeFont = bLarge;
		}

		void SetStartTime( int nMinTime ) {	nStartTime = nMinTime * 60;	}
		void SetIntervalTime( int nMinTime ) {	nIntervalTime = nMinTime * 60;	}
		void CountDown() { if( nPrintCount > 0 ) nPrintCount--; }
		void SetPrint( bool bResult ) { bPrint = bResult; }
		bool GetPrintResult() { return bPrint; }
		BOOL GetLargeFont() { return bLargeFont; }
		int GetCount() { return nPrintCount; }
	};

	bool AddString( int Key, LPTSTR pString, bool bModify = false ); // 스트링을 등록한다
	LPCTSTR GetDataByIndex( int nIndex ); // 아이디에 해당된 스트링을 얻어온다
	
	void CheckingForMessage();	//출력가능한 메시지가 있는지 항상 체크 Update 함수 내에서 호출
	void PrintMessgae( vector<sMsgInfo*>::iterator iter ); //메세지를 출력해준다

	bool SearchSlotFlagItem(); // 기간제 아이템
	int GetTotalDays(int nYear, int nMonth, int nDay ); //지난날짜 계산하기

private:
	enum
	{
		_XDEF_INVENTORYITEMPOSX = 11,
		_XDEF_INVENTORYITEMPOSY = 71,
		_XDEF_INVENTORYSLOT_WIDTH = 5,
		_XDEF_INVENTORYSLOT_HEIGHT = 5
	};

	static const int nSearchInterval;
	SYSTEMTIME m_st;

	map<int, string>	m_mpData;
	vector<sMsgInfo*>	m_vtInfo;
	_XNoticeScript	m_XNoticeParser;

	double m_dCurrnetTime;
	bool m_bFirst;

	static _XNoticeProcess* m_pInstance;
};

#endif // !defined(AFX__XNOTICEPROCESS_H__3B906D3A_0CAB_4068_B77F_91A948E6EE53__INCLUDED_)
