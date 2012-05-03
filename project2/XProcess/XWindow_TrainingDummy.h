// XWindow_TrainingDummy.h: interface for the _XWindow_TrainingDummy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TRAININGDUMMY_H__8BD25E06_2A2F_4EEF_A0B4_4FECAE6FA396__INCLUDED_)
#define AFX_XWINDOW_TRAININGDUMMY_H__8BD25E06_2A2F_4EEF_A0B4_4FECAE6FA396__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow_Training.h"
#include "XDummy.h"

class _XWindow_TrainingDummy : public _XWindow_Training  
{
public :
	BOOL			m_bTrainingStart;

	POINT			m_MouseStartPos;
	POINT			m_MouseEndPos;
	POINT			m_CurrentMousePos;

	FLOAT			m_MousePosX;

	BOOL			m_bMoveLeftDirection;
	DWORD			m_PrevMoveTime;

	int				m_ValidAreaStart;
	int				m_ValidAreaEnd;

	int				m_SuccessPoint;
	int				m_BonusPoint;
	int				m_ComboPoint;
	
	BOOL			m_bStopMousePoint;
	BOOL			m_bSuccess;
	BOOL			m_bFail;

	DWORD			m_PrevClickTime;

	BOOL			m_bWaitForServerRes;
	BOOL			m_bComplete;

	int				m_MouseAlphaValue;

	_XDummy*		m_pDummy;

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

	_XImageStatic	m_ComboImage;
	DWORD			m_ComboImageStartTime;
	int				m_ComboAlphaValue;
	BOOL			m_bComboImageDraw;
	POINT			m_ComboImagePosition;

	_XImageStatic	m_NumberImage[10];

	bool			m_bDrawMouseSuccessAnimation;
	bool			m_bDrawMouseFailAnimation;
	
	DWORD			m_PrevDrawTimeMouse;
	FLOAT			m_EffectFrameMouse;
	bool			m_EffectOrderSubMouse;
	
	_XImageStatic	m_SuccessEffectImage_1[3];
	_XImageStatic	m_SuccessEffectImage_2[3];
	_XImageStatic	m_FailEffectImage[2];
	
public:
	_XWindow_TrainingDummy();
	virtual ~_XWindow_TrainingDummy();

	BOOL			Initialize(void);
	void			DestroyWindow(void);

	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void			ShowWindow(BOOL show);	
	void			SetBackground(int x, int y);

	void			DecideSuccess(void);
	void			DecideFail(void);
	void			DecideBonus(void);

	void			Reset(void);
	void			ResetMousePosition(void);
	void			Upgrade(void);

	void			Complete(void);

	void			DrawMouseSuccessAnimation(void);
	void			DrawMouseFailAnimation(void);

};

#endif // !defined(AFX_XWINDOW_TRAININGDUMMY_H__8BD25E06_2A2F_4EEF_A0B4_4FECAE6FA396__INCLUDED_)
