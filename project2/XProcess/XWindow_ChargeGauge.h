// XWindow_ChargeGauge.h: interface for the _XWindow_ChargeGauge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CHARGEGAUGE_H__FC078968_2F82_4DE0_882C_6964E26566F5__INCLUDED_)
#define AFX_XWINDOW_CHARGEGAUGE_H__FC078968_2F82_4DE0_882C_6964E26566F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 모든 게이지 창
 * 게임에 쓰이는 모든 게이지바를 보여준다.
 * 
 * 각 모드 별로 해당 게이지를 보여주며 관리한다.
 * 게이지바는 독립적이다. 한가지 타입의 게이지만 게임상에 보여준다.
 * 
*/

typedef enum 
{ 
	_XGAUGE_NONE = 0, 
	_XGAUGE_CHARGE,			///< 차지스킬 사용시 
	_XGAUGE_READ,			///< 비급을 읽을시 
	_XGAUGE_REPAIR,			///< 수리시 
	_XGAUGE_SOCKET,			///< 소켓장착시
	_XGAUGE_SOCKETDEATCH,	///< 소켓 제거시
	_XGAUGE_BINSA,			///< 빈사상태시
	_XGAUGE_ELIXIR,			///< 영약복용시
	_XGAUGE_GAMBLE,			///< 겜블을 할 때
	_XGAUGE_INCHANT,		///< 제련을 할 때
	_XGAUGE_SMELTITEM,		///< 용루/ 용린 
//	_XGAUGE_THREATEN,		///< 장원전 협박시
	_XGAUGE_ESCAPE			///< 탈출시
} _XGAUGE_MODE;


class _XWindow_ChargeGauge : public _XWindow 
{
private:
	_XGAUGE_MODE		m_CurrentGaugeMode;
	int					m_iGaugePointAlpha;
	BOOL				m_bGaugePointAlphaFlag;
	BOOL				m_bGaugeFadeOutMode;
	FLOAT				m_dwGaugeDelayTime;	
	FLOAT				m_dwElapsedTime;
	int					m_iSelectedNumber;
	POINT				m_DrawPos;
	DWORD				m_FadeOutElapsedTime;
	DWORD				m_FadeOutAlpha;
	BOOL				m_bReadySandMsg;	///< 이 플래그가 true 일때만 서버로 패킷을 보낼 수 있음
	/// 이 플래그로 게이지바의 동작을 멈추며 이 플래그가 false 일때만 서버로 패킷을 보낼 수 있음
	BOOL				m_bGaugeStop;		

public :
	_XStickGauge*		m_DefaultGaugeBorder;
	_XStickGauge*		m_ReadBookGaugeBorder;
	_XStickGauge*		m_ChargeGaugeBorder;	
	_XImageStatic*		m_GaugePoint;
	
	_XStickGauge*		m_ThreatenGaugeBorder;
	
public:
	_XWindow_ChargeGauge();
	virtual ~_XWindow_ChargeGauge();

	BOOL				Initailize(void);
	void				DestroyWindow(void);

	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void				ShowWindow(BOOL show);
	void				MoveWindow(int X, int Y);

	void				SetSelectedNumber(int number) { m_iSelectedNumber = number;	};
	int					GetSelectedNumber() { return m_iSelectedNumber; };
	/// 출력게이지 설정( 출력게이지가 100%가 되면 서버로 해당 패킷을 보냄)
	void				SetWindowPosition(int& X, int& Y);
	void				SetParameter(FLOAT percentage);
	void				SetGaugeMode( _XGAUGE_MODE mode );
	_XGAUGE_MODE		GetCurrentGaugeMode() { return m_CurrentGaugeMode;	};
	void				SetDelayTime( FLOAT time );
	void				SetDrawPosition(POINT pos) { m_DrawPos = pos; };
	void				StartFadeOutMode();
	void				SetGaugeStop();
	void				StatusBinsa();	///< 빈사 상태시 각종 게이지 모드 닫기
	
	DWORD				GetGaugeDelayTime() { return m_dwGaugeDelayTime; };
	DWORD				GetElapsedTime() { return m_dwElapsedTime; };
};

#endif // !defined(AFX_XWINDOW_CHARGEGAUGE_H__FC078968_2F82_4DE0_882C_6964E26566F5__INCLUDED_)
