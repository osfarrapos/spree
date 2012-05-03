// xwndow_trainingspell.cpp: implementation of the _XWindow_TrainingSpell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xwindow_trainingspell.h"

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

}

BOOL _XWindow_TrainingSpell::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	// TODO

	if(!_XWindow_Training::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_TrainingSpell::ShowWindow(BOOL show)
{
	if(show)
	{
		SetBackground(m_WindowPosition.x + 44, m_WindowPosition.y + 75);
	}
	ShowBackground(show);
	_XWindow_Training::ShowWindow(show);
}