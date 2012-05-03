// XWindow_EnterBossRoom.h: interface for the _XWindow_EnterBossRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_ENTERBOSSROOM_H__D197E0AA_1A56_4688_8C0D_B1B988E9C516__INCLUDED_)
#define AFX_XWINDOW_ENTERBOSSROOM_H__D197E0AA_1A56_4688_8C0D_B1B988E9C516__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XGameItem.h"

struct  _sNeedItem
{
	char	cType ;		// 대분류..
	short	sID ;		// 아이템 아이디..
	bool	bNeedItem ;	// 필요 아이템을 장착했으면 TRUE
};

struct _xNeedItemRef
{
	int		cType;		// 아이템 타입
	int		sID;		// 아이템 아이디
};

struct _xItemInSlot
{
	int		cType;
	int		sID;
	int		nInvenNumber;
	BOOL	bInsert;
};

struct  _sStateOfEnterBoss //파티원의 보스전 진입 진행 상태
{
	int		PartyMemberNo ;		// 파티원 번호
	BOOL	bJoin;				// 승락 : TRUE, 거부 : FALSE
};

class _XWindow_Inventory;

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
	#define _XDEF_BOSSROOM_NEEDITEMNUMBER	4		// item slot 갯수
#else
	#define _XDEF_BOSSROOM_NEEDITEMNUMBER	3		// item slot 갯수
#endif

#define _XDEF_BOSSROOM_NEEDITEMTYPE		3			// 필요한 아이템 종류 - 자색, 녹색, 적색

class _XWindow_EnterBossRoom : public _XWindow    
{
public:
	_XWindow_EnterBossRoom();
	virtual ~_XWindow_EnterBossRoom();
	
public:
	_XButton*				m_OKButton;
	_XButton*				m_CancelButton;

	_sNeedItem				m_NeedItemID[_XDEF_BOSSROOM_NEEDITEMNUMBER];
	_XTradeItem				m_NeedItem[_XDEF_BOSSROOM_NEEDITEMNUMBER];
	int						m_CurrentSlotNumber;
	BOOL					m_bSendPartyRequest;
	int						m_BossRoomLevel;		// 생사결 난이도 
	
	_sStateOfEnterBoss		m_EnterPartyMember[9];
	
private :
	_XImageStatic*			m_NeedItemBorder;		// 아이템을 박는 소켓 
	_XImageStatic*			m_ExistNeedItemBorder;		
	_XImageStatic*			m_OKButtonLeftBorder;	
	_XImageStatic*			m_OKButtonRightBorder;

	_XImageStatic*			m_CashItemBorder;
	_XImageStatic*			m_ExistCashItemBorder;


private :
	POINT					m_NeedItemSlotPosition[_XDEF_BOSSROOM_NEEDITEMNUMBER];	// 아이템 소켓 위치

	_xNeedItemRef			m_NeedNormalItemRef[_XDEF_BOSSROOM_NEEDITEMTYPE];
	_xNeedItemRef			m_NeedCashItemRef[_XDEF_BOSSROOM_NEEDITEMTYPE];

public :
	_xItemInSlot			m_ItemInSlot[_XDEF_BOSSROOM_NEEDITEMNUMBER];

public:
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void ShowWindow(BOOL show);
	void MoveWindow( int X, int Y );
	
	void SetDefaultPosition( void );
	void SaveWindowProperty( void );
	
	int  GetEmptySlotNumber(void);
	int  GetFullSlotNumber(void);

	void ResetNeedItemInfo(void);
	void CopyItemToSlot(int slotnumber, int invennumber);
	void CopySlotToInventory(int slotnumber, int invennumber);
	
	void SetNeedItemCheck( int type, int id, bool bneeditem );
	
	BOOL CheckNeedItem( int type, int id, int& reason);
	BOOL CheckRequirement(void);
	
	void	SetSlotPosition(int mapid);									// 아이템 소켓 위치 정렬

	void	SetNeedNormalItemRef(int index, char ctype, short sid);		// 필요 아이템 정보 세팅
	void	ResetNeedNormalItemRef();

	void	SetItemInSlot(int index, int invennumber);
	void	ResetItemInSlot(int index);
	void	ResetItemInSlot();

	void	ProcessItemInSlot();

	void	SetNeedCashItemRef(int index, char ctype, short sid);
	void	ResetNeedCashItemRef();

	void	ResetData();
	
	BOOL	CheckAllSlot(int invennumber);
};

#endif // !defined(AFX_XWINDOW_ENTERBOSSROOM_H__D197E0AA_1A56_4688_8C0D_B1B988E9C516__INCLUDED_)
