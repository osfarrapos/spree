// XWindow_MonsterStatus.h: interface for the _XWindow_MonsterStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MONSTERSTATUS_H__54407819_326C_40F4_9BAA_95BBD55A7333__INCLUDED_)
#define AFX_XWINDOW_MONSTERSTATUS_H__54407819_326C_40F4_9BAA_95BBD55A7333__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XWindow.h"

extern  RECT g_MobTargetIconPos[_XTARGET_MAX];

// 상태효과 리스트
struct _X_MS_State
{
	int		nType;			// 0:spell	1:effect  2:condition  3:resist

	// resist 일때
	// 1 : 음
	// 2 : 양
	// 3 : 혼
	// 4 : 신
	// spell/effect 일때는 id
	int		nId;
	
	int		nStep;

	_XImageStatic* imageIcon;

	_X_MS_State()
	{
		nType = 0;
		nId = 0;
		nStep = 0;
		imageIcon = NULL;
	}
};

typedef vector<_X_MS_State*>	_vec_MSState;

class _XWindow_MonsterStatus : public _XWindow  
{
private :
	
	_XImageStatic*			m_pMonsterLevelIndicator;
	_XImageStatic*			m_pTargetClanIcon;

	_XItem*					m_pLastCalculateTarget;

	_XButton*				m_PopUpButton[2];

	int						m_TargetGroupIndex;
	BOOL					m_ShowPopUp;

	_XImageStatic			m_AdditionalImage[6];

	
	_vec_MSState			m_vecState;

	_XImageStatic*			m_pExpandBorder; //Author : 양희왕 //breif : 일본 몬스터 창 변경
	_XImageStatic*			m_pExpandBossBorder[2];

	_XButton*				m_pMonsterStatusButton[2]; //Author : 양희왕 //breif : 일본 몬스터 창 변경 창조절해야한다..
	_XButton*				m_pCloseButton;

public :
	_XButton*				m_CharInfoPopUpButton[4];
	BOOL					m_ShowTargetInfoPopUp;

	_XPICKINGTARGETTYPE		m_TargetType;
	_XItem*					m_Target;

	_XStickGauge*			m_pMonsterStaminaGauge;


public:	

	_XImageStatic			m_StateImage[32];
	int						m_StateCount;
	
	DWORD					m_PrevDrawTime;
	FLOAT					m_EffectFrame;
	BOOL					m_EffectOrderSub;


public:
	_XWindow_MonsterStatus();
	virtual ~_XWindow_MonsterStatus();

	BOOL Initialize( void );
	void DestroyWindow( void );

	void SetDefaultPosition( void );
	void SaveWindowProperty( void );
	
	void ShowWindow(BOOL show);
	void MoveWindow(int X, int Y);
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	int	 GetTargetGroupIndex() { return m_TargetGroupIndex;	};

	void	SetStateImage(void);
	BOOL	GetIconRect(short conditionid, int& resourceindex, RECT& rect, bool smallicon);
	
	void	SetPKMode(void);
	void	ResetPKMode(void);
	
	void	SetTargetGroupIndexNTexture(_XItem* target, _XPICKINGTARGETTYPE targettype);
	void	SetTargetClanIconFromModeChange(_XItem* target, _XPICKINGTARGETTYPE targettype);

	void	DrawTargetClanIcon(void);

	void	DrawMonster(void);
	void	DrawUser(void);
	void	DrawNPC(void);
	void	DrawSelf(void);
	void	DrawFunctionalObject(void);
	void	DrawDummy(void);

	void	DrawStateImage(void);
	void	DrawLevel(int level);
	void	DrawName();
};

#endif // !defined(AFX_XWINDOW_MONSTERSTATUS_H__54407819_326C_40F4_9BAA_95BBD55A7333__INCLUDED_)
