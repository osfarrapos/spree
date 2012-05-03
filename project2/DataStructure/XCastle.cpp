// XCastle.cpp: implementation of the _XCastle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XCastle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XCastle::_XCastle()
{
	m_nMapId		= 0;
	m_bMine			= FALSE;
	m_nPropertyDate = 0;
	m_nStep			= 0;
	m_nTax			= 0;
	m_nMoney		= 0;
	m_nNextWar		= 1;
}

_XCastle::~_XCastle()
{
	ResetChallengeList();
}

void _XCastle::SetScheduleList(_sCastleInfo* castleinfo, BOOL bFixed)
{
	m_tOwner.nOrIndex = castleinfo->iOrIndex;
	m_tOwner.nType = castleinfo->sType;
	strncpy(m_tOwner.szOrgName, castleinfo->cOrName, _XDEF_ORGANIZATIONNAMELENGTH);

	memcpy(&m_tWarSchedule[0].tDate, &castleinfo->Schedule[0], sizeof(_XTimeUnit));
	memcpy(&m_tWarSchedule[1].tDate, &castleinfo->Schedule[1], sizeof(_XTimeUnit));
	memcpy(&m_tWarSchedule[2].tDate, &castleinfo->Schedule[2], sizeof(_XTimeUnit));
	
	m_tWarSchedule[0].bValid = bFixed;
	m_tWarSchedule[1].bValid = bFixed;
	m_tWarSchedule[2].bValid = bFixed;
}

void _XCastle::ResetChallengeList(void)
{
	vector <_XCastle_OrgInfo*>::iterator iter_orginfo;
	for(iter_orginfo = m_vecChallengers.begin() ; iter_orginfo != m_vecChallengers.end() ; ++iter_orginfo)
	{
		_XCastle_OrgInfo* orginfo = (*iter_orginfo);
		if(orginfo)
			SAFE_DELETE(orginfo);
	}
	m_vecChallengers.clear();
}

void _XCastle::SetAllChallengeList(int nth, __cb_client_challenge_info* info)
{
	m_nNextWar = nth;

	_XCastle_OrgInfo* orginfo = new _XCastle_OrgInfo;
	orginfo->nMemberCnt = info->usMemberCount;
	orginfo->nType = info->ucType;
	strncpy(orginfo->szOrgName, info->cGroupName, _XDEF_ORGANIZATIONNAMELENGTH);
	strncpy(orginfo->szMasterName, info->cMasterName, 13);

	m_vecChallengers.push_back(orginfo);
}

void _XCastle::SetChallengeList(int nth, int index, __cb_client_challenge_info* info)
{
	m_nNextWar = nth;

	m_vecChallengers[index]->nMemberCnt = info->usMemberCount;
	m_vecChallengers[index]->nType = info->ucType;
	strncpy(m_vecChallengers[index]->szOrgName, info->cGroupName, _XDEF_ORGANIZATIONNAMELENGTH);
	strncpy(m_vecChallengers[index]->szMasterName, info->cMasterName, 13);
}

void _XCastle::ChangeTime(int nth, _sND_TimeUnit* time)
{
	if(nth >= 1 && nth <= 3)
	{
		memcpy(&m_tWarSchedule[nth-1].tDate, time, sizeof(_XTimeUnit));
	}
}


///////////////////////////////////////////////////////////////////////
// Castle common info
///////////////////////////////////////////////////////////////////////
_XCastleCommonInfo::_XCastleCommonInfo()
{
	m_nMapId			= 0;
	m_enWarState		= _XEN_CB_NONE;
	m_nChallengeMoney	= 0;
	m_fChallengeTax		= 0;
	m_nThreatenTime		= 0;
	m_nDeadMaxCnt		= 0;
}

_XCastleCommonInfo::~_XCastleCommonInfo()
{
	Release();
}

