// XSkillItem.cpp: implementation of the _XSkillItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "XSkillItem.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

#pragma warning (disable : 4786)
#include <map>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

// skill concept
_XSI_SkillConceptStruct				g_SkillConceptProperty[9];

// skill
map <short, _XSKILLINFOSTRUCTURE*> g_SkillProperty;
int						g_SkillGroupStartIndex[12];
map <int, _XSI_LEARNSKILL_PACKAGE>	g_SkillPackage;

// mastery skill
map <short, _XSI_MasterySkillStruct*>		g_SkillMasteryProperty;

// 상태효과로 인한 스킬 테이블 내용 변경을 위한 구조체
map <short, _XSKILLINFO_MOD_STRUCTURE*> g_Skill_Mod_Property;


extern void ReplaceCharacter(LPTSTR _string, TCHAR srcchar, TCHAR changechar);

//////////////////////////////////////////////////////////////////////
// User가 소유한 스킬 class
//////////////////////////////////////////////////////////////////////
_XUserSkill::_XUserSkill()
{
	m_sSkillID		=	0;
	m_cSkillLevel	=	-1;
	m_cCurMission	=	0;
	m_iSkillExp		=	0;
	m_dwCooldownStartTime = 0;
	m_ucSkillState	=	0;
	m_cSelectedSkillLevel = 0;
}

_XUserSkill::~_XUserSkill()
{
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XSkillItem::_XSkillItem()
{
}

_XSkillItem::~_XSkillItem()
{
}

BOOL _XSkillItem::ReLoadSkillProperty( void )
{
	map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
	for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
	{
		_XSKILLINFOSTRUCTURE* skillinfo = iter_skill->second;
		if(skillinfo)
		{
			for(int i = 0 ; i < 12 ; ++i)
			{
				if(skillinfo->detailInfo[i].effect)
				{
					delete[] skillinfo->detailInfo[i].effect;
					skillinfo->detailInfo[i].effect = NULL;
				}
				if(skillinfo->detailInfo[i].additiveEffect)
				{
					delete[] skillinfo->detailInfo[i].additiveEffect;
					skillinfo->detailInfo[i].additiveEffect = NULL;
				}
			}
			delete skillinfo;
		}
	}
	g_SkillProperty.clear();

	return LoadSkillProperty();
}

BOOL _XSkillItem::LoadSkillProperty( void )
{
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;
	
#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		_XDeleteFile(_T("skill_table.bsk"));
	}		
#endif

	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );
	
	FILE* fileptr = NULL;


#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		
		fileptr = fopen( _T("skill_table.bsk"), "rb" );
			
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( _T("skill_table.bsk") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("skill_table.bsk") );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( _T("skill_table.bsk"), "rb" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( _T("skill_table.bsk") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("skill_table.bsk") );
	}
