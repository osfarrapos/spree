// XWindow_MatchTablePVP.h: interface for the _XWindow_MatchTablePVP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MATCHTABLEPVP_H__E3501861_B7C5_4689_ABC2_9DCE4957401B__INCLUDED_)
#define AFX_XWINDOW_MATCHTABLEPVP_H__E3501861_B7C5_4689_ABC2_9DCE4957401B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_MatchTablePVP : public _XWindow
{
private :
	_XStatic			m_BorderTile;

	_XCheckButton*		m_chkbtnMatchInfo;
	_XCheckButton*		m_chkbtnMatchTablePVP;
	_XCheckButton*		m_chkbtnMatchTableGroup;
	_XCheckButton*		m_chkbtnMatchTableRage;

	_XImageStatic		m_ItemEdgeIcon[2];

	_XButton*			m_btnMoney;

	_XImageStatic		m_MoneyIconImage;
	_XImageStatic		m_MoneyBackImage;

	_XImageStatic		m_LeftTitleBarImage;
	_XImageStatic		m_RightTitleBarImage;
	_XImageStatic		m_VsImage;

	_XButton*			m_btnRecordType;
	_XButton*			m_btnMatchType;
	_XButton*			m_btnMatchTime;
	_XButton*			m_btnUseHyperrun;
	_XButton*			m_btnUseBuff;
	_XButton*			m_btnMatchBalance;

	_XHSlider*			m_sldMatchBalance;

	_XImageStatic		m_GradeBorderImage;
	_XImageStatic		m_GradeImage;

public :
	_XListBox*			m_lboxRecordType;
	_XListBox*			m_lboxMatchType;
	_XListBox*			m_lboxMatchTime;
	_XListBox*			m_lboxUseHyperrun;
	_XListBox*			m_lboxUseBuff;
	_XListBox*			m_lboxMatchBalance;

	TCHAR				m_strRecordType[128];
	TCHAR				m_strMatchType[128];
	TCHAR				m_strMatchTime[128];
	TCHAR				m_strUseHyperrun[128];
	TCHAR				m_strUseBuff[128];
	TCHAR				m_strMatchBalance[128];
	
	_XButton*			m_btnConfirm;
	_XButton*			m_btnCancel;
	
public:
	_XWindow_MatchTablePVP();
	virtual ~_XWindow_MatchTablePVP();

	BOOL		Initialize(void);
	void		DestroyWindow(void);

	void		Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL		Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void		ShowWindow(BOOL show);
	void		ResetData(void);
	void		MoveWindow(int X, int Y);

	void		SetGradeImageRect(unsigned char ucGrade, RECT& rect);
	void		ChangeTab(int tab);
};

#endif // !defined(AFX_XWINDOW_MATCHTABLEPVP_H__E3501861_B7C5_4689_ABC2_9DCE4957401B__INCLUDED_)
