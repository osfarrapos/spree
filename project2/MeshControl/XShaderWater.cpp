// XShaderWater.cpp: implementation of the _XShaderWater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XShaderWater.h"
#include "XShaderWaterParameterHandler.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



_XShaderWater::_XShaderWater()
{
	// create parameter handler
	prm = new parameterhandler();
	prm->set_float( p_fStrength,	0.9f );
	prm->set_bool(	p_bDisplace,	true );
	prm->set_int(	p_iOctaves,		8 );
	prm->set_float( p_fScale,		0.38f );
	prm->set_float( p_fFalloff,		0.607f );
	prm->set_float( p_fAnimspeed,	1.4f );
	prm->set_float( p_fTimemulti,	1.27f );
	prm->set_bool(	p_bPaused,		false );
	
	prm->set_float(	p_fLODbias,		0.0f);
	prm->set_bool(	p_bDisplayTargets, false );
	prm->set_float(	p_fElevation,	7.0f );
	
	//prm->set_float( p_fSunPosAlpha,		2.7f );
	//prm->set_float( p_fSunPosTheta,		0.1f );
	prm->set_float( p_fSunPosAlpha,		1.38f );
	prm->set_float( p_fSunPosTheta,		1.09f );
	prm->set_float( p_fSunShininess,	84.0f );
	prm->set_float( p_fSunStrength,		12.0f );
	
#ifdef CPU_NORMALS
	prm->set_bool(	p_bSmooth,			false );
#else
	prm->set_bool(	p_bSmooth,			true );
#endif
	prm->set_float(	p_bReflRefrStrength,0.1f );
	
	prm->set_float( p_fWaterColourR,	0.17f );
	prm->set_float( p_fWaterColourG,	0.27f );
	prm->set_float( p_fWaterColourB,	0.26f );
	
	prm->set_bool(	p_bAsPoints,		false );
	prm->set_bool(	p_bDrawDuckie,		true );
	prm->set_bool(	p_bDrawIsland,		false );
	prm->set_bool(	p_bDiffuseRefl,		false );
	
	prm->set_active_parameter(p_fStrength);
}

_XShaderWater::~_XShaderWater()
{

}