#endif

	if( !fileptr )
	{
		_XFatalError( "Load skill table" );
		if( bLoadFromFile ) fclose(fileptr);
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	// Read Header
	TCHAR		fileheadername[_XDEF_SKILLTABLE_HEADERNAMELENGTH + 1];
	short		version[2];
	SYSTEMTIME	updatetime;
	TCHAR		username[16];
	TCHAR		comname[20];

	memset(fileheadername, 0, sizeof(TCHAR)*(_XDEF_SKILLTABLE_HEADERNAMELENGTH + 1));
	memset(username, 0, sizeof(username));
	memset(comname, 0, sizeof(comname));
	if(fread(fileheadername, sizeof(TCHAR)*_XDEF_SKILLTABLE_HEADERNAMELENGTH, 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose(fileptr);
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(&version[0], sizeof(short), 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose(fileptr);
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(&version[1], sizeof(short), 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose(fileptr);
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(&updatetime, sizeof(SYSTEMTIME), 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose(fileptr);
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(username, sizeof(TCHAR)*16, 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose(fileptr);
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(comname, sizeof(TCHAR)*20, 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose(fileptr);
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	// Read Skill information
	short		skillid = 0;
	for(int i = 0 ; i < _XDEF_MAXSKILLCOUNT ; ++i)
	{
		// index
		if(fread(&skillid, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		if(skillid == -1)
			break;
		
		_XSKILLINFOSTRUCTURE *skillinfo;
		skillinfo = new _XSKILLINFOSTRUCTURE;

		memset(skillinfo, 0, sizeof(_XSKILLINFOSTRUCTURE));

		// skill name
		unsigned char strlength;
		if(fread(&strlength, sizeof(unsigned char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

#ifdef _XDEF_NEWSTRINGMANAGER
		skillinfo->skillName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_SKILL_TABLE_XLS_NAME, skillid );

#ifdef _XJAPANESE
		skillinfo->skillHiraganaName = g_StringDataBase.GetSubTextString( _XSTRINGDB_SN_SKILL_TABLE_XLS_NAME, skillid );
#endif

		fseek( fileptr, sizeof(TCHAR) * strlength, SEEK_CUR );		
#else
		if(strlength > 0)
		{
			if(fread(skillinfo->skillName, sizeof(TCHAR), strlength, fileptr) < 1)
			{
				if( bLoadFromFile ) fclose(fileptr);
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}
		}
#endif

		// skill grade
		if(fread(&skillinfo->skillGrade, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill ctype
		if(fread(&skillinfo->skillCType, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// skill stype
		if(fread(&skillinfo->skillSType, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// action type
		if(fread(&skillinfo->actionType, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// useful type
		if(fread(&skillinfo->usefulType, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// target type
		if(fread(&skillinfo->targetType, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// clan type
		if(fread(&skillinfo->clanType, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// weapon ctype
		if(fread(&skillinfo->weaponCType, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// weapon stype
		if(fread(&skillinfo->weaponSType, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// class grade
		if(fread(&skillinfo->classGrade, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// requirement skill 01
		if(fread(&skillinfo->requirementSkill01, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// requirement skill 02
		if(fread(&skillinfo->requirementSkill02, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// requirement condition
		if(fread(&skillinfo->requirementCondition, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// inside type
		if(fread(&skillinfo->insideType, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// requirement level
		if(fread(&skillinfo->requirementLevel, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// requirement contribution (기여도)
		if(fread(&skillinfo->requirementContribution, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// effect apply type
		if(fread(&skillinfo->effectApplyType, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// angle
		if(fread(&skillinfo->angle, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		
		// requirement Excercise
		if(fread(&skillinfo->requirementExcer, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		// martial number
		if(fread(&skillinfo->martialNumber, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// attack point for 1
		if(fread(&skillinfo->attackPointFor1, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// attack point for 2
		if(fread(&skillinfo->attackPointFor2, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// attack point for 3
		if(fread(&skillinfo->attackPointFor3, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		// speed
		if(fread(&skillinfo->speed, sizeof(int), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// combination
		if(fread(&skillinfo->combination, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// distance
		if(fread(&skillinfo->distance, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// radius
		if(fread(&skillinfo->radius, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// finish attack range
		if(fread(&skillinfo->finishAttackRange, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// finish attack speed
		if(fread(&skillinfo->finishAttackSpeed, sizeof(float), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// FERA
		if(fread(&skillinfo->FERA, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// FERP
		if(fread(&skillinfo->FERP, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special type
		if(fread(&skillinfo->specialType, sizeof(char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// visual effect
		if(fread(&skillinfo->visualEffect, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// stance - man
		if(fread(&skillinfo->stance_male, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// stance - woman
		if(fread(&skillinfo->stance_female, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// motion - man
		if(fread(&skillinfo->motion_male, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// interpolate motion 1 - man
		if(fread(&skillinfo->intermotion1_male, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// interpolate motion 2 - man
		if(fread(&skillinfo->intermotion2_male, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// motion - woman
		if(fread(&skillinfo->motion_female, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// interpolate motion 1 - woman
		if(fread(&skillinfo->intermotion1_female, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// interpolate motion 2 - woman
		if(fread(&skillinfo->intermotion2_female, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		// concept
		strlength = 0;
		if(fread(&strlength, sizeof(unsigned char), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

#ifdef _XDEF_NEWSTRINGMANAGER
		skillinfo->concept = g_StringDataBase.GetTextString( _XSTRINGDB_SN_SKILL_TABLE_XLS_CONCEPT, skillid );
		skillinfo->information = g_NoneString;
		fseek( fileptr, sizeof(TCHAR) * strlength, SEEK_CUR );
#else
		if(strlength > 0)
		{
			if(strlength >= 128)
				strlength = 127;

			if(fread(skillinfo->concept, sizeof(TCHAR), strlength, fileptr) < 1)
			{
				if( bLoadFromFile ) fclose(fileptr);
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}
		}
#endif

		if(!LoadSkillDetailProperty(fileptr, skillinfo))
		{
			if( bLoadFromFile ) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		g_SkillProperty[skillid] = skillinfo;
	}

	_XSKILLINFOSTRUCTURE *skillinfo;
	skillinfo = new _XSKILLINFOSTRUCTURE;
	memset(skillinfo, 0, sizeof(_XSKILLINFOSTRUCTURE));
	g_SkillProperty[0] = skillinfo;

	if( bLoadFromFile ) fclose(fileptr);
	else g_ScriptArchive.ClosePackage();

#ifdef _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI
	if(!g_SkillMasteryManager.LoadSkillMasteryDisplayTable())
	{
		return FALSE;
	}
#else
	if(!LoadSkillMasteryDisplayProperty())
	{
		return FALSE;
	}
#endif

	if(!LoadSkillConceptProperty())
	{
		return FALSE;
	}

	if(!LoadSkillMasteryProperty())
	{
		return FALSE;
	}

	if(!LoadMasterySkillConceptProperty())
	{
		return FALSE;
	}

	if(!LoadBattleParameter())
	{
		return FALSE;
	}

	return TRUE;
}


BOOL _XSkillItem::LoadSkillDetailProperty(FILE* fileptr, _XSKILLINFOSTRUCTURE* skillinfo)
{
	for(int i = 0 ; i < 12 ; ++i)
	{
		if(fread(&skillinfo->detailInfo[i].minAttackDamage, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].maxAttackDamage, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].attackRating, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].avoidRating, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].costForce, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].costHeart, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].reqItemCategory, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].costMind, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].finishSpeed, sizeof(float), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].skillExp, sizeof(int), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].casting, sizeof(int), 1, fileptr) < 1)
			return FALSE;
		skillinfo->detailInfo[i].casting *= 100;
		if(fread(&skillinfo->detailInfo[i].delay, sizeof(int), 1, fileptr) < 1)
			return FALSE;
		skillinfo->detailInfo[i].delay *= 100;
		if(fread(&skillinfo->detailInfo[i].coolDown, sizeof(int), 1, fileptr) < 1)
			return FALSE;
		skillinfo->detailInfo[i].coolDown *= 100;
		if(fread(&skillinfo->detailInfo[i].requirementMission, sizeof(char), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].objectID, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].addBonus, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].number, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].requirementItemID, sizeof(short), 1, fileptr) < 1)
			return FALSE;
		if(fread(&skillinfo->detailInfo[i].itemConsumed, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		short id = 0, prob = 0, value = 0, count = 0;
		if(fread(&skillinfo->detailInfo[i].effectCount, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(skillinfo->detailInfo[i].effectCount > 0)
		{
			if(fread(&skillinfo->detailInfo[i].effectTime, sizeof(unsigned short), 1, fileptr) < 1)
				return FALSE;

			skillinfo->detailInfo[i].effect = new _XSI_SkillEffect[skillinfo->detailInfo[i].effectCount];

			for(int j = 0 ; j < skillinfo->detailInfo[i].effectCount ; ++j)
			{
				if(fread(&skillinfo->detailInfo[i].effect[j].id, sizeof(short), 1, fileptr) < 1)
					return FALSE;
				if(fread(&skillinfo->detailInfo[i].effect[j].prob, sizeof(short), 1, fileptr) < 1)
					return FALSE;
				if(fread(&skillinfo->detailInfo[i].effect[j].value, sizeof(short), 1, fileptr) < 1)
					return FALSE;
				
				if(skillinfo->detailInfo[i].effect[j].id > 0)
				{
					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(skillinfo->detailInfo[i].effect[j].id);
					if(iter_table != g_CharacterStateTable.end())
					{
						g_CharacterStateTable[skillinfo->detailInfo[i].effect[j].id]->prob = skillinfo->detailInfo[i].effect[j].prob;
						g_CharacterStateTable[skillinfo->detailInfo[i].effect[j].id]->value = skillinfo->detailInfo[i].effect[j].value;
					}
					else
					{
						_XDWINPRINT("[WARNING] Character State Not Found - id:%d", skillinfo->detailInfo[i].effect[j].id);
					}
				}
			}
		}

		// additive effect
		if(fread(&skillinfo->detailInfo[i].additiveCount, sizeof(short), 1, fileptr) < 1)
			return FALSE;

		if(skillinfo->detailInfo[i].additiveCount > 0)
		{
			skillinfo->detailInfo[i].additiveEffect = new _XSI_SkillAdditive[skillinfo->detailInfo[i].additiveCount];

			for(int j = 0 ; j < skillinfo->detailInfo[i].additiveCount ; ++j)
			{
				if(fread(&skillinfo->detailInfo[i].additiveEffect[j].id, sizeof(short), 1, fileptr) < 1)
					return FALSE;
				if(fread(&skillinfo->detailInfo[i].additiveEffect[j].prob, sizeof(short), 1, fileptr) < 1)
					return FALSE;
				if(fread(&skillinfo->detailInfo[i].additiveEffect[j].value, sizeof(short), 1, fileptr) < 1)
					return FALSE;
				if(fread(&skillinfo->detailInfo[i].additiveEffect[j].count, sizeof(short), 1, fileptr) < 1)
					return FALSE;

				if(skillinfo->detailInfo[i].additiveEffect[j].id > 0)
				{
					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(skillinfo->detailInfo[i].additiveEffect[j].id);
					if(iter_table != g_CharacterStateTable.end())
					{
						g_CharacterStateTable[skillinfo->detailInfo[i].additiveEffect[j].id]->prob = skillinfo->detailInfo[i].additiveEffect[j].prob;
						g_CharacterStateTable[skillinfo->detailInfo[i].additiveEffect[j].id]->value = skillinfo->detailInfo[i].additiveEffect[j].value;
						g_CharacterStateTable[skillinfo->detailInfo[i].additiveEffect[j].id]->additiveCount = skillinfo->detailInfo[i].additiveEffect[j].count;
					}
					else
					{
						_XDWINPRINT("[WARNING] Character State Not Found - id:%d", skillinfo->detailInfo[i].additiveEffect[j].id);
					}
				}
			}
		}
	}
	return TRUE;
}

void _XSkillItem::ReleaseSkillProperty(void)
{
	map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
	for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
	{
		_XSKILLINFOSTRUCTURE* skillinfo = iter_skill->second;
		if(skillinfo)
		{
			for(int i = 0 ; i < 12 ; ++i)
			{
				if(skillinfo->detailInfo[i].effect)
				{
					delete[] skillinfo->detailInfo[i].effect;
					skillinfo->detailInfo[i].effect = NULL;
				}
				if(skillinfo->detailInfo[i].additiveEffect)
				{
					delete[] skillinfo->detailInfo[i].additiveEffect;
					skillinfo->detailInfo[i].additiveEffect = NULL;
				}
			}
			delete skillinfo;
		}
	}
	g_SkillProperty.clear();

	ReleaseSkillModStructure();
}

void _XSkillItem::ReleaseSkillModStructure()
{
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod;
	for(iter_mod = g_Skill_Mod_Property.begin() ; iter_mod != g_Skill_Mod_Property.end() ; ++iter_mod)
	{
		_XDWINPRINT("Release mod table - skillid[%d]", iter_mod->first);

		_XSKILLINFO_MOD_STRUCTURE* currentinfo = iter_mod->second;
		SAFE_DELETE(currentinfo);
	}
	g_Skill_Mod_Property.clear();
}

BOOL _XSkillItem::LoadSkillPackage(void)
{
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		_XDeleteFile(_T("skillpackage.xsp"));
	}		
#endif

	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );

	FILE* fileptr = NULL;

#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		
		fileptr = fopen( _T("skillpackage.xsp"), "rb" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( _T("skillpackage.xsp") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("skillpackage.xsp") );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( _T("skillpackage.xsp"), "rb" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( _T("skillpackage.xsp") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("skillpackage.xsp") );
	}
#endif

	if( !fileptr )
	{
		_XFatalError("Can't load skill package");
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
		
	int packagecount = 0;
	fread(&packagecount, sizeof(int), 1, fileptr);
	
	if(packagecount <= 0)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return TRUE;
	}
	
	for(int i = 0 ; i < packagecount ; ++i)
	{
		fread(g_SkillPackage[i].cPackageName, sizeof(TCHAR)*32, 1, fileptr);
		fread(&g_SkillPackage[i].nSkillCount, sizeof(int), 1, fileptr);
		
		g_SkillPackage[i].skillItem = new _XSI_LEARNSKILL_ITEM[g_SkillPackage[i].nSkillCount];
		memset(g_SkillPackage[i].skillItem, 0, sizeof(_XSI_LEARNSKILL_ITEM)*g_SkillPackage[i].nSkillCount);
		
		for(int j = 0 ; j < g_SkillPackage[i].nSkillCount ; ++j)
		{
			fread(&g_SkillPackage[i].skillItem[j].nCategory, sizeof(int), 1, fileptr);
			fread(&g_SkillPackage[i].skillItem[j].sID, sizeof(short), 1, fileptr);
		}
	}
	
	if( bLoadFromFile ) fclose( fileptr );
	else g_ScriptArchive.ClosePackage();
	return TRUE;
}

void _XSkillItem::ReleaseSkillPackage(void)
{
	map <int, _XSI_LEARNSKILL_PACKAGE>::iterator iter_spackage;
	for(iter_spackage = g_SkillPackage.begin() ; iter_spackage != g_SkillPackage.end() ; ++iter_spackage )
	{
		_XSI_LEARNSKILL_PACKAGE skillpackage = iter_spackage->second;
		if(skillpackage.skillItem)
		{
			SAFE_DELETE(skillpackage.skillItem);
		}
	}
	g_SkillPackage.clear();
}

BOOL _XSkillItem::LoadSkillMasteryDisplayProperty(void)
{
	// Mastery property의 시작 index 구하는 방법
	// (문파)*(역할)*(순서)*24 - 24 = 시작 index

	FILE* fileptr = NULL;

	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

	fileptr = g_ScriptArchive.GetPackedFile(_T("Mastery_Table.XMS"));

	if(!fileptr)
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	InitializeStructure();

	int nClan = 0, nClass = 0, nMasteryClass = 0, index = 0, index_id = 0;
	TCHAR	buffer[256];
	TCHAR	command[64];
	TCHAR	name[256];

	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(TCHAR)*256);
		if(!fgets(buffer, 255, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;

		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if(strncmp(buffer, _T("_XM_CLAN"), 8) == 0)
				sscanf(buffer, "%s %d", command, &nClan);
			else if(strncmp(buffer, _T("_XM_CLASS"), 9) == 0)
				sscanf(buffer, " %s %d", command, &nClass);
			else if(strncmp(buffer, _T("_XM_MASTERYCLASS"), 16) == 0)
			{
				sscanf(buffer, "%s %d %s", command, &nMasteryClass, name);

#ifdef _XDEF_NEWSTRINGMANAGER
				TCHAR tempbuffer[5];
				TCHAR combtempbuffer[10];
				memset( tempbuffer, 0, sizeof(TCHAR) * 5 );
				memset( combtempbuffer, 0, sizeof(TCHAR) * 10 );
				
				itoa( nClan, combtempbuffer, 10 );
				itoa( nClass, tempbuffer, 10 );
				strcat( combtempbuffer, tempbuffer );
				itoa( nMasteryClass, tempbuffer, 10 );
				strcat( combtempbuffer, tempbuffer );

				int strresourceid = atoi( combtempbuffer );
				strcpy( name,  g_StringDataBase.GetTextString( _XSTRINGDB_SN_MASTERY_TABLE_XMS_NAME, strresourceid ) );
				
				_XDWINPRINT("=== Masteryname : [%d] %s ", strresourceid, name);
#endif

				index = 0;
				index_id = 0;

				if(nClan == _XGROUP_NANGIN)
				{
					int strength = strlen(name);
					g_SkillMasteryNameProperty_00_00 = new TCHAR[strength+1];
					g_SkillMasteryNameProperty_00_00[strength] = 0;
					strcpy(g_SkillMasteryNameProperty_00_00, name);
				}
				else if(nClan == _XGROUP_GAEBANG)
				{
#ifdef _XTS_MASTERYOPTIONAL
					if(nClass == 0)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_01_00 = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_01_00[strength] = 0;
						strcpy(g_SkillMasteryNameProperty_01_00, name);
					}
					else if(nClass == 1)
#else
					if(nClass == 1)
#endif
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_01_01[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_01_01[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_01_01[nMasteryClass-1], name);
					}
					else if(nClass == 2)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_01_02[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_01_02[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_01_02[nMasteryClass-1], name);
					}
					else if(nClass == 3)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_01_03[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_01_03[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_01_03[nMasteryClass-1], name);
					}
					else if(nClass == 4)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_01_04[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_01_04[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_01_04[nMasteryClass-1], name);
					}
					else if(nClass == 5)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_01_05[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_01_05[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_01_05[nMasteryClass-1], name);
					}
					else if(nClass == 6)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_01_06[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_01_06[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_01_06[nMasteryClass-1], name);
					}
					else if(nClass == 7)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_01_07[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_01_07[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_01_07[nMasteryClass-1], name);
					}
					else if(nClass == 8)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_01_08[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_01_08[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_01_08[nMasteryClass-1], name);
					}
					else if(nClass == 9)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_01_09[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_01_09[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_01_09[nMasteryClass-1], name);
					}
					else if(nClass == 10)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_01_10[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_01_10[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_01_10[nMasteryClass-1], name);
					}
					else if(nClass == 11)
					{
						int strength = strlen(name);
					g_SkillMasteryNameProperty_01_11[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_01_11[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_01_11[nMasteryClass-1], name);
					}
				}
				else if(nClan == _XGROUP_BEEGOONG)
				{
#ifdef _XTS_MASTERYOPTIONAL
					if(nClass == 0)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_02_00 = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_02_00[strength] = 0;
						strcpy(g_SkillMasteryNameProperty_02_00, name);
					}
					else if(nClass == 1)
#else
					if(nClass == 1)
#endif
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_02_01[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_02_01[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_02_01[nMasteryClass-1], name);
					}
					else if(nClass == 2)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_02_02[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_02_02[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_02_02[nMasteryClass-1], name);
					}
					else if(nClass == 3)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_02_03[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_02_03[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_02_03[nMasteryClass-1], name);
					}
					else if(nClass == 4)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_02_04[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_02_04[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_02_04[nMasteryClass-1], name);
					}
					else if(nClass == 5)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_02_05[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_02_05[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_02_05[nMasteryClass-1], name);
					}
					else if(nClass == 6)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_02_06[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_02_06[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_02_06[nMasteryClass-1], name);
					}
					else if(nClass == 7)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_02_07[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_02_07[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_02_07[nMasteryClass-1], name);
					}
					else if(nClass == 8)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_02_08[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_02_08[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_02_08[nMasteryClass-1], name);
					}
					else if(nClass == 9)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_02_09[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_02_09[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_02_09[nMasteryClass-1], name);
					}
					else if(nClass == 10)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_02_10[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_02_10[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_02_10[nMasteryClass-1], name);
					}
					else if(nClass == 11)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_02_11[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_02_11[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_02_11[nMasteryClass-1], name);
					}
				}
				else if(nClan == _XGROUP_SORIM)
				{
#ifdef _XTS_MASTERYOPTIONAL
					if(nClass == 0)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_03_00 = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_03_00[strength] = 0;
						strcpy(g_SkillMasteryNameProperty_03_00, name);
					}
					else if(nClass == 1)
#else
					if(nClass == 1)
#endif
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_03_01[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_03_01[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_03_01[nMasteryClass-1], name);
					}
					else if(nClass == 2)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_03_02[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_03_02[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_03_02[nMasteryClass-1], name);
					}
					else if(nClass == 3)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_03_03[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_03_03[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_03_03[nMasteryClass-1], name);
					}
					else if(nClass == 4)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_03_04[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_03_04[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_03_04[nMasteryClass-1], name);
					}
					else if(nClass == 5)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_03_05[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_03_05[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_03_05[nMasteryClass-1], name);
					}
					else if(nClass == 6)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_03_06[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_03_06[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_03_06[nMasteryClass-1], name);
					}
					else if(nClass == 7)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_03_07[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_03_07[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_03_07[nMasteryClass-1], name);
					}
					else if(nClass == 8)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_03_08[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_03_08[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_03_08[nMasteryClass-1], name);
					}
					else if(nClass == 9)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_03_09[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_03_09[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_03_09[nMasteryClass-1], name);
					}
					else if(nClass == 10)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_03_10[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_03_10[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_03_10[nMasteryClass-1], name);
					}
					else if(nClass == 11)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_03_11[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_03_11[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_03_11[nMasteryClass-1], name);
					}
				}
				else if(nClan == _XGROUP_NOCKRIM)
				{
#ifdef _XTS_MASTERYOPTIONAL
					if(nClass == 0)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_04_00 = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_04_00[strength] = 0;
						strcpy(g_SkillMasteryNameProperty_04_00, name);
					}
					else if(nClass == 1)
#else
					if(nClass == 1)
#endif
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_04_01[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_04_01[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_04_01[nMasteryClass-1], name);
					}
					else if(nClass == 2)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_04_02[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_04_02[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_04_02[nMasteryClass-1], name);
					}
					else if(nClass == 3)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_04_03[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_04_03[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_04_03[nMasteryClass-1], name);
					}
					else if(nClass == 4)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_04_04[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_04_04[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_04_04[nMasteryClass-1], name);
					}
					else if(nClass == 5)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_04_05[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_04_05[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_04_05[nMasteryClass-1], name);
					}
					else if(nClass == 6)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_04_06[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_04_06[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_04_06[nMasteryClass-1], name);
					}
					else if(nClass == 7)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_04_07[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_04_07[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_04_07[nMasteryClass-1], name);
					}
					else if(nClass == 8)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_04_08[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_04_08[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_04_08[nMasteryClass-1], name);
					}
					else if(nClass == 9)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_04_09[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_04_09[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_04_09[nMasteryClass-1], name);
					}
					else if(nClass == 10)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_04_10[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_04_10[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_04_10[nMasteryClass-1], name);
					}
					else if(nClass == 11)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_04_11[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_04_11[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_04_11[nMasteryClass-1], name);
					}
				}
				else if(nClan == _XGROUP_MOODANG)
				{
#ifdef _XTS_MASTERYOPTIONAL
					if(nClass == 0)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_05_00 = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_05_00[strength] = 0;
						strcpy(g_SkillMasteryNameProperty_05_00, name);
					}
					else if(nClass == 1)
#else
					if(nClass == 1)
#endif
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_05_01[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_05_01[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_05_01[nMasteryClass-1], name);
					}
					else if(nClass == 2)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_05_02[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_05_02[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_05_02[nMasteryClass-1], name);
					}
					else if(nClass == 3)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_05_03[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_05_03[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_05_03[nMasteryClass-1], name);
					}
					else if(nClass == 4)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_05_04[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_05_04[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_05_04[nMasteryClass-1], name);
					}
					else if(nClass == 5)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_05_05[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_05_05[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_05_05[nMasteryClass-1], name);
					}
					else if(nClass == 6)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_05_06[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_05_06[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_05_06[nMasteryClass-1], name);
					}
					else if(nClass == 7)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_05_07[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_05_07[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_05_07[nMasteryClass-1], name);
					}
					else if(nClass == 8)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_05_08[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_05_08[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_05_08[nMasteryClass-1], name);
					}
					else if(nClass == 9)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_05_09[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_05_09[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_05_09[nMasteryClass-1], name);
					}
					else if(nClass == 10)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_05_10[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_05_10[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_05_10[nMasteryClass-1], name);
					}
					else if(nClass == 11)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_05_11[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_05_11[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_05_11[nMasteryClass-1], name);
					}
				}
				else if(nClan == _XGROUP_MAKYO)
				{
#ifdef _XTS_MASTERYOPTIONAL
					if(nClass == 0)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_06_00 = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_06_00[strength] = 0;
						strcpy(g_SkillMasteryNameProperty_06_00, name);
					}
					else if(nClass == 1)
#else
					if(nClass == 1)
#endif
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_06_01[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_06_01[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_06_01[nMasteryClass-1], name);
					}
					else if(nClass == 2)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_06_02[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_06_02[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_06_02[nMasteryClass-1], name);
					}
					else if(nClass == 3)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_06_03[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_06_03[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_06_03[nMasteryClass-1], name);
					}
					else if(nClass == 4)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_06_04[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_06_04[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_06_04[nMasteryClass-1], name);
					}
					else if(nClass == 5)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_06_05[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_06_05[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_06_05[nMasteryClass-1], name);
					}
					else if(nClass == 6)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_06_06[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_06_06[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_06_06[nMasteryClass-1], name);
					}
					else if(nClass == 7)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_06_07[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_06_07[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_06_07[nMasteryClass-1], name);
					}
					else if(nClass == 8)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_06_08[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_06_08[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_06_08[nMasteryClass-1], name);
					}
					else if(nClass == 9)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_06_09[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_06_09[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_06_09[nMasteryClass-1], name);
					}
					else if(nClass == 10)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_06_10[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_06_10[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_06_10[nMasteryClass-1], name);
					}
					else if(nClass == 11)
					{
						int strength = strlen(name);
						g_SkillMasteryNameProperty_06_11[nMasteryClass-1] = new TCHAR[strength+1];
						g_SkillMasteryNameProperty_06_11[nMasteryClass-1][strength] = 0;
						strcpy(g_SkillMasteryNameProperty_06_11[nMasteryClass-1], name);
					}
				}
			}
			else
			{
				if(nClan == _XGROUP_NANGIN)
				{
					sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_00_00[index].skillid,
						&g_SkillMasteryDisplayProperty_00_00[index].needprev);
					index++;
				}
				else if(nClan == _XGROUP_GAEBANG)
				{
#ifdef _XTS_MASTERYOPTIONAL
					if(nClass == 0)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_01_00[index].skillid, 
							&g_SkillMasteryDisplayProperty_01_00[index].needprev);
						index++;
					}
					else if(nClass == 1)
#else
					if(nClass == 1)
#endif
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_01_01[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_01_01[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 2)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_01_02[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_01_02[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 3)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_01_03[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_01_03[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 4)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_01_04[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_01_04[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 5)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_01_05[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_01_05[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 6)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_01_06[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_01_06[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 7)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_01_07[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_01_07[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 8)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_01_08[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_01_08[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 9)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_01_09[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_01_09[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 10)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_01_10[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_01_10[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 11)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_01_11[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_01_11[nMasteryClass-1][index].needprev);
						index++;
					}
				}
				else if(nClan == _XGROUP_BEEGOONG)
				{
#ifdef _XTS_MASTERYOPTIONAL
					if(nClass == 0)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_02_00[index].skillid, 
							&g_SkillMasteryDisplayProperty_02_00[index].needprev);
						index++;
					}
					else if(nClass == 1)
#else
					if(nClass == 1)
#endif
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_02_01[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_02_01[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 2)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_02_02[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_02_02[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 3)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_02_03[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_02_03[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 4)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_02_04[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_02_04[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 5)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_02_05[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_02_05[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 6)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_02_06[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_02_06[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 7)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_02_07[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_02_07[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 8)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_02_08[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_02_08[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 9)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_02_09[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_02_09[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 10)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_02_10[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_02_10[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 11)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_02_11[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_02_11[nMasteryClass-1][index].needprev);
						index++;
					}
				}
				else if(nClan == _XGROUP_SORIM)
				{
#ifdef _XTS_MASTERYOPTIONAL
					if(nClass == 0)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_03_00[index].skillid, 
							&g_SkillMasteryDisplayProperty_03_00[index].needprev);
						index++;
					}
					else if(nClass == 1)
#else
					if(nClass == 1)
#endif
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_03_01[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_03_01[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 2)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_03_02[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_03_02[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 3)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_03_03[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_03_03[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 4)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_03_04[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_03_04[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 5)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_03_05[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_03_05[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 6)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_03_06[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_03_06[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 7)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_03_07[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_03_07[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 8)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_03_08[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_03_08[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 9)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_03_09[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_03_09[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 10)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_03_10[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_03_10[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 11)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_03_11[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_03_11[nMasteryClass-1][index].needprev);
						index++;
					}
				}
				else if(nClan == _XGROUP_NOCKRIM)
				{
#ifdef _XTS_MASTERYOPTIONAL
					if(nClass == 0)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_04_00[index].skillid, 
							&g_SkillMasteryDisplayProperty_04_00[index].needprev);
						index++;
					}
					else if(nClass == 1)
#else
					if(nClass == 1)
#endif
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_04_01[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_04_01[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 2)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_04_02[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_04_02[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 3)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_04_03[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_04_03[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 4)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_04_04[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_04_04[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 5)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_04_05[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_04_05[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 6)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_04_06[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_04_06[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 7)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_04_07[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_04_07[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 8)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_04_08[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_04_08[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 9)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_04_09[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_04_09[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 10)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_04_10[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_04_10[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 11)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_04_11[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_04_11[nMasteryClass-1][index].needprev);
						index++;
					}
				}
				else if(nClan == _XGROUP_MOODANG)
				{
#ifdef _XTS_MASTERYOPTIONAL
					if(nClass == 0)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_05_00[index].skillid, 
							&g_SkillMasteryDisplayProperty_05_00[index].needprev);
						index++;
					}
					else if(nClass == 1)
#else
					if(nClass == 1)
#endif
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_05_01[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_05_01[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 2)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_05_02[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_05_02[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 3)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_05_03[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_05_03[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 4)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_05_04[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_05_04[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 5)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_05_05[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_05_05[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 6)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_05_06[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_05_06[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 7)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_05_07[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_05_07[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 8)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_05_08[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_05_08[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 9)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_05_09[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_05_09[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 10)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_05_10[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_05_10[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 11)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_05_11[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_05_11[nMasteryClass-1][index].needprev);
						index++;
					}
				}
				else if(nClan == _XGROUP_MAKYO)
				{
#ifdef _XTS_MASTERYOPTIONAL
					if(nClass == 0)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_06_00[index].skillid, 
							&g_SkillMasteryDisplayProperty_06_00[index].needprev);
						index++;
					}
					else if(nClass == 1)
#else
					if(nClass == 1)
#endif
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_06_01[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_06_01[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 2)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_06_02[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_06_02[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 3)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_06_03[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_06_03[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 4)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_06_04[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_06_04[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 5)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_06_05[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_06_05[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 6)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_06_06[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_06_06[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 7)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_06_07[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_06_07[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 8)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_06_08[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_06_08[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 9)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_06_09[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_06_09[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 10)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_06_10[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_06_10[nMasteryClass-1][index].needprev);
						index++;
					}
					else if(nClass == 11)
					{
						sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty_06_11[nMasteryClass-1][index].skillid, 
							&g_SkillMasteryDisplayProperty_06_11[nMasteryClass-1][index].needprev);
						index++;
					}
				}

//				sscanf(buffer, "%d %d", &g_SkillMasteryDisplayProperty[index].skillid, &g_SkillMasteryDisplayProperty[index].needprev);
//				index++;
			}	
		}
	} while(!feof(fileptr));

	g_ScriptArchive.ClosePackage();
	return TRUE;
}

BOOL _XSkillItem::FindSkillProperty(short skillid)
{
	map <short, _XSKILLINFOSTRUCTURE*>::iterator iter = g_SkillProperty.find(skillid);
	if(iter != g_SkillProperty.end())
		return TRUE;
	else
		return FALSE;
}

BOOL _XSkillItem::FindMasterySkillProperty(short skillid)
{
	map <short, _XSI_MasterySkillStruct*>::iterator iter = g_SkillMasteryProperty.find(skillid);
	if(iter != g_SkillMasteryProperty.end())
		return TRUE;
	else
		return FALSE;
}

void _XSkillItem::InitializeStructure(void)
{
	memset(g_SkillMasteryDisplayProperty_01_01, 0, sizeof(g_SkillMasteryDisplayProperty_01_01));
	memset(g_SkillMasteryDisplayProperty_01_02, 0, sizeof(g_SkillMasteryDisplayProperty_01_02));
	memset(g_SkillMasteryDisplayProperty_01_03, 0, sizeof(g_SkillMasteryDisplayProperty_01_03));
	memset(g_SkillMasteryDisplayProperty_01_04, 0, sizeof(g_SkillMasteryDisplayProperty_01_04));
	memset(g_SkillMasteryDisplayProperty_01_05, 0, sizeof(g_SkillMasteryDisplayProperty_01_05));
	memset(g_SkillMasteryDisplayProperty_01_06, 0, sizeof(g_SkillMasteryDisplayProperty_01_06));
	memset(g_SkillMasteryDisplayProperty_01_07, 0, sizeof(g_SkillMasteryDisplayProperty_01_07));
	memset(g_SkillMasteryDisplayProperty_01_08, 0, sizeof(g_SkillMasteryDisplayProperty_01_08));
	memset(g_SkillMasteryDisplayProperty_01_09, 0, sizeof(g_SkillMasteryDisplayProperty_01_09));
	memset(g_SkillMasteryDisplayProperty_01_10, 0, sizeof(g_SkillMasteryDisplayProperty_01_10));
	memset(g_SkillMasteryDisplayProperty_01_11, 0, sizeof(g_SkillMasteryDisplayProperty_01_11));

	memset(g_SkillMasteryDisplayProperty_02_01, 0, sizeof(g_SkillMasteryDisplayProperty_02_01));
	memset(g_SkillMasteryDisplayProperty_02_02, 0, sizeof(g_SkillMasteryDisplayProperty_02_02));
	memset(g_SkillMasteryDisplayProperty_02_03, 0, sizeof(g_SkillMasteryDisplayProperty_02_03));
	memset(g_SkillMasteryDisplayProperty_02_04, 0, sizeof(g_SkillMasteryDisplayProperty_02_04));
	memset(g_SkillMasteryDisplayProperty_02_05, 0, sizeof(g_SkillMasteryDisplayProperty_02_05));
	memset(g_SkillMasteryDisplayProperty_02_06, 0, sizeof(g_SkillMasteryDisplayProperty_02_06));
	memset(g_SkillMasteryDisplayProperty_02_07, 0, sizeof(g_SkillMasteryDisplayProperty_02_07));
	memset(g_SkillMasteryDisplayProperty_02_08, 0, sizeof(g_SkillMasteryDisplayProperty_02_08));
	memset(g_SkillMasteryDisplayProperty_02_09, 0, sizeof(g_SkillMasteryDisplayProperty_02_09));
	memset(g_SkillMasteryDisplayProperty_02_10, 0, sizeof(g_SkillMasteryDisplayProperty_02_10));
	memset(g_SkillMasteryDisplayProperty_02_11, 0, sizeof(g_SkillMasteryDisplayProperty_02_11));

	memset(g_SkillMasteryDisplayProperty_03_01, 0, sizeof(g_SkillMasteryDisplayProperty_03_01));
	memset(g_SkillMasteryDisplayProperty_03_02, 0, sizeof(g_SkillMasteryDisplayProperty_03_02));
	memset(g_SkillMasteryDisplayProperty_03_03, 0, sizeof(g_SkillMasteryDisplayProperty_03_03));
	memset(g_SkillMasteryDisplayProperty_03_04, 0, sizeof(g_SkillMasteryDisplayProperty_03_04));
	memset(g_SkillMasteryDisplayProperty_03_05, 0, sizeof(g_SkillMasteryDisplayProperty_03_05));
	memset(g_SkillMasteryDisplayProperty_03_06, 0, sizeof(g_SkillMasteryDisplayProperty_03_06));
	memset(g_SkillMasteryDisplayProperty_03_07, 0, sizeof(g_SkillMasteryDisplayProperty_03_07));
	memset(g_SkillMasteryDisplayProperty_03_08, 0, sizeof(g_SkillMasteryDisplayProperty_03_08));
	memset(g_SkillMasteryDisplayProperty_03_09, 0, sizeof(g_SkillMasteryDisplayProperty_03_09));
	memset(g_SkillMasteryDisplayProperty_03_10, 0, sizeof(g_SkillMasteryDisplayProperty_03_10));
	memset(g_SkillMasteryDisplayProperty_03_11, 0, sizeof(g_SkillMasteryDisplayProperty_03_11));

	memset(g_SkillMasteryDisplayProperty_04_01, 0, sizeof(g_SkillMasteryDisplayProperty_04_01));
	memset(g_SkillMasteryDisplayProperty_04_02, 0, sizeof(g_SkillMasteryDisplayProperty_04_02));
	memset(g_SkillMasteryDisplayProperty_04_03, 0, sizeof(g_SkillMasteryDisplayProperty_04_03));
	memset(g_SkillMasteryDisplayProperty_04_04, 0, sizeof(g_SkillMasteryDisplayProperty_04_04));
	memset(g_SkillMasteryDisplayProperty_04_05, 0, sizeof(g_SkillMasteryDisplayProperty_04_05));
	memset(g_SkillMasteryDisplayProperty_04_06, 0, sizeof(g_SkillMasteryDisplayProperty_04_06));
	memset(g_SkillMasteryDisplayProperty_04_07, 0, sizeof(g_SkillMasteryDisplayProperty_04_07));
	memset(g_SkillMasteryDisplayProperty_04_08, 0, sizeof(g_SkillMasteryDisplayProperty_04_08));
	memset(g_SkillMasteryDisplayProperty_04_09, 0, sizeof(g_SkillMasteryDisplayProperty_04_09));
	memset(g_SkillMasteryDisplayProperty_04_10, 0, sizeof(g_SkillMasteryDisplayProperty_04_10));
	memset(g_SkillMasteryDisplayProperty_04_11, 0, sizeof(g_SkillMasteryDisplayProperty_04_11));

	memset(g_SkillMasteryDisplayProperty_05_01, 0, sizeof(g_SkillMasteryDisplayProperty_04_01));
	memset(g_SkillMasteryDisplayProperty_05_02, 0, sizeof(g_SkillMasteryDisplayProperty_04_02));
	memset(g_SkillMasteryDisplayProperty_05_03, 0, sizeof(g_SkillMasteryDisplayProperty_04_03));
	memset(g_SkillMasteryDisplayProperty_05_04, 0, sizeof(g_SkillMasteryDisplayProperty_04_04));
	memset(g_SkillMasteryDisplayProperty_05_05, 0, sizeof(g_SkillMasteryDisplayProperty_04_05));
	memset(g_SkillMasteryDisplayProperty_05_06, 0, sizeof(g_SkillMasteryDisplayProperty_04_06));
	memset(g_SkillMasteryDisplayProperty_05_07, 0, sizeof(g_SkillMasteryDisplayProperty_04_07));
	memset(g_SkillMasteryDisplayProperty_05_08, 0, sizeof(g_SkillMasteryDisplayProperty_04_08));
	memset(g_SkillMasteryDisplayProperty_05_09, 0, sizeof(g_SkillMasteryDisplayProperty_04_09));
	memset(g_SkillMasteryDisplayProperty_05_10, 0, sizeof(g_SkillMasteryDisplayProperty_04_10));
	memset(g_SkillMasteryDisplayProperty_05_11, 0, sizeof(g_SkillMasteryDisplayProperty_04_11));

	memset(g_SkillMasteryDisplayProperty_06_01, 0, sizeof(g_SkillMasteryDisplayProperty_04_01));
	memset(g_SkillMasteryDisplayProperty_06_02, 0, sizeof(g_SkillMasteryDisplayProperty_04_02));
	memset(g_SkillMasteryDisplayProperty_06_03, 0, sizeof(g_SkillMasteryDisplayProperty_04_03));
	memset(g_SkillMasteryDisplayProperty_06_04, 0, sizeof(g_SkillMasteryDisplayProperty_04_04));
	memset(g_SkillMasteryDisplayProperty_06_05, 0, sizeof(g_SkillMasteryDisplayProperty_04_05));
	memset(g_SkillMasteryDisplayProperty_06_06, 0, sizeof(g_SkillMasteryDisplayProperty_04_06));
	memset(g_SkillMasteryDisplayProperty_06_07, 0, sizeof(g_SkillMasteryDisplayProperty_04_07));
	memset(g_SkillMasteryDisplayProperty_06_08, 0, sizeof(g_SkillMasteryDisplayProperty_04_08));
	memset(g_SkillMasteryDisplayProperty_06_09, 0, sizeof(g_SkillMasteryDisplayProperty_04_09));
	memset(g_SkillMasteryDisplayProperty_06_10, 0, sizeof(g_SkillMasteryDisplayProperty_04_10));
	memset(g_SkillMasteryDisplayProperty_06_11, 0, sizeof(g_SkillMasteryDisplayProperty_04_11));

	for(int i = 0 ; i < 3 ; ++i)
	{
		g_SkillMasteryNameProperty_01_01[i] = NULL;
		g_SkillMasteryNameProperty_01_02[i] = NULL;
		g_SkillMasteryNameProperty_01_03[i] = NULL;
		g_SkillMasteryNameProperty_01_04[i] = NULL;
		g_SkillMasteryNameProperty_01_05[i] = NULL;
		g_SkillMasteryNameProperty_01_06[i] = NULL;
		g_SkillMasteryNameProperty_01_07[i] = NULL;
		g_SkillMasteryNameProperty_01_08[i] = NULL;
		g_SkillMasteryNameProperty_01_09[i] = NULL;
		g_SkillMasteryNameProperty_01_10[i] = NULL;
		g_SkillMasteryNameProperty_01_11[i] = NULL;

		g_SkillMasteryNameProperty_02_01[i] = NULL;
		g_SkillMasteryNameProperty_02_02[i] = NULL;
		g_SkillMasteryNameProperty_02_03[i] = NULL;
		g_SkillMasteryNameProperty_02_04[i] = NULL;
		g_SkillMasteryNameProperty_02_05[i] = NULL;
		g_SkillMasteryNameProperty_02_06[i] = NULL;
		g_SkillMasteryNameProperty_02_07[i] = NULL;
		g_SkillMasteryNameProperty_02_08[i] = NULL;
		g_SkillMasteryNameProperty_02_09[i] = NULL;
		g_SkillMasteryNameProperty_02_10[i] = NULL;
		g_SkillMasteryNameProperty_02_11[i] = NULL;

		g_SkillMasteryNameProperty_03_01[i] = NULL;
		g_SkillMasteryNameProperty_03_02[i] = NULL;
		g_SkillMasteryNameProperty_03_03[i] = NULL;
		g_SkillMasteryNameProperty_03_04[i] = NULL;
		g_SkillMasteryNameProperty_03_05[i] = NULL;
		g_SkillMasteryNameProperty_03_06[i] = NULL;
		g_SkillMasteryNameProperty_03_07[i] = NULL;
		g_SkillMasteryNameProperty_03_08[i] = NULL;
		g_SkillMasteryNameProperty_03_09[i] = NULL;
		g_SkillMasteryNameProperty_03_10[i] = NULL;
		g_SkillMasteryNameProperty_03_11[i] = NULL;

		g_SkillMasteryNameProperty_04_01[i] = NULL;
		g_SkillMasteryNameProperty_04_02[i] = NULL;
		g_SkillMasteryNameProperty_04_03[i] = NULL;
		g_SkillMasteryNameProperty_04_04[i] = NULL;
		g_SkillMasteryNameProperty_04_05[i] = NULL;
		g_SkillMasteryNameProperty_04_06[i] = NULL;
		g_SkillMasteryNameProperty_04_07[i] = NULL;
		g_SkillMasteryNameProperty_04_08[i] = NULL;
		g_SkillMasteryNameProperty_04_09[i] = NULL;
		g_SkillMasteryNameProperty_04_10[i] = NULL;
		g_SkillMasteryNameProperty_04_11[i] = NULL;

		g_SkillMasteryNameProperty_05_01[i] = NULL;
		g_SkillMasteryNameProperty_05_02[i] = NULL;
		g_SkillMasteryNameProperty_05_03[i] = NULL;
		g_SkillMasteryNameProperty_05_04[i] = NULL;
		g_SkillMasteryNameProperty_05_05[i] = NULL;
		g_SkillMasteryNameProperty_05_06[i] = NULL;
		g_SkillMasteryNameProperty_05_07[i] = NULL;
		g_SkillMasteryNameProperty_05_08[i] = NULL;
		g_SkillMasteryNameProperty_05_09[i] = NULL;
		g_SkillMasteryNameProperty_05_10[i] = NULL;
		g_SkillMasteryNameProperty_05_11[i] = NULL;

		g_SkillMasteryNameProperty_06_01[i] = NULL;
		g_SkillMasteryNameProperty_06_02[i] = NULL;
		g_SkillMasteryNameProperty_06_03[i] = NULL;
		g_SkillMasteryNameProperty_06_04[i] = NULL;
		g_SkillMasteryNameProperty_06_05[i] = NULL;
		g_SkillMasteryNameProperty_06_06[i] = NULL;
		g_SkillMasteryNameProperty_06_07[i] = NULL;
		g_SkillMasteryNameProperty_06_08[i] = NULL;
		g_SkillMasteryNameProperty_06_09[i] = NULL;
		g_SkillMasteryNameProperty_06_10[i] = NULL;
		g_SkillMasteryNameProperty_06_11[i] = NULL;
	}
}

BOOL _XSkillItem::LoadSkillConceptProperty(void)
{
	FILE* fileptr = NULL;
	
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

	fileptr = g_ScriptArchive.GetPackedFile(_T("SkillConcept.XMS"));
	
	if(!fileptr)
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	TCHAR buffer[512];
	
	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(TCHAR)*256);
		
		if(!fgets(buffer, 511, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
	
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			switch(buffer[4])
			{
			case 'C' :
				{
					if(strncmp(buffer, "_XM_CLAN", 8) == 0)
					{
						if(!ParseClanConcept(fileptr))
							break;
					}
					else if(strncmp(buffer, "_XM_CLASS", 9) == 0)
					{
						if(!ParseClassConcept(fileptr))
							break;
					}
				}
				break;
			case 'S' :
				{
					if(strncmp(buffer, "_XM_STYPE", 9) == 0)
					{
						if(!ParseSTypeConcept(fileptr))
							break;
					}
					else if(strncmp(buffer, "_XM_SKILLCONCEPT", 16) == 0)
					{
						if(!ParseSkillConcept(fileptr))
							break;
					}
				}
				break;
			}
		}
	} while(!feof(fileptr));

	g_ScriptArchive.ClosePackage();
	return TRUE;

/*
	int clan = 0, stype = 0, cClass = 0, count = 0, readcount = 0;
	bool bClan = false, bStype = false, bClassStype = false;
	TCHAR	buffer[256];
	TCHAR	command[64];

	do 
	{
		if(feof(fileptr))
			break;

		memset(command, 0, sizeof(TCHAR)*64);
		memset(buffer, 0, sizeof(TCHAR)*256);

		if(!fgets(buffer, 255, fileptr))
			break;
		
		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
		
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if(bClan)
			{
				// 문파 설명
				sscanf(buffer, "%s", g_SkillConceptProperty[clan].clan_concept);
				ReplaceCharacter(g_SkillConceptProperty[clan].clan_concept, _T('_'), _T(' '));
				bClan = false;
			}
			if(bStype)
			{
				// stype 설명
				sscanf(buffer, "%s", g_SkillConceptProperty[clan].stype_concept[stype]);
				ReplaceCharacter(g_SkillConceptProperty[clan].stype_concept[stype], _T('_'), _T(' '));
				bStype = false;
			}
			if(bClassStype)
			{
				// stype 번호
				sscanf(buffer, "%d", &g_SkillConceptProperty[clan].class_stype[cClass][readcount]);
				readcount++;

				if(readcount >= count)
				{
					bClassStype = false;
					cClass = count = readcount = 0;
				}
			}

			if(strncmp(buffer, _T("_XM_CLAN"), 8) == 0)
			{
				sscanf(buffer, "%s %d", command, &clan);
				bClan = true;
				stype = cClass = count = readcount = 0;
			}

			if(strncmp(buffer, _T("_XM_STYPE"), 9) == 0)
				sscanf(buffer, "%s %d", command, &stype);

			if(strncmp(buffer, _T("_XM_NAME"), 8) == 0)
			{
				sscanf(buffer, "%s %s", command, g_SkillConceptProperty[clan].stype_name[stype]);
				bStype = true;
			}

			if(strncmp(buffer, _T("_XM_CLASS"), 9) == 0)
				sscanf(buffer, "%s %d", command, &cClass);

			if(strncmp(buffer, _T("_XM_COUNT"), 9) == 0)
			{
				sscanf(buffer, "%s %d", command, &count);
				bClassStype = true;
			}
		}
	} while(!feof(fileptr));
	
	scriptpackage.Close();
	return TRUE;*/
}

BOOL _XSkillItem::LoadMasterySkillConceptProperty(void)
{
	FILE* fileptr = NULL;
	
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

	fileptr = g_ScriptArchive.GetPackedFile(_T("MasterySkillConcept.XMS"));
	
	if(!fileptr)
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	TCHAR buffer[512];
	
	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(TCHAR)*256);
		
		if(!fgets(buffer, 511, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
	
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if(strncmp(buffer, "_XM_MASTERYSKILLCONCEPT", 16) == 0)
			{
				if(!ParseMasterySkillConcept(fileptr))
					break;
			}
		}
	} while(!feof(fileptr));

	g_ScriptArchive.ClosePackage();
	return TRUE;
}

BOOL _XSkillItem::ParseClanConcept(FILE* fileptr)
{
	char cmd[512];
	char arg[512];
	int pos = 0;
	int clanid = 0;
	bool end = false;
	
	// 0 : 시작 하지 않음
	// 1 : clan id
	// 2 : clan concept
	while(!end && !feof(fileptr))
	{
		memset(cmd, 0, sizeof(TCHAR)*512);
		memset(arg, 0, sizeof(TCHAR)*512);

		if(!fgets(cmd, 511, fileptr))
			break;

		switch(cmd[0])
		{
		case '{' :
			{
				if(pos == 0)
					pos = 1;
				clanid = 0;
			}
			break;
		case '}' :
			{
				if(pos == 3)
					pos = 4;
				end = true;
			}
			break;
		default :
			{
				if(pos == 1)
				{
					sscanf(cmd, "%s", arg);
					clanid = atoi(arg);
					if(clanid >= 0)
						pos = 2;
					else
						end = true;
				}
				else if(pos == 2)
				{
#ifdef _XDEF_NEWSTRINGMANAGER
					g_SkillConceptProperty[clanid].clan_concept = 
							g_StringDataBase.GetTextString( _XSTRINGDB_SN_SKILLCONCEPT_XMS_CLAN, clanid+1 );
#else
					sscanf(cmd, "%s", g_SkillConceptProperty[clanid].clan_concept);
					ReplaceCharacter(g_SkillConceptProperty[clanid].clan_concept, _T('_'), _T(' '));
#endif
					pos = 3;
				}
				else
					end = true;
			}
			break;
		}
		if(end)
			break;
	}

	if(pos != 4)
		return FALSE;

	return end;
}

BOOL _XSkillItem::ParseSTypeConcept(FILE* fileptr)
{
	char cmd[512];
	char arg[512];
	int pos = 0;
	int clanid = 0;
	int stypeid = 0;
	bool end = false;
	
	// 0 : 시작 하지 않음
	// 1 : clan id
	// 2 : stype id
	// 3 : stype name
	// 4 : stype concept
	while(!end && !feof(fileptr))
	{
		memset(cmd, 0, sizeof(TCHAR)*512);
		memset(arg, 0, sizeof(TCHAR)*512);
		
		if(!fgets(cmd, 511, fileptr))
			break;
		
		switch(cmd[0])
		{
		case '{' :
			{
				if(pos == 0)
					pos = 1;
				clanid = 0;
				stypeid = 0;
			}
			break;
		case '}' :
			{
				if(pos == 5)
					pos = 6;
				end = true;
			}
			break;
		default :
			{
				if(pos == 1)
				{
					sscanf(cmd, "%s", arg);
					clanid = atoi(arg);
					if(clanid >= 0)
						pos = 2;
					else
						end = true;
				}
				else if(pos == 2)
				{
					sscanf(cmd, "%s", arg);
					stypeid = atoi(arg);
					if(stypeid >= 0)
						pos = 3;
					else 
						end = true;
				}
				else if(pos == 3)
				{
#ifdef _XDEF_NEWSTRINGMANAGER
					int stringid = 0;

					TCHAR highidstringbuffer[10];
					TCHAR lowidstringbuffer[10];
					
					memset( highidstringbuffer, 0, sizeof(TCHAR) * 10 );
					memset( lowidstringbuffer, 0, sizeof(TCHAR) * 10 );

					itoa( clanid, highidstringbuffer, 10 );

					if(stypeid < 10)
					{
						itoa( stypeid, lowidstringbuffer, 10 );
						lowidstringbuffer[1] = lowidstringbuffer[0];
						lowidstringbuffer[0] = _T('0');
					}
					else
					{
						itoa( stypeid, lowidstringbuffer, 10 );
					}

					strcat( highidstringbuffer, lowidstringbuffer );

					stringid = atoi( highidstringbuffer );

					g_SkillConceptProperty[clanid].stype_name[stypeid] = 
						g_StringDataBase.GetTextString( _XSTRINGDB_SN_SKILLCONCEPT_XMS_STYPETITLE, stringid );
#else
					sscanf(cmd, "%s", g_SkillConceptProperty[clanid].stype_name[stypeid]);
#endif
					pos = 4;
				}
				else if(pos == 4)
				{
#ifdef _XDEF_NEWSTRINGMANAGER
					int stringid = 0;
					
					TCHAR highidstringbuffer[10];
					TCHAR lowidstringbuffer[10];
					
					memset( highidstringbuffer, 0, sizeof(TCHAR) * 10 );
					memset( lowidstringbuffer, 0, sizeof(TCHAR) * 10 );
					
					itoa( clanid, highidstringbuffer, 10 );
					
					if(stypeid < 10)
					{
						itoa( stypeid, lowidstringbuffer, 10 );
						lowidstringbuffer[1] = lowidstringbuffer[0];
						lowidstringbuffer[0] = _T('0');
					}
					else
					{
						itoa( stypeid, lowidstringbuffer, 10 );
					}
					
					strcat( highidstringbuffer, lowidstringbuffer );
					
					stringid = atoi( highidstringbuffer );

					g_SkillConceptProperty[clanid].stype_concept[stypeid] = 
						g_StringDataBase.GetTextString( _XSTRINGDB_SN_SKILLCONCEPT_XMS_STYPECONCEPT, stringid );
#else
					sscanf(cmd, "%s", g_SkillConceptProperty[clanid].stype_concept[stypeid]);
					ReplaceCharacter(g_SkillConceptProperty[clanid].stype_concept[stypeid], _T('_'), _T(' '));
#endif
					pos = 5;
				}
				else
					end = true;
			}
			break;
		}
		if(end)
			break;
	}
	
	if(pos != 6)
		return FALSE;
	
	return end;
}

BOOL _XSkillItem::ParseClassConcept(FILE* fileptr)
{
	char cmd[512];
	char arg[512];
	int pos = 0;
	int clanid = 0;
	int classid = 0;
	int stypenumber = 0;
	int readcount = 0;
	bool end = false;
	
	// 0 : 시작 하지 않음
	// 1 : clan id
	// 2 : stype id
	while(!end && !feof(fileptr))
	{
		memset(cmd, 0, sizeof(TCHAR)*512);
		memset(arg, 0, sizeof(TCHAR)*512);
		
		if(!fgets(cmd, 511, fileptr))
			break;
		
		switch(cmd[0])
		{
		case '{' :
			{
				if(pos == 0)
					pos = 1;
				clanid = 0;
				classid = 0;
				stypenumber = 0;
				readcount = 0;
			}
			break;
		case '}' :
			{
				end = true;
			}
			break;
		default :
			{
				if(pos == 1)
				{
					sscanf(cmd, "%s", arg);
					clanid = atoi(arg);
					if(clanid >= 0)
						pos = 2;
					else
						end = true;
				}
				else if(pos == 2)
				{
					sscanf(cmd, "%s", arg);
					classid = atoi(arg);
					if(classid >= 0)
						pos = 3;
					else
						end = true;
				}
				else
				{
					sscanf(cmd, "%s", arg);
					stypenumber = atoi(arg);
					g_SkillConceptProperty[clanid].class_stype[classid][readcount] = (short)stypenumber;
					readcount++;
				}
			}
			break;
		}
		if(end)
			break;
	}
	
	if(pos < 3)
		return FALSE;
	
	return end;
}

BOOL _XSkillItem::ParseSkillConcept(FILE* fileptr)
{
	char cmd[512];
	char arg[512];
	int pos = 0;
	short skillid = 0;
	char temp = 0;
	int readcount = 0;
	bool end = false;
	
	// 0 : 시작 하지 않음
	// 1 : skill id
	// 2 : skill concept
	while(!end && !feof(fileptr))
	{
		memset(cmd, 0, sizeof(TCHAR)*512);
		memset(arg, 0, sizeof(TCHAR)*512);
		
		if(!fgets(cmd, 511, fileptr))
			break;
		
		switch(cmd[0])
		{
		case '{' :
			{
				if(pos == 0)
					pos = 1;
				skillid = 0;
				temp = 0;
				readcount = 0;
			}
			break;
		case '}' :
			{
				end = true;
			}
			break;
		default :
			{
				if(pos == 1)
				{
					sscanf(cmd, "%s", arg);
					skillid = (short)atoi(arg);
					if(skillid >= 0)
						pos = 2;
					else
						end = true;

					//_XLog( "%d", skillid );
				}
				else if(pos == 2)
				{
					map <short, _XSKILLINFOSTRUCTURE*>::iterator iter = g_SkillProperty.find(skillid);
					if(iter != g_SkillProperty.end())
					{
#ifdef _XDEF_NEWSTRINGMANAGER
						_XTEXTSTRING_INFO* pInfo = g_StringDataBase.GetTextInfo( _XSTRINGDB_SN_SKILLCONCEPT_XMS_CONCEPT, skillid );
						if( pInfo )
						{
							g_SkillProperty[skillid]->information = pInfo->strTextString;
							g_SkillProperty[skillid]->wordorder = pInfo->iParameterOrder;
						}
#else
						sscanf(cmd, "%s", g_SkillProperty[skillid]->information);
						ReplaceCharacter(g_SkillProperty[skillid]->information, _T('_'), _T(' '));
#endif

						pos = 3;
					}
					else
						end = true;
				}
				else
				{
					if(readcount < 10)
					{
						sscanf(cmd, "%s", arg);
						temp = GetParameter(arg);
						g_SkillProperty[skillid]->parameter[readcount] = temp;
						readcount++;
					}
				}
			}
			break;
		}
		if(end)
			break;
	}
	
	if(pos < 3)
		return FALSE;
	
	return end;
}

BOOL _XSkillItem::ParseMasterySkillConcept(FILE* fileptr)
{
	char cmd[512];
	char arg[512];
	int pos = 0;
	short skillid = 0;
	char temp = 0;
	int readcount = 0;
	bool end = false;
	
	// 0 : 시작 하지 않음
	// 1 : skill id
	// 2 : skill concept
	while(!end && !feof(fileptr))
	{
		memset(cmd, 0, sizeof(TCHAR)*512);
		memset(arg, 0, sizeof(TCHAR)*512);
		
		if(!fgets(cmd, 511, fileptr))
			break;
		
		switch(cmd[0])
		{
		case '{' :
			{
				if(pos == 0)
					pos = 1;
				skillid = 0;
				temp = 0;
				readcount = 0;
			}
			break;
		case '}' :
			{
				end = true;
			}
			break;
		default :
			{
				if(pos == 1)
				{
					sscanf(cmd, "%s", arg);
					skillid = (short)atoi(arg);
					if(skillid >= 0)
						pos = 2;
					else
						end = true;
				}
				else if(pos == 2)
				{
					map <short, _XSI_MasterySkillStruct*>::iterator iter = g_SkillMasteryProperty.find(skillid);
					if(iter != g_SkillMasteryProperty.end())
					{
#ifdef _XDEF_NEWSTRINGMANAGER		
						_XTEXTSTRING_INFO* pInfo = g_StringDataBase.GetTextInfo( _XSTRINGDB_SN_MASTERYSKILLCONCEPT_XMS, skillid );
						if( pInfo )
						{
							g_SkillMasteryProperty[skillid]->information = pInfo->strTextString;
							g_SkillMasteryProperty[skillid]->wordorder   = pInfo->iParameterOrder;
						}
#else
						sscanf(cmd, "%s", g_SkillMasteryProperty[skillid]->information);
						ReplaceCharacter(g_SkillMasteryProperty[skillid]->information, _T('_'), _T(' '));
#endif
						pos = 3;
					}					
					else
						end = true; 
				}
				else
				{
					if(readcount < 14)
					{
						sscanf(cmd, "%s", arg);
						temp = GetParameter(arg);
						g_SkillMasteryProperty[skillid]->parameter[readcount] = temp;
						readcount++;
					}
				}
			}
			break;
		}
		if(end)
			break;
	}
	
	if(pos < 3)
		return FALSE;
	
	return end;
}

char _XSkillItem::GetParameter(char* arg)
{
	if(arg)
	{
		switch(*arg)
		{
		case 'd' :
			{
				if(strcmp(arg, "distance") == 0)
					return _XSI_PARAM_DISTANCE;
			}
			break;
		case 'r' :
			{
				if(strcmp(arg, "radius") == 0)
					return _XSI_PARAM_RADIUS;
			}
			break;
		case 'm' :
			{
				if(strcmp(arg, "min_attack_damage") == 0)
					return _XSI_PARAM_MINATTACKDAMAGE;
				else if(strcmp(arg, "max_attack_damage") == 0)
					return _XSI_PARAM_MAXATTACKDAMAGE;
			}
			break;
		case 'a' :
			{
				if(strcmp(arg, "attack_rating") == 0)
					return _XSI_PARAM_ATTACKRATING;
				else if(strcmp(arg, "avoid_rating") == 0)
					return _XSI_PARAM_AVOIDRATING;
				else if(strcmp(arg, "add_bonus") == 0)
					return _XSI_PARAM_ADDBONUS;
				else if(strcmp(arg, "add_prob_1") == 0)
					return _XSI_PARAM_ADDPROB_1;
				else if(strcmp(arg, "add_value_1") == 0)
					return _XSI_PARAM_ADDVALUE_1;
				else if(strcmp(arg, "add_prob_2") == 0)
					return _XSI_PARAM_ADDPROB_2;
				else if(strcmp(arg, "add_value_2") == 0)
					return _XSI_PARAM_ADDVALUE_2;
				else if(strcmp(arg, "add_prob_3") == 0)
					return _XSI_PARAM_ADDPROB_3;
				else if(strcmp(arg, "add_value_3") == 0)
					return _XSI_PARAM_ADDVALUE_3;
				else if(strcmp(arg, "add_prob_4") == 0)
					return _XSI_PARAM_ADDPROB_4;
				else if(strcmp(arg, "add_value_4") == 0)
					return _XSI_PARAM_ADDVALUE_4;
				else if(strcmp(arg, "add_count_1") == 0)
					return _XSI_PARAM_ADDCOUNT_1;
				else if(strcmp(arg, "add_count_2") == 0)
					return _XSI_PARAM_ADDCOUNT_2;
				else if(strcmp(arg, "add_count_3") == 0)
					return _XSI_PARAM_ADDCOUNT_3;
				else if(strcmp(arg, "add_count_4") == 0)
					return _XSI_PARAM_ADDCOUNT_4;
			}
			break;
		case 'c' :
			{
				if(strcmp(arg, "cost_force") == 0)
					return _XSI_PARAM_COSTFORCE;
				else if(strcmp(arg, "cost_mana") == 0)
					return _XSI_PARAM_COSTMANA;
				else if(strcmp(arg, "cost_energy") == 0)
					return _XSI_PARAM_COSTENERGY;
			}
			break;
		case 'n' :
			{
				if(strcmp(arg, "number") == 0)
					return _XSI_PARAM_NUMBER;
			}
			break;
		case 'e' :
			{
				if(strcmp(arg, "eff_time") == 0)
					return _XSI_PARAM_EFFTIME;
				else if(strcmp(arg, "eff_prob_1") == 0)
					return _XSI_PARAM_EFFPROB_1;
				else if(strcmp(arg, "eff_value_1") == 0)
					return _XSI_PARAM_EFFVALUE_1;
				else if(strcmp(arg, "eff_prob_2") == 0)
					return _XSI_PARAM_EFFPROB_2;
				else if(strcmp(arg, "eff_value_2") == 0)
					return _XSI_PARAM_EFFVALUE_2;
				else if(strcmp(arg, "eff_prob_3") == 0)
					return _XSI_PARAM_EFFPROB_3;
				else if(strcmp(arg, "eff_value_3") == 0)
					return _XSI_PARAM_EFFVALUE_3;
				else if(strcmp(arg, "eff_prob_4") == 0)
					return _XSI_PARAM_EFFPROB_4;
				else if(strcmp(arg, "eff_value_4") == 0)
					return _XSI_PARAM_EFFVALUE_4;
				else if(strcmp(arg, "eff_prob_5") == 0)
					return _XSI_PARAM_EFFPROB_5;
				else if(strcmp(arg, "eff_value_5") == 0)
					return _XSI_PARAM_EFFVALUE_5;
				else if(strcmp(arg, "eff_prob_6") == 0)
					return _XSI_PARAM_EFFPROB_6;
				else if(strcmp(arg, "eff_value_6") == 0)
					return _XSI_PARAM_EFFVALUE_6;
				else if(strcmp(arg, "eff_prob_7") == 0)
					return _XSI_PARAM_EFFPROB_7;
				else if(strcmp(arg, "eff_value_7") == 0)
					return _XSI_PARAM_EFFVALUE_7;
				else if(strcmp(arg, "eff_prob_8") == 0)
					return _XSI_PARAM_EFFPROB_8;
				else if(strcmp(arg, "eff_value_8") == 0)
					return _XSI_PARAM_EFFVALUE_8;
			}
			break;
		}
	}
	return _XSI_PARAM_NONE;
}

short _XSkillItem::GetParameterValue(short skillid, char skilllevel, int param)
{
	switch(param)
	{
	case _XSI_PARAM_DISTANCE :
		{
			short distance = (short)_XSkillItem::GetDistance(skillid);//g_SkillProperty[skillid]->distance;
			return distance;
		}
		break;
	case _XSI_PARAM_RADIUS :
		{
			short radius = (short)g_SkillProperty[skillid]->radius;
			return radius;
		}
		break;
	case _XSI_PARAM_MINATTACKDAMAGE :
		return _XSkillItem::GetMinAttackDamage(skillid, skilllevel);//g_SkillProperty[skillid]->detailInfo[skilllevel].minAttackDamage;

	case _XSI_PARAM_MAXATTACKDAMAGE :
		return _XSkillItem::GetMaxAttackDamage(skillid, skilllevel);//g_SkillProperty[skillid]->detailInfo[skilllevel].maxAttackDamage;

	case _XSI_PARAM_ATTACKRATING :
		return _XSkillItem::GetAttackRating(skillid, skilllevel);//g_SkillProperty[skillid]->detailInfo[skilllevel].attackRating;

	case _XSI_PARAM_AVOIDRATING :
		return _XSkillItem::GetAvoidRating(skillid, skilllevel);//g_SkillProperty[skillid]->detailInfo[skilllevel].avoidRating;

	case _XSI_PARAM_COSTFORCE :
		return _XSkillItem::GetCostForce(skillid, skilllevel);//g_SkillProperty[skillid]->detailInfo[skilllevel].costForce;

//	case _XSI_PARAM_COSTMANA :
//		return g_SkillProperty[skillid]->detailInfo[skilllevel].costMind;

	case _XSI_PARAM_COSTENERGY :
		return _XSkillItem::GetCostMind(skillid, skilllevel);//g_SkillProperty[skillid]->detailInfo[skilllevel].costMind;

	case _XSI_PARAM_ADDBONUS :
		return g_SkillProperty[skillid]->detailInfo[skilllevel].addBonus;

	case _XSI_PARAM_NUMBER:
		return g_SkillProperty[skillid]->detailInfo[skilllevel].number;

	case _XSI_PARAM_EFFTIME :
		{
			int time = (short)(_XSkillItem::GetEffectTime(skillid, skilllevel)/*g_SkillProperty[skillid]->detailInfo[skilllevel].effectTime*/ / 60 );
			if( time > 0 )
				return time;			
			else
				return (short)(_XSkillItem::GetEffectTime(skillid, skilllevel)/*g_SkillProperty[skillid]->detailInfo[skilllevel].effectTime*/ % 60 );
		}
		break;

	case _XSI_PARAM_EFFPROB_1 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 1 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[0].prob;
			else
				return 0;
		}
	case _XSI_PARAM_EFFVALUE_1 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 1 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[0].value;
			else
				return 0;
		}
	case _XSI_PARAM_ADDPROB_1 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 1 )
				return _XSkillItem::GetAdditiveProb(skillid, skilllevel, 0);//g_SkillProperty[skillid]->detailInfo[skilllevel].additiveEffect[0].prob;
			else 
				return 0;
		}
	case _XSI_PARAM_ADDVALUE_1 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 1 )
				return _XSkillItem::GetAdditiveValue(skillid, skilllevel, 0);//g_SkillProperty[skillid]->detailInfo[skilllevel].additiveEffect[0].value;
			else 
				return 0;
		}
	case _XSI_PARAM_EFFPROB_2 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 2 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[1].prob;
			else
				return 0;
		}		
	case _XSI_PARAM_EFFVALUE_2 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 2 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[1].value;
			else
				return 0;
		}
	case _XSI_PARAM_ADDPROB_2 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 2 )
				return _XSkillItem::GetAdditiveProb(skillid, skilllevel, 1);//g_SkillProperty[skillid]->detailInfo[skilllevel].additiveEffect[1].prob;
			else 
				return 0;
		}
	case _XSI_PARAM_ADDVALUE_2 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 2 )
				return _XSkillItem::GetAdditiveValue(skillid, skilllevel, 1);//g_SkillProperty[skillid]->detailInfo[skilllevel].additiveEffect[1].value;
			else 
				return 0;
		}
	case _XSI_PARAM_EFFPROB_3 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 3 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[2].prob;
			else
				return 0;
		}	
	case _XSI_PARAM_EFFVALUE_3 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 3 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[2].value;
			else
				return 0;
		}		
	case _XSI_PARAM_ADDPROB_3 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 3 )
				return _XSkillItem::GetAdditiveProb(skillid, skilllevel, 2);//g_SkillProperty[skillid]->detailInfo[skilllevel].additiveEffect[2].prob;
			else 
				return 0;
		}
	case _XSI_PARAM_ADDVALUE_3 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 3 )
				return _XSkillItem::GetAdditiveValue(skillid, skilllevel, 2);//g_SkillProperty[skillid]->detailInfo[skilllevel].additiveEffect[2].value;
			else 
				return 0;
		}
	case _XSI_PARAM_EFFPROB_4 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 4 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[3].prob;
			else
				return 0;
		}
	case _XSI_PARAM_EFFVALUE_4 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 4 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[3].value;
			else
				return 0;
		}		
	case _XSI_PARAM_ADDPROB_4 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 4 )
				return _XSkillItem::GetAdditiveProb(skillid, skilllevel, 3);//g_SkillProperty[skillid]->detailInfo[skilllevel].additiveEffect[3].prob;
			else 
				return 0;
		}		
	case _XSI_PARAM_ADDVALUE_4 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 4 )
				return _XSkillItem::GetAdditiveValue(skillid, skilllevel, 3);//g_SkillProperty[skillid]->detailInfo[skilllevel].additiveEffect[3].value;
			else 
				return 0;
		}		
	case _XSI_PARAM_EFFPROB_5 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 5 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[4].prob;
			else
				return 0;
		}		
	case _XSI_PARAM_EFFVALUE_5 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 5 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[4].value;
			else
				return 0;
		}
	case _XSI_PARAM_EFFPROB_6 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 6 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[5].prob;
			else
				return 0;
		}		
	case _XSI_PARAM_EFFVALUE_6 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 6 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[5].value;
			else
				return 0;
		}		
	case _XSI_PARAM_EFFPROB_7 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 7 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[6].prob;
			else
				return 0;
		}		
	case _XSI_PARAM_EFFVALUE_7 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 7 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[6].value;
			else
				return 0;
		}		
	case _XSI_PARAM_EFFPROB_8 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 8 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[7].prob;
			else
				return 0;
		}		
	case _XSI_PARAM_EFFVALUE_8 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].effectCount >= 8 )
				return g_SkillProperty[skillid]->detailInfo[skilllevel].effect[7].value;
			else
				return 0;
		}
	case _XSI_PARAM_ADDCOUNT_1 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 1 )
			{
				int additiveCount = (_XSkillItem::GetAdditiveCount(skillid, skilllevel, 0) / 60 );
				if( additiveCount > 0 )
					return additiveCount;			
				else
					return (_XSkillItem::GetAdditiveCount(skillid, skilllevel, 0) % 60 );				
			}
			else
				return 0;

			/*if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 1 )
				return _XSkillItem::GetAdditiveCount(skillid, skilllevel, 0);//g_SkillProperty[skillid]->detailInfo[skilllevel].additiveEffect[0].count;
			else
				return 0;*/
		}
	case _XSI_PARAM_ADDCOUNT_2 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 2 )
			{
				int additiveCount = (_XSkillItem::GetAdditiveCount(skillid, skilllevel, 1) / 60 );
				if( additiveCount > 0 )
					return additiveCount;			
				else
					return (_XSkillItem::GetAdditiveCount(skillid, skilllevel, 1) % 60 );				
			}
			else
				return 0;
			/*
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 2 )
				return _XSkillItem::GetAdditiveCount(skillid, skilllevel, 1);//g_SkillProperty[skillid]->detailInfo[skilllevel].additiveEffect[1].count;
			else
				return 0;*/
		}
	case _XSI_PARAM_ADDCOUNT_3 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 3 )
			{
				int additiveCount = (_XSkillItem::GetAdditiveCount(skillid, skilllevel, 2) / 60 );
				if( additiveCount > 0 )
					return additiveCount;			
				else
					return (_XSkillItem::GetAdditiveCount(skillid, skilllevel, 2) % 60 );				
			}
			else
				return 0;
			/*
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 3 )
				return _XSkillItem::GetAdditiveCount(skillid, skilllevel, 2); //g_SkillProperty[skillid]->detailInfo[skilllevel].additiveEffect[2].count;
			else
				return 0;*/
		}
	case _XSI_PARAM_ADDCOUNT_4 :
		{
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 4 )
			{
				int additiveCount = (_XSkillItem::GetAdditiveCount(skillid, skilllevel, 3) / 60 );
				if( additiveCount > 0 )
					return additiveCount;			
				else
					return (_XSkillItem::GetAdditiveCount(skillid, skilllevel, 3) % 60 );				
			}
			else
				return 0;
			/*
			if( g_SkillProperty[skillid]->detailInfo[skilllevel].additiveCount >= 4 )
				return _XSkillItem::GetAdditiveCount(skillid, skilllevel, 3);//g_SkillProperty[skillid]->detailInfo[skilllevel].additiveEffect[3].count;
			else
				return 0;*/
		}
	}
	return 0;
}

short _XSkillItem::GetMasteryParameterValue(short skillid, char skilllevel, int param)
{
	switch(param) 
	{
	case _XSI_PARAM_EFFPROB_1 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effectCount >= 1 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effect[0].prob;
			else
				return 0;
		}
	case _XSI_PARAM_EFFVALUE_1 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effectCount >= 1 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effect[0].value;
			else
				return 0;
		}
	case _XSI_PARAM_ADDPROB_1 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveCount >= 1 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveEffect[0].prob;
			else 
				return 0;
		}
	case _XSI_PARAM_ADDVALUE_1 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveCount >= 1 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveEffect[0].value;
			else 
				return 0;
		}
	case _XSI_PARAM_EFFPROB_2 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effectCount >= 2 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effect[1].prob;
			else
				return 0;
		}
	case _XSI_PARAM_EFFVALUE_2 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effectCount >= 2 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effect[1].value;
			else
				return 0;
		}
	case _XSI_PARAM_ADDPROB_2 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveCount >= 2 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveEffect[1].prob;
			else 
				return 0;
		}
	case _XSI_PARAM_ADDVALUE_2 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveCount >= 2 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveEffect[1].value;
			else 
				return 0;
		}
	case _XSI_PARAM_EFFPROB_3 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effectCount >= 3 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effect[2].prob;
			else
				return 0;
		}
	case _XSI_PARAM_EFFVALUE_3 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effectCount >= 3 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effect[2].value;
			else
				return 0;
		}
	case _XSI_PARAM_ADDPROB_3 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveCount >= 3 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveEffect[2].prob;
			else 
				return 0;
		}
	case _XSI_PARAM_ADDVALUE_3 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveCount >= 3 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveEffect[2].value;
			else 
				return 0;
		}
	case _XSI_PARAM_EFFPROB_4 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effectCount >= 4 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effect[3].prob;
			else
				return 0;
		}
	case _XSI_PARAM_EFFVALUE_4 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effectCount >= 4 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].effect[3].value;
			else
				return 0;
		}
	case _XSI_PARAM_ADDCOUNT_1 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveCount >= 1 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveEffect[0].count;
			else 
				return 0;
		}
	case _XSI_PARAM_ADDCOUNT_2 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveCount >= 2 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveEffect[1].count;
			else 
				return 0;
		}
	case _XSI_PARAM_ADDCOUNT_3 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveCount >= 3 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveEffect[2].count;
			else 
				return 0;
		}
	case _XSI_PARAM_ADDCOUNT_4 :
		{
			if( g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveCount >= 4 )
				return g_SkillMasteryProperty[skillid]->detailInfo[skilllevel].additiveEffect[3].count;
			else 
				return 0;
		}
	}
	return 0;	
}

BOOL _XSkillItem::LoadSkillMasteryProperty(void)
{
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		_XDeleteFile(_T("mastery_table.bms"));
	}		
#endif

	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );
	
	FILE* fileptr = NULL;

#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		
		fileptr = fopen( _T("mastery_table.bms"), "rb" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;			
			fileptr = g_ScriptArchive.GetPackedFile( _T("mastery_table.bms") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("mastery_table.bms") );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( _T("mastery_table.bms"), "rb" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( _T("mastery_table.bms") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("mastery_table.bms") );
	}
#endif

	if( !fileptr )
	{
		_XFatalError("Load skill mastery table (%s) file open", _T("mastery_table.bms"));
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	// Read Header
	TCHAR		fileheadername[_XDEF_SKILLTABLE_HEADERNAMELENGTH + 1];
	short		version[2];
	SYSTEMTIME	updatetime;
	char		temp[32];
	
	memset(fileheadername, 0, sizeof(TCHAR)*(_XDEF_SKILLTABLE_HEADERNAMELENGTH + 1));
	if(fread(fileheadername, sizeof(TCHAR)*_XDEF_SKILLTABLE_HEADERNAMELENGTH, 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(&version[0], sizeof(short), 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(&version[1], sizeof(short), 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(&updatetime, sizeof(SYSTEMTIME), 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(temp, sizeof(char)*16, 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	if(fread(temp, sizeof(char)*20, 1, fileptr) < 1)
	{
		if( bLoadFromFile ) fclose( fileptr );
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	// Read mastery skill information
	short	skillid = 0;
	u_char	len = 0;
	while(1)
	{
		// id
		if(fread(&skillid, sizeof(short), 1, fileptr) < 1)
		{
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		if(skillid == -1)
			break;

		_XSI_MasterySkillStruct* skillinfo = new _XSI_MasterySkillStruct;
		memset(skillinfo, 0, sizeof(_XSI_MasterySkillStruct));
		skillinfo->sMasteryID = skillid;

		// name
		if(fread(&len, sizeof(u_char), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		if(len > 0)
		{
#ifdef _XDEF_NEWSTRINGMANAGER					
			fseek(fileptr, sizeof(TCHAR)*len, SEEK_CUR );
			skillinfo->cName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_MASTERY_TABLE_BMS_NAME, skillinfo->sMasteryID );
#else
			if(fread(skillinfo->cName, sizeof(TCHAR)*len, 1, fileptr) < 1)
			{
				delete skillinfo;
				if( bLoadFromFile ) fclose( fileptr );
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}

			//_XLog( "%d : %s\n", skillid, skillinfo->cName );
#endif
		}
		// tree level
		if(fread(&skillinfo->sTreeLevel, sizeof(short), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// clan type
		if(fread(&skillinfo->cClanType, sizeof(char), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// class
		if(fread(&skillinfo->cClass, sizeof(char), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// class grade
		if(fread(&skillinfo->cClassGrade, sizeof(char), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// mastery type
		if(fread(&skillinfo->cMasteryType, sizeof(char), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// max mp
		if(fread(&skillinfo->cMaxMP, sizeof(char), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// need master id
		if(fread(&skillinfo->sNeedMasteryID, sizeof(short), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// need page total
		if(fread(&skillinfo->sNeedPageTotalPoint, sizeof(short), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// apply type
		if(fread(&skillinfo->cApplyType, sizeof(char), 1, fileptr) < 1)
		{
			delete skillinfo;
			if(bLoadFromFile) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		
#ifdef _XTS_MASTERY_04
		// add skill
		if(fread(&skillinfo->sAddSkill, sizeof(short), 1, fileptr) < 1)
		{
			delete skillinfo;
			if(bLoadFromFile) fclose(fileptr);
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
#endif

		// target skill's stype
		if(fread(&skillinfo->cTargetSkill_Stype, sizeof(char), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// target skill 1
		if(fread(&skillinfo->sTargetSkill_1, sizeof(short), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// target skill 2
		if(fread(&skillinfo->sTargetSkill_2, sizeof(short), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// target skill 3
		if(fread(&skillinfo->sTargetSkill_3, sizeof(short), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// target skill 4
		if(fread(&skillinfo->sTargetSkill_4, sizeof(short), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// target weapon
		short sTemp;
		if(fread(&sTemp, sizeof(short), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		skillinfo->cWeaponSType = sTemp / 100;
		skillinfo->cWeaponTType = sTemp % 100;

		// inside type
		if(fread(&skillinfo->cInside_Type, sizeof(char), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		// special type
		if(fread(&skillinfo->cSpecial_Type, sizeof(char), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}

		unsigned char ucContexLen = 0;
		// context length
		if(fread(&ucContexLen, sizeof(u_char), 1, fileptr) < 1)
		{
			delete skillinfo;
			if( bLoadFromFile ) fclose( fileptr );
			else g_ScriptArchive.ClosePackage();
			return FALSE;
		}
		if(ucContexLen > 0)
		{
			if(ucContexLen >= 128)
				ucContexLen = 127;

#ifdef _XDEF_NEWSTRINGMANAGER					
			fseek(fileptr, sizeof(TCHAR)*ucContexLen, SEEK_CUR );
			skillinfo->pContext = g_StringDataBase.GetTextString( _XSTRINGDB_SN_MASTERY_TABLE_BMS_CONTEXT, skillinfo->sMasteryID );
#else
			// concept
			if(fread(skillinfo->pContext, ucContexLen, 1, fileptr) < 1)
			{
				delete skillinfo;
				if( bLoadFromFile ) fclose( fileptr );
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}
#endif
		}

		char nEffect = 0;
		char nAddEffect = 0;
		for(int i = 0 ; i < 20 ; ++i)
		{
			if(fread(&skillinfo->detailInfo[i].sReqMp, sizeof(short), 1, fileptr) < 1)
			{
				delete skillinfo;
				if( bLoadFromFile ) fclose( fileptr );
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}

			if(fread(&skillinfo->detailInfo[i].effectCount, sizeof(char), 1, fileptr) < 1)
			{
				delete skillinfo;
				if( bLoadFromFile ) fclose( fileptr );
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}
			
			if(skillinfo->detailInfo[i].effectCount > 0)
			{
				skillinfo->detailInfo[i].effect = new _XSI_SkillEffect[skillinfo->detailInfo[i].effectCount];

				for(int ii = 0 ; ii < skillinfo->detailInfo[i].effectCount ; ++ii)
				{
					if(fread(&skillinfo->detailInfo[i].effect[ii].id,  sizeof(short), 1, fileptr) < 1)
					{
						if( bLoadFromFile ) fclose( fileptr );
						else g_ScriptArchive.ClosePackage();
						return FALSE;
					}
					if(fread(&skillinfo->detailInfo[i].effect[ii].prob, sizeof(short), 1, fileptr) < 1)
					{
						if( bLoadFromFile ) fclose( fileptr );
						else g_ScriptArchive.ClosePackage();
						return FALSE;
					}
					if(fread(&skillinfo->detailInfo[i].effect[ii].value, sizeof(short), 1, fileptr) < 1)
					{
						if( bLoadFromFile ) fclose( fileptr );
						else g_ScriptArchive.ClosePackage();
						return FALSE;
					}
				}
			}

			if(fread(&skillinfo->detailInfo[i].additiveCount, sizeof(char), 1, fileptr) < 1)
			{
				if( bLoadFromFile ) fclose( fileptr );
				else g_ScriptArchive.ClosePackage();
				return FALSE;
			}

			if(skillinfo->detailInfo[i].additiveCount > 0)
			{
				skillinfo->detailInfo[i].additiveEffect = new _XSI_SkillAdditive[skillinfo->detailInfo[i].additiveCount];
			
				for(int ii = 0 ; ii < skillinfo->detailInfo[i].additiveCount ; ++ii)
				{
					if(fread(&skillinfo->detailInfo[i].additiveEffect[ii].id, sizeof(short), 1, fileptr) < 1)
					{
						if( bLoadFromFile ) fclose( fileptr );
						else g_ScriptArchive.ClosePackage();
						return FALSE;
					}
					if(fread(&skillinfo->detailInfo[i].additiveEffect[ii].prob, sizeof(short), 1, fileptr) < 1)
					{
						if( bLoadFromFile ) fclose( fileptr );
						else g_ScriptArchive.ClosePackage();
						return FALSE;
					}
					if(fread(&skillinfo->detailInfo[i].additiveEffect[ii].value, sizeof(short), 1, fileptr) < 1)
					{
						if( bLoadFromFile ) fclose( fileptr );
						else g_ScriptArchive.ClosePackage();
						return FALSE;
					}
					if(fread(&skillinfo->detailInfo[i].additiveEffect[ii].count, sizeof(short), 1, fileptr) < 1)
					{
						if( bLoadFromFile ) fclose( fileptr );
						else g_ScriptArchive.ClosePackage();
						return FALSE;
					}
				}
			}
		} // for(int i = 0 ; i < 20 ; i++)

		g_SkillMasteryProperty[skillid] = skillinfo;

	} // while(1)

	if( bLoadFromFile ) fclose( fileptr );
	else g_ScriptArchive.ClosePackage();
	
	return TRUE;
}

void _XSkillItem::ReleaseSkillMasteryProperty(void)
{
	map <short, _XSI_MasterySkillStruct*>::iterator iter_mastery;
	for(iter_mastery = g_SkillMasteryProperty.begin() ; iter_mastery != g_SkillMasteryProperty.end() ; ++iter_mastery)
	{
		_XSI_MasterySkillStruct* skillinfo = iter_mastery->second;
		if(skillinfo)
		{
			for(int i = 0 ; i < 20 ; ++i)
			{
				if(skillinfo->detailInfo[i].effect)
				{
					delete[] skillinfo->detailInfo[i].effect;
					skillinfo->detailInfo[i].effect = NULL;
				}
				if(skillinfo->detailInfo[i].additiveEffect)
				{
					delete[] skillinfo->detailInfo[i].additiveEffect;
					skillinfo->detailInfo[i].additiveEffect = NULL;
				}
			}
			delete skillinfo;
		}
	}
	g_SkillMasteryProperty.clear();
}

void _XSkillItem::ResetSkillModTable(void)
{
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod;
	for(iter_mod = g_Skill_Mod_Property.begin() ; iter_mod != g_Skill_Mod_Property.end() ; ++iter_mod)
	{
		_XSKILLINFO_MOD_STRUCTURE* currentinfo = iter_mod->second;
		if(currentinfo)
		{
			memset(currentinfo->bUse, 0, sizeof(currentinfo->bUse));
			currentinfo->targetType = 0;
			currentinfo->distance = 0;
			memset(currentinfo->detailInfo, 0, sizeof(currentinfo->detailInfo));
		}
	}
}

char _XSkillItem::GetTargetType(short skillid)
{
	char targetType = 0;

	if(FindSkillProperty(skillid))
	{
		targetType = g_SkillProperty[skillid]->targetType;
	}

#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_target_type])
			{
				if(currentskill->targetType != 0)
					targetType = currentskill->targetType;
			}
		}
	}
#endif

	return targetType;
}

float _XSkillItem::GetDistance(short skillid)
{
	float distance = 0;

	if(FindSkillProperty(skillid))
	{
		distance = g_SkillProperty[skillid]->distance;
	}

#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_distance])
			{
				distance = currentskill->distance;
			}
		}
	}
#endif
	
	return distance;
}

short _XSkillItem::GetMinAttackDamage(short skillid, char level)
{
	short damage = 0;

	if(FindSkillProperty(skillid))
	{
		damage = g_SkillProperty[skillid]->detailInfo[level].minAttackDamage;
	}
	
#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_min_dam])
			{
				damage = currentskill->detailInfo[level].minAttackDamage;	
			}
		}
	}
#endif
	
	return damage;
}

short _XSkillItem::GetMaxAttackDamage(short skillid, char level)
{
	short damage = 0;

	if(FindSkillProperty(skillid))
	{
		damage = g_SkillProperty[skillid]->detailInfo[level].maxAttackDamage;
	}

#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_max_dam])
			{
				damage = currentskill->detailInfo[level].maxAttackDamage;	
			}
		}
	}
#endif
	
	return damage;
}

short _XSkillItem::GetAttackRating(short skillid, char level)
{
	short attackrate = 0;

	if(FindSkillProperty(skillid))
	{
		attackrate = g_SkillProperty[skillid]->detailInfo[level].attackRating;
	}

#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_attackrate])
			{
				attackrate = currentskill->detailInfo[level].attackRating;		
			}
		}
	}
#endif
	
	return attackrate;
}

short _XSkillItem::GetAvoidRating(short skillid, char level)
{
	short avoidrate = 0;

	if(FindSkillProperty(skillid))
	{
		avoidrate = g_SkillProperty[skillid]->detailInfo[level].avoidRating;
	}

#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_avoidrate])
			{
				avoidrate = currentskill->detailInfo[level].avoidRating;	
			}
		}
	}
#endif
	
	return avoidrate;
}

short _XSkillItem::GetCostForce(short skillid, char level)
{
	short cost = 0;

	if(FindSkillProperty(skillid))
	{
		cost = g_SkillProperty[skillid]->detailInfo[level].costForce;
	}

#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_costinner])
			{
				cost = currentskill->detailInfo[level].costForce;	
			}
		}
	}
#endif
	
	return cost;
}

short _XSkillItem::GetCostMind(short skillid, char level)
{
	short cost = 0;

	if(FindSkillProperty(skillid))
	{
		cost = g_SkillProperty[skillid]->detailInfo[level].costMind;
	}

#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_costmind])
			{
				cost = currentskill->detailInfo[level].costMind;	
			}
		}
	}
#endif
	
	return cost;
}

float _XSkillItem::GetFinishSpeed(short skillid, char level)
{
	float speed = 0;

	if(FindSkillProperty(skillid))
	{
		speed = g_SkillProperty[skillid]->detailInfo[level].finishSpeed;
	}

#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_finishspeed])
			{
				speed = currentskill->detailInfo[level].finishSpeed;	
			}
		}
	}
#endif
	
	return speed;
}

int _XSkillItem::GetCasting(short skillid, char level)
{
	int casting = 0;

	if(FindSkillProperty(skillid))
	{
		casting = g_SkillProperty[skillid]->detailInfo[level].casting;
	}

#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_castingtime])
			{
				casting = currentskill->detailInfo[level].casting;	
			}
		}
	}
#endif
	
	return casting;
}

int	_XSkillItem::GetDelay(short skillid, char level)
{
	int delay = 0;

	if(FindSkillProperty(skillid))
	{
		delay = g_SkillProperty[skillid]->detailInfo[level].delay;
	}

#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_delay])
			{
				delay = currentskill->detailInfo[level].delay;	
			}
		}
	}
#endif
	
	return delay;
}

int	_XSkillItem::GetCoolDown(short skillid, char level)
{
	int cooldown = 0;

	if(FindSkillProperty(skillid))
	{
		cooldown = g_SkillProperty[skillid]->detailInfo[level].coolDown;
	}

#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_cooldown])
			{
				cooldown = currentskill->detailInfo[level].coolDown;	
			}
		}
	}
#endif
	
	return cooldown;
}

unsigned short _XSkillItem::GetEffectTime(short skillid, char level)
{
	unsigned short effecttime = 0;

	if(FindSkillProperty(skillid))
	{
		effecttime = g_SkillProperty[skillid]->detailInfo[level].effectTime;
	}

#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_incre_effecttime])
			{
				effecttime = currentskill->detailInfo[level].effectTime;	
			}
		}
	}
#endif
	
	return effecttime;
}

short _XSkillItem::GetAdditiveValue(short skillid, char level, int index)
{
	short value = 0;
	
	if(FindSkillProperty(skillid))
	{
		if(g_SkillProperty[skillid]->detailInfo[level].additiveCount > index)
			value = g_SkillProperty[skillid]->detailInfo[level].additiveEffect[index].value;
	}
	
#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_additive_1_value+index])
			{
				if(g_SkillProperty[skillid]->detailInfo[level].additiveCount > index)
				{
					value = currentskill->detailInfo[level].additiveEffect[index].value;
				}
			}
		}
	}
#endif
	
	return value;
}

short _XSkillItem::GetAdditiveProb(short skillid, char level, int index)
{
	short prob = 0;
	
	if(FindSkillProperty(skillid))
	{
		if(g_SkillProperty[skillid]->detailInfo[level].additiveCount > index)
			prob = g_SkillProperty[skillid]->detailInfo[level].additiveEffect[index].prob;
	}
	
#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_additive_1_prob+index])
			{
				if(g_SkillProperty[skillid]->detailInfo[level].additiveCount > index)
				{
					prob = currentskill->detailInfo[level].additiveEffect[index].prob;
				}
			}
		}
	}
#endif
	
	return prob;
}

short _XSkillItem::GetAdditiveCount(short skillid, char level, int index)
{
	short count = 0;
	
	if(FindSkillProperty(skillid))
	{
		if(g_SkillProperty[skillid]->detailInfo[level].additiveCount > index)
			count = g_SkillProperty[skillid]->detailInfo[level].additiveEffect[index].count;
	}


#ifdef _XTS_MASTERY
	map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(skillid);
	if(iter_mod != g_Skill_Mod_Property.end())
	{
		_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
		if(currentskill)
		{
			if(currentskill->bUse[en_mod_additive_1_time+index])
			{
				if(g_SkillProperty[skillid]->detailInfo[level].additiveCount > index)
				{
					count = currentskill->detailInfo[level].additiveEffect[index].count;
				}
			}
		}
	}
#endif

	
	return count;
}

BOOL _XSkillItem::LoadBattleParameter(void)
{
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;
	
#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		_XDeleteFile(_T("ND_Battle_Parameter.txt"));
	}		
#endif
	
	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );
	
	FILE* fileptr = NULL;
	
	
#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		
		fileptr = fopen( _T("ND_Battle_Parameter.txt"), "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( _T("ND_Battle_Parameter.txt") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("ND_Battle_Parameter.txt") );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( _T("ND_Battle_Parameter.txt"), "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( _T("ND_Battle_Parameter.txt") );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( _T("ND_Battle_Parameter.txt") );
	}
#endif
	
	if( !fileptr )
	{
		_XFatalError( "Load Battle Parameter" );
		if( bLoadFromFile ) fclose(fileptr);
		else g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	char	cmd[120];
	bool	bEnd = false;
	bool	bComplete = false;

	char	arg1[40];
	FLOAT	param[11];

	// 물리 데미지
	memset(g_PD_Chr_Step_Mul, 0, sizeof(g_PD_Chr_Step_Mul));
	memset(g_PD_Chr_Level_Mul, 0, sizeof(g_PD_Chr_Level_Mul));
	memset(g_PD_Str_Mul, 0, sizeof(g_PD_Str_Mul));
	memset(g_PD_Weapon_Str_Add, 0, sizeof(g_PD_Weapon_Str_Add));

	// 기공 데미지
	memset(g_MD_Chr_Step_Mul, 0, sizeof(g_MD_Chr_Step_Mul));
	memset(g_MD_Chr_Level_Mul, 0, sizeof(g_MD_Chr_Level_Mul));
	memset(g_MD_Int_Mul, 0, sizeof(g_MD_Int_Mul));
	memset(g_MD_Spell_Mul, 0, sizeof(g_MD_Spell_Mul));
	memset(g_MD_Spell_Add, 0, sizeof(g_MD_Spell_Add));
	memset(g_MD_Zen_Mul, 0, sizeof(g_MD_Zen_Mul));

	// 물리 방어력
	memset(g_PF_Chr_Step_Mul, 0, sizeof(g_PF_Chr_Step_Mul));
	memset(g_PF_Chr_Level_Mul, 0, sizeof(g_PF_Chr_Level_Mul));
	memset(g_PF_Str_Mul, 0, sizeof(g_PF_Str_Mul));

	// 기공 방어력
	memset(g_MF_Chr_Step_Mul, 0, sizeof(g_MF_Chr_Step_Mul));
	memset(g_MF_Chr_Level_Mul, 0, sizeof(g_MF_Chr_Level_Mul));
	memset(g_MF_Int_Mul, 0, sizeof(g_MF_Int_Mul));
	memset(g_MF_Zen_Mul, 0, sizeof(g_MF_Zen_Mul));

	// 물리 공격 성공
	memset(g_PAR_Chr_Step_Mul, 0, sizeof(g_PAR_Chr_Step_Mul));
	memset(g_PAR_Chr_Level_Mul, 0, sizeof(g_PAR_Chr_Level_Mul));
	memset(g_PAR_Dex_Mul, 0, sizeof(g_PAR_Dex_Mul));

	// 기공 공격 성공
	memset(g_MAR_Chr_Step_Mul, 0, sizeof(g_MAR_Chr_Step_Mul));
	memset(g_MAR_Chr_Level_Mul, 0, sizeof(g_MAR_Chr_Level_Mul));
	memset(g_MAR_Int_Mul, 0, sizeof(g_MAR_Int_Mul));

	// 물리 회피
	memset(g_PDR_Chr_Step_Mul, 0, sizeof(g_PDR_Chr_Step_Mul));
	memset(g_PDR_Chr_Level_Mul, 0, sizeof(g_PDR_Chr_Level_Mul));
	memset(g_PDR_Dex_Mul, 0, sizeof(g_PDR_Dex_Mul));

	// 기공 회피
	memset(g_MDR_Chr_Step_Mul, 0, sizeof(g_MDR_Chr_Step_Mul));
	memset(g_MDR_Chr_Level_Mul, 0, sizeof(g_MDR_Chr_Level_Mul));
	memset(g_MDR_Int_Mul, 0, sizeof(g_MDR_Int_Mul));
	
#ifdef _XTS_BASEMINMAXWEAPONDAMAGE
	// 물리 무기 기본 공격력
	memset(g_PD_Base_Min_Weapon_Mul, 0, sizeof(g_PD_Base_Min_Weapon_Mul));
	memset(g_PD_Base_Max_Weapon_Mul, 0, sizeof(g_PD_Base_Max_Weapon_Mul));
#endif

#ifdef _XTS_NEWBATTLEPARAMETER
	memset(g_PAB_Str_Mul, 0, sizeof(g_PAB_Str_Mul));
	memset(g_PAB_dex_Mul, 0, sizeof(g_PAB_dex_Mul));
	memset(g_MAB_Int_Mul, 0, sizeof(g_MAB_Int_Mul));
	memset(g_CRI_AR_Dex_Mul, 0, sizeof(g_CRI_AR_Dex_Mul));
	memset(g_CRI_AR_Int_Mul, 0, sizeof(g_CRI_AR_Int_Mul));
	
	memset(g_PF_Vital_Mul, 0, sizeof(g_PF_Vital_Mul));
	memset(g_PAR_Str_Mul, 0, sizeof(g_PAR_Str_Mul));
	memset(g_MAR_Dex_Mul, 0, sizeof(g_MAR_Dex_Mul));
	memset(g_MDR_Dex_Mul, 0, sizeof(g_MDR_Dex_Mul));
	memset(g_PAB_Int_Mul, 0, sizeof(g_PAB_Int_Mul));
	memset(g_CRI_MAR_Dex_Mul, 0, sizeof(g_CRI_MAR_Dex_Mul));
#endif

	while(1)
	{
		while(GetCommand(cmd, fileptr))
		{
			switch(cmd[0])
			{
			case 'P' :
				{
					memset(arg1, 0, sizeof(arg1));
					memset(param, 0, sizeof(param));

					if(sscanf(cmd, "%s %f %f %f %f %f %f %f %f %f %f %f", arg1, &param[0], &param[1], &param[2], &param[3], &param[4], &param[5], &param[6],
						&param[7], &param[8], &param[9], &param[10]))
					{
						// 물리 데미지
						if(strncmp(arg1, "PD_Chr_Step_Mul", strlen("PD_Chr_Step_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PD_Chr_Step_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PD_Chr_Level_Mul", strlen("PD_Chr_Level_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PD_Chr_Level_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PD_Str_Mul", strlen("PD_Str_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PD_Str_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PD_Weapon_Str_Add", strlen("PD_Weapon_Str_Add")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PD_Weapon_Str_Add[i] = param[i];
							}
						}
						// 물리 방어
						else if(strncmp(arg1, "PF_Chr_Step_Mul", strlen("PF_Chr_Step_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PF_Chr_Step_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PF_Chr_Level_Mul", strlen("PF_Chr_Level_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PF_Chr_Level_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PF_Str_Mul", strlen("PF_Str_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PF_Str_Mul[i] = param[i];
							}
						}
						// 물리 공격 성공
						else if(strncmp(arg1, "PAR_Chr_Step_Mul", strlen("PAR_Chr_Step_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PAR_Chr_Step_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PAR_Chr_Level_Mul", strlen("PAR_Chr_Level_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PAR_Chr_Level_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PAR_Dex_Mul", strlen("PAR_Dex_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PAR_Dex_Mul[i] = param[i];
							}
						}
						// 물리 회피
						else if(strncmp(arg1, "PDR_Chr_Step_Mul", strlen("PDR_Chr_Step_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PDR_Chr_Step_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PDR_Chr_Level_Mul", strlen("PDR_Chr_Level_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PDR_Chr_Level_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PDR_Dex_Mul", strlen("PDR_Dex_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PDR_Dex_Mul[i] = param[i];
							}
						}
#ifdef _XTS_BASEMINMAXWEAPONDAMAGE
						// 물리 무기 기본 공격력
						else if(strncmp(arg1, "PD_Base_Min_Weapon_Mul", strlen("PD_Base_Min_Weapon_Mul")) == 0)
						{
							for(int i = 0 ; i < 10 ; ++i)
							{
								g_PD_Base_Min_Weapon_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PD_Base_Max_Weapon_Mul", strlen("PD_Base_Max_Weapon_Mul")) == 0)
						{
							for(int i = 0 ; i < 10 ; ++i)
							{
								g_PD_Base_Max_Weapon_Mul[i] = param[i];
							}
						}
#endif
#ifdef _XTS_NEWBATTLEPARAMETER
						// 물리 공격 균형 : Attack Balance
						else if(strncmp(arg1, "PAB_Str_Mul", strlen("PAB_Str_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PAB_Str_Mul[i] = param[i];
							}
						}
						// 물리 공격 균형 : Attack Balance
						else if(strncmp(arg1, "PAB_dex_Mul", strlen("PAB_dex_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PAB_dex_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PF_Vital_Mul", strlen("PF_Vital_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PF_Vital_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PAR_Str_Mul", strlen("PAR_Str_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PAR_Str_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "PAB_Int_Mul", strlen("PAB_Int_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_PAB_Int_Mul[i] = param[i];
							}
						}
#endif
					}
				}
				break;
			case 'M' :
				{
					memset(arg1, 0, sizeof(arg1));
					memset(param, 0, sizeof(param));

					if(sscanf(cmd, "%s %f %f %f %f %f %f %f %f %f %f %f", arg1, &param[0], &param[1], &param[2], &param[3], &param[4], &param[5], &param[6],
						&param[7], &param[8], &param[9], &param[10]))
					{
						// 기공 데미지
						if(strncmp(arg1, "MD_Chr_Step_Mul", strlen("MD_Chr_Step_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MD_Chr_Step_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MD_Chr_Level_Mul", strlen("MD_Chr_Level_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MD_Chr_Level_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MD_Int_Mul", strlen("MD_Int_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MD_Int_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MD_Spell_Mul", strlen("MD_Spell_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MD_Spell_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MD_Spell_Add", strlen("MD_Spell_Add")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MD_Spell_Add[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MD_Zen_Mul", strlen("MD_Zen_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MD_Zen_Mul[i] = param[i];
							}
						}
						// 기공 방어력
						else if(strncmp(arg1, "MF_Chr_Step_Mul", strlen("MF_Chr_Step_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MF_Chr_Step_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MF_Chr_Level_Mul", strlen("MF_Chr_Level_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MF_Chr_Level_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MF_Int_Mul", strlen("MF_Int_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MF_Int_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MF_Zen_Mul", strlen("MF_Zen_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MF_Zen_Mul[i] = param[i];
							}
						}
						// 기공 공격성공
						else if(strncmp(arg1, "MAR_Chr_Step_Mul", strlen("MAR_Chr_Step_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MAR_Chr_Step_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MAR_Chr_Level_Mul", strlen("MAR_Chr_Level_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MAR_Chr_Level_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MAR_Int_Mul", strlen("MAR_Int_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MAR_Int_Mul[i] = param[i];
							}
						}
						// 기공 회피
						else if(strncmp(arg1, "MDR_Chr_Step_Mul", strlen("MDR_Chr_Step_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MDR_Chr_Step_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MDR_Chr_Level_Mul", strlen("MDR_Chr_Level_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MDR_Chr_Level_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MDR_Int_Mul", strlen("MDR_Int_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MDR_Int_Mul[i] = param[i];
							}
						}
					
#ifdef _XTS_NEWBATTLEPARAMETER						
						// 기공 공격 균형
						else if(strncmp(arg1, "MAB_Int_Mul", strlen("MAB_Int_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MAB_Int_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MAR_Dex_Mul", strlen("MAR_Dex_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MAR_Dex_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "MDR_Dex_Mul", strlen("MDR_Dex_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_MDR_Dex_Mul[i] = param[i];
							}
						}
#endif
					}
				}
				break;
			case 'E' :
				{
					bComplete = true;
					bEnd = true;
				}
				break;
			case 'C' :
				{
#ifdef _XTS_NEWBATTLEPARAMETER						
					memset(arg1, 0, sizeof(arg1));
					memset(param, 0, sizeof(param));

					if(sscanf(cmd, "%s %f %f %f %f %f %f %f %f %f %f %f", arg1, &param[0], &param[1], &param[2], &param[3], &param[4], &param[5], &param[6],
						&param[7], &param[8], &param[9], &param[10]))
					{
						// 치명타 성공 확률 (Dex)
						if(strncmp(arg1, "CRI_AR_Dex_Mul", strlen("CRI_AR_Dex_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_CRI_AR_Dex_Mul[i] = param[i];
							}
						}
						// 치명타 성공 확률 (지혜)
						else if(strncmp(arg1, "CRI_AR_Int_Mul", strlen("CRI_AR_Int_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_CRI_AR_Int_Mul[i] = param[i];
							}
						}
						else if(strncmp(arg1, "CRI_MAR_Dex_Mul", strlen("CRI_MAR_Dex_Mul")) == 0)
						{
							for(int i = 0 ; i < 11 ; ++i)
							{
								g_CRI_MAR_Dex_Mul[i] = param[i];
							}
						}
					}
#endif
				}
				break;
			}

			if(bEnd)
				break;
		}
		if(bEnd)
			break;
	}

	if( bLoadFromFile ) fclose( fileptr );
	else g_ScriptArchive.ClosePackage();

	return bComplete;
}

BOOL _XSkillItem::GetCommand(char* cmd, FILE* fileptr, bool bnumber)
{
	while(1)
	{
		if(!fgets(cmd, 120, fileptr))
			return FALSE;
		else
		{
			if(cmd[0] == '/' || cmd[0] == '\n' || cmd[0] == ' ')
				continue;
			
			int i = 0 ;
			int len = strlen(cmd);
			
			while((!isalnum(cmd[i]) && cmd[i] != '{' && cmd[i] != '}') && i < len)
			{
				i++;
			}
			if(i > 0 && i < len)
			{
				memmove(&cmd[0], &cmd[i], len-i+1);		// 끝에 '0' 포함
			}
			
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
