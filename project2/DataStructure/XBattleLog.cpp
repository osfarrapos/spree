// XBattleLog.cpp: implementation of the _XBattleLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XBattleLog.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XBattleLog::_XBattleLog()
{
	m_pBattleLogFile = NULL;
	memset(m_FileName, 0, sizeof(m_FileName));
	m_bWriteLog = FALSE;
}

_XBattleLog::~_XBattleLog()
{

}

BOOL _XBattleLog::Initialize(TCHAR* charactername)
{
//	_XSetDefaultPath(_T("./Game"));
//	_XSetDefaultPath();

	SYSTEMTIME systemtime;
	GetLocalTime(&systemtime);

	TCHAR filename[_MAX_PATH];
	memset(filename, 0, sizeof(filename));
	_snprintf(filename, sizeof(filename), _T("\\NDBattle_%s_%d_%d_%d_%d_%d.log"), 
		charactername, systemtime.wYear, systemtime.wMonth, systemtime.wDay, systemtime.wHour, systemtime.wMinute);

	_snprintf(m_FileName, sizeof(m_FileName), _T("%s%s"), gModulePath);

	_XLog("=== Filename [%s]", m_FileName);

	m_pBattleLogFile = fopen(m_FileName, _T("wt"));

	if(!m_pBattleLogFile)
	{
		_XLog("ERROR : Battle log open fail");
		return FALSE;
	}

	return TRUE;
}

void _XBattleLog::Release(void)
{
	if(m_pBattleLogFile)
	{
		fclose(m_pBattleLogFile);
		m_pBattleLogFile = NULL;
	}

	memset(m_FileName, 0, sizeof(m_FileName));
}

void _XBattleLog::Print(char* argv, ...)
{
	if(!m_pBattleLogFile)
		return;
	
	TCHAR szBuffer[1024];
	int	retValue;
	va_list argptr;
	
	va_start(argptr, argv);
	retValue = _vsnprintf(szBuffer, sizeof(szBuffer), argv, argptr);
	va_end(argptr);
	
	fputs(szBuffer, m_pBattleLogFile);
}

void _XBattleLog::Flush(void)
{
	fflush(m_pBattleLogFile);
}

void _XBattleLog::SetTime(void)
{
	if(m_BattleLog.bWrite)
	{
		DumpLogResult();
	}

	GetLocalTime(&m_BattleLog.systemtime);
	m_BattleLog.bWrite = TRUE;
}

void _XBattleLog::SetTargetGroup(int targetgroup)
{
	m_BattleLog.targetgroup = targetgroup;
}

void _XBattleLog::SetTargetType(int targettype)
{
	switch(m_BattleLog.targetgroup)
	{
	case _XPICKINGTARGET_PC :
		{
			m_BattleLog.targettype = targettype;

			_XUser* pUser = g_Userlist.FindUser(targettype);
			if(pUser)
			{
				strcpy(m_BattleLog.targetname, pUser->m_CharacterInfo.charactername);
			}
		}
		break;
	case _XPICKINGTARGET_MOB :
		{
			m_BattleLog.targettype = targettype + 4000;
			
			if(g_MonsterName[targettype])
			{
				strcpy(m_BattleLog.targetname, g_MonsterName[targettype]);
			}
		}
		break;
	case _XPICKINGTARGET_SELF :
		{
			m_BattleLog.targettype = targettype;
			strcpy(m_BattleLog.targetname, g_pLocalUser->m_CharacterInfo.charactername);
		}
		break;
	case _XPICKINGTARGET_FUNCTIONALOBJECT :
		{
			m_BattleLog.targettype = targettype;

			_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_LodTerrain.m_FunctionalObjectManager.FindAttackableFunctionalObject(targettype);
			if(pFunctionalObject)
			{
				strcpy(m_BattleLog.targetname, pFunctionalObject->m_ObjectName);
			}
		}
		break;
	}
}

void _XBattleLog::SetSkillId(int skillid, int skilllevel)
{
	m_BattleLog.skillid = skillid;

	if(skilllevel == 0)
	{
		if(skillid > 0)
		{
			int learnedindex = g_pLocalUser->GetLearnedSkillIndex(skillid);

			if(learnedindex >= 0)
				m_BattleLog.skilllevel = g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel;
			else
				m_BattleLog.skilllevel = 0;
		}
		else
			m_BattleLog.skilllevel = 0;
	}
	else
	{
		m_BattleLog.skilllevel = skilllevel;
	}
}