void _XCastleCommonInfo::Release(void)
{
	map <int, TCHAR*>::iterator iter_bgm;
	for(iter_bgm = m_mThreatenBGM.begin() ; iter_bgm != m_mThreatenBGM.end() ; ++iter_bgm)
	{
		TCHAR* bgmname = iter_bgm->second;
		if(bgmname)
		{
			SAFE_DELETE(bgmname);
		}
	}
	m_mThreatenBGM.clear();
	m_mThreatenSE.clear();

	vector <_XCastle_DiePenalty*>::iterator iter_penalty;
	for(iter_penalty = m_vecDiePenalty.begin() ; iter_penalty != m_vecDiePenalty.end() ; ++iter_penalty)
	{
		_XCastle_DiePenalty* penaltyinfo = *iter_penalty;
		if(penaltyinfo)
		{
			SAFE_DELETE(penaltyinfo);
		}
	}
	m_vecDiePenalty.clear();

	map <int, _XCastle_Scheduler*>::iterator iter_schedule;
	for(iter_schedule = m_mCastleScheduler.begin() ; iter_schedule != m_mCastleScheduler.end() ; ++iter_schedule)
	{
		_XCastle_Scheduler* scheduler = iter_schedule->second;
		if(scheduler)
		{
			if(scheduler->szBGMFileName)
			{
				SAFE_DELETE_ARRAY(scheduler->szBGMFileName);
			}
			SAFE_DELETE(scheduler);
		}
	}
	m_mCastleScheduler.clear();

	m_nMapId			= 0;
	m_enWarState		= _XEN_CB_NONE;
	m_nChallengeMoney	= 0;
	m_fChallengeTax		= 0;
	m_nThreatenTime		= 0;
	m_nDeadMaxCnt		= 0;
}

void _XCastleCommonInfo::Reset(void)
{
	m_enWarState		= _XEN_CB_NONE;
	m_dwTimeFromServer  = 0;
	ClearState();
}

void _XCastleCommonInfo::ClearState(void)
{
	m_bMsgDone			= FALSE;
	m_bBGMDone			= FALSE;
	m_bSEDone			= FALSE;
	m_bShoutDone		= FALSE;
	m_bGateDone			= FALSE;
	m_bThreatenDone		= FALSE;
	m_bLightFootDone	= FALSE;
	m_bAnimationDone	= FALSE;
	m_bEffectDone		= FALSE;
	m_bBuffDone			= FALSE;
}

