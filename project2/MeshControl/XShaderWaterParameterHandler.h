// XShaderWaterParameterHandler.h: interface for the _XShaderWaterParameterHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSHADERWATERPARAMETERHANDLER_H__6B902F98_1745_4110_9E99_806ABB165233__INCLUDED_)
#define AFX_XSHADERWATERPARAMETERHANDLER_H__6B902F98_1745_4110_9E99_806ABB165233__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum dtDatatypes
{
	dtBool=0,
		dtFloat,
		dtInt
};

enum pParameters
{
	p_fStrength=0,
		p_fFalloff,	
		p_fScale,
		p_bSmooth,
		p_bReflRefrStrength,
		p_iOctaves,	
		p_fLODbias,
		p_fAnimspeed,	
		p_fTimemulti,
		p_bPaused,
		p_bAsPoints,
		p_fElevation,
		p_bDisplayTargets,
		p_fSunPosTheta,
		p_fSunPosAlpha,
		p_fSunShininess,
		p_fSunStrength,
		p_fWaterColourR,
		p_fWaterColourG,
		p_fWaterColourB,
		p_bDisplace,
		p_bDrawDuckie,
		p_bDrawIsland,
		p_bDiffuseRefl,
		numParameters
};

struct parameter
{
	char name[32];
	unsigned int datatype;
	union{
		int		iData;
		bool	bData;
		float	fData;
	};
};

class _XShaderWaterParameterHandler  
{
public:
	_XShaderWaterParameterHandler();
	virtual ~_XShaderWaterParameterHandler();

	bool add_parameter(unsigned int id, char name[32], unsigned int datatype);
	void next_parameter(){ id = (id+1)%numParameters; }
	void previous_parameter(){ id = (id-1+numParameters)%numParameters; }
	
	float get_float(pParameters id){ return this->params[id].fData; }
	int get_int(pParameters id){ return this->params[id].iData; }
	bool get_bool(pParameters id){ return this->params[id].bData; }
	void set_float(pParameters id, float value){ params[id].fData = value; }
	void set_int(pParameters id, int value){ params[id].iData = value; }
	void set_bool(pParameters id, bool value){ params[id].bData = value; }
	
	void value_increase();
	void value_decrease();
	void value_increaseXL();
	void value_decreaseXL();
	void value_reset();
	void set_active_parameter(pParameters id){ this->id = id; };
	
	const char* get_display();
	
	parameter params[numParameters];
private:
	unsigned int id;
	char text[50];

};

#endif // !defined(AFX_XSHADERWATERPARAMETERHANDLER_H__6B902F98_1745_4110_9E99_806ABB165233__INCLUDED_)
