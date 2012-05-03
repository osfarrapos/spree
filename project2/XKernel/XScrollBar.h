// XScrollBar.h: interface for the _XScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XSCROLLBAR_H_
#define _XSCROLLBAR_H_

#include "XStatic.h"
#include "XImageStatic.h"
#include "XGUIControlCenter.h"

typedef struct{
	BOOL        enable;
	POINT		position;
	int			width;
	int         height;
	int			btnysize;
	int			trackbarysize;
	int         totallinecount;
	int         viewlinecount;
	DWORD		commandid;

	int         imageindex_uparrow;
	int         imageindex_uparrowclick;
	int         imageindex_downarrow;	
	int         imageindex_downarrowclick;	
	int         imageindex_trackbar;	
	_XTextureManager*	texturearchive;
} _XSCRBAR_STRUCTURE;

typedef struct 
{
	RECT topnormalrect;
	RECT topclickrect;
	RECT bottomnormalrect;
	RECT bottomclickrect;
	RECT trackbarrect;
} _XSCRBAR_IMAGECLIPRECTSTRUCTURE;

class _XVScrollBar  : public _XGUIObject
{
private:
	
	BOOL			m_Enable;
	BOOL			m_Focus;
		
	_XImageStatic	m_topnormalbutton;
	_XImageStatic	m_topclickbutton;

	_XImageStatic	m_bottomnormalbutton;
	_XImageStatic	m_bottomclickbutton;

	_XImageStatic	m_TrackbarImage;
	
	int				m_Height;
	int				m_Width;
	int				m_Totallinecount;
	int				m_Viewlinecount;
	int				m_Scrollpos;
	int				m_LineScrollpos;
	int				m_Barsize;
	int				m_Barposition;
	int				m_Btnsize;
	int				m_PageLinecount;
	float			m_ScrollRate;
	POINT			m_WindowPosition;

	BOOL			m_Trackbar_Dragging;
	BOOL			m_Border;
	BOOL			m_ViewNullBorder;

	int				m_DragStartPos;

	int				m_ScrollLevel;
	
public:
	_XVScrollBar();
	virtual ~_XVScrollBar();

	BOOL  Create( _XSCRBAR_STRUCTURE scrbarstructure );
	void  DestroyWindow( void );
				
	void  Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL  Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );	
	BOOL  ProcessWheelMessage( short zDelta );

	void  EnableWindow( BOOL enable )
	{ 
		  m_Enable = enable; 

		  m_topnormalbutton.EnableWindow( m_Enable );
		  m_topclickbutton.EnableWindow( m_Enable );
		  m_bottomnormalbutton.EnableWindow( m_Enable );
		  m_bottomclickbutton.EnableWindow( m_Enable );
		  m_TrackbarImage.EnableWindow( m_Enable );		  
		  
		  if( !enable )
		  {
			  if( g_CurrentFocusedObject == this )
			  {
				  g_CurrentFocusedObject = NULL;
			  }
		  }
	}
	void  ShowWindow( BOOL show )
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
	void  MoveWindow( int X, int Y );
	BOOL  CheckMousePosition( void );	
	BOOL  IsWindowEnabled( void ){ return m_Enable; }

	void  ResetPosition( void );
		
	void  UpdateData( void );

	POINT GetWindowPos( void ){ return m_WindowPosition; }		
	void  SetWidowSize( int height ){ m_Height = height; UpdateData(); }	
	
	void  SetClipRect( _XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure );

	int   GetWindowWidth( void ){ return m_Width;	}
	int   GetWindowHeight( void ){ return m_Height;	}
	void  SetWindowHeight( int height ){ m_Height = height; UpdateData(); }
	
	int   GetScrollPos( void ){ return m_Scrollpos; }	
	void  SetScrollPos( int scrollpos ){ m_Scrollpos = scrollpos; }

	void  SetLineScrollPos( int linescrollpos )
	{		
		  if( m_ScrollRate == 0.f ) return;

		  m_LineScrollpos = linescrollpos;
		  if( m_LineScrollpos < 0 ) m_LineScrollpos = 0;
		  if( m_LineScrollpos  >= (m_Totallinecount - m_Viewlinecount) ) 	
		      m_LineScrollpos  =  (m_Totallinecount - m_Viewlinecount);	

		  if( m_LineScrollpos == (m_Totallinecount - m_Viewlinecount) )
		      m_Scrollpos =  m_Height-(m_Btnsize<<1)-m_Barsize;
		  else m_Scrollpos = (int)((float)m_LineScrollpos * m_ScrollRate);			

		  RebuildTrackBar();
	}

	int   GetLineScrollPos( void ){ return m_LineScrollpos; }

	int   GetTotalLineCount( void ){ return m_Totallinecount; }	
	void  SetTotalLineCount( int linecount ){ m_Totallinecount = linecount; UpdateData(); }

	int   GetViewLineCount( void ){ return m_Viewlinecount; }	
	void  SetViewLineCount( int viewcount ){ m_Viewlinecount = viewcount; UpdateData(); }

	void  SetPagecount( int pagecount ){ m_PageLinecount = pagecount; }

	void  SetViewBorder( BOOL show ){ m_Border = show; }
	void  SetBorderImage( LPSTR imagename );
	void  SetViewNullBorder( BOOL show ){ m_ViewNullBorder = show; }

	void  LineUp( int line = 1 );
	void  LineDown(  int line = 1  );
	void  SetScrollLevel( int level ){ m_ScrollLevel = level; }

	BOOL  Dragging( void ){ return m_Trackbar_Dragging; }

	void  RebuildTrackBar( void );

};

#endif // !defined(_XSCROLLBAR_H_)
