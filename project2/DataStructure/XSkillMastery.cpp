// XSkillMastery.cpp: implementation of the _XSkillMastery class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"
#include "XSkillMastery.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XSkillMastery::_XSkillMastery()
{

}

_XSkillMastery::~_XSkillMastery()
{
	_vec_MasteryDisplayTableInfo::iterator iter_table = m_vecMasteryDisplayTable.begin();
	for( ; iter_table != m_vecMasteryDisplayTable.end() ; ++iter_table)
	{
		_XMasteryDisplayTableInfo* currentinfo = *iter_table;
		SAFE_DELETE(currentinfo);
	}
	m_vecMasteryDisplayTable.clear();

	_vec_MasteryName::iterator iter_name = m_vecMasteryNameId.begin();
	for( ; iter_name != m_vecMasteryNameId.end() ; ++iter_name)
	{
		_XMasteryNameTableInfo* currentinfo = *iter_name;
		SAFE_DELETE(currentinfo);
	}
	m_vecMasteryNameId.clear();

	_vec_MasteryLookupTableInfo::iterator iter_lookup = m_vecMasteryLookupTable.begin();
	for( ; iter_lookup != m_vecMasteryLookupTable.end() ; ++iter_lookup)
	{
		_XMasteryLookupTableInfo* currentinfo = *iter_lookup;
		SAFE_DELETE(currentinfo);
	}
	m_vecMasteryLookupTable.clear();
}

BOOL _XSkillMastery::LoadSkillMasteryDisplayTable()
{
	FILE* fileptr = NULL;

	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

	fileptr = g_ScriptArchive.GetPackedFile(_T("Mastery_Table.XMS"));

	if(!fileptr)
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	TCHAR buffer[256];
	TCHAR cmd[64];
	int index = 0, clan = 0, cClass = 0, grade = 0, page = 0;
	TCHAR name[128];

	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(buffer));
		if(!fgets(buffer, 255, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;

		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if(strncmp(buffer, _T("_XM_CLAN"), 8) == 0)
			{
				sscanf(buffer, "%s %d", cmd, &clan);
			}
			else if(strncmp(buffer, _T("_XM_CLASS"), 9) == 0)
			{
				sscanf(buffer, " %s %d", cmd, &cClass);
			}
			else if(strncmp(buffer, _T("_XM_GRADE"), 9) == 0)
			{
				sscanf(buffer, " %s %d", cmd, &grade);
			}
			else if(strncmp(buffer, _T("_XM_MASTERYCLASS"), 16) == 0)
			{
				sscanf(buffer, "%s %d %s", cmd, &page, name);

				int strresourceid = 0;

#ifdef _XDEF_NEWSTRINGMANAGER
				TCHAR clanbuffer[8];
				TCHAR classbuffer[8];
				TCHAR pagebuffer[8];
				TCHAR combtempbuffer[16];
				memset(clanbuffer, 0, sizeof(clanbuffer));
				memset( combtempbuffer, 0, sizeof(combtempbuffer) );
				
				_snprintf(combtempbuffer, sizeof(combtempbuffer), "%d", grade);
				_snprintf(clanbuffer, sizeof(clanbuffer), "%d", clan);
				strcat(combtempbuffer, clanbuffer);

				_snprintf(classbuffer, sizeof(classbuffer), "%d", cClass);
				strcat(combtempbuffer, classbuffer);

				_snprintf(pagebuffer, sizeof(pagebuffer), "%d", page);
				strcat(combtempbuffer, pagebuffer);

				// combtempbuffer = grade + clan + class + page;

				strresourceid = atoi(combtempbuffer);
#endif

				_XMasteryNameTableInfo* newnameinfo = new _XMasteryNameTableInfo;
				newnameinfo->index = index;
				newnameinfo->strid = strresourceid;

				m_vecMasteryNameId.push_back(newnameinfo);

				// Lookup table ÀúÀå
				_XMasteryLookupTableInfo* newlookupinfo = new _XMasteryLookupTableInfo;
				newlookupinfo->index = index;
				newlookupinfo->clan = clan;
				newlookupinfo->cClass = cClass;
				newlookupinfo->grade = grade;
				newlookupinfo->page = page;

				m_vecMasteryLookupTable.push_back(newlookupinfo);
			}
			else
			{
				// table info
				_XMasteryDisplayTableInfo* newinfo = new _XMasteryDisplayTableInfo;
				sscanf(buffer, "%d %d", &newinfo->skillid, &newinfo->needprev);

				m_vecMasteryDisplayTable.push_back(newinfo);
				index++;
			}
		}

	} while(!feof(fileptr));

	g_ScriptArchive.ClosePackage();
	return TRUE;
}

int _XSkillMastery::GetMasteryDisplayTableIndex(int clan, int cClass, int grade, int page)
{
	_vec_MasteryLookupTableInfo::iterator iter_table = m_vecMasteryLookupTable.begin();
	for( ; iter_table != m_vecMasteryLookupTable.end() ; ++iter_table)
	{
		_XMasteryLookupTableInfo* currentinfo = *iter_table;
		if(currentinfo)
		{
			if(currentinfo->clan == clan && 
				currentinfo->cClass == cClass &&
				currentinfo->grade == grade &&
				currentinfo->page == page)
			{
				return currentinfo->index;
			}
		}
	}

	return -1;
}

int _XSkillMastery::GetMasteryNameId(int index)
{
	_vec_MasteryName::iterator iter_name = m_vecMasteryNameId.begin();
	for( ; iter_name != m_vecMasteryNameId.end() ; ++iter_name)
	{
		_XMasteryNameTableInfo* currentinfo = *iter_name;
		if(currentinfo)
		{
			if(currentinfo->index == index)
			{
				return currentinfo->strid;
			}
		}
	}

	return 0;
}

int _XSkillMastery::GetSkillIDFromMasteryDisplayTable(int index)
{
	_XMasteryDisplayTableInfo* tableinfo = m_vecMasteryDisplayTable[index];

	if(tableinfo)
	{
		return tableinfo->skillid;
	}
	
	return 0;
}

int _XSkillMastery::GetNeedprevFromMasteryDisplayTable(int index)
{
	_XMasteryDisplayTableInfo* tableinfo = m_vecMasteryDisplayTable[index];

	if(tableinfo)
	{
		return tableinfo->needprev;
	}
	
	return 0;
}