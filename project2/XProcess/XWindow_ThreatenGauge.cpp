// XWindow_ThreatenGauge.cpp: implementation of the _XWindow_ThreatenGauge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_ThreatenGauge.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_ThreatenGauge::_XWindow_ThreatenGauge()
{
	m_ThreatenGaugeBorder	= NULL;
}

_XWindow_ThreatenGauge::~_XWindow_ThreatenGauge()
{

}

BOOL _XWindow_ThreatenGauge::Initialize()
{
	SetDrawBorder( FALSE );

	_XSTICKGAUGE_STRUCTURE thretengaugestruct =
	{
		TRUE, {0, 0}, {188, 18},
		g_MainInterfaceTextureArchive.FindResource("mi_manor02.tga"),
		g_MainInterfaceTextureArchive.FindResource("mi_manor02.tga"),
		&g_MainInterfaceTextureArchive
	};
	
	m_ThreatenGaugeBorder = new _XStickGauge;
	m_ThreatenGaugeBorder->Create(thretengaugestruct);	
	m_ThreatenGaugeBorder->SetImageClipRect(_XGAUGEIMAGE_OVERLAY, 1, 49, 189, 67);
	m_ThreatenGaugeBorder->SetImageClipRect(_XGAUGEIMAGE_BORDER, 1, 31, 189, 49);
	m_ThreatenGaugeBorder->SetFactorDraw(TRUE);
	m_ThreatenGaugeBorder->SetBackGroundDraw(TRUE);
	m_ThreatenGaugeBorder->SetGaugeFactor( 0.0f );
	m_ThreatenGaugeBorder->SetGaugeColorFactor( 0xFFFFFFFF, 0x00000000 );
	m_ThreatenGaugeBorder->SetViewDelayGauge( FALSE );
	m_ThreatenGaugeBorder->SetGaugeImageDrawType(_XGAUGEIMAGEDRAWTYPE_RESIZE);
	InsertChildObject(m_ThreatenGaugeBorder);

	SetWindowMoveMode(_XWMOVE_FIXED);

	return TRUE;
}

void _XWindow_ThreatenGauge::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_ThreatenGauge::Draw(_XGUIObject*& pfocusobject)
{
	if(!this->m_ShowWindow)
		return;

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO
	g_XBaseFont->SetColor(0xFFFFFF);
	g_XBaseFont->PutsAlign(	m_WindowPosition.x+79, m_WindowPosition.y-16, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3254) );	//"장주를 협박하는 중"
	g_XBaseFont->Flush();
}

BOOL _XWindow_ThreatenGauge::Process( _XGUIObject*& pfocusobject )
{	
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	// TODO

	if( !_XWindow::Process(pfocusobject) ) 
		return FALSE;

	return TRUE;
}

void _XWindow_ThreatenGauge::ShowWindow(BOOL show)
{
	if(show)
	{
		int X, Y;
		SIZE size = this->GetWindowSize();
		X = (gnWidth>>1) - (size.cx>>1);
		Y = 92;

		MoveWindow(X, Y);
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_ThreatenGauge::SetParameter(FLOAT percentage)
{
	m_ThreatenGaugeBorder->SetGaugeFactor(percentage);
}