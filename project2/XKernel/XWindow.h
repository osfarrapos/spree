#ifndef _XWINDOW_
#define _XWINDOW_

#include "XStatic.h"
#include "XImageStatic.h"
#include "XButton.h"
#include "XGUIControlCenter.h"
#include "XWindowManager.h"

typedef enum {
	_XWMOVE_FIXED = 0,
	_XWMOVE_FREE,
	_XWMOVE_HORIZ,
	_XWMOVE_VERT
} _XWINDOW_MOVEMODE;

typedef enum {
	_XWRESIZE_NONE = 0,
	_XWRESIZE_LEFTTOP,
	_XWRESIZE_RIGHTTOP,
	_XWRESIZE_LEFTBOTTOM,
	_XWRESIZE_RIGHTBOTTOM,
	_XWRESIZE_LEFT,
	_XWRESIZE_TOP,
	_XWRESIZE_RIGHT,	
	_XWRESIZE_BOTTOM	
} _XWINDOW_RESIZEMODE;

typedef enum
{ 
	_XW_DOCKINGTYPE_NONE = 0, 
	_XW_DOCKINGTYPE_LEFT, 
	_XW_DOCKINGTYPE_RIGHT, 
	_XW_DOCKINGTYPE_BOTTOM
} _XWINDOW_DOCKINGTYPE ;


typedef enum
{ 
	_XW_ANIMTYPE_NONE = 0, 
	_XW_ANIMTYPE_OPEN,
	_XW_ANIMTYPE_CLOSE	
} _XWINDOW_ANIMTYPE ;

typedef enum
{
	_XW_STATE_NONE = 0,
		_XW_STATE_LEFTCLICK,
		_XW_STATE_RIGHTCLICK,
		_XW_STATE_DBLCLICK
} _XWINDOW_MOUSE_STATE;

typedef struct
{
	BOOL				enable;
	POINT				position;
	SIZE				windowsize;
	DWORD				commandid;
	int					windowtextureindex;
	_XTextureManager*	texturearchive;
	D3DCOLOR			facecolor;
	DWORD				windowid;
	//TCHAR				WindowTitle[_XDEF_WINDOWTITLESTRINGSIZE];
} _XWINDOW_STRUCTURE;

class _XWindow : public _XGUIObject 
{		
	public	  :
		static	BOOL		m_GlobalPlaySoundEffect;

	protected :	
		int					m_WindowID;
		_XImageStatic		m_borderstatic;
		_XStatic			m_solidborderstatic;
		BOOL				m_SolidBorderMode;
		_XWINDOW_ANIMTYPE	m_WindowAnimationType;
		int					m_WindowAnimationLevel;
		BOOL				m_UseWindowEffect;
		BOOL				m_DrawBorder;
		BOOL				m_PlaySoundEffect;		
		BOOL				m_Dragging;
		POINT				m_DragStartPos;
		_XWINDOW_MOVEMODE	m_WindowMoveMode;
		_XWINDOW_DOCKINGTYPE m_WindowDockingType;
		_XWindow*			m_DockedParentWindow;
		_XWindow*			m_DockedChildWindow;
		BOOL				m_DockableWindow;
		BOOL				m_EdgeDockableWindow;
		int					m_DockingGroup;

		BOOL				m_SelfDestroy;

		BOOL				m_bEscapeWindow;//2004.06.24->oneway48 insert
		BOOL				m_bEnableTopWindow;

		_XWINDOW_MOUSE_STATE	m_WindowMouseState;

		_XList				m_ObjectList;
		TCHAR				m_WindowTitle[_XDEF_WINDOWTITLESTRINGSIZE];
				
		_XWindowManager*	m_pMyManager;
	public :
		_XWindow();
		~_XWindow();
		
		virtual BOOL		 Create( _XWINDOW_STRUCTURE& windowstruct );

		virtual void		 DestroyWindow( void );					
		virtual void		 SaveWindowProperty( void ){};
						
		virtual void		 Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
		virtual BOOL		 Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
		virtual BOOL		 ProcessWheelMessage( short zDelta );
		virtual BOOL		 OnKeyDown(WPARAM wparam, LPARAM lparam){ return FALSE; }

