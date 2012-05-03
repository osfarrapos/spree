// XWindow_TrainingMed.cpp: implementation of the _XWindow_TrainingMed class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_TrainingMed.h"
#include "EODEXTDEF_GlobalObject.h"
#include "SoundEffectList.h"
#include "XWindowObjectDefine.h"
#include "XKernel.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_TrainingComplete.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_TrainingMed::_XWindow_TrainingMed()
{
	for(int i = 0 ; i < 20 ; i++)
		m_Path[i] = NULL;
	memset(m_PathCount, 0, sizeof(int)*20);

	Reset();
}

_XWindow_TrainingMed::~_XWindow_TrainingMed()
{

}

BOOL _XWindow_TrainingMed::Initialize(void)
{
	int borderindex = g_MainInterfaceTextureArchive.FindResource("MI_unki_03.tga");
	int resourceindex_3 = g_MainInterfaceTextureArchive.FindResource("MI_tra_base.tga");
	int effectindex = g_MainInterfaceTextureArchive.FindResource("MI_tra_effect.tga");

	int resourceindex_1 = -1;	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		resourceindex_1 = g_MainInterfaceTextureArchive.FindResource("vn_MI_unki_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		resourceindex_1 = g_MainInterfaceTextureArchive.FindResource("us_MI_unki_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		resourceindex_1 = g_MainInterfaceTextureArchive.FindResource("tw_MI_unki_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		resourceindex_1 = g_MainInterfaceTextureArchive.FindResource("rs_MI_unki_01.tga");
	}
	else 
	{
		resourceindex_1 = g_MainInterfaceTextureArchive.FindResource("MI_unki_01.tga");
	}	

	int resourceindex_2 = -1;	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("vn_MI_unki_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("us_MI_unki_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("tw_MI_unki_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("rs_MI_unki_02.tga");
	}
	else 
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("MI_unki_02.tga");
	}	

	int resourceindex_4 = -1;	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		resourceindex_4 = g_MainInterfaceTextureArchive.FindResource("vn_MI_unki_04.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		resourceindex_4 = g_MainInterfaceTextureArchive.FindResource("us_MI_unki_04.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		resourceindex_4 = g_MainInterfaceTextureArchive.FindResource("tw_MI_unki_04.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		resourceindex_4 = g_MainInterfaceTextureArchive.FindResource("rs_MI_unki_04.tga");
	}
	else 
	{
		resourceindex_4 = g_MainInterfaceTextureArchive.FindResource("MI_unki_04.tga");
	}	
	
	m_BorderImage = new _XImageStatic;
	m_BorderImage->Create(107, 0, 512, 190, &g_MainInterfaceTextureArchive, borderindex);
	m_BorderImage->SetClipRect(0, 0, 512, 190);
	m_BorderImage->SetScale(1.582f, 1.582f);
	InsertChildObject(m_BorderImage);

	for(int i = 0 ; i < 12 ; i++)
	{
		m_Point[i].Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, resourceindex_1);
	}
	m_Point[0].SetClipRect(2, 2, 32, 32);			// 양
	m_Point[1].SetClipRect(2, 36, 32, 66);			// 음
	m_Point[2].SetClipRect(2, 70, 32, 100);			// 무
	m_Point[3].SetClipRect(2, 2, 32, 32);			// 양
	m_Point[4].SetClipRect(2, 2, 32, 32);			// 양
	m_Point[5].SetClipRect(2, 2, 32, 32);			// 양
	m_Point[6].SetClipRect(2, 36, 32, 66);			// 음
	m_Point[7].SetClipRect(2, 36, 32, 66);			// 음
	m_Point[8].SetClipRect(2, 36, 32, 66);			// 음
	m_Point[9].SetClipRect(2, 70, 32, 100);			// 무
	m_Point[10].SetClipRect(2, 70, 32, 100);		// 무
	m_Point[11].SetClipRect(2, 70, 32, 100);		// 무

	for(i = 12 ; i < 32 ; i++)
	{
		m_Point[i].Create(0, 0, 60, 30, &g_MainInterfaceTextureArchive, resourceindex_2);
	}
	m_Point[12].SetClipRect(2, 2, 62, 32);			// 기해
	m_Point[13].SetClipRect(66, 2, 126, 32);		// 천추
	m_Point[14].SetClipRect(130, 2, 190, 32);		// 명문
	m_Point[15].SetClipRect(194, 2, 254, 32);		// 기문
	m_Point[16].SetClipRect(2, 36, 62, 66);			// 영대
	m_Point[17].SetClipRect(66, 36, 126, 66);		// 혈해
	m_Point[18].SetClipRect(130, 36, 190, 66);		// 화개
	m_Point[19].SetClipRect(194, 36, 254, 66);		// 천주
	m_Point[20].SetClipRect(2, 70, 62, 100);		// 선기
	m_Point[21].SetClipRect(66, 70, 126, 100);		// 협백
	m_Point[22].SetClipRect(130, 70, 190, 100);		// 곡지
	m_Point[23].SetClipRect(194, 70, 254, 100);		// 내관
	m_Point[24].SetClipRect(2, 104, 62, 104);		// 중적
	m_Point[25].SetClipRect(66, 104, 126, 104);		// 견정
	m_Point[26].SetClipRect(130, 104, 190, 104);	// 거궐
	m_Point[27].SetClipRect(194, 104, 254, 104);	// 중극
	m_Point[28].SetClipRect(2, 138, 62, 168);		// 풍부
	m_Point[29].SetClipRect(66, 138, 126, 168);		// 아문
	m_Point[30].SetClipRect(130, 138, 190, 168);	// 백회
	m_Point[31].SetClipRect(194, 138, 254, 168);	// 용천

	m_MousePositionImage[0].Create(0, 0, 34, 34, &g_MainInterfaceTextureArchive, effectindex);
	m_MousePositionImage[0].SetClipRect(153, 1, 187, 35);
	m_MousePositionImage[1].Create(0, 0, 66, 36, &g_MainInterfaceTextureArchive, effectindex);
	m_MousePositionImage[1].SetClipRect(120, 106, 186, 142);

	// 혈 종류 - 마우스 클릭타입	1-Left	2-Right	3-Both
	m_PointClickType[0] = 1;			// 양
	m_PointClickType[1] = 2;			// 음
	m_PointClickType[2] = 3;			// 무
	m_PointClickType[3] = 1;			// 소양->양
	m_PointClickType[4] = 1;			// 소양->음
	m_PointClickType[5] = 1;			// 소양->무
	m_PointClickType[6] = 2;			// 소음->양
	m_PointClickType[7] = 2;			// 소음->음
	m_PointClickType[8] = 2;			// 소음->무
	m_PointClickType[9] = 3;			// 소무->양
	m_PointClickType[10] = 3;			// 소무->음
	m_PointClickType[11] = 3;			// 소무->무
	m_PointClickType[12] = 3;			// 기해(무)
	m_PointClickType[13] = 2;			// 천추(음)
	m_PointClickType[14] = 1;			// 명문(양)
	m_PointClickType[15] = 3;			// 기문(무)
	m_PointClickType[16] = 1;			// 영대(양)
	m_PointClickType[17] = 3;			// 혈해(무)
	m_PointClickType[18] = 1;			// 화개(양)
	m_PointClickType[19] = 2;			// 천주(음)
	m_PointClickType[20] = 2;			// 선기(음)
	m_PointClickType[21] = 3;			// 협백(무)
	m_PointClickType[22] = 1;			// 곡지(양)
	m_PointClickType[23] = 2;			// 내관(음)
	m_PointClickType[24] = 2;			// 중적(음)
	m_PointClickType[25] = 3;			// 견정(무)
	m_PointClickType[26] = 1;			// 거궐(양)
	m_PointClickType[27] = 2;			// 중극(음)
	m_PointClickType[28] = 1;			// 풍부(양)
	m_PointClickType[29] = 3;			// 아문(무)
	m_PointClickType[30] = 1;			// 백회(양)
	m_PointClickType[31] = 2;			// 용천(음)

	for(i = 0 ; i < 6 ; i++)
	{
		m_SuccessYangEffectImage[i].Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, resourceindex_1);
		m_SuccessYangEffectImage[i].SetClipRect(2+(34*i), 2, 2+(34*i)+30, 32);

		m_SuccessUmEffectImage[i].Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, resourceindex_1);
		m_SuccessUmEffectImage[i].SetClipRect(2+(34*i), 36, 2+(34*i)+30, 66);

		m_SuccessMuEffectImage[i].Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, resourceindex_1);
		m_SuccessMuEffectImage[i].SetClipRect(2+(34*i), 70, 2+(34*i)+30, 100);
	}

	for(i = 0 ; i < 5 ; i++)
	{
		m_SuccessBigEffectImage[i].Create(0, 0, 64, 32, &g_MainInterfaceTextureArchive, effectindex);
		m_SuccessBigEffectImage[i].SetClipRect(191, 1+(37*i), 255, 1+(37*i)+32);
	}

	m_FailEffectImage[0].Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex_1);
	m_FailEffectImage[0].SetClipRect(73, 105, 109, 141);
	m_FailEffectImage[1].Create(0, 0, 66, 36, &g_MainInterfaceTextureArchive, resourceindex_1);
	m_FailEffectImage[1].SetClipRect(2, 105, 68, 141);
	
	m_CompleteEffectImage.Create(0, 0, 26, 26, &g_MainInterfaceTextureArchive, effectindex);
	m_CompleteEffectImage.SetClipRect(160, 41, 186, 67);

	// 성공
	m_SuccessImage.Create(0, 0, 106, 60, &g_MainInterfaceTextureArchive, resourceindex_4);
	m_SuccessImage.SetClipRect(7, 0, 113, 60);
	m_SuccessImageStartTime = 0;
	m_SuccessAlphaValue = 255;

	// 실패
	m_FailImage.Create(0, 0, 111, 57, &g_MainInterfaceTextureArchive, resourceindex_4);
	m_FailImage.SetClipRect(126, 0, 237, 57);
	m_FailImageStartTime = 0;
	m_FailAlphaValue = 255;

	// 숫자 - 혈수/연타 표시
	for(i = 0 ; i < 10 ; i++)
	{
		m_NumberBigImage[i].Create(0, 0, 32, 48, &g_MainInterfaceTextureArchive, resourceindex_1);
		m_NumberSmallImage[i].Create(0, 0, 11, 17, &g_MainInterfaceTextureArchive, resourceindex_2);
		m_NumberComboImage_1[i].Create(0, 0, 26, 40, &g_MainInterfaceTextureArchive, resourceindex_4);
		m_NumberComboImage_2[i].Create(0, 0, 26, 40, &g_MainInterfaceTextureArchive, resourceindex_4);
		m_NumberComboImage_3[i].Create(0, 0, 26, 40, &g_MainInterfaceTextureArchive, resourceindex_4);
	}

	m_NumberBigImage[0].SetClipRect(206, 103, 206+32, 103+48);
	m_NumberBigImage[1].SetClipRect(4, 152, 4+32, 152+48);
	m_NumberBigImage[2].SetClipRect(36, 152, 36+32, 152+48);
	m_NumberBigImage[3].SetClipRect(70, 152, 70+32, 152+48);
	m_NumberBigImage[4].SetClipRect(103, 152, 103+32, 152+48);
	m_NumberBigImage[5].SetClipRect(138, 152, 138+32, 152+48);
	m_NumberBigImage[6].SetClipRect(172, 152, 172+32, 152+48);
	m_NumberBigImage[7].SetClipRect(205, 152, 205+32, 152+48);
	m_NumberBigImage[8].SetClipRect(137, 103, 137+32, 103+48);
	m_NumberBigImage[9].SetClipRect(171, 103, 171+32, 103+48);

	m_NumberSmallImage[0].SetClipRect(124, 226, 124+11, 226+17);
	m_NumberSmallImage[1].SetClipRect(2, 226, 2+11, 226+17);
	m_NumberSmallImage[2].SetClipRect(13, 226, 13+11, 226+17);
	m_NumberSmallImage[3].SetClipRect(27, 226, 27+11, 226+17);
	m_NumberSmallImage[4].SetClipRect(41, 226, 41+11, 226+17);
	m_NumberSmallImage[5].SetClipRect(55, 226, 55+11, 226+17);
	m_NumberSmallImage[6].SetClipRect(69, 226, 69+11, 226+17);
	m_NumberSmallImage[7].SetClipRect(83, 226, 83+11, 226+17);
	m_NumberSmallImage[8].SetClipRect(96, 226, 96+11, 226+17);
	m_NumberSmallImage[9].SetClipRect(110, 226, 110+11, 226+17);

	// 100자리
	m_NumberComboImage_1[0].SetClipRect(72, 169, 102, 209);
	m_NumberComboImage_1[1].SetClipRect(3, 122, 29, 162);
	m_NumberComboImage_1[2].SetClipRect(37, 122, 63, 162);
	m_NumberComboImage_1[3].SetClipRect(72, 122, 98, 162);
	m_NumberComboImage_1[4].SetClipRect(106, 122, 132, 162);
	m_NumberComboImage_1[5].SetClipRect(139, 122, 165, 162);
	m_NumberComboImage_1[6].SetClipRect(173, 122, 199, 162);
	m_NumberComboImage_1[7].SetClipRect(208, 122, 234, 162);
	m_NumberComboImage_1[8].SetClipRect(3, 169, 29, 209);
	m_NumberComboImage_1[9].SetClipRect(37, 169, 63, 209);

	// 10자리
	m_NumberComboImage_2[0].SetClipRect(72, 169, 102, 209);
	m_NumberComboImage_2[1].SetClipRect(3, 122, 29, 162);
	m_NumberComboImage_2[2].SetClipRect(37, 122, 63, 162);
	m_NumberComboImage_2[3].SetClipRect(72, 122, 98, 162);
	m_NumberComboImage_2[4].SetClipRect(106, 122, 132, 162);
	m_NumberComboImage_2[5].SetClipRect(139, 122, 165, 162);
	m_NumberComboImage_2[6].SetClipRect(173, 122, 199, 162);
	m_NumberComboImage_2[7].SetClipRect(208, 122, 234, 162);
	m_NumberComboImage_2[8].SetClipRect(3, 169, 29, 209);
	m_NumberComboImage_2[9].SetClipRect(37, 169, 63, 209);

	// 1자리
	m_NumberComboImage_3[0].SetClipRect(72, 169, 102, 209);
	m_NumberComboImage_3[1].SetClipRect(3, 122, 29, 162);
	m_NumberComboImage_3[2].SetClipRect(37, 122, 63, 162);
	m_NumberComboImage_3[3].SetClipRect(72, 122, 98, 162);
	m_NumberComboImage_3[4].SetClipRect(106, 122, 132, 162);
	m_NumberComboImage_3[5].SetClipRect(139, 122, 165, 162);
	m_NumberComboImage_3[6].SetClipRect(173, 122, 199, 162);
	m_NumberComboImage_3[7].SetClipRect(208, 122, 234, 162);
	m_NumberComboImage_3[8].SetClipRect(3, 169, 29, 209);
	m_NumberComboImage_3[9].SetClipRect(37, 169, 63, 209);

	// "/"
	m_SlashImage.Create(0, 0, 7, 18, &g_MainInterfaceTextureArchive, resourceindex_2);
	m_SlashImage.SetClipRect(138, 225, 145, 243);

	// 연
	m_ComboImage.Create(0, 0, 51, 54, &g_MainInterfaceTextureArchive, resourceindex_4);
	m_ComboImage.SetClipRect(9, 62, 60, 116);

	if(!LoadMeditationRecipe())
	{
#ifdef _XDWDEBUG
		_XFatalError("Error : Load Meditation Recipe");
#endif
		return FALSE;
	}
	
	SetWindowMoveMode(_XWMOVE_FIXED);

	return TRUE;
}

