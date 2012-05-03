// XStickGauge.h: interface for the _XStickGauge class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XSTICKGAUGE_H_
#define _XSTICKGAUGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XStatic.h"
#include "XImageStatic.h"
#include "XGUIControlCenter.h"

typedef struct
{
	BOOL        enable;
	POINT		position;
	SIZE        windowsize;	
	int         imageindex_border;
	int         imageindex_overlay;	
	_XTextureManager*	texturearchive;
} _XSTICKGAUGE_STRUCTURE;

typedef enum _XGaugeImageClass
{
	_XGAUGEIMAGE_BORDER = 0,
	_XGAUGEIMAGE_OVERLAY,
};

typedef enum _XGaugeProgressType
{
	_XGAUGEPROGRESSTYPE_FORWARD = 0,
	_XGAUGEPROGRESSTYPE_BACKWARD,
};

typedef enum _XGaugeImageDrawType
{
	_XGAUGEIMAGEDRAWTYPE_SCALE,
	_XGAUGEIMAGEDRAWTYPE_RESIZE
};

typedef	void (__stdcall *_XStickGaugeFactorDrawCallback)( POINT& winpos, SIZE& winsize, FLOAT& factor );

class _XStickGauge : public _XGUIObject   
{

protected :			
	
	FLOAT							m_GaugeFactor;
	FLOAT							m_GaugeImageScaleFactor;
	BOOL							m_Blinking;
	DWORD							m_BlinkingPosition;

	BOOL							m_DelayUpdate;
	FLOAT							m_DelayGaugeFactor;
	DWORD							m_PrevDelayTimer;
	DWORD							m_DelaySpeed;

	_XGaugeProgressType				m_GaugeProgressType;	
	
	_XGaugeImageDrawType			m_ImageDrawType;
	RECT							m_ImageClipRect;

	_XImageStatic					m_BorderStatic;
	_XImageStatic					m_DelayBorderStatic;
	_XImageStatic					m_OverlayStatic;

	BOOL							m_VerticalMode;
	RECT							m_OrgRect;

	TCHAR							m_WindowTextFormat[64];
	TCHAR							m_WindowText[128];
	D3DXVECTOR2						m_TextPosition;
	_XGUITooltipCallback			m_TooltipCallbackFunc;
	_XStickGaugeFactorDrawCallback	m_FactorDrawCallbackFunc;

	BOOL							m_IsFactorDraw;
	BOOL							m_bBackGroundDraw; // 2004.06.18->oneway48 insert
	
	BOOL							m_bFadeOutMode;	// 2005. 02.14->hotblood
	DWORD							m_FadeOutStartTime;	// 2005. 02.14->hotblood	

public:
	_XStickGauge();
	virtual ~_XStickGauge();


	virtual BOOL	Create( _XSTICKGAUGE_STRUCTURE& gaugestruct );
	
	virtual void	DestroyWindow( void );					
	
	virtual void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	virtual BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	virtual void	ShowWindow( BOOL show )
	{ 
					m_ShowWindow = show; 

					if( !m_ShowWindow )
					{
						if( g_CurrentFocusedObject == this )
						{
							g_CurrentFocusedObject = NULL;
						}
					}
	}
	virtual void	EnableWindow( BOOL enable )
	{
					m_Enable = enable; 
					m_BorderStatic.EnableWindow( m_Enable );
					m_OverlayStatic.EnableWindow( m_Enable );
					
					if( !m_Enable )
					{
						if( g_CurrentFocusedObject == this )
						{
							g_CurrentFocusedObject = NULL;
						}
					}
	}
	
	virtual BOOL	CheckMousePosition( void );
	
	void			MoveWindow( int X, int Y );

	void			SetCallBackFunction_ToolTip( _XGUITooltipCallback pcallbackfunction )
	{
					m_TooltipCallbackFunc = pcallbackfunction;
	}
	void			SetCallBackFunction_FactorDraw( _XStickGaugeFactorDrawCallback pcallbackfunction )
	{
					m_FactorDrawCallbackFunc = pcallbackfunction;
	}

	void			SetImage( int borderindex, int overlayindex, _XTextureManager* texturearchive = NULL  );
	
	void			SetImageClipRect( _XGaugeImageClass imagetype, int left, int top, int right, int bottom );

	void			SetGaugeFactor( FLOAT factor );
	FLOAT			GetGaugeFactor( void ){ return m_GaugeFactor; }
	void			SetScyncDelayGaugeFactor( void );

	void			SetTextFormat( LPSTR format )
	{
					memset( m_WindowTextFormat, 0, 10 );
					strcpy( m_WindowTextFormat, format );
	}

	void			SetFactorDraw(BOOL draw)
	{
					m_IsFactorDraw = draw;
	}

	void			SetBackGroundDraw(BOOL draw)
	{
					m_bBackGroundDraw = draw;
	}
	
	void			SetBlinking( BOOL blinking, DWORD blinkingpos )
	{
					m_Blinking = blinking;
					m_BlinkingPosition = blinkingpos;
	}

	void			SetGaugeColorFactor( D3DCOLOR colorfactor, D3DCOLOR delaycolorfactor = 0x7fFFFFFF );
	void			SetGaugeProgressType( _XGaugeProgressType progresstype );
	void			SetViewDelayGauge( BOOL show, DWORD speed = 30 );	
	void			SetGaugeImageDrawType( _XGaugeImageDrawType drawtype );
	void			SetGaugeImageScaleFactor( FLOAT factor )
	{
					m_GaugeImageScaleFactor = factor;					
	}
	// 2005.02.15 -> hotblood -----------------------------------------------------------=	
	void			SetFadeOutMode( BOOL flag )
	{
					m_bFadeOutMode = flag;
					m_FadeOutStartTime = g_LocalSystemTime;
	}
	BOOL			GetFadeMode()
	{
					return m_bFadeOutMode;
	}
	void			ResetFadeOutMode(void)
	{
					m_BorderStatic.m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
					m_OverlayStatic.m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	}
	// ----------------------------------------------------------------------------------=
};

#endif // !defined(_XSTICKGAUGE_H_)
