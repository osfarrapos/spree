// XParticleMeshObject.cpp: implementation of the _XParticleMeshObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XParticleMeshObject.h"

// by mahwang
#include "XKeyframeController.h"
#include "XKeyframeDescriptor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XParticleMeshObject::_XParticleMeshObject()
{
	m_pMeshContainer   = NULL;
	m_pModelDescriptor = NULL;
	m_pBipedController = NULL;

// by mahwang
	m_pKeyframeDescriptor = NULL;
	m_pKeyframeController = NULL;
}

_XParticleMeshObject::~_XParticleMeshObject()
{
	SAFE_DELETE( m_pMeshContainer   );
	SAFE_DELETE( m_pModelDescriptor );
	SAFE_DELETE( m_pBipedController );

// by mahwang
	SAFE_DELETE( m_pKeyframeDescriptor );
	SAFE_DELETE( m_pKeyframeController );
}


// by mahwang
BOOL _XParticleMeshObject::LoadKeyframeKey(FILE* Fileptr, LPSTR szFilename )
{
	if (m_pKeyframeController != NULL)
	{
		m_pKeyframeController->LoadKey(Fileptr);
		if (m_pKeyframeController->GetNodeCnt() != m_pMeshContainer->GetNodeCnt())
		{
			return TRUE;
		}
		if (m_pMeshContainer != NULL)
		{
			if( !m_pMeshContainer->InitializeSecondBufferFormTM(m_pKeyframeController->m_lpCurKeyframe) )
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
