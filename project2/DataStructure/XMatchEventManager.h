// XMatchEventManager.h: interface for the _XMatchEventManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMATCHEVENTMANAGER_H__0B19F460_51D7_4B1D_BB64_6FA14DD80F52__INCLUDED_)
#define AFX_XMATCHEVENTMANAGER_H__0B19F460_51D7_4B1D_BB64_6FA14DD80F52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XMatchEventBaseType.h"

typedef map<int, _s_Match_Type*> _map_MatchTypeStr;		// <matchtype, match str>

class _XMatchEventManager  
{
private :
	_map_MatchTypeStr		m_mapMatchTypeList;		// 스크립트에서 읽은 비무대회 정보

	_S_Current_Match_Info	m_CurrentMatchInfo;		// 현재 비무대회 정보 (From server)

	int						m_nCurrentState;		// 현재 상태 (ready/start/end)
	DWORD					m_dwEventStartTime;		// 비무 대회 시작 시간 (LocalSystemTime)

	BOOL					m_bDrawReadyMessage;	// Ready message draw 여부
	DWORD					m_dwReadyMessageStartTime;	// draw 시작 시간
	int						m_nTextArg;

public:
	_XMatchEventManager();
	virtual ~_XMatchEventManager();

	void		Release();
	BOOL		LoadMatchEventScript();
	
	void		Draw();

	BOOL		ParseMatchType(FILE* fp, _s_Match_Type* pMatchType);

	BOOL		ParseConstraint1(FILE* fp, _s_Match_Type* pMatchType);
	BOOL		ParseConstraint2(FILE* fp, _s_Match_Type* pMatchType);
	BOOL		ParseConstraint3(FILE* fp, _s_Match_Type* pMatchType);
	BOOL		ParseReady(FILE* fp, _s_Match_Type* pMatchType);
	BOOL		ParseStart(FILE* fp, _s_Match_Type* pMatchType);
	BOOL		ParseEnd(FILE* fp, _s_Match_Type* pMatchType);
	BOOL		ParseTemp(FILE* fp);

	int			GetTime(TCHAR* arg);

	void		SetCurrentMatchInfo(__fm_info_in_packet* info);
	void		GetLeftTimeMatchEvent(int& minute, int& second);
	
	int			GetMinLevel(void);
	int			GetMaxLevel(void);
	
	void		GetStartDate(_XTimeUnit& time);

	BOOL		CanUseItem(int invennumber);
	BOOL		CanUseSkill(int skillid);

	int			GetActionConstraint(int action);

	void		SetCurrentState(int state)		{		m_nCurrentState = state;		};
	int			GetCurrentState(void)			{		return m_nCurrentState;			};
	
	int			GetCurrentEventArea(void)		{		return m_CurrentMatchInfo.nAreaIndex;	};

	void		SetEventStartTime(DWORD time)	{		m_dwEventStartTime = time;		};
	DWORD		GetEventStartTime(void)			{		return m_dwEventStartTime;		};

	void		SetDrawReadyMessage(BOOL draw)			{	m_bDrawReadyMessage = draw;			};
	void		SetReadyMessageStartTime(DWORD time)	{	m_dwReadyMessageStartTime = time;	};
	void		SetTextArg(int arg)						{	m_nTextArg = arg;					};
};

#endif // !defined(AFX_XMATCHEVENTMANAGER_H__0B19F460_51D7_4B1D_BB64_6FA14DD80F52__INCLUDED_)
