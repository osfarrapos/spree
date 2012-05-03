// XWindow_TrainingDummy.cpp: implementation of the _XWindow_TrainingDummy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_TrainingDummy.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XParticleIDDefine.h"
#include "SoundEffectList.h"
#include "Xwindow_TrainingComplete.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_TrainingDummy::_XWindow_TrainingDummy()
{
}

_XWindow_TrainingDummy::~_XWindow_TrainingDummy()
{

}

BOOL _XWindow_TrainingDummy::Initialize(void)
{
	Reset();

	int resourceindex = g_MainInterfaceTextureArchive.FindResource("MI_tra_base.tga");

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
	{
		m_SuccessImage.Create(0, 0, 65, 14, &g_MainInterfaceTextureArchive, resourceindex);
		m_SuccessImage.SetClipRect(2, 233, 67, 247);
	}
	else
	{
		m_SuccessImage.Create(0, 0, 60, 29, &g_MainInterfaceTextureArchive, resourceindex);
		m_SuccessImage.SetClipRect(5, 226, 65, 255);
	}
	m_SuccessImageStartTime = 0;
	m_SuccessAlphaValue = 255;
	m_SuccessImagePosition.x = 0;
	m_SuccessImagePosition.y = 0;

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
	{
		m_FailImage.Create(0, 0, 51, 14, &g_MainInterfaceTextureArchive, resourceindex);
		m_FailImage.SetClipRect(8, 206, 59, 220);
	}
	else
	{
		m_FailImage.Create(0, 0, 62, 26, &g_MainInterfaceTextureArchive, resourceindex);
		m_FailImage.SetClipRect(5, 197, 67, 223);
	}
	m_FailImageStartTime = 0;
	m_FailAlphaValue = 255;
	m_FailImagePosition.x = 0;
	m_FailImagePosition.y = 0;

	int effectindex = g_MainInterfaceTextureArchive.FindResource("MI_tra_effect.tga");

	
	int resourceindex_2 = -1;	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("vn_MI_unki_04.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("us_MI_unki_04.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("tw_MI_unki_04.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("rs_MI_unki_04.tga");
	}
	else
	{
		resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("MI_unki_04.tga");
	}
	
	m_ComboImage.Create(0, 0, 24, 25, &g_MainInterfaceTextureArchive, resourceindex_2);
	m_ComboImage.SetClipRect(203, 214, 227, 239);
	m_ComboImageStartTime = 0;
	m_ComboAlphaValue = 255;
	m_ComboImagePosition.x = 0;
	m_ComboImagePosition.y = 0;

	m_ComboPoint = 0;

	for(int i = 0 ; i < 10 ; i++)
	{
		m_NumberImage[i].Create(0, 0, 14, 22, &g_MainInterfaceTextureArchive, resourceindex_2);
	}
	// 0
	m_NumberImage[0].SetClipRect(183, 216, 197, 238) ;
	// 1
	m_NumberImage[1].SetClipRect(9, 216, 23, 238);
	// 2
	m_NumberImage[2].SetClipRect(27, 216, 41, 238);
	// 3
	m_NumberImage[3].SetClipRect(46, 216, 60, 238);
	// 4
	m_NumberImage[4].SetClipRect(65, 216, 79, 238);
	// 5
	m_NumberImage[5].SetClipRect(85, 216, 99, 238);
	// 6
	m_NumberImage[6].SetClipRect(105, 216, 119, 238);
	// 7
	m_NumberImage[7].SetClipRect(125, 216, 139, 238);
	// 8
	m_NumberImage[8].SetClipRect(144, 216, 158, 238);
	// 9
	m_NumberImage[9].SetClipRect(164, 216, 178, 238);
	
	m_SuccessEffectImage_1[0].Create(0, 0, 38, 30, &g_MainInterfaceTextureArchive, effectindex);
	m_SuccessEffectImage_1[0].SetClipRect(9, 0, 47, 30);
	m_SuccessEffectImage_1[1].Create(0, 0, 106, 30, &g_MainInterfaceTextureArchive, effectindex);
	m_SuccessEffectImage_1[1].SetClipRect(4, 31, 110, 61);
	m_SuccessEffectImage_1[2].Create(0, 0, 140, 30, &g_MainInterfaceTextureArchive, effectindex);
	m_SuccessEffectImage_1[2].SetClipRect(4, 62, 144, 92);

	m_SuccessEffectImage_2[0].Create(0, 0, 42, 24, &g_MainInterfaceTextureArchive, effectindex);
	m_SuccessEffectImage_2[0].SetClipRect(8, 97, 50, 121);
	m_SuccessEffectImage_2[1].Create(0, 0, 84, 24, &g_MainInterfaceTextureArchive, effectindex);
	m_SuccessEffectImage_2[1].SetClipRect(6, 122, 90, 146);
	m_SuccessEffectImage_2[2].Create(0, 0, 158, 24, &g_MainInterfaceTextureArchive, effectindex);
	m_SuccessEffectImage_2[2].SetClipRect(6, 147, 164, 171);

	m_FailEffectImage[0].Create(0, 0, 64, 24, &g_MainInterfaceTextureArchive, resourceindex);
	m_FailEffectImage[0].SetClipRect(3, 122, 65, 146);
	m_FailEffectImage[1].Create(0, 0, 86, 24, &g_MainInterfaceTextureArchive, resourceindex);
	m_FailEffectImage[1].SetClipRect(77, 122, 163, 146);

	SetWindowMoveMode(_XWMOVE_FIXED);

	return _XWindow_Training::Initailize();
}

void _XWindow_TrainingDummy::DestroyWindow(void)
{
	_XWindow_Training::DestroyWindow();
}

void _XWindow_TrainingDummy::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow_Training::Draw(pfocusobject);

	if(!this->m_ShowWindow) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	_XDrawSolidBar( m_ValidAreaStart, m_WindowPosition.y+60, m_ValidAreaEnd, m_WindowPosition.y+84, D3DCOLOR_ARGB(255, 146, 37, 17) );

	if(m_bDrawMouseFailAnimation)
		DrawMouseFailAnimation();
	else if(m_bDrawMouseSuccessAnimation)
		DrawMouseSuccessAnimation();
	else
		m_MousePointImage[0].Draw(m_CurrentMousePos.x-13, m_CurrentMousePos.y);

	if((g_LocalSystemTime - m_SuccessImageStartTime) < 1000)
	{
		if(g_LocalSystemTime - m_SuccessImageStartTime > 200)
		{
			m_SuccessAlphaValue -= 5;
			m_SuccessImagePosition.y -= 1;
		}
		if(m_SuccessAlphaValue <= 0)
			m_SuccessAlphaValue = 0;

		m_SuccessImage.m_FColor = D3DCOLOR_ARGB(m_SuccessAlphaValue, 255, 255, 255);
		m_SuccessImage.MoveWindow(m_SuccessImagePosition.x, m_SuccessImagePosition.y, m_SuccessImagePosition.x+62, m_SuccessImagePosition.y+26);
		m_SuccessImage.DrawWithRegion();
	}

	if((g_LocalSystemTime - m_ComboImageStartTime) < 800)
	{
		if(g_LocalSystemTime - m_ComboImageStartTime > 500)
		{
			m_ComboAlphaValue -= 8;
			m_ComboImagePosition.y -= 1;
		}
		if(m_ComboAlphaValue <= 0)
			m_ComboAlphaValue = 0;

		m_ComboImage.m_FColor = D3DCOLOR_ARGB(m_ComboAlphaValue, 255, 255, 255);
		m_ComboImage.MoveWindow(m_ComboImagePosition.x, m_ComboImagePosition.y, m_ComboImagePosition.x+24, m_ComboImagePosition.y+25);
		m_ComboImage.DrawWithRegion();

		if(m_ComboPoint >= 2 && m_ComboPoint <= 9)
		{
			m_NumberImage[m_ComboPoint].m_FColor = D3DCOLOR_ARGB(m_ComboAlphaValue, 255, 255, 255);
			m_NumberImage[m_ComboPoint].MoveWindow(m_ComboImagePosition.x-18, m_ComboImagePosition.y, m_ComboImagePosition.x-18+62, m_ComboImagePosition.y+26);
			m_NumberImage[m_ComboPoint].DrawWithRegion();
		}	
	}

	if((g_LocalSystemTime - m_FailImageStartTime) < 1000)
	{
		if(g_LocalSystemTime - m_FailImageStartTime > 200)
		{
			m_FailAlphaValue -= 5;
			m_FailImagePosition.y -= 1;
		}
		if(m_FailAlphaValue <= 0)
			m_FailAlphaValue = 0;
		
		m_FailImage.m_FColor = D3DCOLOR_ARGB(m_FailAlphaValue, 255, 255, 255);
		m_FailImage.MoveWindow(m_FailImagePosition.x, m_FailImagePosition.y, m_FailImagePosition.x+60, m_FailImagePosition.y+29);
		m_FailImage.DrawWithRegion();
	}
}

