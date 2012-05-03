// XParticleMeshObject.h: interface for the _XParticleMeshObject class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XPARTICLEMESHOBJECT_H_
#define _XPARTICLEMESHOBJECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XItem.h"
#include "XMeshContainer.h"
#include "XAnimMeshContainer.h"

class _XModelDescriptor;
class _XBipedController;

// by mahwang
class _XKeyframeDescriptor;
class _XKeyframeController;

class _XParticleMeshObject : public _XItem  
{
public:
	_XAnimMeshAbstractClass*	m_pMeshContainer;
	_XModelDescriptor*			m_pModelDescriptor;
	_XBipedController*			m_pBipedController;	

// by mahwang
	_XKeyframeDescriptor*		m_pKeyframeDescriptor;
	_XKeyframeController*		m_pKeyframeController;

public:
	_XParticleMeshObject();
	virtual ~_XParticleMeshObject();

// by mahwang
	BOOL	LoadKeyframeKey(FILE* Fileptr, LPSTR szFilename );


};

#endif // !defined(_XPARTICLEMESHOBJECT_H_)
