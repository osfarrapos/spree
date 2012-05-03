// XMatchEventManager.cpp: implementation of the _XMatchEventManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XMatchEventManager.h"
#include "XCastleScheduler.h"
#include "XSR_STRINGHEADER.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XMatchEventManager::_XMatchEventManager()
{
	m_nCurrentState = 0;
	memset(&m_CurrentMatchInfo, 0, sizeof(m_CurrentMatchInfo));
	
	m_bDrawReadyMessage = FALSE;
	m_dwReadyMessageStartTime = 0;
}

_XMatchEventManager::~_XMatchEventManager()
{
	Release();
}

void _XMatchEventManager::Release()
{
	_map_MatchTypeStr::iterator iter_match;
	for(iter_match = m_mapMatchTypeList.begin() ; iter_match != m_mapMatchTypeList.end() ; ++iter_match)
	{
		_s_Match_Type* pMatchType = iter_match->second;
		if(pMatchType)
		{
			pMatchType->vSkillConstraints.clear();

			_v_Item_Str::iterator iter_item;
			for(iter_item = pMatchType->vItemConstraints.begin() ; iter_item != pMatchType->vItemConstraints.end() ; ++iter_item)
			{
				_s_Item_Str* pItem = *iter_item;
				SAFE_DELETE(pItem);
			}
			pMatchType->vItemConstraints.clear();

			_map_Match_State::iterator iter_state;
			for(iter_state = pMatchType->mapMatchState.begin() ; iter_state != pMatchType->mapMatchState.end() ; ++iter_state)
			{
				_s_Match_State* pState = iter_state->second;
				if(pState)
				{
					_v_Match_Notice::iterator iter_notice;
					for(iter_notice = pState->vMatchNotice.begin() ; iter_notice != pState->vMatchNotice.end() ; ++iter_notice)
					{
						_s_Match_Notice* pNotice = *iter_notice;
						SAFE_DELETE(pNotice);
					}
					pState->vMatchNotice.clear();
				}
				SAFE_DELETE(pState);
			}
			pMatchType->mapMatchState.clear();

			map<int, _s_Action_Constraint*>::iterator iter_action;
			for(iter_action = pMatchType->mapActionConstraints.begin() ; iter_action != pMatchType->mapActionConstraints.end() ; ++iter_action)
			{
				_s_Action_Constraint* pAction = iter_action->second;
				SAFE_DELETE(pAction);
			}
			pMatchType->mapActionConstraints.clear();
		}
		SAFE_DELETE(pMatchType);
	}
	m_mapMatchTypeList.clear();
}

void _XMatchEventManager::Draw()
{
	if(m_bDrawReadyMessage)
	{
		DWORD elapsedtime = g_LocalSystemTime - m_dwReadyMessageStartTime;
		if(elapsedtime <= 8000)	// 8초 동안 보여줌
		{	
			TCHAR messagestr1[512];
			memset(messagestr1, 0, sizeof(messagestr1));
			_snprintf(messagestr1, sizeof(messagestr1), _XGETINTERFACETEXT(ID_STRING_NEW_3729), m_nTextArg);//%d분 후에 비무대회가 시작됩니다.

			TCHAR messagestr2[512];
			memset(messagestr2, 0, sizeof(messagestr2));

			int fromlevel = g_UserLevelTable[g_MatchEventManager.GetMinLevel()].level;
			int tolevel = g_UserLevelTable[g_MatchEventManager.GetMaxLevel()].level;

			if(fromlevel == tolevel)
			{
				_snprintf(messagestr2, sizeof(messagestr2), _XGETINTERFACETEXT(ID_STRING_NEW_3740), g_LevelName[fromlevel]);//%s 성취단계  분들은 합비 비무장으로 모여주시길  바랍니다.
			}
			else
			{
				_snprintf(messagestr2, sizeof(messagestr2), _XGETINTERFACETEXT(ID_STRING_NEW_3741), g_LevelName[fromlevel], g_LevelName[tolevel]);	//%s-%s 성취단계 분들은 합비 비무장으로 모여주시길 바랍니다.
			}

			g_XLargeFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
			g_XLargeFont->SetColor(D3DCOLOR_ARGB(255, 250, 230, 169));

			if(messagestr1[0] != 0)
				g_XLargeFont->Puts_SeparateAlign((gnWidth>>1) , (gnHeight>>1)-145+10, messagestr1, 700, _XFONT_ALIGNTYPE_CENTER);

			if(messagestr2[0] != 0)
				g_XLargeFont->Puts_SeparateAlign((gnWidth>>1) , (gnHeight>>1)-145+30, messagestr2, 700, _XFONT_ALIGNTYPE_CENTER);

			g_XLargeFont->Flush();
		}
		else
		{
			if(m_bDrawReadyMessage)
				m_bDrawReadyMessage = FALSE;
		}
	}
}

