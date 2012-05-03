// XFunctionalObjectTriggerItem.cpp: implementation of the _XFunctionalObjectTriggerItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XFunctionalObjectTriggerItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XFunctionalObjectTriggerItem::_XFunctionalObjectTriggerItem()
{
	m_lpszTriggerName	= NULL;
	m_bEnable			= FALSE;	
	m_byCompareClass	= 0;	
	m_TriggerID		=0;	
	
	m_ProgressFunctionalObjectTrigger.condition = 0;
	m_ProgressFunctionalObjectTrigger.normalaction = 0;
	m_ProgressFunctionalObjectTrigger.exceptedaction = 0;	

}

_XFunctionalObjectTriggerItem::~_XFunctionalObjectTriggerItem()
{
	
	SAFE_DELETE_ARRAY(m_lpszTriggerName);
	
	map <int, _XFunctionalObjectConditionItem*>::iterator iter_condition;
	_XFunctionalObjectConditionItem* currentcondition = NULL;
	for(iter_condition = m_mapConditionList.begin() ; iter_condition != m_mapConditionList.end() ; ++iter_condition)
	{
		currentcondition = iter_condition->second;
		if(currentcondition)
		{
			delete currentcondition;
			currentcondition = NULL;
		}
	}
	m_mapConditionList.clear();
	
	map <int, _XFunctionalObjectActionItem*>::iterator iter_action;
	_XFunctionalObjectActionItem* currentaction = NULL;
	for(iter_action = m_mapNormalActionList.begin() ; iter_action != m_mapNormalActionList.end() ; ++iter_action)
	{
		currentaction = iter_action->second;
		if(currentaction)
		{
			delete currentaction;
			currentaction = NULL;
		}
	}
	m_mapNormalActionList.clear();
	
	currentaction = NULL;
	for(iter_action = m_mapExceptedActionList.begin() ; iter_action != m_mapExceptedActionList.end() ; ++iter_action)
	{
		currentaction = iter_action->second;
		if(currentaction)
		{
			delete currentaction;
			currentaction = NULL;
		}
	}
	m_mapExceptedActionList.clear();
}

void _XFunctionalObjectTriggerItem::SetTriggerName(LPCTSTR triggername)
{
	if(m_lpszTriggerName)
	{
		delete[] m_lpszTriggerName;
		m_lpszTriggerName = NULL;
	}

	if(triggername == NULL)
		return;

	int strlength = strlen(triggername);
	if(strlength > 0)
	{
		m_lpszTriggerName = new TCHAR[strlength + 1];
		m_lpszTriggerName[strlength] = 0;
		strcpy(m_lpszTriggerName, triggername);
	}
}
BOOL _XFunctionalObjectTriggerItem::LoadString(FILE* fileptr, LPTSTR& string)
{
	int strlength = 0;
	if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	
	if(string)
	{
		delete[] string;
		string = NULL;
	}
	
	if(strlength > 0)
	{
		string = new TCHAR[strlength + 1];
		if(!string)
			return FALSE;
		
		memset(string, 0, sizeof(TCHAR)*(strlength + 1));
		if(fread(string, strlength, 1, fileptr) < 1)
			return FALSE;
	}
	return TRUE;
}

