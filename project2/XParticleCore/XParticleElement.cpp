// XParticleElement.cpp: implementation of the _XParticleElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XParticleElement.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern DWORD FtoDW( FLOAT f );

inline float Eval(float v[4],float u)
{
	return ((((v[3]*u) + v[2])*u + v[1])*u + v[0]);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
_XParticleElement::_XParticleElement()
{
	pLineData = NULL;
}

_XParticleElement::~_XParticleElement()
{
	SAFE_DELETE(pLineData);
}


//////////////////////////////////////////////////////////////////////
// Line effect
//////////////////////////////////////////////////////////////////////

void _XEffect_LineEffect_InstanceData::InsertLastPoint( D3DXVECTOR3& lasttoppos, D3DXVECTOR3& lastbottompos )
{
	if( insertcount == 0 )
	{
		ControlPoint[0][0] = lasttoppos;
		ControlPoint[1][0] = lastbottompos;
		
		ControlPoint[0][1] = lasttoppos;
		ControlPoint[1][1] = lastbottompos;
		
		ControlPoint[0][2] = lasttoppos;
		ControlPoint[1][2] = lastbottompos;

		//ControlPoint[0][3] = lasttoppos;
		//ControlPoint[1][3] = lastbottompos;
		
		insertcount=_XDEF_XLINEEFFECT_TOTAL_CURVEPOINTS;
	}
	else
	{
		ControlPoint[0][0] = ControlPoint[0][1];
		ControlPoint[1][0] = ControlPoint[1][1];

		ControlPoint[0][1] = ControlPoint[0][2];
		ControlPoint[1][1] = ControlPoint[1][2];

		ControlPoint[0][2] = ControlPoint[0][3];
		ControlPoint[1][2] = ControlPoint[1][3];
	}

	ControlPoint[0][3] = lasttoppos;
	ControlPoint[1][3] = lastbottompos;
}

void _XEffect_LineEffect_InstanceData::UpdateLastPoint( D3DXVECTOR3& lasttoppos, D3DXVECTOR3& lastbottompos )
{
	if( insertcount != _XDEF_XLINEEFFECT_TOTAL_CURVEPOINTS )
	{
		ControlPoint[0][0] = ControlPoint[0][1];
		ControlPoint[1][0] = ControlPoint[1][1];

		ControlPoint[0][1] = ControlPoint[0][2];
		ControlPoint[1][1] = ControlPoint[1][2];

		ControlPoint[0][2] = ControlPoint[0][3];
		ControlPoint[1][2] = ControlPoint[1][3];

		insertcount=_XDEF_XLINEEFFECT_TOTAL_CURVEPOINTS;
	}

	ControlPoint[0][3] = lasttoppos;
	ControlPoint[1][3] = lastbottompos;
}

//큐빅커브함수.
void _XEffect_LineEffect::CalcNaturalCubic(float get_abcd[][4],int n, float *vptr)
{
	float gamma[_XDEF_XLINEEFFECT_TOTAL_CURVEPOINTS];
	float delta[_XDEF_XLINEEFFECT_TOTAL_CURVEPOINTS];
	float D[_XDEF_XLINEEFFECT_TOTAL_CURVEPOINTS];
	int i;
	typedef struct{
		float r;
		float no_use[2];
	}_X;
	_X *x=(_X *)vptr;
	
	gamma[0] = 0.5f;//1.0f/2.0f;
	for(i=1; i<n; i++)
	{
		gamma[i] = 1.0f/(4.0f-gamma[i-1]);
	}
	gamma[n] = 1.0f/(2.0f-gamma[n-1]);
	
	delta[0] = 3.0f*(x[1].r-x[0].r)*gamma[0];
	for(i=1; i<n; i++)
	{
		delta[i] = (3.0f*(x[i+1].r-x[i-1].r)-delta[i-1])*gamma[i];
	}
	delta[n] = (3.0f*(x[n].r-x[n-1].r)-delta[n-1])*gamma[n];
	
	D[n] = delta[n];
	for(i=n-1; i>=0; i--)
	{
		D[i] = delta[i] - gamma[i]*D[i+1];
	}
	
	for(i=0; i<n; i++)
	{
		get_abcd[i][0] = (float)x[i].r;
		get_abcd[i][1] = (float)D[i];
		get_abcd[i][2] = (float)3*(x[i+1].r - x[i].r) - 2*D[i] - D[i+1];
		get_abcd[i][3] = (2.0f*(x[i].r - x[i+1].r) + D[i] + D[i+1]);
	}
}

_XEffect_LineEffect::_XEffect_LineEffect()
{	
	m_TotalTraceCnt		= 4;
	
	for( int i=0; i<(_XDEF_XLINEEFFECT_MAXTRACECOUNT<<1); i++)
	{
		m_TraceVBuf[i].diffuse = 0xFFFFFFFF;		
	}

	m_PointStep			= 16;
}

_XEffect_LineEffect::~_XEffect_LineEffect()
{

}

extern float g_swordtrace_xref[2][_XDEF_XLINEEFFECT_MAXTRACECOUNT][4];
extern float g_swordtrace_yref[2][_XDEF_XLINEEFFECT_MAXTRACECOUNT][4];
extern float g_swordtrace_zref[2][_XDEF_XLINEEFFECT_MAXTRACECOUNT][4];

void _XEffect_LineEffect::Process( _XEffect_LineEffect_InstanceData* pInstanceData )
{
	if( !pInstanceData ) return;

	int calc_cnt = _XDEF_XLINEEFFECT_TOTAL_CURVEPOINTS-1;
	
	CalcNaturalCubic(g_swordtrace_xref[0],calc_cnt, &pInstanceData->ControlPoint[0][0].x );
	CalcNaturalCubic(g_swordtrace_xref[1],calc_cnt, &pInstanceData->ControlPoint[1][0].x );
	CalcNaturalCubic(g_swordtrace_yref[0],calc_cnt, &pInstanceData->ControlPoint[0][0].y );
	CalcNaturalCubic(g_swordtrace_yref[1],calc_cnt, &pInstanceData->ControlPoint[1][0].y );
	CalcNaturalCubic(g_swordtrace_zref[0],calc_cnt, &pInstanceData->ControlPoint[0][0].z );
	CalcNaturalCubic(g_swordtrace_zref[1],calc_cnt, &pInstanceData->ControlPoint[1][0].z );
	m_TotalTraceCnt=(calc_cnt)*m_PointStep+1;
	
	if( m_TotalTraceCnt >= _XDEF_XLINEEFFECT_MAXTRACECOUNT )
	{
		m_TotalTraceCnt=0;
		return;
	}
	
	DWORD temp_cnt = m_TotalTraceCnt-1;
	m_AllSwordStart[temp_cnt][0] = Eval(g_swordtrace_xref[0][0],0);
	m_AllSwordEnd[temp_cnt][0]	 = Eval(g_swordtrace_xref[1][0],0);
	m_AllSwordStart[temp_cnt][1] = Eval(g_swordtrace_yref[0][0],0);
	m_AllSwordEnd[temp_cnt][1]	 = Eval(g_swordtrace_yref[1][0],0);
	m_AllSwordStart[temp_cnt][2] = Eval(g_swordtrace_zref[0][0],0);
	m_AllSwordEnd[temp_cnt][2]   = Eval(g_swordtrace_zref[1][0],0);
	temp_cnt--;
	for(int j = 0; j < calc_cnt; j++)
	{
		for(int i = 1; i <= m_PointStep; i++)
		{
			float u = (i) / (float)m_PointStep;
			m_AllSwordStart[temp_cnt][0]	= Eval(g_swordtrace_xref[0][j],u);
			m_AllSwordEnd[temp_cnt][0]		= Eval(g_swordtrace_xref[1][j],u);
			m_AllSwordStart[temp_cnt][1]	= Eval(g_swordtrace_yref[0][j],u);
			m_AllSwordEnd[temp_cnt][1]		= Eval(g_swordtrace_yref[1][j],u);
			m_AllSwordStart[temp_cnt][2]	= Eval(g_swordtrace_zref[0][j],u);
			m_AllSwordEnd[temp_cnt][2]		= Eval(g_swordtrace_zref[1][j],u);
			temp_cnt--;
		}
	}
	
	int index;		
	for( DWORD i=0; i<m_TotalTraceCnt+1; i++)
	{
		float f_u = (i/(float)m_TotalTraceCnt);
		index	  = (i << 1);		
		m_TraceVBuf[index].x = m_AllSwordStart[i][0];
		m_TraceVBuf[index].y = m_AllSwordStart[i][1];
		m_TraceVBuf[index].z = m_AllSwordStart[i][2];
		m_TraceVBuf[index].u = f_u;
		m_TraceVBuf[index].v = 0.0f;
				
		index++;
		m_TraceVBuf[index].x = m_AllSwordEnd[i][0];
		m_TraceVBuf[index].y = m_AllSwordEnd[i][1];
		m_TraceVBuf[index].z = m_AllSwordEnd[i][2];
		m_TraceVBuf[index].u = f_u;		
		m_TraceVBuf[index].v = 1.0f;		
	}
}

void _XEffect_LineEffect::Render( void )
{
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );	
	gpDev->SetFVF( D3DFVF_XLINEEFFECT_VERTEX );	
	if( m_TotalTraceCnt )
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,(m_TotalTraceCnt<<1)-2,m_TraceVBuf,sizeof(_XLINEEFFECT_VERTEX));
}