void _XWindow_TrainingMed::DestroyWindow(void)
{
	ReleaseRecipe();
	_XWindow::DestroyWindow();
}

void _XWindow_TrainingMed::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	// Success
	if(m_ComboPoint >= 2)
	{
		// Combo
		DWORD comboelapsedtime = g_LocalSystemTime - m_ComboImageStartTime;
		if(comboelapsedtime < 500)
		{
			if(comboelapsedtime <= 100)
			{
				if(m_ComboCount[0] != 0)
				{
					m_NumberComboImage_1[m_ComboCount[0]].SetScale((2.0f-comboelapsedtime*(1/100.f)), (2.0f-comboelapsedtime*(1/100.f)));
					m_NumberComboImage_2[m_ComboCount[1]].SetScale((2.0f-comboelapsedtime*(1/100.f)), (2.0f-comboelapsedtime*(1/100.f)));
					m_NumberComboImage_3[m_ComboCount[2]].SetScale((2.0f-comboelapsedtime*(1/100.f)), (2.0f-comboelapsedtime*(1/100.f)));
					
					SIZE size_1 = m_NumberComboImage_1[m_ComboCount[0]].GetWindowSize();
					SIZE size_2 = m_NumberComboImage_2[m_ComboCount[1]].GetWindowSize();
					SIZE size_3 = m_NumberComboImage_3[m_ComboCount[2]].GetWindowSize();
					size_1.cx *= 2.0f-comboelapsedtime*(1/100.f);
					size_1.cy *= 2.0f-comboelapsedtime*(1/100.f);
					size_2.cx *= 2.0f-comboelapsedtime*(1/100.f);
					size_2.cy *= 2.0f-comboelapsedtime*(1/100.f);
					size_3.cx *= 2.0f-comboelapsedtime*(1/100.f);
					size_3.cy *= 2.0f-comboelapsedtime*(1/100.f);
					
					m_NumberComboImage_1[m_ComboCount[0]].MoveWindow((m_WindowPosition.x+840)-(size_1.cx/2), (m_WindowPosition.y+71)-(size_1.cy/2));
					m_NumberComboImage_2[m_ComboCount[1]].MoveWindow((m_WindowPosition.x+840+26)-(size_2.cx/2), (m_WindowPosition.y+71)-(size_2.cy/2));
					m_NumberComboImage_3[m_ComboCount[2]].MoveWindow((m_WindowPosition.x+840+26+26)-(size_3.cx/2), (m_WindowPosition.y+71)-(size_3.cy/2));
				}
				else if(m_ComboCount[1] != 0)
				{
					m_NumberComboImage_2[m_ComboCount[1]].SetScale((2.0f-comboelapsedtime*(1/100.f)), (2.0f-comboelapsedtime*(1/100.f)));
					m_NumberComboImage_3[m_ComboCount[2]].SetScale((2.0f-comboelapsedtime*(1/100.f)), (2.0f-comboelapsedtime*(1/100.f)));
					
					SIZE size_2 = m_NumberComboImage_2[m_ComboCount[1]].GetWindowSize();
					SIZE size_3 = m_NumberComboImage_3[m_ComboCount[2]].GetWindowSize();
					size_2.cx *= 2.0f-comboelapsedtime*(1/100.f);
					size_2.cy *= 2.0f-comboelapsedtime*(1/100.f);
					size_3.cx *= 2.0f-comboelapsedtime*(1/100.f);
					size_3.cy *= 2.0f-comboelapsedtime*(1/100.f);
					
					m_NumberComboImage_2[m_ComboCount[1]].MoveWindow((m_WindowPosition.x+840+26)-(size_2.cx/2), (m_WindowPosition.y+71)-(size_2.cy/2));
					m_NumberComboImage_3[m_ComboCount[2]].MoveWindow((m_WindowPosition.x+840+26+26)-(size_3.cx/2), (m_WindowPosition.y+71)-(size_3.cy/2));
				}
				else
				{
					m_NumberComboImage_3[m_ComboCount[2]].SetScale((2.0f-comboelapsedtime*(1/100.f)), (2.0f-comboelapsedtime*(1/100.f)));
					
					SIZE size_3 = m_NumberComboImage_3[m_ComboCount[2]].GetWindowSize();
					size_3.cx *= 2.0f-comboelapsedtime*(1/100.f);
					size_3.cy *= 2.0f-comboelapsedtime*(1/100.f);
					
					m_NumberComboImage_3[m_ComboCount[2]].MoveWindow((m_WindowPosition.x+840+26+26)-(size_3.cx/2), (m_WindowPosition.y+71)-(size_3.cy/2));
				}
			}
			else if(comboelapsedtime >= 300)
			{
				m_ComboAlphaValue -= 5;
			}
			
			if(m_ComboAlphaValue <= 0)
				m_ComboAlphaValue = 0;
			
			if(m_ComboCount[0] != 0)
			{
				m_NumberComboImage_1[m_ComboCount[0]].m_FColor = D3DCOLOR_ARGB(m_ComboAlphaValue, 255, 255, 255);
				m_NumberComboImage_2[m_ComboCount[1]].m_FColor = D3DCOLOR_ARGB(m_ComboAlphaValue, 255, 255, 255);
				m_NumberComboImage_3[m_ComboCount[2]].m_FColor = D3DCOLOR_ARGB(m_ComboAlphaValue, 255, 255, 255);
				
				m_NumberComboImage_1[m_ComboCount[0]].Draw();
				m_NumberComboImage_2[m_ComboCount[1]].Draw();
				m_NumberComboImage_3[m_ComboCount[2]].Draw();
			}
			else if(m_ComboCount[1] != 0)
			{
				m_NumberComboImage_2[m_ComboCount[1]].m_FColor = D3DCOLOR_ARGB(m_ComboAlphaValue, 255, 255, 255);
				m_NumberComboImage_3[m_ComboCount[2]].m_FColor = D3DCOLOR_ARGB(m_ComboAlphaValue, 255, 255, 255);
				
				m_NumberComboImage_2[m_ComboCount[1]].Draw();
				m_NumberComboImage_3[m_ComboCount[2]].Draw();
			}
			else
			{
				m_NumberComboImage_3[m_ComboCount[2]].m_FColor = D3DCOLOR_ARGB(m_ComboAlphaValue, 255, 255, 255);
				
				m_NumberComboImage_3[m_ComboCount[2]].Draw();
			}
			
			m_ComboImage.m_FColor = D3DCOLOR_ARGB(m_ComboAlphaValue, 255, 255, 255);
			m_ComboImage.Draw(m_WindowPosition.x+911, m_WindowPosition.y+44);
		}
	}
	else
	{
		DWORD successelapsedtime = g_LocalSystemTime - m_SuccessImageStartTime;
		if(successelapsedtime < 500)
		{
			if(successelapsedtime <= 100)
			{
				m_SuccessImage.SetScale((2.0f-successelapsedtime*(1/100.f)), (2.0f-successelapsedtime*(1/100.f)));
				SIZE size = m_SuccessImage.GetWindowSize();
				size.cx *= 2.0f-successelapsedtime*(1/100.f);
				size.cy *= 2.0f-successelapsedtime*(1/100.f);
				m_SuccessImage.MoveWindow((m_WindowPosition.x+909)-(size.cx/2), (m_WindowPosition.y+76)-(size.cy/2)); 
			}
			else if(successelapsedtime >= 200)
			{
				m_SuccessAlphaValue -= 5;
			}
			
			if(m_SuccessAlphaValue <= 0)
				m_SuccessAlphaValue = 0;
			
			m_SuccessImage.m_FColor = D3DCOLOR_ARGB(m_SuccessAlphaValue, 255, 255, 255);
			m_SuccessImage.Draw();
		}
	}
	

	// Fail
	DWORD failelapsedtime = g_LocalSystemTime - m_FailImageStartTime;
	if(failelapsedtime < 500)
	{
		if(failelapsedtime <= 100)
		{
			m_FailImage.SetScale((2.0f-failelapsedtime*(1/100.f)), (2.0f-failelapsedtime*(1/100.f)));
			SIZE size = m_FailImage.GetWindowSize();
			size.cx *= 2.0f-failelapsedtime*(1/100.f);
			size.cy *= 2.0f-failelapsedtime*(1/100.f);
			m_FailImage.MoveWindow((m_WindowPosition.x+909)-(size.cx/2), (m_WindowPosition.y+76)-(size.cy/2)); 
		}
		else if(failelapsedtime >= 200)
		{
			m_FailAlphaValue -= 5;
		}
		
		if(m_FailAlphaValue <= 0)
			m_FailAlphaValue = 0;
		
		m_FailImage.m_FColor = D3DCOLOR_ARGB(m_FailAlphaValue, 255, 255, 255);
		m_FailImage.Draw();
	}

	// Count
	m_SlashImage.Draw(m_WindowPosition.x+946,  m_WindowPosition.y+265);
	if(m_RestCount[0] != 0)
	{
		m_NumberBigImage[m_RestCount[0]].Draw(m_WindowPosition.x+843, m_WindowPosition.y+234);
		m_NumberBigImage[m_RestCount[1]].Draw(m_WindowPosition.x+843+32, m_WindowPosition.y+234);
		m_NumberBigImage[m_RestCount[2]].Draw(m_WindowPosition.x+843+32+32, m_WindowPosition.y+234);
	}
	else if(m_RestCount[1] != 0)
	{
		m_NumberBigImage[m_RestCount[1]].Draw(m_WindowPosition.x+843+32, m_WindowPosition.y+234);
		m_NumberBigImage[m_RestCount[2]].Draw(m_WindowPosition.x+843+32+32, m_WindowPosition.y+234);
	}
	else
	{
		m_NumberBigImage[m_RestCount[2]].Draw(m_WindowPosition.x+843+32+32, m_WindowPosition.y+234);
	}
	if(m_TotalCount[0] != 0)
	{
		m_NumberSmallImage[m_TotalCount[0]].Draw(m_WindowPosition.x+955, m_WindowPosition.y+266);
		m_NumberSmallImage[m_TotalCount[1]].Draw(m_WindowPosition.x+955+11, m_WindowPosition.y+266);
		m_NumberSmallImage[m_TotalCount[2]].Draw(m_WindowPosition.x+955+11+11, m_WindowPosition.y+266);
	}
	else if(m_TotalCount[1] != 0)
	{
		m_NumberSmallImage[m_TotalCount[1]].Draw(m_WindowPosition.x+955, m_WindowPosition.y+266);
		m_NumberSmallImage[m_TotalCount[2]].Draw(m_WindowPosition.x+955+11, m_WindowPosition.y+266);
	}
	else
	{
		m_NumberSmallImage[m_TotalCount[2]].Draw(m_WindowPosition.x+955, m_WindowPosition.y+266);
	}

	SIZE size;
	int x = 0, y = 0;
	RECT rect;
	for(int i = 0 ; i < m_PathCount[m_MeditationStep] ; i++)
	{
		m_BorderImage->GetClipRect(rect);
		size = m_Point[m_Path[m_MeditationStep][i].type].GetWindowSize();
		x = m_WindowPosition.x+107+m_Path[m_MeditationStep][i].x-(size.cx/2);
		y = m_WindowPosition.y+m_Path[m_MeditationStep][i].y-(size.cy/2) - (rect.top*1.582f);

		if((x-size.cx/2) >= m_WindowPosition.x && (x+size.cx) <= m_WindowPosition.x+m_WindowSize.cx &&
			(y-size.cy/2) >= m_WindowPosition.y && (y+size.cy) <= m_WindowPosition.y+m_WindowSize.cy)
		{
			if(m_Path[m_MeditationStep][i].bDuplicate)
			{
				if(m_Path[m_MeditationStep][i].dupIndex < m_MousePositionIndex-1)
				{
					m_Point[m_Path[m_MeditationStep][i].type].Draw(x, y); 
				} 
				else
				{
					// 그리지 않음
				}

				if(m_Path[m_MeditationStep][i].bSuccess)
				{
					DrawSuccessImage(i, x, y);
				}
				else if(m_Path[m_MeditationStep][i].bComplete)	// Success Animation이 모두 끝난후 부터 Draw
				{
					DrawCompleteImage(i, x, y);	
				}
			}
			else
			{
				if(m_Path[m_MeditationStep][i].bSuccess)
				{
					DrawSuccessImage(i, x, y);
				}
				else if(m_Path[m_MeditationStep][i].bComplete) // Success Animation이 모두 끝난후 부터 Draw
				{
					// Complete Image
					DrawCompleteImage(i, x, y);
				}
				else
				{
					// Normal
					m_Point[m_Path[m_MeditationStep][i].type].Draw(x, y);
				}
			}

			if(i == m_MousePositionIndex)
			{
				if(m_bFail)
				{
					DrawFailEffect(x, y);
				}
				else
				{
					DrawMousePositionImage(x, y);
/*					if(m_Path[m_MeditationStep][m_MousePositionIndex].type < 12)
					{
						m_MousePositionImage[0].Draw(x-2, y-2);
					}
					else
					{
						m_MousePositionImage[1].Draw(x-3, y-3);
					}*/
				}
			}
		}
	} // for

