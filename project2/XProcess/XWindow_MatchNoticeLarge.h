// XWindow_MatchNoticeLarge.h: interface for the _XWindow_MatchNoticeLarge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MATCHNOTICELARGE_H__77B85ED7_1D62_4290_9676_D8541BA7013D__INCLUDED_)
#define AFX_XWINDOW_MATCHNOTICELARGE_H__77B85ED7_1D62_4290_9676_D8541BA7013D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "EmperorOfDragons.h"

struct _XMatchNoticeInfo
{
	unsigned char		ucMatchType;
	unsigned char		ucMatchState;

	char				cChiefName0[13];
	char				cChiefName1[13];

	_XMatchMemberInfo	Player[2];

	unsigned char		ucMemberCount;

	unsigned char		ucMatchRecord0[9];
	unsigned char		ucMatchRecord1[9];

	DWORD				dwLastRecieveTime;

	TCHAR				strLevelNamePlayer0[128];
	TCHAR				strLevelNamePlayer1[128];
};

class _XWindow_MatchNoticeLarge : public _XWindow
{
private : 
	_XStickGauge*		m_MyVitalGauge;
	_XStickGauge*		m_TargetVitalGauge;

	int					m_SelectedAreaID;
	TCHAR				m_SelectedAreaName[256];
	_XMatchNoticeInfo*	m_SelectedMatchInfo;

	_XImageStatic		m_WinImage;
	_XImageStatic		m_LoseImage;
	_XImageStatic		m_TieImage;

	_XImageStatic		m_DefaultRecordImage;

public :
	map <int, _XMatchNoticeInfo*> m_mapMatchNoticeInfo;
	
public:
	_XWindow_MatchNoticeLarge();
	virtual ~_XWindow_MatchNoticeLarge();

	BOOL			Initialize();
	void			DestroyWindow();

	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void			SetParameter();

	void			DrawTooltip(LPSTR str, int x, int y);

	void			SetSelectedAreaID(int id);
	inline int		GetSelectedAreaID()				{	return m_SelectedAreaID;	};

};

#endif // !defined(AFX_XWINDOW_MATCHNOTICELARGE_H__77B85ED7_1D62_4290_9676_D8541BA7013D__INCLUDED_)
