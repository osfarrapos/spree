// XWindow_QuickSlot.h: interface for the _XWindow_QuickSlot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_QUICKSLOT_H__0E288073_66CA_44D2_98D1_E3BBA0C0DC2F__INCLUDED_)
#define AFX_XWINDOW_QUICKSLOT_H__0E288073_66CA_44D2_98D1_E3BBA0C0DC2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XProc_ProcessDefine.h"
#include "XGameItem.h"
#include "XUser.h"

class _XWindow_Inventory;

typedef enum _XQSlot_Type
{
	_XQS_SLOTTYPE_NONE = 0,
	_XQS_SLOTTYPE_SKILL,
	_XQS_SLOTTYPE_ITEM,
	_XQS_SLOTTYPE_LIFESKILL

};

struct _XQSlot_Info
{
	_XQSlot_Type	type;
	
	short			skillid;

	short			lifeskillid;

	char			itemCType;
	short			itemSID;
	int				invenNumber;

	bool			used;

	bool			cooldowndraw;
	bool			cooldownlastdraw;
	DWORD			prevdrawcooldowntime;
	FLOAT			cooldowneffectframe;

	BOOL			bChangeItem; //Author : 양희왕 //breif : 아이템 체인지 중
};

// ----------------------------------------------------------------------------
// Quick slot 저장 class
class _XQuickSlotSaveInfo : public _XItem
{
public :
	TCHAR				Owner[_XDEF_MAX_USERNAMESTRINGLENGTH];
	_XQSlot_Info		m_QuickSlot[30];
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

// ----------------------------------------------------------------------------
// Quick slot class
class _XWindow_QuickSlot : public _XWindow
{
private :

	_XImageStatic*			m_SelectedActiveImage;		// 선택된 슬롯 테두리
	_XImageStatic*			m_SelectedOtherImage;

	_XStickGauge*			m_SkillExpGauge[10];		// 스킬 경험치 게이지

	_XButton*				m_PrevSetButton;			// 이전 세트로
	_XButton*				m_NextSetButton;			// 다음 세트로

	_XImageStatic*			m_SlotNumberImage[10];		// 숫자
	
	POINT					m_PositionArray[10];

	_XButton*				m_PeaceModeButton;
	_XButton*				m_BattleModeButton;

	_XImageStatic*			m_BattleModeImage[5];

#ifdef _XTS_PK
	_XImageStatic*			m_PKModeImage[5];
#endif

	DWORD					m_PrevDrawTime;
	bool					m_EffectOrderSub;
	FLOAT					m_EffectFrame;

	int						m_SelectedSet;
	int						m_SelectedSlot;

	_XQSlot_Type			m_MouseSlotType;
	_XMouseSlotItem			m_MouseSlotItem;
	short					m_MouseSlotSkillID;
	bool					m_bHaveMouseSlot;
	int						m_OriginSlotIndex;

	bool					m_bDragState;
	POINT					m_IconDragStartPos;
	int						m_LastDraggingSlot;

	bool					m_bDrawSelectedIcon;

	_XStickGauge*			m_pSkillExpGauge;

	int						m_CurrentSelectedActiveIndex;
	int						m_CurrentSelectedOtherIndex;
	int						m_CurrentSelectedItemIndex;
	
	DWORD					m_CurrentSelectedItemStartTime;
	DWORD					m_CurrentSelectedActiveStartTime;
	DWORD					m_CurrentSelectedOtherStartTime;

	_XButton*				m_LeftLevelButton;
	_XButton*				m_RightLevelButton;
	bool					m_bDrawLevelButton;
	int						m_LevelButtonIndex;
	
	_XImageStatic			m_CooldownImage[125];
	
	_XImageStatic			m_PassiveOnImage[9];
	DWORD					m_PrevPassiveDrawTime;
	FLOAT					m_PassiveEffectFrame;

	int						m_CooldownIndex[30];
	int						m_DelayTimeIndex;
	
	_XImageStatic			m_GuideLineImage;
	BOOL					m_bChangeItem; //Author : 양희왕 //breif : 무기를 교체 중입니당

public :
	_XQSlot_Info			m_QuickSlot[30];

public:
	_XWindow_QuickSlot();
	virtual ~_XWindow_QuickSlot();

