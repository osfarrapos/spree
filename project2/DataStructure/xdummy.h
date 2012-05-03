// xdummy.h: interface for the _XDummy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDUMMY_H__47FA2D30_BB87_4CE6_8570_CB2414C45D62__INCLUDED_)
#define AFX_XDUMMY_H__47FA2D30_BB87_4CE6_8570_CB2414C45D62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XUser.h"
#include "EODEXTDEF_ModelData.h"

class _XDummy : public _XItem
{
public :
	_XModelDescriptor		m_ModelDescriptor;

	_XGROUPTYPE				m_GroupType;
	unsigned short			m_DummyID;

	D3DXVECTOR3				m_Position;
	FLOAT					m_RotateAngle;

	BOOL					m_bEastDir;
	BOOL					m_bWestDir;
	BOOL					m_bSouthDir;
	BOOL					m_bNorthDir;

	BOOL					m_bInsideFrustum;
	BOOL					m_bImpact;
	
public:
	_XDummy();
	virtual ~_XDummy();

	void					Create(unsigned short id, _XGROUPTYPE group, D3DXVECTOR3 position);

	BOOL					CheckInsideFrustum(void);

	void					Render(void);
	void					Process(void);

	void					SetAnimationIndex(int animationindex);
	void					UpdatePosition(void);

};

class _XDummyList : public _XList
{

public :
	_XDummyList();
	~_XDummyList();
	
	BOOL					Add(_XDummy* dummy);
	_XDummy*				Find(unsigned short dummyid);
	void					Delete(unsigned short dummyid);
	void					Delete(_XDummy* dummy);

	void					Process(void);
	void					Render(void);

	void					CheckInsideFrustum(void);
	void					RemoveEffectInstance(int templateid);

};

#endif // !defined(AFX_XDUMMY_H__47FA2D30_BB87_4CE6_8570_CB2414C45D62__INCLUDED_)
