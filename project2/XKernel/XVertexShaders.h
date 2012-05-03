///////////////////////////////////////////////////////////////////////  
//	SpeedTreeRT DirectX Example
//
//	(c) 2003 IDV, Inc.
//
//	This example demonstrates how to render trees using SpeedTreeRT
//	and DirectX.  Techniques illustrated include ".spt" file parsing,
//	static lighting, dynamic lighting, LOD implementation, cloning,
//	instancing, and dynamic wind effects.
//
//
//	*** INTERACTIVE DATA VISUALIZATION (IDV) PROPRIETARY INFORMATION ***
//
//	This software is supplied under the terms of a license agreement or
//	nondisclosure agreement with Interactive Data Visualization and may
//	not be copied or disclosed except in accordance with the terms of
//	that agreement.
//
//      Copyright (c) 2001-2003 IDV, Inc.
//      All Rights Reserved.
//
//		IDV, Inc.
//		1233 Washington St. Suite 610
//		Columbia, SC 29201
//		Voice: (803) 799-1699
//		Fax:   (803) 931-0320
//		Web:   http://www.idvinc.com

#pragma once

#include "D3DUtil.h"
#include <map>
#include <string>


///////////////////////////////////////////////////////////////////////  
//  Constants

// vertex shader constant locations
const unsigned int c_nCompoundMatrix = 0;
const unsigned int c_nLeafMaps = 4;
const unsigned int c_nTreePos = 52;
const unsigned int c_nMatrixIndexConverter = 53;
const unsigned int c_nWindMatrices = 54;

extern void _XSetDefaultPath( LPSTR localpath );
extern void _XFatalError(TCHAR *str, ...);
///////////////////////////////////////////////////////////////////////  
//	CSpeedTreeRTExample::LoadShader

static DWORD LoadShader(LPDIRECT3DDEVICE9 pDevice, const char* pShaderFile, const DWORD* pShaderDecl)
{
    static std::map<std::string, DWORD> mShaderCache;

	_XSetDefaultPath( "./Data/Script" );

    DWORD dwShader = mShaderCache[pShaderFile];
    if (!dwShader)
    {
        // assemble shader from .vsh text file
	    LPD3DXBUFFER pCode, pError;
        if (D3DXAssembleShaderFromFile(pShaderFile, 0, NULL, &pCode, &pError) == D3D_OK)
		{
			if (pDevice->CreateVertexShader(pShaderDecl, (DWORD*) pCode->GetBufferPointer( ), &dwShader, 0) == D3D_OK)
				mShaderCache[pShaderFile] = dwShader;
			else
			{				
				_XFatalError( "Failed to create vertex shader [ %s ]", pShaderFile );
			}
		}
		else
        {            
	        if (pError)
				_XFatalError( "Failed to assemble vertex shader [ %s ],\n%s", pShaderFile, pError->GetBufferPointer() );
	        else
	            _XFatalError( "Failed to assemble vertex shader [ %s ]", pShaderFile );
        }

		if (pCode)
    		pCode->Release( );
    }

    return dwShader;
}
