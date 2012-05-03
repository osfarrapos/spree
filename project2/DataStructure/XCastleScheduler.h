// XCastleScheduler.h: interface for the _XCastleScheduler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCASTLESCHEDULER_H__7558B243_8CBB_41ED_8D25_BBAABCE6489B__INCLUDED_)
#define AFX_XCASTLESCHEDULER_H__7558B243_8CBB_41ED_8D25_BBAABCE6489B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XCastleBaseType.h"
#include "XKernel.h"

#include <vector>
#include <map>

extern BOOL GetCommand(char* cmd, FILE* fileptr, bool bnumber = false);

class _XCastleScheduler  
{
	// script에서 읽은 장원전 정보
public :
	int				m_nMapId;				// map id
	int				m_nMapNameStrindex;		// map name index
	int				m_nMobMasterNameIndex;	// 몬스터 소유일때 이름 표기
	
	_XCastle_Tax	m_tDefaultTax;			// 공통 세율
	_XCastle_Tax	m_tCastleTax[5];		// 장원 세율 (display용)

	int				m_nChallengeMoney[5];	// 도전 비용
	float			m_fChallengeTax;		// 도전 비용 수입

	int				m_nThreatenTime;		// 장주 협박 시간 (초)
	int				m_nThreatenDelayTime;	// 장주 협박 딜레이 시간 (초)

	int				m_nDeadMaxCnt;			// 혼수상태 최대 반복 횟수
	int				m_nMaxShoutCnt;			// 외치기 최대 횟수
	
	TCHAR*			m_szMinimapName;		// 미니맵 이름
	int				m_nMinimapPosition;		// 미니맵 Position

	map <int, TCHAR*>		m_mThreatenBGM;
	map <int, int>			m_mThreatenSE;

	_XCastle_OpenTime		m_tDefaultOpenTime[3];	// default  도전 시간

	vector <_XCastle_OpenTime*> m_vecChallengeTime;

	vector <_XCastle_DiePenalty*>	m_vecDiePenalty;

	map <int, _XCastle_Scheduler*>	m_mCastleScheduler;


public:
	_XCastleScheduler();
	virtual ~_XCastleScheduler();

	void			Release(void);

//	BOOL			GetCommand(char* cmd, FILE* fileptr, bool bnumber = false);
	BOOL			Load(TCHAR* filename);

	BOOL			ParseManorLevel(FILE* fp);
	BOOL			ParseManorOpenTimer(FILE* fp);
	BOOL			ParseManorDiePenalty(FILE* fp);
	BOOL			ParseThreatenBGM(FILE* fp);
	BOOL			ParseChallengeMoney(FILE* fp);
	BOOL			ParseManorChallengeTime(FILE* fp);

	BOOL			ParseEventTime(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseAlarmMsg(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseCharAni(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseEffect(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseBuff(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseState(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseMainBGM(FILE* fp, _XCastle_Scheduler* pScheduler, BOOL bGroup = FALSE);
	BOOL			ParseVictoryType(FILE* fp);
	BOOL			ParseMainSE(FILE* fp, _XCastle_Scheduler* pScheduler);
	
	BOOL			ParseSkipInfo(FILE* fp);

	int				GetTargetType(char* arg);

};

#endif // !defined(AFX_XCASTLESCHEDULER_H__7558B243_8CBB_41ED_8D25_BBAABCE6489B__INCLUDED_)
