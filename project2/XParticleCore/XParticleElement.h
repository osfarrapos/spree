// XParticleElement.h: interface for the _XParticleElement class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XPARTICLEELEMENT_H_
#define _XPARTICLEELEMENT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XParticleDefine.h"

class _XEffect_LineEffect_InstanceData;

struct _XPARTICLEVERTEX
{
	_XPARTICLEVERTEX() {}
	_XPARTICLEVERTEX( D3DXVECTOR3 v, DWORD c ) 
	{
		pos = v;
		col = c;
		tu   = 0.0f;
		tv   = 0.0f;
	}
	D3DXVECTOR3 pos;	
	DWORD       col;
	FLOAT		tu;
	FLOAT		tv;
};

#define _XDEF_FVF_XPARTICLEVERTEX	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

class _XParticleElement
{
public:
	D3DXVECTOR3 position;
	D3DXVECTOR3 positionB;
	D3DXVECTOR3 size;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 velocityB;
	D3DXCOLOR	color;	
	FLOAT		age;
	FLOAT		randomdirection;

	_XEffect_LineEffect_InstanceData* pLineData;

public:
	_XParticleElement();
	virtual ~_XParticleElement();
	
};


//////////////////////////////////////////////////////////////////////////
// line particle

typedef struct _XLINEEFFECT_VERTEX
{
	float x,y,z;
	DWORD diffuse;
	float u,v;
}_XLINEEFFECT_VERTEX;

#define D3DFVF_XLINEEFFECT_VERTEX  (D3DFVF_XYZ |D3DFVF_DIFFUSE| D3DFVF_TEX1)

#define _XDEF_XLINEEFFECT_MAXTRACECOUNT					100	
#define _XDEF_XLINEEFFECT_TOTAL_CURVEPOINTS				4

extern int g_PointStep;

class _XEffect_LineEffect_InstanceData
{
public:
	D3DXVECTOR3			ControlPoint[2][_XDEF_XLINEEFFECT_TOTAL_CURVEPOINTS];
	int					insertcount;
		
	_XEffect_LineEffect_InstanceData()
	{
		ResetControlPoint();
	}
	
	void ResetControlPoint( void )
	{
		insertcount = 0;
		memset( ControlPoint, 0, sizeof(D3DXVECTOR3)*_XDEF_XLINEEFFECT_TOTAL_CURVEPOINTS*2 );
	}

	void InsertLastPoint( D3DXVECTOR3& lasttoppos, D3DXVECTOR3& lastbottompos );
	void UpdateLastPoint( D3DXVECTOR3& lasttoppos, D3DXVECTOR3& lastbottompos );
};

class _XEffect_LineEffect
{
public:
	float				m_AllSwordStart[_XDEF_XLINEEFFECT_MAXTRACECOUNT][3];
	float				m_AllSwordEnd[_XDEF_XLINEEFFECT_MAXTRACECOUNT][3];
	
	DWORD				m_PointStep;		//각포인트간의  스텝수 이값이 클수록 곡선이 부드러워진다.
	DWORD				m_TotalTraceCnt;
	
	_XLINEEFFECT_VERTEX m_TraceVBuf[_XDEF_XLINEEFFECT_MAXTRACECOUNT<<1];	//생산되는 궤적점들..위아래 2개다...
	_XLINEEFFECT_VERTEX m_TotalPoint[_XDEF_XLINEEFFECT_TOTAL_CURVEPOINTS<<1];	//실제 궤적 점들 
	
public:
	_XEffect_LineEffect();
	virtual ~_XEffect_LineEffect();
	
	static void CalcNaturalCubic(float get_abcd[][4],int n, float *vptr);
	
	void ResetControlPoint( void );
	
	void SetPointStep( DWORD pointstep )
	{
		if( m_PointStep > 20 ) m_PointStep = 20;
		else m_PointStep = pointstep;
	}
	
	void SetDiffuseColor( D3DCOLOR diffusecolor )
	{
		for( int i=0; i<(_XDEF_XLINEEFFECT_MAXTRACECOUNT<<1); i++) m_TraceVBuf[i].diffuse = diffusecolor;
	}
	
	void Process( _XEffect_LineEffect_InstanceData* pInstanceData );
	void Render( void );
};


#endif // !defined(_XPARTICLEELEMENT_H_)