void _XCastleCommonInfo::Process(void)
{
	if(!g_pLocalUser->m_bCastleBattle)		// 장원전에 참가하지 않은 경우 skip
		return;

	if(!m_bMsgDone)
	{
		if(m_mCastleScheduler[m_enWarState]->nTextIndex > 0)
		{
			// msg
			ProcessAlarmMsg();
		}
		m_bMsgDone = TRUE;
	}
	if(!m_bBGMDone)
	{
		if(m_mCastleScheduler[m_enWarState]->nBGMStartTime > 0)
		{
			// BGM
//			PostMessage();
		}
		m_bBGMDone = TRUE;
	}
	if(!m_bShoutDone)
	{
		if(m_mCastleScheduler[m_enWarState]->dwShout == 1)
		{
			// 장원전 사자후 on
			// show window
		}
		else
		{
			// close window
		}
		m_bShoutDone = TRUE;
	}
	if(!m_bGateDone)
	{
		if(m_mCastleScheduler[m_enWarState]->dwGateState == 1)
		{
			// gate state on - fuctional object 선행
			// functionalobjectConditionItem.cpp에서 작업 할 것.
		}
		m_bGateDone = TRUE;
	}
	if(!m_bThreatenDone)
	{
		if(m_mCastleScheduler[m_enWarState]->dwThreaten == 1)
		{
			// Threaten state on - fuctional object 선행
			// functionalobjectConditionItem.cpp에서 작업 할 것 - 장주 앞 gate 처리
		}
		m_bThreatenDone = TRUE;
	}
	if(!m_bLightFootDone)
	{
		if(m_mCastleScheduler[m_enWarState]->dwLightFoot == 1)
		{
			// 경공 불가 처리 - 장원안에 있을 때만 (영역 처리 선행)
		}
		m_bLightFootDone = TRUE;
	}
	if(!m_bAnimationDone)
	{
		if(m_mCastleScheduler[m_enWarState]->nAnimationIndex_male > 0 || m_mCastleScheduler[m_enWarState]->nAnimationIndex_female > 0)
		{
			// animation 적용
			ProcessAnimation();
		}
		m_bAnimationDone = TRUE;
	}
	if(!m_bEffectDone)
	{
		if(m_mCastleScheduler[m_enWarState]->nEffectId > 0)
		{
			// effect - 장원전 내에 있을 때 처리 필요
			ProcessVisualEffect();
		}
		m_bEffectDone = TRUE;
	}
	if(!m_bBuffDone)
	{
		if(m_mCastleScheduler[m_enWarState]->nBuffId > 0)
		{
			// Buff - server
		}
		m_bBuffDone = TRUE;
	}

	// state 변경
	if(m_enWarState >= _XEN_CB_READY && m_enWarState < _XEN_CB_PROGRESS)
	{
		map <int, _XCastle_Scheduler*>::iterator iter_schedule = m_mCastleScheduler.find(m_enWarState + 1);
		if(iter_schedule != m_mCastleScheduler.end())
		{
			// 다음 state 있음
			//시간 계산 후 시간이 맞다면
			if(g_LocalSystemTime >= (m_dwTimeFromServer + (m_mCastleScheduler[m_enWarState]->nTime - m_mCastleScheduler[m_enWarState+1]->nTime)))
			{
				m_enWarState = (_XCastle_State)(m_enWarState + 1);
				ClearState();
			}
		}
	}
	else if(m_enWarState == _XEN_CB_START)
	{
		// 시간 계산 후 시간이 맞으면
		if(g_LocalSystemTime >= (m_dwTimeFromServer + m_mCastleScheduler[_XEN_CB_PROGRESS]->nTime))
		{
			m_enWarState = _XEN_CB_PROGRESS;
			ClearState();
		}
	}
	else if(m_enWarState <= _XEN_CB_PROGRESS && m_enWarState < _XEN_CB_END)
	{
		map <int, _XCastle_Scheduler*>::iterator iter_schedule = m_mCastleScheduler.find(m_enWarState + 1);
		if(iter_schedule != m_mCastleScheduler.end())
		{
			// 다음 state 있음
			//시간 계산 후 시간이 맞다면
			if(g_LocalSystemTime >= (m_dwTimeFromServer + m_mCastleScheduler[m_enWarState+1]->nTime))
			{
				m_enWarState = (_XCastle_State)(m_enWarState + 1);
				ClearState();
			}
		}
	}
	else if(m_enWarState >= _XEN_CB_END)
	{
		map <int, _XCastle_Scheduler*>::iterator iter_schedule = m_mCastleScheduler.find(m_enWarState + 1);
		if(iter_schedule != m_mCastleScheduler.end())
		{
			// 다음 state 있음
			//시간 계산 후 시간이 맞다면
			if(g_LocalSystemTime >= (m_dwTimeFromServer + m_mCastleScheduler[m_enWarState+1]->nTime))
			{
				m_enWarState = (_XCastle_State)(m_enWarState + 1);
				ClearState();
			}
		}
		else
		{
			// 끝났음
			Reset();
		}
	}
}

void _XCastleCommonInfo::ProcessAlarmMsg(void)
{
	BOOL bMsg = FALSE;
	switch(m_mCastleScheduler[m_enWarState]->dwAlarmMsgTarget)
	{
	case 0 :
		{
			// all
			bMsg = TRUE;
		}
		break;
	case 1 :
		{
			if(g_pLocalUser->m_dwAttackType == 1)
			{
				// attacker
				bMsg = TRUE;
			}
		}
		break;
	case 2 :
		{
			if(g_pLocalUser->m_dwAttackType == 1 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// attacker-leader
				bMsg = TRUE;
			}
		}
		break;
	case 3 :
		{
			if(g_pLocalUser->m_dwAttackType == 2)
			{
				// defender
				bMsg = TRUE;
			}
		}
		break;
	case 4 :
		{
			if(g_pLocalUser->m_dwAttackType == 2 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// defender-leader
				bMsg = TRUE;
			}
		}
		break;
	case 5 :
		{
			// 장주를 협박하는 총대장
		}
		break;
	case 6 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 1)
			{
				// winners
				bMsg = TRUE;
			}
		}
		break;
	case 7 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 1 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// winners - leader
				bMsg = TRUE;
			}
		}
		break;
	case 8 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 2)
			{
				// losers
				bMsg = TRUE;
			}
		}
		break;
	case 9 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 2 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// losers - leader
				bMsg = TRUE;
			}
		}
		break;
	}

	if(bMsg)
	{
		if(m_mCastleScheduler[m_enWarState]->dwAlarmMsgType == 1 || m_mCastleScheduler[m_enWarState]->dwAlarmMsgType == 2)
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(m_mCastleScheduler[m_enWarState]->nTextIndex), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
		}
		else
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(m_mCastleScheduler[m_enWarState]->nTextIndex), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
		}
	}
}