#ifdef _XDWDEBUG
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 180, 184, 187));
	g_XBaseFont->Print(m_WindowPosition.x+26, m_WindowPosition.y+140, 1.0f, "index:%d type:%d dup:%d", m_MousePositionIndex, 
		m_Path[m_MeditationStep][m_MousePositionIndex].type, m_Path[m_MeditationStep][m_MousePositionIndex].dupIndex);
	g_XBaseFont->Flush();
#endif

	// Font
	int level = 0;
	if(m_bQuestMiniGameStart)
		level = g_pLocalUser->m_CharacterInfo.Get_level();
	else 
		level = g_pLocalUser->m_CharacterInfo.Get_level()+1;

	TCHAR messagestring[64];
	sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_2666), level);
	g_XLargeFont->SetColor(D3DCOLOR_ARGB(255, 255, 254, 255));
	g_XLargeFont->Puts(m_WindowPosition.x+21, m_WindowPosition.y+27, messagestring);
	g_XLargeFont->Puts(m_WindowPosition.x+21, m_WindowPosition.y+98, g_LevelName_CH[level]);
	g_XLargeFont->SetGrowMode(_XFONT_EDGEMODE_GLOW, 0xFF000000);
	g_XLargeFont->Flush();
	g_XLargeFont->DisableGrowMode();

	sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_STEOJIREA), level);		
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 180, 184, 187));
	g_XBaseFont->Puts(m_WindowPosition.x+26, m_WindowPosition.y+60, messagestring);
	sprintf(messagestring, "(%s)", g_LevelName[level]);
	g_XBaseFont->Puts(m_WindowPosition.x+26, m_WindowPosition.y+124, messagestring);
	g_XBaseFont->Flush();

	DrawGuide();
}

