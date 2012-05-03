// XWindow_Training.cpp: implementation of the _XWindow_Training class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_Training.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Training::_XWindow_Training()
{
	m_FirstGradeImage = NULL;
	m_SecondGradeImage = NULL;
	m_ThirdGradeImage = NULL;
	m_LevelImage = NULL;

	Reset();
}

_XWindow_Training::~_XWindow_Training()
{
	DestroyWindow();
}


BOOL _XWindow_Training::Initailize(void)
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("MI_tra_base.tga");

	_XImageStatic* pBackLeftImage = new _XImageStatic;
	pBackLeftImage->Create(0, 0, 186, 109, &g_MainInterfaceTextureArchive, resourceindex);
	pBackLeftImage->SetClipRect(70, 146, 256, 255);
	InsertChildObject(pBackLeftImage);

	_XImageStatic* pBackRightImage = new _XImageStatic;
	pBackRightImage->Create(371, 0, 557, 109, &g_MainInterfaceTextureArchive, resourceindex);
	pBackRightImage->SetClipRect(70, 146, 256, 255);
	pBackRightImage->SetScale(-1, 1);
	InsertChildObject(pBackRightImage);
	
	// 1
	m_FirstGradeImage = new _XImageStatic;
	m_FirstGradeImage->Create(172, 27, 172+14, 27+23, &g_MainInterfaceTextureArchive, resourceindex);
	m_FirstGradeImage->SetClipRect(4, 149, 18, 172);
	m_FirstGradeImage->ShowWindow(FALSE);
	InsertChildObject(m_FirstGradeImage);

	// 2
	m_SecondGradeImage = new _XImageStatic;
	m_SecondGradeImage->Create(172, 27, 172+16, 27+23, &g_MainInterfaceTextureArchive, resourceindex);
	m_SecondGradeImage->SetClipRect(26, 149, 42, 172);
	m_SecondGradeImage->ShowWindow(FALSE);
	InsertChildObject(m_SecondGradeImage);

	// 3
	m_ThirdGradeImage = new _XImageStatic;
	m_ThirdGradeImage->Create(172, 27, 172+16, 27+23, &g_MainInterfaceTextureArchive, resourceindex);
	m_ThirdGradeImage->SetClipRect(48, 149, 64, 172);
	m_ThirdGradeImage->ShowWindow(FALSE);
	InsertChildObject(m_ThirdGradeImage);

	// 成
	m_LevelImage = new _XImageStatic;
	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
	{
		m_LevelImage->Create(186, 31, 186+41, 31+12, &g_MainInterfaceTextureArchive, resourceindex);
		m_LevelImage->SetClipRect(21, 182, 62, 194);
	}
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_LevelImage->Create(186, 27, 186+61, 27+22, &g_MainInterfaceTextureArchive, resourceindex);
		m_LevelImage->SetClipRect(5, 175, 66, 197);
	}
	else
	{
		m_LevelImage->Create(186, 31, 186+20, 31+22, &g_MainInterfaceTextureArchive, resourceindex);
		m_LevelImage->SetClipRect(43, 174, 63, 196);
	}
	m_LevelImage->ShowWindow(FALSE);
	InsertChildObject(m_LevelImage);

	// 검은색 원
	m_GaugeGradeImage.Create(0, 0, 12, 12, &g_MainInterfaceTextureArchive, resourceindex);
	m_GaugeGradeImage.SetClipRect(210, 77 , 222, 89);

	// 색깔 있는 원
	for(int i = 0 ; i < 4 ; i++)
	{
		m_GaugeBallImage[i].Create(0, 0, 16, 14, &g_MainInterfaceTextureArchive, resourceindex);
		m_GaugeBallImage[i].SetClipRect(208, 20+(i*14), 224, 34+(i*14));
	}

	// 마우스 포인터
	for(i = 0 ; i < 7 ; i++)
	{
		m_MousePointImage[i].Create(0, 0, 26, 24, &g_MainInterfaceTextureArchive, resourceindex);
		m_MousePointImage[i].SetClipRect(2+(28*i), 89, 28+(28*i), 113);
	}

	// 취소 버튼
	int buttonindex = g_MainInterfaceTextureArchive.FindResource("MI_quest2.tga");
	_XBTN_STRUCTURE btnstruct =
	{
		TRUE, {248, 87}, {52, 18},
		_XDEF_TRAINING_CANCEL_OK,
		buttonindex,
		buttonindex,
		buttonindex,
		&g_MainInterfaceTextureArchive
	};
	_XButton* pCancelButton = new _XButton;
	pCancelButton->Create(btnstruct);
	pCancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 170, 52, 188);
	pCancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 53, 170, 105, 188);
	pCancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 106, 170, 158, 188);
	pCancelButton->SetGrowTextMode(TRUE);
	pCancelButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 203, 203, 203));
	pCancelButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_CANCEL);
	InsertChildObject(pCancelButton);

	m_TrainingGrade = 0;

	for(i = 0 ; i < 12 ; i++)
	{
		m_GaugePosition[i].x = 77 + (i*14);
		m_GaugePosition[i].y = 92;
	}

	SetWindowMoveMode(_XWMOVE_FIXED);

	return TRUE;
}

