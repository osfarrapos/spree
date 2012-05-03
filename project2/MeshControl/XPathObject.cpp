// XPathObject.cpp: implementation of the CPathObject class.
//
// Created Date	: 2003.03.07
// Author		: Sohyun, Park (kukuri)
// Desc			: Bezier Curve를 이용한 Path Object
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XGUIControlCenter.h"
#include "XPathObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPathObject::CPathObject()
{

}

CPathObject::~CPathObject()
{

}

void CPathObject::Create(int curvecount)
{
	for(int i = 0 ; i < DEF_MAXNUM_CURVE ; ++i)
	{
		for(int j = 0 ; j < DEF_NUM_CONTROLPOINT ; ++j)
		{
			m_Curve[i].vControlPoint[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_Curve[i].bSelected[j] = FALSE;
		}
		m_Curve[i].bValidCurve = FALSE;
		m_Curve[i].bCurveSelect = FALSE;
	}
	m_iInsertIndex = 0;
	m_iCurrentCurve = 0;
	m_bInsertMode = FALSE;
	m_bPointDrawing = TRUE;

	for(i = 0 ; i < DEF_PATHOBJECT_PATHPOINT ; ++i)
	{
		m_vPathPoint[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

void CPathObject::InsertControlPoint(D3DXVECTOR3 point, int index)
{
	int curveindex = index / DEF_NUM_CONTROLPOINT;
	int pointindex = index % DEF_NUM_CONTROLPOINT;

	m_Curve[curveindex].vControlPoint[pointindex] = point;
	AddInsertIndex();	
}

void CPathObject::AddInsertIndex(void)
{
	m_iInsertIndex++;
	SetCurrentCurve(m_iInsertIndex/DEF_NUM_CONTROLPOINT);
}

void CPathObject::SubtractInsertIndex(void)
{
	m_iInsertIndex--;
	SetCurrentCurve(m_iInsertIndex/DEF_NUM_CONTROLPOINT);
}

void CPathObject::Draw(void)
{
//	gpDev->SetTexture(0, NULL);
//	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
//	gpDev->SetFVF(D3DFVF_BOXVERTEX);	
//	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// Control Point Drawing ---------------------------------
/*	if(m_bPointDrawing)
	{
		D3DXVECTOR3		point;
		FLOAT			rectsize = 0.f, dist = 0.f;
		D3DCOLOR		color;
		for(int i = 0 ; i <= m_iCurrentCurve ; i++)
		{
			if(m_Curve[i].bValidCurve)
			{
				for(int j = 0 ; j < DEF_NUM_CONTROLPOINT ; j++)
				{
					point = m_Curve[i].vControlPoint[j];
					rectsize = 0.1f;
					dist = (FLOAT)fabs(_XFC_RSqrt((point.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x)*
												  (point.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x) +
												  (point.y - g_LodTerrain.m_3PCamera.m_CameraPosition.y)*
												  (point.y - g_LodTerrain.m_3PCamera.m_CameraPosition.y) +
												  (point.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z)*
												  (point.z - g_LodTerrain.m_3PCamera.m_CameraPosition)));
					dist *= 10.0f;
					dist = (float)((int)dist);
					if(dist == 0.0f)	dist = 1.0f;
					rectsize /= dist;

					if(m_Curve[i].bSelected[j])
						color = D3DCOLOR_ARGB(255, 255, 0, 0);
					else
						color = D3DCOLOR_ARGB(255, 255, 255, 255);

					_X3D->DrawRectAngle(point.x - rectsize, point.z - rectsize, point.x + rectsize, point.z + rectsize, point.y, color);
				}
			}
			else
			{
				int pointindex = m_iInsertIndex % DEF_NUM_CONTROLPOINT;
				for(int j = 0 ; j < pointindex; j++)
				{
					point = m_Curve[i].vControlPoint[j];
					rectsize = 0.11f;
					dist = (FLOAT)fabs(_XFC_RSqrt((point.x - _X3D->m_vPosition.x)*(point.x - _X3D->m_vPosition.x) +
						(point.y - _X3D->m_vPosition.y)*(point.y - _X3D->m_vPosition.y) +
						(point.z - _X3D->m_vPosition.z)*(point.z - _X3D->m_vPosition.z)));
					dist *= 10.0f;
					dist = (float)((int)dist);
					if(dist == 0.0f)	dist = 1.0f;
					rectsize /= dist;
					
					color = D3DCOLOR_ARGB(255, 0, 0, 255);
					
					_X3D->DrawRectAngle(point.x - rectsize, point.z - rectsize, point.x + rectsize, point.z + rectsize, point.y, color);
				}
			}
		}
	}
*/	

	// Curve Drawing --------------------------------------------------
	// 초기 control point generate 
	// level 결정
	for(int i = 0 ; i < m_iCurrentCurve ; ++i)
	{
		if(m_Curve[i].bValidCurve)
			DrawRecursive(m_Curve[i].vControlPoint, 5);
	}
	//-----------------------------------------------------------------

//	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	
//	gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CPathObject::DrawRecursive(D3DXVECTOR3 controlpoint[], int level)
{
	D3DXVECTOR3			PointLeft[DEF_NUM_CONTROLPOINT];
	D3DXVECTOR3			PointRight[DEF_NUM_CONTROLPOINT];

	if(level == 0)
	{
		_XDrawLine(controlpoint[0], controlpoint[3], D3DCOLOR_ARGB(255, 255, 255, 255));
		return;
	}

	// Subdivide
	PointLeft[0] = controlpoint[0];
	PointRight[3] = controlpoint[3];
	PointLeft[1] = 0.5 * (controlpoint[0] + controlpoint[1]);
	PointRight[2] = 0.5 * (controlpoint[2] + controlpoint[3]);
	PointLeft[2] = 0.5 * (PointLeft[1] + 0.5 * (controlpoint[1] + controlpoint[2]));
	PointRight[1] = 0.5 * (PointRight[2] + 0.5 * (controlpoint[1] + controlpoint[2]));
	PointLeft[3] = PointRight[0] = 0.5 * (PointLeft[2] + PointRight[1]);

	// call recursively for left half
	DrawRecursive(PointLeft, --level);

	// call recursively for right half
	DrawRecursive(PointRight, level);

	return;
}

void CPathObject::SelectByRegion(D3DXVECTOR3 start, D3DXVECTOR3 end, int addmode)
{
	for(int i = 0 ; i < m_iCurrentCurve ; ++i)
	{
		if(m_Curve[i].bValidCurve)
		{
			for(int j = 0 ; j < DEF_NUM_CONTROLPOINT ; ++j)
			{
				if(m_Curve[i].vControlPoint[j].x >= start.x  &&
					m_Curve[i].vControlPoint[j].z >= start.z &&
					m_Curve[i].vControlPoint[j].x <= end.x &&
					m_Curve[i].vControlPoint[j].z <= end.z  )
				{
					// Drag 안에 존재
					if(addmode == DEF_PATHOBJECT_SELECTADDMODE_ONE)
						m_Curve[i].bSelected[j] = TRUE;
					else  // if(addmode == DEF_PathOBJECT_ADDMODE_MERGE)
						m_Curve[i].bSelected[j] = !m_Curve[i].bSelected[j];
				}
				else
				{
					// Drag안에 존재하지 않음
					if(addmode == DEF_PATHOBJECT_SELECTADDMODE_ONE)
						m_Curve[i].bSelected[j] = FALSE;
				}
			}
			BOOL bAllSelected = TRUE;
			for(j = 0 ; j < DEF_NUM_CONTROLPOINT ; ++j)
			{
				if(!m_Curve[i].bSelected[j])
					bAllSelected &= FALSE;
			}
			if(bAllSelected)
				m_Curve[i].bCurveSelect = TRUE;
			else
				m_Curve[i].bCurveSelect = FALSE;
		}
	}
}

void CPathObject::SelectPoint(BOOL selectmode)
{
	for(int i = 0 ; i < m_iCurrentCurve ; ++i)
	{
		if(m_Curve[i].bValidCurve)
		{
			for(int j = 0 ; j < DEF_NUM_CONTROLPOINT ; ++j)
			{
				m_Curve[i].bSelected[j] = selectmode;
			}
		}
	}
}

void CPathObject::MoveSelectedPoint(FLOAT x, FLOAT y, FLOAT z)
{
	for(int i = 0 ; i < m_iCurrentCurve ; ++i)
	{
		if(m_Curve[i].bValidCurve)
		{
			for(int j = 0 ; j < DEF_NUM_CONTROLPOINT ; ++j)
			{
				if(m_Curve[i].bSelected[j])
				{
					m_Curve[i].vControlPoint[j].x += x;
					m_Curve[i].vControlPoint[j].y += y;
					m_Curve[i].vControlPoint[j].z += z;
				}
			}
		}
	}
}

BOOL CPathObject::DeletePoint(void)
{
	int curveindex = m_iInsertIndex / DEF_NUM_CONTROLPOINT;
	int pointindex = m_iInsertIndex % DEF_NUM_CONTROLPOINT;

	if(pointindex == 0)
	{
		if(curveindex == 0)
			return FALSE;

		curveindex--;
		pointindex = DEF_NUM_CONTROLPOINT - 1;

		if(!m_Curve[curveindex].bValidCurve)
		{
			m_Curve[curveindex].vControlPoint[pointindex].x = 0.f;
			m_Curve[curveindex].vControlPoint[pointindex].y = 0.f;
			m_Curve[curveindex].vControlPoint[pointindex].z = 0.f;

			SubtractInsertIndex();
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		pointindex--;
		if(!m_Curve[curveindex].bValidCurve)
		{
			m_Curve[curveindex].vControlPoint[pointindex].x = 0.f;
			m_Curve[curveindex].vControlPoint[pointindex].y = 0.f;
			m_Curve[curveindex].vControlPoint[pointindex].y = 0.f;

			SubtractInsertIndex();
			return TRUE;
		}
		else
			return FALSE;
	}
	return FALSE;
}

BOOL CPathObject::DeleteCurve(void)
{
	int			i = 0, deletecurveindex = -1;
	BOOL		bAllSelected, bDeleted = FALSE;

	do
	{
		bAllSelected = TRUE;
		if(m_Curve[i].bValidCurve)
		{
			// control point가 모두 선택 되어 있는지 확인한다.
			for(int j = 0 ; j < DEF_NUM_CONTROLPOINT ; ++j)
			{
				if(m_Curve[i].bSelected[j])
				{
					deletecurveindex = i;
					bAllSelected &= TRUE;
				}
				else
				{
					deletecurveindex = -1;
					bAllSelected &= FALSE;
				}
			}
			if(bAllSelected)	// 모두 선택되어있으면,
			{
				if(deletecurveindex < m_iCurrentCurve - 1)	// 뒤에 커브가 더 있음
				{
					// 뒤의 커브를 앞으로 옮긴다.
					for(int n = deletecurveindex ; n < m_iCurrentCurve-1 ; ++n)
					{
						if(m_Curve[n+1].bValidCurve)
						{
							for(int m = 0 ; m < DEF_NUM_CONTROLPOINT ; ++m)
							{
								m_Curve[n].vControlPoint[m] = m_Curve[n+1].vControlPoint[m];
								m_Curve[n].bSelected[m] =  m_Curve[n+1].bSelected[m];
							}
							m_Curve[n].bValidCurve = m_Curve[n+1].bValidCurve;
						}
					}
					// 맨끝의 커브는 삭제
					for(int m = 0 ; m < DEF_NUM_CONTROLPOINT ; ++m)
					{
						m_Curve[m_iCurrentCurve-1].vControlPoint[m] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						m_Curve[m_iCurrentCurve-1].bSelected[m] =  FALSE;
					}
					m_Curve[m_iCurrentCurve-1].bValidCurve = FALSE;
				}
				else	// deletecurveindex == m_iCurrentCurve - 1 (맨마지막)
				{
					// 그냥 삭제
					for(int m = 0 ; m < DEF_NUM_CONTROLPOINT ; ++m)
					{
						m_Curve[deletecurveindex].vControlPoint[m] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						m_Curve[deletecurveindex].bSelected[m] =  FALSE;
					}
					m_Curve[deletecurveindex].bValidCurve = FALSE;
				}
				m_iInsertIndex = m_iInsertIndex - 4;  // InsertIndex 감소
				bDeleted = TRUE;
				i = 0;	// 커브가 옮겨졌을수도 있으므로 다시 0부터 검사한다.
				m_iCurrentCurve--; // 전체 커브 갯수 줄인다.
			}
			else
				i++;	// control point가 모두 선택 되지 않았으므로 다음 커브를 검사
		}
	}while(i < m_iCurrentCurve);	// 모든 커브가 선택된 경우가 아닐때 까지 실행

	return bDeleted;
}

void CPathObject::SetCurveSelect(int curveindex, BOOL selected)
{
	m_Curve[curveindex].bCurveSelect = selected;
	for(int i = 0 ; i < DEF_NUM_CONTROLPOINT ; ++i)
		m_Curve[curveindex].bSelected[i] = selected;
}

BOOL CPathObject::SavePathObject(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;

	fwrite(&m_iInsertIndex, sizeof(int), 1, fileptr);
	fwrite(&m_iCurrentCurve, sizeof(int), 1, fileptr);

	for(int i = 0 ; i < m_iCurrentCurve ; ++i)
	{
		if(fwrite(&m_Curve[i], sizeof(S_CURVE), 1, fileptr) < 1 )
			return FALSE;
	}
	return TRUE;
}

BOOL CPathObject::LoadPathObject(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;

	ResetPathData();

	fread(&m_iInsertIndex, sizeof(int), 1, fileptr);
	fread(&m_iCurrentCurve, sizeof(int), 1, fileptr);

	for(int i = 0 ; i < m_iCurrentCurve ; ++i)
	{
		if(fread(&m_Curve[i], sizeof(S_CURVE), 1, fileptr) < 1)
			return FALSE;
	}	

	return TRUE;
}

void CPathObject::SetCurveData( int curveindex, int curvedataindex, D3DXVECTOR3 point )
{
	if( curveindex > m_iCurrentCurve ) return;
	
	m_Curve[curveindex].vControlPoint[curvedataindex] = point;
}

void CPathObject::ResetPathData(void)
{
	for(int i = 0 ; i < DEF_MAXNUM_CURVE ; ++i)
	{
		for(int j = 0 ; j < DEF_NUM_CONTROLPOINT ; ++j)
		{
			m_Curve[i].vControlPoint[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_Curve[i].bSelected[j] = FALSE;
		}
		m_Curve[i].bValidCurve = FALSE;
		m_Curve[i].bCurveSelect = FALSE;
	}
	m_iInsertIndex = 0;
	m_iCurrentCurve = 0;
	m_bInsertMode = FALSE;
	m_bPointDrawing = TRUE;
}

void CPathObject::SelectAll(BOOL selected)
{
	for(int i = 0 ; i < m_iCurrentCurve ; ++i)
	{
		SetCurveSelect(i, selected);
	}
}

void CPathObject::GeneratePathIterate(int selectedcurve, int steps)
{
	float stepsize = 1.0 / steps;
	float t = stepsize;

	m_vPathPoint[0] = m_Curve[selectedcurve].vControlPoint[0];
	
	for(int i = 1 ; i < steps ; i++, t+=stepsize)
	{
		float b0 = (1-t)*(1-t)*(1-t);
		float b1 = 3*t*(1-t)*(1-t);
		float b2 = 3*t*t*(1-t);
		float b3 = t*t*t;

		float x = b0*m_Curve[selectedcurve].vControlPoint[0].x + b1*m_Curve[selectedcurve].vControlPoint[1].x
					+ b2*m_Curve[selectedcurve].vControlPoint[2].x + b3*m_Curve[selectedcurve].vControlPoint[3].x;
		float y = b0*m_Curve[selectedcurve].vControlPoint[0].y + b1*m_Curve[selectedcurve].vControlPoint[1].y
					+ b2*m_Curve[selectedcurve].vControlPoint[2].y + b3*m_Curve[selectedcurve].vControlPoint[3].y;
		float z = b0*m_Curve[selectedcurve].vControlPoint[0].z + b1*m_Curve[selectedcurve].vControlPoint[1].z
					+ b2*m_Curve[selectedcurve].vControlPoint[2].z + b3*m_Curve[selectedcurve].vControlPoint[3].z;

		m_vPathPoint[i].x = x;
		m_vPathPoint[i].y = y;
		m_vPathPoint[i].z = z;
	}
	m_vPathPoint[i] = m_Curve[selectedcurve].vControlPoint[3];
}

int CPathObject::MakeCurve(D3DXVECTOR3 point1, D3DXVECTOR3 point2, D3DXVECTOR3 point3, D3DXVECTOR3 point4, int step)
{
	SetCurveData(m_iCurrentCurve, 0, point1);
	SetCurveData(m_iCurrentCurve, 1, point2);
	SetCurveData(m_iCurrentCurve, 2, point3);
	SetCurveData(m_iCurrentCurve, 3, point4);

	GeneratePathIterate(m_iCurrentCurve, step);
	m_Curve[m_iCurrentCurve].bValidCurve = TRUE;

	int currentindex = m_iCurrentCurve;

	for(int i = 0 ; i < DEF_MAXNUM_CURVE ; ++i)
	{
		if(!m_Curve[i].bValidCurve)
			m_iCurrentCurve = i;
	}

	return currentindex;
}

void CPathObject::ReleaseCurve(int curveindex)
{
	m_Curve[curveindex].bValidCurve = FALSE;
	memset(m_vPathPoint, 0, sizeof(D3DXVECTOR3)*DEF_PATHOBJECT_PATHPOINT);
}