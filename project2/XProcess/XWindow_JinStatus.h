// XWindow_JinStatus.h: interface for the _XWindow_JinStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_JINSTATUS_H__68DC14E4_850D_4695_AF49_1900FE3621C9__INCLUDED_)
#define AFX_XWINDOW_JINSTATUS_H__68DC14E4_850D_4695_AF49_1900FE3621C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XTextureAnimation; //author : 양희왕 // 불꽃 애니

class _XWindow_JinStatus : public _XWindow  
{
public :
	_XImageStatic*	m_PehwangPoisonIcon;				//패황릉 중독 아이콘 
	_XImageStatic*	m_BossRoomPoisonIcon;				//생사결 중독 아이콘 
	_XImageStatic*	m_BossRoomPoisonTimerBorder1;		//생사결 중독시간 바탕 1 
	_XImageStatic*	m_BossRoomPoisonTimerBorder2;		//생사결 중독시간 바탕 2
	_XImageStatic*	m_BossRoomPoisonTimerBorder3;		//생사결 중독시간 바탕 3
	_XImageStatic*	m_LevelUpImage[6];
	_XImageStatic*	m_FuryParameterImage[24];	
	_XImageStatic*	m_ConvergenceImage[5];

	_XStickGauge*	m_pLifePowerBorder;
	_XStickGauge*	m_pLifePowerDamageBorder;
	_XStickGauge*	m_pForcePowerBorder;
	_XStickGauge*	m_pForcePowerDamageBorder;
	
	_XButton*		m_LevelUpButton;
	_XButton*		m_ConvergenceButton;
	_XButton*		m_AbilityUpButton;
	
	
	LONG			m_SpeedCounter;	
	LONG			m_LevelUpSpeedCounter;
	
	DWORD			m_ConvPrevDrawTime;	
	DWORD			m_ConvergenceStartTime;

	FLOAT			m_JinBorderFrame;	
	FLOAT			m_JinMaxEffectFrame;
	FLOAT			m_AccumulatedImageBorderFrame;
	FLOAT			m_FireEffectFrame;
	FLOAT			m_SelectedEffectFrame;
	FLOAT			m_AbilityUpFrame;
	FLOAT			m_ConvEffectFrame;
	

	int				m_JinBorderPrevFrame;	
	int				m_JinMaxEffectPrevFrame;	
	int				m_AccumulatedImageBorderPrevFrame;	
	int				m_FireEffectPrevFrame;	
	int				m_JinGaugePercentage;	
	int				m_SelectedEffectPrevFrame;
	int				m_CurAbilityUpCount;
	

	BOOL			m_bViewBossRoomPoisonTimer;			//생사결 중독시간이 보이는지 아닌지 결정하는 플래그
	BOOL			m_bViewPehwangPoisonIcon;			// 패황릉 중독아이콘을 보일지를 결정하는 변수
	BOOL			m_EffectOrderSub;
	
	bool			m_bConvergenceSendPacket;
	bool			m_ConvEffectOrderSub;
	
	_XImageStatic*	m_pClassIcon;		// 직책 아이콘

	_XImageStatic	m_JinBorderImage;
	_XImageStatic	m_JinGaugeImage;
	_XImageStatic	m_JinMaxImage;	
	_XImageStatic	m_AccumulatedJinImage[8];	
	_XImageStatic	m_LeftFireImage;
	_XImageStatic	m_RightFireImage;


	_XTextureAnimation* m_pFireImage;

public :
	_XButton*		m_btnSDSOnOff;
	int				m_nSDSState;
	DWORD			m_dwSDSSetTime;
	

public:
	_XWindow_JinStatus();
	virtual ~_XWindow_JinStatus();
	
	
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void SetDefaultPosition( void );
	void SaveWindowProperty( void );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void MoveWindow( int X, int Y );
	void ShowWindow( BOOL show );
	
	//2004.06.24->oneway48 insert
	void DrawLevelUp();
	void SetButtonStatus( BOOL Show );
	
	void DrawBossRoomPoisonTimer(void);
	void DrawPehwangPoisonIcon(void);
	void DrawMatchWaitTimer(void);
	void DrawTooltip(void);

	void DrawFuryParameter(void);
	void ProcessConvergence(void);
	void ClearConvergence(void);

	void SetParameter( void );

	void SetSDSState(int state);	// 0:on 1:off 2:diable
	void SetClassIcon(void);
	
	void SetBossroomPoisonTimer(BOOL show)		{	m_bViewBossRoomPoisonTimer = show;	};
	void SetPehwangPoisonIcon(BOOL show)		{	m_bViewPehwangPoisonIcon = show;	};
	void SetConvergenceStartTime(DWORD time)	{	m_ConvergenceStartTime = time;		};

	int	GetSDSState(void)						{	return m_nSDSState;					};
	void SetSDSSetTime(DWORD time)				{	m_dwSDSSetTime = time;				};
	DWORD GetSDSSetTime(void)					{	return m_dwSDSSetTime;				};
};

#endif // !defined(AFX_XWINDOW_JINSTATUS_H__68DC14E4_850D_4695_AF49_1900FE3621C9__INCLUDED_)





