BOOL _XFunctionalObjectTriggerItem::Load(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;

	m_bEnable = TRUE;
	
	map <int, _XFunctionalObjectConditionItem*>::iterator iter_condition;
	_XFunctionalObjectConditionItem* currentcondition = NULL;
	for(iter_condition = m_mapConditionList.begin() ; iter_condition != m_mapConditionList.end() ; ++iter_condition)
	{
		currentcondition = iter_condition->second;
		if(currentcondition)
			delete currentcondition;
	}
	m_mapConditionList.clear();
	
	map <int, _XFunctionalObjectActionItem*>::iterator iter_normalaction;
	_XFunctionalObjectActionItem* currentaction = NULL;
	for(iter_normalaction = m_mapNormalActionList.begin() ; iter_normalaction != m_mapNormalActionList.end() ; ++iter_normalaction)
	{
		currentaction = iter_normalaction->second;
		if(currentaction)
			delete currentaction;
	}
	m_mapNormalActionList.clear();
	
	map <int, _XFunctionalObjectActionItem*>::iterator iter_exceptedaction;
	currentaction = NULL;
	for(iter_exceptedaction = m_mapExceptedActionList.begin() ; iter_exceptedaction != m_mapExceptedActionList.end() ; ++iter_exceptedaction)
	{
		currentaction = iter_exceptedaction->second;
		if(currentaction)
			delete currentaction;
	}
	m_mapExceptedActionList.clear();

	if(fread(&m_TriggerID, sizeof(int), 1, fileptr) < 1) 
		return FALSE;
	if(!LoadString(fileptr, m_lpszTriggerName))	
		return FALSE;
	if(fread(&m_bEnable, sizeof(BOOL), 1, fileptr) < 1)	
		return FALSE;
	if(fread(&m_byCompareClass, sizeof(BYTE), 1, fileptr) < 1) 
		return FALSE;

	int itemcount = 0;
	if(fread(&itemcount, sizeof(int), 1, fileptr) < 1)	
		return FALSE;
	int position = 0;
	if(fread(&position, sizeof(int), 1, fileptr) < 1)	
		return FALSE;

	if(itemcount > 0)
	{
		_XFunctionalObjectConditionItem* newconditionitem;

		for(int i = 0 ; i < itemcount; ++i)
		{
			newconditionitem = new _XFunctionalObjectConditionItem;
			if(newconditionitem->Load(fileptr) == FALSE)
			{
				delete newconditionitem;
				return FALSE;
			}
			m_mapConditionList[ newconditionitem->m_ConditionID ] = newconditionitem;
		}
	}

	itemcount = 0;
	if(fread(&itemcount, sizeof(int), 1, fileptr) < 1)	
		return FALSE;

	if(itemcount > 0)
	{
		_XFunctionalObjectActionItem* newnormalactionitem;

		for(int i = 0 ; i < itemcount ; ++i)
		{
			newnormalactionitem = new _XFunctionalObjectActionItem;
			if(newnormalactionitem->Load(fileptr) == FALSE)
			{
				delete newnormalactionitem;
				return FALSE;
			}
			m_mapNormalActionList[ newnormalactionitem->m_ActionID ] = newnormalactionitem;
		}
	}
	
	itemcount = 0;
	if(fread(&itemcount, sizeof(int), 1, fileptr) < 1)	
		return FALSE;
	
	if(itemcount > 0)
	{
		_XFunctionalObjectActionItem* exceptedactionitem;
		
		for(int i = 0 ; i < itemcount ; ++i)
		{
			exceptedactionitem = new _XFunctionalObjectActionItem;
			if(exceptedactionitem->Load(fileptr) == FALSE)
			{
				delete exceptedactionitem;
				return FALSE;
			}
			m_mapExceptedActionList[ exceptedactionitem->m_ActionID ] = exceptedactionitem;
		}
	}
	

		
	return TRUE;
}

void _XFunctionalObjectTriggerItem::SetProgressCondition( int conditionid)
{
	DWORD progress = 0x80000000 >> (conditionid);
	m_ProgressFunctionalObjectTrigger.condition |= progress;
}
void _XFunctionalObjectTriggerItem::SetProgressNormalAction( int actionid)
{
	DWORD progress = 0x80000000 >> (actionid);
	m_ProgressFunctionalObjectTrigger.normalaction |= progress;
}
void _XFunctionalObjectTriggerItem::SetProgressExceptedAction( int actionid)
{
	DWORD progress = 0x80000000 >> (actionid);
	m_ProgressFunctionalObjectTrigger.exceptedaction |= progress;
}

DWORD _XFunctionalObjectTriggerItem::GetCompareValue(int comparecount)
{
	switch(comparecount)
	{
	case 1 :
		return 0x80000000;
	case 2 :
		return 0xC0000000;
	case 3 :
		return 0xE0000000;
	case 4 :
		return 0xF0000000;
	case 5 :
		return 0xF8000000;
	case 6 :
		return 0xFC000000;
	case 7 :
		return 0xFE000000;
	case 8 :
		return 0xFF000000;
	case 9 :
		return 0xFF800000;
	case 10 :
		return 0xFFC00000;
	case 11 :
		return 0xFFE00000;
	case 12 :
		return 0xFFF00000;
	case 13 :
		return 0xFFF80000;
	case 14 :
		return 0xFFFC0000;
	case 15 :
		return 0xFFFE0000;
	case 16 :
		return 0xFFFF0000;
	case 17 :
		return 0xFFFF8000;
	case 18 :
		return 0xFFFFC000;
	case 19 :
		return 0xFFFFE000;
	case 20 :
		return 0xFFFFF000;
	case 21 : 
		return 0xFFFFF800;
	case 22 : 
		return 0xFFFFFC00;
	case 23 : 
		return 0xFFFFFE00;
	case 24 :
		return 0xFFFFFF00;
	case 25 : 
		return 0xFFFFFF80;
	case 26 :
		return 0xFFFFFFC0;
	case 27 :
		return 0xFFFFFFE0;
	case 28 :
		return 0xFFFFFFF0;
	case 29 :
		return 0xFFFFFFF8;
	case 30 :
		return 0xFFFFFFFC;
	case 31 :
		return 0xFFFFFFFE;
	case 32 :
		return 0xFFFFFFFF;
	default :
		return 0x00000000;
	}
}