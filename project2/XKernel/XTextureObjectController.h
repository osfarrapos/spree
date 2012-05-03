// XTextureObjectController.h: interface for the _XTextureObjectController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTEXTUREOBJECTCONTROLLER_H__7FC986A4_945F_4D27_82E9_A957D94A6A34__INCLUDED_)
#define AFX_XTEXTUREOBJECTCONTROLLER_H__7FC986A4_945F_4D27_82E9_A957D94A6A34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTextureController.h"

class _XTextureObjectController : public _XTextureController
{
public:
	int							m_nNumObjectStack;
	svdef_RenderObject*			m_pObjectStack;
	svdef_RenderObject*			m_pAlphaObjectStack;
	_XDataPool<_RenderObject>	m_RenderObjectPool;
public:
	_XTextureObjectController();
	virtual ~_XTextureObjectController();

	virtual void	DisposeTexture();

public:
	BOOL	ReserveObjectStack( void );
	BOOL	BuildTextureList( BOOL createalphachannel );
	
	void	RenderObjectStack( void );
	void	RenderAlphaObjectStack( void );

	void	RenderAboveWaterObjectStack( void );
	void	ResetObjectStack( void );
	void	PushRenderObject( int stackindex, _RenderObject* prenderobject );
	void	PushAlphaRenderObject( int stackindex, _RenderObject* prenderobject );
};

#endif // !defined(AFX_XTEXTUREOBJECTCONTROLLER_H__7FC986A4_945F_4D27_82E9_A957D94A6A34__INCLUDED_)