BOOL _XWindow_TrainingDummy::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	// TODO
	if(m_bTrainingStart && !m_bWaitForServerRes)
	{
		if(m_PrevMoveTime == 0)
			m_PrevMoveTime = g_LocalSystemTime;

		if(m_PrevClickTime == 0)
			m_PrevClickTime = g_LocalSystemTime;

		DWORD elapsedtime = g_LocalSystemTime - m_PrevMoveTime;
		DWORD elapsedclicktime = g_LocalSystemTime - m_PrevClickTime;

		MouseState* mousestate = gpInput->GetMouseState();
		if(mousestate)
		{
			if(!pfocusobject)
			{
				if(mousestate->bButton[0] && elapsedclicktime >= 200)
				{
//					if(/*g_LastPickedObjectType == _XPICKINGTARGET_DUMMY)//*/g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_DUMMY
//						&& (_XDummy*)g_pLocalUser->m_AttackTarget == m_pDummy)
//					if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_DUMMY && (_XDummy*)g_pLocalUser->m_ViewTarget == m_pDummy)
					{
						if(m_CurrentMousePos.x >= m_ValidAreaStart && m_CurrentMousePos.x <= m_ValidAreaEnd)
						{
							if(gApp->m_bHasFocus)
							{
								DecideSuccess();
								m_bStopMousePoint = TRUE;
								_XDWINPRINT("HIT=============== SP[%d]  BP[%d] CP[%d]", m_SuccessPoint, m_BonusPoint, m_ComboPoint);
							}
						}
						else
						{
							if(gApp->m_bHasFocus)
							{
								DecideFail();
								m_bStopMousePoint = TRUE;
								_XDWINPRINT("MISS=============== SP[%d]  BP[%d] CP[%d]", m_SuccessPoint, m_BonusPoint, m_ComboPoint);
							}
						}
					}

					m_PrevClickTime = g_LocalSystemTime;
				}
			}
		}	

		if(!m_bStopMousePoint)
		{
			FLOAT movelength = 1.0f;
			if(m_TrainingGrade == 1)
				movelength = 0.8f;
			else if(m_TrainingGrade == 2)
				movelength = 0.9f;
			else if(m_TrainingGrade == 3)
				movelength = 1.0f;

			if(m_bMoveLeftDirection)
			{
				// 왼쪽으로 이동
				m_MousePosX -= elapsedtime * 0.4f * movelength;
				m_CurrentMousePos.x = (int)m_MousePosX;
			}
			else
			{
				// 오른쪽으로 이동
				m_MousePosX += elapsedtime * 0.4f * movelength;
				m_CurrentMousePos.x = (int)m_MousePosX;
			}
		}
		else
		{
			// 멈춤
		}

		if(m_CurrentMousePos.x >= m_MouseEndPos.x)
			m_bMoveLeftDirection = TRUE;
		if(m_CurrentMousePos.x <= m_MouseStartPos.x)
			m_bMoveLeftDirection = FALSE;

		if(m_BonusPoint == 3 && !m_bWaitForServerRes)
		{
			DecideBonus();
			_XDWINPRINT("BONUS=============== SP[%d]  BP[%d] CP[%d]", m_SuccessPoint, m_BonusPoint, m_ComboPoint);
		}

		m_PrevMoveTime = g_LocalSystemTime;
	}
	
	if(!_XWindow_Training::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_TrainingDummy::ShowWindow(BOOL show)
{
	if(show)
	{
/*		if(m_pDummy)
		{
			D3DXVECTOR3 rvec;
			D3DXVECTOR3 mpos = m_pDummy->m_Position;
			mpos.y += 2.5f;
			D3DXVec3Project(&rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), &g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);

			MoveWindow(rvec.x-186, rvec.y);
		}
		else
			MoveWindow(160, 170);*/

		MoveWindow((gnWidth>>1)-186, (gnHeight>>1)-236);
		
		SetTrainingGrade(1);
		SetBackground(m_WindowPosition.x, m_WindowPosition.y);

		m_MouseStartPos.x = m_WindowPosition.x + 86;
		m_MouseStartPos.y = m_WindowPosition.y + 60;
		
		m_MouseEndPos.x = m_WindowPosition.x + 286;
		m_MouseEndPos.y = m_WindowPosition.y + 60;

		m_CurrentMousePos = m_MouseStartPos;
		m_MousePosX = m_CurrentMousePos.x;
	}
	
	ShowGrade(show);
	_XWindow_Training::ShowWindow( show );
}

void _XWindow_TrainingDummy::SetBackground(int x, int y)
{
	int size = 0;
	size = (int)(226 * 0.2f);
	switch(m_TrainingGrade)
	{
	case 1 :
		size = (int)(226 * 0.2f);
		break;
	case 2 :
		size = (int)(226 * 0.3f);
		break;
	case 3 :
		size = (int)(226 * 0.4f);
		break;
	}

	m_ValidAreaStart = (x+186) - (size/2);
	m_ValidAreaEnd = (x+186) - (size/2) + size;
}

void _XWindow_TrainingDummy::DecideSuccess(void)
{
	if(g_pLocalUser->GetMotionClass() != _XACTION_IDLE)
		return;

	m_bSuccess = TRUE;
	m_bFail = FALSE;
	m_SuccessPoint++;
	m_BonusPoint++;
	m_ComboPoint++;

	if(m_SuccessPoint >= 12)
	{
		// Send Packet
		g_NetworkKernel.SendPacket(MSG_NO_MINIGAME_MAST_STAGE_C_S, m_TrainingGrade);
		m_bWaitForServerRes = TRUE;
	}

	AddGauge(m_SuccessPoint);
	m_bDrawMouseSuccessAnimation = true;

	if(m_ComboPoint < 4 || m_ComboPoint > 9 || m_SuccessPoint >= 12)
	{
		m_SuccessAlphaValue = 255;
		m_SuccessImageStartTime = g_LocalSystemTime;
		m_SuccessImagePosition.x = m_CurrentMousePos.x - 20; //m_WindowPosition.x + 134;
		m_SuccessImagePosition.y = m_WindowPosition.y + 25;

		_XPlayInterfaceSound(ID_SR_INTERFACE_TRA_OK01_WAV); 
	}
	else
	{
		m_ComboAlphaValue = 255;
		m_ComboImageStartTime = g_LocalSystemTime;
		m_ComboImagePosition.x = m_CurrentMousePos.x; //m_WindowPosition.x + 150;
		m_ComboImagePosition.y = m_WindowPosition.y + 25;

		_XPlayInterfaceSound(ID_SR_INTERFACE_TRA_COMBO01_WAV);
	}
}

void _XWindow_TrainingDummy::DecideFail(void)
{
	if(g_pLocalUser->GetMotionClass() != _XACTION_IDLE)
		return;

	m_bSuccess = FALSE;
	m_bFail = TRUE;
//	m_SuccessPoint--;
	m_BonusPoint = 0;
	m_ComboPoint = 0;

	if(m_SuccessPoint <= 0)
		m_SuccessPoint = 0;

	DeleteGauge(m_SuccessPoint);
	m_bDrawMouseFailAnimation = true;

	m_FailAlphaValue = 255;
	m_FailImageStartTime = g_LocalSystemTime;
	m_FailImagePosition.x = m_CurrentMousePos.x - 20; //m_WindowPosition.x + 134;
	m_FailImagePosition.y = m_WindowPosition.y + 25;

	_XPlayInterfaceSound(ID_SR_INTERFACE_TRA_MISS01_WAV);
}

void _XWindow_TrainingDummy::DecideBonus(void)
{
	m_SuccessPoint++;
	m_BonusPoint = 0;

	if(m_SuccessPoint >= 12)
	{
		// Send Packet
		g_NetworkKernel.SendPacket(MSG_NO_MINIGAME_MAST_STAGE_C_S, m_TrainingGrade);
		m_bWaitForServerRes = TRUE;

		m_SuccessAlphaValue = 255;
		m_SuccessImageStartTime = g_LocalSystemTime;
		m_SuccessImagePosition.x = m_CurrentMousePos.x - 20; //m_WindowPosition.x + 134;
		m_SuccessImagePosition.y = m_WindowPosition.y + 25;

		m_ComboImageStartTime = 0;
	}
	
	AddGauge(m_SuccessPoint);
	m_bDrawMouseSuccessAnimation = true;
}

void _XWindow_TrainingDummy::Reset(void)
{
	m_bTrainingStart = FALSE;
	m_bMoveLeftDirection = FALSE;

	m_bStopMousePoint = FALSE;

	m_SuccessPoint = 0;
	m_BonusPoint = 0;
	m_ComboPoint = 0;

	m_PrevMoveTime = 0;
	m_PrevClickTime = 0;

	m_bWaitForServerRes = FALSE;
	m_bComplete = FALSE;

	m_MouseAlphaValue = 0;
	m_SuccessAlphaValue = 255;
	m_FailAlphaValue = 255;
	m_ComboAlphaValue = 255;

	m_SuccessImageStartTime = 0;
	m_FailImageStartTime = 0;
	m_ComboImageStartTime = 0;

	m_SuccessImagePosition.x = 0;
	m_SuccessImagePosition.y = 0;
	m_FailImagePosition.x = 0;
	m_FailImagePosition.y = 0;
	m_ComboImagePosition.x = 0;
	m_ComboImagePosition.y = 0;

	m_bDrawMouseSuccessAnimation = false;
	m_bDrawMouseFailAnimation = false;
	
	m_PrevDrawTimeMouse = 0;
	m_EffectFrameMouse = 0.0f;
	m_EffectOrderSubMouse = false;

	m_pDummy = NULL;

	_XWindow_Training::Reset();
}

void _XWindow_TrainingDummy::ResetMousePosition(void)
{
	m_bStopMousePoint = FALSE;

	m_PrevMoveTime = 0;
	m_PrevClickTime = 0;
	m_MouseAlphaValue = 0;

	m_MouseAlphaValue -= 5;
	if(m_MouseAlphaValue <= 0)
		m_MouseAlphaValue = 0;
}


void _XWindow_TrainingDummy::Upgrade(void)
{
	m_SuccessPoint = 0;
	m_BonusPoint = 0;
	m_ComboPoint = 0;

	DeleteGauge(m_SuccessPoint);
	
	m_TrainingGrade++;
	SetBackground(m_WindowPosition.x, m_WindowPosition.y);
	
	ShowGrade(TRUE);

	_XPlayInterfaceSound(ID_SR_INTERFACE_TRA_UP01_WAV);
}

void _XWindow_TrainingDummy::Complete(void)
{
	m_bComplete = TRUE;
	g_pLocalUser->m_TrainingDummyInfo.bComplete = true;
	
	TCHAR messagestring[128];
	sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_2580), g_SkillProperty[g_pLocalUser->m_TrainingDummyInfo.skillid]->skillName);
	g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);

	_XWindow_TrainingComplete* pTrainingComplete_Window = (_XWindow_TrainingComplete*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGCOMPLETE);
	if(pTrainingComplete_Window)
	{
		pTrainingComplete_Window->SetSelectedImage(0);	// 수련완료
		pTrainingComplete_Window->ShowWindow(TRUE);
	}


