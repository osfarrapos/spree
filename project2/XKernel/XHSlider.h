// XHSlider.h: interface for the _XHSlider class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XHSLIDER_H_
#define _XHSLIDER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XImageStatic.h"

typedef struct
{
	BOOL				enable;
	POINT				position;
	SIZE				windowsize;
	DWORD				commandid;
	int					bordertextureindex;
	int					indicatortextureindex;
	
	int					trackbarnormaltextureindex;
	int					trackbarpushtextureindex;
	int					trackbarhighlighttextureindex;	
	POINT				trackbarbaseoffset;
	SIZE				trackbarsize;

	int					topbordertextureindex;

	_XTextureManager*	texturearchive;	
	TCHAR				WindowTitle[_XDEF_WINDOWTITLESTRINGSIZE];
} _XSLIDERCONTROL_STRUCTURE;

class _XHSlider : public _XImageStatic
{
	
public:
	_XButton		m_TrackBarButton;
	POINT			m_TrackBarButtonOffset;
	SIZE			m_TrackBarButtonSize;
	_XImageStatic	m_IndicatorImage;

	int				m_SlidingPosition;
	FLOAT			m_GaugeValue;
	FLOAT			m_PrevGaugeValue;

	int				m_GabSize;
	
	BOOL			m_UseSpinButton;
	int				m_SpinTrackingSize;
	_XButton		m_SpinLeftButton;
	_XButton		m_SpinRightButton;
public:
	_XHSlider();
	virtual ~_XHSlider();

	BOOL Create( _XSLIDERCONTROL_STRUCTURE& sliderwindowstruct );	
	void DestroyWindow( void );
	
	void MoveWindow( int X, int Y );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void ShowWindow( BOOL show );

	BOOL CheckMousePosition( void );
	
	void EnableWindow( BOOL enable )
	{ 
		m_TrackBarButton.EnableWindow( enable );
//		m_IndicatorImage.EnableWindow( enable );
//		_XImageStatic::EnableWindow( enable );
		
		if( !enable )
		{
			if( g_CurrentFocusedObject == this )
			{
				g_CurrentFocusedObject = NULL;
			}
		}
	}

	void SetUseSpinButton( BOOL use, int spintrackingsize = 1 )
	{
		m_UseSpinButton = use;
		m_SpinTrackingSize = spintrackingsize;
	}

	void CreateSpinButton( SIZE btnsize, _XTextureManager* pTextureArchive, 
		                   int leftbtntextureindex, int rightbtntextureindex, 
						   RECT* leftbuttonrect, RECT* rightbuttonrect );

	void SetGaugeValue( FLOAT gaugefactor )
	{
		if( gaugefactor < 0.0f   ) gaugefactor = 0.0f;
		if( gaugefactor > 100.0f ) gaugefactor = 100.0f;

		m_GaugeValue = gaugefactor;
		m_SlidingPosition = (FLOAT)(m_WindowSize.cx) * (m_GaugeValue/100.0f);

		m_TrackBarButton.MoveWindow( m_WindowPosition.x + m_TrackBarButtonOffset.x + m_SlidingPosition, m_TrackBarButton.GetWindowPos().y );		
		
		m_IndicatorImage.SetDrawSize( m_IndicatorImage.m_ClipRect.left, m_IndicatorImage.m_ClipRect.top, 
									  m_IndicatorImage.m_ClipRect.left + m_SlidingPosition, m_IndicatorImage.m_ClipRect.bottom );
		
		if( m_PrevGaugeValue != m_GaugeValue )
		{
			if( m_CommandID )
			{						
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CommandID, 0), *((LPARAM*)&m_GaugeValue) );
			}
			
			m_PrevGaugeValue = m_GaugeValue;			
		}
	}

	void SetSlidingPosition( int pos )
	{
		if( pos < 0 ) pos = 0;
		if( pos < m_WindowSize.cx-m_TrackBarButtonSize.cx ) m_SlidingPosition = pos;
		else m_SlidingPosition = m_WindowSize.cx-m_TrackBarButtonSize.cx;

		if( m_GabSize > 0 )
		{
			m_SlidingPosition = m_SlidingPosition - (m_SlidingPosition % m_GabSize);
		}

		m_TrackBarButton.MoveWindow( m_WindowPosition.x + m_TrackBarButtonOffset.x + m_SlidingPosition, m_TrackBarButton.GetWindowPos().y );		
		
		m_IndicatorImage.SetDrawSize( m_IndicatorImage.m_ClipRect.left, m_IndicatorImage.m_ClipRect.top, 
									  m_IndicatorImage.m_ClipRect.left + m_SlidingPosition,m_IndicatorImage.m_ClipRect.bottom );		

		m_GaugeValue = ((FLOAT)m_SlidingPosition / (FLOAT)(m_WindowSize.cx)) * 100.0f;

		if( m_PrevGaugeValue != m_GaugeValue )
		{
			if( m_CommandID )
			{						
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CommandID, 0), *((LPARAM*)&m_GaugeValue) );
			}
			
			m_PrevGaugeValue = m_GaugeValue;			
		}
	}
	
	void SetScale(void);
	
	void SetSlidingPosition2(int pos)
	{
		m_SlidingPosition = pos;

		if( m_GabSize > 0 )
		{
			m_SlidingPosition = m_SlidingPosition - (m_SlidingPosition % m_GabSize);
		}

		m_TrackBarButton.MoveWindow( m_WindowPosition.x + m_TrackBarButtonOffset.x + m_SlidingPosition, m_TrackBarButton.GetWindowPos().y );		
		
		m_IndicatorImage.SetDrawSize( m_IndicatorImage.m_ClipRect.left, m_IndicatorImage.m_ClipRect.top, 
									  m_IndicatorImage.m_ClipRect.left + m_SlidingPosition,m_IndicatorImage.m_ClipRect.bottom );

		m_GaugeValue = ((FLOAT)m_SlidingPosition / (FLOAT)(m_WindowSize.cx)) * 100.0f;

		if( m_PrevGaugeValue != m_GaugeValue )
		{
			if( m_CommandID )
			{						
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CommandID, 0), *((LPARAM*)&m_GaugeValue) );
			}
			
			m_PrevGaugeValue = m_GaugeValue;			
		}
	}	

	void SetGabSize( int size ){ m_GabSize = size; }
};

#endif // !defined _XHSLIDER_H_