BOOL _XWindow_TrainingMed::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	// TODO
	if(m_bCameraMoveStart)
	{
		if(m_bEndCameraRotate)
			ZoomIn();
		else
			CameraMove();
	}

	if(m_bEndCameraZoomIn)
		m_bTrainingStart = TRUE;

	if(m_bTrainingStart)
	{
		RECT rect;
		m_BorderImage->GetClipRect(rect);
		if(rect.top != m_ModifiedYPosition - 95)
		{
			if(rect.top < m_ModifiedYPosition - 95)
			{
				rect.top++;
				rect.bottom++;
				m_BorderImage->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
			}
			else if(rect.top > m_ModifiedYPosition - 95)
			{
				rect.top--;
				rect.bottom--;
				m_BorderImage->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
			}
		}

		MouseState* mousestate = gpInput->GetMouseState();
		if(mousestate)
		{
			if(!pfocusobject)
			{
				if(mousestate->bButton[0] && !m_bLeftClick)
				{
					m_bLeftClick = TRUE;
					if(!m_bStartGame)
						m_bStartGame = TRUE;

					if(!m_bFail)
					{
						if(m_PointClickType[m_Path[m_MeditationStep][m_MousePositionIndex].type] == 1)
						{
							// Success
							m_Path[m_MeditationStep][m_MousePositionIndex].bSuccess = TRUE;
							DecideSuccess();

							m_SuccessAlphaValue = 255;
							m_SuccessImageStartTime = g_LocalSystemTime;
							PlaySuccessSound();
						}
						else if(m_PointClickType[m_Path[m_MeditationStep][m_MousePositionIndex].type] == 3)
						{
							// 판단 보류
						}
						else
						{
							// FAIL
							m_bFail = TRUE;
							m_FailAlphaValue = 255;
							m_FailImageStartTime = g_LocalSystemTime;

							_XPlayInterfaceSound(ID_SR_INTERFACE_MISS_01_WAV);
						}
					}
					else
					{
						// Fail effect 출력중일때는 마우스 입력 받지 않음
					}
				}
				if(mousestate->bButton[1] && !m_bRightClick)
				{
					m_bRightClick = TRUE;
					if(!m_bStartGame)
						m_bStartGame = TRUE;

					if(!m_bFail)
					{
						if(m_PointClickType[m_Path[m_MeditationStep][m_MousePositionIndex].type] == 2)
						{
							// Success
							m_Path[m_MeditationStep][m_MousePositionIndex].bSuccess = TRUE;
							DecideSuccess();

							m_SuccessAlphaValue = 255;
							m_SuccessImageStartTime = g_LocalSystemTime;
							PlaySuccessSound();
						}
						else if(m_PointClickType[m_Path[m_MeditationStep][m_MousePositionIndex].type] == 3 && m_bLeftClick)
						{
							// Mouse button 두개 눌림
							m_Path[m_MeditationStep][m_MousePositionIndex].bSuccess = TRUE;
							DecideSuccess();

							m_SuccessAlphaValue = 255;
							m_SuccessImageStartTime = g_LocalSystemTime;
							PlaySuccessSound();
						}
						else
						{
							// FAIL
							m_bFail = TRUE;
							m_FailAlphaValue = 255;
							m_FailImageStartTime = g_LocalSystemTime;

							_XPlayInterfaceSound(ID_SR_INTERFACE_MISS_01_WAV);
						}
					}
					else
					{
						// Fail effect 출력중일때는 마우스 입력 받지 않음
					}
				}
			}
			if(m_bLeftClick && !mousestate->bButton[0])
			{
				m_bLeftClick = FALSE;
			}
			else if(m_bRightClick && !mousestate->bButton[1])
			{
				m_bRightClick = FALSE;
			}
		}

		if(m_SendPacketTime == 0)
			m_SendPacketTime = g_LocalSystemTime;

		if(!m_bCompleteClient)
		{
			if(g_LocalSystemTime - m_SendPacketTime > 2000)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MINIGAME_MAST_STAGE_C_S, m_MissionStage);
				m_SendPacketTime = g_LocalSystemTime;
			}
		}
	} // if(m_bTrainingStart)

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