BOOL _XMatchEventManager::LoadMatchEventScript(void)
{
	// load 전체 장원전 정보 스크립트	
	if(!g_ScriptArchive.ReOpenPackage())
		return FALSE;
	
	TCHAR filename[64];
	memset( filename, 0, sizeof(TCHAR) * 64 );
	strcpy( filename, _T("PVP_match.txt") );
	
	FILE* fileptr = NULL;
	fileptr = g_ScriptArchive.GetPackedFile( filename );
	
	if( !fileptr )
	{
		_XFatalError("Can't load PVP_match");
		g_ScriptArchive.ClosePackage();		
		return FALSE;
	}

	TCHAR cmd[256];
	TCHAR arg1[128], arg2[128];
	int mapid = 0;
	BOOL bEnd = FALSE, bComplete = FALSE;

	while(1)
	{
		while(GetCommand(cmd, fileptr))
		{
			switch(cmd[0])
			{
			case 'm' :
				{
					if(sscanf(cmd, "%s %s", arg1, arg2) == 2)
					{
						_s_Match_Type* pMatchType = new _s_Match_Type;
						int type = atoi(arg2);
						if(!ParseMatchType(fileptr, pMatchType))
						{
							SAFE_DELETE(pMatchType);
						}
						else
						{
							m_mapMatchTypeList[type] = pMatchType;
							bEnd = TRUE;
							bComplete = TRUE;
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

	if(!bComplete)
	{
		Release();
	}

	g_ScriptArchive.ClosePackage();

	return bComplete;
}

BOOL _XMatchEventManager::ParseMatchType(FILE* fp, _s_Match_Type* pMatchType)
{
	char cmd[256];
	char arg1[64];

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
					if(sscanf(cmd, "%s", arg1) == 1)
					{
						if(strncmp(arg1, "constraint1", strlen("constraint1")) == 0)
						{
							ParseConstraint1(fp, pMatchType);
						}
						else if(strncmp(arg1, "constraint2", strlen("constraint2")) == 0)
						{
							ParseConstraint2(fp, pMatchType);
						}
						else if(strncmp(arg1, "constraint3", strlen("constraint3")) == 0)
						{
							ParseConstraint3(fp, pMatchType);
						}
						else if(strncmp(arg1, "ready", strlen("ready")) == 0)
						{
							ParseReady(fp, pMatchType);
						}
						else if(strncmp(arg1, "start", strlen("start")) == 0)
						{
							ParseStart(fp, pMatchType);
						}
						else if(strncmp(arg1, "end", strlen("end")) == 0)
						{
							ParseEnd(fp, pMatchType);
						}
						else if(strncmp(arg1, "camera", strlen("camera")) == 0)
						{
							ParseTemp(fp);
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

BOOL _XMatchEventManager::ParseConstraint1(FILE* fp, _s_Match_Type* pMatchType)
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
						if(strncmp(arg1, "hyperrun", strlen("hyperrun")) == 0)
						{
							if(strncmp(arg2, "off", strlen("off")) == 0)
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 1;//off
								pMatchType->mapActionConstraints[_xen_const_hyperrun] = pAction;
							}
							else
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 0;//on
								pMatchType->mapActionConstraints[_xen_const_hyperrun] = pAction;
							}
						}
						else if(strncmp(arg1, "breath", strlen("breath")) == 0)
						{
							if(strncmp(arg2, "off", strlen("off")) == 0)
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 1;//off
								pMatchType->mapActionConstraints[_xen_const_breath] = pAction;
							}
							else
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 0;//on
								pMatchType->mapActionConstraints[_xen_const_breath] = pAction;
							}
						}
						else if(strncmp(arg1, "logout", strlen("logout")) == 0)
						{
							if(strncmp(arg2, "off", strlen("off")) == 0)
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 1;//off
								pMatchType->mapActionConstraints[_xen_const_logout] = pAction;
							}
							else
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 0;//on
								pMatchType->mapActionConstraints[_xen_const_logout] = pAction;
							}
						}
						else if(strncmp(arg1, "escape", strlen("escape")) == 0)
						{
							if(strncmp(arg2, "off", strlen("off")) == 0)
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 1;//off
								pMatchType->mapActionConstraints[_xen_const_escape] = pAction;
							}
							else
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 0;//on
								pMatchType->mapActionConstraints[_xen_const_escape] = pAction;
							}
						}
						else if(strncmp(arg1, "party", strlen("party")) == 0)
						{
							if(strncmp(arg2, "off", strlen("off")) == 0)
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 1;//off
								pMatchType->mapActionConstraints[_xen_const_party] = pAction;
							}
							else
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 0;//on
								pMatchType->mapActionConstraints[_xen_const_party] = pAction;
							}
						}
						else if(strncmp(arg1, "trade", strlen("trade")) == 0)
						{
							if(strncmp(arg2, "off", strlen("off")) == 0)
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 1;//off
								pMatchType->mapActionConstraints[_xen_const_trade] = pAction;
							}
							else
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 0;//on
								pMatchType->mapActionConstraints[_xen_const_trade] = pAction;
							}
						}
						else if(strncmp(arg1, "info", strlen("info")) == 0)
						{
							if(strncmp(arg2, "off", strlen("off")) == 0)
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 1;//off
								pMatchType->mapActionConstraints[_xen_const_info] = pAction;
							}
							else
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 0;//on
								pMatchType->mapActionConstraints[_xen_const_info] = pAction;
							}
						}
						else if(strncmp(arg1, "recall", strlen("recall")) == 0)
						{
							if(strncmp(arg2, "off", strlen("off")) == 0)
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 1;//off
								pMatchType->mapActionConstraints[_xen_const_recall] = pAction;
							}
							else
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 0;//on
								pMatchType->mapActionConstraints[_xen_const_recall] = pAction;
							}
						}
						else if(strncmp(arg1, "shop", strlen("shop")) == 0)
						{
							if(strncmp(arg2, "off", strlen("off")) == 0)
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 1;//off
								pMatchType->mapActionConstraints[_xen_const_shop] = pAction;
							}
							else
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 0;//on
								pMatchType->mapActionConstraints[_xen_const_shop] = pAction;
							}
						}
						else if(strncmp(arg1, "hide_me", strlen("hide_me")) == 0)
						{
							if(strncmp(arg2, "off", strlen("off")) == 0)
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 1;//off
								pMatchType->mapActionConstraints[_xen_const_hideme] = pAction;
							}
							else
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 0;//on
								pMatchType->mapActionConstraints[_xen_const_hideme] = pAction;
							}
						}
						else if(strncmp(arg1, "pvp_event", strlen("pvp_event")) == 0)
						{
							if(strncmp(arg2, "off", strlen("off")) == 0)
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 1;//off
								pMatchType->mapActionConstraints[_xen_const_pvp_pr] = pAction;
							}
							else
							{
								_s_Action_Constraint* pAction = new _s_Action_Constraint;
								pAction->nOnOff = 0;//on
								pMatchType->mapActionConstraints[_xen_const_pvp_pr] = pAction;
							}
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