/*	if(g_WeaponItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
	{
		g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_MONSTERID_GUN_LEARN,
			&g_pLocalUser->m_ModelDescriptor.m_Position, NULL, TRUE);
	}
	else
	{
		g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_MONSTERID_WEAPON_LEARN,
			&g_pLocalUser->m_ModelDescriptor.m_Position, NULL, TRUE);
	}*/

	_XPlayInterfaceSound(ID_SR_INTERFACE_LEARNSKILL01_WAV);
}

void _XWindow_TrainingDummy::DrawMouseSuccessAnimation(void)
{
	if(m_PrevDrawTimeMouse == 0)
		m_PrevDrawTimeMouse = g_LocalSystemTime;

	DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTimeMouse;

	if(!m_EffectOrderSubMouse)
		m_EffectFrameMouse += ((FLOAT)elapsedtime / 100.0f);
	else
		m_EffectFrameMouse -= ((FLOAT)elapsedtime / 100.0f);

	if(m_EffectFrameMouse > 5.0f)
	{
		m_EffectOrderSubMouse = true;
		m_EffectFrameMouse = 4.0f;
	}
	else if(m_EffectFrameMouse < 0.0f)
	{	
		m_bDrawMouseSuccessAnimation = false;
		m_EffectFrameMouse = 0.0f;
		m_PrevDrawTimeMouse = 0;
		m_EffectOrderSubMouse = false;
		return;
	}

	int selectedframe = (int)m_EffectFrameMouse;

	if(selectedframe > 4)
		selectedframe = 4;

//	_XDWINPRINT("SUCCESS   selectedframe [%d]   m_EffectFrameMouse [%f]  elapsedtime [%d]", selectedframe, m_EffectFrameMouse, elapsedtime);

	m_MousePointImage[selectedframe].Draw(m_CurrentMousePos.x-13, m_CurrentMousePos.y);

	if(selectedframe < 3 && !m_EffectOrderSubMouse)
	{
		SIZE size_1 = m_SuccessEffectImage_1[selectedframe].GetWindowSize();
		m_SuccessEffectImage_1[selectedframe].Draw(m_CurrentMousePos.x - size_1.cx/2, m_CurrentMousePos.y-5);

		SIZE size_2 = m_SuccessEffectImage_2[selectedframe].GetWindowSize();
		m_SuccessEffectImage_2[selectedframe].Draw(m_CurrentMousePos.x - size_2.cx/2, m_CurrentMousePos.y);
	}

	m_PrevDrawTimeMouse = g_LocalSystemTime;
}