BOOL _XWindow_TrainingMed::LoadMeditationRecipe(void)
{
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	FILE* fileptr = NULL;

	fileptr = g_ScriptArchive.GetPackedFile(_T("Meditation_info.XMS"));

	if(!fileptr)
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	int			step = 0, count = 0, i = 0;
	BOOL		bReadPos = FALSE;

	TCHAR		buffer[256];
	TCHAR		arg1[128], arg2[128];
	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(TCHAR)*256);
		if(!fgets(buffer, 255, fileptr))
		{
			break;
		}

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;

		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if(bReadPos)
			{
				sscanf(buffer, "%d %d %d", &m_Path[step-1][i].x, &m_Path[step-1][i].y, &m_Path[step-1][i].type);
				m_Path[step-1][i].type--;	// 0 base로 변환
				i++;

				if(i == count)
					bReadPos = FALSE;
			}
			else
			{
				memset(arg1, 0, sizeof(arg1));
				memset(arg2, 0, sizeof(arg2));

				sscanf(buffer, "%s %s", arg1, arg2);

				if(strncmp(arg1, "_XM_STEP", 9) == 0)
				{
					step = atoi(arg2);
					bReadPos = FALSE;
				}
				if(strncmp(arg1, "_XM_COUNT", 9) == 0)
				{
					count = atoi(arg2);
					bReadPos = TRUE;

					if(step <= 0)
						return FALSE;

					m_Path[step-1] = new _XMD_Unit[count];
					m_PathCount[step-1] = count;
					i = 0;
				}
			}
		}

	} while(!feof(fileptr));

	if(step > 0)
		m_LastStep = step-1;
	else 
		m_LastStep = 0;

	g_ScriptArchive.ClosePackage();
	return TRUE;
}

void _XWindow_TrainingMed::ReleaseRecipe(void)
{
	for(int i = 0 ; i < 20 ; i++)
	{
		SAFE_DELETE_ARRAY(m_Path[i]);
	}
}

