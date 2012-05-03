// XWindow_BloodPoint.cpp: implementation of the _XWindow_BloodPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_BloodPoint.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_BloodPoint::_XWindow_BloodPoint()
{
	m_TitleImage = NULL;
	m_ReqItemButton = NULL;
	m_bDrawAnimation = FALSE;

	m_bDraw_Button_StartAnimation = FALSE;
	m_bDraw_Button_Animation = FALSE;
	m_Draw_Button_Time = 0;
	m_Current_Button_Frame = 0;

	for(int i = 0 ; i < 3 ; i++)
	{
		m_CurrentDrawType[i] = 0;
		m_CurrentPoint[i] = 0;
	}

	m_CurrentFrame = 0;
	m_DrawTime = 0;
	m_ViewDeadMessageStartTime = 0;
}

_XWindow_BloodPoint::~_XWindow_BloodPoint()
{

}

BOOL _XWindow_BloodPoint::Initialize()
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_bloodpoint.tga");

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
	{
		m_TitleImage = new _XImageStatic;
		m_TitleImage->Create(-10, 0, 7, 15, &g_MainInterfaceTextureArchive, resourceindex);
		m_TitleImage->SetClipRect(53, 78, 81, 93);
		InsertChildObject(m_TitleImage);
	}
	else
	{
		m_TitleImage = new _XImageStatic;
		m_TitleImage->Create(1, 0, 18, 15, &g_MainInterfaceTextureArchive, resourceindex);
		m_TitleImage->SetClipRect(53, 78, 70, 93);
		InsertChildObject(m_TitleImage);
	}

	// 각 숫자 자리 offset
	m_NumberPos[0] = 18;
	m_NumberPos[1] = 32;
	m_NumberPos[2] = 41;

	// 숫자 Type
	for(int i = 0 ; i < 3 ; i++)
	{
		m_NumberType[i] = 0;
	}

	for(i = 0 ; i < 4 ; i++)
	{
		m_NumberImage[i][0].Create(0, 0, 13, 19, &g_MainInterfaceTextureArchive, resourceindex);
		m_NumberImage[i][0].SetClipRect(0+(i*13), 171, 13+(i*13), 190);

		for(int j = 1 ; j < 10 ; j++)
		{
			m_NumberImage[i][j].Create(0, 0, 13, 19, &g_MainInterfaceTextureArchive, resourceindex);
			m_NumberImage[i][j].SetClipRect(0+(i*13), 0+((j-1)*19), 13+(i*13), 19+((j-1)*19));
		}
	}

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {-7, -6}, {67, 27}/*{18, -5}, {34, 19}*/, _XDEF_BLOODPOINT_REQITEM,
		resourceindex,
		resourceindex,
		resourceindex,
		&g_MainInterfaceTextureArchive
	};

	m_ReqItemButton = new _XButton;
	m_ReqItemButton->Create(btnstruct);
	m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 0, 256, 27);
	m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 27, 256, 54);
	m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 54, 256, 81);
	m_ReqItemButton->ShowWindow(FALSE);
	InsertChildObject(m_ReqItemButton);

	return TRUE;
}

void _XWindow_BloodPoint::Draw(_XGUIObject*& pfocusobject)
{	
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;
	
	// TODO
#ifdef _XTS_BLOODPOINT
	if(m_bDrawAnimation)
	{
		// frame 갱신
		if(m_DrawTime == 0)
			m_DrawTime = g_LocalSystemTime;

		if(g_LocalSystemTime - m_DrawTime > 100)
		{
			m_CurrentFrame--;
			if(m_CurrentFrame < 0)
			{
				SetDrawAnimation(FALSE);
			}
			m_DrawTime = g_LocalSystemTime;
		}
	}

	if(!m_ReqItemButton->GetShowStatus() && !m_bDraw_Button_StartAnimation)
		DrawAnimation(m_CurrentFrame);
	else
		DrawButtonAnimation();
	
	// 예제
//	if(g_pLocalUser->m_BloodPoint < 10)
//	{
//		m_NumberImage[0][g_pLocalUser->m_BloodPoint].Draw(m_WindowPosition.x+m_NumberPos[2]+13, m_WindowPosition.y-2);
//		m_NumberImage[1][g_pLocalUser->m_BloodPoint].Draw(m_WindowPosition.x+m_NumberPos[2]-2, m_WindowPosition.y-3);
//		m_NumberImage[2][g_pLocalUser->m_BloodPoint].Draw(m_WindowPosition.x+m_NumberPos[1]-1, m_WindowPosition.y-5);
//		m_NumberImage[3][g_pLocalUser->m_BloodPoint].Draw(m_WindowPosition.x+m_NumberPos[0], m_WindowPosition.y-5);
//	}
#endif
}