void _XWindow_TrainingDummy::DrawMouseFailAnimation(void)
{
	if(m_PrevDrawTimeMouse == 0)
		m_PrevDrawTimeMouse = g_LocalSystemTime;
	
	DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTimeMouse;
	
	if(!m_EffectOrderSubMouse)
		m_EffectFrameMouse += ((FLOAT)elapsedtime / 100.0f);
	else 
		m_EffectFrameMouse -= ((FLOAT)elapsedtime / 100.0f);
	
	if(m_EffectFrameMouse > 5.0f)
	{
		m_EffectOrderSubMouse = true;
	}
	else if(m_EffectFrameMouse < 0.0f)
	{
		m_bDrawMouseFailAnimation = false;
		m_EffectFrameMouse = 0.0f;
		m_PrevDrawTimeMouse = 0;
		m_EffectOrderSubMouse = false;
		return;
	}
	
	int selectedframe = (int)m_EffectFrameMouse;
	
	if(selectedframe > 2)
		selectedframe = 2;

//	_XDWINPRINT("FAIL   selectedframe [%d]   m_EffectFrameMouse [%f]  elapsedtime [%d]", selectedframe, m_EffectFrameMouse, elapsedtime);

	if(selectedframe > 0)
		m_MousePointImage[selectedframe+4].Draw(m_CurrentMousePos.x-13, m_CurrentMousePos.y);
	else
		m_MousePointImage[selectedframe].Draw(m_CurrentMousePos.x-13, m_CurrentMousePos.y);

	if(selectedframe < 3 && !m_EffectOrderSubMouse)
	{
		if(m_CurrentMousePos.x >= m_WindowPosition.x+116 && m_CurrentMousePos.x <= m_WindowPosition.x+256)
		{
			if(selectedframe >= 2)
				selectedframe = 1;

			SIZE size = m_FailEffectImage[selectedframe].GetWindowSize();
			m_FailEffectImage[selectedframe].Draw(m_CurrentMousePos.x - size.cx/2, m_CurrentMousePos.y);
		}
	}

	m_PrevDrawTimeMouse = g_LocalSystemTime;
}