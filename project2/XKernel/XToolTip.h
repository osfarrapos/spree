// XToolTip.h: interface for the _XToolTip class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XTOOLTIP_H_
#define _XTOOLTIP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XImageStatic.h"
#include "XGUIControlCenter.h"

typedef struct
{	
	POINT		position;
	SIZE        windowsize;	
	POINT		textoffset;
	int         imageindex_border;	
	_XTextureManager*	texturearchive;
} _XTOOLTIP_STRUCTURE;


class _XToolTip : public _XGUIObject  
{
private:
	_XImageStatic	m_Border;
	POINT			m_TextOffset;
	TCHAR			m_TextString[512];
	D3DCOLOR		m_TextColor;
	DWORD			m_AlphaLevel;
	BOOL			m_TextBold;
	BOOL			m_CenterAlign;
	BOOL			m_CheckClipping;
	POINT			m_ToolTipPosition;
public:
	_XToolTip();
	virtual ~_XToolTip();

	virtual BOOL	Create( _XTOOLTIP_STRUCTURE tooltipstructure );

	virtual void	DestroyWindow( void );					
					
	virtual void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );	
	virtual void	DrawAlpha( void );	
			void	DrawText( void );	
			void	DrawParty( void );
			
#ifdef _XTS_PK
			void	DrawNoEdge(int bordercolor);
#else
			void	DrawNoEdge(BOOL bEnemy = FALSE);
#endif

	virtual BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	virtual void	EnableWindow( BOOL enable )
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
	void			MoveWindow( int X, int Y );

	void			SetToolTipString( int x, int y, LPTSTR message, D3DCOLOR textcolor, BOOL centeralign = FALSE, BOOL checkclipping = TRUE );
	void			SetToolTipStringEx( int x, int y, LPTSTR message, D3DCOLOR textcolor, int height, BOOL centeralign = FALSE, BOOL checkclipping = TRUE  );
	void			SetToolTipStringEx_Noresize( int x, int y, LPTSTR message, D3DCOLOR textcolor );
	void			SetTextOffSet( int x, int y ){  m_TextOffset.x = x; m_TextOffset.y = y; }

	void			SetTooltipAlphaLevel( DWORD alphalevel );
	
	BOOL			CheckMousePosition( void );
	POINT			GetWindowPos( void )
	{ 
					if( m_Border.m_TextureIndex == -1 )
					{
						return m_WindowPosition;
					}
					else
					{
						POINT pt = { m_Border.m_Position.x, m_Border.m_Position.y };
						return pt; 
					}
	}
	BOOL			IsButtonEnabled( void ){ return m_Enable; }		

	DWORD			GetCommandID( void ){ return m_CommandID; }

	void			SetTextColor( D3DCOLOR color )
	{
					m_TextColor = color;	
	}
	void			SetClipRect( int left, int top, int right, int bottom )
	{
					m_Border.SetClipRect( left, top, right, bottom );
	}
	void			SetTextBold( BOOL enable )
	{ 
					m_TextBold = enable;
	}
	
	void			NotifyUIChange( void );
};

#endif // !defined(_XTOOLTIP_H_)
