// XTextureObjectController.cpp: implementation of the _XTextureObjectController class.
//
//////////////////////////////////////////////////////////////////////

#include "XTextureObjectController.h"

#include "XKernel.h"

#include "LoadThread.h"

#include "XDataPool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XTextureObjectController::_XTextureObjectController()
{
	m_nNumObjectStack	= 0;
	m_pObjectStack		=   NULL;
	m_pAlphaObjectStack =   NULL;
}

_XTextureObjectController::~_XTextureObjectController()
{

}

void _XTextureObjectController::DisposeTexture()
{
	_XTextureController::DisposeTexture();

		// Object
	m_nNumObjectStack = 0;
	SAFE_DELETE_ARRAY( m_pObjectStack );
	SAFE_DELETE_ARRAY( m_pAlphaObjectStack );
	m_RenderObjectPool.DeleteData();
}

BOOL _XTextureObjectController::BuildTextureList( BOOL createalphachannel ) 
{
	if (!_XTextureController::BuildTextureList(createalphachannel))
	{
		return FALSE;
	}

	if( !ReserveObjectStack() )
	{
		return FALSE;	
	}
	return TRUE;
}

BOOL _XTextureObjectController::ReserveObjectStack( void )
{
	SAFE_DELETE_ARRAY( m_pObjectStack );
	SAFE_DELETE_ARRAY( m_pAlphaObjectStack );
	m_nNumObjectStack = 0;

	m_RenderObjectPool.DeleteData();


	if( m_svTexture.empty() ) return TRUE;

	m_RenderObjectPool.Reserve(50);

	m_nNumObjectStack = m_svTexture.size();
	m_pObjectStack = new svdef_RenderObject[m_nNumObjectStack];
	if( !m_pObjectStack )
	{
		_XFatalError( "Can't allocate draw stack [%s]", m_strArchiveName.c_str() );
		return FALSE;
	}	

	m_pAlphaObjectStack = new svdef_RenderObject[m_nNumObjectStack];
	if( !m_pAlphaObjectStack )
	{
		_XFatalError( "Can't allocate alphablend draw stack [%s]", m_strArchiveName.c_str() );
		return FALSE;
	}	

	return TRUE;
}

void _XTextureObjectController::PushRenderObject( int stackindex, _RenderObject* prenderobject )
{
	if(!prenderobject) return;
	
	if( stackindex < 0 || stackindex >= m_svTexture.size() )
	{
		_XLog( "WARNING : Invalid stack index or overflow stack index" );
		delete prenderobject;
		return;
	}

	m_pObjectStack[stackindex].push_back(prenderobject);
}

void _XTextureObjectController::PushAlphaRenderObject( int stackindex, _RenderObject* prenderobject )
{
	if(!prenderobject) return;
	
	if( stackindex < 0 || stackindex >= m_svTexture.size() )
	{
		_XLog( "WARNING : Invalid stack index or overflow stack index" );
		delete prenderobject;
		return;
	}
	
	m_pAlphaObjectStack[stackindex].push_back(prenderobject);
}

void _XTextureObjectController::RenderAboveWaterObjectStack( void )
{
	if( !m_pObjectStack ) return;
	
	if( FAILED( gpDev->SetFVF( D3DFVF_XMESH_VERTEX ) ) ) return;
	
	svdef_RenderObject::iterator it;
	_RenderObject* pRenderObject;
	for( int i = 0; i < m_nNumObjectStack; i++ )
	{				
		if( m_pObjectStack[i].empty() ) continue;
		
		if( !m_svRenderStyleItem.empty() && m_svRenderStyleItem[i] )
		{
			m_svRenderStyleItem[i]->SetRenderStyle( this );
		}
		else
		{
			m_RenderStyleManager.SetRenderStyle( NULL, 0 );
			SetTexture_NoRenderStyle( i, 0 );
		}		
		
		gpDev->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
		
		for (it = m_pObjectStack[i].begin(); it != m_pObjectStack[i].end(); ++ it)
		{
			pRenderObject = (*it);
			if( pRenderObject->abovewater )
			{
				if(pRenderObject->pObject->nVertexCnt > 0)
				{
					gpDev->SetTransform( D3DTS_WORLD, pRenderObject->pObjectMatrix );					
					
					if( pRenderObject->usesecondbuffer )
					{		
						gpDev->SetStreamSource( 0, pRenderObject->pObject->pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
						gpDev->SetIndices( pRenderObject->pObject->pIndex );
						gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pRenderObject->pObject->nVertexCnt, 
							0, pRenderObject->pObject->nIndexCnt );
					}
					else
					{							
						gpDev->SetStreamSource( 0, pRenderObject->pObject->pVertex, 0, sizeof(_XMESH_VERTEX) );
						gpDev->SetIndices( pRenderObject->pObject->pIndex );
						
						gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pRenderObject->pObject->nVertexCnt, 
							0, pRenderObject->pObject->nIndexCnt );
					}	
				}
			}
		}
	}
}

