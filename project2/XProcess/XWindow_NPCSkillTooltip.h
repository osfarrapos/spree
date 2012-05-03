// XWindow_NPCSkillTooltip.h: interface for the _XWindow_NPCSkillTooltip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCSKILLTOOLTIP_H__EFC7E0A8_7EC6_4063_AA9E_E3F553E3A81D__INCLUDED_)
#define AFX_XWINDOW_NPCSKILLTOOLTIP_H__EFC7E0A8_7EC6_4063_AA9E_E3F553E3A81D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_NPCSkillTooltip : public _XWindow  
{
protected :
	_XImageStatic*		m_SkillIcon_Default;
	_XImageStatic*		m_SkillIcon_NangIn;
	_XImageStatic*		m_SkillIcon_GaeBang;
	_XImageStatic*		m_SkillIcon_BeeGoong;
	_XImageStatic*		m_SkillIcon_SoRim;
	_XImageStatic*		m_SkillIcon_NockRim;
	
	_XWindow*			m_ParentWindow;
	
	short				m_SkillID;
	
	RECT				m_IconRect;
	
	TCHAR				m_ClanTypeString[16];
	TCHAR				m_SkillSTypeString[16];
	TCHAR				m_WeaponCTypeString[16];
	TCHAR				m_TargetTypeString[16];
	TCHAR				m_UsefulTypeString[16];
	
	TCHAR				m_FERAString[16];
	TCHAR				m_CombinationString[16];
	TCHAR				m_InsideTypeString[16];
	
	TCHAR				m_AttackDamageString[16];
	TCHAR				m_AttackRatingString[16];
	TCHAR				m_AvoidRatingString[16];
	
	TCHAR				m_CostForceString[16];
	TCHAR				m_CostMindString[16];
	TCHAR				m_CostManaString[16];
	TCHAR				m_CostEnergyString[16];
	
	TCHAR				m_TechniqueExpString[32];
	TCHAR				m_ReqMissionString[16];
	TCHAR				m_ObjectIDString[16];
	TCHAR				m_AddBonusString[16];
	TCHAR				m_NumberString[16];
	
	TCHAR				m_Effect1String[16];
	TCHAR				m_Effect2String[16];
	TCHAR				m_Effect3String[16];
	TCHAR				m_Effect4String[16];
	
	DWORD				m_ShowSignalTime;
	
public:
	_XWindow_NPCSkillTooltip();
	virtual ~_XWindow_NPCSkillTooltip();
	
	BOOL	Initialize(void);
	void	DestroyWindow(void);
	
	void	Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL	Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void	SetWindowPosition(void);
	void	SetSkillID(short skillid);
	
	void	CheckOutside(void);
	void	GenerateIconRect(void);
	
	void	ShowWindow(BOOL show);
	void	DrawSkillInfo(void);
	
	void	SetParentWindow(_XWindow* parentwindow)		{	m_ParentWindow = parentwindow;		}	
	void	SetShowSignalTime(DWORD time)				{	m_ShowSignalTime = time;			}
};
#endif // !defined(AFX_XWINDOW_NPCSKILLTOOLTIP_H__EFC7E0A8_7EC6_4063_AA9E_E3F553E3A81D__INCLUDED_)
