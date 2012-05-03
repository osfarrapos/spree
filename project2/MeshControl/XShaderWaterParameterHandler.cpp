// XShaderWaterParameterHandler.cpp: implementation of the _XShaderWaterParameterHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XShaderWaterParameterHandler.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define f_step		 0.0001f
#define f_stepXL	 0.001f

_XShaderWaterParameterHandler::_XShaderWaterParameterHandler()
{
	id = 0;
	add_parameter(p_fStrength,		"Noise strength",			dtFloat);
	add_parameter(p_bDisplace,		"Toggle displacement",		dtBool);		
	add_parameter(p_bSmooth,		"Smooth heightmap",			dtBool);	
	add_parameter(p_bReflRefrStrength,"Reflection/Refraction strength",		dtFloat);
	add_parameter(p_iOctaves,		"Octaves",					dtInt);
	add_parameter(p_fScale,			"Noise scale",				dtFloat);
	add_parameter(p_fFalloff,		"Noise falloff",			dtFloat);
	add_parameter(p_fAnimspeed,		"Animation speed",			dtFloat);
	add_parameter(p_fTimemulti,		"Animation multi",			dtFloat);
	add_parameter(p_bPaused,		"Pause animation",			dtBool);	
	add_parameter(p_bDisplayTargets,"Display rendertargets(D)",	dtBool);		
	add_parameter(p_fSunPosAlpha,	"Sun location horizontal",	dtFloat);
	add_parameter(p_fSunPosTheta,	"Sun location vertical",	dtFloat);		
	add_parameter(p_fSunShininess,	"Sun shininess",			dtFloat);
	add_parameter(p_fSunStrength,	"Sun strength",				dtFloat);		
	add_parameter(p_bAsPoints,		"Render as points",			dtBool);	
	add_parameter(p_fLODbias,		"Mipmap LOD Bias",			dtFloat);		
	add_parameter(p_fElevation,		"Projector elevation",		dtFloat);
	add_parameter(p_fWaterColourR,	"water colour Red",			dtFloat);
	add_parameter(p_fWaterColourG,	"water colour Green",		dtFloat);
	add_parameter(p_fWaterColourB,	"water colour Blue",		dtFloat);
	add_parameter(p_bDrawDuckie,	"Render Duckie",		dtBool);		
	add_parameter(p_bDrawIsland,	"Render Island",		dtBool);		
	add_parameter(p_bDiffuseRefl,	"Diffuse sky reflection",	dtBool);
}

_XShaderWaterParameterHandler::~_XShaderWaterParameterHandler()
{

}

bool _XShaderWaterParameterHandler::add_parameter(unsigned int id, char name[32], unsigned int datatype)
{
	sprintf ( params[id].name, "%s", name);
	params[id].datatype = datatype;
	return true;
}

void _XShaderWaterParameterHandler::value_increase()
{
	switch(params[id].datatype)
	{
	case dtBool:
		{
			params[id].bData = !params[id].bData;
			break;
		}
	case dtInt:
		{
			params[id].iData += 1;
			break;
		}
	case dtFloat:
		{
			params[id].fData += f_step;
			break;
		}
	};

}

void _XShaderWaterParameterHandler::value_decrease()
{
	switch(params[id].datatype)
	{
	case dtBool:
		{
			params[id].bData = !params[id].bData;
			break;
		}
	case dtInt:
		{
			params[id].iData -= 1;
			break;
		}
	case dtFloat:
		{
			params[id].fData -= f_step;
			break;
		}
	};
}

void _XShaderWaterParameterHandler::value_increaseXL()
{
	switch(params[id].datatype)
	{
	case dtBool:
		{
			params[id].bData = !params[id].bData;
			break;
		}
	case dtInt:
		{
			params[id].iData += 10;
			break;
		}
	case dtFloat:
		{
			params[id].fData *= 1.05;
			break;
		}
	};

}

void _XShaderWaterParameterHandler::value_decreaseXL()
{
	switch(params[id].datatype)
	{
	case dtBool:
		{
			params[id].bData = !params[id].bData;
			break;
		}
	case dtInt:
		{
			params[id].iData -= 10;
			break;
		}
	case dtFloat:
		{
			params[id].fData *= 0.95;
			break;
		}
	};
}

void _XShaderWaterParameterHandler::value_reset()
{
	switch(params[id].datatype)
	{
	case dtBool:
		{
			//params[id].bData = !params[id].bData;
			break;
		}
	case dtInt:
		{
			//params[id].iData -= 1;
			break;
		}
	case dtFloat:
		{
			params[id].fData = 0;
			break;
		}
	};
}

const char* _XShaderWaterParameterHandler::get_display()
{
	switch(params[id].datatype)
	{
	case dtBool:
		{
			if (params[id].bData) 
				sprintf ( text, "%s = true", params[id].name);
			else 
				sprintf ( text, "%s = false", params[id].name);
			break;
		}
	case dtInt:
		{
			sprintf ( text, "%s = %i", params[id].name, params[id].iData);			
			break;
		}
	case dtFloat:
		{
			sprintf ( text, "%s = %f", params[id].name, params[id].fData);			
			break;
		}
	};
	return this->text;	
}