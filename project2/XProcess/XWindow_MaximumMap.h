// XWindow_MaximumMap.h: interface for the _XWindow_MaximumMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MAXIMUMMAP_H__E47D060E_1445_46A3_9BED_ABF02AE0009C__INCLUDED_)
#define AFX_XWINDOW_MAXIMUMMAP_H__E47D060E_1445_46A3_9BED_ABF02AE0009C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_MaximumMap : public _XWindow  
{
public:
	_XImageStatic	m_MoonImage;
	_XImageStatic	m_LargeMinimapImage;
	int				m_CurrentServerIndex;
	
	_XStatic		m_MaximummapArrow;
	_XImageStatic	m_FrustumImage;
	_XImageStatic	m_PartyPlayerImage;
	_XImageStatic	m_NPCImageTradeType;
	_XImageStatic	m_NPCImageEventType;
	
	_XCheckButton*	m_pNPCViewButton;
	_XCheckButton*	m_pPartyViewButton;

	_XImageStatic	m_YellowImpressionMark;
	_XImageStatic	m_GreenImpressionMark;
	_XImageStatic	m_BlueImpressionMark;
	_XImageStatic	m_OrangeImpressionMark;
	
	_XImageStatic	m_YellowQuestionMark;
	_XImageStatic	m_GreenQuestionMark;
	_XImageStatic	m_BlueQuestionMark;
	_XImageStatic	m_OrangeQuestionMark;
	
public:
	_XWindow_MaximumMap();
	virtual ~_XWindow_MaximumMap();
	
	BOOL			Initialize( void );
	void			DestroyWindow( void );
	
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void			MoveWindow( int X, int Y );
	BOOL			SetMinimapImage( LPTSTR mapname );
	void			SaveWindowProperty( void );
	
	void			DrawNPCPosition(void);
	void			CheckCurrentUser(int partyindex, BOOL& isCurrentUser);
	void			DrawPartyPlayerPosition(void);
	void			SetLocalUserPosition(void);
	
	void SetCurrentZoneIndex( int serverindex ){	m_CurrentServerIndex = serverindex; }
};

#endif // !defined(AFX_XWINDOW_MAXIMUMMAP_H__E47D060E_1445_46A3_9BED_ABF02AE0009C__INCLUDED_)