void _XBattleLog::SetAttackSeq(int seq)
{
	m_BattleLog.attackseq = seq;
}

void _XBattleLog::SetAtkInfo(int atkinfo)
{
	m_BattleLog.atkinfo = atkinfo;
}

void _XBattleLog::SetDamage(int damage_1, int damage_2, int damage_3)
{
	m_BattleLog.damage[0] = damage_1;
	m_BattleLog.damage[1] = damage_2;
	m_BattleLog.damage[2] = damage_3;
}

void _XBattleLog::SetEffect(int id, int level)
{
	if(m_BattleLog.skillid == id)
	{
		for(int i = 0 ; i < g_SkillProperty[id]->detailInfo[level].effectCount ; ++i)
		{
			m_BattleLog.effid[i] = g_SkillProperty[id]->detailInfo[level].effect[i].id;
			m_BattleLog.effvalue[i] = g_SkillProperty[id]->detailInfo[level].effect[i].value;
			m_BattleLog.efftime[i] = g_SkillProperty[id]->detailInfo[level].effectTime;
		}

		for(i = 0 ; i < g_SkillProperty[id]->detailInfo[level].additiveCount ; ++i)
		{
			m_BattleLog.addeffid[i] = g_SkillProperty[id]->detailInfo[level].additiveEffect[i].id;
			m_BattleLog.addeffvalue[i] = g_SkillProperty[id]->detailInfo[level].additiveEffect[i].value;
			m_BattleLog.addefftime[i] = g_SkillProperty[id]->detailInfo[level].additiveEffect[i].count;
		}
	}
	else
	{
		SetTime();
		SetSkillId(id, level);

		for(int i = 0 ; i < g_SkillProperty[id]->detailInfo[level].effectCount ; ++i)
		{
			m_BattleLog.effid[i] = g_SkillProperty[id]->detailInfo[level].effect[i].id;
			m_BattleLog.effvalue[i] = g_SkillProperty[id]->detailInfo[level].effect[i].value;
			m_BattleLog.efftime[i] = g_SkillProperty[id]->detailInfo[level].effectTime;
		}
		
		for(i = 0 ; i < g_SkillProperty[id]->detailInfo[level].additiveCount ; ++i)
		{
			m_BattleLog.addeffid[i] = g_SkillProperty[id]->detailInfo[level].additiveEffect[i].id;
			m_BattleLog.addeffvalue[i] = g_SkillProperty[id]->detailInfo[level].additiveEffect[i].value;
			m_BattleLog.addefftime[i] = g_SkillProperty[id]->detailInfo[level].additiveEffect[i].count;
		}
	}
}