BOOL _XWindow_BloodPoint::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;
	
	// TODO
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_BloodPoint::DrawAnimation(int frame)
{
	switch(frame)
	{
	case 5 :
		{
			m_NumberImage[3][m_CurrentPoint[0]].Draw(m_WindowPosition.x+m_NumberPos[0], m_WindowPosition.y-5);
			m_NumberImage[0][m_CurrentPoint[1]].Draw(m_WindowPosition.x+m_NumberPos[1]-3, m_WindowPosition.y-2);
			m_NumberImage[0][m_CurrentPoint[2]].Draw(m_WindowPosition.x+m_NumberPos[2]-3, m_WindowPosition.y-2);
		}
		break;
	case 4 :
		{
			if(m_CurrentPoint[0] != 0)
			{
				m_NumberImage[2][m_CurrentPoint[0]].Draw(m_WindowPosition.x+m_NumberPos[0]+1, m_WindowPosition.y-5);
			}

			m_NumberImage[3][m_CurrentPoint[1]].Draw(m_WindowPosition.x+m_NumberPos[1], m_WindowPosition.y-5);
			m_NumberImage[0][m_CurrentPoint[2]].Draw(m_WindowPosition.x+m_NumberPos[2]-3, m_WindowPosition.y-2);
		}
		break;
	case 3 :
		{
			if(m_CurrentPoint[0] != 0)
			{
				m_NumberImage[1][m_CurrentPoint[0]].Draw(m_WindowPosition.x+m_NumberPos[0]+2, m_WindowPosition.y-3);
			}

			if(m_CurrentPoint[1] != 0)
			{
				m_NumberImage[2][m_CurrentPoint[1]].Draw(m_WindowPosition.x+m_NumberPos[1]-1, m_WindowPosition.y-5);
			}
			else if(m_CurrentPoint[0] != 0)
			{
				m_NumberImage[2][m_CurrentPoint[1]].Draw(m_WindowPosition.x+m_NumberPos[1]-1, m_WindowPosition.y-5);
			}

			m_NumberImage[3][m_CurrentPoint[2]].Draw(m_WindowPosition.x+m_NumberPos[2], m_WindowPosition.y-5);
		}
		break;
	case 2 :
		{
			if(m_CurrentPoint[0] != 0)
			{
				m_NumberImage[0][m_CurrentPoint[0]].Draw(m_WindowPosition.x+m_NumberPos[0]+3, m_WindowPosition.y-2);
			}

			if(m_CurrentPoint[1] != 0)
			{
				m_NumberImage[1][m_CurrentPoint[1]].Draw(m_WindowPosition.x+m_NumberPos[1]-2, m_WindowPosition.y-3);
			}
			else if(m_CurrentPoint[0] != 0)
			{
				m_NumberImage[1][m_CurrentPoint[1]].Draw(m_WindowPosition.x+m_NumberPos[1]-2, m_WindowPosition.y-3);
			}

			m_NumberImage[2][m_CurrentPoint[2]].Draw(m_WindowPosition.x+m_NumberPos[2]-1, m_WindowPosition.y-5);
		}
		break;
	case 1 :
		{
			if(m_CurrentPoint[0] != 0)
			{
				m_NumberImage[0][m_CurrentPoint[0]].Draw(m_WindowPosition.x+m_NumberPos[0]+3, m_WindowPosition.y-2);
			}

			if(m_CurrentPoint[1] != 0)
			{
				m_NumberImage[0][m_CurrentPoint[1]].Draw(m_WindowPosition.x+m_NumberPos[1]-3, m_WindowPosition.y-2);
			}
			else if(m_CurrentPoint[0] != 0)
			{
				m_NumberImage[0][m_CurrentPoint[1]].Draw(m_WindowPosition.x+m_NumberPos[1]-3, m_WindowPosition.y-2);
			}

			m_NumberImage[1][m_CurrentPoint[2]].Draw(m_WindowPosition.x+m_NumberPos[2]-2, m_WindowPosition.y-3);
		}
		break;
	case 0 :
		{
			if(m_CurrentPoint[0] != 0)
			{
				m_NumberImage[0][m_CurrentPoint[0]].Draw(m_WindowPosition.x+m_NumberPos[0]+3, m_WindowPosition.y-2);
			}

			if(m_CurrentPoint[1] != 0)
			{
				m_NumberImage[0][m_CurrentPoint[1]].Draw(m_WindowPosition.x+m_NumberPos[1]-3, m_WindowPosition.y-2);
			}
			else if(m_CurrentPoint[0] != 0)
			{
				m_NumberImage[0][m_CurrentPoint[1]].Draw(m_WindowPosition.x+m_NumberPos[1]-3, m_WindowPosition.y-2);
			}

			m_NumberImage[0][m_CurrentPoint[2]].Draw(m_WindowPosition.x+m_NumberPos[2]-3, m_WindowPosition.y-2);
		}
		break;
	}
}

