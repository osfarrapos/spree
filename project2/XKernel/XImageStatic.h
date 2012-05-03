// XImageStatic.h: interface for the _XImageStatic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XIMAGESTATIC_H_
#define _XIMAGESTATIC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XItem.h"
#include "XTextureManager.h"
#include "XGUIControlCenter.h"

class _XImageStatic : public _XGUIObject  
{
public:
	RECT					m_WindowRect;
	int						m_TextureIndex;
	_XTextureManager*		m_pTextureArchive;
	_XGUITooltipCallback	m_TooltipCallbackFunc;
	
	D3DXVECTOR2				m_Scale;
	D3DXVECTOR2				m_Position;
	D3DXVECTOR2				m_Center;
	FLOAT					m_RotateAngle;	// radian...
	RECT					m_ClipRect;
	D3DCOLOR				m_FColor;

public:
	_XImageStatic();
	virtual ~_XImageStatic();

	HRESULT Create( int left, int top, int right, int bottom, _XTextureManager* ptexturemanager, int textureindex );		
	void MoveWindow( int X, int Y );
	void MoveWindow( int left, int top, int right, int bottom );

	void SetTexture( _XTextureManager* pTextureArchive, int index )
	{
		m_pTextureArchive = pTextureArchive;
		m_TextureIndex	  = index;
	}

	void DestroyWindow( void );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );	
	void Draw( int X, int Y );
	void Draw( int X, int Y, LPDIRECT3DTEXTURE9	pTexture );
	void DrawSolid( int X, int Y );
	
	void DrawWithRegion(int X, int Y, RECT rect);
	void DrawWithRegion( void );	
	void DrawWithRegion( LPDIRECT3DTEXTURE9	pTexture );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void EnableWindow( BOOL enable )
	{ 
		m_Enable = enable; 
		if( !m_Enable )
		{
			if( g_CurrentFocusedObject == this )
			{
				g_CurrentFocusedObject = NULL;
			}
		}
	}
	void ShowWindow( BOOL show )
	{
		if( g_CurrentFocusedObject == (_XGUIObject*)this ) g_CurrentFocusedObject = NULL;
		m_ShowWindow = show; 
	}
	
	BOOL GetShowStatus( void ){ return m_ShowWindow; }
	BOOL CheckMousePosition( void );
	
	BOOL	TestAlphaChannel( int x, int y );	

	void	SetCallBackFunction_ToolTip( _XGUITooltipCallback pcallbackfunction )
	{
		m_TooltipCallbackFunc = pcallbackfunction;
	}

	void  SetWindowSize( SIZE size );
	void  SetWindowSize( int width, int height );

	void	DrawToolTip(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void	SetScale( FLOAT xscale, FLOAT yscale );
	void	SetCenter( FLOAT x, FLOAT y )
	{
			m_Center.x = x;
			m_Center.y = y;
	}
	void	SetDrawSize( int X, int Y, int width, int height )
	{
			m_ClipRect.left   = X;
			m_ClipRect.top    = Y;
			m_ClipRect.right  = width;
			m_ClipRect.bottom = height;
	}
	void	SetRotateAngle( FLOAT angle )
	{
			m_RotateAngle = angle;	
	}
	void    SetClipRect( RECT cliprect )
	{
			m_ClipRect = cliprect;
	}
	void    SetClipRect( int left, int top, int right, int bottom )
	{
			m_ClipRect.left   = left;
			m_ClipRect.top    = top;
			m_ClipRect.right  = right;
			m_ClipRect.bottom = bottom;
	}
	void	GetClipRect(RECT& cliprect)
	{
			cliprect	= m_ClipRect;
	}
};

#endif // !defined(_XIMAGESTATIC_H_)
