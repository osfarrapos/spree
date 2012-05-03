// XEffect_SwordTrace.h: interface for the _XEffect_SwordTrace class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XEFFECT_SWORDTRACE_H_
#define _XEFFECT_SWORDTRACE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef struct _XSWORDTRACE_VERTEX
{
	float x,y,z;
	DWORD diffuse;
	float u,v;
}_XSWORDTRACE_VERTEX;

#define D3DFVF_XSWORDTRACE_VERTEX  (D3DFVF_XYZ |D3DFVF_DIFFUSE| D3DFVF_TEX1)

#define _XDEF_MAXTRACECOUNT					100	
#define _XDEF_TOTAL_CURVEPOINTS				4
#define _XDEF_MAXTRACETEXTURECOUNT			5

extern int g_PointStep;

class _XEffect_SwordTrace_InstanceData
{
public:
	D3DXVECTOR3			ControlPoint[2][_XDEF_TOTAL_CURVEPOINTS];
	int					TextureIndex;
	
	_XEffect_SwordTrace_InstanceData()
	{
		TextureIndex = 0;
	}
	
	void ResetControlPoint( void )
	{
		memset( ControlPoint, 0, sizeof(D3DXVECTOR3)*_XDEF_TOTAL_CURVEPOINTS*2 );
	}
};

class _XEffect_SwordTrace  
{
public:
	float				m_AllSwordStart[_XDEF_MAXTRACECOUNT][3];
	float				m_AllSwordEnd[_XDEF_MAXTRACECOUNT][3];

	//DWORD				m_PointStep;		//각포인트간의  스텝수 이값이 클수록 곡선이 부드러워진다.
	DWORD				m_TotalTraceCnt;

	_XSWORDTRACE_VERTEX m_TraceVBuf[_XDEF_MAXTRACECOUNT<<1];	//생산되는 궤적점들..위아래 2개다...
	_XSWORDTRACE_VERTEX m_TotalPoint[_XDEF_TOTAL_CURVEPOINTS<<1];	//실제 궤적 점들 

	_XTextureManager*   m_pTextureManager;	
	int					m_SelectedTextureIndex;
	int					TextureArchiveIndex[_XDEF_MAXTRACETEXTURECOUNT];
	
public:
	_XEffect_SwordTrace();
	virtual ~_XEffect_SwordTrace();

	void InitEffect( _XTextureManager* pTextureManager );
	void ChangeTextureIndex( int index );

	static void CalcNaturalCubic(float get_abcd[][4],int n, float *vptr);

	void ResetControlPoint( void );
	
	static void SetPointStep( DWORD pointstep )
	{
		if( g_PointStep > 20 ) g_PointStep = 20;
		else g_PointStep = pointstep;
	}

	void SetDiffuseColor( D3DCOLOR diffusecolor )
	{
		for( int i=0; i<(_XDEF_MAXTRACECOUNT<<1); i++) m_TraceVBuf[i].diffuse = 0xFFFFFFFF;
	}

	void Process( _XEffect_SwordTrace_InstanceData* pInstanceData );
	void Render( int renderstyleindex );
};


#endif // !defined(_XEFFECT_SWORDTRACE_H_)
