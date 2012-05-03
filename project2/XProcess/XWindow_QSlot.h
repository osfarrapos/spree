// XWindow_QSlot.h: interface for the _XWindow_QSlot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_QSLOT_H__1B131250_02D8_448A_A0AB_491EA5F094A0__INCLUDED_)
#define AFX_XWINDOW_QSLOT_H__1B131250_02D8_448A_A0AB_491EA5F094A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum _XQSWindowStyle
{
	_XQS_WINSTYLE_HORIZ = 0,
	_XQS_WINSTYLE_VERTLEFT,
	_XQS_WINSTYLE_VERTRIGHT,
	_XQS_WINSTYLE_FLOATING
};

typedef struct 
{
	short		iconid;
	short		slotnumber; // 2004.06.10->oneway48 insert
	BOOL		used;
	BOOL		bskillicon;
}_XQSLOT_INFO;

/*
class _XQuickSlotSaveInfo : public _XItem
{
public :
	TCHAR				Owner[_XDEF_MAX_USERNAMESTRINGLENGTH];
	_XQSLOT_INFO		m_QuickSlot[27];
};

class _XQuickSlotInfoList : public _XList
{
public :

public :
	BOOL LoadQuickSlotInfo( void );
	BOOL SaveQuickSlotInfo( void );

	void UpdateQuickSlotInfo( void );
	void UpdateInfoList( void );
};
*/

class _XWindow_QSlot : public _XWindow  
{
public:
	_XImageStatic		m_HorizBorderStatic;
		
	_XQSWindowStyle		m_WindowStyle;
	_XQSWindowStyle		m_PrevWindowStyle;

	_XImageStatic*		m_SelectImage;

	LONG				m_SpeedCounter;	
	FLOAT				m_SelectedEffectFrame;
	int					m_SelectedEffectPrevFrame;
	BOOL				m_bSelectedCheck;
		
	POINT				m_ButtonPositionArray[10];

	_XButton*			typechangebutton;
	_XButton*			prevbutton;
	_XButton*			nextbutton;
	
	int					m_SelectedSetNumber;
	int					m_SelectedSlotNumber;
	
	_XQSLOT_INFO		m_QuickSlot[27]; // 2004.05.22->oneway48 modify
	
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
	
	_XImageStatic*		m_CooldownImage;
		
	short				m_SelectedSkillID;
	short				m_ActiveSlotID;
//	short				m_PrevSelectedSkillID;
	
	BOOL				m_bNotYetUse;

	//2004.06.23->oneway48 insert
	_XStickGauge*		m_pSkillExpGuage1;
	_XStickGauge*		m_pSkillExpGuage2;
	_XStickGauge*		m_pSkillExpGuage3;
	_XStickGauge*		m_pSkillExpGuage4;
	_XStickGauge*		m_pSkillExpGuage5;
	_XStickGauge*		m_pSkillExpGuage6;
	_XStickGauge*		m_pSkillExpGuage7;
	_XStickGauge*		m_pSkillExpGuage8;
	_XStickGauge*		m_pSkillExpGuage9;

	
	_XWindow_Inventory*		m_pInventory;

public:
	_XWindow_QSlot();
	virtual ~_XWindow_QSlot();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				InitializeQuickSlotInfo( void );

	void				MoveWindow( int X, int Y );	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void				DrawSlotButtons( void );

	_XQSWindowStyle		GetQSWindowStyle( void ){ return m_WindowStyle; }
	void				SetQSWindowStyle( _XQSWindowStyle windowstyle );
	void				SetQSWindowNextStyle( void );
	
	void				SetDefaultPosition( void );
	void				SaveWindowProperty( void );

	// =================================================
	// Quick slot icons....
	BOOL				CheckSlotNumber(BOOL bEmpty);
	BOOL				CheckUsedSlotNumber(int& index, short& iconid, bool& bskillicon );
	BOOL				CheckUsedKeySlotNumber(int index, short& iconid, bool& bskillicon );

	void				SetSelectedSkillIcon(int index, short iconid);
	
	void				SetSkillToQSlot(short iconid);
	void				SetItemToQSlot(short slotnumber); // 2004.06.10->oneway48 insert
	
	void				DrawSkillIcon(void);
	void				GenerateIconRect(short iconid, RECT& rect, BOOL& nextresource);// 2004.06.10->oneway48 insert
	void				DrawSelectedIconBorder(void);
	
	void				DeleteSelectedSlot(void);
	
	int					FindSkillFromQSlot(short iconid);	
	
	// =================================================
	// Selected skill icon...
	
	void				DrawSelectedSkillIcon(void);		
	BOOL				CheckSelectedIconClick(void);
	void				DeleteSelectedSkill(void);
	
	void				DrawSelectedSelectedIconBorder(void);
	
 	// ==================================================
	// Draw cooldown image
	void				DrawCooldownImage(void);
	
	void				GetInventoryPtr(void);
};


#endif // !defined(AFX_XWINDOW_QSLOT_H__1B131250_02D8_448A_A0AB_491EA5F094A0__INCLUDED_)
