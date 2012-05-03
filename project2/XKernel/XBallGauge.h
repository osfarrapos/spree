// XBallGauge.h: interface for the _XBallGauge class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XBALLGAUGE_H_
#define _XBALLGAUGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _XDEF_GAUGELINECOLOR		D3DCOLOR_ARGB( 196, 0, 255, 0 )
#define _XDEF_GAUGELINECOUNT		36

#include "XGUIControlCenter.h"

class _XBallGauge  
{
private:
	int					m_Level;
	int					m_LevelPercentage;

	int					m_TextureIndex1;
	_XTextureManager*	m_pTextureArchive;

	FLOAT				m_fLeft;
	FLOAT				m_fTop;
	FLOAT				m_fRight;
	FLOAT				m_fBottom;

	_XGUITooltipCallback	m_TooltipCallbackFunc;

public:
	_XBallGauge();
	virtual ~_XBallGauge();

	void SetGaugePercentage( int percentage )
	{ 	
		m_LevelPercentage = percentage;

		m_Level = (int)( (FLOAT)(_XDEF_GAUGELINECOUNT) * ((FLOAT)m_LevelPercentage / 100.f) );
		if( m_Level > _XDEF_GAUGELINECOUNT ) m_Level = _XDEF_GAUGELINECOUNT; 		
	}
	void SetGaugeColor( D3DCOLOR color );

	int  GetGaugeLevel( void ){ return m_Level; }
	int  GetGaugePercentage( void ){ return m_LevelPercentage; }

	void Render( void );

	void DrawToolTip(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void SetRigion( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom )
	{
		m_fLeft		=	left;
		m_fTop		=	top;
		m_fRight	=	right;
		m_fBottom	=	bottom;
	}
	void SetTexture( _XTextureManager* ptexturearchive, int textureindex1 );
	BOOL TestAlphaChannel( int x, int y );

	void SetCallBackFunction_ToolTip( _XGUITooltipCallback pcallbackfunction )
	{
		 m_TooltipCallbackFunc = pcallbackfunction;
	}


};

#endif // !defined(_XBALLGAUGE_H_)