	BOOL					Initialize(void);
	void					DestroyWindow(void);

	void					Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL					Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void					ChangePeaceBattleMode(void);
	void					DrawBattleMode(void);

	void					SetDefaultPosition( void );

	void					DrawSkillIcon(short skillid, int X, int Y, bool bColor = true);

	void					ResetMouseSlot(void);
	bool					CopyMouseSlot(int draggingslot);
	void					ProcessIconMove(void);
	int						SearchSlotIndex(int X, int Y);

	void					DrawMouseSlotIcon(void);

	void					ResetQuickSlot(int index);
	void					ResetQuickSlotCurrentSet(void);
	
	void					DeleteQuickSlot(void);
	void					DeleteQuickSlot(short skillid);
	void					DeleteQuickSlot(u_char invennumber);
	
	void					DeleteSelectedSkill(void);
	void					DeleteQuickSlotFromMouse(void);

	void					RegistItem(void);
	void					RegistSkill(void);
	void					RegistSkillNextEmptySlot(short skillid);
	void					RegistLifeSkill(void);
	void					RegistLifeSkillNextEmptySlot(short skillid);
	BOOL					FindQuickSlotItem(_XQSlot_Type type, short id);

	void					DrawSkillExpGauge(void);
	void					DrawTooltip(void);
	void					DrawSkillLevel(void);

	_XQSlot_Type			GetQuickSlotType(int index);
	short					GetQuickSlotSkillID(int index);
	int						GetQuickSlotItemInvenNumber(int index);
	void					GetSlotPosition( int slotnumber, RECT& rect );

	void					CheckQuickSlotItem(void);
	void					CheckQuickSlotSkill(void);

	BOOL					GetIconRect(short skillid, int& resourceindex, RECT& rect, bool bColor = true, bool smallicon = false, bool bNPCSkill = false);
	BOOL					GetIconRectTrainingStateIcon(short skillid, int& resourceindex, RECT& rect, bool smallicon);

	void					SetCurrentSelectedActiveIndex(int index);
	void					SetCurrentSelectedOtherIndex(int index);

	void					SetCurrentSelectedActiveIndexFromSkillID(short skillid);
	void					SetCurrentSelectedOtherIndexFromSkillID(short skillid);

	void					DrawCooldownImage(void);
	void					DrawPassiveOnImage(void);
	void					DrawCooldownLastImage(void);
	
	void					ProcessCooldown(void);
	void					ProcessCooldownLastImage(void);

	void					ProcessDelayTime(void);
	void					DrawDelayTime(void);
	void					SetDrawGuideLine(BOOL flag);
	void					CheckTutorial(short skillid);
	
	inline int		GetSelectedSet(void)	{	return m_SelectedSet;	};
	inline void		SetSelectedSet(int number)	{	m_SelectedSet = number;	};
	
	inline bool		GetDrawSelectedIcon(void)	{	return m_bDrawSelectedIcon;	};
	inline void		SetDrawSelectedIcon(bool draw)	{	m_bDrawSelectedIcon = draw;	};

	inline void		SetCurrentSelectedItemIndex(int index)		{	m_CurrentSelectedItemIndex = index;		};
	inline void		SetCurrentSelectedItemStartTime(DWORD time)	{	m_CurrentSelectedItemStartTime = time;		};
	
	inline int		GetLevelButtonIndex(void)					{	return m_LevelButtonIndex;				};

	inline bool		GetHaveMouseSlot(void)	{	return m_bHaveMouseSlot;	};

	void			ReplacementOfItems(BOOL bChange); //Author : 양희왕 //breif : 아이템 교체 중임..
	inline BOOL		GetChangingItem() { return m_bChangeItem; }//Author : 양희왕 //brief : 현재 아이템을 교체하고 있는지 여부
	
};

#endif // !defined(AFX_XWINDOW_QUICKSLOT_H__0E288073_66CA_44D2_98D1_E3BBA0C0DC2F__INCLUDED_)
