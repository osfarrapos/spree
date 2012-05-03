// XWindow_ItemTooltip.h: interface for the _XWindow_ItemTooltip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_ITEMTOOLTIP_H__09FCD649_CC62_4623_9FE5_F5248B3434DC__INCLUDED_)
#define AFX_XWINDOW_ITEMTOOLTIP_H__09FCD649_CC62_4623_9FE5_F5248B3434DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

class _XWindow_ItemTooltip : public _XWindow  
{
public:
	_XWindow*			m_ParentWindow;
	int					m_HoveredSlotIndex;

	int					m_Category;
	int					m_ID;
	
	// Item Description String ====================
	TCHAR				m_DescNameString[16];				// 감정 이름
	
	TCHAR				m_DescGenderString[16];				// 성별 2004.06.11->oneway48
	TCHAR				m_DescPrice[16];					// 가격 2004.06.14->oneway48 insert
	
	TCHAR				m_DescClassString[16];				// 계열 -> 무기 분류 
	TCHAR				m_DescPowerString[16];				// 공격력
	TCHAR				m_DescSuccessRateString[16];		// 공격성공치  X
	TCHAR				m_DescWeightString[16];				// 무게 
	TCHAR				m_DescRequireString[16];			// 요구치 X
	TCHAR				m_DescDurabilityString[16];			// 내구성
	TCHAR				m_DescStrengthString[16];			// 강도 
	TCHAR				m_DescBalanceString[16];			// 균형
	
	TCHAR				m_DescDefenseString[16];			// 방어력
	TCHAR				m_DescWarmthString[16];				// 보온효과 
	TCHAR				m_DescResistanceString[16];			// 속성 저항력
	
	TCHAR				m_DescRollString[16];				// 역할
	TCHAR				m_DescEffectString[16];				// 효능 
	TCHAR				m_DescFeatureString[16];			// 특징
	
	TCHAR				m_DescHeartString[16];				// 심기
	TCHAR				m_DescStaminaString[16];			// 정기 
	TCHAR				m_DescLevelString[16];				// 내공수위
	
	TCHAR				m_DescRefine1String[16];			// 제련 속성 1
	TCHAR				m_DescRefine2String[16];			// 제련 속성 2
	
	TCHAR				m_DescAdditionalEffect1String[16];	// 부가 효과 1 
	TCHAR				m_DescAdditionalEffect2String[16];	// 부가 효과 2
	TCHAR				m_DescAdditionalEffect3String[16];	// 부가 효과 3
	
	TCHAR				m_DescSetEffect1String[16];			// 세트 효과 1 
	TCHAR				m_DescSetEffect2String[16];			// 세트 효과 2
	TCHAR				m_DescSetEffect3String[16];			// 세트 효과 3 
	TCHAR				m_DescSetEffect4String[16];			// 세트 효과 4
	TCHAR				m_DescSetEffect5String[16];			// 세트 효과 5 
	
public:
	_XWindow_ItemTooltip();
	virtual ~_XWindow_ItemTooltip();
	
	BOOL	Initialize(void);
	void	DestroyWindow(void);
	
	void	Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL	Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void	SetWindowPosition(void);
	void	CheckOutside(void);		
	void	ShowWindow(BOOL show);	
	void	SetParentWindow(_XWindow* parentwindow)		{	m_ParentWindow = parentwindow;		}
	
	void	MoveWindow(int X, int Y);
	
	// 2004.06.11->oneway48 insert
	BOOL CheckMuscularStrength(_XGI_FirstCategory type, int uniqid);
	BOOL CheckWisdom(_XGI_FirstCategory type, int uniqid);
	BOOL CheckLevel(_XGI_FirstCategory type, int uniqid);
};

#endif // !defined(AFX_XWINDOW_ITEMTOOLTIP_H__09FCD649_CC62_4623_9FE5_F5248B3434DC__INCLUDED_)
