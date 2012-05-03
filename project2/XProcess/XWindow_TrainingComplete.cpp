// XWindow_TrainingComplete.cpp: implementation of the _XWindow_TrainingComplete class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "emperorofdragons.h"
#include "XWindow_TrainingComplete.h"
#include "EODEXTDEF_GlobalObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_TrainingComplete::_XWindow_TrainingComplete()
{

}

_XWindow_TrainingComplete::~_XWindow_TrainingComplete()
{

}

BOOL _XWindow_TrainingComplete::Initialize(void)
{
	int resourceindex_2 = -1;	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		m_UpImage.Create(0, 0, 196, 52, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("vn_MI_unki_02.tga"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		m_UpImage.Create(0, 0, 196, 52, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("tw_MI_unki_02.tga"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		m_UpImage.Create(0, 0, 196, 52, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("us_MI_unki_02.tga"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_UpImage.Create(0, 0, 196, 52, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("rs_MI_unki_02.tga"));
	}
	else 
	{
		m_UpImage.Create(0, 0, 196, 52, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_unki_02.tga"));
	}

	m_UpImage.SetClipRect(5, 170, 201, 222);

	int numberresourceindex = -1;	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		m_CompleteImage.Create(0, 0, 193, 52, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("vn_MI_unki_04.tga"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		m_CompleteImage.Create(0, 0, 193, 52, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("us_MI_unki_04.tga"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		m_CompleteImage.Create(0, 0, 193, 52, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("tw_MI_unki_04.tga"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_CompleteImage.Create(0, 0, 193, 52, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("rs_MI_unki_04.tga"));
	}
	else 
	{
		m_CompleteImage.Create(0, 0, 193, 52, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_unki_04.tga"));
	}	
	
	m_CompleteImage.SetClipRect(61, 63, 254, 115);

	return TRUE;
}

void _XWindow_TrainingComplete::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	if((g_LocalSystemTime - m_CompleteTime) < 3000)
	{
		if((g_LocalSystemTime - m_CompleteTime) > 200)
		{
			m_CompleteAlphaValue -= 3;
		}
		if(m_CompleteAlphaValue <= 0)
			m_CompleteAlphaValue = 0;
		
		if(m_SelectedImage == 0)
		{
			m_CompleteImage.m_FColor = D3DCOLOR_ARGB(m_CompleteAlphaValue, 255, 255, 255);
			m_CompleteImage.Draw((gnWidth>>1)-98, (gnHeight>>1)-152);
		}
		else
		{
			m_UpImage.m_FColor = D3DCOLOR_ARGB(m_CompleteAlphaValue, 255, 255, 255);
			m_UpImage.Draw((gnWidth>>1)-98, (gnHeight>>1)-152);
		}
	}
}

BOOL _XWindow_TrainingComplete::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_TrainingComplete::ShowWindow(BOOL show)
{
	if(show)
	{
		m_CompleteTime = g_LocalSystemTime;
		m_CompleteAlphaValue = 255;
	}
	else
	{
		m_CompleteTime = 0;
		m_CompleteAlphaValue = 0;
	}
}