// XCheckButton.h: interface for the _XCheckButton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XCHECKBUTTON_H_
#define _XCHECKBUTTON_H_

#include "XStatic.h"
#include "XImageStatic.h"
#include "XFont.h"
#include "XGUIControlCenter.h"

typedef enum _XCheckButtonImageClass
{
	_XCHECKBUTTONIMAGE_NORMAL = 0,
	_XCHECKBUTTONIMAGE_CLICK,	
};

typedef struct
{
	BOOL        enable;
	POINT		position;
	SIZE        windowsize;
	DWORD		commandid;
	int         imageindex_normal;
	int         imageindex_checked;	
	_XTextureManager*	texturearchive;
} _XCHECKBTN_STRUCTURE;

class _XCheckButton : public _XGUIObject  
{
protected:
	BOOL					m_Checked;
	BOOL					m_SwapMode;
	_XImageStatic			m_btnimage_normal;
	_XImageStatic			m_btnimage_checked;

	LPTSTR					m_ButtonText;
	int						m_ButtonTextCategory;
	int						m_ButtonTextID;

	POINT					m_TextPosition;
	D3DCOLOR				m_TextColor;
	D3DCOLOR				m_TextHightlightColor;	
	D3DCOLOR				m_TextMouseOverColor;	

	_XGUITooltipCallback	m_TooltipCallbackFunc;

	_XFONTID				m_FontID;
	BOOL					m_RenderFontEdge;
	D3DCOLOR				m_FontEdgeColor;

	BOOL					m_ButtonTextClipping;
	
public:
	_XCheckButton();
	virtual ~_XCheckButton();

	virtual BOOL	Create( _XCHECKBTN_STRUCTURE& checkbtnstruct );	
	virtual void	DestroyWindow( void );
	virtual void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );	
	virtual BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );	

	virtual void	EnableWindow( BOOL enable )
	{				
					m_Enable = enable;
					m_btnimage_normal.EnableWindow( m_Enable );
					m_btnimage_checked.EnableWindow( m_Enable );					
					
					if( !enable )
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

					if( !show )
					{
						if( g_CurrentFocusedObject == this )
						{
							g_CurrentFocusedObject = NULL;
						}
					}
	}

	void			MoveWindow( int X, int Y );
	BOOL			CheckMousePosition( void );
	POINT			GetWindowPos( void ){ return m_WindowPosition; }
	BOOL			IsButtonEnabled( void ){ return m_Enable; }
	DWORD			GetCommandID( void ){ return m_CommandID; }
	void			SetCommandID( DWORD dwID ) { m_CommandID = dwID; } //Author : ¾çÈñ¿Õ //breif : Ãß°¡
	void			SetCallBackFunction_ToolTip( _XGUITooltipCallback pcallbackfunction )
	{
					m_TooltipCallbackFunc = pcallbackfunction;
	}
	void			SetButtonImage( int normalindex, int checkedindex, _XTextureManager* texturearchive = NULL  );

	void			SetCheck( BOOL check ){ m_Checked = check; }
	BOOL			GetCheck( void ){ return m_Checked; }

	void			SetButtonImageClipRect( _XCheckButtonImageClass buttontype, int left, int top, int right, int bottom );

	void			SetButtonSwapMode( BOOL swap ){ m_SwapMode = swap; }
	void			AttachXFont( _XFONTID fontid ){ m_FontID = fontid; }	
	
	int				GetButtonTextID( void ) { return m_ButtonTextID; }
	void			SetButtonTextID( int buttontextcategory, int buttontextid );
	LPTSTR			GetButtonText( void );
	void			SetButtonText( LPTSTR buttontext );
	void			SetButtonTextColor( D3DCOLOR color, D3DCOLOR highlightcolor = _XSC_DEFAULT_HIGHLIGHT, D3DCOLOR mouseovercolor = 0xFFB2B2B2 )
	{ 
					m_TextColor = color; 
					m_TextHightlightColor = highlightcolor;
					m_TextMouseOverColor = mouseovercolor;
	}
	void			DrawButtonText( BOOL disabled = FALSE );	

	void			SetGrowTextMode( BOOL use, D3DCOLOR	fontedgecolor = 0xFF000000 )
	{
					m_RenderFontEdge = use;
					m_FontEdgeColor	 = fontedgecolor;
	}
	
	void			NotifyUIChange( void );
	
	void			SetButtonTextClipping( BOOL enable )
	{
					m_ButtonTextClipping = enable;
	}
};

#endif // !defined(_XCHECKBUTTON_H_)
