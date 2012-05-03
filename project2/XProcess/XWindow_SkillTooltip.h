// XWindow_SkillTooltip.h: interface for the _XWindow_SkillTooltip class.
//
// Created Date	: 2003.03.03
// Author		: Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_SKILLTOOLTIP_H__BD6FC853_D6E8_484B_874B_BFB6CDF30F14__INCLUDED_)
#define AFX_XWINDOW_SKILLTOOLTIP_H__BD6FC853_D6E8_484B_874B_BFB6CDF30F14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_SkillTooltip : public _XWindow  
{/*
 protected :
	 _XImageStatic*		m_SkillIcon_Default;
	 _XImageStatic*		m_SkillIcon_NangIn;
	 _XImageStatic*		m_SkillIcon_GaeBang;
	 _XImageStatic*		m_SkillIcon_BeeGoong;
	 _XImageStatic*		m_SkillIcon_SoRim;
	 _XImageStatic*		m_SkillIcon_NockRim;
	
	  _XImageStatic*		m_SkillIcon_Default_g;
	  _XImageStatic*		m_SkillIcon_NangIn_g;
	  _XImageStatic*		m_SkillIcon_GaeBang_g;
	  _XImageStatic*		m_SkillIcon_BeeGoong_g;
	  _XImageStatic*		m_SkillIcon_SoRim_g;
	  _XImageStatic*		m_SkillIcon_NockRim_g;
	  
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
						  
							_XStickGauge*		m_pNeedSkillExpGuage;	// 2004.05.21->oneway48 insert
							public:
							_XWindow_SkillTooltip();
							virtual ~_XWindow_SkillTooltip();
							
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
										
	int		GetSkillId(void)							{	return m_SkillID;					}*/
};

#endif // !defined(AFX_XWINDOW_SKILLTOOLTIP_H__BD6FC853_D6E8_484B_874B_BFB6CDF30F14__INCLUDED_)
