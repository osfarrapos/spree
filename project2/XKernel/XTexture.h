#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#ifdef __cplusplus

#include "xbasedefs.h"

#include "XReference.h"
#include <string>

extern BOOL	g_TexturePreCacheing;

#define _XTEXTUREPRECACHEING_ON	    g_TexturePreCacheing = TRUE;
#define _XTEXTUREPRECACHEING_OFF	g_TexturePreCacheing = FALSE;

//class _XTexture //: public CBase
class _XTexture : public _XReference
{
// public functions ////////////////////////////////////////////////////////
public:
	_XTexture();	
	_XTexture(_XVFile* p_pFP, DWORD mips=0);		
	~_XTexture();
	
	void ZeroMembers( void );
	void Release() { delete this; }
		
	inline operator LPDIRECT3DTEXTURE9 () { return m_tex; }
	
	bool Load( FILE* pVFPtr, int size, DWORD mips=0, bool noresize=false, bool createalphachannel = false );
	bool Load( LPBYTE pVMPtr, int size, DWORD mips=0, bool noresize=false, bool createalphachannel = false );
	
	// cache system 
	void ReleaseTexture( void );
	bool ReLoad( FILE* pVFPtr, int size );
	bool ReLoad( LPBYTE pVMPtr, int size );
	
	bool ThreadReLoad( LPBYTE pVMPtr, int size );
	bool ThreadReLoad( FILE* pVFPtr, int size );

	bool InitDeviceObjects(D3DFORMAT p_fmt = D3DFMT_UNKNOWN);
	void ReleaseDeviceObjects();

	void ReportTextureInfo( void );
// public members //////////////////////////////////////////////////////////
	void		SetFileName(LPSTR name)			{ m_strFileName = name; }
	const char*	GetFileName()					{ return m_strFileName.c_str(); }
public:
	std::string			m_strFileName;
	XString				m_strPath;

	LPBYTE				m_alphachannel;
	int					m_width;
	int					m_height;
	LPDIRECT3DTEXTURE9	m_tex;			// Texture data in graphic memory	
	byte				m_miplevel;
	byte				m_texturequality;
	bool				m_noresize;
	bool				m_initialized;
	DWORD				m_LastUsedTime;
	D3DFORMAT			m_Format;

	bool				m_ThreadLoad;

};

BOOL _XInitializeTextureLoadBuffer( DWORD blockloadingsize, DWORD backgroundloadingsize, DWORD meshloadbuffersize = 0 );
void _XReleaseTextureLoadBuffer( void );

#endif // __cplusplus
#endif // __TEXTURE_H__
