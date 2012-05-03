// XWindow_TradeItemTooltip.h: interface for the _XWindow_TradeItemTooltip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TRADEITEMTOOLTIP_H__E8AC5AB9_2ED4_45EC_830D_83BB4F457F6E__INCLUDED_)
#define AFX_XWINDOW_TRADEITEMTOOLTIP_H__E8AC5AB9_2ED4_45EC_830D_83BB4F457F6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

// 2004.05.18->oneway48 modify
class _XWindow_TradeItemTooltip : public _XWindow  
{
public :
	
	_XWindow*			m_ParentWindow;
	int					m_HoveredSlotIndex;
	
	char			m_Category1;
	short			m_Category3;
	/*
	TCHAR			m_DescNameString[16];
	TCHAR			m_DescClassString[16];
	TCHAR			m_DescPowerString[16];
	TCHAR			m_DescSuccessRateString[16];
	TCHAR			m_DescWeightString[16];
	TCHAR			m_DescRequireString[16];
	TCHAR			m_DescEffectString[16];
	
	  FLOAT			m_RotateAngle;
	  D3DXMATRIX		m_RHWMatrix;
	  _X3PCamera		m_ModelViewCamera;
	  D3DVIEWPORT9	m_ModelViewPort;
	  
		_XImageStatic	m_ItemBorder;
		
		  DWORD			m_ShowSignalTime;
	*/
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
	_XWindow_TradeItemTooltip();
	virtual ~_XWindow_TradeItemTooltip();
	
	BOOL			Initailize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			ShowWindow(BOOL show);
	void			MoveWindow(int X, int Y);
	
	//	void			RenderItem(void);
};

#endif // !defined(AFX_XWINDOW_TRADEITEMTOOLTIP_H__E8AC5AB9_2ED4_45EC_830D_83BB4F457F6E__INCLUDED_)
