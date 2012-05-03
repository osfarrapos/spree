// xseat.h: interface for the _XSeat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSEAT_H__BAE4D067_F31C_4AFD_BD2D_DEB87FB057B2__INCLUDED_)
#define AFX_XSEAT_H__BAE4D067_F31C_4AFD_BD2D_DEB87FB057B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XUser.h"
#include "EODEXTDEF_ModelData.h"

class _XSeat : public _XItem
{
public :
	_XAnimMeshAbstractClass*	m_pMeshContainer;
	
	_XGROUPTYPE				m_GroupType;
	unsigned short			m_SeatID;	
	D3DXVECTOR3				m_Position;
	
	BOOL					m_bInsideFrustum;
	
	FLOAT					m_CameraDistance;
	
public :
	_XSeat();
	~_XSeat();
	
	void					Create(unsigned short id, _XGROUPTYPE group, D3DXVECTOR3 position, _XAnimMeshAbstractClass* pMeshContainer);
	void					Render(void);
};

class _XSeatList : public _XList
{
public :
	_XSeatList();
	~_XSeatList();
	
	BOOL					Add(_XSeat* seat);
	_XSeat*					Find(unsigned short id);
	void					Delete(unsigned short id);
	void					Delete(_XSeat* seat);
	
	void					Render(void);
	
	void					CheckInsideFrustum(void);
};

#endif // !defined(AFX_XSEAT_H__BAE4D067_F31C_4AFD_BD2D_DEB87FB057B2__INCLUDED_)
