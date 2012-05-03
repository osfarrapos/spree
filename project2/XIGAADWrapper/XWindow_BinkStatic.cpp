// XWindow_BinkStatic.cpp: implementation of the _XWindow_BinkStatic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_BinkStatic.h"

#ifdef _ACCLAIM_IGAADSYSTEM

static U32 Bink_surface_type [ RAD3DSURFACECOUNT ];

static void Setup_surface_array( void )
{
  Bink_surface_type[ RAD3DSURFACE24 ] = BINKSURFACE24;
  Bink_surface_type[ RAD3DSURFACE24R ] = BINKSURFACE24R;
  Bink_surface_type[ RAD3DSURFACE32 ] = BINKSURFACE32;
  Bink_surface_type[ RAD3DSURFACE32R ] = BINKSURFACE32R;
  Bink_surface_type[ RAD3DSURFACE32A ] = BINKSURFACE32A;
  Bink_surface_type[ RAD3DSURFACE32RA ] = BINKSURFACE32RA;
  Bink_surface_type[ RAD3DSURFACE555 ] = BINKSURFACE555;
  Bink_surface_type[ RAD3DSURFACE565 ] = BINKSURFACE565;
  Bink_surface_type[ RAD3DSURFACE5551 ] = BINKSURFACE5551;
  Bink_surface_type[ RAD3DSURFACE4444 ] = BINKSURFACE4444;
}

#define is_pow2( val ) ( Round_up_to_next_2_power( val ) == ( val ) )

static S32 percent_changed( HBINK bink )
{
  U32 all;
  U32 changed;
  S32 i, num;

  all = bink->Width * bink->Height;

  changed = 0;

  num = BinkGetRects( bink, BINKSURFACEFAST );

  for( i = 0 ; i < num ; i++ )
  {
    changed += bink->FrameRects[ i ].Width * bink->FrameRects[ i ].Height;
  }

  return( ( changed * 100 ) / all );
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


_XWindow_BinkStatic::_XWindow_BinkStatic()
{
	m_BinkObject			= 0;
	m_maximum_texture_size	= 256;	
	m_RadImage				= 0;
	memset( m_szBinkObjectFilename, 0, sizeof(TCHAR) * _MAX_PATH );

	m_AlphaLevel			= 1.0f;
	m_PlayFlag				= FALSE;
	m_EndOfFrame			= FALSE;

	m_ViewPortWidth			= 0;
	m_ViewPortHeight		= 0;
}

_XWindow_BinkStatic::~_XWindow_BinkStatic()
{

}


BOOL _XWindow_BinkStatic::Initailize(void)
{
	m_ViewPortWidth  = 0;
	m_ViewPortHeight = 0;
	
	int partyresourceindex = g_MainInterfaceTextureArchive.FindResource( "mi_party.tga" );
	
	m_DownloadMessageWindow.Create( 0, 0, 146, 36,
									&g_MainInterfaceTextureArchive, 
									partyresourceindex );
	m_DownloadMessageWindow.SetClipRect( 0, 144, 146, 179 );

	m_DownloadMessageIndicator.Create( 0, 0, 24, 17,
									   &g_MainInterfaceTextureArchive, 
									   partyresourceindex );
	m_DownloadMessageIndicator.SetClipRect( 168, 17, 192, 34 );


	m_IndicatorDirection = TRUE;
	m_IndicatorIndex = 0;	

	return TRUE;
}

void _XWindow_BinkStatic::DestroyWindow(void)
{
	BinkFinalize();	
	_XWindow::DestroyWindow();
}

void _XWindow_BinkStatic::Draw(_XGUIObject*& pfocusobject)
{
	//_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
	{
		return;
	}

	if( gApp->m_bHasFocus && m_BinkObject )
	{
		FLOAT fAspectRatio;
		fAspectRatio = (FLOAT)m_ViewPortHeight/(FLOAT)m_BinkObject->Height; 
		if( (FLOAT)m_BinkObject->Width * fAspectRatio > (FLOAT)m_ViewPortWidth )
		{
			fAspectRatio = (FLOAT)m_ViewPortWidth/(FLOAT)m_BinkObject->Width;
		}

		int displaywidth  = (int)((FLOAT)m_BinkObject->Width*fAspectRatio );
		int displayheight = (int)((FLOAT)m_BinkObject->Height*fAspectRatio );
		
		FLOAT xscale = (FLOAT)displaywidth  / (FLOAT)m_BinkObject->Width;
		FLOAT yscale = (FLOAT)displayheight / (FLOAT)m_BinkObject->Height;
		
		if( m_AlphaLevel < 0.0f )
			m_AlphaLevel = 0.0f;

		_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x + m_ViewPortWidth, m_WindowPosition.y + m_ViewPortHeight, 
						D3DCOLOR_ARGB( (int)(255.0f * m_AlphaLevel), 0, 0, 0 ) );


		Blit_RAD_3D_image( m_RadImage, m_WindowPosition.x + (m_ViewPortWidth>>1)  - (displaywidth>>1), 
									   m_WindowPosition.y + (m_ViewPortHeight>>1) - (displayheight>>1), xscale, yscale, m_AlphaLevel );

		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
	}
	else
	{
		int centerx = m_WindowPosition.x + (m_ViewPortWidth>>1);
		int centery = m_WindowPosition.y + (m_ViewPortHeight>>1);

		int messageboxxpos = centerx - (146>>1);
		int messageboxypos = centery - (36>>1);
		
		m_DownloadMessageWindow.Draw( messageboxxpos, messageboxypos );

		if(m_IndicatorDirection) // forward
		{
			if( (gnFPS>>1) < gnFrameCounter )
			{
				if( m_IndicatorIndex == 2 )
				{
					m_IndicatorDirection = FALSE;
					m_IndicatorIndex=1;
				}
				else
				{
					m_IndicatorIndex++;
				}
			}
		}
		else // backward
		{
			if( (gnFPS>>1) )
			{
				if( m_IndicatorIndex == 0 )
				{
					m_IndicatorDirection = TRUE;
					m_IndicatorIndex=1;
				}
				else
				{
					m_IndicatorIndex--;
				}
			}
		}

		m_DownloadMessageIndicator.SetClipRect( 168 + (24*m_IndicatorIndex), 17, 192 + (24*m_IndicatorIndex), 34 );
		m_DownloadMessageIndicator.Draw( messageboxxpos+3, messageboxypos );
		_XDrawSolidBar( messageboxxpos + 8, messageboxypos+18, messageboxxpos+138, messageboxypos+32, 0xFF000000 );

		g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );		
		g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_ViewPortWidth>>1), m_WindowPosition.y + (m_ViewPortHeight>>1), _XFONT_ALIGNTYPE_CENTER, _T("Downloading...") );
		g_XBaseFont->Flush();
	}
}

