// FSControl.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "fscontrol.h"
#include "FlashControlImpl.h"

#include "version.h"

#include "global.h"
#include "stags.h"
#include "sobject.h"
#include "memcop.h"
#include "edittext.h"
#include "stdio.h"

#include "mcontext.h"
#include "util.h"

#include "Package.h"

// Test defines - for file streaming.
// #define KILOBYTESPERSEC		4			// How many kbytes per sec - set to 0 for fastest
// #define SIMULATE_STREAMING				// When defined, loads files piece by piece on seperate thread,
										// when not defined, loads the entire file from disk

struct ThreadData
{
	HWND		hwnd;
	FILE*		fp;
	void*		notifyData;
	char*		url;
	StreamData	streamData;
};

#define MAX_LOADSTRING 100
#define TIMER_PLAY		1
#define TIMER_CURSOR	2


NativePlayerWnd *flashWin = NULL;

/////////////////////////////////////////////////////////////////////////////
// CFlashStatic

//-----------------------------------------------------------------------------
// So much for the Windows stuff. NativePlayerWnd follows.
//-----------------------------------------------------------------------------

NativePlayerWnd::NativePlayerWnd()
{
	currentURL = CreateStr( "" );
	hwnd = 0;
}

NativePlayerWnd::~NativePlayerWnd()
{
	ClearScript();
	delete currentURL;
}

void NativePlayerWnd::initialize( HWND _hwnd )
{
	hwnd = _hwnd;

	cursorArrow		= LoadCursor( NULL, IDC_ARROW );
//	cursorHand		= LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE(FID_HAND) );
//	cursorButton	= LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE(FID_BUTTON) );
	cursorHand		= LoadCursor( NULL, MAKEINTRESOURCE(IDC_HELP));
	cursorButton	= LoadCursor( NULL, MAKEINTRESOURCE(IDC_CROSS) );
	cursorBeam		= LoadCursor( NULL, IDC_IBEAM );
}

void NativePlayerWnd::PlayTimerFire()
{
	DoPlay( true );
}

void NativePlayerWnd::CursorTimerFire()
{
	#ifdef EDITTEXT
	BlinkCursor();
	#endif
}

void NativePlayerWnd::ClientRect( SRECT* rect )
{
	RECT r;

	if ( hwnd )
	{
		GetClientRect( hwnd, &r );

		rect->xmin = 0;
		rect->xmax = r.right - r.left;
		rect->ymin = 0;
		rect->ymax = r.bottom - r.top;
	}
	else
	{
		memset( rect, 0, sizeof( SRECT ) );
	}
	FLASHASSERT( r.bottom >= r.top );
}

void NativePlayerWnd::InvalidateScreenArea( SRECT* )
{
	// the simple solution is too send a repaint for the whole thing
	InvalidateRect( hwnd, NULL, false );
}

void NativePlayerWnd::AdjustWindow( int width, int height )
{	
	#ifdef FIXEDSIZE
		// Then we don't adjust our size.
	#else
		SetWindowPos( hwnd, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE );
	#endif
}

BOOL NativePlayerWnd::StartTimer( int playTimerInterval, int cursorTimerInterval )
{
	if ( !loaded ) 
		return false;

	FLASHASSERT( playTimerInterval );
	FLASHASSERT( cursorTimerInterval );
	
	playTimer = SetTimer( hwnd, TIMER_PLAY, playTimerInterval, 0 );
	cursorTimer = SetTimer( hwnd, TIMER_CURSOR, cursorTimerInterval, 0 );

	return true;	
}

void NativePlayerWnd::StopTimer()
{
	KillTimer( hwnd, playTimer );
	KillTimer( hwnd, cursorTimer );
}

void NativePlayerWnd::EnableMenus( const MenuStatus& menuStatus )
{
	HMENU menu = GetMenu( hwnd );
/*	EnableMenuItem(menu, IDM_100, menuStatus.view100percent ? MF_ENABLED : MF_GRAYED);
	CheckMenuItem(menu, IDM_100, menuStatus.view100Check ? MF_CHECKED : MF_UNCHECKED);

	EnableMenuItem(menu, IDM_PLAY, menuStatus.play ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(menu, IDM_REWIND, menuStatus.rewind ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(menu, IDM_FORWARD, menuStatus.forward ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(menu, IDM_BACK, menuStatus.back ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(menu, IDM_LOOP, menuStatus.loop ? MF_ENABLED : MF_GRAYED);

	CheckMenuItem(menu, IDM_PLAY, menuStatus.playCheck ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, IDM_LOOP, menuStatus.loopCheck ? MF_CHECKED : MF_UNCHECKED);

 	EnableMenuItem(menu, IDM_VIEWALL, menuStatus.viewAll ? MF_ENABLED : MF_GRAYED);
	CheckMenuItem(menu, IDM_VIEWALL,  menuStatus.viewAllCheck ? MF_CHECKED : MF_UNCHECKED);

	EnableMenuItem(menu, IDM_ZOOMIN, menuStatus.zoomIn ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(menu, IDM_ZOOMOUT, menuStatus.zoomOut ? MF_ENABLED : MF_GRAYED);

	CheckMenuItem(menu, IDM_HIGHQ, menuStatus.highQChecked ? MF_CHECKED : MF_UNCHECKED);
*/
}