void _XWindow_TrainingMed::ShowWindow(BOOL show)
{
	if(show)
	{
		m_MousePositionIndex = 0;//m_PathCount[m_MeditationStep]-1;
		ParseDuplicateIndex();

		SetMedImagePosition();
		m_BorderImage->SetClipRect(0, m_ModifiedYPosition-95, 512, m_ModifiedYPosition+95);

		if(m_PathCount[m_MeditationStep] >= 100)
		{
			m_TotalCount[0] = m_PathCount[m_MeditationStep] / 100;
			m_TotalCount[1] = (m_PathCount[m_MeditationStep] - 100) / 10;
			m_TotalCount[2] = (m_PathCount[m_MeditationStep] - 100) % 10;
		}
		else
		{
			m_TotalCount[0] = 0;
			m_TotalCount[1] = m_PathCount[m_MeditationStep] / 10;
			m_TotalCount[2] = m_PathCount[m_MeditationStep] % 10;
		}

		m_RestCount[0] = m_TotalCount[0];
		m_RestCount[1] = m_TotalCount[1];
		m_RestCount[2] = m_TotalCount[2];

		CloseInterface();
	}
	else
	{
		if(m_bStartGame)
		{
			Reset();
			OpenInterface();
		}
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_TrainingMed::SetMedImagePosition(void)
{
	m_ModifiedYPosition = m_Path[m_MeditationStep][m_MousePositionIndex].y / 1.582f;
	if(m_ModifiedYPosition <= 95)
		m_ModifiedYPosition = 95;
}

void _XWindow_TrainingMed::ParseDuplicateIndex(void)
{
	for(int i = 0 ; i < m_PathCount[m_MeditationStep] ; i++)
	{
		m_Path[m_MeditationStep][i].dupIndex = 0;
		m_Path[m_MeditationStep][i].prevDrawTime = 0;
		m_Path[m_MeditationStep][i].effectFrame = 0.0f;

		m_Path[m_MeditationStep][i].bDuplicate = FALSE;
		m_Path[m_MeditationStep][i].bComplete = FALSE;
		m_Path[m_MeditationStep][i].bSuccess = FALSE;
		m_Path[m_MeditationStep][i].bEffectOrderSub = FALSE;
	}

	for(i = 0 ; i < m_PathCount[m_MeditationStep] ; i++)
	{
		if(m_Path[m_MeditationStep][i].type >= 3 && m_Path[m_MeditationStep][i].type <= 11)
		{
			for(int j = i+1 ; j < m_PathCount[m_MeditationStep] ; j++)
			{
				if(m_Path[m_MeditationStep][j].x == m_Path[m_MeditationStep][i].x &&
					m_Path[m_MeditationStep][j].y == m_Path[m_MeditationStep][i].y)
				{
					if(i != j)
					{
						m_Path[m_MeditationStep][j].bDuplicate = TRUE;
						m_Path[m_MeditationStep][j].dupIndex = i;
					}
				}
			}
		} // 3 <= type <= 11
	}
}

void _XWindow_TrainingMed::DecideSuccess(void)
{
	// MousePosition 옮기기
	if(m_MousePositionIndex == m_PathCount[m_MeditationStep]-1)
	{
		// End Game!!
		if(m_bStartGame)
		{
			m_Path[m_MeditationStep][m_MousePositionIndex].bComplete = TRUE;
			g_NetworkKernel.SendPacket(MSG_NO_MINIGAME_MAST_STAGE_C_S, 0);
			m_bCompleteClient = TRUE;
		}
	}
	else
	{
		m_Path[m_MeditationStep][m_MousePositionIndex].bComplete = TRUE;
		m_MousePositionIndex++;

		m_ComboPoint++;
		if(m_ComboPoint >= 100)
		{
			m_ComboCount[0] = m_ComboPoint / 100;
			m_ComboCount[1] = (m_ComboPoint - 100) / 10;
			m_ComboCount[2] = (m_ComboPoint - 100) % 10;
		}
		else
		{
			m_ComboCount[0] = 0;
			m_ComboCount[1] = m_ComboPoint / 10;
			m_ComboCount[2] = m_ComboPoint % 10;
		}
		if(m_ComboPoint >= 2)
		{
			m_ComboAlphaValue = 255;
			m_ComboImageStartTime = g_LocalSystemTime;
		}

		int restcount = m_PathCount[m_MeditationStep] - m_MousePositionIndex;
		if(restcount >= 100)
		{
			m_RestCount[0] = restcount / 100;
			m_RestCount[1] = (restcount - 100) / 10;
			m_RestCount[2] = (restcount - 100) % 10;
		}
		else
		{
			m_RestCount[0] = 0;
			m_RestCount[1] = restcount / 10;
			m_RestCount[2] = restcount % 10;
		}

		m_SuccessImage.MoveWindow(gnWidth, m_WindowPosition.x+31);
		m_SuccessImagePosition.x = 5;
		m_SuccessImagePosition.y = 31;
	}
	SetMedImagePosition();
}

void _XWindow_TrainingMed::DecideFail(void)
{
//	m_bFail = TRUE;
	m_ComboPoint = 0;
}

void _XWindow_TrainingMed::DrawSuccessImage(int index, int x, int y)
{
	if(m_Path[m_MeditationStep][index].prevDrawTime == 0)
		m_Path[m_MeditationStep][index].prevDrawTime = g_LocalSystemTime;
	
	DWORD elapsedtime = g_LocalSystemTime - m_Path[m_MeditationStep][index].prevDrawTime;
	
	if(m_Path[m_MeditationStep][index].type >= 0 && m_Path[m_MeditationStep][index].type <= 11)
	{
		// 작은 원
		m_Path[m_MeditationStep][index].effectFrame += ((FLOAT)elapsedtime / 100.0f);
		
		if(m_Path[m_MeditationStep][index].effectFrame > 6.0f)
		{
			m_Path[m_MeditationStep][index].effectFrame = 0.0f;
			m_Path[m_MeditationStep][index].prevDrawTime = 0;
			m_Path[m_MeditationStep][index].bSuccess = FALSE;
			return;
		}
		
		int selectedframe = (int)m_Path[m_MeditationStep][index].effectFrame;
		if(selectedframe > 5)
			selectedframe = 5;
		
		switch(m_Path[m_MeditationStep][index].type)
		{
		case 0 :
		case 3 :
		case 4 :
		case 5 :
			{
				m_SuccessYangEffectImage[selectedframe].Draw(x, y);
			}
			break;
		case 1 :
		case 6 :
		case 7 :
		case 8 :
			{
				m_SuccessUmEffectImage[selectedframe].Draw(x, y);
			}
			break;
		case 2 :
		case 9 :
		case 10 :
		case 11 :
			{
				m_SuccessMuEffectImage[selectedframe].Draw(x, y);
			}
			break;
		}
	}
	else
	{
		// 큰 원
		if(!m_Path[m_MeditationStep][index].bEffectOrderSub)
			m_Path[m_MeditationStep][index].effectFrame += ((FLOAT)elapsedtime / 100.0f);
		else
			m_Path[m_MeditationStep][index].effectFrame -= ((FLOAT)elapsedtime / 100.0f);
		
		if(m_Path[m_MeditationStep][index].effectFrame > 5.0f)
		{
			m_Path[m_MeditationStep][index].bEffectOrderSub = TRUE;
			m_Path[m_MeditationStep][index].effectFrame = 4.0f;
		}
		else if(m_Path[m_MeditationStep][index].effectFrame < 0.0f)
		{
			m_Path[m_MeditationStep][index].effectFrame = 0.0f;
			m_Path[m_MeditationStep][index].prevDrawTime = 0;
			m_Path[m_MeditationStep][index].bEffectOrderSub = FALSE;
			m_Path[m_MeditationStep][index].bSuccess = FALSE;
			return;
		}
		
		int selectedframe = (int)m_Path[m_MeditationStep][index].effectFrame;
		if(selectedframe > 4)
			selectedframe = 4;
		
		m_SuccessBigEffectImage[selectedframe].Draw(x-2, y-1);
	}
	
	m_Path[m_MeditationStep][index].prevDrawTime = g_LocalSystemTime;
}

void _XWindow_TrainingMed::DrawFailEffect(int x, int y)
{
	if(m_PrevDrawTime == 0)
		m_PrevDrawTime = g_LocalSystemTime;
	
	DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
	
	if(m_Path[m_MeditationStep][m_MousePositionIndex].type >= 0 && m_Path[m_MeditationStep][m_MousePositionIndex].type <= 11)
	{
		// 작은 원
		if(!m_bEffectOrderSub)
			m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
		else
			m_EffectFrame -= ((FLOAT)elapsedtime / 100.0f);
		
		if(m_EffectFrame > 5.0f)
		{
			m_bEffectOrderSub = TRUE;
			m_EffectFrame = 3.0f;
		}
		else if(m_EffectFrame < 0.0f)
		{
			m_EffectFrame = 0.0f;
			m_PrevDrawTime = 0;
			m_bEffectOrderSub = FALSE;
			m_bFail = FALSE;
			DecideFail();
			return;
		}
		
		int selectedframe = (int)m_EffectFrame;
		if(selectedframe > 3)
			selectedframe = 3;
		
		switch(selectedframe)
		{
		case 0 :
			// Alpha = 0%
			m_FailEffectImage[0].m_FColor = D3DCOLOR_ARGB(0, 255, 255, 255);
			break;
		case 1 :
			// Alpha = 30%
			m_FailEffectImage[0].m_FColor = D3DCOLOR_ARGB(76, 255, 255, 255);
			break;
		case 2 :
			// Alpha = 60%
			m_FailEffectImage[0].m_FColor = D3DCOLOR_ARGB(153, 255, 255, 255);
			break;
		case 3 :
			// Alpha = 100%
			m_FailEffectImage[0].m_FColor = D3DCOLOR_ARGB(255, 255, 255, 255);
			break;
		}
		
		m_FailEffectImage[0].Draw(x-3, y-3);
	}
	else
	{
		// 큰 원
		if(!m_bEffectOrderSub)
			m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
		else
			m_EffectFrame -= ((FLOAT)elapsedtime / 100.0f);
		
		if(m_EffectFrame > 5.0f)
		{
			m_bEffectOrderSub = TRUE;
			m_EffectFrame = 3.0f;
		}
		else if(m_EffectFrame < 0.0f)
		{
			m_EffectFrame = 0.0f;
			m_PrevDrawTime = 0;
			m_bEffectOrderSub = FALSE;
			m_bFail = FALSE;
			DecideFail();
			return;
		}
		
		int selectedframe = (int)m_EffectFrame;
		if(selectedframe > 3)
			selectedframe = 3;

		switch(selectedframe)
		{
		case 0 :
			// Alpha = 0%
			m_FailEffectImage[1].m_FColor = D3DCOLOR_ARGB(0, 255, 255, 255);
			break;
		case 1 :
			// Alpha = 30%
			m_FailEffectImage[1].m_FColor = D3DCOLOR_ARGB(76, 255, 255, 255);
			break;
		case 2 :
			// Alpha = 60%
			m_FailEffectImage[1].m_FColor = D3DCOLOR_ARGB(153, 255, 255, 255);
			break;
		case 3 :
			// Alpha = 100%
			m_FailEffectImage[1].m_FColor = D3DCOLOR_ARGB(255, 255, 255, 255);
			break;
		}
		
		m_FailEffectImage[1].Draw(x-3, y-3);
	}
	
	m_PrevDrawTime = g_LocalSystemTime;
}

void _XWindow_TrainingMed::DrawMousePositionImage(int x, int y)
{
	if(m_PrevDrawTime == 0)
		m_PrevDrawTime = g_LocalSystemTime;
	
	DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
	
	if(m_Path[m_MeditationStep][m_MousePositionIndex].type >= 0 && m_Path[m_MeditationStep][m_MousePositionIndex].type <= 11)
	{
		// 작은 원
		if(!m_bEffectOrderSub)
			m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
		else
			m_EffectFrame -= ((FLOAT)elapsedtime / 100.0f);
		
		if(m_EffectFrame > 5.0f)
		{
			m_bEffectOrderSub = TRUE;
			m_EffectFrame = 3.0f;
		}
		else if(m_EffectFrame < 0.0f)
		{
			m_EffectFrame = 0.0f;
			m_PrevDrawTime = 0;
			m_bEffectOrderSub = FALSE;
			return;
		}
		
		int selectedframe = (int)m_EffectFrame;
		if(selectedframe > 3)
			selectedframe = 3;
		
		switch(selectedframe)
		{
		case 0 :
			// Alpha = 0%
			m_MousePositionImage[0].m_FColor = D3DCOLOR_ARGB(0, 255, 255, 255);
			break;
		case 1 :
			// Alpha = 30%
			m_MousePositionImage[0].m_FColor = D3DCOLOR_ARGB(76, 255, 255, 255);
			break;
		case 2 :
			// Alpha = 60%
			m_MousePositionImage[0].m_FColor = D3DCOLOR_ARGB(153, 255, 255, 255);
			break;
		case 3 :
			// Alpha = 100%
			m_MousePositionImage[0].m_FColor = D3DCOLOR_ARGB(255, 255, 255, 255);
			break;
		}
		
		m_MousePositionImage[0].Draw(x-2, y-2);
	}
	else
	{
		// 큰 원
		if(!m_bEffectOrderSub)
			m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
		else
			m_EffectFrame -= ((FLOAT)elapsedtime / 100.0f);
		
		if(m_EffectFrame > 5.0f)
		{
			m_bEffectOrderSub = TRUE;
			m_EffectFrame = 3.0f;
		}
		else if(m_EffectFrame < 0.0f)
		{
			m_EffectFrame = 0.0f;
			m_PrevDrawTime = 0;
			m_bEffectOrderSub = FALSE;
			return;
		}
		
		int selectedframe = (int)m_EffectFrame;
		if(selectedframe > 3)
			selectedframe = 3;

		switch(selectedframe)
		{
		case 0 :
			// Alpha = 0%
			m_MousePositionImage[1].m_FColor = D3DCOLOR_ARGB(0, 255, 255, 255);
			break;
		case 1 :
			// Alpha = 30%
			m_MousePositionImage[1].m_FColor = D3DCOLOR_ARGB(76, 255, 255, 255);
			break;
		case 2 :
			// Alpha = 60%
			m_MousePositionImage[1].m_FColor = D3DCOLOR_ARGB(153, 255, 255, 255);
			break;
		case 3 :
			// Alpha = 100%
			m_MousePositionImage[1].m_FColor = D3DCOLOR_ARGB(255, 255, 255, 255);
			break;
		}
		
		m_MousePositionImage[1].Draw(x-3, y-3);
	}
	
	m_PrevDrawTime = g_LocalSystemTime;
}

void _XWindow_TrainingMed::DrawCompleteImage(int index, int x, int y)
{
	if(m_Path[m_MeditationStep][index].prevDrawTime ==0)
	{
		m_Path[m_MeditationStep][index].prevDrawTime = g_LocalSystemTime;
	}
	
	DWORD elapsedtime = g_LocalSystemTime - m_Path[m_MeditationStep][index].prevDrawTime;
	
	if(m_Path[m_MeditationStep][index].type >= 0 && m_Path[m_MeditationStep][index].type <= 11)
	{
		// 작은 원
		if(elapsedtime > 300)
		{
			if(m_Path[m_MeditationStep][index].bEffectOrderSub == FALSE)
			{
				// Alpha = 80%로 변경
				m_CompleteEffectImage.m_FColor = D3DCOLOR_ARGB(0, 255, 255, 255);
				m_Path[m_MeditationStep][index].bEffectOrderSub = TRUE;
				m_Path[m_MeditationStep][index].prevDrawTime = g_LocalSystemTime;
			}
			else if(m_Path[m_MeditationStep][index].bEffectOrderSub == TRUE)
			{
				// Alpha = 100%로 변경
				m_CompleteEffectImage.m_FColor = D3DCOLOR_ARGB(255, 255, 255, 255);
				m_Path[m_MeditationStep][index].bEffectOrderSub = FALSE;
				m_Path[m_MeditationStep][index].prevDrawTime = g_LocalSystemTime;
			}
		}

		switch(m_Path[m_MeditationStep][index].type)
		{
		case 0 :
		case 3 :
		case 4 :
		case 5 :
			{
				m_SuccessYangEffectImage[5].Draw(x, y);
			}
			break;
		case 1 :
		case 6 :
		case 7 :
		case 8 :
			{
				m_SuccessUmEffectImage[5].Draw(x, y);
			}
			break;
		case 2 :
		case 9 :
		case 10 :
		case 11 :
			{
				m_SuccessMuEffectImage[5].Draw(x, y);
			}
			break;
		}
		
		m_CompleteEffectImage.Draw(x+2, y+2);
	}
	else
	{
		// 큰원
		m_Point[m_Path[m_MeditationStep][index].type].Draw(x, y);
	}
}

void _XWindow_TrainingMed::Reset(void)
{
	m_MeditationStep = 0;
	m_MousePositionIndex = 0;
	
	m_bLeftClick = FALSE;
	m_bRightClick = FALSE;
	
	m_bFail = FALSE;
	
	m_PrevDrawTime = 0;
	m_bEffectOrderSub = FALSE;
	m_EffectFrame = 0.f;
	
	m_bStartGame = FALSE;
	m_PrevCompleteImageDrawTime = 0;
	m_CompleteImageDrawIndex = 0;

	m_SuccessImageStartTime = 0;
	m_FailImageStartTime = 0;
	m_SuccessAlphaValue = 255;
	m_FailAlphaValue = 255;

	memset(m_RestCount, 0, sizeof(m_RestCount));
	memset(m_TotalCount, 0, sizeof(m_TotalCount));

	m_ComboPoint = 0;
	memset(m_ComboCount, 0, sizeof(m_ComboCount));
	m_ComboImageStartTime = 0;
	m_ComboAlphaValue = 255;

	m_bComplete = FALSE;
	m_bCompleteClient = FALSE;

	m_SendPacketTime = 0;
	m_MissionStage = 0;

	m_ModifiedYPosition = 0;
	m_bTrainingStart = FALSE;
	
	m_bEndCameraRotate = FALSE;
	m_bEndCameraZoomIn = FALSE;
	m_bCameraMoveStart = FALSE;
	
	m_bQuestMiniGameStart = FALSE;

	m_fRotateScreenAngel = 0.0f;
}

void _XWindow_TrainingMed::Complete(void)
{
	m_bComplete = TRUE;
	
	if(!m_bQuestMiniGameStart)
		g_NetworkKernel.SendPacket(MSG_NO_LEVEL_UP_REQ);
	else
		m_bQuestMiniGameStart = FALSE;

	_XWindow_TrainingComplete* pTrainingComplete_Window = (_XWindow_TrainingComplete*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGCOMPLETE);
	if(pTrainingComplete_Window)
	{
		pTrainingComplete_Window->SetSelectedImage(1);	// 단계상승
		pTrainingComplete_Window->ShowWindow(TRUE);
	}
}

void _XWindow_TrainingMed::SetMeditationStep(int step)
{
	if(step == 0)
		m_MeditationStep = 0;
	else
		m_MeditationStep = step-1;
	
	if(!m_Path[m_MeditationStep])
	{
		m_MeditationStep = m_LastStep;
	}
}

void _XWindow_TrainingMed::PlaySuccessSound(void)
{
	switch(m_Path[m_MeditationStep][m_MousePositionIndex].type)
	{
	case 0 :
	case 3 :
	case 4 :
	case 5 :
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_WHITE_01_WAV);
		}
		break;
	case 1 :
	case 6 :
	case 7 :
	case 8 :
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_BLACK_01_WAV);
		}
		break;
	case 2 :
	case 9 :
	case 10 :
	case 11 :
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_BLUE_01_WAV);
		}
		break;
	case 14 :
	case 16 :
	case 18 :
	case 22 :
	case 26 :
	case 28 :
	case 30 :
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_KIPOINT_01_WAV);
			_XPlayInterfaceSound(ID_SR_INTERFACE_WHITE_01_WAV);
		}
		break;
	case 13 :
	case 19 :
	case 20 :
	case 23 :
	case 24 :
	case 27 :
	case 31:
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_KIPOINT_01_WAV);
			_XPlayInterfaceSound(ID_SR_INTERFACE_BLACK_01_WAV);
		}
		break;
	case 12 :
	case 15 :
	case 17 :
	case 21 :
	case 25 :
	case 29 :
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_KIPOINT_01_WAV);
			_XPlayInterfaceSound(ID_SR_INTERFACE_BLUE_01_WAV);
		}
		break;
	}
}