BOOL _XWindow_BinkStatic::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_ShowWindow)
		return FALSE;

	if(!this->m_Enable)
		return FALSE;

	if( gApp->m_bHasFocus )
	{
		if( m_BinkObject && !BinkWait( m_BinkObject ) )
		{
			Decompress_frame( 0, 0 );
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_BinkStatic::ShowWindow(BOOL show)
{
	_XWindow::ShowWindow( show );
}

void _XWindow_BinkStatic::SetWindowSize(int width, int height)
{
	_XWindow::SetWindowSize( width, height );
}


BOOL _XWindow_BinkStatic::BinkInitialize( LPTSTR binkobjectfilename, LPVOID binkobjectmemptr )
{
	BinkFinalize();

	m_PlayFlag	 = FALSE;
	m_EndOfFrame = FALSE;

	BinkSoundUseDirectSound( 0 );
	
	if( binkobjectmemptr )
	{
		m_BinkObject = BinkOpen( (char*)binkobjectmemptr, BINKSNDTRACK | BINKFROMMEMORY | BINKNOSKIP );
	}
	else
	{
		m_BinkObject = BinkOpen( binkobjectfilename, BINKSNDTRACK );
	}

	if( !m_BinkObject )
	{
		_XFatalError( _T("Error opening file : %s"), BinkGetError( ) );
		return FALSE;
	}

	SetWindowSize( m_BinkObject->Width, m_BinkObject->Height );

	m_Rad3D.direct_3d = gpD3D;
	m_Rad3D.direct_3d_device = gpDev;
	m_Rad3D.window = gHWnd;
	m_Rad3D.nonpow2_textures_supported = !!(g_d3dCaps.TextureCaps && D3DPTEXTURECAPS_NONPOW2CONDITIONAL);

	D3DDISPLAYMODE dm;
	gpD3D->GetAdapterDisplayMode( gApp->m_d3dSettings.AdapterOrdinal(), &dm);
	
	if ( !get_texture_formats( gpD3D, dm.Format, &m_Rad3D ) )
	{
		return FALSE;
	}

	Setup_surface_array();

	if ( !Allocate_3D_images( ) )
	{
		_XFatalError( "Bink : Error creating 3D textures." );
		BinkClose( m_BinkObject );
		return FALSE;
	}
	
	if ( m_BinkObject )
		BinkPause( m_BinkObject, TRUE );

	return TRUE;
}

void _XWindow_BinkStatic::BinkFinalize( void )
{
	if ( m_BinkObject )
	{
		BinkPause( m_BinkObject, TRUE );
		BinkClose( m_BinkObject );
		m_BinkObject = NULL;
	}

	if( m_RadImage )
	{
		Close_RAD_3D_image( m_RadImage );
		m_RadImage = NULL;
	}
}

S32 _XWindow_BinkStatic::Allocate_3D_images( void )
{
	HRAD3DIMAGE New_image;

	// Try to open a 3D image for the Bink.
	New_image = Open_RAD_3D_image( &m_Rad3D, m_WindowSize.cx, m_WindowSize.cy, 0, m_Rad3D.nonpow2_textures_supported );
	
    if ( New_image )
	{
		// Yup, it opened! Close the old image (if there was one) and replace it.
		if ( m_RadImage )
		  Close_RAD_3D_image( m_RadImage );

		m_RadImage = New_image;

		//
		// Advance the Bink to fill the texture.
		//

		Decompress_frame( 1, 1 );

		return( 1 );
	}

	//
	// Free the image if one was opened.
	//

	if ( New_image )
		Close_RAD_3D_image( New_image );

	return( 0 );
}

void _XWindow_BinkStatic::Decompress_frame( S32 copy_all, S32 never_skip )
{
  void* pixels;
  U32 pixel_pitch;
  U32 pixel_format;
  U32 src_x, src_y, src_w, src_h;

  //Start_timer( );

  //
  // Decompress the Bink frame.
  //

  BinkDoFrame( m_BinkObject );

  //
  // if we are falling behind, decompress an extra frame to catch up
  //
              
  if ( ( !never_skip ) && ( BinkShouldSkip( m_BinkObject ) ) )
  {
	  if( m_PlayFlag )
		BinkNextFrame( m_BinkObject );
	  
	  BinkDoFrame( m_BinkObject );
  }
 
  //
  // if more than 75% of the image has changed, then tell d3d to discard
  //   the previous pixels and then update everything
  //

  if ( !copy_all )
  {
    if ( percent_changed( m_BinkObject ) >= 75 )
      copy_all = 1;
    }

  //End_and_start_next_timer( Bink_microseconds );

  //
  // Lock the 3D image so that we can copy the decompressed frame into it.
  //

  while ( Lock_RAD_3D_image( m_RadImage, &pixels, &pixel_pitch, &pixel_format,
                             &src_x, &src_y, &src_w, &src_h, copy_all ) )
  {
    //
    // Copy the decompressed frame into the 3D image.
    //

    BinkCopyToBufferRect( m_BinkObject,
                          pixels,
                          pixel_pitch,
                          m_BinkObject->Height,
                          0,0,
                          src_x, src_y, src_w, src_h,
                          Bink_surface_type[ pixel_format ] | BINKNOSKIP |
                          ( ( copy_all ) ? BINKCOPYALL : 0 ) );

    //
    // Unlock the 3D image.
    //

    Unlock_RAD_3D_image( m_RadImage );
  }

  //End_and_start_next_timer( Render_microseconds );

  //
  // Keep playing the movie.
  //

   if ( m_BinkObject->FrameNum >= m_BinkObject->Frames )  // Break out of loop before BinkNextFrame if at the end of the file    break;
   {
		m_EndOfFrame = TRUE;
   }
   else
   {
	  if( m_PlayFlag )
		BinkNextFrame( m_BinkObject );
   }

  //End_timer( Bink_microseconds );
}

#endif //_ACCLAIM_IGAADSYSTEM