// xwndow_trainingspell.cpp: implementation of the _XWindow_TrainingSpell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xwindow_trainingspell.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XParticleIDDefine.h"
#include "SoundEffectList.h"
#include "XWindow_TrainingComplete.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_TrainingSpell::_XWindow_TrainingSpell()
{
}

_XWindow_TrainingSpell::~_XWindow_TrainingSpell()
{
}

BOOL _XWindow_TrainingSpell::Initialize(void)
{
	m_bTrainingStart		= FALSE;
	m_TrainingStartTime		= 0;
	
	m_MouseStartPos.x		= 0;
	m_MouseStartPos.y		= 0;
	m_MouseEndPos.x			= 0;
	m_MouseEndPos.y			= 0;
	m_CurrentMousePos.x		= 0;
	m_CurrentMousePos.y		= 0;

	m_MousePosX				= 0.0f;

	m_MouseDir				= (_XTRAINING_MOUSE_DIR)0;
	m_MouseVelocity			= 0.0f;
	m_MouseForceKeepTime	= 0;
	m_MouseMoveStartTime	= 0;

	m_PrevMoveTime			= 0;
	m_FailStartTime			= 0;
	m_bFailPeriod			= FALSE;

	m_LeftClickForce		= 0.0f;
	m_RightClickForce		= 0.0f;
	
	m_StaySuccessAreaTime	= 0;
	m_StaySuccessKeepTime	= 0;
	m_SuccessPoint			= 0;

	m_bWaitForServerRes		= FALSE;
	m_bComplete				= FALSE;

	m_bDrawMouseSuccessAnimation = false;
	m_bDrawMouseFailAnimation = false;
	
	m_PrevDrawTimeMouse = 0;
	m_EffectFrameMouse = 0.0f;
	m_EffectOrderSubMouse = false;

	int resourceindex = g_MainInterfaceTextureArchive.FindResource("MI_tra_base.tga");
	
	m_SuccessImage.Create(0, 0, 60, 29, &g_MainInterfaceTextureArchive, resourceindex);
	m_SuccessImage.SetClipRect(5, 226, 65, 255);
	m_SuccessImageStartTime = 0;
	m_SuccessAlphaValue = 255;
	m_SuccessImagePosition.x = 0;
	m_SuccessImagePosition.y = 0;
	
	m_FailImage.Create(0, 0, 62, 26, &g_MainInterfaceTextureArchive, resourceindex);
	m_FailImage.SetClipRect(5, 197, 67, 223);
	m_FailImageStartTime = 0;
	m_FailAlphaValue = 255;
	m_FailImagePosition.x = 0;
	m_FailImagePosition.y = 0;

	int effectindex = g_MainInterfaceTextureArchive.FindResource("MI_tra_effect.tga");

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

void _XWindow_TrainingSpell::DestroyWindow(void)
{
	_XWindow_Training::DestroyWindow();
}

void _XWindow_TrainingSpell::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow_Training::Draw();

	if(!this->m_ShowWindow) return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)	return;

	// TODO
	_XDrawSolidBar( m_ValidAreaStart, m_WindowPosition.y+60, m_ValidAreaEnd, m_WindowPosition.y+84, D3DCOLOR_ARGB(255, 146, 37, 17) );

	if(m_bDrawMouseFailAnimation)
		DrawMouseFailAnimation();
	else if(m_bDrawMouseSuccessAnimation)
		DrawMouseSuccessAnimation();
	else
		m_MousePointImage[0].Draw(m_CurrentMousePos.x-13, m_CurrentMousePos.y);

	if((g_LocalSystemTime - m_SuccessImageStartTime) < 800)
	{
		if(g_LocalSystemTime - m_SuccessImageStartTime > 500)
		{
			m_SuccessAlphaValue -= 5;
			m_SuccessImagePosition.y -= 1;
		}
		if(m_SuccessAlphaValue <= 0)
			m_SuccessAlphaValue = 0;
		
		m_SuccessImage.m_FColor = D3DCOLOR_ARGB(m_SuccessAlphaValue, 255, 255, 255);
		m_SuccessImage.MoveWindow(m_SuccessImagePosition.x, m_SuccessImagePosition.y, m_SuccessImagePosition.x+63, m_SuccessImagePosition.y+30);
		m_SuccessImage.DrawWithRegion();
	}

	if((g_LocalSystemTime - m_FailImageStartTime) < 800)
	{
		if(g_LocalSystemTime - m_FailImageStartTime > 500)
		{
			m_FailAlphaValue -= 5;
			m_FailImagePosition.y -= 1;
		}
		if(m_FailAlphaValue <= 0)
			m_FailAlphaValue = 0;
		
		m_FailImage.m_FColor = D3DCOLOR_ARGB(m_FailAlphaValue, 255, 255, 255);
		m_FailImage.MoveWindow(m_FailImagePosition.x, m_FailImagePosition.y, m_FailImagePosition.x+60, m_FailImagePosition.y+30);
		m_FailImage.DrawWithRegion();
	}
}

