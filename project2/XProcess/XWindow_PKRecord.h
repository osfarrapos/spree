// XWindow_PKRecord.h: interface for the _XWindow_PKRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PKRECORD_H__B995F00C_5BEB_45E2_9E12_8E86950F9930__INCLUDED_)
#define AFX_XWINDOW_PKRECORD_H__B995F00C_5BEB_45E2_9E12_8E86950F9930__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

struct _XPKRecord
{
	int			option;
	
	int			year;
	int			month;
	int			day;
	int			hour;
	int			minute;

	int			group;
	TCHAR		name[13];

	_XPKRecord()
	{
		option	= 0;
		year	= 0;
		month	= 0;
		day		= 0;
		hour	= 0;
		minute	= 0;

		memset(name, 0, sizeof(name));
		group	= 0;
	}
};

struct _XPKCount
{
	int			win;
	int			lose;

	_XPKCount()
	{
		win		= 0;
		lose	= 0;
	}
};

class _XWindow_PKRecord : public _XWindow
{
public :
	int					m_TotalWinCount;
	int					m_TotalLoseCount;
	int					m_TotalWinRate;			// ÀüÃ¼ ½Â·ü

	_XPKRecord			m_WinRecord[10];
	_XPKRecord			m_LoseRecord[10];

	_XPKCount			m_GroupRecord[9];
	_XPKCount			m_ClassRecord[8];
	
private :
	_XCheckButton*		m_RecordTab[5];
	int					m_CurrentSelectedTab;
	
	_XImageStatic		m_LBigBorder;
	_XImageStatic		m_RBigBorder;

	_XImageStatic		m_LWinBorder;
	_XImageStatic		m_RWinBorder;

	_XImageStatic		m_LLoseBorder;
	_XImageStatic		m_RLoseBorder;

	_XImageStatic		m_LWinRateBorder_u;
	_XImageStatic		m_LWinRateBorder_d;
	_XImageStatic		m_RWinRateBorder_u;
	_XImageStatic		m_RWinRateBorder_d;
	_XImageStatic		m_UWinRateBorder;
	_XImageStatic		m_DWinRateBorder;
	
	_XImageStatic		m_UGroupRateBorder_l;
	_XImageStatic		m_UGroupRateBorder_r;

	_XImageStatic		m_LGroupRateBorder;
	_XImageStatic		m_RGroupRateBorder;

	_XImageStatic		m_LClassRateBorder;
	_XImageStatic		m_RClassRateBorder;

	_XImageStatic		m_TableColumnBorder;
	_XImageStatic		m_TableRowBorder;
	_XImageStatic		m_TableRateBorder;
	_XImageStatic		m_TableFinishBorder;

	_XImageStatic		m_GroupIcon[8];

	int					m_GroupIndex[8];

public:
	_XWindow_PKRecord();
	virtual ~_XWindow_PKRecord();

	BOOL		Initialize(void);
	void		DestroyWindow(void);

	void		Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL		Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void		RefreshData(void);
	
	void		ChangeTab(int tab);
	void		DrawPKRecord(void);
	void		DrawTotalRecord(void);
	BOOL		DrawTooltip(void);

	void		ResetRecord(void);
	
	BOOL		GetRecentRecord(int win, int index);

};

#endif // !defined(AFX_XWINDOW_PKRECORD_H__B995F00C_5BEB_45E2_9E12_8E86950F9930__INCLUDED_)