void _XTextureObjectController::RenderAlphaObjectStack( void )
{
	if( !m_pObjectStack ) return;
	
	if( FAILED( gpDev->SetFVF( D3DFVF_XMESH_VERTEX ) ) ) return;
	
	_RenderObject* pRenderObject;
	svdef_RenderObject::iterator it;

	g_RenderAlphaBlendFlag = TRUE;
	g_RenderAlphaBlendTextureFactor = ( (64<<24) | 0xFFFFFF );

	for( int i = 0; i < m_nNumObjectStack; i++ )
	{				
		if( m_pAlphaObjectStack[i].empty() ) continue;
		
		if( !m_svRenderStyleItem.empty() && m_svRenderStyleItem[i] )
		{
			m_svRenderStyleItem[i]->SetRenderStyle( this );
		}
		else
		{
			m_RenderStyleManager.SetRenderStyle( NULL, 0 );
			SetTexture_NoRenderStyle( i, 0 );
		}

		for (it = m_pAlphaObjectStack[i].begin(); it != m_pAlphaObjectStack[i].end(); ++ it)
		{
			pRenderObject = (*it);
			if(pRenderObject->pObject->nVertexCnt > 0)
			{
				gpDev->SetTransform( D3DTS_WORLD, pRenderObject->pObjectMatrix );
				
				if( pRenderObject->usesecondbuffer )
				{		
					gpDev->SetStreamSource( 0, pRenderObject->pObject->pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
					gpDev->SetIndices( pRenderObject->pObject->pIndex );
					gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pRenderObject->pObject->nVertexCnt, 
						0, pRenderObject->pObject->nIndexCnt );
				}
				else
				{							
					gpDev->SetStreamSource( 0, pRenderObject->pObject->pVertex, 0, sizeof(_XMESH_VERTEX) );
					gpDev->SetIndices( pRenderObject->pObject->pIndex );
					
					gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pRenderObject->pObject->nVertexCnt, 
						0, pRenderObject->pObject->nIndexCnt );
				}	
			}
		}
	}

	g_RenderAlphaBlendFlag = FALSE;
}

void _XTextureObjectController::RenderObjectStack( void )
{
	if( !m_pObjectStack ) return;
	
	if( FAILED( gpDev->SetFVF( D3DFVF_XMESH_VERTEX ) ) ) return;

	_RenderObject* pRenderObject;
	svdef_RenderObject::iterator it;

	for( int i = 0; i < m_nNumObjectStack; i++ )
	{				
		if( m_pObjectStack[i].empty() ) continue;

		if( !m_svRenderStyleItem.empty() && m_svRenderStyleItem[i] )
		{
			m_svRenderStyleItem[i]->SetRenderStyle( this );
		}
		else
		{
			m_RenderStyleManager.SetRenderStyle( NULL, 0 );
			SetTexture_NoRenderStyle( i, 0 );
		}
		
		for (it = m_pObjectStack[i].begin(); it != m_pObjectStack[i].end(); ++ it)
		{
			pRenderObject = (*it);
			if(pRenderObject->pObject->nVertexCnt > 0)
			{
				gpDev->SetTransform( D3DTS_WORLD, pRenderObject->pObjectMatrix );

				if( pRenderObject->usesecondbuffer )
				{		
					gpDev->SetStreamSource( 0, pRenderObject->pObject->pSecondVertexBuffer, 0, sizeof(_XMESH_VERTEX) );
					gpDev->SetIndices( pRenderObject->pObject->pIndex );
					gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pRenderObject->pObject->nVertexCnt, 
																	 0, pRenderObject->pObject->nIndexCnt );
				}
				else
				{							
					gpDev->SetStreamSource( 0, pRenderObject->pObject->pVertex, 0, sizeof(_XMESH_VERTEX) );
					gpDev->SetIndices( pRenderObject->pObject->pIndex );
					
					gpDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pRenderObject->pObject->nVertexCnt, 
																	 0, pRenderObject->pObject->nIndexCnt );
				}	
			}
		}
	}
}

void _XTextureObjectController::ResetObjectStack( void )
{
	if( m_pObjectStack )
	{	
		for( int i = 0; i < m_nNumObjectStack; i++ )
		{
			m_pObjectStack[i].clear();
		}
	}

	if( m_pAlphaObjectStack )
	{	
		for( int i = 0; i < m_nNumObjectStack; i++ )
		{
			m_pAlphaObjectStack[i].clear();
		}
	}
	m_RenderObjectPool.Reset();
}
