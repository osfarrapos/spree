// XWindow_Training.h: interface for the _XWindow_Training class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TRAINING_H__D670F5DD_67ED_427F_A42C_F347F72880F9__INCLUDED_)
#define AFX_XWINDOW_TRAINING_H__D670F5DD_67ED_427F_A42C_F347F72880F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_Training : public _XWindow  
{
public :
	_XImageStatic*			m_FirstGradeImage;
	_XImageStatic*			m_SecondGradeImage;
	_XImageStatic*			m_ThirdGradeImage;

	_XImageStatic*			m_LevelImage;

	_XImageStatic			m_GaugeGradeImage;
	_XImageStatic			m_GaugeBallImage[4];
	
	_XImageStatic			m_MousePointImage[7];

	int						m_TrainingGrade;

	POINT					m_GaugePosition[12];
	int						m_CurrentGaugePoint;
	bool					m_bDrawGaugeAnimation;

	DWORD					m_PrevDrawTimeGauge;
	FLOAT					m_EffectFrameGauge;
	bool					m_EffectOrderSubGauge;

public:
	_XWindow_Training();
	virtual ~_XWindow_Training();

	virtual BOOL			Initailize(void);
	virtual void			DestroyWindow(void);

	virtual void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	virtual BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	virtual void			Reset(void);

	virtual void			SetBackground(int x, int y);

	virtual void			ShowBackground(BOOL show);
	virtual void			ShowGrade(BOOL show);

	virtual void			AddGauge(int point);
	virtual void			DeleteGauge(int point);
	virtual void			DrawGaugeAnimation(void);

	virtual void			SetTrainingGrade(int grade) {	m_TrainingGrade = grade;	};

};

#endif // !defined(AFX_XWINDOW_TRAINING_H__D670F5DD_67ED_427F_A42C_F347F72880F9__INCLUDED_)