BOOL _XWindow_TrainingSpell::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	if(m_bComplete)
	{
		ResetMousePos();
	}

	// TODO
	if((g_LocalSystemTime - m_TrainingStartTime > 1500) && m_TrainingStartTime != 0)
	{
		if(m_PrevMoveTime == 0)
			m_PrevMoveTime = g_LocalSystemTime;

		DWORD elapsedtime = g_LocalSystemTime - m_PrevMoveTime;

		if(m_MouseMoveStartTime > 0)
		{
			// 마우스 입력
			if(!m_bFailPeriod)
			{
				MouseState* mousestate = gpInput->GetMouseState();
				if(mousestate)
				{
					if(mousestate->bButton[0])
					{
						m_LeftClickForce += m_MouseVelocity * 0.005 * elapsedtime;
						m_RightClickForce = 0.0f;

	//					_XDWINPRINT("LEFT CLICK [left : %f]", m_LeftClickForce);
					}
					else
						m_LeftClickForce = 0.0f;
					if(mousestate->bButton[1])
					{
						m_RightClickForce += m_MouseVelocity * 0.005 * elapsedtime;
						m_LeftClickForce = 0.0f;

	//					_XDWINPRINT("RIGHT CLICK [right : %f]", m_RightClickForce);
					}
					else
						m_RightClickForce = 0.0f;
				}

				// 마우스 움직임
				if(g_LocalSystemTime - m_MouseMoveStartTime <= m_MouseForceKeepTime)
				{
					if(m_MouseDir == MOUSE_DIR_LEFT)
					{
						// 왼쪽으로
						FLOAT velocity = m_MouseVelocity + m_LeftClickForce - m_RightClickForce;

	//					_XDWINPRINT("Velocity [left : %f  / right : %f  / total : %f]", m_LeftClickForce, m_RightClickForce, velocity);

						m_MousePosX = m_MousePosX - elapsedtime * 0.1f * velocity;
						m_CurrentMousePos.x = (int)m_MousePosX;
					}
					else
					{
						// 오른쪽으로
						FLOAT velocity = m_MouseVelocity + m_RightClickForce - m_LeftClickForce;

	//					_XDWINPRINT("Velocity [left : %f  / right : %f  / total : %f]", m_LeftClickForce, m_RightClickForce, velocity);

						m_MousePosX = m_MousePosX + elapsedtime * 0.1f * velocity;
						m_CurrentMousePos.x = (int)m_MousePosX;
					}
				}
				else
				{
					if(!m_bFailPeriod)
					{
						m_MouseMoveStartTime = 0;		// Keeptime 지남, 다시 판정
						m_MousePosX = m_CurrentMousePos.x;
					}
					else
					{
						if(g_LocalSystemTime - m_FailStartTime >= 1000)
						{
							_XDWINPRINT("ERROR ===============================");
						}
					}
				}
			}

			BOOL	bInValidArea = FALSE;
			if(!m_bWaitForServerRes)
			{
				// 성공, 실패 판정은 서버로부터의 답을 기다리고 있을 때는 하지 않는다.

				if(m_CurrentMousePos.x >= m_ValidAreaStart && m_CurrentMousePos.x <= m_ValidAreaEnd)
				{
					if(m_StaySuccessAreaTime == 0)
						m_StaySuccessAreaTime = g_LocalSystemTime;

					bInValidArea = TRUE;
					if(g_LocalSystemTime - m_StaySuccessAreaTime > m_StaySuccessKeepTime)
					{
						if(gApp->m_bHasFocus)
							DecideSuccess();
					}
				}
				if(!bInValidArea)
					m_StaySuccessAreaTime = 0;

				if(m_CurrentMousePos.x >= m_MouseEndPos.x-3)
				{
					if(m_FailStartTime == 0)
					{
						_XDWINPRINT("MISS===");
						DecideMiss();
						m_FailStartTime = g_LocalSystemTime;
						m_bFailPeriod = TRUE;
					}
					
					if(g_LocalSystemTime - m_FailStartTime <= 1000)
						m_CurrentMousePos.x = m_MouseEndPos.x;
					else
						ResetMousePos();	// 다시 시작
				}
				if(m_CurrentMousePos.x <= m_MouseStartPos.x+3)
				{
					if(m_FailStartTime == 0)
					{
						_XDWINPRINT("MISS===");
						DecideMiss();
						m_FailStartTime = g_LocalSystemTime;
						m_bFailPeriod = TRUE;
					}
					
					if(g_LocalSystemTime - m_FailStartTime <= 1000)
						m_CurrentMousePos.x = m_MouseStartPos.x;
					else
						ResetMousePos();	// 다시 시작
				}
			}
		}

		if(m_MouseMoveStartTime == 0 && !m_bComplete)
		{
			// 인력, 지속시간, 속도 지정
			DecideMouseProperty();
			m_MouseMoveStartTime = g_LocalSystemTime;
//			_XDWINPRINT("DECIDE MOUSE PROPERTY [Dir : %d / KeepTime : %d / Velocity : %f]", m_MouseDir, m_MouseForceKeepTime, m_MouseVelocity);
		}

		m_PrevMoveTime = g_LocalSystemTime;
	}

	if(!_XWindow_Training::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_TrainingSpell::ShowWindow(BOOL show)
{
	if(show)
	{
/*		D3DXVECTOR3 rvec;
		D3DXVECTOR3 mpos = g_pLocalUser->m_Position;
		mpos.y += 2.0f;
		D3DXVec3Project(&rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), &g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);

		MoveWindow(rvec.x-180 , rvec.y);
*/
		MoveWindow((gnWidth>>1)-186, (gnHeight>>1)-236);

		SetTrainingGrade(1);
		SetBackground(m_WindowPosition.x, m_WindowPosition.y);

		m_MouseStartPos.x = m_WindowPosition.x + 86;
		m_MouseStartPos.y = m_WindowPosition.y + 60;

		m_MouseEndPos.x = m_WindowPosition.x + 286;
		m_MouseEndPos.y = m_WindowPosition.y + 60;

		m_CurrentMousePos.x = m_MouseStartPos.x + (m_MouseEndPos.x - m_MouseStartPos.x)/2;
		m_CurrentMousePos.y = m_MouseStartPos.y;

		m_MousePosX = m_CurrentMousePos.x;

		_XDWINPRINT("START GAME ===== [start %d / end %d] ", m_MouseStartPos.x, m_MouseEndPos.x);
	}

	ShowBackground(show);
	ShowGrade(show);
	_XWindow_Training::ShowWindow(show);
}

void _XWindow_TrainingSpell::SetBackground(int x, int y)
{
	_XWindow_Training::SetBackground(x, y);

	int size = 20;
	switch(m_TrainingGrade)
	{
	case 1 :
		size = 80;
		break;
	case 2 :
		size = 80;
		break;
	case 3 : 
		size = 80;
		break;
	}

//	m_BackgroundAlphaImage->SetClipRect(0, 31, size, 58);
//	m_BackgroundAlphaImage->MoveWindow(((x+115)-(size/2)), y+2);

	m_ValidAreaStart = (x+186) - (size/2);
	m_ValidAreaEnd = (x+186) + (size/2);
}

void _XWindow_TrainingSpell::Reset(void)
{
	m_bTrainingStart		= FALSE;
	m_TrainingStartTime		= 0;

	m_MouseDir				= (_XTRAINING_MOUSE_DIR)0;
	m_MouseVelocity			= 0.0f;
	m_MouseForceKeepTime	= 0;
	m_MouseMoveStartTime	= 0;

	m_MousePosX				= 0.0f;

	m_PrevMoveTime			= 0;
	m_FailStartTime			= 0;
	m_bFailPeriod			= FALSE;
	
	m_LeftClickForce		= 0.0f;
	m_RightClickForce		= 0.0f;

	m_StaySuccessAreaTime	= 0;
	m_StaySuccessKeepTime	= 0;
	m_SuccessPoint			= 0;

	m_bWaitForServerRes		= FALSE;
	m_bComplete				= FALSE;

	m_bDrawMouseSuccessAnimation = false;
	m_bDrawMouseFailAnimation = false;
	
	m_PrevDrawTimeMouse = 0;
	m_EffectFrameMouse = 0.0f;
	m_EffectOrderSubMouse = false;

	_XWindow_Training::Reset();
}

void _XWindow_TrainingSpell::ResetMousePos(void)
{
	m_MouseMoveStartTime	= 0;

	m_CurrentMousePos.x		= m_MouseStartPos.x + (m_MouseEndPos.x - m_MouseStartPos.x)/2;
	m_CurrentMousePos.y		= m_MouseStartPos.y;
	m_MousePosX				= m_CurrentMousePos.x;

	m_FailStartTime			= 0;
	m_bFailPeriod			= FALSE;
	
	m_LeftClickForce		= 0.0f;
	m_RightClickForce		= 0.0f;

	m_StaySuccessAreaTime	= 0;

//	_XDWINPRINT("ResetMousePosition ==");
}

void _XWindow_TrainingSpell::DecideMouseProperty(void)
{
	// 방향
	int dir = rand() % 2;
	m_MouseDir = (_XTRAINING_MOUSE_DIR)dir;
	
	// 지속시간
	int time = rand() % 4;
	switch(time)
	{
	case 0 :
		m_MouseForceKeepTime = 500;
		break;
	case 1 :
		m_MouseForceKeepTime = 1000;
		break;
	case 2 :
		m_MouseForceKeepTime = 1500;
		break;
	case 3 :
		m_MouseForceKeepTime = 2000;
		break;
	}
	
	// 속도
	m_MouseVelocity = 0.5f;

	switch(m_TrainingGrade)
	{
	case 1 :
		m_StaySuccessKeepTime = 1000;
		break;
	case 2 : 
		m_StaySuccessKeepTime = 1300;
		break;
	case 3 :
		m_StaySuccessKeepTime = 1500;
		break;
	}
}

void _XWindow_TrainingSpell::DecideSuccess(void)
{
	m_SuccessPoint++;
	m_StaySuccessAreaTime = 0;

	AddGauge(m_SuccessPoint);
	m_bDrawMouseSuccessAnimation = true;

//	m_GaugeGradeImage.SetClipRect(0, 82, (m_SuccessPoint*19), 93);

	m_SuccessAlphaValue = 255;
	m_SuccessImageStartTime = g_LocalSystemTime;
	m_SuccessImagePosition.x = m_CurrentMousePos.x - 20;
	m_SuccessImagePosition.y = m_WindowPosition.y + 25;
	
	_XPlayInterfaceSound(ID_SR_INTERFACE_TRA_OK01_WAV);

	if(m_SuccessPoint >= 12)
	{
		// Send Packet
		g_NetworkKernel.SendPacket(MSG_NO_MINIGAME_MAST_STAGE_C_S, m_TrainingGrade);
		m_bWaitForServerRes = TRUE;
	}
}

void _XWindow_TrainingSpell::DecideMiss(void)
{
/*	m_SuccessPoint--;
	if(m_SuccessPoint <= 0)
		m_SuccessPoint = 0;
*/	//m_GaugeGradeImage.SetClipRect(0, 82, (m_SuccessPoint*19), 93);

	m_bDrawMouseFailAnimation = true;

	m_FailAlphaValue = 255;
	m_FailImageStartTime = g_LocalSystemTime;
	m_FailImagePosition.x = m_CurrentMousePos.x - 20;
	m_FailImagePosition.y = m_WindowPosition.y + 25;

	_XPlayInterfaceSound(ID_SR_INTERFACE_TRA_MISS01_WAV);
}

void _XWindow_TrainingSpell::Upgrade(void)
{
	m_SuccessPoint = 0;

	DeleteGauge(m_SuccessPoint);

//	m_GaugeGradeImage->SetClipRect(0, 82, (m_SuccessPoint*19), 93);

	m_TrainingGrade++;
	SetBackground(m_WindowPosition.x, m_WindowPosition.y);
	ShowGrade(TRUE);

	ResetMousePos();

	_XPlayInterfaceSound(ID_SR_INTERFACE_TRA_UP01_WAV);
}

void _XWindow_TrainingSpell::Complete(void)
{
	m_bComplete = TRUE;
	
	TCHAR messagestring[128];
	sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_2580), g_SkillProperty[g_pLocalUser->m_TrainingSpellInfo.skillid]->skillName);
	g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);

	_XWindow_TrainingComplete* pTrainingComplete_Window = (_XWindow_TrainingComplete*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGCOMPLETE);
	if(pTrainingComplete_Window)
	{
		pTrainingComplete_Window->SetSelectedImage(0);		// 수련완료
		pTrainingComplete_Window->ShowWindow(TRUE);
	}
	
//	m_GaugeGradeImage.SetClipRect(0, 82, 12*19, 93);
	
//	g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_MONSTERID_GI_LEARN,
//		&g_pLocalUser->m_ModelDescriptor.m_Position, NULL, TRUE);

	_XPlayInterfaceSound(ID_SR_INTERFACE_LEARNSKILL01_WAV);
}

void _XWindow_TrainingSpell::DrawMouseSuccessAnimation(void)
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

void _XWindow_TrainingSpell::DrawMouseFailAnimation(void)
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