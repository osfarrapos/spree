// XCastleBaseType.h
//
//////////////////////////////////////////////////////////////////////

#ifndef _XCASTLEBASETYPE_H_
#define _XCASTLEBASETYPE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XNetwork.h"

// 세율
struct _XCastle_Tax	
{
	float		fDefaultTax;
	float		fAdditiveTax;

	_XCastle_Tax()
	{
		fDefaultTax = 0.f;
		fAdditiveTax = 0.f;
	}
};

// 도전 시간
struct _XCastle_OpenTime
{
	DWORD		dwHour : 8;
	DWORD		dwMinute : 8;
	DWORD		dwProgressTime : 16;

	_XCastle_OpenTime()
	{
		dwHour = dwMinute = dwProgressTime = 0;
	}
};

// 장원전 state 
enum _XCastle_State
{
	_XEN_CB_NONE = 0,
	_XEN_CB_READY = 100,		// 장원전 시작 전
	_XEN_CB_PROGRESS = 200,		// 시작 + 진행중
	_XEN_CB_END = 300			// 끝
}; 

struct _XCastle_AlarmMsg
{
	DWORD	dwAlarmMsgType : 16;
	DWORD	dwAlarmMsgTarget : 16;
	int		nTextIndex;
};

struct _XCastle_Animation
{
	int		nAnimationTarget;
	int		nAnimationIndex_male;
	int		nAnimationIndex_female;
};

struct _XCastle_VisualEffect
{
	DWORD	dwVisualEffectTarget : 8;
	DWORD	reserved : 8;
	DWORD	dwVisualEffectId : 8;
};

struct _XCastle_MainBGM
{
	TCHAR*	szBGMFileName;
	int		nPlayBGM;
	int		nBGMEndTime;
	BOOL	bLoop;
};

// 도전 일정
struct _XCastle_Scheduler
{
	int			nWhen;
	int			nTime;

	_XCastle_State	enWarState;

	BOOL		bMsg;
	BOOL		bAni;
	BOOL		bVE;

	vector <_XCastle_AlarmMsg*> tAlarmMsg;
	vector <_XCastle_Animation*> tAnimation;
	vector <_XCastle_VisualEffect*> tVisualEffect;

	DWORD		dwShout : 8;
	DWORD		dwGateState : 8;
	DWORD		dwThreaten : 8;
	DWORD		dwLightFoot : 8;

	int			nSEId;
	BOOL		bSELoop;
	int			nSEEndTime;

	int			nPlayBGM;
	int			nBGMEndTime;
	BOOL		bLoop;

	TCHAR*		szBGMFileName;

#ifdef _XDEF_CASTLEBATTLE_SECONDEDITION
	vector <_XCastle_MainBGM*> tMainBGMList;
#endif

	_XCastle_Scheduler()
	{
		nWhen	= 0;
		nTime	= 0;

		bMsg	= FALSE;
		bAni	= FALSE;
		bVE		= FALSE;
		
		dwShout		= 0;
		dwGateState = 0;
		dwThreaten  = 0;
		dwLightFoot = 0;

		nSEId			= 0;
		bSELoop			= FALSE;
		nSEEndTime		= 0;

		nPlayBGM		= 0;
		nBGMEndTime		= 0;
		bLoop			= FALSE;

		szBGMFileName	= NULL;
	}
};

struct _XTimeUnit
{
	DWORD		dwYear : 5;		// base 2000
	DWORD		dwMonth : 4;
	DWORD		dwDay : 6;
	DWORD		dwHour : 5;
	DWORD		dwMinute : 6;
	DWORD		dwSecond : 6;

	_XTimeUnit()
	{
		dwYear = dwMonth = dwDay = dwHour = dwMinute = dwSecond = 0;
	}
};

// 도전 일정
struct _XCastle_ChallengeSchedule
{
	_XTimeUnit	tDate;

	_XCastle_ChallengeSchedule()
	{
	}
};

// 도전자 및 소유자 정보
struct _XCastle_OrgInfo
{
	int			nMemberCnt;
	
	int			nOrIndex;
	int			nType;					// 0(단)  1(회)
	int			nGroupOfMaster;
	TCHAR		szOrgName[_XDEF_ORGANIZATIONNAMELENGTH+1];
	TCHAR		szMasterName[13];

	_XCastle_OrgInfo()
	{
		nMemberCnt	= 0;
		nOrIndex	= 0;
		nType		= 0;
		nGroupOfMaster = 0;
		memset(szOrgName, 0, sizeof(szOrgName));
		memset(szMasterName, 0, sizeof(szMasterName));
	}
};

// 부활 패널티
struct _XCastle_DiePenalty
{
	int			nMinLevel;
	int			nMaxLevel;
	int			nAttackerPenalty;
	int			nDefenderPenalty;

	_XCastle_DiePenalty()
	{
		nMinLevel	= 0;
		nMaxLevel	= 0;
		nAttackerPenalty	= 0;
		nDefenderPenalty	= 0;
	}
};


#endif