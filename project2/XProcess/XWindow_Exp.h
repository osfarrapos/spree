// XWindow_Exp.h: interface for the _XWindow_Exp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_EXP_H__A38A9BA3_EB4D_46DB_94C2_AD7F4697DEEE__INCLUDED_)
#define AFX_XWINDOW_EXP_H__A38A9BA3_EB4D_46DB_94C2_AD7F4697DEEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XTextureAnimation.h"


class _XWindow_Exp  : public _XWindow
{
private : 
	_XButton*			m_pBtnConvergence;
	_XStickGauge*		m_pFuryGauge;
	_XStickGauge*		m_pExpGauge;

	DWORD				m_dwConvergenceStartTime;
	BOOL				m_bSendConvergencePacket;

	_XTextureAnimation*	m_pConvergenceAnimation;
	
public:
	_XWindow_Exp();
	virtual ~_XWindow_Exp();

	BOOL		Initialize();
	void		DestroyWindow();

	void		Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL		Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void		SetParameter(void);

	void		ClearConvergence(void);
	void		ShowConvergenceAnimation(BOOL show);
	void		ProcessConvergence(void);

	FLOAT		GetExpGaugeFactor();
	FLOAT		GetFuryGaugeFactor();

	void		SetConvergenceStartTime(DWORD time)	
	{		
		m_dwConvergenceStartTime = time;	
	};
	
	void		ShowWindow(BOOL show);

};

#endif // !defined(AFX_XWINDOW_EXP_H__A38A9BA3_EB4D_46DB_94C2_AD7F4697DEEE__INCLUDED_)
