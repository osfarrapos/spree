// XHeroLocalUser.h: interface for the _XHeroLocalUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHEROLOCALUSER_H__DCE3FB36_C1C4_4A5E_A119_3140FA9536B0__INCLUDED_)
#define AFX_XHEROLOCALUSER_H__DCE3FB36_C1C4_4A5E_A119_3140FA9536B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XLocalUser.h"

/**
 * \brief ZeroLevel 캐릭 관련
 * zerolevel 상에서는 이 클래스를 사용
*/

class _XHeroLocalUser : public _XLocalUser  
{
	
	float		fLifeUpDelayTime;
public:
	_XHeroLocalUser();
	virtual ~_XHeroLocalUser();

public:
	BOOL		m_bSpecialFlag;	
	void		SetAnimation( _XMOTIONCLASS_TYPE motionclass, bool changemotion = true );
	void		SetAnimationIndex( int animationindex, bool changemotion = true );
	BOOL		CheckDistanceToTarget(D3DXVECTOR3 targetposition, FLOAT& range, unsigned short mobType);
	void		SetSkillAnimation(void);
	void		UpdateLocalUserAttackLevel( void );		
	short		GetStanceMotion(short activeslotid);
	short		GetMotionIndex(short selectedskillid);	
	
	void		Render( void );

	void		ProcessAnimation(void);	
	void		ProcessBattleChargeSkill(void);
	void		ProcessBattleActiveSkill(void);
	void		ProcessMobDie(void);
};

#endif // !defined(AFX_XHEROLOCALUSER_H__DCE3FB36_C1C4_4A5E_A119_3140FA9536B0__INCLUDED_)
