// xwindow_trainingspell.h: interface for the _XWindow_TrainingSpell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TRAININGSPELL_H__C117CC59_4403_4C54_AB0E_C8265626BC74__INCLUDED_)
#define AFX_XWINDOW_TRAININGSPELL_H__C117CC59_4403_4C54_AB0E_C8265626BC74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow_Training.h"

enum _XTRAINING_MOUSE_DIR
{
	MOUSE_DIR_LEFT,
	MOUSE_DIR_RIGHT
};

class _XWindow_TrainingSpell  : public _XWindow_Training
{
public :
	BOOL			m_bTrainingStart;
	DWORD			m_TrainingStartTime;
	BOOL			m_bWaitForServerRes;

	BOOL			m_bComplete;

private :
	POINT			m_MouseStartPos;
	POINT			m_MouseEndPos;
	POINT			m_CurrentMousePos;

	FLOAT			m_MousePosX;

	int				m_ValidAreaStart;
	int				m_ValidAreaEnd;

	_XTRAINING_MOUSE_DIR	m_MouseDir;
	FLOAT			m_MouseVelocity;
	DWORD			m_MouseForceKeepTime;
	DWORD			m_MouseMoveStartTime;

	DWORD			m_PrevMoveTime;

	DWORD			m_FailStartTime;
	BOOL			m_bFailPeriod;

	FLOAT			m_LeftClickForce;
	FLOAT			m_RightClickForce;
	
	DWORD			m_StaySuccessAreaTime;
	DWORD			m_StaySuccessKeepTime;

	int				m_SuccessPoint;

	_XImageStatic	m_SuccessImage;
	DWORD			m_SuccessImageStartTime;
	int				m_SuccessAlphaValue;
	BOOL			m_bSuccessImageDraw;
	POINT			m_SuccessImagePosition;
	
	_XImageStatic	m_FailImage;
	DWORD			m_FailImageStartTime;
	int				m_FailAlphaValue;
	BOOL			m_bFailImageDraw;
	POINT			m_FailImagePosition;
	
	bool			m_bDrawMouseSuccessAnimation;
	bool			m_bDrawMouseFailAnimation;
	
	DWORD			m_PrevDrawTimeMouse;
	FLOAT			m_EffectFrameMouse;
	bool			m_EffectOrderSubMouse;
	
	_XImageStatic	m_SuccessEffectImage_1[3];
	_XImageStatic	m_SuccessEffectImage_2[3];
	_XImageStatic	m_FailEffectImage[2];
	
public:
	_XWindow_TrainingSpell();
	virtual ~_XWindow_TrainingSpell();

	BOOL			Initialize(void);
	void			DestroyWindow(void);

	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void			ShowWindow(BOOL show);
	void			SetBackground(int x, int y);

	void			Reset(void);
	void			ResetMousePos(void);

	void			DecideMouseProperty(void);
	void			DecideSuccess(void);
	void			DecideMiss(void);

	void			Upgrade(void);
	void			Complete(void);

	void			DrawMouseSuccessAnimation(void);
	void			DrawMouseFailAnimation(void);
};

#endif // !defined(AFX_XWINDOW_TRAININGSPELL_H__C117CC59_4403_4C54_AB0E_C8265626BC74__INCLUDED_)
