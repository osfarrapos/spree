// XWindow_CBOrgRecord.h: interface for the _XWindow_CBOrgRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CBORGRECORD_H__D55FC99B_2D06_42E2_B687_BE061D5A4DA4__INCLUDED_)
#define AFX_XWINDOW_CBORGRECORD_H__D55FC99B_2D06_42E2_B687_BE061D5A4DA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

struct _XRank
{
	TCHAR			cName[13];		// 이름
	int				nRank;			// 순위
	int				nCombatPoint;	// 대전 점수
	DWORD			dwExp;			// 경험치
	int				nFame;			// 명성
	DWORD			dwMoney;			// 포상금
};

class _XWindow_CBOrgRecord  : public _XWindow
{
private :
	_XRank			m_Rank[10];
	
public:
	_XWindow_CBOrgRecord();
	virtual ~_XWindow_CBOrgRecord();

	BOOL			Initialize( void );
	void			DestroyWindow( void );
	
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void			SetRank(_OneRank* rank);
	void			ShowWindow(BOOL show);


};

#endif // !defined(AFX_XWINDOW_CBORGRECORD_H__D55FC99B_2D06_42E2_B687_BE061D5A4DA4__INCLUDED_)
