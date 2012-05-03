// XTextureManager.h: interface for the _XTextureController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTEXTURECONTROLLER_H__D844F428_0322_4082_A77D_C7CD847AD136__INCLUDED_)
#define AFX_XTEXTURECONTROLLER_H__D844F428_0322_4082_A77D_C7CD847AD136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTexture.h"
#include "XPackage.h"
#include "XRenderStyleManager.h"
#include "XModelContainerDef.h"

#define _XDEF_TEXTUREMIPLEVEL			0
#define _XDEF_DEFAULTTEXTUREMIPLEVEL	1

#include "XDataPool.h"
#include <vector>

class _RenderObject
{
public :
	_LPXM_MESHOBJECT pObject;
	LPD3DXMATRIX	 pObjectMatrix;
	BOOL			 usesecondbuffer;
	BOOL			 abovewater;
};

class _XTextureController  
{
public:
	typedef std::vector<_XTexture*>				svdef_XTexture;
	typedef std::vector<_XRenderStyleItem*>		svdef_XRenderStyleItem;
	typedef std::vector<_RenderObject*>			svdef_RenderObject;
public:
	// m_nNumTexture 를 따로 두는 이유는 실시간으로 계속 m_svTexture.size() 를 체크해야 하는데
	// 컨테이너 특성상 size() 는 내부적인 계산이 들어 가므로 빠른 계산을 위해 따로 둔다.
	int							m_nNumTexture;
	svdef_XTexture				m_svTexture;

	svdef_XRenderStyleItem		m_svRenderStyleItem;
	_XRenderStyleManager		m_RenderStyleManager;

	std::string					m_strArchiveName;
	_XPackageArchive			m_TextureArchive;


	BOOL						m_bUseThread;
	BOOL						m_bTextureCache;

	D3DFORMAT					m_TextureFormat;

public:
	_XTextureController();
	virtual ~_XTextureController();

	// ================================================================================================
	// XTextureManager.cpp
	// ================================================================================================
	virtual  void	DisposeTexture();

	BOOL	SetTexturePackage(LPCTSTR lpName);
	int		CreateTexture(LPCTSTR lpName, int miplevel = _XDEF_DEFAULTTEXTUREMIPLEVEL, BOOL noresize = FALSE);
	int		FindTextureName(LPCTSTR lpName);
	BOOL	LoadRenderStyle(LPCTSTR lpName);
	void	ReleaseTextureData();
	void	CheckTextureUseHitRate();
	void	ResetTextureCacheTime();

	BOOL	LoadTexture(DWORD index);
	BOOL	ReleaseTexture(DWORD index);

	int		GetRef(DWORD index);
	void	AddRef(DWORD index);
	void	ReleaseRef(DWORD index);


	void	SetTexture( DWORD index, DWORD stage = 0 );
	void	SetTexture_NoRenderStyle( DWORD index, DWORD stage = 0 );

	IDirect3DTexture9*	GetTexture( DWORD index );
	_XTexture*	GetTextureObject( DWORD index );
	int			FindResource( LPSTR resourcename );
	BOOL		IsAlphaBlending( DWORD index );

	BOOL	InitDeviceObject( void );
	void	ReleaseDeviceObject( void );

	void	ReportTextureInfo( void );

	BOOL	BuildTextureList( BOOL createalphachannel = FALSE );
	BOOL	CreateLoadTexture(_XTexture* pTexture, int resourceindex, int miplevel, bool noresize, LPCTSTR lpName, bool b_createalphachannel) ;

	BOOL	TestAlphaChannel( DWORD index, int x, int y );


public:
	void	SetFormat(D3DFORMAT format = D3DFMT_UNKNOWN )				{ m_TextureFormat = format; }

};

#endif // !defined(AFX_XTEXTURECONTROLLER_H__D844F428_0322_4082_A77D_C7CD847AD136__INCLUDED_)
