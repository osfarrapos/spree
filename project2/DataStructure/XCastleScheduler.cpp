// XCastleScheduler.cpp: implementation of the _XCastleScheduler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XCastleScheduler.h"

BOOL GetCommand(char* cmd, FILE* fileptr, bool bnumber)
{
	while(1)
	{
		if(!fgets(cmd, 255, fileptr))
			return FALSE;
		else
		{
//			_XDWINPRINT(cmd);

			if(cmd[0] == '/' || cmd[0] == '\n' || cmd[0] == ' ' || cmd[0] == ';')
				continue;

			if(strncmp(cmd, "_XM_EOS", strlen("_XM_EOS")) == 0)
				return TRUE;

			int i = 0 ;
			int len = strlen(cmd);

			while((!isalnum(cmd[i]) && cmd[i] != '{' && cmd[i] != '}' && cmd[i] != ';' && cmd[i] != '/') && i < len)
			{
				i++;
			}
			if(i > 0 && i < len)
			{
				memmove(&cmd[0], &cmd[i], len-i+1);		// 끝에 '0' 포함
			}

			if(cmd[0] == ';')
				continue;
			if(cmd[0] == '/' && cmd[1] == '/')
				continue;

			len = strlen(cmd);

			while(len > 0)
			{
				if(cmd[len] == '\n' || cmd[len] == '/' || cmd[len] == '\r')
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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XCastleScheduler::_XCastleScheduler()
{
	m_nMapId			= 0;
	m_fChallengeTax		= 0;
	m_nThreatenTime		= 0;
	m_nThreatenDelayTime	= 0;
	m_nDeadMaxCnt		= 0;
	m_nMaxShoutCnt		= 0;

	for(int i = 0 ; i < 5 ; ++i)
		m_nChallengeMoney[i] = 0;
	
	m_szMinimapName		= NULL;
	m_nMinimapPosition	= 0;
}

_XCastleScheduler::~_XCastleScheduler()
{
	Release();
}

void _XCastleScheduler::Release(void)
{
	map <int, TCHAR*>::iterator iter_bgm;
	for(iter_bgm = m_mThreatenBGM.begin() ; iter_bgm != m_mThreatenBGM.end() ; ++iter_bgm)
	{
		TCHAR* bgmname = iter_bgm->second;
		if(bgmname)
		{
			SAFE_DELETE_ARRAY(bgmname);
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

	vector <_XCastle_OpenTime*>::iterator iter_time;
	for(iter_time = m_vecChallengeTime.begin() ; iter_time != m_vecChallengeTime.end() ; ++iter_time)
	{
		_XCastle_OpenTime* challengetime = *iter_time;
		if(challengetime)
		{
			SAFE_DELETE(challengetime);
		}
	}
	m_vecChallengeTime.clear();

	map <int, _XCastle_Scheduler*>::iterator iter_schedule;
	for(iter_schedule = m_mCastleScheduler.begin() ; iter_schedule != m_mCastleScheduler.end() ; ++iter_schedule)
	{
		_XCastle_Scheduler* scheduler = iter_schedule->second;
		if(scheduler)
		{
			vector <_XCastle_AlarmMsg*>::iterator iter_msg;
			for(iter_msg = scheduler->tAlarmMsg.begin() ; iter_msg != scheduler->tAlarmMsg.end() ; ++iter_msg)
			{
				_XCastle_AlarmMsg* pAlarm = *iter_msg;
				if(pAlarm)
				{
					SAFE_DELETE(pAlarm);
				}
			}
			scheduler->tAlarmMsg.clear();

			vector <_XCastle_Animation*>::iterator iter_ani;
			for(iter_ani = scheduler->tAnimation.begin() ; iter_ani != scheduler->tAnimation.end() ; ++iter_ani)
			{
				_XCastle_Animation* pAni = *iter_ani;
				if(pAni)
				{
					SAFE_DELETE(pAni);
				}
			}
			scheduler->tAnimation.clear();

			vector <_XCastle_VisualEffect*>::iterator iter_ve;
			for(iter_ve = scheduler->tVisualEffect.begin() ; iter_ve != scheduler->tVisualEffect.end() ; ++iter_ve)
			{
				_XCastle_VisualEffect* pVE = *iter_ve;
				if(pVE)
				{
					SAFE_DELETE(pVE);
				}
			}
			scheduler->tVisualEffect.clear();

			if(scheduler->szBGMFileName)
			{
				SAFE_DELETE_ARRAY(scheduler->szBGMFileName);
			}

#ifdef _XDEF_CASTLEBATTLE_SECONDEDITION
			vector <_XCastle_MainBGM*>::iterator  iter_bgm;
			for(iter_bgm = scheduler->tMainBGMList.begin() ; iter_bgm != scheduler->tMainBGMList.end() ; ++iter_bgm)
			{
				_XCastle_MainBGM* pBGM = *iter_bgm;
				if(pBGM)
				{
					if(pBGM->szBGMFileName)
						SAFE_DELETE_ARRAY(pBGM->szBGMFileName);
					SAFE_DELETE(pBGM);
				}
			}
			scheduler->tMainBGMList.clear();
#endif

			SAFE_DELETE(scheduler);
		}
	}
	m_mCastleScheduler.clear();

	m_nMapId			= 0;
	m_fChallengeTax		= 0;
	m_nThreatenTime		= 0;
	m_nThreatenDelayTime = 0;
	m_nDeadMaxCnt		= 0;

	for(int i = 0 ; i < 5 ; ++i)
		m_nChallengeMoney[i] = 0;

	SAFE_DELETE_ARRAY(m_szMinimapName);
	m_nMinimapPosition = 0;
}

/*
BOOL _XCastleScheduler::GetCommand(char* cmd, FILE* fileptr, bool bnumber)
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
*/

BOOL _XCastleScheduler::Load(TCHAR* filename)
{
	if(!filename)
		return FALSE;

	if(!g_ScriptArchive.ReOpenPackage())
		return FALSE;
		
	FILE* fileptr = NULL;

	fileptr = g_ScriptArchive.GetPackedFile( filename );

	if( !fileptr )
	{
		_XFatalError("Can't load Manor script");
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	char	cmd[256];
	char	arg1[64], arg2[64], arg3[64]; //, arg4[64];
	BOOL	bEnd = FALSE;
	BOOL	bComplete = FALSE;

	int		before = 0, after = 0, end = 0;

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
							m_tDefaultTax.fDefaultTax = atof(arg2);
						}
					}
					else if(strncmp(cmd, "manor_level_tax_display", strlen("manor_level_tax_display")) == 0)
					{
						if(!ParseManorLevel(fileptr))
							bEnd = TRUE;
					}
					else if(strncmp(cmd, "manor_level", strlen("manor_level")) == 0)
					{
						// 이 정보는 읽지 않음 - display로 대체
						//if(!ParseManorLevel(fileptr))
						//	bEnd = TRUE;
					}
					else if(strncmp(cmd, "manor_open_timer", strlen("manor_open_timer")) == 0)
					{
						if(!ParseManorOpenTimer(fileptr))
							bEnd = TRUE;
					}
					else if(strncmp(cmd, "manor_name", strlen("manor_name")) == 0)
					{
						int n = sscanf(cmd, "%s %s", arg1, arg2);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							m_nMapNameStrindex = atoi(arg2);
						}
					}
					else if(strncmp(cmd, "manor_master", strlen("manor_master")) == 0)
					{
						int n = sscanf(cmd, "%s %s", arg1, arg2);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							m_nMobMasterNameIndex = atoi(arg2);
						}
					}
					else if(strncmp(cmd, "manor_die_penalty", strlen("manor_die_penalty")) == 0)
					{
						if(!ParseManorDiePenalty(fileptr))
							bEnd = TRUE;
					}
					else if(strncmp(cmd, "manor_shout_cnt", strlen("manor_shout_cnt")) == 0)
					{
						int n = sscanf(cmd, "%s %s", arg1, arg2);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							m_nMaxShoutCnt = atoi(arg2);
						}
					}
					else if(strncmp(cmd, "manor_challenge_time", strlen("manor_challenge_time")) == 0)
					{
						if(!ParseManorChallengeTime(fileptr))
							bEnd = TRUE;
					}
					else if(strncmp(cmd, "manor_minimap", strlen("manor_minimap")) == 0)
					{
						int n = sscanf(cmd, "%s %s %s", arg1, arg2, arg3);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							int strlength = strlen(arg2);
							m_szMinimapName = new TCHAR[strlength+1];
							m_szMinimapName[strlength] = 0;
							strcpy(m_szMinimapName, arg2);

							m_nMinimapPosition = atoi(arg3);
						}
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
							m_tDefaultTax.fDefaultTax = atof(arg2);
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
						if(!ParseChallengeMoney(fileptr))
							bEnd = TRUE;

						/*int n = sscanf(cmd, "%s %s", arg1, arg2);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							m_nChallengeMoney = atoi(arg2);
						}*/
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
					else if(strncmp(cmd, "threatener_delay_time", strlen("threatener_delay_time")) == 0)
					{
						int n = sscanf(cmd, "%s %s", arg1, arg2);
						if(n <= 0)
						{
							bEnd = TRUE;
						}
						else
						{
							m_nThreatenDelayTime = atoi(arg2);
						}
					}
					else if(strncmp(cmd, "threatener_bgm", strlen("threatener_bgm")) == 0)
					{
						// 보류
						ParseSkipInfo(fileptr);
//						if(!ParseThreatenBGM(fileptr))
//							bEnd = TRUE;
					}
					else if(strncmp(cmd, "threatener_se", strlen("threatener_se")) == 0)
					{
						ParseSkipInfo(fileptr);
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
								pScheduler->enWarState = (_XCastle_State)(_XEN_CB_END + end);
								end++;
							}

							pScheduler->nTime = atoi(arg3);
							pScheduler->nTime *= 1000;		// ms

							if(!ParseEventTime(fileptr, pScheduler))
							{
								bEnd = TRUE;

								vector <_XCastle_AlarmMsg*>::iterator iter_msg;
								for(iter_msg = pScheduler->tAlarmMsg.begin() ; iter_msg != pScheduler->tAlarmMsg.end() ; ++iter_msg)
								{
									_XCastle_AlarmMsg* pAlarm = *iter_msg;
									if(pAlarm)
									{
										SAFE_DELETE(pAlarm);
									}
								}
								pScheduler->tAlarmMsg.clear();
								vector <_XCastle_Animation*>::iterator iter_ani;
								for(iter_ani = pScheduler->tAnimation.begin() ; iter_ani != pScheduler->tAnimation.end() ; ++iter_ani)
								{
									_XCastle_Animation* pAni = *iter_ani;
									if(pAni)
									{
										SAFE_DELETE(pAni);
									}
								}
								pScheduler->tAnimation.clear();
								vector <_XCastle_VisualEffect*>::iterator iter_ve;
								for(iter_ve = pScheduler->tVisualEffect.begin() ; iter_ve != pScheduler->tVisualEffect.end() ; ++iter_ve)
								{
									_XCastle_VisualEffect* pVE = *iter_ve;
									if(pVE)
									{
										SAFE_DELETE(pVE);
									}
								}
								pScheduler->tVisualEffect.clear();

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

	g_ScriptArchive.ClosePackage();
	return bComplete;
}


BOOL _XCastleScheduler::ParseManorLevel(FILE* fp)
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
							m_tCastleTax[level-1].fDefaultTax = atof(arg2);
							m_tCastleTax[level-1].fAdditiveTax = atof(arg3);
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

BOOL _XCastleScheduler::ParseManorOpenTimer(FILE* fp)
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

BOOL _XCastleScheduler::ParseManorDiePenalty(FILE* fp)
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

BOOL _XCastleScheduler::ParseThreatenBGM(FILE* fp)
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

BOOL _XCastleScheduler::ParseChallengeMoney(FILE* fp)
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
						int index = atoi(arg1);

						if(index >= 1 && index <= 5)
						{
							m_nChallengeMoney[index-1] = atoi(arg2);
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

BOOL _XCastleScheduler::ParseManorChallengeTime(FILE* fp)
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
						_XCastle_OpenTime* pChallengeTime = new _XCastle_OpenTime;
						pChallengeTime->dwHour = atoi(arg1);
						pChallengeTime->dwMinute = atoi(arg2);

						m_vecChallengeTime.push_back(pChallengeTime);
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

BOOL _XCastleScheduler::ParseEventTime(FILE* fp, _XCastle_Scheduler* pScheduler)
{
	char cmd[256];
	char arg1[128], arg2[128];

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
					// buff 정보 읽지 않음
					ParseSkipInfo(fp);
				}
				break;
			case 'm' :
				{
					sscanf(cmd, "%s %s", arg1, arg2);
					
					if(strncmp(arg1, "main_bgm", strlen("main_bgm")) == 0)
					{
						if(strncmp(arg2, "group", strlen("group")) == 0)
						{
							ParseMainBGM(fp, pScheduler, TRUE);
						}
						else
						{
							ParseMainBGM(fp, pScheduler);
						}
					}
					else if(strncmp(arg1, "main_se", strlen("main_se")) == 0)
					{
#ifdef _XDEF_CASTLEBATTLE_SECONDEDITION
						ParseMainSE(fp, pScheduler);
#else
						ParseSkipInfo(fp);
#endif
					}
					else if(strncmp(arg1, "mobdelete", strlen("mobdelete")) == 0)
					{
					}
					else
					{
						ParseSkipInfo(fp);
					}

/*					if(strncmp(cmd, "mobspawn", strlen("mobspawn")) == 0)
					{
						ParseSkipInfo(fp);
					}
					else if(strncmp(cmd, "main_bgm", strlen("main_bgm")) == 0)
					{
						ParseMainBGM(fp, pScheduler);
					}
					else if(strncmp(cmd, "main_se", strlen("main_se")) == 0)
					{
						ParseSkipInfo(fp);
					}
					else if(strncmp(cmd, "manor_reward", strlen("manor_reward")) == 0)
					{
						ParseSkipInfo(fp);
					}*/
				}
				break;
			case 'v' :
				{
					ParseVictoryType(fp);
				}
				break;
			default :
				{
//					ParseSkipInfo(fp);
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bComplete;
}

BOOL _XCastleScheduler::ParseAlarmMsg(FILE* fp, _XCastle_Scheduler* pScheduler)
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
						_XCastle_AlarmMsg* msg = new _XCastle_AlarmMsg;
						msg->dwAlarmMsgType = atoi(arg1);
						msg->nTextIndex = atoi(arg4);
						msg->dwAlarmMsgTarget = GetTargetType(arg2);

						pScheduler->tAlarmMsg.push_back(msg);
						pScheduler->bMsg = TRUE;
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

BOOL _XCastleScheduler::ParseCharAni(FILE* fp, _XCastle_Scheduler* pScheduler)
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
						_XCastle_Animation* pAni = new _XCastle_Animation;
						pAni->nAnimationTarget = GetTargetType(arg1);
						pAni->nAnimationIndex_male = atoi(arg2);
						pAni->nAnimationIndex_female = atoi(arg3);

						pScheduler->tAnimation.push_back(pAni);
						pScheduler->bAni = TRUE;
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

BOOL _XCastleScheduler::ParseEffect(FILE* fp, _XCastle_Scheduler* pScheduler)
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
						_XCastle_VisualEffect* pVE = new _XCastle_VisualEffect;
						pVE->dwVisualEffectTarget = GetTargetType(arg2);
						pVE->dwVisualEffectId = atoi(arg1);

						pScheduler->tVisualEffect.push_back(pVE);
						pScheduler->bVE = TRUE;

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

BOOL _XCastleScheduler::ParseState(FILE* fp, _XCastle_Scheduler* pScheduler)
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
							pScheduler->dwShout = 2;
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
							pScheduler->dwGateState = 0;
						}
						else if(strncmp(arg2, "off", strlen("off")) == 0)
						{
							pScheduler->dwGateState = 1;
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
							pScheduler->dwLightFoot = 0;
						}
						else if(strncmp(arg2, "off", strlen("off")) == 0)
						{
							pScheduler->dwLightFoot = 1;
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

BOOL _XCastleScheduler::ParseMainBGM(FILE* fp, _XCastle_Scheduler* pScheduler, BOOL bGroup)
{
	char cmd[256];
	char arg1[128], arg2[128], arg3[128];

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
#ifdef _XDEF_CASTLEBATTLE_SECONDEDITION
						_XCastle_MainBGM* pMainBGM = new _XCastle_MainBGM;
						pMainBGM->nBGMEndTime = atoi(arg1);

						int strlength = strlen(arg2);
						pMainBGM->szBGMFileName = new TCHAR[strlength+1];
						pMainBGM->szBGMFileName[strlength] = 0;
						strncpy(pMainBGM->szBGMFileName, arg2, strlength);

						if(strncmp(arg3, "loop", 4) == 0)
							pMainBGM->bLoop = TRUE;
						else
							pMainBGM->bLoop = FALSE;

						pMainBGM->nPlayBGM = 1;

						pScheduler->tMainBGMList.push_back(pMainBGM);
#else
						// 임시 적용
						SAFE_DELETE_ARRAY(pScheduler->szBGMFileName);

						pScheduler->nBGMEndTime = atoi(arg1);
						
						int strlength = strlen(arg2);
						pScheduler->szBGMFileName = new TCHAR[strlength+1];
						pScheduler->szBGMFileName[strlength] = 0;
						strncpy(pScheduler->szBGMFileName, arg2, strlength);

						if(strncmp(arg3, "loop", 4) == 0)
							pScheduler->bLoop = TRUE;
						else
							pScheduler->bLoop = FALSE;

						pScheduler->nPlayBGM = 1;
#endif
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

BOOL _XCastleScheduler::ParseVictoryType(FILE* fp)
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
			case 'm' :
				{
					ParseSkipInfo(fp);
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

BOOL _XCastleScheduler::ParseMainSE(FILE* fp, _XCastle_Scheduler* pScheduler)
{
	char cmd[256];

	BOOL bStart = FALSE, bEnd = FALSE;
	char arg1[128], arg2[128], arg3[128];

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
			default :
				{
					if(sscanf(cmd, "%s %s %s", arg1, arg2, arg3) == 3)
					{
						pScheduler->nSEEndTime = atoi(arg1);
						pScheduler->nSEId = g_FMODWrapper.InsertSound(arg2, FSOUND_HW2D);

						if(strncmp(arg3, "loop", 4) == 0)
							pScheduler->bSELoop = TRUE;
						else
							pScheduler->bSELoop = FALSE;
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

BOOL _XCastleScheduler::ParseSkipInfo(FILE* fp)
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

int _XCastleScheduler::GetTargetType(char* arg)
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