		virtual void		 SetDefaultPosition( void ){}
		virtual void		 EnableWindow( BOOL enable );		
		virtual void		 ShowWindow( BOOL show );		
		virtual void		 MoveWindow( int X, int Y );		
		virtual void		 WindowPosChanging( int& X, int& Y );
		virtual void		 CheckOutside( void );
		virtual void		 SetWindowMoveMode( _XWINDOW_MOVEMODE movemode ){ m_WindowMoveMode = movemode; }

		virtual void		 SetPlaySoundEfect( BOOL enable )
		{
							 m_PlaySoundEffect	= enable;
		}

		virtual void		 SetMyManager( _XWindowManager* pManager )
		{
							 m_pMyManager = pManager;
		}

		virtual void		 SetBorderImage( _XTextureManager* texturemanager, int index )
		{
							 if( !m_SolidBorderMode )
								  m_borderstatic.SetTexture( texturemanager, index );
							 else
								  m_solidborderstatic.SetTexture( texturemanager, index );
							 
		}

		virtual void		 SetUseBorderImage( BOOL use )
		{
							 m_SolidBorderMode = !use;
		}

		_XImageStatic*		 GetBorderImageStatic( void ){ return &m_borderstatic; }
		_XStatic*			 GetBorderStatic( void ){ return &m_solidborderstatic; }
		_XWINDOW_MOUSE_STATE GetWindowMouseState( void ){ return m_WindowMouseState; }
		
		virtual void		 SetClippingArea( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom );
		virtual void		 DisableClippingArea( void );
		
		virtual BOOL		 CheckMousePosition( void );
		virtual DWORD		 GetWindowID( void ){ return m_WindowID; }
		virtual void		 SetWindowID( DWORD id ){ m_WindowID = id; }
		virtual POINT		 GetWindowPos( void ){ return m_WindowPosition; }
		virtual SIZE		 GetWindowSize( void ){ return m_WindowSize;	}
		virtual _XWindow*	 GetNextXWindow( void ){ return (_XWindow*)this->right; }
		virtual BOOL		 GetShowStatus( void ){ return m_ShowWindow;	}
		virtual BOOL		 GetSelfDestroyWindow( void ){ return m_SelfDestroy; }
		virtual BOOL		 IsButtonEnabled( void ){ return m_Enable; }
		virtual BOOL		 IsDragging( void ){ return m_Dragging; }
		virtual BOOL		 IsEnableWindow( void ){ return m_Enable; }		

		virtual DWORD		 GetCommandID( void ){ return m_CommandID; }
		virtual LPCTSTR		 GetWindowTitle( void ){ return m_WindowTitle; }
		
		virtual _XGUIObject* InsertChildObject( _XGUIObject* xguiobject );

		virtual _XWINDOW_DOCKINGTYPE GetDockedType( void ){ return m_WindowDockingType; }
		
		virtual void		 SetDocking( BOOL docking, int dockinggroup = 0 )
		{					             m_DockableWindow = docking;
										 m_DockingGroup   = dockinggroup;
		}		
		virtual void		 SetDockingGroup( int dockinggroup ){ m_DockingGroup = dockinggroup; }
		virtual void		 SetEdgeDocking( BOOL edgedocking ){ m_EdgeDockableWindow = edgedocking; }		
		virtual void		 SetChildWindow ( _XWindow* pChildWindow  );	
		virtual int			 GetDockingGroup( void ){ return m_DockingGroup; }
		virtual void		 SetDrawBorder( BOOL draw ){ m_DrawBorder = draw; }
		virtual void		 SetBorderColorFactor( D3DCOLOR factor ); // hotblood zerolevel

		virtual void		 EnableWindowEffect( BOOL effect ){ m_UseWindowEffect = effect; }
		virtual void		 DrawWindowEffect( void );	

		virtual void		 RebuildDockedWindow( void );
		virtual void		 SetEscapeWindow(BOOL bEscape){ m_bEscapeWindow = bEscape; }//2004.06.24->oneway48 insert
		virtual BOOL		 GetEscapeWindow( void ){ return m_bEscapeWindow; }		

		virtual BOOL		 GetEnableTopWindow( void ){ return m_bEnableTopWindow; }
		virtual void		 SetEnableTopWindow( BOOL enable = TRUE ){ m_bEnableTopWindow = enable; }
		
		virtual void		 NotifyUIChange( void );
};



#endif


