// XWindow_PartyMemberMiniBar.h: interface for the _XWindow_PartyMemberMiniBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PARTYMEMBERMINIBAR_H__ECE15E5D_595E_4D5E_8517_453FCAAD4413__INCLUDED_)
#define AFX_XWINDOW_PARTYMEMBERMINIBAR_H__ECE15E5D_595E_4D5E_8517_453FCAAD4413__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_PartyMemberMiniBar : public _XWindow  
{
public:	
	
	_XImageStatic*		m_imagePartyMemberBorder;				// 동행 멤버창 바탕 이미지 
	_XImageStatic*		m_pPartyMemberDanger[3];				// 위험 표시 이미지 
	
	_XStickGauge*		m_pPartyMemberStaminaGauge;
	_XStickGauge*		m_pPartyMemberLifePowerGauge;			// 동행 멤버창 체력 게이지 
	_XStickGauge*		m_pPartyMemberForcePowerGauge;			// 동행 멤버창 진기 게이지 

	
	_XButton*			m_btnPartyMemberClose;					// 동행 행수창 닫기 버튼 
	_XButton*			m_btnPartyMasterEntrust;				// 동행 위임 버튼 
	_XButton*			m_btnClose;

	
	DWORD				m_CloseButtoncommandid;	
	DWORD				m_EntrustButtoncommandid;
	DWORD				m_PrevDrawTime;	
	
	FLOAT				m_EffectFrame;
	
	TCHAR				m_PartyMemberName[_XDEF_MAX_USERNAMESTRINGLENGTH];
	
	int					m_PartyIndex;
	int					m_ZoneIndex;

	
	unsigned short		m_PlayerUniqueid;	
	
	BOOL				m_bIsCurrentUser;
	BOOL				m_bDeathUser;
	BOOL				m_bIsCurrentZone;
	BOOL				m_EffectOrderSub;
	BOOL				m_bDangerous;							// 위험한 상태일때 
	
	_XImageStatic*		m_pFarActionIcon;						// 동행원 행동 표시 - 거리이탈
	_XImageStatic*		m_pOutActionIcon;						// 동행원 행동 표시 - 자리비움
		
	bool				m_bOut;
	bool				m_bFarAway;

	_XImageStatic*		m_pClassBorderIcon;						// 직책 아이콘 보더
	_XImageStatic*		m_pClassIcon;							// 직책 아이콘	
	
#ifdef _XTS_TARGET_STATE
	_XImageStatic		m_StateImage[32];
	int					m_StateCount;
#endif
public:
	_XWindow_PartyMemberMiniBar();
	virtual ~_XWindow_PartyMemberMiniBar();
	
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void MoveWindow( int X, int Y );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void DrawDangerMode(void);	
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void ProcessDblClick();

	void SetPartyPlayerName(int partyindex, BOOL& isCurrentUser);
	void SetPartyPlayerVital(int partyindex, BOOL& isCurrentUser);
	
	void SetZoneIndex(int zoneindex);
	void SetPartyUserStateImage(void);
	
	void SetOut(bool state);
	void SetFarAway(bool state);

	void Reset(void);

	void SetClassIcon( void );
};

#endif // !defined(AFX_XWINDOW_PARTYMEMBERMINIBAR_H__ECE15E5D_595E_4D5E_8517_453FCAAD4413__INCLUDED_)
