#ifndef _XBUTTON_
#define _XBUTTON_

#define _BTNMESSAGEMODE_BTNUP		0
#define _BTNMESSAGEMODE_BTNDOWN		1	

#include "XStatic.h"
#include "XImageStatic.h"
#include "XFont.h"
#include "XGUIControlCenter.h"

typedef enum _XButtonImageClass
{
	_XBUTTONIMAGE_NORMAL = 0,
	_XBUTTONIMAGE_CLICK,
	_XBUTTONIMAGE_READY
};

typedef struct
{
	BOOL        enable;
	POINT		position;
	SIZE        windowsize;
	DWORD		commandid;	
	int         imageindex_normal;
	int         imageindex_click;
	int         imageindex_ready;
	_XTextureManager*	texturearchive;
} _XBTN_STRUCTURE;


class _XButton : public _XGUIObject 
{
		
	protected :			
		_XImageStatic			m_btnimage_normal;
		_XImageStatic			m_btnimage_click;
		_XImageStatic			m_btnimage_ready;

		LPTSTR					m_ButtonText;
		int						m_ButtonTextCategory;
		int						m_ButtonTextID;

		POINT					m_TextPosition;
		POINT					m_TextOffset;
	  	D3DCOLOR				m_TextColor;
		D3DCOLOR				m_TextHighlightColor;
		_XFONT_ALIGNTYPE		m_TextAlign;
		
		int  					m_MessageMode;
		
		_XGUITooltipCallback	m_TooltipCallbackFunc;

		_XFONTID				m_FontID;

		BOOL					m_RenderFontEdge;
		D3DCOLOR				m_FontEdgeColor;
		
		BOOL					m_ButtonTextClipping;

	public :
		_XButton();		
		~_XButton();


		virtual BOOL	Create( _XBTN_STRUCTURE& btnstruct );

		virtual void	DestroyWindow( void );					
						
		virtual void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

		virtual BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

		void			setMessageMode( int mode )
		{   
						if( m_MessageMode == _BTNMESSAGEMODE_BTNUP|| m_MessageMode == _BTNMESSAGEMODE_BTNDOWN ) 
							m_MessageMode = mode;
		}

		virtual void	EnableWindow( BOOL enable )
		{ 						
						m_Enable = enable; 
						m_btnimage_normal.EnableWindow( m_Enable );
						m_btnimage_click.EnableWindow( m_Enable );
						m_btnimage_ready.EnableWindow( m_Enable );
						
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
		virtual void    SetWindowSize( int width, int height );
		
		BOOL			CheckMousePosition( void );
		POINT			GetWindowPos( void ){ return m_WindowPosition; }
		BOOL			IsButtonEnabled( void ){ return m_Enable; }		

		DWORD			GetCommandID( void ){ return m_CommandID; }
		void			SetCommandID(DWORD commandid)	{	m_CommandID = commandid;	}
		
		void			SetCallBackFunction_ToolTip( _XGUITooltipCallback pcallbackfunction )
		{
						m_TooltipCallbackFunc = pcallbackfunction;
		}

		void			SetButtonImage( int normalindex, int clickindex, int readyindex, _XTextureManager* texturearchive = NULL  );

		void			SetButtonImageClipRect( _XButtonImageClass buttontype, int left, int top, int right, int bottom );

		void			AttachXFont( _XFONTID fontid ){ m_FontID = fontid; }
		
		LPTSTR			GetButtonText( void );
		void			SetButtonText( LPTSTR buttontext, _XFONT_ALIGNTYPE align = _XFONT_ALIGNTYPE_CENTER, int xoffset = 0, int yoffset = 0 );
		void			SetButtonTextID( int buttontextcategory, int buttontextid, _XFONT_ALIGNTYPE align = _XFONT_ALIGNTYPE_CENTER, int xoffset = 0, int yoffset = 0 );
		void			SetButtonTextColor( D3DCOLOR color ){ m_TextColor = color; }
		void			SetButtonTextHighlightColor( D3DCOLOR highlightcolor ){ m_TextHighlightColor = highlightcolor; }
		void			DrawButtonText( BOOL disabled = FALSE, BOOL focused = FALSE );

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


#endif


