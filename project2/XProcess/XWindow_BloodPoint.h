// XWindow_BloodPoint.h: interface for the _XWindow_BloodPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_BLOODPOINT_H__455836A3_4CF0_4841_A515_415D4370C5CE__INCLUDED_)
#define AFX_XWINDOW_BLOODPOINT_H__455836A3_4CF0_4841_A515_415D4370C5CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_BloodPoint : public _XWindow
{
public : 
	_XImageStatic*		m_TitleImage;
	_XButton*			m_ReqItemButton;

	int					m_NumberPos[3];
	int					m_NumberType[3];

	BOOL				m_bDrawAnimation;
	int					m_CurrentDrawType[3];
	int					m_CurrentPoint[3];
	int					m_CurrentFrame;

	DWORD				m_DrawTime;

	_XImageStatic		m_NumberImage[4][10];	// 4 type, 0 ~ 9

	BOOL				m_bDraw_Button_StartAnimation;
	BOOL				m_bDraw_Button_Animation;
	DWORD				m_Draw_Button_Time;
	int					m_Current_Button_Frame;
	
	DWORD				m_ViewDeadMessageStartTime;

public:
	_XWindow_BloodPoint();
	virtual ~_XWindow_BloodPoint();

	BOOL		Initialize(void);

	void		Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL		Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void		DrawAnimation(int frame);
	void		DrawButtonAnimation(void);

	void		SetDrawAnimation(BOOL draw);

	void		SetShowReqItem(BOOL show);

	inline void	SetDeadMessageStartTime(DWORD time)	{	m_ViewDeadMessageStartTime = time;	};
	inline DWORD GetDeadMessageStartTime(void)		{	return m_ViewDeadMessageStartTime;	};
};

#endif // !defined(AFX_XWINDOW_BLOODPOINT_H__455836A3_4CF0_4841_A515_415D4370C5CE__INCLUDED_)
