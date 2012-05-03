// XRenderStyleManager.h: interface for the _XRenderStyleManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XRENDERSTYLEMANAGER_H_
#define _XRENDERSTYLEMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
#include "d3dutil.h"
#include "XList.h"

#define _XDEF_HEADER_RENDERSTYLE	0xFE90

enum	_XRS_ZBUFFEROPMODE
{
	_XRS_ZBUFFEROPMODE_READWRITE	=	0,
	_XRS_ZBUFFEROPMODE_READONLY,
	_XRS_ZBUFFEROPMODE_NOOPERATION
};

enum	_XRS_TEXTURESTAGEARGUMENT
{
	_XRS_TSARG_TEXTURE,
	_XRS_TSARG_DIFFUSE,
	_XRS_TSARG_TFACTOR,
	_XRS_TSARG_CURRENT,
	_XRS_TSARG_SPECULAR,
	_XRS_TSARG_SELECTMASK,	
	_XRS_TSARG_TEMP	
};


enum	_XRS_BLENDMODE
{
	_XRS_NOBLEND,					// Cs
	_XRS_BLEND_BLENDINGON,
	_XRS_BLEND_ADD,					// Cs			 + Cd
	_XRS_BLEND_ADDALPHA,			// Cs			 + Ad
	_XRS_BLEND_SATURATE,			// Cs * (1 - Cd) + Cd
	_XRS_BLEND_MOD_SRCALPHA,		// Cs * As		 + Cd * (1 - As)
	_XRS_BLEND_MOD_SRCCOLOR,		// Cs * Cs		 + Cd * (1 - Cs)
	_XRS_BLEND_MOD_DSTCOLOR,		// Cs * Cd		 + Cd * (1 - Cd)
	_XRS_BLEND_MUL_SRCCOL_DSTCOL,	// Cs * Cs		 + Cd * Cd
	_XRS_BLEND_MUL_SRCCOL_ONE,		// Cs * Cs		 + Cd
	_XRS_BLEND_MUL_SRCALPHA_ZERO,	// Cs * As
	_XRS_BLEND_MUL_SRCALPHA_ONE,	// Cs * As		 + Cd
	_XRS_BLEND_MUL_DSTCOL_ZERO		// Cs * Cd
};


enum	_XRS_UVSOURCE
{
	_XRS_UVFROM_MODELDATA_UVSET1,
	_XRS_UVFROM_MODELDATA_UVSET2,
	_XRS_UVFROM_MODELDATA_UVSET3,
	_XRS_UVFROM_MODELDATA_UVSET4,
	_XRS_UVFROM_CAMERASPACENORMAL,
	_XRS_UVFROM_CAMERASPACEPOSITION,
	_XRS_UVFROM_CAMERASPACEREFLTVECT,
	_XRS_UVFROM_WORLDSPACENORMAL,
	_XRS_UVFROM_WORLDSPACEPOSITION,
	_XRS_UVFROM_WORLDSPACEREFLTVECT
};

enum  _XRS_UVEFFECTYPE
{
	_XRS_UVEFFECTYPE_DISABLE = 0,
	_XRS_UVEFFECTYPE_PANNING,
	_XRS_UVEFFECTYPE_OSCILLATION
};

#define _XDEF_DEFAULTRENDERSTYLE_DEFAULT				0
#define _XDEF_DEFAULTRENDERSTYLE_EFFECTBASE				1
#define _XDEF_DEFAULTRENDERSTYLE_RENDERSHADOWMAP		2
#define _XDEF_DEFAULTRENDERSTYLE_LOGINCLOUD				3
#define _XDEF_DEFAULTRENDERSTYLE_CHARACTERSWORDEFFECT	4
#define _XDEF_DEFAULTRENDERSTYLE_MONSTERSWORDEFFECT		5