void _XCastleCommonInfo::ProcessAnimation(void)
{
	BOOL bAnimation = FALSE;
	switch(m_mCastleScheduler[m_enWarState]->dwAnimationTarget)
	{
	case 0 :
		{
			// all
			bAnimation = TRUE;
		}
		break;
	case 1 :
		{
			if(g_pLocalUser->m_dwAttackType == 1)
			{
				// attacker
				bAnimation = TRUE;
			}
		}
		break;
	case 2 :
		{
			if(g_pLocalUser->m_dwAttackType == 1 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// attacker-leader
				bAnimation = TRUE;
			}
		}
		break;
	case 3 :
		{
			if(g_pLocalUser->m_dwAttackType == 2)
			{
				// defender
				bAnimation = TRUE;
			}
		}
		break;
	case 4 :
		{
			if(g_pLocalUser->m_dwAttackType == 2 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// defender-leader
				bAnimation = TRUE;
			}
		}
		break;
	case 5 :
		{
			// 장주를 협박하는 총대장
		}
		break;
	case 6 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 1)
			{
				// winners
				bAnimation = TRUE;
			}
		}
		break;
	case 7 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 1 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// winners - leader
				bAnimation = TRUE;
			}
		}
		break;
	case 8 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 2)
			{
				// losers
				bAnimation = TRUE;
			}
		}
		break;
	case 9 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 2 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// losers - leader
				bAnimation = TRUE;
			}
		}
		break;
	}

	if(bAnimation)
	{
		// 맨손, 단병기, 장병기 처리 필요
		if(g_pLocalUser->m_CharacterInfo.gender == _XGENDER_MAN)
			g_pLocalUser->SetAnimationIndex(m_mCastleScheduler[m_enWarState]->nAnimationIndex_male);
		else
			g_pLocalUser->SetAnimationIndex(m_mCastleScheduler[m_enWarState]->nAnimationIndex_female);
	}
}

void _XCastleCommonInfo::ProcessVisualEffect(void)
{
	BOOL bEffect = FALSE;
	switch(m_mCastleScheduler[m_enWarState]->dwEffectTarget)
	{
	case 0 :
		{
			// all
			bEffect = TRUE;
		}
		break;
	case 1 :
		{
			if(g_pLocalUser->m_dwAttackType == 1)
			{
				// attacker
				bEffect = TRUE;
			}
		}
		break;
	case 2 :
		{
			if(g_pLocalUser->m_dwAttackType == 1 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// attacker-leader
				bEffect = TRUE;
			}
		}
		break;
	case 3 :
		{
			if(g_pLocalUser->m_dwAttackType == 2)
			{
				// defender
				bEffect = TRUE;
			}
		}
		break;
	case 4 :
		{
			if(g_pLocalUser->m_dwAttackType == 2 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// defender-leader
				bEffect = TRUE;
			}
		}
		break;
	case 5 :
		{
			// 장주를 협박하는 총대장
		}
		break;
	case 6 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 1)
			{
				// winners
				bEffect = TRUE;
			}
		}
		break;
	case 7 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 1 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// winners - leader
				bEffect = TRUE;
			}
		}
		break;
	case 8 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 2)
			{
				// losers
				bEffect = TRUE;
			}
		}
		break;
	case 9 :
		{
			if(g_pLocalUser->m_dwResultCastleBattle == 2 && g_pLocalUser->m_dwRoleInCastleBattle == 1)
			{
				// losers - leader
				bEffect = TRUE;
			}
		}
		break;
	}

	if(bEffect)
	{
		D3DXMATRIX offsetmat;
		D3DXMatrixIdentity(&offsetmat);
		g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
			m_mCastleScheduler[m_enWarState]->nEffectId, &offsetmat); //, NULL, TRUE);
	}
}

