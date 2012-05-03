// XPathObject.h: interface for the CPathObject class.
//
// Created Date	: 2003.03.07
// Author		: Sohyun, Park (kukuri)
// Desc			: Bezier Curve를 이용한 Path Object 
//
//////////////////////////////////////////////////////////////////////

#ifndef _KUPATHOBJECT_H_
#define _KUPATHOBJECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <D3DX9.h>

#define DEF_MAXNUM_CURVE							200
#define DEF_NUM_CONTROLPOINT						4

#define DEF_CUBIC_BEZIERCURVE						0
#define DEF_QUADRATIC_BEZIERCURVE					1

#define DEF_PATHOBJECT_SELECTADDMODE_ONE			0
#define DEF_PATHOBJECT_SELECTADDMODE_MERGE			1

#define DEF_PATHOBJECT_PATHPOINT					1050


typedef struct S_CURVE
{
	D3DXVECTOR3		vControlPoint[DEF_NUM_CONTROLPOINT];
	BOOL			bValidCurve;
	BOOL			bSelected[DEF_NUM_CONTROLPOINT];
	BOOL			bCurveSelect;
}S_CURVE;

class CPathObject  
{
protected :
	S_CURVE				m_Curve[DEF_MAXNUM_CURVE];
	int					m_iInsertIndex;
	int					m_iCurrentCurve;

	BOOL				m_bInsertMode;
	BOOL				m_bPointDrawing;

public :
	D3DXVECTOR3			m_vPathPoint[DEF_PATHOBJECT_PATHPOINT];

public:
	CPathObject();
	virtual ~CPathObject();

	void				Create(int curvecount = DEF_MAXNUM_CURVE);

	void				InsertControlPoint(D3DXVECTOR3 point, int index);

	void				AddInsertIndex(void);
	void				SubtractInsertIndex(void);
	int					GetInsertIndex(void)			{	return m_iInsertIndex;		}

	void				Draw(void);
	void				DrawRecursive(D3DXVECTOR3 controlpoint[], int level);

	void				SelectByRegion(D3DXVECTOR3 start, D3DXVECTOR3 end, int addmode);
	void				SelectPoint(BOOL selectmode);

	void				MoveSelectedPoint(FLOAT x, FLOAT y, FLOAT z);

	BOOL				DeletePoint(void);
	BOOL				DeleteCurve(void);

	BOOL				SavePathObject(FILE* fileptr);
	BOOL				LoadPathObject(FILE* fileptr);

	void				ResetPathData(void);

	void				SelectAll(BOOL selected);

	void				GeneratePathIterate(int selectedcurve, int steps);

	D3DXVECTOR3			GetControlPoint(int curveindex, int pointindex)
	{
		return m_Curve[curveindex].vControlPoint[pointindex];
	}

	void				SetValidCurve(int curveindex, BOOL validcurve)
	{
		m_Curve[curveindex].bValidCurve = validcurve;
	}

	BOOL				GetValidCurve(int curveindex)
	{
		return m_Curve[curveindex].bValidCurve;
	}

	void				SetCurveSelect(int curveindex, BOOL selected);

	BOOL				GetCurveSelect(int curveindex)
	{
		return m_Curve[curveindex].bCurveSelect;
	}

	int					GetCurrentCurve(void)			{	return m_iCurrentCurve;		}
	void				SetCurrentCurve(int curveindex)	{	m_iCurrentCurve = curveindex;	}

	BOOL				GetInsertMode(void)				{	return m_bInsertMode;		}
	void				SetInsertMode(BOOL mode)		{	m_bInsertMode = mode;		}

	void				SetCurveData( int curveindex, int curvedataindex, D3DXVECTOR3 point );
	
	int					MakeCurve(D3DXVECTOR3 point1, D3DXVECTOR3 point2, D3DXVECTOR3 point3, D3DXVECTOR3 point4, int step);
	void				ReleaseCurve(int curveindex);

};

#endif
