// XSkillMastery.h: interface for the _XSkillMastery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSKILLMASTERY_H__6B2A617D_1418_4CC3_85D6_0D27D40286A4__INCLUDED_)
#define AFX_XSKILLMASTERY_H__6B2A617D_1418_4CC3_85D6_0D27D40286A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#pragma warning (disable : 4786)
#include <vector>
using namespace std;

class _XWindow_MasterySkill;

// ======================================================
// 
// 2차 전직 오의 페이지 확대를 위해 structure 변경
//
// _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI 이 define 되어있을 경우, 
// 기존의 g_SkillMasteryDisplayProperty_xx_xx 사용하지 않고 아래 class 사용한다.
//
// MasteryDisplayTable은 스크립트를 순서대로 읽어서 저장하고, 
// Lookup table을 참조하여 원하는 정보 얻어가도록 한다.
//
// ======================================================= 

struct _XMasteryDisplayTableInfo 
{
	int skillid;
	int needprev;

	_XMasteryDisplayTableInfo() : skillid(0), needprev(0)
	{
	}

};

struct _XMasteryLookupTableInfo
{
	int	index;	// SkillMasteryDisplayProperty의 index
	int clan;
	int cClass;
	int grade;
	int	page;

	_XMasteryLookupTableInfo() : index(0), clan(0), cClass(0), grade(0), page(0)
	{
	}

};

struct _XMasteryNameTableInfo
{
	int index;
	int strid;

	_XMasteryNameTableInfo() : index(0), strid(0)
	{
	}
};

typedef vector <_XMasteryDisplayTableInfo*> _vec_MasteryDisplayTableInfo;
typedef vector <_XMasteryNameTableInfo*> _vec_MasteryName;
typedef vector <_XMasteryLookupTableInfo*> _vec_MasteryLookupTableInfo;

class _XSkillMastery  
{

private :
	_vec_MasteryDisplayTableInfo	m_vecMasteryDisplayTable;
	_vec_MasteryName				m_vecMasteryNameId;
	_vec_MasteryLookupTableInfo		m_vecMasteryLookupTable;

public:
	_XSkillMastery();
	virtual ~_XSkillMastery();

	BOOL	LoadSkillMasteryDisplayTable();

	int		GetMasteryDisplayTableIndex(int clan, int cClass, int grade, int page);
	int		GetMasteryNameId(int index);

	int		GetSkillIDFromMasteryDisplayTable(int index);
	int		GetNeedprevFromMasteryDisplayTable(int index);
};

#endif // !defined(AFX_XSKILLMASTERY_H__6B2A617D_1418_4CC3_85D6_0D27D40286A4__INCLUDED_)