void _XWindow_BloodPoint::SetDrawAnimation(BOOL draw)
{
#ifdef _XTS_BLOODPOINT

	if(draw)
	{
		m_bDrawAnimation = TRUE;
		m_DrawTime = 0;

		m_CurrentPoint[0] = g_pLocalUser->m_BloodPoint / 100;			// 100자리

		m_CurrentPoint[1] = g_pLocalUser->m_BloodPoint/10;
		if(m_CurrentPoint[1] >= 10)
		{
			m_CurrentPoint[1] %= 10;									// 10자리
		}

		m_CurrentPoint[2] = ((g_pLocalUser->m_BloodPoint%100) % 10);	// 1자리

		// 시작 frame setting
		if(m_CurrentPoint[0] != 0)
		{
			m_CurrentFrame = 5;
		}
		else if(m_CurrentPoint[1] != 0)
		{
			m_CurrentFrame = 4;
		}
		else if(m_CurrentPoint[2] != 0)
		{
			m_CurrentFrame = 3;
		}
	}
	else
	{
		m_bDrawAnimation = FALSE;
		m_CurrentFrame = 0;
		m_DrawTime = 0;

		for(int i = 0 ; i < 3 ; i++)
		{
			// 초기화
			m_CurrentDrawType[i] = 0;
		}
	}

#endif
}

void _XWindow_BloodPoint::SetShowReqItem(BOOL show)
{
	if(show)
	{
		m_bDraw_Button_StartAnimation = TRUE;
		m_bDraw_Button_Animation = FALSE;
		m_Current_Button_Frame = 3;
		m_Draw_Button_Time = 0;
	}
	else
	{
		m_bDraw_Button_StartAnimation = FALSE;
		m_bDraw_Button_Animation = FALSE;
		m_Current_Button_Frame = 0;
		m_Draw_Button_Time = 0;
		m_ReqItemButton->ShowWindow(FALSE);
	}
}

void _XWindow_BloodPoint::DrawButtonAnimation()
{
	if(m_bDraw_Button_StartAnimation)
	{
		// start animation
		// frame 갱신
		if(m_Draw_Button_Time == 0)
			m_Draw_Button_Time = g_LocalSystemTime;
		
		if(g_LocalSystemTime - m_Draw_Button_Time > 100)
		{
			m_Current_Button_Frame--;
			if(m_Current_Button_Frame < 0)
			{
				m_bDraw_Button_StartAnimation = FALSE;
				m_bDraw_Button_Animation = TRUE;
				
				m_Current_Button_Frame = 5;
			}
			m_Draw_Button_Time = g_LocalSystemTime;
		}

		if(m_Current_Button_Frame == 3)
		{
			m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 122, 0, 189, 27);
			m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 122, 0, 189, 27);
			m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 122, 0, 189, 27);
			m_ReqItemButton->ShowWindow(TRUE);
		}
		if(m_Current_Button_Frame == 2)
		{
			m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 122, 27, 189, 54);
			m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 122, 27, 189, 54);
			m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 122, 27, 189, 54);
		}
		if(m_Current_Button_Frame == 1)
		{
			m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 122, 54, 189, 81);
			m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 122, 54, 189, 81);
			m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 122, 54, 189, 81);
		}
		if(m_Current_Button_Frame == 0)
		{
			m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 0, 256, 27);
			m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 27, 256, 54);
			m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 54, 256, 81);
		}

	}

	if(m_bDraw_Button_Animation)
	{
		// normal animation
		if(g_LocalSystemTime - m_Draw_Button_Time > 100)
		{
			m_Current_Button_Frame--;
			if(m_Current_Button_Frame < 0)
			{
				m_Current_Button_Frame = 30;
			}
			m_Draw_Button_Time = g_LocalSystemTime;

			if(m_Current_Button_Frame == 4)
			{
				m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 81, 256, 108);
			}
			else if(m_Current_Button_Frame == 3)
			{
				m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 108, 256, 135);
			}
			else if(m_Current_Button_Frame == 2)
			{
				m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 135, 256, 162);
			}
			else if(m_Current_Button_Frame == 1)
			{
				m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 162, 256, 189);
			}
			else
			{
				m_ReqItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 0, 256, 27);
			}
		}
	}
}