typedef struct _XRenderStyleStructure
{	
	TCHAR			texturename[4][32];		
	D3DXMATRIX		uvmatrix[4];	
	D3DMATERIAL9	material;
	
	//-------------------------------------------------------
	DWORD			uselight					:	1;	
	DWORD			usespecular					:	1;	
	DWORD			zbufferoperationmode		:	2;	
	DWORD			alphatest					:	1;
	DWORD			alphatestcomparemode		:	4;
	DWORD			cullingmode					:	2;	
	DWORD			useuvmatrix1				:	1;
	DWORD			useuvmatrix2				:	1;
	DWORD			useuvmatrix3				:	1;
	DWORD			useuvmatrix4				:	1;
	DWORD			usetransformbumpenvmapping	:	1;
	DWORD			bumpenvmappingstage			:	2;	// 17
	
	DWORD			ts1_texturecoordcount		:   3;
	DWORD			ts2_texturecoordcount		:   3;
	DWORD			ts3_texturecoordcount		:   3;
	DWORD			ts4_texturecoordcount		:   3;
	
	DWORD			reserved1					:	32-30;
		
	//-------------------------------------------------------
	DWORD			texturefactor;
	DWORD			alphareference;
	FLOAT			bumpmapscale;
	FLOAT			bumpmapoffset;

	//-------------------------------------------------------
	DWORD			ts1_coloroperation			:	5;	// 20
	DWORD			ts2_coloroperation			:	5;
	DWORD			ts3_coloroperation			:	5;
	DWORD			ts4_coloroperation			:	5;
	
	DWORD			ts1_colorarg1				:	3;	// 12
	DWORD			ts2_colorarg1				:	3;
	DWORD			ts3_colorarg1				:	3;
	DWORD			ts4_colorarg1				:	3;

	//-------------------------------------------------------
	DWORD			ts1_alphaoperation			:	5;	// 20
	DWORD			ts2_alphaoperation			:	5;
	DWORD			ts3_alphaoperation			:	5;
	DWORD			ts4_alphaoperation			:	5;
	
	DWORD			ts1_alphaarg1				:	3;	// 12
	DWORD			ts2_alphaarg1				:	3;
	DWORD			ts3_alphaarg1				:	3;
	DWORD			ts4_alphaarg1				:	3;

	//-------------------------------------------------------
	DWORD			ts1_colorarg2				:	3;	// 12
	DWORD			ts2_colorarg2				:	3;
	DWORD			ts3_colorarg2				:	3;
	DWORD			ts4_colorarg2				:	3;

	DWORD			ts1_alphaarg2				:	3;	// 12
	DWORD			ts2_alphaarg2				:	3;
	DWORD			ts3_alphaarg2				:	3;
	DWORD			ts4_alphaarg2				:	3;

	DWORD			ts1_projected				:	1;  // 4
	DWORD			ts2_projected				:	1;
	DWORD			ts3_projected				:	1;
	DWORD			ts4_projected				:	1;

	DWORD			blendmode					:	4;	// 4

	//-------------------------------------------------------
	DWORD			ts1_sourcetype				:	3;	// 12
	DWORD			ts2_sourcetype				:	3;
	DWORD			ts3_sourcetype				:	3;
	DWORD			ts4_sourcetype				:	3;

	DWORD			ts1_uvcoordindex			:	3;	// 12
	DWORD			ts2_uvcoordindex			:	3;
	DWORD			ts3_uvcoordindex			:	3;
	DWORD			ts4_uvcoordindex			:	3;

	DWORD			zbuffercomparemode			:	4;	

	DWORD			reserved3					:	32-28;

	//-------------------------------------------------------
	DWORD			ts1_ucoordadress			:	3;	// 12
	DWORD			ts2_ucoordadress			:	3;	
	DWORD			ts3_ucoordadress			:	3;
	DWORD			ts4_ucoordadress			:	3;
	
	DWORD			ts3_vcoordadress			:	3;	// 12
	DWORD			ts4_vcoordadress			:	3;
	DWORD			ts1_vcoordadress			:	3;
	DWORD			ts2_vcoordadress			:	3;

	DWORD			reserved4					:	32-24;	

	_XRenderStyleStructure()
	{
		memset( this, 0, sizeof(_XRenderStyleStructure) );
		
		D3DUtil_InitMaterial( material, 1.0f, 1.0f, 1.0f );
		material.Specular.r = 0.5f;
		material.Specular.g = 0.5f;
		material.Specular.b = 0.5f;
		material.Specular.a = 1.0f;
		material.Power = 1.0f;

		D3DXMatrixIdentity( &uvmatrix[0] );
		uvmatrix[3] = uvmatrix[2] = uvmatrix[1] = uvmatrix[0];

		uselight					=	TRUE; 
		usespecular					=	TRUE;
		blendmode					=	0;
		zbufferoperationmode		=   _XRS_ZBUFFEROPMODE_READWRITE;
		zbuffercomparemode			=	D3DCMP_LESSEQUAL;		
		alphatest					=	FALSE;
		alphatestcomparemode		=	D3DCMP_GREATEREQUAL;
		cullingmode					=	D3DCULL_CCW;
		
		useuvmatrix1				=	FALSE;
		useuvmatrix2				=	FALSE;
		useuvmatrix3				=	FALSE;
		useuvmatrix4				=	FALSE;		

		usetransformbumpenvmapping	=	FALSE;
		bumpenvmappingstage			=	0;
		
		texturefactor				=	D3DCOLOR_ARGB( 128, 128, 128, 128 );
		alphareference				=	0;
		bumpmapscale				=	1.0f;
		bumpmapoffset				=	0.0f;

		ts1_coloroperation			=	D3DTOP_MODULATE;
		ts2_coloroperation			=	D3DTOP_DISABLE;
		ts3_coloroperation			=	D3DTOP_DISABLE;
		ts4_coloroperation			=	D3DTOP_DISABLE;

		ts1_colorarg1				=	_XRS_TSARG_TEXTURE;
		ts2_colorarg1				=	_XRS_TSARG_TEXTURE;
		ts3_colorarg1				=	_XRS_TSARG_TEXTURE;
		ts4_colorarg1				=	_XRS_TSARG_TEXTURE;

		ts1_colorarg2				=	_XRS_TSARG_DIFFUSE;
		ts2_colorarg2				=	_XRS_TSARG_CURRENT;
		ts3_colorarg2				=	_XRS_TSARG_CURRENT;
		ts4_colorarg2				=	_XRS_TSARG_CURRENT;

		ts1_alphaoperation			=	D3DTOP_MODULATE;
		ts2_alphaoperation			=	D3DTOP_DISABLE;
		ts3_alphaoperation			=	D3DTOP_DISABLE;
		ts4_alphaoperation			=	D3DTOP_DISABLE;

		ts1_alphaarg1				=	_XRS_TSARG_TEXTURE;
		ts2_alphaarg1				=	_XRS_TSARG_TEXTURE;
		ts3_alphaarg1				=	_XRS_TSARG_TEXTURE;
		ts4_alphaarg1				=	_XRS_TSARG_TEXTURE;
		
		ts1_alphaarg2				=	_XRS_TSARG_DIFFUSE;
		ts2_alphaarg2				=	_XRS_TSARG_CURRENT;
		ts3_alphaarg2				=	_XRS_TSARG_CURRENT;
		ts4_alphaarg2				=	_XRS_TSARG_CURRENT;
		
		ts1_sourcetype				=	0;
		ts2_sourcetype				=	4;
		ts3_sourcetype				=	4;
		ts4_sourcetype				=	4;


		ts1_uvcoordindex			=	0;
		ts2_uvcoordindex			=	1;
		ts3_uvcoordindex			=	2;
		ts4_uvcoordindex			=	3;

		ts1_ucoordadress			=	1;
		ts2_ucoordadress			=	1;
		ts3_ucoordadress			=	1;
		ts4_ucoordadress			=	1;

		ts1_vcoordadress			=	1;
		ts2_vcoordadress			=	1;
		ts3_vcoordadress			=	1;
		ts4_vcoordadress			=	1;

		ts1_texturecoordcount		=   2;
		ts2_texturecoordcount		=   2;
		ts3_texturecoordcount		=   2;
		ts4_texturecoordcount		=   2;

		ts1_projected				=   FALSE;
		ts2_projected				=	FALSE;
		ts3_projected				=	FALSE;
		ts4_projected				=	FALSE;
	}

} _XRenderStyleStructure;