void _XBattleLog::DumpLogResult(void)
{
	Print(_T("%d/%d\t%d:%d:%d\t"), m_BattleLog.systemtime.wMonth, m_BattleLog.systemtime.wDay, m_BattleLog.systemtime.wHour, m_BattleLog.systemtime.wMinute, m_BattleLog.systemtime.wSecond);
	
	Print(_T("%d\t%d\t%d\t"), g_pLocalUser->m_Strength.Get_result(), g_pLocalUser->m_Intelligence.Get_result(), g_pLocalUser->m_Dex.Get_result());
	
	switch(m_BattleLog.targetgroup)
	{
	case 1 :
		{
			Print(_T("Player\t"));
		}
		break;
	case 2 :
		{
			Print(_T("Monster\t"));
		}
		break;
	case 7 :
		{
			Print(_T("Self\t"));
		}
		break;
	case 9 :
		{
			Print(_T("Functional\t"));
		}
		break;
	}

	Print(_T("%s\t"), m_BattleLog.targetname);
	Print(_T("%d\t"), m_BattleLog.targettype);

	Print(_T("%d\t"), m_BattleLog.skillid);

	if(m_BattleLog.skillid > 0)
	{
		Print(_T("%s\t"), g_SkillProperty[m_BattleLog.skillid]->skillName);
	}
	else
	{
		Print(_T("NormalAttack\t"));
	}

	Print(_T("%d\t"), m_BattleLog.skilllevel);

	Print(_T("%d\t"), m_BattleLog.attackseq);

	if(m_BattleLog.skillid > 0)
	{
		Print(_T("%d\t%d\t%d\t"), g_SkillProperty[m_BattleLog.skillid]->skillCType, g_SkillProperty[m_BattleLog.skillid]->skillSType, g_SkillProperty[m_BattleLog.skillid]->usefulType);
	}
	else
	{
		Print(_T("0\t0\t0\t"));
	}

	switch(m_BattleLog.atkinfo)
	{
	case 0 : 
		Print(_T("Hit\t"));
		break;
	case 1 :
		Print(_T("Critical\t"));
		break;
	case 2 :
		Print(_T("Miss\t"));
		break;
	case 3 :
		Print(_T("Block\t"));
		break;
	case 4 :
		Print(_T("Shock\t"));
		break;
	case 5 :
		Print(_T("Conflic_tHit\t"));
		break;
	case 6 :
		Print(_T("Conflict_Critical\t"));
		break;
	default :
		Print(_T("Unknown\t"));
		break;
	}

	Print(_T("%d\t%d\t%d\t%d\t"), m_BattleLog.damage[0], m_BattleLog.damage[1], m_BattleLog.damage[2], m_BattleLog.damage[0]+m_BattleLog.damage[1]+m_BattleLog.damage[2]);

	if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN)
	{
		for(int i = 0 ; i < 8 ; ++i)
		{
			Print(_T("%d\t%d\t%d\t"), m_BattleLog.effid[i], m_BattleLog.effvalue[i], m_BattleLog.efftime[i]);
		}

		for(i = 0 ; i < 4 ; ++i)
		{
			Print(_T("%d\t%d\t%d\t"), m_BattleLog.addeffid[i], m_BattleLog.addeffvalue[i], m_BattleLog.addefftime[i]);
		}
	}

	Print(_T("\n"));
	Flush();

	memset(&m_BattleLog, 0, sizeof(m_BattleLog));
}

void _XBattleLog::DumpLogEffect(void)
{
	if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN)
	{
		Print(_T("%d/%d\t%d:%d:%d\t"), m_BattleLog.systemtime.wMonth, m_BattleLog.systemtime.wDay, m_BattleLog.systemtime.wHour, m_BattleLog.systemtime.wMinute, m_BattleLog.systemtime.wSecond);
		
		Print(_T("%d\t%d\t%d\t"), g_pLocalUser->m_Strength.Get_result(), g_pLocalUser->m_Intelligence.Get_result(), g_pLocalUser->m_Dex.Get_result());
		
		Print(_T("Effect\t"));
		
		Print(_T("<NULL>\t"));
		Print(_T("0\t"));
		
		Print(_T("%d\t"), m_BattleLog.skillid);
		
		if(m_BattleLog.skillid > 0)
		{
			Print(_T("%s\t"), g_SkillProperty[m_BattleLog.skillid]->skillName);
		}
		else
		{
			Print(_T("NormalAttack\t"));
		}
		
		Print(_T("%d\t"), m_BattleLog.skilllevel);
		
		Print(_T("%d\t"), m_BattleLog.attackseq);
		
		if(m_BattleLog.skillid > 0)
		{
			Print(_T("%d\t%d\t%d\t"), g_SkillProperty[m_BattleLog.skillid]->skillCType, g_SkillProperty[m_BattleLog.skillid]->skillSType, g_SkillProperty[m_BattleLog.skillid]->usefulType);
		}
		else
		{
			Print(_T("0\t0\t0\t"));
		}
		
		Print(_T("<None>\t"));
		
		Print(_T("0\t0\t0\t0\t"));

		for(int i = 0 ; i < 8 ; ++i)
		{
			Print(_T("%d\t%d\t%d\t"), m_BattleLog.effid[i], m_BattleLog.effvalue[i], m_BattleLog.efftime[i]);
		}

		for(i = 0 ; i < 4 ; ++i)
		{
			Print(_T("%d\t%d\t%d\t"), m_BattleLog.addeffid[i], m_BattleLog.addeffvalue[i], m_BattleLog.addefftime[i]);
		}
		
		Print(_T("\n"));
		Flush();
		
		memset(&m_BattleLog, 0, sizeof(m_BattleLog));
	}
}