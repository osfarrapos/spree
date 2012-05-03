// XTriggerTreeItem.cpp: implementation of the _XTriggerTreeItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XTriggerTreeItem.h"

#include "stdio.h"
#include "XKernel.h"
#include "XQuestScriptItem.h"
#include "EODEXTDEF_GlobalObject.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XTriggerTreeItem::_XTriggerTreeItem()
{	
	m_ParentsNodeUniqueID = -1;		
	m_SelfNodeUniqueID = -1;
	m_FLeftNodeUniqueID = -1;
	m_TRightNodeUniqueID = -1;
	m_bQuestBackAtCurrentNode = FALSE;
	m_bPartyResultToOneself	= FALSE;
	m_bLeftNode = FALSE;
	m_bAutoNode = false;
}

_XTriggerTreeItem::~_XTriggerTreeItem()
{

}

int	_XTriggerTreeItem::LoadString(FILE* fileptr, LPTSTR& string)
{
	int strlength = 0;
	
	if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		return -1;
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
	fseek( fileptr, sizeof(TCHAR)*strlength, SEEK_CUR );
	string = NULL;
#else	
	if(string != NULL)
	{
		delete[] string;
		string = NULL;
	}
	
	if(strlength > 0)
	{
		string = new TCHAR[strlength + 1];
		if(!string)
		{
			return -1;
		}
		
		memset(string, 0, sizeof(TCHAR)*(strlength + 1));
		if(fread(string, strlength, 1, fileptr) < 1)
			return -1;
	}
#endif
	return strlength;
}

BOOL _XTriggerTreeItem::Load(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;	
	
	if(fread(&m_ParentsNodeUniqueID, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_TriggerUniqueID, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_FLeftNodeUniqueID, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_TRightNodeUniqueID, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	
	if(fread(&m_SelfNodeUniqueID, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_TreeNodeNumber, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_tempSelfNodeUniqueID, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_tempParentsNodeUniqueID, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_tempFLeftNodeUniqueID, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_tempTRightNodeUniquID, sizeof(int), 1, fileptr) < 1)
		return FALSE;

#ifdef _XTS_QUESTAUTONODE
	bool tempvalue = false;
	if(fread(&tempvalue, sizeof(bool), 1, fileptr) < 1)
		return FALSE;
	if( tempvalue )	m_bLeftNode = TRUE;

	if(fread(&m_bAutoNode, sizeof(bool), 1, fileptr) < 1)
		return FALSE;
	if(fread(&tempvalue, sizeof(bool), 1, fileptr) < 1)
		return FALSE;
	if(fread(&tempvalue, sizeof(bool), 1, fileptr) < 1)
		return FALSE;
#else
	if(fread(&m_bLeftNode, sizeof(BOOL), 1, fileptr) < 1)
		return FALSE;
#endif

	if(fread(&m_bQuestBackAtCurrentNode, sizeof(BOOL), 1, fileptr) < 1)
		return FALSE;
	if(fread(&m_bPartyResultToOneself, sizeof(BOOL), 1, fileptr) < 1)
		return FALSE;
	
	return TRUE;
}

void _XTriggerTreeItem::SetOwnerQuestID(int questid)
{
	m_nOwnerQuestID = questid;
}

int _XTriggerTreeItem::GetOwnerQuestID(void)
{
	return m_nOwnerQuestID;
}
