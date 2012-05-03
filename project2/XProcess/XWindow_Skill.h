// XWindow_Skill.h: interface for the _XWindow_Skill class.
//
// Modified Date	: 2003.03.03
// Author			: Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_SKILL_H_
#define _XWINDOW_SKILL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#define _XDEF_MAX_SKILLSLOTNUM			40

struct _XStr_SkillButton
{
	char			clan;
	char			stype;
};

class _XWindow_Skill : public _XWindow
{
public :
	_XButton*		m_LeftButton;
	_XButton*		m_RightButton;

private :
	_XImageStatic*	m_ButtonBorderImage;
	
	_XListBox*		m_listboxSkill;
	_XStickGauge*	m_SkillExp;
	_XImageStatic*	m_SelectedImage;
	_XImageStatic*	m_SelectedRightImage;

	_XButton*		m_SkillButton[8];
	
	_XImageStatic*	m_SelectedSkillButtonImage;
	
#ifdef _XTS_NEWSKILLWINDOW
	_XCheckButton*	m_SkillSortButton[8];
	int				m_SelectedSortIndex;
#endif
	
	POINT			m_listboxPosition[10];
	
	int				m_SelectedListBoxPos;
	short			m_SelectedSkillID;
	bool			m_bDrawSelectedImage;
	
	short			m_MouseSlotSkillID;
	bool			m_bHaveMouseSlot;

	POINT			m_IconDragStartPos;
	bool			m_bDragState;

	_XStr_SkillButton	m_SkillButtonStr[20];
	int					m_SelectedSkillButtonIndex;
	int					m_SkillButtonStrTotalCount;
	
	_XImageStatic	m_MouseOverLeftImage;
	_XImageStatic	m_MouseOverRightImage;

	int				m_PrevScrollPos;
	
	_XListBox*		m_lstboxSkillDesc;

public :
//	int				m_SkillButtonIndex[8];
	int				m_StartIndex;
	
public :
	_XWindow_Skill();
	~_XWindow_Skill();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			SetListItem(void);
#ifdef _XTS_NEWSKILLWINDOW
	void			SetListItemBySType(void);

	void			ChangeTab(int tab);
#endif
	
	void			DrawText(void);
	int				CheckSelectedItem(bool bListPos = false);
	void			ProcessDblClick(void);

	void			ResetMouseSlot(void);
	int				CheckSelectedIconItem(void);
	void			DrawDragIcon(void);

	void			DrawClanStypeIcon(void);

	void			SetSkillButton(void);
	void			SetSkillButtonTexture(void);
	void			SetStartIndex(int index);

	void			DrawMouseOverImage(void);
	void			RefreshData(void);
	
	inline void		SetSelectedSkillID(short skillid)		{	m_SelectedSkillID = skillid;		};
	inline short	GetSelectedSkillID(void)				{	return m_SelectedSkillID;			};

	inline short	GetMouseSlotSkillID(void)				{	return m_MouseSlotSkillID;			};
	inline bool		GetHaveMouseSlot(void)					{	return m_bHaveMouseSlot;			};

	inline void		SetSelectedSkillButtonIndex(int index)	{	m_SelectedSkillButtonIndex = index;	};
	inline int		GetSelectedSkillButtonIndex(void)		{	return m_SelectedSkillButtonIndex;	};

#ifdef _XTS_NEWSKILLWINDOW
	inline void		SetSelectedSortIndex(int index)			{	m_SelectedSortIndex = index;		};
	inline int		GetSelectedSortIndex(void)				{	return m_SelectedSortIndex;			};
#endif
};

BOOL __stdcall _XSkillSelectCheckCallBack( int param1, int param2 );

#endif // !defined(AFX_XWINDOW_SKILL_H__107E737B_975C_456A_ADBB_B8FC9B47BDD1__INCLUDED_)