#ifdef DEBUG
void DumpTree(SObject* root, int depth)
{
	for ( int i = 0; i < depth; i++ )
		FLASHOUTPUT(" ");
	FLASHOUTPUT("Type=%i Name=%s addr=%X\n", root->character ? root->character->type : -1, root->name ? root->name : "", root);
	for ( SObject* obj = root->bottomChild; obj; obj = obj->above )
		DumpTree(obj, depth+1);
}
#endif


void NativePlayerWnd::ProcessFSCommand( char* cmd, char* args )
{
	if ( StrEqual(cmd, "quit") )
	{
		// Quit
		ExitApp();
	}
	else if ( StrEqual(cmd, "fullscreen") )
	{
		SetFullScreen( IsTrue(args) != 0 );
	}
	else if ( StrEqual(cmd, "allowscale") )
	{
		SetScaleMode( IsTrue(args) );
	}
	else if ( StrEqual(cmd, "exec") )
	{
		// make sure we don't pass any parameters 
		// to the app we want to spawn. We don't want
		// anybody to do "del *.*"
		int     len = strlen(args) + 1;
		char    *tmpString = new char[len];

		if ( tmpString == 0 )
			return;

		for ( int i = 0; i < len; i++ )
		{
			tmpString[i] = args[i];
			if ( tmpString[i] == ' ' )
			{
				tmpString[i] = 0;
				break;
			}
		}
		ExecSystem( tmpString );
		delete [] tmpString;
	}
	else if ( StrEqual(cmd, "showmenu") )
	{
		// Toggle the full screen flags
		showMenu = IsTrue(args);
		HMENU hMenu = GetMenu( hwnd );

		if (!showMenu) 
		{
			if (hMenu) 
			{
				// Save and clear the menu bar
				savedMenu = hMenu;
				SetMenu( hwnd, 0);
			}
		} 
		else 
		{
			SetMenu( hwnd, savedMenu );
		}
	}
}

BOOL NativePlayerWnd::UpdateCursor()
{
	int type = GetCursorType();

	switch( type )
	{
		case CURSOR_TYPE_ARROW:
			SetCursor( cursorArrow );
			return true;

		case CURSOR_TYPE_HAND:
			SetCursor( cursorHand );
			return true;

		case CURSOR_TYPE_BUTTON:
			SetCursor( cursorButton );
			return true;

		case CURSOR_TYPE_BEAM:
			SetCursor( cursorBeam );
			return true;
	}
	return false;
}

#ifdef EDITTEXT
#ifdef EDITTEXT_CLIPBOARD_WIDE

// ------Support for Unicode-------

U16* NativePlayerWnd::GetStringFromClipboard()
{
	const U16* text;
	U16* ret = 0;

	if ( OpenClipboard( 0 ) ) 
	{
		text = (U16*) GetClipboardData( CF_UNICODETEXT );
		if ( text )
		{
			ret = new U16[ WideStrLen( text ) + 1 ];

			// windows uses /r/n, while Flash uses /r...sigh
			// take out the /n
			const U16* p;
			U16* q;
			for( p = text, q = ret; *p; p++ )
			{
				if ( *p != '\n' )
					*q++ = *p;
			}
			*q = 0;
		}
		CloseClipboard();
	}
	return ret;
}

void NativePlayerWnd::SetStringToClipboard( const U16* text )
{
	if ( text && *text && OpenClipboard( 0 ) ) 
	{
		HGLOBAL globalMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, ( WideStrLen( text ) + 1 ) * 2 );
		U16* globalText = (U16*) GlobalLock( globalMem );
		if ( globalText )
		{
			WideStrCopy( globalText, text );
			GlobalUnlock( globalMem );
			EmptyClipboard();
			SetClipboardData( CF_UNICODETEXT, globalText );
		}
		CloseClipboard();
	}
}

#else

// ------Support for ASCII-------

char* NativePlayerWnd::GetStringFromClipboard()
{
	const char* text;
	char* ret = 0;

	if ( OpenClipboard( 0 ) ) 
	{
		text = (char*) GetClipboardData( CF_TEXT );
		if ( text )
		{
			ret = new char[ strlen( text ) + 1 ];

			// windows uses /r/n, while Flash uses /r...sigh
			// take out the /n
			const char* p;
			char* q;
			for( p = text, q = ret; *p; p++ )
			{
				if ( *p != '\n' )
					*q++ = *p;
			}
			*q = 0;
		}
		CloseClipboard();
	}
	return ret;
}

