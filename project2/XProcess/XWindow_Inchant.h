// XWindow_WeaponUpgrade.h: interface for the _XWindow_WeaponUpgrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_WEAPONUPGRADE_H__A98EF4A3_3769_4C6A_878F_A444FC2FB0FC__INCLUDED_)
#define AFX_XWINDOW_WEAPONUPGRADE_H__A98EF4A3_3769_4C6A_878F_A444FC2FB0FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 제련 창
 * 아이템을 제련함
 * 
 * 제련은 1차로 서버에서 결과를 받고 그 결과에 따른 게이지를 보여주게 되어있다.
 * 1차 결과를 받은 후면 비접속종료를 해도 결과는 그대로 남아있다.
 * 성공과 실패2 종류가 있고 각각의 실패에 따라 처리가 틀려진다.
*/

/// 제련 단계
enum InchantStep{ US_NONE=0, US_READY, US_STEP1, US_STEP2, US_STEP3, US_STEP4, US_STEP5 };

class _XWindow_Inchant : public _XWindow  
{
private:
	_XButton*			m_InchantApplyButton;
	_XButton*			m_pApplyCancleButton;
	
	InchantStep			m_InchantStep;
	TCHAR				m_InchantInfoText[128];

	_XImageStatic*		m_slotImage;
	unsigned char		m_FirstResult;

public:
	_XUserItem*			m_InchantItem;
	/// m_InchantStuff[0] 은 필요한 아이템을 표시하기 위한 공간 
	/// m_InchantStuff[1] 은 실제 등록한 아이템
	_XUserItem			m_InchantStuff[3];		// inchant table 확장
												// m_InchantStuff[2]는 필요한 재료 2번째

public:
	_XWindow_Inchant();
	virtual ~_XWindow_Inchant();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void				ShowWindow(BOOL show);
	void				MoveWindow( int X, int Y );
	
	void				SetItemInchantStep(InchantStep step);	///< 단계에 따른 처리
	void				MessagePrint(int msgindex);			///< 에러 메세지
	void				ClickStartButton(void);				///< m_InchantApplyButton 클릭 처리
	void				RebuildStuffInfo(void);

	FLOAT				m_InchantItemtRotateAngle;
	D3DXMATRIX			m_InchantItemRHWMatrix;
	_X3PCamera			m_InchantItemModelViewCamera;
	D3DVIEWPORT9		m_InchantItemModelViewPort;

	void				DrawInchantItem( void );
	int					GetInchantItemModelIndex(void);	

	int					CheckStuffCount(int selectslot);	///< 재료 갯수 검사 : 1 이면 정상 
	void				SetFirstInchantResult(unsigned char result) { m_FirstResult = result; 	};
	unsigned char		GetFirstInchantResult() { return m_FirstResult;	};
	int					CheckWeaponGrade(int searchslot);	///< 무기 등급에 따른 제련 가능 체크 
	
	BOOL				ProcessInchantCashItem(int slotindex);	// 기간제 아이템인지 체크
	BOOL				ProcessInchant();
};

#endif // !defined(AFX_XWINDOW_WEAPONUPGRADE_H__A98EF4A3_3769_4C6A_878F_A444FC2FB0FC__INCLUDED_)
