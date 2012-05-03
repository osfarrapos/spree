// XWindow_NPCSkill.h: interface for the _XWindow_NPCSkill class.
//
//////////////////////////////////////////////////////////////////////
// 2004.05.11->oneway48 insert

#if !defined(AFX_XWINDOW_NPCSKILL_H__0B8A34A8_82E3_4F31_80F9_AEA1ED2026C2__INCLUDED_)
#define AFX_XWINDOW_NPCSKILL_H__0B8A34A8_82E3_4F31_80F9_AEA1ED2026C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "Xwindow_Skill.h"
#include "XNPC.h"

class _XWindow_NPCSkill : public _XWindow  
{
private :
	
	int					m_OwnerNPCID;
	_XNPCJOBCLASS		m_OwnerNPCJobClass;
	_XGROUPTYPE			m_OwnerNPCClan;

	int					m_SkillPackageID[13];	// 0:기본, 1:공통, 2:전투, 3:치유, 4:완전, 5:기공, 6:살수, 7:호심 
												// 8:주술, 9:외공, 10:소환, 11:선도, 12:마도	
	_XButton*			m_LeftButton;
	_XButton*			m_RightButton;
	_XStatic			m_BorderTitle;

	_XListBox*			m_SkillListBox;

	_XImageStatic		m_ButtonBorderImage;
	_XImageStatic		m_SelectedImage;
	_XImageStatic		m_SelectedRightImage;

	POINT				m_listboxPosition[6];

	int					m_SelectedSkillPackageID;		// 선택된 package 번호
	int					m_SelectedSkillNo;				// Package에서 몇번째 스킬인지
	short				m_SelectedSkillID;				// 선택된 스킬 ID

	int					m_SelectedListBoxPos;
	bool				m_bDrawSelectedImage;

	int					m_PrevScrollPos;

	_XImageStatic		m_ExtraImage;
	
	_XButton*			m_SkillTypeButton[8];
	int					m_SkillTypeStr[20];
	int					m_SkillTypeStrTotalCount;
	int					m_SelectedSkillTypeIndex;
	
	_XImageStatic		m_MouseOverLeftImage;
	_XImageStatic		m_MouseOverRightImage;

	_XImageStatic		m_SelectedSkillButtonImage;

public :
	int					m_StartIndex;
	
public:
	_XWindow_NPCSkill();
	virtual ~_XWindow_NPCSkill();
	
	BOOL			Initialize( void );
	void			DestroyWindow( void );
	
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );	

	void			ResetAllSkillPackageID(void);

	int				GetCanLearnSkill(short skillid);
	void			SetListItem(void);
	
	void			DrawText(void);
	void			DrawTooltip(void);

	void			MoveWindow(int X, int Y);

	int				CheckSelectedItem(bool bListPos = false);
	int				CheckSelectedIconItem(void);
	void			ProcessDblClick(void);

	void			ShowWindow(BOOL show);
	void			SetDefaultPosition(void);

	void			DrawMouseOverImage(void);

	void			SetSkillTypeButton(void);
	void			SetSkillTypeButtonTexture(void);
	BOOL			HaveSkillType(char stype);
	void			SetStartIndex(int index);

	inline void		SetOwnerNPCID(int id)		{	m_OwnerNPCID = id;		};
	inline int		GetOwnerNPCID(void)			{	return m_OwnerNPCID;	};
	
	inline void		SetSkillPackageID(int index, int id)	{	m_SkillPackageID[index] = id;	};
	inline void		ResetSkillPackageID(int index)			{	m_SkillPackageID[index] = -1;	};
	inline int		GetSkillPackageID(int index)			{	return m_SkillPackageID[index];	};

	inline int		GetSelectedSkillPackageID(void)			{	return m_SelectedSkillPackageID;	};
	inline int		GetSelectedSkillNo(void)				{	return m_SelectedSkillNo;			};
	inline short	GetSelectedSkillID(void)				{	return m_SelectedSkillID;			};
	
	inline void		SetSelectedSkillPackageID(int id)		{	m_SelectedSkillPackageID = id;		};
	inline void		SetSelectedSkillNo(int no)				{	m_SelectedSkillNo = no;				};
	inline void		SetSelectedSkillID(short skillid)		{	m_SelectedSkillID = skillid;		};

	inline void		SetDrawSelectedImage(bool draw)			{	m_bDrawSelectedImage = draw;		};
	inline bool		GetDrawSelectedImage(void)				{	return m_bDrawSelectedImage;		};

	inline void		SetSelectedSkillTypeIndex(int index)	{	m_SelectedSkillTypeIndex = index;	};
	
};


#endif // !defined(AFX_XWINDOW_NPCSKILL_H__0B8A34A8_82E3_4F31_80F9_AEA1ED2026C2__INCLUDED_)
