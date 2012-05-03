// XSplineContainer.h: interface for the _XSplineContainer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XSPLINECONTAINER_H_
#define _XSPLINECONTAINER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XList.h"

#define _XSL_MAX_CONTROL_PNT 100
#define _XSL_MAX_CURVE_PNT 1000

#define _XSL_B_SPLINE(u, u_2, u_3, cntrl0, cntrl1, cntrl2, cntrl3) \
	( \
	( \
	(-1.0f*u_3 + 3.0f*u_2 - 3.0f*u + 1.0f) * (cntrl0) + \
	( 3.0f*u_3 - 6.0f*u_2 + 0.0f*u + 4.0f) * (cntrl1) + \
	(-3.0f*u_3 + 3.0f*u_2 + 3.0f*u + 1.0f) * (cntrl2) + \
	( 1.0f*u_3 + 0.0f*u_2 + 0.0f*u + 0.0f) * (cntrl3)   \
	) / 6.0f \
) 

#define _XSL_B_TANGENT(u, u_2, cntrl0, cntrl1, cntrl2, cntrl3) \
	( \
	( \
	(-1.0f*u_2 + 2.0f*u - 1.0f) * (cntrl0) + \
	( 3.0f*u_2 - 4.0f*u + 0.0f) * (cntrl1) + \
	(-3.0f*u_2 + 2.0f*u + 1.0f) * (cntrl2) + \
	( 1.0f*u_2 + 0.0f*u + 0.0f) * (cntrl3)   \
	) / 2.0f \
) 

#define _XSL_CATMULL_ROM_SPLINE(u, u_2, u_3, cntrl0, cntrl1, cntrl2, cntrl3) \
	( \
	( \
	(-1.0f*u_3 + 2.0f*u_2 - 1.0f*u + 0.0f) * (cntrl0) + \
	( 3.0f*u_3 - 5.0f*u_2 + 0.0f*u + 2.0f) * (cntrl1) + \
	(-3.0f*u_3 + 4.0f*u_2 + 1.0f*u + 0.0f) * (cntrl2) + \
	( 1.0f*u_3 - 1.0f*u_2 + 0.0f*u + 0.0f) * (cntrl3)   \
	) / 2.0f \
) 

#define _XSL_CATMULL_ROM_TANGENT(u, u_2, cntrl0, cntrl1, cntrl2, cntrl3) \
	( \
	( \
	(-3.0f*u_2 +  4.0f*u - 1.0f) * (cntrl0) + \
	( 9.0f*u_2 - 10.0f*u + 0.0f) * (cntrl1) + \
	(-9.0f*u_2 +  8.0f*u + 1.0f) * (cntrl2) + \
	( 3.0f*u_2 -  2.0f*u + 0.0f) * (cntrl3)   \
	) / 2.0f \
) 

typedef struct _XSplineCurveDataStructure
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 tan;
	D3DXVECTOR3 nrm;
	D3DXVECTOR3 sid;
	float distance;
} _XSplineCurveData;

class _XSplineContainer : public _XItem
{

public:

	LPTSTR				 m_pSplineName;

	int					 m_CurveType;
	FLOAT				 m_ShortestDistance;
	D3DXVECTOR3			 m_DefualtNormal;
	
	D3DXVECTOR3*		 m_pControlData;	
	int					 m_ControlCount;
	int					 m_ControlIndx;
	
	_XSplineCurveData*	 m_pCurveData;
	int					 m_CurveSubDivision;
	int					 m_TotalCurveCount;	
		
public:
	_XSplineContainer();
	virtual ~_XSplineContainer();

	BOOL Load( FILE* pFilePtr );
	BOOL Save( FILE* pFilePtr );

	void SetPathName( LPCTSTR _splinename );	
	BOOL ReserveControlData( int controlcount, int curvesubdivision );
	void BuildControlPoint( int type, D3DXVECTOR3 forwardfactor, int rotatecount, FLOAT radius );
	void SetControlData( int bufferindex, const D3DXVECTOR3& pos );
	void Build( void );
	void Build(D3DXVECTOR3 defualtnormal);
	void ReBuildCurve( void );	
};

#endif // !defined(_XSPLINECONTAINER_H_)
