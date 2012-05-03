// XWindow_Party.h: interface for the _XWindow_Party class.
//
// Modified Date	: 2003.02.26
// Modified by		: Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_PARTY_H_
#define _XWINDOW_PARTY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum
{
	_XPARTYTAB_BANGHOI = 0,
		_XPARTYTAB_SEGA,
		_XPARTYTAB_FRIEND,
		_XPARTYTAB_PARTY,
		_XPARTYTAB_TOTALCOUNT
}_XPartyWindowTab;

#define _XDEF_MAXVIEWPARTYLIST		9 //2004.06.02->oneway48 modify 15->9 

class _XWindow_Party : public _XWindow  
{
public:
		_XCheckButton*		m_PartyTab[ _XPARTYTAB_TOTALCOUNT ];
		
		_XListBox*			m_PartyList;
		
		// 파티 마스터 윈도우
		_XImageStatic*		m_PartyMasterBorder;										// 파티 마스터 윈도우 외곽선
		_XImageStatic*		m_PartyMasterMortalBlowStatusImage;							// 파티 마스터 협필살기 상태 아이콘
		_XCheckButton*		m_PartyMasterMiniInfoButton;								// 파티 마스터 미니정보창 버튼
		_XStickGauge*		m_PartyMasterVitalityGauge;									// 파티 마스터 체력
		_XStickGauge*		m_PartyMasterZenBorderGauge;								// 파티 마스터 진기
		_XButton*			m_PartyLeaveButton;											// 파티 탈퇴 버튼
		_XButton*			m_PartyDissolutionButton;									// 파티 해체 버튼
		
		_XStickGauge*		m_PartyUserVitalityGauge[_XDEF_MAXVIEWPARTYLIST];			// 파티 유저 체력
		_XStickGauge*		m_PartyUserZenGauge[_XDEF_MAXVIEWPARTYLIST];				// 파티 유저 진기
		_XImageStatic*		m_PartyUserMortalBlowStatusImage[_XDEF_MAXVIEWPARTYLIST];	// 파티 유저 필살기 상태 아이콘
		_XCheckButton*		m_PartyUserMiniInfoButton[_XDEF_MAXVIEWPARTYLIST];			// 파티 유저 미니정보창 버튼
		_XButton*			m_PartyUserSecessionButton[_XDEF_MAXVIEWPARTYLIST];			// 파티 유저 이임 버튼
		_XButton*			m_PartyUserLeaveButton[_XDEF_MAXVIEWPARTYLIST];				// 파티 마스터 추방 버튼
		
		int					m_SelectedPartyMember;
		
protected :
	_XPartyWindowTab	m_SelectedTab;
	BOOL				m_ChangeTab;
	
public:
	_XWindow_Party();
	virtual ~_XWindow_Party();
	
	BOOL	Initialize( void );	
	void	DestroyWindow(void);
	
	void	MoveWindow( int X, int Y );	
	void	SetDefaultPosition( void );
	
	void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void	ChangeTab(_XPartyWindowTab selectedtab);
	
	void	SetBanghoiList(void);
	void	SetSegaList(void);
	void	SetPartyList(void);
	void	SetFriendList(void);
	
	void	SetUserButtonEnable( void );
	void	ShowWindow(BOOL show);
};

#endif // !defined(_XWINDOW_PARTY_H_)