void NativePlayerWnd::SetStringToClipboard( const char* text )
{
	if ( text && *text && OpenClipboard( 0 ) ) 
	{
		HGLOBAL globalMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, ( strlen( text ) + 1 ) * 2 );
		char* globalText = (char*) GlobalLock( globalMem );
		if ( globalText )
		{
			strcpy( globalText, text );
			GlobalUnlock( globalMem );
			EmptyClipboard();
			SetClipboardData( CF_TEXT, globalText );
		}
		CloseClipboard();
	}
}

#endif
#endif

HDC NativePlayerWnd::GetNWindowDC()
{
	FLASHASSERT( windowDC );
	return windowDC;
}

void NativePlayerWnd::CloseStream( StreamData* streamData )
{
	// The quick and easy, again. Doesn't actually stop the stream, just prevents
	// future writes.
	streamData->scriptPlayer = 0;
}

void NativePlayerWnd::StreamGetURLNotify(	const char* url,
											const char* window,
											void*		notifyData )
{
	// The quick and easy solution - support for file loading only. But, in order
	// to bring some aura of respectability to this demo, it will be implemented
	// on a thread and simulating a network connection.

	// Remove the file: prefix
	const char* fname;
	char* cmd = SkipPrefix( (char*) url, "file://");
	if ( cmd )
		fname = cmd;
	else
		fname = url;
	
	/*
	// We now have an fname that may or may not be a valid file. Try to open it:

	FILE* fp = fopen( url, "rb" );
	if ( fp )
	{
		#ifdef SIMULATE_STREAMING
			DWORD id;
			ThreadData* data = new ThreadData;

			data->fp = fp;
			data->notifyData = notifyData;
			data->hwnd = hwnd; 
			data->url = CreateStr( (char*)url );

			CreateThread( 0, 0, StreamThread, data, 0, &id );
		#else
			StreamData streamData;
			char block[256];
			int  read;

			StreamInNew( &streamData, (char*) url, notifyData );
			while ( read = fread( block, 1, 256, fp ) )
			{
				StreamInWrite( &streamData, &block, read );
			}
			StreamInDestroy( &streamData );
		#endif
	}
	*/

	CPackage package;
	if( !package.Open( _T("XLauncher.xp") ) )
	{
		AfxMessageBox( _T("ERROR : Can't open XLauncher.xp package file") );
		return;
	}

	TCHAR widestring[1024];
	memset( widestring, 0, sizeof(TCHAR) * 1024 );
	int requirelength = MultiByteToWideChar(CP_ACP, 0, url, strlen(url), widestring, 1024);
	
	FILE* fp = package.GetPackedFile( widestring );
	if ( fp )
	{
#ifdef SIMULATE_STREAMING
		DWORD id;
		ThreadData* data = new ThreadData;
		
		data->fp = fp;
		data->notifyData = notifyData;
		data->hwnd = hwnd; 
		data->url = CreateStr( (char*)url );
		
		CreateThread( 0, 0, StreamThread, data, 0, &id );
#else
		StreamData streamData;
		char block[256];
		int  read;
		
		StreamInNew( &streamData, (char*) url, notifyData );
		while ( read = fread( block, 1, 256, fp ) )
		{
			StreamInWrite( &streamData, &block, read );
		}
		StreamInDestroy( &streamData );
#endif
	}	

	package.Close();
}

#ifdef SIMULATE_STREAMING
DWORD WINAPI StreamThread(  LPVOID lpParameter )
{
	ThreadData* data = (ThreadData*) lpParameter;

	int  delay;
	int  kilobytes = KILOBYTESPERSEC;

	if ( kilobytes > 0 )
		delay = 250 / kilobytes;
	else
		delay = 0;

	// put in some arbitrary delays
	SleepEx( delay*2, false );
	PostMessage( data->hwnd, MSG_STREAM_NEW, (DWORD) data, (DWORD) data->notifyData );

	SleepEx( delay*2, false );

	char block[256];
	int  read;
	
	while ( read = fread( block, 1, 256, data->fp ) )
	{
		WriteBlock* write = new WriteBlock;
		memcpy( write->data, block, read );
		write->size = read;

		PostMessage( data->hwnd, MSG_STREAM_WRITE, (DWORD) data, (DWORD) write );
		SleepEx( delay, false );
	}
	PostMessage( data->hwnd, MSG_STREAM_DESTROY, (DWORD) data, 0 );

	return 0;
}
#endif 


