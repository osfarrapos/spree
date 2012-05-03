// XMatchEventBaseType.h
//
//////////////////////////////////////////////////////////////////////

#ifndef _XMATCHEVENTBASETYPE_H_
#define _XMATCHEVENTBASETYPE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>

enum _en_ConstraintType
{
	_xen_const_hyperrun = 1,
	_xen_const_breath,
	_xen_const_logout,
	_xen_const_escape,
	_xen_const_party,
	_xen_const_trade,
	_xen_const_info,
	_xen_const_recall,
	_xen_const_shop,
	_xen_const_hideme,
	_xen_const_useskill,
	_xen_const_useitem,
	_xen_const_pvp_pr	// 비무 홍보
};

// _xen_const_hyperrun ~ _xen_const_hideme
struct _s_Action_Constraint
{
	int		nOnOff;		// on:0  off:1
};

typedef vector<int> _v_Skill_Str;

struct _s_Skill_Constraint
{
	_v_Skill_Str	vSkills;
};

struct _s_Item_Str 
{
	int		cType;
	int		sID;
};

typedef vector<_s_Item_Str*> _v_Item_Str;

struct _s_Item_Constraint
{
	_v_Item_Str vItems;
};

struct _s_Match_Notice
{
	int		nTime;			// 몇시에?
	int		nToType;		// 누구에게?  0:all  1:winner
	int		nCondition;		// 0:1인승리일때  1:무승부일때
	int		nTextID;
	int		nParam;
};

typedef vector<_s_Match_Notice*> _v_Match_Notice;

struct _s_Match_State
{
	int				nWhen;			// 0:ready  1:start  2:end
	_v_Match_Notice	vMatchNotice;
};

typedef map<int, _s_Match_State*> _map_Match_State;	//<ready/start/end, _s_Match_State>

// 궁극적으로 manager가 가지고 있는 structure
struct _s_Match_Type
{
	int				nMatchType;

	map <int, _s_Action_Constraint*>	mapActionConstraints;	//<_en_ContraintType, _s_Atcion_Const*>

	_v_Skill_Str	vSkillConstraints;
	_v_Item_Str		vItemConstraints;

	_map_Match_State	mapMatchState;
};

// 현재 비무 대회 정보
struct _S_Current_Match_Info
{
	int				nType;
	int				nAreaIndex;

	int				nMinLevel;
	int				nMaxLevel;

	int				nRewardIndex;

	_XTimeUnit		sStartTime;
	_XTimeUnit		sEndTime;
};

#endif