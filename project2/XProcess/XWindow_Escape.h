// XWindow_Escape.h: interface for the _XWindow_Escape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_ESCAPE_H__75E7A43D_3A3F_4964_A159_A1FD72ED0394__INCLUDED_)
#define AFX_XWINDOW_ESCAPE_H__75E7A43D_3A3F_4964_A159_A1FD72ED0394__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XWindow_ChargeGauge.h"

typedef struct XEscapeInfo
{
	int		currentmapid;			// 현재 맵 인덱스
	int		village_stringid_01;	// 공통(낭인)
	int		village_stringid_02;	// 백도
	int		village_stringid_03;	// 흑도
	int		areaid_common;			//공통
	int		areaid_white;			//백도
	int		areaid_black;			//흑도
}_XEscapeInfo;


class _XWindow_Escape  : public _XWindow
{
public:
	_XWindow_ChargeGauge*		m_pChargeGaugeWindow;
	_XStatic					m_BorderTile;
	
	int							m_SelectedVillageStringID;	//현재 선택된 지역에서 이동될 마을 이름
	int							m_SelectedAreaID;			//현재 선택된 지영게서 이동될 영역 아이디
	
	TCHAR						m_SelectedVillageName[512];	// 선택된 마을 이름

	list <_XEscapeInfo*>		m_listEscapeInfo;			// 탈출 정보를 가지는 리스트 
public:
	_XWindow_Escape();
	virtual ~_XWindow_Escape();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);

	void			SetSelectedEscapeInfo(void);//현재 선택된 지역에서 이동될 마을 이름과 영역 아이디를 얻어 온다.
	
	BOOL			LoadEscapeInfo(void);	// 스크립트에서 각각 지역에 따른 탈출 정보를 얻어온다.
};

#endif // !defined(AFX_XWINDOW_ESCAPE_H__75E7A43D_3A3F_4964_A159_A1FD72ED0394__INCLUDED_)
