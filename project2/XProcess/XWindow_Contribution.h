// XWindow_Contribution.h: interface for the _XWindow_Contribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CONTRIBUTION_H__CD341A79_76BC_4013_A3FE_F0D160C2F24E__INCLUDED_)
#define AFX_XWINDOW_CONTRIBUTION_H__CD341A79_76BC_4013_A3FE_F0D160C2F24E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 문파기여도 창 
 * 아이템과 돈으로 문파기여도를 함
 * 
 * [Apply Clan]
 * 문파의 True/False 결정.
 * Apply Clan 일치할 경우 CP1 적용
 * Apply Clan 불일치할 경우 CP2 적용

 * 0 - 낭인(공통)
 * 1 - 개방
 * 2 - 비궁
 * 3 - 소림사
 * 4 - 녹림맹
 * 5 - 미정
 * 6 - 미정
 * 7 - 미정
 * 8 - 미정
 * 9 - 미정
 * 10 - 어디도 속하지 않음. 무조건 CP2.
 * 11 - 개방, 소림사, 무당
 * 12 - 비궁, 녹림맹, 마교
 * 13 - 개방, 소림사, 무당, 마교
 * 20 - 모든 문파
*/

class _XWindow_Contribution : public _XWindow  
{
private:	
	_XImageStatic		m_ContributionBox;
public:
	_XWindow_Contribution();
	virtual ~_XWindow_Contribution();

	BOOL	Initailize(void);
	void	DestroyWindow(void);
	
	void	Draw(_XGUIObject*& pfocusedobject = g_CurrentFocusedObject);
	BOOL	Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);	

	void				MoveWindow( int X, int Y );
	void				ShowWindow(BOOL show);

// Draw item...
public:
	int					m_CurrentItemContribution;
	DWORD				m_SendDelayTime;
	
	int					m_ContributionType;
	int					m_ContributionSecondType;
	int					m_ContributionThirdType;
	int					m_ContributionId;
	int					m_ContributionModelIndex;
	int					m_ContributionGender;
	int					m_ContributionIvenIndex;
	int					m_ContributionItemCount;
	
	FLOAT				m_RotateAngle;
	D3DXMATRIX			m_RHWMatrix;
	_X3PCamera			m_ModelViewCamera;
	D3DVIEWPORT9		m_ModelViewPort;	

	void				DrawContributionItem( void );	///< 3디 모델 디스플레이
	void				ResetContributionData();	
	void				SetAddData();	///< 아이템 정보 저장
	/// 아이템 스키마중 cApplyClan 값에 따른 기여도 값(cp1, cp2)을 리턴
	int					CheckClanPoint(int type, int id);
	int					CalcContribution(int invennumber);	///< 기여도 계산
};

#endif // !defined(AFX_XWINDOW_CONTRIBUTION_H__CD341A79_76BC_4013_A3FE_F0D160C2F24E__INCLUDED_)
