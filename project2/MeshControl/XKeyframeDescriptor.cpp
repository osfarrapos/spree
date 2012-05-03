// XKeyframeDescriptor: implementation of the _XKeyframeDescriptor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKeyframeDescriptor.h"

#include "XKeyframeController.h"
#include "XAnimMeshContainer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////// 
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XKeyframeDescriptor::_XKeyframeDescriptor()
{
	m_bAniFlag					= FALSE;
	m_fAniTime					= 0.f;

	m_pAniMeshAbstractClass		= NULL;
	m_pKeyframeController		= NULL;

	m_pmatFrame					= NULL;

	D3DXMatrixIdentity(&m_matWorld);
}

_XKeyframeDescriptor::~_XKeyframeDescriptor()
{
	SAFE_DELETE_ARRAY( m_pmatFrame );
}

void _XKeyframeDescriptor::SetMeshModel( _XAnimMeshAbstractClass* pAniMeshAbstractClass )
{
	m_pAniMeshAbstractClass = pAniMeshAbstractClass;

	m_pmatFrame = new D3DXMATRIX[pAniMeshAbstractClass->GetNodeCnt()];
	for (int i = 0; i < pAniMeshAbstractClass->GetNodeCnt(); ++ i)
	{
		D3DXMatrixIdentity(&m_pmatFrame[i]);
	}
}

void _XKeyframeDescriptor::LinkKeyframeController( _XKeyframeController* pKeyframeController )
{
	m_pKeyframeController = pKeyframeController;
}


void _XKeyframeDescriptor::Animation(FLOAT fTimeCount)
{
	if( !m_pKeyframeController ) return;
	if( !m_bAniFlag || m_pKeyframeController->GetMotionCnt() == 0) return;

	if (m_pKeyframeController->GetNodeCnt() != m_pAniMeshAbstractClass->GetNodeCnt()) return;

	if (m_pKeyframeController->m_lpCurKeyframe == NULL) return;

	m_fAniTime = fTimeCount;
	if (m_pKeyframeController->m_lpCurKeyframe->fMaxTime < m_fAniTime)
	{
		m_fAniTime = m_fAniTime - ((int)(m_fAniTime / m_pKeyframeController->m_lpCurKeyframe->fMaxTime) * m_pKeyframeController->m_lpCurKeyframe->fMaxTime); 
	}

	m_pKeyframeController->FrameAnimation(m_fAniTime, m_matWorld, m_pmatFrame);
}

void _XKeyframeDescriptor::Animation(FLOAT fTimeCount, D3DXMATRIX& matWorld)
{
	if( !m_pKeyframeController ) return;
	if( !m_bAniFlag || m_pKeyframeController->GetMotionCnt() == 0) return;

	if (m_pKeyframeController->GetNodeCnt() != m_pAniMeshAbstractClass->GetNodeCnt()) return;

	if (m_pKeyframeController->m_lpCurKeyframe == NULL) return;

	m_fAniTime = fTimeCount;
	if (m_pKeyframeController->m_lpCurKeyframe->fMaxTime < m_fAniTime)
	{
		m_fAniTime = m_fAniTime - ((int)(m_fAniTime / m_pKeyframeController->m_lpCurKeyframe->fMaxTime) * m_pKeyframeController->m_lpCurKeyframe->fMaxTime); 
	}

	m_matWorld = matWorld;
	m_pKeyframeController->FrameAnimation(m_fAniTime, m_matWorld, m_pmatFrame);
}


void _XKeyframeDescriptor::Render( void )
{
	m_pAniMeshAbstractClass->Render(m_pmatFrame);
}

void _XKeyframeDescriptor::RenderNoTexture( void )
{
	m_pAniMeshAbstractClass->RenderNoTexture(m_pmatFrame);
}