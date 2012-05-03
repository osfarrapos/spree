// XKeyframeDescriptor.h: interface for the _XKeyframeDescriptor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XKEYFRAMEDESCRIPTOR_H__A5A89E91_093B_4477_A140_3966E641A0FC__INCLUDED_)
#define AFX_XKEYFRAMEDESCRIPTOR_H__A5A89E91_093B_4477_A140_3966E641A0FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
class  _XAnimMeshAbstractClass;
class  _XKeyframeController;

class _XKeyframeDescriptor  
{
public:
protected:
	_XAnimMeshAbstractClass*		m_pAniMeshAbstractClass;
	_XKeyframeController*			m_pKeyframeController;

	BOOL							m_bAniFlag;

	FLOAT							m_fAniTime;

	D3DXMATRIX						m_matWorld;

	D3DXMATRIX*						m_pmatFrame;

public:
	_XKeyframeDescriptor();
	virtual ~_XKeyframeDescriptor();

	void	SetMeshModel( _XAnimMeshAbstractClass* pAniMeshAbstractClass );
	void	LinkKeyframeController( _XKeyframeController* pKeyframeController );
	void	Animation(FLOAT fTimeCount);
	void	Animation(FLOAT fTimeCount, D3DXMATRIX& matWorld);

	void	Render( void );
	void	RenderNoTexture( void );


public:
	inline void		PlayAnimation( BOOL flag )												{ m_bAniFlag = flag; };

	inline void		SetWorldMatrix( D3DXMATRIX& matWorld )									{ m_matWorld = matWorld; }
};

#endif // !defined(AFX_XKEYFRAMEDESCRIPTOR_H__A5A89E91_093B_4477_A140_3966E641A0FC__INCLUDED_)
