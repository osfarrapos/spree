// XFame.cpp: implementation of the _XFame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XFame.h"
#include "EODExtDef_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XFame::_XFame()
{
	m_Index			= 0;
	m_FameRange		= 0;
	m_InfamyRange	= 0;
	memset(m_FameName, 0, sizeof(TCHAR) * 256);
	memset(m_InfamyName, 0, sizeof(TCHAR) * 256);
}

_XFame::~_XFame()
{

}

int _XFame::GetFameIndex(int value)
{
	for(int i = g_FameStrLength-1 ; i >= 0 ; --i)
	{
		if(value >= g_FameStr[i].m_FameRange)
			return i;
	}

	return -1;
}

int _XFame::GetInfamyIndex(int value)
{
	for(int i = g_FameStrLength-1 ; i >= 0 ; --i)
	{
		if(value >= g_FameStr[i].m_InfamyRange)
			return i;
	}

	return -1;
}

BOOL _XFame::CheckItemFame(int index)
{
#ifdef _XTS_FAME
	if( index > 100 )	// 악명
	{
		if( g_pLocalUser->m_CharacterInfo.Get_Notoriety() > 0 )			
		{
#ifdef _XTS_ITEM_V22
			if(index == 199)
			{
				if(g_pLocalUser->m_CharacterInfo.Get_Notoriety() >= g_pLocalUser->m_CharacterInfo.Get_honor())	// 악명이 명성보다 높을 경우 가능
					return TRUE;
			}
			else
			{
#endif
				int step = g_FameStr->GetInfamyIndex(g_pLocalUser->m_CharacterInfo.Get_Notoriety());
				if( step >= index-101 )
					return TRUE;
#ifdef _XTS_ITEM_V22
			}
#endif
		}
	}
	else	// 명성
	{
		if( g_pLocalUser->m_CharacterInfo.Get_honor() > 0 )
		{
#ifdef _XTS_ITEM_V22
			if(index == 99)
			{
				if(g_pLocalUser->m_CharacterInfo.Get_Notoriety() < g_pLocalUser->m_CharacterInfo.Get_honor())	// 명성이 악명보다 높을 경우 가능
					return TRUE;
			}
			else
			{
#endif
				int step = g_FameStr->GetFameIndex(g_pLocalUser->m_CharacterInfo.Get_honor());
				if( step >= index-1 )
					return TRUE;
#ifdef _XTS_ITEM_V22
			}
#endif
		}
	}
#endif
	return FALSE;
}