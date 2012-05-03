// XWindow_RotateWindow.cpp: implementation of the _XWindow_RotateWindow class.
//
// Created Date		: 2003.05.06
// Author			: Sohyun, Park (kukuri)
// Desc				: Character Zoom Window - Character Create
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "XWindow_RotateWindow.h"
#include "XProc_ProcessDefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_RotateWindow::_XWindow_RotateWindow()
{
	m_BaseSlider = NULL;
	m_LeftButton = NULL;
	m_RightButton = NULL;
}

_XWindow_RotateWindow::~_XWindow_RotateWindow()
{

}

BOOL _XWindow_RotateWindow::Initialize(_XTextureManager& texturearchive)
{
	_XSLIDERCONTROL_STRUCTURE basesliderstruct = 
	{
		TRUE,
		{49, 5}, {176, 8},
		_XDEF_CC_ROTATE_SLIDER,
		-1,
		-1,
		texturearchive.FindResource("cs_scrollbar.tga"),
		texturearchive.FindResource("cs_scrollbar.tga"),
		texturearchive.FindResource("cs_scrollbar.tga"),
		{0, 0}, {21, 9},
		-1, //texturearchive.FindResource("scrollbar_base.tga"),
		&texturearchive,
		" "
	};
	m_BaseSlider = new _XHSlider;
	m_BaseSlider->Create(basesliderstruct);
	m_BaseSlider->SetScale();
	m_BaseSlider->ShowWindow(TRUE);
	m_BaseSlider->SetSlidingPosition2(70);
	InsertChildObject(m_BaseSlider);

	m_OriginSlidingPosition = 70;
	
	_XBTN_STRUCTURE leftbtn = 
	{
		TRUE, 
		{40, 5}, {8, 9},
		_XDEF_CC_ROTATE_LEFTBUTTON,
		texturearchive.FindResource("button_scrollleft_normal.tga"),
		texturearchive.FindResource("button_scrollleft_push.tga"),
		texturearchive.FindResource("button_scrollleft_highlight.tga"),
		&texturearchive
	};
	m_LeftButton = new _XButton;
	m_LeftButton->Create(leftbtn);
	m_LeftButton->setMessageMode( _BTNMESSAGEMODE_BTNDOWN );
	InsertChildObject(m_LeftButton);
	
	_XBTN_STRUCTURE rightbtn = 
	{
		TRUE, 
		{225, 5}, {8, 9},
		_XDEF_CC_ROTATE_RIGHTBUTTON,
		texturearchive.FindResource("button_scrollright_normal.tga"),
		texturearchive.FindResource("button_scrollright_push.tga"),
		texturearchive.FindResource("button_scrollright_highlight.tga"),
		&texturearchive
	};
	m_RightButton = new _XButton;
	m_RightButton->Create(rightbtn);
	m_RightButton->setMessageMode( _BTNMESSAGEMODE_BTNDOWN );
	InsertChildObject(m_RightButton);
	
	SetWindowMoveMode(_XWMOVE_FIXED);

	return TRUE;
}

void _XWindow_RotateWindow::DestroyWindow(void)
{
	
}

void _XWindow_RotateWindow::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;
}

BOOL _XWindow_RotateWindow::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_RotateWindow::SubSlidingPosition(void)
{
	m_OriginSlidingPosition = m_BaseSlider->m_SlidingPosition;

	m_OriginSlidingPosition -= 5;
	if(m_OriginSlidingPosition < 0)
		m_OriginSlidingPosition = 0;

	m_BaseSlider->SetSlidingPosition2(m_OriginSlidingPosition);
}

void _XWindow_RotateWindow::AddSlidingPosition(void)
{
	m_OriginSlidingPosition = m_BaseSlider->m_SlidingPosition;

	m_OriginSlidingPosition += 5;
	if(m_OriginSlidingPosition >= 176 - 21)
		m_OriginSlidingPosition = 176 - 21;
	
	m_BaseSlider->SetSlidingPosition2(m_OriginSlidingPosition);
}