BOOL _XCastleCommonInfo::GetCommand(char* cmd, FILE* fileptr, bool bnumber)
{
	while(1)
	{
		if(!fgets(cmd, 120, fileptr))
			return FALSE;
		else
		{
			if(cmd[0] == '/' || cmd[0] == '\n' || cmd[0] == ' ' || cmd[0] == ';')
				continue;

			if(strncmp(cmd, "_XM_EOS", strlen("_XM_EOS")) == 0)
				return TRUE;

			int i = 0 ;
			int len = strlen(cmd);

			while((!isalnum(cmd[i]) && cmd[i] != '{' && cmd[i] != '}' && cmd[i] != ';') && i < len)
			{
				i++;
			}
			if(i > 0 && i < len)
			{
				memmove(&cmd[0], &cmd[i], len-i+1);		// 끝에 '0' 포함
			}

			if(cmd[0] == ';')
				continue;

			len = strlen(cmd);

			while(len > 0)
			{
				if(cmd[len] == '\n' || cmd[len] == '/')
					cmd[len] = 0;
				--len;
			}

			if(cmd[0] == '{' || cmd[0] == '}' || bnumber || isalpha(cmd[0]))
				return TRUE;
			else 
				return FALSE;
		}
	}
}

BOOL _XCastleCommonInfo::Load(int mapid)
{
#ifdef _XDEF_CASTLEBATTLE
	_XPackageArchive scriptorpackage;	
	_XSetDefaultPath();	
	if( !scriptorpackage.Open( "NDCore.dsc" ) ) return FALSE;
	
	FILE* fileptr = NULL;

	BOOL bLoad = FALSE;
	TCHAR filename[256];
	memset(filename, 0, sizeof(filename));

	switch(mapid)
	{
	case 13 :
		{
			// 항주 장원전 스크립트
			strcpy(filename, _T("map_Hangjoo_Manor_script.txt"));
			bLoad = TRUE;
		}
		break;
	}

	if(!bLoad)
	{
		// 장원전이 없는 맵이므로 넘어간다
		return TRUE;
	}

	fileptr = scriptorpackage.GetPackedFile( filename );

	if( !fileptr )
	{
		_XFatalError("Can't load Manor script");
		scriptorpackage.Close();
		return FALSE;
	}

	char	cmd[256];
	char	arg1[64], arg2[64], arg3[64]; //, arg4[64];
	BOOL	bEnd = FALSE;
	BOOL	bComplete = FALSE;

	while(1)
	{
		while(GetCommand(cmd, fileptr))
		{
			switch(cmd[0])
			{
			case 'm' :
				{
					if(strncmp(cmd, "manor_tax", strlen("manor_tax")) == 0)
					{
						int n = sscanf(cmd, "%s %s", arg1, arg2);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							m_tDefaultTax.fCastleRegion = atof(arg2);
						}
					}
					else if(strncmp(cmd, "manor_level", strlen("manor_level")) == 0)
					{
						if(!ParseManorLevel(fileptr))
							bEnd = TRUE;
					}
					else if(strncmp(cmd, "manor_open_timer", strlen("manor_open_timer")) == 0)
					{
						if(!ParseManorOpenTimer(fileptr))
							bEnd = TRUE;
					}
					else if(strncmp(cmd, "map_id", strlen("map_id")) == 0)
					{
						int n = sscanf(cmd, "%s %s", arg1, arg2);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							m_nMapId = atoi(arg2);
						}
					}
					else if(strncmp(cmd, "manor_die_penalty", strlen("manor_die_penalty")) == 0)
					{
						if(!ParseManorDiePenalty(fileptr))
							bEnd = TRUE;
					}
				}
				break;
			case 'c' :
				{
					if(strncmp(cmd, "commonzone_tax", strlen("commonzone_tax")) == 0)
					{
						int n = sscanf(cmd, "%s %s", arg1, arg2);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							m_tDefaultTax.fCommonRegion = atof(arg2);
						}
					}
					else if(strncmp(cmd, "challenge_money_tax", strlen("challenge_money_tax")) == 0)
					{
						int n = sscanf(cmd, "%s %s", arg1, arg2);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							m_fChallengeTax = atof(arg2);
						}
					}
					else if(strncmp(cmd, "challenge_money", strlen("challenge_money")) == 0)
					{
						int n = sscanf(cmd, "%s %s", arg1, arg2);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							m_nChallengeMoney = atoi(arg2);
						}
					}
				}
				break;
			case 't' :
				{
					if(strncmp(cmd, "threatener_time", strlen("threatener_time")) == 0)
					{
						int n = sscanf(cmd, "%s %s", arg1, arg2);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							m_nThreatenTime = atoi(arg2);
						}
					}
					else if(strncmp(cmd, "threatener_bgm", strlen("threatener_bgm")) == 0)
					{
						// 보류
//						if(!ParseThreatenBGM(fileptr))
//							bEnd = TRUE;
					}
					else if(strncmp(cmd, "threatener_se", strlen("threatener_se")) == 0)
					{
					}
				}
				break;
			case 'e' :
				{
					if(strncmp(cmd, "event_time", strlen("event_time")) == 0)
					{
						int n = sscanf(cmd, "%s %s %s", arg1, arg2, arg3);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							int before = 0, after = 0;		// state 체크용

							_XCastle_Scheduler* pScheduler = new _XCastle_Scheduler;

							if(strncmp(arg2, "usual", strlen("usual")) == 0)
							{
								pScheduler->nWhen = 0;
								pScheduler->enWarState = _XEN_CB_NONE;
							}
							else if(strncmp(arg2, "before", strlen("before")) == 0)
							{
								pScheduler->nWhen = 1;
								pScheduler->enWarState = (_XCastle_State)(_XEN_CB_READY + before);
								before++;
							}
							else if(strncmp(arg2, "after", strlen("after")) == 0)
							{
								pScheduler->nWhen = 2;
								pScheduler->enWarState = (_XCastle_State)(_XEN_CB_PROGRESS + after);
								after++;
							}
							else if(strncmp(arg2, "end", strlen("end")) == 0)
							{
								pScheduler->nWhen = 3;
								pScheduler->enWarState = _XEN_CB_END;
							}

							pScheduler->nTime = atoi(arg3);
							pScheduler->nTime *= 1000;		// ms

							if(!ParseEventTime(fileptr, pScheduler))
							{
								bEnd = TRUE;
								SAFE_DELETE(pScheduler);
							}
							else
							{
								m_mCastleScheduler[pScheduler->enWarState] = pScheduler;
							}
						}
					}
				}
				break;
			case 'd' :
				{
					if(strncmp(cmd, "dead_max_cnt", strlen("dead_max_cnt")) == 0)
					{
						int n = sscanf(cmd, "%s %s", arg1, arg2);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							m_nDeadMaxCnt = atoi(arg2);
						}
					}
				}
				break;
			case '_' :
				{
					if(strncmp(cmd, "_XM_EOS", strlen("_XM_EOS")) == 0)
					{
						bEnd = TRUE;
						bComplete = TRUE;
					}
				}
				break;
			}

			if(bEnd)
				break;
		}

		if(bEnd)
			break;
	}

	return bComplete;
