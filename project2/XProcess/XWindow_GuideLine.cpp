// XWindow_GuideLine.cpp: implementation of the _XWindow_GuideLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_MainMenu.h"
#include "XWindow_GuideLine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_GuideLine::_XWindow_GuideLine()
{
	
}

_XWindow_GuideLine::~_XWindow_GuideLine()
{

}

BOOL _XWindow_GuideLine::Initialize( void )
{
	m_GuideLineStep = GS_FIRST;

	int resourceimageindex = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		resourceimageindex = g_MainInterfaceTextureArchive.FindResource( "us_tutorial_01.tga" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		resourceimageindex = g_MainInterfaceTextureArchive.FindResource( "rs_tutorial_01.tga" );
	}
	else
	{
		resourceimageindex = g_MainInterfaceTextureArchive.FindResource( "tutorial_01.tga" );
	}

	// 바탕
	_XImageStatic* pLeftBackBoard = new _XImageStatic;
	pLeftBackBoard->Create( 28, 35, 284, 97, &g_MainInterfaceTextureArchive, resourceimageindex );
	pLeftBackBoard->SetClipRect(0, 194, 256, 256);
	InsertChildObject( pLeftBackBoard );
	
	_XImageStatic* pRightBackBoard = new _XImageStatic;
	pRightBackBoard->Create( 284, 35, 332, 97, &g_MainInterfaceTextureArchive, resourceimageindex );
	pRightBackBoard->SetClipRect(208, 131, 256, 193);
	InsertChildObject( pRightBackBoard );

	// 좌
	_XImageStatic* pLeftSide = new _XImageStatic;
	pLeftSide->Create( 0, 2, 28, 130, &g_MainInterfaceTextureArchive, resourceimageindex );
	pLeftSide->SetClipRect(0, 0, 28, 129);
	InsertChildObject( pLeftSide );

	// 우
	_XImageStatic* pRightSide = new _XImageStatic;
	pRightSide->Create( 332, 2, 360, 130, &g_MainInterfaceTextureArchive, resourceimageindex );
	pRightSide->SetClipRect(29, 0, 57, 129);
	InsertChildObject( pRightSide );

	// 좌상
	_XImageStatic* pLeftTopSide = new _XImageStatic;
	pLeftTopSide->Create( 28, 0, 180, 35, &g_MainInterfaceTextureArchive, resourceimageindex );
	pLeftTopSide->SetClipRect(58, 0, 210, 36);
	InsertChildObject( pLeftTopSide );

	// 우상
	_XImageStatic* pRightTopSide = new _XImageStatic;
	pRightTopSide->Create( 180, 0, 332, 35, &g_MainInterfaceTextureArchive, resourceimageindex );
	pRightTopSide->SetClipRect(58, 52, 210, 88);
	InsertChildObject( pRightTopSide );

	// 좌하
	_XImageStatic* pLeftBottomSide = new _XImageStatic;
	pLeftBottomSide->Create( 28, 97, 180, 112, &g_MainInterfaceTextureArchive, resourceimageindex );
	pLeftBottomSide->SetClipRect(58, 37, 210, 52);
	InsertChildObject( pLeftBottomSide );

	// 우하 
	_XImageStatic* pRightBottomSide = new _XImageStatic;
	pRightBottomSide->Create( 180, 97, 332, 112, &g_MainInterfaceTextureArchive, resourceimageindex );
	pRightBottomSide->SetClipRect(58, 89, 210, 104);
	InsertChildObject( pRightBottomSide );

	return TRUE;
}