#define _XDEF_SCREENDISTANCE 3.0f

void _XWindow_TrainingMed::CameraMove()
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 camerapos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3	dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	
	
	D3DXMATRIX rotmatrix = g_pLocalUser->m_ModelDescriptor.m_Position;
	rotmatrix._41 = 0.0f;
	rotmatrix._42 = 0.0f;
	rotmatrix._43 = 0.0f;
	
	D3DXVec3TransformCoord( &pos, &pos, &rotmatrix );
	dir = pos;
	D3DXVec3Normalize(&dir, &dir);
	camerapos = g_pLocalUser->m_Position + dir * _XDEF_SCREENDISTANCE;	
	
	_XOBBData* pOBB = NULL;	
	camerapos.y = g_LodTerrain.GetTerrainAndOBBHeight(camerapos,pOBB);	
	pos = g_pLocalUser->m_Position;
	pos.y += g_pLocalUser->m_CharacterOBB.m_afExtent[1]*1.8f;
	
	g_LodTerrain.m_3PCamera.SetTargetPosition(pos);	
	
	g_LodTerrain.m_3PCamera.mp_fFov = 40.0f;
	g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
	if( m_fRotateScreenAngel+20.0f < 180.0f )
	{
		m_fRotateScreenAngel += 20.0f;
		g_LodTerrain.m_3PCamera.mp_fYaw = _X_DEG(g_pLocalUser->m_RotateAngle)+m_fRotateScreenAngel;
		
		// reset camera rotate & zoom velocity...
		g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
		g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	
		
		g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		g_LodTerrain.RebuildLevel(FALSE);			
	}
	else
	{
		m_fRotateScreenAngel = 180.0f;	
		g_LodTerrain.m_3PCamera.mp_fYaw = _X_DEG(g_pLocalUser->m_RotateAngle)+180.0f;
		
		// reset camera rotate & zoom velocity...
		g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
		g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	
		
		g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		g_LodTerrain.RebuildLevel(FALSE);			
		m_bEndCameraRotate = TRUE;
	}		
}