class _XTextureManager;
class _XTextureController;

typedef struct uveffectstruct
{
	int		useextexduveffect;
	BOOL	useoscilloscopeuveffect;
	BOOL	userotationeffect;
	
	FLOAT	upanning;
	FLOAT	vpanning;
	
	FLOAT	upanningvelocity;
	FLOAT	vpanningvelocity;
	
	FLOAT	yawrotation;
	FLOAT	pitchrotation;
	FLOAT	rollrotation;
	
	FLOAT	yawrotationvelocity;
	FLOAT	pitchrotationvelocity;
	FLOAT	rollrotationvelocity;
	
	uveffectstruct()
	{
		useextexduveffect			= _XRS_UVEFFECTYPE_DISABLE;
		useoscilloscopeuveffect		= FALSE;
		userotationeffect			= FALSE;
		
		upanning					= -1.0f;
		vpanning					= -1.0f;
		
		upanningvelocity			= 0.0f;
		vpanningvelocity			= 0.0f;
		
		yawrotation					= 0.0f;
		pitchrotation				= 0.0f;
		rollrotation				= 0.0f;
		
		yawrotationvelocity			= 0.0f;
		pitchrotationvelocity		= 0.0f;
		rollrotationvelocity		= 0.0f;
	};
	
} _XRS_UVEFFECT;

