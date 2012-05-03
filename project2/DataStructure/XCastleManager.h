// XCastleManager.h: interface for the _XCastleManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCASTLEMANAGER_H__EE88715B_C961_4D62_9892_2966834B4355__INCLUDED_)
#define AFX_XCASTLEMANAGER_H__EE88715B_C961_4D62_9892_2966834B4355__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XCastleInfo.h"
#include "XCastleScheduler.h"

#include <map>

class _XCastleManager  
{
private :
	// 전체 맵 : 스크립트 정보
	map <int, TCHAR*>				m_mManorScriptName;

	map <int, _XCastleInfo*>		m_mCastleInfo;
	map <int, _XCastleScheduler*>	m_mCBScheduler;
	
	// 현재 진행중인 장원전 정보
public :
	_XCastleScheduler*		m_pScheduler;	// 현재 맵의 장원전 정보
	_XCastle_State			m_enWarState;	// 장원전 진행 상황

	int				m_nMapId;

	DWORD			m_dwTimeFromServer;		// 서버에서 패킷 도착한 시간 (ready, start, end)
	DWORD			m_dwThreatenStartTime;	// 협박 시작 시작 (서버 허락)
	FLOAT			m_fThreatenPercentage;
	BOOL			m_bSuccessThreaten;
	int				m_nProgress;
	
	DWORD			m_dwThreatenRequestTime;	// 협박 req 보낸 시간 - error가 와도 시간 체크해서 packet 연속으로 보내는 것 막는다

	BOOL			m_bMsgDone;
	BOOL			m_bBGMDone;
	BOOL			m_bSEDone;
	BOOL			m_bShoutDone;
	BOOL			m_bGateDone;
	BOOL			m_bThreatenDone;
	BOOL			m_bLightFootDone;
	BOOL			m_bAnimationDone;
	BOOL			m_bEffectDone;
	BOOL			m_bBuffDone;
	
	DWORD			m_dwBGMStartTime;				// BGM 시작할 때 시간 - Loop도 포함
	DWORD			m_dwStateTotalBGMStartTime;		// BGM 시작할 때 시간 - Loop 제외

	DWORD			m_dwSEStartTime;				// SE 시작할 때 시간 
	
	int				m_nSavePointFailResult;	// 장원전 중 여장 풀기 fail 이유
											// 1: 장원전 진행중 아님
											// 2: 공성측이 아님
											// 3: 수성측이 아님
	
	BOOL			m_bDrawOwnerMessage;		// 장원 소유 안내 메시지 출력
	DWORD			m_dwOwnerMessageStartTime;	// 장원 소유 안내 메시지 출력 시작 시간

public:
	_XCastleManager();
	virtual ~_XCastleManager();

	void			Release();
	void			ReleaseCastleInfo(void);

	void			Reset(void);
	void			ClearState(void);

	void			Draw(void);
	void			Process(void);

	void			ProcessAlarmMsg(void);
	void			ProcessAnimation(void);
	void			ProcessVisualEffect(void);
	void			ProcessBGM(void);
	void			ProcessSE(void);
	
	BOOL			GetTargetType(int index);

	BOOL			Load(void);
	BOOL			LoadCastleScheduler(void);
	BOOL			ParseManorMapId(FILE* fp);
	BOOL			ParseSkipInfo(FILE* fp);

	void			SetCurrentScheduler(int mapid);

	_XCastleInfo*	GetCastleInfo(int mapid);
	int				GetCastleNameTextId(int mapid);

	// 아래 get 함수들은 진행중인 장원전 정보 이므로 mapid 필요없음
	int				GetDeadMaxCount();
	int				GetPenaltyTime(DWORD level);
	DWORD			GetThreatenTime();
	DWORD			GetThreatenDelayTime();
	int				GetShoutMaxCount();

	DWORD			GetGateState();
	DWORD			GetThreatenState();
	DWORD			GetLightFootState();
	
	int				GetMinimapPosition();

	// 도전 비용은 진행중인 장원전과는 상관없으므로 mapid 필요
	int				GetChallengeMoney(int mapid, int index);
	FLOAT			GetDefaultTax(int mapid, int step);
	FLOAT			GetAdditiveTax(int mapid, int step);
	void			GetDefaultOpenTime(int mapid, int& first, int& second, int& third);
	int				GetMobMasterNameIndex(int mapid);
	void			GetChallengeTime(int mapid, int* hour, int* minute, int& count);
	
	void			SetScheduleList(int mapid, _sCastleInfo* castleinfo, DWORD money = 0);
	void			SetChallengeList(int mapid, __cb_client_challenge_info* info);
	void			ResetChallengeList(int mapid);
	void			ChangeTime(int mapid, int nth, _sND_TimeUnit* time);

	void			SetCastleTaxMoney(int mapid, DWORD totalmoney, DWORD partmoney);
	DWORD			GetCastleTaxMoney(int mapid);

	void			SetDrawOwnerMessage(BOOL draw)			{	m_bDrawOwnerMessage = draw;		};
	void			SetOwnerMessageStartTime(DWORD time)	{	m_dwOwnerMessageStartTime = time;	};

};

#endif // !defined(AFX_XCASTLEMANAGER_H__EE88715B_C961_4D62_9892_2966834B4355__INCLUDED_)
