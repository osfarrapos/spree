// XWindow_TrainingHelp.h: interface for the _XWindow_TrainingHelp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TRAININGHELP_H__31C41DA3_7447_4E04_A74B_A83B23D4C9F7__INCLUDED_)
#define AFX_XWINDOW_TRAININGHELP_H__31C41DA3_7447_4E04_A74B_A83B23D4C9F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"


class _XWindow_TrainingHelp : public _XWindow
{
private :
	int				m_TrainingType;

	_XButton*		m_SkillButton[8];
	_XButton*		m_LeftButton;
	_XButton*		m_RightButton;

	_XImageStatic	m_ButtonBorderImage;
	_XImageStatic	m_SelectedSkillButtonImage;

	int				m_StartIndex;
	int				m_SelectedSkillButtonIndex;

	_XToolTip		m_Tooltip;

	map <int, short>	m_SkillStr;
	
	_XImageStatic*	m_TypeBorderLeft;
	_XImageStatic*	m_TypeBorderRight;

	_XImageStatic	m_MeditationImage[6];

	_XImageStatic*	m_PointImageLeft;
	_XImageStatic*	m_PointImageRight;
	
public:
	_XButton*		m_EndButton; // 취소 버튼
public:
	_XWindow_TrainingHelp();
	virtual ~_XWindow_TrainingHelp();

	BOOL			Initialize(void);

	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void			ShowWindow(BOOL show);
	void			SetSkillButtonTexture(void);

	void			DrawTooltip(void);

	short			GetSelectedSkillID(void);

	inline void		SetTrainingType(int type)	{	m_TrainingType = type;		};
	inline int		GetTrainingType(void)		{	return m_TrainingType;		};

	void			SetStartIndex(int index);
	inline int		GetStartIndex(void)			{	return m_StartIndex;		};

	inline void		SetSelectedSkillButtonIndex(int index)	{	m_SelectedSkillButtonIndex = index;		};
	inline int		GetSelectedSkillButtonIndex(void)		{	return m_SelectedSkillButtonIndex;		};

};

#endif // !defined(AFX_XWINDOW_TRAININGHELP_H__31C41DA3_7447_4E04_A74B_A83B23D4C9F7__INCLUDED_)