class _XRenderStyleItem
{
public:
	TCHAR					m_rsname[32];
	_XRS_UVEFFECT			m_uveffect[4];	
	FLOAT					m_upanningfactor[4];
	FLOAT					m_vpanningfactor[4];
	FLOAT					m_yawrotationfactor[4];	
	FLOAT					m_pitchrotationfactor[4];	
	FLOAT					m_rollrotationfactor[4];
	_XRenderStyleStructure	m_rs;	
	int						m_textureindex[4];
public:
	_XRenderStyleItem();
	~_XRenderStyleItem();

	void	SetRenderStyle( _XTextureController* pTextureManager  );
	void	SetRenderStyle( _XTextureManager* pTextureManager );
	void	SetBumpEnvMapProperties( DWORD BumpEnvMapStage );
};


class _XRenderStyleManager;
extern _XRenderStyleManager g_DefaultRenderStyleManager;
extern _XRenderStyleItem*	g_LastUsedRenderStyle;

extern BOOL					g_RenderAlphaBlendFlag;
extern D3DCOLOR				g_RenderAlphaBlendTextureFactor;

extern	BOOL				g_SelectedMaterialNoModulate2X;
extern	D3DMATERIAL9		g_SelectedObjectMaterial;
extern	D3DMATERIAL9		g_DamagedObjectMaterial;
extern	D3DMATERIAL9*		g_pCurrentSelectMaterial;

class _XRenderStyleManager
{
public:
	static	BOOL		m_UseVertexShader;

	int					m_RenderStyleCount;	
	_XRenderStyleItem*	m_RenderStyleList;
	
public:
	_XRenderStyleManager();
	virtual ~_XRenderStyleManager();	

	BOOL	Load( LPTSTR Filename );
	BOOL	Load( FILE* Fileptr   );
	BOOL	Reload( LPTSTR Filename );
	void    Dispose( void );
	
	void	SetRenderStyle( _XTextureManager* pTextureManager, int index );
		
	static  void SetDefaultRenderStyle( _XTextureManager* pTextureManager, int index )
	{	
			g_DefaultRenderStyleManager.SetRenderStyle(pTextureManager,index);
	} 	
	static  void SetDefaultRenderState( void );
	
	int     FindRenderStyle( LPCTSTR texturename );
	_XRenderStyleItem* GetRenderStyle( int index )
	{
		if( index < 0 || index >= m_RenderStyleCount ) return NULL;
		return m_RenderStyleList+index;
	}
};

#endif // !defined(_XRENDERSTYLEMANAGER_H_)
