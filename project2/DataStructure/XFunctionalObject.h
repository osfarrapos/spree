// XFunctionalObject.h: interface for the _XFunctionalObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFUNCTIONALOBJECT_H__5DBA77A5_F034_4904_B449_346EF38485D9__INCLUDED_)
#define AFX_XFUNCTIONALOBJECT_H__5DBA77A5_F034_4904_B449_346EF38485D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XObjectManager.h"


class _XFunctionalObject : public _XObject, public _XItem
{
public :
	D3DXVECTOR3				m_Position;
		
	LPTSTR	m_ObjectName;
	int		m_UniqueID;
	int		m_ObjectType;			//타입정보 ( 몬스터 종류와 동일)
 	int		m_ObjectClass;			//역할 ( 0:문 1:보스전입구 2:TRAP_LPS 3:비무전광판 )
	int		m_ActionIndex;			//현재 액션의 종류 ( 몬스터와 동일) 대기, 공격, 막기 ...
	
	BOOL	m_bWideViewMode;
	BOOL	m_bReLocateMode;
	
	float	m_CameraDistance;		//카메라 거리 
	float	m_CameraTargetHeight;	//카메라 대상 높이 
	float	m_CameraPitch;			//카메라 상하 
	float	m_CameraYaw;			//카메라 좌우 
	
	LPTSTR	m_SystemMessage;		//시스템 메세지
	LPTSTR	m_Description;			//간략한 설명 
	
	_XList	m_listFunctionalObjectTrigger;
	
	BOOL    m_InsideFrustum;
	
	_XModelDescriptor	m_ModelDescriptor;
	
	int		m_AttackalbeTypeUniqueID;	// 공격가능한 상태 일때 서버에서 할당해 주는 아이디 
	char	m_HealthPercentage;			// 공격가능한 상태일때 체력
	char	m_DisplayHealthPercentage;
	BOOL	m_bAttackable;				// 공격가능한 상태인지 아닌지를 알 수 있는 변수
	
	int		m_AttackInfo;				//TRAP LPS일 경우에 공격정보를 가진다.
	int		m_AttackTargetObjectType;	// 0:none 1:pc 2:self 3:mob
	_XItem*					m_AttackTarget;
	FLOAT					m_AttackHitValue;
		
	unsigned char  m_DefenderType;
	unsigned short m_DefenderID;
	D3DXMATRIX	   m_matTemp;
	
	DWORD	m_DieStartTime;
	BOOL	m_bDie;

public:
	_XFunctionalObject();
	virtual ~_XFunctionalObject();
	
	BOOL	LoadScript( LPSTR Filename );
	BOOL	LoadScript( FILE* FilePtr );
	BOOL	Load( FILE* fileptr );
	
	void	ProcessAction(void);			// Functional Object의 조건식 행동식을 체크하고 실행하는 프로세스 
	void	ProcessAnimation(void);			// Functional Object의 Animation을 처리하는 프로세스 
	void	CreateEffectManager( void );
	void	SetAnimation( void );
	void	SetIdleAnimation(void);
	void	SetBlowMotion(void);
	void	SetDieAnimation(void);
	void	SetAnimationIndex( int animationindex );
	BOOL	SetAttackableFunctionalObject(void);	// Functional Object가 공격가능한지 아닌지를 결정하는 함수 
	
	void	ProcessMove();				// Fuctional object에서 player move 부분 처리

};


class _XFunctionalObjectManager
{
public:
	int						m_countFunctionalObject;
	_XFunctionalObject*		m_pFunctionalObjectArray;	
	_XOBBData*				m_pFunctionalOBBDataArray;
	
public:
	_XFunctionalObjectManager();
	virtual ~_XFunctionalObjectManager();
	
	void			disposeList( void  );	
	BOOL			LoadFunctionalObjectList( FILE* Fileptr, LPSTR Filename = NULL );

	void			CheckInsideFrustum( void );
	void			Render( void );
	void			Process( void );
	void			SetAttackableFunctionalObjectID(int objectid, int objecttype, int healthpercentage, D3DXVECTOR3& vObjectPosition );
	
	_XFunctionalObject*	FindAttackableFunctionalObject( unsigned short objectuniqueid	);
};

#endif // !defined(AFX_XFUNCTIONALOBJECT_H__5DBA77A5_F034_4904_B449_346EF38485D9__INCLUDED_)
