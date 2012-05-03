// XWindow_PartyMasterMiniBar.h: interface for the _XWindow_PartyMasterMiniBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PARTYMASTERMINIBAR_H__89928846_58AC_4E9C_ADBC_4CF71B096E46__INCLUDED_)
#define AFX_XWINDOW_PARTYMASTERMINIBAR_H__89928846_58AC_4E9C_ADBC_4CF71B096E46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum _XPartyMasterMode
{
	_XPARTYMASTERMODE_MAXIMUM		= 0,
		_XPARTYMASTERMODE_MINIMUM
		
}_XPARTYMASTER_MODE;

class _XWindow_PartyMasterMiniBar : public _XWindow  
{
public:	
	
	_XImageStatic*		m_imagePartyMasterBorder;				// 동행 행수창 바탕 이미지 
	_XImageStatic*		m_imagePartyMasterMinimizeBorder;		// 동행 행수창 축소시 바탕 이미지 
	_XImageStatic*		m_pPartyMasterDanger[3];				// 위험 표시 이미지 
	
	_XStickGauge*		m_pPartyMasterLifePowerGauge;			// 동행 행수창 체력 게이지 
	_XStickGauge*		m_pPartyMasterForcePowerGauge;			// 동행 행수창 진기 게이지 
	
	_XButton*			m_btnPartyMasterMinimize;				// 동행 행수창 축소 버튼 
	_XButton*			m_btnPartyMasterMaximize;				// 동행 행수창 확대 버튼 
	_XButton*			m_btnPartyMasterClose;					// 동행 행수창 닫기 버튼 
	
	_XButton*			m_btnPartyExperienceSharing;			// 동행 행수창 경험치 분배 버튼 
	_XButton*			m_btnPartyItemSharing;					// 동행 행수창 아이템 분배 버튼 
	
	_XListBox*			m_listboxPartyExperienceSharing;
	_XListBox*			m_listboxPartyItemSharing;

	TCHAR				m_PartyMasterName[_XDEF_MAX_USERNAMESTRINGLENGTH];
	
	DWORD				m_CloseButtoncommandid;	
	DWORD				m_PrevDrawTime;
	
	FLOAT				m_EffectFrame;
	
	int					m_indexPartyExperienceSharing;	
	int					m_indexPartyItemSharing;
	int					m_ZoneIndex;	
	int					m_PartyIndex;

	unsigned short		m_PlayerUniqueid;
	
	BOOL				m_bDangerous;							// 위험한 상태일때 
	BOOL				m_bIsCurrentZone;
	BOOL				m_EffectOrderSub;
	BOOL				m_bDeathUser;
	BOOL				m_bIsCurrentUser;

	_XPARTYMASTER_MODE	m_PartyMasterMode;
	
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
	_XWindow_PartyMasterMiniBar();
	virtual ~_XWindow_PartyMasterMiniBar();
	
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void MoveWindow( int X, int Y );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void DrawDangerMode(void);	
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void ProcessDblClick();
	
	void SetPartyMasterName(int partyindex, BOOL& isCurrentUser);
	void SetPartyMasterVital(int partyindex, BOOL& isCurrentUser);
	void SetMode( _XPARTYMASTER_MODE partymastermode );
	
	void SetZoneIndex(int zoneindex);
	
	BOOL CheckMousePositionExperienceSharingBox(void);
	BOOL CheckMousePositionItemSharingBox(void);
	void SetPartyUserStateImage(void);
	
	void SetOut(bool state);
	void SetFarAway(bool state);
	void Reset(void);

	void SetClassIcon( void );
};

#endif // !defined(AFX_XWINDOW_PARTYMASTERMINIBAR_H__89928846_58AC_4E9C_ADBC_4CF71B096E46__INCLUDED_)
