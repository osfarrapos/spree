// XWindow_Poison.cpp: implementation of the _XWindow_Poison class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_Poison.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Poison::_XWindow_Poison()
{
	m_imgPaewangPoison = NULL;
	m_imgBossPoison = NULL;
	m_Mode = 0;
}

_XWindow_Poison::~_XWindow_Poison()
{

}

BOOL _XWindow_Poison::Initialize( void )
{
	// ÆÐ¿Õ¸ª Áßµ¶¾ÆÀÌÄÜ
	int imageindex = g_MainInterfaceTextureArchive.FindResource( "Condition_icon_01.tga" );

	RECT rect = { 33, 33, 63, 63 };
			
	m_imgBossPoison = new _XImageStatic;
	m_imgBossPoison->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
	m_imgBossPoison->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
	m_imgBossPoison->SetScale(0.94f, 0.94f);

	rect.left = 0;
	rect.top = 33;
	rect.right = 33;
	rect.bottom = 63;
	m_imgPaewangPoison = new _XImageStatic;
	m_imgPaewangPoison->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
	m_imgPaewangPoison->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
	m_imgPaewangPoison->SetScale(0.94f, 0.94f);

	m_Mode = _MODE_NONE;

	return TRUE;
}

void _XWindow_Poison::DestroyWindow( void )
{
	SAFE_DELETE(m_imgBossPoison);
	SAFE_DELETE(m_imgPaewangPoison);
	
	_XWindow::DestroyWindow();
}

void _XWindow_Poison::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw(pfocusobject);

	if( !this->m_ShowWindow ) 
		return;

//	_XLog("[DRAW] Poison window call");

	if(m_Mode == _MODE_BOSS)
	{
//		_XLog("[DRAW] Poison window == mode boss");
		if( g_pLocalUser->m_BossStartTime != 0 )
		{
//			_XLog("[DRAW] Poison window == start time [%d]", g_pLocalUser->m_BossStartTime);

			m_imgBossPoison->Draw(m_WindowPosition.x+4, m_WindowPosition.y+7);				
			_XDrawRectAngle(m_WindowPosition.x+4, m_WindowPosition.y+7, m_WindowPosition.x+33, m_WindowPosition.y+36, 0.0f, D3DCOLOR_ARGB(255, 158, 155, 148));
			
			g_XBaseFont->SetColor( _XSC_DEFAULT );
			g_XBaseFont->Puts(m_WindowPosition.x+40, m_WindowPosition.y+9, _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2155));
			
			g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 254, 151, 33 ));	
			
			if( (g_LocalSystemTime - g_pLocalUser->m_BossStartTime) < g_pLocalUser->m_BossTimeLimit*1000	 )
			{
				int temptime = g_pLocalUser->m_BossTimeLimit - (g_LocalSystemTime - g_pLocalUser->m_BossStartTime)/1000;
				int	minute	 =  temptime / 60;
				int	second	 =  temptime % 60;

				g_XBaseFont->Print( m_WindowPosition.x+40, m_WindowPosition.y+24, 1.0f, "%s %02d:%02d", 
					_XGETINTERFACETEXT(ID_STRING_JINSTATUS_2156), minute, second );

				g_XBaseFont->Flush();
			}
			else if( (g_LocalSystemTime - g_pLocalUser->m_BossStartTime) < (g_pLocalUser->m_BossTimeLimit+5)*1000 )
			{
				g_XBaseFont->Print( m_WindowPosition.x+4, m_WindowPosition.y+24, 1.0f, _T("%s 00:00"), 
					_XGETINTERFACETEXT(ID_STRING_JINSTATUS_2156) );

				g_XBaseFont->Flush();
			}
			else
			{
				g_pLocalUser->m_BossStartTime = 0;
				g_pLocalUser->m_BossTimeLimit = 0;
				
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2157), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );

				this->SetMode(_MODE_NONE);
				this->ShowWindow(FALSE);
			}

			g_XBaseFont->Flush();
		}	
	}
	else if(m_Mode == _MODE_PAEWANG)
	{
		m_imgPaewangPoison->Draw( m_WindowPosition.x+4, m_WindowPosition.y+7);				
		_XDrawRectAngle( m_WindowPosition.x+4, m_WindowPosition.y+7, m_WindowPosition.x+33, m_WindowPosition.y+36, 0.0f, D3DCOLOR_ARGB(255, 158, 155, 148));
		
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->SetBoldMode( TRUE );
		g_XBaseFont->Puts( m_WindowPosition.x+40, m_WindowPosition.y+15, _XGETINTERFACETEXT(ID_STRING_NEW_2754) );// _T("ÆÐ¿Õµ¶¹« Áßµ¶»óÅÂ")
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode( FALSE );

		g_XBaseFont->Flush();
	}
}

BOOL _XWindow_Poison::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	return TRUE;
}

void _XWindow_Poison::ShowWindow(BOOL show)
{
    _XWindow::ShowWindow( show );
}