void _XWindow_GuideLine::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return;
	_XWindow::Draw(pfocusobject);
	
	g_XBaseFont->SetColor(0xff000000);		

	switch(m_GuideLineStep) 
	{
	case GS_FIRST :
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
				g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				TCHAR tempBuf[256];
				memset( tempBuf, 0, sizeof(tempBuf) );
				strcpy( tempBuf, _XGETINTERFACETEXT(ID_STRING_NEW_2781) );
				strcat( tempBuf, _XGETINTERFACETEXT(ID_STRING_NEW_2782) );
				strcat( tempBuf, _XGETINTERFACETEXT(ID_STRING_NEW_2783) );

				g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+(m_WindowSize.cx/2), m_WindowPosition.y+40, tempBuf, 300, _XFONT_ALIGNTYPE_CENTER, 1.0f, 3 );
				g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+(m_WindowSize.cx/2)+1, m_WindowPosition.y+40, tempBuf, 300, _XFONT_ALIGNTYPE_CENTER, 1.0f, 3 );
			}	
			else
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2), m_WindowPosition.y+40, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2781) );	// _T("체력/내공 회복이 필요합니다.") 
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2)+1, m_WindowPosition.y+40, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2781) );		// _T("체력/내공 회복이 필요합니다.")
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2), m_WindowPosition.y+60, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2782) );		//_T("무공창[S]을 열어 내공심법 무공(예-양생도인술)을")
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2)+1, m_WindowPosition.y+60, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2782) );		//_T("무공창[S]을 열어 내공심법 무공(예-양생도인술)을")
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2), m_WindowPosition.y+80, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2783) );		//_T("단축창에 등록하십시오.")
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2)+1, m_WindowPosition.y+80, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2783) );		//_T("단축창에 등록하십시오.")
			}			
		}
		break;
	case GS_SECOND :
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
				g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				TCHAR tempBuf[256];
				memset( tempBuf, 0, sizeof(tempBuf) );
				strcpy( tempBuf, _XGETINTERFACETEXT(ID_STRING_NEW_2784) );
				strcat( tempBuf, _XGETINTERFACETEXT(ID_STRING_NEW_2785) );

				g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+(m_WindowSize.cx/2), m_WindowPosition.y+40, tempBuf, 300, _XFONT_ALIGNTYPE_CENTER, 1.0f, 3 );
				g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+(m_WindowSize.cx/2)+1, m_WindowPosition.y+40, tempBuf, 300, _XFONT_ALIGNTYPE_CENTER, 1.0f, 3 );
			}	
			else
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2), m_WindowPosition.y+50, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2784) );//_T("단축키(1~0)을 누르거나 해당 단축창을")
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2)+1, m_WindowPosition.y+50, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2784) );//_T("단축키(1~0)을 누르거나 해당 단축창을")
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2), m_WindowPosition.y+70, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2785) );//_T("클릭해서 내공심법 무공을 활성화 시키십시오.")
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2)+1, m_WindowPosition.y+70, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2785) );//_T("클릭해서 내공심법 무공을 활성화 시키십시오.")
			}			
		}
		break;
	case GS_THIRD :
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
				g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				TCHAR tempBuf[256];
				memset( tempBuf, 0, sizeof(tempBuf) );
				strcpy( tempBuf, _XGETINTERFACETEXT(ID_STRING_NEW_2786) );
				strcat( tempBuf, _XGETINTERFACETEXT(ID_STRING_NEW_2787) );

				g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+(m_WindowSize.cx/2), m_WindowPosition.y+40, tempBuf, 300, _XFONT_ALIGNTYPE_CENTER, 1.0f, 3 );
				g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+(m_WindowSize.cx/2)+1, m_WindowPosition.y+40, tempBuf, 300, _XFONT_ALIGNTYPE_CENTER, 1.0f, 3 );
			}	
			else
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2), m_WindowPosition.y+50, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2786) );//_T("체력과 내공을 회복하려면 아래 버튼을 클릭하거나")
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2)+1, m_WindowPosition.y+50, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2786) );//_T("체력과 내공을 회복하려면 아래 버튼을 클릭하거나")
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2), m_WindowPosition.y+70, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2787) );//_T("단축키[P]를 눌러 운기요상을 진행하십시오.")
				g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2)+1, m_WindowPosition.y+70, _XFONT_ALIGNTYPE_CENTER,
					_XGETINTERFACETEXT(ID_STRING_NEW_2787) );//_T("단축키[P]를 눌러 운기요상을 진행하십시오.")
			}			
		}
		break;
	}
	g_XBaseFont->Flush();
}

BOOL _XWindow_GuideLine::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow )		return FALSE;
	if( !this->m_Enable  )			return FALSE;
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	return TRUE;
}

void _XWindow_GuideLine::ShowWindow(BOOL show)
{
	if( !show )
	{
		SetGuideLIneStep(GS_FIRST);
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_GuideLine::MoveWindow(int X, int Y)
{

}

void _XWindow_GuideLine::SetGuideLIneStep(GuideLineStep step)
{
	m_GuideLineStep = step;

	_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);	
	
	switch(m_GuideLineStep) 
	{
	case GS_FIRST :
		{
			if( g_pQuickSlot_Window )
				g_pQuickSlot_Window->SetDrawGuideLine(FALSE);

			if( pMainMenu_Window )
				pMainMenu_Window->SetDrawGuideLine(FALSE);
		}
		break;
	case GS_SECOND:
		{
			if( g_pQuickSlot_Window )
				g_pQuickSlot_Window->SetDrawGuideLine(TRUE);
			if( pMainMenu_Window )
				pMainMenu_Window->SetDrawGuideLine(FALSE);
		}
		break;
	case GS_THIRD:
		{			
			if( g_pQuickSlot_Window )
				g_pQuickSlot_Window->SetDrawGuideLine(FALSE);
			if( pMainMenu_Window )
				pMainMenu_Window->SetDrawGuideLine(TRUE);
		}
		break;
	}
}