BOOL _XMatchEventManager::ParseConstraint2(FILE* fp, _s_Match_Type* pMatchType)
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
						if(strncmp(arg1, "use_item", strlen("use_item")) == 0)
						{
							_s_Item_Str* pItem = new _s_Item_Str;
							pItem->cType = atoi(arg2);
							pItem->sID = atoi(arg3);

							pMatchType->vItemConstraints.push_back(pItem);
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

BOOL _XMatchEventManager::ParseConstraint3(FILE* fp, _s_Match_Type* pMatchType)
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
						if(strncmp(arg1, "use_skill", strlen("use_skill")) == 0)
						{
							pMatchType->vSkillConstraints.push_back(atoi(arg2));
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

BOOL _XMatchEventManager::ParseReady(FILE* fp, _s_Match_Type* pMatchType)
{
	char cmd[256];
	char arg1[64], arg2[64], arg3[64], arg4[64], arg5[64];

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

	_s_Match_State* pState = new _s_Match_State;
	pState->nWhen = 0;	// ready

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
					pMatchType->mapMatchState[0] = pState;
				}
				break;
			default :
				{
					if(sscanf(cmd, "%s %s %s %s %s", arg1, arg2, arg3, arg4, arg5) == 5)
					{
						if(strncmp(arg2, "notice", strlen("notice")) == 0)
						{
							_s_Match_Notice* pNotice = new _s_Match_Notice;
							pNotice->nTime = GetTime(arg1);
							pNotice->nToType = 0;
							pNotice->nCondition = 0;
							pNotice->nTextID = atoi(arg4);
							pNotice->nParam = atoi(arg5);

							pState->vMatchNotice.push_back(pNotice);
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

BOOL _XMatchEventManager::ParseStart(FILE* fp, _s_Match_Type* pMatchType)
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

	_s_Match_State* pState = new _s_Match_State;
	pState->nWhen = 1;	// start

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
					pMatchType->mapMatchState[1] = pState;
				}
				break;
			default :
				{
					if(sscanf(cmd, "%s %s %s %s %s", arg1, arg2, arg3, arg4) == 4)
					{
						if(strncmp(arg2, "notice", strlen("notice")) == 0)
						{
							_s_Match_Notice* pNotice = new _s_Match_Notice;
							pNotice->nTime = GetTime(arg1);
							pNotice->nToType = 0;
							pNotice->nCondition = 0;
							pNotice->nTextID = atoi(arg4);
							pNotice->nParam = 0;

							pState->vMatchNotice.push_back(pNotice);
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

BOOL _XMatchEventManager::ParseEnd(FILE* fp, _s_Match_Type* pMatchType)
{
	char cmd[256];
	char arg1[64], arg2[64], arg3[64], arg4[64], arg5[64];

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

	_s_Match_State* pState = new _s_Match_State;
	pState->nWhen = 2;	// end

	while(!bEnd && !feof(fp))
	{
		while(GetCommand(cmd, fp, true))
		{
			switch(cmd[0])
			{
			case '}' :
				{
					bEnd = TRUE;
					pMatchType->mapMatchState[2] = pState;
				}
				break;
			default :
				{
					if(sscanf(cmd, "%s %s %s %s %s %s", arg1, arg2, arg3, arg4, arg5) <= 5)
					{
						if(strncmp(arg2, "notice_winner", strlen("notice_winner")) == 0)
						{
							_s_Match_Notice* pNotice = new _s_Match_Notice;
							pNotice->nTime = GetTime(arg1);
							pNotice->nToType = 0;
							pNotice->nCondition = 0;
							pNotice->nTextID = atoi(arg4);
							pNotice->nParam = 0;

							pState->vMatchNotice.push_back(pNotice);
						}
						else if(strncmp(arg2, "notice_nowinner", strlen("notice_nowinner")) == 0)
						{
							_s_Match_Notice* pNotice = new _s_Match_Notice;
							pNotice->nTime = GetTime(arg1);
							pNotice->nToType = 0;
							pNotice->nCondition = 1;
							pNotice->nTextID = atoi(arg4);
							pNotice->nParam = 0;

							pState->vMatchNotice.push_back(pNotice);
						}
						else if(strncmp(arg2, "to_winner", strlen("to_winner")) == 0)
						{
							_s_Match_Notice* pNotice = new _s_Match_Notice;
							pNotice->nTime = GetTime(arg1);
							pNotice->nToType = 1;
							pNotice->nCondition = 0;
							pNotice->nTextID = atoi(arg4);
							pNotice->nParam = 0;

							pState->vMatchNotice.push_back(pNotice);
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

BOOL _XMatchEventManager::ParseTemp(FILE* fp)
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
				}
				break;
			}
			if(bEnd)
				break;
		}
	}

	return bEnd;
}

// 초를 알려준다.
int _XMatchEventManager::GetTime(TCHAR* arg)
{
	int time = 0;
	int strlength = strlen(arg);
	
	if(strcmp(&arg[strlength-1], "m") == 0)	//분
	{
		arg[strlength-1] = 0;
		time = atoi(arg);
		time *= 60;
	}
	else if(strcmp(&arg[strlength-1], "s") == 0)	//초
	{
		arg[strlength-1] = 0;
		time = atoi(arg);
	}

	return time;
}

void _XMatchEventManager::SetCurrentMatchInfo(__fm_info_in_packet* info)
{
	m_CurrentMatchInfo.nType = info->match_type;
	m_CurrentMatchInfo.nAreaIndex = info->field_index;

	m_CurrentMatchInfo.nMinLevel = info->fm_level;
	m_CurrentMatchInfo.nMaxLevel = info->to_level;

	m_CurrentMatchInfo.nRewardIndex = info->reward_index;

	m_CurrentMatchInfo.sStartTime.dwYear = info->start_time.uiYear;
	m_CurrentMatchInfo.sStartTime.dwMonth = info->start_time.uiMonth;
	m_CurrentMatchInfo.sStartTime.dwDay = info->start_time.uiDay;
	m_CurrentMatchInfo.sStartTime.dwHour = info->start_time.uiHour;
	m_CurrentMatchInfo.sStartTime.dwMinute = info->start_time.uiMinute;
	m_CurrentMatchInfo.sStartTime.dwSecond = info->start_time.uiSecond;

	m_CurrentMatchInfo.sEndTime.dwYear = info->end_time.uiYear;
	m_CurrentMatchInfo.sEndTime.dwMonth = info->end_time.uiMonth;
	m_CurrentMatchInfo.sEndTime.dwDay = info->end_time.uiDay;
	m_CurrentMatchInfo.sEndTime.dwHour = info->end_time.uiHour;
	m_CurrentMatchInfo.sEndTime.dwMinute = info->end_time.uiMinute;
	m_CurrentMatchInfo.sEndTime.dwSecond = info->end_time.uiSecond;
}

void _XMatchEventManager::GetLeftTimeMatchEvent(int& minute, int& second)
{
	// 시작, 끝 날짜는 모두 같다는 것을 전제로 함. 초는 무시한다.
	if(m_nCurrentState == 2)
	{
		int eventhour = m_CurrentMatchInfo.sStartTime.dwHour - m_CurrentMatchInfo.sEndTime.dwHour - 1;
		
		int eventminute = 0;
		if(m_CurrentMatchInfo.sEndTime.dwMinute > m_CurrentMatchInfo.sStartTime.dwMinute)
		{
			eventminute = m_CurrentMatchInfo.sEndTime.dwMinute - m_CurrentMatchInfo.sStartTime.dwMinute;
		}
		else
		{
			eventminute = (60 - m_CurrentMatchInfo.sStartTime.dwMinute) + m_CurrentMatchInfo.sEndTime.dwMinute;
		}

		if(eventhour > 0)
		{
			eventminute += eventhour*60;
		}

		DWORD eventmillisecond = eventminute*60*1000;				// 총 비무 시간을 millisecond로
		DWORD elapsedtime = g_LocalSystemTime - m_dwEventStartTime;	// 현재까지 지나간 시간

		if(eventmillisecond >= elapsedtime)
		{
			DWORD lefttime = eventmillisecond - elapsedtime;		// 남은 시간 (millisecond)

			minute = (lefttime / 1000) / 60;
			second = (lefttime / 1000) % 60;
		}
		else
		{
			minute = 0;
			second = 0;
		}
	}
	else
	{
		// 시작 상태가 아닐때
		minute = 0;
		second = 0;
	}
}

int _XMatchEventManager::GetMinLevel()
{
	if(m_nCurrentState != 0)
	{
		return m_CurrentMatchInfo.nMinLevel;
	}
	return 0;
}

int _XMatchEventManager::GetMaxLevel()
{
	if(m_nCurrentState != 0)
	{
		return m_CurrentMatchInfo.nMaxLevel;
	}
	return 0;
}

void _XMatchEventManager::GetStartDate(_XTimeUnit& time)
{
	time = m_CurrentMatchInfo.sStartTime;
}

BOOL _XMatchEventManager::CanUseItem(int invennumber)
{
	if(g_pLocalUser->m_UserItemList[invennumber].Get_m_cType() != _XGI_FC_POTION)
		return TRUE;

	_map_MatchTypeStr::iterator iter_match = m_mapMatchTypeList.find(m_CurrentMatchInfo.nType);
	if(iter_match != m_mapMatchTypeList.end())
	{
		_s_Match_Type* pMatchStr = iter_match->second;
		if(pMatchStr)
		{
			_v_Item_Str::iterator iter_item;
			for(iter_item = pMatchStr->vItemConstraints.begin() ; iter_item != pMatchStr->vItemConstraints.end() ; ++iter_item)
			{
				_s_Item_Str* pItem = *iter_item;
				if(g_pLocalUser->m_UserItemList[invennumber].Get_m_cType() == pItem->cType && 
					g_pLocalUser->m_UserItemList[invennumber].Get_m_sID() == pItem->sID)
				{
					// 사용할 수 있는 아이템
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL _XMatchEventManager::CanUseSkill(int skillid)
{
	_map_MatchTypeStr::iterator iter_match = m_mapMatchTypeList.find(m_CurrentMatchInfo.nType);
	if(iter_match != m_mapMatchTypeList.end())
	{
		_s_Match_Type* pMatchStr = iter_match->second;
		if(pMatchStr)
		{
			_v_Skill_Str::iterator iter_skill;
			for(iter_skill = pMatchStr->vSkillConstraints.begin() ; iter_skill != pMatchStr->vSkillConstraints.end() ; ++iter_skill)
			{
				if(skillid == *iter_skill)
				{
					// 사용할 수 없는 스킬
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

int _XMatchEventManager::GetActionConstraint(int action)
{
	_map_MatchTypeStr::iterator iter_match = m_mapMatchTypeList.find(m_CurrentMatchInfo.nType);
	if(iter_match != m_mapMatchTypeList.end())
	{
		_s_Match_Type* pMatchStr = iter_match->second;
		if(pMatchStr)
		{
			map<int, _s_Action_Constraint*>::iterator iter_action = pMatchStr->mapActionConstraints.find(action);
			if(iter_action != pMatchStr->mapActionConstraints.end())
			{
				_s_Action_Constraint* pConstraint = iter_action->second;
				if(pConstraint)
				{
					return pConstraint->nOnOff;
				}
			}
		}
	}
	return 0;	// default : on
}