void _XWindow_Training::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_Training::Reset(void)
{
	m_CurrentGaugePoint = 0;
	m_bDrawGaugeAnimation = false;
	
	m_PrevDrawTimeGauge = 0;
	m_EffectFrameGauge = 0.0f;
	m_EffectOrderSubGauge = false;
}

void _XWindow_Training::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow) 	
		return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) 
		return;

	for(int i = 0 ; i < 12 ; i++)
	{
		if(i < m_CurrentGaugePoint)
			m_GaugeBallImage[0].Draw(m_WindowPosition.x+m_GaugePosition[i].x-1, m_WindowPosition.y+m_GaugePosition[i].y-1);
		else
			m_GaugeGradeImage.Draw(m_WindowPosition.x+m_GaugePosition[i].x, m_WindowPosition.y+m_GaugePosition[i].y);
	}

	if(m_bDrawGaugeAnimation)
		DrawGaugeAnimation();
}
BOOL _XWindow_Training::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_Training::SetBackground(int x, int y)
{
/*	m_BackgroundImage->MoveWindow(x, y);

	m_GaugeBackImage->MoveWindow(x, y+32);
	m_GaugeRulerImage->MoveWindow(x, y+31);
	m_GaugeGradeImage->MoveWindow(x, y+32);

	m_FirstGradeImage->MoveWindow(x-43, y);
	m_SecondGradeImage->MoveWindow(x-43, y);
	m_ThirdGradeImage->MoveWindow(x-43, y);*/
}

void _XWindow_Training::ShowBackground(BOOL show)
{
/*	m_BackgroundImage->ShowWindow(show);
	m_BackgroundAlphaImage->ShowWindow(show);

	m_GaugeBackImage->ShowWindow(show);
	m_GaugeRulerImage->ShowWindow(show);
	m_GaugeGradeImage->ShowWindow(show);

	m_MousePointImage->ShowWindow(show);*/
}

void _XWindow_Training::ShowGrade(BOOL show)
{
	if(show)
	{
		switch(m_TrainingGrade)
		{
		case 1 :
			{
				m_FirstGradeImage->ShowWindow(show);
				m_SecondGradeImage->ShowWindow(!show);
				m_ThirdGradeImage->ShowWindow(!show);
			}
			break;
		case 2 :
			{
				m_FirstGradeImage->ShowWindow(!show);
				m_SecondGradeImage->ShowWindow(show);
				m_ThirdGradeImage->ShowWindow(!show);
			}
			break;
		case 3 :
			{
				m_FirstGradeImage->ShowWindow(!show);
				m_SecondGradeImage->ShowWindow(!show);
				m_ThirdGradeImage->ShowWindow(show);
			}
			break;
		}
		m_LevelImage->ShowWindow(show);
	}
	else
	{
		m_FirstGradeImage->ShowWindow(show);
		m_SecondGradeImage->ShowWindow(show);
		m_ThirdGradeImage->ShowWindow(show);
		m_LevelImage->ShowWindow(show);
	}
}

void _XWindow_Training::AddGauge(int point)
{
	m_CurrentGaugePoint = point;
	m_bDrawGaugeAnimation = true;
}

void _XWindow_Training::DeleteGauge(int point)
{
	m_CurrentGaugePoint = point;
	m_bDrawGaugeAnimation = false;
}


void _XWindow_Training::DrawGaugeAnimation(void)
{
	if(m_PrevDrawTimeGauge == 0)
		m_PrevDrawTimeGauge = g_LocalSystemTime;

	DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTimeGauge;

	if(!m_EffectOrderSubGauge)
		m_EffectFrameGauge += ((FLOAT)elapsedtime / 150.0f);
	else
		m_EffectFrameGauge -= ((FLOAT)elapsedtime / 150.0f);

	if(m_EffectFrameGauge > 4.0f)
	{
		m_EffectOrderSubGauge = true;
		m_EffectFrameGauge = 3.0f;
	}
	else if(m_EffectFrameGauge <= 0.0f)
	{
		m_EffectOrderSubGauge = false;
		m_EffectFrameGauge = 1.0f;

		m_bDrawGaugeAnimation = false;
	}

	int selectedframe = (int)m_EffectFrameGauge;

	if(selectedframe > 3)
		selectedframe = 3;
	else if(selectedframe < 0)
		selectedframe = 0;

	m_GaugeBallImage[selectedframe].Draw(m_WindowPosition.x+m_GaugePosition[m_CurrentGaugePoint-1].x-1, 
		m_WindowPosition.y+m_GaugePosition[m_CurrentGaugePoint-1].y-1);
	m_PrevDrawTimeGauge = g_LocalSystemTime;
}
