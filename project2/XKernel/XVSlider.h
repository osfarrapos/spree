// XVSlider.h: interface for the _XVSlider class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XVSLIDER_H_
#define _XVSLIDER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XImageStatic.h"
#include "XHSlider.h"

class _XVSlider : public _XImageStatic  
{
public:
	_XButton		m_TrackBarButton;
	POINT			m_TrackBarButtonOffset;
	SIZE			m_TrackBarButtonSize;
	_XImageStatic	m_IndicatorImage;
	
	int				m_SlidingPosition;
	FLOAT			m_GaugeValue;

	int				m_GabSize;

	BOOL			m_InverseIndicate;
	
public:
	_XVSlider();
	virtual ~_XVSlider();
	
	BOOL Create( _XSLIDERCONTROL_STRUCTURE& sliderwindowstruct );
	void DestroyWindow( void );
	
	void MoveWindow( int X, int Y );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL CheckMousePosition( void );

	void EnableWindow( BOOL enable )
	{ 
		m_TrackBarButton.EnableWindow( enable );
		m_IndicatorImage.EnableWindow( enable );
		_XImageStatic::EnableWindow( enable );
	}
	
	void SetSlidingPosition( int pos )
	{
		if( pos < 0 ) pos = 0;		
		if( pos < m_WindowSize.cy-m_TrackBarButtonSize.cy ) m_SlidingPosition = pos;
		else m_SlidingPosition = m_WindowSize.cy-m_TrackBarButtonSize.cy;

		m_TrackBarButton.MoveWindow( m_TrackBarButton.GetWindowPos().x, m_WindowPosition.y + m_TrackBarButtonOffset.y + m_SlidingPosition );

		if(  m_InverseIndicate )
		{
			m_GaugeValue = 100.0f - ((FLOAT)m_SlidingPosition / (FLOAT)(m_WindowSize.cy-m_TrackBarButtonSize.cy)) * 100.0f;
			
			m_IndicatorImage.SetDrawSize( m_IndicatorImage.m_ClipRect.left, m_IndicatorImage.m_ClipRect.bottom-m_SlidingPosition, 
										  m_IndicatorImage.m_ClipRect.right,m_IndicatorImage.m_ClipRect.bottom );

			m_IndicatorImage.MoveWindow( m_IndicatorImage.GetWindowPos().x, m_WindowPosition.y + m_TrackBarButtonOffset.y + m_SlidingPosition );
		}
		else
		{
			m_IndicatorImage.SetDrawSize( m_IndicatorImage.m_ClipRect.left, m_IndicatorImage.m_ClipRect.top, 
										  m_IndicatorImage.m_ClipRect.right,
										  m_IndicatorImage.m_ClipRect.top + m_SlidingPosition + m_TrackBarButtonOffset.y );

			m_GaugeValue = ((FLOAT)m_SlidingPosition / (FLOAT)(m_WindowSize.cy-m_TrackBarButtonSize.cy)) * 100.0f;
		}
	}

	void SetInverseIndicate( BOOL inverse )
	{
		m_InverseIndicate = inverse;
		m_IndicatorImage.SetDrawSize( m_IndicatorImage.m_ClipRect.left, m_IndicatorImage.m_ClipRect.bottom-m_SlidingPosition, 
										  m_IndicatorImage.m_ClipRect.right,m_IndicatorImage.m_ClipRect.bottom );
		
		m_IndicatorImage.MoveWindow( m_IndicatorImage.GetWindowPos().x, m_WindowPosition.y + m_TrackBarButtonOffset.y + m_SlidingPosition );
	}

	void SetGabSize( int size ){ m_GabSize = size; }
};

#endif // !defined( _XVSLIDER_H_)
