// XTextureManager.h: interface for the _XTextureManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XTEXTUREMANAGER_H_
#define _XTEXTUREMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTexture.h"
#include "XPackage.h"
#include "XRenderStyleManager.h"
#include "XModelContainerDef.h"

#include "XStringItem.h"

#define _XDEF_TEXTUREMIPLEVEL			0
#define _XDEF_DEFAULTTEXTUREMIPLEVEL	1

#include "XDataPool.h"
#include <vector>

class _XRenderObject
{
public :
	_LPXM_MESHOBJECT pObject;
	LPD3DXMATRIX	 pObjectMatrix;
	BOOL			 usesecondbuffer;
	BOOL			 abovewater;
public :

	//_XRenderObject()
	//{
	//	pObject = NULL;
	//	pObjectMatrix = NULL;
	//	usesecondbuffer = FASLE;
	//}	
};

class _XTextureManager  
{
public:
	_XTextureManager();
	virtual ~_XTextureManager();

	void				DisposeTexture( void );

	// Linked list function
	BOOL				SetTexturePackage( LPSTR archivename );
	BOOL				BuildTextureList( BOOL createalphachannel = FALSE );
	_XTexture*			CreateLoadTexture(int resourceindex, int miplevel, bool noresize, TCHAR* pName, bool b_createalphachannel) ;

	// AddTextureList 는 GetResourceIndex 와 BuildTextureList 를 다 끝낸후 텍스쳐를 더 추가 해야 하는 경우 사용한다.
	int					AddTextureList( LPSTR resourcename, 
										int miplevel = _XDEF_DEFAULTTEXTUREMIPLEVEL, 
										BOOL noresize = FALSE, 
										bool b_createalphachannel = false );
	int					GetResourceIndex( LPSTR resourcename, 
										  int miplevel = _XDEF_DEFAULTTEXTUREMIPLEVEL, 
										  BOOL noresize = FALSE );
	int					FindTextureName(LPSTR resourcename);
	
	// Array list function
	BOOL				LoadTexturePackage( LPSTR archivename, 
											int miplevel = _XDEF_DEFAULTTEXTUREMIPLEVEL, 
											BOOL noresize = FALSE, 
											BOOL createalphachannel = FALSE );
	BOOL				MergeTexturePackage( LPSTR archivename );
	int					FindResource( LPSTR resourcename );


	IDirect3DTexture9*	GetTexture( int index );
	_XTexture*			GetTextureObject( int index );
	void				SetTexture( int index, DWORD stage = 0 );
	void				SetTexture_NoRenderStyle( int index, DWORD stage = 0 );
	void				SetTexture_NoRenderStyleUseCulling( int index, DWORD stage = 0 );
	BOOL				TestAlphaChannel( int index, int x, int y );	
	BOOL				IsAlphaBlending( int index );

	BOOL				InitDeviceObject( void );
	void				ReleaseDeviceObject( void );

	void				SetFormat( D3DFORMAT textureformat = D3DFMT_UNKNOWN ){ m_TextureFormat = textureformat; }

	void				CheckTextureUseHitRate( void );
	void				ResetTextureCacheTime( void );
	
	void				ReleaseTextureData( void );
	
	BOOL				PreCachingTexture( int index );

public:
	D3DFORMAT			m_TextureFormat;

	typedef std::vector<_XTexture*>		svdef_XTexture;
	svdef_XTexture		m_svXTextureBuffer;

	TCHAR				m_ArchiveName[_MAX_PATH];

	typedef std::vector<_XStringItem*>		svdef_XStringItem;
	svdef_XStringItem	m_svTextureName;

	BOOL				m_Initialized;

	_XRenderStyleItem**  m_pRenderStyleBuffer;
	_XRenderStyleManager m_RenderStyleManager;

	_XPackageArchive	 m_TextureArchive;

	BOOL				m_bUseThread;

public:
	typedef std::vector<_XRenderObject*>	svdef_XRenderObject;

	int							m_nNumObjectStack;
	svdef_XRenderObject*		m_pObjectStack;
	svdef_XRenderObject*		m_pLightmapObjectStack;
	_XDataPool<_XRenderObject>	m_RenderObjectPool;
	_XDataPool<_XRenderObject>	m_RenderLightmapObjectPool;


	BOOL				ReserveObjectStack( void );
	
	void				RenderObjectStack( void );
	void				RenderLightMapObjectStack( void );

	void				RenderAboveWaterObjectStack( void );
	void				RenderAvobeWaterLightMapObjectStack( void );
	void				ResetObjectStack( void );
	void				PushRenderObject( int stackindex, _XRenderObject* prenderobject );
	void				PushLightmapRenderObject( int stackindex, _XRenderObject* prenderobject );

	BOOL				ReadyRenderStyle(  LPTSTR filename );
	BOOL				AttachRenderStyle( LPTSTR filename );
	
	void				ReportTextureInfo( void );

	_XRenderObject*		PopRenderObject( void )			{ return m_RenderObjectPool.New(); }
	_XRenderObject*		PopRenderLightmapObject( void )	{ return m_RenderLightmapObjectPool.New(); }

	void				AttachAlphaChannelData( _XTexture* pTextureObject );
};

extern _XPackageArchive	 g_TextureAlphaInfoArchive;

#endif 
