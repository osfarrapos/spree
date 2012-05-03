#pragma once

///////////////////////////////////////////////////////////////////////  
//	Include Files
#include "SpeedTreeRT.h"
#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>
#include <vector>
#include "SpeedWind.h"

class _XQuadTree;

typedef struct 
{
	TCHAR	compositemapname[64];
	TCHAR	sptmodelname[64];
	FLOAT   scale;
} _XST_TemplateData;

class _XST_WrapperCore
{
// Default data
private:
	float									m_fAccumTime;	
	float									m_fWindStrength;

	DWORD									m_D3DFVF_SPEEDTREE_BRANCH_VERTEX;
	DWORD									m_D3DFVF_SPEEDTREE_LEAF_VERTEX;
	DWORD									m_D3DFVF_SPEEDTREE_BILLBOARD_VERTEX;
public:

	BOOL									m_UseVertexShader;

	int										m_ObjectContainerCount;
	_XST_AbstractObject*					m_ObjectContainerArray;
	
	int										m_TemplateDataCount;
	_XST_TemplateData*						m_TemplateData;

	FLOAT									m_RenderLimitDistance;

	_XSpeedWind								m_cSpeedWind;

	BOOL									ReserveObjectContainerArray( int count );	
	
public:
	_XST_WrapperCore();
	~_XST_WrapperCore();

	void									SetUseVertexShader( BOOL use );

	BOOL									Initialize( void );
	void									Release( void );
	void									CleanUpInstance( void );

	BOOL									InitDeviceObjects( void );
	void									DeleteDeviceObjects( void );
	void									InitSpeedTreeLighting( void );
	void									SetupWindMatrices( void );

	void									Process( void );
	
	BOOL									LoadTemplateData( void );
	int										SearchTemplateData( LPTSTR lpszSPTModelFilename );

	BOOL									LoadSpeedTreeInstanceData( FILE* Fileptr );

	void									ResetRenderStack( void );
	void									DrawRenderStack( void );

	bool									InitVertexShaders( );	
	FLOAT									GetWindStrengths( void ){ return m_fWindStrength;	}
	void									UpdateWindStrengths( FLOAT strength );
	void									UploadWindMatrix(unsigned int uiLocation, const float* pMatrix) const; 

	void									CheckInsideFrustum( void );

	void									SetLodLimits( void );
};

