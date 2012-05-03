// XEffect_SwordTrace.cpp: implementation of the _XEffect_SwordTrace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XEffect_SwordTrace.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int	g_PointStep = 16;

extern DWORD FtoDW( FLOAT f );

inline float Eval(float v[4],float u)
{
	return ((((v[3]*u) + v[2])*u + v[1])*u + v[0]);
}

//큐빅커브함수.
void _XEffect_SwordTrace::CalcNaturalCubic(float get_abcd[][4],int n, float *vptr)
{
	float gamma[_XDEF_TOTAL_CURVEPOINTS];
	float delta[_XDEF_TOTAL_CURVEPOINTS];
	float D[_XDEF_TOTAL_CURVEPOINTS];
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

_XEffect_SwordTrace::_XEffect_SwordTrace()
{	
	m_TotalTraceCnt		= 0;

	m_pTextureManager	= NULL;
	m_SelectedTextureIndex = 0;
		
	for( int i=0; i<(_XDEF_MAXTRACECOUNT<<1); i++)
	{
		m_TraceVBuf[i].diffuse = 0xFFFFFFFF;		
	}	

	for( i = 0; i < _XDEF_MAXTRACETEXTURECOUNT; i++ )
	{
		TextureArchiveIndex[i] = -1;
	}
}

_XEffect_SwordTrace::~_XEffect_SwordTrace()
{

}

void _XEffect_SwordTrace::InitEffect( _XTextureManager* pTextureManager )
{
	m_pTextureManager = pTextureManager;
	m_SelectedTextureIndex = 0;

	if( m_pTextureManager )
	{
		TCHAR texturenamearray[_XDEF_MAXTRACETEXTURECOUNT][32] = 
		{
			_T("swordeffect.jpg"),
			_T("swordeffect01_blue.tga"),
			_T("swordeffect02_brown.tga"),
			_T("swordeffect03_violet.tga"),
			_T("swordeffect04_green.tga")
		};

		for( int i = 0; i < _XDEF_MAXTRACETEXTURECOUNT; i++ )
		{
			TextureArchiveIndex[i] = m_pTextureManager->FindResource(texturenamearray[i]);
		}
	}
}

void _XEffect_SwordTrace::ChangeTextureIndex( int index )
{
	m_SelectedTextureIndex = index;
}

float g_swordtrace_xref[2][_XDEF_MAXTRACECOUNT][4];
float g_swordtrace_yref[2][_XDEF_MAXTRACECOUNT][4];
float g_swordtrace_zref[2][_XDEF_MAXTRACECOUNT][4];

void _XEffect_SwordTrace::Process( _XEffect_SwordTrace_InstanceData* pInstanceData )
{
	if( !pInstanceData ) return;

	int calc_cnt = _XDEF_TOTAL_CURVEPOINTS-1;
	
	CalcNaturalCubic(g_swordtrace_xref[0],calc_cnt, &pInstanceData->ControlPoint[0][0].x );
	CalcNaturalCubic(g_swordtrace_xref[1],calc_cnt, &pInstanceData->ControlPoint[1][0].x );
	CalcNaturalCubic(g_swordtrace_yref[0],calc_cnt, &pInstanceData->ControlPoint[0][0].y );
	CalcNaturalCubic(g_swordtrace_yref[1],calc_cnt, &pInstanceData->ControlPoint[1][0].y );
	CalcNaturalCubic(g_swordtrace_zref[0],calc_cnt, &pInstanceData->ControlPoint[0][0].z );
	CalcNaturalCubic(g_swordtrace_zref[1],calc_cnt, &pInstanceData->ControlPoint[1][0].z );
	m_TotalTraceCnt=(calc_cnt)*g_PointStep+1;
	
	if( m_TotalTraceCnt >= _XDEF_MAXTRACECOUNT )
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
		for(int i = 1; i <= g_PointStep; i++)
		{
			float u = (i) / (float)g_PointStep;
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

	m_SelectedTextureIndex = pInstanceData->TextureIndex;
}

void _XEffect_SwordTrace::Render( int renderstyleindex )
{	
	/*gpDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	gpDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	gpDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	
	gpDev->SetRenderState( D3DRS_CULLMODE,D3DCULL_NONE	 );
	gpDev->SetRenderState( D3DRS_ZWRITEENABLE ,FALSE	 );
	gpDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE	 );
	gpDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
	gpDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	gpDev->SetRenderState( D3DRS_CULLMODE,  D3DCULL_NONE );
	gpDev->SetRenderState( D3DRS_LIGHTING, FALSE );*/
	
	g_DefaultRenderStyleManager.SetRenderStyle(NULL, renderstyleindex );
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	
	gpDev->SetFVF( D3DFVF_XSWORDTRACE_VERTEX );
		
	if( m_pTextureManager )
		m_pTextureManager->SetTexture( TextureArchiveIndex[ m_SelectedTextureIndex ] );	
	if( m_TotalTraceCnt )
		gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,(m_TotalTraceCnt<<1)-2,m_TraceVBuf,sizeof(_XSWORDTRACE_VERTEX));
}