void _XWindow_TrainingMed::ZoomIn()
{
	float dis = g_LodTerrain.m_3PCamera.GetDistance();
	if( dis < _XDEF_SCREENDISTANCE )
	{
		g_LodTerrain.m_3PCamera.m_TargetDistance = _XDEF_SCREENDISTANCE;
		m_bEndCameraZoomIn = TRUE;
	}
	
	if( !m_bEndCameraZoomIn )
		g_LodTerrain.m_3PCamera.m_TargetDistance = dis - 0.4f;							
	
	// reset camera rotate & zoom velocity...
	g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
	g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	
	
	g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.RebuildLevel(FALSE);
}

void _XWindow_TrainingMed::CloseInterface(void)
{
	_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
	if(pMainFrame_Window)
	{
		if(pMainFrame_Window->GetShowStatus())
			pMainFrame_Window->ShowWindow(FALSE);
	}

	if(g_pInventory_Window)
	{
		if(g_pInventory_Window->GetShowStatus())
			g_pInventory_Window->ShowWindow(FALSE);
	}
	if(g_pQuickSlot_Window)
	{
		if(g_pQuickSlot_Window->GetShowStatus())
			g_pQuickSlot_Window->ShowWindow(FALSE);
	}
	if(g_pDefaultTooltip_Window)
	{
		if(g_pDefaultTooltip_Window->GetShowStatus())
			g_pDefaultTooltip_Window->ShowWindow(FALSE);
	}
	if(g_pMiniTooltip_Window)
	{
		if(g_pMiniTooltip_Window->GetShowStatus())
			g_pMiniTooltip_Window->ShowWindow(FALSE);
	}
	if(g_pState_Window)
	{
		if(g_pState_Window->GetShowStatus())
			g_pState_Window->ShowWindow(FALSE);
	}

	_XWindow* pWindow = NULL;

	for(int i = _XDEF_WTITLE_MINIMAPWINDOW ; i <= _XDEF_WTITLE_MINIPARTY_MEMBER9 ; i++)
	{
		pWindow = g_MainWindowManager.FindWindow(i);
		if(pWindow)
			pWindow->ShowWindow(FALSE);
	}

	for(i = _XDEF_WTITLE_PCTRADE ; i <= _XDEF_WTITLE_NPCSCRIPT ; i++)
	{
		pWindow = g_MainWindowManager.FindWindow(i);
		if(pWindow)
			pWindow->ShowWindow(FALSE);
	}

	for(i = _XDEF_WTITLE_NPCTRADE ; i <= _XDEF_WTITLE_TRADECONFIRM ; i++)
	{
		pWindow = g_MainWindowManager.FindWindow(i);
		if(pWindow)
			pWindow->ShowWindow(FALSE);
	}

	for(i = _XDEF_WTITLE_REQUESTPARTY ; i <= _XDEF_WTITLE_MESSENGERWINDOW ; i++)
	{
		pWindow = g_MainWindowManager.FindWindow(i);
		if(pWindow)
			pWindow->ShowWindow(FALSE);
	}

	for(i = _XDEF_WTITLE_MONSTERSTATUSWINDOW ; i <= _XDEF_WTITLE_ZONENAMEWINDOW ; i++)
	{
		pWindow = g_MainWindowManager.FindWindow(i);
		if(pWindow)
			pWindow->ShowWindow(FALSE);
	}

	for(i = _XDEF_WTITLE_WAREHOUSE ; i <= _XDEF_WTITLE_NPCSKILL ; i++)
	{
		pWindow = g_MainWindowManager.FindWindow(i);
		if(pWindow)
			pWindow->ShowWindow(FALSE);
	}

	pWindow = g_MainWindowManager.FindWindow(_XDEF_WTITLE_REPAIRITEM);
	if(pWindow)
		pWindow->ShowWindow(FALSE);
	
	pWindow = g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);
	if(pWindow)
		pWindow->ShowWindow(FALSE);

	for(i = _XDEF_WTITLE_NICKNAME ; i <= _XDEF_WTITLE_SOCKETPLUGIN ; i++)
	{
		pWindow = g_MainWindowManager.FindWindow(i);
		if(pWindow)
			pWindow->ShowWindow(FALSE);
	}

	pWindow = g_MainWindowManager.FindWindow(_XDEF_WTITLE_OPTIONMENU);
	if(pWindow)
		pWindow->ShowWindow(FALSE);

	for(i = _XDEF_WTITLE_EMAILWINDOW ; i <= _XDEF_WTITLE_MESSENGERDIALOGWINDOW ; i++)
	{
		pWindow = g_MainWindowManager.FindWindow(i);
		if(pWindow)
			pWindow->ShowWindow(FALSE);
	}
	
	pWindow = g_MainWindowManager.FindWindow(_XDEF_WTITLE_CONTRIBUTIONWINDOW);
	if(pWindow)
		pWindow->ShowWindow(FALSE);

	for(i = _XDEF_WTITLE_HEDEK ; i <= _XDEF_WTITLE_GMWINDOW ; i++)
	{
		pWindow = g_MainWindowManager.FindWindow(i);
		if(pWindow)
			pWindow->ShowWindow(FALSE);
	}
}

void _XWindow_TrainingMed::OpenInterface(void)
{
	_XWindow* pWindow = g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIMAPWINDOW);
	if(pWindow)
		pWindow->ShowWindow(TRUE);

	if(g_pQuickSlot_Window)
		g_pQuickSlot_Window->ShowWindow(TRUE);

	pWindow = g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
	if(pWindow)
		pWindow->ShowWindow(TRUE);

	if(g_pState_Window)
		g_pState_Window->ShowWindow(TRUE);
}

void _XWindow_TrainingMed::DrawGuide(void)
{
	m_Point[0].Draw(m_WindowPosition.x+10, m_WindowPosition.y+187);
	m_Point[16].Draw(m_WindowPosition.x+43, m_WindowPosition.y+187);

	m_Point[1].Draw(m_WindowPosition.x+10, m_WindowPosition.y+221);
	m_Point[20].Draw(m_WindowPosition.x+43, m_WindowPosition.y+221);

	m_Point[2].Draw(m_WindowPosition.x+10, m_WindowPosition.y+255);
	m_Point[12].Draw(m_WindowPosition.x+43, m_WindowPosition.y+255);

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_XBaseFont->Puts(m_WindowPosition.x+108, m_WindowPosition.y+197, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_2301));
	g_XBaseFont->Puts(m_WindowPosition.x+108, m_WindowPosition.y+231, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_2302));	
	g_XBaseFont->Puts(m_WindowPosition.x+108, m_WindowPosition.y+264, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_2303));	
	g_XBaseFont->Flush();
}