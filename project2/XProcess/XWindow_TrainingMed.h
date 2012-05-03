// XWindow_TrainingMed.h: interface for the _XWindow_TrainingMed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TRAININGMED_H__45F0FFB5_0F40_4DC7_9AD3_AD3D3E3360BD__INCLUDED_)
#define AFX_XWINDOW_TRAININGMED_H__45F0FFB5_0F40_4DC7_9AD3_AD3D3E3360BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

struct _XMD_Unit
{
	int		x;
	int		y;
	int		type;

	int		dupIndex;
	DWORD	prevDrawTime;
	FLOAT	effectFrame;

	BOOL	bDuplicate : 2;
	BOOL	bComplete : 2;
	BOOL	bSuccess : 2;
	BOOL	bEffectOrderSub : 2;
	BOOL	reserved : 24;

	_XMD_Unit()
	{
		x, y, type, dupIndex = 0;
		prevDrawTime = 0;
		effectFrame = 0.0f;
		bDuplicate, bComplete, bSuccess, bEffectOrderSub = FALSE;
	}
};

class _XWindow_TrainingMed : public _XWindow  
{
private :
	_XMD_Unit*		m_Path[20];
	int				m_PathCount[20];

	_XImageStatic*	m_BorderImage;
	_XImageStatic	m_Point[32];
	
	int				m_MeditationStep;
	int				m_MousePositionIndex;

	_XImageStatic	m_MousePositionImage[2];
	int				m_PointClickType[32];

	BOOL			m_bLeftClick;
	BOOL			m_bRightClick;

//	BOOL			m_bSuccess;
	BOOL			m_bFail;

	_XImageStatic	m_SuccessYangEffectImage[6];
	_XImageStatic	m_SuccessUmEffectImage[6];
	_XImageStatic	m_SuccessMuEffectImage[6];
	_XImageStatic	m_SuccessBigEffectImage[5];

	DWORD			m_PrevDrawTime;
	BOOL			m_bEffectOrderSub;
	FLOAT			m_EffectFrame;

	BOOL			m_bStartGame;
	DWORD			m_PrevCompleteImageDrawTime;
	int				m_CompleteImageDrawIndex;

	_XImageStatic	m_FailEffectImage[2];
	_XImageStatic	m_CompleteEffectImage;

	_XImageStatic	m_SuccessImage;			// ¼º°ø
	DWORD			m_SuccessImageStartTime;
	int				m_SuccessAlphaValue;
	POINT			m_SuccessImagePosition;

	_XImageStatic	m_FailImage;
	DWORD			m_FailImageStartTime;
	int				m_FailAlphaValue;
	POINT			m_FailImagePosition;

	_XImageStatic	m_NumberBigImage[10];
	_XImageStatic	m_NumberSmallImage[10];
	_XImageStatic	m_NumberComboImage_1[10];
	_XImageStatic	m_NumberComboImage_2[10];
	_XImageStatic	m_NumberComboImage_3[10];
	_XImageStatic	m_SlashImage;

	int				m_RestCount[3];
	int				m_TotalCount[3];

	int				m_ComboPoint;
	int				m_ComboCount[3];
	_XImageStatic	m_ComboImage;
	DWORD			m_ComboImageStartTime;
	int				m_ComboAlphaValue;

	int				m_ModifiedYPosition;

	BOOL			m_bEndCameraRotate;
	BOOL			m_bEndCameraZoomIn;

	FLOAT			m_fRotateScreenAngel;

	int				m_LastStep;

public :
	BOOL			m_bCompleteClient;
	BOOL			m_bComplete;

	DWORD			m_SendPacketTime;
	int				m_MissionStage;
	
	BOOL			m_bTrainingStart;
	BOOL			m_bCameraMoveStart;

	BOOL			m_bQuestMiniGameStart;


public:
	_XWindow_TrainingMed();
	virtual ~_XWindow_TrainingMed();

	BOOL			Initialize(void);
	void			DestroyWindow(void);

	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	BOOL			LoadMeditationRecipe(void);
	void			ReleaseRecipe(void);

	void			ShowWindow(BOOL show);
	void			SetMedImagePosition(void);
	void			ParseDuplicateIndex(void);

	void			DecideSuccess(void);
	void			DecideFail(void);

	void			DrawFailEffect(int x, int y);
	void			DrawCompleteImage(int index, int x, int y);
	void			DrawSuccessImage(int index, int x, int y);
	void			DrawMousePositionImage(int x, int y);

	void			Reset(void);
	void			Complete(void);
	void			PlaySuccessSound(void);

	void			SetMeditationStep(int step);

	void			CameraMove(void);
	void			ZoomIn(void);

	void			CloseInterface(void);
	void			OpenInterface(void);

	void			DrawGuide(void);

};

#endif // !defined(AFX_XWINDOW_TRAININGMED_H__45F0FFB5_0F40_4DC7_9AD3_AD3D3E3360BD__INCLUDED_)