#else
	return TRUE;
#endif
}

int _XCastleCommonInfo::GetPenaltyTime(DWORD level)
{
	vector <_XCastle_DiePenalty*>::iterator iter_die;
	for(iter_die = m_vecDiePenalty.begin() ; iter_die != m_vecDiePenalty.end() ; ++iter_die)
	{
		_XCastle_DiePenalty* penalty = *iter_die;
		if(penalty)
		{
			if(level >= penalty->nMinLevel && level <= penalty->nMaxLevel)
			{
				if(g_pLocalUser->m_dwAttackType == 1)
				{
					// attacker
					return penalty->nAttackerPenalty;
				}
				else
				{
					// defender
					return penalty->nDefenderPenalty;
				}
			}
		}
	}
	return 0;
}

BOOL _XCastleCommonInfo::ParseManorLevel(FILE* fp)
{
	char cmd[256];
	char arg1[64], arg2[64], arg3[64];

	BOOL bStart = FALSE, bEnd = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
				}
				break;
			default :
				{
					if(sscanf(cmd, "%s %s %s", arg1, arg2, arg3) == 3)
					{
						int level = atoi(arg1);

						if(level >= 1 && level <= 5)
						{
							m_tCastleTax[level-1].fCastleRegion = atof(arg2);
							m_tCastleTax[level-1].fCommonRegion = atof(arg3);
						}
					}
					else
						break;
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

BOOL _XCastleCommonInfo::ParseManorOpenTimer(FILE* fp)
{
	char cmd[256];
	char arg1[64], arg2[64], arg3[64], arg4[64];

	BOOL bStart = FALSE, bEnd = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
				}
				break;
			default :
				{
					if(sscanf(cmd, "%s %s %s %s", arg1, arg2, arg3, arg4) == 4)
					{
						int no = atoi(arg1);

						if(no >= 1 && no <= 3)
						{
							m_tDefaultOpenTime[no-1].dwHour = atoi(arg2);
							m_tDefaultOpenTime[no-1].dwMinute = atoi(arg3);
							m_tDefaultOpenTime[no-1].dwProgressTime = atoi(arg4);
						}
					}
					else
						break;
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

BOOL _XCastleCommonInfo::ParseManorDiePenalty(FILE* fp)
{
	char cmd[256];
	char arg1[64], arg2[64], arg3[64], arg4[64];

	BOOL bStart = FALSE, bEnd = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
				}
				break;
			default :
				{
					if(sscanf(cmd, "%s %s %s %s", arg1, arg2, arg3, arg4) == 4)
					{
						_XCastle_DiePenalty* pPenalty = new _XCastle_DiePenalty;

						pPenalty->nMinLevel = atoi(arg1);
						pPenalty->nMaxLevel = atoi(arg2);
						pPenalty->nAttackerPenalty = atoi(arg3);
						pPenalty->nDefenderPenalty = atoi(arg4);

						m_vecDiePenalty.push_back(pPenalty);
					}
					else
						break;
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

BOOL _XCastleCommonInfo::ParseThreatenBGM(FILE* fp)
{
	char cmd[256];
	char arg1[128], arg2[128];

	BOOL bStart = FALSE, bEnd = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
				}
				break;
			default :
				{
					if(sscanf(cmd, "%s %s", arg1, arg2) == 2)
					{
						int type = 0;
						if(strcmp(arg1, "progress") == 0)
							type = 0;
						else if(strcmp(arg1, "success") == 0)
							type = 1;
						else
							type = 2;

						int strlength = strlen(arg2);
						TCHAR* strBGMName = new TCHAR[strlength+1];
						strBGMName[strlength] = 0;
						strncpy(strBGMName, arg2, strlength);

						m_mThreatenBGM[type] = strBGMName;
					}
					else
						break;
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

BOOL _XCastleCommonInfo::ParseEventTime(FILE* fp, _XCastle_Scheduler* pScheduler)
{
	char cmd[256];

	BOOL bStart = FALSE, bEnd = FALSE, bComplete = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
					bComplete = TRUE;
				}
				break;
			case 'a' :
				{
					if(!ParseAlarmMsg(fp, pScheduler))
						bEnd = TRUE;
				}
				break;
			case 's' :
				{
					if(!ParseState(fp, pScheduler))
						bEnd = TRUE;
				}
				break;
			case 'c' :
				{
					if(!ParseCharAni(fp, pScheduler))
						bEnd = TRUE;
				}
				break;
			case 'e' :
				{
					if(!ParseEffect(fp, pScheduler))
						bEnd = TRUE;
				}
				break;
			case 'b' :
				{
					if(!ParseBuff(fp, pScheduler))
						bEnd = TRUE;
				}
				break;
			case 'm' :
				{
					if(strncmp(cmd, "mobspawn", strlen("mobspawn")) == 0)
					{
						ParseSkipInfo(fp);
					}
					else if(strncmp(cmd, "main_bgm", strlen("main_bgm")) == 0)
					{
						ParseSkipInfo(fp);
					}
					else if(strncmp(cmd, "main_se", strlen("main_se")) == 0)
					{
						ParseSkipInfo(fp);
					}
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bComplete;
}

BOOL _XCastleCommonInfo::ParseAlarmMsg(FILE* fp, _XCastle_Scheduler* pScheduler)
{
	char cmd[256];
	char arg1[64], arg2[64], arg3[64], arg4[64];

	BOOL bStart = FALSE, bEnd = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
				}
				break;
			default :
				{
					if(sscanf(cmd, "%s %s %s %s", arg1, arg2, arg3, arg4) == 4)
					{
						pScheduler->dwAlarmMsgType = atoi(arg1);
						pScheduler->nTextIndex = atoi(arg4);
						pScheduler->dwAlarmMsgTarget = GetTargetType(arg2);
					}
					else
						break;
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

BOOL _XCastleCommonInfo::ParseCharAni(FILE* fp, _XCastle_Scheduler* pScheduler)
{
	char cmd[256];
	char arg1[64], arg2[64], arg3[64];

	BOOL bStart = FALSE, bEnd = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
				}
				break;
			default :
				{
					if(sscanf(cmd, "%s %s %s ", arg1, arg2, arg3) == 3)
					{
						pScheduler->dwAnimationTarget = GetTargetType(arg1);
						pScheduler->nAnimationIndex_male = atoi(arg2);
						pScheduler->nAnimationIndex_female = atoi(arg3);
					}
					else
						break;
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

BOOL _XCastleCommonInfo::ParseEffect(FILE* fp, _XCastle_Scheduler* pScheduler)
{
	char cmd[256];
	char arg1[64], arg2[64];

	BOOL bStart = FALSE, bEnd = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
				}
				break;
			default :
				{
					if(sscanf(cmd, "%s %s", arg1, arg2) == 2)
					{
						pScheduler->dwEffectTarget = GetTargetType(arg2);
						pScheduler->nEffectId = atoi(arg1);
						// se 처리
					}
					else
						break;
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

BOOL _XCastleCommonInfo::ParseBuff(FILE* fp, _XCastle_Scheduler* pScheduler)
{
	char cmd[256];
	char arg1[32], arg2[32], arg3[32], arg4[32], arg5[32];

	BOOL bStart = FALSE, bEnd = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
				}
				break;
			default :
				{
					if(sscanf(cmd, "%s %s %s %s %s", arg1, arg2, arg3, arg4, arg5) == 5)
					{
						pScheduler->dwBuffTarget = GetTargetType(arg2);
						pScheduler->nBuffId = atoi(arg1);
						pScheduler->nBuffTime = atoi(arg5);
					}
					else
						break;
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

BOOL _XCastleCommonInfo::ParseState(FILE* fp, _XCastle_Scheduler* pScheduler)
{
	char cmd[256];
	char arg1[128], arg2[128];

	BOOL bStart = FALSE, bEnd = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
				}
				break;
			case 's' :
				{
					if(sscanf(cmd, "%s %s", arg1, arg2) == 2)
					{
						if(strncmp(arg2, "on", strlen("on")) == 0)
						{
							pScheduler->dwShout = 1;
						}
						else if(strncmp(arg2, "off", strlen("off")) == 0)
						{
							pScheduler->dwShout = 0;
						}
					}
				}
				break;
			case 'u' :
				{
					if(sscanf(cmd, "%s %s", arg1, arg2) == 2)
					{
						if(strncmp(arg2, "on", strlen("on")) == 0)
						{
							pScheduler->dwGateState = 1;
						}
						else if(strncmp(arg2, "off", strlen("off")) == 0)
						{
							pScheduler->dwGateState = 0;
						}
					}
				}
				break;
			case 't' :
				{
					if(sscanf(cmd, "%s %s", arg1, arg2) == 2)
					{
						if(strncmp(arg2, "on", strlen("on")) == 0)
						{
							pScheduler->dwThreaten = 1;
						}
						else if(strncmp(arg2, "off", strlen("off")) == 0)
						{
							pScheduler->dwThreaten = 0;
						}
					}
				}
				break;
			case 'l' :
				{
					if(sscanf(cmd, "%s %s", arg1, arg2) == 2)
					{
						if(strncmp(arg2, "on", strlen("on")) == 0)
						{
							pScheduler->dwLightFoot = 1;
						}
						else if(strncmp(arg2, "off", strlen("off")) == 0)
						{
							pScheduler->dwLightFoot = 0;
						}
					}
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

BOOL _XCastleCommonInfo::ParseSkipInfo(FILE* fp)
{
	char cmd[256];

	BOOL bStart = FALSE, bEnd = FALSE;

	while(!feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			if(cmd[0] == '{')
			{
				bStart = TRUE;
				break;
			}
		}
		if(bStart)
			break;
	}

	if(!bStart)
		return FALSE;

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

int _XCastleCommonInfo::GetTargetType(char* arg)
{
	if(strcmp(arg, "all") == 0)
		return 0;
	else if(strcmp(arg, "attacker") == 0)
		return 1;
	else if(strcmp(arg, "attack_leader") == 0)
		return 2;
	else if(strcmp(arg, "defender") == 0)
		return 3;
	else if(strcmp(arg, "defend_leader") == 0)
		return 4;
	else if(strcmp(arg, "threatener") == 0)
		return 5;
	else if(strcmp(arg, "winner") == 0)
		return 6;
	else if(strcmp(arg, "win_leader") == 0)
		return 7;
	else if(strcmp(arg, "loser") == 0)
		return 8;
	else if(strcmp(arg, "lose_leader") == 0)
		return 9;

	// default
	return 0;
}